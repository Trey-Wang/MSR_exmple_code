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
/**        \file  EthIf_Time.c
 *        \brief  Ethernet Interface timer handling extension source file
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

#define ETHIF_TIME_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthIf_Time.h"
#include "EthIf.h"
#include "EthIf_Int.h"
#include "EthIf_EthCtrlCfg.h"
#include "EthIf_CfgAccess_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of EthIf header file */
/* \trace SPEC-2393557 */
#if (  (ETHIF_SW_MAJOR_VERSION != (8u)) \
    || (ETHIF_SW_MINOR_VERSION != (0u)) \
    || (ETHIF_SW_PATCH_VERSION != (1u)) )
# error "Vendor specific version numbers of EthIf_Time.c and EthIf.h are inconsistent"
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

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#if ( ETHIF_GLOBAL_TIME_SUPPORT == STD_ON )
/***********************************************************************************************************************
 *  EthIf_GetCurrentTime
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetCurrentTime(
        uint8                                              CtrlIdx,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETHIF_APPL_VAR)  timeQualPtr,
  P2VAR(Eth_TimeStampType,     AUTOMATIC, ETHIF_APPL_VAR)  timeStampPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393902 */
    /* \trace SPEC-2393906 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    /* \trace SPEC-2393903 */
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( timeStampPtr == NULL_PTR )
  {
    /* \trace SPEC-2393904 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( timeQualPtr == NULL_PTR )
  {
    /* \trace SPEC-2393904 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_EthCtrlType, AUTOMATIC, ETHIF_CONST)  ethCtrl;

    ethCtrl = EthIf_CfgAccess_VirtCtrl(CtrlIdx).EthCtrl;

    /* #30 Check if driver provides the API */
    if( ethCtrl->DrvApi->GetGlobalTime != NULL_PTR )
    {
      /* #300 Redirect call to the driver */
      retVal = ethCtrl->DrvApi->GetGlobalTime(ethCtrl->CtrlIdx,
                                              timeStampPtr,
                                              timeQualPtr);
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
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_CURRENT_TIME, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* EthIf_GetCurrentTime() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  EthIf_SetGlobalTime
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetGlobalTime(
          uint8                                            CtrlIdx,
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHIF_APPL_CONST)  TimestampPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393928 */
    /* \trace SPEC-2393932 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    /* \trace SPEC-2393929 */
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( TimestampPtr == NULL_PTR )
  {
    /* \trace SPEC-2393930 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_EthCtrlType, AUTOMATIC, ETHIF_CONST)  ethCtrl;

    ethCtrl = EthIf_CfgAccess_VirtCtrl(CtrlIdx).EthCtrl;

    /* #30 Check if driver provides the API */
    if( ethCtrl->DrvApi->SetGlobalTime != NULL_PTR )
    {
      /* #300 Redirect call to the driver */
      retVal = ethCtrl->DrvApi->SetGlobalTime(ethCtrl->CtrlIdx,
                                              TimestampPtr);
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
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SET_GLOBAL_TIME, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* EthIf_SetGlobalTime() */

/***********************************************************************************************************************
 *  EthIf_SetCorrectionTime
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetCorrectionTime(
          uint8                                            CtrlIdx,
  P2CONST(Eth_TimediffType,  AUTOMATIC, ETHIF_APPL_CONST)  OffsetTimePtr,
  P2CONST(Eth_RateRatioType, AUTOMATIC, ETHIF_APPL_CONST)  RateRatioPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2394060 */
    /* \trace SPEC-2393926 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    /* \trace SPEC-2393923 */
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( OffsetTimePtr == NULL_PTR )
  {
    /* \trace SPEC-2393924 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( RateRatioPtr == NULL_PTR )
  {
    /* \trace SPEC-2393924 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_EthCtrlType, AUTOMATIC, ETHIF_CONST)  ethCtrl;

    ethCtrl = EthIf_CfgAccess_VirtCtrl(CtrlIdx).EthCtrl;

    /* #30 Check if driver provides the API */
    if( ethCtrl->DrvApi->SetCorrectionTime != NULL_PTR )
    {
      /* #300 Redirect call to the driver */
      retVal = ethCtrl->DrvApi->SetCorrectionTime(ethCtrl->CtrlIdx,
                                                  OffsetTimePtr,
                                                  RateRatioPtr);
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      /* driver doesn't provide the API */
      errorId = ETHIF_E_INV_DRIVER_API_CALL;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SET_CORRECTION_TIME, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* EthIf_SetCorrectionTime() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  EthIf_EnableEgressTimestamp
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_EnableEgressTimestamp(
  uint8  CtrlIdx,
  uint8  BufIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2394056 */
    /* \trace SPEC-2393910 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    /* \trace SPEC-2393908 */
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( BufIdx >= EthIf_CfgAccess_MaxTxBufNum() )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_EthCtrlType, AUTOMATIC, ETHIF_CONST)  ethCtrl;

    ethCtrl = EthIf_CfgAccess_VirtCtrl(CtrlIdx).EthCtrl;

    /* #30 Check if driver provides the API */
    if( ethCtrl->DrvApi->EnableEgressTimestamp != NULL_PTR )
    {
      /* #300 Redirect call to the driver */
      retVal = ethCtrl->DrvApi->EnableEgressTimestamp(ethCtrl->CtrlIdx,
                                                      BufIdx);
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
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_ENABLE_EGRESS_TIMESTAMP, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* EthIf_EnableEgressTimestamp() */

/***********************************************************************************************************************
 *  EthIf_GetEgressTimestamp
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetEgressTimestamp(
        uint8                                                 CtrlIdx,
        uint8                                                 BufIdx,
  P2VAR(Eth_TimeStampType,        AUTOMATIC, ETHIF_APPL_VAR)  TimestampPtr,
  P2VAR(Eth_TimestampQualityType, AUTOMATIC, ETHIF_APPL_VAR)  TimestampQualityPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393912 */
    /* \trace SPEC-2393916 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    /* \trace SPEC-2393913 */
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( BufIdx >= EthIf_CfgAccess_MaxTxBufNum() )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else if( TimestampPtr == NULL_PTR )
  {
    /* \trace SPEC-2393914 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( TimestampQualityPtr == NULL_PTR )
  {
    /* \trace SPEC-2393914 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_EthCtrlType, AUTOMATIC, ETHIF_CONST)  ethCtrl;

    ethCtrl = EthIf_CfgAccess_VirtCtrl(CtrlIdx).EthCtrl;

    /* #30 Check if driver provides the API */
    if( ethCtrl->DrvApi->GetEgressTimestamp != NULL_PTR )
    {
      /* #300 Redirect call to the driver */
      retVal = ethCtrl->DrvApi->GetEgressTimestamp(ethCtrl->CtrlIdx,
                                                   BufIdx,
                                                   TimestampPtr,
                                                   TimestampQualityPtr);
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
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_EGRESS_TIMESTAMP, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* EthIf_GetEgressTimestamp() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  EthIf_GeIngressTimestamp
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetIngressTimestamp(
        uint8                                                  CtrlIdx,
  P2VAR(Eth_DataType,             AUTOMATIC, ETHIF_APPL_DATA)  DataPtr,
  P2VAR(Eth_TimeStampType,        AUTOMATIC, ETHIF_APPL_VAR)   TimestampPtr,
  P2VAR(Eth_TimestampQualityType, AUTOMATIC, ETHIF_APPL_VAR)   TimestampQualityPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2394058 */
    /* \trace SPEC-2393921 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    /* \trace SPEC-2393918 */
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( DataPtr == NULL_PTR )
  {
    /* \trace SPEC-2393919 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( TimestampPtr == NULL_PTR )
  {
    /* \trace SPEC-2393919 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( TimestampQualityPtr == NULL_PTR )
  {
    /* \trace SPEC-2393919 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_EthCtrlType, AUTOMATIC, ETHIF_CONST)  ethCtrl;

    ethCtrl   = EthIf_CfgAccess_VirtCtrl(CtrlIdx).EthCtrl;

    /* #30 Check if driver provides the API */
    if( ethCtrl->DrvApi->GetIngressTimestamp != NULL_PTR )
    {
      P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA)  u8DataPtr;

      u8DataPtr = (P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA))DataPtr; /* PRQA S 0310,3305 */ /* MD_EthIf_310_Buffer */

      /* #300 Adapt buffer pointer passed to driver depending on type of EthIf controller (un-tagged, tagged) */
      if( EthIf_CfgAccess_VirtCtrl(CtrlIdx).Type == ETHIF_Q_TAG_TAGGED )
      {
        u8DataPtr = &u8DataPtr[-((sint8)ETHIF_VLAN_TAG_LEN_BYTE)]; /* PRQA S 3382 */ /* MD_EthIf_3382 */
      }

      /* #310 Redirect call to the driver */
      retVal = ethCtrl->DrvApi->GetIngressTimestamp(ethCtrl->CtrlIdx,
                                                    (P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA))u8DataPtr, /* PRQA S 0310,3305 */ /* MD_EthIf_310_Buffer */
                                                    TimestampPtr,
                                                    TimestampQualityPtr);
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
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_INGRESS_TIMESTAMP, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* EthIf_GeIngressTimestamp() */ /* PRQA S 6080 */ /* MD_MSR_14.7 */

#endif /* ETHIF_GLOBAL_TIME_SUPPORT */

#define ETHIF_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_Time.c
 *********************************************************************************************************************/

