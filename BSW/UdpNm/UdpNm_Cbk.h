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
 *         File:  UdpNm_Cbk.h
 *      Project:  MICROSAR ETH
 *       Module:  UDP Network Management
 *    Generator:  Nm_AsrNmUdp.jar
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

#ifndef UDPNM_CBK_H /* PRQA S 0883 1 */ /* MD_MSR_19.15 */
#define UDPNM_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComStack_Types.h"
#include "UdpNm_Cfg.h"
#include "UdpNm_Types.h"

/* Turn off PC-lint warnings issued -> issues known and coverd by code review */
/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define UDPNM_START_SEC_CODE
#include "MemMap.h"

/**********************************************************************************************************************
 *  UdpNm_SoAdIfTxConfirmation / UdpNm_SoAdTxConfirmation
 *********************************************************************************************************************/
/*! \brief      This service confirms a previous successfully processed transmit request.
 *  \param[in]  UdpNmTxPduId         Identification of the network through PDU-ID
 *  \context    Interrupt or task level
 *********************************************************************************************************************/
#if ( STD_OFF == UDPNM_PASSIVE_MODE_ENABLED )
FUNC(void, UDPNM_CODE) UdpNm_SoAdIfTxConfirmation(PduIdType UdpNmTxPduId);
#endif /* UDPNM_PASSIVE_MODE_ENABLED */

/**********************************************************************************************************************
 *  UdpNm_SoAdIfRxIndication
 *********************************************************************************************************************/
/*! \brief      This service indicates a successful reception of a received NM message to the UdpNm after passing all
 *              filters and validation checks.
 *  \param[in]  RxPduId         Identification of the network through PDU-ID
 *  \param[in]  PduInfoPtr      Pointer to the received SDU - Only AUTOSAR 4.0 API
 *  \context    Interrupt or task level
 *********************************************************************************************************************/
FUNC(void, UDPNM_CODE) UdpNm_SoAdIfRxIndication(PduIdType RxPduId,
                                                P2CONST(PduInfoType, AUTOMATIC, UDPNM_APPL_DATA) PduInfoPtr);

#define UDPNM_STOP_SEC_CODE
#include "MemMap.h"

#endif /* !defined(UDPNM_CBK_H) */

/**********************************************************************************************************************
 *  END OF FILE: UdpNm_Cbk.h
 *********************************************************************************************************************/
