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
/**        \file  LdCom.c
 *        \brief  MICROSAR Efficient COM for Large Data
 *
 *      \details  Source
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

#define LDCOM_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/
/* *INDENT-OFF* */
/* PRQA S 2006 EOF */ /* MD_MSR_14.7 */
/* PRQA S 3109 EOF */ /* MD_MSR_19.4 */
/* PRQA S 3458 EOF */ /* MD_MSR_19.4 */
/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* PRQA S 0779 EOF */ /* MD_CSL_0779 */
/* PRQA S 3355, 3356, 3358, 3359, 3325 EOF */ /* MD_CSL_3355_3356_3358_3359_3325 */
/* *INDENT-ON* */

/**********************************************************************************************************************
*  INCLUDES
**********************************************************************************************************************/
#include "LdCom.h"
#include "LdCom_Cbk.h"
#if(LDCOM_USE_INIT_POINTER == STD_ON)
# include "EcuM_Error.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of LdCom header file */
#if ((LDCOM_SW_MAJOR_VERSION != (0x02u)) ||\
     (LDCOM_SW_MINOR_VERSION != (0x00u)) ||\
     (LDCOM_SW_PATCH_VERSION != (0x01u)))
# error "Vendor specific version numbers of LdCom.c and LdCom.h are inconsistent"
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
#define LDCOM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_19.1 */
#if (LDCOM_USE_INIT_POINTER == STD_ON)
P2CONST(LdCom_ConfigType, LDCOM_VAR_ZERO_INIT, LDCOM_PBCFG) LdCom_ConfigDataPtr = NULL_PTR;
#endif
#define LDCOM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_19.1 */

/**********************************************************************************************************************
*  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define LDCOM_START_SEC_CODE
#include "MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LdCom_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, LDCOM_CODE) LdCom_Init(P2CONST(LdCom_ConfigType, AUTOMATIC, LDCOM_PBCFG) config)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LDCOM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if(LDCOM_USE_INIT_POINTER == STD_ON)
  LdCom_ConfigDataPtr = config;
  /* Check validity of config pointer */
  if(LdCom_ConfigDataPtr == NULL_PTR)
  {
# if(LDCOM_USE_ECUM_BSW_ERROR_HOOK == STD_ON)   /* COV_LDCOM_VAR_DEPENDENT_SWITCH */
    EcuM_BswErrorHook(LDCOM_MODULE_ID, ECUM_BSWERROR_NULLPTR);
# endif
    errorId = LDCOM_E_PARAM_POINTER;
  }
# if (LDCOM_FINALMAGICNUMBER == STD_ON) /* COV_LDCOM_VAR_DEPENDENT_SWITCH */
  else if(LdCom_GetFinalMagicNumber() != LDCOM_FINAL_MAGIC_NUMBER)
  {
#  if(LDCOM_USE_ECUM_BSW_ERROR_HOOK == STD_ON)  /* COV_LDCOM_VAR_DEPENDENT_SWITCH */
    EcuM_BswErrorHook(LDCOM_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
#  endif
    errorId = LDCOM_E_PARAM_POINTER;
    LdCom_ConfigDataPtr = NULL_PTR;
  }
# endif
  else
#else
# if (LDCOM_DEV_ERROR_DETECT == STD_ON) /* COV_LDCOM_VAR_DEV_ERROR_DETECT */
  if(config != NULL_PTR)
  {
    errorId = LDCOM_E_PARAM_POINTER;
  }
  else
# endif
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    LdCom_SetInitialized(TRUE);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (LDCOM_DEV_ERROR_REPORT == STD_ON)
  if(errorId != LDCOM_E_NO_ERROR)
  {
    (void) Det_ReportError(LDCOM_MODULE_ID, LDCOM_INSTANCE_ID_DET, LDCOM_SID_INIT, errorId);
  }
#endif
  LDCOM_DUMMY_STATEMENT(config);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}

/**********************************************************************************************************************
 *  LdCom_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, LDCOM_CODE) LdCom_InitMemory(void)
{
  LdCom_SetInitialized(FALSE);
#if(LDCOM_USE_INIT_POINTER == STD_ON)
  LdCom_ConfigDataPtr = NULL_PTR;
#endif
}

/**********************************************************************************************************************
 *  LdCom_DeInit()
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
FUNC(void, LDCOM_CODE) LdCom_DeInit(void)
{
  /* ----- Development Error Checks ------------------------------------- */
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)  /* COV_LDCOM_VAR_DEV_ERROR_DETECT */
  if(LdCom_IsInitialized())
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (LDCOM_USE_INIT_POINTER == STD_ON)
    LdCom_ConfigDataPtr = NULL_PTR;
#endif
    LdCom_SetInitialized(FALSE);
  }
#if (LDCOM_DEV_ERROR_REPORT == STD_ON)
  else
  {
    (void) Det_ReportError(LDCOM_MODULE_ID, LDCOM_INSTANCE_ID_DET, LDCOM_SID_DE_INIT, LDCOM_E_UNINIT);
  }
#endif
}

#if (LDCOM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  LdCom_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, LDCOM_CODE) LdCom_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, LDCOM_APPL_DATA) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LDCOM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (LDCOM_DEV_ERROR_DETECT == STD_ON) /* COV_LDCOM_VAR_DEV_ERROR_DETECT */
  if(versioninfo == NULL_PTR)
  {
    errorId = LDCOM_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID = LDCOM_VENDOR_ID;    /* SBSW_LDCOM_GETVERSION_PARAM */
    versioninfo->moduleID = LDCOM_MODULE_ID;    /* SBSW_LDCOM_GETVERSION_PARAM */

    versioninfo->sw_major_version = LDCOM_SW_MAJOR_VERSION;     /* SBSW_LDCOM_GETVERSION_PARAM */
    versioninfo->sw_minor_version = LDCOM_SW_MINOR_VERSION;     /* SBSW_LDCOM_GETVERSION_PARAM */
    versioninfo->sw_patch_version = LDCOM_SW_PATCH_VERSION;     /* SBSW_LDCOM_GETVERSION_PARAM */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (LDCOM_DEV_ERROR_REPORT == STD_ON)
  if(errorId != LDCOM_E_NO_ERROR)
  {
    (void) Det_ReportError(LDCOM_MODULE_ID, LDCOM_INSTANCE_ID_DET, LDCOM_SID_GET_VERSION_INFO, errorId);
  }
# endif
  LDCOM_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}
#endif

/**********************************************************************************************************************
 * LdCom_Transmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LDCOM_CODE) LdCom_Transmit(PduIdType Id, P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = LDCOM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)  /* COV_LDCOM_VAR_DEV_ERROR_DETECT */
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'Id' */
  else if(Id >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_SIGNAL_ID;
  }
# if (LDCOM_INVALIDHNDOFRXTXPDUINFO == STD_ON)
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(Id))
  {
    errorId = LDCOM_E_INVALID_SIGNAL_ID;
  }
# endif
  /* Check if the transmitted Pdu/signal is a Tx Pdu */
  else if(!LdCom_IsTxPduOfRxTxPduInfo(Id))
  {
    errorId = LDCOM_E_INVALID_SIGNAL_ID;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = PduR_LdComTransmit(LdCom_GetPduRTxHandleIdOfRxTxPduInfo(Id), PduInfoPtr);  /* SBSW_LDCOM_VALID_PTR_ENSURED_BY_CALLING_FUNCTION */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (LDCOM_DEV_ERROR_REPORT == STD_ON)
  if(errorId != LDCOM_E_NO_ERROR)
  {
    (void) Det_ReportError(LDCOM_MODULE_ID, LDCOM_INSTANCE_ID_DET, LDCOM_SID_TRANSMIT, errorId);
  }
#endif
  LDCOM_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(Id);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(PduInfoPtr);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return retVal;
}

#if (LDCOM_COMMUNICATION_INTERFACE == STD_ON)
/**********************************************************************************************************************
 * LdCom_RxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, LDCOM_CODE) LdCom_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LDCOM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (LDCOM_DEV_ERROR_DETECT == STD_ON) /* COV_LDCOM_VAR_DEV_ERROR_DETECT */
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'RxPduId' */
  else if(RxPduId >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
#  if (LDCOM_INVALIDHNDOFRXTXPDUINFO == STD_ON)
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(RxPduId))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
#  endif
  /* Check if the handle Id refers to a Rx Pdu */
  else if(LdCom_IsTxPduOfRxTxPduInfo(RxPduId))   /* COV_LDCOM_FEATURE_ALWAYS_FALSE */ /* PRQA S 3201 3 */ /* MD_LdCom_3201 */
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to an If Pdu */
  else if(!LdCom_IsIfPduOfRxTxPduInfo(RxPduId))  /* COV_LDCOM_FEATURE_ALWAYS_FALSE */ /* PRQA S 3201 3 */ /* MD_LdCom_3201 */
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (LDCOM_IFRXINDICATIONFCTPTRUSEDOFRXTXPDUINFO == STD_ON)
    if(LdCom_IsIfRxIndicationFctPtrUsedOfRxTxPduInfo(RxPduId))  /* PRQA S 3355, 3358 */ /* MD_CSL_3355_3356_3358_3359 */
    {
      LdCom_GetIfRxIndicationFctPtr(LdCom_GetIfRxIndicationFctPtrIdxOfRxTxPduInfo(RxPduId)) (PduInfoPtr);       /* SBSW_LDCOM_API_CALL_FORWARDING_WITH_PTR */
    }
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (LDCOM_DEV_ERROR_REPORT == STD_ON)
  if(errorId != LDCOM_E_NO_ERROR)
  {
    (void) Det_ReportError(LDCOM_MODULE_ID, LDCOM_INSTANCE_ID_DET, LDCOM_SID_RX_INDICATION, errorId);
  }
# endif
  LDCOM_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(RxPduId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(PduInfoPtr);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */

# if (LDCOM_TX_CONFIRMATION_API == STD_ON)
/**********************************************************************************************************************
 * LdCom_TxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, LDCOM_CODE) LdCom_TxConfirmation(PduIdType TxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LDCOM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (LDCOM_DEV_ERROR_DETECT == STD_ON)        /* COV_LDCOM_VAR_DEV_ERROR_DETECT */
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'TxPduId' */
  else if(TxPduId >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
#   if (LDCOM_INVALIDHNDOFRXTXPDUINFO == STD_ON)
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(TxPduId))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
#   endif
  /* Check if the handle Id refers to a Tx Pdu */
  else if(!LdCom_IsTxPduOfRxTxPduInfo(TxPduId)) /* COV_LDCOM_FEATURE_ALWAYS_FALSE */ /* PRQA S 3201 3 */ /* MD_LdCom_3201 */
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to an If Pdu */
  else if(!LdCom_IsIfPduOfRxTxPduInfo(TxPduId)) /* COV_LDCOM_FEATURE_ALWAYS_FALSE */ /* PRQA S 3201 3 */ /* MD_LdCom_3201 */
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
#  if (LDCOM_IFTXCONFIRMATIONFCTPTRUSEDOFRXTXPDUINFO == STD_ON)
    if(LdCom_IsIfTxConfirmationFctPtrUsedOfRxTxPduInfo(TxPduId))
    {
      LdCom_GetIfTxConfirmationFctPtr(LdCom_GetIfTxConfirmationFctPtrIdxOfRxTxPduInfo(TxPduId)) ();     /* SBSW_LDCOM_API_CALL_FORWARDING */
    }
#  endif
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (LDCOM_DEV_ERROR_REPORT == STD_ON)
  if(errorId != LDCOM_E_NO_ERROR)
  {
    (void) Det_ReportError(LDCOM_MODULE_ID, LDCOM_INSTANCE_ID_DET, LDCOM_SID_TX_CONFIRMATION, errorId);
  }
#  endif
  LDCOM_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(TxPduId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif

# if (LDCOM_TRIGGER_TRANSMIT_API == STD_ON)
/**********************************************************************************************************************
 * LdCom_TriggerTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 3673 2 */ /* MD_LdCom_3673_AsrPrototype */
FUNC(Std_ReturnType, LDCOM_CODE) LdCom_TriggerTransmit(PduIdType TxPduId, P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = LDCOM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (LDCOM_DEV_ERROR_DETECT == STD_ON)        /* COV_LDCOM_VAR_DEV_ERROR_DETECT */
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'TxPduId' */
  else if(TxPduId >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
#   if (LDCOM_INVALIDHNDOFRXTXPDUINFO == STD_ON)
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(TxPduId))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
#   endif
  /* Check if the handle Id refers to a Tx Pdu */
  else if(!LdCom_IsTxPduOfRxTxPduInfo(TxPduId)) /* COV_LDCOM_FEATURE_ALWAYS_FALSE */ /* PRQA S 3201 3 */ /* MD_LdCom_3201 */
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to an If Pdu */
  else if(!LdCom_IsIfPduOfRxTxPduInfo(TxPduId)) /* COV_LDCOM_FEATURE_ALWAYS_FALSE */ /* PRQA S 3201 3 */ /* MD_LdCom_3201 */
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
#  if (LDCOM_IFTXTRIGGERTRANSMITFCTPTRUSEDOFRXTXPDUINFO == STD_ON)
    if(LdCom_IsIfTxTriggerTransmitFctPtrUsedOfRxTxPduInfo(TxPduId))
    {
      retVal = LdCom_GetIfTxTriggerTransmitFctPtr(LdCom_GetIfTxTriggerTransmitFctPtrIdxOfRxTxPduInfo(TxPduId)) (PduInfoPtr);    /* SBSW_LDCOM_API_CALL_FORWARDING_WITH_PTR */
    }
#  endif
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (LDCOM_DEV_ERROR_REPORT == STD_ON)
  if(errorId != LDCOM_E_NO_ERROR)
  {
    (void) Det_ReportError(LDCOM_MODULE_ID, LDCOM_INSTANCE_ID_DET, LDCOM_SID_TRIGGER_TRANSMIT, errorId);
  }
#  endif
  LDCOM_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(TxPduId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(PduInfoPtr);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return retVal;
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif
#endif

#if (LDCOM_TRANSPORT_PROTOCOL == STD_ON)
/**********************************************************************************************************************
 * LdCom_StartOfReception()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(BufReq_ReturnType, LDCOM_CODE) LdCom_StartOfReception(PduIdType id,
                                                           P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info,
                                                           PduLengthType TpSduLength, P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) bufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint8 errorId = LDCOM_E_NO_ERROR;

/* ----- Development Error Checks ------------------------------------- */
# if (LDCOM_DEV_ERROR_DETECT == STD_ON) /* COV_LDCOM_VAR_DEV_ERROR_DETECT */
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if(id >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
#  if (LDCOM_INVALIDHNDOFRXTXPDUINFO == STD_ON)
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(id))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
#  endif
  /* Check if the handle Id refers to a Rx Pdu */
  else if(LdCom_IsTxPduOfRxTxPduInfo(id))       /* COV_LDCOM_FEATURE_ALWAYS_FALSE */ /* PRQA S 3201 3 */ /* MD_LdCom_3201 */
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to an Tp Pdu */
  else if(LdCom_IsIfPduOfRxTxPduInfo(id))       /* COV_LDCOM_FEATURE_ALWAYS_FALSE */ /* PRQA S 3201 3 */ /* MD_LdCom_3201 */
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (LDCOM_TPSTARTOFRECEPTIONFCTPTRUSEDOFRXTXPDUINFO == STD_ON)
    if(LdCom_IsTpStartOfReceptionFctPtrUsedOfRxTxPduInfo(id))
    {
      retVal = LdCom_GetTpStartOfReceptionFctPtr(LdCom_GetTpStartOfReceptionFctPtrIdxOfRxTxPduInfo(id)) (info, TpSduLength, bufferSizePtr);     /* SBSW_LDCOM_API_CALL_FORWARDING_WITH_PTR */
    }
# endif
  }

/* ----- Development Error Report --------------------------------------- */
# if (LDCOM_DEV_ERROR_REPORT == STD_ON)
  if(errorId != LDCOM_E_NO_ERROR)
  {
    (void) Det_ReportError(LDCOM_MODULE_ID, LDCOM_INSTANCE_ID_DET, LDCOM_SID_START_OF_RECEPTION, errorId);
  }
# endif
  LDCOM_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(id);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(info);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(TpSduLength);   /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(bufferSizePtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return retVal;
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * LdCom_CopyRxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 3673 3 */ /* MD_LdCom_3673_AsrPrototype */
FUNC(BufReq_ReturnType, LDCOM_CODE) LdCom_CopyRxData(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info, P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) bufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint8 errorId = LDCOM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (LDCOM_DEV_ERROR_DETECT == STD_ON) /* COV_LDCOM_VAR_DEV_ERROR_DETECT */
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if(id >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
#  if (LDCOM_INVALIDHNDOFRXTXPDUINFO == STD_ON)
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(id))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
#  endif
  /* Check if the handle Id refers to a Rx Pdu */
  else if(LdCom_IsTxPduOfRxTxPduInfo(id))       /* COV_LDCOM_FEATURE_ALWAYS_FALSE */ /* PRQA S 3201 3 */ /* MD_LdCom_3201 */
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to an Tp Pdu */
  else if(LdCom_IsIfPduOfRxTxPduInfo(id))       /* COV_LDCOM_FEATURE_ALWAYS_FALSE */ /* PRQA S 3201 3 */ /* MD_LdCom_3201 */
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (LDCOM_TPCOPYRXDATAFCTPTRUSEDOFRXTXPDUINFO == STD_ON)
    if(LdCom_IsTpCopyRxDataFctPtrUsedOfRxTxPduInfo(id))
    {
      retVal = LdCom_GetTpCopyRxDataFctPtr(LdCom_GetTpCopyRxDataFctPtrIdxOfRxTxPduInfo(id)) (info, bufferSizePtr);      /* SBSW_LDCOM_API_CALL_FORWARDING_WITH_PTR */
    }
# endif
  }

/* ----- Development Error Report --------------------------------------- */
# if (LDCOM_DEV_ERROR_REPORT == STD_ON)
  if(errorId != LDCOM_E_NO_ERROR)
  {
    (void) Det_ReportError(LDCOM_MODULE_ID, LDCOM_INSTANCE_ID_DET, LDCOM_SID_COPY_RX_DATA, errorId);
  }
# endif
  LDCOM_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(id);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(info);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(bufferSizePtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return retVal;
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * LdCom_TpRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, LDCOM_CODE) LdCom_TpRxIndication(PduIdType id, Std_ReturnType result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LDCOM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (LDCOM_DEV_ERROR_DETECT == STD_ON) /* COV_LDCOM_VAR_DEV_ERROR_DETECT */
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if(id >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
#  if (LDCOM_INVALIDHNDOFRXTXPDUINFO == STD_ON)
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(id))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
#  endif
  /* Check if the handle Id refers to a Rx Pdu */
  else if(LdCom_IsTxPduOfRxTxPduInfo(id))       /* COV_LDCOM_FEATURE_ALWAYS_FALSE */ /* PRQA S 3201 3 */ /* MD_LdCom_3201 */
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to an Tp Pdu */
  else if(LdCom_IsIfPduOfRxTxPduInfo(id))       /* COV_LDCOM_FEATURE_ALWAYS_FALSE */ /* PRQA S 3201 3 */ /* MD_LdCom_3201 */
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (LDCOM_TPRXINDICATIONFCTPTRUSEDOFRXTXPDUINFO == STD_ON)
    if(LdCom_IsTpRxIndicationFctPtrUsedOfRxTxPduInfo(id))
    {
      LdCom_GetTpRxIndicationFctPtr(LdCom_GetTpRxIndicationFctPtrIdxOfRxTxPduInfo(id)) (result);        /* SBSW_LDCOM_API_CALL_FORWARDING */
    }
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (LDCOM_DEV_ERROR_REPORT == STD_ON)
  if(errorId != LDCOM_E_NO_ERROR)
  {
    (void) Det_ReportError(LDCOM_MODULE_ID, LDCOM_INSTANCE_ID_DET, LDCOM_SID_TP_RX_INDICATION, errorId);
  }
# endif
  LDCOM_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(id);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(result);        /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */

/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 3673 4 */ /* MD_LdCom_3673_AsrPrototype */
FUNC(BufReq_ReturnType, LDCOM_CODE) LdCom_CopyTxData(PduIdType id,
                                                     P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info,
                                                     P2VAR(RetryInfoType, AUTOMATIC, LDCOM_APPL_DATA) retry, P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) availableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint8 errorId = LDCOM_E_NO_ERROR;

/* ----- Development Error Checks ------------------------------------- */
# if (LDCOM_DEV_ERROR_DETECT == STD_ON) /* COV_LDCOM_VAR_DEV_ERROR_DETECT */
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if(id >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
#  if (LDCOM_INVALIDHNDOFRXTXPDUINFO == STD_ON)
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(id))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
#  endif
  /* Check if the handle Id refers to a Tx Pdu */
  else if(!LdCom_IsTxPduOfRxTxPduInfo(id))      /* COV_LDCOM_FEATURE_ALWAYS_FALSE */ /* PRQA S 3201 3 */ /* MD_LdCom_3201 */
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to an Tp Pdu */
  else if(LdCom_IsIfPduOfRxTxPduInfo(id))       /* COV_LDCOM_FEATURE_ALWAYS_FALSE */ /* PRQA S 3201 3 */ /* MD_LdCom_3201 */
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (LDCOM_TPCOPYTXDATAFCTPTRUSEDOFRXTXPDUINFO == STD_ON)
    if(LdCom_IsTpCopyTxDataFctPtrUsedOfRxTxPduInfo(id))
    {
      retVal = LdCom_GetTpCopyTxDataFctPtr(LdCom_GetTpCopyTxDataFctPtrIdxOfRxTxPduInfo(id)) (info, retry, availableDataPtr);    /* SBSW_LDCOM_API_CALL_FORWARDING_WITH_PTR */
    }
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (LDCOM_DEV_ERROR_REPORT == STD_ON)
  if(errorId != LDCOM_E_NO_ERROR)
  {
    (void) Det_ReportError(LDCOM_MODULE_ID, LDCOM_INSTANCE_ID_DET, LDCOM_SID_COPY_TX_DATA, errorId);
  }
# endif
  LDCOM_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(id);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(info);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(retry); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(availableDataPtr);      /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return retVal;
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * LdCom_TpTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, LDCOM_CODE) LdCom_TpTxConfirmation(PduIdType id, Std_ReturnType result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LDCOM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (LDCOM_DEV_ERROR_DETECT == STD_ON) /* COV_LDCOM_VAR_DEV_ERROR_DETECT */
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if(id >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
#  if (LDCOM_INVALIDHNDOFRXTXPDUINFO == STD_ON)
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(id))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
#  endif
  /* Check if the handle Id refers to a Tx Pdu */
  else if(!LdCom_IsTxPduOfRxTxPduInfo(id))      /* COV_LDCOM_FEATURE_ALWAYS_FALSE */ /* PRQA S 3201 3 */ /* MD_LdCom_3201 */
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to an Tp Pdu */
  else if(LdCom_IsIfPduOfRxTxPduInfo(id))       /* COV_LDCOM_FEATURE_ALWAYS_FALSE */ /* PRQA S 3201 3 */ /* MD_LdCom_3201 */
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (LDCOM_TPTXCONFIRMATIONFCTPTRUSEDOFRXTXPDUINFO == STD_ON)
    if(LdCom_IsTpTxConfirmationFctPtrUsedOfRxTxPduInfo(id))
    {
      LdCom_GetTpTxConfirmationFctPtr(LdCom_GetTpTxConfirmationFctPtrIdxOfRxTxPduInfo(id)) (result);    /* SBSW_LDCOM_API_CALL_FORWARDING */
    }
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (LDCOM_DEV_ERROR_REPORT == STD_ON)
  if(errorId != LDCOM_E_NO_ERROR)
  {
    (void) Det_ReportError(LDCOM_MODULE_ID, LDCOM_INSTANCE_ID_DET, LDCOM_SID_TP_TX_CONFIRMATION, errorId);
  }
# endif
  LDCOM_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(id);    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  LDCOM_DUMMY_STATEMENT(result);        /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#define LDCOM_STOP_SEC_CODE
#include "MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_19.1 */

/**********************************************************************************************************************
   GLOBAL MISRA / PCLINT JUSTIFICATION
**********************************************************************************************************************/
/* *INDENT-OFF* */
/* SBSW_JUSTIFICATION_BEGIN

  \ID SBSW_LDCOM_GETVERSION_PARAM
    \DESCRIPTION    The LdCom_GetVersionInfo writes to the 'versioninfo' pointer parameter. It is checked against NULL,
                    but this does not guarantee that the referenced memory is valid.
    \COUNTERMEASURE \N  Pointers passed to public LdCom APIs point to a valid memory range. A general countermeasure is included in the safety manual.

  \ID SBSW_LDCOM_VALID_PTR_ENSURED_BY_CALLING_FUNCTION
    \DESCRIPTION A pointer is written or passed to a function. The pointer can possibly be used to write to the corresponding memory location.
    \COUNTERMEASURE \N The calling function ensures that the pointer is valid.

  \ID SBSW_LDCOM_API_CALL_FORWARDING_WITH_PTR
    \DESCRIPTION    The API is called with a pointer.
                    The API call is forwarded using a function pointer which is read using ComStackLib macros.
    \COUNTERMEASURE \N The function pointer is read using the passed Id and ComStackLib macros.
                       The Id is a SNV provided by the lower layer. It is checked when it was first passed to the LdCom module.
                       The pointer is forwarded only. The calling function ensures that the pointer is valid.

  \ID SBSW_LDCOM_API_CALL_FORWARDING
    \DESCRIPTION    The API call is forwarded using a function pointer which is read using ComStackLib macros.
    \COUNTERMEASURE \N The function pointer is read using the passed Id and ComStackLib macros.
                       The Id is a SNV provided by the lower layer. It is checked when it was first passed to the LdCom module.

  SBSW_JUSTIFICATION_END */

/* module specific MISRA deviations:
    MD_LdCom_3673_AsrPrototype:
    Reason:     Function prototype is based on AUTOSAR specification. No optimization possible.
    Risk:       Unexpected value modification.
    Prevention: Covered by code review.

    MD_LdCom_3201:
     Reason:     Depending on the configuration (If/Tp or Rx/Tx only) the previous decision can always be false. In this case the statement is unreachable.
     Risk:       No risk
     Prevention: No prevention necessary
*/
/* COV_JUSTIFICATION_BEGIN

  \ID COV_LDCOM_VAR_DEV_ERROR_DETECT
    \ACCEPT TX
    \REASON Det error detection must always be enabled for SafeBsw.

  \ID COV_LDCOM_VAR_DEPENDENT_SWITCH
    \ACCEPT TX
    \REASON The nested switch depends on a previous switch.

  \ID COV_LDCOM_FEATURE_ALWAYS_FALSE
    \ACCEPT XF
    \REASON Depending on the configuration (If/Tp or Rx/Tx only) the parameter check can always be false. In this case the compiler removes the check.

COV_JUSTIFICATION_END */

/* *INDENT-ON* */

/**********************************************************************************************************************
 *  END OF FILE: LdCom.c
 *********************************************************************************************************************/
