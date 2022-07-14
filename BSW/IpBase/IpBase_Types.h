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
 *         File:  IpBase_Types.h
 *      Project:  MICROSAR IP
 *       Module:  MICROSAR IP Base
 *    Generator:  None
 *
 *  Description:  Types header
 *  
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (IPBASE_TYPES_H)
# define IPBASE_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* PRQA S 857 NO_OF_MACROS */ /* MD_IpBase_857 */
# include "IpBase_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define IPBASE_TCP_EVENT_RESET         0x01u
# define IPBASE_TCP_EVENT_CLOSED        0x02u
# define IPBASE_TCP_EVENT_FIN_RECEIVED  0x03u
# define IPBASE_STR_LEN_INVALID         0xFFFFFFFFu
# define IPBASE_PORT_LEN_BYTE           0x02u
# define IPBASE_UCHAR_MAX               255u        /* as UCHAR_MAX in <limits.h> */
# define IPBASE_USHRT_MAX               65535u      /* as USHRT_MAX in <limits.h> */
# define IPBASE_ULONG_MAX               4294967295u /* as ULONG_MAX in <limits.h> */
# define IPBASE_SLONG_MAX               2147483647  /* as LONG_MAX in <limits.h> */
# define IPBASE_UCHAR_DEC_DIGITS_MAX    3u          /*  3 decimal digits */
# define IPBASE_USHRT_DEC_DIGITS_MAX    5u          /*  5 decimal digits */
# define IPBASE_ULONG_DEC_DIGITS_MAX    10u         /* 10 decimal digits */
# define IPBASE_UCHAR_HEX_DIGITS_MAX    2u          /*  2 hexadecimal digits */
# define IPBASE_USHRT_HEX_DIGITS_MAX    4u          /*  4 hexadecimal digits */
# define IPBASE_ULONG_HEX_DIGITS_MAX    8u          /*  8 hexadecimal digits */

# define IPBASE_AF_UNSPEC               0x0000u
# define IPBASE_AF_INET                 0x0002u
# define IPBASE_AF_INET6                0x001Cu     /* changed from 0x000Au */
# define IPBASE_AF_INVALID              0xFFFFu

# define IPBASE_ETH_PHYS_ADDR_LEN_BYTE     6

# if ( IPBASE_DEFINE_E_PENDING == STD_ON )
#  if defined E_PENDING
#  else
#   define E_PENDING                      2
#  endif
  /* E_PENDING */
# endif
  /* IPBASE_DEFINE_E_PENDING */
/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453,3458 7 */ /* MD_MSR_19.4 MD_MSR_19.7 */
# define IPBASE_P2CONSTAPPLDATA(Type)      P2CONST(Type, AUTOMATIC, IPBASE_APPL_DATA)
# define IPBASE_CONSTP2CONSTAPPLDATA(Type) CONSTP2CONST(Type, AUTOMATIC, IPBASE_APPL_DATA)
# define IPBASE_CONSTP2VARAPPLDATA(Type)   CONSTP2VAR(Type, AUTOMATIC, IPBASE_APPL_DATA)
# define IPBASE_P2VARAPPLDATA(Type)        P2VAR(Type, AUTOMATIC, IPBASE_APPL_DATA)
# define IPBASE_VARNOINIT(Type)            VAR(Type, IPBASE_VAR_NOINIT)
# define IPBASE_CONSTCONST(Type)           CONST(Type, IPBASE_CONST)
# define IPBASE_FUNCCODE(Type)             FUNC(Type, IPBASE_CODE)

/* PRQA L:NO_OF_MACROS */
/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint32    IpBase_AddrInType;
typedef uint32    IpBase_IPAddressType;
typedef uint16    IpBase_FamilyType;
typedef uint16    IpBase_PortType;
typedef uint8     IpBase_EthPhysAddrType[IPBASE_ETH_PHYS_ADDR_LEN_BYTE];
typedef uint8     IpBase_CopyDataType;
typedef uint8     IpBase_SockIdxType;
typedef uint8     IpBase_ReturnType;
typedef uint8     IpBase_TcpIpEventType;

typedef struct
{
  uint8  *payload;
  uint32 totLen;
  uint16 len;
} IpBase_PbufType;
typedef struct
{
  uint16               port;
  uint8                len;
  IpBase_IPAddressType addr;
} IpBase_IpAddrPortType;



typedef struct
{
  IpBase_FamilyType  sa_family;
  uint8              sa_data[1]; /* dummy length */
} IpBase_SockAddrType;
/*  +-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-+
 *  |          sin6_family          |                               |
 *  +-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|                               +
 *  .                            sa_data                            .
 *  .                                                               .
 *  +-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-+
 */

typedef struct
{
  IpBase_FamilyType  sin_family;
  IpBase_PortType    sin_port;
  IpBase_AddrInType  sin_addr;
} IpBase_SockAddrInType;
/*  +-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-+
 *  |           sin_family          |            sin_port           |
 *  +-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-+
 *  |                            sin_addr                           |
 *  +-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-+
 */

/* PRQA S 750 5 */ /* MD_MSR_18.4 */
typedef union
{
  uint8 addr[16];
  uint32 addr32[4];
} IpBase_AddrIn6Type; 

typedef struct
{
  IpBase_FamilyType  sin6_family;
  IpBase_PortType    sin6_port;
  IpBase_AddrIn6Type sin6_addr;
} IpBase_SockAddrIn6Type;
/*  +-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-+
 *  |          sin6_family          |           sin6_port           |
 *  +-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                           sin6_addr                           +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-|-+-+-+-+-+-+-+-+
 */
#endif 
  /* IPBASE_TYPES_H */
/**********************************************************************************************************************
 *  END OF FILE: IpBase_Types.h
 *********************************************************************************************************************/
