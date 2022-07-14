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
 *            Module: NvM
 *           Program: 
 *          Customer: N/A
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: 
 *    License Scope : The usage is restricted to CBD0000000_D80
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: NvM_Cfg.c
 *   Generation Time: 2020-03-02 16:11:30
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/


    
/**********************************************************************************************************************
 *  MODULE SWITCH
 *********************************************************************************************************************/
/* this switch enables the header file(s) to hide some parts. */
#define NVM_CFG_SOURCE

/* multiple inclusion protection */
#define NVM_H_

/* PRQA S 5087 inclusions *//* MD_MSR_19.1 */

/* Define RTE_MICROSAR_PIM_EXPORT is necessary only when serviceports are used */
#define RTE_MICROSAR_PIM_EXPORT  


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"

/* This tag will only be replaced, if one or more callbacks via Service Ports had been configured */
#include "Rte_NvM.h"

/**********************************************************************************************************************
 *  MODULE HEADER INCLUDES
 *********************************************************************************************************************/
/* only includes the public part of config */
#include "NvM_Cfg.h"
#include "NvM_PrivateCfg.h"

#include "BswM_NvM.h"

/**********************************************************************************************************************
 *  VERSION CHECKS
 *********************************************************************************************************************/
#if ((NVM_CFG_MAJOR_VERSION != (5u)) \
        || (NVM_CFG_MINOR_VERSION != (7u)))
# error "Version numbers of NvM_Cfg.c and NvM_Cfg.h are inconsistent!"
#endif

#if ((NVM_PRIV_CFG_MAJOR_VERSION != NVM_CFG_MAJOR_VERSION) \
        || (NVM_PRIV_CFG_MINOR_VERSION != NVM_CFG_MINOR_VERSION))
# error "Version numbers of NvM_Cfg.c and Nvm_PrivateCfg.h.h are inconsistent!"
#endif

#if ((NVM_TYPES_MAJOR_VERSION != NVM_CFG_MAJOR_VERSION) \
        || (NVM_TYPES_MINOR_VERSION != NVM_CFG_MINOR_VERSION))
# error "Version numbers of NvM_Cfg.c and NvM_Types.h are inconsistent!"
#endif

/* include list of the callback definitions */
#include "main.h" 
#include "Dem.h" 
#include "Dem_Cbk.h" 


/* include configured file declaring or defining resource (un)locking service(s) */
#include "SchM_NvM.h"

#define NVM_SIZE_STANDARD_JOB_QUEUE  (2U) 
#define NVM_SIZE_IMMEDIATE_JOB_QUEUE (0U) 


/**********************************************************************************************************************
 *  PUBLIC CONSTANTS
 *********************************************************************************************************************/

#define NVM_START_SEC_CONST_8
#include "MemMap.h"

/* Number of write attempts: 1 + number of retries */
CONST(uint8, NVM_PRIVATE_CONST) NvM_NoOfWrAttempts_u8 = 1U;

#define NVM_STOP_SEC_CONST_8
#include "MemMap.h"

#define NVM_START_SEC_CONST_16
#include "MemMap.h"

/* maximum number of bytes to be processed in one crc calculation step */
CONST(uint16, NVM_CONFIG_CONST) NvM_NoOfCrcBytes_u16 = 64U;

/* constant holding Crc quueue size value */
CONST(uint16, NVM_PRIVATE_CONST) NvM_CrcQueueSize_u16 = NVM_TOTAL_NUM_OF_NVRAM_BLOCKS;

#define NVM_STOP_SEC_CONST_16
#include "MemMap.h"

/* close section, to ensure unique section ends */
#define NVM_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h"

/* constants holding the compiled queue size values */
CONST(NvM_QueueSizesType, NVM_PRIVATE_CONST) NvM_QueueSizes_t =
{
    /* MISRA wants to define a operator precedence with parentheses */
    (NVM_SIZE_STANDARD_JOB_QUEUE + NVM_SIZE_IMMEDIATE_JOB_QUEUE) - 1u,
    NVM_SIZE_STANDARD_JOB_QUEUE
};

/* close section, to ensure unique section ends */
#define NVM_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h"

/* 8Bit Data section containing the CRC buffers, as well as the internal buffer */
#define NVM_START_SEC_VAR_NOINIT_8
#include "MemMap.h"

static VAR(uint8, NVM_PRIVATE_DATA) NvMConfigBlock_RamBlock_au8[4U];

#if ((NVM_CRC_INT_BUFFER == STD_ON) || (NVM_REPAIR_REDUNDANT_BLOCKS_API == STD_ON))
static VAR(uint8, NVM_PRIVATE_DATA) DemAdminDataBlock_Crc_au8[2UL]; 
static VAR(uint8, NVM_PRIVATE_DATA) CpApMySwcSnNvMPimLightOnOffCounter_Crc_au8[2UL]; 
static VAR(uint8, NVM_PRIVATE_DATA) CpApMySwcSnNvMPimOdometer_Crc_au8[2UL]; 
static VAR(uint8, NVM_PRIVATE_DATA) DemStatusDataBlock_Crc_au8[2UL]; 
static VAR(uint8, NVM_PRIVATE_DATA) DemPrimaryDataBlock0_Crc_au8[2UL]; 
static VAR(uint8, NVM_PRIVATE_DATA) DemPrimaryDataBlock1_Crc_au8[2UL]; 
static VAR(uint8, NVM_PRIVATE_DATA) DemPrimaryDataBlock2_Crc_au8[2UL]; 
static VAR(uint8, NVM_PRIVATE_DATA) DemPrimaryDataBlock3_Crc_au8[2UL]; 
static VAR(uint8, NVM_PRIVATE_DATA) DemPrimaryDataBlock4_Crc_au8[2UL]; 
static VAR(uint8, NVM_PRIVATE_DATA) DemPrimaryDataBlock5_Crc_au8[2UL]; 
static VAR(uint8, NVM_PRIVATE_DATA) DemPrimaryDataBlock6_Crc_au8[2UL]; 
static VAR(uint8, NVM_PRIVATE_DATA) DemPrimaryDataBlock7_Crc_au8[2UL]; 


/* create the internal buffer of size NVM_INTERNAL_BUFFER_LENGTH */
VAR(uint8, NVM_PRIVATE_DATA) NvM_InternalBuffer_au8[NVM_INTERNAL_BUFFER_LENGTH];
#endif

/* create a test buffer of size 1 byte for testing readability of a block */
VAR(uint8, NVM_PRIVATE_DATA) NvM_TestBuffer_u8;

#define NVM_STOP_SEC_VAR_NOINIT_8
#include "MemMap.h"

#define NVM_START_SEC_CONST_DESCRIPTOR_TABLE
#include "MemMap.h"

CONST(NvM_BlockIdType, NVM_PUBLIC_CONST) NvM_NoOfBlockIds_t = NVM_TOTAL_NUM_OF_NVRAM_BLOCKS;

/* PRQA S 0759 1 */ /* MD_MSR_18.4 */
CONST(NvM_CompiledConfigIdType, NVM_PUBLIC_CONST) NvM_CompiledConfigId_t = {(uint16)NVM_COMPILED_CONFIG_ID};
 
/* block descriptor table that holds the static configuration parameters of the RAM, ROM and NVBlocks.
* This table has to be adjusted according to the configuration of the NVManager.
*/
     
/* PRQA S 0310 RamRomBlocks *//* MD_NvM_11.4 */
CONST(NvM_BlockDescriptorType, NVM_CONFIG_CONST) NvM_BlockDescriptorTable_at[NVM_TOTAL_NUM_OF_NVRAM_BLOCKS] =
    {
      { /*  MultiBlockRequest  */ 
        NULL_PTR /*  pointer to the RAM data buffer  */ , 
        NULL_PTR /*  pointer to the ROM buffer  */ , 
        NULL_PTR /*  pointer to the Init Callback function  */ , 
        NULL_PTR /*  pointer to the job End Callback function  */ , 
        NULL_PTR /*  pointer to the ReadRamBlockFromNvCallback  */ , 
        NULL_PTR /*  pointer to the WriteRamBlockToNvCallback  */ , 
        NULL_PTR /*  pointer to the CRC data buffer  */ , 
        0x0001U /*  NV block Base number (defined by FEE/EA)  */ , 
        0 /*  Length of the block in bytes  */ , 
        0U /*  Number of multiple blocks  */ , 
        255U /*  Block Priority  */ , 
        0u /*  Device Id (defined by MemIf)  */ , 
        NVM_BLOCK_NATIVE /*  Management Type  */ , 
        NVM_BLOCK_USE_CRC_OFF /*  CRC Type  */ , 
        0u /*  Flags  */ , 
        STD_OFF /*  BswM Block Status Information enabled  */ 
      }, 
      { /*  NvMConfigBlock  */ 
        (NvM_RamAddressType)NvMConfigBlock_RamBlock_au8 /*  pointer to the RAM data buffer  */ , 
        NULL_PTR /*  pointer to the ROM buffer  */ , 
        NULL_PTR /*  pointer to the Init Callback function  */ , 
        NULL_PTR /*  pointer to the job End Callback function  */ , 
        NULL_PTR /*  pointer to the ReadRamBlockFromNvCallback  */ , 
        NULL_PTR /*  pointer to the WriteRamBlockToNvCallback  */ , 
        NULL_PTR /*  pointer to the CRC data buffer  */ , 
        0x00D0U /*  NV block Base number (defined by FEE/EA)  */ , 
        2U /*  Length of the block in bytes  */ , 
        2U /*  Number of multiple blocks  */ , 
        127U /*  Block Priority  */ , 
        MEMIF_Fee /*  Device Id (defined by MemIf)  */ , 
        NVM_BLOCK_REDUNDANT /*  Management Type  */ , 
        NVM_BLOCK_CRC_16_ON /*  CRC Type  */ , 
        (
NVM_CALC_RAM_BLOCK_CRC_OFF |
NVM_BLOCK_WRITE_PROT_OFF |
NVM_BLOCK_WRITE_BLOCK_ONCE_OFF |
NVM_RESISTANT_TO_CHANGED_SW_ON |
NVM_SELECT_BLOCK_FOR_READALL_OFF |
NVM_SELECT_BLOCK_FOR_WRITEALL_ON |
NVM_CBK_DURING_READALL_OFF |
NVM_CRC_COMP_MECHANISM_OFF
) /*  Flags  */ , 
        STD_OFF /*  BswM Block Status Information enabled  */ 
      }, 
      { /*  DemAdminDataBlock  */ 
        (NvM_RamAddressType)&Dem_Cfg_AdminData /*  pointer to the RAM data buffer  */ , 
        NULL_PTR /*  pointer to the ROM buffer  */ , 
        Dem_NvM_InitAdminData /*  pointer to the Init Callback function  */ , 
        Dem_NvM_JobFinished /*  pointer to the job End Callback function  */ , 
        NULL_PTR /*  pointer to the ReadRamBlockFromNvCallback  */ , 
        NULL_PTR /*  pointer to the WriteRamBlockToNvCallback  */ , 
        DemAdminDataBlock_Crc_au8 /*  pointer to the CRC data buffer  */ , 
        0x0030U /*  NV block Base number (defined by FEE/EA)  */ , 
        10U /*  Length of the block in bytes  */ , 
        1U /*  Number of multiple blocks  */ , 
        127U /*  Block Priority  */ , 
        MEMIF_Fee /*  Device Id (defined by MemIf)  */ , 
        NVM_BLOCK_NATIVE /*  Management Type  */ , 
        NVM_BLOCK_CRC_16_ON /*  CRC Type  */ , 
        (
NVM_CALC_RAM_BLOCK_CRC_OFF |
NVM_BLOCK_WRITE_PROT_OFF |
NVM_BLOCK_WRITE_BLOCK_ONCE_OFF |
NVM_RESISTANT_TO_CHANGED_SW_ON |
NVM_SELECT_BLOCK_FOR_READALL_ON |
NVM_SELECT_BLOCK_FOR_WRITEALL_ON |
NVM_CBK_DURING_READALL_ON |
NVM_CRC_COMP_MECHANISM_OFF
) /*  Flags  */ , 
        STD_OFF /*  BswM Block Status Information enabled  */ 
      }, 
      { /*  CpApMySwcSnNvMPimLightOnOffCounter  */ 
        (NvM_RamAddressType)&Rte_CpApMySwc_PimLightOnOffCounter /*  pointer to the RAM data buffer  */ , 
        NULL_PTR /*  pointer to the ROM buffer  */ , 
        NULL_PTR /*  pointer to the Init Callback function  */ , 
        NULL_PTR /*  pointer to the job End Callback function  */ , 
        NULL_PTR /*  pointer to the ReadRamBlockFromNvCallback  */ , 
        NULL_PTR /*  pointer to the WriteRamBlockToNvCallback  */ , 
        CpApMySwcSnNvMPimLightOnOffCounter_Crc_au8 /*  pointer to the CRC data buffer  */ , 
        0x0010U /*  NV block Base number (defined by FEE/EA)  */ , 
        4U /*  Length of the block in bytes  */ , 
        1U /*  Number of multiple blocks  */ , 
        127U /*  Block Priority  */ , 
        MEMIF_Fee /*  Device Id (defined by MemIf)  */ , 
        NVM_BLOCK_NATIVE /*  Management Type  */ , 
        NVM_BLOCK_CRC_16_ON /*  CRC Type  */ , 
        (
NVM_CALC_RAM_BLOCK_CRC_OFF |
NVM_BLOCK_WRITE_PROT_OFF |
NVM_BLOCK_WRITE_BLOCK_ONCE_OFF |
NVM_RESISTANT_TO_CHANGED_SW_OFF |
NVM_SELECT_BLOCK_FOR_READALL_ON |
NVM_SELECT_BLOCK_FOR_WRITEALL_ON |
NVM_CBK_DURING_READALL_OFF |
NVM_CRC_COMP_MECHANISM_OFF
) /*  Flags  */ , 
        STD_OFF /*  BswM Block Status Information enabled  */ 
      }, 
      { /*  CpApMySwcSnNvMPimOdometer  */ 
        (NvM_RamAddressType)&Rte_CpApMySwc_PimOdometer /*  pointer to the RAM data buffer  */ , 
        (NvM_RomAddressType)&Rte_CtApMySwc_CalOdometer /*  pointer to the ROM buffer  */ , 
        NULL_PTR /*  pointer to the Init Callback function  */ , 
        Rte_Call_NvM_RpNotifyJobEnd_CpApMySwcSnNvMPimOdometer_JobFinished /*  pointer to the job End Callback function  */ , 
        NULL_PTR /*  pointer to the ReadRamBlockFromNvCallback  */ , 
        NULL_PTR /*  pointer to the WriteRamBlockToNvCallback  */ , 
        CpApMySwcSnNvMPimOdometer_Crc_au8 /*  pointer to the CRC data buffer  */ , 
        0x0020U /*  NV block Base number (defined by FEE/EA)  */ , 
        4U /*  Length of the block in bytes  */ , 
        1U /*  Number of multiple blocks  */ , 
        127U /*  Block Priority  */ , 
        MEMIF_Fee /*  Device Id (defined by MemIf)  */ , 
        NVM_BLOCK_NATIVE /*  Management Type  */ , 
        NVM_BLOCK_CRC_16_ON /*  CRC Type  */ , 
        (
NVM_CALC_RAM_BLOCK_CRC_OFF |
NVM_BLOCK_WRITE_PROT_OFF |
NVM_BLOCK_WRITE_BLOCK_ONCE_OFF |
NVM_RESISTANT_TO_CHANGED_SW_OFF |
NVM_SELECT_BLOCK_FOR_READALL_ON |
NVM_SELECT_BLOCK_FOR_WRITEALL_ON |
NVM_CBK_DURING_READALL_OFF |
NVM_CRC_COMP_MECHANISM_OFF
) /*  Flags  */ , 
        STD_OFF /*  BswM Block Status Information enabled  */ 
      }, 
      { /*  DemStatusDataBlock  */ 
        (NvM_RamAddressType)&Dem_Cfg_StatusData /*  pointer to the RAM data buffer  */ , 
        NULL_PTR /*  pointer to the ROM buffer  */ , 
        Dem_NvM_InitStatusData /*  pointer to the Init Callback function  */ , 
        Dem_NvM_JobFinished /*  pointer to the job End Callback function  */ , 
        NULL_PTR /*  pointer to the ReadRamBlockFromNvCallback  */ , 
        NULL_PTR /*  pointer to the WriteRamBlockToNvCallback  */ , 
        DemStatusDataBlock_Crc_au8 /*  pointer to the CRC data buffer  */ , 
        0x00C0U /*  NV block Base number (defined by FEE/EA)  */ , 
        22U /*  Length of the block in bytes  */ , 
        1U /*  Number of multiple blocks  */ , 
        127U /*  Block Priority  */ , 
        MEMIF_Fee /*  Device Id (defined by MemIf)  */ , 
        NVM_BLOCK_NATIVE /*  Management Type  */ , 
        NVM_BLOCK_CRC_16_ON /*  CRC Type  */ , 
        (
NVM_CALC_RAM_BLOCK_CRC_OFF |
NVM_BLOCK_WRITE_PROT_OFF |
NVM_BLOCK_WRITE_BLOCK_ONCE_OFF |
NVM_RESISTANT_TO_CHANGED_SW_ON |
NVM_SELECT_BLOCK_FOR_READALL_ON |
NVM_SELECT_BLOCK_FOR_WRITEALL_ON |
NVM_CBK_DURING_READALL_ON |
NVM_CRC_COMP_MECHANISM_OFF
) /*  Flags  */ , 
        STD_OFF /*  BswM Block Status Information enabled  */ 
      }, 
      { /*  DemPrimaryDataBlock0  */ 
        (NvM_RamAddressType)&Dem_Cfg_PrimaryEntry_0 /*  pointer to the RAM data buffer  */ , 
        (NvM_RomAddressType)&Dem_Cfg_MemoryEntryInit /*  pointer to the ROM buffer  */ , 
        NULL_PTR /*  pointer to the Init Callback function  */ , 
        Dem_NvM_JobFinished /*  pointer to the job End Callback function  */ , 
        NULL_PTR /*  pointer to the ReadRamBlockFromNvCallback  */ , 
        NULL_PTR /*  pointer to the WriteRamBlockToNvCallback  */ , 
        DemPrimaryDataBlock0_Crc_au8 /*  pointer to the CRC data buffer  */ , 
        0x0040U /*  NV block Base number (defined by FEE/EA)  */ , 
        20U /*  Length of the block in bytes  */ , 
        1U /*  Number of multiple blocks  */ , 
        127U /*  Block Priority  */ , 
        MEMIF_Fee /*  Device Id (defined by MemIf)  */ , 
        NVM_BLOCK_NATIVE /*  Management Type  */ , 
        NVM_BLOCK_CRC_16_ON /*  CRC Type  */ , 
        (
NVM_CALC_RAM_BLOCK_CRC_OFF |
NVM_BLOCK_WRITE_PROT_OFF |
NVM_BLOCK_WRITE_BLOCK_ONCE_OFF |
NVM_RESISTANT_TO_CHANGED_SW_ON |
NVM_SELECT_BLOCK_FOR_READALL_ON |
NVM_SELECT_BLOCK_FOR_WRITEALL_ON |
NVM_CBK_DURING_READALL_ON |
NVM_CRC_COMP_MECHANISM_OFF
) /*  Flags  */ , 
        STD_OFF /*  BswM Block Status Information enabled  */ 
      }, 
      { /*  DemPrimaryDataBlock1  */ 
        (NvM_RamAddressType)&Dem_Cfg_PrimaryEntry_1 /*  pointer to the RAM data buffer  */ , 
        (NvM_RomAddressType)&Dem_Cfg_MemoryEntryInit /*  pointer to the ROM buffer  */ , 
        NULL_PTR /*  pointer to the Init Callback function  */ , 
        Dem_NvM_JobFinished /*  pointer to the job End Callback function  */ , 
        NULL_PTR /*  pointer to the ReadRamBlockFromNvCallback  */ , 
        NULL_PTR /*  pointer to the WriteRamBlockToNvCallback  */ , 
        DemPrimaryDataBlock1_Crc_au8 /*  pointer to the CRC data buffer  */ , 
        0x0050U /*  NV block Base number (defined by FEE/EA)  */ , 
        20U /*  Length of the block in bytes  */ , 
        1U /*  Number of multiple blocks  */ , 
        127U /*  Block Priority  */ , 
        MEMIF_Fee /*  Device Id (defined by MemIf)  */ , 
        NVM_BLOCK_NATIVE /*  Management Type  */ , 
        NVM_BLOCK_CRC_16_ON /*  CRC Type  */ , 
        (
NVM_CALC_RAM_BLOCK_CRC_OFF |
NVM_BLOCK_WRITE_PROT_OFF |
NVM_BLOCK_WRITE_BLOCK_ONCE_OFF |
NVM_RESISTANT_TO_CHANGED_SW_ON |
NVM_SELECT_BLOCK_FOR_READALL_ON |
NVM_SELECT_BLOCK_FOR_WRITEALL_ON |
NVM_CBK_DURING_READALL_ON |
NVM_CRC_COMP_MECHANISM_OFF
) /*  Flags  */ , 
        STD_OFF /*  BswM Block Status Information enabled  */ 
      }, 
      { /*  DemPrimaryDataBlock2  */ 
        (NvM_RamAddressType)&Dem_Cfg_PrimaryEntry_2 /*  pointer to the RAM data buffer  */ , 
        (NvM_RomAddressType)&Dem_Cfg_MemoryEntryInit /*  pointer to the ROM buffer  */ , 
        NULL_PTR /*  pointer to the Init Callback function  */ , 
        Dem_NvM_JobFinished /*  pointer to the job End Callback function  */ , 
        NULL_PTR /*  pointer to the ReadRamBlockFromNvCallback  */ , 
        NULL_PTR /*  pointer to the WriteRamBlockToNvCallback  */ , 
        DemPrimaryDataBlock2_Crc_au8 /*  pointer to the CRC data buffer  */ , 
        0x0060U /*  NV block Base number (defined by FEE/EA)  */ , 
        20U /*  Length of the block in bytes  */ , 
        1U /*  Number of multiple blocks  */ , 
        127U /*  Block Priority  */ , 
        MEMIF_Fee /*  Device Id (defined by MemIf)  */ , 
        NVM_BLOCK_NATIVE /*  Management Type  */ , 
        NVM_BLOCK_CRC_16_ON /*  CRC Type  */ , 
        (
NVM_CALC_RAM_BLOCK_CRC_OFF |
NVM_BLOCK_WRITE_PROT_OFF |
NVM_BLOCK_WRITE_BLOCK_ONCE_OFF |
NVM_RESISTANT_TO_CHANGED_SW_ON |
NVM_SELECT_BLOCK_FOR_READALL_ON |
NVM_SELECT_BLOCK_FOR_WRITEALL_ON |
NVM_CBK_DURING_READALL_ON |
NVM_CRC_COMP_MECHANISM_OFF
) /*  Flags  */ , 
        STD_OFF /*  BswM Block Status Information enabled  */ 
      }, 
      { /*  DemPrimaryDataBlock3  */ 
        (NvM_RamAddressType)&Dem_Cfg_PrimaryEntry_3 /*  pointer to the RAM data buffer  */ , 
        (NvM_RomAddressType)&Dem_Cfg_MemoryEntryInit /*  pointer to the ROM buffer  */ , 
        NULL_PTR /*  pointer to the Init Callback function  */ , 
        Dem_NvM_JobFinished /*  pointer to the job End Callback function  */ , 
        NULL_PTR /*  pointer to the ReadRamBlockFromNvCallback  */ , 
        NULL_PTR /*  pointer to the WriteRamBlockToNvCallback  */ , 
        DemPrimaryDataBlock3_Crc_au8 /*  pointer to the CRC data buffer  */ , 
        0x0070U /*  NV block Base number (defined by FEE/EA)  */ , 
        20U /*  Length of the block in bytes  */ , 
        1U /*  Number of multiple blocks  */ , 
        127U /*  Block Priority  */ , 
        MEMIF_Fee /*  Device Id (defined by MemIf)  */ , 
        NVM_BLOCK_NATIVE /*  Management Type  */ , 
        NVM_BLOCK_CRC_16_ON /*  CRC Type  */ , 
        (
NVM_CALC_RAM_BLOCK_CRC_OFF |
NVM_BLOCK_WRITE_PROT_OFF |
NVM_BLOCK_WRITE_BLOCK_ONCE_OFF |
NVM_RESISTANT_TO_CHANGED_SW_ON |
NVM_SELECT_BLOCK_FOR_READALL_ON |
NVM_SELECT_BLOCK_FOR_WRITEALL_ON |
NVM_CBK_DURING_READALL_ON |
NVM_CRC_COMP_MECHANISM_OFF
) /*  Flags  */ , 
        STD_OFF /*  BswM Block Status Information enabled  */ 
      }, 
      { /*  DemPrimaryDataBlock4  */ 
        (NvM_RamAddressType)&Dem_Cfg_PrimaryEntry_4 /*  pointer to the RAM data buffer  */ , 
        (NvM_RomAddressType)&Dem_Cfg_MemoryEntryInit /*  pointer to the ROM buffer  */ , 
        NULL_PTR /*  pointer to the Init Callback function  */ , 
        Dem_NvM_JobFinished /*  pointer to the job End Callback function  */ , 
        NULL_PTR /*  pointer to the ReadRamBlockFromNvCallback  */ , 
        NULL_PTR /*  pointer to the WriteRamBlockToNvCallback  */ , 
        DemPrimaryDataBlock4_Crc_au8 /*  pointer to the CRC data buffer  */ , 
        0x0080U /*  NV block Base number (defined by FEE/EA)  */ , 
        20U /*  Length of the block in bytes  */ , 
        1U /*  Number of multiple blocks  */ , 
        127U /*  Block Priority  */ , 
        MEMIF_Fee /*  Device Id (defined by MemIf)  */ , 
        NVM_BLOCK_NATIVE /*  Management Type  */ , 
        NVM_BLOCK_CRC_16_ON /*  CRC Type  */ , 
        (
NVM_CALC_RAM_BLOCK_CRC_OFF |
NVM_BLOCK_WRITE_PROT_OFF |
NVM_BLOCK_WRITE_BLOCK_ONCE_OFF |
NVM_RESISTANT_TO_CHANGED_SW_ON |
NVM_SELECT_BLOCK_FOR_READALL_ON |
NVM_SELECT_BLOCK_FOR_WRITEALL_ON |
NVM_CBK_DURING_READALL_ON |
NVM_CRC_COMP_MECHANISM_OFF
) /*  Flags  */ , 
        STD_OFF /*  BswM Block Status Information enabled  */ 
      }, 
      { /*  DemPrimaryDataBlock5  */ 
        (NvM_RamAddressType)&Dem_Cfg_PrimaryEntry_5 /*  pointer to the RAM data buffer  */ , 
        (NvM_RomAddressType)&Dem_Cfg_MemoryEntryInit /*  pointer to the ROM buffer  */ , 
        NULL_PTR /*  pointer to the Init Callback function  */ , 
        Dem_NvM_JobFinished /*  pointer to the job End Callback function  */ , 
        NULL_PTR /*  pointer to the ReadRamBlockFromNvCallback  */ , 
        NULL_PTR /*  pointer to the WriteRamBlockToNvCallback  */ , 
        DemPrimaryDataBlock5_Crc_au8 /*  pointer to the CRC data buffer  */ , 
        0x0090U /*  NV block Base number (defined by FEE/EA)  */ , 
        20U /*  Length of the block in bytes  */ , 
        1U /*  Number of multiple blocks  */ , 
        127U /*  Block Priority  */ , 
        MEMIF_Fee /*  Device Id (defined by MemIf)  */ , 
        NVM_BLOCK_NATIVE /*  Management Type  */ , 
        NVM_BLOCK_CRC_16_ON /*  CRC Type  */ , 
        (
NVM_CALC_RAM_BLOCK_CRC_OFF |
NVM_BLOCK_WRITE_PROT_OFF |
NVM_BLOCK_WRITE_BLOCK_ONCE_OFF |
NVM_RESISTANT_TO_CHANGED_SW_ON |
NVM_SELECT_BLOCK_FOR_READALL_ON |
NVM_SELECT_BLOCK_FOR_WRITEALL_ON |
NVM_CBK_DURING_READALL_ON |
NVM_CRC_COMP_MECHANISM_OFF
) /*  Flags  */ , 
        STD_OFF /*  BswM Block Status Information enabled  */ 
      }, 
      { /*  DemPrimaryDataBlock6  */ 
        (NvM_RamAddressType)&Dem_Cfg_PrimaryEntry_6 /*  pointer to the RAM data buffer  */ , 
        (NvM_RomAddressType)&Dem_Cfg_MemoryEntryInit /*  pointer to the ROM buffer  */ , 
        NULL_PTR /*  pointer to the Init Callback function  */ , 
        Dem_NvM_JobFinished /*  pointer to the job End Callback function  */ , 
        NULL_PTR /*  pointer to the ReadRamBlockFromNvCallback  */ , 
        NULL_PTR /*  pointer to the WriteRamBlockToNvCallback  */ , 
        DemPrimaryDataBlock6_Crc_au8 /*  pointer to the CRC data buffer  */ , 
        0x00A0U /*  NV block Base number (defined by FEE/EA)  */ , 
        20U /*  Length of the block in bytes  */ , 
        1U /*  Number of multiple blocks  */ , 
        127U /*  Block Priority  */ , 
        MEMIF_Fee /*  Device Id (defined by MemIf)  */ , 
        NVM_BLOCK_NATIVE /*  Management Type  */ , 
        NVM_BLOCK_CRC_16_ON /*  CRC Type  */ , 
        (
NVM_CALC_RAM_BLOCK_CRC_OFF |
NVM_BLOCK_WRITE_PROT_OFF |
NVM_BLOCK_WRITE_BLOCK_ONCE_OFF |
NVM_RESISTANT_TO_CHANGED_SW_ON |
NVM_SELECT_BLOCK_FOR_READALL_ON |
NVM_SELECT_BLOCK_FOR_WRITEALL_ON |
NVM_CBK_DURING_READALL_ON |
NVM_CRC_COMP_MECHANISM_OFF
) /*  Flags  */ , 
        STD_OFF /*  BswM Block Status Information enabled  */ 
      }, 
      { /*  DemPrimaryDataBlock7  */ 
        (NvM_RamAddressType)&Dem_Cfg_PrimaryEntry_7 /*  pointer to the RAM data buffer  */ , 
        (NvM_RomAddressType)&Dem_Cfg_MemoryEntryInit /*  pointer to the ROM buffer  */ , 
        NULL_PTR /*  pointer to the Init Callback function  */ , 
        Dem_NvM_JobFinished /*  pointer to the job End Callback function  */ , 
        NULL_PTR /*  pointer to the ReadRamBlockFromNvCallback  */ , 
        NULL_PTR /*  pointer to the WriteRamBlockToNvCallback  */ , 
        DemPrimaryDataBlock7_Crc_au8 /*  pointer to the CRC data buffer  */ , 
        0x00B0U /*  NV block Base number (defined by FEE/EA)  */ , 
        20U /*  Length of the block in bytes  */ , 
        1U /*  Number of multiple blocks  */ , 
        127U /*  Block Priority  */ , 
        MEMIF_Fee /*  Device Id (defined by MemIf)  */ , 
        NVM_BLOCK_NATIVE /*  Management Type  */ , 
        NVM_BLOCK_CRC_16_ON /*  CRC Type  */ , 
        (
NVM_CALC_RAM_BLOCK_CRC_OFF |
NVM_BLOCK_WRITE_PROT_OFF |
NVM_BLOCK_WRITE_BLOCK_ONCE_OFF |
NVM_RESISTANT_TO_CHANGED_SW_ON |
NVM_SELECT_BLOCK_FOR_READALL_ON |
NVM_SELECT_BLOCK_FOR_WRITEALL_ON |
NVM_CBK_DURING_READALL_ON |
NVM_CRC_COMP_MECHANISM_OFF
) /*  Flags  */ , 
        STD_OFF /*  BswM Block Status Information enabled  */ 
      }
    };

/* PRQA L:RamRomBlocks */

/* Permanent RAM and ROM block length checks - compile time (only available for blocks with enabled length check */

/* How does it work:
 * data length = sizeof(ramBlock - CrcLength 
 *     - CRC internal buffer enabled: CRC length == 0, RAM blocks store only data, CRC is handles internally
 *     - CRC internal buffer disabled: CRC length is the number of CRC bytes, for blocks without CRC the length == 0
 *     - for ROM blocks the CRC does not matter
 * Data length has to be > or < or == to configured NvM block length, depending on configuration (see above). 
 * In case the lengths do not match a bitfield with length -1 will be created and shall cause a compiler error.
 * The compiler error shall mark the line with invalid bitfield (bitfield length == -1) - the line includes all required information:
 *     - Block_ + NvM block name
 *     - length error description
 *     - RAM block name, CRC length and configured NvM block length
 */

typedef unsigned int NvM_LengthCheck;
 
/* Block Length Check and Automatic Block Length enabled: error if sizeof RAM block is greater than the configured block length */
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
#define SizeOfRamBlockGreaterThanConfiguredLength(ramBlock, crcLength, blockLength) (((sizeof(ramBlock) - (crcLength)) > (blockLength)) ? -1 : 1)
/* Block Length Check and Strict Length Check enabled: error if the sizeof RAM block does not match the configured block length */
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
#define SizeOfRamBlockDoesNotMatchConfiguredLength(ramBlock, crcLength, blockLength) (((sizeof(ramBlock) - (crcLength)) != (blockLength)) ? -1 : 1)
/* Block Length Check enabled and Strict Length Check disabled: error if the sizeof RAM block is less than the configured block length */
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
#define SizeOfRamBlockLessThanConfiguredLength(ramBlock, crcLength, blockLength) (((sizeof(ramBlock) - (crcLength)) < (blockLength)) ? -1 : 1)

/* RAM block length checks */



/* Block Length Check and Automatic Block Length enabled: error if sizeof ROM block is less than sizeof RAM block */
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
#define SizeOfRomBlockLessThanSizeOfRamBlock(romBlock, ramBlock) ((sizeof(romBlock) < sizeof(ramBlock)) ? -1 : 1)
/* Block Length Check and Strict Length Check enabled: error if the sizeof ROM block does not match the configured block length */
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
#define SizeOfRomBlockDoesNotMatchConfiguredLength(romBlock, blockLength) ((sizeof(romBlock) != (blockLength)) ? -1 : 1)
/* Block Length Check enabled, Strict Length disabled: error if the sizeof ROM block is less than the configured block length */
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
#define SizeOfRomBlockLessThanConfiguredLength(romBlock, blockLength) ((sizeof(romBlock) < (blockLength)) ? -1 : 1)

/* ROM block length checks */



/* Permanent RAM and ROM block length checks - END */

#define NVM_STOP_SEC_CONST_DESCRIPTOR_TABLE
#include "MemMap.h"

#define NVM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"

#if(NVM_API_CONFIG_CLASS != NVM_API_CONFIG_CLASS_1)
/* Job Queue used for normal and high prio jobs */
VAR(NvM_QueueEntryType, NVM_PRIVATE_DATA) NvM_JobQueue_at[NVM_SIZE_STANDARD_JOB_QUEUE + NVM_SIZE_IMMEDIATE_JOB_QUEUE];
#endif

#define NVM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"

#define NVM_START_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "MemMap.h"

/* array of RAM Block Management areas, defined to be usable in block descriptor table */
VAR(NvM_RamMngmtAreaType, NVM_CONFIG_DATA) NvM_BlockMngmtArea_at[NVM_TOTAL_NUM_OF_NVRAM_BLOCKS];

/* management area for DCM blocks */
VAR(NvM_RamMngmtAreaType, NVM_CONFIG_DATA) NvM_DcmBlockMngmt_t;

#define NVM_STOP_SEC_VAR_POWER_ON_INIT_UNSPECIFIED
#include "MemMap.h"

#define NVM_START_SEC_CODE
#include "MemMap.h"

/**********************************************************************************************************************
*  NvM_EnterCriticalSection
**********************************************************************************************************************/
/*!
 * \internal
 *  - #10 enter SchM exclusive area for NvM 
 * \endinternal
 */
FUNC(void, NVM_PRIVATE_CODE) NvM_EnterCriticalSection(void)
{
    /* do what ever was defined to do for locking the resources */
    SchM_Enter_NvM_NVM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
}

/**********************************************************************************************************************
*  NvM_ExitCriticalSection
**********************************************************************************************************************/
/*!
 * \internal
 *  - #10 exit SchM exclusive area for NvM 
 * \endinternal
 */
FUNC(void, NVM_PRIVATE_CODE) NvM_ExitCriticalSection(void)
{
    /* do what ever was defined to do for unlocking the resources */
    SchM_Exit_NvM_NVM_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
}

/* PRQA S 3453 2 */ /* MD_MSR_19.7 */ 
#define NvM_invokeMultiBlockMode(serv, res) BswM_NvM_CurrentJobMode((serv),(res)) /*  if NvMBswMMultiBlockJobStatusInformation is TRUE  */ 
#define NvM_invokeMultiCbk(serv, res)  

/**********************************************************************************************************************
*  NvM_MultiBlockCbk
**********************************************************************************************************************/
/*!
 * \internal
 *  - #10 invoke the BSWM notification if any is configured
 *  - #20 in case the given job result isn't set to pending, invoke the multi block job end notification
 * \endinternal
 */
FUNC(void, NVM_PRIVATE_CODE) NvM_MultiBlockCbk(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
    /* The complete function body is designed to be optimized away by the compiler, if it is not needed    *
     * If the used macro is empty, the compiler may decide to remove code because it would contain         *
     * emtpy execution blocks (it does not matter whether conditions were TRUE or FALSE                    */
    NvM_invokeMultiBlockMode(ServiceId, JobResult);

    if(JobResult != NVM_REQ_PENDING)
    {
        NvM_invokeMultiCbk(ServiceId, JobResult);
    }   
}

 /* PRQA S 3453 2 */ /* MD_MSR_19.7 */ 
#define NvM_invokeCbk(descr, serv, res) (descr)->CallbackFunc_pt((serv), (res)) /*  if any Block has a Single Block Job End Notifications  */ 
#define NvM_invokeCurrentBlockMode(id, res) 

/**********************************************************************************************************************
*  NvM_BlockNotification
**********************************************************************************************************************/
/*!
 * \internal
 *  - #100 only for normal blocks (no DCM blocks)
 *      - #110 only when
 *        single block callback configured and
 *        the job is done (result != NVM_REQ_PENDING) and
 *        current job is not WriteAll (do not invoke any callbacks during WriteAll) and
 *        for ReadAll as current job the callback invoking is enabled
 *      - #111 invoke the single block callback
 *  - #120 block has enabled BswM callback
 *      - #121 invoke BswM callback
 * \endinternal
 */
void NvM_BlockNotification(NvM_BlockIdType BlockId, NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
    /* The complete function body is designed to be optimized away by the compiler, if it is not needed    *
     * If the used macros are empty, the compiler may decide to remove code because it would contain       *
     * empty execution blocks (it does not matter whether conditions were TRUE or FALSE                    */
    const NvM_BlockIdType orgBlockId = NVM_BLOCK_FROM_DCM_ID(BlockId);
    const NvM_BlockDescrPtrType blockDescriptorPtr = &NvM_BlockDescriptorTable_at[orgBlockId];
    /* #100 only for normal blocks (no DCM blocks) */
    if (orgBlockId == BlockId)
    {
        /* #110 only when
         * single block callback configured and
         * the job is done (result != NVM_REQ_PENDING) and
         * current job is not WriteAll (do not invoke any callbacks during WriteAll) and
         * for ReadAll as current job the callback invoking is enabled */
        if((blockDescriptorPtr->CallbackFunc_pt != NULL_PTR) && (JobResult != NVM_REQ_PENDING) &&
            (ServiceId != NVM_WRITE_ALL) &&
            (!((ServiceId == NVM_READ_ALL) && ((blockDescriptorPtr->Flags_u8 & NVM_CBK_DURING_READALL_ON) != NVM_CBK_DURING_READALL_ON))))
        {
            /* #111 invoke the single block callback */
            NvM_invokeCbk(blockDescriptorPtr, ServiceId, JobResult); /* SBSW_NvM_FuncPtrCall_UserCallbacks */
        }
        /* #120 block has enabled BswM callback */
        /* PRQA S 3760 1 */ /* MD_NvM_10.1 */
        if(blockDescriptorPtr->NotifyBswM == TRUE)
        {
            /* #121 invoke BswM callback */
            NvM_invokeCurrentBlockMode(BlockId, JobResult);
        }
    }      
}

#define NVM_STOP_SEC_CODE
#include "MemMap.h"

/* PRQA L:inclusions */

/* ---- End of File ------------------------------------------------------- */

