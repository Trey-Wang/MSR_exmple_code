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
 *              File: VttCntrl_Can_Cfg.h
 *   Generation Time: 2020-03-02 16:11:30
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/



#if !defined (VTTCNTRL_CAN_CFG_H)
# define VTTCNTRL_CAN_CFG_H

/**********************************************************************************************************************
 * VERSION
 *********************************************************************************************************************/
/* This version is used by the VttCan generator to make sure both components are compatible. */
#define VTTCNTRL_GENERATORMSR_DRVCAN_COMPATIBILITY_VERSION 0x0010U

/* Component Version Information */
# define VTTCAN_CFG_SW_MAJOR_VERSION   (0x14)
# define VTTCAN_CFG_SW_MINOR_VERSION   (0x00)
# define VTTCAN_CFG_SW_PATCH_VERSION   (0x01)

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

#define VTTCAN_CHANNELS (1) /* Number of CAN Channels configured in CAN Driver. */

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
extern const char* VttCan_BusName[VTTCAN_CHANNELS];

#endif  /* VTTCNTRL_CAN_CFG_H */





