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
 *  \defgroup   Os_Hal_Compiler  HAL Compiler
 *  \brief      Abstraction of compiler specific keywords to allow the compiler to generate more efficient code.
 *  \details
 *  The kernel should be able to give the compiler meta information to generate more efficient code.
 *  It also needs to tell the linker where to put data and code to implement Memory Protection.
 *  In multi core systems there may be the need for data/code duplication.
 *  The keywords to provide this information are not part of ANSI-C and are highly compiler dependent.
 *
 *  AUTOSAR already defines concepts to abstract compiler specifics:
 *  - [AUTOSAR Compiler Abstraction](http://www.autosar.org/fileadmin/files/releases/4-2/software-architecture/implementation-integration/standard/AUTOSAR_SWS_CompilerAbstraction.pdf)
 *  - [AUTOSAR Memory Mapping](http://www.autosar.org/fileadmin/files/releases/3-2/software-architecture/implementation-integration/standard/AUTOSAR_SWS_MemoryMapping.pdf)
 *
 *  They are used in MICROSAR OS. This module defines additional compiler abstraction keywords for MICROSAR OS.
 *  They are relevant to provide OS functionality or improve efficiency.
 *
 *  \{
 *
 *  \file
 *  \brief      Additional services defined here are available to the user.
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#if !defined (OS_HAL_COMPILERINT_H)                                                                                     /* PRQA S 0883 */ /* MD_Os_Hal_0883 */
# define OS_HAL_COMPILERINT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */

/* Os kernel module dependencies */
# include "Os_Cfg.h"
# include "Os_CommonInt.h"

/* Os hal dependencies */


#if !defined(OS_VCAST_INSTRUMENTATION_OS_HAL)
/*VCAST_DONT_INSTRUMENT_START*/
#endif

#ifdef CDK_CHECK_MISRA
# ifndef OS_STATIC_CODE_ANALYSIS
#   define OS_STATIC_CODE_ANALYSIS
# endif
#endif

#include "Os_Hal_Compiler_Microsoft.h"

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
# if !defined (OS_ALWAYS_INLINE)
#  error "OS_ALWAYS_INLINE is undefined!"
# endif


# if !defined (OS_NORETURN)
#  error "OS_NORETURN is undefined!"
# endif


# if !defined (OS_LIKELY)
#  error "OS_LIKELY(Condition) undefined"
# endif


# if !defined (OS_UNLIKELY)
#  error "OS_UNLIKELY(Condition) undefined"
# endif

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

#if !defined(OS_VCAST_INSTRUMENTATION_OS_HAL)
/*VCAST_DONT_INSTRUMENT_END*/
#endif

#endif /* OS_HAL_COMPILERINT_H */

/* module specific MISRA deviations:
 MD_Os_Hal_0883:
      Reason:     The protection against repeated inclusion is complete and correct. The MISRA tool is unable to
                  detect this.
      Risk:       None.
      Prevention: None.
 MD_Os_Hal_3112:
      Reason:     This statement is used to avoid warnings caused by unused parameters. Parameters are defined by
                  standardized API requirements, and not needed in case a feature is disabled by configuration.
                  It is either impossible due to API standardization or necessary code duplication (severe maintenance
                  increase) to provide feature dependent APIs with different signatures.
      Risk:       Unavoidable compiler warning or error because of either unused statements or unused parameter.
      Prevention: None.
 MD_Os_Hal_3219:
      Reason:     This function is inlined and therefore it has to be implemented here. The function is not used
                  by all implementation files which include this header file.
      Risk:       This function may not be used by any implementation file.
      Prevention: The reviewer has to ensure the each static inline function is used at leased once.
 MD_Os_Hal_3453:
      Reason:     The purpose of this macro, is to place the given elements in the correct order.
                  A function cannot be used.
      Risk:       None.
      Prevention: None.
 MD_Os_Hal_3453_Decl:
      Reason:     Macro is used for declaration, which is not possible within a function.
      Risk:       None, type errors will be detected by the compiler.
      Prevention: None.
 MD_Os_Hal_3673_HalTemplate:
      Reason:     This function is a template function and has to be implemented hardware specifically. Therefore the
                  out parameters are unused.
      Risk:       None.
      Prevention: None.
 */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_CompilerInt.h
 **********************************************************************************************************************/

