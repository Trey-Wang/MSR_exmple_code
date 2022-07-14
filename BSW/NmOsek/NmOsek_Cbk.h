/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2011-2017 by Vector Informatik GmbH.                                             All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  NmOsek_Cbk.h
 *    Component:  Nm_AsrNmDirOsek
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  Callback Header of Vector Direct OSEK Network Management
 *                Complex Device Driver for AUTOSAR Release 4.0
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the NMOSEK_CBK_*_VERSION defines below.
 *********************************************************************************************************************/

#if !defined (NMOSEK_CBK_H)
#define NMOSEK_CBK_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "NmOsek_Cfg.h"                                                              /* Include NmOsek Configuration */
#include "Can_Cfg.h"                                                      /* Include Can_Cfg.h header for Can_IdType */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Component version information (decimal version of ALM implementation package) ----- */
#define NMOSEK_CBK_MAJOR_VERSION     (7u)
#define NMOSEK_CBK_MINOR_VERSION     (0u)
#define NMOSEK_CBK_PATCH_VERSION     (0u)

/* ----- API service IDs ----- */
#define NMOSEK_SID_TXCONFIRMATION                (0x0Fu) /*!< Service ID: NmOsek_TxConfirmation() */
#define NMOSEK_SID_RXINDICATION                  (0x10u) /*!< Service ID: NmOsek_RxIndication() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define NMOSEK_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* NmOsek functions called by the CanIf */

/**********************************************************************************************************************
 *  NmOsek_RxIndication()
 *********************************************************************************************************************/
/*! \brief        This service indicates a successful reception of a received NM message to the NmOsek after passing all
 *                filters and validation checks.
 *  \details      Triggers state changes and state transitions in dependency of the received message
 *  \param[in]    nmOsekRxPduId            Identification of the network through PDU-ID
 *  \param[in]    canSduPtr                Pointer to received SDU
 *  \param[in]    canId                    CAN Identifier of the received SDU
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    TRUE for different handles
 *  \synchronous  TRUE
 *  \pre          -
 *  \note         Called by CAN Interface or NmOsek_RxIndication_<X>, X=0,1,... (if available)
 *********************************************************************************************************************/
extern FUNC(void, NMOSEK_CODE) NmOsek_RxIndication(VAR(PduIdType, AUTOMATIC) nmOsekRxPduId,
                                                   P2CONST(uint8, AUTOMATIC, NMOSEK_APPL_VAR) canSduPtr,
                                                   CONST(Can_IdType, AUTOMATIC) canId);

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
 *  NmOsek_TxConfirmation()
 *********************************************************************************************************************/
/*! \brief        This service indicates a successful transmission of a sent NM message to the NmOsek on the bus.
 *  \details      Triggers state changes and state transitions in dependency of the transmitted message
 *  \param[in]    nmOsekTxPduId            Identification of the network through PDU-ID
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    TRUE for different handles
 *  \synchronous  TRUE
 *  \pre          -
 *  \note         Called by CAN Interface or NmOsek_TxConfirmation_<X>, X=0,1,... (if available)
 *********************************************************************************************************************/
extern FUNC(void, NMOSEK_CODE) NmOsek_TxConfirmation(VAR(PduIdType, AUTOMATIC) nmOsekTxPduId);
#endif

#define NMOSEK_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* NMOSEK_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: NmOsek_Cbk.h
 *********************************************************************************************************************/
