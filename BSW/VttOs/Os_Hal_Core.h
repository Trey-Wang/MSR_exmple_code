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
 * \addtogroup  Os_Hal_Core
 * \{
 *
 * \file
 * \brief   Core and multi-core related primitives.
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#if !defined (OS_HAL_CORE_H)                                                                                            /* PRQA S 0883 */ /* MD_Os_Hal_0883 */
# define OS_HAL_CORE_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Hal_CoreInt.h"

/* Os kernel module declarations */
# include "Os_Common.h"
# include "Os_Thread.h"
# include "Os_Core.h"
# include "Os_Hal_Context.h"
# include "Os_Common_Types.h"

/* Os HAL dependencies */
# include "Os_Hal_Compiler.h"
# include "Os_Hal_Vtt_LCfg.h"

#include <CANoeApi.h>
#include <CANoeEmuProcessor.h>
#include "Os_Hal_Debug.h"

#if !defined(OS_VCAST_INSTRUMENTATION_OS_HAL)
/*VCAST_DONT_INSTRUMENT_START*/
#endif

#if (CANOEAPI_VERSION < 223)
#error "Unsupported CANoeAPI version"
#endif

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

# define UNLOCKED (0xFF)
# define LOCKED   (0x00)

/*! Defines the value which is returned by \ref Os_Hal_SpinTryLock, if locking succeeded. */
# define OS_HAL_SPINLOCKSUCCEEDED    ((Os_Hal_SpinlockResultType)UNLOCKED)

/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

extern const Os_ThreadConfigType* CurrentThread[OS_COREID_COUNT];
extern const Os_Hal_ContextType*  CurrentContext[OS_COREID_COUNT];

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Os_Hal_CoreGetId()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_CorePhysicalIdType, OS_CODE,                                            /* PRQA S 3219 */ /* MD_Os_Hal_3219 */
OS_ALWAYS_INLINE, Os_Hal_CoreGetId,
(
  void
))
{
  return (Os_CorePhysicalIdType)CANoeEmuProcessor_GetCoreNumber();
}


/***********************************************************************************************************************
 *  Os_Hal_CoreIsThreadPointerUserReadable()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,                               /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
Os_Hal_CoreIsThreadPointerUserReadable,
(
  void
))
{
  return !0;
}

/***********************************************************************************************************************
 *  Os_Hal_CoreSetThread()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_CoreSetThread,
(
    P2CONST(Os_Hal_Core2ThreadConfigType, AUTOMATIC, OS_CONST) Core2Thread,
    P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) Thread
))
{
  CoreIdType coreId = Os_Hal_CoreGetId();

  OS_IGNORE_UNREF_PARAM(Core2Thread);

  CurrentThread[coreId] = Thread;
}


/***********************************************************************************************************************
 *  Os_Hal_CoreGetThread()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE                                                                            /* PRQA S 3219 */ /* MD_Os_Hal_3219 */
P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST), OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_CoreGetThread,
(
    P2CONST(Os_Hal_Core2ThreadConfigType, AUTOMATIC, OS_CONST) Core2Thread
))
{
  CoreIdType coreId = Os_Hal_CoreGetId();

  OS_IGNORE_UNREF_PARAM(Core2Thread);

  return CurrentThread[coreId];
}

/***********************************************************************************************************************
 *  Os_Hal_CountLeadingZeros()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE uint8, OS_CODE, OS_ALWAYS_INLINE,                                          /* PRQA S 3219 */ /* MD_Os_Hal_3219 */
Os_Hal_CountLeadingZeros,
(
    Os_Hal_BitFieldType BitField
))
{
  uint8 n;
  if (BitField == 0) return 32;
  for (n = 0; ((BitField & 0x80000000) == 0); n++, BitField <<= 1);
  return n;
}



/***********************************************************************************************************************
 *  Os_Hal_StackEndGet()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_StackEndGet,
(
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) LowAddress,
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) HighAddress
))
{
  OS_IGNORE_UNREF_PARAM(HighAddress);
  return LowAddress;
}


/***********************************************************************************************************************
 *  Os_Hal_StackBottomGet()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_StackBottomGet,
(
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) LowAddress,
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) HighAddress
))
{
  OS_IGNORE_UNREF_PARAM(LowAddress);
  return HighAddress;
}


/***********************************************************************************************************************
 *  Os_Hal_StackTopGet()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219 */ /* MD_Os_Hal_3219 */
OS_LOCAL_INLINE P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_StackTopGet,
(
  void
))
{
  return (Os_Hal_StackType*)0;
}


/***********************************************************************************************************************
 *  Os_Hal_StackPointerMoveDown()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT), OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_StackPointerMoveDown,
(
  P2VAR(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) StackPointer
))
{
  StackPointer++;
  return StackPointer;
}


/***********************************************************************************************************************
 *  Os_Hal_StackCalculateUsage()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE uint32, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_StackCalculateUsage,
(
  P2CONST(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) StackBottom,
  P2CONST(Os_Hal_StackType, AUTOMATIC, OS_VAR_NOINIT) StackTop
))
{
  return ((uint32)StackBottom - (uint32)StackTop);
}

typedef void (*voidFuncPtr)(void);

/***********************************************************************************************************************
 *  Os_Hal_CoreStart()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_3219 */
Os_Hal_CoreStart,
(
  P2CONST(Os_Hal_CoreConfigType, AUTOMATIC, OS_CONST) Core
))
{
  Os_Hal_Debug_CoreStart_Entry()

  CANoeEmuProcessor_SetStartAddr(Core->CANoeEmuCoreId, (voidFuncPtr)CORE_START_FUNC);
  CANoeEmuProcessor_StartCore(Core->CANoeEmuCoreId);                                                                    /* PRQA S 3112 */ /* MD_Os_3112 */
}


/***********************************************************************************************************************
 *  Os_Hal_CoreSynchronize()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_3219 */
Os_Hal_CoreSynchronize, (void))
{
  /* Not relevant */
}


/***********************************************************************************************************************
 *  Os_Hal_SpinInit()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_3219, MD_Os_3673_HalTemplate */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_SpinInit,
(
  P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
))
{
  *Spinlock = UNLOCKED;
}


/***********************************************************************************************************************
 *  Os_Hal_SpinIsLocked()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                   /* PRQA S 3219, 3673 */ /* MD_Os_Hal_3219, MD_Os_Hal_3673_HalTemplate */
OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_SpinIsLocked,
(
  P2CONST(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
))
{
  CANoeAPI_ConsumeTicks(1); /* prevent CANoe from detecting an infinite loop */
  return (Os_StdReturnType)(*Spinlock != UNLOCKED);
}


/***********************************************************************************************************************
 *  Os_Hal_SpinTryLock()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_3219, MD_Os_3673_HalTemplate */
OS_LOCAL_INLINE Os_Hal_SpinlockResultType, OS_CODE, OS_ALWAYS_INLINE,
Os_Hal_SpinTryLock,
(
  P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
))
{
  CANoeAPI_ConsumeTicks(1); /* prevent CANoe from detecting an infinite loop */
  return CANoeEmuProcessor_InterlockedExchange(Spinlock, LOCKED);
}


/***********************************************************************************************************************
 *  Os_Hal_SpinUnlock()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(                                                                                           /* PRQA S 3219, 3673 */ /* MD_Os_3219, MD_Os_3673_HalTemplate */
OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE, Os_Hal_SpinUnlock,
(
  P2VAR(volatile Os_Hal_SpinlockType, AUTOMATIC, OS_VAR_NOINIT) Spinlock
))
{
  *Spinlock = UNLOCKED;
}


/***********************************************************************************************************************
 *  Os_Hal_XSigInit()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_3219 */
Os_Hal_XSigInit,
(
  P2CONST(Os_Hal_XSigInterruptConfigType, AUTOMATIC, OS_CONST) XSigConfig
))
{
  OS_IGNORE_UNREF_PARAM(XSigConfig);                                                                                    /* PRQA S 3112 */ /* MD_Os_3112 */
}


/***********************************************************************************************************************
 *  Os_Hal_XSigTrigger()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_3219 */
Os_Hal_XSigTrigger,
(
  P2CONST(Os_Hal_XSigInterruptConfigType, AUTOMATIC, OS_CONST) XSigConfig
))
{
  Os_Hal_Debug_XSigTrigger_Entry();

  CANoeEmuProcessor_RequestInterruptOnCore(XSigConfig->DestinationCore, XSigConfig->InterruptSource);
}


/***********************************************************************************************************************
 *  Os_Hal_XSigAcknowledge()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_3219 */
Os_Hal_XSigAcknowledge,
(
  P2CONST(Os_Hal_XSigInterruptConfigType, AUTOMATIC, OS_CONST) XSigConfig
))
{
  OS_IGNORE_UNREF_PARAM(XSigConfig);                                                                                    /* PRQA S 3112 */ /* MD_Os_3112 */
}


/***********************************************************************************************************************
 *  Os_Hal_CoreIdle()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,     /* PRQA S 3219 */ /* MD_Os_Hal_3219 */
Os_Hal_CoreIdle, (void))
{
  Os_Hal_Debug_CoreIdle_Entry()
  CANoeEmuProcessor_SetTaskToRun(CANOEEMUPROCESSOR_IDLE);
}


/***********************************************************************************************************************
 *  Os_Hal_CoreNop()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,                                           /* PRQA S 3219 */ /* MD_Os_Hal_3219 */
Os_Hal_CoreNop, (void))
{
  CANoeAPI_ConsumeTicks(1);
}


/***********************************************************************************************************************
 *  Os_Hal_CoreGetMaxAllowedStack()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(OS_LOCAL_INLINE void, OS_CODE, OS_ALWAYS_INLINE,         /* PRQA S 3219 */ /* MD_Os_3219 */
Os_Hal_CoreGetMaxAllowedStack,
(
  P2VAR(Os_Hal_ContextStackConfigType, AUTOMATIC, OS_VAR_NOINIT) Stack
))
{
  OS_IGNORE_UNREF_PARAM(Stack);                                                       /* PRQA S 3112 */ /* MD_Os_3112 */
}


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                       /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if !defined(OS_VCAST_INSTRUMENTATION_OS_HAL)
/*VCAST_DONT_INSTRUMENT_END*/
#endif

#endif /* OS_HAL_CORE_H */

/* module specific MISRA deviations:
 */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Core.h
 **********************************************************************************************************************/

