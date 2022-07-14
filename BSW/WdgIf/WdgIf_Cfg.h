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
/*!        \file  WdgIf_Cfg.h
 *        \brief  Private include file for WdgIf module
 *
 *      \details  This include file provides defines and declarations for the WdgIf configuration identifiers
 *
 *********************************************************************************************************************/

#ifndef WDGIF_CFG_H
# define WDGIF_CFG_H


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "WdgIf_Types.h"

/* Generated preprocessor options for module features */
# include "WdgIf_Cfg_Features.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS (Version Info)
 *********************************************************************************************************************/

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define WDGIF_CFG_SW_MAJOR_VERSION         (5u)
# define WDGIF_CFG_SW_MINOR_VERSION         (2u)
# define WDGIF_CFG_SW_PATCH_VERSION         (0u)

# define WDGIF_GENERATOR_MAJOR_VERSION      (2u)
# define WDGIF_GENERATOR_MINOR_VERSION      (1u)
# define WDGIF_GENERATOR_PATCH_VERSION      (0u)

/* ----- WdgIf_Types.h version check ----- */
/* Protection against incorrect include file version (WdgIf_Types.h) */
#if (  (WDGIF_TYPES_SW_MAJOR_VERSION != WDGIF_CFG_SW_MAJOR_VERSION) \
    || (WDGIF_TYPES_SW_MINOR_VERSION != WDGIF_CFG_SW_MINOR_VERSION) \
    || (WDGIF_TYPES_SW_PATCH_VERSION != WDGIF_CFG_SW_PATCH_VERSION) )
  #error "Version mismatch! Incorrect version of WdgIf_Types.h"
#endif

/* ----- WdgIf_Cfg_Features.h version check ----- */
/* Protection against incorrect include file version (WdgIf_Cfg_Features.h) */
#if (  (WDGIF_CFG_FEATURE_MAJOR_VERSION != WDGIF_GENERATOR_MAJOR_VERSION) \
    || (WDGIF_CFG_FEATURE_MINOR_VERSION != WDGIF_GENERATOR_MINOR_VERSION) )
  #error "Version mismatch! Incorrect version of WdgIf_Cfg_Features.h"
#endif

# define WDGIF_MAX_SUPPORTED_CORES          (4u)


/**********************************************************************************************************************
 *  MACRO VALIDATION
 *********************************************************************************************************************/

# ifndef WDGIF_DEV_ERROR_DETECT
    #error "Macro WDGIF_DEV_ERROR_DETECT should be defined! (ON/OFF)"
# endif

# ifndef WDGIF_VERSION_INFO_API
    #error "Macro WDGIF_VERSION_INFO_API should be defined! (ON/OFF)"
# endif

# ifndef WDGIF_USE_STATECOMBINER
    #error "Macro WDGIF_USE_STATECOMBINER should be defined! (ON/OFF)"
# endif

# if (WDGIF_USE_STATECOMBINER == STD_ON)
#  ifndef WDGIF_STATECOMBINER_USE_OS_SPIN_LOCK
#   error "Macro WDGIF_STATECOMBINER_USE_OS_SPIN_LOCK must be defined! (ON/OFF)"
#  endif
# endif

# ifndef FALSE
#  error "Macro FALSE should be defined! ((boolean) 0u)"
# endif

# ifndef TRUE
#  error "Macro TRUE should be defined! ((boolean) 1u)"
# endif

# ifndef NULL_PTR /* COV_WDGIF_NULL_PTR_DEFINE */
#  define NULL_PTR ((void *) 0u)
# endif


/**********************************************************************************************************************
 *  TYPE DEFINITIONS AND FURTHER INCLUDES
 *********************************************************************************************************************/

/*! Type for function pointers to the driver's API */
typedef struct {
  Std_ReturnType (*Wdg_SetMode_AR) (WdgIf_ModeType);              /*!< Pointer to a driver function
                                                                   *   Wdg_<platform>_SetMode()
                                                                   */ 
  void (*Wdg_SetTriggerCondition_AR) (uint16);                    /*!< Pointer to a driver function 
                                                                   *   Wdg_<platform>_SetTriggerWindow()
                                                                   */
} WdgIf_InterfaceFunctionsType;


/*! Type containing the reference to WdgIf_InterfaceFunctionsType per WdgIf device, which contains all necessary driver
 *  function as well as the corresponding watchdog instance IDs
 */
typedef struct {
  const WdgIf_InterfaceFunctionsType *WdgFunctions; /*!< Pointer to a struct of watchdog driver functions */
# if (WDGIF_USE_STATECOMBINER == STD_ON)
  uint8 WdgInstance;                                /*!< Watchdog instance or 0 if the driver does not use it */   
# endif
} WdgIf_InterfaceFunctionsPerWdgDeviceType;


# if (WDGIF_USE_STATECOMBINER == STD_ON)
#  if (WDGIF_STATECOMBINER_USE_OS_SPIN_LOCK == STD_ON)
/*! Os.h needed for type SpinlockIdType and spinlock functions GetSpinlock() and ReleaseSpinlock() */
#   include "Os.h"
typedef SpinlockIdType WdgIf_StateCombinerSpinlockIdType;
#  else
/*! Appl_Spinlock.h declares type Appl_SpinlockIdType
 *  and spinlock functions Appl_GetSpinlock() and Appl_ReleaseSpinlock()
 */
#   include "Appl_Spinlock.h"
typedef Appl_SpinlockIdType WdgIf_StateCombinerSpinlockIdType;
#  endif /* (WDGIF_STATECOMBINER_USE_OS_SPIN_LOCK == STD_ON) */

/*! Type for global shared data used by the StateCombiner. \n
 *  Read by master and written by all slave devices and contains current trigger counter values. \n\n
 *
 *  This type is only present in case if pre-processor option WDGIF_USE_STATECOMBINER is set to STD_ON
 */
typedef struct {
  uint32 SlaveCounterValue;     /*!< Current slaves trigger counter value */
  uint32 SlaveCounterValue_INV; /*!< Inverted value of the current slaves trigger counter */
} WdgIf_StateCombinerSharedMemory;


/*! Type for structure of type WdgIf_StateCombinerSlaveTriggerPatternType containing the configuration for the State
 *  Combiner necessary. \n\n
 *
 *  This type is only present in case if pre-processor option WDGIF_USE_STATECOMBINER is set to STD_ON
 */
typedef struct {
  uint16 WdgIfStateCombinerReferenceCycle;      /*!< Defines the reference cycle with which the master will check the
                                                 *   slave
                                                 */
  uint16 WdgIfStateCombinerSlaveIncrementsMin;  /*!< Minimal number of expected slave triggers in one master check
                                                 *   interval
                                                 */
  uint16 WdgIfStateCombinerSlaveIncrementsMax;  /*!< Maximal number of expected slave triggers in one master check
                                                 *   interval
                                                 */
} WdgIf_StateCombinerSlaveTriggerPatternType;


/*! Type for the State Combiner specific configuration structure. \n\n
 *
 *  This type is only present in case if pre-processor option WDGIF_USE_STATECOMBINER is set to STD_ON
 */
typedef struct {
  uint8 WdgIfStateCombinerNumberOfSlaves;                           /*!< Number of slaves configured in the State 
                                                                     *   Combiner
                                                                     */
  WdgIf_StateCombinerSpinlockIdType WdgIfStateCombinerSpinlockId;   /*!< Spinlock ID for synchronizing the access to
                                                                     *   the shared memory section
                                                                     */
  uint16 WdgIfStateCombinerStartUpSyncCycles;                       /*!< Number of master cycles during start-up in 
                                                                     *   which master does not check slave triggers
                                                                     */
  const WdgIf_InterfaceFunctionsType *WdgIfStateCombinerFunctions;  /*!< Addresses of the functions of the watchdog
                                                                     *   device driver connected to the master
                                                                     */

  WdgIf_StateCombinerSharedMemory *WdgIfStateCombinerSData;         /*!< Pointer to array of the shared memory fields
                                                                     *   with data of slave devices
                                                                     */
  const WdgIf_StateCombinerSlaveTriggerPatternType **WdgIfStateCombinerSlaveTriggerPattern;   /*!< Array of structures
                                                                                               * of State  Combiner
                                                                                               * slave trigger pattern
                                                                                               */
} WdgIf_StateCombinerConfigType;
# endif /* (WDGIF_USE_STATECOMBINER == STD_ON) */


/*! Type for global config data \n\n
 *
 *  This structure is used by the embedded code as "entry point" to the Watchdog Interface configuration, as it 
 *  includes pointers which allow navigating (directly or indirectly) to all data structures in the configuration
 */
typedef struct {
  uint8 NumOfWdgs;                                                              /*!< Number of watchdogs. */
  const WdgIf_InterfaceFunctionsPerWdgDeviceType *WdgFunctionsPerDevice;        /*!< Array of structures functions per
                                                                                 * device ordered by device index
                                                                                 */
# if (WDGIF_USE_STATECOMBINER == STD_ON)
  const WdgIf_StateCombinerConfigType *WdgIfStateCombinerConfig;    /*!< Array of structures of State
                                                                                 *   Combiner configuration type
                                                                                 *   (common)
                                                                                 */
# endif /* (WDGIF_USE_STATECOMBINER == STD_ON) */
} WdgIf_InterfaceType;

#endif /* WDGIF_CFG_H */

/**********************************************************************************************************************
 *  END OF FILE: WdgIf_Cfg.h
 *********************************************************************************************************************/
