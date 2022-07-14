/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2011-2017 by Vector Informatik GmbH.                                             All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  NmOsek.c
 *        \brief  NmOsek source file
 *
 *      \details  Vector Direct OSEK Network Management source file
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

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
#define NMOSEK_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "NmOsek.h"
#include "NmOsek_Cbk.h"

#include "Nm_Cbk.h"
#include "CanIf.h"
#if (NMOSEK_DEV_ERROR_DETECT == STD_ON) /* \trace CREQ-119848, CREQ-119849 */
# include "Det.h"
#endif

#if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
# include "CanSM.h"
#endif

#include "SchM_NmOsek.h"

#if (NMOSEK_WAKEUPTXUSERDATAPDUIDOFPBCHANNELCONFIG == STD_ON)
# include "PduR_NmOsek.h"
#endif

#if (NMOSEK_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
# include "EcuM_Error.h"
#endif

#if (NMOSEK_TX_DEADLINE_MONITORING_IN_COM_ENABLED == STD_ON)
# include "Com.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Vendor specific version information is decimal coded */
#if ((NMOSEK_SW_MAJOR_VERSION != (7u)) || \
     (NMOSEK_SW_MINOR_VERSION != (0u)) || \
     (NMOSEK_SW_PATCH_VERSION != (0u)))
# error "Vendor specific version numbers of NmOsek.c and NmOsek.h are inconsistent"
#endif

#if ((NMOSEK_CBK_MAJOR_VERSION != (7u)) || \
     (NMOSEK_CBK_MINOR_VERSION != (0u)) || \
     (NMOSEK_CBK_PATCH_VERSION != (0u)))
# error "Vendor specific version numbers of NmOsek.c and NmOsek_Cbk.h are inconsistent"
#endif

/* AUTOSAR version information check is not applicable since this is a Complex Device Driver */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Definition of channel parameter for local functions. */
/* Local functions do not use a channel parameter in single channel configurations. */
#if (NMOSEK_API_OPTIMIZATION == STD_ON)
/*! Channel parameter if there is only one argument */
# define NMOSEK_CHANNEL_PARA_ONLY

/*! Channel parameter as first argument if there is more than one argument */
# define NMOSEK_CHANNEL_PARA_FIRST

/*! Channel parameter of Appl callbacks if there is only one argument */
# define NMOSEK_CHANNEL_APPLPARA_ONLY

/*! Channel parameter of Appl callbacks if there is more than one argument */
# define NMOSEK_CHANNEL_APPLPARA_FIRST

/*! System channel index */
# define NMOSEK_SYSTEM_CHANNEL_IDX      (NmOsek_GetComMNetworkHandleRefOfChannelConfig(NMOSEK_CHANNEL_IDX))

/*! Channel index */
# define NMOSEK_CHANNEL_IDX             0u

/* The channel handle can be optimized in single channel configurations. */
# define NMOSEK_CHANNEL_NMTYPE_ONLY     void    /* PRQA S 3460 */ /* MD_MSR_19.4 */
# define NMOSEK_CHANNEL_NMTYPE_FIRST
#else
/*! Channel parameter if there is only one argument */
# define NMOSEK_CHANNEL_PARA_ONLY       NMOSEK_CHANNEL_IDX

/*! Channel parameter as first argument if there is more than one argument */
# define NMOSEK_CHANNEL_PARA_FIRST      NMOSEK_CHANNEL_IDX,

/*! Channel parameter of Appl callbacks if there is only one argument */
# define NMOSEK_CHANNEL_APPLPARA_ONLY   NMOSEK_NMTOSYSTEM_CHANNEL_IND

/*! Channel parameter of Appl callbacks if there is more than one argument */
# define NMOSEK_CHANNEL_APPLPARA_FIRST  NMOSEK_NMTOSYSTEM_CHANNEL_IND,

/*! System channel index */
# define NMOSEK_SYSTEM_CHANNEL_IDX      (nmChannelHandle)

# if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
/*! Channel index */
#  define NMOSEK_CHANNEL_IDX            channel
# else
/*! Channel index */
#  define NMOSEK_CHANNEL_IDX            nmChannelHandle
# endif

/*! Definition of channel parameter. */
# if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
#  define NMOSEK_CHANNEL_NMTYPE_ONLY    CONST(NetworkHandleType, AUTOMATIC) channel
#  define NMOSEK_CHANNEL_NMTYPE_FIRST   CONST(NetworkHandleType, AUTOMATIC) channel,
# else
#  define NMOSEK_CHANNEL_NMTYPE_ONLY    CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle
#  define NMOSEK_CHANNEL_NMTYPE_FIRST   CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
# endif
#endif

/*! NM to system channel indirection */
#define NMOSEK_NMTOSYSTEM_CHANNEL_IND  (NmOsek_GetComMNetworkHandleRefOfChannelConfig(NMOSEK_CHANNEL_IDX))

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/*! Macro definitions for the parameters of NmOsek_TransmitSkippedAlive(). The presence of these parameters depends on
 *  the configuration.
 *  \def NMOSEK_TRANSMIT_SKIPPED_ALIVE_CHANNEL_NMTYPE     first parameter of the NmOsek_TransmitSkippedAlive function
 *  \def NMOSEK_TRANSMIT_SKIPPED_ALIVE_REGULARTYPE        second parameter of the NmOsek_TransmitSkippedAlive function
 *
 *  \def NMOSEK_TRANSMIT_SKIPPED_ALIVE_CHANNEL_NMPARA     channel index for the first parameter (if required)
 *  \def NMOSEK_TRANSMIT_SKIPPED_ALIVE_REGULARPARA_FALSE  value for the second parameter: FALSE (if required)
 *  \def NMOSEK_TRANSMIT_SKIPPED_ALIVE_REGULARPARA_TRUE   value for the second parameter: TRUE (if required)
 */
/* ESCAN00054321 */
#  define NMOSEK_TRANSMIT_SKIPPED_ALIVE_CHANNEL_NMTYPE    NMOSEK_CHANNEL_NMTYPE_ONLY
#  define NMOSEK_TRANSMIT_SKIPPED_ALIVE_REGULARTYPE

#  define NMOSEK_TRANSMIT_SKIPPED_ALIVE_CHANNEL_NMPARA    NMOSEK_CHANNEL_PARA_ONLY
#  define NMOSEK_TRANSMIT_SKIPPED_ALIVE_REGULARPARA_FALSE
#  define NMOSEK_TRANSMIT_SKIPPED_ALIVE_REGULARPARA_TRUE
#endif

/*-- NM Message definitions -----------------------------------------------------------------------------------------*/
/* OpCode Definitions */
#if defined (NMOSEK_TYPE_VECTOR2_OSEK)
# define NMOSEK_ALIVE                       0x02u /*!< Alive */
# define NMOSEK_RING                        0x01u /*!< Ring */
# define NMOSEK_LIMPHOME                    0x00u /*!< LimpHome */
# define NMOSEK_OPCODE_RESERVED             0x00u /*!< Value of the reserved bits */

# define NMOSEK_SLEEP_IND                   0x04u /*!< Sleep.ind */
# define NMOSEK_SLEEP_ACK                   0x02u /*!< Sleep.ack */
# define NMOSEK_SLEEP_ACK_INV               0xFDu /*!< Sleep.ack not set, everything else set */

# define NMOSEK_CTRL_INIT                   0x00u /*!< Dest ID and OpCode Initialization */
# define NMOSEK_DATA_INIT                   0x00u /*!< Data Byte Initialization */
/* \trace CREQ-119831 */
# define NMOSEK_OPCODE_RESERVED_MASK        0x00u /*!< Mask for the reserved bits of the OpCode Byte */
# define NMOSEK_OPCODE_CMD_MASK             0x03u /*!< Mask for the two first bits of the OpCode Byte */
# define NMOSEK_OPCODE_SLEEP_MASK           0x06u /*!< Mask with Sleep.ind = Sleep.ack = 1 in the OpCode Byte */
#else
/* \trace CREQ-119834, CREQ-119838 */
# define NMOSEK_ALIVE                       0x01u /*!< Alive */
# define NMOSEK_RING                        0x02u /*!< Ring */
# define NMOSEK_LIMPHOME                    0x04u /*!< LimpHome */
# define NMOSEK_OPCODE_RESERVED             0x00u /*!< Value of the reserved bits */

# define NMOSEK_SLEEP_IND                   0x10u /*!< Sleep.ind */
# define NMOSEK_SLEEP_ACK                   0x20u /*!< Sleep.ack */
# define NMOSEK_SLEEP_ACK_INV               0xDFu /*!< Sleep.ack not set, everything else set */

# define NMOSEK_CTRL_INIT                   0x00u /*!< Dest ID and OpCode Initialization */
# define NMOSEK_DATA_INIT                   0x00u /*!< Data Byte Initialization */
/* \trace CREQ-119831 */
# define NMOSEK_OPCODE_RESERVED_MASK        0x00u /*!< Mask for the reserved bits of the OpCode Byte */
# define NMOSEK_OPCODE_CMD_MASK             0x0Fu /*!< Mask for the four first bits of the OpCode Byte */
# define NMOSEK_OPCODE_SLEEP_MASK           0x30u /*!< Mask with Sleep.ind = Sleep.ack = 1 in the OpCode Byte */

#endif

/*-- Symbolic NmOsek_Action values ----------------------------------------------------------------------------------*/
/* NMOSEK_ACTION_...
 * These are the symbolic names of the states of the NM state machine for every channel.
 */

#define NMOSEK_NO_ACTION                              (0x00u) /*!< NmOsek_MainFunction() calls have no effect */
#define NMOSEK_ACTION_START                           (0x01u) /*!< NMReset */
#define NMOSEK_ACTION_WAIT_SENDTOKEN                  (0x02u) /*!< Token at own node, Ring message TX pending */
#define NMOSEK_ACTION_WAIT_RINGMSG                    (0x03u) /*!< Token at another node, waiting for Ring message */
#define NMOSEK_ACTION_LH_WAIT_LIMPHOME_TX             (0x04u) /*!< LimpHome mode: LimpHome message TX pending */
#define NMOSEK_ACTION_LH_PREPARE_SLEEP                (0x05u) /*!< NMLimpHomePrepSleep */
#define NMOSEK_ACTION_GO_BUSSLEEP                     (0x06u) /*!< NMTwbsNormal / NMTwbsLimpHome */
#define NMOSEK_ACTION_BUS_SLEEP                       (0x07u) /*!< NMBusSleep (and NMInit) */

#define NMOSEK_ACTION_REPEAT                (0x80u) /*!< State transition in same call of NmOsek_MainFunction() */
#define NMOSEK_ACTION_REPEAT_INV            (0x7Fu) /*!< State transition in same call of NmOsek_MainFunction() */

#define NMOSEK_ACTION_START_WITH_REPEAT               (0x81u) /*!< NMReset with repetition flag */
#define NMOSEK_ACTION_LH_PREPARE_SLEEP_WITH_REPEAT    (0x85u) /*!< NMLimpHomePrepSleep with repetition flag */

/*-- NmOsek_RxTxCnt masks -------------------------------------------------------------------------------------------*/
#define NMOSEK_RX_CNT_MASK                  (0x0Fu) /*!< Mask for RX counter in NmOsek_RxTxCnt variable */
#define NMOSEK_RX_CNT_INV_MASK              (0xF0u) /*!< Inverse Mask for RX counter in NmOsek_RxTxCnt variable */
#define NMOSEK_RX_CNT_STEP                  (0x01u) /*!< Incrementation step for RX counter in NmOsek_RxTxCnt */
#define NMOSEK_TX_CNT_MASK                  (0xF0u) /*!< Mask for TX counter in NmOsek_RxTxCnt variable */
#define NMOSEK_TX_CNT_INV_MASK              (0x0Fu) /*!< Inverse Mask for TX counter in NmOsek_RxTxCnt variable */
#define NMOSEK_TX_CNT_STEP                  (0x10u) /*!< Incrementation step for TX counter for NmOsek_RxTxCnt */

#if defined(NMOSEK_RX_CNT_MAX)
/* Note: NMOSEK_RX_CNT_MAX can be configured in the user configuration file */
#else
# define NMOSEK_RX_CNT_MAX                  (4u)    /*!< RX Limit until state change to NMOSEK_ACTION_START */
#endif

#if defined(NMOSEK_TX_CNT_MAX)
/* Note: NMOSEK_TX_CNT_MAX can be configured in the user configuration file */
#else
# define NMOSEK_TX_CNT_MAX                  (8u)    /*!< TX Limit until state change to NMOSEK_ACTION_START */
#endif

/*! \def NMOSEK_USER_DATA_OFFSET First user data byte */
/*! \def NMOSEK_USER_DATA_OFFSET     first byte of the user data bytes */
#   define NMOSEK_USER_DATA_OFFSET                2u

/*-- NmOsek_InternalState masks -------------------------------------------------------------------------------------*/

/* NmOsek_InternalState[NMOSEK_CHANNEL_IDX]: Global State - Lower Byte */
/*! bit 0: (uint8)(1 << 0) */
#define NMOSEK_INTERNAL_STATE_RING_STABLE_FLAG_MASK        0x0001u
/*! bit 0: (uint8)(~(1 << 0)) */
#define NMOSEK_INTERNAL_STATE_RING_STABLE_INV_MASK         0xFFFEu

/*! bit 1: (uint8)(1 << 1) */
#define NMOSEK_INTERNAL_STATE_BUS_ERROR_FLAG_MASK          0x0002u
/*! bit 1: (uint8)(~(1 << 1)) */
#define NMOSEK_INTERNAL_STATE_BUS_ERROR_INV_MASK           0xFFFDu

/*! bit 2: (uint8)(1 << 2) */
#define NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK             0x0004u
/*! bit 2: (uint8)(~(1 << 2)) */
#define NMOSEK_INTERNAL_STATE_ACTIVE_INV_MASK              0xFFFBu

/*! bit 3: (uint8)(1 << 3) */
#define NMOSEK_INTERNAL_STATE_LIMP_HOME_FLAG_MASK          0x0008u
/*! bit 3: (uint8)(~(1 << 3)) */
#define NMOSEK_INTERNAL_STATE_LIMP_HOME_INV_MASK           0xFFF7u

/*! bit 4: (uint8)(1 << 4) */
#define NMOSEK_INTERNAL_STATE_BUS_SLEEP_FLAG_MASK          0x0010u
/*! bit 4: (uint8)(~(1 << 4)) */
#define NMOSEK_INTERNAL_STATE_BUS_SLEEP_INV_MASK           0xFFEFu

/*! bit 5: (uint8)(1 << 5) */
#define NMOSEK_INTERNAL_STATE_WAIT_BUS_SLEEP_FLAG_MASK     0x0020u
/*! bit 5: (uint8)(~(1 << 5)) */
#define NMOSEK_INTERNAL_STATE_WAIT_BUS_SLEEP_INV_MASK      0xFFDFu

/*! bit 6: (uint8)(1 << 6) */
#define NMOSEK_INTERNAL_STATE_TX_RING_DATA_ALLOW_FLAG_MASK 0x0040u
/*! bit 6: (uint8)(~(1 << 6)) */
#define NMOSEK_INTERNAL_STATE_TX_RING_DATA_ALLOW_INV_MASK  0xFFBFu

/*! bit 7: (uint8)(1 << 7) */
#define NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK      0x0080u
/*! bit 7: (uint8)(~(1 << 7)) */
#define NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_INV_MASK       0xFF7Fu

/* NmOsek_InternalState[NMOSEK_CHANNEL_IDX]: Local State - Higher Byte */
/*! bit 0: (uint8)(1 << 0) */
#define NMOSEK_INTERNAL_STATE_NM_MSG_RECV_FLAG_MASK        0x0100u
/*! bit 0: (uint8)(~(1 << 0)) */
#define NMOSEK_INTERNAL_STATE_NM_MSG_RECV_INV_MASK         0xFEFFu

/*! bit 1: (uint8)(1 << 1) */
#define NMOSEK_INTERNAL_STATE_RING_MSG_RECV_FLAG_MASK      0x0200u
/*! bit 1: (uint8)(~(1 << 1)) */
#define NMOSEK_INTERNAL_STATE_RING_MSG_RECV_INV_MASK       0xFDFFu

/*! bit 2: (uint8)(1 << 2) */
#define NMOSEK_INTERNAL_STATE_IM_ADDRESSED_FLAG_MASK       0x0400u
/*! bit 2: (uint8)(~(1 << 2)) */
#define NMOSEK_INTERNAL_STATE_IM_ADDRESSED_INV_MASK        0xFBFFu

/*! bit 3: (uint8)(1 << 3) */
#define NMOSEK_INTERNAL_STATE_IM_SKIPPED_OVER_FLAG_MASK    0x0800u
/*! bit 3: (uint8)(~(1 << 3)) */
#define NMOSEK_INTERNAL_STATE_IM_SKIPPED_OVER_INV_MASK     0xF7FFu

/*! bit 4: (uint8)(1 << 4) */
#define NMOSEK_INTERNAL_STATE_NM_MSG_RECV_REMIND_FLAG_MASK 0x1000u
/*! bit 4: (uint8)(~(1 << 4)) */
#define NMOSEK_INTERNAL_STATE_NM_MSG_RECV_REMIND_INV_MASK  0xEFFFu

/*! bit 5: (uint8)(1 << 5) */
/* ESCAN00050218 */ /* Reserved for future usage */
/*! bit 5: (uint8)(~(1 << 5)) */
/* ESCAN00050218 */ /* Reserved for future usage */

/*! bit 6: (uint8)(1 << 6) */
#define NMOSEK_INTERNAL_STATE_RING_PRE_STABLE_FLAG_MASK    0x4000u
/*! bit 6: (uint8)(~(1 << 6)) */
#define NMOSEK_INTERNAL_STATE_RING_PRE_STABLE_INV_MASK     0xBFFFu

/*! bit 7: (uint8)(1 << 7) */
#define NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK         0x8000u
/*! bit 7: (uint8)(~(1 << 7)) */
#define NMOSEK_INTERNAL_STATE_PREP_SLEEP_INV_MASK          0x7FFFu

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_ON)
/*! NmOsek_InternalState value at initialization:
 *  NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK |
 *  NMOSEK_INTERNAL_STATE_TX_RING_DATA_ALLOW_FLAG_MASK |
 *  NMOSEK_INTERNAL_STATE_BUS_SLEEP_FLAG_MASK |
 *  NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK
 */ /* ESCAN00049912, ESCAN00066879 */
# define NMOSEK_INTERNAL_STATE_BYTES_INIT                  (uint16)(NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK | \
                                                                    NMOSEK_INTERNAL_STATE_TX_RING_DATA_ALLOW_FLAG_MASK | \
                                                                    NMOSEK_INTERNAL_STATE_BUS_SLEEP_FLAG_MASK | \
                                                                    NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK)
/* ESCAN00052621 */
#else
/*! NmOsek_GlobalState value at initialization:
 *  NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK |
 *  NMOSEK_INTERNAL_STATE_TX_RING_DATA_ALLOW_FLAG_MASK |
 *  NMOSEK_INTERNAL_STATE_BUS_SLEEP_FLAG_MASK |
 *  NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK
 */ /* ESCAN00049912, ESCAN00066879 */
# define NMOSEK_INTERNAL_STATE_BYTES_INIT                  (uint16)(NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK | \
                                                                    NMOSEK_INTERNAL_STATE_TX_RING_DATA_ALLOW_FLAG_MASK | \
                                                                    NMOSEK_INTERNAL_STATE_BUS_SLEEP_FLAG_MASK | \
                                                                    NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK | \
                                                                    NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK)
/* ESCAN00052621 */
#endif

/* Masks for multiple flags */

/*! NmOsek_InternalState value at NMOSEK_ACTION_START entry:
 *  Clear: RingStable (bit 0), BusError (bit 1), LimpHome (bit 3)
 *  Clear: BusSleep (bit 4), WaitBusSleep (bit 5)
 *  Keep: Active (bit 2), TxRingDataAllow (bit 6), BusSleepInd (bit 7)
 *  Clear: Higher Byte
 */ /* ESCAN00049912, ESCAN00052621 */
#define NMOSEK_INTERNAL_STATE_ACTION_START_INV_MASK        0x00C4u

/*! NmOsek_InternalState value executed before leaving the do-while loop in NmOsek_MainFunction():
 *  Clear the first four flags of the local state byte
 */ /* ESCAN00049912 */
#define NMOSEK_INTERNAL_STATE_NM_STATUS_RX_FLAGS_INV_MASK  0xF0FFu

/*! NmOsek_InternalState value on BusOff occurrence:
 *  Clear all flags except PrepSleep (bit 7) and RingPreStable (bit 6)
 */ /* ESCAN00049912 */
#define NMOSEK_INTERNAL_STATE_NM_STATUS_RX_FLAGS_AND_RECV_REMIND_RX_TX_LOCK_INV_MASK 0xC0FFu

#if (NMOSEK_LEAVE_LHONLY_ON_RX_INDICATION_ENABLED == STD_ON)
/*! NmOsek_LocalState value after NmOsek_TxConfirmation of LIMPHOME message:
 *  Clear the Nm Msg Recv flag and the Nm Msg Recv Remind flag
 */ /* ESCAN00049912 */
# define NMOSEK_INTERNAL_STATE_NM_STATUS_NM_MSG_RECV_FLAG_NM_MSG_RECV_REMIND_FLAG_INV_MASK 0xEEFFu
#endif

#if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
/*-- NmOsek_RemoteSleepState values ---------------------------------------------------------------------------------*/
/* First bit indicates if remote sleep indication or cancellation was notified. */
# define NMOSEK_REMOTE_SLEEP_STATE_NO_SLEEP       0u
# define NMOSEK_REMOTE_SLEEP_STATE_SLEEP_IND      1u
/* Second bit is to detect if all nodes are ready to sleep, i.e. no NM message with sleep indication 0 was received
 * during one ring cycle. */
# define NMOSEK_REMOTE_SLEEP_STATE_PREP_SLEEP     2u
#endif

#if (NMOSEK_USE_NM_NODE_LIST == STD_ON)
/*! This mask is used for calculation of the desired entry within the node configuration table */
# define NMOSEK_BYTE_OFFSET_MASK                  (0x07u)
#endif

/* ----- Modes ----- */
#define NMOSEK_UNINIT                             (0x00u)
#define NMOSEK_INIT                               (0x01u)

/*! DestId Position */
# define NMOSEK_CFG_PDU_DEST_ID_POSITION     0

/*! OpCode Position */
# define NMOSEK_CFG_PDU_OPCODE_POSITION      1

/*! Sleep Indication and Sleep Acknowledge Byte Position */
# define NMOSEK_CFG_PDU_SLEEP_POSITION       NMOSEK_CFG_PDU_OPCODE_POSITION

#if defined(NMOSEK_LOCAL)
#else
# define NMOSEK_LOCAL         static
#endif

#if defined(NMOSEK_LOCAL_INLINE)
#else
# define NMOSEK_LOCAL_INLINE  LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/*-- Interface to Det Helper Macro ----------------------------------------------------------------------------------*/

/*-- Macros for Testing, Setting and Clearing State Bits ------------------------------------------------------------*/

/*-- NmOsek_GetNodeConfiguration() ---------------------------------------------------------------------------------------*/
/*! \def NMOSEK_CALCULATE_CONFIG_BYTE(i)
 *  Calculates the byte of the node configuration array for the given node i
 */
#if (NMOSEK_API_OPTIMIZATION == STD_ON)
# define NMOSEK_CALCULATE_CONFIG_BYTE(i)  (uint8)((i)>>3)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
/* PRQA S 3453 2 */ /* MD_MSR_19.7 */
# define NMOSEK_CALCULATE_CONFIG_BYTE(i)  (uint8)((uint8)((i)>>3) + \
                                                  (NMOSEK_CHANNEL_IDX*(uint8)(NMOSEK_NUMBER_OF_NM_CONFIG_NODES_DIV_8)))
#endif

#if defined(NMOSEK_TYPE_VECTOR2_OSEK)
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
# define NMOSEK_CALCULATE_SLEEP_IND_FROM_INTERNAL_STATE_BUS_SLEEP_IND(b) ((b) >> 5)
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
# define NMOSEK_CALCULATE_SLEEP_ACK_FROM_BOOLEAN(b) ((b) << 1)
#else
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
# define NMOSEK_CALCULATE_SLEEP_IND_FROM_INTERNAL_STATE_BUS_SLEEP_IND(b) ((b) >> 3)
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
# define NMOSEK_CALCULATE_SLEEP_ACK_FROM_BOOLEAN(b) ((b) << 5)
#endif

#if (NMOSEK_PDUMESSAGEDATASTARTPOSOFPBCHANNELCONFIG == STD_ON)
/* PRQA S 3453 2 */ /* MD_MSR_19.7 */
# define NMOSEK_MESSAGE_DATA_FIRST_BYTE(channel)      (NmOsek_GetPduMessageDataStartPosOfPbChannelConfig(channel))
# define NMOSEK_MESSAGE_DATA_BYTE_INDEX(channel, i)   ((NmOsek_GetPduMessageDataStartPosOfPbChannelConfig(channel)) + (i))
#else
/* PRQA S 3453 2 */ /* MD_MSR_19.7 */
# define NMOSEK_MESSAGE_DATA_FIRST_BYTE(channel)      (0u)
# define NMOSEK_MESSAGE_DATA_BYTE_INDEX(channel, i)   (i)
#endif

#if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
# define NmOsek_GetNumberOfSystemChannels() NmOsek_GetSizeOfSysToNmChannelInd()
#else
# define NmOsek_GetNumberOfSystemChannels() NmOsek_GetSizeOfChannelConfig()
#endif

#if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
# define NmOsek_GetModuleInitialized()      NmOsek_ModuleInitialized
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
# define NmOsek_SetModuleInitialized(Value) NmOsek_ModuleInitialized = (Value)
#endif

#if (NMOSEK_DESIREDNMSTATE == STD_ON)
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
# define NmOsek_GetDesiredOrActualNmState(ch) NmOsek_GetDesiredNmState(ch)
#else
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
# define NmOsek_GetDesiredOrActualNmState(ch) NmOsek_GetNmState(ch)
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define NMOSEK_START_SEC_VAR_CLEARED_8
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Initialization state of the module */
#if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
NMOSEK_LOCAL VAR(uint8, NMOSEK_VAR_ZERO_INIT) NmOsek_ModuleInitialized = NMOSEK_UNINIT;
#endif

#define NMOSEK_STOP_SEC_VAR_CLEARED_8
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (NMOSEK_USE_INIT_POINTER == STD_ON)
# define NMOSEK_START_SEC_VAR_CLEARED_UNSPECIFIED
# include "MemMap.h"    /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Pointer to Postbuild configuration */
NMOSEK_LOCAL P2CONST(NmOsek_ConfigType, NMOSEK_VAR_ZERO_INIT, NMOSEK_INIT_DATA) NmOsek_ConfigPtr = NULL_PTR;

# define NMOSEK_STOP_SEC_VAR_CLEARED_UNSPECIFIED
# include "MemMap.h"    /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define NMOSEK_START_SEC_CONST_8
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* NmOsek version */
CONST(uint8, NMOSEK_CONST) NmOsek_MainVersion      = (uint8)NMOSEK_SW_MAJOR_VERSION;
CONST(uint8, NMOSEK_CONST) NmOsek_SubVersion       = (uint8)NMOSEK_SW_MINOR_VERSION;
CONST(uint8, NMOSEK_CONST) NmOsek_ReleaseVersion   = (uint8)NMOSEK_SW_PATCH_VERSION;

#define NMOSEK_STOP_SEC_CONST_8
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define NMOSEK_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * NmOsek_ResetNodeConfig()
 *********************************************************************************************************************/
/*! \brief      Resets the node configuration.
 *  \details    Reset all node configuration bytes to 1 except the one where the own node is represented. Within this
 *              byte, one bit is set to 1, the others are set to 0.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        NmOsek is initialized.
 *  \note       Called by NmOsek_MainFunction()
 *  \trace      CREQ-119843
 *********************************************************************************************************************/
NMOSEK_LOCAL FUNC(void, NMOSEK_CODE) NmOsek_ResetNodeConfig(NMOSEK_CHANNEL_NMTYPE_ONLY);
#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/* ESCAN00054321 */
/**********************************************************************************************************************
 * NmOsek_TransmitSkippedAlive()
 *********************************************************************************************************************/
/*! \brief      Prepares a skipped alive message for asynchronous transmission
 *  \details    Fill the TX Message buffer with bytes of the skipped alive message and update state variables
 *  \param[in]  channel,nmChannelHandle     Identification of the NM-channel
 *  \param[in]  regularSkippedAliveMessage  FALSE: send a Skipped Alive Message designated for "Countermeasures
 *                                                 against G/W dropout"
 *                                          TRUE:  send a regular Skipped Alive Message (the one that is sent when the
 *                                                 own node is skipped in the logical ring
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction(), NmOsek_SkippedAliveMessageRequest()
 *  \note       regularSkippedAliveMessage is only required if there is the Skipped Alive Message Request API and
 *              the the Node Spec No. In this case there are two types of Skipped Alive Messages.
 *  \trace      CREQ-119844
 *********************************************************************************************************************/
NMOSEK_LOCAL FUNC(void, NMOSEK_CODE) NmOsek_TransmitSkippedAlive(NMOSEK_TRANSMIT_SKIPPED_ALIVE_CHANNEL_NMTYPE
                                                                 NMOSEK_TRANSMIT_SKIPPED_ALIVE_REGULARTYPE);

# if (NMOSEK_WAKEUPTXUSERDATAPDUIDOFPBCHANNELCONFIG == STD_ON)
/**********************************************************************************************************************
 * NmOsek_GetUserDataFromPduR()
 *********************************************************************************************************************/
/*! \brief      Get user data from PduR that shall be placed into the NM PDU
 *  \details    Retrieve user data from PduR by PduR_NmOsekTriggerTransmit and put it into the TX Message Data buffer
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \param[in]  pduId                   PDU Id (Awake or WakeUp PDU ID)
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *  \trace      CREQ-119854
 *********************************************************************************************************************/
NMOSEK_LOCAL FUNC(void, NMOSEK_CODE) NmOsek_GetUserDataFromPduR(NMOSEK_CHANNEL_NMTYPE_FIRST
                                                                CONST(PduIdType, AUTOMATIC) pduId);
# endif

/**********************************************************************************************************************
 * NmOsek_UpdateTxMessageData()
 *********************************************************************************************************************/
/*! \brief      Set most common bytes in the TxMessageDataBuffer for the given channel
 *  \details    Set destination identifier, opCode and Sleep Indication/Acknowledge bits in the TX Message Data
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \param[in]  destId                  Destination Identifier
 *  \param[in]  opCode                  OpCode
 *  \param[in]  sleepBits               Sleep Indication bit and Sleep Acknowledge Bit
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *  \trace      CREQ-119839
 *********************************************************************************************************************/
NMOSEK_LOCAL FUNC(void, NMOSEK_CODE) NmOsek_UpdateTxMessageData(NMOSEK_CHANNEL_NMTYPE_FIRST
                                                                CONST(NmOsek_TxMessageDataType, AUTOMATIC) destId,
                                                                CONST(NmOsek_TxMessageDataType, AUTOMATIC) opCode,
                                                                CONST(NmOsek_TxMessageDataType, AUTOMATIC) sleepBits);

#endif

/**********************************************************************************************************************
 * NmOsek_ChangeNmStateIfPossible()
 *********************************************************************************************************************/
/*! \brief      Change the Nm State according to the provided state value if possible
 *  \details    Updates the NmState variable if certain conditions are fulfilled
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \param[in]  newState                Target state to which the NmState shall change if possible
 *  \pre        -
 *********************************************************************************************************************/
NMOSEK_LOCAL FUNC(void, NMOSEK_CODE) NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_NMTYPE_FIRST
                                                                    CONST(Nm_StateType, AUTOMATIC) newState);

#if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
/**********************************************************************************************************************
 * NmOsek_ChangeRemoteSleepState()
 *********************************************************************************************************************/
/*! \brief      Change the remote sleep state to the provided state and notify Nm if required
 *  \details    Set the NmOsek_RemoteSleepState variable and call Nm_RemoteSleepIndication / Nm_RemoteSleepCancellation
 *              in dependency of the new remote sleep state
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \param[in]  newRemoteSleepState     New remote sleep state or Prep Sleep Flag:
 *                           NMOSEK_REMOTE_SLEEP_STATE_NO_SLEEP   - Not Remote Sleep
 *                           NMOSEK_REMOTE_SLEEP_STATE_SLEEP_IND  - Remote Sleep
 *                           NMOSEK_REMOTE_SLEEP_STATE_PREP_SLEEP - Flag to detect if all nodes are ready to sleep
 *  \pre        -
 *********************************************************************************************************************/
NMOSEK_LOCAL FUNC(void, NMOSEK_CODE) NmOsek_ChangeRemoteSleepState(NMOSEK_CHANNEL_NMTYPE_FIRST
                                                                   VAR(NmOsek_RemoteSleepStateType, AUTOMATIC) newRemoteSleepState);
#endif

#if (NMOSEK_NM_STATE_CHANGE_IND_ENABLED == STD_ON)
/**********************************************************************************************************************
 * NmOsek_Nm_StateChangeNotificationOnStateChange()
 *********************************************************************************************************************/
/*! \brief      Notify Nm about state change if state change has occurred and the current action is not Action Start
 *  \details    Call Nm_StateChangeNotification if a state change has occurred for the Nm state.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *********************************************************************************************************************/
NMOSEK_LOCAL FUNC(void, NMOSEK_CODE) NmOsek_Nm_StateChangeNotificationOnStateChange(NMOSEK_CHANNEL_NMTYPE_ONLY);
#endif

/**********************************************************************************************************************
 * NmOsek_MainFunction_UpdateTaskCounter()
 *********************************************************************************************************************/
/*! \brief      Decrement Task counter if it is running
 *  \details    If the Timeout Timer variable is greater than zero, decrement it.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_UpdateTaskCounter(NMOSEK_CHANNEL_NMTYPE_ONLY);

#if (NMOSEK_MSGTIMEOUTTIMER == STD_ON)
/**********************************************************************************************************************
 * NmOsek_MainFunction_UpdateMsgTimeoutTimer()
 *********************************************************************************************************************/
/*! \brief      Decrement Msg Timeout Timer counter if it is running and notify Tx Timeout on timeout
 *  \details    If the Timeout Timer variable is greater than zero, decrement it.
 *              If is then equal to zero, call Nm_TxTimeoutException()
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_UpdateMsgTimeoutTimer(NMOSEK_CHANNEL_NMTYPE_ONLY);
#endif

#if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 * NmOsek_MainFunction_BusOffOccurrence()
 *********************************************************************************************************************/
/*! \brief      Perform state changes due to BusOff occurrence
 *  \details    Set internal state variables that indicate BusOff and optionally notify Nm about state change
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *  \trace      CREQ-119860
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_BusOffOccurrence(NMOSEK_CHANNEL_NMTYPE_ONLY);

/**********************************************************************************************************************
 * NmOsek_MainFunction_BusOffRecovery()
 *********************************************************************************************************************/
/*! \brief      Perform state changes due to BusOff recovery
 *  \details    Clear Bus Error flag in internal state variable if current state is NMLimpHome
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *  \trace      CREQ-119860
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_BusOffRecovery(NMOSEK_CHANNEL_NMTYPE_ONLY);
#endif

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
 * NmOsek_MainFunction_CheckNmTxCounter()
 *********************************************************************************************************************/
/*! \brief      Check the NM TX Counter
 *  \details    Check whether the NM TX Counter has exceeded its limit. If so, enter LimpHome and notify Nm about
 *              state change.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_CheckNmTxCounter(NMOSEK_CHANNEL_NMTYPE_ONLY);
#endif

/**********************************************************************************************************************
 * NmOsek_MainFunction_ActionStart()
 *********************************************************************************************************************/
/*! \brief      Perform the Action Start handling
 *  \details    Reset internal state variables and check the TX counter. If it has not exceeded its limit,
 *              build the ALIVE message, set the TX request flag and enter NMNormal.
 *              If it has exceeded its limit, enter NMLimpHome.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_ActionStart(NMOSEK_CHANNEL_NMTYPE_ONLY);

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
 * NmOsek_MainFunction_Aux_TimeoutTimerElapsed_NmActive_ActionWaitSendtoken()
 *********************************************************************************************************************/
/*! \brief      Prepare the transmission of the RING message
 *  \details    Build the RING message and update internal state variables accordingly
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction_ActionWaitSendtoken()
 *  \trace      CREQ-119841
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_Aux_TimeoutTimerElapsed_NmActive_ActionWaitSendtoken(NMOSEK_CHANNEL_NMTYPE_ONLY);
#endif

/**********************************************************************************************************************
 * NmOsek_MainFunction_ActionWaitSendtoken()
 *********************************************************************************************************************/
/*! \brief      Perform the Action Wait Sendtoken handling
 *  \details    Send Skipped Alive Message / Change to Wait Ring Msg / Restart TTyp / Send Ring Message in dependency
 *              of the current state
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_ActionWaitSendtoken(NMOSEK_CHANNEL_NMTYPE_ONLY);

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
 * NmOsek_MainFunction_Aux_NmActive_ActionWaitRingmsg()
 *********************************************************************************************************************/
/*! \brief      Perform the auxiliary handling Action Wait Ringmsg if the current state is NmActive
 *  \details    Prepare a Skipped Alive Message for message transmission if the node has been skipped in the logical
 *              ring.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction_ActionWaitRingmsg()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_Aux_NmActive_ActionWaitRingmsg(NMOSEK_CHANNEL_NMTYPE_ONLY);
#endif

#if ((NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON) || (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_ON))
/**********************************************************************************************************************
 * NmOsek_MainFunction_Aux_NmPassive_ActionWaitRingmsg()
 *********************************************************************************************************************/
/*! \brief      Perform the auxiliary handling Action Wait Ringmsg if the current state is NmPassive
 *  \details    Update ring stability status. Change Nm State if the ring is stable now. Set PrepSleep according to the
 *              Bus Sleep Ind flag.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction_ActionWaitRingmsg()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_Aux_NmPassive_ActionWaitRingmsg(NMOSEK_CHANNEL_NMTYPE_ONLY);
#endif

/**********************************************************************************************************************
 * NmOsek_MainFunction_ActionWaitRingmsg()
 *********************************************************************************************************************/
/*! \brief      Perform the Action Wait Ringmsg handling
 *  \details    Check for Ring message reception / being skipped in the logical RING / TMax timeout.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_ActionWaitRingmsg(NMOSEK_CHANNEL_NMTYPE_ONLY);

#if (NMOSEK_SYNCPOINTNETWORKRELEASEINTERVALOFCHANNELCONFIG == STD_ON) && (NMOSEK_FAST_BUSOFF_RECOVERY == STD_ON)
/**********************************************************************************************************************
 * NmOsek_MainFunction_Aux_ShallSyncPointBeCalled_ActionLhWaitLimphomeTx()
 *********************************************************************************************************************/
/*! \brief      Determine whether Nm_SynchronizationPoint() shall be called in Action Lh Wait Limphome handling
 *  \details    Check whether the conditions to call Nm_SynchronizationPoint() are fulfilled if Fast BusOff Recovery is
 *              used.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \return     TRUE - Nm_SynchronizationPoint() shall be called.
 *              FALSE - Nm_SynchronizationPoint() shall be not called.
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(boolean, NMOSEK_CODE) NmOsek_MainFunction_Aux_ShallSyncPointBeCalled_ActionLhWaitLimphomeTx(NMOSEK_CHANNEL_NMTYPE_ONLY);
#endif

/**********************************************************************************************************************
 * NmOsek_MainFunction_ActionLhWaitLimphomeTx()
 *********************************************************************************************************************/
/*! \brief      Perform the Action Lh Wait Limphome handling
 *  \details    Send LIMPHOME message / transition from NMLimpHome to NMLimpHomePrepSleep
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_ActionLhWaitLimphomeTx(NMOSEK_CHANNEL_NMTYPE_ONLY);

/**********************************************************************************************************************
 * NmOsek_MainFunction_ActionLhPrepareSleep()
 *********************************************************************************************************************/
/*! \brief      Perform the Action Lh Prepare Sleep handling
 *  \details    transitions from NMLimpHomePrepSleep to NMLimpHome/NMTwbsLimpHome
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_ActionLhPrepareSleep(NMOSEK_CHANNEL_NMTYPE_ONLY);

/**********************************************************************************************************************
 * NmOsek_MainFunction_ActionsLhWaitLimphomeTxAndLhPrepareSleep()
 *********************************************************************************************************************/
/*! \brief      Perform the Action Lh Wait Limphome and the Action Lh Prepare Sleep handling
 *  \details    Reset node configuration / Set LimpHome flag / Set Remote Sleep state /
 *              Leave LimpHome if message reception/transmission is successful / Send LIMPHOME message / transitions
 *              from NMLimpHome to NMLimpHomePrepSleep and vice versa.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *  \trace      CREQ-119856
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_ActionsLhWaitLimphomeTxAndLhPrepareSleep(NMOSEK_CHANNEL_NMTYPE_ONLY);

/**********************************************************************************************************************
 * NmOsek_MainFunction_ActionGoBussleep()
 *********************************************************************************************************************/
/*! \brief      Perform the Action Go Bussleep handling
 *  \details    Transit to NMReset/NMLimpHome/NMBusSleep in dependency of the current state, if required.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_ActionGoBussleep(NMOSEK_CHANNEL_NMTYPE_ONLY);

/**********************************************************************************************************************
 * NmOsek_MainFunction_ActionBusSleep()
 *********************************************************************************************************************/
/*! \brief      Perform the Action Bus Sleep handling
 *  \details    Transit to NMReset in dependency of the current state, if required.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_ActionBusSleep(NMOSEK_CHANNEL_NMTYPE_ONLY);

/**********************************************************************************************************************
 * NmOsek_MainFunction_NoAction()
 *********************************************************************************************************************/
/*! \brief      Perform the No Action handling
 *  \details    Reset node configuration and status variables
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_NoAction(NMOSEK_CHANNEL_NMTYPE_ONLY);

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
 * NmOsek_MainFunction_IssueTxRequestIfDemandedAndPossible()
 *********************************************************************************************************************/
/*! \brief      Issue a transmission request if demanded and possible
 *  \details    Check whether there is a transmission request and whether it is possible to issue the transmit request.
 *              If so, call CanIf_Transmit(). If successful, clear the TX Request flag and
 *              update the Skipped Alive Message Request state if required.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_IssueTxRequestIfDemandedAndPossible(NMOSEK_CHANNEL_NMTYPE_ONLY);
#endif

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)

/**********************************************************************************************************************
 * NmOsek_TxConfirmation_HandleRingMessage()
 *********************************************************************************************************************/
/*! \brief      Handle a received RING message
 *  \details    Confirm user data PDU transmission if required by PduR Routing Path. Transit to NMTwbsNormal if
 *              Sleep.ack = 1 has been sent and Prep Sleep = 1. If Sleep.ind = 1 has been sent, set Prep Sleep flag.
 *              Outside the Wait Bus Sleep state, restart TMax and transit to Action Wait Ringmsg.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_TxConfirmation()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_TxConfirmation_HandleRingMessage(NMOSEK_CHANNEL_NMTYPE_ONLY);

/**********************************************************************************************************************
 * NmOsek_TxConfirmation_HandleAliveMessage()
 *********************************************************************************************************************/
/*! \brief      Handle a received ALIVE message
 *  \details    Confirm user data PDU transmission if required by PduR Routing Path. If the current state is NMLimpHome,
 *              set TX counter, use as LimpHome TX mark, reset only on LIMPHOME message.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_TxConfirmation()
 *  \trace      CREQ-119840
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_TxConfirmation_HandleAliveMessage(NMOSEK_CHANNEL_NMTYPE_ONLY);

/**********************************************************************************************************************
 * NmOsek_TxConfirmation_HandleLimpHomeMessage()
 *********************************************************************************************************************/
/*! \brief      Handle a received LIMPHOME message
 *  \details    Confirm user data PDU transmission if required by PduR Routing Path. In dependency of the configuration,
 *              LimpHome might be left if a NM message has also been received. Otherwise re-trigger TMax for successful
 *              transmission in NMLimpHomePrepSleep.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_TxConfirmation()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_TxConfirmation_HandleLimpHomeMessage(NMOSEK_CHANNEL_NMTYPE_ONLY);

# if (NMOSEK_CONFIRMTXUSERDATAPDUOFPBCHANNELCONFIG == STD_ON)
/**********************************************************************************************************************
 * NmOsek_TxConfirmation_NotifyPduR()
 *********************************************************************************************************************/
/*! \brief      Notify PduR about the successful transmission of the NM PDU
 *  \details    Confirm user data PDU transmission if required by PduR Routing Path.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \param[in]  opCode                  Operation Code
 *  \pre        -
 *  \note       Called by NmOsek_TxConfirmation()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_TxConfirmation_NotifyPduR(NMOSEK_CHANNEL_NMTYPE_FIRST CONST(uint8, AUTOMATIC) opCode);
# endif
#endif

/**********************************************************************************************************************
 * NmOsek_RxIndication_DetermineWhetherMessageIsValidAndUpdateStatusFlags()
 *********************************************************************************************************************/
/*! \brief      Determine whether the message is valid and update status flags
 *  \details    Check whether message is valid in dependency of the opCode (and eventually current state).
 *              Update node configuration array and determine logical successor for RING and ALIVE messages.
 *              Determine whether node is addressed and/or skipped over for RING messages.
 *              Clear ring stability flags for ALIVE messages and eventually update Nm state.
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \param[in]  sourceAddress           source address of the received NM message
 *  \param[in]  destAddress             destination address of the received NM message
 *  \param[in]  opCode                  op code of the received NM message
 *  \return     TRUE - Received message is a valid NM Message
 *              FALSE - Received message is not a valid NM Message
 *  \pre        -
 *  \note       Called by NmOsek_RxIndication()
 *  \trace      CREQ-119837, CREQ-119838, CREQ-119842, CREQ-119846
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(boolean, NMOSEK_CODE) NmOsek_RxIndication_DetermineWhetherMessageIsValidAndUpdateStatusFlags(NMOSEK_CHANNEL_NMTYPE_FIRST
  VAR(uint8, AUTOMATIC) sourceAddress, VAR(uint8, AUTOMATIC) destAddress, VAR(uint8, AUTOMATIC) opCode);

/**********************************************************************************************************************
 * NmOsek_RxIndication_HandleRxSleepAck()
 *********************************************************************************************************************/
/*! \brief      Handle the received message that has the Sleep Acknowledge bit set to 1.
 *  \details    Handle the received message with Sleep Acknowledge bit set to 1 in dependency of the BusSleepInd flag
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \pre        -
 *  \note       Called by NmOsek_RxIndication()
 *********************************************************************************************************************/
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_RxIndication_HandleRxSleepAck(NMOSEK_CHANNEL_NMTYPE_ONLY);

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
 * NmOsek_SetTxUserDataBytes()
 *********************************************************************************************************************/
/*! \brief      Set each TX User Data Byte to the provided value
 *  \details    Iterate over each TX User Data Byte of the given channel and set the value to the provided one
 *  \param[in]  channel,nmChannelHandle Identification of the NM-channel
 *  \param[in]  value                   The desired value for each TX User Data Byte
 *  \pre        -
 *  \note       Called by NmOsek_MainFunction()
 *  \trace      CREQ-119836
 *********************************************************************************************************************/
NMOSEK_LOCAL FUNC(void, NMOSEK_CODE) NmOsek_SetTxUserDataBytes(NMOSEK_CHANNEL_NMTYPE_FIRST
                                                               CONST(NmOsek_TxMessageDataType, AUTOMATIC) value);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * NmOsek_ResetNodeConfig()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
NMOSEK_LOCAL FUNC(void, NMOSEK_CODE) NmOsek_ResetNodeConfig(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (NMOSEK_USE_NM_NODE_LIST == STD_ON)
  VAR(uint8, AUTOMATIC) configByte;
#endif

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Clear ring stable flags */
  NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)(NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
    (NmOsek_InternalStateType)(NMOSEK_INTERNAL_STATE_RING_STABLE_INV_MASK &
                               NMOSEK_INTERNAL_STATE_RING_PRE_STABLE_INV_MASK)));
#if (NMOSEK_SYNCHRONIZINGNETWORKOFCHANNELCONFIG == STD_ON)
  NmOsek_SetRingCycleTimer(NMOSEK_CHANNEL_IDX, 0u);
  NmOsek_SetSyncPointTimer(NMOSEK_CHANNEL_IDX, 0u);
#endif

#if (NMOSEK_USE_NM_NODE_LIST == STD_ON)
  /* #20 Clear the whole configuration */
# if (NMOSEK_API_OPTIMIZATION == STD_ON)
  for (configByte = 0;
       configByte < NMOSEK_NUMBER_OF_NM_CONFIG_NODES_DIV_8;
       configByte++)
# else
  for (configByte = (uint8)((uint8)NMOSEK_CHANNEL_IDX      * (uint8)(NMOSEK_NUMBER_OF_NM_CONFIG_NODES_DIV_8));
       configByte <        (((uint8)NMOSEK_CHANNEL_IDX + 1) * (uint8)(NMOSEK_NUMBER_OF_NM_CONFIG_NODES_DIV_8));
       configByte++ )
# endif
  {
    NmOsek_GetNodeConfiguration().nmConfigChar[configByte] = 0u;
  }

  /* #30 Set own ECU number as present */
  configByte = NMOSEK_CALCULATE_CONFIG_BYTE(NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX));
  NmOsek_GetNodeConfiguration().nmConfigChar[configByte] |=
    (uint8)(1 << (uint8)(NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX) & (uint8)NMOSEK_BYTE_OFFSET_MASK));
#endif

}

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
 * NmOsek_TransmitSkippedAlive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* ESCAN00054321 */
NMOSEK_LOCAL FUNC(void, NMOSEK_CODE) NmOsek_TransmitSkippedAlive(NMOSEK_TRANSMIT_SKIPPED_ALIVE_CHANNEL_NMTYPE
                                                                 NMOSEK_TRANSMIT_SKIPPED_ALIVE_REGULARTYPE)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Request transmission and increment TX counter */
  NmOsek_SetTxRequest(NMOSEK_CHANNEL_IDX, TRUE);                      /* Request transmission */
  NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, (NmOsek_RxTxCntType)
    (NmOsek_GetRxTxCnt(NMOSEK_CHANNEL_IDX) + NMOSEK_TX_CNT_STEP));  /* Increment TX counter */

  /* ESCAN00049910 */
  /* #20 Fill TX message buffer */
  /* Set OpCode to Alive, Set Sleep.ind = Bus Sleep Ind, Sleep.ack = 0 */
  /* Set Dest ID to logical successor */
  NmOsek_UpdateTxMessageData(NMOSEK_CHANNEL_PARA_FIRST
    NmOsek_GetLogicalSuccessor(NMOSEK_CHANNEL_IDX), NMOSEK_ALIVE, (NmOsek_TxMessageDataType)
     (NMOSEK_CALCULATE_SLEEP_IND_FROM_INTERNAL_STATE_BUS_SLEEP_IND(NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
                                                                  NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK)));

# if (NMOSEK_WAKEUPTXUSERDATAPDUIDOFPBCHANNELCONFIG == STD_ON)
#  if (NMOSEK_ONLY_VALID_USER_DATA_TX_PDU_IDS == STD_OFF)
    if (NmOsek_GetWakeUpTxUserDataPduIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX) != NMOSEK_NO_WAKEUPTXUSERDATAPDUIDOFPBCHANNELCONFIG)
#  endif
    {
      {
        NmOsek_GetUserDataFromPduR(NMOSEK_CHANNEL_PARA_FIRST
                                   NmOsek_GetWakeUpTxUserDataPduIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX));
      }
    }
# endif

  /* #30 Update state variables */
  NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_REPEAT_MESSAGE);

  NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)(NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
    (NmOsek_InternalStateType)(NMOSEK_INTERNAL_STATE_RING_STABLE_INV_MASK & NMOSEK_INTERNAL_STATE_RING_PRE_STABLE_INV_MASK)));
# if (NMOSEK_SYNCHRONIZINGNETWORKOFCHANNELCONFIG == STD_ON)
  NmOsek_SetRingCycleTimer(NMOSEK_CHANNEL_IDX, 0u);
  NmOsek_SetSyncPointTimer(NMOSEK_CHANNEL_IDX, 0u);
# endif

# if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* #40 Cancel remote sleep state if remote sleep state has been previously reached */
  NmOsek_ChangeRemoteSleepState(NMOSEK_CHANNEL_PARA_FIRST NMOSEK_REMOTE_SLEEP_STATE_NO_SLEEP);
# endif
}

# if (NMOSEK_WAKEUPTXUSERDATAPDUIDOFPBCHANNELCONFIG == STD_ON)
/**********************************************************************************************************************
 * NmOsek_GetUserDataFromPduR()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
NMOSEK_LOCAL FUNC(void, NMOSEK_CODE) NmOsek_GetUserDataFromPduR(NMOSEK_CHANNEL_NMTYPE_FIRST
                                                                CONST(PduIdType, AUTOMATIC) pduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;
  VAR(PduInfoType, AUTOMATIC) pduInfo;

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 Build Pdu Info data structure */
  pduInfo.SduDataPtr =
    &NmOsek_GetTxMessageData(NMOSEK_MESSAGE_DATA_BYTE_INDEX(NMOSEK_CHANNEL_IDX, NMOSEK_USER_DATA_OFFSET));
  /* Length of the buffer provided to PduR_NmOsekTriggerTransmit() */ /* ESCAN00087845 */
  pduInfo.SduLength = (PduLengthType)(NmOsek_GetPduLengthOfPbChannelConfig(NMOSEK_CHANNEL_IDX) -
                                      NMOSEK_USER_DATA_OFFSET);

  /* - #20 Retrieve User Data from PduR */
  (void)PduR_NmOsekTriggerTransmit(pduId, &pduInfo);
#  if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* ESCAN00082043 */
  if (pduInfo.SduLength != (NmOsek_GetPduLengthOfPbChannelConfig(NMOSEK_CHANNEL_IDX) - NMOSEK_USER_DATA_OFFSET))
  {
    errorId = NMOSEK_E_PDUR_TRIGGERTX_ERROR;
  }
#  endif

  /* ----- Development Error Report --------------------------------------- */
#  if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_MAINFUNCTION, errorId);
  }
#  else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif

#  if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_PDUMESSAGEDATASTARTPOSOFPBCHANNELCONFIG == STD_OFF) && \
      (NMOSEK_API_OPTIMIZATION == STD_OFF) 
  NMOSEK_DUMMY_STATEMENT_CONST(NMOSEK_CHANNEL_IDX); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
}
# endif

/**********************************************************************************************************************
 * NmOsek_UpdateTxMessageData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
NMOSEK_LOCAL FUNC(void, NMOSEK_CODE) NmOsek_UpdateTxMessageData(NMOSEK_CHANNEL_NMTYPE_FIRST
                                                                CONST(NmOsek_TxMessageDataType, AUTOMATIC) destId,
                                                                CONST(NmOsek_TxMessageDataType, AUTOMATIC) opCode,
                                                                CONST(NmOsek_TxMessageDataType, AUTOMATIC) sleepBits)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 Set the provided bytes in the Tx Message Data */
# if (NMOSEK_LAST_BYTE_OF_CAN_ID_AS_NODE_ID_ENABLED == STD_ON)
  /* Set Dest ID to provided Dest ID with offset */
  NmOsek_SetTxMessageData(NMOSEK_MESSAGE_DATA_BYTE_INDEX(NMOSEK_CHANNEL_IDX, NMOSEK_CFG_PDU_DEST_ID_POSITION),
    (NmOsek_TxMessageDataType)(destId +
                               NmOsek_GetNodeIdOffsetOfChannelConfig(NMOSEK_CHANNEL_IDX)));
# else
  /* Set Dest ID */
  NmOsek_SetTxMessageData(NMOSEK_MESSAGE_DATA_BYTE_INDEX(NMOSEK_CHANNEL_IDX, NMOSEK_CFG_PDU_DEST_ID_POSITION),
                          destId);
# endif
  NmOsek_SetTxMessageData(NMOSEK_MESSAGE_DATA_BYTE_INDEX(NMOSEK_CHANNEL_IDX, NMOSEK_CFG_PDU_OPCODE_POSITION),
                          (NmOsek_TxMessageDataType)(NMOSEK_OPCODE_RESERVED | sleepBits | opCode));

# if (NMOSEK_PDUMESSAGEDATASTARTPOSOFPBCHANNELCONFIG == STD_ON)
# else
  NMOSEK_DUMMY_STATEMENT_CONST(NMOSEK_CHANNEL_IDX); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
}
#endif /* (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) */

/**********************************************************************************************************************
 * NmOsek_ChangeNmStateIfPossible()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
NMOSEK_LOCAL FUNC(void, NMOSEK_CODE) NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_NMTYPE_FIRST
                                                                    CONST(Nm_StateType, AUTOMATIC) newState)
{
#if (NMOSEK_DESIREDNMSTATE == STD_ON)
  /* - #10 If available, update the DesiredNmState variable */
  NmOsek_SetDesiredNmState(NMOSEK_CHANNEL_IDX, newState);
#endif

  /* - #20 If possible, update the NmState variable */
#if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON) || (NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE == STD_ON)
  if ((newState < NM_STATE_READY_SLEEP) ||
# if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON) 
      (newState > NM_STATE_REPEAT_MESSAGE) ||
# endif
      (
# if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
       (!NmOsek_IsCanIsBusOff(NMOSEK_CHANNEL_IDX))
# endif
# if (NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE == STD_ON)
#  if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON) 
       &&
#  endif
       (
        (NmOsek_IsFirstTxConfOrRxInd(NMOSEK_CHANNEL_IDX))
        ||
#  if (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)
        ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & (NMOSEK_INTERNAL_STATE_LIMP_HOME_FLAG_MASK |
                                                         NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK)) !=
         NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK)
#  else
        ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_LIMP_HOME_FLAG_MASK) != 0u)
#  endif
       )
# endif
      ))
#endif
  {
    NmOsek_SetNmState(NMOSEK_CHANNEL_IDX, newState);
  }
#if (NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE == STD_ON)
  else if (NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) <= NM_STATE_PREPARE_BUS_SLEEP)
  {
    /* Enter the state Wait Startup so that NmOsek is in Network Mode */
    NmOsek_SetNmState(NMOSEK_CHANNEL_IDX, NM_STATE_WAIT_STARTUP);
  }
  else
  {
    /* Intentionally left empty */
  }
#endif
}

#if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
/**********************************************************************************************************************
 * NmOsek_ChangeRemoteSleepState()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
NMOSEK_LOCAL FUNC(void, NMOSEK_CODE) NmOsek_ChangeRemoteSleepState(NMOSEK_CHANNEL_NMTYPE_FIRST
                                                                   VAR(NmOsek_RemoteSleepStateType, AUTOMATIC) newRemoteSleepState)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 If the Remote Sleep State shall be set to No Sleep */
  if( newRemoteSleepState == NMOSEK_REMOTE_SLEEP_STATE_NO_SLEEP )
  {
    /* - #20 Call Nm_RemoteSleepCancellation, if not called yet */
    if( ( NmOsek_GetRemoteSleepState(NMOSEK_CHANNEL_IDX) & NMOSEK_REMOTE_SLEEP_STATE_SLEEP_IND ) != 0u )
    {
      Nm_RemoteSleepCancellation(NMOSEK_NMTOSYSTEM_CHANNEL_IND);
    }
    /* - #30 Set Remote Sleep State to No Sleep and clear Remote Prep Sleep flag */
    NmOsek_SetRemoteSleepState( NMOSEK_CHANNEL_IDX, NMOSEK_REMOTE_SLEEP_STATE_NO_SLEEP );
  }
  else
  /* - #40 Else */
  {
    /* - #50 If the Remote Prep Sleep Flag shall be set */
    if( newRemoteSleepState == NMOSEK_REMOTE_SLEEP_STATE_PREP_SLEEP )
    {
      /* - #60 Set the Remote Prep Sleep Flag if not already set */
      if( ( NmOsek_GetRemoteSleepState(NMOSEK_CHANNEL_IDX) & NMOSEK_REMOTE_SLEEP_STATE_PREP_SLEEP ) == 0u )
      {
        NmOsek_SetRemoteSleepState( NMOSEK_CHANNEL_IDX, (NmOsek_RemoteSleepStateType)
                                    (NmOsek_GetRemoteSleepState(NMOSEK_CHANNEL_IDX) | NMOSEK_REMOTE_SLEEP_STATE_PREP_SLEEP) );
      }
      else
      /* - #70 If the Prep Sleep Flag is already set, Remote Sleep state shall be set to Sleep Ind */
      {
        newRemoteSleepState = NMOSEK_REMOTE_SLEEP_STATE_SLEEP_IND;
      }
    }

    /* - #80 If the Remote Sleep State shall be set to Sleep Ind */
    if( newRemoteSleepState == NMOSEK_REMOTE_SLEEP_STATE_SLEEP_IND )
    {
      /* - #90 Call Nm_RemoteSleepIndication, if not called yet */
      if( ( NmOsek_GetRemoteSleepState(NMOSEK_CHANNEL_IDX) & NMOSEK_REMOTE_SLEEP_STATE_SLEEP_IND ) == 0u )
      {
        Nm_RemoteSleepIndication(NMOSEK_NMTOSYSTEM_CHANNEL_IND);
      }
      /* - #100 Set Remote Sleep state to Sleep Ind */
      NmOsek_SetRemoteSleepState( NMOSEK_CHANNEL_IDX, (NmOsek_RemoteSleepStateType)
                                  (NmOsek_GetRemoteSleepState(NMOSEK_CHANNEL_IDX) | NMOSEK_REMOTE_SLEEP_STATE_SLEEP_IND) );
    }
  }
}
#endif

#if (NMOSEK_NM_STATE_CHANGE_IND_ENABLED == STD_ON)
/**********************************************************************************************************************
 * NmOsek_Nm_StateChangeNotificationOnStateChange()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
NMOSEK_LOCAL FUNC(void, NMOSEK_CODE) NmOsek_Nm_StateChangeNotificationOnStateChange(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 If the previous Nm state is not equal to the current Nm state and the current action is not Action start,
   *       notify Nm and set the previous Nm state to the current Nm state.
   */
  /* ESCAN00049564 */
  if ((NmOsek_GetPreviousNmState(NMOSEK_CHANNEL_IDX) != NmOsek_GetNmState(NMOSEK_CHANNEL_IDX)) && /* ESCAN00055975 */
      ((NmOsek_GetPreviousNmState(NMOSEK_CHANNEL_IDX) != NM_STATE_BUS_SLEEP) ||
       ((NmOsek_GetAction(NMOSEK_CHANNEL_IDX) & NMOSEK_ACTION_REPEAT_INV) != NMOSEK_ACTION_START))
     )
  {
    /* Inform Nm */
    Nm_StateChangeNotification(NMOSEK_NMTOSYSTEM_CHANNEL_IND,
                               NmOsek_GetPreviousNmState(NMOSEK_CHANNEL_IDX), /* ESCAN00055975 */
                               NmOsek_GetNmState(NMOSEK_CHANNEL_IDX));

    NmOsek_SetPreviousNmState(NMOSEK_CHANNEL_IDX, NmOsek_GetNmState(NMOSEK_CHANNEL_IDX)); /* ESCAN00055975 */
  }
}
#endif

/**********************************************************************************************************************
 * NmOsek_MainFunction_UpdateTaskCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_UpdateTaskCounter(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 If the Timeout Timer is greater than zero, decrement it by one. */
  if (NmOsek_GetTimeoutTimer(NMOSEK_CHANNEL_IDX) > 0u)
  {
    NmOsek_DecTimeoutTimer(NMOSEK_CHANNEL_IDX);
  }
}

#if (NMOSEK_MSGTIMEOUTTIMER == STD_ON)
/**********************************************************************************************************************
 * NmOsek_MainFunction_UpdateMsgTimeoutTimer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_UpdateMsgTimeoutTimer(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
 /* - #10 If the Msg Timeout Timer is greater than zero, decrement it by one. */
  if (NmOsek_GetMsgTimeoutTimer(NMOSEK_CHANNEL_IDX) > 0u)
  {
    NmOsek_DecMsgTimeoutTimer(NMOSEK_CHANNEL_IDX);

    if (NmOsek_GetMsgTimeoutTimer(NMOSEK_CHANNEL_IDX) == 0u)
    {
      /* - #20 If it is zero: notify timeout */
      Nm_TxTimeoutException(NMOSEK_NMTOSYSTEM_CHANNEL_IND);
    }
  }
}
#endif

#if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 * NmOsek_MainFunction_BusOffOccurrence()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_BusOffOccurrence(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 Set BusError flag in internal state */
  NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX,
    (NmOsek_InternalStateType) (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_BUS_ERROR_FLAG_MASK));
  /* PrepSleep == 1 && ((BusSleep == 1) || (WaitBusSleep == 1)) */
  if ((NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) <= NM_STATE_PREPARE_BUS_SLEEP) &&
      ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK) != 0u))
  {
    /* There was a BusOff after the SleepAck, i.e. while the NM is about to be in sleep mode
     * Hold the Sleep state regardless of BusOff - restart always with Reset Alive.
     */
  }
  else
  {
    /* - #20 If the BusOff has been detected before NMTwbsNormal / NMTwbsLimpHome:
     *     clear message reception flags and cancel any NM TX request
     *     clear RX counter and set TX counter to 1
     */
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
      (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
       NMOSEK_INTERNAL_STATE_NM_STATUS_RX_FLAGS_AND_RECV_REMIND_RX_TX_LOCK_INV_MASK));
# if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
    NmOsek_SetTxRequest(NMOSEK_CHANNEL_IDX, FALSE); /* Cancel any NM TX request */
# endif

    /* Clear RX counter, set TX counter to 1 */
    NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, NMOSEK_TX_CNT_STEP);

    /* - #30 If NMLimpHomePrepSleep has not been entered: */
    if (NmOsek_GetAction(NMOSEK_CHANNEL_IDX) != NMOSEK_ACTION_LH_PREPARE_SLEEP)
    {
      /* - #40 Start LimpHome timer */
      /* LimpHome is left when CanSM has notified about a BusOff recovery and the timer has expired.
       * ESCAN00052622
       */
# if (NMOSEK_FAST_BUSOFF_RECOVERY == STD_ON)
      if (NmOsek_GetBusOffCounter(NMOSEK_CHANNEL_IDX) > 1u) /* ESCAN00083049, ESCAN00083051 */
      {
        NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
          NmOsek_GetLimpHomeShortTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));
      }
      else
# endif
      {
        NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
          NmOsek_GetLimpHomeTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));
      }

      /* - #50 Goto LimpHome */
      NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
        ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_SLEEP_INV_MASK) &
         NMOSEK_INTERNAL_STATE_WAIT_BUS_SLEEP_INV_MASK)); /* ESCAN00085297 */

      NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_LH_WAIT_LIMPHOME_TX);

      if (NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) <= NM_STATE_PREPARE_BUS_SLEEP)
      {
        Nm_NetworkMode(NMOSEK_NMTOSYSTEM_CHANNEL_IND); /* ESCAN00083139 */
      }
    } /* (NmOsek_GetAction(NMOSEK_CHANNEL_IDX) != NMOSEK_ACTION_LH_PREPARE_SLEEP) */

# if (NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE == STD_ON)
    NmOsek_SetFirstTxConfOrRxInd(NMOSEK_CHANNEL_IDX, FALSE);
# endif

    /* - #70 Notify Nm about state change */
    NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_BUS_OFF);
# if (NMOSEK_NM_STATE_CHANGE_IND_ENABLED == STD_ON)
    NmOsek_Nm_StateChangeNotificationOnStateChange(NMOSEK_CHANNEL_PARA_ONLY);
# endif
  }

# if (NMOSEK_FAST_BUSOFF_RECOVERY == STD_ON)
  /* - #80 Update BusOff counter */
  if (NmOsek_GetBusOffCounter(NMOSEK_CHANNEL_IDX) > 0u)
  {
    NmOsek_DecBusOffCounter(NMOSEK_CHANNEL_IDX);
  }
# endif
}

/**********************************************************************************************************************
 * NmOsek_MainFunction_BusOffRecovery()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_BusOffRecovery(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 If the current state is not NMLimpHome, clear the Bus Error flag in internal state variable. */
# if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
  if (NmOsek_GetAction(NMOSEK_CHANNEL_IDX) != NMOSEK_ACTION_LH_WAIT_LIMPHOME_TX)
# else
  /* NMOSEK_ACTION_LH_WAIT_LIMPHOME_TX will be left later on in the context of this NmOsek_MainFunction() call */
  /* ESCAN00084494 */
# endif
  {
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
                            (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_ERROR_INV_MASK));

    if (NmOsek_GetAction(NMOSEK_CHANNEL_IDX) == NMOSEK_ACTION_LH_PREPARE_SLEEP)
    {
      NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_READY_SLEEP);
    }
  }
}
#endif

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
 * NmOsek_MainFunction_CheckNmTxCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_CheckNmTxCounter(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 If the TX Counter has exceeded its limit, enter NMLimpHome */
  if (((NmOsek_GetRxTxCnt(NMOSEK_CHANNEL_IDX) & NMOSEK_TX_CNT_MASK) >> 4) > NMOSEK_TX_CNT_MAX)
  {
    /* TX Counter run off -> Start limphome */
    NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, NMOSEK_TX_CNT_STEP); /* Use TX counter as limphome mark */
    NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
                           NmOsek_GetLimpHomeTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));

    /* Goto LimpHome */
    NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_LH_WAIT_LIMPHOME_TX);

    NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_REPEAT_MESSAGE);

    /* - #20 Notify Nm about state change if occurred */
# if (NMOSEK_NM_STATE_CHANGE_IND_ENABLED == STD_ON)
    NmOsek_Nm_StateChangeNotificationOnStateChange(NMOSEK_CHANNEL_PARA_ONLY);
# endif
  }
}
#endif

/**********************************************************************************************************************
 * NmOsek_MainFunction_ActionStart()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_ActionStart(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 Reset the node configuration */
  NmOsek_ResetNodeConfig(NMOSEK_CHANNEL_PARA_ONLY);

  /* - #20 Reset global status bits that do not depend on application and reset local status byte */
  NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
    (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTION_START_INV_MASK));

#if (NMOSEK_TOKEN_MONITORING == STD_ON)
  /* - #30 Do not allow Ring Data access */
  /* ESCAN00052621 */
  NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
    (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_TX_RING_DATA_ALLOW_FLAG_MASK));
#endif

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
  /* - #40 Clear any pending TX request */
  NmOsek_SetTxRequest(NMOSEK_CHANNEL_IDX, FALSE);
#endif

  /* - #50 Set last received source address to own node ID and increment Nmrxcount */
#if (NMOSEK_NM_NODE_ID_ENABLED == STD_ON)
  NmOsek_SetRxNmMsgSourceAddr(NMOSEK_CHANNEL_IDX,
    NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX)); /* Initial value: self-addressed */
#endif
  NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, (NmOsek_RxTxCntType)
                    (NmOsek_GetRxTxCnt(NMOSEK_CHANNEL_IDX) + NMOSEK_RX_CNT_STEP)); /* Increment Nmrxcount */

#if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* - #60 Clear Remote Sleep State */
  NmOsek_ChangeRemoteSleepState(NMOSEK_CHANNEL_PARA_FIRST NMOSEK_REMOTE_SLEEP_STATE_NO_SLEEP);
#endif

  /* - #70 If the NmActive flag is set, build the ALIVE message and set the TX Request flag */
#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
# if (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)
  if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) != 0u)
# else
  /* (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) != 0u:
   * always true
   */
# endif
  {
    /* Wait for NmOsek_TxConfirmation() */
    NmOsek_SetTxRequest(NMOSEK_CHANNEL_IDX, TRUE); /* Request TX of ALIVE message */

    NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, (NmOsek_RxTxCntType)
                      (NmOsek_GetRxTxCnt(NMOSEK_CHANNEL_IDX) + NMOSEK_TX_CNT_STEP)); /* Increment TX counter */

    /*--- Build ALIVE message ---*/
    NmOsek_SetLogicalSuccessor(NMOSEK_CHANNEL_IDX, NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX));
    /* ESCAN00049910 */
    /* Set Dest ID to own node ID */ /* ESCAN00052665 */
    /* Set OpCode to Alive, Sleep.ind = 0; Sleep.ack = 0; */
    NmOsek_UpdateTxMessageData(NMOSEK_CHANNEL_PARA_FIRST
      NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX), NMOSEK_ALIVE, 0u);

# if (NMOSEK_WAKEUPTXUSERDATAPDUIDOFPBCHANNELCONFIG == STD_ON)
    /* ESCAN00090119 */
#   if (NMOSEK_ONLY_VALID_USER_DATA_TX_PDU_IDS == STD_OFF)
    if (NmOsek_GetWakeUpTxUserDataPduIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX) != NMOSEK_NO_WAKEUPTXUSERDATAPDUIDOFPBCHANNELCONFIG)
#   endif
    {
      NmOsek_GetUserDataFromPduR(NMOSEK_CHANNEL_PARA_FIRST
                                 NmOsek_GetWakeUpTxUserDataPduIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX));
    }
#  if (NMOSEK_ONLY_VALID_USER_DATA_TX_PDU_IDS == STD_OFF) 
    else
#  endif
# endif
# if (NMOSEK_WAKEUPTXUSERDATAPDUIDOFPBCHANNELCONFIG != STD_ON) || (NMOSEK_ONLY_VALID_USER_DATA_TX_PDU_IDS == STD_OFF) 
    {
      NmOsek_SetTxUserDataBytes(NMOSEK_CHANNEL_PARA_FIRST NMOSEK_DATA_INIT);
    }
# endif
  } /* (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) != 0u */
#endif /* (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) */

  /* - #80 Check NM RX counter: change to NMNormal if within limit, otherwise to NMLimpHome */
  if ((NmOsek_GetRxTxCnt(NMOSEK_CHANNEL_IDX) & NMOSEK_RX_CNT_MASK) <= NMOSEK_RX_CNT_MAX)
  {
    NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
                           (NmOsek_TimeoutTimerType)(NmOsek_GetRingTypTimeOfChannelConfig(NMOSEK_CHANNEL_IDX) + 1u));

    NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_WAIT_SENDTOKEN);
  }
  else
  {
    /* RX counter run off -> start limphome */
    NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, NMOSEK_TX_CNT_STEP); /* Use TX counter as limphome mark */

    NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_LH_WAIT_LIMPHOME_TX);

    NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX, NmOsek_GetLimpHomeTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));
  }

  NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_REPEAT_MESSAGE); /* ESCAN00076015 */
}

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
 * NmOsek_MainFunction_Aux_TimeoutTimerElapsed_NmActive_ActionWaitSendtoken()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_Aux_TimeoutTimerElapsed_NmActive_ActionWaitSendtoken(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 Build RING message */

# if (NMOSEK_TOKEN_MONITORING == STD_ON)
  /* Token is passed on => disable access on userdata */ /* ESCAN00052621 */
  NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
    (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_TX_RING_DATA_ALLOW_FLAG_MASK));
# endif

  /* - #20 Set flag that a function call of NmOsek_TxConfirmation() is expected */
  NmOsek_SetTxRequest(NMOSEK_CHANNEL_IDX, TRUE); /* Request TX of RING message */
  NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, (NmOsek_RxTxCntType)
    (NmOsek_GetRxTxCnt(NMOSEK_CHANNEL_IDX) + NMOSEK_TX_CNT_STEP)); /* Increment NM TX counter */

  /* - #30 Set Ring cmd and determine status of flags SleepInd and SleepAck */
  /* Derive Sleep.ind from the application request in case the own node is not the successor */
  if ((NmOsek_GetLogicalSuccessor(NMOSEK_CHANNEL_IDX) != NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX)) &&
      ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
       NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK) != 0u))
  {
    /* Check if SleepAck should be transmitted */
    if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK) != 0u)
    {
      /* Sleep.ind = 1; Sleep.ack = 1; */ /* ESCAN00049910 */ /* ESCAN00066879 */
      NmOsek_UpdateTxMessageData(NMOSEK_CHANNEL_PARA_FIRST NmOsek_GetLogicalSuccessor(NMOSEK_CHANNEL_IDX),
        NMOSEK_RING, NMOSEK_SLEEP_IND | NMOSEK_SLEEP_ACK);
    }
    else
    {
      /* Sleep.ind = 1; Sleep.ack = 0; */ /* ESCAN00049910 */ /* ESCAN00066879 */
      NmOsek_UpdateTxMessageData(NMOSEK_CHANNEL_PARA_FIRST NmOsek_GetLogicalSuccessor(NMOSEK_CHANNEL_IDX),
        NMOSEK_RING, NMOSEK_SLEEP_IND);
    }
  }
  else /* if ((NmOsek_GetLogicalSuccessor(NMOSEK_CHANNEL_IDX) == NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX)) ||
        *     (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK) == 0u) */
  {
    /* Sleep.ind = 0; Sleep.ack = 0; */ /* ESCAN00049910 */
    NmOsek_UpdateTxMessageData(NMOSEK_CHANNEL_PARA_FIRST NmOsek_GetLogicalSuccessor(NMOSEK_CHANNEL_IDX),
      NMOSEK_RING, 0u);
  }

  /* - #40 Check stability of ring and update internal states accordingly */
  if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_RING_PRE_STABLE_FLAG_MASK) != 0u)
  {
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
      (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_RING_STABLE_FLAG_MASK));

    /* Ready to sleep, set new NmState */
    if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
         NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK) != 0u)
    {
      NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_READY_SLEEP);
    }
    else
    {
      NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_NORMAL_OPERATION);
    }

# if (NMOSEK_SYNCHRONIZINGNETWORKOFCHANNELCONFIG == STD_ON)
    /* - #50 Check handling if channel is Synchronizing Network */
    if (NmOsek_GetRingCycleTimer(NMOSEK_CHANNEL_IDX) > 0u)
    {
      /* If the current ring cycle was faster than the previous one, call Nm_SynchronizationPoint() now */
      if (NmOsek_GetSyncPointTimer(NMOSEK_CHANNEL_IDX) > 0u)
      {
        Nm_SynchronizationPoint(NMOSEK_NMTOSYSTEM_CHANNEL_IND); /* ESCAN00086246 */
      }
      /* Since the ring is stable now, calculate the moments when Nm_SynchronizationPoint() shall be called */
      {
#   if (NMOSEK_SYNCPOINTNETWORKRELEASEINTERVALOFCHANNELCONFIG == STD_ON)
        NmOsek_SetSyncPointTimer(NMOSEK_CHANNEL_IDX, (NmOsek_SyncPointTimerType)
          (NmOsek_GetRingCycleTimer(NMOSEK_CHANNEL_IDX) -
            ((((NmOsek_GetRingMaxTimeOfChannelConfig(NMOSEK_CHANNEL_IDX) + 1u) -
              NmOsek_GetRingTypTimeOfChannelConfig(NMOSEK_CHANNEL_IDX)) +
              NmOsek_GetSyncPointNetworkReleaseIntervalOfChannelConfig(NMOSEK_CHANNEL_IDX)) %
            NmOsek_GetRingCycleTimer(NMOSEK_CHANNEL_IDX))));
#   else
        /* NmOsek_GetSyncPointNetworkReleaseIntervalOfChannelConfig() would return 0 for each channel */
        NmOsek_SetSyncPointTimer(NMOSEK_CHANNEL_IDX, (NmOsek_SyncPointTimerType)
          (NmOsek_GetRingCycleTimer(NMOSEK_CHANNEL_IDX) -
            ((((NmOsek_GetRingMaxTimeOfChannelConfig(NMOSEK_CHANNEL_IDX) + 1u) -
              NmOsek_GetRingTypTimeOfChannelConfig(NMOSEK_CHANNEL_IDX))) %
            NmOsek_GetRingCycleTimer(NMOSEK_CHANNEL_IDX))));
#   endif
      }

      NmOsek_SetRingCycleTimer(NMOSEK_CHANNEL_IDX, 0u);
    }
# endif
  }
  else
  {
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
      (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_RING_PRE_STABLE_FLAG_MASK));
  }

# if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* - #60 Process remote sleep indication */
  NmOsek_ChangeRemoteSleepState( NMOSEK_CHANNEL_PARA_FIRST NMOSEK_REMOTE_SLEEP_STATE_PREP_SLEEP );
# endif

  /* - #70 Continue to build RING message */
  /* ESCAN00049910 */

# if (NMOSEK_AWAKETXUSERDATAPDUIDOFPBCHANNELCONFIG == STD_ON)
#  if (NMOSEK_ONLY_VALID_USER_DATA_TX_PDU_IDS == STD_OFF)
  /* ESCAN00090119 */
  if (NmOsek_GetAwakeTxUserDataPduIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX) != NMOSEK_NO_AWAKETXUSERDATAPDUIDOFPBCHANNELCONFIG)
#  endif
  {
    {
      NmOsek_GetUserDataFromPduR(NMOSEK_CHANNEL_PARA_FIRST
        NmOsek_GetAwakeTxUserDataPduIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX));
    }
  }
# endif
  /* - #80 Start TMax timer and change to waiting state */
  NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX, NmOsek_GetRingMaxTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));

  NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_WAIT_RINGMSG);
}
#endif /* (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) */

/**********************************************************************************************************************
 * NmOsek_MainFunction_ActionWaitSendtoken()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_ActionWaitSendtoken(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 If the Active flag is set: */
#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
# if (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)
  if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) != 0u)
# else
  /* (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) != 0u:
   * always true
   */
# endif
  {
    /* NmActive */

    /* - #20 If this node has been skipped in the logical ring, transmit a SKIPPED ALIVE message */
    if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_IM_SKIPPED_OVER_FLAG_MASK) != 0u)
    {
      /*
       * Code: don't change the counter.
       *       The own NM message might be transmitted now or by the next task call,
       */

      /* Transmit SKIPPED ALIVE message */
      NmOsek_TransmitSkippedAlive(NMOSEK_TRANSMIT_SKIPPED_ALIVE_CHANNEL_NMPARA
                                  NMOSEK_TRANSMIT_SKIPPED_ALIVE_REGULARPARA_TRUE); /* ESCAN00054321 */
    }

    /* - #30 On Ring Message Reception,
     *     * restart the TTyp timer (if this node is addressed)
     *     * start TMax timer and change to the waiting state (otherwise)
     *     * cancel the Synchronization Point Timer
     */
    if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_RING_MSG_RECV_FLAG_MASK) != 0u)
    {
      if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_IM_ADDRESSED_FLAG_MASK) != 0u)
      {
        /* Refresh timer */
        NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
          NmOsek_GetRingTypTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));
      }
      else
      {
        /* Another node received the token => cancel own transmit token */
        NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
          NmOsek_GetRingMaxTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));

        NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_WAIT_RINGMSG);
      }

# if (NMOSEK_SYNCHRONIZINGNETWORKOFCHANNELCONFIG == STD_ON)
      /* Cancel Synchronization Timer */
      NmOsek_SetSyncPointTimer(NMOSEK_CHANNEL_IDX, 0u);
# endif
    } /* (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_RING_MSG_RECV_FLAG_MASK) != 0u */
# if (NMOSEK_SYNCHRONIZINGNETWORKOFCHANNELCONFIG == STD_ON)
    /* - #40 If the conditions for calling Nm_SynchronizationPoint() are fulfilled, call Nm_SynchronizationPoint() */
    else if (NmOsek_GetSyncPointTimer(NMOSEK_CHANNEL_IDX) > 0u)
    {
      NmOsek_DecSyncPointTimer(NMOSEK_CHANNEL_IDX);
      if (NmOsek_GetSyncPointTimer(NMOSEK_CHANNEL_IDX) == 0u)
      {
        Nm_SynchronizationPoint(NMOSEK_NMTOSYSTEM_CHANNEL_IND);
      }
    }
    else
    {
      /* Intentionally left empty */
    }
# else
    else
# endif
    {
      /* - #50 Otherwise, if the Timeout Timer has elapsed, build RING message and set Tx Request flag */
      if (NmOsek_GetTimeoutTimer(NMOSEK_CHANNEL_IDX) == 0u)
      {
        NmOsek_MainFunction_Aux_TimeoutTimerElapsed_NmActive_ActionWaitSendtoken(NMOSEK_CHANNEL_PARA_ONLY);
      }
    }
  }
# if (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)
  else
# else
  /* There is no else branch (NmPassive cannot be reached) */
# endif /* (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON) */
#endif /* (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) */
  /* - #60 Otherwise, if the NmActive flag is not set: */
#if ((NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_ON) || (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON))
  {
    /* NmPassive */

    /* - #70 Set the Prep Sleep flag according to the Bus Sleep Ind flag */
# if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
    /* ESCAN00049873 */
    if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK) != 0u)
# endif
    {
      NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
        (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK));
    }
# if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
    /* ESCAN00049873 */
    else
    {
      NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
        (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_PREP_SLEEP_INV_MASK));
    }
# endif

# if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
    /* Remote sleep indication is not defined in passive mode */
# endif

    /* - #80 There is no TX is passive mode => Start TMax timer and change to waiting state */
    NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
      NmOsek_GetRingMaxTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));

    NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_WAIT_RINGMSG);
  }
#endif /* ((NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_ON) || (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)) */
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
 * NmOsek_MainFunction_Aux_NmActive_ActionWaitRingmsg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_Aux_NmActive_ActionWaitRingmsg(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */

  /* - #10 If the node has been skipped in the logical ring: */
  if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_IM_SKIPPED_OVER_FLAG_MASK) != 0u)
  {
    /*
     * Code: don't change the counter.
     *       The own NM message might be transmitted now or by the next task call.
     */

    /* - #20 Transmit SKIPPED ALIVE message */
    NmOsek_TransmitSkippedAlive(NMOSEK_TRANSMIT_SKIPPED_ALIVE_CHANNEL_NMPARA
                                NMOSEK_TRANSMIT_SKIPPED_ALIVE_REGULARPARA_TRUE); /* ESCAN00054321 */
  }

# if (NMOSEK_SYNCHRONIZINGNETWORKOFCHANNELCONFIG == STD_ON)
  if (NmOsek_GetSyncPointTimer(NMOSEK_CHANNEL_IDX) > 0u)
  {
    NmOsek_DecSyncPointTimer(NMOSEK_CHANNEL_IDX);
    if (NmOsek_GetSyncPointTimer(NMOSEK_CHANNEL_IDX) == 0u)
    {
      Nm_SynchronizationPoint(NMOSEK_NMTOSYSTEM_CHANNEL_IND);
    }
  }
# endif

}
#endif

#if ((NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON) || (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_ON))
/**********************************************************************************************************************
 * NmOsek_MainFunction_Aux_NmPassive_ActionWaitRingmsg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_Aux_NmPassive_ActionWaitRingmsg(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */

  /* - #10 If the logical ring is pre-stable: */
  if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_RING_PRE_STABLE_FLAG_MASK) != 0u)
  {
    /* - #20 Consider logical ring as stable */
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
      (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_RING_STABLE_FLAG_MASK));

    /* - #30 set new Nm State in dependency of the current Nm state and the Bus Sleep Ind flag */
# if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
    /* ESCAN00049873 */
    if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK) == 0u)
    {
      NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_NORMAL_OPERATION);
    }
    else
# endif
    {
      NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_READY_SLEEP);
    }
  }
  /* - #40 Otherwise, consider the ring as pre-stable */
  else
  {
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
      (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_RING_PRE_STABLE_FLAG_MASK));
  }

  /* - #50 Set/Clear PrepSleep according to the Bus Sleep Ind flag */
# if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
  /* ESCAN00049873 */
  if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK) != 0u)
# endif
  {
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
      (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK));
  }
# if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
  else /* ESCAN00049873 */
  {
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
      (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_PREP_SLEEP_INV_MASK));
  }
# endif

# if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON )
  /* Remote sleep indication is not defined in passive mode */
# endif
}
#endif

/**********************************************************************************************************************
 * NmOsek_MainFunction_ActionWaitRingmsg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_ActionWaitRingmsg(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 If a RING message has been received, check whether this node is addressed and Active flag is set: */
  /* ESCAN00058055 */
  if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_RING_MSG_RECV_FLAG_MASK) != 0u)
  {
#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
# if (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)
    if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
         (NmOsek_InternalStateType)(NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK | NMOSEK_INTERNAL_STATE_IM_ADDRESSED_FLAG_MASK)) ==
         (NmOsek_InternalStateType)(NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK | NMOSEK_INTERNAL_STATE_IM_ADDRESSED_FLAG_MASK))
# else
    if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_IM_ADDRESSED_FLAG_MASK) != 0u)
        /* (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) != 0u:
         * always true
         */
# endif
    {
      /* - #20 If so, a token has been received. Start TTyp and change state to Wait Sendtoken */

      /* State transition */
      NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
        NmOsek_GetRingTypTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));

      NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_WAIT_SENDTOKEN);
    }
    else
#endif /* (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) */
    {
      /* - #30 Otherwise, restart TMax timer */
      NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
        NmOsek_GetRingMaxTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));
    }
  }

  /* - #40 If Active flag is set, build a Skipped Alive Message if this node has been skipped in the logical ring. */
#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
# if (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)
  if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) != 0u)
# else
  /* (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) != 0u:
   * always true
   */
# endif
  {
    /* NmActive */
    NmOsek_MainFunction_Aux_NmActive_ActionWaitRingmsg(NMOSEK_CHANNEL_PARA_ONLY);
  }
# if (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)
  else
# endif
#endif /* (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) */
  /* - #50 If Active flag is not set, update internal states. */
#if ((NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON) || (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_ON))
  {
    /* NmPassive */
    NmOsek_MainFunction_Aux_NmPassive_ActionWaitRingmsg(NMOSEK_CHANNEL_PARA_ONLY);
  }
#endif  /* ((NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON) || (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_ON)) */

  /* - #60 If TMax has elapsed, trigger a transition to Action Start */
  if (NmOsek_GetTimeoutTimer(NMOSEK_CHANNEL_IDX) == 0u)
  {
    /* RX timeout */

    /* State transition */
    NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_START);

    /* Directly change Nm State if possible */
    NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_REPEAT_MESSAGE);

  }
}

#if (NMOSEK_SYNCPOINTNETWORKRELEASEINTERVALOFCHANNELCONFIG == STD_ON) && (NMOSEK_FAST_BUSOFF_RECOVERY == STD_ON)
/**********************************************************************************************************************
 * NmOsek_MainFunction_Aux_ShallSyncPointBeCalled_ActionLhWaitLimphomeTx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(boolean, NMOSEK_CODE) NmOsek_MainFunction_Aux_ShallSyncPointBeCalled_ActionLhWaitLimphomeTx(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(boolean, AUTOMATIC) callSyncPoint = FALSE;
  VAR(uint16_least, AUTOMATIC) busOffCounterTimesLimpHomeShort;

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 Determine the remaining BusOff short recoveries (under the assumption that BusOff will occur subsequently
   *       due to a permanent bus disturbance.
   */
  if (((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_ERROR_FLAG_MASK) != 0u) &&
      (NmOsek_GetBusOffCounter(NMOSEK_CHANNEL_IDX) > 1u))
  {
    busOffCounterTimesLimpHomeShort = (uint16_least)
      ((uint16_least)(NmOsek_GetBusOffCounter(NMOSEK_CHANNEL_IDX) - 1u) *
        NmOsek_GetLimpHomeShortTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));
  }
  else
  {
    busOffCounterTimesLimpHomeShort = 0u;
  }

  /* - #20 If there will be short BusOff recoveries and if the timeout timer has still a value so that Sync Point can
   *       be called during the short BusOff recoveries...
   */
  if ((busOffCounterTimesLimpHomeShort > 0u) &&
      ((NmOsek_GetTimeoutTimer(NMOSEK_CHANNEL_IDX) + busOffCounterTimesLimpHomeShort) >=
        NmOsek_GetSyncPointNetworkReleaseIntervalOfChannelConfig(NMOSEK_CHANNEL_IDX)))
  {
    /* - #30 ...check if that moment is reached to call Nm_SynchronizationPoint() now. If so, call it. */
    if (NmOsek_GetTimeoutTimer(NMOSEK_CHANNEL_IDX) ==
# if (NMOSEK_SYNCPOINTINTMODLHSHORTTIMEOFCHANNELCONFIG == STD_ON)
        NmOsek_GetSyncPointIntModLHShortTimeOfChannelConfig(NMOSEK_CHANNEL_IDX)
# else
        0u
# endif
        )
    {
      callSyncPoint = TRUE;
    }
  }
  /* - #40 Otherwise, check whether Nm_SynchronizationPoint() shall be called, also eventually during short BusOff recoveries,
   *       so that NmOsek_NetworkRelease() will be called while LimpHomeTime is running. If that moment is
   *       reached to call Nm_SynchronizationPoint() now, call it.
   */
  else if (((NmOsek_GetTimeoutTimer(NMOSEK_CHANNEL_IDX) + busOffCounterTimesLimpHomeShort) %
            NmOsek_GetLimpHomeTimeOfChannelConfig(NMOSEK_CHANNEL_IDX)) ==
# if (NMOSEK_SYNCPOINTINTMODLHTIMEOFCHANNELCONFIG == STD_ON)
            NmOsek_GetSyncPointIntModLHTimeOfChannelConfig(NMOSEK_CHANNEL_IDX)
# else
            0u
# endif
            )
  {
    callSyncPoint = TRUE;
  }
  else
  {
    /* Intentionally left empty */
  }

  return callSyncPoint;
}
#endif

/**********************************************************************************************************************
 * NmOsek_MainFunction_ActionLhWaitLimphomeTx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_ActionLhWaitLimphomeTx(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */

#if (NMOSEK_SYNCHRONIZINGNETWORKOFCHANNELCONFIG == STD_ON)
  /* - #10 If the current timer value matches a moment in time to call Nm_SynchronizationPoint(), do this now */
  if ((NmOsek_IsSynchronizingNetworkOfChannelConfig(NMOSEK_CHANNEL_IDX))
# if (NMOSEK_SYNCPOINTNETWORKRELEASEINTERVALOFCHANNELCONFIG == STD_ON)
#  if (NMOSEK_FAST_BUSOFF_RECOVERY == STD_OFF)
      && (NmOsek_GetTimeoutTimer(NMOSEK_CHANNEL_IDX) ==
          NmOsek_GetSyncPointIntModLHTimeOfChannelConfig(NMOSEK_CHANNEL_IDX))
#  endif
# else
      && (NmOsek_GetTimeoutTimer(NMOSEK_CHANNEL_IDX) == 0u)
# endif
     )
  {
# if (NMOSEK_SYNCPOINTNETWORKRELEASEINTERVALOFCHANNELCONFIG == STD_ON) && (NMOSEK_FAST_BUSOFF_RECOVERY == STD_ON)
    if (NmOsek_MainFunction_Aux_ShallSyncPointBeCalled_ActionLhWaitLimphomeTx(NMOSEK_CHANNEL_PARA_ONLY) != FALSE)
# endif
    {
      Nm_SynchronizationPoint(NMOSEK_NMTOSYSTEM_CHANNEL_IND);
    }
  }
#endif

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
  /* - #20 If TError has elapsed and there is no BusOff recovery pending: */
  if ((NmOsek_GetTimeoutTimer(NMOSEK_CHANNEL_IDX) == 0u)
# if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
      /* One of these two conditions has to be fulfilled:
       * first:
       * - (no BusOff or there is an application sleep request: BusError == 0 || BusSleepInd == 1)
       * second:
       * - CanSM notified us about a BusOff recovery
       */ /* ESCAN00057421 */
      && (((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
            (NmOsek_InternalStateType)(NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK | NMOSEK_INTERNAL_STATE_BUS_ERROR_FLAG_MASK)) !=
           NMOSEK_INTERNAL_STATE_BUS_ERROR_FLAG_MASK) ||
      (!NmOsek_IsCanIsBusOff(NMOSEK_CHANNEL_IDX)))
# endif
     )
  {
# if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
    if (!NmOsek_IsCanIsBusOff(NMOSEK_CHANNEL_IDX))
    { /* ESCAN00057421 */
      NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
        (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_ERROR_INV_MASK));
    }
# endif

    /* No BusOff recovery, this is task of the CanSM */

    /* - #30 If the Active flag is set, build the LIMPHOME message, set the TX Request flag, restart TError */
# if (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)
    if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) != 0u)
# else
    /* ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) != 0u):
     * always true
     */
# endif
    {
      /* Transmit LIMPHOME message */

      if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
           NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK) != 0u)
      {
        NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
          (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK));

        /* Set LimpHome in OpCode, Set Sleep.ind = 1,Sleep.ack = 0 */ /* ESCAN00049910, ESCAN00066879 */
        NmOsek_UpdateTxMessageData(NMOSEK_CHANNEL_PARA_FIRST NmOsek_GetLogicalSuccessor(NMOSEK_CHANNEL_IDX),
          NMOSEK_LIMPHOME, NMOSEK_SLEEP_IND);
      }
      else
      {
        NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
          (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_PREP_SLEEP_INV_MASK));

        /* Set LimpHome in OpCode, Set Sleep.ind = 0, Sleep.ack = 0 in TX Message */ /* ESCAN00049910 */
        NmOsek_UpdateTxMessageData(NMOSEK_CHANNEL_PARA_FIRST NmOsek_GetLogicalSuccessor(NMOSEK_CHANNEL_IDX),
          NMOSEK_LIMPHOME, 0u);
      }

      /* Wait for NmOsek_TxConfirmation() */
      NmOsek_SetTxRequest(NMOSEK_CHANNEL_IDX, TRUE); /* Request transmission of LimpHome message */

      NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_REPEAT_MESSAGE);

      NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, NMOSEK_TX_CNT_STEP); /* Set TX counter as limphome mark */

# if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
      /* Remote sleep indication depends only on received sleep indication flags */
# endif

      /* Build rest of LIMPHOME message */ /* ESCAN00049910 */

# if (NMOSEK_AWAKETXUSERDATAPDUIDOFPBCHANNELCONFIG == STD_ON)
#  if (NMOSEK_ONLY_VALID_USER_DATA_TX_PDU_IDS == STD_OFF)
      /* ESCAN00090119 */
      if (NmOsek_GetAwakeTxUserDataPduIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX) != NMOSEK_NO_AWAKETXUSERDATAPDUIDOFPBCHANNELCONFIG)
#  endif
      {
        {
          NmOsek_GetUserDataFromPduR(NMOSEK_CHANNEL_PARA_FIRST
            NmOsek_GetAwakeTxUserDataPduIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX));
        }
      }
# endif
    }

    NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
      NmOsek_GetLimpHomeTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));
  }
#else
  /* No need to check whether the TError timer has elapsed, LHPrepSleep is entered below anyway.
   * Bus Error flag is already cleared by NmOsek_MainFunction_BusOffRecovery()
   */ /* ESCAN00084494 */
#endif /* (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) */

  /* - #40 If the Active Flag is not set, set Prep Sleep according to Bus Sleep Ind */
#if ((NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_ON) || (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON))
# if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) /* => NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON */
  if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) == 0u)
# else
  /* ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) == 0u):
   * always true
   */
# endif
  {
    /* NmPassive */
# if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
    /* ESCAN00049873 */
    if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK) != 0u)
# endif
    {
      NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
        (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK));
    }
# if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
    /* ESCAN00049873 */
    else
    {
      NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
        (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_PREP_SLEEP_INV_MASK));
    }
# endif

# if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
    /* remote sleep indication is not defined in passive mode */
# endif

  }
#else /* if ((NMOSEK_NM_PASSIVE_MODE_ENABLED != STD_ON) && (NMOSEK_NM_COM_CONTROL_ENABLED != STD_ON)) */
  /* ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) == 0u):
   * always false
   */
#endif  /* ((NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_ON) || (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)) */

  /* - #50 If the Prep Sleep flag is set, transit to NMLimpHomePrepSleep and start TMax */
#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
  if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK) != 0u)
#else
  /* Condition is always true since the Prep Sleep Flag is set above */
#endif
  {
    /* State transition (time-critical) */
    NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
      NmOsek_GetRingMaxTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));
    NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_LH_PREPARE_SLEEP_WITH_REPEAT);

    /* Transition to Ready Sleep if possible */
    NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_READY_SLEEP);
  } /* ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK) != 0u) */
} /* PRQA S 6010,6030 */ /* MD_MSR_STPTH,MD_MSR_STCYC */

/**********************************************************************************************************************
 * NmOsek_MainFunction_ActionLhPrepareSleep()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_ActionLhPrepareSleep(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) || (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
  /* - #10 If the Prep Sleep flag is set: */
  if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK) != 0u)
#endif
  {
    /* - #20 If a RING message has been received, restart TMax */
#if (NMOSEK_RESET_TMAX_IN_LHPREP_SLEEP_ON_RX_RING_ENABLED == STD_ON) && \
    ( (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON) || (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) )
    /* ESCAN00057650, ESCAN00084494 */
    if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_RING_MSG_RECV_FLAG_MASK) != 0u)
    {
      NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
        NmOsek_GetRingMaxTimeOfChannelConfig(NMOSEK_CHANNEL_IDX)); /* Refresh timer */
    }
    else
#endif
    /* - #30 If TMax has elapsed, transit to NMTwbsLimpHome and start T(Error)WaitBusSleep */
    if (NmOsek_GetTimeoutTimer(NMOSEK_CHANNEL_IDX) == 0u)
    {
      /* Start sleep sequence */
      NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
        (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_WAIT_BUS_SLEEP_FLAG_MASK));

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
      NmOsek_SetTxRequest(NMOSEK_CHANNEL_IDX, FALSE); /* Cancel NM TX request */
#endif

#if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
      if (!NmOsek_IsCanIsBusOff(NMOSEK_CHANNEL_IDX))
      { /* ESCAN00086725 */
        NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
          (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_ERROR_INV_MASK));
      }
#endif
#if (NMOSEK_FAST_BUSOFF_RECOVERY == STD_ON)
      /* ESCAN00086725 */
      NmOsek_SetBusOffCounter(NMOSEK_CHANNEL_IDX, NmOsek_GetLimpHomeShortNumberOfChannelConfig(NMOSEK_CHANNEL_IDX));
#endif
      /* No BusOff recovery is done here by NmOsek */

      /* State transition */
      NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
        NmOsek_GetWaitBusSleepTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));

      NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_GO_BUSSLEEP);

      /* Notify NM */
      NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_PREPARE_BUS_SLEEP);

      Nm_PrepareBusSleepMode(NMOSEK_NMTOSYSTEM_CHANNEL_IND);

#if (NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE == STD_ON)
      NmOsek_SetFirstTxConfOrRxInd(NMOSEK_CHANNEL_IDX, FALSE);
#endif
    }
    else
    {
      /* empty */
    }
  }
#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) || (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
  /* - #40 Otherwise: transit to NMLimpHome */
  else
  {
    /* Application wakeup */
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
      (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_NM_MSG_RECV_REMIND_INV_MASK));

# if (NMOSEK_FAST_BUSOFF_RECOVERY == STD_ON)
    if (
#  if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_ON)
        /* BusError flag is always set */
#  else
        ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_ERROR_FLAG_MASK) != 0u) &&
#  endif
        (NmOsek_GetBusOffCounter(NMOSEK_CHANNEL_IDX) > 0u))
    {
      NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
        NmOsek_GetLimpHomeShortTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));
    }
    else
# endif
    {
      NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
        NmOsek_GetLimpHomeTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));
    }
    NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_LH_WAIT_LIMPHOME_TX);
    NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_REPEAT_MESSAGE);
  }
#endif
}

/**********************************************************************************************************************
 * NmOsek_MainFunction_ActionsLhWaitLimphomeTxAndLhPrepareSleep()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_ActionsLhWaitLimphomeTxAndLhPrepareSleep(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 Reset node configuration */
  NmOsek_ResetNodeConfig(NMOSEK_CHANNEL_PARA_ONLY);

  /* - #20 Set LimpHome Flag and reset NM RX counter */
  NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
    (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_LIMP_HOME_FLAG_MASK));

  /* Reset NM RX counter */
  NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, (NmOsek_RxTxCntType)
    (NmOsek_GetRxTxCnt(NMOSEK_CHANNEL_IDX) & NMOSEK_RX_CNT_INV_MASK));

#if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* - #30 Set Remote Sleep state */
  /*
   * If a gateway is in limphome, other networks should not be hindered
   * to enter sleep mode.
   */
  NmOsek_ChangeRemoteSleepState(NMOSEK_CHANNEL_PARA_FIRST NMOSEK_REMOTE_SLEEP_STATE_SLEEP_IND);
#endif

  /* - #40 Remember if a NM message has been received while being in limphome */
  if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_NM_MSG_RECV_FLAG_MASK) != 0u)
  {
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
      (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_NM_MSG_RECV_REMIND_FLAG_MASK));
  }

  /* - #50 If a NM message has been successfully transmitted and received, leave LimpHome and enter Action Start */
  if (
#if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
      ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
        (NmOsek_InternalStateType)(NMOSEK_INTERNAL_STATE_NM_MSG_RECV_REMIND_FLAG_MASK | NMOSEK_INTERNAL_STATE_BUS_ERROR_FLAG_MASK)) ==
       NMOSEK_INTERNAL_STATE_NM_MSG_RECV_REMIND_FLAG_MASK)
#else
      ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
        NMOSEK_INTERNAL_STATE_NM_MSG_RECV_REMIND_FLAG_MASK) != 0u)
      /* ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_ERROR_FLAG_MASK) == 0u):
       * always true
       */
#endif
      /* The following condition is linked by a logical and (&&):
       * (((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) == 0u) ||
       *  (NmOsek_GetRxTxCnt(NMOSEK_CHANNEL_IDX) == 0u))
       *
       * This condition is:
       * - always true if NMOSEK_NM_PASSIVE_MODE_ENABLED != STD_OFF and omitted in this case
       * - depends only on (NmOsek_GetRxTxCnt(NMOSEK_CHANNEL_IDX) == 0u) if
       *   NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF and (NMOSEK_NM_COM_CONTROL_ENABLED != STD_ON)
       */
#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
      && (
# if (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)
          ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) == 0u) ||
# else
          /* (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) == 0u:
           * always false
           */
# endif
          (NmOsek_GetRxTxCnt(NMOSEK_CHANNEL_IDX) == 0u))
#else /* if (NMOSEK_NM_PASSIVE_MODE_ENABLED != STD_OFF) */
      /* (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) == 0u:
       * always true
       */
#endif
     )
  {
    NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, 0u); /* Reset NM RX and TX counter */

    /* State transition (time-critical) */
    NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_START_WITH_REPEAT);

    /* No need to change the Nm state directly, will be done in NmOsek_MainFunction_ActionStart() */
    /* ESCAN00076015 */

  }
  /* - #60 Otherwise, handle current state */
 /*----------------------------------------------------------
   NMOSEK_ACTION_LH_WAIT_LIMPHOME_TX
  *----------------------------------------------------------*/
  else if (NmOsek_GetAction(NMOSEK_CHANNEL_IDX) == NMOSEK_ACTION_LH_WAIT_LIMPHOME_TX)
  {
    NmOsek_MainFunction_ActionLhWaitLimphomeTx(NMOSEK_CHANNEL_PARA_ONLY);
  } /* (NmOsek_GetAction(NMOSEK_CHANNEL_IDX) == NMOSEK_ACTION_LH_WAIT_LIMPHOME_TX) */
  /*----------------------------------------------------------
    NM_ACTION_LH_PREPARE_SLEEP
   *----------------------------------------------------------*/
  else
  {
    NmOsek_MainFunction_ActionLhPrepareSleep(NMOSEK_CHANNEL_PARA_ONLY);
  }
}

/**********************************************************************************************************************
 * NmOsek_MainFunction_ActionGoBussleep()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_ActionGoBussleep(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check whether Wait Bus Sleep Flag is set */
  if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_WAIT_BUS_SLEEP_FLAG_MASK) == 0u)
  {
    errorId = NMOSEK_E_WRONG_WAIT_BUS_SLEEP_FLAG;
  }
  /* - #20 Check whether Bus Sleep Flag is not set */
  else if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_SLEEP_FLAG_MASK) != 0u)
  {
    errorId = NMOSEK_E_WRONG_BUS_SLEEP_FLAG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    /* - #30 Reset the node configuration */
    NmOsek_ResetNodeConfig(NMOSEK_CHANNEL_PARA_ONLY);

    /* - #40 Clear the Nm Message Received Remind flag and reset RX/TX counter */
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
      (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_NM_MSG_RECV_REMIND_INV_MASK));

    /* Reset RX/TX counter */
    NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, 0u);

    /* - #50 If the Prep Sleep flag is no longer set, leave NMTwbs* to NMReset/NMLimpHome */
    if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK) == 0u)
    {
      /*
       * Sleep condition not true anymore
       * => - NmOsek_NetworkRequest() called or
       *    - RX of NM message with SleepInd=0 *
       * => abort sleep sequence
       */

      /* Leave the context because of possible wake-up detection in NmOsek_NetworkRequest() */
      NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
        (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_WAIT_BUS_SLEEP_INV_MASK));

      /* Check if LimpHome was active before the sleep sequence */
      if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_LIMP_HOME_FLAG_MASK) != 0u)
      {
        NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, NMOSEK_TX_CNT_STEP); /* Use TX counter as limphome mark */

        /* Update time counter */
#if (NMOSEK_FAST_BUSOFF_RECOVERY == STD_ON)
        if (((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_ERROR_FLAG_MASK) != 0u)
# if (NMOSEK_ALL_LIMP_HOME_SHORT_NUMBER_GREATER_THAN_ONE == STD_OFF)
            && (NmOsek_GetBusOffCounter(NMOSEK_CHANNEL_IDX) > 0u) /* ESCAN00086725 */
# endif
           )
        {
          NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
            NmOsek_GetLimpHomeShortTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));
        }
        else
#endif
        {
          /* ESCAN00086725 */
          NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
            NmOsek_GetLimpHomeTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));
        }

        /* State transition */
        NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_LH_WAIT_LIMPHOME_TX);

#if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
        if (NmOsek_IsCanIsBusOff(NMOSEK_CHANNEL_IDX))
        {
          NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_BUS_OFF);
        }
        else
#endif
        {
          NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_REPEAT_MESSAGE);
        }
      }
      else /* if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
            *      NMOSEK_INTERNAL_STATE_LIMP_HOME_FLAG_MASK) == 0u)
            */
      {

        /* State transition (time-critical) */
        NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_START_WITH_REPEAT);
        /* No need to change the Nm state directly, will be done in NmOsek_MainFunction_ActionStart() */
        /* ESCAN00076015 */
      }

#if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
      NmOsek_ChangeRemoteSleepState(NMOSEK_CHANNEL_PARA_FIRST NMOSEK_REMOTE_SLEEP_STATE_NO_SLEEP);
#endif

      /* Notify NM that NMTwbsNormal/NMTwbsLimpHome is left */
      Nm_NetworkMode(NMOSEK_NMTOSYSTEM_CHANNEL_IND);
    }
    /* - #60 Otherwise, check whether T(Error)WaitBusSleep has elapsed. If so, enter NMBusSleep. */
    else /* if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
                 NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK) != 0u) */
    {
      /* Sleep condition is still true */

      if (NmOsek_GetTimeoutTimer(NMOSEK_CHANNEL_IDX) == 1u)
      {
#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
        NmOsek_SetTxRequest(NMOSEK_CHANNEL_IDX, FALSE); /* Cancel any NM TX request */
#endif
      }
      else if (NmOsek_GetTimeoutTimer(NMOSEK_CHANNEL_IDX) == 0u)
      {
        /* ESCAN00052619 */

        /* Clear WaitBusSleep flag */
        NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
          (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_WAIT_BUS_SLEEP_INV_MASK));

        /* Set BusSleep flag */
        NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
          (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_BUS_SLEEP_FLAG_MASK));

        NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_BUS_SLEEP);

        /* Wait Bus Sleep Extension from LimpHome was removed */ /* ESCAN00062285 */
        /* Notify NM */
        Nm_BusSleepMode(NMOSEK_NMTOSYSTEM_CHANNEL_IND);

        /* State transition */
        NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_BUS_SLEEP);
      }
      else
      {
        /* empty */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_MAINFUNCTION, errorId);
  }
#else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
} /* PRQA S 6030,6050,6080 */ /* MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/**********************************************************************************************************************
 * NmOsek_MainFunction_ActionBusSleep()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_ActionBusSleep(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check whether Wait Bus Sleep Flag is not set */
  if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_WAIT_BUS_SLEEP_FLAG_MASK) != 0u)
  {
    errorId = NMOSEK_E_WRONG_WAIT_BUS_SLEEP_FLAG;
  }
  /* - #20 Check whether Bus Sleep Flag is set */
  else if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_SLEEP_FLAG_MASK) == 0u)
  {
    errorId = NMOSEK_E_WRONG_BUS_SLEEP_FLAG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */

    /* - #30 Clear LimpHome flag */
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
      (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_LIMP_HOME_INV_MASK));

    /* - #40 If the PrepSleep flag is not set: transit to NMReset */
    if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK) == 0u)
    {

      {
        {
          /*
           * Sleep condition not true anymore
           * => - NmOsek_NetworkRequest() called or
           *    - RX of any NM message * (in NMOSEK_ACTION_BUS_SLEEP)
           * => abort sleep
           */
          NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
            (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_SLEEP_INV_MASK));

          /* Notify Nm about external wakeup, i.e. local sleep state is still set */
#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
          if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & /* ESCAN00049873 */
               NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK) != 0u)
#endif
          {
            /* Nm will call NmOsek_PassiveStartUp() later on */
            Nm_NetworkStartIndication(NMOSEK_NMTOSYSTEM_CHANNEL_IND);
          }

          /* State transition */
          NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_START);

          /* Directly change Nm State if possible to make sure that the NmState is set before Network Mode is entered */
          NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_REPEAT_MESSAGE);

          /* Inform application to enable transceiver and to wake CAN controller */
          Nm_NetworkMode(NMOSEK_NMTOSYSTEM_CHANNEL_IND);
        }
      }
    }

    /* ESCAN00062285 */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_MAINFUNCTION, errorId);
  }
#else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */

/**********************************************************************************************************************
 * NmOsek_MainFunction_NoAction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_NoAction(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 Reset the node configuration */
  NmOsek_ResetNodeConfig(NMOSEK_CHANNEL_PARA_ONLY);

  /* - #20 Reset all status flags */
  NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, 0u);

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
  NmOsek_SetTxRequest(NMOSEK_CHANNEL_IDX, FALSE);
#endif

  NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX, 0u);
  NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, 0u);

#if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /* - #30 Clear Remote Sleep State */
  NmOsek_ChangeRemoteSleepState(NMOSEK_CHANNEL_PARA_FIRST NMOSEK_REMOTE_SLEEP_STATE_NO_SLEEP);
#endif
}

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
 * NmOsek_MainFunction_IssueTxRequestIfDemandedAndPossible()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_MainFunction_IssueTxRequestIfDemandedAndPossible(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(PduInfoType, AUTOMATIC) txBuffer;

  /* ----- Implementation ----------------------------------------------- */
  /* - #10 Check whether there is a transmission request and transmission is possible */
  if ((NmOsek_IsTxRequest(NMOSEK_CHANNEL_IDX))
# if (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)
      && ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) != 0u)
# else
      /* (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) != 0u:
       * always true
       */
# endif
# if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
      /* ESCAN00057421 */
      && (!NmOsek_IsCanIsBusOff(NMOSEK_CHANNEL_IDX))
# endif
     )
  {
# if (NMOSEK_TX_DEADLINE_MONITORING_IN_COM_ENABLED == STD_ON)
#  if (NMOSEK_AWAKETXUSERDATACOMIPDUIDOFPBCHANNELCONFIG == STD_ON)
    VAR(uint8, AUTOMATIC) opCode =
      (uint8)(NmOsek_GetTxMessageData(NMOSEK_MESSAGE_DATA_BYTE_INDEX(NMOSEK_CHANNEL_IDX, NMOSEK_CFG_PDU_OPCODE_POSITION)) & NMOSEK_OPCODE_CMD_MASK);
#  endif
# endif
    /* Update local PduInfoType variable */
    txBuffer.SduDataPtr = &NmOsek_GetTxMessageData(NMOSEK_MESSAGE_DATA_FIRST_BYTE(NMOSEK_CHANNEL_IDX));
    txBuffer.SduLength = NmOsek_GetPduLengthOfPbChannelConfig(NMOSEK_CHANNEL_IDX);

# if (NMOSEK_TX_DEADLINE_MONITORING_IN_COM_ENABLED == STD_ON)
#  if (NMOSEK_AWAKETXUSERDATACOMIPDUIDOFPBCHANNELCONFIG == STD_ON) 
    if (opCode != NMOSEK_ALIVE)
    {
      Com_TriggerIPDUSend(NmOsek_GetAwakeTxUserDataComIpduIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX));
    }
    else
#  endif
    {
      Com_TriggerIPDUSend(NmOsek_GetWakeUpTxUserDataComIpduIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX));
    }
# endif

    /* - #20 Issue the transmission request and if it was successful: */
    if (CanIf_Transmit(NmOsek_GetTxPduIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX), &txBuffer) == E_OK)
    {
      /* - #30 Clear the TX Request flag. */
      NmOsek_SetTxRequest(NMOSEK_CHANNEL_IDX, FALSE);

# if (NMOSEK_MSGTIMEOUTTIMER == STD_ON)
      NmOsek_SetMsgTimeoutTimer(NMOSEK_CHANNEL_IDX, (NmOsek_MsgTimeoutTimerType)
                                (NmOsek_GetTxConfirmationTimeoutTimeOfChannelConfig(NMOSEK_CHANNEL_IDX)));
# endif

    }
  }
}
#endif

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)

/**********************************************************************************************************************
 * NmOsek_TxConfirmation_HandleRingMessage()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_TxConfirmation_HandleRingMessage(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* - #10 If PrepSleep == 1 && Sleep.ack = 1 has been sent, transit to NMTwbsNormal */
  if (((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK) != 0u) &&
      ((NmOsek_GetTxMessageData(NMOSEK_MESSAGE_DATA_BYTE_INDEX(NMOSEK_CHANNEL_IDX,
                                                               NMOSEK_CFG_PDU_SLEEP_POSITION)) &
       NMOSEK_OPCODE_SLEEP_MASK) == NMOSEK_OPCODE_SLEEP_MASK)) /* ESCAN00049910 */ /* ESCAN00049894 */
  {
    /* State transition to WaitBusSleep */
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
      (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_WAIT_BUS_SLEEP_FLAG_MASK));
    NmOsek_SetTxRequest(NMOSEK_CHANNEL_IDX, FALSE); /* cancel NM TX request, RxTxLockUd is already 0 */

    NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
      NmOsek_GetWaitBusSleepTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));

    NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_GO_BUSSLEEP);

    /* ESCAN00050123 */
    if (NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) != NM_STATE_PREPARE_BUS_SLEEP)
    {
      /* Notify NM */
      NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_PREPARE_BUS_SLEEP);

      Nm_PrepareBusSleepMode(NMOSEK_NMTOSYSTEM_CHANNEL_IND);

# if (NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE == STD_ON)
      NmOsek_SetFirstTxConfOrRxInd(NMOSEK_CHANNEL_IDX, FALSE);
# endif
    }
  }
  /* - #20 If Sleep Indication bit = 1 has been sent, set Prep Sleep flag */
  else
  {
    /* Sleep.ind == 1 */
    if (((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
        /* Message reception may have changed the PrepSleepFlag */ /* ESCAN00049894 */
          (NmOsek_InternalStateType)(NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK | NMOSEK_INTERNAL_STATE_NM_MSG_RECV_FLAG_MASK)) ==
          NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK) &&
        ((NmOsek_GetTxMessageData(NMOSEK_MESSAGE_DATA_BYTE_INDEX(NMOSEK_CHANNEL_IDX, NMOSEK_CFG_PDU_SLEEP_POSITION)) &
         NMOSEK_OPCODE_SLEEP_MASK) == NMOSEK_SLEEP_IND)) /* ESCAN00049910 */ /* ESCAN00049894 */
    {
      NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
        (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK));
    }

# if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
    /* Remote sleep indication depends only on received sleep indication flags */
# endif
  }

  /*
    Do not wait for any Ring message if we are currently go to sleep.
    This could happen, if we receive a Sleep Acknowledge (NmOsek_RxIndication()) while waiting for
    our own Confirmation for the Sleep Indication message.
    Then, this last re-trigger and state transition could be discarded.
    Leave the ImAddressed and PrepSleep information too.
    These flags have also got their latest update in NmOsek_RxIndication().
  */
  /* - #30 If not in NMTwbsNormal/NMTwbsLimpHome, restart TMax and transit to Action Wait Ringmsg */
  if (NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) != NM_STATE_PREPARE_BUS_SLEEP)
  {
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
      (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
       (NmOsek_InternalStateType)(NMOSEK_INTERNAL_STATE_IM_ADDRESSED_INV_MASK & NMOSEK_INTERNAL_STATE_LIMP_HOME_INV_MASK)));

    /* Re-trigger timer to avoid duplicate rings */
    NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
      NmOsek_GetRingMaxTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));
    NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_WAIT_RINGMSG);
  }

}

/**********************************************************************************************************************
 * NmOsek_TxConfirmation_HandleAliveMessage()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_TxConfirmation_HandleAliveMessage(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* - #10 If the current state NMLimpHome: */
  if ((NmOsek_GetAction(NMOSEK_CHANNEL_IDX) == NMOSEK_ACTION_LH_WAIT_LIMPHOME_TX) ||
      (NmOsek_GetAction(NMOSEK_CHANNEL_IDX) == NMOSEK_ACTION_LH_PREPARE_SLEEP))
  {
    /* - #20 Set TX counter, use as Limphome TX mark, reset only on limphome msg */
    NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, NMOSEK_TX_CNT_STEP);

  }
}

/**********************************************************************************************************************
 * NmOsek_TxConfirmation_HandleLimpHomeMessage()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_TxConfirmation_HandleLimpHomeMessage(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* - #10 In dependency of the configuration, LimpHome might be left if a NM message has also been received */
# if (NMOSEK_LEAVE_LHONLY_ON_RX_INDICATION_ENABLED == STD_ON)
  /* LimpHome is not left on TxConfirmation, only later if a message is received */ /* ESCAN00058056 */

  /* Clear Msg Recv flag and Msg Recv Remind flag to prevent the next NmOsek_MainFunction call
   * to leave LimpHome if a message has previously been received
   */ /* ESCAN00072460 */
  NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
    (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
     NMOSEK_INTERNAL_STATE_NM_STATUS_NM_MSG_RECV_FLAG_NM_MSG_RECV_REMIND_FLAG_INV_MASK));
# else
  if (
#  if (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)
      ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) != 0u) &&
#  else
      /* (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) != 0u:
       * always true
       */
#  endif
      ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
        NMOSEK_INTERNAL_STATE_NM_MSG_RECV_REMIND_FLAG_MASK) != 0u))
  {
    NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_START);

    /* Directly change the Nm State if possible */
    NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_REPEAT_MESSAGE);

  }
  /* ESCAN00057421 */
  else
# endif
  if (NmOsek_GetAction(NMOSEK_CHANNEL_IDX) == NMOSEK_ACTION_LH_PREPARE_SLEEP)
  {
    /* - #20 Otherwise re-trigger TMax for successful transmission in NMLimpHomePrepSleep */
    NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
      NmOsek_GetRingMaxTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));
  }
  else
  {
    /* Empty */
  }
}

# if (NMOSEK_CONFIRMTXUSERDATAPDUOFPBCHANNELCONFIG == STD_ON)
/**********************************************************************************************************************
 * NmOsek_TxConfirmation_NotifyPduR()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_TxConfirmation_NotifyPduR(NMOSEK_CHANNEL_NMTYPE_FIRST CONST(uint8, AUTOMATIC) opCode)
{
  /* - #10 Confirm user data PDU transmission if required by PduR Routing Path */
  if ((NmOsek_IsConfirmTxUserDataPduOfPbChannelConfig(NMOSEK_CHANNEL_IDX))
     )
  {
    if (opCode == NMOSEK_ALIVE)
    {
      PduR_NmOsekTxConfirmation(NmOsek_GetWakeUpTxUserDataPduIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX));
    }
#  if !defined(NMOSEK_TYPE_VECTOR2_OSEK)
    else if ((opCode == NMOSEK_RING) || (opCode == NMOSEK_LIMPHOME))
#  else
    else
#  endif
    {
      PduR_NmOsekTxConfirmation(NmOsek_GetAwakeTxUserDataPduIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX));
    }
#  if !defined(NMOSEK_TYPE_VECTOR2_OSEK)
    else
    {
      /* Intentionally left empty */
    }
#  endif
  }
}
# endif
#endif

/**********************************************************************************************************************
 * NmOsek_RxIndication_DetermineWhetherMessageIsValidAndUpdateStatusFlags()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(boolean, NMOSEK_CODE) NmOsek_RxIndication_DetermineWhetherMessageIsValidAndUpdateStatusFlags(NMOSEK_CHANNEL_NMTYPE_FIRST
  VAR(uint8, AUTOMATIC) sourceAddress, VAR(uint8, AUTOMATIC) destAddress, VAR(uint8, AUTOMATIC) opCode)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (NMOSEK_NMRESET_IGNORE_RX_EXCEPT_ALIVE_ENABLED == STD_ON)
  VAR(NmOsek_ActionType, AUTOMATIC) nmActionWithoutRepeatFlag;
#endif

  VAR(boolean, AUTOMATIC) validNmMsgReceived;
#if (NMOSEK_USE_NM_NODE_LIST == STD_ON)
  VAR(uint8, AUTOMATIC) configByte;
#endif

  /* ----- Implementation ----------------------------------------------- */
#if (NMOSEK_NMRESET_IGNORE_RX_EXCEPT_ALIVE_ENABLED == STD_ON)
  nmActionWithoutRepeatFlag = (NmOsek_ActionType)(NmOsek_GetAction(NMOSEK_CHANNEL_IDX) & NMOSEK_ACTION_REPEAT_INV);
  /* Ignore all messages except Alive Messages in NMReset */ /* ESCAN00057876 */
  if ((opCode != NMOSEK_ALIVE) &&
      ((nmActionWithoutRepeatFlag == NMOSEK_ACTION_START)
      ))
  {
    /* Ignore this message */
    validNmMsgReceived = FALSE;
  }
  else
#endif
  {
    /* - #10 process the received message in dependency of the op code */

    /* Analyze local variables */
    /* - #20 If a RING or ALIVE message has been received: */
    /*---------------------------------------------------------------------------*/
    if ((opCode == NMOSEK_RING) || (opCode == NMOSEK_ALIVE))
    /*---------------------------------------------------------------------------*/
    {
      /* - #30 Consider this message as valid one */
      validNmMsgReceived = TRUE;

#if (NMOSEK_USE_NM_NODE_LIST == STD_ON)
      /* - #40 Update NM Node Table when RING/ALIVE message has been received */
      /* Calculate the index for the config table */ /* ESCAN00084398 */
      configByte = NMOSEK_CALCULATE_CONFIG_BYTE((uint8)(sourceAddress & (uint8)(NMOSEK_NUMBER_OF_NM_CONFIG_NODES - 1u)));

      /* Set bit of received NM node within NM node table */
      NmOsek_GetNodeConfiguration().nmConfigChar[configByte] |=
          (uint8)(1u << (uint8)(sourceAddress & (uint8)NMOSEK_BYTE_OFFSET_MASK));
#endif

      /* - #50 Determine Logical Successor */
      /* \trace CREQ-119842 */
#if (NMOSEK_NODE_ID_OPTIMIZATION != NMOSEK_HIGHEST_NODE_ID_IN_RANGE)
      if (NmOsek_GetLogicalSuccessor(NMOSEK_CHANNEL_IDX) > NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX))
      {
        if ((sourceAddress < NmOsek_GetLogicalSuccessor(NMOSEK_CHANNEL_IDX)) &&
            (sourceAddress > NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX)))
        {
          NmOsek_SetLogicalSuccessor(NMOSEK_CHANNEL_IDX, sourceAddress);
        }
      }
      else
#endif
      {
        if (
#if (NMOSEK_NODE_ID_OPTIMIZATION != NMOSEK_LOWEST_NODE_ID_IN_RANGE)
            (sourceAddress < NmOsek_GetLogicalSuccessor(NMOSEK_CHANNEL_IDX))
# if (NMOSEK_NODE_ID_OPTIMIZATION != NMOSEK_HIGHEST_NODE_ID_IN_RANGE)
            ||
# endif
#endif
#if (NMOSEK_NODE_ID_OPTIMIZATION != NMOSEK_HIGHEST_NODE_ID_IN_RANGE)
            (sourceAddress > NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX))
#endif
           )
        {
          NmOsek_SetLogicalSuccessor(NMOSEK_CHANNEL_IDX, sourceAddress);
        }
      }

      /* - #60 If the message is a RING message: */
      /*---------------------------------------------------------------------------*/
      if (opCode == NMOSEK_RING) /* RX RING */
      /*---------------------------------------------------------------------------*/
      {
        /* - #70 Set the Ring Msg Recv flag */
        NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
          (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_RING_MSG_RECV_FLAG_MASK));

        /* - #80 Check if own node got the token and set/clear I'm addressed flag and set Skipped flag in dependency
         *       of the addressed node
         */
        if (destAddress == NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX)) /* Own node addressed */
        {
          /* I'm addressed */
          NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
            (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_IM_ADDRESSED_FLAG_MASK));

#if (NMOSEK_TOKEN_MONITORING == STD_ON)
          /* Own node has the token => allow ring data access */ /* ESCAN00052621 */
          NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
            (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_TX_RING_DATA_ALLOW_INV_MASK));
#endif
        }
        else if (destAddress == sourceAddress) /* Sender addressed */
        {
          /* I'm addressed */
          NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
            (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_IM_ADDRESSED_FLAG_MASK));
        }
#if (NMOSEK_NODE_ID_OPTIMIZATION != NMOSEK_LOWEST_NODE_ID_IN_RANGE)
        else if (destAddress < NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX))
        {
          /* I'm not addressed */
          NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
            (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_IM_ADDRESSED_INV_MASK));

          if ((sourceAddress >= destAddress) &&
              (sourceAddress < NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX)))
          {
            /* I'm skipped over */
            NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
              (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_IM_SKIPPED_OVER_FLAG_MASK));
          }
        }
#endif
        else if (
#if (NMOSEK_NODE_ID_OPTIMIZATION != NMOSEK_HIGHEST_NODE_ID_IN_RANGE)
                 (sourceAddress > destAddress)
# if (NMOSEK_NODE_ID_OPTIMIZATION != NMOSEK_LOWEST_NODE_ID_IN_RANGE)
                 ||
# endif
#endif
#if (NMOSEK_NODE_ID_OPTIMIZATION != NMOSEK_LOWEST_NODE_ID_IN_RANGE)
                 (sourceAddress < NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX))
#endif
                )
        {
          /* I'm not addressed */
          NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
            (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_IM_ADDRESSED_INV_MASK));

          /* I'm skipped over */
          NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
            (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_IM_SKIPPED_OVER_FLAG_MASK));
        }
        else
        {
          /* I'm not addressed */
          NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
            (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_IM_ADDRESSED_INV_MASK));
        }
      } /* (opCode == NMOSEK_RING) */
      /* - #90 Otherwise, if the received message is an ALIVE message: clear Ring Stable and Ring Pre Stable flags and
       *       update Nm State if required
       */
      else /* if (opCode != NMOSEK_RING) */
      {
        /* RX ALIVE */
        NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
          (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
           (NmOsek_InternalStateType)(NMOSEK_INTERNAL_STATE_RING_STABLE_INV_MASK & NMOSEK_INTERNAL_STATE_RING_PRE_STABLE_INV_MASK)));
#if (NMOSEK_SYNCHRONIZINGNETWORKOFCHANNELCONFIG == STD_ON)
        NmOsek_SetRingCycleTimer(NMOSEK_CHANNEL_IDX, 0u);
        NmOsek_SetSyncPointTimer(NMOSEK_CHANNEL_IDX, 0u);
#endif

        if (
#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
            /* ESCAN00049873 */
            (NmOsek_GetDesiredOrActualNmState(NMOSEK_CHANNEL_IDX) == NM_STATE_NORMAL_OPERATION) ||
#endif
            (NmOsek_GetDesiredOrActualNmState(NMOSEK_CHANNEL_IDX) == NM_STATE_READY_SLEEP)
           )
        {
          NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_REPEAT_MESSAGE);
        }
        else
        {
          /* Repeat Message: no state change
           * Other states: state change in NmOsek_MainFunction
           */
        }

      }
    } /* ((opCode == NMOSEK_RING) || (opCode == NMOSEK_ALIVE)) */
    /* - #100 If the received message is a LimpHome or Wake-up Message consider the message as a valid one */
    else
#if defined(NMOSEK_TYPE_VECTOR2_OSEK)
#else
    if ((opCode == NMOSEK_LIMPHOME)
       )
#endif
    {
      validNmMsgReceived = TRUE;
    }
#if defined(NMOSEK_TYPE_VECTOR2_OSEK)
#else
    /* - #110 Otherwise, consider the message as invalid one */
    else
    {
      validNmMsgReceived = FALSE;
    }
#endif
  }

  return validNmMsgReceived;
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */

/**********************************************************************************************************************
 * NmOsek_RxIndication_HandleRxSleepAck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
NMOSEK_LOCAL_INLINE FUNC(void, NMOSEK_CODE) NmOsek_RxIndication_HandleRxSleepAck(NMOSEK_CHANNEL_NMTYPE_ONLY)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) != NM_STATE_BUS_SLEEP &&
   * NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) != NM_STATE_PREPARE_BUS_SLEEP
   */
  /* - #10 If the Nm State is neither Bus Sleep nor Prepare Bus Sleep: */
  if (NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) > NM_STATE_PREPARE_BUS_SLEEP)
  {
    /* Start transition to BusSleep */
#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
    /* - #20 If the Bus Sleep Ind flag is set: */
    /* ESCAN00049873 */
    if ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK) != 0u)
#endif
    {
      /* RX Sleep.ack accepted */

      /* - #30 Set the Prep Sleep flag and the Wait Bus Sleep flag */
      NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
        (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) |
         (NmOsek_InternalStateType)(NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK |
                                    NMOSEK_INTERNAL_STATE_WAIT_BUS_SLEEP_FLAG_MASK)));

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
      /* - #40 Cancel NM TX request */
      NmOsek_SetTxRequest(NMOSEK_CHANNEL_IDX, FALSE);
#endif

#if (NMOSEK_TOKEN_MONITORING == STD_ON)
      /* - #50 Disable user(ring)data access */
      /* ESCAN00052621 */
      NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
        (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_TX_RING_DATA_ALLOW_FLAG_MASK));
#endif

#if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
      /* - #60 Set remote sleep state to Sleep Ind */
      NmOsek_ChangeRemoteSleepState(NMOSEK_CHANNEL_PARA_FIRST NMOSEK_REMOTE_SLEEP_STATE_SLEEP_IND);
#endif

      {
        NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX,
          NmOsek_GetWaitBusSleepTimeOfChannelConfig(NMOSEK_CHANNEL_IDX));
      }
      /* - #90 Transit to Action Bus Sleep / Prepare Bus Sleep and notify Nm about state change */
      NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_GO_BUSSLEEP);

      /* ESCAN00086316 */
      /* Reset both Tx and Rx count */ /* ESCAN00085610 */
      NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, 0u);

      /* Notify NM */
      NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NM_STATE_PREPARE_BUS_SLEEP);

      Nm_PrepareBusSleepMode(NMOSEK_NMTOSYSTEM_CHANNEL_IND);

#if (NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE == STD_ON)
      NmOsek_SetFirstTxConfOrRxInd(NMOSEK_CHANNEL_IDX, FALSE);
#endif
    }
#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
    /* - #100 Otherwise: */
    else /* ESCAN00049873 */
    {
      /* RX SleepAck but sleep is not allowed */

# if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
      /* - #130 Clear Remote Sleep state (set to No Sleep) */
      NmOsek_ChangeRemoteSleepState(NMOSEK_CHANNEL_PARA_FIRST NMOSEK_REMOTE_SLEEP_STATE_NO_SLEEP);
# endif
    }
#endif /* (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) */
  }
  /* ESCAN00049894 */
  /* - #140 If the current Nm state is Prepare Bus Sleep and Prep Sleep has been cleared, set Prep Sleep again */
  else if ((NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) == NM_STATE_PREPARE_BUS_SLEEP) &&
#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
           /* ESCAN00049873 */
           ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
            (NmOsek_InternalStateType)(NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK |
                                       NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK)) ==
            NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK)
#else
           ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK) == 0u)
#endif
          )
  {
    /* Prep Sleep Flag has been cleared. Set this flag again to stay in NMTwbsNormal/NMTwbsLimpHome */
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
      (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_PREP_SLEEP_FLAG_MASK));
  }
  else
  {
    /* NM will state in sleep mode => Ignore SleepAck */
  }
}

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
 * NmOsek_SetTxUserDataBytes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
NMOSEK_LOCAL FUNC(void, NMOSEK_CODE) NmOsek_SetTxUserDataBytes(NMOSEK_CHANNEL_NMTYPE_FIRST
                                                               CONST(NmOsek_TxMessageDataType, AUTOMATIC) value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least byteIndex;

  /* ----- Implementation ----------------------------------------------- */

  /* - #10 Set each TX User Data Byte to the provided value */
  for (byteIndex = NMOSEK_USER_DATA_OFFSET;
       byteIndex < NmOsek_GetPduLengthOfPbChannelConfig(NMOSEK_CHANNEL_IDX); byteIndex++)
  {
    NmOsek_SetTxMessageData(NMOSEK_MESSAGE_DATA_BYTE_INDEX(NMOSEK_CHANNEL_IDX, byteIndex), value);
  }
}
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * NmOsek_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, NMOSEK_CODE) NmOsek_Init(P2CONST(NmOsek_ConfigType, AUTOMATIC, NMOSEK_INIT_DATA) nmOsekConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check initialization state of the component */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_INIT)
  {
    errorId = NMOSEK_E_ALREADY_INITIALIZED;
  }
  else
#endif
#if (NMOSEK_USE_INIT_POINTER == STD_ON)
  /* - #20 Check parameter 'nmOsekConfigPtr' */
  if (nmOsekConfigPtr == NULL_PTR)
  {
# if (NMOSEK_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
    EcuM_BswErrorHook(NMOSEK_MODULE_ID, ECUM_BSWERROR_NULLPTR);
#  if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
    errorId = NMOSEK_E_PARAM_CONFIG;
#  else
    /* do not set errorId: explicitly allowed */
#  endif
# else
    /* do not set errorId: explicitly allowed */
# endif
  }
  else
#endif
  {
#if (NMOSEK_USE_INIT_POINTER == STD_ON)
    /* - #30 Copy the provided pointer to the local configuration pointer variable */
    NmOsek_ConfigPtr = nmOsekConfigPtr;
#endif

#if (NMOSEK_GENERATORCOMPATIBILITYVERSION == STD_ON )
    /* - #40 Check generator version check */
    if (NmOsek_GetGeneratorCompatibilityVersion() != NmOsek_GetExpectedCompatibilityVersion() )
    {
      EcuM_BswErrorHook(NMOSEK_MODULE_ID, ECUM_BSWERROR_COMPATIBILITYVERSION);
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
      errorId = NMOSEK_E_INIT_FAILED;
# endif
    }
    else
#endif
#if (NMOSEK_FINALMAGICNUMBER == STD_ON)
    /* - #50 Check the magic number */
    if (NmOsek_GetFinalMagicNumber() != NMOSEK_FINAL_MAGIC_NUMBER)
    {
      EcuM_BswErrorHook(NMOSEK_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
      errorId = NMOSEK_E_INIT_FAILED;
# endif
    }
    else
#endif
    {
      /* ----- Implementation ----------------------------------------------- */

      /* - #60 Initialize every variable except NmOsek_ConfigPtr that is not initialized via NmOsek_InitMemory() */
#if ((NMOSEK_NM_NODE_ID_ENABLED == STD_ON) || (NMOSEK_NM_USER_DATA_ENABLED == STD_ON))
      VAR(uint8, AUTOMATIC) byteIndex; /* ESCAN00086570 */
#endif

#if (NMOSEK_API_OPTIMIZATION == STD_ON)
      /* No need to determine local channel index */
#else
      VAR(NetworkHandleType, AUTOMATIC) NMOSEK_CHANNEL_IDX;
#endif

#if (NMOSEK_API_OPTIMIZATION == STD_ON)
      /* No need to determine local channel index */
#else
      for (NMOSEK_CHANNEL_IDX = 0u; NMOSEK_CHANNEL_IDX < NmOsek_GetSizeOfChannelConfig(); NMOSEK_CHANNEL_IDX++)
#endif
      {
#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
        NmOsek_SetTxRequest(NMOSEK_CHANNEL_IDX, FALSE); /* Reset TxRequest Flag */
#endif
        NmOsek_SetTimeoutTimer(NMOSEK_CHANNEL_IDX, 0u); /* Reset Timer */

#if (NMOSEK_MSGTIMEOUTTIMER == STD_ON)
        NmOsek_SetMsgTimeoutTimer(NMOSEK_CHANNEL_IDX, 0u);
#endif

        NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, 0u);      /* Reset RX/TX counter */

#if (NMOSEK_FAST_BUSOFF_RECOVERY == STD_ON)
        NmOsek_SetBusOffCounter(NMOSEK_CHANNEL_IDX, NmOsek_GetLimpHomeShortNumberOfChannelConfig(NMOSEK_CHANNEL_IDX));
#endif

#if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
        NmOsek_SetCanIsBusOff(NMOSEK_CHANNEL_IDX, FALSE); /* ESCAN00054511 */
#endif

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
        /* Initialize NM Message Control Bytes */ /* ESCAN00049910 */
        NmOsek_SetTxMessageData(NMOSEK_MESSAGE_DATA_BYTE_INDEX(NMOSEK_CHANNEL_IDX, NMOSEK_CFG_PDU_DEST_ID_POSITION),
          NMOSEK_CTRL_INIT);
        NmOsek_SetTxMessageData(NMOSEK_MESSAGE_DATA_BYTE_INDEX(NMOSEK_CHANNEL_IDX, NMOSEK_CFG_PDU_OPCODE_POSITION),
          NMOSEK_CTRL_INIT);

        /* Initialize User Data Bytes */
        NmOsek_SetTxUserDataBytes(NMOSEK_CHANNEL_PARA_FIRST NMOSEK_DATA_INIT);
#endif /* (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) */

#if ((NMOSEK_NM_NODE_ID_ENABLED == STD_ON ) || (NMOSEK_NM_USER_DATA_ENABLED == STD_ON))
        /* Initialize RX message data */
        for (byteIndex = 0u; byteIndex < NmOsek_GetPduLengthOfPbChannelConfig(NMOSEK_CHANNEL_IDX); byteIndex++)
        {
          NmOsek_SetRxMessageData(NMOSEK_MESSAGE_DATA_BYTE_INDEX(NMOSEK_CHANNEL_IDX, byteIndex), NMOSEK_DATA_INIT);
        }
#endif

        /* Initialize State Variables */
        /* Active Channel Initialization */
        NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, NMOSEK_INTERNAL_STATE_BYTES_INIT);

#if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
        /* Initialize Gateway State Variable */
        NmOsek_SetRemoteSleepState(NMOSEK_CHANNEL_IDX, NMOSEK_REMOTE_SLEEP_STATE_SLEEP_IND);
#endif

        /* Set initial NM State: BusSleep */
        NmOsek_SetNmState(NMOSEK_CHANNEL_IDX, NM_STATE_BUS_SLEEP);

#if (NMOSEK_NM_STATE_CHANGE_IND_ENABLED == STD_ON)
        /* Set previous NM State: BusSleep */ /* ESCAN00055975 */
        NmOsek_SetPreviousNmState(NMOSEK_CHANNEL_IDX, NM_STATE_BUS_SLEEP);
#endif

#if (NMOSEK_DESIREDNMSTATE == STD_ON)
        /* Set desired NM State: BusSleep */
        NmOsek_SetDesiredNmState(NMOSEK_CHANNEL_IDX, NM_STATE_BUS_SLEEP);
#endif

#if (NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE == STD_ON)
        /* Clear flag that indicates that a NmOsek_TxConfirmation() call has been executed after the transition to
         * Network Mode / after a BusOff recovery
         */
        NmOsek_SetFirstTxConfOrRxInd(NMOSEK_CHANNEL_IDX, FALSE);
#endif

        /* Set initial Action state to BusSleep */
        NmOsek_SetAction(NMOSEK_CHANNEL_IDX, NMOSEK_ACTION_BUS_SLEEP);

#if (NMOSEK_NM_NODE_ID_ENABLED == STD_ON)
        /* Initialize NmOsek_RxNmMsgSourceAddr earlier in case NmOsek_GetNodeIdentifier() is called before
          * NmOsek_MainFunction() and/or NmOsek_RxIndication() have been called.
          */ /* ESCAN00051046 */
        NmOsek_SetRxNmMsgSourceAddr(NMOSEK_CHANNEL_IDX, NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX));
#endif

        /* Initialize logical successor to own node ID */ /* ESCAN00051046 */
        NmOsek_SetLogicalSuccessor(NMOSEK_CHANNEL_IDX, NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX));

#if (NMOSEK_SYNCPOINTTIMER == STD_ON)
        /* - NmOsek_SetSyncPointTimer
         * - NmOsek_SetRingCycleTimer
         * are used within NmOsek_ResetNodeConfig()
         */
#endif

#if (NMOSEK_ANYCHANNELINLIMPHOME == STD_ON)
        NmOsek_SetAnyChannelInLimpHome(NmOsek_GetAnyChannelInLimpHomeIdxOfChannelConfig(NMOSEK_CHANNEL_IDX), 0u);
#endif

        NmOsek_ResetNodeConfig(NMOSEK_CHANNEL_PARA_ONLY);
      }

#if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
      NmOsek_SetModuleInitialized(NMOSEK_INIT);
#endif
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_INIT, errorId);
  }
#else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

#if (NMOSEK_USE_INIT_POINTER == STD_OFF)
  NMOSEK_DUMMY_STATEMENT(nmOsekConfigPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * NmOsek_PassiveStartUp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_PassiveStartUp(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(Std_ReturnType, AUTOMATIC) retVal = E_NOT_OK;
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check system channel index and NM channel index */
# if (NMOSEK_API_OPTIMIZATION == STD_ON)
  if (nmChannelHandle != NMOSEK_SYSTEM_CHANNEL_IDX)
# else
  if ((nmChannelHandle >= NmOsek_GetNumberOfSystemChannels())
#  if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      || (NmOsek_GetSysToNmChannelInd(nmChannelHandle) >= NmOsek_GetSizeOfChannelConfig())
#  endif
     )
# endif
  {
    errorId = NMOSEK_E_INVALID_CHANNEL;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
    /* Create channel handle of Nm instance (in post-compile or multiple channel configurations). */
    VAR(NetworkHandleType, AUTOMATIC) channel = NmOsek_GetSysToNmChannelInd(nmChannelHandle);
#endif

    /* - #30 If channel state is (Prepare) Bus Sleep Mode, clear Prep Sleep flag so that channel can start up in
     *       NmOsek_MainFunction
     */
    if ((NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) == NM_STATE_BUS_SLEEP) ||
        (NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) == NM_STATE_PREPARE_BUS_SLEEP))
    {
      SchM_Enter_NmOsek_NMOSEK_EXCLUSIVE_AREA_2(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* Clear PrepSleep flag */
      NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
        (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_PREP_SLEEP_INV_MASK));

#if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
      /* Remote sleep variable is not affected */
#endif

      SchM_Exit_NmOsek_NMOSEK_EXCLUSIVE_AREA_2(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* Success */
      retVal = E_OK;
    }
  }

#if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  /* ----- Development Error Report --------------------------------------- */
#if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_PASSIVESTARTUP, errorId);
  }
#else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
}

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
 *  NmOsek_NetworkRequest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_NetworkRequest(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle)
{
  /* ----- Local Variables ---------------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  VAR(Std_ReturnType, AUTOMATIC) retVal = E_NOT_OK;
# else
  VAR(Std_ReturnType, AUTOMATIC) retVal;
# endif
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check system channel index and NM channel index */
#  if (NMOSEK_API_OPTIMIZATION == STD_ON)
  if (nmChannelHandle != NMOSEK_SYSTEM_CHANNEL_IDX)
#  else
  if ((nmChannelHandle >= NmOsek_GetNumberOfSystemChannels())
#   if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      || (NmOsek_GetSysToNmChannelInd(nmChannelHandle) >= NmOsek_GetSizeOfChannelConfig())
#   endif
     )
#  endif
  {
    errorId = NMOSEK_E_INVALID_CHANNEL;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
    /* Create channel handle of Nm instance (in post-compile or multiple channel configurations). */
    VAR(NetworkHandleType, AUTOMATIC) channel = NmOsek_GetSysToNmChannelInd(nmChannelHandle);
# endif

# if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
    /* Remote sleep variable is not affected */
# endif
    SchM_Enter_NmOsek_NMOSEK_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* - #30 Clear Bus Sleep Indication flag */
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
      (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) &
       (NmOsek_InternalStateType)(NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_INV_MASK & NMOSEK_INTERNAL_STATE_PREP_SLEEP_INV_MASK)));

    SchM_Exit_NmOsek_NMOSEK_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* Success */
    retVal = E_OK;
  }

# if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  /* ----- Development Error Report --------------------------------------- */
# if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_NETWORKREQUEST, errorId);
  }
# else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
}

/**********************************************************************************************************************
 *  NmOsek_NetworkRelease()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_NetworkRelease(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle)
{
  /* ----- Local Variables ---------------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  VAR(Std_ReturnType, AUTOMATIC) retVal = E_NOT_OK;
# else
  VAR(Std_ReturnType, AUTOMATIC) retVal;
# endif
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check system channel index and NM channel index */
#  if (NMOSEK_API_OPTIMIZATION == STD_ON)
  if (nmChannelHandle != NMOSEK_SYSTEM_CHANNEL_IDX)
#  else
  if ((nmChannelHandle >= NmOsek_GetNumberOfSystemChannels())
#   if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      || (NmOsek_GetSysToNmChannelInd(nmChannelHandle) >= NmOsek_GetSizeOfChannelConfig())
#   endif
     )
#  endif
  {
    errorId = NMOSEK_E_INVALID_CHANNEL;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
    /* Create channel handle of Nm instance (in post-compile or multiple channel configurations). */
    VAR(NetworkHandleType, AUTOMATIC) channel = NmOsek_GetSysToNmChannelInd(nmChannelHandle);
# endif

    SchM_Enter_NmOsek_NMOSEK_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #30 Set Bus Sleep Indication flag */
    NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
      (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_BUS_SLEEP_IND_FLAG_MASK));

    SchM_Exit_NmOsek_NMOSEK_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* Success */
    retVal = E_OK;
  }

# if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  /* ----- Development Error Report --------------------------------------- */
# if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_NETWORKRELEASE, errorId);
  }
# else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
}
#endif /* NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF */

#if (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_DisableCommunication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_DisableCommunication(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle)
{
# if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
  /* ----- Local Variables ---------------------------------------------- */
  VAR(Std_ReturnType, AUTOMATIC) retVal = E_NOT_OK;
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check system channel index and NM channel index */
#   if (NMOSEK_API_OPTIMIZATION == STD_ON)
  if (nmChannelHandle != NMOSEK_SYSTEM_CHANNEL_IDX)
#   else
  if ((nmChannelHandle >= NmOsek_GetNumberOfSystemChannels())
#    if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      || (NmOsek_GetSysToNmChannelInd(nmChannelHandle) >= NmOsek_GetSizeOfChannelConfig())
#    endif
     )
#   endif
  {
    errorId = NMOSEK_E_INVALID_CHANNEL;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
#  if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
    /* Create channel handle of Nm instance (in post-compile or multiple channel configurations). */
    VAR(NetworkHandleType, AUTOMATIC) channel = NmOsek_GetSysToNmChannelInd(nmChannelHandle);
#  endif

    /* - #30 Disable NM PDU transmission ability when the NM is not about to sleep */
    if (NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) > NM_STATE_PREPARE_BUS_SLEEP)
    {
      SchM_Enter_NmOsek_NMOSEK_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* Clear active flag */
      NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
        (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_INV_MASK));

      SchM_Exit_NmOsek_NMOSEK_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* Success */
      retVal = E_OK;
    }
  }

#  if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif

  /* ----- Development Error Report --------------------------------------- */
#  if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_DISABLECOMMUNICATION, errorId);
  }
#  else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif

  return retVal;
# else /* if (NMOSEK_NM_PASSIVE_MODE_ENABLED != STD_OFF) */
  /* ----- Implementation ----------------------------------------------- */
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  return E_NOT_OK;
# endif /* (NMOSEK_NM_PASSIVE_MODE_ENABLED != STD_OFF) */
}

/**********************************************************************************************************************
 *  NmOsek_EnableCommunication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_EnableCommunication(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle)
{
# if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
  /* ----- Local Variables ---------------------------------------------- */
  VAR(Std_ReturnType, AUTOMATIC) retVal = E_NOT_OK;
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check system channel index and NM channel index */
#   if (NMOSEK_API_OPTIMIZATION == STD_ON)
  if (nmChannelHandle != NMOSEK_SYSTEM_CHANNEL_IDX)
#   else
  if ((nmChannelHandle >= NmOsek_GetNumberOfSystemChannels())
#    if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      || (NmOsek_GetSysToNmChannelInd(nmChannelHandle) >= NmOsek_GetSizeOfChannelConfig())
#    endif
     )
#   endif
  {
    errorId = NMOSEK_E_INVALID_CHANNEL;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
#  if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
    /* Create channel handle of Nm instance (in post-compile or multiple channel configurations). */
    VAR(NetworkHandleType, AUTOMATIC) channel = NmOsek_GetSysToNmChannelInd(nmChannelHandle);
#  endif

    /* - #30 Enable Nm PDU transmission ability only when it is disabled and NM is not about to sleep */
    if ((NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) > NM_STATE_PREPARE_BUS_SLEEP) &&
        ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK) == 0u))
    {
      SchM_Enter_NmOsek_NMOSEK_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* Set active flag */
      NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
        (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_ACTIVE_FLAG_MASK));

      SchM_Exit_NmOsek_NMOSEK_EXCLUSIVE_AREA_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* Success */
      retVal = E_OK;
    }
  }

#  if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif

  /* ----- Development Error Report --------------------------------------- */
#  if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_ENABLECOMMUNICATION, errorId);
  }
#  else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif

  return retVal;
# else /* if (NMOSEK_NM_PASSIVE_MODE_ENABLED != STD_OFF) */
  /* ----- Implementation ----------------------------------------------- */
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  return E_NOT_OK;
# endif /* (NMOSEK_NM_PASSIVE_MODE_ENABLED != STD_OFF) */
}
#endif /* (NMOSEK_NM_COM_CONTROL_ENABLED == STD_ON) */

#if (NMOSEK_NM_USER_DATA_ENABLED == STD_ON)
# if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
#  if (NMOSEK_NM_COM_USER_DATA_SUPPORT == STD_OFF)
/**********************************************************************************************************************
 *  NmOsek_SetUserData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_SetUserData(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                     CONSTP2CONST(uint8, AUTOMATIC, NMOSEK_APPL_VAR) nmUserDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
#   if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  VAR(Std_ReturnType, AUTOMATIC) retVal = E_NOT_OK;
#   else
  VAR(Std_ReturnType, AUTOMATIC) retVal;
#   endif
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;
  VAR(uint8, AUTOMATIC) byteIndex;

  /* ----- Development Error Checks ------------------------------------- */
#   if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check system channel index and NM channel index */
#    if (NMOSEK_API_OPTIMIZATION == STD_ON)
  if (nmChannelHandle != NMOSEK_SYSTEM_CHANNEL_IDX)
#    else
  if ((nmChannelHandle >= NmOsek_GetNumberOfSystemChannels())
#     if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      || (NmOsek_GetSysToNmChannelInd(nmChannelHandle) >= NmOsek_GetSizeOfChannelConfig())
#     endif
     )
#    endif
  {
    errorId = NMOSEK_E_INVALID_CHANNEL;
  }
  else
  /* - #30 Check for NULL pointer */
  if ((nmUserDataPtr) == NULL_PTR)
  {
    errorId = NMOSEK_E_PARAM_POINTER;
  }
  else
#   endif
  {
    /* ----- Implementation ----------------------------------------------- */
#   if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
    /* Create channel handle of Nm instance (in post-compile or multiple channel configurations). */
    VAR(NetworkHandleType, AUTOMATIC) channel = NmOsek_GetSysToNmChannelInd(nmChannelHandle);
#   endif

    /* - #40 Copy user data bytes into TX Message Data buffer */
    SchM_Enter_NmOsek_NMOSEK_EXCLUSIVE_AREA_3(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    for (byteIndex = NMOSEK_USER_DATA_OFFSET;
         byteIndex < NmOsek_GetPduLengthOfPbChannelConfig(NMOSEK_CHANNEL_IDX); byteIndex++)
    {
      NmOsek_SetTxMessageData(NMOSEK_MESSAGE_DATA_BYTE_INDEX(NMOSEK_CHANNEL_IDX, byteIndex),
        nmUserDataPtr[byteIndex - NMOSEK_USER_DATA_OFFSET]);
    }
    SchM_Exit_NmOsek_NMOSEK_EXCLUSIVE_AREA_3(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* Success */
    retVal = E_OK;
  }

#   if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#   endif

  /* ----- Development Error Report --------------------------------------- */
#   if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_SETUSERDATA, errorId);
  }
#   else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#   endif

  return retVal;
}
#  endif /* (NMOSEK_NM_COM_USER_DATA_SUPPORT == STD_OFF) */
# endif /* (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) */

/**********************************************************************************************************************
 *  NmOsek_GetUserData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_GetUserData(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                     CONSTP2VAR(uint8, AUTOMATIC, NMOSEK_APPL_VAR) nmUserDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  VAR(Std_ReturnType, AUTOMATIC) retVal = E_NOT_OK;
# else
  VAR(Std_ReturnType, AUTOMATIC) retVal;
# endif
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;
  VAR(uint8, AUTOMATIC) byteIndex;

  /* ----- Development Error Checks ------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check system channel index and NM channel index */
#  if (NMOSEK_API_OPTIMIZATION == STD_ON)
  if (nmChannelHandle != NMOSEK_SYSTEM_CHANNEL_IDX)
#  else
  if ((nmChannelHandle >= NmOsek_GetNumberOfSystemChannels())
#   if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      || (NmOsek_GetSysToNmChannelInd(nmChannelHandle) >= NmOsek_GetSizeOfChannelConfig())
#   endif
     )
#  endif
  {
    errorId = NMOSEK_E_INVALID_CHANNEL;
  }
  else
  /* - #30 Check for NULL pointer */
  if ((nmUserDataPtr) == NULL_PTR)
  {
    errorId = NMOSEK_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
    /* Create channel handle of Nm instance (in post-compile or multiple channel configurations). */
    VAR(NetworkHandleType, AUTOMATIC) channel = NmOsek_GetSysToNmChannelInd(nmChannelHandle);
# endif

    /* - #40 Copy user data bytes from RX message data buffer to provided address */
    SchM_Enter_NmOsek_NMOSEK_EXCLUSIVE_AREA_4(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    for (byteIndex = NMOSEK_USER_DATA_OFFSET;
         byteIndex < NmOsek_GetPduLengthOfPbChannelConfig(NMOSEK_CHANNEL_IDX); byteIndex++)
    {
      nmUserDataPtr[byteIndex - NMOSEK_USER_DATA_OFFSET] =
        NmOsek_GetRxMessageData(NMOSEK_MESSAGE_DATA_BYTE_INDEX(NMOSEK_CHANNEL_IDX, byteIndex));
    }
    SchM_Exit_NmOsek_NMOSEK_EXCLUSIVE_AREA_4(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* Success */
    retVal = E_OK;
  }

# if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  /* ----- Development Error Report --------------------------------------- */
# if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_GETUSERDATA, errorId);
  }
# else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
}
#endif  /* (NMOSEK_NM_USER_DATA_ENABLED == STD_ON) */

#if (NMOSEK_WAKEUPTXUSERDATAPDUIDOFPBCHANNELCONFIG == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_Transmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_Transmit(PduIdType NmOsekTxPduId,
                                                  P2CONST(PduInfoType, AUTOMATIC, NMOSEK_APPL_VAR) PduInfoPtr)
{
  /* - #10 return E_OK */
  NMOSEK_DUMMY_STATEMENT(NmOsekTxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  NMOSEK_DUMMY_STATEMENT(PduInfoPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  return E_OK;
}
#endif  /* (NMOSEK_NM_COM_USER_DATA_SUPPORT == STD_ON) */

#if (NMOSEK_NM_NODE_ID_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_GetNodeIdentifier()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_GetNodeIdentifier(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                           CONSTP2VAR(uint8, AUTOMATIC, NMOSEK_APPL_VAR) nmNodeIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  VAR(Std_ReturnType, AUTOMATIC) retVal = E_NOT_OK;
# else
  VAR(Std_ReturnType, AUTOMATIC) retVal;
# endif
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check system channel index and NM channel index */
#  if (NMOSEK_API_OPTIMIZATION == STD_ON)
  if (nmChannelHandle != NMOSEK_SYSTEM_CHANNEL_IDX)
#  else
  if ((nmChannelHandle >= NmOsek_GetNumberOfSystemChannels())
#   if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      || (NmOsek_GetSysToNmChannelInd(nmChannelHandle) >= NmOsek_GetSizeOfChannelConfig())
#   endif
     )
#  endif
  {
    errorId = NMOSEK_E_INVALID_CHANNEL;
  }
  else
  /* - #30 Check for NULL pointer */
  if ((nmNodeIdPtr) == NULL_PTR)
  {
    errorId = NMOSEK_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
    /* Create channel handle of Nm instance (in post-compile or multiple channel configurations). */
    VAR(NetworkHandleType, AUTOMATIC) channel = NmOsek_GetSysToNmChannelInd(nmChannelHandle);
# endif

    /* - #40 Copy received source address to provided pointer */
    *nmNodeIdPtr = NmOsek_GetRxNmMsgSourceAddr(NMOSEK_CHANNEL_IDX);

    /* Success */
    retVal = E_OK;
  }

# if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  /* ----- Development Error Report --------------------------------------- */
# if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_GETNODEIDENTIFIER, errorId);
  }
# else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
}

/**********************************************************************************************************************
 *  NmOsek_GetLocalNodeIdentifier()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_GetLocalNodeIdentifier(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                                CONSTP2VAR(uint8, AUTOMATIC, NMOSEK_APPL_VAR)
                                                                nmNodeIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  VAR(Std_ReturnType, AUTOMATIC) retVal = E_NOT_OK;
# else
  VAR(Std_ReturnType, AUTOMATIC) retVal;
# endif
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check system channel index and NM channel index */
#  if (NMOSEK_API_OPTIMIZATION == STD_ON)
  if (nmChannelHandle != NMOSEK_SYSTEM_CHANNEL_IDX)
#  else
  if ((nmChannelHandle >= NmOsek_GetNumberOfSystemChannels())
#   if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      || (NmOsek_GetSysToNmChannelInd(nmChannelHandle) >= NmOsek_GetSizeOfChannelConfig())
#   endif
     )
#  endif
  {
    errorId = NMOSEK_E_INVALID_CHANNEL;
  }
  else
  /* - #30 Check for NULL pointer */
  if ((nmNodeIdPtr) == NULL_PTR)
  {
    errorId = NMOSEK_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
    /* Create channel handle of Nm instance (in post-compile or multiple channel configurations). */
    VAR(NetworkHandleType, AUTOMATIC) channel = NmOsek_GetSysToNmChannelInd(nmChannelHandle);
# endif

    /* - #40 Copy local address to provided pointer */
    *nmNodeIdPtr = NmOsek_GetNodeIdOfPbChannelConfig(NMOSEK_CHANNEL_IDX);

    /* Success */
    retVal = E_OK;
  }

# if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  /* ----- Development Error Report --------------------------------------- */
# if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_GETLOCALNODEIDENTIFIER, errorId);
  }
# else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
}
#endif /* ( NMOSEK_NM_NODE_ID_ENABLED == STD_ON ) */

#if (NMOSEK_NM_NODE_DETECTION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_RepeatMessageRequest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_RepeatMessageRequest(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle)
{
  /* - #10 return E_NOT_OK */
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  return E_NOT_OK;
}
#endif /* (NMOSEK_NM_NODE_DETECTION_ENABLED == STD_ON) */

#if ((NMOSEK_NM_NODE_ID_ENABLED == STD_ON ) || (NMOSEK_NM_USER_DATA_ENABLED == STD_ON))
/**********************************************************************************************************************
 *  NmOsek_GetPduData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_GetPduData(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                    CONSTP2VAR(uint8, AUTOMATIC, NMOSEK_APPL_VAR) nmPduDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  VAR(Std_ReturnType, AUTOMATIC) retVal = E_NOT_OK;
# else
  VAR(Std_ReturnType, AUTOMATIC) retVal;
# endif
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;
  VAR(uint8, AUTOMATIC) byteIndex;

  /* ----- Development Error Checks ------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check system channel index and NM channel index */
#  if (NMOSEK_API_OPTIMIZATION == STD_ON)
  if (nmChannelHandle != NMOSEK_SYSTEM_CHANNEL_IDX)
#  else
  if ((nmChannelHandle >= NmOsek_GetNumberOfSystemChannels())
#   if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      || (NmOsek_GetSysToNmChannelInd(nmChannelHandle) >= NmOsek_GetSizeOfChannelConfig())
#   endif
     )
#  endif
  {
    errorId = NMOSEK_E_INVALID_CHANNEL;
  }
  else
  /* - #30 Check for NULL pointer */
  if ((nmPduDataPtr) == NULL_PTR)
  {
    errorId = NMOSEK_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
    /* Create channel handle of Nm instance (in post-compile or multiple channel configurations). */
    VAR(NetworkHandleType, AUTOMATIC) channel = NmOsek_GetSysToNmChannelInd(nmChannelHandle);
# endif

    SchM_Enter_NmOsek_NMOSEK_EXCLUSIVE_AREA_5(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* - #40 Copy Data from RX Message Data buffer */
    for (byteIndex = 0u; byteIndex < NmOsek_GetPduLengthOfPbChannelConfig(NMOSEK_CHANNEL_IDX); byteIndex++)
    {
      nmPduDataPtr[byteIndex] =
        NmOsek_GetRxMessageData(NMOSEK_MESSAGE_DATA_BYTE_INDEX(NMOSEK_CHANNEL_IDX, byteIndex));
    }

    SchM_Exit_NmOsek_NMOSEK_EXCLUSIVE_AREA_5(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* Success */
    retVal = E_OK;
  }

# if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  /* ----- Development Error Report --------------------------------------- */
# if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_GETPDUDATA, errorId);
  }
# else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
}
#endif /* (NMOSEK_NM_NODE_ID_ENABLED == STD_ON) || (NMOSEK_NM_USER_DATA_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  NmOsek_GetState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_GetState(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                  CONSTP2VAR(Nm_StateType, AUTOMATIC, NMOSEK_APPL_VAR) nmStatePtr,
                                                  CONSTP2VAR( Nm_ModeType, AUTOMATIC, NMOSEK_APPL_VAR) nmModePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  VAR(Std_ReturnType, AUTOMATIC) retVal = E_NOT_OK;
#else
  VAR(Std_ReturnType, AUTOMATIC) retVal;
#endif
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check system channel index and NM channel index */
# if (NMOSEK_API_OPTIMIZATION == STD_ON)
  if (nmChannelHandle != NMOSEK_SYSTEM_CHANNEL_IDX)
# else
  if ((nmChannelHandle >= NmOsek_GetNumberOfSystemChannels())
#  if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      || (NmOsek_GetSysToNmChannelInd(nmChannelHandle) >= NmOsek_GetSizeOfChannelConfig())
#  endif
     )
# endif
  {
    errorId = NMOSEK_E_INVALID_CHANNEL;
  }
  else
  /* - #30 Check for NULL pointer */
  if (((nmStatePtr) == NULL_PTR) || ((nmModePtr) == NULL_PTR))
  {
    errorId = NMOSEK_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
    /* Create channel handle of Nm instance (in post-compile or multiple channel configurations). */
    VAR(NetworkHandleType, AUTOMATIC) channel = NmOsek_GetSysToNmChannelInd(nmChannelHandle);
#endif

    /* - #40 Copy state to provided state pointer */
    *nmStatePtr = NmOsek_GetNmState(NMOSEK_CHANNEL_IDX);

    /* - #50 Set mode respective to the state */
    if (NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) == NM_STATE_BUS_SLEEP)
    {
      /* NMBusSleep */
      *nmModePtr = NM_MODE_BUS_SLEEP;
    }
    else if (NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) == NM_STATE_PREPARE_BUS_SLEEP)
    {
      /* NMTwbsNormal / NMTwbsLimpHome */
      *nmModePtr = NM_MODE_PREPARE_BUS_SLEEP;
    }
    else
    {
      /* Ready Sleep, Normal Operation, Repeat Message */
#if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
      /* BusOff */
#endif
      *nmModePtr = NM_MODE_NETWORK;
    }

    retVal = E_OK;
  }

#if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  /* ----- Development Error Report --------------------------------------- */
#if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_GETSTATE, errorId);
  }
#else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if (NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_CancelWaitForTxConfOrRxInd()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_CancelWaitForTxConfOrRxInd(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle)
{
  /* ----- Local Variables ---------------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  VAR(Std_ReturnType, AUTOMATIC) retVal = E_NOT_OK;
# else
  VAR(Std_ReturnType, AUTOMATIC) retVal;
# endif
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check system channel index and NM channel index */
#  if (NMOSEK_API_OPTIMIZATION == STD_ON)
  if (nmChannelHandle != NMOSEK_SYSTEM_CHANNEL_IDX)
#  else
  if ((nmChannelHandle >= NmOsek_GetNumberOfSystemChannels())
#   if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      || (NmOsek_GetSysToNmChannelInd(nmChannelHandle) >= NmOsek_GetSizeOfChannelConfig())
#   endif
     )
#  endif
  {
    errorId = NMOSEK_E_INVALID_CHANNEL;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
    /* Create channel handle of Nm instance (in post-compile or multiple channel configurations). */
    VAR(NetworkHandleType, AUTOMATIC) channel = NmOsek_GetSysToNmChannelInd(nmChannelHandle);
# endif

    /* - #30 If the current state is appropriate, set the FirstTxConfOrRxInd flag to TRUE. */
    if (NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) > NM_STATE_PREPARE_BUS_SLEEP)
    {
      NmOsek_SetFirstTxConfOrRxInd(NMOSEK_CHANNEL_IDX, TRUE);
      retVal = E_OK;
    }
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
# else
    else
    {
      retVal = E_NOT_OK;
    }
# endif
  }

# if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  /* ----- Development Error Report --------------------------------------- */
# if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_CANCELWAITFORTXCONFORRXIND, errorId);
  }
# else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
}
#endif

#if (NMOSEK_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, NMOSEK_CODE) NmOsek_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, NMOSEK_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check versioninfo for NULL pointer. Note: no uninit check is performed. */
  if ((versioninfo) == NULL_PTR)
  {
    errorId = NMOSEK_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */

    /* - #20 Set versioninfo with corresponding macros from component header. */
    versioninfo->vendorID = NMOSEK_VENDOR_ID;
    versioninfo->moduleID = NMOSEK_MODULE_ID;
    versioninfo->sw_major_version = NMOSEK_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = NMOSEK_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = NMOSEK_SW_PATCH_VERSION;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_GETVERSIONINFO, errorId);
  }
# else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
}
#endif /* (NMOSEK_VERSION_INFO_API == STD_ON) */

#if (NMOSEK_NM_BUS_SYNCHRONIZATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_RequestBusSynchronization()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_RequestBusSynchronization(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle)
{
  /* - #10 return E_NOT_OK */
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* Bus Synchronization is not applicable for NmOsek */
  return E_NOT_OK;
}
#endif /* ( NMOSEK_NM_BUS_SYNCHRONIZATION_ENABLED == STD_ON ) */

#if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_CheckRemoteSleepIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_CheckRemoteSleepIndication(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                                    CONSTP2VAR(boolean, AUTOMATIC, NMOSEK_APPL_VAR)
                                                                    nmRemoteSleepIndPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(Std_ReturnType, AUTOMATIC) retVal = E_NOT_OK;
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check system channel index and NM channel index */
#  if (NMOSEK_API_OPTIMIZATION == STD_ON)
  if (nmChannelHandle != NMOSEK_SYSTEM_CHANNEL_IDX)
#  else
  if ((nmChannelHandle >= NmOsek_GetNumberOfSystemChannels())
#   if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      || (NmOsek_GetSysToNmChannelInd(nmChannelHandle) >= NmOsek_GetSizeOfChannelConfig())
#   endif
     )
#  endif
  {
    errorId = NMOSEK_E_INVALID_CHANNEL;
  }
  else
  /* - #30 Check for NULL pointer */
  if ((nmRemoteSleepIndPtr) == NULL_PTR)
  {
    errorId = NMOSEK_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
    /* Create channel handle of Nm instance (in post-compile or multiple channel configurations). */
    VAR(NetworkHandleType, AUTOMATIC) channel = NmOsek_GetSysToNmChannelInd(nmChannelHandle);
# endif

    /* Check Remote Sleep Indication may only be requested in Ready Sleep State and during Normal Operation. */
    if ((NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) == NM_STATE_READY_SLEEP)
        /* ESCAN00049873 */
        || (NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) == NM_STATE_NORMAL_OPERATION)
       )
    {
      /* - #40 copy remote sleep state to provided pointer */
      if( ( NmOsek_GetRemoteSleepState(NMOSEK_CHANNEL_IDX) & NMOSEK_REMOTE_SLEEP_STATE_SLEEP_IND ) != 0u )
      {
        *nmRemoteSleepIndPtr = TRUE;
      }
      else
      {
        *nmRemoteSleepIndPtr = FALSE;
      }

      /* Success */
      retVal = E_OK;
    }
  }

# if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  /* ----- Development Error Report --------------------------------------- */
# if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_CHECKREMOTESLEEPIND, errorId);
  }
# else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON) */

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
/**********************************************************************************************************************
 *  NmOsek_TxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, NMOSEK_CODE) NmOsek_TxConfirmation(VAR(PduIdType, AUTOMATIC) nmOsekTxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
# endif
  {
    /* - #20 Determine channel index from TX PDU ID */
# if (NMOSEK_API_OPTIMIZATION == STD_OFF)
    VAR(NetworkHandleType, AUTOMATIC) NMOSEK_CHANNEL_IDX = (NetworkHandleType)nmOsekTxPduId;
#  if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
    /* - #30 Check whether channel index is valid */
    if (NMOSEK_CHANNEL_IDX >= NmOsek_GetSizeOfChannelConfig())
    {
      errorId = NMOSEK_E_INVALID_CHANNEL;
    }
    else
#  endif
# endif
    {
      /* ----- Implementation ----------------------------------------------- */

      /* ESCAN00049894 */

      /* ESCAN00055975 */

# if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
      /* - #40 Only process confirmation if there is no BusOff occurrence. */
      if (!NmOsek_IsCanIsBusOff(NMOSEK_CHANNEL_IDX))
# endif
      {
        VAR(uint8, AUTOMATIC) opCode;

        SchM_Enter_NmOsek_NMOSEK_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        opCode = (uint8)
          (NmOsek_GetTxMessageData(NMOSEK_MESSAGE_DATA_BYTE_INDEX(NMOSEK_CHANNEL_IDX, NMOSEK_CFG_PDU_OPCODE_POSITION)) &
           NMOSEK_OPCODE_CMD_MASK);
# if defined(NMOSEK_TYPE_VECTOR2_OSEK)
        if (opCode == (NMOSEK_RING | NMOSEK_SLEEP_ACK))
        {
          opCode = NMOSEK_RING;
        }
# endif

        /* - #50 Message transmitted => reset TX counter */
        NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, (NmOsek_RxTxCntType)
          (NmOsek_GetRxTxCnt(NMOSEK_CHANNEL_IDX) & NMOSEK_TX_CNT_INV_MASK));

# if (NMOSEK_FAST_BUSOFF_RECOVERY == STD_ON)
        /* - #60 Message transmitted => reset BusOff counter */
        NmOsek_SetBusOffCounter(NMOSEK_CHANNEL_IDX, NmOsek_GetLimpHomeShortNumberOfChannelConfig(NMOSEK_CHANNEL_IDX));
# endif

# if (NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE == STD_ON)
        if (NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) > NM_STATE_PREPARE_BUS_SLEEP)
        {
          NmOsek_SetFirstTxConfOrRxInd(NMOSEK_CHANNEL_IDX, TRUE);
        }
# endif
# if (NMOSEK_MSGTIMEOUTTIMER == STD_ON)
        NmOsek_SetMsgTimeoutTimer(NMOSEK_CHANNEL_IDX, 0u);
# endif

        /* - #70 Perform actions in dependency of the transmitted OpCode */
        /* ESCAN00049910 */
        switch (opCode)
        {

          case NMOSEK_RING:
            NmOsek_TxConfirmation_HandleRingMessage(NMOSEK_CHANNEL_PARA_ONLY);
            break;

          case NMOSEK_ALIVE:
            NmOsek_TxConfirmation_HandleAliveMessage(NMOSEK_CHANNEL_PARA_ONLY);
            break;

# if defined(NMOSEK_TYPE_VECTOR2_OSEK)
          default: /* Includes NMOSEK_LIMPHOME */
# else
          case NMOSEK_LIMPHOME:
# endif
            NmOsek_TxConfirmation_HandleLimpHomeMessage(NMOSEK_CHANNEL_PARA_ONLY);
            break;

# if defined(NMOSEK_TYPE_VECTOR2_OSEK)
# else
          default:
            break;
# endif
        }

# if (NMOSEK_NM_STATE_CHANGE_IND_ENABLED == STD_ON)
        /* - #80 Notify state change if occurred */
        /* ESCAN00049564 */
        NmOsek_Nm_StateChangeNotificationOnStateChange(NMOSEK_CHANNEL_PARA_ONLY);
# endif

        SchM_Exit_NmOsek_NMOSEK_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

# if (NMOSEK_CONFIRMTXUSERDATAPDUOFPBCHANNELCONFIG == STD_ON)
        NmOsek_TxConfirmation_NotifyPduR(NMOSEK_CHANNEL_PARA_FIRST opCode); /* ESCAN00091622 */
# endif
      }
    }
  }

# if (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT(nmOsekTxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  /* ----- Development Error Report --------------------------------------- */
# if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_TXCONFIRMATION, errorId);
  }
# else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
} /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif /* (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) */

/**********************************************************************************************************************
 *  NmOsek_RxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, NMOSEK_CODE) NmOsek_RxIndication(VAR(PduIdType, AUTOMATIC) nmOsekRxPduId,
                                            P2CONST(uint8, AUTOMATIC, NMOSEK_APPL_VAR) canSduPtr,
                                            CONST(Can_IdType, AUTOMATIC) canId)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check whether NULL pointer is passed */
  if ((canSduPtr) == NULL_PTR)
  {
    errorId = NMOSEK_E_PARAM_POINTER;
  }
  else
#endif
  {
#if (NMOSEK_API_OPTIMIZATION == STD_OFF)
    /* - #30 Determine channel index from RX PDU ID */
    VAR(NetworkHandleType, AUTOMATIC) NMOSEK_CHANNEL_IDX = (NetworkHandleType)nmOsekRxPduId;
#endif
#if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
    /* - #40 Check whether channel index is valid */
# if (NMOSEK_API_OPTIMIZATION == STD_ON)
    if (NMOSEK_CHANNEL_IDX != nmOsekRxPduId)
# else
    if (NMOSEK_CHANNEL_IDX >= NmOsek_GetSizeOfChannelConfig())
# endif
    {
      errorId = NMOSEK_E_INVALID_CHANNEL;
    }
    else
#endif
    {
      /* ----- Local Variables ---------------------------------------------- */
#if ((NMOSEK_NM_NODE_ID_ENABLED == STD_ON) || (NMOSEK_NM_USER_DATA_ENABLED == STD_ON) || (NMOSEK_NM_BUS_NM_SPECIFIC_PDU_RX_INDICATION_ENABLED == STD_ON))
      /* ESCAN00055975, ESCAN00085997 */
      VAR(uint8, AUTOMATIC) byteIndex;
#endif
      VAR(uint8, AUTOMATIC) sourceAddress;
      VAR(uint8, AUTOMATIC) destAddress;
      VAR(uint8, AUTOMATIC) opCode;
      VAR(uint8, AUTOMATIC) sleepFlags;

      /* ----- Implementation ----------------------------------------------- */

      /* - #50 Determine destination address, source address, op code and sleep flags */
      sourceAddress = (uint8)(canId & NmOsek_GetRangeMaskOfPbChannelConfig(NMOSEK_CHANNEL_IDX));
#if (NMOSEK_LAST_BYTE_OF_CAN_ID_AS_NODE_ID_ENABLED == STD_ON)
      /* The Node Identifier/Destination ID has an offset. This is being subtracted here */ /* ESCAN00052665 */
      destAddress = (uint8)(canSduPtr[NMOSEK_CFG_PDU_DEST_ID_POSITION] -
                            NmOsek_GetNodeIdOffsetOfChannelConfig(NMOSEK_CHANNEL_IDX));
#else
      destAddress = canSduPtr[NMOSEK_CFG_PDU_DEST_ID_POSITION];
#endif
      opCode = (uint8)(canSduPtr[NMOSEK_CFG_PDU_OPCODE_POSITION] & NMOSEK_OPCODE_CMD_MASK);
      sleepFlags = (uint8)(canSduPtr[NMOSEK_CFG_PDU_SLEEP_POSITION] & NMOSEK_OPCODE_SLEEP_MASK);

#if defined (NMOSEK_TYPE_VECTOR2_OSEK)
      if (opCode == (NMOSEK_RING | NMOSEK_SLEEP_ACK))
      {
        opCode = NMOSEK_RING;
        sleepFlags = NMOSEK_OPCODE_SLEEP_MASK;
      }
      else if (opCode == NMOSEK_ALIVE)
      {
        /* There is no ALIVE message with Sleep Acknowledge in this message layout, so keep Sleep Ind only */
        sleepFlags &= NMOSEK_SLEEP_IND;
      }
      else
      {
        /* Intentionally left empty */
      }
#endif

      SchM_Enter_NmOsek_NMOSEK_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* ESCAN00055975 */

#if (NMOSEK_NM_NODE_ID_ENABLED == STD_ON)
      /* Copy source node identifier */
      NmOsek_SetRxNmMsgSourceAddr(NMOSEK_CHANNEL_IDX, sourceAddress);
#endif

      /* - #60 if a valid message has been received, further process this message */
#if defined (NMOSEK_TYPE_VECTOR2_OSEK) && (NMOSEK_NMRESET_IGNORE_RX_EXCEPT_ALIVE_ENABLED == STD_OFF)
      (void)NmOsek_RxIndication_DetermineWhetherMessageIsValidAndUpdateStatusFlags(NMOSEK_CHANNEL_PARA_FIRST
                                                                                   sourceAddress, destAddress, opCode);
#else
      if (NmOsek_RxIndication_DetermineWhetherMessageIsValidAndUpdateStatusFlags(NMOSEK_CHANNEL_PARA_FIRST
                                                                                 sourceAddress, destAddress, opCode) ==
          TRUE)
#endif
      {
        /* Set NmMsgRecvFlag */
        NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
          (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) | NMOSEK_INTERNAL_STATE_NM_MSG_RECV_FLAG_MASK));

        /* Reset RX Counter */
        NmOsek_SetRxTxCnt(NMOSEK_CHANNEL_IDX, (NmOsek_RxTxCntType)
          (NmOsek_GetRxTxCnt(NMOSEK_CHANNEL_IDX) & NMOSEK_RX_CNT_INV_MASK));

        /* Check if network sleep status is not given any more */
        if ((sleepFlags == 0u) || (NmOsek_GetNmState(NMOSEK_CHANNEL_IDX) == NM_STATE_BUS_SLEEP)) /* ESCAN00062285 */
        {
          /* Reset PrepSleepFlag */
          NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
            (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_PREP_SLEEP_INV_MASK));

#if (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
          NmOsek_ChangeRemoteSleepState(NMOSEK_CHANNEL_PARA_FIRST NMOSEK_REMOTE_SLEEP_STATE_NO_SLEEP);
#endif

#if (NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE == STD_ON)
          NmOsek_SetFirstTxConfOrRxInd(NMOSEK_CHANNEL_IDX, TRUE);
#endif
        }

        /* RX SleepAck */
        if ((sleepFlags == NMOSEK_OPCODE_SLEEP_MASK) /* Sleep.ind == Sleep.ack == 1 */
#if (NMOSEK_WBSTRANSITION_ON_RING_MSG_ONLY_ENABLED == STD_ON)
            && (opCode == NMOSEK_RING) /* ESCAN00059016 */
#endif
           )
        {
          NmOsek_RxIndication_HandleRxSleepAck(NMOSEK_CHANNEL_PARA_ONLY);
        }

#if ((NMOSEK_NM_NODE_ID_ENABLED == STD_ON ) || (NMOSEK_NM_USER_DATA_ENABLED == STD_ON) || (NMOSEK_NM_BUS_NM_SPECIFIC_PDU_RX_INDICATION_ENABLED == STD_ON))
        /* - #70 Copy the NM message data to the Rx Message Data Buffer. */
        for (byteIndex = 0u; byteIndex < NmOsek_GetPduLengthOfPbChannelConfig(NMOSEK_CHANNEL_IDX); byteIndex++)
        {
          NmOsek_SetRxMessageData(NMOSEK_MESSAGE_DATA_BYTE_INDEX(NMOSEK_CHANNEL_IDX, byteIndex), canSduPtr[byteIndex]);
        }
#endif

        /* - #80 Notify NM Interface that a Nm message has been received */
#if (NMOSEK_NM_BUS_NM_SPECIFIC_PDU_RX_INDICATION_ENABLED == STD_ON)
        {
          VAR(PduInfoType, AUTOMATIC) pduInfo;
          pduInfo.SduDataPtr = /* ESCAN00086002 */
            (SduDataPtrType)NmOsek_GetAddrRxMessageData(NMOSEK_MESSAGE_DATA_FIRST_BYTE(NMOSEK_CHANNEL_IDX));
          pduInfo.SduLength = NmOsek_GetPduLengthOfPbChannelConfig(NMOSEK_CHANNEL_IDX);

          Nm_NmOsek_PduRxIndication(NMOSEK_NMTOSYSTEM_CHANNEL_IND, &pduInfo);
        }
#else
# if (NMOSEK_NM_PDU_RX_INDICATION_ENABLED == STD_ON)
        Nm_PduRxIndication(NMOSEK_NMTOSYSTEM_CHANNEL_IND);
# endif
#endif
      } /* (NmOsek_RxIndication_DetermineWhetherMessageIsValidAndUpdateStatusFlags() == TRUE) */

#if (NMOSEK_NM_STATE_CHANGE_IND_ENABLED == STD_ON)
      /* - #90 notify about a state change if occurred */
      /* ESCAN00049564 */
      NmOsek_Nm_StateChangeNotificationOnStateChange(NMOSEK_CHANNEL_PARA_ONLY);
#endif

      SchM_Exit_NmOsek_NMOSEK_EXCLUSIVE_AREA_0(); /* ESCAN00052383 */ /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

#if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT(nmOsekRxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  /* ----- Development Error Report --------------------------------------- */
#if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_RXINDICATION, errorId);
  }
#else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
} /* PRQA S 6030,6050,6080 */ /* MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

/**********************************************************************************************************************
 *  NmOsek_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, NMOSEK_CODE) NmOsek_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (NMOSEK_USE_INIT_POINTER == STD_OFF)
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;
#endif
#if (NMOSEK_API_OPTIMIZATION == STD_OFF)
  VAR(NetworkHandleType, AUTOMATIC) NMOSEK_CHANNEL_IDX;
#endif

#if (NMOSEK_DEV_ERROR_DETECT == STD_ON) && (NMOSEK_USE_INIT_POINTER == STD_OFF)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
#endif
  {
#if (NMOSEK_USE_INIT_POINTER == STD_ON)
    if (NmOsek_ConfigPtr != NULL_PTR)
#endif
    {
      /* ESCAN00055975 */

      /* - #20 perform the following for each channel: */
#if (NMOSEK_API_OPTIMIZATION == STD_OFF)
      for (NMOSEK_CHANNEL_IDX = 0u; NMOSEK_CHANNEL_IDX < NmOsek_GetSizeOfChannelConfig(); NMOSEK_CHANNEL_IDX++)
#endif
      {
        /* - #30 Update task counter */
        NmOsek_MainFunction_UpdateTaskCounter(NMOSEK_CHANNEL_PARA_ONLY);

#if (NMOSEK_SYNCHRONIZINGNETWORKOFCHANNELCONFIG == STD_ON)
        if (NmOsek_IsSynchronizingNetworkOfChannelConfig(NMOSEK_CHANNEL_IDX))
        {
          NmOsek_IncRingCycleTimer(NMOSEK_CHANNEL_IDX);
        }
#endif

#if (NMOSEK_MSGTIMEOUTTIMER == STD_ON)
        NmOsek_MainFunction_UpdateMsgTimeoutTimer(NMOSEK_CHANNEL_PARA_ONLY);
#endif

        /* Code: loop for immediate switch to next state in time-critical cases */
        do
        {
          /* Event processing */
          SchM_Enter_NmOsek_NMOSEK_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

          /* Remove repetition flag */ /* ESCAN00074498 */
          NmOsek_SetAction(NMOSEK_CHANNEL_IDX, (NmOsek_ActionType)
            (NmOsek_GetAction(NMOSEK_CHANNEL_IDX) & NMOSEK_ACTION_REPEAT_INV));

          /* ESCAN00055975 */

#if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
          /* - #40 Processing of event: BusOff handling (1) */
          if ((NmOsek_IsCanIsBusOff(NMOSEK_CHANNEL_IDX)) &&
              ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_ERROR_FLAG_MASK) == 0u))
          {
            /* BusOff occurrence notification */
            NmOsek_MainFunction_BusOffOccurrence(NMOSEK_CHANNEL_PARA_ONLY);
          }
          /* - #50 Processing of event: BusOff recovery (2) */
          else if ((!NmOsek_IsCanIsBusOff(NMOSEK_CHANNEL_IDX)) && /* ESCAN00074498 */
                   ((NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_BUS_ERROR_FLAG_MASK) != 0u))
          {
            NmOsek_MainFunction_BusOffRecovery(NMOSEK_CHANNEL_PARA_ONLY);
          }
          else
          {
            /* Intentionally left empty */
          }
          /* Further actions for BusOff recovery notification are performed later on */
#endif /* (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON) */

#if (NMOSEK_DESIREDNMSTATE == STD_ON)
          /* - #90 Change Nm State if there is a new desired state in case it is possible now */
          if (NmOsek_GetDesiredNmState(NMOSEK_CHANNEL_IDX) != NmOsek_GetNmState(NMOSEK_CHANNEL_IDX))
          {
            NmOsek_ChangeNmStateIfPossible(NMOSEK_CHANNEL_PARA_FIRST NmOsek_GetDesiredNmState(NMOSEK_CHANNEL_IDX));
          }
#endif

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
          /* - #100 Check NM TX counter */
          NmOsek_MainFunction_CheckNmTxCounter(NMOSEK_CHANNEL_PARA_ONLY);
#endif

          /* - #110 Process NM state machine */
          switch (NmOsek_GetAction(NMOSEK_CHANNEL_IDX))
          {
           /*----------------------------------------------------------*/
            case NMOSEK_ACTION_START:
           /*----------------------------------------------------------*/
              NmOsek_MainFunction_ActionStart(NMOSEK_CHANNEL_PARA_ONLY);
              break;

           /*----------------------------------------------------------*/
            case NMOSEK_ACTION_WAIT_SENDTOKEN:
           /*----------------------------------------------------------*/
              NmOsek_MainFunction_ActionWaitSendtoken(NMOSEK_CHANNEL_PARA_ONLY);
              break;

           /*----------------------------------------------------------*/
            case NMOSEK_ACTION_WAIT_RINGMSG:
           /*----------------------------------------------------------*/
              NmOsek_MainFunction_ActionWaitRingmsg(NMOSEK_CHANNEL_PARA_ONLY);
              break;

           /*----------------------------------------------------------*/
            case NMOSEK_ACTION_LH_WAIT_LIMPHOME_TX:
            case NMOSEK_ACTION_LH_PREPARE_SLEEP:
           /*----------------------------------------------------------*/
              NmOsek_MainFunction_ActionsLhWaitLimphomeTxAndLhPrepareSleep(NMOSEK_CHANNEL_PARA_ONLY);
              break;

            /*----------------------------------------------------------*/
            case NMOSEK_ACTION_GO_BUSSLEEP:
            /* ESCAN00052619 */
            /*----------------------------------------------------------*/
              NmOsek_MainFunction_ActionGoBussleep(NMOSEK_CHANNEL_PARA_ONLY);
              break;

            /*----------------------------------------------------------*/
            case NMOSEK_ACTION_BUS_SLEEP:
            /* ESCAN00062285 */
            /*----------------------------------------------------------*/
              NmOsek_MainFunction_ActionBusSleep(NMOSEK_CHANNEL_PARA_ONLY);
              break;

            /*----------------------------------------------------------*/
            default: /* includes NMOSEK_NO_ACTION and NMOSEK_ACTION_REPEAT */
            /*----------------------------------------------------------*/
              NmOsek_MainFunction_NoAction(NMOSEK_CHANNEL_PARA_ONLY);
              break;
          }

          /* - #120 Clear RX Flags in internal state */
          NmOsek_SetInternalState(NMOSEK_CHANNEL_IDX, (NmOsek_InternalStateType)
            (NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX) & NMOSEK_INTERNAL_STATE_NM_STATUS_RX_FLAGS_INV_MASK));

#if (NMOSEK_NM_STATE_CHANGE_IND_ENABLED == STD_ON)
          /* - #130 Notify State Change if there was a state change */
          /* ESCAN00049564 */
          NmOsek_Nm_StateChangeNotificationOnStateChange(NMOSEK_CHANNEL_PARA_ONLY);
#endif

          SchM_Exit_NmOsek_NMOSEK_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        /*----------------------------------------------------------*/
        } while ((NmOsek_GetAction(NMOSEK_CHANNEL_IDX) & NMOSEK_ACTION_REPEAT) != 0u);
        /*----------------------------------------------------------*/

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
        /* - #150 Issue TX Request if demanded and possible */
        NmOsek_MainFunction_IssueTxRequestIfDemandedAndPossible(NMOSEK_CHANNEL_PARA_ONLY);
#endif /* (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF) */
      } /* (for) */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (NMOSEK_USE_INIT_POINTER == STD_OFF)
# if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_MAINFUNCTION, errorId);
  }
# else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
#endif
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */

#if (NMOSEK_DEV_ERROR_DETECT == STD_ON) || (NMOSEK_USE_INIT_POINTER == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, NMOSEK_CODE) NmOsek_InitMemory(void)
{
 /* - #10 Initialize the following variables with the corresponding values:
  *       NmOsek_ModuleInitialized = NMOSEK_UNINIT
  *       NmOsek_ConfigPtr = NULL_PTR
  */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  NmOsek_SetModuleInitialized(NMOSEK_UNINIT);
# endif
# if (NMOSEK_USE_INIT_POINTER == STD_ON)
  NmOsek_ConfigPtr = NULL_PTR;
# endif
}
#endif

/**********************************************************************************************************************
 *  NmOsek_GetStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, NMOSEK_CODE) NmOsek_GetStatus(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                   CONSTP2VAR(uint8, AUTOMATIC, NMOSEK_APPL_VAR) nmStatusPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  VAR(Std_ReturnType, AUTOMATIC) retVal = E_NOT_OK;
#else
  VAR(Std_ReturnType, AUTOMATIC) retVal;
#endif
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check system channel index and NM channel index */
# if (NMOSEK_API_OPTIMIZATION == STD_ON)
  if (nmChannelHandle != NMOSEK_SYSTEM_CHANNEL_IDX)
# else
  if ((nmChannelHandle >= NmOsek_GetNumberOfSystemChannels())
#  if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      || (NmOsek_GetSysToNmChannelInd(nmChannelHandle) >= NmOsek_GetSizeOfChannelConfig())
#  endif
     )
# endif
  {
    errorId = NMOSEK_E_INVALID_CHANNEL;
  }
  else
  /* - #30 Check for NULL pointer */
  if ((nmStatusPtr) == NULL_PTR)
  {
    errorId = NMOSEK_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
    /* Create channel handle of Nm instance (in post-compile or multiple channel configurations). */
    VAR(NetworkHandleType, AUTOMATIC) channel = NmOsek_GetSysToNmChannelInd(nmChannelHandle);
#endif

    /* - #40 Copy global status byte */
    *nmStatusPtr = (uint8)NmOsek_GetInternalState(NMOSEK_CHANNEL_IDX);

    retVal = E_OK;
  }

#if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  /* ----- Development Error Report --------------------------------------- */
#if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_GETSTATUS, errorId);
  }
#else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
}

#if (NMOSEK_USE_NM_NODE_LIST == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_GetConfig()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, NMOSEK_CODE) NmOsek_GetConfig(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                         CONSTP2VAR(NmOsek_NodeConfigType, AUTOMATIC, NMOSEK_APPL_VAR) nmDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;
  VAR(uint8, AUTOMATIC) configByte;

  /* ----- Development Error Checks ------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check system channel index and NM channel index */
#  if (NMOSEK_API_OPTIMIZATION == STD_ON)
  if (nmChannelHandle != NMOSEK_SYSTEM_CHANNEL_IDX)
#  else
  if ((nmChannelHandle >= NmOsek_GetNumberOfSystemChannels())
#   if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      || (NmOsek_GetSysToNmChannelInd(nmChannelHandle) >= NmOsek_GetSizeOfChannelConfig())
#   endif
     )
#  endif
  {
    errorId = NMOSEK_E_INVALID_CHANNEL;
  }
  else
  /* - #30 Check for NULL pointer */
  if ((nmDataPtr) == NULL_PTR)
  {
    errorId = NMOSEK_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
    /* Create channel handle of Nm instance (in post-compile or multiple channel configurations). */
    VAR(NetworkHandleType, AUTOMATIC) channel = NmOsek_GetSysToNmChannelInd(nmChannelHandle);
# endif

    /* - #40 Copy configuration bytes to provided pointer */
    SchM_Enter_NmOsek_NMOSEK_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

# if (NMOSEK_API_OPTIMIZATION == STD_ON)
    for (configByte = 0u;
         configByte < NMOSEK_NUMBER_OF_NM_CONFIG_NODES_DIV_8;
         configByte++)
# else
    for (configByte = (uint8)((uint8)NMOSEK_CHANNEL_IDX * (uint8)NMOSEK_NUMBER_OF_NM_CONFIG_NODES_DIV_8);
         configByte < (uint8)((uint8)(NMOSEK_CHANNEL_IDX + 1u) * (uint8)NMOSEK_NUMBER_OF_NM_CONFIG_NODES_DIV_8);
         configByte++)
# endif
    {
      /* Copy data */
      nmDataPtr->nmConfigChar[configByte] = NmOsek_GetNodeConfiguration().nmConfigChar[configByte];
    }

    SchM_Exit_NmOsek_NMOSEK_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

# if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  /* ----- Development Error Report --------------------------------------- */
# if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_GETCONFIG, errorId);
  }
# else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
}

/**********************************************************************************************************************
 *  NmOsek_CmpConfig()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, NMOSEK_CODE) NmOsek_CmpConfig(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                            CONST(uint8, AUTOMATIC) idx)
{
  /* ----- Local Variables ---------------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  VAR(boolean, AUTOMATIC) retVal = FALSE;
# else
  VAR(boolean, AUTOMATIC) retVal;
# endif
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;
  VAR(uint8, AUTOMATIC) configByte;

  /* ----- Development Error Checks ------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* - #20 Check system channel index and NM channel index */
#  if (NMOSEK_API_OPTIMIZATION == STD_ON)
  if (nmChannelHandle != NMOSEK_SYSTEM_CHANNEL_IDX)
#  else
  if ((nmChannelHandle >= NmOsek_GetNumberOfSystemChannels())
#   if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      || (NmOsek_GetSysToNmChannelInd(nmChannelHandle) >= NmOsek_GetSizeOfChannelConfig())
#   endif
     )
#  endif
  {
    errorId = NMOSEK_E_INVALID_CHANNEL;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
    /* Create channel handle of Nm instance (in post-compile or multiple channel configurations). */
    VAR(NetworkHandleType, AUTOMATIC) channel = NmOsek_GetSysToNmChannelInd(nmChannelHandle);
# endif

    SchM_Enter_NmOsek_NMOSEK_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* - #30 Compare configuration data with expected data and return TRUE if it matches, FALSE otherwise */
    retVal = TRUE;

# if (NMOSEK_API_OPTIMIZATION == STD_ON)
    for (configByte = 0u;
         configByte < NMOSEK_NUMBER_OF_NM_CONFIG_NODES_DIV_8;
         configByte++)
# else
    for (configByte = (uint8)((uint8)NMOSEK_CHANNEL_IDX * (uint8)NMOSEK_NUMBER_OF_NM_CONFIG_NODES_DIV_8);
         configByte < (uint8)((uint8)(NMOSEK_CHANNEL_IDX + 1u) * (uint8)NMOSEK_NUMBER_OF_NM_CONFIG_NODES_DIV_8);
         configByte++)
# endif
    {
      /* Compare data */
      if (((NmOsek_TargetConfigTable[idx].nmConfigChar[configByte] ^
            NmOsek_GetNodeConfiguration().nmConfigChar[configByte]) &
           NmOsek_ConfigMaskTable[idx].nmConfigChar[configByte]) != 0u)
      {
        retVal = FALSE;
        break;
      }
    }

    SchM_Exit_NmOsek_NMOSEK_EXCLUSIVE_AREA_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

# if (NMOSEK_DEV_ERROR_DETECT == STD_OFF) && (NMOSEK_API_OPTIMIZATION == STD_ON)
  NMOSEK_DUMMY_STATEMENT_CONST(nmChannelHandle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  /* ----- Development Error Report --------------------------------------- */
# if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_CMPCONFIG, errorId);
  }
# else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
}
#endif /* (NMOSEK_USE_NM_NODE_LIST == STD_ON) */

#if (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  NmOsek_CanSM_BusOffBegin()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 3673 2 */ /* MD_NmOsek_3673 */
FUNC(void, NMOSEK_CODE) NmOsek_CanSM_BusOffBegin(NetworkHandleType NetworkHandle,
                                                 P2VAR(uint8, AUTOMATIC, NMOSEK_APPL_VAR) OnlineDelayCyclesPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
# if (NMOSEK_USE_INIT_POINTER == STD_OFF)
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON) && (NMOSEK_USE_INIT_POINTER == STD_OFF)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* No channel check here, invalid channels are just being ignored */
# endif
  {
    /* ----- Implementation ----------------------------------------------- */

    /* - #20 Set BusOff occurrence flag if NmOsek is attached to this channel */
    if (
# if (NMOSEK_USE_INIT_POINTER == STD_ON)
        (NmOsek_ConfigPtr != NULL_PTR) &&
# endif
# if (NMOSEK_API_OPTIMIZATION == STD_ON)
        (NetworkHandle == NMOSEK_SYSTEM_CHANNEL_IDX)
# else
        (NetworkHandle < NmOsek_GetNumberOfSystemChannels())
#  if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
        && (NmOsek_GetSysToNmChannelInd(NetworkHandle) < NmOsek_GetSizeOfChannelConfig())
#  endif
# endif
       )
    {
# if (NMOSEK_API_OPTIMIZATION == STD_ON)
      NmOsek_SetCanIsBusOff(0, TRUE);
# else
#  if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      NmOsek_SetCanIsBusOff(NmOsek_GetSysToNmChannelInd(NetworkHandle), TRUE);
#  else
      NmOsek_SetCanIsBusOff(NetworkHandle, TRUE);
#  endif
# endif
    }
    else
    {
      /* Notification is being ignored for inactive/invalid channels */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (NMOSEK_USE_INIT_POINTER == STD_OFF)
#  if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_CANSM_BUSOFFBEGIN, errorId);
  }
#  else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
# endif

  /* Value of CanSM_BusOffNotificationCounter is being ignored */
  NMOSEK_DUMMY_STATEMENT(OnlineDelayCyclesPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
}

/**********************************************************************************************************************
 *  NmOsek_CanSM_BusOffEnd()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, NMOSEK_CODE) NmOsek_CanSM_BusOffEnd(NetworkHandleType NetworkHandle)
{
  /* ----- Local Variables ---------------------------------------------- */
# if (NMOSEK_USE_INIT_POINTER == STD_OFF)
  VAR(uint8, AUTOMATIC) errorId = NMOSEK_E_NO_ERROR;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (NMOSEK_DEV_ERROR_DETECT == STD_ON) && (NMOSEK_USE_INIT_POINTER == STD_OFF)
  /* - #10 Check if component is initialized */
  if (NmOsek_GetModuleInitialized() == (uint8)NMOSEK_UNINIT)
  {
    errorId = NMOSEK_E_UNINIT;
  }
  else
  /* No channel check here, invalid channels are just being ignored */
# endif
  {
    /* ----- Implementation ----------------------------------------------- */

    /* - #20 Clear BusOff occurrence flag if NmOsek is attached to this channel */
    if (
# if (NMOSEK_USE_INIT_POINTER == STD_ON)
        (NmOsek_ConfigPtr != NULL_PTR) &&
# endif
# if (NMOSEK_API_OPTIMIZATION == STD_ON)
        (NetworkHandle == NMOSEK_SYSTEM_CHANNEL_IDX)
# else
        (NetworkHandle < NmOsek_GetNumberOfSystemChannels())
#  if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
        && (NmOsek_GetSysToNmChannelInd(NetworkHandle) < NmOsek_GetSizeOfChannelConfig())
#  endif
# endif
       )
    {
# if (NMOSEK_API_OPTIMIZATION == STD_ON)
      NmOsek_SetCanIsBusOff(0, FALSE);
# else
#  if (NMOSEK_SYSTONMCHANNELIND == STD_ON)
      NmOsek_SetCanIsBusOff(NmOsek_GetSysToNmChannelInd(NetworkHandle), FALSE);
#  else
      NmOsek_SetCanIsBusOff(NetworkHandle, FALSE);
#  endif
# endif
    }
    else
    {
      /* Notification is being ignored for inactive/invalid channels */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (NMOSEK_USE_INIT_POINTER == STD_OFF)
#  if (NMOSEK_DEV_ERROR_REPORT == STD_ON)
  if (errorId != NMOSEK_E_NO_ERROR)
  {
    (void)Det_ReportError(NMOSEK_MODULE_ID, NMOSEK_INSTANCE_ID, NMOSEK_SID_CANSM_BUSOFFEND, errorId);
  }
#  else
  NMOSEK_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
# endif
}
#endif /* (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED == STD_ON) */

#define NMOSEK_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  CONFIGURATION VERIFICATION
 *********************************************************************************************************************/

/* Check Error Detection / Diagnostic switches */
#if (!defined(NMOSEK_DEV_ERROR_DETECT) || \
     ((NMOSEK_DEV_ERROR_DETECT != STD_OFF) && (NMOSEK_DEV_ERROR_DETECT != STD_ON)))
# error "NmOsek configuration issue: Development Error Detection Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_DEV_ERROR_REPORT) || \
     ((NMOSEK_DEV_ERROR_REPORT != STD_OFF) && (NMOSEK_DEV_ERROR_REPORT != STD_ON)))
# error "NmOsek configuration issue: Development Error Report Enabled has to be defined STD_OFF or STD_ON"
#endif

/* Check NmOsek Configuration switches */
#if (!defined(NMOSEK_POSTBUILD_VARIANT_SUPPORT) || \
     ((NMOSEK_POSTBUILD_VARIANT_SUPPORT != STD_OFF) && (NMOSEK_POSTBUILD_VARIANT_SUPPORT != STD_ON)))
# error "NmOsek configuration issue: Post-build Variant Support has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_USE_ECUM_BSW_ERROR_HOOK) || \
     ((NMOSEK_USE_ECUM_BSW_ERROR_HOOK != STD_OFF) && (NMOSEK_USE_ECUM_BSW_ERROR_HOOK != STD_ON)))
# error "NmOsek configuration issue: Use EcuM Bsw Error Hook has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_USE_INIT_POINTER) || \
     ((NMOSEK_USE_INIT_POINTER != STD_OFF) && (NMOSEK_USE_INIT_POINTER != STD_ON)))
# error "NmOsek configuration issue: Use Init Pointer has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_API_OPTIMIZATION) || \
     ((NMOSEK_API_OPTIMIZATION != STD_OFF) && (NMOSEK_API_OPTIMIZATION != STD_ON)))
# error "NmOsek configuration issue: Api Optimization has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_USE_NM_NODE_LIST) || \
     ((NMOSEK_USE_NM_NODE_LIST != STD_OFF) && (NMOSEK_USE_NM_NODE_LIST != STD_ON)))
# error "NmOsek configuration issue: Use NM Node List has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_TOKEN_MONITORING) || \
     ((NMOSEK_TOKEN_MONITORING != STD_OFF) && (NMOSEK_TOKEN_MONITORING != STD_ON)))
# error "NmOsek configuration issue: Token Monitoring has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_VERSION_INFO_API) || \
     ((NMOSEK_VERSION_INFO_API != STD_OFF) && (NMOSEK_VERSION_INFO_API != STD_ON)))
# error "NmOsek configuration issue: Version Info Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_INSTANCE_ID) || \
     ((NMOSEK_INSTANCE_ID < 0u) && (NMOSEK_INSTANCE_ID >= 256u)))
# error "NmOsek configuration issue: Instance Id has to be defined to a value greater than or equal to 0 and less than 256"
#endif
#if (!defined(NMOSEK_BUS_OFF_NOTIFICATION_ENABLED) || \
     ((NMOSEK_BUS_OFF_NOTIFICATION_ENABLED != STD_OFF) && (NMOSEK_BUS_OFF_NOTIFICATION_ENABLED != STD_ON)))
# error "NmOsek configuration issue: BusOff Notification Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_LAST_BYTE_OF_CAN_ID_AS_NODE_ID_ENABLED) || \
     ((NMOSEK_LAST_BYTE_OF_CAN_ID_AS_NODE_ID_ENABLED != STD_OFF) && (NMOSEK_LAST_BYTE_OF_CAN_ID_AS_NODE_ID_ENABLED != STD_ON)))
# error "NmOsek configuration issue: Last Byte of CAN ID as Node ID has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_RESET_TMAX_IN_LHPREP_SLEEP_ON_RX_RING_ENABLED) || \
     ((NMOSEK_RESET_TMAX_IN_LHPREP_SLEEP_ON_RX_RING_ENABLED != STD_OFF) && (NMOSEK_RESET_TMAX_IN_LHPREP_SLEEP_ON_RX_RING_ENABLED != STD_ON)))
# error "NmOsek configuration issue: Reset TMax in LHPrepSleep On Rx Ring Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_NMRESET_IGNORE_RX_EXCEPT_ALIVE_ENABLED) || \
     ((NMOSEK_NMRESET_IGNORE_RX_EXCEPT_ALIVE_ENABLED != STD_OFF) && (NMOSEK_NMRESET_IGNORE_RX_EXCEPT_ALIVE_ENABLED != STD_ON)))
# error "NmOsek configuration issue: NMReset Ignore Rx Except Alive Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_LEAVE_LHONLY_ON_RX_INDICATION_ENABLED) || \
     ((NMOSEK_LEAVE_LHONLY_ON_RX_INDICATION_ENABLED != STD_OFF) && (NMOSEK_LEAVE_LHONLY_ON_RX_INDICATION_ENABLED != STD_ON)))
# error "NmOsek configuration issue: Leave LH only on RxIndication Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_WBSTRANSITION_ON_RING_MSG_ONLY_ENABLED) || \
     ((NMOSEK_WBSTRANSITION_ON_RING_MSG_ONLY_ENABLED != STD_OFF) && (NMOSEK_WBSTRANSITION_ON_RING_MSG_ONLY_ENABLED != STD_ON)))
# error "NmOsek configuration issue: WBS Transition on Ring Msg Only Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_NUMBER_OF_NM_CONFIG_NODES_DIV_8) || \
     ((NMOSEK_NUMBER_OF_NM_CONFIG_NODES_DIV_8 <= 0u) && (NMOSEK_NUMBER_OF_NM_CONFIG_NODES_DIV_8 > 32u)))
# error "NmOsek configuration issue: Number Of Nm Config Nodes Div 8 has to be defined to a value greater than 0 and to a value less than or equal to 32"
#endif
#if (!defined(NMOSEK_ALL_LIMP_HOME_SHORT_NUMBER_GREATER_THAN_ONE) || \
     ((NMOSEK_ALL_LIMP_HOME_SHORT_NUMBER_GREATER_THAN_ONE != STD_OFF) && (NMOSEK_ALL_LIMP_HOME_SHORT_NUMBER_GREATER_THAN_ONE != STD_ON)))
# error "NmOsek configuration issue: All Limp Home Short Number Greater Than One has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_ANYCHANNELINLIMPHOME) || \
     ((NMOSEK_ANYCHANNELINLIMPHOME != STD_OFF) && (NMOSEK_ANYCHANNELINLIMPHOME != STD_ON)))
# error "NmOsek configuration issue: Any Channel In LimpHome has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_AWAKETXUSERDATAPDUIDOFPBCHANNELCONFIG) || \
     ((NMOSEK_AWAKETXUSERDATAPDUIDOFPBCHANNELCONFIG != STD_OFF) && (NMOSEK_AWAKETXUSERDATAPDUIDOFPBCHANNELCONFIG != STD_ON)))
# error "NmOsek configuration issue: Awake Tx User Data Pdu Id of Pb Channel Config has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_CONFIRMTXUSERDATAPDUOFPBCHANNELCONFIG) || \
     ((NMOSEK_CONFIRMTXUSERDATAPDUOFPBCHANNELCONFIG != STD_OFF) && (NMOSEK_CONFIRMTXUSERDATAPDUOFPBCHANNELCONFIG != STD_ON)))
# error "NmOsek configuration issue: Confirm Tx User Data Pdu of Pb Channel Config has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_DESIREDNMSTATE) || \
     ((NMOSEK_DESIREDNMSTATE != STD_OFF) && (NMOSEK_DESIREDNMSTATE != STD_ON)))
# error "NmOsek configuration issue: Desired Nm State has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_FAST_BUSOFF_RECOVERY) || \
     ((NMOSEK_FAST_BUSOFF_RECOVERY != STD_OFF) && (NMOSEK_FAST_BUSOFF_RECOVERY != STD_ON)))
# error "NmOsek configuration issue: Fast BusOff Recovery has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_FINALMAGICNUMBER) || \
     ((NMOSEK_FINALMAGICNUMBER != STD_OFF) && (NMOSEK_FINALMAGICNUMBER != STD_ON)))
# error "NmOsek configuration issue: Final Magic Number has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE) || \
     ((NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE != STD_OFF) && (NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE != STD_ON)))
# error "NmOsek configuration issue: First Message shall be Nm Message has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_TX_DEADLINE_MONITORING_IN_COM_ENABLED) || \
     ((NMOSEK_TX_DEADLINE_MONITORING_IN_COM_ENABLED != STD_OFF) && (NMOSEK_TX_DEADLINE_MONITORING_IN_COM_ENABLED != STD_ON)))
# error "NmOsek configuration issue: Tx Deadline Monitoring in Com Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_GENERATORCOMPATIBILITYVERSION) || \
     ((NMOSEK_GENERATORCOMPATIBILITYVERSION != STD_OFF) && (NMOSEK_GENERATORCOMPATIBILITYVERSION != STD_ON)))
# error "NmOsek configuration issue: Generator Compatibility Version has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_MAX_NUMBER_OF_NMOSEK_CHANNELS) || \
     (NMOSEK_MAX_NUMBER_OF_NMOSEK_CHANNELS <= 0U))
# error "NmOsek configuration issue: Max Number of NmOsek Channels has to be defined to a value greater than 0"
#endif
#if (!defined(NMOSEK_MSGTIMEOUTTIMER) || \
     ((NMOSEK_MSGTIMEOUTTIMER != STD_OFF) && (NMOSEK_MSGTIMEOUTTIMER != STD_ON)))
# error "NmOsek configuration issue: Msg Timeout Timer has to be defined STD_OFF or STD_ON"
#endif
#if (NMOSEK_NO_OPTIMIZATION == NMOSEK_LOWEST_NODE_ID_IN_RANGE) || \
    (NMOSEK_NO_OPTIMIZATION == NMOSEK_HIGHEST_NODE_ID_IN_RANGE) || \
    (NMOSEK_LOWEST_NODE_ID_IN_RANGE == NMOSEK_HIGHEST_NODE_ID_IN_RANGE)
# error "NmOsek configuration issue: at least two literals for the NMOSEK_NODE_ID_OPTIMIZATION setting have the same value"
#endif
#if (!defined(NMOSEK_NODE_ID_OPTIMIZATION) || \
     ((NMOSEK_NODE_ID_OPTIMIZATION != NMOSEK_NO_OPTIMIZATION) && (NMOSEK_NODE_ID_OPTIMIZATION != NMOSEK_LOWEST_NODE_ID_IN_RANGE) && (NMOSEK_NODE_ID_OPTIMIZATION != NMOSEK_HIGHEST_NODE_ID_IN_RANGE)))
# error "NmOsek configuration issue: Node Id Optimization has to be defined NMOSEK_NO_OPTIMIZATION, NMOSEK_LOWEST_NODE_ID_IN_RANGE or NMOSEK_HIGHEST_NODE_ID_IN_RANGE"
#endif
#if (!defined(NMOSEK_ONLY_VALID_USER_DATA_TX_PDU_IDS) || \
     ((NMOSEK_ONLY_VALID_USER_DATA_TX_PDU_IDS != STD_OFF) && (NMOSEK_ONLY_VALID_USER_DATA_TX_PDU_IDS != STD_ON)))
# error "NmOsek configuration issue: Only Valid User Data Tx Pdu Ids has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_PDUMESSAGEDATASTARTPOSOFPBCHANNELCONFIG) || \
     ((NMOSEK_PDUMESSAGEDATASTARTPOSOFPBCHANNELCONFIG != STD_OFF) && (NMOSEK_PDUMESSAGEDATASTARTPOSOFPBCHANNELCONFIG != STD_ON)))
# error "NmOsek configuration issue: Pdu Message Data Start Pos of Pb Channel Config has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_SYNCHRONIZINGNETWORKOFCHANNELCONFIG) || \
     ((NMOSEK_SYNCHRONIZINGNETWORKOFCHANNELCONFIG != STD_OFF) && (NMOSEK_SYNCHRONIZINGNETWORKOFCHANNELCONFIG != STD_ON)))
# error "NmOsek configuration issue: Synchronizing Network of Channel Config has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_SYNCPOINTINTALLINNORMALOFCHANNELCONFIG) || \
     ((NMOSEK_SYNCPOINTINTALLINNORMALOFCHANNELCONFIG != STD_OFF) && (NMOSEK_SYNCPOINTINTALLINNORMALOFCHANNELCONFIG != STD_ON)))
# error "NmOsek configuration issue: Sync Point Int All In Normal of Channel Config has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_SYNCPOINTINTMODLHSHORTTIMEOFCHANNELCONFIG) || \
     ((NMOSEK_SYNCPOINTINTMODLHSHORTTIMEOFCHANNELCONFIG != STD_OFF) && (NMOSEK_SYNCPOINTINTMODLHSHORTTIMEOFCHANNELCONFIG != STD_ON)))
# error "NmOsek configuration issue: Sync Point Int Mod Lh Short Time of Channel Config has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_SYNCPOINTINTMODLHTIMEOFCHANNELCONFIG) || \
     ((NMOSEK_SYNCPOINTINTMODLHTIMEOFCHANNELCONFIG != STD_OFF) && (NMOSEK_SYNCPOINTINTMODLHTIMEOFCHANNELCONFIG != STD_ON)))
# error "NmOsek configuration issue: Sync Point Int Mod Lh Time of Channel Config has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_SYNCPOINTNETWORKRELEASEINTERVALOFCHANNELCONFIG) || \
     ((NMOSEK_SYNCPOINTNETWORKRELEASEINTERVALOFCHANNELCONFIG != STD_OFF) && (NMOSEK_SYNCPOINTNETWORKRELEASEINTERVALOFCHANNELCONFIG != STD_ON)))
# error "NmOsek configuration issue: Sync Point Network Release Interval of Channel Config has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_SYNCPOINTTIMER) || \
     ((NMOSEK_SYNCPOINTTIMER != STD_OFF) && (NMOSEK_SYNCPOINTTIMER != STD_ON)))
# error "NmOsek configuration issue: Sync Point Timer of Channel Config has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_SYSTONMCHANNELIND) || \
     ((NMOSEK_SYSTONMCHANNELIND != STD_OFF) && (NMOSEK_SYSTONMCHANNELIND != STD_ON)))
# error "NmOsek configuration issue: Sys To Nm Channel Ind has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_WAKEUPTXUSERDATAPDUIDOFPBCHANNELCONFIG) || \
     ((NMOSEK_WAKEUPTXUSERDATAPDUIDOFPBCHANNELCONFIG != STD_OFF) && (NMOSEK_WAKEUPTXUSERDATAPDUIDOFPBCHANNELCONFIG != STD_ON)))
# error "NmOsek configuration issue: Wake-up Tx User Data Pdu Id of Pb Channel Config has to be defined STD_OFF or STD_ON"
#endif

/* Check Nm configuration settings */
#if (!defined(NMOSEK_NM_BUS_NM_SPECIFIC_PDU_RX_INDICATION_ENABLED) || \
     ((NMOSEK_NM_BUS_NM_SPECIFIC_PDU_RX_INDICATION_ENABLED != STD_OFF) && (NMOSEK_NM_BUS_NM_SPECIFIC_PDU_RX_INDICATION_ENABLED != STD_ON)))
# error "NmOsek configuration issue: Nm BusNm Specific Pdu Rx Indication Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_NM_BUS_SYNCHRONIZATION_ENABLED) || \
     ((NMOSEK_NM_BUS_SYNCHRONIZATION_ENABLED != STD_OFF) && (NMOSEK_NM_BUS_SYNCHRONIZATION_ENABLED != STD_ON)))
# error "NmOsek configuration issue: Nm Bus Synchronization Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_NM_COM_CONTROL_ENABLED) || \
     ((NMOSEK_NM_COM_CONTROL_ENABLED != STD_OFF) && (NMOSEK_NM_COM_CONTROL_ENABLED != STD_ON)))
# error "NmOsek configuration issue: Nm Com Control Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_NM_COM_USER_DATA_SUPPORT) || \
     ((NMOSEK_NM_COM_USER_DATA_SUPPORT != STD_OFF) && (NMOSEK_NM_COM_USER_DATA_SUPPORT != STD_ON)))
# error "NmOsek configuration issue: Nm Com User Data Support has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_NM_NODE_DETECTION_ENABLED) || \
     ((NMOSEK_NM_NODE_DETECTION_ENABLED != STD_OFF) && (NMOSEK_NM_NODE_DETECTION_ENABLED != STD_ON)))
# error "NmOsek configuration issue: Nm Node Detection Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_NM_NODE_ID_ENABLED) || \
     ((NMOSEK_NM_NODE_ID_ENABLED != STD_OFF) && (NMOSEK_NM_NODE_ID_ENABLED != STD_ON)))
# error "NmOsek configuration issue: Nm Node Id Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_NM_PASSIVE_MODE_ENABLED) || \
     ((NMOSEK_NM_PASSIVE_MODE_ENABLED != STD_OFF) && (NMOSEK_NM_PASSIVE_MODE_ENABLED != STD_ON)))
# error "NmOsek configuration issue: Nm Passive Mode Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_NM_PDU_RX_INDICATION_ENABLED) || \
     ((NMOSEK_NM_PDU_RX_INDICATION_ENABLED != STD_OFF) && (NMOSEK_NM_PDU_RX_INDICATION_ENABLED != STD_ON)))
# error "NmOsek configuration issue: Nm Pdu Rx Indication Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED) || \
     ((NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED != STD_OFF) && (NMOSEK_NM_REMOTE_SLEEP_IND_ENABLED != STD_ON)))
# error "NmOsek configuration issue: Nm Remote Sleep Ind Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_NM_STATE_CHANGE_IND_ENABLED) || \
     ((NMOSEK_NM_STATE_CHANGE_IND_ENABLED != STD_OFF) && (NMOSEK_NM_STATE_CHANGE_IND_ENABLED != STD_ON)))
# error "NmOsek configuration issue: Nm State Change Ind Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_NM_USER_DATA_ENABLED) || \
     ((NMOSEK_NM_USER_DATA_ENABLED != STD_OFF) && (NMOSEK_NM_USER_DATA_ENABLED != STD_ON)))
# error "NmOsek configuration issue: Nm User Data Enabled has to be defined STD_OFF or STD_ON"
#endif
#if (!defined(NMOSEK_HIGHEST_NODE_ID_IN_RANGE) || !defined(NMOSEK_LOWEST_NODE_ID_IN_RANGE) || !defined(NMOSEK_NO_OPTIMIZATION))
# error "NmOsek configuration issue: at least one literal for the NMOSEK_NODE_ID_OPTIMIZATION setting is not defined"
#endif

#if (NMOSEK_USE_NM_NODE_LIST == STD_ON)
# if !defined(NMOSEK_NUMBER_OF_NM_CONFIG_NODES)
#  error "NmOsek configuration issue: NMOSEK_NUMBER_OF_NM_CONFIG_NODES must be defined if NMOSEK_USE_NM_NODE_LIST == STD_ON."
# else
#  if !defined(NMOSEK_NUMBER_OF_NM_CONFIG_NODES_DIV_8)
#   error "NmOsek configuration issue: NMOSEK_NUMBER_OF_NM_CONFIG_NODES_DIV_8 must be defined if NMOSEK_USE_NM_NODE_LIST == STD_ON."
#  else
#   if (NMOSEK_NUMBER_OF_NM_CONFIG_NODES !=   1) && (NMOSEK_NUMBER_OF_NM_CONFIG_NODES !=   2) && \
       (NMOSEK_NUMBER_OF_NM_CONFIG_NODES !=   4) && (NMOSEK_NUMBER_OF_NM_CONFIG_NODES !=   8) && \
       (NMOSEK_NUMBER_OF_NM_CONFIG_NODES !=  16) && (NMOSEK_NUMBER_OF_NM_CONFIG_NODES !=  32) && \
       (NMOSEK_NUMBER_OF_NM_CONFIG_NODES !=  64) && (NMOSEK_NUMBER_OF_NM_CONFIG_NODES != 128) && \
       (NMOSEK_NUMBER_OF_NM_CONFIG_NODES != 256)
#    error "NmOsek configuration issue: NMOSEK_NUMBER_OF_NM_CONFIG_NODES must be defined to a power of two less than or equal to 256."
#   endif
#  endif
# endif
#endif

/* Check configuration of Single Channel optimization */
#if (NMOSEK_USE_INIT_POINTER == STD_OFF)
# if (NMOSEK_API_OPTIMIZATION == STD_ON) && (NMOSEK_MAX_NUMBER_OF_NMOSEK_CHANNELS > 1u)
#  error "NmOsek configuration issue: Single Channel optimization is only in single channel configurations possible."
# endif
#endif

#if (NMOSEK_NM_PASSIVE_MODE_ENABLED == STD_OFF)
# if (NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE == STD_ON)
#  if (NMOSEK_TX_DEADLINE_MONITORING_IN_COM_ENABLED == STD_ON)
#   error "NmOsek configuration issue: NMOSEK_TX_DEADLINE_MONITORING_IN_COM_ENABLED must be defined to STD_OFF if NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE is defined to STD_ON."
#  endif
# endif
#else
# if (NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE == STD_ON)
#  error "NmOsek configuration issue: NMOSEK_NM_PASSIVE_MODE_ENABLED must be defined to STD_OFF if NMOSEK_FIRST_MESSAGE_SHALL_BE_NM_MESSAGE is defined to STD_ON."
# endif
# if (NMOSEK_TX_DEADLINE_MONITORING_IN_COM_ENABLED == STD_ON)
#  error "NmOsek configuration issue: NMOSEK_NM_PASSIVE_MODE_ENABLED must be defined to STD_OFF if NMOSEK_TX_DEADLINE_MONITORING_IN_COM_ENABLED is defined to STD_ON."
# endif
#endif

#if (NMOSEK_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
# if (NMOSEK_GENERATORCOMPATIBILITYVERSION == STD_OFF)
#  error "NmOsek configuration issue: NMOSEK_GENERATORCOMPATIBILITYVERSION must be defined to STD_ON if NMOSEK_USE_ECUM_BSW_ERROR_HOOK == STD_ON."
# endif
# if (NMOSEK_FINALMAGICNUMBER == STD_OFF)
#  error "NmOsek configuration issue: NMOSEK_FINALMAGICNUMBER must be defined to STD_ON if NMOSEK_USE_ECUM_BSW_ERROR_HOOK == STD_ON."
# endif
#else
# if (NMOSEK_GENERATORCOMPATIBILITYVERSION == STD_ON)
#  error "NmOsek configuration issue: NMOSEK_USE_ECUM_BSW_ERROR_HOOK must be defined to STD_ON if NMOSEK_GENERATORCOMPATIBILITYVERSION == STD_ON."
# endif
# if (NMOSEK_FINALMAGICNUMBER == STD_ON)
#  error "NmOsek configuration issue: NMOSEK_USE_ECUM_BSW_ERROR_HOOK must be defined to STD_ON if NMOSEK_FINALMAGICNUMBER == STD_ON."
# endif
#endif

#if (NMOSEK_WAKEUPTXUSERDATAPDUIDOFPBCHANNELCONFIG != NMOSEK_AWAKETXUSERDATAPDUIDOFPBCHANNELCONFIG)
# error "NmOsek configuration issue: NMOSEK_WAKEUPTXUSERDATAPDUIDOFPBCHANNELCONFIG must have the same value as NMOSEK_AWAKETXUSERDATAPDUIDOFPBCHANNELCONFIG."
#endif

#if (NMOSEK_TX_DEADLINE_MONITORING_IN_COM_ENABLED == STD_ON)
# if (NMOSEK_WAKEUPTXUSERDATACOMIPDUIDOFPBCHANNELCONFIG != STD_ON) 
#  error "NmOsek configuration issue: NMOSEK_WAKEUPTXUSERDATACOMIPDUIDOFPBCHANNELCONFIG must be defined to STD_ON if NMOSEK_TX_DEADLINE_MONITORING_IN_COM_ENABLED is defined to STD_ON."
# endif
#endif

/**********************************************************************************************************************
 *  ORGANI CHECK
 *********************************************************************************************************************/

/* Justification for module-specific MISRA deviations:
   MD_NmOsek_0779: rule 5.1
      Reason:     In dependency of configuration items, generated identifiers may not be unique within the first 31
                  characters.
      Risk:       A compiler might not correctly pre-process the software what might result in incorrect code.
      Prevention: Each delivery is integrated and tested on the real target system. In addition, preprocessors and
                  compilers used in current MICROSAR projects are not expected to suffer from this (historic)
                  limitation.
                  Compiler selection and validation is done by ECU supplier. 
   MD_NmOsek_3673: rule 16.7
      Reason:     CanSM defines interface with pointer to variable so function prototype cannot be changed
      Risk:       There is no risk.
      Prevention: Covered by code review.
   MD_NmOsek_3684: rule 8.12
      Reason:     Size defined by application.
      Risk:       Invalid memory accesses.
      Prevention: Covered by code review.
*/

/**********************************************************************************************************************
 *  END OF FILE: NmOsek.c
 *********************************************************************************************************************/
