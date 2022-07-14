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
/**        \file  Appl_DoIP.h
 *        \brief  Diagnostic over Internet Protocol
 *
 *      \details  Demo application for component
 *  
 *********************************************************************************************************************/

#if ( !defined (APPL_DOIP_H) )
# define APPL_DOIP_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Platform_Types.h"
# include "Compiler_Cfg.h"
# include "DoIP_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  ApplDoIP_GetVin()
 *********************************************************************************************************************/
/*! \brief          Retrieves VIN from application.
 *  \details        -
 *  \param[in]      Vin       Pointer to buffer where the VIN shall be stored.
 *  \return         E_OK      Request is accepted.
 *  \return         E_NOT_OK  Request is not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_GetVin(
  uint8* Vin);

/**********************************************************************************************************************
 *  ApplDoIP_GetGid()
 *********************************************************************************************************************/
/*! \brief          Retrieves GID from application.
 *  \details        -
 *  \param[in]      GroupId   Pointer to buffer where the GID shall be stored.
 *  \return         E_OK      Request is accepted.
 *  \return         E_NOT_OK  Request is not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_GetGid(
  uint8* GroupId);

/**********************************************************************************************************************
 *  ApplDoIP_TriggerGidSync()
 *********************************************************************************************************************/
/*! \brief          Triggers GID synchronization at application.
 *  \details        -
 *  \return         E_OK      GroupIdentifier Synchronization was triggered.
 *  \return         E_NOT_OK  GroupIdentifier Synchronization could not be triggered so try again next MainFunction.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_TriggerGidSync(void);

/**********************************************************************************************************************
 *  ApplDoIP_GetDiagPowerMode()
 *********************************************************************************************************************/
/*! \brief          Retrieves power mode from application.
 *  \details        -
 *  \param[in]      PowerStateReady   Pointer to buffer where the power mode shall be stored.
 *  \return         E_OK              Request is accepted.
 *  \return         E_NOT_OK          Request is not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_GetDiagPowerMode(
  DoIP_PowerStateType* PowerStateReady);

/**********************************************************************************************************************
 *  ApplDoIP_ShutdownFinished()
 *********************************************************************************************************************/
/*! \brief          Informs upper layer about finished shutdown.
 *  \details        -
 *  \return         E_OK      Request is accepted.
 *  \return         E_NOT_OK  Request is not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_ShutdownFinished(void);

/**********************************************************************************************************************
 *  ApplDoIP_GetActivationLineState()
 *********************************************************************************************************************/
/*! \brief          Retrieves activation line state from application.
 *  \details        -
 *  \param[in]      state     Pointer to buffer where activation line state shall be stored.
 *  \return         E_OK      Request is accepted.
 *  \return         E_NOT_OK  Request is not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_GetActivationLineState(
  DoIP_ActivationLineType* const state);

/**********************************************************************************************************************
 *  ApplDoIP_RoutingActivationAuthentication()
 *********************************************************************************************************************/
/*! \brief          Forwards OEM specific part for authentication of received routing activation request to application
 *                  and retrieves OEM specific part for authentication for routing activation response.
 *  \details        -
 *  \param[in]      Authentified            Indicates if authentication was successful.
 *  \param[in]      AuthenticationReqData   Pointer to OEM specific part for authentication of routing activation
 *                                          request.
 *  \param[in]      AuthenticationResData   Pointer to OEM specific part for authentication of routing activation
 *                                          response.
 *  \return         E_OK                    Authentified and AuthenticationResData contain valid Data.
 *  \return         E_NOT_OK                Authentified and/or AuthenticationResData do not contain valid information.
 *  \return         DOIP_E_PENDING          Authentication still running. Call next DoIP_MainFunction cycle again.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_RoutingActivationAuthentication(
  boolean* Authentified,
  uint8* AuthenticationReqData,
  uint8* AuthenticationResData);

/**********************************************************************************************************************
 *  ApplDoIP_RoutingActivationConfirmation()
 *********************************************************************************************************************/
/*! \brief          Forwards OEM specific part for confirmation of received routing activation request to application
 *                  and retrieves OEM specific part for confirmation for routing activation response.
 *  \details        -
 *  \param[in]      Confirmed             Indicates if confirmation was successful.
 *  \param[in]      ConfirmationReqData   Pointer to OEM specific part for confirmation of routing activation request.
 *  \param[in]      ConfirmationResData   Pointer to OEM specific part for authentication of routing activation
 *                                        response.
 *  \return         E_OK                  Confirmed and ConfirmationResData contain valid Data.
 *  \return         E_NOT_OK              Confirmed and/or ConfirmationResData do not contain valid information.
 *  \return         DOIP_E_PENDING        Confirmation still running. Call next DoIP_MainFunction cycle again.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_RoutingActivationConfirmation(
  boolean* Confirmed,
  uint8* ConfirmationReqData,
  uint8* ConfirmationResData);

/**********************************************************************************************************************
 *  ApplDoIP_OemSpecificPayloadType()
 *********************************************************************************************************************/
/*! \brief          Forwards user data of manufacturer-specific payload types to user and initiate transmission of a
 *                  response.
 *  \details        -
 *  \param[in]      RxPayloadType   Received payload type.
 *  \param[in]      RxUserData      Pointer to received user data.
 *  \param[in]      Flags           Flags indicates protocol (TCP/UDP) and routing activation state.
 *  \param[out]     TxPayloadType   Payload type for response which must not to be set if no response shall be sent.
 *  \param[out]     TxUserData      Pointer to buffer where user can store user data for response. As “in” parameter it
 *                                  it indicates the buffer size provided by DoIP and must be set to length of copied
 *                                  response data.
 *  \return         E_OK            Known payload type.
 *  \return         E_NOT_OK        Unknown payload type.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_OemSpecificPayloadType(
  uint16 RxPayloadType,
  const PduInfoType* RxUserData,
  DoIP_OemPayloadTypeFlagType Flags,
  uint16* TxPayloadType,
  PduInfoType* TxUserData);

/**********************************************************************************************************************
 *  ApplDoIP_VerifyTargetAddr()
 *********************************************************************************************************************/
/*! \brief          Forwards logical target address received within diagnostic message to user.
 *  \details        -
 *  \param[in]      TargetAddr  Target address received within diagnostic message.
 *  \return         E_OK        Target address accepted.
 *  \return         E_NOT_OK    Target address declined.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_VerifyTargetAddr(
  uint16 TargetAddr);

/**********************************************************************************************************************
 *  ApplDoIP_VerifyRxPdu()
 *********************************************************************************************************************/
/*! \brief          Forwards addresses and data received within diagnostic message to user.
 *  \details        -
 *  \param[in]      LocalAddrPtr          Pointer to the local ip address.
 *  \param[in]      RemoteAddrPtr         Pointer to the remote ip address.
 *  \param[in]      SourceAddr            Source address received within diagnostic message.
 *  \param[in]      TargetAddr            Target address received within diagnostic message.
 *  \param[in]      PduInfoPtr            Pointer to received user data (up to the configured length).
 *  \return         E_OK                  Diagnostic message accepted.
 *  \return         E_NOT_OK              Diagnostic message declined.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_VerifyRxPdu(
  SoAd_SockAddrType* LocalAddrPtr,
  SoAd_SockAddrType* RemoteAddrPtr,
  uint16 SourceAddr,
  uint16 TargetAddr,
  PduInfoType* PduInfoPtr);

#endif /* !defined (APPL_DOIP_H) */
/**********************************************************************************************************************
 *  END OF FILE: Appl_DoIP.h
 *********************************************************************************************************************/
