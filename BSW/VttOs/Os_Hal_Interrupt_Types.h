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
 *  \addtogroup Os_Hal_Interrupt
 *  \{
 *
 *  \file
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#if !defined (OS_HAL_INTERRUPT_TYPES_H)                                                                                 /* PRQA S 0883 */ /* MD_Os_Hal_0883 */
# define OS_HAL_INTERRUPT_TYPES_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */

/* Os kernel module dependencies */

/* Os Hal dependencies */


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

 /*! \brief      Disable all interrupt type.
 *  \details    Specifies whether disabling all interrupts is implemented by level or by flag.
 */
typedef enum
{
  OS_HAL_INT_DISABLE_ALL_BY_FLAG = 0, /*!< Disabling all interrupts is implemented by global disable flag. */
  OS_HAL_INT_DISABLE_ALL_BY_LEVEL = 1 /*!< Disabling all interrupts is implemented by level. */
} Os_Hal_IntDisableAllType;

/*! \brief Interrupt state
 *  \details  The kernel uses this data type to suspend/resume interrupts, to protect OS internal critical sections.
 *            VTT does not support a global disable flag. It always locks up to a certain level.
 */
typedef sint32 Os_Hal_IntGlobalStateType;

/*! \brief    Interrupt state.
 *  \details  The kernel uses this data type to suspend/resume interrupts, to protect OS internal critical sections.
 *            VTT does not support a global disable flag. It always locks up to a certain level.
 */
typedef sint32 Os_Hal_IntStateType;

/*! \brief    Interrupt level.
 *  \details  The kernel uses this data type to suspend/resume category 2 ISRs up to a certain level.
 *            This data type is needed to implement interrupt resources.
 */
typedef sint32 Os_Hal_IntLevelType;

/*! \brief    Interrupt state (with trap information).
 *  \details  The kernel uses this data type to restore the caller's interrupt state on OS service requests.
 */
struct Os_Hal_IntTrapStateType_Tag;
typedef struct Os_Hal_IntTrapStateType_Tag Os_Hal_IntTrapStateType;

struct Os_Hal_IntIsrConfigType_Tag;
/*! Hardware specific configuration for category 2 ISRs. */
typedef struct Os_Hal_IntIsrConfigType_Tag Os_Hal_IntIsrConfigType;

/*! CANoe Interrupt Routing Mask */
typedef uint32 Os_Hal_IntRoutingMaskType;

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/


#if !defined(OS_VCAST_INSTRUMENTATION_OS_HAL)
/*VCAST_DONT_INSTRUMENT_END*/
#endif

#endif /* OS_HAL_INTERRUPT_TYPES_H */

/* module specific MISRA deviations:
 */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Interrupt_Types.h
 **********************************************************************************************************************/

