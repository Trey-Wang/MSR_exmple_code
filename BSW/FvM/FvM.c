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
/**       \file   FvM.c
 *        \brief  FvM source file
 *
 *      \details  This is the implementation of the Freshness Value Manager.
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

#define FVM_SOURCE
/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/
/* *INDENT-OFF* */
/* PRQA S 3453 EOF */ /* MD_MSR_19.7 */
/* PRQA S 0779 EOF */ /* MD_CSL_0779 */
/* PRQA S 0857, 0639 EOF */ /* MD_MSR_1.1_857, MD_MSR_1.1_639 */
/* PRQA S 3355, 3356, 3358, 3359 EOF */ /* MD_CSL_3355_3356_3358_3359  */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "FvM.h"
#include "SchM_FvM.h"
#include "Rte_SecOC.h"

#include "FvM_Lcfg.h"
#include "FvM_PBcfg.h"
#include "FvM_PBcfg.h"
#include "SecOC.h"
#include "SecOC_Cbk.h"
#include "vstdlib.h"

#if (FVM_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of FvM header file */
#if (  (FVM_SW_MAJOR_VERSION != (2u)) \
    || (FVM_SW_MINOR_VERSION != (3u)) \
    || (FVM_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of <FvM>.c and <FvM>.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define FVM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ((FVM_EXISTS_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO == STD_ON) && (FVM_STARTUPCYCLECOUNTER == STD_ON))
/**********************************************************************************************************************
  FvM_FVJaspar_SetVerifyOverrideStatus
**********************************************************************************************************************/
/*! \brief       Sets all rx freshness values to the passed override status.
 *  \details     -
 *  \param[in]   overrideStatus
 *               0 ==> Override VerifyStatus to "Fail" until further notice
 *               1 ==> Override VerifyStatus to "Fail" until NumberOfMessagesToOverride of the
 *                     corresponding SecOC processing is reached
 *               2 ==> Cancel override of VerifyStatus
 *               41 ==> Override VerifyStatus to "Pass" until NumberOfMessagesToOverride of the
 *                      corresponding SecOC processing is reached;
 *                      only available if SecOCEnableForcedPassOverride is set to TRUE
 *  \context     TASK|ISR2
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_SetVerifyOverrideStatus(uint8 overrideStatus);
#endif

#if(FVM_RXFRESHNESSVALUEINFO == STD_ON) /* COV_FVM_VAR_SYNC_MSG_ALWAYS_ON */
/**********************************************************************************************************************
  FvM_FVJaspar_CheckSyncMessageValues
**********************************************************************************************************************/
/*!
 *  \brief      Checks the plausibility of the received sync message values.
    \details  -
    \param[in]  latestFreshnessValueIdx  freshness value layout info id.
 */
LOCAL_INLINE FUNC(Std_ReturnType, FVM_CODE) FvM_FVJaspar_CheckSyncMessageValues(FvM_FreshnessValueLayoutInfoIterType latestFreshnessValueIdx);
#endif

/**********************************************************************************************************************
  FvM_FVJaspar_SetTripCounter
**********************************************************************************************************************/
/*!
 *  \brief      Sets the Trip Counter Value.
    \details  -
    \param[in]  freshnessValueLayoutId  freshness value layout info id.
    \param[in]  TripCounterValue        trip counter value
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_SetTripCounter(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId, uint32 TripCounterValue);

/**********************************************************************************************************************
  FvM_FVJaspar_GetTripCounter
**********************************************************************************************************************/
/*!
 * \brief      Returns the Trip Counter Value.
    \details  -
    \param[in]  freshnessValueLayoutId  freshness value layout info id.
    \return    trip counter value.
 */
LOCAL_INLINE FUNC(uint32, FVM_CODE) FvM_FVJaspar_GetTripCounter(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId);

/**********************************************************************************************************************
  FvM_FVJaspar_SetResetCounter
**********************************************************************************************************************/
/*!
 * \brief      Sets the Reset Counter Value.
   \details  -
   \param[in]  freshnessValueLayoutId  freshness value layout info id.
   \param[in]  ResetCounterValue       reset counter value
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_SetResetCounter(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId, uint64 ResetCounterValue);

/**********************************************************************************************************************
  FvM_FVJaspar_GetResetCounter
**********************************************************************************************************************/
/*!
 * \brief      Returns the Reset Counter Value.
   \details  -
   \param[in]  freshnessValueLayoutId  freshness value layout info id.
   \return     reset counter value.
 */
LOCAL_INLINE FUNC(uint64, FVM_CODE) FvM_FVJaspar_GetResetCounter(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId);

/**********************************************************************************************************************
  FvM_FVJaspar_SetMessageCounter
**********************************************************************************************************************/
/*!
 * \brief      Sets the Message Counter Value.
   \details  -
   \param[in]  freshnessValueLayoutId  freshness value layout info id.
   \param[in]  MessageCounterValue     message counter value
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_SetMessageCounter(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId, uint16 MessageCounterValue);

/**********************************************************************************************************************
  FvM_FVJaspar_GetMessageCounter
**********************************************************************************************************************/
/*!
 * \brief      Returns the Message Counter Value.
   \details  -
   \param[in]  freshnessValueLayoutId  freshness value layout info id.
   \return     message counter value.
 */
LOCAL_INLINE FUNC(uint16, FVM_CODE) FvM_FVJaspar_GetMessageCounter(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId);

#if (FVM_TXFRESHNESSVALUEINFO ==  STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_IncMessageCounter
**********************************************************************************************************************/
/*!
 * \brief      Increments the Message Counter Value.
   \details  -
   \param[in]  freshnessValueLayoutId  freshness value layout info id.
   \param[in]  freshnessValueID        freshness value id.
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_IncMessageCounter(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId, uint16 freshnessValueID);
#endif

#if (FVM_EXISTS_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO == STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_MessageCounterCarryProcessing
**********************************************************************************************************************/
/*!
 * \brief      Does Carry processing for the truncated Message Counter Value.
   \details  -
   \param[in]  freshnessValueInfoId  freshness value id
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_MessageCounterCarryProcessing(FvM_RxFreshnessValueInfoIterType freshnessValueInfoId);
#endif

#if (FVM_TXFRESHNESSVALUEINFO ==  STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_SetResetFlag
**********************************************************************************************************************/
/*!
 * \brief      Sets the reset flag.
   \details  -
   \param[in]  freshnessValueLayoutId  freshness value layout info id.
   \param[in]  ResetCounterValue       reset counter value
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_SetResetFlag(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId, uint64 ResetCounterValue);
#endif

/**********************************************************************************************************************
  FvM_FVJaspar_GetResetFlagValueMask
**********************************************************************************************************************/
/*!
 * \brief      Get the reset flag value mask.
   \details  -
   \param[in]  freshnessValueLayoutId  freshness value layout info id.
   \return     reset flag value mask.
 */
LOCAL_INLINE FUNC(uint8, FVM_CODE) FvM_FVJaspar_GetResetFlagValueMask(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId);

/**********************************************************************************************************************
  FvM_FVJaspar_GetResetFlagOfResetCounter
**********************************************************************************************************************/
/*!
 * \brief      Get the reset flag of the the passed reset counter.
   \details  -
   \param[in]  freshnessValueLayoutId  freshness value layout info id.
   \param[in]  ResetCounterValue       reset counter value
   \return     reset flag value.
 */
LOCAL_INLINE FUNC(uint8, FVM_CODE) FvM_FVJaspar_GetResetFlagOfResetCounter(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId, uint64 ResetCounterValue);

#if(FVM_EXISTS_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO == STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_GetResetFlag
**********************************************************************************************************************/
/*!
 * \brief      Returns the Reset Flag Value.
   \details  -
   \param[in]  freshnessValueLayoutId  freshness value layout info id.
   \return     reset flag value.
 */
LOCAL_INLINE FUNC(uint8, FVM_CODE) FvM_FVJaspar_GetResetFlag(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId);
#endif

#if(FVM_TXFRESHNESSVALUEINFO == STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_GetLengthInBytes
**********************************************************************************************************************/
/*!
 * \brief      Returns the length in full bytes
   \details  -
   \param[in]  lengthInBits  the length in bits
   \return     the length in full bytes
 */
LOCAL_INLINE FUNC(uint32, FVM_CODE) FvM_FVJaspar_GetLengthInBytes(uint32 lengthInBits);
#endif

/**********************************************************************************************************************
  FvM_FVJaspar_CopyTruncFreshnessValueToBuffer
**********************************************************************************************************************/
/*!
 * \brief      Copies truncated freshness value to the freshness verify value buffer.
   \details  -
   \param[in]  freshnessValueLayoutId  freshness value layout info id.
   \param[in]  truncatedFreshnessValue truncated freshness value
   \param[in]  truncatedFreshnessBitLength truncated freshness value length in bit.
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_CopyTruncFreshnessValueToBuffer(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId, P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) truncatedFreshnessValue, uint32 truncatedFreshnessBitLength);

#if (FVM_TXFRESHNESSVALUEINFO ==  STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_CopyFreshnessVerifyValueToTruncFreshnessValue
**********************************************************************************************************************/
/*!
 * \brief      Copies freshness verify value to truncated freshness value buffer.
   \details  -
   \param[in]  freshnessValueLayoutId  freshness value layout info id.
   \param[out]  truncatedFreshnessValue truncated freshness value
   \param[in]  truncatedFreshnessBitLength truncated freshness value length in bit.
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_CopyFreshnessVerifyValueToTruncFreshnessValue(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) truncatedFreshnessValue, uint32 truncatedFreshnessBitLength);
#endif

#if (FVM_EXISTS_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO == STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_DoRxFreshnessVerifyValueLatestPreviousCompare
**********************************************************************************************************************/
/*!
 * \brief      Compares the Latest received with the Previous Trip/Rest Counter and updates the freshness verify value.
   \details  -
   \param[in]  freshnessValueInfoId    freshness value info id.
   \param[in]  latestResetCounterWithOffset  offset that is taken into account if latest and previous Trip/Rest Counter are compared.
   \return     E_OK, if freshness verify value was updated and E_NOT_OK if not freshness verify value was not updated.
 */
LOCAL_INLINE FUNC(Std_ReturnType, FVM_CODE) FvM_FVJaspar_DoRxFreshnessVerifyValueLatestPreviousCompare(FvM_RxFreshnessValueInfoIterType freshnessValueInfoId, uint64 latestResetCounterWithOffset);
#endif

#if (FVM_EXISTS_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO == STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_DoRxFreshnessVerifyValueCalculation
**********************************************************************************************************************/
/*!
 * \brief      Does the Rx freshness verify value calculation.
   \details  -
   \param[in]  freshnessValueInfoId    freshness value info id.
   \param[in]  offset                  signed offset value.
   \param[in]  negate                  TRUE  -> the negative offset is used.
                                       FALSE -> the positive offset is used.
   \return     E_OK, if successful and E_NOT_OK if not successful.
 */
LOCAL_INLINE FUNC(Std_ReturnType, FVM_CODE) FvM_FVJaspar_DoRxFreshnessVerifyValueCalculation(FvM_RxFreshnessValueInfoIterType freshnessValueInfoId, uint8 offset, boolean negate);
#endif

#if(FVM_EXISTS_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO == STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_DoRxResetFlagEvaluation
**********************************************************************************************************************/
/*!
 * \brief      Evaluates the received Rest Flag
   \details  -
   \param[in]  freshnessValueInfoId    freshness value info id.
   \param[in]  SecOCAuthVerifyAttempts Authentication Verify Attempts parameter.
   \return     E_OK, if successful and E_NOT_OK if not successful.
 */
LOCAL_INLINE FUNC(Std_ReturnType, FVM_CODE) FvM_FVJaspar_DoRxResetFlagEvaluation(FvM_RxFreshnessValueInfoIterType freshnessValueInfoId, uint16 SecOCAuthVerifyAttempts);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
  FvM_FVJaspar_SetTripCounter
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_SetTripCounter(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId, uint32 TripCounterValue)
{
  FvM_TripCounterBitSizeType bitsToCopy = FvM_GetTripCounterBitSize();
  FvM_FreshnessValueArrayIterType interVal = (FvM_FreshnessValueArrayIterType) FvM_GetFreshnessValueArrayTripCounterEndIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId);

  for(;interVal > FvM_GetFreshnessValueArrayTripCounterStartIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId); interVal--)
  {
    FvM_FreshnessValueArrayIterType destIdx = interVal - 1;
    if((bitsToCopy % 8) != 0) /* COV_FVM_TRIP_COUNTER_BIT_COUNT */
    {
      uint8 remainder = (bitsToCopy % 8);
      uint8 offset = 8 - remainder;
      uint8 copyByte = 0;
      uint8 tripCounterValueMask = 255U >> offset;
      uint8 unUsedBitsMask = 255U >> remainder;

      copyByte = (uint8) (TripCounterValue & tripCounterValueMask) << offset;
      copyByte = (uint8) (copyByte | (FvM_GetFreshnessValueArray(destIdx) & unUsedBitsMask));

      FvM_SetFreshnessValueArray(destIdx, copyByte); /* SBSW_FVM_CSL03 */

      TripCounterValue = TripCounterValue >> remainder;
      bitsToCopy = bitsToCopy - remainder;
    }
    else
    {
      uint8 copyByte = (uint8) (TripCounterValue & 255U);
      FvM_SetFreshnessValueArray(destIdx, copyByte);  /* SBSW_FVM_CSL03 */
      TripCounterValue = TripCounterValue >> 8;
      bitsToCopy = bitsToCopy - 8;
    }
  }
}

/**********************************************************************************************************************
  FvM_FVJaspar_GetTripCounter
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint32, FVM_CODE) FvM_FVJaspar_GetTripCounter(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId)
{
  uint32 TripCounterValue = 0;
  FvM_TripCounterBitSizeType bitsToCopy = FvM_GetTripCounterBitSize();
  FvM_FreshnessValueArrayIterType srcIdx = (FvM_FreshnessValueArrayIterType) FvM_GetFreshnessValueArrayTripCounterStartIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId);

  for(;srcIdx < FvM_GetFreshnessValueArrayTripCounterEndIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId); srcIdx++)
  {
    uint8 copyByte = 0;
    if ((bitsToCopy / 8) > 0) /* COV_FVM_TRIP_COUNTER_BIT_COUNT */
    {
      copyByte = FvM_GetFreshnessValueArray(srcIdx);
      TripCounterValue = TripCounterValue << 8;
      bitsToCopy = bitsToCopy - 8;
    }
    else if (bitsToCopy > 0) /* COV_FVM_TRIP_COUNTER_BIT_COUNT */
    {
      uint8 unUsedBits = 8 - bitsToCopy;
      uint8 usedBitsLastByteMask = 255U << unUsedBits;

      copyByte = (uint8) (FvM_GetFreshnessValueArray(srcIdx) & usedBitsLastByteMask) >> unUsedBits;
      TripCounterValue = TripCounterValue << bitsToCopy;

      bitsToCopy = 0;
    }
    else /* COV_FVM_MISRA */
    {
      /* do nothing */
    }
    TripCounterValue = TripCounterValue + copyByte;
  }
  return TripCounterValue;
}

/**********************************************************************************************************************
  FvM_FVJaspar_SetResetCounter
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_SetResetCounter(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId, uint64 ResetCounterValue)
{
  FvM_ResetCounterBitSizeType bitsToCopy = FvM_GetResetCounterBitSize();
  FvM_FreshnessValueArrayIterType interVal = FvM_GetFreshnessValueArrayResetCounterEndIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId);
# if (FVM_RESETCOUNTERFIRSTBITCOUNT ==  STD_ON)
  FvM_ResetCounterFirstBitCountType firstBitCount = FvM_GetResetCounterFirstBitCount();
# endif
# if (FVM_RESETCOUNTERLASTBITCOUNT == STD_ON)
  FvM_ResetCounterLastBitCountType lastBitCount = FvM_GetResetCounterLastBitCount();
# endif

  for(;interVal > FvM_GetFreshnessValueArrayResetCounterStartIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId); interVal--)
  {
    FvM_FreshnessValueArrayIterType destIdx = interVal - 1;
# if (FVM_RESETCOUNTERLASTBITCOUNT == STD_ON)
    if(lastBitCount != 0)
    {
      uint8 copyByte = 0;
      uint8 unUsedBits = 8 - lastBitCount;
      uint8 resetCounterValueMask = 255U >> unUsedBits;
      uint8 unUsedBitsMask = 255U >> lastBitCount;

      copyByte = (uint8) (ResetCounterValue & resetCounterValueMask) << unUsedBits;
      copyByte = (uint8) (copyByte | (FvM_GetFreshnessValueArray(destIdx) & unUsedBitsMask));
      FvM_SetFreshnessValueArray(destIdx, copyByte); /* SBSW_FVM_CSL03 */

      ResetCounterValue = ResetCounterValue >> lastBitCount;
      bitsToCopy = bitsToCopy - lastBitCount;
      lastBitCount = 0;
    }
    else
# endif
    if ((bitsToCopy / 8) > 0)  /* COV_FVM_RESET_COUNTER_BIT_COUNT */
    {
      uint8 copyByte = (uint8) (ResetCounterValue & 255U);
      FvM_SetFreshnessValueArray(destIdx, copyByte); /* SBSW_FVM_CSL03 */
      ResetCounterValue = ResetCounterValue >> 8;
      bitsToCopy = bitsToCopy - 8;
    }
# if(FVM_RESETCOUNTERFIRSTBITCOUNT == STD_ON)
    else if(firstBitCount != 0) /* COV_FVM_RESET_COUNTER_BIT_COUNT */
    {
      uint8 copyByte = 0;
      uint8 unUsedBits = 8 - firstBitCount;
      uint8 resetCounterValueMask = 255U >> unUsedBits;
      uint8 unUsedBitsMask = 255U << firstBitCount;

      copyByte = (uint8) (ResetCounterValue & resetCounterValueMask);
      copyByte = (uint8) (copyByte | (FvM_GetFreshnessValueArray(destIdx) & unUsedBitsMask));
      FvM_SetFreshnessValueArray(destIdx, copyByte);  /* SBSW_FVM_CSL03 */

      ResetCounterValue = ResetCounterValue >> firstBitCount;
      bitsToCopy = (FvM_ResetCounterBitSizeType) bitsToCopy - firstBitCount; /* PRQA S 0291 */ /* MD_FvM_0291 */
      firstBitCount = 0;
    }
# endif
    else /* COV_FVM_MISRA */
    {
      /* do nothing */
    }
  }
}

/**********************************************************************************************************************
  FvM_FVJaspar_GetResetCounter
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint64, FVM_CODE) FvM_FVJaspar_GetResetCounter(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId)
{
  uint64 ResetCounterValue = 0;
  FvM_ResetCounterBitSizeType bitsToCopy = FvM_GetResetCounterBitSize();
  FvM_FreshnessValueArrayIterType srcIdx = FvM_GetFreshnessValueArrayResetCounterStartIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId);
# if (FVM_RESETCOUNTERFIRSTBITCOUNT ==  STD_ON)
  FvM_ResetCounterFirstBitCountType firstBitCount = FvM_GetResetCounterFirstBitCount();
# endif
# if (FVM_RESETCOUNTERLASTBITCOUNT == STD_ON)
  FvM_ResetCounterLastBitCountType lastBitCount = FvM_GetResetCounterLastBitCount();
# endif

  for(;srcIdx < FvM_GetFreshnessValueArrayResetCounterEndIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId); srcIdx++)
  {
    uint8 copyByte = 0;
# if (FVM_RESETCOUNTERFIRSTBITCOUNT ==  STD_ON)
    if(firstBitCount != 0)
    {
      uint8 unUsedBits = 8 - firstBitCount;
      uint8 usedBitsMask = 255U >> unUsedBits;

      copyByte = (uint8) (FvM_GetFreshnessValueArray(srcIdx) & usedBitsMask);
      bitsToCopy = (FvM_ResetCounterBitSizeType) bitsToCopy - firstBitCount; /* PRQA S 0291 */ /* MD_FvM_0291 */
      firstBitCount = 0;
    }
    else
# endif
    if ((bitsToCopy / 8) > 0) /* COV_FVM_RESET_COUNTER_BIT_COUNT */
    {
      copyByte = FvM_GetFreshnessValueArray(srcIdx);

      ResetCounterValue = ResetCounterValue << 8;
      bitsToCopy = bitsToCopy - 8;
    }
# if (FVM_RESETCOUNTERLASTBITCOUNT == STD_ON)
    else if (lastBitCount != 0) /* COV_FVM_RESET_COUNTER_BIT_COUNT */
    {
      uint8 unUsedBits = 8 - lastBitCount;
      uint8 usedBitsMask = 255U << unUsedBits;

      copyByte = (uint8) (FvM_GetFreshnessValueArray(srcIdx) & usedBitsMask) >> unUsedBits;
      ResetCounterValue = ResetCounterValue << bitsToCopy;

      bitsToCopy = 0;
    }
# endif
    else /* COV_FVM_MISRA */
    { /* PRQA S 3201 */ /* MD_FvM_3201 */
      /* do nothing */
    }
    ResetCounterValue = ResetCounterValue + copyByte;
  }
  return ResetCounterValue;
}

/**********************************************************************************************************************
  FvM_FVJaspar_SetMessageCounter
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_SetMessageCounter(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId, uint16 MessageCounterValue)
{
  FvM_MessageCounterBitSizeOfFreshnessValueLayoutInfoType bitsToCopy = FvM_GetMessageCounterBitSizeOfFreshnessValueLayoutInfo(freshnessValueLayoutId);

  FvM_FreshnessValueArrayIterType interVal = FvM_GetFreshnessValueArrayMessageCounterEndIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId);
# if (FVM_FIRSTUSEDBITSOFMESSAGECOUNTEROFFRESHNESSVALUELAYOUTINFO == STD_ON)
  FvM_FirstUsedBitsOfMessageCounterOfFreshnessValueLayoutInfoType firstBitCount = FvM_GetFirstUsedBitsOfMessageCounterOfFreshnessValueLayoutInfo(freshnessValueLayoutId);
# endif
# if (FVM_LASTUSEDBITSOFMESSAGECOUNTEROFFRESHNESSVALUELAYOUTINFO == STD_ON)
  FvM_LastUsedBitsOfMessageCounterOfFreshnessValueLayoutInfoType lastBitCount = FvM_GetLastUsedBitsOfMessageCounterOfFreshnessValueLayoutInfo(freshnessValueLayoutId);
# endif

  if(FvM_FVJaspar_GetResetCounter(freshnessValueLayoutId) == FvM_GetResetCounterMax()){
    MessageCounterValue = FvM_GetMessageCounterMaxValueOfFreshnessValueLayoutInfo(freshnessValueLayoutId);
  }

  for(;interVal > FvM_GetFreshnessValueArrayMessageCounterStartIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId); interVal--)
  {
    FvM_FreshnessValueArrayIterType destIdx = interVal - 1;
# if (FVM_LASTUSEDBITSOFMESSAGECOUNTEROFFRESHNESSVALUELAYOUTINFO == STD_ON)
    if(lastBitCount != 0)
    {
      uint8 copyByte = 0;
      uint8 unUsedBits = 8 - lastBitCount;
      uint8 messageCounterValueMask = 255U >> unUsedBits;
      uint8 unUsedBitsMask = 255U >> lastBitCount;

      copyByte = (uint8) (MessageCounterValue & messageCounterValueMask) << unUsedBits;
      copyByte = (uint8) (copyByte | (FvM_GetFreshnessValueArray(destIdx) & unUsedBitsMask));
      FvM_SetFreshnessValueArray(destIdx, copyByte); /* SBSW_FVM_CSL03 */

      MessageCounterValue = MessageCounterValue >> lastBitCount;
      bitsToCopy = bitsToCopy - lastBitCount;
      lastBitCount = 0;
    }
    else
# endif
    if ((bitsToCopy / 8) > 0) /* COV_FVM_MESSAGE_COUNTER_BIT_COUNT */
    {
      uint8 copyByte = (uint8) MessageCounterValue & 255U;
      FvM_SetFreshnessValueArray(destIdx, copyByte); /* SBSW_FVM_CSL03 */
      MessageCounterValue = MessageCounterValue >> 8;
      bitsToCopy = bitsToCopy - 8;
    }
# if (FVM_FIRSTUSEDBITSOFMESSAGECOUNTEROFFRESHNESSVALUELAYOUTINFO == STD_ON)
    else if(firstBitCount != 0) /* COV_FVM_MESSAGE_COUNTER_BIT_COUNT */
    {
      uint8 copyByte = 0;
      uint8 unUsedBits = 8 - firstBitCount;
      uint8 messageCounterValueMask = 255U >> unUsedBits;
      uint8 unUsedBitsMask = 255U << firstBitCount;

      copyByte = (uint8) (MessageCounterValue & messageCounterValueMask);
      copyByte = (uint8) (copyByte | (FvM_GetFreshnessValueArray(destIdx) & unUsedBitsMask));
      FvM_SetFreshnessValueArray(destIdx, copyByte); /* SBSW_FVM_CSL03 */

      MessageCounterValue = MessageCounterValue >> firstBitCount;
      bitsToCopy = (FvM_MessageCounterBitSizeOfFreshnessValueLayoutInfoType) bitsToCopy - firstBitCount; /* PRQA S 0291 */ /* MD_FvM_0291 */
      firstBitCount = 0;
    }
# endif
    else /* COV_FVM_MISRA */
    {
      /* do nothing */
    }
  }
}

/**********************************************************************************************************************
  FvM_FVJaspar_GetMessageCounter
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint16, FVM_CODE) FvM_FVJaspar_GetMessageCounter(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId)
{
  uint16 MessageCounterValue = 0;
  FvM_MessageCounterBitSizeOfFreshnessValueLayoutInfoType bitsToCopy = FvM_GetMessageCounterBitSizeOfFreshnessValueLayoutInfo(freshnessValueLayoutId);
  FvM_FreshnessValueArrayIterType srcIdx = FvM_GetFreshnessValueArrayMessageCounterStartIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId);
# if (FVM_FIRSTUSEDBITSOFMESSAGECOUNTEROFFRESHNESSVALUELAYOUTINFO == STD_ON)
  FvM_FirstUsedBitsOfMessageCounterOfFreshnessValueLayoutInfoType firstBitCount = FvM_GetFirstUsedBitsOfMessageCounterOfFreshnessValueLayoutInfo(freshnessValueLayoutId);
# endif
# if (FVM_LASTUSEDBITSOFMESSAGECOUNTEROFFRESHNESSVALUELAYOUTINFO == STD_ON)
  FvM_LastUsedBitsOfMessageCounterOfFreshnessValueLayoutInfoType lastBitCount = FvM_GetLastUsedBitsOfMessageCounterOfFreshnessValueLayoutInfo(freshnessValueLayoutId);
# endif

  for(;srcIdx < FvM_GetFreshnessValueArrayMessageCounterEndIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId); srcIdx++)
  {
    uint8 copyByte = 0;
# if (FVM_FIRSTUSEDBITSOFMESSAGECOUNTEROFFRESHNESSVALUELAYOUTINFO == STD_ON)
    if(firstBitCount != 0)
    {
      uint8 unUsedBits = (uint8) (8 - firstBitCount);
      uint8 usedBitsFirstByteMask = 255U >> unUsedBits;

      copyByte = (uint8) (FvM_GetFreshnessValueArray(srcIdx) & usedBitsFirstByteMask);
      bitsToCopy = (FvM_MessageCounterBitSizeOfFreshnessValueLayoutInfoType) bitsToCopy - firstBitCount; /* PRQA S 0291 */ /* MD_FvM_0291 */
      firstBitCount = 0;
    }
    else
# endif
    if ((bitsToCopy / 8) > 0) /* COV_FVM_MESSAGE_COUNTER_BIT_COUNT */
    {
      { /* PRQA S 3201 */ /* MD_FvM_3201 */
        copyByte = FvM_GetFreshnessValueArray(srcIdx);
      }

      MessageCounterValue = MessageCounterValue << 8;
      bitsToCopy = bitsToCopy - 8;
    }
# if (FVM_LASTUSEDBITSOFMESSAGECOUNTEROFFRESHNESSVALUELAYOUTINFO == STD_ON)
    else if (lastBitCount != 0) /* COV_FVM_MESSAGE_COUNTER_BIT_COUNT */
    {
      uint8 unUsedBits = 8 - lastBitCount;
      uint8 usedBitsLastByteMask = 255U << unUsedBits;

      copyByte = (uint8) ((FvM_GetFreshnessValueArray(srcIdx) & usedBitsLastByteMask) >> unUsedBits);
      MessageCounterValue = MessageCounterValue << lastBitCount;

      lastBitCount = 0;
      bitsToCopy = 0;
    }
# endif
    else /* COV_FVM_MISRA */
    {
      /* do nothing */
    }
    MessageCounterValue = MessageCounterValue + copyByte;
  }
  return MessageCounterValue;
}

#if (FVM_TXFRESHNESSVALUEINFO ==  STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_IncMessageCounter
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_IncMessageCounter(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId, uint16 freshnessValueID)
{
  uint16 MessageCounterValue = FvM_FVJaspar_GetMessageCounter(freshnessValueLayoutId);
  if(MessageCounterValue != FvM_GetMessageCounterMaxValueOfFreshnessValueLayoutInfo(freshnessValueLayoutId))
  {
    MessageCounterValue++;
    FvM_FVJaspar_SetMessageCounter(freshnessValueLayoutId, MessageCounterValue);
# if(FVM_TXMESSAGECOUNTERMAXNOTIFICATION == STD_ON)
    if(MessageCounterValue == FvM_GetMessageCounterMaxValueOfFreshnessValueLayoutInfo(freshnessValueLayoutId))
    {
      FvM_GetTxMessageCounterMaxNotification(0)(freshnessValueID);  /* SBSW_FVM_CSL_FCT_PTR */
    }
# endif
  }
  FVM_DUMMY_STATEMENT(freshnessValueID);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}
#endif

#if (FVM_EXISTS_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO == STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_MessageCounterCarryProcessing
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_MessageCounterCarryProcessing(FvM_RxFreshnessValueInfoIterType freshnessValueInfoId)
{
  FvM_FreshnessValueLayoutInfoIterType previousFreshnessValueIdx = FvM_GetFreshnessValueLayoutInfoPreviousIdxOfRxFreshnessValueInfo(freshnessValueInfoId);
  FvM_FreshnessValueLayoutInfoIterType latestFreshnessValueIdx = FvM_GetFreshnessValueLayoutInfoLatestIdxOfRxFreshnessValueInfo(freshnessValueInfoId);
  uint16 previousMessageCounter = FvM_FVJaspar_GetMessageCounter(previousFreshnessValueIdx);
  uint16 latestMessageCounter = FvM_FVJaspar_GetMessageCounter(latestFreshnessValueIdx);
  if(latestMessageCounter <= previousMessageCounter)
  {
    latestMessageCounter = latestMessageCounter + FvM_GetMessageCounterHighBitsIncValueOfRxFreshnessValueInfo(freshnessValueInfoId);
    FvM_FVJaspar_SetMessageCounter(latestFreshnessValueIdx, latestMessageCounter);
  }
}
#endif

#if (FVM_TXFRESHNESSVALUEINFO ==  STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_SetResetFlag
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_SetResetFlag(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId, uint64 ResetCounterValue)
{
  FvM_FreshnessValueArrayIterType destIdx = FvM_GetFreshnessValueArrayResetFlagIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId);
  uint8 copyByte = FvM_FVJaspar_GetResetFlagOfResetCounter(freshnessValueLayoutId, ResetCounterValue);

  copyByte = copyByte << FvM_GetEndPaddingBitSizeOfFreshnessValueLayoutInfo(freshnessValueLayoutId);
  copyByte = (uint8) (copyByte | (FvM_GetFreshnessValueArray(destIdx) & FvM_GetResetFlagUnUsedBitMaskOfFreshnessValueLayoutInfo(freshnessValueLayoutId)));
  FvM_SetFreshnessValueArray(destIdx, copyByte); /* SBSW_FVM_CSL03 */
}
#endif

/**********************************************************************************************************************
  FvM_FVJaspar_GetResetFlagValueMask
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint8, FVM_CODE) FvM_FVJaspar_GetResetFlagValueMask(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId)
{
  FvM_ResetFlagBitSizeOfFreshnessValueLayoutInfoType bitCount = FvM_GetResetFlagBitSizeOfFreshnessValueLayoutInfo(freshnessValueLayoutId);

  uint8 unUsedBits = 8 - bitCount;
  uint8 resetFlagValueMask = 255U >> unUsedBits;

  FVM_DUMMY_STATEMENT(freshnessValueLayoutId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
  return resetFlagValueMask;
}

/**********************************************************************************************************************
  FvM_FVJaspar_GetResetFlagOfResetCounter
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint8, FVM_CODE) FvM_FVJaspar_GetResetFlagOfResetCounter(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId, uint64 ResetCounterValue)
{
  uint8 resetFlag = 0;
  uint8 resetFlagValueMask = FvM_FVJaspar_GetResetFlagValueMask(freshnessValueLayoutId);

  resetFlag = (uint8) (ResetCounterValue & resetFlagValueMask);
  return resetFlag;
}

#if(FVM_EXISTS_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO == STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_GetResetFlag
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint8, FVM_CODE) FvM_FVJaspar_GetResetFlag(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId)
{
  uint8 resetFlag = 0;
  FvM_FreshnessValueArrayIterType srcIdx = FvM_GetFreshnessValueArrayResetFlagIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId);

  resetFlag = FvM_GetFreshnessValueArray(srcIdx) & FvM_GetResetFlagCopyBitMaskOfFreshnessValueLayoutInfo(freshnessValueLayoutId);
  resetFlag = resetFlag >> FvM_GetEndPaddingBitSizeOfFreshnessValueLayoutInfo(freshnessValueLayoutId);

  return resetFlag;
}
#endif

#if(FVM_TXFRESHNESSVALUEINFO == STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_GetLengthInBytes
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(uint32, FVM_CODE) FvM_FVJaspar_GetLengthInBytes(uint32 lengthInBits)
{
  uint32 fullBytes;
  fullBytes = lengthInBits >> 3;
  if((lengthInBits & 7) != 0){
    fullBytes++;
  }
  return fullBytes;
}
#endif

/**********************************************************************************************************************
  FvM_FVJaspar_CopyTruncFreshnessValueToBuffer
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_CopyTruncFreshnessValueToBuffer(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId, P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) truncatedFreshnessValue, uint32 truncatedFreshnessBitLength)
{
  FvM_FreshnessValueArrayIterType destIdx = FvM_GetFreshnessValueArrayTruncatedFreshnessValueStartIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId);

  uint8 bitOffset = FvM_GetEndPaddingBitSizeOfFreshnessValueLayoutInfo(freshnessValueLayoutId);
  uint8 unUsedBits = 8 - bitOffset;
  uint8 srcIdx = 0;
  for(;destIdx < FvM_GetFreshnessValueArrayTruncatedFreshnessValueEndIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId); destIdx++)
  {
    uint8 copyByte = 0;
    if((bitOffset != 0) || ((truncatedFreshnessBitLength & 7) != 0)) /* COV_FVM_TX_ONLY_CONF_TX_tx_xx */
    {
      copyByte = (uint8) (truncatedFreshnessValue[srcIdx] << bitOffset);
      if((destIdx + 1) < FvM_GetFreshnessValueArrayTruncatedFreshnessValueEndIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId))
      {
        copyByte = (uint8) (copyByte | (truncatedFreshnessValue[srcIdx +1] >> unUsedBits));
      }
      else if((truncatedFreshnessBitLength + bitOffset) < 8) /* COV_FVM_TX_ONLY_CONF_XF */
      {
        uint8 startOffset = (uint8) (8 - (truncatedFreshnessBitLength + bitOffset));
        uint8 trucFvMask = (uint8) (255U >> startOffset);
        uint8 fvVerifyMaks = (uint8) (255U << (truncatedFreshnessBitLength + bitOffset));

        copyByte = (uint8) (copyByte & trucFvMask);
        copyByte = (uint8) (copyByte | (FvM_GetFreshnessValueArray(destIdx) & fvVerifyMaks));
      }
      else /* COV_FVM_MISRA */
      { /* PRQA S 3201 */ /* MD_FvM_3201 */
        /* do nothing */
      }
    }
    else
    { /* PRQA S 3201 */ /* MD_FvM_3201 */
      copyByte = truncatedFreshnessValue[srcIdx];
    }
    FvM_SetFreshnessValueArray(destIdx, copyByte); /* SBSW_FVM_CSL03 */
    srcIdx++;
  }
}

#if (FVM_TXFRESHNESSVALUEINFO ==  STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_CopyFreshnessVerifyValueToTruncFreshnessValue
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_CopyFreshnessVerifyValueToTruncFreshnessValue(FvM_FreshnessValueLayoutInfoIterType freshnessValueLayoutId, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) truncatedFreshnessValue, uint32 truncatedFreshnessBitLength)
{

  FvM_FreshnessValueArrayIterType srcIdx = FvM_GetFreshnessValueArrayTruncatedFreshnessValueStartIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId);

  uint8 bitOffset = FvM_GetEndPaddingBitSizeOfFreshnessValueLayoutInfo(freshnessValueLayoutId);
  uint8 unUsedBits = 8 - bitOffset;
  uint32 destIdx = 0;
  uint32 destByteLength = FvM_FVJaspar_GetLengthInBytes(truncatedFreshnessBitLength);

  for(;destIdx < destByteLength; destIdx++)
  {
    uint8 copyByte = 0;
    if((bitOffset != 0) || ((truncatedFreshnessBitLength & 7) != 0)) /* COV_FVM_TRUNC_FRESHNESS_COPY_TX_tx_xx */
    {
      copyByte = (uint8) (FvM_GetFreshnessValueArray(srcIdx) >> bitOffset);
      if((srcIdx + 1) < FvM_GetFreshnessValueArrayTruncatedFreshnessValueEndIdxOfFreshnessValueLayoutInfo(freshnessValueLayoutId))
      {
        copyByte = (uint8) (copyByte | ((uint8) (FvM_GetFreshnessValueArray(srcIdx +1) << unUsedBits)));
      }
      else if((truncatedFreshnessBitLength + bitOffset) < 8) /* COV_FVM_TRUNC_FRESHNESS_COPY_XF */
      {
        uint8 startOffset = (uint8) (8 - (truncatedFreshnessBitLength + bitOffset));
        uint8 trucFvMask = (uint8) (255U >> startOffset);
        uint8 fvVerifyMaks = (uint8) (255U << (truncatedFreshnessBitLength + bitOffset));

        copyByte = (uint8) (copyByte & trucFvMask);
        copyByte = (uint8) (copyByte | (truncatedFreshnessValue[destIdx] & fvVerifyMaks));
      }
      else
      {
        /* do nothing */
      }
    }
    else
    { /* PRQA S 3201 */ /* MD_FvM_3201 */
      copyByte = FvM_GetFreshnessValueArray(srcIdx);
    }
    truncatedFreshnessValue[destIdx] = copyByte; /* SBSW_FVM_PTR_WRITE_ACCESS */
    srcIdx++;
  }

}
#endif

#if (FVM_EXISTS_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO == STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_DoRxFreshnessVerifyValueLatestPreviousCompare
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(Std_ReturnType, FVM_CODE) FvM_FVJaspar_DoRxFreshnessVerifyValueLatestPreviousCompare(FvM_RxFreshnessValueInfoIterType freshnessValueInfoId, uint64 latestResetCounterWithOffset)
{
  Std_ReturnType ret = E_OK;
  FvM_FreshnessValueLayoutInfoIterType previousFreshnessValueIdx = FvM_GetFreshnessValueLayoutInfoPreviousIdxOfRxFreshnessValueInfo(freshnessValueInfoId);
  FvM_FreshnessValueLayoutInfoIterType latestFreshnessValueIdx = FvM_GetFreshnessValueLayoutInfoLatestIdxOfRxFreshnessValueInfo(freshnessValueInfoId);
  uint64 previousReceivedResetCounter = FvM_FVJaspar_GetResetCounter(previousFreshnessValueIdx);
  uint32 previousReceivedTripCounter = FvM_FVJaspar_GetTripCounter(previousFreshnessValueIdx);

  if((previousReceivedTripCounter == FvM_GetLatestTripCounter()) && (previousReceivedResetCounter == latestResetCounterWithOffset))
  {
    FvM_FVJaspar_MessageCounterCarryProcessing((FvM_RxFreshnessValueInfoIterType) freshnessValueInfoId);
  }
  else if((latestResetCounterWithOffset > previousReceivedResetCounter) || (FvM_GetLatestTripCounter() > previousReceivedTripCounter))
  {
    FvM_FVJaspar_SetTripCounter(latestFreshnessValueIdx, FvM_GetLatestTripCounter());
    FvM_FVJaspar_SetResetCounter(latestFreshnessValueIdx, latestResetCounterWithOffset);
    FvM_FVJaspar_SetMessageCounter(latestFreshnessValueIdx, FvM_FVJaspar_GetMessageCounter(latestFreshnessValueIdx) & FvM_GetMessageCounterLowerBitsMaskOfRxFreshnessValueInfo(freshnessValueInfoId));
  }
  else
  {
    ret = E_NOT_OK;
  }
  return ret;
}
#endif

#if (FVM_EXISTS_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO == STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_DoRxFreshnessVerifyValueCalculation
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(Std_ReturnType, FVM_CODE) FvM_FVJaspar_DoRxFreshnessVerifyValueCalculation(FvM_RxFreshnessValueInfoIterType freshnessValueInfoId, uint8 offset, boolean negate)
{
  Std_ReturnType ret = E_OK;
  uint64 latestResetCounterWithOffsetFirstTry;
  uint64 latestResetCounterWithOffsetSecoundTry;

  if(negate == TRUE)
  {
    latestResetCounterWithOffsetFirstTry = (uint64) (FvM_GetLatestResetCounter() - offset);
    latestResetCounterWithOffsetSecoundTry = (uint64) (FvM_GetLatestResetCounter() + offset);
  }
  else
  {
    latestResetCounterWithOffsetFirstTry = (uint64) (FvM_GetLatestResetCounter() + offset);
    latestResetCounterWithOffsetSecoundTry = (uint64) (FvM_GetLatestResetCounter() - offset);
  }

  ret = FvM_FVJaspar_DoRxFreshnessVerifyValueLatestPreviousCompare(freshnessValueInfoId, latestResetCounterWithOffsetFirstTry);

  if(ret != E_OK)
  {
    ret = FvM_FVJaspar_DoRxFreshnessVerifyValueLatestPreviousCompare(freshnessValueInfoId, latestResetCounterWithOffsetSecoundTry);
  }

  return ret;
}
#endif

#if(FVM_EXISTS_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO == STD_ON)
/**********************************************************************************************************************
  FvM_FVJaspar_DoRxResetFlagEvaluation
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(Std_ReturnType, FVM_CODE) FvM_FVJaspar_DoRxResetFlagEvaluation(FvM_RxFreshnessValueInfoIterType freshnessValueInfoId, uint16 SecOCAuthVerifyAttempts)
{
  Std_ReturnType ret = E_NOT_OK;
  FvM_FreshnessValueLayoutInfoIterType latestFreshnessValueIdx = FvM_GetFreshnessValueLayoutInfoLatestIdxOfRxFreshnessValueInfo(freshnessValueInfoId);
  uint8 receivedResetFlag = FvM_FVJaspar_GetResetFlag(latestFreshnessValueIdx);
  uint8 latestResetFlag = FvM_FVJaspar_GetResetFlagOfResetCounter(latestFreshnessValueIdx, (uint64) FvM_GetLatestResetCounter());
  uint8 resetFlagValueMask = FvM_FVJaspar_GetResetFlagValueMask(latestFreshnessValueIdx);

  if(latestResetFlag == receivedResetFlag)
  {
    ret = FvM_FVJaspar_DoRxFreshnessVerifyValueCalculation((FvM_RxFreshnessValueInfoIterType) freshnessValueInfoId, 0, FALSE);
  }
  else if((((uint8) (latestResetFlag - 1)) & resetFlagValueMask) == receivedResetFlag)
  {
    ret = FvM_FVJaspar_DoRxFreshnessVerifyValueCalculation((FvM_RxFreshnessValueInfoIterType) freshnessValueInfoId, 1, TRUE);
  }
  else if(((latestResetFlag + 1) & resetFlagValueMask) == receivedResetFlag)
  {
    ret = FvM_FVJaspar_DoRxFreshnessVerifyValueCalculation((FvM_RxFreshnessValueInfoIterType) freshnessValueInfoId, 1, FALSE);
  }
  else if((((uint8) (latestResetFlag - 2)) & resetFlagValueMask) == receivedResetFlag)  /* COV_FVM_ALWAYS_TRUE_MISRA */
  {
    if(SecOCAuthVerifyAttempts == 0)
    {
      ret = FvM_FVJaspar_DoRxFreshnessVerifyValueCalculation((FvM_RxFreshnessValueInfoIterType) freshnessValueInfoId, 2, TRUE);
    }
    else if(SecOCAuthVerifyAttempts == 1) /* COV_FVM_ALWAYS_TRUE_MISRA */
    {
      ret = FvM_FVJaspar_DoRxFreshnessVerifyValueCalculation((FvM_RxFreshnessValueInfoIterType) freshnessValueInfoId, 2, FALSE);
    }
    else
    {
      /* do nothing */
    }
  }
  else
  {
    /* do nothing */
  }
  return ret;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if ((FVM_EXISTS_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO == STD_ON) && (FVM_STARTUPCYCLECOUNTER == STD_ON))
/**********************************************************************************************************************
  FvM_FVJaspar_SetVerifyOverrideStatus
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(void, FVM_CODE) FvM_FVJaspar_SetVerifyOverrideStatus(uint8 overrideStatus)
{
  FvM_RxFreshnessValueInfoIterType rxFreshnessValueId = 0;
  for(;rxFreshnessValueId < FvM_GetSizeOfRxFreshnessValueInfo(); rxFreshnessValueId++)
  {
# if(FVM_INVALIDHNDOFRXFRESHNESSVALUEINFO == STD_ON)
    if(!FvM_IsInvalidHndOfRxFreshnessValueInfo(rxFreshnessValueId))
# endif
    {
      if(FvM_GetProcessingTypeOfRxFreshnessValueInfo(rxFreshnessValueId) == FVM_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO)
      {
        (void)SecOC_VerifyStatusOverride((uint16) rxFreshnessValueId, overrideStatus, 255);
      }
    }
  }
}
#endif

#if(FVM_RXFRESHNESSVALUEINFO == STD_ON) /* COV_FVM_VAR_SYNC_MSG_ALWAYS_ON */
/**********************************************************************************************************************
  FvM_FVJaspar_CheckSyncMessageValues
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
LOCAL_INLINE FUNC(Std_ReturnType, FVM_CODE) FvM_FVJaspar_CheckSyncMessageValues(FvM_FreshnessValueLayoutInfoIterType latestFreshnessValueIdx)
{
  Std_ReturnType ret = E_NOT_OK;
  uint32 receivedTripCounter = FvM_FVJaspar_GetTripCounter(latestFreshnessValueIdx);
  uint64 receivedResetCounter = FvM_FVJaspar_GetResetCounter(latestFreshnessValueIdx);

  if(receivedTripCounter > FvM_GetLatestTripCounter())
  {
    ret = E_OK;
  }
  else if((receivedTripCounter == FvM_GetLatestTripCounter()) && (receivedResetCounter > FvM_GetLatestResetCounter()))
  {
    ret = E_OK;
  }
  else if((FvM_GetLatestTripCounter() + FvM_GetClearAcceptanceWindow()) >= FvM_GetTripCounterMax())
  {
    if((receivedTripCounter >= 1) && (receivedTripCounter <= (FvM_GetClearAcceptanceWindow() + 1)))
    {
      ret = E_OK;
    }
  }
  else
  {
    /* do nothing */
  }
  return ret;
}
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FvM_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, FVM_CODE) FvM_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  FvM_SetInitialized(FALSE);
}

/**********************************************************************************************************************
  FvM_DeInit
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, FVM_CODE) FvM_DeInit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = FVM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  if(!FvM_IsInitialized())
  {
    errorId = FVM_E_INVALID_REQUEST;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    FvM_SetInitialized(FALSE);
  }

#if(FVM_DEV_ERROR_REPORT == STD_ON)
  if(errorId != FVM_E_NO_ERROR)
  {
    (void)Det_ReportError(FVM_MODULE_ID, FVM_INSTANCE_ID_DET, FVM_SID_DEINIT, errorId);
  }
#endif

  FVM_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}

/**********************************************************************************************************************
 *  FvM_Init()
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
FUNC(void, FVM_CODE) FvM_Init(P2CONST(FvM_ConfigType, AUTOMATIC, FVM_PBCFG) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = FVM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FVM_DEV_ERROR_DETECT == STD_ON) /* COV_FVM_VAR_DEV_ERROR_DETECT */
  /* Check initialization state of the component */
  if (FvM_IsInitialized())
  {
    errorId = FVM_E_ALREADY_INITIALIZED;
  }
  /* Check parameter 'ConfigPtr' */
  else if (ConfigPtr != NULL_PTR)
  {
    errorId = FVM_E_PARAM_CONFIG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    VStdMemClr(FvM_GetAddrFreshnessValueArray(0), FvM_GetSizeOfFreshnessValueArray()); /* SBSW_FVM_MEM_CPY_CLR */
# if (FVM_STARTUPCYCLECOUNTER == STD_ON)
    FvM_SetStartUpCycleCounter(FvM_GetStartUpCycleCnt());
#  if (FVM_EXISTS_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO == STD_ON)
    FvM_FVJaspar_SetVerifyOverrideStatus(SECOC_OVERRIDE_VERIFICATION_STATUS_PASS_N_MESSAGES);
#  endif
    FvM_SetSyncState(FVM_INITIAL_OUT_OF_SYNC_SYNCSTATE);
# else
    FvM_SetSyncState(FVM_INVALID_OUT_OF_SYNC_SYNCSTATE);
# endif

    FvM_SetLatestTripCounter(0);
    FvM_SetLatestResetCounter(0);

    FvM_SetInitialized(TRUE);
  }
  /* ----- Development Error Report --------------------------------------- */
#if (FVM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FVM_E_NO_ERROR)
  {
    (void)Det_ReportError(FVM_MODULE_ID, FVM_INSTANCE_ID_DET, FVM_SID_INIT, errorId);
  }
#else
  FVM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
  FVM_DUMMY_STATEMENT(ConfigPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
}

#if (FVM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  FvM_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, FVM_CODE) FvM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, FVM_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = FVM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (FVM_DEV_ERROR_DETECT == STD_ON) /* COV_FVM_VAR_DEV_ERROR_DETECT */
  if (versioninfo == NULL_PTR)
  {
    errorId = FVM_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID = (FVM_VENDOR_ID);  /* SBSW_FVM_CHECKED_POINTER */
    versioninfo->moduleID = (FVM_MODULE_ID);  /* SBSW_FVM_CHECKED_POINTER */
    versioninfo->sw_major_version = (FVM_SW_MAJOR_VERSION);  /* SBSW_FVM_CHECKED_POINTER */
    versioninfo->sw_minor_version = (FVM_SW_MINOR_VERSION);  /* SBSW_FVM_CHECKED_POINTER */
    versioninfo->sw_patch_version = (FVM_SW_PATCH_VERSION);  /* SBSW_FVM_CHECKED_POINTER */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (FVM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FVM_E_NO_ERROR)
  {
    (void)Det_ReportError(FVM_MODULE_ID, FVM_INSTANCE_ID_DET, FVM_SID_GET_VERSION_INFO, errorId);
  }
# else
  FVM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
}
#endif

#if(FVM_RXFRESHNESSVALUEINFO == STD_ON) /* COV_FVM_VAR_SYNC_MSG_ALWAYS_ON */
/**********************************************************************************************************************
 *  FvM_VerificationStatusCallout()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, FVM_CODE) FvM_VerificationStatusCallout(SecOC_VerificationStatusType verificationStatus)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = FVM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  if(!FvM_IsInitialized())
  {
    errorId = FVM_E_UNINIT;
  }
# if(FVM_DEV_ERROR_DETECT == STD_ON) /* COV_FVM_VAR_DEV_ERROR_DETECT */
  else if (verificationStatus.freshnessValueID >= FvM_GetSizeOfRxFreshnessValueInfo())
  {
    errorId = FVM_E_PARAM;
  }
#  if(FVM_INVALIDHNDOFRXFRESHNESSVALUEINFO == STD_ON)
  else if (FvM_IsInvalidHndOfRxFreshnessValueInfo(verificationStatus.freshnessValueID))
  {
    errorId = FVM_E_PARAM;
  }
#  endif
# endif
  else
  {
    SchM_Enter_FvM_FVM_EXCLUSIVE_AREA_RX_FRESHNESS();
    /* ----- Implementation ----------------------------------------------- */
    if(FvM_GetProcessingTypeOfRxFreshnessValueInfo(verificationStatus.freshnessValueID) == FVM_JASPAR_FRESHNESS_SYNC_MESSAGE_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO) /* COV_FVM_TX_ONLY_CONF_TX */
    {
      if(verificationStatus.verificationStatus == SECOC_VERIFICATIONSUCCESS)
      {
        FvM_FreshnessValueLayoutInfoIterType latestFreshnessValueIdx = FvM_GetFreshnessValueLayoutInfoLatestIdxOfRxFreshnessValueInfo(verificationStatus.freshnessValueID);
        uint32 newTripCounter = FvM_FVJaspar_GetTripCounter(latestFreshnessValueIdx);
        uint64 newResetCounter = FvM_FVJaspar_GetResetCounter(latestFreshnessValueIdx);

        FvM_SetLatestTripCounter(newTripCounter);
        FvM_SetLatestResetCounter(newResetCounter);

# if (FVM_TRIPCOUNTERMAXNOTIFICATION == STD_ON)
        if(newTripCounter == FvM_GetTripCounterMax())
        {
          FvM_GetTripCounterMaxNotification(0)(); /* SBSW_FVM_CSL_FCT_PTR */
        }
# endif
# if(FVM_RESETCOUNTERMAXNOTIFICATION == STD_ON)
        if(newResetCounter == FvM_GetResetCounterMax())
        {
          FvM_GetResetCounterMaxNotification(0)(); /* SBSW_FVM_CSL_FCT_PTR */
        }
# endif

        if(FvM_GetSyncState() != FVM_IN_SYNC_SYNCSTATE)
        {
          FvM_SetSyncState(FVM_IN_SYNC_SYNCSTATE);
        }
      }
    }
# if(FVM_EXISTS_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO == STD_ON)
    else if((FvM_GetSyncState() == FVM_IN_SYNC_SYNCSTATE)
#  if (FVM_STARTUPCYCLECOUNTER == STD_ON)
       && (FvM_GetStartUpCycleCounter() == 0)
#  endif
    )
    {
      if(verificationStatus.verificationStatus == SECOC_VERIFICATIONSUCCESS)
      {
        FvM_FreshnessValueLayoutInfoIterType previousFreshnessValueIdx = FvM_GetFreshnessValueLayoutInfoPreviousIdxOfRxFreshnessValueInfo(verificationStatus.freshnessValueID);
        FvM_FreshnessValueLayoutInfoIterType latestFreshnessValueIdx = FvM_GetFreshnessValueLayoutInfoLatestIdxOfRxFreshnessValueInfo(verificationStatus.freshnessValueID);
        VStdMemCpyRamToRam(FvM_GetAddrFreshnessValueArray(FvM_GetFreshnessValueArrayCompleteFreshnessStartIdxOfFreshnessValueLayoutInfo(previousFreshnessValueIdx)), /* SBSW_FVM_CSL03 */
                           FvM_GetAddrFreshnessValueArray(FvM_GetFreshnessValueArrayCompleteFreshnessStartIdxOfFreshnessValueLayoutInfo(latestFreshnessValueIdx)),
                           FvM_GetFreshnessValueArrayCompleteFreshnessLengthOfFreshnessValueLayoutInfo(previousFreshnessValueIdx));
#  if(FVM_RXMESSAGECOUNTERMAXNOTIFICATION == STD_ON)
        if(FvM_FVJaspar_GetMessageCounter(previousFreshnessValueIdx) == FvM_GetMessageCounterMaxValueOfFreshnessValueLayoutInfo(previousFreshnessValueIdx))
        {
          FvM_GetRxMessageCounterMaxNotification(0)(verificationStatus.freshnessValueID);  /* SBSW_FVM_CSL_FCT_PTR */
        }
#  endif
      }
    }
    else
    {
      /* do nothing */
    }
# endif
    SchM_Exit_FvM_FVM_EXCLUSIVE_AREA_RX_FRESHNESS();
  }
  /* ----- Development Error Report --------------------------------------- */
# if (FVM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FVM_E_NO_ERROR)
  {
    (void)Det_ReportError(FVM_MODULE_ID, FVM_INSTANCE_ID_DET, FVM_SID_VERIFICATION_STATUS_CALLOUT, errorId);
  }
# else
  FVM_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
} /* PRQA S 6050, 6080, 6030 */ /* MD_MSR_STCAL MD_MSR_STMIF MD_MSR_STCYC */
#endif

#if(FVM_TXFRESHNESSVALUEINFO == STD_ON)
/**********************************************************************************************************************
 *  SecOC_GetTxFreshnessTruncData()
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
FUNC(Std_ReturnType, FVM_CODE) SecOC_GetTxFreshnessTruncData(uint16 SecOCFreshnessValueID,  P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValue,  P2VAR(uint32, AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValueLength, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) SecOCTruncatedFreshnessValue,  P2VAR(uint32, AUTOMATIC, SECOC_APPL_DATA) SecOCTruncatedFreshnessValueLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = FVM_E_NO_ERROR;
  Std_ReturnType ret = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  if(!FvM_IsInitialized())
  {
    errorId = FVM_E_UNINIT;
  }
# if(FVM_DEV_ERROR_DETECT == STD_ON) /* COV_FVM_VAR_DEV_ERROR_DETECT */
  else if (SecOCFreshnessValueID >= FvM_GetSizeOfTxFreshnessValueInfo())
  {
    errorId = FVM_E_PARAM;
  }
#  if (FVM_INVALIDHNDOFTXFRESHNESSVALUEINFO == STD_ON)
  else if (FvM_IsInvalidHndOfTxFreshnessValueInfo(SecOCFreshnessValueID))
  {
    errorId = FVM_E_PARAM;
  }
#  endif
  else if (SecOCFreshnessValue == NULL_PTR)
  {
    errorId = FVM_E_PARAM_POINTER;
  }
  else if (SecOCFreshnessValueLength == NULL_PTR)
  {
    errorId = FVM_E_PARAM_POINTER;
  }
  else if (*SecOCFreshnessValueLength != (uint32) (FvM_GetFreshnessValueArrayCompleteFreshnessLengthOfFreshnessValueLayoutInfo(FvM_GetFreshnessValueLayoutInfoLatestIdxOfTxFreshnessValueInfo(SecOCFreshnessValueID)) << 3))
  {
    errorId = FVM_E_PARAM;
  }
  else if (SecOCTruncatedFreshnessValue == NULL_PTR)
  {
    errorId = FVM_E_PARAM_POINTER;
  }
  else if (SecOCTruncatedFreshnessValueLength == NULL_PTR)
  {
    errorId = FVM_E_PARAM_POINTER;
  }
  else if (FvM_FVJaspar_GetLengthInBytes(*SecOCTruncatedFreshnessValueLength) > FvM_GetFreshnessValueArrayTruncatedFreshnessValueLengthOfFreshnessValueLayoutInfo(FvM_GetFreshnessValueLayoutInfoLatestIdxOfTxFreshnessValueInfo(SecOCFreshnessValueID)))
  {
    errorId = FVM_E_PARAM;
  }
# endif
  else
  {
    SchM_Enter_FvM_FVM_EXCLUSIVE_AREA_TX_FRESHNESS();
# if(FVM_TX_USE_DEFAULT_FRESHNESS_VALUE_DURING_START_UP == STD_OFF)
    if(FvM_GetSyncState() == FVM_IN_SYNC_SYNCSTATE)
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      FvM_FreshnessValueLayoutInfoIterType previousFreshnessValueIdx = (FvM_FreshnessValueLayoutInfoIterType) FvM_GetFreshnessValueLayoutInfoPreviousIdxOfTxFreshnessValueInfo(SecOCFreshnessValueID);
      FvM_FreshnessValueLayoutInfoIterType latestFreshnessValueIdx = (FvM_FreshnessValueLayoutInfoIterType) FvM_GetFreshnessValueLayoutInfoLatestIdxOfTxFreshnessValueInfo(SecOCFreshnessValueID);
      uint32 previousTripCounter = FvM_FVJaspar_GetTripCounter(previousFreshnessValueIdx);
      uint64 previousResetCounter = FvM_FVJaspar_GetResetCounter(previousFreshnessValueIdx);

      if((FvM_GetLatestTripCounter() == previousTripCounter) && (FvM_GetLatestResetCounter() == previousResetCounter))
      {
        FvM_FVJaspar_IncMessageCounter(latestFreshnessValueIdx, SecOCFreshnessValueID);
      }
      else
      {
        FvM_FVJaspar_SetTripCounter(latestFreshnessValueIdx, FvM_GetLatestTripCounter());
        FvM_FVJaspar_SetResetCounter(latestFreshnessValueIdx, FvM_GetLatestResetCounter());
        FvM_FVJaspar_SetMessageCounter(latestFreshnessValueIdx, 1);
        FvM_FVJaspar_SetResetFlag(latestFreshnessValueIdx, (uint32) FvM_GetLatestResetCounter());
      }
      VStdMemCpyRamToRam(SecOCFreshnessValue, /* SBSW_SECOC_MEM_CPY_RAM2RAM */
                         FvM_GetAddrFreshnessValueArray(FvM_GetFreshnessValueArrayCompleteFreshnessStartIdxOfFreshnessValueLayoutInfo(latestFreshnessValueIdx)),
                         FvM_GetFreshnessValueArrayCompleteFreshnessLengthOfFreshnessValueLayoutInfo(latestFreshnessValueIdx));

      FvM_FVJaspar_CopyFreshnessVerifyValueToTruncFreshnessValue(latestFreshnessValueIdx, SecOCTruncatedFreshnessValue, *SecOCTruncatedFreshnessValueLength); /* SBSW_FVM_CHECKED_POINTER_1 */

      ret = E_OK;
    }
# if(FVM_TX_USE_DEFAULT_FRESHNESS_VALUE_DURING_START_UP == STD_OFF)
    else
    {
      ret = E_BUSY;
    }
# endif
    SchM_Exit_FvM_FVM_EXCLUSIVE_AREA_TX_FRESHNESS();
  }

  /* ----- Development Error Report --------------------------------------- */
# if(FVM_DEV_ERROR_REPORT == STD_ON)
  if(errorId != FVM_E_NO_ERROR)
  {
    (void)Det_ReportError(FVM_MODULE_ID, FVM_INSTANCE_ID_DET, FVM_SID_GET_TX_FRESHNESS, errorId);
  }
# endif

  FVM_DUMMY_STATEMENT(SecOCFreshnessValueID);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FVM_DUMMY_STATEMENT(SecOCTruncatedFreshnessValueLength);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FVM_DUMMY_STATEMENT(SecOCFreshnessValueLength);             /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FVM_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  return ret;
} /* PRQA S 6030, 6080, 6050 */ /* MD_MSR_STCYC, MD_MSR_STMIF, MD_MSR_STCAL */
#endif

#if(FVM_TXFRESHNESSVALUEINFO == STD_ON)
/**********************************************************************************************************************
 *  SecOC_SPduTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, FVM_CODE) SecOC_SPduTxConfirmation(uint16 SecOCFreshnessValueID)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = FVM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  if(!FvM_IsInitialized())
  {
    errorId = FVM_E_UNINIT;
  }
# if(FVM_DEV_ERROR_DETECT == STD_ON) /* COV_FVM_VAR_DEV_ERROR_DETECT */
  else if (SecOCFreshnessValueID >= FvM_GetSizeOfTxFreshnessValueInfo())
  {
    errorId = FVM_E_PARAM;
  }
#  if (FVM_INVALIDHNDOFTXFRESHNESSVALUEINFO == STD_ON)
  else if (FvM_IsInvalidHndOfTxFreshnessValueInfo(SecOCFreshnessValueID))
  {
    errorId = FVM_E_PARAM;
  }
#  endif
# endif
  else
  {
    SchM_Enter_FvM_FVM_EXCLUSIVE_AREA_TX_FRESHNESS();
    {
      FvM_FreshnessValueLayoutInfoIterType previousFreshnessValueIdx = (FvM_FreshnessValueLayoutInfoIterType) FvM_GetFreshnessValueLayoutInfoPreviousIdxOfTxFreshnessValueInfo(SecOCFreshnessValueID);
      FvM_FreshnessValueLayoutInfoIterType latestFreshnessValueIdx = (FvM_FreshnessValueLayoutInfoIterType) FvM_GetFreshnessValueLayoutInfoLatestIdxOfTxFreshnessValueInfo(SecOCFreshnessValueID);
      VStdMemCpyRamToRam(FvM_GetAddrFreshnessValueArray(FvM_GetFreshnessValueArrayCompleteFreshnessStartIdxOfFreshnessValueLayoutInfo(previousFreshnessValueIdx)), FvM_GetAddrFreshnessValueArray(FvM_GetFreshnessValueArrayCompleteFreshnessStartIdxOfFreshnessValueLayoutInfo(latestFreshnessValueIdx)), FvM_GetFreshnessValueArrayCompleteFreshnessLengthOfFreshnessValueLayoutInfo(previousFreshnessValueIdx)); /* SBSW_FVM_CSL03 */
    }
    SchM_Exit_FvM_FVM_EXCLUSIVE_AREA_TX_FRESHNESS();
  }
  /* ----- Development Error Report --------------------------------------- */
# if(FVM_DEV_ERROR_REPORT == STD_ON)
  if(errorId != FVM_E_NO_ERROR)
  {
    (void)Det_ReportError(FVM_MODULE_ID, FVM_INSTANCE_ID_DET, FVM_SID_TX_FRESHNESS_CONFIRMATION, errorId);
  }
# endif
  FVM_DUMMY_STATEMENT(SecOCFreshnessValueID);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FVM_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}
#endif

#if(FVM_RXFRESHNESSVALUEINFO == STD_ON) /* COV_FVM_VAR_SYNC_MSG_ALWAYS_ON */
/**********************************************************************************************************************
 *  SecOC_GetRxFreshness()
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
FUNC(Std_ReturnType, FVM_CODE) SecOC_GetRxFreshness(uint16 SecOCFreshnessValueID,  P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) SecOCTruncatedFreshnessValue,  uint32 SecOCTruncatedFreshnessValueLength, uint16 SecOCAuthVerifyAttempts, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValue, P2VAR(uint32, AUTOMATIC, SECOC_APPL_DATA) SecOCFreshnessValueLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = FVM_E_NO_ERROR;
  Std_ReturnType ret = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  if(!FvM_IsInitialized())
  {
    errorId = FVM_E_UNINIT;
  }
# if(FVM_DEV_ERROR_DETECT == STD_ON) /* COV_FVM_VAR_DEV_ERROR_DETECT */
  else if (SecOCFreshnessValueID >= FvM_GetSizeOfRxFreshnessValueInfo())
  {
    errorId = FVM_E_PARAM;
  }
#  if(FVM_INVALIDHNDOFRXFRESHNESSVALUEINFO == STD_ON)
  else if (FvM_IsInvalidHndOfRxFreshnessValueInfo(SecOCFreshnessValueID))
  {
    errorId = FVM_E_PARAM;
  }
#  endif
  else if (SecOCTruncatedFreshnessValue == NULL_PTR)
  {
    errorId = FVM_E_PARAM_POINTER;
  }
  else if (SecOCFreshnessValue == NULL_PTR)
  {
    errorId = FVM_E_PARAM_POINTER;
  }
  else if (SecOCFreshnessValueLength == NULL_PTR)
  {
    errorId = FVM_E_PARAM_POINTER;
  }
  else if (*SecOCFreshnessValueLength != (uint32) (FvM_GetFreshnessValueArrayCompleteFreshnessLengthOfFreshnessValueLayoutInfo(FvM_GetFreshnessValueLayoutInfoLatestIdxOfRxFreshnessValueInfo(SecOCFreshnessValueID)) << 3))
  {
    errorId = FVM_E_PARAM;
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    SchM_Enter_FvM_FVM_EXCLUSIVE_AREA_RX_FRESHNESS();
    if(FvM_GetProcessingTypeOfRxFreshnessValueInfo(SecOCFreshnessValueID) == FVM_JASPAR_FRESHNESS_SYNC_MESSAGE_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO) /* COV_FVM_TX_ONLY_CONF_TX */
    {
      FvM_FreshnessValueLayoutInfoIterType latestFreshnessValueIdx = FvM_GetFreshnessValueLayoutInfoLatestIdxOfRxFreshnessValueInfo(SecOCFreshnessValueID);
      FvM_FVJaspar_CopyTruncFreshnessValueToBuffer(latestFreshnessValueIdx, SecOCTruncatedFreshnessValue, SecOCTruncatedFreshnessValueLength); /* SBSW_FVM_CONST_POINTER */

      ret = FvM_FVJaspar_CheckSyncMessageValues(latestFreshnessValueIdx);

      if(ret == E_OK)
      {
        VStdMemCpyRamToRam(SecOCFreshnessValue, /* SBSW_SECOC_MEM_CPY_RAM2RAM */
                           FvM_GetAddrFreshnessValueArray(FvM_GetFreshnessValueArrayCompleteFreshnessStartIdxOfFreshnessValueLayoutInfo(latestFreshnessValueIdx)),
                           FvM_GetFreshnessValueArrayCompleteFreshnessLengthOfFreshnessValueLayoutInfo(latestFreshnessValueIdx));
      }
    }
# if(FVM_EXISTS_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO == STD_ON)
    else if(FvM_GetSyncState() == FVM_IN_SYNC_SYNCSTATE)
    {
      FvM_FreshnessValueLayoutInfoIterType previousFreshnessValueIdx = FvM_GetFreshnessValueLayoutInfoPreviousIdxOfRxFreshnessValueInfo(SecOCFreshnessValueID);
      FvM_FreshnessValueLayoutInfoIterType latestFreshnessValueIdx = FvM_GetFreshnessValueLayoutInfoLatestIdxOfRxFreshnessValueInfo(SecOCFreshnessValueID);

      VStdMemCpyRamToRam(FvM_GetAddrFreshnessValueArray(FvM_GetFreshnessValueArrayCompleteFreshnessStartIdxOfFreshnessValueLayoutInfo(latestFreshnessValueIdx)), /* SBSW_FVM_CSL03 */
                         FvM_GetAddrFreshnessValueArray(FvM_GetFreshnessValueArrayCompleteFreshnessStartIdxOfFreshnessValueLayoutInfo(previousFreshnessValueIdx)),
                         FvM_GetFreshnessValueArrayCompleteFreshnessLengthOfFreshnessValueLayoutInfo(latestFreshnessValueIdx));
      FvM_FVJaspar_CopyTruncFreshnessValueToBuffer(latestFreshnessValueIdx, SecOCTruncatedFreshnessValue, SecOCTruncatedFreshnessValueLength); /* SBSW_FVM_CONST_POINTER */

      ret = FvM_FVJaspar_DoRxResetFlagEvaluation((FvM_RxFreshnessValueInfoIterType) SecOCFreshnessValueID, SecOCAuthVerifyAttempts);

      if(ret == E_OK) /* COV_FVM_2BIT_RESET_FLAG */
      {
        VStdMemCpyRamToRam(SecOCFreshnessValue, /* SBSW_SECOC_MEM_CPY_RAM2RAM */
                           FvM_GetAddrFreshnessValueArray(FvM_GetFreshnessValueArrayCompleteFreshnessStartIdxOfFreshnessValueLayoutInfo(latestFreshnessValueIdx)),
                           FvM_GetFreshnessValueArrayCompleteFreshnessLengthOfFreshnessValueLayoutInfo(latestFreshnessValueIdx));
      }
    }
#  if (FVM_STARTUPCYCLECOUNTER == STD_ON)
    else if(FvM_GetSyncState() == FVM_INITIAL_OUT_OF_SYNC_SYNCSTATE)
    {
      ret = E_OK;
    }
#  endif
    else
    {
      ret = E_BUSY;
    }
# endif
    SchM_Exit_FvM_FVM_EXCLUSIVE_AREA_RX_FRESHNESS();
  }

  /* ----- Development Error Report --------------------------------------- */
# if(FVM_DEV_ERROR_REPORT == STD_ON)
  if(errorId != FVM_E_NO_ERROR)
  {
    (void)Det_ReportError(FVM_MODULE_ID, FVM_INSTANCE_ID_DET, FVM_SID_GET_RX_FRESHNESS, errorId);
  }
# endif

  FVM_DUMMY_STATEMENT(errorId);                         /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  FVM_DUMMY_STATEMENT(SecOCFreshnessValueLength);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  return ret;
} /* PRQA S 6030, 6080, 6050, 6060 */ /* MD_MSR_STCYC, MD_MSR_STMIF, MD_MSR_STCAL, MD_MSR_STPAR */
#endif

#if (FVM_MAINFUNCTION_API == STD_ON)
/**********************************************************************************************************************
  FvM_MainFunction
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, FVM_CODE) FvM_MainFunction(void)
{
  if(FvM_IsInitialized())
  {
# if (FVM_STARTUPCYCLECOUNTER == STD_ON) /* COV_FVM_VAR_DEPENDENT_SWITCH */
    if(FvM_GetStartUpCycleCounter() != 0)
    {
      FvM_DecStartUpCycleCounter();
      if(FvM_GetStartUpCycleCounter() == 0)
      {
#  if (FVM_EXISTS_JASPAR_FRESHNESS_PROCESSINGTYPEOFRXFRESHNESSVALUEINFO == STD_ON)
        FvM_FVJaspar_SetVerifyOverrideStatus(SECOC_OVERRIDE_VERIFICATION_STATUS_CANCEL);
#  endif
        if(FvM_GetSyncState() == FVM_INITIAL_OUT_OF_SYNC_SYNCSTATE)
        {
          FvM_SetSyncState(FVM_INVALID_OUT_OF_SYNC_SYNCSTATE);
        }
      }
    }
# endif
  }
}
#endif

#define FVM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:
 MD_FvM_3201: rule 14.1
      Reason:     Due to the configuration variant this code section is unreachable.
      Risk:       none.
      Prevention: Program flow has been verified by component tests and review.

 MD_FvM_0291: rule 21.1
      Reason:     Due to the subtraction the result might be negative, but due to configuration condition this can never happen.
      Risk:       none.
      Prevention: Many configuration variants are tested.

*/
/* SBSW_JUSTIFICATION_BEGIN

  \ID SBSW_FVM_CSL03
    \DESCRIPTION Access to Table1 via indirection over Table2.
    \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

  \ID SBSW_FVM_CONST_POINTER
    \DESCRIPTION Pointer is handed over to a function
    \COUNTERMEASURE \N The pointer references a constant memory location (P2CONST). Therefore it can not be written.

  \ID SBSW_FVM_CHECKED_POINTER
    \DESCRIPTION Pointer write access
    \COUNTERMEASURE \R It is checked at runtime that the pointer is no NULL_PTR. The caller has to ensure that the pointer references a valid memory location.

  \ID SBSW_FVM_CHECKED_POINTER_1
    \DESCRIPTION Pointer is handed over to a function
    \COUNTERMEASURE \R It is checked at runtime that the pointer is no NULL_PTR. The caller has to ensure that the pointer references a valid memory location.
                       The length of the buffer is checked for sufficient length.

  \ID SBSW_FVM_MEM_CPY_CLR
    \DESCRIPTION A Pointer is handed to a function to clear the corresponding memory.
    \COUNTERMEASURE \N The address is read via a ComStackLib macro and is valid. The corresponding size is also read via the corresponding ComStackLib macro and ensures that only the correct length is cleared.

  \ID SBSW_FVM_CSL_FCT_PTR
    \DESCRIPTION A Function Pointer is called.
    \COUNTERMEASURE \N The function pointer is read using a ComStackLib macro and is therefore valid. Only one function pointer is written to the corresponding table, therefore it can always be read using index 0.

  \ID SBSW_SECOC_MEM_CPY_RAM2RAM
    \DESCRIPTION Call to copy RAM data to RAM
    \COUNTERMEASURE \R The passed pointers are either read by ComStackLib macros or are passed through and checked at the beginning of the function. The caller has to ensure that it references a valid memory location.
                    \R The size of the copy operation is checked against the freshness value length provided by the caller.

  \ID SBSW_FVM_PTR_WRITE_ACCESS
    \DESCRIPTION A pointer is written
    \COUNTERMEASURE \R The passed pointer is passed through and checked in the calling function. The caller has to ensure that it references a valid memory location.
                    \R The size of the pointer is checked in the calling function.

   SBSW_JUSTIFICATION_END */
/* START_COVERAGE_JUSTIFICATION

  \ID COV_FVM_VAR_DEV_ERROR_DETECT
    \ACCEPT TX
    \REASON Det error detection must always be enabled for SafeBsw.

  \ID COV_FVM_VAR_SYNC_MSG_ALWAYS_ON
    \ACCEPT TX
    \REASON This switch is always on. A Rx sync message does always exist in the FvM.

  \ID COV_FVM_VAR_DEPENDENT_SWITCH
    \ACCEPT TX
    \REASON The nested switch depends on a previous switch.

  \ID COV_FVM_RESET_COUNTER_BIT_COUNT
    \ACCEPT TX
    \REASON Due to the configuration this condition evaluates always to true. The Reset Counter bit size is fixed for one configuration.

  \ID COV_FVM_TRIP_COUNTER_BIT_COUNT
    \ACCEPT TX
    \ACCEPT XX
    \ACCEPT XF
    \REASON Due to the configuration this condition evaluates always to true. The Trip Counter bit size is fixed for one configuration.

  \ID COV_FVM_MESSAGE_COUNTER_BIT_COUNT
    \ACCEPT TX
    \REASON Due to the configuration this condition evaluates always to true.

  \ID COV_FVM_MISRA
    \ACCEPT X
    \ACCEPT XF
    \REASON [MSR_COV_MISRA]

   \ID COV_FVM_ALWAYS_TRUE_MISRA
    \ACCEPT TX
    \REASON [MSR_COV_MISRA]

   \ID COV_FVM_2BIT_RESET_FLAG
    \ACCEPT TX
    \REASON In configurations with 2 bit reset flag the evaluation will always be successful.

   \ID COV_FVM_TX_ONLY_CONF_TX_tx_xx
    \ACCEPT TX tx xx
    \REASON In Tx Only configurations the Sync message is the only Rx message and has fix layout.

   \ID COV_FVM_TX_ONLY_CONF_XF
    \ACCEPT XF
    \REASON In Tx Only configurations the Sync message is the only Rx message and has fix layout.

   \ID COV_FVM_TX_ONLY_CONF_TX
    \ACCEPT TX
    \REASON In Tx Only configurations the Sync message is the only Rx message and has fix layout.

   \ID COV_FVM_TRUNC_FRESHNESS_COPY_TX_tx_xx
    \ACCEPT TX tx xx
    \REASON Due to the configuration the offset will always be true.

   \ID COV_FVM_TRUNC_FRESHNESS_COPY_XF
    \ACCEPT XF
    \REASON Due to the configuration the sum of truncated Freshness length and offset is never smaller than 8.

END_COVERAGE_JUSTIFICATION
*/

/**********************************************************************************************************************
 *  END OF FILE: FvM.c
 *********************************************************************************************************************/
