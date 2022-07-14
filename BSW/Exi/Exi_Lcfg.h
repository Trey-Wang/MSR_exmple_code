/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 * 
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Exi_Lcfg.h
 *       Module:  Efficient XML Interchange
 *    Generator:  SysService_Exi.dll
 *
 *  Description:  link-time configuration header
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file Exi.h.
 *********************************************************************************************************************/

#if (!defined EXI_LCFG_H)
# define EXI_LCFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Exi_Types.h"

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

#if defined (EXI_CONFIG_VARIANT) && (3u == EXI_CONFIG_VARIANT)
#define EXI_START_SEC_CONST_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */
extern CONST(uint32, EXI_CONST) Exi_LinktimeCRC;
#define EXI_STOP_SEC_CONST_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */
#endif
/* (EXI_CONFIG_VARIANT) && (3u == EXI_CONFIG_VARIANT) */

#if defined (EXI_CONFIG_VARIANT) && (3u != EXI_CONFIG_VARIANT)
#define EXI_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /*  MD_MSR_19.1 */
extern CONST(Exi_ConfigType, EXI_CONST) Exi_Config;
#define EXI_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /*  MD_MSR_19.1 */
#endif

#endif
  /* EXI_LCFG_H */
/**********************************************************************************************************************
 *  END OF FILE: Exi_Lcfg.h
 *********************************************************************************************************************/
