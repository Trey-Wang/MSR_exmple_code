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
/**      \file    VTTCntrl_Fr.h
*        \brief   Declares the public API of the virtual Ethernet controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual Ethernet controller that enables to send and receive Ethernet frames.
*
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Max-Ferdinand Suffel           vismsl       Vector Informatik GmbH
 *  Damian Philipp                 visdap       Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  04.00.00  2015-11-03  visdap  -             Updated header comment.
 *  04.00.01  2016-01-20  vismsl  ESCAN00087533 Cleanup of VttCntrl codebase: Refactoring and additional documentation.
 *  04.01.00  2016-06-23  visfer  -             Added frame length checks - compability with R16 DrvEth_Vtt
 *  04.02.00  2016-11-22  visfer  ESCAN00092941 Introduce CANoe system variable to control link state for component test
*********************************************************************************************************************/

#if !defined (VTTCNTRL_ETH_H)
#define VTTCNTRL_ETH_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

#include "VttCntrl_Eth_Cfg.h"
#include "Eth_GeneralTypes.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Component version information ----- */
#define VTTETH_SW_MAJOR_VERSION (4u)
#define VTTETH_SW_MINOR_VERSION (3u)
#define VTTETH_SW_PATCH_VERSION (0u)
 
/*! \brief Offset of IRQ numbers in os.h for Ethernet controller interrupts. */
#define ETH_IRQ_OFFSET                IRQ_EthIsrRx_0

/*! \brief Size of Ethernet frame buffer. */
#define VTTETH_FRAME_BUFFER_SIZE      1520

/*! \brief Possible return values for VttEth_GetState. */
#define VTTETH_STATE_UNKNOWN          (0u)
#define VTTETH_STATE_NOT_CONNECTED    (1u)
#define VTTETH_STATE_CONNECTED        (2u)
#define VTTETH_STATE_CHANNEL_UNKNOWN  (3u)

/*! \brief The same packet handler is triggered for Tx and Rx frames. Distinguished by direction variable. */
#define ETH_DIRECTION_RX              0
#define ETH_DIRECTION_TX              1

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL DATA TYPES AND STRUCTURES
*********************************************************************************************************************/
 
 /*! \brief Internal Ethernet controller structure. */
typedef struct
{
  uint8 Dir;
  uint32 PacketSize;
  uint8 PacketData[VTTETH_FRAME_BUFFER_SIZE];
  Eth_ModeType CtrlMode;
  sint32 IsrTx_No;
  sint32 IsrRx_No;
  uint32 Status;
} VttEth_CntrlType;

/*! \brief Ethernet Rx descriptor structure. */
typedef struct {
  Eth_DataType* DataPtr;
  uint16 AllowedPacketSize;
  uint16 ReceivedPacketSize;
  uint8 SoftwareOwned;
  uint8 LastElement;
} VttEth_RxDescriptor;

/*! \brief Ethernet Tx descriptor structure. */
typedef struct {
  uint8 SoftwareOwned;
  uint8 LastElement;
} VttEth_TxDescriptor;

/*! \brief Internal controller structure associated with each Ethernet channel. */
extern VttEth_CntrlType VttEth_Cntrl[VTTETH_CHANNELS];

/**********************************************************************************************************************
*  GLOBAL DATA PROTOTYPES
*********************************************************************************************************************/
extern uint32 VttEth_SysVar_TrcvLinkState;

/**********************************************************************************************************************
*  GLOBAL FUNCTION PROTOTYPES
*********************************************************************************************************************/

void VttEth_SetControllerMode(uint8 channel, Eth_ModeType CtrlMode);
void VttEth_GetControllerMode(uint8 channel, Eth_ModeType* CtrlModePtr);
void VttEth_SetAllChannelsOnDownMode(void);
void VttEth_SetDescriptorBaseAddress(VttEth_RxDescriptor* RxDescriptorBaseAddress, VttEth_TxDescriptor* TxDescriptorBaseAddress);

void VttEth_OutputEthernetPacket(uint8 channel, uint32 packetSize, const uint8 *packetData);
uint32 VttEth_GetStatus(uint8 channel);
VttEth_CntrlType* VttEth_GetData(uint8 channel);

void VttEth_OnStateChange(uint8 action, uint8 oldState, uint8 newState);

#endif  /* VTTCNTRL_ETH_H */

/**********************************************************************************************************************
 *  END OF FILE: VttCntrl_Eth.h
 *********************************************************************************************************************/
