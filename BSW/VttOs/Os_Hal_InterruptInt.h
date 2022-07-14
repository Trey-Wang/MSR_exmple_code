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
 *  \ingroup    Os_Hal
 *  \defgroup   Os_Hal_Interrupt  HAL Interrupt
 *  \brief      Primitives for interrupt state manipulation
 *  \details
 *  This component provides the following interrupt primitives:
 *  - Enter/leave critical sections
 *  - Disable interrupt sources
 *
 *  ### Interrupt Priority Levels ###
 *  The kernel distinguishes between the following logical groups of interrupt priority levels:
 *
 *  Interrupt Priority Level    | Description
 *  :-------------------------- | :-------------------------------------------------------------------------------------
 *  Timing Protection Interrupt | The timing protection interrupt is handled on this level.
 *  Category 1 ISRs             | AUTOSAR category 1 ISRs are handled on this level.
 *  Category 2 ISRs             | AUTOSAR category 2 ISRs are handled on this level.
 *
 *  The kernel needs some primitive interrupt functions, to enter/leave critical sections on these interrupt priority
 *  levels.  Os_Hal_IntSuspend() and Os_Hal_IntResume() work either up to timing protection level or up to category 1
 *  level, depending whether timing protection is level or NMI based.
 *
 *  ![Interrupt Priorities and Relevant HAL Functions](InterruptPriorities.png)
 *
 *  \{
 *
 *  \file
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#if !defined (OS_HAL_INTERRUPTINT_H)                                                                                    /* PRQA S 0883 */ /* MD_Os_Hal_0883 */
# define OS_HAL_INTERRUPTINT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os kernel module dependencies */
# include "Os_Hal_Interrupt_Types.h"

/* Os module declarations */
# include "Os_CommonInt.h"

/* Os Hal dependencies */
# include "Os_Hal_CompilerInt.h"
# include "Os_Hal_CoreInt.h"


#if !defined(OS_VCAST_INSTRUMENTATION_OS_HAL)
/*VCAST_DONT_INSTRUMENT_START*/
#endif

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
/*!
 * \brief This is the interrupt level for Autosar task.
 */
# define OS_HAL_INTERRUPT_LEVEL_TASK (0)

/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/*! \brief    Data type which contains the complete interrupt state.
 *  \details  This data type contains:
 *               - The global interrupt flag
 *               - The timing protection interrupt flag
 *               - The interrupt level
 */
struct Os_Hal_InterruptStateType_Tag
{
  sint32 InterruptLevel;
};

struct Os_Hal_IntTrapStateType_Tag
{
  sint32 InterruptLevel;
};



/*! Interrupt handler type*/
typedef void (*Os_Hal_IntHandlerType)(void);

/*! Interrupt source ID type */
typedef sint32 Os_Hal_IntSourceType;

/*! \brief    Hardware specific configuration for category 2 ISRs.
 *  \details  It typically contains the interrupt priority level and the interrupt source of the ISR.
 *            The kernel uses this data type to:
 *            - Disable an interrupt source as part of killing applications.
 *            - Implement interrupt resources.
 */
struct Os_Hal_IntIsrConfigType_Tag
{
  /*! Name of the ISR */
  const char*               Name;

  /*! Interrupt source ID */
  Os_Hal_IntSourceType      Source;

  /*! Interrupt level */
  Os_Hal_IntLevelType       Level;

  /* Interrupt handler */
  Os_Hal_IntHandlerType     Handler;

  /* Interrupt routing mask */
  Os_Hal_IntRoutingMaskType RoutingMask;
};

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
 *  Os_Hal_IntGetGlobalState()
 **********************************************************************************************************************/
/*! \brief          Returns current core interrupt global state.
 *  \details        -
 *
 *  \param[out]     State   Current interrupt global flag.
 *                          Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/

OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                          /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
Os_Hal_IntGetGlobalState,
(
  P2VAR(Os_Hal_IntGlobalStateType, AUTOMATIC, OS_APPL_DATA) State
));

/***********************************************************************************************************************
 *  Os_Hal_IntSetGlobalState()
 **********************************************************************************************************************/
/*! \brief          Set the global interrupt state to the given value.
 *  \details        This function is used to enable and disable interrupt handling globally.
 *
 *  \param[in]      State   In: Desired interrupt global flag.
 *                          Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/

OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                          /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
Os_Hal_IntSetGlobalState,
(
  P2CONST(Os_Hal_IntGlobalStateType, AUTOMATIC, OS_APPL_DATA) State
));


/***********************************************************************************************************************
 *  Os_Hal_IntGetLevel()
 **********************************************************************************************************************/
/*! \brief          Returns the current interrupt priority level.
 *  \details        -
 *
 *  \param[out]     Level   Current interrupt priority level. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntGetLevel,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
));


/***********************************************************************************************************************
 *  Os_Hal_IntSetLevel()
 **********************************************************************************************************************/
/*! \brief          Set the interrupt priority level to the given value.
 *  \details        -
 *
 *  \param[in]      Level   Desired interrupt priority level.
 *                          Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                          /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
Os_Hal_IntSetLevel,
(
  P2CONST(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
));


/***********************************************************************************************************************
 *  Os_Hal_IntGetGlobalDisable()
 **********************************************************************************************************************/
/*! \brief          Returns the global flag value which is used to disable all interrupts (Cat1, Cat2 and TP).
 *  \details        -
 *
 *  \param[out]     State   Global flag value to disable all interrupts.
 *                          Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/

OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntGetGlobalDisable,
(
  P2VAR(Os_Hal_IntGlobalStateType, AUTOMATIC, OS_APPL_DATA) State
));


/***********************************************************************************************************************
 *  Os_Hal_IntGetGlobalEnable()
 **********************************************************************************************************************/
/*! \brief          Returns the global flag value which is used to enable all interrupts.
 *  \details        -
 *
 *  \param[out]     State   Global flag value to enable all interrupts.
 *                          Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntGetGlobalEnable,
(
  P2VAR(Os_Hal_IntGlobalStateType, AUTOMATIC, OS_APPL_DATA) State
));


/***********************************************************************************************************************
 *  Os_Hal_IntGetCat2LockLevel()
 **********************************************************************************************************************/
/*! \brief          Returns the interrupt priority level which is used to lock all category 2 ISRs.
 *  \details        -
 *
 *  \param[out]     Level   Category 2 Lock Level. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntGetCat2LockLevel,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
));


/***********************************************************************************************************************
 *  Os_Hal_IntGetCat1LockLevel()
 **********************************************************************************************************************/
/*! \brief          Returns the interrupt priority level which is used to lock all category 1 ISRs.
 *  \details        -
 *
 *  \param[out]     Level   Category 1 Lock Level. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntGetCat1LockLevel,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
));


/***********************************************************************************************************************
 *  Os_Hal_IntGetTPLockLevel()
 **********************************************************************************************************************/
/*! \brief          Returns the interrupt priority level which is used to lock up to Cat0 level including the TP ISR.
 *  \details        -
 *
 *  \param[out]     Level   TP Lock Level. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntGetTPLockLevel,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
));


/***********************************************************************************************************************
 *  Os_Hal_IntGetTaskLevel()
 **********************************************************************************************************************/
/*! \brief          Returns the interrupt level which is used to enable interrupt handling on task level.
 *  \details        -
 *

 *  \param[out]     Level   Task level to enable interrupt handling. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                          /* PRQA S 3219 */ /* MD_Os_Hal_3219 */
Os_Hal_IntGetTaskLevel,
(
  P2VAR(Os_Hal_IntLevelType, AUTOMATIC, OS_APPL_DATA) Level
));


/***********************************************************************************************************************
 *  Os_Hal_IntGetInterruptState()
 **********************************************************************************************************************/
/*! \brief          Returns the current interrupt state.
 *  \details        This function is used to transfer the current interrupt state between different context
 *                  (e.g. non-trusted functions).
 *
 *  \param[out]     InterruptState   Location where to store the current interrupt state. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntGetInterruptState,
(
  P2VAR(Os_Hal_IntStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));


/***********************************************************************************************************************
 *  Os_Hal_IntDisableSource()
 **********************************************************************************************************************/
/*! \brief          Disable interrupt delivery for the given interrupt source.
 *  \details        -
 *
 *  \param[in]      IsrConfig   Interrupt configuration to the ISR which shall be disabled.
 *                              Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntDisableSource,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntEnableSource()
 **********************************************************************************************************************/
/*! \brief          Enable interrupt delivery for the given interrupt source.
 *  \details        -
 *
 *  \param[in]      IsrConfig   Interrupt configuration to the ISR which shall be enabled.
 *                              Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntEnableSource,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntIsClearPendingSupported()
 **********************************************************************************************************************/
/*! \brief        Returns whether the hardware supports clearing of a pending interrupt.
 *  \details      --no details--
 *
 *  \retval       !0    Pending interrupts can be cleared.
 *  \retval       0     Pending interrupts cannot be cleared.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsClearPendingSupported,( void ));


/***********************************************************************************************************************
 *  Os_Hal_IntClearPending()
 **********************************************************************************************************************/
/*! \brief          Clears the pending flag of the given interrupt source.
 *  \details        --no details--
 *
 *  \param[in]      IsrConfig   Interrupt configuration to the ISR which shall be cleared.
 *                              Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntClearPending,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptSourceEnabled()
 **********************************************************************************************************************/
/*! \brief          Checks if the interrupt source is enabled
 *  \details        --no details--
 *
 *  \param[in]      IsrConfig   Interrupt configuration to the ISR which shall be checked.
 *                              Parameter must not be NULL.
 *  \retval         0           not enabled
 *  \retval         !0          enabled
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsInterruptSourceEnabled,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntIsInterruptPending()
 **********************************************************************************************************************/
/*! \brief          Checks if the specified ISR has a pending request
 *  \details        --no details--
 *
 *  \param[in]      IsrConfig   Interrupt configuration to the ISR which shall be cleared.
 *                              Parameter must not be NULL.
 *  \retval         0           no pending request.
 *  \retval         !0          pending request.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsInterruptPending,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntEndOfInterrupt()
 **********************************************************************************************************************/
/*! \brief          Signal hardware that interrupt processing has been completed.
 *  \details        -
 *
 *  \param[in]      IsrConfig  Interrupt configuration to the ISR which shall signal an end of interrupt.
 *                             Parameter must not be NULL.
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntEndOfInterrupt,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig
));


/***********************************************************************************************************************
 *  Os_Hal_IntIsrLevelIsLe()
 **********************************************************************************************************************/
/*! \brief          Returns whether the given ISR configuration has an interrupt priority level, which is lower than
 *                  or equal to the given interrupt priority level.
 *  \details        -
 *
 *  \param[in]      IsrConfig   The ISR configuration which shall be checked. Parameter must not be NULL.
 *  \param[in]      Level       The interrupt priority level to compare with. Parameter must not be NULL.
 *
 *   \retval        !0  The configured interrupt priority level of Isr is logically lower than or equal to Level.
 *  \retval         0   The configured interrupt priority level of Isr is logically greater than Level.
 *
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Supervisor mode
 *  \note           Candidate for compiler attributes OS_NOSIDEEFFECTS.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsrLevelIsLe,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) IsrConfig,
  P2CONST(Os_Hal_IntLevelType, AUTOMATIC, OS_CONST) Level
));


/***********************************************************************************************************************
 *  Os_Hal_IntLevelIsGt()
 **********************************************************************************************************************/
/*! \brief          Returns whether given interrupt priority level LevelX is logically greater than LevelY.
 *  \details        -
 *
 *
 *  \param[in]      LevelX   Left interrupt priority level. Parameter must not be NULL.
 *  \param[in]      LevelY   Right interrupt priority level. Parameter must not be NULL.
 *
 *  \retval         !0  LevelX is logically greater than LevelY.
 *  \retval         0   LevelX is logically less than or equal to LevelY.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            -
 *  \note           Candidate for compiler attributes OS_NOSIDEEFFECTS.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntLevelIsGt,
(
  P2CONST(Os_Hal_IntLevelType, AUTOMATIC, OS_CONST) LevelX,
  P2CONST(Os_Hal_IntLevelType, AUTOMATIC, OS_CONST) LevelY
));


/***********************************************************************************************************************
 *  Os_Hal_IntTrapRestore()
 **********************************************************************************************************************/
/*! \brief          Restore interrupt state which has been saved during trap.
 *  \details        This function shall restore the interrupt state which was active before the trap handler has been
 *                  triggered. The interrupt state given as parameter has been stored in \ref Os_Exc_SysCall().
 *
 *  \param[in]      InterruptState   Previous interrupt state to restore. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode.
 *  \pre            Called after trap.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntTrapRestore,
(
  P2CONST(Os_Hal_IntTrapStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));


/***********************************************************************************************************************
 *  Os_Hal_IntTrapUpdate()
 **********************************************************************************************************************/
/*! \brief          Copies the current interrupt state to the interrupt state which has been saved during trap.
 *  \details        This function is called by the trap handler. It allows modification of the interrupt state of the
 *                  trap caller. The interrupt state given as parameter has been stored in \ref Os_Exc_SysCall().
 *
 *  \param[out]     InterruptState   User space interrupt information. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode.
 *  \pre            Called after trap.
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE,
OS_ALWAYS_INLINE, Os_Hal_IntTrapUpdate,
(
  P2VAR(Os_Hal_IntTrapStateType, AUTOMATIC, OS_VAR_NOINIT) InterruptState
));


/***********************************************************************************************************************
 *  Os_Hal_IntInterruptSourceInit()
 **********************************************************************************************************************/
/*! \brief          Initialize given interrupt source so interrupt handling is possible.
 *  \details
 *  Initialization includes:
 *    - Binding of interrupt sources to cores.
 *    - Setting the interrupt priority of each configured ISR.
 *
 *  The enabled-state interrupt sources shall not be touched. (This might not be possible on some interrupt
 *  controllers.)
 *
 *  \param[in,out]  InterruptSource   Interrupt source which shall be initialized. Parameter must not be NULL.
 *
 *  \context        ANY
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Supervisor mode
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntInterruptSourceInit,
(
  P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_APPL_DATA) InterruptSource
));


/***********************************************************************************************************************
 *  Os_Hal_IntIsGlobal2LevelSwitchSupported()
 **********************************************************************************************************************/
/*! \brief        Returns whether the platform supports the global disable to level lock switch (!0) or not (0).
 *  \details      If the platform does not have global disable flag then the feature cannot be supported.
 *                Feature is supported if platform allows to disable global, switch priority level and resume global
 *                interrupt state and a pending interrupt with lower priority does not occur after this sequence.
 *
 *  \retval       !0    Supported.
 *  \retval       0     Not supported.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_IntIsGlobal2LevelSwitchSupported, (void));



/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if !defined(OS_VCAST_INSTRUMENTATION_OS_HAL)
/*VCAST_DONT_INSTRUMENT_END*/
#endif

#endif /* OS_HAL_INTERRUPTINT_H */

/* module specific MISRA deviations:
 */

/*!
 * \}
 */

/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_InterruptInt.h
 *********************************************************************************************************************/

