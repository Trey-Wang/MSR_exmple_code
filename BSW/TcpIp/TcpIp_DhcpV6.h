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
 *         \file  TcpIp_DhcpV6.h
 *        \brief  Implementation of Dynamic Host Configuration Protocol for IPv6 - Client (DHCPv6 Client)
 *
 *      \details  This file is part of the TcpIp IPv6 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv6 submodule. >> TcpIp_IpV6.h
 *********************************************************************************************************************/

#if !defined (TCPIP_DHCPV6_H)
# define TCPIP_DHCPV6_H

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV6 == STD_ON) && (TCPIP_SUPPORT_DHCPV6 == STD_ON)

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "TcpIp_Lcfg.h"
#include "EthIf.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

#define TCPIP_DHCPV6_VENDOR_ID                 30U   /* Vector ID */
#define TCPIP_DHCPV6_MODULE_ID                  TCPIP_MODULE_ID
#define TCPIP_DHCPV6_VINSTANCE_ID               TCPIP_VINSTANCE_ID

/* DHCPv6 Message Format:
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |    msg-type   |               transaction-id                  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                                               |
 * .                            options                            .
 * .                           (variable)                          .
 * |                                                               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *                        [RFC3315 6. Client/Server Message Formats]
 */
#define TCPIP_DHCPV6_MSG_HDR_LEN                4U

#define TCPIP_DHCPV6_MSG_SOL                    TCPIP_DHCPV6_MSG_SOLICIT             /* RFC3315 15.2.  */
#define TCPIP_DHCPV6_MSG_ADV                    TCPIP_DHCPV6_MSG_ADVERTISE           /* RFC3315 15.3.  */
#define TCPIP_DHCPV6_MSG_REQ                    TCPIP_DHCPV6_MSG_REQUEST             /* RFC3315 15.4.  */
#define TCPIP_DHCPV6_MSG_CNF                    TCPIP_DHCPV6_MSG_CONFIRM             /* RFC3315 15.5.  */
#define TCPIP_DHCPV6_MSG_REN                    TCPIP_DHCPV6_MSG_RENEW               /* RFC3315 15.6.  */
#define TCPIP_DHCPV6_MSG_REB                    TCPIP_DHCPV6_MSG_REBIND              /* RFC3315 15.7.  */
#define TCPIP_DHCPV6_MSG_REP                    TCPIP_DHCPV6_MSG_REPLY               /* RFC3315 15.10. */
#define TCPIP_DHCPV6_MSG_REL                    TCPIP_DHCPV6_MSG_RELEASE             /* RFC3315 15.9.  */
#define TCPIP_DHCPV6_MSG_DEC                    TCPIP_DHCPV6_MSG_DECLINE             /* RFC3315 15.8.  */
#define TCPIP_DHCPV6_MSG_REC                    TCPIP_DHCPV6_MSG_RECONFIGURE         /* RFC3315 15.11. */ /* not supported */
#define TCPIP_DHCPV6_MSG_INF                    TCPIP_DHCPV6_MSG_INFORMATION_REQUEST /* RFC3315 15.12. */
#define TCPIP_DHCPV6_MSG_RELAY_FORW             TCPIP_DHCPV6_MSG_RELAY_FORWARD       /* RFC3315 15.13. */ /* not supported */
#define TCPIP_DHCPV6_MSG_RELAY_REPL             TCPIP_DHCPV6_MSG_RELAY_REPLY         /* RFC3315 15.14. */ /* not supported */

/* [RFC3315 5.5. Transmission and Retransmission Parameters] */

/* [17.1.2. Transmission of Solicit Messages] */
#define TCPIP_DHCPV6_SOL_TIMEOUT             1000U /* [msec]   (1.000) Initial Solicit timeout */
#define TCPIP_DHCPV6_SOL_MAX_RT            120000U /* [msec] (120.000) Max Solicit timeout value */
#define TCPIP_DHCPV6_SOL_MAX_RC                 0U /*    [#]       (0) */
#define TCPIP_DHCPV6_SOL_MAX_RD                 0U /* [msec]       (0) */

/* [18.1.1. Creation and Transmission of Request Messages] */
#define TCPIP_DHCPV6_REQ_TIMEOUT             1000U /* [msec]   (1.000) Initial Request timeout */
#define TCPIP_DHCPV6_REQ_MAX_RT             30000U /* [msec]  (30.000) Max Request timeout value */
#define TCPIP_DHCPV6_REQ_MAX_RC                10U /*    [#]      (10) Max Request retry attempts */
#define TCPIP_DHCPV6_REQ_MAX_RD                 0U /* [msec]       (0) */

/* [18.1.2. Creation and Transmission of Confirm Messages] */
#define TCPIP_DHCPV6_CNF_TIMEOUT             1000U /* [msec]   (1.000) Initial Confirm timeout */
#define TCPIP_DHCPV6_CNF_MAX_RT              4000U /* [msec]   (4.000) Max Confirm timeout */
#define TCPIP_DHCPV6_CNF_MAX_RC                 0U /*    [#]       (0) */
#define TCPIP_DHCPV6_CNF_MAX_RD             10000U /* [msec]  (10.000) Max Confirm duration */

/* [18.1.3. Creation and Transmission of Renew Messages] */
#define TCPIP_DHCPV6_REN_TIMEOUT            10000U /* [msec]  (10.000) Initial Renew timeout */
#define TCPIP_DHCPV6_REN_MAX_RT            600000U /* [msec] (600.000) Max Renew timeout value */
#define TCPIP_DHCPV6_REN_MAX_RC                 0U /* [#]          (0) */
#define TCPIP_DHCPV6_REN_MAX_RD                 0U /* [msec] (Remaining time until T2) */

/* [18.1.4. Creation and Transmission of Rebind Messages] */
#define TCPIP_DHCPV6_REB_TIMEOUT            10000U /* [msec]  (10.000) Initial Rebind timeout */
#define TCPIP_DHCPV6_REB_MAX_RT            600000U /* [msec] (600.000) Max Rebind timeout value */
#define TCPIP_DHCPV6_REB_MAX_RC                 0U /* [#]          (0) */
#define TCPIP_DHCPV6_REB_MAX_RD                 0U /* [msec] (Remaining time until valid lifetimes of all addresses have expired) */

/* [18.1.5. Creation and Transmission of Information-request Messages] */
#define TCPIP_DHCPV6_INF_TIMEOUT             1000U /* [msec]   (1.000) Initial Information-request timeout */
#define TCPIP_DHCPV6_INF_MAX_RT            120000U /* [msec] (120.000) Max Information-request timeout value */
#define TCPIP_DHCPV6_INF_MAX_RC                 0U /* [#]          (0) */
#define TCPIP_DHCPV6_INF_MAX_RD                 0U /* [msec]       (0) */

/* [18.1.6. Creation and Transmission of Release Messages] */
#define TCPIP_DHCPV6_REL_TIMEOUT             1000U /* [msec]   (1.000) Initial Release timeout */
#define TCPIP_DHCPV6_REL_MAX_RT                 0U /* [msec]       (0) */
#define TCPIP_DHCPV6_REL_MAX_RC                 5U /*    [#]       (5) MAX Release attempts */
#define TCPIP_DHCPV6_REL_MAX_RD                 0U /* [msec]       (0) */

/* [18.1.7. Creation and Transmission of Decline Messages] */
#define TCPIP_DHCPV6_DEC_TIMEOUT             1000U /* [msec]   (1.000) Initial Decline timeout */
#define TCPIP_DHCPV6_DEC_MAX_RT                 0U /* [msec]       (0) */
#define TCPIP_DHCPV6_DEC_MAX_RC                 5U /*    [#]       (5) Max Decline attempts */
#define TCPIP_DHCPV6_DEC_MAX_RD                 0U /* [msec]       (0) */

/* DHCPv6 Option Format:
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |          option-code          |           option-len          |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                          option-data                          |
 * |                      (option-len octets)                      |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *                            [RFC3315 22.1. Format of DHCP Options]
 */
#define TCPIP_DHCPV6_OPT_HDR_LEN                4U /* each options begins with 2 byte code and 2 byte len */

/* DHCPv6 option codes and lengths */
#define TCPIP_DHCPV6_OPT_CLIENTID               1U /*               [RFC3315 22.2.  Client Identifier Option]                                */
#define TCPIP_DHCPV6_OPT_SERVERID               2U /*               [RFC3315 22.3.  Server Identifier Option]                                */
#define TCPIP_DHCPV6_OPT_IA_NA                  3U /*               [RFC3315 22.4.  Identity Association for Non-temporary Addresses Option] */
#define TCPIP_DHCPV6_OPT_IA_NA_HDR_LEN         12U /*               (fixed header + variable options)                                        */
#define TCPIP_DHCPV6_OPT_IA_TA                  4U /* -unsupported- [RFC3315 22.5.  Identity Association for Temporary Addresses Option]     */
#define TCPIP_DHCPV6_OPT_IA_TA_HDR_LEN          4U /*               (fixed header + variable options)                                        */
#define TCPIP_DHCPV6_OPT_IAADDR                 5U /*               [RFC3315 22.6.  IA Address Option]                                       */
#define TCPIP_DHCPV6_OPT_IAADDR_HDR_LEN        24U /*               (fixed header + variable options)                                        */
#define TCPIP_DHCPV6_OPT_ORO                    6U /*               [RFC3315 22.7.  Option Request Option]                                   */
#define TCPIP_DHCPV6_OPT_PREFERENCE             7U /*               [RFC3315 22.8.  Preference Option]                                       */
#define TCPIP_DHCPV6_OPT_PREFERENCE_LEN         1U /*               (fixed length)                                                           */
#define TCPIP_DHCPV6_OPT_ELAPSED_TIME           8U /*               [RFC3315 22.9.  Elapsed Time Option]                                     */
#define TCPIP_DHCPV6_OPT_ELAPSED_TIME_LEN       2U /*               (fixed length)                                                           */
#define TCPIP_DHCPV6_OPT_RELAY_MSG              9U /* -unsupported- [RFC3315 22.10. Relay Message Option]                                    */
#define TCPIP_DHCPV6_OPT_AUTH                  11U /* -unsupported- [RFC3315 22.11. Authentication Option]                                   */
#define TCPIP_DHCPV6_OPT_AUTH_HDR_LEN          11U /*               (fixed header + variable options)                                        */
#define TCPIP_DHCPV6_OPT_UNICAST               12U /* -unsupported- [RFC3315 22.12. Server Unicast Option]                                   */
#define TCPIP_DHCPV6_OPT_UNICAST_LEN           16U /*               (fixed length)                                                           */
#define TCPIP_DHCPV6_OPT_STATUS_CODE           13U /*               [RFC3315 22.13. Status Code Option]                                      */
#define TCPIP_DHCPV6_OPT_STATUS_CODE_HDR_LEN    2U /*               (fixed header + variable options)                                        */
#define TCPIP_DHCPV6_OPT_RAPID_COMMIT          14U /* -unsupported- [RFC3315 22.14. Rapid Commit Option]                                     */
#define TCPIP_DHCPV6_OPT_RAPID_COMMIT_LEN       0U /*               (fixed length)                                                           */
#define TCPIP_DHCPV6_OPT_USER_CLASS            15U /* -unsupported- [RFC3315 22.15. User Class Option]                                       */
#define TCPIP_DHCPV6_OPT_VENDOR_CLASS          16U /* -unsupported- [RFC3315 22.16. Vendor Class Option]                                     */
#define TCPIP_DHCPV6_OPT_VENDOR_OPTS           17U /* -unsupported- [RFC3315 22.17. Vendor-specific Information Option]                      */
#define TCPIP_DHCPV6_OPT_INTERFACE_ID          18U /* -unsupported- [RFC3315 22.18. Interface-Id Option]                                     */
#define TCPIP_DHCPV6_OPT_RECONF_MSG            19U /* -unsupported- [RFC3315 22.19. Reconfigure Message Option]                              */
#define TCPIP_DHCPV6_OPT_RECONF_ACCEPT         20U /* -unsupported- [RFC3315 22.20. Reconfigure Accept Option]                               */

#define TCPIP_DHCPV6_OPT_DNS_SERVERS           23U /*               [RFC3646  3. DNS Recursive Name Server option]                           */
#define TCPIP_DHCPV6_OPT_DOMAIN_LIST           24U /*               [RFC3646  4. Domain Search List option]                                  */

#define TCPIP_DHCPV6_OPT_IA_PD                 25U /* -unsupported- [RFC3633  9. Identity Association for Prefix Delegation Option]          */
#define TCPIP_DHCPV6_OPT_IAPREFIX              26U /* -unsupported- [RFC3633 10. IA_PD Prefix option]                                        */

#define TCPIP_DHCPV6_MAX_PREFERENCE           255U

/* [RFC3315 24.4. Status Codes] */
#define TCPIP_DHCPV6_STATUS_CODE_SUCCESS        0U /* "Success." */
#define TCPIP_DHCPV6_STATUS_CODE_UNSPEC_FAIL    1U /* "Failure, reason unspecified; this status code is sent by either
                                                       a client or a server to indicate a failure not explicitly
                                                       specified in this document." */
#define TCPIP_DHCPV6_STATUS_CODE_NO_ADDRS_AVAIL 2U /* "Server has no addresses available to assign to the IA(s)." */
#define TCPIP_DHCPV6_STATUS_CODE_NO_BINDING     3U /* "Client record (binding) unavailable." */
#define TCPIP_DHCPV6_STATUS_CODE_NOT_ON_LINK    4U /* "The prefix for the address is not appropriate for the link to
                                                       which the client is attached." */
#define TCPIP_DHCPV6_STATUS_CODE_USE_MULTICAST  5U /* "Sent by a server to a client to force the client to send messages
                                                       to the server. using the All_DHCP_Relay_Agents_and_Servers address." */

#define TCPIP_DHCPV6_CLIENT_PORT              546U /* DHCPv6 client port according to [RFC3315 5.2. UDP Ports] */
#define TCPIP_DHCPV6_SERVER_PORT              547U /* DHCPv6 server port according to [RFC3315 5.2. UDP Ports] */

#define TCPIP_DHCPV6_DUID_TYPE_1                1U
#define TCPIP_DHCPV6_DUID_TYPE_2                2U /* not supported */
#define TCPIP_DHCPV6_DUID_TYPE_3                3U /* not supported */

#define TCPIP_DHCPV6_LIFETIME_UNLIMITED 0xFFFFFFFFU

/* DHCPv6 states */

/* \/ No active lease */
#define TCPIP_DHCPV6_STATE_UNINIT            0x00U
#define TCPIP_DHCPV6_STATE_INIT              0x01U

#define TCPIP_DHCPV6_STATE_UNBOUND           0xB0U /* No Active Lease */

#define TCPIP_DHCPV6_STATE_TX_SOL            0xB1U /* Tx Solicit Message */
#define TCPIP_DHCPV6_STATE_TX_REQ            0xB2U /* Tx Request Message */
#define TCPIP_DHCPV6_STATE_TX_INF            0xB3U /* Tx Information Request Message */

#define TCPIP_DHCPV6_STATE_TX_DEC            0xB4U /* Tx Decline Message */

/* \/ Active lease */
#define TCPIP_DHCPV6_STATE_TX_REN            0xB5U /* Tx Renew Message */
#define TCPIP_DHCPV6_STATE_TX_REB            0xB6U /* Tx Rebind Message */
#define TCPIP_DHCPV6_STATE_TX_CNF            0xB7U /* Tx Confirm Message */
#define TCPIP_DHCPV6_STATE_TX_REL            0xB8U /* Tx Release Message */

#define TCPIP_DHCPV6_STATE_BOUND             0xBDU /* Active lease Idle */

#define TCPIP_DHCPV6_STATE_FAIL              0xE0U

/* DHCPv6 ApiIds */
#define TCPIP_DHCPV6_API_ID_INIT               0xF0U
#define TCPIP_DHCPV6_API_ID_RX_INDICATION      0xF1U
#define TCPIP_DHCPV6_API_ID_TX_CONFIRMATION    0xF2U
#define TCPIP_DHCPV6_API_ID_START              0xF3U
#define TCPIP_DHCPV6_API_ID_RELEASE            0xF4U
#define TCPIP_DHCPV6_API_ID_MAIN_FUNCTION      0xF5U
#define TCPIP_DHCPV6_API_ID_IP_ASSIGNMENT_CHG  0xF6U
#define TCPIP_DHCPV6_API_ID_INTERNAL_FUNCTION  0xFFU

/* TCPIP DET errors */
#define TCPIP_DHCPV6_E_NOT_INITIALIZED       0x01U
#define TCPIP_DHCPV6_E_INV_POINTER           0x02U
#define TCPIP_DHCPV6_E_INV_PARAM             0x03U
#define TCPIP_DHCPV6_E_INV_SCK_HND           0x04U

/* DHCPv6 ErrorIds */
#define TCPIP_DHCPV6_E_SERVER_ID_OVFL        0x10U

#define TCPIP_DHCPV6_MAX_STATUS_MSG_LEN       100U

/* integer multiplication and division */
#define TCPIP_DHCPV6_RSHIFT_DIV_2               1U /* devide integer by  2 using right bitshift */
#define TCPIP_DHCPV6_RSHIFT_DIV_4               2U /* devide integer by  4 using right bitshift */
#define TCPIP_DHCPV6_RSHIFT_DIV_8               3U /* devide integer by  8 using right bitshift */
#define TCPIP_DHCPV6_RSHIFT_DIV_16              4U /* devide integer by 16 using right bitshift */

#define TCPIP_DHCPV6_LSHIFT_MUL_2               1U /* multiply integer by  2 using left bitshift */
#define TCPIP_DHCPV6_LSHIFT_MUL_4               2U /* multiply integer by  4 using left bitshift */
#define TCPIP_DHCPV6_LSHIFT_MUL_8               3U /* multiply integer by  8 using left bitshift */
#define TCPIP_DHCPV6_LSHIFT_MUL_16              4U /* multiply integer by 16 using left bitshift */

#define TCPIP_DHCPV6_MASK_MOD_2              0x01U /* use mask instead of modulo division by  2 */
#define TCPIP_DHCPV6_MASK_MOD_4              0x03U /* use mask instead of modulo division by  4 */
#define TCPIP_DHCPV6_MASK_MOD_8              0x07U /* use mask instead of modulo division by  8 */
#define TCPIP_DHCPV6_MASK_MOD_16             0x0FU /* use mask instead of modulo division by 16 */

#define TCPIP_DHCPV6_GET_OPT_OK                 0U
#define TCPIP_DHCPV6_GET_OPT_INV_HDR_LENGTH     1U
#define TCPIP_DHCPV6_GET_OPT_INV_LENGTH         2U

#define TCPIP_DHCPV6_ENABLE_DYNAMIC_IA_NA        STD_OFF /* currently unsupported feature */

/* PRQA S 3453 FctLikeMacros */ /* Macros improve code readability */

/* Timestamp handling functions */

#define TCPIP_DHCPV6_TIME_ADD_MS(TIME_VAR, MSECS)           TcpIp_DhcpV6_VTimeAddMs(&(TIME_VAR), (MSECS))
#define TCPIP_DHCPV6_SET_LIFETIME_S(TIME_VAR, LIFETIME_S, NEXT_EVENT_TIME_VAR_PTR)   TcpIp_DhcpV6_VSetLifetimeS(&(TIME_VAR), (LIFETIME_S), (NEXT_EVENT_TIME_VAR_PTR))

#define TCPIP_DHCPV6_TIME_IS_LOWER(TIMEA, TIMEB)              (((TIMEA).S < (TIMEB).S) || (((TIMEA).S == (TIMEB).S) && ((TIMEA).Ms < (TIMEB).Ms)))
#define TCPIP_DHCPV6_TIME_EXPIRED(TIME_VAR)                   (IPV6_TIME_IS_LOWER((TIME_VAR), TcpIp_DhcpV6_Time))
#define TCPIP_DHCPV6_SET_CURRENT_TIME(TIME_VAR)               ((TIME_VAR) = TcpIp_DhcpV6_Time)

#define TCPIP_DHCPV6_TIME_SET(TIME_VAR, S_VAL, MS_VAL)        do { (TIME_VAR).S = (S_VAL); (TIME_VAR).Ms = (MS_VAL); } while(0)
#define TCPIP_DHCPV6_SET_EVENT_TIME_MS(TIME_VAR, TIME_MS)     do { (TIME_VAR) = TcpIp_DhcpV6_Time; TcpIp_DhcpV6_VTimeAddMs(&(TIME_VAR), (TIME_MS)); } while(0)

#define TCPIP_DHCPV6_SET_IDLE_TIME_MS(TIME_MS)                do { TCPIP_DHCPV6_STATUS_VAR(IdleTime) = TcpIp_DhcpV6_Time; TcpIp_DhcpV6_VTimeAddMs(&TCPIP_DHCPV6_STATUS_VAR(IdleTime), (TIME_MS)); } while(0)
#define TCPIP_DHCPV6_SET_IDLE_TIME_S(TIME_VAL)                do { TCPIP_DHCPV6_STATUS_VAR(IdleTime) = TcpIp_DhcpV6_Time; TCPIP_DHCPV6_STATUS_VAR(IdleTime).S += (TIME_VAL); } while(0)

#define TCPIP_DHCPV6_IS_INITIALIZED() (TCPIP_DHCPV6_STATE_INIT <= TCPIP_DHCPV6_STATUS_VAR(State))

/* Development Error Tracer */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
# define TcpIp_DhcpV6_CheckDetErrorReturnVoid(CONDITION, API_ID, ERROR_CODE) { if(!(CONDITION)) { \
  (void)Det_ReportError(TCPIP_DHCPV6_MODULE_ID, TCPIP_DHCPV6_VINSTANCE_ID, (API_ID), (ERROR_CODE)); return; }}             /* PRQA S 3458 */ /* MD_MSR_19.4 */

# define TcpIp_DhcpV6_CheckDetErrorReturnValue(CONDITION, API_ID, ERROR_CODE, RET_VAL) { if(!(CONDITION)) { \
  (void)Det_ReportError(TCPIP_DHCPV6_MODULE_ID, TCPIP_DHCPV6_VINSTANCE_ID, (API_ID), (ERROR_CODE)); return (RET_VAL); }}   /* PRQA S 3458 */ /* MD_MSR_19.4 */

# define TcpIp_DhcpV6_CheckDetErrorContinue(CONDITION, API_ID, ERROR_CODE) { if(!(CONDITION)) { \
  (void)Det_ReportError(TCPIP_DHCPV6_MODULE_ID, TCPIP_DHCPV6_VINSTANCE_ID, (API_ID), (ERROR_CODE)); }}                     /* PRQA S 3458 */ /* MD_MSR_19.4 */

# define TcpIp_DhcpV6_DetReportError(API_ID, ERROR_CODE) \
  (void)Det_ReportError(TCPIP_DHCPV6_MODULE_ID, TCPIP_DHCPV6_VINSTANCE_ID, (API_ID), (ERROR_CODE))
#else
# define TcpIp_DhcpV6_CheckDetErrorReturnVoid(CONDITION, API_ID, ERROR_CODE)
# define TcpIp_DhcpV6_CheckDetErrorReturnValue(CONDITION, API_ID, ERROR_CODE, RET_VAL)
# define TcpIp_DhcpV6_CheckDetErrorContinue(CONDITION, API_ID, ERROR_CODE)
# define TcpIp_DhcpV6_DetReportError(API_ID, ERROR_CODE)
#endif /* (TCPIP_DEV_ERROR_REPORT == STD_ON) */

/* PRQA L:FctLikeMacros */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef struct
{
  uint16 OptOfs;  /* byte offset of option payload (start of option + TCPIP_DHCPV6_OPT_HDR_LEN) */
  uint16 OptCode; /* option code value */
  uint16 OptLen;  /* length of option payload in bytes */
  uint16 ErrCode; /* indicates whether data in this structure is valid */
} TcpIp_DhcpV6_OptLocationType;

typedef struct
{
  uint32 IaIdNbo; /* identifier of the address association (network-byte-order) */
  uint32 T1Nbo;   /* T1 time in seconds before renew messages will be sent (network-byte-order) */
  uint32 T2Nbo;   /* T2 time in seconds before rebind messages will be sent (network-byte-order) */
} TcpIp_DhcpV6_OptIaNaHdrRawType;

typedef struct
{
  IpBase_AddrIn6Type Addr;
  uint32             PreferredLifetimeNbo;
  uint32             ValidLifetimeNbo;
} TcpIp_DhcpV6_OptIaAddrHdrRawType;

typedef struct
{
  uint16             StatusCodeNbo;
} TcpIp_DhcpV6_OptStatusCodeHdrRawType;

typedef struct
{
  uint16             ElapsedTimeNbo;
} TcpIp_DhcpV6_OptElapsedTimeRawType;

#if ((defined TCPIP_SUPPORT_DNS) && (TCPIP_SUPPORT_DNS == STD_ON))
typedef struct
{
  boolean                                                 Valid;
  P2CONST(IpBase_AddrIn6Type, AUTOMATIC, TCPIP_APPL_DATA) Addrs;
  uint8                                                   AddrCount;
} TcpIp_DhcpV6_OptDnsServersType;
#endif

#if ((defined TCPIP_SUPPORT_DNS) && (TCPIP_SUPPORT_DNS == STD_ON))
typedef struct
{
  boolean                                    Valid;
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) List;
  uint16                                     ListLength;
} TcpIp_DhcpV6_OptDomainListType;
#endif

typedef uint8 TcpIp_DhcpV6_ConfigType;

# define TCPIP_DHCPV6_CLIENT_DUID_LEN          14U

/*  */
#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* link-local multicast address for all DHCPv6 server and relay agents */
extern CONST(IpBase_AddrIn6Type, TCPIP_CONST) TcpIp_DhcpV6_Addr_All_DHCP_Relay_Agents_and_Servers; /* PRQA S 0759 1 */ /* MD_IpV6_0759 */

#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define TCPIP_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VGetTxOption
 **********************************************************************************************************************/
/*! \brief      Get user option data for outgoing DHCPv6 messages. 
 *  \description  By this API service the TCP/IP stack reads the DHCP option data identified by parameter option.
 *  \param[in]  IpAddrId       IP address identifier representing the local IP address and EthIf controller for which
*                              the DHCP option shall be read.
 *  \param[in]  OptId          DHCP option, e.g. Host Name
 *  \param[in]  OptLenPtr      length of DHCP option data
 *  \param[in]  OptPtr         Pointer to memory containing DHCP option data
 *  \param[out] MsgTypesPtr    Define for which DHCP message types the option are sent
 *  \return     E_OK           no error occured. 
 *              E_NOT_OK       DHCP option data could not be read.
 *  \note       Currently only the option 'TCPIP_DHCPV6_OPT_ID_CLIENT_FQDN' is supported.
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VGetTxOption(
    uint8 IpAddrId,
    uint16 OptId,
    P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) OptLenPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) OptPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) MsgTypesPtr );  /* out parameter */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_VSetTxOption
 **********************************************************************************************************************/
/*! \brief      Set user option data for outgoing DHCPv6 messages. 
 *  \description  By this API service the TCP/IP stack writes the DHCP option data identified by parameter option.
 *  \param[in]  IpAddrId       IP address identifier representing the local IP address and EthIf controller for which
*                              the DHCP option shall be written.
 *  \param[in]  OptId          DHCP option, e.g. Host Name
 *  \param[in]  OptLen         length of DHCP option data
 *  \param[in]  OptPtr         Pointer to memory containing DHCP option data
 *  \param[in]  MsgTypes       Define for which DHCP message types the option shall be sent
 *  \return     E_OK           no error occured. 
 *              E_NOT_OK       DHCP option data could not be written.
 *  \note       Currently only the option 'TCPIP_DHCPV6_OPT_ID_CLIENT_FQDN' is supported.
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_VSetTxOption(
    uint8 IpAddrId,
    uint16 OptId,
    uint16 OptLen,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) OptPtr,
    uint16 MsgTypes);

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_InitMemory()
 **********************************************************************************************************************/
/*! \brief      Initializes global variables
 *  \pre        This function has to be called before any other function of this module
 *  \context    System Startup
 **********************************************************************************************************************/
extern FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_InitMemory(void);

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_Init
 **********************************************************************************************************************/
/*! \brief      This API call stores the start address of the post build time configuration  of the DHCP module and may
 *              be used to initialize the data structures.
 *  \pre        TcpIp_VInitSockets()
 *  \param[in]  CfgPtr             pointer to module configuration
 *  \context    initialization
 **********************************************************************************************************************/
extern FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_Init(
    P2CONST(TcpIp_DhcpV6_ConfigType, TCPIP_PBCFG, TCPIP_CONST) CfgPtr );

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_MainFunction
 **********************************************************************************************************************/
/*! \brief      This function handles the DHCPv6 global status
 *  \context    task level
 *  \note       This function is called every TCPIP_MAIN_FCT_PERIOD_MSEC ms
 **********************************************************************************************************************/
extern FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_MainFunction(void);

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_IsActive()
 **********************************************************************************************************************/
/*! \brief      Checks if the DhcpV6 is currently trying to acquire a valid address lease or already has one.
 *  \retval     TRUE     DHCPv6 address assignment in progress
 *  \retval     FALSE    DHCPv6 is inactive
 *  \context    task level
 **********************************************************************************************************************/
extern FUNC(boolean, TCPIP_CODE) TcpIp_DhcpV6_IsActive(uint8 IpCtrlIdx);

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_StartAddressAssignment()
 **********************************************************************************************************************/
/*! \brief      Manually start DHCPv6 address assignment procedure.
 *              This function will be called by IpV6 automatically if configured.
 *  \retval     E_OK     DHCPv6 address assignment in progress
 *  \retval     E_NOT_OK Could not start DHCPv6 address assignment
 *  \context    task level
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_StartAddressAssignment(uint8 IpCtrlIdx);

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_StartAddressAssignmentForAddr()
 **********************************************************************************************************************/
/*! \brief      Trigger the address assignment via DHCP
 *  \param[in]  IpAddrId    Address ID for which the address assignment shall be triggered
 *  \retval     E_OK        triggered succeeded
 *  \retval     E_NOT_OK    trigger failed
 *  \context    task level
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_StartAddressAssignmentForAddr(uint8 IpAddrId);  /* PRQA S 0777 */ /* MD_IpV4_0777 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_RequestInformation()
 **********************************************************************************************************************/
/*! \brief      Send Information Request message to DHCPv6 servers in order to receive other configuration options
 *              like DNS servers without performig DHCPv6 address assignment.
 *  \retval     E_OK     Sending of Information Request message has been triggered
 *  \retval     E_NOT_OK Sending of Information Request message not possible
 *  \context    task level
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_RequestInformation(uint8 IpCtrlIdx);

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_Decline()
 **********************************************************************************************************************/
/*! \brief      Decline an IPv6 address that has been assigned via DHCPv6
 *  \param[in]  AddrPtr  Declined IPv6 address
 *  \retval     E_OK     Decline procedure has been triggered
 *  \retval     E_NOT_OK There is no valid address lease that can be declined
 *  \context    task level
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_Decline(uint8 IpCtrlIdx, P2CONST(IpBase_AddrIn6Type, AUTOMATIC, IPV6_APPL_DATA) AddrPtr);

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_Confirm()
 **********************************************************************************************************************/
/*! \brief      Trigger confirm of active DHCPv6 address leases.
 *              If confirm fails address assignment procedure will be restarted automatically.
 *  \retval     E_OK     Confirm procedure has been triggered
 *  \retval     E_NOT_OK There is no valid address lease that can be confirmed
 *  \context    task level
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_Confirm(uint8 IpCtrlIdx);

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_ReleaseAddress()
 **********************************************************************************************************************/
/*! \brief      Release DHCPv6 address assignment.
 *              This function will call IpV6_SetAddress() with zero lifetime values for any address configured via
 *              DHCPv6, so IpV6 will stop using these addresses.
 *  \retval     E_OK     Release procedure has been triggered
 *  \retval     E_NOT_OK There is no valid address lease that can be released
 *  \context    task level
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6_ReleaseAddress(uint8 IpCtrlIdx);

/***********************************************************************************************************************
 *  TcpIp_DhcpV6_Reset()
 **********************************************************************************************************************/
/*! \brief      Reset DHCP.
 *              DHCP has to be triggered via TcpIp_DhcpV6_StartAddressAssignment() after calling this function.
 *  \context    task level
 **********************************************************************************************************************/
extern FUNC(void, TCPIP_CODE) TcpIp_DhcpV6_Reset(uint8 IpCtrlIdx);

#define TCPIP_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* (TCPIP_SUPPORT_IPV6 == STD_ON) && (TCPIP_SUPPORT_DHCPV6 == STD_ON) */
#endif  /* TCPIP_DHCPV6_H */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_Dhcp.h
 *********************************************************************************************************************/
