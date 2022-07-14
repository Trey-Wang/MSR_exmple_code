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
 *  \addtogroup  Os_Hal_Core
 *  \{
 *
 *  \file
 *  \brief       Core related primitives which don't have to be inlined.
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

                                                                                                                        /* PRQA S 0777, 0779, 0810, 0828, 0857 EOF */ /* MD_MSR_5.1_777, MD_MSR_5.1_779, MD_MSR_1.1_810, MD_MSR_1.1_828, MD_MSR_1.1_857 */


#define OS_HAL_CORE_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
#include "Std_Types.h"

/* Os module declarations */
#include "Os_Hal_Core.h"
#include "Os_IsrInt.h"

#include "Os_Cfg.h"
#include "Os_Hal_Context_Lcfg.h"
#include "Os_Hal_Interrupt_Lcfg.h"
#include "Os_Hal_Vtt_LCfg.h"
/* Os kernel module dependencies */

/* Os hal dependencies */
#include "Os_Hal_Os.h"
#include "Os_Hal_Cfg.h"
#include "Os_Hal_Debug.h"

/***********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/

/* Check the version of OS header file */
#if (  (OS_HAL_SW_MAJOR_VERSION != (2u)) \
    || (OS_HAL_SW_MINOR_VERSION != (8u)) \
    || (OS_HAL_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of Os_Hal_Core.c and Os_Hal_Os.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (OS_CFG_HAL_MAJOR_VERSION != (2u)) \
    || (OS_CFG_HAL_MINOR_VERSION != (7u)) )
# error "Version numbers of Os_Hal_Core.c and Os_Hal_Cfg.h are inconsistent!"
#endif

/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

const Os_ThreadConfigType* CurrentThread[OS_COREID_COUNT];

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define OS_START_SEC_CODE
#include "Os_MemMap_OsCode.h"                                                                                           /* PRQA S 5087 */ /* MD_MSR_19.1 */


/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

void Os_Hal_InitInterrupt(P2CONST(Os_Hal_IntIsrConfigType, AUTOMATIC, OS_CONST) Config,  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) Thread)
{
  Os_Hal_IntSourceType source;

  Os_Hal_Debug_InitInterrupt_Entry();

  source = Config->Source;
  CANoeEmuProcessor_SetInterruptRoutingMask(source, Config->RoutingMask);
  CANoeEmuProcessor_ConfigureISR(source,  (void (*)(void))Config->Handler);
  CANoeEmuProcessor_ConfigureInterruptPriority(source, Config->Level);
  CANoeEmuProcessor_ConfigureInterruptName(source, Config->Name);
  CANoeEmuProcessor_UnmaskInterrupt(source);

  if(Thread != NULL_PTR)
  {
    Os_Hal_ContextInit(Thread->ContextConfig, Thread->Context);
  }

}

void Os_Hal_InitHook(P2CONST(Os_HookConfigType, AUTOMATIC, OS_CONST) Config)
{
   if(Config != NULL_PTR)
   {
	   Os_Hal_Debug_InitHook_Entry();

	   Os_Hal_ContextInit(Config->Thread.ContextConfig, Config->Thread.Context);
   }
}

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Os_Hal_CoreInit()
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_Hal_CoreInit(P2CONST(Os_Hal_CoreAsrConfigType, AUTOMATIC, OS_CONST) Config)
{
  Os_ObjIdxType i, j;
  const Os_CoreAsrConfigType *core;

  Os_Hal_Debug_CoreInit_Entry();

  core = Os_Core2AsrCore(Os_CoreGetCurrentCore());

  /* Set the number of CANoeEmu tasks running on this core */
  CANoeEmuProcessor_ConfigureNumberOfTasks(Config->NumberOfCANoeEmuTasks);

  /* Initialize all application specific Isrs, Sercvice functions and hooks on this core */
  for(i = 0; i < core->AppCount; i++)
  {
    Os_AppConfigRefType app = core->AppRefs[i];

    for(j = 0; j < app->IsrCat1Count; j++)
    {
      Os_Hal_InitInterrupt(app->IsrCat1Refs[j]->HwConfig, NULL_PTR);
    }

    for(j = 0; j < app->IsrCat2Count; j++)
    {
      Os_Hal_InitInterrupt(app->IsrCat2Refs[j]->HwConfig, &(app->IsrCat2Refs[j]->Thread));
    }

    /* The context of a service function is initialized by Os_ServiceNtfCall() */

    Os_Hal_InitHook(&app->StartupHookRef->Hook);
    Os_Hal_InitHook(&app->ShutdownHookRef->Hook);
    Os_Hal_InitHook(&app->ErrorHookRef->Hook);
  }

  /* Initialize all core specific Isrs and hooks */


  for(j = 0; j < core->KernelApp->IsrCat1Count; j++)
  {
    Os_Hal_InitInterrupt(core->KernelApp->IsrCat1Refs[j]->HwConfig, NULL_PTR);
  }

   for(j = 0; j < core->KernelApp->IsrCat2Count; j++)
  {
    Os_Hal_InitInterrupt(core->KernelApp->IsrCat2Refs[j]->HwConfig, &(core->KernelApp->IsrCat2Refs[j]->Thread));
  }

  Os_Hal_InitHook(&core->StartupHookRef->Hook);
  Os_Hal_InitHook(&core->ShutdownHookRef->Hook);
  Os_Hal_InitHook(&core->ErrorHookRef->Hook);
  Os_Hal_InitHook(&core->ProtectionHookRef->Hook);

}


/***********************************************************************************************************************
 *  Os_Hal_ConsumeStack()
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_Hal_ConsumeStack(P2CONST(Os_StackConfigType, AUTOMATIC, OS_CONST) stack)
{
  *stack->HighAddress = ~OS_HAL_STACK_FILL_PATTERN;
}

/***********************************************************************************************************************
 *  Os_Hal_CANoeTaskWrapper()
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_Hal_CANoeTaskWrapper(void)
{
  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) currentThread;

  currentThread = Os_CoreGetThread();
  currentThread->ContextConfig->Entry(currentThread->Context->Parameter);
}

/***********************************************************************************************************************
 *  Os_Hal_TaskEndHook()
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_Hal_TaskEndHook(void)
{

  P2CONST(Os_ThreadConfigType, AUTOMATIC, OS_CONST) currentThread;

  currentThread = Os_CoreGetThread();

  Os_Hal_CoreFreeze();
}

/***********************************************************************************************************************
 *  Os_Hal_CoreInitHardware()
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_Hal_CoreInitHardware(P2CONST(Os_Hal_SystemConfigType, AUTOMATIC, OS_CONST) Config)
{
  int i;

  OS_IGNORE_UNREF_PARAM(Config);

  Os_Hal_Debug_CoreInitMaster_Entry();

  CANoeEmuProcessor_InitInterruptRouter();
  CANoeEmuProcessor_DisableInterruptsAtStartOfISR(1);

  for(i=0; i<OS_ISRID_COUNT; i++)
  {
    const Os_IsrConfigType*      config = OsCfg_IsrRefs[i];
    const Os_Hal_IntIsrConfigType*  hwConfig = config->HwConfig;

    Os_Hal_IntSourceType source = hwConfig->Source;
    config->Thread.Context->Config = config->Thread.ContextConfig;
    CANoeEmuProcessor_SetInterruptRoutingMask(source, 0xFFFFFFFF);
  }

  /*
   * Highest possible level before CANoeEmuProcessor_SetInterruptRoutingMask is 0 because
   * CANoeEmuProcessor_SetInterruptLevel reduces the level to the lowest configured maskable
   * interrupt.
   * Keep this interrupt level by rising the level to OS_CFG_MAX_INT_LEVEL.
   */
  CANoeEmuProcessor_SetInterruptLevel(OS_CFG_MAX_INT_LEVEL);
}


/***********************************************************************************************************************
 *  Os_Hal_CoreFreeze()
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DEFINITION(void, OS_CODE, OS_NORETURN, Os_Hal_CoreFreeze,                                             /* PRQA S 3219 */ /* MD_Os_Hal_3219 */
(
  void
))
{
  Os_Hal_Debug_CoreFreeze_Entry();
  CANoeAPI_Printf("[Unrecoverable OS error detected]");
  for( ; ; )
  {
    CANoeAPI_ConsumeTicks(1);
  }
}


#define OS_STOP_SEC_CODE
#include "Os_MemMap_OsCode.h"                                                                                           /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* module specific MISRA deviations:
 */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Core.c
 **********************************************************************************************************************/
