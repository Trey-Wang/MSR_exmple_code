/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector
 * Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set
 * out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file
 *        \brief  vIpc source file
 *
 *      \details  Main source file containing all public API implementations
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

/* Configuration, interrupt handling implementations differ from the source
 * identification define used in this example.
 * The naming schemes for those files can be taken from this list:
 *
 * vIpc_LCfg.c:    VIPC_LCFG_SOURCE
 * vIpc_PBCfg.c:   VIPC_PBCFG_SOURCE
 * vIpc_Irq.c:     VIPC_IRQ_SOURCE
 */
#define VIPC_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vIpc.h"
#include "vIpc_PQ.h"
#include "vIpc_Tx.h"
#include "vIpc_Rx.h"
#include "SchM_vIpc.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of vIpc header file */
#if ((VIPC_SW_MAJOR_VERSION != (0u)) || (VIPC_SW_MINOR_VERSION != (3u)) || (VIPC_SW_PATCH_VERSION != (0u)))
#error "Vendor specific version numbers of vIpc.c and vIpc.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if ((VIPC_CFG_MAJOR_VERSION != (0u)) || (VIPC_CFG_MINOR_VERSION != (2u)))
#error "Version numbers of vIpc.c and vIpc_Cfg.h are inconsistent!"
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

#define VIPC_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* clang-format off */
VAR(vIpc_IsInitialized, VIPC_VAR_ZERO_INIT) vIpc_InitStatus = VIPC_NOT_INITIALIZED;
/* clang-format on */

#define VIPC_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define VIPC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vIpc_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, VIPC_CODE) vIpc_InitMemory(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #20 Set vIpc to 'not initialized' */
  vIpc_InitStatus = VIPC_NOT_INITIALIZED;
} /* vIpc_InitMemory */

/**********************************************************************************************************************
 *  vIpc_Init()
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
FUNC(void, VIPC_CODE) vIpc_Init(P2CONST(vIpc_ConfigType, AUTOMATIC, VIPC_PBCFG) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  vIpc_ErrorCode errorCode = VIPC_E_NO_ERROR;
  VIPC_DUMMY_STATEMENT_CONST(ConfigPtr); /* PRQA S 3112 */ /* MD_MSR_14.2 */

  /* #10 If Development Errors are enabled and vIpc is already initialized, report an error. */
  if ((vIpc_XCfg_DevErrorDetect() == TRUE) && (vIpc_XCfg_IsVIpcInitialized() == TRUE)) /* PRQA S 3415 */ /* MD_vIpc_12.4 */
  {
    errorCode = VIPC_E_ALREADY_INITIALIZED;
  }
  /* #20 Otherwise:
   *       Initialize each configured Rx and Tx physical channel.
   *       Initialize each Tx connection.
   *       Initialize the queues. */
  else
  {
    PduIdType i;

    /* ----- Implementation ----------------------------------------------- */
    vIpc_InitStatus = VIPC_INITIALIZED;

    for (i = 0; i < vIpc_XCfg_GetSizeOfRxConnections(); i++)
    {
      vIpc_XCfg_SetRxCon_State(i, VIPC_RX_SDU_STATE_IDLE);
      vIpc_XCfg_SetRxPayloadLength(i, 0u);
      vIpc_XCfg_SetRxRemainingPayloadLength(i, 0u);
    }

    for (i = 0; i < vIpc_XCfg_GetSizeOfTxChannel(); i++)
    {
      vIpc_XCfg_TxChan_SetActiveTxConId(i, 0);
      vIpc_XCfg_TxChan_SetAlreadySentDataOfCurrSeg(i, 0);
      vIpc_XCfg_TxChan_SetIsActiveTxConAvailable(i, FALSE);
      vIpc_XCfg_TxChan_InitRemainingSeparationCycles(i);
    }

    for (i = 0; i < vIpc_XCfg_GetSizeOfTxConnections(); i++)
    {
      vIpc_XCfg_TxCon_SetCurrentSegment(i, 0);
      vIpc_XCfg_TxCon_SetMessageType(i, VIPC_MSG_SF);
      vIpc_XCfg_TxCon_SetRemCompleteLength(i, 0);
      vIpc_XCfg_TxCon_SetRemPayloadLength(i, 0);
      vIpc_XCfg_TxCon_SetState(i, VIPC_TX_CON_STATE_IDLE);
    }

    vIpc_PQ_Init();
  }

  (void) vIpc_XCfg_ReportError(VIPC_SID_INIT, errorCode);

} /* vIpc_Init */ /* PRQA 6050 1 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  vIpc_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, VIPC_CODE) vIpc_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, VIPC_APPL_VAR) VersionInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  vIpc_ErrorCode errorId = VIPC_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 If Dev Error Detect is enabled and the version pointer is valid. */
  if ((vIpc_XCfg_DevErrorDetect() == TRUE) && (VersionInfo == NULL_PTR))
  {
    errorId = VIPC_E_INV_POINTER;
  }
  /* #20 Otherwise, set the version info the the values from the component header. */
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    VersionInfo->vendorID         = (VIPC_VENDOR_ID);
    VersionInfo->moduleID         = (VIPC_MODULE_ID);
    VersionInfo->sw_major_version = (VIPC_SW_MAJOR_VERSION);
    VersionInfo->sw_minor_version = (VIPC_SW_MINOR_VERSION);
    VersionInfo->sw_patch_version = (VIPC_SW_PATCH_VERSION);
  }

  /* ----- Development Error Report --------------------------------------- */
  (void) vIpc_XCfg_ReportError(VIPC_SID_GET_VERSION_INFO, errorId);
} /* vIpc_GetVersionInfo */

/**********************************************************************************************************************
 * vIpc_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, VIPC_CODE) vIpc_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType channelId;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Execute main function only if vIpc is initialized. */
  if (vIpc_XCfg_IsVIpcInitialized() == TRUE)
  {
    /* #20 Iterate over all Tx channels.
     *       If the channel is idle and the delay is past, trigger next transmission.
     *       Otherwise, decrement the delay counter. */
    for (channelId = 0; channelId < vIpc_XCfg_GetSizeOfTxChannel(); channelId++)
    {
      if ((vIpc_XCfg_TxChan_GetRemainingSeparationCycles(channelId) == 0)
       && (vIpc_XCfg_IsLLBusy(channelId) == FALSE)) /* PRQA S 3415 */ /* MD_vIpc_12.4 */
      {
        vIpc_Tx_Process(channelId);
      }
      else
      {
        vIpc_XCfg_TxChan_DecRemainingSeparationCycles(channelId);
      }
    }
  }
} /* vIpc_MainFunction */

#define VIPC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:
 MD_vIpc_12.4:    Rule 12.4
      Reason:     The called functions to not have side-effects, but are only getter functions.
      Risk:       There is no risk as there is no side-effect.
      Prevention: Covered by code review.
 MD_vIpc_19.10    Rule 19.10
      Reason:     The macros would not create valid c code if enclosed in parentheses.
      Risk:       Risk is minimal because it can not compile with invalid parameters.
      Prevention: Covered by compiler.
 MD_vIpc_8.7      Rule 8.7
      Reason:     Looks cleaner and is easier to change.
      Risk:       Risk that compiler will not inline.
      Prevention: -
 */

/* SBSW_JUSTIFICATION_BEGIN

 */

/* START_COVERAGE_JUSTIFICATION

 */

/**********************************************************************************************************************
 *  END OF FILE: vIpc.c
 *********************************************************************************************************************/
