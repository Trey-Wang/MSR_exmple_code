/***********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  vIpc_Cbk.h
 *        \brief  vIpc callback header file
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 **********************************************************************************************************************/

#ifndef VIPC_CBK_H
# define VIPC_CBK_H

/* Todo: This file must be generated! */

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "vIpc_Types.h"

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#define VIPC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  vIpc_RxIndication()
 *********************************************************************************************************************/
/*! \brief       Callback that informs vIpc that the receive sequence is finished.
 *  \details     -
 *  \param[in]   RxPduId                 Id of the PDU that is to be finished.
 *  \param[in]   Result                  Notify whether receive process was succesful or failed.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        -
 *  \trace       -
 *  \trace       -
 *  \ingroup     Rx
 *  \trace DSGN-vIpc24041
 *********************************************************************************************************************/
extern FUNC(void, VIPC_CODE) vIpc_RxIndication(PduIdType RxPduId,
                                               Std_ReturnType Result);

/**********************************************************************************************************************
 * vIpc_CopyRxData()
 *********************************************************************************************************************/
/*! \brief       Copy one or multiple sub-segments during a receive sequence.
 *  \details     -
 *  \return      BUFREQ_OK        Copying was successful.
 *  \return      BUFREQ_E_NOT_OK  Copying failed.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         -
 *  \ingroup     Rx
 *  \trace DSGN-vIpc24041
 *********************************************************************************************************************/
extern FUNC(BufReq_ReturnType, VIPC_CODE) vIpc_CopyRxData(PduIdType RxPduId,
                                                          P2VAR(PduInfoType, AUTOMATIC, VIPC_APPL_VAR) PduInfoPtr,
                                                          P2VAR(PduLengthType, AUTOMATIC, VIPC_APPL_VAR) BufferSizePtr);

/**********************************************************************************************************************
 * vIpc_StartOfReception()
 *********************************************************************************************************************/
/*! \brief       Notifies the start of a receive sequence.
 *  \details     Notifies the start of a receive sequence to the upper layer and allocates required buffers.
 *               Note that the lower layer must provide the first segment in the pdu info ptr.
 *  \return      BUFREQ_OK        Buffer allocation successful.
 *  \return      BUFREQ_E_NOT_OK  Buffer allocation failed.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         -
 *  \ingroup     Rx
 *  \trace DSGN-vIpc24041
 *********************************************************************************************************************/
extern FUNC(BufReq_ReturnType, VIPC_CODE) vIpc_StartOfReception(PduIdType RxPduId,
                                                                P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr,
                                                                PduLengthType TpSduLength,
                                                                P2VAR(PduLengthType, AUTOMATIC, VIPC_APPL_VAR) BufferSizePtr);

/**********************************************************************************************************************
 *  vIpc_TxConfirmation()
 *********************************************************************************************************************/
/*! \brief       Callback that informs vIpc about the result of a previous transmission.
 *  \details     -
 *  \param[in]   TxPduId                 Id of the PDU for which a confirmation is given.
 *  \param[in]   Result                  Result of the transmission for given PDU.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        -
 *  \trace       -
 *  \trace       -
 *  \ingroup     Tx
 *  \trace DSGN-vIpc23963
 *********************************************************************************************************************/
extern FUNC(void, VIPC_CODE) vIpc_TxConfirmation(PduIdType TxPduId, Std_ReturnType Result);

/**********************************************************************************************************************
 * vIpc_CopyTxData()
 *********************************************************************************************************************/
/*! \brief       Copy one segment to the lower layer.
 *  \details     Copy one segment to the lower layer.
 *  \return      BUFREQ_OK        Copying successful.
 *  \return      BUFREQ_E_NOT_OK  Copying failed.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         -
 *  \ingroup     Tx
 *  \trace DSGN-vIpc23963
 *********************************************************************************************************************/
extern FUNC(BufReq_ReturnType, VIPC_CODE) vIpc_CopyTxData(PduIdType TxPduId,
                                                          P2VAR(PduInfoType, AUTOMATIC, VIPC_APPL_VAR) PduInfoPtr,
                                                          P2VAR(RetryInfoType, AUTOMATIC, VIPC_APPL_VAR) RetryPtr,
                                                          P2VAR(PduLengthType, AUTOMATIC, VIPC_APPL_VAR) AvailableDataPtr);

#define VIPC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* VIPC_CBK_H */

/***********************************************************************************************************************
 *  END OF FILE: vIpc_Cbk.h
 **********************************************************************************************************************/
