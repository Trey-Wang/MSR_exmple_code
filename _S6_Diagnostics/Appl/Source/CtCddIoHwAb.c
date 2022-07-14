/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *          File:  CtCddIoHwAb.c
 *        Config:  D:/usr/usage/Presentations/MSR_Exercises/MSR_Exercises_AiP4_UPDATE/MSR_Exercises_AiP4/_S6_Diagnostics/MyECU.dpa
 *     SW-C Type:  CtCddIoHwAb
 *  Generated at:  Tue Jun 26 09:53:36 2018
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  C-Code implementation template for SW-C <CtCddIoHwAb>
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
 *********************************************************************************************************************/

#include "Rte_CtCddIoHwAb.h" /* PRQA S 0857 */ /* MD_MSR_1.1_857 */


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of include and declaration area >>        DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

#include "Dio.h"

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
 * uint8: Integer in interval [0...255] (standard type)
 *
 *********************************************************************************************************************/


#define CtCddIoHwAb_START_SEC_CODE
#include "CtCddIoHwAb_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtCddIoHwAb_PpDisplayState_ReadChannel
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <ReadChannel> of PortPrototype <PpDisplayState>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   void RCtCddIoHwAb_PpDisplayState_ReadChannel(IdtDioValueType *value)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtCddIoHwAb_PpDisplayState_ReadChannel_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtCddIoHwAb_CODE) RCtCddIoHwAb_PpDisplayState_ReadChannel(P2VAR(IdtDioValueType, AUTOMATIC, RTE_CTCDDIOHWAB_APPL_VAR) value) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtCddIoHwAb_PpDisplayState_ReadChannel
 *********************************************************************************************************************/

  *value = Dio_ReadChannel(DioConf_DioChannel_env_DisplayBusAwake);

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtCddIoHwAb_PpDisplayState_WriteChannel
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <WriteChannel> of PortPrototype <PpDisplayState>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   void RCtCddIoHwAb_PpDisplayState_WriteChannel(IdtDioValueType value)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtCddIoHwAb_PpDisplayState_WriteChannel_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtCddIoHwAb_CODE) RCtCddIoHwAb_PpDisplayState_WriteChannel(IdtDioValueType value) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtCddIoHwAb_PpDisplayState_WriteChannel
 *********************************************************************************************************************/

  Dio_WriteChannel(DioConf_DioChannel_env_DisplayBusAwake, value);

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtCddIoHwAb_PpDoorFrontLeft_ReadChannel
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <ReadChannel> of PortPrototype <PpDoorFrontLeft>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   void RCtCddIoHwAb_PpDoorFrontLeft_ReadChannel(IdtDioValueType *value)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtCddIoHwAb_PpDoorFrontLeft_ReadChannel_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtCddIoHwAb_CODE) RCtCddIoHwAb_PpDoorFrontLeft_ReadChannel(P2VAR(IdtDioValueType, AUTOMATIC, RTE_CTCDDIOHWAB_APPL_VAR) value) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtCddIoHwAb_PpDoorFrontLeft_ReadChannel
 *********************************************************************************************************************/

  *value = Dio_ReadChannel(DioConf_DioChannel_env_DoorFrontLeft);

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtCddIoHwAb_PpDoorFrontRight_ReadChannel
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <ReadChannel> of PortPrototype <PpDoorFrontRight>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   void RCtCddIoHwAb_PpDoorFrontRight_ReadChannel(IdtDioValueType *value)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtCddIoHwAb_PpDoorFrontRight_ReadChannel_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtCddIoHwAb_CODE) RCtCddIoHwAb_PpDoorFrontRight_ReadChannel(P2VAR(IdtDioValueType, AUTOMATIC, RTE_CTCDDIOHWAB_APPL_VAR) value) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtCddIoHwAb_PpDoorFrontRight_ReadChannel
 *********************************************************************************************************************/

  *value = Dio_ReadChannel(DioConf_DioChannel_env_DoorFrontRight);

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtCddIoHwAb_PpInteriorLightFrontDEFECT_ReadChannel
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <ReadChannel> of PortPrototype <PpInteriorLightFrontDEFECT>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   void RCtCddIoHwAb_PpInteriorLightFrontDEFECT_ReadChannel(IdtDioValueType *value)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtCddIoHwAb_PpInteriorLightFrontDEFECT_ReadChannel_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtCddIoHwAb_CODE) RCtCddIoHwAb_PpInteriorLightFrontDEFECT_ReadChannel(P2VAR(IdtDioValueType, AUTOMATIC, RTE_CTCDDIOHWAB_APPL_VAR) value) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtCddIoHwAb_PpInteriorLightFrontDEFECT_ReadChannel
 *********************************************************************************************************************/

  *value = Dio_ReadChannel(DioConf_DioChannel_env_InteriorLightFrontDEFECT);

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtCddIoHwAb_PpInteriorLightFront_WriteChannel
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <WriteChannel> of PortPrototype <PpInteriorLightFront>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   void RCtCddIoHwAb_PpInteriorLightFront_WriteChannel(IdtDioValueType value)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtCddIoHwAb_PpInteriorLightFront_WriteChannel_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtCddIoHwAb_CODE) RCtCddIoHwAb_PpInteriorLightFront_WriteChannel(IdtDioValueType value) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtCddIoHwAb_PpInteriorLightFront_WriteChannel
 *********************************************************************************************************************/

  Dio_WriteChannel(DioConf_DioChannel_env_InteriorLightFront, value);

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtCddIoHwAb_PpLocalClamp15_ReadChannel
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <ReadChannel> of PortPrototype <PpLocalClamp15>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   void RCtCddIoHwAb_PpLocalClamp15_ReadChannel(IdtDioValueType *value)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtCddIoHwAb_PpLocalClamp15_ReadChannel_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtCddIoHwAb_CODE) RCtCddIoHwAb_PpLocalClamp15_ReadChannel(P2VAR(IdtDioValueType, AUTOMATIC, RTE_CTCDDIOHWAB_APPL_VAR) value) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtCddIoHwAb_PpLocalClamp15_ReadChannel
 *********************************************************************************************************************/

  *value = Dio_ReadChannel(DioConf_DioChannel_env_LocalClamp15);

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}

/**********************************************************************************************************************
 *
 * Runnable Entity Name: RCtCddIoHwAb_PpLocalClamp15_WriteChannel
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered by server invocation for OperationPrototype <WriteChannel> of PortPrototype <PpLocalClamp15>
 *
 **********************************************************************************************************************
 *
 * Runnable prototype:
 * ===================
 *   void RCtCddIoHwAb_PpLocalClamp15_WriteChannel(IdtDioValueType value)
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of documentation area >>                  DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtCddIoHwAb_PpLocalClamp15_WriteChannel_doc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of documentation area >>                    DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/

FUNC(void, CtCddIoHwAb_CODE) RCtCddIoHwAb_PpLocalClamp15_WriteChannel(IdtDioValueType value) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: RCtCddIoHwAb_PpLocalClamp15_WriteChannel
 *********************************************************************************************************************/

  Dio_WriteChannel(DioConf_DioChannel_env_LocalClamp15, value);

/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
}


#define CtCddIoHwAb_STOP_SEC_CODE
#include "CtCddIoHwAb_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


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
