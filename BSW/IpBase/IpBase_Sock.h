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
 *         File:  IpBase_Sock.h
 *      Project:  MICROSAR ETH
 *    Component:  SysService_IpBase
 *       Module:  MICROSAR IP Base
 *    Generator:  None
 *
 *  Description:  Socket header
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
#if !defined (IPBASE_SOCK_H)
# define IPBASE_SOCK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "IpBase_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# if ( IPBASE_SOCK_ENABLE == STD_ON )
#  define IPBASE_START_SEC_CODE
#  include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */


/***********************************************************************************************************************
 *  IpBase_DelSockAddr
 **********************************************************************************************************************/
/*! \brief         Delete socket address
 *  \details       Delete socket address i.e. reset values (incl. family, port, ip-addr)
 *  \param[in]     SockPtr              socket address
 *  \param[in]     Family               supported family
 *  \return        E_OK                 SockAddr could be deleted
 *  \return        E_NOT_OK             SockAddr could not be deleted
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120246
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_DelSockAddr(IPBASE_P2VARAPPLDATA(IpBase_SockAddrType) SockPtr,
                                                          uint16 Family);

/***********************************************************************************************************************
 *  IpBase_CopySockAddr
 **********************************************************************************************************************/
/*! \brief         Copy socket address
 *  \details       Copy socket address (incl. family, port, ip-addr) from Src to Tgt
 *  \param[in,out] TgtSockPtr           target socket address
 *  \param[in]     SrcSockPtr           source socket address
 *  \return        E_OK                 SockAddr could be copied
 *  \return        E_NOT_OK             SockAddr could not be copied
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120246
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_CopySockAddr(IPBASE_P2VARAPPLDATA(IpBase_SockAddrType) TgtSockPtr,
                                                           IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SrcSockPtr);

/***********************************************************************************************************************
 *  IpBase_CopyIpV6Addr
 **********************************************************************************************************************/
/*! \brief         Copy IPv6 socket address
 *  \details       Copy IPv6 socket address (incl. family, port, ip-addr) from Src to Tgt
 *  \param[in,out] TgtIpAddrPtr           target ip address
 *  \param[in]     SrcIpAddrPtr           source ip address
 *  \return        E_OK                 IP address could be copied
 *  \return        E_NOT_OK             IP address could not be copied
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120246
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(Std_ReturnType) IpBase_CopyIpV6Addr(IPBASE_P2VARAPPLDATA(IpBase_AddrIn6Type) TgtIpAddrPtr,
                                                           IPBASE_P2CONSTAPPLDATA(IpBase_AddrIn6Type) SrcIpAddrPtr);

/***********************************************************************************************************************
 *  IpBase_SockIpAddrIsEqual
 **********************************************************************************************************************/
/*! \brief         Check if IP address is equal
 *  \details       Check if IP address of sockets is equal
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
extern IPBASE_FUNCCODE(boolean) IpBase_SockIpAddrIsEqual(IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockAPtr,
                                                         IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockBPtr);

/***********************************************************************************************************************
 *  IpBase_SockPortIsEqual
 **********************************************************************************************************************/
/*! \brief         Check if port is equal
 *  \details       Check if port of sockets is equal
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
extern IPBASE_FUNCCODE(boolean) IpBase_SockPortIsEqual(IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockAPtr,
                                                       IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockBPtr);

/***********************************************************************************************************************
 *  IpBase_CalcTcpIpChecksum
 **********************************************************************************************************************/
/*! \brief         Calculate TCP/IP checksum
 *  \details       This API calculates the checksum over a given data range. The checksum is TcpIp specific. I.e. it 
 *                 expects 16bit data chunks and uses one's complement checksum algorithm.
 *  \param[in]     DataPtr           pointer to the data
 *  \param[in]     LenByte           data length in bytes
 *  \return        Calculated checksum
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120246
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(uint16) IpBase_CalcTcpIpChecksum(IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 LenByte);

/***********************************************************************************************************************
 *  IpBase_CalcTcpIpChecksum2
 **********************************************************************************************************************/
/*! \brief         Calculate TCP/IP checksum 
 *  \details       This API calculates the checksum over a given data range. The checksum is TcpIp specific. I.e. it 
 *                 expects 16bit data chunks and uses one's complement checksum algorithm.
 *  \param[in]     DataPtr           pointer to the data
 *  \param[in]     LenByte           data length in bytes
 *  \param[in]     PseudoHdrPtr      pointer to optional pseudo header (NULL_PTR if omitted)
 *  \param[in]     PseudoHdrLenByte  pseudo header length in bytes
 *  \return        Calculated checksum
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120246
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(uint16) IpBase_CalcTcpIpChecksum2(IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 LenByte,
                                                         IPBASE_P2CONSTAPPLDATA(uint8) PseudoHdrPtr,
                                                         uint32 PseudoHdrLenByte);
/***********************************************************************************************************************
 *  IpBase_TcpIpChecksumAdd
 **********************************************************************************************************************/
/*! \brief         Adds to TCP/IP checkusm.
 *  \details       This API adds a range to TcpIp checkusm calculation. 
 *  \param[in]     DataPtr           pointer to the data
 *  \param[in]     LenByte           data length in bytes
 *  \param[in]     Checksum          current checksum. 0 to start a new checksum calculation
 *  \param[in]     Stop              builds the one's complement to finalize the checksum
 *  \return        calculated checksum in network byte order
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120246
 **********************************************************************************************************************/
extern IPBASE_FUNCCODE(uint32) IpBase_TcpIpChecksumAdd(IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 LenByte, 
                                                       uint32 Checksum, boolean Stop);
#  define IPBASE_STOP_SEC_CODE
#  include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/* PRQA L:ASR_MEMMAP */
# endif
  /* IPBASE_SOCK_ENABLE */
#endif
  /* IPBASE_SOCK_H */
/**********************************************************************************************************************
 *  END OF FILE: IpBase_Sock.h
 *********************************************************************************************************************/
