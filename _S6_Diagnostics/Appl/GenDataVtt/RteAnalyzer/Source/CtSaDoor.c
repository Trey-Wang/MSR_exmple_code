/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *          File:  CtSaDoor.c
 *        Config:  MyECU.dpa
 *     SW-C Type:  CtSaDoor
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  C-Code implementation template for SW-C <CtSaDoor>
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of version logging area >>                DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

/* PRQA S 0777, 0779 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779 */
/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* PRQA S 0614 EOF */ /* MD_Rte_TestCode */

/* PRQA S 3109 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3112 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3197 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3198 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3199 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3201 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3203 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3205 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3206 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3218 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3229 EOF */ /* MD_Rte_TestCode */
/* PRQA S 2002 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3334 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3417 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3426 EOF */ /* MD_Rte_TestCode */
/* PRQA S 3453 EOF */ /* MD_Rte_TestCode */

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of version logging area >>                  DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *
 * AUTOSAR Modelling Object Descriptions
 *
 **********************************************************************************************************************
 *
 * Data Types:
 * ===========
 * BswM_ESH_Mode
 *   
 *
 * IdtDioValueType
 *   
 *
 * IdtDoorState
 *   
 *
 *********************************************************************************************************************/

#include "Rte_CtSaDoor.h" /* PRQA S 0857 */ /* MD_MSR_1.1_857 */
#include "TSC_CtSaDoor.h"


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of include and declaration area >>        DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

#include "string.h"

static void CtSaDoor_TestDefines(void);


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of include and declaration area >>          DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *
 * Used AUTOSAR Data Types
 *
 **********************************************************************************************************************
 *
 * Primitive Types:
 * ================
 * IdtDioValueType: Integer in interval [0...255]
 * boolean: Boolean (standard type)
 * uint8: Integer in interval [0...255] (standard type)
 * uint8: Integer in interval [0...255] (standard type)
 *
 * Enumeration Types:
 * ==================
 * BswM_ESH_Mode: Enumeration of integer in interval [0...255] with enumerators
 *   STARTUP (0U)
 *   RUN (1U)
 *   POSTRUN (2U)
 *   WAKEUP (3U)
 *   SHUTDOWN (4U)
 * IdtDoorState: Enumeration of integer in interval [0...1] with enumerators
 *   CMDOORSTATE_DOORCLOSED (0U)
 *   CMDOORSTATE_DOOROPEN (1U)
 *
 *********************************************************************************************************************/


#define CtSaDoor_START_SEC_CODE
#include "CtSaDoor_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtSaDoorReadDoor
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered on TimingEvent every 200ms
 *
 **********************************************************************************************************************
 *
 * Output Interfaces:
 * ==================
 *   Explicit S/R API:
 *   -----------------
 *   Std_ReturnType Rte_Write_PpDoorState_DeDoorState(Rte_Instance self, IdtDoorState data)
 *
 * Client/Server Interfaces:
 * =========================
 *   Server Invocation:
 *   ------------------
 *   Std_ReturnType Rte_Call_PpDoorIoHwAb_ReadChannel(Rte_Instance self, IdtDioValueType *value)
 *     Synchronous Server Invocation. Timeout: None
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtSaDoorReadDoor_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtSaDoor_CODE) RCtSaDoorReadDoor(Rte_Instance self) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtSaDoorReadDoor
 *********************************************************************************************************************/

  Std_ReturnType fct_status;
  boolean fct_error = 0;

  IdtDioValueType Call_PpDoorIoHwAb_ReadChannel_value = 0U;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  fct_status = TSC_CtSaDoor_Rte_Write_PpDoorState_DeDoorState(self, Rte_InitValue_PpDoorState_DeDoorState);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }

  fct_status = TSC_CtSaDoor_Rte_Call_PpDoorIoHwAb_ReadChannel(self, &Call_PpDoorIoHwAb_ReadChannel_value);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
    case RTE_E_UNCONNECTED:
      fct_error = 1;
      break;
    case RTE_E_TIMEOUT:
      fct_error = 1;
      break;
  }

  CtSaDoor_TestDefines();


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}


#define CtSaDoor_STOP_SEC_CODE
#include "CtSaDoor_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of function definition area >>            DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

static void CtSaDoor_TestDefines(void)
{
  /* Enumeration Data Types */

  BswM_ESH_Mode Test_BswM_ESH_Mode_V_1 = STARTUP;
  BswM_ESH_Mode Test_BswM_ESH_Mode_V_2 = RUN;
  BswM_ESH_Mode Test_BswM_ESH_Mode_V_3 = POSTRUN;
  BswM_ESH_Mode Test_BswM_ESH_Mode_V_4 = WAKEUP;
  BswM_ESH_Mode Test_BswM_ESH_Mode_V_5 = SHUTDOWN;

  IdtDoorState Test_IdtDoorState_V_1 = CMDOORSTATE_DOORCLOSED;
  IdtDoorState Test_IdtDoorState_V_2 = CMDOORSTATE_DOOROPEN;

  /* Modes */

  uint8 Test_ComMMode_MV_1 = RTE_MODE_ComMMode_COMM_FULL_COMMUNICATION;
  uint8 Test_ComMMode_MV_2 = RTE_MODE_ComMMode_COMM_NO_COMMUNICATION;
  uint8 Test_ComMMode_MV_3 = RTE_MODE_ComMMode_COMM_SILENT_COMMUNICATION;
  uint8 Test_ComMMode_TV = RTE_TRANSITION_ComMMode;

  BswM_ESH_Mode Test_ESH_Mode_MV_1 = RTE_MODE_ESH_Mode_POSTRUN;
  BswM_ESH_Mode Test_ESH_Mode_MV_2 = RTE_MODE_ESH_Mode_RUN;
  BswM_ESH_Mode Test_ESH_Mode_MV_3 = RTE_MODE_ESH_Mode_SHUTDOWN;
  BswM_ESH_Mode Test_ESH_Mode_MV_4 = RTE_MODE_ESH_Mode_STARTUP;
  BswM_ESH_Mode Test_ESH_Mode_MV_5 = RTE_MODE_ESH_Mode_WAKEUP;
  BswM_ESH_Mode Test_ESH_Mode_TV = RTE_TRANSITION_ESH_Mode;
}

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of function definition area >>              DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of removed code area >>                   DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of removed code area >>                     DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

/**********************************************************************************************************************
 MISRA 2004 violations and justifications
 *********************************************************************************************************************/

/* module specific MISRA deviations:
   MD_Rte_TestCode:
     Reason:     This justification is used within the generated test code by the Rte Analyzer.
     Risk:       No functional risk.
     Prevention: Not required.

*/
