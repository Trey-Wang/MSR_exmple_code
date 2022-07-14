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
/**        \file  VTTCntrl_Icu.h
 *        \brief  Declares the public API of the virtual ICU controller integrated in the vVIRTUALtarget.
 *
 *     \details   Virtual ICU controller that enables edge detection and time measurement of signal changes.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Max-Ferdinand Suffel          vismsl        Vector Informatik GmbH
 *  Damian Philipp                visdap        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  03.00.01  2015-07-24  vismsl  -             Initial version extracted from VTTCntrl generator.
 *  03.00.04  2015-09-22  visdap  -             ESCAN00085394: Added pragmas to protect VTTCntrl during simulated
 *                                              ECU reset.
 *  03.00.05  2015-12-18  vismsl  -             Incremented version number since VTTCntrl_Base changed.
 *  03.00.06  2016-01-22  vismsl  ESCAN00087533 Cleanup of VttCntrl codebase: Refactoring and additional documentation.
*********************************************************************************************************************/

#if !defined(VTTCNTRL_ICU_H)
# define VTTCNTRL_ICU_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
 
#include "Icu.h"
#include "VttCntrl_Base.h"
#include "VTTCntrl_Icu_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Component version information ----- */
#define VTTICU_SW_MAJOR_VERSION (3u)
#define VTTICU_SW_MINOR_VERSION (0u)
#define VTTICU_SW_PATCH_VERSION (6u)

#define VTTICU_FALSE   0
#define VTTICU_TRUE    1

#define VTTICU_VALID_EDGE    (0x00)
#define VTTICU_INVALID_EDGE  (0x01)

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL DATA TYPES AND STRUCTURES
*********************************************************************************************************************/
 
 /*! \brief Internal ICU channel state. */
typedef enum
{
  VttIcu_State_Edge_x = 0,
  VttIcu_State_Measurement_First,
  VttIcu_State_Measurement_Running,

} VttIcu_State;

/*! \brief Internal ICU controller structure associated with a ICU channel. */
typedef struct
{
  /* Derived from ICU configuration. */
  const char* ChName;
  const unsigned char ChWakeupCapability;

  /* Internal data. */
  Icu_ActivationType ChStartEdge;
  Icu_ActivationType ChTriggerEdge;
  unsigned char ChIsActive;
  VttIcu_State ChState;
  sint32 ChIsrNr;
  sint32 ChSysVarHandle;
  long long ChTimerValue;
  sint32 ChLastValue;
  
} VttIcu_CntrlType;

/**********************************************************************************************************************
*  GLOBAL DATA PROTOTYPES
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL FUNCTION PROTOTYPES
*********************************************************************************************************************/

void VttIcu_StartEdgeMonitoring( uint8 channel, Icu_ActivationType triggerEdge );
void VttIcu_StopEdgeMonitoring ( uint8 channel );

void VttIcu_StartTimeMeasurement( uint8 channel, Icu_ActivationType startingEdge, Icu_ActivationType triggerEdge );
void VttIcu_StopTimeMeasurement( uint8 channel );

unsigned long long VttIcu_GetTimerValue( uint8 channel );

void VttIcu_OnStateChange(uint8 action, uint8 oldState, uint8 newState);

#endif  /* VTTCNTRL_ICU_H */

/**********************************************************************************************************************
 *  END OF FILE: VttCntrl_Icu.h
 *********************************************************************************************************************/
