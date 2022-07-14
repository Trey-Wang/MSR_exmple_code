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
/**        \file  Eth_GeneralTypes.h
 *        \brief  General types header for the Ethernet stack
 *
 *      \details  Holds general data types and defines provided to and used by multiple components of the Vector
 *                Ethernet stack.
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Harald Walter                 vishaw         Vector Informatik GmbH
 *  Patrick Sommer                vissop         Vector Informatik GmbH
 *  Mark Harsch                   vismha         Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2013-07-18  vishaw  -             Initial creation
 *  01.00.01  2014-03-04  vishaw  ESCAN00074052 Ethernet DET error defines for ETH_STATE_MODE_DOWN and
 *                                              ETH_STATE_MODE_ACTIVE missing
 *  01.01.00  2016-03-23  vissop  ESCAN00089066 Add Eth_PhysAddrType
 *  01.02.00  2016-10-20  vismha  FEATC-327     FEAT-1998: Support of HW Time Stamping for Switch in Eth
 *  01.03.00  2017-02-20  vismha  FEATC-679     FEAT-2151: Extended Ethernet Bus Diagnostic
 *  01.04.00  2017-03-20  visfer  ESCAN00094496 Add ETH_TIMESTAMP_(IN)VALID to Eth_GeneralTypes
 *  02.00.00  2017-05-10  vismha  ESCAN00095096 Introduced time stamping related data types
 **********************************************************************************************************************/
#ifndef ETH_GENERAL_TYPES_H
#define ETH_GENERAL_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "ComStack_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ETH modes */
#define ETH_MODE_DOWN                        (0x00U)
#define ETH_MODE_ACTIVE                      (0x01U)

/* ETH return type */
#define ETH_OK                               (0x00U)
#define ETH_E_NOT_OK                         (0x01U)
#define ETH_E_NO_ACCESS                      (0x02U)

/* ETH RX status type */
#define ETH_RECEIVED                         (0x00U)
#define ETH_NOT_RECEIVED                     (0x01U)
#define ETH_RECEIVED_MORE_DATA_AVAILABLE     (0x02U)
#define ETH_RECEIVED_FRAMES_LOST             (0x03U)

/* ETH filter action types */
#define ETH_ADD_TO_FILTER                    (0x00U)
#define ETH_REMOVE_FROM_FILTER               (0x01U)

/* ETH states */
#define ETH_STATE_UNINIT                     (0x00U)
#define ETH_STATE_INIT                       (0x01U)
#define ETH_STATE_ACTIVE                     (0x02U)
#define ETH_STATE_MODE_DOWN                  (0x03U)
#define ETH_STATE_MODE_ACTIVE                (0x04U)

/* ETH MII modes */
#define ETH_MII_MODE                         (0x00U)
#define ETH_RMII_MODE                        (0x01U)
#define ETH_GMII_MODE                        (0x02U)
#define ETH_RGMII_MODE                       (0x03U)
#define ETH_SGMII_MODE                       (0x04U)

#define ETH_INVALID_FRAME_ID                 (0x00U)

#define ETH_PHYS_ADDR_LEN_BYTE                  (6U)
#define ETH_ETHER_TYPE_LEN_BYTE                 (2U)
#define ETH_HEADER_LEN_BYTE                    (14U)

/* ETH Timestamp Quality Types */
#define ETH_TIMESTAMP_VALID                     (0U)
#define ETH_TIMESTAMP_INVALID                   (1U)
#define ETH_TIMESTAMP_UNCERTAIN                 (2U)

/*! Value defining that the counter isn't supported */
# define ETH_RXTX_STATS_INV_COUNTER_VAL       (0xFFFFFFFFu)
/*! Value defining that the counter has overflown */
# define ETH_RXTX_STATS_COUNTER_OVERFLOW_VAL  (0xFFFFFFFEu)
/*! Value defining the maximum possible counter value */
# define ETH_RXTX_STATS_MAX_COUNTER_VAL       (0xFFFFFFFDu)

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8          Eth_ReturnType;
typedef uint8          Eth_ModeType;
typedef uint16         Eth_FrameType;
typedef uint32         Eth_DataType;
typedef uint8          Eth_RxStatusType;
typedef uint8          Eth_FilterActionType;
typedef uint16         Eth_FrameIdType;

typedef uint8          Eth_StateType;

/* ETH Physical Address Type */
typedef uint8 Eth_PhysAddrType[ETH_PHYS_ADDR_LEN_BYTE];

/*! \brief Type defining a time stamp according to AUTOSAR 4.2 */
typedef struct
{
  uint32 nanoseconds;
  uint32 seconds;
  uint16 secondsHi;
} Eth_TimeStampType;

/*! \brief Type defining the quality of a time stamp */
typedef uint8    Eth_TimestampQualityType;
/*! \brief Type defining the difference between time stamps */
typedef sint32   Eth_TimediffType;
/*! \brief Type defining the drift of a clock related to another clock */
typedef float64  Eth_RateRatioType;

/*! \brief Structure holding transmission statistic counters related to the Eth controller. */
typedef struct
{
  uint32 TxNumberOfOctets;
  uint32 TxNUcastPkts;
  uint32 TxUniCastPkts;
} Eth_TxStatsType;

/*! brief Structure holding reception statistic counters related to the Eth controller. */
typedef struct
{
  uint32 RxStatsDropEvents;
  uint32 RxStatsOctets;
  uint32 RxStatsPkts;
  uint32 RxStatsBroadcastPkts;
  uint32 RxStatsMulticastPkts;
  uint32 RxStatsCrcAlignErrors;
  uint32 RxStatsUndersizePkts;
  uint32 RxStatsOversizePkts;
  uint32 RxStatsFragments;
  uint32 RxStatsJabbers;
  uint32 RxStatsCollisions;
  uint32 RxStatsPkts64Octets;
  uint32 RxStatsPkts65to127Octets;
  uint32 RxStatsPkts128to255Octets;
  uint32 RxStatsPkts256to511Octets;
  uint32 RxStatsPkts512to1023Octets;
  uint32 RxStatsPkts1024to1518Octets;
  uint32 RxUnicastFrames;
} Eth_RxStatsType;

#endif /* ETH_GENERAL_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: Eth_GeneralTypes.h
 *********************************************************************************************************************/
