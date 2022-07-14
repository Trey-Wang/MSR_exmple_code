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
/**        \file  VTTCntrl_Det.h
 *        \brief  Initialization of Det Errors for VTT.
 *
 *      \details  In a VTT environment DET Errors are reported by using the CANoeAPI. For ease of use the Ids 
 *                of (hex values) BSW modules, functions and errors are replaced by plain text. 
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Patrick Markl                 visml         Vector Informatik GmbH
 *  Wladimir Gerber               viswge        Vector Informatik GmbH
 *  Damian Philipp                visdap        Vector Informatik GmbH
 *  Max-Ferdinand Suffel          vismsl        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2014-10-14  viswge  -             Initial version.
 *  01.00.01  2014-11-01  viswge  -             Added FrIf.
 *  01.00.02  2014-12-15  viswge  -             Added void to function signature to reduce compiler warnings.
 *  02.00.00  2015-01-07  viswge  -             FEAT254: Rename of VIP to VTT.
 *  02.00.01  2015-04-02  visdap  -             Refactoring: Moved non-det-related functionality to base
 *  02.00.02  2015-04-15  viswge  -             Added VTTCntrl_Compiler_Cfg.inc.
 *  03.00.01  2015-07-15  visdap  -             Update for R13. ESCAN00083479.
 *  03.00.02  2015-08-20  vismsl  -             Updated for R13 based on new TechRefs.
 *  04.00.00  2015-12-11  vismsl  -             Updated for R14 based on new TechRefs.
 *********************************************************************************************************************/


#if !defined (VTTCNTRL_DET_H)
#define VTTCNTRL_DET_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

#include "VTTCntrl_ModuleIncludes.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Component Version Information */
#define VTTCNTRL_DET_MAJOR_VERSION       (0x04)
#define VTTCNTRL_DET_MINOR_VERSION       (0x00)
#define VTTCNTRL_DET_PATCH_VERSION       (0x00)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
void VttCntrl_Det_OnStateChange(uint8 action, uint8 oldState, uint8 newState);

#endif /* define VTTCNTRL_DET_H */
