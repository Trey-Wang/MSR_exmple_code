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
 *         File:  IpBase.h
 *      Project:  MICROSAR ETH
 *    Component:  SysService_IpBase
 *       Module:  MICROSAR IP Base
 *    Generator:  None
 *
 *  Description:  Module header
 *  
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Alex Lunkenheimer             visalr        Vector Informatik GmbH
 *  Andreas Reifert               visart        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2010-01-01  visalr  -             created
 *  01.00.01  2010-11-11  visalr  -             extended by case insensitive string compare
 *  01.01.00  2010-12-09  visalr  ESCAN00082018 extended by string length (strlen())
 *  01.01.01  2010-12-15  visalr  ESCAN00047701 Missing Include for VStdLib.h
 *  01.02.00  2011-01-25  visalr  -             enlarged data types to avoid possible overflow
 *  01.02.01  2011-02-07  visalr  -             dynamic length up to uint32 in IpBase_ConvInt2String, 
                                                IpBase_ConvInt2HexString
 *            2011-02-11  visart  -             Base64: accept CR,LF in source data
 *            2011-02-16  visalr  -             IpBase_ConvInt2String wrong string pointer
 *            2011-02-16  visart  -             ASN.1 / BER decoder added
 *            2011-02-16  visalr  -             compiler abstraction adapted (use of AUTOMATIC)
 *            2011-02-16  visalr  -             compiler abstraction adapted (use of AUTOMATIC)
 *            2011-02-22  visalr  -             defines for numeric limits added IPBASE_U..._MAX
 *  01.02.02  2011-03-25  visalr  -             wrong return idx in IpBase_StrFindSubStr
 *  01.02.03  2011-04-08  visalr  -             IPBASE_CODE_BER_ENABLE added
 *            2011-05-13  visalr  -             _Appl_Rand template files added
 *  01.02.04  2011-04-08  visalr  -             _Appl_Rand #if0 removed
 *  01.02.05  2011-05-19  visalr  ESCAN00051027 Issue in convert functions between hex strings and binary arrays
 *  01.02.06  2011-08-26  visalr  ESCAN00053070 add _Appl_Time for TLS
 *  01.02.07  2011-08-26  visalr  ESCAN00053075 Change IPBASE_UCHAR_MAX to 255 
 *  01.02.08  2011-09-12  visalr  ESCAN00053495 IpBase_ConvString2IntBase: Return value IPBASE_ULONG_MAX if no number 
                                                was converted
 *  01.02.09  2011-09-12  visalr                comment adapted in IpBase_ConvString2IntBase
 *  01.02.10  2011-09-19  visalr                Appl_Rand/Time extended by default names in GENy for TcpIp and Tls
 *  01.02.11  2011-09-23  visalr                GHS compiler warnings resolved (undefined preprocessing identifier)
 *  01.02.12  2011-09-28  visalr  ESCAN00053907 Compiler-Error: return command used in void function within Appl_Time.c
 *  01.02.13  2011-09-30  visalr                support of negative integers (IpBase_ConvString2SignedIntBase added)
 *  01.02.14  2011-10-13  visalr  ESCAN00054217 Resolve compiler warnings using undefined defines
 *  01.02.15  2011-10-13  visalr  ESCAN00054217 Resolve compiler warnings using undefined defines
 *  01.02.16  2011-10-21  visalr  ESCAN00053988 Compiler warning: in _Appl_Time.c: possible loss of data
 *  01.02.17  2011-10-25  visalr  ESCAN00054429 IpBase_Code: Wrong MemMap define
 *  01.02.18  2011-12-02  visalr  ESCAN00055319 Extend functionality for IPv6 (IpBase_Sock added)
 *  01.02.19  2011-12-05  visalr  ESCAN00055319 Extend functionality for IPv6 (IpBase_Sock added)
 *  01.02.20  2011-12-12  visalr  ESCAN00055497 Compiler error missing length in array (IpBase_SockAddrType, array 
 *                                              length undefined)
 *  01.02.21  2012-02-22  visalr  ESCAN00056598 Compiler Abstractions are partly wrong
 *            2012-02-22          ESCAN00057031 Remove Macro support for Version Info API
 *  01.02.22  2012-04-20  visalr  ESCAN00058502 Compiler warning: ContentSize possibly uninitialized
 *  01.02.23  2012-04-27  visalr                Compiler warning: condition is always true 
 *                                              (IpBase_String.c, IpBase_ConvString2SignedIntBase)
 *            2012-09-11  visalr  ESCAN00060497 Runtime exception on TriCore platforms due to unaligned memory access
 *  01.02.24  2012-09-12  visalr  ESCAN00060497 Runtime exception on TriCore platforms due to unaligned memory access
 *  01.02.25  2012-10-01  visalr  ESCAN00061864 ASR4 Enhancement
 *            2012-10-01  visalr  ESCAN00058886 AR4-125: Remove support for v_cfg.h in Cfg5 systems
 *  01.02.26  2012-10-09  visalr  ESCAN00062047 Improved Random Number Generation
 *            2012-10-31  visalr  ESCAN00062662 Code contains C++ comments
 *            2012-11-19  visalr  ESCAN00063090 Rename IPBASE_SWAPxx to IPBASE_HTONxx
 *  01.02.27  2012-11-30  visalr                Include Std_Types.h in Appl_Rand.c
 *            2012-12-17  visalr  ESCAN00053450 AR3-1698: Add MISRA justifications in source code for automatic 
 *                                              generation of MISRA reports
 *            2012-12-17  visalr  ESCAN00062251 AR4-220: Remove STATIC
 *            2012-12-17  visalr  ESCAN00062662 Code contains C++ comments
 *  01.02.28  2013-01-07  visalr  ESCAN00053450 AR3-1698: Add MISRA justifications in source code for automatic 
 *                                              generation of MISRA reports
 *  01.02.29  2013-03-12  visalr  ESCAN00053450 AR3-1698: Add MISRA justifications in source code for automatic 
 *                                              generation of MISRA reports
 *  01.02.30  2013-03-13  visalr  ESCAN00053450 AR3-1698: Add MISRA justifications in source code for automatic 
 *                                              generation of MISRA reports
 *  01.02.31  2013-03-13  visalr  ESCAN00053450 AR3-1698: Add MISRA justifications in source code for automatic 
 *                                              generation of MISRA reports
 *  01.02.32  2013-03-18  visalr  ESCAN00065987 Integrate Code Review 
 *  01.02.33  2013-04-19  visalr  ESCAN00065987 Integrate Code Review 
 *  01.02.34  2013-12-20  visalr  ESCAN00066430 MISRA deviation: MISRA-C:2004 Rule 19.6 in MSN_MemMap.inc 
 *            2013-12-20  visalr  ESCAN00066406 Compiler warning: in Appl_Rand.c 
 *            2014-01-14  visalr  ESCAN00069637 Suggestion: Tcp/Udp checksum speedup
 *  01.02.35  2014-01-09  visalr  ESCAN00069637 Suggestion: Tcp/Udp checksum speedup 
 *            2014-01-28  visalr  ESCAN00073256 Compiler warning: "\" followed by white space in IpBase_Types.h
 *            2014-02-06  visalr  ESCAN00073508 Enhancement uint8, uint16, uint32 get and put macros
 *            2014-02-06  visalr  ESCAN00073509 Enhancement IpBase_AddrIn6Type with 32bit access
 *  01.02.36  2014-02-12  visalr  ESCAN00073630 Appl_Crypto_GetRandArray indirectly calls esl_getBytesRNG before 
 *                                              Crypto is initialized
 *  01.02.37  2014-09-10  visalr  ESCAN00076469 Implement IPBASE_HTOLE and IPBASE_LETOH macros
 *  01.02.38  2015-01-09  visalr  ESCAN00080128 Misssing IPBASE_START_SEC_CODE in IpBase_Sock.c
 *            2015-01-09  visalr  ESCAN00077701 Compiler warning: Warning when the component is compiled with Visual 
 *                                              Studio
 *  01.02.39  2015-02-02  visalr  ESCAN00080957 Wrong Checksum for Big Endian with overflow in last byte and odd length
 *  02.00.00  2015-02-20  visalr  ESCAN00081375 Adapt  struct IpBase_SockAddrIn6Type and define IPBASE_AF_INET6
 *            2015-02-27  visalr  ESCAN00081516 Improve copy routine IpBase_Copy by loop unroll
 *            2015-03-03  visalr  ESCAN00081583 Compiler warning: Wrong compiler abstraction in IpBase_PBuf.c 
 *                                              IpBase_StrCmpPBuf
 *  02.01.00  2015-05-06  visalr  ESCAN00082270 FEAT-1379: Optimized copy operation for large data
 *            2015-05-06  visalr  ESCAN00082868 Compiler warning: truncating assignment, IpBase_String.c IpBase_Code.c
 *            2015-05-06  visalr  ESCAN00082869 MISRA deviation: MISRA-C:2004 Cyclomatic complexity too high, 
 *                                              IpBase_Copy.c
 *  02.01.01  2015-09-25  visalr  ESCAN00085443 Passive Definition of E_PENDING
 *            2015-10-12  visalr  ESCAN00085799 MISRA deviation: MISRA-C:2004 The initializer is always modified before
 *                                              being used
 *            2015-11-27  visalr  ESCAN00086858 Minor Maintenance Changes
 *  02.01.02  2016-02-02  visalr  ESCAN00088013 Basic Encoding Rules parser returns invalid result
 *  02.01.03  2016-03-15  visalr  ESCAN00088927 IpBase_CopyString2PbufAt function does no more limit length
 *            2016-03-24  visalr  ESCAN00089055 Compiler error: Unknown include file Eth.h and function 
 *                                              Eth_GetPhysAddr() in _ApplRand.c
 *  02.01.04  2016-05-04  visalr  ESCAN00089895 Review integration
 *  02.01.05  2016-10-17  visalr  ESCAN00091168 Source file define does not match the file name
 *  02.01.06  2016-11-08  visalr  ESCAN00092695 MISRA deviation: MISRA-C:2004 Rule 19.7 Function instead of macro
 *  03.00.00  2016-05-06  visalr  ESCAN00094509 Process3.0 adaption
 *            2017-03-27  visalr  ESCAN00094504 IpBase_StrLen incorrect length check 
 *********************************************************************************************************************/
#if !defined (IPBASE_H)
# define IPBASE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* PRQA S 857 NO_OF_MACROS */ /* MD_IpBase_857 */
# include "IpBase_Types.h"
# include "IpBase_Code.h"
# include "IpBase_Copy.h"
# include "IpBase_String.h"
# include "IpBase_PBuf.h"
# include "IpBase_Sock.h"

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ##V_CFG_MANAGEMENT ##CQProject :   SYSSERVICE_IPBASE CQComponent : Implementation */
# define SYSSERVICE_IPBASE_VERSION          (0x0300u) /* BCD coded version number */
# define SYSSERVICE_IPBASE_RELEASE_VERSION  (0x00u) /* BCD coded release version number */

/* Supported AUTOSAR version */
#  define IPBASE_AR_RELEASE_MAJOR_VERSION           (4u)
#  define IPBASE_AR_RELEASE_MINOR_VERSION           (0u)
#  define IPBASE_AR_RELEASE_REVISION_VERSION        (3u)
/* for compatibility */
#  define IPBASE_AR_MAJOR_VERSION                   (4u)
#  define IPBASE_AR_MINOR_VERSION                   (0u)
#  define IPBASE_AR_PATCH_VERSION                   (3u)

# define IPBASE_SW_MAJOR_VERSION                    (3u)
# define IPBASE_SW_MINOR_VERSION                    (0u)
# define IPBASE_SW_PATCH_VERSION                    (0u)

/* limited check if component is enabled */

/* IpBase Vendor and Module ID */
# define IPBASE_VENDOR_ID                              (30u)
# define IPBASE_MODULE_ID                              (255u)
/* IpBase API IDs */
# define IPBASE_API_ID_GET_VERSION_INFO                (0x01u) /*!< Service ID: IpBase_GetVersionInfo() */
# define IPBASE_API_ID_ENCODE                          (0x11u) /*!< Service ID: IpBase_Encode() */
# define IPBASE_API_ID_DECODE                          (0x12u) /*!< Service ID: IpBase_Decode() */
# define IPBASE_API_ID_BER_INIT_WORKSPACE              (0x13u) /*!< Service ID: IpBase_BerInitWorkspace() */
# define IPBASE_API_ID_BER_GET_ELEMENT                 (0x14u) /*!< Service ID: IpBase_BerGetElement() */
# define IPBASE_API_ID_COPY                            (0x21u) /*!< Service ID: IpBase_Copy() */
# define IPBASE_API_ID_COPY_SMALL_DATA                 (0x22u) /*!< Service ID: IpBase_CopySmallData() */
# define IPBASE_API_ID_FILL                            (0x23u) /*!< Service ID: IpBase_Fill() */
# define IPBASE_API_ID_STR_COPY                        (0x31u) /*!< Service ID: IpBase_StrCpy() */
# define IPBASE_API_ID_STR_COPY_MAX_LEN                (0x32u) /*!< Service ID: IpBase_StrCpyMaxLen() */
# define IPBASE_API_ID_STR_CMP                         (0x33u) /*!< Service ID: IpBase_StrCmp() */
# define IPBASE_API_ID_STR_CMP_LEN                     (0x34u) /*!< Service ID: IpBase_StrCmpLen() */
# define IPBASE_API_ID_STR_CMP_NO_CASE                 (0x35u) /*!< Service ID: IpBase_StrCmpNoCase() */
# define IPBASE_API_ID_STR_FIND_SUB_STR                (0x36u) /*!< Service ID: IpBase_StrFindSubStr() */
# define IPBASE_API_ID_STR_LEN                         (0x37u) /*!< Service ID: IpBase_StrLen() */
# define IPBASE_API_ID_CONV_INT_TO_STR                 (0x38u) /*!< Service ID: IpBase_ConvInt2String() */
# define IPBASE_API_ID_CONV_INT_TO_HEX_STR             (0x39u) /*!< Service ID: IpBase_ConvInt2HexString() */
# define IPBASE_API_ID_CONV_INT_TO_STR_BASE            (0x3Au) /*!< Service ID: IpBase_ConvInt2StringBase() */
# define IPBASE_API_ID_CONV_ARRAY_TO_STR_BASE          (0x3Bu) /*!< Service ID: IpBase_ConvArray2HexStringBase() */
# define IPBASE_API_ID_CONV_INT_TO_STR_FRONT           (0x3Cu) /*!< Service ID: IpBase_ConvInt2StringFront() */
# define IPBASE_API_ID_CONV_STR_TO_INT                 (0x3Du) /*!< Service ID: IpBase_ConvString2Int() */
# define IPBASE_API_ID_CONV_STR_TO_INT_DYN             (0x3Eu) /*!< Service ID: IpBase_ConvString2IntDyn() */
# define IPBASE_API_ID_CONV_HEX_STR_TO_INT             (0x3Fu) /*!< Service ID: IpBase_ConvHexString2Int() */
# define IPBASE_API_ID_CONV_HEX_STR_TO_INT_DYN         (0x40u) /*!< Service ID: IpBase_ConvHexString2IntDyn() */
# define IPBASE_API_ID_CONV_STR_TO_INT_BASE            (0x41u) /*!< Service ID: IpBase_ConvString2IntBase() */
# define IPBASE_API_ID_CONV_STR_TO_SIGNED_INT_BASE     (0x42u) /*!< Service ID: IpBase_ConvString2SignedIntBase() */
# define IPBASE_API_ID_CONV_HEX_STR_TO_ARRAY_BASE      (0x43u) /*!< Service ID: IpBase_ConvHexString2ArrayBase() */
# define IPBASE_API_ID_STR_CMP_PBUF                    (0x51u) /*!< Service ID: IpBase_StrCmpPBuf() */
# define IPBASE_API_ID_INC_PBUF                        (0x52u) /*!< Service ID: IpBase_IncPBuf() */
# define IPBASE_API_ID_COPY_STR_TO_PBUF_AT             (0x53u) /*!< Service ID: IpBase_CopyString2PBufAt() */
# define IPBASE_API_ID_COPY_PBUF_TO_STR                (0x54u) /*!< Service ID: IpBase_CopyPBuf2Str() */
# define IPBASE_API_ID_FIND_STR_IN_PBUF                (0x55u) /*!< Service ID: IpBase_FindStringInPBuf() */
# define IPBASE_API_ID_CHK_STR_IN_PBUF                 (0x56u) /*!< Service ID: IpBase_CheckStringInPBuf() */
# define IPBASE_API_ID_READ_BYTE_IN_PBUF               (0x57u) /*!< Service ID: IpBase_ReadByteInPBuf() */
# define IPBASE_API_ID_DEL_SOCK_ADDR                   (0x60u) /*!< Service ID: IpBase_DelSockAddr() */
# define IPBASE_API_ID_COPY_SOCK_ADDR                  (0x61u) /*!< Service ID: IpBase_CopySockAddr() */
# define IPBASE_API_ID_COPY_IPV6_ADDR                  (0x62u) /*!< Service ID: IpBase_CopyIpV6Addr() */
# define IPBASE_API_ID_SOCK_IP_ADDR_IS_EQUAL           (0x63u) /*!< Service ID: IpBase_SockIpAddrIsEqual() */
# define IPBASE_API_ID_SOCK_PORT_IS_EQUAL              (0x64u) /*!< Service ID: IpBase_SockPortIsEqual() */
# define IPBASE_API_ID_CALC_TCPIP_CHCECKSUM            (0x65u) /*!< Service ID: IpBase_CalcTcpIpChecksum() */
# define IPBASE_API_ID_CALC_TCPIP_CHCECKSUM2           (0x66u) /*!< Service ID: IpBase_CalcTcpIpChecksum2() */
# define IPBASE_API_ID_CALC_TCPIP_CHCECKSUMADD         (0x67u) /*!< Service ID: IpBase_TcpIpCheckumAdd() */

/* IPBASE DET errors */
# define IPBASE_E_NO_ERROR                             (0x00u) /*!< Error code: no error */
# define IPBASE_E_INV_POINTER                          (0x01u) /*!< Error code: API service called with invalid pointer parameter (NULL) */
# define IPBASE_E_INV_PARAM                            (0x02u) /*!< Error code: API service called with invalid parameter */

/* IPBASE additional return types */
# define IPBASE_E_OK                                   (0x00u) /*!< Return code: O.K. */
# define IPBASE_E_NOT_OK                               (0x81u) /*!< Return code: Not O.K. */
# define IPBASE_E_PENDING                              (0x82u) /*!< Return code: Pending */
# define IPBASE_E_MEM                                  (0x83u) /*!< Return code: Out of memory */
# define IPBASE_E_BER_PARAM                            (0x84u) /*!< Return code: Basic Encoding Rules Param */

/* IPBASE compare results */
# define IPBASE_CMP_EQUAL                              (0x00u) /*!< Return code: Comparison result equal */
# define IPBASE_CMP_NOT_EQUAL                          (0x01u) /*!< Return code: Comparison result not equal */

/* IPBASE length limits */
# define IPBASE_UINT32_STR_LEN_MAX                     (0x0Au) /*!< Limit: Max string length for 32 bit number */
# define IPBASE_UINT16_STR_LEN_MAX                     (0x05u) /*!< Limit: Max string length for 16 bit number */
# define IPBASE_UINT32_HEX_STR_LEN_MAX                 (0x08u) /*!< Limit: Max string length for 32 bit hex number */
# define IPBASE_UINT16_HEX_STR_LEN_MAX                 (0x04u) /*!< Limit: Max string length for 16 bit hex number */
# define IPBASE_INT_DIGIT_MAX                          (0x0Au) /*!< Limit: Max number of digits in int */
# define IPBASE_HEX_INT_DIGIT_MAX                      (0x08u) /*!< Limit: Max number of digits in hex int */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* IPBASE byte swap */
/* PRQA S 3453 3 */ /* MD_MSR_19.7 */
# define IPBASE_BYTE_SWAP16(Data)  ((((uint16)(Data) >>  8)&0x00FFU) | (((uint16)(Data) <<  8)&0xFF00U))
# define IPBASE_BYTE_SWAP32(Data)  ((((uint32)(Data) >> 24)&0x000000FFU) | (((uint32)(Data) >>  8)&0x0000FF00U) | \
                                   ( ((uint32)(Data) <<  8)&0x00FF0000U) | (((uint32)(Data) << 24)&0xFF000000U))
#if ( CPU_BYTE_ORDER == HIGH_BYTE_FIRST )
  /* IPBASE_SWAP16 and IPBASE_SWAP32 are deprecated, please use IPBASE_HTON16 and IPBASE_HTON32 */
#  define IPBASE_SWAP16(Data) (Data)                /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define IPBASE_SWAP32(Data) (Data)                /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define IPBASE_HTON16(Data) (Data)                /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define IPBASE_HTON32(Data) (Data)                /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define IPBASE_NTOH16(Data) (Data)                /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define IPBASE_NTOH32(Data) (Data)                /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define IPBASE_LE2HE16 IPBASE_BYTE_SWAP16(Data)   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define IPBASE_LE2HE32 IPBASE_BYTE_SWAP32(Data)   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
  /* IPBASE_SWAP16 and IPBASE_SWAP32 are deprecated, please use IPBASE_HTON16 and IPBASE_HTON32 */
#  define IPBASE_SWAP16(Data)  IPBASE_BYTE_SWAP16(Data)   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define IPBASE_SWAP32(Data)  IPBASE_BYTE_SWAP32(Data)   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define IPBASE_HTON16(Data)  IPBASE_BYTE_SWAP16(Data)   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define IPBASE_HTON32(Data)  IPBASE_BYTE_SWAP32(Data)   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define IPBASE_NTOH16(Data)  IPBASE_BYTE_SWAP16(Data)   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define IPBASE_NTOH32(Data)  IPBASE_BYTE_SWAP32(Data)   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define IPBASE_LE2HE16(Data) (Data)                     /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define IPBASE_LE2HE32(Data) (Data)                     /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif
  /* CPU_BYTE_ORDER */

/* IPBASE bytewise access */
/* PRQA S 3453 9 */ /* MD_MSR_19.7 */
# define IPBASE_PUT_UINT8(Buffer, Offset, Value)        ( (Buffer)[(Offset)] = (uint8)(Value))

# define IPBASE_PUT_UINT16(Buffer, Offset, Value) do { (Buffer)[(Offset)+0] = (uint8)((uint16_least)(Value) >> 8); \
                                                       (Buffer)[(Offset)+1] = (uint8)((uint16_least)(Value)); } while(0)

# define IPBASE_PUT_UINT32(Buffer, Offset, Value) do { (Buffer)[(Offset)+0] = (uint8)((uint32_least)(Value) >> 24); \
                                                       (Buffer)[(Offset)+1] = (uint8)((uint32_least)(Value) >> 16); \
                                                       (Buffer)[(Offset)+2] = (uint8)((uint32_least)(Value) >>  8); \
                                                       (Buffer)[(Offset)+3] = (uint8)((uint32_least)(Value)); } while(0)

/* PRQA S 3453 9 */ /* MD_MSR_19.7 */
# define IPBASE_GET_UINT8(Buffer, Offset, Variable)  ( (Variable) = (uint8)(Buffer)[(Offset)])

# define IPBASE_GET_UINT16(Buffer, Offset, Variable) ( (Variable) = (uint16)(((uint16)((Buffer)[(Offset)+0]) << 8) | \
                                                                            ((uint16)((Buffer)[(Offset)+1]))) )

# define IPBASE_GET_UINT32(Buffer, Offset, Variable) ( (Variable) = (uint32)(((uint32)((Buffer)[(Offset)+0]) << 24) | \
                                                                             ((uint32)((Buffer)[(Offset)+1]) << 16) | \
                                                                             ((uint32)((Buffer)[(Offset)+2]) <<  8) | \
                                                                             ((uint32)((Buffer)[(Offset)+3]))) )

/* PRQA L:NO_OF_MACROS */
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define IPBASE_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

# if ( IPBASE_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  IpBase_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     Returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  VersionInfoPtr        pointer for version info
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-108035
 *********************************************************************************************************************/
extern IPBASE_FUNCCODE(void) IpBase_GetVersionInfo(IPBASE_P2VARAPPLDATA(Std_VersionInfoType) VersionInfoPtr);
# endif
  /* IPBASE_VERSION_INFO_API */

# define IPBASE_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif 
  /* IPBASE_H */
/**********************************************************************************************************************
 *  END OF FILE: IpBase.h
 *********************************************************************************************************************/
