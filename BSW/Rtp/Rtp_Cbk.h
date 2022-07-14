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
 *         File:  Rtp_Cbk.h
 *       Module:  Tp_Rtp
 *    Generator:  Tp_Rtp.jar (DaVinci Configurator Pro)
 *
 *  Description:  Callback header of Realtime Transport Protocol (RTP)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Philipp Christmann            vispcn        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2016-08-10  vispcn  FEATC-253     FEAT-1879:  [Beta] Development of RTP
 *********************************************************************************************************************/

#if !defined(RTP_CBK_H)
#define RTP_CBK_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Rtp_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define RTP_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  Rtp_SoAdRxIndication()
 *********************************************************************************************************************/
/*! \brief       Indication of a received I-PDU from the SoAd lower layer communication interface.
 *  \details     API is used for reception of RTP and RTCP packets.
 *  \param[in]   RxPduId        ID of the received I-PDU.
 *  \param[in]   PduInfoPtr     Contains the length (SduLength) of the received I-PDU and a pointer to
 *                              a buffer (SduDataPtr) containing the I-PDU.
 *  \return      void
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   TRUE for different streams
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, RTP_CODE) Rtp_SoAdRxIndication(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  Rtp_SoAdTriggerTransmit()
 *********************************************************************************************************************/
/*! \brief       Trigger RTP to copy its payload in the provided buffer.
 *  \details     Function is called by SoAd module if ETH buffer is successfully requested and ready to copy data to.
 *  \param[in]   TxPduId        ID of the I-PDU to transmit.
 *  \param[in]   PduInfoPtr     Contains the length (SduLength) of the received I-PDU and a pointer to
 *                              a buffer (SduDataPtr) containing the I-PDU.
 *  \return      E_OK           Data was successfully copied to the buffer.
 *               E_NOT_OK       Failed to copy data.
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   TRUE for different streams
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, RTP_CODE) Rtp_SoAdTriggerTransmit(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, RTP_APPL_DATA) PduInfoPtr);

#define RTP_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* RTP_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: Rtp_Cbk.h
 *********************************************************************************************************************/
