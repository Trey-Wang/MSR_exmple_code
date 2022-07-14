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
 * \ingroup      Os_Hal_Compiler
 * \{
 *
 * \file
 * \brief        Compiler abstraction template file.
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#if !defined(OS_HAL_COMPILER_MICROSOFT_H)                                                                                              /* PRQA S 0883 */ /* MD_Os_0883 */
# define OS_HAL_COMPILER_MICROSOFT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
# include "Std_Types.h"

#if !defined(OS_VCAST_INSTRUMENTATION_OS_HAL)
/*VCAST_DONT_INSTRUMENT_START*/
#endif


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/*! \brief    Force the compiler to inline a function.
 *  \details
 *  This attribute forces the compiler to inline the specified function, regardless of the optimization strategy of the
 *  compiler itself.
 *  On some compilers you may be able to disable even this kind of inlining, using debugging options.
 *
 *  Availability: GCC ('always_inline'), Diab, Tasking
 */
# define OS_ALWAYS_INLINE


/*! \brief    This attribute tells the compiler that a function never returns.
 *  \details
 *  It can then optimize without regard to what would happen if the function ever did return.
 *  This makes slightly better code. More importantly, it helps avoid spurious warnings of uninitialized variables.
 *
 *  Availability: GCC ('noreturn'), Diab, Tasking, Green Hills
 */
# define OS_NORETURN	__declspec(noreturn)


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/
/*! \brief      Declares a function with a compiler specific attribute.
 *  \details    The implementations prototype has to be created with OS_FUNC_ATTRIBUTE_DEFINITION.
 *
 *  \param[in]  rettype       The return type of the function.
 *  \param[in]  memclass      The memory class of the function.
 *  \param[in]  attribute     The function attribute {OS_ALWAYS_INLINE; OS_PURE, ...}.
 *  \param[in]  functionName  The function name.
 *  \param[in]  arguments     The function arguments in brackets. E.g. (uint8 a, uint16 b) or (void).
 *
 *  \note       Prevent multiple usage of memclass in case of pointer return type.
 */
#define OS_FUNC_ATTRIBUTE_DECLARATION(rettype, memclass, attribute, functionName, arguments)                           \
      attribute FUNC(rettype, memclass) functionName arguments                                                           /* PRQA S 3410, 3453 */ /* MD_Os_3410, MD_Os_3453 */


/*! \brief      Provides a function prototype for function definitions with a compiler specific attribute.
 *  \details    The declaration has to be created with OS_FUNC_ATTRIBUTE_DECLARE.
 *
 *  \param[in]  rettype       The return type of the function.
 *  \param[in]  memclass      The memory class of the function.
 *  \param[in]  attribute     The function attribute {OS_ALWAYS_INLINE; OS_PURE, ...}.
 *  \param[in]  functionName  The function name.
 *  \param[in]  arguments     The function arguments in brackets. E.g. (uint8 a, uint16 b) or (void).
 *
 *  \note       Prevent multiple usage of memclass in case of pointer return type.
 */
#define OS_FUNC_ATTRIBUTE_DEFINITION(rettype, memclass, attribute, functionName, arguments)                            \
      FUNC(rettype, memclass)  functionName arguments                                                                   /* PRQA S 3410, 3453 */ /* MD_Os_3410, MD_Os_3453 */



/*! \brief    Tells the compiler that the conditional statement is likely to be true.
 *  \details
 *  The compiler, in turn, can then perform block reordering, and other optimization to
 *  improve the performance of conditional branches.
 *
 *  Availability: GCC ('__builtin_expect'), Diab
 *
 *  \param[in]  CONDITION   The condition which is likely to be true
 */
# define OS_LIKELY(CONDITION)           (CONDITION)                                                                     /* PRQA S 3453 */ /* MD_Os_3453 */

/*! \brief    Tells the compiler that the conditional statement is likely to be false.
 *  \details
 *  The compiler, in turn, can then perform block reordering, and other optimization to
 *  improve the performance of conditional branches.
 *
 *  Availability: GCC ('__builtin_expect'), Diab
 *
 *  \param[in]  CONDITION   The condition which is likely to be false
 */
# define OS_UNLIKELY(CONDITION)         (CONDITION)                                                                     /* PRQA S 3453 */ /* MD_Os_3453 */




/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#if !defined(OS_VCAST_INSTRUMENTATION_OS_HAL)
/*VCAST_DONT_INSTRUMENT_END*/
#endif

#endif /* OS_HAL_COMPILER_MICROSOFT_H */

/*!
 * \}
 */
/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Compiler_Microsoft.h
 **********************************************************************************************************************/
