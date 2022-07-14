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
 *  \addtogroup Os_Hal_Context
 *  \trace      CREQ-1201
 *
 *  \{
 *
 *  \file
 *  \brief      Context switching primitives.
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#if !defined (OS_HAL_CONTEXT_H)                                                                                         /* PRQA S 0883 */ /* MD_Os_Hal_0883 */
# define OS_HAL_CONTEXT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module dependencies */
# include "Os_Hal_ContextInt.h"
# include "Os_Hal_Context_Lcfg.h"

/* Os kernel module dependencies */

/* Os Hal dependencies */
# include "Os_Hal_Os.h"
# include "Os_Hal_Compiler.h"
# include "Os_Hal_Interrupt.h"
# include "Os_Hal_Core.h"
# include "Os_Hal_Debug.h"

#if !defined(OS_VCAST_INSTRUMENTATION_OS_HAL)
/*VCAST_DONT_INSTRUMENT_START*/
#endif

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  Os_Hal_ContextInit()
 **********************************************************************************************************************/
typedef void (*voidFuncPtr)(void);
extern void Os_Hal_CANoeTaskWrapper(void);
extern void Os_Hal_TaskEndHook(void);
extern void Os_Hal_ConsumeStack(P2CONST(Os_StackConfigType, AUTOMATIC, OS_CONST) stack);

OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_ContextInit,
(
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) Config,
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context
))
{
  sint32 taskId;

  Os_Hal_Debug_ContextInit_Entry();

  taskId = Config->CANoeEmuTaskId;
  Context->Config = Config;
  Context->InterruptState = Config->IntLockLevel;
  Context->IsAborted = FALSE;

  CANoeEmuProcessor_ConfigureTaskName(taskId, (char*)Config->Name);
  CANoeEmuProcessor_ConfigureTaskEndHook((voidFuncPtr)&Os_TaskMissingTerminateTask);
  CANoeEmuProcessor_ConfigureTaskFunction(taskId, (void (*)(void))Config->CANoeTaskFunction);
}


/***********************************************************************************************************************
 *  Os_Hal_ContextReset()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_ContextReset,
(
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) Config,
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context
))
{
  Os_Hal_Debug_ContextReset_Entry();

  Context->InterruptState = Config->IntLockLevel;

  if(CANoeEmuProcessor_RollbackStackOfTask(Config->CANoeEmuTaskId) != CANOEEMUPROCESSOR_ROLLBACK)
  {
     Os_Hal_CoreFreeze();
  }
}

/***********************************************************************************************************************
 *  Os_Hal_ContextSetParameter()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_ContextSetParameter,
(
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context,
  P2CONST(void, AUTOMATIC, OS_VAR_NOINIT) Parameter
))
{
  Context->Parameter = Parameter;
}


/***********************************************************************************************************************
 *  Os_Hal_ContextSetStack()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_ContextSetStack,
(
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context,
  P2CONST(Os_Hal_ContextStackConfigType, AUTOMATIC, OS_VAR_NOINIT) Stack
))
{
  OS_IGNORE_UNREF_PARAM(Context);                                                                                       /* PRQA S 3112 */ /* MD_Os_Hal_3112 */
  OS_IGNORE_UNREF_PARAM(Stack);                                                                                         /* PRQA S 3112 */ /* MD_Os_Hal_3112 */  
}


/***********************************************************************************************************************
 *  Os_Hal_ContextGetStack()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Hal_3219 */
Os_Hal_ContextGetStack,
(
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) Context,
  P2VAR(Os_Hal_ContextStackConfigType, AUTOMATIC, OS_VAR_NOINIT) Stack
))
{
  OS_IGNORE_UNREF_PARAM(Context);                                                                                       /* PRQA S 3112 */ /* MD_Os_Hal_3112 */
  OS_IGNORE_UNREF_PARAM(Stack);                                                                                         /* PRQA S 3112 */ /* MD_Os_Hal_3112 */  
}


/***********************************************************************************************************************
 *  Os_Hal_ContextSetInterruptState()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_ContextSetInterruptState,
(
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context,
  P2CONST(Os_Hal_IntStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  Os_Hal_Debug_ContextSetInterruptState_Entry();

  Context->InterruptState = *InterruptState;
}


/***********************************************************************************************************************
 *  Os_Hal_ContextFirstResume()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_ContextFirstResume,
(
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Next
))
{
  Os_Hal_Debug_ContextFirstResume_Entry();

  CANoeEmuProcessor_DisableInterrupts();
  Os_Hal_IntSetLevel(&(Next->InterruptState));
  Os_Hal_ConsumeStack(Next->Config->Stack);
  CANoeEmuProcessor_SetTaskToRunAndEnableInterrupts(Next->Config->CANoeEmuTaskId);

  CANoeEmuProcessor_RollbackStack();
}


/***********************************************************************************************************************
 *  Os_Hal_ContextSwitch()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_ContextSwitch,
(
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Current,
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Next
))
{
  Os_Hal_Debug_ContextSwitch_Entry();

  CANoeEmuProcessor_DisableInterrupts();
  Os_Hal_IntGetLevel(&(Current->InterruptState));

  CANoeEmuProcessor_SetInterruptLevel(Next->Config->IntLockLevel);
  Os_Hal_ConsumeStack(Next->Config->Stack);

  CANoeEmuProcessor_SetTaskToRunAndEnableInterrupts(Next->Config->CANoeEmuTaskId);

  Os_Hal_Debug_ContextSwitch_Exit();
}


/***********************************************************************************************************************
 *  Os_Hal_ContextResetAndResume()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_ContextResetAndResume,
(
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) CurrentConfig,
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Current,
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Next
))
{
  Os_Hal_Debug_ContextResetAndResume_Entry();

  CANoeEmuProcessor_DisableInterrupts();
  Current->InterruptState = CurrentConfig->IntLockLevel;

  //Os_Hal_IntSetInterruptState(&(Next->InterruptState));
  CANoeEmuProcessor_SetInterruptLevel(Next->Config->IntLockLevel);
  Os_Hal_ConsumeStack(Next->Config->Stack);
  CANoeEmuProcessor_SetTaskToRunAndEnableInterrupts(Next->Config->CANoeEmuTaskId);

  Os_Hal_Debug_ContextResetAndResume_BeforeRollbackStack();

  CANoeEmuProcessor_RollbackStack();
  Os_Hal_CoreFreeze();
}





/***********************************************************************************************************************
 *  Os_Hal_ContextAbort()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_ContextAbort,
(
  P2CONST(Os_Hal_ContextConfigType, AUTOMATIC, OS_CONST) Config,
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Context
))
{
  Os_Hal_Debug_ContextAbort_Entry();

  Context->InterruptState = Config->IntLockLevel;
  Context->IsAborted = TRUE;
}


/***********************************************************************************************************************
 *  Os_Hal_ContextCall()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_ContextCall,
(
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Current,
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Next
))
{
  Os_Hal_Debug_ContextCall_Entry();

  Os_Hal_IntGetLevel(&(Current->InterruptState));
  Os_Hal_IntSetLevel(&(Next->InterruptState));

  if(!setjmp(((Os_Hal_ContextType*)Current)->JmpBuf))
  {
    Next->Config->Entry(Next->Parameter);
  }

  CANoeEmuProcessor_EnableInterrupts();
}


/***********************************************************************************************************************
 *  Os_Hal_ContextCallOnStack()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_ContextCallOnStack,
(
  P2VAR(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Current,
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Next
))
{
  Os_Hal_Debug_ContextCallOnStack_Entry();

  Os_Hal_IntGetLevel(&(Current->InterruptState));
  Os_Hal_IntSetLevel(&(Next->InterruptState));

  if((Next->Config->Type == Isr) || (Next->Config->Type == Hook))
  {
    Os_Hal_ConsumeStack(Next->Config->Stack);
  }

  if(Next->Config->Type == Isr)
  {
    CANoeEmuProcessor_EnableInterrupts();

    if(!setjmp(((Os_Hal_ContextType*)Next)->JmpBuf))
    {
       Next->Config->Entry(Next->Parameter);
       Os_Hal_Debug_CanoeIrqHandler_Epilogue(Next->Config->Name);
       Next->Config->ReturnAddress();        /* Call the return function (ISR epilogue) and come back on Os_Hal_ContextReturn (called by ISR epilogue)*/
    }

  }
  else if(Next->Config->Type == NonTrustedFunction)
  {
    if(!setjmp(((Os_Hal_ContextType*)Next)->JmpBuf))
    {
      Next->Config->Entry(Next->Parameter);
    }
    CANoeEmuProcessor_EnableInterrupts();
  }
  else
  {
    CANoeEmuProcessor_RollbackStackOfTask(Next->Config->CANoeEmuTaskId);
    CANoeEmuProcessor_SetTaskToRunAndEnableInterrupts(Next->Config->CANoeEmuTaskId);
  }
}



/***********************************************************************************************************************
 *  Os_Hal_ContextReturn()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_ContextReturn,
(
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Current,
  P2CONST(Os_Hal_ContextType, AUTOMATIC, OS_VAR_NOINIT) Next
))
{
  Os_Hal_Debug_ContextReturn_Entry();

  /* Reset interrupt state of current context */
  ((Os_Hal_ContextType*)Current)->InterruptState = Current->Config->IntLockLevel;

  CANoeEmuProcessor_DisableInterrupts();
  Os_Hal_IntSetLevel(&(Next->InterruptState));

  if((Current->Config->Type == Isr) || (Current->Config->Type == NonTrustedFunction))
  {
    if(Current->IsAborted == TRUE)
    {
      ((Os_Hal_ContextType*)Current)->IsAborted = FALSE;
      CANoeEmuProcessor_SetInterruptLevel(Next->Config->IntLockLevel);
      CANoeEmuProcessor_SetTaskToRunAndEnableInterrupts(Next->Config->CANoeEmuTaskId);
      CANoeEmuProcessor_RollbackStack();
      Os_Hal_CoreFreeze();
    }

    if(Next->IsAborted == TRUE)
    {
      CANoeEmuProcessor_RollbackStackOfTask(Next->Config->CANoeEmuTaskId);
      Next->Config->ReturnAddress();
      Os_Hal_CoreFreeze();
    }

    /* Function longjmp never returns. */
    longjmp((int*)Current->JmpBuf,1);
  }
  else if(Current->Config->Type == TrustedFunction)
  {
    /* Function longjmp never returns. */
    longjmp((int*)Next->JmpBuf,1);
  }
  CANoeEmuProcessor_SetTaskToRunAndEnableInterrupts(Next->Config->CANoeEmuTaskId);

  Os_Hal_CoreFreeze();
}


/***********************************************************************************************************************
 *  Os_Hal_ContextIsExceptionContextSupported()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_Hal_3219 */
    Os_Hal_ContextIsExceptionContextSupported,( void ))
{
  return 0;
}


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if !defined(OS_VCAST_INSTRUMENTATION_OS_HAL)
/*VCAST_DONT_INSTRUMENT_END*/
#endif

#endif /* OS_HAL_CONTEXT_H */

/* module specific MISRA deviations:
 */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Context.h
 **********************************************************************************************************************/

