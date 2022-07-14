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
 *         \file  TcpIp_DhcpV4.c
 *        \brief  Implementation of Dynamic Host Configuration Protocol for IPv4 - Client (DHCPv4 Client)
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

#define TCPIP_DHCPV4_SOURCE

#include "TcpIp_Cfg.h"

#if (TCPIP_SUPPORT_IPV4 == STD_ON) && (TCPIP_SUPPORT_DHCPV4 == STD_ON)
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "TcpIp_DhcpV4.h"
#include "TcpIp_DhcpV4_Cbk.h"
#include "TcpIp_Lcfg.h"
#include "TcpIp_Priv.h"
#include "TcpIp_Udp.h"

#include "TcpIp_IpV4.h"
#include "TcpIp_IpV4_XCfg.h"
#include "IpBase.h"
#include "IpBase_Types.h"
#include "IpBase_Copy.h"
#include "EthIf.h"

#if ((defined TCPIP_SUPPORT_DNS) && (TCPIP_SUPPORT_DNS == STD_ON))
# include "Dns.h"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#if defined (STATIC)
#else
# define STATIC static
#endif

#define TCPIP_DHCPV4_TX_BUFFER_MIN_SIZE_BYTE  260  /* size without "DHCP host name" */

#define TCPIP_DHCPV4_HW_TYPE_ETH            1

#define TCPIP_DHCPV4_OP_BOOTREQUEST         1
#define TCPIP_DHCPV4_OP_BOOTREPLY           2
#define TCPIP_DHCPV4_SERVER_PORT           67
#define TCPIP_DHCPV4_CLIENT_PORT           68

/* DHCP OPTIONS */
#define TCPIP_DHCPV4_OPT_PAD_TYPE                       0
#define TCPIP_DHCPV4_OPT_END_TYPE                     255

#define TCPIP_DHCPV4_OPT_SUBNET_MASK_TYPE               1  /* client subnet mask */
#define TCPIP_DHCPV4_OPT_ROUTER_TYPE                    3  /* Router = Default Gateway */
#define TCPIP_DHCPV4_OPT_DOMAIN_NAME_SERVER_TYPE        6
#define TCPIP_DHCPV4_OPT_HOST_NAME_TYPE                12
#define TCPIP_DHCPV4_OPT_REQ_IP_ADDR_TYPE              50
#define TCPIP_DHCPV4_OPT_IP_ADDR_LEASE_TIME_TYPE       51
#define TCPIP_DHCPV4_OPT_OVERLOAD_TYPE                 52
#define TCPIP_DHCPV4_OPT_DHCPV4_MSG_TYPE               53
#define TCPIP_DHCPV4_OPT_SERVER_ID_TYPE                54  /* IP address of DHCP server */
#define TCPIP_DHCPV4_OPT_PARAM_REQ_LIST_TYPE           55  /* requested DHCP options */
#define TCPIP_DHCPV4_OPT_MSG_TYPE                      56  /* text (e.g. error) message */
#define TCPIP_DHCPV4_OPT_MAX_DHCPV4_MSG_SIZE_TYPE      57
#define TCPIP_DHCPV4_OPT_RENEWAL_TIME_T1_VAL_TYPE      58
#define TCPIP_DHCPV4_OPT_REBINDING_TIME_T2_VAL_TYPE    59

#define TCPIP_DHCPV4_OPT_SUBNET_MASK_LEN                4
#define TCPIP_DHCPV4_OPT_ROUTER_LEN                     4  /* n times */
#define TCPIP_DHCPV4_OPT_DOMAIN_NAME_SERVER_LEN         4  /* n times */
#define TCPIP_DHCPV4_OPT_REQ_IP_ADDR_LEN                4
#define TCPIP_DHCPV4_OPT_IP_ADDR_LEASE_TIME_LEN         4
#define TCPIP_DHCPV4_OPT_OVERLOAD_LEN                   1
#define TCPIP_DHCPV4_OPT_DHCPV4_MSG_LEN                 1
#define TCPIP_DHCPV4_OPT_SERVER_ID_LEN                  4
#define TCPIP_DHCPV4_OPT_PARAM_REQ_LIST_ID_LEN          1  /* min size */
#define TCPIP_DHCPV4_OPT_MSG_LEN                        1  /* min size */
#define TCPIP_DHCPV4_OPT_MAX_DHCPV4_MSG_SIZE_LEN        2
#define TCPIP_DHCPV4_OPT_RENEWAL_TIME_T1_VAL_LEN        4
#define TCPIP_DHCPV4_OPT_REBINDING_TIME_T2_VAL_LEN      4

#if (TCPIP_DHCPV4_MULTIPLE_INSTANCES == STD_ON)
# define TCPIP_DHCPV4_IS_ENABLED_ON_CTRL (NULL_PTR != TcpIp_DhcpV4_ConfigSets[IpCtrlIdx])
#else
# define TCPIP_DHCPV4_IS_ENABLED_ON_CTRL (TCPIP_DHCPV4_IP_CTRL_IDX != IpCtrlIdx)
#endif

/**********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
/* ----- */
#define TCPIP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC VAR(TcpIp_SockHndType, TCPIP_VAR_NOINIT) TcpIp_DhcpV4_SockHnd;  /* dummy value needed for API to IpV4 */

#define TCPIP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ----- */
#define TCPIP_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIP_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ---------- */
#define TCPIP_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIP_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define TCPIP_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_VSendDhcpMsg(
    uint8 IpCtrlIdx,
    uint8 MsgType,
    TcpIp_NetAddrType DestAddr);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_VAddReqIpAddrOption
 **********************************************************************************************************************/
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4_VAddReqIpAddrOption(
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) OptPtr,
    IpBase_AddrInType                       Yiaddr);

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_VAddServerIdOption
 **********************************************************************************************************************/
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4_VAddServerIdOption(
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) OptPtr,
    IpBase_AddrInType                       Siaddr);

#if (TCPIP_DHCPV4_SUPPORT_OPTIONS == STD_ON)
# if (TCPIP_DHCPV4_RX_OPTION_CNT > 0)
/***********************************************************************************************************************
 *  TcpIp_DhcpV4_VHandleUserOptions
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_VHandleUserOptions(
  uint8 IpCtrlIdx,
  uint8 MsgType,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) OptDataPtr,
  uint16 LenByte);
# endif

# if (TCPIP_DHCPV4_TX_OPTION_CNT > 0)
/***********************************************************************************************************************
 *  TcpIp_DhcpV4_VAddUserOptions
 **********************************************************************************************************************/
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV4_VAddUserOptions(
    uint8 IpCtrlIdx,
    uint8 MsgType,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) MsgDataPtr );
# endif
#endif

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_VAddParamReqListOpt
 **********************************************************************************************************************/
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4_VAddParamReqListOpt(
  uint8                                   IpCtrlIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) OptPtr);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_Init
 **********************************************************************************************************************/
/*! \brief      This API call stores the start address of the post build time configuration  of the DHCP module and may
 *              be used to initialize the data structures.
 *  \pre        TcpIp_VInitSockets()
 *  \param[in]  CfgPtr             pointer to module configuration
 *  \note       No configuration variant support implemented, so only TCPIP_CONFIG_VARIANT_1 (compile time) is available.
 *  \context    initialization
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_Init( void )
{
  uint8  IpCtrlIdx;      /* ip controller index */  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  TcpIp_DhcpV4_SockHnd = (TcpIp_SockHndType)TcpIp_VCfgGetMaxNumUdpSocks();  /* dummy value needed for API to IpV4 */

#if (TCPIP_DHCPV4_MULTIPLE_INSTANCES == STD_ON)
  IpCtrlIdx = IpV4_VCfgGetMaxNumCtrls();
  /* for each controller: */
  while (0 < IpCtrlIdx)
  {
    IpCtrlIdx--;

    if (!TCPIP_DHCPV4_IS_ENABLED_ON_CTRL)
    {
      continue;  /* PRQA S 0770 */ /* MD_TCPIP_14.5_0770 */
    }
#else
  {
#endif
    TcpIp_DhcpV4_RAM_SecTimer(IpCtrlIdx) = 0;
    TcpIp_DhcpV4_RAM_MsTimer(IpCtrlIdx)  = 0;

    TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx)              = TCPIP_DHCPV4_CTRL_STATE_OFF;
    TcpIp_DhcpV4_RAM_HostNameLen(IpCtrlIdx)            = 0;
    TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).Ciaddr      = 0;
    TcpIp_DhcpV4_RAM_AddrAssTimeoutOccurred(IpCtrlIdx) = FALSE;
  }

}

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_MainFunction
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
 *
 */
/* PRQA S 0715 CTRL_NEST_LIMIT */ /* MD_MSR_1.1_0715 */
FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_MainFunction(void)
{
#if (TCPIP_DHCPV4_MULTIPLE_INSTANCES == STD_ON)
  uint8 IpCtrlIdx;  /* controller index */  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
#endif

  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);

  /* #10 Iterate over all IPv4 controllers that have a local address with DHCPv4 address assignment method. */
#if (TCPIP_DHCPV4_MULTIPLE_INSTANCES == STD_ON)
  IpCtrlIdx = IpV4_VCfgGetMaxNumCtrls();

  while (0 < IpCtrlIdx)
  {
    IpCtrlIdx--;

    if(!TCPIP_DHCPV4_IS_ENABLED_ON_CTRL)
    {
      /* DHCP is not enabled on this controller */
      continue;  /* PRQA S 0770 */ /* MD_TCPIP_14.5_0770 */
    }
#else
  {
    const uint8 IpCtrlIdx = 0;
#endif

    /* #20 If configured, restart DHCPv4 address assignment if a failure has occurred. */
    if(TCPIP_DHCPV4_CTRL_STATE_OFF == TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx))
    {
      if( (TcpIp_DhcpV4_VCfgGetRestartOnFail() == STD_ON) &&
          (TcpIp_DhcpV4_RAM_AddrAssTimeoutOccurred(IpCtrlIdx) == TRUE) )
      {
        /* restart addr config */
        TcpIp_DhcpV4_TriggerAddrRequest(IpCtrlIdx, (TcpIp_NetAddrType)0);
      }
#if (TCPIP_DHCPV4_MULTIPLE_INSTANCES == STD_ON)
      continue;  /* PRQA S 0770 */ /* MD_TCPIP_14.5_0770 */
#else
      return;
#endif
    }

    /* #30 Update time variable used for timeout handling. */
    TcpIp_DhcpV4_RAM_MsTimer(IpCtrlIdx) += (uint16)TcpIp_VCfgGetMainFctPeriodMsec();

    if(1000 <= TcpIp_DhcpV4_RAM_MsTimer(IpCtrlIdx))
    {
      /* one second passed */
      TcpIp_DhcpV4_RAM_MsTimer(IpCtrlIdx) -= 1000;
      TcpIp_DhcpV4_RAM_SecTimer(IpCtrlIdx)++;
    }

    /* #40 Perform actions based on current DHCP state is necessary. */
    if(TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx) <= TcpIp_DhcpV4_RAM_SecTimer(IpCtrlIdx))
    {
      /* -------------------------------------------------------------------- */
      /* start of STATE dependent processing -------------------------------- */
      switch(TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx))
      {
      case TCPIP_DHCPV4_CTRL_STATE_INIT:
        {
          /* #50 [INIT] Send first DHCPDISCOVER message, schedule retransmission and switch to state SELECTING. */
          TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).Ciaddr = 0; /* unassign client ip address */

          TcpIp_DhcpV4_RAM_SecTimer(IpCtrlIdx) = 0;

          /* send first DISCOVER message */
          if (E_OK == TcpIp_DhcpV4_VSendDhcpMsg(IpCtrlIdx, TCPIP_DHCPV4_MSG_DISCOVER, IPV4_HTONL(TCPIP_INADDR_BROADCAST)))
          {
            /* TX successful. Switch to state SELECTING */

            TcpIp_DhcpV4_RAM_SecNextInterval(IpCtrlIdx) = TcpIp_DhcpV4_VCfgGetDiscoverMinIntervalSec();
            TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx) = TcpIp_DhcpV4_RAM_SecNextInterval(IpCtrlIdx);

            TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx) = TCPIP_DHCPV4_CTRL_STATE_SELECTING;
          }
          else
          {
            /* TX failed. Stay in current state and retry in next MainFunction cycle */
          }
          break;
        }
      case TCPIP_DHCPV4_CTRL_STATE_SELECTING:
        {
          /* #60 [SELECTING] Retransmit DHCPDISCOVER message and schedule next retransmission or cancel DHCP assignment if number of retransmission has been reached. */
          /* check if event has to be triggered */
          /* check if another DISCOVER message may be sent */

          if( (0 == TcpIp_DhcpV4_VCfgGetDiscoverMaxNum())  /* retry number is not limited */
               || ((TcpIp_DhcpV4_VCfgGetDiscoverMaxNum() -1) > TcpIp_DhcpV4_RAM_RetryCounter(IpCtrlIdx)) )  /* retry limit not yet reached */
          {
            /* send another DISCOVER message */
            if (E_OK == TcpIp_DhcpV4_VSendDhcpMsg(IpCtrlIdx, TCPIP_DHCPV4_MSG_DISCOVER, IPV4_HTONL(TCPIP_INADDR_BROADCAST)))
            {
              /* TX successfull. Schedule next retry if no response is received. */
              uint32 NextSecInterval;

              if(TcpIp_DhcpV4_RAM_SecNextInterval(IpCtrlIdx) < TcpIp_DhcpV4_VCfgGetDiscoverMaxIntervalSec())
              {
                NextSecInterval = TcpIp_DhcpV4_RAM_SecNextInterval(IpCtrlIdx) * 2;
                if(NextSecInterval > TcpIp_DhcpV4_VCfgGetDiscoverMaxIntervalSec())
                {
                  NextSecInterval = TcpIp_DhcpV4_VCfgGetDiscoverMaxIntervalSec();
                }
                TcpIp_DhcpV4_RAM_SecNextInterval(IpCtrlIdx) = NextSecInterval;
              }
              else
              {
                NextSecInterval = TcpIp_DhcpV4_RAM_SecNextInterval(IpCtrlIdx);
              }

              TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx) += NextSecInterval;
              TcpIp_DhcpV4_RAM_RetryCounter(IpCtrlIdx)++;
            }
            else
            {
              /* TX failed. Stay in current state and retry in next MainFunction cycle */
            }
          }
          else
          {
            /* cancel complete DHCP address request */
            TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx) = TCPIP_DHCPV4_CTRL_STATE_OFF;
            TcpIp_DhcpV4_RAM_AddrAssTimeoutOccurred(IpCtrlIdx) = TRUE;
          }
          break;
        }
      case TCPIP_DHCPV4_CTRL_STATE_REQUESTING:
        {
          /* #70 [REQUESTING] Send DHCPREQUEST message and schedule next retransmission or cancel DHCP assignment if number of retransmission has been reached. */
          if((TcpIp_DhcpV4_VCfgGetRequestMaxNum()) > TcpIp_DhcpV4_RAM_RetryCounter(IpCtrlIdx))
          {
            TcpIp_NetAddrType DhcpServAddr = IPV4_HTONL(TCPIP_INADDR_BROADCAST);
            /* send REQUEST message */
            if(E_OK == TcpIp_DhcpV4_VSendDhcpMsg(IpCtrlIdx, TCPIP_DHCPV4_MSG_REQUEST, DhcpServAddr))
            {
              /* TX successfull. Schedule next retry if no response is received. */
              TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx) += TcpIp_DhcpV4_VCfgGetRequestIntervalSec();
              TcpIp_DhcpV4_RAM_RetryCounter(IpCtrlIdx)++;
            }
            else
            {
              /* TX failed. Stay in current state and retry in next MainFunction cycle */
            }
          }
          else
          {
            /* cancel complete DHCP address request */
            TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx) = TCPIP_DHCPV4_CTRL_STATE_OFF;
            TcpIp_DhcpV4_RAM_AddrAssTimeoutOccurred(IpCtrlIdx) = TRUE;
          }
          break;
        }
      case TCPIP_DHCPV4_CTRL_STATE_BOUND:
        {
          /* #80 [BOUND] Switch to state RENEWING after T1 timeout has expired. */
          TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx) = TCPIP_DHCPV4_CTRL_STATE_RENEWING;
          break;
        }
      case TCPIP_DHCPV4_CTRL_STATE_RENEWING:
        {
          /* #90 [RENEWING] Transmit DHCPREQUEST (unicast) and schedule next retransmission or switch to state REBINDING if T2 timeout has expired. */

          if(TcpIp_DhcpV4_RAM_SecTimer(IpCtrlIdx) < TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).T2)
          {
            /* send DHCP REQUEST (unicast) */
            if (E_OK == TcpIp_DhcpV4_VSendDhcpMsg(IpCtrlIdx, TCPIP_DHCPV4_MSG_REQUEST, TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).DhcpSiaddr))
            {
              /* TX successful. Schedule next retransmission */
              uint32 TimeLeft = (TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).T2 - TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx));
              if((uint32)TcpIp_DhcpV4_VCfgGetRenewMinIntervalSec() < TimeLeft)
              {
                if(((uint32)TcpIp_DhcpV4_VCfgGetRenewMinIntervalSec() *2) < TimeLeft)
                {
                  TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx) += (TimeLeft >> 1);
                }
                else
                {
                  TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx) += TcpIp_DhcpV4_VCfgGetRenewMinIntervalSec();
                }
              }
              else
              {
                TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx) = TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).T2;
              }
            }
            else
            {
              /* TX failed. Stay in current state and retry in next MainFunction cycle */
            }
          }
          else
          {
            /* switch to REBINDING */
            TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx) = TCPIP_DHCPV4_CTRL_STATE_REBINDING;
          }
          break;
        }
      case TCPIP_DHCPV4_CTRL_STATE_REBINDING:
        {
          /* #100 [REBINDING] Transmit DHCPREQUEST (broadcast) and schedule next retransmission or cancel DHCP assignment if number of retransmission has been reached. */
          if(TcpIp_DhcpV4_RAM_SecTimer(IpCtrlIdx) < TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).LeaseTime)
          {
            TcpIp_NetAddrType DhcpServAddr = IPV4_HTONL(TCPIP_INADDR_BROADCAST);
            /* send DHCP REQUEST (broadcast) */
            if (E_OK == TcpIp_DhcpV4_VSendDhcpMsg(IpCtrlIdx, TCPIP_DHCPV4_MSG_REQUEST, DhcpServAddr))
            {
              /* TX successful. Schedule next retransmission. */
              uint32 TimeLeft = (TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).LeaseTime - TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx));

              if(TcpIp_DhcpV4_VCfgGetRebindMinIntervalSec() < TimeLeft)
              {
                if(((uint32)TcpIp_DhcpV4_VCfgGetRebindMinIntervalSec() *2) < TimeLeft)
                {
                  TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx) += (TimeLeft >> 1);
                }
                else
                {
                  TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx) += TcpIp_DhcpV4_VCfgGetRebindMinIntervalSec();
                }
              }
              else
              {
                TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx) = TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).LeaseTime;
              }
            }
            else
            {
              /* TX failed. Stay in current state and retry in next MainFunction cycle */
            }
          }
          else
          {
            /* lease time has just expired */
            CANOE_WRITE_STRING("TcpIp_DhcpV4_MainFunction: lease time has just expired")
            TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx) = TCPIP_DHCPV4_CTRL_STATE_OFF;
            TcpIp_DhcpV4_RAM_AddrAssTimeoutOccurred(IpCtrlIdx) = TRUE;

            /* indicate loss of IP address to IP-module */
            IpV4_Ip_VIpAddrAssignmentReadyNotification(IpCtrlIdx, IPV4_IP_ADDR_ASSIGNMENT_METHOD_DHCP, FALSE);
          }
          break;
        }
      default:
        {
          /* (currently) unhandled state, or nothing to do */
          TCPIP_DUMMY_ASM_NOP
            break;
        }
        /* end of STATE dependend processing ---------------------------------- */
        /* -------------------------------------------------------------------- */
      }
    }
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6080 */ /* MD_MSR_STMIF */
/* PRQA L:CTRL_NEST_LIMIT */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_TriggerAddrRequest
 **********************************************************************************************************************/
/*! \brief      This function starts the address request process
 *  \param[in]  IpCtrlIdx            controller index
 *  \param[in]  Ciaddr             desired network address
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_TriggerAddrRequest(
    uint8 IpCtrlIdx,
    TcpIp_NetAddrType Ciaddr )
{
  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);

  IPV4_UNUSED_PARAMETER(Ciaddr);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  

  CANOE_WRITE_STRING("TcpIp_DhcpV4_TriggerAddrRequest: function was called")

  TcpIp_DhcpV4_RAM_RetryCounter(IpCtrlIdx) = 0;
  TcpIp_DhcpV4_RAM_SecTimer(IpCtrlIdx)     = 0;
  {
    /* set timer and event for initial wait */
    uint16 RandTimeMsec;
    RandTimeMsec = ( TcpIp_GetRandNoFct() % (TcpIp_DhcpV4_VCfgGetInitWaitMaxMsec() - TcpIp_DhcpV4_VCfgGetInitWaitMinMsec()) ) +
                   TcpIp_DhcpV4_VCfgGetInitWaitMinMsec();

    TcpIp_DhcpV4_RAM_MsTimer(IpCtrlIdx)      = 1000 - (RandTimeMsec % 1000);
    TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx) = (RandTimeMsec / 1000) +1;
  }

  TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx) = TCPIP_DHCPV4_CTRL_STATE_INIT;

}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_StopAddrRequest
 **********************************************************************************************************************/
/*! \brief      This function stops the (ongoing) address request process
 *  \param[in]  IpCtrlIdx            controller index
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_StopAddrRequest(
    uint8 IpCtrlIdx )
{
  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);

  if (TCPIP_DHCPV4_IS_ENABLED_ON_CTRL)
  {
    TcpIp_DhcpV4_Reset(IpCtrlIdx);
  }
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_RxIndication
 **********************************************************************************************************************/
/*! \brief      Receive Indication Callback
 *  \param[in]  IpCtrlIdx            controller index of the received message
 *  \param[in]  DataPtr              pointer to the received data
 *  \param[in]  DataLen              length of the received data
 *  \param[in]  SourcePtr            source network address and port
 *  \param[in]  DestinationPtr       destination network address and port
 *  \param[in]  ChecksumCalculated   indicates if the UDP checksum is already checked by lower layer
 *  \context    interrupt or task level
 **********************************************************************************************************************/
void TcpIp_DhcpV4_RxIndication(
    uint8 IpCtrlIdx,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
    uint16 DataLen,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SourcePtr,
    P2CONST(IpBase_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DestinationPtr
#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
    , boolean ChecksumCalculated
#endif
        )
{
  TcpIp_DhcpV4_FrameType *FramePtr;
  uint8            PhysAddr[TCPIP_PHYS_ADDR_LEN_BYTE];  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  const TcpIp_Udp_HdrType *UdpHdrPtr;
  uint16 udpHdrLen;

  /* In Vector's implementation of the TCPIP-Stack, receiving on UDP sockets always provides a
     linear buffer. So direct DHCP header access via a single struct is possible. */

  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);
  TCPIP_ASSERT(DataPtr     != NULL_PTR);

  if (!TCPIP_DHCPV4_IS_ENABLED_ON_CTRL)
  {
    /* DHCP not enabled for this controller */
    return;
  }

  if (DataLen < TCPIP_UDP_HDR_LEN_BYTE)
  {
    /* invalid length */
    return;
  }

  UdpHdrPtr = (const TcpIp_Udp_HdrType *)DataPtr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */

  if ((TCPIP_DHCPV4_SERVER_PORT != IPV4_HTONS(UdpHdrPtr->SrcPort)) || (TCPIP_DHCPV4_CLIENT_PORT != IPV4_HTONS(UdpHdrPtr->TgtPort)))
  {
    return;
  }

  udpHdrLen = IPV4_HTONS(UdpHdrPtr->LenByte);

  if ((udpHdrLen > DataLen) || (udpHdrLen < (TCPIP_UDP_HDR_LEN_BYTE + TCPIP_DHCPV4_MIN_HEADER_LENGTH_BYTE)))
  {
    return;
  }

#if (TCPIP_TCPIP_HW_BASED_CHECKSUM_ENABLED == STD_ON)
  if(ChecksumCalculated == TRUE)
  {
    /* The DHCP checksum was already checked by the lower layer / hardware. If the checksum was wrong the packet will not reach the DHCP at all, so no further check is necessary here. */
  }
  else
#endif
  {
    TcpIp_PseudoHdrV4Type PseudoHdr;
    uint32 ChecksumTmp;

    PseudoHdr.RemAddr  = ((const IpBase_SockAddrInType*)SourcePtr)->sin_addr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    PseudoHdr.LocAddr  = ((const IpBase_SockAddrInType*)DestinationPtr)->sin_addr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_sock */
    PseudoHdr.Null     = 0;
    PseudoHdr.Protocol = TCPIP_SOCK_PROT_UDP;
    PseudoHdr.LenByte  = IPV4_HTONS(DataLen);
    ChecksumTmp = IpBase_TcpIpChecksumAdd((const uint8*)UdpHdrPtr,                      DataLen,           0, FALSE); /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
    ChecksumTmp = IpBase_TcpIpChecksumAdd((const uint8*)&PseudoHdr, (uint32)(sizeof(PseudoHdr)), ChecksumTmp,  TRUE); /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */

    if (0 != (uint16)ChecksumTmp)
    {
      return;
    }
  }

  /* skip UDP header */
  DataPtr = &DataPtr[TCPIP_UDP_HDR_LEN_BYTE];
  DataLen = (uint16)(udpHdrLen - TCPIP_UDP_HDR_LEN_BYTE);

  FramePtr = (TcpIp_DhcpV4_FrameType *)(&DataPtr[0]);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */

  /* Ignore this message if magic pattern at end of DHCP header is not set correctly. */
  if (FramePtr->MagicPattern != TCPIP_DHCPV4_MAGIC_PATTERN)
  {
    return;
  }

  /* find the corresponding controller */
  /* compare Msg-HW-Addr with own addr (future: compare to all controller addresses) */
  TCPIP_LL_GetPhysAddr(IpV4_CtrlIdxIp2EthIf[IpCtrlIdx], &PhysAddr[0] );
  if( (TRUE == TcpIp_CmpLLAddr(&FramePtr->Chaddr[0], &PhysAddr[0])) &&  /* message is for this node */
      (TCPIP_DHCPV4_OP_BOOTREPLY == FramePtr->Op) &&                              /* this message comes from a server */
      (FramePtr->Xid == TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).Xid) )               /* this message corresponds to the current DHCP XID */
  {
        TcpIp_DhcpV4_DataStructType LocConfigData;
        uint8 MsgType = TCPIP_DHCPV4_DHCPINVALIDMSG;
        uint16 OverallOptLen = (uint16)(DataLen - TCPIP_DHCPV4_MIN_HEADER_LENGTH_BYTE);
        uint16 RemainingOptLen = OverallOptLen;
        uint8 *OptPtr = &DataPtr[TCPIP_DHCPV4_MIN_HEADER_LENGTH_BYTE];
#if ((defined TCPIP_SUPPORT_DNS) && (TCPIP_SUPPORT_DNS == STD_ON))
        IpBase_AddrInType *DnsServerOpt = (IpBase_AddrInType*)NULL_PTR;
        uint8 DnsServerOptCount = 0;
#endif
        boolean EndOptFound = FALSE;
        boolean MsgIsValid = TRUE;

        /* now evaluate DHCP options */
        LocConfigData.T1          = 0;
        LocConfigData.T2          = 0;
        LocConfigData.Ciaddr      = FramePtr->Yiaddr;
        LocConfigData.NetMask     = 0;
        LocConfigData.DefGWiaddr  = 0;
        LocConfigData.DhcpSiaddr  = 0;
        LocConfigData.LeaseTime   = 0;
        while(0 != RemainingOptLen)
        {
          uint8 OptLen; /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

          /* evaluate all supported options */
          if(TCPIP_DHCPV4_OPT_PAD_TYPE == *OptPtr)
          {
            /* option Pad */
            OptPtr = &OptPtr[1];
            RemainingOptLen--;
          }
          else if(TCPIP_DHCPV4_OPT_END_TYPE == *OptPtr)
          {
            /* option End */
            EndOptFound = TRUE;
            break;
          }
          else
          {
            if( (RemainingOptLen < 2) || (OptPtr[1] > (RemainingOptLen -2)) )
            {
              /* error:
                 - options have at least option type and length fields
                 - option length may not be greater than remaning data length */
              break;  /* PRQA S 0771 */ /* MD_TCPIP_DHCPV4_14.6_0771_a */
            }

            switch(*OptPtr)
            {
            case(TCPIP_DHCPV4_OPT_SUBNET_MASK_TYPE):
              {
                /* option SUBNET_MASK */
                OptLen = TCPIP_DHCPV4_OPT_SUBNET_MASK_LEN +2;
                IPV4_GET_UINT32_HBO(OptPtr, 2, LocConfigData.NetMask); /* Address is stored in host byte-order */
                OptPtr = &OptPtr[OptLen];
                RemainingOptLen -= OptLen;

                /* If the server specifies a subnet mask it must not be zero. */
                if (LocConfigData.NetMask == 0) 
                {
                  /* Option parsing will continue, but message will be dropped later. */
                  MsgIsValid = FALSE;
                }

                break;
              }
            case(TCPIP_DHCPV4_OPT_ROUTER_TYPE):
              {
                /* option ROUTER -> DefaultGateway */
                OptLen = TCPIP_DHCPV4_OPT_ROUTER_LEN +2;
                IPV4_GET_UINT32_HBO(OptPtr, 2, LocConfigData.DefGWiaddr); /* Address is stored in host byte-order */
                OptPtr = &OptPtr[OptLen];
                RemainingOptLen -= OptLen;
                break;
              }
            case(TCPIP_DHCPV4_OPT_IP_ADDR_LEASE_TIME_TYPE):
              {
                /* option IP_ADDR_LEASE_TIME */
                OptLen = TCPIP_DHCPV4_OPT_IP_ADDR_LEASE_TIME_LEN +2;
                IPBASE_GET_UINT32(OptPtr, 2, LocConfigData.LeaseTime);
                OptPtr = &OptPtr[OptLen];
                RemainingOptLen -= OptLen;
                break;
              }
            case(TCPIP_DHCPV4_OPT_DHCPV4_MSG_TYPE):
              {
                /* option TCPIP_DHCPV4_MSG -> message type (DHCPOFFER, DHCPACK, DHCPNAK) */
                OptLen = TCPIP_DHCPV4_OPT_DHCPV4_MSG_LEN +2;
                MsgType = OptPtr[2];
                OptPtr = &OptPtr[OptLen];
                RemainingOptLen -= OptLen;
                break;
              }
            case(TCPIP_DHCPV4_OPT_SERVER_ID_TYPE):
              {
                /* option SERVER_ID */
                OptLen = TCPIP_DHCPV4_OPT_SERVER_ID_LEN +2;
                IPV4_GET_UINT32_HBO(OptPtr, 2, LocConfigData.DhcpSiaddr); /* Address is stored in host byte-order */
                OptPtr = &OptPtr[OptLen];
                RemainingOptLen -= OptLen;
                break;
              }
            case(TCPIP_DHCPV4_OPT_RENEWAL_TIME_T1_VAL_TYPE):
              {
                /* option RENEWAL_TIME_T1 */
                OptLen = TCPIP_DHCPV4_OPT_RENEWAL_TIME_T1_VAL_LEN +2;
                IPBASE_GET_UINT32(OptPtr, 2, LocConfigData.T1);
                OptPtr = &OptPtr[OptLen];
                RemainingOptLen -= OptLen;
                break;
              }
            case(TCPIP_DHCPV4_OPT_REBINDING_TIME_T2_VAL_TYPE):
              {
                /* option RENEWAL_TIME_T2 */
                OptLen = TCPIP_DHCPV4_OPT_REBINDING_TIME_T2_VAL_LEN +2;
                IPBASE_GET_UINT32(OptPtr, 2, LocConfigData.T2);
                OptPtr = &OptPtr[OptLen];
                RemainingOptLen -= OptLen;
                break;
              }
#if ((defined TCPIP_SUPPORT_DNS) && (TCPIP_SUPPORT_DNS == STD_ON))
            case(TCPIP_DHCPV4_OPT_DOMAIN_NAME_SERVER_TYPE):
              {
                /* option DOMAIN_NAME_SERVER */
                uint8 LocOptLen = OptPtr[1];
                if( (0 == (LocOptLen % TCPIP_DHCPV4_OPT_DOMAIN_NAME_SERVER_LEN)) && (TCPIP_DHCPV4_OPT_DOMAIN_NAME_SERVER_LEN <= LocOptLen) )
                {
                  DnsServerOpt = (IpBase_AddrInType *)(&OptPtr[2]);
                  DnsServerOptCount = (uint8)(LocOptLen / TCPIP_DHCPV4_OPT_DOMAIN_NAME_SERVER_LEN);
                }
                else
                {
                  DnsServerOptCount = 0;
                }
                OptLen = LocOptLen +2;
                OptPtr = &OptPtr[OptLen];
                RemainingOptLen -= OptLen;
                break;
              }
#endif
            default:
              {
                /* not supportet DHCP option -> skip this option */
                OptLen = OptPtr[1] +2;
                OptPtr = &OptPtr[OptLen];
                RemainingOptLen -= OptLen;
                break;
              }
            }
          }
        }  /* while(0 != RemainingOptLen) */

        /* -------------------------------------------------------------------------- */
        /* -------------------------------------------------------------------------- */

        /* check if message type has a valid value */
        if( (TCPIP_DHCPV4_DHCPINVALIDMSG == MsgType) ||
            (TCPIP_DHCPV4_DHCPINFORM < MsgType) )
        {
          MsgIsValid = FALSE;
        }
        else if(TRUE != EndOptFound)
        {
          /* end option is missing */
          MsgIsValid = FALSE;
        }
        else if( (0 == LocConfigData.LeaseTime) && ((TCPIP_DHCPV4_DHCPACK == MsgType) || (TCPIP_DHCPV4_DHCPOFFER == MsgType)) )
        {
          /* invalid DHCP message type, or invalid lease time */
          MsgIsValid = FALSE;
        }
        else if( (TCPIP_DHCPV4_CTRL_STATE_SELECTING != TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx)) &&
                 (LocConfigData.DhcpSiaddr != TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).DhcpSiaddr) )
        {
          /* discard messages from other servers than the selected one */
          MsgIsValid = FALSE;
        }
        else if((LocConfigData.Ciaddr == 0) && (MsgType != TCPIP_DHCPV4_DHCPNACK))
        {
          /* check for invalid client address '0' */
          MsgIsValid = FALSE;
        }
        else if(LocConfigData.Ciaddr == TCPIP_INADDR_BROADCAST)
        {
          /* check for invalid client address 'global broadcast' */
          MsgIsValid = FALSE;
        }
        else if((IPBASE_HTON32(LocConfigData.Ciaddr) & 0xF0000000u) == 0xE0000000u)
        {
          /* check for invalid client address 'multicast' */
          MsgIsValid = FALSE;
        }
        else if(LocConfigData.NetMask != 0)
        {
          /* check for invalid network mask */
          uint8_least ShiftCnt = 0u;
          uint32 NetmaskShifted = IPBASE_HTON32(LocConfigData.NetMask);
          while(((NetmaskShifted & 0x01) == 0u)) /* This condition will be false after a maximum of 31 iterations since at least one bit in NetmaskShifted is set. */
          {
            NetmaskShifted = NetmaskShifted >> 1;
            ShiftCnt++;
          }
          while((ShiftCnt < 32u) && ((NetmaskShifted & 0x01) == 1u))
          {
            NetmaskShifted = NetmaskShifted >> 1;
            ShiftCnt++;
          }
          if(32u != ShiftCnt)
          {
            /* discard messages, network mask is not a sequence of '1'-bits followed by a sequence of '0'-bits */
            MsgIsValid = FALSE;
          }
        }
        else
        {
          /* do nothing */
        }

        if(TRUE == MsgIsValid)
        {
          if(TCPIP_DHCPV4_CTRL_STATE_SELECTING == TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx))
          {
            if(TCPIP_DHCPV4_DHCPOFFER == MsgType)
            {
              /* incoming offer for IP address */

              /* Always take the first DHCPOFFER and use is for further processing. */

              /*  store addr */
              TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).Ciaddr      = FramePtr->Yiaddr;
              TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).DhcpSiaddr  = LocConfigData.DhcpSiaddr;
              TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).NetMask     = LocConfigData.NetMask;
              TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).DefGWiaddr  = LocConfigData.DefGWiaddr;

              /* switch mode to next state and set timeout (for transmission of request) */
              TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx)    = TCPIP_DHCPV4_CTRL_STATE_REQUESTING;
              TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx) = TcpIp_DhcpV4_RAM_SecTimer(IpCtrlIdx);
              TcpIp_DhcpV4_RAM_RetryCounter(IpCtrlIdx) = 0;
            }
            else
            {
              /* ignore incoming ACK and NAK in this state */
            }
          }
          else if( (TCPIP_DHCPV4_CTRL_STATE_REQUESTING == TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx)) ||
            (TCPIP_DHCPV4_CTRL_STATE_RENEWING == TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx)) ||
            (TCPIP_DHCPV4_CTRL_STATE_REBINDING == TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx)) )
          {
            switch (MsgType)
            {
            case TCPIP_DHCPV4_DHCPACK:
            {
              /* incoming ack for requested IP address */

              /* According to the RFC, the DHCP module SHOULD probe its address before using it for
              IP module configuration. This implementation does not implement this feature! */

              /* store addr */
              TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).Ciaddr      = FramePtr->Yiaddr;
              TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).DhcpSiaddr  = LocConfigData.DhcpSiaddr;
              TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).NetMask     = LocConfigData.NetMask;
              TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).DefGWiaddr  = LocConfigData.DefGWiaddr;

              TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).LeaseTime  = LocConfigData.LeaseTime;

              /* set timeout T1 */
              if(0 != LocConfigData.T1)
              {
                TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).T1 = LocConfigData.T1;
              }
              else
              {
                /* T1 is 0.5 times the lease time */
                TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).T1 = TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).LeaseTime >> 1;
              }
              /* set timeout T2 */
              if(0 != LocConfigData.T2)
              {
                TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).T2 = LocConfigData.T2;
              }
              else
              {
                /* T2 is 0.875 times the lease time */
                TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).T2 = TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).LeaseTime
                                                  - (TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).LeaseTime >>3);
              }
#if ((defined TCPIP_SUPPORT_DNS) && (TCPIP_SUPPORT_DNS == STD_ON))
              /* set DNS server address */
              if(0 != DnsServerOptCount)
              {
                Dns_UpdateIpV4Servers(DnsServerOpt, DnsServerOptCount);
              }
#endif

              /* indicate valid address to IP-module */
              if(TCPIP_DHCPV4_CTRL_STATE_REQUESTING == TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx))
              {
                IpV4_Ip_VIpAddrAssignmentReadyNotification(IpCtrlIdx, IPV4_IP_ADDR_ASSIGNMENT_METHOD_DHCP, TRUE);
              }

              if(TCPIP_DHCPV4_CTRL_STATE_REQUESTING != TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx))
              {
                /* states RENEWING and REBINDING */
                /* reset SecTimer */
                TcpIp_DhcpV4_RAM_SecTimer(IpCtrlIdx) = 0;
                TcpIp_DhcpV4_RAM_MsTimer(IpCtrlIdx)  = 0;
              }

              /* set event timer to T1 */
              TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx) = TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).T1;
              CANOE_WRITE_STRING_2("TcpIp_DhcpV4_RxIndication: next event in %d seconds, last state was %d", TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx), TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx))
              CANOE_WRITE_STRING_2("TcpIp_DhcpV4_RxIndication: event timers are %d seconds and %d milliseconds", TcpIp_DhcpV4_RAM_SecTimer(IpCtrlIdx), TcpIp_DhcpV4_RAM_MsTimer(IpCtrlIdx))

              /* switch mode to next state */
              TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx) = TCPIP_DHCPV4_CTRL_STATE_BOUND;

              break;
            }
            case TCPIP_DHCPV4_DHCPOFFER:
            {
              /* incoming offer for IP address */
              /* discard msg */
              break;
            }
            case TCPIP_DHCPV4_DHCPNACK:
            {
              /* incoming NACK (for requested IP address) -> back to INIT */
              if(TCPIP_DHCPV4_CTRL_STATE_REQUESTING != TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx))
              {
                /* indicate loss of IP address to IP-module */
                IpV4_Ip_VIpAddrAssignmentReadyNotification(IpCtrlIdx, IPV4_IP_ADDR_ASSIGNMENT_METHOD_DHCP, FALSE);
              }

              if(TCPIP_DHCPV4_CTRL_STATE_BOUND == TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx))
              {
                /* ignore incoming NACK */
                break;  /* PRQA S 3333 */ /* MD_TCPIP_DHCP_14.6_3333 */
              }
              else  /* switch to INIT */
              {
                TcpIp_DhcpV4_RAM_RetryCounter(IpCtrlIdx) = 0;
                TcpIp_DhcpV4_RAM_SecTimer(IpCtrlIdx) = 0;
                {
                  /* set timer and event for initial wait */
                  uint16 RandTimeMsec;
                  RandTimeMsec = ( TcpIp_GetRandNoFct()
                    % (TcpIp_DhcpV4_VCfgGetInitWaitMaxMsec() - TcpIp_DhcpV4_VCfgGetInitWaitMinMsec()) )
                    + TcpIp_DhcpV4_VCfgGetInitWaitMinMsec();

                  TcpIp_DhcpV4_RAM_MsTimer(IpCtrlIdx)      = 1000 - (RandTimeMsec % 1000);
                  TcpIp_DhcpV4_RAM_SecNextEvent(IpCtrlIdx) = (RandTimeMsec / 1000) +1;
                }
                TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx) = TCPIP_DHCPV4_CTRL_STATE_INIT;
              }
              break;
            }
            default:
            {
              /* no other messages should arrive, ignore them */
              break;
            }
            }
          }
          else
          {
            /* ignore messages in all other states */
          }

#if ((TCPIP_DHCPV4_SUPPORT_OPTIONS == STD_ON) && (TCPIP_DHCPV4_RX_OPTION_CNT > 0))  /* PRQA S 3332 */ /* MD_TCPIP_19.11_3332 */
          if (E_OK != TcpIp_DhcpV4_VHandleUserOptions(0, MsgType, &((uint8*)(DataPtr))[TCPIP_DHCPV4_MIN_HEADER_LENGTH_BYTE], OverallOptLen))
          {
            /* error */
          }
#endif

          /* end of DHCP message "switch" */
        }
        else
        {
          /* message is invalid */
        }  /* end if(MsgIsValid) */
  }

}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010, 6030, 6050, 6060, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCA, MD_MSR_STPAR, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_Reset
 **********************************************************************************************************************/
/*! \brief      Reset
 *  \param[in]  IpCtrlIdx            controller index
 *  \context    task level
 **********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DhcpV4_Reset(
    uint8 IpCtrlIdx )
{
  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);

  /* reset DHCP module for this controller */
  TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx)         = TCPIP_DHCPV4_CTRL_STATE_OFF;
  TcpIp_DhcpV4_RAM_SecTimer(IpCtrlIdx)          = 0;
  TcpIp_DhcpV4_RAM_MsTimer(IpCtrlIdx)           = 0;
  TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).Ciaddr = 0;

  TcpIp_DhcpV4_RAM_AddrAssTimeoutOccurred(IpCtrlIdx) = FALSE;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_SetHostName
 **********************************************************************************************************************/
/*! \brief      set DHCP host name
 *  \param[in]  HostNamePtr           pointer to host name
 *  \param[in]  HostNameLenByte       host name length in bytes
 *  \return     E_OK                  host name set
 *  \return     E_NOT_OK              setting host name failed
 *  \context    task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_SetHostName(
    uint8                                    IpCtrlIdx,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) HostNamePtr,
    uint8 HostNameLenByte )
{
  TCPIP_ASSERT(TcpIp_State == TCPIP_STATE_INIT);
  TCPIP_ASSERT(HostNamePtr != NULL_PTR);

  /* check host name length */
  /* ------------------------------------------------- */

  if (HostNameLenByte > TcpIp_DhcpV4_VCfgGetHostNameLenMax())
  {
    TcpIp_CallDetReportError(TCPIP_DHCPV4_API_ID_SET_HOST_NAME, TCPIP_E_INV_ARG)
    return E_NOT_OK;
  }

  /* set name length */
  TcpIp_DhcpV4_RAM_HostNameLen(IpCtrlIdx) = HostNameLenByte;

  /* copy name */
  {
    uint8 BytePos;
    for(BytePos = 0; BytePos < HostNameLenByte; BytePos++)
    {
      TcpIp_DhcpV4_RAM_HostName(IpCtrlIdx)[BytePos] = HostNamePtr[BytePos];
    }
  }

  return E_OK;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_GetDhcpTimeoutInfo
 **********************************************************************************************************************/
/*! \brief      check if a DHCP address finding timeout occurred
 *  \param[in]  IpCtrlIdx               controller index
 *  \return     TRUE                  there was a DHCP timeout
 *  \return     FALSE                 there was no DHCP timeout
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_DhcpV4_GetDhcpTimeoutInfo(
    uint8 IpCtrlIdx )
{
  return TcpIp_DhcpV4_RAM_AddrAssTimeoutOccurred(IpCtrlIdx);
}

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_GetActiveAddress
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_DhcpV4_GetActiveAddress(
    uint8                                                 IpCtrlIdx,
    P2VAR(IpV4_IpAddrSetType, AUTOMATIC, TCPIP_APPL_DATA) addrInfoPtr)
{
  boolean retVal = FALSE;

  TCPIP_ASSERT(IpV4_Ip_VCfgIsAddrAssignmentEnabled(IPV4_IP_ADDR_ASSIGNMENT_METHOD_DHCP));

  /* #10 Check if DHCP instance has an active lease from a DHCP server. */
  if ((TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx) == TCPIP_DHCPV4_CTRL_STATE_BOUND) ||
      (TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx) == TCPIP_DHCPV4_CTRL_STATE_RENEWING) ||
      (TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx) == TCPIP_DHCPV4_CTRL_STATE_REBINDING))
  {
    /* #20 Copy address information into provided output structure. */
    addrInfoPtr->NetAddr   = TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).Ciaddr;
    addrInfoPtr->NetMask   = TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).NetMask;
    addrInfoPtr->DefGwAddr = TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).DefGWiaddr;
    retVal = TRUE;
  }

  return retVal;
}

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_VSendDhcpMsg
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_VSendDhcpMsg(
    uint8 IpCtrlIdx,
    uint8 MsgType,
    TcpIp_NetAddrType DestAddr)
{
  IpV4_AddrIdType IpV4AddrId;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  uint8        *BufPtr;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
  uint16        BufLenByte;
  uint16        UdpLenByte;

  uint16                DhcpMsgLen;
  Std_ReturnType        Result;
  TcpIp_IpTxRequestDescriptorType IpTxReqestDescr;
  /* UDP */
  IpBase_SockAddrInType Dest;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  Dest.sin_family = IPBASE_AF_INET;
  Dest.sin_port   = IPV4_UINT8_HTONS(TCPIP_DHCPV4_SERVER_PORT);
  Dest.sin_addr   = DestAddr;

  /* Calculate length of DHCP message */
  DhcpMsgLen = TCPIP_DHCPV4_MIN_HEADER_LENGTH_BYTE + (3 + 1); /* at least the DHCP header + 'message type' option + 'end' option must be sent. */

  DhcpMsgLen += TcpIp_DhcpV4_VAddParamReqListOpt(IpCtrlIdx, NULL_PTR); /* get length of parameter request option */

  /* add message specific option lengths */
  switch (MsgType)
  {
  case TCPIP_DHCPV4_MSG_DISCOVER:
    Result = E_OK;
    break;

  case TCPIP_DHCPV4_MSG_REQUEST:
    if( (TCPIP_DHCPV4_CTRL_STATE_SELECTING  == TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx)) ||
        (TCPIP_DHCPV4_CTRL_STATE_REQUESTING == TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx))
    /*  (TCPIP_DHCPV4_CTRL_STATE_INITREBOOT == TcpIp_DhcpV4_RAM_CtrlState(CtrlIdx)) */ /* mode not supported */ )
    {
      /* option 50, requested IP address option */
      DhcpMsgLen += TcpIp_DhcpV4_VAddReqIpAddrOption(NULL_PTR, 0); /* get length of requested ip address option */

      /* option 54, server identifier option */
      DhcpMsgLen += TcpIp_DhcpV4_VAddServerIdOption(NULL_PTR, 0); /* get length of server identifier option */
    }
    else
    {
      /* TCPIP_DHCPV4_CTRL_STATE_BOUND, TCPIP_DHCPV4_CTRL_STATE_RENEWING */ /* TCPIP_DHCPV4_CTRL_STATE_REBINDING ?? */
    }
    Result = E_OK;
    break;

  default:
    Result = E_NOT_OK;
  }

#if (TCPIP_DHCPV4_SUPPORT_OPTIONS == STD_ON)
# if (TCPIP_DHCPV4_TX_OPTION_CNT > 0)
  /* add user options lengths */
  DhcpMsgLen += TcpIp_DhcpV4_VAddUserOptions(IpCtrlIdx, MsgType, NULL_PTR); /* get length of all user options */
# endif
#endif

  UdpLenByte = DhcpMsgLen + 8;  /* Add UDP Header length */
  BufLenByte = UdpLenByte;
  IpV4AddrId = TcpIp_DhcpV4_IpAddrId[IpCtrlIdx];

  /* request IP buffer */
  if( BUFREQ_OK == IpV4_Ip_ProvideTxBuffer(&IpV4AddrId, (TcpIp_SockIdxType)TcpIp_DhcpV4_SockHnd, &Dest,&BufPtr, &BufLenByte, &IpTxReqestDescr, FALSE) )
  {
    /* Build UDP Header */
    TcpIp_Udp_HdrType *UdpHdrPtr;
    UdpHdrPtr = (TcpIp_Udp_HdrType*)BufPtr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */

    UdpHdrPtr->SrcPort  = IPV4_UINT8_HTONS(TCPIP_DHCPV4_CLIENT_PORT);
    UdpHdrPtr->TgtPort  = IPV4_UINT8_HTONS(TCPIP_DHCPV4_SERVER_PORT);
    UdpHdrPtr->LenByte  = IPV4_HTONS(UdpLenByte);
    UdpHdrPtr->Checksum = 0;
    /* Checksum is calculated by lower layer (IPv4) or in hardware) */

    {
      /* Assemble DHCP message into provided tx buffer */
      TcpIp_DhcpV4_FrameType *DhcpTxBufPtr = (TcpIp_DhcpV4_FrameType *)(&BufPtr[8]);  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */

      DhcpTxBufPtr->Op     = TCPIP_DHCPV4_OP_BOOTREQUEST;
      DhcpTxBufPtr->Htype  = TCPIP_DHCPV4_HW_TYPE_ETH;
      DhcpTxBufPtr->Hlen   = TCPIP_PHYS_ADDR_LEN_BYTE;
      DhcpTxBufPtr->Hops   = 0;

      if(TCPIP_DHCPV4_CTRL_STATE_INIT == TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx))
      {
        /* generate new Session ID */
        uint16 RandNo1 = TcpIp_GetRandNoFct();
        uint16 RandNo2 = TcpIp_GetRandNoFct();
        TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).Xid = ((uint32)RandNo1) | (((uint32)RandNo2)<<16);
      }

      DhcpTxBufPtr->Xid    = TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).Xid;
      DhcpTxBufPtr->Secs   = IPV4_HTONS((uint16)TcpIp_DhcpV4_RAM_SecTimer(IpCtrlIdx));

      DhcpTxBufPtr->Flags  = 0; /* message specific flags will be set below */

      if( (TCPIP_DHCPV4_CTRL_STATE_BOUND     == TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx)) ||
          (TCPIP_DHCPV4_CTRL_STATE_RENEWING  == TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx)) ||
          (TCPIP_DHCPV4_CTRL_STATE_REBINDING == TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx)) )
      {
        DhcpTxBufPtr->Ciaddr = TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).Ciaddr; /* set client address */
      }
      else
      {
        DhcpTxBufPtr->Ciaddr = 0;  /* msg DICOVER, and msg REQUEST in state SELECTING */
      }

      DhcpTxBufPtr->Yiaddr = 0; /* set 'your' (client) IP address. (only set by servers) */
      DhcpTxBufPtr->Siaddr = 0; /* set IP address of next server to use in bootstrap. (only set by servers) */
      DhcpTxBufPtr->Giaddr = 0; /* set Relay agent IP address (only set by relay agents) */

      TCPIP_LL_GetPhysAddr(0, &DhcpTxBufPtr->Chaddr[0]); /* add mac address to first 6 bytes of 16 byte hardware address field */
      IpBase_Fill((IpBase_CopyDataType*)&DhcpTxBufPtr->Chaddr[ETH_PHYS_ADDR_LEN_BYTE], 0, sizeof(DhcpTxBufPtr->Chaddr) - ETH_PHYS_ADDR_LEN_BYTE); /* fill with 10 byte padding (zeroes) */  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */

      IpBase_Fill((IpBase_CopyDataType*)&DhcpTxBufPtr->Sname[0], 0, sizeof(DhcpTxBufPtr->Sname)); /* clear 'server name' field */  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */
      IpBase_Fill((IpBase_CopyDataType*)&DhcpTxBufPtr->File[0],  0, sizeof(DhcpTxBufPtr->File));  /* clear 'boot file' field */  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */

      DhcpTxBufPtr->MagicPattern = TCPIP_DHCPV4_MAGIC_PATTERN;

      /* Append DHCP options */
      {
        uint8  OptLen;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */
        uint8 *OptPtr = &((uint8*)DhcpTxBufPtr)[TCPIP_DHCPV4_MIN_HEADER_LENGTH_BYTE];  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
        uint16 OptOfs = 0;

        /* option 53, DHCP message type option */
        OptLen            = TCPIP_DHCPV4_OPT_DHCPV4_MSG_LEN +2;
        OptPtr[OptOfs+0]  = TCPIP_DHCPV4_OPT_DHCPV4_MSG_TYPE;
        OptPtr[OptOfs+1]  = TCPIP_DHCPV4_OPT_DHCPV4_MSG_LEN;
        OptPtr[OptOfs+2]  = MsgType;
        OptOfs           += OptLen;

        OptOfs += TcpIp_DhcpV4_VAddParamReqListOpt(IpCtrlIdx, &OptPtr[OptOfs]);

        /* set message specific flags and options */
        if (E_OK == Result)
        {
          switch (MsgType)
          {
          case TCPIP_DHCPV4_MSG_DISCOVER:
            DhcpTxBufPtr->Flags = IPV4_HTONS(TCPIP_DHCPV4_FLAGS_BROADCAST);
            break;

          case TCPIP_DHCPV4_MSG_REQUEST:
            DhcpTxBufPtr->Flags = IPV4_HTONS(TCPIP_DHCPV4_FLAGS_BROADCAST);

            if( (TCPIP_DHCPV4_CTRL_STATE_SELECTING  == TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx)) ||
                (TCPIP_DHCPV4_CTRL_STATE_REQUESTING == TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx))
            /*  (TCPIP_DHCPV4_CTRL_STATE_INITREBOOT == TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx)) */ /* mode not supported */ )
            {
              /* option 50, requested IP address option */
              OptOfs += TcpIp_DhcpV4_VAddReqIpAddrOption(&OptPtr[OptOfs], TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).Ciaddr);

              /* option 54, server identifier option */
              OptOfs += TcpIp_DhcpV4_VAddServerIdOption(&OptPtr[OptOfs], TcpIp_DhcpV4_RAM_DataStruct(IpCtrlIdx).DhcpSiaddr);
            }
            else
            {
              /* TCPIP_DHCPV4_CTRL_STATE_BOUND, TCPIP_DHCPV4_CTRL_STATE_RENEWING */ /* TCPIP_DHCPV4_CTRL_STATE_REBINDING ?? */
            }
            break;

          default:
            DhcpTxBufPtr->Flags = 0;
          }
        }

        if (((DhcpTxBufPtr->Flags & IPV4_HTONS(TCPIP_DHCPV4_FLAGS_BROADCAST)) != 0) && (TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx) < TCPIP_DHCPV4_CTRL_STATE_BOUND))
        {
          /* DHCP messages broadcast by a client prior to that client obtaining
             its IP address must have the source address field in the IP header
             set to 0. [RFC2131 4.1 Constructing and sending DHCP messages] */
          IPV4_SET_FLAG_U8(IpV4_Ip_TransferBlock[TcpIp_DhcpV4_SockHnd].Flags, IPV4_IP_TRANSFER_BLOCK_FLAG_ZERO_SRC_ADDR);
        }
        else
        {
          IPV4_CLR_FLAG_U8(IpV4_Ip_TransferBlock[TcpIp_DhcpV4_SockHnd].Flags, IPV4_IP_TRANSFER_BLOCK_FLAG_ZERO_SRC_ADDR);
        }

#if (TCPIP_DHCPV4_SUPPORT_OPTIONS == STD_ON)
# if (TCPIP_DHCPV4_TX_OPTION_CNT > 0)
        /* add user options */
        OptOfs += TcpIp_DhcpV4_VAddUserOptions(IpCtrlIdx, MsgType, &OptPtr[OptOfs]);
# endif
#endif

        /* option 255, end option */
        OptPtr[OptOfs]  = TCPIP_DHCPV4_OPT_END_TYPE;
        OptOfs++;

#if ( TCPIP_DEV_ERROR_REPORT == STD_ON )
        /* Check that created message is as long as calculated at beginning of this function */
        if ((TCPIP_DHCPV4_MIN_HEADER_LENGTH_BYTE + OptOfs) != DhcpMsgLen)
        {
          TcpIp_CallDetReportError(TCPIP_DHCPV4_API_ID_V_SEND_DHCP_MSG, TCPIP_E_INV_ARG)
        }
#endif
      }
    }

    /* send data */
    if(TCPIP_OK == IpV4_Ip_Transmit(&IpTxReqestDescr, UdpLenByte, TCPIP_PROTOCOL_UDP, FALSE, 0, FALSE))
    {
      CANOE_WRITE_STRING_1("TcpIp_DhcpV4_VSendDhcpMsg: DHCP message sent with %d byte", UdpLenByte);
      return E_OK;
    }
    else
    {
      TCPIP_DUMMY_ASM_NOP
        return E_NOT_OK;
      /* if TcpIp_ProvideTxBuffer(), TcpIp_TransmitTo() may not fail */
    }
  }
  else
  {
    return E_NOT_OK;
  }

}  /* PRQA S 2006 */ /* MD_MSR_14.7 */  /* PRQA S 6010 */ /* MD_MSR_STPTH */  /* PRQA S 6030 */ /* MD_MSR_STCYC */  /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (TCPIP_DHCPV4_SUPPORT_OPTIONS == STD_ON)
# if (TCPIP_DHCPV4_RX_OPTION_CNT > 0)
/***********************************************************************************************************************
 *  TcpIp_DhcpV4_VHandleUserOptions
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_VHandleUserOptions(
  uint8 IpCtrlIdx,
  uint8 MsgType,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) OptDataPtr,
  uint16 LenByte)
{
  uint8 OptCode;
  uint8 OptLen;
  uint16 OptOfs = 0;

  while (OptOfs < LenByte)
  {
    OptCode = OptDataPtr[OptOfs];

    if (TCPIP_DHCPV4_OPT_PAD_TYPE == OptCode)
    {
      OptOfs++;
      continue;  /* PRQA S 0770 */ /* MD_TCPIP_14.5_0770 */
    }

    if (TCPIP_DHCPV4_OPT_END_TYPE == OptCode)
    {
      break;
    }

    OptLen = OptDataPtr[OptOfs+1];

    if (((OptOfs + 2) + OptLen) <= LenByte)
    {
      TcpIp_RxDhcpV4OptFcts(IpCtrlIdx, OptCode, OptLen, &OptDataPtr[OptOfs+2], MsgType);
    }

    OptOfs += 2 + OptLen;
  }

  return E_OK;
} /* End of TcpIp_DhcpV4_VHandleUserOptions() */
# endif

# if ((TCPIP_DHCPV4_SUPPORT_OPTIONS == STD_ON) && (TCPIP_DHCPV4_TX_OPTION_CNT > 0))
/***********************************************************************************************************************
 *  TcpIp_DhcpV4_VGetTxOption
 **********************************************************************************************************************/
/*! \brief      Get user option data for outgoing DHCPv4 messages.
 *  \description  By this API service the TCP/IP stack reads the DHCP option data identified by parameter option.
 *  \param[in]  IpAddrId       IP address identifier representing the local IP address and EthIf controller for which
*                              the DHCP option shall be read.
 *  \param[in]  OptId          DHCP option, e.g. Host Name
 *  \param[in]  OptLenPtr      length of DHCP option data
 *  \param[in]  OptPtr         Pointer to memory containing DHCP option data
 *  \param[out] MsgTypesPtr    Define for which DHCP message types the option are sent
 *  \return     E_OK           no error occured.
 *              E_NOT_OK       DHCP option data could not be read.
 *  \note       Currently only the option 'TCPIP_DHCPV4_OPT_ID_CLIENT_FQDN' is supported.
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_VGetTxOption(
    uint8 IpAddrId,
    uint8 OptId,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) OptLenPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) OptPtr,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) MsgTypesPtr) /* PRQA S 3673 */ /* MD_IPV4_16.7_3673 */
{
  uint8_least OptSelector;
  Std_ReturnType RetValue;
  uint8 IpCtrlIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  /* Currently the implementation is limited to one option that is always sent, but the API is already prepared to do a more generic handling */
  IPV4_UNUSED_PARAMETER(MsgTypesPtr);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_OFF)
  IPV4_UNUSED_PARAMETER(IpAddrId);     /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  
#endif

  IPV4_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx, IpAddrId);

  if(IpV4_Ip_VCfgIsAddrAssignmentEnabled(IPV4_IP_ADDR_ASSIGNMENT_METHOD_DHCP) == FALSE)
  {
    return E_NOT_OK;
  }

  /* check for valid DHCP option */
  for(OptSelector = 0; OptSelector < TCPIP_DHCPV4_TX_OPTION_CNT; OptSelector++)
  {
    if(TcpIp_DhcpV4_TxOptionId[OptSelector] == OptId)
    {
      break;
    }
  }

  if(OptSelector < TCPIP_DHCPV4_TX_OPTION_CNT)
  {
    /* currently only one single option (FQDN-Option) is supported */
    if(OptId == TCPIP_DHCPV4_OPT_ID_CLIENT_FQDN)
    {
      if (*OptLenPtr >= TcpIp_DhcpV4_RAM_HostNameLen(IpCtrlIdx))
      {
        /* copy length and host name */
        *OptLenPtr = TcpIp_DhcpV4_RAM_HostNameLen(IpCtrlIdx);
        if (*OptLenPtr > 0)
        {
          IpBase_Copy((IpBase_CopyDataType*)OptPtr, (const IpBase_CopyDataType*)&TcpIp_DhcpV4_RAM_HostName(IpCtrlIdx)[0], *OptLenPtr);  /* PRQA S 0310 */ /* MD_IpV4_11.04 */
        }
        RetValue = E_OK;
      }
      else
      {
        /* error, provided buffer is too short */
        RetValue = E_NOT_OK;
      }
    }
    else
    {
      /* error, not supported DHCPoption requested */
      RetValue = E_NOT_OK;
    }
  }
  else
  {
    /* error, no matching supported DHCP option found */
    RetValue = E_NOT_OK;
  }

  return RetValue;
} /* End of TcpIp_DhcpV4_VGetTxOption() */  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_VSetTxOption
 **********************************************************************************************************************/
/*! \brief      Set user option data for outgoing DHCPv4 messages.
 *  \description  By this API service the TCP/IP stack writes the DHCP option data identified by parameter option.
 *  \param[in]  IpAddrId       IP address identifier representing the local IP address and EthIf controller for which
*                              the DHCP option shall be written.
 *  \param[in]  OptId          DHCP option, e.g. Host Name
 *  \param[in]  OptLen         length of DHCP option data
 *  \param[in]  OptPtr         Pointer to memory containing DHCP option data
 *  \param[in]  MsgTypes       Define for which DHCP message types the option shall be sent
 *  \return     E_OK           no error occured.
 *              E_NOT_OK       DHCP option data could not be written.
 *  \note       Currently only the option 'TCPIP_DHCPV4_OPT_ID_CLIENT_FQDN' is supported.
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_VSetTxOption(
    uint8 IpAddrId,
    uint8 OptId,
    uint8 OptLen,
    P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) OptPtr,
    uint8 MsgTypes)
{
  uint8_least OptSelector;
  Std_ReturnType RetValue;
  uint8 IpCtrlIdx;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  /* Currently the implementation is limited to one option that is always sent, but the API is already prepared to do a more generic handling */
  IPV4_UNUSED_PARAMETER(MsgTypes);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  
#if (IPV4_MULTI_CONTROLLER_SUPPORT == STD_OFF)
  IPV4_UNUSED_PARAMETER(IpAddrId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  
#endif

  IPV4_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx, IpAddrId);

  if(IpV4_Ip_VCfgIsAddrAssignmentEnabled(IPV4_IP_ADDR_ASSIGNMENT_METHOD_DHCP) == FALSE)
  {
    return E_NOT_OK;
  }

  /* check for valid DHCP option */
  for(OptSelector = 0; OptSelector < TCPIP_DHCPV4_TX_OPTION_CNT; OptSelector++)
  {
    if(TcpIp_DhcpV4_TxOptionId[OptSelector] == OptId)
    {
      break;
    }
  }

  if(OptSelector < TCPIP_DHCPV4_TX_OPTION_CNT)
  {
    /* currently only one single option is supported */
    if(OptId == TCPIP_DHCPV4_OPT_ID_CLIENT_FQDN)
    {
      /* An old API for setting a host name was TcpIp_DhcpV4_SetHostName(). */

      if (OptLen <= TcpIp_DhcpV4_VCfgGetHostNameLenMax())
      {
        /* copy length and host name */
        TcpIp_DhcpV4_RAM_HostNameLen(IpCtrlIdx) = OptLen;
        IpBase_Copy((IpBase_CopyDataType*)&TcpIp_DhcpV4_RAM_HostName(IpCtrlIdx)[0], (const IpBase_CopyDataType*)OptPtr, OptLen);  /* PRQA S 0310 */ /* MD_IpV4_11.04 */
        RetValue = E_OK;
      }
      else
      {
        /* error, fqdn option data is too long */
        RetValue = E_NOT_OK;
      }
    }
    else
    {
      /* error, not supported DHCPoption requested */
      RetValue = E_NOT_OK;
    }
  }
  else
  {
    /* error, no matching supported DHCP option found */
    RetValue = E_NOT_OK;
  }

  return RetValue;
} /* End of TcpIp_DhcpV4_VSetTxOption() */  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_VAddUserOptions
 **********************************************************************************************************************/
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_DhcpV4_VAddUserOptions(
    uint8                                   IpCtrlIdx,
    uint8                                   MsgType,
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) MsgDataPtr)
{
  uint16 MsgDataOfs = 0;

  /* Currently the implementation is limited to one option that is always sent, but the API is already prepared to do a more generic handling */
  IPV4_UNUSED_PARAMETER(MsgType);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  

  /* currently only one single option (FQDN-Option) is supported, and this option is sent in all DHCP messages */
  if(0 < TcpIp_DhcpV4_RAM_HostNameLen(IpCtrlIdx))
  {
    if (NULL_PTR != MsgDataPtr)
    {
      /* copy the option to the buffer */
      MsgDataPtr[0] = TCPIP_DHCPV4_OPT_ID_CLIENT_FQDN;  /* option id */
      MsgDataPtr[1] = TcpIp_DhcpV4_RAM_HostNameLen(IpCtrlIdx) +3;  /* option length */
      /* IPv4
          0 1 2 3 4 5 6 7
         +-+-+-+-+-+-+-+-+
         |  MBZ  |N|E|O|S|
         +-+-+-+-+-+-+-+-+
         S = 0   the server SHOULD NOT perform the A RR (FQDN-to-address) DNS updates
         O = 0   must be 0
         E = 1   canonical wire format
         N = 1   the server SHOULD NOT perform any DNS updates
      */
      MsgDataPtr[2] = 0x0C;  /* set flags N and E */
      MsgDataPtr[3] = 0;     /* RCODE1, is always 0 */
      MsgDataPtr[4] = 0;     /* RCODE2, is always 0 */
      IpBase_Copy((IpBase_CopyDataType*)&MsgDataPtr[5], (const IpBase_CopyDataType*)&TcpIp_DhcpV4_RAM_HostName(IpCtrlIdx)[0], TcpIp_DhcpV4_RAM_HostNameLen(IpCtrlIdx));  /* option data */ /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_copy */
    }
    MsgDataOfs += 5 + TcpIp_DhcpV4_RAM_HostNameLen(IpCtrlIdx);  /* increase the offset */
  }

  return MsgDataOfs;
} /* End of TcpIp_DhcpV4_VAddUserOptions() */
# endif
#endif

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_VAddReqIpAddrOption
 **********************************************************************************************************************/
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4_VAddReqIpAddrOption(
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) OptPtr,
    IpBase_AddrInType                       Yiaddr)
{
  if (NULL_PTR != OptPtr)
  {
    OptPtr[0] = TCPIP_DHCPV4_OPT_REQ_IP_ADDR_TYPE;
    OptPtr[1] = TCPIP_DHCPV4_OPT_REQ_IP_ADDR_LEN;
    *((TcpIp_NetAddrType *)(&OptPtr[2])) = Yiaddr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
  }

  return (2 + TCPIP_DHCPV4_OPT_REQ_IP_ADDR_LEN);
} /* End of TcpIp_DhcpV4_VAddReqIpAddrOption() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_VAddServerIdOption
 **********************************************************************************************************************/
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4_VAddServerIdOption(
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) OptPtr,
    IpBase_AddrInType                       Siaddr)
{
  if (NULL_PTR != OptPtr)
  {
    OptPtr[0] = TCPIP_DHCPV4_OPT_SERVER_ID_TYPE;
    OptPtr[1] = TCPIP_DHCPV4_OPT_SERVER_ID_LEN;
    *((TcpIp_NetAddrType *)(&OptPtr[2])) = Siaddr;  /* PRQA S 0310 */ /* MD_TCPIP_11.4_0310_struct */
  }

  return (2 + TCPIP_DHCPV4_OPT_SERVER_ID_LEN);
} /* End of TcpIp_DhcpV4_VAddServerIdOption() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_VAddParamReqListOpt
 **********************************************************************************************************************/
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DhcpV4_VAddParamReqListOpt(
  uint8                                   IpCtrlIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_VAR) OptPtr)
{
  uint8 OptLen = 3;  /* PRQA S 0781 */ /* MD_TCPIP_5.6_0781 */

  if (NULL_PTR != OptPtr)
  {
    OptPtr[0] = TCPIP_DHCPV4_OPT_PARAM_REQ_LIST_TYPE;
    /* OptPtr[1] (option length) will be set at end of this function */
    OptPtr[2] = TCPIP_DHCPV4_OPT_SUBNET_MASK_TYPE;
    OptPtr[3] = TCPIP_DHCPV4_OPT_ROUTER_TYPE;
    OptPtr[4] = TCPIP_DHCPV4_OPT_DOMAIN_NAME_SERVER_TYPE;
  }

#if ((TCPIP_DHCPV4_SUPPORT_OPTIONS == STD_ON) && (TCPIP_DHCPV4_REQ_OPTION_CNT > 0))  /* PRQA S 3332 */ /* MD_TCPIP_19.11_3332 */
  {
    uint16 ReqOptIdx = 0;
    for (ReqOptIdx = 0; ReqOptIdx < TCPIP_DHCPV4_REQ_OPTION_CNT; ReqOptIdx++)
    {
      if ((TRUE == TcpIp_DhcpV4_ReqOptConstData[ReqOptIdx].SendOnAllCtrls) || (TcpIp_DhcpV4_ReqOptConstData[ReqOptIdx].IpCtrlIdx == IpCtrlIdx))
      {
        if (NULL_PTR != OptPtr)
        {
          OptPtr[5 + ReqOptIdx] = TcpIp_DhcpV4_ReqOptConstData[ReqOptIdx].OptId;
        }
        OptLen++;
      }
    }
  }
#else
  IPV4_UNUSED_PARAMETER(IpCtrlIdx);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */  
#endif

  if (NULL_PTR != OptPtr)
  {
    OptPtr[1] = OptLen;
  }

  return (2 + OptLen);
} /* end of TcpIp_DhcpV4_VAddParamReqListOpt() */

/***********************************************************************************************************************
 *  TcpIp_DhcpV4_GetStatus
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_GetStatus(
    IpV4_AddrIdType IpAddrId,
    CONSTP2VAR(uint8, AUTOMATIC, IPV4_APPL_DATA) DhcpStatePtr)
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
  else if(DhcpStatePtr == NULL_PTR)
  {
    errorId = IPV4_E_INV_POINTER;
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
      *DhcpStatePtr = 0;
      /* #10 Verify that DHCP is configured for this IP controller. */
      if(TCPIP_DHCPV4_IS_ENABLED_ON_CTRL)
      {
        /* #20 Verify that IP instance is in state ONLINE. */
        if(IPV4_CTRL_VAR(IpV4_Ip_CtrlState) == IPV4_IP_ONLINE_ACTIVE)
        {
          retVal = E_OK;
          /* #30 Convert the internal IpCtrlIdx-specific status to the simplified status desired by the caller. */
          switch(TcpIp_DhcpV4_RAM_CtrlState(IpCtrlIdx))
          {
            case TCPIP_DHCPV4_CTRL_STATE_OFF:
              /* no break; */
            case TCPIP_DHCPV4_CTRL_STATE_INIT:
              *DhcpStatePtr = TCPIP_DHCP_SIMPLIFIED_STATE_NO_DISCVR_SENT;
              break;
            case TCPIP_DHCPV4_CTRL_STATE_SELECTING:
              *DhcpStatePtr = TCPIP_DHCP_SIMPLIFIED_STATE_DISCVR_SENT_NO_ANSWR_RCVD_YET;
              break;
            case TCPIP_DHCPV4_CTRL_STATE_REQUESTING:
              *DhcpStatePtr = TCPIP_DHCP_SIMPLIFIED_STATE_REQ_SENT_NO_ACK_RCVD_YET;
              break;
            case TCPIP_DHCPV4_CTRL_STATE_BOUND: /* Address Lease valid. */
              /* no break; */
            case TCPIP_DHCPV4_CTRL_STATE_RENEWING: /* Address Lease valid. */
              /* no break; */
            case TCPIP_DHCPV4_CTRL_STATE_REBINDING: /* Address Lease valid. */
              *DhcpStatePtr = TCPIP_DHCP_SIMPLIFIED_STATE_ACK_RCVD_DHCP_ADDR_ASSIGND;
              break;
            case TCPIP_DHCPV4_CTRL_STATE_INITREBOOT: /* mode not supported by this DHCP impl. */
              /* no break; */
            case TCPIP_DHCPV4_CTRL_STATE_REBOOTING: /* mode not supported by this DHCP impl. */
              /* no break; */
            default:
              retVal = E_NOT_OK;
              errorId = IPV4_E_INV_PARAM;
          }
        }
        else
        {
          /* DHCP configured but controller not active, i.e. DHCP client must be inactive for sure. */
          *DhcpStatePtr = TCPIP_DHCP_SIMPLIFIED_STATE_NOT_ACTIVE;
          retVal = E_OK;
        }
      }
      /* else: DHCP not configured, i.e. invalid condition to call this API: Leave retVal E_NOT_OK */
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (IPV4_DEV_ERROR_REPORT == STD_ON)
  if(errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(IPV4_MODULE_ID, IPV4_VINSTANCE_ID, TCPIP_DHCPV4_API_ID_GET_STATUS, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return retVal;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */  

#define TCPIP_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* module specific MISRA deviations:

   module global deviations -> see file TcpIp.c

   DHCP-specific deviations:
   MD_TCPIP_DHCPV4_14.6_0771_a:
      Reason:     The break statement is used multiple (two) times because parsing a received DHCP message can have multiple fail conditions that should cause stopps.
                  Using breaks makes the code much easier to unterstand.
      Risk:       Unstructured code should be avoided.
      Prevention: Covered by code review.
   MD_TCPIP_DHCP_14.6_3333:
     Reason:     Break statement used to exit from a switch case block in case the received message is not a message that is expected here.
     Risk:       none
     Prevention: Covered by code review.

*/

#endif  /* (TCPIP_SUPPORT_IPV4 == STD_ON) && (TCPIP_SUPPORT_DHCPV4 == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_DhcpV4.c
 *********************************************************************************************************************/
