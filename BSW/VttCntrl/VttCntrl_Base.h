/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**       \file    VTTCntrl_Base.h
 *        \brief  Declares the public API of the vVIRTUALtarget base functionalities.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
*  AUTHOR IDENTITY
*  -------------------------------------------------------------------------------------------------------------------
*  Name                          Initials      Company
*  -------------------------------------------------------------------------------------------------------------------
*  Wladimir Gerber                viswge       Vector Informatik GmbH
*  Damian Philipp                 visdap       Vector Informatik GmbH
*  Max-Ferdinand Suffel        vismsl     Vector Informatik GmbH
*  -------------------------------------------------------------------------------------------------------------------
*  REVISION HISTORY
*  -------------------------------------------------------------------------------------------------------------------
*  Version   Date        Author  Change Id     Description
*  -------------------------------------------------------------------------------------------------------------------
*  02.00.01  2015-04-02  visdap    -           Initial version.
*  02.00.02  2015-04-15  viswge    -           Added VTTCntrl_Compiler_Cfg.inc.
*  02.00.05  2015-05-08  visdap    -           Updated function names
*  02.00.07  2015-06-02  visdap    -           VttCntrl_Base_MCALInitMemory
*  02.00.08  2015-06-10  visdap    -           Adapt to new set of ISRs in VTTOs
*  02.00.09  2015-06-11  visdap    -           ESCAN00083322: Add support for VttCntrl_Base_UserCallout_OnStateChange
*  03.00.01  2015-07-31  visdap    -           Removed dependency on WinAPI (sprintf_s)
*  03.00.04  2015-09-22  visdap    -           ESCAN00085394: Added pragmas to protect VTTCntrl during simulated
*                                              ECU reset
*  03.01.00  2015-11-06  visdap    -           Renamed defines to include VTT prefix
*  04.00.00  2015-09-22  vismsl    -           Added wrapper for CANoeAPI_BaseAddressOfDLL used in XCP.
*            2015-11-18  visdap    -           Interrupts are suppressed if no VTT OS is available.
*            2015-12-18  vismsl    -           Check if interrupt service routine is present when requesting an
*                                              interrupt is done by VTT OS now.
*  04.00.01  2016-01-08  vismsl  ESCAN00087533 Cleanup of VttCntrl codebase: Refactoring and additional documentation.
*  08.00.01  2017-04-12  visjth  ESCAN00094762: Added functions VttCntrl_Base_MaskInterrupt VttCntrl_Base_UnmaskInterrupt
*********************************************************************************************************************/

#if !defined (VTTCNTRL_BASE_H)
#define VTTCNTRL_BASE_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

#include "VttCntrl_ModuleIncludes.h"
#include "VttCntrl_Cfg.h"

/**********************************************************************************************************************
*  GLOBAL CONSTANT MACROS
*********************************************************************************************************************/

/* ----- Component version information ----- */
#define VTTCNTRL_SW_MAJOR_VERSION (9u)
#define VTTCNTRL_SW_MINOR_VERSION (0u)
#define VTTCNTRL_SW_PATCH_VERSION (0u)

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL DATA TYPES AND STRUCTURES
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL DATA PROTOTYPES
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL FUNCTION PROTOTYPES
*********************************************************************************************************************/

void VttCntrl_Base_SetCheckedInterrupt(sint32 irq);
void VttCntrl_Base_MaskInterrupt(sint32 irq);
void VttCntrl_Base_UnmaskInterrupt(sint32 irq);

uint32 VttCntrl_Base_GetBaseAddressOfDll(void);

void VttCntrl_Base_OnStateChange(uint8 action, uint8 oldState, uint8 newState);

#endif /* define VTTCNTRL_BASE_H */

/**********************************************************************************************************************
*  END OF FILE: VTTCntrl_Base.h
*********************************************************************************************************************/
