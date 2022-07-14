/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 * 
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Exi_Priv.h
 *       Module:  Efficient XML Interchange
 *    Generator:  SysService_Exi.dll
 *
 *  Description:  private header
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file Exi.h.
 *********************************************************************************************************************/

#if (!defined EXI_PRIV_H)
# define EXI_PRIV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi_Types.h"
#if (EXI_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif
  /* EXI_DEV_ERROR_DETECT */
#include "Exi_Lcfg.h"
#include "SchM_Exi.h"
#if (EXI_ENABLE_VSTDLIB == STD_ON)
#include "vstdlib.h"
#else
#include "string.h"
#endif

/**********************************************************************************************************************
 *  CONSISTENCY CHECK
 *********************************************************************************************************************/
/* pre-processor checks for building the library */
# if defined ( V_EXTENDED_BUILD_LIB_CHECK )
#  if defined ( EXI_CONFIG_VARIANT ) && (1u == EXI_CONFIG_VARIANT)
#   error "Component Exi: Configuration variant must be 2 or 3 at lib delivery."
#  endif
# endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* PRQA S 0857 MACRO_NUM */ /* MD_MSR_1.1_857 */
#ifndef EXI_ENABLE_STREAMING_SUPPORT
#define EXI_ENABLE_STREAMING_SUPPORT         STD_OFF
#endif

#define EXI_HEADER_BYTE                      0x80u
#define EXI_BITS_IN_BYTE                     8u

#define EXI_SINT8_MAX_POS_NUM                0x7FU                /*                     127 */
#define EXI_SINT8_MAX_NEG_NUM                0x80U                /* (-)                 128 */
#define EXI_SINT16_MAX_POS_NUM               0x7FFFU              /*                   32767 */
#define EXI_SINT16_MAX_NEG_NUM               0x8000U              /* (-)               32768 */
#define EXI_SINT32_MAX_POS_NUM               0x7FFFFFFFU          /*              2147483647 */
#define EXI_SINT32_MAX_NEG_NUM               0x80000000U          /* (-)          2147483648 */
#if defined(Exi_SInt64_Available)
#define EXI_SINT64_MAX_POS_NUM               0x7FFFFFFFFFFFFFFFULL /*     9223372036854775807 */
#define EXI_SINT64_MAX_NEG_NUM               0x8000000000000000ULL /* (-) 9223372036854775808 */
#endif

#define EXI_UINT8_MAX_NUM                    0xFFU
#define EXI_UINT16_MAX_NUM                   0xFFFFU
#define EXI_UINT32_MAX_NUM                   0xFFFFFFFFU
#if defined(Exi_UInt64_Available)
#define EXI_UINT64_MAX_NUM                   0xFFFFFFFFFFFFFFFFULL
#endif

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/
/* PRQA S 3453 FUNCTIONLIKEMACRO */ /* MD_MSR_19.7 */

#if (EXI_ENABLE_VSTDLIB == STD_ON)
# define Exi_ClearMemory(MEMPTR, LENGTH) VStdMemClr((MEMPTR), (LENGTH))
#else
# define Exi_ClearMemory(MEMPTR, LENGTH) memset((MEMPTR), 0, (LENGTH))
#endif

/* Development Error Tracer */
#if ( EXI_DEV_ERROR_DETECT == STD_ON )
#  define Exi_CheckDetErrorReturnVoid(  CONDITION, API_ID, ERROR_CODE )          { if(!(CONDITION)) { (void)Det_ReportError( EXI_MODULE_ID, EXI_INSTANCE_ID, (API_ID), (ERROR_CODE)); return; } }  /* PRQA S 3458 */ /*  MD_MSR_19.4 */
#  define Exi_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL ) { if(!(CONDITION)) { (void)Det_ReportError( EXI_MODULE_ID, EXI_INSTANCE_ID, (API_ID), (ERROR_CODE)); return (RET_VAL); } }  /* PRQA S 3458 */ /*  MD_MSR_19.4 */
#  define Exi_CheckDetErrorContinue(    CONDITION, API_ID, ERROR_CODE )          { if(!(CONDITION)) { (void)Det_ReportError( EXI_MODULE_ID, EXI_INSTANCE_ID, (API_ID), (ERROR_CODE)); } }  /* PRQA S 3458 */ /*  MD_MSR_19.4 */
#  define Exi_CallDetReportError( API_ID, ERROR_CODE )                           (void)Det_ReportError( EXI_MODULE_ID, EXI_INSTANCE_ID, (API_ID), (ERROR_CODE))
#else
#  define Exi_CheckDetErrorReturnVoid(  CONDITION, API_ID, ERROR_CODE )
#  define Exi_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL ) 
#  define Exi_CheckDetErrorContinue(    CONDITION, API_ID, ERROR_CODE ) 
#  define Exi_CallDetReportError( API_ID, ERROR_CODE ) 
#endif
#if ( EXI_INTERNAL_DEV_ERROR_DETECT == STD_ON )
#  define Exi_CheckInternalDetErrorReturnVoid(  CONDITION, INSTANCE_ID, API_ID, ERROR_CODE )          { if(!(CONDITION)) { (void)Det_ReportError( EXI_MODULE_ID, (INSTANCE_ID), (uint8)(API_ID), (ERROR_CODE)); return; } }  /* PRQA S 3458 */ /*  MD_MSR_19.4 */
#  define Exi_CheckInternalDetErrorReturnValue( CONDITION, INSTANCE_ID, API_ID, ERROR_CODE, RET_VAL ) { if(!(CONDITION)) { (void)Det_ReportError( EXI_MODULE_ID, (INSTANCE_ID), (uint8)(API_ID), (ERROR_CODE)); return (RET_VAL); } }  /* PRQA S 3458 */ /*  MD_MSR_19.4 */
#  define Exi_CheckInternalDetErrorContinue(    CONDITION, INSTANCE_ID, API_ID, ERROR_CODE )          { if(!(CONDITION)) { (void)Det_ReportError( EXI_MODULE_ID, (INSTANCE_ID), (uint8)(API_ID), (ERROR_CODE)); } }  /* PRQA S 3458 */ /*  MD_MSR_19.4 */
#  define Exi_CallInternalDetReportError( INSTANCE_ID, API_ID, ERROR_CODE )                           (void)Det_ReportError( EXI_MODULE_ID, (INSTANCE_ID), (uint8)(API_ID), (ERROR_CODE))
#else
# define Exi_CheckInternalDetErrorReturnVoid(  CONDITION, INSTANCE_ID, API_ID, ERROR_CODE )
# define Exi_CheckInternalDetErrorReturnValue( CONDITION, INSTANCE_ID, API_ID, ERROR_CODE, RET_VAL ) 
# define Exi_CheckInternalDetErrorContinue(    CONDITION, INSTANCE_ID, API_ID, ERROR_CODE ) 
# define Exi_CallInternalDetReportError( INSTANCE_ID, API_ID, ERROR_CODE ) 
#endif
  /* EXI_DEV_ERROR_DETECT */
  
/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define EXI_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */  /*  MD_MSR_19.1 */
#if ( EXI_CONFIG_VARIANT == 3 )
extern P2CONST(Exi_PostBuildConfigType, AUTOMATIC, EXI_CONST) Exi_PostBuildConfigPtr;
#endif
  /* EXI_CONFIG_VARIANT */
#define EXI_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */  /*  MD_MSR_19.1 */

#define EXI_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */  /*  MD_MSR_19.1 */
extern VAR(Exi_StateType, EXI_VAR_ZERO_INIT) Exi_State; /* Exi needs state also without DET */
#define EXI_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */  /*  MD_MSR_19.1 */

/* PRQA L:FUNCTIONLIKEMACRO */
/* PRQA L:MACRO_NUM */
#endif
  /* EXI_PRIV_H */
/**********************************************************************************************************************
 *  END OF FILE: Exi_Priv.h
 *********************************************************************************************************************/
