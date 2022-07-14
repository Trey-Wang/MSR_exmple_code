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
 *         File:  J1939Tp_Bam.h
 *       Module:  J1939 Transport Layer
 *
 *  Description:  Header of the BAM sub-module of the SAE J1939 Transport Layer module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !(defined J1939TP_BAM_H)
# define J1939TP_BAM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "J1939Tp_Cfg.h"

# if (J1939TP_BAM_ENABLED == STD_ON)
/*! \trace SPEC-2167284 */
#  include "ComStack_Types.h"
#  include "J1939Tp_Types.h"


/**********************************************************************************************************************
 *  INTERNAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*============================================== J1939TP_START_SEC_CODE =============================================*/
#  define J1939TP_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

/************************************************************************************************************
 *  J1939Tp_BamTransmit()
 ***********************************************************************************************************/
/*! \brief       Creates BAM frame
 *  \details     Creates the BAM frame according to its structure and is called by J1939Tp_Transmit.
 *  \param[in]   sduIdx     ID of the J1939Tp N-SDU to be transmitted.
 *  \param[in]   info       Pointer to J1939Tp N-SDU structure.
 *  \return      E_NOT_OK - Creation failed, e.g. when the requested transmission would use a channel that is currently active
 *  \return      E_OK     - BAM frame has been created
 *  \pre         The info parameter and its field SduDataPtr must not be NULL.
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      J1939TP_BAM_TX_ENABLED
 *  \trace       SPEC-2167314
 ***********************************************************************************************************/
#  if (J1939TP_BAM_TX_ENABLED == STD_ON)
FUNC(Std_ReturnType, J1939TP_CODE) J1939Tp_BamTransmit(PduIdType sduIdx,
                                                       P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info);
#  endif


/************************************************************************************************************
 *  J1939Tp_BamTxConfirmation()
 ***********************************************************************************************************/
/*! \brief       Confirms BAM transmission
 *  \details     Confirms the successful BAM transmission by the CanIf.
 *               Is called by the J1939Tp_TxConfirmation after successful transmission of a J1939Tp Tx N-PDU.
 *  \param[in]   sduIdx      ID of the J1939Tp N-SDU that has been transmitted.
 *  \param[in]   pduType     Type of PDU    range: J1939TP_TPCM_PDUTYPEOFTXPDU, J1939TP_TPDT_PDUTYPEOFTXPDU
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different sduIdx
 *  \synchronous TRUE
 *  \config      J1939TP_BAM_TX_ENABLED
 ***********************************************************************************************************/
#  if (J1939TP_BAM_TX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_BamTxConfirmation(PduIdType sduIdx, J1939Tp_PduTypeOfTxPduType pduType);
#  endif


/************************************************************************************************************
 *  J1939Tp_BamTxMain()
 ***********************************************************************************************************/
/*! \brief       Supervises BAM transmission timeouts
 *  \details     Used for scheduling purposes and timeout supervision of BAM transmissions.
 *  \param[in]   sduIdx      ID of the J1939Tp N-SDU that has been transmitted.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different sduIdx
 *  \synchronous TRUE
 *  \config      J1939TP_BAM_TX_ENABLED
 *  \note        The main function must be called cyclically with a timing corresponding to the configured main
 *               function period.
 ***********************************************************************************************************/
#  if (J1939TP_BAM_TX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_BamTxMain(PduIdType sduIdx);
#  endif

/************************************************************************************************************
 *  J1939Tp_BamRxIndication()
 ***********************************************************************************************************/
/*! \brief       Indicates BAM reception
 *  \details     Indicates the BAM reception of an N-PDU from the CanIf.
 *  \param[in]   pduIdx      ID of the J1939Tp N-PDU that has been transmitted.
 *  \param[in]   info        Pointer to J1939Tp N-PDU structure.
 *  \param[in]   pduType     Type of PDU    range: J1939TP_TPCM_PDUTYPEOFTXPDU, J1939TP_TPDT_PDUTYPEOFTXPDU
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different pduIdx
 *  \synchronous TRUE
 *  \config      J1939TP_BAM_RX_ENABLED
 *  \trace       SPEC-2167307
 ***********************************************************************************************************/
#  if (J1939TP_BAM_RX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_BamRxIndication(PduIdType pduIdx,
                                                 P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info,
                                                 J1939Tp_PduTypeOfRxPduType pduType, uint8 sa, uint8 da);
#  endif

/************************************************************************************************************
 *  J1939Tp_BamRxMain()
 ***********************************************************************************************************/
/*! \brief       Supervises BAM reception timeouts
 *  \details     Used for scheduling purposes and timeout supervision of BAM reception.
 *  \param[in]   sduIdx      ID of the J1939Tp N-SDU that has been received.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different sduIdx
 *  \synchronous TRUE
 *  \config      J1939TP_BAM_RX_ENABLED
 *  \note        The main function must be called cyclically with a timing corresponding to the configured main
 *               function period.
 ***********************************************************************************************************/
#  if (J1939TP_BAM_RX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_BamRxMain(PduIdType sduIdx);
#  endif

#  define J1939TP_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*============================================== J1939TP_STOP_SEC_CODE ==============================================*/

# endif /* (J1939TP_BAM_ENABLED == STD_ON) */

#endif /* !(defined J1939TP_BAM_H) */

/**********************************************************************************************************************
 *  END OF FILE: J1939Tp_Bam.h
 *********************************************************************************************************************/
