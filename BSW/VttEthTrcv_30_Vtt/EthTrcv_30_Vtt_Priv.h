/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                                  All rights reserved.
 * 
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  EthTrcv_30_Vtt_Priv.h
 *      Project:  MICROSAR Eth
 *       Module:  Ethernet Transceiver Driver for VTT
 *    Generator:  DrvTrans_VttEthAsr.jar
 *
 *  Description:  Header file containing the modules private data structures
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

#if !defined (ETHTRCV_30_VTT_PRIV_H)
# define ETHTRCV_30_VTT_PRIV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#if (ETHTRCV_30_VTT_DEM_ERROR_DETECT == STD_ON)
#include "Dem.h"
#endif
  /* ETHTRCV_30_VTT_DEM_ERROR_DETECT */
#if (ETHTRCV_30_VTT_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif
  /* ETHTRCV_30_VTT_DEV_ERROR_DETECT */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define ETHTRCV_30_VTT_MAX_CFGS_TOTAL             1u
#define ETHTRCV_30_VTT_CONFIG_VARIANT_PRECOMPILE  1u /* value for pre-compile time config variant */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453,3458 FCT_LIKE_MACROS *//* MISRA 19.4,19.7: Macros improve code readability */

/* Development Error Tracer */
#if ( ETHTRCV_30_VTT_DEV_ERROR_DETECT == STD_ON )
/* PRQA S 3453 8 */ /* MD_MSR_19.7 */
#define EthTrcv_30_Vtt_CheckDetErrorReturnVoid(  CONDITION, INSTANCE_ID, API_ID, ERROR_CODE )          if(!(CONDITION)) { (void)Det_ReportError( ETHTRCV_30_VTT_MODULE_ID, (INSTANCE_ID), (API_ID), (ERROR_CODE)); return; }
#define EthTrcv_30_Vtt_CheckDetErrorReturnValue( CONDITION, INSTANCE_ID, API_ID, ERROR_CODE, RET_VAL ) if(!(CONDITION)) { (void)Det_ReportError( ETHTRCV_30_VTT_MODULE_ID, (INSTANCE_ID), (API_ID), (ERROR_CODE)); return (RET_VAL); }
#define EthTrcv_30_Vtt_CheckDetErrorContinue(    CONDITION, INSTANCE_ID, API_ID, ERROR_CODE )          if(!(CONDITION)) { (void)Det_ReportError( ETHTRCV_30_VTT_MODULE_ID, (INSTANCE_ID), (API_ID), (ERROR_CODE)); }
#define EthTrcv_30_Vtt_CallDetReportError( INSTANCE_ID, API_ID, ERROR_CODE )                           (void)Det_ReportError( ETHTRCV_30_VTT_MODULE_ID, (INSTANCE_ID), (API_ID), (ERROR_CODE))
#else
/* PRQA S 3453 4 */ /* MD_MSR_19.7 */
#define EthTrcv_30_Vtt_CheckDetErrorReturnVoid(  CONDITION, INSTANCE_ID, API_ID, ERROR_CODE )
#define EthTrcv_30_Vtt_CheckDetErrorReturnValue( CONDITION, INSTANCE_ID, API_ID, ERROR_CODE, RET_VAL )
#define EthTrcv_30_Vtt_CheckDetErrorContinue(    CONDITION, INSTANCE_ID, API_ID, ERROR_CODE )
#define EthTrcv_30_Vtt_CallDetReportError( INSTANCE_ID, API_ID, ERROR_CODE )
#endif
  /* ETHTRCV_30_VTT_DEV_ERROR_DETECT */

/* Configuration access (depending on configuration variant)*/

#  define EthTrcv_30_Vtt_VCfgGetCtrlIdx(TrcvIdx)           (EthTrcv_30_Vtt_HwIndex[TrcvIdx])

#  define EthTrcv_30_Vtt_VCfgGetMiiAddr(TrcvIdx)           (EthTrcv_30_Vtt_HwSubIndex[TrcvIdx])

#  define EthTrcv_30_Vtt_VCfgGetDemErrorId(TrcvIdx)        (EthTrcv_30_Vtt_DemEvents[TrcvIdx])

/* PRQA L:FCT_LIKE_MACROS */
#endif
  /* ETHTRCV_30_VTT_PRIV_H */
/**********************************************************************************************************************
 *  END OF FILE: EthTrcv_30_Vtt_Priv.h
 *********************************************************************************************************************/
