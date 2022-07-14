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
/**        \file  EthIf_ExtndTrafficHndl_Int.h
 *        \brief  Ethernet Interface extended traffic handling extension internal header file
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
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#if !defined (ETHIF_EXTND_TRAFFIC_HNDL_INT_H)
# define ETHIF_EXTND_TRAFFIC_HNDL_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthIf_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ----- API service IDs ----- */
# define ETHIF_SID_MIRRORING_GET_FREE_TX_TRAFFIC_INFO      (0x70U)
# define ETHIF_SID_MIRRORING_GET_RELATED_TX_TRAFFFIC_INFO  (0x71U)
# define ETHIF_SID_GATEWAY_MANIPULATE_PROMISCOUS_MODE      (0x72U)
# define ETHIF_SID_GATEWAY_GATEWAY_TRAFFIC                 (0x73U)

/* Extended traffic handling DET errors */
# define ETHIF_TRAFFIC_GATEWAY_E_MANIPULATE_PROMISCOUS_MODE   (0x10U)
# define ETHIF_TRAFFIC_GATEWAY_E_NO_BUFFER                    (0x11U)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define ETHIF_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if   ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_MIRRORING )

/***********************************************************************************************************************
 *  EthIf_Mirroring_ProvideTxMirroringBuffer
 **********************************************************************************************************************/
/*! \brief      Allocates a transmission buffer at the destination Ethernet controller if the EthIf controller is
 *              involved in transmission traffic mirroring
 *  \details    Function checks if the EthIf controller is involved in transmission traffic mirroring and, if this
 *              is the case, allocates a transmission buffer on the mirroring destination Ethernet controller. The
 *              information is then stored in an data object which can be used during transmission trigger for the
 *              frame to be mirrored.
 *  \param[in]  srcEthIfCtrlIdx  EthIf Controller Index of mirroring source
 *  \param[in]  srcDataPtr       Pointer to the Ethernet payload data of mirroring source
 *  \param[in]  srcDataBufIdx    Buffer index acquired by mirroring source
 *  \param[in]  srcFrameLen      Frame length of mirroring source
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_Mirroring_ProvideTxMirroringBuffer(
          uint8                                     srcEthIfCtrlIdx,
    P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_VAR)  srcDataPtr,
          uint8                                     srcDataBufIdx,
          uint16                                    srcFrameLen);

/***********************************************************************************************************************
 *  EthIf_Mirroring_MirrorTxTraffic
 **********************************************************************************************************************/
/*! \brief      Mirrors transmission traffic to the destination Ethernet controller if the EthIf controller is
 *              involved in transmission traffic mirroring
 *  \details    Function checks if the EthIf controller is involved in transmission traffic mirroring and, if this
 *              is the case, uses the data object setup by EthIf_Mirroring_ProvideTxMirroringBuffer() to mirror the
 *              frame on the destination Ethernet controller.
 *  \param[in]  srcEthIfCtrlIdx  EthIf Controller Index of mirroring source
 *  \param[in]  srcBufIdx        Buffer index acquired by mirroring source
 *  \param[in]  srcFrameType     Frame type of mirroring source
 *  \param[in]  srcFrameLen      Frame length of mirroring source
 *  \param[in]  destMacAddr      Destination MAC of mirroring source
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_Mirroring_MirrorTxTraffic(
          uint8                              srcEthIfCtrlIdx,
          uint8                              srcBufIdx,
          uint16                             srcFrameType,
          uint16                             srcFrameLen,
    P2VAR(uint8, AUTOMATIC, ETHIF_APPL_VAR)  destMacAddr);

/***********************************************************************************************************************
 *  EthIf_Mirroring_MirrorRxTraffic
 **********************************************************************************************************************/
/*! \brief      Mirrors reception traffic to the destination Ethernet controller if the EthIf controller is
 *              involved in transmission traffic mirroring
 *  \details    Function checks if the EthIf controller is involved in reception traffic mirroring and, if this
 *              is the case, allocates buffer and transmits the frame to be mirrored on the destination Ethernet
 *              controller.
 *  \param[in]  srcEthIfCtrlIdx  EthIf Controller Index of mirroring source
 *  \param[in]  srcFrameType     Frame type of mirroring source
 *  \param[in]  srcFrameLen      Frame length of mirroring source
 *  \param[in]  srcDataPtr       Pointer to the Ethernet payload data of mirroring source
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_Mirroring_MirrorRxTraffic(
          uint8                                     srcEthIfCtrlIdx,
          uint16                                    srcFrameType,
          uint16                                    srcFrameLen,
    P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_VAR)  srcDataPtr);
#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */

#if ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_GATEWAY )

/***********************************************************************************************************************
 *  EthIf_Gateway_ManipulatePromiscousMode
 **********************************************************************************************************************/
/*! \brief      Enables/disables the promiscouse mode of the Ethernet controller related to the EthIf controller in
 *              in case EthIf controller is involved in traffic gateway.
 *  \details    Function enables/disables the promiscouse mode of the Ethernet controller related to the EthIf
 *              controller if the EthIf controller is involved in a traffic gateway.
 *  \param[in]  ethIfCtrlIdx  EthIf Controller Index of gateway route source
 *  \param[in]  filterAction  Action to be performed:\n
 *                            - ETH_ADD_TO_FILTER:       enables promiscous mode\n
 *                            - ETH_REMOVE_FROM_FILTER:  disables promiscous mode
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_Gateway_ManipulatePromiscousMode(
    uint8                 ethIfCtrlIdx,
    Eth_FilterActionType  filterAction);

/***********************************************************************************************************************
 *  EthIf_Gateway_GatewayTraffic
 **********************************************************************************************************************/
/*! \brief      Routes traffic to the destination EthIf controller if the source EthIf controller is
 *              involved in a traffic gateway route
 *  \details    Function checks if the EthIf controller is involved in a traffic gateway route and, if this
 *              is the case, allocates buffer and transmits the frame on the destination EthIf controller.
 *  \param[in]  srcEthIfCtrlIdx  EthIf Controller Index of gateway route source
 *  \param[in]  srcFrameType     Frame type of gateway route source
 *  \param[in]  srcFrameLen      Frame length of gateway route source
 *  \param[in]  srcDataPtr       Pointer to the Ethernet payload data of gateway route source
 *  \return     FALSE - Source EthIf controller isn't involved in a gateway route\n
 *  \return     TRUE - Source EthIf controller is involved in a gateway route
 **********************************************************************************************************************/
FUNC(boolean, ETHIF_CODE) EthIf_Gateway_GatewayTraffic(
        uint8                                     srcEthIfCtrlIdx,
        uint16                                    srcFrameType,
        uint16                                    srcFrameLen,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_VAR)  srcDataPtr);

#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */

#define ETHIF_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* ETHIF_EXTND_TRAFFIC_HNDL_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_ExtndTrafficHndl_Int.h
 *********************************************************************************************************************/

