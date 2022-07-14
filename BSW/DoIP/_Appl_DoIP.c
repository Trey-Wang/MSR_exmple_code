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
/**        \file  Appl_DoIP.c
 *        \brief  Diagnostic over Internet Protocol
 *
 *      \details  Demo application for component
 *  
 *********************************************************************************************************************/

#define APPLDOIP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Appl_DoIP.h"

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/

/* EXAMPLE
 *  uint8 Appl_DoIP_Vin[17u]           = { 0x57u, 0x30u, 0x4Cu, 0x30u, 0x30u, 0x30u, 0x30u, 0x34u, 0x33u, 0x4Du, 0x42u, 0x35u, 0x34u, 0x31u, 0x33u, 0x32u, 0x36u };
 *  uint8 Appl_DoIP_Gid[6u]            = { 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu };
 *  uint8 Appl_DoIP_RoutActState       = DOIP_ACTIVATION_LINE_ACTIVE; // DOIP_ACTIVATION_LINE_ACTIVE/DOIP_ACTIVATION_LINE_INACTIVE
 *  uint8 Appl_DoIP_RoutActAuthReq[]   = { 0x00u, 0x01u };
 *  uint8 Appl_DoIP_RoutActAuthReqLen  = 2u;
 *  uint8 Appl_DoIP_RoutActConfReq[]   = { 0x02u, 0x03u };
 *  uint8 Appl_DoIP_RoutActConfReqLen  = 2u;
 *  uint8 Appl_DoIP_RoutActAuthRes[]   = { 0x04u, 0x05u };
 *  uint8 Appl_DoIP_RoutActAuthResLen  = 2u;
 *  uint8 Appl_DoIP_RoutActConfRes[]   = { 0x06u, 0x07u };
 *  uint8 Appl_DoIP_RoutActConfResLen  = 2u;
 *  uint8 Appl_DoIP_OemPayloadTypeSend = 0x01u;
 *  uint8 Appl_DoIP_DiagPowerMode      = 0x01u;
 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  ApplDoIP_GetVin()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_GetVin(
  uint8* Vin)
{
#error "Appl_DoIP: ApplDoIP_GetVin() needs to be implemented! An example is given in template."

  /* EXAMPLE
   *  uint8 idx;
   *
   *  for ( idx = 0u; idx < 17u; idx++)
   *  {
   *    Vin[idx] = Appl_DoIP_Vin[idx];
   *  }
   *
   *  return E_OK;
   */
}

/**********************************************************************************************************************
 *  ApplDoIP_GetGid()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_GetGid(
  uint8* GroupId)
{
#error "Appl_DoIP: ApplDoIP_GetGid() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."

  /* EXAMPLE
   *  uint8 idx;
   *
   *  for ( idx = 0u; idx < 6u; idx++)
   *  {
   *    GroupId[idx] = Appl_DoIP_Gid[idx];
   *  }
   *
   *  return E_OK;
   */
}

/**********************************************************************************************************************
 *  ApplDoIP_TriggerGidSync()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_TriggerGidSync(void)
{
#error "Appl_DoIP: ApplDoIP_TriggerGidSync() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."

  /* EXAMPLE
   *  return E_OK;
   */
}

/**********************************************************************************************************************
 *  ApplDoIP_GetDiagPowerMode()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_GetDiagPowerMode(
  DoIP_PowerStateType* PowerStateReady)
{
#error "Appl_DoIP: ApplDoIP_GetDiagPowerMode() needs to be implemented! An example is given in template."

  /* EXAMPLE
   *  *PowerStateReady = Appl_DoIP_DiagPowerMode;
   *
   *  return E_OK;
   */
}

/**********************************************************************************************************************
 *  ApplDoIP_ShutdownFinished()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_ShutdownFinished(void)
{
#error "Appl_DoIP: ApplDoIP_ShutdownFinished() is an optional callback. Implement or remove the function depending on your configuration!"
}

/**********************************************************************************************************************
 *  ApplDoIP_GetActivationLineState()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_GetActivationLineState(
  DoIP_ActivationLineType* const state)
{
#error "Appl_DoIP: ApplDoIP_GetActivationLineState() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."

  /* EXAMPLE
   *  *state = Appl_DoIP_RoutActState;
   *
   *  return E_OK;
   */
}

/**********************************************************************************************************************
 *  ApplDoIP_RoutingActivationAuthentication()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_RoutingActivationAuthentication(
  boolean* Authentified,
  uint8* AuthenticationReqData,
  uint8* AuthenticationResData)
{
#error "Appl_DoIP: ApplDoIP_RoutingActivationAuthentication() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."

  /* EXAMPLE
   *  uint8 idx;
   *
   *  // check OEM specific data
   *  for ( idx = 0u; idx < Appl_DoIP_RoutActAuthReqLen; idx++ )
   *  {
   *    if ( AuthenticationReqData[idx] != Appl_DoIP_RoutActAuthReq[idx] )
   *    {
   *      return E_NOT_OK;
   *    }
   *  }
   *
   *  // fill OEM spcific data/
   *  for ( idx = 0u; idx < Appl_DoIP_RoutActAuthResLen; idx++ )
   *  {
   *    AuthenticationResData[idx] = Appl_DoIP_RoutActAuthRes[idx];
   *  }
   *
   *  *Authentified = TRUE;
   *
   *  return E_OK;
   */
}

/**********************************************************************************************************************
 *  ApplDoIP_RoutingActivationConfirmation()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_RoutingActivationConfirmation(
  boolean* Confirmed,
  uint8* ConfirmationReqData,
  uint8* ConfirmationResData)
{
#error "Appl_DoIP: ApplDoIP_RoutingActivationConfirmation() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."

  /* EXAMPLE
   *  uint8 idx;
   *
   *  // check OEM specific data
   *  for ( idx = 0u; idx < Appl_DoIP_RoutActConfReqLen; idx++ )
   *  {
   *    if ( ConfirmationReqData[idx] != Appl_DoIP_RoutActConfReq[idx] )
   *    {
   *      return E_NOT_OK;
   *    }
   *  }
   *
   *  // fill OEM spcific data/
   *  for ( idx = 0u; idx < Appl_DoIP_RoutActConfResLen; idx++ )
   *  {
   *    ConfirmationResData[idx] = Appl_DoIP_RoutActConfRes[idx];
   *  }
   *
   *  *Confirmed = TRUE;
   *
   *  return E_OK;
   */
}

/**********************************************************************************************************************
 *  ApplDoIP_OemSpecificPayloadType()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_OemSpecificPayloadType(
  uint16 RxPayloadType,
  const PduInfoType* RxUserData,
  DoIP_OemPayloadTypeFlagType Flags,
  uint16* TxPayloadType,
  PduInfoType* TxUserData)
{
#error "Appl_DoIP: ApplDoIP_OemSpecificPayloadType() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."

  /* EXAMPLE
   *  uint8 idx;
   *
   *  // check payload type
   *  if ( RxPayloadType != 0xF000u )
   *  {
   *    return E_NOT_OK;
   *  }
   *
   *  // check flags
   *  if ( (Flags & DOIP_OEM_PAYLOAD_TYPE_FLAG_PROT_TCP) == 0u )
   *  { // not received on TCP
   *    return E_NOT_OK;
   *  }
   *  if ( (Flags & DOIP_OEM_PAYLOAD_TYPE_FLAG_ROUT_ACTIV) == 0u )
   *  { // routing activation not active
   *    return E_NOT_OK;
   *  }
   *
   *  // check user data
   *  for ( idx = 0u; idx < RxUserData->SduLength; idx++ )
   *  {
   *    if ( RxUserData->SduDataPtr[idx] != idx )
   *    {
   *      return E_NOT_OK;
   *    }
   *  }
   *
   *  // prepare response if required
   *  if ( Appl_DoIP_OemPayloadTypeSend == 0x01u )
   *  {
   *    *TxPayloadType = 0xF001u;
   *
   *    if ( RxUserData->SduLength == 0u )
   *    {
   *      // if request has no user data send response without user data, too
   *      TxUserData->SduLength = 0u;
   *    }
   *    else
   *    {
   *      // copy have of request data
   *      for ( idx = 0u; idx < (RxUserData->SduLength / 2u); idx++ )
   *      {
   *        TxUserData->SduDataPtr[idx] = idx + 1u;
   *      }
   *      TxUserData->SduLength = RxUserData->SduLength / 2u;
   *    }
   *  }
   *
   *  return E_OK;
   */
}

/**********************************************************************************************************************
 *  ApplDoIP_VerifyTargetAddr()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_VerifyTargetAddr(
  uint16 TargetAddr)
{
#error "Appl_DoIP: ApplDoIP_VerifyTargetAddr() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."

  /* EXAMPLE
   *  Std_ReturnType retVal = E_NOT_OK;
   *
   *  // check target address
   *  if ( TargetAddr == 0x0200u )
   *  {
   *    retVal = E_OK;
   *  }
   *
   *  return retVal;
   */
}

/**********************************************************************************************************************
 *  ApplDoIP_VerifyRxPdu()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_VerifyRxPdu(
  SoAd_SockAddrType* LocalAddrPtr,
  SoAd_SockAddrType* RemoteAddrPtr,
  uint16 SourceAddr,
  uint16 TargetAddr,
  PduInfoType* PduInfoPtr)
{
#error "Appl_DoIP: ApplDoIP_VerifyRxPdu() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."

  /* EXAMPLE
   *  Std_ReturnType retVal = E_NOT_OK;
   *  
   *  // check logical addresses
   *  if ( (SourceAddr == 0x0201u) &&
   *       (TargetAddr == 0x0E00u) )
   *  {
   *    if ( // check local ip addresses
   *         (*(((SoAd_SockAddrInetType*)LocalAddrPtr)->domain)  == TCPIP_AF_INET) &&
   *         (*(((SoAd_SockAddrInetType*)LocalAddrPtr)->port)    == 0x5834u) &&    // 0x5834u nbo --> 0x3458u / 13400u
   *         (*(((SoAd_SockAddrInetType*)LocalAddrPtr)->addr)    == 0x2001FEA9u) && // 0x2001FEA9u nbo --> 169.254.1.32
   *         // check remote ip addresses
   *         (*(((SoAd_SockAddrInetType*)RemoteAddrPtr)->domain) == TCPIP_AF_INET) &&
   *         (*(((SoAd_SockAddrInetType*)RemoteAddrPtr)->port)   == 0x4858u) &&    // 0x4858u nbo --> 0x5848u / 22600u
   *         (*(((SoAd_SockAddrInetType*)RemoteAddrPtr)->addr)   == 0x0501FEA9u) )  // 0x0501FEA9u nbo --> 169.254.1.5
   *    { // ip addresss chack passes
   *      
   *      // check user data
   *      if ( (PduInfoPtr != NULL_PTR) &&
   *           (PduInfoPtr->SduLength > 0u) &&
   *           (PduInfoPtr->SduDataPtr[0u] == 'A') )
   *      { // user data check (of first byte) passes
   *        retVal = E_OK;
   *      }
   *      else
   *      { // user data check (of first byte) fails
   *        retVal = E_NOT_OK;
   *      }
   *    }
   *    else
   *    { // ip address check fails
   *      retVal = E_NOT_OK;
   *    }
   *  }
   *   
   *  return retVal;
   */
}

/**********************************************************************************************************************
 *  END OF FILE: Appl_DoIP.c
 *********************************************************************************************************************/
