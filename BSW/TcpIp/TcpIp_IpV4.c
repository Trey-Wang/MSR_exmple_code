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
 *        \file  TcpIp_IpV4.c
 *        \brief  Implementation of Internet Protocol version 4 (IPv4)
 *
 *        \details  This file is part of the TcpIp IPv4 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv4 submodule. >> TcpIp_IpV4.h
 *********************************************************************************************************************/

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* PRQA S 0828 EOF */ /* MD_MSR_1.1_828 */

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

#define TCPIP_IPV4_SOURCE

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "TcpIp_IpV4.h"
#include "TcpIp_IpV4_Cbk.h"
#include "TcpIp_IpV4_Priv.h"
#include "TcpIp_IpV4_Lcfg.h"
#if (IPV4_SUPPORT_ICMP == STD_ON)
# include "TcpIp_IcmpV4.h"
# include "TcpIp_IcmpV4_Cbk.h"
#endif
#include "TcpIp_Arp.h"
#include "TcpIp_Priv.h"

#include "IpBase.h"

#include "TcpIp_Cbk.h"  /* for IP link status change */

#if (IPV4_IP_DHCP_ADDR_CFG_SUPP == STD_ON)
# include "TcpIp_DhcpV4.h"
# include "TcpIp_DhcpV4_Cbk.h"
#endif

#if (IPV4_SUPPORT_TCP == STD_ON)
# include "TcpIp_Tcp_Cbk.h"
#endif /* IPV4_SUPPORT_TCP */

#include "TcpIp_Udp_Cbk.h"
#include "TcpIp_Lcfg.h"

#include "EthIf.h"

#if (IPV4_SUPPORT_ETHSM == STD_ON)
# include "EthSM.h"
#endif

#if (IPV4_IP_ENABLE_ADDR_NVM_STORAGE == STD_ON)
# include "NvM.h"
#endif

#if defined(TCPIP_INV_DEM_EVENT_ID)
# include "Dem.h"
#endif

#if !defined (STATIC)
# define STATIC static
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of TcpIp_IpV4.h file */
#if (  (IPV4_SW_MAJOR_VERSION != 6u) \
    || (IPV4_SW_MINOR_VERSION != 4u) \
    || (IPV4_SW_PATCH_VERSION != 1u) )
  #error "TcpIp_IpV4.c: Source and Header file are inconsistent!"
#endif

/*
Design:
- The function Ip_AddrConflictInd only indicates address conflicts for usage with IP-LL config.
  Since DHCP won't probe its addresses (design decision) no further differentiation is necessary.
*/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/
#define IPV4_IP_TXPROGR_FREE       0
#define IPV4_IP_TXPROGR_REQU       1
#define IPV4_IP_TXPROGR_PART_SENT  2

#define IPV4_IP_TB_STATUS_READY    0
#define IPV4_IP_TB_STATUS_NOT_OK   1

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
/* PRQA S 3453 FunctionLikeMacros */ /* MD_MSR_19.7 */

/* PRQA L:FunctionLikeMacros */ /* MD_MSR_19.7 */
/**********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
#define IPV4_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

#define IPV4_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/* -------------------------------------------------------------------------- */

#define IPV4_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

#if (IPV4_IP_ENABLE_REASSEMBLY == STD_ON)
STATIC VAR(uint32, IPV4_VAR_NOINIT) IpV4_Ip_TimeMs;
#endif

#define IPV4_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/* -------------------------------------------------------------------------- */

#define IPV4_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

STATIC VAR(uint16, IPV4_VAR_NOINIT) IpV4_Ip_Identification;

#define IPV4_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/* -------------------------------------------------------------------------- */

#define IPV4_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

#if IPV4_DEV_ERROR_DETECT == STD_ON
VAR(IpV4_StateType, IPV4_VAR_ZERO_INIT) IpV4_State = IPV4_STATE_UNINIT;
#endif

#define IPV4_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define IPV4_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/***********************************************************************************************************************
 *  IpV4_Ip_Init
 **********************************************************************************************************************/
/*! \brief         Initializes the Ip submodule of the IpV4 subcomponent.
 *  \details       This API call stores the start address of the post build time configuration  of the IP module
 *                 and may be used to initialize the data structures.
 *  \param[in]     Ip_ConfigType  *CfgPtr: pointer to post-build configuration
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_Init(
    P2CONST(IpV4_Ip_ConfigType, IPV4_CONST, IPV4_CONST) CfgPtr );

/***********************************************************************************************************************
 *  IpV4_Ip_MainFunction
 **********************************************************************************************************************/
/*! \brief         Handles the IP global status.
 *  \details       Has to be called every IPV4_MAIN_FCT_PERIOD_MSEC ms.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_MainFunction(void);

#if (IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON)

/***********************************************************************************************************************
 *  IpV4_Ip_VLl_GenerateIpAddr
 **********************************************************************************************************************/
/*! \brief         Generates a random link-local IP address according to IETF RFC3927.
 *  \param[in]     IpCtrlIdx     Index of the IP controller
 *  \param[in]     conflictCount Number of link-local address configuration conflicts that occured on this controller 
 *                               since last restart of the address assignment method.
 *  \return        Generated IP address is in range 169.254.1.0 to 169.254.254.255 (inclusive).
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
STATIC FUNC(IpBase_AddrInType, TCPIP_CODE) IpV4_Ip_VLl_GenerateIpAddr(uint8 IpCtrlIdx, uint8 conflictCount);

STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VLl_Reset(uint8 IpCtrlIdx);
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VLl_Start(uint8 IpCtrlIdx);
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VLl_Main(uint8 IpCtrlIdx);
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VLl_FindAddr(uint8 IpCtrlIdx);
#endif /* IPV4_IP_DYN_LL_ADDR_CFG_SUPP */

/***********************************************************************************************************************
 *  IpV4_Ip_VForwardPacketToUpperLayer()
 **********************************************************************************************************************/
/*! \brief       Forwards IP packet payload to the corresponding upper layer (TCP, UDP, ICMP) for the protocol.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 **********************************************************************************************************************/
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VForwardPacketToUpperLayer(
  uint8                      IpCtrlIdx,
  IpV4_AddrIdType            IpAddrId,
  IPV4_P2C(IpV4_Ip_HdrType)  IpHdrPtr,
  IPV4_P2V(uint8)            IpPayloadPtr,
  uint16                     IpPayloadLen);

#if (IPV4_IP_ENABLE_REASSEMBLY == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Ip_VReassemblyInitBuffers()
 **********************************************************************************************************************/
/*! \brief         Initializes all configured reassembly buffers.
 *  \details       -
 *  \context       TASK
 *  \reentrant     FALSE
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Ip_VReassemblyInitBuffers(void);

/***********************************************************************************************************************
 *  IpV4_Ip_VReassemblyFindBufferDesc()
 **********************************************************************************************************************/
/*! \brief         Locates a matching, an unused or the least recently used reassembly buffer descriptor.
 *  \details       -
 *  \param[in]     IpCtrlIdx         Index if the IP controller/instance.
 *  \param[in]     ipHdrPtr          IPv4 header containing information that shall be matched against the reassembly buffer.
 *  \param[out]    unusedDescIdxPtr  Index of an unused reassembly buffer descriptor.
 *                                   (only valid if not matching reassembly buffer descriptor was found)
 *  \param[out]    oldestDescIdxPtr  Index of the oldest (least recently used) reassembly buffer descriptor.
 *                                   (only valid if not matching or unused reassembly buffer descriptor was found)
 *  \return                          Index of a matching reassembly buffer descriptor.
 *  \return                          IPV4_IP_REASSEMBLY_BUF_DESC_IDX_INV if no match was found.
 *  \context       ISR|TASK
 *  \reentrant     FALSE
 *  \pre           -
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(uint8, IPV4_CODE) IpV4_Ip_VReassemblyFindBufferDesc(
  uint8                     IpCtrlIdx,
  IPV4_P2C(IpV4_Ip_HdrType) ipHdrPtr,
  IPV4_P2V(uint8)           unusedDescIdxPtr,
  IPV4_P2V(uint8)           oldestDescIdxPtr);

/***********************************************************************************************************************
 *  IpV4_Ip_VReassemblyGetBufferDesc()
 **********************************************************************************************************************/
/*! \brief         Returns a matching or a newly initialized reassembly buffer descriptor.
 *  \details       -
 *  \param[in]     IpCtrlIdx         Index if the IP controller/instance.
 *  \param[in]     ipHdrPtr          IPv4 header containing information that is matched against the reassembly buffer.
 *  \return                          Index of a valid reassembly buffer descriptor.
 *  \return                          IPV4_IP_REASSEMBLY_BUF_DESC_IDX_INV if there is currently no reassembly buffer available.
 *  \context       ISR|TASK
 *  \reentrant     FALSE
 *  \pre           -
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(uint8, IPV4_CODE) IpV4_Ip_VReassemblyGetBufferDesc(
  uint8                     IpCtrlIdx,
  IPV4_P2C(IpV4_Ip_HdrType) ipHdrPtr);

/***********************************************************************************************************************
 *  IpV4_Ip_VReassemblyGetBufferPtr()
 **********************************************************************************************************************/
/*! \brief         Returns the reassembly buffer that is currently assigned the the specified reassembly buffer descriptor.
 *  \details       -
 *  \param[in]     reassemblyBufDescPtr Index of a reassembly buffer descriptor.
 *  \return                             Pointer to the assigned reassembly buffer.
 *  \context       ISR|TASK
 *  \reentrant     FALSE
 *  \pre           reassemblyBufDescPtr->Status must be IPV4_IP_REASSEMBLY_BUF_STATUS_(IN_PROGRESS|COMPLETE).
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC_P2VAR(uint8, AUTOMATIC, IPV4_CODE) IpV4_Ip_VReassemblyGetBufferPtr(
  IPV4_P2C(IpV4_Ip_ReassemblyBufferDescType) reassemblyBufDescPtr);

/***********************************************************************************************************************
 *  IpV4_Ip_VReassemblyAdaptGap()
 **********************************************************************************************************************/
/*! \brief         Adapts the linked list of 'gaps' (payload/fragments not received yet) based on a received fragment.
 *  \details       -
 *  \param[in]     reassemblyBufDescPtr  Index of a reassembly buffer descriptor.
 *  \param[in]     fragOfs               Offset of the fragment payload. [bytes]
 *  \param[in]     fragEndOfs            Offset of the fragment payload end. [bytes]
 *  \param[in]     thisGapOfs            Start offset of the gap that shall be adapted. [bytes]
 *  \param[in]     thisGapEndOfs         End offset of the gap that shall be adapted. [bytes]
 *  \param[in]     prevGapOfs            Start offset of the previous gap. (position and length of current gap are stored here) [bytes]
 *  \param[in]     nextGapOfs            Start offset of the following gap. (gap may be joined with the current gap) [bytes]
 *  \note          The gaps are managed as linked list inside the reassembly buffer as suggested in
 *                 [RFC815 4. Part Two: Managing the Hole Descriptor List].
 *  \context       ISR|TASK
 *  \reentrant     FALSE
 *  \pre           -
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Ip_VReassemblyAdaptGap(
  IPV4_P2V(IpV4_Ip_ReassemblyBufferDescType) reassemblyBufDescPtr,
  uint16                                     fragOfs,
  uint16                                     fragEndOfs,
  uint16                                     thisGapOfs,
  uint16                                     thisGapEndOfs,
  uint16                                     prevGapOfs,
  uint16                                     nextGapOfs);

/***********************************************************************************************************************
 *  IpV4_Ip_VReassemblyHandleGaps()
 **********************************************************************************************************************/
/*! \brief         Returns the position inside the reassembly buffer where the payload of a received fragment shall be stored.
 *  \details       This function also updates the linked list of the not yet received payload fragments.
 *  \param[in]     reassemblyBufDescPtr  Index of a reassembly buffer descriptor.
 *  \param[in]     fragOfs               Offset of the fragment payload. [bytes]
 *  \param[in]     fragLen               Length of the fragment payload. [bytes]
 *  \param[in]     fragIsLast            TRUE if this is the last fragment, FALSE otherwise.
 *  \param[out]    gapOfsPtr             Position inside reassembly buffer where to payload of the fragment shall be copied to.
 *                                       IPV4_IP_REASSEMBLY_BUF_NO_NEXT_GAP_OFS if reassembly buffer is too small to hold the fragment.
 *  \return        E_OK                  Fragment fits into a gap. Payload can be copied to *gapOfsPtr.
 *  \return        E_NOT_OK              Fragment does not fit into a gap.
 *  \note          The gaps are managed as linked list inside the reassembly buffer as suggested in
 *                 [RFC815 4. Part Two: Managing the Hole Descriptor List].
 *  \context       ISR|TASK
 *  \reentrant     FALSE
 *  \pre           -
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(Std_ReturnType, IPV4_CODE) IpV4_Ip_VReassemblyHandleGaps(
  IPV4_P2V(IpV4_Ip_ReassemblyBufferDescType) reassemblyBufDescPtr,
  uint16                                     fragOfs,
  uint16                                     fragLen,
  boolean                                    fragIsLast,
  IPV4_P2V(uint16)                           gapOfsPtr);

/***********************************************************************************************************************
 *  IpV4_Ip_VReassemblyRxFragment()
 **********************************************************************************************************************/
/*! \brief         Receives an IP fragment and returns the reassembled IP packet if all fragments were received.
 *  \details       This function is able to handle reception of out-of-order fragments.
 *  \param[in]     reassemblyBufDescPtr  Index of a reassembly buffer descriptor.
 *  \param[in]     fragAbsByteOfs        Offset of the fragment payload. [bytes]
 *  \param[in]     fragPayloadPtr        Payload of the fragment.
 *  \param[in]     fragPayloadLen        Length of the fragment payload. [bytes]
 *  \param[in]     fragIsLast            TRUE if this is the last fragment, FALSE otherwise.
 *  \note          Result of the function is reflected by reassemblyBufDescPtr->Status.
 *  \context       ISR|TASK
 *  \reentrant     FALSE
 *  \pre           -
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Ip_VReassemblyRxFragment(
  IPV4_P2V(IpV4_Ip_ReassemblyBufferDescType) reassemblyBufDescPtr,
  uint16                                     fragAbsByteOfs,
  IPV4_P2C(uint8)                            fragPayloadPtr,
  uint16                                     fragPayloadLen,
  boolean                                    fragIsLast);

/***********************************************************************************************************************
 *  IpV4_Ip_VReassemblyRxFragmentInOrder()
 **********************************************************************************************************************/
/*! \brief         Receives an IP fragment and returns the reassembled IP packet if all fragments were received.
 *  \details       This function will abort reassembly if the fragment does directly follow the previous received fragment.
 *  \param[in]     IpCtrlIdx             Index the IP controller/instance.
 *  \param[in]     reassemblyBufDescPtr  Index of a reassembly buffer descriptor.
 *  \param[in]     fragAbsByteOfs        Offset of the fragment payload. [bytes]
 *  \param[in]     fragPayloadPtr        Payload of the fragment.
 *  \param[in]     fragPayloadLen        Length of the fragment payload. [bytes]
 *  \param[in]     fragIsLast            TRUE if this is the last fragment, FALSE otherwise.
 *  \note          Result of the function is reflected by reassemblyBufDescPtr->Status.
 *  \context       ISR|TASK
 *  \reentrant     FALSE
 *  \pre           -
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Ip_VReassemblyRxFragmentInOrder(
  uint8                                      IpCtrlIdx,
  IPV4_P2V(IpV4_Ip_ReassemblyBufferDescType) reassemblyBufDescPtr,
  uint16                                     fragAbsByteOfs,
  IPV4_P2C(uint8)                            fragPayloadPtr,
  uint16                                     fragPayloadLen,
  boolean                                    fragIsLast);

/***********************************************************************************************************************
 *  IpV4_Ip_VReassemblyCheckFragment()
 **********************************************************************************************************************/
/*! \brief         Verifies that the first and the last fragment are not received twice.
 *  \details       -
 *  \param[in]     reassemblyBufDescPtr  Index of a reassembly buffer descriptor.
 *  \param[in]     fragBlockOfs          Offset of the fragment payload. [bytes]
 *  \param[in]     fragIsLast            TRUE if this is the last fragment, FALSE otherwise.
 *  \return        TRUE                  Fragment is valid.
 *  \return        FALSE                 Fragment is not valid. (first or last fragment received twice)
 *  \context       ISR|TASK
 *  \reentrant     FALSE
 *  \pre           -
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(boolean, IPV4_CODE) IpV4_Ip_VReassemblyCheckFragment(
  IPV4_P2V(IpV4_Ip_ReassemblyBufferDescType) reassemblyBufDescPtr,
  uint16                                     fragBlockOfs,
  boolean                                    fragIsLast);

/***********************************************************************************************************************
 *  IpV4_Ip_VReassemblyRxIndication()
 **********************************************************************************************************************/
/*! \brief         Handles reception of of a fragmented IP packet.
 *  \details       -
 *  \param[in]     IpCtrlIdx          Index the IP controller/instance.
 *  \param[in]     ipHdrPtr           Header of the received IPv4 packet.
 *  \param[in,out] fragPayloadPtrPtr  In:  Payload of the received IPv4 packet.
 *                                    Out: Payload of the reassembled IPv4 packet. (if TRUE is returned)
 *  \param[in,out] fragPayloadLenPtr  In:  Payload length of the received IPv4 packet. [bytes]
 *                                    Out: Payload length of the reassembled IPv4 packet. [bytes] (if TRUE is returned)
 *  \return        TRUE               IPv4 packet reassembled. (*fragPayloadPtrPtr and *fragPayloadLenPtr updated)
 *  \return        FALSE              Packet was dropped or reassembly is still progress (other fragments missing).
 *  \context       ISR|TASK
 *  \reentrant     FALSE
 *  \pre           -
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(boolean, IPV4_CODE) IpV4_Ip_VReassemblyRxIndication(
  uint8                     IpCtrlIdx,
  IPV4_P2C(IpV4_Ip_HdrType) ipHdrPtr,
  IPV4_P2V(uint8*)          fragPayloadPtrPtr,
  IPV4_P2V(uint16)          fragPayloadLenPtr);
#endif

/***********************************************************************************************************************
 *  IpV4_Ip_VFragmentProvideTxBuffers()
 **********************************************************************************************************************/
/*! \brief         Provides the required number of ethernet buffers for transmitting the (fragmented) IP packet.
 *  \details       -
 *  \param[in]     IpCtrlIdx          Index the IP controller/instance.
 *  \param[in]     socketId           Index of the upper layer socket.
 *  \param[in]     ipPayloadLen       Length of the IP payload. [bytes]
 *                                    If this length exceeds the IP MTU, multiple buffers are provided.
 *  \param[in,out] ipTxReqDescrPtr    IP TX request descriptor.
 *  \return        E_OK               Buffer(s) provided.
 *  \return        FALSE              No buffer provided.
 *  \context       ISR|TASK
 *  \reentrant     FALSE
 *  \pre           -
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(BufReq_ReturnType, IPV4_CODE) IpV4_Ip_VFragmentProvideTxBuffers(
  uint8                                     IpCtrlIdx,
  TcpIp_SockIdxType                         socketId,
  uint16                                    ipPayloadLen,
  IPV4_P2V(TcpIp_IpTxRequestDescriptorType) ipTxReqDescrPtr);

/***********************************************************************************************************************
 *  IpV4_Ip_VStoreProtocolChecksum
 **********************************************************************************************************************/
/*! \brief         Stores the protocol checksum value at the correct offset inside the protocol header.
 *  \details       -
 *  \param[in]     protocol           Upper layer protocol.
 *                                    (TCPIP_PROTOCOL_TCP,  TCPIP_PROTOCOL_UDP, IPV4_IP_PROTOCOL_ICMP).
 *  \param[in]     protocolChecksum   Calculated Internet checksum value.
 *  \param[in]     protocolHeaderPtr  Position of the beginning of the upper layer protocol header.
 *  \context       ISR|TASK
 *  \reentrant     FALSE
 *  \pre           -
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Ip_VStoreProtocolChecksum(
  uint8           protocol,
  uint16          protocolChecksum,
  IPV4_P2V(uint8) protocolHeaderPtr);
  
/***********************************************************************************************************************
 *  IpV4_Ip_VTransmitFragments
 **********************************************************************************************************************/
/*! \brief         Transmits an IP packet or all fragments of an IP packet, if the packet is fragmented.
 *  \details       -
 *  \param[in]     IpCtrlIdx          Index the IP controller/instance.
 *  \param[in]     ipTxReqDescrPtr    IP TX request descriptor.
 *  \param[in]     ipProtocol         IP protocol type.
 *                                    (TCPIP_PROTOCOL_TCP,  TCPIP_PROTOCOL_UDP, IPV4_IP_PROTOCOL_ICMP).
 *  \param[in]     ipPayloadLen       Total length of the IP payload that shall be sent.
 *                                    Must not be bigger than the requested length in Ip_ProvideTxBuffer.
 *                                    If value is less than the requested length only the required number
 *                                    fragments is transmitted and the remaining fragment buffers are released.
 *  \param[in]     destPhysAddrPtr    Destination IP address and port.
 *  \param[in]     calculateChecksum  Calculate Internet checksum of upper layer protocol in software.
 *  \param[in]     reqTxConfirmation  Request a TX confirmation from lower layer (EthIf).
 *  \param[in]     ulReqIdx           Upper layer request index.
 *                                    User defined value that will be passed to to TxConfirmation callback.
 *  \return        E_OK               Transmit of all fragments accepted.
 *  \return        FALSE              Transmit failed.
 *  \context       ISR|TASK
 *  \reentrant     FALSE
 *  \pre           -
 **********************************************************************************************************************/
IPV4_LOCAL FUNC(Std_ReturnType, IPV4_CODE) IpV4_Ip_VTransmitFragments(
    uint8                                     IpCtrlIdx,
    IPV4_P2C(TcpIp_IpTxRequestDescriptorType) ipTxReqDescrPtr,
    uint8                                     ipProtocol,
    uint16                                    ipPayloadLen,
    IPV4_P2V(uint8)                           destPhysAddrPtr,
    boolean                                   calculateChecksum,
    boolean                                   reqTxConfirmation,
    uint8                                     ulReqIdx);

/***********************************************************************************************************************
 *  IpV4_Ip_VGetIpAddrIdAndCtrlIdx
 **********************************************************************************************************************/
/*! \brief         Returns the IP controller index of the given addrId or finds a suitable controller index and addrId
 *                 that matches best for the destAddr.
 *  \details       -
 *  \param[in,out] addrIdPtr  In:  IP address identifier for which the IP controller index shall be returned or
 *                                 IPV4_ADDR_ID_ANY, if IP address identifier and controller index shall be determined
 *                                 using destAddr.
 *                            Out: IP address identifier that belongs to returned IP controller.
 *  \param[in]     destAddr   Destination IP address.
 *  \return        Index of the matching IP controller instance or IPV4_CTRL_IDX_INV if no matching controller exists.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \pre           -
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(uint8, IPV4_CODE) IpV4_Ip_VGetIpAddrIdAndCtrlIdx(
  IPV4_P2V(IpV4_AddrIdType)                 addrIdPtr,
  IpBase_AddrInType                         destAddr);

/***********************************************************************************************************************
 *  IpV4_Ip_VGetNextHopPhysAddr
 **********************************************************************************************************************/
/*! \brief         Returns the link layer (physical/MAC) address of the next hop that shall be used to reach destAddr.
 *  \details       -
 *  \param[in]     IpCtrlIdx           Index the IP controller/instance.
 *  \param[in]     destAddr            Destination IP address.
 *  \param[in]     socketId            Upper layer socket index. (used for optimized/cached lookup)
 *  \param[out]    nextHopPhysAddrPtr  Address of the next hop.
 *  \return        E_OK                Next hop address returned.
 *  \return        E_NOT_OK            No next hop known. Destination not reachable.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \pre           -
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(TcpIp_ReturnType, IPV4_CODE) IpV4_Ip_VGetNextHopPhysAddr(
  uint8                 IpCtrlIdx,
  IpBase_AddrInType     destAddr,
  TcpIp_SocketIdType    socketId,
  IPV4_P2V(uint8)       nextHopPhysAddrPtr);

#if (IPV4_IP_ENABLE_MULTICAST_RX == STD_ON)
# if (IPV4_ENABLE_ETHIF_ADDR_FILTER_API == STD_ON)
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VJoinMulticastGroup(uint8 IpCtrlIdx, IpBase_AddrInType MulticastAddr, boolean Join);
# endif
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VUpdateMulticastMemberships(uint8 IpCtrlIdx, boolean Join);
#endif

/* PRQA S 3453 5 */ /* MD_MSR_19.7 */
#if (IPV4_SUPPORT_ETHSM == STD_ON)
# define ETHSM_TCPIP_MODE_INDICATION(CtrlIdx, IsIpAddrAssigned) (void)EthSM_VTcpIpModeIndication((CtrlIdx), (IsIpAddrAssigned))
#else
# define ETHSM_TCPIP_MODE_INDICATION(CtrlIdx, IsIpAddrAssigned)
#endif

/***********************************************************************************************************************
 *  IpV4_Ip_VGetBestAvailableAddrAssignment()
 **********************************************************************************************************************/
/*! \brief      Find available IP address assignment method that has highest priority (STATIC, DHCP, LINKLOCAL).
 *  \param[in]  IpCtrlIdx                 Index of the IP controller.
 *  \param[out] addrAssignmentPriorityPtr Priority of the returned assignment method (Range 1..3 based on configuration).
 *  \param[out] addrInfoPtr               IP address information (IP address, Netmask, Default Router) tuple.
 *  \return     IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE      No IP address assignment ready.
 *  \return     IPV4_IP_ADDR_ASSIGNMENT_METHOD_DHCP
 *  \return     IPV4_IP_ADDR_ASSIGNMENT_METHOD_LINKLOCAL
 *  \return     IPV4_IP_ADDR_ASSIGNMENT_METHOD_STATIC
 *  \context    TASK
 *  \reentrant  TRUE for different controllers.
 **********************************************************************************************************************/
STATIC FUNC(uint8, IPV4_CODE) IpV4_Ip_VGetBestAvailableAddrAssignment(
  uint8                         IpCtrlIdx, 
  IPV4_P2V(uint8)               addrAssignmentPriorityPtr,
  IPV4_P2V(IpV4_IpAddrSetType)  addrInfoPtr);

/***********************************************************************************************************************
 *  IpV4_Ip_VGetActiveAddressOfAssignmentMethod()
 **********************************************************************************************************************/
/*! \brief      Checks whether an address assignment method is ready and returns the address parameters.
 *  \param[in]  IpCtrlIdx             Index of the IP controller.
 *  \param[in]  addrAssignmentMethod  Address assignment method (DHCP, LINKLOCAL, STATIC).
 *  \param[out] addrInfoPtr           IP address information (IP address, Netmask, Default Router) tuple.
 *  \return     TRUE                  Address assignment method is ready.
 *  \return     FALSE                 Address assignment method is not ready. (No address available)
 *  \context    TASK
 *  \reentrant  TRUE for different controllers.
 **********************************************************************************************************************/
STATIC FUNC(boolean, IPV4_CODE) IpV4_Ip_VGetActiveAddressOfAssignmentMethod(
  uint8                        IpCtrlIdx, 
  uint8                        addrAssignmentMethod,
  IPV4_P2V(IpV4_IpAddrSetType) addrInfoPtr);

/***********************************************************************************************************************
 *  IpV4_Ip_VTriggerAddrAssignment()
 **********************************************************************************************************************/
/*! \brief Set/Unset address assignment trigger flag and start/stop dynamic (DHCP, LINKLOCAL) assignment if necessary.
 *  \param[in]  IpCtrlIdx                 Index of the IP controller.
 *  \param[in]  addrAssignmentCfg         IP address assignment configuration set.
 *  \param[in]  triggerValue              Specifies whether the flag shall be set (TRUE) or unset (FALSE).
 *  \return     E_OK     Flag was changed.
 *  \return     E_NOT_OK Flag was already set/unset.
 *  \context    TASK
 *  \reentrant  TRUE for different controllers.
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Ip_VTriggerAddrAssignment(
    uint8   IpCtrlIdx,
    uint8   addrAssignmentCfg,
    boolean triggerValue);

/***********************************************************************************************************************
 *  IpV4_Ip_VTriggerAutomaticAddrAssignments()
 **********************************************************************************************************************/
/*! \brief      Sets trigger flags for all configured unicast address assignments with trigger AUTOMATIC.
 *  \param[in]  IpCtrlIdx  Index of IP controller.
 *  \context    TASK
 *  \reentrant  TRUE for different controllers.
 **********************************************************************************************************************/
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VTriggerAutomaticAddrAssignments(uint8 IpCtrlIdx);

/***********************************************************************************************************************
 *  IpV4_Ip_VSetCtrlStateOffline()
 **********************************************************************************************************************/
/*! \brief Sets IP controller into state OFFLINE and perform required operations based on current state.
 *  \details    In state OFFLINE the IP controller does not send any messages.
 *              This state is entered based on the current state of the ethernet transceiver.
 *  \param[in]  IpCtrlIdx  Index of IP controller.
 *  \context    TASK
 *  \reentrant  TRUE for different controllers.
 **********************************************************************************************************************/
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VSetCtrlStateOffline(uint8 IpCtrlIdx);

/***********************************************************************************************************************
 *  IpV4_Ip_VSetCtrlStateOnlinePassive()
 **********************************************************************************************************************/
/*! \brief Sets IP controller into state ONLINE_PASSIVE and perform required operations based on current state.
 *  \details    In state ONLINE_PASSIVE the IP controller is online but has not completed unicast address assignment.
 *  \param[in]  IpCtrlIdx  Index of IP controller.
 *  \context    TASK
 *  \reentrant  TRUE for different controllers.
 **********************************************************************************************************************/
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VSetCtrlStateOnlinePassive(uint8 IpCtrlIdx);

/***********************************************************************************************************************
 *  IpV4_Ip_VSetCtrlStateOnlineActive()
 **********************************************************************************************************************/
/*! \brief      Sets IP controller into state ONLINE_ACTIVE and perform required operations based on current state.
 *  \details    In state ONLINE_ACTIVE the IP controller is online and has completed unicast address assignment.
 *  \param[in]  IpCtrlIdx  Index of IP controller.
 *  \context    TASK
 *  \reentrant  TRUE for different controllers.
 **********************************************************************************************************************/
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VSetCtrlStateOnlineActive(uint8 IpCtrlIdx);

/***********************************************************************************************************************
 *  IpV4_Ip_VCtrlIsOnline
 **********************************************************************************************************************/
/*! \brief      Determines whether a IP instance/controller is ONLINE.
 *  \details    -
 *  \param[in]  ipCtrlIdx  Index of IP controller.
 *  \context    TASK
 *  \reentrant  TRUE for different controllers.
 *  \pre        -
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(boolean, IPV4_CODE) IpV4_Ip_VCtrlIsOnline(uint8 ipCtrlIdx);

/***********************************************************************************************************************
 *  IpV4_VLocalIpAddrAssignmentChange()
 **********************************************************************************************************************/
/*! \brief      Notifies upper layers about an IP address assignment change of a unicast or multicast address.
 *  \param[in]  IpCtrlIdx    Index of IP controller.
 *  \param[in]  ctrlAddrIdx  Index of the IP address of the controller.
 *  \context    TASK
 *  \reentrant  TRUE for different controllers.
 **********************************************************************************************************************/
STATIC FUNC(void, IPV4_CODE) IpV4_VLocalIpAddrAssignmentChange(
    uint8                 IpCtrlIdx,
    uint8                 CtrlAddrIdx,
    TcpIp_IpAddrStateType State);

#if (IPV4_IP_ENABLE_MULTICAST_RX == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Ip_VChangeMulticastAddrAssignment
 **********************************************************************************************************************/
/*! \brief      Changes value of a multicast address.
 *  \param[in]  IpCtrlIdx       Index of the IP controller.
 *  \param[in]  mcAddrIdx       Index of the multicast address.
 *  \param[in]  addrValue       Value that shall be assigned to the specified multicast address index.
 *  \return     E_OK            Address was changed.
 *  \return     E_NOT_OK        Address could not be changed.
 *  \context    TASK
 *  \reentrant  TRUE for different controllers.
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Ip_VChangeMulticastAddrAssignment(
    uint8             IpCtrlIdx,
    uint8             mcAddrIdx,
    IpBase_AddrInType addrValue);
#endif

#if defined TCPIP_INV_DEM_EVENT_ID
/***********************************************************************************************************************
 *  IpV4_ResetDemEventAllIpCtrl
 **********************************************************************************************************************/
/*! \brief      Sets the status of a DEM event to 'passed' for all IpCtrlIdx that the event is configured for.
 *  \param[in]  CONSTP2CONST(PduIdType, IPV4_CONST) DemEventSnvsByIpV4Ctrl: Array of the SNVs of the DEM event.
 *  \param[in]  uint8 NumIpV4CtrlsOfDemEvent: Max. number of ip controllers that have the DEM event configured.
 *  \context    TASK
 *  \reentrant  FALSE
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_ResetDemEventAllIpCtrl(
  CONSTP2CONST(PduIdType, AUTOMATIC, IPV4_CONST) DemEventSnvsByIpV4Ctrl,
  uint8 NumIpV4CtrlsOfDemEvent
);

/***********************************************************************************************************************
 *  IpV4_ResetDemEventIpCtrl
 **********************************************************************************************************************/
/*! \brief      Sets the status of a DEM event to 'passed' for given IpCtrlIdx that the event is configured for.
 *  \param[in]  CONSTP2CONST(PduIdType, IPV4_CONST) DemEventSnvsByIpV4Ctrl: Array of the SNVs of the DEM event.
 *  \param[in]  uint8 NumIpV4CtrlsOfDemEvent: Max. number of ip controllers that have the DEM event configured.
 *  \param[in]  uint8 IpCtrlIdx: Ip controller that the DEM event shall be reset for.
 *  \context    TASK
 *  \reentrant  FALSE
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_ResetDemEventIpCtrl(
  CONSTP2CONST(PduIdType, AUTOMATIC, IPV4_CONST) DemEventSnvsByIpV4Ctrl,
  uint8 NumIpV4CtrlsOfDemEvent,
  uint8 IpCtrlIdx
);

#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  IpV4_InitMemory
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_InitMemory(void)
{
#if (IPV4_DEV_ERROR_DETECT == STD_ON)
  IpV4_State = IPV4_STATE_UNINIT;
#endif
  /* IPV4_DEV_ERROR_DETECT == STD_ON */
}

/***********************************************************************************************************************
 *  IpV4_Init
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPV4_CODE) IpV4_Init(
    P2CONST(IpV4_ConfigType, IPV4_CONST, IPV4_CONST) CfgPtr )
{
  IPV4_UNUSED_PARAMETER(CfgPtr);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #10 Initialize sub components (ARP, ICMP and IP). */
  IpV4_Arp_Init( NULL_PTR );
# if (IPV4_SUPPORT_ICMP == STD_ON)
  IpV4_Icmp_Init( NULL_PTR );
# endif
  /* IPV4_SUPPORT_ICMP == STD_ON */
  IpV4_Ip_Init( NULL_PTR );

#if (IPV4_DEV_ERROR_DETECT == STD_ON)
  IpV4_State = IPV4_STATE_INIT;
#endif

#if (IPV4_IP_ENABLE_REASSEMBLY == STD_ON)
  IpV4_Ip_TimeMs = 0;
#endif

#if defined TCPIP_INV_DEM_EVENT_ID
# if (TcpIp_NumIpV4CtrlsOfDemEventTCPIP_E_DUPLICATE_DHCP_ADDR > 0)
  IpV4_ResetDemEventAllIpCtrl(TcpIp_DemEventTCPIP_E_DUPLICATE_DHCP_ADDRSnvsByIpV4Ctrl, TcpIp_NumIpV4CtrlsOfDemEventTCPIP_E_DUPLICATE_DHCP_ADDR);
  /* Initialize array of last duplicated DHCP DID data */
  {
    uint8 IpCtrlIdx;  /* PRQA S 0781 */ /* MD_IpV4_5.6 */
    uint8 initPhysAddr[IPBASE_ETH_PHYS_ADDR_LEN_BYTE] = {0,0,0,0,0,0};
    for(IpCtrlIdx = 0; IpCtrlIdx < TcpIp_NumIpV4CtrlsOfDemEventTCPIP_E_DUPLICATE_DHCP_ADDR; IpCtrlIdx++)
    {
      IpV4_VAssignPhysAddr(TcpIp_LastDuplicateDhcpNetAddr[IpCtrlIdx].PhysAddr, initPhysAddr);
      TcpIp_LastDuplicateDhcpNetAddr[IpCtrlIdx].NetAddr = 0u;
    }
  }
# endif
#endif
} /* IpV4_Init */

/***********************************************************************************************************************
 *  IpV4_MainFunction
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
FUNC(void, IPV4_CODE) IpV4_MainFunction(void)
{
  /* ------------------------------------------------- */
  /* DET */
  IpV4_CheckDetErrorReturnVoid( IPV4_COND_INIT, IPV4_API_ID_MAIN_FUNCTION, IPV4_E_NOT_INITIALIZED )
  /* ------------------------------------------------- */

#if (IPV4_IP_ENABLE_REASSEMBLY == STD_ON)
  /* #10 Update time variable used for timeout handling. */
  IpV4_Ip_TimeMs += IPV4_MAIN_FCT_PERIOD_MSEC;
#endif

#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
  /* #20 Call ARP MainFunction. */
  /* ARP MainFunction is only necessary if ECU needs to perform active address resolution */
  IpV4_Arp_MainFunction();
#endif

  /* #30 Call IP MainFunction. */
  IpV4_Ip_MainFunction();

#if (IPV4_SUPPORT_ICMP == STD_ON)
  /* #40 Call ICMP MainFunction. */
  IpV4_Icmp_MainFunction();
#endif

#if (IPV4_IP_DHCP_ADDR_CFG_SUPP == STD_ON)
  /* #50 Call DHCP MainFunction. */
  TcpIp_DhcpV4_MainFunction();
#endif
  /* TCPIP_SUPPORT_DHCPV4 == STD_ON */
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  IpV4_Ip_Init
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
 */
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_Init(
    P2CONST(IpV4_Ip_ConfigType, IPV4_CONST, IPV4_CONST) CfgPtr )
{
#if(IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8 IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV4_5.6 */
#endif

  TcpIp_SockIdxType TcpIpSockIdx;   /* IpV4_Ip_TransferBlock / socket index */

  IPV4_UNUSED_PARAMETER(CfgPtr);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  IpV4_Ip_Identification = 0;

  /* #10 Iterate over all IP controller instances. */
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  for (IpCtrlIdx = 0; IpCtrlIdx < IpV4_VCfgGetMaxNumCtrls(); IpCtrlIdx++) /* PRQA S 0781 */ /* MD_IpV4_05.06 */
#endif
  {
    /* #20 Set IP controller instance to state OFFLINE. */
    IPV4_CTRL_VAR(IpV4_Ip_CtrlState)                    = IPV4_IP_OFFLINE;
    IPV4_CTRL_VAR(IpV4_Ip_PhysLinkActive)               = FALSE;
    IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentReadyChanged)   = FALSE;
    IPV4_CTRL_VAR(IpV4_Ip_ActiveAddrAssignmentMethod)   = IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE;
    IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentTriggeredFlags) = 0;
#if (IPV4_IP_ENABLE_ADDR_NVM_STORAGE == STD_ON)
    IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentMethodPersist)  = IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE;
#endif

    /* #30 Reset Multicast address assignment. */
#if (IPV4_IP_ENABLE_MULTICAST_RX == STD_ON)
    {
      uint8_least mcAddrIdx;
      for (mcAddrIdx = 0; mcAddrIdx < IpV4_Ip_VCfgGetMulticastAddrCount(); mcAddrIdx++)
      {
        IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs)[mcAddrIdx] = IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs_Default)[mcAddrIdx];
      }
    }
#endif

    /* #40 Reset LinkLocal address assignment. */
#if (IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON)
    if (IpV4_Ip_VCfgIsAddrAssignmentEnabled(IPV4_IP_ADDR_ASSIGNMENT_METHOD_LINKLOCAL))
    {
      IpV4_Ip_VLl_Reset(IPV4_IPCTRLIDX);
    }
#endif
  } /* for each controller */

  /* #50 Reset socket specific information. */

  TcpIpSockIdx = IPV4_IP_MAX_NUM_SOCKETS;

  /* for each socket */
  while (0 < TcpIpSockIdx)
  {
    TcpIpSockIdx--;
    IpV4_Ip_ResetSocket(TcpIpSockIdx);
  }

  /* #60 Initialize packet reassembly buffers. */

#if (IPV4_IP_ENABLE_REASSEMBLY == STD_ON)
  IpV4_Ip_VReassemblyInitBuffers();
#endif
}

/***********************************************************************************************************************
 *  IpV4_Ip_MainFunction
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_MainFunction(void)
{
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8 IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV4_5.6 */
#endif

  /* ------------------------------------------------- */
  /* DET */
  IpV4_CheckDetErrorReturnVoid( IPV4_COND_INIT, IPV4_IP_API_ID_MAIN_FUNCTION, IPV4_E_NOT_INITIALIZED )
  /* ------------------------------------------------- */

  /* #10 Iterate over all IP controller instances. */
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  for (IpCtrlIdx = 0; IpCtrlIdx < IpV4_VCfgGetMaxNumCtrls(); IpCtrlIdx++)
#endif
  {
#if (IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON)
    /* #20 Call link local address assignment MainFunction if configured. */
    if (IpV4_Ip_VCfgIsAddrAssignmentEnabled(IPV4_IP_ADDR_ASSIGNMENT_METHOD_LINKLOCAL))
    {
      IpV4_Ip_VLl_Main(IPV4_IPCTRLIDX);
    }
#endif

    /* #30 Perform IP instance state and address assignment handling. */
    if (IPV4_CTRL_VAR(IpV4_Ip_CtrlState) == IPV4_IP_OFFLINE)
    {
      if (IPV4_CTRL_VAR(IpV4_Ip_PhysLinkActive) == TRUE)
      {
        /* IP instance is OFFLINE and physical link has become active. */
        IpV4_Ip_VSetCtrlStateOnlinePassive(IPV4_IPCTRLIDX);
      }
    }
    else
    {
      if (IPV4_CTRL_VAR(IpV4_Ip_PhysLinkActive) == TRUE)
      {
#if (IPV4_IP_ENABLE_ADDR_NVM_STORAGE == STD_ON)
        /* #40 Store IP address into NvM if required. */
        if (IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentMethodPersist) != IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE)
        {
          uint8 NvMStatus = NVM_REQ_PENDING;
          
          TCPIP_ASSERT(IpV4_Ip_VCfgGetNvmAddrVar() != NULL_PTR);

          (void)NvM_GetErrorStatus(TCPIP_NVM_BLOCK_ID, &NvMStatus);

          if(NVM_REQ_PENDING != NvMStatus)
          {
            if (IpV4_Ip_VGetActiveAddressOfAssignmentMethod(IpCtrlIdx, IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentMethodPersist), IpV4_Ip_VCfgGetNvmAddrVar()) == TRUE)
            {
              /* set block Status to true */
              (void)NvM_SetRamBlockStatus(TCPIP_NVM_BLOCK_ID, TRUE);
              IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentMethodPersist) = IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE;
            }

          }
        }
        else
#endif
        {
          uint8              bestAvailableAddrAssignmentMethod;
          uint8              bestAvailableAddrAssignmentPriority;
          IpV4_IpAddrSetType bestAvailableAddr;

          bestAvailableAddrAssignmentMethod = IpV4_Ip_VGetBestAvailableAddrAssignment(IPV4_IPCTRLIDX, &bestAvailableAddrAssignmentPriority, &bestAvailableAddr);

          if (IPV4_CTRL_VAR(IpV4_Ip_ActiveAddrAssignmentMethod) == IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE)
          {
            if (bestAvailableAddrAssignmentMethod == IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE)
            {
              /* Still no IP address of any configured assignment method available. */
            }
            else
            {
              /* IP address ready. */
              IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr)                 = bestAvailableAddr;
              IPV4_CTRL_VAR(IpV4_Ip_ActiveAddrAssignmentMethod) = bestAvailableAddrAssignmentMethod;

              IpV4_Ip_VSetCtrlStateOnlineActive(IPV4_IPCTRLIDX);
            }
          }
          else
          {
            if (bestAvailableAddrAssignmentMethod != IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE)
            {
              /* IP address available still available. */
              if (IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr != bestAvailableAddr.NetAddr)
              {
                /* IP address value has changed. Inform upper-layer. */
                IpV4_VLocalIpAddrAssignmentChange(IPV4_IPCTRLIDX, IPV4_CTRL_ADDR_IDX_FIRST, TCPIP_IPADDR_STATE_UNASSIGNED);

                IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr)                 = bestAvailableAddr;
                IPV4_CTRL_VAR(IpV4_Ip_ActiveAddrAssignmentMethod) = bestAvailableAddrAssignmentMethod;

                IpV4_VLocalIpAddrAssignmentChange(IPV4_IPCTRLIDX, IPV4_CTRL_ADDR_IDX_FIRST, TCPIP_IPADDR_STATE_ASSIGNED);
              }
              else if (IPV4_CTRL_VAR(IpV4_Ip_ActiveAddrAssignmentMethod) != bestAvailableAddrAssignmentMethod)
              {
                /* IP address assignment method has changed but IP address is the same. No need to inform upper-layer. */
                IPV4_CTRL_VAR(IpV4_Ip_ActiveAddrAssignmentMethod) = bestAvailableAddrAssignmentMethod;
              }
              else
              {
                /* IP address and assignment method did not change. */
              }
            }
            else
            {
              /* IP address lost. */
              IpV4_Ip_VSetCtrlStateOnlinePassive(IPV4_IPCTRLIDX);
            }
          }
        }
      }
      else
      {
        /* Physical link lost. */
        IpV4_Ip_VSetCtrlStateOffline(IPV4_IPCTRLIDX);
      }
    }
  } /* for each controller */
} /* PRQA S 2006, 6010, 6030, 6050, 6080 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  IpV4_Ip_RxIndication
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
 */
FUNC(void, IPV4_CODE) IpV4_Ip_RxIndication(
  uint8                                    CtrlIdx,
  Eth_FrameType                            FrameType,
  boolean                                  IsBroadcast,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr, /* PRQA S 3673 */ /* MD_IPV4_16.7_3673 */
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  uint16                                   LenByte)
{
  IPV4_P2V(IpV4_Ip_HdrType) IpHeaderPtr = (IPV4_P2V(IpV4_Ip_HdrType))DataPtr; /* PRQA S 0310 */ /* MD_IpV4_11.04 */
  IPV4_P2V(uint8)           IpPayloadPtr; /* PRQA S 0781 */ /* MD_IpV4_5.6 */
  uint8                     IpHdrLenByte;
  uint16                    IpTotalLenByte;
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8 IpCtrlIdx;           /* PRQA S 0781 */ /* MD_IpV4_5.6 */
#endif

  IPV4_UNUSED_PARAMETER(FrameType);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  IPV4_UNUSED_PARAMETER(IsBroadcast);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

#if defined TCPIP_INV_DEM_EVENT_ID
# if (TcpIp_NumIpV4CtrlsOfDemEventTCPIP_E_DUPLICATE_DHCP_ADDR > 0)
  /* PhysAddrPtr is used in case DEM event is triggered */
# endif
#else
  IPV4_UNUSED_PARAMETER(PhysAddrPtr);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  TCPIP_ASSERT(IpV4_State == IPV4_STATE_INIT);
  TCPIP_ASSERT(DataPtr    != NULL_PTR);

  /* ------------------------------------------------- */

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  if (!IPV4_CTRL_IDX_ETHIF_2_IP(CtrlIdx, IPV4_IPCTRLIDX))
#else
  if (CtrlIdx != IPV4_CTRL_IDX_IP_2_ETHIF(IPV4_IPCTRLIDX))
#endif
  {
    return;
  }

  /* #10 Check, that the IP controller is not offline and the given packet length is valid. */
  if(IPV4_CTRL_VAR(IpV4_Ip_CtrlState) == IPV4_IP_OFFLINE)
  {
    /* IpV4 is shut down, so do not process any incoming message -> drop message */
    return;
  }

  if(IPV4_IP_DEF_HEADER_LENGTH_BYTE > LenByte)
  {
    /* invalid message length, message is smaller than minimum IP header length -> drop message */
    return;
  }

  /* #20 Extract the header and total packet length. */  
  IpHdrLenByte   = IPV4_HDR_FIELD_GET_IHL_BYTE(IpHeaderPtr);
  IpTotalLenByte = IPV4_HDR_FIELD_GET_TOTAL_LEN(IpHeaderPtr);

  /* #30 Check IP header length, IP version number and consistency of length values of the IP packet. */
  /* check IP header length: */
  if(IpHdrLenByte >= IPV4_IP_DEF_HEADER_LENGTH_BYTE)
  {
    /* check IP version number: */
    if (IPV4_HDR_FIELD_GET_VERSION(IpHeaderPtr) == IPV4_IP_HEADER_VERSION_V4)
    {
      /* Check if indicated length equals length provided in IP header.
         -> if not, IP fragmentation is used (not supported) or wrong length is indicated */
      if(LenByte < IpTotalLenByte)
      {
        /* message seams to be fragmented -> drop message */
        return;
      }

      if (IpHdrLenByte > IpTotalLenByte)
      {
        /* total length (including IP header) is smaller than IP header length. -> drop message */
        return;
      }
      /* #40 Check the header checksum, if not already done in hardware. */
#if (IPV4_IP_CHECK_RX_CHECKSUM == STD_ON)
#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_OFF)  /* If the hardware supports checking the checksum, the check if the checksum is ok will be done later. */
      /* check header checksum */
      if(0 == IpBase_TcpIpChecksumAdd((IPV4_P2C(uint8))IpHeaderPtr, IpHdrLenByte, 0, TRUE)) /* PRQA S 0310 */ /* MD_IpV4_11.04 */
#endif /* TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED */
#endif /* IP_CHECK_RX_CHECKSUM */
      {
        IpV4_AddrIdType       AddrId;      /* PRQA S 0781 */ /* MD_IpV4_5.6 */

        /* #50 Discard the packet in case the source address is not broadcast or multicast. */
        /* check source address */
        if(TCPIP_INADDR_BROADCAST == IpHeaderPtr->SrcAddr)
        {
          /* According to RFC 791, section 3.3 ("The internet module must check to see that the
             source address is one of the legal address for this host."), and RFC 1122, section
             3.2.1.3, case c ("It MUST NOT be used as a source address."), the broadcast address
             may not be used as a source address. Therefore an incoming message with broadcast addr
             as the source addr must be discarded. */
          /* discard message immediately */
          return;
        }

        if ((IPV4_CTRL_VAR(IpV4_Ip_CtrlState) == IPV4_IP_ONLINE_ACTIVE) &&
            (IpHeaderPtr->SrcAddr == (IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr | ~(IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetMask))))
        {
          /* This message uses the local broadcast address as source address */
          /* discard message immediately */
          return;
        }

        if(IPV4_ADDR_IS_MULTICAST(IpHeaderPtr->SrcAddr))
        {
          /* This message uses a multicast address as source address */
          /* discard message immediately */
          return;
        }

        /* #60 In case the IP controller is online and it is a DHCP IP address, check for a duplicate DHCP address. */
        if ((IPV4_CTRL_VAR(IpV4_Ip_CtrlState) == IPV4_IP_ONLINE_ACTIVE) &&
            (IpHeaderPtr->SrcAddr == IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr))
        {
          /* This message uses the node's local IP address as source address */
#if defined TCPIP_INV_DEM_EVENT_ID
#  if (TcpIp_NumIpV4CtrlsOfDemEventTCPIP_E_DUPLICATE_DHCP_ADDR > 0)
          /* Issue the related DEM event in case it's a DHCP address */
          if(    ( IPV4_CTRL_VAR(IpV4_Ip_ActiveAddrAssignmentMethod) == IPV4_IP_ADDR_ASSIGNMENT_METHOD_DHCP )
              && ( TcpIp_DemEventTCPIP_E_DUPLICATE_DHCP_ADDRSnvsByIpV4Ctrl[IpCtrlIdx] != TCPIP_INV_DEM_EVENT_ID )
            )
          {
            IpV4_VAssignPhysAddr(TcpIp_LastDuplicateDhcpNetAddr[IpCtrlIdx].PhysAddr, PhysAddrPtr);
            TcpIp_LastDuplicateDhcpNetAddr[IpCtrlIdx].NetAddr = IpHeaderPtr->SrcAddr;
            Dem_ReportErrorStatus(TcpIp_DemEventTCPIP_E_DUPLICATE_DHCP_ADDRSnvsByIpV4Ctrl[IpCtrlIdx], DEM_EVENT_STATUS_PREFAILED);
          }
#  endif
#endif
          /* discard message immediately */
          return;
        }

        if((0 == IpHeaderPtr->SrcAddr) && (IpHeaderPtr->Prot == IPV4_IP_PROTOCOL_TCP))
        {
          /* This message uses the invalid address as source address */
          /* UDP messages with source address 0.0.0.0 are accepted since a DHCPv4 server
           * and link-local address configuration must be able to receive UDP packets sent from 0.0.0.0.
           */
          /* discard message immediately */
          return;
        }

        if(IPV4_ADDR_IN_SUBNET(IpHeaderPtr->SrcAddr, IPV4_LOCALNET_PREFIX, IPV4_LOCALNET_PREFIX_MASK))
        {
          /* This message uses a localnet address as source address */
          /* discard message immediately */
          return;
        }

        /* check destination IP address of the message */
        /* if node:
             check if IP packet is intended for this node (unicast, multicast, broadcast)
             if unicast: check if IP address matches node address (dependent on CtrlIdx)
           if router:
             not implemented */

        if (IpHeaderPtr->DestAddr == IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr) /* local IP address */
        {
          AddrId      = IPV4_CTRL_VAR(IpV4_Ip_AddrConfig)[IPV4_CTRL_ADDR_IDX_FIRST].IpAddrId;
        }
        else if (IPV4_ADDR_IS_BROADCAST(IpHeaderPtr->DestAddr)) /* local or directed broadcast */
        {
          AddrId      = IPV4_CTRL_VAR(IpV4_Ip_BcAddrId);
          IPV4_CTRL_VAR(IpV4_Ip_LastBcAddr) = IpHeaderPtr->DestAddr;
        }
#if (IPV4_IP_ENABLE_MULTICAST_RX == STD_ON)
        else if (IPV4_ADDR_IS_MULTICAST(IpHeaderPtr->DestAddr)) /* multicast */
        {
          boolean McCtrlAddrIdxFound = FALSE;
          uint8   McCtrlAddrIdx      = IpV4_Ip_VCfgGetMulticastAddrCount();

          while (0 < McCtrlAddrIdx)
          {
            McCtrlAddrIdx--;

            if (IpHeaderPtr->DestAddr == IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs)[McCtrlAddrIdx])
            {
              McCtrlAddrIdxFound = TRUE;
              break;
            }
          }

          if (TRUE == McCtrlAddrIdxFound)
          {
            /* accept packet and map to specific multicast address id */
            AddrId      = IPV4_CTRL_VAR(IpV4_Ip_AddrConfig)[IPV4_CTRL_MULTICAST_ADDR_IDX_2_ADDR_IDX(McCtrlAddrIdx)].IpAddrId;
          }
          else if (IpV4_Ip_VCfgIsReceiveAllMulticastsEnabled())
          {
            /* accept packet and map to AutoSelectBroadcast address id */
            AddrId      = IPV4_CTRL_VAR(IpV4_Ip_BcAddrId);
            IPV4_CTRL_VAR(IpV4_Ip_LastBcAddr) = IpHeaderPtr->DestAddr;
          }
          else
          {
            /* drop packet */
            AddrId      = IPV4_ADDR_ID_INV;
          }
        }
#endif
        else
        {
          /* Destination of the packet is not this node.
           * Packet will be dropped.
           */
          AddrId      = IPV4_ADDR_ID_INV;
        }

        if(IPV4_ADDR_ID_INV != AddrId)
        {
          uint16 FlagsOffsetHbo = IPV4_NTOHS(IpHeaderPtr->Flags_Offset);
          uint16 FragmentOffset = (uint16)(FlagsOffsetHbo & IPV4_IP_FRAGMENT_OFFSET_MASK);
          /* defragment message if necessary and forward to next layer or protocol */

          LenByte = IpTotalLenByte - IpHdrLenByte;

          if (0 == LenByte)
          {
            /* ip datagram does not contain any payload. -> drop message */
            return;
          }

          /* ignore IP header options if available: */
          IpPayloadPtr = &DataPtr[IpHdrLenByte]; /* PRQA S 0310 */ /* MD_IpV4_11.04 */

#if (IPV4_IP_CHECK_RX_CHECKSUM == STD_ON)
# if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
          /* check header checksum */
          if(IPV4_CTRL_VAR(TcpIp_IpV4_ChecksumOffloadingSupport).Ip == FALSE)
          {
            if(0 == IpBase_TcpIpChecksumAdd((IPV4_P2C(uint8))IpHeaderPtr, IpHdrLenByte, 0, TRUE)) /* PRQA S 0310 */ /* MD_IpV4_11.04 */
            {
              /* checksum is ok */
            }
            else
            {
              /* checksum is invalid, so drop the message */
              return;
            }
          }
          else
          {
            /* IP checksum was already checked by HW */
          }
# endif /* TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED */
#endif /* IP_CHECK_RX_CHECKSUM */

          if ((0 != FragmentOffset) || (0 != (FlagsOffsetHbo & IPV4_IP_FLAGS_MORE_FRAGMENTS)))
          {
            /* this packet is a fragment */

#if (IPV4_IP_ENABLE_REASSEMBLY == STD_ON)
            if (IpV4_Ip_VCfgHasFragmentationConfig())
            {
              if (FALSE == IpV4_Ip_VReassemblyRxIndication(IPV4_IPCTRLIDX, IpHeaderPtr, &IpPayloadPtr, &LenByte))
              {
                IpPayloadPtr = NULL_PTR;
              }
            }
            else
#endif
            {
              IpPayloadPtr = NULL_PTR;
            }
          }

          if (NULL_PTR != IpPayloadPtr)
          {
            IpV4_Ip_VForwardPacketToUpperLayer(IPV4_IPCTRLIDX, AddrId, IpHeaderPtr, IpPayloadPtr, LenByte);
          }
        }
        else
        {
          /* incoming message not for this node */
          /* if(routing is activated) ? proceed : discard */
        }
      }
#if (IPV4_IP_CHECK_RX_CHECKSUM == STD_ON)
#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_OFF)
      else
      {
        /* header checksum error */
        /* discard message */
      }
#endif /* TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED */
#endif /* IPV4_IP_CHECK_RX_CHECKSUM */
    }
    else
    {
      /* IP version number error */
      /* discard message */
    }
  }
  else
  {
    /* IP header length error */
    /* discard message */
  }
} /* PRQA S 2006, 6010, 6030, 6060, 6080 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  IpV4_Ip_ProvideTxBuffer
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
FUNC(BufReq_ReturnType, IPV4_CODE) IpV4_Ip_ProvideTxBuffer(
    IPV4_P2V(IpV4_AddrIdType)                  addrIdPtr,
    TcpIp_SocketIdType                         socketId,
    IPV4_P2C(IpBase_SockAddrInType)            destAddrPtr,
    IPV4_P2V(uint8)                           *ipBufferPtr,
    IPV4_P2V(uint16)                           ipBufferLenPtr,
    IPV4_P2V(TcpIp_IpTxRequestDescriptorType)  ipTxReqDescrPtr,
    boolean                                    forceProvideBuffer)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType        retVal      = BUFREQ_E_NOT_OK;
  uint8                    IpCtrlIdx;                            /* PRQA S 0781 */ /* MD_IpV4_5.6 */
  IpBase_AddrInType        destAddr    = destAddrPtr->sin_addr;  /* PRQA S 0781 */ /* MD_IpV4_05.06 */
  IpBase_EthPhysAddrType   destMacAddr;
  TcpIp_ReturnType         arpRetVal;
  
  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IpV4_State == IPV4_STATE_INIT);
  TCPIP_ASSERT(IPV4_IP_COND_VALID_SOCKIDX(socketId));
  TCPIP_ASSERT(addrIdPtr      != NULL_PTR);
  TCPIP_ASSERT(IPV4_COND_VALID_OR_ANY_IPADDRID(*addrIdPtr));
  TCPIP_ASSERT(destAddrPtr    != NULL_PTR);
  TCPIP_ASSERT(ipBufferPtr    != NULL_PTR);
  TCPIP_ASSERT(ipBufferLenPtr != NULL_PTR);

  /* #10 Determine corresponding IP controller based on the IpAddrId. */
  IpCtrlIdx = IpV4_Ip_VGetIpAddrIdAndCtrlIdx(addrIdPtr, destAddr);
  
  if (IpCtrlIdx == IPV4_CTRL_IDX_INV)
  {
    /* ERROR: No matching IP controller instance found. */
  }
  else if ((*ipBufferLenPtr) > (IpV4_Ip_VCfgGetMaxTxMsgSize() - IPV4_IP_DEF_HEADER_LENGTH_BYTE))
  {
    /* ERROR: Requested IP payload size does not fit into IP packet. */
  }
  else
  {
    uint8 ctrlAddrIdx;

    /* Update TX request descriptor. */
    ipTxReqDescrPtr->IpAddrId = (*addrIdPtr);
    ipTxReqDescrPtr->CtrlIdx  = IPV4_CTRL_IDX_IP_2_ETHIF(IPV4_IPCTRLIDX);
    ipTxReqDescrPtr->SockIdx  = socketId;
    ipTxReqDescrPtr->DestAddr = destAddr;

    /* #20 Determine IP address the will be used as source address in transmitted packet. */
    IPV4_GET_ADDR_IDX_FROM_ADDR_ID(ctrlAddrIdx, *addrIdPtr);

    if ((IPV4_CTRL_ADDR_IDX_ANY == ctrlAddrIdx)
#if (IPV4_IP_ENABLE_MULTICAST_RX == STD_ON)
      || IPV4_CTRL_ADDR_IDX_IS_MULTICAST(ctrlAddrIdx)
#endif
      )
    {
      /* automatically select source address for transmission */
      /* Only one address supported for IPv4 */
      (*addrIdPtr) = IPV4_CTRL_VAR(IpV4_Ip_AddrConfig)[IPV4_CTRL_ADDR_IDX_FIRST].IpAddrId;
    }

    /* #30 Check if controller is online or passive and DHCP or LINKLOCAL address assignment is configured. */
    if ((IPV4_CTRL_VAR(IpV4_Ip_CtrlState) == IPV4_IP_OFFLINE) || (
        (IPV4_CTRL_VAR(IpV4_Ip_CtrlState) == IPV4_IP_ONLINE_PASSIVE) &&
        (IpV4_Ip_VCfgIsAddrAssignmentEnabled(IPV4_IP_ADDR_ASSIGNMENT_METHOD_DHCP)      == FALSE) &&
        (IpV4_Ip_VCfgIsAddrAssignmentEnabled(IPV4_IP_ADDR_ASSIGNMENT_METHOD_LINKLOCAL) == FALSE) ))
    {
      /* ERROR: IP controller is offline or passive and neither DHCP not LINKLOCAL address assignment is active. */
    }
    /* Provide buffer only if destination address is not the address of this node. */
    else if (destAddr == IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr)
    {
      /* ERROR: It is not allowed to send packets to ourself. */
    }
    else
    {
      /* #40 Determine physical address of next hop. */
      if (IPV4_CHK_FLAG_U8(IpV4_Ip_TransferBlock[socketId].Flags, IPV4_IP_TRANSFER_BLOCK_FLAG_PHYS_DEST_ADDR))
      {
        /* Physical destination address for the socket has been set manually. Use this address and skip ARP. */
        destMacAddr[0] = IpV4_Ip_TransferBlock[socketId].PhysDestAddr[0];
        destMacAddr[1] = IpV4_Ip_TransferBlock[socketId].PhysDestAddr[1];
        destMacAddr[2] = IpV4_Ip_TransferBlock[socketId].PhysDestAddr[2];
        destMacAddr[3] = IpV4_Ip_TransferBlock[socketId].PhysDestAddr[3];
        destMacAddr[4] = IpV4_Ip_TransferBlock[socketId].PhysDestAddr[4];
        destMacAddr[5] = IpV4_Ip_TransferBlock[socketId].PhysDestAddr[5];

        arpRetVal = TCPIP_OK;
      }
      else
      {
        arpRetVal = IpV4_Ip_VGetNextHopPhysAddr(IpCtrlIdx, destAddr, socketId, &destMacAddr[0]);
      }

      if ((arpRetVal == TCPIP_OK) || (forceProvideBuffer == TRUE))
      {
        /* #50 Provide TX buffer(s). */
        retVal = IpV4_Ip_VFragmentProvideTxBuffers(IpCtrlIdx, socketId, *ipBufferLenPtr, ipTxReqDescrPtr);

        if (retVal == BUFREQ_OK)
        {
          (*ipBufferPtr)    =         &ipTxReqDescrPtr->EthPayloadPtr[IPV4_IP_DEF_HEADER_LENGTH_BYTE];
          (*ipBufferLenPtr) = (uint16)(ipTxReqDescrPtr->EthPayloadLen - IPV4_IP_DEF_HEADER_LENGTH_BYTE);
        }
      }
      else
      {
        /* Evaluate the ARP's return code in case it is not TCPIP_OK */
        if (arpRetVal == TCPIP_E_PHYS_ADDR_MISS)
        {
          /* Physical address could not be received from ARP module. Either address is not yet stored in
              ARP table (then an ARP request was triggered), or ARP is not able to allocate a Tx buffer.
              -> Try again later */
          retVal = BUFREQ_E_BUSY;
        }
        else
        {
          /* ARP's return code is not TCPIP_OK and not TCPIP_E, i.e. it is TCPIP_E_NOT_OK or invalid:
              Address resolution could not be started or the resolution is not desired ("ARP table full" mode is active) */
          retVal = BUFREQ_E_NOT_OK;
        }
      }
    }
  }

  return retVal;
} /* PRQA S 2006, 6010, 6030, 6060, 6080 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  IpV4_Ip_VCtrlIsOnline
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
IPV4_LOCAL_INLINE FUNC(boolean, IPV4_CODE) IpV4_Ip_VCtrlIsOnline(uint8 ipCtrlIdx)
{
  IPV4_UNUSED_PARAMETER(ipCtrlIdx);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  return (boolean)((IPV4_CTRL_VAR_X(IpV4_Ip_PhysLinkActive,             ipCtrlIdx) == TRUE) &&
                   (IPV4_CTRL_VAR_X(IpV4_Ip_CtrlState,                  ipCtrlIdx) == IPV4_IP_ONLINE_ACTIVE) &&
                   (IPV4_CTRL_VAR_X(IpV4_Ip_ActiveAddrAssignmentMethod, ipCtrlIdx) != IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE));
}

/***********************************************************************************************************************
 *  IpV4_Ip_VGetNextHopPhysAddr
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
IPV4_LOCAL_INLINE FUNC(TcpIp_ReturnType, IPV4_CODE) IpV4_Ip_VGetNextHopPhysAddr(
  uint8                 IpCtrlIdx,
  IpBase_AddrInType     destAddr,
  TcpIp_SocketIdType    socketId,
  IPV4_P2V(uint8)       nextHopPhysAddrPtr)
{
  TcpIp_ReturnType retVal;

  if ((IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetMask & IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr) == (IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetMask & destAddr))
  {
    /* address is inside the local subnet */
    retVal = E_OK;
  }
  else if (destAddr == TCPIP_INADDR_BROADCAST)
  {
    /* address is a global broadcast */
    retVal = E_OK;
  }
  else if (IPV4_ADDR_IS_MULTICAST(destAddr))
  {
    /* address is a multicast */
    retVal = E_OK;
  }
  else if (IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).DefGwAddr != IPV4_ADDR_ANY)
  {
    /* use default gateway. */
    destAddr = IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).DefGwAddr;
    retVal = E_OK;
  }
  else
  {
    /* Destination is not on this link and no default router known -> No next hop existing. */
    retVal = E_NOT_OK;
  }

  if (retVal == E_OK)
  {
    retVal = IpV4_Arp_GetPhysicalAddress(IpCtrlIdx, socketId, destAddr, nextHopPhysAddrPtr, TRUE);
  }

  return retVal;
}

/***********************************************************************************************************************
 *  IpV4_Ip_VGetIpAddrIdAndCtrlIdx
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
IPV4_LOCAL_INLINE FUNC(uint8, IPV4_CODE) IpV4_Ip_VGetIpAddrIdAndCtrlIdx(
  IPV4_P2V(IpV4_AddrIdType)                 addrIdPtr,
  IpBase_AddrInType                         destAddr)
{
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8 IpCtrlIdx;  /* PRQA S 0781 */ /* MD_IpV4_5.6 */

  if (*addrIdPtr != IPV4_ADDR_ID_ANY)
  {
    /* #10 Return index of the IP controller that belongs to *addrIdPtr, if *addrPtr != IPV4_ADDR_ID_ANY. */
    IPV4_GET_CTRL_IDX_FROM_ADDR_ID(IPV4_IPCTRLIDX, *addrIdPtr);
  }
  else
  {
    /* #20 Otherwise return index and auto-select/broadcast address ID of the IP controller that has a unicast address which
     *     is in the same subnet as destAddr. */
    uint8 ipCtrlIdxCandidate = 0;

    while (ipCtrlIdxCandidate < IpV4_VCfgGetMaxNumCtrls())
    {
      if (IpV4_Ip_VCtrlIsOnline(ipCtrlIdxCandidate) == TRUE)
      {
        if ((destAddr & IPV4_CTRL_VAR_X(IpV4_Ip_ActiveAddr, ipCtrlIdxCandidate).NetMask) == (IPV4_CTRL_VAR_X(IpV4_Ip_ActiveAddr, ipCtrlIdxCandidate).NetAddr & IPV4_CTRL_VAR_X(IpV4_Ip_ActiveAddr, ipCtrlIdxCandidate).NetMask))
        {
          /* address is part of this subnet */
          break;
        }
      }

      ipCtrlIdxCandidate++;
    }

    if (ipCtrlIdxCandidate < IpV4_VCfgGetMaxNumCtrls())
    {
      /* use selected controller */
      IpCtrlIdx = ipCtrlIdxCandidate;
      /* set AddrId to AutoSelect_Broadcast address of the selected controller */
      (*addrIdPtr) = IPV4_CTRL_VAR(IpV4_Ip_BcAddrId);
    }
# if (IpV4_Ip_VCfgGetDefaultCtrlIdx() < IpV4_VCfgGetMaxNumCtrls())
    /* #30 Otherwise return index and auto-select/broadcast address ID of the default controller, if configured. */
    else if (IpV4_Ip_VCtrlIsOnline(IpV4_Ip_VCfgGetDefaultCtrlIdx()) == TRUE)
    {
      /* use default controller for all messages not directed to nodes in own subnet(s) */
      IpCtrlIdx = IpV4_Ip_VCfgGetDefaultCtrlIdx();
      /* set AddrId to AutoSelect_Broadcast address of the selected controller */
      (*addrIdPtr) = IPV4_CTRL_VAR(IpV4_Ip_BcAddrId);
    }
# endif
    /* #40 Otherwise return negative result (IPV4_CTRL_IDX_INV). */
    else
    {
      /* no suitable controller in active state found */
      IpCtrlIdx = IPV4_CTRL_IDX_INV;
    }
  }
#else
  TCPIP_UNUSED_PARAMETER(destAddr);  /* PRQA S 3112 */ /* MD_MSR_14.2 */

  if (*addrIdPtr == IPV4_ADDR_ID_ANY)
  {
    *addrIdPtr = IPV4_CTRL_VAR(IpV4_Ip_AddrConfig)[IPV4_CTRL_ADDR_IDX_FIRST].IpAddrId;
  }
#endif

  return IPV4_IPCTRLIDX;
}

/***********************************************************************************************************************
 *  IpV4_Ip_VFragmentProvideTxBuffers
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
 */
IPV4_LOCAL_INLINE FUNC(BufReq_ReturnType, IPV4_CODE) IpV4_Ip_VFragmentProvideTxBuffers(
  uint8                                     IpCtrlIdx,
  TcpIp_SockIdxType                         socketId,
  uint16                                    ipPayloadLen,
  IPV4_P2V(TcpIp_IpTxRequestDescriptorType) ipTxReqDescrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal        = BUFREQ_E_NOT_OK;
  uint8             prevEthBufIdx = IPV4_IP_TX_BUF_IDX_INV;

  /* ----- Implementation ----------------------------------------------- */
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_OFF)
  IPV4_UNUSED_PARAMETER(IpCtrlIdx);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  /* #10 Loop: Request as many buffers from the lower-layer (EthIf) as required in order to store the complete IP payload. */
  /* If the IP payload exceeds the size of one EthIf buffer, multiple buffers will be requested and a fragmented IP packet will be sent. */
  while (ipPayloadLen > 0)
  {
    IPV4_P2V(Eth_DataType) ethBufPtr;
    uint8                  ethBufIdx;
    uint16                 ethBufLen;
    uint16                 fragPayloadLen = (uint16)(IpV4_Ip_VCfgGetMtuTx() - IPV4_IP_DEF_HEADER_LENGTH_BYTE);

    /* #20 Calculate length of IP fragment and remaining payload length. */
    if (fragPayloadLen < ipPayloadLen)
    {
      /* IP payload does not fit into one IP packet.
        * -> IP packet has to be sent in multiple fragments.
        */

      /* Trim length of fragment payload to be multiple of 8 octets */
      fragPayloadLen -= (fragPayloadLen % 8);
      ipPayloadLen   -=  fragPayloadLen;
    }
    else
    {
      /* Complete IP payload fits into this fragment. */
      fragPayloadLen      = ipPayloadLen;
      ipPayloadLen = 0;
    }

    /* #30 Request buffer from lower-layer (EthIf). */
    ethBufLen = (fragPayloadLen + IPV4_IP_DEF_HEADER_LENGTH_BYTE);

    retVal = TCPIP_LL_ProvideTxBuffer(IPV4_CTRL_IDX_IP_2_ETHIF(IpCtrlIdx), IPV4_IP_FRAME_TYPE, IPV4_GET_SOCKET_ETHIF_FRAME_PRIO(socketId), &ethBufIdx, &ethBufPtr, &ethBufLen);

    if (BUFREQ_OK == retVal)
    {
      /* #40 Store buffer index in TX descriptor or append buffer to previously requested buffer, if this is not the first fragment.  */
      if (prevEthBufIdx == IPV4_IP_TX_BUF_IDX_INV)
      {
        /* This is an ethernet buffer for the first (and maybe the only) fragment. */
        ipTxReqDescrPtr->BufIdx    = ethBufIdx;
        ipTxReqDescrPtr->CurBufIdx = ethBufIdx;
      }
      else
      {
        /* This is an ethernet buffer for an additional IP fragment.
          * Link buffer of previous fragment with this fragment.
          */
        IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[prevEthBufIdx].EthBufIdxNext = ethBufIdx;
      }

      /* #50 Store data pointer and length of buffer. */
      IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[ethBufIdx].EthBufPtr     = (IPV4_P2V(uint8))ethBufPtr; /* PRQA S 0310 */ /* MD_IpV4_11.04 */
      IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[ethBufIdx].EthBufLen     = (fragPayloadLen + IPV4_IP_DEF_HEADER_LENGTH_BYTE);
      IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[ethBufIdx].EthBufIdxNext = IPV4_IP_TX_BUF_IDX_INV;

      prevEthBufIdx = ethBufIdx;
    }
    else
    {
      /* Ethernet buffer could not be provided successfully. */

      /* #60 Rollback: Release already provided buffers for all previous fragments, if a buffer request failed. */
      if (IPV4_IP_TX_BUF_IDX_INV != prevEthBufIdx)
      {
        (void)IpV4_Ip_Transmit(ipTxReqDescrPtr, 0, 0, FALSE, 0, FALSE);
      }

      retVal = BUFREQ_E_NOT_OK;
      break;
    }
  } /* while (ipPayloadLen > 0) */

  /* #70 Update information in TX descriptor, if no error occurred. */
  if (BUFREQ_OK == retVal)
  {
    ipTxReqDescrPtr->IsEthBuf      = TRUE;

    /* IpPayloadPtr points to payload of the first (and maybe the only) fragment of this IP packet */
    ipTxReqDescrPtr->EthPayloadPtr = IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[ipTxReqDescrPtr->BufIdx].EthBufPtr;
    ipTxReqDescrPtr->EthPayloadLen = IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[ipTxReqDescrPtr->BufIdx].EthBufLen;
    ipTxReqDescrPtr->IpPayloadOfs  = IPV4_IP_DEF_HEADER_LENGTH_BYTE;
  }

  return retVal;
}

/***********************************************************************************************************************
 *  IpV4_Ip_FragmentProvideTxBuffer
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, IPV4_CODE) IpV4_Ip_FragmentProvideTxBuffer(
  IPV4_P2V(TcpIp_IpTxRequestDescriptorType)  ipTxReqDescrPtr,
  IPV4_P2V(uint8)                           *bufPtrPtr,
  IPV4_P2V(uint16)                           bufLenPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8          IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV4_5.6 */
#endif

  TCPIP_ASSERT(ipTxReqDescrPtr != NULL_PTR);
  TCPIP_ASSERT(bufPtrPtr       != NULL_PTR);
  TCPIP_ASSERT(bufLenPtr       != NULL_PTR);

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  IPV4_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx, ipTxReqDescrPtr->IpAddrId);
#endif

  if (IPV4_IP_TX_BUF_IDX_INV != ipTxReqDescrPtr->CurBufIdx)
  {
    uint8 NextBufIdx = IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[ipTxReqDescrPtr->CurBufIdx].EthBufIdxNext;

    if (IPV4_IP_TX_BUF_IDX_INV != NextBufIdx)
    {
      (*bufPtrPtr) =         &IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[NextBufIdx].EthBufPtr[IPV4_IP_DEF_HEADER_LENGTH_BYTE];
      (*bufLenPtr) = (uint16)(IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[NextBufIdx].EthBufLen - IPV4_IP_DEF_HEADER_LENGTH_BYTE);
      retVal = E_OK;
    }

    ipTxReqDescrPtr->CurBufIdx = NextBufIdx;
  }

  return retVal;
} /* IpV4_Ip_ProvideNextTxBuffer */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  IpV4_Ip_VStoreProtocolChecksum
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Ip_VStoreProtocolChecksum(
  uint8           protocol,
  uint16          protocolChecksum,
  IPV4_P2V(uint8) protocolHeaderPtr)
{
  switch (protocol)
  {
#if (IPV4_SUPPORT_TCP == STD_ON)
  case TCPIP_PROTOCOL_TCP:
    TcpIp_Tcp_StoreChecksumInHeader(protocolChecksum, protocolHeaderPtr);
    break;
#endif
  case TCPIP_PROTOCOL_UDP:
    TcpIp_Udp_StoreChecksumInHeader(protocolChecksum, protocolHeaderPtr);
    break;

#if (IPV4_SUPPORT_ICMP == STD_ON)
  case IPV4_IP_PROTOCOL_ICMP:
    IpV4_Icmp_StoreChecksumInHeader(protocolChecksum, protocolHeaderPtr);
    break;
#endif

  default:
    {
      /* unsupported protocol.
        * -> do nothing
        */
    }
  }  /* switch(Protocol) */
}

/***********************************************************************************************************************
 *  IpV4_Ip_VTransmitFragments
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
 */
IPV4_LOCAL FUNC(Std_ReturnType, IPV4_CODE) IpV4_Ip_VTransmitFragments(
    uint8                                     IpCtrlIdx,
    IPV4_P2C(TcpIp_IpTxRequestDescriptorType) ipTxReqDescrPtr,
    uint8                                     ipProtocol,
    uint16                                    ipPayloadLen,
    IPV4_P2V(uint8)                           destPhysAddrPtr,
    boolean                                   calculateChecksum,
    boolean                                   reqTxConfirmation,
    uint8                                     ulReqIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType    retVal             = E_OK;
  uint16            ipPayloadLenRemain = ipPayloadLen;
  uint8             ethBufIdx          = ipTxReqDescrPtr->BufIdx;
  uint16            nextFragBlockOfs   = 0;
  uint32            protocolChecksum   = 0;
  IpBase_AddrInType srcAddr            = (IPV4_GET_SOCKET_FORCE_ZERO_SRC_ADDR(ipTxReqDescrPtr->SockIdx)) ? 0 : IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr;

  /* ----- Implementation ----------------------------------------------- */
  IPV4_UNUSED_PARAMETER(IpCtrlIdx);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* for each provided ethernet buffer */
  while (ethBufIdx != IPV4_IP_TX_BUF_IDX_INV)
  {
    /* Store next ethernet buffer index before releasing current buffer because next pointer may be overwritten directly after releasing the buffer. */
    uint8 ethBufIdxNext = IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[ethBufIdx].EthBufIdxNext;

    if (ipPayloadLenRemain > 0)
    {
      IPV4_P2V(uint8)           ethPayloadPtr  = IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[ethBufIdx].EthBufPtr; /* PRQA S 0781 */ /* MD_IpV4_05.06 */
      uint16                    ethPayloadLen  = IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[ethBufIdx].EthBufLen; /* PRQA S 0781 */ /* MD_IpV4_05.06 */
      uint16                    fragBlockOfs   = nextFragBlockOfs;
      uint16                    fragPayloadLen = (uint16)(ethPayloadLen - IPV4_IP_DEF_HEADER_LENGTH_BYTE);
      boolean                   fragIsLast;
      boolean                   fragIsFirst    = (boolean)(fragBlockOfs == 0);

      if (ipPayloadLenRemain > fragPayloadLen)
      {
        /* remaining data does not fit into one IP packet */
        fragIsLast = FALSE;

        TCPIP_ASSERT(0 == (fragPayloadLen % 8)); /* All fragment buffers except for the last should have a length that is a multiple of 8 octets. */

        nextFragBlockOfs   += (fragPayloadLen / 8);
        ipPayloadLenRemain -=  fragPayloadLen;
      }
      else
      {
        /* this is the last fragment */
        fragIsLast = TRUE;

        fragPayloadLen = ipPayloadLenRemain;
        ipPayloadLenRemain  = 0;
      }

      if (calculateChecksum == TRUE)
      {
        /* Pass 1: Calculate checksum of fragment */
        IPV4_P2V(uint8) fragPayloadPtr = &ethPayloadPtr[IPV4_IP_DEF_HEADER_LENGTH_BYTE];

        /* call the upper layer checksum calculation function */
        if ((fragIsFirst == TRUE) && ((ipProtocol == TCPIP_PROTOCOL_TCP) || (ipProtocol == TCPIP_PROTOCOL_UDP)))
        {
          TcpIp_SockAddrInetType  LocSockAddr;
          TcpIp_SockAddrInetType  RemSockAddr;

          LocSockAddr.domain  = IPBASE_AF_INET;
          LocSockAddr.addr[0] = srcAddr;
          RemSockAddr.domain  = IPBASE_AF_INET;
          RemSockAddr.addr[0] = ipTxReqDescrPtr->DestAddr;

          /* Calculate pseudo header checksum */
          protocolChecksum = TcpIp_VCalcPseudoHdrAndChecksum((const IpBase_SockAddrType*)&RemSockAddr, (const IpBase_SockAddrType*)&LocSockAddr, NULL_PTR, ipPayloadLen, ipProtocol);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
        }

        if (fragIsLast == TRUE)
        {
          IPV4_P2V(uint8) protocolHdrPtr = &IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[ipTxReqDescrPtr->BufIdx].EthBufPtr[IPV4_IP_DEF_HEADER_LENGTH_BYTE];

          /* Calculate payload checksum of last fragment and store finalized value in upper layer header */
          protocolChecksum = IpBase_TcpIpChecksumAdd(fragPayloadPtr, fragPayloadLen, protocolChecksum, TRUE);

          IpV4_Ip_VStoreProtocolChecksum(ipProtocol, (uint16)protocolChecksum, protocolHdrPtr);
        }
        else
        {
          /* Calculate payload checksum of first or intermediate fragment */
          protocolChecksum = IpBase_TcpIpChecksumAdd(fragPayloadPtr, fragPayloadLen, protocolChecksum, FALSE);
        }
      }
      else
      {
        /* Pass 2: Trigger transmission fragment */
        uint16                    flagsOffsetHbo;
        uint16                    FragTotalLen   = fragPayloadLen + IPV4_IP_DEF_HEADER_LENGTH_BYTE;
        IPV4_P2V(IpV4_Ip_HdrType) ipHdrPtr       = (IPV4_P2V(IpV4_Ip_HdrType))ethPayloadPtr; /* PRQA S 0310 */ /* MD_IpV4_11.04 */

        /* Update IP header */
        ipHdrPtr->Ver_Ihl                        = IPV4_HDR_FIELD_CREATE_VER_IHL(IPV4_IP_DEF_HEADER_LENGTH_BYTE);
        ipHdrPtr->Tos                            = IPV4_GET_SOCKET_TYPE_OF_SERVICE(ipTxReqDescrPtr->SockIdx);
        ipHdrPtr->TotLen                         = IPV4_HTONS(FragTotalLen);
        ipHdrPtr->Ident                          = IPV4_HTONS(IpV4_Ip_Identification);

        flagsOffsetHbo = (uint16)(IPV4_IP_FLAGS_DONT_FRAGMENT | fragBlockOfs);

        if (fragIsLast == FALSE)
        {
          flagsOffsetHbo |= IPV4_IP_FLAGS_MORE_FRAGMENTS;
        }

        ipHdrPtr->Flags_Offset = IPV4_HTONS(flagsOffsetHbo);
        ipHdrPtr->Ttl          = IPV4_GET_SOCKET_TIME_TO_LIVE(ipTxReqDescrPtr->SockIdx);
        ipHdrPtr->Prot         = ipProtocol;
        ipHdrPtr->SrcAddr      = srcAddr;
        ipHdrPtr->DestAddr     = ipTxReqDescrPtr->DestAddr;
        ipHdrPtr->Checksum     = 0; /* Zero used for calculation. Correct checksum will be set later. */

        /* Calculate IP header checksum and insert into header. */
  #if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
        if (IPV4_CTRL_VAR(TcpIp_IpV4_ChecksumOffloadingSupport).Ip == TRUE)
        {
          /* Checksum already set to zero. Checksum will be calculated by lower layer. */
        }
        else
  #endif  /* TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED */
        {
          ipHdrPtr->Checksum = (uint16)IpBase_TcpIpChecksumAdd(ethPayloadPtr, IPV4_IP_DEF_HEADER_LENGTH_BYTE, 0, TRUE); /* PRQA S 0310 */ /* MD_IpV4_11.04 */
        }

        /* Store information that may be required for TxConfirmation */
        IpV4_Ip_SocketDesc[ipTxReqDescrPtr->SockIdx].Prot = ipProtocol;
        IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[ethBufIdx].TxSockIdx  = ipTxReqDescrPtr->SockIdx;
        IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[ethBufIdx].UlTxReqIdx = ulReqIdx;

        /* call TCPIP_LL_Transmit to transmit frame ----------- */
        if(E_OK == TCPIP_LL_Transmit(IPV4_CTRL_IDX_IP_2_ETHIF(IpCtrlIdx), ethBufIdx, IPV4_IP_FRAME_TYPE, reqTxConfirmation, FragTotalLen, destPhysAddrPtr))
        {
          retVal = E_OK;
        }
        else
        {
          retVal = E_NOT_OK;
          /* Transmission of packet failed. */
        }
      }
    }
    else if ((calculateChecksum == FALSE) || (ipPayloadLen == 0))
    {
      /* No more data to send. */
      /* Cancel transmission of this fragment */
      /* DestMacAddr will be ignored in the called function if the length to be transmitted is 0. */
      (void)TCPIP_LL_Transmit(IPV4_CTRL_IDX_IP_2_ETHIF(IpCtrlIdx), ethBufIdx, IPV4_IP_FRAME_TYPE, FALSE, 0, destPhysAddrPtr);
    }
    else
    {
      /* This is pass 1/2 (checksum calculation) and transmit shall not be aborted (LenByte > 0).
        * Transmit will be done in pass 2/2 (transmit). 
        */
    }

    if (ethBufIdxNext != IPV4_IP_TX_BUF_IDX_INV)
    {
      /* Continue with next fragment. */
      ethBufIdx = ethBufIdxNext;
    }
    else if ((retVal == E_OK) && (calculateChecksum == TRUE) && (ipPayloadLen > 0))
    {
      /* Checksum calculation for all fragments complete.
        * ->Restart loop send packets.
        */
      ethBufIdx          = ipTxReqDescrPtr->BufIdx;
      calculateChecksum  =  FALSE;
      ipPayloadLenRemain = ipPayloadLen;
      nextFragBlockOfs   = 0;
    }
    else
    {
      /* All done.
        * -> End the loop
        */
      ethBufIdx = IPV4_IP_TX_BUF_IDX_INV;
    }
  } /* while */

  return retVal;
}  /* PRQA S 6060, 6030 */ /* MD_MSR_STPAR, MD_MSR_STCYC */

/***********************************************************************************************************************
 *  IpV4_Ip_Transmit
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(TcpIp_ReturnType, IPV4_CODE) IpV4_Ip_Transmit(
    IPV4_P2C(TcpIp_IpTxRequestDescriptorType) ipTxReqDescrPtr, /* PRQA S 3673 */ /* MD_IPV4_16.7_3673 */
    uint16                                    ipPayloadLen,
    uint8                                     ipProtocol,
    boolean                                   reqTxConfirmation,
    uint8                                     ulTxReqIdx,
    boolean                                   retryOnPhysAddrMiss)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ReturnType       retVal = TCPIP_OK;
  IpBase_EthPhysAddrType destMacAddr =
  {
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U
  }; /* dummy value for call of TCPIP_LL_Transmit in case of 'cancel transmit' */
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8              IpCtrlIdx;     /* PRQA S 0781 */ /* MD_IpV4_05.06 */
#endif
  /* IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON */

  /* ----- Implementation ----------------------------------------------- */

  TCPIP_ASSERT(IpV4_State == IPV4_STATE_INIT);
  TCPIP_ASSERT(ipTxReqDescrPtr != NULL_PTR);
  TCPIP_ASSERT(IPV4_IP_COND_VALID_SOCKIDX(ipTxReqDescrPtr->SockIdx));

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  /* use controller that was chosen during "Ip_ProvideTxBuffer": */
  IpCtrlIdx = IPV4_ADDR_ID_2_CTRL_IDX(ipTxReqDescrPtr->IpAddrId);
#endif

  /* #10 Determine next hop link layer address. */
  if (ipPayloadLen == 0)
  {
    /* Transmit will be aborted. Next Hop determination not required. */
  }
  else if (IPV4_CHK_FLAG_U8(IpV4_Ip_TransferBlock[ipTxReqDescrPtr->SockIdx].Flags, IPV4_IP_TRANSFER_BLOCK_FLAG_PHYS_DEST_ADDR))
  {
    /* Physical destination address for the socket has been set manually. Use this address and skip ARP. */
    destMacAddr[0] = IpV4_Ip_TransferBlock[ipTxReqDescrPtr->SockIdx].PhysDestAddr[0];
    destMacAddr[1] = IpV4_Ip_TransferBlock[ipTxReqDescrPtr->SockIdx].PhysDestAddr[1];
    destMacAddr[2] = IpV4_Ip_TransferBlock[ipTxReqDescrPtr->SockIdx].PhysDestAddr[2];
    destMacAddr[3] = IpV4_Ip_TransferBlock[ipTxReqDescrPtr->SockIdx].PhysDestAddr[3];
    destMacAddr[4] = IpV4_Ip_TransferBlock[ipTxReqDescrPtr->SockIdx].PhysDestAddr[4];
    destMacAddr[5] = IpV4_Ip_TransferBlock[ipTxReqDescrPtr->SockIdx].PhysDestAddr[5];
  }
  else
  {
    retVal = IpV4_Ip_VGetNextHopPhysAddr(IPV4_IPCTRLIDX, ipTxReqDescrPtr->DestAddr, ipTxReqDescrPtr->SockIdx, &destMacAddr[0]);

    if (retVal == TCPIP_E_PHYS_ADDR_MISS)
    {
      /* Setting length 0 here causes release of all ethernet buffers for this packet below. */
      ipPayloadLen = 0;
    }
  }

  if ((retVal == TCPIP_OK) || (retryOnPhysAddrMiss == FALSE))
  {
    boolean calculateProtocolChecksum;

    IpV4_Ip_Identification++;  /* increment IP identification number */

    /* #20 Determine if upper layer protocol checksum needs to be calculated in software. */
#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
    if (ipPayloadLen > (IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[ipTxReqDescrPtr->BufIdx].EthBufLen - IPV4_IP_DEF_HEADER_LENGTH_BYTE))
    {
      /* this packet needs fragmentation.
       * Checksum calculation of IP protocols by hardware not supported for fragmented packets.
       */
      calculateProtocolChecksum = TRUE;
    }
    else
    {
      /* Check if the checksum of the upper layer (TCP, UDP or ICMPv4) can be calculated by the lower layer (HW) or has to be calculated by the upper layer itself. */
      switch(ipProtocol)
      {
# if (IPV4_SUPPORT_TCP == STD_ON)
        case TCPIP_PROTOCOL_TCP:
          calculateProtocolChecksum = (boolean)(IPV4_CTRL_VAR(TcpIp_IpV4_ChecksumOffloadingSupport).Tcp == FALSE);
          break;
# endif
        case TCPIP_PROTOCOL_UDP:
          calculateProtocolChecksum = (boolean)(IPV4_CTRL_VAR(TcpIp_IpV4_ChecksumOffloadingSupport).Udp == FALSE);
          break;

        case IPV4_IP_PROTOCOL_ICMP:
          calculateProtocolChecksum = (boolean)(IPV4_CTRL_VAR(TcpIp_IpV4_ChecksumOffloadingSupport).Icmp == FALSE);
          break;

        default:
          calculateProtocolChecksum = TRUE;
          break;

      }  /* switch(Protocol) */
    }
#else
    calculateProtocolChecksum = TRUE;
#endif

    /* #30 Trigger transmission of all fragments. */
    retVal = IpV4_Ip_VTransmitFragments(IPV4_IPCTRLIDX, ipTxReqDescrPtr, ipProtocol, ipPayloadLen, &destMacAddr[0], calculateProtocolChecksum, reqTxConfirmation, ulTxReqIdx);
  }
  /* else: Skip sending the packet. */

  return retVal;
} /* PRQA S 2006, 6060, 6010, 6030, 6050, 6080 */ /* MD_MSR_14.7, MD_MSR_STPAR, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  IpV4_Ip_GetPhysAddr
 **********************************************************************************************************************/
/*! \brief        Obtains the physical source address used by the EthIf controller implicitly specified via IpAddrId.
 *  \param[in]    IpAddrId           Local address identifier implicitly specifying the EthIf controller for which the
 *                                   physical address shall be obtained.
 *  \param[out]   PhysAddrPtr        Pointer to the memory where the physical source address (MAC address) in network
 *                                   byte order is stored
 *  \context      task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) IpV4_Ip_GetPhysAddr(
    IpV4_AddrIdType IpAddrId,
    IPV4_P2V(uint8) PhysAddrPtr )
{
  uint8 IpCtrlIdx;    /* PRQA S 0781 */ /* MD_IpV4_5.6 */
  uint8 EthIfCtrlIdx; /* PRQA S 0781 */ /* MD_IpV4_5.6 */

  TCPIP_ASSERT(IPV4_COND_VALID_IPADDRID(IpAddrId));

  /* Get the IpControllerIndex */
  IPV4_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx, IpAddrId);

  /* Get the EthIf controller index */
  EthIfCtrlIdx = IpV4_CtrlIdxIp2EthIf[IpCtrlIdx];

  /* Get the physical address */
  TCPIP_LL_GetPhysAddr(EthIfCtrlIdx, PhysAddrPtr);
}

/***********************************************************************************************************************
 *  IpV4_Ip_GetRemotePhysAddr
 **********************************************************************************************************************/
/*! \brief        Lookup the physical address for a remote network address. 
 *  \description  TcpIp_GetRemotePhysAddr queries the IP/physical address translation table specified by CtrlIdx and
 *                returns the physical address related to the IP address specified by IpAddrPtr. In case no physical
 *                address can be retrieved and parameter initRes is TRUE, address resolution for the specified IP
 *                address is initiated on the local network.
 *  \param[in]    CtrlIdx             EthIf controller index to identify the related ARP/NDP table.
 *  \param[in]    IpAddr              specifies the IP address for which the physical address shall be retrieved
 *  \param[in]    InitRes             specifies if the address resolution shall be initiated (TRUE) or not (FALSE) in
 *                                    case the physical address related to the specified IP address is currently
 *                                    unknown.
 *  \param[out]   PhysAddrPtr         Pointer to the memory where the physical address (MAC address) related to the
 *                                    specified IP address is stored in network byte order.
 *  \return       TCPIP_E_OK               specified IP address resolved, physical address provided via PhysAddrPtr 
 *                TCPIP_E_NOT_OK           physical address currently unknown, not even resolution could be started!
 *                TCPIP_E_PHYS_ADDR_MISS   physical address currently unknown (address resolution initiated if initRes set to TRUE)
 *  \context      task level
 *  \trace        CREQ_115906
 **********************************************************************************************************************/
FUNC(TcpIp_ReturnType, TCPIP_CODE) IpV4_Ip_GetRemotePhysAddr(
    uint8              CtrlIdx,
    IpBase_AddrInType  IpAddr,
    IPV4_P2V(uint8)    PhysAddrPtr,
    boolean            InitRes)
{
  TcpIp_ReturnType  RetVal;
  uint8 IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV4_05.06 */

  /* get IpCtrlIdx */
  if (IpV4_CtrlIdxEthIf2Ip(CtrlIdx, &IpCtrlIdx) == E_OK)
  {
    /* Get the physical address */
    RetVal = IpV4_Arp_GetPhysicalAddress(IpCtrlIdx, IPV4_INV_SOCK_IDX, IpAddr, PhysAddrPtr, InitRes);
  }
  else
  {
    RetVal = TCPIP_E_NOT_OK;
  }

  return RetVal;
}

/***********************************************************************************************************************
 *  IpV4_GetCtrlIdx
 **********************************************************************************************************************/
/*! \brief        IpV4_GetCtrlIdx returns the index of the EthIf controller related to LocalAddrId.
 *  \param[in]    AddrId              Local IP address identifier implicitly specifing the EthIf controller that shall
 *                                    be returned
 *  \param[out]   CtrlIdxPtr          Pointer to the memory where the index of the EthIf controller related to
 *                                    LocalAddrId is stored
 *  \return       E_OK                The request was successful 
 *                E_NOT_OK            The request was not successful
 *  \context      task level
 *  \note
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) IpV4_GetCtrlIdx(
    IpV4_AddrIdType AddrId,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) CtrlIdxPtr )
{

  uint8 IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV4_5.6 */

  /* Get the IpControllerIndex */
  IPV4_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx, AddrId);

  /* Get the EthIf controller index */
  *CtrlIdxPtr = IpV4_CtrlIdxIp2EthIf[IpCtrlIdx];

  return E_OK;
}

/***********************************************************************************************************************
 *  IpV4_Ip_TxConfirmation
 **********************************************************************************************************************/
/*! \brief        Confirmation of a message transmission.
 *  \description  This callback function is called by lower layer (EthIf) if a message has been transmitted by the
 *                hardware.
 *  \param[in]    CtrlIdx               controller index
 *  \param[in]    BufIdx                buffer index
 *  \return       void
 *  \note
 *  \context      interrupt or task level
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Ip_TxConfirmation(
    uint8 CtrlIdx,
    uint8 BufIdx )
{
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8 IpCtrlIdx;           /* PRQA S 0781 */ /* MD_IpV4_05.06 */
#endif
  TcpIp_SockIdxType SockIdx; /* PRQA S 0781 */ /* MD_IpV4_05.06 */
  uint8 UlTxReqTabIdx;
  uint8 IpProtocol;

  /* ------------------------------------------------- */
  /* DET */
  IpV4_CheckDetErrorReturnVoid( IPV4_COND_INIT, IPV4_IP_API_ID_TX_CONFIRMATION, IPV4_E_NOT_INITIALIZED )
  /* ------------------------------------------------- */

  /* Map EthIfCtrlIdx to IpCtrlIdx or return if IP is not active on this EthIfCtrl. */
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  if (!IPV4_CTRL_IDX_ETHIF_2_IP(CtrlIdx, IpCtrlIdx))
#else
  if (CtrlIdx != IPV4_CTRL_IDX_IP_2_ETHIF(IpCtrlIdx))
#endif
  {
    /* IP not active on this EthIf controller */
    return;
  }

  SockIdx       = IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[BufIdx].TxSockIdx;  /* PRQA S 0781 */ /* MD_IpV4_05.06 */
  UlTxReqTabIdx = IPV4_CTRL_VAR(IpV4_Ip_Buf2TxReqMap)[BufIdx].UlTxReqIdx;
  IpProtocol    = IpV4_Ip_SocketDesc[SockIdx].Prot;

  if(IPV4_IP_PROTOCOL_UDP == IpProtocol)
  {
    TcpIp_Udp_TxConfirmation(SockIdx, UlTxReqTabIdx);
  }
#if (IPV4_SUPPORT_TCP == STD_ON)
  else if(IPV4_IP_PROTOCOL_TCP == IpProtocol)
  {
    /* do not forward TxConfirmation to TCP, TCP is not interested in this information
       -> do nothing */
  }
#endif /* IPV4_SUPPORT_TCP */
  else
  {
    /* invalid protocol:
       this should never happen -> ignore tx confirmation */
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  IpV4_Ip_Cbk_TrcvLinkStateChg
 **********************************************************************************************************************/
/*! \brief      This callback function is called by lower layer on physical link status change
 *  \param[in]  CtrlIdx               controller index
 *  \param[in]  TrcvLinkState         transceiver link state
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Ip_Cbk_TrcvLinkStateChg(
    uint8 CtrlIdx,
    EthTrcv_LinkStateType TrcvLinkState )
{
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8 IpCtrlIdx;           /* PRQA S 0781 */ /* MD_IpV4_05.06 */
#endif

  /* ------------------------------------------------- */
  /* DET */
  IpV4_CheckDetErrorReturnVoid( IPV4_COND_INIT, IPV4_IP_API_ID_TRCV_LNK_ST_CH, IPV4_E_NOT_INITIALIZED )
  /* ------------------------------------------------- */

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  if (!IPV4_CTRL_IDX_ETHIF_2_IP(CtrlIdx, IpCtrlIdx))
#else
  if (CtrlIdx != IPV4_CTRL_IDX_IP_2_ETHIF(IpCtrlIdx))
#endif
  {
    return;
  }

  if(ETHTRCV_LINK_STATE_DOWN == TrcvLinkState)
  {
    TcpIp_Cbk_VTrcvLinkStateChg(IPV4_IPCTRLIDX, IPBASE_AF_INET, FALSE); /* notify TcpIp/Dhcp */

    /* Set physical link indication to FALSE. (State of IP instance will be changed in MainFunction) */
    IPV4_CTRL_VAR(IpV4_Ip_PhysLinkActive) = FALSE;
  }
  else
  {
    TcpIp_Cbk_VTrcvLinkStateChg(IPV4_IPCTRLIDX, IPBASE_AF_INET, TRUE); /* notify TcpIp/Dhcp */

    /* Set physical link indication to TRUE. (State of IP instance will be changed in MainFunction) */
    IPV4_CTRL_VAR(IpV4_Ip_PhysLinkActive) = TRUE;

#if defined TCPIP_INV_DEM_EVENT_ID
# if (TcpIp_NumIpV4CtrlsOfDemEventTCPIP_E_DUPLICATE_DHCP_ADDR > 0)
    /* In case the link has been down, before, the address-assignment was renewed and thus related DEM event is now being reset */
    IpV4_ResetDemEventIpCtrl(TcpIp_DemEventTCPIP_E_DUPLICATE_DHCP_ADDRSnvsByIpV4Ctrl, TcpIp_NumIpV4CtrlsOfDemEventTCPIP_E_DUPLICATE_DHCP_ADDR, IpCtrlIdx);
# endif
#endif
  }
} /* IpV4_Ip_Cbk_TrcvLinkStateChg */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  IpV4_Ip_GetLocNetAddr
 **********************************************************************************************************************/
/*! \brief        Request the current IP address for a given IP address id
 *  \description  Reading the local IP address is mostly used for diagnostic purposes.
 *  \param[in]    AddrId                id of the IPv4 address
 *  \param[out]   NetAddrPtr            pointer for the local network address
 *  \return       E_OK                  local network address returned
 *  \return       E_NOT_OK              local network address access failed
 *  \note
 *  \context      task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV4_CODE) IpV4_Ip_GetLocNetAddr(
  IpV4_AddrIdType AddrId,
  P2VAR(IpBase_AddrInType, AUTOMATIC, IPV4_APPL_DATA) NetAddrPtr )
{
  Std_ReturnType RetValue = E_NOT_OK;
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8 IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV4_5.6 */
#endif
  uint8 CtrlAddrIdx;

  /* ------------------------------------------------- */
  /* DET */
  IpV4_CheckDetErrorReturnValue( IPV4_COND_INIT,                   IPV4_IP_API_ID_GET_LOC_NET_ADDR, IPV4_E_NOT_INITIALIZED, E_NOT_OK )
  IpV4_CheckDetErrorReturnValue( IPV4_COND_VALID_IPADDRID(AddrId), IPV4_IP_API_ID_GET_LOC_NET_ADDR, IPV4_E_INV_PARAM,       E_NOT_OK )
  IpV4_CheckDetErrorReturnValue( IPV4_COND_VALID_PTR(NetAddrPtr),  IPV4_IP_API_ID_GET_LOC_NET_ADDR, IPV4_E_INV_POINTER,     E_NOT_OK )
  /* ------------------------------------------------- */

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  IPV4_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);
#endif
  IPV4_GET_ADDR_IDX_FROM_ADDR_ID(CtrlAddrIdx, AddrId);

  if (IPV4_CTRL_ADDR_IDX_ANY != CtrlAddrIdx)
  {
#if (IPV4_IP_ENABLE_MULTICAST_RX == STD_ON)
    if (IPV4_CTRL_ADDR_IDX_IS_MULTICAST(CtrlAddrIdx))
    {
      /* return multicast address */
      uint8 McCtrlAddrIdx = IPV4_CTRL_ADDR_IDX_2_MULTICAST_ADDR_IDX(CtrlAddrIdx);

      if (IPV4_ADDR_ANY != IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs)[McCtrlAddrIdx])
      {
        (*NetAddrPtr) = IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs)[McCtrlAddrIdx];
        RetValue = E_OK;
      }
    }
    else
#endif
    {
      /* return unicast address */
      (*NetAddrPtr) = IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr;

      if(IPV4_ADDR_ANY != IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr)
      {
        RetValue = E_OK;
      }
    }
  }
  else
  {
    /* return destination address of last received broadcast packet */
    *NetAddrPtr = IPV4_CTRL_VAR(IpV4_Ip_LastBcAddr);
    RetValue = E_OK;
  }

  return RetValue;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  IpV4_Ip_GetDefaultRouterNetAddr
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, IPV4_CODE) IpV4_Ip_GetDefaultRouterNetAddr(
  IpV4_AddrIdType                                     ipAddrId,
  P2VAR(IpBase_AddrInType, AUTOMATIC, IPV4_APPL_DATA) defRouterNetAddrPtr)
{
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8 IpCtrlIdx;  /* PRQA S 0781 */ /* MD_IpV4_5.6 */
#endif

  TCPIP_ASSERT(IPV4_COND_VALID_IPADDRID(ipAddrId));
  TCPIP_ASSERT(defRouterNetAddrPtr != NULL_PTR);

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  IpCtrlIdx = IPV4_ADDR_ID_2_CTRL_IDX(ipAddrId); /* PRQA S 0781 */ /* MD_IpV4_5.6 */
#endif
  
  *defRouterNetAddrPtr = IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).DefGwAddr;
}

/***********************************************************************************************************************
 *  IpV4_Ip_GetLocNetMask
 **********************************************************************************************************************/
/*! \brief        Request the current net mask for a given IP address id
 *  \description  Reading the local network mask is mostly used for diagnostic purposes.
 *  \param[in]    AddrId                id of the IPv4 address
 *  \param[out]   NetMaskPtr            pointer for the local network mask
 *  \return       E_OK                  local network mask returned
 *  \return       E_NOT_OK              local network mask access failed
 *  \note
 *  \context      task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV4_CODE) IpV4_Ip_GetLocNetMask(
    IpV4_AddrIdType AddrId,
    P2VAR(IpBase_AddrInType, AUTOMATIC, IPV4_APPL_DATA) NetMaskPtr )
{
  Std_ReturnType RetValue;
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8 IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV4_5.6 */
#endif

  /* ------------------------------------------------- */
  /* DET */
  IpV4_CheckDetErrorReturnValue( IPV4_COND_INIT,                   IPV4_IP_API_ID_GET_LOC_NET_MASK, IPV4_E_NOT_INITIALIZED, E_NOT_OK )
  IpV4_CheckDetErrorReturnValue( IPV4_COND_VALID_IPADDRID(AddrId), IPV4_IP_API_ID_GET_LOC_NET_ADDR, IPV4_E_INV_PARAM, E_NOT_OK )
  IpV4_CheckDetErrorReturnValue( IPV4_COND_VALID_PTR(NetMaskPtr),  IPV4_IP_API_ID_GET_LOC_NET_MASK, IPV4_E_INV_POINTER, E_NOT_OK )
  /* ------------------------------------------------- */

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  IPV4_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx, AddrId);
#endif

  if(0 != IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetMask)
  {
    *NetMaskPtr = IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetMask;
    RetValue = E_OK;
  }
  else
  {
    /* no valid address available */
    *NetMaskPtr = 0;
    RetValue = E_NOT_OK;
  }

  return RetValue;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  IpV4_Ip_GetActiveAddrAssignmentMethod
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, IPV4_CODE) IpV4_Ip_GetActiveAddrAssignmentMethod(
  IpV4_AddrIdType ipAddrId)
{
  uint8 retVal;
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8 IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV4_5.6 */
#endif
  uint8 ctrlAddrIdx;

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  IPV4_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   ipAddrId);
#endif
  IPV4_GET_ADDR_IDX_FROM_ADDR_ID(ctrlAddrIdx, ipAddrId);

  /* #10 Determine address type (ANY, UNICAST, MULTICAST) specified by ipAddrId and return assignment method. */

  if (ctrlAddrIdx == IPV4_CTRL_ADDR_IDX_ANY)
  {
    /* Broadcast address is assigned statically. */
    retVal = IPV4_IP_ADDR_ASSIGNMENT_METHOD_STATIC;
  }
  else if (IPV4_CTRL_ADDR_IDX_IS_UNICAST(ctrlAddrIdx))
  {
    /* Unicast address assignment method is configuration and runtime dependent. */
    retVal = IPV4_CTRL_VAR(IpV4_Ip_ActiveAddrAssignmentMethod);
  }
  else
  {
#if (IPV4_IP_ENABLE_MULTICAST_RX == STD_ON)
    uint8 mcAddrIdx = IPV4_CTRL_ADDR_IDX_2_MULTICAST_ADDR_IDX(ctrlAddrIdx);

    TCPIP_ASSERT(mcAddrIdx < IpV4_Ip_VCfgGetMulticastAddrCount());

    if (IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs)[mcAddrIdx] != IPV4_ADDR_ANY)
    {
      /* MULTICAST addresses are assigned statically. */
      retVal = IPV4_IP_ADDR_ASSIGNMENT_METHOD_STATIC;
    }
    else
    {
      retVal = IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE;
    }
#else
    TCPIP_ASSERT_UNREACHABLE();
    retVal = IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE;
#endif
  }

  return retVal;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV4_Ip_GetActiveAddrAssignmentMethod() */

/**********************************************************************************************************************
 *  IpV4_Ip_IsAddrIdAcceptable()
 **********************************************************************************************************************/
/*! \brief        This function checks whether the address identifier of a received packet is acceptable for a socket
 *                bound to a specific address identifier.
 *                The check is performed according to the acceptance filter given in [SWS_TCPIP_00106].
 *  \param[in]    BindIpAddrId           ip address identifier to which the socket has been bound
 *  \param[in]    RecvIpAddrId           ip address identifier of the received packet
 *  \param[out]   MatchType              specifies the type of the match (e.g. TCPIP_IF_MATCH, TCPIP_EXACT_MATCH)
 *  \return       TRUE                   packet is acceptable
 *  \return       FALSE                  packet is not acceptable
 *  \context      interrupt or task level
 **********************************************************************************************************************/
FUNC(boolean, IPV4_CODE) IpV4_Ip_IsAddrIdAcceptable(IpV4_AddrIdType BindIpAddrId, IpV4_AddrIdType RecvIpAddrId, IPV4_P2V(uint8) MatchType)
{
  /* Det checks */
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV4_CheckDetErrorReturnValue(IPV4_COND_VALID_OR_ANY_IPADDRID(BindIpAddrId), IPV4_API_ID_IS_ADDR_ID_ACCEPTABLE, IPV4_E_INV_PARAM, FALSE);
  IpV4_CheckDetErrorReturnValue(IPV4_COND_VALID_IPADDRID(RecvIpAddrId),        IPV4_API_ID_IS_ADDR_ID_ACCEPTABLE, IPV4_E_INV_PARAM, FALSE);
  /**/

  if (IPV4_ADDR_ID_ANY == BindIpAddrId)
  {
    (*MatchType) = IPV4_ADDR_MATCH_EVER;
    return TRUE;
  }

  if (RecvIpAddrId == BindIpAddrId)
  {
    /* address matches filter address (exact match) */
    (*MatchType) = IPV4_ADDR_MATCH_EXACT;
    return TRUE;
  }

  if ((IPV4_ADDR_ID_2_ADDR_IDX(BindIpAddrId) == IPV4_CTRL_ADDR_IDX_ANY) || (IPV4_ADDR_ID_2_ADDR_IDX(RecvIpAddrId) == IPV4_CTRL_ADDR_IDX_ANY))
  {
    /* bind or received address is an AutoSelect_Multicast address */
    if (IPV4_ADDR_ID_2_CTRL_IDX(BindIpAddrId) == IPV4_ADDR_ID_2_CTRL_IDX(RecvIpAddrId))
    {
      /* both addresses are on same controller If-match */
      (*MatchType) = IPV4_ADDR_MATCH_IF;
      return TRUE;
    }
  }

  return FALSE;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV4_Ip_IsAddrIdAcceptable() */

/***********************************************************************************************************************
 *  IpV4_Ip_RequestIpAddrAssignment
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
 */
FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Ip_RequestIpAddrAssignment(
    uint8                       ipAddrId,
    TcpIp_IpAddrAssignmentType  addrAssgnmentType,
    IpBase_AddrInType           localIpAddr,
    uint8                       netmaskCidr,
    IpBase_AddrInType           defaultRouter)
{
  Std_ReturnType          retVal = E_NOT_OK;
  uint8                   IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV4_5.6 */
  uint8                   ctrlAddrIdx;

  /* #05 Map TcpIp address assignment type to IPv4 address assignment method. */
  uint8 addrAssignmentMethod = IPV4_CONVERT_TCPIP_ADDR_ASSIGNMENT_TYPE(addrAssgnmentType);

  /* #10 Check input parameters. */

  /* DET checks */
  IpV4_CheckDetErrorReturnValue(IPV4_COND_INIT, IPV4_IP_API_ID_REQ_IP_ADDR_ASSIGN, IPV4_E_NOT_INITIALIZED, E_NOT_OK)

  if(FALSE == IPV4_COND_VALID_IPADDRID(ipAddrId))
  {
    return E_NOT_OK;
  }

  IPV4_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   ipAddrId);
  IPV4_GET_ADDR_IDX_FROM_ADDR_ID(ctrlAddrIdx, ipAddrId);

  /* #20 Verify that IP instance is not in state OFFLINE. */
  if(IPV4_CTRL_VAR(IpV4_Ip_CtrlState) != IPV4_IP_OFFLINE)
  {
    /* #30 Check if address type is UNICAST or MULTICAST. */
    if (IPV4_CTRL_ADDR_IDX_IS_UNICAST(ctrlAddrIdx))
    {
      uint8 addrAssignmentPrio = IpV4_Ip_VCfgGetAddrAssignmentPrio(addrAssignmentMethod);

      if (addrAssignmentPrio != IPV4_IP_ADDR_ASSIGNMENT_PRIO_NONE)
      {
        uint8 addrAssignmentCfg  = IpV4_Ip_VCfgGetAddrAssignmentCfgByPrio(addrAssignmentPrio);

        /* #40 Do not allow address assignment if trigger is configured to AUTOMATIC. */
        if (IPV4_CHK_FLAG_U8(addrAssignmentCfg, IPV4_IP_ADDR_ASSIGNMENT_FLAG_AUTO_TRIGGER) == FALSE)
        {
          /* #50 Trigger address assignment method. */
          retVal = IpV4_Ip_VTriggerAddrAssignment(IpCtrlIdx, addrAssignmentCfg, TRUE);
          /* #60 Store requested address value if assignment method is STATIC. */
          if (addrAssignmentMethod == IPV4_IP_ADDR_ASSIGNMENT_METHOD_STATIC)
          {
            if (localIpAddr != IPV4_ADDR_ANY)
            {
              IPV4_CTRL_VAR(IpV4_Ip_StaticAddr_Requested)->NetAddr   = localIpAddr;
              IPV4_CTRL_VAR(IpV4_Ip_StaticAddr_Requested)->NetMask   = IPBASE_HTON32(0xFFFFFFFFu << (32 - netmaskCidr));
              IPV4_CTRL_VAR(IpV4_Ip_StaticAddr_Requested)->DefGwAddr = defaultRouter;

              IpV4_Ip_VIpAddrAssignmentReadyNotification(IpCtrlIdx, IPV4_IP_ADDR_ASSIGNMENT_METHOD_STATIC, TRUE);
              retVal = E_OK;
            }
            else if (IPV4_CTRL_VAR(IpV4_Ip_StaticAddr_Default) != NULL_PTR)
            {
              /* Assign configured default address. */
              *IPV4_CTRL_VAR(IpV4_Ip_StaticAddr_Requested) = *IPV4_CTRL_VAR(IpV4_Ip_StaticAddr_Default);

              IpV4_Ip_VIpAddrAssignmentReadyNotification(IpCtrlIdx, IPV4_IP_ADDR_ASSIGNMENT_METHOD_STATIC, TRUE);
              retVal = E_OK;
            }
            else
            {
              /* ERROR: No default address configured. */
              TCPIP_ASSERT_UNREACHABLE();
            }
          }
        }
      }
    }
    else
    {
      /* #70 Update MULTICAST address value. */
#if (IPV4_IP_ENABLE_MULTICAST_RX == STD_ON)
      uint8 mcAddrIdx = IPV4_CTRL_ADDR_IDX_2_MULTICAST_ADDR_IDX(ctrlAddrIdx);
      retVal = IpV4_Ip_VChangeMulticastAddrAssignment(IpCtrlIdx, mcAddrIdx, localIpAddr);
#else
      TCPIP_ASSERT_UNREACHABLE();
#endif
    }
  }

  return retVal;
} /* IpV4_Ip_RequestIpAddrAssignment() */ /* PRQA S 2006, 6010, 6030, 6060, 6080 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  IpV4_Ip_ReleaseIpAddrAssignment
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
 */
FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Ip_ReleaseIpAddrAssignment(
    uint8                      ipAddrId,
    TcpIp_IpAddrAssignmentType addrAssignmentType)
{
  Std_ReturnType          retVal = E_NOT_OK;
  uint8                   IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV4_5.6 */
  uint8                   ctrlAddrIdx;
  uint8                   addrAssignmentPrio;

  uint8 addrAssignmentMethodToRelease;

  TCPIP_ASSERT(IPV4_COND_INIT);

  /* #10 Check input parameters. */

  /* #15 Map TcpIp address assignment type to IPv4 address assignment method. */
  if(IPV4_COND_VALID_IPADDRID(ipAddrId))
  {
    if (addrAssignmentType < 0xFFu)
    {
      addrAssignmentMethodToRelease = IPV4_CONVERT_TCPIP_ADDR_ASSIGNMENT_TYPE(addrAssignmentType);
    }
    else
    {
      addrAssignmentMethodToRelease = 0xFFu;
    }
 
    IPV4_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx, ipAddrId);
    IPV4_GET_ADDR_IDX_FROM_ADDR_ID(ctrlAddrIdx, ipAddrId);

    /* #20 Handle addresses of type UNICAST. */
    if (IPV4_CTRL_ADDR_IDX_IS_UNICAST(ctrlAddrIdx))
    {
      /* #30 Iterate over all configured address assignment methods (priority is not important here). */
      for (addrAssignmentPrio = IPV4_IP_ADDR_ASSIGNMENT_PRIO_HIGHEST; addrAssignmentPrio <= IPV4_IP_ADDR_ASSIGNMENT_PRIO_LOWEST; addrAssignmentPrio++)
      {
        uint8 addrAssignmentCfg    = IpV4_Ip_VCfgGetAddrAssignmentCfgByPrio(addrAssignmentPrio);
        uint8 addrAssignmentMethod = IpV4_Ip_VCfgGetAddrAssignmentMethodByCfg(addrAssignmentCfg);

        /* #40 Only release address assignment if trigger is not configured to AUTOMATIC. */
        if ((addrAssignmentMethod != IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE) && (IPV4_CHK_FLAG_U8(addrAssignmentCfg, IPV4_IP_ADDR_ASSIGNMENT_FLAG_AUTO_TRIGGER) == FALSE))
        {
          /* #50 Release address assignment if method was explicitly specified or all assignments shall be released. */
          if ((addrAssignmentMethodToRelease == TCPIP_IPADDR_ASSIGNMENT_ANY) || (addrAssignmentMethodToRelease == addrAssignmentMethod))
          {
            (void)IpV4_Ip_VTriggerAddrAssignment(IpCtrlIdx, addrAssignmentCfg, FALSE);
            retVal = E_OK;
          }
        }
      }
    }
    else 
    {
#if (IPV4_IP_ENABLE_MULTICAST_RX == STD_ON)
      if ((addrAssignmentMethodToRelease == TCPIP_IPADDR_ASSIGNMENT_ANY) || (addrAssignmentMethodToRelease == IPV4_IP_ADDR_ASSIGNMENT_METHOD_STATIC))
      {
        /* #60 Handle addresses of type MULTICAST. */
        uint8 mcAddrIdx = IPV4_CTRL_ADDR_IDX_2_MULTICAST_ADDR_IDX(ctrlAddrIdx);
        retVal = IpV4_Ip_VChangeMulticastAddrAssignment(IpCtrlIdx, mcAddrIdx, IPV4_ADDR_ANY);
      }
#else
      TCPIP_ASSERT_UNREACHABLE();
#endif
    }
  }

  return retVal;
} /* IpV4_Ip_ReleaseIpAddrAssignment() */ /* PRQA S 6080 */ /* MD_MSR_MD_MSR_STMIF */

/***********************************************************************************************************************
 *  IpV4_Ip_ResetSocket
 **********************************************************************************************************************/
/*! \brief      Reset parameters (e.g.: TTL, ToS, Frame Priority) for a specific socket to default values
 *  \param[in]  SockIdx             socket index
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Ip_ResetSocket(TcpIp_SockIdxType SockIdx)
{
  /* ------------------------------------------------- */
  /* DET */
  IpV4_CheckDetErrorReturnVoid( IPV4_IP_COND_VALID_SOCKIDX(SockIdx), IPV4_IP_API_ID_RESET_SOCKET, IPV4_E_INV_SOCK_IDX )
  /* ------------------------------------------------- */

  IpV4_Ip_TransferBlock[SockIdx].PhysDestAddr[0] = 0;
  IpV4_Ip_TransferBlock[SockIdx].PhysDestAddr[1] = 0;
  IpV4_Ip_TransferBlock[SockIdx].PhysDestAddr[2] = 0;
  IpV4_Ip_TransferBlock[SockIdx].PhysDestAddr[3] = 0;
  IpV4_Ip_TransferBlock[SockIdx].PhysDestAddr[4] = 0;
  IpV4_Ip_TransferBlock[SockIdx].PhysDestAddr[5] = 0;
  IpV4_Ip_TransferBlock[SockIdx].EthIfFramePrio  = 0;
  IpV4_Ip_TransferBlock[SockIdx].TimeToLive      = 0;
  IpV4_Ip_TransferBlock[SockIdx].TypeOfService   = 0;
  IpV4_Ip_TransferBlock[SockIdx].Flags           = 0;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  IpV4_Ip_SetTimeToLive
 **********************************************************************************************************************/
/*! \brief      Set socket specific value of the 'Time To Live' IPv4 header field used in outgoing packets
 *  \param[in]  SockIdx             socket index
 *  \param[in]  Value               value of the 'Time to Live' field
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Ip_SetTimeToLive(TcpIp_SockIdxType SockIdx, uint8 Value)
{
  /* ------------------------------------------------- */
  /* DET */
  IpV4_CheckDetErrorReturnVoid( IPV4_COND_INIT,                      IPV4_IP_API_ID_SET_TIME_TO_LIVE, IPV4_E_NOT_INITIALIZED )
  IpV4_CheckDetErrorReturnVoid( IPV4_IP_COND_VALID_SOCKIDX(SockIdx), IPV4_IP_API_ID_SET_TIME_TO_LIVE, IPV4_E_INV_SOCK_IDX    )
  /* ------------------------------------------------- */

  IpV4_Ip_TransferBlock[SockIdx].TimeToLive = Value;
  IpV4_Ip_TransferBlock[SockIdx].Flags |= IPV4_IP_TRANSFER_BLOCK_FLAG_TIME_TO_LIVE;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  IpV4_Ip_SetTypeOfService
 **********************************************************************************************************************/
/*! \brief      Set socket specific value of the 'Type of Service' IPv4 header field used in outgoing packets
 *  \param[in]  SockIdx             socket index
 *  \param[in]  Value               value of the 'Type of Service' field
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Ip_SetTypeOfService(TcpIp_SockIdxType SockIdx, uint8 Value)
{
  /* ------------------------------------------------- */
  /* DET */
  IpV4_CheckDetErrorReturnVoid( IPV4_COND_INIT,                      IPV4_IP_API_ID_SET_TYPE_OF_SERVICE, IPV4_E_NOT_INITIALIZED )
  IpV4_CheckDetErrorReturnVoid( IPV4_IP_COND_VALID_SOCKIDX(SockIdx), IPV4_IP_API_ID_SET_TYPE_OF_SERVICE, IPV4_E_INV_SOCK_IDX    )
  /* ------------------------------------------------- */

  IpV4_Ip_TransferBlock[SockIdx].TypeOfService = Value;
  IpV4_Ip_TransferBlock[SockIdx].Flags |= IPV4_IP_TRANSFER_BLOCK_FLAG_TYPE_OF_SERVICE;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  IpV4_Ip_SetPhysDestAddr
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, IPV4_CODE) IpV4_Ip_SetPhysDestAddr(TcpIp_SockIdxType sockIdx, IPV4_P2C(uint8) physDestAddrPtr)
{
  /* ------------------------------------------------- */
  /* DET */
  IpV4_CheckDetErrorReturnVoid( IPV4_COND_INIT,                      IPV4_IP_API_ID_SET_TYPE_OF_SERVICE, IPV4_E_NOT_INITIALIZED )
  IpV4_CheckDetErrorReturnVoid( IPV4_IP_COND_VALID_SOCKIDX(sockIdx), IPV4_IP_API_ID_SET_TYPE_OF_SERVICE, IPV4_E_INV_SOCK_IDX    )
  /* ------------------------------------------------- */

  /* #10 Check if physDestAddrPtr is not a NULL_PTR. */
  if (physDestAddrPtr != NULL_PTR)
  {
    /* #20 Store provided physical address in the socket data structure and set valid flag. */
    uint8_least physAddrByteIdx;
    
    for (physAddrByteIdx = 0; physAddrByteIdx < ETH_PHYS_ADDR_LEN_BYTE; physAddrByteIdx++)
    {
      IpV4_Ip_TransferBlock[sockIdx].PhysDestAddr[physAddrByteIdx] = physDestAddrPtr[physAddrByteIdx];
    }

    IPV4_SET_FLAG_U8(IpV4_Ip_TransferBlock[sockIdx].Flags, IPV4_IP_TRANSFER_BLOCK_FLAG_PHYS_DEST_ADDR);
  }
  else
  {
    /* #30 Otherwise clear flag. */
    IPV4_CLR_FLAG_U8(IpV4_Ip_TransferBlock[sockIdx].Flags, IPV4_IP_TRANSFER_BLOCK_FLAG_PHYS_DEST_ADDR);
  }  
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  IpV4_Ip_SetEthIfFramePrio
 **********************************************************************************************************************/
/*! \brief      Set socket specific Frame Priority value that is passed to the lower layer (EthIf)
 *  \param[in]  SockIdx             socket index
 *  \param[in]  Value               Frame priority value (0-7)
 *  \return     E_OK                Frame priority updated
 *  \return     E_NOT_OK            Value out of range
 *  \context    task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV4_CODE) IpV4_Ip_SetEthIfFramePrio(TcpIp_SockIdxType SockIdx, uint8 Value)
{
  /* ------------------------------------------------- */
  /* DET */
  IpV4_CheckDetErrorReturnValue( IPV4_COND_INIT,                      IPV4_IP_API_ID_SET_ETHIF_FRAME_PRIO, IPV4_E_NOT_INITIALIZED, E_NOT_OK )
  IpV4_CheckDetErrorReturnValue( IPV4_IP_COND_VALID_SOCKIDX(SockIdx), IPV4_IP_API_ID_SET_ETHIF_FRAME_PRIO, IPV4_E_INV_SOCK_IDX,    E_NOT_OK )
  /* ------------------------------------------------- */

  if (Value > IPV4_IP_MAX_ETHIF_FRAME_PRIO)
  {
    return E_NOT_OK;
  }

  IpV4_Ip_TransferBlock[SockIdx].EthIfFramePrio = Value;
  IpV4_Ip_TransferBlock[SockIdx].Flags |= IPV4_IP_TRANSFER_BLOCK_FLAG_ETHIF_FRAME_PRIO;
  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if (IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Ip_AddrConflictInd
 **********************************************************************************************************************/
/*! \brief        Indicate an address conflict in the ARP.
 *  \description  This function is used for IP LL address configuration.
 *  \param[in]    AddrId             ip address identifier
 *  \return       void
 *  \note
 *  \context      task level
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Ip_AddrConflictInd(
    IpV4_AddrIdType AddrId )
{
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8 IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV4_5.6 */
#endif

  /* ------------------------------------------------- */
  /* DET */
  IpV4_CheckDetErrorReturnVoid( IPV4_COND_INIT, IPV4_IP_API_ID_ADDR_CONFL_IND, IPV4_E_NOT_INITIALIZED )
  IpV4_CheckDetErrorReturnVoid( IPV4_COND_VALID_IPADDRID(AddrId), IPV4_IP_API_ID_ADDR_CONFL_IND, IPV4_E_INV_CTRL_IDX )
  /* ------------------------------------------------- */

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  IPV4_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,   AddrId);
#endif

  TCPIP_ASSERT(IPV4_CTRL_ADDR_IDX_IS_UNICAST(IPV4_ADDR_ID_2_ADDR_IDX(AddrId)));

  if (IpV4_Ip_VCfgIsAddrAssignmentEnabled(IPV4_IP_ADDR_ASSIGNMENT_METHOD_LINKLOCAL))
  {
    /* check address conflict timeout */
    if(0 != IPV4_CTRL_LL_VAR(AddrConflictTimeout))
    {
      /* This is not the first conflict, so release current local IP address assignment. */

      IPV4_CTRL_LL_VAR(State) = IPV4_IP_LL_STATE_CONFLICT;
      IpV4_Ip_VIpAddrAssignmentReadyNotification(IPV4_IPCTRLIDX, IPV4_IP_ADDR_ASSIGNMENT_METHOD_LINKLOCAL, FALSE);
    }
    else
    {
      IPV4_CTRL_LL_VAR(AddrConflictTimeout) = IpV4_Ip_VCfgGetDefendIntervalMsec() / IpV4_VCfgGetMainFctPeriodMsec();

      /* send ARP announcement */
      (void)IpV4_Arp_SendArpAnnouncement(IPV4_IPCTRLIDX, IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr);
    }
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON */

#if (IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Ip_LlAddrInUse
 **********************************************************************************************************************/
/*! \brief      This function handles address conflicts
 *  \param[in]  IpCtrlIdx             ip controller index
 *  \param[in]  IpAddr                ip address that seems to be in use by other host
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Ip_LlAddrInUse(
    uint8 IpCtrlIdx,
    IpBase_AddrInType IpAddr)
{
  IPV4_UNUSED_IPCTRLIDX();  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  

  if (IpV4_Ip_VCfgIsAddrAssignmentEnabled(IPV4_IP_ADDR_ASSIGNMENT_METHOD_LINKLOCAL))
  {
    if(IPV4_IP_LL_STATE_PROBE == IPV4_CTRL_LL_VAR(State))
    {
      if ((0 != IpAddr) && (IPV4_CTRL_LL_VAR(NetAddrCand) == IpAddr))
      {
        IPV4_CTRL_LL_VAR(AddrConflict) = 1;
      }
    }
  }
}
#endif  /* IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON */

#if defined(TCPIP_INV_DEM_EVENT_ID)
# if (TcpIp_NumIpV4CtrlsOfDemEventTCPIP_E_DUPLICATE_DHCP_ADDR > 0)
/***********************************************************************************************************************
 *  IpV4_GetLastDuplicateDhcpAddrDid
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, IPV4_CODE) IpV4_GetLastDuplicateDhcpAddrDid(
    uint8                                                         IpCtrlIdx,
    CONSTP2VAR(IpBase_AddrInType, AUTOMATIC, IPV4_APPL_DATA)      IpAddrPtr,
    CONSTP2VAR(IpBase_EthPhysAddrType, AUTOMATIC, IPV4_APPL_DATA) PhysAddrPtr
    )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (IPV4_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if(!IPV4_COND_INIT)
  {
    errorId = IPV4_E_NOT_INITIALIZED;
  }
  /* Check parameter 'IpAddrPtr' */
  else if(IpAddrPtr == NULL_PTR)
  {
    errorId = IPV4_E_INV_POINTER;
  }
  /* Check parameter 'PhysAddrPtr' */
  else if(PhysAddrPtr == NULL_PTR)
  {
    errorId = IPV4_E_INV_POINTER;
  }
  /* Check parameter 'IpCtrlIdx' */
  else if(IpCtrlIdx >= TcpIp_NumIpV4CtrlsOfDemEventTCPIP_E_DUPLICATE_DHCP_ADDR)
  {
    errorId = IPV4_E_INV_PARAM;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 In case all parameters are valid assign the last duplicate DHCP address DID to the C-structs given as out-parameters */
    IpV4_VAssignPhysAddr((uint8 *)(PhysAddrPtr), TcpIp_LastDuplicateDhcpNetAddr[IpCtrlIdx].PhysAddr);  /* PRQA S 0310 */ /* MD_IpV4_11.04 */
    *IpAddrPtr = TcpIp_LastDuplicateDhcpNetAddr[IpCtrlIdx].NetAddr;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (IPV4_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(IPV4_MODULE_ID, IPV4_VINSTANCE_ID, IPV4_IP_API_ID_GET_LAST_DUPLICATE_DHCP_ADDR_DID, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */ /* SBSW_IpV4_56 */
#endif

  return retVal;
}
# endif
#endif

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

#if (IPV4_IP_ENABLE_MULTICAST_RX == STD_ON)
# if (IPV4_ENABLE_ETHIF_ADDR_FILTER_API == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Ip_VJoinMulticastGroup
 **********************************************************************************************************************/
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VJoinMulticastGroup(uint8 IpCtrlIdx, IpBase_AddrInType MulticastAddr, boolean Join)
{
  Eth_PhysAddrType     PhysMulticastAddr;
  Eth_FilterActionType EthFilterAction = (Eth_FilterActionType)((TRUE == Join) ? ETH_ADD_TO_FILTER : ETH_REMOVE_FROM_FILTER);

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  IPV4_UNUSED_PARAMETER(IpCtrlIdx);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  
#endif

  if (IPV4_ADDR_IS_MULTICAST(MulticastAddr))
  {
    /* configure physical address filter */
    IpV4_Arp_MapIpToPhysMulticastAddr(MulticastAddr, &PhysMulticastAddr[0]);

    if (E_OK != TCPIP_LL_UpdatePhysAddrFilter(IPV4_CTRL_IDX_IP_2_ETHIF(IpCtrlIdx), &PhysMulticastAddr[0], EthFilterAction))
    {
      IpV4_ReportDetError(IPV4_IP_API_ID_V_INTERNAL_FUNCTION, IPV4_E_INV_PARAM);
    }
  }
} /* End of IpV4_Ip_VJoinMulticastGroup() */
# endif

/***********************************************************************************************************************
 *  IpV4_Ip_VUpdateMulticastMemberships
 **********************************************************************************************************************/
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VUpdateMulticastMemberships(uint8 IpCtrlIdx, boolean Join)
{
  /* initialize configured multicast address assignments */
  uint8 MulticastAddrIdx = IpV4_Ip_VCfgGetMulticastAddrCount();

  while (0 < MulticastAddrIdx)
  {
    MulticastAddrIdx--;

    if (IPV4_ADDR_ANY != IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs)[MulticastAddrIdx])
    {
      TcpIp_IpAddrStateType AddrState = (TcpIp_IpAddrStateType)((TRUE == Join) ? TCPIP_IPADDR_STATE_ASSIGNED : TCPIP_IPADDR_STATE_UNASSIGNED);

# if (IPV4_ENABLE_ETHIF_ADDR_FILTER_API == STD_ON)
      if (!IpV4_Ip_VCfgIsReceiveAllMulticastsEnabled())
      {
        /* configure individual physical address filter */
        IpV4_Ip_VJoinMulticastGroup(IpCtrlIdx, IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs)[MulticastAddrIdx], Join);
      }
# endif

      IpV4_VLocalIpAddrAssignmentChange(IpCtrlIdx, IPV4_CTRL_MULTICAST_ADDR_IDX_2_ADDR_IDX(MulticastAddrIdx), AddrState);
    }
  }

# if (IPV4_ENABLE_ETHIF_ADDR_FILTER_API == STD_ON)
  if (IpV4_Ip_VCfgIsReceiveAllMulticastsEnabled())
  {
    const Eth_PhysAddrType PhysBroadcastAddr  = { 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU };
    Eth_FilterActionType EthFilterAction = (Eth_FilterActionType)((TRUE == Join) ? ETH_ADD_TO_FILTER : ETH_REMOVE_FROM_FILTER);

    /* Enable the promiscuous mode to receive multicast messages */
    if (E_OK != TCPIP_LL_UpdatePhysAddrFilter(IPV4_CTRL_IDX_IP_2_ETHIF(IpCtrlIdx), &PhysBroadcastAddr[0], EthFilterAction))
    {
      IpV4_ReportDetError(IPV4_IP_API_ID_V_INTERNAL_FUNCTION, IPV4_E_INV_PARAM);
    }
  }
# endif
} /* End of IpV4_Ip_VUpdateMulticastMemberships() */
#endif

/***********************************************************************************************************************
 *  IpV4_Ip_VIpAddrAssignmentReadyNotification()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, IPV4_CODE) IpV4_Ip_VIpAddrAssignmentReadyNotification(
  uint8   IpCtrlIdx, 
  uint8   addrAssignmentMethod,
  boolean addrAssigned)
{
#if (IPV4_IP_ENABLE_ADDR_NVM_STORAGE == STD_ON)
  uint8 addrAssignmentPrio = IpV4_Ip_VCfgGetAddrAssignmentPrio(addrAssignmentMethod);
  uint8 addrAssignmentCfg  = IpV4_Ip_VCfgGetAddrAssignmentCfgByPrio(addrAssignmentPrio);

  /* #10 Check if address of specified assignment method shall be stored into NvM. */

  TCPIP_ASSERT(addrAssignmentMethod != IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE);
  TCPIP_ASSERT(IPV4_CHK_BIT_U8(IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentTriggeredFlags), addrAssignmentMethod) == TRUE);

  if ((addrAssigned == TRUE) && (IPV4_CHK_FLAG_U8(addrAssignmentCfg, IPV4_IP_ADDR_ASSIGNMENT_FLAG_LIFETIME_STORE) == TRUE))
  {
    TCPIP_ASSERT(IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentMethodPersist) == IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE);

    /* #20 Set 'persist' flag, so address will be written into NvM in MainFunction. */
    IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentMethodPersist) = addrAssignmentMethod;
  }
#else
  IPV4_UNUSED_PARAMETER(IpCtrlIdx);            /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  IPV4_UNUSED_PARAMETER(addrAssignmentMethod); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
  IPV4_UNUSED_PARAMETER(addrAssigned);         /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentReadyChanged) = TRUE;
} /* IpV4_Ip_VIpAddrAssignmentReadyNotification() */

/***********************************************************************************************************************
 *  IpV4_Ip_VGetBestAvailableAddrAssignment
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
STATIC FUNC(uint8, IPV4_CODE) IpV4_Ip_VGetBestAvailableAddrAssignment(
  uint8                         IpCtrlIdx, 
  IPV4_P2V(uint8)               addrAssignmentPriorityPtr,
  IPV4_P2V(IpV4_IpAddrSetType)  addrInfoPtr)
{
  uint8_least addrAssignmentPrio;
  boolean     addrAssignmentReady  = FALSE;
  uint8       addrAssignmentMethod = IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE;

  if (IPV4_COND_VALID_CTRLIDX(IpCtrlIdx))
  {
    /* #10 Iterate over configured address assignments for current controller (ordered by high to low priority). */
    for (addrAssignmentPrio = IPV4_IP_ADDR_ASSIGNMENT_PRIO_HIGHEST; addrAssignmentPrio <= IPV4_IP_ADDR_ASSIGNMENT_PRIO_LOWEST; addrAssignmentPrio++)
    {
      uint8 addrAssignmentCfg = IpV4_Ip_VCfgGetAddrAssignmentCfgByPrio(addrAssignmentPrio);
      addrAssignmentMethod    = IpV4_Ip_VCfgGetAddrAssignmentMethodByCfg(addrAssignmentCfg);

      if (addrAssignmentMethod == IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE)
      {
        /* No more address assignments configured. */
        break;
      }

      /* #20 Check if address assignment is active. */
      if (IPV4_CHK_BIT_U8(IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentTriggeredFlags), addrAssignmentMethod) == TRUE)
      {
#if (IPV4_IP_ENABLE_ADDR_NVM_STORAGE == STD_ON)
        /* #30 Check if an address is stored into NvM for this assignment method. Is so, use it. */
        if (IPV4_CHK_FLAG_U8(addrAssignmentCfg, IPV4_IP_ADDR_ASSIGNMENT_FLAG_LIFETIME_STORE) == TRUE)
        {
          TCPIP_ASSERT(IpV4_Ip_VCfgGetNvmAddrVar() != NULL_PTR);
       
          if ((IpV4_Ip_VCfgGetNvmAddrVar()->NetAddr != IPV4_ADDR_ANY) && 
              (IpV4_Ip_VCfgGetNvmAddrVar()->NetMask  > 0))
          {
            /* Use address stored in NvM. */
            addrAssignmentReady = TRUE;
            *addrInfoPtr = *IpV4_Ip_VCfgGetNvmAddrVar();
          }
        }

        if (addrAssignmentReady == FALSE)
#endif
        {
          addrAssignmentReady = IpV4_Ip_VGetActiveAddressOfAssignmentMethod(IpCtrlIdx, addrAssignmentMethod, addrInfoPtr);
        }

        if (addrAssignmentReady == TRUE)
        {
          break; /* PRQA S 0771 */ /* MD_IPV4_0771 */
        }
      }
    }

    /* #40 Store priority of best available address assignment method at addrAssignmentPriorityPtr (if there is one). */
    if (addrAssignmentReady == TRUE) 
    {
      *addrAssignmentPriorityPtr = (uint8)addrAssignmentPrio;
    }
    else
    {
      addrAssignmentMethod = IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE;
    }
  }
  else
  {
    TCPIP_ASSERT_UNREACHABLE();
  }

  return addrAssignmentMethod;
} /* IpV4_Ip_VGetBestAvailableAddrAssignment() */ /* PRQA S 6010, 6030, 6080  */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  IpV4_Ip_VGetActiveAddressOfAssignmentMethod()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
STATIC FUNC(boolean, IPV4_CODE) IpV4_Ip_VGetActiveAddressOfAssignmentMethod(
  uint8                        IpCtrlIdx, 
  uint8                        addrAssignmentMethod,
  IPV4_P2V(IpV4_IpAddrSetType) addrInfoPtr)
{
  boolean retVal = FALSE;

  IPV4_UNUSED_IPCTRLIDX();  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  /* #10 Check if specified address assignment method has a valid address and store value in addrInfoPtr. */
  switch (addrAssignmentMethod)
  {
#if (IPV4_IP_DHCP_ADDR_CFG_SUPP == STD_ON)
  case IPV4_IP_ADDR_ASSIGNMENT_METHOD_DHCP:
    retVal = TcpIp_DhcpV4_GetActiveAddress(IpCtrlIdx, addrInfoPtr);
    break;
#endif

#if (IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON)
  case IPV4_IP_ADDR_ASSIGNMENT_METHOD_LINKLOCAL:
    if ((IPV4_CTRL_LL_VAR(State) == IPV4_IP_LL_STATE_ANNOUNCE) ||
        (IPV4_CTRL_LL_VAR(State) == IPV4_IP_LL_STATE_READY))
    {
      addrInfoPtr->NetAddr   = IPV4_CTRL_LL_VAR(AddrFound);
      addrInfoPtr->NetMask   = IPV4_IP_LL_NET_ADDR_MASK;
      addrInfoPtr->DefGwAddr = TCPIP_INADDR_BROADCAST;
      retVal = TRUE;
    }
    break;
#endif

  case IPV4_IP_ADDR_ASSIGNMENT_METHOD_STATIC:
    if (IPV4_CTRL_VAR(IpV4_Ip_StaticAddr_Requested)->NetAddr != IPV4_ADDR_ANY)
    {
      (*addrInfoPtr) = *IPV4_CTRL_VAR(IpV4_Ip_StaticAddr_Requested);
      retVal = TRUE;
    }
    break;

  default:
    TCPIP_ASSERT_UNREACHABLE();
    break;
  }

  return retVal;
} /* IpV4_Ip_VGetActiveAddressOfAssignmentMethod() */

/***********************************************************************************************************************
 *  IpV4_Ip_VTriggerAddrAssignment
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
STATIC FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Ip_VTriggerAddrAssignment(
    uint8   IpCtrlIdx,
    uint8   addrAssignmentCfg,
    boolean triggerValue)
{
  boolean retVal = E_NOT_OK;
  uint8 addrAssignmentMethod = IpV4_Ip_VCfgGetAddrAssignmentMethodByCfg(addrAssignmentCfg);

  IPV4_UNUSED_IPCTRLIDX();  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  

  TCPIP_ASSERT((addrAssignmentMethod == IPV4_IP_ADDR_ASSIGNMENT_METHOD_DHCP) ||
              (addrAssignmentMethod == IPV4_IP_ADDR_ASSIGNMENT_METHOD_LINKLOCAL) ||
              (addrAssignmentMethod == IPV4_IP_ADDR_ASSIGNMENT_METHOD_STATIC));

  /* #10 Check if trigger value differs from current value. */
  if (triggerValue == TRUE)
  {
    if (IPV4_CHK_BIT_U8(IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentTriggeredFlags), addrAssignmentMethod))
    {
      /* Trigger already set. */
    }
    else
    {
      IPV4_SET_BIT_U8(IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentTriggeredFlags), addrAssignmentMethod);

      {
        /* #20 Do not trigger dynamic address assignment (DHCP, LINKLOCAL) if an address of this method was stored in the NvM before. */
#if (IPV4_IP_ENABLE_ADDR_NVM_STORAGE == STD_ON)
        boolean triggerAssignment = TRUE;

        if (IPV4_CHK_FLAG_U8(addrAssignmentCfg, IPV4_IP_ADDR_ASSIGNMENT_FLAG_LIFETIME_STORE) == TRUE)
        {
          TCPIP_ASSERT(IpV4_Ip_VCfgGetNvmAddrVar() != NULL_PTR);

          if (IpV4_Ip_VCfgGetNvmAddrVar()->NetAddr != IPV4_ADDR_ANY)
          {
            /* There is an address stored in the NvM. Avoid that dynamic address assignment is started below. */ 
            triggerAssignment = FALSE;
          }
        }

        /* #30 Otherwise start dynamic address assignment method (in case of DHCP or LINKLOCAL). */
        if (triggerAssignment == TRUE)
#endif
        {
          switch (addrAssignmentMethod)
          {
#if (IPV4_IP_DHCP_ADDR_CFG_SUPP == STD_ON)
          case IPV4_IP_ADDR_ASSIGNMENT_METHOD_DHCP:
            TcpIp_DhcpV4_TriggerAddrRequest(IpCtrlIdx, 0);
            retVal = E_OK;
            break;
#endif

#if (IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON)
          case IPV4_IP_ADDR_ASSIGNMENT_METHOD_LINKLOCAL:
            IpV4_Ip_VLl_Start(IpCtrlIdx);
            retVal = E_OK;
            break;
#endif

          case IPV4_IP_ADDR_ASSIGNMENT_METHOD_STATIC:
            retVal = E_OK;
            break;

          default:
            /* No action required. */
            TCPIP_ASSERT_UNREACHABLE();
            break;
          }
        }
      }
    }
  }
  else
  {
    if (IPV4_CHK_BIT_U8(IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentTriggeredFlags), addrAssignmentMethod))
    {
      IPV4_CLR_BIT_U8(IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentTriggeredFlags), addrAssignmentMethod);

      /* #40 Stop dynamic address assignment method (in case of DHCP or LINKLOCAL). */
      switch (addrAssignmentMethod)
      {
#if (IPV4_IP_DHCP_ADDR_CFG_SUPP == STD_ON)
      case IPV4_IP_ADDR_ASSIGNMENT_METHOD_DHCP:
        TcpIp_DhcpV4_StopAddrRequest(IpCtrlIdx);
        retVal = E_OK;
        break;
#endif

#if (IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON)
      case IPV4_IP_ADDR_ASSIGNMENT_METHOD_LINKLOCAL:
        IpV4_Ip_VLl_Reset(IpCtrlIdx);
        retVal = E_OK;
        break;
#endif

      case IPV4_IP_ADDR_ASSIGNMENT_METHOD_STATIC:
        retVal = E_OK;
        break;

      default:
        /* No action required. */
        TCPIP_ASSERT_UNREACHABLE();
          break;
      }
    }
    else
    {
      /* Trigger already unset. */
    }
  }

  return retVal;
} /* IpV4_Ip_VTriggerAddrAssignment() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  IpV4_Ip_VTriggerAutomaticAddrAssignments()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VTriggerAutomaticAddrAssignments(uint8 IpCtrlIdx)
{
  uint8_least addrAssignmentPrio;

  /* #10 Iterate over all configured address assignment methods for the IP instance. */
  for (addrAssignmentPrio = IPV4_IP_ADDR_ASSIGNMENT_PRIO_HIGHEST; addrAssignmentPrio <= IPV4_IP_ADDR_ASSIGNMENT_PRIO_LOWEST; addrAssignmentPrio++)
  {
    uint8 addrAssignmentCfg     = IpV4_Ip_VCfgGetAddrAssignmentCfgByPrio(addrAssignmentPrio);
    uint8 addrAssignmentMethod  = IpV4_Ip_VCfgGetAddrAssignmentMethodByCfg(addrAssignmentCfg);

    /* #20 If assignment method trigger is configured to AUTOMATIC set trigger flag. */
    if ((addrAssignmentMethod != IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE) && ((addrAssignmentCfg & IPV4_IP_ADDR_ASSIGNMENT_FLAG_AUTO_TRIGGER) != 0))
    {
      if (IpV4_Ip_VTriggerAddrAssignment(IpCtrlIdx, addrAssignmentCfg, TRUE) == E_NOT_OK)
      {
        TCPIP_ASSERT_UNREACHABLE_MSG("Address assignment method should always be triggerable here.");
      }

      /* #30 If assignment method is STATIC set active address of assignment to configured default address (if there is one). */
      if (addrAssignmentMethod == IPV4_IP_ADDR_ASSIGNMENT_METHOD_STATIC)
      {
        TCPIP_ASSERT(IPV4_CTRL_VAR(IpV4_Ip_StaticAddr_Default) != NULL_PTR);

        *IPV4_CTRL_VAR(IpV4_Ip_StaticAddr_Requested) = *IPV4_CTRL_VAR(IpV4_Ip_StaticAddr_Default);
      }
    }
  }
}

/**********************************************************************************************************************
 *  IpV4_Ip_VSetCtrlStateOffline()
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
 */
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VSetCtrlStateOffline(uint8 IpCtrlIdx)
{
  /* #10 If IP instance is in state ONLINE_ACTIVE move to ONLINE_PASSIVE first. */
  if (IPV4_CTRL_VAR(IpV4_Ip_CtrlState) == IPV4_IP_ONLINE_ACTIVE)
  {
    IpV4_Ip_VSetCtrlStateOnlinePassive(IpCtrlIdx);
  }

  if (IPV4_CTRL_VAR(IpV4_Ip_CtrlState) == IPV4_IP_ONLINE_PASSIVE)
  {

    /* #20 If IP instance is in state ONLINE_PASSIVE stop all dynamic address assignments. */
    (void)IpV4_Ip_VTriggerAddrAssignment(IpCtrlIdx, IPV4_IP_ADDR_ASSIGNMENT_METHOD_DHCP, FALSE);
    (void)IpV4_Ip_VTriggerAddrAssignment(IpCtrlIdx, IPV4_IP_ADDR_ASSIGNMENT_METHOD_LINKLOCAL, FALSE);   

    /* #30 Clear address assignment trigger flags for all address assignment methods of the IP instance. */
    IPV4_CTRL_VAR(IpV4_Ip_AddrAssignmentTriggeredFlags) = 0;

    /* #40 Reset ARP submodule. */
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
    IpV4_Arp_Reset(IPV4_IPCTRLIDX);
#endif

    /* #50 Reset all multicast addresses to default values. */
#if (IPV4_IP_ENABLE_MULTICAST_RX == STD_ON)
    {
      uint8_least mcAddrIdx;
      
      for (mcAddrIdx = 0; mcAddrIdx < IpV4_Ip_VCfgGetMulticastAddrCount(); mcAddrIdx++)
      {
        IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs)[mcAddrIdx] = IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs_Default)[mcAddrIdx];
      }
    }
#endif

    /* #60 Set state to OFFLINE. */
    IPV4_CTRL_VAR(IpV4_Ip_CtrlState) = IPV4_IP_OFFLINE;
  }
  else
  {
    /* Invalid state transition. */
    TCPIP_ASSERT_UNREACHABLE();
  }
}

/**********************************************************************************************************************
 *  IpV4_Ip_VSetCtrlStateOnlinePassive()
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
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VSetCtrlStateOnlinePassive(uint8 IpCtrlIdx)
{
  /* #10 If IP instance is in state ONLINE_ACTIVE: */
  if (IPV4_CTRL_VAR(IpV4_Ip_CtrlState) == IPV4_IP_ONLINE_ACTIVE)
  {

#if (IPV4_IP_ENABLE_MULTICAST_RX == STD_ON)
    /* #20 Leave all multicast groups (if reception of multicast packets is configured.) */
    if ((0 < IpV4_Ip_VCfgGetMulticastAddrCount()) || (IpV4_Ip_VCfgIsReceiveAllMulticastsEnabled()))
    {
      /* update physical address filters */
      IpV4_Ip_VUpdateMulticastMemberships(IpCtrlIdx, FALSE);
    }
#endif

    /* #30 Unassign broadcast address. */
    if (IPV4_ADDR_ID_INV != IPV4_CTRL_VAR(IpV4_Ip_BcAddrId))
    {
      IpV4_VLocalIpAddrAssignmentChange(IpCtrlIdx, IPV4_CTRL_ADDR_IDX_ANY, TCPIP_IPADDR_STATE_UNASSIGNED);
    }

    TCPIP_ASSERT(IPV4_CTRL_VAR(IpV4_Ip_ActiveAddrAssignmentMethod) != IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE);

    /* #40 Unassign unicast address. */
    IpV4_VLocalIpAddrAssignmentChange(IpCtrlIdx, IPV4_CTRL_ADDR_IDX_FIRST, TCPIP_IPADDR_STATE_UNASSIGNED);

    /* #50 Set IP instance state to PASSIVE and active assignment method to NONE. */
    IPV4_CTRL_VAR(IpV4_Ip_CtrlState)                  = IPV4_IP_ONLINE_PASSIVE;
    IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr         = IPV4_ADDR_ANY;
    IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetMask         = 0;
    IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).DefGwAddr       = IPV4_ADDR_ANY;
    IPV4_CTRL_VAR(IpV4_Ip_ActiveAddrAssignmentMethod) = IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE;

    /* #60 Notify TcpIp/EthSM. */
    ETHSM_TCPIP_MODE_INDICATION(IpV4_CtrlIdxIp2EthIf[IpCtrlIdx], FALSE);
  }
  /* #70 If IP instance is in state OFFLINE: */
  else if (IPV4_CTRL_VAR(IpV4_Ip_CtrlState) == IPV4_IP_OFFLINE)
  {

    TCPIP_ASSERT(IPV4_CTRL_VAR(IpV4_Ip_ActiveAddrAssignmentMethod) == IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE);

    /* #80 Trigger all configured address assignments with trigger AUTOMATIC. */
    IpV4_Ip_VTriggerAutomaticAddrAssignments(IpCtrlIdx);

    /* #90 Set IP instance state to ONLINE_PASSIVE. */
    IPV4_CTRL_VAR(IpV4_Ip_CtrlState) = IPV4_IP_ONLINE_PASSIVE;
  }
  else
  {
    /* ERROR: Invalid state transition. */
    TCPIP_ASSERT_UNREACHABLE();
  }
} /* End of IpV4_Ip_VChangeCtrlState() */

/**********************************************************************************************************************
 *  IpV4_Ip_VSetCtrlStateOnlineActive()
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
 */
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VSetCtrlStateOnlineActive(uint8 IpCtrlIdx)
{
  /* #10 If IP instance is in state ONLINE_PASSIVE. */
  if (IPV4_CTRL_VAR(IpV4_Ip_CtrlState) == IPV4_IP_ONLINE_PASSIVE)
  {

    /* #20 Set IP instance state to ONLINE_ACTIVE. */
    IPV4_CTRL_VAR(IpV4_Ip_CtrlState) = IPV4_IP_ONLINE_ACTIVE;

    TCPIP_ASSERT(IPV4_CTRL_VAR(IpV4_Ip_ActiveAddrAssignmentMethod) != IPV4_IP_ADDR_ASSIGNMENT_METHOD_NONE);

    /* #30 Notify upper layer about available unicast address. */
    IpV4_VLocalIpAddrAssignmentChange(IpCtrlIdx, IPV4_CTRL_ADDR_IDX_FIRST, TCPIP_IPADDR_STATE_ASSIGNED);

    /* #40 Notify upper layer about available broadcast address. */
    if (IPV4_ADDR_ID_INV != IPV4_CTRL_VAR(IpV4_Ip_BcAddrId))
    {
      IpV4_VLocalIpAddrAssignmentChange(IpCtrlIdx, IPV4_CTRL_ADDR_IDX_ANY, TCPIP_IPADDR_STATE_ASSIGNED);
    }

#if (IPV4_IP_ENABLE_MULTICAST_RX == STD_ON)
    /* #50 Join multicast groups. (if reception of multicast packets is configured.) */
    if ((0 < IpV4_Ip_VCfgGetMulticastAddrCount()) || (IpV4_Ip_VCfgIsReceiveAllMulticastsEnabled()))
    {
      /* update physical address filters */
      IpV4_Ip_VUpdateMulticastMemberships(IpCtrlIdx, TRUE);
    }
#endif

    /* #60 Notify TcpIp/EthSM. */
    ETHSM_TCPIP_MODE_INDICATION(IpV4_CtrlIdxIp2EthIf[IpCtrlIdx], TRUE);
  }
  else
  {
    /* ERROR: Invalid state transition. */
    TCPIP_ASSERT_UNREACHABLE();
  }
}

/***********************************************************************************************************************
 *  IpV4_VLocalIpAddrAssignmentChange()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV4_CODE) IpV4_VLocalIpAddrAssignmentChange(
    uint8                 IpCtrlIdx,
    uint8                 CtrlAddrIdx,
    TcpIp_IpAddrStateType State)
{
  IpV4_AddrIdType       TmpIpAddrId;

  IPV4_UNUSED_IPCTRLIDX();  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */

  if (IPV4_CTRL_ADDR_IDX_ANY != CtrlAddrIdx)
  {
    TmpIpAddrId    = IPV4_CTRL_VAR(IpV4_Ip_AddrConfig)[CtrlAddrIdx].IpAddrId;
  }
  else
  {
    TmpIpAddrId    = IPV4_CTRL_VAR(IpV4_Ip_BcAddrId);
  }

#if (IPV4_SUPPORT_ICMP  == STD_ON)
  IpV4_Icmp_VLocalIpAddrAssignmentChange(TmpIpAddrId, State);
#endif
  TcpIp_Cbk_VLocalIpAssignmentChg(IPV4_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(TmpIpAddrId), State);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if (IPV4_IP_ENABLE_MULTICAST_RX == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Ip_VChangeMulticastAddrAssignment
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
STATIC FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Ip_VChangeMulticastAddrAssignment(
    uint8             IpCtrlIdx,
    uint8             mcAddrIdx,
    IpBase_AddrInType addrValue)
{
  Std_ReturnType  retVal = E_NOT_OK;

  TCPIP_ASSERT(mcAddrIdx < IpV4_Ip_VCfgGetMulticastAddrCount());

  /* #10 Check if address value differs from current value. */
  if (IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs)[mcAddrIdx] != addrValue)
  {
    /* #20 Check if address value is a valid multicast address or ANY (in order remove address assignment). */
    if (IPV4_ADDR_IS_MULTICAST(addrValue) || IPV4_ADDR_IS_BROADCAST(addrValue) || (addrValue == IPV4_ADDR_ANY))
    {
      /* #30 Leave multicast group if an address is currently assigned. */
      if (IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs)[mcAddrIdx] != IPV4_ADDR_ANY)
      {
# if (IPV4_ENABLE_ETHIF_ADDR_FILTER_API == STD_ON)
        /* leave current multicast group */
        IpV4_Ip_VJoinMulticastGroup(IpCtrlIdx, IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs)[mcAddrIdx], FALSE);
# endif

        IpV4_VLocalIpAddrAssignmentChange(IpCtrlIdx, IPV4_CTRL_MULTICAST_ADDR_IDX_2_ADDR_IDX(mcAddrIdx), TCPIP_IPADDR_STATE_UNASSIGNED);
      }

      /* #40 Update address value. */
      if (addrValue != IPV4_ADDR_ANY)
      {
        /* update multicast address assignment */
        IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs)[mcAddrIdx] = addrValue;
      }
      else
      {
        /* set configured default address value. */
        IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs)[mcAddrIdx] = IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs_Default)[mcAddrIdx];
      }

      /* #50 Join multicast group if a new address was assigned. */
      if (IPV4_ADDR_ANY != IPV4_CTRL_VAR(IpV4_Ip_MulticastAddrs)[mcAddrIdx])
      {
# if (IPV4_ENABLE_ETHIF_ADDR_FILTER_API == STD_ON)
        /* join new multicast group */
        IpV4_Ip_VJoinMulticastGroup(IpCtrlIdx, addrValue, TRUE);
# endif

        IpV4_VLocalIpAddrAssignmentChange(IpCtrlIdx, IPV4_CTRL_MULTICAST_ADDR_IDX_2_ADDR_IDX(mcAddrIdx), TCPIP_IPADDR_STATE_ASSIGNED);
      }

      retVal = E_OK;
    }
    else
    {
      /* address is not a valid multicast or broadcast */
    }
  }

  return retVal;
} /* IpV4_Ip_VChangeMulticastAddrAssignment() */
#endif

#if defined TCPIP_INV_DEM_EVENT_ID
/***********************************************************************************************************************
 *  IpV4_ResetDemEventAllIpCtrl
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_ResetDemEventAllIpCtrl(
  CONSTP2CONST(PduIdType, AUTOMATIC, IPV4_CONST) DemEventSnvsByIpV4Ctrl,
  uint8 NumIpV4CtrlsOfDemEvent
)
{
  uint8 IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV4_5.6 */
  for(IpCtrlIdx = 0; IpCtrlIdx < NumIpV4CtrlsOfDemEvent; IpCtrlIdx++)
  {
    if(DemEventSnvsByIpV4Ctrl[IpCtrlIdx] != TCPIP_INV_DEM_EVENT_ID)
    {
      Dem_ReportErrorStatus(DemEventSnvsByIpV4Ctrl[IpCtrlIdx], DEM_EVENT_STATUS_PREPASSED);
    }
  }
}

/***********************************************************************************************************************
 *  IpV4_ResetDemEventIpCtrl
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_ResetDemEventIpCtrl(
  CONSTP2CONST(PduIdType, AUTOMATIC, IPV4_CONST) DemEventSnvsByIpV4Ctrl,
  uint8 NumIpV4CtrlsOfDemEvent,
  uint8 IpCtrlIdx
)
{
  if(    (IpCtrlIdx < NumIpV4CtrlsOfDemEvent)
      && (DemEventSnvsByIpV4Ctrl[IpCtrlIdx] != TCPIP_INV_DEM_EVENT_ID)
    )
  {
    Dem_ReportErrorStatus(DemEventSnvsByIpV4Ctrl[IpCtrlIdx], DEM_EVENT_STATUS_PREPASSED);
  }
}
#endif

#if (IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Ip_VLl_GenerateIpAddr
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(IpBase_AddrInType, TCPIP_CODE) IpV4_Ip_VLl_GenerateIpAddr(uint8 IpCtrlIdx, uint8 conflictCount)
{
  IpV4_AddrIdType ipAddrId = IPV4_CTRL_VAR(IpV4_Ip_AddrConfig)[IPV4_CTRL_ADDR_IDX_FIRST].IpAddrId;
  IpV4_LinkLocalAddrCalloutFuncType addrCalloutFunc = IpV4_Ip_VCfgGetLinkLocalAddrCalloutFunc();

  /* #10 Initialize IP address value to 0.0.0.0. */
  IpBase_AddrInType ipAddr = IPV4_ADDR_ANY;

  IPV4_UNUSED_IPCTRLIDX();  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  

  /* #20 If configured, call user callout function for IP address generation. */
  if (addrCalloutFunc != NULL_PTR)
  {
    addrCalloutFunc(IPV4_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(ipAddrId), conflictCount, &ipAddr);
  }

  /* #30 If callout does not specify IP address, generate random address in link-local subnet (169.254.1.0) */
  if (ipAddr == IPV4_ADDR_ANY)
  {
    ipAddr = IPV4_IP_LL_NET_ADDR_START_NBO + (TcpIp_GetRandNoFct() % IPV4_IP_LL_NET_ADDR_COUNT);
  }

  return IPV4_NTOHL(ipAddr);
} /* IpV4_Ip_VLl_GenerateIpAddr */

/***********************************************************************************************************************
 *  IpV4_Ip_VLl_Reset
 **********************************************************************************************************************/
/*! \brief      This function resets variables for the dynamic IP address assignment
 *  \param[in]  IpCtrlIdx             controller index
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VLl_Reset(
    uint8 IpCtrlIdx )
{
  IPV4_UNUSED_IPCTRLIDX();  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  

  IPV4_CTRL_LL_VAR(AddrConflictTimeout) = 0;
  IPV4_CTRL_LL_VAR(AddrFound)           = 0;
  IPV4_CTRL_LL_VAR(NetAddrCand)         = 0;
  IPV4_CTRL_LL_VAR(MsecCnt)             = 0;
  IPV4_CTRL_LL_VAR(ProbeNumSinceConf)   = 0;
  IPV4_CTRL_LL_VAR(AnnounceNum)         = 0;
  IPV4_CTRL_LL_VAR(ConflictsNum)        = 0;
  IPV4_CTRL_LL_VAR(State)               = IPV4_IP_LL_STATE_INACTIVE;
  IPV4_CTRL_LL_VAR(ProbeState)          = IPV4_IP_LL_PROBE_STATE_CHOOSE;
  IPV4_CTRL_LL_VAR(AnnState)            = IPV4_IP_LL_ANN_STATE_PROBE;

  IPV4_CTRL_LL_VAR(WaitUntilTime)       = 0;
  IPV4_CTRL_LL_VAR(AddrConflict)        = 0;
}

/***********************************************************************************************************************
 *  IpV4_Ip_VLl_Start
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VLl_Start(
    uint8 IpCtrlIdx )
{
  IPV4_UNUSED_IPCTRLIDX();  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  

  /* #10 Start LINKLOCAL address assignment if not already started. */
  if (IPV4_CTRL_LL_VAR(State) == IPV4_IP_LL_STATE_INACTIVE)
  {
    IPV4_CTRL_LL_VAR(State) = IPV4_IP_LL_STATE_INIT;
  }
  else
  {
    TCPIP_ASSERT_UNREACHABLE();
  }
}

/***********************************************************************************************************************
 *  IpV4_Ip_VLl_Main
 **********************************************************************************************************************/
/*! \brief      This function processes handling of LinkLocal address
 *  \param[in]  IpCtrlIdx             controller index
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VLl_Main(
    uint8 IpCtrlIdx )
{
  if (IPV4_CTRL_LL_VAR(State) != IPV4_IP_LL_STATE_INACTIVE)
  {
    if(0 != IPV4_CTRL_LL_VAR(AddrConflictTimeout))
    {
      /* decrease address conflict timeout */
      IPV4_CTRL_LL_VAR(AddrConflictTimeout)--;
    }

    /* No address has been configured or there are pending announcements for a link-local address */
    IpV4_Ip_VLl_FindAddr(IpCtrlIdx);
  }
  else
  {
  }

}

/***********************************************************************************************************************
 *  IpV4_Ip_VLl_FindAddr
 **********************************************************************************************************************/
/*! \brief      This function tries to get a LinkLocal address
 *  \param[in]  IpCtrlIdx             controller index
 *  \context    task level
 **********************************************************************************************************************/
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VLl_FindAddr(
    uint8 IpCtrlIdx )
{

  /*
     This function is only called under the following conditions:
     - assert( Ip_GlobalState == IP_INIT )
     - assert( Ip_RAM_ActiveStatus(CtrlIdx).CtrlState == IP_ONLINE_ADDR_REQUESTED )

     while(no addr found)
       choose random IP addr
       send ARP request to check if Addr is already in use
       poll ARP for a while (x seconds)
         if(not in use) -> take this addr -> break
         else -> increment counter
     if(no valid (free) add after y seconds) -> timeout -> error?
  */
  switch (IPV4_CTRL_LL_VAR(State))
  {
  case IPV4_IP_LL_STATE_INIT:
    {
      uint16 RandNo = TcpIp_VIpGetRandNoFct();

      IPV4_CTRL_LL_VAR(WaitUntilTime) = IPV4_CTRL_LL_VAR(MsecCnt) + IpV4_Ip_VCfgGetProbeWaitMinMsec();
      /* choose random time between "IPV4_IP_PROBE_WAIT_MIN_MS" and "IPV4_IP_PROBE_WAIT_MAX_MS" */
      if (IpV4_Ip_VCfgGetProbeWaitMaxMsec() > IpV4_Ip_VCfgGetProbeWaitMinMsec())
      {
        IPV4_CTRL_LL_VAR(WaitUntilTime) += (RandNo % ( IpV4_Ip_VCfgGetProbeWaitMaxMsec() - IpV4_Ip_VCfgGetProbeWaitMinMsec()));
      }

      IPV4_CTRL_LL_VAR(State) = IPV4_IP_LL_STATE_WAIT;
    }
    break;

  case IPV4_IP_LL_STATE_WAIT:
    {
      /* wait some time before issue the next probe cycle */
      if(IPV4_CTRL_LL_VAR(WaitUntilTime) <= IPV4_CTRL_LL_VAR(MsecCnt))
      {
        IPV4_CTRL_LL_VAR(State)      = IPV4_IP_LL_STATE_PROBE;
        IPV4_CTRL_LL_VAR(ProbeState) = IPV4_IP_LL_PROBE_STATE_CHOOSE;
      }
      else
      {
        /* wait a little longer */
        /* do nothing */
      }
    }
    break;

  case IPV4_IP_LL_STATE_PROBE:
    {
      if(IPV4_CTRL_LL_VAR(AddrConflict) != 0)
      {
        /* goto conflict */
        IPV4_CTRL_LL_VAR(State) = IPV4_IP_LL_STATE_CONFLICT;
      }
      else
      {

        if(IPV4_CTRL_LL_VAR(ProbeState) == IPV4_IP_LL_PROBE_STATE_CHOOSE)
        {
          IPV4_CTRL_LL_VAR(NetAddrCand) = IpV4_Ip_VLl_GenerateIpAddr(IpCtrlIdx, IPV4_CTRL_LL_VAR(ConflictsNum));
          IPV4_CTRL_LL_VAR(ProbeState)  = IPV4_IP_LL_PROBE_STATE_PROBE;
        }

        if(IPV4_CTRL_LL_VAR(ProbeState) == IPV4_IP_LL_PROBE_STATE_PROBE)
        {
          Std_ReturnType     RetArp;

          RetArp = IpV4_Arp_SendArpProbe(IpCtrlIdx, IPV4_CTRL_LL_VAR(NetAddrCand));
          if(E_OK == RetArp)
          {
            IPV4_CTRL_LL_VAR(ProbeNumSinceConf)++;
            IPV4_CTRL_LL_VAR(ProbeState) = IPV4_IP_LL_PROBE_STATE_WAIT;

            if(IPV4_CTRL_LL_VAR(ProbeNumSinceConf) == IpV4_Ip_VCfgGetProbeNum())
            {
              /* wait IPV4_IP_ANNOUNCE_WAIT_MS ms */
              IPV4_CTRL_LL_VAR(WaitUntilTime) = IPV4_CTRL_LL_VAR(MsecCnt) + IpV4_Ip_VCfgGetAnnounceWaitMsec();
            }
            else
            {
              /* wait random time (IPV4_IP_PROBE_MIN_MS .. IPV4_IP_PROBE_MAX_MS) ms */
              IPV4_CTRL_LL_VAR(WaitUntilTime) = IPV4_CTRL_LL_VAR(MsecCnt) +
                                            ((uint32)(IpV4_Ip_VCfgGetProbeMinMsec() + IpV4_Ip_VCfgGetProbeMaxMsec()) / 2);
            }
          }
          else
          {
            /* ARP request could not be sent, so try again later */
          }
        }
        else if(IPV4_CTRL_LL_VAR(ProbeState) == IPV4_IP_LL_PROBE_STATE_WAIT)
        {
          if(IPV4_CTRL_LL_VAR(WaitUntilTime) < IPV4_CTRL_LL_VAR(MsecCnt))
          {
            if(IPV4_CTRL_LL_VAR(ProbeNumSinceConf) == IpV4_Ip_VCfgGetProbeNum())
            {
              /* goto announce */
              IPV4_CTRL_LL_VAR(State)    = IPV4_IP_LL_STATE_ANNOUNCE;
              IPV4_CTRL_LL_VAR(AnnState) = IPV4_IP_LL_ANN_STATE_PROBE;

              /* "If, by ANNOUNCE_WAIT seconds after the transmission of the last ARP
               *  Probe no conflicting ARP Reply or ARP Probe has been received, then
               *  the host has successfully claimed the desired IPv4 Link-Local address."
               *                                           [RFC3927 2.2.1. Probe details]
               */

              /* valid addr found, so indicate it! */
              IPV4_CTRL_LL_VAR(AddrFound) = IPV4_CTRL_LL_VAR(NetAddrCand);
              IpV4_Ip_VIpAddrAssignmentReadyNotification(IpCtrlIdx, IPV4_IP_ADDR_ASSIGNMENT_METHOD_LINKLOCAL, TRUE);
            }
            else
            {
              /* next probe */
              IPV4_CTRL_LL_VAR(ProbeState) = IPV4_IP_LL_PROBE_STATE_PROBE;
            }
          }
          else
          {
            /* wait a little longer */
            /* do nothing */
          }
        }
        else
        {
          /* ERROR: invalid state */
        }
      }
    }
    break;

  case IPV4_IP_LL_STATE_CONFLICT:
    {
      IPV4_CTRL_LL_VAR(AnnounceNum)  = 0;       /* reset announcement number */
      IPV4_CTRL_LL_VAR(AddrConflict) = 0;       /* reset conflict marker */
      IPV4_CTRL_LL_VAR(ProbeNumSinceConf) = 0;  /* reset probe counter */
      IPV4_CTRL_LL_VAR(ConflictsNum)++;
      if(IpV4_Ip_VCfgGetMaxConflicts() > IPV4_CTRL_LL_VAR(ConflictsNum))
      {
        /* no delay, try next random network address */
        IPV4_CTRL_LL_VAR(WaitUntilTime) = IPV4_CTRL_LL_VAR(MsecCnt);  /* now (or in the next call cycle)! */
      }
      else
      {
        /* (too) many conflicts occurred, so delay further address tests */
        IPV4_CTRL_LL_VAR(WaitUntilTime) = IPV4_CTRL_LL_VAR(MsecCnt) + IpV4_Ip_VCfgGetRateLimitIntervalMsec();
      }
      IPV4_CTRL_LL_VAR(State) = IPV4_IP_LL_STATE_WAIT;

    }
    break;

  case IPV4_IP_LL_STATE_ANNOUNCE:
    {
      if(IPV4_CTRL_LL_VAR(AnnState) == IPV4_IP_LL_ANN_STATE_PROBE)
      {
        Std_ReturnType RetArp;

        /* send ARP announcement */
        /* copy old addr, send with new candidate addr, restore old addr */
        RetArp = IpV4_Arp_SendArpAnnouncement(IpCtrlIdx, IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr);

        if(E_OK == RetArp)
        {
          IPV4_CTRL_LL_VAR(AnnounceNum)++;
          IPV4_CTRL_LL_VAR(AnnState) = IPV4_IP_LL_ANN_STATE_WAIT;

          /* wait IPV4_IP_ANNOUNCE_INTERVAL_MS ms */
          IPV4_CTRL_LL_VAR(WaitUntilTime) = IPV4_CTRL_LL_VAR(MsecCnt) + IpV4_Ip_VCfgGetAnnounceIntervalMsec();
        }
      }
      else if(IPV4_CTRL_LL_VAR(AnnState) == IPV4_IP_LL_ANN_STATE_WAIT)
      {
        if(IPV4_CTRL_LL_VAR(WaitUntilTime) < IPV4_CTRL_LL_VAR(MsecCnt))
        {
          if(IPV4_CTRL_LL_VAR(AnnounceNum) >= IpV4_Ip_VCfgGetAnnounceNum())
          {
            /* goto ready */
            IPV4_CTRL_LL_VAR(State) = IPV4_IP_LL_STATE_READY;
          }
          else
          {
            /* next announcement */
            IPV4_CTRL_LL_VAR(AnnState) = IPV4_IP_LL_ANN_STATE_PROBE;
          }
        }
        else
        {
          /* wait a little longer */
          /* do nothing */
        }
      }
      else
      {
        /* Error: invalid state */
      }
    }
    break;

  case IPV4_IP_LL_STATE_READY:
    /* nothing to do. */
    break;
    
  default:
    /* Error: This state should not be reached. */
    TCPIP_ASSERT_UNREACHABLE();
    break;
  }

  IPV4_CTRL_LL_VAR(MsecCnt) += IpV4_VCfgGetMainFctPeriodMsec();
  return;
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

#endif /* IPV4_IP_DYN_LL_ADDR_CFG_SUPP */

/************************************************************************************************************
 *  IpV4_Ip_VForwardPacketToUpperLayer()
 ***********************************************************************************************************/
STATIC FUNC(void, IPV4_CODE) IpV4_Ip_VForwardPacketToUpperLayer(
  uint8                      IpCtrlIdx,
  IpV4_AddrIdType            IpAddrId,
  IPV4_P2C(IpV4_Ip_HdrType)  IpHdrPtr,
  IPV4_P2V(uint8)            IpPayloadPtr,
  uint16                     IpPayloadLen)
{
  IpBase_SockAddrInType IpRxSockAddrRem;

  IpRxSockAddrRem.sin_family   = IPBASE_AF_INET;
  IpRxSockAddrRem.sin_addr     = IpHdrPtr->SrcAddr;
  IpRxSockAddrRem.sin_port     = 0;  /* undefined */

#if (TCPIP_SUPPORT_DHCPV4 == STD_OFF)
  IPV4_UNUSED_IPCTRLIDX();           /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  switch (IpHdrPtr->Prot)
  {
    case IPV4_IP_PROTOCOL_UDP:
    {
#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
      boolean checksumValidated = IPV4_CTRL_VAR(TcpIp_IpV4_ChecksumOffloadingSupport).Udp;
#else
      boolean checksumValidated = FALSE;
#endif

#if (TCPIP_SUPPORT_DHCPV4 == STD_ON)
      {
        IpBase_SockAddrInType IpRxSockAddrLoc;

        IpRxSockAddrLoc.sin_family   = IPBASE_AF_INET;
        IpRxSockAddrLoc.sin_addr     = IpHdrPtr->DestAddr;
        IpRxSockAddrLoc.sin_port     = 0;  /* undefined */

        /* forward message to upper layer DHCP (in addition to forwarding it to UDP) */
        TcpIp_DhcpV4_RxIndication(IpCtrlIdx, IpPayloadPtr, IpPayloadLen, (IpBase_SockAddrType *)&IpRxSockAddrRem, (IpBase_SockAddrType *)&IpRxSockAddrLoc /* PRQA S 0310 */ /* MD_IpV4_11.04 */
# if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
          , checksumValidated /* boolean ChecksumCalculated */
# endif
        );
      }
#endif
      /* forward message to upper layer (UDP) */
      if (TcpIp_Udp_RxIndication(IPV4_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(IpAddrId), (IpBase_SockAddrType *)&IpRxSockAddrRem, IpPayloadPtr, IpPayloadLen, checksumValidated) == TCPIP_UDP_E_NO_MATCHING_SOCKET)  /* PRQA S 0310 */ /* MD_IpV4_11.04 */
      {
#if (IPV4_ICMP_ENABLE_DST_UNREACHABLE_MSG == STD_ON)
        if (!(IPV4_ADDR_IS_MULTICAST(IpHdrPtr->DestAddr) || IPV4_ADDR_IS_BROADCAST(IpHdrPtr->DestAddr)))
        {
          IpV4_Icmp_SendDestUnreachable(IpAddrId, IpHdrPtr, IPV4_ICMP_CODE_PORT_UNREACHABLE);
        }
#endif
      }
    }
    break;

#if (IPV4_SUPPORT_TCP == STD_ON)
    case IPV4_IP_PROTOCOL_TCP:
    {
      /* forward message only if IP address of the ECU is already configured */
      if(IPV4_IP_ONLINE_ACTIVE == IPV4_CTRL_VAR(IpV4_Ip_CtrlState))
      {
        /* forward message to upper layer (TCP) */
        TcpIp_Tcp_RxIndication(IPV4_ADDR_ID_2_TCPIP_LOCAL_ADDR_ID(IpAddrId), (IpBase_SockAddrType *)&IpRxSockAddrRem, IpPayloadPtr, IpPayloadLen /* PRQA S 0310 */ /* MD_IpV4_11.04 */
#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
          , IPV4_CTRL_VAR(TcpIp_IpV4_ChecksumOffloadingSupport).Tcp /* boolean ChecksumCalculated */
#endif
          );
      }
    }
    break;
#endif
  /* IPV4_SUPPORT_TCP */

#if (IPV4_SUPPORT_ICMP == STD_ON)
    case IPV4_IP_PROTOCOL_ICMP:
    {
      /* forward message to upper layer (ICMP) */
      IpV4_Icmp_RxIndication(IpAddrId, IpHdrPtr->SrcAddr, IpHdrPtr->Ttl, IpPayloadPtr, IpPayloadLen,
#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
        IPV4_CTRL_VAR(TcpIp_IpV4_ChecksumOffloadingSupport).Icmp /* boolean ChecksumCalculated */
#else
        FALSE
#endif
        );
    }
    break;
#endif
  /* IPV4_SUPPORT_ICMP == STD_ON */

    default:
    /* unsupported protocol -> discard message */
#if (IPV4_ICMP_ENABLE_DST_UNREACHABLE_MSG == STD_ON)
    if (!(IPV4_ADDR_IS_MULTICAST(IpHdrPtr->DestAddr) || IPV4_ADDR_IS_BROADCAST(IpHdrPtr->DestAddr)))
    {
      IpV4_Icmp_SendDestUnreachable(IpAddrId, IpHdrPtr, IPV4_ICMP_CODE_PROTOCOL_UNREACHABLE);
    }
#endif
    break;
  }
} /* IpV4_Ip_VForwardPacketToUpperLayer() */ /* PRQA S 6010, 6030, 6060, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */

#if (IPV4_IP_ENABLE_REASSEMBLY == STD_ON)
/************************************************************************************************************
 *  IpV4_Ip_VReassemblyInitBuffers()
 ***********************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Ip_VReassemblyInitBuffers(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV4_5.6 */
  uint8_least ReassemblyBufDescIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all IP instances/controllers. */
  for (IpCtrlIdx = 0; IpCtrlIdx < IpV4_VCfgGetMaxNumCtrls(); IpCtrlIdx++)
  {
    if (IpV4_Ip_VCfgHasFragmentationConfig())
    {
      /* #20 Iterate over all reassembly buffer descriptors. */
      for (ReassemblyBufDescIdx = 0; ReassemblyBufDescIdx < IpV4_Ip_VCfgGetReassemblyBufDescCnt(); ReassemblyBufDescIdx++)
      {
        IPV4_P2V(IpV4_Ip_ReassemblyBufferDescType) ReassemblyBufDescPtr = &IPV4_CTRL_VAR(IpV4_Ip_ReassemblyBufferDesc)[ReassemblyBufDescIdx];

        ReassemblyBufDescPtr->Status = IPV4_IP_REASSEMBLY_BUF_STATUS_UNUSED;

        /* #40 Assign reassembly buffers to the first descriptors.
         * (Configuration allows to have more descriptors than buffers in order to handle incomplete/failed
         *  packets without blocking a buffer) */
        if (ReassemblyBufDescIdx < IpV4_Ip_VCfgGetReassemblyBufCnt())
        {
          ReassemblyBufDescPtr->ReassemblyBufIdx = (uint8)ReassemblyBufDescIdx;
        }
        else
        {
          ReassemblyBufDescPtr->ReassemblyBufIdx = IPV4_IP_REASSEMBLY_BUF_IDX_INV;
        }
      }
    }
  }
} /* IpV4_Ip_VInitReassemblyBuffers() */

/************************************************************************************************************
 *  IpV4_Ip_VReassemblyFindBufferDesc()
 ***********************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
IPV4_LOCAL_INLINE FUNC(uint8, IPV4_CODE) IpV4_Ip_VReassemblyFindBufferDesc(
  uint8                     IpCtrlIdx,
  IPV4_P2C(IpV4_Ip_HdrType) ipHdrPtr,
  IPV4_P2V(uint8)           unusedDescIdxPtr,
  IPV4_P2V(uint8)           oldestDescIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least descIdx;
  boolean matchFound = FALSE;
  *unusedDescIdxPtr = IPV4_IP_REASSEMBLY_BUF_DESC_IDX_INV; /* will be used if no matching descriptor is found */
  *oldestDescIdxPtr = IPV4_IP_REASSEMBLY_BUF_DESC_IDX_INV; /* will be used if no matching descriptor and no free descriptor is found */

  /* ----- Implementation ----------------------------------------------- */
  for (descIdx = 0; descIdx < IpV4_Ip_VCfgGetReassemblyBufDescCnt(); descIdx++)
  {
    IPV4_P2V(IpV4_Ip_ReassemblyBufferDescType) ReassemblyBufDescPtr = &IPV4_CTRL_VAR(IpV4_Ip_ReassemblyBufferDesc)[descIdx];

    if (IPV4_IP_REASSEMBLY_BUF_STATUS_COMPLETE == ReassemblyBufDescPtr->Status)
    {
      /* Release reassembly buffer. */
      ReassemblyBufDescPtr->Status = IPV4_IP_REASSEMBLY_BUF_STATUS_UNUSED;
    }

    if (ReassemblyBufDescPtr->Status != IPV4_IP_REASSEMBLY_BUF_STATUS_UNUSED)
    {
      /* Buffers are exclusive for IP controller, so matching IpCtrlIdx is not necessary. */
      if ((ReassemblyBufDescPtr->MatchIpSrcAddr      == ipHdrPtr->SrcAddr) &&
          (ReassemblyBufDescPtr->MatchIpDstAddr      == ipHdrPtr->DestAddr) &&
          (ReassemblyBufDescPtr->MatchIpProtocol     == ipHdrPtr->Prot) &&
          (ReassemblyBufDescPtr->MatchIdentification == ipHdrPtr->Ident))
      {
        /* matching reassembly buffer found */
        matchFound = TRUE;
        break;
      }

      /* Record index of the oldest descriptor that has a buffer if no unused descriptor has been found yet. */
      if (IPV4_IP_REASSEMBLY_BUF_DESC_IDX_INV == *unusedDescIdxPtr)
      {
        if (IPV4_IP_REASSEMBLY_BUF_DESC_IDX_INV == *oldestDescIdxPtr)
        {
          *oldestDescIdxPtr = (uint8)descIdx;
        }
        else if (IPV4_UINT32_TIMESTAMP_IS_OLDER(ReassemblyBufDescPtr->TimeMs, IPV4_CTRL_VAR(IpV4_Ip_ReassemblyBufferDesc)[*oldestDescIdxPtr].TimeMs))
        {
          /* update oldest descriptor index if this descriptor is older than the previous stored */
          *oldestDescIdxPtr = (uint8)descIdx;
        }
        else
        {
          /* skip this descriptor. */
        }
      }
    }
    else if (IPV4_IP_REASSEMBLY_BUF_DESC_IDX_INV == *unusedDescIdxPtr)
    {
      *unusedDescIdxPtr = (uint8)descIdx;
    }
    else
    {
      /* skip this descriptor. */
    }
  }

  if (matchFound == FALSE)
  {
    descIdx = IPV4_IP_REASSEMBLY_BUF_DESC_IDX_INV;
  }

  return (uint8)descIdx;
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/************************************************************************************************************
 *  IpV4_Ip_VReassemblyGetBufferDesc()
 ***********************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
IPV4_LOCAL_INLINE FUNC(uint8, IPV4_CODE) IpV4_Ip_VReassemblyGetBufferDesc(
  uint8                     IpCtrlIdx,
  IPV4_P2C(IpV4_Ip_HdrType) ipHdrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 unusedDescIdx;
  uint8 oldestDescIdx;
  
  uint8 descIdx = IpV4_Ip_VReassemblyFindBufferDesc(IpCtrlIdx, ipHdrPtr, &unusedDescIdx, &oldestDescIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* Prepare unused assembly buffer descriptor if no matching reassembly buffer was found. */
  if (descIdx >= IpV4_Ip_VCfgGetReassemblyBufDescCnt())
  {
    /* no matching reassembly buffer descriptor was found. */

    if (IPV4_IP_REASSEMBLY_BUF_DESC_IDX_INV != unusedDescIdx)
    {
      descIdx = unusedDescIdx;
    }
    else
    {
      uint32 ageMs;

      TCPIP_ASSERT(IPV4_IP_REASSEMBLY_BUF_DESC_IDX_INV != oldestDescIdx); /* There must always be an oldest descriptor index. */

      ageMs = IpV4_Ip_TimeMs - IPV4_CTRL_VAR(IpV4_Ip_ReassemblyBufferDesc)[oldestDescIdx].TimeMs;

      if (ageMs >= IpV4_Ip_VCfgGetReassemblyTimeoutMs())
      {
        descIdx = oldestDescIdx;
      }
      else
      {
        descIdx = IPV4_IP_REASSEMBLY_BUF_DESC_IDX_INV;
      }
    }

    if (descIdx != IPV4_IP_REASSEMBLY_BUF_DESC_IDX_INV)
    {
      IPV4_P2V(IpV4_Ip_ReassemblyBufferDescType) reassemblyBufDescPtr = &IPV4_CTRL_VAR(IpV4_Ip_ReassemblyBufferDesc)[descIdx];

      uint8_least  reassemblyBufIdx = reassemblyBufDescPtr->ReassemblyBufIdx;  /* PRQA S 0781 */ /* MD_IpV4_5.6 */
      uint16_least firstGapOfs      = IPV4_IP_REASSEMBLY_BUF_NO_NEXT_GAP_OFS;
      uint16_least firstGapSize     = IpV4_Ip_VCfgGetReassemblyBufSize() - IPV4_IP_REASSEMBLY_MAX_HEADER_LEN_BYTE;

      reassemblyBufDescPtr->MatchIpCtrlIdx       = IpCtrlIdx;
      reassemblyBufDescPtr->MatchIpSrcAddr       = ipHdrPtr->SrcAddr;
      reassemblyBufDescPtr->MatchIpDstAddr       = ipHdrPtr->DestAddr;
      reassemblyBufDescPtr->MatchIpProtocol      = ipHdrPtr->Prot;
      reassemblyBufDescPtr->MatchIdentification  = ipHdrPtr->Ident;
      reassemblyBufDescPtr->NextFragOfs          = IPV4_IP_REASSEMBLY_MAX_HEADER_LEN_BYTE;

      IPBASE_PUT_UINT16(&IPV4_CTRL_VAR(IpV4_Ip_ReassemblyBuffers)[reassemblyBufIdx * IpV4_Ip_VCfgGetReassemblyBufSize()], IPV4_IP_REASSEMBLY_MAX_HEADER_LEN_BYTE,                                       firstGapOfs);
      IPBASE_PUT_UINT16(&IPV4_CTRL_VAR(IpV4_Ip_ReassemblyBuffers)[reassemblyBufIdx * IpV4_Ip_VCfgGetReassemblyBufSize()], IPV4_IP_REASSEMBLY_MAX_HEADER_LEN_BYTE + IPV4_IP_REASSEMBLY_BUF_GAP_SIZE_OFS, firstGapSize);

      reassemblyBufDescPtr->TimeMs               = IpV4_Ip_TimeMs;
      reassemblyBufDescPtr->Status               = IPV4_IP_REASSEMBLY_BUF_STATUS_IN_PROGRESS;
      reassemblyBufDescPtr->Flags                = 0;
      reassemblyBufDescPtr->FragmentLen          = 0;
      reassemblyBufDescPtr->FragmentsDropped     = 0;
      reassemblyBufDescPtr->FragmentsReceived    = 0;
    }
    /* else: RESOURCE LIMIT: no free descriptor available */
  }

  return descIdx;
} /* IpV4_Ip_VGetReassemblyBufferDesc() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/************************************************************************************************************
 *  IpV4_Ip_VReassemblyGetBufferPtr()
 ***********************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
IPV4_LOCAL_INLINE FUNC_P2VAR(uint8, AUTOMATIC, IPV4_CODE) IpV4_Ip_VReassemblyGetBufferPtr(
  IPV4_P2C(IpV4_Ip_ReassemblyBufferDescType) reassemblyBufDescPtr)
{
  uint8 IpCtrlIdx = reassemblyBufDescPtr->MatchIpCtrlIdx;  /* PRQA S 0781 */ /* MD_IpV4_5.6 */

  TCPIP_ASSERT(reassemblyBufDescPtr->ReassemblyBufIdx != IPV4_IP_REASSEMBLY_BUF_IDX_INV);

  return &IPV4_CTRL_VAR(IpV4_Ip_ReassemblyBuffers)[reassemblyBufDescPtr->ReassemblyBufIdx * IpV4_Ip_VCfgGetReassemblyBufSize()];
}

/************************************************************************************************************
 *  IpV4_Ip_VReassemblyAdaptGap()
 ***********************************************************************************************************/
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
 *
 */
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Ip_VReassemblyAdaptGap(
  IPV4_P2V(IpV4_Ip_ReassemblyBufferDescType) reassemblyBufDescPtr,
  uint16                                     fragOfs,
  uint16                                     fragEndOfs,
  uint16                                     thisGapOfs,
  uint16                                     thisGapEndOfs,
  uint16                                     prevGapOfs,
  uint16                                     nextGapOfs)
{
  /* ----- Local Variables ---------------------------------------------- */
  IPV4_P2V(uint8) reassemblyBufPtr = IpV4_Ip_VReassemblyGetBufferPtr(reassemblyBufDescPtr);

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT_MSG((thisGapOfs <= fragOfs) && (fragEndOfs <= thisGapEndOfs), "Fragment must fit into gap.");

  if (fragOfs == thisGapOfs)
  {
    if (fragEndOfs == thisGapEndOfs)
    {
      /* |######| Case 1: fragment complete fits into gap. */

      /* #15 If there is a previous gap, set next pointer to following gap (or INV). */
      if (IPV4_IP_REASSEMBLY_BUF_NO_NEXT_GAP_OFS != prevGapOfs)
      {
        IPBASE_PUT_UINT16(reassemblyBufPtr, prevGapOfs, nextGapOfs);
      }
      else
      {
        /* set first gap pointer to next gap */
        reassemblyBufDescPtr->NextFragOfs = nextGapOfs;
      }
    }
    else
    {
      /* #20 |####__| Case 2: fragment starts at beginning of gap but does not completely fill the gap. */

      /* #25 Create new gap (old one will be overwritten). */
      uint16 newGapOfs  = fragEndOfs;
      IPBASE_PUT_UINT16(reassemblyBufPtr, newGapOfs + IPV4_IP_REASSEMBLY_BUF_GAP_SIZE_OFS, (thisGapEndOfs - fragEndOfs));

      /* If there is a previous gap, set next pointer to new gap */
      if (IPV4_IP_REASSEMBLY_BUF_NO_NEXT_GAP_OFS != prevGapOfs)
      {
        IPBASE_PUT_UINT16(reassemblyBufPtr, prevGapOfs, newGapOfs);
      }
      else
      {
        reassemblyBufDescPtr->NextFragOfs = newGapOfs;
      }

      /* Set next pointer of new gap to next gap (or INV) */
      IPBASE_PUT_UINT16(reassemblyBufPtr, newGapOfs, nextGapOfs);
    }
  }
  else
  {
    if (fragEndOfs == thisGapEndOfs)
    {
      /* #30 |__####| Case 3: fragment ends at end of gap but does not start at beginning of gap. */

      /* #35 Decrease size of this gap. */
      IPBASE_PUT_UINT16(reassemblyBufPtr, thisGapOfs + IPV4_IP_REASSEMBLY_BUF_GAP_SIZE_OFS, (fragOfs - thisGapOfs));
    }
    else
    {
      /* #40 |__##__| Case 4: fragment fills some part in the middle of the gap but there will remain a gap before and after the fragment. */
      uint16 newGapOfs;

      /* #41 Decrease size of this gap. */
      IPBASE_PUT_UINT16(reassemblyBufPtr, thisGapOfs + IPV4_IP_REASSEMBLY_BUF_GAP_SIZE_OFS, (fragOfs - thisGapOfs));

      /* #43 Create new gap. */
      newGapOfs = fragEndOfs;
      IPBASE_PUT_UINT16(reassemblyBufPtr, newGapOfs + IPV4_IP_REASSEMBLY_BUF_GAP_SIZE_OFS, (thisGapEndOfs - fragEndOfs));

      /* Set next pointer of this gap to new gap */
      IPBASE_PUT_UINT16(reassemblyBufPtr, thisGapOfs, newGapOfs);

      /* Set next pointer of new gap to next gap (or INV) */
      IPBASE_PUT_UINT16(reassemblyBufPtr, newGapOfs, nextGapOfs);
    }
  }
}  /* PRQA S 6030, 6060 */ /* MD_MSR_STCYC, MD_MSR_STPAR */

/************************************************************************************************************
 *  IpV4_Ip_VReassemblyHandleGaps()
 ***********************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
IPV4_LOCAL_INLINE FUNC(Std_ReturnType, IPV4_CODE) IpV4_Ip_VReassemblyHandleGaps(
  IPV4_P2V(IpV4_Ip_ReassemblyBufferDescType) reassemblyBufDescPtr,
  uint16                                     fragOfs,
  uint16                                     fragLen,
  boolean                                    fragIsLast,
  IPV4_P2V(uint16)                           gapOfsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal            = E_NOT_OK;
  boolean        searchMatchingGap = TRUE;
  uint16         gapOfs            = reassemblyBufDescPtr->NextFragOfs;
  uint16         prevGapOfs        = IPV4_IP_REASSEMBLY_BUF_NO_NEXT_GAP_OFS;
  uint16         fragEndOfs        = (fragOfs + fragLen);
  IPV4_P2V(uint8) reassemblyBufPtr = IpV4_Ip_VReassemblyGetBufferPtr(reassemblyBufDescPtr);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over linked list of payload gaps until a a gap is found the fragment fits into. */

  /* payload gaps are non overlapping and stored in ascending order.
   * all gaps but the last gap will always have a size of at least 8 bytes,
   * because all fragments except for the last fragment must be a multiple of 8 bytes long.
   */
  while ((TRUE == searchMatchingGap) && (gapOfs != IPV4_IP_REASSEMBLY_BUF_NO_NEXT_GAP_OFS))
  {
    uint16 nextGapOfs;
    uint16 gapSize;
    uint16 gapEndOfs;

    TCPIP_ASSERT((gapOfs % IPV4_IP_FRAGMENT_BLOCK_SIZE) == 0);

    IPBASE_GET_UINT16(reassemblyBufPtr, gapOfs, nextGapOfs);
    TCPIP_ASSERT((nextGapOfs == IPV4_IP_REASSEMBLY_BUF_NO_NEXT_GAP_OFS) || ((nextGapOfs % IPV4_IP_FRAGMENT_BLOCK_SIZE) == 0));

    IPBASE_GET_UINT16(reassemblyBufPtr, gapOfs + IPV4_IP_REASSEMBLY_BUF_GAP_SIZE_OFS, gapSize);
    TCPIP_ASSERT(gapSize > 0);

    gapEndOfs = (gapOfs + gapSize);

    if (fragOfs < gapOfs)
    {
      /* INVALID FRAGMENT: Fragment starts before this gap.
        * Since all following gaps will have bigger GapOfs, they do not need to be checked.
        * This is an invalid, overlapping or already received fragment.
        * -> Ignore this fragment.
        */
      searchMatchingGap = FALSE;
    }
    else if (fragEndOfs > gapEndOfs)
    {
      /* Fragments ends after end of this gap.
        * Check if fragment fits into next gap.
        */
    }
    else
    { /* PRQA S 0715 ControlStructNesting */ /* MD_MSR_1.1_715 */
      /* OK: Fragment fits (partially or fully) into this gap */

      if (TRUE == fragIsLast)
      {
        TCPIP_ASSERT(nextGapOfs == IPV4_IP_REASSEMBLY_BUF_NO_NEXT_GAP_OFS);

        if (fragEndOfs < gapEndOfs)
        {
          /* shrink gap to it ends after last fragment */
          gapSize   = fragLen;
          gapEndOfs = fragEndOfs;
          IPBASE_PUT_UINT16(reassemblyBufPtr, gapOfs + IPV4_IP_REASSEMBLY_BUF_GAP_SIZE_OFS, gapSize);
        }

        reassemblyBufDescPtr->FragmentLen = gapEndOfs;
      }

      /* #20 Adapt gap, if matching gap was found. */
      IpV4_Ip_VReassemblyAdaptGap(reassemblyBufDescPtr, fragOfs, fragEndOfs, gapOfs, gapEndOfs, prevGapOfs, nextGapOfs);

      retVal = E_OK;
      break;
    } /* PRQA L:ControlStructNesting */ /* MD_MSR_1.1_715 */

    prevGapOfs = gapOfs;

    /* move to next gap */
    gapOfs = nextGapOfs;
  } /* while(more gaps) */

  *gapOfsPtr = gapOfs;

  return retVal;
}  /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */

/************************************************************************************************************
 *  IpV4_Ip_VReassemblyRxFragment()
 ***********************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Ip_VReassemblyRxFragment(
  IPV4_P2V(IpV4_Ip_ReassemblyBufferDescType) reassemblyBufDescPtr,
  uint16                                     fragAbsByteOfs,
  IPV4_P2C(uint8)                            fragPayloadPtr,
  uint16                                     fragPayloadLen,
  boolean                                    fragIsLast)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16          gapOfs;
  IPV4_P2V(uint8) reassemblyBufPtr = IpV4_Ip_VReassemblyGetBufferPtr(reassemblyBufDescPtr);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if fragments fits into one of the existing gap inside the reassembly buffer and update gap list. */
  if (IpV4_Ip_VReassemblyHandleGaps(reassemblyBufDescPtr, fragAbsByteOfs, fragPayloadLen, fragIsLast, &gapOfs) == E_OK)
  {
    /* #20 Copy payload of fragment into the reassemble buffer. */
    IpBase_Copy((IpBase_CopyDataType*)&reassemblyBufPtr[fragAbsByteOfs], (const IpBase_CopyDataType*)&(fragPayloadPtr)[0], fragPayloadLen);  /* PRQA S 0310 */ /* MD_IpV4_11.04 */
  }

  /* #30 Check if fragmentation is complete, has failed or if more fragments are expected. */
  if (reassemblyBufDescPtr->NextFragOfs == IPV4_IP_REASSEMBLY_BUF_NO_NEXT_GAP_OFS)
  {
    if (0 != (reassemblyBufDescPtr->Flags & IPV4_IP_REASSEMBLY_FLAG_LAST_FRAG_RECEIVED))
    {
      /* no more gaps... all fragments received
        *  - Set reassembly status of this buffer descriptor to complete.
        */
      reassemblyBufDescPtr->Status = IPV4_IP_REASSEMBLY_BUF_STATUS_COMPLETE;
    }
    else
    {
      /* RESOURCE LIMIT: Reassembly buffer is full before last fragment has been received.
        * -> Abort reassembly
        */

      reassemblyBufDescPtr->FragmentsDropped++;
      reassemblyBufDescPtr->Status = IPV4_IP_REASSEMBLY_BUF_STATUS_FAILED;
    }
  }
  else if (gapOfs == IPV4_IP_REASSEMBLY_BUF_NO_NEXT_GAP_OFS)
  {
    /* RESOURCE LIMIT: fragment does not fit into any gap. Assembly buffer too small.
      * -> Abort reassembly
      */

    reassemblyBufDescPtr->FragmentsDropped++;
    reassemblyBufDescPtr->Status = IPV4_IP_REASSEMBLY_BUF_STATUS_FAILED;
    /* Descriptor will be released after the reassembly timeout has expired */
  }
  else
  {
    /* there are still gaps in the buffer. Wait for further fragments... */
  }
}

/************************************************************************************************************
 *  IpV4_Ip_VReassemblyRxFragmentInOrder()
 ***********************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Ip_VReassemblyRxFragmentInOrder(
  uint8                                      IpCtrlIdx,
  IPV4_P2V(IpV4_Ip_ReassemblyBufferDescType) reassemblyBufDescPtr,
  uint16                                     fragAbsByteOfs,
  IPV4_P2C(uint8)                            fragPayloadPtr,
  uint16                                     fragPayloadLen,
  boolean                                    fragIsLast)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Check is received fragment fits directly after the previously received fragment. */
  if (fragAbsByteOfs == reassemblyBufDescPtr->NextFragOfs)
  {
    /* fragment fits exactly after the previous fragment */
    uint16 RemainingBufLen = (IpV4_Ip_VCfgGetReassemblyBufSize() - fragAbsByteOfs);

    if (fragPayloadLen > RemainingBufLen)
    {
      /* RESOURCE LIMIT: Assembly buffer too small.
        * -> Abort reassembly
        */
      reassemblyBufDescPtr->FragmentsDropped++;
      reassemblyBufDescPtr->Status = IPV4_IP_REASSEMBLY_BUF_STATUS_UNUSED;
      /* We do not expect segments out-of-order. So free reassembly buffer immediately. */
    }
    else
    {
      IPV4_P2V(uint8) reassemblyBufPtr = IpV4_Ip_VReassemblyGetBufferPtr(reassemblyBufDescPtr);

      /* Copy fragment payload. */
      IpBase_Copy((IpBase_CopyDataType*)&reassemblyBufPtr[fragAbsByteOfs], (const IpBase_CopyDataType*)&(fragPayloadPtr)[0], fragPayloadLen);  /* PRQA S 0310 */ /* MD_IpV4_11.04 */

      reassemblyBufDescPtr->NextFragOfs = fragAbsByteOfs + fragPayloadLen;

      /* Update reassembly status to COMPLETE, if this was the last fragment. */
      if (TRUE == fragIsLast)
      {
        reassemblyBufDescPtr->Status = IPV4_IP_REASSEMBLY_BUF_STATUS_COMPLETE;
        reassemblyBufDescPtr->FragmentLen = reassemblyBufDescPtr->NextFragOfs;
      }
    }
  }
  else
  {
    /* Unexpected fragment offset. Out of order reassembly not supported.
      * -> Abort fragment reassembly
      */
    reassemblyBufDescPtr->FragmentsDropped++;
    reassemblyBufDescPtr->Status = IPV4_IP_REASSEMBLY_BUF_STATUS_UNUSED;
    /* We do not expect segments out-of-order. So free reassembly buffer immediately. */
  }
}  /* PRQA S 6060 */ /* MD_MSR_STPAR */

/************************************************************************************************************
 *  IpV4_Ip_VReassemblyCheckFragment()
 ***********************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
IPV4_LOCAL_INLINE FUNC(boolean, IPV4_CODE) IpV4_Ip_VReassemblyCheckFragment(
  IPV4_P2V(IpV4_Ip_ReassemblyBufferDescType) reassemblyBufDescPtr,
  uint16                                     fragBlockOfs,
  boolean                                    fragIsLast)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean acceptFragment = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set first fragment received flag, if this is the first fragment and the flag is not already set.
   *     Return FALSE, if a first fragment was received before. */
  if (fragBlockOfs == 0)
  {
    /* this is the first fragment. Note: it may arrive after other fragments. */

    if ((reassemblyBufDescPtr->Flags & IPV4_IP_REASSEMBLY_FLAG_FIRST_FRAG_RECEIVED) == 0)
    {
      reassemblyBufDescPtr->Flags |= IPV4_IP_REASSEMBLY_FLAG_FIRST_FRAG_RECEIVED;
    }
    else
    {
      /* INVALID FRAGMENT: received first fragment twice!
        * -> Ignore this fragment
        */
      acceptFragment = FALSE;
    }
  }

  /* #20 Set last fragment received flag, if this is the last fragment and the flag is not already set.
   *     Return FALSE, if a last fragment was received before. */
  if (acceptFragment == TRUE)
  {
    if (fragIsLast == TRUE)
    {
      if ((reassemblyBufDescPtr->Flags & IPV4_IP_REASSEMBLY_FLAG_LAST_FRAG_RECEIVED) == 0)
      {
        reassemblyBufDescPtr->Flags |= IPV4_IP_REASSEMBLY_FLAG_LAST_FRAG_RECEIVED;
      }
      else
      {
        /* INVALID FRAGMENT: received last fragment twice!
          * -> Ignore this fragment
          */
        acceptFragment = FALSE;
      }
    }
  }

  return acceptFragment;
}

/************************************************************************************************************
 *  IpV4_Ip_VReassemblyRxIndication()
 ***********************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
IPV4_LOCAL_INLINE FUNC(boolean, IPV4_CODE) IpV4_Ip_VReassemblyRxIndication(
  uint8                     IpCtrlIdx,
  IPV4_P2C(IpV4_Ip_HdrType) ipHdrPtr,
  IPV4_P2V(uint8*)          fragPayloadPtrPtr,
  IPV4_P2V(uint16)          fragPayloadLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean     reassemblyComplete = FALSE;
  uint8_least raBufDescIdx;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IPV4_COND_VALID_CTRLIDX(IpCtrlIdx));
  TCPIP_ASSERT(ipHdrPtr != NULL_PTR);
  
  TCPIP_ASSERT(fragPayloadPtrPtr != NULL_PTR);
  TCPIP_ASSERT(fragPayloadPtrPtr[0] != NULL_PTR);

  TCPIP_ASSERT(fragPayloadLenPtr != NULL_PTR);
  TCPIP_ASSERT(fragPayloadLenPtr[0] != 0);
  
  /* #10 Get (or create) reassembly buffer descriptor for this IP packet. */
  raBufDescIdx = IpV4_Ip_VReassemblyGetBufferDesc(IpCtrlIdx, ipHdrPtr);

  if (raBufDescIdx == IPV4_IP_REASSEMBLY_BUF_DESC_IDX_INV)
  {
    /* RESOURCE LIMIT: No free reassembly buffer and descriptor
      * -> Do not start reassembly.
      */
  }
  else
  {
    IpV4_Ip_ReassemblyBufferDescType *reassemblyBufDescPtr = &IPV4_CTRL_VAR(IpV4_Ip_ReassemblyBufferDesc)[raBufDescIdx];
    uint16                            flagsOffsetHbo       = IPV4_NTOHS(ipHdrPtr->Flags_Offset);
    uint16                            fragBlockOfs   = (uint16)(flagsOffsetHbo & IPV4_IP_FRAGMENT_OFFSET_MASK);
    boolean                           fragIsLast     = (boolean)((flagsOffsetHbo & IPV4_IP_FLAGS_MORE_FRAGMENTS) == 0);
    boolean                           acceptFragment;

    reassemblyBufDescPtr->FragmentsReceived++;

    TCPIP_ASSERT(IPV4_IP_REASSEMBLY_BUF_STATUS_UNUSED != reassemblyBufDescPtr->Status);

    /* #20 Check status of the descriptor and validate the fragment. */
    if (reassemblyBufDescPtr->Status != IPV4_IP_REASSEMBLY_BUF_STATUS_IN_PROGRESS)
    {
      /* Reassembly of this packet has already failed or is complete.
       * -> Ignore this fragment
       */
      acceptFragment = FALSE;
    }
    else
    {
      acceptFragment = IpV4_Ip_VReassemblyCheckFragment(reassemblyBufDescPtr, fragBlockOfs, fragIsLast);
    }

    if (acceptFragment == TRUE)
    {
      if ((fragIsLast == TRUE) || (((*fragPayloadLenPtr) % IPV4_IP_FRAGMENT_BLOCK_SIZE) == 0))
      {
        uint16 fragAbsByteOfs = (fragBlockOfs * IPV4_IP_FRAGMENT_BLOCK_SIZE)  + IPV4_IP_REASSEMBLY_MAX_HEADER_LEN_BYTE;

        TCPIP_ASSERT(reassemblyBufDescPtr->ReassemblyBufIdx < IpV4_Ip_VCfgGetReassemblyBufCnt());

        /* #30 Copy payload of the fragment into the reassembly buffer, if the fragment fits into a gap inside the buffer. */
# if (IPV4_IP_ENABLE_REASSEMBLY_OOO == STD_ON)
        if (FALSE == IpV4_Ip_VCfgIsReassemblyInOrderEnabled())
        {
          IpV4_Ip_VReassemblyRxFragment(reassemblyBufDescPtr, fragAbsByteOfs, *fragPayloadPtrPtr, *fragPayloadLenPtr, fragIsLast);
        }
        else
# endif
        {
          IpV4_Ip_VReassemblyRxFragmentInOrder(IpCtrlIdx, reassemblyBufDescPtr, fragAbsByteOfs, *fragPayloadPtrPtr, *fragPayloadLenPtr, fragIsLast);
        }

        /* #40 Update *fragPayloadPtrPtr and *fragPayloadLenPtr is reassembly is complete. */
        if (reassemblyBufDescPtr->Status == IPV4_IP_REASSEMBLY_BUF_STATUS_COMPLETE)
        {
          IPV4_P2V(uint8) reassemblyBufPtr = IpV4_Ip_VReassemblyGetBufferPtr(reassemblyBufDescPtr);

          (*fragPayloadPtrPtr) = &reassemblyBufPtr[IPV4_IP_REASSEMBLY_MAX_HEADER_LEN_BYTE];
          (*fragPayloadLenPtr) =  reassemblyBufDescPtr->FragmentLen - IPV4_IP_REASSEMBLY_MAX_HEADER_LEN_BYTE;
        }
      }
      else
      {
        /* INVALID FRAGMENT: Only the last fragment may have a length that is not a multiple of 8 bytes.
         * -> Abort Reassembly of this packet.
         */

        reassemblyBufDescPtr->Status = IPV4_IP_REASSEMBLY_BUF_STATUS_FAILED;
      }
    }

    reassemblyComplete = (boolean)(reassemblyBufDescPtr->Status == IPV4_IP_REASSEMBLY_BUF_STATUS_COMPLETE);
  }

  return reassemblyComplete;
} /* IpV4_Ip_VReassemblyRxIndication() */ /* PRQA S 6010, 6030, 6080, 2006 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF, MD_MSR_14.7 */
#endif

#define IPV4_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/* module specific MISRA deviations:
   MD_IPV4_1.1_0857:
     Reason:     Number of macro definitions is huge but improves readability. (rule 0857)
     Risk:       Some compilers might have a problem with too many macro definitions.
     Prevention: Use only compilers supporting more than 1024 macro definitions.
   MD_IPV4_5.6_0781:
     Reason:     Readability is increased by using well known names. (rule 0781)
     Risk:       none
     Prevention: not applicable
   MD_IPV4_6.3_5013:
     Reason:     Usage of basic types is defined in SWS_SoAd
     Risk:       problem with different platforms
     Prevention: not possible
   MD_IPV4_11.4_0310:
     Reason:     Pointer casts are standard proceeding in TcpIp data handling. (rule 0310)
     Risk:       none
     Prevention: not applicable
   MD_IPV4_16.7_3673:
     Reason:     Reworking the code to use 'const' pointers more often would cause much effort.
     Risk:       none
     Prevention: not applicable
   MD_IPV4_0771:
     Reason:     Restructuring the code so that only one break statement is used would create complex conditions that
                 are hard to understand and reduce runtime performance.
     Risk:       Code may become unstructured.
     Prevention: Covered by code review.
*/

#endif  /* (TCPIP_SUPPORT_IPV4 == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: Ip.c
 *********************************************************************************************************************/
