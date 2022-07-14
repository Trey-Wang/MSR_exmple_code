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
 *         File:  Rtp.c
 *       Module:  Tp_Rtp
 *    Generator:  Tp_Rtp.jar (DaVinci Configurator Pro)
 *
 *  Description:  Implementation of Realtime Transport Protocol (RTP)
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

#define RTP_SOURCE

/*lint -e438 -e550 */ /* Return values can be used to gain debug information */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Rtp.h"
#include "Rtp_Priv.h"
#include "Rtp_Cbk.h"
#include "Rtp_Types.h"
#include "IpBase.h"

#if(RTP_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif /* RTP_DEV_ERROR_REPORT == STD_ON */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of Rtp header file */
#if(   (RTP_SW_MAJOR_VERSION != (0x01u)) \
    || (RTP_SW_MINOR_VERSION != (0x01u)) \
    || (RTP_SW_PATCH_VERSION != (0x00u)) )
# error "Vendor specific version numbers of Rtp.c and Rtp.h are inconsistent"
#endif /* Version Check */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (RTP_LOCAL)
# define RTP_LOCAL static
#endif /* RTP_LOCAL */

#if !defined (RTP_LOCAL_INLINE)
# define RTP_LOCAL_INLINE LOCAL_INLINE
#endif /* RTP_LOCAL_INLINE */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define RTP_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if(RTP_DEV_ERROR_DETECT == STD_ON)
RTP_LOCAL VAR(Rtp_StateType, RTP_VAR_ZERO_INIT) Rtp_ModuleInitialized = RTP_UNINIT;
#endif /* RTP_DEV_ERROR_DETECT == STD_ON */

#define RTP_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define RTP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if(RTP_TXSTREAM == STD_ON)
/**********************************************************************************************************************
 *  Rtp_SerializeHeader()
 *********************************************************************************************************************/
/*! \brief        Serialize RTP header.
 *  \details      If given buffer is not large enough, RTP serializes buffer segment and proceeds during the next call.
 *  \param[in]    TxStreamIdx     Index of the relevant Tx stream
 *  \param[in]    PduInfoPtr      Contains the length (SduLength) of the received I-PDU and a pointer to
 *                                a buffer (SduDataPtr) containing the I-PDU.
 *  \return       Amount of serialized header bytes.
 *  \pre          none
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
RTP_LOCAL FUNC(uint16, RTP_CODE) Rtp_SerializeHeader(
  Rtp_TxStreamIdxOfStreamType TxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  Rtp_RtpTriggerTransmit()
 *********************************************************************************************************************/
/*! \brief          Trigger RTP to copy its payload to the provided buffer.
 *  \details        Specialized function of Rtp_SoAdTriggerTransmit().
 *  \param[in]      TxStreamIdx   Index of the relevant Tx stream
 *  \param[in/out]  PduInfoPtr    Contains the length (SduLength) of the received I-PDU and a pointer to
 *                                a buffer (SduDataPtr) containing the I-PDU.
 *  \return         E_OK          RTP payload successfully copied.
 *                  E_NOT_OK      No pending trigger transmit or not able to request data from upper layer.
 *  \pre            none
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_RtpTriggerTransmit(
  Rtp_TxStreamIdxOfStreamType TxStreamIdx,
  P2VAR(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  Rtp_RtcpTriggerTransmit()
 *********************************************************************************************************************/
/*! \brief          Trigger RTCP to copy its payload to the provided buffer.
 *  \details        Specialized function of Rtp_SoAdTriggerTransmit().
 *  \param[in]      TxStreamIdx     Index of the relevant Tx stream
 *  \param[in/out]  PduInfoPtr      Contains the length (SduLength) of the received I-PDU and a pointer to
 *                                  a buffer (SduDataPtr) containing the I-PDU.
 *  \return         E_OK            All compound packets successfully serialized.
 *                  E_NOT_OK        Not able to serialize all compound packets.
 *  \pre            none
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_RtcpTriggerTransmit(
  Rtp_TxStreamIdxOfStreamType TxStreamIdx,
  P2VAR(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  Rtp_RtcpSerializeEmptyRr()
 *********************************************************************************************************************/
/*! \brief          Serialize empty Receiver Report (RR) to the buffer.
 *  \details        Each RTCP message requires a SR or RR.
 *  \param[in]      TxStreamIdx     Index of the relevant Tx stream
 *  \param[in]      PduInfoPtr      Contains the length (SduLength) of the received I-PDU and a pointer to
 *                                  a buffer (SduDataPtr) containing the I-PDU.
 *  \param[in/out]  PayloadIdxPtr   Indexes the next free buffer element.
 *  \return         E_OK            RR packet successfully serialized.
 *                  E_NOT_OK        Buffer too small for RR packet.
 *  \pre            none
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_RtcpSerializeEmptyRr(
  Rtp_TxStreamIdxOfStreamType TxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, RTP_APPL_DATA) PayloadIdxPtr);

/**********************************************************************************************************************
 *  Rtp_RtcpSerializeBye()
 *********************************************************************************************************************/
/*! \brief          Serialize BYE packet to the buffer.
 *  \details        Indicates that a used SSRC is not valid anymore.
 *  \param[in]      TxStreamIdx     Index of the relevant Tx stream
 *  \param[in]      PduInfoPtr      Contains the length (SduLength) of the received I-PDU and a pointer to
 *                                  a buffer (SduDataPtr) containing the I-PDU.
 *  \param[in/out]  PayloadIdxPtr   Indexes the next free buffer element.
 *  \return         E_OK            BYE packet successfully serialized.
 *                  E_NOT_OK        Buffer too small for BYE packet.
 *  \pre            none
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_RtcpSerializeBye(
  Rtp_TxStreamIdxOfStreamType TxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, RTP_APPL_DATA) PayloadIdxPtr);

/**********************************************************************************************************************
 *  Rtp_RtcpSerializeSdes()
 *********************************************************************************************************************/
/*! \brief          Serialize SDES packet to the buffer.
 *  \details        Contains sender information.
 *  \param[in]      TxStreamIdx     Index of the relevant Tx stream
 *  \param[in]      PduInfoPtr      Contains the length (SduLength) of the received I-PDU and a pointer to
 *                                  a buffer (SduDataPtr) containing the I-PDU.
 *  \param[in/out]  PayloadIdxPtr   Indexes the next free buffer element.
 *  \return         E_OK            SDES packet successfully serialized.
 *                  E_NOT_OK        Buffer too small for SDES packet.
 *  \pre            none
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_RtcpSerializeSdes(
  Rtp_TxStreamIdxOfStreamType TxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, RTP_APPL_DATA) PayloadIdxPtr);

/**********************************************************************************************************************
 *  Rtp_RtcpSerializeAvbRtcp()
 *********************************************************************************************************************/
/*! \brief          Serialize AVB RTCP packet to the buffer.
 *  \details        Allows the synchronization or RTP and global time.
 *  \param[in]      TxStreamIdx     Index of the relevant Tx stream
 *  \param[in]      PduInfoPtr      Contains the length (SduLength) of the received I-PDU and a pointer to
 *                                  a buffer (SduDataPtr) containing the I-PDU.
 *  \param[in/out]  PayloadIdxPtr   Indexes the next free buffer element.
 *  \return         E_OK            AVB RTCP packet successfully serialized.
 *                  E_NOT_OK        Buffer too small for AVB RTCP packet.
 *  \pre            none
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_RtcpSerializeAvbRtcp(
  Rtp_TxStreamIdxOfStreamType TxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, RTP_APPL_DATA) PayloadIdxPtr);
#endif /* RTP_TXSTREAM == STD_ON */

#if(RTP_RXSTREAM == STD_ON)
/**********************************************************************************************************************
 *  Rtp_ReadRtpHeaderInformation()
 *********************************************************************************************************************/
/*! \brief        Read RTP header information.
 *  \details      Data is saved in the structures.
 *  \param[in]    PduInfoPtr      Contains the length (SduLength) of the received I-PDU and a pointer to
 *                                a buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]    AddHeaderPtr    Additional header information not forwarded to the upper layer.
 *  \param[in]    HeaderInfoPtr   Header information forwarded to the upper layer.
 *  \return       E_OK            Header successfully read.
 *                E_NOT_OK        Buffer too small to contain entire header.
 *  \pre          none
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_ReadRtpHeaderInformation(
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  P2VAR(Rtp_AddRtpHeaderType, AUTOMATIC, RTP_APPL_DATA) AddHeaderPtr,
  P2VAR(Rtp_RxRtpInfoType, AUTOMATIC, RTP_APPL_DATA)  HeaderInfoPtr);

/**********************************************************************************************************************
 *  Rtp_RtpRxIndication()
 *********************************************************************************************************************/
/*! \brief          Process received RTP message.
 *  \details        Specialized function of Rtp_SoAdRxIndication().
 *  \param[in]      RxStreamIdx     Index of the relevant Rx stream
 *  \param[in]      PduInfoPtr      Contains the length (SduLength) of the received I-PDU and a pointer to
 *                                  a buffer (SduDataPtr) containing the I-PDU.
 *  \return         void
 *  \pre            none
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
RTP_LOCAL FUNC(void, RTP_CODE) Rtp_RtpRxIndication(
  Rtp_RxStreamIdxOfStreamType RxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  Rtp_ReadRtcpHeaderInformation()
 *********************************************************************************************************************/
/*! \brief        Read RTCP header information.
 *  \details      Data is saved in the structures.
 *  \param[in]    PduInfoPtr      Contains the length (SduLength) of the received I-PDU and a pointer to
 *                                a buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]    StartIdx        Payload start index of the header.
 *  \param[out]   RtcpHeaderPtr   RTP header information.
 *  \return       E_OK            Header successfully read.
 *                E_NOT_OK        Buffer too small to contain entire header.
 *  \pre          none
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_ReadRtcpHeaderInformation(
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  uint16 StartIdx,
  P2VAR(Rtp_RtcpHeaderType, AUTOMATIC, RTP_APPL_DATA) RtcpHeaderPtr);

/**********************************************************************************************************************
 *  Rtp_ProcessRtcpAvbPacket()
 *********************************************************************************************************************/
/*! \brief          Process received AVB RTCP packet.
 *  \details        Data will be forwarded to the upper layer.
 *  \param[in]      RxStreamIdx   Index of the relevant Rx stream
 *  \param[in]      PduInfoPtr    Contains the length (SduLength) of the received I-PDU and a pointer to
 *                                a buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      StartIdx      Payload start index of the header.
 *  \param[in]      Length        Length given in the RTP header.
 *  \return         E_OK          AVB RTCP packet successfully read.
 *                  E_NOT_OK      Buffer too small to contain entire packet.
 *  \pre            none
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_ProcessRtcpAvbPacket(
  Rtp_RxStreamIdxOfStreamType RxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  uint16 StartIdx,
  uint16 Length);

# if(RTP_ULRTCPSDESCBKOFRXSTREAM == STD_ON)
/**********************************************************************************************************************
 *  Rtp_ProcessRtcpSdesPacket()
 *********************************************************************************************************************/
/*! \brief          Process received SDES packet.
 *  \details        Data will be forwarded to the upper layer if callback is configured.
 *  \param[in]      RxStreamIdx   Index of the relevant Rx stream
 *  \param[in]      PduInfoPtr    Contains the length (SduLength) of the received I-PDU and a pointer to
 *                                a buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      StartIdx      Payload start index of the header.
 *  \param[in]      Length        Length given in the RTP header.
 *  \return         E_OK          SDES packet successfully read.
 *                  E_NOT_OK      Buffer too small to contain entire packet.
 *  \pre            none
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_ProcessRtcpSdesPacket(
  Rtp_RxStreamIdxOfStreamType RxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  uint16 StartIdx,
  uint16 Length);
# endif /* RTP_ULRTCPSDESCBKOFRXSTREAM == STD_ON */

/**********************************************************************************************************************
 *  Rtp_ProcessRtcpReportPacket()
 *********************************************************************************************************************/
/*! \brief          Process received Report packet.
 *  \details        SSRC will be read in order to handle SSRC collision.
 *  \param[in]      PduInfoPtr    Contains the length (SduLength) of the received I-PDU and a pointer to
 *                                a buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      StartIdx      Payload start index of the header.
 *  \param[in]      Length        Length given in the RTP header.
 *  \return         E_OK          Report packet successfully read.
 *                  E_NOT_OK      Buffer too small to contain entire packet.
 *  \pre            none
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_ProcessRtcpReportPacket(
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  uint16 StartIdx,
  uint16 Length);

/**********************************************************************************************************************
 *  Rtp_RtcpRxIndication()
 *********************************************************************************************************************/
/*! \brief          Process received RTCP message.
 *  \details        Specialized function of Rtp_SoAdRxIndication().
 *  \param[in]      RxStreamIdx     Index of the relevant Rx stream
 *  \param[in]      PduInfoPtr      Contains the length (SduLength) of the received I-PDU and a pointer to
 *                                  a buffer (SduDataPtr) containing the I-PDU.
 *  \return         void
 *  \pre            none
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
RTP_LOCAL FUNC(void, RTP_CODE) Rtp_RtcpRxIndication(
  Rtp_RxStreamIdxOfStreamType RxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  Rtp_HandleSsrcCollision()
 *********************************************************************************************************************/
/*! \brief          Check if received SSRC is also used by canfigured Tx stream.
 *  \details        If a collision is detected, the internal SSRC will be renewed.
 *  \param[in]      RxSsrc
 *  \return         void
 *  \pre            none
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace CREQ-116496
 *********************************************************************************************************************/
RTP_LOCAL FUNC(void, RTP_CODE) Rtp_HandleSsrcCollision(
  uint32 RxSsrc);
#endif /* RTP_RXSTREAM == STD_ON */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if(RTP_TXSTREAM == STD_ON)
/**********************************************************************************************************************
 *  Rtp_SerializeHeader()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
RTP_LOCAL FUNC(uint16, RTP_CODE) Rtp_SerializeHeader(
  Rtp_TxStreamIdxOfStreamType TxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 headerFlags = RTP_GENERAL_HEADER_FLAGS;
  uint8 bytesToCopy;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Build header flags of RTP message */
  if(Rtp_IsPaddingFlagOfDynamicTxStreamData(TxStreamIdx))
  {
    headerFlags |= (uint16)((uint16)(1U) << RTP_HEADERFLAG_BITOFFSET_PADDING);
  }

  if(Rtp_IsMarkerFlagOfDynamicTxStreamData(TxStreamIdx))
  {
    headerFlags |= (uint16)((uint16)(1U)  << RTP_HEADERFLAG_BITOFFSET_MARKER);
  }

  headerFlags |= (uint16)(Rtp_GetTypeOfStream(Rtp_GetStreamIdxOfTxStream(TxStreamIdx) & RTP_HEADERFLAG_PAYLOAD_TYPE));

  /* #20 Check available buffer size and serialize entire header or header segment to buffer */
  if((PduInfoPtr->SduLength >= RTP_HEADER_SIZE) && (Rtp_GetCopiedTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx) == 0))
  {
    /* Serialize entire header */
    RTP_PUT_UINT16(PduInfoPtr->SduDataPtr,
      RTP_HEADER_OFFSET_FLAGS,
      headerFlags);

    RTP_PUT_UINT16(PduInfoPtr->SduDataPtr,
      RTP_HEADER_OFFSET_SEQUENCENUMBER,
      Rtp_GetSequenceNumberOfDynamicTxStreamData(TxStreamIdx));

    RTP_PUT_UINT32(PduInfoPtr->SduDataPtr,
      RTP_HEADER_OFFSET_TIMESTAMP,
      Rtp_GetRtpTimestampOfDynamicTxStreamData(TxStreamIdx));

    RTP_PUT_UINT32(PduInfoPtr->SduDataPtr,
      RTP_HEADER_OFFSET_SSRC,
      Rtp_GetSsrcOfDynamicStreamData(Rtp_GetStreamIdxOfTxStream(TxStreamIdx)));

    bytesToCopy = RTP_HEADER_SIZE;
  }
  else
  {
    /* Serialize header segment */
    uint8 tmpHeader[RTP_HEADER_SIZE];
    uint8 tmpHeaderIdx = 0;

    bytesToCopy = (uint8)(RTP_HEADER_SIZE - Rtp_GetCopiedTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx));

    if(bytesToCopy > PduInfoPtr->SduLength)
    {
      bytesToCopy = (uint8)PduInfoPtr->SduLength;
    }

    RTP_PUT_UINT16(tmpHeader,
      RTP_HEADER_OFFSET_FLAGS,
      headerFlags);

    RTP_PUT_UINT16(tmpHeader,
      RTP_HEADER_OFFSET_SEQUENCENUMBER,
      Rtp_GetSequenceNumberOfDynamicTxStreamData(TxStreamIdx));

    RTP_PUT_UINT32(tmpHeader,
      RTP_HEADER_OFFSET_TIMESTAMP,
      Rtp_GetRtpTimestampOfDynamicTxStreamData(TxStreamIdx));

    RTP_PUT_UINT32(tmpHeader,
      RTP_HEADER_OFFSET_SSRC,
      Rtp_GetSsrcOfDynamicStreamData(Rtp_GetStreamIdxOfTxStream(TxStreamIdx)));

    /* Copy relevant segment to buffer */
    while(tmpHeaderIdx < bytesToCopy)
    {
      PduInfoPtr->SduDataPtr[tmpHeaderIdx] = tmpHeader[tmpHeaderIdx + Rtp_GetCopiedTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx)];
      tmpHeaderIdx++;
    }
  }

  Rtp_SetCopiedTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx, Rtp_GetCopiedTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx) + bytesToCopy);
  Rtp_SetPendingTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx, Rtp_GetPendingTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx) - bytesToCopy);

  /* #30 Increment sequence number if header was totally copied */
  if(Rtp_GetCopiedTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx) == RTP_HEADER_SIZE)
  {
    Rtp_IncSequenceNumberOfDynamicTxStreamData(TxStreamIdx);
  }

  return bytesToCopy;
} /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */

/**********************************************************************************************************************
 *  Rtp_RtpTriggerTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_RtpTriggerTransmit(
  Rtp_TxStreamIdxOfStreamType TxStreamIdx,
  P2VAR(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if there is a pending TriggerTransmit callback for this stream */
  if(Rtp_GetPendingTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx) == 0)
  {
    /* No pending copy TX data */
    PduInfoPtr->SduLength = 0;
    retVal = E_NOT_OK;
  }
  else
  {
    uint16 tmpCopiedBytes = 0;

    /* #20 Serialize header if not already serialized within previous TriggerTransmit call */
    if(Rtp_GetCopiedTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx) < RTP_HEADER_SIZE)
    {
      /* Serialize Rtp Header */
      tmpCopiedBytes = Rtp_SerializeHeader(TxStreamIdx, PduInfoPtr);
    }

    if(tmpCopiedBytes < PduInfoPtr->SduLength)
    {
      /* #30 Check if data has to be copied by RTP or by UpperLayer */
# if(RTP_ULCOPYTXDATACBKOFTXSTREAM == STD_ON)
      if(Rtp_GetPayloadPtrOfDynamicTxStreamData(TxStreamIdx) == NULL_PTR)
      {
        PduLengthType tmpLength = PduInfoPtr->SduLength - tmpCopiedBytes;
        /* #40 Trigger UpperLayer to copy data to the buffer */
        retVal = Rtp_GetUlCopyTxDataCbkOfTxStream(TxStreamIdx)(
          (uint16)Rtp_GetStreamIdxOfTxStream(TxStreamIdx),
          &PduInfoPtr->SduDataPtr[tmpCopiedBytes],
          &tmpLength);

          PduInfoPtr->SduLength = tmpCopiedBytes + tmpLength;
      }
      else
# endif /* RTP_ULCOPYTXDATACBKOFTXSTREAM == STD_ON */
      {
        /* Calculate amount of data to copy */
        PduLengthType bytesToCopy = PduInfoPtr->SduLength - tmpCopiedBytes;

        if(bytesToCopy > Rtp_GetPendingTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx))
        {
          bytesToCopy = Rtp_GetPendingTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx);
        }

        /* #50 Copy data to the buffer */
        IpBase_Copy(
          &PduInfoPtr->SduDataPtr[tmpCopiedBytes],
          &Rtp_GetPayloadPtrOfDynamicTxStreamData(TxStreamIdx)[Rtp_GetCopiedTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx) - RTP_HEADER_SIZE],
          bytesToCopy);

          PduInfoPtr->SduLength = tmpCopiedBytes + bytesToCopy;
      }

      Rtp_SetCopiedTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx,
        Rtp_GetCopiedTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx) + PduInfoPtr->SduLength);
      Rtp_SetPendingTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx,
        Rtp_GetPendingTxCallbackBytesOfDynamicTxStreamData(TxStreamIdx) - PduInfoPtr->SduLength);
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Rtp_RtcpTriggerTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_RtcpTriggerTransmit(
  Rtp_TxStreamIdxOfStreamType TxStreamIdx,
  P2VAR(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint16 payloadIdx = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Serialize compound RTCP packet */

  /* #20 Serialize empty Receiver Report (RR) packet */
  retVal |= Rtp_RtcpSerializeEmptyRr(TxStreamIdx, PduInfoPtr, &payloadIdx);

  /* #30 Serialize a BYE packet if an SSRC collision was detected or the stream was disabled */
  if(Rtp_IsTransmitByeOfDynamicTxStreamData(TxStreamIdx))
  {
    /* Serialize BYE packet */
    retVal |= Rtp_RtcpSerializeBye(TxStreamIdx, PduInfoPtr, &payloadIdx);
  }
  /* #40 Serialize an SDES and AVB RTCP packet in case of normal processing */
  else
  {
    /* Serialize SDES packet */
    retVal |= Rtp_RtcpSerializeSdes(TxStreamIdx, PduInfoPtr, &payloadIdx);

    /* Serialize AVB RTCP packet */
    retVal |= Rtp_RtcpSerializeAvbRtcp(TxStreamIdx, PduInfoPtr, &payloadIdx);
  }

  PduInfoPtr->SduLength = payloadIdx;

  return retVal;
}

/**********************************************************************************************************************
 *  Rtp_RtcpSerializeEmptyRr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_RtcpSerializeEmptyRr(
  Rtp_TxStreamIdxOfStreamType TxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, RTP_APPL_DATA) PayloadIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the RR message can be serialized into the remaining buffer */
  if((*PayloadIdxPtr + RTP_RTCP_EMPTYRR_PACKET_SIZE) > PduInfoPtr->SduLength)
  {
    retVal = E_NOT_OK;
  }
  else
  {
    /* #20 Serialize empty Receiver Report (RR) compound packet */
    RTP_PUT_UINT8(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_EMPTYRR_HEADER_FIXED_VPRC_OFFSET,
      RTP_RTCP_EMPTYRR_HEADER_FIXED_VPRC);

    RTP_PUT_UINT8(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_EMPTYRR_HEADER_FIXED_TYPE_OFFSET,
      RTP_RTCP_EMPTYRR_HEADER_FIXED_TYPE);

    RTP_PUT_UINT8(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_EMPTYRR_HEADER_FIXED_LENGTH_HB_OFFSET,
      RTP_RTCP_EMPTYRR_HEADER_FIXED_LENGTH_HB);

    RTP_PUT_UINT8(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_EMPTYRR_HEADER_FIXED_LENGTH_LB_OFFSET,
      RTP_RTCP_EMPTYRR_HEADER_FIXED_LENGTH_LB);

    RTP_PUT_UINT32(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_BYE_SSRC_OFFSET,
      Rtp_GetSsrcOfDynamicStreamData(Rtp_GetStreamIdxOfTxStream(TxStreamIdx)));

    *PayloadIdxPtr += RTP_RTCP_EMPTYRR_PACKET_SIZE;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Rtp_RtcpSerializeBye()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_RtcpSerializeBye(
  Rtp_TxStreamIdxOfStreamType TxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, RTP_APPL_DATA) PayloadIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the BYE message can be serialized into the remaining buffer */
  if((*PayloadIdxPtr + RTP_RTCP_BYE_PACKET_SIZE) > PduInfoPtr->SduLength)
  {
    retVal = E_NOT_OK;
  }
  else
  {
    /* #20 Serialize BYE compound packet */
    RTP_PUT_UINT8(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_BYE_HEADER_FIXED_VPSC_OFFSET,
      RTP_RTCP_BYE_HEADER_FIXED_VPSC);

    RTP_PUT_UINT8(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_BYE_HEADER_FIXED_TYPE_OFFSET,
      RTP_RTCP_BYE_HEADER_FIXED_TYPE);

    RTP_PUT_UINT8(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_BYE_HEADER_FIXED_LENGTH_HB_OFFSET,
      RTP_RTCP_BYE_HEADER_FIXED_LENGTH_HB);

    RTP_PUT_UINT8(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_BYE_HEADER_FIXED_LENGTH_LB_OFFSET,
      RTP_RTCP_BYE_HEADER_FIXED_LENGTH_LB);

    RTP_PUT_UINT32(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_BYE_SSRC_OFFSET,
      Rtp_GetSsrcOfDynamicStreamData(Rtp_GetStreamIdxOfTxStream(TxStreamIdx)));

    *PayloadIdxPtr += RTP_RTCP_BYE_PACKET_SIZE;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Rtp_RtcpSerializeSdes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_RtcpSerializeSdes(
  Rtp_TxStreamIdxOfStreamType TxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, RTP_APPL_DATA) PayloadIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint16 flags = RTP_RTCP_SDES_HEADER_FIXED_16BITWORD, cNameLength, sdesLength;
  uint8 chunkPadding;

  /* ----- Implementation ----------------------------------------------- */
  cNameLength = (uint16)(Rtp_GetCNameEndIdxOfTxStream(TxStreamIdx) - Rtp_GetCNameStartIdxOfTxStream(TxStreamIdx));

  /* Calculate mandatory amount of padding for this SDES chunk */
  chunkPadding = (uint8)(4 - ((RTP_RTCP_SDES_ITEM_TYPE_AND_LENGTH_SIZE + cNameLength) % 4));

  sdesLength = RTP_RTCP_SDES_SSRC_SIZE + RTP_RTCP_SDES_ITEM_TYPE_AND_LENGTH_SIZE + cNameLength + chunkPadding;

  /* #10 Check if the SDES message can be serialized into the remaining buffer */
  if((*PayloadIdxPtr + RTP_RTCP_SDES_HEADER_FIXED_SIZE + sdesLength) > PduInfoPtr->SduLength)
  {
    retVal = E_NOT_OK;
  }
  else
  {
    /* #20 Serialize source description (SDES) items containing the CNAME to the compound packet*/
    RTP_PUT_UINT16(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_SDES_HEADER_FIXED_16BITWORD_OFFSET,
      flags);

    RTP_PUT_UINT16(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_SDES_LENGTH_OFFSET,
      RTP_LENGTH_IN_32WORDS(sdesLength));

    RTP_PUT_UINT32(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_SDES_FIRST_SSRC_OFFSET,
      Rtp_GetSsrcOfDynamicStreamData(Rtp_GetStreamIdxOfTxStream(TxStreamIdx)));

    RTP_PUT_UINT8(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_SDES_ITEM_FIRST_TYPE_OFFSET,
      RTP_RTCP_SDES_TYPE_CNAME);

    RTP_PUT_UINT8(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_SDES_ITEM_FIRST_LENGTH_OFFSET,
      cNameLength);

    IpBase_Copy(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr + RTP_RTCP_SDES_ITEM_FIRST_VALUE_OFFSET],
      &Rtp_GetCName(Rtp_GetCNameStartIdxOfTxStream(TxStreamIdx)),
      cNameLength);

    /* Add END items until the chunk is padded */
    while(chunkPadding > 0)
    {
      chunkPadding--;
      PduInfoPtr->SduDataPtr[*PayloadIdxPtr + RTP_RTCP_SDES_ITEM_FIRST_VALUE_OFFSET + cNameLength + chunkPadding] = RTP_RTCP_SDES_TYPE_END;
    }

    *PayloadIdxPtr += RTP_RTCP_SDES_HEADER_FIXED_SIZE;
    *PayloadIdxPtr += sdesLength;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Rtp_RtcpSerializeAvbRtcp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_RtcpSerializeAvbRtcp(
  Rtp_TxStreamIdxOfStreamType TxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, RTP_APPL_DATA) PayloadIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the AVB RTCP message can be serialized into the remaining buffer */
  if((*PayloadIdxPtr + RTP_RTCP_AVB_PACKET_SIZE) > PduInfoPtr->SduLength)
  {
    retVal = E_NOT_OK;
  }
  else
  {
    /* #20 Serialize AVB RTCP compound packet */
    RTP_PUT_UINT8(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_AVB_HEADER_FIXED_VSP_OFFSET,
      RTP_RTCP_AVB_HEADER_FIXED_VSP);
    
    RTP_PUT_UINT8(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_AVB_HEADER_FIXED_TYPE_OFFSET,
      RTP_RTCP_AVB_HEADER_FIXED_TYPE);
    
    RTP_PUT_UINT8(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_AVB_HEADER_FIXED_LENGTH_HB_OFFSET,
      RTP_RTCP_AVB_HEADER_FIXED_LENGTH_HB);
    
    RTP_PUT_UINT8(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_AVB_HEADER_FIXED_LENGTH_LB_OFFSET,
      RTP_RTCP_AVB_HEADER_FIXED_LENGTH_LB);

    RTP_PUT_UINT32(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_AVB_SSRC_OFFSET,
      Rtp_GetSsrcOfDynamicStreamData(Rtp_GetStreamIdxOfTxStream(TxStreamIdx)));

    IpBase_Copy(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr + RTP_RTCP_AVB_NAME_OFFSET],
      &Rtp_GetNameOfTxStream(TxStreamIdx)[0],
      RTP_RTCP_AVB_NAME_SIZE);

    RTP_PUT_UINT16(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_AVB_GMTIMEBASEINDICATOR_OFFSET,
      Rtp_GetGmTimeBaseIndicatorOfDynamicTxStreamData(TxStreamIdx));

    IpBase_Copy(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr + RTP_RTCP_AVB_GMIDENTITY_OFFSET],
      &Rtp_GetGmIdentityOfDynamicTxStreamData(TxStreamIdx)[0],
      RTP_RTCP_AVB_GMIDENTITY_SIZE);

    RTP_PUT_UINT64(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_AVB_STREAM_ID_OFFSET,
      Rtp_GetIdOfStream(Rtp_GetStreamIdxOfTxStream(TxStreamIdx)));

    RTP_PUT_UINT32(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_AVB_AS_TIMESTAMP_OFFSET,
      Rtp_GetAsTimestampOfDynamicTxStreamData(TxStreamIdx));

    RTP_PUT_UINT32(&PduInfoPtr->SduDataPtr[*PayloadIdxPtr],
      RTP_RTCP_AVB_RTP_TIMESTAMP_OFFSET,
      Rtp_GetRtpTimestampOfDynamicTxStreamData(TxStreamIdx));

    *PayloadIdxPtr += RTP_RTCP_AVB_PACKET_SIZE;
  }

  return retVal;
}
#endif /* RTP_TXSTREAM == STD_ON */

#if (RTP_RXSTREAM == STD_ON)
/**********************************************************************************************************************
 *  Rtp_ReadRtpHeaderInformation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_ReadRtpHeaderInformation(
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  P2VAR(Rtp_AddRtpHeaderType, AUTOMATIC, RTP_APPL_DATA) AddHeaderPtr,
  P2VAR(Rtp_RxRtpInfoType, AUTOMATIC, RTP_APPL_DATA)  HeaderInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the payload contains the entire RTP header */
  if(RTP_HEADER_SIZE <= PduInfoPtr->SduLength)
  {
    /* #20 Read RTP header information */
    AddHeaderPtr->Version         =   (uint8)((PduInfoPtr->SduDataPtr[0] & RTP_HEADERFLAG_VERSION)       >> RTP_HEADERFLAG_BITOFFSET_VERSION);
    HeaderInfoPtr->Padding        = (boolean)((PduInfoPtr->SduDataPtr[0] & RTP_HEADERFLAG_PADDING)       >> RTP_HEADERFLAG_BITOFFSET_PADDING);
    AddHeaderPtr->ExtensionHeader = (boolean)((PduInfoPtr->SduDataPtr[0] & RTP_HEADERFLAG_EXTENSION)     >> RTP_HEADERFLAG_BITOFFSET_EXTENSION);
    AddHeaderPtr->NrCSRC          =   (uint8)((PduInfoPtr->SduDataPtr[0] & RTP_HEADERFLAG_CSRC_COUNT)    >> RTP_HEADERFLAG_BITOFFSET_CSRC_COUNT);
    HeaderInfoPtr->Marker         = (boolean)((PduInfoPtr->SduDataPtr[1] & RTP_HEADERFLAG_MARKER)        >> RTP_HEADERFLAG_BITOFFSET_MARKER);
    AddHeaderPtr->PayloadType     =   (uint8)((PduInfoPtr->SduDataPtr[1] & RTP_HEADERFLAG_PAYLOAD_TYPE)  >> RTP_HEADERFLAG_BITOFFSET_PAYLOAD_TYPE);
    RTP_GET_UINT16(PduInfoPtr->SduDataPtr, RTP_HEADER_OFFSET_SEQUENCENUMBER, HeaderInfoPtr->SequenceNumber);
    RTP_GET_UINT32(PduInfoPtr->SduDataPtr, RTP_HEADER_OFFSET_TIMESTAMP,      HeaderInfoPtr->RtpTimestamp);
    RTP_GET_UINT32(PduInfoPtr->SduDataPtr, RTP_HEADER_OFFSET_SSRC,           AddHeaderPtr->Ssrc);

    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Rtp_RtpRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
RTP_LOCAL FUNC(void, RTP_CODE) Rtp_RtpRxIndication(
  Rtp_RxStreamIdxOfStreamType RxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint16 payloadOffset = RTP_HEADER_SIZE;
  Rtp_AddRtpHeaderType addHeader;
  Rtp_RxRtpInfoType headerInfo;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read RTP header of the message */
  retVal = Rtp_ReadRtpHeaderInformation(PduInfoPtr, &addHeader, &headerInfo);

  if(retVal == E_OK)
  {
    /* #20 Check if the received SSRC is also used by a configured Tx stream */
    Rtp_HandleSsrcCollision(addHeader.Ssrc);

    /* #30 Check header information */
    if(addHeader.Version != 2U)
    {
      retVal = E_NOT_OK;
    }
    else if(addHeader.PayloadType != Rtp_GetTypeOfStream(Rtp_GetStreamIdxOfRxStream(RxStreamIdx)))
    {
      retVal = E_NOT_OK;
    }
    else if(addHeader.Ssrc != Rtp_GetSsrcOfDynamicStreamData(Rtp_GetStreamIdxOfRxStream(RxStreamIdx)))
    {
      retVal = E_NOT_OK;
    }
    else
    {
      /* Process the received message */
    }

    /* #40 Skip present contributing source (CSRC) identifiers or attached extension headers */

    /* Check if the payload contains contributing source (CSRC) identifiers */
    if(addHeader.NrCSRC != 0)
    {
      /* Ignore all attached CSRC identifiers */
      payloadOffset += (addHeader.NrCSRC * RTP_CSRC_SIZE);
    }

    /* Check if the payload contains an attached extension header */
    if((addHeader.ExtensionHeader == TRUE) && (retVal == E_OK))
    {
      uint16 length;

      /* Check if the payload can contain an extension header */
      if(PduInfoPtr->SduLength > (RTP_HEADER_SIZE + RTP_EXTENSIONHEADER_FIXED_SIZE))
      {
        /* Get length of extension header (length counts the number of subsequent 32-bit words) */
        RTP_GET_UINT16(PduInfoPtr->SduDataPtr, payloadOffset + RTP_EXTENSIONHEADER_OFFSET_LENGH, length);

        /* Ignore the attached extension header */
        payloadOffset += RTP_EXTENSIONHEADER_FIXED_SIZE + (length * RTP_EXTENSIONHEADER_BYTES_PER_LENGTH);
      }
      else
      {
        retVal = E_NOT_OK;
      }
    }

    /* Check if the payload offset is feasible */
    if((retVal == E_OK) && (PduInfoPtr->SduLength > payloadOffset))
    {
      /* #50 Forward data to the upper layer */
      Rtp_GetUlRtpRxCbkOfRxStream(RxStreamIdx)(
        (uint16)Rtp_GetStreamIdxOfRxStream(RxStreamIdx),
        &headerInfo,
        &PduInfoPtr->SduDataPtr[payloadOffset],
        PduInfoPtr->SduLength - payloadOffset);
    }
  }
}

/**********************************************************************************************************************
 *  Rtp_ReadRtcpHeaderInformation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_ReadRtcpHeaderInformation(
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  uint16 StartIdx,
  P2VAR(Rtp_RtcpHeaderType, AUTOMATIC, RTP_APPL_DATA) RtcpHeaderPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the payload contains the fixed part of the header */
  if((StartIdx + RTP_RTCP_FIXED_HEADER_SIZE) <= PduInfoPtr->SduLength)
  {
    /* #20 Read RTCP header information */
    RtcpHeaderPtr->Version =   (uint8)((PduInfoPtr->SduDataPtr[StartIdx] & RTP_RTCP_HEADERFLAG_VERSION) >> RTP_RTCP_HEADERFLAG_BITOFFSET_VERSION);
    RtcpHeaderPtr->Padding = (boolean)((PduInfoPtr->SduDataPtr[StartIdx] & RTP_RTCP_HEADERFLAG_PADDING) >> RTP_RTCP_HEADERFLAG_BITOFFSET_PADDING);
    RtcpHeaderPtr->Subtype =   (uint8)((PduInfoPtr->SduDataPtr[StartIdx] & RTP_RTCP_HEADERFLAG_SUBTYPE) >> RTP_RTCP_HEADERFLAG_BITOFFSET_SUBTYPE);
    RTP_GET_UINT8( PduInfoPtr->SduDataPtr, RTP_RTCP_HEADER_PACKET_TYPE_OFFSET + StartIdx,  RtcpHeaderPtr->PayloadType);
    RTP_GET_UINT16(PduInfoPtr->SduDataPtr, RTP_RTCP_HEADER_LENGTH_OFFSET + StartIdx, RtcpHeaderPtr->Length);

    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Rtp_ProcessRtcpAvbPacket()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_ProcessRtcpAvbPacket(
  Rtp_RxStreamIdxOfStreamType RxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  uint16 StartIdx,
  uint16 Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Rtp_AddAvbRtcpInfoType addAvbInfo;
  Rtp_RxAvbRtcpInfoType avbInfo;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the length of the AVB RTCP packet is feasible */
  if(((Length + RTP_RTCP_AVB_HEADER_FIXED_SIZE) >= RTP_RTCP_AVB_PACKET_SIZE) &&
    ((StartIdx + RTP_RTCP_AVB_PACKET_SIZE) <= PduInfoPtr->SduLength))
  {
    /* #20 Read the packet information */
    RTP_GET_UINT32(&PduInfoPtr->SduDataPtr[StartIdx], RTP_RTCP_AVB_SSRC_OFFSET,                addAvbInfo.Ssrc);
    RTP_GET_UINT32(&PduInfoPtr->SduDataPtr[StartIdx], RTP_RTCP_AVB_NAME_OFFSET,                avbInfo.NameAscii);
    RTP_GET_UINT16(&PduInfoPtr->SduDataPtr[StartIdx], RTP_RTCP_AVB_GMTIMEBASEINDICATOR_OFFSET, avbInfo.GmTimeBaseIndicator);
    IpBase_Copy(&avbInfo.GmIdentity[0],
      &PduInfoPtr->SduDataPtr[StartIdx + RTP_RTCP_AVB_GMIDENTITY_OFFSET],
      RTP_RTCP_AVB_GMIDENTITY_SIZE);
    RTP_GET_UINT64(&PduInfoPtr->SduDataPtr[StartIdx], RTP_RTCP_AVB_STREAM_ID_OFFSET,           addAvbInfo.StreamId);
    RTP_GET_UINT32(&PduInfoPtr->SduDataPtr[StartIdx], RTP_RTCP_AVB_AS_TIMESTAMP_OFFSET,        avbInfo.AsTimestamp);
    RTP_GET_UINT32(&PduInfoPtr->SduDataPtr[StartIdx], RTP_RTCP_AVB_RTP_TIMESTAMP_OFFSET,       avbInfo.RtpTimestamp);

    /* #30 Check if the received SSRC is also used by a configured Tx stream */
    Rtp_HandleSsrcCollision(addAvbInfo.Ssrc);

    /* #40 Check if the packet corresponds to the configuration */
    if(addAvbInfo.StreamId == Rtp_GetIdOfStream(Rtp_GetStreamIdxOfRxStream(RxStreamIdx)))
    {
      /* Save SSRC of the remote server */
      Rtp_SetSsrcOfDynamicStreamData(Rtp_GetStreamIdxOfRxStream(RxStreamIdx), addAvbInfo.Ssrc);

      /* #50 Forward the information to the UpperLayer */
      Rtp_GetUlRtcpRxCbkOfRxStream(RxStreamIdx)(
        (uint16)Rtp_GetStreamIdxOfRxStream(RxStreamIdx),
        &avbInfo);
    }
  }
  else
  {
    /* The payload does not contain the entire AVB RTCP packet */
    retVal = E_NOT_OK;
  }

  return retVal;
}

# if(RTP_ULRTCPSDESCBKOFRXSTREAM == STD_ON)
/**********************************************************************************************************************
 *  Rtp_ProcessRtcpSdesPacket()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_ProcessRtcpSdesPacket(
  Rtp_RxStreamIdxOfStreamType RxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  uint16 StartIdx,
  uint16 Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  boolean endItem;
  uint32 ssrc;
  uint16 chunkOffset;
  uint8 itemOffset, itemType, itemLength;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the length is feasible */
  if((StartIdx + RTP_RTCP_SDES_HEADER_FIXED_SIZE + Length) <= PduInfoPtr->SduLength)
  {
    chunkOffset = RTP_RTCP_SDES_HEADER_FIXED_SIZE;

    /* #20 Iterate over all SDES chunks */
    while(chunkOffset < Length)
    {
      endItem = FALSE;

      /* Read SSRC */
      RTP_GET_UINT32(&PduInfoPtr->SduDataPtr[StartIdx + chunkOffset], RTP_RTCP_SDES_CHUNK_SSRC_OFFSET, ssrc);
      itemOffset = RTP_RTCP_SDES_SSRC_SIZE;

      /* #30 Check if the received SSRC is also used by a configured Tx stream */
      Rtp_HandleSsrcCollision(ssrc);

      /* Iterate over all SDES items */
      while(((chunkOffset + itemOffset) < Length) && (endItem == FALSE))
      {
        /* Read the type of the item */
        RTP_GET_UINT8(&PduInfoPtr->SduDataPtr[StartIdx + chunkOffset + itemOffset], RTP_RTCP_SDES_ITEM_TYPE_OFFSET, itemType);

        if(itemType == RTP_RTCP_SDES_TYPE_END)
        {
          endItem = TRUE;

          /* Termination of item list. Next chunk or compound packet is padded to the next 32-bit boundary */
          itemOffset += (uint8)(RTP_RTCP_SDES_ITEM_TYPE_SIZE + (4 - ((chunkOffset + itemOffset + RTP_RTCP_SDES_ITEM_TYPE_SIZE) % 4)));
        }
        else
        {
          /* Read the length of the item */
          RTP_GET_UINT8(&PduInfoPtr->SduDataPtr[StartIdx + chunkOffset + itemOffset], RTP_RTCP_SDES_ITEM_LENGTH_OFFSET, itemLength);

          itemOffset += RTP_RTCP_SDES_ITEM_TYPE_AND_LENGTH_SIZE + itemLength;
        }
      }

      if(ssrc == Rtp_GetSsrcOfDynamicStreamData(Rtp_GetStreamIdxOfRxStream(RxStreamIdx)))
      {
        /* #40 Forward SDES chunk items to upper layer */
        Rtp_GetUlRtcpSdesCbkOfRxStream(RxStreamIdx)(
          (uint16)Rtp_GetStreamIdxOfRxStream(RxStreamIdx),
          &PduInfoPtr->SduDataPtr[StartIdx + chunkOffset + RTP_RTCP_SDES_SSRC_SIZE],
          (uint16)(itemOffset - RTP_RTCP_SDES_SSRC_SIZE));
      }

      chunkOffset += itemOffset;
    }
  }
  else
  {
    /* The payload does not contain the entire SDES packet */
    retVal = E_NOT_OK;
  }

  return retVal;
}
# endif /* RTP_ULRTCPSDESCBKOFRXSTREAM == STD_ON */

/**********************************************************************************************************************
 *  Rtp_ProcessRtcpReportPacket()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
RTP_LOCAL FUNC(Std_ReturnType, RTP_CODE) Rtp_ProcessRtcpReportPacket(
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr,
  uint16 StartIdx,
  uint16 Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint32 ssrc;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the length is feasible */
  if((StartIdx + RTP_RTCP_REPORT_HEADER_FIXED_SIZE + Length) <= PduInfoPtr->SduLength)
  {
    /* Read SSRC */
    RTP_GET_UINT32(&PduInfoPtr->SduDataPtr[StartIdx], RTP_RTCP_REPORT_HEADER_FIXED_SIZE, ssrc);

    /* #20 Check if the received SSRC is also used by a configured Tx stream */
    Rtp_HandleSsrcCollision(ssrc);
  }
  else
  {
    /* The payload does not contain the entire report packet */
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Rtp_RtcpRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
RTP_LOCAL FUNC(void, RTP_CODE) Rtp_RtcpRxIndication(
  Rtp_RxStreamIdxOfStreamType RxStreamIdx,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Rtp_RtcpHeaderType rtcpHeader;
  uint16 payloadIdx = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* Iterate over all compound packets */
  while(retVal == E_OK)
  {
    /* #10 Read fixed RTCP header information */
    retVal = Rtp_ReadRtcpHeaderInformation(PduInfoPtr, payloadIdx, &rtcpHeader);

    if(retVal == E_OK)
    {
      switch(rtcpHeader.PayloadType)
      {
        case RTP_RTCP_PACKETTYPE_SDES:
          /* #20 Process SDES packet in sub-function */
# if(RTP_ULRTCPSDESCBKOFRXSTREAM == STD_ON)
          retVal = Rtp_ProcessRtcpSdesPacket(RxStreamIdx, PduInfoPtr, payloadIdx, RTP_LENGTH_IN_BYTES(rtcpHeader.Length));
# else
          /* No SDES callback configured. Ignore SDES compound packet. */
# endif /* RTP_ULRTCPSDESCBKOFRXSTREAM == STD_ON */
          break;

        case RTP_RTCP_PACKETTYPE_AVB_RTCP:
          /* #30 Process AVB RTCP packet in sub-function */
          retVal = Rtp_ProcessRtcpAvbPacket(RxStreamIdx, PduInfoPtr, payloadIdx, RTP_LENGTH_IN_BYTES(rtcpHeader.Length));
          break;

        case RTP_RTCP_PACKETTYPE_SR:
        case RTP_RTCP_PACKETTYPE_RR:
          retVal = Rtp_ProcessRtcpReportPacket(PduInfoPtr, payloadIdx, RTP_LENGTH_IN_BYTES(rtcpHeader.Length));
          break;

        case RTP_RTCP_PACKETTYPE_BYE:
        case RTP_RTCP_PACKETTYPE_APP:
        default:
          /* #40 Ignore irrelevant or unknown packet types.
           *     (Optional padding is already included in the length field and has not to be considered) */
          break;
      }

      payloadIdx += RTP_RTCP_FIXED_HEADER_SIZE;
      payloadIdx += RTP_LENGTH_IN_BYTES(rtcpHeader.Length); /* Optional padding bytes are also included in the length value */
    }
  }
}

/**********************************************************************************************************************
 *  Rtp_HandleSsrcCollision()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
RTP_LOCAL FUNC(void, RTP_CODE) Rtp_HandleSsrcCollision(
  uint32 RxSsrc)
{
# if(RTP_TXSTREAM == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Rtp_TxStreamIterType txStreamIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the received SSRC is also used by a TX stream */
  for(txStreamIdx = 0; txStreamIdx < Rtp_GetSizeOfTxStream(); txStreamIdx++)
  {
    /* #20 Trigger transmission of a BYE packet and renew the SSRC if a collision is detected */
    if(RxSsrc == Rtp_GetSsrcOfDynamicStreamData(Rtp_GetStreamIdxOfTxStream(txStreamIdx)))
    {
      Rtp_SetTransmitByeOfDynamicTxStreamData(txStreamIdx, TRUE);
    }
  }
# else
  RTP_DUMMY_STATEMENT(RxSsrc); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif /* RTP_TXSTREAM == STD_ON */
}
#endif /* RTP_RXSTREAM == STD_ON */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Rtp_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, RTP_CODE) Rtp_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
#if(RTP_DEV_ERROR_DETECT == STD_ON)
  Rtp_ModuleInitialized = RTP_UNINIT;
#endif /* RTP_DEV_ERROR_DETECT == STD_ON */
}

/**********************************************************************************************************************
 *  Rtp_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, RTP_CODE) Rtp_Init(
  P2CONST(Rtp_ConfigType, AUTOMATIC, RTP_APPL_CONST) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Rtp_StreamIterType streamIdx;
  uint8 errorId = RTP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if(RTP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters */
  if(Rtp_ModuleInitialized == (uint8)RTP_INIT)
  {
    errorId = RTP_E_ALREADY_INITIALIZED;
  }
  else if(ConfigPtr != NULL_PTR)
  {
    errorId = RTP_E_PARAM_CONFIG;
  }
  else
#endif /* RTP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if(RTP_DEV_ERROR_DETECT == STD_ON)
    Rtp_ModuleInitialized = (uint8)RTP_INIT;
#endif /* RTP_DEV_ERROR_DETECT == STD_ON */

    /* #20 Initialize dynamic parameters on stream level */
    for(streamIdx = 0; streamIdx < Rtp_GetSizeOfStream(); streamIdx++)
    {
      Rtp_SetSsrcOfDynamicStreamData(streamIdx, RTP_INVALID_RANDOM_NUMBER);
    }

#if(RTP_TXSTREAM == STD_ON)
    /* #30 Initialize dynamic parameters on Tx stream level */
    for(streamIdx = 0; streamIdx < Rtp_GetSizeOfTxStream(); streamIdx++)
    {
      Rtp_SetPendingTxCallbackBytesOfDynamicTxStreamData(streamIdx, 0);
      Rtp_SetCopiedTxCallbackBytesOfDynamicTxStreamData(streamIdx, 0);
      Rtp_SetTransmitByeOfDynamicTxStreamData(streamIdx, FALSE);
      Rtp_SetSequenceNumberOfDynamicTxStreamData(streamIdx, RTP_INVALID_RANDOM_NUMBER);
      Rtp_SetPayloadPtrOfDynamicTxStreamData(streamIdx, NULL_PTR);
      Rtp_SetPaddingFlagOfDynamicTxStreamData(streamIdx, FALSE);
      Rtp_SetMarkerFlagOfDynamicTxStreamData(streamIdx, FALSE);
      Rtp_SetRtpTimestampOfDynamicTxStreamData(streamIdx, 0);
      Rtp_SetAsTimestampOfDynamicTxStreamData(streamIdx, 0);
      Rtp_SetGmTimeBaseIndicatorOfDynamicTxStreamData(streamIdx, 0U);
      IpBase_Copy(
        &Rtp_GetGmIdentityOfDynamicTxStreamData(streamIdx)[0],
        &Rtp_GetGmIdentityOfTimeDomain(Rtp_GetTimeDomainIdxOfStream(streamIdx))[0],
        RTP_RTCP_AVB_GMIDENTITY_SIZE);
      Rtp_SetRtcpMainFuncToWaitOfDynamicTxStreamData(streamIdx, 0);
      Rtp_SetStateOfDynamicTxStreamData(streamIdx, RTP_STREAM_DISABLED);
    }
#endif /* RTP_TXSTREAM == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if(RTP_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if(errorId != RTP_E_NO_ERROR)
  {
    (void)Det_ReportError(RTP_MODULE_ID, RTP_INSTANCE_ID_DET, RTP_SID_INIT, errorId);
  }
#else
  RTP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* RTP_DEV_ERROR_REPORT == STD_ON */
}

#if(RTP_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Rtp_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, RTP_CODE) Rtp_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, RTP_APPL_DATA) VersionInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = RTP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if(RTP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters */
  if(VersionInfoPtr == NULL_PTR)
  {
    errorId = RTP_E_PARAM_POINTER;
  }
  else
# endif /* RTP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set VersionInfo structure with corresponding macros */
    VersionInfoPtr->vendorID = (RTP_VENDOR_ID);
    VersionInfoPtr->moduleID = (RTP_MODULE_ID);
    VersionInfoPtr->sw_major_version = (RTP_SW_MAJOR_VERSION);
    VersionInfoPtr->sw_minor_version = (RTP_SW_MINOR_VERSION);
    VersionInfoPtr->sw_patch_version = (RTP_SW_PATCH_VERSION);
  }

  /* ----- Development Error Report --------------------------------------- */
# if(RTP_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report default errors if any occurred */
  if(errorId != RTP_E_NO_ERROR)
  {
    (void)Det_ReportError(RTP_MODULE_ID, RTP_INSTANCE_ID_DET, RTP_SID_GET_VERSION_INFO, errorId);
  }
# else
  RTP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif /* RTP_DEV_ERROR_REPORT == STD_ON */
}
#endif /* RTP_VERSION_INFO_API */

/**********************************************************************************************************************
 *  Rtp_SoAdRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, RTP_CODE) Rtp_SoAdRxIndication(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr)
{
#if(RTP_RXSTREAM == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Rtp_RxStreamIterType rxStreamIdx;
  uint8 errorId = RTP_E_NO_ERROR;
  boolean pduIdMatch = FALSE;

  /* ----- Development Error Checks ------------------------------------- */
# if(RTP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters */
  if(Rtp_ModuleInitialized == RTP_UNINIT)
  {
    errorId = RTP_E_UNINIT;
  }
  else if(RxPduId >= (Rtp_GetSizeOfRxStream() * 2))
  {
    errorId = RTP_E_PARAM_VALUE;
  }
  else if(PduInfoPtr == NULL_PTR)
  {
    errorId = RTP_E_PARAM_POINTER;
  }
  else if(PduInfoPtr->SduDataPtr == NULL_PTR)
  {
    errorId = RTP_E_PARAM_POINTER;
  }
  else
# endif /* RTP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Search for corresponding stream */
    for(rxStreamIdx = 0; rxStreamIdx < Rtp_GetSizeOfRxStream(); rxStreamIdx++)
    {
      /* #30 Check whether the received message is an RTP or RTCP message.
       *     Forward message to the corresponding sub-function */
      if(RxPduId == Rtp_GetRtpRxPduIdOfRxStream(rxStreamIdx))
      {
        /* Forward message to RTP sub-function */
        Rtp_RtpRxIndication((Rtp_RxStreamIdxOfStreamType)rxStreamIdx, PduInfoPtr);
        pduIdMatch = TRUE;
      }
      else if(RxPduId == Rtp_GetRtcpRxPduIdOfRxStream(rxStreamIdx))
      {
        /* Forward message to RTCP sub-function */
        Rtp_RtcpRxIndication((Rtp_RxStreamIdxOfStreamType)rxStreamIdx, PduInfoPtr);
        pduIdMatch = TRUE;
      }
      else
      {
        /* RxPduId does not match - Proceed search */
      }

      if(pduIdMatch == TRUE)
      {
        /* Stop searching for further streams */
        break;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if(RTP_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if(errorId != RTP_E_NO_ERROR)
  {
    (void)Det_ReportError(RTP_MODULE_ID, RTP_INSTANCE_ID_DET, RTP_SID_RX_INDICATION, errorId);
  }
# else
  RTP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif /* RTP_DEV_ERROR_REPORT == STD_ON */
#else
  RTP_DUMMY_STATEMENT(RxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  RTP_DUMMY_STATEMENT(PduInfoPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

# if(RTP_DEV_ERROR_REPORT == STD_ON)
  (void)Det_ReportError(RTP_MODULE_ID, RTP_INSTANCE_ID_DET, RTP_SID_RX_INDICATION, RTP_E_API_DISABLED);
# endif
#endif /* RTP_RXSTREAM == STD_ON */
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Rtp_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, RTP_CODE) Rtp_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = RTP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if(RTP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if module is initialized */
  if(Rtp_ModuleInitialized == RTP_UNINIT)
  {
    errorId = RTP_E_UNINIT;
  }
  else
#endif /* RTP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
#if(RTP_TXSTREAM == STD_ON)
    Std_ReturnType retval;
    Rtp_TxStreamIterType streamIdx;

    /* #20 Iterate all Tx streams */
    for(streamIdx = 0; streamIdx < Rtp_GetSizeOfTxStream(); streamIdx++)
    {
      /* #30 Transmit BYE packet and renew SSRC if a collision was detected or the stream was disabled */
      if(Rtp_IsTransmitByeOfDynamicTxStreamData(streamIdx))
      {
        /* Trigger transmission of a BYE packet */
        PduInfoType PduInfo;

        if(Rtp_GetStateOfDynamicTxStreamData(streamIdx) != RTP_STREAM_DISABLED)
        {
          /* Transmit RTCP packet */
          PduInfo.SduDataPtr = NULL_PTR;
          PduInfo.SduLength = RTP_RTCP_EMPTYRR_PACKET_SIZE + RTP_RTCP_BYE_PACKET_SIZE;
          retval = SoAd_IfTransmit(Rtp_GetSoAdRtcpTxPduIdOfTxStream(streamIdx), &PduInfo);
        }
        else
        {
          /* Stream is disabled, renew the SSRC, but do not transmit a BYE message */
          retval = E_OK;
        }

        if(retval == E_OK)
        {
          uint32 oldSsrc = Rtp_GetSsrcOfDynamicStreamData(Rtp_GetStreamIdxOfTxStream(streamIdx));

          /* Renew the SSRC */
          do
          {
            /* PRQA S 0310 3 */ /* MD_Rtp_11.4_0310 */
            Rtp_ExternalRandomNumberFct(
              (P2VAR(uint8, AUTOMATIC, RTP_APPL_DATA))(&Rtp_GetSsrcOfDynamicStreamData(Rtp_GetStreamIdxOfTxStream(streamIdx))),
              sizeof(uint32));
          }
          while((Rtp_GetSsrcOfDynamicStreamData(Rtp_GetStreamIdxOfTxStream(streamIdx)) == RTP_INVALID_RANDOM_NUMBER) ||
            (Rtp_GetSsrcOfDynamicStreamData(Rtp_GetStreamIdxOfTxStream(streamIdx)) == oldSsrc));

          /* Remove the renew flag */
          Rtp_SetTransmitByeOfDynamicTxStreamData(streamIdx, FALSE);

          if(Rtp_GetStateOfDynamicTxStreamData(streamIdx) == RTP_STREAM_SHUTDOWN)
          {
            /* Disable if stream was shut down and BYE passe is transmitted */
            Rtp_GetStateOfDynamicTxStreamData(streamIdx) = RTP_STREAM_DISABLED;

            /* Reset the timer for RTCP message transmission */
            Rtp_SetRtcpMainFuncToWaitOfDynamicTxStreamData(streamIdx, 0);
          }
        }
        else
        {
          /* Failed to transmit BYE message. Retry in next MainFunction. */
        }
      }

      if(Rtp_GetStateOfDynamicTxStreamData(streamIdx) == RTP_STREAM_ENABLED)
      {
        /* #40 Trigger transmission of AVB RTCP packet if timer exceeded */
        if(Rtp_GetRtcpMainFuncToWaitOfDynamicTxStreamData(streamIdx) == 0)
        {
          PduInfoType PduInfo;
          uint16 sdesLength;

          /* Calculate length of SDES packet */
          sdesLength = (uint16)(Rtp_GetCNameEndIdxOfTxStream(streamIdx) - Rtp_GetCNameStartIdxOfTxStream(streamIdx));
          sdesLength += (uint8)(4 - ((RTP_RTCP_SDES_ITEM_TYPE_AND_LENGTH_SIZE + sdesLength) % 4));
          sdesLength += RTP_RTCP_SDES_HEADER_FIXED_SIZE + RTP_RTCP_SDES_SSRC_SIZE + RTP_RTCP_SDES_ITEM_TYPE_AND_LENGTH_SIZE;

          /* Transmit RTCP packet */
          PduInfo.SduDataPtr = NULL_PTR;
          PduInfo.SduLength = RTP_RTCP_EMPTYRR_PACKET_SIZE + sdesLength  + RTP_RTCP_AVB_PACKET_SIZE;
          retval = SoAd_IfTransmit(Rtp_GetSoAdRtcpTxPduIdOfTxStream(streamIdx), &PduInfo);

          if(retval == E_OK)
          {
            /* Message transmitted. Reset timer. */
            Rtp_SetRtcpMainFuncToWaitOfDynamicTxStreamData(streamIdx, Rtp_GetRtcpCycleTimeInMainFuncOfTxStream(streamIdx));
          }
          else
          {
            /* Failed to transmit RTCP message. Retry in next MainFunction. */
          }
        }

        Rtp_DecRtcpMainFuncToWaitOfDynamicTxStreamData(streamIdx);
      }
    }
#endif /* RTP_TXSTREAM == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if(RTP_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report default errors if any occurred */
  if(errorId != RTP_E_NO_ERROR)
  {
    (void)Det_ReportError(RTP_MODULE_ID, RTP_INSTANCE_ID_DET, RTP_SID_MAIN_FUNCTION, errorId);
  }
#else
  RTP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* RTP_DEV_ERROR_REPORT == STD_ON */
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Rtp_Transmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, RTP_CODE) Rtp_Transmit(
  uint16 StreamHandleId,
  P2CONST(Rtp_TxRtpInfoType, AUTOMATIC, RTP_APPL_DATA) InfoPtr,
  P2CONST(uint8, AUTOMATIC, RTP_APPL_DATA) PayloadPtr,
  uint16 Length)
{
#if(RTP_TXSTREAM == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  PduInfoType PduInfo;
  Rtp_TxStreamIdxOfStreamType txStreamIdx;
  uint8 errorId = RTP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if(RTP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters */
  if(Rtp_ModuleInitialized == RTP_UNINIT)
  {
    errorId = RTP_E_UNINIT;
  }
  else if(StreamHandleId >= Rtp_GetSizeOfStream())
  {
    errorId = RTP_E_PARAM_VALUE;
  }
  else if(!Rtp_IsTxStreamUsedOfStream(StreamHandleId))
  {
    errorId = RTP_E_PARAM_VALUE;
  }
#  if(RTP_ULCOPYTXDATACBKOFTXSTREAM == STD_OFF)
  else if(PayloadPtr == NULL_PTR)
  {
    errorId = RTP_E_PARAM_POINTER;
  }
#  endif /* RTP_ULCOPYTXDATACBKOFTXSTREAM == STD_OFF */
  else
# endif /* RTP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    txStreamIdx = Rtp_GetTxStreamIdxOfStream(StreamHandleId);

    /* #20 Check current state of stream */
    if(Rtp_GetSsrcOfDynamicStreamData(StreamHandleId) == RTP_INVALID_RANDOM_NUMBER)
    {
      retVal = E_NOT_OK;
    }
    else if(Rtp_GetStateOfDynamicTxStreamData(txStreamIdx) != RTP_STREAM_ENABLED)
    {
      retVal = E_NOT_OK;
    }
# if(RTP_ULCOPYTXDATACBKOFTXSTREAM == STD_ON)
    /* Check if the PayloadPtr is valid */
    else if((PayloadPtr == NULL_PTR) && (Rtp_GetUlCopyTxDataCbkOfTxStream(txStreamIdx) == NULL_PTR))
    {
      retVal = E_NOT_OK;
    }
# endif /* RTP_ULCOPYTXDATACBKOFTXSTREAM == STD_ON */
    else
    {
      RTP_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #30 Save upper layer data and trigger transmission */
      Rtp_SetRtpTimestampOfDynamicTxStreamData(txStreamIdx, InfoPtr->RtpTimestamp);
      Rtp_SetAsTimestampOfDynamicTxStreamData(txStreamIdx, InfoPtr->AsTimestamp);
      Rtp_SetPaddingFlagOfDynamicTxStreamData(txStreamIdx, InfoPtr->Padding);
      Rtp_SetMarkerFlagOfDynamicTxStreamData(txStreamIdx, InfoPtr->Marker);
      Rtp_SetPendingTxCallbackBytesOfDynamicTxStreamData(txStreamIdx, Length + RTP_HEADER_SIZE);
      Rtp_SetCopiedTxCallbackBytesOfDynamicTxStreamData(txStreamIdx, 0);
      Rtp_GetPayloadPtrOfDynamicTxStreamData(txStreamIdx) = PayloadPtr;

      RTP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* Request buffer */
      PduInfo.SduDataPtr = NULL_PTR;
      PduInfo.SduLength = Length + RTP_HEADER_SIZE;
      retVal = SoAd_IfTransmit(Rtp_GetSoAdRtpTxPduIdOfTxStream(txStreamIdx), &PduInfo);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if(RTP_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if(errorId != RTP_E_NO_ERROR)
  {
    (void)Det_ReportError(RTP_MODULE_ID, RTP_INSTANCE_ID_DET, RTP_SID_TRANSMIT_FUNCTION, errorId);
  }
# else
  RTP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif /* RTP_DEV_ERROR_REPORT == STD_ON */
#else
  Std_ReturnType retVal = E_NOT_OK;
  RTP_DUMMY_STATEMENT(StreamHandleId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  RTP_DUMMY_STATEMENT(InfoPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  RTP_DUMMY_STATEMENT(PayloadPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  RTP_DUMMY_STATEMENT(Length); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

# if(RTP_DEV_ERROR_REPORT == STD_ON)
  (void)Det_ReportError(RTP_MODULE_ID, RTP_INSTANCE_ID_DET, RTP_SID_RX_INDICATION, RTP_E_API_DISABLED);
# endif
#endif /* RTP_TXSTREAM == STD_ON */

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Rtp_SoAdTriggerTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 3673 3 */ /* MD_Rtp_16.7_3673 */
FUNC(Std_ReturnType, RTP_CODE) Rtp_SoAdTriggerTransmit(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr)
{
#if(RTP_TXSTREAM == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Rtp_TxStreamIterType txStreamIdx;
  uint8 errorId = RTP_E_NO_ERROR;
  boolean pduIdMatch = FALSE;

  /* ----- Development Error Checks ------------------------------------- */
# if(RTP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters */
  if(Rtp_ModuleInitialized == RTP_UNINIT)
  {
    errorId = RTP_E_UNINIT;
  }
  else if(PduInfoPtr == NULL_PTR)
  {
    errorId = RTP_E_PARAM_POINTER;
  }
  else if(PduInfoPtr->SduDataPtr == NULL_PTR)
  {
    errorId = RTP_E_PARAM_POINTER;
  }
  else
# endif /* RTP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Search for corresponding stream */
    for(txStreamIdx = 0; txStreamIdx < Rtp_GetSizeOfTxStream(); txStreamIdx++)
    {
      /* #30 Check whether request is related to an RTP or RTCP message.
       *     Forward request to the corresponding sub-function */
      if(TxPduId == Rtp_GetRtpRtpTxPduIdOfTxStream(txStreamIdx))
      {
        retVal = Rtp_RtpTriggerTransmit((Rtp_TxStreamIdxOfStreamType)txStreamIdx, PduInfoPtr);
        pduIdMatch = TRUE;
      }
      else if(TxPduId == Rtp_GetRtpRtcpTxPduIdOfTxStream(txStreamIdx))
      {
        retVal = Rtp_RtcpTriggerTransmit((Rtp_TxStreamIdxOfStreamType)txStreamIdx, PduInfoPtr);
        pduIdMatch = TRUE;
      }
      else
      {
        /* TxPduId does not match - Proceed search */
      }

      if(pduIdMatch == TRUE)
      {
        /* Stop searching for further streams */
        break;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if(RTP_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if(errorId != RTP_E_NO_ERROR)
  {
    (void)Det_ReportError(RTP_MODULE_ID, RTP_INSTANCE_ID_DET, RTP_SID_TRIGGER_TRANSMIT_FUNCTION, errorId);
  }
# else
  RTP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif /* RTP_DEV_ERROR_REPORT == STD_ON */
#else
  Std_ReturnType retVal = E_NOT_OK;
  RTP_DUMMY_STATEMENT(TxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  RTP_DUMMY_STATEMENT(PduInfoPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

# if(RTP_DEV_ERROR_REPORT == STD_ON)
  (void)Det_ReportError(RTP_MODULE_ID, RTP_INSTANCE_ID_DET, RTP_SID_RX_INDICATION, RTP_E_API_DISABLED);
# endif
#endif /* RTP_TXSTREAM == STD_ON */

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Rtp_SetControlInformation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, RTP_CODE) Rtp_SetControlInformation(
  uint16 StreamHandleId,
  boolean StreamEnabled,
  P2CONST(uint32, AUTOMATIC, RTP_APPL_DATA) AsTimestampPtr,
  P2CONST(uint32, AUTOMATIC, RTP_APPL_DATA) RtpTimestampPtr,
  P2CONST(uint16, AUTOMATIC, RTP_APPL_DATA) GmTimeBaseIndicatorPtr,
  P2CONST(Rtp_GmIdentityType, AUTOMATIC, RTP_APPL_DATA) GmIdentityPtr)
{
#if(RTP_TXSTREAM == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Rtp_TxStreamIdxOfStreamType txStreamIdx;
  uint8 errorId = RTP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if(RTP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters */
  if(Rtp_ModuleInitialized == RTP_UNINIT)
  {
    errorId = RTP_E_UNINIT;
  }
  else if(StreamHandleId >= Rtp_GetSizeOfStream())
  {
    errorId = RTP_E_PARAM_VALUE;
  }
  else if(!Rtp_IsTxStreamUsedOfStream(StreamHandleId))
  {
    errorId = RTP_E_PARAM_VALUE;
  }
  else
# endif /* RTP_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    txStreamIdx = Rtp_GetTxStreamIdxOfStream(StreamHandleId);

    RTP_BEGIN_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    if(StreamEnabled == TRUE)
    {
      Rtp_SetStateOfDynamicTxStreamData(txStreamIdx, RTP_STREAM_ENABLED);
    }
    else
    {
      /* #20 Trigger transmission of BYE packet if stream is disabled */
      if(Rtp_GetStateOfDynamicTxStreamData(txStreamIdx) == RTP_STREAM_ENABLED)
      {
        Rtp_SetTransmitByeOfDynamicTxStreamData(txStreamIdx, TRUE);
        Rtp_SetStateOfDynamicTxStreamData(txStreamIdx, RTP_STREAM_SHUTDOWN);
      }
    }

    /* #30 Save new parameter values */
    if(AsTimestampPtr != NULL_PTR)
    {
      Rtp_SetAsTimestampOfDynamicTxStreamData(txStreamIdx, AsTimestampPtr[0]);
    }

    if(RtpTimestampPtr != NULL_PTR)
    {
      Rtp_SetRtpTimestampOfDynamicTxStreamData(txStreamIdx, RtpTimestampPtr[0]);
    }

    if(GmTimeBaseIndicatorPtr != NULL_PTR)
    {
      Rtp_SetGmTimeBaseIndicatorOfDynamicTxStreamData(txStreamIdx, GmTimeBaseIndicatorPtr[0]);
    }

    if(GmIdentityPtr != NULL_PTR)
    {
      IpBase_Copy(
        &Rtp_GetGmIdentityOfDynamicTxStreamData(txStreamIdx)[0],
        GmIdentityPtr,
        RTP_RTCP_AVB_GMIDENTITY_SIZE);
    }

    /* #40 Initialize all random values if not already done */
    while(Rtp_GetSsrcOfDynamicStreamData(StreamHandleId) == RTP_INVALID_RANDOM_NUMBER)
    {
      /* PRQA S 0310 3 */ /* MD_Rtp_11.4_0310 */
      Rtp_ExternalRandomNumberFct(
        (P2VAR(uint8, AUTOMATIC, RTP_APPL_DATA))(&Rtp_GetSequenceNumberOfDynamicTxStreamData(txStreamIdx)),
        sizeof(uint16));

      /* PRQA S 0310 3 */ /* MD_Rtp_11.4_0310 */
      Rtp_ExternalRandomNumberFct(
        (P2VAR(uint8, AUTOMATIC, RTP_APPL_DATA))(&Rtp_GetSsrcOfDynamicStreamData(StreamHandleId)),
        sizeof(uint32));
    }

    RTP_END_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if(RTP_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report default errors if any occurred */
  if(errorId != RTP_E_NO_ERROR)
  {
    (void)Det_ReportError(RTP_MODULE_ID, RTP_INSTANCE_ID_DET, RTP_SID_SET_CONTROL_INFO_FUNCTION, errorId);
  }
# else
  RTP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif /* RTP_DEV_ERROR_REPORT == STD_ON */
#else
  Std_ReturnType retVal = E_NOT_OK;
  RTP_DUMMY_STATEMENT(StreamHandleId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  RTP_DUMMY_STATEMENT(StreamEnabled); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  RTP_DUMMY_STATEMENT(AsTimestampPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  RTP_DUMMY_STATEMENT(RtpTimestampPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  RTP_DUMMY_STATEMENT(GmTimeBaseIndicatorPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  RTP_DUMMY_STATEMENT(GmIdentityPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

# if(RTP_DEV_ERROR_REPORT == STD_ON)
  (void)Det_ReportError(RTP_MODULE_ID, RTP_INSTANCE_ID_DET, RTP_SID_RX_INDICATION, RTP_E_API_DISABLED);
# endif
#endif /* RTP_TXSTREAM == STD_ON */

  return retVal;
} /* PRQA S 6010, 6030, 6060, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */

#define RTP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:

   MD_Rtp_11.4_0310:
     Reason:     Pointer cast is used to fill memory section with random data.
     Risk:       None
     Prevention: Not applicable
   MD_Rtp_16.7_3673:
     Reason:     The pointer is modified if RTP_TXSTREAM == STD_ON
     Risk:       None
     Prevention: Not applicable
*/

/**********************************************************************************************************************
 *  END OF FILE: Rtp.c
 *********************************************************************************************************************/
