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
 *         File:  IpBase_Sock.c
 *      Project:  MICROSAR ETH
 *    Component:  SysService_IpBase
 *       Module:  MICROSAR IP Base
 *    Generator:  None
 *
 *  Description:  Socket API implementation
 *  
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/
#define IPBASE_SOCK_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* PRQA S 857 NO_OF_MACROS */ /* MD_IpBase_857 */
#include "IpBase.h"
#include "IpBase_Priv.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check consistency of source and header file. */
#if ( (IPBASE_SW_MAJOR_VERSION != 3u) || (IPBASE_SW_MINOR_VERSION != 0u) || (IPBASE_SW_PATCH_VERSION != 0u) )
# error "IpBase_Sock.c: Source and Header file are inconsistent!"
#endif

/* PRQA L:NO_OF_MACROS */
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define IPBASE_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#if ( IPBASE_SOCK_ENABLE == STD_ON )
/***********************************************************************************************************************
 *  IpBase_SockIpAddrIsEqualHlp
 **********************************************************************************************************************/
/*! \brief         Check if IP address is equal helper
 *  \details       Check if IP address of sockets is equal actual comparison
 *  \param[in,out] SockAPtr             target socket address
 *  \param[in]     SockBPtr             source socket address
 *  \return        TRUE                 IP address is equal
 *  \return        FALSE                IP address is not equal
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120246
 **********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(boolean) IpBase_SockIpAddrIsEqualHlp(IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockAPtr,
                                                                  IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockBPtr);
/***********************************************************************************************************************
 *  IpBase_TcpIpChecksumAddHlp
 **********************************************************************************************************************/
/*! \brief         Adds to TCP/IP checkusm helper
 *  \details       Adds a range to TcpIp checkusm calculation 
 *  \param[in]     DataPtr           pointer to the data
 *  \param[in]     LenByte           data length in bytes
 *  \param[in]     Checksum          current checksum. 0 to start a new checksum calculation
 *  \return        calculated checksum in network byte order
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120246
 **********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(uint32) IpBase_TcpIpChecksumAddHlp(IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 LenByte, 
                                                                uint32 Checksum);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/***********************************************************************************************************************
 *  IpBase_DelSockAddr
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_DelSockAddr(IPBASE_P2VARAPPLDATA(IpBase_SockAddrType) SockPtr, uint16 Family)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           lenByte = 0;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check SockPtr for NULL pointer */
  if ( SockPtr == NULL_PTR )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check address family */
    if(Family == IPBASE_AF_INET)
    {
      lenByte = sizeof(IpBase_SockAddrInType);
    }
    else if(Family == IPBASE_AF_INET6)
    {
      lenByte = sizeof(IpBase_SockAddrIn6Type);
    }
    else
    {
      retVal = E_NOT_OK;
    }

    /* #30 Reset bytes of socket address */
    if(retVal == E_OK)
    {
      uint16  i;
      for(i=0; i<lenByte; i++)
      {
        ((IPBASE_P2VARAPPLDATA(uint8))SockPtr)[i] = 0;   /* PRQA S 310,3305 */ /* MD_IpBase_310 */
      }
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #40 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_DEL_SOCK_ADDR, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_DelSockAddr() */
/***********************************************************************************************************************
 *  IpBase_CopySockAddr
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_CopySockAddr(IPBASE_P2VARAPPLDATA(IpBase_SockAddrType) TgtSockPtr,
                                                    IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SrcSockPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint8           lenByte = 0;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check SockPtr for NULL pointer */
  if ( (TgtSockPtr == NULL_PTR) || (SrcSockPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check address family */
    if(SrcSockPtr->sa_family == IPBASE_AF_INET)
    {
      lenByte = sizeof(IpBase_SockAddrInType);
    }
    else if(SrcSockPtr->sa_family == IPBASE_AF_INET6)
    {
      lenByte = sizeof(IpBase_SockAddrIn6Type);
    }
    else
    {
      retVal = E_NOT_OK;
    }

    /* #30 Copy bytes of socket address */
    if(retVal == E_OK)
    { /* PRQA S 310,3305 2 */ /* MD_IpBase_310  */
      IpBase_Copy((IPBASE_P2VARAPPLDATA(IpBase_CopyDataType))TgtSockPtr, 
                  (IPBASE_P2CONSTAPPLDATA(IpBase_CopyDataType))SrcSockPtr, lenByte);
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #40 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_COPY_SOCK_ADDR, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_CopySockAddr() */
/***********************************************************************************************************************
 *  IpBase_CopyIpV6Addr
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_CopyIpV6Addr(IPBASE_P2VARAPPLDATA(IpBase_AddrIn6Type) TgtIpAddrPtr,
                                                    IPBASE_P2CONSTAPPLDATA(IpBase_AddrIn6Type) SrcIpAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_OK;
  uint8           errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check IpAddrPtr for NULL pointer */
  if ( (TgtIpAddrPtr == NULL_PTR) || (SrcIpAddrPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Copy IPv6 address */
    /* PRQA S 310,3305 2 */ /* MD_IpBase_310 */
    IpBase_Copy((IPBASE_P2VARAPPLDATA(IpBase_CopyDataType))TgtIpAddrPtr, 
                (IPBASE_P2CONSTAPPLDATA(IpBase_CopyDataType))SrcIpAddrPtr, sizeof(IpBase_AddrIn6Type));
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_COPY_IPV6_ADDR, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_CopyIpV6Addr() */
/***********************************************************************************************************************
 *  IpBase_SockIpAddrIsEqual
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(boolean) IpBase_SockIpAddrIsEqual(IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockAPtr,
                                                  IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockBPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean         retVal;
  uint8           errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check SockPtr for NULL pointer */
  if ( (SockAPtr == NULL_PTR) || (SockBPtr == NULL_PTR) )
  {
    retVal  = TRUE;
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check for equal address family */
    if(SockAPtr->sa_family != SockBPtr->sa_family)
    {
      retVal = FALSE;
    }
    else
    { /* #30 Check for equal address helper */
      retVal = IpBase_SockIpAddrIsEqualHlp(SockAPtr, SockBPtr);
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #40 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_SOCK_IP_ADDR_IS_EQUAL, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_SockIpAddrIsEqual() */

/***********************************************************************************************************************
 *  IpBase_SockPortIsEqual
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(boolean) IpBase_SockPortIsEqual(IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockAPtr,
                                                IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockBPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean         retVal = TRUE;
  uint8           errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check SockPtr for NULL pointer */
  if ( (SockAPtr == NULL_PTR) || (SockBPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check for equal port */ /* PRQA S 310,3305 2 */ /* MD_IpBase_310 */
    if( ((IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrInType))SockAPtr)->sin_port !=  
        ((IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrInType))SockBPtr)->sin_port )
    {
      retVal = FALSE;
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_SOCK_PORT_IS_EQUAL, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_SockPortIsEqual() */
/***********************************************************************************************************************
 *  IpBase_CalcTcpIpChecksum
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(uint16) IpBase_CalcTcpIpChecksum(IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16          retVal;
  uint8           errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check DataPtr for NULL pointer */
  if ( DataPtr == NULL_PTR )
  {
    retVal = 0;
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Calculate checksum */
    retVal = IpBase_CalcTcpIpChecksum2(DataPtr, LenByte, (IPBASE_P2VARAPPLDATA(uint8))NULL_PTR, 0);
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_CALC_TCPIP_CHCECKSUM, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_CalcTcpIpChecksum() */
/***********************************************************************************************************************
 *  IpBase_CalcTcpIpChecksum2
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(uint16) IpBase_CalcTcpIpChecksum2(IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 LenByte,
                                                  IPBASE_P2CONSTAPPLDATA(uint8) PseudoHdrPtr,
                                                  uint32 PseudoHdrLenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16          retVal;
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint32          idx      = LenByte>>1;  /* 16 bit length */
  uint32          checksum = 0U; 

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check DataPtr, PseudoHdrPtr for NULL pointer */
  if ( (DataPtr == NULL_PTR) || ((PseudoHdrPtr == NULL_PTR) && (PseudoHdrLenByte != 0U))  )
  {
    retVal  = 0;
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Add Checksum */
    while ( idx != 0U )
    {
      idx--;
      checksum += (uint16)(((uint32)(DataPtr[(idx<<1)+0]) << 8)+DataPtr[(idx<<1)+1]);
    }
    /* #30 Add odd byte */
    if ( (LenByte & 0x00000001) != 0U )
    {
      checksum += (uint16)(((uint32)(DataPtr[LenByte-1]) << 8));
    }
    /* #40 Add pseudo header checksum */
    if ( PseudoHdrPtr != NULL_PTR )
    {
      idx   = PseudoHdrLenByte>>1;
      while ( idx != 0U )
      {
        idx--;
        checksum += (uint16)(((uint32)(PseudoHdrPtr[(idx<<1)+0]) << 8)+PseudoHdrPtr[(idx<<1)+1]);
      }
    }
    /* #50 Consider overflow */
    while ( (checksum>>16) != 0U )
    {
      checksum = (uint32)((checksum & 0xFFFF)+(checksum>>16));
    }
    /* #60 Build one's complement */
    retVal = (uint16) ~checksum;
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #70 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_CALC_TCPIP_CHCECKSUM2, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* IpBase_CalcTcpIpChecksum2() */
/***********************************************************************************************************************
 *  IpBase_TcpIpChecksumAdd
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(uint32) IpBase_TcpIpChecksumAdd(IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 LenByte, 
                                                uint32 Checksum, boolean Stop)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           errorId = IPBASE_E_NO_ERROR;
  uint16          lower16bit;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check DataPtr for NULL pointer */
  if ( DataPtr == NULL_PTR )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Calculate additive checksum in helper function */
    Checksum = IpBase_TcpIpChecksumAddHlp(DataPtr, LenByte, Checksum);

    if ( Stop == TRUE )
    { /* #30 Consider overflow after last data chunk */
      lower16bit = (uint16)(Checksum);
      Checksum  = (uint16)(lower16bit + (uint16)(Checksum>>16));
      if ( Checksum < lower16bit )
      { /* overflow */
        Checksum++;
      }
      /* #40 Build one's complement after last data chunk */
      Checksum = (uint16) ~Checksum;
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #50 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_CALC_TCPIP_CHCECKSUMADD, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return Checksum;
} /* IpBase_TcpIpChecksumAdd() */
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  IpBase_SockIpAddrIsEqualHlp
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(boolean) IpBase_SockIpAddrIsEqualHlp(IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockAPtr,
                                                                  IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockBPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean         retVal  = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  if(SockAPtr->sa_family == IPBASE_AF_INET)
  { /* #10 Check for equal IPv4 address */ /* PRQA S 310,3305 2 */ /* MD_IpBase_310 */
    if(((IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrInType))SockAPtr)->sin_addr != 
        ((IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrInType))SockBPtr)->sin_addr)
    {
      retVal = FALSE;
    }
  }
  else if(SockAPtr->sa_family == IPBASE_AF_INET6)
  { /* #20 Check for equal IPv6 address */ 
    uint16 i = sizeof(IpBase_AddrIn6Type);
    while(i != 0)
    {
      i--;
      /* PRQA S 310,3305 2 */ /* MD_IpBase_310 */
      if( ((IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrIn6Type))SockAPtr)->sin6_addr.addr[i] != 
          ((IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrIn6Type))SockBPtr)->sin6_addr.addr[i] )
      {
        retVal = FALSE;
        break;
      }
    }
  }
  else
  { /* #30 Unknown address family */ 
    retVal = FALSE;
  }

  return retVal;
} /* IpBase_SockIpAddrIsEqualHlp() */
/***********************************************************************************************************************
 *  IpBase_TcpIpChecksumAddHlp
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(uint32) IpBase_TcpIpChecksumAddHlp(IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 LenByte, 
                                                                uint32 Checksum)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32          dataIdx  = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Add Checksum */
  while ( LenByte > 3 )
  { /* #20 Add 32 bit values */
    LenByte -= 4;
    Checksum += *((const uint32 *)(&DataPtr[dataIdx])); /* PRQA S 310,3305 */ /* MD_IpBase_310 */
    if ( Checksum < *((const uint32 *)(&DataPtr[dataIdx])) ) /* PRQA S 310,3305 */ /* MD_IpBase_310 */
    { /* overflow */
      Checksum++;
    }
    dataIdx += 4;
  }

  if ( LenByte > 1 )
  { /* #30 Add two '32-odd' bytes */
    LenByte -= 2;
    Checksum += *((const uint16 *)(&DataPtr[dataIdx])); /* PRQA S 310,3305 */ /* MD_IpBase_310 */
    if ( Checksum < *((const uint16 *)(&DataPtr[dataIdx])) ) /* PRQA S 310,3305 */ /* MD_IpBase_310 */
    { /* overflow */
      Checksum++;
    }
    dataIdx += 2;
  }

  if ( LenByte > 0 )
  { /* #40 Add one '32-odd' bytes */
# if ( CPU_BYTE_ORDER == HIGH_BYTE_FIRST )
    /* big endian (network byte order) */
    Checksum += ((uint16)(DataPtr[dataIdx])) << 8;
    if ( Checksum < (((uint16)(DataPtr[dataIdx])) << 8) )
# else
    /* litte endian */
    Checksum += DataPtr[dataIdx];
    if ( Checksum < DataPtr[dataIdx] )
# endif
    { /* overflow */
      Checksum++;
    }
  }

  return Checksum;
} /* IpBase_TcpIpChecksumAddHlp() */
# define IPBASE_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif
  /* IPBASE_SOCK_ENABLE */

/* module specific MISRA deviations:
 MD_IpBase_5.6_781:
      Reason:     Non-unique identifier (additionally used within structs)
      Risk:       Uncritical if only within structs
      Prevention: Covered by code review
 MD_IpBase_6.3_5013:
      Reason:     Basic data types used for text
      Risk:       Size may differ on depending on platform
      Prevention: Covered by integration tests
 MD_IpBase_11.4:
      Reason:     Caused by type conversions due to message buffer interpretation
      Risk:       Possible programming error may be undetected by compiler
      Prevention: Covered by code review
 MD_IpBase_13.7:
      Reason:     Boolean check with fix result in DET checks due to code configuration
      Risk:       DET checks may fail
      Prevention: AUTOSAR DET checks shall be switched off in production code
 MD_IpBase_17.4:
      Reason:     Usage of pointer within array
      Risk:       Memory Access violation
      Prevention: Covered by code review
 MD_IpBase_306:
      Reason:     type cast for copy routine
      Risk:       Memory Access violation
      Prevention: Covered by integration tests
 MD_IpBase_310:
      Reason:     type cast for copy routine
      Risk:       Memory Access violation
      Prevention: Covered by integration tests
 MD_IpBase_750:
      Reason:     union used for IPv6/IPv4 single API
      Risk:       Memory Access violation
      Prevention: Covered by component tests
 MD_IpBase_776:
      Reason:     Prefixing required by AUTOSAR
      Risk:       Compiler error
      Prevention: Covered by component tests
 MD_IpBase_850:
      Reason:     Performance improvement by using macros
      Risk:       Compiler error
      Prevention: Covered by component tests
 MD_IpBase_857:
      Reason:     Number of macros too high
      Risk:       Limited test depth
      Prevention: Covered by component tests
 MD_IpBase_3673:
      Reason:     Parameter no const
      Risk:       Unwanted change of variable
      Prevention: Covered by component tests
*/
/**********************************************************************************************************************
 *  END OF FILE: IpBase_Sock.c
 *********************************************************************************************************************/

