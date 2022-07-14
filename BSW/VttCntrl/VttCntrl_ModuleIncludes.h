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
/**        \file  VTTCntrl_ModuleIncludes.h
 *        \brief  Ifdef switches to include the headers of all active VTT modules
 *
 *      \details  A set of static ifdefs to include the header files of all VTT modules that are active
 *                in the current configuration.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Damian Philipp                 visdap       Vector Informatik GmbH
 *  Wladimir Gerber                viswge       Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  02.00.01  2015-04-02  visdap  -             Initial version.
 *  02.00.02  2015-04-15  viswge  -             Added VTTCntrl_Compiler_Cfg.inc.
 *  03.01.00  2015-11-06  visdap  -             Renamed defines to include VTT prefix
 *  03.01.01  2016-01-19  vismsl  ESCAN00087533 Cleanup of VttCntrl codebase: Refactoring and additional documentation.
 *********************************************************************************************************************/

#if !defined (VTTCNTRL_MODULEINCLUDE_H)
#define VTTCNTRL_MODULEINCLUDE_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

#include "VttCntrl.h"

#if (STD_ON == VTTCNTRL_MODULE_VTTADC_EXISTS )
#include "Adc.h"
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTCAN_EXISTS )
#include "Can.h"
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTFR_EXISTS  )
#include "Fr.h"
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTLIN_EXISTS )
#include "Lin.h"
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTETH_EXISTS )
#include "Eth_30_Vtt.h"
#include "EthTrcv_30_Vtt.h"
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTMCU_EXISTS )
#include "Mcu.h"
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTDIO_EXISTS )
#include "Dio.h"
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTWDG_EXISTS )
#include "Wdg.h"
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTFLS_EXISTS )
#include "Fls.h"
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTGPT_EXISTS )
#include "Gpt.h"
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTPWM_EXISTS )
#include "Pwm.h"
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTEEP_EXISTS )
#include "Eep.h"
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTICU_EXISTS )
#include "Icu.h"
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTSPI_EXISTS )
#include "Spi.h"
#endif

#if (STD_ON == VTTCNTRL_MODULE_VTTPORT_EXISTS)
#include "Port.h"
#endif

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

#endif  /* VTTCNTRL_MODULEINCLUDE_H */

/**********************************************************************************************************************
 *  END OF FILE: VTTCntrl_ModuleIncludes.h
 *********************************************************************************************************************/
