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
/**        \file  EthTSyn_Slave_Int.c
 *        \brief  EthTSyn Slave source file - Internal Functions
 *
 *      \details  EthTSyn Slave source file containing the EthTSyn Slave implementation of the EthTSyn module.
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

#define ETHTSYN_SLAVE_INT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_Slave_Int.h" /* PRQA S 3219 */ /* MD_EthTSyn_14.1_3219 */
#include "EthTSyn_Pdelay_Int.h"
#include "EthTSyn_SwtMgmt_Int.h"
#include "EthTSyn_Crc_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of Msn header file */
#if (  (ETHTSYN_SW_MAJOR_VERSION != (5u)) \
    || (ETHTSYN_SW_MINOR_VERSION != (1u)) \
    || (ETHTSYN_SW_PATCH_VERSION != (1u)) )
# error "Vendor specific version numbers of EthTSyn_Slave_Int.c and EthTSyn.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (ETHTSYN_CFG_MAJOR_VERSION != (5u)) \
    || (ETHTSYN_CFG_MINOR_VERSION != (1u)) )
# error "Version numbers of EthTSyn_Slave_Int.c and EthTSyn_Cfg.h are inconsistent!"
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

#if !defined (ETHTSYN_LOCAL)
# define ETHTSYN_LOCAL static
#endif

#if !defined (ETHTSYN_LOCAL_INLINE)
# define ETHTSYN_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define ETHTSYN_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_IsPortReadyToReceiveSync
 *********************************************************************************************************************/
/*! \brief      Checks if the given Port is ready to receive a sync message (including check of PortRole == SLAVE)
 *  \param[in]  PortIdx         Index of EthTSyn port that shall receive the sync message
 *  \return     TRUE - Port is ready to receive the sync message
 *  \return     FALSE - Port is not ready to receive the sync message
 *  \context    ANY
 *********************************************************************************************************************/
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SlaveRx_IsPortReadyToReceiveSync(
  EthTSyn_PortIdxType PortIdx);
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_HandleArFollowUpTlvs
 *********************************************************************************************************************/
/*! \brief         Handles the AUTOSAR (Sub-)Tlvs included in a FollowUp message
 *  \details       This function validates the CRC of each AR (Sub-)Tlv included in the received FollowUp message
 *                 (if applicable), extracts the Status and UserData from the corresponding Sub-Tlv and handles the
 *                 Offset TimeDomain Sub-Tlvs.
 *  \param[in]     PortIdx       Index of the Port the message was received on
 *  \param[in]     ComMsgHdrPtr  The common message header of the received FollowUp message
 *  \param[out]    StatusPtr     Pointer to where to store the retrieved Status
 *  \param[in,out] UserDataPtr   In:  Pointer to Pointer to where to store the extracted UserData
 *                               Out: NULL_PTR if UserData are not applicable
 *  \return        E_OK - Successfully retrieved handled Tlvs. FollowUp message can be received
 *  \return        E_NOT_OK - Failed to process the SubTlvs.
 *  \context       ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SlaveRx_HandleArFollowUpTlvs(
                  EthTSyn_PortIdxType        PortIdx,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType)  ComMsgHdrPtr,
    ETHTSYN_P2VAR(StbM_TimeBaseStatusType)   StatusPtr,
    ETHTSYN_P2VAR(StbM_UserDataType*)        UserDataPtrPtr);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_GetFirstArSubTlvPtrAndCnt
 *********************************************************************************************************************/
/*! \brief      Get pointer to first Ar Sub-Tlv and Ar Sub-Tlv count.
 *  \details    Gets the amount of Ar Sub-Tlvs and the Pointer to the first Sub-Tlv. Furthermore some general checks
 *              regarding the Ar (Sub-) Tlvs are performed (if applicable: check Crc as well)
 *  \param[in]  PortIdx            Index of the Port the message was received on
 *  \param[in]  ComMsgHdrPtr       Pointer to the common message header of the received FollowUp message
 *  \param[out] FirstSubTlvPtrPtr  Pointer to the first Ar Sub-Tlv of the message (NULL_PTR) if no Sub-Tlvs is found
 *  \param[out] SubTlvCnt          The number of found Ar Sub-Tlvs (will also be increased for unknown TLV Types)
 *  \return     E_OK - Successfully retrieved the SubTlv Ptr and count. General checks passed.
 *  \return     E_NOT_OK - Failed to retrieve the SubTlvPtr/count or general checks failed.
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SlaveRx_GetFirstArSubTlvPtrAndCnt(
                                EthTSyn_PortIdxType                   PortIdx,
                ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType)             ComMsgHdrPtr,
  ETHTSYN_P2VAR(ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType))  FirstSubTlvPtrPtr,
                  ETHTSYN_P2VAR(uint8)                                SubTlvCnt);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_CheckArTlvHeader
 *********************************************************************************************************************/
/*! \brief      Checks the validity of the AR Tlv-Header (if present) and extracts the total length of the Sub-Tlvs
 *  \param[in]  ArTlvHdrPtr        Pointer to the potential Ar Tlv header of the received FollowUp message
 *  \param[out] TotalSubTlvLength  The total length of all Ar Sub-Tlvs (i.e. Length field of the Ar Tlv-Header -
 *                                 length of the Ar Tlv-Header itselfe)
 *  \return     E_OK - Check of Ar Tlv Header passed.
 *  \return     E_NOT_OK - Check of Ar Tlv Header failed.
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SlaveRx_CheckArTlvHeader(
  ETHTSYN_P2CONST(EthTSyn_ArTlvHeaderType)  ArTlvHdrPtr,
    ETHTSYN_P2VAR(uint16)                   TotalSubTlvLength);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_CheckArSubTlvCrc
 *********************************************************************************************************************/
/*! \brief      Validates the Type and the Crc (if applicable) of the given FollowUp Ar Sub-Tlv
 *  \param[in]  PortIdx            Index of the Port the FollowUp message was received on
 *  \param[in]  ComMsgPtr          Pointer to the common message header of the FollowUp message
 *  \param[in]  ArTlvHdrPtr        Pointer to the potential Ar Tlv header of the received FollowUp message
 *  \return     E_OK - Check of Ar Sub Tlv passed.
 *  \return     E_NOT_OK - Check of Ar Sub Tlv failed.
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SlaveRx_CheckArSubTlvCrc(
                  EthTSyn_PortIdxType                 PortIdx,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType)           ComMsgPtr,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType) ArSubTlvPtr);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_HandleOfsSubTlvs
 *********************************************************************************************************************/
/*! \brief      Unpacks all Ofs-Sub-Tlvs and forwards the new Offset to the StbM
 *  \param[in]  TimeDomainIdx   Index of the TimeDomain the FollowUp was received for
 *  \param[in]  FirstSubTlvPtr  Pointer to the first Ar Sub-Tlv
 *  \param[in]  SubTlvCount     The amount of the Ar Sub-Tlvs (including unknown Tlv-Types)
 *  \pre        The Pointer to the FirstSubTlv and the SubTlvCount must have been retrieved using
 *              EthTSyn_SlaveRx_GetFirstArSubTlvPtrAndCnt
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_HandleOfsSubTlvs(
                  EthTSyn_TimeDomainIdxType            TimeDomainIdx,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType)  FirstSubTlvPtr,
                  uint8                                SubTlvCount);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcessOfsSubTlv
 *********************************************************************************************************************/
/*! \brief      Process the given offset time domain Sub-Tlv
 *  \param[in]  TimeDomainIdx   Index of the TimeDomain the FollowUp was received for
 *  \param[in]  FirstSubTlvPtr  Pointer to the Offset time domain Sub-Tlv
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcessOfsSubTlv(
                  EthTSyn_TimeDomainIdxType  TimeDomainIdx,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvOfsType)   OfsSubTlvPtr);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_GetUserDataFromSubTlv
 *********************************************************************************************************************/
/*! \brief      Check if a UserData Sub-Tlv is present and retrieve the UserData
 *  \param[in]  FirstSubTlvPtr  Pointer to the first Ar Sub-Tlv
 *  \param[in]  SubTlvCount     The amount of the Ar Sub-Tlvs (including unknown Tlv-Types)
 *  \param[out] UserDataPtr     Pointer to where to store the retrieved UserData
 *  \return     E_OK - UserData Sub-Tlv found and successfully retrieved the UserData
 *  \return     E_NOT_OK - No UserData Sub-Tlv found or failed to retrieve the UserData
 *  \pre        The Pointer to the FirstSubTlv and the SubTlvCount must have been retrieved using
 *              EthTSyn_SlaveRx_GetFirstArSubTlvPtrAndCnt
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SlaveRx_GetUserDataFromSubTlv(
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType)  FirstSubTlvPtr,
                  uint8                                SubTlvCount,
    ETHTSYN_P2VAR(StbM_UserDataType)                   UserDataPtr);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_GetStatusFromSubTlv
 *********************************************************************************************************************/
/*! \brief      Check if a Status Sub-Tlv is present and retrieve the Status
 *  \param[in]  FirstSubTlvPtr  Pointer to the first Ar Sub-Tlv
 *  \param[in]  SubTlvCount     The amount of the Ar Sub-Tlvs (including unknown Tlv-Types)
 *  \param[out] StatusPtr       Pointer to where to store the retrieved Status
 *  \return     E_OK - Status Sub-Tlv found and successfully retrieved the Status
 *  \return     E_NOT_OK - No Status Sub-Tlv found or failed to retrieve the Status
 *  \pre        The Pointer to the FirstSubTlv and the SubTlvCount must have been retrieved using
 *              EthTSyn_SlaveRx_GetFirstArSubTlvPtrAndCnt
 *  \context    ANY
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SlaveRx_GetStatusFromSubTlv(
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType)  FirstSubTlvPtr,
                  uint8                                SubTlvCount,
    ETHTSYN_P2VAR(StbM_TimeBaseStatusType)             StatusPtr);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_GetTimeDiffSinceSyncRx
 *********************************************************************************************************************/
/*! \brief      Computes the time passed since the reception of the corresponding Sync message
 *  \param[in]  PortIdx                  Index of the Port the FollowUp message was received on
 *  \param[in]  SyncIngressTimestampPtr  The ingress timestamp of the corresponding Sync message
 *  \param[out] TimeDiffSinceSyncRxPtr   The time difference since the reception of the Sync message
 *  \context    EthTSyn_SlaveRx_ProcessReceivedFollowUpMsg()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SlaveRx_GetTimeDiffSinceSyncRx(
                  EthTSyn_PortIdxType           PortIdx,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType)  SyncIngressTimestampPtr,
    ETHTSYN_P2VAR(EthTSyn_TimediffType)         TimeDiffSinceSyncRxPtr);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProvideTimeToStbm
 *********************************************************************************************************************/
/*! \brief      Handles the call of StbM_BusSetGlobalTime() for a received FollowUp message (no Offset time domain)
 *  \param[in]  PortIdx                   Index of the Port the FollowUp message was received on
 *  \param[in]  TimeDiffSinceSyncRx       The time difference passed since reception of the corresponding Sync message
 *  \param[in]  TimeBaseStatus            The Time base status extracted from Status TLV (if present)
 *  \param[in]  ComMsgPtr                 Pointer to the Rx Buffer (message header) of the FollowUp message
 *  \param[in]  UserDataPtr               The User Data extracted form the UserData TLV (if present)
 *  \context    EthTSyn_SlaveRx_ProcessReceivedFollowUpMsg()
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProvideTimeToStbm(
                  EthTSyn_PortIdxType          PortIdx,
                  EthTSyn_TimediffType         TimeDiffSinceSyncRx,
                  StbM_TimeBaseStatusType      TimeBaseStatus,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType)    ComMsgPtr,
  ETHTSYN_P2CONST(StbM_UserDataType)           UserDataPtr);

/**********************************************************************************************************************
 *  EthTSyn_Slave_SyncReceiveHandleFollowUpTimeout
 *********************************************************************************************************************/
/*! \brief      Handles a FollowUp Rx timeout
 *  \param[in]  TimeDomainIdx     Index of the TimeDomain
 *  \context    EthTSyn_Slave_ProcessSmSyncReceive()
 *********************************************************************************************************************/
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Slave_SyncReceiveHandleFollowUpTimeout(
  EthTSyn_TimeDomainIdxType   TimeDomainIdx);
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_Slave_SyncReceiveHandleSyncTimeout
 *********************************************************************************************************************/
/*! \brief      Checks if an sync receive timeout occurred and handles it
 *  \param[in]  TimeDomainIdx     Index of the TimeDomain
 *  \param[in]  SyncReceiveSmPtr  Pointer to the Sync receive state machine
 *  \context    EthTSyn_Slave_ProcessSmSyncReceive()
 *********************************************************************************************************************/
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Slave_SyncReceiveHandleSyncTimeout(
                EthTSyn_TimeDomainIdxType   TimeDomainIdx,
  ETHTSYN_P2VAR(EthTSyn_SyncReceiveSmType)  SyncReceiveSmPtr); /* PRQA S 0779 */ /* MD_MSR_5.1_779 */
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_IsPorTReadyToReceiveSync
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SlaveRx_IsPortReadyToReceiveSync(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isReady = FALSE;
  ETHTSYN_P2VAR(EthTSyn_SyncReceiveSmType) syncReceiveSmPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the given Port is a slave port. */
  if(EthTSyn_CfgGetPortRole(PortIdx) == ETHTSYN_PORT_ROLE_SLAVE)
  {
    /* #20 Check if port is AsCapable. */
    if(EthTSyn_Util_GetAsCapable(PortIdx) == TRUE)
    {
      syncReceiveSmPtr = ETHTSYN_SYNC_RECEIVE_SM_PTR(PortIdx); /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
      /* #30 Check sync receive state machine state. */
      if((syncReceiveSmPtr->State == ETHTSYN_STATE_SYNC_RECEIVE_DISCARD) ||
          (syncReceiveSmPtr->State == ETHTSYN_STATE_SYNC_RECEIVE_WAITING_FOR_SYNC))
      {
        isReady = TRUE;
      }
    }
  }
  return isReady;
}
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_HandleArFollowUpTlvs
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
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SlaveRx_HandleArFollowUpTlvs(
  EthTSyn_PortIdxType PortIdx, ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgHdrPtr,
  ETHTSYN_P2VAR(StbM_TimeBaseStatusType) StatusPtr, ETHTSYN_P2VAR(StbM_UserDataType*) UserDataPtrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  uint8 subTlvCnt = 0;
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType) firstSubTlvPtr = NULL_PTR;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check message compliance setting. */
  if(EthTSyn_CfgAccess_IsMessageComplianceEnabled() == TRUE)
  {
    /* #100 Message compliance is enabled. Nothing to do here. */
    retVal = E_OK;
  }
  else
  {
    /* #101 Message compliance is disabled. Handle possible FollowUp Tlvs. */
    /* #20 Retrieve Pointer to the first Ar Sub-Tlv and the Sub-Tlv count. */
    retVal = EthTSyn_SlaveRx_GetFirstArSubTlvPtrAndCnt(PortIdx, ComMsgHdrPtr, &firstSubTlvPtr, &subTlvCnt);
    if((retVal == E_OK) && (firstSubTlvPtr != NULL_PTR))
    {
      /* #30 Get User data from corresponding Sub-Tlv. */
      if(EthTSyn_SlaveRx_GetUserDataFromSubTlv(firstSubTlvPtr, subTlvCnt, *UserDataPtrPtr) == E_NOT_OK)
      {
        /* #300 Failed to retrieve the UserData. Set UserDataPtrPtr to NULL_PTR. */
        *UserDataPtrPtr = NULL_PTR;
      }
      /* #40 Get Status from corresponding Sub-Tlv. */
      if(EthTSyn_SlaveRx_GetStatusFromSubTlv(firstSubTlvPtr, subTlvCnt, StatusPtr) == E_NOT_OK)
      {
        /* #400 Failed to retrieve the status. Set status to '0'. */
        *StatusPtr = 0;
      }

      /* #50 Handle Offset Sub-Tlvs. */
      EthTSyn_SlaveRx_HandleOfsSubTlvs(ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx), firstSubTlvPtr, subTlvCnt);
    }
  }

  return retVal;
} /* EthTSyn_SlaveRx_HandleArFollowUpTlvs() */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_GetFirstArSubTlvPtrAndCnt
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
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SlaveRx_GetFirstArSubTlvPtrAndCnt(
  EthTSyn_PortIdxType PortIdx, ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgHdrPtr,
  ETHTSYN_P2VAR(ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType)) FirstSubTlvPtrPtr, ETHTSYN_P2VAR(uint8) SubTlvCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  ETHTSYN_P2CONST(EthTSyn_ArTlvHeaderType) arTlvHdrPtr;
  uint16 offsetToArTlvHdr;
  sint32 remainingLength;
  uint16 subTlvTotalLength = 0;
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType) subTlvPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize out parameters */
  *FirstSubTlvPtrPtr = NULL_PTR;
  *SubTlvCnt = 0;

  /* #20 Check if the message length indicates at least one Ar-Tlv */
  if((uint16)EthTSyn_Ntohs(ComMsgHdrPtr->MessageLength) >=
    (ETHTSYN_MSG_FOLLOW_UP_RX_MIN_LENGTH + ETHTSYN_AR_TLV_HEADER_LENGTH))
  {
    /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
    arTlvHdrPtr = (ETHTSYN_P2CONST(EthTSyn_ArTlvHeaderType))
      &((ETHTSYN_P2CONST(uint8))ComMsgHdrPtr)[ETHTSYN_MSG_FOLLOW_UP_RX_MIN_LENGTH];

    /* #30 Check Ar Tlv Header */
    retVal = EthTSyn_SlaveRx_CheckArTlvHeader(arTlvHdrPtr, &subTlvTotalLength);

    if(retVal == E_OK)
    {
      /* #40 Unpack and check Sub-Tlvs */
      offsetToArTlvHdr = ETHTSYN_AR_TLV_HEADER_LENGTH;
      remainingLength = (sint32)subTlvTotalLength;

      /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      *FirstSubTlvPtrPtr = (ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType))
        &((ETHTSYN_P2CONST(uint8))arTlvHdrPtr)[offsetToArTlvHdr];

      do
      {
        uint8 tlvLength;

        /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
        subTlvPtr = (ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType))
          &((ETHTSYN_P2CONST(uint8))arTlvHdrPtr)[offsetToArTlvHdr];
        tlvLength = ETHTSYN_AR_SUB_TLV_ABSTRACT_HDR_LENGTH;
        tlvLength += subTlvPtr->Length;

        remainingLength -= (sint32)tlvLength;

        /* #50 Check if Sub Tlv fits in the message */
        if(remainingLength >= 0)
        {
          offsetToArTlvHdr += tlvLength;
          (*SubTlvCnt)++;

          /* Process Sub-Tlv */
          /* #500 Check if Crc is included and valid */
          retVal = EthTSyn_SlaveRx_CheckArSubTlvCrc(PortIdx, ComMsgHdrPtr, subTlvPtr);
        }
        else
        {
          /* #501 Message is to small to hold this Sub Tlv -> Drop message */
          retVal = E_NOT_OK;
        }
      } while((remainingLength > (sint32)ETHTSYN_AR_SUB_TLV_ABSTRACT_HDR_LENGTH) && (retVal == E_OK));
    }
  }
  else
  {
    /* According to message length, no Ar Tlv can be included in the message. This is valid. */
    retVal = E_OK;
  }

  return retVal;
} /* EthTSyn_SlaveRx_GetFirstArSubTlvPtrAndCnt() */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_CheckArTlvHeader
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SlaveRx_CheckArTlvHeader(
  ETHTSYN_P2CONST(EthTSyn_ArTlvHeaderType) ArTlvHdrPtr, ETHTSYN_P2VAR(uint16) TotalSubTlvLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint16 arTlvTypeField;
  uint16 arTlvLengthField;
  uint32 arTlvOrgaId;
  uint32 arTlvOrgaSubType;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Ar-Tlv Header is present */
  ETHTSYN_GET_UINT16(ArTlvHdrPtr->TlvType, 0u, arTlvTypeField);

  if(arTlvTypeField == ETHTSYN_AR_TLV_TYPE)
  {
    /* #20 Perform general checks of the AR-Tlv Header fields */
    ETHTSYN_GET_UINT16(ArTlvHdrPtr->LengthField, 0u, arTlvLengthField);
    ETHTSYN_GET_3_BYTE(ArTlvHdrPtr->OrganisationId, 0u, arTlvOrgaId);
    ETHTSYN_GET_3_BYTE(ArTlvHdrPtr->OrganisationSubType, 0u, arTlvOrgaSubType);

    if(arTlvLengthField > (ETHTSYN_AR_TLV_HEADER_LENGTH_FIELD_VALUE + ETHTSYN_AR_SUB_TLV_ABSTRACT_HDR_LENGTH))
    {
      if((arTlvOrgaId == ETHTSYN_AR_TLV_ORGANIZATION_ID) &&
        (arTlvOrgaSubType == ETHTSYN_AR_TLV_ORGANIZATION_SUB_TYPE))
      {
        /* #30 Ar Tlv Header valid. Extract total length of Sub-Tlvs */
        *TotalSubTlvLength = (uint16)(arTlvLengthField - ETHTSYN_AR_TLV_HEADER_LENGTH_FIELD_VALUE);
        retVal = E_OK;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_CheckArSubTlvCrc
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
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SlaveRx_CheckArSubTlvCrc(
  EthTSyn_PortIdxType PortIdx, ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType) ArSubTlvPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 dataId;
  uint8 rxCrcValidated = ETHTSYN_CFG_GET_RX_CRC_VALIDATED(PortIdx); /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
  boolean timeSecuredCrcFlagsValid = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get corresponding DataId */
  dataId = EthTSyn_CfgAccess_GetFollowUpDataIdListEntry(ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx),
    (uint16)EthTSyn_Ntohs(ComMsgPtr->SequenceId));

  /* #20 Check if Crc is included and matches the configuration. Validate Crc if required. */
  switch(ArSubTlvPtr->Type)
  {
    case ETHTSYN_AR_SUB_TLV_TYPE_TIME_SECURED:
      /* #200 Time secured Tlv. Check if the CrcTimeFlags match the configuration. */
      /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
      timeSecuredCrcFlagsValid = (boolean)(
          ((ETHTSYN_P2CONST(EthTSyn_ArSubTlvTimeSecuredType))ArSubTlvPtr)->CrcTimeFlags ==
          ETHTSYN_CFG_GET_RX_CRC_VALIDATED_FLAGS(PortIdx));
          /* no break */
    case ETHTSYN_AR_SUB_TLV_TYPE_OFS_SECURED: /* PRQA S 2003 */ /* MD_EthTSyn_15.2 */
    case ETHTSYN_AR_SUB_TLV_TYPE_STATUS_SECURED:
    case ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_SECURED:
      /* #201 Secured Tlv. Check if this is valid and if Crc validation is required. */
      if(rxCrcValidated == ETHTSYN_CRC_RX_CRC_IGNORED)
      {
        /* #2010 Secured type valid. No validation of Crc required. */
        retVal = E_OK;
      }
      else if((rxCrcValidated == ETHTSYN_CRC_RX_CRC_OPTIONAL) ||
        (rxCrcValidated == ETHTSYN_CRC_RX_CRC_VALIDATED) )
      {
        /* #2011 Secured type valid. Validation of Crc required -> Validate Crc. */
        /* PRQA S 3415 2 */ /* MD_EthTSyn_12.4 */
        if((timeSecuredCrcFlagsValid == TRUE) &&
            (EthTSyn_Crc_ValidateSubTlvCrc(dataId, ComMsgPtr, ArSubTlvPtr) == TRUE))
        {
          retVal = E_OK;
        }
      }
      else
      {
        /* Nothing to do here (MISRA). */
      }
      break;

    case ETHTSYN_AR_SUB_TLV_TYPE_OFS_NOT_SECURED:
    case ETHTSYN_AR_SUB_TLV_TYPE_STATUS_NOT_SECURED:
    case ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_NOT_SECURED:
      /* #201 Not Secured Sub-Tlv --> Check if this is valid. */
      if(rxCrcValidated != ETHTSYN_CRC_RX_CRC_VALIDATED)
      {
        /* #2010 No Crc required -> receive message. */
        retVal = E_OK;
      }
      break;

    default:
      /* #202 Unknown Tlv-Type --> skip. */
      retVal = E_OK;
      break;
  }

  return retVal;
} /* EthTSyn_SlaveRx_CheckArSubTlvCrc() */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_HandleOfsSubTlvs
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_HandleOfsSubTlvs(EthTSyn_TimeDomainIdxType TimeDomainIdx,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType) FirstSubTlvPtr, uint8 SubTlvCount)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least subTlvCounter;
  ETHTSYN_P2CONST(EthTSyn_OfsTimeDomainConfigType) ofsTimeDomainPtr =
    ETHTSYN_CFG_GET_OFFSET_TIME_DOMAIN_PTR(TimeDomainIdx);
  uint16 currentOffsetFromFirstSubTlv = 0u;
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType) currentSubTlvPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if there are any offset time domains for the passed time domain. */
  if(ofsTimeDomainPtr != NULL_PTR)
  {
    /* #20 Iterate over all Sub-Tlvs. */
    for(subTlvCounter = 0; subTlvCounter < SubTlvCount; subTlvCounter++)
    {
      /* Set current sub tlv pointer */ /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      currentSubTlvPtr = (ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType))
        &((ETHTSYN_P2CONST(uint8))FirstSubTlvPtr)[currentOffsetFromFirstSubTlv];
      currentOffsetFromFirstSubTlv += ETHTSYN_AR_SUB_TLV_ABSTRACT_HDR_LENGTH;
      currentOffsetFromFirstSubTlv += currentSubTlvPtr->Length;

      /* #30 Check if Sub-Tlv is Ofs Tlv. */
      if((currentSubTlvPtr->Type == ETHTSYN_AR_SUB_TLV_TYPE_OFS_NOT_SECURED) ||
        (currentSubTlvPtr->Type == ETHTSYN_AR_SUB_TLV_TYPE_OFS_SECURED))
      {
        /* Ofs Tlv-found */
        ETHTSYN_P2CONST(EthTSyn_ArSubTlvOfsType) ofsTlvPtr =
          (ETHTSYN_P2CONST(EthTSyn_ArSubTlvOfsType))currentSubTlvPtr; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */

        /* #40 Process the offset time domain Sub-Tlv. */
        EthTSyn_SlaveRx_ProcessOfsSubTlv(TimeDomainIdx, ofsTlvPtr);

      } /* END: if(Ofs-Sub-Tlv) */
    } /* END: Iteration of Sub-Tlvs */
  } /* END: if(TimeDomain has Offset time domains) */

} /* EthTSyn_SlaveRx_HandleOfsSubTlvs() */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcessOfsSubTlv
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcessOfsSubTlv(EthTSyn_TimeDomainIdxType TimeDomainIdx,
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvOfsType) OfsSubTlvPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least ofsTimeDomainCounter;
  ETHTSYN_P2CONST(EthTSyn_OfsTimeDomainConfigType) ofsTimeDomainPtr =
      ETHTSYN_CFG_GET_OFFSET_TIME_DOMAIN_PTR(TimeDomainIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Ensure that given time domain has offset time domains. */
  if(ofsTimeDomainPtr != NULL_PTR)
  {
    /* #20 Check if offset time domain is valid for the TimeDomain of the FollowUp. */
    for(ofsTimeDomainCounter = 0; ofsTimeDomainCounter < ETHTSYN_CFG_GET_OFFSET_TIME_DOMAIN_COUNT(TimeDomainIdx);
        ofsTimeDomainCounter++)
    {
      if(OfsSubTlvPtr->OfsTimeDomain == ofsTimeDomainPtr[ofsTimeDomainCounter].OfsTimeDomainId)
      {
        /* #30 Matching Offset TimeDomain found -> Forward new offset to StbM. */
        StbM_TimeStampType timeStamp;
        StbM_UserDataType userData;
        StbM_MeasurementType measureData;

        ETHTSYN_AR_SUB_TLV_STATUS_TO_TIME_BASE_STATUS(OfsSubTlvPtr->Status, timeStamp.timeBaseStatus);

        ETHTSYN_GET_UINT16(OfsSubTlvPtr->OfsTimeSec, 0u, timeStamp.secondsHi);
        ETHTSYN_GET_UINT32(OfsSubTlvPtr->OfsTimeSec, ETHTSYN_AR_SUB_TLV_OFS_TIME_SEC_SECONDS_OFFSET,
            timeStamp.seconds);
        ETHTSYN_GET_UINT32(OfsSubTlvPtr->OfsTimeNSec, 0u, timeStamp.nanoseconds);

        userData.userDataLength = OfsSubTlvPtr->UserDataLength;
        userData.userByte0 = OfsSubTlvPtr->UserByte0;
        userData.userByte1 = OfsSubTlvPtr->UserByte1;
        userData.userByte2 = OfsSubTlvPtr->UserByte2;

        /* Pdelay not required for Offset TimeDomains. Therefore it can be set to '0' */
        measureData.pathDelay = 0;

        ETHTSYN_CHECK_CRITICAL_SECTION_0();
        (void)StbM_BusSetGlobalTime(ofsTimeDomainPtr[ofsTimeDomainCounter].SynchronizedTimeBaseId, &timeStamp,
            &userData, &measureData);

        break;
      }
    } /* END: Iteration of Offset time domains */
  }
} /* EthTSyn_SlaveRx_ProcessOfsSubTlv() */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_GetUserDataFromSubTlv
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SlaveRx_GetUserDataFromSubTlv(
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType) FirstSubTlvPtr, uint8 SubTlvCount,
  ETHTSYN_P2VAR(StbM_UserDataType) UserDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 subTlvCounter;
  uint16 currentOffsetFromFirstSubTlv = 0u;
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType) currentSubTlvPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all Ar Sub-Tlvs. */
  for(subTlvCounter = 0; subTlvCounter < SubTlvCount; subTlvCounter++)
  {
    /* Set current sub tlv pointer */
    /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
    currentSubTlvPtr = (ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType))
      &((ETHTSYN_P2CONST(uint8))FirstSubTlvPtr)[currentOffsetFromFirstSubTlv];
    currentOffsetFromFirstSubTlv += ETHTSYN_AR_SUB_TLV_ABSTRACT_HDR_LENGTH;
    currentOffsetFromFirstSubTlv += currentSubTlvPtr->Length;

    /* #20 Check if Sub-Tlv is UserData Tlv as required. */
    if((currentSubTlvPtr->Type == ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_NOT_SECURED) ||
      (currentSubTlvPtr->Type == ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_SECURED))
    {
      /* #30 UserData Tlv-found -> store the UserData. */
      /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      ETHTSYN_P2CONST(EthTSyn_ArSubTlvUserDataType) userDataTlvPtr =
        (ETHTSYN_P2CONST(EthTSyn_ArSubTlvUserDataType))currentSubTlvPtr;

      UserDataPtr->userDataLength = userDataTlvPtr->UserDataLength;
      UserDataPtr->userByte0 = userDataTlvPtr->UserByte0;
      UserDataPtr->userByte1 = userDataTlvPtr->UserByte1;
      UserDataPtr->userByte2 = userDataTlvPtr->UserByte2;

      retVal = E_OK;

      break; /* UserData found and retrieved -> Stop iteration of Sub-Tlvs */
    }
  }

  return retVal;
} /* EthTSyn_SlaveRx_GetUserDataFromSubTlv() */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_GetStatusFromSubTlv
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SlaveRx_GetStatusFromSubTlv(
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType) FirstSubTlvPtr, uint8 SubTlvCount,
  ETHTSYN_P2VAR(StbM_TimeBaseStatusType) StatusPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 subTlvCounter;
  uint16 currentOffsetFromFirstSubTlv = 0u;
  ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType) currentSubTlvPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all Ar Sub-Tlvs. */
  for(subTlvCounter = 0; subTlvCounter < SubTlvCount; subTlvCounter++)
  {
    /* Set current sub tlv pointer */
    /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
    currentSubTlvPtr = (ETHTSYN_P2CONST(EthTSyn_ArSubTlvAbstractHeaderType))
      &((ETHTSYN_P2CONST(uint8))FirstSubTlvPtr)[currentOffsetFromFirstSubTlv];
    currentOffsetFromFirstSubTlv += ETHTSYN_AR_SUB_TLV_ABSTRACT_HDR_LENGTH;
    currentOffsetFromFirstSubTlv += currentSubTlvPtr->Length;

    /* #20 Check if Sub-Tlv is Status Tlv as required. */
    if((currentSubTlvPtr->Type == ETHTSYN_AR_SUB_TLV_TYPE_STATUS_NOT_SECURED) ||
      (currentSubTlvPtr->Type == ETHTSYN_AR_SUB_TLV_TYPE_STATUS_SECURED))
    {
      /* #30 Status Tlv-found -> store the Status. */
      /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      ETHTSYN_P2CONST(EthTSyn_ArSubTlvStatusType) statusTlvPtr =
        (ETHTSYN_P2CONST(EthTSyn_ArSubTlvStatusType))currentSubTlvPtr;

      ETHTSYN_AR_SUB_TLV_STATUS_TO_TIME_BASE_STATUS(statusTlvPtr->Status, *StatusPtr);

      retVal = E_OK;

      break; /* Status found and retrieved -> Stop iteration of Sub-Tlvs */
    }
  }

  return retVal;
} /* EthTSyn_SlaveRx_GetStatusFromSubTlv() */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_GetTimeDiffSinceSyncRx
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SlaveRx_GetTimeDiffSinceSyncRx(
  EthTSyn_PortIdxType PortIdx, ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) SyncIngressTimestampPtr,
  ETHTSYN_P2VAR(EthTSyn_TimediffType) TimeDiffSinceSyncRxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
#if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  Eth_TimeStampQualType timeStampQual;
  EthTSyn_GlobalTimestampType timeStampRef; /* used as reference Timestamp when HW-Timestamping is used */
#else
  StbM_TimeStampRawType timeStampDiff; /* used to show the time passed between Sync and FollowUp reception */
#endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  /* HW-Timestamping */
  ETHTSYN_CHECK_CRITICAL_SECTION_0();
  if(EthIf_GetCurrentTime(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), &timeStampQual, &timeStampRef) == E_OK)
  {
    if(timeStampQual == ETH_VALID)
    {
      retVal = EthTSyn_Util_TimestampMinusTimestamp(&timeStampRef, SyncIngressTimestampPtr, TimeDiffSinceSyncRxPtr);
    }
  }
#else
  ETHTSYN_DUMMY_STATEMENT(PortIdx); /* PRQA S 3112 */ /* MD_MSR_14.2 */
  /* SW-Timestamping */
  ETHTSYN_CHECK_CRITICAL_SECTION_0();
  if(StbM_GetCurrentTimeDiff(SyncIngressTimestampPtr->nanoseconds, &timeStampDiff) == E_OK)
  {
    *TimeDiffSinceSyncRxPtr = (EthTSyn_TimediffType)timeStampDiff;
    retVal = E_OK;
  }
#endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */

  return retVal;
} /* EthTSyn_SlaveRx_GetTimeDiffSinceSyncRx() */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProvideTimeToStbm
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProvideTimeToStbm(EthTSyn_PortIdxType PortIdx,
  EthTSyn_TimediffType TimeDiffSinceSyncRx, StbM_TimeBaseStatusType TimeBaseStatus,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr, ETHTSYN_P2CONST(StbM_UserDataType) UserDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PdelayType pDelay;
  EthTSyn_GlobalTimestampType preciseOriginTimestamp;
  ETHTSYN_P2CONST(EthTSyn_FollowUpMsgType) followUpMsgPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
  followUpMsgPtr = (ETHTSYN_P2CONST(EthTSyn_FollowUpMsgType))
    &((ETHTSYN_P2CONST(uint8))ComMsgPtr)[ETHTSYN_HDR_COM_LENGTH];

  /* #10 Get the precise origin timestamp from FollowUp message */ /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
  EthTSyn_NtohTimestamp(&preciseOriginTimestamp,
    (ETHTSYN_P2CONST(EthTSyn_TimestampType))followUpMsgPtr->PreciseOriginTimestampSeconds);

  /* #20 Add the FollowUp correction field to the time difference */
  TimeDiffSinceSyncRx += (EthTSyn_TimediffType)
    ETHTSYN_GET_FOLLOW_UP_CORRECTION_VALUE_NS_FROM_MSG(ComMsgPtr->CorrectionField);

  /* #30 Add current Pdelay to the time difference */
  if(EthTSyn_Pdelay_GetCurrentPdelay(PortIdx, &pDelay) == E_OK)
  {
    TimeDiffSinceSyncRx += (EthTSyn_TimediffType)pDelay;
  }

  /* #30 Compute the new time which should be passed to the Stbm */
  if(EthTSyn_Util_TimestampPlusTimediff(&preciseOriginTimestamp, &TimeDiffSinceSyncRx) == E_OK)
  {
    StbM_TimeStampType timeStampStbm;
    StbM_MeasurementType measureData;

    ETHTSYN_COPY_GLOBAL_TS_TO_STBM_TS_W_TBS(preciseOriginTimestamp, timeStampStbm, TimeBaseStatus)
    measureData.pathDelay = pDelay;

    /* #40 Provide new Time to the StbM */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    (void)StbM_BusSetGlobalTime(ETHTSYN_CFG_GET_SYNCHRONIZTED_TIME_BASE_ID_OF_PORT(PortIdx),
      &timeStampStbm, UserDataPtr, &measureData);
  }

} /* EthTSyn_SlaveRx_ProvideTimeToStbm() */

/**********************************************************************************************************************
 *  EthTSyn_Slave_SyncReceiveHandleFollowUpTimeout
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Slave_SyncReceiveHandleFollowUpTimeout(
  EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset state of all SyncSend state machines (Ports) of the TimeDomain. */
  if(EthTSyn_CfgGetSlavePortCnt(TimeDomainIdx) > 0)
  {
    EthTSyn_PortIdxIterType masterPortCnt;
    for(masterPortCnt = 0; masterPortCnt < EthTSyn_CfgGetMasterPortCnt(TimeDomainIdx); masterPortCnt++)
    {
      /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
      ETHTSYN_SYNC_SEND_SM_PTR(
          EthTSyn_TimeDomainConfig[TimeDomainIdx].MasterPortIdxPtr[masterPortCnt])->State =
              ETHTSYN_STATE_SYNC_SEND_SITE_SYNC_SYNC_PENDING;
    }
  }
  /* #20 Reset state of SiteSyncSync state machine */
  if(EthTSyn_SiteSyncSyncSm(TimeDomainIdx).State == ETHTSYN_STATE_SITE_SYNC_SYNC_SEND_FOLLOW_UP)
  {
    EthTSyn_SiteSyncSyncSm(TimeDomainIdx).State = ETHTSYN_STATE_SITE_SYNC_SYNC_PENDING;
  }
} /* EthTSyn_Slave_SyncReceiveHandleFollowUpTimeout() */
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_Slave_SyncReceiveHandleSyncTimeout
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Slave_SyncReceiveHandleSyncTimeout(
  EthTSyn_TimeDomainIdxType TimeDomainIdx, ETHTSYN_P2VAR(EthTSyn_SyncReceiveSmType) SyncReceiveSmPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PortIdxIterType masterPortCnt;
  EthTSyn_PortIdxType masterPortIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check for Sync receive timeout. */
  if(SyncReceiveSmPtr->Sync.SyncRxTimeoutCnt == 0) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
  {
    /* #20 Sync receive timeout occurred -> Take up role as master now. */
    EthTSyn_SiteSyncSyncSm(TimeDomainIdx).State = ETHTSYN_STATE_SITE_SYNC_SYNC_SYNC_TIMEOUT;
    SyncReceiveSmPtr->State = ETHTSYN_STATE_SYNC_RECEIVE_SYNC_TIMEOUT;
    ETHTSYN_STATE_GET_SLAVE_PORT_IDX(TimeDomainIdx) = ETHTSYN_INVALID_PORT_IDX;
    /* #30 Initialize the SyncSendSm of each Master-Port of the TimeDomain. */
    for(masterPortCnt = 0; masterPortCnt < EthTSyn_CfgGetMasterPortCnt(TimeDomainIdx); masterPortCnt++)
    {
      ETHTSYN_P2VAR(EthTSyn_SyncSendSmType) syncSendSmPtr;
      masterPortIdx = EthTSyn_CfgGetMasterPortIdx(TimeDomainIdx, masterPortCnt);
      syncSendSmPtr = ETHTSYN_SYNC_SEND_SM_PTR(masterPortIdx); /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
      syncSendSmPtr->SyncTxBufIdx = ETHTSYN_INV_BUF_IDX;
      syncSendSmPtr->FollowUpTxBufIdx = ETHTSYN_INV_BUF_IDX;
      syncSendSmPtr->TxConfirmationPending = 0;
      syncSendSmPtr->SequenceId = 0;
      /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      syncSendSmPtr->LogMessageInterval =
          EthTSyn_MasterPortConfigPtr(masterPortIdx)->SyncLogMessageInterval;
      syncSendSmPtr->TxIntervalCnt =
          EthTSyn_MasterPortConfigPtr(masterPortIdx)->SyncSendInterval; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */

      syncSendSmPtr->State = ETHTSYN_STATE_SYNC_SEND_INITIALIZING;
    }
  }

} /* EthTSyn_Slave_SyncReceiveHandleSyncTimeout() */
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthTSyn_Slave_ProcessSmSyncReceive
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_Slave_ProcessSmSyncReceive(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TimeDomainIdxType timeDomainIdx;
  ETHTSYN_P2VAR(EthTSyn_SyncReceiveSmType) syncReceiveSmPtr;

  /* ----- Implementation ----------------------------------------------- */
  timeDomainIdx = EthTSyn_CfgAccess_GetTimeDomainIdxOfPort(PortIdx);

  if(timeDomainIdx < ETHTSYN_TIME_DOMAIN_COUNT)
  {
    syncReceiveSmPtr = ETHTSYN_SYNC_RECEIVE_SM_PTR(PortIdx); /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */

    ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    /* #10 Check sync receive state machine state. */
    switch(syncReceiveSmPtr->State)
    {
    case ETHTSYN_STATE_SYNC_RECEIVE_WAITING_FOR_FOLLOW_UP:
      /* #100 State: Waiting for follow up -> check for FollowUp receive timeout. */
      if(EthTSyn_SlavePortConfigPtr(PortIdx)->FollowUpAllowedRxDelay > 0) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
      {
        if(syncReceiveSmPtr->FollowUp.FollowUpRxTimeoutCnt == 0) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
        {
          /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
          syncReceiveSmPtr->State = ETHTSYN_STATE_SYNC_RECEIVE_DISCARD;
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
          EthTSyn_Slave_SyncReceiveHandleFollowUpTimeout(timeDomainIdx);
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
        }
      }
      break;
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    case ETHTSYN_STATE_SYNC_RECEIVE_WAITING_FOR_SYNC:
      /* #101 State: Waiting for sync (with switch management only) -> Check if system is grand master capable. */
      if(EthTSyn_CfgAccess_IsGmCapable() == TRUE)
      {
        /* #1010 Check if system is currently acting as bridge. If so handle possible sync timeout. */
        if( (EthTSyn_CfgGetSlavePortCnt(timeDomainIdx) > 0) &&
            ( (EthTSyn_Util_GetPortType(PortIdx) & ETHTSYN_PORT_TYPE_SWITCH_PORT) != 0) )
        {
          EthTSyn_Slave_SyncReceiveHandleSyncTimeout(timeDomainIdx, syncReceiveSmPtr);
        }
      }
      break;
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
    default:
      /* do nothing */
      break;
    }
    ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
} /* EthTSyn_Slave_ProcessSmSyncReceive() */

/**********************************************************************************************************************
 *  EthTSyn_Slave_ProcessSmAnnounceReceive
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_Slave_ProcessSmAnnounceReceive(EthTSyn_PortIdxType PortIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if port is AsCapable. */
  if(EthTSyn_Util_GetAsCapable(PortIdx) == TRUE)
  {
    /* #20 Check for announce receive timeout and handle it. */
    if(ETHTSYN_ANNOUNCE_RECEIVE_SM_PTR(PortIdx)->RxTimeoutCnt == 0) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
    {
      EthTSyn_ResetMasterPriorityVector(ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx));
    }
  }

} /* EthTSyn_Slave_ProcessSmAnnounceReceive() */

/**********************************************************************************************************************
 *  EthTSyn_Slave_HandleCyclicTasks
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
FUNC(void, ETHTSYN_CODE) EthTSyn_Slave_HandleCyclicTasks(EthTSyn_TimeDomainIdxType TimeDomainIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PortIdxIterType slavePortCnt;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all Slave-Ports of the given TimeDomain */
  for(slavePortCnt = 0; slavePortCnt < EthTSyn_CfgGetSlavePortCnt(TimeDomainIdx); slavePortCnt++)
  {
    EthTSyn_PortIdxType slavePortIdx = EthTSyn_CfgGetSlavePortIdx(TimeDomainIdx, slavePortCnt);
    /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
    ETHTSYN_P2VAR(EthTSyn_SyncReceiveSmType) syncReceiveSmPtr = ETHTSYN_SYNC_RECEIVE_SM_PTR(slavePortIdx);

    ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* AnnounceReceive state-machine */
    /* #20 Handle announce receive state machine timeouts if applicable. */
    if(ETHTSYN_ANNOUNCE_RECEIVE_SM_PTR(slavePortIdx) != NULL_PTR) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
    {
      if(ETHTSYN_ANNOUNCE_RECEIVE_SM_PTR(slavePortIdx)->RxTimeoutCnt > 0) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
      {
        ETHTSYN_ANNOUNCE_RECEIVE_SM_PTR(slavePortIdx)->RxTimeoutCnt--; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
      }
      /* #200 Process announce receive state machine */
      EthTSyn_Slave_ProcessSmAnnounceReceive(slavePortIdx);
    }

    /* #30 Handle sync receive state machine timeouts */
    if(syncReceiveSmPtr->FollowUp.FollowUpRxTimeoutCnt > 0)
    {
      syncReceiveSmPtr->FollowUp.FollowUpRxTimeoutCnt--;
    }

#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    if(syncReceiveSmPtr->Sync.SyncRxTimeoutCnt > 0)
    {
      syncReceiveSmPtr->Sync.SyncRxTimeoutCnt--;
    }
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

    ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #40 Process sync receive state machine. */
    EthTSyn_Slave_ProcessSmSyncReceive(slavePortIdx);
  } /* END: Iteration Slave Ports */
} /* EthTSyn_Slave_HandleCyclicTasks() */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcessReceivedSyncMsg
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
 */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcessReceivedSyncMsg(
  ETHTSYN_CONSTP2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr, EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_SyncReceiveSmType) syncReceiveSmPtr;
  boolean receiveMsg = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if port is able to receive the sync message. */
  if(EthTSyn_SlaveRx_IsPortReadyToReceiveSync(PortIdx) == TRUE)
  {
    /* Get Sync receive state machine Pointer */
    syncReceiveSmPtr = ETHTSYN_SYNC_RECEIVE_SM_PTR(PortIdx); /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
    /* #20 Get sync ingress timestamp. */
# if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
    /* HW-Timestamping */
    {
      Eth_TimestampQualityType timestampQuality;
      syncReceiveSmPtr->Sync.IngressTimestampState =
          ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_NOT_AVAILABLE;

      ETHTSYN_CHECK_CRITICAL_SECTION_0();
      /* PRQA S 0310, 0311 2 */ /* MD_EthTSyn_11.4, MD_EthTSyn_11.5 */
      if(EthIf_GetIngressTimeStamp(ETHTSYN_CFG_GET_ETH_IF_CTRL_IDX(PortIdx), (ETHTSYN_P2VAR(Eth_DataType))ComMsgPtr,
          &timestampQuality, &syncReceiveSmPtr->Sync.IngressTimestamp) == E_OK)
      {
        if(timestampQuality == ETH_VALID)
        {
          syncReceiveSmPtr->Sync.IngressTimestampState =
              ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE;
        }
      }
    }
# else
    /* SW-Timestamping */
    ETHTSYN_CHECK_CRITICAL_SECTION_0();
    if(StbM_GetCurrentTimeRaw(&syncReceiveSmPtr->Sync.IngressTimestamp.nanoseconds) == E_OK)
    {
      syncReceiveSmPtr->Sync.IngressTimestampState =
          ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE;
    }
    else
    {
      syncReceiveSmPtr->Sync.IngressTimestampState =
          ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_NOT_AVAILABLE;
    }
# endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */

    ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #30 Check if source port identity check is enabled. */
    if(EthTSyn_CfgGetEnableSourcePortIdentityCheck(PortIdx) == TRUE) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
    {
      /* #300 Verify source port identity. */
      if(EthTSyn_CheckSourcePortIdentity(ComMsgPtr, PortIdx) == TRUE)
      {
        /* Source port identity valid --> receive message */
        receiveMsg = TRUE;
      }
    }
    /* #40 Check if Announce is enabled. */
    else if(EthTSyn_CfgGetEnableAnnounce(PortIdx) == TRUE) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
    {
      /* #400 Validate System State. */
      if(EthTSyn_CheckAnnounce(ComMsgPtr, ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx)) == TRUE)
      {
        /* Announce check passed -> receive message */
        receiveMsg = TRUE;
      }
    }
    else
    {
      receiveMsg = TRUE;
    }

    /* #50 Check if message is ready for reception. */
    if(receiveMsg == TRUE)
    {
      /* Set Sync message attributes */
      syncReceiveSmPtr->Sync.LogMessageInterval = ComMsgPtr->LogMessageInterval;

      EthTSyn_SetClockIdentity(syncReceiveSmPtr->Sync.SourcePortIdentity.ClockIdentity,
          ComMsgPtr->SourceClockIdentity);

      syncReceiveSmPtr->Sync.SourcePortIdentity.PortNumber = (EthTSyn_PortNumberType)
                EthTSyn_Ntohs(ComMsgPtr->SourcePortNumber);

      syncReceiveSmPtr->Sync.SequenceId = (uint16)EthTSyn_Ntohs(ComMsgPtr->SequenceId);

      /* Set FollowUp receive timeout if required */
      if(EthTSyn_SlavePortConfigPtr(PortIdx)->FollowUpAllowedRxDelay > 0u) /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
      {
        syncReceiveSmPtr->FollowUp.FollowUpRxTimeoutCnt =
            EthTSyn_SlavePortConfigPtr(PortIdx)->FollowUpAllowedRxDelay; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */
      }

      /* Change state to waiting for FollowUp */
      syncReceiveSmPtr->State = ETHTSYN_STATE_SYNC_RECEIVE_WAITING_FOR_FOLLOW_UP;
    }

    ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

} /* EthTSyn_SlaveRx_ProcessReceivedSyncMsg() */
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcessReceivedFollowUpMsg
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
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcessReceivedFollowUpMsg(
  ETHTSYN_CONSTP2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr, EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_SyncReceiveSmType) syncReceiveSmPtr;
  boolean processMsg = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check that Rx-Port is Slave Port */
  if(EthTSyn_CfgGetPortRole(PortIdx) == ETHTSYN_PORT_ROLE_SLAVE)
  {
    syncReceiveSmPtr = ETHTSYN_SYNC_RECEIVE_SM_PTR(PortIdx); /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */

    /* #20 Check sync receive state machine state and matching sequence id */
    if((syncReceiveSmPtr->State == ETHTSYN_STATE_SYNC_RECEIVE_WAITING_FOR_FOLLOW_UP) &&
        (syncReceiveSmPtr->Sync.SequenceId == (uint16)EthTSyn_Ntohs(ComMsgPtr->SequenceId)))
    {
      /* Waiting for FollowUp + matching Sequence Id -> continue message processing */

      /* #30 Check for matching source port identity */
      /* PRQA S 0310 3 */ /* MD_EthTSyn_11.4 */
      if(EthTSyn_PortIdentityIsEqual(
          (ETHTSYN_P2CONST(EthTSyn_PortIdentityType))&syncReceiveSmPtr->Sync.SourcePortIdentity,
          (ETHTSYN_P2CONST(EthTSyn_PortIdentityType))ComMsgPtr->SourceClockIdentity) == TRUE)
      {
        processMsg = TRUE;
      }
    }

    /* #40 Check if message should be processed */
    if(processMsg == TRUE)
    {
      EthTSyn_TimediffType timeDiff = 0;
      StbM_UserDataType userData;
      ETHTSYN_P2VAR(StbM_UserDataType) userDataPtr = &userData;
      StbM_TimeBaseStatusType status = 0;

      /* #50 Check for Ar Tlvs */
      if(EthTSyn_SlaveRx_HandleArFollowUpTlvs(PortIdx, ComMsgPtr, &status, &userDataPtr) == E_OK)
      {
        /* PRQA S 0310 1 */ /* MD_EthTSyn_11.4 */
        if(EthTSyn_SlaveRx_GetTimeDiffSinceSyncRx(PortIdx, &ETHTSYN_SYNC_RECEIVE_SM_PTR(PortIdx)->Sync.IngressTimestamp,
            &timeDiff) == E_OK)
        {
          EthTSyn_SlaveRx_ProvideTimeToStbm(PortIdx, timeDiff, status, ComMsgPtr, userDataPtr);
        }
        syncReceiveSmPtr->State = ETHTSYN_STATE_SYNC_RECEIVE_WAITING_FOR_SYNC;
      }
    } /* END: if(processMsg == TRUE) */
  } /* END: if(Slave-Port) */

} /* EthTSyn_SlaveRx_ProcessReceivedFollowUpMsg() */

/**********************************************************************************************************************
 *  EthTSyn_Slave_ProcessReceivedAnnounceMsg
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_Slave_ProcessReceivedAnnounceMsg(
  ETHTSYN_CONSTP2CONST(EthTSyn_CommonMsgHdrType) ComMsgPtr, EthTSyn_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2CONST(EthTSyn_AnnounceMsgType) announceMsgPtr;
  EthTSyn_TimeDomainIdxType timeDomainIdx = ETHTSYN_CFG_GET_TIME_DOMAIN_IDEX_OF_PORT(PortIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if announce message can be received on the Port. */
  if(EthTSyn_UtilRx_IsPortReadyToReceiveAnnounce(PortIdx) == TRUE)
  {
    ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    /* #20 Validated the Announce Message. */
    if(EthTSyn_UtilRx_QualifyAnnounceMsg(PortIdx, ComMsgPtr) == E_OK)
    {
      /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      announceMsgPtr =
          (ETHTSYN_P2CONST(EthTSyn_AnnounceMsgType))&((ETHTSYN_P2CONST(uint8))ComMsgPtr)[ETHTSYN_HDR_COM_LENGTH];

      /* Reset announce timeout */ /* PRQA S 0310 2 */ /* MD_EthTSyn_11.4 */
      ETHTSYN_ANNOUNCE_RECEIVE_SM_PTR(PortIdx)->RxTimeoutCnt =
          EthTSyn_SlavePortConfigPtr(PortIdx)->AnnounceAllowedRxDelay;

      /* #30 Set new Gransmaster if applicable. */
      if(EthTSyn_SystemState[timeDomainIdx].GmPresent == FALSE)
      {
        EthTSyn_SetMasterPriorityVector(announceMsgPtr, timeDomainIdx);
      }
    }
    ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

} /* EthTSyn_Slave_ProcessReceivedAnnounceMsg() */

/**********************************************************************************************************************
 *  EthTSyn_EndStation_Slave_ProcessReceivedSync
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
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
FUNC(void, ETHTSYN_CODE) EthTSyn_EndStation_Slave_ProcessReceivedSync(EthTSyn_PortIdxType  PortIdx,
  ETHTSYN_P2CONST(EthTSyn_CommonMsgHdrType) ComMsgHdrPtr,
  ETHTSYN_P2CONST(EthTSyn_SwtMgmt_RxFrameInfoType) RxFrameInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  ETHTSYN_P2VAR(EthTSyn_SyncReceiveSmType) syncReceiveSmPtr;
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) SyncIngressTimestampPtr = &RxFrameInfoPtr->IngressTimestamp;
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check for min/correct Sync message length. */
  if(ETHTSYN_MSG_SYNC_LENGTH != EthTSyn_Ntohs(ComMsgHdrPtr->MessageLength))
  {
    errorId = ETHTSYN_E_INV_MSG_LENGTH;
  }
  else
  {
    if(EthTSyn_CfgGetPortRole(PortIdx) == ETHTSYN_PORT_ROLE_SLAVE)
    {
      ETHTSYN_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      syncReceiveSmPtr = ETHTSYN_SYNC_RECEIVE_SM_PTR(PortIdx); /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */

      /* #20 Verify sync receive state machine state. */
      if( (syncReceiveSmPtr->State == ETHTSYN_STATE_SYNC_RECEIVE_DISCARD) ||
        (syncReceiveSmPtr->State == ETHTSYN_STATE_SYNC_RECEIVE_WAITING_FOR_SYNC) )
      {
        /* #30 Store the current ingress timestamp. */
# if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
        /* HW-Timestamping */
        syncReceiveSmPtr->Sync.IngressTimestamp = *SyncIngressTimestampPtr;
# else
        /* SW-Timestamping */
        syncReceiveSmPtr->Sync.IngressTimestamp.nanoseconds = SyncIngressTimestampPtr->nanoseconds;
# endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */
        syncReceiveSmPtr->Sync.IngressTimestampState = ETHTSYN_TIMESTAMP_STATE_TIMESTAMP_REQUIRED_AVAILABLE;

        /* #40 Set Sync message attributes in sync receive state machine. */
        syncReceiveSmPtr->Sync.LogMessageInterval = ComMsgHdrPtr->LogMessageInterval;

        EthTSyn_SetClockIdentity(syncReceiveSmPtr->Sync.SourcePortIdentity.ClockIdentity,
          ComMsgHdrPtr->SourceClockIdentity);
        syncReceiveSmPtr->Sync.SourcePortIdentity.PortNumber =
          (uint16)EthTSyn_Ntohs(ComMsgHdrPtr->SourcePortNumber);

        /* store sequence id */
        syncReceiveSmPtr->Sync.SequenceId = (uint16)EthTSyn_Ntohs(ComMsgHdrPtr->SequenceId);

        /* set FollowUp receive timeout time */
        syncReceiveSmPtr->FollowUp.FollowUpRxTimeoutCnt =
          EthTSyn_SlavePortConfigPtr(PortIdx)->FollowUpAllowedRxDelay; /* PRQA S 0310 */ /* MD_EthTSyn_11.4 */

        /* #50 Change state to waiting for FollowUp. */
        syncReceiveSmPtr->State = ETHTSYN_STATE_SYNC_RECEIVE_WAITING_FOR_FOLLOW_UP;
      }
      ETHTSYN_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

# if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  if(errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_RX_INDICATION, errorId);
  }
# else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif /*  */
} /* EthTSyn_EndStation_Slave_ProcessReceivedSync() */
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

#define ETHTSYN_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:
 * see file EthTSyn.c
*/

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Slave_Int.c
 *********************************************************************************************************************/
