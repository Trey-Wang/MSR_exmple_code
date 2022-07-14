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
/**      \file    VTTCntrl_Can.h
*        \brief   Declares the public API of the virtual CAN controller integrated in the vVIRTUALtarget.
*
*      \details   Virtual CAN controller that enables to send and receive CAN frames via different CAN channels.
*
*********************************************************************************************************************/

/**********************************************************************************************************************
*  AUTHOR IDENTITY
*  -------------------------------------------------------------------------------------------------------------------
*  Name                           Initials      Company
*  -------------------------------------------------------------------------------------------------------------------
*  Max-Ferdinand Suffel           vismsl        Vector Informatik GmbH
*  Damian Philipp                 visdap        Vector Informatik GmbH
*  -------------------------------------------------------------------------------------------------------------------
*  REVISION HISTORY
*  -------------------------------------------------------------------------------------------------------------------
*  Version   Date        Author  Change Id      Description
*  -------------------------------------------------------------------------------------------------------------------
*  03.01.00  2015-11-03  visdap  -              Initial version extracted from VTTCntrl generator.
*  03.01.01  2015-12-03  vismsl  ESCAN00086855  Added support for CAN Polling.
*  03.01.02  2016-01-18  vismsl  ESCAN00087533  Cleanup of VttCntrl codebase: Refactoring and additional documentation.
*  08.00.01  2017-04-12  visjth  ESCAN00094762: Use functions VttCntrl_Base_MaskInterrupt VttCntrl_Base_UnmaskInterrupt
*                                               for masking and unmasking of interrupts.
*********************************************************************************************************************/

#if !defined (VTTCNTRL_CAN_H)
#define VTTCNTRL_CAN_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

#include "VttCntrl_Base.h"
#include "VttCntrl_Can_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Component version information ----- */
#define VTTCAN_SW_MAJOR_VERSION  (3u)
#define VTTCAN_SW_MINOR_VERSION  (1u)
#define VTTCAN_SW_PATCH_VERSION  (2u)

/* ----- Error codes ----- */
/*! \brief Notify error state Active is reached. */
#define VTTCAN_ERR_ACTIVE         CANOEAPI_CANERRORACTIVE
/*! \brief Notify error state Passive is reached. */
#define VTTCAN_ERR_PASSIVE        CANOEAPI_CANERRORPASSIVE
/*! \brief Notify error state Warning is reached. */
#define VTTCAN_ERR_WARNINGLIMIT   CANOEAPI_CANWARNINGLIMIT  
/*! \brief Notify error state BusOff is reached. */
#define VTTCAN_ERR_BUSOFF         CANOEAPI_CANBUSOFF          

/* ----- Virtual CAN controller modes ----- */
/*! \brief To Initialize CAN controller (called while start-up and re-initialization), leave in state STOP. */
#define VTTCAN_MODE_INIT          (0x00)
/*! \brief Put controller to state STOP (no Rx/Tx and bus-acknowledge, all interrupt sources are deactivated). */
#define VTTCAN_MODE_STOP          (0x01)
/*! \brief Put controller to state START (full bus communication, Rx/Tx/Error-interrupts are active). */
#define VTTCAN_MODE_START         (0x02)
/*! \brief Put controller to state SLEEP (like STOP but the wakeup interrupt will be triggered next Rx). */
#define VTTCAN_MODE_SLEEP         (0x03)
/*! \brief Put controller to state WAKEUP (like STOP mode but out of SLEEP will deactivate wakeup interrupt). */
#define VTTCAN_MODE_WAKEUP        (0x04)   

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#ifdef VTTCAN_CFG_VARIANT_CHANNEL_MAPPING
typedef struct {
  char* variant;
  int number_of_channels;
  char* bus_names[VTTCAN_CHANNELS];
} VttCan_ChannelsStruct;
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

void VttCan_SetCanConfiguration(uint8 channelId,
  double arbBaudrate, uint8 arbTseg1, uint8 arbTseg2,
  uint8 arbSjw, uint8 arbSam, uint32 arbFlags,
  double dbrBaudrate, uint8 dbrTseg1, uint8 dbrTseg2,
  uint8 dbrSjw, uint8 dbrSam, uint32 dbrFlags);

void VttCan_SetCanBtr(uint8 channelId, uint8 btr0, uint8 btr1);
void VttCan_SetMode(uint8 channelId, uint8 mode);
void VttCan_EnableISR(uint8 VttChannel, boolean enableTx, boolean enableRx, boolean enableError, boolean enableWakeup);

void VttCan_SendMessage(uint8 channelId, uint32 id, uint8 dlc, uint8 data[], uint32 flags);
void VttCan_SendCanMessage(uint8 channelId, 
  uint32 id, uint32 flags, uint8 txReqCount, uint8 dataLength, const uint8* data);

void VttCan_AtomicBegin(void);
void VttCan_AtomicEnd(void);
void VttCan_OnStateChange(uint8 action, uint8 oldState, uint8 newState);

#ifdef VTTCAN_CFG_VARIANT_CHANNEL_MAPPING
VttCan_ChannelsStruct *getVttCanCfgPtr(void);
#endif

#endif  /* VTTCNTRL_CAN_H */

/**********************************************************************************************************************
 *  END OF FILE: VTTCntrl_Can.h
 *********************************************************************************************************************/
