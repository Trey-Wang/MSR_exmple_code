
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
 *           File:  Lin_Types.h
 *      Component:  AUTOSAR LIN Driver
 *         Module:  DrvLin_VttCanoeAsr / DrvLin_CanoeemuAsr
 *      Generator:  DrvLin_VttCanoeAsr / DrvLin_CanoeemuAsr
 *
 *  Target system:  Canoe
 *       Compiler:  Ansi

 *    Derivatives:  VTT/EMU
 *
 *    Description:  Type definitions of the AUTOSAR LIN Driver
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Lutz Pflueger                 vislpr        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  05.00.00  2013-08-12  vislpr  -             Port Canoeemu to VIP
 *  05.01.00  2014-07-17  vislpr  -             Atomic access to VipCntrl, bugfixing
 *  06.00.00  2014-09-29  vislpr  -             Add Canoeemu to VIP, ComStackLib, R11, PB-S
 *  06.00.01  2014-11-20  vislpr  -             HL Part was updated
 *  07.00.00  2015-01-29  vislpr  ESCAN00080255 FEAT-254: Globale Umbennenung von VIP nach VTT
 *                        vislpr  ESCAN00081345 FEAT-427: SafeBSW Step I
 *  07.01.00  2015-06-15  vislpr  ESCAN00083109 VTT Lin shall derive its interrupt offset from the values configured in the OS
 *  07.02.00  2015-06-15  vislpr  -             Remove EMU and HL part dependencies 
 *  07.03.00  2016-04-15  vislpr  -             HL Part was updated
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Friedrich Kiesel              visfki        Vector Informatik GmbH
 *  Lutz Pflueger                 vislpr        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY - CORE -
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  04.00.00  2012-07-20  visfki  ESCAN00059300 Implementation of High Level AUTOSAR 4 LIN Driver
 *  04.01.00  2012-11-20  visfki  ESCAN00061782 AR4-220: Remove STATIC
 *                                ESCAN00063147 Rename Lin_WakeUp to Lin_Wakeup
 *  04.02.00  2013-02-08  visfki  ESCAN00064956 AR4-325: Implement Post-Build Loadable
 *                                ESCAN00065149 Compiler error: LIN_CH_OPERATIONAL changed in ASR4 to LIN_OPERATIONAL
 *  05.00.00  2013-09-19  vislpr  ESCAN00066851 Improve include structure for EcuM headers
 *                                ESCAN00079533 AR4-260: Remove ProdErrorDetection configuration switch
 *                                ESCAN00069952 Justify MISRA Warnings on functions
 *                                ESCAN00067708 AR4-328: Predefined Runtime Measurement Points
 *  05.00.01  2013-11-19  vislpr  ESCAN00071647 Remove MISRA warning 0850/3453
 *                                ESCAN00072030 Change RTM macros
 *  05.00.02  2013-11-21  vislpr  ESCAN00072030 Fix MISRA Warnings and GNU Compiler error on RTM
 *  06.00.00  2014-10-01  vislpr  ESCAN00076482 AR4-832: External wake up via LIN transceiver
 *                                ESCAN00078696 AR4-698: Post-Build Selectable (Identity Manager)
 *  06.00.01  2014-11-14  vislpr  ESCAN00079471 Compiler error: lin.c: 'Lin_Dev_InitDetect'/'LIN_DEV_INITVALUE'
 *  06.00.02  2014-11-14  vislpr  ESCAN00079588 Compiler error: Identifier Channel not declared
 *  06.00.03  2014-12-02  vislpr  -             Change wakeup handling
 *  07.00.00  2014-12-12  vislpr  ESCAN00080307 FEAT-427: SafeBSW Step I
 *  07.00.01  2015-01-27  vislpr  ESCAN00080823 Add channel DET check for out of bounds of channel id and channel idx
 *                        vislpr  ESCAN00080824 Use LOCAL_INLINE macro of Compiler.h instead "static inline"
 *  07.00.02  2015-03-26  vislpr  ESCAN00082080 Remove redundant code in Lin_GoToSleep
 *  07.00.03  2015-05-26  vislpr  ESCAN00083154 Dem.h not included if Dem repoting enabled
 *                        vislpr  ESCAN00083156 EcuM_BswErrorHook() are used on precompile with Selectable enabled
 *  07.01.00  2015-07-13  vislpr  ESCAN00081794 FEAT-1275: SafeBSW Step 2
 *  07.01.01  2015-09-15  vislpr  ESCAN00085267 Unkown return value 0x01u of Lin_GetStatus() in case of LIN_TX_BUSY error
 *  07.01.02  2015-10-23  vislpr  ESCAN00086056 Check data length parameter of Lin_SendFrame()
 *  07.02.00  2015-11-04  vislpr  ESCAN00086254 Unification the register base address struct and typedef for each driver
 *            2015-11-04  vislpr  ESCAN00086267 Code refactoring
 *            2015-11-16  vislpr  ESCAN00086493 Remove Lin_GetRxPin() Macro.
 *            2015-11-16  vislpr  ESCAN00086494 Do not set the state to wake after calling Lin_CheckWakeup()
 *            2015-11-17  vislpr  ESCAN00086546 Inconsistency of wakeup support between Lin, LinIf and EcuM in variant Post Build
 *  07.03.00  2016-01-27  vislpr  ESCAN00087812 FEAT-1688: SafeBSW Step 4
 *  07.03.01  2016-03-16  vislpr  -             SafeBSW review rework
 *  07.04.00  2016-04-13  vislpr  -             SafeBSW review rework (ISR)
 *********************************************************************************************************************/

#if !defined (LIN_TYPES_H)
#define LIN_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/* include ComStack_Types.h so all needed types are known */
#include "ComStack_Types.h"
#include "Lin_GeneralTypes.h"
#include "EcuM_Generated_Types.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

#define LIN_TPS_MAJOR_VERSION 7
#define LIN_TPS_MINOR_VERSION 3
#define LIN_TPS_PATCH_VERSION 0

/* Compatibility defines */


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef volatile struct Lin_RegisterStructTag /*!< Hardware register layout */
{
uint8 dummy;
} Lin_RegisterStruct;
 

/**********************************************************************************************************************
 *  TYPE AND DEFINE DEFINITIONS FOR INTERNAL USE IN LIN DRIVER
 *********************************************************************************************************************/
/*! Pointer to hardware register base address of channel. */
typedef Lin_RegisterStruct* Lin_RegisterStructPtr;
 
 
#define Lin_ChannelDataBufferSize             0x08u
typedef uint8 Lin_DataBufferType[Lin_ChannelDataBufferSize];

typedef uint8 Lin_ChannelConfigIdxOfChannelHwType;



#endif /* LIN_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: Lin_Types.h
 *********************************************************************************************************************/

