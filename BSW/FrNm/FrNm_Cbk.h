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
 /*!        \file  FrNm_Cbk.h
 *         \brief  AUTOSAR FlexRay Network Management
 *
 *       \details  Callback Header of AUTOSAR FlexRay Network Management
 *                 AUTOSAR Release 4.0
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

#if !defined (FRNM_CBK_H)
# define FRNM_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "FrNm_Cfg.h"                                                            /* Include FlexRay NM Configuration */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define FRNM_CBK_MAJOR_VERSION    (0x06U)
# define FRNM_CBK_MINOR_VERSION    (0x01U)
# define FRNM_CBK_PATCH_VERSION    (0x01U)

/* Definition of Callback API-Ids. */
# define FRNM_SID_TXCONFIRMATION   (0x40U) /*!< Service ID: FrNm_TxConfirmation() */
# define FRNM_SID_RXINDICATION     (0x42U) /*!< Service ID: FrNm_RxIndication() */
# define FRNM_SID_TRIGGERTRANSMIT  (0x41U) /*!< Service ID: FrNm_TriggerTransmit() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# if ( FRNM_PASSIVE_MODE_ENABLED == STD_ON)
/* ESCAN00078610 */
/*! Define FrNm_TxConfirmation() and FrNm_TriggerTransmit to empty functions in case FRNM_PASSIVE_MODE_ENABLED is STD_ON */
#  define FrNm_TxConfirmation(FrNmTxPduId)
#  define FrNm_TriggerTransmit(FrNmTxPduId, PduInfoPtr)  E_NOT_OK
# endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define FRNM_START_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  FrNm_RxIndication()
 **********************************************************************************************************************/
/*! \brief       This function is called by the FlexRay Interface after a FlexRay NM PDU has been received.
 *  \details     -
 *  \param[in]   FrNmRxPduId                 FrNm Rx PDU handle
 *  \param[out]  PduInfoPtr                  Pointer to received message data
 *  \pre         -
 *  \note        Called from FlexRay Interface
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-16122, SPEC-50103, SPEC-16105, SPEC-15881, SPEC-16052, SPEC-16130, SPEC-15961, SPEC-16092, SPEC-15968
 **********************************************************************************************************************/
extern FUNC( void, FRNM_CODE ) FrNm_RxIndication( VAR( PduIdType, AUTOMATIC ) FrNmRxPduId,
                                                  P2CONST(PduInfoType, AUTOMATIC, FRNM_APPL_VAR) PduInfoPtr );

# if ( FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
/***********************************************************************************************************************
 *  FrNm_TxConfirmation()
 **********************************************************************************************************************/
/*! \brief       This function is called by the FlexRay Interface after a FlexRay NM PDU has been transmitted.
 *  \details     -
 *  \param[in]   FrNmTxPduId                 FrNm channel handle
 *  \pre         -
 *  \note        Called from FlexRay Interface
 *  \config      FRNM_PASSIVE_MODE_ENABLED = STD_OFF
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-50141, SPEC-50091
 **********************************************************************************************************************/
extern FUNC( void, FRNM_CODE ) FrNm_TxConfirmation( VAR( PduIdType, AUTOMATIC ) FrNmTxPduId );

/***********************************************************************************************************************
 *  FrNm_TriggerTransmit()
 **********************************************************************************************************************/
/*! \brief       This function is called by the FlexRay Interface
 *               when FlexRay NM L-PDU has to be transmitted (Decoupled Mode).
 *  \details     -
 *  \param[in]   FrNmTxPduId                 FrNm Tx PDU handle
 *  \param[out]  PduInfoPtr                  Pointer to NM data buffer
 *  \return      E_OK                        Function successfully executed
 *               E_NOT_OK                    Wrong parameters passed or message transmission is disabled.
 *  \pre         -
 *  \note        Called from FlexRay Interface
 *  \config      FRNM_PASSIVE_MODE_ENABLED = STD_OFF
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-15884, SPEC-50097, SPEC-16033
 **********************************************************************************************************************/
extern FUNC( Std_ReturnType, FRNM_CODE ) FrNm_TriggerTransmit( VAR( PduIdType, AUTOMATIC ) FrNmTxPduId,
                                                               P2VAR(PduInfoType, AUTOMATIC, FRNM_APPL_VAR) PduInfoPtr );
# endif

# define FRNM_STOP_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif  /* FRNM_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: FrNm_Cbk.h
 *********************************************************************************************************************/
