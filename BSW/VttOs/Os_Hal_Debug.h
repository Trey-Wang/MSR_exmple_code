/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 * \ingroup    CmpHal
 * \addtogroup CmpHalCore HAL Core
 * \brief      Core and multi-core related primitives.
 * \details
 * Some of the defined types shall be mapped to the word type. Therefore it is recommended to
 * create a type Os_Hal_WordType which is typedef-ed to an unsigned word.
 *
 * \{
 * \file
 * \brief   Core and multi-core related primitives.
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Johannes Thull                visjth        Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2014-10-17  visjth  -             Initial version
 **********************************************************************************************************************/

#if !defined(OS_HAL_DEBUG_H)
#define OS_HAL_DEBUG_H

#include "Os_Hal_Vtt_Cfg.h"

# if (OS_CFG_HAL_DEBUG_VTT == STD_ON)

#define Os_Hal_Debug_ContextInit_Entry()                                  CANoeAPI_Printf("Init Context %s (Canoe task ID: 0x%02X, Function: 0x%p)",Config->Name, Config->CANoeEmuTaskId, Config->Entry);
#define Os_Hal_Debug_ContextInit_Exit()
#define Os_Hal_Debug_CoreInit_Entry()                                     CANoeAPI_Printf("Init Core %s: Number Of CANoeEmu Tasks: %d [running on core %d]", Config->HalConfig->Name, Config->NumberOfCANoeEmuTasks, CANoeEmuProcessor_GetCoreNumber());
#define Os_Hal_Debug_CoreInitMaster_Entry()
#define Os_Hal_Debug_InitInterrupt_Entry()                  CANoeAPI_Printf("Init ISR %s [source=%d, level=%d, routingmask=%08x]", Config->Name, Config->Source, Config->Level, Config->RoutingMask);
#define Os_Hal_Debug_InitHook_Entry()                       CANoeAPI_Printf("Init Hook %s ", Config->Thread.ContextConfig->Name);


#define Os_Hal_Debug_ContextReset_Entry()                                   CANoeAPI_Printf("Context Reset: %s (Int level: %d)", Config->Name, Config->IntLockLevel);
#define Os_Hal_Debug_ContextSetInterruptState_Entry()                       CANoeAPI_Printf("Context Set Interrupt State (level: %d)", *InterruptState);
#define Os_Hal_Debug_ContextFirstResume_Entry()                             CANoeAPI_Printf("Context First Resume: %s (Int level: %d)", Next->Config->Name, Next->InterruptState);
#define Os_Hal_Debug_ContextSwitch_Entry()                                  CANoeAPI_Printf("Context Switch: %s (Int level: %d)", Next->Config->Name, Next->InterruptState);
#define Os_Hal_Debug_ContextSwitch_Exit()
#define Os_Hal_Debug_ContextResetAndResume_Entry()                          CANoeAPI_Printf("Context Reset and Resume: %s (Int level: %d)", Next->Config->Name, Next->InterruptState);
#define Os_Hal_Debug_ContextResetAndResume_BeforeRollbackStack()
#define Os_Hal_Debug_ContextSuspendAndStart_Entry()                         CANoeAPI_Printf("Context Suspend and Start: %s (Int level: %d)", NextConfig->Name, NextConfig->IntLockLevel);
#define Os_Hal_Debug_ContextSuspendAndStart_Exit()
#define Os_Hal_Debug_ContextCleanupAndResume_Entry()                        CANoeAPI_Printf("Context Cleanup and Resume: %s -> %s (Int level: %d)",CurrentConfig->Name, Next->Config->Name, Next->InterruptState);
#define Os_Hal_Debug_ContextCleanupAndResume_BeforeRollbackStack()
#define Os_Hal_Debug_ContextAbort_Entry()                                    CANoeAPI_Printf("Context Abort: %s",Config->Name);
#define Os_Hal_Debug_ContextReturn_Entry()                                   CANoeAPI_Printf("Context Return: from %s", Current->Config->Name);
#define Os_Hal_Debug_ContextCall_Entry()                                     CANoeAPI_Printf("Context Call: %s",  Next->Config->Name);
#define Os_Hal_Debug_ContextCallOnStack_Entry()                              CANoeAPI_Printf("Context Call on Stack: %s (level: %d)",  Next->Config->Name, Next->InterruptState);


#define Os_Hal_Debug_CoreStart_Entry()
#define Os_Hal_Debug_CoreEmulateMaster_Entry()
#define Os_Hal_Debug_CoreEmulateSlave_Entry()
#define Os_Hal_Debug_XSigTrigger_Entry()
#define Os_Hal_Debug_CoreFreeze_Entry()
#define Os_Hal_Debug_CanoeIrqHandler_Entry(x)                                 CANoeAPI_Printf("<<< INT ENTRY: %s >>>", x);
#define Os_Hal_Debug_CanoeIrqHandler_Epilogue(x)                              CANoeAPI_Printf("Int epilogue: %s", x);

#define Os_Hal_Debug_TimerPitInit_Entry()
#define Os_Hal_Debug_TimerFrtInit_Entry()
#define Os_Hal_Debug_TimerFrtGetCounterValue_Entry()
#define Os_Hal_Debug_TimerFrtGetCounterValue_Exit()
#define Os_Hal_Debug_TimerFrtGetCompareValue_Entry()
#define Os_Hal_Debug_TimerFrtSetCompareValue_Entry()         CANoeAPI_Printf("Set Compare Value: %d (Counter value: %d)", ExpirationTime,   CANoeEmuHRTimer_GetCounterValue(TimerConfig->CANoeHrtId));
#define Os_Hal_Debug_TimerFrtTriggerIrq_Entry()

#define Os_Hal_Debug_IntGetGlobalState_Exit()               CANoeAPI_Printf("Int get global state (level: %d)", *State);
#define Os_Hal_Debug_IntSetGlobalState_Entry()              CANoeAPI_Printf("Int set global state (level: %d --> %d)", *State, CANoeEmuProcessor_GetInterruptLevel());
#define Os_Hal_Debug_IntSetLevel_Entry()                    CANoeAPI_Printf("Int set level (level: %d)", *Level);
#define Os_Hal_Debug_IntGetLevel_Exit()                     CANoeAPI_Printf("Int get level (level: %d)", *Level);

#define Os_Hal_Debug_IntSuspendTp_Exit()                    CANoeAPI_Printf("Int Suspend TP (level: %d --> %d)", *State, CANoeEmuProcessor_GetInterruptLevel());
#define Os_Hal_Debug_IntResumeTp_Entry()                    CANoeAPI_Printf("Int Resume  TP (level: -> %d)", *State);

#define Os_Hal_Debug_IntTrapCallerSave_Exit()                CANoeAPI_Printf("Int Trap Caller Save (level: %d)", *State);
#define Os_Hal_Debug_IntTrapCallerRestore_Entry()            CANoeAPI_Printf("Int Trap Caller Restore (level: %d)", *State);

#define Os_Hal_Debug_IntDisable_Entry()                     CANoeAPI_Printf("Int Disable");
#define Os_Hal_Debug_IntEnable_Entry()                      CANoeAPI_Printf("Int Enable");
#define Os_Hal_Debug_DisableSource_Entry()                  CANoeAPI_Printf("Int Disable Source %d: '%s' ", IsrConfig->Source, IsrConfig->Name);
#define Os_Hal_Debug_EnableSource_Entry()                   CANoeAPI_Printf("Int Enable Source %d: '%s' ", IsrConfig->Source, IsrConfig->Name);

#define Os_Hal_Debug_IntGetInterruptState_Exit()            CANoeAPI_Printf("Int Get Interrupt State (level: %d)", *InterruptState);
#define Os_Hal_Debug_IntSetInterruptState_Entry()           CANoeAPI_Printf("Int Set Interrupt State (level: %d)", *InterruptState);


#define Os_Hal_Debug_Trap_Entry()                     CANoeAPI_Printf("Trap (Id: %d)", Parameters->TrapId);

#define Os_Hal_Debug_CoreIdle_Entry()                 CANoeAPI_Printf("Core Idle (curr. level: %d curr. flag: %d)",CANoeEmuProcessor_GetInterruptLevel(),CANoeEmuProcessor_InterruptsEnabled());

# else

#define Os_Hal_Debug_ContextInit_Entry()
#define Os_Hal_Debug_ContextInit_Exit()
#define Os_Hal_Debug_CoreInit_Entry()
#define Os_Hal_Debug_CoreInitMaster_Entry()
#define Os_Hal_Debug_InitInterrupt_Entry()
#define Os_Hal_Debug_InitHook_Entry()


#define Os_Hal_Debug_ContextReset_Entry()
#define Os_Hal_Debug_ContextSetInterruptState_Entry()
#define Os_Hal_Debug_ContextFirstResume_Entry()
#define Os_Hal_Debug_ContextSwitch_Entry()
#define Os_Hal_Debug_ContextSwitch_Exit()
#define Os_Hal_Debug_ContextResetAndResume_Entry()
#define Os_Hal_Debug_ContextResetAndResume_BeforeRollbackStack()
#define Os_Hal_Debug_ContextSuspendAndStart_Entry()
#define Os_Hal_Debug_ContextSuspendAndStart_Exit()
#define Os_Hal_Debug_ContextCleanupAndResume_Entry()
#define Os_Hal_Debug_ContextCleanupAndResume_BeforeRollbackStack()
#define Os_Hal_Debug_ContextAbort_Entry()
#define Os_Hal_Debug_ContextServiceCall_Entry()
#define Os_Hal_Debug_ContextServiceReturn_Entry()
#define Os_Hal_Debug_ContextServiceTpReturn_Entry()
#define Os_Hal_Debug_ContextReturn_Entry()
#define Os_Hal_Debug_ContextCall_Entry()
#define Os_Hal_Debug_ContextCallOnStack_Entry()


#define Os_Hal_Debug_CoreStart_Entry()
#define Os_Hal_Debug_CoreEmulateMaster_Entry()
#define Os_Hal_Debug_CoreEmulateSlave_Entry()
#define Os_Hal_Debug_XSigTrigger_Entry()
#define Os_Hal_Debug_CoreIdle_Entry()
#define Os_Hal_Debug_CoreFreeze_Entry()
#define Os_Hal_Debug_CanoeIrqHandler_Entry(x)
#define Os_Hal_Debug_CanoeIrqHandler_Epilogue(x)

#define Os_Hal_Debug_TimerPitInit_Entry()
#define Os_Hal_Debug_TimerFrtInit_Entry()
#define Os_Hal_Debug_TimerFrtGetCounterValue_Entry()
#define Os_Hal_Debug_TimerFrtGetCounterValue_Exit()
#define Os_Hal_Debug_TimerFrtGetCompareValue_Entry()
#define Os_Hal_Debug_TimerFrtSetCompareValue_Entry()
#define Os_Hal_Debug_TimerFrtTriggerIrq_Entry()

#define Os_Hal_Debug_IntGetGlobalState_Exit()
#define Os_Hal_Debug_IntSetGlobalState_Entry()
#define Os_Hal_Debug_IntSetLevel_Entry()
#define Os_Hal_Debug_IntGetInterruptState_Entry()
#define Os_Hal_Debug_IntGetLevel_Exit()

#define Os_Hal_Debug_IntTrapCallerSave_Exit()
#define Os_Hal_Debug_IntTrapCallerRestore_Entry()

#define Os_Hal_Debug_IntDisable_Entry()
#define Os_Hal_Debug_IntEnable_Entry()
#define Os_Hal_Debug_DisableSource_Entry()
#define Os_Hal_Debug_EnableSource_Entry()

#define Os_Hal_Debug_IntGetInterruptState_Exit()
#define Os_Hal_Debug_IntSetInterruptState_Entry()


#define Os_Hal_Debug_Trap_Entry()

#define Os_Hal_Debug_CoreIdle_Entry()

#endif

#endif








