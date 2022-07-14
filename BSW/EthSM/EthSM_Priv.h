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
 *         File:  EthSM_Priv.h
 *      Project:  MICROSAR ETH
 *       Module:  Ethernet State Manager
 *    Generator:  Ccl_AsrSmEth.jar
 *
 *  Description:  header containing macros, types and declarations used EthSM internal only
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

#if !defined( ETHSM_PRIV_H )
#define ETHSM_PRIV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthSM_Types.h"
#if (( STD_ON == ETHSM_IS_LINK_DOWN_DEM_ENABLED ) || ( STD_ON == ETHSM_IS_ETHIF_TIMEOUT_DEM_ENABLED ) )
#include "Dem.h"
#endif /* ETHSM_IS_LINK_DOWN_DEM_ENABLED, ETHSM_IS_ETHIF_TIMEOUT_DEM_ENABLED */
#if ( STD_ON == ETHSM_DEV_ERROR_DETECT )
#include "Det.h"
#endif /* ETHSM_DEV_ERROR_DETECT */
#include "EthSM_Lcfg.h"
#include "SchM_EthSM.h"
#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
#  if ( STD_ON == ETHSM_IS_VECTOR_TCPIP)
#include "TcpIp.h"
#  else
#include "TcpIp_EthSM.h"
#  endif /* ETHSM_IS_VECTOR_TCPIP */
#endif /* ETHSM_IS_TCPIP_ENABLED */
#if ( STD_ON == ETHSM_IS_COMM_ENABLED )
#include "ComM.h"
#endif
#if ( STD_OFF == ETHSM_DUMMY_MODE )
#include "EthIf.h"
#endif


/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define ETHSM_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"

/*
 * Because there is no UNINIT state in the EthSM_NetworkModeStateType defined this boolean value is used to
 * indicate if EthSM was initialized.
 */
extern ETHSM_VARZEROINIT(boolean) EthSM_IsInitialized;

#define ETHSM_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* only one instance is supported */
#define ETHSM_INSTANCE_ID                   0u
#define ETHSM_MAX_CFGS_TOTAL                1u
#define ETHSM_INV_NETWORK_IDX               0xFFu

#if ( ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT ) || ( STD_ON == ETHSM_ENABLE_OOO_INDICATIONS ) )
/* Macros for manipulation of EthSM_IndCtrlBitfieldType */
/* init value and bit positions */
#define ETHSM_IND_CTRL_BITFIELD_INIT                         (EthSM_IndCtrlBitfieldType)0x00u
#  if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
#define ETHSM_IND_CTRL_BITFIELD_ETHIF_CALL_PENDING_MASK      (EthSM_IndCtrlBitfieldType)0x01u
#  endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
#  if ( STD_ON == ETHSM_ENABLE_OOO_INDICATIONS )
#define ETHSM_IND_CTRL_BITFIELD_OOO_TRCVLINK_STATE_CHG_MASK  (EthSM_IndCtrlBitfieldType)0x02u
#define ETHSM_IND_CTRL_BITFIELD_OOO_TRCVLINK_STATE_CHG_SHIFT 0x01u
#define ETHSM_IND_CTRL_BITFIELD_OOO_TCPIP_MODE_IND_MASK      (EthSM_IndCtrlBitfieldType)0x04u
#define ETHSM_IND_CTRL_BITFIELD_OOO_TCPIP_MODE_IND_SHIFT     0x02u
#  endif /* ETHSM_ENABLE_OOO_INDICATIONS */
/* PRQA S 3453 FCT_MACROS */ /* MD_MSR_19.7 */
/* init macro */
#define EthSM_IndCtrlBitfield_Init(Bitfield) (Bitfield) = ETHSM_IND_CTRL_BITFIELD_INIT
#  if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
/* Pending Request macros */
#define EthSM_IndCtrlBitfield_SetEthIfCallPending(Bitfield) \
            (Bitfield) = (EthSM_IndCtrlBitfieldType)( (uint32)(Bitfield) | (uint32)ETHSM_IND_CTRL_BITFIELD_ETHIF_CALL_PENDING_MASK )

#define EthSM_IndCtrlBitfield_IsEthIfCallPending(Bitfield) \
            (boolean)((uint32)(Bitfield) & (uint32)ETHSM_IND_CTRL_BITFIELD_ETHIF_CALL_PENDING_MASK)

#define EthSM_IndCtrlBitfield_ClearEthIfCallPending(Bitfield) \
            (Bitfield) = (EthSM_IndCtrlBitfieldType)( (uint32)(Bitfield) & (~((uint32)ETHSM_IND_CTRL_BITFIELD_ETHIF_CALL_PENDING_MASK)) )

#  endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
#  if ( STD_ON == ETHSM_ENABLE_OOO_INDICATIONS )
/* Out of Order Transceiver Link State Change macros */
#define EthSM_IndCtrlBitfield_SetOooTrcvLinkStateChg(Bitfield) \
            (Bitfield) = (EthSM_IndCtrlBitfieldType)( (uint32)(Bitfield) | (uint32)ETHSM_IND_CTRL_BITFIELD_OOO_TRCVLINK_STATE_CHG_MASK )

#define EthSM_IndCtrlBitfield_IsOooTrcvLinkStateChgOccurred(Bitfield) \
            (boolean)(((uint32)(Bitfield) & (uint32)ETHSM_IND_CTRL_BITFIELD_OOO_TRCVLINK_STATE_CHG_MASK) \
                       >> (uint32)ETHSM_IND_CTRL_BITFIELD_OOO_TRCVLINK_STATE_CHG_SHIFT)

#define EthSM_IndCtrlBitfield_ClearOooTrcvLinkStateChg(Bitfield) \
            (Bitfield) = (EthSM_IndCtrlBitfieldType)( (uint32)(Bitfield) & (~((uint32)ETHSM_IND_CTRL_BITFIELD_OOO_TRCVLINK_STATE_CHG_MASK)) )

/* Out of Order TcpIp Mode Indication macros */
#define EthSM_IndCtrlBitfield_SetOooTcpIpModeInd(Bitfield) \
            (Bitfield) = (EthSM_IndCtrlBitfieldType)( (uint32)(Bitfield) | (uint32)ETHSM_IND_CTRL_BITFIELD_OOO_TCPIP_MODE_IND_MASK )

#define EthSM_IndCtrlBitfield_IsOooTcpIpModeIndOccurred(Bitfield) \
            (boolean)(((uint32)(Bitfield) & (uint32)ETHSM_IND_CTRL_BITFIELD_OOO_TCPIP_MODE_IND_MASK) \
                       >> (uint32)ETHSM_IND_CTRL_BITFIELD_OOO_TCPIP_MODE_IND_SHIFT)

#define EthSM_IndCtrlBitfield_ClearOooTcpIpModeInd(Bitfield) \
            (Bitfield) = (EthSM_IndCtrlBitfieldType)( (uint32)(Bitfield) & (~((uint32)ETHSM_IND_CTRL_BITFIELD_OOO_TCPIP_MODE_IND_MASK)) )

#  endif /* ETHSM_ENABLE_OOO_INDICATIONS */
/* PRQA L:FCT_MACROS */
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT, ETHSM_ENABLE_OOO_INDICATIONS */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
#define ETHSM_BEGIN_CRITICAL_SECTION() SchM_Enter_EthSM_ETHSM_EXCLUSIVE_AREA_0()
#define ETHSM_END_CRITICAL_SECTION()   SchM_Exit_EthSM_ETHSM_EXCLUSIVE_AREA_0()


/* Development Error Tracer */
#if ( STD_ON == ETHSM_DEV_ERROR_DETECT )
/* PRQA S 3453 FCT_MACROS */ /* MD_MSR_19.7 */
# define ETHSM_CheckDetErrorReturnVoid(CONDITION, API_ID, ERROR_CODE ) \
    { \
      if( !(CONDITION) ) \
      { \
        (void)Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, (API_ID), (ERROR_CODE)); \
        return; \
      } \
    } /* PRQA S 3458, 3109 */ /* MD_MSR_19.4, MD_MSR_14.3 */

# define ETHSM_CheckDetErrorReturnValue(CONDITION, API_ID, ERROR_CODE, RET_VAL ) \
    { \
      if( !(CONDITION) ) \
      { \
        (void)Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, (API_ID), (ERROR_CODE)); \
        return (RET_VAL); \
      } \
    } /* PRQA S 3458 */ /* MD_MSR_19.4 */

# define ETHSM_CheckDetErrorContinue(CONDITION, API_ID, ERROR_CODE ) \
    { \
      if( !(CONDITION) ) \
      { \
          (void)Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, (API_ID), (ERROR_CODE)); \
      } \
    } /* PRQA S 3458 */ /* MD_MSR_19.4 */

# define ETHSM_CheckDetErrorIsInitReturnVoid(API_ID) \
    { \
      if( FALSE == EthSM_IsInitialized ) \
      { \
        (void)Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, (API_ID), ETHSM_E_UNINIT); \
        return; \
      } \
    } /* PRQA S 3458, 3109 */ /* MD_MSR_19.4, MD_MSR_14.3 */

# define ETHSM_CheckDetErrorIsInitReturnValue(API_ID, RET_VAL) \
    { \
      if( FALSE == EthSM_IsInitialized ) \
      { \
        (void)Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, (API_ID), ETHSM_E_UNINIT); \
        return (RET_VAL); \
      } \
    } /* PRQA S 3458 */ /* MD_MSR_19.4 */

# define ETHSM_CallDetReportError( API_ID, ERROR_CODE ) \
    (void)Det_ReportError(ETHSM_MODULE_ID, ETHSM_INSTANCE_ID, (API_ID), (ERROR_CODE))

#else

# define ETHSM_CheckDetErrorReturnVoid(CONDITION, API_ID, ERROR_CODE )
# define ETHSM_CheckDetErrorReturnValue(CONDITION, API_ID, ERROR_CODE, RET_VAL )
# define ETHSM_CheckDetErrorContinue(CONDITION, API_ID, ERROR_CODE )
# define ETHSM_CheckDetErrorIsInitReturnVoid(API_ID)
# define ETHSM_CheckDetErrorIsInitReturnValue(API_ID, RET_VAL )
# define ETHSM_CallDetReportError(API_ID, ERROR_CODE )

/* PRQA L:FCT_MACROS */
#endif /* ETHSM_DEV_ERROR_DETECT */

/* Configuration access (depending on configuration variant)*/
#if  ( ETHSM_CONFIGURATION_VARIANT == ETHSM_CONFIGURATION_VARIANT_PRECOMPILE )
#define EthSM_VCfgGetNetworks()                (EthSM_Networks)
#define EthSM_VCfgGetNetworkTotal()            (ETHSM_NETWORK_TOTAL)
#endif /* ETHSM_CONFIG_VARIANT */

#endif /* ETHSM_PRIV_H */
/**********************************************************************************************************************
 *  END OF FILE: EthSM_priv.h
 *********************************************************************************************************************/
