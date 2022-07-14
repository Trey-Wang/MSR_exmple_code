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
 *         File:  EthFw_Types.h
 *       Module:  SysService_FwEth
 *    Generator:  SysService_FwEth.jar (DaVinci Configurator Pro)
 *
 *  Description:  Types definitions of Firewall for Ethernet (ETHFW)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined(ETHFW_TYPES_H)
#define ETHFW_TYPES_H

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA S 0777, 0779 EOF */ /* MD_MSR_5.1 */

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "EthFw_Cfg.h"
#include "Std_Types.h"
#include "IpBase_Types.h"
#include "EthIf.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8 EthFw_StateType;

#define ETHFW_BYTES_PER_32WORD                4U
#define ETHFW_BYTES_PER_64WORD                8U

typedef enum
{
  ETHFW_DIRECTION_RX                          = 0U,
  ETHFW_DIRECTION_TX                          = 1U
} EthFw_DirectionType;

#define ETHFW_MAC_ADDR_SIZE                   6
typedef uint8                                 EthFw_EthPhysAddrType[ETHFW_MAC_ADDR_SIZE];

typedef enum
{
  ETHFW_ETHERTYPE_IPV4                        = 0x0800U,
  ETHFW_ETHERTYPE_IPV6                        = 0x86DDU
} EthFw_EthertypeType;

#define ETHFW_AVB_STREAMID_SIZE               8
#define ETHFW_AVB_STREAMID_OFFSET             4
typedef uint8                                 EthFw_AvbStreamIdType[ETHFW_AVB_STREAMID_SIZE];

typedef enum
{
  ETHFW_AF_INVALID =                          0xFFFFU,
  ETHFW_AF_INET =                             0x0002U,
  ETHFW_AF_INET6 =                            0x001CU
} EthFw_FamilyType;

#define ETHFW_IP_VERSION_OFFSET               0U
#define ETHFW_IP_VERSION_4                    4U
#define ETHFW_IP_VERSION_6                    6U

#define ETHFW_IPV4_HEADER_SIZE                20U
#define ETHFW_IPV4_IHL_OFFSET                 0U
#define ETHFW_IPV4_IHL_MASK                   0x0FU
#define ETHFW_IPV4_TOTALLENGTH_OFFSET         2U
#define ETHFW_IPV4_PROTOCOL_OFFSET            9U
#define ETHFW_IPV4_ADDR_SIZE                  4U
#define ETHFW_IPV4_SRC_ADDR_OFFSET            12U
#define ETHFW_IPV4_DEST_ADDR_OFFSET           16U

/* PRQA S 3453 FCT_MACROS */ /* MD_MSR_19.7 */
#define ETHFW_GET_LEN32_IN_BYTES(Value)       ((Value) * ETHFW_BYTES_PER_32WORD)
#define ETHFW_GET_LEN64_IN_BYTES(Value)       ((Value) * ETHFW_BYTES_PER_64WORD)
/* PRQA L:FCT_MACROS */

#define ETHFW_IPV6_HEADER_SIZE                40U
#define ETHFW_IPV6_PAYLOAD_LENGH_OFFSET       4U
#define ETHFW_IPV6_NEXT_HEADER_OFFSET         6U
#define ETHFW_IPV6_ADDR_SIZE                  16U
#define ETHFW_IPV6_SRC_ADDR_OFFSET            8U
#define ETHFW_IPV6_DEST_ADDR_OFFSET           24U

typedef uint8                                 EthFw_AddrInType[ETHFW_IPV4_ADDR_SIZE];
typedef uint8                                 EthFw_AddrIn6Type[ETHFW_IPV6_ADDR_SIZE];

#if(ETHFW_IPV6_PRESENT == STD_ON)
typedef EthFw_AddrIn6Type                     EthFw_AddrContainerType;
#else
typedef EthFw_AddrInType                      EthFw_AddrContainerType;
#endif

typedef enum
{
  ETHFW_PROTOCOL_IPV6_HOP_BY_HOP              = 0U,
  ETHFW_PROTOCOL_TCP                          = 6U,
  ETHFW_PROTOCOL_UDP                          = 17U,
  ETHFW_PROTOCOL_IPV6_ROUTE                   = 43U,
  ETHFW_PROTOCOL_IPV6_FRAG                    = 44U,
  ETHFW_PROTOCOL_ESP                          = 50U,
  ETHFW_PROTOCOL_AUTH                         = 51U,
  ETHFW_PROTOCOL_IPV6_NO_NEXT                 = 59U,
  ETHFW_PROTOCOL_IPV6_DST_OPTS                = 60U
} EthFw_ProtocolType;

typedef struct
{
  EthFw_FamilyType                            Family;
  uint16                                      TotalLength;
  uint16                                      PayloadOffset;
  EthFw_ProtocolType                          Protocol;
  EthFw_AddrContainerType                     SrcAddr;
  EthFw_AddrContainerType                     DstAddr;
} EthFw_IpHeaderType;

#define ETHFW_UDP_HEADER_SIZE                 8U
#define ETHFW_UDP_SRC_PORT_OFFSET             0U
#define ETHFW_UDP_DST_PORT_OFFSET             2U
#define ETHFW_UDP_LENGTH_OFFSET               4U

typedef struct
{
  uint16                                      SrcPort;
  uint16                                      DstPort;
  uint16                                      Length;
} EthFw_UdpHeaderType;

#define ETHFW_TCP_HEADER_SIZE                 20U
#define ETHFW_TCP_SRC_PORT_OFFSET             0U
#define ETHFW_TCP_DST_PORT_OFFSET             2U
#define ETHFW_TCP_DATA_OFFSET_OFFSET          12U

typedef struct
{
  uint16                                      SrcPort;
  uint16                                      DstPort;
  uint8                                       DataOffset;
} EthFw_TcpHeaderType;

#endif /* ETHFW_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: EthFw_Types.h
 *********************************************************************************************************************/
