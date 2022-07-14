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
 *         File:  J1939Tp.h
 *       Module:  J1939 Transport Layer
 *
 *  Description:  Main header of the SAE J1939 Transport Layer module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Martin Schlodder              vismsr        Vector Informatik GmbH
 *  Thomas Albrecht               vistat        Vector Informatik GmbH
 *  Matthias Mueller              vismmu        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version  Date        Author  Change Id      Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  0.01.00  2013-10-28  vismsr   -             First Version.
 *  0.01.01  2013-08-27  vistat   -             Adapted to Vector template and performed MISRA analysis.
 *                       vismsr   -             Fixed use case without transmission.
 *  0.01.02  2013-11-17  vismsr  ESCAN00071938  Fixed handling of SDUs without MetaData.
 *  0.01.03  2013-11-27  vismsr  ESCAN00072228  Fixed Rx only use case.
 *                               ESCAN00072231  Removed immediate initialization of local structs.
 *  0.02.00  2014-03-13  vismsr  ESCAN00072481  Fixed linker error in pure BAM transmission szenario.
 *                               ESCAN00072814  Fixed compiler error because of undefined NULL.
 *                               ESCAN00073275  Fixed destination address of transmitted direct frame.
 *                               ESCAN00073360  Added timeout supervision of received messages.
 *                               ESCAN00073429  Introduced strict sequence check for transmission.
 *                               ESCAN00073921  Added check for running connections.
 *                                -             Implemented module state handling and DET reporting.
 *                                -             Introduced abort reasons.
 *                                -             Accept new RTS immediately.
 *                                -             MISRA analysis performed.
 *  0.02.01  2014-03-17  vismsr  ESCAN00074198  Fixed allocation of pure BAM channels.
 *  0.02.02  2014-03-20  vismsr   -             Fixed version.
 *  0.03.00  2014-06-16  vismsr  ESCAN00071926  Fixed parameters of StartOfReception, RxIndication, and TxConfirmation.
 *                               ESCAN00074033  Removed superfluous initialization.
 *                               ESCAN00074034  Fixed assignment to PduLengthType variables.
 *                               ESCAN00074131  Support for post-build configuration.
 *                               ESCAN00074579  Different combinations of BAM and CMDT support.
 *                               ESCAN00074580  Support for configurations without direct PDUs.
 *                               ESCAN00074806  Fixed handling of new BAM and CMDT Rx connections.
 *                               ESCAN00075574  Added source identification.
 *                               ESCAN00075907  Configured channel parameters are now binding for Tx and Rx.
 *                                -             Added STATE_CMDT_FIRSTDATA, removed Timer from TxNSduStateType.
 *                                -             Added proprietary DET errors.
 *                                -             Fixed DET reporting.
 *  1.00.00  2014-10-29  vismsr  ESCAN00076572  Fixed lookup of channel index in RxIndication.
 *                               ESCAN00076996  Added support for ETP.
 *                               ESCAN00077125  Fixed block calculation for transmission.
 *                               ESCAN00077182  Increased type of SDU size to avoid wrap-around.
 *                               ESCAN00077329  Removed local variable only used for DET.
 *                               ESCAN00077424  Fixed inclusion of Det.h when DET is disabled.
 *                               ESCAN00077435  Fixed DA for direct transmission on BAM channels.
 *                               ESCAN00077611  Added DET checks for TP message sizes (fixed and variable).
 *                               ESCAN00077627  Fixed check for correct DA of direct PDU1 frames.
 *                               ESCAN00077630  Fixed DA in case of direct reception of PDU2 frames.
 *                               ESCAN00077649  Check added for already bound connection.
 *                               ESCAN00077656  Saving connection parameters for abort after TP.CM_RTS.
 *                               ESCAN00077690  Added check for occupied N-SDU in case of direct transmission.
 *                               ESCAN00077811  Retry CopyTxData after BUFREQ_E_BUSY until timeout Tr.
 *                               ESCAN00077955  Retry transmission until timeout Tr using additional states.
 *                               ESCAN00078341  Handling of TP.CM_CTS(WAIT).
 *                               ESCAN00078529  Fixed handling of J1939Tp_Init parameter, added invalid handle checks.
 *                               ESCAN00078542  Fixed MNOP of TP.CM.
 *                               ESCAN00078925  Set BAM receiver to BC address, check for valid CMDT states.
 *                                -             Fixed information passed to StartOfReception.
 *                                -             Changed to access Tx N-PDU directly, fixed timeout handling.
 *                                -             Fixed configuration based code encapsulation.
 *                                -             Reworked headers.
 *                                -             Added version check.
 *  1.00.01  2014-12-02  vismsr  ESCAN00079406  Fixed reception of direct PDU1 messages with DA 0xFF.
 *                               ESCAN00079474  Fixed SDU size assertions.
 *                               ESCAN00079885  Fixed effect of configured DA on direct messages.
 *                                -             Reworked API descriptions.
 *  1.01.00  2015-01-30  vismsr  ESCAN00076996  Implemented ETP.
 *                               ESCAN00080198  Call RxIndication only after StartOfReception succeeded.
 *                               ESCAN00080348  N-PDU length check not needed if done in CanIf.
 *                               ESCAN00080367  Check maximum message size independent of DET.
 *  1.02.00  2015-07-23  vismsr  ESCAN00080221  Accept direct frames with less than 8 bytes.
 *  1.02.01  2015-09-04  vismsr  ESCAN00084262  Adapted to fixed MDL of direct Rx N-PDUs.
 *  1.02.02  2015-10-07  vismsr  ESCAN00081649  Changed CMDT/ETP reception to check available buffer.
 *                               ESCAN00082664  Avoid DET error after reception of unrelated (E)TP.RTS.
 *                               ESCAN00083144  Fixed detection of Tx SDUs affected by (E)TP.ConnAbort.
 *  1.02.03  2016-07-26  vismsr  ESCAN00085062  Ensured data and state consistency of direct and TP messages.
 *                               ESCAN00085614  Added wait frame transmission in CMDT and ETP.
 *                               ESCAN00085798  Fixed search for conflicting BAM receptions.
 *                               ESCAN00085811  Avoid transmission of TP.ConnAbort before TP.CM_RTS.
 *                               ESCAN00088608  Separated check for illegal address from check for broadcast address.
 *                               ESCAN00090588  Introduced error code for wrong addressing in TP.CM frames.
 *                               ESCAN00090589  Report J1939TP_E_NO_CONNECTION also for direct frames and BAM.
 *                               ESCAN00090736  Fixed Tx N-PDU locking when Rx buffer is too small for next block.
 *                               ESCAN00090742  Stop ETP/CMDT Rx connection when CopyRxData(0) returns BUFREQ_NOT_OK.
 *                               ESCAN00090904  Introduced error code for Tx confirmation timeout.
 *                               ESCAN00090905  Introduced error code for interrupted BAM reception.
 *                               ESCAN00091119  Introduced error code for premature EOMAck.
 *                               ESCAN00091155  Fixed timeout between ETP.CM_DPO and ETP.DT.
 *  1.02.04  2016-08-05  vismsr  ESCAN00091337  Fixed reaction on failed CopyRxData(0).
 *  1.03.00  2016-12-15  vismmu  FEATC-300      Implemented Fast Packet Protocol.
 *                       vismsr  ESCAN00078384  Fixed abort code for untimely ETP.CM_CTS.
 *                               ESCAN00091443  Separated runtime errors from development errors.
 *                               ESCAN00092774  Fixed pairing of exclusive areas.
 *                                -             Moved handling of direct PDUs to J1939Tp_Direct.c.
 *                       vismmu   -             Reorganisation of source code to improve metrics.
 *                       vismmu  ESCAN00093040  Avoid error J1939TP_E_WRONG_ADDRESSING for unrelated CMDT session.
 *  1.03.01  2017-03-21  vismsr  FEATC-398      Rework and release of Fast Packet Protocol.
 *                       vismmu  ESCAN00078384  Fixed abort code for untimely TP.CM_CTS.
 *                               ESCAN00085080  Use DET error numbers of AUTOSAR 4.2.1
 *                       vismsr  ESCAN00093372  Fixed reception of too large messages.
 *  1.03.02  2017-08-02  vismmu  ESCAN00095631  Accept NULL SduDataPtr in J1939Tp_Transmit.
 *                       vismsr  ESCAN00080476  Fixed ETP abort for ECTS with invalid PGN.
 *                               ESCAN00080199  Fixed abort reason for TP.DT with wrong sequence number on ISOBUS.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  MISRA / PClint JUSTIFICATIONS
 *********************************************************************************************************************/
/* *INDENT-OFF* */

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */

/* *INDENT-ON* */


#if !defined (J1939TP_H)
# define J1939TP_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "J1939Tp_Cfg.h"
# include "J1939Tp_Lcfg.h"
# include "J1939Tp_PBcfg.h"

/*! \trace SPEC-2167284 */
# include "ComStack_Types.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define J1939TP_VENDOR_ID                      (30u)
# define J1939TP_MODULE_ID                      (37u)

/* AUTOSAR Specification version information */
# define J1939TP_AR_RELEASE_MAJOR_VERSION       (4u)
# define J1939TP_AR_RELEASE_MINOR_VERSION       (2u)
# define J1939TP_AR_RELEASE_REVISION_VERSION    (2u)

/* Vendor specific BSW module version information */
# define J1939TP_SW_MAJOR_VERSION               (1u)
# define J1939TP_SW_MINOR_VERSION               (3u)
# define J1939TP_SW_PATCH_VERSION               (2u)


/*
 * Development Error Detection
 */

/* ----- API service IDs ----- */
/*! \brief Service ID: J1939Tp_Init() */
# define J1939TP_SID_INIT                       (0x01u)
/*! \brief Service ID: J1939Tp_Shutdown() */
# define J1939TP_SID_SHUTDOWN                   (0x02u)
/*! \brief Service ID: J1939Tp_GetVersionInfo() */
# define J1939TP_SID_GETVERSIONINFO             (0x03u)
/*! \brief Service ID: J1939Tp_MainFunction() */
# define J1939TP_SID_MAINFUNCTION               (0x04u)
/*! \brief Service ID: J1939Tp_Transmit() */
# define J1939TP_SID_TRANSMIT                   (0x05u)
/*! \brief Service ID: J1939Tp_ChangeParameter() */
# define J1939TP_SID_CHANGEPARAMETER            (0x08u)
/*! \brief Service ID: J1939Tp_CancelTransmit() */
# define J1939TP_SID_CANCELTRANSMIT             (0x09u)
/*! \brief Service ID: J1939Tp_CancelReceive() */
# define J1939TP_SID_CANCELRECEIVE              (0x0au)
/*! \brief Service ID: J1939Tp_RxIndication() */
# define J1939TP_SID_RXINDICATION               (0x42u)
/*! \brief Service ID: J1939Tp_TxConfirmation() */
# define J1939TP_SID_TXCONFIRMATION             (0x40u)
/*! \brief Proprietary service ID: J1939Tp_InitMemory() */
# define J1939TP_SID_INITMEMORY                 (0x80u)

/* ----- Error codes ----- */
/* ----- Development Error codes ----- */
/*! \brief Proprietary development error code: No error occurred */
# define J1939TP_E_NO_ERROR                     (0x00u)
/*! \brief Error code: API service called before J1939Tp_Init or after J1939Tp_Shutdown */
# define J1939TP_E_UNINIT                       (0x01u)
/*! \brief Error code: J1939Tp_Init called after J1939Tp_Init and before J1939Tp_Shutdown */
# define J1939TP_E_REINIT                       (0x02u)
/*! \brief Error code: J1939Tp_Init called with invalid init structure */
# define J1939TP_E_INIT_FAILED                  (0x03u)
/*! \brief Error code: API service called with null pointer */
# define J1939TP_E_PARAM_POINTER                (0x10u)
/*! \brief Error code: API service called with wrong ID */
# define J1939TP_E_INVALID_PDU_SDU_ID           (0x11u)
/*! \brief Proprietary error code: Invalid length of transmitted N-SDU */
# define J1939TP_E_INVALID_LENGTH               (0x80u)
/*! \brief Proprietary error code: Invalid parameter argument to ChangeParameter */
# define J1939TP_E_INVALID_CHANGE_PARAM         (0x83u)
/*! \brief Proprietary error code: Invalid value argument to ChangeParameter */
# define J1939TP_E_INVALID_CHANGE_VALUE         (0x84u)
/*! \brief Proprietary error code: A dummy API was called */
# define J1939TP_E_DUMMY_API                    (0x9Bu)

/* ----- Runtime Error codes ----- */
/*! \brief Error code: Timeout occurred on receiver side after reception of an intermediate (E)TP.DT frame of a block or an FPP frame */
# define J1939TP_E_TIMEOUT_T1                   (0x30u)
/*! \brief Error code: Timeout occurred on receiver side after transmission of a (E)TP.CM_CTS frame */
# define J1939TP_E_TIMEOUT_T2                   (0x31u)
/*! \brief Error code: Timeout occurred on transmitter side after transmission of the last (E)TP.DT frame of a block */
# define J1939TP_E_TIMEOUT_T3                   (0x32u)
/*! \brief Error code: Timeout occurred on transmitter side after reception of a (E)TP.CM_CTS(0) frame */
# define J1939TP_E_TIMEOUT_T4                   (0x33u)
/*! \brief Error code: Timeout occurred on transmitter or receiver side while trying to send the next (E)TP.DT or (E)TP.CM frame */
# define J1939TP_E_TIMEOUT_TR                   (0x34u)
/*! \brief Error code: Timeout occurred on receiver side while trying to send the next (E)TP.CM_CTS frame after a (E)TP.CM_CTS(0) frame */
# define J1939TP_E_TIMEOUT_TH                   (0x35u)
/*! \brief Error code: Invalid value for "total message size" in received TP.CM_RTS frame */
# define J1939TP_E_INVALID_TMS                  (0x40u)
/*! \brief Error code: Value for "total number of packets" in received TP.CM_RTS frame does not match the "total message size" */
# define J1939TP_E_INVALID_TNOP                 (0x41u)
/*! \brief Error code: Invalid value for "maximum number of packets" in received TP.CM_RTS frame */
# define J1939TP_E_INVALID_MNOP                 (0x42u)
/*! \brief Error code: Unexpected PGN in received TP.CM frame */
# define J1939TP_E_INVALID_PGN                  (0x43u)
/*! \brief Error code: Invalid value for "number of packets" in received TP.CM_CTS frame */
# define J1939TP_E_INVALID_NOP                  (0x44u)
/*! \brief Error code: Invalid value for "next packet number" in received TP.CM_CTS frame */
# define J1939TP_E_INVALID_NPN                  (0x45u)
/*! \brief Error code: Invalid value for "connection abort reason" in received TP.ConnAbort frame */
# define J1939TP_E_INVALID_CAR                  (0x46u)
/*! \brief Error code: Unexpected sequence number in received TP.DT frame */
# define J1939TP_E_INVALID_SN                   (0x47u)
/*! \brief Proprietary error code: Invalid source address in received frame */
# define J1939TP_E_INVALID_SA                   (0x81u)
/*! \brief Proprietary error code: Invalid destination address in received frame */
# define J1939TP_E_INVALID_DA                   (0x82u)
/*! \brief Proprietary error code: TP.CM_RTS frame received while a CMDT reception was still active on the same connection */
# define J1939TP_E_UNTIMELY_RTS                 (0x85u)
/*! \brief Proprietary error code: Ignored untimely TP.CM_CTS frame */
# define J1939TP_E_IGNORED_CTS                  (0x86u)
/*! \brief Proprietary error code: Ignored untimely TP.CM_EndOfMsgAck frame */
# define J1939TP_E_IGNORED_EOMACK               (0x87u)
/*! \brief Proprietary error code: Ignored untimely TP.ConnAbort frame */
# define J1939TP_E_IGNORED_ABORT                (0x88u)
/*! \brief Proprietary error code: TP.CM_RTS frame received, but no free connection found */
# define J1939TP_E_NO_CONNECTION                (0x89u)
/*! \brief Proprietary error code: Invalid length of received N-PDU */
# define J1939TP_E_INVALID_DLC                  (0x8Au)
/*! \brief Proprietary error code: Value for "total message size" in received TP.CM_EOMAck frame differs from the same value in TP.CM_RTS */
# define J1939TP_E_INVALID_ATMS                 (0x8Bu)
/*! \brief Proprietary error code: Value for "total number of packets" in received TP.CM_EOMAck frame differs from the same value in TP.CM_RTS */
# define J1939TP_E_INVALID_ATNOP                (0x8Cu)
/*! \brief Proprietary error code: Unexpected TP.CM_CTS frame received during data transmission */
# define J1939TP_E_UNEXPECTED_CTS               (0x8Du)
/*! \brief Proprietary error code: Invalid value for "number of bytes to transfer" in received ETP.CM_RTS frame */
# define J1939TP_E_INVALID_NBT                  (0x90u)
/*! \brief Proprietary error code: Value for "number of bytes transferred" in received ETP.CM_EOMA frame differs from the same value in ETP.CM_RTS */
# define J1939TP_E_INVALID_ANBT                 (0x91u)
/*! \brief Proprietary error code: Unexpected ETP.CM_CTS frame received during data transmission */
# define J1939TP_E_UNEXPECTED_ECTS              (0x92u)
/*! \brief Proprietary error code: Invalid value for "data packet offset" in received ETP.CM_DPO frame */
# define J1939TP_E_INVALID_DPO                  (0x93u)
/*! \brief Proprietary error code: Invalid value for "number of packets to which to apply the offset" in received ETP.CM_DPO frame */
# define J1939TP_E_INVALID_NPO                  (0x94u)
/*! \brief Proprietary error code: Unexpected ETP.CM_DPO frame (wrong PGN) */
# define J1939TP_E_UNEXPECTED_DPO               (0x95u)
/*! \brief Proprietary error code: (E)TP.CM used with wrong addressing or invalid control byte, e.g. TP.CM_BAM with DA != 0xFF or ETP.CM with an unknown CB */
# define J1939TP_E_INVALID_CONTROL_BYTE         (0x96u)
/*! \brief Proprietary error code: Timeout of transmission confirmation callback */
# define J1939TP_E_TIMEOUT_TXCONF               (0x97u)
/*! \brief Proprietary error code: TP.CM_BAM frame received while a BAM reception was still active on the same connection */
# define J1939TP_E_UNTIMELY_BAM                 (0x98u)
/*! \brief Proprietary error code: TP.CM_EndOfMsgAck frame received before all data was transmitted */
# define J1939TP_E_EARLY_EOMACK                 (0x99u)
/*! \brief Proprietary error code: Invalid length of received N-SDU */
# define J1939TP_E_INVALID_SIZE                 (0x9Au)
/*! \brief Proprietary error code: Ignored untimely ETP.CM_CTS frame */
# define J1939TP_E_IGNORED_ECTS                 (0x9Cu)
/*! \brief Proprietary error code: Unexpected sequence counter of received FF or AF frame */
# define J1939TP_E_INVALID_SC                   (0x9Du)
/*! \brief Proprietary error code: Unexpected frame counter of received AF frame */
# define J1939TP_E_INVALID_FC                   (0x9Eu)
/*! \brief Proprietary error code: First frame received while an FPP reception was still active on the same connection */
# define J1939TP_E_UNTIMELY_FF                  (0x9Fu)
/*! \brief Proprietary error code: Timeout on transmitter side while trying to send the next FPP frame */
# define J1939TP_E_TIMEOUT_FP                   (0xA0u)


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*============================================== J1939TP_START_SEC_CODE =============================================*/
# define J1939TP_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

/************************************************************************************************************
 *  J1939Tp_InitMemory()
 ***********************************************************************************************************/
/*! \brief       Power-up memory initialization
 *  \details     Initializes component variables in *_INIT_* sections at power up.
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note         Use this function in case these variables are not initialized by the startup code.
 *******************************************************************************************/
FUNC(void, J1939TP_CODE) J1939Tp_InitMemory(void);


/*******************************************************************************************
 *  J1939Tp_Init()
 *******************************************************************************************/
/*! \brief       Initializes component
 *  \details     Initializes all component variables and sets the component state to
 *               initialized.
 *  \param[in]   config          Component configuration structure
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \pre         J1939Tp_InitMemory has been called unless J1939Tp_ModuleInitialized is initialized by start-up code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *******************************************************************************************/
FUNC(void, J1939TP_CODE) J1939Tp_Init(P2CONST(J1939Tp_ConfigType, AUTOMATIC, J1939TP_INIT_DATA) config);


/************************************************************************************************************
 *  J1939Tp_Shutdown()
 ***********************************************************************************************************/
/*! \brief       Shuts the J1939Tp module down.
 *  \details     This function is used to shutdown the J1939Tp module.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 ***********************************************************************************************************/
FUNC(void, J1939TP_CODE) J1939Tp_Shutdown(void);


# if (J1939TP_VERSION_INFO_API == STD_ON)
/*******************************************************************************************
 *  J1939Tp_GetVersionInfo()
*******************************************************************************************/
/*! \brief       Returns the version information
 *  \details     Returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  VersionInfo    Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      J1939TP_VERSION_INFO_API
 *******************************************************************************************/
FUNC(void, J1939TP_CODE) J1939Tp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, J1939TP_APPL_DATA) VersionInfo);
# endif


/************************************************************************************************************
 *  J1939Tp_Transmit()
 ***********************************************************************************************************/
/*! \brief       Requests transmission of a J1939Tp N-SDU.
 *  \details     This service function is used to request the transfer of a J1939Tp N-SDU.
 *  \param[in]   txSduId      ID of the J1939Tp N-SDU to be transmitted.
 *  \param[in]   pduInfoPtr   Pointer to J1939Tp N-SDU structure.
 *  \return      E_NOT_OK - Request failed, e.g. when the requested transmission would use a channel that is currently active.
 *  \return      E_OK     - Transmit request has been accepted.
 *  \pre         The pduInfoPtr parameter and its field SduDataPtr must not be NULL.
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-2167313
 ***********************************************************************************************************/
FUNC(Std_ReturnType, J1939TP_CODE) J1939Tp_Transmit(PduIdType txSduId,
                                                    P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) pduInfoPtr);


/************************************************************************************************************
 *  J1939Tp_CancelTransmit()
 ***********************************************************************************************************/
/*! \brief       Cancels a transmission
 *  \details     Cancels the ongoing transmission of a J1939Tp N-SDU.
 *  \param[in]   id      ID of the J1939Tp N-SDU to be canceled.
 *  \return      E_NOT_OK - Request failed, e.g. provided N-SDU is currently not transmitted.
 *  \return      E_OK     - The request has been accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        This function is not yet implemented, and returns always E_NOT_OK.
 ***********************************************************************************************************/
FUNC(Std_ReturnType, J1939TP_CODE) J1939Tp_CancelTransmit(PduIdType id);


/************************************************************************************************************
 *  J1939Tp_CancelReceive()
 ***********************************************************************************************************/
/*! \brief       Cancels a reception
 *  \details     Cancels the ongoing reception of a J1939Tp N-SDU.
 *  \param[in]   id      ID of the J1939Tp N-SDU to be canceled.
 *  \return      E_NOT_OK - Request failed, e.g. provided N-SDU is currently not received.
 *  \return      E_OK     - The request has been accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        This function is not yet implemented, and returns always E_NOT_OK.
 *  \trace       SPEC-2167311
 ***********************************************************************************************************/
FUNC(Std_ReturnType, J1939TP_CODE) J1939Tp_CancelReceive(PduIdType id);


/************************************************************************************************************
 *  J1939Tp_ChangeParameter()
 ***********************************************************************************************************/
/*! \brief       Changes parameters
 *  \details     Changes reception parameters of J1939Tp for a specific N-SDU.
 *  \param[in]   id         ID of the N-SDU for which parameters should be changed.
 *  \param[in]   parameter  ID of parameter that should be changed. range: TP_STMIN, TP_BS, TP_BC
 *  \param[in]   value      Input parameter, must be in range of <?>.
 *  \param[out]  Value       Output parameter reference, must not be NULL_PTR.
 *  \return      E_NOT_OK - The request failed, e.g. the provided parameter does not exist.
 *  \return      E_OK     - The request has been accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        This function is not yet implemented, and returns always E_NOT_OK.
 ***********************************************************************************************************/
FUNC(Std_ReturnType, J1939TP_CODE) J1939Tp_ChangeParameter(PduIdType id, TPParameterType parameter, uint16 value);

# define J1939TP_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*============================================== J1939TP_STOP_SEC_CODE ==============================================*/

#endif /* !defined (J1939TP_H) */

/**********************************************************************************************************************
 *  END OF FILE: J1939Tp.h
 *********************************************************************************************************************/
