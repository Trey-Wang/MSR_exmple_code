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
/**        \file  Etm_Types.h
 *        \brief  Etm header file
 *
 *      \details  Implementation of the Etm (Ethernet Testability Module) module. This module implements the
 *                AUTOSAR Testability Protocol and Service Primitives protocol for interaction with external testers
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

#if !defined (ETM_TYPES_H)
# define ETM_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

# include "TcpIp_Types.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/****************************************************************************************************
 *  ENUMS
 ****************************************************************************************************/

typedef enum
{
  ETM_CMD_CHAN_INACTIVE      = 0,  /* CHANNEL IS DEACTIVATED AND IS NOT USED !!! */
  ETM_CMD_CHAN_DO_GETSOCKET  = 1,  /* Call GetSocket next */
  ETM_CMD_CHAN_DO_BIND       = 2,  /* Call Bind next */
  ETM_CMD_CHAN_DO_LISTEN     = 3,  /* Call Listen next (TCP ONLY) */
  ETM_CMD_CHAN_DO_RX_BUFFER  = 4,  /* Call ChangeParameter next (TCP ONLY) */
  ETM_CMD_CHAN_DO_TX_BUFFER  = 5,  /* Call ChangeParameter next (TCP ONLY) */
  ETM_CMD_CHAN_ACTIVE        = 6   /* CHANNEL IS READY TO USE */
} Etm_CmdChanStateType;

/****************************************************************************************************
 *  TYPES
 ****************************************************************************************************/

typedef uint8 *Etm_ConfigType;

typedef struct
{
  TcpIp_SocketIdType SocketId;
  Etm_CmdChanStateType ChannelState;
} Etm_CommandSocketType;

typedef struct
{
  TcpIp_SocketIdType SocketId;
  boolean RecvFwdActive;
  uint32 RecvFwdDropCount;
  uint16 RecvFwdMaxForward;
  uint16 RecvFwdMaxLength;
} Etm_TestSocketType;

#endif /* ETM_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: Etm_Types.h
 *********************************************************************************************************************/
