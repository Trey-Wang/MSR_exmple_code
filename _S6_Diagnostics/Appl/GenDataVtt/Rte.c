/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *          File:  Rte.c
 *        Config:  MyECU.dpa
 *   ECU-Project:  MyECU
 *
 *     Generator:  MICROSAR RTE Generator Version 4.16.0
 *                 RTE Core Version 1.16.0
 *       License:  Unlimited license CBD0000000 for N/A
 *
 *   Description:  RTE implementation file
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* PRQA S 0777, 0779, 0781, 0857 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_Rte_0781, MD_MSR_1.1_857 */

#define RTE_CORE
#include "Os.h" /* PRQA S 0828, 0883 */ /* MD_MSR_1.1_828, MD_Rte_Os */
#include "Rte_Type.h"
#include "Rte_Main.h"

#include "Rte_BswM.h"
#include "Rte_ComM.h"
#include "Rte_CtApMySwc.h"
#include "Rte_CtCddIoHwAb.h"
#include "Rte_CtSaDisplay.h"
#include "Rte_CtSaDoor.h"
#include "Rte_CtSaInteriorLight.h"
#include "Rte_Dcm.h"
#include "Rte_DemMaster_0.h"
#include "Rte_DemSatellite_0.h"
#include "Rte_Det.h"
#include "Rte_EcuM.h"
#include "Rte_NvM.h"
#include "SchM_BswM.h"
#include "SchM_Can.h"
#include "SchM_CanIf.h"
#include "SchM_CanNm.h"
#include "SchM_CanSM.h"
#include "SchM_CanTp.h"
#include "SchM_Com.h"
#include "SchM_ComM.h"
#include "SchM_Dcm.h"
#include "SchM_Dem.h"
#include "SchM_Det.h"
#include "SchM_EcuM.h"
#include "SchM_Fee.h"
#include "SchM_Fls.h"
#include "SchM_Nm.h"
#include "SchM_NvM.h"
#include "SchM_PduR.h"

#include "Rte_Hook.h"
#include "Rte_VttHook.h"

#include "Com.h"
#if defined(IL_ASRCOM_VERSION)
# define RTE_USE_COM_TXSIGNAL_RDACCESS
#endif

#include "Rte_Cbk.h"

/* AUTOSAR 3.x compatibility */
#if !defined (RTE_LOCAL)
# define RTE_LOCAL static
#endif


/**********************************************************************************************************************
 * API for enable / disable interrupts global
 *********************************************************************************************************************/

#if defined(osDisableGlobalKM) && !defined(RTE_DISABLE_ENHANCED_INTERRUPT_LOCK_API)
# define Rte_DisableAllInterrupts() osDisableGlobalKM()   /* MICROSAR OS */
#else
# define Rte_DisableAllInterrupts() DisableAllInterrupts()   /* AUTOSAR OS */
#endif

#if defined(osEnableGlobalKM) && !defined(RTE_DISABLE_ENHANCED_INTERRUPT_LOCK_API)
# define Rte_EnableAllInterrupts() osEnableGlobalKM()   /* MICROSAR OS */
#else
# define Rte_EnableAllInterrupts() EnableAllInterrupts()   /* AUTOSAR OS */
#endif

/**********************************************************************************************************************
 * API for enable / disable interrupts up to the systemLevel
 *********************************************************************************************************************/

#if defined(osDisableLevelKM) && !defined(RTE_DISABLE_ENHANCED_INTERRUPT_LOCK_API)
# define Rte_DisableOSInterrupts() osDisableLevelKM()   /* MICROSAR OS */
#else
# define Rte_DisableOSInterrupts() SuspendOSInterrupts()   /* AUTOSAR OS */
#endif

#if defined(osEnableLevelKM) && !defined(RTE_DISABLE_ENHANCED_INTERRUPT_LOCK_API)
# define Rte_EnableOSInterrupts() osEnableLevelKM()   /* MICROSAR OS */
#else
# define Rte_EnableOSInterrupts() ResumeOSInterrupts()   /* AUTOSAR OS */
#endif


/**********************************************************************************************************************
 * Buffers for unqueued S/R
 *********************************************************************************************************************/

#define RTE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(BswM_MdgShutdownTimerState, RTE_VAR_NOINIT) Rte_BswM_Provide_MrpPShutdownTimerDisplay_requestedMode; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(BswM_ESH_RunRequest, RTE_VAR_NOINIT) Rte_CpApMySwc_PpBswMPostRunRequestCtApMySwc_requestedMode; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(BswM_ESH_RunRequest, RTE_VAR_NOINIT) Rte_CpApMySwc_PpBswMRunRequestCtApMySwc_requestedMode; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_NOINIT) Rte_CpApMySwc_PpDisplayState_DeDisplayBusState; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint32, RTE_VAR_NOINIT) Rte_CpApMySwc_PpDisplayState_DeLightOnOffCounter; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint32, RTE_VAR_NOINIT) Rte_CpApMySwc_PpDisplayState_DeOdometerValue; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_NOINIT) Rte_CpApMySwc_PpDisplayState_DeOdometerWriteRequestPending; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(IdtLightState, RTE_VAR_NOINIT) Rte_CpApMySwc_PpLightStateFront_DeLightState; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(IdtDisplayIgnitionState, RTE_VAR_NOINIT) Rte_CpSaDisplay_PpLocalIgnitionState_DeIgnitionState; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(IdtDoorState, RTE_VAR_NOINIT) Rte_CpSaDoorFrontLeft_PpDoorState_DeDoorState; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(IdtDoorState, RTE_VAR_NOINIT) Rte_CpSaDoorFrontRight_PpDoorState_DeDoorState; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */

#define RTE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * Calibration Parameters (SW-C local and calibration component calibration parameters)
 *********************************************************************************************************************/

#define RTE_START_SEC_CONST_DEFAULT_RTE_CDATA_GROUP_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* PRQA S 0850, 3408 L1 */ /* MD_MSR_19.8, MD_Rte_3408 */
CONST(uint32, RTE_CONST_DEFAULT_RTE_CDATA_GROUP) Rte_CtApMySwc_CalOdometer = 1000U;
/* PRQA L:L1 */

#define RTE_STOP_SEC_CONST_DEFAULT_RTE_CDATA_GROUP_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * Per-Instance Memory
 *********************************************************************************************************************/

#define RTE_START_SEC_VAR_DEFAULT_RTE_PIM_GROUP_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(uint32, RTE_VAR_DEFAULT_RTE_PIM_GROUP) Rte_CpApMySwc_PimLightOnOffCounter; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint32, RTE_VAR_DEFAULT_RTE_PIM_GROUP) Rte_CpApMySwc_PimOdometer; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */

#define RTE_STOP_SEC_VAR_DEFAULT_RTE_PIM_GROUP_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * Buffers for implicit communication
 *********************************************************************************************************************/

#define RTE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(Rte_tsMy_Task, RTE_VAR_NOINIT) Rte_My_Task; /* PRQA S 0850, 0759, 3408, 1504 */ /* MD_MSR_19.8, MD_MSR_18.4, MD_Rte_3408, MD_MSR_8.10 */

#define RTE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * Buffer for inter-runnable variables
 *********************************************************************************************************************/

#define RTE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(uint16, RTE_VAR_NOINIT) Rte_Irv_CpApMySwc_IrvDoorStateOld; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */

#define RTE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define RTE_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * API Prototypes
 *********************************************************************************************************************/
FUNC(Std_ReturnType, RTE_CODE) Rte_CallInst1_CtSaDoor_PpDoorIoHwAb_ReadChannel(P2VAR(IdtDioValueType, AUTOMATIC, RTE_CTSADOOR_APPL_VAR) value); /* PRQA S 0850, 3447 */ /* MD_MSR_19.8, MD_Rte_3447 */
FUNC(Std_ReturnType, RTE_CODE) Rte_WriteInst1_CtSaDoor_PpDoorState_DeDoorState(IdtDoorState data); /* PRQA S 0850, 3447 */ /* MD_MSR_19.8, MD_Rte_3447 */
FUNC(Std_ReturnType, RTE_CODE) Rte_CallInst2_CtSaDoor_PpDoorIoHwAb_ReadChannel(P2VAR(IdtDioValueType, AUTOMATIC, RTE_CTSADOOR_APPL_VAR) value); /* PRQA S 0850, 3447 */ /* MD_MSR_19.8, MD_Rte_3447 */
FUNC(Std_ReturnType, RTE_CODE) Rte_WriteInst2_CtSaDoor_PpDoorState_DeDoorState(IdtDoorState data); /* PRQA S 0850, 3447 */ /* MD_MSR_19.8, MD_Rte_3447 */

#define RTE_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define RTE_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * Component Data Structures
 *********************************************************************************************************************/

/* PRQA S 0850, 3408, 1504 L1 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
CONST(struct Rte_CDS_CtSaDoor, RTE_CONST) Rte_Instance_CpSaDoorFrontLeft =
{
  /* Port API section */
  {
    Rte_CallInst1_CtSaDoor_PpDoorIoHwAb_ReadChannel,
  },
  {
    Rte_WriteInst1_CtSaDoor_PpDoorState_DeDoorState,
  },
  /* Instance Id section */
  0,
  /* Vendor specific section */
};
/* PRQA L:L1 */

/* PRQA S 0850, 3408, 1504 L1 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
CONST(struct Rte_CDS_CtSaDoor, RTE_CONST) Rte_Instance_CpSaDoorFrontRight =
{
  /* Port API section */
  {
    Rte_CallInst2_CtSaDoor_PpDoorIoHwAb_ReadChannel,
  },
  {
    Rte_WriteInst2_CtSaDoor_PpDoorState_DeDoorState,
  },
  /* Instance Id section */
  1,
  /* Vendor specific section */
};
/* PRQA L:L1 */

#define RTE_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */



#define RTE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * Data structures for mode management
 *********************************************************************************************************************/
RTE_LOCAL VAR(Rte_ModeInfoType_BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode, RTE_VAR_NOINIT) Rte_ModeInfo_BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode;

VAR(BswM_ESH_Mode, RTE_VAR_NOINIT) Rte_ModeMachine_BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode; /* PRQA S 3408 */ /* MD_Rte_3408 */
RTE_LOCAL VAR(Rte_ModeInfoType_ComM_UM_USR_INTERIOR_LIGHT_currentMode, RTE_VAR_NOINIT) Rte_ModeInfo_ComM_UM_USR_INTERIOR_LIGHT_currentMode;

VAR(ComM_ModeType, RTE_VAR_NOINIT) Rte_ModeMachine_ComM_UM_USR_INTERIOR_LIGHT_currentMode; /* PRQA S 3408 */ /* MD_Rte_3408 */


#define RTE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define RTE_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* PRQA S 3408 L1 */ /* MD_Rte_3408 */
CONST(EventMaskType, RTE_CONST) Rte_ModeEntryEventMask_BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode[5] =
{
  Rte_Ev_Run1_CpApMySwc_RCtApMySwcPostRunCode, /* POSTRUN */
  (EventMaskType) 0, /* RUN */
  (EventMaskType) 0, /* SHUTDOWN */
  (EventMaskType) 0, /* STARTUP */
  (EventMaskType) 0  /* WAKEUP */
};
/* PRQA L:L1 */


/* PRQA S 3408 L1 */ /* MD_Rte_3408 */
CONST(EventMaskType, RTE_CONST) Rte_ModeExitEventMask_ComM_UM_USR_INTERIOR_LIGHT_currentMode[3] =
{
  Rte_Ev_Run_CpApMySwc_RCtApMySwcComM_ModeChange_FULL_COMM_Exit, /* COMM_FULL_COMMUNICATION */
  (EventMaskType) 0, /* COMM_NO_COMMUNICATION */
  (EventMaskType) 0  /* COMM_SILENT_COMMUNICATION */
};
/* PRQA L:L1 */

/* PRQA S 3408 L1 */ /* MD_Rte_3408 */
CONST(EventMaskType, RTE_CONST) Rte_ModeEntryEventMask_ComM_UM_USR_INTERIOR_LIGHT_currentMode[3] =
{
  Rte_Ev_Run_CpApMySwc_RCtApMySwcComM_ModeChange_FULL_COMM_Entry, /* COMM_FULL_COMMUNICATION */
  (EventMaskType) 0, /* COMM_NO_COMMUNICATION */
  (EventMaskType) 0  /* COMM_SILENT_COMMUNICATION */
};
/* PRQA L:L1 */


#define RTE_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * Timer handling
 *********************************************************************************************************************/

#if defined OS_US2TICKS_SystemTimer
# define RTE_USEC_SystemTimer OS_US2TICKS_SystemTimer
#else
# define RTE_USEC_SystemTimer(val) ((TickType)RTE_CONST_USEC_SystemTimer_##val) /* PRQA S 0342 */ /* MD_MSR_19.13_0342 */
#endif

#if defined OS_MS2TICKS_SystemTimer
# define RTE_MSEC_SystemTimer OS_MS2TICKS_SystemTimer
#else
# define RTE_MSEC_SystemTimer(val) ((TickType)RTE_CONST_MSEC_SystemTimer_##val) /* PRQA S 0342 */ /* MD_MSR_19.13_0342 */
#endif

#if defined OS_SEC2TICKS_SystemTimer
# define RTE_SEC_SystemTimer OS_SEC2TICKS_SystemTimer
#else
# define RTE_SEC_SystemTimer(val)  ((TickType)RTE_CONST_SEC_SystemTimer_##val) /* PRQA S 0342 */ /* MD_MSR_19.13_0342 */
#endif

#define RTE_CONST_MSEC_SystemTimer_0 (0UL)
#define RTE_CONST_MSEC_SystemTimer_10 (10UL)
#define RTE_CONST_MSEC_SystemTimer_20 (20UL)
#define RTE_CONST_MSEC_SystemTimer_200 (200UL)
#define RTE_CONST_MSEC_SystemTimer_5 (5UL)


/**********************************************************************************************************************
 * Internal definitions
 *********************************************************************************************************************/

#define RTE_TASK_TIMEOUT_EVENT_MASK   ((EventMaskType)0x01)
#define RTE_TASK_WAITPOINT_EVENT_MASK ((EventMaskType)0x02)

/**********************************************************************************************************************
 * RTE life cycle API
 *********************************************************************************************************************/

#define RTE_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

FUNC(void, RTE_CODE) SchM_Init(void)
{
  /* activate the tasks */
  (void)ActivateTask(SchM_Task); /* PRQA S 3417 */ /* MD_Rte_Os */

  /* activate the alarms used for TimingEvents */
  (void)SetRelAlarm(Rte_Al_TE2_SchM_Task_0_10ms, RTE_MSEC_SystemTimer(0) + (TickType)1, RTE_MSEC_SystemTimer(10)); /* PRQA S 3417 */ /* MD_Rte_Os */
  (void)SetRelAlarm(Rte_Al_TE2_SchM_Task_0_20ms, RTE_MSEC_SystemTimer(0) + (TickType)1, RTE_MSEC_SystemTimer(20)); /* PRQA S 3417 */ /* MD_Rte_Os */
  (void)SetRelAlarm(Rte_Al_TE2_SchM_Task_0_5ms, RTE_MSEC_SystemTimer(0) + (TickType)1, RTE_MSEC_SystemTimer(5)); /* PRQA S 3417 */ /* MD_Rte_Os */

}

FUNC(Std_ReturnType, RTE_CODE) Rte_Start(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* set default values for internal data */
  Rte_BswM_Provide_MrpPShutdownTimerDisplay_requestedMode = 1U;
  Rte_CpApMySwc_PpBswMPostRunRequestCtApMySwc_requestedMode = 0U;
  Rte_CpApMySwc_PpBswMRunRequestCtApMySwc_requestedMode = 0U;
  Rte_CpApMySwc_PpDisplayState_DeDisplayBusState = FALSE;
  Rte_CpApMySwc_PpDisplayState_DeLightOnOffCounter = 0U;
  Rte_CpApMySwc_PpDisplayState_DeOdometerValue = 0U;
  Rte_CpApMySwc_PpDisplayState_DeOdometerWriteRequestPending = FALSE;
  Rte_CpApMySwc_PpLightStateFront_DeLightState = 0U;
  Rte_CpSaDisplay_PpLocalIgnitionState_DeIgnitionState = 1U;
  Rte_CpSaDoorFrontLeft_PpDoorState_DeDoorState = 0U;
  Rte_CpSaDoorFrontRight_PpDoorState_DeDoorState = 0U;

  /* initialize inter-runnable variables */
  Rte_Irv_CpApMySwc_IrvDoorStateOld = 0U;

  /* mode management initialization part 1 */

  Rte_ModeMachine_BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode = RTE_MODE_ESH_Mode_STARTUP;

  Rte_ModeMachine_ComM_UM_USR_INTERIOR_LIGHT_currentMode = RTE_MODE_ComMMode_COMM_NO_COMMUNICATION;

  /* activate the tasks */
  (void)ActivateTask(IO_Task); /* PRQA S 3417 */ /* MD_Rte_Os */
  (void)ActivateTask(My_Task); /* PRQA S 3417 */ /* MD_Rte_Os */

  /* activate the alarms used for TimingEvents */
  (void)SetRelAlarm(Rte_Al_TE_CpApMySwc_RCtApMySwcCode, RTE_MSEC_SystemTimer(0) + (TickType)1, RTE_MSEC_SystemTimer(200)); /* PRQA S 3417 */ /* MD_Rte_Os */
  (void)SetRelAlarm(Rte_Al_TE_IO_Task_0_200ms, RTE_MSEC_SystemTimer(0) + (TickType)1, RTE_MSEC_SystemTimer(200)); /* PRQA S 3417 */ /* MD_Rte_Os */

  return RTE_E_OK;
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Stop(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* deactivate alarms */
  (void)CancelAlarm(Rte_Al_TE_IO_Task_0_200ms); /* PRQA S 3417 */ /* MD_Rte_Os */
  (void)CancelAlarm(Rte_Al_TE_CpApMySwc_RCtApMySwcCode); /* PRQA S 3417 */ /* MD_Rte_Os */

  return RTE_E_OK;
}

FUNC(void, RTE_CODE) SchM_Deinit(void)
{
  /* deactivate alarms */
  (void)CancelAlarm(Rte_Al_TE2_SchM_Task_0_10ms); /* PRQA S 3417 */ /* MD_Rte_Os */
  (void)CancelAlarm(Rte_Al_TE2_SchM_Task_0_20ms); /* PRQA S 3417 */ /* MD_Rte_Os */
  (void)CancelAlarm(Rte_Al_TE2_SchM_Task_0_5ms); /* PRQA S 3417 */ /* MD_Rte_Os */

}

FUNC(void, RTE_CODE) Rte_InitMemory(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
}


/**********************************************************************************************************************
 * Internal/External Tx connections
 *********************************************************************************************************************/

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_BswM_Provide_MrpPShutdownTimerDisplay_requestedMode(BswM_MdgShutdownTimerState data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_BswM_Provide_MrpPShutdownTimerDisplay_requestedMode = *(&data);
  /* scheduled trigger for runnables: RCtApMySwcPostRunCode */
  (void)SetEvent(My_Task, Rte_Ev_Run_CpApMySwc_RCtApMySwcPostRunCode); /* PRQA S 3417 */ /* MD_Rte_Os */

  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_CtApMySwc_PpDisplayState_DeDisplayBusState(boolean data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CpApMySwc_PpDisplayState_DeDisplayBusState = *(&data);
  /* scheduled trigger for runnables: RCtApDisplayBusStateHandling */
  (void)SetEvent(IO_Task, Rte_Ev_Run_CpSaDisplay_RCtApDisplayBusStateHandling); /* PRQA S 3417 */ /* MD_Rte_Os */

  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_CtApMySwc_PpDisplayState_DeLightOnOffCounter(uint32 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CpApMySwc_PpDisplayState_DeLightOnOffCounter = *(&data);
  /* scheduled trigger for runnables: RCtApDisplayLightOnOffHandling */
  (void)SetEvent(IO_Task, Rte_Ev_Run_CpSaDisplay_RCtApDisplayLightOnOffHandling); /* PRQA S 3417 */ /* MD_Rte_Os */

  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_CtApMySwc_PpDisplayState_DeOdometerValue(uint32 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CpApMySwc_PpDisplayState_DeOdometerValue = *(&data);
  /* scheduled trigger for runnables: RCtApDisplayOdometerValue */
  (void)SetEvent(IO_Task, Rte_Ev_Run_CpSaDisplay_RCtApDisplayOdometerValue); /* PRQA S 3417 */ /* MD_Rte_Os */

  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_CtApMySwc_PpDisplayState_DeOdometerWriteRequestPending(boolean data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CpApMySwc_PpDisplayState_DeOdometerWriteRequestPending = *(&data);
  /* scheduled trigger for runnables: RCtApDisplayOdometerWriteRequestPending */
  (void)SetEvent(IO_Task, Rte_Ev_Run_CpSaDisplay_RCtApDisplayOdometerWriteRequestPending); /* PRQA S 3417 */ /* MD_Rte_Os */

  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_CtApMySwc_PpLightStateFront_DeLightState(IdtLightState data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CpApMySwc_PpLightStateFront_DeLightState = *(&data);
  /* scheduled trigger for runnables: RCtSaInteriorLightSwitchLight */
  (void)SetEvent(IO_Task, Rte_Ev_Run_CpSaInteriorLightFront_RCtSaInteriorLightSwitchLight); /* PRQA S 3417 */ /* MD_Rte_Os */

  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_CtApMySwc_PpLightStateRear_DeLightState(IdtLightState data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  ret |= Com_SendSignal(ComConf_ComSignal_sig_RearInteriorLight_0723e95e, (&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */

  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_WriteInst1_CtSaDoor_PpDoorState_DeDoorState(IdtDoorState data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CpSaDoorFrontLeft_PpDoorState_DeDoorState = *(&data);

  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_WriteInst2_CtSaDoor_PpDoorState_DeDoorState(IdtDoorState data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CpSaDoorFrontRight_PpDoorState_DeDoorState = *(&data);

  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */


/**********************************************************************************************************************
 * Internal/External Rx connections
 *********************************************************************************************************************/

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_BswM_Request_ESH_PostRunRequest_0_requestedMode(P2VAR(BswM_ESH_RunRequest, AUTOMATIC, RTE_BSWM_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  *(data) = Rte_CpApMySwc_PpBswMPostRunRequestCtApMySwc_requestedMode;

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_BswM_Request_ESH_RunRequest_0_requestedMode(P2VAR(BswM_ESH_RunRequest, AUTOMATIC, RTE_BSWM_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  *(data) = Rte_CpApMySwc_PpBswMRunRequestCtApMySwc_requestedMode;

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */


/**********************************************************************************************************************
 * Internal C/S connections
 *********************************************************************************************************************/

FUNC(Std_ReturnType, RTE_CODE) Rte_CallInst1_CtSaDoor_PpDoorIoHwAb_ReadChannel(P2VAR(IdtDioValueType, AUTOMATIC, RTE_CTSADOOR_APPL_VAR) value) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_OK; /* PRQA S 3197 */ /* MD_Rte_3197 */

  RCtCddIoHwAb_PpDoorFrontLeft_ReadChannel(value);

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_CallInst2_CtSaDoor_PpDoorIoHwAb_ReadChannel(P2VAR(IdtDioValueType, AUTOMATIC, RTE_CTSADOOR_APPL_VAR) value) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_OK; /* PRQA S 3197 */ /* MD_Rte_3197 */

  RCtCddIoHwAb_PpDoorFrontRight_ReadChannel(value);

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_Dcm_SecurityAccess_Level_01_CompareKey(P2CONST(Dcm_Data2ByteType, AUTOMATIC, RTE_DCM_APPL_DATA) Key, Dcm_OpStatusType OpStatus, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_DCM_APPL_VAR) ErrorCode) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */


  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_Dcm_SecurityAccess_Level_01_GetSeed(Dcm_OpStatusType OpStatus, P2VAR(Dcm_Data2ByteType, AUTOMATIC, RTE_DCM_APPL_VAR) Seed, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_DCM_APPL_VAR) ErrorCode) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */


  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_CpApMySwcSnNvMPimOdometer_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_OK; /* PRQA S 3197 */ /* MD_Rte_3197 */

  PpNvMNotifyJobFinishedOdometerCtApMySwc_JobFinished(ServiceId, JobResult);

  return ret;
}


/**********************************************************************************************************************
 * Mode Switch API (Rte_Switch)
 *********************************************************************************************************************/

FUNC(Std_ReturnType, RTE_CODE) Rte_Switch_BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode(BswM_ESH_Mode nextMode) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  BswM_ESH_Mode currentMode;
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  currentMode = Rte_ModeMachine_BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode;
  if (nextMode >= 5U)
  {
    ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_CallDetReportError(RTE_SWITCH_SERVICE_ID, RTE_E_DET_MODEARGUMENT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    ret = RTE_E_LIMIT;
  }
  else if (currentMode == RTE_TRANSITION_ESH_Mode)
  {
    ret = RTE_E_LIMIT;
    ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  else if (currentMode >= 5U)
  {
    ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_CallDetReportError(RTE_SWITCH_SERVICE_ID, RTE_E_DET_MODESTATE); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    ret = RTE_E_LIMIT;
  }
  else
  {
    EventMaskType ModeSwitchEventMask;

    ModeSwitchEventMask = Rte_ModeEntryEventMask_BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode[nextMode];

    if (ModeSwitchEventMask != (EventMaskType)0)
    {
      Rte_ModeInfo_BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode.Rte_ModeQueue[0] = nextMode;

      Rte_ModeMachine_BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode = RTE_TRANSITION_ESH_Mode;
      ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      (void)SetEvent(My_Task, ModeSwitchEventMask); /* PRQA S 3417 */ /* MD_Rte_Os */
    }
    else
    {
      Rte_ModeMachine_BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode = nextMode;
      ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */


/**********************************************************************************************************************
 * Mode reading API (Rte_Mode)
 *********************************************************************************************************************/

FUNC(BswM_ESH_Mode, RTE_CODE) Rte_Mode_BswM_Notification_ESH_ModeNotification_BswM_MDGP_ESH_Mode(void) /* PRQA S 3408 */ /* MD_Rte_3408 */
{
  return Rte_ModeMachine_BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode;
}


/**********************************************************************************************************************
 * Mode Switch API (Rte_Switch)
 *********************************************************************************************************************/

FUNC(Std_ReturnType, RTE_CODE) Rte_Switch_ComM_UM_USR_INTERIOR_LIGHT_currentMode(ComM_ModeType nextMode) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  ComM_ModeType currentMode;
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  currentMode = Rte_ModeMachine_ComM_UM_USR_INTERIOR_LIGHT_currentMode;
  if (nextMode >= 3U)
  {
    ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_CallDetReportError(RTE_SWITCH_SERVICE_ID, RTE_E_DET_MODEARGUMENT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    ret = RTE_E_LIMIT;
  }
  else if (currentMode == RTE_TRANSITION_ComMMode)
  {
    ret = RTE_E_LIMIT;
    ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  else if (currentMode >= 3U)
  {
    ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_CallDetReportError(RTE_SWITCH_SERVICE_ID, RTE_E_DET_MODESTATE); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    ret = RTE_E_LIMIT;
  }
  else
  {
    EventMaskType ModeSwitchEventMask;

    ModeSwitchEventMask = Rte_ModeExitEventMask_ComM_UM_USR_INTERIOR_LIGHT_currentMode[currentMode] | Rte_ModeEntryEventMask_ComM_UM_USR_INTERIOR_LIGHT_currentMode[nextMode];

    if (ModeSwitchEventMask != (EventMaskType)0)
    {
      Rte_ModeInfo_ComM_UM_USR_INTERIOR_LIGHT_currentMode.Rte_ModeQueue[0] = nextMode;

      Rte_ModeMachine_ComM_UM_USR_INTERIOR_LIGHT_currentMode = RTE_TRANSITION_ComMMode;
      ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      (void)SetEvent(My_Task, ModeSwitchEventMask); /* PRQA S 3417 */ /* MD_Rte_Os */
    }
    else
    {
      Rte_ModeMachine_ComM_UM_USR_INTERIOR_LIGHT_currentMode = nextMode;
      ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */


/**********************************************************************************************************************
 * Transmission/Mode Switch Acknowledgement handling (Rte_Feedback/Rte_SwitchAck)
 *********************************************************************************************************************/

FUNC(Std_ReturnType, RTE_CODE) Rte_SwitchAck_Dcm_DcmEcuReset_DcmEcuReset(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{

  return RTE_E_UNCONNECTED;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */


/**********************************************************************************************************************
 * Mode Switch API (Rte_Switch)
 *********************************************************************************************************************/

FUNC(Std_ReturnType, RTE_CODE) Rte_Switch_Dcm_DcmControlDtcSetting_DcmControlDtcSetting(Dcm_ControlDtcSettingType nextMode) /* PRQA S 0850, 1505, 3206 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206 */
{
  Std_ReturnType ret = RTE_E_OK;


  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Switch_Dcm_DcmDiagnosticSessionControl_DcmDiagnosticSessionControl(Dcm_DiagnosticSessionControlType nextMode) /* PRQA S 0850, 1505, 3206 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206 */
{
  Std_ReturnType ret = RTE_E_OK;


  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Switch_Dcm_DcmEcuReset_DcmEcuReset(Dcm_EcuResetType nextMode) /* PRQA S 0850, 1505, 3206 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206 */
{
  Std_ReturnType ret = RTE_E_OK;


  return ret;
}


/**********************************************************************************************************************
 * Task bodies for RTE controlled tasks
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Task:     IO_Task
 * Priority: 2
 * Schedule: NON
 *********************************************************************************************************************/
TASK(IO_Task) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  EventMaskType ev;

  for(;;)
  {
    (void)WaitEvent(Rte_Ev_Cyclic_IO_Task_0_200ms | Rte_Ev_Run_CpSaDisplay_RCtApDisplayBusStateHandling | Rte_Ev_Run_CpSaDisplay_RCtApDisplayLightOnOffHandling | Rte_Ev_Run_CpSaDisplay_RCtApDisplayOdometerValue | Rte_Ev_Run_CpSaDisplay_RCtApDisplayOdometerWriteRequestPending | Rte_Ev_Run_CpSaInteriorLightFront_RCtSaInteriorLightSwitchLight); /* PRQA S 3417 */ /* MD_Rte_Os */
    Rte_Vtt_Task_WaitEventRet(IO_Task, Rte_Ev_Cyclic_IO_Task_0_200ms | Rte_Ev_Run_CpSaDisplay_RCtApDisplayBusStateHandling | Rte_Ev_Run_CpSaDisplay_RCtApDisplayLightOnOffHandling | Rte_Ev_Run_CpSaDisplay_RCtApDisplayOdometerValue | Rte_Ev_Run_CpSaDisplay_RCtApDisplayOdometerWriteRequestPending | Rte_Ev_Run_CpSaInteriorLightFront_RCtSaInteriorLightSwitchLight); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    (void)GetEvent(IO_Task, &ev); /* PRQA S 3417 */ /* MD_Rte_Os */
    (void)ClearEvent(ev & (Rte_Ev_Cyclic_IO_Task_0_200ms | Rte_Ev_Run_CpSaDisplay_RCtApDisplayBusStateHandling | Rte_Ev_Run_CpSaDisplay_RCtApDisplayLightOnOffHandling | Rte_Ev_Run_CpSaDisplay_RCtApDisplayOdometerValue | Rte_Ev_Run_CpSaDisplay_RCtApDisplayOdometerWriteRequestPending | Rte_Ev_Run_CpSaInteriorLightFront_RCtSaInteriorLightSwitchLight)); /* PRQA S 3417 */ /* MD_Rte_Os */

    if ((ev & Rte_Ev_Cyclic_IO_Task_0_200ms) != (EventMaskType)0)
    {
      /* call runnable */
      RCtApDisplayReadIgnitionSignal();

      /* call runnable */
      RCtSaDoorReadDoor(&Rte_Instance_CpSaDoorFrontRight);

      /* call runnable */
      RCtSaDoorReadDoor(&Rte_Instance_CpSaDoorFrontLeft);
    }

    if ((ev & Rte_Ev_Run_CpSaDisplay_RCtApDisplayBusStateHandling) != (EventMaskType)0)
    {
      /* call runnable */
      RCtApDisplayBusStateHandling();
    }

    if ((ev & Rte_Ev_Run_CpSaDisplay_RCtApDisplayLightOnOffHandling) != (EventMaskType)0)
    {
      /* call runnable */
      RCtApDisplayLightOnOffHandling();
    }

    if ((ev & Rte_Ev_Run_CpSaDisplay_RCtApDisplayOdometerValue) != (EventMaskType)0)
    {
      /* call runnable */
      RCtApDisplayOdometerValue();
    }

    if ((ev & Rte_Ev_Run_CpSaDisplay_RCtApDisplayOdometerWriteRequestPending) != (EventMaskType)0)
    {
      /* call runnable */
      RCtApDisplayOdometerWriteRequestPending();
    }

    if ((ev & Rte_Ev_Run_CpSaInteriorLightFront_RCtSaInteriorLightSwitchLight) != (EventMaskType)0)
    {
      /* call runnable */
      RCtSaInteriorLightSwitchLight((uint8)0);
    }
  }
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Task:     My_Task
 * Priority: 3
 * Schedule: NON
 *********************************************************************************************************************/
TASK(My_Task) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  EventMaskType ev;


  /* call runnable */
  RCtApMySwcInit();

  for(;;)
  {
    (void)WaitEvent(Rte_Ev_Run1_CpApMySwc_RCtApMySwcPostRunCode | Rte_Ev_Run_CpApMySwc_RCtApMySwcCode | Rte_Ev_Run_CpApMySwc_RCtApMySwcComM_ModeChange_FULL_COMM_Entry | Rte_Ev_Run_CpApMySwc_RCtApMySwcComM_ModeChange_FULL_COMM_Exit | Rte_Ev_Run_CpApMySwc_RCtApMySwcPostRunCode); /* PRQA S 3417 */ /* MD_Rte_Os */
    Rte_Vtt_Task_WaitEventRet(My_Task, Rte_Ev_Run1_CpApMySwc_RCtApMySwcPostRunCode | Rte_Ev_Run_CpApMySwc_RCtApMySwcCode | Rte_Ev_Run_CpApMySwc_RCtApMySwcComM_ModeChange_FULL_COMM_Entry | Rte_Ev_Run_CpApMySwc_RCtApMySwcComM_ModeChange_FULL_COMM_Exit | Rte_Ev_Run_CpApMySwc_RCtApMySwcPostRunCode); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    (void)GetEvent(My_Task, &ev); /* PRQA S 3417 */ /* MD_Rte_Os */
    (void)ClearEvent(ev & (Rte_Ev_Run1_CpApMySwc_RCtApMySwcPostRunCode | Rte_Ev_Run_CpApMySwc_RCtApMySwcCode | Rte_Ev_Run_CpApMySwc_RCtApMySwcComM_ModeChange_FULL_COMM_Entry | Rte_Ev_Run_CpApMySwc_RCtApMySwcComM_ModeChange_FULL_COMM_Exit | Rte_Ev_Run_CpApMySwc_RCtApMySwcPostRunCode)); /* PRQA S 3417 */ /* MD_Rte_Os */

    if ((ev & Rte_Ev_Run_CpApMySwc_RCtApMySwcCode) != (EventMaskType)0)
    {
      /* read implicit data */
      (void)Com_ReceiveSignal(ComConf_ComSignal_sig_State_RearLeftDoor_2b456e3f, (&Rte_My_Task.Rte_RB.Rte_CtApMySwc_RCtApMySwcCode.Rte_PpDoorStateRearLeft_DeDoorState.value)); /* PRQA S 3453 */ /* MD_MSR_19.7 */

      /* call runnable */
      RCtApMySwcCode();
    }

    if ((ev & Rte_Ev_Run_CpApMySwc_RCtApMySwcComM_ModeChange_FULL_COMM_Exit) != (EventMaskType)0)
    {
      /* call runnable */
      RCtApMySwcComM_ModeChange_FULL_COMM_Exit();
    }

    if ((ev & Rte_Ev_Run_CpApMySwc_RCtApMySwcComM_ModeChange_FULL_COMM_Entry) != (EventMaskType)0)
    {
      /* call runnable */
      RCtApMySwcComM_ModeChange_FULL_COMM_Entry();
    }

    if (((ev & Rte_Ev_Run1_CpApMySwc_RCtApMySwcPostRunCode) != (EventMaskType)0) || ((ev & Rte_Ev_Run_CpApMySwc_RCtApMySwcPostRunCode) != (EventMaskType)0))
    {
      /* call runnable */
      RCtApMySwcPostRunCode();
    }

    if (((ev & Rte_Ev_Run1_CpApMySwc_RCtApMySwcPostRunCode) != (EventMaskType)0) && (Rte_ModeMachine_BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode == RTE_TRANSITION_ESH_Mode))
    {

      Rte_DisableAllInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      Rte_ModeMachine_BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode = Rte_ModeInfo_BswM_Switch_ESH_ModeSwitch_BswM_MDGP_ESH_Mode.Rte_ModeQueue[0];

      Rte_EnableAllInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */


    }

    if (((ev & (Rte_Ev_Run_CpApMySwc_RCtApMySwcComM_ModeChange_FULL_COMM_Entry | Rte_Ev_Run_CpApMySwc_RCtApMySwcComM_ModeChange_FULL_COMM_Exit)) != (EventMaskType)0) && (Rte_ModeMachine_ComM_UM_USR_INTERIOR_LIGHT_currentMode == RTE_TRANSITION_ComMMode))
    {

      Rte_DisableAllInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      Rte_ModeMachine_ComM_UM_USR_INTERIOR_LIGHT_currentMode = Rte_ModeInfo_ComM_UM_USR_INTERIOR_LIGHT_currentMode.Rte_ModeQueue[0];

      Rte_EnableAllInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */


    }
  }
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Task:     SchM_Task
 * Priority: 6
 * Schedule: FULL
 *********************************************************************************************************************/
TASK(SchM_Task) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  EventMaskType ev;

  for(;;)
  {
    (void)WaitEvent(Rte_Ev_Cyclic2_SchM_Task_0_10ms | Rte_Ev_Cyclic2_SchM_Task_0_20ms | Rte_Ev_Cyclic2_SchM_Task_0_5ms); /* PRQA S 3417 */ /* MD_Rte_Os */
    Rte_Vtt_Task_WaitEventRet(SchM_Task, Rte_Ev_Cyclic2_SchM_Task_0_10ms | Rte_Ev_Cyclic2_SchM_Task_0_20ms | Rte_Ev_Cyclic2_SchM_Task_0_5ms); /* PRQA S 3112 */ /* MD_MSR_14.2 */
    (void)GetEvent(SchM_Task, &ev); /* PRQA S 3417 */ /* MD_Rte_Os */
    (void)ClearEvent(ev & (Rte_Ev_Cyclic2_SchM_Task_0_10ms | Rte_Ev_Cyclic2_SchM_Task_0_20ms | Rte_Ev_Cyclic2_SchM_Task_0_5ms)); /* PRQA S 3417 */ /* MD_Rte_Os */

    if ((ev & Rte_Ev_Cyclic2_SchM_Task_0_10ms) != (EventMaskType)0)
    {
      /* call runnable */
      NvM_MainFunction();
    }

    if ((ev & Rte_Ev_Cyclic2_SchM_Task_0_5ms) != (EventMaskType)0)
    {
      /* call schedulable entity */
      Fee_MainFunction();
    }

    if ((ev & Rte_Ev_Cyclic2_SchM_Task_0_10ms) != (EventMaskType)0)
    {
      /* call schedulable entity */
      Fls_MainFunction();

      /* call schedulable entity */
      Can_MainFunction_BusOff();

      /* call schedulable entity */
      Can_MainFunction_Mode();

      /* call schedulable entity */
      Can_MainFunction_Wakeup();

      /* call schedulable entity */
      CanNm_MainFunction();

      /* call schedulable entity */
      CanSM_MainFunction();

      /* call schedulable entity */
      Com_MainFunctionRx();

      /* call runnable */
      BswM_MainFunction();

      /* call schedulable entity */
      Com_MainFunctionTx();
    }

    if ((ev & Rte_Ev_Cyclic2_SchM_Task_0_20ms) != (EventMaskType)0)
    {
      /* call runnable */
      ComM_MainFunction_0();
    }

    if ((ev & Rte_Ev_Cyclic2_SchM_Task_0_10ms) != (EventMaskType)0)
    {
      /* call runnable */
      Dem_MasterMainFunction();

      /* call runnable */
      Dem_SatelliteMainFunction();

      /* call runnable */
      EcuM_MainFunction();
    }

    if ((ev & Rte_Ev_Cyclic2_SchM_Task_0_5ms) != (EventMaskType)0)
    {
      /* call schedulable entity */
      CanTp_MainFunction();
    }

    if ((ev & Rte_Ev_Cyclic2_SchM_Task_0_10ms) != (EventMaskType)0)
    {
      /* call runnable */
      Dcm_MainFunction();
    }
  }
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

#define RTE_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 MISRA 2004 violations and justifications
 *********************************************************************************************************************/

/* module specific MISRA deviations:
   MD_Rte_0781:  MISRA rule: 5.6
     Reason:     The name is being used as a structure/union member as well as being a label, tag or ordinary identifier.
                 The compliance to this rule is under user's control.
     Risk:       Ambiguous identifiers can lead to compiler errors / warnings.
     Prevention: Verified during compile time. If the compiler reports an error or warning, the user has to rename the objects leading to the violation.

   MD_Rte_3197:  MISRA rule: -
     Reason:     The variable is not written by the code in all possible code variants. Due to this, the variable is initialized.
     Risk:       No functional risk.
     Prevention: Not required.

   MD_Rte_3206:  MISRA rule: -
     Reason:     The parameter's are not used by the code in all possible code variants.
     Risk:       No functional risk.
     Prevention: Not required.

   MD_Rte_3408:  MISRA rule: 8.8
     Reason:     For the purpose of monitoring during calibration or debugging it is necessary to use non-static declarations.
                 This is covered in the MISRA C compliance section of the Rte specification.
     Risk:       No functional risk.
     Prevention: Not required.

   MD_Rte_3447:  MISRA rule: 8.8
     Reason:     For the purpose of monitoring during calibration or debugging it is necessary to use non-static declarations.
                 This is covered in the MISRA C compliance section of the Rte specification.
     Risk:       No functional risk.
     Prevention: Not required.

   MD_Rte_Os:
     Reason:     This justification is used as summary justification for all deviations caused by the MICROSAR OS
                 which is for testing of the RTE. Those deviations are no issues in the RTE code.
     Risk:       No functional risk.
     Prevention: Not required.

   MD_Rte_Qac:
     Reason:     This justification is used as summary justification for all deviations caused by wrong analysis tool results.
                 The used analysis tool QAC 7.0 sometimes creates wrong messages. Those deviations are no issues in the RTE code.
     Risk:       No functional risk.
     Prevention: Not required.

*/
