/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *          File:  CtSaDisplay.c
 *        Config:  MyECU.dpa
 *     SW-C Type:  CtSaDisplay
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  C-Code implementation template for SW-C <CtSaDisplay>
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
 *********************************************************************************************************************/

#include "Rte_CtSaDisplay.h" /* PRQA S 0857 */ /* MD_MSR_1.1_857 */
#include "TSC_CtSaDisplay.h"


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of include and declaration area >>        DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

#include "string.h"

static void CtSaDisplay_TestDefines(void);


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
 * uint32: Integer in interval [0...4294967295] (standard type)
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
 * IdtDisplayIgnitionState: Enumeration of integer in interval [0...255] with enumerators
 *   CMIGNITION_STATEON (0U)
 *   CMIGNITION_STATEOFF (1U)
 *
 *********************************************************************************************************************/


#define CtSaDisplay_START_SEC_CODE
#include "CtSaDisplay_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtApDisplayBusStateHandling
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered on DataReceivedEvent for DataElementPrototype <DeDisplayBusState> of PortPrototype <PpDisplayState>
 *
 **********************************************************************************************************************
 *
 * Input Interfaces:
 * =================
 *   Explicit S/R API:
 *   -----------------
 *   Std_ReturnType Rte_Read_PpDisplayState_DeDisplayBusState(boolean *data)
 *
 * Client/Server Interfaces:
 * =========================
 *   Server Invocation:
 *   ------------------
 *   Std_ReturnType Rte_Call_PpDisplayStateIoHwAb_WriteChannel(IdtDioValueType value)
 *     Synchronous Server Invocation. Timeout: None
 *   Std_ReturnType Rte_Call_PplocalIgnitionStateIoHwAb_ReadChannel(IdtDioValueType *value)
 *     Synchronous Server Invocation. Timeout: None
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApDisplayBusStateHandling_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtSaDisplay_CODE) RCtApDisplayBusStateHandling(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApDisplayBusStateHandling
 *********************************************************************************************************************/

  Std_ReturnType fct_status;
  boolean fct_error = 0;

  boolean Read_PpDisplayState_DeDisplayBusState;

  IdtDioValueType Call_PplocalIgnitionStateIoHwAb_ReadChannel_value = 0U;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  fct_status = TSC_CtSaDisplay_Rte_Read_PpDisplayState_DeDisplayBusState(&Read_PpDisplayState_DeDisplayBusState);
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

  fct_status = TSC_CtSaDisplay_Rte_Call_PpDisplayStateIoHwAb_WriteChannel(0U);
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

  fct_status = TSC_CtSaDisplay_Rte_Call_PplocalIgnitionStateIoHwAb_ReadChannel(&Call_PplocalIgnitionStateIoHwAb_ReadChannel_value);
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

  CtSaDisplay_TestDefines();


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtApDisplayLightOnOffHandling
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered on DataReceivedEvent for DataElementPrototype <DeLightOnOffCounter> of PortPrototype <PpDisplayState>
 *
 **********************************************************************************************************************
 *
 * Input Interfaces:
 * =================
 *   Explicit S/R API:
 *   -----------------
 *   Std_ReturnType Rte_Read_PpDisplayState_DeLightOnOffCounter(uint32 *data)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApDisplayLightOnOffHandling_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtSaDisplay_CODE) RCtApDisplayLightOnOffHandling(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApDisplayLightOnOffHandling
 *********************************************************************************************************************/

  Std_ReturnType fct_status;
  boolean fct_error = 0;

  uint32 Read_PpDisplayState_DeLightOnOffCounter;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  fct_status = TSC_CtSaDisplay_Rte_Read_PpDisplayState_DeLightOnOffCounter(&Read_PpDisplayState_DeLightOnOffCounter);
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


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtApDisplayOdometerValue
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered on DataReceivedEvent for DataElementPrototype <DeOdometerValue> of PortPrototype <PpDisplayState>
 *
 **********************************************************************************************************************
 *
 * Input Interfaces:
 * =================
 *   Explicit S/R API:
 *   -----------------
 *   Std_ReturnType Rte_Read_PpDisplayState_DeOdometerValue(uint32 *data)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApDisplayOdometerValue_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtSaDisplay_CODE) RCtApDisplayOdometerValue(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApDisplayOdometerValue
 *********************************************************************************************************************/

  Std_ReturnType fct_status;
  boolean fct_error = 0;

  uint32 Read_PpDisplayState_DeOdometerValue;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  fct_status = TSC_CtSaDisplay_Rte_Read_PpDisplayState_DeOdometerValue(&Read_PpDisplayState_DeOdometerValue);
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


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtApDisplayOdometerWriteRequestPending
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered on DataReceivedEvent for DataElementPrototype <DeOdometerWriteRequestPending> of PortPrototype <PpDisplayState>
 *
 **********************************************************************************************************************
 *
 * Input Interfaces:
 * =================
 *   Explicit S/R API:
 *   -----------------
 *   Std_ReturnType Rte_Read_PpDisplayState_DeOdometerWriteRequestPending(boolean *data)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApDisplayOdometerWriteRequestPending_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtSaDisplay_CODE) RCtApDisplayOdometerWriteRequestPending(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApDisplayOdometerWriteRequestPending
 *********************************************************************************************************************/

  Std_ReturnType fct_status;
  boolean fct_error = 0;

  boolean Read_PpDisplayState_DeOdometerWriteRequestPending;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  fct_status = TSC_CtSaDisplay_Rte_Read_PpDisplayState_DeOdometerWriteRequestPending(&Read_PpDisplayState_DeOdometerWriteRequestPending);
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


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtApDisplayReadIgnitionSignal
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
 *   Std_ReturnType Rte_Write_PpLocalIgnitionState_DeIgnitionState(IdtDisplayIgnitionState data)
 *
 * Client/Server Interfaces:
 * =========================
 *   Server Invocation:
 *   ------------------
 *   Std_ReturnType Rte_Call_PplocalIgnitionStateIoHwAb_ReadChannel(IdtDioValueType *value)
 *     Synchronous Server Invocation. Timeout: None
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApDisplayReadIgnitionSignal_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtSaDisplay_CODE) RCtApDisplayReadIgnitionSignal(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtApDisplayReadIgnitionSignal
 *********************************************************************************************************************/

  Std_ReturnType fct_status;
  boolean fct_error = 0;

  IdtDioValueType Call_PplocalIgnitionStateIoHwAb_ReadChannel_value = 0U;

  /*************************************************
  * Direct Function Accesses
  *************************************************/

  fct_status = TSC_CtSaDisplay_Rte_Write_PpLocalIgnitionState_DeIgnitionState(Rte_InitValue_PpLocalIgnitionState_DeIgnitionState);
  switch (fct_status)
  {
    case RTE_E_OK:
      fct_error = 0;
      break;
  }

  fct_status = TSC_CtSaDisplay_Rte_Call_PplocalIgnitionStateIoHwAb_ReadChannel(&Call_PplocalIgnitionStateIoHwAb_ReadChannel_value);
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


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}


#define CtSaDisplay_STOP_SEC_CODE
#include "CtSaDisplay_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of function definition area >>            DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

static void CtSaDisplay_TestDefines(void)
{
  /* Enumeration Data Types */

  BswM_ESH_Mode Test_BswM_ESH_Mode_V_1 = STARTUP;
  BswM_ESH_Mode Test_BswM_ESH_Mode_V_2 = RUN;
  BswM_ESH_Mode Test_BswM_ESH_Mode_V_3 = POSTRUN;
  BswM_ESH_Mode Test_BswM_ESH_Mode_V_4 = WAKEUP;
  BswM_ESH_Mode Test_BswM_ESH_Mode_V_5 = SHUTDOWN;

  IdtDisplayIgnitionState Test_IdtDisplayIgnitionState_V_1 = CMIGNITION_STATEON;
  IdtDisplayIgnitionState Test_IdtDisplayIgnitionState_V_2 = CMIGNITION_STATEOFF;

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
