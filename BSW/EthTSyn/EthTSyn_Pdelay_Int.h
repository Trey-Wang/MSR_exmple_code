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
 *         \file  EthTSyn_Pdelay_Int.h
 *        \brief  EthTSyn Path Delay internal header file
 *
 *      \details  Contains internal definitions and declarations used by EthTSyn (Path Delay) only
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

#if !defined(ETHTSYN_PDELAY_INT_H)
# define ETHTSYN_PDELAY_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_CfgAccess_Int.h" /* PRQA S 0828 */ /* MD_MSR_1.1_828 */

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
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define ETHTSYN_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_ProcessSmPdelayReq
 *********************************************************************************************************************/
/*! \brief      Handles the processing of the Path Delay Request state-machine for the given Port
 *  \param[in]  PortIdx     Index of the Port which state-machine should be processed
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_Pdelay_ProcessSmPdelayReq(
  EthTSyn_PortIdxType PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_ProcessSmPdelayResp
 *********************************************************************************************************************/
/*! \brief      Handles the processing of the given Path Delay Response state-machine
 *  \param[in]  PortIdx    Index of the Port the state machine should be processed for
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_Pdelay_ProcessSmPdelayResp(
  EthTSyn_PortIdxType PortIdx); /* PRQA S 0777 */ /* MD_MSR_5.1_777 */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_HandleCyclicTasks
 *********************************************************************************************************************/
/*! \brief      Handles cyclic Pdelay tasks
 *  \context    EthTSyn_MainFunction()
 *********************************************************************************************************************/
# if (ETHTSYN_P_DELAY_CONFIG_COUNT > 0u)
FUNC(void, ETHTSYN_CODE) EthTSyn_Pdelay_HandleCyclicTasks(void);
# endif /* (ETHTSYN_P_DELAY_CONFIG_COUNT > 0u) */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_GetCurrentPdelay
 *********************************************************************************************************************/
/*! \brief      Checks if Pdelay is used for the given port and returns its current value
 *  \param[in]  PortIdx    Index of the port the Pdelay is requested for
 *  \param[out] PdelayPtr  Pointer to where to store the current Pdelay
 *  \return     E_OK - Successfully retrieved the current Pdelay
 *  \return     E_NOT_OK - Failed to retrieve the current Pdelay (e.g. invalid PortIdx)
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Pdelay_GetCurrentPdelay(
                EthTSyn_PortIdxType PortIdx,
  ETHTSYN_P2VAR(EthTSyn_PdelayType) PdelayPtr);

# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
/**********************************************************************************************************************
 *  EthTSyn_Pdelay_ProcessReceivedPdelayReqMsg
 *********************************************************************************************************************/
/*! \brief      Processing of a received Pdelay request message
 *  \param[in]  ComMsgPtr                  Pointer to the common message header
 *  \param[in]  PortIdx                    Index of the Port the message was received on
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_Pdelay_ProcessReceivedPdelayReqMsg(
  ETHTSYN_CONSTP2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr,
                       EthTSyn_PortIdxType       PortIdx);

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_ProcessReceivedPdelayRespMsg
 *********************************************************************************************************************/
/*! \brief      Processing of a received Pdelay response message
 *  \param[in]  ComMsgPtr                  Pointer to the common message header
 *  \param[in]  PortIdx                    Index of the Port the message was received on
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_Pdelay_ProcessReceivedPdelayRespMsg(
  ETHTSYN_CONSTP2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr,
                       EthTSyn_PortIdxType       PortIdx); /* PRQA S 0777 */ /* MD_MSR_5.1_777 */

/**********************************************************************************************************************
 *  EthTSyn_Pdelay_ProcessReceivedRespFollowUpMsg
 *********************************************************************************************************************/
/*! \brief      Processing of a received Pdelay response FollowUp message
 *  \param[in]  ComMsgPtr                  Pointer to the common message header
 *  \param[in]  PortIdx                    Index of the Port the message was received on
 *  \context    ANY
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_Pdelay_ProcessReceivedRespFollowUpMsg(
  ETHTSYN_CONSTP2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr,
                       EthTSyn_PortIdxType       PortIdx);
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

# define ETHTSYN_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* ETHTSYN_PDELAY_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Pdelay_Int.h
 *********************************************************************************************************************/
