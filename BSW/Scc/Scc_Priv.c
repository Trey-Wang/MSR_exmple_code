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
/**        \file  Scc_Priv.c
 *        \brief  Smart Charge Communication Source Code File
 *
 *      \details  Implements Vehicle 2 Grid communication according to the specifications ISO/IEC 15118-2,
 *                DIN SPEC 70121 and customer specific schemas.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the Scc module. >> Scc.h
 *********************************************************************************************************************/ 

#define SCC_PRIV_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Scc.h"
#include "Scc_Priv.h"
#include "Scc_Cfg.h"
#include "Scc_Lcfg.h"
#include SCC_IF_CFG_H

#if ( defined SCC_PROD_ERROR_DETECT ) && ( SCC_PROD_ERROR_DETECT == STD_ON )
#include "Dem.h"
#endif /* SCC_PROD_ERROR_DETECT */
#if ( defined SCC_DEV_ERROR_DETECT ) && ( SCC_DEV_ERROR_DETECT == STD_ON )
#include "Det.h"
#endif /* SCC_DEV_ERROR_DETECT */
#include "IpBase.h"
#include "TcpIp.h"
#if ( defined SCC_ENABLE_TLS ) && ( SCC_ENABLE_TLS == STD_ON )
#include "Tls.h"
#endif /* SCC_ENABLE_TLS */
#if ( defined SCC_ENABLE_PRIVATE_KEY_ENCRYPTION ) && ( SCC_ENABLE_PRIVATE_KEY_ENCRYPTION == STD_ON )
#include "actIKDF2.h"
#endif /* SCC_ENABLE_PRIVATE_KEY_ENCRYPTION */

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/
/* PRQA S 0715 NEST_STRUCTS */ /* MD_MSR_1.1 */
/* PRQA S 2006 RETURN_PATHS */ /* MD_MSR_14.7 */
/*lint -e438 */
/*lint -e571 */
/*lint -e830 */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define SCC_BER_STACK_DEPTH 6
#define SCC_CPS_TAG_LEN 3

/**********************************************************************************************************************
 *  LOCAL / GLOBAL DATA
 *********************************************************************************************************************/
/* uint8 variables - no init */
#define SCC_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( defined SCC_ENABLE_PRIVATE_KEY_ENCRYPTION ) && ( SCC_ENABLE_PRIVATE_KEY_ENCRYPTION == STD_ON )
SCC_VARNOINIT(uint8) Scc_XcryptPrivKeyBuf[SCC_PRIV_KEY_LEN];
#endif /* SCC_ENABLE_PRIVATE_KEY_ENCRYPTION */

#define SCC_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* other variables - no init */
#define SCC_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC SCC_VARNOINIT(IpBase_BerWorkspaceType)    Scc_BerWs;

#define SCC_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define SCC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  Scc_VInit
 *********************************************************************************************************************/
SCC_FUNCCODE(void) Scc_Priv_Init(void)
{

  return;
}

SCC_FUNCCODE(void) Scc_ReportError(Scc_StackErrorType StackError)
{
  /* check if a tx buffer was requested */
  if ( NULL_PTR != Scc_ExiStreamTxPBufPtr )
  {
    /* set the tx data to 0 */
    Scc_ExiStreamTxPBufPtr->len = 0;
    Scc_ExiStreamTxPBufPtr->totLen = 0;
    /* free the buffer */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    (void) Tl_TransmitTo(Scc_V2GSocket, (SCC_P2VARAPPLDATA(Scc_PbufType))Scc_ExiStreamTxPBufPtr,
    (SCC_P2VARAPPLDATA(Scc_SockAddrType))&Scc_ServerSockAddr);
    /* reset the pbuf pointer */
    Scc_ExiStreamTxPBufPtr = (SCC_P2VARAPPLDATA(Scc_PbufType))NULL_PTR;
  }

  /* check if a StackError is already set or if no MsgTrig is active */
  if ( Scc_StackError_NoError != Scc_StackError )
  {
    /* do not overwrite the current StackError */
    return;
  }

  /* set the StackError */
  Scc_StackError = StackError;

  /* set the MsgStatus */
  switch ( Scc_MsgTrig )
  {
  case Scc_MsgTrig_SECCDiscoveryProtocol:
    Scc_MsgStatus = Scc_MsgStatus_SECCDiscoveryProtocol_Failed;
    break;

  case Scc_MsgTrig_TransportLayer:
    Scc_MsgStatus = Scc_MsgStatus_TransportLayer_Failed;
    break;

  case Scc_MsgTrig_SupportedAppProtocol:
    Scc_MsgStatus = Scc_MsgStatus_SupportedAppProtocol_Failed;
    break;

  case Scc_MsgTrig_SessionSetup:
    Scc_MsgStatus = Scc_MsgStatus_SessionSetup_Failed;
    break;

  case Scc_MsgTrig_ServiceDiscovery:
    Scc_MsgStatus = Scc_MsgStatus_ServiceDiscovery_Failed;
    break;

  case Scc_MsgTrig_ReadContrCertChain:
    Scc_MsgStatus = Scc_MsgStatus_ReadContrCertChain_Failed;
    break;

  case Scc_MsgTrig_ServiceDetail:
    Scc_MsgStatus = Scc_MsgStatus_ServiceDetail_Failed;
    break;

  case Scc_MsgTrig_PaymentServiceSelection:
    Scc_MsgStatus = Scc_MsgStatus_PaymentServiceSelection_Failed;
    break;

  case Scc_MsgTrig_ReadRootCerts:
    Scc_MsgStatus = Scc_MsgStatus_ReadRootCerts_Failed;
    break;

  case Scc_MsgTrig_ReadProvCert:
    Scc_MsgStatus = Scc_MsgStatus_ReadProvCert_Failed;
    break;

  case Scc_MsgTrig_CertificateInstallation:
    Scc_MsgStatus = Scc_MsgStatus_CertificateInstallation_Failed;
    break;

  case Scc_MsgTrig_CertificateUpdate:
    Scc_MsgStatus = Scc_MsgStatus_CertificateUpdate_Failed;
    break;

  case Scc_MsgTrig_PaymentDetails:
    Scc_MsgStatus = Scc_MsgStatus_PaymentDetails_Failed;
    break;

  case Scc_MsgTrig_Authorization:
    Scc_MsgStatus = Scc_MsgStatus_Authorization_Failed;
    break;

  case Scc_MsgTrig_ChargeParameterDiscovery:
    Scc_MsgStatus = Scc_MsgStatus_ChargeParameterDiscovery_Failed;
    break;

  case Scc_MsgTrig_PowerDelivery:
    Scc_MsgStatus = Scc_MsgStatus_PowerDelivery_Failed;
    break;

  case Scc_MsgTrig_MeteringReceipt:
    Scc_MsgStatus = Scc_MsgStatus_MeteringReceipt_Failed;
    break;

#if ( defined SCC_CHARGING_AC ) && ( SCC_CHARGING_AC == STD_ON )
  case Scc_MsgTrig_ChargingStatus:
    Scc_MsgStatus = Scc_MsgStatus_ChargingStatus_Failed;
    break;
#endif /* SCC_CHARGING_AC */

#if ( defined SCC_CHARGING_DC ) && ( SCC_CHARGING_DC == STD_ON )

  case Scc_MsgTrig_CableCheck:
    Scc_MsgStatus = Scc_MsgStatus_CableCheck_Failed;
    break;

  case Scc_MsgTrig_PreCharge:
    Scc_MsgStatus = Scc_MsgStatus_PreCharge_Failed;
    break;

  case Scc_MsgTrig_CurrentDemand:
    Scc_MsgStatus = Scc_MsgStatus_CurrentDemand_Failed;
    break;

  case Scc_MsgTrig_WeldingDetection:
    Scc_MsgStatus = Scc_MsgStatus_WeldingDetection_Failed;
    break;

#endif /* SCC_CHARGING_DC */

  case Scc_MsgTrig_SessionStop:
    Scc_MsgStatus = Scc_MsgStatus_SessionStop_Failed;
    break;

  default: /* case Scc_MsgTrig_StopCommunicationSession: */
    Scc_MsgStatus = Scc_MsgStatus_StopCommunicationSession_Failed;
    break;
  }

  /* report the MsgStatus and the StackError to the application */
  Scc_Set_Core_MsgStatus(Scc_MsgStatus);
  Scc_Set_Core_StackError(Scc_StackError);

  return;
} /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */

SCC_FUNCCODE(void) Scc_ReportErrorAndStatus(Scc_StackErrorType StackError, Scc_MsgStatusType MsgStatus)
{
  /* check if a tx buffer was requested */
  if ( NULL_PTR != Scc_ExiStreamTxPBufPtr )
  {
    /* set the tx data to 0 */
    Scc_ExiStreamTxPBufPtr->len = 0;
    Scc_ExiStreamTxPBufPtr->totLen = 0;
    /* free the buffer */ /* PRQA S 0310,3305 2 */ /* MD_Scc_0310_3305 */
    (void) Tl_TransmitTo(Scc_V2GSocket, (SCC_P2VARAPPLDATA(Scc_PbufType))Scc_ExiStreamTxPBufPtr,
    (SCC_P2VARAPPLDATA(Scc_SockAddrType))&Scc_ServerSockAddr);
    /* reset the pbuf pointer */
    Scc_ExiStreamTxPBufPtr = (SCC_P2VARAPPLDATA(Scc_PbufType))NULL_PTR;
  }

  /* check if a StackError is already set or if no MsgTrig is active */
  if ( Scc_StackError_NoError != Scc_StackError )
  {
    /* do not overwrite the current StackError */
    return;
  }

  /* set the StackError */
  Scc_StackError = StackError;
  /* set the MsgStatus */
  Scc_MsgStatus = MsgStatus;

  /* report the MsgStatus and the StackError to the application */
  Scc_Set_Core_MsgStatus(Scc_MsgStatus);
  Scc_Set_Core_StackError(Scc_StackError);

  return;
}

SCC_FUNCCODE(void) Scc_ReportSuccessAndStatus(Scc_MsgStatusType MsgStatus)
{
  /* check if a StackError is already set or if no MsgTrig is active */
  if (   ( Scc_StackError_NoError != Scc_StackError )
      || ( Scc_MsgTrig_None == Scc_MsgTrig ))
  {
    /* do not overwrite the current StackError or set a StackError if no trigger is active */
    return;
  }

  /* set the MsgStatus */
  Scc_MsgStatus = MsgStatus;

  /* report the MsgStatus and the StackError to the application */
  Scc_Set_Core_MsgStatus(Scc_MsgStatus);

  return;
}

/**********************************************************************************************************************
 *  Scc_AdjustValueToMultiplier
 *********************************************************************************************************************/
SCC_FUNCCODE(sint32) Scc_AdjustValueToMultiplier(sint16 Value, sint8 Multiplier, boolean LimitSInt16)
{
  sint32 ValueBuf;

  switch ( Multiplier )
  {
  case -4:
    ValueBuf = ( Value / 10000 );
    break;
  case -3:
    ValueBuf = ( Value / 1000 );
    break;
  case -2:
    ValueBuf = ( Value / 100 );
    break;
  case -1:
    ValueBuf = ( Value / 10 );
    break;

  case 0:
    ValueBuf = Value;
    break;

  case 1:
    if (( TRUE == LimitSInt16 ) && ( 3276 < Value ))
    {
      ValueBuf = 32767;
    }
    else if (( TRUE == LimitSInt16 ) && ( -3276 > Value ))
    {
      ValueBuf = -32768;
    }
    else
    {
      ValueBuf = ( Value * 10 );
    }
    break;

  case 2:
    if (( TRUE == LimitSInt16 ) && ( 327 < Value ))
    {
      ValueBuf = 32767;
    }
    else if (( TRUE == LimitSInt16 ) && ( -327 > Value ))
    {
      ValueBuf = -32768;
    }
    else
    {
      ValueBuf = ( Value * 100 );
    }
    break;

  case 3:
    if (( TRUE == LimitSInt16 ) && ( 32 < Value ))
    {
      ValueBuf = 32767;
    }
    else if (( TRUE == LimitSInt16 ) && ( -32 > Value ))
    {
      ValueBuf = -32768;
    }
    else
    {
      ValueBuf = ( Value * 1000 );
    }
    break;

  case 4:
    if (( TRUE == LimitSInt16 ) && ( 3 < Value ))
    {
      ValueBuf = 32767;
    }
    else if (( TRUE == LimitSInt16 ) && ( -3 > Value ))
    {
      ValueBuf = -32768;
    }
    else
    {
      ValueBuf = ( Value * 10000 );
    }
    break;

  case 5:
    if (( TRUE == LimitSInt16 ) && ( 0 < Value ))
    {
      ValueBuf = 32767;
    }
    else if (( TRUE == LimitSInt16 ) && ( 0 > Value ))
    {
      ValueBuf = -32768;
    }
    else
    {
      ValueBuf = ( Value * 100000 );
    }
    break;

  case 6:
    if (( TRUE == LimitSInt16 ) && ( 0 < Value ))
    {
      ValueBuf = 32767;
    }
    else if (( TRUE == LimitSInt16 ) && ( 0 > Value ))
    {
      ValueBuf = -32768;
    }
    else
    {
      ValueBuf = ( Value * 1000000 );
    }
    break;

  default:
    if (( -4 > Multiplier ) && ( -7 < Multiplier ))
    {
      ValueBuf = 0;
    }
    else
    {
      ValueBuf = 0;
      /* multiplier is out of range, this should not happen */
      Scc_CallDetReportError(SCC_API_ID_V_ADJUST_VALUE_2_MULT, SCC_DET_INV_PARAM)
    }
    break;
  }

  return ValueBuf;
} /* PRQA S 6030 */ /* MD_MSR_STCYC */

/**********************************************************************************************************************
 *  Scc_AdjustAmountToMultiplier
 *********************************************************************************************************************/
SCC_FUNCCODE(uint32) Scc_AdjustAmountToMultiplier(uint32 amount, sint8 Multiplier, boolean RoundUp)
{
  uint32 ValueBuf;

  switch ( Multiplier )
  {
  case -3:
    ValueBuf = (uint32)(amount / 1000u);
    /* check if the value shall be rounded up */
    if (   ( TRUE == RoundUp )
        && ( 499u < ( amount % 1000u )))
    {
      ValueBuf++;
    }
    break;

  case -2:
    ValueBuf = (uint32)(amount / 100u);
    /* check if the value shall be rounded up */
    if (   ( TRUE == RoundUp )
        && ( 49u < ( amount % 100u )))
    {
      ValueBuf++;
    }
    break;

  case -1:
    ValueBuf = (uint32)(amount / 10u);
    /* check if the value shall be rounded up */
    if (   ( TRUE == RoundUp )
        && ( 4u < ( amount % 10u )))
    {
      ValueBuf++;
    }
    break;

  case 0:
    ValueBuf = amount;
    break;

  case 1:
    /* check if the amount will overflow after the multiplication */
    if ( 429496729u < amount )
    {
      /* restrict the amount */
      ValueBuf = 4294967290u;
    }
    else
    {
      ValueBuf = (uint32)(amount * 10);
    }
    break;

  case 2:
    /* check if the amount will overflow after the multiplication */
    if ( 42949672u < amount )
    {
      /* restrict the amount */
      ValueBuf = 4294967200u;
    }
    else
    {
      ValueBuf = (uint32)(amount * 100u);
    }
    break;

  case 3:
    /* check if the amount will overflow after the multiplication */
    if ( 4294967u < amount )
    {
      /* restrict the amount */
      ValueBuf = 4294967000u;
    }
    else
    {
      ValueBuf = (uint32)(amount * 1000u);
    }
    break;

  default:
    Scc_CallDetReportError(SCC_API_ID_V_ADJUST_AMOUNT_2_MULT, SCC_DET_INV_PARAM)
    ValueBuf = 0u;
    break;
  }

  return ValueBuf;
} /* PRQA S 6030 */ /* MD_MSR_STCYC */

#if ( defined SCC_ENABLE_PRIVATE_KEY_VALIDATION ) && ( SCC_ENABLE_PRIVATE_KEY_VALIDATION == STD_ON )
/**********************************************************************************************************************
 *  Scc_ValidateKeyPair
 *********************************************************************************************************************/
SCC_FUNCCODE(Std_ReturnType) Scc_ValidateKeyPair(
  SCC_P2CONSTNOINIT(uint8) PrivKeyPtr, uint16 PrivKeyLen, SCC_P2CONSTNOINIT(uint8) CertPtr, uint16 CertLen)
{
  uint16 SigLen = SCC_CRYPTO_BUF_LEN;
  uint16 PubKeyIdx;
  uint16 PubKeyLen;

# if ( defined SCC_ENABLE_CSM_SUPPORT ) && ( SCC_ENABLE_CSM_SUPPORT == STD_ON )
  uint32 SignatureLengthCsm = sizeof(Scc_CryptoBuf);
  Crypto_VerifyResultType verifyResult;
# endif /* SCC_ENABLE_CSM_SUPPORT */

  /* search the PublicKey in the next given certificate */
  if ( (Std_ReturnType)E_OK != Scc_GetIndexOfPublicKey(CertPtr, CertLen, &PubKeyIdx, &PubKeyLen) )
  {
    return (Std_ReturnType)E_NOT_OK;
  }

# if ( defined SCC_ENABLE_CSM_SUPPORT ) && ( SCC_ENABLE_CSM_SUPPORT == STD_ON )

  /* create the ecdsa signature using the private key */
  if( (Std_ReturnType)E_OK != Csm_KeySetValid(Scc_ECDSASignCertKeyIds[Scc_CertsWs.ChosenContrCertChainIdx]))
  {
    return (Std_ReturnType)E_NOT_OK;
  }
  else if( (Std_ReturnType)E_OK != Csm_SignatureGenerate(Scc_ECDSASignCertJobIds[Scc_CertsWs.ChosenContrCertChainIdx],
    CRYPTO_OPERATIONMODE_SINGLECALL, &Scc_Sha256HashBuf[0],
    ESL_SIZEOF_SHA256_DIGEST, &Scc_CryptoBuf[0], &SignatureLengthCsm))
  {
    return (Std_ReturnType)E_NOT_OK;
  }

  /* validate the ecdsa signature using the public key */
  /* PubKeyPtr[0] == 0x04 --> uncompressed point */
  else if( (Std_ReturnType)E_OK != Csm_KeyElementSet(Scc_ECDSAVerifyKeyId,
    CRYPTO_KE_CERTIFICATE_SUBJECT_PUBLIC_KEY, &CertPtr[PubKeyIdx+1], PubKeyLen-1))
  {
    return (Std_ReturnType)E_NOT_OK;
  }
  else if( (Std_ReturnType)E_OK != Csm_KeySetValid(Scc_ECDSAVerifyKeyId))
  {
    return (Std_ReturnType)E_NOT_OK;
  }
  else if( (Std_ReturnType)E_OK != Csm_SignatureVerify(Scc_ECDSAVerifyJobId, 
    CRYPTO_OPERATIONMODE_SINGLECALL, &Scc_Sha256HashBuf[0],
    ESL_SIZEOF_SHA256_DIGEST, &Scc_CryptoBuf[0], SignatureLengthCsm, &verifyResult))
  {
    return (Std_ReturnType)E_NOT_OK;
  }
  else if( CRYPTO_E_VER_OK != verifyResult )
  {
    return (Std_ReturnType)E_NOT_OK;
  }

# else /* SCC_ENABLE_CSM_SUPPORT */
  /* create the ecdsa signature using the private key */
  if ( (Std_ReturnType)E_OK != Crypto_GenerateEcdsaSignature(&Scc_CryptoWs, &Scc_CryptoBuf[0], PrivKeyPtr,
    &Scc_Sha256HashBuf[0], &SigLen, PrivKeyLen, ESL_SIZEOF_SHA256_DIGEST, FALSE) )
  {
    return (Std_ReturnType)E_NOT_OK;
  }

  /* validate the ecdsa signature using the public key */
  if ( (Std_ReturnType)E_OK != Crypto_ValidateEcdsaSignature(&Scc_CryptoWs, &Scc_CryptoBuf[0], &CertPtr[PubKeyIdx],
    &Scc_Sha256HashBuf[0], SigLen, PubKeyLen, ESL_SIZEOF_SHA256_DIGEST, FALSE) )
  {
    return (Std_ReturnType)E_NOT_OK;
  }
# endif /* SCC_ENABLE_CSM_SUPPORT */
  else
  {
    return (Std_ReturnType)E_OK;
  }
}
#endif /* SCC_ENABLE_PRIVATE_KEY_VALIDATION */

/**********************************************************************************************************************
 *  Scc_CheckForCpoInDc
 *********************************************************************************************************************/
SCC_FUNCCODE(Std_ReturnType) Scc_CheckForCpoInDc(SCC_P2CONSTAPPLDATA(uint8) CertPtr, uint16 CertLen)
{
  IpBase_BerElementType BerEleDesc;
  uint8                 BerObjIdDC[10] = { 0x09, 0x92, 0x26, 0x89, 0x93, 0xf2, 0x2c, 0x64, 0x01, 0x19 };
  uint8                 BerIssuerEleNr[6] = { 1, 1, 6, 1, 1, 1 };

  BerEleDesc.Idx = 0;

  /* search the domain component within the issuer */
  while ( BerEleDesc.Idx < CertLen )
  {
    /* search for the object identifier and check if an error occured */
    if ( IPBASE_E_OK != IpBase_BerGetElement(&Scc_BerWs, &BerEleDesc, &BerIssuerEleNr[0], sizeof(BerIssuerEleNr),
      &CertPtr[0], CertLen) )
    {
      /* while loop will return here, if certificate did not contain the CPS string */
      Scc_ReportError(Scc_StackError_IpBase);
      return (Std_ReturnType)E_NOT_OK;
    }
    /* check if the length does not match */
    if ( ( BerEleDesc.EndIdx - BerEleDesc.ContentIdx ) != sizeof(BerObjIdDC) )
    {
      /* search for the next object identifier within the issuer */
      BerIssuerEleNr[3]++;
      continue; /* PRQA S 0770 */ /* MD_Scc_0770 */
    }
    /* check if the object identifier does not match */
    if ( IPBASE_CMP_EQUAL != IpBase_StrCmpLen(&BerObjIdDC[0], &CertPtr[BerEleDesc.ContentIdx], sizeof(BerObjIdDC)) )
    {
      /* search for the next object identifier within the issuer */
      BerIssuerEleNr[3]++;
      continue; /* PRQA S 0770 */ /* MD_Scc_0770 */
    }

    /* DomainComponent was found, switch from AttributeType to AttributeValue */
    BerIssuerEleNr[5]++;
    /* get the AttributeValue and check if an error occured */
    if ( IPBASE_E_OK != IpBase_BerGetElement(&Scc_BerWs, &BerEleDesc, &BerIssuerEleNr[0], sizeof(BerIssuerEleNr),
      &CertPtr[0], CertLen) )
    {
      Scc_ReportError(Scc_StackError_IpBase);
      return (Std_ReturnType)E_NOT_OK;
    }
    /* check if the AttributeValue matches "CPS" */
    if ( SCC_CPS_TAG_LEN == ( BerEleDesc.EndIdx - BerEleDesc.ContentIdx ))
    {
      uint8 CpsTag[SCC_CPS_TAG_LEN] = { 'C', 'P', 'O' };
      if ( IPBASE_CMP_EQUAL == IpBase_StrCmpLen(&CpsTag[0], &CertPtr[BerEleDesc.ContentIdx], SCC_CPS_TAG_LEN) )
      {
        return (Std_ReturnType)E_OK;
      }
    }

    /* search for the next object identifier within the issuer */
    BerIssuerEleNr[3]++;
  }

  /* certificate did not contain the CPS string */
  Scc_ReportError(Scc_StackError_InvalidRxParameter);
  return (Std_ReturnType)E_NOT_OK;
}

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#define SCC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/
/* PRQA L:NEST_STRUCTS */
/* PRQA L:RETURN_PATHS */

/**********************************************************************************************************************
 *  END OF FILE: Scc_Priv.c
 *********************************************************************************************************************/
