/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 * \addtogroup Os_Interrupt
 * \{
 *
 * \file
 * \brief       Implementation of the interrupt module.
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os.h.
 **********************************************************************************************************************/

                                                                                                                        /* PRQA S 0777, 0779, 0810, 0828, 0857 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_1.1_810, MD_MSR_1.1_828, MD_MSR_1.1_857 */

#define OS_INTERRUPT_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
#include "Os_Interrupt.h"

/* Os module dependencies */
#include "Os_Thread.h"
#include "Os_TimingProtection.h"
#include "Os_Error.h"
#include "Os_Core.h"
#include "Os_Interrupt_Cfg.h"

/* Os Hal dependencies */
#include "Os_Hal_Compiler.h"
#include "Os_Hal_Core.h"

/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#define OS_START_SEC_CODE
#include "Os_MemMap_OsCode.h"                                                                                           /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  Os_InterruptCheckNotInCriticalSection()
 **********************************************************************************************************************/
/*! \brief        Returns whether the caller is in a critical section (OS_CHECK_FAILED) or not (Not OS_CHECK_FAILED).
 *  \details      --no details--
 *
 *  \param[in]    InterruptState   The interrupt state to check. Parameter must not be NULL.
 *
 *  \retval       Not OS_CHECK_FAILED   Not in critical section.
 *  \retval       OS_CHECK_FAILED       In critical section.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptCheckNotInCriticalSection,
(
  P2CONST(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));

/***********************************************************************************************************************
 *  Os_InterruptDisableAllEnter()
 **********************************************************************************************************************/
/*! \brief          Sets the in "DisableAll" flag.
 *  \details        --no details--
 *
 *  \param[in,out]  InterruptState   The interrupt state to modify. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptDisableAllEnter,
(
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));

/***********************************************************************************************************************
 *  Os_InterruptDisableAllExit()
 **********************************************************************************************************************/
/*! \brief          Clears the in "DisableAll" flag.
 *  \details        --no details--
 *
 *  \param[in,out]  InterruptState   The interrupt state to modify. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptDisableAllExit,
(
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));

/***********************************************************************************************************************
 *  Os_InterruptCheckInDisableAll()
 **********************************************************************************************************************/
/*! \brief          Returns whether the given interrupt state object is not in a disable-all critical section
 *                  (OS_CHECK_FAILED) or (Not OS_CHECK_FAILED).
 *  \details        --no details--
 *
 *  \param[in]      InterruptState   The interrupt state to modify. Parameter must not be NULL.
 *
 *  \retval         Not OS_CHECK_FAILED   In disable all.
 *  \retval         OS_CHECK_FAILED       Not in disable all.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptCheckInDisableAll,
(
  P2CONST(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));

/***********************************************************************************************************************
 *  Os_InterruptCheckNotInDisableAll()
 **********************************************************************************************************************/
/*! \brief          Returns whether the given interrupt state object is in a disable-all critical section
 *                  (OS_CHECK_FAILED) or (Not OS_CHECK_FAILED).
 *  \details        --no details--
 *
 *  \param[in]      InterruptState   The interrupt state to modify. Parameter must not be NULL.
 *
 *  \retval         Not OS_CHECK_FAILED   Not in disable all.
 *  \retval         OS_CHECK_FAILED       In disable all.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptCheckNotInDisableAll,
(
  P2CONST(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));

/***********************************************************************************************************************
 *  Os_InterruptSuspendAllEnter()
 **********************************************************************************************************************/
/*! \brief          Sets the in "SuspendAll" flag.
 *  \details        --no details--
 *
 *  \param[in,out]  InterruptState   The interrupt state to modify. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptSuspendAllEnter,
(
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));

/***********************************************************************************************************************
 *  Os_InterruptSuspendAllExit()
 **********************************************************************************************************************/
/*! \brief          Clears the in "SuspendAll" flag.
 *  \details        --no details--
 *
 *  \param[in,out]  InterruptState   The interrupt state to modify. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptSuspendAllExit,
(
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));

/***********************************************************************************************************************
 *  Os_InterruptCheckInSuspendAll()
 **********************************************************************************************************************/
/*! \brief          Returns whether the given interrupt state object is not in a "SuspendAll" critical section
 *                  (OS_CHECK_FAILED) or (Not OS_CHECK_FAILED).
 *  \details        --no details--
 *
 *  \param[in,out]  InterruptState   The interrupt state to modify. Parameter must not be NULL.
 *
 *  \retval         Not OS_CHECK_FAILED   In "SuspendAll".
 *  \retval         OS_CHECK_FAILED       Not in "SuspendAll".
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptCheckInSuspendAll,
(
  P2CONST(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));

/***********************************************************************************************************************
 *  Os_InterruptCheckNoOverflow()
 **********************************************************************************************************************/
/*! \brief        Returns whether the given nesting counter is greater zero (Not OS_CHECK_FAILED) or
 *                not (OS_CHECK_FAILED).
 *  \details      --no details--
 *
 *  \param[in]    InterruptState   The interrupt state to check.
 *
 *  \retval       Not OS_CHECK_FAILED   Greater zero.
 *  \retval       OS_CHECK_FAILED       zero.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptCheckNoOverflow,
(
  Os_IntApiCntType Counter
));

/***********************************************************************************************************************
 *  Os_InterruptSuspendOsEnter()
 **********************************************************************************************************************/
/*! \brief          Sets the in "SuspendOs" flag.
 *  \details        --no details--
 *
 *  \param[in,out]  InterruptState   The interrupt state to modify. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptSuspendOsEnter,
(
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));

/***********************************************************************************************************************
 *  Os_InterruptSuspendOsExit()
 **********************************************************************************************************************/
/*! \brief          Clears the in "SuspendOs" flag.
 *  \details        --no details--
 *
 *  \param[in,out]  InterruptState   The interrupt state to modify. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptSuspendOsExit,
(
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));

/***********************************************************************************************************************
 *  Os_InterruptCheckInSuspendOs()
 **********************************************************************************************************************/
/*! \brief          Returns whether the given interrupt state object is not in a "SuspendOs" critical section
 *                  (OS_CHECK_FAILED) or (Not OS_CHECK_FAILED).
 *  \details        --no details--
 *
 *  \param[in,out]  InterruptState   The interrupt state to modify. Parameter must not be NULL.
 *
 *  \retval         Not OS_CHECK_FAILED   In "SuspendAll".
 *  \retval         OS_CHECK_FAILED       Not in "SuspendAll".
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptCheckInSuspendOs,
(
  P2CONST(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));

/***********************************************************************************************************************
 *  Os_InterruptTakeSnapshot()
 **********************************************************************************************************************/
/*! \brief          Takes a snapshot of the given counter, if service protection is enabled.
 *  \details        --no details--
 *
 *  \param[in,out]  Snapshot  Snapshot where the counter value shall be stored. Parameter must not be NULL.
 *  \param[in]      Counter   Counter value.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptTakeSnapshot,
(
  P2VAR(Os_IntApiCntType, AUTOMATIC, OS_VAR_NOINIT) Snapshot,
  Os_IntApiCntType Counter
));

/***********************************************************************************************************************
 *  Os_InterruptCheckSnapshot()
 **********************************************************************************************************************/
/*! \brief        Returns whether the given snapshot and the counter are identical (Not OS_CHECK_FAILED) or not
 *                (OS_CHECK_FAILED).
 *  \details      --no details--
 *
 *  \param[in]    Snapshot  Previous snapshot of the counter.
 *  \param[in]    Counter   Counter value.
 *
 *  \retval       Not OS_CHECK_FAILED   Identical.
 *  \retval       OS_CHECK_FAILED       Not identical.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          Os_InterruptTakeSnapshot() has been called with same parameters previously.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptCheckSnapshot,
(
  Os_IntApiCntType Snapshot,
  Os_IntApiCntType Counter
));

/***********************************************************************************************************************
 *  Os_InterruptTpLockAllStart()
 **********************************************************************************************************************/
/*! \brief          Starts "all-interrupts" blocking time monitoring for given thread.
 *  \details        --no details--
 *
 *  \param[in]      Thread   Thread which shall be used as the budget owner. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptTpLockAllStart,
(
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) Thread
));

/***********************************************************************************************************************
 *  Os_InterruptTpLockAllStop()
 **********************************************************************************************************************/
/*! \brief          Stop "all-interrupts" blocking time monitoring for given thread.
 *  \details        --no details--
 *
 *  \param[in]      Thread   Thread which shall be used as the budget owner. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptTpLockAllStop,
(
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) Thread
));

/***********************************************************************************************************************
 *  Os_InterruptTpLockOsStart()
 **********************************************************************************************************************/
/*! \brief          Starts "OS-interrupts" blocking time monitoring for given thread.
 *  \details        --no details--
 *
 *  \param[in]      Thread   Thread which shall be used as the budget owner. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptTpLockOsStart,
(
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) Thread
));

/***********************************************************************************************************************
 *  Os_InterruptTpLockOsStop()
 **********************************************************************************************************************/
/*! \brief          Stop "OS-interrupts" blocking time monitoring for given thread.
 *  \details        --no details--
 *
 *  \param[in]      Thread   Thread which shall be used as the budget owner. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptTpLockOsStop,
(
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) Thread
));

/***********************************************************************************************************************
 *  Os_IntSuspendCat2()
 **********************************************************************************************************************/
/*! \brief          Suspend all category 2 ISRs.
 *  \details        Disable core local interrupt delivery of category 2 ISRs and return previous state.
 *                  Prevent to change the interrupt level in case that would allow more interrupts to occur.
 *
 *  \param[out]     State   Previous interrupt state information which allows resuming cat2 interrupt handling in HAL.
 *                          Parameter must not be NULL.
 *
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_IntSuspendCat2,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) State
));

/***********************************************************************************************************************
 *  Os_IntSuspendCat1()
 **********************************************************************************************************************/
/*! \brief          Suspend all ISRs of categories 1 and 2.
 *  \details        Disable core local interrupt delivery of category 1+2 ISRs and return previous state.
 *                  Prevent to change the interrupt level in case that would allow more interrupts to occur.
 *
 *  \param[out]     Cat1State Previous interrupt state information which allows resuming cat1 interrupt handling in HAL.
 *                            Parameter must not be NULL.
 *
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_IntSuspendCat1,
(
  P2VAR(Os_IntStateType, AUTOMATIC, OS_APPL_DATA) Cat1State
));

/***********************************************************************************************************************
 *  Os_IntResumeCat1()
 **********************************************************************************************************************/
/*! \brief          Resume all ISRs of categories 1 and 2.
 *  \details        Enable core local interrupt delivery of category 1+2 ISRs and return previous state.
 *
 *  \param[in]     Cat1State  Interrupt state information retrieved by Os_IntSuspendCat1 which allows resuming
 *                            cat1 interrupt handling in HAL. Parameter must not be NULL.
 *
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_IntResumeCat1,
(
  P2CONST(Os_IntStateType, AUTOMATIC, OS_APPL_DATA) Cat1State
));

/***********************************************************************************************************************
 *  Os_InterruptIsCat0Enabled()
 **********************************************************************************************************************/
/*! \brief        Returns whether Cat0 Isrs are enabled (!0) or not (0).
 *  \details      --no details--
 *
 *  \retval       !0    Enabled.
 *  \retval       0     Disabled.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptIsCat0Enabled, (void));

/***********************************************************************************************************************
 *  Os_InterruptLockOnLevelIsNeeded()
 **********************************************************************************************************************/
/*! \brief        Returns whether interrupt locking on level is needed (!0) or not (0).
 *  \details      If Cat 0 ISRs or timing protection are enabled it is not allowed to use the global interrupt flag.
 *
 *  \retval       !0    Enabled.
 *  \retval       0     Disabled.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE,
OS_ALWAYS_INLINE, Os_InterruptLockOnLevelIsNeeded, (void));

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  Os_InterruptCheckNotInCriticalSection()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptCheckNotInCriticalSection,
(
  P2CONST(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  return Os_ErrServiceProtectionCheck((Os_StdReturnType)((InterruptState->CriticalSections.InDisableAll |
                                                          InterruptState->CriticalSections.InSuspendOs |
                                                          InterruptState->CriticalSections.InSuspendAll) == 0));
}

/***********************************************************************************************************************
 *  Os_InterruptDisableAllEnter()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptDisableAllEnter,
(
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  if(Os_InterruptIsCriticalSectionTracingEnabled() != 0)
  {
    InterruptState->CriticalSections.InDisableAll = 1;                                                                  /* SBSW_OS_PWA_PRECONDITION */
  }
}

/***********************************************************************************************************************
 *  Os_InterruptDisableAllExit()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptDisableAllExit,
(
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  if(Os_InterruptIsCriticalSectionTracingEnabled() != 0)
  {
    InterruptState->CriticalSections.InDisableAll = 0;                                                                  /* SBSW_OS_PWA_PRECONDITION */
  }
}

/***********************************************************************************************************************
 *  Os_InterruptCheckInDisableAll()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptCheckInDisableAll,
(
  P2CONST(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  return Os_ErrServiceProtectionCheck((Os_StdReturnType)(InterruptState->CriticalSections.InDisableAll == 1));
}

/***********************************************************************************************************************
 *  Os_InterruptCheckNotInDisableAll()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptCheckNotInDisableAll,
(
  P2CONST(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  return Os_ErrServiceProtectionCheck((Os_StdReturnType)(InterruptState->CriticalSections.InDisableAll == 0));
}

/***********************************************************************************************************************
 *  Os_InterruptSuspendAllEnter()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptSuspendAllEnter,
(
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  if(Os_InterruptIsCriticalSectionTracingEnabled() != 0)
  {
    InterruptState->CriticalSections.InSuspendAll = 1;                                                                  /* SBSW_OS_PWA_PRECONDITION */
  }
}

/***********************************************************************************************************************
 *  Os_InterruptSuspendAllExit()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptSuspendAllExit,
(
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  if(Os_InterruptIsCriticalSectionTracingEnabled() != 0)
  {
    InterruptState->CriticalSections.InSuspendAll = 0;                                                                  /* SBSW_OS_PWA_PRECONDITION */
  }
}

/***********************************************************************************************************************
 *  Os_InterruptCheckInSuspendAll()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptCheckInSuspendAll,
(
  P2CONST(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  return Os_ErrServiceProtectionCheck((Os_StdReturnType)(InterruptState->CriticalSections.InSuspendAll == 1));
}

/***********************************************************************************************************************
 *  Os_InterruptSuspendOsEnter()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptSuspendOsEnter,
(
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  if(Os_InterruptIsCriticalSectionTracingEnabled() != 0)
  {
    InterruptState->CriticalSections.InSuspendOs = 1;                                                                   /* SBSW_OS_PWA_PRECONDITION */
  }
}

/***********************************************************************************************************************
 *  Os_InterruptSuspendOsExit()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptSuspendOsExit,
(
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  if(Os_InterruptIsCriticalSectionTracingEnabled() != 0)
  {
    InterruptState->CriticalSections.InSuspendOs = 0;                                                                   /* SBSW_OS_PWA_PRECONDITION */
  }
}

/***********************************************************************************************************************
 *  Os_InterruptCheckInSuspendOs()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptCheckInSuspendOs,
(
  P2CONST(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
))
{
  return Os_ErrServiceProtectionCheck((Os_StdReturnType)(InterruptState->CriticalSections.InSuspendOs == 1));
}

/***********************************************************************************************************************
 *  Os_InterruptCheckNoOverflow()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptCheckNoOverflow,
(
  Os_IntApiCntType Counter
))
{
  return Os_ErrServiceProtectionCheck((Os_StdReturnType)(Counter > 0));
}

/***********************************************************************************************************************
 *  Os_InterruptTakeSnapshot()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptTakeSnapshot,
(
  P2VAR(Os_IntApiCntType, AUTOMATIC, OS_VAR_NOINIT) Snapshot,
  Os_IntApiCntType Counter
))
{
  *Snapshot = Counter;                                                                                                  /* SBSW_OS_PWA_PRECONDITION */
}

/***********************************************************************************************************************
 *  Os_InterruptCheckSnapshot()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptCheckSnapshot,
(
  Os_IntApiCntType Snapshot,
  Os_IntApiCntType Counter
))
{
  return Os_ErrServiceProtectionCheck((Os_StdReturnType)(Snapshot == Counter));
}

/***********************************************************************************************************************
 *  Os_InterruptTpLockAllStart()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptTpLockAllStart,
(
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) Thread
))
{
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) core;
  P2CONST(Os_TpConfigType, TYPEDEF, OS_CONST) coreTp;

  /* #10 Get the correct parameter values and call the respective timing protection function. */
  core = Os_ThreadGetCore(Thread);                                                                                      /* SBSW_OS_FC_PRECONDITION */
  coreTp = Os_CoreAsrGetTimingProtection(core);                                                                         /* SBSW_OS_INT_COREASRGETTIMINGPROTECTION_001 */

  Os_TpStartAllInterrupt(coreTp);                                                                                       /* SBSW_OS_INT_TPSTARTALLINTERRUPT_001 */
}

/***********************************************************************************************************************
 *  Os_InterruptTpLockAllStop()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptTpLockAllStop,
(
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) Thread
))
{
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) core;
  P2CONST(Os_TpConfigType, TYPEDEF, OS_CONST) coreTp;

  /* #10 Get the correct parameter values and call the respective timing protection function. */
  core = Os_ThreadGetCore(Thread);                                                                                      /* SBSW_OS_FC_PRECONDITION */
  coreTp = Os_CoreAsrGetTimingProtection(core);                                                                         /* SBSW_OS_INT_COREASRGETTIMINGPROTECTION_001 */

  Os_TpStopAllInterrupt(coreTp);                                                                                        /* SBSW_OS_INT_TPSTOPALLINTERRUPT_001 */
}

/***********************************************************************************************************************
 *  Os_InterruptTpLockOsStart()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptTpLockOsStart,
(
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) Thread
))
{
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) core;
  P2CONST(Os_TpConfigType, TYPEDEF, OS_CONST) coreTp;

  /* #10 Get the correct parameter values and call the respective timing protection function. */
  core = Os_ThreadGetCore(Thread);                                                                                      /* SBSW_OS_FC_PRECONDITION */
  coreTp = Os_CoreAsrGetTimingProtection(core);                                                                         /* SBSW_OS_INT_COREASRGETTIMINGPROTECTION_001 */

  Os_TpStartOsInterrupt(coreTp);                                                                                        /* SBSW_OS_INT_TPSTARTOSINTERRUPT_001 */
}

/***********************************************************************************************************************
 *  Os_InterruptTpLockOsStop()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_InterruptTpLockOsStop,
(
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) Thread
))
{
  P2CONST(Os_CoreAsrConfigType, AUTOMATIC, OS_CONST) core;
  P2CONST(Os_TpConfigType, TYPEDEF, OS_CONST) coreTp;

  /* #10 Get the correct parameter values and call the respective timing protection function. */
  core = Os_ThreadGetCore(Thread);                                                                                      /* SBSW_OS_FC_PRECONDITION */
  coreTp = Os_CoreAsrGetTimingProtection(core);                                                                         /* SBSW_OS_INT_COREASRGETTIMINGPROTECTION_001 */

  Os_TpStopOsInterrupt(coreTp);                                                                                         /* SBSW_OS_INT_TPSTOPOSINTERRUPT_001 */
}

/***********************************************************************************************************************
 *  Os_IntSuspendCat1()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_IntSuspendCat1,
(
  P2VAR(Os_IntStateType, AUTOMATIC, OS_APPL_DATA) Cat1State
))
{
  /* #10 Check if interrupt locking via level is needed: */
  if(Os_InterruptLockOnLevelIsNeeded() != 0)
  {
    /* #20 Fetch current state and disable up category 1 ISRs if required. */
    Os_Hal_IntLevelType cat1DisableLevel;

    Os_Hal_IntGetCat1LockLevel(&cat1DisableLevel);                                                                      /* SBSW_OS_FC_POINTER2LOCAL */
    Os_Hal_IntGetLevel(&(Cat1State->Level));                                                                            /* SBSW_OS_FC_PRECONDITION */

    if(Os_Hal_IntLevelIsGt(&cat1DisableLevel, &(Cat1State->Level)) != 0)                                                /* SBSW_OS_FC_POINTER2LOCAL */ /* SBSW_OS_FC_PRECONDITION */
    {
      Os_Hal_IntSetLevel(&cat1DisableLevel);                                                                            /* SBSW_OS_FC_PRECONDITION */
    }
  }
  /* #30 Otherwise fetch current state and disable globally. */
  else
  {
    Os_Hal_IntGlobalStateType globalDisableState;

    Os_Hal_IntGetGlobalDisable(&globalDisableState);                                                                    /* SBSW_OS_FC_POINTER2LOCAL */
    Os_Hal_IntGetGlobalState(&(Cat1State->Global));                                                                     /* SBSW_OS_FC_PRECONDITION */
    Os_Hal_IntSetGlobalState(&globalDisableState);                                                                      /* SBSW_OS_FC_POINTER2LOCAL */
  }
}                                                                                                                       /* PRQA S 6050 */ /* MD_Os_STCAL */

/***********************************************************************************************************************
 *  Os_IntResumeCat1()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_IntResumeCat1,
(
  P2CONST(Os_IntStateType, AUTOMATIC, OS_APPL_DATA) Cat1State
))
{
  /* #10 If level lock is needed restore previous state level based. */
  if(Os_InterruptLockOnLevelIsNeeded() != 0)
  {
    Os_Hal_IntSetLevel(&(Cat1State->Level));                                                                            /* SBSW_OS_FC_PRECONDITION */
  }
  /* #20 Otherwise restore previous state based on global state. */
  else
  {
    Os_Hal_IntSetGlobalState(&(Cat1State->Global));                                                                     /* SBSW_OS_FC_PRECONDITION */
  }
}

/***********************************************************************************************************************
 *  Os_IntSuspendCat2()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_IntSuspendCat2,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) State
))
{
  Os_Hal_IntLevelType cat2DisableLevel;

  /* #10 Get the level which has to be set to disable category2 interrupt handling. */
  Os_Hal_IntGetCat2LockLevel(&cat2DisableLevel);                                                                        /* SBSW_OS_FC_POINTER2LOCAL */
  /* #20 Get and store the current level from HAL. */
  Os_Hal_IntGetLevel(State);                                                                                            /* SBSW_OS_FC_PRECONDITION */

  /* #30 If the category 2 disable level is logically higher than the current level: */
  if(Os_Hal_IntLevelIsGt(&cat2DisableLevel, State) != 0)                                                                /* SBSW_OS_FC_POINTER2LOCAL */
  {
    /* #40 Call HAL method to disable level based. */
    Os_Hal_IntSetLevel(&cat2DisableLevel);                                                                              /* SBSW_OS_FC_PRECONDITION */
  }
}

/***********************************************************************************************************************
 *  Os_InterruptIsCat0Enabled()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE,                                                 /* PRQA S 3219 */ /* MD_Os_3219 */
OS_ALWAYS_INLINE, Os_InterruptIsCat0Enabled, (void))
{
  return (Os_StdReturnType)(OS_CFG_CAT0 == STD_ON);
}

/***********************************************************************************************************************
 *  Os_InterruptLockOnLevelIsNeeded()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE,                                                 /* PRQA S 3219 */ /* MD_Os_3219 */
OS_ALWAYS_INLINE, Os_InterruptLockOnLevelIsNeeded, (void))
{
  Os_StdReturnType result;
  if(Os_TpIsEnabled() != 0)
  {
    result = !0;    
  }
  else
  {
    result = Os_InterruptIsCat0Enabled();
  }
  return result;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Os_InterruptInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_InterruptInit
(
  P2VAR(Os_IntApiStateType, TYPEDEF, OS_VAR_NOINIT) IntApiState
)
{

  if(Os_InterruptIsCriticalSectionTracingEnabled() != 0)
  {
    /* #10 Set all critical section flags to zero. */
    IntApiState->CriticalSections.InDisableAll = 0;                                                                     /* SBSW_OS_PWA_PRECONDITION */
    IntApiState->CriticalSections.InSuspendAll = 0;                                                                     /* SBSW_OS_PWA_PRECONDITION */
    IntApiState->CriticalSections.InSuspendOs = 0;                                                                      /* SBSW_OS_PWA_PRECONDITION */
  }

  /* #20 Set the nesting counter for SuspendAll to zero. */
  IntApiState->SuspendAllCounter = 0;                                                                                   /* SBSW_OS_PWA_PRECONDITION */

  /* #30 Set the nesting counter for SuspendOS to zero. */
  IntApiState->SuspendOsCounter = 0;                                                                                    /* SBSW_OS_PWA_PRECONDITION */
}

/***********************************************************************************************************************
 *  Os_Api_DisableAllInterrupts()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_Api_DisableAllInterrupts                                                                         /* PRQA S 1503 */ /* MD_MSR_14.1 */
(
  void
)
{
  Os_StatusType status;
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) caller;
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) interruptApiState;

  caller = Os_CoreGetThread();
  interruptApiState = Os_ThreadGetInterruptApiState(caller);                                                            /* SBSW_OS_INT_THREADGETINTERRUPTAPISTATE_001 */

  /* #10 Check that no interrupt API has been called previously. */
  if(OS_UNLIKELY(Os_InterruptCheckNotInCriticalSection(interruptApiState) == OS_CHECK_FAILED))                          /* SBSW_OS_INT_INTERRUPTCHECKNOTINCRITICALSECTION_001 */
  {
    /*! Internal comment removed.
 *
 * */
    status = OS_STATUS_API_ERROR;
  }
  else
  {
    /* #20 Tell timing protection module to start measuring locking time. */
    Os_InterruptTpLockAllStart(caller);                                                                                 /* SBSW_OS_INT_INTERRUPTTPLOCKALLSTART_001 */

    /* #30 Save current interrupt state and disable up to cat1 ISRs. */
    Os_IntSuspendCat1(&(interruptApiState->SuspendAllPrevState));                                                       /* SBSW_OS_INT_INTSUSPENDCAT1_001 */

    /* #40 Inform the trace module that interrupts have been disabled */
    Os_TraceInterruptsGlobalDisabled();

    /* #50 Remember that we are in a DisableAllInterrupts() sequence so we are able to detect invalid service calls. */
    Os_InterruptDisableAllEnter(interruptApiState);                                                                     /* SBSW_OS_INT_INTERRUPTDISABLEALLENTER_001 */

    status = OS_STATUS_OK;
  }

  /* #60 Report error if applicable or return. */
  Os_ErrReportDisableAllInterrupts(status);
}                                                                                                                       /* PRQA S 6050 */ /* MD_Os_STCAL */

/***********************************************************************************************************************
 *  Os_Api_EnableAllInterrupts()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_Api_EnableAllInterrupts                                                                          /* PRQA S 1503 */ /* MD_MSR_14.1 */
(
  void
)
{
  Os_StatusType status;
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) caller;
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) interruptApiState;

  caller = Os_CoreGetThread();
  interruptApiState = Os_ThreadGetInterruptApiState(caller);                                                            /* SBSW_OS_INT_THREADGETINTERRUPTAPISTATE_001 */

  /* #10 Check that DisableAllInterrupts() has been called previously. */
  if(OS_UNLIKELY(Os_InterruptCheckInDisableAll(interruptApiState) == OS_CHECK_FAILED))                                  /* SBSW_OS_INT_INTERRUPTCHECKINDISABLEALL_001 */
  {
    status = OS_STATUS_API_ERROR;
  }
  else
  {
    /* #20 Remember that we left the interrupt critical section. */
    Os_InterruptDisableAllExit(interruptApiState);                                                                      /* SBSW_OS_INT_INTERRUPTDISABLEALLEXIT_001 */

    /* #30 Inform the trace module that interrupts are going to be enabled */
    Os_TraceInterruptsGlobalEnabled();

    /* #40 Restore the interrupt state which has been saved in DisableAllInterrupts(). */
    Os_IntResumeCat1(&interruptApiState->SuspendAllPrevState);                                                          /* SBSW_OS_INT_INTRESUMECAT1_001 */

    /* #50 Tell timing protection module to stop measuring locking time. */
    Os_InterruptTpLockAllStop(caller);                                                                                  /* SBSW_OS_INT_INTERRUPTTPLOCKALLSTOP_001 */

    status = OS_STATUS_OK;
  }

  /* #60 Report error if applicable or return. */
  Os_ErrReportEnableAllInterrupts(status);
}                                                                                                                       /* PRQA S 6050 */ /* MD_Os_STCAL */

/***********************************************************************************************************************
 *  Os_Api_SuspendAllInterrupts()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_Api_SuspendAllInterrupts                                                                         /* PRQA S 1503 */ /* MD_MSR_14.1 */
(
  void
)
{
  Os_StatusType status;
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) caller;
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) interruptApiState;

  caller = Os_CoreGetThread();
  interruptApiState = Os_ThreadGetInterruptApiState(caller);                                                            /* SBSW_OS_INT_THREADGETINTERRUPTAPISTATE_001 */

  /* #10 Check that DisableAllInterrupts() has not been called previously. */
  if(OS_UNLIKELY(Os_InterruptCheckNotInDisableAll(interruptApiState) == OS_CHECK_FAILED))                               /* SBSW_OS_INT_INTERRUPTCHECKNOTINCRITICALSECTION_001 */
  {
    status = OS_STATUS_API_ERROR;
  }
  else
  {
    /* #20 If SuspendAllInterrupts() nesting counter is zero: */
    if(interruptApiState->SuspendAllCounter == 0)
    {
      /* #30 Tell timing protection module to start measuring locking time. */
      Os_InterruptTpLockAllStart(caller);                                                                               /* SBSW_OS_INT_INTERRUPTTPLOCKALLSTART_001 */

      /* #40 Save current interrupt state and disable up to cat1 ISRs. */
      Os_IntSuspendCat1(&interruptApiState->SuspendAllPrevState);                                                       /* SBSW_OS_INT_INTSUSPENDCAT1_001 */

      /* #50 Inform trace module that interrupts have been disabled */
      Os_TraceInterruptsGlobalDisabled();

      /* #60 Remember that we are in a SuspendAllInterrupts() sequence. */
      Os_InterruptSuspendAllEnter(interruptApiState);                                                                   /* SBSW_OS_INT_INTERRUPTSUSPENDALLENTER_001 */

      /* #70 Take SuspendOS nesting counter snapshot. */
      Os_InterruptTakeSnapshot(&interruptApiState->SuspendOsSnapshot, interruptApiState->SuspendOsCounter);             /* SBSW_OS_INT_INTERRUPTTAKESNAPSHOT_001 */
    }

    /* #80 Increment the nesting counter for SuspendAllInterrupts() calls. */
    interruptApiState->SuspendAllCounter++;                                                                             /* SBSW_OS_INT_THREADGETINTERRUPTAPISTATE_002 */

    /* #90 Check that there is no counter overflow. */
    if(OS_UNLIKELY(Os_InterruptCheckNoOverflow(interruptApiState->SuspendAllCounter) == OS_CHECK_FAILED))               /* COV_OS_INTCOUNTEROVERFLOW */
    {
      status = OS_STATUS_API_ERROR;
    }
    else
    {
      status = OS_STATUS_OK;
    }
  }

  /* #100 Report error if applicable or return. */
  Os_ErrReportSuspendAllInterrupts(status);
}                                                                                                                       /* PRQA S 6050 */ /* MD_Os_STCAL */

/***********************************************************************************************************************
 *  Os_Api_ResumeAllInterrupts()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_Api_ResumeAllInterrupts                                                                          /* PRQA S 1503 */ /* MD_MSR_14.1 */
(
  void
)
{
  Os_StatusType status;
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) caller;
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) interruptApiState;

  caller = Os_CoreGetThread();
  interruptApiState = Os_ThreadGetInterruptApiState(caller);                                                            /* SBSW_OS_INT_THREADGETINTERRUPTAPISTATE_001 */

  /* #10 Check that SuspendAllInterrupts() has been called previously. */
  if(OS_UNLIKELY(Os_InterruptCheckInSuspendAll(interruptApiState) == OS_CHECK_FAILED))                                  /* SBSW_OS_INT_INTERRUPTCHECKINSUSPENDALL_001 */
  {
    status = OS_STATUS_API_ERROR;
  }
  else
  {
    /* #20 Decrement the nesting counter for SuspendAllInterrupts() calls. */
    interruptApiState->SuspendAllCounter--;                                                                             /* SBSW_OS_INT_THREADGETINTERRUPTAPISTATE_002 */

    /* #30 If the nesting counter is zero: */
    if (interruptApiState->SuspendAllCounter == 0)
    {
      /* #40 Check whether the user did open/close a SuspendOSInterrupts() pair meanwhile. */
      if(OS_UNLIKELY(Os_InterruptCheckSnapshot(interruptApiState->SuspendOsSnapshot,
              interruptApiState->SuspendOsCounter) == OS_CHECK_FAILED))
      {
        status = OS_STATUS_API_ERROR;
      }
      else
      {
        /* #50 Remember that we have left the SuspendAllInterrupts() sequence. */
        Os_InterruptSuspendAllExit(interruptApiState);                                                                  /* SBSW_OS_INT_INTERRUPTSUSPENDALLEXIT_001 */

        /* #60 Inform the trace module that interrupts are going to be enabled */
        Os_TraceInterruptsGlobalEnabled();

        /* #70 Restore the interrupt state which has been saved in SuspendAllInterrupts(). */
        Os_IntResumeCat1(&interruptApiState->SuspendAllPrevState);                                                      /* SBSW_OS_INT_INTRESUMECAT1_001 */

        /* #80 Tell timing protection module to stop measuring locking time. */
        Os_InterruptTpLockAllStop(caller);                                                                              /* SBSW_OS_INT_INTERRUPTTPLOCKALLSTOP_001 */

        status = OS_STATUS_OK;
      }
    }
    else
    {
      status = OS_STATUS_OK;
    }
  }

  /* #90 Report error if applicable or return. */
  Os_ErrReportResumeAllInterrupts(status);
}                                                                                                                       /* PRQA S 6050 */ /* MD_Os_STCAL */

/***********************************************************************************************************************
 *  Os_Api_SuspendOSInterrupts()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_Api_SuspendOSInterrupts                                                                          /* PRQA S 1503 */ /* MD_MSR_14.1 */
(
  void
)
{
  Os_StatusType status;
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) caller;
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) interruptApiState;

  caller = Os_CoreGetThread();
  interruptApiState = Os_ThreadGetInterruptApiState(caller);                                                            /* SBSW_OS_INT_THREADGETINTERRUPTAPISTATE_001 */

  /* #10 Check that DisableAllInterrupts() has not been called previously. */
  if(OS_UNLIKELY(Os_InterruptCheckNotInDisableAll(interruptApiState) == OS_CHECK_FAILED))                               /* SBSW_OS_INT_INTERRUPTCHECKNOTINDISABLEALL_001 */
  {
    status = OS_STATUS_API_ERROR;
  }
  else
  {
    /* #20 If SuspendOSInterrupts() nesting counter is zero: */
    if (interruptApiState->SuspendOsCounter == 0)
    {
      /* #30 Tell timing protection module to start measuring locking time. */
      Os_InterruptTpLockOsStart(caller);                                                                                /* SBSW_OS_INT_INTERRUPTTPLOCKOSSTART_001 */

      /* #40 Save current interrupt state and disable up to cat2 ISRs. */
      Os_IntSuspendCat2(&interruptApiState->SuspendOsPrevState);                                                        /* SBSW_OS_INT_INTSUSPENDCAT2_001 */

      /* #50 Inform the trace module that interrupts have been disabled. */
      Os_TraceInterruptsLevelDisabled();

      /* #60 Remember that we are in a SuspendOSInterrupts() sequence. */
      Os_InterruptSuspendOsEnter(interruptApiState);                                                                    /* SBSW_OS_INT_INTERRUPTSUSPENDOSENTER_001 */

      /* #70 Take SuspendAll nesting counter snapshot. */
      Os_InterruptTakeSnapshot(&interruptApiState->SuspendAllSnapshot, interruptApiState->SuspendAllCounter);           /* SBSW_OS_INT_INTERRUPTTAKESNAPSHOT_001 */
    }

    /* #80 Increment the nesting counter for SuspendOSInterrupts() calls. */
    interruptApiState->SuspendOsCounter++;                                                                              /* SBSW_OS_INT_THREADGETINTERRUPTAPISTATE_002 */

    /* #90 Check that there is no counter overflow. */
    if(OS_UNLIKELY(Os_InterruptCheckNoOverflow(interruptApiState->SuspendOsCounter) == OS_CHECK_FAILED))                /* COV_OS_INTCOUNTEROVERFLOW */
    {
      status = OS_STATUS_API_ERROR;
    }
    else
    {
      status = OS_STATUS_OK;
    }
  }

  /* #100 Report error if applicable or return. */
  Os_ErrReportSuspendOSInterrupts(status);
}                                                                                                                       /* PRQA S 6050 */ /* MD_Os_STCAL */

/***********************************************************************************************************************
 *  Os_Api_ResumeOSInterrupts()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_Api_ResumeOSInterrupts                                                                           /* PRQA S 1503 */ /* MD_MSR_14.1 */
(
  void
)
{
  Os_StatusType status;
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) caller;
  P2VAR(Os_IntApiStateType, AUTOMATIC, OS_VAR_NOINIT) interruptApiState;

  caller = Os_CoreGetThread();
  interruptApiState = Os_ThreadGetInterruptApiState(caller);                                                            /* SBSW_OS_INT_THREADGETINTERRUPTAPISTATE_001 */

  /* #10 Check that SuspendOSInterrupts() has been called previously. */
  if(OS_UNLIKELY(Os_InterruptCheckInSuspendOs(interruptApiState) == OS_CHECK_FAILED))                                   /* SBSW_OS_INT_INTERRUPTCHECKINSUSPENDOS_001 */
  {
    status = OS_STATUS_API_ERROR;
  }
  else
  {
    /* #20 Decrement the nesting counter for SuspendOSInterrupts() calls. */
    interruptApiState->SuspendOsCounter--;                                                                              /* SBSW_OS_INT_THREADGETINTERRUPTAPISTATE_002 */

    /* #30 If the nesting counter is zero: */
    if (interruptApiState->SuspendOsCounter == 0)
    {
      /* #40 Check whether the user did open/close a SuspendAllInterrupts() pair meanwhile. */
      if(OS_UNLIKELY(Os_InterruptCheckSnapshot(interruptApiState->SuspendAllSnapshot,
              interruptApiState->SuspendAllCounter) == OS_CHECK_FAILED))
      {
        status = OS_STATUS_API_ERROR;
      }
      else
      {
        /* #50 Remember that we have left the SuspendOSInterrupts() sequence. */
        Os_InterruptSuspendOsExit(interruptApiState);                                                                   /* SBSW_OS_INT_INTERRUPTSUSPENDOSEXIT_001 */

        /* #60 Inform the trace module that interrupts are going to be enabled. */
        Os_TraceInterruptsLevelEnabled();

        /* #70 Restore the interrupt state which has been saved in SuspendOSInterrupts(). */
        Os_Hal_IntSetLevel(&interruptApiState->SuspendOsPrevState);                                                     /* SBSW_OS_HAL_INTSETLEVEL */

        /* #80 Tell timing protection module to stop measuring locking time. */
        Os_InterruptTpLockOsStop(caller);                                                                               /* SBSW_OS_INT_INTERRUPTTPLOCKOSSTOP_001 */

        status = OS_STATUS_OK;
      }
    }
    else
    {
      status = OS_STATUS_OK;
    }
  }

  /* #90 Report error if applicable or return. */
  Os_ErrReportResumeOSInterrupts(status);
}                                                                                                                       /* PRQA S 6050 */ /* MD_Os_STCAL */

/***********************************************************************************************************************
 *  Os_Api_DisableGlobal()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, OS_CODE) Os_Api_DisableGlobal
(
  void
)
{
  /*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
   */
  /* #10 call Os_Api_DisableAllInterrupts(). */
  Os_Api_DisableAllInterrupts();
}

/***********************************************************************************************************************
 *  Os_Api_EnableGlobal()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, OS_CODE) Os_Api_EnableGlobal
(
  void
)
{
  /*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
   */

  /* #10 call Os_Api_EnableAllInterrupts(). */
  Os_Api_EnableAllInterrupts();
}

/***********************************************************************************************************************
 *  Os_Api_DisableLevel()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_Api_DisableLevel
(
  void
)
{
  Os_Hal_IntLevelType cat2DisableLevel;

  /* #10 Tell timing protection module to start measuring locking time. */
  Os_InterruptTpLockOsStart(Os_CoreGetThread());                                                                        /* SBSW_OS_INT_INTERRUPTTPLOCKOSSTART_001 */

  Os_Hal_IntGetCat2LockLevel(&cat2DisableLevel);                                                                        /* SBSW_OS_FC_POINTER2LOCAL */
  Os_Hal_IntSetLevel(&cat2DisableLevel);                                                                                /* SBSW_OS_FC_POINTER2LOCAL */

  /* #30 Inform the trace module that interrupts have been disabled. */
  Os_TraceInterruptsLevelDisabled();

}

/***********************************************************************************************************************
 *  Os_Api_EnableLevel()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, OS_CODE) Os_Api_EnableLevel
(
  void
)
{
  Os_Hal_IntLevelType taskLevel;

  /* #10 Inform the trace module that interrupts are going to be enabled. */
  Os_TraceInterruptsLevelEnabled();

  /* #20 Enable OS interrupts. */
  Os_Hal_IntGetTaskLevel(&taskLevel);                                                                                   /* SBSW_OS_FC_POINTER2LOCAL */
  Os_Hal_IntSetLevel(&taskLevel);                                                                                       /* SBSW_OS_FC_POINTER2LOCAL */

  /* #30 Tell timing protection module to stop measuring locking time. */
  Os_InterruptTpLockOsStop(Os_CoreGetThread());                                                                         /* SBSW_OS_INT_INTERRUPTTPLOCKOSSTOP_001 */
}

#define OS_STOP_SEC_CODE
#include "Os_MemMap_OsCode.h"                                                                                           /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* module specific MISRA deviations:
 */

/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_OS_INT_THREADGETINTERRUPTAPISTATE_001
 \DESCRIPTION    Os_ThreadGetInterruptApiState is called with the return value of Os_CoreGetThread.
 \COUNTERMEASURE \N [CM_OS_COREGETTHREAD_N]

\ID SBSW_OS_INT_THREADGETINTERRUPTAPISTATE_002
 \DESCRIPTION    Write access to the return value of Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTERRUPTTPLOCKALLSTART_001
 \DESCRIPTION    Os_InterruptTpLockAllStart is called with the return value of Os_CoreGetThread.
 \COUNTERMEASURE \N [CM_OS_COREGETTHREAD_N]

\ID SBSW_OS_INT_INTERRUPTTPLOCKOSSTART_001
 \DESCRIPTION    Os_InterruptTpLockOsStart is called with the return value of Os_CoreGetThread.
 \COUNTERMEASURE \N [CM_OS_COREGETTHREAD_N]

\ID SBSW_OS_INT_INTERRUPTCHECKNOTINCRITICALSECTION_001
 \DESCRIPTION    Os_InterruptCheckNotInCriticalSection is called with the return value of Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTERRUPTCHECKNOOVERFLOW_001
 \DESCRIPTION    Os_InterruptCheckNoOverflow is called with the return value of Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTERRUPTCHECKNOTINDISABLEALL_001
 \DESCRIPTION    Os_InterruptCheckNotInDisableAll is called with the return value of Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTERRUPTSUSPENDALLENTER_001
 \DESCRIPTION    Os_InterruptSuspendAllEnter is called with the return value of Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTERRUPTSUSPENDOSENTER_001
 \DESCRIPTION    Os_InterruptSuspendOsEnter is called with the return value of Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTERRUPTSUSPENDOSEXIT_001
 \DESCRIPTION    Os_InterruptSuspendOsExit is called with the return value of Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTERRUPTSUSPENDALLEXIT_001
 \DESCRIPTION    Os_InterruptSuspendAllExit is called with the return value of Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTERRUPTTAKESNAPSHOT_001
 \DESCRIPTION    Os_InterruptTakeSnapshot is called with the return value of Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTERRUPTCHECKSNAPSHOT_001
 \DESCRIPTION    Os_InterruptCheckSnapshot is called with the return value of Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTSUSPENDCAT1_001
 \DESCRIPTION    Os_IntSuspendCat1 is called with a value derived from the return value of
                 Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTSUSPENDCAT2_001
 \DESCRIPTION    Os_IntSuspendCat2 is called with a value derived from the return value of
                 Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTERRUPTDISABLEALLENTER_001
 \DESCRIPTION    Os_InterruptDisableAllEnter is called with the return value of Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTERRUPTCHECKINDISABLEALL_001
 \DESCRIPTION    Os_InterruptCheckInDisableAll is called with the return value of Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTERRUPTCHECKINSUSPENDALL_001
 \DESCRIPTION    Os_InterruptCheckInSuspendAll is called with the return value of Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTERRUPTCHECKINSUSPENDOS_001
 \DESCRIPTION    Os_InterruptCheckInSuspendOs is called with the return value of Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTERRUPTDISABLEALLEXIT_001
 \DESCRIPTION    Os_InterruptDisableAllExit is called with the return value of Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTRESUMECAT1_001
 \DESCRIPTION    Os_Hal_IntResumeCat1 is called with a value derived from the return value of
                 Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_HAL_INTSETLEVEL
 \DESCRIPTION    Os_Hal_IntSetLevel is called with a value derived from the return value of
                 Os_ThreadGetInterruptApiState.
 \COUNTERMEASURE \M [CM_OS_THREADGETINTERRUPTAPISTATE_M]

\ID SBSW_OS_INT_INTERRUPTTPLOCKALLSTOP_001
 \DESCRIPTION    Os_InterruptTpLockAllStop is called with the return value of Os_CoreGetThread.
 \COUNTERMEASURE \N [CM_OS_COREGETTHREAD_N]

\ID SBSW_OS_INT_INTERRUPTTPLOCKOSSTOP_001
 \DESCRIPTION    Os_InterruptTpLockOsStop is called with the return value of Os_CoreGetThread.
 \COUNTERMEASURE \N [CM_OS_COREGETTHREAD_N]

\ID SBSW_OS_INT_TPSTARTALLINTERRUPT_001
 \DESCRIPTION    Os_TpStartAllInterrupt is called with the return value of Os_CoreAsrGetTimingProtection.
 \COUNTERMEASURE \M [CM_OS_COREASRGETTIMINGPROTECTION_M]

\ID SBSW_OS_INT_TPSTOPALLINTERRUPT_001
 \DESCRIPTION    Os_TpStopAllInterrupt is called with the return value of Os_CoreAsrGetTimingProtection.
 \COUNTERMEASURE \M [CM_OS_COREASRGETTIMINGPROTECTION_M]

\ID SBSW_OS_INT_TPSTARTOSINTERRUPT_001
 \DESCRIPTION    Os_TpStartOsInterrupt is called with the return value of Os_CoreAsrGetTimingProtection.
 \COUNTERMEASURE \M [CM_OS_COREASRGETTIMINGPROTECTION_M]

\ID SBSW_OS_INT_TPSTOPOSINTERRUPT_001
 \DESCRIPTION    Os_TpStopOsInterrupt is called with the return value of Os_CoreAsrGetTimingProtection.
 \COUNTERMEASURE \M [CM_OS_COREASRGETTIMINGPROTECTION_M]

\ID SBSW_OS_INT_COREASRGETTIMINGPROTECTION_001
 \DESCRIPTION    Os_CoreAsrGetTimingProtection is called with the return value of Os_ThreadGetCore.
 \COUNTERMEASURE \M [CM_OS_THREADGETCORE_M]

SBSW_JUSTIFICATION_END */

/* START_COVERAGE_JUSTIFICATION

\ID COV_OS_INTCOUNTEROVERFLOW
   \ACCEPT XF
   \REASON Counter overflow cannot be checked for type uint16_least as the type is quite large and
           even depends on the used compiler (+options). However, the code is unit tested and code inspected.

END_COVERAGE_JUSTIFICATION */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Interrupt.c
 **********************************************************************************************************************/

