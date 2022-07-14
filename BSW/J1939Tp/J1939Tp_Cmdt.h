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
 *         File:  J1939Tp_Cmdt.h
 *       Module:  J1939 Transport Layer
 *
 *  Description:  Header of the CMDT sub-module of the SAE J1939 Transport Layer module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !(defined J1939TP_CMDT_H)
# define J1939TP_CMDT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "J1939Tp.h"

# if (J1939TP_CMDT_ENABLED == STD_ON)
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
 *  J1939Tp_CmdtTransmit()
 ***********************************************************************************************************/
/*! \brief       Initiates transmission of a CMDT N-SDU.
 *  \details     Creates the Cmdt frame according to its structure and is called by J1939Tp_Transmit.
 *  \param[in]   sduIdx     ID of the J1939Tp N-SDU to be transmitted.
 *  \param[in]   info       Pointer to J1939Tp N-SDU structure.
 *  \return      E_NOT_OK - Creation failed, e.g. when the requested transmission would use a channel that is currently active
 *  \return      E_OK     - Cmdt frame has been created
 *  \pre         The info parameter and its field SduDataPtr must not be NULL.
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      J1939TP_CMDT_TX_ENABLED
 ***********************************************************************************************************/
#  if (J1939TP_CMDT_TX_ENABLED == STD_ON)
FUNC(Std_ReturnType, J1939TP_CODE) J1939Tp_CmdtTransmit(PduIdType sduIdx,
                                                        P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info);
#  endif

/************************************************************************************************************
 *  J1939Tp_CmdtTxMain()
 ***********************************************************************************************************/
/*! \brief       Supervises Cmdt transmission timeouts
 *  \details     Used for scheduling purposes and timeout supervision of Cmdt transmissions.
 *  \param[in]   sduIdx      ID of the J1939Tp N-SDU that has been transmitted.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different sduIdx
 *  \synchronous TRUE
 *  \config      J1939TP_CMDT_TX_ENABLED
 *  \note        The main function must be called cyclically with a timing corresponding to the configured main
 *               function period.
 ***********************************************************************************************************/
#  if (J1939TP_CMDT_TX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_CmdtTxMain(PduIdType sduIdx);
#  endif

/************************************************************************************************************
 *  J1939Tp_CmdtRxMain()
 ***********************************************************************************************************/
/*! \brief       Supervises Cmdt reception timeouts
 *  \details     Used for scheduling purposes and timeout supervision of Cmdt reception.
 *  \param[in]   sduIdx      ID of the J1939Tp N-SDU that has been received.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different sduIdx
 *  \synchronous TRUE
 *  \config      J1939TP_CMDT_RX_ENABLED
 *  \note        The main function must be called cyclically with a timing corresponding to the configured main
 *               function period.
 ***********************************************************************************************************/
#  if (J1939TP_CMDT_RX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_CmdtRxMain(PduIdType sduIdx);
#  endif

/************************************************************************************************************
 *  J1939Tp_CmdtTxConfirmation()
 ***********************************************************************************************************/
/*! \brief       Confirms Cmdt transmission
 *  \details     Confirms the successful Cmdt transmission by the CanIf.
 *               Is called by the J1939Tp_TxConfirmation after successful transmission of a J1939Tp Tx N-PDU.
 *  \param[in]   sduIdx      ID of the J1939Tp N-SDU that has been transmitted.
 *  \param[in]   pduType     Type of PDU    range: J1939TP_TPCM_PDUTYPEOFTXPDU, J1939TP_TPDT_PDUTYPEOFTXPDU
 *  \param[in]   sduType     Type of SDU    range: J1939TP_NSDU_CMDT_TX, J1939TP_NSDU_CMDT_RX
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different sduIdx
 *  \synchronous TRUE
 *  \config      J1939TP_CMDT_TX_ENABLED
 ***********************************************************************************************************/
FUNC(void, J1939TP_CODE) J1939Tp_CmdtTxConfirmation(PduIdType sduIdx, J1939Tp_PduTypeOfTxPduType pduType,
                                                    J1939Tp_NSduType sduType);

/************************************************************************************************************
 *  J1939Tp_CmdtRxIndication()
 ***********************************************************************************************************/
/*! \brief       Indicates Cmdt reception
 *  \details     Indicates the Cmdt reception of an N-PDU from the CanIf.
 *  \param[in]   pduIdx      ID of the J1939Tp N-PDU that has been transmitted.
 *  \param[in]   info        Pointer to J1939Tp N-PDU structure.
 *  \param[in]   pduType     Type of PDU    range: J1939TP_TPCM_PDUTYPEOFTXPDU, J1939TP_TPDT_PDUTYPEOFTXPDU
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different pduIdx
 *  \synchronous TRUE
 *  \config      J1939TP_CMDT_RX_ENABLED
 *  \trace       SPEC-2167307
 ***********************************************************************************************************/
FUNC(void, J1939TP_CODE) J1939Tp_CmdtRxIndication(PduIdType pduIdx,
                                                  P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info,
                                                  J1939Tp_PduTypeOfRxPduType pduType, uint8 sa, uint8 da);

#  define J1939TP_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*============================================== J1939TP_STOP_SEC_CODE ==============================================*/

# endif /* (J1939TP_CMDT_ENABLED == STD_ON) */

#endif /* !(defined J1939TP_CMDT_H) */

/**********************************************************************************************************************
 *  END OF FILE: J1939Tp_Cmdt.h
 *********************************************************************************************************************/
