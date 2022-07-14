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
 *         File:  IpBase.c
 *      Project:  MICROSAR ETH
 *    Component:  SysService_IpBase
 *       Module:  MICROSAR IP Base
 *    Generator:  None
 *
 *  Description:  Implementation
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
#define IPBASE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* PRQA S 857 NO_OF_MACROS */ /* MD_IpBase_857 */
/*lint -e451 */ /* LD_IpBase_451 */
#include "IpBase.h"
#include "IpBase_Priv.h"

/* PRQA L:NO_OF_MACROS */
/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check consistency of source and header file. */
#if ( (IPBASE_SW_MAJOR_VERSION != 3u) || (IPBASE_SW_MINOR_VERSION != 0u) || (IPBASE_SW_PATCH_VERSION != 0u) )
# error "IpBase.c: Source and Header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define IPBASE_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/**********************************************************************************************************************
 *  IpBase_GetVersionInfo
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
#if ( IPBASE_VERSION_INFO_API == STD_ON )
IPBASE_FUNCCODE(void) IpBase_GetVersionInfo( IPBASE_P2VARAPPLDATA(Std_VersionInfoType) VersionInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (IPBASE_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check VersionInfoPtr for NULL pointer */
  if ( VersionInfoPtr == NULL_PTR )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set vendor ID, module ID, major+minor+patch version */
    VersionInfoPtr->vendorID = (IPBASE_VENDOR_ID);
    VersionInfoPtr->moduleID = (IPBASE_MODULE_ID);
    VersionInfoPtr->sw_major_version = (IPBASE_SW_MAJOR_VERSION);
    VersionInfoPtr->sw_minor_version = (IPBASE_SW_MINOR_VERSION);
    VersionInfoPtr->sw_patch_version = (IPBASE_SW_PATCH_VERSION);
  }

  /* ----- Development Error Report ------------------------------------- */
# if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_ID_GET_VERSION_INFO, errorId);
  }
# else
  IPBASE_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
} /* IpBase_GetVersionInfo() */
#endif
  /* IPBASE_VERSION_INFO_API */

#define IPBASE_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

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
 MD_IpBase_21.1:
      Reason:     No wraparound in code
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
/* module specific LINT deviations:
 LD_IpBase_451:
      Reason:     AUTOSAR MemMap
      Risk:       Uncritical
      Prevention: Covered by code review
*/
/**********************************************************************************************************************
 *  END OF FILE: IpBase.c
 *********************************************************************************************************************/
