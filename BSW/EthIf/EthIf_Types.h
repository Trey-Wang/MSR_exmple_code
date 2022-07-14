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
/**        \file  EthIf_Types.h
 *        \brief  Ethernet Interface data type header file
 *
 *      \details  Component to abstract the underlying Ethernet hardware for the upper layers to provide a generic
 *                interface to be used for controlling and communicate with the help of the Ethernet hardware
 *                provided by an ECU (Ethernet controller, Ethernet transceiver, Ethernet switch).
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

#if !defined (ETHIF_TYPES_H)
# define ETHIF_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
/* \trace SPEC-2393718 */
/* \trace SPEC-2393858 */
# include "EthIf_HwTypes.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* \trace SPEC-2393572 */
typedef uint8  EthIf_ConfigType;
typedef uint8  EthIf_TrcvStateType;
/* \trace SPEC-2393573 */
typedef uint8  EthIf_StateType;


/*! Defines the invalid VLAN ID */
# define ETHIF_INV_VLAN_ID  (4095u)

/*! Holds Ethernet header information on VLAN level */
typedef struct
{
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) DstMacAddrPtr;  /*!< Destionation MAC address contained in Ethernet header */
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) SrcMacAddrPtr;  /*!< Soruce MAC address contained in Ethernet header */
          Eth_FrameType                EtherType;      /*!< EtherType contained in Ethernet header */
          uint16                       VlanId;         /*!< VLAN ID contained in VLAN tag of Ethernet header - if not VLAN tagged it holds ETHIF_INV_VLAN_ID */
          uint8                        Priority;       /*!< VLAN PCP contained in VLAN tag of Ethernet header - only valid if VlanId is not equal ETHIF_INV_VLAN_ID */
} EthIf_FrameHdrType;

/*! Transmission statistic counters of an EthIf-controller */
typedef struct
{
  uint32  NumTxPkts;   /*!< Pakets transmitted on the EthIf-controller */
  uint32  NumTxBytes;  /*!< Bytes transmitted on the EthIf-controller */
} EthIf_TxStatsType;
/*! Reception statistic counters of an EthIf-controller */
typedef struct
{
  uint32  NumRxPkts;   /*!< Packets received on the EthIf-controller */
  uint32  NumRxBytes;  /*!< Bytes received on the EthIf-controller */
} EthIf_RxStatsType;

#endif /* ETHIF_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_Types.h
 *********************************************************************************************************************/

