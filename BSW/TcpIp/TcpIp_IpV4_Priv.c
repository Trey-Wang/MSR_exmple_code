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
 *         \file  TcpIp_IpV4_Priv.c
 *        \brief  Implementation of Internet Protocol version 4 (IPv4) - Internal Functions
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

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

#define TCPIP_IPV4_PRIV_SOURCE

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV4 == STD_ON)
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "TcpIp_IpV4_Cfg.h"
#include "TcpIp_IpV4_Priv.h"
#include "TcpIp_IpV4.h"
#include "TcpIp_Arp.h"

#if (IPV4_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

#include "TcpIp.h"

/**********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
/* ----- */
#define IPV4_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

#define IPV4_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/* ----- */
#define IPV4_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

#define IPV4_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
#define IPV4_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/***********************************************************************************************************************
 *  IpV4_VAssignPhysAddr
 **********************************************************************************************************************/
/*! \brief      This API assigns a physical address to a pointer
 * \details      -
 *  \param[in]  PhysAddrDstPtr        destination pointer for the physical address
 *  \param[in]  PhysAddrSrcPtr        source pointer to the physical address
 *  \context    interrupt or task level
 **********************************************************************************************************************/
FUNC(void, IPV4_CODE) IpV4_VAssignPhysAddr(
    P2VAR(uint8, AUTOMATIC, IPV4_APPL_DATA) PhysAddrDstPtr,
    P2CONST(uint8, IPV4_APPL_DATA, IPV4_CONST) PhysAddrSrcPtr )
{
  uint8 Idx = IPBASE_ETH_PHYS_ADDR_LEN_BYTE;

  /* ------------------------------------------------- */
  /* DET: */
  IpV4_CheckDetErrorReturnVoid( IPV4_COND_VALID_PTR(PhysAddrDstPtr), IPV4_API_ID_V_ASSIGN_PHYS_ADDR, IPV4_E_INV_POINTER )
  IpV4_CheckDetErrorReturnVoid( IPV4_COND_VALID_PTR(PhysAddrSrcPtr), IPV4_API_ID_V_ASSIGN_PHYS_ADDR, IPV4_E_INV_POINTER )
  /* ------------------------------------------------- */

  while ( 0 < Idx )
  {
    Idx--;
    PhysAddrDstPtr[Idx] = PhysAddrSrcPtr[Idx];
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

#define IPV4_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */

#endif  /* (TCPIP_SUPPORT_IPV4 == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: IpV4_Priv.c
 *********************************************************************************************************************/
