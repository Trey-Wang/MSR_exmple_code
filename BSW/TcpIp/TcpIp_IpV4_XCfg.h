/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  TcpIp_IpV4_XCfg.h
 *        \brief  Header for static inline configuration access functions for sub-module IpV4 of the TcpIp.
 *
 *      \details  Declares and defines the functions of the sub-module XCfg, called by other modules of the TcpIp_IpV4.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Thorsten Albers               visal         Vector Informatik GmbH
 *  Frederik Dornemann            visfdn        Vector Informatik GmbH
 *  Knut Winkelbach               viswnk        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv4 submodule. >> TcpIp_IpV4.h
 *********************************************************************************************************************/

#if !defined (IPV4_XCFG_H)
# define IPV4_XCFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "TcpIp_IpV4.h"
# if (TCPIP_SUPPORT_IPV4 == STD_ON)
#  if (IPV4_ARP_SUPPORT_DISCARD_HANDLING == STD_ON)
#   include "TcpIp_LCfg.h"
#  endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

 
/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#  define IPV4_START_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  IpV4_Arp_GetCtrlIdxFromAddrId()
 *********************************************************************************************************************/
/*! \brief       Returns the statically configured IpCtrlIdx for the given IpAddrId 
 *  \details     Local inline wrapper function for macro IPV4_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,  IpAddrId);
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(uint8, IPV4_CODE) IpV4_Arp_GetCtrlIdxFromAddrId(IpV4_AddrIdType IpAddrId);

/**********************************************************************************************************************
 *  IpV4_Arp_CfgIsDiscardedEntryHandlingEnabled()
 *********************************************************************************************************************/
/*! \brief       Returns whether ARP table shall have a state "full" and new entries shall be discarded then.
 *  \details     In case the new entry is discarded an optional callout is called.
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(boolean, IPV4_CODE) IpV4_Arp_CfgIsDiscardedEntryHandlingEnabled(void);

/**********************************************************************************************************************
 *  IpV4_Arp_CfgDiscardedEntryHandlingCbkCount()
 *********************************************************************************************************************/
/*! \brief       Returns the number of callbacks for discarded ARP entry handling.
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(uint8, IPV4_CODE) IpV4_Arp_CfgDiscardedEntryHandlingCbkCount(void);

/**********************************************************************************************************************
 *  IpV4_Arp_CfgDiscardedEntryHandlingCbk()
 *********************************************************************************************************************/
/*! \brief       Returns the pointer to the callbacks for discarded ARP entry handling according to given index.
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
IPV4_LOCAL_INLINE FUNC(TcpIp_PhysAddrTableChgDiscardedCbkType, IPV4_CODE) IpV4_Arp_CfgDiscardedEntryHandlingCbk(uint8 CalloutIdx);

#  define IPV4_STOP_SEC_CODE 
#  include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#  ifndef IPV4_UNIT_TEST /* COV_IPV4_TESTSUITE_INSTRUMENTATION */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#   define IPV4_START_SEC_CODE 
#   include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  IpV4_Arp_GetCtrlIdxFromAddrId()
 *********************************************************************************************************************/
/* PRQA S 3219 1 */ /* MD_MSR_14.1 */
IPV4_LOCAL_INLINE FUNC(uint8, IPV4_CODE) IpV4_Arp_GetCtrlIdxFromAddrId(IpV4_AddrIdType IpAddrId)
{
  uint8 IpCtrlIdx;
  IPV4_GET_CTRL_IDX_FROM_ADDR_ID(IpCtrlIdx,  IpAddrId);
  return IpCtrlIdx;
}

/**********************************************************************************************************************
 *  IpV4_Arp_CfgIsDiscardedEntryHandlingEnabled()
 *********************************************************************************************************************/
/* PRQA S 3219 1 */ /* MD_MSR_14.1 */
IPV4_LOCAL_INLINE FUNC(boolean, IPV4_CODE) IpV4_Arp_CfgIsDiscardedEntryHandlingEnabled(void)
{
  return (IPV4_ARP_SUPPORT_DISCARD_HANDLING == STD_ON);
}

/**********************************************************************************************************************
 *  IpV4_Arp_CfgDiscardedEntryHandlingCbkCount()
 *********************************************************************************************************************/
/* PRQA S 3219 1 */ /* MD_MSR_14.1 */
IPV4_LOCAL_INLINE FUNC(uint8, IPV4_CODE) IpV4_Arp_CfgDiscardedEntryHandlingCbkCount(void)
{
  return TCPIP_PHYS_ADDR_TABLE_CHG_DISCARDED_FUNCTION_CNT;
}

#   if (IPV4_ARP_SUPPORT_DISCARD_HANDLING == STD_ON)
/**********************************************************************************************************************
 *  IpV4_Arp_CfgDiscardedEntryHandlingCbk()
 *********************************************************************************************************************/
/* PRQA S 3219 1 */ /* MD_MSR_14.1 */
IPV4_LOCAL_INLINE FUNC(TcpIp_PhysAddrTableChgDiscardedCbkType, IPV4_CODE) IpV4_Arp_CfgDiscardedEntryHandlingCbk(uint8 CalloutIdx)
{
  return TcpIp_PhysAddrTableChgDiscardedCallbacks[CalloutIdx];
}
#   endif

#   define IPV4_STOP_SEC_CODE
#   include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#  endif /* IPV4_UNIT_TEST */
# endif
#endif /* IPV4_XCFG_H */

/**********************************************************************************************************************
 *  END OF FILE: TcpIp_IpV4_XCfg.h
 *********************************************************************************************************************/
