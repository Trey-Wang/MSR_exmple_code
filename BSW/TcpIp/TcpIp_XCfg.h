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
/**        \file  TcpIp_XCfg.h
 *        \brief  Header for static inline configuration access functions for module TcpIp.
 *
 *      \details  Declares and defines the functions of the sub-module XCfg, called by other modules of the TcpIp.
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
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/

#if !defined (TCPIP_XCFG_H)
# define TCPIP_XCFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "TcpIp.h"

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

#  define TCPIP_START_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 *  TcpIp_CfgDuplicateAddrDetectionHandlingCbkCount()
 *********************************************************************************************************************/
/*! \brief       Returns the number of callbacks for discarded ARP entry handling.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(uint8, TCPIP_CODE) TcpIp_CfgDuplicateAddrDetectionHandlingCbkCount(void);

/**********************************************************************************************************************
 *  TcpIp_Ndp_CfgDiscardedEntryHandlingCbk()
 *********************************************************************************************************************/
/*! \brief       Returns the pointer to the callbacks for discarded ARP entry handling according to given index.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(TcpIp_DuplicateAddrDetectionCbkType, TCPIP_CODE) TcpIp_CfgDuplicateAddrDetectionHandlingCbk(uint8 CalloutIdx);

#  define TCPIP_STOP_SEC_CODE 
#  include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#  ifndef TCPIP_UNIT_TEST /* COV_TESTSUITE_INSTRUMENTATION */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#   define TCPIP_START_SEC_CODE 
#   include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  TcpIp_CfgDuplicateAddrDetectionHandlingCbkCount()
 *********************************************************************************************************************/
/* PRQA S 3219 1 */ /* MD_MSR_14.1 */
TCPIP_LOCAL_INLINE FUNC(uint8, TCPIP_CODE) TcpIp_CfgDuplicateAddrDetectionHandlingCbkCount(void)
{
  return TCPIP_DUPLICATE_ADDRESS_DETECTION_CFG_CNT;
}

/**********************************************************************************************************************
 *  TcpIp_CfgDuplicateAddrDetectionHandlingCbk()
 *********************************************************************************************************************/
/* PRQA S 3219 1 */ /* MD_MSR_14.1 */
TCPIP_LOCAL_INLINE FUNC(TcpIp_DuplicateAddrDetectionCbkType, TCPIP_CODE) TcpIp_CfgDuplicateAddrDetectionHandlingCbk(uint8 CalloutIdx)
{
#if (TCPIP_DUPLICATE_ADDRESS_DETECTION_CFG_CNT > 0)
  return TcpIp_DuplicateAddrDetectionCallbacks[CalloutIdx];
#else
  TCPIP_UNUSED_PARAMETER(CalloutIdx);
  return NULL_PTR;
#endif
}

#   define TCPIP_STOP_SEC_CODE
#   include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#  endif /* TCPIP_UNIT_TEST */
#endif /* TCPIP_XCFG_H */

/**********************************************************************************************************************
 *  END OF FILE: TcpIp_XCfg.h
 *********************************************************************************************************************/
