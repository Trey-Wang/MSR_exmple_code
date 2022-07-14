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
 *         File:  IpBase_Cfg.h
 *      Project:  MICROSAR ETH
 *    Component:  SysService_IpBase
 *       Module:  MICROSAR IP Base
 *    Generator:  None
 *
 *  Description:  Configuration header
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
#if !defined (IPBASE_CFG_H)
# define IPBASE_CFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define IPBASE_VERSION_INFO_API                          STD_ON
#  define IPBASE_DEV_ERROR_DETECT                         STD_OFF
#  define IPBASE_DEV_ERROR_REPORT                         STD_OFF
# define IPBASE_LOCAL                                     static
/* module function activation */
# define IPBASE_CODE_ENABLE                               STD_ON
# define IPBASE_COPY_ENABLE                               STD_ON
/* IPBASE_COPY_MACRO_ENABLE requires VStdMemCpy in VStdLib.c, switch off for local copy routine
   Please note that VStdMemCpy is limited to 64k blocks */
# define IPBASE_COPY_MACRO_ENABLE                         STD_ON
# define IPBASE_PBUF_ENABLE                               STD_ON
# define IPBASE_STRING_ENABLE                             STD_ON
# define IPBASE_SOCK_ENABLE                               STD_ON
/* module function code activation */
# if ( IPBASE_CODE_ENABLE == STD_ON )
#  define IPBASE_CODE_BASE64_ENABLE                       STD_ON
#  define IPBASE_CODE_BER_ENABLE                          STD_ON
# else
#  define IPBASE_CODE_BASE64_ENABLE                       STD_OFF
#  define IPBASE_CODE_BER_ENABLE                          STD_OFF
# endif
  /* IPBASE_CODE_ENABLE */
/* switch off/on local definition of E_PENDING */
# define IPBASE_DEFINE_E_PENDING                          STD_OFF

#endif 
  /* IPBASE_CFG_H */
/**********************************************************************************************************************
 *  END OF FILE: IpBase_Cfg.h
 *********************************************************************************************************************/
