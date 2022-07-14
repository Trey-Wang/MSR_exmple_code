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
 *            Module: VTTCntrl
 *           Program: 
 *          Customer: N/A
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: 
 *    License Scope : The usage is restricted to CBD0000000_D80
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: VttCntrl_SysVar_Cfg.h
 *   Generation Time: 2020-03-02 16:11:30
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

# define VTTSYSVAR_CFG

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/
# include "VttCntrl_SysVar.h"

/**********************************************************************************************************************
*  VERSION CHECK
*********************************************************************************************************************/
 /* Component Version Information */
# define VTTSYSVAR_CFG_MAJOR_VERSION    (0x13u) 
# define VTTSYSVAR_CFG_MINOR_VERSION    (0x00u) 
# define VTTSYSVAR_CFG_PATCH_VERSION    (0x01u)

/**********************************************************************************************************************
*  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

#define VTTSYSVAR_CHANNELS	(3)

/* SysVar Types */
#define VTTSYSVAR_TYPE_ID_uint64  (3U)

/* Access methods for all types */
Std_ReturnType VttSysVar_Read_uint64(uint32 sysVar, uint64 * value);
Std_ReturnType VttSysVar_Write_uint64(uint32 sysVar, uint64 value);

/* SysVar Names */
#define VttCntrlConf_VttCntrl_SysVar_Display_LightOnOffCounter  (0U)
#define VttCntrlConf_VttCntrl_SysVar_Display_Odometer  (1U)
#define VttCntrlConf_VttCntrl_SysVar_Display_OdometerWriteReqPending  (2U)


/**********************************************************************************************************************
*  END OF FILE: VttCntrl_SysVar_Cfg.h
*********************************************************************************************************************/


