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
/**        \file  EthIf_ExtndTrafficHndl.c
 *        \brief  Ethernet Interface extended traffic handling extension source file
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

#define ETHIF_EXTND_TRAFFIC_HNDL_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthIf_ExtndTrafficHndl_Int.h"
#include "EthIf.h"
#include "EthIf_Int.h"
#include "IpBase.h"
#if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"
#endif /* ETHIF_DEV_ERROR_REPORT */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of EthIf header file */
/* \trace SPEC-2393557 */
#if (  (ETHIF_SW_MAJOR_VERSION != (8u)) \
    || (ETHIF_SW_MINOR_VERSION != (0u)) \
    || (ETHIF_SW_PATCH_VERSION != (1u)) )
# error "Vendor specific version numbers of EthIf_ExtndTrafficHndl.c and EthIf.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define ETHIF_MAC_ADDR_SIZE         (6U)
#define ETHIF_ETHERNET_HEADER_SIZE  (14U)

#if   ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE != ETHIF_NO_EXTENDED_TRAFFIC_HANDLING )

# define EthIf_GetDestMacAddrOffset() (-ETHIF_ETHERNET_HEADER_SIZE)
# define EthIf_GetSrcMacAddrOffset()  (-ETHIF_ETHERNET_HEADER_SIZE + ETHIF_MAC_ADDR_SIZE)

#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */

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
#if ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_GATEWAY )

ETHIF_LOCAL CONST(uint8, ETH_CONST) EthIf_Gateway_Broadcast[ETHIF_MAC_ADDR_SIZE] = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFF, 0xFFU};

#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */

#define ETHIF_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#if ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_MIRRORING )

/***********************************************************************************************************************
 *  EthIf_Mirroring_GetFreeTxTrafficInfo
 **********************************************************************************************************************/
/*! \brief      Retrieves a free Tx traffic info element
 *  \details    This function retrieves a Tx traffic info element used to store context between buffer request and
 *              transmit of a frame to be mirrored.
 *  \param[in]  txTrafficInfoPtr    Pointer to pass the retrieved element
 *  \return     E_OK - free element found
 *  \return     E_NOT_OK - no free element available
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(Std_ReturnType, ETHIF_CODE) EthIf_Mirroring_GetFreeTxTrafficInfo(
    P2VAR(EthIf_Mirroring_TxTrafficInfoType*, AUTOMATIC, ETHIF_APPL_VAR) txTrafficInfoPtr);

/***********************************************************************************************************************
 *  EthIf_Mirroring_ReleaseTxTrafficInfo
 **********************************************************************************************************************/
/*! \brief      Releases Tx traffic info element
 *  \details    This function releases a previously allocated Tx traffic info element.
 *  \param[in]  txTrafficInfoPtr    Tx Traffic info element to be released
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_Mirroring_ReleaseTxTrafficInfo(
    P2VAR(EthIf_Mirroring_TxTrafficInfoType, AUTOMATIC, ETHIF_APPL_VAR) txTrafficInfoPtr);

/***********************************************************************************************************************
 *  EthIf_Mirroring_GetRelatedTxTrafficInfo
 **********************************************************************************************************************/
/*! \brief      Retrieves a allocated Tx traffic info element
 *  \details    This function retrieves a allocated Tx traffic info element to get access to the information of
 *              the buffer request context in transmit context.
 *  \param[in]  ethIfCtrlIdx      EthIf controller index
 *  \param[in]  ethTxBufIdx       Tx buffer index
 *  \param[in]  txTrafficInfoPtr  Pointer to pass the retrieved element
 *  \return     E_OK - a Tx traffic info element was retrieved
 *  \return     E_NOT_OK - no Tx traffic info element available for the given transmit procedure context
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(Std_ReturnType, ETHIF_CODE) EthIf_Mirroring_GetRelatedTxTrafficInfo(
          uint8                                                           ethIfCtrlIdx,
          uint8                                                           ethTxBufIdx,
    P2VAR(EthIf_Mirroring_TxTrafficInfoType*, AUTOMATIC, ETHIF_APPL_VAR)  txTrafficInfoPtr);
#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */

#if ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_GATEWAY )

# if ( ETHIF_GATEWAY_BLACKLIST_SIZE > 0 )
/***********************************************************************************************************************
 *  EthIf_Gateway_IsMacOnBlackList
 **********************************************************************************************************************/
/*! \brief      Checks if the MAC address is on the gateway black list
 *  \details    This function checks if the passed MAC address is on the gateway black list.
 *  \param[in]  srcMacAddrPtr      EthIf controller index
 *  \return     TRUE - MAC address is on the gateway black list
 *  \return     FALSE - MAC address isn't on the gateway black list
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_Gateway_IsMacOnBlackList(
    P2VAR(uint8, AUTOMATIC, ETHIF_APPL_VAR) srcMacAddrPtr);
# endif /* ETHIF_GATEWAY_BLACKLIST_SIZE */

#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_MIRRORING )

/***********************************************************************************************************************
 *  EthIf_Mirroring_GetFreeTxTrafficInfo
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(Std_ReturnType, ETHIF_CODE) EthIf_Mirroring_GetFreeTxTrafficInfo(
    P2VAR(EthIf_Mirroring_TxTrafficInfoType*, AUTOMATIC, ETHIF_APPL_VAR) txTrafficInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8_least    idx;

  /* ----- Implementation ----------------------------------------------- */
  retVal              = E_NOT_OK;
  (*txTrafficInfoPtr) = NULL_PTR;

  /* #10 Enter exclusive area (Reason: Ensure that a Tx traffic info element can only be allocated once) */
  EthIf_EnterExclusiveArea(TX_MIRROR_ELEMENT); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 For each Tx traffic info element */
  for( idx = 0; idx < ETHIF_TRAFFIC_MIRRORING_MAX_PARALLEL_TX; idx++ )
  {
    /* #200 Check if element is free */
    if( EthIf_Mirroring_TxInfoObjects[idx].IsUsed == FALSE )
    {
      /* #2000 Allocate free element and pass it to caller */
      EthIf_Mirroring_TxInfoObjects[idx].IsUsed = TRUE;
      (*txTrafficInfoPtr) = &EthIf_Mirroring_TxInfoObjects[idx];
      retVal = E_OK;
      break;
    }
  }

  /* #30 Leave exclusive area */
  EthIf_ExitExclusiveArea(TX_MIRROR_ELEMENT); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  return retVal;
} /* EthIf_Mirroring_GetFreeTxTrafficInfo() */

/***********************************************************************************************************************
 *  EthIf_Mirroring_ReleaseTxTrafficInfo
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_Mirroring_ReleaseTxTrafficInfo(
    P2VAR(EthIf_Mirroring_TxTrafficInfoType, AUTOMATIC, ETHIF_APPL_VAR) txTrafficInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Release Tx traffic info element if it is in use */
  if( txTrafficInfoPtr->IsUsed == TRUE )
  {
    txTrafficInfoPtr->IsInfoValid = FALSE;
    txTrafficInfoPtr->IsUsed      = FALSE;
  }
} /* EthIf_Mirroring_ReleaseTxTrafficInfo() */

/***********************************************************************************************************************
 *  EthIf_Mirroring_GetRelatedTxTrafficInfo
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(Std_ReturnType, ETHIF_CODE) EthIf_Mirroring_GetRelatedTxTrafficInfo(
          uint8                                                           ethIfCtrlIdx,
          uint8                                                           ethTxBufIdx,
    P2VAR(EthIf_Mirroring_TxTrafficInfoType*, AUTOMATIC, ETHIF_APPL_VAR)  txTrafficInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8_least    idx;

  /* ----- Implementation ----------------------------------------------- */
  retVal              = E_NOT_OK;
  (*txTrafficInfoPtr) = NULL_PTR;

  /* #10 For each Tx traffic info element */
  for( idx = 0; idx < ETHIF_TRAFFIC_MIRRORING_MAX_PARALLEL_TX; idx++ )
  {
    /* #100 Check if the element is related to the context queried for */
    if( (EthIf_Mirroring_TxInfoObjects[idx].IsUsed            == TRUE)         &&
        (EthIf_Mirroring_TxInfoObjects[idx].IsInfoValid       == TRUE)         &&
        (EthIf_Mirroring_TxInfoObjects[idx].EthIfCtrlIdx      == ethIfCtrlIdx) &&
        (EthIf_Mirroring_TxInfoObjects[idx].SrcEthTxBufferIdx == ethTxBufIdx)     )
    {
      /* #1000 Pass related element to the caller */
      (*txTrafficInfoPtr) = &EthIf_Mirroring_TxInfoObjects[idx];
      retVal = E_OK;
      break;
    }
  }

  return retVal;
} /* EthIf_Mirroring_GetRelatedTxTrafficInfo() */
#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */

#if ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_GATEWAY )

# if ( ETHIF_GATEWAY_BLACKLIST_SIZE > 0 )
/***********************************************************************************************************************
 *  EthIf_Gateway_IsMacOnBlackList
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_Gateway_IsMacOnBlackList(
    P2VAR(uint8, AUTOMATIC, ETHIF_APPL_VAR) srcMacAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean      result;
  uint8_least  idx;

  /* ----- Implementation ----------------------------------------------- */
  result = FALSE;

  /* #10 For each entry of the black list */
  for( idx = 0; idx < ETHIF_GATEWAY_BLACKLIST_SIZE; idx++ )
  {
      uint8_least macAddrPartIdx;
      boolean     isEntryFound;

      isEntryFound = TRUE;

      /* #100 Check if MAC address is on black list */
      for( macAddrPartIdx = 0; macAddrPartIdx < ETHIF_MAC_ADDR_SIZE; macAddrPartIdx++ )
      {
          if( srcMacAddrPtr[macAddrPartIdx] != EthIf_Gateway_SrcMacBlackList[idx][macAddrPartIdx] )
          {
              isEntryFound = FALSE;
              break;
          }
      }

      if( isEntryFound == TRUE )
      {
        /* #1000 Indicate that MAC address is on black list */
        result = TRUE;
        break;
      }
  }

  return result;
} /* EthIf_Gateway_IsMacOnBlackList() */
# endif /* ETHIF_GATEWAY_BLACKLIST_SIZE */

#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#if ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_MIRRORING )

/***********************************************************************************************************************
 *  EthIf_Mirroring_ProvideTxMirroringBuffer
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
FUNC(void, ETHIF_CODE) EthIf_Mirroring_ProvideTxMirroringBuffer(
          uint8                                     srcEthIfCtrlIdx,
    P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_VAR)  srcDataPtr,
          uint8                                     srcDataBufIdx,
          uint16                                    srcFrameLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl;

  /* ----- Local data definition ---------------------------------------- */
  virtCtrl = &EthIf_CfgAccess_VirtCtrl(srcEthIfCtrlIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Tx traffic must be mirrored */
  if( virtCtrl->TxTrafficDest != NULL_PTR )
  {
    P2VAR(EthIf_Mirroring_TxTrafficInfoType, AUTOMATIC, ETHIF_APPL_VAR) txTrafficInfo;

    /* #100 Check if a Tx traffic mirror info element for buffer request context storage is available */
    if( EthIf_Mirroring_GetFreeTxTrafficInfo(&txTrafficInfo) == E_OK )
    {
      P2CONST(EthIf_Mirroring_DestinationType, AUTOMATIC, ETHIF_CONST) txTrafficDest;

      txTrafficDest = virtCtrl->TxTrafficDest;

      /* #1000 Check if buffer allocation on mirroring destination is successful */
      if( txTrafficDest->DestEthCtrl->DrvApi->ProvideTxBuffer(txTrafficDest->DestEthCtrl->CtrlIdx,
                                                              &txTrafficInfo->DstEthTxBufferIdx,
                                                              &txTrafficInfo->DstPayloadPtr,
                                                              &srcFrameLen) == BUFREQ_OK )
      {
        /* #10000 Store buffer request context in Tx traffic info element to be able to retrieve it in transmit context */
        txTrafficInfo->SrcPayloadPtr     = srcDataPtr;
        txTrafficInfo->EthIfCtrlIdx      = srcEthIfCtrlIdx;
        txTrafficInfo->SrcEthTxBufferIdx = srcDataBufIdx;
        txTrafficInfo->IsInfoValid       = TRUE;
      }
      /* #1010 Otherwise (no buffer for mirroring could be allocated) */
      else
      {
        /* #10100 Release the allocated Tx traffic mirror element */
        EthIf_Mirroring_ReleaseTxTrafficInfo(txTrafficInfo);
      }
    }
  }
} /* EthIf_Mirroring_ProvideTxMirroringBuffer() */

/***********************************************************************************************************************
 *  EthIf_Mirroring_MirrorTxTraffic
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
FUNC(void, ETHIF_CODE) EthIf_Mirroring_MirrorTxTraffic(
          uint8                              srcEthIfCtrlIdx,
          uint8                              srcBufIdx,
          uint16                             srcFrameType,
          uint16                             srcFrameLen,
    P2VAR(uint8, AUTOMATIC, ETHIF_APPL_VAR)  destMacAddr)
{
  /* ----- Local data declaration --------------------------------------- */
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl;

  /* ----- Local data definition ---------------------------------------- */
  virtCtrl = &EthIf_CfgAccess_VirtCtrl(srcEthIfCtrlIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Tx traffic must be mirrored */
  if( virtCtrl->TxTrafficDest != NULL_PTR )
  {
    P2VAR(EthIf_Mirroring_TxTrafficInfoType, AUTOMATIC, ETHIF_APPL_VAR) txTrafficInfo;

    /* #100 Check if a Tx traffic mirroring element of the related buffer request conexts can be retrieved */
    if( EthIf_Mirroring_GetRelatedTxTrafficInfo(srcEthIfCtrlIdx, srcBufIdx, &txTrafficInfo) == E_OK )
    {
      P2CONST(EthIf_Mirroring_DestinationType, AUTOMATIC, ETHIF_CONST) txTrafficDest;

      txTrafficDest = virtCtrl->TxTrafficDest;

      /* #1000 Check if a transmit is triggered */
      if( srcFrameLen != 0u )
      {
        /* #10000 Copy frame to mirroring destination buffer */
        IpBase_Copy((IPBASE_P2VARAPPLDATA(IpBase_CopyDataType))txTrafficInfo->DstPayloadPtr,
                    (IPBASE_P2CONSTAPPLDATA(IpBase_CopyDataType))txTrafficInfo->SrcPayloadPtr,
                    srcFrameLen);

        /* #10010 Check if source MAC address of original frame shall be preserved */
        if( txTrafficDest->KeepSrcMac == TRUE )
        {
          /* #100100 Trigger transmission with original source MAC address */
          (void)txTrafficDest->DestEthCtrl->DrvApi->VTransmit(txTrafficDest->DestEthCtrl->CtrlIdx,
                                                              txTrafficInfo->DstEthTxBufferIdx,
                                                              srcFrameType,
                                                              FALSE,
                                                              srcFrameLen,
                                                              destMacAddr,
                                                              &((P2VAR(uint8, AUTOMATIC, ETHIF_APPL_VAR))txTrafficInfo->SrcPayloadPtr)[EthIf_GetSrcMacAddrOffset()]);
        }
        /* #10020 Otherwise (source MAC address shall not be preserved) */
        else
        {
          /* #100200 Trigger transmission with the MAC address of the mirroring destination Ethernet controller */
          (void)txTrafficDest->DestEthCtrl->DrvApi->Transmit(txTrafficDest->DestEthCtrl->CtrlIdx,
                                                             txTrafficInfo->DstEthTxBufferIdx,
                                                             srcFrameType,
                                                             FALSE,
                                                             srcFrameLen,
                                                             destMacAddr);
        }
      }

      /* #1010 Release Tx traffic mirror info element */
      EthIf_Mirroring_ReleaseTxTrafficInfo(txTrafficInfo);
    }
  }
} /* EthIf_Mirroring_MirrorTxTraffic() */

/***********************************************************************************************************************
 *  EthIf_Mirroring_MirrorRxTraffic
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
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_Mirroring_MirrorRxTraffic(
          uint8                                     srcEthIfCtrlIdx,
          uint16                                    srcFrameType,
          uint16                                    srcFrameLen,
    P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_VAR)  srcDataPtr)
{
  /* ----- Local data declaration --------------------------------------- */
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl;

  /* ----- Local data definition ---------------------------------------- */
  virtCtrl = &EthIf_CfgAccess_VirtCtrl(srcEthIfCtrlIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Rx traffic must be mirrored */
  if( virtCtrl->RxTrafficDest != NULL_PTR )
  {
    P2CONST(EthIf_Mirroring_DestinationType, AUTOMATIC, ETHIF_CONST) rxTrafficDest;
    P2VAR(  Eth_DataType, AUTOMATIC, ETHIF_APPL_VAR)                 dstBufPtr;
            uint8                                                    dstBufIdx;
            uint16                                                   lenByte;

    rxTrafficDest = virtCtrl->RxTrafficDest;
    lenByte       = srcFrameLen;

    /* #100 Check if buffer allocation on mirroring destination is successful */
    if( rxTrafficDest->DestEthCtrl->DrvApi->ProvideTxBuffer(rxTrafficDest->DestEthCtrl->CtrlIdx,
                                                            &dstBufIdx,
                                                            &dstBufPtr,
                                                            &lenByte) == BUFREQ_OK)
    {
      /* #1000 Copy frame to mirroring destination buffer */
      IpBase_Copy((IPBASE_P2VARAPPLDATA(IpBase_CopyDataType))dstBufPtr,
                  (IPBASE_P2CONSTAPPLDATA(IpBase_CopyDataType))srcDataPtr,
                  srcFrameLen);

      /* #1010 Check if source MAC address of original frame shall be preserved */
      if( rxTrafficDest->KeepSrcMac == TRUE )
      {
        /* #10100 Trigger transmission with original source MAC address */
        (void)rxTrafficDest->DestEthCtrl->DrvApi->VTransmit(rxTrafficDest->DestEthCtrl->CtrlIdx,
                                                            dstBufIdx,
                                                            srcFrameType,
                                                            FALSE,
                                                            srcFrameLen,
                                                            &((P2VAR(uint8, AUTOMATIC, ETHIF_APPL_VAR))srcDataPtr)[EthIf_GetDestMacAddrOffset()],
                                                            &((P2VAR(uint8, AUTOMATIC, ETHIF_APPL_VAR))srcDataPtr)[EthIf_GetSrcMacAddrOffset()]);
      }
      /* #1020 Otherwise */
      else
      {
        /* #10200 Trigger transmission with the MAC address of the mirroring destination Ethernet controller */
        (void)rxTrafficDest->DestEthCtrl->DrvApi->Transmit(rxTrafficDest->DestEthCtrl->CtrlIdx,
                                                           dstBufIdx,
                                                           srcFrameType,
                                                           FALSE,
                                                           srcFrameLen,
                                                           &((P2VAR(uint8, AUTOMATIC, ETHIF_APPL_VAR))srcDataPtr)[EthIf_GetDestMacAddrOffset()]);
      }
    }
  }
} /* EthIf_Mirroring_MirrorRxTraffic() */
#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */

#if ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_GATEWAY )

/***********************************************************************************************************************
 *  EthIf_Gateway_ManipulatePromiscousMode
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_Gateway_ManipulatePromiscousMode(
    uint8                 ethIfCtrlIdx,
    Eth_FilterActionType  filterAction)
{
  /* ----- Local data declaration --------------------------------------- */
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST)  virtCtrl;

  /* ----- Local data definition ---------------------------------------- */
  virtCtrl = &EthIf_CfgAccess_VirtCtrl(EthIfCtrlIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if EthIf controller is involved in traffic gateway */
  if( virtCtrl->GatewayDestEthIfCtrlIdx != ETHIF_INVALID_ETHIF_CONTROLLER_IDX )
  {
    Std_ReturnType  retVal;

    /* #100 Apply promiscous mode change by calling the driver */
    retVal = EthIf_UpdatePhysAddrFilter(ethIfCtrlIdx,
                                        EthIf_Gateway_Broadcast,
                                        filterAction);

#if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
    if( retVal == E_NOT_OK )
    {
      /* Promiscous mode couldn't be enabled -> traffic gateway will not work */
      (void)Det_ReportError(ETHIF_MODULE_ID,
                            ETHIF_INSTANCE_ID_DET,
                            ETHIF_SID_GATEWAY_MANIPULATE_PROMISCOUS_MODE,
                            ETHIF_TRAFFIC_GATEWAY_E_MANIPULATE_PROMISCOUS_MODE);
    }
#else
    ETHIF_DUMMY_STATEMENT(retVal); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* ETHIF_DEV_ERROR_DETECT */
  }
} /* EthIf_Gateway_ManipulatePromiscousMode() */

/***********************************************************************************************************************
 *  EthIf_Gateway_GatewayTraffic
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
FUNC(boolean, ETHIF_CODE) EthIf_Gateway_GatewayTraffic(
          uint8                                     srcEthIfCtrlIdx,
          uint16                                    srcFrameType,
          uint16                                    srcFrameLen,
    P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_VAR)  srcDataPtr)
{
  /* ----- Local data declaration --------------------------------------- */
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl;
          boolean                                     isTrafficGateway;
          uint8                                       additionalHdrLen;

  /* ----- Local data definition ---------------------------------------- */
  virtCtrl         = &EthIf_CfgAccess_VirtCtrl(srcEthIfCtrlIdx);
  isTrafficGateway = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if EthIf controller is involved in a traffic gateway route */
  if( virtCtrl->GatewayDestEthIfCtrlIdx != ETHIF_INVALID_ETHIF_CONTROLLER_IDX )
  {
      additionalHdrLen = 0;

    /* #100 Take VLAN tag into account for buffer length required if needed */
    if( virtCtrl->Type == ETHIF_Q_TAG_TAGGED )
    {
        additionalHdrLen += ETHIF_VLAN_TAG_LEN_BYTE;
    }

#if ( ETHIF_GATEWAY_BLACKLIST_SIZE > 0 )
    /* #110 Check if MAC address is on the gateway blacklist */
    if( EthIf_Gateway_IsMacOnBlackList(&((P2VAR(uint8, AUTOMATIC, ETHIF_APPL_VAR))srcDataPtr)[EthIf_GetSrcMacAddrOffset()]) == FALSE )
#endif /* ETHIF_GATEWAY_BLACKLIST_SIZE */
    {
      P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_VAR) destBufPtr;
            uint8                                    destBufIdx;
            uint16                                   destLenOrg;
            uint16                                   destLenBuf;

      isTrafficGateway  = TRUE;
      destLenOrg        = srcFrameLen;
      /* Adapt frame length dependent on outcome of VLAN tag check */
      destLenOrg       -= additionalHdrLen;
      destLenBuf        = destLenOrg;

      /* #1000 Check if buffer allocation on gateway route destination is successful */
      if( EthIf_ProvideTxBuffer(virtCtrl->GatewayDestEthIfCtrlIdx,
                                srcFrameType,
                                0u,
                                &destBufIdx,
                                &destBufPtr,
                                &destLenBuf) == BUFREQ_OK )
      {
        /* #10000 Copy frame to be routed */
        IpBase_Copy((IPBASE_P2VARAPPLDATA(IpBase_CopyDataType))destBufPtr,
                    (IPBASE_P2CONSTAPPLDATA(IpBase_CopyDataType))&((P2VAR(uint8, AUTOMATIC, ETHIF_APPL_VAR))srcDataPtr)[additionalHdrLen],
                    destLenOrg);

        /* #10010 Trigger transmission */
        (void)EthIf_VTransmit(virtCtrl->GatewayDestEthIfCtrlIdx,
                              destBufIdx,
                              srcFrameType,
                              FALSE,
                              destLenOrg,
                              &((P2VAR(uint8, AUTOMATIC, ETHIF_APPL_VAR))srcDataPtr)[EthIf_GetDestMacAddrOffset()],
                              &((P2VAR(uint8, AUTOMATIC, ETHIF_APPL_VAR))srcDataPtr)[EthIf_GetSrcMacAddrOffset()]);
      }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
      else
      {
        /* No buffer provided -> traffic can't be routed */
        (void)Det_ReportError(ETHIF_MODULE_ID,
                              ETHIF_INSTANCE_ID_DET,
                              ETHIF_SID_GATEWAY_GATEWAY_TRAFFIC,
                              ETHIF_TRAFFIC_GATEWAY_E_NO_BUFFER);
      }
# endif /* ETHIF_DEV_ERROR_DETECT */
    }
  }

  return isTrafficGateway;
} /* EthIf_Gateway_GatewayTraffic() */

#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */

#define ETHIF_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_ExtndTrafficHndl.c
 *********************************************************************************************************************/

