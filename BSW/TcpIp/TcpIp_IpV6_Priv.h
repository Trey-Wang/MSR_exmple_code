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
 *         \file  TcpIp_IpV6_Priv.h
 *        \brief  Implementation of Internet Protocol version 6 (IPv6) - Internal Functions
 *
 *      \details  This file is part of the TcpIp IPv6 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv6 submodule. >> TcpIp_IpV6.h
 *********************************************************************************************************************/

#if !defined(IPV6_PRIV_H)
#define IPV6_PRIV_H

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV6 == STD_ON)

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "TcpIp_IpV6.h"
#include "SchM_TcpIp.h"

#include "Platform_Types.h"

#if (IPV6_ENABLE_TCPIP == STD_ON)
# include "TcpIp_Priv.h"
#endif

#if (IPV6_ENABLE_DHCP == STD_ON)
# include "TcpIp_DhcpV6.h"
#endif

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FctLikeMacros */ /* Macros improve code readability */

#define IPV6_ETHER_TYPE_ID                  0x86DDu /* network byte order */

#define IPV6_HEADER_VERSION                      6u /* IPv6 version number */
#define IPV6_HEADER_NXT_HDR_FIELD_OFFS           6u /* position of NextHeader field in IPv6 header */
#define IPV6_HEADER_PLD_LEN_FIELD_OFFS           4u /* position of PayloadLength field in IPv6 header */

#define IPV6_MINIMUM_MTU                      1280U /* Minimum MTU value that every host must accept [RFC2460] */

# define IPV6_VENABLE_INTERNAL_DET_CHECKS    STD_OFF
# define IPV6_ENABLE_RX_DBG_COUNTERS         STD_OFF
# define IPV6_ENABLE_TX_DBG_COUNTERS         STD_OFF

#define IPV6_RSHIFT_DIV_2                        1u /*!< devide integer by  2 using right bitshift */
#define IPV6_RSHIFT_DIV_4                        2u /*!< devide integer by  4 using right bitshift */
#define IPV6_RSHIFT_DIV_8                        3u /*!< devide integer by  8 using right bitshift */
#define IPV6_RSHIFT_DIV_16                       4u /*!< devide integer by 16 using right bitshift */

#define IPV6_LSHIFT_MUL_2                        1u /*!< multiply integer by  2 using left bitshift */
#define IPV6_LSHIFT_MUL_4                        2u /*!< multiply integer by  4 using left bitshift */
#define IPV6_LSHIFT_MUL_8                        3u /*!< multiply integer by  8 using left bitshift */
#define IPV6_LSHIFT_MUL_16                       4u /*!< multiply integer by 16 using left bitshift */

#define IPV6_MASK_MOD_2                        0x01 /*!< use mask instead of modulo division by  2 */
#define IPV6_MASK_MOD_4                        0x03 /*!< use mask instead of modulo division by  4 */
#define IPV6_MASK_MOD_8                        0x07 /*!< use mask instead of modulo division by  8 */
#define IPV6_MASK_MOD_16                       0x0F /*!< use mask instead of modulo division by 16 */

/* compare results */
#define IPV6_CMP_RES_NO_PREFERENCE               0u
#define IPV6_CMP_RES_PREFER_A                    1u
#define IPV6_CMP_RES_PREFER_B                    2u

#define IPV6_MULTI_CONTROLLER_SUPPORT            (1 < IPV6_MAX_NUM_CONTROLLERS)

/* ------------------------------------------------- */
/* ------------------------------------------------- */
/* Configuration access (depending on configuration variant)*/

#define IPV6_CTRL_VAR(Var)                   ((Var)[IpCtrlIdx])

/* PRQA S 3410 1 */ /* MD_IpV6_19.10 */
#define IPV6_PRIVEXT_VAR(VAR_NAME)           (IpV6_PrivExt_Data[IpCtrlIdx]->VAR_NAME)

#define IPV6_CTRLIDX_IP_2_ETHIF(CtrlIdx)                IpV6_CtrlIdxIp2EthIf[(CtrlIdx)]
#define IPV6_CTRLIDX_ETHIF_2_IP(CtrlIdx, IpCtrlIdx)     IpV6_CtrlIdxEthIf2Ip((CtrlIdx), &(IpCtrlIdx))

/* General configuration values */

#define IpV6_VCfgGetMaxAddrId()                          (IPV6_MAX_ADDR_ID)
#define IpV6_VCfgGetMaxNumCtrls()                        (IPV6_MAX_NUM_CONTROLLERS)
#define IpV6_VCfgGetMaxNumTcpIpSockets()                 (IPV6_MAX_NUM_SOCKETS)

/* Controller instance specific configuration values */

#define IpV6_VCfgGetNeighborCacheSize()                  IPV6_CTRL_VAR(IpV6_NeighborCacheSize)
#define IpV6_VCfgGetDestinationCacheSize()               IPV6_CTRL_VAR(IpV6_DestinationCacheSize)
#define IpV6_VCfgGetSourceAddressTableSize()             IPV6_CTRL_VAR(IpV6_SourceAddressTableSize)
#define IpV6_VCfgGetPrefixListSize()                     IPV6_CTRL_VAR(IpV6_PrefixListSize)
#define IpV6_VCfgGetDefaultRouterListSize()              IPV6_CTRL_VAR(IpV6_DefaultRouterListSize)

#define IpV6_VCfgGetDhcpMode()                           IPV6_CTRL_VAR(IpV6_DhcpMode)

#if (IPV6_FRAGMENTATION_CONFIG_COUNT == 1)
/* only one fragmentation configuration, we can optimize access via defines */
# define IpV6_VCfgHasFragmentationConfig()               (TRUE == IPV6_CTRL_VAR(IpV6_HasFragmentationConfig))

# define IpV6_VCfgGetReassemblyTimeout()                 IPV6_FRAGMENTATION_REASSEMBLY_TIMEOUT
# define IpV6_VCfgGetReassemblyBufferSize()              IPV6_FRAGMENTATION_REASSEMBLY_BUFFER_SIZE
# define IpV6_VCfgGetReassemblyBufferCount()             IPV6_FRAGMENTATION_REASSEMBLY_BUFFER_COUNT
# define IpV6_VCfgGetTxFragmentBufferSize()              IPV6_FRAGMENTATION_TXFRAGMENT_BUFFER_SIZE
# define IpV6_VCfgGetTxFragmentBufferCount()             IPV6_FRAGMENTATION_TXFRAGMENT_BUFFER_COUNT
# define IpV6_VCfgGetReassemblySegmentCount()            IPV6_FRAGMENTATION_REASSEMBLY_SEGMENT_COUNT

#endif
#if (IPV6_FRAGMENTATION_CONFIG_COUNT > 1)
/* more than one fragmentation configuration we cannot use defines */
# define IpV6_VCfgHasFragmentationConfig()               (NULL_PTR != IPV6_CTRL_VAR(IpV6_FragmentationConfig))

# define IpV6_VCfgGetReassemblyTimeout()                 IPV6_CTRL_VAR(IpV6_FragmentationConfig)->ReassemblyTimeout
# define IpV6_VCfgGetReassemblyBufferSize()              IPV6_CTRL_VAR(IpV6_FragmentationConfig)->ReassemblyBufferSize
# define IpV6_VCfgGetReassemblyBufferCount()             IPV6_CTRL_VAR(IpV6_FragmentationConfig)->ReassemblyBufferCount
# define IpV6_VCfgGetTxFragmentBufferSize()              IPV6_CTRL_VAR(IpV6_FragmentationConfig)->TxFragmentBufferSize
# define IpV6_VCfgGetTxFragmentBufferCount()             IPV6_CTRL_VAR(IpV6_FragmentationConfig)->TxFragmentBufferCount
# define IpV6_VCfgGetReassemblySegmentCount()            IPV6_CTRL_VAR(IpV6_FragmentationConfig)->ReassemblySegmentCount
#endif

#define IpV6_VCfgIsRxFragmentationEnabled()              (IpV6_VCfgHasFragmentationConfig() && (0 < IpV6_VCfgGetReassemblyBufferCount()))
#define IpV6_VCfgIsTxFragmentationEnabled()              (IpV6_VCfgHasFragmentationConfig() && (0 < IpV6_VCfgGetTxFragmentBufferCount()))

#define IpV6_VCfgGetPathMtuTimeout()                     IPV6_CTRL_VAR(IpV6_MtuConfig).PathMtuTimeout
#define IpV6_VCfgGetDefaultLinkMtu()                     IPV6_CTRL_VAR(IpV6_MtuConfig).DefaultLinkMtu
#define IpV6_VCfgIsLinkMtuReconfigurationEnabled()       (TRUE == IPV6_CTRL_VAR(IpV6_MtuConfig).AllowLinkMtuReconfiguration)
#define IpV6_VCfgIsPathMtuEnabled()                      (TRUE == IPV6_CTRL_VAR(IpV6_MtuConfig).EnablePathMtu)

#if (IPV6_NDP_CONFIG_COUNT == 1)
/* If Ndp config count is 1, the same NDP config is used by all ip controller instances */
# define IpV6_VCfgGetSlaacMinLifetime()                  IPV6_NDP_SLAAC_MIN_LIFETIME
# define IpV6_VCfgGetReachableTime()                     IPV6_NDP_REACHABLE_TIME
# define IpV6_VCfgGetNudFirstProbeDelay()                IPV6_NDP_NUD_FIRST_PROBE_DELAY
# define IpV6_VCfgGetMaxRtrSolicitationDelay()           IPV6_NDP_MAX_RTR_SOLICITATION_DELAY
# define IpV6_VCfgGetRtrSolicitationInterval()           IPV6_NDP_RTR_SOLICITATION_INTERVAL
# define IpV6_VCfgGetRetransTimer()                      IPV6_NDP_RETRANS_TIMER
# define IpV6_VCfgGetMaxSlaacDelay()                     IPV6_NDP_MAX_SLAAC_DELAY
# define IpV6_VCfgGetMaxRtrSolicitations()               IPV6_NDP_MAX_RTR_SOLICITATIONS
# define IpV6_VCfgGetDadTransmits()                      IPV6_NDP_DAD_TRANSMITS
# define IpV6_VCfgGetUnicastSolicits()                   IPV6_NDP_UNICAST_SOLICITS
# define IpV6_VCfgGetMulticastSolicits()                 IPV6_NDP_MULTICAST_SOLICITS
# define IpV6_VCfgGetMinRandomFactor()                   IPV6_NDP_MIN_RANDOM_FACTOR
# define IpV6_VCfgGetMaxRandomFactor()                   IPV6_NDP_MAX_RANDOM_FACTOR
# define IpV6_VCfgGetStaticOnLinkPrefixCount()           IPV6_NDP_STATIC_ON_LINK_PREFIX_COUNT
# define IpV6_VCfgGetStaticOnLinkPrefixValue(IDX)        IpV6_StaticOnLinkPrefixValues[(IDX)]
# define IpV6_VCfgGetStaticOnLinkPrefixLength(IDX)       IpV6_StaticOnLinkPrefixLengths[(IDX)]
/* Code of the following features will only be ignored if feature is not active. */
# define IpV6_VCfgIsRndRtrSolicitationDelayEnabled()     (TRUE)
# define IpV6_VCfgIsNudEnabled()                         (TRUE)
# define IpV6_VCfgIsDynamicRetransTimerEnabled()         (TRUE)
# define IpV6_VCfgIsDynamicReachableTimeEnabled()        (TRUE)
# define IpV6_VCfgIsOptimizeCacheLookupsEnabled()        (TRUE)
# define IpV6_VCfgIsRandomReachableTimeEnabled()         (TRUE)
# define IpV6_VCfgIsSlaacDelayEnabled()                  (TRUE)
# define IpV6_VCfgIsRfc6106DnsslOptEnabled()             (TRUE)
# define IpV6_VCfgIsRfc6106RdnssOptEnabled()             (TRUE)
# define IpV6_VCfgIsOptimisticDadEnabled()               (TRUE)
# define IpV6_VCfgIsNdpInvNaEnabled()                    (TRUE)
# define IpV6_VCfgIsNdpInvNsEnabled()                    (TRUE)
# define IpV6_VCfgIsNdpInvNaNcUpdateEnabled()            (TRUE)
#endif
#if (IPV6_NDP_CONFIG_COUNT > 1)
# define IpV6_VCfgGetSlaacMinLifetime()                  IPV6_CTRL_VAR(IpV6_NdpConfig)->SlaacMinLifetime
# define IpV6_VCfgGetReachableTime()                     IPV6_CTRL_VAR(IpV6_NdpConfig)->ReachableTime
# define IpV6_VCfgGetNudFirstProbeDelay()                IPV6_CTRL_VAR(IpV6_NdpConfig)->NudFirstProbeDelay
# define IpV6_VCfgGetMaxRtrSolicitationDelay()           IPV6_CTRL_VAR(IpV6_NdpConfig)->MaxRtrSolicitationDelay
# define IpV6_VCfgGetRtrSolicitationInterval()           IPV6_CTRL_VAR(IpV6_NdpConfig)->RtrSolicitationInterval
# define IpV6_VCfgGetRetransTimer()                      IPV6_CTRL_VAR(IpV6_NdpConfig)->RetransTimer
# define IpV6_VCfgGetMaxSlaacDelay()                     IPV6_CTRL_VAR(IpV6_NdpConfig)->MaxSlaacDelay
# define IpV6_VCfgGetMaxRtrSolicitations()               IPV6_CTRL_VAR(IpV6_NdpConfig)->MaxRtrSolicitations
# define IpV6_VCfgGetDadTransmits()                      IPV6_CTRL_VAR(IpV6_NdpConfig)->DadTransmits
# define IpV6_VCfgGetUnicastSolicits()                   IPV6_CTRL_VAR(IpV6_NdpConfig)->UnicastSolicits
# define IpV6_VCfgGetMulticastSolicits()                 IPV6_CTRL_VAR(IpV6_NdpConfig)->MulticastSolicits
# define IpV6_VCfgGetMinRandomFactor()                   IPV6_CTRL_VAR(IpV6_NdpConfig)->MinRandomFactor
# define IpV6_VCfgGetMaxRandomFactor()                   IPV6_CTRL_VAR(IpV6_NdpConfig)->MaxRandomFactor
# define IpV6_VCfgGetStaticOnLinkPrefixCount()           IPV6_CTRL_VAR(IpV6_NdpConfig)->OnLinkPrefixCount
# define IpV6_VCfgGetStaticOnLinkPrefixValue(IDX)        IpV6_StaticOnLinkPrefixValues[IPV6_CTRL_VAR(IpV6_NdpConfig)->OnLinkPrefixListPtr[(IDX)]]
# define IpV6_VCfgGetStaticOnLinkPrefixLength(IDX)       IpV6_StaticOnLinkPrefixLengths[IPV6_CTRL_VAR(IpV6_NdpConfig)->OnLinkPrefixListPtr[(IDX)]]
# define IpV6_VCfgIsRndRtrSolicitationDelayEnabled()     (TRUE == IPV6_CTRL_VAR(IpV6_NdpConfig)->RndRtrSolicitationDelay)
# define IpV6_VCfgIsNudEnabled()                         (TRUE == IPV6_CTRL_VAR(IpV6_NdpConfig)->EnableNud)
# define IpV6_VCfgIsDynamicRetransTimerEnabled()         (TRUE == IPV6_CTRL_VAR(IpV6_NdpConfig)->DynamicRetransTimer)
# define IpV6_VCfgIsDynamicReachableTimeEnabled()        (TRUE == IPV6_CTRL_VAR(IpV6_NdpConfig)->DynamicReachableTime)
# define IpV6_VCfgIsOptimizeCacheLookupsEnabled()        (TRUE == IPV6_CTRL_VAR(IpV6_NdpConfig)->OptimizeCacheLookups)
# define IpV6_VCfgIsRandomReachableTimeEnabled()         (TRUE == IPV6_CTRL_VAR(IpV6_NdpConfig)->RandomReachableTime)
# define IpV6_VCfgIsSlaacDelayEnabled()                  (TRUE == IPV6_CTRL_VAR(IpV6_NdpConfig)->EnableSlaacDelay)
# define IpV6_VCfgIsRfc6106DnsslOptEnabled()             (TRUE == IPV6_CTRL_VAR(IpV6_NdpConfig)->EnableRfc6106DnsslOpt)
# define IpV6_VCfgIsRfc6106RdnssOptEnabled()             (TRUE == IPV6_CTRL_VAR(IpV6_NdpConfig)->EnableRfc6106RdnssOpt)
# define IpV6_VCfgIsOptimisticDadEnabled()               (TRUE == IPV6_CTRL_VAR(IpV6_NdpConfig)->EnableOptimisticDad)
# define IpV6_VCfgIsNdpInvNaEnabled()                    (TRUE == IPV6_CTRL_VAR(IpV6_NdpConfig)->EnableNdpInvNa)
# define IpV6_VCfgIsNdpInvNsEnabled()                    (TRUE == IPV6_CTRL_VAR(IpV6_NdpConfig)->EnableNdpInvNs)
# define IpV6_VCfgIsNdpInvNaNcUpdateEnabled()            (TRUE == IPV6_CTRL_VAR(IpV6_NdpConfig)->EnableNdpInvNaNcUpdate)
#endif

#if (IPV6_MLD_CONFIG_COUNT > 1)
# define IpV6_VCfgGetMldV1QuerierPresentTimeoutMs()      IPV6_CTRL_VAR(IpV6_MldConfig)->MldV1QuerierPresetTimeoutMs
# define IpV6_VCfgGetMldMinUnsolicitedReportIntMs()      IPV6_CTRL_VAR(IpV6_MldConfig)->MldMinUnsolicitedReportIntMs
# define IpV6_VCfgGetMldMaxUnsolicitedReportIntMs()      IPV6_CTRL_VAR(IpV6_MldConfig)->MldMaxUnsolicitedReportIntMs
# define IpV6_VCfgGetMldMaxReportMsgSize()               IPV6_CTRL_VAR(IpV6_MldConfig)->MldMaxReportMsgSize
# define IpV6_VCfgGetMldIfStatePoolSize()                IPV6_CTRL_VAR(IpV6_MldConfig)->MldIfStatePoolSize
# define IpV6_VCfgGetMldSrcAddrPoolSize()                IPV6_CTRL_VAR(IpV6_MldConfig)->MldSrcAddrPoolSize
# define IpV6_VCfgGetMldSocketMembershipPoolSize()       IPV6_CTRL_VAR(IpV6_MldConfig)->MldSocketMembershipPoolSize
# define IpV6_VCfgGetMldIfStateSrcAddrPoolSize()         IPV6_CTRL_VAR(IpV6_MldConfig)->MldIfStateSrcAddrPoolSize
# define IpV6_VCfgGetMldRobustness()                     IPV6_CTRL_VAR(IpV6_MldConfig)->MldRobustness
#endif
#if (IPV6_MLD_CONFIG_COUNT == 1)
# define IpV6_VCfgGetMldV1QuerierPresentTimeoutMs()      IPV6_MLD_V1_QUERIER_PRESENT_TIMEOUT
# define IpV6_VCfgGetMldMinUnsolicitedReportIntMs()      IPV6_MLD_MIN_UNSOL_REPORT_INTERVAL
# define IpV6_VCfgGetMldMaxUnsolicitedReportIntMs()      IPV6_MLD_MAX_UNSOL_REPORT_INTERVAL
# define IpV6_VCfgGetMldMaxReportMsgSize()               IPV6_MLD_MAX_REPORT_MSG_SIZE
# define IpV6_VCfgGetMldIfStatePoolSize()                IPV6_MLD_MCADDR_ENTRY_COUNT
# define IpV6_VCfgGetMldSrcAddrPoolSize()                IPV6_MLD_MCSRCADDR_ENTRY_COUNT
# define IpV6_VCfgGetMldSocketMembershipPoolSize()       IPV6_MLD_MCMS_ENTRY_COUNT
# define IpV6_VCfgGetMldIfStateSrcAddrPoolSize()         IPV6_MLD_IFSRCADDR_ENTRY_COUNT
# define IpV6_VCfgGetMldRobustness()                     IPV6_MLD_ROBUSTNESS
#endif

#if (IPV6_PRIV_EXT_CONFIG_COUNT > 1)
# define IpV6_VCfgGetPrivExtTempValidLifetime()          IPV6_CTRL_VAR(IpV6_PrivExtConfig)->PrivExtTempValidLifetime
# define IpV6_VCfgGetPrivExtTempPreferredLifetime()      IPV6_CTRL_VAR(IpV6_PrivExtConfig)->PrivExtTempPreferredLifetime
# define IpV6_VCfgGetPrivExtRegenAdvance()               IPV6_CTRL_VAR(IpV6_PrivExtConfig)->PrivExtRegenAdvance
# define IpV6_VCfgGetPrivExtMaxDesyncFactor()            IPV6_CTRL_VAR(IpV6_PrivExtConfig)->PrivExtMaxDesyncFactor
# define IpV6_VCfgGetPrivExtTempIdGenRetries()           IPV6_CTRL_VAR(IpV6_PrivExtConfig)->PrivExtTempIdGenRetries
# define IpV6_VCfgGetPrivExtMaxTempAddrs()               IPV6_CTRL_VAR(IpV6_PrivExtConfig)->PrivExtMaxTempAddrs
# define IpV6_VCfgIsPrivExtUseOnLinkPrefixes()           IPV6_CTRL_VAR(IpV6_PrivExtConfig)->PrivExtUseOnLinkPrefixes
#endif
#if (IPV6_PRIV_EXT_CONFIG_COUNT == 1)
# define IpV6_VCfgGetPrivExtTempValidLifetime()          IPV6_PRIV_EXT_TEMP_VALID_LIFETIME
# define IpV6_VCfgGetPrivExtTempPreferredLifetime()      IPV6_PRIV_EXT_TEMP_PREFERRED_LIFETIME
# define IpV6_VCfgGetPrivExtRegenAdvance()               IPV6_PRIV_EXT_REGEN_ADVANCE
# define IpV6_VCfgGetPrivExtMaxDesyncFactor()            IPV6_PRIV_EXT_MAX_DESYNC_FACTOR
# define IpV6_VCfgGetPrivExtTempIdGenRetries()           IPV6_PRIV_EXT_TEMP_IDGEN_RETRIES
# define IpV6_VCfgGetPrivExtMaxTempAddrs()               IPV6_PRIV_EXT_MAX_TEMP_ADDRS
# define IpV6_VCfgIsPrivExtUseOnLinkPrefixes()           IPV6_PRIV_EXT_USE_ON_LINK_PREFIXES
#endif

#define IpV6_VCfgIsDynHopLimitEnabled()                  (TRUE == IPV6_CTRL_VAR(IpV6_EnableDynHopLimit))

#define IpV6_VCfgGetDefaultHopLimit()                    IPV6_CTRL_VAR(IpV6_DefaultHopLimit)
#define IpV6_VCfgGetDefaultTrafficClassFlowLabelNbo()    IPV6_CTRL_VAR(IpV6_DefaultTrafficClassFlowLabelNbo)
#define IpV6_VCfgGetDefaultEthIfFramePrio()              IPV6_CTRL_VAR(IpV6_EthCtrlFramePriorityDefault)

#define IpV6_VCfgHasMldConfig()                          (NULL_PTR != IPV6_CTRL_VAR(IpV6_Mld_Data))
#define IpV6_VCfgHasPrivExtConfig()                      (NULL_PTR != IPV6_CTRL_VAR(IpV6_PrivExt_Data))

#define IpV6_VCfgGetIcmpTxMsgBufferSize()                (IPV6_ICMP_TX_MSG_BUFFER_SIZE)

/* If an IPv6 controlelr has a DHCP address, this will always be the last entry in the source address table */
#define IpV6_VCfgGetDhcpCtrlAddrIdx()                    (IpV6_VCfgGetSourceAddressTableSize()-1)

#define IPV6_ADDR_ASSIGNMENT_FORGET   0x00U
#define IPV6_ADDR_ASSIGNMENT_STORE    0x01U

#define IPV6_MAX_ETHIF_FRAME_PRIO        7U

#define IPV6_DAD_FAIL_NS                 1u
#define IPV6_DAD_FAIL_NA                 2u

#if (IPV6_ENABLE_RX_FRAGMENTATION == STD_ON)
# define IPV6_FRAG_STATUS_UNUSED      0xFFU
# define IPV6_FRAG_STATUS_ACTIVE         0U
# define IPV6_FRAG_STATUS_COMPLETE       1U
# define IPV6_FRAG_STATUS_TOO_BIG        2U
# define IPV6_FRAG_STATUS_SEGM_ERROR     3U
# define IPV6_FRAG_STATUS_INV_UNFRAG_LEN 4U
#endif

#if (IPV6_ENABLE_TX_FRAGMENTATION == STD_ON)
#define IPV6_FRAG_TX_STATUS_UNUSED       0U
#define IPV6_FRAG_TX_STATUS_PROVIDED     1U
#define IPV6_FRAG_TX_STATUS_TRANSMIT     2U
#endif

#if (IPV6_ENABLE_RX_DBG_COUNTERS == STD_ON)
# define IPV6_DBG_COUNTER_RX_INC(VAR) (IPV6_CTRL_VAR(IpV6_Dbg_RxCounters).VAR)++
#else
# define IPV6_DBG_COUNTER_RX_INC(VAR)
#endif

#if (IPV6_ENABLE_TX_DBG_COUNTERS == STD_ON)
# define IPV6_DBG_COUNTER_TX_INC(VAR) (IPV6_CTRL_VAR(IpV6_Dbg_TxCounters).VAR)++
#else
# define IPV6_DBG_COUNTER_TX_INC(VAR)
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

# define IPV6_CHANGE_STATE(NEW_STATE) (IPV6_CTRL_VAR(IpV6_CtrlState) = (NEW_STATE))

#if ((IPV6_ENABLE_ETHIF_ADDR_FILTER_API == STD_ON) || (IPV6_ENABLE_MLD == STD_ON))
# define IPV6_VJOIN_MULTICAST_GROUP(IP_CTRL_IDX, ADDR_PTR, JOIN)                 (void)IpV6_VJoinMulticastGroup((IP_CTRL_IDX), (ADDR_PTR), (JOIN))
# define IPV6_VJOIN_SOLICITED_NODE_MULTICAST_GROUP(IP_CTRL_IDX, ADDR_PTR, JOIN)  IpV6_VJoinSolicitedNodeMulticastGroup((IP_CTRL_IDX), (ADDR_PTR), (JOIN))
#else
# define IPV6_VJOIN_MULTICAST_GROUP(IP_CTRL_IDX, ADDR_PTR, JOIN)
# define IPV6_VJOIN_SOLICITED_NODE_MULTICAST_GROUP(IP_CTRL_IDX, ADDR_PTR, JOIN)
#endif

/* Development Error Tracer */
#if (IPV6_DEV_ERROR_REPORT == STD_ON)
# define IpV6_CheckDetErrorReturnVoid(CONDITION, API_ID, ERROR_CODE) { if(!(CONDITION)) { \
  (void)Det_ReportError(IPV6_MODULE_ID, IPV6_VINSTANCE_ID, (API_ID), (ERROR_CODE)); return; }}             /* PRQA S 3458 */ /* MD_MSR_19.4 */

# define IpV6_CheckDetErrorReturnValue(CONDITION, API_ID, ERROR_CODE, RET_VAL) { if(!(CONDITION)) { \
  (void)Det_ReportError(IPV6_MODULE_ID, IPV6_VINSTANCE_ID, (API_ID), (ERROR_CODE)); return (RET_VAL); }}   /* PRQA S 3458 */ /* MD_MSR_19.4 */

# define IpV6_CheckDetErrorContinue(CONDITION, API_ID, ERROR_CODE) { if(!(CONDITION)) { \
  (void)Det_ReportError(IPV6_MODULE_ID, IPV6_VINSTANCE_ID, (API_ID), (ERROR_CODE)); }}                     /* PRQA S 3458 */ /* MD_MSR_19.4 */

# define IpV6_DetReportError(API_ID, ERROR_CODE) (void)Det_ReportError(IPV6_MODULE_ID, IPV6_VINSTANCE_ID, (API_ID), (ERROR_CODE))
#else
# define IpV6_CheckDetErrorReturnVoid(CONDITION, API_ID, ERROR_CODE)
# define IpV6_CheckDetErrorReturnValue(CONDITION, API_ID, ERROR_CODE, RET_VAL)
# define IpV6_CheckDetErrorContinue(CONDITION, API_ID, ERROR_CODE)
# define IpV6_DetReportError(API_ID, ERROR_CODE)
#endif /* (IPV6_DEV_ERROR_REPORT == STD_ON) */

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
# define IpV6_DetReportInternalError(ERROR_CODE) IpV6_DetReportError(IPV6_API_ID_V_INTERNAL_FUNCTION, ERROR_CODE)
#else
# define IpV6_DetReportInternalError(ERROR_CODE)
#endif

#define IPV6_CALC_EXT_HEADER_LENGTH(OPT_LEN_BYTE) (8 + (8 * (OPT_LEN_BYTE)))

/* Timestamp handling functions */

#define IPV6_TIME_ADD_MS(TIME_VAR, MSECS)           IpV6_VTimeAddMs(&(TIME_VAR), (MSECS))
#define IPV6_SET_LIFETIME_S(TIME_VAR, LIFETIME_S, NEXT_EVENT_TIME_VAR_PTR)   IpV6_VSetLifetimeS(&(TIME_VAR), (LIFETIME_S), (NEXT_EVENT_TIME_VAR_PTR))
#define IPV6_TIME_SET_INFINITY(TIME_VAR)            ((TIME_VAR).S = IPV6_LIFETIME_UNLIMITED)

#define IPV6_TIME_IS_LOWER(TIMEA, TIMEB)              (((TIMEA).S < (TIMEB).S) || (((TIMEA).S == (TIMEB).S) && ((TIMEA).Ms < (TIMEB).Ms)))
#define IPV6_TIME_EXPIRED(TIME_VAR)                   (IPV6_TIME_IS_LOWER((TIME_VAR), IpV6_Time))
#define IPV6_SET_CURRENT_TIME(TIME_VAR)               ((TIME_VAR) = IpV6_Time)

#define IPV6_TIME_SET(TIME_VAR, S_VAL, MS_VAL)        do { (TIME_VAR).S = (S_VAL); (TIME_VAR).Ms = (MS_VAL); } while(0)
#define IPV6_SET_EVENT_TIME_MS(TIME_VAR, TIME_MS)     do { (TIME_VAR) = IpV6_Time; IpV6_VTimeAddMs(&(TIME_VAR), (TIME_MS)); } while(0)

#define IPV6_IS_VALID_SOCK_IDX(IDX)                   ((IDX) < IpV6_VCfgGetMaxNumTcpIpSockets())
#define IPV6_IS_VALID_CTRL_IDX(IDX)                   ((IDX) < IpV6_VCfgGetMaxNumCtrls())
#define IPV6_IS_VALID_ADDR_ID(IP_ADDR_ID)             ((IP_ADDR_ID) <= IpV6_VCfgGetMaxAddrId())
#define IPV6_IS_VALID_OR_ANY_ADDR_ID(IP_ADDR_ID)      (IPV6_IS_VALID_ADDR_ID(IP_ADDR_ID) || (IPV6_ADDR_ID_ANY == (IP_ADDR_ID)))

#define IPV6_IS_VALID_SAT_IDX(IDX)           ( (IDX) < IpV6_VCfgGetSourceAddressTableSize() )
#define IPV6_IS_VALID_PL_IDX(IDX)            (((IDX) < IPV6_CTRL_VAR(IpV6_PrefixListEntryCount))         && ((IDX) < IpV6_VCfgGetPrefixListSize()))
#define IPV6_IS_VALID_DC_IDX(IDX)            (((IDX) < IPV6_CTRL_VAR(IpV6_DestinationCacheEntryCount))   && ((IDX) < IpV6_VCfgGetDestinationCacheSize()))
#define IPV6_IS_VALID_NC_IDX(IDX)            (((IDX) < IPV6_CTRL_VAR(IpV6_NeighborCacheEntryCount))      && ((IDX) < IpV6_VCfgGetNeighborCacheSize()))
#define IPV6_IS_VALID_DRL_IDX(IDX)           (((IDX) < IPV6_CTRL_VAR(IpV6_DefaultRouterListEntryCount))  && ((IDX) < IpV6_VCfgGetDefaultRouterListSize()))

#define IPV6_MIN(VALUE1, VALUE2)             (((VALUE1) < (VALUE2)) ? (VALUE1) : (VALUE2))

#define IPV6_CMP_64BIT_PREFIX(ADDR1, ADDR2)               (IpV6_MemCmp32(&(ADDR1).addr[0], &(ADDR2).addr[0], IPV6_PREFIX_LEN_BYTE / 4))
#define IPV6_CMP_64BIT_INTERFACE_IDENTIFIER(ADDR1, ADDR2) (IpV6_MemCmp32(&(ADDR1).addr[IPV6_PREFIX_LEN_BYTE], &(ADDR2).addr[IPV6_PREFIX_LEN_BYTE], (IPV6_INTERFACE_IDENTIFIER_LEN_BYTE / 4)))

#define IPV6_IS_INITIALIZED                  (IpV6_State != IPV6_STATE_UNINIT)

#define IPV6_ADDR_ID_2_CTRL_IDX(ADDR_ID)                              IpV6_AddrId2CtrlAddrMap[(ADDR_ID)].IpCtrlIdx
#define IPV6_ADDR_ID_2_ADDR_IDX(ADDR_ID)                              IpV6_AddrId2CtrlAddrMap[(ADDR_ID)].IpCtrlAddrIdx
#define IPV6_CTRL_ADDR_IS_UNICAST(CTRL_ADDR_IDX)                      ((CTRL_ADDR_IDX)      < IpV6_VCfgGetSourceAddressTableSize())
#define IPV6_CTRL_ADDR_IDX_TO_MULTICAST_ADDR_IDX(CTRL_ADDR_IDX)       ((CTRL_ADDR_IDX)      - IpV6_VCfgGetSourceAddressTableSize())
#define IPV6_MULTICAST_ADDR_IDX_TO_CTRL_ADDR_IDX(MULTICAST_ADDR_IDX)  ((MULTICAST_ADDR_IDX) + IpV6_VCfgGetSourceAddressTableSize())

#define IPV6_GET_CTRL_IDX_FROM_ADDR_ID(IP_CTRL_IDX,   ADDR_ID) ((IP_CTRL_IDX)   = IPV6_ADDR_ID_2_CTRL_IDX(ADDR_ID))
#define IPV6_GET_ADDR_IDX_FROM_ADDR_ID(CTRL_ADDR_IDX, ADDR_ID) ((CTRL_ADDR_IDX) = IPV6_ADDR_ID_2_ADDR_IDX(ADDR_ID))

#if (TCPIP_USE_DUMMY_STATEMENT == STD_ON)
# define IPV6_UNUSED_PARAMETER(VAR_NAME) TCPIP_DUMMY_STATEMENT(VAR_NAME)
#else
# define IPV6_UNUSED_PARAMETER(VAR_NAME)
#endif

#define IPV6_SOCKET_DATA_FLAG_ETHIF_FRAME_PRIO   1u
#define IPV6_SOCKET_DATA_FLAG_HOP_LIMIT          2u
#define IPV6_SOCKET_DATA_FLAG_TRAFFIC_CLASS      4u
#define IPV6_SOCKET_DATA_FLAG_FLOW_LABEL         8u

#define IPV6_GET_SOCKET_ETHIF_FRAME_PRIO(SocketIdx) ((0 != (IpV6_SocketData[SocketIdx].Flags & IPV6_SOCKET_DATA_FLAG_ETHIF_FRAME_PRIO)) ? IpV6_SocketData[SocketIdx].EthIfFramePrio : IpV6_VCfgGetDefaultEthIfFramePrio())
#define IPV6_GET_SOCKET_HOP_LIMIT(SocketIdx)        ((0 != (IpV6_SocketData[SocketIdx].Flags & IPV6_SOCKET_DATA_FLAG_HOP_LIMIT       )) ? IpV6_SocketData[SocketIdx].HopLimit       : IPV6_CTRL_VAR(IpV6_CurHopLimit))

/* Byte-Order conversion macros */
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
  /* Host byte-order is big endian (network byte-order) */
# define IPV6_NTOHS(UINT16_VALUE) (UINT16_VALUE)
# define IPV6_HTONS(UINT16_VALUE) (UINT16_VALUE)
# define IPV6_NTOHL(UINT32_VALUE) (UINT32_VALUE)
# define IPV6_HTONL(UINT32_VALUE) (UINT32_VALUE)

# define IPV6_UINT8_HTONS(UINT8_VALUE) (UINT8_VALUE)
# define IPV6_UINT16_HTONL(UINT16_VALUE) (UINT16_VALUE)
#else
  /* Host byte-order is little endian */
# define IPV6_NTOHS(UINT16_VALUE) ((uint16)( (((UINT16_VALUE) & 0xFF00u) >> 8) | (((UINT16_VALUE) & 0x00FFu) << 8) ))
# define IPV6_HTONS(UINT16_VALUE) IPV6_NTOHS(UINT16_VALUE)
# define IPV6_NTOHL(UINT32_VALUE) ((uint32)( (uint32)(((UINT32_VALUE) & 0xFF000000u) >> 24) | \
                                             (uint32)(((UINT32_VALUE) & 0x00FF0000u) >>  8) | \
                                             (uint32)(((UINT32_VALUE) & 0x0000FF00u) <<  8) | \
                                             (uint32)(((UINT32_VALUE) & 0x000000FFu) << 24) ))
# define IPV6_HTONL(UINT32_VALUE) IPV6_NTOHL(UINT32_VALUE)

# define IPV6_UINT8_HTONS(UINT8_VALUE)   ((uint16)(  ((uint16)(UINT8_VALUE) << 8)))
# define IPV6_UINT16_HTONL(UINT16_VALUE) ((uint32)( (((uint32)(UINT16_VALUE) & 0x0000FF00u) <<  8) | \
                                                    (((uint32)(UINT16_VALUE) & 0x000000FFu) << 24) ))
#endif

#if (IPV6_ENABLE_DLISTS == STD_ON)

# define IPV6_DLIST_GET_NEXT(NodeIdx, DList) ((DList).DescPtr->NodePtr[(NodeIdx)].NextIdx)
# define IPV6_DLIST_GET_PREV(NodeIdx, DList) ((DList).DescPtr->NodePtr[(NodeIdx)].PrevIdx)

# define IPV6_DLIST_SET_NEXT(NodeIdx, DList) ((NodeIdx) = IPV6_DLIST_GET_NEXT(NodeIdx, DList))
# define IPV6_DLIST_SET_PREV(NodeIdx, DList) ((NodeIdx) = IPV6_DLIST_GET_PREV(NodeIdx, DList))

# define IPV6_DLIST_IS_EMPTY(DList)           (IPV6_DLIST_END_IDX == (DList).FirstIdx)

# define IPV6_DLIST_END_IDX 0xFF

#endif

/* PRQA L:FctLikeMacros */

/* Critical section defines */
#define IPV6_BEGIN_CRITICAL_SECTION()     SchM_Enter_TcpIp_TCPIP_EXCLUSIVE_AREA_0()
#define IPV6_END_CRITICAL_SECTION()       SchM_Exit_TcpIp_TCPIP_EXCLUSIVE_AREA_0()

#define IPV6_BEGIN_CRITICAL_SECTION_SAT() IPV6_BEGIN_CRITICAL_SECTION()
#define IPV6_END_CRITICAL_SECTION_SAT()   IPV6_END_CRITICAL_SECTION()

#define IPV6_BEGIN_CRITICAL_SECTION_DC()  IPV6_BEGIN_CRITICAL_SECTION()
#define IPV6_END_CRITICAL_SECTION_DC()    IPV6_END_CRITICAL_SECTION()

#define IPV6_BEGIN_CRITICAL_SECTION_NC()  IPV6_BEGIN_CRITICAL_SECTION()
#define IPV6_END_CRITICAL_SECTION_NC()    IPV6_END_CRITICAL_SECTION()

#define IPV6_BEGIN_CRITICAL_SECTION_PL()  IPV6_BEGIN_CRITICAL_SECTION()
#define IPV6_END_CRITICAL_SECTION_PL()    IPV6_END_CRITICAL_SECTION()

#define IPV6_BEGIN_CRITICAL_SECTION_DRL() IPV6_BEGIN_CRITICAL_SECTION()
#define IPV6_END_CRITICAL_SECTION_DRL()   IPV6_END_CRITICAL_SECTION()

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if (IPV6_ENABLE_RX_DBG_COUNTERS == STD_ON)
typedef struct
{
  uint32 RxTotal;            /* total number of IpV6_RxIndictation calls */
  uint32 RxMulticast;        /* number of received multicast packets */
  uint32 DropInvHdr;         /* number of dropped packets due to invalid header */
  uint32 DropInvSrc;         /* number of dropped packets due invalid source address */
  uint32 DropInvLen;         /* number of dropped packets due invalid length */
  uint32 DropOwn;            /* number of dropped packets sent from this node */
  uint32 DropOtherNode;      /* number of dropped packets destinated to oter node */
  uint32 DropInvNxtHdr;      /* number of dropped packets with invalid/unknown next header value */
  uint32 DropInvNxtHdrLen;   /* number of dropped packets with invalid next header length value */
  uint32 DropHdrOrder;       /* number of dropped packets due to wrong extension header order */
  uint32 DropInvOptLen;      /* number of dropped packets due to invalid option length */
  uint32 DropUknOpt;         /* number of dropped packets due to unknown header option */
} IpV6_Dbg_RxCountersType;
#endif

#if (IPV6_ENABLE_TX_DBG_COUNTERS == STD_ON)
typedef struct
{
  uint32 ProvTxBufTotalCalls;             /* number of calls to IpV6_ProvideTxBuffer() */
  uint32 ProvTxBufFailInvDstAddr;         /* number of times ProvideTxBuffer failed due to invalid destination address */
  uint32 ProvTxBufFailInvSrcAddr;         /* number of times ProvideTxBuffer failed due to invalid source address */
  uint32 ProvTxBufDcHits;                 /* number of destination cache lookup hits */
  uint32 ProvTxBufFailNoDefRouter;        /* number of times ProvideTxBuffer failed due to missing default router */
  uint32 ProvTxBufFailNoSrcAddr;          /* number of times ProvideTxBuffer failed because no valid source address could be found */
  uint32 ProvTxBufNoDefRouterOptimistic;  /* number of packets sent to default router because source was optimistic */
  uint32 ProvTxBufBusyNextHopUnreachable; /* number of times ProvideTxBuffer returned BUFREQ_E_BUSY due to unreachable next hop */
  uint32 ProvTxBufFragmentation;          /* number of provided tx fragment buffers */
  uint32 ProvTxBufBusyNoFragBuf;          /* number of times ProvideTxBuffer returned BUFREQ_E_BUSY due insufficient fragment tx buffers */
  uint32 ProvTxBufFailBufLenTooBig;       /* number of times ProvideTxBuffer failed due to too big fragmented packet */
  uint32 ProvTxBufEthIfBusy;              /* number of times EthIf_ProvideTxBuffer returned BUFREQ_E_BUSY */
  uint32 ProvTxBufEthIfFail;              /* number of times EthIf_ProvideTxBuffer returned BUFREQ_E_NOT_OK */
  uint32 ProvTxBufSuccess;                /* number of successful provide tx buffers */
  uint32 TransmitCalls;                   /* number of calls to IpV6_Transmit() */
  uint32 TransmitFailures;                /* number of times IpV6_Transmit failed */
  uint32 TransmitSuccess;                 /* number of times IpV6_Transmit succeeded */
} IpV6_Dbg_TxCountersType;
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define IPV6_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern VAR(uint8, IPV6_VAR_ZERO_INIT) IpV6_State;

#define IPV6_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**/

#define IPV6_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define IPV6_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**/

#define IPV6_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define IPV6_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**/

#define IPV6_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define IPV6_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**/

#define IPV6_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern VAR(IpV6_TimeType, IPV6_VAR_NOINIT) IpV6_Time;

#define IPV6_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define IPV6_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * IpV6_VGetSolicitedNodeAddr
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VGetSolicitedNodeAddr(
  IPV6_P2V(IpV6_AddrType) MulticastAddrPtr,
  IPV6_P2C(IpV6_AddrType) NetAddrPtr,
  IpV6_MulticastScopeType Scope);

/**********************************************************************************************************************
 *  IpV6_MemSet()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_MemSet(
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_DATA) TargetPtr, uint8 Value, uint16 Length);

/**********************************************************************************************************************
 *  IpV6_MemCmp32()
 **********************************************************************************************************************/
FUNC(boolean, IPV6_CODE) IpV6_MemCmp32(
  IPV6_P2C(uint8) Data1Ptr,
  IPV6_P2C(uint8) Data2Ptr,
  uint8 Blocks);

/**********************************************************************************************************************
 *  IpV6_VGetAddrType()
 **********************************************************************************************************************/
FUNC(IpV6_AddrTypeType, IPV6_CODE) IpV6_VGetAddrType(
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) AddrPtr);

/* PREFIX LIST FUNCTIONS */

/**********************************************************************************************************************
 *  IpV6_VMatchPrefix()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_VMatchPrefix(
  IPV6_P2C(IpV6_AddrType) AddrPtr,
  IPV6_P2C(uint8)         PrefixPtr,
  uint8                   PrefixLenBits);

/**********************************************************************************************************************
 *  IpV6_VIsStaticOnLinkPrefix()
 **********************************************************************************************************************/
/*! \brief      Determines whether a prefix is coverd by a statically configured prefix in the on link prefix list.
 *  \param[in]  IpCtrlIdx     IP controller index
 *  \param[in]  PrefixPtr     Prefix that shall be searched in the prefix list.
 *  \param[in]  PrefixBitLen  Length (in bits) of the prefix at PrefixPtr.
 *  \return     TRUE  - Prefix is covered by one of the entries in the static on link prefix list.
 *  \return     FALSE - Prefix is not covered by any entry in the static on link prefix list.
 **********************************************************************************************************************/
FUNC(boolean, IPV6_CODE) IpV6_VIsStaticOnLinkPrefix(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) PrefixPtr,
  uint8                   PrefixBitLen);

/**********************************************************************************************************************
 *  IpV6_VIsDestinationOnLink()
 **********************************************************************************************************************/
FUNC(boolean, IPV6_CODE) IpV6_VIsDestinationOnLink(
  uint8                                             IpCtrlIdx,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) DstAddrPtr);

/**********************************************************************************************************************
 *  IpV6_VIsValidSourceAddress()
 **********************************************************************************************************************/
/*! \brief      Check if a source address belongs to this node and is in state PREFERRED or DEPRECATED.
 *              Other addresses must not be used as source address for outgoing packets.
 *  \param[in]  IpCtrlIdx       ip controller index
 *  \param[in]  SrcAddrPtr      source address
 *  \param[in]  AllowUnspecAddr if TRUE the unspecified (::) address will be accepted as valid source address
 *  \param[out] SrcAddrStatePtr specifies the status of the source address
 *  \param[out] CtrlAddrIdxPtr  specifies the controller-specific address index. Afterwards caller must check validity!
 *  \return     TRUE       address belongs to this node and may be used as source address for outgoing packets
 *  \return     FALSE      address not valid as source address for outgoing packets
 *  \pre        -
 *  \context    Call MUST be protected with IPV6_BEGIN_CRITICAL_SECTION_SAT() or IPV6_BEGIN_CRITICAL_SECTION()
 **********************************************************************************************************************/
FUNC(boolean, IPV6_CODE) IpV6_VIsValidSourceAddress(
  uint8                           IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType)         SrcAddrPtr,
  boolean                         AllowUnspecAddr,
  IPV6_P2V(IpV6_LocAddrStateType) SrcAddrStatePtr,
  IPV6_P2V(IpV6_CtrlAddrIdxType)  CtrlAddrIdxPtr);

/* DEFAULT ROUTER TABLE FUNCTIONS */

/**********************************************************************************************************************
 *  IpV6_VGetDefaultRouterAddr()
 **********************************************************************************************************************/
FUNC(P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA), IPV6_CODE) IpV6_VGetDefaultRouterAddr(
  uint8                   IpCtrlIdx,
  IPV6_P2C(IpV6_AddrType) DstAddrPtr,
  IPV6_P2V(boolean)       IsReachable);

/* SOURCE ADDRESS HANDLING FUNCTIONS */

/**********************************************************************************************************************
 *  IpV6_VBuildSourceAddress()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VBuildSourceAddress(
  P2CONST(uint8, AUTOMATIC, IPV6_APPL_DATA) IfIdPtr,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) PrefixPtr,
  P2VAR(IpV6_AddrType, AUTOMATIC, IPV6_APPL_VAR) SrcAddrPtr);

/***********************************************************************************************************************
 *  IpV6_VSATCreateEntry()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_VSATCreateEntry(
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) SrcAddrPtr,
  P2VAR(uint8, AUTOMATIC, IPV6_APPL_VAR) EntryIdxPtr);

/**********************************************************************************************************************
 *  IpV6_VSATLookupPrefix()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_VSATLookupPrefix(
  uint8                          IpCtrlIdx,
  IPV6_P2C(uint8)                AddrPrefixPtr,
  IPV6_P2V(IpV6_CtrlAddrIdxType) CtrlAddrIdxPtr,
  uint8                          AddrCfgSrc);

/***********************************************************************************************************************
 *  IpV6_VSATLookup()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_VSATLookup(
  uint8                                                 IpCtrlIdx,
  P2CONST(IpV6_AddrType,      AUTOMATIC, IPV6_APPL_VAR) SrcAddrPtr,
  P2VAR(IpV6_CtrlAddrIdxType, AUTOMATIC, IPV6_APPL_VAR) CtrlAddrIdxPtr
#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
  , P2VAR(IpV6_DListIdxType, AUTOMATIC, IPV6_APPL_VAR)  SatTempAddrIdxPtr
#endif
  );

/**********************************************************************************************************************
 *  IpV6_VSATRemoveEntry()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VSATRemoveEntry(uint8 IpCtrlIdx, IpV6_CtrlAddrIdxType CtrlAddrIdx);

/**********************************************************************************************************************
 *  IpV6_VSATRemoveAllEntrys()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VSATRemoveAllEntrys(uint8 IpCtrlIdx);

/**********************************************************************************************************************
 *  IpV6_VHandleDADFailure()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VHandleDADFailure(
  TCPIP_P2C(uint8)  PhysAddrPtr,
  IpV6_AddrIdType   AddrId,
  uint8             EventType);

#if (IPV6_ENABLE_PRIVACY_EXTENSIONS == STD_ON)
/**********************************************************************************************************************
 *  IpV6_VHandleTempAddrDADFailure()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_VHandleTempAddrDADFailure(uint8 IpCtrlIdx, IpV6_ListIdxType SatEntryIdx, IpV6_DListIdxType TempAddrIdx, uint8 EventType);
#endif

/**********************************************************************************************************************
 *  IpV6_VHandleSourceAddrAssignmentChange()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VHandleSourceAddrAssignmentChange(uint8 IpCtrlIdx, uint8 CtrlAddrIdx, boolean AddrAssigned);

#if ((IPV6_ENABLE_ETHIF_ADDR_FILTER_API == STD_ON) || (IPV6_ENABLE_MLD == STD_ON))
/**********************************************************************************************************************
 *  IpV6_VJoinMulticastGroup()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_VJoinMulticastGroup(
  uint8                                             IpCtrlIdx,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) MulticastAddrPtr,
  boolean                                           Join);

/**********************************************************************************************************************
 *  IpV6_VJoinSolicitedNodeMulticastGroup()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VJoinSolicitedNodeMulticastGroup(
  uint8                                             IpCtrlIdx,
  P2CONST(IpV6_AddrType, AUTOMATIC, IPV6_APPL_DATA) UnicastAddrPtr,
  boolean                                           Join);
#endif

#if (IPV6_ENABLE_RND_FUNCTIONS == STD_ON)
/**********************************************************************************************************************
 *  IpV6_VGetRandomValue()
 **********************************************************************************************************************/
FUNC(uint32, IPV6_CODE) IpV6_VGetRandomValue(uint32 MinValue, uint32 MaxValue);

/**********************************************************************************************************************
 *  IpV6_VRandomizeValue()
 **********************************************************************************************************************/
FUNC(uint32, IPV6_CODE) IpV6_VRandomizeValue(
  uint8  IpCtrlIdx,
  uint32 BaseValue);
#endif /* (IPV6_ENABLE_RND_FUNCTIONS == STD_ON) */

/* TIMESTAMP FUNCTIONS */

/**********************************************************************************************************************
 *  IpV6_VTimeAddMs()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VTimeAddMs(
  P2VAR(IpV6_TimeType, AUTOMATIC, IPV6_APPL_DATA) TimestampPtr,
  uint32 Milliseconds);

/**********************************************************************************************************************
 *  IpV6_VSetLifetimeS()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_VSetLifetimeS(
  P2VAR(IpV6_TimeType, AUTOMATIC, IPV6_APPL_VAR) TimestampPtr,
  uint32 LifetimeS,
  P2VAR(IpV6_TimeType, AUTOMATIC, IPV6_APPL_VAR) NextEventTimePtr);

#if (IPV6_ENABLE_MTU_RECONFIGURATION == STD_ON)
FUNC(void, IPV6_CODE) IpV6_VUpdateDefaultLinkMtu(uint8 IpCtrlIdx, uint16 Mtu);
#endif

#if (IPV6_ENABLE_DLISTS == STD_ON)

/* DList public functions */

/**********************************************************************************************************************
 *  IpV6_DListInitDesc()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_DListInitDesc(
  IPV6_P2V(IpV6_DListDescType)  DescPtr,
  IPV6_P2V(IpV6_DListNodeType)  NodePtr,
  uint8                         NodeCount);

/**********************************************************************************************************************
 *  IpV6_DListInit()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_DListInit(
  IPV6_P2V(IpV6_DListType)      ListPtr,
  IPV6_P2V(IpV6_DListDescType)  DescPtr);

/**********************************************************************************************************************
 *  IpV6_DListDiff()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_DListDiff(
  IPV6_P2V(IpV6_DListType)      ListAPtr,
  IPV6_P2V(IpV6_DListType)      ListBPtr,
  IPV6_P2V(IpV6_DListType)      ListBothPtr);

/**********************************************************************************************************************
 *  IpV6_DListAddElement()
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_DListAddElement(
  IPV6_P2V(IpV6_DListType)      DListPtr,
  IpV6_DListIdxType             AfterIdx,
  IPV6_P2V(IpV6_DListIdxType)   NewIdxPtr);

/**********************************************************************************************************************
 *  IpV6_DListRemoveElement()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_DListRemoveElement(
  IPV6_P2V(IpV6_DListType)      DListPtr,
  IpV6_DListIdxType             RemIdx);

/**********************************************************************************************************************
 *  IpV6_DListRemoveElementRange()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_DListRemoveElementRange(
  IPV6_P2V(IpV6_DListType)      DListPtr,
  IpV6_DListIdxType             RemStartIdx,
  IpV6_DListIdxType             RemEndIdx);

/**********************************************************************************************************************
 *  IpV6_DListMoveElement()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_DListMoveElement(
  IPV6_P2V(IpV6_DListType)      SrcListPtr,
  IpV6_DListIdxType             SrcListIdx,
  IPV6_P2V(IpV6_DListType)      TgtListPtr,
  IpV6_DListIdxType             TgtPrevIdx);

/**********************************************************************************************************************
 *  IpV6_DListMoveElementRange()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_DListMoveElementRange(
  IPV6_P2V(IpV6_DListType)      SrcListPtr,
  IpV6_DListIdxType             SrcListStartIdx,
  IpV6_DListIdxType             SrcListEndIdx,
  IPV6_P2V(IpV6_DListType)      TgtListPtr,
  IpV6_DListIdxType             TgtPrevIdx);

/**********************************************************************************************************************
 *  IpV6_DListClear()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_DListClear(
  IPV6_P2V(IpV6_DListType)      DListPtr);

/**********************************************************************************************************************
 *  IpV6_DListSwap()
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_DListSwap(
  IPV6_P2V(IpV6_DListType)      ListAPtr,
  IPV6_P2V(IpV6_DListType)      ListBPtr);

#endif

#define IPV6_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* (TCPIP_SUPPORT_IPV6 == STD_ON) */
#endif  /* IPV6_PRIV_H */

/**********************************************************************************************************************
 *  END OF FILE: IpV6_Priv.h
 *********************************************************************************************************************/
