/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 Vector Informatik GmbH.                                                     All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  J1939Nm_Cbk.h
 *       Module:  SAE J1939 Network Management (J1939Nm)
 *
 *  Description:  Callback header of the SAE J1939 Network Management module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (J1939NM_CBK_H)
# define J1939NM_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "J1939Nm_Cfg.h"
# include "J1939Nm_Lcfg.h"
# include "J1939Nm_PBcfg.h"


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*============================================== J1939NM_START_SEC_CODE =============================================*/
# define J1939NM_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
 *  J1939Nm_RxIndication
 *********************************************************************************************************************/
/*! \brief      Indicates the reception of an AddressClaimed NM-PDU from the CanIf.
 *
 *  \param      RxPduId         ID of the received NM-PDU.
 *  \param      info            Contains the length (SduLength) of the received NM-PDU and a pointer to a buffer
 *                              (SduDataPtr) containing the NM-PDU and MetaData.
 *  \return     none
 *  \pre        J1939Nm_RxIndication is not currently executed with the same id.
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_RxIndication(PduIdType RxPduId,
                                              P2CONST(PduInfoType, AUTOMATIC, J1939NM_APPL_DATA) info);

/**********************************************************************************************************************
 *  J1939Nm_TxConfirmation
 *********************************************************************************************************************/
/*! \brief      Confirms the successful transmission of an AddressClaimed NM-PDU by the CanIf.
 *
 *  \param      TxPduId         ID of the NM-PDU that has been transmitted.
 *  \return     none
 *  \pre        J1939Nm_TxConfirmation is not currently executed with the same TxPduId.
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_TxConfirmation(PduIdType TxPduId);

/**********************************************************************************************************************
 *  J1939Nm_RequestIndication
 *********************************************************************************************************************/
/*! \brief      Indicates reception of a Request PG.
 *
 *  \param      node            Node by which the request was received.
 *  \param      channel         Channel on which the request was received.
 *  \param      requestedPgn    PGN of the requested PG.
    \param      extIdInfo       Extended identifier bytes.
 *  \param      sourceAddress   Address of the node that sent the Request PG.
 *  \param      destAddress     Address of this node or 0xFF for broadcast.
 *  \param      priority        Priority of the Request PG.
 *  \return     none
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_RequestIndication(uint8 node, NetworkHandleType channel, uint32 requestedPgn,
                                                   P2CONST(J1939Rm_ExtIdInfoType, AUTOMATIC, J1939NM_APPL_DATA)
                                                   extIdInfo, uint8 sourceAddress, uint8 destAddress, uint8 priority);

# if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) && (J1939NM_CAPDU == STD_ON))
/**********************************************************************************************************************
 *  J1939Nm_StartOfReception
 *********************************************************************************************************************/
/*! \brief      This function is called at the start of receiving a CommandedAddress N-SDU from J1939Tp.
 *
 *  \param      CaPduId         Identification of the N-SDU.
 *  \param      info            Pointer to a PduInfoType structure containing the MetaData and MetaDataLength.
 *  \param      TpSduLength     Total length of the N-SDU to be received.
 *  \param      bufferSizePtr   Available receive buffer in the receiving module. This parameter will be used to
 *                              compute the block size in the transport protocol module.
 *  \return     BUFREQ_OK:      Connection has been accepted. bufferSizePtr indicates the available receive buffer;
 *                              reception is continued. If no buffer of the requested size is available, a receive
 *                              buffer size of 0 shall be indicated by bufferSizePtr.
 *              BUFREQ_E_NOT_OK: Connection has been rejected; reception is aborted. bufferSizePtr remains unchanged.
 *              BUFREQ_E_OVFL:  No buffer of the required length can be provided; reception is aborted. bufferSizePtr
 *                              remains unchanged.
 *  \pre        No N-SDU reception is currently active with the same id.
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, J1939NM_CODE) J1939Nm_StartOfReception(PduIdType CaPduId,
                                                               P2VAR(PduInfoType, AUTOMATIC, J1939NM_APPL_VAR) info,
                                                               PduLengthType TpSduLength,
                                                               P2VAR(PduLengthType, AUTOMATIC,
                                                                     J1939NM_APPL_VAR) bufferSizePtr);

/**********************************************************************************************************************
 *  J1939Nm_CopyRxData
 *********************************************************************************************************************/
/*! \brief      This function is called to provide the received data of an N-SDU segment (N-PDU) to the upper layer.
 *              Each call to this function provides the next part of the N-SDU data. The size of the remaining data is
 *              written to the position indicated by bufferSizePtr.
 *
 *  \param      CaPduId         Identification of the received N-SDU.
 *  \param      info            Provides the source buffer (SduDataPtr) and the number of bytes to be copied
 *                              (SduLength). An SduLength of 0 indicates a query for the current amount of available
 *                              buffer in the upper layer module. In this case, the SduDataPtr may be a NULL_PTR.
 *  \param      bufferSizePtr   Available receive buffer after data has been copied.
 *  \return     BUFREQ_OK:      Data copied successfully.
 *              BUFREQ_E_NOT_OK: Data was not copied because an error occurred.
 *  \pre        The reception of the N-SDU with this id was previously accepted using J1939Nm_StartOfReception.
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, J1939NM_CODE) J1939Nm_CopyRxData(PduIdType CaPduId,
                                                         P2VAR(PduInfoType, AUTOMATIC, J1939NM_APPL_VAR) info,
                                                         P2VAR(PduLengthType, AUTOMATIC,
                                                               J1939NM_APPL_VAR) availableDataPtr);

/**********************************************************************************************************************
 *  J1939Nm_TpRxIndication
 *********************************************************************************************************************/
/*! \brief      Called after a CommandedAddress N-SDU has been received via the TP API, the result indicates whether
 *              the reception was successful or not.
 *
 *  \param      CaPduId         Identification of the received N-SDU.
 *  \param      result          Result of the reception.
 *  \return     none
 *  \pre        The reception of the N-SDU with this id was previously accepted using J1939Nm_StartOfReception.
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_TpRxIndication(PduIdType CaPduId, Std_ReturnType result);

/**********************************************************************************************************************
 *  Disable unused callbacks of PduR, which are always present because the PduR cannot switch the Rx path separately.
 *********************************************************************************************************************/
#  define J1939Nm_CopyTxData NULL_PTR
#  define J1939Nm_TpTxConfirmation NULL_PTR
# endif

/**********************************************************************************************************************
 *  J1939Nm_GetBusOffDelay
 *********************************************************************************************************************/
/*! \brief      Called when a bus-off was detected by the CanSM, returns the number of CanSM main cycles to delay the
 *              recovery.
 *
 *  \param      channel         ComM network ID of the affected channel.
 *  \param      delayCyclesPtr  Pointer to the location where the number of delay cycles shall be stored.
 *  \return     none
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_GetBusOffDelay(NetworkHandleType channel,
                                                P2VAR(uint8, AUTOMATIC, J1939NM_APPL_VAR) delayCyclesPtr);

/**********************************************************************************************************************
 *  J1939Nm_BusOffEnd
 *********************************************************************************************************************/
/*! \brief      Called by CanSM after bus-off recovery succeeded.
 *
 *  \param      channel         ComM network ID of the affected channel.
 *  \return     none
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_BusOffEnd(NetworkHandleType channel);

# define J1939NM_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*============================================== J1939NM_STOP_SEC_CODE ==============================================*/

#endif /* J1939NM_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: J1939Nm_Cbk.h
 *********************************************************************************************************************/
