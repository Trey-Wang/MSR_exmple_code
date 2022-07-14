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
/**        \file  Eth_30_Vtt_LL.C
 *        \brief  Source file of lower layer part of the VTT Ethernet controller driver
 *
 *      \details  Provides implementation needed to adapte to the platform specific behavior.
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
#define ETH_30_VTT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Eth_30_Vtt.h"
#include "Eth_30_Vtt_Types.h"
#include "Eth_30_Vtt_LL_Types.h"
#include "Eth_30_Vtt_LL.h"
#include "Eth_30_Vtt_Int.h"
#include "Eth_30_Vtt_LL_Int.h"
#include "EthIf_Cbk.h"
#if (ETH_30_VTT_ENABLE_MAC_WRITE_ACCESS == STD_ON )
# include "Nvm.h"
#endif /* ETH_30_VTT_ENABLE_MAC_WRITE_ACCESS */
#include "IpBase.h"
#include "Os.h"
#include "CANoeApi.h"
#include "VttCntrl_Eth.h"
#include "IpBase.h"
#if (ETH_30_VTT_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif /* ETH_30_TRICORE_DEV_ERROR_DETECT */
/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check consistency of source and header file. */
#if ( (ETH_30_VTT_SW_MAJOR_VERSION != 6U) || \
      (ETH_30_VTT_SW_MINOR_VERSION != 0U) || \
      (ETH_30_VTT_SW_PATCH_VERSION != 1U) )
# error "Eth_30_Vtt_LL.c: Source and Header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ETH Multicast Defines */
#define ETH_30_VTT_MULTICAST_MASK                          0x01U
#define ETH_30_VTT_MULTICAST_BUCKET_COUNTERS                 64U
#define ETH_30_VTT_MULTICAST_BUCKET_COUNTERS_MOD_2           32U
#define ETH_30_VTT_MULTICAST_HASH_31_27_MASK         0xF8000000U
#define ETH_30_VTT_MULTICAST_HASH_31_27_SHIFT                27U
#define ETH_30_VTT_BUCKET_UPPER_CNT_MASK                   0xF0U
#define ETH_30_VTT_BUCKET_LOWER_CNT_MASK                   0x0FU
#define ETH_30_VTT_BUCKET_UPPER_CNT_SHIFT                     4U
#define ETH_30_VTT_BUCKET_LOWER_CNT_SHIFT                     0U
#define ETH_30_VTT_BUCKET_MAX_CNT                            15U

/* ETH Filter modes */
#define ETH_30_VTT_FILTER_MULTICAST_MODE                      0U
#define ETH_30_VTT_FILTER_PROMISCUOUS_MODE                    1U

#ifndef  STATIC
# define STATIC static
#endif /* STATIC */

typedef P2CONST(uint8, AUTOMATIC, CRC_APPL_DATA) Crc_DataRefType;
/* CRC32 polynomial */
#define ETH_30_VTT_CRC_INITIAL_VALUE32         (0xFFFFFFFFUL)
#define ETH_30_VTT_CRC_POLYNOMIAL_32           (0x04C11DB7UL)
#define ETH_30_VTT_CRC_POLYNOMIAL_32_REFLECT   (0xEDB88320UL)
#define ETH_30_VTT_CRC_FINAL_XOR_CRC32         (0xFFFFFFFFUL)

#define ETH_30_VTT_INVALID_FRAME_ID                (0xFFFFUL)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define ETH_30_VTT_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 1 */ /* MD_MSR_19.1 */

#define ETH_30_VTT_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 1 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/

#define ETH_30_VTT_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 1 */ /* MD_MSR_19.1 */

STATIC VAR(uint8, ETH_30_VTT_VAR_NOINIT) Eth_30_Vtt_RxBufferRingCurProcPos[ETH_30_VTT_MAX_CTRLS_TOTAL];
STATIC VAR(uint8, ETH_30_VTT_VAR_NOINIT) Eth_30_Vtt_TxBufferRingCurProcPos[ETH_30_VTT_MAX_CTRLS_TOTAL];
#if (ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER == STD_ON)
STATIC VAR(uint8, ETH_30_VTT_VAR_NOINIT) Eth_30_Vtt_FilterMode[ETH_30_VTT_MAX_CTRLS_TOTAL];
#endif /* ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER */

#define ETH_30_VTT_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 1 */ /* MD_MSR_19.1 */

#define ETH_30_VTT_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 1 */ /* MD_MSR_19.1 */

STATIC VAR(uint16, ETH_30_VTT_VAR_NOINIT) Eth_30_Vtt_RxBufferRingCurFrameLen[ETH_30_VTT_MAX_CTRLS_TOTAL];
STATIC VAR(Eth_FrameIdType, ETH_30_VTT_VAR_NOINIT) Eth_30_Vtt_CurFrameId[ETH_30_VTT_MAX_CTRLS_TOTAL];
#define ETH_30_VTT_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 1 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define ETH_30_VTT_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 1 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  Eth_30_Vtt_VIncRxBufRingCurProcPtr
 **********************************************************************************************************************/
/*! \brief      
 **********************************************************************************************************************/
STATIC FUNC(void, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_VIncRxBufRingCurProcPtr(
  uint8 CtrlIdx,
  uint8 RingIdx);

/***********************************************************************************************************************
 *  Eth_30_Vtt_VIsBroadcast
 **********************************************************************************************************************/
/*! \brief      
 **********************************************************************************************************************/
STATIC FUNC(boolean, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_VIsBroadcast(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) PhysAddrPtr);

/***********************************************************************************************************************
 *  Eth_30_Vtt_VIsZeroAddress
 **********************************************************************************************************************/
/*! \brief      
 **********************************************************************************************************************/
STATIC FUNC(boolean, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_VIsZeroAddress(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) PhysAddrPtr);

#if (ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_VIsOpenMulticast
 **********************************************************************************************************************/
/*! \brief      
 **********************************************************************************************************************/
STATIC FUNC(boolean, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_VIsOpenMulticast(
          uint8                        CtrlIdx,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) PhysAddrPtr);
#endif /* ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER */

/***********************************************************************************************************************
 *  Eth_30_Vtt_VIsUnicast
 **********************************************************************************************************************/
/*! \brief      
 **********************************************************************************************************************/
STATIC FUNC(boolean, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_VIsUnicast(
          uint8                        CtrlIdx,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) PhysAddrPtr);

/***********************************************************************************************************************
 *  Eth_30_Vtt_VInitBuffersAndDescriptors
 **********************************************************************************************************************/
/*! \brief      
 **********************************************************************************************************************/
STATIC FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_VInitBuffersAndDescriptors(
  uint8 CtrlIdx);

/***********************************************************************************************************************
 *  Eth_30_Vtt_VCalculateCRC32
 **********************************************************************************************************************/
/*! \brief      
 **********************************************************************************************************************/
STATIC FUNC(uint32, CRC_CODE) Eth_30_Vtt_VCalculateCRC32(
  Crc_DataRefType CrcDataPtr,
  uint32          CrcLength,
  uint32          CrcStartValue32,
  boolean         CrcIsFirstCall);

  STATIC INLINE FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_SetFrameId(
        uint8                        CtrlIdx,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) PayloadPtr,
        uint16                       PayloadLen);
/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  Eth_30_Vtt_VIncRxBufRingCurProcPtr
 **********************************************************************************************************************/
/*! Internal comment removed.
 * 
 **********************************************************************************************************************/
STATIC FUNC(void, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_VIncRxBufRingCurProcPtr(
  uint8 CtrlIdx,
  uint8 RingIdx)
{
  ETH_30_VTT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (Eth_30_Vtt_RxBufferRingCurProcPos[CtrlIdx] < (Eth_30_Vtt_VCfgGetRxBuf4Ring(CtrlIdx, RingIdx).SegmentNumber - 1))
  {
    Eth_30_Vtt_RxBufferRingCurProcPos[CtrlIdx]++;
  }
  else
  {
    Eth_30_Vtt_RxBufferRingCurProcPos[CtrlIdx] = 0;
  }
  ETH_30_VTT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* Eth_30_Vtt_VIncRxBufRingCurProcPtr() */

/***********************************************************************************************************************
 *  Eth_30_Vtt_VIsBroadcast
 **********************************************************************************************************************/
/*! Internal comment removed.
 *   
 **********************************************************************************************************************/
STATIC FUNC(boolean, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_VIsBroadcast(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) PhysAddrPtr)
{
  boolean IsBroadcast = TRUE;
  uint8 Idx = 0;

  while (Idx < ETH_PHYS_ADDR_LEN_BYTE)
  {
    if (0xFF != PhysAddrPtr[Idx])
    {
      IsBroadcast = FALSE;
      break;
    }
    Idx++;
  }

  return IsBroadcast;
} /* Eth_30_Vtt_VIsBroadcast() */

/***********************************************************************************************************************
 *  Eth_30_Vtt_VIsZeroAddress
 **********************************************************************************************************************/
/*! Internal comment removed.
 *  
 **********************************************************************************************************************/
STATIC FUNC(boolean, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_VIsZeroAddress(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) PhysAddrPtr)
{
  boolean IsZeroAddress = TRUE;
  uint8 Idx = 0;

  while (Idx < ETH_PHYS_ADDR_LEN_BYTE)
  {
    if (0x00 != PhysAddrPtr[Idx])
    {
      IsZeroAddress = FALSE;
      break;
    }
    Idx++;
  }

  return IsZeroAddress;
} /* Eth_30_Vtt_VIsZeroAddress() */

STATIC INLINE FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_SetFrameId(
        uint8                        CtrlIdx,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) PayloadPtr,
        uint16                       PayloadLen)
{
  ETH_30_VTT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  Eth_30_Vtt_CurFrameId[CtrlIdx]++;

  if( Eth_30_Vtt_CurFrameId[CtrlIdx] == ETH_30_VTT_INVALID_FRAME_ID )
  {
    Eth_30_Vtt_CurFrameId[CtrlIdx]++;
  }

  (*(Eth_FrameIdType*)&PayloadPtr[PayloadLen]) = Eth_30_Vtt_CurFrameId[CtrlIdx];

  ETH_30_VTT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* Eth_IncFrameId() */

#if (ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_VIsOpenMulticast
 **********************************************************************************************************************/
/*! Internal comment removed.
 * 
 **********************************************************************************************************************/
STATIC FUNC(boolean, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_VIsOpenMulticast(
          uint8                        CtrlIdx,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) PhysAddrPtr)
{
  boolean RetVal = FALSE;

  /* Multicasts may pass in multicast or promiscuous mode */
  if (ETH_30_VTT_FILTER_PROMISCUOUS_MODE == Eth_30_Vtt_FilterMode[CtrlIdx])
  {
    RetVal = TRUE;
  }
  else if ((ETH_30_VTT_FILTER_MULTICAST_MODE == Eth_30_Vtt_FilterMode[CtrlIdx]) && ((PhysAddrPtr[0] & ETH_30_VTT_MULTICAST_MASK) != 0))
  {
    /* Is multicast ... */
    /* calculate CRC32 */
    uint32 Crc32 = Eth_30_Vtt_VCalculateCRC32(PhysAddrPtr, ETH_PHYS_ADDR_LEN_BYTE, 0xFFFFFFFF, TRUE);
    uint8 BucketNum = (uint8)((Crc32 & ETH_30_VTT_MULTICAST_HASH_31_27_MASK) >> ETH_30_VTT_MULTICAST_HASH_31_27_SHIFT);

    /* Increase bucket counter */
    uint8 BucketCntBytePos = BucketNum % 2;
    uint8 BucketCnt = BucketNum >> 1;

    if (0 == BucketCntBytePos)
    {
      if (((uint32)Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][BucketCnt] & ETH_30_VTT_BUCKET_LOWER_CNT_MASK) > 0)
      {
        RetVal = TRUE;
      }
    }
    else
    {
      if ((((uint32)Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][BucketCnt] & ETH_30_VTT_BUCKET_UPPER_CNT_MASK) >> ETH_30_VTT_BUCKET_UPPER_CNT_SHIFT) > 0)
      {
        RetVal = TRUE;
      }
    }
  }
  else
  {
    /* Nothing to do */
  }

  return RetVal;
} /* Eth_30_Vtt_VIsOpenMulticast() */
#endif /* ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER */

/***********************************************************************************************************************
 *  Eth_30_Vtt_VIsUnicast
 **********************************************************************************************************************/
/*! Internal comment removed.
 *   
 **********************************************************************************************************************/
STATIC FUNC(boolean, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_VIsUnicast(
          uint8                        CtrlIdx,
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) PhysAddrPtr)
{
  uint8 MacIdx = 0;
  boolean FilterCond = TRUE;

  while (ETH_PHYS_ADDR_LEN_BYTE > MacIdx)
  {
    if (Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx)[MacIdx] != PhysAddrPtr[MacIdx])
    {
      FilterCond = FALSE;
      break;
    }
    MacIdx++;
  }

  return FilterCond;
} /* Eth_30_Vtt_VIsUnicast() */

/***********************************************************************************************************************
 *  Eth_30_Vtt_VInitBuffersAndDescriptors
 **********************************************************************************************************************/
/*! Internal comment removed.
 *   
 **********************************************************************************************************************/
FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_VInitBuffersAndDescriptors(
  uint8 CtrlIdx)
{
  uint16 DescriptorIdx = 0;
  uint8 RingIdx;

  ETH_30_VTT_DUMMY_STATEMENT(CtrlIdx); /* PRQA S 3199 */ /* MD_Eth_30_Vtt_3199 */

  /* VTT_Eth only implements one ring at the moment, but has to cope with hardware which has several rings configured */
  for(RingIdx = 0; RingIdx < 1; RingIdx++)
  {
    for(DescriptorIdx = 0; DescriptorIdx < Eth_30_Vtt_VCfgGetRxBufferNum(CtrlIdx, RingIdx); DescriptorIdx++)
    {
      Eth_30_Vtt_RxDescriptors[DescriptorIdx].DataPtr = (uint32*)&Eth_30_Vtt_VCfgGetRxBuf4Ring(CtrlIdx, RingIdx).BufPtr.U8[DescriptorIdx * Eth_30_Vtt_VCfgGetRxBuf4Ring(CtrlIdx, RingIdx).AlignedSegmentSize];
      Eth_30_Vtt_RxDescriptors[DescriptorIdx].SoftwareOwned = FALSE;
      Eth_30_Vtt_RxDescriptors[DescriptorIdx].LastElement = FALSE;
      Eth_30_Vtt_RxDescriptors[DescriptorIdx].AllowedPacketSize = Eth_30_Vtt_VCfgGetRxBuf4Ring(CtrlIdx, RingIdx).MaximumFrameSize;
    }
  }
  
  for(RingIdx = 0; RingIdx < 1; RingIdx++)
  {
    for(DescriptorIdx = 0; DescriptorIdx < Eth_30_Vtt_VCfgGetTxBufferNum(CtrlIdx); DescriptorIdx++)
    {
      Eth_30_Vtt_TxDescriptors[DescriptorIdx].SoftwareOwned = TRUE;
      Eth_30_Vtt_TxDescriptors[DescriptorIdx].LastElement = FALSE;
      Eth_30_Vtt_VCfgGetTxConfirmationCalled(CtrlIdx, DescriptorIdx) = (uint8)ETH_30_VTT_CONFIRMATION_CALLED;
    }
  }

  /* Mark last ring elements */
  Eth_30_Vtt_RxDescriptors[Eth_30_Vtt_VCfgGetRxBufferNum(CtrlIdx, 0) - 1].LastElement = TRUE;
  Eth_30_Vtt_TxDescriptors[Eth_30_Vtt_VCfgGetTxBufferNum(CtrlIdx) - 1].LastElement = TRUE;
} /* Eth_30_Vtt_VInitBuffersAndDescriptors() */

/***********************************************************************************************************************
 *  Eth_30_Vtt_VCalculateCRC32
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 **********************************************************************************************************************/
STATIC FUNC(uint32, CRC_CODE) Eth_30_Vtt_VCalculateCRC32(
  Crc_DataRefType CrcDataPtr,
  uint32          CrcLength,
  uint32          CrcStartValue32,
  boolean         CrcIsFirstCall)
{
  uint32 CrcValue_u32;
  uint16 DataPtrAddr_u16 = 0;

  /* On first call in a sequence or individual CRC calculation CrcStartValue32
   * is ignored and initial value is used */
  if( CrcIsFirstCall == TRUE )
  {
      CrcValue_u32 = ETH_30_VTT_CRC_INITIAL_VALUE32;
  }
  else
  {
      CrcValue_u32 = (ETH_30_VTT_CRC_FINAL_XOR_CRC32 ^ CrcStartValue32);
  }

  while ( CrcLength > 0 )
  {
    uint16 i16; /* loop counter 1..8 */

    CrcValue_u32 ^= 0xFFuL & (uint32) CrcDataPtr[DataPtrAddr_u16];

    for ( i16 = 0; i16 < 8; i16 ++ )
    {
      if ( (CrcValue_u32 & 1) > 0 )
      {
        CrcValue_u32 = ( CrcValue_u32 >> 1 ) ^ ETH_30_VTT_CRC_POLYNOMIAL_32_REFLECT;
      }
      else
      {
        CrcValue_u32 = ( CrcValue_u32 >> 1 );
      }
    }
    CrcLength--;
    DataPtrAddr_u16++;
  }
  return ETH_30_VTT_CRC_FINAL_XOR_CRC32 ^ CrcValue_u32;
} /* Eth_30_Vtt_VCalculateCRC32() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Eth_30_Vtt_ControllerInit
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_ControllerInit(
  uint8 CtrlIdx,
  uint8 CfgIdx)
{
  uint32 Idx;
  Std_ReturnType RetVal = E_OK;

  Eth_30_Vtt_CheckDetErrorReturnValue(
    (ETH_30_VTT_MAX_CTRLS_TOTAL > CtrlIdx),
    CtrlIdx,
    ETH_30_VTT_SID_CONTROLLER_INIT,
    ETH_30_VTT_E_INV_CTRL_IDX,
    E_NOT_OK);
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (ETH_STATE_UNINIT != Eth_30_Vtt_State),
    CtrlIdx,
    ETH_30_VTT_SID_CONTROLLER_INIT,
    ETH_30_VTT_E_NOT_INITIALIZED,
    E_NOT_OK);
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (ETH_30_VTT_MAX_CFGS_TOTAL > CfgIdx),
    CtrlIdx,
    ETH_30_VTT_SID_CONTROLLER_INIT,
    ETH_30_VTT_E_INV_CONFIG,
    E_NOT_OK);

#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  Eth_30_Vtt_Mode[CtrlIdx] = ETH_MODE_DOWN;
#endif /* ETH_30_VTT_DEV_ERROR_DETECT */

  ETH_30_VTT_DUMMY_STATEMENT(CfgIdx);
  Eth_30_Vtt_SetControllerMode(CtrlIdx, ETH_MODE_DOWN);

  /* Initialize busy tables and buffer states */
  Idx = Eth_30_Vtt_VCfgGetTxBufferNum(CtrlIdx);
  while( 0 < Idx )
  {
    Idx--;
    Eth_30_Vtt_VCfgGetUlTxConfState(CtrlIdx, Idx) = 0;
    Eth_30_Vtt_VCfgGetTxBufferBusyTable(CtrlIdx, Idx) = (uint8)ETH_30_VTT_BUFFER_NOT_BUSY;
    Eth_30_Vtt_VCfgGetTxConfirmationCalled(CtrlIdx, Idx) = (uint8)ETH_30_VTT_CONFIRMATION_CALLED;
  }

  /* Initialize ring pointers */
  Eth_30_Vtt_RxBufferRingCurProcPos[CtrlIdx] = 0;

  Eth_30_Vtt_VInitBuffersAndDescriptors(CtrlIdx);

  VttEth_SetDescriptorBaseAddress(&Eth_30_Vtt_RxDescriptors[0], &Eth_30_Vtt_TxDescriptors[0]);

  /* Initialize multicast bucket counters */
#if (ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER == STD_ON)
  Idx = ETH_30_VTT_MULTICAST_BUCKET_COUNTERS_MOD_2;
  while (0 < Idx)
  {
    Idx--;
    Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][Idx] = 0x00;
  }
  Eth_30_Vtt_FilterMode[CtrlIdx] = ETH_30_VTT_FILTER_MULTICAST_MODE;
#endif /* ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER */

  return RetVal;
} /* Eth_30_Vtt_ControllerInit() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  Eth_30_Vtt_SetControllerMode
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_SetControllerMode(
  uint8        CtrlIdx,
  Eth_ModeType CtrlMode)
{
  uint8 Idx;

  Eth_30_Vtt_CheckDetErrorReturnValue(
    (ETH_30_VTT_MAX_CTRLS_TOTAL > CtrlIdx),
    CtrlIdx,
    ETH_30_VTT_SID_SET_CONTROLLER_MODE,
    ETH_30_VTT_E_INV_CTRL_IDX,
    E_NOT_OK);
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (ETH_STATE_INIT == Eth_30_Vtt_State),
    CtrlIdx,
    ETH_30_VTT_SID_SET_CONTROLLER_MODE,
    ETH_30_VTT_E_NOT_INITIALIZED,
    E_NOT_OK);

  if ( ETH_MODE_DOWN == CtrlMode )
  {
    /* Unlock all transmit buffers */
    Idx = Eth_30_Vtt_VCfgGetTxBufferNum(CtrlIdx);
    while( 0 < Idx )
    {
      Idx--;
      Eth_30_Vtt_VCfgGetUlTxConfState(CtrlIdx, Idx) = 0;
      Eth_30_Vtt_VCfgGetTxBufferBusyTable(CtrlIdx, Idx) = (uint8)ETH_30_VTT_BUFFER_NOT_BUSY;
      Eth_30_Vtt_VCfgGetTxConfirmationCalled(CtrlIdx, Idx) = (uint8)ETH_30_VTT_CONFIRMATION_CALLED;
    }

    Eth_30_Vtt_TxDescRingCurProcPos[CtrlIdx][0] = 0;
    Eth_30_Vtt_TxDescRingFreePos[CtrlIdx][0] = 0;
    VttEth_SetControllerMode(CtrlIdx, ETH_MODE_DOWN);
  }
  else
  {
      VttEth_SetControllerMode(CtrlIdx, ETH_MODE_ACTIVE);
  }

#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  Eth_30_Vtt_Mode[CtrlIdx] = CtrlMode;
#endif /* ETH_30_VTT_DEV_ERROR_DETECT */

  return E_OK;
} /* Eth_30_Vtt_SetControllerMode() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Eth_30_Vtt_GetControllerMode
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_GetControllerMode(
        uint8                                          CtrlIdx,
  P2VAR(Eth_ModeType, AUTOMATIC, ETH_30_VTT_APPL_DATA) CtrlModePtr)
{
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (ETH_30_VTT_MAX_CTRLS_TOTAL > CtrlIdx),
    CtrlIdx,
    ETH_30_VTT_SID_GET_CONTROLLER_MODE,
    ETH_30_VTT_E_INV_CTRL_IDX,
    E_NOT_OK);
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (ETH_STATE_INIT == Eth_30_Vtt_State),
    CtrlIdx,
    ETH_30_VTT_SID_GET_CONTROLLER_MODE,
    ETH_30_VTT_E_NOT_INITIALIZED,
    E_NOT_OK);
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (NULL_PTR != CtrlModePtr),
    CtrlIdx,
    ETH_30_VTT_SID_GET_CONTROLLER_MODE,
    ETH_30_VTT_E_INV_POINTER,
    E_NOT_OK);

  VttEth_GetControllerMode(CtrlIdx, CtrlModePtr);

  return E_OK;
} /* Eth_30_Vtt_GetControllerMode() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if (ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER == STD_ON)
/**********************************************************************************************************************
 *  Eth_30_Vtt_UpdatePhysAddrFilter
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *********************************************************************************************************************/
FUNC (Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_UpdatePhysAddrFilter(
          uint8                                   CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETH_30_VTT_APPL_DATA) PhysAddrPtr,
          Eth_FilterActionType                    Action)
{
  uint32 Crc32;
  uint8 BucketNum;
  uint8 BucketCntBytePos;
  uint8 BucketCnt;
  uint8 CurBucketCnt;

  Eth_30_Vtt_CheckDetErrorReturnValue(
    (ETH_30_VTT_MAX_CTRLS_TOTAL > CtrlIdx),
    CtrlIdx,
    ETH_30_VTT_SID_UPDATE_PHYS_ADDR_FILTER,
    ETH_30_VTT_E_INV_CTRL_IDX,
    E_NOT_OK);
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (ETH_STATE_INIT == Eth_30_Vtt_State),
    CtrlIdx,
    ETH_30_VTT_SID_UPDATE_PHYS_ADDR_FILTER,
    ETH_30_VTT_E_NOT_INITIALIZED,
    E_NOT_OK);
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (NULL_PTR != PhysAddrPtr),
    CtrlIdx,
    ETH_30_VTT_SID_UPDATE_PHYS_ADDR_FILTER,
    ETH_30_VTT_E_INV_POINTER,
    E_NOT_OK);

  if (TRUE == Eth_30_Vtt_VIsBroadcast(PhysAddrPtr))
  {
    Eth_30_Vtt_FilterMode[CtrlIdx] = ETH_30_VTT_FILTER_PROMISCUOUS_MODE;
  }
  else if (TRUE == Eth_30_Vtt_VIsZeroAddress(PhysAddrPtr))
  {
    Eth_30_Vtt_FilterMode[CtrlIdx] = ETH_30_VTT_FILTER_MULTICAST_MODE;
  }
  else
  {
    /* calculate CRC32 */
    Crc32 = Eth_30_Vtt_VCalculateCRC32(PhysAddrPtr, ETH_PHYS_ADDR_LEN_BYTE, 0xFFFFFFFF, TRUE);
    BucketNum = (uint8)((Crc32 & ETH_30_VTT_MULTICAST_HASH_31_27_MASK) >> ETH_30_VTT_MULTICAST_HASH_31_27_SHIFT);

    /* Increase bucket counter */
    BucketCntBytePos = BucketNum % 2;
    BucketCnt = BucketNum >> 1;

    if (ETH_ADD_TO_FILTER == Action)
    {
      if (0 == BucketCntBytePos)
      {
        CurBucketCnt = (uint8)((uint32)Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][BucketCnt] & ETH_30_VTT_BUCKET_LOWER_CNT_MASK);
        if (ETH_30_VTT_BUCKET_MAX_CNT > CurBucketCnt)
        {
          CurBucketCnt++;

          /* Reset lower counter */
          Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][BucketCnt] &= ETH_30_VTT_BUCKET_UPPER_CNT_MASK;
          /* Set lower counter */
          Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][BucketCnt] |= (uint8)((uint32)CurBucketCnt & ETH_30_VTT_BUCKET_LOWER_CNT_MASK);
        }
      }
      else
      {
        CurBucketCnt = (uint8)(((uint32)Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][BucketCnt] & ETH_30_VTT_BUCKET_UPPER_CNT_MASK)
                                                                                        >> ETH_30_VTT_BUCKET_UPPER_CNT_SHIFT);
        if (ETH_30_VTT_BUCKET_MAX_CNT > CurBucketCnt)
        {
          CurBucketCnt++;

          /* Reset upper counter */
          Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][BucketCnt] &= ETH_30_VTT_BUCKET_LOWER_CNT_MASK;
          /* Set upper counter */
          Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][BucketCnt] |= (uint8)(((uint32)CurBucketCnt << ETH_30_VTT_BUCKET_UPPER_CNT_SHIFT)
                                                                                          & ETH_30_VTT_BUCKET_UPPER_CNT_MASK);
        }
      }
    }
    else
    {
      if (0 == BucketCntBytePos)
      {
        CurBucketCnt = (uint8)((uint32)Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][BucketCnt] & ETH_30_VTT_BUCKET_LOWER_CNT_MASK);
        if (0 < CurBucketCnt)
        {
          CurBucketCnt--;

          /* Reset lower counter */
          Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][BucketCnt] &= ETH_30_VTT_BUCKET_UPPER_CNT_MASK;
          /* Set lower counter */
          Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][BucketCnt] |= (uint8)((uint32)CurBucketCnt & ETH_30_VTT_BUCKET_LOWER_CNT_MASK);
        }
      }
      else
      {
        CurBucketCnt = (uint8)(((uint32)Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][BucketCnt] & ETH_30_VTT_BUCKET_UPPER_CNT_MASK)
                                                                                        >> ETH_30_VTT_BUCKET_UPPER_CNT_SHIFT);
        if (0 < CurBucketCnt)
        {
          CurBucketCnt--;

          /* Reset upper counter */
          Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][BucketCnt] &= ETH_30_VTT_BUCKET_LOWER_CNT_MASK;
          /* Set upper counter */
          Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][BucketCnt] |= (uint8)(((uint32)CurBucketCnt << ETH_30_VTT_BUCKET_UPPER_CNT_SHIFT)
                                                                                          & ETH_30_VTT_BUCKET_UPPER_CNT_MASK);
        }
      }
    }
  }
  return E_OK;
} /* Eth_30_Vtt_UpdatePhysAddrFilter() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER */

#if ( ETH_30_VTT_MII_INTERFACE == STD_ON )
/***********************************************************************************************************************
 *  Eth_30_Vtt_WriteMii
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 **********************************************************************************************************************/
FUNC(Eth_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_WriteMii(
  uint8  CtrlIdx,
  uint8  TrcvIdx,
  uint8  RegIdx,
  uint16 RegVal)
{
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (ETH_30_VTT_MAX_CTRLS_TOTAL > CtrlIdx),
    CtrlIdx,
    ETH_30_VTT_SID_WRITE_MII,
    ETH_30_VTT_E_INV_CTRL_IDX,
    E_NOT_OK);
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (ETH_STATE_INIT == Eth_30_Vtt_State),
    CtrlIdx,
    ETH_30_VTT_SID_WRITE_MII,
    ETH_30_VTT_E_NOT_INITIALIZED,
    E_NOT_OK);
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (ETH_30_VTT_MAX_MII_REG_TOTAL > RegIdx),
    CtrlIdx,
    ETH_30_VTT_SID_WRITE_MII,
    ETH_30_VTT_E_INV_PARAM,
    E_NOT_OK);

  ETH_30_VTT_DUMMY_STATEMENT(CtrlIdx); /* PRQA S 3199 */ /* MD_Eth_30_Vtt_3199 */
  ETH_30_VTT_DUMMY_STATEMENT(TrcvIdx); /* PRQA S 3199 */ /* MD_Eth_30_Vtt_3199 */
  ETH_30_VTT_DUMMY_STATEMENT(RegIdx); /* PRQA S 3199 */ /* MD_Eth_30_Vtt_3199 */
  ETH_30_VTT_DUMMY_STATEMENT(RegVal); /* PRQA S 3199 */ /* MD_Eth_30_Vtt_3199 */

  return ETH_E_NO_ACCESS; /* inform caller that such functionality isn't available by VTT
                           * -> otherwise returned data could be mis-interpreted */

} /* Eth_30_Vtt_WriteMii() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* ETH_30_VTT_MII_INTERFACE */

#if ( ETH_30_VTT_MII_INTERFACE == STD_ON )
/***********************************************************************************************************************
 *  Eth_30_Vtt_ReadMii
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 **********************************************************************************************************************/
FUNC(Eth_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_ReadMii(
         uint8                                   CtrlIdx,
         uint8                                   TrcvIdx,
         uint8                                   RegIdx,
  P2VAR(uint16, AUTOMATIC, ETH_30_VTT_APPL_DATA) RegValPtr)
{
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (ETH_30_VTT_MAX_CTRLS_TOTAL > CtrlIdx),
    CtrlIdx,
    ETH_30_VTT_SID_READ_MII,
    ETH_30_VTT_E_INV_CTRL_IDX,
    E_NOT_OK);
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (ETH_STATE_INIT == Eth_30_Vtt_State),
    CtrlIdx,
    ETH_30_VTT_SID_READ_MII,
    ETH_30_VTT_E_NOT_INITIALIZED,
    E_NOT_OK);
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (NULL_PTR != RegValPtr),
    CtrlIdx,
    ETH_30_VTT_SID_READ_MII,
    ETH_30_VTT_E_INV_POINTER,
    E_NOT_OK);

  ETH_30_VTT_DUMMY_STATEMENT(CtrlIdx); /* PRQA S 3199 */ /* MD_Eth_30_Vtt_3199 */
  ETH_30_VTT_DUMMY_STATEMENT(TrcvIdx); /* PRQA S 3199 */ /* MD_Eth_30_Vtt_3199 */
  ETH_30_VTT_DUMMY_STATEMENT(RegIdx); /* PRQA S 3199 */ /* MD_Eth_30_Vtt_3199 */
  ETH_30_VTT_DUMMY_STATEMENT(RegValPtr); /* PRQA S 3199 */ /* MD_Eth_30_Vtt_3199 */

  return ETH_E_NO_ACCESS; /* inform caller that such functionality isn't available by VTT
                           * -> otherwise returned data could be mis-interpreted */

} /* Eth_30_Vtt_ReadMii() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* ( ETH_30_VTT_MII_INTERFACE == STD_ON ) */

#if ( ETH_30_VTT_ENABLE_MIB_COUNTER == STD_ON )
/***********************************************************************************************************************
 *  Eth_30_Vtt_GetCounterState
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_GetCounterState(
         uint8                                   CtrlIdx,
        uint16                                   CtrOffs,
  P2VAR(uint32, AUTOMATIC, ETH_30_VTT_APPL_DATA) CtrValPtr)
{
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (ETH_30_VTT_MAX_CTRLS_TOTAL > CtrlIdx),
    CtrlIdx,
    ETH_30_VTT_SID_GET_COUNTER_STATE,
    ETH_30_VTT_E_INV_CTRL_IDX,
    E_NOT_OK);
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (ETH_STATE_INIT == Eth_30_Vtt_State),
    CtrlIdx,
    ETH_30_VTT_SID_GET_COUNTER_STATE,
    ETH_30_VTT_E_NOT_INITIALIZED,
    E_NOT_OK);
  Eth_30_Vtt_CheckDetErrorReturnValue(
    (NULL_PTR != CtrValPtr),
    CtrlIdx,
    ETH_30_VTT_SID_GET_COUNTER_STATE,
    ETH_30_VTT_E_INV_POINTER,
    E_NOT_OK);

  ETH_30_VTT_DUMMY_STATEMENT(CtrlIdx); /* PRQA S 3199 */ /* MD_Eth_30_Vtt_3199 */
  ETH_30_VTT_DUMMY_STATEMENT(CtrOffs); /* PRQA S 3199 */ /* MD_Eth_30_Vtt_3199 */

  *CtrValPtr = 0;
  
  return E_OK;
} /* Eth_30_Vtt_GetCounterState() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* ETH_30_VTT_ENABLE_MIB_COUNTER */

/***********************************************************************************************************************
 *  Eth_30_Vtt_Transmit
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_Transmit(
          uint8                               CtrlIdx,
          uint8                               BufIdx,
          Eth_FrameType                       FrameType,
          boolean                             TxConfirmation,
          uint16                              LenByte,
  P2CONST(uint8, AUTOMATIC, ETH_30_VTT_CONST) PhysAddrPtr)
{
  /* no transformation between local and global controller index necessary */
  return Eth_30_Vtt_VTransmit(CtrlIdx,
                              BufIdx,
                              FrameType,
                              TxConfirmation,
                              LenByte,
                              PhysAddrPtr,
                              Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx));
} /* Eth_30_Vtt_Transmit() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */

/***********************************************************************************************************************
 *  Eth_30_Vtt_Receive
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 **********************************************************************************************************************/
FUNC(void, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_Receive(
        uint8                                             CtrlIdx,
  P2VAR(Eth_RxStatusType, AUTOMATIC, ETH_30_VTT_APPL_VAR) RxStatusPtr)
{
  /* PRQA S 0759 1 */ /* MD_Eth_30_Vtt_0759 */
  Eth_30_Vtt_AlignedFrameCacheType RxBufDataPtr;
  uint16 FrameType;
  uint8* frameData;

  Eth_30_Vtt_CheckDetErrorReturnVoid(
    (ETH_30_VTT_MAX_CTRLS_TOTAL > CtrlIdx),
    CtrlIdx,
    ETH_30_VTT_SID_RECEIVE,
    ETH_30_VTT_E_INV_CTRL_IDX);
  Eth_30_Vtt_CheckDetErrorReturnVoid(
    (ETH_STATE_INIT == Eth_30_Vtt_State),
    CtrlIdx,
    ETH_30_VTT_SID_RECEIVE,
    ETH_30_VTT_E_NOT_INITIALIZED);
  Eth_30_Vtt_CheckDetErrorReturnVoid(
    (ETH_MODE_ACTIVE == Eth_30_Vtt_Mode[CtrlIdx]),
    CtrlIdx,
    ETH_30_VTT_SID_RECEIVE,
    ETH_30_VTT_E_INV_MODE);

    CANoeAPI_AtomicBegin();

    if(Eth_30_Vtt_RxDescriptors[Eth_30_Vtt_RxBufferRingCurProcPos[CtrlIdx]].SoftwareOwned == TRUE)
    {
      CANoeAPI_AtomicEnd();

      /* discard packets from own source address */
      frameData = (uint8*)Eth_30_Vtt_RxDescriptors[Eth_30_Vtt_RxBufferRingCurProcPos[CtrlIdx]].DataPtr;

      /* filter packet */
      if ((TRUE == Eth_30_Vtt_VIsUnicast(CtrlIdx, &frameData[ETH_30_VTT_RX_DST_OFFSET_U8])) ||
#if (ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER == STD_ON)
          (TRUE == Eth_30_Vtt_VIsOpenMulticast(CtrlIdx, &frameData[ETH_30_VTT_RX_DST_OFFSET_U8])) ||
#endif /* ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER */
          (TRUE == Eth_30_Vtt_VIsBroadcast(&frameData[ETH_30_VTT_RX_DST_OFFSET_U8])))
      {
        /* Get buffer pointer */
        RxBufDataPtr.Native = Eth_30_Vtt_RxDescriptors[Eth_30_Vtt_RxBufferRingCurProcPos[CtrlIdx]].DataPtr;

        /* Get frame type */
        FrameType = IPBASE_NTOH16(RxBufDataPtr.U16[(ETH_30_VTT_RX_TYPE_OFFSET_U16)]);

        Eth_30_Vtt_SetFrameId(CtrlIdx, (P2VAR(uint8, AUTOMATIC, AUTOMATIC))&RxBufDataPtr.Native[ETH_30_VTT_RX_DATA_OFFSET_U32],
        Eth_30_Vtt_RxDescriptors[Eth_30_Vtt_RxBufferRingCurProcPos[CtrlIdx]].ReceivedPacketSize - ETH_30_VTT_HDR_LEN_BYTE);

        /*  Callback  */
        EthIf_RxIndication(CtrlIdx,
          FrameType,
          Eth_30_Vtt_VIsBroadcast(&RxBufDataPtr.U8[ETH_30_VTT_RX_DST_OFFSET_U8]),
          &RxBufDataPtr.U8[ETH_30_VTT_RX_SRC_OFFSET_U8],
          &RxBufDataPtr.Native[ETH_30_VTT_RX_DATA_OFFSET_U32],
          Eth_30_Vtt_RxDescriptors[Eth_30_Vtt_RxBufferRingCurProcPos[CtrlIdx]].ReceivedPacketSize - ETH_30_VTT_HDR_LEN_BYTE);
          *RxStatusPtr = ETH_RECEIVED;
        }
    }
    else
    {
      /* descriptor is locked by hardware - interrupt for a frame which has been processed before */
      CANoeAPI_AtomicEnd();
      *RxStatusPtr = ETH_NOT_RECEIVED;
      return;
    }
    CANoeAPI_AtomicBegin();
    /* unlock descriptor and go to next one*/
    Eth_30_Vtt_RxDescriptors[Eth_30_Vtt_RxBufferRingCurProcPos[CtrlIdx]].SoftwareOwned = FALSE;

    /* check next descriptor */
    Eth_30_Vtt_VIncRxBufRingCurProcPtr(CtrlIdx, 0);
    if(Eth_30_Vtt_RxDescriptors[Eth_30_Vtt_RxBufferRingCurProcPos[CtrlIdx]].SoftwareOwned == TRUE)
    {
      *RxStatusPtr = ETH_RECEIVED_MORE_DATA_AVAILABLE;
    }
    CANoeAPI_AtomicEnd();

} /* Eth_30_Vtt_Receive() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  Eth_30_Vtt_RxIrqHdlr
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 **********************************************************************************************************************/
FUNC(void, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_RxIrqHdlr(
  uint8 CtrlIdx)
{
  /* First parameter is the CANoe channel number. The Channel equals the CtrlIdx plus one */
  Eth_RxStatusType RxStatus;
  do
  {
    Eth_30_Vtt_Receive(CtrlIdx, &RxStatus);
  }
  while(RxStatus == ETH_RECEIVED_MORE_DATA_AVAILABLE);
} /* Eth_30_Vtt_RxIrqHdlr() */

/***********************************************************************************************************************
 *  Eth_30_Vtt_LL_MainFunction
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 **********************************************************************************************************************/
FUNC(void, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_LL_MainFunction(void)
{

} /* Eth_30_Vtt_LL_MainFunction() */

#define ETH_30_VTT_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* module specific MISRA deviations:
 MD_Eth_30_Vtt_0310:
      Reason:     The pointer where the cast led to has a less stricter alignment. The cast
                  is necessary since the array represents a continuous data buffer.
      Risk:       There is no risk since the array is a continuous data buffer.
      Prevention: Covered by code review.
 MD_Eth_30_Vtt_3199:
      Reason:     Dummy statement to avoid compiler warnings.
      Risk:       There is no risk as such statements have no effect on the code.
      Prevention: Covered by code review.
*/

/**********************************************************************************************************************
 *  END OF FILE: Eth_30_Vtt_LL.c
 *********************************************************************************************************************/

