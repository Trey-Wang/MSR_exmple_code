/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Eth_30_Vtt_Types.h
 *        \brief  Public header file of VTT Ethernet driver for data type access
 *
 *      \details  Contains data type declaration that shall be accessible in a public manner.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/
#if !defined( ETH_30_VTT_TYPES_H )
# define      ETH_30_VTT_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "Eth_GeneralTypes.h"
# include "Eth_30_Vtt_Cfg.h"

# if (ETH_30_VTT_ENABLE_MAC_WRITE_ACCESS == STD_ON)
#  include "NvM.h"
# endif /* ETH_30_VTT_ENABLE_MAC_WRITE_ACCESS */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  PUBLIC DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef struct
{
  P2CONST(void, AUTOMATIC, AUTOMATIC) DerivativeConfigPtr;
} Eth_ConfigType;

/* Structure holding data needed for register initialization. */
typedef struct
{
  uint32 RegValue;  /* Register value */
  uint16 RegOffset; /* Offset relative to register base address */
} Eth_30_Vtt_RegDataType;

/* Structure summarizing configuration data in multiple controller use-case. */
typedef struct
{
  CONSTP2CONST(Eth_30_Vtt_RegDataType, ETH_30_VTT_CONST, ETH_30_VTT_CONST) RegVals; /*!<< Data for register configuration */
               uint8                                                          RegNum; /*!<< Number of registers to configure */
               uint8                                                          Features; /*!<< Features enabled on controller (Bitfield) */
               uint8                                                          RxDescriptorRingNum; /*!<< Number of Rx descriptor rings utilized */
               uint8                                                          TxDescriptorRingNum; /*!<< Number of Tx descriptor rings utilized */
  CONSTP2CONST(uint32, ETH_30_VTT_CONST, ETH_30_VTT_CONST)                  TxBufferStart; /*!<< TxBufferStart Array */
  CONSTP2CONST(uint16, ETH_30_VTT_CONST, ETH_30_VTT_CONST)                  TxBufferLen; /*!<< TxBufferLen Array */
} Eth_30_Vtt_CtrlCfgData;

#if (ETH_30_VTT_ENABLE_MAC_WRITE_ACCESS == STD_ON)
typedef struct
{
   boolean          EthIsMacAddrWriteAccess;
   NvM_BlockIdType  EthNvMBlockDesc;
} Eth_30_Vtt_CtrlMacAddrBlockIdType;
#endif /* ETH_30_VTT_ENABLE_MAC_WRITE_ACCESS */

#endif /* ETH_30_VTT_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: Eth_30_Vtt_Types.h
 *********************************************************************************************************************/

