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
 *         \file  EthTSyn_SwtMgmt_Cbk.h
 *        \brief  EthTSyn Switch Management Callback header file
 *
 *      \details  Contains all callback function declarations with respect to the EthTSyn module used for the Ethernet
 *                Switch management
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

#if !defined(ETHTSYN_SWT_MGMT_CBK_H)
# define ETHTSYN_SWT_MGMT_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) /* PRQA S 0828 */ /* MD_MSR_1.1_828 */
#  define ETHTSYN_START_SEC_CODE
/* PRQA S 5087 */ /*  MD_MSR_19.1 */
#  include "MemMap.h"

/**********************************************************************************************************************
 *  EthTSyn_SwitchMgmtInfoIndication
 *********************************************************************************************************************/
/*! \brief       Provide switch management information to the EthTSyn.
 *  \details     Ingress Switch management info indication redirected call to upper layers who registered for the call.
 *  \param[in]   CtrlIdx      EthIf controller index.
 *  \param[in]   DataPtr      Data pointer where the management information belongs to.
 *  \param[in]   MgmtInfoPtr  Management information if not NULL
 *  \pre         Module is initialized.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwitchMgmtInfoIndication(
                  uint8                 CtrlIdx,
  ETHTSYN_P2CONST(uint8)                DataPtr,
  ETHTSYN_P2CONST(EthSwt_MgmtInfoType)  MgmtInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwitchEgressTimeStampIndication
 *********************************************************************************************************************/
/*! \brief      Provide switch egress timestamp to the EthTSyn.
 * \details     Delivers to upper layers an egress timestamp value out of the Switch where MgmtInfo refers. If the HW
                resolution is lower than the Eth_TimeStampType resolution resp. range, than the remaining bits will be
                filled with 0.
 * \param[in]   CtrlIdx       EthIf controller index.
 * \param[in]   DataPointer   Buffer pointer to map the timestamp indication to the received/transmitted frame.
 * \param[in]   MgmtInfoPtr   Management information
 * \param[in]   timeStampPtr  Switch egress timestamp
 * \pre         Module is initialized.
 * \context     ANY
 * \reentrant   FALSE
 * \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwitchEgressTimeStampIndication(
                    uint8                         CtrlIdx,
    ETHTSYN_P2CONST(uint8)                        DataPtr,
    ETHTSYN_P2CONST(EthSwt_MgmtInfoType)          MgmtInfoPtr,
    ETHTSYN_P2CONST(Eth_TimeStampType)            timeStampPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwitchIngressTimeStampIndication
 *********************************************************************************************************************/
/*! \brief      Provide switch ingress timestamp to the EthTSyn.
 * \details     Delivers to upper layers an ingress timestamp value out of the Switch where MgmtInfo refers. If the HW
                resolution is lower than the Eth_TimeStampType resolution resp. range, than the remaining bits will be
                filled with 0.
 * \param[in]   CtrlIdx       EthIf controller index.
 * \param[in]   DataPointer   Buffer pointer to map the timestamp indication to the received/transmitted frame.
 * \param[in]   MgmtInfoPtr   Management information
 * \param[in]   timeStampPtr  Switch ingress timestamp
 * \pre         Module is initialized.
 * \context     ANY
 * \reentrant   FALSE
 * \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwitchIngressTimeStampIndication(
                    uint8                CtrlIdx,
    ETHTSYN_P2CONST(uint8)               DataPtr,
    ETHTSYN_P2CONST(EthSwt_MgmtInfoType) MgmtInfoPtr,
    ETHTSYN_P2CONST(Eth_TimeStampType)   timeStampPtr);


#  define ETHTSYN_STOP_SEC_CODE
/* PRQA S 5087 */ /*  MD_MSR_19.1 */
#  include "MemMap.h"

# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
#endif /* ETHTSYN_SWT_MGMT_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_SwtMgmt_Cbk.h
 *********************************************************************************************************************/
