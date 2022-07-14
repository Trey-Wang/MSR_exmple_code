/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *          File:  CtSaInteriorLight.c
 *        Config:  D:/usr/usage/Presentations/MSR_Exercises/MSR_Exercises_AiP4_UPDATE/MSR_Exercises_AiP4/_S6_Diagnostics/MyECU.dpa
 *     SW-C Type:  CtSaInteriorLight
 *  Generated at:  Tue Jun 26 09:53:41 2018
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
 * Dem_EventStatusType
 *   uint8 represents integers with a minimum value of 0 and a maximum value of 255.
 *      The order-relation on uint8 is: x < y if y - x is positive.
 *      uint8 has a lexical representation consisting of a finite-length sequence 
 *      of decimal digits (#x30-#x39).
 *      
 *      For example: 1, 0, 126, +10.
 *
 * IdtDioValueType
 *   uint8 represents integers with a minimum value of 0 and a maximum value of 255.
 *      The order-relation on uint8 is: x < y if y - x is positive.
 *      uint8 has a lexical representation consisting of a finite-length sequence 
 *      of decimal digits (#x30-#x39).
 *      
 *      For example: 1, 0, 126, +10.
 *
 * IdtLightState
 *   boolean has the value space required to support the mathematical concept of 
 *      binary-valued logic: {true, false}.
 *
 *********************************************************************************************************************/

#include "Rte_CtSaInteriorLight.h" /* PRQA S 0857 */ /* MD_MSR_1.1_857 */


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of include and declaration area >>        DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/


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

	IdtLightState     localLightState;
	IdtDioValueType   localLightDefectState;

	/* Read new light state, to be set. */
	(void)Rte_Read_PpLightState_DeLightState(self, &localLightState);

	/* Check if front light is currently defect or not. */
	(void)Rte_Call_PpInteriorLightDefect_ReadChannel(self, &localLightDefectState);

	/* Check if front light is defect. */
	if (localLightDefectState == TRUE)
	{
		/* Inform Dem about the defect -> DEM_EVENT_STATUS_FAILED */
		(void)Rte_Call_Event_DTC_0x000002_SetEventStatus(self, DEM_EVENT_STATUS_FAILED);

		/* Front light is defect. So the light is always off, beacause we will not set it under voltage in this case. */
		(void)Rte_Call_PpInteriorLightIoHwAb_WriteChannel(self, (IdtDioValueType)FALSE);
	}
	else
	{
		/* Inform Dem about no defect -> DEM_EVENT_STATUS_PASSED */
		(void)Rte_Call_Event_DTC_0x000002_SetEventStatus(self, DEM_EVENT_STATUS_PASSED);

		if (CMLIGHTSTATE_LIGHTOFF == localLightState)
		{
			/* The value FALSE is defined to be a light off. */
			/* As CMLIGHTSTATE_LIGHTOFF might either be TRUE or FALSE, use the define instead of using localLightState directly. */
			/* This decouples the SWCs from each other. */
			(void)Rte_Call_PpInteriorLightIoHwAb_WriteChannel(self, (IdtDioValueType)FALSE);
		}
		else
		{
			(void)Rte_Call_PpInteriorLightIoHwAb_WriteChannel(self, (IdtDioValueType)TRUE);
		}
	}
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}


#define CtSaInteriorLight_STOP_SEC_CODE
#include "CtSaInteriorLight_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of function definition area >>            DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of function definition area >>              DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of removed code area >>                   DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of removed code area >>                     DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
