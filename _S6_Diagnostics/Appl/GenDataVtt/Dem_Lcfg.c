/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                 Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                 All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  LICENSE
 *  -------------------------------------------------------------------------------------------------------------------
 *            Module: Dem
 *           Program: 
 *          Customer: N/A
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: 
 *    License Scope : The usage is restricted to CBD0000000_D80
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: Dem_Lcfg.c
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/



/* configuration, interrupt handling implementations differ from the
 * source identification define used here. The naming
 * schemes for those files can be taken from this list:
 *
 * Dem.c:         DEM_SOURCE
 * Dem_Lcfg.c:    DEM_LCFG_SOURCE
 * Dem_PBcfg.c:   DEM_PBCFG_SOURCE */
#define DEM_LCFG_SOURCE


/**********************************************************************************************************************
  MISRA JUSTIFICATIONS
**********************************************************************************************************************/

/* PRQA S 0810 EOF */ /* MD_MSR_1.1_810 */                                      /* #include "..." causes nesting to exceed 8 levels - program is non-conforming. -- caused by #include'd files. */
/* PRQA S 0828 EOF */ /* MD_MSR_1.1_828 */                                      /* Maximum '#if...' nesting exceeds 8 levels - program is non-conforming -- caused by #include'd files. */
/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */                                      /* Number of macro definitions exceeds 1024 - program is non-conforming -- caused by #include'd files. */
/* PRQA S 0779 EOF */ /* MD_DEM_5.1 */                                          /* Identifier does not differ in 32 significant characters -- caused by Autosar algorithm for unique symbolic names. */
/* PRQA S 0612 EOF */ /* MD_DEM_1.1_612 */                                      /* The size of an object exceeds 32767 bytes - program is non-conforming -- caused by large user configration. */


/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/

#include "Dem.h"
#include "Os.h"
#if (DEM_CFG_USE_NVM == STD_ON)
# include "NvM.h"                                                               /* get: symbolic names for NvM block IDs */
#endif
#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
# include "J1939Nm.h"                                                           /* get: symbolic names for J1939Nm node IDs */
#endif
#if (DEM_CFG_USE_RTE == STD_ON)
/* DEM used with RTE */
# include "Rte_DemMaster_0.h"
#endif
#include "Dem_AdditionalIncludeCfg.h"                                           /* additional, configuration defined files */

/**********************************************************************************************************************
  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/



/**********************************************************************************************************************
  LOCAL DATA
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: LOCAL DATA
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: LOCAL DATA
**********************************************************************************************************************/



/**********************************************************************************************************************
  GLOBAL DATA
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL DATA
**********************************************************************************************************************/
/**********************************************************************************************************************
  Dem_Cfg_CycleIdTable
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_CycleIdTable
  \details
  Element         Description
  UsedForAging    DemOperationCycle is target of any DemEventParameter/DemEventClass/DemAgingCycleRef
  OpCycleType     DemOperationCycleType of the DemOperationCycle: [DEM_CFG_OPCYC_IGNITION, DEM_CFG_OPCYC_OBD_DCY, DEM_CFG_OPCYC_OTHER, DEM_CFG_OPCYC_POWER, DEM_CFG_OPCYC_TIME, DEM_CFG_OPCYC_WARMUP, DEM_CFG_OPCYC_IGNITION_HYBRID, DEM_CFG_OPCYC_AGING]
*/ 
#define DEM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(Dem_Cfg_CycleIdTableType, DEM_CONST) Dem_Cfg_CycleIdTable[3] = {
    /* Index    UsedForAging  OpCycleType                   Comment */
  { /*     0 */        FALSE, DEM_CFG_OPCYC_OTHER    },  /* [DemConf_DemOperationCycle_DemOperationCycle] */
  { /*     1 */         TRUE, DEM_CFG_OPCYC_IGNITION },  /* [DemConf_DemOperationCycle_IgnitionCycle] */
  { /*     2 */        FALSE, DEM_CFG_OPCYC_POWER    }   /* [DemConf_DemOperationCycle_PowerCycle] */
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_DataCollectionTable
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_DataCollectionTable
  \details
  Element                                Description
  CollectionSize                     
  DataElementTableCol2ElmtIndEndIdx      the end index of the 0:n relation pointing to Dem_Cfg_DataElementTableCol2ElmtInd
  DataElementTableCol2ElmtIndStartIdx    the start index of the 0:n relation pointing to Dem_Cfg_DataElementTableCol2ElmtInd
  MaskedBits                             contains bitcoded the boolean data of Dem_Cfg_DataElementTableCol2ElmtIndUsedOfDataCollectionTable, Dem_Cfg_UpdateOfDataCollectionTable
  StorageKind                        
*/ 
#define DEM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(Dem_Cfg_DataCollectionTableType, DEM_CONST) Dem_Cfg_DataCollectionTable[3] = {
    /* Index    CollectionSize  DataElementTableCol2ElmtIndEndIdx                                  DataElementTableCol2ElmtIndStartIdx                                  MaskedBits  StorageKind                     Referable Keys */
  { /*     0 */             0U, DEM_CFG_NO_DATAELEMENTTABLECOL2ELMTINDENDIDXOFDATACOLLECTIONTABLE, DEM_CFG_NO_DATAELEMENTTABLECOL2ELMTINDSTARTIDXOFDATACOLLECTIONTABLE,      0x00U, DEM_CFG_EREC_TYPE_GLOBAL },  /* [#NoDataCollectionConfigured] */
  { /*     1 */             4U,                                                                1U,                                                                  0U,      0x03U, DEM_CFG_EREC_TYPE_USER   },  /* [#EdrOccurrenceCounter, Ext:ExtendedDataClass_4f5344cd] */
  { /*     2 */             4U,                                                                2U,                                                                  1U,      0x03U, DEM_CFG_EREC_TYPE_USER   }   /* [#EdrOdometerValue, Ext:ExtendedDataClass_4f5344cd] */
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_DataElementTable
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_DataElementTable
  \details
  Element         Description
  ElementKind     DataElement kind, returned by Dem_Cfg_DataCallbackType()
  ElementSize     Size of data element in Byte.
  ReadDataFunc    C-function for getting the data. Its signature depends on ElementKind: With value(s) DEM_CFG_DATA_FROM_CBK_WITH_EVENTID use: Std_ReturnType (*)(uint8* Buffer, uint16 EventId); - and use: Std_ReturnType (*)(uint8* Buffer); with the other values DEM_CFG_DATA_FROM_CBK, DEM_CFG_DATA_FROM_SR_PORT_BOOLEAN, DEM_CFG_DATA_FROM_SR_PORT_SINT16, DEM_CFG_DATA_FROM_SR_PORT_SINT16_INTEL, DEM_CFG_DATA_FROM_SR_PORT_SINT32, DEM_CFG_DATA_FROM_SR_PORT_SINT32_INTEL, DEM_CFG_DATA_FROM_SR_PORT_SINT8, DEM_CFG_DATA_FROM_SR_PORT_SINT8_N, DEM_CFG_DATA_FROM_SR_PORT_UINT16, DEM_CFG_DATA_FROM_SR_PORT_UINT16_INTEL, DEM_CFG_DATA_FROM_SR_PORT_UINT32, DEM_CFG_DATA_FROM_SR_PORT_UINT32_INTEL, DEM_CFG_DATA_FROM_SR_PORT_UINT8, DEM_CFG_DATA_FROM_SR_PORT_UINT8_N.
*/ 
#define DEM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(Dem_Cfg_DataElementTableType, DEM_CONST) Dem_Cfg_DataElementTable[3] = {
    /* Index    ElementKind                  ElementSize  ReadDataFunc                                                                                                                       Referable Keys */
  { /*     0 */ DEM_CFG_DATAELEMENT_INVALID,          0U,  (Dem_ReadDataFPtrType) /* PRQA S 0313 */ /* MD_DEM_11.1 */ NULL_PTR                                                        },  /* [#NoDataElementConfigured] */
  { /*     1 */ DEM_CFG_DATA_FROM_CBK      ,          4U,  (Dem_ReadDataFPtrType) /* PRQA S 0313 */ /* MD_DEM_11.1 */ Rte_Call_CBReadData_OccurrenceCounter_OccurenceCounter_ReadData },  /* [#OccurrenceCounter_OccurenceCounter, EdrOccurrenceCounter] */
  { /*     2 */ DEM_CFG_DATA_FROM_CBK      ,          4U,  (Dem_ReadDataFPtrType) /* PRQA S 0313 */ /* MD_DEM_11.1 */ Rte_Call_CBReadData_OdometerValue_OdometerValue_ReadData        }   /* [#OdometerValue_OdometerValue, EdrOdometerValue] */
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_DebounceTable
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_DebounceTable
  \details
  Element                   Description
  DecrementStepSize         (-1) * DemDebounceCounterDecrementStepSize of the DemEventParameter/DemEventClass/DemDebounceAlgorithmClass/DemDebounceCounterBased
  FailedThreshold           DemDebounceCounterFailedThreshold of the DemEventParameter/DemEventClass/DemDebounceAlgorithmClass/DemDebounceCounterBased
  IncrementStepSize         DemDebounceCounterIncrementStepSize of the DemEventParameter/DemEventClass/DemDebounceAlgorithmClass/DemDebounceCounterBased
  PassedThreshold           DemDebounceCounterPassedThreshold of the DemEventParameter/DemEventClass/DemDebounceAlgorithmClass/DemDebounceCounterBased
  EventDebounceAlgorithm    Used DemEventParameter/DemEventClass/DemDebounceAlgorithmClass
  MaskedBits                contains bitcoded the boolean data of Dem_Cfg_DebounceContinuousOfDebounceTable, Dem_Cfg_EventDebounceBehaviorOfDebounceTable, Dem_Cfg_JumpDownOfDebounceTable, Dem_Cfg_JumpUpOfDebounceTable, Dem_Cfg_StorageOfDebounceTable
*/ 
#define DEM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(Dem_Cfg_DebounceTableType, DEM_CONST) Dem_Cfg_DebounceTable[3] = {
    /* Index    DecrementStepSize  FailedThreshold  IncrementStepSize  PassedThreshold  EventDebounceAlgorithm                                                      MaskedBits        Referable Keys */
  { /*     0 */                -1,             127,                 1,            -128, DEM_CFG_DEM_CFG_DEBOUNCETYPE_COUNTER_EVENTDEBOUNCEALGORITHMOFDEBOUNCETABLE,      0x02U },  /* [DTC_0x000002] */
  { /*     1 */                -1,             127,                 1,            -128, DEM_CFG_DEM_CFG_DEBOUNCETYPE_COUNTER_EVENTDEBOUNCEALGORITHMOFDEBOUNCETABLE,      0x06U },  /* [AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_ALL_RUN_REQUESTS_KILLED, AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_CONFIGURATION_DATA_INCONSISTENT, AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_IMPROPER_CALLER, AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_RAM_CHECK_FAILED, AutoCreatedDemEvent_NVM_E_INTEGRITY_FAILED, AutoCreatedDemEvent_NVM_E_LOSS_OF_REDUNDANCY, AutoCreatedDemEvent_NVM_E_QUEUE_OVERFLOW, AutoCreatedDemEvent_NVM_E_REQ_FAILED, AutoCreatedDemEvent_NVM_E_VERIFY_FAILED, AutoCreatedDemEvent_NVM_E_WRITE_PROTECTED, AutoCreatedDemEvent_NVM_E_WRONG_BLOCK_ID] */
  { /*     2 */                 0,               0,                 0,               0, DEM_CFG_DEM_CFG_DEBOUNCETYPE_INVALID_EVENTDEBOUNCEALGORITHMOFDEBOUNCETABLE,      0x00U }   /* [#EVENT_INVALID] */
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_DtcTable
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_DtcTable
  \details
  Element           Description
  UdsDtc        
  FunctionalUnit
*/ 
#define DEM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(Dem_Cfg_DtcTableType, DEM_CONST) Dem_Cfg_DtcTable[2] = {
    /* Index    UdsDtc        FunctionalUnit        Referable Keys */
  { /*     0 */ 0x00FFFFFFUL,           255U },  /* [#NoUdsDtcConfigured, #NoObdDtcConfigured, #NoJ1939DtcConfigured] */
  { /*     1 */ 0x00000002UL,             0U }   /* [DTCClass_DTC_0x000002] */
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_EventTable
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_EventTable
  \details
  Element             Description
  AgingCycleId        DemOperationCycle (ID) referenced by DemEventParameter/DemEventClass/DemAgingCycleRef
  DebounceTableIdx    the index of the 1:1 relation pointing to Dem_Cfg_DebounceTable
  EventKind           DemEventKind of the DemEventParameter
  OperationCycleId    DemOperationCycle (ID) referenced by DemEventParameter/DemEventClass/DemOperationCycleRef
*/ 
#define DEM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(Dem_Cfg_EventTableType, DEM_CONST) Dem_Cfg_EventTable[13] = {
    /* Index    AgingCycleId                             DebounceTableIdx  EventKind                                         OperationCycleId                                   Referable Keys */
  { /*     0 */ /*no AgingCycle*/ 3U                   ,               1U, DEM_CFG_DEM_EVENT_KIND_BSW_EVENTKINDOFEVENTTABLE, DemConf_DemOperationCycle_DemOperationCycle },  /* [#EVENT_INVALID, Satellite#0] */
  { /*     1 */ /*no AgingCycle*/ 3U                   ,               1U, DEM_CFG_DEM_EVENT_KIND_BSW_EVENTKINDOFEVENTTABLE, DemConf_DemOperationCycle_DemOperationCycle },  /* [AutoCreatedDemEvent_NVM_E_INTEGRITY_FAILED, Satellite#0] */
  { /*     2 */ /*no AgingCycle*/ 3U                   ,               1U, DEM_CFG_DEM_EVENT_KIND_BSW_EVENTKINDOFEVENTTABLE, DemConf_DemOperationCycle_DemOperationCycle },  /* [AutoCreatedDemEvent_NVM_E_LOSS_OF_REDUNDANCY, Satellite#0] */
  { /*     3 */ /*no AgingCycle*/ 3U                   ,               1U, DEM_CFG_DEM_EVENT_KIND_BSW_EVENTKINDOFEVENTTABLE, DemConf_DemOperationCycle_DemOperationCycle },  /* [AutoCreatedDemEvent_NVM_E_QUEUE_OVERFLOW, Satellite#0] */
  { /*     4 */ /*no AgingCycle*/ 3U                   ,               1U, DEM_CFG_DEM_EVENT_KIND_BSW_EVENTKINDOFEVENTTABLE, DemConf_DemOperationCycle_DemOperationCycle },  /* [AutoCreatedDemEvent_NVM_E_REQ_FAILED, Satellite#0] */
  { /*     5 */ /*no AgingCycle*/ 3U                   ,               1U, DEM_CFG_DEM_EVENT_KIND_BSW_EVENTKINDOFEVENTTABLE, DemConf_DemOperationCycle_DemOperationCycle },  /* [AutoCreatedDemEvent_NVM_E_VERIFY_FAILED, Satellite#0] */
  { /*     6 */ /*no AgingCycle*/ 3U                   ,               1U, DEM_CFG_DEM_EVENT_KIND_BSW_EVENTKINDOFEVENTTABLE, DemConf_DemOperationCycle_DemOperationCycle },  /* [AutoCreatedDemEvent_NVM_E_WRITE_PROTECTED, Satellite#0] */
  { /*     7 */ /*no AgingCycle*/ 3U                   ,               1U, DEM_CFG_DEM_EVENT_KIND_BSW_EVENTKINDOFEVENTTABLE, DemConf_DemOperationCycle_DemOperationCycle },  /* [AutoCreatedDemEvent_NVM_E_WRONG_BLOCK_ID, Satellite#0] */
  { /*     8 */ /*no AgingCycle*/ 3U                   ,               1U, DEM_CFG_DEM_EVENT_KIND_BSW_EVENTKINDOFEVENTTABLE, DemConf_DemOperationCycle_DemOperationCycle },  /* [AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_ALL_RUN_REQUESTS_KILLED, Satellite#0] */
  { /*     9 */ /*no AgingCycle*/ 3U                   ,               1U, DEM_CFG_DEM_EVENT_KIND_BSW_EVENTKINDOFEVENTTABLE, DemConf_DemOperationCycle_DemOperationCycle },  /* [AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_CONFIGURATION_DATA_INCONSISTENT, Satellite#0] */
  { /*    10 */ /*no AgingCycle*/ 3U                   ,               1U, DEM_CFG_DEM_EVENT_KIND_BSW_EVENTKINDOFEVENTTABLE, DemConf_DemOperationCycle_DemOperationCycle },  /* [AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_IMPROPER_CALLER, Satellite#0] */
  { /*    11 */ /*no AgingCycle*/ 3U                   ,               1U, DEM_CFG_DEM_EVENT_KIND_BSW_EVENTKINDOFEVENTTABLE, DemConf_DemOperationCycle_DemOperationCycle },  /* [AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_RAM_CHECK_FAILED, Satellite#0] */
  { /*    12 */ DemConf_DemOperationCycle_IgnitionCycle,               0U, DEM_CFG_DEM_EVENT_KIND_SWC_EVENTKINDOFEVENTTABLE, DemConf_DemOperationCycle_IgnitionCycle     }   /* [DTC_0x000002, Satellite#0] */
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_ExtendedDataTable
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_ExtendedDataTable
  \details
  Element                                   Description
  DataCollectionTableEdr2CollIndEndIdx      the end index of the 0:n relation pointing to Dem_Cfg_DataCollectionTableEdr2CollInd
  DataCollectionTableEdr2CollIndStartIdx    the start index of the 0:n relation pointing to Dem_Cfg_DataCollectionTableEdr2CollInd
  MaxRecordSize                         
*/ 
#define DEM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(Dem_Cfg_ExtendedDataTableType, DEM_CONST) Dem_Cfg_ExtendedDataTable[2] = {
    /* Index    DataCollectionTableEdr2CollIndEndIdx                                DataCollectionTableEdr2CollIndStartIdx                                MaxRecordSize        Referable Keys */
  { /*     0 */ DEM_CFG_NO_DATACOLLECTIONTABLEEDR2COLLINDENDIDXOFEXTENDEDDATATABLE, DEM_CFG_NO_DATACOLLECTIONTABLEEDR2COLLINDSTARTIDXOFEXTENDEDDATATABLE,            0U },  /* [#NoExtendedDataRecordConfigured, #EVENT_INVALID, AutoCreatedDemEvent_NVM_E_INTEGRITY_FAILED, AutoCreatedDemEvent_NVM_E_LOSS_OF_REDUNDANCY, AutoCreatedDemEvent_NVM_E_QUEUE_OVERFLOW, AutoCreatedDemEvent_NVM_E_REQ_FAILED, AutoCreatedDemEvent_NVM_E_VERIFY_FAILED, AutoCreatedDemEvent_NVM_E_WRITE_PROTECTED, AutoCreatedDemEvent_NVM_E_WRONG_BLOCK_ID, AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_ALL_RUN_REQUESTS_KILLED, AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_CONFIGURATION_DATA_INCONSISTENT, AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_IMPROPER_CALLER, AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_RAM_CHECK_FAILED] */
  { /*     1 */                                                                 2U,                                                                   0U,            4U }   /* [#ExtendedDataClass_4f5344cd, DTC_0x000002] */
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_MemoryBlockId
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_MemoryBlockId
  \brief  The array contains these items: Admin, Status, 8 * Primary
*/ 
#define DEM_START_SEC_CONST_16BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(Dem_Cfg_MemoryBlockIdType, DEM_CONST) Dem_Cfg_MemoryBlockId[10] = {
  /* Index     MemoryBlockId                                                          Referable Keys */
  /*     0 */ NvMConf_NvMBlockDescriptor_DemAdminDataBlock /*NvMBlockId=2*/     ,  /* [Dem_AdminData] */
  /*     1 */ NvMConf_NvMBlockDescriptor_DemStatusDataBlock /*NvMBlockId=5*/    ,  /* [Dem_StatusData] */
  /*     2 */ NvMConf_NvMBlockDescriptor_DemPrimaryDataBlock0 /*NvMBlockId=6*/  ,  /* [Dem_PrimaryEntry0] */
  /*     3 */ NvMConf_NvMBlockDescriptor_DemPrimaryDataBlock1 /*NvMBlockId=7*/  ,  /* [Dem_PrimaryEntry1] */
  /*     4 */ NvMConf_NvMBlockDescriptor_DemPrimaryDataBlock2 /*NvMBlockId=8*/  ,  /* [Dem_PrimaryEntry2] */
  /*     5 */ NvMConf_NvMBlockDescriptor_DemPrimaryDataBlock3 /*NvMBlockId=9*/  ,  /* [Dem_PrimaryEntry3] */
  /*     6 */ NvMConf_NvMBlockDescriptor_DemPrimaryDataBlock4 /*NvMBlockId=10*/ ,  /* [Dem_PrimaryEntry4] */
  /*     7 */ NvMConf_NvMBlockDescriptor_DemPrimaryDataBlock5 /*NvMBlockId=11*/ ,  /* [Dem_PrimaryEntry5] */
  /*     8 */ NvMConf_NvMBlockDescriptor_DemPrimaryDataBlock6 /*NvMBlockId=12*/ ,  /* [Dem_PrimaryEntry6] */
  /*     9 */ NvMConf_NvMBlockDescriptor_DemPrimaryDataBlock7 /*NvMBlockId=13*/    /* [Dem_PrimaryEntry7] */
};
#define DEM_STOP_SEC_CONST_16BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_MemoryDataPtr
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_MemoryDataPtr
  \brief  The array contains these items: Admin, Status, 8 * Primary
*/ 
#define DEM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(Dem_NvDataPtrType, DEM_CONST) Dem_Cfg_MemoryDataPtr[10] = {
  /* Index     MemoryDataPtr                                                                                  Referable Keys */
  /*     0 */ (Dem_NvDataPtrType) &Dem_Cfg_GetAdminData()        /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_AdminData] */
  /*     1 */ (Dem_NvDataPtrType) &Dem_Cfg_GetStatusData()       /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_StatusData] */
  /*     2 */ (Dem_NvDataPtrType) &Dem_Cfg_GetPrimaryEntry_0()   /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_PrimaryEntry0] */
  /*     3 */ (Dem_NvDataPtrType) &Dem_Cfg_GetPrimaryEntry_1()   /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_PrimaryEntry1] */
  /*     4 */ (Dem_NvDataPtrType) &Dem_Cfg_GetPrimaryEntry_2()   /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_PrimaryEntry2] */
  /*     5 */ (Dem_NvDataPtrType) &Dem_Cfg_GetPrimaryEntry_3()   /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_PrimaryEntry3] */
  /*     6 */ (Dem_NvDataPtrType) &Dem_Cfg_GetPrimaryEntry_4()   /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_PrimaryEntry4] */
  /*     7 */ (Dem_NvDataPtrType) &Dem_Cfg_GetPrimaryEntry_5()   /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_PrimaryEntry5] */
  /*     8 */ (Dem_NvDataPtrType) &Dem_Cfg_GetPrimaryEntry_6()   /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_PrimaryEntry6] */
  /*     9 */ (Dem_NvDataPtrType) &Dem_Cfg_GetPrimaryEntry_7()   /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */   /* [Dem_PrimaryEntry7] */
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_MemoryDataSize
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_MemoryDataSize
  \brief  The array contains these items: Admin, Status, 8 * Primary
*/ 
#define DEM_START_SEC_CONST_16BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(Dem_Cfg_MemoryDataSizeType, DEM_CONST) Dem_Cfg_MemoryDataSize[10] = {
  /* Index     MemoryDataSize                                                        Referable Keys */
  /*     0 */ (Dem_Cfg_MemoryDataSizeType) sizeof(Dem_Cfg_GetAdminData())      ,  /* [Dem_AdminData] */
  /*     1 */ (Dem_Cfg_MemoryDataSizeType) sizeof(Dem_Cfg_GetStatusData())     ,  /* [Dem_StatusData] */
  /*     2 */ (Dem_Cfg_MemoryDataSizeType) sizeof(Dem_Cfg_GetPrimaryEntry_0()) ,  /* [Dem_PrimaryEntry0] */
  /*     3 */ (Dem_Cfg_MemoryDataSizeType) sizeof(Dem_Cfg_GetPrimaryEntry_1()) ,  /* [Dem_PrimaryEntry1] */
  /*     4 */ (Dem_Cfg_MemoryDataSizeType) sizeof(Dem_Cfg_GetPrimaryEntry_2()) ,  /* [Dem_PrimaryEntry2] */
  /*     5 */ (Dem_Cfg_MemoryDataSizeType) sizeof(Dem_Cfg_GetPrimaryEntry_3()) ,  /* [Dem_PrimaryEntry3] */
  /*     6 */ (Dem_Cfg_MemoryDataSizeType) sizeof(Dem_Cfg_GetPrimaryEntry_4()) ,  /* [Dem_PrimaryEntry4] */
  /*     7 */ (Dem_Cfg_MemoryDataSizeType) sizeof(Dem_Cfg_GetPrimaryEntry_5()) ,  /* [Dem_PrimaryEntry5] */
  /*     8 */ (Dem_Cfg_MemoryDataSizeType) sizeof(Dem_Cfg_GetPrimaryEntry_6()) ,  /* [Dem_PrimaryEntry6] */
  /*     9 */ (Dem_Cfg_MemoryDataSizeType) sizeof(Dem_Cfg_GetPrimaryEntry_7())    /* [Dem_PrimaryEntry7] */
};
#define DEM_STOP_SEC_CONST_16BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_MemoryEntry
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_MemoryEntry
  \brief  The array contains these items: 8 * Primary, ReadoutBuffer; size = DEM_CFG_GLOBAL_PRIMARY_SIZE + DEM_CFG_GLOBAL_SECONDARY_SIZE + DEM_CFG_NUMBER_OF_READOUTBUFFERS
*/ 
#define DEM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(Dem_SharedDataPtrType, DEM_CONST) Dem_Cfg_MemoryEntry[9] = {
  /* Index     MemoryEntry                                                                                             Referable Keys */
  /*     0 */ (Dem_SharedDataPtrType) &Dem_Cfg_GetPrimaryEntry_0()        /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_PrimaryEntry0] */
  /*     1 */ (Dem_SharedDataPtrType) &Dem_Cfg_GetPrimaryEntry_1()        /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_PrimaryEntry1] */
  /*     2 */ (Dem_SharedDataPtrType) &Dem_Cfg_GetPrimaryEntry_2()        /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_PrimaryEntry2] */
  /*     3 */ (Dem_SharedDataPtrType) &Dem_Cfg_GetPrimaryEntry_3()        /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_PrimaryEntry3] */
  /*     4 */ (Dem_SharedDataPtrType) &Dem_Cfg_GetPrimaryEntry_4()        /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_PrimaryEntry4] */
  /*     5 */ (Dem_SharedDataPtrType) &Dem_Cfg_GetPrimaryEntry_5()        /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_PrimaryEntry5] */
  /*     6 */ (Dem_SharedDataPtrType) &Dem_Cfg_GetPrimaryEntry_6()        /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_PrimaryEntry6] */
  /*     7 */ (Dem_SharedDataPtrType) &Dem_Cfg_GetPrimaryEntry_7()        /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */,  /* [Dem_PrimaryEntry7] */
  /*     8 */ (Dem_SharedDataPtrType) &Dem_Cfg_GetReadoutBuffer(0).Data   /* PRQA S 0310 */ /* MD_DEM_11.4_nvm */   /* [Dem_Cfg_ReadoutBuffer[0].Data] */
};
#define DEM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_MemoryEntryInit
**********************************************************************************************************************/
#define DEM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(Dem_Cfg_PrimaryEntryType, DEM_CONST) Dem_Cfg_MemoryEntryInit = { 0 };
#define DEM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_TimeSeriesEntryInit
**********************************************************************************************************************/
#define DEM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(Dem_Cfg_TimeSeriesEntryType, DEM_CONST) Dem_Cfg_TimeSeriesEntryInit = { 0 };
#define DEM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_AdminData
**********************************************************************************************************************/
#define DEM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_AdminDataType, DEM_NVM_DATA_NOINIT) Dem_Cfg_AdminData;
#define DEM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_ClearDTCTable
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_ClearDTCTable
  \brief  size = DEM_CFG_NUMBER_OF_CLEARDTCS
*/ 
#define DEM_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_ClearDTC_DataType, DEM_VAR_NOINIT) Dem_Cfg_ClearDTCTable[1];
#define DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_CommitBuffer
**********************************************************************************************************************/
#define DEM_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_CommitBufferType, DEM_VAR_NOINIT) Dem_Cfg_CommitBuffer;  /* PRQA S 0759 */ /* MD_MSR_18.4 */
#define DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_DTCSelectorTable
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_DTCSelectorTable
  \brief  size = DEM_CFG_NUMBER_OF_DTCSELECTORS
*/ 
#define DEM_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_DTCSelector_DataType, DEM_VAR_NOINIT) Dem_Cfg_DTCSelectorTable[1];
#define DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_EnableConditionGroupCounter
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_EnableConditionGroupCounter
  \brief  (DEM_CFG_SUPPORT_ENABLE_CONDITIONS == STD_ON) or there are internal EnableConditions. Table index: Condition group number. Table value: count of conditions in state 'enable'.
*/ 
#define DEM_START_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_EnableConditionGroupCounterType, DEM_VAR_NOINIT) Dem_Cfg_EnableConditionGroupCounter[1];
  /* Index        Referable Keys  */
  /*     0 */  /* [##NoEnableConditionGroupConfigured, __Internal_ControlDtcSetting] */

#define DEM_STOP_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_EnableConditionGroupState
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_EnableConditionGroupState
  \brief  (DEM_CFG_SUPPORT_ENABLE_CONDITIONS == STD_ON) or there are internal EnableConditions. Table index: Condition group number. Table value: count of conditions in state 'enable'.
*/ 
#define DEM_START_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
volatile VAR(Dem_Cfg_EnableConditionGroupStateType, DEM_VAR_NOINIT) Dem_Cfg_EnableConditionGroupState[1];
  /* Index        Referable Keys  */
  /*     0 */  /* [##NoEnableConditionGroupConfigured, __Internal_ControlDtcSetting] */

#define DEM_STOP_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_EnableConditionState
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_EnableConditionState
  \brief  (DEM_CFG_SUPPORT_ENABLE_CONDITIONS == STD_ON) or there are internal EnableConditions. Table index: Condition ID. Table value: current condition state '0' disable, '1' enable.
*/ 
#define DEM_START_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_EnableConditionStateType, DEM_VAR_NOINIT) Dem_Cfg_EnableConditionState[1];
#define DEM_STOP_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_EventDebounceValue
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_EventDebounceValue
  \brief  size = DEM_G_NUMBER_OF_EVENTS
*/ 
#define DEM_START_SEC_VAR_NOINIT_16BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
volatile VAR(Dem_Cfg_EventDebounceValueType, DEM_VAR_NOINIT) Dem_Cfg_EventDebounceValue[13];
  /* Index        Referable Keys  */
  /*     0 */  /* [#EVENT_INVALID, Satellite#0] */
  /*     1 */  /* [AutoCreatedDemEvent_NVM_E_INTEGRITY_FAILED, Satellite#0] */
  /*     2 */  /* [AutoCreatedDemEvent_NVM_E_LOSS_OF_REDUNDANCY, Satellite#0] */
  /*     3 */  /* [AutoCreatedDemEvent_NVM_E_QUEUE_OVERFLOW, Satellite#0] */
  /*     4 */  /* [AutoCreatedDemEvent_NVM_E_REQ_FAILED, Satellite#0] */
  /*     5 */  /* [AutoCreatedDemEvent_NVM_E_VERIFY_FAILED, Satellite#0] */
  /*     6 */  /* [AutoCreatedDemEvent_NVM_E_WRITE_PROTECTED, Satellite#0] */
  /*     7 */  /* [AutoCreatedDemEvent_NVM_E_WRONG_BLOCK_ID, Satellite#0] */
  /*     8 */  /* [AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_ALL_RUN_REQUESTS_KILLED, Satellite#0] */
  /*     9 */  /* [AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_CONFIGURATION_DATA_INCONSISTENT, Satellite#0] */
  /*    10 */  /* [AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_IMPROPER_CALLER, Satellite#0] */
  /*    11 */  /* [AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_RAM_CHECK_FAILED, Satellite#0] */
  /*    12 */  /* [DTC_0x000002, Satellite#0] */

#define DEM_STOP_SEC_VAR_NOINIT_16BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_EventInternalStatus
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_EventInternalStatus
  \brief  size = DEM_G_NUMBER_OF_EVENTS
*/ 
#define DEM_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
volatile VAR(Dem_Event_InternalStatusType, DEM_VAR_NOINIT) Dem_Cfg_EventInternalStatus[13];
  /* Index        Referable Keys  */
  /*     0 */  /* [#EVENT_INVALID, Satellite#0] */
  /*     1 */  /* [AutoCreatedDemEvent_NVM_E_INTEGRITY_FAILED, Satellite#0] */
  /*     2 */  /* [AutoCreatedDemEvent_NVM_E_LOSS_OF_REDUNDANCY, Satellite#0] */
  /*     3 */  /* [AutoCreatedDemEvent_NVM_E_QUEUE_OVERFLOW, Satellite#0] */
  /*     4 */  /* [AutoCreatedDemEvent_NVM_E_REQ_FAILED, Satellite#0] */
  /*     5 */  /* [AutoCreatedDemEvent_NVM_E_VERIFY_FAILED, Satellite#0] */
  /*     6 */  /* [AutoCreatedDemEvent_NVM_E_WRITE_PROTECTED, Satellite#0] */
  /*     7 */  /* [AutoCreatedDemEvent_NVM_E_WRONG_BLOCK_ID, Satellite#0] */
  /*     8 */  /* [AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_ALL_RUN_REQUESTS_KILLED, Satellite#0] */
  /*     9 */  /* [AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_CONFIGURATION_DATA_INCONSISTENT, Satellite#0] */
  /*    10 */  /* [AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_IMPROPER_CALLER, Satellite#0] */
  /*    11 */  /* [AutoCreatedDemEvent_EcuMConfiguration_ECUM_E_RAM_CHECK_FAILED, Satellite#0] */
  /*    12 */  /* [DTC_0x000002, Satellite#0] */

#define DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_FilterInfoTable
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_FilterInfoTable
  \brief  size = DEM_CFG_NUMBER_OF_FILTER
*/ 
#define DEM_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_FilterData_InfoType, DEM_VAR_NOINIT) Dem_Cfg_FilterInfoTable[1];
#define DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_FilterReportedEvents
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_FilterReportedEvents
  \brief  size = ceiling( DEM_G_NUMBER_OF_EVENTS / 8 )
*/ 
#define DEM_START_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_FilterReportedEventsType, DEM_VAR_NOINIT) Dem_Cfg_FilterReportedEvents[2];
#define DEM_STOP_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_FreezeFrameIteratorTable
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_FreezeFrameIteratorTable
  \brief  size = DEM_CFG_NUMBER_OF_FREEZEFRAMEITERATORS
*/ 
#define DEM_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_FreezeFrameIterator_FilterType, DEM_VAR_NOINIT) Dem_Cfg_FreezeFrameIteratorTable[1];
#define DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_MemoryCommitNumber
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_MemoryCommitNumber
  \brief  The array contains these items: Admin, Status, 8 * Primary
*/ 
#define DEM_START_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
volatile VAR(Dem_Cfg_MemoryCommitNumberType, DEM_VAR_NOINIT) Dem_Cfg_MemoryCommitNumber[10];
#define DEM_STOP_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_MemoryStatus
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_MemoryStatus
  \brief  The array contains these items: Admin, Status, 8 * Primary
*/ 
#define DEM_START_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_MemoryStatusType, DEM_VAR_NOINIT) Dem_Cfg_MemoryStatus[10];
#define DEM_STOP_SEC_VAR_NOINIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_PrimaryChronology
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_PrimaryChronology
  \brief  size = DEM_CFG_GLOBAL_PRIMARY_SIZE
*/ 
#define DEM_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_MemoryIndexType, DEM_VAR_NOINIT) Dem_Cfg_PrimaryChronology[8];
#define DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_PrimaryEntry_0
**********************************************************************************************************************/
#define DEM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_PrimaryEntryType, DEM_NVM_DATA_NOINIT) Dem_Cfg_PrimaryEntry_0;
#define DEM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_PrimaryEntry_1
**********************************************************************************************************************/
#define DEM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_PrimaryEntryType, DEM_NVM_DATA_NOINIT) Dem_Cfg_PrimaryEntry_1;
#define DEM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_PrimaryEntry_2
**********************************************************************************************************************/
#define DEM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_PrimaryEntryType, DEM_NVM_DATA_NOINIT) Dem_Cfg_PrimaryEntry_2;
#define DEM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_PrimaryEntry_3
**********************************************************************************************************************/
#define DEM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_PrimaryEntryType, DEM_NVM_DATA_NOINIT) Dem_Cfg_PrimaryEntry_3;
#define DEM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_PrimaryEntry_4
**********************************************************************************************************************/
#define DEM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_PrimaryEntryType, DEM_NVM_DATA_NOINIT) Dem_Cfg_PrimaryEntry_4;
#define DEM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_PrimaryEntry_5
**********************************************************************************************************************/
#define DEM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_PrimaryEntryType, DEM_NVM_DATA_NOINIT) Dem_Cfg_PrimaryEntry_5;
#define DEM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_PrimaryEntry_6
**********************************************************************************************************************/
#define DEM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_PrimaryEntryType, DEM_NVM_DATA_NOINIT) Dem_Cfg_PrimaryEntry_6;
#define DEM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_PrimaryEntry_7
**********************************************************************************************************************/
#define DEM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_PrimaryEntryType, DEM_NVM_DATA_NOINIT) Dem_Cfg_PrimaryEntry_7;
#define DEM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_ReadoutBuffer
**********************************************************************************************************************/
#define DEM_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_ReadoutBufferEntryType, DEM_VAR_NOINIT) Dem_Cfg_ReadoutBuffer[1];
#define DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_ReportedEventsOfFilter
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_ReportedEventsOfFilter
  \brief  size = DEM_CFG_NUMBER_OF_FILTER
*/ 
#define DEM_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_ReportedEventsType, DEM_VAR_NOINIT) Dem_Cfg_ReportedEventsOfFilter[1];
#define DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_SatelliteInfo0
**********************************************************************************************************************/
/** 
  \var    Dem_Cfg_SatelliteInfo0
  \brief  Buffer for satellite data on OsApplication "0"
*/ 
#define DEM_START_SEC_0_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_SatelliteInfoType0, DEM_VAR_NOINIT) Dem_Cfg_SatelliteInfo0;
#define DEM_STOP_SEC_0_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  Dem_Cfg_StatusData
**********************************************************************************************************************/
#define DEM_START_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(Dem_Cfg_StatusDataType, DEM_NVM_DATA_NOINIT) Dem_Cfg_StatusData;
#define DEM_STOP_SEC_VAR_SAVED_ZONE0_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: GLOBAL DATA
**********************************************************************************************************************/



/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL FUNCTIONS
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: GLOBAL FUNCTIONS
**********************************************************************************************************************/



/**********************************************************************************************************************
  END OF FILE: Dem_Lcfg.c     [Vector, VARIANT-PRE-COMPILE, 11.06.01.91134]
**********************************************************************************************************************/

