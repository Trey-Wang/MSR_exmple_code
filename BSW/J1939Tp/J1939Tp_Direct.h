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
 *         File:  J1939Tp_Direct.h
 *       Module:  J1939 Transport Layer
 *
 *  Description:  Header of the direct sub-module of the SAE J1939 Transport Layer module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !(defined J1939TP_DIRECT_H)
# define J1939TP_DIRECT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "J1939Tp.h"

# if (J1939TP_DIRECT_ENABLED == STD_ON)
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

/**********************************************************************************************************************
 *  J1939Tp_DirectTransmit()
 *********************************************************************************************************************/
/*! \brief       Creates Direct frame
 *  \details     Creates the Direct frame according to its structure and is called by J1939Tp_Transmit.
 *  \param[in]   sduIdx     ID of the J1939Tp N-SDU to be transmitted.
 *  \param[in]   info       Pointer to J1939Tp N-SDU structure.
 *  \return      E_NOT_OK - Creation failed, e.g. when the requested transmission would use a channel that is currently active
 *  \return      E_OK     - Direct frame has been created
 *  \pre         The info parameter and its field SduDataPtr must not be NULL.
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      J1939TP_DIRECT_TX_ENABLED
 *********************************************************************************************************************/
#  if (J1939TP_DIRECT_TX_ENABLED == STD_ON)
FUNC(Std_ReturnType, J1939TP_CODE) J1939Tp_DirectTransmit(PduIdType sduIdx,
                                                          P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info);
#  endif

/************************************************************************************************************
 *  J1939Tp_DirectTxMain()
 ***********************************************************************************************************/
/*! \brief       Supervises Direct transmission timeouts
 *  \details     Used for scheduling purposes and timeout supervision of Direct transmissions.
 *  \param[in]   sduIdx      ID of the J1939Tp N-SDU that has been transmitted.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different sduIdx
 *  \synchronous TRUE
 *  \config      J1939TP_DIRECT_TX_ENABLED
 *  \note        The main function must be called cyclically with a timing corresponding to the configured main
 *               function period.
 ***********************************************************************************************************/
#  if (J1939TP_DIRECT_TX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_DirectTxMain(PduIdType sduIdx);
#  endif

/************************************************************************************************************
 *  J1939Tp_DirectTxConfirmation()
 ***********************************************************************************************************/
/*! \brief       Confirms Direct transmission
 *  \details     Confirms the successful Direct transmission by the CanIf.
 *               Is called by the J1939Tp_TxConfirmation after successful transmission of a J1939Tp Tx N-PDU.
 *  \param[in]   sduIdx      ID of the J1939Tp N-SDU that has been transmitted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different sduIdx
 *  \synchronous TRUE
 *  \config      J1939TP_DIRECT_TX_ENABLED
 ***********************************************************************************************************/
#  if (J1939TP_DIRECT_TX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_DirectTxConfirmation(PduIdType sduIdx);
#  endif

/**********************************************************************************************************************
 *  J1939Tp_DirectRxIndication()
 *********************************************************************************************************************/
/*! \brief       Indicates Direct reception
 *  \details     Indicates the Direct reception of an N-PDU from the CanIf.
 *  \param[in]   pduIdx      ID of the J1939Tp N-PDU that has been transmitted.
 *  \param[in]   info        Pointer to J1939Tp N-PDU structure.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different pduIdx
 *  \synchronous TRUE
 *  \config      J1939TP_DIRECT_RX_ENABLED
 *********************************************************************************************************************/
#  if (J1939TP_DIRECT_RX_ENABLED == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_DirectRxIndication(PduIdType pduIdx,
                                                    P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) info, uint8 sa);
#  endif

#  define J1939TP_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*============================================== J1939TP_STOP_SEC_CODE ==============================================*/

# endif /* (J1939TP_DIRECT_ENABLED == STD_ON) */

#endif /* !(defined J1939TP_DIRECT_H) */

/**********************************************************************************************************************
 *  END OF FILE: J1939Tp_DIRECT.h
 *********************************************************************************************************************/
