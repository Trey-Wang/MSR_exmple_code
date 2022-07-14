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
 *         File:  IpBase_Priv.h
 *      Project:  MICROSAR ETH
 *    Component:  SysService_IpBase
 *       Module:  MICROSAR IP Base
 *    Generator:  None
 *
 *  Description:  Private header
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
#if !defined (IPBASE_PRIV_H)
# define IPBASE_PRIV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* PRQA S 857 NO_OF_MACROS */ /* MD_IpBase_857 */
#include "IpBase_Types.h"
#if ( IPBASE_DEV_ERROR_DETECT == STD_ON )
  #include "Det.h"
#endif
  /* IPBASE_DEV_ERROR_DETECT */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* MICROSAR-IP internal instance to distinguish CDDs in DETs */
#define IPBASE_INSTANCE_ID              110u

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453,3458 2 */ /* MD_MSR_19.7 MD_MSR_19.4 */
#define IPBASE_BEGIN_CRITICAL_SECTION() SchM_Enter_IpBase_IPBASE_EXCLUSIVE_AREA_0()
#define IPBASE_END_CRITICAL_SECTION() SchM_Exit_IpBase_IPBASE_EXCLUSIVE_AREA_0()

/* Development Error Tracer */
#if ( IPBASE_DEV_ERROR_DETECT == STD_ON )
#else
  #define IPBASE_DUMMY_STATEMENT(v) (void)(v) /* PRQA S 3453 */ /* MD_MSR_19.7 */  /* /MICROSAR/EcuC/EcucGeneral/DummyStatementKind */
#endif
  /* IPBASE_DEV_ERROR_DETECT */

/* PRQA L:NO_OF_MACROS */
#endif
  /* IPBASE_PRIV_H */
/**********************************************************************************************************************
 *  END OF FILE: IpBase_Priv.h
 *********************************************************************************************************************/
