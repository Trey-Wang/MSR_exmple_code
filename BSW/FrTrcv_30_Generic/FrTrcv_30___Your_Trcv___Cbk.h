
/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  FrTrcv_30___Your_Trcv___Cbk.h
 *    Component:  MICROSAR FR Transceiver Driver
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  FlexRay transceiver driver implementation, according to:
 *                AUTOSAR FlexRay Transceiver Driver, AUTOSAR Release 3.0
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * ATTENTION: Replace the placeholders __Your_Trcv__ and __YOUR_TRCV__ with the according name of the used transceiver.
 *            __YOUR_TRCV__ is used for definitions in upper case.
 *            __Your_Trcv__ is used for variables in camel case.
 * EXAMPLE:   Replace __YOUR_TRCV__ to TJA1080 and __Your_Trcv__ to Tja1080.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Andreas Herkommer             hr            Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  1.00.00   2010-01-02  hr      -             Creation
 *            2010-04-21  hr      ESCAN00041515 Move FrTrcv_TrcvModeType and FrTrcv_TrcvWUReasonType to Fr_GeneralTypes.h
 *  1.01.00   2011-01-11  hr      ESCAN00047305 Adapt MainFunction for usage with IdentityManagerConfig
 *            2011-01-31  hr      ESCAN00048304 Compile error because of unknown Memmap sections
 *                                ESCAN00048376 Compiler warning in the FR transceiver implementation
 *                                ESCAN00048396 Compile error for the exclusive areas
 *  1.01.01   2011-02-17  hr      ESCAN00048742 Missing START_SEC_CONST_32BIT in FrTrcv_30___Your_Trcv___MemMap.inc
 *  1.01.02   2011-03-02  hr      ESCAN00049012 "Dem_"  prefix is missing for FRTRCV_E_FR_NO_TRCV_CONTROL
 *            2011-05-04  hr      ESCAN00049932 Wakeup detection is checked even though the option is not enabled in GENy
 *  1.01.03   2012-01-09  hr      ESCAN00053416 AR3-2069: Remove non-SchM code for critical section handling
 *  1.01.04   2013-11-12  hr      ESCAN00071791 The MISRA justifications are not implemented according WI_MISRAC2004_PES.pdf
 *  2.00.00   2014-01-13  hr      ESCAN00073038 Critical Section not left if SetTransceiverMode called with invalid mode
 *                                ESCAN00073041 EcuM_SetWakeupEvent call with active interrupt locks
 *                                ESCAN00072928 Support ASR4
 *  2.00.01   2015-05-19  hr      ESCAN00083046 Compile error: DEM error constant renamed for ASR4
 *********************************************************************************************************************/
#ifndef FRTRCV_30___YOUR_TRCV___CBK_H
#define FRTRCV_30___YOUR_TRCV___CBK_H

#include "FrTrcv_30___Your_Trcv__.h"

#if ( FRTRCV_30_GENERIC_MEM_MAPPING == STD_ON )
  #define FRTRCV_30___YOUR_TRCV___START_SEC_CODE
  #include "MemMap.h"
#endif

/* wakeup notification */
extern FUNC(void, FRTRCV_30___YOUR_TRCV___CODE) FrTrcv_30___Your_Trcv___Cbk_WakeupByTransceiver(uint8 FrTrcv_TrcvIdx);

#if (FRTRCV_30___YOUR_TRCV___USE_TIMERS == STD_ON)
extern FUNC(void, FRTRCV_30___YOUR_TRCV___APPL_CODE) Appl_FrTrcv_30___Your_Trcv___Wait(uint8 TimerIndex);
#else
#define Appl_FrTrcv_30___Your_Trcv___Wait(delay)
#endif

# if (FRTRCV_30___YOUR_TRCV___USE_ICU == STD_ON)
/* overwrite the ICU channel proprieties, this ICU channel is used if the platform does not support wakeup by bus */
extern FUNC(void, FRTRCV_30___YOUR_TRCV___APPL_CODE) Appl_FrTrcv_30___Your_Trcv___EnableIcuNotification(uint8 FrTrcv_TrcvIdx);
extern FUNC(void, FRTRCV_30___YOUR_TRCV___APPL_CODE) Appl_FrTrcv_30___Your_Trcv___DisableIcuNotification(uint8 FrTrcv_TrcvIdx);
# endif

#if ( FRTRCV_30_GENERIC_MEM_MAPPING == STD_ON )
  #define FRTRCV_30___YOUR_TRCV___STOP_SEC_CODE
  #include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

#endif
  /* FRTRCV_30___YOUR_TRCV___CBK_H */
