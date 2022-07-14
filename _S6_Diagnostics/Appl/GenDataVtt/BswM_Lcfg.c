/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                 Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                 All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  LICENSE
 *  -------------------------------------------------------------------------------------------------------------------
 *            Module: BswM
 *           Program: 
 *          Customer: N/A
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: 
 *    License Scope : The usage is restricted to CBD0000000_D80
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: BswM_Lcfg.c
 *   Generation Time: 2020-07-09 10:06:54
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

#define BSWM_LCFG_SOURCE

/* -----------------------------------------------------------------------------
    &&&~ INCLUDE
 ----------------------------------------------------------------------------- */
/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
#include "BswM.h"
#include "BswM_Private_Cfg.h"
#include "SchM_BswM.h"

#if !defined (BSWM_LOCAL)
# define BSWM_LOCAL static
#endif

#if !defined (BSWM_LOCAL_INLINE) /* COV_BSWM_LOCAL_INLINE */
# define BSWM_LOCAL_INLINE LOCAL_INLINE
#endif

/* -----------------------------------------------------------------------------
    &&&~ MACROS
 ----------------------------------------------------------------------------- */
/* PRQA S 3453 1 */ /* MD_BswM_3453 */
#define BswM_SetIpduGroup(pduId, bitVal) Com_SetIpduGroup(BswM_ComIPduGroupState, (pduId), (bitVal))
#define BswM_SetIpduReinitGroup(pduId, bitVal)
/* PRQA S 3453 1 */ /* MD_BswM_3453 */
#define BswM_SetIpduDMGroup(pduId, bitVal) Com_SetIpduGroup(BswM_ComRxIPduGroupDMState, (pduId), (bitVal))

/* PRQA S 3453 1 */ /* MD_BswM_3453 */
#define BswM_MarkPduGroupControlInvocation(control) BswM_PduGroupControlInvocation |= (control)
#define BswM_MarkDmControlInvocation() BswM_PduGroupControlInvocation |= BSWM_GROUPCONTROL_DM



#define BSWM_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/* -----------------------------------------------------------------------------
    &&&~ LOCAL FUNCTION DECLARATIONS
 ----------------------------------------------------------------------------- */
/**********************************************************************************************************************
 *  BswM_Action_RuleHandler()
 **********************************************************************************************************************/
/*!
 * \brief       Executes a rule.
 * \details     Arbitrates a rule and executes corresponding action list.
 * \param[in]   handleId  Id of the rule to execute.
 * \return      E_OK      No action list was executed or corresponding action list was completely executed.
 * \return      E_NOT_OK  Action list was aborted because an action failed.
 * \pre         -
 * \context     ANY
 * \reentrant   TRUE
 * \synchronous TRUE
 */
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_Action_RuleHandler(BswM_HandleType handleId);

/**********************************************************************************************************************
 *  BswM_UpdateRuleStates()
 **********************************************************************************************************************/
/*!
 * \brief       Updates the state of a rule.
 * \details     Set rule state of passed ruleId to passed state.
 * \param[in]   ruleId    Id of the rule to update.
 * \param[in]   state     New state of the rule.
 * \pre         -
 * \context     ANY
 * \reentrant   TRUE
 * \synchronous TRUE
 */
BSWM_LOCAL_INLINE FUNC(void, BSWM_CODE) BswM_UpdateRuleStates(BswM_SizeOfRuleStatesType ruleId, BswM_RuleStatesType state);

/**********************************************************************************************************************
 *  BswM_UpdateTimer()
 **********************************************************************************************************************/
/*!
 * \brief       Updates a timer.
 * \details     Set timer value of passed timerId to passed value and calculates the new state.
 * \param[in]   timerId   Id of the timer to update.
 * \param[in]   value     New value of the timer.
 * \pre         -
 * \context     ANY
 * \reentrant   TRUE
 * \synchronous TRUE
 */
BSWM_LOCAL_INLINE FUNC(void, BSWM_CODE) BswM_UpdateTimer(BswM_SizeOfTimerValueType timerId, BswM_TimerValueType value);

/*! \addtogroup    BswMGeneratedFunctions BswM Generated Functions
 * \{
 */
/* PRQA S 0779 FUNCTIONDECLARATIONS */ /* MD_BswM_0779 */

/**********************************************************************************************************************
 *  BswMActionListFunctions
 *********************************************************************************************************************/
/*! \defgroup    BswMActionListFunctions
 * \{
 */

/*!
 * \{
 * \brief       Execute actions of action list.
 * \details     Generated function which depends on the configuration. Executes the actions of the action list in the
 *              configured order.
 * \return      E_OK      Action list was completely executed.
 * \return      E_NOT_OK  Action list was aborted because an action failed.
 * \pre         -
 * \context     ANY
 * \reentrant   TRUE
 * \synchronous TRUE
 */
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_ExitRun(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_RunToPostRun(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_WaitForNvMToShutdown(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_WakeupToPrep(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_WaitForNvMWakeup(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_WakeupToRun(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_DemInit(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_InitToWakeup(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_PostRunToPrepShutdown(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_ESH_PostRunToPrepCheck(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_PostRunToRun(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_ExitPostRun(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_PrepShutdownToWaitForNvM(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_INIT_AL_Initialize(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_CC_AL_CAN00_f26020e5_TX_Disable(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_CC_AL_CAN00_f26020e5_TX_EnableNoinit(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_CC_AL_CAN00_f26020e5_Disable_DM(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_CC_AL_CAN00_f26020e5_Enable_DM(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_CC_AL_CAN00_f26020e5_RX_Disable(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_CC_AL_CAN00_f26020e5_RX_EnableNoinit(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_AlShutdownTimerStart(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_AlShutdownTimerStop(void);
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_AlShutdownTimerExpired(void);
/*! \} */ /* End of sharing description for BswMActionListFunctions */
/*! \} */ /* End of group BswMActionListFunctions */

/**********************************************************************************************************************
 *  BswMRuleFunctions
 *********************************************************************************************************************/
/*! \defgroup    BswMRuleFunctions
 * \{
 */

/*!
 * \{
 * \brief       Arbitrates the configured rule.
 * \details     Generated function which depends on the configuration. Arbitrates the rule and returns the action list
 *              which shall be executed.
 * \return      ID of action list to execute (BSWM_NO_ACTIONLIST if no ActionList shall be executed)
 * \pre         -
 * \context     ANY
 * \reentrant   TRUE
 * \synchronous TRUE
 */
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_RunToPostRun(void);
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_RunToPostRunNested(void);
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_WaitToShutdown(void);
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_WakeupToPrep(void);
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_WaitToWakeup(void);
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_WakeupToRun(void);
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_DemInit(void);
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_InitToWakeup(void);
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_PostRunToPrepNested(void);
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_PostRunNested(void);
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_PostRun(void);
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_PrepToWait(void);
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_CC_CAN00_f26020e5_TX(void);
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_CC_CAN00_f26020e5_RX_DM(void);
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_CC_CAN00_f26020e5_RX(void);
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_RuShutdownTimerStart(void);
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_RuShutdownTimerExpired(void);
/*! \} */ /* End of sharing description for BswMRuleFunctions */
/*! \} */ /* End of group BswMRuleFunctions */
/* PRQA L:FUNCTIONDECLARATIONS */
/*! \} */ /* End of group BswMGeneratedFunctions */

#define BSWM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"


/* -----------------------------------------------------------------------------
    &&&~ LOCAL VARIABLE DECLARATIONS
 ----------------------------------------------------------------------------- */

/* PRQA S 0779 VARIABLEDECLARATIONS */ /* MD_BswM_0779 */ 

#define BSWM_START_SEC_VAR_NOINIT_8BIT
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

BSWM_LOCAL VAR(uint8, BSWM_VAR_NOINIT) BswM_PduGroupControlInvocation;

#define BSWM_STOP_SEC_VAR_NOINIT_8BIT
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

#define BSWM_START_SEC_VAR_NOINIT_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"


BSWM_LOCAL VAR(Rte_ModeType_ESH_Mode, BSWM_VAR_NOINIT) BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode;
BSWM_LOCAL VAR(BswM_MdgShutdownTimerState, BSWM_VAR_NOINIT) BswM_RteRequest_Provide_MrpPShutdownTimerDisplay_requestedMode;

BSWM_LOCAL VAR(BswM_ESH_RunRequest, BSWM_VAR_NOINIT) Request_ESH_RunRequest_0_requestedMode;
BSWM_LOCAL VAR(BswM_ESH_RunRequest, BSWM_VAR_NOINIT) Request_ESH_PostRunRequest_0_requestedMode;
BSWM_LOCAL VAR(Rte_ModeType_ESH_Mode, BSWM_VAR_NOINIT) BswM_Mode_Notification_ESH_ModeNotification_BswM_MDGP_ESH_Mode;


/* PRQA S 3218 3 */ /* MD_BswM_3218 */
BSWM_LOCAL VAR(Com_IpduGroupVector, BSWM_VAR_NOINIT) BswM_ComIPduGroupState;
BSWM_LOCAL VAR(Com_IpduGroupVector, BSWM_VAR_NOINIT) BswM_ComRxIPduGroupDMState;

#define BSWM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/* PRQA L:VARIABLEDECLARATIONS */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/



/**********************************************************************************************************************
 *  LOCAL DATA
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: LOCAL DATA
**********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA
**********************************************************************************************************************/
/* PRQA S 0310 GLOBALDATADECLARATIONS */ /* MD_BswM_0310 */ 
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL DATA
**********************************************************************************************************************/
/**********************************************************************************************************************
  BswM_ActionLists
**********************************************************************************************************************/
/** 
  \var    BswM_ActionLists
  \details
  Element    Description
  FctPtr     Pointer to the array list function.
*/ 
#define BSWM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(BswM_ActionListsType, BSWM_CONST) BswM_ActionLists[23] = {
    /* Index    FctPtr                                                      Comment                   Referable Keys */
  { /*     0 */ BswM_ActionList_ESH_AL_ExitRun                       },  /* [Priority: 0] */  /* [AL_ESH_AL_ExitRun] */
  { /*     1 */ BswM_ActionList_ESH_AL_RunToPostRun                  },  /* [Priority: 0] */  /* [AL_ESH_AL_RunToPostRun] */
  { /*     2 */ BswM_ActionList_ESH_AL_WaitForNvMToShutdown          },  /* [Priority: 0] */  /* [AL_ESH_AL_WaitForNvMToShutdown] */
  { /*     3 */ BswM_ActionList_ESH_AL_WakeupToPrep                  },  /* [Priority: 0] */  /* [AL_ESH_AL_WakeupToPrep] */
  { /*     4 */ BswM_ActionList_ESH_AL_WaitForNvMWakeup              },  /* [Priority: 0] */  /* [AL_ESH_AL_WaitForNvMWakeup] */
  { /*     5 */ BswM_ActionList_ESH_AL_WakeupToRun                   },  /* [Priority: 0] */  /* [AL_ESH_AL_WakeupToRun] */
  { /*     6 */ BswM_ActionList_ESH_AL_DemInit                       },  /* [Priority: 0] */  /* [AL_ESH_AL_DemInit] */
  { /*     7 */ BswM_ActionList_ESH_AL_InitToWakeup                  },  /* [Priority: 0] */  /* [AL_ESH_AL_InitToWakeup] */
  { /*     8 */ BswM_ActionList_ESH_AL_PostRunToPrepShutdown         },  /* [Priority: 0] */  /* [AL_ESH_AL_PostRunToPrepShutdown] */
  { /*     9 */ BswM_ActionList_ESH_AL_ESH_PostRunToPrepCheck        },  /* [Priority: 0] */  /* [AL_ESH_AL_ESH_PostRunToPrepCheck] */
  { /*    10 */ BswM_ActionList_ESH_AL_PostRunToRun                  },  /* [Priority: 0] */  /* [AL_ESH_AL_PostRunToRun] */
  { /*    11 */ BswM_ActionList_ESH_AL_ExitPostRun                   },  /* [Priority: 0] */  /* [AL_ESH_AL_ExitPostRun] */
  { /*    12 */ BswM_ActionList_ESH_AL_PrepShutdownToWaitForNvM      },  /* [Priority: 0] */  /* [AL_ESH_AL_PrepShutdownToWaitForNvM] */
  { /*    13 */ BswM_ActionList_INIT_AL_Initialize                   },  /* [Priority: 0] */  /* [AL_INIT_AL_Initialize] */
  { /*    14 */ BswM_ActionList_CC_AL_CAN00_f26020e5_TX_Disable      },  /* [Priority: 0] */  /* [AL_CC_AL_CAN00_f26020e5_TX_Disable] */
  { /*    15 */ BswM_ActionList_CC_AL_CAN00_f26020e5_TX_EnableNoinit },  /* [Priority: 0] */  /* [AL_CC_AL_CAN00_f26020e5_TX_EnableNoinit] */
  { /*    16 */ BswM_ActionList_CC_AL_CAN00_f26020e5_Disable_DM      },  /* [Priority: 0] */  /* [AL_CC_AL_CAN00_f26020e5_Disable_DM] */
  { /*    17 */ BswM_ActionList_CC_AL_CAN00_f26020e5_Enable_DM       },  /* [Priority: 0] */  /* [AL_CC_AL_CAN00_f26020e5_Enable_DM] */
  { /*    18 */ BswM_ActionList_CC_AL_CAN00_f26020e5_RX_Disable      },  /* [Priority: 0] */  /* [AL_CC_AL_CAN00_f26020e5_RX_Disable] */
  { /*    19 */ BswM_ActionList_CC_AL_CAN00_f26020e5_RX_EnableNoinit },  /* [Priority: 0] */  /* [AL_CC_AL_CAN00_f26020e5_RX_EnableNoinit] */
  { /*    20 */ BswM_ActionList_AlShutdownTimerStart                 },  /* [Priority: 0] */  /* [AL_AlShutdownTimerStart] */
  { /*    21 */ BswM_ActionList_AlShutdownTimerStop                  },  /* [Priority: 0] */  /* [AL_AlShutdownTimerStop] */
  { /*    22 */ BswM_ActionList_AlShutdownTimerExpired               }   /* [Priority: 0] */  /* [AL_AlShutdownTimerExpired] */
};
#define BSWM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  BswM_DeferredRules
**********************************************************************************************************************/
/** 
  \var    BswM_DeferredRules
  \details
  Element     Description
  RulesIdx    the index of the 1:1 relation pointing to BswM_Rules
*/ 
#define BSWM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(BswM_DeferredRulesType, BSWM_CONST) BswM_DeferredRules[9] = {
    /* Index    RulesIdx        Referable Keys */
  { /*     0 */       0U },  /* [R_ESH_RunToPostRun] */
  { /*     1 */       2U },  /* [R_ESH_WaitToShutdown] */
  { /*     2 */       3U },  /* [R_ESH_WakeupToPrep] */
  { /*     3 */       4U },  /* [R_ESH_WaitToWakeup] */
  { /*     4 */       5U },  /* [R_ESH_WakeupToRun] */
  { /*     5 */      10U },  /* [R_ESH_PostRun] */
  { /*     6 */      11U },  /* [R_ESH_PrepToWait] */
  { /*     7 */      15U },  /* [R_RuShutdownTimerStart] */
  { /*     8 */      16U }   /* [R_RuShutdownTimerExpired] */
};
#define BSWM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  BswM_GenericMapping
**********************************************************************************************************************/
/** 
  \var    BswM_GenericMapping
  \brief  Maps the external id of BswMGenericRequest to an internal id and references immediate request ports.
  \details
  Element                  Description
  ExternalId               External id of BswMGenericRequest.
  ImmediateUserEndIdx      the end index of the 0:n relation pointing to BswM_ImmediateUser
  ImmediateUserStartIdx    the start index of the 0:n relation pointing to BswM_ImmediateUser
  InitValue                Initialization value of port.
*/ 
#define BSWM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(BswM_GenericMappingType, BSWM_CONST) BswM_GenericMapping[3] = {
    /* Index    ExternalId  ImmediateUserEndIdx                          ImmediateUserStartIdx                          InitValue                                                            Referable Keys */
  { /*     0 */ 230       ,                                          2U,                                            1U, BSWM_GENERICVALUE_ESH_State_ESH_INIT                          },  /* [GENERIC_0, MRP_ESH_State] */
  { /*     1 */ 231       ,                                          3U,                                            2U, BSWM_GENERICVALUE_ESH_DemInitStatus_DEM_INITIALIZED           },  /* [GENERIC_1, MRP_ESH_DemInitStatus] */
  { /*     2 */ 232       , BSWM_NO_IMMEDIATEUSERENDIDXOFGENERICMAPPING, BSWM_NO_IMMEDIATEUSERSTARTIDXOFGENERICMAPPING, BSWM_GENERICVALUE_ESH_ComMPendingRequests_ESH_COMM_NO_REQUEST }   /* [GENERIC_2, MRP_ESH_ComMPendingRequests] */
};
#define BSWM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  BswM_ImmediateUser
**********************************************************************************************************************/
/** 
  \var    BswM_ImmediateUser
  \brief  Contains all immediate request ports.
  \details
  Element             Description
  MaskedBits          contains bitcoded the boolean data of BswM_OnInitOfImmediateUser, BswM_RulesIndUsedOfImmediateUser
  RulesIndEndIdx      the end index of the 0:n relation pointing to BswM_RulesInd
  RulesIndStartIdx    the start index of the 0:n relation pointing to BswM_RulesInd
*/ 
#define BSWM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(BswM_ImmediateUserType, BSWM_CONST) BswM_ImmediateUser[3] = {
    /* Index    MaskedBits  RulesIndEndIdx                         RulesIndStartIdx                               Comment                                               Referable Keys */
  { /*     0 */      0x01U,                                    3U,                                      0U },  /* [Name: CC_CanSMIndication_CAN00_f26020e5] */  /* [MRP_CC_CanSMIndication_CAN00_f26020e5, CANSM_CHANNEL_0] */
  { /*     1 */      0x03U,                                   11U,                                      3U },  /* [Name: ESH_State]                         */  /* [MRP_ESH_State, GENERIC_0] */
  { /*     2 */      0x00U, BSWM_NO_RULESINDENDIDXOFIMMEDIATEUSER, BSWM_NO_RULESINDSTARTIDXOFIMMEDIATEUSER }   /* [Name: ESH_DemInitStatus]                 */  /* [MRP_ESH_DemInitStatus, GENERIC_1] */
};
#define BSWM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  BswM_Rules
**********************************************************************************************************************/
/** 
  \var    BswM_Rules
  \details
  Element    Description
  Id         External id of rule.
  FctPtr     Pointer to the rule function which does the arbitration.
*/ 
#define BSWM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(BswM_RulesType, BSWM_CONST) BswM_Rules[17] = {
    /* Index    Id   FctPtr                                   Referable Keys */
  { /*     0 */  9U, BswM_Rule_ESH_RunToPostRun        },  /* [R_ESH_RunToPostRun, MRP_ESH_State, MRP_ESH_ComMIndication_CAN00_f26020e5, MRP_ESH_RunRequest_0, MRP_ESH_ModeNotification] */
  { /*     1 */ 10U, BswM_Rule_ESH_RunToPostRunNested  },  /* [R_ESH_RunToPostRunNested] */
  { /*     2 */ 11U, BswM_Rule_ESH_WaitToShutdown      },  /* [R_ESH_WaitToShutdown, MRP_ESH_State, MRP_ESH_NvMIndication, MRP_ESH_NvM_WriteAllTimer, MRP_ESH_EcuM_GetValidatedWakeupEvents, MRP_ESH_ComMPendingRequests] */
  { /*     3 */ 13U, BswM_Rule_ESH_WakeupToPrep        },  /* [R_ESH_WakeupToPrep, MRP_ESH_State, MRP_ESH_NvMIndication, MRP_ESH_NvM_CancelWriteAllTimer, MRP_ESH_EcuM_GetPendingWakeupEvents, MRP_ESH_EcuM_GetValidatedWakeupEvents, MRP_ESH_ComMPendingRequests, MRP_ESH_ModeNotification] */
  { /*     4 */ 12U, BswM_Rule_ESH_WaitToWakeup        },  /* [R_ESH_WaitToWakeup, MRP_ESH_State, MRP_ESH_EcuM_GetValidatedWakeupEvents, MRP_ESH_ComMPendingRequests] */
  { /*     5 */ 14U, BswM_Rule_ESH_WakeupToRun         },  /* [R_ESH_WakeupToRun, MRP_ESH_State, MRP_ESH_EcuM_GetValidatedWakeupEvents, MRP_ESH_ComMPendingRequests, MRP_ESH_NvMIndication, MRP_ESH_NvM_CancelWriteAllTimer, MRP_ESH_ModeNotification] */
  { /*     6 */  3U, BswM_Rule_ESH_DemInit             },  /* [R_ESH_DemInit] */
  { /*     7 */  4U, BswM_Rule_ESH_InitToWakeup        },  /* [R_ESH_InitToWakeup, MRP_ESH_State] */
  { /*     8 */  7U, BswM_Rule_ESH_PostRunToPrepNested },  /* [R_ESH_PostRunToPrepNested] */
  { /*     9 */  6U, BswM_Rule_ESH_PostRunNested       },  /* [R_ESH_PostRunNested] */
  { /*    10 */  5U, BswM_Rule_ESH_PostRun             },  /* [R_ESH_PostRun, MRP_ESH_State, MRP_ESH_ModeNotification] */
  { /*    11 */  8U, BswM_Rule_ESH_PrepToWait          },  /* [R_ESH_PrepToWait, MRP_ESH_State, MRP_ESH_ModeNotification] */
  { /*    12 */  2U, BswM_Rule_CC_CAN00_f26020e5_TX    },  /* [R_CC_CAN00_f26020e5_TX, MRP_CC_CanSMIndication_CAN00_f26020e5] */
  { /*    13 */  1U, BswM_Rule_CC_CAN00_f26020e5_RX_DM },  /* [R_CC_CAN00_f26020e5_RX_DM, MRP_CC_CanSMIndication_CAN00_f26020e5] */
  { /*    14 */  0U, BswM_Rule_CC_CAN00_f26020e5_RX    },  /* [R_CC_CAN00_f26020e5_RX, MRP_CC_CanSMIndication_CAN00_f26020e5] */
  { /*    15 */ 16U, BswM_Rule_RuShutdownTimerStart    },  /* [R_RuShutdownTimerStart, MRP_ESH_RunRequest_0] */
  { /*    16 */ 15U, BswM_Rule_RuShutdownTimerExpired  }   /* [R_RuShutdownTimerExpired, MRP_BswMrpShutdownTimer] */
};
#define BSWM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  BswM_RulesInd
**********************************************************************************************************************/
/** 
  \var    BswM_RulesInd
  \brief  the indexes of the 1:1 sorted relation pointing to BswM_Rules
*/ 
#define BSWM_START_SEC_CONST_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(BswM_RulesIndType, BSWM_CONST) BswM_RulesInd[11] = {
  /* Index    RulesInd      Referable Keys */
  /*     0 */      12U,  /* [MRP_CC_CanSMIndication_CAN00_f26020e5] */
  /*     1 */      13U,  /* [MRP_CC_CanSMIndication_CAN00_f26020e5] */
  /*     2 */      14U,  /* [MRP_CC_CanSMIndication_CAN00_f26020e5] */
  /*     3 */       0U,  /* [MRP_ESH_State] */
  /*     4 */       2U,  /* [MRP_ESH_State] */
  /*     5 */       3U,  /* [MRP_ESH_State] */
  /*     6 */       4U,  /* [MRP_ESH_State] */
  /*     7 */       5U,  /* [MRP_ESH_State] */
  /*     8 */       7U,  /* [MRP_ESH_State] */
  /*     9 */      10U,  /* [MRP_ESH_State] */
  /*    10 */      11U   /* [MRP_ESH_State] */
};
#define BSWM_STOP_SEC_CONST_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  BswM_ActionListQueue
**********************************************************************************************************************/
/** 
  \var    BswM_ActionListQueue
  \brief  Variable to store action lists which shall be executed.
*/ 
#define BSWM_START_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(uBswM_ActionListQueueType, BSWM_VAR_NOINIT) BswM_ActionListQueue;  /* PRQA S 0759 */  /* MD_CSL_18.4 */
  /* Index        Referable Keys  */
  /*     0 */  /* [AL_ESH_AL_ExitRun] */
  /*     1 */  /* [AL_ESH_AL_RunToPostRun] */
  /*     2 */  /* [AL_ESH_AL_WaitForNvMToShutdown] */
  /*     3 */  /* [AL_ESH_AL_WakeupToPrep] */
  /*     4 */  /* [AL_ESH_AL_WaitForNvMWakeup] */
  /*     5 */  /* [AL_ESH_AL_WakeupToRun] */
  /*     6 */  /* [AL_ESH_AL_DemInit] */
  /*     7 */  /* [AL_ESH_AL_InitToWakeup] */
  /*     8 */  /* [AL_ESH_AL_PostRunToPrepShutdown] */
  /*     9 */  /* [AL_ESH_AL_ESH_PostRunToPrepCheck] */
  /*    10 */  /* [AL_ESH_AL_PostRunToRun] */
  /*    11 */  /* [AL_ESH_AL_ExitPostRun] */
  /*    12 */  /* [AL_ESH_AL_PrepShutdownToWaitForNvM] */
  /*    13 */  /* [AL_INIT_AL_Initialize] */
  /*    14 */  /* [AL_CC_AL_CAN00_f26020e5_TX_Disable] */
  /*    15 */  /* [AL_CC_AL_CAN00_f26020e5_TX_EnableNoinit] */
  /*    16 */  /* [AL_CC_AL_CAN00_f26020e5_Disable_DM] */
  /*    17 */  /* [AL_CC_AL_CAN00_f26020e5_Enable_DM] */
  /*    18 */  /* [AL_CC_AL_CAN00_f26020e5_RX_Disable] */
  /*    19 */  /* [AL_CC_AL_CAN00_f26020e5_RX_EnableNoinit] */
  /*    20 */  /* [AL_AlShutdownTimerStart] */
  /*    21 */  /* [AL_AlShutdownTimerStop] */
  /*    22 */  /* [AL_AlShutdownTimerExpired] */

#define BSWM_STOP_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  BswM_CanSMChannelState
**********************************************************************************************************************/
/** 
  \var    BswM_CanSMChannelState
  \brief  Variable to store current mode of BswMCanSMIndication mode request ports.
*/ 
#define BSWM_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(CanSM_BswMCurrentStateType, BSWM_VAR_NOINIT) BswM_CanSMChannelState[1];
  /* Index        Referable Keys  */
  /*     0 */  /* [CANSM_CHANNEL_0, MRP_CC_CanSMIndication_CAN00_f26020e5] */

#define BSWM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  BswM_ComMChannelState
**********************************************************************************************************************/
/** 
  \var    BswM_ComMChannelState
  \brief  Variable to store current mode of BswMComMIndication mode request ports.
*/ 
#define BSWM_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(ComM_ModeType, BSWM_VAR_NOINIT) BswM_ComMChannelState[1];
  /* Index        Referable Keys  */
  /*     0 */  /* [COMM_CHANNEL_0, MRP_ESH_ComMIndication_CAN00_f26020e5] */

#define BSWM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  BswM_GenericState
**********************************************************************************************************************/
/** 
  \var    BswM_GenericState
  \brief  Variable to store current mode of BswMGenericRequest mode request ports.
*/ 
#define BSWM_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(BswM_ModeType, BSWM_VAR_NOINIT) BswM_GenericState[3];
  /* Index        Referable Keys  */
  /*     0 */  /* [GENERIC_0, MRP_ESH_State] */
  /*     1 */  /* [GENERIC_1, MRP_ESH_DemInitStatus] */
  /*     2 */  /* [GENERIC_2, MRP_ESH_ComMPendingRequests] */

#define BSWM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  BswM_Initialized
**********************************************************************************************************************/
#define BSWM_START_SEC_VAR_ZERO_INIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(BswM_InitializedType, BSWM_VAR_ZERO_INIT) BswM_Initialized = FALSE;
#define BSWM_STOP_SEC_VAR_ZERO_INIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  BswM_ModeRequestQueue
**********************************************************************************************************************/
/** 
  \var    BswM_ModeRequestQueue
  \brief  Variable to store an immediate mode request which must be queued because of a current active arbitration.
*/ 
#define BSWM_START_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(BswM_ModeRequestQueueType, BSWM_VAR_NOINIT) BswM_ModeRequestQueue[3];
  /* Index        Referable Keys  */
  /*     0 */  /* [MRP_CC_CanSMIndication_CAN00_f26020e5, CANSM_CHANNEL_0] */
  /*     1 */  /* [MRP_ESH_State, GENERIC_0] */
  /*     2 */  /* [MRP_ESH_DemInitStatus, GENERIC_1] */

#define BSWM_STOP_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  BswM_NvMJobState
**********************************************************************************************************************/
/** 
  \var    BswM_NvMJobState
  \brief  Variable to store current mode of BswMNvMJobModeIndication mode request ports.
*/ 
#define BSWM_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(NvM_RequestResultType, BSWM_VAR_NOINIT) BswM_NvMJobState[1];
  /* Index        Referable Keys  */
  /*     0 */  /* [NVM_JOB_0, MRP_ESH_NvMIndication] */

#define BSWM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  BswM_RuleStates
**********************************************************************************************************************/
/** 
  \var    BswM_RuleStates
  \brief  Stores the last execution state of the rule.
*/ 
#define BSWM_START_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(BswM_RuleStatesType, BSWM_VAR_NOINIT) BswM_RuleStates[17];
  /* Index        Referable Keys  */
  /*     0 */  /* [R_ESH_RunToPostRun, MRP_ESH_State, MRP_ESH_ComMIndication_CAN00_f26020e5, MRP_ESH_RunRequest_0, MRP_ESH_ModeNotification] */
  /*     1 */  /* [R_ESH_RunToPostRunNested] */
  /*     2 */  /* [R_ESH_WaitToShutdown, MRP_ESH_State, MRP_ESH_NvMIndication, MRP_ESH_NvM_WriteAllTimer, MRP_ESH_EcuM_GetValidatedWakeupEvents, MRP_ESH_ComMPendingRequests] */
  /*     3 */  /* [R_ESH_WakeupToPrep, MRP_ESH_State, MRP_ESH_NvMIndication, MRP_ESH_NvM_CancelWriteAllTimer, MRP_ESH_EcuM_GetPendingWakeupEvents, MRP_ESH_EcuM_GetValidatedWakeupEvents, MRP_ESH_ComMPendingRequests, MRP_ESH_ModeNotification] */
  /*     4 */  /* [R_ESH_WaitToWakeup, MRP_ESH_State, MRP_ESH_EcuM_GetValidatedWakeupEvents, MRP_ESH_ComMPendingRequests] */
  /*     5 */  /* [R_ESH_WakeupToRun, MRP_ESH_State, MRP_ESH_EcuM_GetValidatedWakeupEvents, MRP_ESH_ComMPendingRequests, MRP_ESH_NvMIndication, MRP_ESH_NvM_CancelWriteAllTimer, MRP_ESH_ModeNotification] */
  /*     6 */  /* [R_ESH_DemInit] */
  /*     7 */  /* [R_ESH_InitToWakeup, MRP_ESH_State] */
  /*     8 */  /* [R_ESH_PostRunToPrepNested] */
  /*     9 */  /* [R_ESH_PostRunNested] */
  /*    10 */  /* [R_ESH_PostRun, MRP_ESH_State, MRP_ESH_ModeNotification] */
  /*    11 */  /* [R_ESH_PrepToWait, MRP_ESH_State, MRP_ESH_ModeNotification] */
  /*    12 */  /* [R_CC_CAN00_f26020e5_TX, MRP_CC_CanSMIndication_CAN00_f26020e5] */
  /*    13 */  /* [R_CC_CAN00_f26020e5_RX_DM, MRP_CC_CanSMIndication_CAN00_f26020e5] */
  /*    14 */  /* [R_CC_CAN00_f26020e5_RX, MRP_CC_CanSMIndication_CAN00_f26020e5] */
  /*    15 */  /* [R_RuShutdownTimerStart, MRP_ESH_RunRequest_0] */
  /*    16 */  /* [R_RuShutdownTimerExpired, MRP_BswMrpShutdownTimer] */

#define BSWM_STOP_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  BswM_TimerState
**********************************************************************************************************************/
/** 
  \var    BswM_TimerState
  \brief  Variable to store current state of BswMTimer (STARTED, STOPPER OR EXPIRED).
*/ 
#define BSWM_START_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(uBswM_TimerStateType, BSWM_VAR_NOINIT) BswM_TimerState;  /* PRQA S 0759 */  /* MD_CSL_18.4 */
  /* Index        Referable Keys  */
  /*     0 */  /* [MRP_BswMrpShutdownTimer] */
  /*     1 */  /* [MRP_ESH_NvM_CancelWriteAllTimer] */
  /*     2 */  /* [MRP_ESH_NvM_WriteAllTimer] */

#define BSWM_STOP_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  BswM_TimerValue
**********************************************************************************************************************/
/** 
  \var    BswM_TimerValue
  \brief  Variable to store current timer values.
*/ 
#define BSWM_START_SEC_VAR_NOINIT_32BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(uBswM_TimerValueType, BSWM_VAR_NOINIT) BswM_TimerValue;  /* PRQA S 0759 */  /* MD_CSL_18.4 */
  /* Index        Referable Keys  */
  /*     0 */  /* [MRP_BswMrpShutdownTimer] */
  /*     1 */  /* [MRP_ESH_NvM_CancelWriteAllTimer] */
  /*     2 */  /* [MRP_ESH_NvM_WriteAllTimer] */

#define BSWM_STOP_SEC_VAR_NOINIT_32BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */


/* PRQA L:GLOBALDATADECLARATIONS */


#define BSWM_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"


/* -----------------------------------------------------------------------------
    &&&~ FUNCTIONS
 ----------------------------------------------------------------------------- */
/**********************************************************************************************************************
 *  BswM_ExecuteIpduGroupControl()
 **********************************************************************************************************************/
/*!
 * \internal
 *  - Check if a PDU action was executed.
 *    - Copy modified vectors to local variables for not calling Com_IpduGroupControl within a critical area.
 *      - If a PDU Switch Action with enabled re-initialization was used, copy vector to a local variable.
 *      - If a PDU Switch Action with without re-initialization was used, copy vector to a local variable. 
 *        Furthermore, transfer PDU Group States to the re-initialization vector for having the same state and not 
 *        disabling/enabling the PDU during the next execution.
 *    - If a Deadline Monitoring Action was executed, copy vector to a local variable.
 *    - If a PDU Switch Action with enabled re-initialization was used, pass local vector to Com_IpduGroupControl.
 *    - If a PDU Switch Action with without re-initialization was used, pass local vector to Com_IpduGroupControl.
 *    - If a Deadline Monitoring Action was executed, pass local vector to Com_ReceptionDMControl.
 * \endinternal
 */
FUNC(void, BSWM_CODE) BswM_ExecuteIpduGroupControl(void)
{
  Com_IpduGroupVector ipduGroupState;
  Com_IpduGroupVector dmState;
  uint16 iCnt;
  uint8 controlInvocation = BSWM_GROUPCONTROL_IDLE;

  SchM_Enter_BswM_BSWM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if(BswM_PduGroupControlInvocation != BSWM_GROUPCONTROL_IDLE)
  {
    if((BswM_PduGroupControlInvocation & BSWM_GROUPCONTROL_NORMAL) != 0u)
    {
      iCnt = BSWM_IPDUGROUPVECTORSIZE;
      while(iCnt-- > (uint16)0x0000) /* PRQA S 3440 */ /* MD_BswM_3440 */
      {
        ipduGroupState[iCnt] = BswM_ComIPduGroupState[iCnt]; /* SBSW_BSWM_SETIPDUGROUPVECTOR */
      }
    }
    if((BswM_PduGroupControlInvocation & BSWM_GROUPCONTROL_DM) != 0u)
    {
      iCnt = BSWM_IPDUGROUPVECTORSIZE;
      while(iCnt-- > (uint16)0x0000) /* PRQA S 3440 */ /* MD_BswM_3440 */
      {
        dmState[iCnt] = BswM_ComRxIPduGroupDMState[iCnt]; /* SBSW_BSWM_SETIPDUGROUPVECTOR */
      }
    }
    controlInvocation = BswM_PduGroupControlInvocation;
    BswM_PduGroupControlInvocation = BSWM_GROUPCONTROL_IDLE;
  }
  SchM_Exit_BswM_BSWM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if(controlInvocation != BSWM_GROUPCONTROL_IDLE)
  {
    if((controlInvocation & BSWM_GROUPCONTROL_NORMAL) != 0u)
    {
      Com_IpduGroupControl(ipduGroupState, FALSE); /* SBSW_BSWM_IPDUGROUPVECTORCALL */
    }
    if((controlInvocation & BSWM_GROUPCONTROL_DM) != 0u)
    {
      Com_ReceptionDMControl(dmState); /* SBSW_BSWM_IPDUGROUPVECTORCALL */
    }
  }
} /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */

/**********************************************************************************************************************
 *  BswM_Action_RuleHandler()
 **********************************************************************************************************************/
/*!
 * \internal
 *  - Arbitrate the corresponding rule to the passed id.
 *  - If arbitration leads to a valid action list ID, execute the corresponding action list. (SPEC-7866)
 *  - Return E_OK if no action list was executed, otherwise the result of the executed action list.
 * \endinternal
 */
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_Action_RuleHandler(BswM_HandleType handleId)
{
  BswM_HandleType actionListIndex;
  Std_ReturnType retVal = E_NOT_OK;
  
  if (handleId < BswM_GetSizeOfRules())
  {
    SchM_Enter_BswM_BSWM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    actionListIndex = BswM_GetFctPtrOfRules(handleId)(); /* SBSW_BSWM_RULEFCTPTR */
    SchM_Exit_BswM_BSWM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if(actionListIndex < BswM_GetSizeOfActionLists())
    {
      retVal = BswM_GetFctPtrOfActionLists(actionListIndex)(); /* SBSW_BSWM_ACTIONLISTFCTPTR */
    }
    else
    {
      retVal = E_OK;
    }
  }
  return retVal;
} 

/**********************************************************************************************************************
 *  BswM_UpdateRuleStates()
 **********************************************************************************************************************/
/*!
 * \internal
 *  Set rule state of passed ruleId to passed state.
 * \endinternal
 */
BSWM_LOCAL_INLINE FUNC(void, BSWM_CODE) BswM_UpdateRuleStates(BswM_SizeOfRuleStatesType ruleId, BswM_RuleStatesType state)
{
  if (ruleId < BswM_GetSizeOfRuleStates())
  {
    BswM_SetRuleStates(ruleId, state); /* SBSW_BSWM_SETRULESTATE */
  }
}

/**********************************************************************************************************************
 *  BswM_UpdateTimer()
 **********************************************************************************************************************/
/*!
 * \internal
 *  Set timer value of passed timerId to passed value.
 *  Set timer to BSWM_TIMER_STOPPED if value is zero, otherwise to BSWM_TIMER_STARTED.
 * \endinternal
 */
BSWM_LOCAL_INLINE FUNC(void, BSWM_CODE) BswM_UpdateTimer(BswM_SizeOfTimerValueType timerId, BswM_TimerValueType value)
{
  if (timerId < BswM_GetSizeOfTimerValue())
  {
      SchM_Enter_BswM_BSWM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      BswM_SetTimerValue(timerId, value); /* SBSW_BSWM_SETTIMER */
      BswM_SetTimerState(timerId, (BswM_TimerStateType)((value != 0u) ? BSWM_TIMER_STARTED : BSWM_TIMER_STOPPED)); /* SBSW_BSWM_SETTIMER */
      SchM_Exit_BswM_BSWM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
}

/**********************************************************************************************************************
 *  BswM_Init_Gen
 *********************************************************************************************************************/
/*!
\internal
 *  - Initialize all generated variables.
 *  - Execute all action lists which are configured to be executed during initialization.
\endinternal
*/
FUNC(void, BSWM_CODE) BswM_Init_Gen(void)
{

  BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode = 0xFF;
  BswM_RteRequest_Provide_MrpPShutdownTimerDisplay_requestedMode = 0xFFU;
  Request_ESH_RunRequest_0_requestedMode = RELEASED;
  Request_ESH_PostRunRequest_0_requestedMode = RELEASED;
  BswM_Mode_Notification_ESH_ModeNotification_BswM_MDGP_ESH_Mode = RTE_MODE_ESH_Mode_STARTUP;
  BswM_PduGroupControlInvocation = BSWM_GROUPCONTROL_IDLE;

  /* PRQA S 3109 COMCLEARIPDU */ /* MD_BswM_3109 */
  Com_ClearIpduGroupVector(BswM_ComIPduGroupState); /* SBSW_BSWM_IPDUGROUPVECTORCALL */
  Com_ClearIpduGroupVector(BswM_ComRxIPduGroupDMState); /* SBSW_BSWM_IPDUGROUPVECTORCALL */
  /* PRQA L:COMCLEARIPDU */

  (void)BswM_ActionList_INIT_AL_Initialize();
}

/**********************************************************************************************************************
 *  BswM_ModeNotificationFct
 *********************************************************************************************************************/
/*!
\internal
 *  - Do the following for all configured SwitchPorts:
 *    - If mode was changed by an action.
 *      - Forward mode change to RTE by the corresponding Rte Switch API.
 *      - If mode was accepted, mark local mode as unchanged for next execution.
 *  - Do the following for all configured RteModeRequestPort:
 *    - If mode was changed by an action.
 *      - Forward mode change to RTE by the corresponding Rte Write API.
 *      - If mode was accepted, mark local mode as unchanged for next execution.
\endinternal
*/
FUNC(void, BSWM_CODE) BswM_ModeNotificationFct(void)
{
  if(BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode != 0xFF)
  {
    if(Rte_Switch_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode(BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode) == RTE_E_OK)
    {
      BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode = 0xFF;
    }
  }
  if(BswM_RteRequest_Provide_MrpPShutdownTimerDisplay_requestedMode != 0xFFU)
  {
    if(Rte_Write_Provide_MrpPShutdownTimerDisplay_requestedMode(BswM_RteRequest_Provide_MrpPShutdownTimerDisplay_requestedMode) == RTE_E_OK)
    {
      BswM_RteRequest_Provide_MrpPShutdownTimerDisplay_requestedMode = 0xFFU;
    }
  }
}

/**********************************************************************************************************************
 *  BswM_SwcModeRequestUpdateFct
 *********************************************************************************************************************/
/*!
\internal
 *  - Do the following for all SwcModeNotifications:
 *    - Get mode from Rte by calling the corresponding Rte_Mode API.
 *    - If mode is valid (not in transition), store mode for arbitration.
 *  - Do the following for all SwcModeRequests:
 *    - Get mode from Rte by calling the corresponding Rte_Read API.
 *    - Store mode for arbitration.
\endinternal
*/
FUNC(void, BSWM_CODE) BswM_SwcModeRequestUpdateFct(void)
{
  uint32 mode;
  mode = Rte_Mode_Notification_ESH_ModeNotification_BswM_MDGP_ESH_Mode();
  if (mode != RTE_TRANSITION_ESH_Mode)
  {
    BswM_Mode_Notification_ESH_ModeNotification_BswM_MDGP_ESH_Mode = (Rte_ModeType_ESH_Mode)mode;
  }
  (void)Rte_Read_Request_ESH_RunRequest_0_requestedMode(&Request_ESH_RunRequest_0_requestedMode); /* SBSW_BSWM_RTE_READ */
  (void)Rte_Read_Request_ESH_PostRunRequest_0_requestedMode(&Request_ESH_PostRunRequest_0_requestedMode); /* SBSW_BSWM_RTE_READ */
}

/**********************************************************************************************************************
 *  BswMActionListFunctions
 *********************************************************************************************************************/
/*!
\internal
 *  - Do this for every configured action in action list:
 *    - Execute action.
 *    - If action fails and Dem Event is configured for the action: Set Error Status of Dem Event to failed.
 *    - If action fails and Aborting is enabled for the action: Abort iteration and return E_NOT_OK.
 *  - If all actions were executed return E_OK.
\endinternal
*/

/**********************************************************************************************************************
 *  BswM_ActionList_ESH_AL_ExitRun
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_ExitRun(void)
{
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  SchM_Enter_BswM_BSWM_EXCLUSIVE_AREA_0();
  /*lint -restore */
  ESH_ComM_CheckPendingRequests();
  (void)BswM_Action_RuleHandler(BSWM_ID_RULE_ESH_RunToPostRunNested);
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  SchM_Exit_BswM_BSWM_EXCLUSIVE_AREA_0();
  /*lint -restore */
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_ESH_AL_RunToPostRun
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_RunToPostRun(void)
{
  ComM_CommunicationAllowed(ComMConf_ComMChannel_CAN00_f26020e5, FALSE);
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  EcuM_ClearValidatedWakeupEvent(ECUM_WKSOURCE_ALL_SOURCES);
  /*lint -restore */
  BswM_ESH_OnEnterPostRun();
  BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode = RTE_MODE_ESH_Mode_POSTRUN;
  BswM_RequestMode(BSWM_GENERIC_ESH_State, BSWM_GENERICVALUE_ESH_State_ESH_POST_RUN);
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_ESH_AL_WaitForNvMToShutdown
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_WaitForNvMToShutdown(void)
{
  BswM_UpdateTimer(BSWM_TMR_ESH_NvM_WriteAllTimer, 0U);
  BswM_ESH_OnEnterShutdown();
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  (void)EcuM_GoToSelectedShutdownTarget();
  /*lint -restore */
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_ESH_AL_WakeupToPrep
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_WakeupToPrep(void)
{
  BswM_ESH_OnEnterPrepShutdown();
  BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode = RTE_MODE_ESH_Mode_SHUTDOWN;
  BswM_RequestMode(BSWM_GENERIC_ESH_State, BSWM_GENERICVALUE_ESH_State_ESH_PREP_SHUTDOWN);
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_ESH_AL_WaitForNvMWakeup
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_WaitForNvMWakeup(void)
{
  BswM_UpdateTimer(BSWM_TMR_ESH_NvM_WriteAllTimer, 0U);
  BswM_UpdateTimer(BSWM_TMR_ESH_NvM_CancelWriteAllTimer, 6000UL);
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  NvM_CancelWriteAll();
  /*lint -restore */
  BswM_ESH_OnEnterWakeup();
  BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode = RTE_MODE_ESH_Mode_WAKEUP;
  BswM_RequestMode(BSWM_GENERIC_ESH_State, BSWM_GENERICVALUE_ESH_State_ESH_WAKEUP);
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_ESH_AL_WakeupToRun
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_WakeupToRun(void)
{
  BswM_UpdateTimer(BSWM_TMR_ESH_NvM_CancelWriteAllTimer, 0U);
  (void)BswM_Action_RuleHandler(BSWM_ID_RULE_ESH_DemInit);
  ComM_CommunicationAllowed(ComMConf_ComMChannel_CAN00_f26020e5, TRUE);
  BswM_ESH_OnEnterRun();
  BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode = RTE_MODE_ESH_Mode_RUN;
  BswM_RequestMode(BSWM_GENERIC_ESH_State, BSWM_GENERICVALUE_ESH_State_ESH_RUN);
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_ESH_AL_DemInit
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_DemInit(void)
{
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  Dem_Init(Dem_Config_Ptr);
  /*lint -restore */
  BswM_RequestMode(BSWM_GENERIC_ESH_DemInitStatus, BSWM_GENERICVALUE_ESH_DemInitStatus_DEM_INITIALIZED);
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_ESH_AL_InitToWakeup
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_InitToWakeup(void)
{
  BswM_ESH_OnEnterWakeup();
  BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode = RTE_MODE_ESH_Mode_WAKEUP;
  BswM_RequestMode(BSWM_GENERIC_ESH_State, BSWM_GENERICVALUE_ESH_State_ESH_WAKEUP);
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_ESH_AL_PostRunToPrepShutdown
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_PostRunToPrepShutdown(void)
{
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  Dem_Shutdown();
  /*lint -restore */
  BswM_RequestMode(BSWM_GENERIC_ESH_DemInitStatus, BSWM_GENERICVALUE_ESH_DemInitStatus_DEM_NOT_INITIALIZED);
  BswM_ESH_OnEnterPrepShutdown();
  BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode = RTE_MODE_ESH_Mode_SHUTDOWN;
  BswM_RequestMode(BSWM_GENERIC_ESH_State, BSWM_GENERICVALUE_ESH_State_ESH_PREP_SHUTDOWN);
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_ESH_AL_ESH_PostRunToPrepCheck
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_ESH_PostRunToPrepCheck(void)
{
  (void)BswM_Action_RuleHandler(BSWM_ID_RULE_ESH_PostRunToPrepNested);
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_ESH_AL_PostRunToRun
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_PostRunToRun(void)
{
  ComM_CommunicationAllowed(ComMConf_ComMChannel_CAN00_f26020e5, TRUE);
  BswM_ESH_OnEnterRun();
  BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode = RTE_MODE_ESH_Mode_RUN;
  BswM_RequestMode(BSWM_GENERIC_ESH_State, BSWM_GENERICVALUE_ESH_State_ESH_RUN);
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_ESH_AL_ExitPostRun
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_ExitPostRun(void)
{
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  SchM_Enter_BswM_BSWM_EXCLUSIVE_AREA_0();
  /*lint -restore */
  ESH_ComM_CheckPendingRequests();
  (void)BswM_Action_RuleHandler(BSWM_ID_RULE_ESH_PostRunNested);
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  SchM_Exit_BswM_BSWM_EXCLUSIVE_AREA_0();
  /*lint -restore */
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_ESH_AL_PrepShutdownToWaitForNvM
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_ESH_AL_PrepShutdownToWaitForNvM(void)
{
  BswM_UpdateTimer(BSWM_TMR_ESH_NvM_WriteAllTimer, 6000UL);
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  NvM_WriteAll();
  /*lint -restore */
  ESH_ComM_CheckPendingRequests();
  BswM_ESH_OnEnterWaitForNvm();
  BswM_RequestMode(BSWM_GENERIC_ESH_State, BSWM_GENERICVALUE_ESH_State_ESH_WAIT_FOR_NVM);
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_INIT_AL_Initialize
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_INIT_AL_Initialize(void)
{
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  Dio_Init(&DioConfig);
  /*lint -restore */
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  Fls_Init(&FlsConfigSet);
  /*lint -restore */
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  Mcu_Init(McuModuleConfiguration);
  /*lint -restore */
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  Can_Init(Can_Config_Ptr);
  /*lint -restore */
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  CanIf_Init(CanIf_Config_Ptr);
  /*lint -restore */
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  Fee_Init();
  /*lint -restore */
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  Com_Init(Com_Config_Ptr);
  /*lint -restore */
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  PduR_Init(PduR_Config_Ptr);
  /*lint -restore */
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  CanSM_Init(CanSM_Config_Ptr);
  /*lint -restore */
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  CanNm_Init(CanNm_Config_Ptr);
  /*lint -restore */
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  Nm_Init();
  /*lint -restore */
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  CanTp_Init(CanTp_Config_Ptr);
  /*lint -restore */
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  NvM_Init();
  /*lint -restore */
  BswM_INIT_NvMReadAll();
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  ComM_Init();
  /*lint -restore */
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  Dcm_Init(NULL_PTR);
  /*lint -restore */
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  Dem_Init(Dem_Config_Ptr);
  /*lint -restore */
  /*lint -save -e534 *//* PRQA S 3109, 3200 1 */ /* MD_MSR_14.3, MD_BswM_3200 */
  (void)Rte_Start();
  /*lint -restore */
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_CC_AL_CAN00_f26020e5_TX_Disable
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_CC_AL_CAN00_f26020e5_TX_Disable(void)
{
  /* PRQA S 0277, 3109, 3201, 3325 COMSETIPDU */ /* MD_BswM_0277, MD_BswM_3109, MD_BswM_3201, MD_BswM_3325 */ /*lint -e506 -e572 */
  BswM_SetIpduGroup(ComConf_ComIPduGroup_MyECU_oCAN00_Tx_fcef2243, FALSE); /* SBSW_BSWM_IPDUGROUPVECTORCALL */
  /* PRQA L:COMSETIPDU */ /*lint +e506 +e572 */
  /* PRQA S 0277, 3109, 3201, 3325 COMSETIPDU */ /* MD_BswM_0277, MD_BswM_3109, MD_BswM_3201, MD_BswM_3325 */ /*lint -e506 -e572 */
  BswM_SetIpduDMGroup(ComConf_ComIPduGroup_MyECU_oCAN00_Tx_fcef2243, FALSE); /* SBSW_BSWM_IPDUGROUPVECTORCALL */
  /* PRQA L:COMSETIPDU */ /*lint +e506 +e572 */
  BswM_MarkPduGroupControlInvocation(BSWM_GROUPCONTROL_NORMAL);
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_CC_AL_CAN00_f26020e5_TX_EnableNoinit
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_CC_AL_CAN00_f26020e5_TX_EnableNoinit(void)
{
  /* PRQA S 0277, 3109, 3201, 3325 COMSETIPDU */ /* MD_BswM_0277, MD_BswM_3109, MD_BswM_3201, MD_BswM_3325 */ /*lint -e506 -e572 */
  BswM_SetIpduGroup(ComConf_ComIPduGroup_MyECU_oCAN00_Tx_fcef2243, TRUE); /* SBSW_BSWM_IPDUGROUPVECTORCALL */
  /* PRQA L:COMSETIPDU */ /*lint +e506 +e572 */
  /* PRQA S 0277, 3109, 3201, 3325 COMSETIPDU */ /* MD_BswM_0277, MD_BswM_3109, MD_BswM_3201, MD_BswM_3325 */ /*lint -e506 -e572 */
  BswM_SetIpduDMGroup(ComConf_ComIPduGroup_MyECU_oCAN00_Tx_fcef2243, TRUE); /* SBSW_BSWM_IPDUGROUPVECTORCALL */
  /* PRQA L:COMSETIPDU */ /*lint +e506 +e572 */
  BswM_MarkPduGroupControlInvocation(BSWM_GROUPCONTROL_NORMAL);
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_CC_AL_CAN00_f26020e5_Disable_DM
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_CC_AL_CAN00_f26020e5_Disable_DM(void)
{
  /* PRQA S 0277, 3109, 3201, 3325 COMSETIPDU */ /* MD_BswM_0277, MD_BswM_3109, MD_BswM_3201, MD_BswM_3325 */ /*lint -e506 -e572 */
  BswM_SetIpduDMGroup(ComConf_ComIPduGroup_MyECU_oCAN00_Rx_fcef2243, FALSE); /* SBSW_BSWM_IPDUGROUPVECTORCALL */
  /* PRQA L:COMSETIPDU */ /*lint +e506 +e572 */
  BswM_MarkDmControlInvocation();
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_CC_AL_CAN00_f26020e5_Enable_DM
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_CC_AL_CAN00_f26020e5_Enable_DM(void)
{
  /* PRQA S 0277, 3109, 3201, 3325 COMSETIPDU */ /* MD_BswM_0277, MD_BswM_3109, MD_BswM_3201, MD_BswM_3325 */ /*lint -e506 -e572 */
  BswM_SetIpduDMGroup(ComConf_ComIPduGroup_MyECU_oCAN00_Rx_fcef2243, TRUE); /* SBSW_BSWM_IPDUGROUPVECTORCALL */
  /* PRQA L:COMSETIPDU */ /*lint +e506 +e572 */
  BswM_MarkDmControlInvocation();
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_CC_AL_CAN00_f26020e5_RX_Disable
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_CC_AL_CAN00_f26020e5_RX_Disable(void)
{
  /* PRQA S 0277, 3109, 3201, 3325 COMSETIPDU */ /* MD_BswM_0277, MD_BswM_3109, MD_BswM_3201, MD_BswM_3325 */ /*lint -e506 -e572 */
  BswM_SetIpduGroup(ComConf_ComIPduGroup_MyECU_oCAN00_Rx_fcef2243, FALSE); /* SBSW_BSWM_IPDUGROUPVECTORCALL */
  /* PRQA L:COMSETIPDU */ /*lint +e506 +e572 */
  /* PRQA S 0277, 3109, 3201, 3325 COMSETIPDU */ /* MD_BswM_0277, MD_BswM_3109, MD_BswM_3201, MD_BswM_3325 */ /*lint -e506 -e572 */
  BswM_SetIpduDMGroup(ComConf_ComIPduGroup_MyECU_oCAN00_Rx_fcef2243, FALSE); /* SBSW_BSWM_IPDUGROUPVECTORCALL */
  /* PRQA L:COMSETIPDU */ /*lint +e506 +e572 */
  BswM_MarkPduGroupControlInvocation(BSWM_GROUPCONTROL_NORMAL);
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_CC_AL_CAN00_f26020e5_RX_EnableNoinit
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_CC_AL_CAN00_f26020e5_RX_EnableNoinit(void)
{
  /* PRQA S 0277, 3109, 3201, 3325 COMSETIPDU */ /* MD_BswM_0277, MD_BswM_3109, MD_BswM_3201, MD_BswM_3325 */ /*lint -e506 -e572 */
  BswM_SetIpduGroup(ComConf_ComIPduGroup_MyECU_oCAN00_Rx_fcef2243, TRUE); /* SBSW_BSWM_IPDUGROUPVECTORCALL */
  /* PRQA L:COMSETIPDU */ /*lint +e506 +e572 */
  /* PRQA S 0277, 3109, 3201, 3325 COMSETIPDU */ /* MD_BswM_0277, MD_BswM_3109, MD_BswM_3201, MD_BswM_3325 */ /*lint -e506 -e572 */
  BswM_SetIpduDMGroup(ComConf_ComIPduGroup_MyECU_oCAN00_Rx_fcef2243, TRUE); /* SBSW_BSWM_IPDUGROUPVECTORCALL */
  /* PRQA L:COMSETIPDU */ /*lint +e506 +e572 */
  BswM_MarkPduGroupControlInvocation(BSWM_GROUPCONTROL_NORMAL);
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_AlShutdownTimerStart
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_AlShutdownTimerStart(void)
{
  BswM_UpdateTimer(BSWM_TMR_BswMrpShutdownTimer, 1000UL);
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_AlShutdownTimerStop
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_AlShutdownTimerStop(void)
{
  BswM_UpdateTimer(BSWM_TMR_BswMrpShutdownTimer, 0U);
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswM_ActionList_AlShutdownTimerExpired
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(Std_ReturnType, BSWM_CODE) BswM_ActionList_AlShutdownTimerExpired(void)
{
  BswM_RteRequest_Provide_MrpPShutdownTimerDisplay_requestedMode = TIMER_EXPIRED;
  return E_OK;
}/* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  BswMRuleFunctions
 *********************************************************************************************************************/
/*!
\internal
 *  - Check if rule is not deactivated [only if BswMRuleControlEnabled]:
 *    - Evaluate logical expression.
 *    - If evaluation of logical expression is true:
 *      - If TrueActionList is configured and its execution type is TRIGGER:
 *        - Check if last evaluation result was not true:
 *          - Store current evaluation result.
 *          - Execute the configured action list.
 *      - Else store current evaluation result if execution type of FalseActionList is TRIGGER.
 *      - If TrueActionList is configured and its execution type is CONDITIONAL:
 *        - Execute the configured action list.
 *    - If evaluation of logical expression is false:
 *      - If FalseActionList is configured and its execution type is TRIGGER:
 *        - Check if last evaluation result was not false:
 *          - Store current evaluation result.
 *          - Execute the configured action list.
 *      - Else store current evaluation result if execution type of TrueActionList is TRIGGER.
 *      - If FalsActionList is configured and its execution type is CONDITIONAL:
 *        - Execute the configured action list.
\endinternal
*/

/**********************************************************************************************************************
 *  BswM_Rule_ESH_RunToPostRun
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_RunToPostRun(void)
{
  BswM_HandleType retVal = BSWM_NO_ACTIONLIST;
  /* Evaluate logical expression ESH_LE_RunToPostRun. */ /* PRQA S 3415 1 */ /* MD_MSR_12.4 */
  if((BswM_GetGenericState(0) == BSWM_GENERICVALUE_ESH_State_ESH_RUN) && (BswM_GetComMChannelState(0) == COMM_NO_COMMUNICATION) && (Request_ESH_RunRequest_0_requestedMode == RELEASED) && (BswM_Mode_Notification_ESH_ModeNotification_BswM_MDGP_ESH_Mode == RTE_MODE_ESH_Mode_RUN))
  {
    /* Return conditional action list BswM_ActionList_ESH_AL_ExitRun. */
    retVal = BSWM_ID_AL_ESH_AL_ExitRun;
  }
  /* No false action list configured. */
  return retVal;
}

/**********************************************************************************************************************
 *  BswM_Rule_ESH_RunToPostRunNested
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_RunToPostRunNested(void)
{
  BswM_HandleType retVal = BSWM_NO_ACTIONLIST;
  /* Evaluate logical expression ESH_LE_ComMNoPendingRequests. */
  if(BswM_GetGenericState(2) == BSWM_GENERICVALUE_ESH_ComMPendingRequests_ESH_COMM_NO_REQUEST)
  {
    /* Return conditional action list BswM_ActionList_ESH_AL_RunToPostRun. */
    retVal = BSWM_ID_AL_ESH_AL_RunToPostRun;
  }
  /* No false action list configured. */
  return retVal;
}

/**********************************************************************************************************************
 *  BswM_Rule_ESH_WaitToShutdown
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_WaitToShutdown(void)
{
  BswM_HandleType retVal = BSWM_NO_ACTIONLIST;
  /* Evaluate logical expression ESH_LE_WaitForNvMToShutdown. */ /* PRQA S 3415 1 */ /* MD_MSR_12.4 */
  if((BswM_GetGenericState(0) == BSWM_GENERICVALUE_ESH_State_ESH_WAIT_FOR_NVM) && ((BswM_GetNvMJobState(0) != NVM_REQ_PENDING) || (BswM_GetTimerState(2) != BSWM_TIMER_STARTED)) && ((EcuM_GetValidatedWakeupEvents() == 0u) && (BswM_GetGenericState(2) == BSWM_GENERICVALUE_ESH_ComMPendingRequests_ESH_COMM_NO_REQUEST)))
  {
    /* Return conditional action list BswM_ActionList_ESH_AL_WaitForNvMToShutdown. */
    retVal = BSWM_ID_AL_ESH_AL_WaitForNvMToShutdown;
  }
  /* No false action list configured. */
  return retVal;
}

/**********************************************************************************************************************
 *  BswM_Rule_ESH_WakeupToPrep
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_WakeupToPrep(void)
{
  BswM_HandleType retVal = BSWM_NO_ACTIONLIST;
  /* Evaluate logical expression ESH_LE_WakeupToPrepShutdown. */ /* PRQA S 3415 1 */ /* MD_MSR_12.4 */
  if((BswM_GetGenericState(0) == BSWM_GENERICVALUE_ESH_State_ESH_WAKEUP) && ((BswM_GetNvMJobState(0) != NVM_REQ_PENDING) || (BswM_GetTimerState(1) != BSWM_TIMER_STARTED)) && (EcuM_GetPendingWakeupEvents() == 0u) && ((EcuM_GetValidatedWakeupEvents() == 0u) && (BswM_GetGenericState(2) == BSWM_GENERICVALUE_ESH_ComMPendingRequests_ESH_COMM_NO_REQUEST)) && (BswM_Mode_Notification_ESH_ModeNotification_BswM_MDGP_ESH_Mode == RTE_MODE_ESH_Mode_WAKEUP))
  {
    /* Return conditional action list BswM_ActionList_ESH_AL_WakeupToPrep. */
    retVal = BSWM_ID_AL_ESH_AL_WakeupToPrep;
  }
  /* No false action list configured. */
  return retVal;
}

/**********************************************************************************************************************
 *  BswM_Rule_ESH_WaitToWakeup
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_WaitToWakeup(void)
{
  BswM_HandleType retVal = BSWM_NO_ACTIONLIST;
  /* Evaluate logical expression ESH_LE_WaitForNvMToWakeup. */ /* PRQA S 3415 1 */ /* MD_MSR_12.4 */
  if((BswM_GetGenericState(0) == BSWM_GENERICVALUE_ESH_State_ESH_WAIT_FOR_NVM) && ((EcuM_GetValidatedWakeupEvents() != 0u) || (BswM_GetGenericState(2) == BSWM_GENERICVALUE_ESH_ComMPendingRequests_ESH_COMM_PENDING_REQUEST)))
  {
    /* Return conditional action list BswM_ActionList_ESH_AL_WaitForNvMWakeup. */
    retVal = BSWM_ID_AL_ESH_AL_WaitForNvMWakeup;
  }
  /* No false action list configured. */
  return retVal;
}

/**********************************************************************************************************************
 *  BswM_Rule_ESH_WakeupToRun
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_WakeupToRun(void)
{
  BswM_HandleType retVal = BSWM_NO_ACTIONLIST;
  /* Evaluate logical expression ESH_LE_WakeupToRun. */ /* PRQA S 3415 1 */ /* MD_MSR_12.4 */
  if((BswM_GetGenericState(0) == BSWM_GENERICVALUE_ESH_State_ESH_WAKEUP) && ((EcuM_GetValidatedWakeupEvents() != 0u) || (BswM_GetGenericState(2) == BSWM_GENERICVALUE_ESH_ComMPendingRequests_ESH_COMM_PENDING_REQUEST)) && ((BswM_GetNvMJobState(0) != NVM_REQ_PENDING) || (BswM_GetTimerState(1) != BSWM_TIMER_STARTED)) && (BswM_Mode_Notification_ESH_ModeNotification_BswM_MDGP_ESH_Mode == RTE_MODE_ESH_Mode_WAKEUP))
  {
    /* Return conditional action list BswM_ActionList_ESH_AL_WakeupToRun. */
    retVal = BSWM_ID_AL_ESH_AL_WakeupToRun;
  }
  /* No false action list configured. */
  return retVal;
}

/**********************************************************************************************************************
 *  BswM_Rule_ESH_DemInit
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_DemInit(void)
{
  BswM_HandleType retVal = BSWM_NO_ACTIONLIST;
  /* Evaluate logical expression ESH_LE_DemNotInitialized. */
  if(BswM_GetGenericState(1) == BSWM_GENERICVALUE_ESH_DemInitStatus_DEM_NOT_INITIALIZED)
  {
    /* Return conditional action list BswM_ActionList_ESH_AL_DemInit. */
    retVal = BSWM_ID_AL_ESH_AL_DemInit;
  }
  /* No false action list configured. */
  return retVal;
}

/**********************************************************************************************************************
 *  BswM_Rule_ESH_InitToWakeup
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_InitToWakeup(void)
{
  BswM_HandleType retVal = BSWM_NO_ACTIONLIST;
  /* Evaluate logical expression ESH_LE_InitToWakeup. */
  if(BswM_GetGenericState(0) == BSWM_GENERICVALUE_ESH_State_ESH_INIT)
  {
    /* Return conditional action list BswM_ActionList_ESH_AL_InitToWakeup. */
    retVal = BSWM_ID_AL_ESH_AL_InitToWakeup;
  }
  /* No false action list configured. */
  return retVal;
}

/**********************************************************************************************************************
 *  BswM_Rule_ESH_PostRunToPrepNested
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_PostRunToPrepNested(void)
{
  BswM_HandleType retVal = BSWM_NO_ACTIONLIST;
  /* Evaluate logical expression ESH_LE_PostRunToPrep. */
  if(Request_ESH_PostRunRequest_0_requestedMode == RELEASED)
  {
    /* Return conditional action list BswM_ActionList_ESH_AL_PostRunToPrepShutdown. */
    retVal = BSWM_ID_AL_ESH_AL_PostRunToPrepShutdown;
  }
  /* No false action list configured. */
  return retVal;
}

/**********************************************************************************************************************
 *  BswM_Rule_ESH_PostRunNested
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_PostRunNested(void)
{
  BswM_HandleType retVal;
  /* Evaluate logical expression ESH_LE_RunRequestsOrWakeup. */ /* PRQA S 3415 1 */ /* MD_MSR_12.4 */
  if((Request_ESH_RunRequest_0_requestedMode == REQUESTED) || ((EcuM_GetValidatedWakeupEvents() != 0u) || (BswM_GetGenericState(2) == BSWM_GENERICVALUE_ESH_ComMPendingRequests_ESH_COMM_PENDING_REQUEST)))
  {
    /* Return conditional action list BswM_ActionList_ESH_AL_PostRunToRun. */
    retVal = BSWM_ID_AL_ESH_AL_PostRunToRun;
  }
  else
  {
    /* Return conditional action list BswM_ActionList_ESH_AL_ESH_PostRunToPrepCheck. */
    retVal = BSWM_ID_AL_ESH_AL_ESH_PostRunToPrepCheck;
  }
  return retVal;
}

/**********************************************************************************************************************
 *  BswM_Rule_ESH_PostRun
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_PostRun(void)
{
  BswM_HandleType retVal = BSWM_NO_ACTIONLIST;
  /* Evaluate logical expression ESH_LE_PostRun. */
  if((BswM_GetGenericState(0) == BSWM_GENERICVALUE_ESH_State_ESH_POST_RUN) && (BswM_Mode_Notification_ESH_ModeNotification_BswM_MDGP_ESH_Mode == RTE_MODE_ESH_Mode_POSTRUN))
  {
    /* Return conditional action list BswM_ActionList_ESH_AL_ExitPostRun. */
    retVal = BSWM_ID_AL_ESH_AL_ExitPostRun;
  }
  /* No false action list configured. */
  return retVal;
}

/**********************************************************************************************************************
 *  BswM_Rule_ESH_PrepToWait
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_ESH_PrepToWait(void)
{
  BswM_HandleType retVal = BSWM_NO_ACTIONLIST;
  /* Evaluate logical expression ESH_LE_PrepShutdownToWaitForNvM. */
  if((BswM_GetGenericState(0) == BSWM_GENERICVALUE_ESH_State_ESH_PREP_SHUTDOWN) && (BswM_Mode_Notification_ESH_ModeNotification_BswM_MDGP_ESH_Mode == RTE_MODE_ESH_Mode_SHUTDOWN))
  {
    /* Return conditional action list BswM_ActionList_ESH_AL_PrepShutdownToWaitForNvM. */
    retVal = BSWM_ID_AL_ESH_AL_PrepShutdownToWaitForNvM;
  }
  /* No false action list configured. */
  return retVal;
}

/**********************************************************************************************************************
 *  BswM_Rule_CC_CAN00_f26020e5_TX
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_CC_CAN00_f26020e5_TX(void)
{
  BswM_HandleType retVal = BSWM_NO_ACTIONLIST;
  /* Evaluate logical expression CC_LE_CAN00_f26020e5_TX. */
  if(BswM_GetCanSMChannelState(0) == CANSM_BSWM_FULL_COMMUNICATION)
  {
    if( BswM_GetRuleStates(BSWM_ID_RULE_CC_CAN00_f26020e5_TX) != BSWM_TRUE ) /* COV_BSWM_TRIGGEREDRULEEXECUTION */
    {
      BswM_UpdateRuleStates(BSWM_ID_RULE_CC_CAN00_f26020e5_TX, BSWM_TRUE);
      /* Return triggered action list BswM_ActionList_CC_AL_CAN00_f26020e5_TX_EnableNoinit. */
      retVal = BSWM_ID_AL_CC_AL_CAN00_f26020e5_TX_EnableNoinit;
    }
  }
  else
  {
    if( BswM_GetRuleStates(BSWM_ID_RULE_CC_CAN00_f26020e5_TX) != BSWM_FALSE ) /* COV_BSWM_TRIGGEREDRULEEXECUTION */
    {
      BswM_UpdateRuleStates(BSWM_ID_RULE_CC_CAN00_f26020e5_TX, BSWM_FALSE);
      /* Return triggered action list BswM_ActionList_CC_AL_CAN00_f26020e5_TX_Disable. */
      retVal = BSWM_ID_AL_CC_AL_CAN00_f26020e5_TX_Disable;
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 *  BswM_Rule_CC_CAN00_f26020e5_RX_DM
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_CC_CAN00_f26020e5_RX_DM(void)
{
  BswM_HandleType retVal = BSWM_NO_ACTIONLIST;
  /* Evaluate logical expression CC_LE_CAN00_f26020e5_RX_DM. */
  if(BswM_GetCanSMChannelState(0) == CANSM_BSWM_FULL_COMMUNICATION)
  {
    if( BswM_GetRuleStates(BSWM_ID_RULE_CC_CAN00_f26020e5_RX_DM) != BSWM_TRUE ) /* COV_BSWM_TRIGGEREDRULEEXECUTION */
    {
      BswM_UpdateRuleStates(BSWM_ID_RULE_CC_CAN00_f26020e5_RX_DM, BSWM_TRUE);
      /* Return triggered action list BswM_ActionList_CC_AL_CAN00_f26020e5_Enable_DM. */
      retVal = BSWM_ID_AL_CC_AL_CAN00_f26020e5_Enable_DM;
    }
  }
  else
  {
    if( BswM_GetRuleStates(BSWM_ID_RULE_CC_CAN00_f26020e5_RX_DM) != BSWM_FALSE ) /* COV_BSWM_TRIGGEREDRULEEXECUTION */
    {
      BswM_UpdateRuleStates(BSWM_ID_RULE_CC_CAN00_f26020e5_RX_DM, BSWM_FALSE);
      /* Return triggered action list BswM_ActionList_CC_AL_CAN00_f26020e5_Disable_DM. */
      retVal = BSWM_ID_AL_CC_AL_CAN00_f26020e5_Disable_DM;
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 *  BswM_Rule_CC_CAN00_f26020e5_RX
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_CC_CAN00_f26020e5_RX(void)
{
  BswM_HandleType retVal = BSWM_NO_ACTIONLIST;
  /* Evaluate logical expression CC_LE_CAN00_f26020e5_RX. */
  if(BswM_GetCanSMChannelState(0) != CANSM_BSWM_NO_COMMUNICATION)
  {
    if( BswM_GetRuleStates(BSWM_ID_RULE_CC_CAN00_f26020e5_RX) != BSWM_TRUE ) /* COV_BSWM_TRIGGEREDRULEEXECUTION */
    {
      BswM_UpdateRuleStates(BSWM_ID_RULE_CC_CAN00_f26020e5_RX, BSWM_TRUE);
      /* Return triggered action list BswM_ActionList_CC_AL_CAN00_f26020e5_RX_EnableNoinit. */
      retVal = BSWM_ID_AL_CC_AL_CAN00_f26020e5_RX_EnableNoinit;
    }
  }
  else
  {
    if( BswM_GetRuleStates(BSWM_ID_RULE_CC_CAN00_f26020e5_RX) != BSWM_FALSE ) /* COV_BSWM_TRIGGEREDRULEEXECUTION */
    {
      BswM_UpdateRuleStates(BSWM_ID_RULE_CC_CAN00_f26020e5_RX, BSWM_FALSE);
      /* Return triggered action list BswM_ActionList_CC_AL_CAN00_f26020e5_RX_Disable. */
      retVal = BSWM_ID_AL_CC_AL_CAN00_f26020e5_RX_Disable;
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 *  BswM_Rule_RuShutdownTimerStart
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_RuShutdownTimerStart(void)
{
  BswM_HandleType retVal = BSWM_NO_ACTIONLIST;
  /* Evaluate logical expression LeShutdownTimerStart. */
  if(Request_ESH_RunRequest_0_requestedMode == RELEASED)
  {
    if( BswM_GetRuleStates(BSWM_ID_RULE_RuShutdownTimerStart) != BSWM_TRUE ) /* COV_BSWM_TRIGGEREDRULEEXECUTION */
    {
      BswM_UpdateRuleStates(BSWM_ID_RULE_RuShutdownTimerStart, BSWM_TRUE);
      /* Return triggered action list BswM_ActionList_AlShutdownTimerStart. */
      retVal = BSWM_ID_AL_AlShutdownTimerStart;
    }
  }
  else
  {
    if( BswM_GetRuleStates(BSWM_ID_RULE_RuShutdownTimerStart) != BSWM_FALSE ) /* COV_BSWM_TRIGGEREDRULEEXECUTION */
    {
      BswM_UpdateRuleStates(BSWM_ID_RULE_RuShutdownTimerStart, BSWM_FALSE);
      /* Return triggered action list BswM_ActionList_AlShutdownTimerStop. */
      retVal = BSWM_ID_AL_AlShutdownTimerStop;
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 *  BswM_Rule_RuShutdownTimerExpired
 *********************************************************************************************************************/
BSWM_LOCAL FUNC(BswM_HandleType, BSWM_CODE) BswM_Rule_RuShutdownTimerExpired(void)
{
  BswM_HandleType retVal = BSWM_NO_ACTIONLIST;
  /* Evaluate logical expression LeShutdownTimerExpired. */
  if(BswM_GetTimerState(0) == BSWM_TIMER_EXPIRED)
  {
    if( BswM_GetRuleStates(BSWM_ID_RULE_RuShutdownTimerExpired) != BSWM_TRUE ) /* COV_BSWM_TRIGGEREDRULEEXECUTION */
    {
      BswM_UpdateRuleStates(BSWM_ID_RULE_RuShutdownTimerExpired, BSWM_TRUE);
      /* Return triggered action list BswM_ActionList_AlShutdownTimerExpired. */
      retVal = BSWM_ID_AL_AlShutdownTimerExpired;
    }
  }
  else
  {
    BswM_UpdateRuleStates(BSWM_ID_RULE_RuShutdownTimerExpired, BSWM_FALSE);
    /* No false action list configured. */
  }
  return retVal;
}



#define BSWM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

