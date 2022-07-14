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
 *         File:  J1939Nm_Types.h
 *       Module:  J1939 Network Management
 *
 *  Description:  Global types header of the SAE J1939 Network Management module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (J1939NM_TYPES_H)
# define J1939NM_TYPES_H


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "ComStack_Types.h" /* !< \trace SPEC-2168255 */
# include "NmStack_Types.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/*! \brief Size of NAME. */
# define J1939NM_NAME_SIZE                      (8)
# define J1939NM_NM_QUEUE_DATA_SIZE             (10u)

/*! \brief Size of bit field for all possible addresses. */
# define J1939NM_BUSADDRESSSTATE_SIZE           (32)


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*! \brief State of channels. */
typedef enum
{
  J1939NM_NETWORKSTATE_DOWN = 0,
  J1939NM_NETWORKSTATE_SCANNING = 1,
  J1939NM_NETWORKSTATE_REFRESH = 2,
  J1939NM_NETWORKSTATE_STABLE = 3
} J1939Nm_NetworkStateType;

/*! \brief Sub state of internal nodes. */
typedef enum
{
  J1939NM_NODESTATE_BUS_SLEEP = 0,
  J1939NM_NODESTATE_WAIT_NETWORK = 1,
  J1939NM_NODESTATE_PRE_CLAIMING = 2,
  J1939NM_NODESTATE_CLAIMING = 3,
  J1939NM_NODESTATE_PRE_NORMAL = 4,
  J1939NM_NODESTATE_NORMAL = 5,
  J1939NM_NODESTATE_ACLOST = 6
} J1939Nm_NodeStateType;

/*! \brief State of external nodes. */
typedef enum
{
  J1939NM_EXTNODESTATE_UNKNOWN = 0,
  J1939NM_EXTNODESTATE_CLAIMED = 1,
  J1939NM_EXTNODESTATE_CANNOT_CLAIM = 2
} J1939Nm_ExtNodeStateType;

/*! \brief State of internal node per channel. */
typedef struct
{
  uint16 Timer;
  J1939Nm_NodeStateType NodeState;
  boolean AcPending;
  boolean AcDelayed;
  boolean StatePending;
} J1939Nm_NodeChannelStateType;

/*! \brief State of external node per channel. */
typedef struct
{
  J1939Nm_ExtNodeStateType ExtNodeState;
  boolean Confirmed;
} J1939Nm_ExternalNodeChannelStateType;

/*! \brief State of channel. */
typedef struct
{
  uint8 BusAddressState[J1939NM_BUSADDRESSSTATE_SIZE];
  J1939Nm_NetworkStateType NetworkState;
  boolean BusOff;
  uint16 Timer;
  uint16 NmQueueWrite;
  uint16 NmQueueRead;
  Nm_ModeType NmMode;
  Nm_StateType NmState;
  uint8 NmQueueFree;
} J1939Nm_ChannelStateType;

/*! \brief State of Tx PDUs. */
typedef struct
{
  uint16 ConfTimeout;
  uint8 NodeId;
  boolean Locked;
} J1939Nm_TxPduStateType;

/*! \brief Element of J1939Nm Name Management queue. */
typedef struct
{
  uint8 Data[J1939NM_NM_QUEUE_DATA_SIZE];
  uint8 ChannelId;
  uint8 NodeId;
} J1939Nm_NmQueueElementType;

/*! \brief Type for NAME variables. */
typedef uint8 J1939Nm_NameType[J1939NM_NAME_SIZE];

/*! \brief Type for NAME constants. */
typedef const uint8 J1939Nm_ConstNameType[J1939NM_NAME_SIZE];

/**********************************************************************************************************************
 *  User_AddressClaimedIndication
 *********************************************************************************************************************/
/*! \brief      Provides the content of all received and all transmitted AddressClaimed (AC) messages.
 *
 *  \param      channel         Channel on which the AC was received or transmitted.
 *  \param      sourceAddress   Claimed address.
 *  \param      name            NAME of the node that sent the AC.
 *  \return     none
 *  \context    May be called from interrupt or task level.
 *
 *  \trace SPEC-2168307
 *********************************************************************************************************************/
typedef P2FUNC(void, J1939NM_APPL_CODE, J1939Nm_UserCalloutPtrType) (NetworkHandleType channel, uint8 sourceAddress,
                                                                     P2CONST(uint8, AUTOMATIC, J1939NM_APPL_DATA) name);

#endif /* J1939NM_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: J1939Nm_Types.h
 *********************************************************************************************************************/
