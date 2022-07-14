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
/**      \file    VTTCntrl_SysVar.h
*        \brief   Declares the public API of vVIRTUALtarget for handling user-defined CANoe system variables.
*
*      \details   Each configured CANoe system variable may trigger an notification function if value changes. 
*
*********************************************************************************************************************/
 
/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Damian Philipp                 visdap       Vector Informatik GmbH
 *  Max-Ferdinand Suffel       vismsl    Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  03.00.05  2015-10-01  visdap  -             Initial Version
 *  03.00.06  2016-01-18  vismsl  ESCAN00087533 Cleanup of VttCntrl codebase: Refactoring and additional documentation.
*********************************************************************************************************************/

#if !defined (VTTCNTRL_SYSVAR_H)
#define VTTCNTRL_SYSVAR_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

#include "VttCntrl_Base.h"
#include "VttCntrl_SysVar_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Component version information ----- */
#define VTTCNTRL_SYSVAR_MAJOR_VERSION (3u)
#define VTTCNTRL_SYSVAR_MINOR_VERSION (0u)
#define VTTCNTRL_SYSVAR_PATCH_VERSION (6u)

/*! \brief IRQ assigned to ISR triggered when user-defined CANoe system variables have pending changes. */
#define VttSysVar_IRQ  IRQ_VttSysVarIsr_0

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL DATA TYPES AND STRUCTURES
*********************************************************************************************************************/

/*! \brief Internal structure associated with each configured user-defined CANoe system variable. */
typedef struct
{
  sint32 handle;
  void(*notification) (void);
  uint8 hasChange;
  uint32 typeId;
} VttSysVar_CntrlType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

void VttSysVarIsr_0(void);

void VttCntrl_SysVar_OnStateChange(uint8 action, uint8 oldState, uint8 newState);

#endif /* VTTCNTRL_SYSVAR_H */

/**********************************************************************************************************************
*  END OF FILE: VTTCntrl_SysVar.h
*********************************************************************************************************************/
