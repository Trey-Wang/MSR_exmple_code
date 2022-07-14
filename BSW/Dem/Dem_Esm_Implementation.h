/* ********************************************************************************************************************
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
 *  -------------------------------------------------------------------------------------------------------------------
 */
/*! \addtogroup Dem_Esm
 *  \{
 *  \file       Dem_Esm_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Savas Ates                    vissat        Vector Informatik GmbH
 *  Anna Bosch                    visanh        Vector Informatik GmbH
 *  Stefan Huebner                vishrs        Vector Informatik GmbH
 *  Thomas Dedler                 visdth        Vector Informatik GmbH
 *  Alexander Ditte               visade        Vector Informatik GmbH
 *  Matthias Heil                 vismhe        Vector Informatik GmbH
 *  Erik Jeglorz                  visejz        Vector Informatik GmbH
 *  Friederike Schulze            visfrs        Vector Informatik GmbH
 *  Aswin Vijayamohanan Nair      visavi        Vector Informatik GmbH
 *  Fabian Wild                   viszfa        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_ESM_IMPLEMENTATION_H)
#define DEM_ESM_IMPLEMENTATION_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Esm_Interface.h"

                                                    /* Used subcomponent API */
/* ------------------------------------------------------------------------- */
#include "Dem_Queue_Interface.h"

                                                   /* Subcomponents callbacks*/
/* ------------------------------------------------------------------------- */
#include "Dem_MemAccess_Interface.h"
#include "Dem_OperationCycle_Interface.h"
#include "Dem_Error_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

#define DEM_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)                                                                                           /* COV_MSR_UNSUPPORTED XF */
/*! Number of qualified active and failed events per WWHO-OBD class. Index 0 (no class) is unused. */
DEM_LOCAL VAR(Dem_EventIdType, DEM_VAR_NOINIT)   Dem_Esm_QualifiedActiveEventCount[DEM_ESM_WWHOBD_NUMBER_DTCCLASS];
/*! Number of MIL requests per WWHO-OBD class. Index 0 (no class) is unused. */
DEM_LOCAL VAR(Dem_EventIdType, DEM_VAR_NOINIT)   Dem_Esm_DtcClassMILCount[DEM_ESM_WWHOBD_NUMBER_DTCCLASS];
#endif

#define DEM_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_Esm_Public
 * \{
 */
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)                                                                                           /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Util_DcmIndicatorActivationMode
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Util_DcmIndicatorActivationMode(
  CONST(Dem_IndicatorStatusType, AUTOMATIC)  ActivationMode
  )
{
  uint8 lReturnValue;

  switch (ActivationMode)
  {
  case DEM_INDICATOR_CONTINUOUS:
    lReturnValue = 0x03U;
    break;
  case DEM_INDICATOR_SHORT:
    lReturnValue = 0x02U;
    break;
  case DEM_INDICATOR_ON_DEMAND:
    lReturnValue = 0x01U;
    break;
  case DEM_INDICATOR_OFF:
    lReturnValue = 0x00U;
    break;
  default:
    Dem_Error_ReportError(DEM_INTERNAL_APIID, DEM_E_INCONSISTENT_STATE);
    lReturnValue = 0x00U;
    break;
  }
  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)                                                                                           /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Util_SeverityWwhObdDtcClass
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Util_SeverityWwhObdDtcClass(
  CONST(Dem_DTCSeverityType, AUTOMATIC)  Severity
  )
{
  uint8 lReturnValue;

  switch (Severity & 0x1fU) /* ignore the UDS severity bits */
  {
  case DEM_SEVERITY_WWHOBD_CLASS_A:
    lReturnValue = DEM_ESM_WWHOBD_DTCCLASS_A;
    break;
  case DEM_SEVERITY_WWHOBD_CLASS_B1:
    lReturnValue = DEM_ESM_WWHOBD_DTCCLASS_B1;
    break;
  case DEM_SEVERITY_WWHOBD_CLASS_B2:
    lReturnValue = DEM_ESM_WWHOBD_DTCCLASS_B2;
    break;
  case DEM_SEVERITY_WWHOBD_CLASS_C:
    lReturnValue = DEM_ESM_WWHOBD_DTCCLASS_C;
    break;
  case DEM_SEVERITY_WWHOBD_CLASS_NO_CLASS:
    lReturnValue = DEM_ESM_WWHOBD_DTCCLASS_NO;
    break;
  default:
    Dem_Error_ReportError(DEM_INTERNAL_APIID, DEM_E_INCONSISTENT_STATE);
    lReturnValue = 0x00U;
    break;
  }
  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_J1939 == STD_ON) && (DEM_FEATURE_NEED_SPECIAL_INDICATORS == STD_ON)
/* ****************************************************************************
 % Dem_Util_J1939IndicatorStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Util_J1939IndicatorStatus(
  CONST(uint8, AUTOMATIC)  ConfigIndicatorState
  )
{
  uint16 lReturnValue;
  switch(ConfigIndicatorState)
  {
  case DEM_INDICATOR_OFF:
    lReturnValue = DEM_J1939_INDICATOR_LAMP_OFF;
    break;
  case DEM_INDICATOR_FAST_FLASH:
    lReturnValue = DEM_J1939_INDICATOR_LAMP_FAST_FLASH;
    break;
  case DEM_INDICATOR_SLOW_FLASH:
    lReturnValue = DEM_J1939_INDICATOR_LAMP_SLOW_FLASH;
    break;
  case DEM_INDICATOR_CONTINUOUS:
    lReturnValue = DEM_J1939_INDICATOR_LAMP_CONT;
    break;

  default:
    Dem_Error_ReportError(DEM_INTERNAL_APIID, DEM_E_INCONSISTENT_STATE);
    lReturnValue = DEM_J1939_INDICATOR_LAMP_OFF;
    break;
  }
  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_J1939 == STD_ON) && (DEM_CFG_SUPPORT_J1939_NODES == STD_ON)                                                 /* COV_MSR_UNSUPPORTED XF xf xf */
/* ****************************************************************************
 % Dem_Util_J1939PriorityOfIndicatorState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Util_J1939PriorityOfIndicatorState(
  CONST(uint8, AUTOMATIC)  ConfigIndicatorState
  )
{
  uint8 lReturnValue;
  switch(ConfigIndicatorState)
  {
  case DEM_INDICATOR_OFF:
    lReturnValue = 0x00U;
    break;
  case DEM_INDICATOR_SLOW_FLASH:
    lReturnValue = 0x01U;
    break;
  case DEM_INDICATOR_FAST_FLASH:
    lReturnValue = 0x02U;
    break;
  case DEM_INDICATOR_CONTINUOUS:
    lReturnValue = 0x03U;
    break;

  default:
    Dem_Error_ReportError(DEM_INTERNAL_APIID, DEM_E_INCONSISTENT_STATE);
    lReturnValue = 0x00U;
    break;
  }
  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_J1939 == STD_ON) && (DEM_CFG_SUPPORT_J1939_NODES == STD_ON)                                                 /* COV_MSR_UNSUPPORTED XF xf xf */
/* ****************************************************************************
 % Dem_Util_J1939IndicatorStateFromPriority
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Util_J1939IndicatorStateFromPriority(
  CONST(uint8, AUTOMATIC)  Priority
  )
{
  uint8 lReturnValue;
  switch(Priority)
  {
  case 0x00U:
    lReturnValue = DEM_INDICATOR_OFF;
    break;
  case 0x01U:
    lReturnValue = DEM_INDICATOR_SLOW_FLASH;
    break;
  case 0x02U:
    lReturnValue = DEM_INDICATOR_FAST_FLASH;
    break;
  case 0x03U:
    lReturnValue = DEM_INDICATOR_CONTINUOUS;
    break;

  default:
    Dem_Error_ReportError(DEM_INTERNAL_APIID, DEM_E_INCONSISTENT_STATE);
    lReturnValue = DEM_INDICATOR_OFF;
    break;
  }
  return lReturnValue;
}
#endif

#if ( (DEM_CFG_SUPPORT_J1939 == STD_ON) \
   && (DEM_CFG_SUPPORT_J1939_NODES == STD_ON) \
   && (DEM_FEATURE_NEED_SPECIAL_INDICATORS == STD_ON) )                                                                          /* COV_MSR_UNSUPPORTED XF xf xf xf */
/* ****************************************************************************
 % Dem_Util_J1939GlobalSpecialIndicatorState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Util_J1939GlobalSpecialIndicatorState(
  CONST(uint8, AUTOMATIC)  IndicatorId
  )
{
  uint8 lSpecialIndicatorPriority;
  uint8 lJ1939NodeId;

  lSpecialIndicatorPriority = 0;
  lJ1939NodeId = Dem_Cfg_GlobalJ1939NodeCount();

  while (lJ1939NodeId != 0)
  {
    uint8 lTempPriority;

    --lJ1939NodeId;
    lTempPriority = Dem_Util_J1939PriorityOfIndicatorState(Dem_Cfg_GlobalSpecialIndicatorStates[IndicatorId][lJ1939NodeId]);

    if (lSpecialIndicatorPriority < lTempPriority)
    {
      lSpecialIndicatorPriority = lTempPriority;
    }
  }

  return Dem_Util_J1939IndicatorStateFromPriority(lSpecialIndicatorPriority);
}
#endif /* (DEM_CFG_SUPPORT_J1939 == STD_ON) && (DEM_CFG_SUPPORT_J1939_NODES == STD_ON) && (DEM_FEATURE_NEED_SPECIAL_INDICATORS == STD_ON) */

#if (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON) || (DEM_CFG_SUPPORT_J1939_DM31 == STD_ON)                                         /* COV_MSR_UNSUPPORTED XF xf xf */
/* ****************************************************************************
 % Dem_Util_J1939OccurrenceCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Util_J1939OccurrenceCounter(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  uint8 lReturnValue;

  if (Dem_DTC_TestStoredStatus(EventId) == TRUE)
  {
    /* Stored DTC, so occurrence counter is available in primary memory */
    Dem_Mem_MemoryInfoPtrType lMemoryInfo;
    Dem_Data_OccurrenceCounterType lOccurrenceCounter;

    lMemoryInfo = Dem_MemStorageManager_MemoryInfoInit(Dem_Cfg_EventDestination(EventId));

#if (DEM_CFG_DATA_OCCCTR_2BYTE == STD_ON)
    lOccurrenceCounter = 
      Dem_MemoryEntry_GetOccurrenceCounter2Byte(
        Dem_MemStorageManager_GetMemoryEntryId(Dem_MemStorageManager_MemoryFindIndex_Checked(lMemoryInfo, EventId)));            /* SBSW_DEM_CALL_MEMORYINFO_POINTER */
#else
    lOccurrenceCounter = 
      Dem_MemoryEntry_GetOccurrenceCounter(
        Dem_MemStorageManager_GetMemoryEntryId(Dem_MemStorageManager_MemoryFindIndex_Checked(lMemoryInfo, EventId)));            /* SBSW_DEM_CALL_MEMORYINFO_POINTER */
#endif

    /* Latch the counter value at 126 */
    if (lOccurrenceCounter > 0x7eU)
    {
      lOccurrenceCounter = 0x7eU;
    }
    lReturnValue = (uint8)lOccurrenceCounter;
  }
  else
  {
    lReturnValue = 0x7fU;
  }

  return lReturnValue;
}
#endif /* (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON) || (DEM_CFG_SUPPORT_J1939_DM31 == STD_ON) */

#if ( (DEM_FEATURE_NEED_PID01_CALCULATION == STD_ON) || (DEM_FEATURE_NEED_PID41_CALCULATION == STD_ON) )
/* ****************************************************************************
 % Dem_Util_SetReadinessGroupSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Util_SetReadinessGroupSupported(
  CONST(boolean, AUTOMATIC)  Supported,
  CONST(uint8, AUTOMATIC)  ReadinessGroup,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_APPL_DATA)  PidSupportedMask
)
{
  uint8 lReadinessGroup = ReadinessGroup;
  uint32 lPidSupportedMask = DEM_READINESS_SUPPORT_NONE_MASK;

  switch (lReadinessGroup)
  {
  case DEM_CFG_READINESS_MISF:
    lPidSupportedMask = DEM_READINESS_SUPPORT_MISF_MASK;
    break;
  case DEM_CFG_READINESS_FLSYS:
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    lPidSupportedMask = DEM_READINESS_SUPPORT_FLSYS_MASK;
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    lPidSupportedMask = DEM_READINESS_SUPPORT_CMPRCMPT;
    break;
  case DEM_CFG_READINESS_CAT:
    lPidSupportedMask = DEM_READINESS_SUPPORT_CAT_MASK;
    break;
  case DEM_CFG_READINESS_HCCAT:
    lPidSupportedMask = DEM_READINESS_SUPPORT_HCCAT_MASK;
    break;
  case DEM_CFG_READINESS_HTCAT:
    lPidSupportedMask = DEM_READINESS_SUPPORT_HTCAT_MASK;
    break;
  case DEM_CFG_READINESS_NOXCAT:
    lPidSupportedMask = DEM_READINESS_SUPPORT_NOXCAT_MASK;
    break;
  case DEM_CFG_READINESS_EVAP:
    lPidSupportedMask = DEM_READINESS_SUPPORT_EVAP_MASK;
    break;
  case DEM_CFG_READINESS_SECAIR:
    lPidSupportedMask = DEM_READINESS_SUPPORT_SECAIR_MASK;
    break;
  case DEM_CFG_READINESS_BOOSTPR:
    lPidSupportedMask = DEM_READINESS_SUPPORT_BOOSTPR_MASK;
    break;
  case DEM_CFG_READINESS_O2SENS:
    lPidSupportedMask = DEM_READINESS_SUPPORT_O2SENS_MASK;
    break;
  case DEM_CFG_READINESS_EGSENS:
    lPidSupportedMask = DEM_READINESS_SUPPORT_EGSENS_MASK;
    break;
  case DEM_CFG_READINESS_O2SENSHT:
    lPidSupportedMask = DEM_READINESS_SUPPORT_O2SENSHT_MASK;
    break;
  case DEM_CFG_READINESS_PMFLT:
    lPidSupportedMask = DEM_READINESS_SUPPORT_PMFLT_MASK;
    break;
  case DEM_CFG_READINESS_EGR:
    lPidSupportedMask = DEM_READINESS_SUPPORT_EGR_MASK;
    break;
  default:
    lReadinessGroup = DEM_CFG_READINESS_NONE;
    break;
  }

  if (lReadinessGroup != DEM_CFG_READINESS_NONE)
  {
    if (Supported == TRUE)
    {
      *PidSupportedMask |= lPidSupportedMask;
    }
    else
    {
      *PidSupportedMask &= (uint32) (~lPidSupportedMask);
    }
  }
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif

#if ((DEM_FEATURE_NEED_PID01_CALCULATION == STD_ON) || (DEM_FEATURE_NEED_PID41_CALCULATION == STD_ON) )
/* ****************************************************************************
 % Dem_Util_SetReadinessGroupCompleted
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Util_SetReadinessGroupCompleted(
  CONST(boolean, AUTOMATIC) Completed,
  CONST(uint8, AUTOMATIC)  ReadinessGroup,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_APPL_DATA) PidSupportedMask
)
{
  uint8 lReadinessGroup;
  uint32 lPidSupportedMask;

  lReadinessGroup = ReadinessGroup;
  lPidSupportedMask = DEM_READINESS_SUPPORT_NONE_MASK;

  switch (lReadinessGroup)
  {
  case DEM_CFG_READINESS_MISF:
    lPidSupportedMask = DEM_READINESS_COMPLETE_MISF_MASK;
    break;
  case DEM_CFG_READINESS_FLSYS:
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    lPidSupportedMask = DEM_READINESS_COMPLETE_FLSYS_MASK;
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    lPidSupportedMask = DEM_READINESS_COMPLETE_CMPRCMPT;
    break;
  case DEM_CFG_READINESS_CAT:
    lPidSupportedMask = DEM_READINESS_COMPLETE_CAT_MASK;
    break;
  case DEM_CFG_READINESS_HCCAT:
    lPidSupportedMask = DEM_READINESS_COMPLETE_HCCAT_MASK;
    break;
  case DEM_CFG_READINESS_HTCAT:
    lPidSupportedMask = DEM_READINESS_COMPLETE_HTCAT_MASK;
    break;
  case DEM_CFG_READINESS_NOXCAT:
    lPidSupportedMask = DEM_READINESS_COMPLETE_NOXCAT_MASK;
    break;
  case DEM_CFG_READINESS_EVAP:
    lPidSupportedMask = DEM_READINESS_COMPLETE_EVAP_MASK;
    break;
  case DEM_CFG_READINESS_SECAIR:
    lPidSupportedMask = DEM_READINESS_COMPLETE_SECAIR_MASK;
    break;
  case DEM_CFG_READINESS_BOOSTPR:
    lPidSupportedMask = DEM_READINESS_COMPLETE_BOOSTPR_MASK;
    break;
  case DEM_CFG_READINESS_O2SENS:
    lPidSupportedMask = DEM_READINESS_COMPLETE_O2SENS_MASK;
    break;
  case DEM_CFG_READINESS_EGSENS:
    lPidSupportedMask = DEM_READINESS_COMPLETE_EGSENS_MASK;
    break;
  case DEM_CFG_READINESS_O2SENSHT:
    lPidSupportedMask = DEM_READINESS_COMPLETE_O2SENSHT_MASK;
    break;
  case DEM_CFG_READINESS_PMFLT:
    lPidSupportedMask = DEM_READINESS_COMPLETE_PMFLT_MASK;
    break;
  case DEM_CFG_READINESS_EGR:
    lPidSupportedMask = DEM_READINESS_COMPLETE_EGR_MASK;
    break;
  default:
    lReadinessGroup = DEM_CFG_READINESS_NONE;
    break;
  }

  if (lReadinessGroup != DEM_CFG_READINESS_NONE)
  {
    if (Completed == TRUE)
    {
      *PidSupportedMask &= (uint32) (~lPidSupportedMask);
    }
    else
    {
      *PidSupportedMask |= lPidSupportedMask;
    }
  }
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif

#if ((DEM_FEATURE_NEED_PID41_CALCULATION == STD_ON) || (DEM_FEATURE_NEED_PID41_CCONLY_CALCULATION == STD_ON) )
/* ****************************************************************************
 % Dem_Util_SetReadinessGroupDisabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Util_SetReadinessGroupDisabled(
  CONST(uint8, AUTOMATIC)  ReadinessGroup
  )
{
  uint16 lPid41DisabledeMask;
  uint8 lReadinessGroup;

  lPid41DisabledeMask = Dem_Mem_GetPid41DisabledMask();

  if (ReadinessGroup == DEM_CFG_READINESS_FLSYS)
  {
    lReadinessGroup = DEM_CFG_READINESS_FLSYS_NONCONT;
  }
  else
  {
    lReadinessGroup = ReadinessGroup;
  }

  lPid41DisabledeMask |= (1u << lReadinessGroup);

  Dem_Mem_SetPid41DisabledMask(lPid41DisabledeMask);
}
#endif

#if (DEM_FEATURE_NEED_REPORTED_MIL_STATE == STD_ON)
/* ****************************************************************************
 % Dem_Indicator_TestMilIsActive
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Indicator_TestMilIsActive(
void
)
{
  boolean lMilIsActive;
  uint8 lMilState;

  lMilIsActive = FALSE;

  lMilState = Dem_Util_GlobalSpecialIndicatorState(Dem_Cfg_GlobalMilIndicator());

  if ( ((lMilState & DEM_INDICATOR_CONTINUOUS) != 0)
# if (DEM_CFG_REPORT_BLINKING_MIL_AS_ACTIVE == STD_ON)
    || (lMilState == DEM_INDICATOR_BLINKING)
# endif
    )
  {
    lMilIsActive = TRUE;
  }

  return lMilIsActive;
}
#endif

#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)                                                                                           /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Esm_GlobalIndicatorActivationMode
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_Esm_GlobalIndicatorActivationMode(
  void
  )
{
  Dem_IndicatorStatusType lReturnValue;

  /* calculate the current activation mode */
  if ( (Dem_Esm_QualifiedActiveEventCount[DEM_ESM_WWHOBD_DTCCLASS_A] != 0)
    || (DEM_DATA_B1COUNTER_200H <= Dem_Mem_GetB1Counter()) )
  { /* Class A event currently active or B1 counter exceeds threshold */
    lReturnValue = DEM_INDICATOR_CONTINUOUS;
  }
  else if ( (Dem_Esm_DtcClassMILCount[DEM_ESM_WWHOBD_DTCCLASS_A]  != 0)
         || (Dem_Esm_DtcClassMILCount[DEM_ESM_WWHOBD_DTCCLASS_B1] != 0)
         || (Dem_Esm_DtcClassMILCount[DEM_ESM_WWHOBD_DTCCLASS_B2] != 0) )
  {
    lReturnValue = DEM_INDICATOR_SHORT;
  }
  else if (Dem_Esm_DtcClassMILCount[DEM_ESM_WWHOBD_DTCCLASS_C] != 0)
  {
    lReturnValue = DEM_INDICATOR_ON_DEMAND;
  }
  else /* DEM_ESM_WWHOBD_DTCCLASS_NO */
  {
    lReturnValue = DEM_INDICATOR_OFF;
  }
  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)                                                                                           /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Esm_AddAgingTime
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Esm_AddAgingTime(
  CONST(uint32, AUTOMATIC)  CurrentTime,
  CONST(uint16, AUTOMATIC)  DeltaValue
  )
{
  uint32 lTargetTime;

  lTargetTime = (uint32)(CurrentTime + DeltaValue);

  return lTargetTime;
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Esm_EventCreateTimeSeriesEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Esm_EventCreateTimeSeriesEntry(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_EventIdType lMasterEventId;
  Dem_Cfg_MemoryIndexType lMemoryIndex;

  /* check if there is already a time series entry for this DTC */
  lMasterEventId = Dem_Cfg_GetMasterEvent(EventId);
  lMemoryIndex = Dem_Mem_TimeSeriesFindIndex(lMasterEventId);

  if (lMemoryIndex == DEM_MEM_INVALID_MEMORY_INDEX)
  {
    /* get time series entry for this event if possible */
    lMemoryIndex = Dem_Mem_TimeSeriesAllocateIndex(EventId);

    if (lMemoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
    {
# if (DEM_FEATURE_NEED_TIME_SERIES_PAST == STD_ON)
      Dem_Data_StoreTimeSeriesPastSamples(EventId, lMemoryIndex);
# endif

# if (DEM_FEATURE_NEED_TIME_SERIES_FUTURE == STD_OFF)
      if (Dem_Cfg_EventSupportImmediateNv(EventId) == TRUE)
      {
        Dem_Nvm_SetMemoryBlockState(Dem_MemStorageManager_GetNvBlockType(lMemoryIndex),
                                    Dem_MemStorageManager_GetEntryIndex(lMemoryIndex),
                                    DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
      }
      else
# endif
      {
        Dem_Nvm_SetMemoryBlockState(Dem_MemStorageManager_GetNvBlockType(lMemoryIndex),
                                    Dem_MemStorageManager_GetEntryIndex(lMemoryIndex),
                                    DEM_NVM_BLOCKSTATE_DIRTY);
      }

# if (DEM_FEATURE_NEED_TIME_SERIES_FUTURE == STD_ON)
      {
        Dem_Mem_MemoryInfoPtrType lMemoryInfo;

        lMemoryInfo = Dem_MemStorageManager_MemoryInfoInit(DEM_CFG_MEMORYID_TIMESERIES);
#  if (DEM_CFG_SUPPORT_TIME_SERIES_FUTURE_NORMAL == STD_ON)
        /* determine missing future samples */
        Dem_Mem_SetMissingTimeSeriesNormalRate((lMemoryIndex - Dem_MemStorageManager_MemoryIter_Begin(lMemoryInfo)), Dem_Cfg_TimeSeriesFutureSamplesNormal());  /* SBSW_DEM_CALL_MEMORYINFO_POINTER */
#  endif
#  if (DEM_CFG_SUPPORT_TIME_SERIES_FUTURE_FAST == STD_ON)
        if (Dem_Cfg_EventSupportTimeSeriesFast(EventId) == TRUE)
        {
          Dem_Mem_SetMissingTimeSeriesFastRate((lMemoryIndex - Dem_MemStorageManager_MemoryIter_Begin(lMemoryInfo)), Dem_Cfg_TimeSeriesFutureSamplesFast());  /* SBSW_DEM_CALL_MEMORYINFO_POINTER */
        }
#  endif
      }
# endif
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_FEATURE_NEED_GLOBAL_MIL_STATE == STD_ON)
/* ****************************************************************************
 % Dem_Esm_GlobalMilStateEnable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Esm_GlobalMilStateEnable(
  void
  )
{
  /* The MIL Indicator state has changed from FALSE to TRUE, the current Odometer value shall be fetched */
# if (DEM_CFG_SUPPORT_PID21 == STD_ON)
/* >>>> -------------------------------- Enter Critical Section: DcmApi */
  Dem_EnterCritical_DcmApi();                                                                                                    /* PRQA S 3109 */ /* MD_MSR_14.3 */

  Dem_Mem_SetOdometerMilOn(DEM_MEM_SET_ODOMETER_INVALID(0));
  Dem_LeaveCritical_DcmApi();                                                                                                    /* PRQA S 3109 */ /* MD_MSR_14.3 */
/* <<<< -------------------------------- Leave Critical Section: DcmApi */
# endif
# if (DEM_CFG_SUPPORT_PID4D == STD_ON)
  Dem_Mem_SetEngineTimeMilOn(0);
# endif
}
#endif /* (DEM_FEATURE_NEED_GLOBAL_MIL_STATE == STD_ON) */

#if (DEM_FEATURE_NEED_OBD == STD_ON) && (DEM_CFG_SUPPORT_PID21 == STD_ON)                                                        /* COV_MSR_UNSUPPORTED XF xf xf */
/* ****************************************************************************
 % Dem_Esm_GlobalMilStateDisable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Esm_GlobalMilStateDisable(
  void
  )
{
  /* If the Indicator state changes from on to off, the current Odometer value is set to invalid */
/* >>>> -------------------------------- Enter Critical Section: DcmApi */
  Dem_EnterCritical_DcmApi();                                                                                                    /* PRQA S 3109 */ /* MD_MSR_14.3 */

  Dem_Mem_SetOdometerMilOn(DEM_MEM_SET_ODOMETER_INVALID(Dem_Mem_GetOdometerMilOn()));

  Dem_LeaveCritical_DcmApi();                                                                                                    /* PRQA S 3109 */ /* MD_MSR_14.3 */
/* <<<< -------------------------------- Leave Critical Section: DcmApi */
}
#endif /* (DEM_FEATURE_NEED_OBD == STD_ON) && (DEM_CFG_SUPPORT_PID21 == STD_ON) */

#if (DEM_CFG_SUPPORT_USER_INDICATORS == STD_ON)
/* ****************************************************************************
 % Dem_Esm_UserIndicatorEnable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Esm_UserIndicatorEnable(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Cfg_EventIndicatorIterType lIndicatorIter;

  for (Dem_Cfg_EventIndicatorIterInit(EventId, &lIndicatorIter);                                                                 /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_EventIndicatorIterExists(&lIndicatorIter) == TRUE;                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_EventIndicatorIterNext(&lIndicatorIter))                                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_IndicatorIndexType lIndicatorIndex;
# if (DEM_CFG_SUPPORT_J1939_NODES == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED XF */
    Dem_Cfg_EventJ1939NodeIterType lNodeIter;
    uint8 DEM_J1939_NODE_ID;
# endif
    lIndicatorIndex = Dem_Cfg_EventIndicatorIterGet(&lIndicatorIter);                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
    DEM_IGNORE_UNUSED_VARIABLE(lIndicatorIndex)                                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */

# if (DEM_CFG_SUPPORT_J1939_NODES == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED XF */
    for (Dem_Cfg_EventJ1939NodeIterInit(EventId, &lNodeIter);                                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_EventJ1939NodeIterExists(&lNodeIter) == TRUE;                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_EventJ1939NodeIterNext(&lNodeIter))                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
# endif
    {
      /* Increment the (node specific) indicator counter for continuous or blinking, according to configuration */
# if (DEM_CFG_SUPPORT_J1939_NODES == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED XF */
      DEM_J1939_NODE_ID = Dem_Cfg_EventJ1939NodeIterGet(&lNodeIter);                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
# endif
      if ((DEM_INDICATOR_CONTINUOUS & Dem_Cfg_IndicatorStatus(lIndicatorIndex)) != 0)
      {
        Dem_Mem_IndicatorSetEventCountContinuous(Dem_Cfg_IndicatorId(lIndicatorIndex), DEM_J1939_NODE_ID,
          Dem_Mem_IndicatorGetEventCountContinuous(Dem_Cfg_IndicatorId(lIndicatorIndex), DEM_J1939_NODE_ID) + 1);
      }
      if ((DEM_INDICATOR_BLINKING & Dem_Cfg_IndicatorStatus(lIndicatorIndex)) != 0)
      {
        Dem_Mem_IndicatorSetEventCountBlinking(Dem_Cfg_IndicatorId(lIndicatorIndex), DEM_J1939_NODE_ID,
          Dem_Mem_IndicatorGetEventCountBlinking(Dem_Cfg_IndicatorId(lIndicatorIndex), DEM_J1939_NODE_ID) + 1);
      }
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif /* (DEM_CFG_SUPPORT_USER_INDICATORS == STD_ON) */

#if (DEM_FEATURE_NEED_SPECIAL_INDICATORS == STD_ON)                                                                              /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Esm_SpecialIndicatorEnable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Esm_SpecialIndicatorEnable(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId                                                                                     /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  uint8 lSpecialIndicator;
  /* EventId might not be used depending on configuration optimization */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  lSpecialIndicator = Dem_Cfg_EventSpecialIndicator(EventId);
  if (lSpecialIndicator != DEM_CFG_SPECIAL_INDICATOR_NONE)
  {
# if (DEM_CFG_SUPPORT_J1939_NODES == STD_ON)
    Dem_Cfg_EventJ1939NodeIterType lNodeIter;

    for (Dem_Cfg_EventJ1939NodeIterInit(EventId, &lNodeIter);                                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_EventJ1939NodeIterExists(&lNodeIter) == TRUE;                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_EventJ1939NodeIterNext(&lNodeIter))                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
# endif
    {
# if (DEM_CFG_SUPPORT_J1939_NODES == STD_ON)
      uint8 DEM_J1939_NODE_ID;

      DEM_J1939_NODE_ID = Dem_Cfg_EventJ1939NodeIterGet(&lNodeIter);                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
# endif
      switch (Dem_Cfg_EventSpecialIndicatorState(EventId))
      {
      case DEM_INDICATOR_CONTINUOUS:
        Dem_Mem_IndicatorSetEventCountContinuous(Dem_Cfg_EventSpecialIndicator(EventId), DEM_J1939_NODE_ID,
          Dem_Mem_IndicatorGetEventCountContinuous(Dem_Cfg_EventSpecialIndicator(EventId), DEM_J1939_NODE_ID) + 1);
        break;
# if (DEM_CFG_SUPPORT_J1939 == STD_ON)
      case DEM_INDICATOR_FAST_FLASH:
        Dem_Cfg_IndicatorFastFlash[Dem_Cfg_EventSpecialIndicator(EventId)][DEM_J1939_NODE_ID] =
          (uint16)(Dem_Cfg_IndicatorFastFlash[Dem_Cfg_EventSpecialIndicator(EventId)][DEM_J1939_NODE_ID] + 1);
        break;
      case DEM_INDICATOR_SLOW_FLASH:
        Dem_Cfg_IndicatorSlowFlash[Dem_Cfg_EventSpecialIndicator(EventId)][DEM_J1939_NODE_ID] =
          (uint16)(Dem_Cfg_IndicatorSlowFlash[Dem_Cfg_EventSpecialIndicator(EventId)][DEM_J1939_NODE_ID] + 1);
        break;
# endif /* (DEM_CFG_SUPPORT_J1939 == STD_ON) */
# if (DEM_CFG_SUPPORT_J1939 != STD_ON)
      case DEM_INDICATOR_BLINKING:
        Dem_Mem_IndicatorSetEventCountBlinking(Dem_Cfg_EventSpecialIndicator(EventId), DEM_J1939_NODE_ID,
          Dem_Mem_IndicatorGetEventCountBlinking(Dem_Cfg_EventSpecialIndicator(EventId), DEM_J1939_NODE_ID) + 1);
        break;
      case DEM_INDICATOR_BLINK_CONT:
        Dem_Mem_IndicatorSetEventCountContinuous(Dem_Cfg_EventSpecialIndicator(EventId), DEM_J1939_NODE_ID,
          Dem_Mem_IndicatorGetEventCountContinuous(Dem_Cfg_EventSpecialIndicator(EventId), DEM_J1939_NODE_ID) + 1);
        Dem_Mem_IndicatorSetEventCountBlinking(Dem_Cfg_EventSpecialIndicator(EventId), DEM_J1939_NODE_ID,
          Dem_Mem_IndicatorGetEventCountBlinking(Dem_Cfg_EventSpecialIndicator(EventId), DEM_J1939_NODE_ID) + 1);
        break;
# endif
      default:
        Dem_Error_ReportError(DEM_INTERNAL_APIID, DEM_E_INCONSISTENT_STATE);
        break;
      }
    }
# if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
    if (lSpecialIndicator == Dem_Cfg_GlobalMilIndicator())
    {
      ++Dem_Esm_DtcClassMILCount[Dem_Util_SeverityWwhObdDtcClass(Dem_Cfg_EventSeverity(EventId))];
    }
# endif
  }

  return lSpecialIndicator;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_USER_INDICATORS == STD_ON)
/* ****************************************************************************
 % Dem_Esm_UserIndicatorDisable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Esm_UserIndicatorDisable(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Cfg_EventIndicatorIterType lIndicatorIter;

  for (Dem_Cfg_EventIndicatorIterInit(EventId, &lIndicatorIter);                                                                 /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_EventIndicatorIterExists(&lIndicatorIter) == TRUE;                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_EventIndicatorIterNext(&lIndicatorIter))                                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_IndicatorIndexType lIndicatorIndex;
# if (DEM_CFG_SUPPORT_J1939_NODES == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED XF */
    Dem_Cfg_EventJ1939NodeIterType lNodeIter;
# endif

    lIndicatorIndex = Dem_Cfg_EventIndicatorIterGet(&lIndicatorIter);                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
    DEM_IGNORE_UNUSED_VARIABLE(lIndicatorIndex)                                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */

# if (DEM_CFG_SUPPORT_J1939_NODES == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED XF */
    /* Process all nodes linked to the event */
    for (Dem_Cfg_EventJ1939NodeIterInit(EventId, &lNodeIter);                                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_EventJ1939NodeIterExists(&lNodeIter) == TRUE;                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_EventJ1939NodeIterNext(&lNodeIter))                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
# endif
    {
# if (DEM_CFG_SUPPORT_J1939_NODES == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED XF */
      uint8 DEM_J1939_NODE_ID;

      DEM_J1939_NODE_ID = Dem_Cfg_EventJ1939NodeIterGet(&lNodeIter);                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
# endif
      if ((Dem_Cfg_IndicatorStatus(lIndicatorIndex) & DEM_INDICATOR_CONTINUOUS) != 0)
      {
        Dem_Mem_IndicatorSetEventCountContinuous(Dem_Cfg_IndicatorId(lIndicatorIndex), DEM_J1939_NODE_ID,
          Dem_Mem_IndicatorGetEventCountContinuous(Dem_Cfg_IndicatorId(lIndicatorIndex), DEM_J1939_NODE_ID) - 1);
      }
      if ((Dem_Cfg_IndicatorStatus(lIndicatorIndex) & DEM_INDICATOR_BLINKING) != 0)
      {
        Dem_Mem_IndicatorSetEventCountBlinking(Dem_Cfg_IndicatorId(lIndicatorIndex), DEM_J1939_NODE_ID,
          Dem_Mem_IndicatorGetEventCountBlinking(Dem_Cfg_IndicatorId(lIndicatorIndex), DEM_J1939_NODE_ID) - 1);
      }
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif /* DEM_CFG_SUPPORT_USER_INDICATORS */

#if (DEM_FEATURE_NEED_SPECIAL_INDICATORS == STD_ON)                                                                              /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Esm_SpecialIndicatorDisable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Esm_SpecialIndicatorDisable(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId                                                                                     /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  boolean lSpecialIndicator;

  /* EventId might not be used depending on configuration */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  lSpecialIndicator = Dem_Cfg_EventSpecialIndicator(EventId);
  if (lSpecialIndicator != DEM_CFG_SPECIAL_INDICATOR_NONE)
  {
# if (DEM_CFG_SUPPORT_J1939_NODES == STD_ON)
    Dem_Cfg_EventJ1939NodeIterType lNodeIter;

    for (Dem_Cfg_EventJ1939NodeIterInit(EventId, &lNodeIter);                                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_EventJ1939NodeIterExists(&lNodeIter) == TRUE;                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_EventJ1939NodeIterNext(&lNodeIter))                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
# endif
    {
# if (DEM_CFG_SUPPORT_J1939_NODES == STD_ON)
      uint8 DEM_J1939_NODE_ID;

      DEM_J1939_NODE_ID = Dem_Cfg_EventJ1939NodeIterGet(&lNodeIter);                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
# endif
      switch (Dem_Cfg_EventSpecialIndicatorState(EventId))
      {
      case DEM_INDICATOR_CONTINUOUS:
        Dem_Mem_IndicatorSetEventCountContinuous(lSpecialIndicator, DEM_J1939_NODE_ID,
          Dem_Mem_IndicatorGetEventCountContinuous(lSpecialIndicator, DEM_J1939_NODE_ID) - 1);
        break;
# if (DEM_CFG_SUPPORT_J1939 == STD_ON)
      case DEM_INDICATOR_FAST_FLASH:
        Dem_Cfg_IndicatorFastFlash[lSpecialIndicator][DEM_J1939_NODE_ID] =
          (uint16)(Dem_Cfg_IndicatorFastFlash[lSpecialIndicator][DEM_J1939_NODE_ID] - 1);
        break;
      case DEM_INDICATOR_SLOW_FLASH:
        Dem_Cfg_IndicatorSlowFlash[lSpecialIndicator][DEM_J1939_NODE_ID] =
          (uint16)(Dem_Cfg_IndicatorSlowFlash[lSpecialIndicator][DEM_J1939_NODE_ID] - 1);
        break;
# endif /* (DEM_CFG_SUPPORT_J1939 == STD_ON) */
# if (DEM_CFG_SUPPORT_J1939 != STD_ON)
      case DEM_INDICATOR_BLINKING:
        Dem_Mem_IndicatorSetEventCountBlinking(lSpecialIndicator, DEM_J1939_NODE_ID,
          Dem_Mem_IndicatorGetEventCountBlinking(lSpecialIndicator, DEM_J1939_NODE_ID) - 1);
        break;
      case DEM_INDICATOR_BLINK_CONT:
        Dem_Mem_IndicatorSetEventCountContinuous(lSpecialIndicator, DEM_J1939_NODE_ID,
          Dem_Mem_IndicatorGetEventCountContinuous(lSpecialIndicator, DEM_J1939_NODE_ID) - 1);
        Dem_Mem_IndicatorSetEventCountBlinking(lSpecialIndicator, DEM_J1939_NODE_ID,
          Dem_Mem_IndicatorGetEventCountBlinking(lSpecialIndicator, DEM_J1939_NODE_ID) - 1);
        break;
# endif
      default:
        Dem_Error_ReportError(DEM_INTERNAL_APIID, DEM_E_INCONSISTENT_STATE);
        break;
      }
    }
# if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
    if (lSpecialIndicator == Dem_Cfg_GlobalMilIndicator())
    {
      --Dem_Esm_DtcClassMILCount[Dem_Util_SeverityWwhObdDtcClass(Dem_Cfg_EventSeverity(EventId))];
    }
# endif
  }

  return lSpecialIndicator;
}                                                                                                                                /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */
#endif /* (DEM_FEATURE_NEED_SPECIAL_INDICATORS == STD_ON) */

#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)                                                                                           /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Esm_IncrementQualifiedActiveEventCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Esm_IncrementQualifiedActiveEventCount(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  if ( (Dem_Cfg_EventObdRelated(EventId) == TRUE)                                                                                /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
    && (Dem_EventInternalStatus_Test_QualifyStatus_CDTC(Dem_Event_GetQualifyStatus(EventId)) == TRUE)
    && (Dem_UDSStatus_Test_TF(Dem_Event_GetUDSStatus(EventId)) == TRUE) )
  {
    ++Dem_Esm_QualifiedActiveEventCount[Dem_Util_SeverityWwhObdDtcClass(Dem_Cfg_EventSeverity(EventId))];

    /* if continuous MI counter is not active yet and activation mode 4 becomes active , set to Run state */
    if ( (Dem_Mem_GetContinuousMiHealingCounter() != DEM_DATA_CONTINUOUS_MI_HEALING_COUNTER_INVALID)                             /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
      && (Dem_Esm_GlobalIndicatorActivationMode() == DEM_INDICATOR_CONTINUOUS) )
    {
      /* if continuous MI counter was healed and is currently aging, reset continuous MI counter */
      if (Dem_Mem_GetContinuousMiAgingCounter() < DEM_DATA_CONTINUOUS_MI_AGING_COUNTER_AGED)
      {
        Dem_Mem_SetContinuousMiCounter(0);
      }
      Dem_Mem_SetContinuousMiHealingCounter(DEM_DATA_CONTINUOUS_MI_HEALING_COUNTER_INVALID);
      Dem_Mem_SetContinuousMiAgingCounter(DEM_DATA_CONTINUOUS_MI_AGING_COUNTER_INVALID);
      Dem_Mem_SetContinuousMiAgingTimer(DEM_DATA_CONTINUOUS_MI_AGING_TIMER_INVALID);
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)                                                                                           /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Esm_DecrementQualifiedActiveEventCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Esm_DecrementQualifiedActiveEventCount(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(boolean, AUTOMATIC) StartB1Healing
  )
{
  if ( (Dem_Cfg_EventObdRelated(EventId) == TRUE)                                                                                /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
    && (Dem_EventInternalStatus_Test_QualifyStatus_CDTC(Dem_Event_GetQualifyStatus(EventId)) == TRUE) )
  {
    uint16 lEventCountB1Old;

    lEventCountB1Old = Dem_Esm_QualifiedActiveEventCount[DEM_ESM_WWHOBD_DTCCLASS_B1];

    --Dem_Esm_QualifiedActiveEventCount[Dem_Util_SeverityWwhObdDtcClass(Dem_Cfg_EventSeverity(EventId))];

    if ( (lEventCountB1Old != 0)
      && (Dem_Esm_QualifiedActiveEventCount[DEM_ESM_WWHOBD_DTCCLASS_B1] == 0) )
    {
      if (Dem_Mem_GetB1Counter() >= DEM_DATA_B1COUNTER_200H)
      { /* set B1Counter to 190h (11400min) if no B1 event is confirmed and failed and B1 counter is greater than 200h (12000min)
        also clear the healing counter */
        Dem_Mem_SetB1Counter(DEM_DATA_B1COUNTER_190H);
      }

      if (StartB1Healing == TRUE)
      {
        Dem_Mem_SetB1HealingCounter(0);
      }
      else
      {
        Dem_Mem_SetB1HealingCounter(DEM_DATA_B1_HEALING_COUNTER_THRESHOLD);
      }
    }

    /* prepare continuous MI Healing if activation mode 4 is not active */
    if ( (Dem_Mem_GetContinuousMiHealingCounter() == DEM_DATA_CONTINUOUS_MI_HEALING_COUNTER_INVALID)                             /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
      && (Dem_Esm_GlobalIndicatorActivationMode() != DEM_INDICATOR_CONTINUOUS) )
    {
      Dem_Mem_SetContinuousMiHealingCounter(DEM_DATA_CONTINUOUS_MI_HEALING_COUNTER_LOCKED);
    }
  }
}
#endif

#if (DEM_FEATURE_NEED_IUMPR == STD_ON)                                                                                           /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Esm_ResetIumprFidPending
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Esm_ResetIumprFidPending(
  void
  )
{
  Dem_Cfg_ObdRatioIdIterType lRatioIter;
  uint8 lLeaveCSCounter;

  lLeaveCSCounter = 0;
/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
  Dem_EnterCritical_DiagMonitor();                                                                                               /* PRQA S 3109 */ /* MD_MSR_14.3 */

  for (Dem_Cfg_ObdRatioIdIterInit(&lRatioIter);                                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_ObdRatioIdIterExists(&lRatioIter) == TRUE;                                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_ObdRatioIdIterNext(&lRatioIter))                                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_RatioIdIndexType lRatioIndex;
    lRatioIndex = Dem_Cfg_ObdRatioIdIterGet(&lRatioIter);                                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Mem_SetIumprStatus(lRatioIndex, DEM_ESM_RESET_IUMPR_FID_PENDING(Dem_Mem_GetIumprStatus(lRatioIndex)));

    ++lLeaveCSCounter;
    if (lLeaveCSCounter >= DEM_CFG_IUMPR_RESET_PENDING_RATIO)
    {
      Dem_LeaveCritical_DiagMonitor();                                                                                           /* PRQA S 3109 */ /* MD_MSR_14.3 */
      lLeaveCSCounter = 0;
      Dem_EnterCritical_DiagMonitor();                                                                                           /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

  Dem_LeaveCritical_DiagMonitor();                                                                                               /* PRQA S 3109 */ /* MD_MSR_14.3 */
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

  Dem_Nvm_SetBlockState(DEM_NVM_BLOCKTYPE_IUMPR, DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_FEATURE_NEED_IUMPR == STD_ON)                                                                                           /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Esm_RatioQueueAsync
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Esm_RatioQueueAsync(
  CONST(Dem_RatioIdIndexType, AUTOMATIC)  RatioIndex
  )
{
  /* only trigger increment of numerator if it is not yet incremented */
  uint8   lRatioStatus;

  lRatioStatus = Dem_Mem_GetIumprStatus(RatioIndex);

  if (DEM_ESM_TEST_IUMPR_NUM_INCREMENTED(lRatioStatus) == FALSE)
  {
    /* set fault detect flag */
    lRatioStatus = DEM_ESM_SET_IUMPR_NUM_FAULTDETECT(lRatioStatus);
    Dem_Mem_SetIumprStatus(RatioIndex, lRatioStatus);

    /* set pending flags --> check for increment on task level */
    Dem_Mem_IumprSetUpdatePending(RatioIndex, TRUE);
    Dem_Scheduler_EnableTaskOnce(Dem_Scheduler_Task_IUMPR);
  }
}
#endif /* (DEM_FEATURE_NEED_IUMPR == STD_ON) */

#if (DEM_FEATURE_NEED_OBD == STD_ON) && (DEM_CFG_SUPPORT_RESTART_DCY_ON_CLEAR_DTC == STD_ON)                                     /* COV_MSR_UNSUPPORTED XF xf xf */
/* ****************************************************************************
 % Dem_Esm_PreOnClear_AllDtc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Esm_PreOnClear_AllDtc(
  CONST(uint8, AUTOMATIC)  MemoryId
  )
{
  if (MemoryId == DEM_CFG_MEMORYID_PRIMARY)
  {
    Dem_OperationCycle_CycleUpdate(Dem_Cfg_GlobalObdDrivingCycleId(), DEM_OPERATIONCYCLE_CYCLE_RESTARTED);
  }
}
#endif

/* ****************************************************************************
 % Dem_Esm_PostOnClear_AllDtc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Esm_PostOnClear_AllDtc(
  CONST(uint8, AUTOMATIC)  MemoryId
  )
{
#if (DEM_FEATURE_NEED_OVFLIND == STD_ON)
  Dem_Mem_ResetOverflow(MemoryId);
#endif

  if (MemoryId == DEM_CFG_MEMORYID_PRIMARY)
  {
    Dem_Mem_SetFirstFailedEvent(DEM_EVENT_INVALID);
    Dem_Mem_SetFirstConfirmedEvent(DEM_EVENT_INVALID);
    Dem_Mem_SetMostRecentFailedEvent(DEM_EVENT_INVALID);
    Dem_Mem_SetMostRecentConfmdEvent(DEM_EVENT_INVALID);
    Dem_DtrIF_ClearDtrOfEvent(DEM_EVENT_INVALID);
  }
}

/* ****************************************************************************
 % Dem_Esm_PostOnClear_Always
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Esm_PostOnClear_Always(
  CONST(uint8, AUTOMATIC)  MemoryId                                                                                              /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(MemoryId)                                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */

#if (DEM_FEATURE_NEED_OBD == STD_ON)                                                                                             /* COV_MSR_UNSUPPORTED XF */
  if (MemoryId == DEM_CFG_MEMORYID_PRIMARY)
  {
# if (DEM_FEATURE_NEED_GLOBAL_MIL_STATE == STD_ON)
    if (Dem_Indicator_TestMilIsActive() == FALSE)
    {
#  if (DEM_CFG_SUPPORT_PID21 == STD_ON)
/* >>>> -------------------------------- Enter Critical Section: DcmApi */
      Dem_EnterCritical_DcmApi();                                                                                                /* PRQA S 3109 */ /* MD_MSR_14.3 */
      /* Clear Odometer for MIL on */
      Dem_Mem_SetOdometerMilOn(0);
      Dem_LeaveCritical_DcmApi();                                                                                                /* PRQA S 3109 */ /* MD_MSR_14.3 */
/* <<<< -------------------------------- Leave Critical Section: DcmApi */
#  endif
#  if (DEM_CFG_SUPPORT_PID4D == STD_ON)
      Dem_Mem_SetEngineTimeMilOn(0);
#  endif
    }
# endif
# if (DEM_FEATURE_NEED_IUMPR == STD_ON)
    Dem_Esm_ResetIumprFidPending();
# endif
# if (DEM_CFG_SUPPORT_PID30 == STD_ON)
    /* Clear WarmUpCycle since last clear counter */
    Dem_Mem_SetWarmUpCountSinceClear(0x00U);
# endif
# if (DEM_CFG_SUPPORT_PID31 == STD_ON)
    /* Set current Odometer value to invalid */
    Dem_Mem_SetOdometerAtClear(DEM_MEM_SET_ODOMETER_INVALID(0));
# endif
# if (DEM_CFG_SUPPORT_PID4E == STD_ON)
    Dem_Mem_SetEngineTimeSinceClear(0x0000U);
# endif
# if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* >>>> -------------------------------- Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();                                                                                            /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Dem_OperationCycle_SetObdCycleStates(DEM_OPERATIONCYCLE_RESET_PFC_CYCLE(Dem_OperationCycle_GetObdCycleStates()));
    Dem_LeaveCritical_StateManager();                                                                                            /* PRQA S 3109 */ /* MD_MSR_14.3 */
/* <<<< -------------------------------- Leave Critical Section: StateManager */

# endif
# if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
    Dem_Mem_SetContinuousMiAgingCounter(DEM_DATA_CONTINUOUS_MI_AGING_COUNTER_AGED);
    Dem_Mem_SetContinuousMiHealingCounter(DEM_DATA_CONTINUOUS_MI_HEALING_COUNTER_HEALED);
    Dem_Mem_SetContinuousMiAgingTimer(DEM_DATA_CONTINUOUS_MI_AGING_TIMER_AGED);
    Dem_Mem_SetContinuousMiCounter(0);
# endif
# if (DEM_FEATURE_NEED_PID01_CALCULATION  == STD_ON)
    /* all readiness groups are set to not complete -> reserved bits are set to 0 */
    if ((Dem_Cfg_GlobalPid01SupportedMask() & DEM_READINESS_SUPPORT_COMPRESSION) != 0)
    {
      Dem_Mem_SetPid01CompletedMask(DEM_READINESS_INITIALIZE_PID01COMPLETED_COMPRESSION);
    }
    else
    {
      Dem_Mem_SetPid01CompletedMask(DEM_READINESS_INITIALIZE_PID01COMPLETED_SPARK);
    }

/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();                                                                                             /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* set flag for all readiness group supported by engine type to recalculate monitor state -> not supported groups will be set to N/A = completed*/
    if ((Dem_Cfg_GlobalPid01SupportedMask() & DEM_READINESS_SUPPORT_COMPRESSION) != 0)
    {
      Dem_Queue_SetPid01UpdateFlag(DEM_READINESS_INITIALIZE_UPDATE_COMPRESSION);
    }
    else
    {
      Dem_Queue_SetPid01UpdateFlag(DEM_READINESS_INITIALIZE_UPDATE_SPARK);
    }

    Dem_LeaveCritical_DiagMonitor();                                                                                             /* PRQA S 3109 */ /* MD_MSR_14.3 */
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */
# endif
  }
#endif /* DEM_FEATURE_NEED_OBD == STD_ON */

#if (DEM_CFG_SUPPORT_DEBOUNCE_NV == STD_ON)
  /* copy debounce values from debounce array to nvram mirror */
  Dem_Mem_CopyDataDebounceNv();
  Dem_Nvm_SetBlockState(DEM_NVM_BLOCKTYPE_DEBOUNCE, DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);
#endif

  Dem_Nvm_SetBlockState(DEM_NVM_BLOCKTYPE_ADMIN, DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Esm_InitWwhObdStatistics
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Esm_InitWwhObdStatistics(
  void
  )
{
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
  {
    uint8 lDtcClassIndex;

    lDtcClassIndex = DEM_ESM_WWHOBD_NUMBER_DTCCLASS;

    while (lDtcClassIndex != 0)
    {
      --lDtcClassIndex;

      Dem_Esm_QualifiedActiveEventCount[lDtcClassIndex] = 0;
      Dem_Esm_DtcClassMILCount[lDtcClassIndex] = 0;
    }
  }
#endif
}

/* ****************************************************************************
 % Dem_Esm_InitSpecialIndicator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Esm_InitSpecialIndicator(
  void
  )
{
#if (DEM_FEATURE_NEED_OBD == STD_ON)                                                                                             /* COV_MSR_UNSUPPORTED XF */
  Dem_Mem_UpdateGlobalSpecialIndicatorStates(Dem_Cfg_GlobalMilIndicator());
#endif
#if (DEM_CFG_SUPPORT_J1939 == STD_ON)                                                                                            /* COV_MSR_UNSUPPORTED XF */
# if (DEM_CFG_SUPPORT_RSL == STD_ON)
  Dem_Mem_UpdateGlobalSpecialIndicatorStates(Dem_Cfg_GlobalRslIndicator());
# endif
# if (DEM_CFG_SUPPORT_AWL == STD_ON)
  Dem_Mem_UpdateGlobalSpecialIndicatorStates(Dem_Cfg_GlobalAwlIndicator());
# endif
# if (DEM_CFG_SUPPORT_PL == STD_ON)
  Dem_Mem_UpdateGlobalSpecialIndicatorStates(Dem_Cfg_GlobalPlIndicator());
# endif
#endif
}                                                                                                                                /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Esm_GetIndicatorStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_Esm_GetIndicatorStatus(
  CONST(uint8, AUTOMATIC)  IndicatorId
  )
{
  Dem_IndicatorStatusType lIndicatorStatus;
#if (DEM_FEATURE_NEED_SPECIAL_INDICATORS == STD_ON)
  if ( (Dem_Cfg_GlobalMilIndicator() == IndicatorId)                                                                             /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
    || (Dem_Cfg_GlobalRslIndicator() == IndicatorId)
    || (Dem_Cfg_GlobalAwlIndicator() == IndicatorId)
    || (Dem_Cfg_GlobalPlIndicator() == IndicatorId) )
  {
# if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
    lIndicatorStatus = Dem_Esm_GlobalIndicatorActivationMode();                                                                  /* SBSW_DEM_POINTER_WRITE_API */
# else
    lIndicatorStatus = Dem_Util_GlobalSpecialIndicatorState(IndicatorId);                                                        /* SBSW_DEM_POINTER_WRITE_API */
# endif
  }
  else
#endif
  {
    lIndicatorStatus = DEM_INDICATOR_OFF;

/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();                                                                                             /* PRQA S 3109 */ /* MD_MSR_14.3 */
    {
#if (DEM_CFG_SUPPORT_J1939_NODES == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED XF */
      uint8 DEM_J1939_NODE_ID;

      DEM_J1939_NODE_ID = Dem_Cfg_GlobalJ1939NodeCount();

      while (DEM_J1939_NODE_ID != 0)
#endif
      {
#if (DEM_CFG_SUPPORT_J1939_NODES == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED XF */
        --DEM_J1939_NODE_ID;
#endif
        if (Dem_Mem_IndicatorGetEventCountContinuous(IndicatorId, DEM_J1939_NODE_ID) > 0)
        {
          lIndicatorStatus = (uint8)(lIndicatorStatus | DEM_INDICATOR_CONTINUOUS);
        }
        if (Dem_Mem_IndicatorGetEventCountBlinking(IndicatorId, DEM_J1939_NODE_ID) > 0)
        {
          lIndicatorStatus = (uint8)(lIndicatorStatus | DEM_INDICATOR_BLINKING);
        }
      }
    }

    Dem_LeaveCritical_DiagMonitor();                                                                                             /* PRQA S 3109 */ /* MD_MSR_14.3 */
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */
  }
  return lIndicatorStatus;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (DEM_FEATURE_NEED_PID01_CALCULATION == STD_ON)
/* ****************************************************************************
% Dem_Esm_SetPid01ReadinessState
*****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Esm_SetPid01ReadinessState(
  CONST(uint16, AUTOMATIC) State,
  CONST(uint8, AUTOMATIC) ReadinessGroup
  )
{
  uint16 lReturnValue;
  lReturnValue = State;
  if ( (ReadinessGroup != DEM_CFG_READINESS_NONE)
    && (ReadinessGroup != DEM_CFG_READINESS_FLSYS))
  {
    lReturnValue |= (uint16)(1U << ReadinessGroup);
  }
  return lReturnValue;
}
#endif

#if (DEM_FEATURE_NEED_PID01_CALCULATION == STD_ON)
/* ****************************************************************************
% Dem_Esm_ResetPid01ReadinessState
*****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Esm_ResetPid01ReadinessState(
  CONST(uint16, AUTOMATIC) State,
  CONST(uint8, AUTOMATIC) ReadinessGroup
  )
{
  uint16 lReturnValue;
  lReturnValue = State;
  if (ReadinessGroup != DEM_CFG_READINESS_NONE)
  {
    lReturnValue &= (uint16)~(1U << ReadinessGroup);
  }
  return lReturnValue;
}
#endif

#if (DEM_FEATURE_NEED_PID01_CALCULATION == STD_ON)
/* ****************************************************************************
% Dem_Esm_TestPid01ReadinessState
*****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Esm_TestPid01ReadinessState(
  CONST(uint16, AUTOMATIC) State,
  CONST(uint8, AUTOMATIC) ReadinessGroup
  )
{
  boolean lReturnValue;
  lReturnValue = FALSE;
  if (ReadinessGroup != DEM_CFG_READINESS_NONE)
  {
    if ((State & ((uint16)(1U << (ReadinessGroup)))) != 0)
    {
      lReturnValue = TRUE;
    }
  }
  return lReturnValue;
}
#endif
/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_ESM_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Esm_Implementation.h
 *********************************************************************************************************************/
