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
 *         \file  TcpIp_Priv_Types.h
 *        \brief  Implementation of TcpIp Module - Internal Type Definitions
 *
 *      \details  This file is part of the TcpIp module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/

#if !defined (TCPIP_PRIV_TYPES_H)
# define TCPIP_PRIV_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "TcpIp_Types.h"
#include "IpBase_Types.h"

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef struct
{
  IpBase_AddrInType  RemAddr;  /* remote IP address */
  IpBase_AddrInType  LocAddr;  /* local IP address */
  uint8              Null;
  uint8              Protocol;
  uint16             LenByte;
} TcpIp_PseudoHdrV4Type;

typedef struct
{
  IpBase_AddrIn6Type RemAddr;  /* remote IP address */
  IpBase_AddrIn6Type LocAddr;  /* local IP address */
  uint8              Null;
  uint8              Protocol;
  uint16             LenByte;
} TcpIp_PseudoHdrV6Type;

typedef uint32 TcpIp_PseudoHdrBlockV4Type[3];
typedef uint32 TcpIp_PseudoHdrBlockV6Type[9];

#endif
  /* TCPIP_PRIV_TYPES_H */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_Priv_Types.h
 *********************************************************************************************************************/
