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
/*! \addtogroup Dem_Data
 *  \{
 *  \file       Dem_Data_Implementation.h
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

#if !defined (DEM_DATA_IMPLEMENTATION_H)
#define DEM_DATA_IMPLEMENTATION_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Data_Interface.h"

                                                    /* Used subcomponent API */
/* ------------------------------------------------------------------------- */
#include "Dem_UDSStatus_Interface.h"

                                                   /* Subcomponents callbacks*/
/* ------------------------------------------------------------------------- */
#include "Dem_Error_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/
/*! Preconfirmed DTC status after re-initialization */
#define DEM_PC_STATUS_INITIALIZE                 (0x00U)

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/
#define DEM_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define DEM_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define DEM_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define DEM_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define DEM_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define DEM_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define DEM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define DEM_START_SEC_CONST_8BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define DEM_STOP_SEC_CONST_8BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define DEM_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define DEM_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_Data_Properties
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_Data_Private
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_Data_Public
 * \{
 */

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)                                                                                            /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Data_PermanentEntryRemove
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Data_PermanentEntryRemove(
  CONST(uint8, AUTOMATIC)  PermanentIndex
  )
{
  /* Free the given permanent slot */
  Dem_Mem_PermanentSetEvent(PermanentIndex, DEM_EVENT_INVALID);

  --Dem_Mem_PermanentCurrentCount;

  Dem_Mem_PermanentSetStatus(PermanentIndex, DEM_ESM_PERMANENT_NONE);
  Dem_Nvm_SetBlockState(DEM_NVM_BLOCKTYPE_PERMANENT, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (DEM_FEATURE_NEED_PERMANENT == STD_ON) */

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)                                                                                            /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Data_PermanentEntryFind
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Data_PermanentEntryFind(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  uint8 lPermanentIndex;

  for (lPermanentIndex = 0; lPermanentIndex < Dem_Cfg_GlobalPermanentMaxCount(); ++lPermanentIndex)
  {
    if (Dem_Mem_PermanentGetEvent(lPermanentIndex) == EventId)
    {
      break;
    }
  }
  return lPermanentIndex;
}
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)                                                                                            /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Data_PermanentEntryFillUp
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
 */

DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Data_PermanentEntryFillUp(
  void
  )
{
  Dem_EventIdType lEventId;

  /* scan for events which are OBD relevant, MIL relevant and which command the MIL; try to enter them in permanent memory */
  for (lEventId = Dem_Cfg_GlobalPrimaryFirst();                                                                                  /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
       (lEventId <= Dem_Cfg_GlobalPrimaryLast()) && (Dem_Mem_PermanentCurrentCount < Dem_Cfg_GlobalPermanentMaxCount());
       ++lEventId)
  {
# if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
    /* Only process master events (subevents are handled with the master) */
    if (Dem_Cfg_GetMasterEvent(lEventId) != lEventId)
    {
      continue;                                                                                                                  /* PRQA S 0770 */ /* MD_DEM_14.5 */
    }
# endif

    if ( (Dem_Cfg_GlobalMilIndicator() == Dem_Cfg_EventSpecialIndicator(lEventId))                                               /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
      && (Dem_UDSStatus_Test_WIR(Dem_DTC_GetDTCStatus(lEventId)) == TRUE) )
    { /* OBD relevant Event found which is currently commanding the MIL */
      uint8 lPermanentIndex;

      lPermanentIndex = Dem_Data_PermanentEntryFind(lEventId);
      if (Dem_Cfg_GlobalPermanentMaxCount() == lPermanentIndex)
      {
        uint8 lPermanentState;
        uint8 lQualifyState;
# if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
        Dem_Cfg_CombinedGroupIndexType lGroupId;
# endif

        lPermanentIndex = Dem_Data_PermanentEntryFind(DEM_EVENT_INVALID);
        lPermanentState = DEM_ESM_PERMANENT_NONE;

# if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
        lGroupId = Dem_Cfg_EventCombinedGroup(lEventId);
        if (DEM_CFG_COMBINED_GROUP_INVALID != lGroupId)
        {
          Dem_Cfg_CombinedGroupIterType lCombinedIter;

          lQualifyState = 0x77U;

           /* Scan through all active sub-events */
          for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lCombinedIter);
               Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;
               Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))
          {
            Dem_EventIdType lSubEventId;

            lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);
            if ( (Dem_Cfg_EventAvailableByVariant(lSubEventId) == TRUE)                                                          /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
              && (Dem_EventInternalStatus_TestEventDisconnected(Dem_Event_GetInternalStatus(lSubEventId)) == FALSE) )
            {
              lQualifyState = (uint8)(Dem_Event_GetQualifyStatus(lSubEventId) | lQualifyState);
            }
          }
        }
        else
# endif
        {
          lQualifyState = Dem_Event_GetQualifyStatus(lEventId);
        }

        Dem_Mem_PermanentSetEvent(lPermanentIndex, lEventId);
        lPermanentState = DEM_ESM_SET_PERMANENT_STATE(lPermanentState, DEM_ESM_PERMANENT_ACTIVE);

        /* Only a qualified WIR is immediately visible */
        if (Dem_EventInternalStatus_Test_QualifyStatus_WIR(lQualifyState) == TRUE)
        {
          lPermanentState = DEM_ESM_SET_PERMANENT_VISIBLE(lPermanentState);
        }

        ++Dem_Mem_PermanentCurrentCount;

        Dem_Mem_PermanentSetStatus(lPermanentIndex, lPermanentState);
        Dem_Nvm_SetBlockState(DEM_NVM_BLOCKTYPE_PERMANENT, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
      }
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_OBDII == STD_ON) */

#if (DEM_FEATURE_NEED_OBD_ODOMETER == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Data_GetObdOdometer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_Data_GetObdOdometer(
  CONSTP2VAR(uint32, AUTOMATIC, DEM_VAR_NOINIT)  Odometer
  )
{
  Std_ReturnType lReturnValue;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if ( (Dem_Cfg_ObdDistanceInformation() >= Dem_Cfg_GlobalDataElementCount())                                                    /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
    || (Dem_Cfg_DataIsStoredInNV(Dem_Cfg_ObdDistanceInformation()) == FALSE)
    || (Dem_Cfg_DataCallbackType(Dem_Cfg_ObdDistanceInformation()) != DEM_CFG_DATA_FROM_CBK)
    || (Dem_Cfg_DataSize(Dem_Cfg_ObdDistanceInformation()) != 4) )
  {
    Dem_Error_RunTimeCheckFailed(__FILE__, __LINE__);
    *Odometer = 0;
    lReturnValue = E_NOT_OK;
  }
  else
# endif
  {
    uint8 lBuffer[4];
    lReturnValue = (*((Dem_ReadDataStandardFPtrType)Dem_Cfg_DataCbkRead(Dem_Cfg_ObdDistanceInformation())))(&lBuffer[0]);        /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_LOCAL_DESTINATIONBUFFER */
    if (lReturnValue == E_OK)
    {
      *Odometer = Dem_Make32Bit(lBuffer[0], lBuffer[1], lBuffer[2], lBuffer[3]);
    }
    else
    {
      *Odometer = 0;
    }
  }

  return lReturnValue;
}
#endif /* (DEM_FEATURE_NEED_OBD_ODOMETER == STD_ON) */

/* ****************************************************************************
 % Dem_Data_EntryInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_EntryInit(
  CONST(Dem_MemoryEntry_HandleType, AUTOMATIC)  MemoryEntryId
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(MemoryEntryId)                                                                                /* PRQA S 3112 */ /* MD_DEM_14.2 */

#if (DEM_FEATURE_NEED_OCC_COUNTER == STD_ON)
  Dem_MemoryEntry_ResetOccurrenceCounter(MemoryEntryId);
#endif

  Dem_MemoryEntry_SetAgingCounter(MemoryEntryId, DEM_OPCYCLE_CYCLECOUNT_INVALID);
#if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON)
  Dem_ExtendedEntry_ResetStoredExtendedRecords(MemoryEntryId);
#endif
#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
  Dem_SnapshotEntry_ResetStoredSnapshotRecords(MemoryEntryId);
#endif
#if (DEM_CFG_DATA_MAX_FDC_SINCE_LAST_CLEAR == STD_ON)
  Dem_MemoryEntry_SetDebounceValueMax(MemoryEntryId, 0);
#endif
#if (DEM_CFG_DATA_FAILED_CYCLES == STD_ON)
  Dem_MemoryEntry_SetFailedCycleCounter(MemoryEntryId, 0U);
#endif
#if (DEM_CFG_DATA_CONSECUTIVE_FAILED_CYCLES == STD_ON)
  Dem_MemoryEntry_SetConsecutiveFailedCycleCounter(MemoryEntryId, 0U);
#endif
#if (DEM_CFG_DATA_CYCLES_SINCE_FIRST_FAILED == STD_ON)
  Dem_MemoryEntry_SetCyclesSinceFirstFailed(MemoryEntryId, DEM_OPCYCLE_CYCLECOUNT_INVALID);
#endif
#if (DEM_CFG_DATA_CYCLES_SINCE_LAST_FAILED == STD_ON)
  Dem_MemoryEntry_SetCyclesSinceLastFailed(MemoryEntryId, DEM_OPCYCLE_CYCLECOUNT_INVALID);
#endif
#if (DEM_CFG_DATA_CYCLES_TESTED_SINCE_FIRST_FAILED == STD_ON)
  Dem_MemoryEntry_SetTestedCyclesSinceFirstFailed(MemoryEntryId, 0U);
#endif
#if (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON) || (DEM_FEATURE_NEED_OBD == STD_ON)                                          /* COV_MSR_UNSUPPORTED TF tf xf */
  Dem_MemoryEntry_SetState(MemoryEntryId, 0U);
#endif
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)                                                                                           /* COV_MSR_UNSUPPORTED XF */
  Dem_MemoryEntry_SetAgingTimer(MemoryEntryId, DEM_ESM_AGINGTIME_INVALID);
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON)
/* ****************************************************************************
 % Dem_Data_EntryUpdate_UserERecs
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
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Data_EntryUpdate_UserERecs(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_MemoryEntry_HandleType, AUTOMATIC)  MemoryEntryId
  )
{
  uint8 lBlockModification;
  Dem_ExtendedEntry_IterType lExtendedEntryIter;

  lBlockModification = DEM_DATA_NO_CHANGES;
  
  for (Dem_ExtendedEntry_IteratorInit(EventId, MemoryEntryId, &lExtendedEntryIter);
       Dem_ExtendedEntry_IteratorExists(&lExtendedEntryIter) == TRUE;
       Dem_ExtendedEntry_IteratorNext(&lExtendedEntryIter))
  {
    Dem_Cfg_ERecIndexType lCfgERecIndex;
    lCfgERecIndex = Dem_ExtendedEntry_IteratorGetExtendedRecordIndex(&lExtendedEntryIter);

    if (Dem_Cfg_ERecType(lCfgERecIndex) == DEM_CFG_EREC_TYPE_USER)
    {
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
      if (Dem_ExtendedEntry_IteratorGetExtendedEntryIndex(&lExtendedEntryIter) >= Dem_Cfg_GlobalERecMaxCount())
      {
        Dem_Error_RunTimeCheckFailed(__FILE__, __LINE__);
        break;
      }
# endif
      if ((Dem_Cfg_ERecUpdate(lCfgERecIndex) == TRUE)                                                                           /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
        || (Dem_ExtendedEntry_IteratorIsExtendedRecordStored(&lExtendedEntryIter) == FALSE))
      {
        Dem_SharedDataPtrType lDestinationBuffer;
        Dem_ExtendedEntry_IteratorUpdateExtendedEntry(&lExtendedEntryIter);
        lDestinationBuffer = Dem_ExtendedEntry_IteratorGetExtendedRecordDataPtr(&lExtendedEntryIter);

        Dem_DataStorageIF_CollectERec(EventId, Dem_Cfg_ERecDid(lCfgERecIndex), lDestinationBuffer, Dem_Cfg_GlobalERecMaxRawSize());

        lBlockModification = DEM_DATA_USERDATA_CHANGED;
      }
    }
  }

  return lBlockModification;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_Data_EntryUpdate_SRecs
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
Dem_Data_EntryUpdate_SRecs(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_MemoryEntry_HandleType, AUTOMATIC)  MemoryEntryId,
  CONST(uint8, AUTOMATIC)  UpdateFlags
  )
{
  uint8 lBlockModification;
# if (DEM_CFG_SUPPORT_SRECS_CONFIGURED == STD_ON)
  uint8 lLastStoredEntryIndex;
# endif
  Dem_SnapshotEntry_IterType lSnapshotEntryIter;

  lBlockModification = DEM_DATA_NO_CHANGES;
# if (DEM_CFG_SUPPORT_SRECS_CONFIGURED == STD_ON)
  lLastStoredEntryIndex = 0xffU; /* Invalid */
# endif

  for (Dem_SnapshotEntry_IteratorInit(EventId, MemoryEntryId, &lSnapshotEntryIter);
       Dem_SnapshotEntry_IteratorExists(&lSnapshotEntryIter) == TRUE;
       Dem_SnapshotEntry_IteratorNext(&lSnapshotEntryIter))
  {
    Dem_Cfg_SRecIndexType lCfgSRecIndex;

    lCfgSRecIndex = Dem_SnapshotEntry_IteratorGetSnapshotRecordIndex(&lSnapshotEntryIter);

    /* If an update is needed: Updates are required when the configured trigger matches,
     * and the record is either not stored or configured for updates */
    if ( (                                                                                                                       /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
# if (DEM_FEATURE_NEED_SREC_OEMTRIGGER == STD_ON)
           (Dem_Data_OemSRecTrigger(EventId, MemoryEntryId, lCfgSRecIndex, UpdateFlags)) ||
# endif
           (Dem_Cfg_SRecTrigger(lCfgSRecIndex) == (UpdateFlags & Dem_Cfg_SRecTrigger(lCfgSRecIndex))) ) &&
         ( (Dem_Cfg_SRecUpdate(lCfgSRecIndex) == TRUE) || (Dem_SnapshotEntry_IteratorIsSnapshotRecordStored(&lSnapshotEntryIter) == FALSE) )
       )
    {
      Dem_SnapshotEntry_IteratorUpdateSnapshotEntry(&lSnapshotEntryIter);

# if (DEM_CFG_SUPPORT_SRECS_CONFIGURED == STD_ON)
      /* For configured snapshot numbers, if the snapshot is already collected copy its data */
      if (lLastStoredEntryIndex != 0xffU)
      {
        Dem_SnapshotEntry_IteratorCopySnapshotEntry(&lSnapshotEntryIter, lLastStoredEntryIndex);
      }
      else
# endif
      {
# if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
        /* If 'prestore freezeframe' is enabled and data is prestored, copy the prestored data */
        if (Dem_DataStorageIF_PrestorageApplySRec(Dem_SnapshotEntry_IteratorGetSnapshotRecordDataPtr(&lSnapshotEntryIter),
                                         Dem_Cfg_GlobalSRecMaxRawSize()) == FALSE)
# endif
        {
          /* Otherwise, collect new samples from the application. */
          Dem_DataStorageIF_CollectSnapshot(EventId,                                                                             /* SBSW_DEM_POINTER_SNAPSHOT_BUFFER */
                                   Dem_SnapshotEntry_IteratorGetSnapshotRecordDataPtr(&lSnapshotEntryIter),
                                   Dem_Cfg_GlobalSRecMaxRawSize());
# if (DEM_CFG_SUPPORT_SRECS_CONFIGURED == STD_ON)
          lLastStoredEntryIndex = Dem_SnapshotEntry_IteratorGetSnapshotEntryIndex(&lSnapshotEntryIter);
# endif
        }
        lBlockModification = DEM_DATA_USERDATA_CHANGED;
      }
      /* For calculated snapshot records, processing is completed */
# if (DEM_CFG_SUPPORT_SRECS_CALCULATED == STD_ON)
      break;
# endif
    }
  }

  return lBlockModification;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)                                                                               /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Data_EntryUpdate_J1939FF
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Data_EntryUpdate_J1939FF(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_MemoryEntry_HandleType, AUTOMATIC)  MemoryEntryId
  )
{
# if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
  if (Dem_DataStorageIF_PrestorageApplyJ1939FF(Dem_MemoryEntry_GetJ1939FreezeFramePtr(MemoryEntryId, 0),
                                      (Dem_Cfg_GlobalJ1939FFRawSize() + Dem_Cfg_GlobalJ1939ExpFFRawSize())) == FALSE)
# endif
  {
    Dem_DataStorageIF_CollectJ1939FreezeFrame(EventId,
                                     Dem_MemoryEntry_GetJ1939FreezeFramePtr(MemoryEntryId, 0),
                                     (Dem_Cfg_GlobalJ1939FFRawSize() + Dem_Cfg_GlobalJ1939ExpFFRawSize()));
  }
  return DEM_DATA_USERDATA_CHANGED;
}
#endif /* (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) */

/* ****************************************************************************
 % Dem_Data_EntryUpdate
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
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Data_EntryUpdate(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,                                                                                    /* PRQA S 3206 */ /* MD_DEM_3206 */
  CONST(uint8, AUTOMATIC)  UpdateFlags,
  CONST(Dem_Cfg_MemoryIndexType, AUTOMATIC)  MemoryIndex
  )
{
  uint8 lBlockModification;

  lBlockModification = DEM_DATA_NO_CHANGES;

  if (MemoryIndex != DEM_MEM_INVALID_MEMORY_INDEX)
  {
    Dem_Mem_MemoryUpdateStart(MemoryIndex);

    /* On a TestFailed trigger */
    if ((UpdateFlags & DEM_CFG_TRIGGER_TESTFAILED) != 0)
    {
      {
# if (DEM_CFG_PROCESS_OCCTR_CONFIRMED == STD_ON)
        if (Dem_UDSStatus_Test_CDTC(Dem_DTC_GetDTCStatus(EventId)) == TRUE) /* Event had confirmed earlier */
# endif
        {
          if (Dem_MemoryEntry_IncrementOccurrenceCounter(Dem_MemStorageManager_GetMemoryEntryId(MemoryIndex)) == TRUE)
          {
            lBlockModification = DEM_DATA_SET_STATISTICS_CHANGED(lBlockModification);
          }
        }
      }
      {
        Dem_MemoryEntry_SetAgingCounter(Dem_MemStorageManager_GetMemoryEntryId(MemoryIndex), DEM_OPCYCLE_CYCLECOUNT_INVALID);
        lBlockModification = DEM_DATA_SET_STATISTICS_CHANGED(lBlockModification);
      }
#if (DEM_CFG_DATA_CYCLES_SINCE_FIRST_FAILED == STD_ON)
      {
        if (Dem_MemoryEntry_GetCyclesSinceFirstFailed(Dem_MemStorageManager_GetMemoryEntryId(MemoryIndex)) == DEM_OPCYCLE_CYCLECOUNT_INVALID)
        {
          Dem_MemoryEntry_SetCyclesSinceFirstFailed(Dem_MemStorageManager_GetMemoryEntryId(MemoryIndex), Dem_OperationCycle_GetCycleCounter(Dem_Cfg_EventOperationCycle(EventId)));
          lBlockModification = DEM_DATA_SET_STATISTICS_CHANGED(lBlockModification);
        }
      }
#endif
#if (DEM_CFG_DATA_MAX_FDC_SINCE_LAST_CLEAR == STD_ON)
      {
        Dem_MemoryEntry_SetDebounceValueMax(Dem_MemStorageManager_GetMemoryEntryId(MemoryIndex), (sint16)0x7F);
        lBlockModification = DEM_DATA_SET_STATISTICS_CHANGED(lBlockModification);
      }
#endif

#if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON)
      lBlockModification |= Dem_Data_EntryUpdate_UserERecs(EventId, Dem_MemStorageManager_GetMemoryEntryId(MemoryIndex));
#endif
    } /* TEST PASSED->FAILED */

    /* On a TestFailedThisCycle trigger */
    if ((UpdateFlags & DEM_CFG_TRIGGER_TESTFAILEDTHISCYCLE) != 0)
    {
      if (Dem_MemoryEntry_IncrementFailedCycleCounter(Dem_MemStorageManager_GetMemoryEntryId(MemoryIndex)) == TRUE)
      {
        lBlockModification = DEM_DATA_SET_STATISTICS_CHANGED(lBlockModification);
      }
      if (Dem_MemoryEntry_IncrementConsecutiveFailedCycleCounter(Dem_MemStorageManager_GetMemoryEntryId(MemoryIndex)) == TRUE)
      {
        lBlockModification = DEM_DATA_SET_STATISTICS_CHANGED(lBlockModification);
      }
# if (DEM_CFG_DATA_CYCLES_SINCE_LAST_FAILED == STD_ON)
      {
        Dem_MemoryEntry_SetCyclesSinceLastFailed(Dem_MemStorageManager_GetMemoryEntryId(MemoryIndex), Dem_OperationCycle_GetCycleCounter(Dem_Cfg_EventOperationCycle(EventId)));
        lBlockModification = DEM_DATA_SET_STATISTICS_CHANGED(lBlockModification);
      }
# endif

    }

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
# if (DEM_CFG_SUPPORT_SRECS_CALCULATED == STD_ON)
    if ((UpdateFlags & DEM_CFG_TRIGGER_TESTFAILED) != 0)
# endif
    {
      lBlockModification |= Dem_Data_EntryUpdate_SRecs(EventId, Dem_MemStorageManager_GetMemoryEntryId(MemoryIndex), UpdateFlags);
    }
#endif /* #if (DEM_CFG_SUPPORT_SRECS == STD_ON) */
#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)                                                                               /* COV_MSR_UNSUPPORTED XF */
    if ( (Dem_Cfg_EventJ1939Dtc(EventId) != DEM_CFG_DTC_J1939_INVALID)
      && ((UpdateFlags & DEM_CFG_TRIGGER_CONFIRMED) != 0) )
    {
      lBlockModification |= Dem_Data_EntryUpdate_J1939FF(EventId, Dem_MemStorageManager_GetMemoryEntryId(MemoryIndex));
    }
#endif /* (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) */

    Dem_Mem_MemoryUpdateFinish(MemoryIndex);
  } /* if (DEM_MEM_INVALID_MEMORY_INDEX != MemoryIndex) */

  /* In rare cases this function does nothing */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  return lBlockModification;
}                                                                                                                                /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_ERECS == STD_ON)
/* ****************************************************************************
% Dem_Data_ERecAllDataElementsStored
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_ERecAllDataElementsStored(
  CONST(Dem_Cfg_DidIndexType, AUTOMATIC)  DidIndex,
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId
  )
{
  boolean lReturnValue;

  lReturnValue = TRUE;
# if (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
  if (DEM_MEM_TEST_AGING_ONLY(Dem_MemoryEntry_GetState(Dem_DTCReadoutBuffer_GetMemoryEntryId(ReadoutBufferId))) == TRUE)
  {
    Dem_Cfg_DidDataIterType lDataIter;

    for (Dem_Cfg_DidDataIterInit(DidIndex, &lDataIter);                                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_DidDataIterExists(&lDataIter) == TRUE;                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_DidDataIterNext(&lDataIter))                                                                                       /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      uint8 lCallbackType;
      Dem_Cfg_DataIndexType lDataIndex;
      lDataIndex = Dem_Cfg_DidDataIterGet(&lDataIter);                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */

      lCallbackType = Dem_Cfg_DataCallbackType(lDataIndex);

      if ((lCallbackType == DEM_CFG_DATA_FROM_OCCCTR)
        || (lCallbackType == DEM_CFG_DATA_FROM_OCCCTR_2BYTE)
        || (lCallbackType == DEM_CFG_DATA_FROM_MAX_FDC_SINCE_LAST_CLEAR)
        || (lCallbackType == DEM_CFG_DATA_FROM_FAILED_CYCLES)
        || (lCallbackType == DEM_CFG_DATA_FROM_CONSECUTIVE_FAILED_CYCLES)
        || (lCallbackType == DEM_CFG_DATA_FROM_CYCLES_TESTED_SINCE_FIRST_FAILED)
        )
      {
        lReturnValue = FALSE;
        break;
      }
    }
  }
# else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DidIndex)                                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */   
# endif
  return lReturnValue;
}
#endif

#if (DEM_FEATURE_NEED_CYCLE_COUNTERS == STD_ON)
/* ****************************************************************************
 % Dem_Data_EntryOpCycleStart
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_EntryOpCycleStart(
  CONST(Dem_MemoryEntry_HandleType, AUTOMATIC)  MemoryEntryId,
  CONST(uint16, AUTOMATIC)  CycleCount
  )
{
  boolean lReturnValue;
  lReturnValue = FALSE;

# if (DEM_CFG_DATA_CYCLES_SINCE_FIRST_FAILED == STD_ON)
  if ((Dem_MemoryEntry_GetCyclesSinceFirstFailed(MemoryEntryId) < DEM_OPCYCLE_CYCLECOUNT_MAX)                                    /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
    && (Dem_OperationCycle_CycleCountDistance(Dem_MemoryEntry_GetCyclesSinceFirstFailed(MemoryEntryId), CycleCount) >= 0xffU))
  {
    Dem_MemoryEntry_SetCyclesSinceFirstFailed(MemoryEntryId, DEM_OPCYCLE_CYCLECOUNT_LATCHED);
    lReturnValue = TRUE;
  }
# endif
# if (DEM_CFG_DATA_CYCLES_SINCE_LAST_FAILED == STD_ON)
  if ((Dem_MemoryEntry_GetCyclesSinceLastFailed(MemoryEntryId) < DEM_OPCYCLE_CYCLECOUNT_MAX)                                     /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
    && (Dem_OperationCycle_CycleCountDistance(Dem_MemoryEntry_GetCyclesSinceLastFailed(MemoryEntryId), CycleCount) >= 0xffU))
  {
    Dem_MemoryEntry_SetCyclesSinceLastFailed(MemoryEntryId, DEM_OPCYCLE_CYCLECOUNT_LATCHED);
    lReturnValue = TRUE;
  }
# endif
  return lReturnValue;
}
#endif /* (DEM_FEATURE_NEED_ENTRY_CYCLE_COUNTERS == STD_ON) */

#if (DEM_CFG_DATA_CYCLES_TESTED_SINCE_FIRST_FAILED == STD_ON) || (DEM_CFG_DATA_CONSECUTIVE_FAILED_CYCLES == STD_ON)
/* ****************************************************************************
 % Dem_Data_DtcOpCycleUpdate
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_DtcOpCycleUpdate(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  CycleStatus
  )
{
  uint8 lDtcStatus;
  lDtcStatus = Dem_DTC_ApplyCombinedStatus(Dem_DTC_GetDTCStatus(EventId));

  if ( ((CycleStatus & DEM_OPERATIONCYCLE_CYCLE_STOPPED) != 0)                                                                   /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
    && (Dem_UDSStatus_Test_TNCTOC(lDtcStatus) == FALSE)
    && (Dem_DTC_TestStoredStatus(EventId) == TRUE) )
  {
    Dem_Mem_MemoryInfoPtrType lMemoryInfo;
    Dem_Cfg_MemoryIndexType lMemoryIndex;

    lMemoryInfo = Dem_MemStorageManager_MemoryInfoInit(Dem_Cfg_EventDestination(EventId));                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */
    lMemoryIndex = Dem_MemStorageManager_MemoryFindIndex_Checked(lMemoryInfo, EventId);                                          /* SBSW_DEM_CALL_MEMORYINFO_POINTER */

# if (DEM_CFG_DATA_CONSECUTIVE_FAILED_CYCLES == STD_ON)
    if (Dem_UDSStatus_Test_TFTOC(lDtcStatus) == FALSE)
    { /* reset the 'consecutive failed cycles' */
      Dem_MemoryEntry_SetConsecutiveFailedCycleCounter(Dem_MemStorageManager_GetMemoryEntryId(lMemoryIndex), 0);
    }
# endif

# if (DEM_CFG_DATA_CYCLES_TESTED_SINCE_FIRST_FAILED == STD_ON)
#  if (DEM_CFG_STORAGE_AT_FDC == STD_ON)
    if (Dem_UDSStatus_Test_TFSLC(lDtcStatus) == TRUE)
#  endif
    {
      Dem_MemoryEntry_HandleType lMemoryEntryId;
      lMemoryEntryId = Dem_MemStorageManager_GetMemoryEntryId(lMemoryIndex);
      if (Dem_MemoryEntry_GetTestedCyclesSinceFirstFailed(lMemoryEntryId) < 255)
      {
        Dem_MemoryEntry_SetTestedCyclesSinceFirstFailed(lMemoryEntryId, 
                                                        Dem_MemoryEntry_GetTestedCyclesSinceFirstFailed(lMemoryEntryId) + 1);
      }
    }
# endif
  }
}                                                                                                                                /* PRQA S 6080, 6050 */ /* MD_MSR_STMIF,  MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)                                                                                            /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Data_TestObdIIFreezeFrameUpdates
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
Dem_Data_TestObdIIFreezeFrameUpdates(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  Effects,
  CONST(uint8, AUTOMATIC)  FreezeFrameIndex
  )
{
  uint8 lUpdates;

  lUpdates = 0;
  if (Dem_Mem_FreezeFrameGetEvent(FreezeFrameIndex) == EventId)
  { /* 0-Trip events have both confirmed and pending triggers simultaneously, prioritize confirmed trigger */
    if ((Effects & DEM_CFG_TRIGGER_CONFIRMED) != 0)
    { /* If configured, confirmed trigger sets (to become visible in Mode02) or updates the timestamp */
# if ((DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON) || (DEM_CFG_SUPPORT_PENDING_UPDATE_FREEZE_FRAME == STD_ON) )
      lUpdates = (uint8)(lUpdates | DEM_DATA_FF_UPDATE_TIMESTAMP);
# endif
      /* If configured, confirmed trigger updates the data */
# if (DEM_CFG_SUPPORT_PENDING_UPDATE_FREEZE_FRAME == STD_ON)
      lUpdates = (uint8)(lUpdates | DEM_DATA_FF_UPDATE_DATA);
# endif
    }
  }
  else
  { /* 0-Trip events have both confirmed and pending triggers simultaneously, prioritize confirmed trigger */
    if ((Effects & DEM_CFG_TRIGGER_CONFIRMED) != 0)
    {
      lUpdates = (uint8)(lUpdates | DEM_DATA_FF_UPDATE_TIMESTAMP);
      lUpdates = (uint8)(lUpdates | DEM_DATA_FF_UPDATE_DATA);
    }
# if (DEM_CFG_SUPPORT_PENDING_STORAGE_FREEZE_FRAME == STD_ON)
    else /* ((Effects & DEM_CFG_TRIGGER_PENDING) != 0) */
    {
#  if (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_OFF)
      lUpdates = (uint8)(lUpdates | DEM_DATA_FF_UPDATE_TIMESTAMP);
#  endif
      lUpdates = (uint8)(lUpdates | DEM_DATA_FF_UPDATE_DATA);
    }
# endif
  }

  return lUpdates;
}
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_PENDING_DISPLACE_FREEZE_FRAME == STD_ON)                               /* COV_MSR_UNSUPPORTED XF xf xf */
/* ****************************************************************************
 % Dem_Data_SelectDisplacedFreezeFrameIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Data_SelectDisplacedFreezeFrameIndex(
  void
)
{
  uint8_least lSelectedIndex;
  uint8_least lFreezeFrameIndex;

  lFreezeFrameIndex = Dem_Cfg_GlobalObdIIFFCount();
  lSelectedIndex    = Dem_Cfg_GlobalObdIIFFCount();

  while (lFreezeFrameIndex != 0U)
  {
    Dem_EventIdType lEventId;

    lFreezeFrameIndex--;

    lEventId = Dem_Mem_FreezeFrameGetEvent(lFreezeFrameIndex);

    if (lEventId != DEM_EVENT_INVALID)
    {
      if (Dem_UDSStatus_Test_CDTC(Dem_DTC_GetDTCStatus(lEventId)) == FALSE)
      {
        if (lSelectedIndex == Dem_Cfg_GlobalObdIIFFCount())
        { /* store initial index */
          lSelectedIndex = lFreezeFrameIndex;
        }
        else
        {
          if (Dem_Mem_FreezeFrameGetTimestamp(lFreezeFrameIndex) < Dem_Mem_FreezeFrameGetTimestamp(lSelectedIndex))
          { /* slot is older, store reference */
            lSelectedIndex = lFreezeFrameIndex;
          }
        }
      }
    }
  }
  return (uint8)lSelectedIndex;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)                                                                                            /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Data_EntryStore_ObdIIFreezeFrame
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_EntryStore_ObdIIFreezeFrame(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  Effects
  )
{
  if ( ((Effects & DEM_CFG_TRIGGER_CONFIRMED) != 0)
# if (DEM_CFG_SUPPORT_PENDING_STORAGE_FREEZE_FRAME == STD_ON)
    || ((Effects & DEM_CFG_TRIGGER_PENDING) != 0)
# endif
    )
  {
    Dem_EventIdType lMasterEventId;
    uint8 lSelectedIndex;
    uint8 lFreezeFrameIndex;

    lMasterEventId = Dem_Cfg_GetMasterEvent(EventId);

    lFreezeFrameIndex = Dem_Cfg_GlobalObdIIFFCount();
    lSelectedIndex    = Dem_Cfg_GlobalObdIIFFCount();

    /* Find the stored freeze frame for the event (using the master event in case of event combination) */
    while (lFreezeFrameIndex != 0U)
    {
      Dem_EventIdType lEventId;

      lFreezeFrameIndex--;
      lEventId = Dem_Mem_FreezeFrameGetEvent(lFreezeFrameIndex);

      if (lEventId == lMasterEventId)
      {
        lSelectedIndex = (uint8)lFreezeFrameIndex;
        break;
      }
      if (lEventId == DEM_EVENT_INVALID)
      {
        lSelectedIndex = (uint8)lFreezeFrameIndex;
      }
    }

    /* If configured and no empty slot or stored freeze frame for the event was found, search for displacable freeze frame */
# if (DEM_CFG_SUPPORT_PENDING_DISPLACE_FREEZE_FRAME == STD_ON)
    if ( (lSelectedIndex == Dem_Cfg_GlobalObdIIFFCount())
      && ((Effects & DEM_CFG_TRIGGER_CONFIRMED) != 0) )
    {
      lSelectedIndex = Dem_Data_SelectDisplacedFreezeFrameIndex();
    }
# endif

    if (lSelectedIndex != Dem_Cfg_GlobalObdIIFFCount())
    {
      /* Test for required updates */
      uint8 lRequiredUpdates;

      lRequiredUpdates = Dem_Data_TestObdIIFreezeFrameUpdates(lMasterEventId, Effects, lSelectedIndex);

      if ((lRequiredUpdates & DEM_DATA_FF_UPDATE_DATA) != 0)
      {
        Dem_Mem_FreezeFrameSetEvent(lSelectedIndex, lMasterEventId);
        Dem_Mem_FreezeFrameSetTimestamp(lSelectedIndex, 0);

        /* Use the sub-event for prestored data in case of combination */
# if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
        if (Dem_DataStorageIF_PrestorageApplyObdIIFF(&Dem_Mem_FreezeFrameObdIIGetDataPtr(lSelectedIndex)[0],
                                            Dem_Cfg_GlobalObdIIFFRawSize()) == FALSE)
# endif
        {
          Dem_DataStorageIF_CollectObdIIFreezeFrame(EventId,
                                           &Dem_Mem_FreezeFrameObdIIGetDataPtr(lSelectedIndex)[0],
                                           Dem_Cfg_GlobalObdIIFFRawSize());
        }
      }

      if ((lRequiredUpdates & DEM_DATA_FF_UPDATE_TIMESTAMP) != 0)
      {
        Dem_Mem_FreezeFrameSetTimestamp(lSelectedIndex, Dem_Mem_CurrentTimestamp);
        ++Dem_Mem_CurrentTimestamp;
      }

      Dem_Mem_FreezeFrameIndexMode02 = Dem_Mem_FreezeFrameSelectOldest();

      if (lRequiredUpdates != 0)
      {
        Dem_Nvm_SetBlockState(DEM_NVM_BLOCKTYPE_OBDFREEZEFRAME, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
      }
    }
  }
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_OBDII == STD_ON) */

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)                                                                                            /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Data_PermanentEntryUpdate
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
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_PermanentEntryUpdate(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  OldDtcStatus,
  CONST(uint8, AUTOMATIC)  NewDtcStatus
  )
{
  uint8 lPermanentIndex;

  /* If the event has a permanent DTC */
  lPermanentIndex = Dem_Data_PermanentEntryFind(EventId);
  if (lPermanentIndex != Dem_Cfg_GlobalPermanentMaxCount())
  {
    boolean lExitPassive;
    uint8 lPermanentStateNew;
    uint8 lPermanentStateOld;

    lExitPassive = FALSE;
    lPermanentStateNew = Dem_Mem_PermanentGetStatus(lPermanentIndex);
    lPermanentStateOld = lPermanentStateNew;

    /* Mark the permanent entry as visible */
    lPermanentStateNew = DEM_ESM_SET_PERMANENT_VISIBLE(lPermanentStateNew);

    switch (DEM_ESM_GET_PERMANENT_STATE(lPermanentStateOld))
    {
    /* In state PERMANENT_ACTIVE: */
    case DEM_ESM_PERMANENT_ACTIVE:
      if (Dem_UDSStatus_Test_WIR(OldDtcStatus) == TRUE)
      {
        if (Dem_UDSStatus_Test_WIR(NewDtcStatus) == FALSE)
        { /* If the MIL became inactive, remove the permanent entry */
          Dem_Data_PermanentEntryRemove(lPermanentIndex);
        }
      }
      break;

    /* In state PERMANENT_PASSIVE: */
    case DEM_ESM_PERMANENT_PASSIVE:
# if (DEM_CFG_SUPPORT_PERMANENT_MANDATORY_PFC == STD_OFF)
      /* Unless MANDATORY_PFC is configured, leave the PASSIVE state if the Event requests the MIL */
      if (Dem_UDSStatus_Test_WIR(NewDtcStatus) == TRUE)
      {
        lExitPassive = TRUE;
      }
      else
# endif
      /* Otherwise, if the event was not tested failed this cycle */
      if (Dem_UDSStatus_Test_TFTOC(OldDtcStatus) == FALSE)
      {
        /* Test if the PCF conditions are met, if so set the PFC condition for the permanent entry */
        if (DEM_OPERATIONCYCLE_TEST_PFC_CYCLE(Dem_OperationCycle_GetObdCycleStates()) == TRUE)
        {
          lPermanentStateNew = DEM_ESM_SET_PERMANENT_CONDITION_PFC(lPermanentStateNew);
        }
        /* Test for a qualified passed test result, if so set the TESTED condition for the permanent entry */
        if (Dem_UDSStatus_Test_TNCTOC(OldDtcStatus) == FALSE)
        {
          lPermanentStateNew = DEM_ESM_SET_PERMANENT_CONDITION_TESTED(lPermanentStateNew);
        }
        /* If all conditions are met for the permanent entry, exit the passive state */
        if ( (DEM_ESM_TEST_PERMANENT_CONDITION_TESTED(lPermanentStateNew) == TRUE)
          && (DEM_ESM_TEST_PERMANENT_CONDITION_PFC(lPermanentStateNew) == TRUE)
# if (DEM_CFG_SUPPORT_PERMANENT_CONDITION_DCY == STD_ON)
          && (DEM_ESM_TEST_PERMANENT_CONDITION_DCY(lPermanentStateNew) == TRUE)
# endif
          )
        {
          lExitPassive = TRUE;
        }
# if (DEM_CFG_SUPPORT_PERMANENT_CONDITION_DCY == STD_ON)
        /* Otherwise, if DCY qualification is configured as permanent clear condition, set the DCY condition
         * for the permanent entry */
        else
        {
          lPermanentStateNew = DEM_ESM_SET_PERMANENT_CONDITION_DCY(lPermanentStateNew);
        }
# endif
      }
      else
      {
        /* If the event was tested failed, reset the PFC and TESTED conditions for the permanent entry */
        lPermanentStateNew = DEM_ESM_RESET_PERMANENT_CONDITION_TESTED(lPermanentStateNew);
        lPermanentStateNew = DEM_ESM_RESET_PERMANENT_CONDITION_PFC(lPermanentStateNew);
# if (DEM_CFG_SUPPORT_PERMANENT_CONDITION_DCY == STD_ON)
        /* If DCY qualification is configured as permanent clear condition, also set the DCY condition for the
         * permanent entry */
        lPermanentStateNew = DEM_ESM_SET_PERMANENT_CONDITION_DCY(lPermanentStateNew);
# endif
      }
      break;

    default:
      break;
    } /* end switch */

    /* On exit of state PASSIVE */
    if (lExitPassive == TRUE)
    {
      if (Dem_UDSStatus_Test_WIR(NewDtcStatus) == FALSE)
      { /* If the event no longer requests the MIL, remove the permanent entry. */
        Dem_Data_PermanentEntryRemove(lPermanentIndex);
      }
      else
      { /* Otherwise, go to ACTIVE state and reset the PFC and TESTED conditions for the permanent entry. */
        lPermanentStateNew = DEM_ESM_SET_PERMANENT_STATE(lPermanentStateNew, DEM_ESM_PERMANENT_ACTIVE);
        lPermanentStateNew = DEM_ESM_RESET_PERMANENT_CONDITION_TESTED(lPermanentStateNew);
        lPermanentStateNew = DEM_ESM_RESET_PERMANENT_CONDITION_PFC(lPermanentStateNew);
      }
    }

    /* If the permanent entry was changed, mark it for immediate NV synchronization */
    if (lPermanentStateNew != lPermanentStateOld)
    {
      Dem_Mem_PermanentSetStatus(lPermanentIndex, lPermanentStateNew);
      Dem_Nvm_SetBlockState(DEM_NVM_BLOCKTYPE_PERMANENT, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
  } /* (lPermanentIndex != Dem_Cfg_GlobalPermanentMaxCount()) */
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* (DEM_FEATURE_NEED_PERMANENT == STD_ON) */

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)                                                                                            /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Data_PermanentEntryClear
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_PermanentEntryClear(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  uint8 lPermanentIndex;

  lPermanentIndex = Dem_Data_PermanentEntryFind(EventId);
  if (lPermanentIndex != Dem_Cfg_GlobalPermanentMaxCount())
  {
    uint8 lPermanentState;

    lPermanentState = Dem_Mem_PermanentGetStatus(lPermanentIndex);

    /* Go to PASSIVE state and reset the conditions to remove the PDTC. Keep its visibility state. */
    lPermanentState = DEM_ESM_SET_PERMANENT_STATE(lPermanentState, DEM_ESM_PERMANENT_PASSIVE);
    lPermanentState = DEM_ESM_RESET_PERMANENT_CONDITION_TESTED(lPermanentState);
# if (DEM_CFG_SUPPORT_CLEAR_PDTC_NO_PFC == STD_ON)
    /* If event has a ratio attached, it can be cleared without respect the PFC cycle. So set PFC cycle to fulfilledPerm */
    if (Dem_Cfg_EventRatioIdIdx(EventId) != DEM_CFG_RATIOINDEX_INVALID)
    {
      lPermanentState = DEM_ESM_SET_PERMANENT_CONDITION_PFC(lPermanentState);
    }
    else
# endif
    {
      lPermanentState = DEM_ESM_RESET_PERMANENT_CONDITION_PFC(lPermanentState);
    }
# if (DEM_CFG_SUPPORT_PERMANENT_CONDITION_DCY == STD_ON)
    /* If DCY qualification is configured as permanent clear condition */
    if (DEM_OPERATIONCYCLE_TEST_DCY_QUALIFIED(Dem_OperationCycle_GetObdCycleStates()) == TRUE)
    { /* If the DCY is qualified, RESET the DCY condition for the permanent entry */
      lPermanentState = DEM_ESM_RESET_PERMANENT_CONDITION_DCY(lPermanentState);
    }
    else
    { /* Otherwise, set the DCY condition for the permanent entry */
      lPermanentState = DEM_ESM_SET_PERMANENT_CONDITION_DCY(lPermanentState);
    }
# endif

    /* If the permanent entry was changed, mark it for cleared and immediate NV synchronization */
    if (lPermanentState != Dem_Mem_PermanentGetStatus(lPermanentIndex))
    {
      Dem_Mem_PermanentSetStatus(lPermanentIndex, lPermanentState);
      Dem_Nvm_SetBlockState(DEM_NVM_BLOCKTYPE_PERMANENT, DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);
    }
  }
}
#endif /* (DEM_FEATURE_NEED_PERMANENT == STD_ON) */

#if ((DEM_FEATURE_NEED_PID41_CCONLY_CALCULATION == STD_ON) && (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_OFF))
/* ****************************************************************************
 % Dem_Data_TestReadinessGroupSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_TestReadinessGroupSupported(
  CONST(uint8, AUTOMATIC) ReadinessGroup,
  CONST(uint32, AUTOMATIC) PidSupportedMask
  )
{
  boolean lReturnValue;

  switch (ReadinessGroup)
  {
  case DEM_CFG_READINESS_MISF:
    lReturnValue = (boolean)(((PidSupportedMask & DEM_READINESS_SUPPORT_MISF_MASK) != 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_FLSYS:
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    lReturnValue = (boolean)(((PidSupportedMask & DEM_READINESS_SUPPORT_FLSYS_MASK) != 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    lReturnValue = (boolean)(((PidSupportedMask & DEM_READINESS_SUPPORT_CMPRCMPT) != 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_CAT:
    lReturnValue = (boolean)(((PidSupportedMask & DEM_READINESS_SUPPORT_CAT_MASK) != 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_HCCAT:
    lReturnValue = (boolean)(((PidSupportedMask & DEM_READINESS_SUPPORT_HCCAT_MASK) != 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_HTCAT:
    lReturnValue = (boolean)(((PidSupportedMask & DEM_READINESS_SUPPORT_HTCAT_MASK) != 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_NOXCAT:
    lReturnValue = (boolean)(((PidSupportedMask & DEM_READINESS_SUPPORT_NOXCAT_MASK) != 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_EVAP:
    lReturnValue = (boolean)(((PidSupportedMask & DEM_READINESS_SUPPORT_EVAP_MASK) != 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_SECAIR:
    lReturnValue = (boolean)(((PidSupportedMask & DEM_READINESS_SUPPORT_SECAIR_MASK) != 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_BOOSTPR:
    lReturnValue = (boolean)(((PidSupportedMask & DEM_READINESS_SUPPORT_BOOSTPR_MASK) != 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_O2SENS:
    lReturnValue = (boolean)(((PidSupportedMask & DEM_READINESS_SUPPORT_O2SENS_MASK) != 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_EGSENS:
    lReturnValue = (boolean)(((PidSupportedMask & DEM_READINESS_SUPPORT_EGSENS_MASK) != 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_O2SENSHT:
    lReturnValue = (boolean)(((PidSupportedMask & DEM_READINESS_SUPPORT_O2SENSHT_MASK) != 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_PMFLT:
    lReturnValue = (boolean)(((PidSupportedMask & DEM_READINESS_SUPPORT_PMFLT_MASK) != 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_EGR:
    lReturnValue = (boolean)(((PidSupportedMask & DEM_READINESS_SUPPORT_EGR_MASK) != 0) ? TRUE : FALSE);
    break;
  default:
  /* case DEM_CFG_READINESS_NONE: */
    lReturnValue = FALSE;
    break;
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif

#if (DEM_FEATURE_NEED_PID01_CALCULATION == STD_ON)
/* ****************************************************************************
 % Dem_Data_Calc_Pid01_MonitorCompleted
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
 */
DEM_LOCAL FUNC(void, DEM_CODE) Dem_Data_Calc_Pid01_MonitorCompleted(
  CONST(uint8, AUTOMATIC)  ReadinessGroup
  )
{
  /* If readiness group is valid */
  if ( (ReadinessGroup != DEM_CFG_READINESS_NONE) 
    && (ReadinessGroup != DEM_CFG_READINESS_FLSYS))
  {
    /* If readiness group is not completed */
    if (Dem_Data_TestReadinessGroupCompleted(ReadinessGroup, Dem_Mem_GetPid01CompletedMask()) == FALSE)
    {
      Dem_EventIdType lEventId;
      boolean lIsCompleted;
      
      lIsCompleted = TRUE;

      for (lEventId = Dem_Cfg_GlobalPrimaryFirst();
           lEventId <= Dem_Cfg_GlobalPrimaryLast();
           lEventId++)
      {
        uint8 lReadinessGroup;
        lReadinessGroup = Dem_Cfg_EventReadinessGroup(lEventId);
        /* If event is assigned to passed group */
        if (lReadinessGroup == ReadinessGroup)
        {
          /* If event is available */
          if (Dem_Event_TestEventSuppressed(lEventId) == FALSE)
          {
            uint8 lDtcStatus;
            uint8 lExtStatus;
            lDtcStatus = Dem_DTC_ApplyExternalOnlyStatus(lEventId, Dem_DTC_GetDTCStatus(lEventId));
            lExtStatus = Dem_Event_GetExtendedStatus(lEventId);
            if ((Dem_DTCInternalStatus_TestExtendedStatus_TPSLC(lExtStatus) == FALSE)                                           /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
              || (Dem_UDSStatus_Test_PDTC(lDtcStatus) == TRUE))
            {
              /* Leave readiness group not completed */
              lIsCompleted = FALSE;
              break;
            }
          }
        }
      }

      /* Otherwise set readiness group to completed */
      if (lIsCompleted == TRUE)
      {
        Dem_Util_SetReadinessGroupCompleted(TRUE, ReadinessGroup, &Dem_Mem_GetPid01CompletedMask());
      }
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if ((DEM_FEATURE_NEED_PID41_CALCULATION == STD_ON) || (DEM_FEATURE_NEED_PID41_CCONLY_CALCULATION == STD_ON) )
/* ****************************************************************************
 % Dem_Data_TestReadinessGroupDisabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_TestReadinessGroupDisabled(
  CONST(uint8, AUTOMATIC)  ReadinessGroup
  )
{
  uint8 lReadinessGroup;
  if (ReadinessGroup == DEM_CFG_READINESS_FLSYS)
  {
    lReadinessGroup = DEM_CFG_READINESS_FLSYS_NONCONT;
  }
  else
  {
    lReadinessGroup = ReadinessGroup;
  }
  return (boolean)(((Dem_Mem_GetPid41DisabledMask() & (1u << (lReadinessGroup))) != 0) ? TRUE : FALSE);
}
#endif

#if (DEM_FEATURE_NEED_PID41_CALCULATION == STD_ON)
/* ****************************************************************************
 % Dem_Data_GetPid41
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
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_GetPid41(
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  PID41value
  )
{
  uint32 lReadinessCompletedMask;
  uint32 lPid41;
  Dem_EventIdType lEventId;

  /* Set all groups in CompletedMask to not completed -> (completed = 0, not completed = 1) */
  lReadinessCompletedMask = 0xFFFFFFFFUL;

  /* Get supported and completed state from configuration */
  lPid41 = Dem_Cfg_GlobalPid01SupportedMask();

  /* Iterate over all events assigned to primary memory (OBD related events are assigned to primary memory */
  for (lEventId = Dem_Cfg_GlobalPrimaryFirst(); lEventId <= Dem_Cfg_GlobalPrimaryLast(); lEventId++)
  {
    uint8 lReadinessGroup = Dem_Cfg_EventReadinessGroup(lEventId);

    /* If a readiness group is assigned to event */
    if ((lReadinessGroup != DEM_CFG_READINESS_NONE)                                                                              /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
      /* If event is available */
      && (Dem_Event_TestEventSuppressed(lEventId) == FALSE)
      )
    {
      /* If Comprehensive Complete and Event availabilty is supported and readiness group is DEM_CFG_READINESS_CMPRCMPT */
# if (DEM_CFG_PROCESS_PID41_COMPREHENSIVE_COMPLETE == STD_ON)
      if (lReadinessGroup == DEM_CFG_READINESS_CMPRCMPT)
      {
        if (Dem_Data_TestReadinessGroupDisabled(lReadinessGroup) == TRUE)
        {
          /* Set readiness group to not supported */
          Dem_Util_SetReadinessGroupSupported(FALSE, lReadinessGroup, &lPid41);
        }
        else
        {
          /* Readiness group is set to supported (it will be set also to completed in last step) */
          Dem_Util_SetReadinessGroupSupported(TRUE, lReadinessGroup, &lPid41);
        }

        /* continue with next event */
        continue;                                                                                                                /* PRQA S 0770 */ /* MD_DEM_14.5 */
      }
# endif
      /* If readiness group is disabled */
      if (Dem_Data_TestReadinessGroupDisabled(lReadinessGroup) == TRUE)
      {
        /* Set readiness group to n/a (= completed)*/
        Dem_Util_SetReadinessGroupCompleted(TRUE, lReadinessGroup, &lPid41);

        /* Set readiness group to not supported */
        Dem_Util_SetReadinessGroupSupported(FALSE, lReadinessGroup, &lPid41);
      }
      else
      {
        /* If readiness group in CompletedMask is not completed */
        if (Dem_Data_TestReadinessGroupCompleted(lReadinessGroup, lReadinessCompletedMask) == FALSE)
        {
          uint8 lDtcStatus;
          lDtcStatus = Dem_DTC_GetDTCStatus(lEventId);

          /* If event is failed this operation cycle */
          if (Dem_UDSStatus_Test_TFTOC(lDtcStatus) == TRUE)
          {
            /* Set readiness group to completed  */
            Dem_Util_SetReadinessGroupCompleted(TRUE, lReadinessGroup, &lReadinessCompletedMask);
          }
          /* Otherwise if event is not tested this cycle */
          else if (Dem_UDSStatus_Test_TNCTOC(lDtcStatus) == TRUE)
          {
            /* Set readiness group to not completed  */
            Dem_Util_SetReadinessGroupCompleted(FALSE, lReadinessGroup, &lPid41);
          }
          else
          {
            /* do nothing */
          }
        }

        /* If Event Availability is supported */
# if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
        /* Set readiness group to supported*/
        Dem_Util_SetReadinessGroupSupported(TRUE, lReadinessGroup, &lPid41);
# endif
      }
    }
  }

  /* If readiness group in CompletedMask is completed set readiness group to completed -> (completed = 0, not completed = 1) */
  lPid41 &= lReadinessCompletedMask;

  /* Set reserved completed bits to 0 */
  if ((lPid41 & DEM_READINESS_SUPPORT_COMPRESSION) != 0)
  {
    lPid41 &= DEM_READINESS_INITIALIZE_PID01COMPLETED_COMPRESSION;
  }
  else
  {
    lPid41 &= DEM_READINESS_INITIALIZE_PID01COMPLETED_SPARK;
  }

  /* If Comprehensive Complete is supported set readiness group DEM_CFG_READINESS_CMPRCMPT to complete */
# if (DEM_CFG_PROCESS_PID41_COMPREHENSIVE_COMPLETE == STD_ON)
  Dem_Util_SetReadinessGroupCompleted(TRUE, DEM_CFG_READINESS_CMPRCMPT, &lPid41);
# endif

  PID41value[1] = Dem_GetHiLoByte(lPid41);
  PID41value[2] = Dem_GetLoHiByte(lPid41);
  PID41value[3] = Dem_GetLoLoByte(lPid41);
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_FEATURE_NEED_OBD == STD_ON) && (DEM_CFG_SUPPORT_PID41 == STD_ON) && (DEM_CFG_SUPPORT_MAJOR_MONITORS == STD_OFF)
/* ****************************************************************************
 % Dem_Data_GetPid41ComprehensiveOnly
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
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Data_GetPid41ComprehensiveOnly(
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  PID41value
  )
{
# if (DEM_CFG_PROCESS_PID41_COMPREHENSIVE_COMPLETE == STD_OFF)
  Dem_EventIdType lEventId;

  boolean lSupported;
  boolean lCompleted = TRUE;

  /* If readiness group is disabled */
  if (Dem_Data_TestReadinessGroupDisabled(DEM_CFG_READINESS_CMPRCMPT) == TRUE)
  {
    /* Set readiness group to not complete ( = not applicable) */
    lCompleted = FALSE;

    /* Set readiness group to not supported */
    lSupported = FALSE;
  }
  /* Otherwise */
  else
  {

#  if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
    /* Set readiness group DEM_CFG_READINESS_CMPRCMPT to not supported -> will be calculated later*/
    lSupported = FALSE;
#  else
    /* Otherwise get state for DEM_CFG_READINESS_CMPRCMPT from configuration, all others are not supported and complete */
    uint32 lPid41 = (Dem_Cfg_GlobalPid01SupportedMask() & DEM_READINESS_SUPPORT_CMPRCMPT);
    lSupported = Dem_Data_TestReadinessGroupSupported(DEM_CFG_READINESS_CMPRCMPT, lPid41);

    if (lSupported == TRUE)
#  endif
    /* If Event Availability is supported or readiness group DEM_CFG_READINESS_CMPRCMPT is supported */
    {
      /* Iterate over all events assigned to primary memory (OBD related events are assigned to primary memory */
      for (lEventId = Dem_Cfg_GlobalPrimaryFirst(); lEventId <= Dem_Cfg_GlobalPrimaryLast(); lEventId++)
      {
        /* If event readiness group is DEM_CFG_READINESS_CMPRCMPT and event is available */
        if ((Dem_Cfg_EventReadinessGroup(lEventId) == DEM_CFG_READINESS_CMPRCMPT)                                                /* PRQA S 3415 */ /* MD_DEM_12.4_cs */
          && (Dem_Event_TestEventSuppressed(lEventId) == FALSE)
          )
        {
          uint8 lDtcStatus;
          lDtcStatus = Dem_DTC_GetDTCStatus(lEventId);

          /* If Event Availability is supported */
#  if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
          /* Set readiness group to supported*/
          lSupported = TRUE;
#  endif /* If Event Availability is not supported supported state is in configuration */

          /* If event is failed this operation cycle */
          if (Dem_UDSStatus_Test_TFTOC(lDtcStatus) == TRUE)
          {
            /* Stop iterating over all events (group is still set to completed) */
            break;
          }
          /* Otherwise if event is not tested this cycle */
          else if (Dem_UDSStatus_Test_TNCTOC(lDtcStatus) == TRUE)
          {
            /* Set readiness group to not completed and stop iterating over all events */
            lCompleted = FALSE;
            break;                                                                                                               /* PRQA S 0771 */ /* MD_DEM_14.6_opt */
          }
          else
          {
            /* do nothing */
          }
        }
      }
    }
  }

  /* Return Pid41 value */
  PID41value[1] = 0x00;
  PID41value[2] = 0x00;
  PID41value[3] = 0x00;
  if (lSupported == TRUE)
  {
    PID41value[1] |= 0x04;
  }
  if (lCompleted == FALSE)
  {
    PID41value[1] |= 0x40;
  }
# else
  PID41value[1] = 0x04;
  PID41value[2] = 0x00; /* not supported */
  PID41value[3] = 0x00; /* not supported */
# endif
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if ( (DEM_FEATURE_NEED_PID01_CALCULATION == STD_ON) || (DEM_FEATURE_NEED_PID41_CALCULATION == STD_ON) )
/* ****************************************************************************
 % Dem_Data_TestReadinessGroupCompleted
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_TestReadinessGroupCompleted(
  CONST(uint8, AUTOMATIC)  ReadinessGroup,
  CONST(uint32, AUTOMATIC)  Pid01CompletedMask
  )
{
  boolean lReturnValue;

  switch (ReadinessGroup)
  {
  case DEM_CFG_READINESS_MISF:
    lReturnValue = (boolean)(((Pid01CompletedMask & DEM_READINESS_COMPLETE_MISF_MASK) == 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_FLSYS:
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    lReturnValue = (boolean)(((Pid01CompletedMask & DEM_READINESS_COMPLETE_FLSYS_MASK) == 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    lReturnValue = (boolean)(((Pid01CompletedMask & DEM_READINESS_COMPLETE_CMPRCMPT) == 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_CAT:
    lReturnValue = (boolean)(((Pid01CompletedMask & DEM_READINESS_COMPLETE_CAT_MASK) == 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_HCCAT:
    lReturnValue = (boolean)(((Pid01CompletedMask & DEM_READINESS_COMPLETE_HCCAT_MASK) == 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_HTCAT:
    lReturnValue = (boolean)(((Pid01CompletedMask & DEM_READINESS_COMPLETE_HTCAT_MASK) == 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_NOXCAT:
    lReturnValue = (boolean)(((Pid01CompletedMask & DEM_READINESS_COMPLETE_NOXCAT_MASK) == 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_EVAP:
    lReturnValue = (boolean)(((Pid01CompletedMask & DEM_READINESS_COMPLETE_EVAP_MASK) == 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_SECAIR:
    lReturnValue = (boolean)(((Pid01CompletedMask & DEM_READINESS_COMPLETE_SECAIR_MASK) == 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_BOOSTPR:
    lReturnValue = (boolean)(((Pid01CompletedMask & DEM_READINESS_COMPLETE_BOOSTPR_MASK) == 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_O2SENS:
    lReturnValue = (boolean)(((Pid01CompletedMask & DEM_READINESS_COMPLETE_O2SENS_MASK) == 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_EGSENS:
    lReturnValue = (boolean)(((Pid01CompletedMask & DEM_READINESS_COMPLETE_EGSENS_MASK) == 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_O2SENSHT:
    lReturnValue = (boolean)(((Pid01CompletedMask & DEM_READINESS_COMPLETE_O2SENSHT_MASK) == 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_PMFLT:
    lReturnValue = (boolean)(((Pid01CompletedMask & DEM_READINESS_COMPLETE_PMFLT_MASK) == 0) ? TRUE : FALSE);
    break;
  case DEM_CFG_READINESS_EGR:
    lReturnValue = (boolean)(((Pid01CompletedMask & DEM_READINESS_COMPLETE_EGR_MASK) == 0) ? TRUE : FALSE);
    break;
  default:
    /* case DEM_CFG_READINESS_NONE: */
    lReturnValue = FALSE;
    break;
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && \
( (DEM_CFG_DATA_CYCLES_TESTED_SINCE_LAST_FAILED == STD_ON) || (DEM_CFG_DATA_HEALINGCTR_DOWNCNT == STD_ON) )
/* ****************************************************************************
 % Dem_Data_TestHealingCounterReadable
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_TestHealingCounterReadable(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  boolean lCounterReadable;
  uint8 lDtcStatus;

  lCounterReadable = FALSE;
  lDtcStatus = Dem_DTC_GetDTCStatus(EventId);

  if (Dem_UDSStatus_Test_TFSLC(lDtcStatus) == TRUE)
  {
    if ((Dem_UDSStatus_Test_PDTC(lDtcStatus) == FALSE)                                                                     /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
# if (DEM_CFG_PROCESS_PDTC_STOREDONLY == STD_ON)
      && (Dem_DTC_TestStoredStatus(EventId) == TRUE)
# endif
      )
    {
      lCounterReadable = TRUE;
    }
  }

  return lCounterReadable;
}
#endif

                                                         /* Permanent Memory */
/* ------------------------------------------------------------------------- */

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)                                                                                            /* COV_MSR_UNSUPPORTED XF */
/* ****************************************************************************
 % Dem_Data_PermanentEntryAdd
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
Dem_Data_PermanentEntryAdd(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_EventIdType lMasterEventId;
  uint8   lPermanentIndex;

  lMasterEventId = Dem_Cfg_GetMasterEvent(EventId);

  /* scan through the permanent memory if the given event is available */
  lPermanentIndex = Dem_Data_PermanentEntryFind(lMasterEventId);

  if (Dem_Cfg_GlobalPermanentMaxCount() == lPermanentIndex)
  { /* Event not stored, try to find an empty slot */
    lPermanentIndex = Dem_Data_PermanentEntryFind(DEM_EVENT_INVALID);

    if (lPermanentIndex != Dem_Cfg_GlobalPermanentMaxCount())
    {
      uint8 lPermanentState;

      lPermanentState = DEM_ESM_PERMANENT_NONE;
      lPermanentState = DEM_ESM_SET_PERMANENT_STATE(lPermanentState, DEM_ESM_PERMANENT_ACTIVE);

      if (0 == Dem_Cfg_EventTripTarget(EventId))
      { /* 0-trip DTCs shall be visible immediately */
        lPermanentState = DEM_ESM_SET_PERMANENT_VISIBLE(lPermanentState);
      }

      Dem_Mem_PermanentSetEvent(lPermanentIndex, lMasterEventId);
      Dem_Mem_PermanentSetStatus(lPermanentIndex, lPermanentState);

      ++Dem_Mem_PermanentCurrentCount;

      /* persist status change in NVM */
      Dem_Nvm_SetBlockState(DEM_NVM_BLOCKTYPE_PERMANENT, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
# if (DEM_FEATURE_NEED_OVFLIND == STD_ON)
    else
    { /* Entry was not available and no free slot detected --> set overflow indication */
      Dem_Mem_SetOverflow(DEM_CFG_MEMORYID_PERMANENT);
    }
# endif
  }
}
#endif /* (DEM_FEATURE_NEED_PERMANENT == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_AGINGCTR == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyUpwardsAgingCounter
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
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyUpwardsAgingCounter(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  uint8 lAgingCounter;

  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

  if ( (Dem_MemoryEntry_GetAgingCounter(CopyDidInfoPtr->MemoryEntryId) == DEM_OPCYCLE_CYCLECOUNT_INVALID)                            /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
# if (DEM_CFG_AGING_COUNT_ONLY_AGEABLE == STD_ON)
    || (Dem_Cfg_EventSupportAging(CopyDidInfoPtr->EventId) == FALSE)
# endif
     )
  {
    lAgingCounter = 0x00U;
  }
  else
  if (Dem_MemoryEntry_GetAgingCounter(CopyDidInfoPtr->MemoryEntryId) == DEM_OPCYCLE_CYCLECOUNT_LATCHED)
  {
    lAgingCounter = 0xffU;
  }
# if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
  else
  if (Dem_MemoryEntry_GetAgingCounter(CopyDidInfoPtr->MemoryEntryId) == DEM_OPCYCLE_CYCLECOUNT_AGED)
  {
    lAgingCounter = (uint8)(Dem_Cfg_EventAgingTarget(CopyDidInfoPtr->EventId));
  }
# endif
  else
  {
    uint16 lTarget = Dem_MemoryEntry_GetAgingCounter(CopyDidInfoPtr->MemoryEntryId);
    uint16 lCurrent = Dem_OperationCycle_GetCycleCounter(Dem_Cfg_EventAgingCycle(CopyDidInfoPtr->EventId));
    lAgingCounter = (uint8)(Dem_Cfg_EventAgingTarget(CopyDidInfoPtr->EventId) - Dem_OperationCycle_CycleCountDistance(lCurrent, lTarget));
# if (DEM_CFG_AGING_REPORT_ZERO_AS_ONE == STD_ON)
    if (lAgingCounter == 0x00U)
    {
      lAgingCounter = 0x01U;
    }
# endif
  }

  if (CopyDidInfoPtr->BufferSize > CopyDidInfoPtr->WriteIndex)
  {
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex] = lAgingCounter;                                               /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->WriteIndex += 1;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  return TRUE;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_AGINGCTR == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_AGINGCTR_INVERTED == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyDownwardsAgingCounter
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyDownwardsAgingCounter(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  uint8 lAgingCounter;

  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

  if ((Dem_MemoryEntry_GetAgingCounter(CopyDidInfoPtr->MemoryEntryId) == DEM_OPCYCLE_CYCLECOUNT_INVALID)                         /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
# if (DEM_CFG_AGING_COUNT_ONLY_AGEABLE == STD_ON)
    || (Dem_Cfg_EventSupportAging(CopyDidInfoPtr->EventId) == FALSE)
# endif
     )
  {
    lAgingCounter = Dem_Cfg_EventAgingTarget(CopyDidInfoPtr->EventId);
  }
  else
  if ( (Dem_MemoryEntry_GetAgingCounter(CopyDidInfoPtr->MemoryEntryId) == DEM_OPCYCLE_CYCLECOUNT_LATCHED)                        /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
# if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
    || (Dem_MemoryEntry_GetAgingCounter(CopyDidInfoPtr->MemoryEntryId) == DEM_OPCYCLE_CYCLECOUNT_AGED)
# endif
    )
  {
    lAgingCounter = 0x00U;
  }
  else
  {
    uint16 lTarget = Dem_MemoryEntry_GetAgingCounter(CopyDidInfoPtr->MemoryEntryId);
    uint16 lCurrent = Dem_OperationCycle_GetCycleCounter(Dem_Cfg_EventAgingCycle(CopyDidInfoPtr->EventId));
    lAgingCounter = (uint8)(Dem_OperationCycle_CycleCountDistance(lCurrent, lTarget));
  }

  if (CopyDidInfoPtr->BufferSize > CopyDidInfoPtr->WriteIndex)
  {
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex] = lAgingCounter;                                               /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->WriteIndex += 1;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  return TRUE;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_AGINGCTR_INVERTED == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_OCCCTR == STD_ON)
/* ****************************************************************************
 % Dem_Data_Copy1ByteOccurrenceCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_Copy1ByteOccurrenceCounter(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  boolean lReturnValue;
  uint8 lOccurrenceCounter;

  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

# if (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
  if (DEM_MEM_TEST_AGING_ONLY(Dem_MemoryEntry_GetState(CopyDidInfoPtr->MemoryEntryId)) == TRUE)
  {
    lReturnValue = FALSE;
  }
  else
# endif
  {
# if (DEM_CFG_DATA_OCCCTR_2BYTE == STD_ON)
    {
      uint16 lOccurrenceCounter2Byte;
      lOccurrenceCounter2Byte = Dem_MemoryEntry_GetOccurrenceCounter2Byte(CopyDidInfoPtr->MemoryEntryId);
      if (lOccurrenceCounter2Byte > 0xffU)
      {
        lOccurrenceCounter = 0xffU;
      }
      else
      {
        lOccurrenceCounter = (uint8) lOccurrenceCounter2Byte;
      }
    }
# else
    lOccurrenceCounter = (uint8)Dem_MemoryEntry_GetOccurrenceCounter(CopyDidInfoPtr->MemoryEntryId);
#endif

    if (CopyDidInfoPtr->BufferSize > CopyDidInfoPtr->WriteIndex)
    {
      CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex] = lOccurrenceCounter;                                        /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
      CopyDidInfoPtr->WriteIndex += 1;                                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }

    lReturnValue = TRUE;
  }
  return lReturnValue;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_OCCCTR == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_OCCCTR_2BYTE == STD_ON)
/* ****************************************************************************
 % Dem_Data_Copy2ByteOccurrenceCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_Copy2ByteOccurrenceCounter(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  boolean lReturnValue;

  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

# if (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
  if (DEM_MEM_TEST_AGING_ONLY(Dem_MemoryEntry_GetState(CopyDidInfoPtr->MemoryEntryId)) == TRUE)
  {
    lReturnValue = FALSE;
  }
  else
# endif
  {
    if (CopyDidInfoPtr->BufferSize >= (CopyDidInfoPtr->WriteIndex + 2))
    {
      CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 0] =                                                        /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
        Dem_GetHiByte(Dem_MemoryEntry_GetOccurrenceCounter2Byte(CopyDidInfoPtr->MemoryEntryId));
      CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 1] =                                                        /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
        Dem_GetLoByte(Dem_MemoryEntry_GetOccurrenceCounter2Byte(CopyDidInfoPtr->MemoryEntryId));
      CopyDidInfoPtr->WriteIndex += 2;                                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    lReturnValue = TRUE;
  }
  return lReturnValue;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_OCCCTR_2BYTE == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_OVFLIND == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyOverflowIndication
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyOverflowIndication(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->EventId != DEM_EVENT_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

  if (CopyDidInfoPtr->BufferSize > CopyDidInfoPtr->WriteIndex)
  {
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex] = (uint8)Dem_Mem_GetOverflow(Dem_Cfg_EventDestination(CopyDidInfoPtr->EventId));  /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->WriteIndex += 1;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  return TRUE;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_OVFLIND == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_SIGNIFICANCE == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyEventSignificance
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyEventSignificance(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->EventId != DEM_EVENT_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

  if (CopyDidInfoPtr->BufferSize > CopyDidInfoPtr->WriteIndex)
  {
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex] = (uint8)(Dem_Cfg_EventSignificance(CopyDidInfoPtr->EventId) == DEM_CFG_EVENT_SIGNIFICANCE_FAULT);  /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->WriteIndex += 1;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  return TRUE;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_SIGNIFICANCE == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_PRIORITY == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyEventPriority
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyEventPriority(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->EventId != DEM_EVENT_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

  if (CopyDidInfoPtr->BufferSize > CopyDidInfoPtr->WriteIndex)
  {
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex] = (uint8)Dem_Cfg_EventPriority(CopyDidInfoPtr->EventId);       /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->WriteIndex += 1;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  return TRUE;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_PRIORITY == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_MAX_FDC_SINCE_LAST_CLEAR == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyMaxFdcSinceLastClear
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyMaxFdcSinceLastClear(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  boolean lReturnValue;
  sint8 lFdc;

  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)
  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->EventId != DEM_EVENT_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

# if (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
  if (DEM_MEM_TEST_AGING_ONLY(Dem_MemoryEntry_GetState(CopyDidInfoPtr->MemoryEntryId)) == TRUE)
  {
    lReturnValue = FALSE;
  }
  else
# endif
  {
    if (Dem_UDSStatus_Test_TFSLC(Dem_DTC_GetDTCStatus(CopyDidInfoPtr->EventId)) == TRUE)
    {
      lFdc = 127;
    }
    else
    {
      lFdc = (sint8)Dem_MemoryEntry_GetDebounceValueMax(CopyDidInfoPtr->MemoryEntryId);
    }
    if (CopyDidInfoPtr->BufferSize > CopyDidInfoPtr->WriteIndex)
    {
      CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex] = (uint8)lFdc;                                               /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
      CopyDidInfoPtr->WriteIndex += 1;                                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    lReturnValue = TRUE;
  }

  return lReturnValue;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_MAX_FDC_SINCE_LAST_CLEAR == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_MAX_FDC_DURING_CURRENT_CYCLE == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyMaxFdcThisCycle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyMaxFdcThisCycle(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  sint8 lFdc;

  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->EventId != DEM_EVENT_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

  {
# if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
    Dem_Cfg_CombinedGroupIndexType lGroupId;
    lGroupId = Dem_Cfg_EventCombinedGroup(CopyDidInfoPtr->EventId);
    if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
    {
      lFdc = Dem_DTC_CombinedGroupGetFDCMax(lGroupId);
    }
    else
# endif
    {
      lFdc = Dem_Satellite_GetMaxFaultDetectionCounter(
          Dem_Cfg_EventSatelliteId(CopyDidInfoPtr->EventId),
          Dem_Cfg_EventSatelliteEventId(CopyDidInfoPtr->EventId)
          );
    }
  }

  if (CopyDidInfoPtr->BufferSize > CopyDidInfoPtr->WriteIndex)
  {
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex] = (uint8)lFdc;                                                 /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->WriteIndex += 1;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }

  return TRUE;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_MAX_FDC_DURING_CURRENT_CYCLE == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_CYCLES_SINCE_LAST_FAILED == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyCyclesSinceLastFailed
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyCyclesSinceLastFailed(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  uint8 lCycleCounter;

  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)
  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->EventId != DEM_EVENT_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

  if (Dem_MemoryEntry_GetCyclesSinceLastFailed(CopyDidInfoPtr->MemoryEntryId) == DEM_OPCYCLE_CYCLECOUNT_INVALID)
  {
    lCycleCounter = 0x00U;
  }
  else
  if (Dem_MemoryEntry_GetCyclesSinceLastFailed(CopyDidInfoPtr->MemoryEntryId) == DEM_OPCYCLE_CYCLECOUNT_LATCHED)
  {
    lCycleCounter = 0xffU;
  }
  else
  {
    lCycleCounter = (uint8)Dem_OperationCycle_CycleCountDistance(
      Dem_MemoryEntry_GetCyclesSinceLastFailed(CopyDidInfoPtr->MemoryEntryId),
      Dem_OperationCycle_GetCycleCounter(Dem_Cfg_EventOperationCycle(CopyDidInfoPtr->EventId)));
  }

  if (CopyDidInfoPtr->BufferSize > CopyDidInfoPtr->WriteIndex)
  {
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex] = lCycleCounter;                                               /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->WriteIndex += 1;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  return TRUE;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_CYCLES_SINCE_LAST_FAILED == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_CYCLES_SINCE_FIRST_FAILED == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyCyclesSinceFirstFailed
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyCyclesSinceFirstFailed(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  uint8 lCycleCounter;

  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)
  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->EventId != DEM_EVENT_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

  if (Dem_MemoryEntry_GetCyclesSinceFirstFailed(CopyDidInfoPtr->MemoryEntryId) == DEM_OPCYCLE_CYCLECOUNT_INVALID)
  {
    lCycleCounter = 0x00U;
  }
  else
  if (Dem_MemoryEntry_GetCyclesSinceFirstFailed(CopyDidInfoPtr->MemoryEntryId) == DEM_OPCYCLE_CYCLECOUNT_LATCHED)
  {
    lCycleCounter = 0xffU;
  }
  else
  {
    lCycleCounter = (uint8)Dem_OperationCycle_CycleCountDistance(
      Dem_MemoryEntry_GetCyclesSinceFirstFailed(CopyDidInfoPtr->MemoryEntryId),
      Dem_OperationCycle_GetCycleCounter(Dem_Cfg_EventOperationCycle(CopyDidInfoPtr->EventId)));
  }

  if (CopyDidInfoPtr->BufferSize > CopyDidInfoPtr->WriteIndex)
  {
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex] = lCycleCounter;                                               /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->WriteIndex += 1;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  return TRUE;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_CYCLES_SINCE_FIRST_FAILED == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_FAILED_CYCLES == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyFailedCycles
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyFailedCycles(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  boolean lReturnValue;

  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

# if (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
  if (DEM_MEM_TEST_AGING_ONLY(Dem_MemoryEntry_GetState(CopyDidInfoPtr->MemoryEntryId)) == TRUE)
  {
    lReturnValue = FALSE;
  }
  else
# endif
  {
    if (CopyDidInfoPtr->BufferSize > CopyDidInfoPtr->WriteIndex)
    {
      CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex] =                                                            /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
        Dem_MemoryEntry_GetFailedCycleCounter(CopyDidInfoPtr->MemoryEntryId);
      CopyDidInfoPtr->WriteIndex += 1;                                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    lReturnValue = TRUE;
  }

  return lReturnValue;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_FAILED_CYCLES == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_CONSECUTIVE_FAILED_CYCLES == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyConsecutiveFailedCycles
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyConsecutiveFailedCycles(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  boolean lReturnValue;

  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

# if (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
  if (DEM_MEM_TEST_AGING_ONLY(Dem_MemoryEntry_GetState(CopyDidInfoPtr->MemoryEntryId)) == TRUE)
  {
    lReturnValue = FALSE;
  }
  else
# endif
  {
    if (CopyDidInfoPtr->BufferSize > CopyDidInfoPtr->WriteIndex)
    {
      CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex] =                                                            /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
        Dem_MemoryEntry_GetConsecutiveFailedCycleCounter(CopyDidInfoPtr->MemoryEntryId);
      CopyDidInfoPtr->WriteIndex += 1;                                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    lReturnValue = TRUE;
  }

  return lReturnValue;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_CONSECUTIVE_FAILED_CYCLES == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_CYCLES_TESTED_SINCE_FIRST_FAILED == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyCyclesTestedSinceFirstFailed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyCyclesTestedSinceFirstFailed(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  boolean lReturnValue;

  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->MemoryEntryId != DEM_MEMORYENTRY_HANDLE_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

# if (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
  if (DEM_MEM_TEST_AGING_ONLY(Dem_MemoryEntry_GetState(CopyDidInfoPtr->MemoryEntryId)) == TRUE)
  {
    lReturnValue = FALSE;
  }
  else
# endif
  {
    if (CopyDidInfoPtr->BufferSize > CopyDidInfoPtr->WriteIndex)
    {
      CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex] =                                                            /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
        Dem_MemoryEntry_GetTestedCyclesSinceFirstFailed(CopyDidInfoPtr->MemoryEntryId);
      CopyDidInfoPtr->WriteIndex += 1;                                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    lReturnValue = TRUE;
  }

  return lReturnValue;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_CYCLES_TESTED_SINCE_FIRST_FAILED == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_CYCLES_TESTED_SINCE_LAST_FAILED == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyCyclesTestedSinceLastFailed
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyCyclesTestedSinceLastFailed(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  uint8 lCyclesTestedSinceLastFailed;

  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->EventId != DEM_EVENT_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)
    
  if (Dem_Data_TestHealingCounterReadable(CopyDidInfoPtr->EventId) == TRUE)
  {
    lCyclesTestedSinceLastFailed = Dem_Event_GetTripCount(CopyDidInfoPtr->EventId);
  }
  else
  {
    lCyclesTestedSinceLastFailed = 0x00;
  }

  if (CopyDidInfoPtr->BufferSize > CopyDidInfoPtr->WriteIndex)
  {
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex] = lCyclesTestedSinceLastFailed;                                /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->WriteIndex += 1;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }

  return TRUE;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_CYCLES_TESTED_SINCE_LAST_FAILED == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_HEALINGCTR_DOWNCNT == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyHealingCounterDownwards
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
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyHealingCounterDownwards(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  uint8 lHealingCounterInverted;

  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->EventId != DEM_EVENT_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

  if (Dem_Data_TestHealingCounterReadable(CopyDidInfoPtr->EventId) == TRUE)
  {
    if (Dem_Event_GetTripCount(CopyDidInfoPtr->EventId) < Dem_Cfg_EventHealingTarget(CopyDidInfoPtr->EventId))
    {
      lHealingCounterInverted = Dem_Cfg_EventHealingTarget(CopyDidInfoPtr->EventId) - Dem_Event_GetTripCount(CopyDidInfoPtr->EventId);
    }
    else
    {
      lHealingCounterInverted = 0x00;
    }
  }
  else
  {
    lHealingCounterInverted = Dem_Cfg_EventHealingTarget(CopyDidInfoPtr->EventId);
  }

  if (CopyDidInfoPtr->BufferSize > CopyDidInfoPtr->WriteIndex)
  {
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex] = lHealingCounterInverted;                                     /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->WriteIndex += 1;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }

  return TRUE;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_HEALINGCTR_DOWNCNT == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_ROOTCAUSE_EVENTID == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyRootCauseEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyRootCauseEventId(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->SourceBuffer != NULL_PTR, DEM_E_INCONSISTENT_STATE, FALSE)

  if (CopyDidInfoPtr->BufferSize >= (CopyDidInfoPtr->WriteIndex + 2))
  {
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 0] = CopyDidInfoPtr->SourceBuffer[CopyDidInfoPtr->ReadIndex + 0];  /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 1] = CopyDidInfoPtr->SourceBuffer[CopyDidInfoPtr->ReadIndex + 1];  /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->ReadIndex += 2;                                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    CopyDidInfoPtr->WriteIndex += 2;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  return TRUE;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_ROOTCAUSE_EVENTID == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_OBDDTC == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyEventObdDtc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyEventObdDtc(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->EventId != DEM_EVENT_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

  if (CopyDidInfoPtr->BufferSize >= (CopyDidInfoPtr->WriteIndex + 2))
  {
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 0] = Dem_GetHiByte(Dem_Cfg_EventObdDtc(CopyDidInfoPtr->EventId));  /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 1] = Dem_GetLoByte(Dem_Cfg_EventObdDtc(CopyDidInfoPtr->EventId));  /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->WriteIndex += 2;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  return TRUE;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_OBDDTC == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_OBDDTC_3BYTE == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyEventObdDtc_3Byte
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyEventObdDtc_3Byte(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->EventId != DEM_EVENT_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

  if (CopyDidInfoPtr->BufferSize >= (CopyDidInfoPtr->WriteIndex + 3))
  {
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 0] = Dem_GetHiByte(Dem_Cfg_EventObdDtc(CopyDidInfoPtr->EventId));  /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 1] = Dem_GetLoByte(Dem_Cfg_EventObdDtc(CopyDidInfoPtr->EventId));  /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 2] = 0x00;                                                    /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
    CopyDidInfoPtr->WriteIndex += 3;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  return TRUE;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_OBDDTC == STD_ON) */

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_OBD_RATIO == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyEventObdRatio
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyEventObdRatio(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->EventId != DEM_EVENT_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

  if (CopyDidInfoPtr->BufferSize >= (CopyDidInfoPtr->WriteIndex + 4))
  {
# if (DEM_CFG_SUPPORT_IUMPR == STD_ON)
    Dem_RatioIdIndexType  lRatioIndex;
    lRatioIndex = Dem_Cfg_EventRatioIdIdx(CopyDidInfoPtr->EventId);

    if ( (lRatioIndex != DEM_CFG_RATIOINDEX_INVALID)                                                                             /* PRQA S 3415 */ /* MD_DEM_12.4_cf */
      && (Dem_Cfg_EventIsCombined(CopyDidInfoPtr->EventId) == FALSE)
       )
    {
      uint16 lNumerator;
      uint16 lDenominator;

      lDenominator = Dem_Mem_GetDenominator(lRatioIndex);
      lNumerator = Dem_Mem_GetNumerator(lRatioIndex);

      CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 0] = Dem_GetHiByte(lNumerator);
      CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 1] = Dem_GetLoByte(lNumerator);
      CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 2] = Dem_GetHiByte(lDenominator);
      CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 3] = Dem_GetLoByte(lDenominator);
    }
    else
# endif
    {
      CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 0] = 0x00;
      CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 1] = 0x00;
      CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 2] = 0x00;
      CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex + 3] = 0x00;
    }
    CopyDidInfoPtr->WriteIndex += 4;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  return TRUE;
}
#endif

#if (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_CURRENT_FDC == STD_ON)
/* ****************************************************************************
 % Dem_Data_CopyCurrentFdc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Data_CopyCurrentFdc(
  CONST(Dem_DataReportIF_CopyDidInfoPtrType, AUTOMATIC)  CopyDidInfoPtr
  )
{
  boolean lReturnValue;
  sint8 lFdc;

  Dem_Internal_AssertReturnValue(CopyDidInfoPtr->EventId != DEM_EVENT_INVALID, DEM_E_INCONSISTENT_STATE, FALSE)

  lReturnValue = (boolean)(E_OK == Dem_DTC_GetFaultDetectionCounter(CopyDidInfoPtr->EventId, &lFdc));                        /* SBSW_DEM_POINTER_FORWARD_STACK */
  if (lReturnValue == TRUE)
  {
    if (CopyDidInfoPtr->BufferSize > CopyDidInfoPtr->WriteIndex)
    {
      CopyDidInfoPtr->DestinationBuffer[CopyDidInfoPtr->WriteIndex] = (uint8)lFdc;                                               /* SBSW_DEM_WRITE_DIDINFO_DESTINATIONBUFFER */
      CopyDidInfoPtr->WriteIndex += 1;                                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }

  return lReturnValue;
}
#endif /* (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) && (DEM_CFG_DATA_CURRENT_FDC == STD_ON) */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_DATA_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Data_Implementation.h
 *********************************************************************************************************************/
