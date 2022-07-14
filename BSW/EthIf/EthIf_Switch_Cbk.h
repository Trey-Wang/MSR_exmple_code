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
/**        \file  EthIf_Switch_Cbk.h
 *        \brief  Ethernet Interface Ethernet switch abstraction callback header file
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

#ifndef ETHIF_SWITCH_CBK_H
#define ETHIF_SWITCH_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthIf_Types.h"
#include "EthIf_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define ETHIF_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( ETHIF_ETHSWT_USED == STD_ON )

# if ( ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_SwitchMgmtInfoIndication
 *********************************************************************************************************************/
/*! \brief         Notifies the EthIf about switch management information related to a received Ethernet frame
 *  \details       Function allows to provide switch management information for an received Ethernet frame to
 *                 the EthIf.
 *  \param[in]     CtrlIdx         Index of the EthIf controller
 *  \param[in]     DataPtr         Pointer to identify the frame context
 *  \param[in]     MgmtInfo        Switch management information
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace CREQ-111224
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_SwitchMgmtInfoIndication(
          uint8                                            CtrlIdx,
    P2VAR(uint8,               AUTOMATIC, ETHIF_APPL_VAR)  DataPtr,
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_CONST)     MgmtInfo);
# endif /* ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT */

# if ( ETHIF_ETHSWT_TIME_STAMP_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_SwitchEgressTimeStampIndication
 *********************************************************************************************************************/
/*! \brief         Notifies the EthIf about time stamping information related to an Ethernet frame transmitted
 *  \details       Function allows to provide time stamping information for an Ethernet frame transmitted at a
 *                 switch port to the EthIf.
 *  \param[in]     CtrlIdx         Index of the EthIf controller
 *  \param[in]     DataPtr         Pointer to identify the frame context
 *  \param[in]     MngmtInfo       Switch management information
 *  \param[in]     timeStamp       Port Egress Time stamp
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_SwitchEgressTimeStampIndication(
          uint8                                            CtrlIdx,
    P2VAR(uint8,               AUTOMATIC, ETHIF_APPL_VAR)  DataPtr,
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_CONST)     MgmtInfo,
  P2CONST(Eth_TimeStampType,   AUTOMATIC, ETHIF_CONST)     timeStamp);

/**********************************************************************************************************************
 *  EthIf_SwitchIngressTimeStampIndication
 *********************************************************************************************************************/
/*! \brief         Notifies the EthIf about time stamping information related to an Ethernet frame received
 *  \details       Function allows to provide time stamping information for an Ethernet frame received at a switch
 *                 port to the EthIf.
 *  \param[in]     CtrlIdx         Index of the EthIf controller
 *  \param[in]     DataPtr         Pointer to identify the frame context
 *  \param[in]     MngmtInfo       Switch management information
 *  \param[in]     timeStamp       Port Ingress Time stamp
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_SwitchIngressTimeStampIndication(
          uint8                                            CtrlIdx,
    P2VAR(uint8,               AUTOMATIC, ETHIF_APPL_VAR)  DataPtr,
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_CONST)     MgmtInfo,
  P2CONST(Eth_TimeStampType,   AUTOMATIC, ETHIF_CONST)     timeStamp);

# endif /* ETHIF_ETHSWT_TIME_STAMP_SUPPORT */

#endif /* ETHIF_ETHSWT_USED */

#define ETHIF_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* ETHIF_SWITCH_CBK_H */
/**********************************************************************************************************************
 *  END OF FILE: EthIf_Switch_Cbk.h
 *********************************************************************************************************************/

