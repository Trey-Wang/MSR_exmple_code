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
/**      \file    LinTrcv_GeneralTypes.h
*        \brief   Type definitions of the AUTOSAR Lin Transceiver component
*
*********************************************************************************************************************/

/**********************************************************************************************************************
*  REVISION HISTORY
*  -------------------------------------------------------------------------------------------------------------------
*  Refer to the module's header file.
*
*  FILE VERSION
*  -------------------------------------------------------------------------------------------------------------------
*  Refer to the VERSION CHECK below.
*********************************************************************************************************************/

#ifndef __LINTRCV_GENERALTYPES_H
#define __LINTRCV_GENERALTYPES_H

typedef enum LinTrcv_TrcvModeTypeTag 
{
  LINTRCV_TRCV_MODE_NORMAL  = 0u,
  LINTRCV_TRCV_MODE_STANDBY = 1u,
  LINTRCV_TRCV_MODE_SLEEP   = 2u
} LinTrcv_TrcvModeType;


typedef enum LinTrcv_TrcvWakeupModeTypeTag
{
  LINTRCV_WUMODE_ENABLE  = 0u,  
  LINTRCV_WUMODE_DISABLE = 1u,
  LINTRCV_WUMODE_CLEAR   = 2u
} LinTrcv_TrcvWakeupModeType;

typedef enum LinTrcv_TrcvWakeupReasonTypeTag
{
  LINTRCV_WU_ERROR = 0u,
  LINTRCV_WU_NOT_SUPPORTED = 1u,
  LINTRCV_WU_BY_BUS = 2u,
  LINTRCV_WU_BY_PIN = 3u,
  LINTRCV_WU_INTERNALLY = 4u,
  LINTRCV_WU_RESET = 5u,
  LINTRCV_WU_POWER_ON = 6u  
} LinTrcv_TrcvWakeupReasonType;


#endif

