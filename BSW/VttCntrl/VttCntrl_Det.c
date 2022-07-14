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
/**        \file  VTTCntrl_Det.c
 *        \brief  Initialization of Det Errors for VTT.
 *
 *      \details  In a VTT environment DET Errors are reported by using the CANoeAPI. For ease of use the Ids 
 *                of (hex values) BSW modules, functions and errors are replaced by plain text. 
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Patrick Markl                 visml         Vector Informatik GmbH
 *  Wladimir Gerber               viswge        Vector Informatik GmbH
 *  Damian Philipp                visdap        Vector Informatik GmbH
 *  Max-Ferdinand Suffel          vismsl        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2014-10-14  viswge  -             Initial version.
 *  01.00.01  2014-11-01  viswge  -             Added FrIf.
 *  01.00.02  2014-12-15  viswge  -             Added void to function signature to reduce compiler warnings.
 *  02.00.00  2015-01-07  viswge  -             FEAT254: Rename of VIP to VTT.
 *  02.00.01  2015-04-02  visdap  -             Refactoring: Moved non-det-related functionality to base
 *  02.00.02  2015-04-15  viswge  -             Added VTTCntrl_Compiler_Cfg.inc.
 *  03.00.01  2015-07-15  visdap  -             Update for R13. ESCAN00083479.
 *  03.00.02  2015-08-20  vismsl  -             Updated for R13 based on new TechRefs.
 *  04.00.00  2015-12-11  vismsl  -             Updated for R14 based on new TechRefs.
*********************************************************************************************************************/

#define VTTCNTRL_DET

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
 
#include "VTTCntrl_Det.h" 
#include "CANoeAPI.h" 

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/

/* vendor specific version information is BCD coded */
#if (  (VTTCNTRL_DET_MAJOR_VERSION != (0x04)) \
    || (VTTCNTRL_DET_MINOR_VERSION != (0x00)) \
    || (VTTCNTRL_DET_PATCH_VERSION != (0x00)) )
  #error "Vendor specific version numbers of VTTCntrl_Det.c and VTTCntrl_Det.h are inconsistent"
#endif

/**********************************************************************************************************************
*  LOCAL CONSTANT MACROS
**********************************************************************************************************************/
#if !defined (STATIC)
# define STATIC static
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
/* Initialize DET codes for module Adc*/
STATIC void VTTCntrl_DetInitialization_Adc(void)
{
  CANoeAPI_DetSetModule(123, "Adc");
  CANoeAPI_DetSetFunction(123, 0x00, "Adc_Init");
  CANoeAPI_DetSetFunction(123, 0x05, "Adc_EnableHardwareTrigger");
  CANoeAPI_DetSetFunction(123, 0x0A, "Adc_GetVersionInfo");
  CANoeAPI_DetSetFunction(123, 0x0C, "Adc_SetupResultBuffer");
  CANoeAPI_DetSetFunction(123, 0x08, "Adc_DisableGroupNotification");
  CANoeAPI_DetSetFunction(123, 0x06, "Adc_DisableHardwareTrigger");
  CANoeAPI_DetSetFunction(123, 0x04, "Adc_ReadGroup");
  CANoeAPI_DetSetFunction(123, 0x02, "Adc_StartGroupConversion");
  CANoeAPI_DetSetFunction(123, 0x07, "Adc_EnableGroupNotification");
  CANoeAPI_DetSetFunction(123, 0x03, "Adc_StopGroupConversion");
  CANoeAPI_DetSetFunction(123, 0x01, "Adc_DeInit");
  CANoeAPI_DetSetFunction(123, 0x0B, "Adc_GetStreamLastPointer");
  CANoeAPI_DetSetFunction(123, 0x09, "Adc_GetGroupStatus");
  CANoeAPI_DetSetError(123, 0x0C, "ADC_E_IDLE");
  CANoeAPI_DetSetError(123, 0x0A, "ADC_E_UNINIT");
  CANoeAPI_DetSetError(123, 0x1C, "ADC_E_PARAM_STREAM_PTR");
  CANoeAPI_DetSetError(123, 0x0B, "ADC_E_BUSY");
  CANoeAPI_DetSetError(123, 0x0D, "ADC_E_ALREADY_INITIALIZED");
  CANoeAPI_DetSetError(123, 0x19, "ADC_E_BUFFER_UNINIT");
  CANoeAPI_DetSetError(123, 0x0E, "ADC_E_PARAM_CONFIG");
  CANoeAPI_DetSetError(123, 0x16, "ADC_E_WRONG_CONV_MODE");
  CANoeAPI_DetSetError(123, 0x1B, "ADC_E_PARAM_RESULT_BUFFER");
  CANoeAPI_DetSetError(123, 0x15, "ADC_E_PARAM_GROUP");
  CANoeAPI_DetSetError(123, 0x18, "ADC_E_NOTIF_CAPABILITY");
  CANoeAPI_DetSetError(123, 0x1D, "ADC_E_PARAM_VINFO");
  CANoeAPI_DetSetError(123, 0x1A, "ADC_E_PARAM_READ_BUFFER");
  CANoeAPI_DetSetError(123, 0x17, "ADC_E_WRONG_TRIGG_SRC");
}

/* Initialize DET codes for module BswM*/
STATIC void VTTCntrl_DetInitialization_BswM(void)
{
  CANoeAPI_DetSetModule(42, "BswM");
  CANoeAPI_DetSetFunction(42, 0x88, "BswM_PduR_TpStartOfReception");
  CANoeAPI_DetSetFunction(42, 0x21, "BswM_Sd_ConsumedEventGroupCurrentState");
  CANoeAPI_DetSetFunction(42, 0x1b, "BswM_J1939DcmBroadcastStatus");
  CANoeAPI_DetSetFunction(42, 0x05, "BswM_CanSM_CurrentState");
  CANoeAPI_DetSetFunction(42, 0x00, "BswM_Init");
  CANoeAPI_DetSetFunction(42, 0x1f, "BswM_Sd_ClientServiceCurrentState");
  CANoeAPI_DetSetFunction(42, 0x17, "BswM_NvM_CurrentJobMode");
  CANoeAPI_DetSetFunction(42, 0x01, "BswM_GetVersionInfo");
  CANoeAPI_DetSetFunction(42, 0x85, "BswM_LinSM_ScheduleEndNotification");
  CANoeAPI_DetSetFunction(42, 0x03, "BswM_MainFunction");
  CANoeAPI_DetSetFunction(42, 0x84, "BswM_RuleControl");
  CANoeAPI_DetSetFunction(42, 0x86, "BswM_PduR_RxIndication");
  CANoeAPI_DetSetFunction(42, 0x18, "BswM_J1939Nm_StateChangeNotification");
  CANoeAPI_DetSetFunction(42, 0x0A, "BswM_LinSM_CurrentSchedule");
  CANoeAPI_DetSetFunction(42, 0x15, "BswM_ComM_CurrentPNCMode");
  CANoeAPI_DetSetFunction(42, 0x14, "BswM_Dcm_ApplicationUpdated");
  CANoeAPI_DetSetFunction(42, 0x87, "BswM_PduR_TpRxIndication");
  CANoeAPI_DetSetFunction(42, 0x8B, "BswM_PduR_TxConfirmation");
  CANoeAPI_DetSetFunction(42, 0x11, "BswM_WdgM_RequestPartitionReset");
  CANoeAPI_DetSetFunction(42, 0x83, "BswM_Read_SWCModeRequestName");
  CANoeAPI_DetSetFunction(42, 0x82, "BswM_Notification_SWCNotificationName");
  CANoeAPI_DetSetFunction(42, 0x8A, "BswM_PduR_Transmit");
  CANoeAPI_DetSetFunction(42, 0x0D, "BswM_EthSM_CurrentState");
  CANoeAPI_DetSetFunction(42, 0x06, "BswM_Dcm_CommunicationMode_CurrentState");
  CANoeAPI_DetSetFunction(42, 0x10, "BswM_EcuM_CurrentWakeup");
  CANoeAPI_DetSetFunction(42, 0x02, "BswM_RequestMode");
  CANoeAPI_DetSetFunction(42, 0x04, "BswM_Deinit");
  CANoeAPI_DetSetFunction(42, 0x0C, "BswM_FrSM_CurrentState");
  CANoeAPI_DetSetFunction(42, 0x80, "BswM_InitMemory");
  CANoeAPI_DetSetFunction(42, 0x09, "BswM_LinSM_CurrentState");
  CANoeAPI_DetSetFunction(42, 0x0E, "BswM_ComM_CurrentMode");
  CANoeAPI_DetSetFunction(42, 0x89, "BswM_PduR_TpTxConfirmation");
  CANoeAPI_DetSetFunction(42, 0x81, "BswM_Nm_StateChangeNotification");
  CANoeAPI_DetSetFunction(42, 0x16, "BswM_NvM_CurrentBlockMode");
  CANoeAPI_DetSetFunction(42, 0x0F, "BswM_EcuM_CurrentState");
  CANoeAPI_DetSetFunction(42, 0x0B, "BswM_LinTp_RequestMode");
  CANoeAPI_DetSetFunction(42, 0x23, "BswM_EcuM_RequestedState");
  CANoeAPI_DetSetFunction(42, 0x20, "BswM_Sd_EventHandlerCurrentState");
  CANoeAPI_DetSetError(42, 0x03, "BSWM_E_PARAM_INVALID");
  CANoeAPI_DetSetError(42, 0x05, "BSWM_E_REQ_MODE_OUT_OF_RANGE");
  CANoeAPI_DetSetError(42, 0xA0, "BSWM_E_ALREADY_QUEUED");
  CANoeAPI_DetSetError(42, 0x02, "BSWM_E_NULL_POINTER");
  CANoeAPI_DetSetError(42, 0x01, "BSWM_E_NO_INIT");
  CANoeAPI_DetSetError(42, 0x06, "BSWM_E_PARAM_CONFIG");
  CANoeAPI_DetSetError(42, 0x04, "BSWM_E_REQ_USER_OUT_OF_RANGE");
}

/* Initialize DET codes for module Can*/
STATIC void VTTCntrl_DetInitialization_Can(void)
{
  CANoeAPI_DetSetModule(80, "Can");
  CANoeAPI_DetSetFunction(80, 0x01, "Can_MainFunction_Write");
  CANoeAPI_DetSetFunction(80, 0x0E, "Can_CheckBaudrate");
  CANoeAPI_DetSetFunction(80, 0x0F, "Can_SetBaudrate");
  CANoeAPI_DetSetFunction(80, 0x00, "Can_Init");
  CANoeAPI_DetSetFunction(80, 0x04, "Can_DisableControllerInterrupts");
  CANoeAPI_DetSetFunction(80, 0x02, "Can_InitController");
  CANoeAPI_DetSetFunction(80, 0x03, "Can_SetControllerMode");
  CANoeAPI_DetSetFunction(80, 0x06, "Can_WriteCan_CancelTx");
  CANoeAPI_DetSetFunction(80, 0x0A, "Can_MainFunction_Wakeup");
  CANoeAPI_DetSetFunction(80, 0x07, "Can_GetVersionInfo");
  CANoeAPI_DetSetFunction(80, 0x0D, "Can_ChangeBaudrate");
  CANoeAPI_DetSetFunction(80, 0x11, "CanHL_TxConfirmation");
  CANoeAPI_DetSetFunction(80, 0x0B, "Can_CheckWakeup");
  CANoeAPI_DetSetFunction(80, 0x09, "Can_MainFunction_BusOff");
  CANoeAPI_DetSetFunction(80, 0x12, "CanHL_WakeUpHandling");
  CANoeAPI_DetSetFunction(80, 0x05, "Can_EnableControllerInterrupts");
  CANoeAPI_DetSetFunction(80, 0x13, "CanHL_ErrorHandling");
  CANoeAPI_DetSetFunction(80, 0x10, "CanBasicCanMsgReceived / CanFullCanMsgReceived");
  CANoeAPI_DetSetFunction(80, 0x08, "Can_MainFunction_Read");
  CANoeAPI_DetSetFunction(80, 0x20, "Can_SetMirrorMode / Can_GetStatus");
  CANoeAPI_DetSetFunction(80, 0x0C, "Can_MainFunction_Mode");
  CANoeAPI_DetSetError(80, 0x10, "CAN_E_RXQUEUE");
  CANoeAPI_DetSetError(80, 0x07, "CAN_E_DATALOST");
  CANoeAPI_DetSetError(80, 0x05, "CAN_E_UNINIT");
  CANoeAPI_DetSetError(80, 0x08, "CAN_E_PARAM_BAUDRATE");
  CANoeAPI_DetSetError(80, 0x06, "CAN_E_TRANSITION");
  CANoeAPI_DetSetError(80, 0x01, "CAN_E_PARAM_POINTER");
  CANoeAPI_DetSetError(80, 0x04, "CAN_E_PARAM_CONTROLLER");
  CANoeAPI_DetSetError(80, 0x03, "CAN_E_PARAM_DLC");
  CANoeAPI_DetSetError(80, 0x02, "CAN_E_PARAM_HANDLE");
  CANoeAPI_DetSetError(80, 0x11, "CAN_E_TIMEOUT_DET");
}

/* Initialize DET codes for module CanIf*/
STATIC void VTTCntrl_DetInitialization_CanIf(void)
{
  CANoeAPI_DetSetModule(60, "CanIf");
  CANoeAPI_DetSetFunction(60, 0x21, "CanIf_CheckTrcvWakeFlagIndication");
  CANoeAPI_DetSetFunction(60, 0x1F, "CanIf_CheckTrcvWakeFlag");
  CANoeAPI_DetSetFunction(60, 0x18, "CanIf_TrcvModeIndication");
  CANoeAPI_DetSetFunction(60, 0x10, "CanIf_SetTrcvWakeupMode");
  CANoeAPI_DetSetFunction(60, 0x4, "CanIf_GetControllerMode");
  CANoeAPI_DetSetFunction(60, 0x2, "CanIf_InitController");
  CANoeAPI_DetSetFunction(60, 0x1E, "CanIf_ClearTrcvWufFlag");
  CANoeAPI_DetSetFunction(60, 0xD, "CanIf_SetTrcvMode");
  CANoeAPI_DetSetFunction(60, 0x13, "CanIf_TxConfirmation");
  CANoeAPI_DetSetFunction(60, 0x5, "CanIf_Transmit");
  CANoeAPI_DetSetFunction(60, 0xB, "CanIf_GetVersionInfo");
  CANoeAPI_DetSetFunction(60, 0xFA, "CanIf_CancelTransmit");
  CANoeAPI_DetSetFunction(60, 0x1A, "CanIf_ConfirmPnAvailability");
  CANoeAPI_DetSetFunction(60, 0x14, "CanIf_RxIndication");
  CANoeAPI_DetSetFunction(60, 0x19, "CanIf_GetTxConfirmationState");
  CANoeAPI_DetSetFunction(60, 0xF, "CanIf_GetTrcvWakeupReason");
  CANoeAPI_DetSetFunction(60, 0x9, "CanIf_SetPduMode");
  CANoeAPI_DetSetFunction(60, 0x1C, "CanIf_CheckBaudrate");
  CANoeAPI_DetSetFunction(60, 0x3, "CanIf_SetControllerMode");
  CANoeAPI_DetSetFunction(60, 0x27, "CanIf_SetBaudrate");
  CANoeAPI_DetSetFunction(60, 0x11, "CanIf_CheckWakeup");
  CANoeAPI_DetSetFunction(60, 0x1, "CanIf_Init");
  CANoeAPI_DetSetFunction(60, 0x12, "CanIf_CheckValidation");
  CANoeAPI_DetSetFunction(60, 0xFB, "CanIf_CancelTxNotification");
  CANoeAPI_DetSetFunction(60, 0x6, "CanIf_ReadRxPduData");
  CANoeAPI_DetSetFunction(60, 0x17, "CanIf_ControllerModeIndication");
  CANoeAPI_DetSetFunction(60, 0x20, "CanIf_ClearTrcvWufFlagIndication");
  CANoeAPI_DetSetFunction(60, 0x16, "CanIf_ControllerBusoff");
  CANoeAPI_DetSetFunction(60, 0xA, "CanIf_GetPduMode");
  CANoeAPI_DetSetFunction(60, 0x15, "CanIf_CancelTxConfirmation");
  CANoeAPI_DetSetFunction(60, 0xE, "CanIf_GetTrcvMode");
  CANoeAPI_DetSetFunction(60, 0x1B, "CanIf_ChangeBaudrate");
  CANoeAPI_DetSetFunction(60, 0xC, "CanIf_SetDynamicTxId");
  CANoeAPI_DetSetError(60, 0x1E, "CANIF_E_UNINIT");
  CANoeAPI_DetSetError(60, 0xA, "CANIF_E_PARAM_CANID");
  CANoeAPI_DetSetError(60, 0x13, "CANIF_E_PARAM_TRCVWAKEUPMODE");
  CANoeAPI_DetSetError(60, 0x3D, "CANIF_E_INVALID_DLC");
  CANoeAPI_DetSetError(60, 0x2D, "CANIF_E_CONFIG");
  CANoeAPI_DetSetError(60, 0x32, "CANIF_E_INVALID_TXPDUID");
  CANoeAPI_DetSetError(60, 0x47, "CANIF_E_NOT_SLEEP");
  CANoeAPI_DetSetError(60, 0x11, "CANIF_E_PARAM_TRCV");
  CANoeAPI_DetSetError(60, 0xD, "CANIF_E_PARAM_LPDU");
  CANoeAPI_DetSetError(60, 0x14, "CANIF_E_PARAM_POINTER");
  CANoeAPI_DetSetError(60, 0x15, "CANIF_E_PARAM_CTRLMODE");
  CANoeAPI_DetSetError(60, 0xF, "CANIF_E_PARAM_CONTROLLERID");
  CANoeAPI_DetSetError(60, 0x12, "CANIF_E_PARAM_TRCVMODE");
  CANoeAPI_DetSetError(60, 0xC, "CANIF_E_PARAM_HRH");
  CANoeAPI_DetSetError(60, 0x10, "CANIF_E_PARAM_WAKEUPSOURCE");
  CANoeAPI_DetSetError(60, 0x28, "CANIF_E_NOK_NOSUPPORT");
  CANoeAPI_DetSetError(60, 0x46, "CANIF_E_STOPPED");
  CANoeAPI_DetSetError(60, 0xE, "CANIF_E_PARAM_CONTROLLER");
  CANoeAPI_DetSetError(60, 0xB, "CANIF_E_PARAM_DLC");
}

/* Initialize DET codes for module CanNm*/
STATIC void VTTCntrl_DetInitialization_CanNm(void)
{
  CANoeAPI_DetSetModule(31, "CanNm");
  CANoeAPI_DetSetFunction(31, 0x0C, "CanNm_DisableCommunication");
  CANoeAPI_DetSetFunction(31, 0x40, "CanNm_TxConfirmation");
  CANoeAPI_DetSetFunction(31, 0x02, "CanNm_NetworkRequest");
  CANoeAPI_DetSetFunction(31, 0x06, "CanNm_GetNodeIdentifier");
  CANoeAPI_DetSetFunction(31, 0x08, "CanNm_RepeatMessageRequest");
  CANoeAPI_DetSetFunction(31, 0x05, "CanNm_GetUserData");
  CANoeAPI_DetSetFunction(31, 0x07, "CanNm_GetLocalNodeIdentifier");
  CANoeAPI_DetSetFunction(31, 0x17, "CanNm_SetSleepReadyBit");
  CANoeAPI_DetSetFunction(31, 0x01, "CanNm_PassiveStartUp");
  CANoeAPI_DetSetFunction(31, 0x16, "CanNm_ConfirmPnAvailability");
  CANoeAPI_DetSetFunction(31, 0xF1, "CanNm_GetVersionInfo");
  CANoeAPI_DetSetFunction(31, 0x0B, "CanNm_GetState");
  CANoeAPI_DetSetFunction(31, 0xC0, "CanNm_RequestBusSynchronization");
  CANoeAPI_DetSetFunction(31, 0x04, "CanNm_SetUserData");
  CANoeAPI_DetSetFunction(31, 0x03, "CanNm_NetworkRelease");
  CANoeAPI_DetSetFunction(31, 0x0A, "CanNm_GetPduData");
  CANoeAPI_DetSetFunction(31, 0x14, "CanNm_Transmit");
  CANoeAPI_DetSetFunction(31, 0x00, "CanNm_Init");
  CANoeAPI_DetSetFunction(31, 0x13, "CanNm_MainFunction");
  CANoeAPI_DetSetFunction(31, 0xD0, "CanNm_CheckRemoteSleepIndication");
  CANoeAPI_DetSetFunction(31, 0x42, "CanNm_RxIndication");
  CANoeAPI_DetSetFunction(31, 0x0D, "CanNm_EnableCommunication");
  CANoeAPI_DetSetError(31, 0x12, "CANNM_E_PARAM_POINTER");
  CANoeAPI_DetSetError(31, 0x33, "CANNM_E_INVALID_GENDATA");
  CANoeAPI_DetSetError(31, 0x04, "CANNM_E_NET_START_IND");
  CANoeAPI_DetSetError(31, 0x01, "CANNM_E_NO_INIT");
  CANoeAPI_DetSetError(31, 0x05, "CANNM_E_INIT_FAILED");
  CANoeAPI_DetSetError(31, 0x03, "CANNM_E_INVALID_PDUID");
  CANoeAPI_DetSetError(31, 0x20, "CANNM_E_RXINDICATION_DLC_ERROR");
  CANoeAPI_DetSetError(31, 0x22, "CANNM_E_ALREADY_INITIALIZED");
  CANoeAPI_DetSetError(31, 0x11, "CANNM_E_NETWORK_TIMEOUT");
  CANoeAPI_DetSetError(31, 0x21, "CANNM_E_PDUR_TRIGGERTX_ERROR5");
  CANoeAPI_DetSetError(31, 0x02, "CANNM_E_INVALID_CHANNEL");
}

/* Initialize DET codes for module CanSM*/
STATIC void VTTCntrl_DetInitialization_CanSM(void)
{
  CANoeAPI_DetSetModule(140, "CanSM");
  CANoeAPI_DetSetFunction(140, 0x08, "CanSM_ClearTrcvWufFlagIndication");
  CANoeAPI_DetSetFunction(140, 0x07, "CanSM_ControllerModeIndication");
  CANoeAPI_DetSetFunction(140, 0x20, "CanSM_StartWakeupSources");
  CANoeAPI_DetSetFunction(140, 0x03, "CanSM_GetCurrentComMode");
  CANoeAPI_DetSetFunction(140, 0x00, "CanSM_Init");
  CANoeAPI_DetSetFunction(140, 0x0C, "CanSM_CheckBaudrate");
  CANoeAPI_DetSetFunction(140, 0x06, "CanSM_ConfirmPnAvailability");
  CANoeAPI_DetSetFunction(140, 0x40, "CanSM_PreventBusSleepAtStartUp");
  CANoeAPI_DetSetFunction(140, 0x0B, "CanSM_TxTimeoutException");
  CANoeAPI_DetSetFunction(140, 0x0F, "CanSM_CheckBorLevel");
  CANoeAPI_DetSetFunction(140, 0x0A, "CanSM_CheckTransceiverWakeFlagIndication");
  CANoeAPI_DetSetFunction(140, 0x05, "CanSM_MainFunction");
  CANoeAPI_DetSetFunction(140, 0x04, "CanSM_ControllerBusOff");
  CANoeAPI_DetSetFunction(140, 0x01, "CanSM_GetVersionInfo");
  CANoeAPI_DetSetFunction(140, 0x0E, "CanSM_ChangeBaudrate");
  CANoeAPI_DetSetFunction(140, 0x0D, "CanSM_SetBaudrate");
  CANoeAPI_DetSetFunction(140, 0x02, "CanSM_RequestComMode");
  CANoeAPI_DetSetFunction(140, 0x21, "CanSM_StopWakeupSources");
  CANoeAPI_DetSetFunction(140, 0x09, "CanSM_TransceiverModeIndication");
  CANoeAPI_DetSetError(140, 0x02, "CANSM_E_PARAM_POINTER");
  CANoeAPI_DetSetError(140, 0x0A, "CANSM_E_MODE_REQUEST_TIMEOUT");
  CANoeAPI_DetSetError(140, 0x05, "CANSM_E_PARAM_TRANSCEIVER");
  CANoeAPI_DetSetError(140, 0x01, "CANSM_E_UNINIT");
  CANoeAPI_DetSetError(140, 0x03, "CANSM_E_INVALID_NETWORK_HANDLE");
  CANoeAPI_DetSetError(140, 0x0B, "CANSM_E_INITIALIZED");
  CANoeAPI_DetSetError(140, 0x06, "CANSM_E_BUSOFF_RECOVERY_ACTIVE");
  CANoeAPI_DetSetError(140, 0x08, "CANSM_E_INVALID_COMM_REQUEST");
  CANoeAPI_DetSetError(140, 0x09, "CANSM_E_PARAM_INVALID_BAUDRATE");
  CANoeAPI_DetSetError(140, 0x04, "CANSM_E_PARAM_CONTROLLER");
  CANoeAPI_DetSetError(140, 0x07, "CANSM_E_WAIT_MODE_INDICATION");
}

/* Initialize DET codes for module CanTSyn*/
STATIC void VTTCntrl_DetInitialization_CanTSyn(void)
{
  CANoeAPI_DetSetModule(161, "CanTSyn");
  CANoeAPI_DetSetFunction(161, 0x42, "CanTSyn_RxIndication");
  CANoeAPI_DetSetFunction(161, 0x02, "CanTSyn_GetVersionInfo");
  CANoeAPI_DetSetFunction(161, 0x03, "CanTSyn_SetTransmissionMode");
  CANoeAPI_DetSetFunction(161, 0x06, "CanTSyn_MainFunction");
  CANoeAPI_DetSetFunction(161, 0x01, "CanTSyn_Init");
  CANoeAPI_DetSetFunction(161, 0x40, "CanTSyn_TxConfirmation");
  CANoeAPI_DetSetError(161, 0x2, "CANTSYN_E_NOT_INITIALIZED");
  CANoeAPI_DetSetError(161, 0x3, "CANTSYN_E_NULL_POINTER");
  CANoeAPI_DetSetError(161, 0x1, "CANTSYN_E_INVALID_PDUID");
}

/* Initialize DET codes for module CanTp*/
STATIC void VTTCntrl_DetInitialization_CanTp(void)
{
  CANoeAPI_DetSetModule(35, "CanTp");
  CANoeAPI_DetSetFunction(35, 0x35, "CanTp_TxInit");
  CANoeAPI_DetSetFunction(35, 0x36, "CanTp_StopSeparationTimer");
  CANoeAPI_DetSetFunction(35, 0x05, "CanTp_TxConfirmation");
  CANoeAPI_DetSetFunction(35, 0x31, "CanTp_TxGetBuffer");
  CANoeAPI_DetSetFunction(35, 0x30, "CanTp_RxGetBuffer");
  CANoeAPI_DetSetFunction(35, 0x07, "CanTp_GetVersionInfo");
  CANoeAPI_DetSetFunction(35, 0x34, "CanTp_RxInit");
  CANoeAPI_DetSetFunction(35, 0x06, "CanTp_MainFunction");
  CANoeAPI_DetSetFunction(35, 0x21, "CanTp_MainFunctionTx");
  CANoeAPI_DetSetFunction(35, 0x09, "CanTp_CancelReceive");
  CANoeAPI_DetSetFunction(35, 0x03, "CanTp_Transmit");
  CANoeAPI_DetSetFunction(35, 0x33, "CanTp_TxTransmitFrame");
  CANoeAPI_DetSetFunction(35, 0x32, "CanTp_RxTransmitFrame");
  CANoeAPI_DetSetFunction(35, 0x02, "CanTp_Shutdown");
  CANoeAPI_DetSetFunction(35, 0x04, "CanTp_RxIndication");
  CANoeAPI_DetSetFunction(35, 0x20, "CanTp_MainFunctionRx");
  CANoeAPI_DetSetFunction(35, 0x0B, "CanTp_ReadParameter");
  CANoeAPI_DetSetFunction(35, 0x0A, "CanTp_ChangeParameter");
  CANoeAPI_DetSetFunction(35, 0x01, "CanTp_Init");
  CANoeAPI_DetSetFunction(35, 0x08, "CanTp_CancelTransmit");
  CANoeAPI_DetSetError(35, 0x40, "CANTP_E_INVALID_RX_ID");
  CANoeAPI_DetSetError(35, 0xD4, "CANTP_E_TX_FC_OVFL");
  CANoeAPI_DetSetError(35, 0xC1, "CANTP_E_RX_TIMEOUT_AR");
  CANoeAPI_DetSetError(35, 0xC3, "CANTP_E_RX_TIMEOUT_CR");
  CANoeAPI_DetSetError(35, 0xC6, "CANTP_E_RX_RESTART");
  CANoeAPI_DetSetError(35, 0xC5, "CANTP_E_RX_WFTMAX");
  CANoeAPI_DetSetError(35, 0x20, "CANTP_E_UNINIT");
  CANoeAPI_DetSetError(35, 0x80, "CANTP_E_INVALID_RX_LENGTH");
  CANoeAPI_DetSetError(35, 0x70, "CANTP_E_INVALID_TX_LENGTH");
  CANoeAPI_DetSetError(35, 0xD0, "CANTP_E_TX_COM");
  CANoeAPI_DetSetError(35, 0xD6, "CANTP_E_TX_RES_STMIN");
  CANoeAPI_DetSetError(35, 0x50, "CANTP_E_INVALID_TX_BUFFER");
  CANoeAPI_DetSetError(35, 0xC4, "CANTP_E_RX_INVALID_SN");
  CANoeAPI_DetSetError(35, 0xC0, "CANTP_E_RX_COM");
  CANoeAPI_DetSetError(35, 0xD7, "CANTP_E_TX_TRANSMIT_ERROR");
  CANoeAPI_DetSetError(35, 0x90, "CANTP_E_INVALID_TA_TYPE");
  CANoeAPI_DetSetError(35, 0xD1, "CANTP_E_TX_TIMEOUT_AS");
  CANoeAPI_DetSetError(35, 0xD5, "CANTP_E_TX_INVALID_FS");
  CANoeAPI_DetSetError(35, 0xD3, "CANTP_E_TX_TIMEOUT_CS");
  CANoeAPI_DetSetError(35, 0xC2, "CANTP_E_RX_TIMEOUT_BR");
  CANoeAPI_DetSetError(35, 0xA0, "CANTP_E_OPER_NOT_SUPPORTED");
  CANoeAPI_DetSetError(35, 0xB1, "CANTP_E_INVALID_RX_STATE");
  CANoeAPI_DetSetError(35, 0x30, "CANTP_E_INVALID_TX_ID");
  CANoeAPI_DetSetError(35, 0x60, "CANTP_E_INVALID_RX_BUFFER");
  CANoeAPI_DetSetError(35, 0xB2, "CANTP_E_INVALID_TX_STATE");
  CANoeAPI_DetSetError(35, 0xC7, "CANTP_E_RX_TRANSMIT_ERROR");
  CANoeAPI_DetSetError(35, 0x04, "CANTP_E_PARAM_POINTER");
  CANoeAPI_DetSetError(35, 0xB0, "CANTP_E_COM");
  CANoeAPI_DetSetError(35, 0x01, "CANTP_E_PARAM_CONFIG");
  CANoeAPI_DetSetError(35, 0x02, "CANTP_E_PARAM_ID");
  CANoeAPI_DetSetError(35, 0xD2, "CANTP_E_TX_TIMEOUT_BS");
  CANoeAPI_DetSetError(35, 0xB3, "CANTP_E_INVALID_FRAME_TYPE");
}

/* Initialize DET codes for module CanXcp*/
STATIC void VTTCntrl_DetInitialization_CanXcp(void)
{
  CANoeAPI_DetSetModule(212, "CanXcp");
  CANoeAPI_DetSetFunction(212, 0x65, "Xcp_CanIfTxConfirmation");
  CANoeAPI_DetSetFunction(212, 0x02, "CanXcp_Send");
  CANoeAPI_DetSetFunction(212, 0x07, "CanXcp_SetPduMode");
  CANoeAPI_DetSetFunction(212, 0x04, "CanXcp_SendFlush");
  CANoeAPI_DetSetFunction(212, 0x00, "COMServiceId_Init");
  CANoeAPI_DetSetFunction(212, 0x66, "Xcp_CanIfRxIndication");
  CANoeAPI_DetSetFunction(212, 0x06, "CanXcp_GetVersionInfo");
  CANoeAPI_DetSetFunction(212, 0x01, "CanXcp_TLService");
  CANoeAPI_DetSetFunction(212, 0xc8, "CanXcp_MainFunction");
  CANoeAPI_DetSetFunction(212, 0x05, "CanXcp_Control");
  CANoeAPI_DetSetError(212, 0x12, "CANXCP_E_NULL_POINTER");
  CANoeAPI_DetSetError(212, 0x5, "CANXCP_E_WRONG_CONTROL_COMMAND");
  CANoeAPI_DetSetError(212, 0x4, "CANXCP_E_RX_BUFFER_OVERFLOW");
  CANoeAPI_DetSetError(212, 0x2, "CANXCP_E_NOT_INITIALIZED");
  CANoeAPI_DetSetError(212, 0x1, "CANXCP_E_INV_POINTER");
  CANoeAPI_DetSetError(212, 0x6, "CANXCP_E_INVALID_NW_HANDLE");
  CANoeAPI_DetSetError(212, 0x3, "CANXCP_E_INVALID_PDUID");
}

/* Initialize DET codes for module Com*/
STATIC void VTTCntrl_DetInitialization_Com(void)
{
  CANoeAPI_DetSetModule(50, "Com");
  CANoeAPI_DetSetFunction(50, 0x48, "COMServiceId_TpTxConfirmation");
  CANoeAPI_DetSetFunction(50, 0x19, "COMServiceId_MainFunctionTx");
  CANoeAPI_DetSetFunction(50, 0x3, "COMServiceId_IpduGroupControl");
  CANoeAPI_DetSetFunction(50, 0xC, "COMServiceId_UpdateShadowSignal");
  CANoeAPI_DetSetFunction(50, 0x80, "COMServiceId_Il_AsrComCfg5Internal");
  CANoeAPI_DetSetFunction(50, 0x1C, "COMServiceId_ClearIpduGroupVector");
  CANoeAPI_DetSetFunction(50, 0x17, "COMServiceId_TriggerIPDUSend");
  CANoeAPI_DetSetFunction(50, 0x22, "COMServiceId_ReceiveDynSignal");
  CANoeAPI_DetSetFunction(50, 0x44, "COMServiceId_CopyRxData");
  CANoeAPI_DetSetFunction(50, 0xE, "COMServiceId_ReceiveSignalGroup");
  CANoeAPI_DetSetFunction(50, 0xB, "COMServiceId_ReceiveSignal");
  CANoeAPI_DetSetFunction(50, 0x6, "COMServiceId_ReceptionDMControl");
  CANoeAPI_DetSetFunction(50, 0x7, "COMServiceId_GetStatus");
  CANoeAPI_DetSetFunction(50, 0x18, "COMServiceId_MainFunctionRx");
  CANoeAPI_DetSetFunction(50, 0x8, "COMServiceId_GetConfigurationId");
  CANoeAPI_DetSetFunction(50, 0x40, "COMServiceId_TxConfirmation");
  CANoeAPI_DetSetFunction(50, 0xD, "COMServiceId_SendSignalGroup");
  CANoeAPI_DetSetFunction(50, 0x24, "COMServiceId_ReceiveSignalGroupArray");
  CANoeAPI_DetSetFunction(50, 0x43, "COMServiceId_CopyTxData");
  CANoeAPI_DetSetFunction(50, 0x23, "COMServiceId_SendSignalGroupArray");
  CANoeAPI_DetSetFunction(50, 0x2, "COMServiceId_DeInit");
  CANoeAPI_DetSetFunction(50, 0x9, "COMServiceId_GetVersionInfo");
  CANoeAPI_DetSetFunction(50, 0x1, "COMServiceId_Init");
  CANoeAPI_DetSetFunction(50, 0xA, "COMServiceId_SendSignal");
  CANoeAPI_DetSetFunction(50, 0x1B, "COMServiceId_InvalidateSignalGroup");
  CANoeAPI_DetSetFunction(50, 0x41, "COMServiceId_TriggerTransmit");
  CANoeAPI_DetSetFunction(50, 0x42, "COMServiceId_RxIndication");
  CANoeAPI_DetSetFunction(50, 0x46, "COMServiceId_StartOfReception");
  CANoeAPI_DetSetFunction(50, 0x1D, "COMServiceId_SetIpduGroup");
  CANoeAPI_DetSetFunction(50, 0x16, "COMServiceId_InvalidateShadowSignal");
  CANoeAPI_DetSetFunction(50, 0x27, "COMServiceId_SwitchIpduTxMode");
  CANoeAPI_DetSetFunction(50, 0x1A, "COMServiceId_MainFunctionRouteSignals");
  CANoeAPI_DetSetFunction(50, 0x21, "COMServiceId_SendDynSignal");
  CANoeAPI_DetSetFunction(50, 0x10, "COMServiceId_InvalidateSignal");
  CANoeAPI_DetSetFunction(50, 0xF, "COMServiceId_ReceiveShadowSignal");
  CANoeAPI_DetSetFunction(50, 0x45, "COMServiceId_TpRxIndication");
  CANoeAPI_DetSetFunction(50, 0x28, "COMServiceId_TriggerIPDUSendWithMetaData");
  CANoeAPI_DetSetError(50, 0x1A, "Com_LMgt_PduIdTypeTooSmall");
  CANoeAPI_DetSetError(50, 0xF, "Com_ReceiveSignalGroup_PduId");
  CANoeAPI_DetSetError(50, 0x1E, "Com_LMgt_IpduId");
  CANoeAPI_DetSetError(50, 0x2, "Com_TxModeHdlr_Param_StopCyclicTransmission");
  CANoeAPI_DetSetError(50, 0x12, "Com_TxNHdlr_IpduIdParamTooLarge");
  CANoeAPI_DetSetError(50, 0x14, "Com_TxNHdlr_TimeoutFuncIdxTooLarge");
  CANoeAPI_DetSetError(50, 0x7, "Com_TxBuffer_InvalidPointer_Param");
  CANoeAPI_DetSetError(50, 0x18, "Com_SignalGw_InvalidUpdateBitPosition");
  CANoeAPI_DetSetError(50, 0x21, "Com_TxNHdlr_NotificationFlagIdxTooLarge");
  CANoeAPI_DetSetError(50, 0x8, "Com_TxBuffer_IpduId_Param");
  CANoeAPI_DetSetError(50, 0x23, "Com_Signal_UnsupportedSigFilter");
  CANoeAPI_DetSetError(50, 0x11, "Com_TxSigIf_Init_SignalGroupIndex");
  CANoeAPI_DetSetError(50, 0x15, "Com_LMgt_NullPointerParameter");
  CANoeAPI_DetSetError(50, 0x13, "Com_TxNHdlr_NotiFuncIdxTooLarge");
  CANoeAPI_DetSetError(50, 0xE, "Com_ReceiveSignal_PduId");
  CANoeAPI_DetSetError(50, 0x20, "Com_TxNHdlr_TimeoutFlagByteIdxTooLarge");
  CANoeAPI_DetSetError(50, 0x24, "Com_TxModeHdlr_IllegalTxModeTransition");
  CANoeAPI_DetSetError(50, 0x19, "Com_SignalGw_InvalidSignalGrpHandle");
  CANoeAPI_DetSetError(50, 0x6, "Com_TxModeHdlr_Param_TriggerImmediateIpduSend");
  CANoeAPI_DetSetError(50, 0xD, "Com_TriggerTransmit_PduInfoNullPointer");
  CANoeAPI_DetSetError(50, 0x1F, "Com_TxNHdlr_TimeoutFlagIdxTooLarge");
  CANoeAPI_DetSetError(50, 0x1, "Com_TxModeHdlr_Param_Confirmation");
  CANoeAPI_DetSetError(50, 0xC, "Com_TxLLIf_Transmit_PduInfoNullPointer");
  CANoeAPI_DetSetError(50, 0x16, "Com_SignalGw_InvalidGwHandle");
  CANoeAPI_DetSetError(50, 0x9, "Com_TxBuffer_SignalGroupId_Param");
  CANoeAPI_DetSetError(50, 0xA, "Com_Signal_UnsupportedBusSignalType");
  CANoeAPI_DetSetError(50, 0x1D, "Com_LMgt_SubIpduGroupId");
  CANoeAPI_DetSetError(50, 0x10, "Com_TxLLIf_Transmit_IpduIdParam");
  CANoeAPI_DetSetError(50, 0x1C, "Com_LMgt_SizeOfBoolean");
  CANoeAPI_DetSetError(50, 0x0, "Com_TxModeHdlr_Param_TriggerDeferredIpduSend");
  CANoeAPI_DetSetError(50, 0x1B, "Com_TxModeHdlr_Param_TriggerIpduSendOnceDeferred");
  CANoeAPI_DetSetError(50, 0x5, "Com_TxModeHdlr_Param_Stop");
  CANoeAPI_DetSetError(50, 0x22, "Com_TxNHdlr_NotificationFlagByteIdxTooLarge");
  CANoeAPI_DetSetError(50, 0x17, "Com_SignalGw_InvalidSignalHandle");
  CANoeAPI_DetSetError(50, 0xB, "Com_Signal_InconsistentConfigurationData");
}

/* Initialize DET codes for module ComM*/
STATIC void VTTCntrl_DetInitialization_ComM(void)
{
  CANoeAPI_DetSetModule(12, "ComM");
  CANoeAPI_DetSetFunction(12, 0x73, "ComM_TF_ReadyS_NetReq");
  CANoeAPI_DetSetFunction(12, 0x09, "ComM_PreventWakeUp");
  CANoeAPI_DetSetFunction(12, 0x60, "ComM_MainFunction");
  CANoeAPI_DetSetFunction(12, 0x0e, "ComM_ResetInhibitCounter");
  CANoeAPI_DetSetFunction(12, 0x34, "ComM_GetState");
  CANoeAPI_DetSetFunction(12, 0x75, "ComM_TF_NetReq_ReadyS");
  CANoeAPI_DetSetFunction(12, 0x01, "ComM_Init");
  CANoeAPI_DetSetFunction(12, 0x1a, "ComM_Nm_BusSleepMode");
  CANoeAPI_DetSetFunction(12, 0x35, "ComM_CommunicationAllowed");
  CANoeAPI_DetSetFunction(12, 0x05, "ComM_RequestComMode");
  CANoeAPI_DetSetFunction(12, 0x70, "ComM_TF_NoCom_NetReq");
  CANoeAPI_DetSetFunction(12, 0x19, "ComM_Nm_PrepareBusSleepMode");
  CANoeAPI_DetSetFunction(12, 0x07, "ComM_GetRequestedComMode");
  CANoeAPI_DetSetFunction(12, 0x10, "ComM_GetVersionInfo");
  CANoeAPI_DetSetFunction(12, 0x15, "ComM_Nm_NetworkStartIndication");
  CANoeAPI_DetSetFunction(12, 0x1f, "ComM_DCM_ActiveDiagnostic");
  CANoeAPI_DetSetFunction(12, 0x0b, "ComM_LimitToNoComMode");
  CANoeAPI_DetSetFunction(12, 0x33, "ComM_BusSM_ModeIndication");
  CANoeAPI_DetSetFunction(12, 0x20, "ComM_DCM_InactiveDiagnostic");
  CANoeAPI_DetSetFunction(12, 0x0c, "ComM_LimitECUToNoComMode");
  CANoeAPI_DetSetFunction(12, 0x03, "ComM_GetStatus");
  CANoeAPI_DetSetFunction(12, 0x0f, "ComM_SetECUGroupClassification");
  CANoeAPI_DetSetFunction(12, 0x02, "ComM_DeInit");
  CANoeAPI_DetSetFunction(12, 0x2a, "ComM_EcuM_WakeUpIndication");
  CANoeAPI_DetSetFunction(12, 0x1b, "ComM_Nm_RestartIndication");
  CANoeAPI_DetSetFunction(12, 0x08, "ComM_GetCurrentComMode");
  CANoeAPI_DetSetFunction(12, 0x0d, "ComM_ReadInhibitCounter");
  CANoeAPI_DetSetFunction(12, 0x18, "ComM_Nm_NetworkMode");
  CANoeAPI_DetSetFunction(12, 0x06, "ComM_GetMaxComMode");
  CANoeAPI_DetSetFunction(12, 0x04, "ComM_GetInhibitionStatus");
  CANoeAPI_DetSetError(12, 0x05, "COMM_E_DIAGNOSTIC_NOT_SUPPORTED");
  CANoeAPI_DetSetError(12, 0x01, "COMM_E_NOT_INITED");
  CANoeAPI_DetSetError(12, 0x04, "COMM_E_NOSUPPORTED_MODECHANGE");
  CANoeAPI_DetSetError(12, 0x02, "COMM_E_WRONG_PARAMETERS");
  CANoeAPI_DetSetError(12, 0x03, "COMM_E_ERROR_IN_PROV_SERVICE");
}

/* Initialize DET codes for module Crc*/
STATIC void VTTCntrl_DetInitialization_Crc(void)
{
  CANoeAPI_DetSetModule(202, "Crc");
}

/* Initialize DET codes for module Csm*/
STATIC void VTTCntrl_DetInitialization_Csm(void)
{
  CANoeAPI_DetSetModule(110, "Csm");
  CANoeAPI_DetSetFunction(110, 0x4B, "Csm_AsymPrivateKeyWrapAsymFinish");
  CANoeAPI_DetSetFunction(110, 0x3D, "Csm_KeyExchangeCalcSymKeyStart");
  CANoeAPI_DetSetFunction(110, 0x0C, "Csm_RandomSeedStart");
  CANoeAPI_DetSetFunction(110, 0x2B, "Csm_KeyDeriveStart");
  CANoeAPI_DetSetFunction(110, 0x26, "Csm_SignatureVerifyUpdate");
  CANoeAPI_DetSetFunction(110, 0x29, "Csm_ChecksumUpdate");
  CANoeAPI_DetSetFunction(110, 0x10, "Csm_SymBlockEncryptStart");
  CANoeAPI_DetSetFunction(110, 0x23, "Csm_SignatureGenerateUpdate");
  CANoeAPI_DetSetFunction(110, 0x09, "Csm_MacVerifyStart");
  CANoeAPI_DetSetFunction(110, 0x39, "Csm_AsymPrivateKeyExtractUpdate");
  CANoeAPI_DetSetFunction(110, 0x1F, "Csm_AsymDecryptStart");
  CANoeAPI_DetSetFunction(110, 0x42, "Csm_SymKeyWrapSymFinish");
  CANoeAPI_DetSetFunction(110, 0x47, "Csm_AsymPrivateKeyWrapSymUpdate");
  CANoeAPI_DetSetFunction(110, 0x48, "Csm_AsymPrivateKeyWrapSymFinish");
  CANoeAPI_DetSetFunction(110, 0x37, "Csm_AsymPublicKeyExtractFinish");
  CANoeAPI_DetSetFunction(110, 0x24, "Csm_SignatureGenerateFinish");
  CANoeAPI_DetSetFunction(110, 0x07, "Csm_MacGenerateUpdate");
  CANoeAPI_DetSetFunction(110, 0x40, "Csm_SymKeyWrapSymStart");
  CANoeAPI_DetSetFunction(110, 0x36, "Csm_AsymPublicKeyExtractUpdate");
  CANoeAPI_DetSetFunction(110, 0x1C, "Csm_AsymEncryptStart");
  CANoeAPI_DetSetFunction(110, 0x17, "Csm_SymEncryptUpdate");
  CANoeAPI_DetSetFunction(110, 0x2C, "Csm_KeyDeriveUpdate");
  CANoeAPI_DetSetFunction(110, 0x2F, "Csm_KeyExchangeCalcSecretStart");
  CANoeAPI_DetSetFunction(110, 0x2E, "Csm_KeyExchangeCalcPubVal");
  CANoeAPI_DetSetFunction(110, 0x22, "Csm_SignatureGenerateStart");
  CANoeAPI_DetSetFunction(110, 0x19, "Csm_SymDecryptStart");
  CANoeAPI_DetSetFunction(110, 0x33, "Csm_SymKeyExtractUpdate");
  CANoeAPI_DetSetFunction(110, 0x43, "Csm_SymKeyWrapAsymStart");
  CANoeAPI_DetSetFunction(110, 0x3F, "Csm_KeyExchangeCalcSymKeyFinish");
  CANoeAPI_DetSetFunction(110, 0x15, "Csm_SymBlockDecryptFinish");
  CANoeAPI_DetSetFunction(110, 0x1D, "Csm_AsymEncryptUpdate");
  CANoeAPI_DetSetFunction(110, 0x49, "Csm_AsymPrivateKeyWrapAsymStart");
  CANoeAPI_DetSetFunction(110, 0x38, "Csm_AsymPrivateKeyExtractStart");
  CANoeAPI_DetSetFunction(110, 0x08, "Csm_MacGenerateFinish");
  CANoeAPI_DetSetFunction(110, 0x27, "Csm_SignatureVerifyFinish");
  CANoeAPI_DetSetFunction(110, 0x0F, "Csm_RandomGenerate");
  CANoeAPI_DetSetFunction(110, 0x20, "Csm_AsymDecryptUpdate");
  CANoeAPI_DetSetFunction(110, 0x30, "Csm_KeyExchangeCalcSecretUpdate");
  CANoeAPI_DetSetFunction(110, 0x16, "Csm_SymEncryptStart");
  CANoeAPI_DetSetFunction(110, 0x0D, "Csm_RandomSeedUpdate");
  CANoeAPI_DetSetFunction(110, 0x2D, "Csm_KeyDeriveFinish");
  CANoeAPI_DetSetFunction(110, 0x0E, "Csm_RandomSeedFinish");
  CANoeAPI_DetSetFunction(110, 0x32, "Csm_SymKeyExtractStart");
  CANoeAPI_DetSetFunction(110, 0x46, "Csm_AsymPrivateKeyWrapSymStart");
  CANoeAPI_DetSetFunction(110, 0x41, "Csm_SymKeyWrapSymUpdate");
  CANoeAPI_DetSetFunction(110, 0x06, "Csm_MacGenerateStart");
  CANoeAPI_DetSetFunction(110, 0x04, "Csm_HashUpdate");
  CANoeAPI_DetSetFunction(110, 0x2A, "Csm_ChecksumFinish");
  CANoeAPI_DetSetFunction(110, 0x1B, "Csm_SymDecryptFinish");
  CANoeAPI_DetSetFunction(110, 0x44, "Csm_SymKeyWrapAsymUpdate");
  CANoeAPI_DetSetFunction(110, 0x1A, "Csm_SymDecryptUpdate");
  CANoeAPI_DetSetFunction(110, 0x13, "Csm_SymBlockDecryptStart");
  CANoeAPI_DetSetFunction(110, 0x25, "Csm_SignatureVerifyStart");
  CANoeAPI_DetSetFunction(110, 0x0B, "Csm_MacVerifyFinish");
  CANoeAPI_DetSetFunction(110, 0x4C, "Csm_KeyDeriveSymKey");
  CANoeAPI_DetSetFunction(110, 0x0A, "Csm_MacVerifyUpdate");
  CANoeAPI_DetSetFunction(110, 0x3A, "Csm_AsymPrivateKeyExtractFinish");
  CANoeAPI_DetSetFunction(110, 0x14, "Csm_SymBlockDecryptUpdate");
  CANoeAPI_DetSetFunction(110, 0x4A, "Csm_AsymPrivateKeyWrapAsymUpdate");
  CANoeAPI_DetSetFunction(110, 0x11, "Csm_SymBlockEncryptUpdate");
  CANoeAPI_DetSetFunction(110, 0x3E, "Csm_KeyExchangeCalcSymKeyUpdate");
  CANoeAPI_DetSetFunction(110, 0x03, "Csm_HashStart");
  CANoeAPI_DetSetFunction(110, 0x31, "Csm_KeyExchangeCalcSecretFinish");
  CANoeAPI_DetSetFunction(110, 0x05, "Csm_HashFinish");
  CANoeAPI_DetSetFunction(110, 0x45, "Csm_SymKeyWrapAsymFinish");
  CANoeAPI_DetSetFunction(110, 0x21, "Csm_AsymDecryptFinish");
  CANoeAPI_DetSetFunction(110, 0x12, "Csm_SymBlockEncryptFinish");
  CANoeAPI_DetSetFunction(110, 0x1E, "Csm_AsymEncryptFinish");
  CANoeAPI_DetSetFunction(110, 0x28, "Csm_ChecksumStart");
  CANoeAPI_DetSetFunction(110, 0x34, "Csm_SymKeyExtractFinish");
  CANoeAPI_DetSetFunction(110, 0x35, "Csm_AsymPublicKeyExtractStart");
  CANoeAPI_DetSetFunction(110, 0x18, "Csm_SymEncryptFinish");
  CANoeAPI_DetSetError(110, 0x02, "CSM_E_SERVICE_NOT_STARTED");
  CANoeAPI_DetSetError(110, 0x01, "CSM_E_PARAM_PTR_INVALID");
  CANoeAPI_DetSetError(110, 0x03, "CSM_E_PARAM_METHOD_INVALID");
  CANoeAPI_DetSetError(110, 0x06, "CSM_E_BUFFER_TOO_SMALL");
  CANoeAPI_DetSetError(110, 0x05, "CSM_E_UNINT");
  CANoeAPI_DetSetError(110, 0x04, "CSM_E_PARAM_KEY_TYPE_INVALID");
}

/* Initialize DET codes for module Dcm*/
STATIC void VTTCntrl_DetInitialization_Dcm(void)
{
  CANoeAPI_DetSetModule(53, "Dcm");
  CANoeAPI_DetSetFunction(53, 0x01, "Dcm_Init");
  CANoeAPI_DetSetFunction(53, 0x30, "Dcm_ExternalSetNegResponse");
  CANoeAPI_DetSetFunction(53, 0x06, "Dcm_GetSesCtrlType");
  CANoeAPI_DetSetFunction(53, 0x3B, "ReadData (asynchronous)");
  CANoeAPI_DetSetFunction(53, 0x31, "Dcm_ExternalProcessingDone");
  CANoeAPI_DetSetFunction(53, 0x3F, "IsDidAvailable");
  CANoeAPI_DetSetFunction(53, 0xA6, "Dcm_GetTesterSourceAddress");
  CANoeAPI_DetSetFunction(53, 0x45, "GetSeed");
  CANoeAPI_DetSetFunction(53, 0x34, "ReadData (synchronous)");
  CANoeAPI_DetSetFunction(53, 0x05, "Dcm_TpTxConfirmation");
  CANoeAPI_DetSetFunction(53, 0xA7, "Dcm_GetSecurityLevelFixedBytes");
  CANoeAPI_DetSetFunction(53, 0x41, "WriteDidData");
  CANoeAPI_DetSetFunction(53, 0x40, "ReadDidData");
  CANoeAPI_DetSetFunction(53, 0x04, "Dcm_CopyTxData");
  CANoeAPI_DetSetFunction(53, 0x47, "CompareKey");
  CANoeAPI_DetSetFunction(53, 0x25, "Dcm_MainFunction");
  CANoeAPI_DetSetFunction(53, 0x02, "Dcm_CopyRxData");
  CANoeAPI_DetSetFunction(53, 0xA3, "Dcm_ProvideRecoveryStates");
  CANoeAPI_DetSetFunction(53, 0xA2, "Dcm_TriggerTransmit");
  CANoeAPI_DetSetFunction(53, 0xA8, "Dcm_ProcessVirtualRequest");
  CANoeAPI_DetSetFunction(53, 0xA4, "Dcm_OnRequestDetection");
  CANoeAPI_DetSetFunction(53, 0x59, "GetSecurityAttemptCounter");
  CANoeAPI_DetSetFunction(53, 0x2A, "Dcm_ResetToDefaultSession");
  CANoeAPI_DetSetFunction(53, 0x56, "Dcm_SetActiveDiagnostic");
  CANoeAPI_DetSetFunction(53, 0x0D, "Dcm_GetSecurityLevel");
  CANoeAPI_DetSetFunction(53, 0x24, "Dcm_GetVersionInfo");
  CANoeAPI_DetSetFunction(53, 0x00, "Dcm_StartOfReception");
  CANoeAPI_DetSetFunction(53, 0x32, "Module_DiagnosticService");
  CANoeAPI_DetSetFunction(53, 0xA9, "Dcm_SetSecurityLevel");
  CANoeAPI_DetSetFunction(53, 0xF0, "DCMinternalfunction");
  CANoeAPI_DetSetFunction(53, 0xAA, "ReadData (paged-data-reading)");
  CANoeAPI_DetSetFunction(53, 0x22, "Dcm_ComM_SilentComModeEntered");
  CANoeAPI_DetSetFunction(53, 0x23, "Dcm_ComM_FullComModeEntered");
  CANoeAPI_DetSetFunction(53, 0x5A, "SetSecurityAttemptCounter");
  CANoeAPI_DetSetFunction(53, 0x21, "Dcm_ComM_NoComModeEntered");
  CANoeAPI_DetSetFunction(53, 0x2D, "Dcm_TriggerOnEvent");
  CANoeAPI_DetSetFunction(53, 0x0F, "Dcm_GetActiveProtocol");
  CANoeAPI_DetSetFunction(53, 0xA1, "Dcm_TxConfirmation");
  CANoeAPI_DetSetFunction(53, 0x03, "Dcm_TpRxIndication");
  CANoeAPI_DetSetError(53, 0x41, "DCM_E_INVALID_CONFIG");
  CANoeAPI_DetSetError(53, 0x06, "DCM_E_PARAM");
  CANoeAPI_DetSetError(53, 0x40, "DCM_E_ILLEGAL_STATE");
  CANoeAPI_DetSetError(53, 0x02, "DCM_E_INTERFACE_RETURN_VALUE");
  CANoeAPI_DetSetError(53, 0x01, "DCM_E_INTERFACE_TIMEOUT");
  CANoeAPI_DetSetError(53, 0x03, "DCM_E_INTERFACE_BUFFER_OVERFLOW");
  CANoeAPI_DetSetError(53, 0x07, "DCM_E_PARAM_POINTER");
  CANoeAPI_DetSetError(53, 0x05, "DCM_E_UNINIT");
}

/* Initialize DET codes for module Dem*/
STATIC void VTTCntrl_DetInitialization_Dem(void)
{
  CANoeAPI_DetSetModule(54, "Dem");
  CANoeAPI_DetSetFunction(54, 0x6B, "Dem_DcmGetInfoTypeValue08");
  CANoeAPI_DetSetFunction(54, 0x1D, "Dem_DcmGetFreezeFrameDataByDTC");
  CANoeAPI_DetSetFunction(54, 0x1B, "Dem_DcmEnableDTCRecordUpdate");
  CANoeAPI_DetSetFunction(54, 0xAA, "Dem_SetPfcCycle");
  CANoeAPI_DetSetFunction(54, 0xD6, "Dem_GetNextIUMPRRatioData");
  CANoeAPI_DetSetFunction(54, 0x21, "Dem_DcmGetSizeOfExtendedDataRecordByDTC");
  CANoeAPI_DetSetFunction(54, 0x71, "Dem_RepIUMPRDenLock");
  CANoeAPI_DetSetFunction(54, 0x17, "Dem_DcmGetNumberOfFilteredDTC");
  CANoeAPI_DetSetFunction(54, 0xD2, "Dem_GetEventEnableCondition");
  CANoeAPI_DetSetFunction(54, 0x97, "Dem_J1939DcmGetNextFreezeFrame");
  CANoeAPI_DetSetFunction(54, 0x3B, "Dem_DcmGetNextFilteredDTCAndFDC");
  CANoeAPI_DetSetFunction(54, 0xF2, "Dem_NvM_JobFinished");
  CANoeAPI_DetSetFunction(54, 0x98, "Dem_J1939DcmGetNextSPNInFreezeFrame");
  CANoeAPI_DetSetFunction(54, 0xD1, "Dem_PostRunRequested");
  CANoeAPI_DetSetFunction(54, 0x02, "Dem_Init");
  CANoeAPI_DetSetFunction(54, 0x3F, "Dem_DcmSetFreezeFrameRecordFilter");
  CANoeAPI_DetSetFunction(54, 0x52, "Dem_DcmReadDataOfOBDFreezeFrame");
  CANoeAPI_DetSetFunction(54, 0x40, "Dem_DltGetAllExtendedDataRecords");
  CANoeAPI_DetSetFunction(54, 0x07, "Dem_ClearPrestoredFreezeFrame");
  CANoeAPI_DetSetFunction(54, 0xD8, "Dem_GetCurrentIUMPRRatioDataAndDTC");
  CANoeAPI_DetSetFunction(54, 0x0A, "Dem_GetEventStatus");
  CANoeAPI_DetSetFunction(54, 0x6C, "Dem_DcmGetInfoTypeValue0B");
  CANoeAPI_DetSetFunction(54, 0x32, "Dem_GetEventMemoryOverflow");
  CANoeAPI_DetSetFunction(54, 0x95, "Dem_J1939DcmClearDTC");
  CANoeAPI_DetSetFunction(54, 0x20, "Dem_DcmGetExtendedDataRecordByDTC");
  CANoeAPI_DetSetFunction(54, 0x99, "Dem_J1939DcmSetRatioFilter");
  CANoeAPI_DetSetFunction(54, 0x11, "Dem_SetAgingCycleState");
  CANoeAPI_DetSetFunction(54, 0x9D, "Dem_J1939DcmReadDiagnosticReadiness3");
  CANoeAPI_DetSetFunction(54, 0x39, "Dem_SetEnableCondition");
  CANoeAPI_DetSetFunction(54, 0x67, "Dem_DcmReadDataOfPID41");
  CANoeAPI_DetSetFunction(54, 0x30, "Dem_GetEventExtendedDataRecord");
  CANoeAPI_DetSetFunction(54, 0x2A, "Dem_DcmCancelOperation");
  CANoeAPI_DetSetFunction(54, 0xD5, "Dem_GetIUMPRGeneralData");
  CANoeAPI_DetSetFunction(54, 0x90, "Dem_J1939DcmSetDTCFilter");
  CANoeAPI_DetSetFunction(54, 0x38, "Dem_SetStorageCondition");
  CANoeAPI_DetSetFunction(54, 0x94, "Dem_J1939DcmGetNextDTCwithLampStatus");
  CANoeAPI_DetSetFunction(54, 0x0B, "Dem_GetEventFailed");
  CANoeAPI_DetSetFunction(54, 0x69, "Dem_DcmReadDataOfPID4E");
  CANoeAPI_DetSetFunction(54, 0x68, "Dem_DcmReadDataOfPID4D");
  CANoeAPI_DetSetFunction(54, 0x53, "Dem_DcmGetDTCOfOBDFreezeFrame");
  CANoeAPI_DetSetFunction(54, 0x29, "Dem_GetIndicatorStatus");
  CANoeAPI_DetSetFunction(54, 0x92, "Dem_J1939DcmGetNextFilteredDTC");
  CANoeAPI_DetSetFunction(54, 0xAE, "Dem_SetIUMPRDenCondition");
  CANoeAPI_DetSetFunction(54, 0x55, "Dem_MainFunction");
  CANoeAPI_DetSetFunction(54, 0x25, "Dem_DcmEnableDTCSetting");
  CANoeAPI_DetSetFunction(54, 0x3C, "Dem_DcmGetTranslationType");
  CANoeAPI_DetSetFunction(54, 0x06, "Dem_PrestoreFreezeFrame");
  CANoeAPI_DetSetFunction(54, 0x19, "Dem_DcmGetDTCByOccurrenceTime");
  CANoeAPI_DetSetFunction(54, 0x12, "Dem_SetAgingCycleCounterValue");
  CANoeAPI_DetSetFunction(54, 0x36, "Dem_SetEventSuppression");
  CANoeAPI_DetSetFunction(54, 0x3D, "Dem_DcmGetNextFilteredDTCAndSeverity");
  CANoeAPI_DetSetFunction(54, 0x79, "Dem_SetPtoStatus");
  CANoeAPI_DetSetFunction(54, 0x37, "Dem_SetEventAvailable");
  CANoeAPI_DetSetFunction(54, 0x16, "Dem_DcmGetDTCStatusAvailabilityMask");
  CANoeAPI_DetSetFunction(54, 0x63, "Dem_DcmReadDataOfPID1C");
  CANoeAPI_DetSetFunction(54, 0x34, "Dem_DcmGetFunctionalUnitOfDTC");
  CANoeAPI_DetSetFunction(54, 0xAB, "Dem_GetPfcCycle");
  CANoeAPI_DetSetFunction(54, 0x1C, "Dem_DcmGetOBDFreezeFrameData");
  CANoeAPI_DetSetFunction(54, 0x72, "Dem_RepIUMPRDenRelease");
  CANoeAPI_DetSetFunction(54, 0x73, "Dem_RepIUMPRFaultDetect");
  CANoeAPI_DetSetFunction(54, 0xDB, "Dem_RequestNvSynchronization");
  CANoeAPI_DetSetFunction(54, 0x0D, "Dem_GetDTCOfEvent");
  CANoeAPI_DetSetFunction(54, 0x0F, "Dem_ReportErrorStatus");
  CANoeAPI_DetSetFunction(54, 0x15, "Dem_DcmGetStatusOfDTC");
  CANoeAPI_DetSetFunction(54, 0x05, "Dem_ResetEventStatus");
  CANoeAPI_DetSetFunction(54, 0xD4, "Dem_EnablePermanentStorage");
  CANoeAPI_DetSetFunction(54, 0x64, "Dem_DcmReadDataOfPID21");
  CANoeAPI_DetSetFunction(54, 0x09, "Dem_SetOperationCycleCntValue");
  CANoeAPI_DetSetFunction(54, 0x1F, "Dem_DcmGetSizeOfFreezeFrameByDTC");
  CANoeAPI_DetSetFunction(54, 0x1A, "Dem_DcmDisableDTCRecordUpdate");
  CANoeAPI_DetSetFunction(54, 0x18, "Dem_DcmGetNextFilteredDTC");
  CANoeAPI_DetSetFunction(54, 0xFF, "Internal function calls");
  CANoeAPI_DetSetFunction(54, 0xD0, "Dem_InitMemory");
  CANoeAPI_DetSetFunction(54, 0x24, "Dem_DcmDisableDTCSetting");
  CANoeAPI_DetSetFunction(54, 0x41, "Dem_DltGetMostRecentFreezeFrameRecordData");
  CANoeAPI_DetSetFunction(54, 0x3E, "Dem_GetFaultDetectionCounter");
  CANoeAPI_DetSetFunction(54, 0xD7, "Dem_GetNextIUMPRRatioDataAndDTC");
  CANoeAPI_DetSetFunction(54, 0x9B, "Dem_J1939DcmReadDiagnosticReadiness1");
  CANoeAPI_DetSetFunction(54, 0x9C, "Dem_J1939DcmReadDiagnosticReadiness2");
  CANoeAPI_DetSetFunction(54, 0x23, "Dem_ClearDTC");
  CANoeAPI_DetSetFunction(54, 0xF1, "Dem_NvM_InitAdminData / Dem_NvM_InitStatusData / Dem_NvM_InitDebounceData");
  CANoeAPI_DetSetFunction(54, 0x01, "Dem_PreInit");
  CANoeAPI_DetSetFunction(54, 0x3A, "Dem_DcmGetNextFilteredRecord");
  CANoeAPI_DetSetFunction(54, 0x93, "Dem_J1939DcmFirstDTCwithLampStatus");
  CANoeAPI_DetSetFunction(54, 0x91, "Dem_J1939DcmGetNumberOfFilteredDTC");
  CANoeAPI_DetSetFunction(54, 0x04, "Dem_SetEventStatus");
  CANoeAPI_DetSetFunction(54, 0x61, "Dem_DcmReadDataOfPID01");
  CANoeAPI_DetSetFunction(54, 0x33, "Dem_SetDTCSuppression");
  CANoeAPI_DetSetFunction(54, 0x03, "Dem_Shutdown");
  CANoeAPI_DetSetFunction(54, 0x08, "Dem_SetOperationCycleState");
  CANoeAPI_DetSetFunction(54, 0x13, "Dem_DcmSetDTCFilter");
  CANoeAPI_DetSetFunction(54, 0x0C, "Dem_GetEventTested");
  CANoeAPI_DetSetFunction(54, 0x0E, "Dem_DcmGetSeverityOfDTC");
  CANoeAPI_DetSetFunction(54, 0x31, "Dem_GetEventFreezeFrameData");
  CANoeAPI_DetSetFunction(54, 0x51, "Dem_SetEventDisabled");
  CANoeAPI_DetSetFunction(54, 0x00, "Dem_GetVersionInfo");
  CANoeAPI_DetSetFunction(54, 0x65, "Dem_DcmReadDataOfPID30");
  CANoeAPI_DetSetFunction(54, 0x66, "Dem_DcmReadDataOfPID31");
  CANoeAPI_DetSetFunction(54, 0x22, "Dem_DcmClearDTC");
  CANoeAPI_DetSetFunction(54, 0x7A, "Dem_SetWIRStatus");
  CANoeAPI_DetSetFunction(54, 0x9A, "Dem_J1939DcmGetNextFilteredRatio");
  CANoeAPI_DetSetFunction(54, 0xD3, "Dem_GetWIRStatus");
  CANoeAPI_DetSetFunction(54, 0x96, "Dem_J1939DcmSetFreezeFrameFilter");
  CANoeAPI_DetSetError(54, 0x30, "DEM_E_NODATAAVAILABLE");
  CANoeAPI_DetSetError(54, 0x13, "DEM_E_PARAM_LENGTH");
  CANoeAPI_DetSetError(54, 0x20, "DEM_E_UNINIT");
  CANoeAPI_DetSetError(54, 0xF0, "DEM_E_INCONSISTENT_STATE");
  CANoeAPI_DetSetError(54, 0x11, "DEM_E_PARAM_POINTER");
  CANoeAPI_DetSetError(54, 0x10, "DEM_E_PARAM_CONFIG");
  CANoeAPI_DetSetError(54, 0x40, "DEM_E_WRONG_CONDITION");
  CANoeAPI_DetSetError(54, 0x12, "DEM_E_PARAM_DATA");
}

/* Initialize DET codes for module Dio*/
STATIC void VTTCntrl_DetInitialization_Dio(void)
{
  CANoeAPI_DetSetModule(120, "Dio");
  CANoeAPI_DetSetFunction(120, 0x04, "Dio_ReadChannelGroup");
  CANoeAPI_DetSetFunction(120, 0x10, "Dio_Init");
  CANoeAPI_DetSetFunction(120, 0x03, "Dio_WritePort");
  CANoeAPI_DetSetFunction(120, 0x00, "Dio_ReadChannel");
  CANoeAPI_DetSetFunction(120, 0x12, "Dio_GetVersionInfo");
  CANoeAPI_DetSetFunction(120, 0x02, "Dio_ReadPort");
  CANoeAPI_DetSetFunction(120, 0x01, "Dio_WriteChannel");
  CANoeAPI_DetSetFunction(120, 0x05, "Dio_WriteChannelGroup");
  CANoeAPI_DetSetError(120, 0x0A, "DIO_E_PARAM_INVALID_CHANNEL_ID");
  CANoeAPI_DetSetError(120, 0x1F, "DIO_E_PARAM_INVALID_GROUP");
  CANoeAPI_DetSetError(120, 0x20, "DIO_E_PARAM_VINFO");
  CANoeAPI_DetSetError(120, 0x10, "DIO_E_PARAM_CONFIG");
  CANoeAPI_DetSetError(120, 0x14, "DIO_E_PARAM_INVALID_PORT_ID");
}

/* Initialize DET codes for module DoIp*/
STATIC void VTTCntrl_DetInitialization_DoIp(void)
{
  CANoeAPI_DetSetModule(173, "DoIp");
  CANoeAPI_DetSetFunction(173, 0x03, "DOIP_API_ID_CANCEL_RECEIVE");
  CANoeAPI_DetSetFunction(173, 0x10, "DOIP_API_ID_VIF_TRANSMIT");
  CANoeAPI_DetSetFunction(173, 0x01, "DOIP_API_ID_TRANSMIT");
  CANoeAPI_DetSetFunction(173, 0x09, "DOIP_API_ID_TP_COPY_TX_DATA");
  CANoeAPI_DetSetFunction(173, 0x02, "DOIP_API_ID_CANCEL_TRANSMIT");
  CANoeAPI_DetSetFunction(173, 0x04, "DOIP_API_ID_IF_RX_INDICATION");
  CANoeAPI_DetSetFunction(173, 0x05, "DOIP_API_ID_TP_START_OF_RECEPTION");
  CANoeAPI_DetSetFunction(173, 0x0D, "DOIP_API_ID_MAIN_FUNCTION");
  CANoeAPI_DetSetFunction(173, 0x0E, "DOIP_API_ID_ENABLE_PDU_SIZE_ROUTING");
  CANoeAPI_DetSetFunction(173, 0x0F, "DOIP_API_ID_DISABLE_PDU_SIZE_ROUTING");
  CANoeAPI_DetSetFunction(173, 0x0A, "DOIP_API_ID_TP_TX_CONFIRMATION");
  CANoeAPI_DetSetFunction(173, 0x0C, "DOIP_API_ID_LOC_IP_ADDR_ASSIGN_CHG");
  CANoeAPI_DetSetFunction(173, 0x12, "DOIP_API_ID_GET_VERSION_INFO");
  CANoeAPI_DetSetFunction(173, 0x07, "DOIP_API_ID_TP_RX_INDICATION");
  CANoeAPI_DetSetFunction(173, 0x0B, "DOIP_API_ID_SO_CON_MODE_CHG");
  CANoeAPI_DetSetFunction(173, 0x00, "DOIP_API_ID_INIT");
  CANoeAPI_DetSetFunction(173, 0x11, "DOIP_API_ID_VTP_TRANSMIT");
  CANoeAPI_DetSetFunction(173, 0x08, "DOIP_API_ID_IF_TX_CONFIRMATION");
  CANoeAPI_DetSetFunction(173, 0x06, "DOIP_API_ID_TP_COPY_RX_DATA");
  CANoeAPI_DetSetError(173, 0x02, "DOIP_E_PARAM_POINTER");
  CANoeAPI_DetSetError(173, 0x04, "DOIP_E_INVALID_PARAMETER");
  CANoeAPI_DetSetError(173, 0x03, "DOIP_E_INVALID_PDU_SDU_ID");
  CANoeAPI_DetSetError(173, 0x01, "DOIP_E_UNINIT");
  CANoeAPI_DetSetError(173, 0x05, "DOIP_E_NOBUFS");
}

/* Initialize DET codes for module Ea*/
STATIC void VTTCntrl_DetInitialization_Ea(void)
{
  CANoeAPI_DetSetModule(40, "Ea");
  CANoeAPI_DetSetFunction(40, 0x11, "Ea_JobErrorNotification");
  CANoeAPI_DetSetFunction(40, 0x08, "Ea_GetVersionInfo");
  CANoeAPI_DetSetFunction(40, 0x01, "Ea_SetMode");
  CANoeAPI_DetSetFunction(40, 0x10, "Ea_JobEndNotification");
  CANoeAPI_DetSetFunction(40, 0x00, "Ea_Init");
  CANoeAPI_DetSetFunction(40, 0x05, "Ea_GetStatus");
  CANoeAPI_DetSetFunction(40, 0x02, "Ea_Read");
  CANoeAPI_DetSetFunction(40, 0x12, "Ea_MainFunction");
  CANoeAPI_DetSetFunction(40, 0x09, "Ea_EraseImmediateBlock");
  CANoeAPI_DetSetFunction(40, 0x04, "Ea_Cancel");
  CANoeAPI_DetSetFunction(40, 0x06, "Ea_GetJobResult");
  CANoeAPI_DetSetFunction(40, 0x03, "Ea_Write");
  CANoeAPI_DetSetFunction(40, 0x07, "Ea_InvalidateBlock");
  CANoeAPI_DetSetError(40, 0x04, "EA_E_INVALID_DATA_POINTER");
  CANoeAPI_DetSetError(40, 0x08, "EA_E_INVALID_CANCEL");
  CANoeAPI_DetSetError(40, 0x02, "EA_E_INVALID_BLOCK_NO");
  CANoeAPI_DetSetError(40, 0x0A, "EA_E_PARAM_MODE");
  CANoeAPI_DetSetError(40, 0x05, "EA_E_INVALID_BLOCK_LEN");
  CANoeAPI_DetSetError(40, 0x06, "EA_E_BUSY");
  CANoeAPI_DetSetError(40, 0x07, "EA_E_BUSY_INTERNAL");
  CANoeAPI_DetSetError(40, 0x01, "EA_E_UNINIT");
  CANoeAPI_DetSetError(40, 0x03, "EA_E_INVALID_BLOCK_OFS");
}

/* Initialize DET codes for module EcuM*/
STATIC void VTTCntrl_DetInitialization_EcuM(void)
{
  CANoeAPI_DetSetModule(10, "EcuM");
  CANoeAPI_DetSetFunction(10, 0x1E, "EcuM_GetNextRecentShutdown[notsupportedinthisrelease]");
  CANoeAPI_DetSetFunction(10, 0x06, "EcuM_SelectShutdownTarget");
  CANoeAPI_DetSetFunction(10, 0x16, "EcuM_ClearWakeupEvent");
  CANoeAPI_DetSetFunction(10, 0x24, "EcuM_AbortWakeupAlarm");
  CANoeAPI_DetSetFunction(10, 0x1A, "EcuM_StartupTwo");
  CANoeAPI_DetSetFunction(10, 0x0C, "EcuM_SetWakeupEvent");
  CANoeAPI_DetSetFunction(10, 0x0B, "EcuM_ReleasePOST_RUN");
  CANoeAPI_DetSetFunction(10, 0x13, "EcuM_GetBootTarget");
  CANoeAPI_DetSetFunction(10, 0x27, "EcuM_SetClock");
  CANoeAPI_DetSetFunction(10, 0x22, "EcuM_SetRelWakeupAlarm");
  CANoeAPI_DetSetFunction(10, 0x2A, "EcuM_KillAllPostRUNRequests");
  CANoeAPI_DetSetFunction(10, 0x1D, "EcuM_GetMostRecentShutdown[notsupportedinthisrelease]");
  CANoeAPI_DetSetFunction(10, 0x1B, "EcuM_SelectShutdownCause");
  CANoeAPI_DetSetFunction(10, 0x28, "EcuM_StartCheckWakeup");
  CANoeAPI_DetSetFunction(10, 0x00, "EcuM_GetVersionInfo");
  CANoeAPI_DetSetFunction(10, 0x2B, "EcuM_SetState");
  CANoeAPI_DetSetFunction(10, 0x29, "EcuM_EndCheckWakeup");
  CANoeAPI_DetSetFunction(10, 0x18, "EcuM_MainFunction");
  CANoeAPI_DetSetFunction(10, 0x02, "EcuM_Shutdown");
  CANoeAPI_DetSetFunction(10, 0x65, "EcuM_CB_NfyNvMJobEnd");
  CANoeAPI_DetSetFunction(10, 0x0A, "EcuM_RequestPOST_RUN");
  CANoeAPI_DetSetFunction(10, 0x15, "EcuM_GetValidatedWakeupEvents");
  CANoeAPI_DetSetFunction(10, 0x26, "EcuM_GetWakeupTime");
  CANoeAPI_DetSetFunction(10, 0x1C, "EcuM_GetShutdownCause");
  CANoeAPI_DetSetFunction(10, 0x03, "EcuM_RequestRun");
  CANoeAPI_DetSetFunction(10, 0x05, "EcuM_KillAllRUNRequests");
  CANoeAPI_DetSetFunction(10, 0x21, "EcuM_GoPoll");
  CANoeAPI_DetSetFunction(10, 0x25, "EcuM_GetCurrentTime");
  CANoeAPI_DetSetFunction(10, 0x1F, "EcuM_GoDown");
  CANoeAPI_DetSetFunction(10, 0x30, "EcuM_ClearValidatedWakeupEvent");
  CANoeAPI_DetSetFunction(10, 0x0D, "EcuM_GetPendingWakeupEvents");
  CANoeAPI_DetSetFunction(10, 0x07, "EcuM_GetState");
  CANoeAPI_DetSetFunction(10, 0x14, "EcuM_ValidateWakeupEvent");
  CANoeAPI_DetSetFunction(10, 0x20, "EcuM_GoHalt");
  CANoeAPI_DetSetFunction(10, 0x19, "EcuM_GetExpiredWakeupEvents");
  CANoeAPI_DetSetFunction(10, 0x01, "EcuM_Init");
  CANoeAPI_DetSetFunction(10, 0x23, "EcuM_SetAbsWakeupAlarm");
  CANoeAPI_DetSetFunction(10, 0x09, "EcuM_GetShutdownTarget");
  CANoeAPI_DetSetFunction(10, 0x08, "EcuM_GetLastShutdownTarget");
  CANoeAPI_DetSetFunction(10, 0x04, "EcuM_ReleaseRun");
  CANoeAPI_DetSetFunction(10, 0x12, "EcuM_SelectBootTarget");
  CANoeAPI_DetSetError(10, 0x24, "ECUM_E_INVALID_STATEREQUEST");
  CANoeAPI_DetSetError(10, 0x12, "ECUM_E_NULL_POINTER");
  CANoeAPI_DetSetError(10, 0x21, "ECUM_E_MODULE_NOT_IN_PREPSHUTDOWN");
  CANoeAPI_DetSetError(10, 0x13, "ECUM_E_INVALID_PAR");
  CANoeAPI_DetSetError(10, 0x14, "ECUM_E_MULTIPLE_RUN_REQUESTS");
  CANoeAPI_DetSetError(10, 0x10, "ECUM_E_UNINIT");
  CANoeAPI_DetSetError(10, 0x22, "ECUM_E_MODULE_NOT_IN_RUN_STATE");
  CANoeAPI_DetSetError(10, 0x23, "ECUM_E_NO_SLEEPMODE_CONFIGURED");
  CANoeAPI_DetSetError(10, 0x15, "ECUM_E_MISMATCHED_RUN_RELEASE");
  CANoeAPI_DetSetError(10, 0x16, "ECUM_E_STATE_PAR_OUT_OF_RANGE");
  CANoeAPI_DetSetError(10, 0x17, "ECUM_E_UNKNOWN_WAKEUP_SOURCE");
  CANoeAPI_DetSetError(10, 0x20, "ECUM_E_MODULE_NOT_IN_STARTUP");
  CANoeAPI_DetSetError(10, 0x11, "ECUM_E_SERVICE_DISABLED");
}

/* Initialize DET codes for module Eep*/
STATIC void VTTCntrl_DetInitialization_Eep(void)
{
  CANoeAPI_DetSetModule(90, "Eep");
  CANoeAPI_DetSetFunction(90, 0x08, "Eep_GetJobResult");
  CANoeAPI_DetSetFunction(90, 0x09, "Eep_MainFunction");
  CANoeAPI_DetSetFunction(90, 0x04, "Eep_Erase");
  CANoeAPI_DetSetFunction(90, 0x0A, "Eep_GetVersionInfo");
  CANoeAPI_DetSetFunction(90, 0x01, "Eep_SetMode");
  CANoeAPI_DetSetFunction(90, 0x05, "Eep_Compare");
  CANoeAPI_DetSetFunction(90, 0x07, "Eep_GetStatus");
  CANoeAPI_DetSetFunction(90, 0x06, "Eep_Cancel");
  CANoeAPI_DetSetFunction(90, 0x02, "Eep_Read");
  CANoeAPI_DetSetFunction(90, 0x00, "Eep_Init");
  CANoeAPI_DetSetFunction(90, 0x03, "Eep_Write");
  CANoeAPI_DetSetError(90, 0x15, "EEP_E_PARAM_VINFO");
  CANoeAPI_DetSetError(90, 0x10, "EEP_E_PARAM_CONFIG");
  CANoeAPI_DetSetError(90, 0X11, "EEP_E_PARAM_ADDRESS");
  CANoeAPI_DetSetError(90, 0x21, "EEP_E_BUSY");
  CANoeAPI_DetSetError(90, 0x14, "EEP_E_PARAM_POINTER");
  CANoeAPI_DetSetError(90, 0x13, "EEP_E_PARAM_LENGTH");
  CANoeAPI_DetSetError(90, 0x20, "EEP_E_UNINIT");
  CANoeAPI_DetSetError(90, 0x12, "EEP_E_PARAM_DATA");
}

/* Initialize DET codes for module Eth*/
STATIC void VTTCntrl_DetInitialization_Eth(void)
{
  CANoeAPI_DetSetModule(88, "Eth");
  CANoeAPI_DetSetFunction(88, 0x0D, "ETH_API_ID_RECEIVE");
  CANoeAPI_DetSetFunction(88, 0x0B, "ETH_API_ID_PROVIDE_TX_BUFFER");
  CANoeAPI_DetSetFunction(88, 0x04, "ETH_API_ID_GET_CONTROLLER_MODE");
  CANoeAPI_DetSetFunction(88, 0x10, "ETH_API_ID_RX_IRQ_HDLR_0");
  CANoeAPI_DetSetFunction(88, 0x02, "ETH_API_ID_CONTROLLER_INIT");
  CANoeAPI_DetSetFunction(88, 0x09, "ETH_API_ID_SET_PHYS_ADDR");
  CANoeAPI_DetSetFunction(88, 0x05, "ETH_API_ID_WRITE_MII");
  CANoeAPI_DetSetFunction(88, 0x03, "ETH_API_ID_SET_CONTROLLER_MODE");
  CANoeAPI_DetSetFunction(88, 0x08, "ETH_API_ID_GET_PHYS_ADDR");
  CANoeAPI_DetSetFunction(88, 0x06, "ETH_API_ID_READ_MII");
  CANoeAPI_DetSetFunction(88, 0x0C, "ETH_API_ID_TRANSMIT");
  CANoeAPI_DetSetFunction(88, 0x01, "ETH_API_ID_INIT");
  CANoeAPI_DetSetFunction(88, 0x0F, "ETH_API_ID_GET_VERSION_INFO");
  CANoeAPI_DetSetFunction(88, 0x0A, "ETH_API_ID_UPDATE_PHYS_ADDR_FILTER");
  CANoeAPI_DetSetFunction(88, 0x07, "ETH_API_ID_GET_COUNTER_STATE");
  CANoeAPI_DetSetFunction(88, 0x0E, "ETH_API_ID_TX_CONFIRMATION");
  CANoeAPI_DetSetError(88, 0x02, "ETH_E_NOT_INITIALIZED");
  CANoeAPI_DetSetError(88, 0x04, "ETH_E_INV_PARAM");
  CANoeAPI_DetSetError(88, 0x01, "ETH_E_INV_CTRL_IDX");
  CANoeAPI_DetSetError(88, 0x05, "ETH_E_INV_CONFIG");
  CANoeAPI_DetSetError(88, 0x03, "ETH_E_INV_POINTER");
  CANoeAPI_DetSetError(88, 0x06, "ETH_E_FRAMES_LOST");
}

/* Initialize DET codes for module EthIf*/
STATIC void VTTCntrl_DetInitialization_EthIf(void)
{
  CANoeAPI_DetSetModule(65, "EthIf");
  CANoeAPI_DetSetFunction(65, 0x20, "ETHIF_API_ID_MAIN_FUNCTION_RX");
  CANoeAPI_DetSetFunction(65, 0x03, "ETHIF_API_ID_SET_CONTROLLER_MODE");
  CANoeAPI_DetSetFunction(65, 0x2A, "ETHIF_API_ID_GET_BUFFER_LEVEL");
  CANoeAPI_DetSetFunction(65, 0x22, "ETHIF_API_ID_TRCV_LINK_STATE_CHG");
  CANoeAPI_DetSetFunction(65, 0x41, "ETHIF_API_ID_GET_RX_HEADER_PTR");
  CANoeAPI_DetSetFunction(65, 0x51, "ETHIF_PTP_API_ID_SET_GLOBAL_TIME");
  CANoeAPI_DetSetFunction(65, 0x0F, "ETHIF_API_ID_PROVIDE_EXT_TX_BUFFER");
  CANoeAPI_DetSetFunction(65, 0x53, "ETHIF_PTP_API_ID_ENABLE_EGRESS_TIMESTAMP");
  CANoeAPI_DetSetFunction(65, 0x02, "ETHIF_API_ID_CONTROLLER_INIT");
  CANoeAPI_DetSetFunction(65, 0x60, "ETHIF_API_ID_GET_BANDWIDTH_LIMIT");
  CANoeAPI_DetSetFunction(65, 0x2D, "ETHIF_API_ID_RESET_CONFIGURATION");
  CANoeAPI_DetSetFunction(65, 0x0B, "ETHIF_API_ID_GET_VERSION_INFO");
  CANoeAPI_DetSetFunction(65, 0x55, "ETHIF_PTP_API_ID_GET_INGRESS_TIMESTAMP");
  CANoeAPI_DetSetFunction(65, 0x0C, "ETHIF_API_ID_UPDATE_PHYS_ADDR_FILTER");
  CANoeAPI_DetSetFunction(65, 0x05, "ETHIF_API_ID_TRANSCEIVER_INIT");
  CANoeAPI_DetSetFunction(65, 0x2B, "ETHIF_API_ID_GET_DROP_COUNT");
  CANoeAPI_DetSetFunction(65, 0x2C, "ETHIF_API_ID_STORE_CONFIGURATION");
  CANoeAPI_DetSetFunction(65, 0x50, "ETHIF_PTP_API_ID_GET_GLOBAL_TIME");
  CANoeAPI_DetSetFunction(65, 0x06, "ETHIF_API_ID_SET_TRANSCEIVER_MODE");
  CANoeAPI_DetSetFunction(65, 0x54, "ETHIF_PTP_API_ID_GET_EGRESS_TIMESTAMP");
  CANoeAPI_DetSetFunction(65, 0x29, "ETHIF_API_ID_GET_ARL_TABLE");
  CANoeAPI_DetSetFunction(65, 0x61, "ETHIF_API_ID_SET_BANDWIDTH_LIMIT");
  CANoeAPI_DetSetFunction(65, 0x01, "ETHIF_API_ID_INIT");
  CANoeAPI_DetSetFunction(65, 0x52, "ETHIF_PTP_API_ID_SET_CORRECTION_TIME");
  CANoeAPI_DetSetFunction(65, 0x2F, "ETHIF_API_ID_GET_TRANSCEIVER_WAKEUP_MODE");
  CANoeAPI_DetSetFunction(65, 0x07, "ETHIF_API_ID_GET_TRANSCEIVER_MODE");
  CANoeAPI_DetSetFunction(65, 0x10, "ETHIF_API_ID_RX_INDICATION");
  CANoeAPI_DetSetFunction(65, 0x40, "ETHIF_API_ID_GET_TX_HEADER_PTR");
  CANoeAPI_DetSetFunction(65, 0x0D, "ETHIF_API_ID_SET_PHYS_ADDR");
  CANoeAPI_DetSetFunction(65, 0x09, "ETHIF_API_ID_PROVIDE_TX_BUFFER");
  CANoeAPI_DetSetFunction(65, 0x0E, "ETHIF_API_ID_RELEASE_RX_BUFFER");
  CANoeAPI_DetSetFunction(65, 0x2E, "ETHIF_API_ID_SET_TRANSCEIVER_WAKEUP_MODE");
  CANoeAPI_DetSetFunction(65, 0x0A, "ETHIF_API_ID_TRANSMIT");
  CANoeAPI_DetSetFunction(65, 0x11, "ETHIF_API_ID_TX_CONFIRMATION");
  CANoeAPI_DetSetFunction(65, 0x28, "ETHIF_API_ID_GET_PORT_MAC_ADDR");
  CANoeAPI_DetSetFunction(65, 0x30, "ETHIF_API_ID_CHECK_WAKEUP");
  CANoeAPI_DetSetFunction(65, 0x04, "ETHIF_API_ID_GET_CONTROLLER_MODE");
  CANoeAPI_DetSetFunction(65, 0x21, "ETHIF_API_ID_MAIN_FUNCTION_TX");
  CANoeAPI_DetSetFunction(65, 0x08, "ETHIF_API_ID_GET_PHYS_ADDR");
  CANoeAPI_DetSetError(65, 0x02, "ETHIF_E_INV_TRCV_IDX");
  CANoeAPI_DetSetError(65, 0x03, "ETHIF_E_NOT_INITIALIZED");
  CANoeAPI_DetSetError(65, 0x05, "ETHIF_E_INV_PARAM");
  CANoeAPI_DetSetError(65, 0x06, "ETHIF_E_INV_SWITCH_IDX");
  CANoeAPI_DetSetError(65, 0x04, "ETHIF_E_INV_POINTER");
  CANoeAPI_DetSetError(65, 0x01, "ETHIF_E_INV_CTRL_IDX");
}

/* Initialize DET codes for module EthSM*/
STATIC void VTTCntrl_DetInitialization_EthSM(void)
{
  CANoeAPI_DetSetModule(143, "EthSM");
  CANoeAPI_DetSetFunction(143, 0x07, "ETHSM_API_ID_INIT");
  CANoeAPI_DetSetFunction(143, 0x08, "ETHSM_API_ID_TCPIP_MODE_INDICATION");
  CANoeAPI_DetSetFunction(143, 0x04, "ETHSM_API_ID_GET_CURRENT_COM_MODE");
  CANoeAPI_DetSetFunction(143, 0x10, "ETHSM_API_ID_TRCV_MODE_INDICATION");
  CANoeAPI_DetSetFunction(143, 0x03, "ETHSM_API_ID_GET_CURRENT_INTERNAL_MODE");
  CANoeAPI_DetSetFunction(143, 0x02, "ETHSM_API_ID_GET_VERSION_INFO");
  CANoeAPI_DetSetFunction(143, 0x01, "ETHSM_API_ID_MAIN_FUNCTION");
  CANoeAPI_DetSetFunction(143, 0x09, "ETHSM_API_ID_CTRL_MODE_INDICATION");
  CANoeAPI_DetSetFunction(143, 0x06, "ETHSM_API_ID_TRCV_LINK_STATE_CHG");
  CANoeAPI_DetSetFunction(143, 0x05, "ETHSM_API_ID_REQUEST_COM_MODE");
  CANoeAPI_DetSetError(143, 0x02, "ETHSM_E_UNINIT");
  CANoeAPI_DetSetError(143, 0x09, "ETHSM_E_ETHIF_TIMEOUT");
  CANoeAPI_DetSetError(143, 0x01, "ETHSM_E_INV_NETWORK_MODE");
  CANoeAPI_DetSetError(143, 0x08, "ETHSM_E_INVALID_PARAM_TRANSCEIVER");
  CANoeAPI_DetSetError(143, 0x03, "ETHSM_E_PARAM_POINTER");
  CANoeAPI_DetSetError(143, 0x07, "ETHSM_E_INVALID_PARAM_CONTRILLER");
  CANoeAPI_DetSetError(143, 0x05, "ETHSM_E_INVALID_TcpIpMode");
  CANoeAPI_DetSetError(143, 0x06, "ETHSM_E_INVALID_TRCV_LINK_STATE");
  CANoeAPI_DetSetError(143, 0x04, "ETHSM_E_INVALID_NETOWRK_HANDLE");
}

/* Initialize DET codes for module EthTSyn*/
STATIC void VTTCntrl_DetInitialization_EthTSyn(void)
{
  CANoeAPI_DetSetModule(164, "EthTSyn");
  CANoeAPI_DetSetFunction(164, 0x43, "EthTSyn_SetLocalTime");
  CANoeAPI_DetSetFunction(164, 0x20, "EthTSyn_TxPDelayReqMsg");
  CANoeAPI_DetSetFunction(164, 0x10, "EthTSyn_RxIndication");
  CANoeAPI_DetSetFunction(164, 0x32, "EthTSyn_ProcessSmSyncReceive");
  CANoeAPI_DetSetFunction(164, 0x11, "EthTSyn_TxConfirmation");
  CANoeAPI_DetSetFunction(164, 0x49, "EthTSyn_GetIngressTimestamp");
  CANoeAPI_DetSetFunction(164, 0x17, "EthTSyn_HtonTimestamp");
  CANoeAPI_DetSetFunction(164, 0x15, "EthTSyn_SetGlobalTime");
  CANoeAPI_DetSetFunction(164, 0x1B, "EthTSyn_SetTransmissionMode");
  CANoeAPI_DetSetFunction(164, 0x25, "EthTSyn_TxAnnounceMsg");
  CANoeAPI_DetSetFunction(164, 0x44, "EthTSyn_TimestampPlusFullTimeDiff");
  CANoeAPI_DetSetFunction(164, 0x12, "EthTSyn_Transmit");
  CANoeAPI_DetSetFunction(164, 0x24, "EthTSyn_TxFollowUpMsg");
  CANoeAPI_DetSetFunction(164, 0x40, "EthTSyn_UpdateLocalTime");
  CANoeAPI_DetSetFunction(164, 0x13, "EthTSyn_Cbk_TrcvLinkStateChg");
  CANoeAPI_DetSetFunction(164, 0x48, "EthTSyn_GetEgressTimestamp");
  CANoeAPI_DetSetFunction(164, 0x03, "EthTSyn_MainFunction");
  CANoeAPI_DetSetFunction(164, 0x23, "EthTSyn_TxSyncMsg");
  CANoeAPI_DetSetFunction(164, 0x45, "EthTSyn_CalculateTimeDiff");
  CANoeAPI_DetSetFunction(164, 0x21, "EthTSyn_TxPDelayRespMsg");
  CANoeAPI_DetSetFunction(164, 0x31, "EthTSyn_ProcessSmPDelayResp");
  CANoeAPI_DetSetFunction(164, 0x22, "EthTSyn_TxPDelayRespFollowUpMsg");
  CANoeAPI_DetSetFunction(164, 0x01, "EthTSyn_GetVersionInfo");
  CANoeAPI_DetSetFunction(164, 0x1A, "EthTSyn_QualifyAnnounceMsg");
  CANoeAPI_DetSetFunction(164, 0x46, "EthTSyn_RawTimeToTimestamp");
  CANoeAPI_DetSetFunction(164, 0x33, "EthTSyn_ProcessSmPortSyncSyncReceive");
  CANoeAPI_DetSetFunction(164, 0x14, "EthTSyn_GetCurrentTime");
  CANoeAPI_DetSetFunction(164, 0x19, "EthTSyn_TimestampPlusTimediff");
  CANoeAPI_DetSetFunction(164, 0x16, "EthTSyn_NtohTimestamp");
  CANoeAPI_DetSetFunction(164, 0x02, "EthTSyn_Init");
  CANoeAPI_DetSetFunction(164, 0x18, "EthTSyn_TimestampMinusTimestamp");
  CANoeAPI_DetSetFunction(164, 0x41, "EthTSyn_GetCurrentLocalTime");
  CANoeAPI_DetSetFunction(164, 0x34, "EthTSyn_ProcessSmSyncSend");
  CANoeAPI_DetSetFunction(164, 0x47, "EthTSyn_PrepareEgressTimestamping");
  CANoeAPI_DetSetFunction(164, 0x30, "EthTSyn_ProcessSmPDelayReq");
  CANoeAPI_DetSetFunction(164, 0x42, "EthTSyn_GetCurrentTimeRaw");
  CANoeAPI_DetSetError(164, 0x0C, "ETHTSYN_E_GET_INGRESS_TIMESTAMP_FAILED");
  CANoeAPI_DetSetError(164, 0x02, "ETHTSYN _E_INV_POINTER");
  CANoeAPI_DetSetError(164, 0x06, "ETHTSYN _E_BUFREQ_FAILED");
  CANoeAPI_DetSetError(164, 0x0B, "ETHTSYN_E_SET_CORRECTION_TIME_FAILED");
  CANoeAPI_DetSetError(164, 0x11, "ETHTSYN_E_INV_OS_COUNTER_CHANNEL_REF");
  CANoeAPI_DetSetError(164, 0x07, "ETHTSYN _E_INV_TIMESTAMP");
  CANoeAPI_DetSetError(164, 0x03, "ETHTSYN _E_INV_PARAM");
  CANoeAPI_DetSetError(164, 0x05, "ETHTSYN _E_TX_FAILED");
  CANoeAPI_DetSetError(164, 0x08, "ETHTSYN _E_ENABLE_EGRESS_TIMESTAMP_FAILED");
  CANoeAPI_DetSetError(164, 0x0F, "ETHTSYN_E_INV_SYNCHRONIZED_TIME_BASE_REF");
  CANoeAPI_DetSetError(164, 0x01, "ETHTSYN_E_NOT_INITIALIZED");
  CANoeAPI_DetSetError(164, 0x09, "ETHTSYN_E_UPDATE_PHYS_ADDR_FILTER_FAILED");
  CANoeAPI_DetSetError(164, 0x04, "ETHTSYN _E_INV_MSG_LENGTH");
  CANoeAPI_DetSetError(164, 0x0D, "ETHTSYN_E_GET_EGRESS_TIMESTAMP_FAILED");
  CANoeAPI_DetSetError(164, 0x10, "ETHTSYN_E_INV_GPT_COUNTER_CHANNEL_REF");
  CANoeAPI_DetSetError(164, 0x0E, "ETHTSYN_E_INV_STATE");
  CANoeAPI_DetSetError(164, 0x0A, "ETHTSYN_E_SET_GLOBAL_TIME_FAILED");
}

/* Initialize DET codes for module EthTrcv*/
STATIC void VTTCntrl_DetInitialization_EthTrcv(void)
{
  CANoeAPI_DetSetModule(73, "EthTrcv");
  CANoeAPI_DetSetFunction(73, 0x07, "ETHTRCV_30_CANOEEMU_API_ID_GET_BAUD_RATE");
  CANoeAPI_DetSetFunction(73, 0x04, "ETHTRCV_30_CANOEEMU_API_ID_GET_TRANSCEIVER_MODE");
  CANoeAPI_DetSetFunction(73, 0x08, "ETHTRCV_30_CANOEEMU_API_ID_GET_DUPLEX_MODE");
  CANoeAPI_DetSetFunction(73, 0x06, "ETHTRCV_30_CANOEEMU_API_ID_GET_LINK_STATE");
  CANoeAPI_DetSetFunction(73, 0x09, "ETHTRCV_30_CANOEEMU_API_ID_GET_VERSION_INFO");
  CANoeAPI_DetSetFunction(73, 0x05, "ETHTRCV_30_CANOEEMU_API_ID_START_AUTO_NEG");
  CANoeAPI_DetSetFunction(73, 0x01, "ETHTRCV_30_CANOEEMU_API_ID_INIT");
  CANoeAPI_DetSetFunction(73, 0x02, "ETHTRCV_30_CANOEEMU_API_ID_TRANSCEIVER_INIT");
  CANoeAPI_DetSetFunction(73, 0x03, "ETHTRCV_30_CANOEEMU_API_ID_SET_TRANSCEIVER_MODE");
  CANoeAPI_DetSetError(73, 0x01, "ETHTRCV_30_CANOEEMU_E_INV_TRCV_IDX");
  CANoeAPI_DetSetError(73, 0x04, "ETHTRCV_30_CANOEEMU_E_INV_PARAM");
  CANoeAPI_DetSetError(73, 0x02, "ETHTRCV_30_CANOEEMU_E_NOT_INITIALIZED");
  CANoeAPI_DetSetError(73, 0x05, "ETHTRCV_30_CANOEEMU_E_INV_CONFIG");
  CANoeAPI_DetSetError(73, 0x03, "ETHTRCV_30_CANOEEMU_E_INV_POINTER");
}

/* Initialize DET codes for module Fee*/
STATIC void VTTCntrl_DetInitialization_Fee(void)
{
  CANoeAPI_DetSetModule(21, "Fee");
  CANoeAPI_DetSetFunction(21, 0x07, "Fee_InvalidateBlock");
  CANoeAPI_DetSetFunction(21, 0x10, "Fee_JobEndNotification");
  CANoeAPI_DetSetFunction(21, 0x22, "Fee_GetSectorSwitchStatus");
  CANoeAPI_DetSetFunction(21, 0x06, "Fee_GetJobResult");
  CANoeAPI_DetSetFunction(21, 0x02, "Fee_Read");
  CANoeAPI_DetSetFunction(21, 0x20, "Fee_GetEraseCycle");
  CANoeAPI_DetSetFunction(21, 0x09, "Fee_EraseImmediateBlock");
  CANoeAPI_DetSetFunction(21, 0x24, "Fee_ConvertBlockConfig");
  CANoeAPI_DetSetFunction(21, 0x23, "Fee_ForceSectorSwitch");
  CANoeAPI_DetSetFunction(21, 0x08, "Fee_GetVersionInfo");
  CANoeAPI_DetSetFunction(21, 0x11, "Fee_JobErrorNotification");
  CANoeAPI_DetSetFunction(21, 0x03, "Fee_Write");
  CANoeAPI_DetSetFunction(21, 0x01, "Fee_SetMode");
  CANoeAPI_DetSetFunction(21, 0x12, "Fee_MainFunction");
  CANoeAPI_DetSetFunction(21, 0x04, "Fee_Cancel");
  CANoeAPI_DetSetFunction(21, 0x00, "Fee_InitFee_InitEx");
  CANoeAPI_DetSetFunction(21, 0x05, "Fee_GetStatus");
  CANoeAPI_DetSetFunction(21, 0x21, "Fee_GetWriteCycle");
  CANoeAPI_DetSetError(21, 0x12, "FEE_E_PARAM_LENGTH_OFFSET");
  CANoeAPI_DetSetError(21, 0x13, "FEE_E_BUSY");
  CANoeAPI_DetSetError(21, 0x15, "FEE_E_PARAM_MODE");
  CANoeAPI_DetSetError(21, 0x02, "FEE_E_INVALID_BLOCK_NO");
  CANoeAPI_DetSetError(21, 0x11, "FEE_E_PARAM_SECTOR_NUMBER");
  CANoeAPI_DetSetError(21, 0x10, "FEE_E_PARAM_DATABUFFERPTR");
  CANoeAPI_DetSetError(21, 0x14, "FEE_E_NO_INIT");
}

/* Initialize DET codes for module FiM*/
STATIC void VTTCntrl_DetInitialization_FiM(void)
{
  CANoeAPI_DetSetModule(11, "FiM");
  CANoeAPI_DetSetFunction(11, 0x03, "FiM_DemInit");
  CANoeAPI_DetSetFunction(11, 0x00, "FiM_Init");
  CANoeAPI_DetSetFunction(11, 0x02, "FiM_DemTriggerOnEventStatus");
  CANoeAPI_DetSetFunction(11, 0x05, "FiM_MainFunction");
  CANoeAPI_DetSetFunction(11, 0x01, "FiM_GetFunctionPermission");
  CANoeAPI_DetSetFunction(11, 0x80, "FiM_GetFunctionPendingStatus");
  CANoeAPI_DetSetFunction(11, 0x04, "FiM_GetVersionInfo");
  CANoeAPI_DetSetError(11, 0x03, "FIM_E_FID_OUT_OF_RANGE");
  CANoeAPI_DetSetError(11, 0x01, "FIM_E_WRONG_PERMISSION_REQ");
  CANoeAPI_DetSetError(11, 0x80, "FIM_E_DEM_GETEVENTSTATUS_WRONG_RETURN_VALUE");
  CANoeAPI_DetSetError(11, 0x02, "FIM_E_WRONG_TRIGGER_ON_EVENT");
  CANoeAPI_DetSetError(11, 0x05, "FIM_E_INVALID_POINTER");
  CANoeAPI_DetSetError(11, 0x81, "FIM_E_INITIALIZATION_NOT_COMPLETED");
}

/* Initialize DET codes for module Fls*/
STATIC void VTTCntrl_DetInitialization_Fls(void)
{
  CANoeAPI_DetSetModule(92, "Fls");
  CANoeAPI_DetSetFunction(92, 0x20, "Fls_Copy");
  CANoeAPI_DetSetFunction(92, 0x03, "Fls_Cancel");
  CANoeAPI_DetSetFunction(92, 0x00, "Fls_Init");
  CANoeAPI_DetSetFunction(92, 0x05, "Fls_GetJobResult");
  CANoeAPI_DetSetFunction(92, 0x10, "Fls_GetVersionInfo");
  CANoeAPI_DetSetFunction(92, 0x08, "Fls_Compare");
  CANoeAPI_DetSetFunction(92, 0x06, "Fls_MainFunction");
  CANoeAPI_DetSetFunction(92, 0x02, "Fls_Write");
  CANoeAPI_DetSetFunction(92, 0x04, "Fls_GetStatus");
  CANoeAPI_DetSetFunction(92, 0x21, "Fls_ReadSync");
  CANoeAPI_DetSetFunction(92, 0x09, "Fls_SetMode");
  CANoeAPI_DetSetFunction(92, 0x07, "Fls_Read");
  CANoeAPI_DetSetFunction(92, 0x01, "Fls_Erase");
  CANoeAPI_DetSetError(92, 0x15, "FLS_E_PARAM_VINFO");
  CANoeAPI_DetSetError(92, 0x20, "FLS_E_ADDRESS_OVERLAP");
  CANoeAPI_DetSetError(92, 0x04, "FLS_E_PARAM_DATA");
  CANoeAPI_DetSetError(92, 0x05, "FLS_E_UNINIT");
  CANoeAPI_DetSetError(92, 0x01, "FLS_E_PARAM_CONFIG");
  CANoeAPI_DetSetError(92, 0x03, "FLS_E_PARAM_LENGTH");
  CANoeAPI_DetSetError(92, 0x06, "FLS_E_BUSY");
  CANoeAPI_DetSetError(92, 0x02, "FLS_E_PARAM_ADDRESS");
}

/* Initialize DET codes for module Fr*/
STATIC void VTTCntrl_DetInitialization_Fr(void)
{
  CANoeAPI_DetSetModule(81, "Fr");
  CANoeAPI_DetSetFunction(81, 0x28, "FR_API_ID_GET_CHANNEL_STATUS");
  CANoeAPI_DetSetFunction(81, 0x26, "FR_API_ID_DISABLE_LPDU");
  CANoeAPI_DetSetFunction(81, 0x16, "FR_API_ID_ENABLE_RELATIVE_TIMER_IRQ");
  CANoeAPI_DetSetFunction(81, 0x27, "FR_API_ID_GETNUMOFSTARTUPFRAMESoptional");
  CANoeAPI_DetSetFunction(81, 0x06, "FR_API_ID_SEND_WUP");
  CANoeAPI_DetSetFunction(81, 0x04, "FR_API_ID_HALT_COMMUNICATION");
  CANoeAPI_DetSetFunction(81, 0x15, "FR_API_ID_ENABLE_ABSOLUTE_TIMER_IRQ");
  CANoeAPI_DetSetFunction(81, 0x05, "FR_API_ID_ABORT_COMMUNICATION");
  CANoeAPI_DetSetFunction(81, 0x2E, "FR_API_ID_READCCCONFIG");
  CANoeAPI_DetSetFunction(81, 0x22, "FR_API_ID_GET_NM_VECTOR");
  CANoeAPI_DetSetFunction(81, 0x07, "FR_API_ID_SET_WAKEUP_CHANNEL");
  CANoeAPI_DetSetFunction(81, 0x00, "FR_API_ID_CONTROLLER_INIT");
  CANoeAPI_DetSetFunction(81, 0x1B, "FR_API_ID_GET_VERSION_INFO");
  CANoeAPI_DetSetFunction(81, 0x0C, "FR_API_ID_RECEIVE_RX_LPDU");
  CANoeAPI_DetSetFunction(81, 0x34, "FR_API_ID_LOCK_FTUoptional");
  CANoeAPI_DetSetFunction(81, 0x01, "FR_API_ID_SEND_MTS");
  CANoeAPI_DetSetFunction(81, 0x1C, "FR_API_ID_INIT");
  CANoeAPI_DetSetFunction(81, 0x20, "FR_API_ID_GET_ABSOLUTE_TIMER_IRQ_STATUS");
  CANoeAPI_DetSetFunction(81, 0x11, "FR_API_ID_SET_ABSOLUTE_TIMER");
  CANoeAPI_DetSetFunction(81, 0x1A, "FR_API_ID_DISABLE_RELATIVE_TIMER_IRQ");
  CANoeAPI_DetSetFunction(81, 0x33, "FR_API_ID_UNLOCK_RX_LPDU_DBAoptional");
  CANoeAPI_DetSetFunction(81, 0x2A, "FR_API_ID_GET_SYNC_FRAME_LISToptional");
  CANoeAPI_DetSetFunction(81, 0x0D, "FR_API_ID_CHECK_TX_LPDU_STATUS");
  CANoeAPI_DetSetFunction(81, 0x29, "FR_API_ID_GET_CLOCK_CORRECTION");
  CANoeAPI_DetSetFunction(81, 0x18, "FR_API_ID_ACK_RELATIVE_TIMER_IRQ");
  CANoeAPI_DetSetFunction(81, 0x17, "FR_API_ID_ACK_ABSOLUTE_TIMER_IRQ");
  CANoeAPI_DetSetFunction(81, 0x24, "FR_API_ID_ALLSLOTSoptional");
  CANoeAPI_DetSetFunction(81, 0x02, "FR_API_ID_CHECK_MTS");
  CANoeAPI_DetSetFunction(81, 0x12, "FR_API_ID_SET_RELATIVE_TIMER");
  CANoeAPI_DetSetFunction(81, 0x31, "FR_API_ID_TRANSMIT_TX_LPDU_DBAoptional");
  CANoeAPI_DetSetFunction(81, 0x32, "FR_API_ID_RECEIVE_RX_LPDU_DBAoptional");
  CANoeAPI_DetSetFunction(81, 0x35, "FR_API_ID_UNLOCK_FTUoptional");
  CANoeAPI_DetSetFunction(81, 0x13, "FR_API_ID_CANCEL_ABSOLUTE_TIMER");
  CANoeAPI_DetSetFunction(81, 0x03, "FR_API_ID_START_COMMUNICATION");
  CANoeAPI_DetSetFunction(81, 0x1D, "FR_API_ID_STOP_MTS");
  CANoeAPI_DetSetFunction(81, 0x09, "FR_API_ID_GET_SYNC_STATE");
  CANoeAPI_DetSetFunction(81, 0x0A, "FR_API_ID_GET_POC_STATUS");
  CANoeAPI_DetSetFunction(81, 0x19, "FR_API_ID_DISABLE_ABSOLUTE_TIMER_IRQ");
  CANoeAPI_DetSetFunction(81, 0x10, "FR_API_ID_GET_GLOBAL_TIME");
  CANoeAPI_DetSetFunction(81, 0x08, "FR_API_ID_SET_EXT_SYNC");
  CANoeAPI_DetSetFunction(81, 0x0B, "FR_API_ID_TRANSMIT_TX_LPDU");
  CANoeAPI_DetSetFunction(81, 0x30, "FR_API_ID_REQUEST_BUFFER_DBAoptional");
  CANoeAPI_DetSetFunction(81, 0x21, "FR_API_ID_GET_RELATIVE_TIMER_IRQ_STATUS");
  CANoeAPI_DetSetFunction(81, 0x1F, "FR_API_ID_PREPARE_LPDU");
  CANoeAPI_DetSetFunction(81, 0x23, "FR_API_ID_ALLOW_COLDSTART");
  CANoeAPI_DetSetFunction(81, 0x2B, "FR_API_ID_GETWAKEUPRXSTATUSoptional");
  CANoeAPI_DetSetFunction(81, 0x36, "FR_API_ID_TRANSMIT_TX_LPDU_IMMEDIATE_DBAoptional");
  CANoeAPI_DetSetFunction(81, 0x2D, "FR_API_ID_CANCELTXLPDUoptional");
  CANoeAPI_DetSetFunction(81, 0x25, "FR_API_ID_RECONFIG_LPDU");
  CANoeAPI_DetSetFunction(81, 0x14, "FR_API_ID_CANCEL_RELATIVE_TIMER");
  CANoeAPI_DetSetError(81, 0x01, "FR_E_INV_TIMER_IDX");
  CANoeAPI_DetSetError(81, 0x03, "FR_E_INV_OFFSET");
  CANoeAPI_DetSetError(81, 0x06, "FR_E_INV_CYCLE");
  CANoeAPI_DetSetError(81, 0x0B, "FR_E_INV_LPDU_IDX");
  CANoeAPI_DetSetError(81, 0x40, "FR_E_INV_LISTSIZE");
  CANoeAPI_DetSetError(81, 0x70, "FR_E_INV_CANOE_CLUSTER_NAME");
  CANoeAPI_DetSetError(81, 0x02, "FR_E_INV_POINTER");
  CANoeAPI_DetSetError(81, 0x04, "FR_E_INV_CTRL_IDX");
  CANoeAPI_DetSetError(81, 0x07, "FR_E_INV_CONFIG");
  CANoeAPI_DetSetError(81, 0x0D, "FR_E_INV_CONFIG_IDX");
  CANoeAPI_DetSetError(81, 0x0A, "FR_E_INV_LENGTH");
  CANoeAPI_DetSetError(81, 0x0C, "FR_E_INV_HEADERCRC");
  CANoeAPI_DetSetError(81, 0x09, "FR_E_INV_POCSTATE");
  CANoeAPI_DetSetError(81, 0x05, "FR_E_INV_CHNL_IDX");
  CANoeAPI_DetSetError(81, 0x08, "FR_E_NOT_INITIALIZED");
}

/* Initialize DET codes for module FrIf*/
STATIC void VTTCntrl_DetInitialization_FrIf(void)
{
  CANoeAPI_DetSetModule(61, "FrIf");
  CANoeAPI_DetSetFunction(61, 0x34, "FrIf_GetNumOfStartupFrames");
  CANoeAPI_DetSetFunction(61, 0x2B, "FrIf_GetTransceiverWUReason");
  CANoeAPI_DetSetFunction(61, 0x33, "FrIf_AllSlots");
  CANoeAPI_DetSetFunction(61, 0x33, "FrIf_CancelTransmitoptional");
  CANoeAPI_DetSetFunction(61, 0x29, "FrIf_GetClockCorrectionoptional");
  CANoeAPI_DetSetFunction(61, 0x09, "FrIf_HaltCommunication");
  CANoeAPI_DetSetFunction(61, 0x11, "FrIf_GetMacroticksDuration");
  CANoeAPI_DetSetFunction(61, 0x44, "FrIf_GetTxConflictStatus");
  CANoeAPI_DetSetFunction(61, 0x03, "FrIf_ControllerInit");
  CANoeAPI_DetSetFunction(61, 0x46, "FrIf_DisableLPduoptional");
  CANoeAPI_DetSetFunction(61, 0x26, "FrIf_DisableAbsoluteTimerIRQ");
  CANoeAPI_DetSetFunction(61, 0x25, "FrIf_AckRelativeTimerIRQ");
  CANoeAPI_DetSetFunction(61, 0x47, "FrIf_GetSyncFrameListoptional");
  CANoeAPI_DetSetFunction(61, 0x19, "FrIf_GetPOCStatus");
  CANoeAPI_DetSetFunction(61, 0x2E, "FrIf_DisableTransceiverBranch");
  CANoeAPI_DetSetFunction(61, 0x2C, "FrIf_EnableTransceiverBranch");
  CANoeAPI_DetSetFunction(61, 0x45, "FrIf_AllowColdstart");
  CANoeAPI_DetSetFunction(61, 0x21, "FrIf_CancelRelativeTimer");
  CANoeAPI_DetSetFunction(61, 0x3C, "FrIf_GetChannelStatusoptional");
  CANoeAPI_DetSetFunction(61, 0x1F, "FrIf_SetRelativeTimer");
  CANoeAPI_DetSetFunction(61, 0x42, "FrIf_TxConfirmationFunctions");
  CANoeAPI_DetSetFunction(61, 0x2A, "FrIf_GetTransceiverMode");
  CANoeAPI_DetSetFunction(61, 0x28, "FrIf_SetTransceiverMode");
  CANoeAPI_DetSetFunction(61, 0x2B, "FrIf_GetWakeupRxStatus");
  CANoeAPI_DetSetFunction(61, 0x27, "FrIf_DisableRelativeTimerIRQ");
  CANoeAPI_DetSetFunction(61, 0x24, "FrIf_AckAbsoluteTimerIRQ");
  CANoeAPI_DetSetFunction(61, 0x35, "FrIf_GetState");
  CANoeAPI_DetSetFunction(61, 0x41, "FrIf_TriggerTransmitFunctions");
  CANoeAPI_DetSetFunction(61, 0x39, "FrIf_CheckWakeupByTransceiver");
  CANoeAPI_DetSetFunction(61, 0x3D, "FrIf_ReadCCConfigoptional");
  CANoeAPI_DetSetFunction(61, 0x11, "FrIf_GetMacroticksPerCycle");
  CANoeAPI_DetSetFunction(61, 0x02, "FrIf_Init");
  CANoeAPI_DetSetFunction(61, 0x17, "FrIf_SetExtSync");
  CANoeAPI_DetSetFunction(61, 0x23, "FrIf_EnableRelativeTimerIRQ");
  CANoeAPI_DetSetFunction(61, 0x2C, "FrIf_EnableTransceiverWakeup");
  CANoeAPI_DetSetFunction(61, 0x01, "FrIf_GetVersionInfo");
  CANoeAPI_DetSetFunction(61, 0x0B, "FrIf_AbortCommunication");
  CANoeAPI_DetSetFunction(61, 0x44, "FrIf_SetState");
  CANoeAPI_DetSetFunction(61, 0x30, "FrIf_ClearTransceiverWakeup");
  CANoeAPI_DetSetFunction(61, 0x11, "FrIf_SetWakeupChannel");
  CANoeAPI_DetSetFunction(61, 0x2E, "FrIf_DisableTransceiverWakeup");
  CANoeAPI_DetSetFunction(61, 0x40, "FrIf_ExecCurrentJob_0");
  CANoeAPI_DetSetFunction(61, 0x3A, "FrIf_GetCycleLength");
  CANoeAPI_DetSetFunction(61, 0x07, "FrIf_StartCommunication");
  CANoeAPI_DetSetFunction(61, 0x35, "FrIf_GetTransceiverError");
  CANoeAPI_DetSetFunction(61, 0x06, "FrIf_Transmit");
  CANoeAPI_DetSetFunction(61, 0x3A, "FrIf_GetRelativeTimerIRQStatus");
  CANoeAPI_DetSetFunction(61, 0x1E, "FrIf_SetAbsoluteTimer");
  CANoeAPI_DetSetFunction(61, 0x39, "FrIf_GetAbsoluteTimerIRQStatus");
  CANoeAPI_DetSetFunction(61, 0x00, "FrIf_ReconfigLPdu");
  CANoeAPI_DetSetFunction(61, 0x12, "FrIf_SendWUP");
  CANoeAPI_DetSetFunction(61, 0x33, "FrIf_MainFunction_0");
  CANoeAPI_DetSetFunction(61, 0x34, "FrIf_GetNmVector");
  CANoeAPI_DetSetFunction(61, 0x1A, "FrIf_GetGlobalTime");
  CANoeAPI_DetSetFunction(61, 0x43, "FrIf_RxIndicationFunctions");
  CANoeAPI_DetSetFunction(61, 0x22, "FrIf_EnableAbsoluteTimerIRQ");
  CANoeAPI_DetSetFunction(61, 0x32, "FrIf_JobListExec_0");
  CANoeAPI_DetSetFunction(61, 0x20, "FrIf_CancelAbsoluteTimer");
  CANoeAPI_DetSetFunction(61, 0x16, "FrIf_GetSyncState");
  CANoeAPI_DetSetFunction(61, 0x3B, "FrIf_Cbk_WakeupByTransceiver");
  CANoeAPI_DetSetError(61, 0x02, "FRIF_E_INV_CTRL_IDX");
  CANoeAPI_DetSetError(61, 0x0A, "FRIF_E_INV_LPDU_IDX");
  CANoeAPI_DetSetError(61, 0x09, "FRIF_E_INV_FRIF_CC_STATE");
  CANoeAPI_DetSetError(61, 0x08, "FRIF_E_NOT_INITIALIZED");
  CANoeAPI_DetSetError(61, 0x05, "FRIF_E_INV_TIMER_IDX");
  CANoeAPI_DetSetError(61, 0x06, "FRIF_E_INV_TXPDUID");
  CANoeAPI_DetSetError(61, 0x26, "FRIF_E_TXTASK_RET_E_NOT_OK");
  CANoeAPI_DetSetError(61, 0x25, "FRIF_E_INVALID_PB_CONFIG");
  CANoeAPI_DetSetError(61, 0x04, "FRIF_E_INV_CHNL_IDX");
  CANoeAPI_DetSetError(61, 0x09, "FRIF_E_JLE_SYNC");
  CANoeAPI_DetSetError(61, 0x27, "FRIF_E_INVALID_PDU_OWNER");
  CANoeAPI_DetSetError(61, 0x03, "FRIF_E_INV_CLST_IDX");
  CANoeAPI_DetSetError(61, 0x01, "FRIF_E_INV_POINTER");
  CANoeAPI_DetSetError(61, 0x0B, "FRIF_E_INV_FRAME_ID");
}

/* Initialize DET codes for module FrNm*/
STATIC void VTTCntrl_DetInitialization_FrNm(void)
{
  CANoeAPI_DetSetModule(32, "FrNm");
  CANoeAPI_DetSetFunction(32, 0x0A, "FrNm_GetNodeIdentifier");
  CANoeAPI_DetSetFunction(32, 0xE4, "FrNm_TriggerTransmit");
  CANoeAPI_DetSetFunction(32, 0x0F, "FrNm_GetVersionInfo");
  CANoeAPI_DetSetFunction(32, 0x03, "FrNm_NetworkRelease");
  CANoeAPI_DetSetFunction(32, 0xE1, "FrNm_RxIndication");
  CANoeAPI_DetSetFunction(32, 0xC0, "FrNm_RequestBusSynchronization");
  CANoeAPI_DetSetFunction(32, 0x01, "FrNm_PassiveStartUp");
  CANoeAPI_DetSetFunction(32, 0xEF, "FrNm_StartupError");
  CANoeAPI_DetSetFunction(32, 0x06, "FrNm_SetUserData");
  CANoeAPI_DetSetFunction(32, 0x05, "FrNm_EnableCommunication");
  CANoeAPI_DetSetFunction(32, 0x11, "FrNm_Transmit");
  CANoeAPI_DetSetFunction(32, 0x0D, "FrNm_CheckRemoteSleepIndication");
  CANoeAPI_DetSetFunction(32, 0x08, "FrNm_GetPduData");
  CANoeAPI_DetSetFunction(32, 0x07, "FrNm_GetUserData");
  CANoeAPI_DetSetFunction(32, 0x02, "FrNm_NetworkRequest");
  CANoeAPI_DetSetFunction(32, 0x00, "FrNm_Init");
  CANoeAPI_DetSetFunction(32, 0x09, "FrNm_RepeatMessageRequest");
  CANoeAPI_DetSetFunction(32, 0x0C, "FrNm_DisableCommunication");
  CANoeAPI_DetSetFunction(32, 0xF0, "FrNm_MainFunction_X");
  CANoeAPI_DetSetFunction(32, 0x0E, "FrNm_GetState");
  CANoeAPI_DetSetFunction(32, 0xE0, "FrNm_TxConfirmation");
  CANoeAPI_DetSetFunction(32, 0x12, "FrNm_SetSleepReadyBit");
  CANoeAPI_DetSetFunction(32, 0x0B, "FrNm_GetLocalNodeIdentifier");
  CANoeAPI_DetSetError(32, 0x01, "FRNM_E_UNINIT");
  CANoeAPI_DetSetError(32, 0x10, "FRNM_E_INIT_FAILED");
  CANoeAPI_DetSetError(32, 0x02, "FRNM_E_INVALID_CHANNEL");
  CANoeAPI_DetSetError(32, 0x11, "FRNM_E_RXINDICATION_DLC_ERROR9");
  CANoeAPI_DetSetError(32, 0x12, "FRNM_E_PDUR_TRIGGERTX_ERROR9");
  CANoeAPI_DetSetError(32, 0x04, "FRNM_E_PDU_ID_INVALID");
  CANoeAPI_DetSetError(32, 0x03, "FRNM_E_INVALID_POINTER");
}

/* Initialize DET codes for module FrSM*/
STATIC void VTTCntrl_DetInitialization_FrSM(void)
{
  CANoeAPI_DetSetModule(142, "FrSM");
  CANoeAPI_DetSetFunction(142, 0x05, "FrSM_AllSlots");
  CANoeAPI_DetSetFunction(142, 0x06, "FrSM_SetEcuPassive");
  CANoeAPI_DetSetFunction(142, 0x80, "FrSM_MainFunction_ClusterId");
  CANoeAPI_DetSetFunction(142, 0x02, "FrSM_RequestComMode");
  CANoeAPI_DetSetFunction(142, 0x01, "FrSM_Init");
  CANoeAPI_DetSetFunction(142, 0x03, "FrSM_GetCurrentComMode");
  CANoeAPI_DetSetFunction(142, 0x04, "FrSM_GetVersionInfo");
  CANoeAPI_DetSetError(142, 0x02, "FRSM_E_INV_HANDLE");
  CANoeAPI_DetSetError(142, 0x01, "FRSM_E_NULL_PTR");
  CANoeAPI_DetSetError(142, 0x04, "FRSM_E_INV_MODE");
  CANoeAPI_DetSetError(142, 0x03, "FRSM_E_UNINIT");
}

/* Initialize DET codes for module FrTSyn*/
STATIC void VTTCntrl_DetInitialization_FrTSyn(void)
{
  CANoeAPI_DetSetModule(163, "FrTSyn");
  CANoeAPI_DetSetFunction(163, 0x03, "FrTSyn_SetTransmissionMode");
  CANoeAPI_DetSetFunction(163, 0x02, "FrTSyn_GetVersionInfo");
  CANoeAPI_DetSetFunction(163, 0x04, "FrTSyn_MainFunction");
  CANoeAPI_DetSetFunction(163, 0x41, "FrTSyn_TriggerTransmit");
  CANoeAPI_DetSetFunction(163, 0x42, "FrTSyn_RxIndication");
  CANoeAPI_DetSetFunction(163, 0x01, "FrTSyn_Init");
  CANoeAPI_DetSetError(163, 0x21, "FRTSYN_E_NULL_POINTER");
  CANoeAPI_DetSetError(163, 0x20, "FRTSYN_E_NOT_INITIALIZED");
  CANoeAPI_DetSetError(163, 0x01, "FRTSYN_E_INVALID_PDUID");
}

/* Initialize DET codes for module FrTp*/
STATIC void VTTCntrl_DetInitialization_FrTp(void)
{
  CANoeAPI_DetSetModule(36, "FrTp");
  CANoeAPI_DetSetFunction(36, 0x01, "FrTp_Shutdown");
  CANoeAPI_DetSetFunction(36, 0x10, "FrTp_MainFunction");
  CANoeAPI_DetSetFunction(36, 0x27, "FrTp_GetVersionInfo");
  CANoeAPI_DetSetFunction(36, 0x02, "FrTp_Transmit");
  CANoeAPI_DetSetFunction(36, 0x00, "FrTp_Init");
  CANoeAPI_DetSetFunction(36, 0x41, "FrTp_TriggerTransmit");
  CANoeAPI_DetSetFunction(36, 0x03, "FrTp_CancelTransmit");
  CANoeAPI_DetSetFunction(36, 0x04, "FrTp_ChangeParameter");
  CANoeAPI_DetSetFunction(36, 0x42, "FrTp_RxIndication");
  CANoeAPI_DetSetFunction(36, 0x08, "FrTp_CancelReceive");
  CANoeAPI_DetSetFunction(36, 0x40, "FrTp_TxConfirmation");
  CANoeAPI_DetSetError(36, 0x02, "FRTP_E_NULL_PTR");
  CANoeAPI_DetSetError(36, 0x05, "FRTP_E_SEG_ERROR");
  CANoeAPI_DetSetError(36, 0x01, "FRTP_E_UNINIT");
  CANoeAPI_DetSetError(36, 0x06, "FRTP_E_UMSG_LENGTH_ERROR");
  CANoeAPI_DetSetError(36, 0x03, "FRTP_E_INVALID_PDU_SDU_ID");
  CANoeAPI_DetSetError(36, 0x04, "FRTP_E_INVALID_PARAMETER");
  CANoeAPI_DetSetError(36, 0x07, "FRTP_E_NO_CHANNEL");
}

/* Initialize DET codes for module FrXcp*/
STATIC void VTTCntrl_DetInitialization_FrXcp(void)
{
  CANoeAPI_DetSetModule(211, "FrXcp");
  CANoeAPI_DetSetFunction(211, 0x02, "FrXcp_Send (CTO)");
  CANoeAPI_DetSetFunction(211, 0x00, "FrXcp_Init");
  CANoeAPI_DetSetFunction(211, 0xc8, "FrXcp_MainFunctionRx");
  CANoeAPI_DetSetFunction(211, 0x66, "FrXcp_RxIndication");
  CANoeAPI_DetSetFunction(211, 0x67, "FrXcp_TriggerTransmit");
  CANoeAPI_DetSetFunction(211, 0x05, "FrXcp_Control");
  CANoeAPI_DetSetFunction(211, 0x01, "FrXcp_TLService");
  CANoeAPI_DetSetFunction(211, 0x65, "FrXcp_TxConfirmation");
  CANoeAPI_DetSetFunction(211, 0x06, "FrXcp_GetVersionInfo");
  CANoeAPI_DetSetFunction(211, 0x03, "FrXcp_Send (DTO)");
  CANoeAPI_DetSetFunction(211, 0x07, "FrXcp_SetPduMode");
  CANoeAPI_DetSetFunction(211, 0x04, "FrXcp_SendFlush");
  CANoeAPI_DetSetFunction(211, 0xc9, "FrXcp_MainFunctionTx");
  CANoeAPI_DetSetError(211, 0x03, "FRXCP_E_INV_POINTER");
  CANoeAPI_DetSetError(211, 0x02, "FRXCP_E_INV_PDU_ID");
  CANoeAPI_DetSetError(211, 0x04, "FRXCP_E_RX_BUFFER_OVERFLOW");
  CANoeAPI_DetSetError(211, 0x01, "FRXCP_E_NO_INIT");
  CANoeAPI_DetSetError(211, 0x05, "FRXCP_E_WRONG_CONTROL_COMMAND");
}

/* Initialize DET codes for module Gpt*/
STATIC void VTTCntrl_DetInitialization_Gpt(void)
{
  CANoeAPI_DetSetModule(100, "Gpt");
  CANoeAPI_DetSetFunction(100, 0x09, "Gpt_SetMode");
  CANoeAPI_DetSetFunction(100, 0x00, "Gpt_GetVersionInfo");
  CANoeAPI_DetSetFunction(100, 0x01, "Gpt_Init");
  CANoeAPI_DetSetFunction(100, 0x06, "Gpt_StopTimer");
  CANoeAPI_DetSetFunction(100, 0x03, "Gpt_GetTimerElapsed");
  CANoeAPI_DetSetFunction(100, 0x0C, "Gpt_CheckWakeup");
  CANoeAPI_DetSetFunction(100, 0x05, "Gpt_StartTimer");
  CANoeAPI_DetSetFunction(100, 0x07, "Gpt_EnableNotification");
  CANoeAPI_DetSetFunction(100, 0x0B, "Gpt_EnableWakeup");
  CANoeAPI_DetSetFunction(100, 0x02, "Gpt_DeInit");
  CANoeAPI_DetSetFunction(100, 0x0A, "Gpt_DisableWakeup");
  CANoeAPI_DetSetFunction(100, 0x08, "Gpt_DisableNotification");
  CANoeAPI_DetSetFunction(100, 0x04, "Gpt_GetTimerRemaining");
  CANoeAPI_DetSetError(100, 0x0D, "GPT_E_ALREADY_INITIALIZED");
  CANoeAPI_DetSetError(100, 0x0B, "GPT_E_BUSY");
  CANoeAPI_DetSetError(100, 0x14, "GPT_E_PARAM_CHANNEL");
  CANoeAPI_DetSetError(100, 0x0A, "GPT_E_UNINIT");
  CANoeAPI_DetSetError(100, 0x21, "GPT_E_PARAM_CONFIG");
  CANoeAPI_DetSetError(100, 0x15, "GPT_E_PARAM_VALUE");
  CANoeAPI_DetSetError(100, 0x22, "GPT_E_PARAM_VINFO");
  CANoeAPI_DetSetError(100, 0x1F, "GPT_E_PARAM_MODE");
}

/* Initialize DET codes for module Icu*/
STATIC void VTTCntrl_DetInitialization_Icu(void)
{
  CANoeAPI_DetSetModule(122, "Icu");
  CANoeAPI_DetSetFunction(122, 0x02, "Icu_SetMode");
  CANoeAPI_DetSetFunction(122, 0x0E, "Icu_DisableEdgeCount");
  CANoeAPI_DetSetFunction(122, 0x0F, "Icu_GetEdgeNumbers");
  CANoeAPI_DetSetFunction(122, 0x00, "Icu_Init");
  CANoeAPI_DetSetFunction(122, 0x16, "Icu_EnableEdgeDetection");
  CANoeAPI_DetSetFunction(122, 0x11, "Icu_GetDutyCycleValues");
  CANoeAPI_DetSetFunction(122, 0x09, "Icu_StartTimestamp");
  CANoeAPI_DetSetFunction(122, 0x08, "Icu_GetInputState");
  CANoeAPI_DetSetFunction(122, 0x12, "Icu_GetVersionInfo");
  CANoeAPI_DetSetFunction(122, 0x0B, "Icu_GetTimestampIndex");
  CANoeAPI_DetSetFunction(122, 0x01, "Icu_DeInit");
  CANoeAPI_DetSetFunction(122, 0x17, "Icu_DisableEdgeDetection");
  CANoeAPI_DetSetFunction(122, 0x15, "Icu_CheckWakeup");
  CANoeAPI_DetSetFunction(122, 0x04, "Icu_EnableWakeup");
  CANoeAPI_DetSetFunction(122, 0x10, "Icu_GetTimeElapsed");
  CANoeAPI_DetSetFunction(122, 0x0C, "Icu_ResetEdgeCount");
  CANoeAPI_DetSetFunction(122, 0x03, "Icu_DisableWakeup");
  CANoeAPI_DetSetFunction(122, 0x07, "Icu_EnableNotification");
  CANoeAPI_DetSetFunction(122, 0x0D, "Icu_EnableEdgeCount");
  CANoeAPI_DetSetFunction(122, 0x05, "Icu_SetActivationCondition");
  CANoeAPI_DetSetFunction(122, 0x06, "Icu_DisableNotification");
  CANoeAPI_DetSetFunction(122, 0x0A, "Icu_StopTimestamp");
  CANoeAPI_DetSetFunction(122, 0x13, "Icu_StartSignalMeasurement");
  CANoeAPI_DetSetFunction(122, 0x14, "Icu_StopSignalMeasurement");
  CANoeAPI_DetSetError(122, 0x0C, "ICU_E_PARAM_ACTIVATION");
  CANoeAPI_DetSetError(122, 0x0F, "ICU_E_PARAM_MODE");
  CANoeAPI_DetSetError(122, 0x17, "ICU_ALREADY_INITIALIZED");
  CANoeAPI_DetSetError(122, 0x0A, "ICU_E_PARAM_CONFIG");
  CANoeAPI_DetSetError(122, 0x0D, "ICU_E_PARAM_BUFFER_PTR");
  CANoeAPI_DetSetError(122, 0x18, "ICU_PARAM_NOTIFY_INTERVAL");
  CANoeAPI_DetSetError(122, 0x16, "ICU_E_BUSY_OPERATION");
  CANoeAPI_DetSetError(122, 0x0E, "ICU_E_PARAM_BUFFER_SIZE");
  CANoeAPI_DetSetError(122, 0x19, "ICU_E_PARAM_VINFO");
  CANoeAPI_DetSetError(122, 0x0B, "ICU_E_PARAM_CHANNEL");
  CANoeAPI_DetSetError(122, 0x15, "ICU_E_NOT_STARTED");
  CANoeAPI_DetSetError(122, 0x14, "ICU_E_UNINIT");
}

/* Initialize DET codes for module IoHwAb*/
STATIC void VTTCntrl_DetInitialization_IoHwAb(void)
{
  CANoeAPI_DetSetModule(254, "IoHwAb");
  CANoeAPI_DetSetFunction(254, 0x10, "IoHwAb_GetVersionInfo");
  CANoeAPI_DetSetFunction(254, 0x01, "IoHwAb_Init");
  CANoeAPI_DetSetError(254, 0x10, "IOHWAB_E_INV_POINTER");
}

/* Initialize DET codes for module J1939Dcm*/
STATIC void VTTCntrl_DetInitialization_J1939Dcm(void)
{
  CANoeAPI_DetSetModule(58, "J1939Dcm");
  CANoeAPI_DetSetFunction(58, 0x04, "J1939Dcm_MainFunction");
  CANoeAPI_DetSetFunction(58, 0x80, "J1939Dcm_InitMemory");
  CANoeAPI_DetSetFunction(58, 0x05, "J1939Dcm_CopyRxData");
  CANoeAPI_DetSetFunction(58, 0x06, "J1939Dcm_CopyTxData");
  CANoeAPI_DetSetFunction(58, 0x01, "J1939Dcm_Init");
  CANoeAPI_DetSetFunction(58, 0x09, "J1939Dcm_TpTxConfirmation");
  CANoeAPI_DetSetFunction(58, 0x43, "J1939Dcm_RequestIndication");
  CANoeAPI_DetSetFunction(58, 0x50, "J1939Dcm_MemResponseTransmit");
  CANoeAPI_DetSetFunction(58, 0x42, "J1939Dcm_RxIndication");
  CANoeAPI_DetSetFunction(58, 0x08, "J1939Dcm_TpRxIndication");
  CANoeAPI_DetSetFunction(58, 0x03, "J1939Dcm_GetVersionInfo");
  CANoeAPI_DetSetFunction(58, 0x02, "J1939Dcm_DeInit");
  CANoeAPI_DetSetFunction(58, 0x07, "J1939Dcm_StartOfReception");
  CANoeAPI_DetSetFunction(58, 0x0B, "J1939Dcm_SetState");
  CANoeAPI_DetSetFunction(58, 0x0A, "J1939Dcm_DemTriggerOnDTCStatus");
  CANoeAPI_DetSetFunction(58, 0x0C, "J1939Dcm_TxConfirmation");
  CANoeAPI_DetSetFunction(58, 0x51, "J1939Dcm_MemDataTransmit");
  CANoeAPI_DetSetError(58, 0x10, "J1939DCM_E_PARAM_CONFIG");
  CANoeAPI_DetSetError(58, 0x11, "J1939DCM_E_OVERRUN");
  CANoeAPI_DetSetError(58, 0x02, "J1939DCM_E_PARAM_POINTER");
  CANoeAPI_DetSetError(58, 0x09, "J1939DCM_E_INVALID_PGN");
  CANoeAPI_DetSetError(58, 0x0B, "J1939DCM_E_INVALID_CHANNEL");
  CANoeAPI_DetSetError(58, 0x21, "J1939DCM_E_REINIT");
  CANoeAPI_DetSetError(58, 0x08, "J1939DCM_E_INVALID_NODE");
  CANoeAPI_DetSetError(58, 0x20, "J1939DCM_E_UNINIT");
  CANoeAPI_DetSetError(58, 0x07, "J1939DCM_E_PARAM_ID");
  CANoeAPI_DetSetError(58, 0x06, "J1939DCM_E_INVALID_STATE");
  CANoeAPI_DetSetError(58, 0x01, "J1939DCM_E_INVALID_PDU_SDU_ID");
  CANoeAPI_DetSetError(58, 0x0A, "J1939DCM_E_INVALID_PRIO");
}

/* Initialize DET codes for module J1939Nm*/
STATIC void VTTCntrl_DetInitialization_J1939Nm(void)
{
  CANoeAPI_DetSetModule(34, "J1939Nm");
  CANoeAPI_DetSetFunction(34, 0x89, "J1939Nm_FindNodeByName");
  CANoeAPI_DetSetFunction(34, 0x8c, "J1939Nm_SetAddress");
  CANoeAPI_DetSetFunction(34, 0x8a, "J1939Nm_FindNodeByAddress");
  CANoeAPI_DetSetFunction(34, 0x85, "J1939Nm_BusOffEnd");
  CANoeAPI_DetSetFunction(34, 0x82, "J1939Nm_CopyRxData");
  CANoeAPI_DetSetFunction(34, 0x43, "J1939Nm_RequestIndication");
  CANoeAPI_DetSetFunction(34, 0x40, "J1939Nm_TxConfirmation");
  CANoeAPI_DetSetFunction(34, 0x04, "J1939Nm_MainFunction");
  CANoeAPI_DetSetFunction(34, 0x03, "J1939Nm_GetVersionInfo");
  CANoeAPI_DetSetFunction(34, 0x80, "J1939Nm_InitMemory");
  CANoeAPI_DetSetFunction(34, 0x8b, "J1939Nm_SetName");
  CANoeAPI_DetSetFunction(34, 0x05, "J1939Nm_NetworkRequest");
  CANoeAPI_DetSetFunction(34, 0x0f, "J1939Nm_PassiveStartup");
  CANoeAPI_DetSetFunction(34, 0x01, "J1939Nm_Init");
  CANoeAPI_DetSetFunction(34, 0x81, "J1939Nm_StartOfReception");
  CANoeAPI_DetSetFunction(34, 0x02, "J1939Nm_DeInit");
  CANoeAPI_DetSetFunction(34, 0x0d, "J1939Nm_GetState");
  CANoeAPI_DetSetFunction(34, 0x42, "J1939Nm_RxIndication");
  CANoeAPI_DetSetFunction(34, 0x06, "J1939Nm_NetworkRelease");
  CANoeAPI_DetSetFunction(34, 0x10, "J1939Nm_GetBusOffDelay");
  CANoeAPI_DetSetFunction(34, 0x87, "J1939Nm_GetFirstUnknownNameIdx");
  CANoeAPI_DetSetFunction(34, 0x83, "J1939Nm_TpRxIndication");
  CANoeAPI_DetSetFunction(34, 0x88, "J1939Nm_GetLastNodeIdx");
  CANoeAPI_DetSetFunction(34, 0x86, "J1939Nm_GetNode");
  CANoeAPI_DetSetError(34, 0x82, "J1939NM_E_DUPLICATE_NAME");
  CANoeAPI_DetSetError(34, 0x80, "J1939NM_E_INVALID_PDU_SIZE");
  CANoeAPI_DetSetError(34, 0x03, "J1939NM_E_PARAM_POINTER");
  CANoeAPI_DetSetError(34, 0x02, "J1939NM_E_REINIT");
  CANoeAPI_DetSetError(34, 0x05, "J1939NM_E_INVALID_NETWORK_ID");
  CANoeAPI_DetSetError(34, 0x83, "J1939NM_E_EXTERNAL_NODE");
  CANoeAPI_DetSetError(34, 0x07, "J1939NM_E_INVALID_PRIO");
  CANoeAPI_DetSetError(34, 0x08, "J1939NM_E_INVALID_ADDRESS");
  CANoeAPI_DetSetError(34, 0x06, "J1939NM_E_INVALID_PGN");
  CANoeAPI_DetSetError(34, 0x0a, "J1939NM_E_INIT_FAILED");
  CANoeAPI_DetSetError(34, 0x09, "J1939NM_E_INVALID_NODE");
  CANoeAPI_DetSetError(34, 0x01, "J1939NM_E_UNINIT");
  CANoeAPI_DetSetError(34, 0x90, "J1939NM_E_DUMMY_API");
  CANoeAPI_DetSetError(34, 0x04, "J1939NM_E_INVALID_PDU_SDU_ID");
  CANoeAPI_DetSetError(34, 0x81, "J1939NM_E_TIMEOUT_TXCONF");
}

/* Initialize DET codes for module J1939Rm*/
STATIC void VTTCntrl_DetInitialization_J1939Rm(void)
{
  CANoeAPI_DetSetModule(59, "J1939Rm");
  CANoeAPI_DetSetFunction(59, 0x01, "J1939Rm_Init");
  CANoeAPI_DetSetFunction(59, 0x09, "J1939Rm_SendAck");
  CANoeAPI_DetSetFunction(59, 0x03, "J1939Rm_GetVersionInfo");
  CANoeAPI_DetSetFunction(59, 0x04, "J1939Rm_MainFunction");
  CANoeAPI_DetSetFunction(59, 0x80, "J1939Rm_InitMemory");
  CANoeAPI_DetSetFunction(59, 0x08, "J1939Rm_CancelRequestTimeout");
  CANoeAPI_DetSetFunction(59, 0x42, "J1939Rm_RxIndication");
  CANoeAPI_DetSetFunction(59, 0x05, "J1939Rm_SetState");
  CANoeAPI_DetSetFunction(59, 0x02, "J1939Rm_DeInit");
  CANoeAPI_DetSetFunction(59, 0x07, "J1939Rm_SendRequest");
  CANoeAPI_DetSetFunction(59, 0x40, "J1939Rm_TxConfirmation");
  CANoeAPI_DetSetError(59, 0x08, "J1939RM_E_INVALID_PGN");
  CANoeAPI_DetSetError(59, 0x0e, "J1939RM_E_INIT_FAILED");
  CANoeAPI_DetSetError(59, 0x81, "J1939RM_E_TIMEOUT_TXCONF");
  CANoeAPI_DetSetError(59, 0x07, "J1939RM_E_INVALID_USER");
  CANoeAPI_DetSetError(59, 0x83, "J1939RM_E_ACK_QUEUE_OVERRUN");
  CANoeAPI_DetSetError(59, 0x02, "J1939RM_E_REINIT");
  CANoeAPI_DetSetError(59, 0x04, "J1939RM_E_INVALID_PDU_SDU_ID");
  CANoeAPI_DetSetError(59, 0x09, "J1939RM_E_INVALID_PRIO");
  CANoeAPI_DetSetError(59, 0x0b, "J1939RM_E_INVALID_OPTION");
  CANoeAPI_DetSetError(59, 0x03, "J1939RM_E_PARAM_POINTER");
  CANoeAPI_DetSetError(59, 0x84, "J1939RM_E_REQ_QUEUE_OVERRUN");
  CANoeAPI_DetSetError(59, 0x06, "J1939RM_E_INVALID_STATE");
  CANoeAPI_DetSetError(59, 0x0d, "J1939RM_E_INVALID_NODE");
  CANoeAPI_DetSetError(59, 0x0c, "J1939RM_E_INVALID_ACK_CODE");
  CANoeAPI_DetSetError(59, 0x85, "J1939RM_E_INVALID_API");
  CANoeAPI_DetSetError(59, 0x82, "J1939RM_E_ISOBUS_ACKM_ADDR");
  CANoeAPI_DetSetError(59, 0x80, "J1939RM_E_INVALID_PDU_SIZE");
  CANoeAPI_DetSetError(59, 0x05, "J1939RM_E_INVALID_NETWORK_ID");
  CANoeAPI_DetSetError(59, 0x01, "J1939RM_E_UNINIT");
  CANoeAPI_DetSetError(59, 0x0a, "J1939RM_E_INVALID_ADDRESS");
}

/* Initialize DET codes for module J1939Tp*/
STATIC void VTTCntrl_DetInitialization_J1939Tp(void)
{
  CANoeAPI_DetSetModule(37, "J1939Tp");
  CANoeAPI_DetSetFunction(37, 0x42, "J1939Tp_RxIndication");
  CANoeAPI_DetSetFunction(37, 0x09, "J1939Tp_CancelTransmit");
  CANoeAPI_DetSetFunction(37, 0x02, "J1939Tp_Shutdown");
  CANoeAPI_DetSetFunction(37, 0x0A, "J1939Tp_CancelReceive");
  CANoeAPI_DetSetFunction(37, 0x01, "J1939Tp_Init");
  CANoeAPI_DetSetFunction(37, 0x40, "J1939Tp_TxConfirmation");
  CANoeAPI_DetSetFunction(37, 0x08, "J1939Tp_ChangeParameter");
  CANoeAPI_DetSetFunction(37, 0x05, "J1939Tp_Transmit");
  CANoeAPI_DetSetFunction(37, 0x04, "J1939Tp_MainFunction");
  CANoeAPI_DetSetFunction(37, 0x80, "J1939Tp_InitMemory");
  CANoeAPI_DetSetFunction(37, 0x03, "J1939Tp_GetVersionInfo");
  CANoeAPI_DetSetError(37, 0x95, "J1939TP_E_UNEXPECTED_DPO");
  CANoeAPI_DetSetError(37, 0x42, "J1939TP_E_INVALID_MNOP");
  CANoeAPI_DetSetError(37, 0x93, "J1939TP_E_INVALID_DPO");
  CANoeAPI_DetSetError(37, 0x01, "J1939TP_E_PARAM_ID");
  CANoeAPI_DetSetError(37, 0x87, "J1939TP_E_IGNORED_EOMACK");
  CANoeAPI_DetSetError(37, 0x41, "J1939TP_E_INVALID_TNOP");
  CANoeAPI_DetSetError(37, 0x85, "J1939TP_E_UNTIMELY_RTS");
  CANoeAPI_DetSetError(37, 0x8A, "J1939TP_E_INVALID_DLC");
  CANoeAPI_DetSetError(37, 0x37, "J1939TP_E_TIMEOUT_TH");
  CANoeAPI_DetSetError(37, 0x34, "J1939TP_E_TIMEOUT_T3");
  CANoeAPI_DetSetError(37, 0x44, "J1939TP_E_INVALID_NOP");
  CANoeAPI_DetSetError(37, 0x8C, "J1939TP_E_INVALID_ATNOP");
  CANoeAPI_DetSetError(37, 0x83, "J1939TP_E_INVALID_CHANGE_PARAM");
  CANoeAPI_DetSetError(37, 0x32, "J1939TP_E_TIMEOUT_T1");
  CANoeAPI_DetSetError(37, 0x82, "J1939TP_E_INVALID_DA");
  CANoeAPI_DetSetError(37, 0x81, "J1939TP_E_INVALID_SA");
  CANoeAPI_DetSetError(37, 0x47, "J1939TP_E_INVALID_SN");
  CANoeAPI_DetSetError(37, 0x36, "J1939TP_E_TIMEOUT_TR");
  CANoeAPI_DetSetError(37, 0x90, "J1939TP_E_INVALID_NBT");
  CANoeAPI_DetSetError(37, 0x88, "J1939TP_E_IGNORED_ABORT");
  CANoeAPI_DetSetError(37, 0x21, "J1939TP_E_REINIT");
  CANoeAPI_DetSetError(37, 0x02, "J1939TP_E_PARAM_ADDRESS");
  CANoeAPI_DetSetError(37, 0x43, "J1939TP_E_INVALID_PGN");
  CANoeAPI_DetSetError(37, 0x35, "J1939TP_E_TIMEOUT_T4");
  CANoeAPI_DetSetError(37, 0x40, "J1939TP_E_INVALID_TMS");
  CANoeAPI_DetSetError(37, 0x33, "J1939TP_E_TIMEOUT_T2");
  CANoeAPI_DetSetError(37, 0x46, "J1939TP_E_INVALID_CAR");
  CANoeAPI_DetSetError(37, 0x80, "J1939TP_E_INVALID_LENGTH");
  CANoeAPI_DetSetError(37, 0x86, "J1939TP_E_IGNORED_CTS");
  CANoeAPI_DetSetError(37, 0x45, "J1939TP_E_INVALID_NPN");
  CANoeAPI_DetSetError(37, 0x94, "J1939TP_E_INVALID_NPO");
  CANoeAPI_DetSetError(37, 0x8B, "J1939TP_E_INVALID_ATMS");
  CANoeAPI_DetSetError(37, 0x92, "J1939TP_E_UNEXPECTED_ECTS");
  CANoeAPI_DetSetError(37, 0x91, "J1939TP_E_INVALID_ANBT");
  CANoeAPI_DetSetError(37, 0x89, "J1939TP_E_NO_CONNECTION");
  CANoeAPI_DetSetError(37, 0x84, "J1939TP_E_INVALID_CHANGE_VALUE");
  CANoeAPI_DetSetError(37, 0x20, "J1939TP_E_UNINIT");
  CANoeAPI_DetSetError(37, 0x22, "J1939TP_E_INIT_FAILED");
}

/* Initialize DET codes for module Lin*/
STATIC void VTTCntrl_DetInitialization_Lin(void)
{
  CANoeAPI_DetSetModule(82, "Lin");
  CANoeAPI_DetSetFunction(82, 0x07, "Lin_Wakeup");
  CANoeAPI_DetSetFunction(82, 0x00, "Lin_Init");
  CANoeAPI_DetSetFunction(82, 0x0A, "Lin_WakeupValidation");
  CANoeAPI_DetSetFunction(82, 0x90, "Lin_Interrupt");
  CANoeAPI_DetSetFunction(82, 0x01, "Lin_GetVersionInfo");
  CANoeAPI_DetSetFunction(82, 0x06, "Lin_GoToSleep");
  CANoeAPI_DetSetFunction(82, 0x02, "Lin_InitChannel");
  CANoeAPI_DetSetFunction(82, 0x0B, "Lin_WakeupInternal");
  CANoeAPI_DetSetFunction(82, 0x09, "Lin_GoToSleepInternal");
  CANoeAPI_DetSetFunction(82, 0x08, "Lin_GetStatus");
  CANoeAPI_DetSetFunction(82, 0x03, "Lin_DeInitChannel");
  CANoeAPI_DetSetFunction(82, 0x04, "Lin_SendFrame");
  CANoeAPI_DetSetError(82, 0x10, "LIN_E_TIMEOUT");
  CANoeAPI_DetSetError(82, 0x04, "LIN_E_STATE_TRANSITION");
  CANoeAPI_DetSetError(82, 0x05, "LIN_E_PARAM_POINTER");
  CANoeAPI_DetSetError(82, 0x00, "LIN_E_UNINIT");
  CANoeAPI_DetSetError(82, 0x02, "LIN_E_INVALID_CHANNEL");
  CANoeAPI_DetSetError(82, 0x03, "LIN_E_INVALID_POINTER");
}

/* Initialize DET codes for module LinIf*/
STATIC void VTTCntrl_DetInitialization_LinIf(void)
{
  CANoeAPI_DetSetModule(62, "LinIf");
  CANoeAPI_DetSetFunction(62, 0x44, "LinTp_ChangeParameter");
  CANoeAPI_DetSetFunction(62, 0x80, "LinIf_MainFunction");
  CANoeAPI_DetSetFunction(62, 0x47, "LinTp_CancelReceive");
  CANoeAPI_DetSetFunction(62, 0x40, "LinTp_Init");
  CANoeAPI_DetSetFunction(62, 0x41, "LinTp_Transmit");
  CANoeAPI_DetSetFunction(62, 0x03, "LinIf_GetVersionInfo");
  CANoeAPI_DetSetFunction(62, 0x09, "LinIf_GetTrcvMode");
  CANoeAPI_DetSetFunction(62, 0x0A, "LinIf_GetTrcvWakeupReason");
  CANoeAPI_DetSetFunction(62, 0x07, "LinIf_Wakeup");
  CANoeAPI_DetSetFunction(62, 0x46, "LinTp_CancelTransmit");
  CANoeAPI_DetSetFunction(62, 0x60, "LinIf_CheckWakeup");
  CANoeAPI_DetSetFunction(62, 0x42, "LinTp_GetVersionInfo");
  CANoeAPI_DetSetFunction(62, 0x01, "LinIf_Init");
  CANoeAPI_DetSetFunction(62, 0x06, "LinIf_GotoSleep");
  CANoeAPI_DetSetFunction(62, 0x04, "LinIf_Transmit");
  CANoeAPI_DetSetFunction(62, 0x61, "LinIf_WakeupConfirmation");
  CANoeAPI_DetSetFunction(62, 0x43, "LinTp_Shutdown");
  CANoeAPI_DetSetFunction(62, 0x05, "LinIf_ScheduleRequest");
  CANoeAPI_DetSetFunction(62, 0x08, "LinIf_SetTrcvMode");
  CANoeAPI_DetSetFunction(62, 0x0C, "LinIf_CancelTransmit");
  CANoeAPI_DetSetFunction(62, 0x0B, "LinIf_SetTrcvWakeupMode");
  CANoeAPI_DetSetError(62, 0x53, "LINIF_E_TRCV_INV_MODE");
  CANoeAPI_DetSetError(62, 0x72, "LINIF_E_TRIGGERTRANSMIT_NO_DATA");
  CANoeAPI_DetSetError(62, 0x70, "LINIF_E_SCHEDULE_INCONSISTENT_ERROR");
  CANoeAPI_DetSetError(62, 0x51, "LINIF_E_SCHEDULE_REQUEST_ERROR");
  CANoeAPI_DetSetError(62, 0x60, "LINIF_E_RESPONSE");
  CANoeAPI_DetSetError(62, 0x00, "LINIF_E_UNINIT");
  CANoeAPI_DetSetError(62, 0x40, "LINIF_E_PARAMETER_POINTER");
  CANoeAPI_DetSetError(62, 0x54, "LINIF_E_TRCV_NOT_NORMAL");
  CANoeAPI_DetSetError(62, 0x10, "LINIF_E_ALREADY_INITIALIZED");
  CANoeAPI_DetSetError(62, 0x20, "LINIF_E_NONEXISTENT_CHANNEL");
  CANoeAPI_DetSetError(62, 0x30, "LINIF_E_PARAMETER");
  CANoeAPI_DetSetError(62, 0x52, "LINIF_E_TRCV_INV_CHANNEL");
  CANoeAPI_DetSetError(62, 0x61, "LINIF_E_NC_NO_RESPONSE");
  CANoeAPI_DetSetError(62, 0x71, "LINIF_E_CONFIG");
  CANoeAPI_DetSetError(62, 0x55, "LINIF_E_PARAM_WAKEUPSOURCE");
}

/* Initialize DET codes for module LinNm*/
STATIC void VTTCntrl_DetInitialization_LinNm(void)
{
  CANoeAPI_DetSetModule(63, "LinNm");
  CANoeAPI_DetSetFunction(63, 0x02, "LinNm_NetworkRequest");
  CANoeAPI_DetSetFunction(63, 0x0E, "LinNm_GetState");
  CANoeAPI_DetSetFunction(63, 0x01, "LinNm_PassiveStartUp");
  CANoeAPI_DetSetFunction(63, 0x03, "LinNm_NetworkRelease");
  CANoeAPI_DetSetFunction(63, 0xF1, "LinNm_GetVersionInfo");
  CANoeAPI_DetSetError(63, 0x02, "LINNM_E_INVALID_CHANNEL");
  CANoeAPI_DetSetError(63, 0x01, "LINNM_E_NO_INIT");
  CANoeAPI_DetSetError(63, 0x12, "LINNM_E_PARAM_POINTER");
}

/* Initialize DET codes for module LinSM*/
STATIC void VTTCntrl_DetInitialization_LinSM(void)
{
  CANoeAPI_DetSetModule(141, "LinSM");
  CANoeAPI_DetSetFunction(141, 0x02, "LinSM_GetVersionInfo");
  CANoeAPI_DetSetFunction(141, 0x21, "LinSM_WakeupConfirmation");
  CANoeAPI_DetSetFunction(141, 0x22, "LinSM_GotoSleepConfirmation");
  CANoeAPI_DetSetFunction(141, 0x01, "LinSM_Init");
  CANoeAPI_DetSetFunction(141, 0x10, "LinSM_ScheduleRequest");
  CANoeAPI_DetSetFunction(141, 0x20, "LinSM_ScheduleRequestConfirmation");
  CANoeAPI_DetSetFunction(141, 0x30, "LinSM_MainFunction");
  CANoeAPI_DetSetFunction(141, 0x23, "LinSM_ScheduleEndNotification");
  CANoeAPI_DetSetFunction(141, 0x12, "LinSM_RequestComMode");
  CANoeAPI_DetSetFunction(141, 0x40, "LinSM_SwitchTransceiver");
  CANoeAPI_DetSetFunction(141, 0x11, "LinSM_GetCurrentComMode");
  CANoeAPI_DetSetError(141, 0x66, "LINSM_E_FULLCOM_NOT_REQUESTED");
  CANoeAPI_DetSetError(141, 0x61, "LINSM_E_GETTRCVMODE_FAILED");
  CANoeAPI_DetSetError(141, 0x60, "LINSM_E_INIT_FAILED");
  CANoeAPI_DetSetError(141, 0x65, "LINSM_E_MODE_ALREADY_ACTIVE");
  CANoeAPI_DetSetError(141, 0x50, "LINSM_E_CONFIRMATION_TIMEOUT");
  CANoeAPI_DetSetError(141, 0x67, "LINSM_E_NOCOM_NOT_REQUESTED");
  CANoeAPI_DetSetError(141, 0x68, "LINSM_E_TIMEOUT_IN_NO_COM");
  CANoeAPI_DetSetError(141, 0x63, "LINSM_E_NOT_IN_FULL_COM");
  CANoeAPI_DetSetError(141, 0x00, "LINSM_E_UNINIT");
  CANoeAPI_DetSetError(141, 0x40, "LINSM_E_PARAMETER_POINTER");
  CANoeAPI_DetSetError(141, 0x30, "LINSM_E_PARAMETER");
  CANoeAPI_DetSetError(141, 0x0A, "LINSM_E_PARAM_CONFIG");
  CANoeAPI_DetSetError(141, 0x20, "LINSM_E_NONEXISTENT_NETWORK");
  CANoeAPI_DetSetError(141, 0x64, "LINSM_E_REQUEST_IN_PROCESS");
  CANoeAPI_DetSetError(141, 0x69, "LINSM_E_WAKEUPCONF_NO_SUCCESS");
  CANoeAPI_DetSetError(141, 0x62, "LINSM_E_SETTRCVMODE_FAILED");
}

/* Initialize DET codes for module Mcu*/
STATIC void VTTCntrl_DetInitialization_Mcu(void)
{
  CANoeAPI_DetSetModule(101, "Mcu");
  CANoeAPI_DetSetFunction(101, 0x08, "Mcu_SetMode");
  CANoeAPI_DetSetFunction(101, 0x04, "Mcu_GetPllStatus");
  CANoeAPI_DetSetFunction(101, 0x04, "Mcu_GetRamState");
  CANoeAPI_DetSetFunction(101, 0x03, "Mcu_DistributeClock");
  CANoeAPI_DetSetFunction(101, 0x06, "Mcu_GetResetRawValue");
  CANoeAPI_DetSetFunction(101, 0x01, "Mcu_InitRamSection");
  CANoeAPI_DetSetFunction(101, 0x00, "Mcu_Init");
  CANoeAPI_DetSetFunction(101, 0x05, "Mcu_GetResetReason");
  CANoeAPI_DetSetFunction(101, 0x07, "Mcu_PerformReset");
  CANoeAPI_DetSetFunction(101, 0x09, "Mcu_GetVersionInfo");
  CANoeAPI_DetSetFunction(101, 0x02, "Mcu_InitClock");
  CANoeAPI_DetSetError(101, 0x0A, "MCU_E_PARAM_CONFIG");
  CANoeAPI_DetSetError(101, 0x0D, "MCU_E_PARAM_RAMSECTION");
  CANoeAPI_DetSetError(101, 0x0C, "MCU_E_PARAM_MODE");
  CANoeAPI_DetSetError(101, 0x15, "MCU_E_PARAM_VINFO");
  CANoeAPI_DetSetError(101, 0x0B, "MCU_E_PARAM_CLOCK");
  CANoeAPI_DetSetError(101, 0x0E, "MCU_E_PLL_NOT_LOCKED");
  CANoeAPI_DetSetError(101, 0x0F, "MCU_E_UNINIT");
}

/* Initialize DET codes for module MemIf*/
STATIC void VTTCntrl_DetInitialization_MemIf(void)
{
  CANoeAPI_DetSetModule(22, "MemIf");
  CANoeAPI_DetSetFunction(22, 0x9, "MemIf_EraseImmediateBlock");
  CANoeAPI_DetSetFunction(22, 0x5, "MemIf_GetStatus");
  CANoeAPI_DetSetFunction(22, 0x7, "MemIf_InvalidateBlock");
  CANoeAPI_DetSetFunction(22, 0x8, "MemIf_GetVersionInfo");
  CANoeAPI_DetSetFunction(22, 0x2, "MemIf_Read");
  CANoeAPI_DetSetFunction(22, 0x3, "MemIf_Write");
  CANoeAPI_DetSetFunction(22, 0x4, "MemIf_Cancel");
  CANoeAPI_DetSetFunction(22, 0x6, "MemIf_GetJobResult");
  CANoeAPI_DetSetFunction(22, 0x1, "MemIf_SetMode");
  CANoeAPI_DetSetError(22, 0x02, "MEMIF_E_PARAM_POINTER");
  CANoeAPI_DetSetError(22, 0x01, "MEMIF_E_PARAM_DEVICE");
}

/* Initialize DET codes for module Nm*/
STATIC void VTTCntrl_DetInitialization_Nm(void)
{
  CANoeAPI_DetSetModule(29, "Nm");
  CANoeAPI_DetSetFunction(29, 0x09, "Nm_RepeatMessageRequest");
  CANoeAPI_DetSetFunction(29, 0x08, "Nm_GetPduData");
  CANoeAPI_DetSetFunction(29, 0x00, "Nm_Init");
  CANoeAPI_DetSetFunction(29, 0x1F, "Nm_CoordReadyToSleepCancellation");
  CANoeAPI_DetSetFunction(29, 0x1C, "Nm_BusNm_PduRxIndication");
  CANoeAPI_DetSetFunction(29, 0x1E, "Nm_CoordReadyToSleepIndication");
  CANoeAPI_DetSetFunction(29, 0x11, "Nm_NetworkStartIndication");
  CANoeAPI_DetSetFunction(29, 0x16, "Nm_StateChangeNotification");
  CANoeAPI_DetSetFunction(29, 0x0A, "Nm_GetNodeIdentifier");
  CANoeAPI_DetSetFunction(29, 0x18, "Nm_RemoteSleepCancellation");
  CANoeAPI_DetSetFunction(29, 0x05, "Nm_EnableCommunication");
  CANoeAPI_DetSetFunction(29, 0x11, "Nm_InitMemory");
  CANoeAPI_DetSetFunction(29, 0x01, "Nm_PassiveStartUp");
  CANoeAPI_DetSetFunction(29, 0x12, "Nm_NetworkMode");
  CANoeAPI_DetSetFunction(29, 0x14, "Nm_BusSleepMode");
  CANoeAPI_DetSetFunction(29, 0x04, "Nm_DisableCommunication");
  CANoeAPI_DetSetFunction(29, 0x02, "Nm_NetworkRequest");
  CANoeAPI_DetSetFunction(29, 0x1B, "Nm_TxTimeoutException");
  CANoeAPI_DetSetFunction(29, 0x13, "Nm_PrepareBusSleepMode");
  CANoeAPI_DetSetFunction(29, 0x06, "Nm_SetUserData");
  CANoeAPI_DetSetFunction(29, 0x10, "Nm_MainFunction");
  CANoeAPI_DetSetFunction(29, 0x19, "Nm_SynchronizationPoint");
  CANoeAPI_DetSetFunction(29, 0x15, "Nm_PduRxIndication");
  CANoeAPI_DetSetFunction(29, 0x0D, "Nm_CheckRemoteSleepIndication");
  CANoeAPI_DetSetFunction(29, 0x0E, "Nm_GetState");
  CANoeAPI_DetSetFunction(29, 0x0B, "Nm_GetLocalNodeIdentifier");
  CANoeAPI_DetSetFunction(29, 0x07, "Nm_GetUserData");
  CANoeAPI_DetSetFunction(29, 0x0F, "Nm_GetVersionInfo");
  CANoeAPI_DetSetFunction(29, 0x03, "Nm_NetworkRelease");
  CANoeAPI_DetSetFunction(29, 0x1A, "Nm_RepeatMessageIndication");
  CANoeAPI_DetSetFunction(29, 0x1D, "Nm_CarWakeUpIndication");
  CANoeAPI_DetSetFunction(29, 0x17, "Nm_RemoteSleepIndication");
  CANoeAPI_DetSetError(29, 0x24, "NM_E_NOT_AVAILABLE_IN_PASSIVE_MODE");
  CANoeAPI_DetSetError(29, 0x21, "NM_E_FUNCTION_PTR_IS_NULL");
  CANoeAPI_DetSetError(29, 0x20, "NM_E_SYNCHRONIZATION_TIMEOUT");
  CANoeAPI_DetSetError(29, 0x00, "NM_E_UNINIT");
  CANoeAPI_DetSetError(29, 0x01, "NM_E_HANDLE_UNDEF");
  CANoeAPI_DetSetError(29, 0x23, "NM_E_SAME_STATES");
  CANoeAPI_DetSetError(29, 0x02, "NM_E_PARAM_POINTER");
  CANoeAPI_DetSetError(29, 0x22, "NM_E_INVALID_STATE");
}

/* Initialize DET codes for module NvM*/
STATIC void VTTCntrl_DetInitialization_NvM(void)
{
  CANoeAPI_DetSetModule(20, "NvM");
  CANoeAPI_DetSetFunction(20, 0x05, "NvM_SetRamBlockStatus");
  CANoeAPI_DetSetFunction(20, 0x07, "NvM_WriteBlock");
  CANoeAPI_DetSetFunction(20, 0x0D, "NvM_WriteAll");
  CANoeAPI_DetSetFunction(20, 0x0C, "NvM_ReadAll");
  CANoeAPI_DetSetFunction(20, 0x10, "NvM_CancelJobs");
  CANoeAPI_DetSetFunction(20, 0x04, "NvM_GetErrorStatus");
  CANoeAPI_DetSetFunction(20, 0x08, "NvM_RestoreBlockDefaults");
  CANoeAPI_DetSetFunction(20, 0x03, "NvM_SetBlockProtection");
  CANoeAPI_DetSetFunction(20, 0x00, "NvM_Init");
  CANoeAPI_DetSetFunction(20, 0x02, "NvM_GetDataIndex");
  CANoeAPI_DetSetFunction(20, 0x09, "NvM_EraseNvBlock");
  CANoeAPI_DetSetFunction(20, 0x06, "NvM_ReadBlock");
  CANoeAPI_DetSetFunction(20, 0x0A, "NvM_CancelWriteAll");
  CANoeAPI_DetSetFunction(20, 0x01, "NvM_SetDataIndex");
  CANoeAPI_DetSetFunction(20, 0x0F, "NvM_GetVersionInfo");
  CANoeAPI_DetSetFunction(20, 0x0B, "NvM_InvalidateNvBlock");
  CANoeAPI_DetSetFunction(20, 0x13, "NvM_SetBlockLockStatus");
  CANoeAPI_DetSetFunction(20, 0x0E, "NvM_MainFunction");
  CANoeAPI_DetSetError(20, 0x0E, "NVM_E_PARAM_DATA");
  CANoeAPI_DetSetError(20, 0x0D, "NVM_E_PARAM_ADDRESS");
  CANoeAPI_DetSetError(20, 0x18, "NVM_E_BLOCK_CONFIG");
  CANoeAPI_DetSetError(20, 0x14, "NVM_E_NOT_INITIALIZED");
  CANoeAPI_DetSetError(20, 0x0B, "NVM_E_PARAM_BLOCK_TYPE");
  CANoeAPI_DetSetError(20, 0x0A, "NVM_E_PARAM_BLOCK_ID");
  CANoeAPI_DetSetError(20, 0x15, "NVM_E_BLOCK_PENDING");
  CANoeAPI_DetSetError(20, 0x0C, "NVM_E_PARAM_BLOCK_DATA_IDX");
}

/* Initialize DET codes for module Port*/
STATIC void VTTCntrl_DetInitialization_Port(void)
{
  CANoeAPI_DetSetModule(124, "Port");
  CANoeAPI_DetSetFunction(124, 0x01, "Port_SetPinDirection");
  CANoeAPI_DetSetFunction(124, 0x03, "Port_GetVersionInfo");
  CANoeAPI_DetSetFunction(124, 0x02, "Port_RefreshPortDirection");
  CANoeAPI_DetSetFunction(124, 0x00, "Port_Init");
  CANoeAPI_DetSetFunction(124, 0x04, "Port_SetPinMode");
  CANoeAPI_DetSetError(124, 0x0B, "PORT_E_DIRECTION_UNCHANGEABLE");
  CANoeAPI_DetSetError(124, 0x0C, "PORT_E_PARAM_CONFIG");
  CANoeAPI_DetSetError(124, 0x20, "PORT_E_PARAM_VINFO");
  CANoeAPI_DetSetError(124, 0x0F, "PORT_E_UNINIT");
  CANoeAPI_DetSetError(124, 0x0A, "PORT_E_PARAM_PIN");
}

/* Initialize DET codes for module Pwm*/
STATIC void VTTCntrl_DetInitialization_Pwm(void)
{
  CANoeAPI_DetSetModule(121, "Pwm");
  CANoeAPI_DetSetFunction(121, 0x01, "Pwm_DeInit");
  CANoeAPI_DetSetFunction(121, 0x06, "Pwm_DisableNotification");
  CANoeAPI_DetSetFunction(121, 0x02, "Pwm_SetDutyCycle");
  CANoeAPI_DetSetFunction(121, 0x08, "Pwm_GetVersionInfo");
  CANoeAPI_DetSetFunction(121, 0x04, "Pwm_SetOutputToIdle");
  CANoeAPI_DetSetFunction(121, 0x07, "Pwm_Enable_Notification");
  CANoeAPI_DetSetFunction(121, 0x05, "Pwm_GetOutputState");
  CANoeAPI_DetSetFunction(121, 0x00, "Pwm_Init");
  CANoeAPI_DetSetFunction(121, 0x03, "Pwm_SetPeriodAndDuty");
  CANoeAPI_DetSetError(121, 0x12, "PWM_E_PARAM_CHANNEL");
  CANoeAPI_DetSetError(121, 0x14, "PWM_E_ALREADY_INITIALIZED");
  CANoeAPI_DetSetError(121, 0x17, "PWM_E_PERIOD_ZERO");
  CANoeAPI_DetSetError(121, 0x13, "PWM_E_PERIOD_UNCHANGEABLE");
  CANoeAPI_DetSetError(121, 0x11, "PWM_E_UNINIT");
  CANoeAPI_DetSetError(121, 0x15, "PWM_E_PARAM_POINTER");
  CANoeAPI_DetSetError(121, 0x16, "PWM_E_PARAM_VINFO");
  CANoeAPI_DetSetError(121, 0x10, "PWM_E_PARAM_CONFIG");
}

/* Initialize DET codes for module RamTst*/
STATIC void VTTCntrl_DetInitialization_RamTst(void)
{
  CANoeAPI_DetSetModule(93, "RamTst");
  CANoeAPI_DetSetFunction(93, 0x05, "RamTst_Resume");
  CANoeAPI_DetSetFunction(93, 0x10, "RamTst_RunPartialTest");
  CANoeAPI_DetSetFunction(93, 0x08, "RamTst_GetTestResultPerBlock");
  CANoeAPI_DetSetFunction(93, 0x0D, "RamTst_SelectAlgParams");
  CANoeAPI_DetSetFunction(93, 0x04, "RamTst_Suspend");
  CANoeAPI_DetSetFunction(93, 0x0C, "RamTst_GetNumberOfTestedCells");
  CANoeAPI_DetSetFunction(93, 0x00, "RamTst_Init");
  CANoeAPI_DetSetFunction(93, 0x07, "RamTst_GetTestResult");
  CANoeAPI_DetSetFunction(93, 0x01, "RamTst_DeInit");
  CANoeAPI_DetSetFunction(93, 0x02, "RamTst_Stop");
  CANoeAPI_DetSetFunction(93, 0x06, "RamTst_GetExecutionStatus");
  CANoeAPI_DetSetFunction(93, 0x09, "RamTst_GetVersionInfo");
  CANoeAPI_DetSetFunction(93, 0x0F, "RamTst_RunFullTest");
  CANoeAPI_DetSetFunction(93, 0x03, "RamTst_Allow");
  CANoeAPI_DetSetFunction(93, 0x0B, "RamTst_GetTestAlgorithm");
  CANoeAPI_DetSetFunction(93, 0x0E, "RamTst_ChangeNumberOfTestedCells");
  CANoeAPI_DetSetFunction(93, 0x0A, "RamTst_GetAlgParams");
  CANoeAPI_DetSetError(93, 0x11, "RAMTST_E_PARAM_VINFO");
  CANoeAPI_DetSetError(93, 0x10, "RAMTST_E_ALREADY_INITIALIZED");
  CANoeAPI_DetSetError(93, 0x02, "RAMTST_E_OUT_OF_RANGE");
  CANoeAPI_DetSetError(93, 0x03, "RAMTST_E_UNINIT");
  CANoeAPI_DetSetError(93, 0x01, "RAMTST_E_STATUS_FAILURE");
}

/* Initialize DET codes for module Rte*/
STATIC void VTTCntrl_DetInitialization_Rte(void)
{
  CANoeAPI_DetSetModule(2, "Rte");
  CANoeAPI_DetSetFunction(2, 0x29, "Rte_IrvWrite");
  CANoeAPI_DetSetFunction(2, 0x97, "Rte_COMCbkTErr_sg");
  CANoeAPI_DetSetFunction(2, 0x14, "Rte_Write");
  CANoeAPI_DetSetFunction(2, 0x1B, "Rte_Receive");
  CANoeAPI_DetSetFunction(2, 0x90, "Rte_COMCbkTAck_sn");
  CANoeAPI_DetSetFunction(2, 0x91, "Rte_COMCbkTErr_sn");
  CANoeAPI_DetSetFunction(2, 0x17, "Rte_Feedback");
  CANoeAPI_DetSetFunction(2, 0x93, "Rte_COMCbkRxTOut_sn");
  CANoeAPI_DetSetFunction(2, 0x15, "Rte_Switch");
  CANoeAPI_DetSetFunction(2, 0x30, "Rte_IsUpdated");
  CANoeAPI_DetSetFunction(2, 0x70, "Rte_Start");
  CANoeAPI_DetSetFunction(2, 0x99, "Rte_COMCbkRxTOut_sg");
  CANoeAPI_DetSetFunction(2, 0x9F, "Rte_COMCbk_sn");
  CANoeAPI_DetSetFunction(2, 0x96, "Rte_COMCbkTAck_sg");
  CANoeAPI_DetSetFunction(2, 0x20, "Rte_Prm");
  CANoeAPI_DetSetFunction(2, 0x92, "Rte_COMCbkInv_sn");
  CANoeAPI_DetSetFunction(2, 0x1A, "Rte_DRead");
  CANoeAPI_DetSetFunction(2, 0x03, "SchM_Enter");
  CANoeAPI_DetSetFunction(2, 0x2C, "Rte_Mode");
  CANoeAPI_DetSetFunction(2, 0x98, "Rte_COMCbkInv_sg");
  CANoeAPI_DetSetFunction(2, 0x18, "Rte_SwitchAck");
  CANoeAPI_DetSetFunction(2, 0x04, "SchM_Exit");
  CANoeAPI_DetSetFunction(2, 0x01, "SchM_Deinit");
  CANoeAPI_DetSetFunction(2, 0x1D, "Rte_Result");
  CANoeAPI_DetSetFunction(2, 0xF2, "Rte_DecDisableFlags");
  CANoeAPI_DetSetFunction(2, 0x71, "Rte_Stop");
  CANoeAPI_DetSetFunction(2, 0x13, "Rte_Send");
  CANoeAPI_DetSetFunction(2, 0xF0, "Rte_Task");
  CANoeAPI_DetSetFunction(2, 0x95, "Rte_COMCbk_sg");
  CANoeAPI_DetSetFunction(2, 0x19, "Rte_Read");
  CANoeAPI_DetSetFunction(2, 0x1C, "Rte_Call");
  CANoeAPI_DetSetFunction(2, 0x2B, "Rte_Exit");
  CANoeAPI_DetSetFunction(2, 0x16, "Rte_Invalidate");
  CANoeAPI_DetSetFunction(2, 0x2A, "Rte_Enter");
  CANoeAPI_DetSetFunction(2, 0xF1, "Rte_IncDisableFlags");
  CANoeAPI_DetSetFunction(2, 0x1F, "Rte_CData");
  CANoeAPI_DetSetFunction(2, 0x28, "Rte_IrvRead");
  CANoeAPI_DetSetFunction(2, 0x9A, "Rte_COMCbkTxTOut_sg");
  CANoeAPI_DetSetFunction(2, 0x94, "Rte_COMCbkTxTOut_sn");
  CANoeAPI_DetSetFunction(2, 0x00, "SchM_Init");
  CANoeAPI_DetSetError(2, 0x05, "RTE_E_DET_ILLEGAL_NESTED_EXCLUSIVE_AREA");
  CANoeAPI_DetSetError(2, 0x22, "RTE_E_DET_TRANSITIONSTATE");
  CANoeAPI_DetSetError(2, 0x20, "RTE_E_DET_MODEARGUMENT");
  CANoeAPI_DetSetError(2, 0x24, "RTE_E_DET_MULTICORE_STARTUP");
  CANoeAPI_DetSetError(2, 0x21, "RTE_E_DET_TRIGGERDISABLECOUNTER");
  CANoeAPI_DetSetError(2, 0x01, "RTE_E_DET_ILLEGAL_SIGNAL_ID");
  CANoeAPI_DetSetError(2, 0x07, "RTE_E_DET_UNINIT");
}

/* Initialize DET codes for module Sd*/
STATIC void VTTCntrl_DetInitialization_Sd(void)
{
  CANoeAPI_DetSetModule(171, "Sd");
  CANoeAPI_DetSetFunction(171, 0x05, "SD_API_ID_LOCAL_IP_ADDR_ASSIGNMENT_CHG");
  CANoeAPI_DetSetFunction(171, 0x01, "SD_API_ID_INIT");
  CANoeAPI_DetSetFunction(171, 0xA0, "SD_API_ID_INTERNAL_FUNCTION");
  CANoeAPI_DetSetFunction(171, 0x42, "SD_API_ID_RX_INDICATION");
  CANoeAPI_DetSetFunction(171, 0x06, "SD_API_ID_MAIN_FUNCTION");
  CANoeAPI_DetSetFunction(171, 0x02, "SD_API_ID_GET_VERSION_INFO");
  CANoeAPI_DetSetFunction(171, 0x08, "SD_API_ID_CLIENT_SERVICE_SET_STATE");
  CANoeAPI_DetSetFunction(171, 0x09, "SD_API_ID_CONSUMED_EVENTGROUP_SET_STATE");
  CANoeAPI_DetSetFunction(171, 0x07, "SD_API_ID_SERVER_SERVICE_SET_STATE");
  CANoeAPI_DetSetError(171, 0x01, "SD_E_NOT_INITIALIZED");
  CANoeAPI_DetSetError(171, 0x05, "SD_E_INIT_FAILED");
  CANoeAPI_DetSetError(171, 0x03, "SD_E_INV_MODE");
  CANoeAPI_DetSetError(171, 0xA1, "SD_E_LIST_FULL");
  CANoeAPI_DetSetError(171, 0x02, "SD_E_PARAM_POINTER");
  CANoeAPI_DetSetError(171, 0x04, "SD_E_INV_ID");
}

/* Initialize DET codes for module SecOC*/
STATIC void VTTCntrl_DetInitialization_SecOC(void)
{
  CANoeAPI_DetSetModule(150, "SecOC");
  CANoeAPI_DetSetFunction(150, 0x03, "SECOC_SID_TRANSMIT");
  CANoeAPI_DetSetFunction(150, 0x04, "SECOC_SID_CANCEL_TRANSMIT");
  CANoeAPI_DetSetFunction(150, 0x09, "SECOC_SID_FRESHNESS_VALUE_WRITE");
  CANoeAPI_DetSetFunction(150, 0x05, "SECOC_SID_DE_INIT");
  CANoeAPI_DetSetFunction(150, 0x80, "SECOC_SID_INTERNAL");
  CANoeAPI_DetSetFunction(150, 0x06, "SECOC_SID_MAIN_FUNCTION");
  CANoeAPI_DetSetFunction(150, 0x0F, "SECOC_SID_TRIGGER_TRANSMIT");
  CANoeAPI_DetSetFunction(150, 0x08, "SECOC_SID_FRESHNESS_VALUE_READ");
  CANoeAPI_DetSetFunction(150, 0x0C, "SECOC_SID_RX_INDICATION");
  CANoeAPI_DetSetFunction(150, 0x0E, "SECOC_SID_TX_CONFIRMATION");
  CANoeAPI_DetSetFunction(150, 0x01, "SECOC_SID_INIT");
  CANoeAPI_DetSetFunction(150, 0x07, "SECOC_SID_ASSOCIATE_KEY");
  CANoeAPI_DetSetFunction(150, 0x02, "SECOC_SID_GET_VERSION_INFO");
  CANoeAPI_DetSetError(150, 0x03, "SECOC_E_INVALID_PDU_SDU_ID");
  CANoeAPI_DetSetError(150, 0x01, "SECOC_E_NO_PARAM_POINTER");
  CANoeAPI_DetSetError(150, 0x04, "SECOC_E_CRYPTO_FAILURE");
  CANoeAPI_DetSetError(150, 0x02, "SECOC_E_INVALID_REQUEST");
  CANoeAPI_DetSetError(150, 0x06, "SECOC_E_FRESHNESS_VALUE_AT_LIMIT");
  CANoeAPI_DetSetError(150, 0x05, "SECOC_E_RESTORE_FAILURE");
  CANoeAPI_DetSetError(150, 0x07, "SECOC_E_PARAM");
}

/* Initialize DET codes for module SoAd*/
STATIC void VTTCntrl_DetInitialization_SoAd(void)
{
  CANoeAPI_DetSetModule(56, "SoAd");
  CANoeAPI_DetSetFunction(56, 0x2D, "SOAD_API_ID_VCALL_TP_COPY_TX_DATA");
  CANoeAPI_DetSetFunction(56, 0x08, "SOAD_API_ID_SHUTDOWN");
  CANoeAPI_DetSetFunction(56, 0x0C, "SOAD_API_ID_CBK_TCPIP_EVENT");
  CANoeAPI_DetSetFunction(56, 0x09, "SOAD_API_ID_GET_VERSION_INFO");
  CANoeAPI_DetSetFunction(56, 0x01, "SOAD_API_ID_INIT");
  CANoeAPI_DetSetFunction(56, 0x02, "SOAD_API_ID_RX_INDICATION");
  CANoeAPI_DetSetFunction(56, 0x19, "SOAD_API_ID_DISABLE_ROUTING");
  CANoeAPI_DetSetFunction(56, 0x23, "SOAD_API_ID_CBK_TRCV_LINK_STATE_CHG");
  CANoeAPI_DetSetFunction(56, 0x29, "SOAD_API_ID_VTCP_PDU_HDR_RX_INDICATION");
  CANoeAPI_DetSetFunction(56, 0x20, "SOAD_API_ID_MAIN_FUNCTION");
  CANoeAPI_DetSetFunction(56, 0x2A, "SOAD_API_ID_VTCP_RX_INDICATION");
  CANoeAPI_DetSetFunction(56, 0x0F, "SOAD_API_ID_GET_RCV_REMOTE_ADDR");
  CANoeAPI_DetSetFunction(56, 0x04, "SOAD_API_ID_COPY_TX_DATA");
  CANoeAPI_DetSetFunction(56, 0x27, "SOAD_API_ID_VIF_TRANSMIT");
  CANoeAPI_DetSetFunction(56, 0x07, "SOAD_API_ID_IF_ROUT_GROUP_TRANSMIT");
  CANoeAPI_DetSetFunction(56, 0x28, "SOAD_API_ID_VOPEN_SOCK_CON");
  CANoeAPI_DetSetFunction(56, 0x11, "SOAD_API_ID_CLOSE_SO_CON");
  CANoeAPI_DetSetFunction(56, 0x31, "SOAD_API_ID_VCALL_TP_COPY_RX_DATA");
  CANoeAPI_DetSetFunction(56, 0x15, "SOAD_API_ID_GET_PHYS_ADDR");
  CANoeAPI_DetSetFunction(56, 0x17, "SOAD_API_ID_ENABLE_ROUTING");
  CANoeAPI_DetSetFunction(56, 0x2C, "SOAD_API_ID_VCALL_IF_TX_CONFIRMATION");
  CANoeAPI_DetSetFunction(56, 0x18, "SOAD_API_ID_ENABLE_SPECIFIC_ROUTING");
  CANoeAPI_DetSetFunction(56, 0x1A, "SOAD_API_ID_DISABLE_SPECIFIC_ROUTING");
  CANoeAPI_DetSetFunction(56, 0x1B, "SOAD_API_ID_SET_REMOTE_ADDR");
  CANoeAPI_DetSetFunction(56, 0x0B, "SOAD_API_ID_CBK_TCP_CONNECTED");
  CANoeAPI_DetSetFunction(56, 0x25, "SOAD_API_ID_TP_CANCEL_RECEIVE");
  CANoeAPI_DetSetFunction(56, 0x24, "SOAD_API_ID_TP_CANCEL_TRANSMIT");
  CANoeAPI_DetSetFunction(56, 0x06, "SOAD_API_ID_IF_TRANSMIT");
  CANoeAPI_DetSetFunction(56, 0x2E, "SOAD_API_ID_VCALL_TP_TX_CONFIRMATION");
  CANoeAPI_DetSetFunction(56, 0x13, "SOAD_API_ID_RLS_IP_ADDR_ASSIGN");
  CANoeAPI_DetSetFunction(56, 0x0D, "SOAD_API_ID_TP_TRANSMIT");
  CANoeAPI_DetSetFunction(56, 0x26, "SOAD_API_ID_VPROVIDE_TX_BUFFER");
  CANoeAPI_DetSetFunction(56, 0x1C, "SOAD_API_ID_SET_UNI_REMOTE_ADDR");
  CANoeAPI_DetSetFunction(56, 0x03, "SOAD_API_ID_TX_CONFIRMATION");
  CANoeAPI_DetSetFunction(56, 0x12, "SOAD_API_ID_REQ_IP_ADDR_ASSIGN");
  CANoeAPI_DetSetFunction(56, 0x30, "SOAD_API_ID_VCALL_TP_START_OF_RECEPTION");
  CANoeAPI_DetSetFunction(56, 0x21, "SOAD_API_ID_CBK_BUSSM_MODE_INDICATION");
  CANoeAPI_DetSetFunction(56, 0x22, "SOAD_API_ID_LOCAL_IP_ADDR_ASSIGNMENT_CHG");
  CANoeAPI_DetSetFunction(56, 0x2B, "SOAD_API_ID_VCALL_IF_TRIGGER_TRANSMIT");
  CANoeAPI_DetSetFunction(56, 0x1F, "SOAD_API_ID_WRITE_DHCP_HOST_NAME_OPT");
  CANoeAPI_DetSetFunction(56, 0x33, "SOAD_API_ID_SET_LOCAL_ADDR");
  CANoeAPI_DetSetFunction(56, 0x16, "SOAD_API_ID_GET_REMOTE_ADDR");
  CANoeAPI_DetSetFunction(56, 0x1E, "SOAD_API_ID_READ_DHCP_HOST_NAME_OPT");
  CANoeAPI_DetSetFunction(56, 0x32, "SOAD_API_ID_VCALL_TP_RX_INDICATION");
  CANoeAPI_DetSetFunction(56, 0x14, "SOAD_API_ID_GET_LOCAL_ADDR");
  CANoeAPI_DetSetFunction(56, 0x2F, "SOAD_API_ID_VCALL_IF_RX_INDICATION");
  CANoeAPI_DetSetFunction(56, 0x0E, "SOAD_API_ID_GET_SO_CON_ID");
  CANoeAPI_DetSetFunction(56, 0x10, "SOAD_API_ID_OPEN_SO_CON");
  CANoeAPI_DetSetFunction(56, 0x05, "SOAD_API_ID_SOCKET_RESET");
  CANoeAPI_DetSetFunction(56, 0x0A, "SOAD_API_ID_CBK_TCP_ACCEPTED");
  CANoeAPI_DetSetFunction(56, 0x1D, "SOAD_API_ID_CHANGE_PARAMETER");
  CANoeAPI_DetSetError(56, 0x07, "SOAD_E_INV_SOCKETID");
  CANoeAPI_DetSetError(56, 0x02, "SOAD_E_NULL_PTR");
  CANoeAPI_DetSetError(56, 0x06, "SOAD_E_INV_PDUID");
  CANoeAPI_DetSetError(56, 0x01, "SOAD_E_NOTINIT");
  CANoeAPI_DetSetError(56, 0x04, "SOAD_E_NOBUFS");
  CANoeAPI_DetSetError(56, 0x08, "SOAD_E_INIT_FAILED");
  CANoeAPI_DetSetError(56, 0x05, "SOAD_E_INV_PDUHEADER_ID");
  CANoeAPI_DetSetError(56, 0x03, "SOAD_E_INV_ARG");
}

/* Initialize DET codes for module Spi*/
STATIC void VTTCntrl_DetInitialization_Spi(void)
{
  CANoeAPI_DetSetModule(83, "Spi");
  CANoeAPI_DetSetFunction(83, 0x09, "Spi_GetVersionInfo");
  CANoeAPI_DetSetFunction(83, 0x0A, "Spi_SyncTransmit");
  CANoeAPI_DetSetFunction(83, 0x07, "Spi_GetJobResult");
  CANoeAPI_DetSetFunction(83, 0x06, "Spi_GetStatus");
  CANoeAPI_DetSetFunction(83, 0x03, "Spi_AsyncTransmit");
  CANoeAPI_DetSetFunction(83, 0x08, "Spi_GetSequenceResult");
  CANoeAPI_DetSetFunction(83, 0x10, "Spi_MainFunction_Handling");
  CANoeAPI_DetSetFunction(83, 0x05, "Spi_SetupEB");
  CANoeAPI_DetSetFunction(83, 0x0B, "Spi_GetHWUnitStatus");
  CANoeAPI_DetSetFunction(83, 0x01, "Spi_DeInit");
  CANoeAPI_DetSetFunction(83, 0x0C, "Spi_Cancel");
  CANoeAPI_DetSetFunction(83, 0x0D, "Spi_SetAsyncMode");
  CANoeAPI_DetSetFunction(83, 0x02, "Spi_WriteIB");
  CANoeAPI_DetSetFunction(83, 0x04, "Spi_ReadIB");
  CANoeAPI_DetSetFunction(83, 0x00, "Spi_Init");
  CANoeAPI_DetSetError(83, 0x10, "SPI_E_PARAM_POINTER");
  CANoeAPI_DetSetError(83, 0x0A, "SPI_E_PARAM_CHANNEL");
  CANoeAPI_DetSetError(83, 0x4A, "SPI_E_ALREADY_INITIALIZED");
  CANoeAPI_DetSetError(83, 0x0D, "SPI_E_PARAM_LENGTH");
  CANoeAPI_DetSetError(83, 0x12, "SPI_E_PARAM_VINFO");
  CANoeAPI_DetSetError(83, 0x2A, "SPI_E_SEQ_PENDING");
  CANoeAPI_DetSetError(83, 0x0E, "SPI_E_PARAM_UNIT");
  CANoeAPI_DetSetError(83, 0x11, "SPI_E_PARAM_MODE");
  CANoeAPI_DetSetError(83, 0x0B, "SPI_E_PARAM_JOB");
  CANoeAPI_DetSetError(83, 0x1A, "SPI_E_UNINIT");
  CANoeAPI_DetSetError(83, 0x0F, "SPI_E_PARAM_CONFIG");
  CANoeAPI_DetSetError(83, 0x3A, "SPI_E_SEQ_IN_PROCESS");
  CANoeAPI_DetSetError(83, 0x0C, "SPI_E_PARAM_SEQ");
}

/* Initialize DET codes for module StbM*/
STATIC void VTTCntrl_DetInitialization_StbM(void)
{
  CANoeAPI_DetSetModule(160, "StbM");
  CANoeAPI_DetSetFunction(160, 0x0C, "StbM_SetUserData");
  CANoeAPI_DetSetFunction(160, 0x0B, "StbM_SetGlobalTime");
  CANoeAPI_DetSetFunction(160, 0x0F, "StbM_BusSetGlobalTime");
  CANoeAPI_DetSetFunction(160, 0x07, "StbM_GetCurrentTime");
  CANoeAPI_DetSetFunction(160, 0x00, "StbM_Init");
  CANoeAPI_DetSetFunction(160, 0x0A, "StbM_GetCurrentTimeDiff");
  CANoeAPI_DetSetFunction(160, 0x0E, "StbM_GetOffset");
  CANoeAPI_DetSetFunction(160, 0x05, "StbM_GetVersionInfo");
  CANoeAPI_DetSetFunction(160, 0x04, "StbM_MainFunction");
  CANoeAPI_DetSetFunction(160, 0x0D, "StbM_SetOffset");
  CANoeAPI_DetSetFunction(160, 0x08, "StbM_GetCurrentTimeExtended");
  CANoeAPI_DetSetFunction(160, 0x09, "StbM_GetCurrentTimeRaw");
  CANoeAPI_DetSetError(160, 0x0A, "STBM_E_PARAM");
  CANoeAPI_DetSetError(160, 0x0B, "STBM_E_NOT_INITIALIZED");
  CANoeAPI_DetSetError(160, 0x10, "STBM_E_PARAM_POINTER");
}

/* Initialize DET codes for module UdpNm*/
STATIC void VTTCntrl_DetInitialization_UdpNm(void)
{
  CANoeAPI_DetSetModule(33, "UdpNm");
  CANoeAPI_DetSetFunction(33, 0x0E, "UdpNm_PassiveStartUp");
  CANoeAPI_DetSetFunction(33, 0x08, "UdpNm_RepeatMessageRequest");
  CANoeAPI_DetSetFunction(33, 0x16, "UdpNm_SetSleepReadyBit");
  CANoeAPI_DetSetFunction(33, 0x0F, "UdpNm_SoAdIfTxConfirmation");
  CANoeAPI_DetSetFunction(33, 0x14, "UdpNm_RequestBusSynchronization");
  CANoeAPI_DetSetFunction(33, 0x03, "UdpNm_NetworkRelease");
  CANoeAPI_DetSetFunction(33, 0x15, "UdpNm_Transmit");
  CANoeAPI_DetSetFunction(33, 0x12, "UdpNm_SetCoordBits");
  CANoeAPI_DetSetFunction(33, 0x01, "UdpNm_Init");
  CANoeAPI_DetSetFunction(33, 0x0B, "UdpNm_GetState");
  CANoeAPI_DetSetFunction(33, 0x09, "UdpNm_GetVersionInfo");
  CANoeAPI_DetSetFunction(33, 0x13, "UdpNm_MainFunction_x");
  CANoeAPI_DetSetFunction(33, 0x02, "UdpNm_NetworkRequest");
  CANoeAPI_DetSetFunction(33, 0x06, "UdpNm_GetNodeIdentifier");
  CANoeAPI_DetSetFunction(33, 0x0D, "UdpNm_EnableCommunication");
  CANoeAPI_DetSetFunction(33, 0x07, "UdpNm_GetLocalNodeIdentifier");
  CANoeAPI_DetSetFunction(33, 0x0C, "UdpNm_DisableCommunication");
  CANoeAPI_DetSetFunction(33, 0x20, "UdpNm_VTransmitPdu");
  CANoeAPI_DetSetFunction(33, 0x05, "UdpNm_GetUserData");
  CANoeAPI_DetSetFunction(33, 0x11, "UdpNm_CheckRemoteSleepIndication");
  CANoeAPI_DetSetFunction(33, 0x0A, "UdpNm_GetPduData");
  CANoeAPI_DetSetFunction(33, 0x10, "UdpNm_SoAdIfRxIndication");
  CANoeAPI_DetSetFunction(33, 0x04, "UdpNm_SetUserData");
  CANoeAPI_DetSetError(33, 0x12, "UDPNM_E_NULL_POINTER");
  CANoeAPI_DetSetError(33, 0x02, "UDPNM_E_INVALID_CHANNEL");
  CANoeAPI_DetSetError(33, 0x01, "UDPNM_E_NO_INIT");
  CANoeAPI_DetSetError(33, 0x03, "UDPNM_E_INVALID_PDUID");
  CANoeAPI_DetSetError(33, 0x21, "UDPNM_E_PDUR_TRIGGERTX_ERROR");
}

/* Initialize DET codes for module Wdg*/
STATIC void VTTCntrl_DetInitialization_Wdg(void)
{
  CANoeAPI_DetSetModule(102, "Wdg");
  CANoeAPI_DetSetFunction(102, 0x02, "Wdg_SetTriggerCondition");
  CANoeAPI_DetSetFunction(102, 0x05, "Wdg_Cbk_Gpt_Notification");
  CANoeAPI_DetSetFunction(102, 0x01, "Wdg_SetMode");
  CANoeAPI_DetSetFunction(102, 0x00, "Wdg_Init");
  CANoeAPI_DetSetFunction(102, 0x04, "Wdg_GetVersionInfo");
  CANoeAPI_DetSetError(102, 0x14, "WDG_E_PARAM_VINFO");
  CANoeAPI_DetSetError(102, 0x10, "WDG_E_DRIVER_STATE");
  CANoeAPI_DetSetError(102, 0x11, "WDG_E_PARAM_MODE");
  CANoeAPI_DetSetError(102, 0x12, "WDG_E_PARAM_CONFIG");
}

/* Initialize DET codes for module WdgIf*/
STATIC void VTTCntrl_DetInitialization_WdgIf(void)
{
  CANoeAPI_DetSetModule(43, "WdgIf");
  CANoeAPI_DetSetFunction(43, 0x01, "WdgIf_SetMode");
  CANoeAPI_DetSetFunction(43, 0x03, "WdgIf_GetVersionInfo");
  CANoeAPI_DetSetFunction(43, 0x02, "WdgIf_SetTriggerCondition");
  CANoeAPI_DetSetError(43, 0x02, "WDGIF_E_INV_POINTER");
  CANoeAPI_DetSetError(43, 0x01, "WDGIF_E_PARAM_DEVICE");
}

/* Initialize DET codes for module WdgM*/
STATIC void VTTCntrl_DetInitialization_WdgM(void)
{
  CANoeAPI_DetSetModule(13, "WdgM");
  CANoeAPI_DetSetFunction(13, 0x02, "WdgM_GetVersionInfo");
  CANoeAPI_DetSetFunction(13, 0x04, "WdgM_UpdateAliveCounter");
  CANoeAPI_DetSetFunction(13, 0x0E, "WdgM_CheckpointReached");
  CANoeAPI_DetSetFunction(13, 0x03, "WdgM_SetMode");
  CANoeAPI_DetSetFunction(13, 0x0F, "WdgM_PerformReset");
  CANoeAPI_DetSetFunction(13, 0x10, "WdgM_GetFirstExpiredSEID");
  CANoeAPI_DetSetFunction(13, 0x01, "WdgM_DeInit");
  CANoeAPI_DetSetFunction(13, 0x00, "WdgM_Init");
  CANoeAPI_DetSetFunction(13, 0x0C, "WdgM_GetLocalStatus");
  CANoeAPI_DetSetFunction(13, 0x0B, "WdgM_GetMode");
  CANoeAPI_DetSetFunction(13, 0x0D, "WdgM_GetGlobalStatus");
  CANoeAPI_DetSetError(13, 0x18, "WDGM_E_AMBIGIOUS");
  CANoeAPI_DetSetError(13, 0x14, "WDGM_E_INV_POINTER");
  CANoeAPI_DetSetError(13, 0x12, "WDGM_E_PARAM_MODE");
  CANoeAPI_DetSetError(13, 0x11, "WDGM_E_PARAM_CONFIG");
  CANoeAPI_DetSetError(13, 0x10, "WDGM_E_NO_INIT");
  CANoeAPI_DetSetError(13, 0x17, "WDGM_E_DEPERCATED");
  CANoeAPI_DetSetError(13, 0x15, "WDGM_E_DISABLE_NOT_ALLOWED");
  CANoeAPI_DetSetError(13, 0x19, "WDGM_E_SEDEACTIVATED");
  CANoeAPI_DetSetError(13, 0x13, "WDGM_E_PARAM_SEID");
  CANoeAPI_DetSetError(13, 0x24, "WDGM_E_ILLEGAL_CALLBACK_HANDLING");
  CANoeAPI_DetSetError(13, 0x16, "WDGM_E_CPID");
}

/* Initialize DET codes for module Xcp*/
STATIC void VTTCntrl_DetInitialization_Xcp(void)
{
  CANoeAPI_DetSetModule(26, "Xcp");
  CANoeAPI_DetSetFunction(26, 0x10, "Xcp_GetActiveTl");
  CANoeAPI_DetSetFunction(26, 0x0b, "Xcp_Command");
  CANoeAPI_DetSetFunction(26, 0x0d, "Xcp_Exit");
  CANoeAPI_DetSetFunction(26, 0x08, "Xcp_GetXcpDataPointer");
  CANoeAPI_DetSetFunction(26, 0x05, "Xcp_Print");
  CANoeAPI_DetSetFunction(26, 0x04, "Xcp_Putchar");
  CANoeAPI_DetSetFunction(26, 0x03, "Xcp_SendEvent");
  CANoeAPI_DetSetFunction(26, 0x09, "Xcp_Control");
  CANoeAPI_DetSetFunction(26, 0x0a, "Xcp_GetVersionInfo");
  CANoeAPI_DetSetFunction(26, 0x06, "Xcp_Disconnect");
  CANoeAPI_DetSetFunction(26, 0x0e, "Xcp_GetSessionStatus");
  CANoeAPI_DetSetFunction(26, 0x0f, "Xcp_SetActiveTl");
  CANoeAPI_DetSetFunction(26, 0x02, "Xcp_StimEventStatus");
  CANoeAPI_DetSetFunction(26, 0x0c, "Xcp_SendCallBack");
  CANoeAPI_DetSetFunction(26, 0x07, "Xcp_SendCrm");
  CANoeAPI_DetSetFunction(26, 0x11, "Xcp_Printf");
  CANoeAPI_DetSetError(26, 0x4, "XCP_E_WRONG_CONTROL_COMMAND");
  CANoeAPI_DetSetError(26, 0x3, "XCP_E_INVALID_PDUID");
  CANoeAPI_DetSetError(26, 0x1, "XCP_E_INV_POINTER");
  CANoeAPI_DetSetError(26, 0x12, "XCP_E_NULL_POINTER");
  CANoeAPI_DetSetError(26, 0x2, "XCP_E_NOT_INITIALIZED");
}

/* Initialize all DET codes for all modules */
STATIC void VTTCntrl_DetInitialization(void) 
{
  VTTCntrl_DetInitialization_Adc();
  VTTCntrl_DetInitialization_BswM();
  VTTCntrl_DetInitialization_Can();
  VTTCntrl_DetInitialization_CanIf();
  VTTCntrl_DetInitialization_CanNm();
  VTTCntrl_DetInitialization_CanSM();
  VTTCntrl_DetInitialization_CanTSyn();
  VTTCntrl_DetInitialization_CanTp();
  VTTCntrl_DetInitialization_CanXcp();
  VTTCntrl_DetInitialization_Com();
  VTTCntrl_DetInitialization_ComM();
  VTTCntrl_DetInitialization_Crc();
  VTTCntrl_DetInitialization_Csm();
  VTTCntrl_DetInitialization_Dcm();
  VTTCntrl_DetInitialization_Dem();
  VTTCntrl_DetInitialization_Dio();
  VTTCntrl_DetInitialization_DoIp();
  VTTCntrl_DetInitialization_Ea();
  VTTCntrl_DetInitialization_EcuM();
  VTTCntrl_DetInitialization_Eep();
  VTTCntrl_DetInitialization_Eth();
  VTTCntrl_DetInitialization_EthIf();
  VTTCntrl_DetInitialization_EthSM();
  VTTCntrl_DetInitialization_EthTSyn();
  VTTCntrl_DetInitialization_EthTrcv();
  VTTCntrl_DetInitialization_Fee();
  VTTCntrl_DetInitialization_FiM();
  VTTCntrl_DetInitialization_Fls();
  VTTCntrl_DetInitialization_Fr();
  VTTCntrl_DetInitialization_FrIf();
  VTTCntrl_DetInitialization_FrNm();
  VTTCntrl_DetInitialization_FrSM();
  VTTCntrl_DetInitialization_FrTSyn();
  VTTCntrl_DetInitialization_FrTp();
  VTTCntrl_DetInitialization_FrXcp();
  VTTCntrl_DetInitialization_Gpt();
  VTTCntrl_DetInitialization_Icu();
  VTTCntrl_DetInitialization_IoHwAb();
  VTTCntrl_DetInitialization_J1939Dcm();
  VTTCntrl_DetInitialization_J1939Nm();
  VTTCntrl_DetInitialization_J1939Rm();
  VTTCntrl_DetInitialization_J1939Tp();
  VTTCntrl_DetInitialization_Lin();
  VTTCntrl_DetInitialization_LinIf();
  VTTCntrl_DetInitialization_LinNm();
  VTTCntrl_DetInitialization_LinSM();
  VTTCntrl_DetInitialization_Mcu();
  VTTCntrl_DetInitialization_MemIf();
  VTTCntrl_DetInitialization_Nm();
  VTTCntrl_DetInitialization_NvM();
  VTTCntrl_DetInitialization_Port();
  VTTCntrl_DetInitialization_Pwm();
  VTTCntrl_DetInitialization_RamTst();
  VTTCntrl_DetInitialization_Rte();
  VTTCntrl_DetInitialization_Sd();
  VTTCntrl_DetInitialization_SecOC();
  VTTCntrl_DetInitialization_SoAd();
  VTTCntrl_DetInitialization_Spi();
  VTTCntrl_DetInitialization_StbM();
  VTTCntrl_DetInitialization_UdpNm();
  VTTCntrl_DetInitialization_Wdg();
  VTTCntrl_DetInitialization_WdgIf();
  VTTCntrl_DetInitialization_WdgM();
  VTTCntrl_DetInitialization_Xcp();
}
void VttCntrl_Det_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  newState = newState;
  oldState = oldState;

  switch(action)
  {
  case CANOEAPI_ECUACTION_LOAD:
    VTTCntrl_DetInitialization();
    break;
  case CANOEAPI_ECUACTION_STARTMEASUREMENT:
  case CANOEAPI_ECUACTION_SWITCHON:
  case CANOEAPI_ECUACTION_RESET:
  case CANOEAPI_ECUACTION_GOTOSLEEP:
  case CANOEAPI_ECUACTION_WAKEUP:
  case CANOEAPI_ECUACTION_SWITCHOFF:
  case CANOEAPI_ECUACTION_STOPMEASUREMENT:
  case CANOEAPI_ECUACTION_UNLOAD:
    break;
  }
}
/**********************************************************************************************************************
 *  END OF FILE: VttCntrl_Det.c
 *********************************************************************************************************************/
