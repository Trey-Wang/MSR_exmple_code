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
 *         \file  TcpIp_Arp.c
 *        \brief  Implementation of Address Resolution Protocol (ARP) for IPv4
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

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/* The ARP module is an integral part of the IP module. For this the ARP module may diectly access
   the IP configuration. */

#define TCPIP_ARP_SOURCE

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "TcpIp_Arp.h"
#include "TcpIp_Arp_Cbk.h"
#include "TcpIp_IpV4_Priv.h"
#include "TcpIp_IpV4_Lcfg.h"
#include "TcpIp_Priv.h"

#include "EthIf.h"

#include "TcpIp_IpV4.h"  /* needed for dynamic IP LL config */
#include "TcpIp_IpV4_XCfg.h"

#if (IPV4_SUPPORT_ICMP == STD_ON)
#include "TcpIp_IcmpV4_Cbk.h"
#endif

#if (IPV4_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

#if !defined (STATIC)
# define STATIC static
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define IPV4_ARP_TIMESTAMP_SEC_MAX  (0xFFFFFFFFu)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
#define IPV4_ARP_TIMESTAMP_IS_OLDER(TS_A, TS_B) IPV4_UINT32_TIMESTAMP_IS_OLDER((TS_A), (TS_B))

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef struct
{
  uint16 HwAddrType;
  uint16 ProtAddrType;
  uint8 HwAddrSize;
  uint8 ProtAddrSize;
  uint16 Operation;
  IpBase_EthPhysAddrType SrcMacAddr;
  uint16 SrcIpAddrHigh;
  uint16 SrcIpAddrLow;
  IpBase_EthPhysAddrType DestMacAddr;
  uint16 DestIpAddrHigh;
  uint16 DestIpAddrLow;
} IpV4_Arp_Packet4Type;

#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
typedef uint16 IpV4_Arp_TimestampMsecType;
typedef uint32 IpV4_Arp_TimestampSecType;
#endif

/**********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
/* ----- */
#define IPV4_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
STATIC VAR(IpV4_Arp_TimestampSecType, IPV4_VAR_NOINIT)  IpV4_Arp_TimestampSec;
STATIC VAR(IpV4_Arp_TimestampMsecType, IPV4_VAR_NOINIT) IpV4_Arp_TimestampMsec;
#endif

#define IPV4_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/* ----- */
#define IPV4_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

#define IPV4_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/* ----- */
#define IPV4_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
STATIC VAR(uint8, IPV4_VAR_NOINIT)  IpV4_Arp_NumPendEle;  /* number of pending ARP requests */
#endif

#define IPV4_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/* ----- */
#define IPV4_START_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

STATIC CONST(IpBase_EthPhysAddrType, IPV4_CONST) IpV4_Arp_PhysBroadcastAddr =
{
  0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU
};

#define IPV4_STOP_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define IPV4_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */
/***********************************************************************************************************************
 *  IpV4_Arp_VSendMessage
 **********************************************************************************************************************/
/*! \brief        Sends an ARP message.
 *  \details      -
 *  \param[in]    IpCtrlIdx             Index of the IP controller/instance.
 *  \param[in]    RemNetAddr            Remote Network address(Destination Ip address).
 *  \param[in]    LocNetAddr            Local Network address(Source Ip address).
 *  \param[in]    RemPhysAddrPtr        Remote Physical address(Pointer to the destination MAC address).
 *  \param[in]    Operation             Type of operation (Reply or Request).
 *  \return       E_OK                  ARP message sent successfully.
 *  \return       E_NOT_OK              ARP message could not be sent.
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
  **********************************************************************************************************************/
IPV4_LOCAL FUNC(Std_ReturnType, IPV4_CODE) IpV4_Arp_VSendMessage(
  uint8                   IpCtrlIdx,
  IpBase_AddrInType       RemNetAddr,
  IpBase_AddrInType       LocNetAddr,
  IPV4_P2C(uint8)         RemPhysAddrPtr,
  uint16                  Operation );

#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Arp_VUpdateEntry
 **********************************************************************************************************************/
/*! \brief          Sets the state of an entry in the dynamic ARP table to valid and notifies the upper layer if required.
 *  \details        -
 *  \param[in]      IpCtrlIdx                Index of the IP controller/instance.
 *  \param[in]      PhysAddrPtr              Pointer for physical address (network byte order).
 *  \param[in]      TableIdx                 Entry Index in the ARP resolution table.
 *  \param[in]      EntryInTable             Indicates if an existing entry is updated.
 *  \pre            PRE_TCPIP_ARP_INIT
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
  **********************************************************************************************************************/
IPV4_LOCAL FUNC(void, IPV4_CODE) IpV4_Arp_VUpdateEntry(
  uint8           IpCtrlIdx,
  IPV4_P2C(uint8) PhysAddrPtr,
  uint8           TableIdx,
  boolean         EntryInTable); 

/***********************************************************************************************************************
 *  IpV4_Arp_VStoreDynamicEntry
 **********************************************************************************************************************/
/*! \brief        Stores an entry into the dynamic ARP table.
 *  \details      -
 *  \param[in]    IpCtrlIdx                Index of the IP controller/instance
 *  \param[in]    NetAddr                  Network address.
 *  \param[in]    PhysAddrPtr              Pointer to physical address (network byte order).
 *  \param[in]    AddIfNotExisting TRUE:   Add a new entry if no matching entry is found.
 *                                 FALSE:  Leave ARP table unchanged if no matching entry is found.

 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
  **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Arp_VStoreDynamicEntry(
  uint8                                      IpCtrlIdx,
  IpBase_AddrInType                          NetAddr,
  P2CONST(uint8, IPV4_APPL_DATA, IPV4_CONST) PhysAddrPtr,
  boolean                                    AddIfNotExisting);

/***********************************************************************************************************************
 *  IpV4_Arp_VPhysAddrTableChgNotification
 **********************************************************************************************************************/
/*! \brief        Notifies upper layer about a change in the ARP table related to controller, specified by IpCtrlIdx.
 *  \details      -
 *  \param[in]    IpCtrlIdx     IP controller index of the related ARP table.
 *  \param[in]    ArpTableIdx   ARP table Index.
 *  \param[in]    Valid         Specifies if the ARP table entry is added or changed (TRUE) or has been removed (FALSE).
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Arp_VPhysAddrTableChgNotification(
  uint8             IpCtrlIdx,
  uint8             ArpTableIdx,
  boolean           Valid);

# if (IPV4_ARP_SUPPORT_DISCARD_HANDLING == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Arp_VPhysAddrTableChgDiscardedNotification
 **********************************************************************************************************************/
/*! \brief        Notifies ULs about a new ARP-table entry that was discarded because the ARP-table was full, already.
 *  \details      Called only in case the functionality defined by DSGN-TcpIpIpV4AddressResolutionModes is active, i.e.:
 *                IPV4_ARP_SUPPORT_DISCARD_HANDLING == STD_ON
 *                Does nothing if no callback for this event is configured.
 *  \param[in]    IpCtrlIdx                IP controller index of the related ARP table.
 *  \param[in]    DiscardedNetAddr         Specifies the IP address of the discarded ARP table entry
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-TcpIpIpV4AddressResolutionModes
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Arp_VPhysAddrTableChgDiscardedNotification(
  uint8                                       IpCtrlIdx,
  IpBase_AddrInType                           DiscardedNetAddr);
# endif
#endif /* (IPV4_ARP_DYNAMIC_TABLES == STD_ON) */

/***********************************************************************************************************************
 *  IpV4_Arp_VStaticTableLookup
 **********************************************************************************************************************/
/*! \brief        Lookup static configured IP address to physical address mapping.
 *  \details      This function returns the physical address for the configured IP address for the given IP controller.
 *  \param[in]    IpCtrlIdx             Index of the IP controller/instance
 *  \param[in]    NetAddr               Network address
 *  \param[out]   PhysAddrPtr           Pointer to physical address (network byte order).
 *                                      (May be NULL_PTR in case it shall only be checked if an entry exists.)
 *  \return       E_OK                  Static mapping has been found. *PhysAddrPtr has been updated if != NULL_PTR
 *  \return       E_NOT_OK              No static mapping has been found. *PhysAddrPtr unchanged
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
#if (IPV4_ARP_STATIC_TABLES == STD_ON)
IPV4_LOCAL_INLINE FUNC(Std_ReturnType, IPV4_CODE) IpV4_Arp_VStaticTableLookup(
  uint8                                    IpCtrlIdx,
  IpBase_AddrInType                        NetAddr,
  P2VAR(uint8, IPV4_APPL_DATA, IPV4_CONST) PhysAddrPtr);
#endif

#if (IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Arp_VDetectLinkLocalAddressConflict
 **********************************************************************************************************************/
/*! \brief        Notifies the link-local address configuration module about IP addresses used in received ARP packets.
 *  \details      The link-local address configuration module uses this information to detect if the IP address of this
 *                node conflicts with the address of other nodes.
 *  \param[in]    IpCtrlIdx        Index of the IP controller/instance.
 *  \param[in]    ArpOperation     Operation of the ARP packet. (IPV4_ARP_OP_REQUEST or IPV4_ARP_OP_REPLY)
 *  \param[in]    LocIpAddr        Unicast IP address used by the IP controller/instance.
 *  \param[in]    ArpSrcIpAddr     Source IP address contained in the ARP packet.
 *  \param[in]    ArpDstIpAddr     Target IP address contained in the ARP packet.
 *  \param[in]    ArpSrcMacAddrPtr Source MAC address contained in the ARP packet.
 *  \return                        E_OK:     No conflict was detected.
 *  \return                        E_NOT_OK: Address conflict detected.
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
IPV4_LOCAL FUNC(Std_ReturnType, IPV4_CODE) IpV4_Arp_VDetectLinkLocalAddressConflict(
  uint8              IpCtrlIdx,
  uint16             ArpOperation,
  IpBase_AddrInType  LocIpAddr,
  IpBase_AddrInType  ArpSrcIpAddr,
  IpBase_AddrInType  ArpDstIpAddr,
  IPV4_P2C(uint8)    ArpSrcMacAddrPtr);
#endif /* IPV4_IP_DYN_LL_ADDR_CFG_SUPP */

#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Arp_VUpdateTable
 **********************************************************************************************************************/
/*! \brief        Updates an existing or inserts a new entry into the dynamic ARP table.
 *  \details      -
 *  \param[in]    IpCtrlIdx               Index of the IP controller/instance.
 *  \param[in]    ArpSrcIpAddr            Source IP address contained in the ARP packet.
 *  \param[in]    ArpSrcMacAddrPtr        Source MAC address contained in the ARP packet.
 *  \param[in]    AddIfNotExisting TRUE:  Add a new entry if no matching entry is found.
 *                                 FALSE: Leave ARP table unchanged if no matching entry is found.
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Arp_VUpdateTable(
  uint8              IpCtrlIdx,
  IpBase_AddrInType  ArpSrcIpAddr,
  IPV4_P2C(uint8)    ArpSrcMacAddrPtr,
  boolean            AddIfNotExisting);
#endif

/***********************************************************************************************************************
 *  IpV4_Arp_VRespondToPacket
 **********************************************************************************************************************/
/*! \brief        Responds to a received ARP packet if required.
 *  \details      -
 *  \param[in]    IpCtrlIdx        Index of the IP controller/instance.
 *  \param[in]    ArpOperation     Operation of the ARP packet. (IPV4_ARP_OP_REQUEST or IPV4_ARP_OP_REPLY)
 *  \param[in]    ArpSrcIpAddr     Source IP address contained in the ARP packet.
 *  \param[in]    ArpDstIpAddr     Target IP address contained in the ARP packet.
 *  \param[in]    ArpSrcMacAddrPtr Source MAC address contained in the ARP packet.
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
IPV4_LOCAL FUNC(void, IPV4_CODE) IpV4_Arp_VRespondToPacket(
   uint8              IpCtrlIdx,
   uint16             ArpOperation,
   IpBase_AddrInType  ArpSrcIpAddr,
   IpBase_AddrInType  ArpDstIpAddr,
   IPV4_P2C(uint8)    ArpSrcMacAddrPtr);

/***********************************************************************************************************************
 *  IpV4_Arp_VHandlePacket
 **********************************************************************************************************************/
/*! \brief        Handles a received ARP packet.
 *  \details      -
 *  \param[in]    IpCtrlIdx        Index of the IP controller/instance.
 *  \param[in]    ArpOperation     Operation of the ARP packet. (IPV4_ARP_OP_REQUEST or IPV4_ARP_OP_REPLY)
 *  \param[in]    ArpSrcIpAddr     Source IP address contained in the ARP packet.
 *  \param[in]    ArpDstIpAddr     Target IP address contained in the ARP packet.
 *  \param[in]    ArpSrcMacAddrPtr Source MAC address contained in the ARP packet.
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
IPV4_LOCAL FUNC(void, IPV4_CODE) IpV4_Arp_VHandlePacket(
  uint8              IpCtrlIdx,
  uint16             ArpOperation,
  IpBase_AddrInType  ArpSrcIpAddr,
  IpBase_AddrInType  ArpDstIpAddr,
  IPV4_P2C(uint8)    ArpSrcMacAddrPtr);

/***********************************************************************************************************************
 * IpV4_Arp_VReadStaticTable
 **********************************************************************************************************************/
/*! \brief        Copies the elements of the static ARP table into the given user defined buffer.
 *  \details      -
 *  \param[in]    IpCtrlIdx              Index of the IP controller/instance.
 *  \param[in]    NumOfElements          Maximum number of entries that can be stored.
 *  \param[out]   EntryListPtr           Pointer to the memory where the list of cache entries are stored.
 *                                       (May be NULL_PTR if only the number of entries shall be returned.)
 *  \param[out]   TableIdxPtr            Entry Index Pointer for the User defined buffer.
 *  \return                              EntryListPtr != NULL_PTR: Number of entries written to EntryList.
 *                                       EntryListPtr == NULL_PTR: Total number of entries in cache.
 *  \pre          PRE_TCPIP_ARP_INIT
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        CREQ_111995
 **********************************************************************************************************************/
#if (IPV4_ARP_STATIC_TABLES == STD_ON) 
IPV4_LOCAL FUNC(uint32, IPV4_CODE) IpV4_Arp_VReadStaticTable(
  uint8                                                         IpCtrlIdx,
  uint32                                                        NumOfElements,
  P2VAR(TcpIp_ArpCacheEntryType, IPV4_APPL_DATA, IPV4_APPL_VAR) EntryListPtr,
  IPV4_P2V(uint32)                                              TableIdxPtr);
#endif
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
/***********************************************************************************************************************
 * IpV4_Arp_VWriteValidEntries
 **********************************************************************************************************************/
/*! \brief         Copies Dynamic ARP Table elements into the given, user defined buffer.
 *  \details       -
 *  \param[in]     IpCtrlIdx                        Index of the IP controller/instance.
 *  \param[out]    EntryListPtr                     Pointer to the memory where the list of cache entries shall be stored.
 *  \param[in,out] TableIdxPtr                      Entry Index Pointer for the User defined buffer.
 *  \param[in,out] MaxNumOfDynamicElementsToReadPtr Maximum number of elements that can be read from dynamic ARP table.
 *  \return        TableIdxPtr                      Current Entry Index for the user defined buffer.
 *  \pre           PRE_TCPIP_ARP_INIT
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace         CREQ_111995
 **********************************************************************************************************************/
IPV4_LOCAL FUNC(uint32, IPV4_CODE) IpV4_Arp_VWriteValidEntries(
  uint8                             IpCtrlIdx,
  IPV4_P2V(TcpIp_ArpCacheEntryType) EntryListPtr,
  IPV4_P2V(uint32)                  TableIdxPtr,
  IPV4_P2V(uint32)                  MaxNumOfDynamicElementsToReadPtr );
/***********************************************************************************************************************
 * IpV4_Arp_VReadDynTable
 **********************************************************************************************************************/
/*! \brief         Copies Dynamic ARP Table elements into the given, user defined buffer.
 *  \details       -
 *  \param[in]     IpCtrlIdx              Index of the IP controller/instance.
 *  \param[in]     NumOfElements          Maximum number of entries that can be stored.
 *  \param[out]    EntryListPtr           Pointer to the memory where the list of cache entries shall be stored.
 *                                        (May be NULL_PTR if only the number of entries shall be returned.)
 *  \param[in,out] TableIdxPtr            Entry Index Pointer for the User defined buffer.
 *  \return                               EntryListPtr != NULL_PTR: Number of entries written to EntryList.
 *                                        EntryListPtr == NULL_PTR: Total number of entries in cache.
 *  \pre           PRE_TCPIP_ARP_INIT
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace         CREQ_111995
 **********************************************************************************************************************/ 
IPV4_LOCAL FUNC(uint32, IPV4_CODE) IpV4_Arp_VReadDynTable(
  uint8                                                         IpCtrlIdx,
  uint32                                                        NumOfElements,
  P2VAR(TcpIp_ArpCacheEntryType, IPV4_APPL_DATA, IPV4_APPL_VAR) EntryListPtr,
  IPV4_P2V(uint32)                                              TableIdxPtr);
#endif
/***********************************************************************************************************************
 * IpV4_Arp_VCreateEntry
 **********************************************************************************************************************/
/*! \brief          Create new entry and trigger ARP request in case the caller desires that.
 *  \details        -
 *  \param[in]      IpCtrlIdx              Index of the IP controller/instance.
 *  \param[in]      SockIdx                Socket index [range: SockIdx < IpV4_VCfgGetMaxNumSocks() or IPV4_INV_SOCK_IDX]
 *  \param[in]      NetAddr                Network address (network byte order).
 *  \param[in]      TableFull              Flag to indicate that TableIdxLru contains an entry that will be overwritten
 *  \param[in]      TableIdx               Index of ARP entry that shall be created.
 *
 *  \return         TCPIP_E_PHYS_ADDR_MISS  Address could not be resolved, resolution has been triggered successfully.
 *                  TCP_E_NOT_OK            Address could not be resolved, resolution has NOT been triggered
 *
 *  \pre            PRE_TCPIP_ARP_INIT
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \trace         
 **********************************************************************************************************************/
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
IPV4_LOCAL FUNC(TcpIp_ReturnType, IPV4_CODE) IpV4_Arp_VCreateEntry(
  uint8                IpCtrlIdx,
  IpBase_SockIdxType   SockIdx,
  IpBase_AddrInType    NetAddr,
  boolean              TableFull,
  uint8                TableIdx);
#endif

/***********************************************************************************************************************
 * IpV4_Arp_VCheckValidityOfEntry
 **********************************************************************************************************************/
/*! \brief          Check if the Entry is Valid or out of date.
 *  \details        This function verifies whether an ARP entry is too old, if so then it resets the entry timer,
                    changes status of that entry to INPROGRESS and tries to send the ARP request again.
 *  \param[in]      IpCtrlIdx              Index of the IP controller/instance.
 *  \param[in]      PhysAddrPtr            Pointer to the Physical address.
 *  \param[in]      NetAddr                Network address (network byte order).
 *  \param[in]      TableIdx               ARP resolution table Index.
 *  \return         TCPIP_E_PHYS_ADDR_MISS  ARP request already is in progress.
 *                  TCP_E_NOT_OK            Not resolved
 *                  TCP_E_OK                there is a valid entry for the given network address
 *  \pre            PRE_TCPIP_ARP_INIT
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \trace         
 **********************************************************************************************************************/
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
IPV4_LOCAL FUNC(TcpIp_ReturnType, IPV4_CODE) IpV4_Arp_VCheckValidityOfEntry(
  uint8                                   IpCtrlIdx,
  P2VAR(uint8, AUTOMATIC, IPV4_APPL_DATA) PhysAddrPtr,
  IpBase_AddrInType                       NetAddr,
  uint8                                   TableIdx);
#endif
/***********************************************************************************************************************
 * IpV4_Arp_VCheckArpEntry
 **********************************************************************************************************************/
/*! \brief          Check if the entry is already in the ARP table or not.
 *  \details        This function checks whether a valid entry exists in ARP resolution table.
 *  \param[in]      IpCtrlIdx              Index of the IP controller/instance.
 *  \param[in]      SockIdx                Socket index [range: SockIdx < IpV4_VCfgGetMaxNumSocks() or IPV4_INV_SOCK_IDX]
 *                                         (If a valid socket index is specified, the index of the matching ARP entry, if there is one, 
 *                                         is stored related to the socket in order to allow an optimized lookup next time.)
 *  \param[in]      NetAddr                Network address (network byte order).
 *  \param[out]     TableFullPtr           Indicates whether the ARP cache is completely filled.
 *  \param[out]     TableIdxLruPtr         Index of the least recently used entry.
 *  \return         Index of the matching entry or IPV4_ARP_INVALID_RES_IDX if not matching entry was found.
 *  \pre            PRE_TCPIP_ARP_INIT
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \trace         
 **********************************************************************************************************************/
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
IPV4_LOCAL FUNC(uint8, IPV4_CODE) IpV4_Arp_VCheckArpEntry(
  uint8                                        IpCtrlIdx,
  IpBase_SockIdxType                           SockIdx,
  IpBase_AddrInType                            NetAddr,
  P2VAR(boolean, AUTOMATIC, IPV4_APPL_VAR)     TableFullPtr,
  P2VAR(uint8, AUTOMATIC, IPV4_APPL_VAR)       TableIdxLruPtr);
#endif

/***********************************************************************************************************************
 * IpV4_Arp_VProcessPendingEntries
 **********************************************************************************************************************/
/*! \brief          Retransmits ARP requests of pending entries if required and handles address resolution timeout.
 *  \details        -
 *  \param[in]      IpCtrlIdx              Index of the IP controller/instance.
 *  \pre            PRE_TCPIP_ARP_INIT
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \trace         
 *******************************************************************************************************************/
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
IPV4_LOCAL FUNC(void, IPV4_CODE) IpV4_Arp_VProcessPendingEntries(
  uint8 IpCtrlIdx);
#endif

/***********************************************************************************************************************
 *  IpV4_Arp_GetPhysicalAddressFromDynamicTables
 **********************************************************************************************************************/
/*! \brief        Get physical address from a dynamic ARP table for a a given network address.
 *  \details      The cache table access is optimized for requesting the same address many times for the same socket.
 *                
 *  \param[in]    IpCtrlIdx               Index of the IP controller/instance.
 *  \param[in]    SockIdx                 Socket index [range: SockIdx < IpV4_VCfgGetMaxNumSocks() or IPV4_INV_SOCK_IDX]
 *  \param[in]    NetAddr                 Network address (network byte order)
 *  \param[out]   PhysAddrPtr             Pointer for physical address (network byte order)
 *  \param[in]    InitRes                 Specifies if the address resolution shall be initiated (TRUE) or not (FALSE) in
 *                                        case the physical address related to the specified IP address is currently
 *                                        unknown.
 *  \return       TCP_OK                  Address could be resolved
 *                TCP_E_NOT_OK            Address could not be resolved, resolution has NOT been triggered, even in case
 *                                        the parameter InitRes has indicated the callers wish to do so,
 *                TCPIP_E_PHYS_ADDR_MISS  Address could not be resolved, resolution has been triggered successfully,
 *                                        depending on the value of the parameter InitRes.
 *  \pre           PRE_TCPIP_ARP_INIT
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace         
 **********************************************************************************************************************/
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
IPV4_LOCAL FUNC(TcpIp_ReturnType, IPV4_CODE) IpV4_Arp_GetPhysicalAddressFromDynamicTables(
  uint8                                   IpCtrlIdx,
  TcpIp_SockIdxType                       SockIdx,
  IpBase_AddrInType                       NetAddr,
  P2VAR(uint8, AUTOMATIC, IPV4_APPL_DATA) PhysAddrPtr,
  boolean InitRes);
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  IpV4_Arp_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, IPV4_CODE) IpV4_Arp_Init(
    P2CONST(IpV4_Arp_ConfigType, IPV4_APPL_DATA, IPV4_CONST) CfgPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8 IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV4_05.06 */
#endif

  /* ----- Implementation ----------------------------------------------- */
  IPV4_UNUSED_PARAMETER(CfgPtr);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  

#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)

  /* #10 Initialize dynamic ARP table entries */
# if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  for (IpCtrlIdx = 0; IpCtrlIdx < IpV4_VCfgGetMaxNumCtrls(); IpCtrlIdx++)
# endif
  {
    uint8_least resTableIdx;
    uint8_least resMapTableIdx;

    /* #20 Invalidate all entries in the ARP table. */
    for (resTableIdx = 0; resTableIdx < IpV4_Arp_VCfgGetMaxNumAddrRes(); resTableIdx++)
    {
      /* for all table elements */
      IPV4_CTRL_VAR(IpV4_Arp_ResTable)[resTableIdx].Status = IPV4_ARP_ELSTATUS_INVALID;
    }

    /* mark all elements of Arp_ResMapTable as invalid */
    for (resMapTableIdx = 0; resMapTableIdx < IpV4_VCfgGetMaxNumSocks(); resMapTableIdx++)
    {
      /* for all table elements */      
# if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
      IpV4_Arp_ResMapTable[resMapTableIdx].IpCtrlIdx = IpCtrlIdx;
# endif
      IpV4_Arp_ResMapTable[resMapTableIdx].ResTableIdx = IPV4_ARP_INVALID_RES_IDX;
    }
  }

  IpV4_Arp_NumPendEle    = 0;
  IpV4_Arp_TimestampSec  = 0;
  IpV4_Arp_TimestampMsec = 0;
#endif
} /* IpV4_Arp_Init() */

/***********************************************************************************************************************
 *  IpV4_Arp_MapIpToPhysMulticastAddr
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPV4_CODE) IpV4_Arp_MapIpToPhysMulticastAddr(
  IpBase_AddrInType  NetMcAddr, 
  IPV4_P2V(uint8)    PhysMcAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_AddrInType netMcAddrHbo;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IpV4_State == IPV4_STATE_INIT);
  TCPIP_ASSERT(PhysMcAddr != NULL_PTR);
  TCPIP_ASSERT(IPV4_ADDR_IS_MULTICAST(NetMcAddr));

  /* "An IP host group address is mapped to an Ethernet multicast address
    *  by placing the low-order 23-bits of the IP address into the low-order
    *  23 bits of the Ethernet multicast address 01-00-5E-00-00-00 (hex)."
    * [RFC1112 6.4. Extensions to an Ethernet Local Network Module]
    */
  /* convert IP address from network byte order into host byte order */
  netMcAddrHbo = IPV4_NTOHL(NetMcAddr);

  /* Set first three bytes 01:00:5e:XX:XX:XX */
  PhysMcAddr[0] = 0x01U;
  PhysMcAddr[1] = 0x00U;
  PhysMcAddr[2] = 0x5EU;

  /* Set last 23 bits of IP address */
  PhysMcAddr[3] = (uint8)((netMcAddrHbo >> 16) & 0x0000007FU); /* 7 bit */
  PhysMcAddr[4] = (uint8) (netMcAddrHbo >> 8);                 /* 8 bit */
  PhysMcAddr[5] = (uint8) (netMcAddrHbo);                      /* 8 bit */
} /*IpV4_Arp_MapIpToPhysMulticastAddr() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  IpV4_Arp_VCreateEntry
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
IPV4_LOCAL FUNC(TcpIp_ReturnType, IPV4_CODE) IpV4_Arp_VCreateEntry(
  uint8                IpCtrlIdx,
  IpBase_SockIdxType   SockIdx,
  IpBase_AddrInType    NetAddr,
  boolean              TableFull,
  uint8                TableIdx)
{ 
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ReturnType retValue = TCPIP_E_PHYS_ADDR_MISS;

  /* ----- Implementation ----------------------------------------------- */  
  /* #10 In case DiscardedEntryHandling is active: Only modify an entry in case the ARP-table-entry is NOT full. */
  if ((!IpV4_Arp_CfgIsDiscardedEntryHandlingEnabled()) || (TableFull == FALSE))
  {
    /* #20 Try to send the ARP request. */
    if (IpV4_Arp_SendArpRequest(IpCtrlIdx, NetAddr) == E_OK)
    {
      uint8_least PhysAddrByteIdx;

      IPV4_VENTER_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      if (IPV4_CTRL_VAR(IpV4_Arp_ResTable)[TableIdx].Status != IPV4_ARP_ELSTATUS_INVALID)
      {
        IpV4_Arp_VPhysAddrTableChgNotification(IpCtrlIdx, TableIdx, FALSE);
      }

      /* set entry */
      for (PhysAddrByteIdx = 0; PhysAddrByteIdx < ETH_PHYS_ADDR_LEN_BYTE; PhysAddrByteIdx++)
      {
        IPV4_CTRL_VAR(IpV4_Arp_ResTable)[TableIdx].PhysAddr[PhysAddrByteIdx] = 0x00U;
      }

      IPV4_CTRL_VAR(IpV4_Arp_ResTable)[TableIdx].NetAddr   = NetAddr;
      IPV4_CTRL_VAR(IpV4_Arp_ResTable)[TableIdx].Status    = IPV4_ARP_ELSTATUS_INPROGESS;
      IPV4_CTRL_VAR(IpV4_Arp_ResTable)[TableIdx].TimeStamp = IpV4_Arp_TimestampSec;
      if (SockIdx != IPV4_INV_SOCK_IDX)
      {
# if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
        IpV4_Arp_ResMapTable[SockIdx].IpCtrlIdx   = IpCtrlIdx;
# endif
        IpV4_Arp_ResMapTable[SockIdx].ResTableIdx = TableIdx;
      }
      IpV4_Arp_NumPendEle++;
      
      IPV4_VLEAVE_CRITICAL_SECTION(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
    else
    {
      /* currently no buffer available, try again later (main function ?)
      "E_NOT_OK" will be returned */
    }
  }
  else
  {
    /* Due to the if-statement of this if-else-block it is sure that DiscardedEntryHandling is active, when we get here. */
    /* In case the new entry would overwrite an existing entry because the table is "full":
    Conduct the optional callout to indicate, which entry was discarded.
    IpV4_Arp_VPhysAddrTableChgNotification() is NOT used to do the discard callback, because in case of a new entry being discarded the ARP table is not changed. */
#if (IPV4_ARP_SUPPORT_DISCARD_HANDLING == STD_ON)
    IpV4_Arp_VPhysAddrTableChgDiscardedNotification(IpCtrlIdx, /*Discarded*/NetAddr);
    retValue = TCPIP_E_NOT_OK;
#endif
  }
  return retValue;
} /* IpV4_Arp_VCreatNewEntry() */

/***********************************************************************************************************************
 *  IpV4_Arp_VCheckValidityOfEntry
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
IPV4_LOCAL FUNC(TcpIp_ReturnType, IPV4_CODE) IpV4_Arp_VCheckValidityOfEntry(
  uint8                                   IpCtrlIdx,
  P2VAR(uint8, AUTOMATIC, IPV4_APPL_DATA) PhysAddrPtr,
  IpBase_AddrInType                       NetAddr,
  uint8                                   TableIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ReturnType retValue = TCPIP_E_PHYS_ADDR_MISS; 

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the entry is too old or not. */
  IpV4_Arp_TimestampSecType TimestampSec = IPV4_CTRL_VAR(IpV4_Arp_ResTable)[TableIdx].TimeStamp;
  /* we know that IpV4_Arp_TimestampSec will always be bigger than TimestampSec, so the values can be subtracted without considering the integer overflow */
  if((IpV4_Arp_VCfgGetElementTimeoutSec() == 0) || ((IpV4_Arp_TimestampSec - TimestampSec) < IpV4_Arp_VCfgGetElementTimeoutSec()))
  {
    /* table entry is not out of date */
    if ((IPV4_CTRL_VAR(IpV4_Arp_ResTable)[TableIdx].Status == IPV4_ARP_ELSTATUS_VALID) || 
        (IPV4_CTRL_VAR(IpV4_Arp_ResTable)[TableIdx].Status == IPV4_ARP_ELSTATUS_PROBE))
    {
      /* there is a valid entry for this network address -> return physical address */
      IpV4_VAssignPhysAddr(PhysAddrPtr, &IPV4_CTRL_VAR(IpV4_Arp_ResTable)[TableIdx].PhysAddr[0]);
      retValue = TCPIP_OK;
    }
    /* else: ARP request already is in progress, RetValue remains TCPIP_E_PHYS_ADDR_MISS */
  }
  else
  {
    /* element is too old ! */
    /* #20 Reset entry to "in progress" and reset timestamp. */
    IPV4_CTRL_VAR(IpV4_Arp_ResTable)[TableIdx].Status    = IPV4_ARP_ELSTATUS_PROBE;
    IPV4_CTRL_VAR(IpV4_Arp_ResTable)[TableIdx].TimeStamp = IpV4_Arp_TimestampSec;
    IpV4_Arp_NumPendEle++;
    if(IpV4_Arp_SendArpRequest(IpCtrlIdx, NetAddr) != E_OK)
    {
      retValue = TCPIP_E_NOT_OK;
    }
    /* else RetValue remains TCPIP_E_PHYS_ADDR_MISS */
  }
  return retValue;
} /* IpV4_Arp_VCheckValidityOfEntry() */

/***********************************************************************************************************************
 *  IpV4_Arp_VCheckArpEntry
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
IPV4_LOCAL FUNC(uint8, IPV4_CODE) IpV4_Arp_VCheckArpEntry(
  uint8                                    IpCtrlIdx,
  IpBase_SockIdxType                       SockIdx,
  IpBase_AddrInType                        NetAddr,
  P2VAR(boolean, AUTOMATIC, IPV4_APPL_VAR) TableFullPtr,
  P2VAR(uint8, AUTOMATIC, IPV4_APPL_VAR)   TableIdxLruPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least tableIdx;

  IPV4_UNUSED_IPCTRLIDX();  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  
  
  /* Assume that first entry is the least recently used entry, this value is updated below if an older entry is found. */
  *TableIdxLruPtr = 0; 

  /* ----- Implementation ----------------------------------------------- */
  /* there is no valid entry in mapping table */
  /* #10 Check if ARP entry already exists (without entry in mapping table): */
  for(tableIdx = 0; (tableIdx < IpV4_Arp_VCfgGetMaxNumAddrRes()); tableIdx++)
  {
    if(IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].Status != IPV4_ARP_ELSTATUS_INVALID)
    {
      /* #20 Compare network addresses */
      if(IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].NetAddr == NetAddr)
      {
        /* there is an entry for this network address ("valid" or "in progress") */
        /* update socket mapping table, so entry is found faster next time */
        if (SockIdx != IPV4_INV_SOCK_IDX)
        {
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
          IpV4_Arp_ResMapTable[SockIdx].IpCtrlIdx   = IpCtrlIdx;
#endif
          IpV4_Arp_ResMapTable[SockIdx].ResTableIdx = (uint8)tableIdx;
          /* check for "not too old" will be done in next step */
        }
        break;
      }
      if ((IPV4_ARP_ELSTATUS_INVALID != IPV4_CTRL_VAR(IpV4_Arp_ResTable)[*TableIdxLruPtr].Status) && IPV4_ARP_TIMESTAMP_IS_OLDER(IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].TimeStamp, IPV4_CTRL_VAR(IpV4_Arp_ResTable)[*TableIdxLruPtr].TimeStamp))
      {
        /* store index of least recently used entry, if we need space for a new element later */
        *TableIdxLruPtr = (uint8)tableIdx;
        *TableFullPtr = IpV4_Arp_CfgIsDiscardedEntryHandlingEnabled();
      }
    }
    else
    {
      /* #30 Store invalid (unused) entry as least recently used entry */
      *TableIdxLruPtr = (uint8)tableIdx;
    }
  }

  /* Return IPV4_ARP_INVALID_RES_IDX if no matching entry was found. */
  if (tableIdx >= IpV4_Arp_VCfgGetMaxNumAddrRes())
  {
    tableIdx = IPV4_ARP_INVALID_RES_IDX;
  }

  return (uint8)tableIdx;
} /* IpV4_Arp_VCheckArpEntry() */
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)

/***********************************************************************************************************************
 *  IpV4_Arp_VProcessPendingEntries
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
IPV4_LOCAL FUNC(void, IPV4_CODE) IpV4_Arp_VProcessPendingEntries(uint8 IpCtrlIdx)
{
  uint8_least tableIdx;
  /* #10 Iterate over the ARP table. */
  for(tableIdx = 0; (tableIdx < IpV4_Arp_VCfgGetMaxNumAddrRes()); tableIdx++)
  {
    /* #20 Check the status of each entry. */
    if ((IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].Status == IPV4_ARP_ELSTATUS_INPROGESS) ||
        (IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].Status == IPV4_ARP_ELSTATUS_PROBE) )
    {
      /* #30 Remove entry from ARP table if retry time has exceeded. */
      if((IpV4_Arp_TimestampSec - IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].TimeStamp) >= IpV4_Arp_VCfgGetRetryTimeSec())
      {
        /* #40 Invalidate the ARP entry. */
        /* timeout expired, invalidate the table entry */
        IpV4_Arp_VPhysAddrTableChgNotification(IpCtrlIdx, (uint8)tableIdx, FALSE);
        IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].Status = IPV4_ARP_ELSTATUS_INVALID;
        IpV4_Arp_NumPendEle--;
        {
          IpBase_SockAddrInType DestSockAddrIn;
          DestSockAddrIn.sin_family = IPBASE_AF_INET;
          DestSockAddrIn.sin_addr   = IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].NetAddr;
          
          /* call timeout function */
          TcpIp_Cbk_VAddrResTimeout(IpCtrlIdx, ((IpBase_SockAddrType *)&DestSockAddrIn)); /* PRQA S 0310 */ /* MD_IpV4_11.04 */
#if (IPV4_SUPPORT_ICMP == STD_ON)
          IpV4_Icmp_Cbk_VAddrResTimeout(IpCtrlIdx, ((IpBase_SockAddrType *)&DestSockAddrIn)); /* PRQA S 0310 */ /* MD_IpV4_11.04 */
#endif
        }
        if(IpV4_Arp_NumPendEle == 0)
        {
          break;
        }
      }
      /* #50 Send ARP Request if time interval between ARP requests has been reached. */
      else if(((IpV4_Arp_TimestampSec - IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].TimeStamp) % IpV4_Arp_VCfgGetRetryIntervalSec()) == 0)
      {
        /* trigger retry */
        (void)IpV4_Arp_SendArpRequest(IpCtrlIdx, IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].NetAddr);
      }
      else
      {
         /* Wait for ARP response or next retry timeout. */
      }
    }
  }
} /* IpV4_Arp_VProcessPendingEntries() */
#endif
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Arp_GetPhysicalAddressFromDynamicTables
 **********************************************************************************************************************/
/*!
 * Internal comment removed. *
 *
 *
 *
 */
IPV4_LOCAL FUNC(TcpIp_ReturnType, IPV4_CODE) IpV4_Arp_GetPhysicalAddressFromDynamicTables(
  uint8                                   IpCtrlIdx,
  TcpIp_SockIdxType                       SockIdx,
  IpBase_AddrInType                       NetAddr,
  P2VAR(uint8, AUTOMATIC, IPV4_APPL_DATA) PhysAddrPtr,
  boolean                                 InitRes)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ReturnType retValue = TCPIP_E_PHYS_ADDR_MISS;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the Dynamic table is configured for the given controller. */
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
  if (IpV4_Arp_VCfgGetMaxNumAddrRes() == 0)
  {
    /* no dynamic ARP table configured on this controller */
    retValue = TCPIP_E_NOT_OK;
  }
  else
  {
    boolean        elemInTable;
    uint8_least    tableIdx;
    uint8          tableIdxLru = 0;  /* least recently used entry index */
    boolean        tableFull   = FALSE; /* flag to indicate that tableIdxLru contains an entry that will be overwritten (i.e. not an entry that's still unused) */

    /* Do not check the whole table but only the element for this socket
    - check if IP address is still the same
    - read physical address */
    if(SockIdx != IPV4_INV_SOCK_IDX)
    {
# if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
      if (IpV4_Arp_ResMapTable[SockIdx].IpCtrlIdx != IpCtrlIdx)
      {
        tableIdx = IPV4_ARP_INVALID_RES_IDX;
      }
      else
# endif
      {
        tableIdx = IpV4_Arp_ResMapTable[SockIdx].ResTableIdx;
      }
    }
    else
    {
      tableIdx = IPV4_ARP_INVALID_RES_IDX;
    }

    if ((IPV4_ARP_INVALID_RES_IDX != tableIdx) && (IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].Status != IPV4_ARP_ELSTATUS_INVALID) && (IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].NetAddr == NetAddr))
    {
      /* there is an entry for this network address ("valid" or "in progress") */
      elemInTable = TRUE;
      /* check for "not too old" will be done in next step */
    }
    else
    {
      /* there is no valid entry in mapping table */
      /* #20 Check if ARP entry already exists (without entry in mapping table) */
      tableIdx = IpV4_Arp_VCheckArpEntry(IpCtrlIdx, SockIdx, NetAddr, &tableFull, &tableIdxLru);

      elemInTable = (boolean)(tableIdx != IPV4_ARP_INVALID_RES_IDX);
    }

    if(elemInTable == TRUE)
    {
      /* check if too old */
      retValue = IpV4_Arp_VCheckValidityOfEntry(IpCtrlIdx, PhysAddrPtr, NetAddr, (uint8)tableIdx);
    }
    else
    {
      /* no matching element in table. Create new entry and trigger ARP request in case the caller desires that. */
      if(InitRes == TRUE)
      {
        retValue = IpV4_Arp_VCreateEntry(IpCtrlIdx, SockIdx, NetAddr, tableFull, tableIdxLru);
      }
    }
#endif
  }
  return retValue;
}  /* IpV4_Arp_GetPhysicalAddressFromDynamicTables() */ /* PRQA S 2006, 6010, 6030, 6050, 6060, 6080 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MS_MSR_STCAL, MD_MSR_STPAR, MD_MSR_STIMF */
#endif

/***********************************************************************************************************************
 *  IpV4_Arp_GetPhysicalAddress
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
FUNC(TcpIp_ReturnType, IPV4_CODE) IpV4_Arp_GetPhysicalAddress(
  uint8                                   IpCtrlIdx,
  TcpIp_SockIdxType                       SockIdx,
  IpBase_AddrInType                       NetAddr,
  P2VAR(uint8, AUTOMATIC, IPV4_APPL_DATA) PhysAddrPtr,
  boolean                                 InitRes )
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ReturnType retVal;
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)

#endif
  IpBase_AddrInType invertedNetMask;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IpV4_State == IPV4_STATE_INIT);
  TCPIP_ASSERT(IPV4_ARP_COND_VALID_SOCKIDX(SockIdx) || (SockIdx == IPV4_INV_SOCK_IDX));
  TCPIP_ASSERT(IpCtrlIdx < IpV4_VCfgGetMaxNumCtrls());
  TCPIP_ASSERT(PhysAddrPtr != NULL_PTR);

  /* #10 Return calculated group MAC address, if IP address is multicast address. */
  if (IPV4_ADDR_IS_MULTICAST(NetAddr))
  {    
    IpV4_Arp_MapIpToPhysMulticastAddr(NetAddr, PhysAddrPtr);
    retVal = TCPIP_OK;
  }
  else
  {
    invertedNetMask = ~(IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetMask);
    /* #20 Return broadcast MAC address, if IP address is global or local broadcast. */
    if ((NetAddr & invertedNetMask) == invertedNetMask)
    {      
      IpV4_VAssignPhysAddr(PhysAddrPtr, IpV4_Arp_PhysBroadcastAddr);
      retVal = TCPIP_OK;
    }
    /* #30 Verify if the IP address is valid. */
    else if ((invertedNetMask & NetAddr) == 0)
    {      
      retVal = TCPIP_E_NOT_OK;
    }
    /* #40 Check if static mapping is configured, if static ARP table is configured. */
#if (IPV4_ARP_STATIC_TABLES == STD_ON)
    else if (IpV4_Arp_VStaticTableLookup(IpCtrlIdx, NetAddr, PhysAddrPtr) == E_OK)
    {
      retVal = TCPIP_OK;
    }
#endif
    /* #50 Search for cached mapping in dynamic ARP table or trigger address resolution if required. */
    else 
    {
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
      retVal = IpV4_Arp_GetPhysicalAddressFromDynamicTables(IpCtrlIdx, SockIdx, NetAddr, PhysAddrPtr, InitRes);
#else
      retVal = TCPIP_E_NOT_OK;
#endif
    }
  }
  return retVal;
} /* IpV4_Arp_GetPhysicalAddress() */ /* PRQA S 2006, 6010, 6030, 6050, 6060, 6080 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MS_MSR_STCAL, MD_MSR_STPAR, MD_MSR_STIMF */

/***********************************************************************************************************************
 *  IpV4_Arp_RxIndication
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Arp_RxIndication(
  uint8                                           CtrlIdx,
  Eth_FrameType                                   FrameType,
  boolean                                         IsBroadcast,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA)        PhysAddrPtr, /* PRQA S 3673 */ /* MD_IPV4_16.7_3673 */
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA)        DataPtr,     /* PRQA S 3673 */ /* MD_IPV4_16.7_3673 */
  uint16                                          LenByte )
{
  /* ----- Implementation ----------------------------------------------- */
  IPV4_UNUSED_PARAMETER(FrameType);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  
  IPV4_UNUSED_PARAMETER(IsBroadcast);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  
  IPV4_UNUSED_PARAMETER(PhysAddrPtr);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  

  TCPIP_ASSERT(IpV4_State == IPV4_STATE_INIT);
  TCPIP_ASSERT(PhysAddrPtr != NULL_PTR);
  TCPIP_ASSERT(DataPtr     != NULL_PTR);
  
  /* #10 Check if ARP packet has minimum length. */
  if (LenByte >= IPV4_ARP_PACKET_LEN_BYTE)
  {
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
    uint8 IpCtrlIdx;           /* PRQA S 0781 */ /* MD_IpV4_05.06 */
#else
    const uint8 IpCtrlIdx = 0; /* PRQA S 0781 */ /* MD_IpV4_05.06 */
#endif

    /* #20 Check if IP instance is configured for CtrlIdx. */
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
    if (IPV4_CTRL_IDX_ETHIF_2_IP(CtrlIdx, IpCtrlIdx))
#else
    if (CtrlIdx == IPV4_CTRL_IDX_IP_2_ETHIF(IpCtrlIdx))
#endif
    {
      IPV4_P2C(IpV4_Arp_Packet4Type) arpRxMsgPtr = (IPV4_P2C(IpV4_Arp_Packet4Type))DataPtr; /* PRQA S 0310 */ /* MD_IpV4_11.04 */
      
      /* #30 Check ARP hardware and protocol address formats. */
      if((arpRxMsgPtr->HwAddrType   == IPV4_ARP_HW_ADDR_TYPE_ETHERNET) &&
         (arpRxMsgPtr->HwAddrSize   == IPV4_ARP_HW_ADDR_SIZE_ETHERNET) &&
         (arpRxMsgPtr->ProtAddrType == IPV4_ARP_PROT_ADDR_TYPE_IPV4  ) &&
         (arpRxMsgPtr->ProtAddrSize == IPV4_ARP_PROT_ADDR_SIZE_IPV4  ))
      {
        IpBase_AddrInType SrcIpAddr;
        IpBase_AddrInType DstIpAddr;

#if ( CPU_BYTE_ORDER == HIGH_BYTE_FIRST )
        /* store physical source address of incoming ARP message */
        SrcIpAddr = ((uint32)(arpRxMsgPtr->SrcIpAddrLow) +
                   (((uint32)(arpRxMsgPtr->SrcIpAddrHigh))<<16));
        /* store physical destination address of incoming ARP message */
        DstIpAddr = ((uint32)(arpRxMsgPtr->DestIpAddrLow) +
                   (((uint32)(arpRxMsgPtr->DestIpAddrHigh))<<16 ));
#else
        /* store physical source address of incoming ARP message */
        SrcIpAddr = ((uint32)(arpRxMsgPtr->SrcIpAddrHigh) +
                   (((uint32)(arpRxMsgPtr->SrcIpAddrLow)) << 16));
        /* store physical destination address of incoming ARP message */
        DstIpAddr = ((uint32)(arpRxMsgPtr->DestIpAddrHigh) +
                   (((uint32)(arpRxMsgPtr->DestIpAddrLow)) << 16 ));
#endif

        /* #40 Handle received ARP packet. */
        IpV4_Arp_VHandlePacket(IpCtrlIdx, arpRxMsgPtr->Operation, SrcIpAddr, DstIpAddr, &arpRxMsgPtr->SrcMacAddr[0]);
      }
    }
  } 
} /* IpV4_Arp_RxIndication() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STIMF */

/***********************************************************************************************************************
 *  IpV4_Arp_SendArpRequest
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, IPV4_CODE) IpV4_Arp_SendArpRequest(
  uint8 IpCtrlIdx,
  IpBase_AddrInType NetAddr )
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_AddrInType       locNetAddr; /* Source IP address */

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IpV4_State == IPV4_STATE_INIT);
  TCPIP_ASSERT(IpCtrlIdx < IpV4_VCfgGetMaxNumCtrls());

  /* #10 Prepare ARP request. */
  locNetAddr = IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr;

  /* #20 Send an ARP request. */
  return IpV4_Arp_VSendMessage(IpCtrlIdx, NetAddr, locNetAddr, &IpV4_Arp_PhysBroadcastAddr[0], IPV4_ARP_OP_REQUEST);
} /* IpV4_Arp_SendArpRequest() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if (IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON)
/**********************************************************************************************************************
 *  IpV4_Arp_SendArpProbe
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, IPV4_CODE) IpV4_Arp_SendArpProbe(
  uint8             IpCtrlIdx,
  IpBase_AddrInType IpAddrToProbe)
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_AddrInType       remNetAddr;
  IpBase_AddrInType       locNetAddr;
  IpBase_EthPhysAddrType  remPhysAddr;
  uint16                  operation;
  uint8_least             physAddrByteIdx;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IpV4_State == IPV4_STATE_INIT);
  TCPIP_ASSERT(IpCtrlIdx < IpV4_VCfgGetMaxNumCtrls());

  /* #10 Set all bytes of a target hardware address to zero. */
  /* "The 'target hardware address' field is ignored and SHOULD be set to all zeroes. */
  for (physAddrByteIdx = 0; physAddrByteIdx < ETH_PHYS_ADDR_LEN_BYTE; physAddrByteIdx++) 
  {
    remPhysAddr[physAddrByteIdx] = 0;
  }

  /* #20 Prepare ARP request. */
  locNetAddr = 0;                   /* "... with an all-zero 'sender IP address'." */
  remNetAddr = IpAddrToProbe;       /* "The 'target IP address' field MUST be set to the address being probed." */
  operation  = IPV4_ARP_OP_REQUEST; /* "ARP Request packet, broadcast on the local link" */

  /* #30 Send ARP request. */
  return IpV4_Arp_VSendMessage(IpCtrlIdx, remNetAddr, locNetAddr, &remPhysAddr[0], operation);
} /* IpV4_Arp_SendArpProbe() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  IpV4_Arp_SendArpAnnouncement
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, IPV4_CODE) IpV4_Arp_SendArpAnnouncement(
  uint8             IpCtrlIdx,
  IpBase_AddrInType IpAddrToAnnounce)
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_AddrInType       remNetAddr;
  IpBase_AddrInType       locNetAddr;
  IpBase_EthPhysAddrType  remPhysAddr;
  uint16                  operation;
  uint8_least             physAddrByteIdx;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IpV4_State == IPV4_STATE_INIT);
  TCPIP_ASSERT(IpCtrlIdx < IpV4_VCfgGetMaxNumCtrls());

  /* "'ARP Announcement' is [...] an ARP Request packet,
   *  broadcast on the local link, identical to the ARP Probe [...]
   *  except that both the sender and target IP address fields contain the IP address being announced."
   */
  /* #10 Set all bytes of a target hardware address to zero.*/
  /* "The 'target hardware address' field is ignored and SHOULD be set to all zeroes. */
  for (physAddrByteIdx = 0; physAddrByteIdx < ETH_PHYS_ADDR_LEN_BYTE; physAddrByteIdx++)
  {
    remPhysAddr[physAddrByteIdx] = 0;
  }

  /* #20 Prepare ARP request. */
  locNetAddr = IpAddrToAnnounce;
  remNetAddr = IpAddrToAnnounce;    /* "The 'target IP address' field MUST be set to the address being probed." */
  operation  = IPV4_ARP_OP_REQUEST; /* "ARP Request packet, broadcast on the local link" */

  /* #30 Send ARP request. */
  return IpV4_Arp_VSendMessage(IpCtrlIdx, remNetAddr, locNetAddr, &remPhysAddr[0], operation);
} /* IpV4_Arp_SendArpAnnouncement() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

/***********************************************************************************************************************
 *  IpV4_Arp_SendGratuitousArpReq
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, IPV4_CODE) IpV4_Arp_SendGratuitousArpReq(
  IpV4_AddrIdType IpAddrId )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType          retVal = E_NOT_OK;
  uint8                   errorId = TCPIP_E_NO_ERROR;
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8                   IpCtrlIdx;        /* PRQA S 0781 */ /* MD_IpV4_05.06 */
#else
  const uint8             IpCtrlIdx = 0;    /* PRQA S 0781 */ /* MD_IpV4_05.06 */
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (IPV4_DEV_ERROR_DETECT == STD_ON)
  if(!IPV4_COND_INIT)
  {
    errorId = IPV4_E_NOT_INITIALIZED;
  }
  else if(!IPV4_COND_VALID_IPADDRID(IpAddrId))
  {
    errorId = IPV4_E_INV_PARAM;
  }
  else
#endif
  {
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
    IpCtrlIdx = IpV4_Arp_GetCtrlIdxFromAddrId(IpAddrId);
    if(!IPV4_COND_VALID_CTRLIDX(IpCtrlIdx))
    {
      errorId = IPV4_E_INV_CTRL_IDX;
    }
    else
#endif
    {
      /* ----- Implementation ----------------------------------------------- */
      /* #10 Verify that IP instance is in state ONLINE. */
      if(IPV4_CTRL_VAR(IpV4_Ip_CtrlState) == IPV4_IP_ONLINE_ACTIVE)
      {
        IpBase_AddrInType       remNetAddr;
        IpBase_AddrInType       locNetAddr;
        uint16                  operation;
        IpBase_AddrInType       locIpAddr = IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr;

        /* #20 Fill the C-struct parameter that is given to IpV4_Arp_VSendMessage() in order to send a broadcast ARP request to publish the LocIpAddr. */
        locNetAddr = locIpAddr;           /* "In either case, the ARP Sender Protocol Address [...] are both set to the IP address of the cache entry to be updated" */
        remNetAddr = locIpAddr;           /* "In either case, the [...] ARP Target Protocol Address are both set to the IP address of the cache entry to be updated" */
        operation  = IPV4_ARP_OP_REQUEST; /* "ARP Request packet, broadcast on the local link" */

        /* #30 Pass the request to send the ARP request to the internal helper function that transmits ARP packages. */
        retVal = IpV4_Arp_VSendMessage(IpCtrlIdx, remNetAddr, locNetAddr, &IpV4_Arp_PhysBroadcastAddr[0], operation);
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (IPV4_DEV_ERROR_REPORT == STD_ON)
  if(errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(IPV4_MODULE_ID, IPV4_VINSTANCE_ID, IPV4_ARP_API_ID_SEND_GRATUITOUS_ARP_REQ, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* IpV4_Arp_SendGratuitousArpReq() */

#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Arp_MainFunction
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, IPV4_CODE) IpV4_Arp_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */

# if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  uint8 IpCtrlIdx; /* PRQA S 0781 */ /* MD_IpV4_05.06 */
# endif

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IpV4_State == IPV4_STATE_INIT);
  /* #10 Update time variable for timeout handling of ARP table entries. */
  IpV4_Arp_TimestampMsec += IpV4_VCfgGetMainFctPeriodMsec();
  /* #20 Execute MainFunction every second. */
  if(IpV4_Arp_TimestampMsec >= 1000)
  {
    IpV4_Arp_TimestampSec++;
    IpV4_Arp_TimestampMsec -= 1000;

    /* #30 Iterate over all configured IPv4 controllers. */
# if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
    IpCtrlIdx = IpV4_VCfgGetMaxNumCtrls();
    while (IpCtrlIdx > 0)
# endif
    {
# if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
      IpCtrlIdx--;
# else
      const uint8 IpCtrlIdx = 0; /* PRQA S 0781 */ /* MD_IpV4_05.06 */
# endif
      /* #40 Check if there are pending entries in the ARP table and ARP Requests have to be sent. */
      if(IpV4_Arp_NumPendEle != 0)
      {
        IpV4_Arp_VProcessPendingEntries(IpCtrlIdx);
      } 
    }
  }
} /* IpV4_Arp_MainFunction() */ /* PRQA S 2006, 6080 */ /* MD_MSR_14.7, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  IpV4_Arp_Reset
 **********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, IPV4_CODE) IpV4_Arp_Reset(
    uint8 IpCtrlIdx )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 tableIdx;

  /* ----- Implementation ----------------------------------------------- */  
  TCPIP_ASSERT(IpV4_State == IPV4_STATE_INIT);
  TCPIP_ASSERT(IpCtrlIdx  <  IPV4_MAX_NUM_CONTROLLERS);
  
  /* #10 Mark each element in the ARP table as invalid. */
  for (tableIdx = 0; tableIdx < IpV4_Arp_VCfgGetMaxNumAddrRes(); tableIdx++) 
  {
    /* for all table elements */
    IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].Status  = IPV4_ARP_ELSTATUS_INVALID;
    IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].NetAddr = 0;
  }

  /* #20 Reset counter of pending ARP resolutions. */
  IpV4_Arp_NumPendEle = 0;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

/***********************************************************************************************************************
 *  IpV4_Arp_GetCacheEntries
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType,IPV4_CODE) IpV4_Arp_GetCacheEntries(
  uint8                                                         CtrlIdx,
  P2VAR(uint32, IPV4_APPL_DATA, IPV4_APPL_VAR)                  NumberOfElementsPtr,
  P2VAR(TcpIp_ArpCacheEntryType, IPV4_APPL_DATA, IPV4_APPL_VAR) EntryListPtr)
{ 
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8  IpCtrlIdx = 0;  /* PRQA S 0781 */ /* MD_IpV4_05.06 */

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Check if IP instance is configured for given CtrlIdx */
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
  if (IPV4_CTRL_IDX_ETHIF_2_IP(CtrlIdx, IpCtrlIdx))
#else
  if (CtrlIdx == IPV4_CTRL_IDX_IP_2_ETHIF(IpCtrlIdx))
#endif
  {
    /* #20 Verify that IP instance is in state ONLINE. */
    if (IPV4_CTRL_VAR(IpV4_Ip_CtrlState) == IPV4_IP_ONLINE_ACTIVE)
    {
      uint32 totalNumOfDynamicElements = 0;
      uint32 totalNumOfStaticElements  = 0;
      uint32 globTableIdx = 0;  /* Entrylist Index */

      if (NumberOfElementsPtr[0] == 0)
      {
#if (IPV4_ARP_STATIC_TABLES == STD_ON)

        totalNumOfStaticElements = IpV4_Arp_VReadStaticTable(IpCtrlIdx, NumberOfElementsPtr[0], NULL_PTR, &globTableIdx);
#endif
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)

        totalNumOfDynamicElements = IpV4_Arp_VReadDynTable(IpCtrlIdx, NumberOfElementsPtr[0], NULL_PTR, &globTableIdx);
#endif
      }

      if (NumberOfElementsPtr[0] != 0)
      {
        /* #30 Copy static table entries, if configured. */
#if (IPV4_ARP_STATIC_TABLES == STD_ON)

        totalNumOfStaticElements = IpV4_Arp_VReadStaticTable(IpCtrlIdx, NumberOfElementsPtr[0], EntryListPtr, &globTableIdx);

        /* IpV4_Arp_VReadStaticTable() does not return a value > NumberOfElementsPtr[0]. */
        NumberOfElementsPtr[0] -= totalNumOfStaticElements;
#endif
        /* #40 Copy dynamic table entries, if configured. */
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
        
        totalNumOfDynamicElements = IpV4_Arp_VReadDynTable(IpCtrlIdx, NumberOfElementsPtr[0], EntryListPtr, &globTableIdx);
#endif
      }

      NumberOfElementsPtr[0] = totalNumOfDynamicElements + totalNumOfStaticElements;
      retVal = E_OK;
    }
  }
  return retVal;
} /* IpV4_Arp_GetCacheEntries() */

/***********************************************************************************************************************
 *  IpV4_Arp_ClearCache
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_Arp_ClearCache(
  IpV4_AddrIdType IpAddrId )
{
  TCPIP_ASSERT(IPV4_COND_VALID_IPADDRID(IpAddrId));

  IpV4_Arp_Reset(IPV4_ADDR_ID_2_CTRL_IDX(IpAddrId));
}

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  IpV4_Arp_VReadStaticTable
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
#if (IPV4_ARP_STATIC_TABLES == STD_ON)
IPV4_LOCAL FUNC(uint32, IPV4_CODE) IpV4_Arp_VReadStaticTable(
  uint8                             IpCtrlIdx,
  uint32                            NumOfElements,
  IPV4_P2V(TcpIp_ArpCacheEntryType) EntryListPtr,
  IPV4_P2V(uint32)                  TableIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 maxNumOfStaticElementsToRead = 0;

  /* ----- Implementation ----------------------------------------------- */
  if (Ipv4_Arp_VCfgGetStaticTableSize() > 0)
  {
    if ((EntryListPtr == NULL_PTR) || (NumOfElements > Ipv4_Arp_VCfgGetStaticTableSize()))
    {
      maxNumOfStaticElementsToRead = Ipv4_Arp_VCfgGetStaticTableSize();
    }
    else
    {
      maxNumOfStaticElementsToRead = NumOfElements;
    }

    if (EntryListPtr != NULL_PTR)
    {    
      /* #10 Copy valid entries from  Static table */
      for (*TableIdxPtr = 0; *TableIdxPtr < maxNumOfStaticElementsToRead; (*TableIdxPtr)++)
      { /* #20 Check the state of each entry */
        EntryListPtr[*TableIdxPtr].InetAddr[0] = IPV4_CTRL_VAR(IpV4_Arp_StaticTable)[*TableIdxPtr].IpAddr;
        EntryListPtr[*TableIdxPtr].PhysAddr[0] = IPV4_CTRL_VAR(IpV4_Arp_StaticTable)[*TableIdxPtr].PhysAddr[0];
        EntryListPtr[*TableIdxPtr].PhysAddr[1] = IPV4_CTRL_VAR(IpV4_Arp_StaticTable)[*TableIdxPtr].PhysAddr[1];
        EntryListPtr[*TableIdxPtr].PhysAddr[2] = IPV4_CTRL_VAR(IpV4_Arp_StaticTable)[*TableIdxPtr].PhysAddr[2];
        EntryListPtr[*TableIdxPtr].PhysAddr[3] = IPV4_CTRL_VAR(IpV4_Arp_StaticTable)[*TableIdxPtr].PhysAddr[3];
        EntryListPtr[*TableIdxPtr].PhysAddr[4] = IPV4_CTRL_VAR(IpV4_Arp_StaticTable)[*TableIdxPtr].PhysAddr[4];
        EntryListPtr[*TableIdxPtr].PhysAddr[5] = IPV4_CTRL_VAR(IpV4_Arp_StaticTable)[*TableIdxPtr].PhysAddr[5];
        EntryListPtr[*TableIdxPtr].State = TCPIP_ARP_ENTRY_STATIC;
      }
    }
  }

  return maxNumOfStaticElementsToRead; 
} /* IpV4_Arp_VReadStaticTable() */
#endif
/***********************************************************************************************************************
 * IpV4_Arp_VWriteValidEntries
 **********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 *
 *
 */
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
IPV4_LOCAL FUNC(uint32, IPV4_CODE) IpV4_Arp_VWriteValidEntries(
  uint8                             IpCtrlIdx,
  IPV4_P2V(TcpIp_ArpCacheEntryType) EntryListPtr,
  IPV4_P2V(uint32)                  TableIdxPtr,
  IPV4_P2V(uint32)                  MaxNumOfDynamicElementsToReadPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 dynTableIdx;

  /* ----- Implementation ----------------------------------------------- */
  IPV4_UNUSED_IPCTRLIDX();  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  

  /* #10 Iterate over dynamic ARP table. */
  for (dynTableIdx = 0; dynTableIdx < *MaxNumOfDynamicElementsToReadPtr; dynTableIdx++)
  {
    /* #20 Skip entry if state is INVALID or address resolution is still in progress. */
    if ((IPV4_CTRL_VAR(IpV4_Arp_ResTable)[dynTableIdx].Status == IPV4_ARP_ELSTATUS_INVALID) || (IPV4_CTRL_VAR(IpV4_Arp_ResTable)[dynTableIdx].Status == IPV4_ARP_ELSTATUS_INPROGESS))
    {
      if (*MaxNumOfDynamicElementsToReadPtr < IpV4_Arp_VCfgGetMaxNumAddrRes())
      {
        (*MaxNumOfDynamicElementsToReadPtr)++;
      }
    }
    /* #30 Otherwise: Write valid or stale entry to output entry list. */
    else
    { 
      EntryListPtr[*TableIdxPtr].InetAddr[0] = IPV4_CTRL_VAR(IpV4_Arp_ResTable)[dynTableIdx].NetAddr;
      EntryListPtr[*TableIdxPtr].PhysAddr[0] = IPV4_CTRL_VAR(IpV4_Arp_ResTable)[dynTableIdx].PhysAddr[0];
      EntryListPtr[*TableIdxPtr].PhysAddr[1] = IPV4_CTRL_VAR(IpV4_Arp_ResTable)[dynTableIdx].PhysAddr[1];
      EntryListPtr[*TableIdxPtr].PhysAddr[2] = IPV4_CTRL_VAR(IpV4_Arp_ResTable)[dynTableIdx].PhysAddr[2];
      EntryListPtr[*TableIdxPtr].PhysAddr[3] = IPV4_CTRL_VAR(IpV4_Arp_ResTable)[dynTableIdx].PhysAddr[3];
      EntryListPtr[*TableIdxPtr].PhysAddr[4] = IPV4_CTRL_VAR(IpV4_Arp_ResTable)[dynTableIdx].PhysAddr[4];
      EntryListPtr[*TableIdxPtr].PhysAddr[5] = IPV4_CTRL_VAR(IpV4_Arp_ResTable)[dynTableIdx].PhysAddr[5];

      if (IPV4_CTRL_VAR(IpV4_Arp_ResTable)[dynTableIdx].Status == IPV4_ARP_ELSTATUS_VALID)
      {
        EntryListPtr[*TableIdxPtr].State = TCPIP_ARP_ENTRY_VALID;
      }
      else
      {
        EntryListPtr[*TableIdxPtr].State = TCPIP_ARP_ENTRY_STALE;
      }
      (*TableIdxPtr)++;
    }
  }
  return *TableIdxPtr;
}
#endif
/***********************************************************************************************************************
 * IpV4_Arp_VReadDynTable
 **********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
IPV4_LOCAL FUNC(uint32, IPV4_CODE) IpV4_Arp_VReadDynTable(
    uint8                             IpCtrlIdx,
    uint32                            NumOfElements,
    IPV4_P2V(TcpIp_ArpCacheEntryType) EntryListPtr,
    IPV4_P2V(uint32)                  TableIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 maxNumOfDynamicElementsToRead;
  uint32 dynTableIdx;
  uint32 invalidEntryCount = 0;

  /* ----- Implementation ----------------------------------------------- */
  maxNumOfDynamicElementsToRead = IpV4_Arp_VCfgGetMaxNumAddrRes();

#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_ON)
#else
  IPV4_UNUSED_IPCTRLIDX();  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ 
#endif
  /* #10 Check if dynamic ARP table is configured on specified IP controller. */
  if (IpV4_Arp_VCfgGetMaxNumAddrRes() > 0)
  {
    /* #20 Return number of valid entries in the ARP table, if no output buffer was supplied. */
    if (EntryListPtr == NULL_PTR)
    {
      for (dynTableIdx = 0; dynTableIdx < maxNumOfDynamicElementsToRead; dynTableIdx++)
      {
        if ((IPV4_CTRL_VAR(IpV4_Arp_ResTable)[dynTableIdx].Status == IPV4_ARP_ELSTATUS_INVALID) || (IPV4_CTRL_VAR(IpV4_Arp_ResTable)[dynTableIdx].Status == IPV4_ARP_ELSTATUS_INPROGESS))
        {
          /* Ignore the entry*/
          invalidEntryCount++;
        }
      }
      maxNumOfDynamicElementsToRead -= invalidEntryCount;
    }
    else
    /* #30 Otherwise copy valid entries from the ARP table into the output buffer. 
           (Number of entries may be limited by the size of output buffer) */
    {
      uint32 previousTableIdx = *TableIdxPtr;

      /* limit maximum number of returned entries if provided output buffer cannot hold all entries. */
      if (NumOfElements < IpV4_Arp_VCfgGetMaxNumAddrRes())
      {      
        maxNumOfDynamicElementsToRead = NumOfElements;
      }
      *TableIdxPtr = IpV4_Arp_VWriteValidEntries(IpCtrlIdx, EntryListPtr, TableIdxPtr, &maxNumOfDynamicElementsToRead);
      maxNumOfDynamicElementsToRead = *TableIdxPtr - previousTableIdx;
    }
  }
  return maxNumOfDynamicElementsToRead;
} /* IpV4_Arp_VReadDynTable() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif
/***********************************************************************************************************************
 *  IpV4_Arp_VSendMessage
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
IPV4_LOCAL FUNC(Std_ReturnType, IPV4_CODE) IpV4_Arp_VSendMessage(
  uint8                   IpCtrlIdx,
  IpBase_AddrInType       RemNetAddr,
  IpBase_AddrInType       LocNetAddr,
  IPV4_P2C(uint8)         RemPhysAddrPtr,
  uint16                  Operation )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType           retValue   = E_NOT_OK;
  uint8                    bufIdx; /* PRQA S 0781 */ /* MD_IpV4_05.06 */
  uint16                   bufLenByte = IPV4_ARP_PACKET_LEN_BYTE;
  IPV4_P2V(TcpIp_DataType) ethIfBufferPtr;

  /* ----- Implementation ----------------------------------------------- */
  IPV4_UNUSED_IPCTRLIDX();  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  

  /* #10 Obtain ethernet TX buffer. */
  if (TCPIP_LL_ProvideTxBuffer(IPV4_CTRL_IDX_IP_2_ETHIF(IpCtrlIdx), IPV4_ARP_FRAME_TYPE, IpV4_Ip_VCfgGetDefaultEthIfFramePrio(), &bufIdx, &ethIfBufferPtr, &bufLenByte) == BUFREQ_OK)
  { 
    IPV4_P2V(IpV4_Arp_Packet4Type) arpTxMsgPtr = (IPV4_P2V(IpV4_Arp_Packet4Type))(ethIfBufferPtr); /* PRQA S 0310 */ /* MD_IpV4_11.04 */
    IpBase_EthPhysAddrType         ethDstAddr;

    TCPIP_ASSERT(bufLenByte >= IPV4_ARP_PACKET_LEN_BYTE);

    /* #20 Build the ARP packet. */
    arpTxMsgPtr->HwAddrType     = IPV4_ARP_HW_ADDR_TYPE_ETHERNET;
    arpTxMsgPtr->ProtAddrType   = IPV4_ARP_PROT_ADDR_TYPE_IPV4;
    arpTxMsgPtr->HwAddrSize     = IPV4_ARP_HW_ADDR_SIZE_ETHERNET;
    arpTxMsgPtr->ProtAddrSize   = IPV4_ARP_PROT_ADDR_SIZE_IPV4;
    arpTxMsgPtr->Operation      = Operation;

    /* store local MAC address. */
    TCPIP_LL_GetPhysAddr(IPV4_CTRL_IDX_IP_2_ETHIF(IpCtrlIdx), &(arpTxMsgPtr->SrcMacAddr[0]));

#if ( CPU_BYTE_ORDER == HIGH_BYTE_FIRST )
    arpTxMsgPtr->SrcIpAddrLow   = (uint16)(LocNetAddr & 0xFFFF);
    arpTxMsgPtr->SrcIpAddrHigh  = (uint16)(LocNetAddr >>16);
#else
    arpTxMsgPtr->SrcIpAddrHigh  = (uint16)(LocNetAddr & 0xFFFFu);
    arpTxMsgPtr->SrcIpAddrLow   = (uint16)(LocNetAddr >> 16);
#endif

    /* store remote MAC address. */
    IpV4_VAssignPhysAddr( &(arpTxMsgPtr->DestMacAddr[0]), RemPhysAddrPtr);

#if ( CPU_BYTE_ORDER == HIGH_BYTE_FIRST )
    arpTxMsgPtr->DestIpAddrLow  = (uint16)(RemNetAddr & 0xFFFF);
    arpTxMsgPtr->DestIpAddrHigh = (uint16)(RemNetAddr >> 16);
#else
    arpTxMsgPtr->DestIpAddrHigh = (uint16)(RemNetAddr & 0xFFFFu);
    arpTxMsgPtr->DestIpAddrLow  = (uint16)(RemNetAddr >> 16);
#endif

    /* Set destination MAC address of the ethernet frame to broadcast if sender IP address is a link-local address. */
    if ((LocNetAddr == 0) || ((LocNetAddr & IPV4_IP_LL_NET_ADDR_MASK) == IPV4_IP_LL_NET_ADDR_NET))
    {
      /* "All ARP packets (*replies* as well as requests) that contain a Link-
        * Local 'sender IP address' MUST be sent using link-layer broadcast
        * instead of link-layer unicast."
        * [RFC3927 2.5. Conflict Detection and Defense]
        */

      IpV4_VAssignPhysAddr(&ethDstAddr[0], &IpV4_Arp_PhysBroadcastAddr[0]);
    }
    else
    {
      IpV4_VAssignPhysAddr(&ethDstAddr[0], RemPhysAddrPtr);
    }

    /* #30 Transmit the message. */
    retValue = TCPIP_LL_Transmit( IPV4_CTRL_IDX_IP_2_ETHIF(IpCtrlIdx), bufIdx, IPV4_ARP_FRAME_TYPE, FALSE, IPV4_ARP_PACKET_LEN_BYTE, &ethDstAddr[0]);
  }
  /* else: EthIf could not provide a buffer, function will return with "E_NOT_OK". */

  return retValue;
} /* IpV4_Arp_VSendMessage() */

#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Arp_VUpdateEntry
 **********************************************************************************************************************/
/*! Internal comment removed. *
 *
 *
 *
 */
IPV4_LOCAL FUNC(void, IPV4_CODE) IpV4_Arp_VUpdateEntry(
  uint8            IpCtrlIdx,
  IPV4_P2C(uint8)  PhysAddrPtr,
  uint8            TableIdx,
  boolean          EntryInTable) 
{
  /* ----- Implementation ----------------------------------------------- */
  if ((EntryInTable == FALSE) || (!IPV4_LL_ADDR_IS_EQUAL(IPV4_CTRL_VAR(IpV4_Arp_ResTable)[TableIdx].PhysAddr, PhysAddrPtr)))
  {
    /* new entry in table or existing entry is updated */
    IpV4_VAssignPhysAddr(&(IPV4_CTRL_VAR(IpV4_Arp_ResTable)[TableIdx].PhysAddr[0]), PhysAddrPtr);
    /* Update state before notifying the upper layer. */
    IPV4_CTRL_VAR(IpV4_Arp_ResTable)[TableIdx].Status    = IPV4_ARP_ELSTATUS_VALID;
    IpV4_Arp_VPhysAddrTableChgNotification(IpCtrlIdx, (uint8)TableIdx, TRUE);
  }
  else
  {
    IPV4_CTRL_VAR(IpV4_Arp_ResTable)[TableIdx].Status    = IPV4_ARP_ELSTATUS_VALID;
  }
} /* IpV4_Arp_VUpdateEntry() */

/***********************************************************************************************************************
 *  IpV4_Arp_VStoreDynamicEntry
 **********************************************************************************************************************/
/*! Internal comment removed. *
 *
 *
 *
 *
 *
 */
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Arp_VStoreDynamicEntry(
  uint8                                      IpCtrlIdx,
  IpBase_AddrInType                          NetAddr,
  P2CONST(uint8, IPV4_APPL_DATA, IPV4_CONST) PhysAddrPtr,
  boolean                                    AddIfNotExisting)
{
   /* ----- Local Variables ---------------------------------------------- */
  boolean entryInTable = FALSE;
  uint8_least tableIdx;
  uint8_least oldestArpResTabEleIdx = 0;
  uint8_least freeArpResTabEleIdx   = IPV4_ARP_INVALID_RES_IDX;
 
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over ARP table and locate position of existing entry, unused entry or least recently used entry. */
  for (tableIdx = 0; tableIdx < IpV4_Arp_VCfgGetMaxNumAddrRes(); tableIdx++)
  {
    if(IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].Status != IPV4_ARP_ELSTATUS_INVALID)
    {
      /* entry is "valid" or "in progress" */
      if(IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].NetAddr == NetAddr)
      {
        /* matching entry found */
        entryInTable = TRUE;
        break;
      }

      if (IPV4_ARP_TIMESTAMP_IS_OLDER(IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].TimeStamp, IPV4_CTRL_VAR(IpV4_Arp_ResTable)[oldestArpResTabEleIdx].TimeStamp))
      {
        /* remember oldest entry */
        oldestArpResTabEleIdx = tableIdx;
      }
    }
    else if (IPV4_ARP_INVALID_RES_IDX == freeArpResTabEleIdx)
    {
      /* store free entry index */
      freeArpResTabEleIdx = tableIdx;
    }
    else
    {
      /* ignore further free entries */
    }
  }

  /* #20 Update existing entry or create new entry, if requested. */
  if ((entryInTable == TRUE) || (AddIfNotExisting == TRUE))
  {
    if (entryInTable == TRUE)
    {
      /* if entry has been found NetAddr already has the correct value */
      if ((IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].Status == IPV4_ARP_ELSTATUS_INPROGESS) ||
          (IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].Status == IPV4_ARP_ELSTATUS_PROBE) )
      {
        /* This is an answer for a pending request. */
        IpV4_Arp_NumPendEle--;
      }
    }
    else
    {
      /* #30 Check if an existing entry needs to be replaced by the new entry. */
      if (IPV4_ARP_INVALID_RES_IDX != freeArpResTabEleIdx)
      {
        /* use free entry */
        tableIdx = freeArpResTabEleIdx;
      }
      else
      {
        /* #40 Notify upper layer about new or changed entry, if necessary. */
        tableIdx = oldestArpResTabEleIdx;
        IpV4_Arp_VPhysAddrTableChgNotification(IpCtrlIdx, (uint8)tableIdx, FALSE);
      }

      IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].NetAddr   = NetAddr;
    }

    IPV4_CTRL_VAR(IpV4_Arp_ResTable)[tableIdx].TimeStamp = IpV4_Arp_TimestampSec;
    /* #40 Notify upper layer about new or changed entry, if necessary. */
    IpV4_Arp_VUpdateEntry(IpCtrlIdx, PhysAddrPtr, (uint8)tableIdx, entryInTable); 
  }

  /* PRQA S 2006, 6010, 6030 1 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC */
} /* IpV4_Arp_VStorePhysicalAddress() */

/**********************************************************************************************************************
 *  IpV4_Arp_VPhysAddrTableChgNotification
 *********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 */
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Arp_VPhysAddrTableChgNotification(
  uint8             IpCtrlIdx,
  uint8             ArpTableIdx,
  boolean           Valid)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SockAddrInetType sockAddrInet;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(IPV4_COND_VALID_CTRLIDX(IpCtrlIdx));
  TCPIP_ASSERT(ArpTableIdx < IpV4_Arp_VCfgGetMaxNumAddrRes());

  sockAddrInet.domain  = TCPIP_AF_INET;
  sockAddrInet.addr[0] = IPV4_CTRL_VAR(IpV4_Arp_ResTable)[ArpTableIdx].NetAddr;
  sockAddrInet.port    = TCPIP_PORT_ANY;

  TcpIp_VPhysAddrTableChg(IPV4_CTRL_IDX_IP_2_ETHIF(IpCtrlIdx), (TCPIP_P2V(TcpIp_SockAddrType))&sockAddrInet, &IPV4_CTRL_VAR(IpV4_Arp_ResTable)[ArpTableIdx].PhysAddr[0], Valid); /* PRQA S 0310 */ /* MD_IpV4_11.04 */

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* IpV4_Arp_VPhysAddrTableChgNotification() */

# if (IPV4_ARP_SUPPORT_DISCARD_HANDLING == STD_ON)
/**********************************************************************************************************************
 *  IpV4_Arp_VPhysAddrTableChgDiscardedNotification
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Arp_VPhysAddrTableChgDiscardedNotification(
  uint8                                       IpCtrlIdx,
  IpBase_AddrInType                           DiscardedNetAddr)  
{
   /* ----- Local Variables ---------------------------------------------- */
  uint8_least cbkIdx;
  TcpIp_SockAddrInetType sockAddrInet;

  /* ----- Implementation ----------------------------------------------- */
  sockAddrInet.domain  = TCPIP_AF_INET; /*IpV4 Protocol */
  sockAddrInet.addr[0] = DiscardedNetAddr;
  sockAddrInet.port    = TCPIP_PORT_ANY;
  /* Iterate over the ....*/
  for (cbkIdx = 0; cbkIdx < IpV4_Arp_CfgDiscardedEntryHandlingCbkCount(); cbkIdx++)
  {
    IpV4_Arp_CfgDiscardedEntryHandlingCbk((uint8)cbkIdx)(IPV4_CTRL_IDX_IP_2_ETHIF(IpCtrlIdx), (TCPIP_P2V(TcpIp_SockAddrType))&sockAddrInet);  /* PRQA S 0310 */ /* MD_IpV4_11.04 */
  }
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* IpV4_Arp_VPhysAddrTableChgDiscardedNotification() */
# endif
#endif

/**********************************************************************************************************************
 *  IpV4_Arp_VStaticTableLookup
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
#if (IPV4_ARP_STATIC_TABLES == STD_ON)
IPV4_LOCAL_INLINE FUNC(Std_ReturnType, IPV4_CODE) IpV4_Arp_VStaticTableLookup(
  uint8                                    IpCtrlIdx,
  IpBase_AddrInType                        NetAddr,
  P2VAR(uint8, IPV4_APPL_DATA, IPV4_CONST) PhysAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
   uint8_least tableIdx;
   Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */ 

  /* #10 Iterate over static ARP table and search for entry with matching IP address. */
  for (tableIdx = 0; tableIdx < Ipv4_Arp_VCfgGetStaticTableSize(); tableIdx++)
  {
    if (NetAddr == IPV4_CTRL_VAR(IpV4_Arp_StaticTable)[tableIdx].IpAddr)
    {
      if (PhysAddrPtr != NULL_PTR)
      {
        /* #20 Return configured physical/MAC address of matching entry. */
        IpV4_VAssignPhysAddr(&PhysAddrPtr[0], IPV4_CTRL_VAR(IpV4_Arp_StaticTable)[tableIdx].PhysAddr);
      }
      retVal = E_OK;
      break;
    }
  }
  /* No static ARP table entry exists for the supplied NetAddr. */
  return retVal;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* IpV4_Arp_VStaticTableLookup() */
#endif

#if (IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Arp_VDetectLinkLocalAddressConflict
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPV4_LOCAL FUNC(Std_ReturnType, IPV4_CODE) IpV4_Arp_VDetectLinkLocalAddressConflict(
  uint8              IpCtrlIdx,
  uint16             ArpOperation,
  IpBase_AddrInType  LocIpAddr,
  IpBase_AddrInType  ArpSrcIpAddr,
  IpBase_AddrInType  ArpDstIpAddr,
  IPV4_P2C(uint8)    ArpSrcMacAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if link-local address assignment is configured on controller. */
  if (IpV4_Ip_VCfgIsAddrAssignmentEnabled(IPV4_IP_ADDR_ASSIGNMENT_METHOD_LINKLOCAL))
  {
    boolean                sentByOtherHost;
    IpBase_EthPhysAddrType ownPhysAddr;

    TCPIP_LL_GetPhysAddr(IPV4_CTRL_IDX_IP_2_ETHIF(IpCtrlIdx), &(ownPhysAddr[0]));

    sentByOtherHost = (boolean)(!(IPV4_LL_ADDR_IS_EQUAL(ownPhysAddr, ArpSrcMacAddrPtr)));

    /* #20 Notify link-local address configuration that sender IP address is used by other node, if required. */
    if (ArpSrcIpAddr != 0) 
    {
      /* "If during this period, from the beginning of the probing process
        *  until ANNOUNCE_WAIT seconds after the last probe packet is sent,
        *  the host receives any ARP packet (Request *or* Reply) where the packet's
        *  'sender IP address' is the address being probed for, then the
        *  host MUST treat this address as being in use by some other host,
        *  and MUST select a new pseudo-random address and repeat the process."
        *  [RFC3927 2.2.1. Probe details]
        */

      IpV4_Ip_LlAddrInUse(IpCtrlIdx, ArpSrcIpAddr);
    }
    /* #30 Notify link-local address configuration that target IP address is used by other node, if required. */
    else if ((ArpOperation == IPV4_ARP_OP_REQUEST) && (sentByOtherHost == TRUE))
    {
      /* "In addition, if during this period the host receives any ARP Probe
        *  where the packet's 'target IP address' is the address being probed for,
        *  and the packet's 'sender hardware address' is not the hardware address
        *  of the interface the host is attempting to configure, then the host MUST
        *  similarly treat this as an address conflict and select a new address as above."
        */

      IpV4_Ip_LlAddrInUse(IpCtrlIdx, ArpDstIpAddr);
    }
    else
    {
      /* nothing to do */
    }

    /* #40 Notify link-local address configuration if a conflict was detected. */
    if((LocIpAddr != 0) && (ArpSrcIpAddr == LocIpAddr) && (sentByOtherHost == TRUE))
    {
      /* "At any time, if a host receives an ARP packet (request *or* reply) on
        *  an interface where the 'sender IP address' is the IP address the host
        *  has configured for that interface, but the 'sender hardware address'
        *  does not match the hardware address of that interface, then this is a
        *  conflicting ARP packet, indicating an address conflict."
        *  [RFC3927 2.5. Conflict Detection and Defense]
        */

      /* -> defend local address or restart address configuration */
      IpV4_Ip_AddrConflictInd(IPV4_CTRL_VAR(IpV4_Ip_AddrConfig)[IPV4_CTRL_ADDR_IDX_FIRST].IpAddrId);

      retVal = E_NOT_OK;
    }
  }

  return retVal;
} /* IpV4_Arp_VDetectLinkLocalAddressConflict() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */
#endif /* IPV4_IP_DYN_LL_ADDR_CFG_SUPP */

#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
/***********************************************************************************************************************
 *  IpV4_Arp_VUpdateTable
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(void, IPV4_CODE) IpV4_Arp_VUpdateTable(
  uint8              IpCtrlIdx,
  IpBase_AddrInType  ArpSrcIpAddr,
  IPV4_P2C(uint8)    ArpSrcMacAddrPtr,
  boolean            AddIfNotExisting)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if dynamic ARP table is configured for this controller. */
  if (IpV4_Arp_VCfgGetMaxNumAddrRes() > 0)
  {
# if (IPV4_ARP_STATIC_TABLES == STD_ON)     
    /* #20 Verify that entry does not exist in static ARP table, if configured. */
    if (IpV4_Arp_VStaticTableLookup(IpCtrlIdx, ArpSrcIpAddr, NULL_PTR) != E_OK)
# endif
    {
      /* #30 Store entry in the dynamic ARP table. */
      IpV4_Arp_VStoreDynamicEntry(IpCtrlIdx, ArpSrcIpAddr, ArpSrcMacAddrPtr, AddIfNotExisting);
    }
  }
} /* IpV4_Arp_VUpdateTable() */
#endif
/***********************************************************************************************************************
 *  IpV4_Arp_VRespondToPacket
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/

IPV4_LOCAL FUNC(void, IPV4_CODE) IpV4_Arp_VRespondToPacket(
  uint8              IpCtrlIdx,
  uint16             ArpOperation,
  IpBase_AddrInType  ArpSrcIpAddr,
  IpBase_AddrInType  ArpDstIpAddr,
  IPV4_P2C(uint8)    ArpSrcMacAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_AddrInType locIpAddr = IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if ARP packet is an ARP Request. */
  if (ArpOperation == IPV4_ARP_OP_REQUEST)
  {
    /* #20 Check if ARP Request is addressed to this node. */
    if ((locIpAddr != 0) && (ArpDstIpAddr == locIpAddr))
    {
      /* #30 Send ARP Reply. */
      (void)IpV4_Arp_VSendMessage(IpCtrlIdx, ArpSrcIpAddr, locIpAddr, ArpSrcMacAddrPtr, IPV4_ARP_OP_REPLY);
    }
  }
  /* else: No action necessary. */
} /* IpV4_Arp_VProccessArpRequest() */
/***********************************************************************************************************************
 *  IpV4_Arp_VHandlePacket
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPV4_LOCAL FUNC(void, IPV4_CODE) IpV4_Arp_VHandlePacket(
  uint8              IpCtrlIdx,
  uint16             ArpOperation,
  IpBase_AddrInType  ArpSrcIpAddr,
  IpBase_AddrInType  ArpDstIpAddr,
  IPV4_P2C(uint8)    ArpSrcMacAddrPtr)
{
   /* ----- Local Variables ---------------------------------------------- */
  /* get IP address of the controller */
  IpBase_AddrInType locIpAddr = IPV4_CTRL_VAR(IpV4_Ip_ActiveAddr).NetAddr;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Forward relevant information from ARP packet to link-local address configuration, if configured. (in order to detect IP address conflict) */
#if (IPV4_IP_DYN_LL_ADDR_CFG_SUPP == STD_ON)
  if (IpV4_Arp_VDetectLinkLocalAddressConflict(IpCtrlIdx, ArpOperation, locIpAddr, ArpSrcIpAddr, ArpDstIpAddr, ArpSrcMacAddrPtr) == E_OK)
#endif
  {
    /* #20 Check if source IP address of ARP packet is valid. */
    if (IPV4_ADDR_IS_MULTICAST(ArpSrcIpAddr) || IPV4_ADDR_IS_BROADCAST(ArpSrcIpAddr))
    {
      /* Ignore invalid source IP address. */
    }
    /* #30 Check if source MAC address of ARP packet is valid. */
    else if (!IPV4_LL_ADDR_IS_VALID(ArpSrcMacAddrPtr))
    {
      /* Ignore invalid source MAC address. */
    }
    else 
    {
      /* #40 Update dynamic ARP table, if required. */
#if (IPV4_ARP_DYNAMIC_TABLES == STD_ON)
      if (ArpSrcIpAddr != 0)
      {
        IpV4_Arp_VUpdateTable(IpCtrlIdx, ArpSrcIpAddr, ArpSrcMacAddrPtr, (boolean)(ArpDstIpAddr == locIpAddr));
      }
      else
      {
        /* this is an ARP probe (from an IP link local address configuration) or a packet with an invalid source IP address.
         -> do not store the physical address */
      }
#endif

      IpV4_Arp_VRespondToPacket(IpCtrlIdx, ArpOperation, ArpSrcIpAddr, ArpDstIpAddr, ArpSrcMacAddrPtr);
    }
  }
} /* IpV4_Arp_VHandlePacket() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */

#define IPV4_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

#endif  /* (TCPIP_SUPPORT_IPV4 == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: IpV4_Arp.c
 *********************************************************************************************************************/
