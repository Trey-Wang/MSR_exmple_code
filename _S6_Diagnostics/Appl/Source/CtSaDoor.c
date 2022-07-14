/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *          File:  CtSaDoor.c
 *        Config:  D:/usr/usage/Presentations/MSR_Exercises/MSR_Exercises_AiP4_UPDATE/MSR_Exercises_AiP4/_S6_Diagnostics/MyECU.dpa
 *     SW-C Type:  CtSaDoor
 *  Generated at:  Tue Jun 26 09:53:39 2018
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
 * IdtDioValueType
 *   uint8 represents integers with a minimum value of 0 and a maximum value of 255.
 *      The order-relation on uint8 is: x < y if y - x is positive.
 *      uint8 has a lexical representation consisting of a finite-length sequence 
 *      of decimal digits (#x30-#x39).
 *      
 *      For example: 1, 0, 126, +10.
 *
 * IdtDoorState
 *   boolean has the value space required to support the mathematical concept of 
 *      binary-valued logic: {true, false}.
 *
 *********************************************************************************************************************/

#include "Rte_CtSaDoor.h" /* PRQA S 0857 */ /* MD_MSR_1.1_857 */


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
 *
 * Enumeration Types:
 * ==================
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
	
   IdtDioValueType localIoHwAbDoorState;
    /* Read in the sensor value from the hardware */
  (void)Rte_Call_PpDoorIoHwAb_ReadChannel(self, &localIoHwAbDoorState);

  if ( FALSE == localIoHwAbDoorState )
  {
    /* The sensor value read from the hardware goes to LOW / FALSE in case the door is closed. */
    /* As CMDOORSTATE_DOORCLOSED is defined in a top down way on application level, it may differ from the sensor value  */
    /* In order to decouple from the properties of the used sensor, you have to propagate the application level data type 
	/* defined in the CompuMethod of the ImplementationDataType */
    (void)Rte_Write_PpDoorState_DeDoorState(self, CMDOORSTATE_DOORCLOSED);
  }
  else
  {
    (void)Rte_Write_PpDoorState_DeDoorState(self, CMDOORSTATE_DOOROPEN);
  }

  

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}


#define CtSaDoor_STOP_SEC_CODE
#include "CtSaDoor_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


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
