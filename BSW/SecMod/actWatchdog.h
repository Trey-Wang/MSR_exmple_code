/****************************************************************************
 ** Main author: MNS                     Creation date: 01/28/04
 ** $Author:: alf                      $ $Date:: 2008-01-16 18:18:19 +010#$
 ** $Workfile:: actWatchdog.h          $ $Revision:: 1414                 $
 ** $NoKeywords::                                                         $
 **
 **
 ** \copyright(cv cryptovision GmbH, 1999 - 2006                          )
 **
 ** \version(1.0                                                          )
 ***************************************************************************/

/****************************************************************************
 **
 **     Part of the actCLibrary
 **
 **     Layer: Core Module - Watchdog
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** This file contains:
 **   Watchdog functionality
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **
 ***************************************************************************/

#ifndef __ACTWATCHDOG_H
#define __ACTWATCHDOG_H

#include "actConfig.h"


/****************************************************************************
 ** Macros
 ***************************************************************************/

#ifndef actWD_LEVEL
   #error  actWD_LEVEL is not defined in actConfig.h
#endif

#if (actWD_LEVEL > 0)
   #define actTriggerWD(watchdog)   if (watchdog) (*watchdog) ()
   #define actL1trigger(watchdog)   actTriggerWD(watchdog)
#else
   #define actTriggerWD(watchdog)
   #define actL1trigger(watchdog)
#endif

#if (actWD_LEVEL > 1)
   #define actL2trigger(watchdog)   actTriggerWD(watchdog)
#else
   #define actL2trigger(watchdog)
#endif

#if (actWD_LEVEL > 2)
   #define actL3trigger(watchdog)   actTriggerWD(watchdog)
#else
   #define actL3trigger(watchdog)
#endif

/* additional WD macros */
#if (actWD_LEVEL >= 1)
   #define ifWDisL1(condition, watchdog)  if (watchdog && (condition)) (*watchdog) ()
#else
   #define ifWDisL1(condition, watchdog)
#endif

#if (actWD_LEVEL >= 2)
   #define ifWDisL2(condition, watchdog)  if (watchdog && (condition)) (*watchdog) ()
#else
   #define ifWDisL2(condition, watchdog)
#endif

#if (actWD_LEVEL >= 3)
   #define ifWDisL3(condition, watchdog)  if (watchdog && (condition)) (*watchdog) ()
#else
   #define ifWDisL3(condition, watchdog)
#endif


#endif /* __ACTWATCHDOG_H */
