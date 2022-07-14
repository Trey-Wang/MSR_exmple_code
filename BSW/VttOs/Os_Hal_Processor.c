/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 *  \addtogroup  VttOs
 *  \{
 *
 *  \file
 *  \brief       
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#define OS_HAL_PROCESSOR_SOURCE
#define _HAS_CXX17 0
/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
#include <Os_Hal_Processor.h>

/* Os kernel module dependencies */
# include "Os_Common_Types.h"

/* Os hal dependencies */
#include <Os_Hal_Debug.h>
#include <cassert>


/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/
uint8 VttOs_Pits[OS_CANOE_PIT_COUNT];
uint8 VttOs_PitCount;

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  Os_Hal_ProcessorInit()
 **********************************************************************************************************************/
void Os_Hal_ProcessorInit(void)
{
  VttOs_PitCount = 0;
}

/***********************************************************************************************************************
 *  Os_Hal_ProcessorCreatePit()
 **********************************************************************************************************************/
void Os_Hal_ProcessorCreatePit(const char* name, uint32 cycleTime, VUserTimerHandler callbackFunction)
{
  uint8 index;

  assert(VttOs_PitCount <= OS_CANOE_PIT_COUNT);
  index = CANoeAPI_UserTimer_Create(name);
  CANoeAPI_UserTimer_SetCallbackFunction(index, callbackFunction);
#ifndef OS_HAL_DEBUG_NO_TIMER
  CANoeAPI_UserTimer_SetCyclic(index, cycleTime);
#else
  CANoeAPI_Printf("WARNING: PIT NOT STARTED!");
#endif
  VttOs_Pits[VttOs_PitCount++] = index;
}

/***********************************************************************************************************************
 *  Os_Hal_ProcessorDestroyTimers()
 **********************************************************************************************************************/
void Os_Hal_ProcessorDestroyTimers(void)
{
  uint8 i;

  for(i = 0; i < OS_CANOE_HRT_COUNT; i++)
  {
    CANoeEmuHRTimer_SetIrqNumber(OS_CANOE_HRT_COUNT, CANOEEMUPROCESSOR_INVALIDIRQ);
  }

  for(i = 0; i < VttOs_PitCount; i++)
  {
    CANoeAPI_UserTimer_Destroy(i);
  }
  VttOs_PitCount = 0;
}

/***********************************************************************************************************************
 *  VttOs_OnStateChange()
 **********************************************************************************************************************/
void VttOs_OnStateChange(uint8 action, uint8 oldState, uint8 newState)
{
  OS_IGNORE_UNREF_PARAM(newState);
  OS_IGNORE_UNREF_PARAM(oldState);

  switch(action)
  {
  case CANOEAPI_ECUACTION_LOAD:
    break;

  case CANOEAPI_ECUACTION_UNLOAD:
    break;

  case CANOEAPI_ECUACTION_STARTMEASUREMENT:
    break;

  case CANOEAPI_ECUACTION_STOPMEASUREMENT:
    break;
  
  case CANOEAPI_ECUACTION_SWITCHON:
    Os_Hal_ProcessorInit();
    break;

  case CANOEAPI_ECUACTION_RESET:
    CANoeEmuProcessor_DisableInterrupts();
    Os_Hal_ProcessorDestroyTimers();
    break;

  case CANOEAPI_ECUACTION_SWITCHOFF:
    CANoeEmuProcessor_DisableInterrupts();
    Os_Hal_ProcessorDestroyTimers();
    break;

  case CANOEAPI_ECUACTION_GOTOSLEEP:
    break;

  case CANOEAPI_ECUACTION_WAKEUP:
    break;
  }
}
/* module specific MISRA deviations:
 */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Processor.c
 **********************************************************************************************************************/
