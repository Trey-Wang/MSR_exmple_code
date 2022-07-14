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
/*! \file     CddDes_Cbk.h
 *  \brief    CddDes callback definitions
 *
 *  \details  Function prototypes of CddDes callback and notification functions.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Alexander Ditte               visade        Vector Informatik GmbH
 *  Sebastian Kobbe               vissko        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  refer to CddDes.h
 *********************************************************************************************************************/

#ifndef CDDDES_CBK_H
# define CDDDES_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* Configuration */
#include "CddDes_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define CDDDES_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  CddDes_TpTxConfirmation()
**********************************************************************************************************************/
/*! \brief        Confirmation of completed transmission.
 *  \details      Implementation of PduR 4 Callout \c CddDes_TpTxConfirmation, called by PduR to confirm the end
 *                of a transmission request giving its final result.
 *  \param[in]    CddDesTxPduId   The transmitted PDU.
 *  \param[in]    Result          The result of the transmission.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different CddDesTxPduId.
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22732
**********************************************************************************************************************/
FUNC(void, CDDDES_CODE) CddDes_TpTxConfirmation(
  PduIdType CddDesTxPduId, 
  Std_ReturnType Result
  );

/**********************************************************************************************************************
 *  CddDes_StartOfReception()
**********************************************************************************************************************/
/*! \brief        Indication of incoming new PDU reception.
 *  \details      Implementation of PduR 4 Callout \c CddDes_StartOfReception, called by PduR to indicate the start
 *                of reception of the indicated Pdu ID.
 *  \param[in]    CddDesRxPduId PduId to be received.
 *  \param[in]    PduInfoPtr    Must be NULL_PTR. Any functionallity associated with not being a NULL_PTR is not supported
 *                              by CddDes.
 *  \param[in]    TpSduLength   The total length of the PDU to be received.
 *  \param[out]   BufferSizePtr The remaining internal buffer size after current function call is provided to the caller.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different CddDesRxPduId.
 *  \synchronous  TRUE
 *  \return       BUFREQ_E_NOT_OK   If a wrong parameter is passed, the PduId is not assigned to an active channel (for cases
 *                                  other than polling the buffer size), or the channel is in a wrong state.
 *  \return       BUFREQ_E_OVFL     If the Pdu is longer than the maximum buffer that could be allocated.
 *  \return       BUFREQ_OK         Successful execution.
 *  \trace        DSGN-CddDes22732
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, CDDDES_CODE) CddDes_StartOfReception(
  PduIdType CddDesRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, CDDDES_APPL_DATA) PduInfoPtr,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, CDDDES_APPL_DATA) BufferSizePtr
  );

/**********************************************************************************************************************
 *  CddDes_CopyRxData()
**********************************************************************************************************************/
/*! \brief        Indication of incoming PDU segment.
 *  \details      Implementation of PduR 4 Callout \c CddDes_CopyRxData, called by PduR to give CddDes a received segment
 *                of the indicated Pdu ID.
 *  \param[in]    CddDesRxPduId   PduId to be received.
 *  \param[in]    PduInfoPtr      If PduInfoPtr->SduLength contains the length of the segment to be received, and
 *                                PduInfoPtr->SduDataPtr contains the segment to be received.
 *  \param[out]   BufferSizePtr   The remaining internal buffer size after current function call is provided to the caller
 *                                through this pointer.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different CddDesRxPduId.
 *  \synchronous  TRUE
 *  \return       BUFREQ_E_NOT_OK   If a wrong parameter is passed, the PduId is not assigned to an active channel, or the
 *                                  channel is in a wrong state.
 *  \return       BUFREQ_OK         Successful execution.
 *  \trace        DSGN-CddDes22732
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, CDDDES_CODE) CddDes_CopyRxData(
  PduIdType CddDesRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, CDDDES_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, CDDDES_APPL_DATA) BufferSizePtr
  );

/**********************************************************************************************************************
 *  CddDes_CopyTxData()
**********************************************************************************************************************/
/*! \brief        Request for transmitted segment.
 *  \details      Implementation of PduR 4 Callout \c CddDes_CopyTxData, called by PduR to get segment of the indicated Pdu ID
 *                in order to transmit it.
 *
 *  \param[in]    CddDesTxPduId    PduId for which Tx Data is being requested.
 *  \param[in,out] PduInfoPtr      PduInfoPtr->SduLength is the length requested to be provided, PduInfoPtr->SduDataPtr
 *                                 is where the data should be written.
 *  \param[in,out] Retry           Not used, present only to match the callout prototype in PduR.
 *  \param[out]   AvailableDataPtr Remaining data to be sent is provided to the caller through this pointer.
 *  \return       BUFREQ_E_NOT_OK  If a wrong parameter is passed, the PduId is not assigned to an active channel,
 *                                 or the channel is in a wrong state.
 *  \return       BUFREQ_OK        Successful execution.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different CddDesTxPduId.
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22732
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, CDDDES_CODE) CddDes_CopyTxData(
  PduIdType CddDesTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, CDDDES_APPL_DATA) PduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, CDDDES_APPL_DATA) Retry,
  P2VAR(PduLengthType, AUTOMATIC, CDDDES_APPL_DATA) AvailableDataPtr
  );


/**********************************************************************************************************************
 *  CddDes_TpRxIndication()
**********************************************************************************************************************/
/*! \brief        Indication of completed reception.
 *  \details      Implementation of PduR 4 Callout \c CddDes_TpRxIndication, called by PduR to indicate the end
 *                of reception of the indicated Pdu ID giving its final result.
 *  \param[in]    CddDesRxPduId The received PDU.
 *  \param[in]    Result        The result of the reception.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different CddDesRxPduId.
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22732
**********************************************************************************************************************/
FUNC(void, CDDDES_CODE) CddDes_TpRxIndication(
  PduIdType CddDesRxPduId, 
  Std_ReturnType Result
  );


  /* == DEM Interface == */

#if (CDDDES_CFG_SUPPORT_DEPENDENT == STD_ON)
/**********************************************************************************************************************
 *  CddDes_DtcStatusChanged()
 *********************************************************************************************************************/
/*! \brief        Callback function used by the Dem to notify a changed DTC Status
 *  \details      Is triggered on changes of the UDS DTC status byte. The trigger will not occur for changed status bits 
                  which are disabled by the DTCStatusAvailabilityMask.
 *  \param[in]    DTC           Diagnostic Trouble Code in UDS format.
 *  \param[in]    DTCStatusOld  DTC status ANDed with DTCStatusAvailabilityMask before change.
 *  \param[in]    DTCStatusNew  DTC status ANDed with DTCStatusAvailabilityMask after change
 *  \return       E_OK          Always returned
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22814
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDES_CODE) CddDes_DtcStatusChanged(
  uint32 DTC, 
  uint8 DTCStatusOld, 
  uint8 DTCStatusNew
  );
#endif

#if (CDDDES_CFG_SUPPORT_MASTER == STD_ON)
/**********************************************************************************************************************
 *  CddDes_ClearDtcNotification()
 *********************************************************************************************************************/
/*! \brief        Called by the Dem when performing a clear DTC operation.
 *  \details      Queues the event in the EventQueue and synchronizes with dependent ECU.
 *  \param[in]    DTC         DTC or group of DTC that is cleared.
 *  \param[in]    DTCFormat   Format of the DTC value.
 *  \param[in]    DTCOrigin   Event memory which is selected by the current clear operation.
 *  \return       E_OK        Always returned
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22814
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDES_CODE) CddDes_ClearDtcNotification(
  uint32 DTC,
  Dem_DTCFormatType DTCFormat,
  Dem_DTCOriginType DTCOrigin
  );
#endif

# define CDDDES_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* CDDEM_CBK_H */

/*********************************************************************************************************************
 *  END OF FILE: CddDes_Cbk.h
 ********************************************************************************************************************/