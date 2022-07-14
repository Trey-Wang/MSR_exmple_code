/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**
 * \addtogroup Os_Interrupt
 * \{
 * \file
 * \brief       This interface provides the AUTOSAR OS interrupt API and Vector extensions.
 * \details     --no details--
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os.h.
 **********************************************************************************************************************/

#if !defined (OS_INTERRUPT_H)                                                                                           /* PRQA S 0883 */ /* MD_Os_0883 */
# define OS_INTERRUPT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module dependencies */
# include "Os_Interrupt_Types.h"
# include "Os_InterruptInt.h"

/* Os kernel module dependencies */
# include "Os_Common.h"
# include "Os_Error.h"
# include "Os_Trace.h"
# include "Os_TimingProtection.h"

/* Os Hal dependencies */
# include "Os_Hal_Compiler.h"
# include "Os_Hal_Interrupt.h"

#if !defined(OS_VCAST_INSTRUMENTATION_OS_INTERRUPT)
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
 *  GLOBAL FUNCTION
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Os_IntDisableGlobal()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_3219 */
Os_IntDisableGlobal, (void))                                                                                            /* COV_OS_HALPLATFORMGLOBAL2LEVELSUPPORTED */
{
  Os_Hal_IntGlobalStateType globalState;

  /* #10 Get the state to disable all interrupts global. */
  Os_Hal_IntGetGlobalDisable(&globalState);                                                                             /* SBSW_OS_FC_POINTER2LOCAL */

  /* #20 Disable interrupt handling. */
  Os_Hal_IntSetGlobalState(&globalState);                                                                               /* SBSW_OS_FC_POINTER2LOCAL */
}

/***********************************************************************************************************************
 *  Os_IntEnableGlobal()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_3219 */
Os_IntEnableGlobal, (void))                                                                                             /* COV_OS_HALPLATFORMGLOBAL2LEVELSUPPORTED */
{
  Os_Hal_IntGlobalStateType globalState;

  /* #10 Get the state to enable all interrupts global. */
  Os_Hal_IntGetGlobalEnable(&globalState);                                                                              /* SBSW_OS_FC_POINTER2LOCAL */

  /* #20 Enable interrupt handling. */
  Os_Hal_IntSetGlobalState(&globalState);                                                                               /* SBSW_OS_FC_POINTER2LOCAL */
}

/***********************************************************************************************************************
 *  Os_IntSuspend()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_3219 */
Os_IntSuspend,
(
  P2VAR(Os_IntStateType, AUTOMATIC, AUTOMATIC) State
))
{
  Os_Hal_IntLevelType level;

  /* #10 Get the level to suspend all interrupts including timing protection interrupt. */
  Os_Hal_IntGetTPLockLevel(&level);                                                                                     /* SBSW_OS_FC_POINTER2LOCAL */

  /* #20 Get the current level, which should later be restored to resume interrupt handling. */
  Os_Hal_IntGetLevel(&(State->Level));                                                                                  /* SBSW_OS_FC_PRECONDITION */

  /* #30 Change to interrupt level. */
  Os_Hal_IntSetLevel(&level);                                                                                           /* SBSW_OS_FC_POINTER2LOCAL */
}

/***********************************************************************************************************************
 *  Os_IntResume()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_3219 */
Os_IntResume,
(
  P2CONST(Os_IntStateType, AUTOMATIC, AUTOMATIC) State
))
{
  Os_Hal_IntSetLevel(&(State->Level));                                                                                  /* SBSW_OS_FC_PRECONDITION */
}

/***********************************************************************************************************************
 *  Os_IntDisable()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_3219 */
Os_IntDisable,
(
  void
))
{
  Os_IntStateType state;

  /* #10 Call suspend behavior and forget current state. */
  Os_IntSuspend(&state);                                                                                                /* SBSW_OS_FC_POINTER2LOCAL */
}

/***********************************************************************************************************************
 *  Os_IntSuspendLevel()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_3219 */
Os_IntSuspendLevel,
(
  P2VAR(Os_IntLevelType, AUTOMATIC, AUTOMATIC) Level
))
{
  Os_Hal_IntLevelType level;

  /* #10 Temporary store the new level. */
  level = Level->State;

  /* #20 Call HAL to return current interrupt level. */
  Os_Hal_IntGetLevel(&(Level->State));                                                                                  /* SBSW_OS_FC_PRECONDITION */

  /* #30 Change to interrupt level. */
  Os_Hal_IntSetLevel(&level);                                                                                           /* SBSW_OS_FC_POINTER2LOCAL */
}

/***********************************************************************************************************************
 *  Os_IntResumeLevel()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_3219 */
Os_IntResumeLevel,
(
  P2CONST(Os_IntLevelType, AUTOMATIC, AUTOMATIC) Level
))
{
  /* #10 Call HAL to return to given interrupt level. */
  Os_Hal_IntSetLevel(&(Level->State));                                                                                  /* SBSW_OS_FC_PRECONDITION */
}

/***********************************************************************************************************************
 *  Os_IntGetLevel()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_3219 */
Os_IntGetLevel,
(
  P2VAR(Os_IntLevelType, AUTOMATIC, AUTOMATIC) Level
))
{
  /* #10 Call HAL to return current interrupt level. */
  Os_Hal_IntGetLevel(&(Level->State));                                                                                  /* SBSW_OS_FC_PRECONDITION */
}

/***********************************************************************************************************************
 *  Os_IntLevelIsGt()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219 */ /* MD_Os_3219 */
Os_IntLevelIsGt,
(
  P2CONST(Os_IntLevelType, AUTOMATIC, AUTOMATIC) LevelX,
  P2CONST(Os_IntLevelType, AUTOMATIC, AUTOMATIC) LevelY
))
{
  return Os_Hal_IntLevelIsGt(&(LevelX->State), &(LevelY->State));                                                       /* SBSW_OS_FC_PRECONDITION */

}

/***********************************************************************************************************************
 *  Os_InterruptCheckEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE,                                                 /* PRQA S 3219 */ /* MD_Os_3219 */
OS_ALWAYS_INLINE, Os_InterruptCheckEnabled,
(
  P2CONST(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  return Os_ErrExtendedCheck((Os_StdReturnType)((InterruptState->CriticalSections.InDisableAll |
                                                 InterruptState->CriticalSections.InSuspendOs  |
                                                 InterruptState->CriticalSections.InSuspendAll  ) == 0));
}

/***********************************************************************************************************************
 *  Os_InterruptCleanup()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_InterruptCleanup,                      /* PRQA S 3219, 3673 */ /* MD_Os_3219, MD_Os_3673_StubFunction */
(
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  /* #10 Initialize the given interrupt API state. */
  Os_InterruptInit(InterruptState);                                                                                     /* SBSW_OS_FC_PRECONDITION */
}

/***********************************************************************************************************************
 *  Os_InterruptCopy()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_InterruptCopy,                         /* PRQA S 3219 */ /* MD_Os_3219 */
(
  P2CONST(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) Source,
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) Destination
))
{
  *Destination = *Source;                                                                                               /* SBSW_OS_PWA_PRECONDITION */
}

/***********************************************************************************************************************
 *  Os_InterruptIsCriticalSectionTracingEnabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE,                                                 /* PRQA S 3219 */ /* MD_Os_3219 */
OS_ALWAYS_INLINE, Os_InterruptIsCriticalSectionTracingEnabled, (void))
{
  return (Os_ErrIsServiceProtectionEnabled() | Os_ErrIsExtendedStatusEnabled());
}

/***********************************************************************************************************************
 *  Os_IntIsGlobal2LevelSwitchSupported()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE,OS_ALWAYS_INLINE,                                /* PRQA S 3219 */ /* MD_Os_3219 */
Os_IntIsGlobal2LevelSwitchSupported, (void))
{
  return Os_Hal_IntIsGlobal2LevelSwitchSupported();
}

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if !defined(OS_VCAST_INSTRUMENTATION_OS_INTERRUPT)
/*VCAST_DONT_INSTRUMENT_END*/
#endif

#endif /* OS_INTERRUPT_H */

/* module specific MISRA deviations:
 */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Interrupt.h
 **********************************************************************************************************************/

