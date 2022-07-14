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
/**        \file  EthTSyn_Crc_Int.h
 *        \brief  EthTSyn internal header file for Crc handling
 *
 *      \details  Contains all macros and function declarations used by the EthTSyn to handle Crc calculation and
 *                validation
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined (ETHTSYN_CRC_INT_H)
# define ETHTSYN_CRC_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types.h"
# include "EthTSyn_CfgAccess_Int.h"
# include "EthTSyn_Int.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#  if !defined (ETHTSYN_INLINE)
#  define ETHTSYN_INLINE static INLINE
#  endif

#  define ETHTSYN_CRC_TX_SECURED_NOT_SUPPORTED       (0u)
#  define ETHTSYN_CRC_TX_SECURED_SUPPORTED           (1u)

#  define ETHTSYN_CRC_RX_CRC_IGNORED                 (0u)
#  define ETHTSYN_CRC_RX_CRC_NOT_VALIDATED           (1u)
#  define ETHTSYN_CRC_RX_CRC_OPTIONAL                (2u)
#  define ETHTSYN_CRC_RX_CRC_VALIDATED               (3u)

#  define ETHTSYN_CRC_FLAGS_MSG_LENGTH_MASK          (0x01u)
#  define ETHTSYN_CRC_FLAGS_MSG_LENGTH_SHIFT         (0u)

#  define ETHTSYN_CRC_FLAGS_DOMAIN_NUMBER_MASK       (0x02u)
#  define ETHTSYN_CRC_FLAGS_DOMAIN_NUMBER_SHIFT      (1u)

#  define ETHTSYN_CRC_FLAGS_CORRECTION_FIELD_MASK    (0x04u)
#  define ETHTSYN_CRC_FLAGS_CORRECTION_FIELD_SHIFT   (2u)

#  define ETHTSYN_CRC_FLAGS_SRC_PORT_IDENTITY_MASK   (0x08u)
#  define ETHTSYN_CRC_FLAGS_SRC_PORT_IDENTITY_SHIFT  (3u)

#  define ETHTSYN_CRC_FLAGS_SEQUENCE_ID_MASK         (0x10u)
#  define ETHTSYN_CRC_FLAGS_SEQUENCE_ID_SHIFT        (4u)

#  define ETHTSYN_CRC_FLAGS_PRECISE_ORIGIN_TS_MASK   (0x20u)
#  define ETHTSYN_CRC_FLAGS_PRECISE_ORIGIN_TS_SHIFT  (5u)

#  define ETHTSYN_CRC_STATUS_CRC_DATA_SIZE           (2u)
#  define ETHTSYN_CRC_USER_DATA_CRC_DATA_SIZE        (5u)
#  define ETHTSYN_CRC_OFS_CRC_DATA_SIZE              (17u)

/* CRC_TIME_0 Max Size: TimeFlages (1) + DomainNumber (1) + SrcPortIdentity (10) + PreciseOriginTimestamp (10) +
 *                      DataId (1) = 22 */
#  define ETHTSYN_CRC_TIME_0_MAX_DATA_SIZE    (23u)
/* CRC_TIME_1 Max Size: TimeFlages (1) + MsgLength (2) + CorrectionField (8) + SequenceId (2) + DataId (1) = 13*/
#  define ETHTSYN_CRC_TIME_1_MAX_DATA_SIZE    (14u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FCT_MACRO */ /* MD_MSR_19.7 */
#  define ETHTSYN_CRC_FLAGS_GET_MSG_LENGTH_FLAG(Flags) \
  ((uint8)(((Flags) & ETHTSYN_CRC_FLAGS_MSG_LENGTH_MASK) >> ETHTSYN_CRC_FLAGS_MSG_LENGTH_SHIFT))

#  define ETHTSYN_CRC_FLAGS_GET_DOMAIN_NUMBER_FLAG(Flags) \
  ((uint8)(((Flags) & ETHTSYN_CRC_FLAGS_DOMAIN_NUMBER_MASK) >> ETHTSYN_CRC_FLAGS_DOMAIN_NUMBER_SHIFT))

#  define ETHTSYN_CRC_FLAGS_GET_CORRECTION_FIELD_FLAG(Flags) \
  ((uint8)(((Flags) & ETHTSYN_CRC_FLAGS_CORRECTION_FIELD_MASK) >> ETHTSYN_CRC_FLAGS_CORRECTION_FIELD_SHIFT))

#  define ETHTSYN_CRC_FLAGS_GET_SRC_PORT_IDENTITY_FLAG(Flags) \
  ((uint8)(((Flags) & ETHTSYN_CRC_FLAGS_SRC_PORT_IDENTITY_MASK) >> ETHTSYN_CRC_FLAGS_SRC_PORT_IDENTITY_SHIFT))

#  define ETHTSYN_CRC_FLAGS_GET_SEQUENCE_ID_FLAG(Flags) \
  ((uint8)(((Flags) & ETHTSYN_CRC_FLAGS_SEQUENCE_ID_MASK) >> ETHTSYN_CRC_FLAGS_SEQUENCE_ID_SHIFT))

#  define ETHTSYN_CRC_FLAGS_GET_PRECISE_ORIGIN_TS_FLAG(Flags) \
  ((uint8)(((Flags) & ETHTSYN_CRC_FLAGS_PRECISE_ORIGIN_TS_MASK) >> ETHTSYN_CRC_FLAGS_PRECISE_ORIGIN_TS_SHIFT))

/* PRQA L: FCT_MACRO */
/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#  define ETHTSYN_START_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EthTSyn_Crc_ComputeTime0Crc
 *********************************************************************************************************************/
/*! \brief      Computes the uint8 Crc (CRC_TIME_0) for a TimeSecured FollowUp message which should be transmitted
 *  \param[in]  DataId          The DataId for the FollowUp
 *  \param[in]  Flags           The Flags determining which fields to use for the Crc calculation
 *  \param[in]  ComMsgPtr       Pointer to the common message header
 *  \return     The computed Crc (with the use Crc_CalculateCRC8H2F).
 *  \pre        For Tx: All Sub-Tlv information except the Crc itself are already copied to the Tx-Buffer
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Crc_ComputeTime0Crc(
                  uint8                      DataId,
                  uint8                      Flags,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType)  ComMsgPtr);

/**********************************************************************************************************************
 *  EthTSyn_Crc_ComputeTime1Crc
 *********************************************************************************************************************/
/*! \brief      Computes the uint8 Crc (CRC_TIME_1) for a TimeSecured FollowUp message which should be transmitted
 *  \param[in]  DataId          The DataId for the FollowUp
 *  \param[in]  Flags           The Flags determining which fields to use for the Crc calculation
 *  \param[in]  ComMsgPtr       Pointer to the common message header
 *  \return     The computed Crc (with the use Crc_CalculateCRC8H2F).
 *  \pre        For Tx: All Sub-Tlv information except the Crc itself are already copied to the Tx-Buffer
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Crc_ComputeTime1Crc(
                  uint8                      DataId,
                  uint8                      Flags,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType)  ComMsgPtr);

/**********************************************************************************************************************
 *  EthTSyn_Crc_ComputeStatusCrc
 *********************************************************************************************************************/
/*! \brief      Computes the uint8 Crc for a Status Secured FollowUp message which should be transmitted
 *  \param[in]  DataId               The DataId for the FollowUp
 *  \param[in]  StatusSecuredTlvPtr  Pointer to the Statues Secured Sub-Tlv of the FollowUp message
 *  \return     The computed Crc (CRC_Status) (with the use Crc_CalculateCRC8H2F).
 *  \pre        For Tx: All Sub-Tlv information except the Crc itself are already copied to the Tx-Buffer
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Crc_ComputeStatusCrc(
                  uint8                        DataId,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvStatusType)  StatusSecuredTlvPtr);

/**********************************************************************************************************************
 *  EthTSyn_Crc_ComputeUserDataCrc
 *********************************************************************************************************************/
/*! \brief      Computes the uint8 Crc for a UserData Secured FollowUp message which should be transmitted
 *  \param[in]  DataId                 The DataId for the FollowUp
 *  \param[in]  UserDataSecuredTlvPtr  Pointer to the UserData Secured Sub-Tlv of the FollowUp message
 *  \return     The computed Crc (CRC_UserData) (with the use Crc_CalculateCRC8H2F).
 *  \pre        For Tx: All Sub-Tlv information except the Crc itself are already copied to the Tx-Buffer
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Crc_ComputeUserDataCrc(
                  uint8                          DataId,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvUserDataType)  UserDataSecuredTlvPtr);

/**********************************************************************************************************************
 *  EthTSyn_Crc_ComputeOfsCrc
 *********************************************************************************************************************/
/*! \brief      Computes the uint8 Crc for a Ofs Secured FollowUp message which should be transmitted
 *  \param[in]  DataId            The DataId for the FollowUp
 *  \param[in]  OfsSecuredTlvPtr  Pointer to the Ofs Secured Sub-Tlv of the FollowUp message
 *  \return     The computed Crc (CRC_OFS) (with the use Crc_CalculateCRC8H2F).
 *  \pre        For Tx: All Sub-Tlv information except the Crc itself are already copied to the Tx-Buffer
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Crc_ComputeOfsCrc(
                  uint8                     DataId,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvOfsType)  OfsSecuredTlvPtr);

/**********************************************************************************************************************
 *  EthTSyn_Crc_ValidateSubTlvCrc
 *********************************************************************************************************************/
/*! \brief      Validates the Crc for the given Ar Sub-Tlv of the FollowUp
 *  \param[in]  DataId       The DataId for the FollowUp
 *  \param[in]  ComMsgPtr    Pointer to the common message header
 *  \param[in]  ArSubTlvPtr  Pointer to the Ar Sub-Tlv of the FollowUp
 *  \return     TRUE - Crc is valid or Sub-Tlv without Crc was passed
 *  \return     False - Crc is invalid
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Crc_ValidateSubTlvCrc(
                  uint8                               DataId,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType)           ComMsgPtr,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType) ArSubTlvPtr);

/**********************************************************************************************************************
 *  EthTSyn_Crc_ValidateTimeCrc
 *********************************************************************************************************************/
/*! \brief      Validates the Crc (CRC_TIME_0 + CRC_TIME_1) for a TimeSecured FollowUp
 *  \param[in]  DataId             The DataId for the FollowUp
 *  \param[in]  ComMsgPtr          Pointer to the common message header
 *  \param[in]  TimeSecuredTlvPtr  Pointer to the Time Secured Sub-Tlv of the FollowUp
 *  \return     TRUE - Crc is valid
 *  \return     False - Crc is invalid
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Crc_ValidateTimeCrc(
                  uint8                            DataId,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType)        ComMsgPtr,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvTimeSecuredType) TimeSecuredTlvPtr);

/**********************************************************************************************************************
 *  EthTSyn_Crc_ValidateStatusCrc
 *********************************************************************************************************************/
/*! \brief      Validates the Crc (CRC_Status) for a Status Secured FollowUp message
 *  \param[in]  DataId               The DataId for the FollowUp
 *  \param[in]  StatusSecuredTlvPtr  Pointer to the Statues Secured Sub-Tlv of the FollowUp message
 *  \return     TRUE - Crc is valid
 *  \return     False - Crc is invalid
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Crc_ValidateStatusCrc(
                  uint8                        DataId,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvStatusType)  StatusSecuredTlvPtr);

/**********************************************************************************************************************
 *  EthTSyn_Crc_ValidateUserDataCrc
 *********************************************************************************************************************/
/*! \brief      Validates the Crc (CRC_UserData) for a UserData Secured FollowUp message
 *  \param[in]  DataId                 The DataId for the FollowUp
 *  \param[in]  UserDataSecuredTlvPtr  Pointer to the UserData Secured Sub-Tlv of the FollowUp message
 *  \return     TRUE - Crc is valid
 *  \return     False - Crc is invalid
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Crc_ValidateUserDataCrc(
                  uint8                          DataId,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvUserDataType)  UserDataSecuredTlvPtr);

/**********************************************************************************************************************
 *  EthTSyn_Crc_ValidateOfsCrc
 *********************************************************************************************************************/
/*! \brief      Validates the Crc (CRC_OFS) for a Ofs Secured FollowUp message
 *  \param[in]  DataId            The DataId for the FollowUp
 *  \param[in]  OfsSecuredTlvPtr  Pointer to the Ofs Secured Sub-Tlv of the FollowUp message
 *  \return     TRUE - Crc is valid
 *  \return     False - Crc is invalid
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Crc_ValidateOfsCrc(
                  uint8                     DataId,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvOfsType)  OfsSecuredTlvPtr);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#  ifndef ETHTSYN_UNIT_TEST /* COV_ETHTSYN_TESTSUITE_INSTRUMENTATION */
/**********************************************************************************************************************
 *  EthTSyn_Crc_ComputeTime0Crc
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
 */
ETHTSYN_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Crc_ComputeTime0Crc(uint8 DataId, uint8 Flags,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 crcData[ETHTSYN_CRC_TIME_0_MAX_DATA_SIZE];
  uint32 crcDataLength = 0;
  ETHTSYN_P2CONST(EthTSyn_FollowUpMsgType) followUpMsgPtr;
  uint8 crcValue = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Crc is enabled. */
  if(EthTSyn_CfgAccess_IsCrcEnabled() == TRUE)
  {
    /* #20 Crc is enabled. Calculate the Crc. */

    /* #30 Add the Value of Flags to Crc */
    crcData[crcDataLength] = Flags;
    crcDataLength++;
    /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
    followUpMsgPtr = (ETHTSYN_P2CONST(EthTSyn_FollowUpMsgType))
        &((ETHTSYN_P2CONST(uint8))ComMsgPtr)[ETHTSYN_HDR_COM_LENGTH];

    /* #40 Add Domain Number to Crc if applicable */
    if(ETHTSYN_CRC_FLAGS_GET_DOMAIN_NUMBER_FLAG(Flags) == 0x01u)
    {
      crcData[crcDataLength] = ComMsgPtr->DomainNumber;
      crcDataLength++;
    }

    /* #50 Add Source Port Identity to Crc if applicable */
    if(ETHTSYN_CRC_FLAGS_GET_SRC_PORT_IDENTITY_FLAG(Flags) == 0x01u)
    {
      crcData[crcDataLength] = ComMsgPtr->SourceClockIdentity[0];
      crcDataLength++;
      crcData[crcDataLength] = ComMsgPtr->SourceClockIdentity[1];
      crcDataLength++;
      crcData[crcDataLength] = ComMsgPtr->SourceClockIdentity[2];
      crcDataLength++;
      crcData[crcDataLength] = ComMsgPtr->SourceClockIdentity[3];
      crcDataLength++;
      crcData[crcDataLength] = ComMsgPtr->SourceClockIdentity[4];
      crcDataLength++;
      crcData[crcDataLength] = ComMsgPtr->SourceClockIdentity[5];
      crcDataLength++;
      crcData[crcDataLength] = ComMsgPtr->SourceClockIdentity[6];
      crcDataLength++;
      crcData[crcDataLength] = ComMsgPtr->SourceClockIdentity[7];
      crcDataLength++;

      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      crcData[crcDataLength] = ((ETHTSYN_P2CONST(uint8))&ComMsgPtr->SourcePortNumber)[0];
      crcDataLength++;
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      crcData[crcDataLength] = ((ETHTSYN_P2CONST(uint8))&ComMsgPtr->SourcePortNumber)[1];
      crcDataLength++;
    }

    /* #60 Add Precise Origin Timestamp to Crc if applicable */
    if(ETHTSYN_CRC_FLAGS_GET_PRECISE_ORIGIN_TS_FLAG(Flags) == 0x01u)
    {
      crcData[crcDataLength] = followUpMsgPtr->PreciseOriginTimestampSeconds[0];
      crcDataLength++;
      crcData[crcDataLength] = followUpMsgPtr->PreciseOriginTimestampSeconds[1];
      crcDataLength++;
      crcData[crcDataLength] = followUpMsgPtr->PreciseOriginTimestampSeconds[2];
      crcDataLength++;
      crcData[crcDataLength] = followUpMsgPtr->PreciseOriginTimestampSeconds[3];
      crcDataLength++;
      crcData[crcDataLength] = followUpMsgPtr->PreciseOriginTimestampSeconds[4];
      crcDataLength++;
      crcData[crcDataLength] = followUpMsgPtr->PreciseOriginTimestampSeconds[5];
      crcDataLength++;

      crcData[crcDataLength] = followUpMsgPtr->PreciseOriginTimestampNanoSeconds[0];
      crcDataLength++;
      crcData[crcDataLength] = followUpMsgPtr->PreciseOriginTimestampNanoSeconds[1];
      crcDataLength++;
      crcData[crcDataLength] = followUpMsgPtr->PreciseOriginTimestampNanoSeconds[2];
      crcDataLength++;
      crcData[crcDataLength] = followUpMsgPtr->PreciseOriginTimestampNanoSeconds[3];
      crcDataLength++;
    }

    /* #70 Add DataId to Crc */
    crcData[crcDataLength] = DataId;
    crcDataLength++;

    /* #80 Calculate the Crc. */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    crcValue = EthTSyn_Crc_CalculateCrc8H2FFctPtr(crcData, crcDataLength, 0u, TRUE);
  }

  return crcValue;

} /* EthTSyn_Crc_ComputeTime0Crc() */

/**********************************************************************************************************************
 *  EthTSyn_Crc_ComputeTime1Crc
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
 */
ETHTSYN_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Crc_ComputeTime1Crc(uint8  DataId, uint8 Flags,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType)  ComMsgPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 crcData[ETHTSYN_CRC_TIME_1_MAX_DATA_SIZE];
  uint32 crcDataLength = 0;
  uint8 crcValue = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Crc is enabled. */
  if(EthTSyn_CfgAccess_IsCrcEnabled() == TRUE)
  {
    /* #20 Crc is enabled. Calculate the Crc. */

    /* #30 Add the Value of Flags to Crc. */
    crcData[crcDataLength] = Flags;
    crcDataLength++;

    /* #40 Add Message Length to Crc if applicable. */
    if(ETHTSYN_CRC_FLAGS_GET_MSG_LENGTH_FLAG(Flags) == 0x01u)
    {
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      crcData[crcDataLength] = ((ETHTSYN_P2CONST(uint8))&ComMsgPtr->MessageLength)[0];
      crcDataLength++;
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      crcData[crcDataLength] = ((ETHTSYN_P2CONST(uint8))&ComMsgPtr->MessageLength)[1];
      crcDataLength++;
    }

    /* #50 Add Correction field to Crc if applicable. */
    if(ETHTSYN_CRC_FLAGS_GET_CORRECTION_FIELD_FLAG(Flags) == 0x01u)
    {
      crcData[crcDataLength] = ComMsgPtr->CorrectionField[0];
      crcDataLength++;
      crcData[crcDataLength] = ComMsgPtr->CorrectionField[1];
      crcDataLength++;
      crcData[crcDataLength] = ComMsgPtr->CorrectionField[2];
      crcDataLength++;
      crcData[crcDataLength] = ComMsgPtr->CorrectionField[3];
      crcDataLength++;
      crcData[crcDataLength] = ComMsgPtr->CorrectionField[4];
      crcDataLength++;
      crcData[crcDataLength] = ComMsgPtr->CorrectionField[5];
      crcDataLength++;
      crcData[crcDataLength] = ComMsgPtr->CorrectionField[6];
      crcDataLength++;
      crcData[crcDataLength] = ComMsgPtr->CorrectionField[7];
      crcDataLength++;
    }

    /* #60 Add Sequence Id to Crc if applicable. */
    if(ETHTSYN_CRC_FLAGS_GET_SEQUENCE_ID_FLAG(Flags) == 0x01u)
    {
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      crcData[crcDataLength] = ((ETHTSYN_P2CONST(uint8))&ComMsgPtr->SequenceId)[0];
      crcDataLength++;
      /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
      crcData[crcDataLength] = ((ETHTSYN_P2CONST(uint8))&ComMsgPtr->SequenceId)[1];
      crcDataLength++;
    }

    /* #70 Add DataId to Crc. */
    crcData[crcDataLength] = DataId;
    crcDataLength++;

    /* #80 Calculate the Crc. */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    crcValue = EthTSyn_Crc_CalculateCrc8H2FFctPtr(crcData, crcDataLength, 0, TRUE);
  }

  return crcValue;
} /* EthTSyn_Crc_ComputeTime1Crc() */

/**********************************************************************************************************************
 *  EthTSyn_Crc_ComputeStatusCrc
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
 */
ETHTSYN_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Crc_ComputeStatusCrc(uint8 DataId,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvStatusType) StatusSecuredTlvPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 crcData[ETHTSYN_CRC_STATUS_CRC_DATA_SIZE];
  uint8 crcDataLength = 0;
  uint8 crcValue = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Crc is enabled. */
  if(EthTSyn_CfgAccess_IsCrcEnabled() == TRUE)
  {
    /* #20 Crc is enabled. Calculate the Crc. */

    /* #30 Add Status to Crc. */
    crcData[crcDataLength] = StatusSecuredTlvPtr->Status;
    crcDataLength++;

    /* #40 Add DataId to Crc. */
    crcData[crcDataLength] = DataId;
    crcDataLength++;

    /* #50 Calculate the Crc. */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    crcValue = EthTSyn_Crc_CalculateCrc8H2FFctPtr(crcData, crcDataLength, 0, TRUE);
  }

  return crcValue;
} /* EthTSyn_Crc_ComputeStatusCrc() */

/**********************************************************************************************************************
 *  EthTSyn_Crc_ComputeUserDataCrc
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
 */
ETHTSYN_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Crc_ComputeUserDataCrc(uint8 DataId,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvUserDataType) UserDataSecuredTlvPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 crcData[ETHTSYN_CRC_USER_DATA_CRC_DATA_SIZE];
  uint8 crcDataLength = 0;
  uint8 crcValue = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Crc is enabled. */
  if(EthTSyn_CfgAccess_IsCrcEnabled() == TRUE)
  {
    /* #20 Crc is enabled. Calculate the Crc. */

    /* #30 Add UserData Length to Crc. */
    crcData[crcDataLength] = UserDataSecuredTlvPtr->UserDataLength;
    crcDataLength++;

    /* #40 Add UserData Bytes to Crc. */
    /* Add UserData Byte_0 to Crc. */
    crcData[crcDataLength] = UserDataSecuredTlvPtr->UserByte0;
    crcDataLength++;
    /* Add UserData Byte_1 to Crc. */
    crcData[crcDataLength] = UserDataSecuredTlvPtr->UserByte1;
    crcDataLength++;
    /* Add UserData Byte_2 to Crc. */
    crcData[crcDataLength] = UserDataSecuredTlvPtr->UserByte2;
    crcDataLength++;

    /* #50 Add DataId to Crc. */
    crcData[crcDataLength] = DataId;
    crcDataLength++;

    /* #60 Calculate the Crc. */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    crcValue = EthTSyn_Crc_CalculateCrc8H2FFctPtr(crcData, crcDataLength, 0, TRUE);
  }

  return crcValue;
} /* EthTSyn_Crc_ComputeUserDataCrc() */

/**********************************************************************************************************************
 *  EthTSyn_Crc_ComputeOfsCrc
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
 */
ETHTSYN_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Crc_ComputeOfsCrc(uint8 DataId,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvOfsType) OfsSecuredTlvPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 crcData[ETHTSYN_CRC_OFS_CRC_DATA_SIZE];
  uint8 crcDataLength = 0;
  uint8 crcValue = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Crc is enabled. */
  if(EthTSyn_CfgAccess_IsCrcEnabled() == TRUE)
  {
    /* #20 Add Ofs Time Domain to Crc. */
    crcData[crcDataLength] = OfsSecuredTlvPtr->OfsTimeDomain;
    crcDataLength++;

    /* #30 Add Ofs Time Sec to Crc. */
    crcData[crcDataLength] = OfsSecuredTlvPtr->OfsTimeSec[0];
    crcDataLength++;
    crcData[crcDataLength] = OfsSecuredTlvPtr->OfsTimeSec[1];
    crcDataLength++;
    crcData[crcDataLength] = OfsSecuredTlvPtr->OfsTimeSec[2];
    crcDataLength++;
    crcData[crcDataLength] = OfsSecuredTlvPtr->OfsTimeSec[3];
    crcDataLength++;
    crcData[crcDataLength] = OfsSecuredTlvPtr->OfsTimeSec[4];
    crcDataLength++;
    crcData[crcDataLength] = OfsSecuredTlvPtr->OfsTimeSec[5];
    crcDataLength++;

    /* #40 Add Ofs Time NSec to Crc. */
    crcData[crcDataLength] = OfsSecuredTlvPtr->OfsTimeNSec[0];
    crcDataLength++;
    crcData[crcDataLength] = OfsSecuredTlvPtr->OfsTimeNSec[1];
    crcDataLength++;
    crcData[crcDataLength] = OfsSecuredTlvPtr->OfsTimeNSec[2];
    crcDataLength++;
    crcData[crcDataLength] = OfsSecuredTlvPtr->OfsTimeNSec[3];
    crcDataLength++;

    /* #50 Add Status to Crc. */
    crcData[crcDataLength] = OfsSecuredTlvPtr->Status;
    crcDataLength++;

    /* #60 Add UserData Length to Crc. */
    crcData[crcDataLength] = OfsSecuredTlvPtr->UserDataLength;
    crcDataLength++;

    /* #70 Add UserData bytes to Crc. */
    /* Add UserData Byte_0 to Crc. */
    crcData[crcDataLength] = OfsSecuredTlvPtr->UserByte0;
    crcDataLength++;
    /* Add UserData Byte_1 to Crc. */
    crcData[crcDataLength] = OfsSecuredTlvPtr->UserByte1;
    crcDataLength++;
    /* Add UserData Byte_2 to Crc. */
    crcData[crcDataLength] = OfsSecuredTlvPtr->UserByte2;
    crcDataLength++;

    /* #80 Add DataId to Crc. */
    crcData[crcDataLength] = DataId;
    crcDataLength++;

    /* #90 Calculate the Crc. */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    crcValue = EthTSyn_Crc_CalculateCrc8H2FFctPtr(crcData, crcDataLength, 0, TRUE);
  }

  return crcValue;
} /* EthTSyn_Crc_ComputeOfsCrc() */

/**********************************************************************************************************************
 *  EthTSyn_Crc_ValidateSubTlvCrc
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
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Crc_ValidateSubTlvCrc(uint8 DataId,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr, ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType) ArSubTlvPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean crcIsValid;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check Ar Sub Tlv type */
  switch(ArSubTlvPtr->Type)
  {
    case ETHTSYN_AR_SUB_TLV_TYPE_TIME_SECURED:
      /* #100 Ar Sub Tlv type: Time Secured -> Validate Crc */
      crcIsValid = EthTSyn_Crc_ValidateTimeCrc(DataId, ComMsgPtr,
        (ETHTSYN_P2CONST(EthTSyn_ArSubTlvTimeSecuredType))ArSubTlvPtr);
      break;
    case ETHTSYN_AR_SUB_TLV_TYPE_OFS_SECURED:
      /* #101 Ar Sub Tlv type: Ofs Secured -> Validate Crc */
      crcIsValid = EthTSyn_Crc_ValidateOfsCrc(DataId, (ETHTSYN_P2CONST(EthTSyn_ArSubTlvOfsType))ArSubTlvPtr);
      break;
    case ETHTSYN_AR_SUB_TLV_TYPE_STATUS_SECURED:
      /* #102 Ar Sub Tlv type: Status Secured -> Validate Crc */
      crcIsValid = EthTSyn_Crc_ValidateStatusCrc(DataId, (ETHTSYN_P2CONST(EthTSyn_ArSubTlvStatusType))ArSubTlvPtr);
      break;
    case ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_SECURED:
      /* #103 Ar Sub Tlv type: UserData Secured -> Validate Crc */
      crcIsValid = EthTSyn_Crc_ValidateUserDataCrc(DataId, (ETHTSYN_P2CONST(EthTSyn_ArSubTlvUserDataType))ArSubTlvPtr);
      break;

    default:
      /* #104 Not secured or unknown Ar Sub Tlv type -> return with positive value */
      crcIsValid = TRUE;
      break;
  }

  return crcIsValid;
} /* EthTSyn_Crc_ValidateSubTlvCrc() */

/**********************************************************************************************************************
 *  EthTSyn_Crc_ValidateTimeCrc
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Crc_ValidateTimeCrc(uint8 DataId,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvTimeSecuredType) TimeSecuredTlvPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean timeCrcIsValid = FALSE;
  uint8 crcTime0;
  uint8 crcTime1;

  /* ----- Implementation ----------------------------------------------- */
  crcTime0 = EthTSyn_Crc_ComputeTime0Crc(DataId, TimeSecuredTlvPtr->CrcTimeFlags, ComMsgPtr);
  crcTime1 = EthTSyn_Crc_ComputeTime1Crc(DataId, TimeSecuredTlvPtr->CrcTimeFlags, ComMsgPtr);

  if((TimeSecuredTlvPtr->CrcTime0 == crcTime0) && (TimeSecuredTlvPtr->CrcTime1 == crcTime1) )
  {
    timeCrcIsValid = TRUE;
  }

  return timeCrcIsValid;
} /* EthTSyn_Crc_ValidateTimeCrc() */

/**********************************************************************************************************************
 *  EthTSyn_Crc_ValidateStatusCrc
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Crc_ValidateStatusCrc(uint8 DataId,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvStatusType) StatusSecuredTlvPtr)
{
  /* ----- Local Variable ----------------------------------------------- */
  boolean statusCrcIsValid = FALSE;
  uint8 statusCrc;

  /* ----- Implementation ----------------------------------------------- */
  statusCrc = EthTSyn_Crc_ComputeStatusCrc(DataId, StatusSecuredTlvPtr);

  if(StatusSecuredTlvPtr->CrcStatus == statusCrc)
  {
    statusCrcIsValid = TRUE;
  }

  return statusCrcIsValid;
} /* EthTSyn_Crc_ValidateStatusCrc() */

/**********************************************************************************************************************
 *  EthTSyn_Crc_ValidateUserDataCrc
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Crc_ValidateUserDataCrc(uint8 DataId,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvUserDataType) UserDataSecuredTlvPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean userDataCrcIsValid = FALSE;
  uint8 userDataCrc;

  /* ----- Implementation ----------------------------------------------- */
  userDataCrc = EthTSyn_Crc_ComputeUserDataCrc(DataId, UserDataSecuredTlvPtr);

  if(UserDataSecuredTlvPtr->CrcUserData == userDataCrc)
  {
    userDataCrcIsValid = TRUE;
  }

  return userDataCrcIsValid;
} /* EthTSyn_Crc_ValidateUserDataCrc() */

/**********************************************************************************************************************
 *  EthTSyn_Crc_ValidateOfsCrc
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Crc_ValidateOfsCrc(uint8 DataId,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvOfsType) OfsSecuredTlvPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean ofsCrcIsValid = FALSE;
  uint8 ofsCrc;

  /* ----- Implementation ----------------------------------------------- */
  ofsCrc = EthTSyn_Crc_ComputeOfsCrc(DataId, OfsSecuredTlvPtr);

  if(OfsSecuredTlvPtr->CrcOfs == ofsCrc)
  {
    ofsCrcIsValid = TRUE;
  }

  return ofsCrcIsValid;
} /* EthTSyn_Crc_ValidateOfsCrc() */

#  endif /* ETHTSYN_UNIT_TEST */
#  define ETHTSYN_STOP_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* ETHTSYN_CRC_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Crc_Int.h
 *********************************************************************************************************************/
