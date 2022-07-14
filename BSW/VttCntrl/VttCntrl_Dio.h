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
/**      \file    VTTCntrl_Dio.h
*        \brief   Declares the public API of the virtual DIO controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual DIO controller that enables to read/write single IO ports or complete groups at once.
*
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Max-Ferdinand Suffel           vismsl       Vector Informatik GmbH
 *  Damian Philipp           visdap    Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  03.00.01  2015-07-24  vismsl  -             Initial version extracted from VTTCntrl generator.
 *  03.00.04  2015-09-22  visdap  ESCAN00085394 Added pragmas to protect VTTCntrl during simulated ECU reset.
 *  03.00.05  2016-01-22  vismsl  ESCAN00087533 Cleanup of VttCntrl codebase: Refactoring and additional documentation.
 *  03.00.06  2016-03-21  vismsl  ESCAN00089004 External declaration of VttDio_MapChannel moved to VttCntrl_Dio_Cfg.h.
*********************************************************************************************************************/

#if !defined (VTTCNTRL_DIO_H)
# define VTTCNTRL_DIO_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
 
#include "Dio.h"
#include "VttCntrl_Base.h"
#include "VTTCntrl_Dio_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Component version information ----- */
# define VTTDIO_SW_MAJOR_VERSION (3u)
# define VTTDIO_SW_MINOR_VERSION (0u)
# define VTTDIO_SW_PATCH_VERSION (6u)

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL DATA TYPES AND STRUCTURES
*********************************************************************************************************************/
 
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

uint32 VttDio_ReadChannel(Dio_ChannelType ChannelId);
void   VttDio_WriteChannel(Dio_ChannelType ChannelId, uint32 Level);

Dio_PortLevelType VttDio_ReadPort(Dio_PortType PortId);
void   VttDio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);

void   VttDio_OnStateChange(uint8 action, uint8 oldState, uint8 newState);

#endif  /* VTTCNTRL_DIO_H */

/**********************************************************************************************************************
*  END OF FILE: VTTCntrl_Dio.h
*********************************************************************************************************************/
