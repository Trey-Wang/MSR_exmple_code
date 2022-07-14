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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Rtm_Types.h
 *    Component:  -
 *       Module:  RTM
 *    Generator:  -
 *
 *  Description:  RTM static type definition header file.
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Alexander Zeeb                visazb        Vector Informatik GmbH
 *  Oliver Reineke                visore        Vector Informatik GmbH
 *  David Zentner                 viszda        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2013-04-22  visazb  -             Initial version
 *  01.00.01  2013-08-06  visore  ESCAN00068379 No changes here
 *            2013-08-06  visore  ESCAN00069628 No changes here
 *            2013-08-06  visore  ESCAN00069629 No changes here
 *            2013-09-19  visore  ESCAN00070505 No changes here
 *            2013-09-19  visore  ESCAN00070506 No changes here
 *  01.01.00  2013-11-26  visore  ESCAN00070889 No changes here
 *  01.01.01  2014-05-21  visore  ESCAN00075552 No changes here
 *            2014-05-21  visore  ESCAN00075805 No changes here
 *            2014-05-21  visore  ESCAN00075806 No changes here
 *  01.01.02  2014-07-07  visore  ESCAN00076777 No changes here
 *            2014-07-07  visore  ESCAN00076778 No changes here
 *            2014-07-07  visore  ESCAN00076779 No changes here
 *  01.02.00  2014-07-08  viszda  ESCAN00076464 AR4-817: Runtime optimization of the RTM measurement functions
 *            2014-07-09  viszda  ESCAN00076861 No changes here
 *            2014-07-23  viszda  ESCAN00077300 No changes here
 *            2014-07-29  viszda  ESCAN00077455 No changes here
 *            2014-07-29  viszda  ESCAN00076463 AR4-791: Serial and parallel measurement of CPU load
 *            2014-10-20  viszda  ESCAN00077119 No changes here
 *            2014-10-20  viszda  ESCAN00078841 Calculated CPU Load is wrong
 *            2014-10-24  viszda  ESCAN00079113 No changes here
 *            2014-11-06  viszda  ESCAN00077452 No changes here
 *  01.02.01  2014-11-27  viszda  ESCAN00079842 No changes here
 *            2014-11-27  viszda  ESCAN00079844 The current result of CPU load measurement should be available for CPU load control mode C_API
 *  01.02.02  2015-06-25  viszda  ESCAN00080430 No changes here
 *            2015-07-01  viszda  ESCAN00083746 No changes here
 *  02.00.00  2015-10-01  viszda  ESCAN00085574 FEAT-921: Multicore support for RTM Measurement Points [AR4-816]
 *            2015-10-01  viszda  ESCAN00083646 Support of 32bit timer
 *            2015-10-01  viszda  ESCAN00085572 Optimization of Net Runtime implementation
 *  2.00.01   2016-02-25  viszda  ESCAN00087879 DET occurs during start up of RTM
 *  2.00.02   2016-04-05  viszda  ESCAN00089228 No changes here
 *  2.01.00   2016-04-15  viszda  ESCAN00089508 No changes here
 *            2016-05-17  viszda  ESCAN00089265 No changes here
 *            2016-04-07  viszda  ESCAN00090816 No changes here
 *  2.02.00   2016-09-14  viszda  ESCAN00091859 FEAT-2055: SafeRTM (Disable RTM safely)
 *  3.00.00   2017-04-03  viszda  STORYC-724    [AMD] Support net runtime measurement with RTM in Gen7 OS
 *            2017-04-03  viszda  ESCAN00087435 Rework RTM implementation structure and comments
 *            2017-04-03  viszda  ESCAN00080428 Correction of formal issues
 *            2017-04-05  viszda  ESCAN00093290 Wrong measurement results in case of execution time measurement
 *            2017-04-05  viszda  ESCAN00085578 Add DET checks to RTM's APIs
 *            2017-04-05  viszda  ESCAN00094525 Net runtime measurement deviates to a large extent from real runtime in special cases
 *            2017-04-07  viszda  ESCAN00094694 Add missing initialization of Rtm_DisableInterruptFlag in Rtm_InitMemory
 *  3.01.00   2017-04-19  viszda  STORYC-671    Rtm: reduce RTM measurement overhead.
 *  3.01.01   2017-06-09  viszda  ESCAN00095484 DET occurs before second initialization phase, no measurement possible
 *********************************************************************************************************************/

#ifndef RTM_TYPES_H_
# define RTM_TYPES_H_

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

# include "ComStack_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ESCAN00078841 */
typedef uint32 Rtm_MeasurementTimestampType;

typedef struct
{
  uint32 time; /* PRQA S 5115 */ /* MD_Rtm_5115 */
  uint32 count;
  Rtm_MeasurementTimestampType min;
  Rtm_MeasurementTimestampType max;
} Rtm_DataSet;

typedef struct
{
  uint32 execTime_Nested_EnableISRs;
  uint32 execTime_Nested_DisableISRs;
} Rtm_OverheadCorrectionType;

/* ESCAN00079844 */
typedef enum
{
  RTM_ITEM_CPU_LOAD_AVERAGE  = 0u, /* The average result of CPU load measurement. */
  RTM_ITEM_CPU_LOAD_CURRENT  = 1u, /* The result of the latest CPU load measurement. */
  RTM_ITEM_MIN               = 2u, /* The minimum measurement time. */
  RTM_ITEM_MAX               = 3u  /* The maximum measurement time. */
} Rtm_ItemType;                    /* This type describes the requested measurement item type. */

typedef void (*Rtm_ThresholdCbkFctType) (Rtm_MeasurementTimestampType);

#endif /* RTM_TYPES_H_ */
