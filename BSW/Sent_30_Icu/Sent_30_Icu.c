/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Sent_30_Icu.c
 *        \brief  Source file of the Vector Sent_30_Icu Handler
 *
 *      \details  The Sent_30_Icu handler is a software component which is meant to enable usage of SENT sensors on
 *                Vector ECUs. The SWC assists the application layer / sensor driver by providing the raw data which
 *                is received from the sensor.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to Sent_30_Icu.h
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

/* PRQA S 0857 EOF */ /* MD_MSR_1.1 */

#define SENT_30_ICU_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Sent_30_Icu.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of module header file */
#if (  (SENT_30_ICU_SW_MAJOR_VERSION != (1u)) \
    || (SENT_30_ICU_SW_MINOR_VERSION != (0u)) \
    || (SENT_30_ICU_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of Sent_30_Icu.c and Sent_30_Icu.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/*! \brief defines the number of additional falling edges (crc, sync etc.) without data nibbles */
#define SENT_30_ICU_AMOUNT_OF_EXTRA_EDGES         (3u)
/*! \brief defines the number of falling edges in a SENT message */
#define SENT_30_ICU_AMOUNT_OF_EDGES_PER_MSG       (SENT_30_ICU_AMOUNT_OF_DATA_NIBBLES+SENT_30_ICU_AMOUNT_OF_EXTRA_EDGES)
/*! \brief defines the number of data pulses without calibration pulse */
#define SENT_30_ICU_AMOUNT_OF_DATA_PULSES_PER_MSG (SENT_30_ICU_AMOUNT_OF_DATA_NIBBLES+2u)
/*! \brief define the minimum amount of ticks in a nibble (=0x0) */
#define SENT_30_ICU_MIN_TICKS_PER_NIBBLE          (12u)
/*! \brief define the maximum amount of ticks in a nibble (=0xF) */
#define SENT_30_ICU_MAX_TICKS_PER_NIBBLE          (27u)
/*! \brief define for an invalid nibble */
#define SENT_30_ICU_INVALID_NIBBLE                (0xFFu)
/*! \brief define an upscaling factor for floating point operations */
#define SENT_30_ICU_UPSCALING_FACTOR              (1000u)
/*! \brief defines amount of SENT ticks in a calibration pulse */
#define SENT_30_ICU_CALIBRATION_PULSE_TICKS       (56u)
/*! \brief defines time in us of a calibration pulse */
#define SENT_30_ICU_CALIBRATION_PULSE_US          (SENT_30_ICU_CALIBRATION_PULSE_TICKS*SENT_30_ICU_TICK_TIME)
/*! \brief defines the SENT specification CRC4 seed */
#define SENT_30_ICU_CRC4_SEED                     (0x05u)
/*! \brief defines the SENT specification CRC6 seed */
#define SENT_30_ICU_CRC6_SEED                     (0x15u)
/*! \brief defines the size of the crc4 lookup table */
#define SENT_30_ICU_CRC4TABLE_SIZE                (16u)
/*! \brief defines the size of the crc6 lookup table */
#define SENT_30_ICU_CRC6TABLE_SIZE                (64u)

#if (SENT_30_ICU_SHORT_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT)
/*! \brief defines the amount of fast channel messages required for a full slow message */
#define SENT_30_ICU_SLOW_CHANNEL_FAST_MSGS        (16u)
/*! \brief defines the amount of nibbles in a slow message */
#define SENT_30_ICU_SLOW_CHANNEL_NIBBLES          (4u)
#endif
#if (SENT_30_ICU_ENHANCED_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT)
/*! \brief defines the amount of fast channel messages required for a full slow message */
#define SENT_30_ICU_SLOW_CHANNEL_FAST_MSGS        (18u)
/*! \brief defines the amount of nibbles in a slow message */
#define SENT_30_ICU_SLOW_CHANNEL_NIBBLES          (5u)
/*! \brief defines the amount of 1-bits in the enhanced slow message */
#define SENT_30_ICU_SLOW_CHANNEL_ONE_BITS         (6u)
#endif

/*! \brief defines the mask for extracting the start bit for slow messages */
#define SENT_30_ICU_SLOW_CHANNEL_STARTBIT_MASK    (0x8u)
/*! \brief defines the shift for extracting the start bit for slow messages */
#define SENT_30_ICU_SLOW_CHANNEL_STARTBIT_SHIFT   (0x3u)
/*! \brief defines the mask for extracting the data bit for slow messages */
#define SENT_30_ICU_SLOW_CHANNEL_DATABIT_MASK     (0x4u)
/*! \brief defines the shift for extracting the data bit for slow messages */
#define SENT_30_ICU_SLOW_CHANNEL_DATABIT_SHIFT    (0x2u)

/*! \brief defines the states for receiving slow messages */
#define SENT_30_ICU_SLOW_STATE_WAITING            (0x0u)
#define SENT_30_ICU_SLOW_STATE_RECEIVING          (0x1u)
#define SENT_30_ICU_SLOW_STATE_PATTERN_ONE        (0x2u)
#define SENT_30_ICU_SLOW_STATE_PATTERN_ZERO       (0x3u)

#if (STD_OFF == SENT_30_ICU_USE_MAINFUNCTION)
/*! \brief defines the size of the Icu channel buffer - in notification mode 1 is sufficient */
#define SENT_30_ICU_ICUBUFFER_SIZE                (1u)
#else
/*! \brief defines the size of the Icu channel buffer - this has to be a multiple of 8 */
#define SENT_30_ICU_ICUBUFFER_SIZE                (256u)
/*! \brief defines the size of the Icu channel buffer */
#define SENT_30_ICU_ICUBUFFER_SIZEMASK            (SENT_30_ICU_ICUBUFFER_SIZE-1u)
#endif

#if (STD_ON == SENT_30_ICU_ENABLE_DIAGNOSIS)
#define SENT_30_ICU_MAXERROR_COUNT                (0xFFu)
#define SENT_30_ICU_MAXERROR_COUNT_U32            (0xFFFFFFFFu)
#define SENT_30_ICU_MAXERROR_COUNT_U16            (0xFFFFu)
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/* Preprocessor define STATIC is no longer available in Compiler.h in MSR4 - provide local define */
#if !defined (STATIC)
# define STATIC static
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

#define SENT_30_ICU_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! \brief Initialization state of the module */
STATIC VAR(uint8, SENT_30_ICU_VAR) Sent_30_Icu_ModuleInitialized = SENT_30_ICU_M_UNINIT; /* PRQA S 3229 */ /* MD_SENT_3229 */

/*! \brief Local variable which holds the current SENT state */
STATIC VAR(uint8, SENT_30_ICU_VAR) Sent_30_Icu_State = SENT_30_ICU_UNINIT;

#define SENT_30_ICU_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SENT_30_ICU_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! \brief Local variable which holds the nibble index */
STATIC VAR(uint8, SENT_30_ICU_VAR) Sent_30_Icu_NibbleCount; /* PRQA S 3218 */ /* MD_SENT_3218 */
/*! \brief Local variable which holds the last nibbles from partial messages */
STATIC VAR(uint8, SENT_30_ICU_VAR) Sent_30_Icu_Nibble[SENT_30_ICU_AMOUNT_OF_DATA_PULSES_PER_MSG];

#if ( (SENT_30_ICU_SHORT_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) || \
      (SENT_30_ICU_ENHANCED_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) )
/*! \brief Local variable which holds the slow message counter */
STATIC VAR(uint8, SENT_30_ICU_VAR) Sent_30_Icu_SlowChannelCounter;
/*! \brief Local variable which holds the slow channel state */
STATIC VAR(uint8, SENT_30_ICU_VAR) Sent_30_Icu_SlowChannelState;
#endif

#define SENT_30_ICU_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SENT_30_ICU_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (STD_OFF == SENT_30_ICU_USE_MAINFUNCTION)
/*! \brief Local memory to hold icu timestamps */
STATIC VAR(uint16, SENT_30_ICU_VAR) Sent_30_Icu_LastTimestamp;
#else
/*! \brief Local memory to hold icu timestamps */
STATIC VAR(Icu_IndexType, SENT_30_ICU_VAR) Sent_30_Icu_TimestampIndex; /* PRQA S 3218 */ /* MD_SENT_3218 */
#endif

STATIC VAR(uint16, SENT_30_ICU_VAR) Sent_30_Icu_Calib_Thresh_L;
STATIC VAR(uint16, SENT_30_ICU_VAR) Sent_30_Icu_Calib_Thresh_H;
STATIC VAR(uint16, SENT_30_ICU_VAR) Sent_30_Icu_LastCalibPulse; /* PRQA S 3218 */ /* MD_SENT_3218 */

#define SENT_30_ICU_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SENT_30_ICU_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! \brief Local variable to hold the current calibration pulse correction divisor */
STATIC VAR(uint32, SENT_30_ICU_VAR) Sent_30_Icu_Corrector; /* PRQA S 3218 */ /* MD_SENT_3218 */
/*! \brief Global variable which holds the actual raw sensor data without metadata */
STATIC VAR(Sent_30_Icu_FastDataType, SENT_30_ICU_VAR) Sent_30_Icu_FastChannelData;

#define SENT_30_ICU_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SENT_30_ICU_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! \brief Local memory to hold Icu timestamps */
STATIC VAR(Icu_ValueType, SENT_30_ICU_VAR) Sent_30_Icu_IcuBuffer[SENT_30_ICU_ICUBUFFER_SIZE];

#if ( (SENT_30_ICU_SHORT_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) || \
      (SENT_30_ICU_ENHANCED_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) )
/*! \brief Local variable to hold the slow channel nibbles */
STATIC VAR(uint8, SENT_30_ICU_VAR) Sent_30_Icu_SlowChannelNibble[SENT_30_ICU_SLOW_CHANNEL_FAST_MSGS];
/*! \brief Global variable which holds the slow channel data */
STATIC VAR(Sent_30_Icu_SlowDataType, SENT_30_ICU_VAR) Sent_30_Icu_SlowChannelData;
#endif

#if (STD_ON == SENT_30_ICU_ENABLE_DIAGNOSIS)
/*! \brief Local memory to hold diagnostic data */
STATIC VAR(Sent_30_Icu_DiagDataType, SENT_30_ICU_VAR) Sent_30_Icu_DiagData;
#endif

#define SENT_30_ICU_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define SENT_30_ICU_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* MISRA-C:2004 Rule 8.7 -> All config arrays are static to increase the readability of the code */
/*! \brief Local lookup table for the CRC-4 calculations */
STATIC CONST(uint8, SENT_30_ICU_CONST) Sent_30_Icu_Crc4Table[SENT_30_ICU_CRC4TABLE_SIZE] = { /* PRQA S 3218 */ /* MD_SENT_3218 */
                                                                                            0u,13u,7u,10u,
                                                                                            14u,3u,9u,4u,
                                                                                            1u,12u,6u,11u,
                                                                                            15u,2u,8u,5u
                                                                                            };
#if (SENT_30_ICU_ENHANCED_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT)
/* MISRA-C:2004 Rule 8.7 -> All config arrays are static to increase the readability of the code */
/*! \brief Local lookup table for the CRC-4 calculations */
STATIC CONST(uint8, SENT_30_ICU_CONST) Sent_30_Icu_Crc6Table[SENT_30_ICU_CRC6TABLE_SIZE] =  { /* PRQA S 3218 */ /* MD_SENT_3218 */
                                                                                            0, 25, 50, 43, 61, 36, 15, 22, 35, 58, 17,  8, 30,  7, 44, 53,
                                                                                           31,  6, 45, 52, 34, 59, 16,  9, 60, 37, 14, 23,  1, 24, 51, 42,
                                                                                           62, 39, 12, 21,  3, 26, 49, 40, 29,  4, 47, 54, 32, 57, 18, 11,
                                                                                           33, 56, 19, 10, 28,  5, 46, 55,  2, 27, 48, 41, 63, 38, 13, 20
                                                                                            };
#endif

#define SENT_30_ICU_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#define SENT_30_ICU_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC FUNC(uint8, SENT_30_ICU_CODE) Sent_30_Icu_TicksToNibble(uint8 Tick);
STATIC FUNC(Std_ReturnType, SENT_30_ICU_CODE) Sent_30_Icu_CheckCRC4
    (
        P2CONST(uint8, AUTOMATIC, SENT_30_ICU_CODE) NibblePtr,
        uint8 Length
    );
STATIC FUNC(Std_ReturnType, SENT_30_ICU_CODE) Sent_30_Icu_ProcessPulse(uint16 PulseLength);
#if ( (SENT_30_ICU_SHORT_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) || (SENT_30_ICU_ENHANCED_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) )
STATIC FUNC(Std_ReturnType, SENT_30_ICU_CODE) Sent_30_Icu_ProcessSlowChannel(uint8 StatusNibble);
#endif
#if (SENT_30_ICU_ENHANCED_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT)
STATIC FUNC(Std_ReturnType, SENT_30_ICU_CODE) Sent_30_Icu_CheckCRC6(uint32 DataSB, uint32 DataPL);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

#if (SENT_30_ICU_SHORT_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT)
/***********************************************************************************************************************
 *  Sent_30_Icu_ProcessSlowChannel
 **********************************************************************************************************************/
/*! \brief      The handler function for processing a single short fastchannel message for the slow communication.
 *  \details    Once a full fastchannel message is received, the statusnibble gets extracted for the serial/slow
 *              communication.
 *  \param[in]  StatusNibble                 The status nibble of the fastchannel
 *  \return     E_NOT_OK - No slow channel message yet
 *  \return     E_OK - Slow channel message complete
 *  \pre        Complete fastchannel message received
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, SENT_30_ICU_CODE) Sent_30_Icu_ProcessSlowChannel(uint8 StatusNibble)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal;
  uint8 StartBit;

  uint8 NibPL;
  uint8 NibSB;
  uint8 NibblesPL[SENT_30_ICU_SLOW_CHANNEL_NIBBLES];
  uint8 NibblesSB[SENT_30_ICU_SLOW_CHANNEL_NIBBLES];
  uint8 index;
  uint8 NibCtr;
  uint8 BitCtr;

  /* ----- Implementation ----------------------------------------------- */
  RetVal = E_NOT_OK;

  switch (Sent_30_Icu_SlowChannelState)
  {
  case SENT_30_ICU_SLOW_STATE_WAITING:
    StartBit = (StatusNibble & SENT_30_ICU_SLOW_CHANNEL_STARTBIT_MASK) >> SENT_30_ICU_SLOW_CHANNEL_STARTBIT_SHIFT;
    if (1u == StartBit)
    {
      /* Slow message starting */
      Sent_30_Icu_SlowChannelState = SENT_30_ICU_SLOW_STATE_RECEIVING;
      Sent_30_Icu_SlowChannelNibble[0] = StatusNibble;
      Sent_30_Icu_SlowChannelCounter = 1u;
    }
    break;

  case SENT_30_ICU_SLOW_STATE_RECEIVING:
    StartBit = (StatusNibble & SENT_30_ICU_SLOW_CHANNEL_STARTBIT_MASK) >> SENT_30_ICU_SLOW_CHANNEL_STARTBIT_SHIFT;

    if (0u == StartBit)
    {
      Sent_30_Icu_SlowChannelNibble[Sent_30_Icu_SlowChannelCounter] = StatusNibble;
      Sent_30_Icu_SlowChannelCounter++;

      if (SENT_30_ICU_SLOW_CHANNEL_FAST_MSGS == Sent_30_Icu_SlowChannelCounter)
      {
        /* Full message received */
        NibPL = 0;
        NibSB = 0;
        NibCtr = 0;
        BitCtr = 0;
        for (index=0; index<SENT_30_ICU_SLOW_CHANNEL_FAST_MSGS; index++)
        {
          NibPL = NibPL << 1u;
          NibSB = NibPL << 1u;
          NibPL |= (Sent_30_Icu_SlowChannelNibble[index] & SENT_30_ICU_SLOW_CHANNEL_DATABIT_MASK) >> SENT_30_ICU_SLOW_CHANNEL_DATABIT_SHIFT;
          NibSB |= (Sent_30_Icu_SlowChannelNibble[index] & SENT_30_ICU_SLOW_CHANNEL_STARTBIT_MASK) >> SENT_30_ICU_SLOW_CHANNEL_STARTBIT_SHIFT;
          BitCtr++;
          if (4u == BitCtr)
          {
            /* Nibble complete */
            NibblesPL[NibCtr] = NibPL;
            NibblesSB[NibCtr] = NibSB;
            NibPL = 0;
            NibSB = 0;
            BitCtr = 0;
            NibCtr++;
          }
        }

        RetVal = Sent_30_Icu_CheckCRC4(&(NibblesPL[0]), SENT_30_ICU_SLOW_CHANNEL_NIBBLES-1u);

        if (E_OK == RetVal)
        {
          /* CRC is ok, write to global */
        Sent_30_Icu_SlowChannelData.MsgId = NibblesPL[0];
        Sent_30_Icu_SlowChannelData.Data = (NibblesPL[1] << 4u) | NibblesPL[2];
        }
        /* Wait for the next message start */
        Sent_30_Icu_SlowChannelState = SENT_30_ICU_SLOW_STATE_WAITING;
      }
    } else
    {
      /* Invalid startbit for SSM format */
      Sent_30_Icu_SlowChannelState = SENT_30_ICU_SLOW_STATE_WAITING;
    }

    break;

  default:
    /* this state should never be reached */
    break;
  }

  return RetVal;

} /* Sent_30_Icu_ProcessSlowChannel */
#endif

#if (SENT_30_ICU_ENHANCED_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT)
/***********************************************************************************************************************
 *  Sent_30_Icu_ProcessSlowChannel
 **********************************************************************************************************************/
/*! \brief      The handler function for processing a single enhanced fastchannel message for the slow communication.
 *  \details    Once a full fastchannel message is received, the statusnibble gets extracted for the serial/slow
 *              communication.
 *  \param[in]  StatusNibble                 The status nibble of the fastchannel
 *  \return     E_NOT_OK - No slow channel message yet
 *  \return     E_OK - Slow channel message complete
 *  \pre        Complete fastchannel message received
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, SENT_30_ICU_CODE) Sent_30_Icu_ProcessSlowChannel(uint8 StatusNibble)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal;
  uint8 StartBit;
  uint8 ConfigBit;

  uint32 DataPL;
  uint32 DataSB;
  uint8 index;

  /* ----- Implementation ----------------------------------------------- */
  RetVal = E_NOT_OK;

  switch (Sent_30_Icu_SlowChannelState)
  {
  case SENT_30_ICU_SLOW_STATE_WAITING:
    StartBit = (StatusNibble & SENT_30_ICU_SLOW_CHANNEL_STARTBIT_MASK) >> SENT_30_ICU_SLOW_CHANNEL_STARTBIT_SHIFT;
    if (0u == StartBit)
    {
      /* potential Slow message starting */
      Sent_30_Icu_SlowChannelState = SENT_30_ICU_SLOW_STATE_PATTERN_ONE;
    }
    break;

  case SENT_30_ICU_SLOW_STATE_PATTERN_ONE:
    StartBit = (StatusNibble & SENT_30_ICU_SLOW_CHANNEL_STARTBIT_MASK) >> SENT_30_ICU_SLOW_CHANNEL_STARTBIT_SHIFT;
    if (1u == StartBit)
    {
      /* Correct enhanced serial message pattern bit */
      Sent_30_Icu_SlowChannelNibble[Sent_30_Icu_SlowChannelCounter] = StatusNibble;
      Sent_30_Icu_SlowChannelCounter++;

      if (SENT_30_ICU_SLOW_CHANNEL_ONE_BITS == Sent_30_Icu_SlowChannelCounter)
      {
        /* The 1-bits are complete, check for the following 0-bit */
        Sent_30_Icu_SlowChannelState = SENT_30_ICU_SLOW_STATE_PATTERN_ZERO;
      }
    } else
    {
      /* This pattern didn't match - potential begin of the next message */
      Sent_30_Icu_SlowChannelState = SENT_30_ICU_SLOW_STATE_PATTERN_ONE;
      Sent_30_Icu_SlowChannelCounter = 0;
    }
    break;

  case SENT_30_ICU_SLOW_STATE_PATTERN_ZERO:
    StartBit = (StatusNibble & SENT_30_ICU_SLOW_CHANNEL_STARTBIT_MASK) >> SENT_30_ICU_SLOW_CHANNEL_STARTBIT_SHIFT;
    if (0u == StartBit)
    {
      /* Complete enhanced serial message pattern is correct */
      Sent_30_Icu_SlowChannelState = SENT_30_ICU_SLOW_STATE_RECEIVING;
      Sent_30_Icu_SlowChannelNibble[Sent_30_Icu_SlowChannelCounter] = StatusNibble;
      Sent_30_Icu_SlowChannelCounter++;
    }
    break;

  case SENT_30_ICU_SLOW_STATE_RECEIVING:
    StartBit = (StatusNibble & SENT_30_ICU_SLOW_CHANNEL_STARTBIT_MASK) >> SENT_30_ICU_SLOW_CHANNEL_STARTBIT_SHIFT;
    Sent_30_Icu_SlowChannelNibble[Sent_30_Icu_SlowChannelCounter] = StatusNibble;
    Sent_30_Icu_SlowChannelCounter++;

    if ((SENT_30_ICU_SLOW_CHANNEL_FAST_MSGS == Sent_30_Icu_SlowChannelCounter) &&
        (0u == StartBit))
    {
      /* Full message received */
      DataSB = 0;
      DataPL = 0;
      for (index=0; index<SENT_30_ICU_SLOW_CHANNEL_FAST_MSGS; index++)
      {
        DataPL = DataPL << 1u;
        DataSB = DataSB << 1u;

        DataPL |= (uint32)((Sent_30_Icu_SlowChannelNibble[index] & SENT_30_ICU_SLOW_CHANNEL_DATABIT_MASK) >> SENT_30_ICU_SLOW_CHANNEL_DATABIT_SHIFT);
        DataSB |= (uint32)((Sent_30_Icu_SlowChannelNibble[index] & SENT_30_ICU_SLOW_CHANNEL_STARTBIT_MASK) >> SENT_30_ICU_SLOW_CHANNEL_STARTBIT_SHIFT);
      }

      RetVal = Sent_30_Icu_CheckCRC6(DataSB, DataPL);

      if (E_OK == RetVal)
      {
        /* CRC is ok */

        /* Check configuration bit for payload / message id layout */
        ConfigBit = (uint8)((DataSB & 0x00000400u) >> 10u);
        if (0u == ConfigBit)
        {
          /* C = 0 layout: 12-Bit Data + 8-Bit MsgId */
          Sent_30_Icu_SlowChannelData.MsgId = (uint8)( ((DataSB & 0x000003C0u) >> 2u) | ((DataSB & 0x0000001Eu) >> 1u) );
          Sent_30_Icu_SlowChannelData.Data = (uint16)( (DataPL & 0x00000FFFu) );
        } else
        {
          /* C = 1 layout 16-Bit Data + 4-Bit MsgId */
          Sent_30_Icu_SlowChannelData.MsgId = (uint8)( (DataSB & 0x000003C0u) >> 6u );
          Sent_30_Icu_SlowChannelData.Data = (uint16)( (DataPL & 0x00000FFFu) | ((DataSB & 0x0000001Eu) << 11u));
        }

      }
      /* Wait for the next message start */
      Sent_30_Icu_SlowChannelState = SENT_30_ICU_SLOW_STATE_PATTERN_ONE;
    }
    break;

  default:
    /* this state should never be reached */
    break;
  }

  return RetVal;

} /* Sent_30_Icu_ProcessSlowChannel */


/***********************************************************************************************************************
 *  Sent_30_Icu_CheckCRC6
 **********************************************************************************************************************/
/*! \brief      This function checks the CRC-6 checksum of enhanced serial messages.
 *  \details    The function sorts the two input data dwords according the SENT spec. Afterwards the checksum is
 *              calculated with a lookup table.
 *  \param[in]  DataSB                 Bit 2 of every statusnibble
 *  \param[in]  DataPL                 Bit 3 of every statusnibble
 *  \return     E_NOT_OK - CRC doesnt match
 *  \return     E_OK - CRC matches
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, SENT_30_ICU_CODE) Sent_30_Icu_CheckCRC6(uint32 DataSB, uint32 DataPL)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal;
  uint8 Checksum;
  uint8 CRC;
  uint8 ExNibble;
  uint8 index;

  /* ----- Development Error Checks ------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */

  Checksum = SENT_30_ICU_CRC6_SEED;
  CRC = (uint8)((DataPL & 0x0003F000) >> 12u);

  /* Extract bits for the correct CRC checking order (to 6-bit containers) and XOR */
  for (index=0; index<4; index++)
  {
    ExNibble = (uint8)((DataPL & 0x00000800u) >> 11u);
    ExNibble |= (uint8)((DataSB & 0x00000800u) >> 10u);

    ExNibble |= (uint8)((DataPL & 0x00000400u) >> 8u);
    ExNibble |= (uint8)((DataSB & 0x00000400u) >> 7u);

    ExNibble |= (uint8)((DataPL & 0x00000200u) >> 5u);
    ExNibble |= (uint8)((DataSB & 0x00000200u) >> 4u);

    Checksum = ExNibble ^ Sent_30_Icu_Crc6Table[Checksum];

    DataPL = DataPL << 3u;
    DataSB = DataSB << 3u;
  }

  Checksum = 0 ^ Sent_30_Icu_Crc6Table[Checksum];

  if (Checksum == CRC)
  {
    RetVal = E_OK;
  } else
  {
    RetVal = E_NOT_OK;
  }

  return RetVal;

} /* Sent_30_Icu_CheckCRC6 */
#endif

/***********************************************************************************************************************
 *  Sent_30_Icu_ProcessPulse
 **********************************************************************************************************************/
/*! \brief      This function processes single pulse lengths to fastchannel messages.
 *  \details    The statemachine acts depending on the current global state - at the beginning it checks if
 *              any pulse measured is within the tolerance so it could be a calibration pulse. It then stores the
 *              corrector to normalize the following pulses to this pulse and changes the state to "receiving"
 *              In the receiving state the pulses are normalized and finally converted to their respective SENT
 *              data nibble.
 *  \param[in]  PulseLength                 The delta time between two ICU timestamps
 *  \return     E_NOT_OK - No complete fastchannel message received
 *  \return     E_OK - Complete fastchannel message received
 *  \note       Implements REQ7, REQ8, REQ9, REQ10, REQ11
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, SENT_30_ICU_CODE) Sent_30_Icu_ProcessPulse(uint16 PulseLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal;
  uint32 NormalizedPulse;

  /* ----- Implementation ----------------------------------------------- */
  /* No message received yet */
  RetVal = E_NOT_OK;

  switch (Sent_30_Icu_State)
  {
    case (SENT_30_ICU_CALIBRATING):
      /* No calibration pulse found yet */
      if ((PulseLength > Sent_30_Icu_Calib_Thresh_L) &&
          (PulseLength < Sent_30_Icu_Calib_Thresh_H))
      {

        /* Check if a frame is complete */
        if (SENT_30_ICU_AMOUNT_OF_DATA_PULSES_PER_MSG == Sent_30_Icu_NibbleCount)
        {
          /* Full message received */
          /* Check successive calibration pulse tolerance
           * they may differ no more than +-1.5625% (1/64) */
          if (((PulseLength - Sent_30_Icu_LastCalibPulse) <= (Sent_30_Icu_LastCalibPulse>>6u)) ||
              ((Sent_30_Icu_LastCalibPulse - PulseLength) <= (PulseLength>>6u)))
          {
            /* Successive calibration pulse within tolerance */
            /* Store the correction factor for further pulses (add half divisor for rounding)
             * all pulses will be normalized to this pulse */
            Sent_30_Icu_Corrector = (uint32)((((uint32)(PulseLength) * (uint32)(SENT_30_ICU_UPSCALING_FACTOR)) + \
                    (SENT_30_ICU_CALIBRATION_PULSE_TICKS>>1u)) / (uint32)(SENT_30_ICU_CALIBRATION_PULSE_TICKS));

            /* Update state machine for future pulses */
            Sent_30_Icu_State = SENT_30_ICU_CALIBRATION_FOUND;
            Sent_30_Icu_NibbleCount = 0;
            Sent_30_Icu_LastCalibPulse = PulseLength;

            /* If CRC is ok, return that a full message has been received */
            RetVal = Sent_30_Icu_CheckCRC4(&(Sent_30_Icu_Nibble[1]), SENT_30_ICU_AMOUNT_OF_DATA_NIBBLES);
#if ( (SENT_30_ICU_SHORT_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) || (SENT_30_ICU_ENHANCED_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) )
            if (E_NOT_OK == RetVal)
            {
              /* Invalid CRC */
              Sent_30_Icu_SlowChannelState = SENT_30_ICU_SLOW_STATE_WAITING;
            }
#endif
#if (STD_ON == SENT_30_ICU_ENABLE_DIAGNOSIS)
            if ((E_OK != RetVal) &&
                (Sent_30_Icu_DiagData.InvalidCrcFrames < SENT_30_ICU_MAXERROR_COUNT))
            {
                Sent_30_Icu_DiagData.InvalidCrcFrames++;
            }
            Sent_30_Icu_DiagData.ValidFastFramesSuccession = 0u;
#endif
          } else
          {
            /* If successive calibration pulses are not within tolerance, reset the slow channel */
#if ( (SENT_30_ICU_SHORT_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) || (SENT_30_ICU_ENHANCED_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) )
            Sent_30_Icu_SlowChannelState = SENT_30_ICU_SLOW_STATE_WAITING;
#endif

#if (STD_ON == SENT_30_ICU_ENABLE_DIAGNOSIS)
            if (Sent_30_Icu_DiagData.InvalidCalibPulses < SENT_30_ICU_MAXERROR_COUNT)
            {
                Sent_30_Icu_DiagData.InvalidCalibPulses++;
            }
            Sent_30_Icu_DiagData.ValidFastFramesSuccession = 0u;
#endif
          }

        } else
        {
          /* Calibration pulse with no successor */

          /* Store the correction factor for further pulses (add half divisor for rounding)
           * all pulses will be normalized to this pulse */
          Sent_30_Icu_Corrector = (uint32)((((uint32)(PulseLength) * (uint32)(SENT_30_ICU_UPSCALING_FACTOR)) + \
                  (SENT_30_ICU_CALIBRATION_PULSE_TICKS>>1u)) / (uint32)(SENT_30_ICU_CALIBRATION_PULSE_TICKS));

          Sent_30_Icu_State = SENT_30_ICU_CALIBRATION_FOUND;
          Sent_30_Icu_NibbleCount = 0;
          Sent_30_Icu_LastCalibPulse = PulseLength;
        }
      }
#if (STD_ON == SENT_30_ICU_ENABLE_DIAGNOSIS)
      else
      {
          /* Calibration pulse exceeds max/min */
          if (Sent_30_Icu_DiagData.InvalidCalibPulses < SENT_30_ICU_MAXERROR_COUNT)
          {
              Sent_30_Icu_DiagData.InvalidCalibPulses++;
          }
          Sent_30_Icu_DiagData.ValidFastFramesSuccession = 0u;
      }
#endif
    break;

    case (SENT_30_ICU_CALIBRATION_FOUND):
      /* Check if last 'calibration pulse' was actually a 'pause pulse' (or invalid nibble count) */
      if ((PulseLength > Sent_30_Icu_Calib_Thresh_L) &&
          (PulseLength < Sent_30_Icu_Calib_Thresh_H))
      {
        Sent_30_Icu_Corrector = (uint32)((((uint32)(PulseLength) * (uint32)(SENT_30_ICU_UPSCALING_FACTOR)) + \
                (SENT_30_ICU_CALIBRATION_PULSE_TICKS>>1u)) / (uint32)(SENT_30_ICU_CALIBRATION_PULSE_TICKS));
        Sent_30_Icu_State = SENT_30_ICU_CALIBRATION_FOUND;
        Sent_30_Icu_NibbleCount = 0;
        Sent_30_Icu_LastCalibPulse = PulseLength;
#if (STD_ON == SENT_30_ICU_ENABLE_DIAGNOSIS)
        /* If a calibration pulse is found and the amount of data pulses is less than configured
         * then the amount of SENT pulses received is less than configured in the driver */
        if (Sent_30_Icu_NibbleCount < SENT_30_ICU_AMOUNT_OF_DATA_PULSES_PER_MSG)
        {
            if (Sent_30_Icu_DiagData.InvalidNibCounts < SENT_30_ICU_MAXERROR_COUNT)
            {
                Sent_30_Icu_DiagData.InvalidNibCounts++;
            }
        }
#endif
      } else
      {

        /* Pulse after calibration pulse, apply the message correction factor (add half divisor for rounding) */
        NormalizedPulse = (uint32)((((uint32)(PulseLength) * (uint32)(SENT_30_ICU_UPSCALING_FACTOR)) + (Sent_30_Icu_Corrector>>1u)) / Sent_30_Icu_Corrector);

        /* Convert the SENT ticks to an actual data value */
        Sent_30_Icu_Nibble[Sent_30_Icu_NibbleCount] = Sent_30_Icu_TicksToNibble((uint8)NormalizedPulse);

        if (Sent_30_Icu_Nibble[Sent_30_Icu_NibbleCount] == SENT_30_ICU_INVALID_NIBBLE)
        {
          /* Invalid nibble detected, break this message */
          Sent_30_Icu_State = SENT_30_ICU_CALIBRATING;
#if ( (SENT_30_ICU_SHORT_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) || (SENT_30_ICU_ENHANCED_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) )
          Sent_30_Icu_SlowChannelState = SENT_30_ICU_SLOW_STATE_WAITING;
#endif
#if (STD_ON == SENT_30_ICU_ENABLE_DIAGNOSIS)
          if (Sent_30_Icu_DiagData.InvalidNibbles < SENT_30_ICU_MAXERROR_COUNT)
          {
              Sent_30_Icu_DiagData.InvalidNibbles++;
          }
          Sent_30_Icu_DiagData.ValidFastFramesSuccession = 0u;
#endif
        } else
        {
          Sent_30_Icu_NibbleCount++;
          if (SENT_30_ICU_AMOUNT_OF_DATA_PULSES_PER_MSG == Sent_30_Icu_NibbleCount)
          {
#if (STD_ON == SENT_30_ICU_PAUSE_PULSE)
            /* If the frame is complete, skip the pause pulse */
            Sent_30_Icu_State = SENT_30_ICU_SKIP_PAUSE_PULSE;
#else
            /* If the frame is complete, look for the next calibration pulse */
            Sent_30_Icu_State = SENT_30_ICU_CALIBRATING;
#endif
          }
#if (STD_ON == SENT_30_ICU_ENABLE_DIAGNOSIS)
          else if (Sent_30_Icu_NibbleCount > SENT_30_ICU_AMOUNT_OF_DATA_PULSES_PER_MSG)
          {
              if (Sent_30_Icu_DiagData.InvalidNibCounts < SENT_30_ICU_MAXERROR_COUNT)
              {
                  Sent_30_Icu_DiagData.InvalidNibCounts++;
              }
          }
#endif

        }

      }

    break;
#if (STD_ON == SENT_30_ICU_PAUSE_PULSE)
    case (SENT_30_ICU_SKIP_PAUSE_PULSE):
          /* Pause pulse occured, check for the next calibration pulse */
          Sent_30_Icu_State = SENT_30_ICU_CALIBRATING;
    break;

#endif

    default:
      /* Invalid state, this shouldn't be reached */
    break;

  }

  return RetVal;

} /* Sent_30_Icu_ProcessPulse */

/***********************************************************************************************************************
 *  Sent_30_Icu_CheckCRC4
 **********************************************************************************************************************/
/*! \brief      This function checks the CRC4 of messages according to the SENT spec.
 *  \details    Implemented with a lookup table as recommended in the SENT JAN2010 spec
 *  \param[in]  NibblePtr                 Pointer to the Nibble array
 *  \param[in]  Length                    Amount of nibbles to check
 *  \return     E_NOT_OK - CRC doesnt match
 *  \return     E_OK - CRC matches
 **********************************************************************************************************************/
STATIC FUNC(Std_ReturnType, SENT_30_ICU_CODE) Sent_30_Icu_CheckCRC4
    (
        P2CONST(uint8, AUTOMATIC, SENT_30_ICU_VAR) NibblePtr,
        uint8 Length
    )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal;
  uint8 Checksum;
  uint8 loop;

  /* ----- Implementation ----------------------------------------------- */

  Checksum = SENT_30_ICU_CRC4_SEED;

  for (loop=0; loop<Length; loop++)
  {
    Checksum = NibblePtr[loop] ^ Sent_30_Icu_Crc4Table[Checksum];
  }

  Checksum = 0 ^ Sent_30_Icu_Crc4Table[Checksum];

  if (Checksum == NibblePtr[Length])
  {
    RetVal = E_OK;
  } else
  {
    RetVal = E_NOT_OK;
  }

  return RetVal;

} /* Sent_30_Icu_CheckCRC */


/***********************************************************************************************************************
 *  Sent_30_Icu_TickToNibble()
 **********************************************************************************************************************/
/*! \brief      This function converts a number of SENT ticks to a data value
 *  \details    First the range of the ticks is checked, then the minimum ticks (12) is subtracted from the tick
 *              amount to get the data value.
 *  \param[in]  Channel                 Input parameter
 *  \return     Data nibble value
 **********************************************************************************************************************/
STATIC FUNC(uint8, SENT_30_ICU_CODE) Sent_30_Icu_TicksToNibble(uint8 Tick)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 RetVal;

  /* ----- Implementation ----------------------------------------------- */
  if ((Tick < SENT_30_ICU_MIN_TICKS_PER_NIBBLE) || (Tick > SENT_30_ICU_MAX_TICKS_PER_NIBBLE))
  {
    /* Invalid tick amount */
    RetVal = SENT_30_ICU_INVALID_NIBBLE;
  } else
  {
    /* Ticks are within range, subtract the minimum nibble tick time */
    RetVal = Tick - SENT_30_ICU_MIN_TICKS_PER_NIBBLE;
  }

  return RetVal;

} /* Sent_30_Icu_TickToNibble */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#if (STD_OFF == SENT_30_ICU_USE_MAINFUNCTION)
/***********************************************************************************************************************
 *  Sent_30_Icu_IcuChannelCallback
 **********************************************************************************************************************/
/*! \brief      The handler ICU callback function
 *  \details    The callback function has to be called upon every detected falling edge by the ICU driver.
 *              A delta is made from the last ICU timestamp and the current timestamp and the pulselength is fed into
 *              the 'ProcessPulse' (fastchannel) statemachine.
 *              Upon a complete fastchannel message, the slowchannel processing is started.
 *  \pre        Sent_30_Icu_Init
 *  \note       Implements R_SENT_5, R_SENT_12
 **********************************************************************************************************************/
FUNC(void,  SENT_30_ICU_CODE) Sent_30_Icu_IcuChannelCallback(void) /* PRQA S 1503 */ /* MD_SENT_1503 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal;
  uint8 loop;
  uint16 Delta;

  /* ----- Development Error Checks ------------------------------------- */
  /* Check initialization state of the component */
  Sent_30_Icu_CheckDetErrorReturnVoid(Sent_30_Icu_ModuleInitialized == (uint8)SENT_30_ICU_M_INIT,
      SENT_30_ICU_SID_ICUCALLBACK, SENT_30_ICU_E_NOT_INITIALIZED); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* ----- Implementation ----------------------------------------------- */
  if (SENT_30_ICU_UNINIT != Sent_30_Icu_State)
  {
    Delta = Sent_30_Icu_IcuBuffer[0] - Sent_30_Icu_LastTimestamp;
    Sent_30_Icu_LastTimestamp = Sent_30_Icu_IcuBuffer[0];

    RetVal = Sent_30_Icu_ProcessPulse(Delta);

    if (E_OK == RetVal)
    {
      /* Message received */
      /* Copy fast channel data to global variable */
      Sent_30_Icu_FastChannelData.FastData = 0x00000000u;
      for (loop=SENT_30_ICU_AMOUNT_OF_DATA_NIBBLES; loop>0; loop--)
      {
        Sent_30_Icu_FastChannelData.FastData = Sent_30_Icu_FastChannelData.FastData << 4u;
        Sent_30_Icu_FastChannelData.FastData |= Sent_30_Icu_Nibble[loop];
      }
      Sent_30_Icu_FastChannelData.StatusNibble = Sent_30_Icu_Nibble[0];

#if (SENT_30_ICU_USE_NOTIFICATION == STD_ON)
      SENT_30_ICU_NOTIFICATION_FUNC_FASTCH();
#endif

#if (STD_ON == SENT_30_ICU_ENABLE_DIAGNOSIS)
            if (Sent_30_Icu_DiagData.ValidFastFramesReceived < SENT_30_ICU_MAXERROR_COUNT_U32)
            {
                Sent_30_Icu_DiagData.ValidFastFramesReceived++;
            }
            if (Sent_30_Icu_DiagData.ValidFastFramesSuccession < SENT_30_ICU_MAXERROR_COUNT_U16)
            {
                Sent_30_Icu_DiagData.ValidFastFramesSuccession++;
            }
#endif

#if ( (SENT_30_ICU_SHORT_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) || (SENT_30_ICU_ENHANCED_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) )
        /* Process slow (serial) channel data */
        RetVal = Sent_30_Icu_ProcessSlowChannel(Sent_30_Icu_Nibble[0]);
#if (SENT_30_ICU_USE_NOTIFICATION == STD_ON)
        if (E_OK == RetVal)
        {
            SENT_30_ICU_NOTIFICATION_FUNC_SLOWCH();
        }
#endif
#endif
    }

  }

  return;

} /* Sent_30_Icu_IcuChannelCallback */


#else
/***********************************************************************************************************************
 *  Sent_30_Icu_MainFunction
 **********************************************************************************************************************/
/*! \brief      Periodic mainfunction which is called by a periodic task
 *  \details    The mainfunction gets the timestamp index of the ringbuffer from last call and
 *              works through all indexes until the current timestamp is reached. For each timestamp
 *              a delta is made to form a pulselength and is fed to the ProcessPulse (fastchannel) statemachine.
 *              Upon a complete fastchannel message, the slowchannel processing is started.
 *  \pre        Sent_30_Icu_Init
 *  \note       Implements R_SENT_5, R_SENT_12
 **********************************************************************************************************************/
FUNC(void, SENT_30_ICU_CODE) Sent_30_Icu_MainFunction(void) /* PRQA S 1503 */ /* MD_SENT_1503 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Icu_IndexType index;
  Std_ReturnType RetVal;
  uint16 Delta;
  uint8 loop;

  /* ----- Development Error Checks ------------------------------------- */
  /* Check initialization state of the component */
  Sent_30_Icu_CheckDetErrorReturnVoid(Sent_30_Icu_ModuleInitialized == (uint8)SENT_30_ICU_M_INIT,
    SENT_30_ICU_SID_MAINFUNCTION, SENT_30_ICU_E_NOT_INITIALIZED); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* ----- Implementation ----------------------------------------------- */
  if (SENT_30_ICU_UNINIT != Sent_30_Icu_State)
  {

    /* Set index to the last ICU timestamp counter */
    index = Sent_30_Icu_TimestampIndex;

    /* Get the current ICU timestamp index - and use a sizemask for rollover handling */
    Sent_30_Icu_TimestampIndex = Icu_GetTimestampIndex(SENT_30_ICU_CHANNEL) & SENT_30_ICU_ICUBUFFER_SIZEMASK;

    /* Iterate over data which hasn't been processed */
    while (index != Sent_30_Icu_TimestampIndex)
    {
      /* Create delta pulselength with two edges */
      Delta = (Sent_30_Icu_IcuBuffer[index] - Sent_30_Icu_IcuBuffer[(index-1u) & SENT_30_ICU_ICUBUFFER_SIZEMASK]);

      RetVal = Sent_30_Icu_ProcessPulse(Delta);

      if (E_OK == RetVal)
      {
        /* Message received */
        /* Copy fast channel data to global variable */
        Sent_30_Icu_FastChannelData.FastData = 0x00000000u;
        for (loop=SENT_30_ICU_AMOUNT_OF_DATA_NIBBLES; loop>0; loop--)
        {
          Sent_30_Icu_FastChannelData.FastData = Sent_30_Icu_FastChannelData.FastData << 4u;
          Sent_30_Icu_FastChannelData.FastData |= Sent_30_Icu_Nibble[loop];
        }
        Sent_30_Icu_FastChannelData.StatusNibble = Sent_30_Icu_Nibble[0];

#if (SENT_30_ICU_USE_NOTIFICATION == STD_ON)
      SENT_30_ICU_NOTIFICATION_FUNC_FASTCH();
#endif

#if (STD_ON == SENT_30_ICU_ENABLE_DIAGNOSIS)
            if (Sent_30_Icu_DiagData.ValidFastFramesReceived < SENT_30_ICU_MAXERROR_COUNT_U32)
            {
                Sent_30_Icu_DiagData.ValidFastFramesReceived++;
            }
            if (Sent_30_Icu_DiagData.ValidFastFramesSuccession < SENT_30_ICU_MAXERROR_COUNT_U16)
            {
                Sent_30_Icu_DiagData.ValidFastFramesSuccession++;
            }
#endif

#if ( (SENT_30_ICU_SHORT_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) || (SENT_30_ICU_ENHANCED_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) )
        /* Process slow (serial) channel data */
        RetVal = Sent_30_Icu_ProcessSlowChannel(Sent_30_Icu_Nibble[0]);

#if (SENT_30_ICU_USE_NOTIFICATION == STD_ON)
        if (E_OK == RetVal)
        {
            SENT_30_ICU_NOTIFICATION_FUNC_SLOWCH();
        }
#endif
#endif

      }

      index = (index+1u) & SENT_30_ICU_ICUBUFFER_SIZEMASK;
    }
  }

  return;

} /* Sent_30_Icu_MainFunction */
#endif


/***********************************************************************************************************************
 *  Sent_30_Icu_InitMemory()
 **********************************************************************************************************************/
/*! \brief      Function for *_INIT_*-variable initialization
 *  \details    Service to initialize module global variables at power up. This function can be used to initialize the
 *              variables in *_INIT_* sections in case they are not initialized by the startup code.
 *  \pre        Module must not be initialized
 *  \pre        Function shall be called from task level
 **********************************************************************************************************************/
FUNC(void, SENT_30_ICU_CODE) Sent_30_Icu_InitMemory(void) /* PRQA S 1503 */ /* MD_SENT_1503 */
{
  /* ----- Implementation ----------------------------------------------- */
  Sent_30_Icu_ModuleInitialized = SENT_30_ICU_UNINIT;
  Sent_30_Icu_State = SENT_30_ICU_UNINIT;
}  /* Sent_30_Icu_InitMemory() */

/***********************************************************************************************************************
 *  Sent_30_Icu_Init
 **********************************************************************************************************************/
/*! \brief      Initialization function
 *  \details    This function initializes the module Sent_30_Icu. It calculates thresholds to save time in future
 *              function calls and configures the ICU for timestamping. It then sets the module status to initialized.
 *  \pre        Interrupts have to be disabled.
 *  \pre        The module has to be uninitialized.
 **********************************************************************************************************************/
FUNC(void, SENT_30_ICU_CODE) Sent_30_Icu_Init(void) /* PRQA S 1503 */ /* MD_SENT_1503 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 calibPulse_uc;
  uint16 index;

  /* ----- Development Error Checks ------------------------------------- */
  /* Check initialization state of the component */
  Sent_30_Icu_CheckDetErrorReturnVoid(Sent_30_Icu_ModuleInitialized == (uint8)SENT_30_ICU_M_UNINIT,
      SENT_30_ICU_SID_INIT, SENT_30_ICU_E_ALREADY_INITIALIZED); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  /* Check slow channel configuration */
  Sent_30_Icu_CheckDetErrorReturnVoid(SENT_30_ICU_SERIAL_FORMAT <= (uint8)SENT_30_ICU_ENHANCED_SERIAL_MESSAGE,
      SENT_30_ICU_SID_INIT, SENT_30_ICU_E_INVALID_CFG_SLOW_CH_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  /* Check tick time */
  Sent_30_Icu_CheckDetErrorReturnVoid(SENT_30_ICU_TIMER_CLOCK_UC_TICKS > (uint8)0,
      SENT_30_ICU_SID_INIT, SENT_30_ICU_E_INVALID_CFG_CLOCK_TICKS); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  /* Check tick time */
  Sent_30_Icu_CheckDetErrorReturnVoid(SENT_30_ICU_TICK_TIME > (uint8)0,
      SENT_30_ICU_SID_INIT, SENT_30_ICU_E_INVALID_CFG_TICK_TIME); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  /* Check data nibbles */
  Sent_30_Icu_CheckDetErrorReturnVoid(SENT_30_ICU_AMOUNT_OF_DATA_NIBBLES > (uint8)0,
      SENT_30_ICU_SID_INIT, SENT_30_ICU_E_INVALID_CFG_DATA_NIBBLES); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* ----- Implementation ----------------------------------------------- */
  /* Calculations to save time in the callback */
  calibPulse_uc = SENT_30_ICU_CALIBRATION_PULSE_US*SENT_30_ICU_TIMER_CLOCK_UC_TICKS;

  Sent_30_Icu_Calib_Thresh_H = (uint16)(((uint32)(calibPulse_uc) * (uint32)(SENT_30_ICU_CALIB_THRESH_PERCENT+100u)) / 100u);
  Sent_30_Icu_Calib_Thresh_L = calibPulse_uc - (Sent_30_Icu_Calib_Thresh_H - calibPulse_uc);

#if (STD_ON == SENT_30_ICU_ENABLE_DIAGNOSIS)
  Sent_30_Icu_DiagData.CorrectionFactor = 0u;
  Sent_30_Icu_DiagData.InvalidCalibPulses = 0u;
  Sent_30_Icu_DiagData.ValidFastFramesReceived = 0u;
  Sent_30_Icu_DiagData.ValidFastFramesSuccession = 0u;
  Sent_30_Icu_DiagData.InvalidCrcFrames = 0u;
  Sent_30_Icu_DiagData.InvalidNibCounts = 0u;
  Sent_30_Icu_DiagData.InvalidNibbles = 0u;
#endif

#if ( (SENT_30_ICU_SHORT_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) || (SENT_30_ICU_ENHANCED_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) )
  Sent_30_Icu_SlowChannelState = SENT_30_ICU_SLOW_STATE_WAITING;
  Sent_30_Icu_SlowChannelCounter = 0u;
  Sent_30_Icu_SlowChannelData.Data = 0u;
  Sent_30_Icu_SlowChannelData.MsgId = 0u;
#endif

  Sent_30_Icu_FastChannelData.FastData = 0u;
  Sent_30_Icu_FastChannelData.StatusNibble = 0u;
  Sent_30_Icu_Corrector = 0u;
  Sent_30_Icu_NibbleCount = 0u;

  for (index=0; index < SENT_30_ICU_AMOUNT_OF_DATA_PULSES_PER_MSG; index++)
  {
      Sent_30_Icu_Nibble[index] = 0u;
  }

  for (index=0; index < SENT_30_ICU_ICUBUFFER_SIZE; index++)
  {
      Sent_30_Icu_IcuBuffer[index] = 0u;
  }

  Sent_30_Icu_State = (uint8)SENT_30_ICU_CALIBRATING;
  Sent_30_Icu_ModuleInitialized = (uint8)SENT_30_ICU_M_INIT;

#if (STD_OFF == SENT_30_ICU_USE_MAINFUNCTION)
  Sent_30_Icu_LastTimestamp = 0u;

  Icu_EnableNotification(SENT_30_ICU_CHANNEL);
  /* tell the ICU to start timestamp measurements */
  Icu_StartTimestamp(SENT_30_ICU_CHANNEL,                                    /* channel identifier                 */
                     Sent_30_Icu_IcuBuffer,                                  /* pointer to the exchange ringbuffer */
                     1u,                                                     /* size of the ringbuffer             */
                     1u);                                                    /* notification interval              */
#else
  Sent_30_Icu_TimestampIndex = 0u;
  /* tell the ICU to start timestamp measurements */
  Icu_StartTimestamp(SENT_30_ICU_CHANNEL,                                    /* channel identifier                 */
                     Sent_30_Icu_IcuBuffer,                                  /* pointer to the exchange ringbuffer */
                     SENT_30_ICU_ICUBUFFER_SIZE,                             /* size of the ringbuffer             */
                     0);                                                     /* notification interval              */
#endif
}  /* Sent_30_Icu_Init */

/***********************************************************************************************************************
 *  Sent_30_Icu_DeInit
 **********************************************************************************************************************/
/*! \brief      The Sent handler deinitialization function
 *  \details    Timestamping and processing of timestamps stops after calling this function.
 **********************************************************************************************************************/
FUNC(void, SENT_30_ICU_CODE) Sent_30_Icu_DeInit(void) /* PRQA S 1503 */ /* MD_SENT_1503 */
{
  /* ----- Development Error Checks ------------------------------------- */
  /* Check initialization state of the component */
  Sent_30_Icu_CheckDetErrorReturnVoid(Sent_30_Icu_ModuleInitialized == (uint8)SENT_30_ICU_M_INIT,
      SENT_30_ICU_SID_DEINIT, SENT_30_ICU_E_NOT_INITIALIZED); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* ----- Implementation ----------------------------------------------- */
  /* tell the ICU to stop timestamp measurements */
  Icu_StopTimestamp(SENT_30_ICU_CHANNEL);
#if (STD_OFF == SENT_30_ICU_USE_MAINFUNCTION)
  Icu_DisableNotification(SENT_30_ICU_CHANNEL);
#endif

  Sent_30_Icu_State = (uint8)SENT_30_ICU_UNINIT;
  Sent_30_Icu_ModuleInitialized = (uint8)SENT_30_ICU_M_UNINIT;

  return;

} /* Sent_30_Icu_DeInit */

#if (SENT_30_ICU_VERSION_INFO_API == STD_ON)
/***********************************************************************************************************************
 *  Sent_30_Icu_GetVersionInfo()
 **********************************************************************************************************************/
/*! \brief      Returns the version information
 *  \details    Sent_30_Icu_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the
 *              component.
 *  \param[out] versioninfo             Pointer to where to store the version information
 *  \pre        Input parameter must not be NULL.
 *  \pre        Function shall be called from task level
 **********************************************************************************************************************/
FUNC(void, SENT_30_ICU_CODE) Sent_30_Icu_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, SENT_30_ICU_APPL_VAR) versioninfo) /* PRQA S 1503 */ /* MD_SENT_1503 */
{
  /* ----- Development Error Checks ------------------------------------- */
  Sent_30_Icu_CheckDetErrorReturnVoid(NULL_PTR != versioninfo,
      SENT_30_ICU_SID_GET_VERSION_INFO, SENT_30_ICU_E_NULL_POINTER); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* ----- Implementation ----------------------------------------------- */
  versioninfo->vendorID = (SENT_30_ICU_VENDOR_ID);
  versioninfo->moduleID = (SENT_30_ICU_MODULE_ID);
#if (0x03u == SENT_30_ICU_AR_RELEASE_MAJOR_VERSION)
  versioninfo->instanceID = (SENT_30_ICU_INSTANCE_ID);
#endif

  versioninfo->sw_major_version = (SENT_30_ICU_SW_MAJOR_VERSION);
  versioninfo->sw_minor_version = (SENT_30_ICU_SW_MINOR_VERSION);
  versioninfo->sw_patch_version = (SENT_30_ICU_SW_PATCH_VERSION);

} /* Sent_30_Icu_GetVersionInfo() */  /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

/***********************************************************************************************************************
 *  Sent_30_Icu_GetFastChannelData()
 **********************************************************************************************************************/
/*! \brief      Writes fast channel data to location
 *  \details    Sent_30_Icu_GetFastChannelData writes the last successfully received fast channel data of the
 *              SENT channel to the pointer location.
 *  \param[out] DataPtr             Pointer to where to store the data
 *  \pre        Input parameter must not be NULL.
 *  \pre        Function shall be called from application level
 **********************************************************************************************************************/
FUNC(void, SENT_30_ICU_CODE) Sent_30_Icu_GetFastChannelData /* PRQA S 1503 */ /* MD_SENT_1503 */
(
        P2VAR(Sent_30_Icu_FastDataType, AUTOMATIC, SENT_30_ICU_APPL_DATA) DataPtr
)
{
  /* ----- Development Error Checks ------------------------------------- */
  Sent_30_Icu_CheckDetErrorReturnVoid(NULL_PTR != DataPtr,
        SENT_30_ICU_SID_GET_FASTCHANNEL_DATA, SENT_30_ICU_E_NULL_POINTER); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* ----- Implementation ----------------------------------------------- */
  *DataPtr = Sent_30_Icu_FastChannelData;

} /* Sent_30_Icu_GetFastChannelData() */  /* PRQA S 2006 */ /* MD_MSR_14.7 */


#if ( (SENT_30_ICU_SHORT_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) || \
(SENT_30_ICU_ENHANCED_SERIAL_MESSAGE == SENT_30_ICU_SERIAL_FORMAT) )
/***********************************************************************************************************************
 *  Sent_30_Icu_GetSlowChannelData()
 **********************************************************************************************************************/
/*! \brief      Writes slow channel data to location
 *  \details    Sent_30_Icu_GetSlowChannelData writes the last successfully received slow serial channel data of the
 *              SENT channel to the pointer location.
 *  \param[out] DataPtr             Pointer to where to store the data
 *  \pre        Input parameter must not be NULL.
 *  \pre        Function shall be called from application level
 **********************************************************************************************************************/
FUNC(void, SENT_30_ICU_CODE) Sent_30_Icu_GetSlowChannelData
(
        P2VAR(Sent_30_Icu_SlowDataType, AUTOMATIC, SENT_30_ICU_APPL_DATA) DataPtr
)
{
  /* ----- Development Error Checks ------------------------------------- */
  Sent_30_Icu_CheckDetErrorReturnVoid(NULL_PTR != DataPtr,
        SENT_30_ICU_SID_GET_SLOWCHANNEL_DATA, SENT_30_ICU_E_NULL_POINTER); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* ----- Implementation ----------------------------------------------- */
  *DataPtr = Sent_30_Icu_SlowChannelData;

} /* Sent_30_Icu_GetSlowChannelData() */  /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

#if (STD_ON == SENT_30_ICU_ENABLE_DIAGNOSIS)
/***********************************************************************************************************************
 *  Sent_30_Icu_GetDiagnosticValues()
 **********************************************************************************************************************/
/*! \brief      Queries current Sent diagnostic data
 *  \details    The function writes the current collected diagnostic data values to the
 *              given location.
 *  \param[out] DataPtr             Pointer data storage location
 *  \pre        Input parameter must not be NULL.
 *  \pre        Function shall be called from application level
 *  \note       Implements R_SENT_13-28
 **********************************************************************************************************************/
FUNC(void, SENT_30_ICU_CODE) Sent_30_Icu_GetDiagnosticValues
(
        P2VAR(Sent_30_Icu_DiagDataType, AUTOMATIC, SENT_30_ICU_APPL_DATA) DataPtr
)
{
    Sent_30_Icu_DiagData.CorrectionFactor = Sent_30_Icu_Corrector;
    *DataPtr = Sent_30_Icu_DiagData;
} /* Sent_30_Icu_GetDiagnosticValues() */

/***********************************************************************************************************************
 *  Sent_30_Icu_ResetDiagnosticValues()
 **********************************************************************************************************************/
/*! \brief      Resets the current SENT diagnostic data
 *  \details    The function resets the diangostic counter values to 0.
 *  \pre        Function shall be called from application level
 *  \note       Implements R_SENT_13-28
 **********************************************************************************************************************/
FUNC(void, SENT_30_ICU_CODE) Sent_30_Icu_ResetDiagnosticValues(void)
{
    Sent_30_Icu_DiagData.CorrectionFactor = 0u;
    Sent_30_Icu_DiagData.InvalidCalibPulses = 0u;
    Sent_30_Icu_DiagData.ValidFastFramesReceived = 0u;
    Sent_30_Icu_DiagData.ValidFastFramesSuccession = 0u;
    Sent_30_Icu_DiagData.InvalidCrcFrames = 0u;
    Sent_30_Icu_DiagData.InvalidNibCounts = 0u;
    Sent_30_Icu_DiagData.InvalidNibbles = 0u;
} /* Sent_30_Icu_ResetDiagnosticValues() */
#endif

#define SENT_30_ICU_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:
 MD_SENT_883:
      Reason:     The include file code is protected against repeated inclusion by the
                  SENT_30_ICU_H define.
      Risk:       -
      Prevention: -
*/

/* Justification for module-specific MISRA deviations:
 MD_SENT_1503:
      Reason:     Usage of the APIs is configuration and application dependent.
      Risk:       -
      Prevention: -
*/

/* Justification for module-specific MISRA deviations:
 MD_SENT_3218:
      Reason:     Static scope variable used in only one function: This variable changes over
                  multiple function calls or is defined here to reduce RAM usage.
      Risk:       -
      Prevention: Program flow has been verified by component tests.
*/

/* Justification for module-specific MISRA deviations:
 MD_SENT_3229:
      Reason:     The ModuleInitialized variable will only be used when DET is enabled
      Risk:       -
      Prevention: Program flow has been verified by component tests.
*/

/**********************************************************************************************************************
 *  END OF FILE: Sent_30_Icu.c
 *********************************************************************************************************************/
