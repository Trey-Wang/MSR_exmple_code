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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  AvTp.c
 *      Project:  Audio Video Bridging Transport Protocol
 *       Module:  Tp_Ieee1722AvTp
 *    Generator:  Tp_Ieee1722AvTp.jar (DaVinci Configurator Pro)
 *
 *  Description:  Implementation of the Audio Video Bridging (AVB) Transport Protocol
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

#define AVTP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "AvTp.h"
#include "AvTp_Priv.h"
#include "AvTp_Cfg.h"
#include "AvTp_Lcfg.h"
#include "SchM_AvTp.h"

#include "IpBase.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* vendor specific version information is BCD coded */
#if (  (AVTP_SW_MAJOR_VERSION != 5u) \
    || (AVTP_SW_MINOR_VERSION != 0u) \
    || (AVTP_SW_PATCH_VERSION != 0u)  )
  #error "Vendor specific version numbers of AvTp.c and AvTp.h are inconsistent"
#endif

/* Check configurator version generated to AvTp_Cfg.h */
#if (   (AVTP_CFG_MAJOR_VERSION != (0x05)) \
     || (AVTP_CFG_MINOR_VERSION != (0x00))  )
# error "Version numbers of AvTp.c and AvTp_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#if defined (STATIC)
#else
# define STATIC static
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define AVTP_START_SEC_CONST_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

#if (STD_ON == AVTP_MAAP_SUPPORT)
/*! \brief First MAC-address of dynamic allocation pool */
STATIC CONST(Eth_PhysAddrType, AVTP_CONST) Maap_DynAllocPoolStartAddr =
{
  0x91U,
  0xE0U,
  0xF0U,
  0x00U,
  0x00U,
  0x00U
};

/*! \brief Destination address for MAAP related messages */
STATIC CONST(Eth_PhysAddrType, AVTP_CONST) AvTp_Maap_MulticastMac =
{
  0x91U,
  0xE0U,
  0xF0U,
  0x00U,
  0xFFU,
  0x00U
};
#endif /* (STD_ON == AVTP_MAAP_SUPPORT) */

#define AVTP_STOP_SEC_CONST_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define AVTP_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC FUNC(void, AVTP_CODE) AvTp_ResetCtrl(
  AvTp_CtrlIdxType AvTpCtrlIdx);

#if (STD_ON == AVTP_MAAP_SUPPORT)
STATIC FUNC(void, AVTP_CODE) AvTp_Maap_Init(
  AvTp_CtrlIdxType AvTpCtrlIdx);

STATIC FUNC(void, AVTP_CODE) AvTp_Maap_MainFunction(void);

STATIC FUNC(void, AVTP_CODE) AvTp_Maap_RxIndication(
  AvTp_CtrlIdxType AvTpCtrlIdx,
  AVTP_P2VAR(uint8) PhysSrcAddrPtr,
  AVTP_P2VAR(AvTp_DataType) DataPtr,
  uint16 LenByte);

STATIC FUNC(boolean, AVTP_CODE) AvTp_Maap_CompareMac(
  CONST(Eth_PhysAddrType, AVTP_CONST) RxPhysAddrPtr);

STATIC FUNC(boolean, AVTP_CODE) AvTp_Maap_CheckConflict(
  AvTp_CtrlIdxType AvTpCtrlIdx,
  CONST(Eth_PhysAddrType, AVTP_CONST) RxPhysAddrPtr,
  uint16 RequestCount);

STATIC FUNC(uint16, AVTP_CODE) AvTp_Maap_CalculateConflictCount(
  AvTp_CtrlIdxType AvTpCtrlIdx,
  CONST(Eth_PhysAddrType, AVTP_CONST) RxPhysAddrPtr,
  uint16 RequestCount,
  Eth_PhysAddrType ConflictStartPhysAddrPtr);

STATIC FUNC(void, AVTP_CODE) AvTp_Maap_TxProbe(
  AvTp_CtrlIdxType AvTpCtrlIdx);

STATIC FUNC(void, AVTP_CODE) AvTp_Maap_TxAnnounce(
  AvTp_CtrlIdxType AvTpCtrlIdx);

STATIC FUNC(void, AVTP_CODE) AvTp_Maap_TxDefend(
  AvTp_CtrlIdxType AvTpCtrlIdx,
  AVTP_P2VAR(uint8) PhysSrcAddrPtr,
  Eth_PhysAddrType RxPhysAddrPtr,
  uint16 RequestCount);

STATIC FUNC(void, AVTP_CODE) AvTp_Maap_GenerateDynamicAddr(
  AvTp_CtrlIdxType AvTpCtrlIdx);
#endif /* (STD_ON == AVTP_MAAP_SUPPORT) */

#if (0 < AVTP_STREAM_CNT_RX)
STATIC FUNC(void, AVTP_CODE) AvTp_ProcessStreamPdu(
  AvTp_CtrlIdxType AvTpCtrlIdx,
  AVTP_P2VAR(AvTp_DataType) DataPtr,
  uint16 LenByte);

#if (STD_ON == AVTP_61883IIDC_SUPPORT)
LOCAL_INLINE FUNC(boolean, AVTP_CODE) AvTp_61883iidc_Process(
  #if (STD_ON == AVTP_RX_CHECK)
  AvTp_StreamIdxType StreamIdx,
  #endif /* (STD_ON == AVTP_RX_CHECK) */
  AVTP_P2VAR(AvTp_StreamHdrType) StreamHdrPtr,
  AVTP_P2VAR(AvTp_RxXPduInfoType) AvTp_RxXPduInfoPtr);
#endif /* (STD_ON == AVTP_61883IIDC_SUPPORT) */

#if (STD_ON == AVTP_AAF_SUPPORT)
LOCAL_INLINE FUNC(boolean, AVTP_CODE) AvTp_Aaf_Process(
  #if (STD_ON == AVTP_RX_CHECK)
  AvTp_StreamIdxType StreamIdx,
  #endif /*(STD_ON == AVTP_RX_CHECK) */
  AVTP_P2VAR(AvTp_StreamHdrType) StreamHdrPtr,
  AVTP_P2VAR(AvTp_RxXPduInfoType) AvTp_RxXPduInfoPtr);
#endif /*( STD_ON == AVTP_AAF_SUPPORT) */

#if (STD_ON == AVTP_CVF_SUPPORT)
LOCAL_INLINE FUNC(boolean, AVTP_CODE) AvTp_Cvf_Process(
  #if (STD_ON == AVTP_RX_CHECK)
  AvTp_StreamIdxType StreamIdx,
  #endif /* (STD_ON == AVTP_RX_CHECK) */
  AVTP_P2VAR(AvTp_StreamHdrType) StreamHdrPtr,
  AVTP_P2VAR(AvTp_RxXPduInfoType) AvTp_RxXPduInfoPtr);
#endif /* (STD_ON == AVTP_CVF_SUPPORT) */

#if (STD_ON == AVTP_CRF_SUPPORT)
LOCAL_INLINE FUNC(boolean, AVTP_CODE) AvTp_Crf_Process(
  #if (STD_ON == AVTP_RX_CHECK)
  AvTp_StreamIdxType StreamIdx,
  #endif /* (STD_ON == AVTP_RX_CHECK) */
  AVTP_P2VAR(AvTp_StreamHdrType) StreamHdrPtr,
  AVTP_P2VAR(AvTp_RxXPduInfoType) AvTp_RxXPduInfoPtr);
#endif /* (STD_ON == AVTP_CRF_SUPPORT) */

#if (STD_ON == AVTP_VSF_SUPPORT)
LOCAL_INLINE FUNC(boolean, AVTP_CODE) AvTp_Vsf_Process(
  #if (STD_ON == AVTP_RX_CHECK)
  AvTp_StreamIdxType StreamIdx,
  #endif /* (STD_ON == AVTP_RX_CHECK) */
  AVTP_P2VAR(AvTp_StreamHdrType) StreamHdrPtr,
  AVTP_P2VAR(AvTp_RxXPduInfoType) AvTp_RxXPduInfoPtr);
#endif /* (STD_ON == AVTP_VSF_SUPPORT) */

LOCAL_INLINE FUNC(Std_ReturnType, AVTP_CODE) AvTp_SearchStreamIdx(
  AVTP_P2CONST(AvTp_StreamIdType) StreamIdPtr,
  AVTP_P2VAR(AvTp_StreamIdxType) StreamIdxPtr);

STATIC FUNC(AvTp_CompareType, AVTP_CODE) AvTp_CompareStreamID(
  CONST(AvTp_StreamIdType, AVTP_CONST) StreamId1,
  CONST(AvTp_StreamIdType, AVTP_CONST) StreamId2);
#endif /* (0 < AVTP_STREAM_CNT_RX) */

#if (0 < AVTP_STREAM_CNT_TX)
STATIC FUNC(void, AVTP_CODE) AvTp_StateChangedFunctions(
  AvTp_CtrlIdxType AvTpCtrlIdx,
  AvTp_StateType NewState);

# if (STD_ON == AVTP_TIME_SUPPORT)
STATIC FUNC(Std_ReturnType, AVTP_CODE) AvTp_TimestampPlusTimediff(
  AVTP_P2VAR(AvTp_GlobalTimestampType) DestTimestampPtr,
  AVTP_P2CONST(AvTp_TimediffType) TimediffPtr);
# endif /* (STD_ON == AVTP_TIME_SUPPORT) */

/**********************************************************************************************************************
 *    AvTp_GetCurrentTime
 *********************************************************************************************************************/
/**
  \brief       Retrieves the current time for the given stream
  \param[in]   StreamIdx       Stream Index of received stream
  \param[out]  TimestampPtr    Pointer to timestamp information
  \param[out]  SyncStatePtr    Pointer containing synchronization state information 
  \return      none
  \context     This function can be called in any context.
 *********************************************************************************************************************/
# if (STD_ON == AVTP_TIME_SUPPORT)
STATIC FUNC(Std_ReturnType, AVTP_CODE) AvTp_GetCurrentTime(
  AvTp_StreamIdxType StreamIdx,
  AVTP_P2VAR(AvTp_GlobalTimestampType) TimestampPtr,
  AVTP_P2VAR(AvTp_SyncStateType) SyncStatePtr);
# endif /* (STD_ON == AVTP_TIME_SUPPORT) */

/**********************************************************************************************************************
 *    AvTp_TimeBaseStatusToSyncStatus
 *********************************************************************************************************************/
/**
  \brief       Transforms the TimeBaseStatus reported by the StbM to a Sync State
  \param[in]   TimeBaseStatus  The TimeBaseStatus reported by the StbM
  \param[out]  SyncStatePtr    Pointer containing synchronization state information 
  \return      none
  \context     This function can be called in any context.
 *********************************************************************************************************************/
# if (AVTP_STBM_SUPPORT == STD_ON)
LOCAL_INLINE FUNC(void, AVTP_CODE) AvTp_TimeBaseStatusToSyncStatus(
             StbM_TimeBaseStatusType TimeBaseStatus,
  AVTP_P2VAR(AvTp_SyncStateType)     SyncStatePtr);
# endif /* (AVTP_STBM_SUPPORT == STD_ON) */

#endif /* (0 < AVTP_STREAM_CNT_TX) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/* PRQA S 0310 TYPE_CAST */ /* MD_AvTp_11.4 */

#if (STD_ON == AVTP_VERSION_INFO_API)
/**********************************************************************************************************************
 *  AvTp_GetVersionInfo
 *********************************************************************************************************************/
/**
  \brief        Return the BCD-coded version information of the AvTp module.
  \param[in]    VersionInfoPtr   Pointer to a memory location where the AvTp version information shall be stored.
  \return       none
  \context      This function can be called in any context.
  \availability This function is only available if AvTpVersionInfoApi is enabled.
 *********************************************************************************************************************/
FUNC(void, AVTP_CODE) AvTp_GetVersionInfo(AVTP_P2VAR(Std_VersionInfoType) VersionInfoPtr)
{
  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  AvTp_CheckDetErrorReturnVoid(AVTP_COND_VALID_PTR(VersionInfoPtr), AVTP_API_ID_GET_VERSION_INFO, AVTP_E_INV_POINTER)

  /* ----- Implementation ----------------------------------------------- */
  VersionInfoPtr->vendorID = (uint16)AVTP_VENDOR_ID;
  VersionInfoPtr->moduleID = (uint16)AVTP_MODULE_ID;
  VersionInfoPtr->sw_major_version = (uint8)AVTP_SW_MAJOR_VERSION;
  VersionInfoPtr->sw_minor_version = (uint8)AVTP_SW_MINOR_VERSION;
  VersionInfoPtr->sw_patch_version = (uint8)AVTP_SW_PATCH_VERSION;
}/* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif  /* (STD_ON == AVTP_VERSION_INFO_API) */

/**********************************************************************************************************************
 *  AvTp_RxIndication
 *********************************************************************************************************************/
/**
  \brief          Handles processing of received AvTp frames.
  \param[in]      VCtrlIdx     Index of the virtual controller that has received the frame.
  \param[in]      FrameType    Ethertype of the frame
  \param[in]      IsBroadcast  Determines that the frame was transmitted as broadcast
  \param[in]      PhysAddrPtr  Pointer to the physical address of the transmitted interface
  \param[in]      DataPtr      Pointer to the received data.
  \param[in]      LenByte      Byte count of the received frame.
  \return         none
  \context        This function can be called in any context.
 *********************************************************************************************************************/
/* PRQA S 2006 RET_PATH */ /* MD_MSR_14.7 */
/* PRQA S 6030 STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6010 STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6060 STPAR */ /* MD_AvTp_STPAR_6060 */
FUNC(void, AVTP_CODE) AvTp_RxIndication(
  uint8 VCtrlIdx,
  Eth_FrameType FrameType,
  boolean IsBroadcast,
  AVTP_P2VAR(uint8) PhysAddrPtr,
  AVTP_P2VAR(uint8) DataPtr,
  uint16 LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  AVTP_P2VAR(AvTp_CommonHdrType) CommonHdrPtr;
  AvTp_CtrlIdxType AvTpCtrlIdx; /* PRQA S 0781 */ /* MD_AvTp_5.6 */

  AVTP_DUMMY_STATEMENT(IsBroadcast); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#if (STD_OFF == AVTP_MAAP_SUPPORT)
  AVTP_DUMMY_STATEMENT(PhysAddrPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */
#endif

  /* ----- Development Error Checks ------------------------------------- */
  AvTp_CheckDetErrorReturnVoid(AVTP_COND_VALID_PTR(DataPtr), AVTP_API_ID_RX_INDICATION, AVTP_E_INV_POINTER)
  AvTp_CheckDetErrorReturnVoid(AVTP_COND_MIN_COM_HDR_LEN(LenByte), AVTP_API_ID_RX_INDICATION, AVTP_E_INV_LENGTH)
  AvTp_CheckDetErrorReturnVoid(AVTP_CONT_VALID_VCTRL_IDX(VCtrlIdx), AVTP_API_ID_RX_INDICATION, AVTP_E_INV_VCTRL_IDX)
  AvTp_CheckDetErrorReturnVoid(AVTP_CONT_VALID_FRAME_TYPE(FrameType), AVTP_API_ID_RX_INDICATION,
    AVTP_E_INV_FRAME_TYPE)

  /* get AvTpCtrlIdx to VCtrlIdx */
  AvTpCtrlIdx = AvTp_CtrlMap[VCtrlIdx];

  if(AvTpCtrlIdx == AVTP_CTRL_IDX_INVALID)
  {
    return;
  }
  AvTp_CheckDetErrorReturnVoid(AVTP_COND_NOT_UNINT(AvTp_State[AvTpCtrlIdx]), AVTP_API_ID_RX_INDICATION,
    AVTP_E_NOT_INITIALIZED)

  /* ----- Implementation ----------------------------------------------- */
  /* set received data to common header */
  CommonHdrPtr = (AVTP_P2VAR(AvTp_CommonHdrType)) DataPtr;

  /* Check Version */
  if(AVTP_VERSION !=  AVTP_HDR_COM_VERSION(CommonHdrPtr->HVersion))
  {
    /* Invalid version -> drop message */
    return;
  }

  /* Switch by Subtype */
  switch(CommonHdrPtr->Subtype)
  {
    /* stream */
#if (0 < AVTP_STREAM_CNT_RX)
  #if (STD_ON == AVTP_61883IIDC_SUPPORT)
  case AVTP_SUBTYPE_61883IIDC:
  #endif /* (STD_ON == AVTP_61883IIDC_SUPPORT) */
  #if (STD_ON == AVTP_AAF_SUPPORT)
  case AVTP_SUBTYPE_AAF:
  #endif /* (STD_ON == AVTP_AAF_SUPPORT) */
  #if (STD_ON == AVTP_CVF_SUPPORT)
  case AVTP_SUBTYPE_CVF:
  #endif /* (STD_ON == AVTP_CVF_SUPPORT) */
  #if (STD_ON == AVTP_CRF_SUPPORT)
  case AVTP_SUBTYPE_CRF:
  #endif /* (STD_ON == AVTP_CRF_SUPPORT) */
  #if (STD_ON == AVTP_VSF_SUPPORT)
  case AVTP_SUBTYPE_VSF:
  #endif /* (STD_ON == AVTP_VSF_SUPPORT) */
    AvTp_ProcessStreamPdu(AvTpCtrlIdx, (AVTP_P2VAR(AvTp_DataType)) DataPtr, LenByte);
    break;
#endif /* (0 < AVTP_STREAM_CNT_RX) */

    /* control */
#if (STD_ON == AVTP_MAAP_SUPPORT)
  case AVTP_SUBTYPE_MAAP:
    AvTp_Maap_RxIndication(AvTpCtrlIdx, PhysAddrPtr, (AVTP_P2VAR(AvTp_DataType)) DataPtr, LenByte);
    break;
#endif /* (STD_ON == AVTP_MAAP_SUPPORT) */

  default:
    {
      /* invalid / unsupported subtype */
#if (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT)
      AVTP_P2VAR(AvTp_StreamHdrType) StreamHdrPtr;
      AvTp_StreamIdxType StreamIdx = 0; /* PRQA S 0781 */ /* MD_AvTp_5.6 */

      StreamHdrPtr = (AVTP_P2VAR(AvTp_StreamHdrType)) DataPtr;

      /* get StreamIdx corresponding to received StreamId */
      if(AvTp_SearchStreamIdx((AVTP_P2CONST(AvTp_StreamIdType))(&StreamHdrPtr->StreamId[0]), &StreamIdx) == E_OK)
      {
        AvTp_DiagnosticCounterListener[StreamIdx].UnsupportedFormat++;

        #if (STD_ON == AVTP_DIAG_WRITE_LISTENER_UNSUPPORTED_FORMAT)
        CANOE_WRITE_STRING1("[DIAG_L:] UnsupportedFormat  [StreamIdx: %d]: %d",
          StreamIdx,
          AvTp_DiagnosticCounterListener[StreamIdx].UnsupportedFormat);
        #endif /* (STD_ON == AVTP_DIAG_WRITE_LISTENER_UNSUPPORTED_FORMAT) */
      }
#endif /* (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT) */
      AvTp_CallDetReportError(AVTP_API_ID_RX_INDICATION, AVTP_E_INV_SUBTYPE)
      return;
    }
  }
  return;
} /* AvTp_RxIndication */
/* PRQA L: RET_PATH */
/* PRQA L: STCYC */
/* PRQA L: STPTH */
/* PRQA L: STPAR */

/**********************************************************************************************************************
 *  AvTp_InitMemory
 *********************************************************************************************************************/
/**
  \brief          Memory initialization of AvTp module.
  \param          none
  \return         none
  \context        This function can be called in any context.
  \note           This function has to be called before AvTp_Init.
 *********************************************************************************************************************/
FUNC(void, AVTP_CODE) AvTp_InitMemory(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  AvTp_CtrlIdxType AvTpCtrlIdx = AVTP_CTRL_CNT; /* PRQA S 0781 */ /* MD_AvTp_5.6 */
#if (AVTP_STREAM_CNT_TX > 0)
  AvTp_StreamIdxType StreamIdx; /* PRQA S 0781 */ /* MD_AvTp_5.6 */
#endif /* (AVTP_STREAM_CNT_TX > 0) */

  /* ----- Implementation ----------------------------------------------- */
#if (AVTP_STREAM_CNT_RX > 0)
  #if (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT)
  /* Init Rx Streams Management */
  StreamIdx = AVTP_STREAM_CNT_RX;

  while(0u < StreamIdx)
  {
    StreamIdx--;

    AvTp_RxStreamMgmt[StreamIdx].SequenceNum = 0;
    AvTp_RxStreamMgmt[StreamIdx].MediaClockReset = FALSE;
    AvTp_RxStreamMgmt[StreamIdx].IsUncertainTimestamp = FALSE;

    /* Init Diagnostic Counter */
    AvTp_DiagnosticCounterListener[StreamIdx].SeqNumMismatch = 0;
    AvTp_DiagnosticCounterListener[StreamIdx].MediaReset = 0;
    AvTp_DiagnosticCounterListener[StreamIdx].TimestampUncertain = 0;
    AvTp_DiagnosticCounterListener[StreamIdx].TimestampValid = 0;
    AvTp_DiagnosticCounterListener[StreamIdx].TimestampNotValid = 0;
    AvTp_DiagnosticCounterListener[StreamIdx].UnsupportedFormat = 0;
    AvTp_DiagnosticCounterListener[StreamIdx].FramesRx = 0;
    AvTp_DiagnosticCounterListener[StreamIdx].LateTimestamp = 0;
    AvTp_DiagnosticCounterListener[StreamIdx].FirstFrame = TRUE;
  }
  #endif /* (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT) */
#endif /* (AVTP_STREAM_CNT_RX > 0) */

#if (AVTP_STREAM_CNT_TX > 0)
  /* Init Tx Streams Management */
  StreamIdx = AVTP_STREAM_CNT_TX;

  while(0u < StreamIdx)
  {
    StreamIdx--;
    AvTp_TxStreamMgmt[StreamIdx].BufIdx = AVTP_BUF_IDX_INVALID;
    AvTp_TxStreamMgmt[StreamIdx].LenByte = 0;
    AvTp_TxStreamMgmt[StreamIdx].BufPtr = NULL_PTR;
    AvTp_TxStreamMgmt[StreamIdx].SequenceNum = 0;
    AvTp_TxStreamMgmt[StreamIdx].Mr = FALSE;
    AvTp_TxStreamMgmt[StreamIdx].Event = 0;
  #if (STD_ON == AVTP_61883IIDC_SUPPORT)
    AvTp_TxStreamMgmt[StreamIdx].Sy = 0;
  #endif /* (STD_ON == AVTP_61883IIDC_SUPPORT) */
  #if (STD_ON == AVTP_AAF_SUPPORT)
    AvTp_TxStreamMgmt[StreamIdx].SampleCount = 0;
    AvTp_TxStreamMgmt[StreamIdx].Busy = FALSE;
  #endif /* (STD_ON == AVTP_AAF_SUPPORT) */
  #if (STD_ON == AVTP_CVF_SUPPORT)
    AvTp_TxStreamMgmt[StreamIdx].M = FALSE;
  #endif /* (STD_ON == AVTP_CVF_SUPPORT) */
  #if (STD_ON == AVTP_CRF_SUPPORT)
    AvTp_TxStreamMgmt[StreamIdx].FS = FALSE;
  #endif /* (STD_ON == AVTP_CRF_SUPPORT) */
  #if (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT)
    AvTp_TxStreamMgmt[StreamIdx].MediaClockResetLast = FALSE;
    AvTp_TxStreamMgmt[StreamIdx].IsUncertainTimestampLast = FALSE;
  #endif /* (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT) */

  #if (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT)
    /* Init Diagnostic Counter */
    AvTp_DiagnosticCounterTalker[StreamIdx].MediaReset = 0;
    AvTp_DiagnosticCounterTalker[StreamIdx].TimestampUncertain = 0;
    AvTp_DiagnosticCounterTalker[StreamIdx].TimestampValid = 0;
    AvTp_DiagnosticCounterTalker[StreamIdx].TimestampNotValid = 0;
    AvTp_DiagnosticCounterTalker[StreamIdx].FramesTx = 0;
  #endif /* (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT) */
  }
#endif /* (AVTP_STREAM_CNT_TX > 0) */

  /* Init AvTp Controller */
  while(0u < AvTpCtrlIdx)
  {
    AvTpCtrlIdx--;
    AvTp_State[AvTpCtrlIdx] = AVTP_STATE_UNINIT;
  }
} /* AvTp_InitMemory */

/**********************************************************************************************************************
 *  AvTp_Init
 *********************************************************************************************************************/
/**
  \brief          Initialization of AvTp module.
  \param          none
  \return         none
  \context        This function can be called in any context.
  \note           This function has to be called before using the module.
  \pre            Function AvTp_InitMemory() has to be called first.
 *********************************************************************************************************************/
FUNC(void, AVTP_CODE) AvTp_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  AvTp_CtrlIdxType AvTpCtrlIdx = AVTP_CTRL_CNT; /* PRQA S 0781 */ /* MD_AvTp_5.6 */

  /* ----- Implementation ----------------------------------------------- */
  /* Reset / Initialize all AvTp Controller */
  while(0u < AvTpCtrlIdx)
  {
    AvTpCtrlIdx --;
    AvTp_ResetCtrl(AvTpCtrlIdx);
  }
} /* AvTp_Init */

/**********************************************************************************************************************
 *  AvTp_MainFunction
 *********************************************************************************************************************/
/**
  \brief          Handles the periodic tasks of state machines, timers and timeouts of the AvTp module.
  \param          none
  \return         none
  \context        This function can be called in any context.
  \note           This function has to be called periodically.
 *********************************************************************************************************************/
FUNC(void, AVTP_CODE) AvTp_MainFunction(void)
{
  /* ----- Implementation ----------------------------------------------- */
#if (STD_ON == AVTP_MAAP_SUPPORT)
  AvTp_Maap_MainFunction();
#endif /* (STD_ON == AVTP_MAAP_SUPPORT) */
} /* AvTp_MainFunction */

/**********************************************************************************************************************
 *  AvTp_Cbk_TrcvLinkStateChg
 *********************************************************************************************************************/
/**
  \brief      Callback function that indicates a changed transceiver link state
  \param[in]  VCtrlIdx       Index of virtual controller that link status changed
  \param[in]  TrcvLinkState  New link state of the transceiver
  \return     none
  \context    Function must be called from task level
 *********************************************************************************************************************/
FUNC(void, AVTP_CODE) AvTp_Cbk_TrcvLinkStateChg(
  uint8 VCtrlIdx,
  AvTp_LinkStateType TrcvLinkState)
{
  /* ----- Local Variables ---------------------------------------------- */
  CONST (Eth_PhysAddrType, AVTP_CONST) BroadcastMacAddr =
  {
    0xFFU,
    0xFFU,
    0xFFU,
    0xFFU,
    0xFFU,
    0xFFU,
  };

  /* ----- Implementation ----------------------------------------------- */
  if(AVTP_VCTRL_CNT > VCtrlIdx)
  {
    AvTp_CtrlIdxType AvTpCtrlIdx = AvTp_CtrlMap[VCtrlIdx]; /* PRQA S 0781 */ /* MD_AvTp_5.6 */

    /* check if AvTp is using this controller */
    if(AVTP_CTRL_IDX_INVALID != AvTpCtrlIdx)
    {
      /* active link state */
      if(AVTP_LINK_STATE_ACTIVE == TrcvLinkState)
      {
        /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
        AvTp_CheckDetErrorReturnVoid(AVTP_COND_NOT_UNINT(AvTp_State[AvTpCtrlIdx]),
          AVTP_API_ID_CBK_TRCV_LINK_STATE_CHG, AVTP_E_NOT_INITIALIZED )

        /* Enable Promiscuous-Mode to receive all MulticastMac-Messages */
        if(E_OK != EthIf_UpdatePhysAddrFilter(AvTp_Ctrl[AvTpCtrlIdx].VCtrlIdx, BroadcastMacAddr, ETH_ADD_TO_FILTER))
        {
          CANOE_WRITE_STRING1("Enabling Promiscuous-Mode for VCtrlIdx = %d failed!", AvTp_Ctrl[AvTpCtrlIdx].VCtrlIdx);
        }

#if (AVTP_STREAM_CNT_TX > 0)
  #if (STD_ON == AVTP_MAAP_SUPPORT)
        if(0u < AvTp_Ctrl[AvTpCtrlIdx].TxStreamCnt)
        {
          AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_State = AVTP_MAAP_STATE_INITIAL;
          AvTp_State[AvTpCtrlIdx] = AVTP_STATE_LINK_ACTIVE;
        }
        else
        {
          /* Rx-Only-Controller */
          AvTp_State[AvTpCtrlIdx] = AVTP_STATE_TX_READY;
        }
  #else
        AvTp_State[AvTpCtrlIdx] = AVTP_STATE_TX_READY;
  #endif /* (STD_ON == AVTP_MAAP_SUPPORT */
        AvTp_StateChangedFunctions(AvTpCtrlIdx, AvTp_State[AvTpCtrlIdx]);
#endif /* (AVTP_STREAM_CNT_TX > 0) */
      }
      /* link state down */
      else
      {
        /* Disable Promiscuous-Mode to receive all MulticastMac-Messages */
        if(E_OK != EthIf_UpdatePhysAddrFilter(AvTp_Ctrl[AvTpCtrlIdx].VCtrlIdx,
                                              BroadcastMacAddr, ETH_REMOVE_FROM_FILTER))
        {
          CANOE_WRITE_STRING1("Disabling Promiscuous-Mode for VCtrlIdx = %d failed!", AvTp_Ctrl[AvTpCtrlIdx].VCtrlIdx);
        }

        AvTp_ResetCtrl(AvTpCtrlIdx);
      }
    }
  }
} /* AvTp_Cbk_TrcvLinkStateChg */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if (AVTP_STREAM_CNT_TX > 0)
/**********************************************************************************************************************
 *    AvTp_ProvideTxBuffer
 *********************************************************************************************************************/
/**
  \brief          Provides a transmit buffer to a upper layer module.
  \param[in]      StreamIdx   Pointer to the received data
  \param[in]      BufPtr      Pointer to the address where the buffer is placed
  \param[in,out]  LenBytePtr is an in/out parameter.\n
                  [in]  The requested buffer length.\n
                  [out] The actual buffer length reduced by Ethernet header and AvTp header length.
                      The actual buffer length is equal or bigger than the requested payload length
                      as far as the return value is BUFREQ_OK.
  \retval         BUFREQ_OK        Buffer request accomplished successful.
  \retval         BUFREQ_E_NOT_OK  Buffer request not successful. Buffer cannot be accessed.
  \retval         BUFREQ_E_BUSY    Temporarily no buffer available. It's up the requester to retry for a certain time.
  \retval         BUFREQ_E_OVFL    No Buffer of the required length can be provided.
  \context        This function can be called in task context.
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, AVTP_CODE) AvTp_ProvideTxBuffer(
  AvTp_StreamIdxType StreamIdx,
  AVTP_P2VAR(AvTp_DataType) *BufPtr,
  AVTP_P2VAR(uint16) LenBytePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;
  AVTP_P2VAR(AvTp_DataType) EthIfBufferPtr = NULL_PTR;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 6 */ /* MD_MSR_14.3 */
  AvTp_CheckDetErrorReturnValue(AVTP_COND_NOT_UNINT(AvTp_State[AvTp_CfgGetAvTpCtrlIdx(StreamIdx)]),
    AVTP_API_ID_PROVIDE_TX_BUFFER,
    AVTP_E_NOT_INITIALIZED, BUFREQ_E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_PTR(BufPtr), AVTP_API_ID_PROVIDE_TX_BUFFER,
    AVTP_E_INV_POINTER, BUFREQ_E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_TX_STREAM_IDX(StreamIdx),
    AVTP_API_ID_PROVIDE_TX_BUFFER, AVTP_E_INV_STREAM_IDX, BUFREQ_E_NOT_OK)

  /* ----- Implementation ----------------------------------------------- */
  /* check that stream buffer is not used at the moment */
  if(AvTp_TxStreamMgmt[StreamIdx].BufPtr != NULL_PTR)
  {
    CANOE_WRITE_STRING("ProvideTxBuffer(): New request, before successful transmit");
    return BUFREQ_E_BUSY;
  }

  /* add AvTp header to requested Length */
  *LenBytePtr = (uint16)(*LenBytePtr + (AvTp_TxStreamInfo[StreamIdx].StreamCfgPtr->AvTpHdrLen));

  /* check if data fits in one ethernet frame -> no fragmentation allowed */
  if(AvTp_CfgGetMTU(StreamIdx) >= *LenBytePtr)
  {
    /* PRQA S 0310 2 */ /* MD_AvTp_11.4 */
    RetVal = EthIf_ProvideTxBuffer(AvTp_CfgGetVCtrlIdx(StreamIdx),
      (Eth_FrameType) AVTP_FRAME_TYPE,
      AvTp_TxStreamInfo[StreamIdx].FramePriority,
      &AvTp_TxStreamMgmt[StreamIdx].BufIdx,
      (AVTP_P2VAR(Eth_DataType*)) &EthIfBufferPtr,
      LenBytePtr);

    if(BUFREQ_OK == RetVal)
    {
      /* store buffer pointer to place AvTp Header on transmit */
      AvTp_TxStreamMgmt[StreamIdx].BufPtr = EthIfBufferPtr;
      AvTp_TxStreamMgmt[StreamIdx].LenByte = *LenBytePtr;

      /* adjust upper layer buffer pointer and length to AvTp Payload */
      *BufPtr = &EthIfBufferPtr[AvTp_TxStreamInfo[StreamIdx].StreamCfgPtr->AvTpHdrLen];
      *LenBytePtr = (uint16)(*LenBytePtr - (AvTp_TxStreamInfo[StreamIdx].StreamCfgPtr->AvTpHdrLen));
    }
#if defined ( AVTP_CANOE_DEBUG )
    else
    {
      CANOE_WRITE_STRING("ProvideTxBuffer(): Call to EthIf_ProvideTxBuffer did not return BUFREQ_OK");
      CANOE_WRITE_STRING1("                   StreamIdx: %d ", StreamIdx);
      CANOE_WRITE_STRING1("                   LenByte: %d", *LenBytePtr);
      CANOE_WRITE_STRING1("                   RetVal: %d (0:OK,1:NOK,3:BUSY,4:OVFL)", RetVal);
    }
#endif /* defined  AVTP_CANOE_DEBUG */
  }

  return RetVal;
} /* AvTp_ProvideTxBuffer */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  AvTp_Transmit
 *********************************************************************************************************************/
/**
  \brief         Transmits a AvTp stream Pdu.
  \param[in]     StreamIdx      Index of TxStream
  \param[in]     LenByte        Byte count of the data to transmit
  \retval        E_OK           Data transmission was successful\n
  \retval        E_NOT_OK       Data could not be transmitted\n
  \context       Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, AVTP_CODE) AvTp_Transmit(
  AvTp_StreamIdxType StreamIdx,
  uint16 LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  AVTP_P2VAR(AvTp_StreamHdrType) AvTp_StrHdrPtr;
  Std_ReturnType RetVal;
#if (STD_ON == AVTP_TIME_SUPPORT)
  AvTp_GlobalTimestampType GlobalTimestamp;
  AvTp_TimestampType PresentationTimestamp;
  AvTp_SyncStateType SyncState; /* PRQA S 0781 */ /* MD_AvTp_5.6 */
#endif /* (STD_ON == AVTP_TIME_SUPPORT) */

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 5 */ /* MD_MSR_14.3 */
  AvTp_CheckDetErrorReturnValue(AVTP_COND_GREATER_ZERO(LenByte), AVTP_API_ID_TRANSMIT, AVTP_E_INV_LENGTH, E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_TX_READY(AvTp_State[AvTp_CfgGetAvTpCtrlIdx(StreamIdx)]),
    AVTP_API_ID_TRANSMIT, AVTP_E_NOT_TX_READY, E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_TX_STREAM_IDX(StreamIdx),
    AVTP_API_ID_TRANSMIT, AVTP_E_INV_STREAM_IDX, E_NOT_OK)

  /* ----- Implementation ----------------------------------------------- */
  AvTp_StrHdrPtr = (AVTP_P2VAR(AvTp_StreamHdrType)) AvTp_TxStreamMgmt[StreamIdx].BufPtr;

  /* set subtype */
  AvTp_StrHdrPtr->Subtype = AvTp_TxStreamInfo[StreamIdx].StreamCfgPtr->Subtype;

  /* set AvTp version */
  AvTp_StrHdrPtr->SvVerMrSsdTv = AVTP_VERSION << AVTP_HDR_COM_VERSION_BIT_POS;

  /* set as valid stream */
  AVTP_SET_BIT(AvTp_StrHdrPtr->SvVerMrSsdTv, AVTP_HDR_COM_H_BIT_POS);

  /* set Media Clock reset */
  AvTp_StrHdrPtr->SvVerMrSsdTv |= (uint8)((AvTp_TxStreamMgmt[StreamIdx].Mr & 0x01) << AVTP_HDR_STREAM_MR_BIT_POS);

  /* set and increment SequenceNum */
  AvTp_StrHdrPtr->SequenceNum = AvTp_TxStreamMgmt[StreamIdx].SequenceNum;
  AvTp_TxStreamMgmt[StreamIdx].SequenceNum++;

  /* set StreamId */
  IpBase_Copy((AVTP_P2VAR(IpBase_CopyDataType))AvTp_StrHdrPtr->StreamId,
    (AVTP_P2CONSTCFG(IpBase_CopyDataType))AvTp_TxStreamInfo[StreamIdx].StreamId,
    AVTP_STREAM_ID_LEN_BYTE);

#if (STD_ON == AVTP_TIME_SUPPORT)
  {
    Std_ReturnType Success = E_NOT_OK;

    if(AVTP_TIME_SOURCE_NONE != AvTp_TxStreamInfo[StreamIdx].TimeSource)
    {
      /* get current time mod 2^32 */
      Success = AvTp_GetCurrentTime(StreamIdx, &GlobalTimestamp, &SyncState);

      if(Success == E_OK)
      {
        /* add presentation time offset */
        (void)AvTp_TimestampPlusTimediff(&GlobalTimestamp, &AvTp_TxStreamInfo[StreamIdx].PresentationTimeOffset);

        /* calculate: avtp_timestamp = (AS_sec * 10^9 + AS_ns) modulo 2^32 */
        PresentationTimestamp = (AvTp_TimestampType)(((GlobalTimestamp.seconds * AVTP_TIMESTAMP_MAX_NANOSECONDS) +
          GlobalTimestamp.nanoseconds) % 0xFFFFFFFFU);

        /* set time */
        AvTp_StrHdrPtr->Timestamp = AvTp_Htonl(PresentationTimestamp);
        if(AVTP_PORT_STATE_SYNC == SyncState)
        {
          /* set timestamp valid information */
          AVTP_SET_BIT(AvTp_StrHdrPtr->SvVerMrSsdTv, AVTP_HDR_STREAM_TV_BIT_POS);

          /* set timestamp certain uncertain information */
          AvTp_StrHdrPtr->SubtypeSpecificData1Tu = 0u;
        }
        else if(AVTP_PORT_STATE_UNSYNC == SyncState)
        {
          /* set timestamp valid information (to invalid) */
          AVTP_RESET_BIT(AvTp_StrHdrPtr->SvVerMrSsdTv, AVTP_HDR_STREAM_TV_BIT_POS);

          /* set timestamp certain uncertain information */
          AvTp_StrHdrPtr->SubtypeSpecificData1Tu = 0u;
          AVTP_SET_BIT(AvTp_StrHdrPtr->SubtypeSpecificData1Tu, AVTP_HDR_STREAM_TU_BIT_POS);
        }
        else
        {
          /* do nothing */
        }
      }
    }

    if(E_NOT_OK == Success)
    {
      /* set default timestamp as zero */
      AvTp_StrHdrPtr->Timestamp = 0u; /*lint !e572 */

      /* set timestamp as not valid */
      AVTP_RESET_BIT(AvTp_StrHdrPtr->SvVerMrSsdTv, AVTP_HDR_STREAM_TV_BIT_POS);

      /* set timestamp as not certain */
      AvTp_StrHdrPtr->SubtypeSpecificData1Tu = 0u;
      AVTP_SET_BIT(AvTp_StrHdrPtr->SubtypeSpecificData1Tu, AVTP_HDR_STREAM_TU_BIT_POS);
    }
  }
#else
  /* set default timestamp as zero */
  AvTp_StrHdrPtr->Timestamp = 0u; /*lint !e572 */

  /* set timestamp as not valid */
  AVTP_RESET_BIT(AvTp_StrHdrPtr->SvVerMrSsdTv, AVTP_HDR_STREAM_TV_BIT_POS);

  /* set timestamp as not certain */
  AvTp_StrHdrPtr->SubtypeSpecificData1Tu = 0u;
  AVTP_SET_BIT(AvTp_StrHdrPtr->SubtypeSpecificData1Tu, AVTP_HDR_STREAM_TU_BIT_POS);
#endif /* (STD_ON == AVTP_TIME_SUPPORT) */

#if (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT)
  /* Check for media clock reset */
  if(AvTp_TxStreamMgmt[StreamIdx].MediaClockResetLast != (boolean)AVTP_HDR_STREAM_MR(AvTp_StrHdrPtr->SvVerMrSsdTv))
  {
    AvTp_TxStreamMgmt[StreamIdx].MediaClockResetLast = (boolean)AVTP_HDR_STREAM_MR(AvTp_StrHdrPtr->SvVerMrSsdTv);

    AvTp_DiagnosticCounterTalker[StreamIdx].MediaReset++;

    #if (STD_ON == AVTP_DIAG_WRITE_TALKER_MEDIA_RESET)
    CANOE_WRITE_STRING2("[DIAG_T:] MediaReset         [StreamIdx: %d]: %d",
      StreamIdx,
      AvTp_DiagnosticCounterTalker[StreamIdx].MediaReset);
    #endif /* (STD_ON == AVTP_DIAG_WRITE_TALKER_MEDIA_RESET) */
  }

  /* Check for timestamp (in-)valid  */
  if(TRUE == AVTP_HDR_STREAM_TV(AvTp_StrHdrPtr->SvVerMrSsdTv))
  {
    AvTp_DiagnosticCounterTalker[StreamIdx].TimestampValid++;

    #if (STD_ON == AVTP_DIAG_WRITE_TALKER_TIMESTAMP_VALID)
    CANOE_WRITE_STRING2("[DIAG_T:] TimestampValid     [StreamIdx: %d]: %d",
      StreamIdx,
      AvTp_DiagnosticCounterTalker[StreamIdx].TimestampValid);
    #endif /* (STD_ON == AVTP_DIAG_WRITE_TALKER_TIMESTAMP_VALID) */
  }
  else
  {
    AvTp_DiagnosticCounterTalker[StreamIdx].TimestampNotValid++;

    #if (STD_ON == AVTP_DIAG_WRITE_TALKER_TIMESTAMP_NOT_VALID)
    CANOE_WRITE_STRING2("[DIAG_T:] TimestampNotValid  [StreamIdx: %d]: %d",
      StreamIdx,
      AvTp_DiagnosticCounterTalker[StreamIdx].TimestampNotValid);
    #endif /* (STD_ON == AVTP_DIAG_WRITE_TALKER_TIMESTAMP_NOT_VALID) */
  }

  /* Check for timestamp uncertain */
  if(AvTp_TxStreamMgmt[StreamIdx].IsUncertainTimestampLast !=
     AVTP_HDR_STREAM_TU(AvTp_StrHdrPtr->SubtypeSpecificData1Tu))
  {
    AvTp_TxStreamMgmt[StreamIdx].IsUncertainTimestampLast = (boolean)AVTP_HDR_STREAM_TU(
        AvTp_StrHdrPtr->SubtypeSpecificData1Tu);

    AvTp_DiagnosticCounterTalker[StreamIdx].TimestampUncertain++;

    #if (STD_ON == AVTP_DIAG_WRITE_TALKER_TIMESTAMP_UNCERTAIN)
    CANOE_WRITE_STRING2("[DIAG_T:] TimestampUncertain [StreamIdx: %d]: %d",
      StreamIdx,
      AvTp_DiagnosticCounterTalker[StreamIdx].TimestampUncertain);
    #endif /* (STD_ON == AVTP_DIAG_WRITE_TALKER_TIMESTAMP_UNCERTAIN) */
  }
#endif /* (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT) */

  /* Process subtype specific content */
  switch(AvTp_TxStreamInfo[StreamIdx].StreamCfgPtr->Subtype)
  {
#if (STD_ON == AVTP_61883IIDC_SUPPORT)
  case AVTP_SUBTYPE_61883IIDC:
    {
      AVTP_P2VAR(AvTp_61883IIDC_MsgType) AvTp_61883iidcMsgPtr;
      AvTp_61883iidcMsgPtr = (AVTP_P2VAR(AvTp_61883IIDC_MsgType)) &AvTp_StrHdrPtr->SubtypeSpecificData2;

      /* set default gateway info as zero */
      AvTp_StrHdrPtr->SubtypeSpecificData2 = 0u; /* Host to Network conversion of the Byte order not required for 0 */

      /* set stream data length */
      AvTp_61883iidcMsgPtr->StreamDataLen = (uint16)AvTp_Htons(LenByte);

      /* set tag */
      AvTp_61883iidcMsgPtr->TagChannel = (uint8) (AvTp_CfgGetTxIecTag(StreamIdx) << AVTP_61883IIDC_TAG_BIT_POS);

      /* set channel */
      AvTp_61883iidcMsgPtr->TagChannel |= AvTp_CfgGetTxIecChannel(StreamIdx);

      /* set type code */
      AvTp_61883iidcMsgPtr->TcodeSy = (uint8)(AVTP_61883IIDC_TCODE_TALKER << AVTP_61883IIDC_TCODE_BIT_POS);

      /* set sy */
      AvTp_61883iidcMsgPtr->TcodeSy |=  (uint8)(AvTp_TxStreamMgmt[StreamIdx].Sy << AVTP_61883IIDC_SY_BIT_POS);
    }
    break;
#endif /* (STD_ON == AVTP_61883IIDC_SUPPORT) */

#if (STD_ON == AVTP_AAF_SUPPORT)
  case AVTP_SUBTYPE_AAF:
    /* AVTP_SUBTYPE_AAF */
    {
      AVTP_P2VAR(AVTP_AAF_MsgType) AvTp_AafMsgPtr;
      AvTp_AafMsgPtr = (AVTP_P2VAR(AVTP_AAF_MsgType)) &AvTp_StrHdrPtr->SubtypeSpecificData2;

      /* set Format */
      AvTp_AafMsgPtr->Format = AvTp_CfgGetTxAvtpFormat(StreamIdx);

      /* set Nominal Sample Rate */
      AvTp_AafMsgPtr->NsrChPerFrame_High = (uint8)(AvTp_CfgGetTxAvtpNsr(StreamIdx) << AVTP_AAF_NRS_BIT_POS);

      /* set Channels Per Frame */
      AvTp_AafMsgPtr->NsrChPerFrame_High |= (uint8)AVTP_AAF_CHPERFRAME_HIGH(AvTp_CfgGetTxAvtpChanPerFram(StreamIdx));
      AvTp_AafMsgPtr->ChPerFrame_Low = (uint8)AVTP_AAF_CHPERFRAME_LOW(AvTp_CfgGetTxAvtpChanPerFram(StreamIdx));

      /* set Bit Depth */
      AvTp_AafMsgPtr->BitDepth = AvTp_CfgGetTxAvtpBitDepth(StreamIdx);

      /* set Stream Data Length */
      AvTp_AafMsgPtr->StreamDataLen = (uint16)AvTp_Htons(LenByte);

      /* set Sparse timestamp flag */
      AvTp_AafMsgPtr->RSpEvt = (uint8)(AvTp_CfgGetTxAvtpSparseMode(StreamIdx) << AVTP_AAF_SP_BIT_POS);

      /* set Event */
      AvTp_AafMsgPtr->RSpEvt |= (AvTp_TxStreamMgmt[StreamIdx].Event << AVTP_AAF_EVT_BIT_POS);
    }
    break;
#endif /* (STD_ON == AVTP_AAF_SUPPORT) */

#if (STD_ON == AVTP_CVF_SUPPORT)
  case AVTP_SUBTYPE_CVF:
    {
      AVTP_P2VAR(AvTp_CVF_MsgType) AvTp_CvfMsgPtr;
      AvTp_CvfMsgPtr = (AVTP_P2VAR(AvTp_CVF_MsgType))&AvTp_StrHdrPtr->SubtypeSpecificData2;

      /* set format */
      AvTp_CvfMsgPtr->Format = AvTp_CfgGetTxCmpVideoFormat(StreamIdx);

      /* set format subtype */
      AvTp_CvfMsgPtr->FormatSubtype = AvTp_CfgGetTxCmpVideoFormatSubtype(StreamIdx);

      /* set stream data length */
      AvTp_CvfMsgPtr->StreamDataLen = (uint16)AvTp_Htons(LenByte);

      /* set M and Event */
      AvTp_CvfMsgPtr->PtvMEvt = (uint8)(AvTp_TxStreamMgmt[StreamIdx].M << AVTP_CVF_M_BIT_POS);
      AvTp_CvfMsgPtr->PtvMEvt |= (uint8)(AvTp_TxStreamMgmt[StreamIdx].Event << AVTP_CVF_EVT_BIT_POS);

#if ((AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) && (AVTP_CVF_H264_SUPPORT == STD_ON))
      /* Set H264 Timestamp if required */
      if(AvTp_CvfMsgPtr->FormatSubtype == AVTP_CVF_FMTSUBTYPE_H264)
      {
        if(AvTp_TxStreamMgmt[StreamIdx].H264TimestampValid == TRUE)
        {
          AVTP_SET_BIT(AvTp_CvfMsgPtr->PtvMEvt, AVTP_CVF_H264_PTV_BIT_POS);
          AvTp_CvfMsgPtr->H264Timestamp = (uint32)AvTp_Htonl(AvTp_TxStreamMgmt[StreamIdx].H264Timestamp);
          /* Invalidated H264 Timestamp */
          AvTp_TxStreamMgmt[StreamIdx].H264TimestampValid = FALSE;
        }
        else
        {
          AVTP_RESET_BIT(AvTp_CvfMsgPtr->PtvMEvt, AVTP_CVF_H264_PTV_BIT_POS);
          AvTp_CvfMsgPtr->H264Timestamp = 0u;
        }
      }
#endif /* ((AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) && (AVTP_CVF_H264_SUPPORT == STD_ON)) */
    }
    break;
#endif /* (STD_ON == AVTP_CVF_SUPPORT) */

#if (STD_ON == AVTP_CRF_SUPPORT)
# if (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE)
  case AVTP_SUBTYPE_CRF:
    // ToDo: Implement!!!
    {
      AVTP_P2VAR(AvTp_Crf_HdrType) crfHdrPtr;
      crfHdrPtr = (AVTP_P2VAR(AvTp_Crf_HdrType))AvTp_StrHdrPtr;

      /* For a Crf message the following fields have to be set in addition to the common fields already set:
      * fs: frame sync field (0 for Audio Sample Type, Video Frame Sync Type, Machine Cycle Time
      * tu: timing uncertain field
      * type: type field
      * pull: pull field
      * base_frequency: base_frequency field
      * crf_data_length: crf_data_length field
      * timestamp_interval: timestamp_interval field
      */

      /* Set fs Bit */
      if(AvTp_TxStreamMgmt[StreamIdx].FS == TRUE)
      {
        AVTP_SET_BIT(crfHdrPtr->SvVerMrRFsTu, AVTP_CRF_FS_BIT_POS);
      }
      else
      {
        AVTP_RESET_BIT(crfHdrPtr->SvVerMrRFsTu, AVTP_CRF_FS_BIT_POS);
      }

      /* Set Tu bit */
      if(AvTp_TxStreamMgmt[StreamIdx].Tu == TRUE)
      {
        AVTP_SET_BIT(crfHdrPtr->SvVerMrRFsTu, AVTP_CRF_TU_BIT_POS);
      }
      else
      {
        AVTP_RESET_BIT(crfHdrPtr->SvVerMrRFsTu, AVTP_CRF_TU_BIT_POS);
      }

      /* Set type field */
      crfHdrPtr->Type = AvTp_CfgGetTxCrfType(StreamIdx);

      /* Set pull and base_frequency fields */
      crfHdrPtr->PullBaseFreq = (uint32)((uint32)AvTp_CfgGetTxCrfClockMultiplier(StreamIdx) << AVTP_CRF_PULL_BIT_POS);
      crfHdrPtr->PullBaseFreq |= (uint32)AvTp_CfgGetTxCrfClockFrequency(StreamIdx);
      crfHdrPtr->PullBaseFreq = AvTp_Htonl(crfHdrPtr->PullBaseFreq);

      /* Set crf_data_length field */
      crfHdrPtr->CrfDataLength = (uint16)AvTp_Htons(LenByte);

      /* Set timestamp_interval field */
      crfHdrPtr->TimestampInterval = (uint16)AvTp_Htons(AvTp_CfgGetTxCrfClockTimestampInterval(StreamIdx));
    }
    break;
# else
  case AVTP_SUBTYPE_CRF:
    {
      AVTP_P2VAR(AvTp_CRF_Pdu_MsgType) AvTp_CrfMsgPtr;
      AvTp_CrfMsgPtr = (AVTP_P2VAR(AvTp_CRF_Pdu_MsgType))&AvTp_StrHdrPtr->StreamDataLen;

      /* set stream data length */
      AvTp_CrfMsgPtr->StreamDataLen = (uint16)AvTp_Htons(LenByte + (AVTP_HDR_LEN_CRF_VIDEO - AVTP_HDR_LEN_STREAM));

      /* set type */
      AvTp_CrfMsgPtr->Type =  AvTp_CfgGetTxCrfType(StreamIdx);

      switch(AvTp_CfgGetTxCrfType(StreamIdx))
      {
      case AVTP_CRF_TYPE_AUDIO_SAMPLE:
        {
          AVTP_P2VAR(AvTp_CRF_Audio_MsgType) AvTp_CrfAudioMsgPtr;
          AvTp_CrfAudioMsgPtr = (AVTP_P2VAR(AvTp_CRF_Audio_MsgType)) &AvTp_StrHdrPtr->StreamDataLen;

          /* set stream data length */
          AvTp_CrfAudioMsgPtr->StreamDataLen = (uint16)AvTp_Htons(LenByte + 2); /* Add 2Byte for CRF-TypeHdr */

          /* set clock frequency */
          AvTp_CrfAudioMsgPtr->ClockFrequency = (uint8)AvTp_CfgGetTxCrfClockFrequency(StreamIdx);

          /* set clock multiplier */
          AvTp_CrfAudioMsgPtr->ClockMultiplier = AvTp_CfgGetTxCrfClockMultiplier(StreamIdx);

          /* set timestamp interval */
          AvTp_CrfAudioMsgPtr->TimestampInterval = (uint16)AvTp_Htons(
              AvTp_CfgGetTxCrfClockTimestampInterval(StreamIdx));
        }
        break;
      case AVTP_CRF_TYPE_VIDEO_FRAME:
        {
          AVTP_P2VAR(AvTp_CRF_VideoFrameSync_MsgType) AvTp_CrfVideoFrameMsgPtr;
          AvTp_CrfVideoFrameMsgPtr = (AVTP_P2VAR(AvTp_CRF_VideoFrameSync_MsgType)) &AvTp_StrHdrPtr->StreamDataLen;

          /* set stream data length */
          AvTp_CrfVideoFrameMsgPtr->StreamDataLen = (uint16)AvTp_Htons(LenByte + 4); /* Add 4Byte for CRF-TypeHdr */

          /* set clock frequency */
          AvTp_CrfVideoFrameMsgPtr->ClockFrequency = (uint32)AvTp_Htonl(
              AvTp_CfgGetTxCrfClockFrequency(StreamIdx));

          /* set clock multiplier */
          AvTp_CrfVideoFrameMsgPtr->ClockMultiplier = AvTp_CfgGetTxCrfClockMultiplier(StreamIdx);

          /* set timestamp interval */
          AvTp_CrfVideoFrameMsgPtr->TimestampInterval = (uint16)AvTp_Htons(
              AvTp_CfgGetTxCrfClockTimestampInterval(StreamIdx));
        }
        break;
      case AVTP_CRF_TYPE_VIDEO_LINE:
        {
          AVTP_P2VAR(AvTp_CRF_VideoLineSync_MsgType) AvTp_CrfVideoLineMsgPtr;
          AvTp_CrfVideoLineMsgPtr = (AVTP_P2VAR(AvTp_CRF_VideoLineSync_MsgType)) &AvTp_StrHdrPtr->StreamDataLen;

          /* set stream data length */
          AvTp_CrfVideoLineMsgPtr->StreamDataLen = (uint16)AvTp_Htons(LenByte + 4); /* Add 4Byte for CRF-TypeHdr */

          /* set clock frequency */
          AvTp_CrfVideoLineMsgPtr->ClockFrequency = (uint32)AvTp_Htonl(AvTp_CfgGetTxCrfClockFrequency(StreamIdx));

          /* set clock multiplier */
          AvTp_CrfVideoLineMsgPtr->ClockMultiplier = AvTp_CfgGetTxCrfClockMultiplier(StreamIdx);

          /* set fs bit */
          AvTp_CrfVideoLineMsgPtr->ResFs = (uint8)(AvTp_TxStreamMgmt[StreamIdx].FS << AVTP_CRF_FS_BIT_POS);

          /* set timestamp interval */
          AvTp_CrfVideoLineMsgPtr->TimestampInterval =(uint16)AvTp_Htons(
              AvTp_CfgGetTxCrfClockTimestampInterval(StreamIdx));
        }
        break;

      default:
        /* required by MISRA */
        break;
      }
    }
    break;
# endif /* (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) */
#endif /* (STD_ON == AVTP_CRF_SUPPORT) */

#if (STD_ON == AVTP_VSF_SUPPORT)
  case AVTP_SUBTYPE_VSF:
    {
      AVTP_P2VAR(AvTp_Vsf_HdrType) AvTp_VsfHdrPtr;
      AvTp_VsfHdrPtr = (AVTP_P2VAR(AvTp_Vsf_HdrType)) AvTp_StrHdrPtr;

      /* set stream data length */
      AvTp_VsfHdrPtr->StreamDataLen = (uint16)AvTp_Htons(LenByte);

      /* set Vendor EUI1 */
      AvTp_VsfHdrPtr->VendorEui1 = (uint32)AvTp_Htonl(AvTp_CfgGetTxVsfEui1(StreamIdx));

      /* set Vendor EUI2 */
      AvTp_VsfHdrPtr->VendorEui2 = (uint16)AvTp_Htons(AvTp_CfgGetTxVsfEui2(StreamIdx));
    }
    break;
#endif /* (STD_ON == AVTP_VSF_SUPPORT) */
  default:
    AvTp_CallDetReportError(AVTP_API_ID_TRANSMIT, AVTP_E_INV_SUBTYPE)
    return E_NOT_OK;
  }

  /* transmit frame */
  RetVal = EthIf_Transmit(
    AvTp_CfgGetVCtrlIdx(StreamIdx),
    AvTp_TxStreamMgmt[StreamIdx].BufIdx,
    (Eth_FrameType) AVTP_FRAME_TYPE,
    FALSE,
    LenByte + AvTp_TxStreamInfo[StreamIdx].StreamCfgPtr->AvTpHdrLen,
#if (STD_ON == AVTP_MAAP_SUPPORT)
    (AVTP_P2VAR(uint8)) AvTp_TxStreamMgmt[StreamIdx].MulticastMacAddr);
#else
    (AVTP_P2VAR(uint8)) AvTp_TxStreamInfo[StreamIdx].DefaultMulticastMacAddr);
#endif /* (STD_ON == AVTP_MAAP_SUPPORT) */

#if (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT)
  if(E_OK == RetVal)
  {
    AvTp_DiagnosticCounterTalker[StreamIdx].FramesTx++;

    #if (STD_ON == AVTP_DIAG_WRITE_TALKER_FRAMES_TX)
    CANOE_WRITE_STRING2("[DIAG_T:] FrameTx            [StreamIdx: %d]: %d",
      StreamIdx,
      AvTp_DiagnosticCounterTalker[StreamIdx].FramesTx);
    #endif /* (STD_ON == AVTP_DIAG_WRITE_TALKER_FRAMES_TX) */
  }
#endif /* (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT) */

#if defined ( AVTP_CANOE_DEBUG )
  if(E_OK != RetVal)
  {
    CANOE_WRITE_STRING("Transmit(): Call to EthIf_Transmit did return E_NOT_OK");
    CANOE_WRITE_STRING1("            StreamIdx: %d", StreamIdx);
    CANOE_WRITE_STRING1("            SubType: %d", AvTp_TxStreamInfo[StreamIdx].StreamCfgPtr->Subtype);
  }
#endif /* defined  AVTP_CANOE_DEBUG */

  /* release buffer */
  AvTp_TxStreamMgmt[StreamIdx].BufPtr = NULL_PTR;

  return RetVal;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6010 */ /* MD_MSR_STPTH */
/* AvTp_Transmit */

/**********************************************************************************************************************
 *  AvTp_SetMr
 *********************************************************************************************************************/
/**
  \brief      Set Media-Clock-Restart-Bit (Mr) which will be transmitted with next sent Pdu of corresponding stream
  \param[in]  StreamIdx      Index of Stream
  \param[in]  Mr             Value of Mr-bit
  \retval     E_OK           Setting Mr-bit was successful\n
  \retval     E_NOT_OK       Mr-bit could not be set\n
  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, AVTP_CODE) AvTp_SetMr(
  AvTp_StreamIdxType StreamIdx,
  boolean Mr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_OK;

  /* ----- Development Error Checks ------------------------------------- */
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_TX_STREAM_IDX(StreamIdx),
    AVTP_API_ID_SETMR, AVTP_E_INV_STREAM_IDX, E_NOT_OK)

  /* ----- Implementation ----------------------------------------------- */
  AvTp_TxStreamMgmt[StreamIdx].Mr = Mr;

  CANOE_WRITE_STRING1("SetMr(): StreamIdx %d", StreamIdx);
  CANOE_WRITE_STRING1("         Mr: %d", Mr);
  CANOE_WRITE_STRING1("         RetVal: %d", RetVal);

  return RetVal;
} /* AvTp_SetMr */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  AvTp_SetEvent
 *********************************************************************************************************************/
/**
  \brief      Set Event which will be transmitted with next sent Pdu
  \param[in]  StreamIdx      Index of Stream
  \param[in]  Event          Event
  \retval     E_OK           Setting Event was successful\n
  \retval     E_NOT_OK       Event could not be set\n
  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, AVTP_CODE) AvTp_SetEvent(
  AvTp_StreamIdxType StreamIdx,
  uint8 Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_TX_STREAM_IDX(StreamIdx),
    AVTP_API_ID_SETEVENT, AVTP_E_INV_STREAM_IDX, E_NOT_OK)

  /* ----- Implementation ----------------------------------------------- */
  /* Check if configured subtype has marker bits */
  switch(AvTp_TxStreamInfo[StreamIdx].StreamCfgPtr->Subtype)
  {
#if (STD_ON == AVTP_AAF_SUPPORT)
  case AVTP_SUBTYPE_AAF:
    /* Set Event */
    AvTp_TxStreamMgmt[StreamIdx].Event = Event;

    RetVal = E_OK;
    break;
#endif /* (STD_ON == AVTP_AAF_SUPPORT) */

#if (STD_ON == AVTP_CVF_SUPPORT)
  case AVTP_SUBTYPE_CVF:
    /* Set Event */
    AvTp_TxStreamMgmt[StreamIdx].Event = Event;

    RetVal = E_OK;
    break;
#endif /* (STD_ON == AVTP_CVF_SUPPORT) */

  default:
    AvTp_CallDetReportError(AVTP_API_ID_SETEVENT, AVTP_E_INV_SUBTYPE)
    break;
  }

  CANOE_WRITE_STRING1("SetEvent(): StreamIdx: %d", StreamIdx);
  CANOE_WRITE_STRING1("            Event: %d", Event);
  CANOE_WRITE_STRING1("            RetVal: %d", RetVal);

  return RetVal;
} /* AvTp_SetEvent */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if (STD_ON == AVTP_61883IIDC_SUPPORT)
/**********************************************************************************************************************
 *  AvTp_SetSy
 *********************************************************************************************************************/
/**
  \brief      Set Sy which will be transmitted with next sent Pdu
  \param[in]  StreamIdx      Index of Stream
  \param[in]  Sy             Sy field
  \retval     E_OK           Setting Sy was successful\n
  \retval     E_NOT_OK       Sy could not be set\n
  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, AVTP_CODE) AvTp_SetSy(
  AvTp_StreamIdxType StreamIdx,
  AvTp_DataType Sy)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_TX_STREAM_IDX(StreamIdx),
    AVTP_API_ID_SETSY, AVTP_E_INV_STREAM_IDX, E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_SUBTYPE_IEC(AvTp_CfgGetTxSubtype(StreamIdx)),
    AVTP_API_ID_SETSY, AVTP_E_INV_SUBTYPE, E_NOT_OK)

  /* ----- Implementation ----------------------------------------------- */
  AvTp_TxStreamMgmt[StreamIdx].Sy = Sy;

  CANOE_WRITE_STRING1("SetSy():  StreamIdx: %d", StreamIdx);
  CANOE_WRITE_STRING1("          Sy: %d", Sy);
  CANOE_WRITE_STRING1("          RetVal: %d", RetVal);

  return RetVal;
} /* AvTp_SetSy */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (STD_ON == AVTP_61883IIDC_SUPPORT) */

#if (STD_ON == AVTP_AAF_SUPPORT)
/**********************************************************************************************************************
 *  AvTp_Aaf_WriteSamples
 *********************************************************************************************************************/
/**
  \brief      Write AVTP Audio samples to Buffer. If the configured number of samples per frame has been written
              the Frame is transmitted. This function can only used with subtype AVTP Audio and other formats than
              USERSPEC.
  \param[in]  StreamIdx      Index of Stream
  \param[in]  SamplePtr      Pointer to current sample frame (containing samples of all channel)
  \retval     E_OK           Writing Sample was successful\n
  \retval     E_NOT_OK       Writing Sample failed\n
  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, AVTP_CODE) AvTp_Aaf_WriteSamples(
  AvTp_StreamIdxType StreamIdx,
  AVTP_P2CONST(AvTp_DataType) SamplePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 Idx; /* PRQA S 0781 */ /* MD_AvTp_5.6 */
  Std_ReturnType RetVal = E_OK;

  /* ----- Development Error Checks ------------------------------------- */
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_TX_STREAM_IDX(StreamIdx),
    AVTP_API_ID_AAF_WRITESAMPLES, AVTP_E_INV_STREAM_IDX, E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_TX_READY(AvTp_State[AvTp_TxStreamInfo[StreamIdx].AvTpCtrlIdx]),
    AVTP_API_ID_AAF_WRITESAMPLES, AVTP_E_NOT_TX_READY, E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_SUBTYPE_AUDIO(AvTp_TxStreamInfo[StreamIdx].StreamCfgPtr->Subtype),
    AVTP_API_ID_AAF_WRITESAMPLES, AVTP_E_INV_SUBTYPE, E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_PTR(SamplePtr),
    AVTP_API_ID_AAF_WRITESAMPLES, AVTP_E_INV_POINTER, E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_NOT_BUSY(AvTp_TxStreamMgmt[StreamIdx].Busy),
    AVTP_API_ID_AAF_WRITESAMPLES, AVTP_E_MULTIPLE_CALLS, E_NOT_OK)

  AvTp_CheckDetErrorReturnValue(AVTP_COND_FORMAT_NOT_USERSPEC(((AVTP_P2CONSTCFG(AvTp_AAF_CfgType))
    AvTp_TxStreamInfo[StreamIdx].StreamCfgPtr)->Format), AVTP_API_ID_AAF_WRITESAMPLES,
    AVTP_AAF_E_INV_FORMAT, E_NOT_OK)

  /* ----- Implementation ----------------------------------------------- */
  /* Set busy flag */
  AvTp_TxStreamMgmt[StreamIdx].Busy = TRUE;

  if(NULL_PTR == AvTp_TxStreamMgmt[StreamIdx].BufPtr)
  {
    /* This is the first sample frame of a new packet -> get Buffer first! */
    AVTP_P2VAR(AvTp_DataType) EthIfBufferPtr = NULL_PTR;
    uint16 LenByte; /* PRQA S 0781 */ /* MD_AvTp_5.6 */

    LenByte = (uint16)(((AVTP_P2CONSTCFG(AvTp_AAF_CfgType))
      AvTp_TxStreamInfo[StreamIdx].StreamCfgPtr)->FrameSize_Byte + AVTP_HDR_LEN_STREAM);

    /* PRQA S 0310 2 */ /* MD_AvTp_11.4 */
    if(BUFREQ_OK == EthIf_ProvideTxBuffer(AvTp_CfgGetVCtrlIdx(StreamIdx),
      (Eth_FrameType) AVTP_FRAME_TYPE,
      AvTp_TxStreamInfo[StreamIdx].FramePriority,
      &AvTp_TxStreamMgmt[StreamIdx].BufIdx,
      (AVTP_P2VAR(Eth_DataType*)) &EthIfBufferPtr,
      &LenByte))
    {
      /* store buffer pointer to place AvTp Header on transmit */
      AvTp_TxStreamMgmt[StreamIdx].BufPtr = EthIfBufferPtr;
      AvTp_TxStreamMgmt[StreamIdx].LenByte = ((AVTP_P2CONSTCFG(AvTp_AAF_CfgType)) AvTp_TxStreamInfo[StreamIdx].
        StreamCfgPtr)->FrameSize_Byte;
      AvTp_TxStreamMgmt[StreamIdx].SampleCount = 0;
    }
    else
    {
      /* no buffer available -> release busy flag and return */
      AvTp_TxStreamMgmt[StreamIdx].Busy = FALSE;
      RetVal = E_NOT_OK;
      return RetVal;
    }
  }

  /* Write current sample frame to buffer */
  /* See 8.2.7, IEEE 1722a-2014-d8 for channel alignment */
  Idx = ((AVTP_P2CONSTCFG(AvTp_AAF_CfgType)) AvTp_TxStreamInfo[StreamIdx].StreamCfgPtr)->ChanPerFram;

  switch(((AVTP_P2CONSTCFG(AvTp_AAF_CfgType)) AvTp_TxStreamInfo[StreamIdx].StreamCfgPtr)->Format)
  {
  case AVTP_AAF_FMT_FLOAT_32BIT:
  case AVTP_AAF_FMT_INT_32BIT:
    {
      uint16 BufStartIdx = (AVTP_HDR_LEN_STREAM +
                            (AvTp_TxStreamMgmt[StreamIdx].SampleCount * AVTP_AAF_32BIT_2BYTE * Idx));
      AVTP_P2CONST(uint32) SamplePtr_32 = ((AVTP_P2CONST(uint32))SamplePtr);
      AVTP_P2VAR(uint32)   BufPtr_32 = ((AVTP_P2VAR(uint32)) &AvTp_TxStreamMgmt[StreamIdx].BufPtr[BufStartIdx]);

      while(0u < Idx)
      {
        Idx--;
        /* Write sample of channel 'Idx' to packet buffer */
        BufPtr_32[Idx] = SamplePtr_32[Idx];
      }
    }
    break;

  case AVTP_AAF_FMT_INT_24BIT:
    {
      uint16 BufStartIdx = (AVTP_HDR_LEN_STREAM +
                            (AvTp_TxStreamMgmt[StreamIdx].SampleCount * AVTP_AAF_24BIT_2BYTE * Idx));
      AVTP_P2CONST(uint32) SamplePtr_32 = ((AVTP_P2CONST(uint32))SamplePtr);
      AVTP_P2VAR(uint8)    BufPtr_8 = &AvTp_TxStreamMgmt[StreamIdx].BufPtr[BufStartIdx];

      while(0u < Idx)
      {
        Idx--;
        /* Write sample of channel 'Idx' to packet buffer */
        BufPtr_8[(Idx * 3) + 0] = (uint8) AVTP_AAF_24BIT_1ST(SamplePtr_32[Idx]);
        BufPtr_8[(Idx * 3) + 1] = (uint8) AVTP_AAF_24BIT_2ND(SamplePtr_32[Idx]);
        BufPtr_8[(Idx * 3) + 2] = (uint8) AVTP_AAF_24BIT_3RD(SamplePtr_32[Idx]);
      }
    }
    break;

  case AVTP_AAF_FMT_INT_16BIT:
    {
      uint16 BufStartIdx = (AVTP_HDR_LEN_STREAM +
                            (AvTp_TxStreamMgmt[StreamIdx].SampleCount * AVTP_AAF_16BIT_2BYTE * Idx));
      AVTP_P2CONST(uint16) SamplePtr_16 = ((AVTP_P2CONST(uint16))SamplePtr);
      AVTP_P2VAR(uint16)   BufPtr_16 = ((AVTP_P2VAR(uint16)) &AvTp_TxStreamMgmt[StreamIdx].BufPtr[BufStartIdx]);

      while(0u < Idx)
      {
        Idx--;
        /* Write sample of channel 'Idx' to packet buffer */
        BufPtr_16[Idx] = SamplePtr_16[Idx];
      }
    }
    break;

  default:
    /* required by MISRA */
    break;
  }

  /* Count written sample frames */
  AvTp_TxStreamMgmt[StreamIdx].SampleCount++;

  /* If configured number of samples is written to buffer -> transmit packet */
  if(((AVTP_P2CONSTCFG(AvTp_AAF_CfgType)) AvTp_TxStreamInfo[StreamIdx].StreamCfgPtr)->SampleFramesPerPacket ==
    AvTp_TxStreamMgmt[StreamIdx].SampleCount)
  {
    RetVal = AvTp_Transmit(StreamIdx, AvTp_TxStreamMgmt[StreamIdx].LenByte);

    /* Reset sample frame counter */
    AvTp_TxStreamMgmt[StreamIdx].SampleCount = 0;
  }

  /* Release busy flag */
  AvTp_TxStreamMgmt[StreamIdx].Busy = FALSE;

  return RetVal;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6010 */ /* MD_MSR_STPTH */
/* AvTp_AVTPAudio_WriteSamples */
#endif /* (STD_ON == AVTP_AAF_SUPPORT) */

#if (STD_ON == AVTP_CVF_SUPPORT)
/**********************************************************************************************************************
 *  AvTp_SetMarker
 *********************************************************************************************************************/
/**
  \brief      Set Marker Bit which will be transmitted with next sent Pdu
  \param[in]  StreamIdx      Index of Stream
  \param[in]  M              Marker Bit
  \retval     E_OK           Setting Marker Bit was successful\n
  \retval     E_NOT_OK       Marker Bit could not be set\n
  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, AVTP_CODE) AvTp_SetMarker(
  AvTp_StreamIdxType StreamIdx,
  boolean M)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_OK;

  /* ----- Development Error Checks ------------------------------------- */
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_TX_STREAM_IDX(StreamIdx),
    AVTP_API_ID_SETMARKER, AVTP_E_INV_STREAM_IDX, E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_SUBTYPE_CVF(AvTp_CfgGetTxSubtype(StreamIdx)),
    AVTP_API_ID_SETMARKER, AVTP_E_INV_SUBTYPE, E_NOT_OK)

  /* ----- Implementation ----------------------------------------------- */
  /* Set Marker Bit */
  AvTp_TxStreamMgmt[StreamIdx].M = M;

  CANOE_WRITE_STRING1("SetMarker(): StreamIdx %d", StreamIdx);
  CANOE_WRITE_STRING1("             Marker: %d", M);
  CANOE_WRITE_STRING1("             RetVal: %d", RetVal);

  return RetVal;
} /* AvTp_SetMarker */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (STD_ON == AVTP_CVF_SUPPORT) */

/**********************************************************************************************************************
 *  AvTp_SetFs
 *********************************************************************************************************************/
/**
  \brief      Set Fs which will be transmitted with next sent Pdu
  \param[in]  StreamIdx      Index of Stream
  \param[in]  Fs             Fs bit
  \retval     E_OK           Setting Fs was successful\n
  \retval     E_NOT_OK       Fs could not be set\n
  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
#if (STD_ON == AVTP_CRF_SUPPORT)
FUNC(Std_ReturnType, AVTP_CODE) AvTp_SetFs(
  AvTp_StreamIdxType StreamIdx,
  boolean Fs)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_OK;

  /* ----- Development Error Checks ------------------------------------- */
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_TX_STREAM_IDX(StreamIdx),
    AVTP_API_ID_SETFS, AVTP_E_INV_STREAM_IDX, E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_SUBTYPE_CRF(AvTp_CfgGetTxSubtype(StreamIdx)),
    AVTP_API_ID_SETFS, AVTP_E_INV_SUBTYPE, E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_TYPE_VIDEO_LINE(StreamIdx),
    AVTP_API_ID_SETFS, AVTP_CRF_E_INV_TYPE, E_NOT_OK)

  /* ----- Implementation ----------------------------------------------- */
  AvTp_TxStreamMgmt[StreamIdx].FS = Fs;

  CANOE_WRITE_STRING1("SetFs():  StreamIdx: %d", StreamIdx);
  CANOE_WRITE_STRING1("          Fs: %d", Fs);
  CANOE_WRITE_STRING1("          RetVal: %d", RetVal);

  return RetVal;
} /* AvTp_SetFs */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (STD_ON == AVTP_CRF_SUPPORT) */

/**********************************************************************************************************************
 *  AvTp_SetTu
 *********************************************************************************************************************/
/**
  \brief      Set Tu for a Clock Reference Format PDU which will be transmitted with next sent Pdu
  \param[in]  StreamIdx      Index of Stream
  \param[in]  Tu             Timing uncertain
  \retval     E_OK           Setting Tu was successful\n
  \retval     E_NOT_OK       Tu could not be set\n
  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
#if (STD_ON == AVTP_CRF_SUPPORT)
# if (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE)
FUNC(Std_ReturnType, AVTP_CODE) AvTp_SetTu(AvTp_StreamIdxType StreamIdx,boolean Tu)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = AVTP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (AVTP_DEV_ERROR_DETECT == STD_ON)
  if(StreamIdx >= AVTP_STREAM_CNT_TX)
  {
    errorId = AVTP_E_INV_STREAM_IDX;
  }
  else if(AvTp_CfgGetTxSubtype(StreamIdx) != AVTP_SUBTYPE_CRF)
  {
    errorId = AVTP_E_INV_SUBTYPE;
  }
  else
# endif /*  */
  {
    /* ----- Implementation --------------------------------------------- */
    AvTp_TxStreamMgmt[StreamIdx].Tu = Tu;
    retVal = E_OK;
  }

  /* ----- Development Error Report ------------------------------------- */
# if (AVTP_DEV_ERROR_REPORT == STD_ON)
  if(errorId != AVTP_E_NO_ERROR)
  {
    AvTp_CallDetReportError(AVTP_API_ID_SET_TU, errorId); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
# else
  AVTP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif /* (AVTP_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
} /* AvTp_SetTu() */
# endif /* (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) */
#endif /* (STD_ON == AVTP_CRF_SUPPORT) */

/**********************************************************************************************************************
 *  AvTp_SetH264Timestamp
 *********************************************************************************************************************/
/**
  \brief      Set H264 Timestamp for a Compressed Video Format H264 PDU which will be transmitted with next sent Pdu\n
  \param[in]  StreamIdx         Index of Stream\n
  \param[in]  H264TimestampPtr  Pointer to the H264 Timestamp\n
  \retval     E_OK              Setting the H264 Timestamp was successful\n
  \retval     E_NOT_OK          H264 Timestamp could not be set\n
  \context    Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
#if (AVTP_CVF_H264_SUPPORT == STD_ON)
# if (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE)
FUNC(Std_ReturnType, AVTP_CODE) AvTp_SetH264Timestamp(AvTp_StreamIdxType StreamIdx,
  AVTP_P2CONST(uint32) H264TimestampPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = AVTP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (AVTP_DEV_ERROR_DETECT == STD_ON)
  if(StreamIdx >= AVTP_STREAM_CNT_TX)
  {
    errorId = AVTP_E_INV_STREAM_IDX;
  }
  else if (H264TimestampPtr == NULL_PTR)
  {
    errorId = AVTP_E_INV_POINTER;
  }
  else
#endif /* (AVTP_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation --------------------------------------------- */
    AvTp_TxStreamMgmt[StreamIdx].H264Timestamp = *H264TimestampPtr;
    AvTp_TxStreamMgmt[StreamIdx].H264TimestampValid = TRUE;
    retVal = E_OK;
  }

  /* ----- Development Error Report ------------------------------------- */
# if (AVTP_DEV_ERROR_REPORT == STD_ON)
  if(errorId != AVTP_E_NO_ERROR)
  {
    AvTp_CallDetReportError(AVTP_API_ID_SET_H264_TIMESTAMP, errorId); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
# else
  AVTP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif /* (AVTP_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
} /* AvTp_SetH264Timestamp() */
# endif /* (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) */
#endif /* (AVTP_CVF_H264_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  AvTp_GetTxStreamConfig
 *********************************************************************************************************************/
/**
  \brief       Get a pointer to configured values of corresponding TxStream
  \param[in]   StreamIdx      Index of Stream
  \param[out]  StreamCfgPtr   Pointer to config of corresponding values
  \retval      E_OK           Pointer is valid\n
  \retval      E_NOT_OK       Pointer is not valid\n
  \context     Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, AVTP_CODE) AvTp_GetTxStreamConfig(
  AvTp_StreamIdxType StreamIdx,
  AVTP_P2VAR(AVTP_P2CONSTCFG(AvTp_StreamCfgType)) StreamCfgPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_OK;

  /* ----- Development Error Checks ------------------------------------- */
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_TX_STREAM_IDX(StreamIdx),
    AVTP_API_ID_GET_TX_STREAM_CONFIG, AVTP_E_INV_STREAM_IDX, E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_PTR(StreamCfgPtr),
    AVTP_API_ID_GET_TX_STREAM_CONFIG, AVTP_E_INV_STREAM_IDX, E_NOT_OK)

  /* ----- Implementation ----------------------------------------------- */
  *StreamCfgPtr = (AVTP_P2CONSTCFG(AvTp_StreamCfgType))(AvTp_TxStreamInfo[StreamIdx].StreamCfgPtr);

  return RetVal;
} /* AvTp_GetTxStreamConfig */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (AVTP_STREAM_CNT_TX > 0) */

#if (0 < AVTP_STREAM_CNT_RX)
/**********************************************************************************************************************
 *  AvTp_GetRxStreamConfig
 *********************************************************************************************************************/
/**
  \brief       Get a pointer to configured values of corresponding RxStream
  \param[in]   StreamIdx      Index of Stream
  \param[out]  StreamCfgPtr   Pointer to config of corresponding values
  \retval      E_OK           Pointer is valid\n
  \retval      E_NOT_OK       Pointer is not valid\n
  \context     Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, AVTP_CODE) AvTp_GetRxStreamConfig(
  AvTp_StreamIdxType StreamIdx,
  AVTP_P2VAR(AVTP_P2CONSTCFG(AvTp_StreamCfgType)) StreamCfgPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_OK;

  /* ----- Development Error Checks ------------------------------------- */
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_RX_STREAM_IDX(StreamIdx),
    AVTP_API_ID_GET_RX_STREAM_CONFIG, AVTP_E_INV_STREAM_IDX, E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_PTR(StreamCfgPtr),
    AVTP_API_ID_GET_RX_STREAM_CONFIG, AVTP_E_INV_STREAM_IDX, E_NOT_OK)

  /* ----- Implementation ----------------------------------------------- */
  *StreamCfgPtr = (AVTP_P2CONSTCFG(AvTp_StreamCfgType))(AvTp_RxStreamInfo[StreamIdx].StreamCfgPtr);

  return RetVal;
} /* AvTp_GetRxStreamConfig */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (0 < AVTP_STREAM_CNT_RX) */

#if (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT)
/**********************************************************************************************************************
 *  AvTp_ReadDiagnosticCounterListener
 *********************************************************************************************************************/
/**
  \brief       Readout current state of a Diagnostic Counter for Listener stream
  \param[in]   StreamIdx                      Index of Rx-Stream
  \param[out]  DiagnosticCounterListenerPtr   Pointer to set of Diagnostic Counters of a Listener stream
  \retval      E_OK                           Pointer is valid\n
  \retval      E_NOT_OK                       Pointer is not valid\n
  \context     Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, AVTP_CODE) AvTp_ReadDiagnosticCounterListener(
  AvTp_StreamIdxType StreamIdx,
  AVTP_P2VAR(AVTP_P2VAR(AvTp_DiagnosticCounterListenerType)) DiagnosticCounterListenerPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_OK;

  /* ----- Development Error Checks ------------------------------------- */
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_RX_STREAM_IDX(StreamIdx),
    AVTP_API_ID_READ_DIAGNOSTIC_COUNTER_LISTENER, AVTP_E_INV_STREAM_IDX, E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_PTR(DiagnosticCounterListenerPtr),
    AVTP_API_ID_READ_DIAGNOSTIC_COUNTER_LISTENER, AVTP_E_INV_STREAM_IDX, E_NOT_OK)

  /* ----- Implementation ----------------------------------------------- */
  *DiagnosticCounterListenerPtr = (AVTP_P2VAR(AvTp_DiagnosticCounterListenerType))
    (&AvTp_DiagnosticCounterListener[StreamIdx]);

  return RetVal;
} /* AvTp_ReadDiagnosticCounterListener */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  AvTp_ReadDiagnosticCounterTalker
 *********************************************************************************************************************/
/**
  \brief       Readout current state of a Diagnostic Counter for Talker stream
  \param[in]   StreamIdx                    Index of Tx-Stream
  \param[out]  DiagnosticCounterTalkerPtr   Pointer to set of Diagnostic Counters of a Talker stream
  \retval      E_OK                         Pointer is valid\n
  \retval      E_NOT_OK                     Pointer is not valid\n
  \context     Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, AVTP_CODE) AvTp_ReadDiagnosticCounterTalker(
  AvTp_StreamIdxType StreamIdx,
  AVTP_P2VAR(AVTP_P2VAR(AvTp_DiagnosticCounterTalkerType)) DiagnosticCounterTalkerPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_OK;

  /* ----- Development Error Checks ------------------------------------- */
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_TX_STREAM_IDX(StreamIdx),
    AVTP_API_ID_READ_DIAGNOSTIC_COUNTER_TALKER, AVTP_E_INV_STREAM_IDX, E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_PTR(DiagnosticCounterTalkerPtr),
    AVTP_API_ID_READ_DIAGNOSTIC_COUNTER_TALKER, AVTP_E_INV_STREAM_IDX, E_NOT_OK)

  /* ----- Implementation ----------------------------------------------- */
  *DiagnosticCounterTalkerPtr = (AVTP_P2VAR(AvTp_DiagnosticCounterTalkerType))
    (&AvTp_DiagnosticCounterTalker[StreamIdx]);

  return RetVal;
} /* AvTp_ReadDiagnosticCounterTalker */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  AvTp_ResetDiagnosticCounter
 *********************************************************************************************************************/
/**
  \brief       Reset all Diagnostic Counters of a Listener or Talker stream
  \param[in]   StreamIdx    Index of Stream
  \param[in]   IsListener   TRUE:  StreamIdx is a Listener(Rx)-StreamIdx
                            FALSE: StreamIdx is a Talker(Tx)-StreamIdx
  \retval      E_OK         Reset has been successful \n
  \retval      E_NOT_OK     Reset has failed \n
  \context     Function could be called from interrupt level or from task level
 *********************************************************************************************************************/
FUNC(Std_ReturnType, AVTP_CODE) AvTp_ResetDiagnosticCounter(
  AvTp_StreamIdxType StreamIdx,
  boolean IsListener)
{
    /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_OK;

  if(TRUE == IsListener)
  {
    /* ----- Development Error Checks ------------------------------------- */
    AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_RX_STREAM_IDX(StreamIdx),
      AVTP_API_ID_RESET_DIAGNOSTIC_COUNTER, AVTP_E_INV_STREAM_IDX, E_NOT_OK)

    /* ----- Implementation ----------------------------------------------- */
    AvTp_DiagnosticCounterListener[StreamIdx].SeqNumMismatch = 0;
    AvTp_DiagnosticCounterListener[StreamIdx].MediaReset = 0;
    AvTp_DiagnosticCounterListener[StreamIdx].TimestampUncertain = 0;
    AvTp_DiagnosticCounterListener[StreamIdx].TimestampValid = 0;
    AvTp_DiagnosticCounterListener[StreamIdx].TimestampNotValid = 0;
    AvTp_DiagnosticCounterListener[StreamIdx].UnsupportedFormat = 0;
    AvTp_DiagnosticCounterListener[StreamIdx].FramesRx = 0;
    AvTp_DiagnosticCounterListener[StreamIdx].FirstFrame = TRUE;
  }
  else
  {
    /* ----- Development Error Checks ------------------------------------- */
    AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_TX_STREAM_IDX(StreamIdx),
      AVTP_API_ID_RESET_DIAGNOSTIC_COUNTER, AVTP_E_INV_STREAM_IDX, E_NOT_OK)

    /* ----- Implementation ----------------------------------------------- */
    AvTp_DiagnosticCounterTalker[StreamIdx].MediaReset = 0;
    AvTp_DiagnosticCounterTalker[StreamIdx].TimestampUncertain = 0;
    AvTp_DiagnosticCounterTalker[StreamIdx].TimestampValid = 0;
    AvTp_DiagnosticCounterTalker[StreamIdx].TimestampNotValid = 0;
    AvTp_DiagnosticCounterTalker[StreamIdx].FramesTx = 0;
  }
  return RetVal;
} /* AvTp_ResetDiagnosticCounter */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AvTp_ResetCtrl
 *********************************************************************************************************************/
STATIC FUNC(void, AVTP_CODE) AvTp_ResetCtrl(AvTp_CtrlIdxType AvTpCtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (AVTP_STREAM_CNT_TX > 0)
  AvTp_StreamIdxType StreamIdx; /* PRQA S 0781 */ /* MD_AvTp_5.6 */
#endif /* (AVTP_STREAM_CNT_TX > 0) */

  /* ----- Implementation ----------------------------------------------- */
  if(0 == AvTp_Ctrl[AvTpCtrlIdx].TxStreamCnt)
  {
    /* Rx-Only-Controller -> do nothing */
    return;
  }

#if (AVTP_STREAM_CNT_TX > 0)
  /* Reset all TxStreams configured for this AvTpCtrl */
  StreamIdx = AvTp_Ctrl[AvTpCtrlIdx].TxStreamCnt;

  while(0u < StreamIdx)
  {
    StreamIdx--;

    if(AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].BufPtr != NULL_PTR)
    {
      /* release buffer */
      if(E_OK != EthIf_Transmit(AvTp_Ctrl[AvTpCtrlIdx].VCtrlIdx,
        AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].BufIdx,
        (Eth_FrameType) AVTP_FRAME_TYPE,
        FALSE,
        0,
# if (STD_ON == AVTP_MAAP_SUPPORT)
        (AVTP_P2VAR(uint8)) AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].MulticastMacAddr))
# else
        (AVTP_P2VAR(uint8)) AvTp_TxStreamInfo[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].DefaultMulticastMacAddr))
# endif /* (STD_ON == AVTP_MAAP_SUPPORT) */
      {
        ;
      }
      /* Tx Stream Management */
      AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].BufIdx = AVTP_BUF_IDX_INVALID;
      AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].LenByte = 0;
      AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].BufPtr = NULL_PTR;
      AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].SequenceNum = 0;
      AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].Mr = FALSE;
      AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].Event = 0;
# if (STD_ON == AVTP_61883IIDC_SUPPORT)
      AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].Sy = 0;
# endif /* (STD_ON == AVTP_61883IIDC_SUPPORT) */
# if (STD_ON == AVTP_AAF_SUPPORT)
      AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].SampleCount = 0;
      AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].Busy = FALSE;
# endif /* (STD_ON == AVTP_AAF_SUPPORT) */
# if (STD_ON == AVTP_CVF_SUPPORT)
      AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].M = FALSE;
# endif /* (STD_ON == AVTP_CVF_SUPPORT) */
# if (STD_ON == AVTP_CRF_SUPPORT)
      AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].FS = FALSE;
# endif /* (STD_ON == AVTP_CRF_SUPPORT) */
# if (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT)
      AvTp_TxStreamMgmt[StreamIdx].MediaClockResetLast = FALSE;
      AvTp_TxStreamMgmt[StreamIdx].IsUncertainTimestampLast = FALSE;
# endif /* (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT) */

# if (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT)
      /* Diagnostic Counter */
      AvTp_DiagnosticCounterTalker[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].MediaReset = 0;
      AvTp_DiagnosticCounterTalker[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].TimestampUncertain = 0;
      AvTp_DiagnosticCounterTalker[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].TimestampValid = 0;
      AvTp_DiagnosticCounterTalker[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].TimestampNotValid = 0;
      AvTp_DiagnosticCounterTalker[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[StreamIdx]].FramesTx = 0;
# endif /* (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT) */
    }
  }

# if (STD_ON == AVTP_MAAP_SUPPORT)
  /* Reset MAAP-StateMachine for this AvTp Controller */
  if(NULL_PTR != AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr)
  {
    AvTp_Maap_Init(AvTpCtrlIdx);
    AvTp_State[AvTpCtrlIdx] = AVTP_STATE_INIT;
  }
  else
  {
    /* Controller has only Rx-Streams configured */
    AvTp_State[AvTpCtrlIdx] = AVTP_STATE_TX_READY;
  }
# else
  AvTp_State[AvTpCtrlIdx] = AVTP_STATE_INIT;
# endif /* (STD_ON == AVTP_MAAP_SUPPORT ) */

  /* Call State-Change function to inform upper-layer */
  AvTp_StateChangedFunctions(AvTpCtrlIdx, AvTp_State[AvTpCtrlIdx]);
#endif /* (AVTP_STREAM_CNT_TX > 0) */

#if (AVTP_STREAM_CNT_RX > 0)
# if (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT)
  /* Reset all TxStreams configured for this AvTpCtrl */
  StreamIdx = AvTp_Ctrl[AvTpCtrlIdx].RxStreamCnt;

  while(0u < StreamIdx)
  {
    StreamIdx--;

    AvTp_RxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].RxStreamPtr[StreamIdx]].SequenceNum = 0;
    AvTp_RxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].RxStreamPtr[StreamIdx]].MediaClockReset = FALSE;
    AvTp_RxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].RxStreamPtr[StreamIdx]].IsUncertainTimestamp = FALSE;

    /* Diagnostic Counter */
    AvTp_DiagnosticCounterListener[AvTp_Ctrl[AvTpCtrlIdx].RxStreamPtr[StreamIdx]].SeqNumMismatch = 0;
    AvTp_DiagnosticCounterListener[AvTp_Ctrl[AvTpCtrlIdx].RxStreamPtr[StreamIdx]].MediaReset = 0;
    AvTp_DiagnosticCounterListener[AvTp_Ctrl[AvTpCtrlIdx].RxStreamPtr[StreamIdx]].TimestampUncertain = 0;
    AvTp_DiagnosticCounterListener[AvTp_Ctrl[AvTpCtrlIdx].RxStreamPtr[StreamIdx]].TimestampValid = 0;
    AvTp_DiagnosticCounterListener[AvTp_Ctrl[AvTpCtrlIdx].RxStreamPtr[StreamIdx]].TimestampNotValid = 0;
    AvTp_DiagnosticCounterListener[AvTp_Ctrl[AvTpCtrlIdx].RxStreamPtr[StreamIdx]].UnsupportedFormat = 0;
    AvTp_DiagnosticCounterListener[AvTp_Ctrl[AvTpCtrlIdx].RxStreamPtr[StreamIdx]].FramesRx = 0;
    AvTp_DiagnosticCounterListener[AvTp_Ctrl[AvTpCtrlIdx].RxStreamPtr[StreamIdx]].LateTimestamp = 0;
    AvTp_DiagnosticCounterListener[AvTp_Ctrl[AvTpCtrlIdx].RxStreamPtr[StreamIdx]].FirstFrame = TRUE;
  }
# endif /* (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT) */
#endif /* (AVTP_STREAM_CNT_RX > 0) */
} /* AvTp_ResetCtrl */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if (STD_ON == AVTP_MAAP_SUPPORT)
/**********************************************************************************************************************
 *  AvTp_Maap_Init
 *********************************************************************************************************************/
STATIC FUNC(void, AVTP_CODE) AvTp_Maap_Init(AvTp_CtrlIdxType AvTpCtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 Idx; /* PRQA S 0781 */ /* MD_AvTp_5.6 */

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize MAAP state variables */
  AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_State = AVTP_MAAP_STATE_INIT;
  AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ProbeInterval = AVTP_MAAP_PROBE_INTERVAL;
  AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_AnnounceInterval = AVTP_MAAP_ANNOUNCE_INTERVAL;
  AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ProbeCount = AVTP_MAAP_PROBE_RETRANSMITS;

  Idx = AvTp_Ctrl[AvTpCtrlIdx].TxStreamCnt;

  while(0u < Idx)
  {
    Idx--;
    /* Copy Maap Start Address of Dynamic Allocation Pool to Base-MulticastMacAddress */
    IpBase_Copy((AVTP_P2VAR(IpBase_CopyDataType))
      AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[Idx]].MulticastMacAddr,
      (AVTP_P2CONST(IpBase_CopyDataType)) Maap_DynAllocPoolStartAddr,
      ETH_PHYS_ADDR_LEN_BYTE);
  }
} /* AvTp_Maap_Init */

/**********************************************************************************************************************
 *  AvTp_Maap_MainFunction
 *********************************************************************************************************************/
STATIC FUNC(void, AVTP_CODE) AvTp_Maap_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  AvTp_CtrlIdxType AvTpCtrlIdx = AVTP_CTRL_CNT; /* PRQA S 0781 */ /* MD_AvTp_5.6 */

  /* ----- Implementation ----------------------------------------------- */
  while(0u < AvTpCtrlIdx)
  {
    AvTpCtrlIdx--;

    /* Check if AvTpCtrl has Tx Streams */
    if(0 < AvTp_Ctrl[AvTpCtrlIdx].TxStreamCnt)
    {
      /* decrement maap probe interval */
      if(0u < AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ProbeInterval)
      {
        AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ProbeInterval--;
      }

      /* decrement maap announce interval */
      if(0u < AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_AnnounceInterval)
      {
        AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_AnnounceInterval--;
      }

      AVTP_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      /* process states of state machine */
      if(AVTP_MAAP_STATE_INITIAL == AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_State)
      {
        AvTp_State[AvTpCtrlIdx] = AVTP_STATE_INIT;
        AvTp_StateChangedFunctions(AvTpCtrlIdx, AvTp_State[AvTpCtrlIdx]);

        /* Generate MulticastMacAddresses for all TxStreams on this AvTpCtrl */
        AvTp_Maap_GenerateDynamicAddr(AvTpCtrlIdx);
        AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ProbeCount = AVTP_MAAP_PROBE_RETRANSMITS;
        AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ProbeInterval = AVTP_MAAP_PROBE_INTERVAL;
        AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_State = AVTP_MAAP_STATE_PROBE;
      }

      if(AVTP_MAAP_STATE_PROBE == AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_State)
      {
        if(0u == AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ProbeInterval)
        {
          AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ProbeInterval = AVTP_MAAP_PROBE_INTERVAL;
          AvTp_Maap_TxProbe(AvTpCtrlIdx);
          AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ProbeCount--;
        }
        /* all probe messages are transmitted */
        if(0u == AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ProbeCount)
        {
          AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ProbeInterval = 0;
          AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_State = AVTP_MAAP_STATE_DEFEND;
          AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_AnnounceInterval = AVTP_MAAP_ANNOUNCE_INTERVAL;
          /* announce message must be transmitted immediately */
          AvTp_Maap_TxAnnounce(AvTpCtrlIdx);
          /* set AvTp in Tx ready state */
          AvTp_State[AvTpCtrlIdx] = AVTP_STATE_TX_READY;
          AvTp_StateChangedFunctions(AvTpCtrlIdx, AvTp_State[AvTpCtrlIdx]);
        }
      }

      if(AVTP_MAAP_STATE_DEFEND == AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_State)
      {
        if(0u == AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_AnnounceInterval)
        {
          AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_AnnounceInterval = AVTP_MAAP_ANNOUNCE_INTERVAL;
          AvTp_Maap_TxAnnounce(AvTpCtrlIdx);
        }
      }
      AVTP_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
    else
    {/* AvTpCtrl has only Rx Streams */
      if( AVTP_STATE_INIT != AvTp_State[AvTpCtrlIdx])
      {
        /* set AvTp in Tx ready state */
        AvTp_State[AvTpCtrlIdx] = AVTP_STATE_INIT;
        AvTp_StateChangedFunctions(AvTpCtrlIdx, AvTp_State[AvTpCtrlIdx]);
      }
    }
  }
} /* AvTp_Maap_MainFunction */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6010 */ /* MD_MSR_STPTH */

/**********************************************************************************************************************
 *  AvTp_Maap_RxIndication
 *********************************************************************************************************************/
STATIC FUNC(void, AVTP_CODE) AvTp_Maap_RxIndication(AvTp_CtrlIdxType AvTpCtrlIdx,
  AVTP_P2VAR(uint8) PhysSrcAddrPtr,
  AVTP_P2VAR(AvTp_DataType) DataPtr,
  uint16 LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  AVTP_P2VAR(AvTp_Maap_MsgType) AvTp_Maap_MsgPtr;
  boolean IsMacConflict;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  AvTp_CheckDetErrorReturnVoid(AVTP_COND_VALID_PTR(AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr),
    AVTP_MAAP_API_ID_RX_INDICATION, AVTP_E_INV_POINTER)
  AvTp_CheckDetErrorReturnVoid(AVTP_COND_NOT_UNINT(AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_State),
    AVTP_MAAP_API_ID_RX_INDICATION, AVTP_MAAP_E_NOT_INITIALIZED)
  AvTp_CheckDetErrorReturnVoid(AVTP_COND_MAAP_MIN_LEN(LenByte), AVTP_MAAP_API_ID_RX_INDICATION, AVTP_MAAP_E_INV_LENGTH)

  /* ----- Implementation ----------------------------------------------- */
  AvTp_Maap_MsgPtr = (AVTP_P2VAR(AvTp_Maap_MsgType)) DataPtr; /* PRQA S 0310 */ /* MD_AvTp_11.4 */
  /* sanity check of msg values */
  if((AVTP_MAAP_VERSION > AVTP_MAAP_HDR_VERSION(AvTp_Ntohs(AvTp_Maap_MsgPtr->MaapVerDataLen)))||
     (AVTP_MAAP_DATA_LEN != AVTP_MAAP_HDR_DATA_LEN(AvTp_Ntohs(AvTp_Maap_MsgPtr->MaapVerDataLen))))
  {
    /* msg attributes are not valid -> drop message */
    return;
  }

  /* check for existing mac address conflict */
  IsMacConflict = AvTp_Maap_CheckConflict(AvTpCtrlIdx, AvTp_Maap_MsgPtr->ReqStartMac,
                                          (uint16)AvTp_Ntohs(AvTp_Maap_MsgPtr->ReqCount));

  if(TRUE == IsMacConflict)
  {
    AVTP_ENTER_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    switch(AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_State)
    {
      /* State probe behaviour */
      case AVTP_MAAP_STATE_PROBE:
        switch(AVTP_MAAP_MSG_TYPE(AvTp_Maap_MsgPtr->SvVerMsgType))
        {
          case AVTP_MAAP_MSG_TYPE_PROBE:
            if(FALSE == AvTp_Maap_CompareMac(AvTp_Maap_MsgPtr->ReqStartMac))
            {
              Eth_PhysAddrType Placeholder;

              AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ProbeInterval = 0;
              AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_State = AVTP_MAAP_STATE_INITIAL;
              (void)AvTp_Maap_CalculateConflictCount(AvTpCtrlIdx, AvTp_Maap_MsgPtr->ReqStartMac,
                AvTp_Maap_MsgPtr->ReqCount, Placeholder);
            }
            break;

          case AVTP_MAAP_MSG_TYPE_DEFEND:
            AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ProbeInterval = 0;
            AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_State = AVTP_MAAP_STATE_INITIAL;
            AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ConflictCount = AvTp_Maap_MsgPtr->ConflictCount;
            break;

          case AVTP_MAAP_MSG_TYPE_ANNOUNCE:
            {
              Eth_PhysAddrType Placeholder;

              AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ProbeInterval = 0;
              AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_State = AVTP_MAAP_STATE_INITIAL;
              (void)AvTp_Maap_CalculateConflictCount(AvTpCtrlIdx, AvTp_Maap_MsgPtr->ReqStartMac,
                AvTp_Maap_MsgPtr->ReqCount, Placeholder);
            }
            break;

          default:
            ;
        }
        break;

      /* State defend behavior */
      case AVTP_MAAP_STATE_DEFEND:
        switch(AVTP_MAAP_MSG_TYPE(AvTp_Maap_MsgPtr->SvVerMsgType))
        {
          case AVTP_MAAP_MSG_TYPE_PROBE:
            AvTp_Maap_TxDefend(AvTpCtrlIdx, PhysSrcAddrPtr, AvTp_Maap_MsgPtr->ReqStartMac,
                               (uint16)AvTp_Ntohs(AvTp_Maap_MsgPtr->ReqCount));
            break;

          case AVTP_MAAP_MSG_TYPE_DEFEND:
            {
              Eth_PhysAddrType Placeholder;

              AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ProbeInterval = 0;
              AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_State = AVTP_MAAP_STATE_INITIAL;
              (void)AvTp_Maap_CalculateConflictCount(AvTpCtrlIdx, AvTp_Maap_MsgPtr->ReqStartMac,
                AvTp_Maap_MsgPtr->ReqCount, Placeholder);
            }
            break;

          case AVTP_MAAP_MSG_TYPE_ANNOUNCE:
            {
              Eth_PhysAddrType Placeholder;

              AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ProbeInterval = 0;
              AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_State = AVTP_MAAP_STATE_INITIAL;
              (void)AvTp_Maap_CalculateConflictCount(AvTpCtrlIdx, AvTp_Maap_MsgPtr->ReqStartMac,
                AvTp_Maap_MsgPtr->ReqCount, Placeholder);
            }
            break;

          default:
            ;
        }
        break;
      default:
        ;
    }
    AVTP_LEAVE_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6010 */ /* MD_MSR_STPTH */
/* AvTp_Maap_RxIndication */

/**********************************************************************************************************************
 *  AvTp_Maap_CompareMac
 *********************************************************************************************************************/
STATIC FUNC(boolean, AVTP_CODE) AvTp_Maap_CompareMac(
  CONST(Eth_PhysAddrType, AVTP_CONST) RxPhysAddrPtr) /* PRQA S 3673 */ /* MD_AvTp_16.7 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = FALSE;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_PTR(RxPhysAddrPtr),
    AVTP_MAAP_API_ID_COMPARE_MAC, AVTP_MAAP_E_INV_POINTER, FALSE)

  /* ----- Implementation ----------------------------------------------- */

  return RetVal;
} /* AvTp_Maap_CompareMac */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *    AvTp_Maap_CheckConflict
 *********************************************************************************************************************/
STATIC FUNC(boolean, AVTP_CODE) AvTp_Maap_CheckConflict(AvTp_CtrlIdxType AvTpCtrlIdx,
  CONST(Eth_PhysAddrType, AVTP_CONST) RxPhysAddrPtr, uint16 RequestCount) /* PRQA S 3673 */ /* MD_AvTp_16.7 */
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean RetVal = FALSE;
  uint16 LocalMacBegin;
  uint16 LocalMacEnd;
  uint16 RequestMacBegin;
  uint16 RequestMacEnd;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_PTR(RxPhysAddrPtr), AVTP_MAAP_API_ID_CHECK_CONFLICT,
    AVTP_MAAP_E_INV_POINTER, FALSE)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_GREATER_ZERO(RequestCount), AVTP_MAAP_API_ID_CHECK_CONFLICT,
    AVTP_MAAP_E_INV_LENGTH, FALSE)

  /* ----- Implementation ----------------------------------------------- */
  LocalMacBegin = (uint16)((uint16)AvTp_GetMaapBaseAddress(AvTpCtrlIdx)[4] << 8) |
                          ((uint16)AvTp_GetMaapBaseAddress(AvTpCtrlIdx)[5]);

  LocalMacEnd = (LocalMacBegin + AvTp_Ctrl[AvTpCtrlIdx].TxStreamCnt) - 1;

  RequestMacBegin = (uint16)((uint16)RxPhysAddrPtr[4] << 8 ) | ((uint16)RxPhysAddrPtr[5]);
  RequestMacEnd = (RequestMacBegin + RequestCount) - 1;

  /* compare MACs */
  if( /* request mac end inside check */
    ((RequestMacEnd >= LocalMacBegin) && (RequestMacEnd <= LocalMacEnd)) ||
    /* request mac begin inside check */
    ((RequestMacBegin >= LocalMacBegin) && (RequestMacBegin <= LocalMacEnd)) ||
    /* request mac begin and end outside check */
    ((RequestMacBegin < LocalMacBegin) && (RequestMacEnd >LocalMacEnd)))
  {
    /* compare least significant bytes */
    if((AvTp_GetMaapBaseAddress(AvTpCtrlIdx)[3] == RxPhysAddrPtr[3]) &&
       (AvTp_GetMaapBaseAddress(AvTpCtrlIdx)[2] == RxPhysAddrPtr[2]) &&
       (AvTp_GetMaapBaseAddress(AvTpCtrlIdx)[1] == RxPhysAddrPtr[1]) &&
       (AvTp_GetMaapBaseAddress(AvTpCtrlIdx)[0] == RxPhysAddrPtr[0]))
    {
      /* Conflict detected */
      RetVal = TRUE;
    }
  }
  return RetVal;
} /* AvTp_Maap_CheckConflict */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *    AvTp_Maap_CalculateConflictCount
 *********************************************************************************************************************/
STATIC FUNC(uint16, AVTP_CODE) AvTp_Maap_CalculateConflictCount(AvTp_CtrlIdxType AvTpCtrlIdx,
  CONST(Eth_PhysAddrType, AVTP_CONST) RxPhysAddrPtr, uint16 RequestCount, /* PRQA S 3673 */ /* MD_AvTp_16.7 */
  Eth_PhysAddrType ConflictStartPhysAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 LocalMacBegin;
  uint16 LocalMacEnd;
  uint16 RequestMacBegin;
  uint16 RequestMacEnd;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 6 */ /* MD_MSR_14.3 */
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_PTR(RxPhysAddrPtr), AVTP_MAAP_API_ID_CALCULATE_CONFLICT,
    AVTP_MAAP_E_INV_POINTER, E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_GREATER_ZERO(RequestCount), AVTP_MAAP_API_ID_CALCULATE_CONFLICT,
    AVTP_MAAP_E_INV_LENGTH, E_NOT_OK)
  AvTp_CheckDetErrorReturnValue(AVTP_COND_VALID_PTR(ConflictStartPhysAddrPtr), AVTP_MAAP_API_ID_CALCULATE_CONFLICT,
    AVTP_MAAP_E_INV_POINTER, E_NOT_OK)

  /* ----- Implementation ----------------------------------------------- */
  LocalMacBegin = (uint16)
                 ((uint16)AvTp_GetMaapBaseAddress(AvTpCtrlIdx)[4] << 8) |
                 ((uint16)AvTp_GetMaapBaseAddress(AvTpCtrlIdx)[5]);
  LocalMacEnd = (LocalMacBegin + AvTp_Ctrl[AvTpCtrlIdx].TxStreamCnt) - 1;

  RequestMacBegin = (uint16)((uint16)RxPhysAddrPtr[4] << 8) | ((uint16)RxPhysAddrPtr[5]);
  RequestMacEnd = (RequestMacBegin + RequestCount) - 1;

  /* Conflict detected, calculate conflict count and conflict start macs */
  /* outside border check */
  if( (RequestMacBegin < LocalMacBegin) && (RequestMacEnd > LocalMacEnd) )
  {
    AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ConflictCount = AvTp_Ctrl[AvTpCtrlIdx].TxStreamCnt;
    IpBase_Copy( \
      (AVTP_P2VAR(IpBase_CopyDataType)) ConflictStartPhysAddrPtr, \
      (AVTP_P2VAR(IpBase_CopyDataType)) AvTp_GetMaapBaseAddress(AvTpCtrlIdx), \
      ETH_PHYS_ADDR_LEN_BYTE);
  }
  /* left border check */
  else if( (RequestMacBegin <= LocalMacBegin) && (RequestMacEnd <= LocalMacEnd) )
  {
    AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ConflictCount = (RequestMacEnd - LocalMacBegin) + 1;
    IpBase_Copy( \
    (AVTP_P2VAR(IpBase_CopyDataType)) ConflictStartPhysAddrPtr, \
    (AVTP_P2VAR(IpBase_CopyDataType)) AvTp_GetMaapBaseAddress(AvTpCtrlIdx), \
    ETH_PHYS_ADDR_LEN_BYTE);
  }
  /* within border check */
  else if((RequestMacBegin >= LocalMacBegin) && ((RequestMacEnd <= LocalMacEnd)))
  {
    AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ConflictCount = RequestCount;
    IpBase_Copy( \
    (AVTP_P2VAR(IpBase_CopyDataType)) ConflictStartPhysAddrPtr, \
    (AVTP_P2VAR(IpBase_CopyDataType)) RxPhysAddrPtr, \
    ETH_PHYS_ADDR_LEN_BYTE);
  }
  /* right border check */
  else if( (RequestMacBegin <= LocalMacEnd) && (RequestMacEnd >= LocalMacEnd) )
  {
    AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ConflictCount = (LocalMacEnd - RequestMacBegin) + 1;
    IpBase_Copy( \
    (AVTP_P2VAR(IpBase_CopyDataType)) ConflictStartPhysAddrPtr, \
    (AVTP_P2VAR(IpBase_CopyDataType)) RxPhysAddrPtr, \
    ETH_PHYS_ADDR_LEN_BYTE);
  }
  else
  {
    /* do nothing */
  }
  return AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ConflictCount;
} /* AvTp_Maap_CalculateConflictCount */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *    AvTp_Maap_TxProbe
 *********************************************************************************************************************/
STATIC FUNC(void, AVTP_CODE) AvTp_Maap_TxProbe(AvTp_CtrlIdxType AvTpCtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ArrayIdx = 6;
  uint16 BufferLen = AVTP_MAAP_MSG_LEN; /* PRQA S 0781 */ /* MD_AvTp_5.6 */
  uint8 BufferIdx;
  AVTP_P2VAR(AvTp_Maap_MsgType) AvTp_Maap_ProbePtr;

  /* ----- Implementation ----------------------------------------------- */
  if(BUFREQ_OK == EthIf_ProvideTxBuffer(AvTp_Ctrl[AvTpCtrlIdx].VCtrlIdx,
                                        AVTP_FRAME_TYPE,
                                        AVTP_MAAP_FRAME_PRIORITY,
                                        &BufferIdx,
                                        (AVTP_P2VAR(Eth_DataType*)) &AvTp_Maap_ProbePtr,
                                        &BufferLen))
  {
    AvTp_Maap_ProbePtr->SvVerMsgType =   (AVTP_VERSION << AVTP_HDR_COM_VERSION_BIT_POS ) |
                                         (AVTP_MAAP_MSG_TYPE_PROBE);
    AvTp_Maap_ProbePtr->CdSubtype =      AVTP_SUBTYPE_MAAP;
    /*lint -e572 */
    AvTp_Maap_ProbePtr->MaapVerDataLen = (uint16)AvTp_Htons(((AVTP_MAAP_VERSION << AVTP_MAAP_HDR_VERSION_BIT_POS) |
                                                     AVTP_MAAP_DATA_LEN));
    AvTp_Maap_ProbePtr->ConflictCount =  (uint16)0u;
    AvTp_Maap_ProbePtr->ReqCount =       (uint16)AvTp_Htons(AvTp_Ctrl[AvTpCtrlIdx].TxStreamCnt); /*lint +e572 */
    while(0u < ArrayIdx)
    {
      ArrayIdx--;
      AvTp_Maap_ProbePtr->ReqStartMac[ArrayIdx] = AvTp_GetMaapBaseAddress(AvTpCtrlIdx)[ArrayIdx];
      AvTp_Maap_ProbePtr->ConflictStartMac[ArrayIdx] = 0;
    }
    IpBase_Fill((AVTP_P2VAR(IpBase_CopyDataType))AvTp_Maap_ProbePtr->StreamId, 0, AVTP_STREAM_ID_LEN_BYTE);

    if(E_OK != EthIf_Transmit(AvTp_Ctrl[AvTpCtrlIdx].VCtrlIdx, BufferIdx, AVTP_FRAME_TYPE, FALSE, AVTP_MAAP_MSG_LEN,
      (AVTP_P2VAR(uint8))AvTp_Maap_MulticastMac))
    {
      ;
    }
  }
} /* AvTp_Maap_TxProbe */

/**********************************************************************************************************************
 *    AvTp_Maap_TxAnnounce
 *********************************************************************************************************************/
STATIC FUNC(void, AVTP_CODE) AvTp_Maap_TxAnnounce(AvTp_CtrlIdxType AvTpCtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ArrayIdx = 6;
  uint16 BufferLen = AVTP_MAAP_MSG_LEN; /* PRQA S 0781 */ /* MD_AvTp_5.6 */
  uint8 BufferIdx;
  AVTP_P2VAR(AvTp_Maap_MsgType) AvTp_Maap_AnnouncePtr;

  /* ----- Implementation ----------------------------------------------- */
  if(BUFREQ_OK == EthIf_ProvideTxBuffer(AvTp_Ctrl[AvTpCtrlIdx].VCtrlIdx,
                                        AVTP_FRAME_TYPE,
                                        AVTP_MAAP_FRAME_PRIORITY,
                                        &BufferIdx,
                                        (AVTP_P2VAR(Eth_DataType*)) &AvTp_Maap_AnnouncePtr,
                                        &BufferLen))
  {
    AvTp_Maap_AnnouncePtr->SvVerMsgType = (AVTP_VERSION << AVTP_HDR_COM_VERSION_BIT_POS ) |
                                          (AVTP_MAAP_MSG_TYPE_ANNOUNCE);
    AvTp_Maap_AnnouncePtr->CdSubtype =    AVTP_SUBTYPE_MAAP;
    /*lint -e572 */
    AvTp_Maap_AnnouncePtr->MaapVerDataLen = (uint16)AvTp_Htons((AVTP_MAAP_VERSION << AVTP_MAAP_HDR_VERSION_BIT_POS) |
                                                       AVTP_MAAP_DATA_LEN);
    AvTp_Maap_AnnouncePtr->ConflictCount =  (uint16)0u;
    AvTp_Maap_AnnouncePtr->ReqCount =       (uint16)AvTp_Htons(AvTp_Ctrl[AvTpCtrlIdx].TxStreamCnt); /*lint +e572 */
    while(0u < ArrayIdx)
    {
      ArrayIdx--;
      AvTp_Maap_AnnouncePtr->ReqStartMac[ArrayIdx] = AvTp_GetMaapBaseAddress(AvTpCtrlIdx)[ArrayIdx];
      AvTp_Maap_AnnouncePtr->ConflictStartMac[ArrayIdx] = 0;
    }
    IpBase_Fill((AVTP_P2VAR(IpBase_CopyDataType))AvTp_Maap_AnnouncePtr->StreamId, 0, AVTP_STREAM_ID_LEN_BYTE);
    if(E_OK != EthIf_Transmit(AvTp_Ctrl[AvTpCtrlIdx].VCtrlIdx, BufferIdx, AVTP_FRAME_TYPE, FALSE, AVTP_MAAP_MSG_LEN,
      (AVTP_P2VAR(uint8))AvTp_Maap_MulticastMac))
    {
      ;
    }
  }
} /* AvTp_Maap_TxAnnounce */

/**********************************************************************************************************************
 *    AvTp_Maap_TxDefend
 *********************************************************************************************************************/
STATIC FUNC(void, AVTP_CODE) AvTp_Maap_TxDefend(AvTp_CtrlIdxType AvTpCtrlIdx, AVTP_P2VAR(uint8) PhysSrcAddrPtr,
  Eth_PhysAddrType RxPhysAddrPtr, uint16 RequestCount)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ArrayIdx = 6;
  uint16 BufferLen = AVTP_MAAP_MSG_LEN; /* PRQA S 0781 */ /* MD_AvTp_5.6 */
  uint8 BufferIdx;
  AVTP_P2VAR(AvTp_Maap_MsgType) AvTp_Maap_DefendPtr;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  AvTp_CheckDetErrorReturnVoid(AVTP_COND_VALID_PTR(RxPhysAddrPtr), AVTP_MAAP_API_ID_TX_DEFEND,
  AVTP_MAAP_E_INV_POINTER)
  AvTp_CheckDetErrorReturnVoid(AVTP_COND_GREATER_ZERO(RequestCount), AVTP_MAAP_API_ID_TX_DEFEND,
  AVTP_MAAP_E_INV_LENGTH)

  /* ----- Implementation ----------------------------------------------- */
  if(BUFREQ_OK == EthIf_ProvideTxBuffer(AvTp_Ctrl[AvTpCtrlIdx].VCtrlIdx,
                                        AVTP_FRAME_TYPE,
                                        AVTP_MAAP_FRAME_PRIORITY,
                                        &BufferIdx,
                                        (AVTP_P2VAR(Eth_DataType*)) &AvTp_Maap_DefendPtr,
                                        &BufferLen))
  {
    AvTp_Maap_DefendPtr->SvVerMsgType =   (AVTP_VERSION << AVTP_HDR_COM_VERSION_BIT_POS ) |
                                          (AVTP_MAAP_MSG_TYPE_DEFEND);
    AvTp_Maap_DefendPtr->CdSubtype =      AVTP_SUBTYPE_MAAP;
    AvTp_Maap_DefendPtr->MaapVerDataLen = (uint16)AvTp_Htons((AVTP_MAAP_VERSION << AVTP_MAAP_HDR_VERSION_BIT_POS) |
                                                     AVTP_MAAP_DATA_LEN);
    AvTp_Maap_DefendPtr->ReqCount = (uint16)AvTp_Htons(RequestCount);
    /*lint -e666 */
    AvTp_Maap_DefendPtr->ConflictCount = (uint16)AvTp_Htons(
                                          AvTp_Maap_CalculateConflictCount(AvTpCtrlIdx,
                                          RxPhysAddrPtr, RequestCount, AvTp_Maap_DefendPtr->ConflictStartMac));
    /*lint +e666 */

    while(0u < ArrayIdx)
    {
      ArrayIdx--;
      AvTp_Maap_DefendPtr->ReqStartMac[ArrayIdx] = RxPhysAddrPtr[ArrayIdx];
    }
    IpBase_Fill((AVTP_P2VAR(IpBase_CopyDataType))AvTp_Maap_DefendPtr->StreamId, 0, AVTP_STREAM_ID_LEN_BYTE);

    /* Transmit to Phy-Source Address of conflicting Message (unicast) */
    if(E_OK != EthIf_Transmit(AvTp_Ctrl[AvTpCtrlIdx].VCtrlIdx, BufferIdx,
                              AVTP_FRAME_TYPE, FALSE, AVTP_MAAP_MSG_LEN, PhysSrcAddrPtr))
    {
      ;
    }
  }
} /* AvTp_Maap_TxDefend */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *    AvTp_Maap_GenerateDynamicAddr
 *********************************************************************************************************************/
STATIC FUNC(void, AVTP_CODE) AvTp_Maap_GenerateDynamicAddr(AvTp_CtrlIdxType AvTpCtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 CurrentAddr;
  uint32 NewAddr;
  uint32 Offset;
  uint8 Idx; /* PRQA S 0781 */ /* MD_AvTp_5.6 */

  /* ----- Implementation ----------------------------------------------- */
  /* Generated Address have to be in 'MAAP Dynamic Allocation Pool' (IEEE 1722-2011, Annex B):
     Start: 91:E0:F0:00:00:00
     End  : 91:E0:F0:00:FD:FF */

  /* Check if all address range fits in 'MAAP Dynamic Allocation Pool' */
  CurrentAddr = (uint16)((uint16)AvTp_GetMaapBaseAddress(AvTpCtrlIdx)[4] << 8) |
                        ((uint16)AvTp_GetMaapBaseAddress(AvTpCtrlIdx)[5]);

  /* get max offset */
  Offset = AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ConflictCount + AvTp_Ctrl[AvTpCtrlIdx].TxStreamCnt;

  if(AVTP_MAAP_DYN_ALLOC_END < (CurrentAddr + Offset))
  {
    Idx = AvTp_Ctrl[AvTpCtrlIdx].TxStreamCnt;

    /* Address exceeds 'MAAP Dynamic Allocation Pool' -> reset to MAAP_DYN_ALLOC_START */
    while(0u < Idx)
    {
      Idx--;
      /* Copy Maap Start Address of Dynamic Allocation Pool to Base-MulticastMacAddress */
      IpBase_Copy((AVTP_P2VAR(IpBase_CopyDataType))
        AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[Idx]].MulticastMacAddr,
        (AVTP_P2CONST(IpBase_CopyDataType)) Maap_DynAllocPoolStartAddr,
        ETH_PHYS_ADDR_LEN_BYTE);
    }

    CurrentAddr = (uint16)((uint16)AvTp_GetMaapBaseAddress(AvTpCtrlIdx)[4] << 8) |
                          ((uint16)AvTp_GetMaapBaseAddress(AvTpCtrlIdx)[5]);
  }

  /* Calculate new base address */
  NewAddr = (CurrentAddr + Offset);

  /* Assign a MulticastMacAddress to all TxStreams configured for this AvTpCtrl */
  Idx = AvTp_Ctrl[AvTpCtrlIdx].TxStreamCnt;

  while(0u < Idx)
  {
    Idx --;

    /* Assign new MulticastMacAddress */
    AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[Idx]].MulticastMacAddr[4] = (uint8)((NewAddr & 0xFF00) >> 8);
    AvTp_TxStreamMgmt[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[Idx]].MulticastMacAddr[5] = (uint8) (NewAddr & 0x00FF);
  }

  /* New addresses have solved current conflict -> reset ConflictCount */
  AvTp_Ctrl[AvTpCtrlIdx].Maap_MgmtPtr->Maap_ConflictCount = 0;
} /* AvTp_Maap_GenerateDynamicAddr */
#endif /* (STD_ON == AVTP_MAAP_SUPPORT) */

#if (0 < AVTP_STREAM_CNT_RX)
/**********************************************************************************************************************
 *    AvTp_ProcessStreamPdu
 *********************************************************************************************************************/
/* PRQA S 2006 RET_PATH */ /* MD_MSR_14.7 */
/* PRQA S 6030 STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6010 STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6050 STCAL */ /* MD_MSR_STCAL */
STATIC FUNC(void, AVTP_CODE) AvTp_ProcessStreamPdu(
  AvTp_CtrlIdxType AvTpCtrlIdx,
  AVTP_P2VAR(AvTp_DataType) DataPtr,
  uint16 LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  AvTp_StreamIdxType StreamIdx = 0; /* PRQA S 0781 */ /* MD_AvTp_5.6 */
  AVTP_P2VAR(AvTp_StreamHdrType) StreamHdrPtr;
  AvTp_RxXPduInfoType AvTp_RxXPduInfo;
  boolean IsMsgValid;

  /* ----- Development Error Checks ------------------------------------- */
  AvTp_CheckDetErrorReturnVoid(AVTP_COND_MIN_STR_HDR_LEN(LenByte), AVTP_API_ID_PROCESS_STREAM_PDU,
    AVTP_E_INV_LENGTH)
  AvTp_CheckDetErrorReturnVoid(AVTP_COND_NOT_UNINT(AvTp_State[AvTpCtrlIdx]), AVTP_API_ID_PROCESS_STREAM_PDU,
    AVTP_E_NOT_INITIALIZED)

  /* ----- Implementation ----------------------------------------------- */
  /* set received data to stream header */
  StreamHdrPtr = (AVTP_P2VAR(AvTp_StreamHdrType)) DataPtr;

  /* check StreamDataLength */
  if((LenByte-AVTP_HDR_LEN_STREAM) < AvTp_Ntohs(StreamHdrPtr->StreamDataLen))
  {
    /* StreamDataLength indicates more data than contained in ethernet frame -> drop message */
    return;
  }

  /* check if StreamId is valid */
  if(TRUE != (boolean)AVTP_HDR_STREAM_SV(StreamHdrPtr->SvVerMrSsdTv))
  {
    /* StreamId is not valid -> drop message */
    return;
  }

  /* get StreamIdx corresponding to received StreamId */
  if(E_OK != AvTp_SearchStreamIdx((AVTP_P2CONST(AvTp_StreamIdType))(&StreamHdrPtr->StreamId[0]), &StreamIdx))
  {
    /* unknown StreamId -> drop message */
    return;
  }

#if (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT)
  /* Check for nonsequential sequence number */
  if(TRUE == AvTp_DiagnosticCounterListener[StreamIdx].FirstFrame)
  {
    AvTp_DiagnosticCounterListener[StreamIdx].FirstFrame = FALSE;

    /* Inital Frame -> adjust sequence number */
    AvTp_RxStreamMgmt[StreamIdx].SequenceNum = StreamHdrPtr->SequenceNum;
  }
  else if((AvTp_RxStreamMgmt[StreamIdx].SequenceNum + 1) != StreamHdrPtr->SequenceNum)
  {
    AvTp_DiagnosticCounterListener[StreamIdx].SeqNumMismatch++;

    #if (STD_ON == AVTP_DIAG_WRITE_LISTENER_SEQNUM_MISMATCH)
    CANOE_WRITE_STRING2("[DIAG_L:] SeqNumMismatch     [StreamIdx: %d]: %d",
      StreamIdx,
      AvTp_DiagnosticCounterListener[StreamIdx].SeqNumMismatch);
    #endif /* (STD_ON == AVTP_DIAG_WRITE_LISTENER_SEQNUM_MISMATCH) */
  }
  else
  {
    /* nothing to do */
  }
  AvTp_RxStreamMgmt[StreamIdx].SequenceNum++;

  /* Check for media clock reset */
  if(AvTp_RxStreamMgmt[StreamIdx].MediaClockReset != AVTP_HDR_STREAM_MR(StreamHdrPtr->SvVerMrSsdTv))
  {
    AvTp_DiagnosticCounterListener[StreamIdx].MediaReset++;

    #if (STD_ON == AVTP_DIAG_WRITE_LISTENER_MEDIA_RESET)
    CANOE_WRITE_STRING2("[DIAG_L:] MediaReset         [StreamIdx: %d]: %d",
      StreamIdx,
      AvTp_DiagnosticCounterListener[StreamIdx].MediaReset);
    #endif /* (STD_ON == AVTP_DIAG_WRITE_LISTENER_MEDIA_RESET) */
  }

  /* Check for timestamp (in-)valid  */
  if(TRUE == AVTP_HDR_STREAM_TV(StreamHdrPtr->SvVerMrSsdTv))
  {
    AvTp_DiagnosticCounterListener[StreamIdx].TimestampValid++;

    #if (STD_ON == AVTP_DIAG_WRITE_LISTENER_TIMESTAMP_VALID)
    CANOE_WRITE_STRING2("[DIAG_L:] TimestampValid     [StreamIdx: %d]: %d",
      StreamIdx,
      AvTp_DiagnosticCounterListener[StreamIdx].TimestampValid);
    #endif /* (STD_ON == AVTP_DIAG_WRITE_LISTENER_TIMESTAMP_VALID) */
  }
  else
  {
    AvTp_DiagnosticCounterListener[StreamIdx].TimestampNotValid++;

    #if (STD_ON == AVTP_DIAG_WRITE_LISTENER_TIMESTAMP_NOT_VALID)
    CANOE_WRITE_STRING2("[DIAG_L:] TimestampNotValid  [StreamIdx: %d]: %d",
      StreamIdx,
      AvTp_DiagnosticCounterListener[StreamIdx].TimestampNotValid);
    #endif /* (STD_ON == AVTP_DIAG_WRITE_LISTENER_TIMESTAMP_NOT_VALID) */
  }

  /* Check for timestamp uncertain */
  if(AvTp_RxStreamMgmt[StreamIdx].IsUncertainTimestamp != AVTP_HDR_STREAM_TU(StreamHdrPtr->SubtypeSpecificData1Tu))
  {
    AvTp_DiagnosticCounterListener[StreamIdx].TimestampUncertain++;

    #if (STD_ON == AVTP_DIAG_WRITE_LISTENER_TIMESTAMP_UNCERTAIN)
    CANOE_WRITE_STRING2("[DIAG_L:] TimestampUncertain [StreamIdx: %d]: %d",
      StreamIdx,
      AvTp_DiagnosticCounterListener[StreamIdx].TimestampUncertain);
    #endif /* (STD_ON == AVTP_DIAG_WRITE_LISTENER_TIMESTAMP_UNCERTAIN) */
  }

  #if (STD_ON == AVTP_TIME_SUPPORT)
  /* Check for late timestamp */
  if(AVTP_INV_TIME_BASE_ID != AvTp_TxStreamInfo[StreamIdx].TimeBaseId)
  {
    AvTp_GlobalTimestampType CurrentTime;
    AvTp_TimestampType CurrentTime_Mod32;
    AvTp_SyncStateType SyncState; /* PRQA S 0781 */ /* MD_AvTp_5.6 */

    /* get current time mod 2^32 */
    (void)AvTp_GetCurrentTime(StreamIdx, &CurrentTime, &SyncState);

    if( (AVTP_PORT_STATE_SYNC == SyncState) || (AVTP_PORT_STATE_UNCERTAIN == SyncState))
    {
      /* calculate: avtp_timestamp = (AS_sec * 10^9 + AS_ns) modulo 2^32 */
      CurrentTime_Mod32 = (AvTp_TimestampType)(((CurrentTime.seconds * AVTP_NANOSECONDS_PER_SECOND) +
        CurrentTime.nanoseconds) % 0xFFFFFFFFU);

      /* Check if Timestamp is in the past */
      if(CurrentTime_Mod32 > AvTp_Ntohl(StreamHdrPtr->Timestamp))
      {
        AvTp_DiagnosticCounterListener[StreamIdx].LateTimestamp++;

        #if (STD_ON == AVTP_DIAG_WRITE_LISTENER_TIMESTAMP_UNCERTAIN)
        CANOE_WRITE_STRING2("[DIAG_L:] LateTimestamp      [StreamIdx: %d]: %d",
          StreamIdx,
          AvTp_DiagnosticCounterListener[StreamIdx].LateTimestamp);
        #endif /* (STD_ON == AVTP_DIAG_WRITE_LISTENER_TIMESTAMP_UNCERTAIN) */
      }
    }
  }
  #endif /* (STD_ON == AVTP_TIME_SUPPORT) */
#endif /* (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT) */

  switch(StreamHdrPtr->Subtype)
  {
#if (STD_ON == AVTP_61883IIDC_SUPPORT)
  case AVTP_SUBTYPE_61883IIDC:
    #if (STD_ON == AVTP_RX_CHECK)
    IsMsgValid = AvTp_61883iidc_Process(StreamIdx, StreamHdrPtr, &AvTp_RxXPduInfo);
    #else
    IsMsgValid = AvTp_61883iidc_Process(StreamHdrPtr, &AvTp_RxXPduInfo);
    #endif /* (STD_ON == AVTP_RX_CHECK) */
    break;
#endif /* (STD_ON == AVTP_61883IIDC_SUPPORT) */

#if (STD_ON == AVTP_AAF_SUPPORT)
  case AVTP_SUBTYPE_AAF:
    #if (STD_ON == AVTP_RX_CHECK)
    IsMsgValid = AvTp_Aaf_Process(StreamIdx, StreamHdrPtr, &AvTp_RxXPduInfo);
    #else
    IsMsgValid = AvTp_Aaf_Process(StreamHdrPtr, &AvTp_RxXPduInfo);
    #endif /* (STD_ON == AVTP_RX_CHECK) */
    break;
#endif /* (STD_ON == AVTP_AAF_SUPPORT) */

#if (STD_ON == AVTP_CVF_SUPPORT)
  case AVTP_SUBTYPE_CVF:
    #if (STD_ON == AVTP_RX_CHECK)
    IsMsgValid = AvTp_Cvf_Process(StreamIdx, StreamHdrPtr, &AvTp_RxXPduInfo);
    #else
    IsMsgValid = AvTp_Cvf_Process(StreamHdrPtr, &AvTp_RxXPduInfo);
    #endif /* (STD_ON == AVTP_RX_CHECK) */
    break;
#endif /* (STD_ON == AVTP_CVF_SUPPORT) */

#if (STD_ON == AVTP_CRF_SUPPORT)
  case AVTP_SUBTYPE_CRF:
    #if (STD_ON == AVTP_RX_CHECK)
    IsMsgValid = AvTp_Crf_Process(StreamIdx, StreamHdrPtr, &AvTp_RxXPduInfo);
    #else
    IsMsgValid = AvTp_Crf_Process(StreamHdrPtr, &AvTp_RxXPduInfo);
    #endif /* (STD_ON == AVTP_RX_CHECK) */
    break;
#endif /* (STD_ON == AVTP_CRF_SUPPORT) */

#if (STD_ON == AVTP_VSF_SUPPORT)
  case AVTP_SUBTYPE_VSF:
  #if (STD_ON == AVTP_RX_CHECK)
    IsMsgValid = AvTp_Vsf_Process(StreamIdx, StreamHdrPtr, &AvTp_RxXPduInfo);
  #else
    IsMsgValid = AvTp_Vsf_Process(StreamHdrPtr, &AvTp_RxXPduInfo);
  #endif /* (STD_ON == AVTP_RX_CHECK) */
    break;
#endif /* (STD_ON == AVTP_VSF_SUPPORT) */

  default:
    /* Unsupported Subtype -> drop message */
    IsMsgValid = AVTP_MSG_INVALID;
    AvTp_CallDetReportError(AVTP_API_ID_PROCESS_STREAM_PDU, AVTP_E_INV_SUBTYPE)
    break;
  }

  if(AVTP_MSG_VALID == IsMsgValid)
  {
#if (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT)
    AvTp_DiagnosticCounterListener[StreamIdx].FramesRx++;
    #if (STD_ON == AVTP_DIAG_WRITE_LISTENER_FRAMES_RX)
    CANOE_WRITE_STRING2("[DIAG_L:] FrameRx            [StreamIdx: %d]: %d",
      StreamIdx,
      AvTp_DiagnosticCounterListener[StreamIdx].FramesRx);
    #endif /* (STD_ON == AVTP_DIAG_WRITE_LISTENER_FRAMES_RX) */
#endif /* (STD_ON == AVTP_DIAGNOSTIC_COUNTER_SUPPORT) */

    /* find upper layer RxIndication with corresponding streamId */
    /*lint -e645 */ /* AvTp_RxPduInfo is initialized. It is ensured by the Value of 'IsMsgValid' */
    /* PRQA S 3354 1 */ /* AvTp_RxPduInfo is initialized. It is ensured by the Value of 'IsMsgValid' */
    AvTp_StreamRxIndicationFunctions(StreamIdx, DataPtr, (AVTP_P2VAR(AvTp_RxPduInfoType))&AvTp_RxXPduInfo,
      (uint16)(LenByte - AvTp_RxStreamInfo[StreamIdx].StreamCfgPtr->AvTpHdrLen)); /*lint +e645 */
  }
  else
  {
    /* Invalid message -> drop message */
    return;
  }
} /* AvTp_ProcessStreamPdu */
/* PRQA L: RET_PATH */
/* PRQA L: STCYC */
/* PRQA L: STPTH */
/* PRQA L: STCAL */

#if (STD_ON == AVTP_61883IIDC_SUPPORT)
/**********************************************************************************************************************
 *    AvTp_61883iidc_Process
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean, AVTP_CODE) AvTp_61883iidc_Process(
#if (STD_ON == AVTP_RX_CHECK)
  AvTp_StreamIdxType StreamIdx,
#endif /* (STD_ON == AVTP_RX_CHECK) */
  AVTP_P2VAR(AvTp_StreamHdrType) StreamHdrPtr,
  AVTP_P2VAR(AvTp_RxXPduInfoType) AvTp_RxXPduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  AVTP_P2VAR(AvTp_61883IIDC_MsgType) Iec61883iidcHdrPtr;
  AVTP_P2VAR(AvTp_61883IIDC_RxInfoType) Iec61883iidcInfoPtr = (AVTP_P2VAR(AvTp_61883IIDC_RxInfoType)) \
                                                               AvTp_RxXPduInfoPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* Get Information for Upper-layer */
  Iec61883iidcInfoPtr->Subtype = AVTP_SUBTYPE_61883IIDC;
#if (STD_ON == AVTP_RX_PDU_INFO_SUPPORT)
  Iec61883iidcInfoPtr->MediaClockReset       = (boolean)AVTP_HDR_STREAM_MR(StreamHdrPtr->SvVerMrSsdTv);
  Iec61883iidcInfoPtr->IsUncertainTimestamp  = (boolean)AVTP_HDR_STREAM_TU(StreamHdrPtr->SubtypeSpecificData1Tu);
  Iec61883iidcInfoPtr->IsValidTimestamp      = (boolean)AVTP_HDR_STREAM_TV(StreamHdrPtr->SvVerMrSsdTv);
  Iec61883iidcInfoPtr->Timestamp             = AvTp_Ntohl(StreamHdrPtr->Timestamp);
  Iec61883iidcInfoPtr->SequenceNum           = StreamHdrPtr->SequenceNum;
#endif /* (STD_ON == AVTP_RX_PDU_INFO_SUPPORT) */

  /* Set Pointer to IEC 61883 data */
  Iec61883iidcHdrPtr = (AVTP_P2VAR(AvTp_61883IIDC_MsgType)) &StreamHdrPtr->SubtypeSpecificData2;
#if (STD_ON == AVTP_RX_CHECK)
  /* Parse received header and check if values correspond to configured values.*/
  if(AvTp_CfgGetRxIecTag(StreamIdx) != AVTP_61883IIDC_TAG(Iec61883iidcHdrPtr->TagChannel))
  {
    /* unexpected Tag -> drop message */
    return AVTP_MSG_INVALID;
  }
  if(AvTp_CfgGetRxIecChannel(StreamIdx) != AVTP_61883IIDC_CHANNEL(Iec61883iidcHdrPtr->TagChannel))
  {
    /* unexpected Channel -> drop message */
    return AVTP_MSG_INVALID;
  }
  /* -> Message is valid! */
#else
  if( (AVTP_61883IIDC_TAG_NO_CIP != AVTP_61883IIDC_TAG(Iec61883iidcHdrPtr->TagChannel)) &&
      (AVTP_61883IIDC_TAG_CIP    != AVTP_61883IIDC_TAG(Iec61883iidcHdrPtr->TagChannel)) )
  {
    /* invalid Tag -> drop message */
    return AVTP_MSG_INVALID;
  }
  /* -> Message is valid! */
#endif /* (STD_ON == AVTP_RX_CHECK) */

  /* set Sy */
  Iec61883iidcInfoPtr->Sy = AVTP_61883IIDC_SY(Iec61883iidcHdrPtr->TcodeSy);

  return AVTP_MSG_VALID;
} /* AvTp_61883iidc_Process */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (STD_ON == AVTP_61883IIDC_SUPPORT) */

/**********************************************************************************************************************
 *    AvTp_Aaf_Process
 *********************************************************************************************************************/
#if (STD_ON == AVTP_AAF_SUPPORT)
LOCAL_INLINE FUNC(boolean, AVTP_CODE) AvTp_Aaf_Process(
#if (STD_ON == AVTP_RX_CHECK)
  AvTp_StreamIdxType StreamIdx,
#endif /* (STD_ON == AVTP_RX_CHECK) */
  AVTP_P2VAR(AvTp_StreamHdrType) StreamHdrPtr,
  AVTP_P2VAR(AvTp_RxXPduInfoType) AvTp_RxXPduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  AVTP_P2VAR(AVTP_AAF_MsgType) AafHdrPtr;
  AVTP_P2VAR(AvTp_AAF_RxInfoType) AafInfoPtr = (AVTP_P2VAR(AvTp_AAF_RxInfoType)) AvTp_RxXPduInfoPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* Get Information for Upper-layer */
  AafInfoPtr->Subtype = AVTP_SUBTYPE_AAF;
#if (STD_ON == AVTP_RX_PDU_INFO_SUPPORT)
  AafInfoPtr->MediaClockReset      = (boolean)AVTP_HDR_STREAM_MR(StreamHdrPtr->SvVerMrSsdTv);
  AafInfoPtr->IsUncertainTimestamp = (boolean)AVTP_HDR_STREAM_TU(StreamHdrPtr->SubtypeSpecificData1Tu);
  AafInfoPtr->IsValidTimestamp     = (boolean)AVTP_HDR_STREAM_TV(StreamHdrPtr->SvVerMrSsdTv);
  AafInfoPtr->Timestamp            = AvTp_Ntohl(StreamHdrPtr->Timestamp);
  AafInfoPtr->SequenceNum          = StreamHdrPtr->SequenceNum;
#endif /* (STD_ON == AVTP_RX_PDU_INFO_SUPPORT */

  /* Set Pointer to AVTP Audio data */
  AafHdrPtr = (AVTP_P2VAR(AVTP_AAF_MsgType)) &StreamHdrPtr->SubtypeSpecificData2;
#if (STD_ON == AVTP_RX_CHECK)
  /* Parse received header and check if values correspond to configured values.*/
  if(AvTp_CfgGetRxAvtpFormat(StreamIdx) != AafHdrPtr->Format)
  {
    /* unexpected Format -> drop message */
    return AVTP_MSG_INVALID;
  }
  if(AvTp_CfgGetRxAvtpNsr(StreamIdx) != AVTP_AAF_NRS(AafHdrPtr->NsrChPerFrame_High))
  {
    /* unexpected Nominal Sample Rate -> drop message */
    return AVTP_MSG_INVALID;
  }
  if(AvTp_CfgGetRxAvtpChanPerFram(StreamIdx) != AVTP_AAF_CHPERFRAME(AafHdrPtr->NsrChPerFrame_High,
    AafHdrPtr->ChPerFrame_Low))
  {
    /* unexpected Channels Per Frame -> drop message */
    return AVTP_MSG_INVALID;
  }
  if(AvTp_CfgGetRxAvtpBitDepth(StreamIdx) != AafHdrPtr->BitDepth)
  {
    /* unexpected Bit Depth -> drop message */
    return AVTP_MSG_INVALID;
  }
  if((uint8)(AvTp_CfgGetRxAvtpSparseMode(StreamIdx)) != AVTP_AAF_SP(AafHdrPtr->RSpEvt))
  {
    /* unexpected timestamp Spares Mode -> drop message */
    return AVTP_MSG_INVALID;
  }
  /* -> Message is valid! */
#else
  if(AVTP_AAF_FMT_INT_16BIT < AafHdrPtr->Format)
  {
    /* invalid Format = Reserved -> drop message */
    return AVTP_MSG_INVALID;
  }
  if(AVTP_AAF_NSR_192KHZ < AVTP_AAF_NRS(AafHdrPtr->NsrChPerFrame_High))
  {
    /* invalid Nominal Sample Rate = Reserved -> drop message */
    return AVTP_MSG_INVALID;
  }
  switch(AafHdrPtr->Format)
  {
  case AVTP_AAF_FMT_FLOAT_32BIT:
  case AVTP_AAF_FMT_USERSPEC:
    if(AafHdrPtr->BitDepth != 0)
    {
      /* Invalid Bit Depth -> drop message */
      return AVTP_MSG_INVALID;
    }
    break;
  case AVTP_AAF_FMT_INT_16BIT:
    if(AafHdrPtr->BitDepth > 16)
    {
      /* Invalid Bit Depth -> drop message */
      return AVTP_MSG_INVALID;
    }
    break;
  case AVTP_AAF_FMT_INT_24BIT:
    if(AafHdrPtr->BitDepth > 24)
    {
      /* Invalid Bit Depth -> drop message */
      return AVTP_MSG_INVALID;
    }
    break;
  case AVTP_AAF_FMT_INT_32BIT:
    if(AafHdrPtr->BitDepth > 32)
    {
      /* Invalid Bit Depth -> drop message */
      return AVTP_MSG_INVALID;
    }
    break;
  }
  if(0 == AVTP_AAF_CHPERFRAME(AafHdrPtr->NsrChPerFrame_High,
    AafHdrPtr->ChPerFrame_Low))
  {
    /* invalid Channels Per Frame -> drop message */
    return AVTP_MSG_INVALID;
  }
  /* -> Message is valid! */
#endif /* (STD_ON == AVTP_RX_CHECK) */

  /* get Event */
  AafInfoPtr->Event = (uint8)AVTP_AAF_EVT(AafHdrPtr->RSpEvt);

  return AVTP_MSG_VALID;
} /* AvTp_Aaf_Process */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (STD_ON == AVTP_AAF_SUPPORT) */

#if (STD_ON == AVTP_CVF_SUPPORT)
/**********************************************************************************************************************
 *    AvTp_Cvf_Process
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean, AVTP_CODE) AvTp_Cvf_Process(
#if (STD_ON == AVTP_RX_CHECK)
  AvTp_StreamIdxType StreamIdx,
#endif /* (STD_ON == AVTP_RX_CHECK) */
  AVTP_P2VAR(AvTp_StreamHdrType) StreamHdrPtr,
  AVTP_P2VAR(AvTp_RxXPduInfoType) AvTp_RxXPduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  AVTP_P2VAR(AvTp_CVF_MsgType) CvfHdrPtr;
  AVTP_P2VAR(AvTp_CVF_RxInfoType) CvfInfoPtr = (AVTP_P2VAR(AvTp_CVF_RxInfoType)) AvTp_RxXPduInfoPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* Get Information for Upper-layer */
  CvfInfoPtr->Subtype = AVTP_SUBTYPE_CVF;
#if (STD_ON == AVTP_RX_PDU_INFO_SUPPORT)
  CvfInfoPtr->MediaClockReset      = (boolean)AVTP_HDR_STREAM_MR(StreamHdrPtr->SvVerMrSsdTv);
  CvfInfoPtr->IsUncertainTimestamp = (boolean)AVTP_HDR_STREAM_TU(StreamHdrPtr->SubtypeSpecificData1Tu);
  CvfInfoPtr->IsValidTimestamp     = (boolean)AVTP_HDR_STREAM_TV(StreamHdrPtr->SvVerMrSsdTv);
  CvfInfoPtr->Timestamp            = AvTp_Ntohl(StreamHdrPtr->Timestamp);
  CvfInfoPtr->SequenceNum          = StreamHdrPtr->SequenceNum;
#endif /* (STD_ON == AVTP_RX_PDU_INFO_SUPPORT */

  /* Set Point to Compressed Video data */
  CvfHdrPtr = (AVTP_P2VAR(AvTp_CVF_MsgType)) &StreamHdrPtr->SubtypeSpecificData2;
#if (STD_ON == AVTP_RX_CHECK)
  if(AvTp_CfgGetRxCmpVideoFormat(StreamIdx) != CvfHdrPtr->Format)
  {
    /* unexpected Format -> drop message */
    return AVTP_MSG_INVALID;
  }
  if(AvTp_CfgGetRxCmpVideoFormatSubtype(StreamIdx) != CvfHdrPtr->FormatSubtype)
  {
    /* unexpected FormatSubtype -> drop message */
    return AVTP_MSG_INVALID;
  }
  /* -> Message is valid */
#else
  if(AVTP_CVF_FMT_RFC != CvfHdrPtr->Format)
  {
    /* Invalid Format -> drop message */
    return AVTP_MSG_INVALID;
  }
  if(AVTP_CVF_FMTSUBTYPE_JPEG2000 < CvfHdrPtr->FormatSubtype)
  {
    /* Invalid FormatSubtype -> drop message */
     return AVTP_MSG_INVALID;
  }
  /* -> Message is valid */
#endif /* (STD_ON == AVTP_RX_CHECK) */

  /* get Marker bit */
  CvfInfoPtr->M = (uint8)AVTP_CVF_M(CvfHdrPtr->PtvMEvt);

  /* get Event */
  CvfInfoPtr->Event = (uint8)AVTP_CVF_EVENT(CvfHdrPtr->PtvMEvt);

#if ((AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) && (AVTP_CVF_H264_SUPPORT == STD_ON))
  /* Get H264 Timestamp if required */
  if(CvfHdrPtr->FormatSubtype == AVTP_CVF_FMTSUBTYPE_H264)
  {
    if((uint8)AVTP_CVF_H264_PTV(CvfHdrPtr->PtvMEvt) == 0x01u)
    {
      CvfInfoPtr->H264TimestampValid = TRUE;
      CvfInfoPtr->H264Timestamp = (uint32)AvTp_Ntohl(CvfHdrPtr->H264Timestamp);
    }
    else
    {
      CvfInfoPtr->H264TimestampValid = FALSE;
      CvfInfoPtr->H264Timestamp = 0u;
    }
  }
#endif /* ((AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) && (AVTP_CVF_H264_SUPPORT == STD_ON)) */

  return AVTP_MSG_VALID;
} /* AvTp_Cvf_Process */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (STD_ON == AVTP_CVF_SUPPORT) */

#if (STD_ON == AVTP_CRF_SUPPORT)
/**********************************************************************************************************************
 *    AvTp_Crf_Process
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean, AVTP_CODE) AvTp_Crf_Process(
#if (STD_ON == AVTP_RX_CHECK)
  AvTp_StreamIdxType StreamIdx,
#endif /* (STD_ON == AVTP_RX_CHECK) */
  AVTP_P2VAR(AvTp_StreamHdrType) StreamHdrPtr,
  AVTP_P2VAR(AvTp_RxXPduInfoType) AvTp_RxXPduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE)
  AVTP_P2VAR(AvTp_Crf_HdrType) CrfPduHdrPtr;
#else
  AVTP_P2VAR(AvTp_CRF_Pdu_MsgType) CrfPduHdrPtr;
#endif /* (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) */
  AVTP_P2VAR(AvTp_CRF_RxInfoType) CrfInfoPtr =
    (AVTP_P2VAR(AvTp_CRF_RxInfoType)) AvTp_RxXPduInfoPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* Get Information for Upper-layer */
  CrfInfoPtr->Subtype = AVTP_SUBTYPE_CRF;
#if (STD_ON == AVTP_RX_PDU_INFO_SUPPORT)
  CrfInfoPtr->MediaClockReset      = (boolean)AVTP_HDR_STREAM_MR(StreamHdrPtr->SvVerMrSsdTv);
  CrfInfoPtr->IsUncertainTimestamp = (boolean)AVTP_HDR_STREAM_TU(StreamHdrPtr->SubtypeSpecificData1Tu);
  CrfInfoPtr->IsValidTimestamp     = (boolean)AVTP_HDR_STREAM_TV(StreamHdrPtr->SvVerMrSsdTv);
  CrfInfoPtr->SequenceNum          = StreamHdrPtr->SequenceNum;
#endif /* (STD_ON == AVTP_RX_PDU_INFO_SUPPORT) */

  /* Set Point to Clock Reference data */
#if (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE)
  CrfPduHdrPtr = (AVTP_P2VAR(AvTp_Crf_HdrType))StreamHdrPtr;
#else
  CrfPduHdrPtr = (AVTP_P2VAR(AvTp_CRF_Pdu_MsgType)) &StreamHdrPtr->StreamDataLen;
#endif /* (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) */

  /* Find corresponding Clock Reference Format Type */
#if (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE)
  switch(CrfPduHdrPtr->Type)
  {
  case AVTP_CRF_TYPE_USER:
  case AVTP_CRF_TYPE_AUDIO_SAMPLE:
  case AVTP_CRF_TYPE_VIDEO_FRAME:
  case AVTP_CRF_TYPE_VIDEO_LINE:
  case AVTP_CRF_TYPE_MACHINE_CYCLE:
# if (STD_ON == AVTP_RX_CHECK)
    if(AvTp_CfgGetRxCrfClockFrequency(StreamIdx) != AVTP_CRF_BASE_FREQUENCY(AvTp_Ntohl(CrfPduHdrPtr->PullBaseFreq)))
    {
      return AVTP_MSG_INVALID;
    }
    if(AvTp_CfgGetRxCrfClockMultiplier(StreamIdx) !=
      (uint8)(AVTP_CRF_PULL(AvTp_Ntohl(CrfPduHdrPtr->PullBaseFreq)) >> AVTP_CRF_PULL_BIT_POS))
    {
      return AVTP_MSG_INVALID;
    }
    if(AvTp_CfgGetRxCrfClockTimestampInterval(StreamIdx) != AvTp_Ntohs(CrfPduHdrPtr->TimestampInterval))
    {
      return AVTP_MSG_INVALID;
    }
    /* -> Msg is valid */
# endif /* (STD_ON == AVTP_RX_CHECK) */
    /* Set Fs and Tu Bits */
    if(AVTP_CRF_FS(CrfPduHdrPtr->SvVerMrRFsTu) != 0u)
    {
      CrfInfoPtr->Fs = TRUE;
    }
    else
    {
      CrfInfoPtr->Tu = FALSE;
    }

    if(AVTP_CRF_TU(CrfPduHdrPtr->SvVerMrRFsTu) != 0u)
    {
      CrfInfoPtr->Tu = TRUE;
    }
    else
    {
      CrfInfoPtr->Tu = FALSE;
    }
    break;
#else
  switch(AvTp_Ntohs(CrfPduHdrPtr->Type))
  {
  case AVTP_CRF_TYPE_USER:
    /* User Specified */
    break;

  case AVTP_CRF_TYPE_AUDIO_SAMPLE:
    /* Audio Sample Timestamp */
    {
      AVTP_P2VAR(AvTp_CRF_Audio_MsgType) CrfAudioHdrPtr;

      /* Set Point to Clock Reference Format data */
      CrfAudioHdrPtr = (AVTP_P2VAR(AvTp_CRF_Audio_MsgType)) &CrfPduHdrPtr->StreamDataLen;
# if (STD_ON == AVTP_RX_CHECK)
      if((uint8) AvTp_CfgGetRxCrfClockFrequency(StreamIdx) != CrfAudioHdrPtr->ClockFrequency)
      {
        /* unexpected Clock Frequency -> drop message */
         return AVTP_MSG_INVALID;
      }
      if(AvTp_CfgGetRxCrfClockMultiplier(StreamIdx) != CrfAudioHdrPtr->ClockMultiplier)
      {
        /* unexpected Clock Multiplier -> drop message */
         return AVTP_MSG_INVALID;
      }
      if(AvTp_CfgGetRxCrfClockTimestampInterval(StreamIdx) != AvTp_Ntohs(CrfAudioHdrPtr->TimestampInterval))
      {
        /* unexpected Timestamp Interval -> drop message */
         return AVTP_MSG_INVALID;
      }
      /* -> Message is valid */
# else
      if(AVTP_CRF_NSR_192KHZ < CrfAudioHdrPtr->ClockFrequency)
      {
        /* unexpected Clock Frequency -> drop message */
         return AVTP_MSG_INVALID;
      }
      if(AVTP_CRF_CM_PAL_VIDEO_PULL_UP < CrfAudioHdrPtr->ClockMultiplier)
      {
        /* unexpected Clock Multiplier -> drop message */
         return AVTP_MSG_INVALID;
      }
      /* -> Message is valid */
# endif /* (STD_ON == AVTP_RX_CHECK) */
    }
    break;

  case AVTP_CRF_TYPE_VIDEO_FRAME:
    /* Video Frame Sync Timestamp */
    {
      AVTP_P2VAR(AvTp_CRF_VideoFrameSync_MsgType) CrfVideoFrameHdrPtr;

      /* Set Point to Clock Reference Format data */
      CrfVideoFrameHdrPtr = (AVTP_P2VAR(AvTp_CRF_VideoFrameSync_MsgType)) &CrfPduHdrPtr->
        StreamDataLen;
# if (STD_ON == AVTP_RX_CHECK)
      if(AvTp_CfgGetRxCrfClockFrequency(StreamIdx) != AvTp_Ntohl(CrfVideoFrameHdrPtr->ClockFrequency))
      {
        /* unexpected Clock Frequency -> drop message */
         return AVTP_MSG_INVALID;
      }
      if(AvTp_CfgGetRxCrfClockMultiplier(StreamIdx) != CrfVideoFrameHdrPtr->ClockMultiplier)
      {
        /* unexpected Clock Multiplier -> drop message */
         return AVTP_MSG_INVALID;
      }
      if(AvTp_CfgGetRxCrfClockTimestampInterval(StreamIdx) != AvTp_Ntohs(CrfVideoFrameHdrPtr->TimestampInterval))
      {
        /* unexpected Timestamp Interval -> drop message */
         return AVTP_MSG_INVALID;
      }
      /* -> Message is valid */
# else
      if(AVTP_CRF_CM_PAL_VIDEO_PULL_UP < CrfVideoFrameHdrPtr->ClockMultiplier)
      {
        /* unexpected Clock Multiplier -> drop message */
         return AVTP_MSG_INVALID;
      }
      /* -> Message is valid */
# endif /* (STD_ON == AVTP_RX_CHECK) ) */
    }
    break;

  case AVTP_CRF_TYPE_VIDEO_LINE:
    /* Video Line Sync Timestamp */
    {
      AVTP_P2VAR(AvTp_CRF_VideoLineSync_MsgType) CrfVideoLineHdrPtr;

      /* Set Point to Clock Reference Format data */
      CrfVideoLineHdrPtr = (AVTP_P2VAR(AvTp_CRF_VideoLineSync_MsgType)) &CrfPduHdrPtr->
        StreamDataLen;
# if (STD_ON == AVTP_RX_CHECK)
      if(AvTp_CfgGetRxCrfClockFrequency(StreamIdx) != AvTp_Ntohl(CrfVideoLineHdrPtr->ClockFrequency))
      {
        /* unexpected Clock Frequency -> drop message */
         return AVTP_MSG_INVALID;
      }
      if(AvTp_CfgGetRxCrfClockMultiplier(StreamIdx) != CrfVideoLineHdrPtr->ClockMultiplier)
      {
        /* unexpected Clock Multiplier -> drop message */
         return AVTP_MSG_INVALID;
      }
      if(AvTp_CfgGetRxCrfClockTimestampInterval(StreamIdx) != AvTp_Ntohs(CrfVideoLineHdrPtr->TimestampInterval))
      {
        /* unexpected Timestamp Interval -> drop message */
         return AVTP_MSG_INVALID;
      }
      /* -> Message is valid */
# else
      if(AVTP_CRF_CM_PAL_VIDEO_PULL_UP < CrfVideoLineHdrPtr->ClockMultiplier)
      {
        /* unexpected Clock Multiplier -> drop message */
         return AVTP_MSG_INVALID;
      }
      /* -> Message is valid */
# endif /* (STD_ON == AVTP_RX_CHECK) */

      /* get frame sync flag */
      CrfInfoPtr->Fs = (boolean) AVTP_CRF_FS(CrfVideoLineHdrPtr->ResFs);
    }
    break;
#endif /* (AVTP_IEEE1722A_CONFORMANCE == AVTP_IEEE1722A_2015_D16_CONFORMANCE) */
  default:
    return AVTP_MSG_INVALID;
  }

   return AVTP_MSG_VALID;
} /* AvTp_Crf_Process */ /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6030 */ /* MD_MSR_STCYC */
/* PRQA S 6010 */ /* MD_MSR_STPTH */
#endif /*  (STD_ON == AVTP_CRF_SUPPORT) */

#if (STD_ON == AVTP_VSF_SUPPORT)
/**********************************************************************************************************************
 *    AvTp_Vsf_Process
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean, AVTP_CODE) AvTp_Vsf_Process(
#if (STD_ON == AVTP_RX_CHECK)
  AvTp_StreamIdxType StreamIdx,
#endif /* (STD_ON == AVTP_RX_CHECK) */
  AVTP_P2VAR(AvTp_StreamHdrType) StreamHdrPtr,
  AVTP_P2VAR(AvTp_RxXPduInfoType) AvTp_RxXPduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  AVTP_P2VAR(AvTp_Vsf_HdrType) VsfHdrPtr;
  AVTP_P2VAR(AvTp_VSF_RxInfoType) VsfInfoPtr = (AVTP_P2VAR(AvTp_VSF_RxInfoType)) AvTp_RxXPduInfoPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* Get Information for Upper-layer */
  VsfInfoPtr->Subtype = AVTP_SUBTYPE_VSF;
#if (STD_ON == AVTP_RX_PDU_INFO_SUPPORT)
  VsfInfoPtr->MediaClockReset      = (boolean)AVTP_HDR_STREAM_MR(StreamHdrPtr->SvVerMrSsdTv);
  VsfInfoPtr->IsUncertainTimestamp = (boolean)AVTP_HDR_STREAM_TU(StreamHdrPtr->SubtypeSpecificData1Tu);
  VsfInfoPtr->IsValidTimestamp     = (boolean)AVTP_HDR_STREAM_TV(StreamHdrPtr->SvVerMrSsdTv);
  VsfInfoPtr->Timestamp            = AvTp_Ntohl(StreamHdrPtr->Timestamp);
  VsfInfoPtr->SequenceNum          = StreamHdrPtr->SequenceNum;
#endif /* (STD_ON == AVTP_RX_PDU_INFO_SUPPORT) */

  /* Set Pointer to Vendor Specific Format Header */
  VsfHdrPtr = (AVTP_P2VAR(AvTp_Vsf_HdrType))StreamHdrPtr;
#if (STD_ON == AVTP_RX_CHECK)
  /* Check if EUI correspond to the configured EUI */
  if((AvTp_CfgGetRxVsfEui1(StreamIdx) != AvTp_Ntohl(VsfHdrPtr->VendorEui1)) ||
    (AvTp_CfgGetRxVsfEui2(StreamIdx) != AvTp_Ntohs(VsfHdrPtr->VendorEui2)))
  {
    return AVTP_MSG_INVALID;
  }
#endif /* (STD_ON == AVTP_RX_CHECK) */

  /* get EUI */
  VsfInfoPtr->VendorEui1 = (uint32)AvTp_Ntohl(VsfHdrPtr->VendorEui1);
  VsfInfoPtr->VendorEui2 = (uint16)AvTp_Ntohs(VsfHdrPtr->VendorEui2);

  return AVTP_MSG_VALID;
}  /* AvTp_Vsf_Process() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (STD_ON == AVTP_VSF_SUPPORT) */

/**********************************************************************************************************************
 *    AvTp_SearchStreamIdx
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(Std_ReturnType, AVTP_CODE) AvTp_SearchStreamIdx(
  AVTP_P2CONST(AvTp_StreamIdType) StreamIdPtr,
  AVTP_P2VAR(AvTp_StreamIdxType) StreamIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
#if (STD_ON == AVTP_BINARY_SEARCH)
  AvTp_CompareType compareResult;
  sint8 left, right, center;
  left = 0;
  right = AVTP_STREAM_CNT_RX - 1;

  while(left <= right)
  {
    /* calculate new center */
    center = left + ((right - left) / 2);

    compareResult = AvTp_CompareStreamID(*((AVTP_P2CONST(AvTp_StreamIdType))&AvTp_RxStreamIdMap[center].StreamId[0]),
                                         *StreamIdPtr);

    if(AVTP_COMPARE_EQUAL == compareResult)
    {
      /* StreamId found */
      *StreamIdxPtr = AvTp_RxStreamIdMap[center].StreamIdx;

      retVal = E_OK;
      break;
    }
    else
    {
      /* elements differ */
      if(AVTP_COMPARE_LESS == compareResult)
      {
        /* keep searching in the right half */
        left = center + 1;
      }
      else
      {
        /* keep searching in the left half */
        right = center - 1;
      }
    }
  }
#else
  uint8 Idx = AVTP_STREAM_CNT_RX;

  while(0 < Idx)
  {
    Idx--;

    if(AVTP_COMPARE_EQUAL == AvTp_CompareStreamID(AvTp_RxStreamIdMap[Idx].StreamId, *StreamIdPtr))
    {
      /* StreamId found */
      *StreamIdxPtr = AvTp_RxStreamIdMap[Idx].StreamIdx;

      retVal = E_OK;
      break;
    }
  }
#endif /* (STD_ON == AVTP_BINARY_SEARCH) */
  return retVal;
} /* AvTp_SearchStreamIdx */

/**********************************************************************************************************************
 *  AvTp_CompareStreamID
 *********************************************************************************************************************/
STATIC FUNC(AvTp_CompareType, AVTP_CODE) AvTp_CompareStreamID(
  CONST(AvTp_StreamIdType, AVTP_CONST) StreamId1, /* PRQA S 3673 */ /* MD_AvTp_16.7 */
  CONST(AvTp_StreamIdType, AVTP_CONST) StreamId2) /* PRQA S 3673 */ /* MD_AvTp_16.7 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 Idx = 0; /* PRQA S 0781 */ /* MD_AvTp_5.6 */
  AVTP_P2VAR(uint8) Stream1U8Ptr = (AVTP_P2VAR(uint8))StreamId1;
  AVTP_P2VAR(uint8) Stream2U8Ptr = (AVTP_P2VAR(uint8))StreamId2;

  /* ----- Implementation ----------------------------------------------- */
  while(Idx < 8)
  {
    if(Stream1U8Ptr[Idx] < Stream2U8Ptr[Idx])
    {
      /* StreamId 1 is smaller than StreamId 2 */
      return AVTP_COMPARE_LESS;
    }
    if(Stream1U8Ptr[Idx] > Stream2U8Ptr[Idx])
    {
      /* StreamId 1 is greater than StreamId 2 */
      return AVTP_COMPARE_GREATER;
    }
    ++Idx;
  }

  /* StreamId 1 and StreamId 2 are equal */
  return AVTP_COMPARE_EQUAL;

} /* AvTp_CompareStreamID */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

#endif /* (0 < AVTP_STREAM_CNT_RX) */

#if (0 < AVTP_STREAM_CNT_TX)
/**********************************************************************************************************************
 *    AvTp_StateChangedFunctions
 *********************************************************************************************************************/
STATIC FUNC(void, AVTP_CODE) AvTp_StateChangedFunctions(AvTp_CtrlIdxType AvTpCtrlIdx, AvTp_StateType NewState)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 Idx = AvTp_Ctrl[AvTpCtrlIdx].TxStreamCnt; /* PRQA S 0781 */ /* MD_AvTp_5.6 */

  /* ----- Implementation ----------------------------------------------- */
  while(0u < Idx)
  {
    Idx--;
    AvTp_TxStreamInfo[AvTp_Ctrl[AvTpCtrlIdx].TxStreamPtr[Idx]].StateChgFunc(NewState);
  }
} /* AvTp_StateChangedFunctions */

#if (STD_ON == AVTP_TIME_SUPPORT)
/**********************************************************************************************************************
 *  AvTp_TimestampPlusTimediff
 *********************************************************************************************************************/
/* PRQA S 6080 FUNC_PARAM_COUNT */ /* Splitting into functions is less efficient */
STATIC FUNC(Std_ReturnType, AVTP_CODE) AvTp_TimestampPlusTimediff(
  AVTP_P2VAR(AvTp_GlobalTimestampType) DestTimestampPtr,
  AVTP_P2CONST(AvTp_TimediffType) TimediffPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_OK;
  AvTp_InternalTimestampType InternalTimestamp;

  /* ----- Implementation ----------------------------------------------- */
  /* Convert Global to internal Timestamp */
  InternalTimestamp.nanoseconds = DestTimestampPtr->nanoseconds;
  InternalTimestamp.seconds     = DestTimestampPtr->seconds;
  InternalTimestamp.seconds    |= ((uint64)DestTimestampPtr->secondsHi << 32);

  /* check for positive value of time difference */
  if(*TimediffPtr > 0U)
  {
    AvTp_UTimediffType TimediffU64Ns = (AvTp_UTimediffType)(*TimediffPtr);
    AvTp_UTimediffType DiffSeconds = (AvTp_UTimediffType)(TimediffU64Ns / AVTP_TIMESTAMP_MAX_NANOSECONDS);
    uint32 DiffNanoseconds = (uint32)(TimediffU64Ns % AVTP_TIMESTAMP_MAX_NANOSECONDS);

     /* overflow check of rounded up seconds -> we don't have to check nanoseconds overflow */
    if((AvTp_UTimediffType)((InternalTimestamp.seconds + 1) + (DiffSeconds + 1)) <=
       (AvTp_UTimediffType)AVTP_TIMESTAMP_MAX_SECONDS)
    {
      /* add nanoseconds of both summands */
      InternalTimestamp.nanoseconds += (uint32)DiffNanoseconds;

      /* add seconds of overflow nanoseconds */
      InternalTimestamp.seconds += (AvTp_UTimediffType)((uint32)(InternalTimestamp.nanoseconds /
                                                           AVTP_TIMESTAMP_MAX_NANOSECONDS));

     /* truncate overflow nanoseconds */
      InternalTimestamp.nanoseconds = InternalTimestamp.nanoseconds % AVTP_TIMESTAMP_MAX_NANOSECONDS;

     /* add seconds */
      InternalTimestamp.seconds += (AvTp_UTimediffType)DiffSeconds;
    }
    else
    {
      RetVal = E_NOT_OK;
    }
  }
  /* check for negative value of time difference */
  else if(*TimediffPtr < 0U)
  {
    AvTp_UTimediffType TimediffU64Ns = (AvTp_UTimediffType)(-(*TimediffPtr));
    AvTp_UTimediffType DiffSeconds = (AvTp_UTimediffType)(TimediffU64Ns / AVTP_TIMESTAMP_MAX_NANOSECONDS);
    uint32 DiffNanoseconds = (uint32)(TimediffU64Ns % AVTP_TIMESTAMP_MAX_NANOSECONDS);

    /* check that result is not negative */
    if(DiffSeconds <= InternalTimestamp.seconds)
    {
      if(DiffNanoseconds <= InternalTimestamp.nanoseconds)
      {
        InternalTimestamp.seconds -= DiffSeconds;
        InternalTimestamp.nanoseconds -= DiffNanoseconds;
      }
      /* handle nanoseconds underflow */
      else
      {
        /* destination seconds must greater than diff seconds due to underflow */
        if(InternalTimestamp.seconds > DiffSeconds)
        {
          /* decrement seconds due to underflow of nanoseconds */
          InternalTimestamp.seconds--;
          InternalTimestamp.nanoseconds =
            (uint32)(((AVTP_TIMESTAMP_MAX_NANOSECONDS + InternalTimestamp.nanoseconds) - DiffNanoseconds));
          /* subtract seconds */
          InternalTimestamp.seconds -= DiffSeconds;
        }
        /* negative timestamp value */
        else
        {
          RetVal = E_NOT_OK;
        }
      }
    }
    else
    {
      RetVal = E_NOT_OK;
    }
  }
  /* check for negative value of time difference */
  else
  {
   /* time difference is zero -> add nothing */
  }

  /* Convert Internal to global Timestamp */
  DestTimestampPtr->nanoseconds = InternalTimestamp.nanoseconds;
  DestTimestampPtr->seconds     = (uint32)InternalTimestamp.seconds;
  DestTimestampPtr->secondsHi   = (uint16)(InternalTimestamp.seconds >> 32);

  return RetVal;
}/* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
/* PRQA L: FUNC_PARAM_COUNT */
# endif /* (STD_ON == AVTP_TIME_SUPPORT) */

/**********************************************************************************************************************
 *    AvTp_GetCurrentTime
 *********************************************************************************************************************/
# if (STD_ON == AVTP_TIME_SUPPORT)
STATIC FUNC(Std_ReturnType, AVTP_CODE) AvTp_GetCurrentTime(AvTp_StreamIdxType StreamIdx,
  AVTP_P2VAR(AvTp_GlobalTimestampType) TimestampPtr, AVTP_P2VAR(AvTp_SyncStateType) SyncStatePtr)
{
  Std_ReturnType RetVal = E_NOT_OK;
  switch(AvTp_TxStreamInfo[StreamIdx].TimeSource)
  {
#  if (AVTP_STBM_SUPPORT == STD_ON)
  case AVTP_TIME_SOURCE_STBM:
    {
      StbM_TimeStampType stbmTimestamp;
      StbM_UserDataType userData;

      if(StbM_GetCurrentTime(AvTp_TxStreamInfo[StreamIdx].TimeBaseId, &stbmTimestamp, &userData) == E_OK)
      {
        TimestampPtr->secondsHi = stbmTimestamp.secondsHi;
        TimestampPtr->seconds = stbmTimestamp.seconds;
        TimestampPtr->nanoseconds = stbmTimestamp.nanoseconds;

        /* Get Sync State from StbM TimeBase status */
        AvTp_TimeBaseStatusToSyncStatus(stbmTimestamp.timeBaseStatus, SyncStatePtr);
        RetVal = E_OK;
      }
    }
    break;
#  endif /* (AVTP_STBM_SUPPORT == STD_ON) */

#  if (AVTP_USER_CALLOUT_SUPPORT == STD_ON)
  case AVTP_TIME_SOURCE_USER_CALLOUT:
    if(AvTp_TxStreamInfo[StreamIdx].UserCalloutFct != NULL_PTR)
    {
      RetVal = AvTp_TxStreamInfo[StreamIdx].UserCalloutFct(
        TimestampPtr,
        SyncStatePtr);
    }
    break;
#  endif /* (AVTP_USER_CALLOUT_SUPPORT == STD_ON) */

  default:
    /* Nothing happens */
    break;
  }

  return RetVal;
} /* AvTp_GetCurrentTime() */
# endif /* (STD_ON == AVTP_TIME_SUPPORT) */

/**********************************************************************************************************************
 *    AvTp_TimeBaseStatusToSyncStatus
 *********************************************************************************************************************/
# if (AVTP_STBM_SUPPORT == STD_ON)
LOCAL_INLINE FUNC(void, AVTP_CODE) AvTp_TimeBaseStatusToSyncStatus(StbM_TimeBaseStatusType TimeBaseStatus,
  AVTP_P2VAR(AvTp_SyncStateType) SyncStatePtr)
{
  if((TimeBaseStatus & AVTP_TIME_BASE_STATUS_GLOBAL_TIME_BASE_MASK) == 0)
  {
    /* Never Sync */
    (*SyncStatePtr) = AVTP_PORT_STATE_NEVERSYNC;
  }
  else
  {
    /* Synced at least once */
    if((TimeBaseStatus & AVTP_TIME_BASE_STATUS_TIMEOUT_MASK) == 0)
    {
      /* No Timeout */
      if((TimeBaseStatus & AVTP_TIME_BASE_STATUS_TIMELEAP_MASK) == 0)
      {
        /* No Timeleap occurred -> Sync */
        (*SyncStatePtr) = AVTP_PORT_STATE_SYNC;
      }
      else
      {
        /* Timeleap occurred -> Unsync */
        (*SyncStatePtr) = AVTP_PORT_STATE_UNSYNC;
      }
    }
    else
    {
      /* Timeout occurred -> State is uncertain */
      (*SyncStatePtr) = AVTP_PORT_STATE_UNCERTAIN;
    }
  }
} /* AvTp_TimeBaseStatusToSyncStatus() */
# endif /* (AVTP_STBM_SUPPORT == STD_ON) */
#endif /* (0 < AVTP_STREAM_CNT_TX) */

#define AVTP_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* PRQA L:TYPE_CAST */

/* module specific MISRA deviations:
  MD_AvTp_11.4:
      Reason:     Casting to different object pointer type
      Risk:       misinterpretation of data
      Prevention: Covered by code review.
  MD_AvTp_12.4:
      Reason:     The right hand operand of a logical && or || operator shall not contain side effects.
      Risk:       No Risk. The right hand operand shall only be evaluated when the left hand operand is valid.
      Prevention: Covered by code review.
  MD_AvTp_16.7:
      Reason:     The parameter is not modified and so the pointer could be of type 'pointer to const'
      Risk:       No risk. The object addressed by the pointer parameter is of type 'pointer to const'
      Prevention: Covered by code review.
  MD_AvTp_5.6:
     Reason:     Readablitiy is increased by using well known names.
     Risk:       Elements can be confused with each other.
     Prevention: Covered by code review.
  MD_AvTp_STPAR_6060:
     Reason:     The number of function parameters can not be reduced without extra copy actions to put several
                 parameters into one structure.
     Risk:       Some restricted compilers might not be able to compile the code.
     Prevention: Covered by code review.
  MD_AvTp_CANoeEmu: Use of basic type 'char'.
    Reason:     This type is only used in CANoe based debugging environments
    Risk:       This code will not be compiled for embedded environments.
    Prevention: Disable CANoeEmu
*/
/**********************************************************************************************************************
 *  END OF FILE: AvTp.C
 *********************************************************************************************************************/
