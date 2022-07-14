/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *          File:  CtSaInteriorLight.c
 *        Config:  MyECU.dpa
 *     SW-C Type:  CtSaInteriorLight
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  C-Code implementation template for SW-C <CtSaInteriorLight>
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
 * Dem_EventStatusType
 *   
 *
 * IdtDioValueType
 *   
 *
 * IdtLightState
 *   
 *
 *********************************************************************************************************************/

#include "Rte_CtSaInteriorLight.h" /* PRQA S 0857 */ /* MD_MSR_1.1_857 */
#include "TSC_CtSaInteriorLight.h"


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of include and declaration area >>        DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

#include "string.h"

static void CtSaInteriorLight_TestDefines(void);


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
 * Dem_EventStatusType: Enumeration of integer in interval [0...255] with enumerators
 *   DEM_EVENT_STATUS_PASSED (0U)
 *   DEM_EVENT_STATUS_FAILED (1U)
 *   DEM_EVENT_STATUS_PREPASSED (2U)
 *   DEM_EVENT_STATUS_PREFAILED (3U)
 *   DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED (4U)
 *   DEM_EVENT_STATUS_PASSED_CONDITIONS_NOT_FULFILLED (5U)
 *   DEM_EVENT_STATUS_FAILED_CONDITIONS_NOT_FULFILLED (6U)
 *   DEM_EVENT_STATUS_PREPASSED_CONDITIONS_NOT_FULFILLED (7U)
 *   DEM_EVENT_STATUS_PREFAILED_CONDITIONS_NOT_FULFILLED (8U)
 * IdtLightState: Enumeration of integer in interval [0...1] with enumerators
 *   CMLIGHTSTATE_LIGHTOFF (0U)
 *   CMLIGHTSTATE_LIGHTON (1U)
 *
 *********************************************************************************************************************/


#define CtSaInteriorLight_START_SEC_CODE
#include "CtSaInteriorLight_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtSaInteriorLightSwitchLight
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered on DataReceivedEvent for DataElementPrototype <DeLightState> of PortPrototype <PpLightState>
 *
 **********************************************************************************************************************
 *
 * Input Interfaces:
 * =================
 *   Explicit S/R API:
 *   -----------------
 *   Std_ReturnType Rte_Read_PpLightState_DeLightState(Rte_Instance self, IdtLightState *data)
 *
 * Client/Server Interfaces:
 * =========================
 *   Server Invocation:
 *   ------------------
 *   Std_ReturnType Rte_Call_PpInteriorLightDefect_ReadChannel(Rte_Instance self, IdtDioValueType *value)
 *     Synchronous Server Invocation. Timeout: None
 *   Std_ReturnType Rte_Call_PpInteriorLightIoHwAb_WriteChannel(Rte_Instance self, IdtDioValueType value)
 *     Synchronous Server Invocation. Timeout: None
 *
 * Service Calls:
 * ==============
 *   Service Invocation:
 *   -------------------
 *   Std_ReturnType Rte_Call_Event_DTC_0x000002_SetEventStatus(Rte_Instance self, Dem_EventStatusType EventStatus)
 *     Synchronous Service Invocation. Timeout: None
 *     Returned Application Errors: RTE_E_DiagnosticMonitor_E_NOT_OK
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtSaInteriorLightSwitchLight_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtSaInteriorLight_CODE) RCtSaInteriorLightSwitchLight(Rte_Instance self) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtSaInteriorLightSwitchLight
 *********************************************************************************************************************/

  Std_ReturnType fct_status;
  boolean fct_error = 0;

  IdtLightState Read_PpLightState_DeLightState;

  IdtDioValueType Call_PpInteriorLightDefect_ReadChannel_value = 0U;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  fct_status = TSC_CtSaInteriorLight_Rte_Read_PpLightState_DeLightState(self, &Read_PpLightState_DeLightState);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
    case RTE_E_UNCONNECTED:
      fct_error = 1;
      break;
    case RTE_E_NEVER_RECEIVED:
      fct_error = 1;
      break;
    case RTE_E_INVALID:
      fct_error = 1;
      break;
    case RTE_E_MAX_AGE_EXCEEDED:
      fct_error = 1;
      break;
  }

  fct_status = TSC_CtSaInteriorLight_Rte_Call_PpInteriorLightDefect_ReadChannel(self, &Call_PpInteriorLightDefect_ReadChannel_value);
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

  fct_status = TSC_CtSaInteriorLight_Rte_Call_PpInteriorLightIoHwAb_WriteChannel(self, 0U);
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

  fct_status = TSC_CtSaInteriorLight_Rte_Call_Event_DTC_0x000002_SetEventStatus(self, 0U);
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
    case RTE_E_DiagnosticMonitor_E_NOT_OK:
      fct_error = 1;
      break;
  }

  CtSaInteriorLight_TestDefines();


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}


#define CtSaInteriorLight_STOP_SEC_CODE
#include "CtSaInteriorLight_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of function definition area >>            DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

static void CtSaInteriorLight_TestDefines(void)
{
  /* Enumeration Data Types */

  BswM_ESH_Mode Test_BswM_ESH_Mode_V_1 = STARTUP;
  BswM_ESH_Mode Test_BswM_ESH_Mode_V_2 = RUN;
  BswM_ESH_Mode Test_BswM_ESH_Mode_V_3 = POSTRUN;
  BswM_ESH_Mode Test_BswM_ESH_Mode_V_4 = WAKEUP;
  BswM_ESH_Mode Test_BswM_ESH_Mode_V_5 = SHUTDOWN;

  Dem_EventStatusType Test_Dem_EventStatusType_V_1 = DEM_EVENT_STATUS_PASSED;
  Dem_EventStatusType Test_Dem_EventStatusType_V_2 = DEM_EVENT_STATUS_FAILED;
  Dem_EventStatusType Test_Dem_EventStatusType_V_3 = DEM_EVENT_STATUS_PREPASSED;
  Dem_EventStatusType Test_Dem_EventStatusType_V_4 = DEM_EVENT_STATUS_PREFAILED;
  Dem_EventStatusType Test_Dem_EventStatusType_V_5 = DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED;
  Dem_EventStatusType Test_Dem_EventStatusType_V_6 = DEM_EVENT_STATUS_PASSED_CONDITIONS_NOT_FULFILLED;
  Dem_EventStatusType Test_Dem_EventStatusType_V_7 = DEM_EVENT_STATUS_FAILED_CONDITIONS_NOT_FULFILLED;
  Dem_EventStatusType Test_Dem_EventStatusType_V_8 = DEM_EVENT_STATUS_PREPASSED_CONDITIONS_NOT_FULFILLED;
  Dem_EventStatusType Test_Dem_EventStatusType_V_9 = DEM_EVENT_STATUS_PREFAILED_CONDITIONS_NOT_FULFILLED;

  IdtLightState Test_IdtLightState_V_1 = CMLIGHTSTATE_LIGHTOFF;
  IdtLightState Test_IdtLightState_V_2 = CMLIGHTSTATE_LIGHTON;

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
