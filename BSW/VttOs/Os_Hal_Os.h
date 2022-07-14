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
 *  \addtogroup Os_Hal_Os
 *  \{
 *
 *  \file
 *  \brief      HAL interfaces which are visible to the user.
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Anton Schmukel                visasl        Vector Informatik GmbH
 *  Da He                         visdhe        Vector Informatik GmbH
 *  Fabian Wild                   viszfa        Vector Informatik GmbH
 *  Michael Kock                  vismkk        Vector Informatik GmbH
 *  Tosten Schmidt                visto         Vector Informatik GmbH
 *  David Feuerstein              visdfe        Vector Informatik GmbH
 *  Senol Cendere                 visces        Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2016-01-05  visasl  -             Initial version
 *                        viszfa
 *  01.01.00  2016-05-09  visasl  -             Developer version (RC3)
 *                        viszfa
 *  01.01.01  2016-05-31  visasl  ESCAN00088874 Bypass interrupt state to/from service functions.
 *  01.01.07  2016-08-11  visdhe  WORKITEM8032  Improved performance
 *  01.01.08  2016-09-08  visdhe  FEAT-1931     Provide access to interrupt control register (Step2)
 *  01.02.00  2016-10-12  visdhe                Updated to sprint 11
 *  01.02.01  2016-11-14  virbse                Updated Generator to latest BSWMD version
 *  01.03.00  2016-12-02  visjth  -             Allow HRT to run with any frequency
 *                                ESCAN00092119 Set CANoeTaskFunction to Os_Hal_CANoeTaskWrapper for Hooks and (N)TFs
 *                                ESCAN00092653 Added state change handler to switch off timers during reset
 *            2016-12-02  vismkk  WORKITEM11170 Extended naming convention for HAL context interface
 *  01.04.00  2016-12-23  -       -             Identical to 1.03.00
 *  01.05.00  2017-01-12  -       -             Internal improvements
 *  02.00.00  2017-02-01  -       -             Internal improvements
 *  02.01.00  2017-03-08  -       -             Internal improvements
 *  02.02.00  2017-03-28  visto   WORKITEM14883 Keep compatibility to core generator
 *  02.03.00  2017-05-04  -       -             Internal improvements
 *  02.04.00  2017-06-01  visdfe  ESCAN00094046 Wrong validation of the maximum available MPU regions
 *  02.05.00  2017-07-03  visces  STORYC-1308   Support for cat. 0 ISRs
 *  02.06.00  2017-07-20  viszfa  -             Fixed compiler warnings
 *  02.07.00  2017-07-24  virsmn  ESCAN00094867 Compiler warning: warning #1514-D: incorrect GHS #pragma: small data
 *                                              area is not supported in this configuration
 *  02.08.00  2017-09-14  vismkk  STORYC-1658   #Core:Generator: Remove support of IOC inline interface
 **********************************************************************************************************************/

#if !defined (OS_HAL_OS_H)                                                                                              /* PRQA S 0883 */ /* MD_Os_Hal_0883 */
# define OS_HAL_OS_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Hal_OsInt.h"

/* Os kernel module dependencies */
# include "Os_Lcfg.h"

/* Os hal dependencies */


#if !defined(OS_VCAST_INSTRUMENTATION_OS_HAL)
/*VCAST_DONT_INSTRUMENT_START*/
#endif


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
/* ----- Component version information ----- */
# define OS_HAL_SW_MAJOR_VERSION                     (2u)
# define OS_HAL_SW_MINOR_VERSION                     (8u)
# define OS_HAL_SW_PATCH_VERSION                     (0u)

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

#endif /* OS_HAL_OS_H */


/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Os.h
 **********************************************************************************************************************/

