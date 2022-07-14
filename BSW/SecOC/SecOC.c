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
/**        \file  SecOC.c
 *        \brief  MICROSAR  Secure Onboard Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Secure Onboard Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
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

#define SECOC_SOURCE
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
#include "SecOC_Cbk.h"
#include "SecOC.h"
#include "SchM_SecOC.h"
#include "vstdlib.h"
#include "Rte_SecOC.h"
#include "Csm.h"
#if (SECOC_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif
#include "PduR_SecOC.h"
#if(SECOC_USE_INIT_POINTER == STD_ON)
# include "EcuM_Error.h"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
#if (SECOC_DEV_ERROR_REPORT == STD_ON)
# define SecOC_ReportDetError(API_ID, ERROR_CODE) ((void) Det_ReportError(SECOC_MODULE_ID, SECOC_INSTANCE_ID_DET, (API_ID), (ERROR_CODE)))
# define SecOC_ReportRuntimeDetError(API_ID, ERROR_CODE) ((void) Det_ReportRuntimeError( SECOC_MODULE_ID, SECOC_INSTANCE_ID_DET, (API_ID), (ERROR_CODE)))
#else
# define SecOC_ReportRuntimeDetError(API_ID, ERROR_CODE)
#endif
#if (SECOC_RXPDUINFO == STD_ON)
# define SecOC_IsRxPduStateUnverified(rxPduId) ((SecOC_GetRxPduState(rxPduId) & SECOC_UNVERIFIED_RXPDUSTATE) == SECOC_UNVERIFIED_RXPDUSTATE)
#endif
#if (SECOC_TXPDUINFO == STD_ON)
# define SecOC_IsTxPduStateUnauthenticated(txPduId) ((SecOC_GetTxPduState(txPduId) & SECOC_UNAUTHENTICATED_TXPDUSTATE) == SECOC_UNAUTHENTICATED_TXPDUSTATE)
#endif
/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
typedef enum
{
  SECOC_E_VER_OK = 0,           /* !< Verified */
  SECOC_E_VER_NOT_OK = 1,       /* !< not Verified */
  SECOC_E_VER_BUSY = 2  /* !< CSM or CAL is busy */
} SecOC_VerifyResultType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/
#if(SECOC_USE_INIT_POINTER == STD_ON)
# define SECOC_START_SEC_VAR_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*! \brief Pointer to the current configuration */
P2CONST(SecOC_ConfigType, SECOC_VAR_ZERO_INIT, SECOC_PBCFG) SecOC_ConfigDataPtr = NULL_PTR;
# define SECOC_STOP_SEC_VAR_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define SECOC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */   /* MD_MSR_19.1 */

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_FreshnessValue_GetVerifyValueFromCallout
**********************************************************************************************************************/
/*! \brief      Gets the freshness verify value form the configured callout.
    \details    -
    \param[in]  rxPduId  rx PduId
    \param[out] freshnessValue       freshness verify value buffer
    \param[in]  freshnessValueBufferLength freshness verify value buffer length
    \return     E_OK request successful
                E_NOT_OK request failed
**********************************************************************************************************************/
LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_FreshnessValue_GetVerifyValueFromCallout(SecOC_RxPduInfoIterType rxPduId, uint8 * freshnessValue, uint8 freshnessValueBufferLength);
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_RxIf_InitPdus
**********************************************************************************************************************/
/*! \brief      Init IF rx PDUs.
    \details    -
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_RxIf_InitPdus(void);
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_InitPdus
**********************************************************************************************************************/
/*! \brief      Init IF tx PDUs.
    \details    -
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_TxIf_InitPdus(void);
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_MACUsingCSM43
**********************************************************************************************************************/
/*! \brief      Verifies the MAC of a Secured I-Pdu using the CSM.
    \details  -
    \param[in]  rxPduId  rx PduId
    \return     verification result.
    \trace      SPEC-2697662
**********************************************************************************************************************/
LOCAL_INLINE FUNC(SecOC_VerifyResultType, SECOC_CODE) SecOC_Verify_MACUsingCSM43(SecOC_RxPduInfoIterType rxPduId);
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_GetByteAlignedTruncatedAuthenticator
**********************************************************************************************************************/
/*! \brief      Reads the Authenticator for the Secured PDU byte aligned.
    \details  -
    \param[in]  rxPduId  rx PduId
    \param[out]  byteAlignedAuthenticator  byte aligned authenticator
    \param[in]   byteAlignedAuthenticatorLength size of the byteAlignedAuthenticator buffer
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_GetByteAlignedTruncatedAuthenticator(SecOC_RxPduInfoIterType rxPduId, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) byteAlignedAuthenticator, SecOC_AuthenticatorCacheLengthOfRxPduInfoType byteAlignedAuthenticatorLength);
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_GetVerificationResultOfOverrideStatus
**********************************************************************************************************************/
/*! \brief      Translates the verify override status in a verification result.
    \details  -
    \param[in]  rxPduId  rx PduId
    \return     verification result.
**********************************************************************************************************************/
LOCAL_INLINE FUNC(SecOC_VerifyResultType, SECOC_CODE) SecOC_Verify_GetVerificationResultOfOverrideStatus(SecOC_RxPduInfoIterType rxPduId);
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_ProcessVerificationResult
**********************************************************************************************************************/
/*! \brief      Processes the passed verification result.
    \details  -
    \param[in]  verificationResult verification result.
    \param[in]  rxPduId  rx PduId
    \trace      SPEC-2697654, SPEC-2697659, SPEC-2968995
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_ProcessVerificationResult(SecOC_VerifyResultType verificationResult, SecOC_RxPduInfoIterType rxPduId);
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_MAC
**********************************************************************************************************************/
/*! \brief      Verifies the MAC of a Secured I-Pdu.
    \details  -
    \param[in]  rxPduId  rx PduId
    \trace      SPEC-2977504
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_MAC(SecOC_RxPduInfoIterType rxPduId);
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_RxIf_HandlePdus
**********************************************************************************************************************/
/*! \brief      Handle IF rx PDUs.
    \details    -
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_RxIf_HandlePdus(void);
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_FreshnessValue_CopyVerifyValueToBuffer
******************************************************************************************** **************************/
/*! \brief      Copys the FreshnessVerifyValue to the Data To Authenticate Buffer.
    \details  -
    \param[in]  rxPduId  rx PduId
    \param[in]  freshnessVerifyValue  the freshness value to verify
    \return     parsed freshness value
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_CopyVerifyValueToBuffer(SecOC_RxPduInfoIterType rxPduId, P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) freshnessVerifyValue);
#endif

#if (SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_FreshnessValue_GetParsedFreshnessValue
**********************************************************************************************************************/
/*! \brief      Returns the parsed freshness value.
    \details  -
    \param[in]  rxPduId  rx PduId
    \param[out] parsedFreshnessValue parsed freshness value.
    \param[in]  parsedFreshnessValueBufferLength parsed freshness value buffer length
    \return     parsed freshness value
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_GetParsedFreshnessValue(SecOC_RxPduInfoIterType rxPduId, uint8 * parsedFreshnessValue, uint8 parsedFreshnessValueBufferLength);
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Authenticate_GenerateAuthenticatorUsingCSM43
**********************************************************************************************************************/
/*! \brief      generates the MAC using the CSM
    \details  -
    \param[in]  txPduId  tx PduId
    \return     Std_ReturnType
                E_OK:      generation of the MAC using CSM was successful
                E_NOT_OK:  generation of the MAC using CSM was not successful
**********************************************************************************************************************/
LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_Authenticate_GenerateAuthenticatorUsingCSM43(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_FreshnessValue_CalculateVerifyValueFromCallout
**********************************************************************************************************************/
/*! \brief      Gets the freshness verify value for a Rx Pdu form a configured callout
    \details  -
    \param[in]  rxPduId  Rx Pdu Id
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_CalculateVerifyValueFromCallout(SecOC_RxPduInfoIterType rxPduId);
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_FreshnessValue_GetValueForTxPduOfCallout
**********************************************************************************************************************/
/*! \brief      Gets the freshness verify value for a Tx Pdu form a configured callout
    \details  -
    \param[in]  txPduId  tx PduId
    \param[out] freshnessValue freshness verify value
    \param[in]  freshnessValueBufferLength length of the freshness verify value buffer
    \param[out] truncatedFreshnessValue truncated transmit value
    \param[in]  truncatedFreshnessValueBufferLength length of the truncated freshness value buffer
    \return     STD_ON  if successful
                STD_OFF if failed
**********************************************************************************************************************/
LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_FreshnessValue_GetValueForTxPduOfCallout(SecOC_TxPduInfoIterType txPduId, uint8 * freshnessValue,  uint8 freshnessValueBufferLength, uint8 * truncatedFreshnessValue, uint8 truncatedFreshnessValueBufferLength);
#endif

#if (SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_FreshnessValue_WriteTruncFreshnessValue
**********************************************************************************************************************/
/*! \brief      Copys the truncated freshness value to the SecuredPdu buffer.
    \details  -
    \param[in]  txPduId              tx PduId
    \param[in]  trucFreshnessValue   truncated freshness value
    \return     none
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_WriteTruncFreshnessValue(SecOC_TxPduInfoIterType txPduId, P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) trucFreshnessValue);
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_FreshnessValue_CopyTxFreshnessValues
**********************************************************************************************************************/
/*! \brief      Copys the freshness value to the Data To Authenticate Buffer.
    \details  -
    \param[in]  txPduId              tx PduId
    \return     none
**********************************************************************************************************************/
LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_FreshnessValue_CopyTxFreshnessValues(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_PropagateVerificationStatus
**********************************************************************************************************************/
/*! \brief      Calls all verification status Callouts.
    \details  -
    \param[in]  rxPduId              rx PduId
    \param[in]  verificationStatus   verification status
    \return     none
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_PropagateVerificationStatus(SecOC_RxPduInfoIterType rxPduId, SecOC_VerificationResultType verificationStatus);
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Authenticate_GenerateAuthenticator
**********************************************************************************************************************/
/*! \brief      Generates the authenticator of the secured PDU.
    \details  -
    \param[in]  txPduId              tx PduId
    \return     none
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Authenticate_GenerateAuthenticator(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_HandlePdu_ReauthenticationNeeded
**********************************************************************************************************************/
/*! \brief      Processes the REAUTHENTICATION_NEEDED state of the passed Tx Pdu.
 *  \details  -
    \param[in]  txPduId  Pdu Id
    \return     none
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_TxIf_HandlePdu_ReauthenticationNeeded(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_TXSPLITPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_HandlePdu_SplitPdu
**********************************************************************************************************************/
/*! \brief      Handle the transmission of the related split Pdu.
    \details  -
    \param[in]  txPduId  Pdu Id
    \return     E_OK if successful
                E_NOT_OK if not successful
**********************************************************************************************************************/
LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_TxIf_HandlePdu_SplitPdu(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_HandlePdus
**********************************************************************************************************************/
/*! \brief      Handles Tx Pdus.
    \details  -
    \return     none
    \trace      SPEC-2697631, SPEC-2697636, SPEC-2697642, SPEC-2697701
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_TxIf_HandlePdus(void);
#endif

#if (SECOC_CURRENTMESSAGELINKER == STD_ON)
/**********************************************************************************************************************
  SecOC_Split_CheckMessageLinker
**********************************************************************************************************************/
/*! \brief          Compares the passed message linker values.
 *                  If the new value is different, the old value is replaced with the new.
 *  \details  -
    \param[in]      msgLinkNew    new message linker value.
 *  \param[in,out]  msgLinkOld    old message linker value.
 *  \param[in]      msgLinkLenInBytes   message linker length in bytes.
 *  \param[in]      currentIsNewStatus  current is new status.
 *  \return         TRUE          if the new message linker is different.
 *                  FALSE         if the message linker are equal.
**********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean, SECOC_CODE) SecOC_Split_CheckMessageLinker(P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) msgLinkNew, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) msgLinkOld, SecOC_MessageLinkLenOfRxSecuredPduInfoType msgLinkLenInBytes, boolean currentIsNewStatus);
#endif

#if ((SECOC_CURRENTMESSAGELINKER == STD_ON) || (SECOC_MESSAGELINKLENOFTXSPLITPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_Split_ExtractMessageLinker
**********************************************************************************************************************/
/*! \brief          Extracts the message linker form the source buffer and copies it to the dest buffer.
 *  \details  -
    \param[in]      msgLinkSrc    source buffer
 *  \param[out]     msgLinkDest   destination buffer
 *  \param[in]      msgLinkLen    message linker length in bits.
 *  \param[in]      msgLinkBitInByteOffset  bit in byte offset.
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Split_ExtractMessageLinker(P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) msgLinkSrc, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) msgLinkDest, uint16 msgLinkLen, uint16 msgLinkBitInByteOffset);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_FreshnessValue_GetValueForTxPduOfCallout
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_FreshnessValue_GetValueForTxPduOfCallout(SecOC_TxPduInfoIterType txPduId, uint8 * freshnessValue, uint8 freshnessValueBufferLength,  uint8 * truncatedFreshnessValue, uint8 truncatedFreshnessValueBufferLength)
{
  Std_ReturnType ret = E_NOT_OK;
  uint32 freshnessValueLength;
  freshnessValueLength = (uint32) (SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueLengthOfTxPduInfo(txPduId) << 3U);

# if ((SECOC_USE_GET_TX_FRESHNESS_TRUNC_DATA_API == STD_ON) && (SECOC_USE_GET_TX_FRESHNESS_API == STD_ON))
  if(SecOC_GetProvideTruncTxFreshnessOfTxPduInfo(txPduId) == SECOC_PROVIDE_PROVIDETRUNCTXFRESHNESSOFTXPDUINFO)
# endif
# if (SECOC_USE_GET_TX_FRESHNESS_TRUNC_DATA_API == STD_ON)
  {
    uint32 truncatedFreshnessValueLength;
    truncatedFreshnessValueLength = (uint32) SecOC_GettruncatedFreshnessValueLengthOfTxPduInfo(txPduId);
    if((truncatedFreshnessValueLength <= (uint32) (truncatedFreshnessValueBufferLength << 3U)) && (freshnessValueLength <= (uint32) (freshnessValueBufferLength << 3U))) /* COV_SECOC_SAFEBSW_CHECK */
    {
      /* *INDENT-OFF* */
      ret = GetTxFreshnessTruncData_FuncName(
          SecOC_GetExternalFreshnessIdOfTxPduInfo(txPduId),
          freshnessValue,
          &freshnessValueLength,
          truncatedFreshnessValue,
          &truncatedFreshnessValueLength
      ); /* SBSW_SECOC_PTR2FUNC_6 */
      /* *INDENT-ON* */
    }
  }
# endif
# if ((SECOC_USE_GET_TX_FRESHNESS_TRUNC_DATA_API == STD_ON) && (SECOC_USE_GET_TX_FRESHNESS_API == STD_ON))
  else
# endif
# if (SECOC_USE_GET_TX_FRESHNESS_API == STD_ON)
  {
    if(freshnessValueLength <= (uint32) (freshnessValueBufferLength << 3U)) /* COV_SECOC_SAFEBSW_CHECK */
    {
      /* *INDENT-OFF* */
      ret = GetTxFreshness_FuncName(
          SecOC_GetExternalFreshnessIdOfTxPduInfo(txPduId),
          freshnessValue,
          &freshnessValueLength
      ); /* SBSW_SECOC_PTR2FUNC_6 */
      /* *INDENT-ON* */
#  if (SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFTXPDUINFO == STD_ON)
      VStdLib_MemCpy_s(truncatedFreshnessValue, truncatedFreshnessValueBufferLength, &(freshnessValue[SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueLengthOfTxPduInfo(txPduId) - SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueLengthOfTxPduInfo(txPduId)]), SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueLengthOfTxPduInfo(txPduId)); /* SBSW_SECOC_VSTDLIB_SAFE */
#  endif
    }
  }
# endif
  SECOC_DUMMY_STATEMENT(truncatedFreshnessValueBufferLength);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  return ret;
}
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_FreshnessValue_CalculateVerifyValueFromCallout
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_CalculateVerifyValueFromCallout(SecOC_RxPduInfoIterType rxPduId)
{
  Std_ReturnType result = E_OK;
  uint8 freshnessVerifyValue[SECOC_FRESHNESS_SIZE_RXPDUS] = { 0 };

  result = SecOC_FreshnessValue_GetVerifyValueFromCallout(rxPduId, freshnessVerifyValue, SECOC_FRESHNESS_SIZE_RXPDUS); /* SBSW_SECOC_PTR2FUNC_2 */
  if(result == E_OK)
  {
    SecOC_FreshnessValue_CopyVerifyValueToBuffer(rxPduId, freshnessVerifyValue); /* SBSW_SECOC_PTR2FUNC_2 */
    SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_RXPDUSTATE); /* SBSW_SECOC_CSL02 */
  }
  else if(result == SECOC_E_BUSY)
  {
    /* Increment Build attempts and check state */
    SecOC_Verify_IncrementBuildAttempts(rxPduId);
  }
  else
  {
    SecOC_Verify_PropagateVerificationStatus(rxPduId, SECOC_FRESHNESSFAILURE);
    SecOC_ReportRuntimeDetError(SECOC_SID_MAIN_FUNCTION_RX, SECOC_E_FRESHNESS_FAILURE);
    SecOC_SetRxPduState(rxPduId, SECOC_IDLE_RXPDUSTATE); /* SBSW_SECOC_CSL02 */
  }
}
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_FreshnessValue_CopyVerifyValueToBuffer
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_CopyVerifyValueToBuffer(SecOC_RxPduInfoIterType rxPduId, P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) freshnessVerifyValue)
{
  /* get the index of the last octet of the freshness value in buffer */
  SecOC_AuthenticPduDataContainerFreshnessVerifyValueEndIdxOfRxPduInfoType destValueidx = SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueStartIdxOfRxPduInfo(rxPduId);
  uint8 srcIdx = 0;
  /* iterate from the LSB to the MSB. The address of the MSB will then be smaller then the address of the LSB */
  for(; destValueidx < SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueEndIdxOfRxPduInfo(rxPduId); destValueidx++)
  {
    SecOC_SetAuthenticPduDataContainer(destValueidx, freshnessVerifyValue[srcIdx]); /* SBSW_SECOC_CSL03 */
    srcIdx++;
  }
}
#endif

#if (SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_FreshnessValue_WriteTruncFreshnessValue
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_WriteTruncFreshnessValue(SecOC_TxPduInfoIterType txPduId, P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) trucFreshnessValue)
{
  SecOC_SecuredPduDataContainerTruncatedFreshnessValueEndIdxOfTxPduInfoType iterValue = SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueEndIdxOfTxPduInfo(txPduId);
  SecOC_SecuredPduDataContainerTruncatedFreshnessValueEndIdxOfTxPduInfoType srcValueIdx = SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueLengthOfTxPduInfo(txPduId) - 1;
  SecOC_truncatedFreshnessValueLengthOfTxPduInfoType bitsToCopy = SecOC_GettruncatedFreshnessValueLengthOfTxPduInfo(txPduId);

  uint8 firstByteShift = SecOC_GetTruncFreshnessValueFirstByteShiftOfTxPduInfo(txPduId);
  uint8 secondByteShift = SecOC_GetTruncFreshnessValueSecondByteShiftOfTxPduInfo(txPduId);

  for(; iterValue > SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueStartIdxOfTxPduInfo(txPduId); iterValue--)
  {
    SecOC_SecuredPduDataContainerTruncatedFreshnessValueEndIdxOfTxPduInfoType destValueIdx = iterValue - 1;
    if((bitsToCopy & 7U) != 0)
    {
      uint8 bitOffset = (uint8) (bitsToCopy & 7U);
      uint8 byteToCopy = (uint8) (trucFreshnessValue[srcValueIdx] << firstByteShift);
      uint8 copyMask = (uint8) (255U >> secondByteShift);

      byteToCopy = (SecOC_GetSecuredPduDataContainer(destValueIdx) & copyMask) | byteToCopy;
      SecOC_SetSecuredPduDataContainer(destValueIdx, byteToCopy); /* SBSW_SECOC_CSL03 */

      bitsToCopy = bitsToCopy - bitOffset;
    }
    else
    {
      uint8 byteToCopy = (uint8) (trucFreshnessValue[srcValueIdx] << firstByteShift);

      if(secondByteShift != 0)
      {
        byteToCopy = byteToCopy | ((uint8) (trucFreshnessValue[srcValueIdx + 1] >> secondByteShift));
      }

      SecOC_SetSecuredPduDataContainer(destValueIdx, byteToCopy); /* SBSW_SECOC_CSL03 */
      bitsToCopy = bitsToCopy - 8;
    }
    srcValueIdx--;
  }
}
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_FreshnessValue_CopyTxFreshnessValues
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_FreshnessValue_CopyTxFreshnessValues(SecOC_TxPduInfoIterType txPduId)
{
  uint8 freshnessValue[SECOC_FRESHNESS_SIZE_TXPDUS] = { 0 };    /* PRQA S 0759 */ /* MD_MSR_18.4 */
  uint8 truncatedFreshnessValue[SECOC_TRUNC_FRESHNESS_SIZE_TXPDUS] = { 0 };     /* PRQA S 0759 */ /* MD_MSR_18.4 */
  Std_ReturnType ret = E_OK;

  ret = SecOC_FreshnessValue_GetValueForTxPduOfCallout(txPduId, freshnessValue, SECOC_FRESHNESS_SIZE_TXPDUS, truncatedFreshnessValue, SECOC_TRUNC_FRESHNESS_SIZE_TXPDUS); /* SBSW_SECOC_PTR2FUNC_2 */

  if(ret == E_OK)
  {
    {
      /* get the index of the last octet of the freshness value in buffer */
      SecOC_AuthenticPduDataContainerFreshnessVerifyValueEndIdxOfTxPduInfoType valueidx = SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueStartIdxOfTxPduInfo(txPduId);

      /* iterate from the LSB to the MSB. The address of the MSB will then be smaller then the address of the LSB. */
      for(; valueidx < SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueEndIdxOfTxPduInfo(txPduId); valueidx++)
      {
        SecOC_SetAuthenticPduDataContainer(valueidx, freshnessValue[valueidx - SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueStartIdxOfTxPduInfo(txPduId)]); /* SBSW_SECOC_CSL03 */
      }
    }
# if (SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFTXPDUINFO == STD_ON)
    SecOC_FreshnessValue_WriteTruncFreshnessValue(txPduId, truncatedFreshnessValue); /* SBSW_SECOC_PTR2FUNC_2 */
# endif
  }
  return ret;
}
#endif

#if (SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_FreshnessValue_GetParsedFreshnessValue
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_GetParsedFreshnessValue(SecOC_RxPduInfoIterType rxPduId, uint8 * parsedFreshnessValue, uint8 parsedFreshnessValueBufferLength)
{
  if(parsedFreshnessValueBufferLength >= SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueLengthOfRxPduInfo(rxPduId))  /* COV_SECOC_SAFEBSW_CHECK */
  {
    SecOC_SecuredPduDataContainerTruncatedFreshnessValueEndIdxOfRxPduInfoType iterValueidx = SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueEndIdxOfRxPduInfo(rxPduId);
    SecOC_SecuredPduDataContainerTruncatedFreshnessValueEndIdxOfRxPduInfoType destValueIdx = SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueLengthOfRxPduInfo(rxPduId) - 1;
    SecOC_truncatedFreshnessValueLengthOfRxPduInfoType truncatedFreshnessValueBitsToCopy = SecOC_GettruncatedFreshnessValueLengthOfRxPduInfo(rxPduId);

    uint8 firstByteShift = SecOC_GetTruncFreshnessValueFirstByteShiftOfRxPduInfo(rxPduId);
    uint8 secondByteShift = SecOC_GetTruncFreshnessValueSecondByteShiftOfRxPduInfo(rxPduId);

    for(; iterValueidx > SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueStartIdxOfRxPduInfo(rxPduId); iterValueidx--)
    {
      SecOC_SecuredPduDataContainerTruncatedFreshnessValueEndIdxOfRxPduInfoType srcValueidx = iterValueidx - 1;
      if(truncatedFreshnessValueBitsToCopy >= 8)
      {
        uint8 byteToCopy = 0;
        byteToCopy = (uint8) (SecOC_GetSecuredPduDataContainer(srcValueidx) >> firstByteShift);

        if(secondByteShift != 0)
        {
          byteToCopy = byteToCopy | ((uint8) (SecOC_GetSecuredPduDataContainer(srcValueidx - 1) << secondByteShift));
        }

        parsedFreshnessValue[destValueIdx] = byteToCopy; /* SBSW_SECOC_PTR2FUNC_4 */
        truncatedFreshnessValueBitsToCopy = truncatedFreshnessValueBitsToCopy - 8;
      }
      else if(truncatedFreshnessValueBitsToCopy > 0) /* COV_SECOC_MISRA */
      {
        parsedFreshnessValue[destValueIdx] = (uint8) (SecOC_GetSecuredPduDataContainer(srcValueidx) >> firstByteShift); /* SBSW_SECOC_PTR2FUNC_4 */
        truncatedFreshnessValueBitsToCopy = 0;
      }
      else
      {
        /* COV_SECOC_MISRA */
      }
      destValueIdx--;
    }
  }
}
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Authenticate_GenerateAuthenticatorUsingCSM43
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_Authenticate_GenerateAuthenticatorUsingCSM43(SecOC_TxPduInfoIterType txPduId)
{
  Std_ReturnType ret = E_NOT_OK;
  uint32 resultLength = SecOC_GetAuthenticatorCacheLengthOfTxPduInfo(txPduId);
  /* *INDENT-OFF* */
  ret = Csm_MacGenerate(
      SecOC_GetAuthenticationLayerIdOfTxPduInfo(txPduId),
    CRYPTO_OPERATIONMODE_SINGLECALL,
    SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetAuthenticPduDataContainerDataToAuthenticateStartIdxOfTxPduInfo(txPduId)),
    SecOC_GetAuthenticPduDataContainerDataToAuthenticateLengthOfTxPduInfo(txPduId),
    SecOC_GetAddrAuthenticatorCache(SecOC_GetAuthenticatorCacheStartIdxOfTxPduInfo(txPduId)),
    &resultLength); /* SBSW_SECOC_PTR2FUNC_5 */
  /* *INDENT-ON* */
  if(ret == E_OK)
  {
# if (SECOC_WAITFORCALLBACKOFTXPDUINFO == STD_ON)
    if(SecOC_IsWaitForCallbackOfTxPduInfo(txPduId))
    {
      SecOC_SetTxPduState(txPduId, SECOC_UNAUTHENTICATED_FINISHED_WAIT_FOR_CALLBACK_TXPDUSTATE); /* SBSW_SECOC_CSL02 */
    }
    else
# endif
    {
      SecOC_Authenticate_CopyAuthenticatorToSecuredPdu(txPduId, SecOC_GetAddrAuthenticatorCache(SecOC_GetAuthenticatorCacheStartIdxOfTxPduInfo(txPduId))); /* SBSW_SECOC_PTR2FUNC_1 */
      SecOC_SetTxPduState(txPduId, SECOC_UNAUTHENTICATED_FINISHED_TXPDUSTATE); /* SBSW_SECOC_CSL02 */
    }
  }

  return ret;
}
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Authenticate_GenerateAuthenticator
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Authenticate_GenerateAuthenticator(SecOC_TxPduInfoIterType txPduId)
{
  Std_ReturnType ret = E_NOT_OK;
  ret = SecOC_Authenticate_GenerateAuthenticatorUsingCSM43(txPduId);
  if(ret == SECOC_E_BUSY)
  {
    SecOC_Authenticate_IncrementAndCheckBuildAttempts(txPduId);
  }
  else if(ret == E_NOT_OK)
  {
    SecOC_SetTxPduState(txPduId, SECOC_INVALID_SECURED_PDU_TXPDUSTATE); /* SBSW_SECOC_CSL02 */
    SecOC_SetTransmitRequest(txPduId, SECOC_IDLE_TRANSMITREQUEST); /* SBSW_SECOC_CSL02 */
# if (SECOC_TXSPLITPDUINFO == STD_ON)
    if(SecOC_IsTxSplitPduInfoUsedOfTxPduInfo(txPduId))
    {
      SecOC_SizeOfTxSplitPduInfoType splitTxPduId = SecOC_GetTxSplitPduInfoIdxOfTxPduInfo(txPduId);
      SecOC_SetAuthenticSplitPduTransmit(splitTxPduId, SECOC_IDLE_AUTHENTICSPLITPDUTRANSMIT); /* SBSW_SECOC_CSL80 */
      SecOC_SetCryptographicPduTransmit(splitTxPduId, SECOC_IDLE_AUTHENTICSPLITPDUTRANSMIT); /* SBSW_SECOC_CSL80 */
    }
# endif
  }
  else
  {
    /* COV_SECOC_MISRA */
  }
}
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_HandlePdu_ReauthenticationNeeded
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_TxIf_HandlePdu_ReauthenticationNeeded(SecOC_TxPduInfoIterType txPduId)
{
  if(SecOC_GetTxPduState(txPduId) == SECOC_REAUTHENTICATION_NEEDED_TXPDUSTATE)
  {
    Std_ReturnType ret = SecOC_FreshnessValue_CopyTxFreshnessValues(txPduId);
    if(ret == E_OK)
    {
      SecOC_SetTxPduState(txPduId, SECOC_UNAUTHENTICATED_TO_BE_STARTED_TXPDUSTATE); /* SBSW_SECOC_CSL02 */
    }
    else if(ret == E_NOT_OK)
    {
      SecOC_SetTxPduState(txPduId, SECOC_INVALID_SECURED_PDU_TXPDUSTATE); /* SBSW_SECOC_CSL02 */
      SecOC_SetTransmitRequest(txPduId, SECOC_IDLE_TRANSMITREQUEST); /* SBSW_SECOC_CSL02 */
# if (SECOC_TXSPLITPDUINFO == STD_ON)
      if(SecOC_IsTxSplitPduInfoUsedOfTxPduInfo(txPduId))
      {
        SecOC_SizeOfTxSplitPduInfoType splitTxPduId = SecOC_GetTxSplitPduInfoIdxOfTxPduInfo(txPduId);
        SecOC_SetAuthenticSplitPduTransmit(splitTxPduId, SECOC_IDLE_AUTHENTICSPLITPDUTRANSMIT); /* SBSW_SECOC_CSL80 */
        SecOC_SetCryptographicPduTransmit(splitTxPduId, SECOC_IDLE_AUTHENTICSPLITPDUTRANSMIT); /* SBSW_SECOC_CSL80 */
      }
# endif
    }
    else
    {
      SecOC_Authenticate_IncrementAndCheckBuildAttempts(txPduId);
    }
  }
}
#endif

#if (SECOC_TXSPLITPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_HandlePdu_SplitPdu
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_TxIf_HandlePdu_SplitPdu(SecOC_TxPduInfoIterType txPduId)
{
  SecOC_SizeOfTxSplitPduInfoType splitTxPduId = SecOC_GetTxSplitPduInfoIdxOfTxPduInfo(txPduId);
  Std_ReturnType retVal;
  Std_ReturnType retValAuthPdu;
  Std_ReturnType retValCryptoPdu;
  if(SecOC_GetAuthenticSplitPduTransmit(splitTxPduId) == SECOC_TRANSMIT_AUTHENTICSPLITPDUTRANSMIT)
  {
    PduInfoType pduInfo;
    pduInfo.SduDataPtr = SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerAuthenticSplitPduStartIdxOfTxSplitPduInfo(splitTxPduId));
    pduInfo.SduLength = (PduLengthType) SecOC_GetSecuredPduDataContainerAuthenticSplitPduLengthOfTxSplitPduInfo(splitTxPduId);
    SecOC_SetTxPduState(txPduId, SECOC_TRANSMISSION_INITIATED_TXPDUSTATE); /* SBSW_SECOC_CSL02 */
    retValAuthPdu = PduR_SecOCTransmit(SecOC_GetAuthSplitPduExternalIdOfTxSplitPduInfo(splitTxPduId), &pduInfo); /* SBSW_SECOC_PTR2FUNC_2 */
    if(retValAuthPdu == E_OK)
    {
      SecOC_SetAuthenticSplitPduTransmit(splitTxPduId, SECOC_IDLE_AUTHENTICSPLITPDUTRANSMIT); /* SBSW_SECOC_CSL80 */
    }
  }
  else
  {
    retValAuthPdu = E_OK;
  }

  if(SecOC_GetCryptographicPduTransmit(splitTxPduId) == SECOC_TRANSMIT_CRYPTOGRAPHICPDUTRANSMIT)
  {
    PduInfoType pduInfo;
    pduInfo.SduDataPtr = SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerCryptographicPduStartIdxOfTxSplitPduInfo(splitTxPduId));
    pduInfo.SduLength = (PduLengthType) SecOC_GetSecuredPduDataContainerCryptographicPduLengthOfTxSplitPduInfo(splitTxPduId);

# if(SECOC_MESSAGELINKLENOFTXSPLITPDUINFO == STD_ON)
    if(SecOC_IsSecuredPduDataContainerMessageLinkerCryptoPduUsedOfTxSplitPduInfo(splitTxPduId))
    {
      /* *INDENT-OFF* */
      SecOC_Split_ExtractMessageLinker(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerMessageLinkerAuthSplitPduStartIdxOfTxSplitPduInfo(splitTxPduId)),
                                       SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerMessageLinkerCryptoPduStartIdxOfTxSplitPduInfo(splitTxPduId)),
                                       SecOC_GetSecuredPduDataContainerMessageLinkerCryptoPduLengthOfTxSplitPduInfo(splitTxPduId),
                                       (SecOC_GetMessageLinkPosOfTxSplitPduInfo(splitTxPduId) & 7U)); /* SBSW_SECOC_PTR2FUNC_1 */
      /* *INDENT-ON* */
    }
# endif
    SecOC_SetTxPduState(txPduId, SECOC_TRANSMISSION_INITIATED_TXPDUSTATE); /* SBSW_SECOC_CSL02 */
    retValCryptoPdu = PduR_SecOCTransmit(SecOC_GetCyptographicPduExternalIdOfTxSplitPduInfo(splitTxPduId), &pduInfo); /* SBSW_SECOC_PTR2FUNC_2 */
    if(retValCryptoPdu == E_OK)
    {
      SecOC_SetCryptographicPduTransmit(splitTxPduId, SECOC_IDLE_CRYPTOGRAPHICPDUTRANSMIT); /* SBSW_SECOC_CSL80 */
    }
  }
  else
  {
    retValCryptoPdu = E_OK;
  }

  if((retValAuthPdu == E_OK) && (retValCryptoPdu == E_OK))
  {
    retVal = E_OK;
  }
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_HandlePdus
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_TxIf_HandlePdus(void)
{
  SecOC_TxPduInfoIterType txPduId = 0;
  for(; txPduId < SecOC_GetSizeOfTxPduInfo(); txPduId++)
  {
    SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_TXSTATE();

    SecOC_TxIf_HandlePdu_ReauthenticationNeeded(txPduId);

    if(SecOC_IsTxPduStateUnauthenticated(txPduId))
    {
      /* Generates MAC and copies the truncated MAC to the Secured Pdu Buffer */
# if (SECOC_WAITFORCALLBACKOFTXPDUINFO == STD_ON)
      if(SecOC_GetTxPduState(txPduId) != SECOC_UNAUTHENTICATED_FINISHED_TXPDUSTATE)
# endif
      {
        SecOC_Authenticate_GenerateAuthenticator(txPduId);
      }
      if(SecOC_GetTxPduState(txPduId) == SECOC_UNAUTHENTICATED_FINISHED_TXPDUSTATE)
      {
        VStdMemCpyRamToRam(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerAuthenticPduStartIdxOfTxPduInfo(txPduId)), SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetAuthenticPduDataContainerAuthenticPduStartIdxOfTxPduInfo(txPduId)), SecOC_GetSecuredPduDataContainerAuthenticPduLengthOfTxPduInfo(txPduId)); /* SBSW_SECOC_MEM_CPY_2RAM */
        SecOC_SetTxPduState(txPduId, SECOC_AUTHENTICATED_TXPDUSTATE); /* SBSW_SECOC_CSL02 */
      }
    }

    if((SecOC_GetTxPduState(txPduId) == SECOC_AUTHENTICATED_TXPDUSTATE) && (SecOC_GetTransmitRequest(txPduId) == SECOC_TRANSMIT_TRANSMITREQUEST)) /* COV_SECOC_NO_TRIGGER_TX_CONFIG */
    {
      Std_ReturnType retVal;

# if (SECOC_TXSPLITPDUINFO == STD_ON)
      if(SecOC_IsTxSplitPduInfoUsedOfTxPduInfo(txPduId))
      {
        retVal = SecOC_TxIf_HandlePdu_SplitPdu(txPduId);
      }
      else
# endif
      {
        PduInfoType pduInfo;
        pduInfo.SduDataPtr = SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerSecuredPduStartIdxOfTxPduInfo(txPduId));
        pduInfo.SduLength = (PduLengthType) SecOC_GetSecuredPduDataContainerSecuredPduLengthOfTxPduInfo(txPduId);
        SecOC_SetTxPduState(txPduId, SECOC_TRANSMISSION_INITIATED_TXPDUSTATE); /* SBSW_SECOC_CSL02 */
        retVal = PduR_SecOCTransmit(SecOC_GetSecuredExternalIdOfTxPduInfo(txPduId), &pduInfo); /* SBSW_SECOC_PTR2FUNC_2 */
      }

      if(retVal == E_OK)
      {
# if (SECOC_SPDUTXCONFIRMATIONNEEDEDOFTXPDUINFO == STD_ON)
        if(SecOC_IsSPduTxConfirmationNeededOfTxPduInfo(txPduId))
        {
          SPduTxConfirmation_FuncName(SecOC_GetExternalFreshnessIdOfTxPduInfo(txPduId));
        }
# endif
        SecOC_SetTransmitRequest(txPduId, SECOC_IDLE_TRANSMITREQUEST); /* SBSW_SECOC_CSL02 */
      }
      else
      {
        SecOC_SetTxPduState(txPduId, SECOC_AUTHENTICATED_TXPDUSTATE); /* SBSW_SECOC_CSL02 */
      }
    }
    SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_TXSTATE();
  }
}  /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_PropagateVerificationStatus
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_PropagateVerificationStatus(SecOC_RxPduInfoIterType rxPduId, SecOC_VerificationResultType verificationStatus)
{
  SecOC_VerificationStatusType verificationStatusStruct;
  verificationStatusStruct.verificationStatus = verificationStatus;
  verificationStatusStruct.freshnessValueID = SecOC_GetExternalFreshnessIdOfRxPduInfo(rxPduId);
  verificationStatusStruct.secOCDataId = SecOC_GetDataIdOfRxPduInfo(rxPduId);
  {
# if (SECOC_VERIFICATIONSTATUSCALLOUT == STD_ON)
    SecOC_VerificationStatusCalloutIterType idx = 0;
    for(; idx < SecOC_GetSizeOfVerificationStatusCallout(); idx++)
    {
      SecOC_GetVerificationStatusCallout(idx) (verificationStatusStruct); /* SBSW_SECOC_VERIFY_STATUS_CALLOUT */
    }
# endif /* (SECOC_VERIFICATIONSTATUSCALLOUT == STD_ON) */
    (void) Rte_Write_VerificationStatus_verificationStatus(&verificationStatusStruct); /* SBSW_SECOC_PTR2FUNC_2 */
  }
}
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_MACUsingCSM43
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(SecOC_VerifyResultType, SECOC_CODE) SecOC_Verify_MACUsingCSM43(SecOC_RxPduInfoIterType rxPduId)
{
  Std_ReturnType retValue = E_NOT_OK;
  Crypto_VerifyResultType csmVerifyResult = CSM_E_VER_NOT_OK;
  SecOC_VerifyResultType secOCVerifyResult = SECOC_E_VER_NOT_OK;

# if(SECOC_WAITFORCALLBACKOFRXPDUINFO == STD_ON)
  if(SecOC_GetRxPduState(rxPduId) == SECOC_UNVERIFIED_VERIFICATION_FINISHED_VERIFICATION_RESULT_READY_RXPDUSTATE)
  {
    retValue = E_OK;
    csmVerifyResult = SecOC_GetVerifyResult(SecOC_GetVerifyResultIdxOfRxPduInfo(rxPduId));
    SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_VERIFICATION_FINISHED_RXPDUSTATE); /* SBSW_SECOC_CSL02 */
  }
  else
# endif
  {
    SecOC_Verify_GetByteAlignedTruncatedAuthenticator(rxPduId, SecOC_GetAddrAuthenticatorCache(SecOC_GetAuthenticatorCacheStartIdxOfRxPduInfo(rxPduId)), SecOC_GetAuthenticatorCacheLengthOfRxPduInfo(rxPduId)); /* SBSW_SECOC_PTR2FUNC_1 */
    /* *INDENT-OFF* */
    retValue = Csm_MacVerify(
        SecOC_GetAuthenticationLayerIdOfRxPduInfo(rxPduId),
        CRYPTO_OPERATIONMODE_SINGLECALL,
        SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetAuthenticPduDataContainerDataToAuthenticateStartIdxOfRxPduInfo(rxPduId)),
        SecOC_GetAuthenticPduDataContainerDataToAuthenticateLengthOfRxPduInfo(rxPduId),
        SecOC_GetAddrAuthenticatorCache(SecOC_GetAuthenticatorCacheStartIdxOfRxPduInfo(rxPduId)),
        SecOC_GettruncatedAuthenticationLengthOfRxPduInfo(rxPduId),
        SecOC_GetAddrVerifyResult(SecOC_GetVerifyResultIdxOfRxPduInfo(rxPduId))); /* SBSW_SECOC_PTR2FUNC_1 */
    /* *INDENT-ON* */
    if(retValue == E_OK)
    {
# if(SECOC_WAITFORCALLBACKOFRXPDUINFO == STD_ON)
      if(SecOC_IsWaitForCallbackOfRxPduInfo(rxPduId))
      {
        SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_VERIFICATION_FINISHED_WAIT_FOR_CALLBACK_RXPDUSTATE); /* SBSW_SECOC_CSL02 */
      }
      else
# endif
      {
        csmVerifyResult = SecOC_GetVerifyResult(SecOC_GetVerifyResultIdxOfRxPduInfo(rxPduId));
        SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_VERIFICATION_FINISHED_RXPDUSTATE); /* SBSW_SECOC_CSL02 */
      }
    }
  }

  if(retValue == E_NOT_OK)
  {
    /* CSM returned none recoverable return value */
    SecOC_SetRxPduState(rxPduId, SECOC_IDLE_RXPDUSTATE); /* SBSW_SECOC_CSL02 */
  }
  else if(retValue != E_OK)
  {
    /* CSM returned recoverable return value */
    SecOC_SetRxPduState(rxPduId, SECOC_FRESHNESS_VALUE_NEEDED_RXPDUSTATE); /* SBSW_SECOC_CSL02 */
    SecOC_Verify_IncrementBuildAttempts(rxPduId);
  }
  else
  {
    /* CSM API call was successful */
  }

  if(csmVerifyResult == CRYPTO_E_VER_OK)
  {
    secOCVerifyResult = SECOC_E_VER_OK;
  }

  return secOCVerifyResult;
}
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_GetByteAlignedTruncatedAuthenticator
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_GetByteAlignedTruncatedAuthenticator(SecOC_RxPduInfoIterType rxPduId, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) byteAlignedAuthenticator, SecOC_AuthenticatorCacheLengthOfRxPduInfoType byteAlignedAuthenticatorLength)
{
  if(byteAlignedAuthenticatorLength >= SecOC_GetSecuredPduDataContainerTruncatedAuthenticatorLengthOfRxPduInfo(rxPduId)) /* COV_SECOC_SAFEBSW_CHECK */
  {
    SecOC_truncatedAuthenticationLengthOfRxPduInfoType bitsToCopy = SecOC_GettruncatedAuthenticationLengthOfRxPduInfo(rxPduId);
    SecOC_SecuredPduDataContainerTruncatedAuthenticatorEndIdxOfRxPduInfoType sourceIdx = SecOC_GetSecuredPduDataContainerTruncatedAuthenticatorStartIdxOfRxPduInfo(rxPduId);
    uint8 msbBitOffset = SecOC_GetTruncatedAuthenticatorShiftOffestForMsbOfRxPduInfo(rxPduId);
    uint8 lsbBitOffset = SecOC_GetTruncatedAuthenticatorShiftOffestForLsbOfRxPduInfo(rxPduId);
    uint8 destIdx = 0;

    for(; sourceIdx < SecOC_GetSecuredPduDataContainerTruncatedAuthenticatorEndIdxOfRxPduInfo(rxPduId); sourceIdx++)
    {
      if(bitsToCopy >= 8) /* COV_SECOC_NO_TRUNCATED_FRESHNESS_VALUE */
      {
        uint8 byteToWrite = 0;
        uint8 secondByte = 0;
        byteToWrite = SecOC_GetSecuredPduDataContainer(sourceIdx);
        secondByte = SecOC_GetSecuredPduDataContainer(sourceIdx + 1);

        if(msbBitOffset != 0) /* COV_SECOC_NO_TRUNCATED_FRESHNESS_VALUE */
        {
          byteToWrite = (uint8) (byteToWrite << msbBitOffset);
          secondByte = (uint8) (secondByte >> lsbBitOffset);

          byteToWrite = byteToWrite | secondByte;
        }
        byteAlignedAuthenticator[destIdx] = byteToWrite; /* SBSW_SECOC_PTR2FUNC_4 */

        bitsToCopy = bitsToCopy - 8;
      }
      else if(bitsToCopy != 0) /* COV_SECOC_MISRA */
      {
        uint8 byteToWrite = 0;
        byteToWrite = SecOC_GetSecuredPduDataContainer(sourceIdx);
        byteToWrite = (uint8) (byteToWrite << msbBitOffset);
        byteAlignedAuthenticator[destIdx] = byteToWrite; /* SBSW_SECOC_PTR2FUNC_4 */
        bitsToCopy = 0;
      }
      else
      {
        /* do nothing */
      }
      destIdx++;
    }
  }
}

#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_GetVerificationResultOfOverrideStatus
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(SecOC_VerifyResultType, SECOC_CODE) SecOC_Verify_GetVerificationResultOfOverrideStatus(SecOC_RxPduInfoIterType rxPduId)
{

  SecOC_VerifyResultType verificationResult = SECOC_E_VER_NOT_OK;
  SecOC_VerifyOverrideStatusType verifyOverrideStatus = SecOC_GetVerifyOverrideStatus(SecOC_GetVerifyOverrideStatusIdxOfRxPduInfo(rxPduId));

  switch (verifyOverrideStatus)
  {
# if (SECOC_ENABLE_FORCED_PASS_OVERRIDESTATUS == STD_ON)
    case SECOC_PASS_VERIFYOVERRIDESTATUS:
      verificationResult = SECOC_E_VER_OK;
      break;
    case SECOC_PASS_N_TIMES_VERIFYOVERRIDESTATUS:
      verificationResult = SECOC_E_VER_OK;
      SecOC_DecNumberOfMessagesToOverride(SecOC_GetNumberOfMessagesToOverrideIdxOfRxPduInfo(rxPduId)); /* SBSW_SECOC_CSL03 */
      if(SecOC_GetNumberOfMessagesToOverride(SecOC_GetNumberOfMessagesToOverrideIdxOfRxPduInfo(rxPduId)) == 0)
      {
        SecOC_SetVerifyOverrideStatus(SecOC_GetVerifyOverrideStatusIdxOfRxPduInfo(rxPduId), SECOC_CANCEL_VERIFYOVERRIDESTATUS); /* SBSW_SECOC_CSL03 */
      }
      break;
# endif
    case SECOC_FAIL_N_TIMES_VERIFYOVERRIDESTATUS:
      SecOC_DecNumberOfMessagesToOverride(SecOC_GetNumberOfMessagesToOverrideIdxOfRxPduInfo(rxPduId)); /* SBSW_SECOC_CSL03 */
      if(SecOC_GetNumberOfMessagesToOverride(SecOC_GetNumberOfMessagesToOverrideIdxOfRxPduInfo(rxPduId)) == 0)
      {
        SecOC_SetVerifyOverrideStatus(SecOC_GetVerifyOverrideStatusIdxOfRxPduInfo(rxPduId), SECOC_CANCEL_VERIFYOVERRIDESTATUS); /* SBSW_SECOC_CSL03 */
      }
      break;
    default:   /* COV_SECOC_MISRA */
      break;
  }

  SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_VERIFICATION_FINISHED_RXPDUSTATE); /* SBSW_SECOC_CSL02 */
  return verificationResult;
}
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_ProcessVerificationResult
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_ProcessVerificationResult(SecOC_VerifyResultType verificationResult, SecOC_RxPduInfoIterType rxPduId)
{
  if(verificationResult == SECOC_E_VER_OK) /* COV_SECOC_IGNORE_VERIFICATION_RESULT_CONFIG */
  {
    /* --- Hand the Authentic Pdu to the Upper Layer --- */
    PduInfoType pduInfo;
    pduInfo.SduDataPtr = SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetAuthenticPduDataContainerAuthenticPduStartIdxOfRxPduInfo(rxPduId));
    pduInfo.SduLength = (PduLengthType) SecOC_GetAuthenticPduDataContainerAuthenticPduLengthOfRxPduInfo(rxPduId);
    SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
# if (SECOC_EXISTS_BOTH_VERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON)
    if(SecOC_GetVerificationStatusPropagationModeOfRxPduInfo(rxPduId) == SECOC_BOTH_VERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO) /* COV_SECOC_VERIFICATION_STATUS_PROPAGATION_BOTH_ONLY_CONFIG */
    {
      SecOC_Verify_PropagateVerificationStatus(rxPduId, SECOC_VERIFICATIONSUCCESS);
    }
# endif
    PduR_SecOCRxIndication(SecOC_GetAuthenticExternalIdOfRxPduInfo(rxPduId), &pduInfo); /* SBSW_SECOC_PTR2FUNC_2 */
    SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
# if (SECOC_EXISTS_AUTHENTIC_SPLIT_MESSAGETYPEOFRXSECUREDPDUINFO == STD_ON)
    if(SecOC_IsRxSplitMessageCounterUsedOfRxPduInfo(rxPduId)) /* COV_SECOC_RX_SPLIT_ONLY_CONFIG */
    {
      SecOC_SetRxSplitMessageCounter(SecOC_GetRxSplitMessageCounterIdxOfRxPduInfo(rxPduId), 0); /* SBSW_SECOC_CSL03 */
    }
# endif
    SecOC_SetRxPduState(rxPduId, SECOC_VERIFIED_RXPDUSTATE); /* SBSW_SECOC_CSL02 */
  }
  else
  {
    SecOC_IncVerifyAttemptsCounter(rxPduId); /* SBSW_SECOC_CSL80 */
# if ((SECOC_EXISTS_BOTH_VERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON) || (SECOC_EXISTS_FAILURE_ONLY_VERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON))
#  if(SECOC_EXISTS_NONE_VERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON)
    if(SecOC_GetVerificationStatusPropagationModeOfRxPduInfo(rxPduId) != SECOC_NONE_VERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO)
#  endif
    {
#  if(SECOC_PROPAGATE_ONLY_FINAL_VERIFICATION_STATUS == STD_ON)
      if(SecOC_GetVerifyAttemptsCounter(rxPduId) >= SecOC_GetVerifyAttemptsOfRxPduInfo(rxPduId))
#  endif
      {
        SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
        SecOC_Verify_PropagateVerificationStatus(rxPduId, SECOC_VERIFICATIONFAILURE);
        SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
      }
    }
# endif

    if(SecOC_GetVerifyAttemptsCounter(rxPduId) < SecOC_GetVerifyAttemptsOfRxPduInfo(rxPduId)) /* COV_SECOC_IGNORE_VERIFICATION_RESULT_CONFIG */
    {
      SecOC_FreshnessValue_CalculateVerifyValueFromCallout(rxPduId);
      SecOC_SetRxBuildAttemptsCounter(rxPduId, 0); /* SBSW_SECOC_CSL02 */
      SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_VERIFICATION_TO_BE_STARTED_RXPDUSTATE); /* SBSW_SECOC_CSL02 */
    }
    else
    {
# if (SECOC_DEVMODEENABLED == STD_ON)
      if(SecOC_IsDevModeEnabled())
      {
        PduInfoType pduInfo;
        pduInfo.SduDataPtr = SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetAuthenticPduDataContainerAuthenticPduStartIdxOfRxPduInfo(rxPduId));
        pduInfo.SduLength = (PduLengthType) SecOC_GetAuthenticPduDataContainerAuthenticPduLengthOfRxPduInfo(rxPduId);
        SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
        PduR_SecOCRxIndication(SecOC_GetAuthenticExternalIdOfRxPduInfo(rxPduId), &pduInfo); /* SBSW_SECOC_PTR2FUNC_2 */
        SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
      }
# endif
      SecOC_SetRxPduState(rxPduId, SECOC_IDLE_RXPDUSTATE); /* SBSW_SECOC_CSL02 */
    }
  }
}
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_MAC
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_MAC(SecOC_RxPduInfoIterType rxPduId)
{
  if(SecOC_GetRxPduState(rxPduId) == SECOC_COPY_AUTHENTIC_PDU_TO_DATA_TO_AUTHENTICATE_RXPDUSTATE)
  {
    VStdMemCpyRamToRam(SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetAuthenticPduDataContainerAuthenticPduStartIdxOfRxPduInfo(rxPduId)), SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerAuthenticPduStartIdxOfRxPduInfo(rxPduId)), SecOC_GetAuthenticPduDataContainerAuthenticPduLengthOfRxPduInfo(rxPduId)); /* SBSW_SECOC_MEM_CPY_2RAM */
    SecOC_SetRxPduState(rxPduId, SECOC_FRESHNESS_VALUE_NEEDED_RXPDUSTATE); /* SBSW_SECOC_CSL02 */
  }

  if(SecOC_GetRxPduState(rxPduId) == SECOC_FRESHNESS_VALUE_NEEDED_RXPDUSTATE) /* COV_SECOC_SYNC_CSM_ONLY_CONFIG */
  {
    /* --- calculate FreshnessVerifyValue --- */
    SecOC_FreshnessValue_CalculateVerifyValueFromCallout(rxPduId);
  }

  /* --- calculate MAC and verify the Secured Pdu --- */

  /* calculate the MAC with the referenced key and Crypto Library and compare the calculated MAC with the received MAC of the Secured PDU. */
  if(SecOC_IsRxPduStateUnverified(rxPduId))
  {
    SecOC_VerifyResultType verificationResult = SECOC_E_VER_NOT_OK;

    if(SecOC_GetVerifyOverrideStatus(SecOC_GetVerifyOverrideStatusIdxOfRxPduInfo(rxPduId)) == SECOC_CANCEL_VERIFYOVERRIDESTATUS)
    {
      verificationResult = SecOC_Verify_MACUsingCSM43(rxPduId);
    }
    else
    {
      verificationResult = SecOC_Verify_GetVerificationResultOfOverrideStatus(rxPduId);
    }

    if(SecOC_GetRxPduState(rxPduId) == SECOC_UNVERIFIED_VERIFICATION_FINISHED_RXPDUSTATE)
    {
# if(SECOC_IGNORE_VERIFICATION_RESULT == STD_ON)
      /* \trace SPEC-2969140, SPEC-2969077 */
      verificationResult = SECOC_E_VER_OK;
# endif
      SecOC_Verify_ProcessVerificationResult(verificationResult, rxPduId);
    }

    if(SecOC_GetRxPduState(rxPduId) == SECOC_VERIFIED_RXPDUSTATE)
    {
      SecOC_SetRxPduState(rxPduId, SECOC_IDLE_RXPDUSTATE); /* SBSW_SECOC_CSL02 */
    }
  }
}  /* PRQA S 6050, 6010 */ /* MD_MSR_STCAL, MD_MSR_STPTH */
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_RxIf_HandlePdus
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_RxIf_HandlePdus(void)
{
  /* Verify the received Secured I-Pdus and hand the Authentic I-Pdus to the UpperLayer. */
  SecOC_RxPduInfoIterType rxPduId = 0;
  for(; rxPduId < SecOC_GetSizeOfRxPduInfo(); rxPduId++)
  {
    if(SecOC_GetRxPduState(rxPduId) != SECOC_IDLE_RXPDUSTATE)
    {
      SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
      SecOC_Verify_MAC(rxPduId);
      SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
    }
  }
}
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_FreshnessValue_GetVerifyValueFromCallout
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_FreshnessValue_GetVerifyValueFromCallout(SecOC_RxPduInfoIterType rxPduId, uint8 * freshnessValue, uint8 freshnessValueBufferLength)
{
  Std_ReturnType ret = E_NOT_OK;
  uint32 freshnessValueLength = (uint32) (SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueLengthOfRxPduInfo(rxPduId) << 3U);
  if(freshnessValueLength <= (uint32) (freshnessValueBufferLength << 3U)) /* COV_SECOC_SAFEBSW_CHECK */
  {
# if ((SECOC_USE_GET_RX_FRESHNESS_AUTH_DATA_API == STD_ON) && (SECOC_USE_GET_RX_FRESHNESS_API == STD_ON))
    if(SecOC_GetAuthDataFreshnessOfRxPduInfo(rxPduId) == SECOC_USE_AUTHDATAFRESHNESSOFRXPDUINFO)
# endif
# if (SECOC_USE_GET_RX_FRESHNESS_AUTH_DATA_API == STD_ON)
    {
      uint8 truncatedFreshnessValue[SECOC_TRUNC_FRESHNESS_SIZE_RXPDUS] = { 0 };

#  if (SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFRXPDUINFO == STD_ON)
      SecOC_FreshnessValue_GetParsedFreshnessValue(rxPduId, truncatedFreshnessValue, SECOC_TRUNC_FRESHNESS_SIZE_RXPDUS); /* SBSW_SECOC_PTR2FUNC_2 */
#  endif
      /* *INDENT-OFF* */
      ret = GetRxFreshnessAuthData_FuncName(
          SecOC_GetExternalFreshnessIdOfRxPduInfo(rxPduId),
          truncatedFreshnessValue,
          SecOC_GettruncatedFreshnessValueLengthOfRxPduInfo(rxPduId),
          SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerAuthenticPduStartIdxOfRxPduInfo(rxPduId) + SecOC_GetAuthDataFreshnessStartBytePositionOfRxPduInfo(rxPduId)),
          SecOC_GetAuthDataFreshnessLenOfRxPduInfo(rxPduId),
          (uint16) SecOC_GetVerifyAttemptsCounter(rxPduId),
          freshnessValue,
          &freshnessValueLength
      ); /* SBSW_SECOC_PTR2FUNC_8 */
      /* *INDENT-ON* */
    }
# endif
# if ((SECOC_USE_GET_RX_FRESHNESS_AUTH_DATA_API == STD_ON) && (SECOC_USE_GET_RX_FRESHNESS_API == STD_ON))
    else
# endif
# if (SECOC_USE_GET_RX_FRESHNESS_API == STD_ON)
    {
      uint8 truncatedFreshnessValue[SECOC_TRUNC_FRESHNESS_SIZE_RXPDUS] = { 0 };

#  if (SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFRXPDUINFO == STD_ON)
      SecOC_FreshnessValue_GetParsedFreshnessValue(rxPduId, truncatedFreshnessValue, SECOC_TRUNC_FRESHNESS_SIZE_RXPDUS); /* SBSW_SECOC_PTR2FUNC_2 */
#  endif
      /* *INDENT-OFF* */
      ret = GetRxFreshness_FuncName(
          SecOC_GetExternalFreshnessIdOfRxPduInfo(rxPduId),
          truncatedFreshnessValue,
          SecOC_GettruncatedFreshnessValueLengthOfRxPduInfo(rxPduId),
          (uint16) SecOC_GetVerifyAttemptsCounter(rxPduId),
          freshnessValue,
          &freshnessValueLength
      ); /* SBSW_SECOC_PTR2FUNC_6 */
      /* *INDENT-ON* */
    }
# endif
  }
  return ret;
}
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_RxIf_InitPdus
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_RxIf_InitPdus(void)
{
  SecOC_RxPduInfoIterType rxPduId = 0;
  for(; rxPduId < SecOC_GetSizeOfRxPduInfo(); rxPduId++)
  {
    SecOC_SetAuthenticPduDataContainer(SecOC_GetAuthenticPduDataContainerDataIdFirstByteIdxOfRxPduInfo(rxPduId), SecOC_GetDataIdFirstByteOfRxPduInfo(rxPduId)); /* SBSW_SECOC_CSL02 */
    SecOC_SetAuthenticPduDataContainer(SecOC_GetAuthenticPduDataContainerDataIdSecondByteIdxOfRxPduInfo(rxPduId), SecOC_GetDataIdSecondByteOfRxPduInfo(rxPduId)); /* SBSW_SECOC_CSL02 */
    SecOC_SetVerifyAttemptsCounter(rxPduId, 0); /* SBSW_SECOC_CSL02 */
    SecOC_SetRxBuildAttemptsCounter(rxPduId, 0); /* SBSW_SECOC_CSL02 */
    SecOC_SetRxTpWrittenBytesCounter(rxPduId, 0); /* SBSW_SECOC_CSL02 */
    SecOC_SetVerifyOverrideStatus(SecOC_GetVerifyOverrideStatusIdxOfRxPduInfo(rxPduId), SECOC_CANCEL_VERIFYOVERRIDESTATUS);  /* SBSW_SECOC_CSL02 */
  }
# if (SECOC_EXISTS_AUTHENTIC_SPLIT_MESSAGETYPEOFRXSECUREDPDUINFO == STD_ON)
  VStdMemClr(SecOC_GetAddrRxSplitMessageCounter(0), SecOC_GetSizeOfRxSplitMessageCounter()); /* SBSW_SECOC_CSL01 */
# endif
}
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_InitPdus
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_TxIf_InitPdus(void)
{
  SecOC_TxPduInfoIterType txPduId = 0;
  for(; txPduId < SecOC_GetSizeOfTxPduInfo(); txPduId++)
  {
    SecOC_SetAuthenticPduDataContainer(SecOC_GetAuthenticPduDataContainerDataIdFirstByteIdxOfTxPduInfo(txPduId), SecOC_GetDataIdFirstByteOfTxPduInfo(txPduId)); /* SBSW_SECOC_CSL02 */
    SecOC_SetAuthenticPduDataContainer(SecOC_GetAuthenticPduDataContainerDataIdSecondByteIdxOfTxPduInfo(txPduId), SecOC_GetDataIdSecondByteOfTxPduInfo(txPduId)); /* SBSW_SECOC_CSL02 */
    SecOC_SetTxBuildAttemptsCounter(txPduId, 0); /* SBSW_SECOC_CSL02 */
    SecOC_SetTxTpWrittenBytesCounter(txPduId, 0); /* SBSW_SECOC_CSL02 */
  }
}
#endif

#if ((SECOC_CURRENTMESSAGELINKER == STD_ON) || (SECOC_MESSAGELINKLENOFTXSPLITPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_Split_ExtractMessageLinker
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Split_ExtractMessageLinker(P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) msgLinkSrc, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) msgLinkDest, uint16 msgLinkLen, uint16 msgLinkBitInByteOffset)
{
  uint16_least i = 0;

  for(; i < msgLinkLen; i++)

  {
    uint8 byteToCopy = 0;
    if(msgLinkBitInByteOffset != 0)
    {
      uint8 numLeftShift = (uint8) (8 - msgLinkBitInByteOffset);
      uint8 numRightShift = (uint8) msgLinkBitInByteOffset;
      byteToCopy = (uint8) (msgLinkSrc[i] >> numRightShift);
      byteToCopy = byteToCopy | ((uint8) (msgLinkSrc[i + 1] << numLeftShift));
    }
    else
    {
      byteToCopy = msgLinkSrc[i];
    }

    msgLinkDest[i] = byteToCopy; /* SBSW_SECOC_PTR2FUNC_4 */
  }
}
#endif

#if (SECOC_CURRENTMESSAGELINKER == STD_ON)
/**********************************************************************************************************************
  SecOC_Split_CheckMessageLinker
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean, SECOC_CODE) SecOC_Split_CheckMessageLinker(P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) msgLinkNew, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) msgLinkOld, SecOC_MessageLinkLenOfRxSecuredPduInfoType msgLinkLenInBytes, boolean currentIsNewStatus)
{

  boolean msgLinkerState = TRUE;
  uint16_least i = 0;

  for(; i < msgLinkLenInBytes; i++)
  {
    if(msgLinkNew[i] != msgLinkOld[i])
    {
      msgLinkerState = FALSE;
      break;
    }
  }

  if(msgLinkerState == FALSE)
  {
    currentIsNewStatus = FALSE;
    VStdMemCpyRamToRam(msgLinkOld, msgLinkNew, msgLinkLenInBytes); /* SBSW_SECOC_PTR2FUNC_3 */
  }

  return currentIsNewStatus;
}
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Authenticate_IncrementAndCheckBuildAttempts
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_Authenticate_IncrementAndCheckBuildAttempts(SecOC_TxPduInfoIterType txPduId)
{
  SecOC_IncTxBuildAttemptsCounter(txPduId); /* SBSW_SECOC_CSL02 */
  if(SecOC_GetTxBuildAttemptsCounter(txPduId) >= SecOC_GetBuildAttemptsOfTxPduInfo(txPduId))
  {
    SecOC_SetTxPduState(txPduId, SECOC_INVALID_SECURED_PDU_TXPDUSTATE); /* SBSW_SECOC_CSL02 */
    SecOC_SetTransmitRequest(txPduId, SECOC_IDLE_TRANSMITREQUEST); /* SBSW_SECOC_CSL02 */
# if (SECOC_TXSPLITPDUINFO == STD_ON)
    if(SecOC_IsTxSplitPduInfoUsedOfTxPduInfo(txPduId))
    {
      SecOC_SizeOfTxSplitPduInfoType splitTxPduId = SecOC_GetTxSplitPduInfoIdxOfTxPduInfo(txPduId);
      SecOC_SetAuthenticSplitPduTransmit(splitTxPduId, SECOC_IDLE_AUTHENTICSPLITPDUTRANSMIT); /* SBSW_SECOC_CSL03 */
      SecOC_SetCryptographicPduTransmit(splitTxPduId, SECOC_IDLE_AUTHENTICSPLITPDUTRANSMIT); /* SBSW_SECOC_CSL03 */
    }
# endif
    SecOC_ReportRuntimeDetError(SECOC_SID_MAIN_FUNCTION_TX, SECOC_E_CRYPTO_FAILURE);
  }
}
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_IncrementBuildAttempts
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_Verify_IncrementBuildAttempts(SecOC_RxPduInfoIterType rxPduId)
{
  SecOC_IncRxBuildAttemptsCounter(rxPduId); /* SBSW_SECOC_CSL02 */
  if(SecOC_GetRxBuildAttemptsCounter(rxPduId) >= SecOC_GetBuildAttemptsOfRxPduInfo(rxPduId))
  {
    SecOC_SetRxPduState(rxPduId, SECOC_IDLE_RXPDUSTATE); /* SBSW_SECOC_CSL02 */
  }
}
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Authenticate_CopyAuthenticatorToSecuredPdu
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_Authenticate_CopyAuthenticatorToSecuredPdu(SecOC_TxPduInfoIterType txPduId, P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) authenticatorArray)
{
  SecOC_truncatedAuthenticationLengthOfTxPduInfoType bitsToCopy = SecOC_GettruncatedAuthenticationLengthOfTxPduInfo(txPduId);
  SecOC_SecuredPduDataContainerTruncatedAuthenticatorEndIdxOfTxPduInfoType destIdx = SecOC_GetSecuredPduDataContainerTruncatedAuthenticatorStartIdxOfTxPduInfo(txPduId);
  uint8 msbBitOffset = SecOC_GetTruncatedAuthenticatorShiftOffestForMsbOfTxPduInfo(txPduId);
  uint8 lsbBitOffset = SecOC_GetTruncatedAuthenticatorShiftOffestForLsbOfTxPduInfo(txPduId);
  uint8 srcIdx = 0;

  for(; destIdx < SecOC_GetSecuredPduDataContainerTruncatedAuthenticatorEndIdxOfTxPduInfo(txPduId); destIdx++)
  {
    if((bitsToCopy & 7U) != 0) /* COV_SECOC_NO_TRUNCATED_FRESHNESS_VALUE */
    {
      uint8 byteToWrite = 0;
      uint8 copyMask = (uint8) (255U << lsbBitOffset);
      byteToWrite = (uint8) (authenticatorArray[srcIdx] >> msbBitOffset);
      byteToWrite = (SecOC_GetSecuredPduDataContainer(destIdx) & copyMask) | byteToWrite;
      SecOC_SetSecuredPduDataContainer(destIdx, byteToWrite); /* SBSW_SECOC_CSL03 */

      bitsToCopy = bitsToCopy - (bitsToCopy & 7U);
    }
    else
    {
      if(msbBitOffset != 0) /* COV_SECOC_NO_TRUNCATED_FRESHNESS_VALUE */
      {
        uint8 byteToWrite = 0;
        byteToWrite = (uint8) (authenticatorArray[srcIdx] >> msbBitOffset);
        byteToWrite = byteToWrite | ((uint8) (authenticatorArray[srcIdx - 1] << lsbBitOffset));
        SecOC_SetSecuredPduDataContainer(destIdx, byteToWrite); /* SBSW_SECOC_CSL03 */
      }
      else
      {
        SecOC_SetSecuredPduDataContainer(destIdx, authenticatorArray[srcIdx]); /* SBSW_SECOC_CSL03 */
      }
      bitsToCopy = bitsToCopy - 8;
    }
    srcIdx++;
  }
}
#endif

/**********************************************************************************************************************
  SecOC_InitMemory
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  SecOC_SetInitialized(FALSE);
#if(SECOC_USE_INIT_POINTER == STD_ON)
  SecOC_ConfigDataPtr = NULL_PTR;
#endif
}

/**********************************************************************************************************************
  SecOC_DeInit
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_DeInit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check if component is initialized */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    SecOC_SetInitialized(FALSE);
#if(SECOC_USE_INIT_POINTER == STD_ON)
    SecOC_ConfigDataPtr = NULL_PTR;
#endif
  }

#if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)
  {
    SecOC_ReportDetError(SECOC_SID_DE_INIT, errorId);
  }
#endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}

/**********************************************************************************************************************
  SecOC_Init
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_Init(P2CONST(SecOC_ConfigType, AUTOMATIC, SECOC_PBCFG) config)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;
#if(SECOC_USE_INIT_POINTER == STD_ON)
  SecOC_ConfigDataPtr = config;
#endif

  /* ----- Development Error Checks ------------------------------------- */
  if(SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
  /* --- parameter check --- */
#if(SECOC_USE_INIT_POINTER == STD_ON)
  /* Check validtity of Config pointer */
  else if(SecOC_ConfigDataPtr == NULL_PTR)
  {
# if(SECOC_USE_ECUM_BSW_ERROR_HOOK == STD_ON) /* COV_SECOC_VAR_DEPENDENT_SWITCH */
    EcuM_BswErrorHook(SECOC_MODULE_ID, ECUM_BSWERROR_NULLPTR);
# endif
    errorId = SECOC_E_PARAM_POINTER;
  }
# if (SECOC_FINALMAGICNUMBER == STD_ON) /* COV_SECOC_VAR_DEPENDENT_SWITCH */
  else if(SecOC_GetFinalMagicNumber() != SECOC_FINAL_MAGIC_NUMBER)
  {
# if(SECOC_USE_ECUM_BSW_ERROR_HOOK == STD_ON) /* COV_SECOC_VAR_DEPENDENT_SWITCH */
    EcuM_BswErrorHook(SECOC_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
# endif
    errorId = SECOC_E_PARAM_POINTER;
    SecOC_ConfigDataPtr = NULL_PTR;
  }
# endif
#else
  if(config != NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
#endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#if (SECOC_RXPDUINFO == STD_ON)
    SecOC_RxIf_InitPdus();
#endif
#if(SECOC_TXPDUINFO == STD_ON)
    SecOC_TxIf_InitPdus();
#endif
#if (SECOC_RXPDUSTATE == STD_ON)
    {
      SecOC_RxPduStateIterType pduStateIdx = 0;
      for(; pduStateIdx < SecOC_GetSizeOfRxPduState(); pduStateIdx++)
      {
        SecOC_SetRxPduState(pduStateIdx, SECOC_IDLE_RXPDUSTATE); /* SBSW_SECOC_CSL01 */
      }
    }
#endif
#if (SECOC_TXPDUSTATE == STD_ON)
    {
      SecOC_TxPduStateIterType pduStateIdx = 0;
      for(; pduStateIdx < SecOC_GetSizeOfTxPduState(); pduStateIdx++)
      {
        SecOC_SetTxPduState(pduStateIdx, SECOC_INVALID_SECURED_PDU_TXPDUSTATE); /* SBSW_SECOC_CSL01 */
      }
    }
#endif
#if (SECOC_VERIFYRESULT == STD_ON)
    {
      SecOC_VerifyResultIterType verifyResultIdx = 0;
      for(; verifyResultIdx < SecOC_GetSizeOfVerifyResult(); verifyResultIdx++)
      {
        SecOC_SetVerifyResult(verifyResultIdx, CSM_E_VER_NOT_OK); /* SBSW_SECOC_CSL01 */
      }
    }
#endif
#if(SECOC_DEVMODEENABLED == STD_ON)
    SecOC_SetDevModeEnabled(FALSE);
#endif
    SecOC_SetInitialized(TRUE);
  }

#if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)
  {
    SecOC_ReportDetError(SECOC_SID_INIT, errorId);
  }
#endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if (SECOC_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
  SecOC_GetVersionInfo
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, SECOC_APPL_DATA) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if(SECOC_DEV_ERROR_DETECT == STD_ON) /* COV_SECOC_VAR_DEV_ERROR_DETECT */
  if(versioninfo == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID = (SECOC_VENDOR_ID); /* SBSW_SECOC_PTR2FUNC_3 */
    versioninfo->moduleID = (SECOC_MODULE_ID); /* SBSW_SECOC_PTR2FUNC_3 */
    versioninfo->sw_major_version = (SECOC_SW_MAJOR_VERSION); /* SBSW_SECOC_PTR2FUNC_3 */
    versioninfo->sw_minor_version = (SECOC_SW_MINOR_VERSION); /* SBSW_SECOC_PTR2FUNC_3 */
    versioninfo->sw_patch_version = (SECOC_SW_PATCH_VERSION); /* SBSW_SECOC_PTR2FUNC_3 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)
  {
    SecOC_ReportDetError(SECOC_SID_GET_VERSION_INFO, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

/**********************************************************************************************************************
  SecOC_Transmit
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE) SecOC_Transmit(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;
  Std_ReturnType ret = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
#if(SECOC_DEV_ERROR_DETECT == STD_ON) /* COV_SECOC_VAR_DEV_ERROR_DETECT */
# if(SECOC_TXAUTHENTICPDUINFO == STD_ON)
  else if(id >= SecOC_GetSizeOfTxAuthenticPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#  if(SECOC_INVALIDHNDOFTXAUTHENTICPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfTxAuthenticPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#  endif
  else if(info == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else if(info->SduDataPtr == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
# endif
# if(SECOC_TXAUTHENTICPDUINFO == STD_ON)
  else if(info->SduLength != SecOC_GetAuthenticPduDataContainerAuthenticPduLengthOfTxPduInfo(SecOC_GetTxPduInfoIdxOfTxAuthenticPduInfo(id)))
  {
    errorId = SECOC_E_PARAM;
  }
# endif
#endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#if(SECOC_TXAUTHENTICPDUINFO == STD_ON)
    SecOC_TxPduInfoIterType txPduInfoIdx = SecOC_GetTxPduInfoIdxOfTxAuthenticPduInfo(id);
    SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_TXSTATE();
    /* --- Reevaluate the Pdu state --- */
    if((SecOC_GetTxPduState(txPduInfoIdx) == SECOC_REAUTHENTICATION_NEEDED_TXPDUSTATE) || (SecOC_GetTxPduState(txPduInfoIdx) == SECOC_TRANSMISSION_INITIATED_TXPDUSTATE) || (SecOC_GetTxPduState(txPduInfoIdx) == SECOC_TRIGGER_TRANSMIT_TXPDUSTATE) || (SecOC_GetTxPduState(txPduInfoIdx) == SECOC_AUTHENTICATED_TXPDUSTATE)) /* COV_SECOC_NO_TRIGGER_TX_CONFIG */
    {
      SecOC_SetTxPduState(txPduInfoIdx, SECOC_INVALID_SECURED_PDU_TXPDUSTATE); /* SBSW_SECOC_CSL80 */
    }
    if(SecOC_GetTxPduState(txPduInfoIdx) == SECOC_INVALID_SECURED_PDU_TXPDUSTATE) /* COV_SECOC_SYNC_CSM_ONLY_CONFIG */
    {
      /* --- Copy the Authentic Pdu into an internal Buffer --- */
      VStdMemCpyRamToRam(SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetAuthenticPduDataContainerAuthenticPduStartIdxOfTxPduInfo(txPduInfoIdx)), info->SduDataPtr, SecOC_GetAuthenticPduDataContainerAuthenticPduLengthOfTxPduInfo(txPduInfoIdx)); /* SBSW_SECOC_MEM_CPY_2RAM */
      /* Reset Tx Sync Attempts Counter */
      SecOC_SetTxBuildAttemptsCounter(txPduInfoIdx, 0); /* SBSW_SECOC_PTR2FUNC_2 */
      /* --- Set the Tx Pdu State to "Reauthentication needed" --- */
      SecOC_SetTxPduState(txPduInfoIdx, SECOC_REAUTHENTICATION_NEEDED_TXPDUSTATE); /* SBSW_SECOC_CSL80 */
      /* Set the Transmit Request state of the corresponding Pdu to Transmit */
      /* --- Set the Transmit request --- */
      SecOC_SetTransmitRequest(txPduInfoIdx, SECOC_TRANSMIT_TRANSMITREQUEST); /* SBSW_SECOC_PTR2FUNC_2 */
# if (SECOC_TXSPLITPDUINFO == STD_ON)
      if(SecOC_IsTxSplitPduInfoUsedOfTxPduInfo(txPduInfoIdx))
      {
        SecOC_SizeOfTxSplitPduInfoType splitTxPduId = SecOC_GetTxSplitPduInfoIdxOfTxPduInfo(txPduInfoIdx);
        SecOC_SetAuthenticSplitPduTransmit(splitTxPduId, SECOC_TRANSMIT_AUTHENTICSPLITPDUTRANSMIT); /* SBSW_SECOC_CSL80 */
        SecOC_SetCryptographicPduTransmit(splitTxPduId, SECOC_TRANSMIT_AUTHENTICSPLITPDUTRANSMIT); /* SBSW_SECOC_CSL80 */
      }
# endif
      /* --- return E_OK --- */
      ret = E_OK;
    }
    SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_TXSTATE();
#else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR) /* COV_SECOC_RX_ONLY_CONFIG */
  {
    SecOC_ReportDetError(SECOC_SID_TRANSMIT, errorId);
  }
#endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  return ret;
}  /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

#if (SECOC_COMMUNICATION_INTERFACE == STD_ON) /* COV_SECOC_ALWAYS_ON */
/**********************************************************************************************************************
  SecOC_RxIndication
**********************************************************************************************************************/
/** Internal comment removed.
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
FUNC(void, SECOC_CODE) SecOC_RxIndication(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
# if(SECOC_DEV_ERROR_DETECT == STD_ON) /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#  if(SECOC_RXSECUREDPDUINFO == STD_ON)
  else if(id >= SecOC_GetSizeOfRxSecuredPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   if(SECOC_INVALIDHNDOFRXSECUREDPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfRxSecuredPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   endif
  else if(info == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else if(info->SduDataPtr == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else if(info->SduLength != SecOC_GetSecuredPduDataContainerLengthOfRxSecuredPduInfo(id))
  {
    errorId = SECOC_E_PARAM;
  }
#  endif
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
# if(SECOC_RXSECUREDPDUINFO == STD_ON)
#  if (SECOC_EXISTS_AUTHENTIC_SPLIT_MESSAGETYPEOFRXSECUREDPDUINFO == STD_ON)
    boolean isNewCompletMsg = TRUE;
#  endif
    SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
    switch (SecOC_GetMessageTypeOfRxSecuredPduInfo(id))
    {
#  if(SECOC_EXISTS_NORMAL_MESSAGETYPEOFRXSECUREDPDUINFO == STD_ON)
      case SECOC_NORMAL_MESSAGETYPEOFRXSECUREDPDUINFO:
      {
        VStdMemCpyRamToRam(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerStartIdxOfRxSecuredPduInfo(id)), info->SduDataPtr, SecOC_GetSecuredPduDataContainerLengthOfRxSecuredPduInfo(id)); /* SBSW_SECOC_MEM_CPY_2RAM */
        break;
      }
#  endif
#  if (SECOC_EXISTS_AUTHENTIC_SPLIT_MESSAGETYPEOFRXSECUREDPDUINFO == STD_ON)
      case SECOC_AUTHENTIC_SPLIT_MESSAGETYPEOFRXSECUREDPDUINFO:
      {
        SecOC_IncRxSplitMessageCounter(SecOC_GetRxSplitMessageCounterIdxOfRxSecuredPduInfo(id)); /* SBSW_SECOC_CSL03 */
        if(SecOC_GetRxSplitMessageCounter(SecOC_GetRxSplitMessageCounterIdxOfRxSecuredPduInfo(id)) < 2)
        {
          isNewCompletMsg = FALSE;
        }
#   if (SECOC_CURRENTMESSAGELINKER == STD_ON)
        {
          uint8 messageLinker[SECOC_MESSAGE_LINKER_SIZE_RXPDUS];
          SecOC_Split_ExtractMessageLinker(&(info->SduDataPtr[((PduLengthType) (SecOC_GetMessageLinkPosOfRxSecuredPduInfo(id) >> 3U))]), messageLinker, SecOC_GetMessageLinkLenOfRxSecuredPduInfo(id) >> 3U, (SecOC_GetMessageLinkPosOfRxSecuredPduInfo(id) & 7U)); /* SBSW_SECOC_PTR2FUNC_6 */
          isNewCompletMsg = SecOC_Split_CheckMessageLinker(messageLinker, SecOC_GetAddrCurrentMessageLinker(SecOC_GetCurrentMessageLinkerStartIdxOfRxSecuredPduInfo(id)), SecOC_GetCurrentMessageLinkerLengthOfRxSecuredPduInfo(id), isNewCompletMsg); /* SBSW_SECOC_PTR2FUNC_5 */
        }
#   endif
        VStdMemCpyRamToRam(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerStartIdxOfRxSecuredPduInfo(id)), info->SduDataPtr, SecOC_GetSecuredPduDataContainerLengthOfRxSecuredPduInfo(id)); /* SBSW_SECOC_MEM_CPY_2RAM */
        break;
      }
#  endif
#  if (SECOC_EXISTS_CRYPTOGRAPHIC_MESSAGETYPEOFRXSECUREDPDUINFO == STD_ON)
      case SECOC_CRYPTOGRAPHIC_MESSAGETYPEOFRXSECUREDPDUINFO:
      {
        SecOC_IncRxSplitMessageCounter(SecOC_GetRxSplitMessageCounterIdxOfRxSecuredPduInfo(id)); /* SBSW_SECOC_CSL03 */
        if(SecOC_GetRxSplitMessageCounter(SecOC_GetRxSplitMessageCounterIdxOfRxSecuredPduInfo(id)) < 2)
        {
          isNewCompletMsg = FALSE;
        }
#   if (SECOC_CURRENTMESSAGELINKER == STD_ON)
        {
          isNewCompletMsg= SecOC_Split_CheckMessageLinker(&(info->SduDataPtr[(info->SduLength - SecOC_GetCurrentMessageLinkerLengthOfRxSecuredPduInfo(id))]), SecOC_GetAddrCurrentMessageLinker(SecOC_GetCurrentMessageLinkerStartIdxOfRxSecuredPduInfo(id)), SecOC_GetCurrentMessageLinkerLengthOfRxSecuredPduInfo(id), isNewCompletMsg); /* SBSW_SECOC_PTR2FUNC_7 */
        }
#   endif
        VStdMemCpyRamToRam(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerStartIdxOfRxSecuredPduInfo(id)), info->SduDataPtr, SecOC_GetSecuredPduDataContainerLengthOfRxSecuredPduInfo(id)); /* SBSW_SECOC_MEM_CPY_2RAM */
        break;
      }
#  endif
      default: /* COV_SECOC_MISRA */
        break;
    }
#  if (SECOC_EXISTS_AUTHENTIC_SPLIT_MESSAGETYPEOFRXSECUREDPDUINFO == STD_ON)
    if(isNewCompletMsg == TRUE)
#  endif
    {
      SecOC_RxPduInfoIterType rxPduInfoIdx = SecOC_GetRxPduInfoIdxOfRxSecuredPduInfo(id);
      /* --- Set the state of the corresponding PDU to Unverified --- */
      SecOC_SetRxPduState(rxPduInfoIdx, SECOC_COPY_AUTHENTIC_PDU_TO_DATA_TO_AUTHENTICATE_RXPDUSTATE); /* SBSW_SECOC_CSL80 */
      SecOC_SetVerifyAttemptsCounter(rxPduInfoIdx, 0); /* SBSW_SECOC_CSL80 */
      SecOC_SetRxBuildAttemptsCounter(rxPduInfoIdx, 0); /* SBSW_SECOC_CSL80 */
    }
    SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
# else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR) /* COV_SECOC_TX_ONLY_CONFIG */
  {
    SecOC_ReportDetError(SECOC_SID_RX_INDICATION, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}  /* PRQA S 6080, 6030 */ /* MD_MSR_STMIF, MD_MSR_STCYC */

# if (SECOC_TX_CONFIRMATION_API == STD_ON)
/**********************************************************************************************************************
  SecOC_TxConfirmation
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_TxConfirmation(PduIdType id)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
#  if(SECOC_DEV_ERROR_DETECT == STD_ON) /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#   if(SECOC_TXSECUREDPDUINFO == STD_ON) /* COV_SECOC_VAR_DEPENDENT_SWITCH */
  else if(id >= SecOC_GetSizeOfTxSecuredPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#    if(SECOC_INVALIDHNDOFTXSECUREDPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfTxSecuredPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#    endif
#   endif
#  endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#  if(SECOC_TXSECUREDPDUINFO == STD_ON) /* COV_SECOC_VAR_DEPENDENT_SWITCH */
    SecOC_TxPduInfoIterType txPduInfoIdx = SecOC_GetTxPduInfoIdxOfTxSecuredPduInfo(id);
    /* --- Action: SecOC_TxConfirmation --- */
    /* --- Call PduR_SecOCTxConfirmation --- */
#   if( SECOC_CONFIRMATIONNEEDEDOFTXPDUINFO == STD_ON)
    if(SecOC_IsConfirmationNeededOfTxPduInfo(txPduInfoIdx))
    {
      PduR_SecOCTxConfirmation(SecOC_GetAuthenticExternalIdOfTxPduInfo(txPduInfoIdx));
    }
#   endif
    SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_TXSTATE();
    switch (SecOC_GetTxPduState(txPduInfoIdx))
    {
      case SECOC_TRANSMISSION_INITIATED_TXPDUSTATE:
        SecOC_SetTxPduState(txPduInfoIdx, SECOC_INVALID_SECURED_PDU_TXPDUSTATE); /* SBSW_SECOC_CSL80 */
        break;
      case SECOC_TRIGGER_TRANSMIT_TXPDUSTATE:
        /* Reset Tx Sync Attempts Counter */
        SecOC_SetTxBuildAttemptsCounter(txPduInfoIdx, 0); /* SBSW_SECOC_CSL80 */
        SecOC_SetTxPduState(txPduInfoIdx, SECOC_REAUTHENTICATION_NEEDED_TXPDUSTATE); /* SBSW_SECOC_CSL80 */
        break;
      default: /* COV_SECOC_MISRA */
        break;
    }
    SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_TXSTATE();
#  else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
#  endif
  }

  /* ----- Development Error Report --------------------------------------- */
#  if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)
  {
    SecOC_ReportDetError(SECOC_SID_TX_CONFIRMATION, errorId);
  }
#  endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}
# endif

# if (SECOC_TRIGGER_TRANSMIT_API == STD_ON)
/**********************************************************************************************************************
  SecOC_TriggerTransmit
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
 *

 *
**********************************************************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE) SecOC_TriggerTransmit(PduIdType id, P2VAR(PduInfoType, AUTOMATIC, SECOC_APPL_VAR) info)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;
  Std_ReturnType ret = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check if component is initialized */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
#  if(SECOC_DEV_ERROR_DETECT == STD_ON) /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#   if(SECOC_TXSECUREDPDUINFO == STD_ON) /* COV_SECOC_VAR_DEPENDENT_SWITCH */
  else if(id >= SecOC_GetSizeOfTxSecuredPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#    if(SECOC_INVALIDHNDOFTXSECUREDPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfTxSecuredPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#    endif
#   endif
  else if(info == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else if(info->SduDataPtr == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
#  endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#  if(SECOC_TXSECUREDPDUINFO == STD_ON) /* COV_SECOC_VAR_DEPENDENT_SWITCH */
    SecOC_TxPduInfoIterType txPduInfoIdx = SecOC_GetTxPduInfoIdxOfTxSecuredPduInfo(id);
    /* --- Action: SecOC_TriggerTransmit --- */
    /* --- Check state --- */
    /* return E_NOT_OK if the Pdu has not the state "Authenticated" or "transmission initiated" */
    if(info->SduLength >= SecOC_GetSecuredPduDataContainerSecuredPduLengthOfTxPduInfo(SecOC_GetTxPduInfoIdxOfTxSecuredPduInfo(id)))
    {
      SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_TXSTATE();
      if((SecOC_GetTxPduState(txPduInfoIdx) == SECOC_AUTHENTICATED_TXPDUSTATE) || (SecOC_GetTxPduState(txPduInfoIdx) == SECOC_TRANSMISSION_INITIATED_TXPDUSTATE))
      {
        /* --- copy data --- */
        /* --- update state and Freshness Value if required --- */
        switch (SecOC_GetTxPduState(txPduInfoIdx))
        {
          case SECOC_AUTHENTICATED_TXPDUSTATE:
            VStdMemCpyRamToRam(info->SduDataPtr, SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerSecuredPduStartIdxOfTxPduInfo(txPduInfoIdx)), SecOC_GetSecuredPduDataContainerSecuredPduLengthOfTxPduInfo(txPduInfoIdx)); /* SBSW_SECOC_MEM_CPY_2RAM_PARAMCKECK */
            info->SduLength = (PduLengthType) SecOC_GetSecuredPduDataContainerSecuredPduLengthOfTxPduInfo(txPduInfoIdx); /* SBSW_SECOC_PTR2FUNC_4 */
            SecOC_SetTxPduState(txPduInfoIdx, SECOC_TRIGGER_TRANSMIT_TXPDUSTATE); /* SBSW_SECOC_CSL80 */
            break;
          case SECOC_TRANSMISSION_INITIATED_TXPDUSTATE:
            VStdMemCpyRamToRam(info->SduDataPtr, SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerSecuredPduStartIdxOfTxPduInfo(txPduInfoIdx)), SecOC_GetSecuredPduDataContainerSecuredPduLengthOfTxPduInfo(txPduInfoIdx)); /* SBSW_SECOC_MEM_CPY_2RAM_PARAMCKECK */
            info->SduLength = (PduLengthType) SecOC_GetSecuredPduDataContainerSecuredPduLengthOfTxPduInfo(txPduInfoIdx); /* SBSW_SECOC_PTR2FUNC_4 */
            SecOC_SetTxPduState(txPduInfoIdx, SECOC_TRIGGER_TRANSMIT_TXPDUSTATE); /* SBSW_SECOC_CSL80 */
            break;
          default:     /* COV_SECOC_MISRA */
            break;
        }
        ret = E_OK;
      }
      SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_TXSTATE();
    }
#  else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
#  endif
  }

  /* ----- Development Error Report --------------------------------------- */
#  if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)
  {
    SecOC_ReportDetError(SECOC_SID_TRIGGER_TRANSMIT, errorId);
  }
#  endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  return ret;
}  /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
# endif
#endif

#if (SECOC_TRANSPORT_PROTOCOL == STD_ON)
/**********************************************************************************************************************
  SecOC_TpRxIndication
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_TpRxIndication(PduIdType id, Std_ReturnType result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
# if(SECOC_DEV_ERROR_DETECT == STD_ON) /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#  if(SECOC_RXSECUREDPDUINFO == STD_ON)
  else if(id >= SecOC_GetSizeOfRxSecuredPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   if(SECOC_INVALIDHNDOFRXSECUREDPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfRxSecuredPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   endif
#  endif
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
# if (SECOC_RXSECUREDPDUINFO == STD_ON)
    SecOC_RxPduInfoIterType rxPduInfoIdx = SecOC_GetRxPduInfoIdxOfRxSecuredPduInfo(id);
    if(SecOC_GetRxPduState(rxPduInfoIdx) == SECOC_COPYING_RXPDUSTATE)
    {
      if(result == E_OK)
      {
        SecOC_SetRxPduState(rxPduInfoIdx, SECOC_COPY_AUTHENTIC_PDU_TO_DATA_TO_AUTHENTICATE_RXPDUSTATE); /* SBSW_SECOC_CSL80 */
      }
      else
      {
        SecOC_SetRxPduState(rxPduInfoIdx, SECOC_IDLE_RXPDUSTATE); /* SBSW_SECOC_CSL80 */
      }
    }
    {
      SecOC_SetRxTpWrittenBytesCounter(rxPduInfoIdx, 0); /* SBSW_SECOC_CSL80 */
    }
# else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR) /* COV_SECOC_TX_ONLY_CONFIG */
  {
    SecOC_ReportDetError(SECOC_SID_TP_RX_INDICATION, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
  SecOC_TpTxConfirmation
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_TpTxConfirmation(PduIdType id, Std_ReturnType result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
# if(SECOC_DEV_ERROR_DETECT == STD_ON) /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#  if(SECOC_TXSECUREDPDUINFO == STD_ON)
  else if(id >= SecOC_GetSizeOfTxSecuredPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   if(SECOC_INVALIDHNDOFTXSECUREDPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfTxSecuredPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   endif
#  endif
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
# if(SECOC_TXSECUREDPDUINFO == STD_ON)
    SecOC_TxPduInfoIterType txPduInfoIdx = SecOC_GetTxPduInfoIdxOfTxSecuredPduInfo(id);
    if((SecOC_GetTxPduState(txPduInfoIdx) == SECOC_TRANSMISSION_INITIATED_TXPDUSTATE) || (SecOC_GetTxPduState(txPduInfoIdx) == SECOC_COPYING_TXPDUSTATE))
    {
      SecOC_SetTxPduState(txPduInfoIdx, SECOC_INVALID_SECURED_PDU_TXPDUSTATE); /* SBSW_SECOC_CSL80 */
    }
    SecOC_SetTxTpWrittenBytesCounter(txPduInfoIdx, 0); /* SBSW_SECOC_CSL80 */
# else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR) /* COV_SECOC_RX_ONLY_CONFIG */
  {
    SecOC_ReportDetError(SECOC_SID_TP_TX_CONFIRMATION, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}

/**********************************************************************************************************************
  SecOC_CopyRxData
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_CopyRxData(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info, P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) bufferSizePtr)       /* PRQA S 3673 */ /* MD_SecOC_3673_AsrProto */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check if component is initialized */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
# if(SECOC_DEV_ERROR_DETECT == STD_ON) /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#  if(SECOC_RXSECUREDPDUINFO == STD_ON)
  else if(id >= SecOC_GetSizeOfRxSecuredPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   if(SECOC_INVALIDHNDOFRXSECUREDPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfRxSecuredPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   endif
  else if(info == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else if(info->SduDataPtr == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else if(bufferSizePtr == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
#  endif
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
# if (SECOC_RXSECUREDPDUINFO == STD_ON)
    SecOC_RxPduInfoIterType rxPduInfoIdx = SecOC_GetRxPduInfoIdxOfRxSecuredPduInfo(id);
    if((info->SduLength <= (SecOC_GetSecuredPduDataContainerSecuredPduLengthOfRxPduInfo(rxPduInfoIdx) - SecOC_GetRxTpWrittenBytesCounter(rxPduInfoIdx))) &&
        (SecOC_GetRxPduState(rxPduInfoIdx) == SECOC_COPYING_RXPDUSTATE))
    {
      VStdMemCpyRamToRam(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerSecuredPduStartIdxOfRxPduInfo(rxPduInfoIdx) + SecOC_GetRxTpWrittenBytesCounter(rxPduInfoIdx)), info->SduDataPtr, info->SduLength); /* SBSW_SECOC_PTR_WITH_OFFSET */
      SecOC_SetRxTpWrittenBytesCounter(rxPduInfoIdx, (SecOC_GetRxTpWrittenBytesCounter(rxPduInfoIdx) + info->SduLength)); /* SBSW_SECOC_CSL80 */
      *bufferSizePtr = (PduLengthType) (SecOC_GetSecuredPduDataContainerSecuredPduLengthOfRxPduInfo(rxPduInfoIdx) - SecOC_GetRxTpWrittenBytesCounter(rxPduInfoIdx)); /* SBSW_SECOC_PTR2FUNC_4 */
      retVal = BUFREQ_OK;
    }
# else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR) /* COV_SECOC_TX_ONLY_CONFIG */
  {
    SecOC_ReportDetError(SECOC_SID_COPY_RX_DATA, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  return retVal;
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
  SecOC_CopyTxData
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_CopyTxData(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info, P2VAR(RetryInfoType, AUTOMATIC, SECOC_APPL_DATA) retry, P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) availableDataPtr)    /* PRQA S 3673 */ /* MD_SecOC_3673_AsrProto */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check if component is initialized */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
# if(SECOC_DEV_ERROR_DETECT == STD_ON) /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#  if(SECOC_TXSECUREDPDUINFO == STD_ON)
  else if(id >= SecOC_GetSizeOfTxSecuredPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   if(SECOC_INVALIDHNDOFTXSECUREDPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfTxSecuredPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   endif
  else if(info == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else if(info->SduDataPtr == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else if(availableDataPtr == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
#  endif
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
# if(SECOC_TXSECUREDPDUINFO == STD_ON)
    SecOC_TxPduInfoIterType txPduInfoIdx = SecOC_GetTxPduInfoIdxOfTxSecuredPduInfo(id);
    if(SecOC_GetTxPduState(txPduInfoIdx) == SECOC_TRANSMISSION_INITIATED_TXPDUSTATE)
    {
      SecOC_SetTxPduState(txPduInfoIdx, SECOC_COPYING_TXPDUSTATE); /* SBSW_SECOC_CSL80 */
    }
    if(SecOC_GetTxPduState(txPduInfoIdx) == SECOC_COPYING_TXPDUSTATE)
    {
      if(info->SduLength <= (SecOC_GetSecuredPduDataContainerSecuredPduLengthOfTxPduInfo(txPduInfoIdx) - SecOC_GetTxTpWrittenBytesCounter(txPduInfoIdx)))
      {
        VStdMemCpyRamToRam(info->SduDataPtr, SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerSecuredPduStartIdxOfTxPduInfo(txPduInfoIdx) + SecOC_GetTxTpWrittenBytesCounter(txPduInfoIdx)), info->SduLength); /* SBSW_SECOC_PTR_WITH_OFFSET */
        SecOC_SetTxTpWrittenBytesCounter(txPduInfoIdx, (SecOC_GetTxTpWrittenBytesCounter(txPduInfoIdx) + info->SduLength)); /* SBSW_SECOC_CSL80 */
        *availableDataPtr = (PduLengthType) (SecOC_GetSecuredPduDataContainerSecuredPduLengthOfTxPduInfo(txPduInfoIdx) - SecOC_GetTxTpWrittenBytesCounter(txPduInfoIdx)); /* SBSW_SECOC_PTR2FUNC_4 */
        retVal = BUFREQ_OK;
      }
    }
# else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR) /* COV_SECOC_RX_ONLY_CONFIG */
  {
    SecOC_ReportDetError(SECOC_SID_COPY_TX_DATA, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(retry); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  return retVal;
}  /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
  SecOC_StartOfReception
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_StartOfReception(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info, PduLengthType TpSduLength, P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) bufferSizePtr)      /* PRQA S 3673 */ /* MD_SecOC_3673_AsrProto */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check if component is initialized */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
# if(SECOC_DEV_ERROR_DETECT == STD_ON) /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#  if(SECOC_RXSECUREDPDUINFO == STD_ON)
  else if(id >= SecOC_GetSizeOfRxSecuredPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   if(SECOC_INVALIDHNDOFRXSECUREDPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfRxSecuredPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   endif
  else if(bufferSizePtr == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
#  endif
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
# if (SECOC_RXSECUREDPDUINFO == STD_ON)
    if(SecOC_GetSecuredPduDataContainerSecuredPduLengthOfRxPduInfo(SecOC_GetRxPduInfoIdxOfRxSecuredPduInfo(id)) == TpSduLength)
    {
      SecOC_RxPduInfoIterType rxPduInfoIdx = SecOC_GetRxPduInfoIdxOfRxSecuredPduInfo(id);
      if(SecOC_GetRxPduState(rxPduInfoIdx) == SECOC_IDLE_RXPDUSTATE)
      {
        SecOC_SetRxPduState(rxPduInfoIdx, SECOC_COPYING_RXPDUSTATE); /* SBSW_SECOC_CSL80 */
        *bufferSizePtr = (PduLengthType) SecOC_GetSecuredPduDataContainerSecuredPduLengthOfRxPduInfo(rxPduInfoIdx) - SecOC_GetRxTpWrittenBytesCounter(rxPduInfoIdx); /* SBSW_SECOC_PTR2FUNC_4 */
        retVal = BUFREQ_OK;
      }
    }
    else
    {
      retVal = BUFREQ_E_OVFL;
    }
# else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR) /* COV_SECOC_TX_ONLY_CONFIG */
  {
    SecOC_ReportDetError(SECOC_SID_START_OF_RECEPTION, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(info);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  return retVal;
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (SECOC_RXSECUREDPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_VerifyStatusOverride
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE) SecOC_VerifyStatusOverride(uint16 freshnessValueID, uint8 overrideStatus, uint8 numberOfMessagesToOverride)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;
  Std_ReturnType ret = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check if component is initialized */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
#if(SECOC_DEV_ERROR_DETECT == STD_ON) /* COV_SECOC_VAR_DEV_ERROR_DETECT */
  else if(!((overrideStatus == SECOC_CANCEL_VERIFYOVERRIDESTATUS) || (overrideStatus == SECOC_FAIL_N_TIMES_VERIFYOVERRIDESTATUS) || (overrideStatus == SECOC_FAIL_INFINETE_VERIFYOVERRIDESTATUS)
# if (SECOC_ENABLE_FORCED_PASS_OVERRIDESTATUS == STD_ON)
            || (overrideStatus == SECOC_PASS_N_TIMES_VERIFYOVERRIDESTATUS) || (overrideStatus == SECOC_PASS_VERIFYOVERRIDESTATUS)
# endif
          ))
  {
    errorId = SECOC_E_PARAM;
  }
#endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    SecOC_RxPduInfoIterType rxPduId = 0;
    for(; rxPduId < SecOC_GetSizeOfRxPduInfo(); rxPduId++)
    {
      if(freshnessValueID == SecOC_GetExternalFreshnessIdOfRxPduInfo(rxPduId))
      {
        if((overrideStatus == SECOC_FAIL_N_TIMES_VERIFYOVERRIDESTATUS)
# if (SECOC_ENABLE_FORCED_PASS_OVERRIDESTATUS == STD_ON)
            || (overrideStatus == SECOC_PASS_N_TIMES_VERIFYOVERRIDESTATUS)
# endif
        )
        {
          SecOC_SetNumberOfMessagesToOverride(SecOC_GetNumberOfMessagesToOverrideIdxOfRxPduInfo(rxPduId), numberOfMessagesToOverride); /* SBSW_SECOC_CSL03 */
        }
        SecOC_SetVerifyOverrideStatus(SecOC_GetVerifyOverrideStatusIdxOfRxPduInfo(rxPduId), overrideStatus); /* SBSW_SECOC_CSL03 */
      }
    }
    ret = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)
  {
    SecOC_ReportDetError(SECOC_SID_VERIFY_STATUS_OVERRIDE, errorId);
  }
#endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  return ret;
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if(SECOC_DEVMODEENABLED == STD_ON)
/***********************************************************************************************************************
 *  SecOC_SetDevelopmentMode()
 **********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 */
FUNC(void, SECOC_CODE) SecOC_SetDevelopmentMode(boolean enableDevMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    SecOC_SetDevModeEnabled(enableDevMode);
  }
  /* ----- Development Error Report --------------------------------------- */
#if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)
  {
    SecOC_ReportDetError(SECOC_SID_SET_DEVELOPMENT_MODE, errorId);
  }
#endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
}
#endif

/**********************************************************************************************************************
  SecOC_MainFunctionRx
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_MainFunctionRx(void)
{
#if (SECOC_RXPDUINFO == STD_ON)
  /* #10 Check if COM is initialized */
  if(SecOC_IsInitialized())
  {
    /* ----- Implementation ----------------------------------------------- */
    SecOC_RxIf_HandlePdus();
  }
#endif
}

/**********************************************************************************************************************
  SecOC_MainFunctionTx
**********************************************************************************************************************/
/** Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_MainFunctionTx(void)
{
#if (SECOC_TXPDUINFO == STD_ON)
  /* #10 Check if COM is initialized */
  if(SecOC_IsInitialized())
  {
    /* ----- Implementation ----------------------------------------------- */
    SecOC_TxIf_HandlePdus();
  }
#endif
}

#define SECOC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */   /* MD_MSR_19.1 */

/* *INDENT-OFF* */
/* module specific MISRA deviations:
   MD_SecOC_3673_AsrProto:
     Reason:     Function prototype is based on AUTOSAR specification. No optimization possible.
     Risk:       Unexpected value modification.
     Prevention: Covered by code review.
*/

/* SBSW_JUSTIFICATION_BEGIN

  \ID SBSW_SECOC_CSL01
    \DESCRIPTION Index Access is using ComStackLib
    \COUNTERMEASURE  \N Qualified use-case CSL01 of ComStackLib.

  \ID SBSW_SECOC_CSL02
    \DESCRIPTION  Access of Table1, via indirection over Table2 with same index using ComStackLib.
    \COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

  \ID SBSW_SECOC_CSL03
    \DESCRIPTION Access to Table1 via indirection over Table2.
    \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

  \ID SBSW_SECOC_CSL80
    \DESCRIPTION Access of Table1, via indirection over Table2 with same index via indirection over Table 3.
    \COUNTERMEASURE \N Qualified use-case CSL02 and CSL03 of ComStackLib

  \ID SBSW_SECOC_PTR2FUNC_1
    \DESCRIPTION A pointer is passed to a function.
    \COUNTERMEASURE \N CM_SECOC_READ_WITH_CSL_MACRO

  \ID SBSW_SECOC_PTR2FUNC_2
    \DESCRIPTION A pointer is passed to a function.
    \COUNTERMEASURE \N CM_SECOC_LOCAL_STACK_VAR

  \ID SBSW_SECOC_PTR2FUNC_3
    \DESCRIPTION A pointer is passed to a function.
    \COUNTERMEASURE \N CM_SECOC_PTR_HANDED_THROUGH

  \ID SBSW_SECOC_PTR2FUNC_4
    \DESCRIPTION Write access to a passed pointer.
    \COUNTERMEASURE \N CM_SECOC_PTR_HANDED_THROUGH

  \ID SBSW_SECOC_PTR2FUNC_5
    \DESCRIPTION A pointer is passed to a function.
    \COUNTERMEASURE \N CM_SECOC_READ_WITH_CSL_MACRO
                       CM_SECOC_LOCAL_STACK_VAR

  \ID SBSW_SECOC_PTR2FUNC_6
    \DESCRIPTION A pointer is passed to a function.
    \COUNTERMEASURE \N CM_SECOC_LOCAL_STACK_VAR
                       CM_SECOC_PTR_HANDED_THROUGH

  \ID SBSW_SECOC_PTR2FUNC_7
    \DESCRIPTION A pointer is passed to a function.
    \COUNTERMEASURE \N CM_SECOC_READ_WITH_CSL_MACRO
                       CM_SECOC_PTR_HANDED_THROUGH

  \ID SBSW_SECOC_PTR2FUNC_8
    \DESCRIPTION A pointer is passed to a function.
    \COUNTERMEASURE \N CM_SECOC_READ_WITH_CSL_MACRO
                       CM_SECOC_LOCAL_STACK_VAR
                       CM_SECOC_PTR_HANDED_THROUGH

  \ID SBSW_SECOC_VERIFY_STATUS_CALLOUT
    \DESCRIPTION A function pointer is called.
    \COUNTERMEASURE \S see SMI-41233

  \ID SBSW_SECOC_PTR_WITH_OFFSET
    \DESCRIPTION Two pointer are passed to a function.
    \COUNTERMEASURE \R One pointer is read with a generated macro of the ComStackLib and an offset which is checked for out-of-bounds access.
                       The other pointer is handed through from the caller and therefore assumed valid. It is also checked that it is no NULL_PTR.
  \ID SBSW_SECOC_VSTDLIB_SAFE
    \DESCRIPTION    Pointer is passed as function argument.
    \COUNTERMEASURE \N The VStdLib_MemCpy_s guarantees that the destination pointer is not overwritten.

  \ID SBSW_SECOC_MEM_CPY_2RAM
    \DESCRIPTION    Function call to copy data to RAM.
    \COUNTERMEASURE \N The address of the destination and the amount of bytes to be copied are
                       retrieved through use-case CSL03 of ComStackLib indirection access macros.
  \ID SBSW_SECOC_MEM_CPY_2RAM_PARAMCKECK
    \DESCRIPTION    Function call to copy data to RAM.
    \COUNTERMEASURE \N The address of the destination and the amount of bytes to be copied are
                       checked by the parameter checks.
   SBSW_JUSTIFICATION_END */
/*
\CM CM_SECOC_READ_WITH_CSL_MACRO The pointer is read with generated macros of the ComStackLib. Therefore it is always valid.
\CM CM_SECOC_LOCAL_STACK_VAR The pointer references a local stack variable. Therefore it is always a valid memory location.
\CM CM_SECOC_PTR_HANDED_THROUGH The pointer is handed through from the caller. The caller has to ensure it points to a valid memory location and it is of sufficient size.
*/
/* COV_JUSTIFICATION_BEGIN

  \ID COV_SECOC_VAR_DEV_ERROR_DETECT
    \ACCEPT TX
    \REASON Det error detection must always be enabled for SafeBsw.

  \ID COV_SECOC_VAR_DEPENDENT_SWITCH
    \ACCEPT TX
    \REASON The nested switch depends on a previous switch.

  \ID COV_SECOC_TX_ONLY_CONFIG
    \ACCEPT TX
    \REASON In Tx only configs the API shall always return an error.

  \ID COV_SECOC_RX_ONLY_CONFIG
    \ACCEPT TX
    \REASON In Rx only configs the API shall always return an error.

\ID COV_SECOC_RX_SPLIT_ONLY_CONFIG
    \ACCEPT TX
    \REASON In configurations with only Rx Split PDUs this check will always evaluate to true.

  \ID COV_SECOC_VERIFICATION_STATUS_PROPAGATION_BOTH_ONLY_CONFIG
    \ACCEPT TX
    \REASON This configuration only has Pdus with verification status propagation set to BOTH.

  \ID COV_SECOC_SYNC_CSM_ONLY_CONFIG
    \ACCEPT TX
    \REASON This is always true in an interface only config with only synchronous Csm calls.

  \ID COV_SECOC_NO_TRIGGER_TX_CONFIG
    \ACCEPT TF tf tx
    \ACCEPT TF tf tf xf tf
    \REASON In configs without trigger transmit the transmit request is always set by the SecOC_Transmit API.

  \ID COV_SECOC_IGNORE_VERIFICATION_RESULT_CONFIG
    \ACCEPT TX
    \ACCEPT XX
    \REASON If IGNORE_VERIFICATION_RESULT is configured to true the verification result is always SECOC_E_VER_OK.

  \ID COV_SECOC_NO_TRUNCATED_FRESHNESS_VALUE
    \ACCEPT TX
    \ACCEPT XF
    \REASON If no Rx Pdu has a truncated Freshness Value configured, the truncated MAC is always >= 8 and multiple of 8 Bits. Therefore no Byte offset is existing.

  \ID COV_SECOC_ALWAYS_ON
    \ACCEPT TX
    \REASON This preprocessor switch is always on if Tp or If Pdus are configured.

  \ID COV_SECOC_MISRA
    \ACCEPT X
    \ACCEPT TX
    \REASON [MSR_COV_MISRA]

  \ID COV_SECOC_NOT_SUPPORTED
    \ACCEPT XF
    \REASON Message linker with not byte aligned length is not supported.

  \ID COV_SECOC_SAFEBSW_CHECK
    \ACCEPT TX
    \ACCEPT TX tx tx
    \REASON This condition is required for safeBSW, the if will only evaluate to false if the generated code would be incorrect.

COV_JUSTIFICATION_END */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  END OF FILE: SecOC.c
 *********************************************************************************************************************/
