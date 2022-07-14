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
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Scc_Cbk.h
 *        \brief  Smart Charge Communication Header File
 *
 *      \details  Implements Vehicle 2 Grid communication according to the specifications ISO/IEC 15118-2,
 *                DIN SPEC 70121 and customer specific schemas.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the Scc module. >> Scc.h
 *********************************************************************************************************************/ 
#if !defined (SCC_CBK_H)
# define SCC_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Scc_Types.h"
#include "TcpIp_Types.h"
#include "EthIf_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define SCC_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/**********************************************************************************************************************
 *  Scc_Cbk_TL_RxIndication
 *********************************************************************************************************************/
/*! \brief      receive indication
 *  \param[in]  SockHnd               socket handle
 *  \param[in]  PbufPtr               pointer to the received data
 *  \param[in]  SourcePtr             source network address and port
 *  \context    interrupt or task level
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_Cbk_TL_RxIndication(
  Scc_SocketType SockHnd,
  SCC_P2VARAPPLDATA(Scc_SockAddrType) SourcePtr,
  SCC_P2VARAPPLDATA(uint8) DataPtr,
  uint16 DataLen
  );

/**********************************************************************************************************************
 *  Scc_Cbk_TL_TCPConnected
 *********************************************************************************************************************/
/*! \brief      TCP connected
 *  \param[in]  SockHnd            socket handle
 *  \context    interrupt or task level
 **********************************************************************************************************************/
extern SCC_FUNCCODE(void) Scc_Cbk_TL_TCPConnected(TcpIp_SockHndType SockHnd);

/**********************************************************************************************************************
 *  Scc_Cbk_TL_TCPEvent
 *********************************************************************************************************************/
/*! \brief      TCP event handling
 *  \param[in]  SockHnd            socket handle
 *  \param[in]  Event              socket event
 *  \context    interrupt or task level
 **********************************************************************************************************************/
extern SCC_FUNCCODE(void) Scc_Cbk_TL_TCPEvent(Scc_SocketType SockHnd, IpBase_TcpIpEventType Event);

/**********************************************************************************************************************
 *  Scc_Cbk_TLS_CertChain
 *********************************************************************************************************************/
/*! \brief          TLS validation result for a server certificate chain
 *  \param[in]      SockHnd                socket handle
 *  \param[in,out]  validationResultPtr    Validation result (VALIDATION_OK, VALIDATION_UNKNOWN_CA) 
 *  \param[in]      certChainPtr           pointer to start of the certificate chain 
 *  \param[in]      certChainLen           length of the certificate chain
 *  \context        interrupt or task level
 **********************************************************************************************************************/
extern SCC_FUNCCODE(void) Scc_Cbk_TLS_CertChain(
  Scc_SocketType              SockHnd,
  SCC_P2VARAPPLDATA(uint8)    validationResultPtr,
  SCC_P2CONSTAPPLDATA(uint8)  certChainPtr,
  uint32                      certChainLen );

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
/**********************************************************************************************************************
 *  Scc_Cbk_SLAC_FirmwareDownloadComplete
 *********************************************************************************************************************/
/*! \brief      callback that is called after the firmware download was processed and the firmware was started
 *  \context    interrupt or task level
 *********************************************************************************************************************/
extern SCC_FUNCCODE(void) Scc_Cbk_SLAC_FirmwareDownloadComplete(boolean DownloadSuccessful);
#endif /* SCC_ENABLE_SLAC_HANDLING */

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
/**********************************************************************************************************************
 *  Scc_Cbk_SLAC_AssociationStatus
 *********************************************************************************************************************/
/*! \brief      callback function for ongoing association errors and status information
 *  \param[in]  AssociationStatus      the association status reported to the application
 *  \context    interrupt or task level
 *********************************************************************************************************************/
extern SCC_FUNCCODE(void) Scc_Cbk_SLAC_AssociationStatus(uint8 AssociationStatus);
#endif /* SCC_ENABLE_SLAC_HANDLING */

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
/**********************************************************************************************************************
 *  Scc_Cbk_SLAC_DLinkReady
 *********************************************************************************************************************/
/*! \brief      D-LINK READY indication informs higher layers about a change of PLC link status
 *  \param[in]  DLinkReady      the power line link state after SLAC was finished
 *  \param[in]  NMKPtr          the network membership key (NMK) that was established during the SLAC session
 *  \param[in]  NIDPtr          the network identifier (NID) that was established during the SLAC session
 *  \context    interrupt or task level
 *********************************************************************************************************************/
extern SCC_FUNCCODE(void) Scc_Cbk_SLAC_DLinkReady(EthTrcv_LinkStateType DLinkReady,
  SCC_P2VARAPPLDATA(uint8) NMKPtr, SCC_P2VARAPPLDATA(uint8) NIDPtr);
#endif /* SCC_ENABLE_SLAC_HANDLING */

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
/**********************************************************************************************************************
 *  Scc_Cbk_SLAC_GetRandomizedDataBuffer
 *********************************************************************************************************************/
/*! \brief      the callback is used by the SLAC component to gain a random byte array
 *  \param[out] RandomDataPtr      the buffer that must be randomized
 *  \param[in]  RandomDataLen      the length of the buffer
 *  \context    interrupt or task level
 *********************************************************************************************************************/
extern SCC_FUNCCODE(void) Scc_Cbk_SLAC_GetRandomizedDataBuffer(SCC_P2VARAPPLDATA(uint8) RandomDataPtr, uint16 RandomDataLen);
#endif /* SCC_ENABLE_SLAC_HANDLING */

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
/**********************************************************************************************************************
 *  Scc_Cbk_SLAC_GetValidateToggles
 *********************************************************************************************************************/
/*! \brief      callback function to generate a random number of validation toggles
 *  \return     [1,2,3]      random number of validation toggles
 *  \context    interrupt or task level
 *********************************************************************************************************************/
extern SCC_FUNCCODE(uint8) Scc_Cbk_SLAC_GetValidateToggles(void);
#endif /* SCC_ENABLE_SLAC_HANDLING */

#if ( defined SCC_ENABLE_SLAC_HANDLING ) && ( SCC_ENABLE_SLAC_HANDLING == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
/**********************************************************************************************************************
 *  Scc_Cbk_SLAC_ToggleRequest
 *********************************************************************************************************************/
/*! \brief      callback to request the toggle process during the validation phase
 *  \param[in]  ToggleNum      number of BCB-Toggles the application must generate on the Control Pilot
 *  \context    interrupt or task level
 *********************************************************************************************************************/
extern SCC_FUNCCODE(void) Scc_Cbk_SLAC_ToggleRequest(uint8 ToggleNum);
#endif /* SCC_ENABLE_SLAC_HANDLING */

/**********************************************************************************************************************
 *  Scc_Cbk_Eth_TransceiverLinkStateChange
 **********************************************************************************************************************/
/*! \brief        Called by lower layer (e.g. EthIf) to indicate a change of the transceiver link state
 *  \param[in]    CtrlIdx         index of the ethernet controller
 *  \param[in]    TrcvLinkState   new state of the tranceiver
 **********************************************************************************************************************/
extern SCC_FUNCCODE(void) Scc_Cbk_Eth_TransceiverLinkStateChange(
  uint8 CtrlIdx,
  EthTrcv_LinkStateType TrcvLinkState
  );

/**********************************************************************************************************************
 *  Scc_Cbk_IP_AddressAssignmentChange
 *********************************************************************************************************************/
/*! \brief      IP address assignment change callback
 *  \param[in]  LocalAddrId           ID of the local address
 *  \param[in]  State                 state of the local address
 *  \context    interrupt or task level
 *********************************************************************************************************************/
extern SCC_FUNCCODE(void) Scc_Cbk_IP_AddressAssignmentChange(TcpIp_LocalAddrIdType LocalAddrId,
  TcpIp_IpAddrStateType State);

#define SCC_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

#endif  /* SCC_CBK_H */
/**********************************************************************************************************************
 *  END OF FILE: Scc_Cbk.h
 *********************************************************************************************************************/
