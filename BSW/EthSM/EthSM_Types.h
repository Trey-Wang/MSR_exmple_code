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
 *         File:  EthSM_Types.h
 *      Project:  MICROSAR ETH
 *       Module:  Ethernet State Manager
 *    Generator:  Ccl_AsrSmEth.jar
 *
 *  Description:  types header
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

#if !defined( ETHSM_TYPES_H )
#define ETHSM_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComStack_Types.h"
#include "EthSM_Cfg.h"

/* adjacent module includes */
#if (( STD_ON == ETHSM_IS_LINK_DOWN_DEM_ENABLED ) || ( STD_ON == ETHSM_IS_ETHIF_TIMEOUT_DEM_ENABLED ))
#include "Dem.h"
#endif /* ETHSM_IS_LINK_DOWN_DEM_ENABLED, ETHSM_IS_ETHIF_TIMEOUT_DEM_ENABLED */
#include "ComM.h"
#if ( STD_ON == ETHSM_IS_COMM_ENABLED )
#include "ComM_BusSM.h"
#endif /* ETHSM_ENABLE_CCL_ASRCOMM */
#if ( STD_OFF == ETHSM_DUMMY_MODE )
#include "EthIf.h"
#endif /* ETHSM_DUMMY_MODE */
#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
#  if ( STD_ON == ETHSM_IS_VECTOR_TCPIP)
#include "TcpIp.h"
#  else
#include "TcpIp_EthSM.h"
#  endif /* ETHSM_IS_VECTOR_TCPIP */
#endif /* ETHSM_IS_TCPIP_ENABLED */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#if ( ( STD_OFF == ETHSM_IS_TCPIP_ENABLED ) || ( STD_ON == ETHSM_IS_VECTOR_TCPIP ) )
#define TCPIP_STATE_OFFLINE          0u
#define TCPIP_STATE_ONHOLD           1u
#define TCPIP_STATE_ONLINE           2u
#endif /* ETHSM_IS_TCPIP_ENABLED, ETHSM_IS_VECTOR_TCPIP */

#if ( STD_ON == ETHSM_IS_MIXED_STACK )
#define ETHSM_STACK_TYPE_TCPIP               0u
#define ETHSM_STACK_TYPE_AVB                 1u
#endif /* ETHSM_IS_MIXED_STACK */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FCT_MACROS */ /* MD_MSR_19.7 */
#define ETHSM_P2CONSTCFGROOT(Type)       P2CONST(Type, AUTOMATIC, ETHSM_CONST)
#define ETHSM_P2CONSTCFG(Type)           P2CONST(Type, AUTOMATIC, ETHSM_CONST)
#define ETHSM_P2CONSTAPPLDATA(Type)      P2CONST(Type, AUTOMATIC, ETHSM_APPL_DATA)
#define ETHSM_CONSTP2CONSTAPPLDATA(Type) CONSTP2CONST(Type, AUTOMATIC, ETHSM_APPL_DATA)
#define ETHSM_P2VARAPPLDATA(Type)        P2VAR(Type, AUTOMATIC, ETHSM_APPL_DATA)
#define ETHSM_VARNOINIT(Type)            VAR(Type, ETHSM_VAR_NOINIT)
#define ETHSM_VARZEROINIT(Type)          VAR(Type, ETHSM_VAR_ZERO_INIT)
#define ETHSM_CONSTCONST(Type)           CONST(Type, ETHSM_CONST)
/* PRQA L:FCT_MACROS */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*
 * Data Types and Structures specified by AUTOSAR SWS
 */

#if ( STD_OFF == ETHSM_IS_TCPIP_ENABLED )
typedef uint8  TcpIp_StateType;
#endif /* ETHSM_IS_TCPIP_ENABLED */

typedef uint8  EthSM_NetworkModeStateType;

/*
 * Due to SREQ00018431, which is derived from SWS_EthSM_00061 of the EthSM AUTOSAR SWS, only PreCompile configuration
 * is supported and no PostBuild configuration structure will be provided.
 */
typedef uint8  EthSM_ConfigType;

/*
 * Data Types and Structures used by EthSM internal
 */

typedef uint16 EthSM_EthIfTimeoutType;
typedef uint8  EthSM_EthIfRetriesType;
typedef uint8  EthSM_NetworkIdxType;

/*
 * The EthSM_IndCtrlBitfieldType describes a Bitfield containing control and indication bits.
 *
 *      Bit   Description
 *      _____ _____________________________________________________
 *     |0    | EthIf API call pending                              |
 *     -------------------------------------------------------------
 *     |1    | Out of Order Transceiver Link State Change occurred |
 *     -------------------------------------------------------------
 *     |2    | Out of Order TcpIp Mode Indication occurred         |
 *     -------------------------------------------------------------
 *     |3    | Reserved                                            |
 *     |4    | Reserved                                            |
 *     |5    | Reserved                                            |
 *     |6    | Reserved                                            |
 *     |7    | Reserved                                            |
 *     |_____|_____________________________________________________|
 */
#if ( ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT ) || (STD_ON == ETHSM_ENABLE_OOO_INDICATIONS) )
typedef uint8  EthSM_IndCtrlBitfieldType;
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT, ETHSM_ENABLE_OOO_INDICATIONS */

/*
 * The EthSM_NetworkType contains all constant properties of an Ethernet Network.
 */
typedef struct
{
#if ( STD_ON == ETHSM_IS_LINK_DOWN_DEM_ENABLED )
  const Dem_EventIdType        LinkDownDemEventId;     /* ID of the Link Down DEM Event used by the network */
#endif /* ETHSM_IS_LINK_DOWN_DEM_ENABLED */
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
#  if ( STD_ON == ETHSM_IS_ETHIF_TIMEOUT_DEM_ENABLED )
  const Dem_EventIdType        EthIfTimeoutDemEventId; /* ID of the EthIf Timeout DEM Event used by the network */
#  endif
  const EthSM_EthIfTimeoutType ModeChgTimeout;         /* time to wait for next retry attempt */
  const EthSM_EthIfRetriesType MaxRetries;             /* maximum number of retries */
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
  const NetworkHandleType      ComMNetwHnd;            /* corresponding Network Handle of the ComM Network */
#if ( STD_OFF == ETHSM_DUMMY_MODE )
  const uint8                  EthIfCtrlIdx;           /* corresponding Controller Index of the EthIf Controller */
#endif
#if ( STD_ON == ETHSM_IS_MIXED_STACK )
  const uint8                  StackType;              /* type of stack communicating on network (TcpIp, Avb, ...) */
#endif /* ETHSM_IS_MIXED_STACK */
} EthSM_NetworkType;

/*
 * The EthSM_NetworkMngmtType contains all properties of an Ethernet Network, which will change during runtime.
 */
typedef struct
{
  void  (*ExeStateTrans)(EthSM_NetworkIdxType NetIdx); /* Function to execute for state transition */
#if ( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT )
  EthSM_EthIfTimeoutType   RemModeChgTime;             /* remaining time to wait for next retry */
  EthSM_EthIfRetriesType   RetriesLeft;                /* remaining number of retries */
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT */
  ComM_ModeType              Mode;                     /* Communication Mode of Ethernet network */
  EthSM_NetworkModeStateType State;                    /* internal State Machine state */
#if ( STD_OFF == ETHSM_DUMMY_MODE )
  EthTrcv_LinkStateType      TrcvLinkState;            /* current Transceiver Link State */
#endif /* ETHSM_DUMMY_MODE */
#if ( STD_ON == ETHSM_IS_TCPIP_ENABLED )
  TcpIp_StateType            TcpIpState;               /* current TcpIp state */
#endif /* ETHSM_IS_TCPIP_ENABLED */
#if ( STD_ON == ETHSM_IS_VECTOR_TCPIP )
  uint8                      ActiveIpVxCnt;            /* counter for IP activity on Network */
#endif
#if (( STD_ON == ETHSM_ENABLE_ETHIF_API_TIMEOUT ) || ( STD_ON == ETHSM_ENABLE_OOO_INDICATIONS ) )
  EthSM_IndCtrlBitfieldType  IndCtrlBitfield;          /* Bitfield containing information about indications of
                                                          adjacent modules and control bits */
#endif /* ETHSM_ENABLE_ETHIF_API_TIMEOUT, ETHSM_ENABLE_OOO_INDICATIONS */
} EthSM_NetworkStateMngmtType;

#endif /* ETHSM_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: EthSM_Types.h
 *********************************************************************************************************************/
