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
/**        \file  EthIf_ZeroCopy.c
 *        \brief  Ethernet Interface Zero Copy extensions source file
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

#define ETHIF_ZERO_COPY_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthIf_ZeroCopy.h"
#include "EthIf.h"
#include "EthIf_CfgAccess_Int.h"
#include "EthIf_Int.h"
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
# error "Vendor specific version numbers of EthIf_ZeroCopy.c and EthIf.h are inconsistent"
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

#if ( ETHIF_ENABLE_ZERO_COPY_EXTENSIONS == STD_ON )
/***********************************************************************************************************************
 *  EthIf_ProvideExtTxBuffer
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
 *
 **********************************************************************************************************************/
FUNC(BufReq_ReturnType, ETHIF_CODE) EthIf_ProvideExtTxBuffer(
        uint8                                       CtrlIdx,
        Eth_FrameType                               FrameType,
        uint8                                       Priority,
  P2VAR(uint8,         AUTOMATIC, ETHIF_APPL_DATA)  BufIdxPtr,
  P2VAR(Eth_DataType*, AUTOMATIC, ETHIF_APPL_DATA)  BufPtr,
  P2VAR(uint16,        AUTOMATIC, ETHIF_APPL_DATA)  LenBytePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType  retVal  = BUFREQ_E_NOT_OK;
  uint8              errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( EthIf_QueryForFrameOwner(FrameType) > EthIf_CfgAccess_MaxValidFrameOwnerIdx() )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else if( Priority > ETHIF_VLAN_PCP_MAX_VAL )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else if( BufIdxPtr == NULL_PTR)
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( BufPtr == NULL_PTR)
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( LenBytePtr == NULL_PTR)
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check if the EthIf controller is active */
    if( EthIf_CfgAccess_EthIfCtrlMode(CtrlIdx) == ETH_MODE_ACTIVE )
    {
      P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST)  virtCtrl;

      virtCtrl = &EthIf_CfgAccess_VirtCtrl(CtrlIdx);

      /* #300 Check if driver provides the API */
      if( virtCtrl->EthCtrl->DrvApi->ProvideExtTxBuffer != NULL_PTR)
      {
        /* #3000 Redirect call to the driver */
        retVal = virtCtrl->EthCtrl->DrvApi->ProvideExtTxBuffer(virtCtrl->EthCtrl->CtrlIdx,
                                                               BufIdxPtr,
                                                               BufPtr,
                                                               LenBytePtr);

        /* #3010 Check if buffer was successfully provided by the driver */
        if( retVal == BUFREQ_OK )
        {
          P2VAR(EthIf_TxContextType, AUTOMATIC, AUTOMATIC) txContext;
          /* #30100 Store Tx context for later use in EthIf_Transmit() and EthIf_TxConfirmation() */
          txContext = EthIf_CfgAccess_GetTxContextByContextTuple(virtCtrl->EthCtrl->CtrlIdx, (*BufIdxPtr));
          txContext->EthIfCtrlIdx       = CtrlIdx;
          txContext->FrameHdr.EtherType = FrameType;
          txContext->FrameHdr.Priority  = Priority;
          txContext->FrameHdr.VlanId    = virtCtrl->VID;
          txContext->IsTxPending        = FALSE;
          txContext->IsValid            = TRUE;

          /* #30110 Check if the EthIf controller represents a tagged data path */
          if( virtCtrl->Type == ETHIF_Q_TAG_TAGGED )
          { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            /* #301100 Prepare VLAN tag */
            /* PRQA S 0310, 3305 3 */ /* MD_EthIf_310_Buffer, MD_EthIf_3305_Buffer */
            (*BufPtr) = (P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA))EthIf_PrepareVlanTag((P2CONST(EthIf_FrameHdrType, AUTOMATIC, AUTOMATIC))&txContext->FrameHdr,
                                                                                              (P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA))*BufPtr,
                                                                                              LenBytePtr);
          }
          
          txContext->PayloadPtr = (P2VAR(uint8, AUTOMATIC, AUTOMATIC))(*BufPtr);  /* PRQA S 0310,3305 */ /* MD_EthIf_310_Buffer, MD_EthIf_3305_Buffer */
          txContext->PayloadLen = (*LenBytePtr);
        }
      }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
      else
      {
        errorId =  ETHIF_E_INV_DRIVER_API_CALL;
      }
# endif /* ETHIF_DEV_ERROR_DETECT */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_PROVIDE_EXT_TX_BUFFER, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;

} /* EthIf_ProvideExtTxBuffer() */ /* PRQA S 6030, 6060, 6080 */ /* MD_MSR_STCYC, MD_MS_STPAR, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  EthIf_ReleaseRxBuffer
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_ReleaseRxBuffer(
        uint8                                      CtrlIdx,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA)  BufPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_NOT_OK;
  uint8           errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( BufPtr == NULL_PTR)
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl;

    virtCtrl = &EthIf_CfgAccess_VirtCtrl(CtrlIdx);

    /* #300 Check if driver provides the API */
    if( virtCtrl->EthCtrl->DrvApi->ReleaseRxBuffer != NULL_PTR )
    {
      P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) tagAwareBufPtr;

      tagAwareBufPtr = (P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA))BufPtr; /* PRQA S 0310 */ /* MD_EthIf_310_Buffer */

      /* #3000 Check the EthIf controller type (non-tagged or tagged) and adapt the buffer pointer
       *       to the location the driver expects */
      if( virtCtrl->Type == ETHIF_Q_TAG_TAGGED )
      {
        tagAwareBufPtr = &tagAwareBufPtr[-((sint8)ETHIF_VLAN_TAG_LEN_BYTE)]; /* PRQA S 3382 */ /* MD_EthIf_3382 */
      }

      /* #3010 Redirect call to the driver */
      /* PRQA S 0310, 3305 2 */ /* MD_EthIf_310_Buffer, MD_EthIf_3305_Buffer */
      retVal = virtCtrl->EthCtrl->DrvApi->ReleaseRxBuffer(virtCtrl->EthCtrl->CtrlIdx,
                                                          (P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA))tagAwareBufPtr);
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_RELEASE_RX_BUFFER, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* EthIf_ReleaseRxBuffer() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* ETHIF_ENABLE_ZERO_COPY_EXTENSIONS */

#if ( ETHIF_ENABLE_HEADER_ACCESS_API == STD_ON )
/***********************************************************************************************************************
 *  EthIf_GetTxHeaderPtr
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetTxHeaderPtr(
        uint8                                       CtrlIdx,
        uint8                                       BufIdx,
  P2VAR(Eth_DataType*, AUTOMATIC, ETHIF_APPL_DATA)  BufPtr,
  P2VAR(uint16,        AUTOMATIC, ETHIF_APPL_DATA)  LenBytePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal   = E_NOT_OK;
  uint8           errorId  = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( BufIdx >= EthIf_CfgAccess_MaxTxBufNum() )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else if( BufPtr == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( LenBytePtr == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl;

    virtCtrl = &EthIf_CfgAccess_VirtCtrl(CtrlIdx);

    /* #30 Check if driver provides the API */
    if( virtCtrl->EthCtrl->DrvApi->GetTxHeaderPtr != NULL_PTR )
    {
      /* #300 Redirect call to driver */
      retVal = virtCtrl->EthCtrl->DrvApi->GetTxHeaderPtr(virtCtrl->EthCtrl->CtrlIdx,
                                                         BufIdx,
                                                         BufPtr,
                                                         LenBytePtr);

      /* #310 Check the EthIf controller type (non-tagged or tagged) and consider the
       *      tag length in the resulting buffer length if necessary */
      if( virtCtrl->Type == ETHIF_Q_TAG_TAGGED )
      {
        (*LenBytePtr) += ETHIF_VLAN_TAG_LEN_BYTE;
      }
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId =  ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_TX_HEADER_PTR, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* EthIf_GetTxHeaderPtr() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  EthIf_GetRxHeaderPtr
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetRxHeaderPtr(
        uint8                                       CtrlIdx,
  P2VAR(Eth_DataType*, AUTOMATIC, ETHIF_APPL_DATA)  BufPtr,
  P2VAR(uint16,        AUTOMATIC, ETHIF_APPL_DATA)  LenBytePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal   = E_NOT_OK;
  uint8           errorId  = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( BufPtr == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( LenBytePtr == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl;

    virtCtrl = &EthIf_CfgAccess_VirtCtrl(CtrlIdx);

    /* #30 Check if driver provides the API */
    if( virtCtrl->EthCtrl->DrvApi->GetRxHeaderPtr != NULL_PTR )
    {
      P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) tagAwareBufPtr;

      tagAwareBufPtr = (P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA))*BufPtr; /* PRQA S 0310 */ /* MD_EthIf_310_Buffer */

      /* #300 Check the EthIf controller type (non-tagged or tagged) and adapt the buffer pointer
       *       to the location the driver expects */
      if( virtCtrl->Type == ETHIF_Q_TAG_TAGGED )
      {
        tagAwareBufPtr = &tagAwareBufPtr[-((sint8)ETHIF_VLAN_TAG_LEN_BYTE)]; /* PRQA S 3382 */ /* MD_EthIf_3382 */
      }

      /* #310 Redirect call to the driver */
      /* PRQA S 0310, 3305 3 */ /* MD_EthIf_310_Buffer, MD_EthIf_3305_Buffer */
      retVal = virtCtrl->EthCtrl->DrvApi->GetRxHeaderPtr(virtCtrl->EthCtrl->CtrlIdx,
                                                         (P2VAR(Eth_DataType*, AUTOMATIC, ETHIF_APPL_VAR))&tagAwareBufPtr,
                                                         LenBytePtr);

      /* #320 Prepare data that shall be passed to the caller so access to Ethernet header is possible */
      /* PRQA S 0310, 3305 1 */ /* MD_EthIf_310_Buffer, MD_EthIf_3305_Buffer */
      (*BufPtr) = (P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA))tagAwareBufPtr;

      if( virtCtrl->Type == ETHIF_Q_TAG_TAGGED )
      {
        (*LenBytePtr) += ETHIF_VLAN_TAG_LEN_BYTE;
      }
    }
#if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
#endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_RX_HEADER_PTR, errorId);
  }
#else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* EthIf_GetRxHeaderPtr() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* ETHIF_ENABLE_HEADER_ACCESS_API */

#define ETHIF_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_ZeroCopy.c
 *********************************************************************************************************************/

