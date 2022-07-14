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
 *            Module: Fee
 *           Program: 
 *          Customer: N/A
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: 
 *    License Scope : The usage is restricted to CBD0000000_D80
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: Fee_Cfg.h
 *   Generation Time: 2020-03-02 16:11:30
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

    
/**********************************************************************************************************************
 *  PUBLIC SECTION
 *********************************************************************************************************************/
#if !defined (FEE_CFG_H_PUBLIC)
# define FEE_CFG_H_PUBLIC

  /********************************************************************************************************************
   *  GLOBAL CONSTANT MACROS
   *******************************************************************************************************************/
  /****************************************************************************
   * VERSION IDENTIFICATION
   ***************************************************************************/
# define FEE_CFG_MAJOR_VERSION                    (8u)
# define FEE_CFG_MINOR_VERSION                    (1u)
# define FEE_CFG_PATCH_VERSION                    (1u)

  /****************************************************************************
   * API CONFIGURATION
   ***************************************************************************/
# define FEE_VERSION_INFO_API                     (STD_ON)
# define FEE_GET_ERASE_CYCLE_API                  (STD_OFF)
# define FEE_GET_WRITE_CYCLE_API                  (STD_OFF)
# define FEE_FORCE_SECTOR_SWITCH_API              (STD_OFF)
# define FEE_FSS_CONTROL_API                   	  (STD_OFF)

# define FEE_DATA_CONVERSION_API                  (STD_OFF)

  /****************************************************************************
   * DEVELOPMENT CONFIGURATION
   ***************************************************************************/
# define FEE_DEV_ERROR_DETECT                     (STD_ON)

# define FEE_DEBUG_REPORTING                      (STD_ON)

  /****************************************************************************
   * GENERAL CONFIGURATION PARAMETER
   ***************************************************************************/

#define FeeConf_FeeBlockConfiguration_FeeConfigBlock (208UL) 
#define FeeConf_FeeBlockConfiguration_FeeDemAdminDataBlock (48UL) 
#define FeeConf_FeeBlockConfiguration_FeeDemPrimaryDataBlock0 (64UL) 
#define FeeConf_FeeBlockConfiguration_FeeDemPrimaryDataBlock1 (80UL) 
#define FeeConf_FeeBlockConfiguration_FeeDemPrimaryDataBlock2 (96UL) 
#define FeeConf_FeeBlockConfiguration_FeeDemPrimaryDataBlock3 (112UL) 
#define FeeConf_FeeBlockConfiguration_FeeDemPrimaryDataBlock4 (128UL) 
#define FeeConf_FeeBlockConfiguration_FeeDemPrimaryDataBlock5 (144UL) 
#define FeeConf_FeeBlockConfiguration_FeeDemPrimaryDataBlock6 (160UL) 
#define FeeConf_FeeBlockConfiguration_FeeDemPrimaryDataBlock7 (176UL) 
#define FeeConf_FeeBlockConfiguration_FeeDemStatusDataBlock (192UL) 
#define FeeConf_FeeBlockConfiguration_FeeCpApMySwcSnNvMPimOdometer (32UL) 
#define FeeConf_FeeBlockConfiguration_FeeCpApMySwcSnNvMPimLightOnOffCounter (16UL) 


#define FeePartitionConfiguration (0U) 


#define FEE_NUMBER_OF_PARTITIONS (1)

#endif /* FEE_CFG_H_PUBLIC */

/**********************************************************************************************************************
 *  END OF FILE: Fee_Cfg.h
 *********************************************************************************************************************/
 

