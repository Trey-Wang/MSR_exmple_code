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
/**        \file  EthIf_Switch.c
 *        \brief  Ethernet Interface Ethernet switch abstraction source file
 *
 *      \details  Component to abstract the underlying Ethernet hardware for the upper layers to provide a generic
 *                interface to be used for controlling and communicate with the help of the Ethernet hardware
 *                provided by an ECU (Ethernet controller, Ethernet transceiver, Ethernet switch).
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

#define ETHIF_SWITCH_SOURCE

/*lint -e438 */ /* Suppress ID438 because Config pointer only used in Post-Build Variant */
/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */
/*lint -e506 */ /* Suppress ID506 due to MD_MSR_14.1 */
/*lint -e537 */ /* Suppress ID537 due to MD_MSR_19.1 */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthIf.h"
#include "EthIf_CfgAccess_Int.h"
#include "EthIf_Switch.h"
#include "EthIf_Types.h"
#include "EthIf_Int.h"
#include "EthIf_Switch_Cbk.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of EthIf header file */
/* \trace SPEC-2393557 */
#if (  (ETHIF_SW_MAJOR_VERSION != (8u)) \
    || (ETHIF_SW_MINOR_VERSION != (0u)) \
    || (ETHIF_SW_PATCH_VERSION != (1u)) )
# error "Vendor specific version numbers of EthIf_Switch.c and EthIf.h are inconsistent"
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

#if !defined (ETHIF_LOCAL)
# define ETHIF_LOCAL static
#endif

#if !defined (ETHIF_LOCAL_INLINE)
# define ETHIF_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define ETHIF_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( ETHIF_ETHSWT_TIME_STAMP_SUPPORT == STD_ON )
/**********************************************************************************************************************
 * EthIf_RestoreSwtTimeStampRxInfo
 **********************************************************************************************************************/
/*! \brief      Restores time stamp related information stored during reception of a frame
 *  \details    Function restores information related to a time stamp for being able to pass the time stamp to the
 *              designated upper layer.
 *  \param[in]  ethCtrlId         Eth controller the time stamp is related to
 *  \param[in]  ctxtFlag          Flag indicating the time stamp context (ingress or egress time stamp)
 *  \param[in]  frameId           Identifier of the frame the time stamp is related to
 *  \param[out] ethIfCtrlIdxPtr   EthIf controller the frame related to the time stamp was passed to
 *  \param[out] frameOwnerIdxPtr  User the frame related to the time stamp was passed to
 *  \param[out] dataPtr           Pointer to the payload of the frame related to the time stamp passed to the user
 *  \return     TRUE - there was information related to the time stamp stored previously
 *  \return     FALSE - there was no information related to the time stamp stored previously
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_RestoreSwtTimeStampRxInfo(
        uint8                                ethCtrlIdx,
        EthIf_SwtTimeStampProcFlagsType      ctxtFlag,
        uint16                               frameId,
  P2VAR(uint8,  AUTOMATIC, AUTOMATIC)        ethIfCtrlIdxPtr,
  P2VAR(uint8,  AUTOMATIC, AUTOMATIC)        frameOwnerIdxPtr,
  P2VAR(uint8*, AUTOMATIC, AUTOMATIC)        dataPtr);

/**********************************************************************************************************************
 * EthIf_RestoreSwtTimeStampTxInfo
 **********************************************************************************************************************/
 /*! \brief      Restores time stamp related information stored during transmission of a frame
  *  \details    Function restores information related to a time stamp for being able to pass the time stamp to the
  *              designated upper layer.
  *  \param[in]  ethCtrlId         Eth controller the time stamp is related to
  *  \param[in]  ctxtFlag          Flag indicating the time stamp context (ingress or egress time stamp)
  *  \param[in]  frameId           Identifier of the frame the time stamp is related to
  *  \param[out] ethIfCtrlIdxPtr   EthIf controller the frame related to the time stamp was passed to
  *  \param[out] frameOwnerIdxPtr  User the frame related to the time stamp was passed to
  *  \param[out] dataPtr           Pointer to the payload of the frame related to the time stamp passed to the user
  *  \return     TRUE - there was information related to the time stamp stored previously
  *  \return     FALSE - there was no information related to the time stamp stored previously
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_RestoreSwtTimeStampTxInfo(
        uint8                                ethCtrlIdx,
        EthIf_SwtTimeStampProcFlagsType      ctxtFlag,
        uint16                               frameId,
  P2VAR(uint8,  AUTOMATIC, AUTOMATIC)        ethIfCtrlIdxPtr,
  P2VAR(uint8,  AUTOMATIC, AUTOMATIC)        frameOwnerIdxPtr,
  P2VAR(uint8*, AUTOMATIC, AUTOMATIC)        dataPtr);

/**********************************************************************************************************************
 * EthIf_GetSwtTimeStampTxInfo
 **********************************************************************************************************************/
/*! \brief      Retrieves a time stamp handling data element related to frame buffer
 *  \details    Function retrieves the transmission time stamp handling data element related to the given buffer if
 *              available.
 *  \param[in]  ethCtrlIdx  Eth controller the frame buffer is related to
 *  \param[in]  bufIdx      Identifier of the buffer in context of the Eth controller
 *  \param[in]  frameId     Ethernet frame identifier
 *  \param[out] txInfoPtr   Pointer to the retrieved tx time stamp handling data element
 *  \return     TRUE - there was a data element stored for the frame buffer
 *  \return     FALSE - there wasn't a data element stored for the frame buffer
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_GetSwtTimeStampTxInfo(
        uint8                                                        ethCtrlIdx,
        uint8                                                        bufIdx,
        Eth_FrameIdType                                              frameId,
  P2VAR(EthIf_SwtTimeStampTxCtxtPoolElemType*, AUTOMATIC, AUTOMATIC) txInfoPtr);

/**********************************************************************************************************************
 * EthIf_ArmSwtTimeStampTxInfo
 **********************************************************************************************************************/
/*! \brief      Sets up the data element for later handling of time stamp information
 *  \details    Function sets up the passed data element so it is possible to handle time stamp information indicated
 *              by the Eth switch driver later on.
 *  \param[in]  txInfoPtr   Pointer to the tx time stamp handling data element
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_ArmSwtTimeStampTxInfo(
  P2VAR(EthIf_SwtTimeStampTxCtxtPoolElemType, AUTOMATIC, AUTOMATIC) txInfoPtr);
#endif /* ETHIF_ETHSWT_TIME_STAMP_SUPPORT */

#if ( ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT == STD_ON )
/**********************************************************************************************************************
 * EthIf_RestoreSwtMgmtRxInfo
 **********************************************************************************************************************/
/*! \brief      Restores frame management related information stored during reception of a frame
 *  \details    Function restores information related to management information for being able to pass the information
 *              to the designated upper layer.
 *  \param[in]  ethCtrlId         Eth controller the management information is related to
 *  \param[in]  frameId           Identifier of the frame the management information is related to
 *  \param[out] ethIfCtrlIdxPtr   EthIf controller the frame related to the management information was passed to
 *  \param[out] frameOwnerIdxPtr  User the frame related to the management information was passed to
 *  \param[out] dataPtr           Pointer to the payload of the frame related to the management information passed to
 *                                the user
 *  \return     TRUE - there was information related to the management information stored previously
 *  \return     FALSE - there was no information related to the management information stored previously
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_RestoreSwtMgmtRxInfo(
        uint8                                ethCtrlIdx,
        uint16                               frameId,
  P2VAR(uint8,  AUTOMATIC, AUTOMATIC)        ethIfCtrlIdxPtr,
  P2VAR(uint8,  AUTOMATIC, AUTOMATIC)        frameOwnerIdxPtr,
  P2VAR(uint8*, AUTOMATIC, AUTOMATIC)        dataPtr);
#endif /* ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if ( ETHIF_ETHSWT_TIME_STAMP_SUPPORT == STD_ON )
/**********************************************************************************************************************
 * EthIf_RestoreSwtTimeStampRxInfo
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_RestoreSwtTimeStampRxInfo(
        uint8                                ethCtrlIdx,
        EthIf_SwtTimeStampProcFlagsType      ctxtFlag,
        uint16                               frameId,
  P2VAR(uint8,  AUTOMATIC, AUTOMATIC)        ethIfCtrlIdxPtr,
  P2VAR(uint8,  AUTOMATIC, AUTOMATIC)        frameOwnerIdxPtr,
  P2VAR(uint8*, AUTOMATIC, AUTOMATIC)        dataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(EthIf_SwtTimeStampRxCtxtHandlingType, AUTOMATIC, ETHIF_CONST) rxCtxtHndl;
          uint8                                                         poolElemIdx;
  boolean result;
  /* ----- Implementation ----------------------------------------------- */
  result = FALSE;
  rxCtxtHndl = &EthIf_CfgAccess_SwtTimeStampRxCtxtHandling(ethCtrlIdx);

  /* #10 Enter exclusive area (Reason: Ensure consistency of switch reception timestamp elements) */
  EthIf_EnterExclusiveArea(SWT_TIME_STAMP_RX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Query for the information related to the frame */
  for( poolElemIdx = 0; poolElemIdx < rxCtxtHndl->PoolSize; poolElemIdx++ )
  {
   if( rxCtxtHndl->Pool[poolElemIdx].FrameId == frameId )
   {
     /* #200 Entry found -> retrieve the information store for being able to pass the time stamp to the
      *      designated upper layer */
     (*ethIfCtrlIdxPtr)  = rxCtxtHndl->Pool[poolElemIdx].EthIfCtrlIdx;
     (*frameOwnerIdxPtr) = rxCtxtHndl->Pool[poolElemIdx].FrameOwnerIdx;
     (*dataPtr)          = rxCtxtHndl->Pool[poolElemIdx].DataPtr;
     /* #210 Check if information is restored for both ingress and egress time stamp indication.
      *      If so, invalidate the information, otherwise keep it for restoring the information
      *      in the other context. */
     rxCtxtHndl->Pool[poolElemIdx].ProcFlags = (EthIf_SwtTimeStampProcFlagsType)((uint32)rxCtxtHndl->Pool[poolElemIdx].ProcFlags & ~(uint32)ctxtFlag);
     if( rxCtxtHndl->Pool[poolElemIdx].ProcFlags == 0x00U )
     {
       rxCtxtHndl->Pool[poolElemIdx].DataPtr = NULL_PTR;
       rxCtxtHndl->Pool[poolElemIdx].FrameId = ETH_INVALID_FRAME_ID;
     }
     result = TRUE;
     break;
   }
  }

  /* #30 Leave exclusive area -> processing on elements finished */
  EthIf_ExitExclusiveArea(SWT_TIME_STAMP_RX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return result;
} /* EthIf_RestoreSwtTimeStampRxInfo() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
 * EthIf_RestoreSwtTimeStampTxInfo
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_RestoreSwtTimeStampTxInfo(
        uint8                                ethCtrlIdx,
        EthIf_SwtTimeStampProcFlagsType      ctxtFlag,
        uint16                               frameId,
  P2VAR(uint8,  AUTOMATIC, AUTOMATIC)        ethIfCtrlIdxPtr,
  P2VAR(uint8,  AUTOMATIC, AUTOMATIC)        frameOwnerIdxPtr,
  P2VAR(uint8*, AUTOMATIC, AUTOMATIC)        dataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(EthIf_SwtTimeStampTxCtxtHandlingType, AUTOMATIC, ETHIF_CONST) txCtxtHndl;
          uint8                                                         poolElemIdx;
  boolean result;
  /* ----- Implementation ----------------------------------------------- */
  result = FALSE;
  txCtxtHndl = &EthIf_CfgAccess_SwtTimeStampTxCtxtHandling(ethCtrlIdx);

  /* #10 Enter exclusive area (Reason: Ensure consistency of switch transmission timestamp elements) */
  EthIf_EnterExclusiveArea(SWT_TIME_STAMP_TX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Query for the information related to the frame */
  for( poolElemIdx = 0; poolElemIdx < txCtxtHndl->PoolSize; poolElemIdx++ )
  {
    if( txCtxtHndl->Pool[poolElemIdx].FrameId == frameId )
    {
      /* #200 Entry found -> retrieve the information store for being able to pass the time stamp to the
       *      designated upper layer */
      (*ethIfCtrlIdxPtr)  = txCtxtHndl->Pool[poolElemIdx].EthIfCtrlIdx;
      (*frameOwnerIdxPtr) = txCtxtHndl->Pool[poolElemIdx].FrameOwnerIdx;
      (*dataPtr)          = txCtxtHndl->Pool[poolElemIdx].DataPtr;
      /* #210 Check if information is restored for both ingress and egress time stamp indication.
       *      If so, invalidate the information, otherwise keep it for restoring the information
       *      in the other context. */
      txCtxtHndl->Pool[poolElemIdx].ProcFlags = (EthIf_SwtTimeStampProcFlagsType)((uint32)txCtxtHndl->Pool[poolElemIdx].ProcFlags & ~(uint32)ctxtFlag);
      if( txCtxtHndl->Pool[poolElemIdx].ProcFlags == 0x00U )
      {
        txCtxtHndl->Pool[poolElemIdx].DataPtr = NULL_PTR;
        txCtxtHndl->Pool[poolElemIdx].FrameId = ETH_INVALID_FRAME_ID;
        txCtxtHndl->Pool[poolElemIdx].BufIdx  = EthIf_CfgAccess_InvalidTxBufIdx();
      }
      result = TRUE;
      break;
    }
  }

  /* #30 Leave exclusive area -> Processing of elements finished */
  EthIf_ExitExclusiveArea(SWT_TIME_STAMP_TX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return result;
} /* EthIf_RestoreSwtTimeStampTxInfo() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
 * EthIf_GetSwtTimeStampTxInfo
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_GetSwtTimeStampTxInfo(
        uint8                                                        ethCtrlIdx,
        uint8                                                        bufIdx,
        Eth_FrameIdType                                              frameId,
  P2VAR(EthIf_SwtTimeStampTxCtxtPoolElemType*, AUTOMATIC, AUTOMATIC) txInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(EthIf_SwtTimeStampTxCtxtHandlingType, AUTOMATIC, ETHIF_CONST) txCtxtHndl;
          uint8                                                         poolElemIdx;
          boolean                                                       result;

  /* ----- Implementation ----------------------------------------------- */
  result     = FALSE;
  txCtxtHndl = &EthIf_CfgAccess_SwtTimeStampTxCtxtHandling(ethCtrlIdx);

  /* #10 Enter exclusive area (Reason: Ensure consistency of switch transmission timestamp elements) */
  EthIf_EnterExclusiveArea(SWT_TIME_STAMP_TX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Query for the information related to the frame buffer */
  for( poolElemIdx = 0; poolElemIdx < txCtxtHndl->PoolSize; poolElemIdx++ )
  {
    if( (txCtxtHndl->Pool[poolElemIdx].BufIdx   == bufIdx) &&
        (txCtxtHndl->Pool[poolElemIdx].FrameId == frameId)    )
    {
      (*txInfoPtr) = &txCtxtHndl->Pool[poolElemIdx];
      result       = TRUE;
      break;
    }
  }

  /* #30 Leave exclusive area -> Processing of elements finished */
  EthIf_ExitExclusiveArea(SWT_TIME_STAMP_TX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return result;
} /* EthIf_GetSwtTimeStampTxInfo() */

/**********************************************************************************************************************
 * EthIf_ArmSwtTimeStampTxInfo
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_ArmSwtTimeStampTxInfo(
  P2VAR(EthIf_SwtTimeStampTxCtxtPoolElemType, AUTOMATIC, AUTOMATIC) txInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter exclusive area (Reason: Ensure consistency of switch transmission timestamp elements) */
  EthIf_EnterExclusiveArea(SWT_TIME_STAMP_TX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Set the flags to indicate the context the information wasn't retrieved for yet. */
  txInfoPtr->ProcFlags = (ETHIF_SWT_TIMESTAMP_PROC_FLAG_INGRESS_TS_PENDING |
                          ETHIF_SWT_TIMESTAMP_PROC_FLAG_EGRESS_TS_PENDING    );

  /* #30 Leave exclusive area -> Processing of elements finished */
  EthIf_ExitExclusiveArea(SWT_TIME_STAMP_TX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* EthIf_ArmSwtTimeStampTxInfo() */
#endif /* ETHIF_ETHSWT_TIME_STAMP_SUPPORT */

#if ( ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT == STD_ON )
/**********************************************************************************************************************
 * EthIf_RestoreSwtMgmtRxInfo
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_RestoreSwtMgmtRxInfo(
        uint8                                ethCtrlIdx,
        uint16                               frameId,
  P2VAR(uint8,  AUTOMATIC, AUTOMATIC)        ethIfCtrlIdxPtr,
  P2VAR(uint8,  AUTOMATIC, AUTOMATIC)        frameOwnerIdxPtr,
  P2VAR(uint8*, AUTOMATIC, AUTOMATIC)        dataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(EthIf_MgmtRxCtxtPoolElemType, AUTOMATIC, ETHIF_VAR_NOINIT) rxCtxtHndlPool;
          uint8                                                    poolElemIdx;
  boolean result;
  /* ----- Implementation ----------------------------------------------- */
  result = FALSE;
  rxCtxtHndlPool = EthIf_CfgAccess_MgmtRxCtxtHandling(ethCtrlIdx);

  /* #10 Enter exclusive area (Reason: Ensure consistency of switch transmission timestamp elements) */
  EthIf_EnterExclusiveArea(SWT_TIME_STAMP_TX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Query for the information related to the frame */
  for( poolElemIdx = 0; poolElemIdx < ETHIF_MAX_PARALELL_RX_CONTEXT_PER_ETHCTRL; poolElemIdx++ )
  {
    if( rxCtxtHndlPool[poolElemIdx].FrameId == frameId )
    {
      /* #200 Entry found -> retrieve the information store for being able to pass the time stamp to the
       *      designated upper layer */
      (*dataPtr)          = rxCtxtHndlPool[poolElemIdx].DataPtr;
      (*ethIfCtrlIdxPtr)  = rxCtxtHndlPool[poolElemIdx].EthIfCtrlIdx;
      (*frameOwnerIdxPtr) = rxCtxtHndlPool[poolElemIdx].FrameOwnerIdx;
      /* #210 Information was restored so element can be invalidated */
      rxCtxtHndlPool[poolElemIdx].DataPtr       = NULL_PTR;
      rxCtxtHndlPool[poolElemIdx].FrameId       = ETH_INVALID_FRAME_ID;
      rxCtxtHndlPool[poolElemIdx].EthIfCtrlIdx  = EthIf_CfgAccess_InvalidEthIfCtrlIdx();
      rxCtxtHndlPool[poolElemIdx].FrameOwnerIdx = EthIf_CfgAccess_InvalidFrameOwnerIdx();
      result = TRUE;
      break;
    }
  }

  /* #30 Leave exclusive area -> Processing of elements finished */
  EthIf_ExitExclusiveArea(SWT_TIME_STAMP_TX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return result;
} /* EthIf_RestoreSwtTimeStampTxInfo() */
#endif /* ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#if ( ETHIF_ETHSWT_USED == STD_ON )

# if ( ETHIF_GET_PORT_MAC_ADDR_API == STD_ON )
/**********************************************************************************************************************
 * EthIf_GetPortMacAddr
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetPortMacAddr(
  P2CONST(uint8,              AUTOMATIC, ETHIF_APPL_DATA)  MacAddrPtr,
  P2VAR(EthSwt_SwitchIdxType, AUTOMATIC, ETHIF_APPL_DATA)  SwitchIdxPtr,
  P2VAR(EthSwt_PortIdxType,   AUTOMATIC, ETHIF_APPL_DATA)  PortIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393875 */
    /* \trace SPEC-2393877 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( MacAddrPtr == NULL_PTR )
  {
    /* \trace SPEC-2393876 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( SwitchIdxPtr == NULL_PTR )
  {
    /* \trace SPEC-2393876 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( PortIdxPtr == NULL_PTR )
  {
    /* \trace SPEC-2393876 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
#  endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_EthSwtDrvType, AUTOMATIC, ETHIF_CONST) ethSwtDrv;
            uint8_least                                  ethSwtDrvIdx;

    /* #30 For each Ethernet switch driver managed by EthIf */
    for( ethSwtDrvIdx = 0; ethSwtDrvIdx < EthIf_CfgAccess_EthSwtDrvNum(); ethSwtDrvIdx++ )
    {
      ethSwtDrv = &EthIf_CfgAccess_EthSwtDrvSummary(ethSwtDrvIdx);

      /* #300 Check if driver provides the API */
      if( ethSwtDrv->DrvApi->GetPortMacAddr != NULL_PTR )
      {
        /* #3000 Redirect call to the driver */
        /* \trace SPEC-2394044 */
        retVal = ethSwtDrv->DrvApi->GetPortMacAddr(MacAddrPtr,
                                                   SwitchIdxPtr,
                                                   PortIdxPtr);

        /* #3010 Check if call to driver was successful -> port information found */
        if( retVal == E_OK )
        {
          uint8_least  ethIfSwtIdx;
          uint8_least  ethIfSwtNextRangeIdx;

          retVal = E_NOT_OK;

          /* #30100 Transform the switch index from the driver context to the EthIf context */
          /* define the end index of the search range */
          ethIfSwtNextRangeIdx = EthIf_CfgAccess_EthSwtNum();
#if ( ETHIF_ETHSWT_DRIVERS_NUM > 1U )
          if( ethSwtDrvIdx < (EthIf_CfgAccess_EthSwtDrvNum() - 1U) )
          {
            ethIfSwtNextRangeIdx = EthIf_CfgAccess_EthSwtDrvSummary(ethSwtDrvIdx + 1U).SummaryOffset;
          }
#endif /* ETHIF_ETHSWT_DRIVERS_NUM */

          /* search for a matching driver local switch index within the EthIf switch index search range */
          for( ethIfSwtIdx = ethSwtDrv->SummaryOffset; ethIfSwtIdx < ethIfSwtNextRangeIdx; ethIfSwtIdx++ )
          {
            /* check if the driver context switch index matches */
            if( EthIf_CfgAccess_EthSwtSummary(ethIfSwtIdx)->SwitchIdx == (*SwitchIdxPtr) )
            {
              (*SwitchIdxPtr) = (uint8)ethIfSwtIdx;
              retVal = E_OK;
              break;
            }
          }

          /* leave outer loop if transformation to EthIf context was done */
          if( retVal == E_OK )
          {
            break;
          }
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_PORT_MAC_ADDR, errorId);
  }
#  else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#  endif

  return retVal;
} /* EthIf_GetPortMacAddr() */ /* PRQA S 6010,6030,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STMIF */
# endif /* ETHIF_GET_PORT_MAC_ADDR_API */

# if ( ETHIF_GET_ARL_TABLE_API == STD_ON )
/**********************************************************************************************************************
 * EthIf_GetArlTable
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetArlTable(
        EthSwt_SwitchIdxType                             SwitchIdx,
  P2VAR(uint32,              AUTOMATIC, ETHIF_APPL_VAR)  LenPtr,
  P2VAR(EthSwt_MacVlanType,  AUTOMATIC, ETHIF_APPL_VAR)  ArlTablePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393880 */
    /* \trace SPEC-2393882 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( SwitchIdx >= EthIf_CfgAccess_EthSwtNum()  )
  {
    errorId = ETHIF_E_INV_SWITCH_IDX;
  }
  else if( LenPtr == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( ArlTablePtr == NULL_PTR )
  {
    /* \trace SPEC-2393881 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
#  endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_EthSwtType, AUTOMATIC, ETHIF_CONST) ethSwt;

    ethSwt = EthIf_CfgAccess_EthSwtSummary(SwitchIdx);

    /* #30 Check if driver provides the API */
    if( ethSwt->DrvApi->GetArlTable != NULL_PTR )
    {
      /* #300 Redirect call to the driver */
      /* \trace SPEC-2394046 */
      retVal = ethSwt->DrvApi->GetArlTable(ethSwt->SwitchIdx,
                                           LenPtr,
                                           ArlTablePtr);
    }
#  if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
#  endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_ARL_TABLE, errorId);
  }
#  else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#  endif

  return retVal;
} /* EthIf_GetArlTable() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif /* ETHIF_GET_ARL_TABLE_API */

# if ( ETHIF_GET_BUFFER_LEVEL_API == STD_ON )
/**********************************************************************************************************************
 * EthIf_GetBufferLevel
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetBufferLevel(
        EthSwt_SwitchIdxType                                SwitchIdx,
  P2VAR(EthSwt_BufferLevelType, AUTOMATIC, ETHIF_APPL_VAR)  SwitchBufferLevelPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393885 */
    /* \trace SPEC-2393887 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( SwitchIdx >= EthIf_CfgAccess_EthSwtNum()  )
  {
    errorId = ETHIF_E_INV_SWITCH_IDX;
  }
  else if( SwitchBufferLevelPtr == NULL_PTR )
  {
    /* \trace SPEC-2393886 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
#  endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_EthSwtType, AUTOMATIC, ETHIF_CONST) ethSwt;

    ethSwt = EthIf_CfgAccess_EthSwtSummary(SwitchIdx);

    /* #30 Check if driver provides the API */
    if( ethSwt->DrvApi->GetBufferLevel != NULL_PTR )
    {
      /* #300 Redirect call to the driver */
      /* \trace SPEC-2394048 */
      retVal = ethSwt->DrvApi->GetBufferLevel(ethSwt->SwitchIdx,
                                              SwitchBufferLevelPtr);
    }
#  if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
#  endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_BUFFER_LEVEL, errorId);
  }
#  else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#  endif

  return retVal;
} /* EthIf_GetBufferLevel() */
# endif /* ETHIF_GET_BUFFER_LEVEL_API */

# if ( ETHIF_GET_DROP_COUNT_API == STD_ON )
/**********************************************************************************************************************
 * EthIf_GetDropCount
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetDropCount(
        EthSwt_SwitchIdxType                             SwitchIdx,
  P2VAR(uint16,              AUTOMATIC, ETHIF_APPL_VAR)  LenPtr,
  P2VAR(uint32,              AUTOMATIC, ETHIF_APPL_VAR)  DropCountPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393890 */
    /* \trace SPEC-2393892 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( SwitchIdx >= EthIf_CfgAccess_EthSwtNum()  )
  {
    errorId = ETHIF_E_INV_SWITCH_IDX;
  }
  else if( LenPtr == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( DropCountPtr == NULL_PTR )
  {
    /* \trace SPEC-2393891 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
#  endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_EthSwtType, AUTOMATIC, ETHIF_CONST) ethSwt;

    ethSwt = EthIf_CfgAccess_EthSwtSummary(SwitchIdx);

    /* #30 Check if driver provides the API */
    if( ethSwt->DrvApi->GetDropCount != NULL_PTR )
    {
      /* #300 Redirect call to the driver */
      /* \trace SPEC-2394050 */
      retVal = ethSwt->DrvApi->GetDropCount(ethSwt->SwitchIdx,
                                            LenPtr,
                                            DropCountPtr);
    }
#  if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
#  endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_DROP_COUNT, errorId);
  }
#  else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#  endif

  return retVal;
} /* EthIf_GetDropCount() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif /* ETHIF_GET_DROP_COUNT_API */

# if ( ETHIF_STORE_CONFIG_API == STD_ON )
/**********************************************************************************************************************
 * EthIf_StoreConfiguration
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_StoreConfiguration(
  EthSwt_SwitchIdxType  SwitchIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393895 */
    /* \trace SPEC-2393896 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( SwitchIdx >= EthIf_CfgAccess_EthSwtNum()  )
  {
    errorId = ETHIF_E_INV_SWITCH_IDX;
  }
  else
#  endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_EthSwtType, AUTOMATIC, ETHIF_CONST) ethSwt;

    ethSwt = EthIf_CfgAccess_EthSwtSummary(SwitchIdx);

    /* #30 Check if driver provides the API */
    if( ethSwt->DrvApi->StoreConfiguration != NULL_PTR )
    {
      /* #300 Redirect call to the driver */
      retVal = ethSwt->DrvApi->StoreConfiguration(ethSwt->SwitchIdx);
    }
#  if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
#  endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_STORE_CONFIGURATION, errorId);
  }
#  else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#  endif

  return retVal;
} /* EthIf_StoreConfiguration() */
# endif /* ETHIF_STORE_CONFIG_API */

# if ( ETHIF_RESET_CONFIG_API == STD_ON )
/**********************************************************************************************************************
 * EthIf_ResetConfiguration
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_ResetConfiguration(
  EthSwt_SwitchIdxType  SwitchIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393899 */
    /* \trace SPEC-2393900 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( SwitchIdx >= EthIf_CfgAccess_EthSwtNum()  )
  {
    errorId = ETHIF_E_INV_SWITCH_IDX;
  }
  else
#  endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_EthSwtType, AUTOMATIC, ETHIF_CONST) ethSwt;

    ethSwt = EthIf_CfgAccess_EthSwtSummary(SwitchIdx);

    /* #30 Check if driver provides the API */
    if( ethSwt->DrvApi->ResetConfiguration != NULL_PTR )
    {
      /* #300 Redirect call to the driver */
      retVal = ethSwt->DrvApi->ResetConfiguration(ethSwt->SwitchIdx);
    }
#  if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
#  endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_RESET_CONFIGURATION, errorId);
  }
#  else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#  endif

  return retVal;
} /* EthIf_ResetConfiguration() */
# endif /* ETHIF_RESET_CONFIG_API */

# if ( ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT == STD_ON )
/**********************************************************************************************************************
 * EthIf_SetSwitchMgmtInfo
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetSwitchMgmtInfo(
          uint8                                         EthIfCtrlIdx,
          uint8                                         BufIdx,
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_CONST)  MgmtInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( EthIfCtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx()  )
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( BufIdx >= EthIf_CfgAccess_MaxTxBufNum() )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else if( MgmtInfo == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
#  endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST)  virtCtrl;
    P2VAR(EthIf_TxContextType, AUTOMATIC, AUTOMATIC)     txContext;

    virtCtrl = &EthIf_CfgAccess_VirtCtrl(EthIfCtrlIdx);
    txContext = EthIf_CfgAccess_GetTxContextByContextTuple(virtCtrl->EthCtrl->CtrlIdx, BufIdx);

    /* #30 Check if tx buffer was acquired during EthIf_ProvideTxBuffer() and EthIf_Transmit() wasn't called yet */
    if ( (txContext->IsValid == TRUE) &&
         (txContext->EthIfCtrlIdx == virtCtrl->EthIfCtrlIdx) )
    {
      /* #40 Check if driver provides the API */
      if( virtCtrl->EthSwtFrameMngmt != NULL_PTR )
      {
        if( virtCtrl->EthSwtFrameMngmt->DrvApi->SetSwitchMgmtInfo != NULL_PTR )
        {
          /* #400 Redirect call to the driver */
          retVal = virtCtrl->EthSwtFrameMngmt->DrvApi->SetSwitchMgmtInfo(virtCtrl->EthCtrl->CtrlIdx,
                                                                         BufIdx,
                                                                         MgmtInfo);
        }
#  if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
        else
        {
          errorId = ETHIF_E_INV_DRIVER_API_CALL;
        }
#  endif /* ETHIF_DEV_ERROR_DETECT */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SET_SWITCH_MGMT_INFO, errorId);
  }
#  else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#  endif

  return retVal;
} /* EthIf_SetSwitchMgmtInfo() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  EthIf_SwitchMgmtInfoIndication
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_SwitchMgmtInfoIndication(
          uint8                                            CtrlIdx,
    P2VAR(uint8,               AUTOMATIC, ETHIF_APPL_VAR)  DataPtr,
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_CONST)     MgmtInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx >= EthIf_CfgAccess_EthCtrlNum()  )
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( DataPtr == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( EthIf_CfgAccess_EthCtrlSummary(CtrlIdx)->IsHandlingSwtMgmtFrames == FALSE )
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else
#  endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 ethIfCtrlIdx;
    uint8 ownerIdx;

    /* #30 Check if there was information to redirect the call stored */
    if( EthIf_RestoreSwtMgmtRxInfo(CtrlIdx,
                                   MgmtInfo->FrameId,
                                   &ethIfCtrlIdx,
                                   &ownerIdx,
                                   &DataPtr) == TRUE )
    {
      uint8_least indicationIdx;

      /* #300 Inform owner about frame related management information from switch if has registered for the indication */
      for( indicationIdx = 0; indicationIdx < EthIf_CfgAccess_OwnerMgmtInfoIndInfo(ownerIdx).MgmtInfoIndicationNum; indicationIdx++ )
      {
        EthIf_CfgAccess_OwnerMgmtInfoIndInfo(ownerIdx).MgmtInfoIndications[indicationIdx](ethIfCtrlIdx, DataPtr, MgmtInfo);
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_MGMT_INFO_INDICATION, errorId);
  }
#  else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#  endif

} /* EthIf_IngressSwitchMngmtInfoIndication() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif /* ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT */

# if ( ETHIF_ETHSWT_TIME_STAMP_SUPPORT == STD_ON )
/**********************************************************************************************************************
 * EthIf_SwitchEnableEgressTimeStamp
 **********************************************************************************************************************/
/*! Internal comment removed.
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
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchEnableEgressTimeStamp(
          uint8                                         EthIfCtrlIdx,
          uint8                                         BufIdx,
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_CONST)  MgmtInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( EthIfCtrlIdx >= EthIf_CfgAccess_EthIfCtrlNum()  )
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( BufIdx >= EthIf_CfgAccess_MaxTxBufNum() )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else if( MgmtInfo == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
#  endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST)  virtCtrl;
    P2VAR(EthIf_TxContextType, AUTOMATIC, AUTOMATIC)     txContext;

    virtCtrl = &EthIf_CfgAccess_VirtCtrl(EthIfCtrlIdx);
    txContext = EthIf_CfgAccess_GetTxContextByContextTuple(virtCtrl->EthCtrl->CtrlIdx, BufIdx);

    /* #30 Check if tx buffer was acquired during EthIf_ProvideTxBuffer() and EthIf_Transmit() wasn't called yet */
    if ( (txContext->IsValid == TRUE) &&
         (txContext->EthIfCtrlIdx == virtCtrl->EthIfCtrlIdx) )
    {
      /* #40 Check if driver provides the API */
      if( virtCtrl->EthSwtFrameMngmt != NULL_PTR )
      {
        if( virtCtrl->EthSwtFrameMngmt->DrvApi->SwitchEnableEgressTimeStamp != NULL_PTR )
        {
          P2VAR(EthIf_SwtTimeStampTxCtxtPoolElemType, AUTOMATIC, AUTOMATIC) txInfoPtr;
                EthSwt_MgmtInfoType                                         mgmtInfo;

          /* #400 Retrieve the tx context element */
          if( EthIf_GetSwtTimeStampTxInfo(virtCtrl->EthCtrl->CtrlIdx,
                                          BufIdx,
                                          MgmtInfo->FrameId,
                                          &txInfoPtr) == TRUE )
          {
            /* #410 Prepare information for driver call */
            mgmtInfo.SwitchIdx = MgmtInfo->SwitchIdx;
            mgmtInfo.PortIdx   = MgmtInfo->PortIdx;
            mgmtInfo.FrameId   = MgmtInfo->FrameId;

            /* #420 Redirect call to the driver */
            retVal = virtCtrl->EthSwtFrameMngmt->DrvApi->SwitchEnableEgressTimeStamp(virtCtrl->EthCtrl->CtrlIdx,
                                                                                     BufIdx,
                                                                                     (P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_CONST))&mgmtInfo);

            /* #430 Arm tx context element for processing in timestamp indication context */
            EthIf_ArmSwtTimeStampTxInfo(txInfoPtr);
          }
        }
      }
#  if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
      else
      {
        errorId = ETHIF_E_INV_DRIVER_API_CALL;
      }
#  endif /* ETHIF_DEV_ERROR_DETECT */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_ENABLE_EGRESS_TIME_STAMP, errorId);
  }
#  else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#  endif

  return retVal;
} /* EthIf_SwitchEnableEgressTimeStamp() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  EthIf_SwitchEgressTimeStampIndication
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_SwitchEgressTimeStampIndication(
          uint8                                            CtrlIdx,
    P2VAR(uint8,               AUTOMATIC, ETHIF_APPL_VAR)  DataPtr,
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_CONST)     MgmtInfo,
  P2CONST(Eth_TimeStampType,   AUTOMATIC, ETHIF_CONST)     timeStamp)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx >= EthIf_CfgAccess_EthCtrlNum()  )
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( MgmtInfo == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( EthIf_CfgAccess_EthCtrlSummary(CtrlIdx)->IsHandlingSwtTsFrames == FALSE )
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else
#  endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 ethIfCtrlIdx;
    uint8 frameOwnerIdx;
    boolean isIndicationRequested;

    isIndicationRequested = FALSE;

    /* #30 Check if there was information related to the time stamp stored either during rx or tx processing. */
    if( EthIf_RestoreSwtTimeStampTxInfo(CtrlIdx,
                                        ETHIF_SWT_TIMESTAMP_PROC_FLAG_EGRESS_TS_PENDING,
                                        MgmtInfo->FrameId,
                                        &ethIfCtrlIdx,
                                        &frameOwnerIdx,
                                        &DataPtr) == TRUE )
    {
      isIndicationRequested = TRUE;
    }
    else if( EthIf_RestoreSwtTimeStampRxInfo(CtrlIdx,
                                             ETHIF_SWT_TIMESTAMP_PROC_FLAG_EGRESS_TS_PENDING,
                                             MgmtInfo->FrameId,
                                             &ethIfCtrlIdx,
                                             &frameOwnerIdx,
                                             &DataPtr) == TRUE )
    {
      isIndicationRequested = TRUE;
    }
    else
    {
      /* only for MISRA compliance */
    }

    /* #40 If information was stored and could be retrieved inform the related owner about the time stamp information */
    if( isIndicationRequested == TRUE )
    {
      /* handle upper layer indication */
      uint8_least egrTsIndIdx;

      for( egrTsIndIdx = 0u; egrTsIndIdx < EthIf_CfgAccess_OwnerSwtTsIndsInfo(frameOwnerIdx).TimeStampIndicationNum; egrTsIndIdx++ )
      {
        EthIf_CfgAccess_OwnerSwtTsIndsInfo(frameOwnerIdx).TimeStampIndications[egrTsIndIdx].EgressTimeStampIndication(ethIfCtrlIdx,
                                                                                                                      DataPtr,
                                                                                                                      MgmtInfo,
                                                                                                                      timeStamp);
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_EGRESS_TIME_STAMP_INDICATION, errorId);
  }
#  else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#  endif

} /* EthIf_SwitchEgressTimeStampIndication() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  EthIf_SwitchIngressTimeStampIndication
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_SwitchIngressTimeStampIndication(
          uint8                                            CtrlIdx,
    P2VAR(uint8,               AUTOMATIC, ETHIF_APPL_VAR)  DataPtr,
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHIF_CONST)     MgmtInfo,
  P2CONST(Eth_TimeStampType,   AUTOMATIC, ETHIF_CONST)     timeStamp)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx >= EthIf_CfgAccess_EthCtrlNum()  )
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( MgmtInfo == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( EthIf_CfgAccess_EthCtrlSummary(CtrlIdx)->IsHandlingSwtTsFrames == FALSE )
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else
#  endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 ethIfCtrlIdx;
    uint8 frameOwnerIdx;
    boolean isIndicationRequested;

    isIndicationRequested = FALSE;

    /* #30 Check if there was information related to the time stamp stored either during rx or tx processing. */
    if( EthIf_RestoreSwtTimeStampTxInfo(CtrlIdx,
                                        ETHIF_SWT_TIMESTAMP_PROC_FLAG_INGRESS_TS_PENDING,
                                        MgmtInfo->FrameId,
                                        &ethIfCtrlIdx,
                                        &frameOwnerIdx,
                                        &DataPtr) == TRUE )
    {
      isIndicationRequested = TRUE;
    }
    else if( EthIf_RestoreSwtTimeStampRxInfo(CtrlIdx,
                                             ETHIF_SWT_TIMESTAMP_PROC_FLAG_INGRESS_TS_PENDING,
                                             MgmtInfo->FrameId,
                                             &ethIfCtrlIdx,
                                             &frameOwnerIdx,
                                             &DataPtr) == TRUE )
    {
      isIndicationRequested = TRUE;
    }
    else
    {
      /* only for MISRA compliance */
    }

    /* #40 If information was stored and could be retrieved inform the related owner about the time stamp information */
    if( isIndicationRequested == TRUE )
    {
      /* handle upper layer indication */
      uint8_least igrTsIndIdx;

      for( igrTsIndIdx = 0u; igrTsIndIdx < EthIf_CfgAccess_OwnerSwtTsIndsInfo(frameOwnerIdx).TimeStampIndicationNum; igrTsIndIdx++ )
      {
        EthIf_CfgAccess_OwnerSwtTsIndsInfo(frameOwnerIdx).TimeStampIndications[igrTsIndIdx].IngressTimeStampIndication(ethIfCtrlIdx,
                                                                                                                       DataPtr,
                                                                                                                       MgmtInfo,
                                                                                                                       timeStamp);
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_INGRESS_TIME_STAMP_INDICATION, errorId);
  }
#  else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#  endif

} /* EthIf_SwitchIngressTimeStampIndication() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

# endif /* ETHIF_ETHSWT_TIME_STAMP_SUPPORT */

# if ( ETHIF_SWITCH_UPDATE_MCAST_PORT_ASSIGN_API == STD_ON )
/**********************************************************************************************************************
 * EthIf_SwitchUpdateMCastPortAssignment
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchUpdateMCastPortAssignment(
          uint8                            SwitchIdx,
          uint8                            PortIdx,
  P2CONST(uint8, AUTOMATIC, ETHIF_CONST)   MCastAddr,
          EthSwt_MCastPortAssignActionType Action)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( SwitchIdx >= EthIf_CfgAccess_EthSwtNum()  )
  {
    errorId = ETHIF_E_INV_SWITCH_IDX;
  }
  else if( MCastAddr == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( (Action != ETHSWT_MCAST_PORT_ASSIGN_ACTION_ADD)    &&
           (Action != ETHSWT_MCAST_PORT_ASSIGN_ACTION_REMOVE)    )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else
#  endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_EthSwtType, AUTOMATIC, ETHIF_CONST) ethSwt;

    ethSwt = EthIf_CfgAccess_EthSwtSummary(SwitchIdx);

    /* #30 Check if driver provides the API */
    if( ethSwt->DrvApi->UpdateMCastPortAssignment != NULL_PTR )
    {
      /* #300 Redirect call to the driver */
      retVal = ethSwt->DrvApi->UpdateMCastPortAssignment(ethSwt->SwitchIdx, PortIdx, MCastAddr, Action);
    }
#  if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
#  endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_UPDATE_MCAST_PORT_ASSIGNMENT, errorId);
  }
#  else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#  endif

  return retVal;
} /* EthIf_SwitchUpdateMCastPortAssignment() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

# endif /* ETHIF_SWITCH_UPDATE_MCAST_PORT_ASSIGN_API */

#endif /* ETHIF_ETHSWT_USED */

#define ETHIF_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_Switch.c
 *********************************************************************************************************************/

