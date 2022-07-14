/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 Vector Informatik GmbH.                                                     All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Mirror_Types.h
 *       Module:  Gw_Mirror
 *
 *  Description:  Global types header of the Mirror component.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (MIRROR_TYPES_H)
# define MIRROR_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComStack_Types.h"
# include "PduR_Mirror.h"
# include "ComM.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define MIRROR_INACTIVE                    (Mirror_ModeType) 0x00u
# define MIRROR_ACTIVE                      (Mirror_ModeType) 0x01u

# define MIRROR_SOURCECHANNELCAN_NUMBER_OF_FILTERS              2u

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**!< Enable/Disable the mirroring functionality. */
typedef uint8 Mirror_ModeType;

typedef uint8 *Mirror_DataPtrType;

typedef union uId
{                               /* PRQA S 0750 */ /*   MD_MSR_18.4 */
  uint32 id32;
  uint16 id16[2];
  uint8 id8[4];
} Mirror_IdType;

typedef struct sMirror_MsgInfo
{
  Mirror_IdType msgId;          /* max 4 Bytes; looks different for each busType */
  uint16 payloadLen;
  uint8 busType;
  uint8 busId;
  uint8 state;                  /* optional */
  Mirror_DataPtrType payload;
} Mirror_MsgInfoType;

typedef struct sMirror_FilterState
{
  boolean isFilterActive[MIRROR_SOURCECHANNELCAN_NUMBER_OF_FILTERS];
  struct
  {
    Mirror_IdType filterValueLowOrId;
    Mirror_IdType filterValueHighOrMask;
  } filter[MIRROR_SOURCECHANNELCAN_NUMBER_OF_FILTERS];
} Mirror_FilterStateType;

typedef struct sMirror_Info
{
  NetworkHandleType networkId;
  Mirror_ModeType activeMode;
  Mirror_FilterStateType range;
} Mirror_InfoType;

typedef struct sMirror_DestChannelStateType
{
  uint32 writeTimeStamp;
  uint32 startTimeStamp;
  uint16 queueWriteIdx;
  uint16 queueReadIdx;
  uint16 queueFreeElements;
  uint16 queueElementWriteIdx;
} Mirror_DestChannelStateType;

typedef struct sMirror_CanFilterType
{
  uint32 filterValueLowOrId;
  uint32 filterValueHighOrMask;
} Mirror_CanFilterType;

typedef enum eMirror_ChannelType
{
  MIRROR_CHANNEL_TYPE_CAN = 0,
  MIRROR_CHANNEL_TYPE_IP,
  MIRROR_CHANNEL_TYPE_ERROR
} Mirror_ChannelType;

typedef P2FUNC(uint32, MIRROR_CODE, Mirror_UserCalloutPtrType) (void);

#endif
