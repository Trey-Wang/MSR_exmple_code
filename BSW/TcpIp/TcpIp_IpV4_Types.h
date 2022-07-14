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
 *         \file  TcpIp_IpV4_Types.h
 *        \brief  Implementation of Internet Protocol version 4 (IPv4) - Type Definitions
 *
 *      \details  This file is part of the TcpIp IPv4 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv4 submodule. >> TcpIp_IpV4.h
 *********************************************************************************************************************/

#if !defined (IPV4_TYPES_H)
# define IPV4_TYPES_H

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV4 == STD_ON)

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "TcpIp_IpV4_Cfg.h"

#include "Std_Types.h"
#include "TcpIp_Types.h"

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef uint8      IpV4_ConfigType;

typedef uint8          IpV4_StateType;  /* module state */

typedef uint8          IpV4_AddrIdType;

typedef struct
{
  uint8                                      OptLen;
  uint8                                      MsgFlags;
} IpV4_DhcpV4_TxOptUserDataType;

typedef void (*TcpIp_PhysAddrTableChgDiscardedCbkType)(
  uint8                         CtrlIdx,
  TCPIP_P2C(TcpIp_SockAddrType) IpAddrPtr);

typedef void (*IpV4_LinkLocalAddrCalloutFuncType)(TcpIp_LocalAddrIdType ipAddrId, uint8 conflictCount, IpBase_AddrInType * addrCandidatePtr);

#endif  /* (TCPIP_SUPPORT_IPV4 == STD_ON) */
#endif  /* IPV4_TYPES_H */
/**********************************************************************************************************************
 *  END OF FILE: IpV4_Types.h
 *********************************************************************************************************************/
