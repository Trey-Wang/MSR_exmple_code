/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Srp.c
 *        \brief  SRP source file
 *
 *      \details  Implementation of the Stream Registration Protocol (SRP)
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

# define SRP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Srp.h" /* PRQA S 0883 */ /* MD_Srp_19.5 */
# include "EthIf.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of Srp header file */
#if (  (SRP_SW_MAJOR_VERSION != (3u)) \
    || (SRP_SW_MINOR_VERSION != (1u)) \
    || (SRP_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of Srp.c and Srp.h are inconsistent"
#endif

/* Check configurator version generated to Srp_Cfg.h */
#if (   (SRP_CFG_MAJOR_VERSION != (0x03)) \
     || (SRP_CFG_MINOR_VERSION != (0x01)) )
# error "Version numbers of Srp.c and Srp_Cfg.h are inconsistent!"
#endif

/* PRQA S 0857 MACRO_LIMIT */ /* MD_MSR_1.1_857 */
/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 2 */ /* MD_MSR_19.7 */
# define SRP_PACK_THREE_PACKED_EVENT(Event1, Event2, Event3)    (((((Event1)*6)+(Event2))*6)+(Event3))
# define SRP_PACK_THREE_PACKED_EVENT_SINGLE(Event)              (SRP_PACK_THREE_PACKED_EVENT((Event), 0, 0))
/* PRQA S 3453 2 */ /* MD_MSR_19.7 */
# define SRP_PACK_FOUR_PACKED_EVENT(Event1, Event2, Event3, Event4) (((Event1)*64)+((Event2)*16)+((Event3)*4)+(Event4))
# define SRP_PACK_FOUR_PACKED_EVENT_SINGLE(Event)                   (SRP_PACK_FOUR_PACKED_EVENT((Event), 0, 0, 0))

/* 
 * Compose the VectorHeader containing the LeaveAllEvent(LA) and the NumberOfValues (NOV) 
 * apply Bit-masks for LA and NOV. LA << 13 equals LA * 8192
 * assemble the VectorHeader by link LA and NOV with logical or
 */
/* PRQA S 3453 2 */ /* MD_MSR_19.7 */
# define Srp_ComposeVectorHeader(LA, NOV) \
  (uint16)( ((uint16)((uint16)(LA) & 0x0001) << 13u) | ((uint16)(NOV) & 0x1FFFu) )

/* Incrementing VID and 'return' if VID is greater than 4094 (max allowed value for VID) */
# define Srp_IncrementVid(VID) \
  {  \
    (VID)++; \
    if((VID) > 4094) \
    { \
      return; \
    } \
   } /* PRQA S 3458 */ /* MD_MSR_19.4 */ /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Incrementing FirstValue of MSRP Talker Advertise/Failed */
/* PRQA S 3412 3 */ /* MD_MSR_19.4 */
# define Srp_IncrementFirstValueTalker(UniqueId, DA) \
  (UniqueId)++; \
  Srp_IncrementMacAddress((DA)); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Incrementing FirstValue of MSRP Domain */
/* PRQA S 3412 3 */ /* MD_MSR_19.4  */
# define Srp_IncrementFirstValueDomain(FirstValueDomain) \
  (FirstValueDomain).SrClassId++; \
  (FirstValueDomain).SrClassPriority++; /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* PRQA L: MACRO_LIMIT */
/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#ifndef SRP_LOCAL
# define SRP_LOCAL static
#endif

#ifndef SRP_LOCAL_INLINE
# define SRP_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define SRP_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! \brief Initialization state of the module */
#if (SRP_DEV_ERROR_DETECT == STD_ON)
SRP_LOCAL VAR(Srp_StateType, SRP_VAR_ZERO_INIT) Srp_State = SRP_STATE_UNINIT;
#endif /* (SRP_DEV_ERROR_DETECT == STD_ON) */

#define SRP_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define SRP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ----- Init Functions ------------------------------------------------- */
SRP_LOCAL FUNC(void, SRP_CODE) Srp_InitPort(
  Srp_PortIdxType PortIdx);

SRP_LOCAL FUNC(void, SRP_CODE) Srp_InitSmLeaveAll(
  Srp_PortIdxType PortIdx, SRP_P2VAR(Srp_LeaveAllStateMachineType) LeaveAllSmPtr);

SRP_LOCAL FUNC(void, SRP_CODE) Srp_InitSmPeriodicTransmission(
  SRP_P2VAR(Srp_PeridodicTransmissionStateMachineType) PeriodicTransmissionSmPtr);

SRP_LOCAL FUNC(void, SRP_CODE) Srp_InitSmApplicant(
  SRP_P2VAR(Srp_ApplicantStateMachineType) ApplicantSmPtr);

SRP_LOCAL_INLINE FUNC(void, SRP_CODE) Srp_InitSmRegistrar(
  Srp_PortIdxType PortIdx, SRP_P2VAR(Srp_RegistrarStateMachineType) RegistrarSmPtr);

/* ----- Process state machines ----------------------------------------- */
SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessSmApplicant(
  SRP_P2VAR(Srp_MrpParticipantType) MrpParticipantPtr, Srp_PortIdxType PortIdx, Srp_AttributeIdxType AttributeIdx,
  Srp_MrpEventType Event);

SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessSmRegistrar(
  SRP_P2VAR(Srp_MrpParticipantType) MrpParticipantPtr, Srp_PortIdxType PortIdx, Srp_AttributeIdxType AttributeIdx, 
  Srp_MrpEventType Event);

SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessSmLeaveAll(
  SRP_P2VAR(Srp_MrpParticipantType) MrpParticipantPtr, Srp_PortIdxType PortIdx, Srp_MrpEventType Event);

SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessSmPeriodicTransmission(
  SRP_P2VAR(Srp_MrpParticipantType) MrpParticipantPtr, Srp_PortIdxType PortIdx, Srp_MrpEventType Event);

/* ----- Process Attribute ---------------------------------------------- */
#if (SRP_MMRP_SUPPORT == STD_ON)
SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessAttributeMac(
  Srp_PortIdxType PortIdx, SRP_P2CONST(uint8) MacAddress, Srp_AttributeEventType AttributeEvent, boolean LeaveAll);

SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessAttributeServiceReq(
  Srp_PortIdxType PortIdx, Srp_MmrpServiceRequirementAttributeType ServiceReq, Srp_AttributeEventType AttributeEvent,
  boolean LeaveAll);
#endif /* (SRP_MMRP_SUPPORT == STD_ON) */

SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessAttributeVid(
  Srp_PortIdxType PortIdx, Srp_VLanIdType VID, Srp_AttributeEventType AttributeEvent, boolean LeaveAll);

SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessAttributeTalker(
  Srp_PortIdxType PortIdx, Srp_MsrpStreamInfoType StreamInfo, Srp_AttributeEventType AttributeEvent, boolean LeaveAll,
  Srp_MsrpAttributeType AttributeType);

SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessAttributeListener(
  Srp_PortIdxType PortIdx, Srp_StreamIdType StreamID, Srp_AttributeEventType AttributeEvent, boolean LeaveAll, 
  Srp_ListenerDeclarationType ListenerDeclaration);

SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessAttributeDomain(
  Srp_PortIdxType PortIdx, Srp_MsrpDomainInfoType DomainInfo, Srp_AttributeEventType AttributeEvent, boolean LeaveAll);

/* ----- Transmit functions --------------------------------------------- */
SRP_LOCAL FUNC(Std_ReturnType, SRP_CODE) Srp_AssembleMsgJoin(
  SRP_P2VAR(Srp_MrpParticipantType)MrpParticipantPtr, SRP_P2CONST(Srp_ApplicantStateMachineType) ApplicantSmPtr,
  Srp_PortIdxType PortIdx, Srp_AttributeIdxType AttributeIdx, Srp_LeaveAllType LeaveAllEvent);

SRP_LOCAL FUNC(Std_ReturnType, SRP_CODE) Srp_AssembleMsgInOrMt(
  SRP_P2VAR(Srp_MrpParticipantType)MrpParticipantPtr, SRP_P2CONST(Srp_ApplicantStateMachineType) ApplicantSmPtr,
  Srp_PortIdxType PortIdx, Srp_AttributeIdxType AttributeIdx, Srp_LeaveAllType LeaveAllEvent);

SRP_LOCAL FUNC(Std_ReturnType, SRP_CODE) Srp_Transmit(
  Srp_PortIdxType PortIdx, SRP_P2VAR(Srp_MrpParticipantType) MrpParticipantPtr, uint16 FrameType, 
  boolean TxConfirmation, const Srp_MacAddressType DestinationMac);

SRP_LOCAL FUNC(Std_ReturnType, SRP_CODE) Srp_AssembleMsg(
  SRP_P2VAR(Srp_MrpParticipantType) MrpParticipantPtr, Srp_PortIdxType PortIdx, Srp_AttributeIdxType AttributeIdx,
  Srp_LeaveAllType LeaveAllEvent, Srp_AttributeEventType AttributeEvent);

SRP_LOCAL FUNC(Std_ReturnType, SRP_CODE) Srp_RequestTransmitOpportunity(
  Srp_PortIdxType PortIdx, SRP_P2VAR(Srp_MrpParticipantType) MrpParticipantPtr);

/* ----- Unpack events -------------------------------------------------- */
SRP_LOCAL FUNC(void, SRP_CODE) Srp_UnpackThreePackedEvent(
  uint8 ThreePackedEvent, SRP_P2VAR(uint8) Event1, SRP_P2VAR(uint8) Event2, SRP_P2VAR(uint8) Event3);

SRP_LOCAL FUNC(void, SRP_CODE) Srp_UnpackFourPackedEvent(
  uint8 FourPackedEvent, SRP_P2VAR(uint8) Event1, SRP_P2VAR(uint8) Event2, 
  SRP_P2VAR(uint8) Event3, SRP_P2VAR(uint8) Event4);

/* ----- Check Attributes equality -------------------------------------- */
SRP_LOCAL FUNC(boolean, SRP_CODE) Srp_EqualsMacAddress(
  SRP_P2CONST(uint8) FirstMacAddressPtr, SRP_P2CONST(uint8) SecondMacAddressPtr);

SRP_LOCAL FUNC(boolean, SRP_CODE) Srp_EqualsStreamId(
  const Srp_StreamIdType FirstStreamId, const Srp_StreamIdType SecondStreamId);

/* ----- Check Bandwidth ------------------------------------------------ */
SRP_LOCAL_INLINE FUNC(Std_ReturnType, SRP_CODE) Srp_CheckBandwidthLimitRx(
  Srp_PortIdxType PortIdx, uint32 AdditionalBandwidth, boolean ReserveBandwidth);

SRP_LOCAL_INLINE FUNC(Std_ReturnType, SRP_CODE) Srp_CheckBandwidthLimitTx(
  Srp_PortIdxType PortIdx, uint32 AdditionalBandwidth, uint8 FramePrio, boolean ReserveBandwidth);

SRP_LOCAL_INLINE FUNC(Std_ReturnType, SRP_CODE) Srp_ReleaseBandwidthRx(
  Srp_PortIdxType PortIdx, uint32 Bandwidth);

SRP_LOCAL_INLINE FUNC(Std_ReturnType, SRP_CODE) Srp_ReleaseBandwidthTx(
  Srp_PortIdxType PortIdx, uint8 FramePrio, uint32 Bandwidth);

/* ----- Compute required bandwidth ------------------------------------- */
SRP_LOCAL_INLINE FUNC(Std_ReturnType, SRP_CODE) Srp_ComputeRequiredBandwidth(
  Srp_PortIdxType PortIdx, Srp_AttributeIdxType AttributeIdx, SRP_P2VAR(uint32) BandwidthPtr);

/* ----- Check Valid Attributes ----------------------------------------- */
SRP_LOCAL FUNC(boolean, SRP_CODE) Srp_MrpHasAtLeastOneValidAttribute(
  Srp_PortIdxType PortIdx, Srp_MrpApplicationType MrpApplicationType);

/* ----- Get Message Attributes from RxBuffer---------------------------- */
SRP_LOCAL FUNC(void, SRP_CODE) Srp_GetStreamInfo(
  SRP_P2CONST(uint8) DataPtr, SRP_P2VAR(uint16) Offset, SRP_P2VAR(Srp_MsrpStreamInfoType) StreamInfo, 
  boolean GetFailure);

/* ----- In-line functions ---------------------------------------------- */
LOCAL_INLINE FUNC(void, SRP_CODE) Srp_IterateAttributesAndProcessApplicantSm(
  SRP_P2VAR(Srp_MrpParticipantType) MrpParticipantPtr, Srp_PortIdxType PortIdx, Srp_MrpEventType Event, 
  boolean ProcessRegistrar);

LOCAL_INLINE FUNC(void, SRP_CODE) Srp_CopyMacToBuffer(
  SRP_P2CONST(uint8) MacAddressPtr, SRP_P2VAR(uint8) TxBufU8Ptr, SRP_P2VAR(uint16) Offset);

LOCAL_INLINE FUNC(void, SRP_CODE) Srp_IncrementMacAddress(
  SRP_P2VAR(uint8) MacAddressPtr);

LOCAL_INLINE FUNC(void, SRP_CODE) Srp_GetLeavAllAndNumberOfValues(
  SRP_P2CONST(uint8) DataPtr, SRP_P2VAR(uint16) Offset, SRP_P2VAR(boolean) LeaveAllEvent, 
  SRP_P2VAR(uint16) NumberOfValues);

LOCAL_INLINE FUNC(void, SRP_CODE) Srp_GetMacAddress(
  SRP_P2CONST(uint8) DataPtr, SRP_P2VAR(uint16) Offset, SRP_P2VAR(Srp_MacAddressType) MacAddress);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Srp_InitPort()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(void, SRP_CODE) Srp_InitPort(Srp_PortIdxType PortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 JoinTimer = Srp_PortConfig[PortIdx].JoinTime;         /* PRQA S 781 */ /* MD_Srp_5.6 */
  Srp_AttributeIdxType AttributeIndex;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  Srp_CheckDetErrorReturnVoid(SRP_COND_VALID_PORT(PortIdx), SRP_SID_INIT_PORT, SRP_E_INV_PARAM);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
#if (SRP_TYPE == SRP_TYPE_STATIC)
  Srp_PortState[PortIdx].UsedBandwithTx = 0u;
#endif /* (SRP_TYPE == SRP_TYPE_STATIC) */
  Srp_PortState[PortIdx].UsedBandwithRx = 0u;
  Srp_PortState[PortIdx].Enabled = FALSE;

# if (SRP_MMRP_SUPPORT == STD_ON)
  /* Init MMRP Participant */
  Srp_PortState[PortIdx].MmrpParticipant.MrpApplication = SRP_MMRP_APPLICATION;
  Srp_PortState[PortIdx].MmrpParticipant.JoinTimer = JoinTimer;
  Srp_InitSmLeaveAll(PortIdx, &Srp_PortState[PortIdx].MmrpParticipant.LeaveAllSM);
  Srp_InitSmPeriodicTransmission(&Srp_PortState[PortIdx].MmrpParticipant.PeriodicTransmissionSM);
  AttributeIndex = SRP_SUPPORTED_MMRP_ATTRIBUTES;
  while(AttributeIndex > 0u)
  {
    AttributeIndex--;
    Srp_PortState[PortIdx].MmrpParticipant.AttributeIsValid[AttributeIndex] = FALSE;
  }
  Srp_PortState[PortIdx].MmrpParticipant.BufIdx = SRP_INV_BUF_IDX;
  Srp_PortState[PortIdx].MmrpParticipant.BufPtr = NULL_PTR;
# endif /* (SRP_MMRP_SUPPORT == STD_ON) */

  /* Init MVRP Participant */
  Srp_PortState[PortIdx].MvrpParticipant.MrpApplication = SRP_MVRP_APPLICATION;
  Srp_PortState[PortIdx].MvrpParticipant.JoinTimer = JoinTimer;
  Srp_InitSmLeaveAll(PortIdx, &Srp_PortState[PortIdx].MvrpParticipant.LeaveAllSM);
  Srp_InitSmPeriodicTransmission(&Srp_PortState[PortIdx].MvrpParticipant.PeriodicTransmissionSM);
  AttributeIndex = SRP_SUPPORTED_MVRP_ATTRIBUTES;
  while(AttributeIndex > 0u)
  {
    AttributeIndex--;
    Srp_PortState[PortIdx].MvrpParticipant.AttributeIsValid[AttributeIndex] = FALSE;
  }
  Srp_PortState[PortIdx].MvrpParticipant.BufIdx = SRP_INV_BUF_IDX;
  Srp_PortState[PortIdx].MvrpParticipant.BufPtr = NULL_PTR;

  /* Init MSRP Participant */
  Srp_PortState[PortIdx].MsrpParticipant.MrpApplication = SRP_MSRP_APPLICATION;
  Srp_PortState[PortIdx].MsrpParticipant.JoinTimer = JoinTimer;
  Srp_InitSmLeaveAll(PortIdx, &Srp_PortState[PortIdx].MsrpParticipant.LeaveAllSM);
  Srp_InitSmPeriodicTransmission(&Srp_PortState[PortIdx].MsrpParticipant.PeriodicTransmissionSM);
  AttributeIndex = SRP_SUPPORTED_MSRP_ATTRIBUTES;
  while(AttributeIndex > 0u)
  {
    AttributeIndex--;
    Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[AttributeIndex] = FALSE;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIndex].ReservedRxBandwidth = 0;
  }
  Srp_PortState[PortIdx].MsrpParticipant.BufIdx = SRP_INV_BUF_IDX;
  Srp_PortState[PortIdx].MsrpParticipant.BufPtr = NULL_PTR;

  /* Init MSRP Domain Attribute */
  Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[0] = TRUE;
  Srp_PortState[PortIdx].MsrpParticipant.Attribute[0].AttributeType = SRP_MSRP_DOMAIN;
  Srp_PortState[PortIdx].MsrpParticipant.Attribute[0].DomainInfo.SrClassId = 
    Srp_PortConfig[PortIdx].DomainInfoPtr->SrClassId;
  Srp_PortState[PortIdx].MsrpParticipant.Attribute[0].DomainInfo.SrClassPriority = 
    Srp_PortConfig[PortIdx].DomainInfoPtr->SrClassPriority;
  Srp_PortState[PortIdx].MsrpParticipant.Attribute[0].DomainInfo.SrClassVid = 
    Srp_PortConfig[PortIdx].DomainInfoPtr->SrClassVid;

  /* PRQA S 310 1 */ /* MD_Srp_11.4 */
  Srp_InitSmApplicant(&Srp_PortState[PortIdx].MsrpParticipant.ApplicantSM[0]);
  /* PRQA S 310 1 */ /* MD_Srp_11.4 */
  Srp_InitSmRegistrar(PortIdx, &Srp_PortState[PortIdx].MsrpParticipant.RegistrarSM[0]);
  Srp_PortState[PortIdx].MsrpParticipant.ApplicantSM[0].RegistrarSmPtr = 
    &Srp_PortState[PortIdx].MsrpParticipant.RegistrarSM[0];

  /* PRQA S 310 2 */ /* MD_Srp_11.4 */
  Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant,
    PortIdx, 0, SRP_MRP_EVENT_NEW);

} /* Srp_InitPort() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_InitSmLeaveAll()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(void, SRP_CODE) Srp_InitSmLeaveAll(Srp_PortIdxType PortIdx, 
  SRP_P2VAR(Srp_LeaveAllStateMachineType) LeaveAllSmPtr)
{
  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  Srp_CheckDetErrorReturnVoid(SRP_COND_VALID_PORT(PortIdx), SRP_SID_INIT_LEAVE_ALL_SM, SRP_E_INV_PARAM);
  Srp_CheckDetErrorReturnVoid(SRP_COND_VALID_PTR(LeaveAllSmPtr), SRP_SID_INIT_LEAVE_ALL_SM, SRP_E_NULL_POINTER);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  LeaveAllSmPtr->State = SRP_STATE_LEAVEL_ALL_PASSIVE;
  LeaveAllSmPtr->LeaveAllTimer = Srp_PortConfig[PortIdx].LeaveAllTime;
  LeaveAllSmPtr->LeaveAllTimerRunning = TRUE;

} /* Srp_InitSmLeaveAll() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_InitSmPeriodicTransmission()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(void, SRP_CODE) Srp_InitSmPeriodicTransmission(
  SRP_P2VAR(Srp_PeridodicTransmissionStateMachineType) PeriodicTransmissionSmPtr)
{

  /* ----- Implementation ----------------------------------------------- */
  PeriodicTransmissionSmPtr->PeriodicTimer = SRP_PERIODIC_TIMER;
  PeriodicTransmissionSmPtr->State = SRP_STATE_PERIODIC_TRANSMISSION_ACTIVE;
  PeriodicTransmissionSmPtr->PeriodicTimerRunning = TRUE;

} /* Srp_InitSmPeriodicTransmission() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_InitSmApplicant()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(void, SRP_CODE) Srp_InitSmApplicant(SRP_P2VAR(Srp_ApplicantStateMachineType) ApplicantSmPtr)
{
  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  Srp_CheckDetErrorReturnVoid(SRP_COND_VALID_PTR(ApplicantSmPtr), SRP_SID_INIT_APPLICANT_SM, SRP_E_NULL_POINTER);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  ApplicantSmPtr->State = SRP_STATE_APPLICANT_VO;
  ApplicantSmPtr->RegistrarSmPtr = NULL_PTR;
  ApplicantSmPtr->SmIsValid = TRUE;
  ApplicantSmPtr->RequestTx = FALSE;

} /* Srp_InitSmApplicant() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

  /********************************************************************************************************************
 *  Srp_InitSmRegistrar()
 *********************************************************************************************************************/
SRP_LOCAL_INLINE FUNC(void, SRP_CODE) Srp_InitSmRegistrar(Srp_PortIdxType PortIdx, 
  SRP_P2VAR(Srp_RegistrarStateMachineType) RegistrarSmPtr)
{
  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  Srp_CheckDetErrorReturnVoid(SRP_COND_VALID_PORT(PortIdx), SRP_SID_INIT_REGISTRAR_SM, SRP_E_INV_PARAM);
  Srp_CheckDetErrorReturnVoid(SRP_COND_VALID_PTR(RegistrarSmPtr), SRP_SID_INIT_REGISTRAR_SM, SRP_E_NULL_POINTER);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  RegistrarSmPtr->LeaveTimer = Srp_PortConfig[PortIdx].LeaveTime;
  RegistrarSmPtr->State = SRP_STATE_REGISTRAR_MT;

} /* Srp_InitSmRegistrar() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_ProcessSmApplicant()
 *********************************************************************************************************************/
/* PRQA S 6010 STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 STCYC */ /* MD_MSR_STCYC */
SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessSmApplicant(SRP_P2VAR(Srp_MrpParticipantType) MrpParticipantPtr, 
  Srp_PortIdxType PortIdx, Srp_AttributeIdxType AttributeIdx, Srp_MrpEventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  SRP_P2VAR(Srp_ApplicantStateMachineType) ApplicantSmPtr;
  SRP_P2VAR(boolean) AttributeIsValidPtr;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  Srp_CheckDetErrorReturnVoid(SRP_COND_VALID_PTR(MrpParticipantPtr), SRP_SID_SM_APPLICANT, 
    SRP_E_NULL_POINTER);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
    /* get the right MRP Participant (MMRP, MVRP or MSRP) */
  switch(MrpParticipantPtr->MrpApplication)
  {
#if (SRP_MMRP_SUPPORT == STD_ON)
  case SRP_MMRP_APPLICATION:
    /* MMRP Participant */
    /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
    ApplicantSmPtr = &(((SRP_P2VAR(Srp_MmrpParticipantType))MrpParticipantPtr)->ApplicantSM[AttributeIdx]);
    AttributeIsValidPtr = &(((SRP_P2VAR(Srp_MmrpParticipantType))MrpParticipantPtr)->AttributeIsValid[AttributeIdx]);
    break;
#endif /* (SRP_MMRP_SUPPORT == STD_ON) */

  case SRP_MVRP_APPLICATION:
    /* MVRP Participant */
    /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
    ApplicantSmPtr = &(((SRP_P2VAR(Srp_MvrpParticipantType))MrpParticipantPtr)->ApplicantSM[AttributeIdx]);
    AttributeIsValidPtr = &(((SRP_P2VAR(Srp_MvrpParticipantType))MrpParticipantPtr)->AttributeIsValid[AttributeIdx]);
    break;

  case SRP_MSRP_APPLICATION:
    /* MSRP Participant */
    /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
    ApplicantSmPtr = &(((SRP_P2VAR(Srp_MsrpParticipantType))MrpParticipantPtr)->ApplicantSM[AttributeIdx]);
    AttributeIsValidPtr = &(((SRP_P2VAR(Srp_MsrpParticipantType))MrpParticipantPtr)->AttributeIsValid[AttributeIdx]);
    break;

  default:
    /* invalid MRP Application */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_SM_APPLICANT, SRP_E_INV_MRP_APPLICATION);
    return;
  }

  /* check if state machine is valid */
  if((ApplicantSmPtr->SmIsValid == FALSE) && (Event != SRP_MRP_EVENT_BEGIN))
  {
    return;
  }

  /* reset RequestTx */
  if(Event == SRP_MRP_EVENT_TX)
  {
    ApplicantSmPtr->RequestTx = FALSE;
  }

/* Process the Event */
  switch(Event)
  {
  case SRP_MRP_EVENT_BEGIN:
    Srp_InitSmApplicant(ApplicantSmPtr);
    break;
  case SRP_MRP_EVENT_NEW:
    if(ApplicantSmPtr->State != SRP_STATE_APPLICANT_AN)
    {
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_VN;
      /* Request transmit opportunity */
      ApplicantSmPtr->RequestTx = TRUE;
    }
    break;
  case SRP_MRP_EVENT_JOIN:
    switch(ApplicantSmPtr->State)
    {
    case SRP_STATE_APPLICANT_VO:
    case SRP_STATE_APPLICANT_LO:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_VP;
      /* Request transmit opportunity */
      ApplicantSmPtr->RequestTx = TRUE;
      break;
    case SRP_STATE_APPLICANT_LA:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_AA;
      /* Request transmit opportunity */
      ApplicantSmPtr->RequestTx = TRUE;
      break;
    case SRP_STATE_APPLICANT_AO:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_AP;
      break;
    case SRP_STATE_APPLICANT_QO:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_QP;
      break;
    default:
      /* do nothing */
      break;
    }
    break;
  case SRP_MRP_EVENT_LV:
    switch(ApplicantSmPtr->State)
    {
    case SRP_STATE_APPLICANT_VP:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_VO;
      /* when entering VO, set state machine to invalid */
      if(ApplicantSmPtr->RegistrarSmPtr->State == SRP_STATE_REGISTRAR_MT)
      {
        *AttributeIsValidPtr = FALSE;
      }
      else
      {
        ApplicantSmPtr->SmIsValid = FALSE;
      }
      break;
    case SRP_STATE_APPLICANT_VN:
    case SRP_STATE_APPLICANT_AN:
    case SRP_STATE_APPLICANT_AA:
    case SRP_STATE_APPLICANT_QA:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_LA;
      /* Request transmit opportunity */
      ApplicantSmPtr->RequestTx = TRUE;
      break;
    case SRP_STATE_APPLICANT_AP:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_AO;
      /* when entering AO, set state machine to invalid */
      if(ApplicantSmPtr->RegistrarSmPtr->State == SRP_STATE_REGISTRAR_MT)
      {
        *AttributeIsValidPtr = FALSE;
      }
      else
      {
        ApplicantSmPtr->SmIsValid = FALSE;
      }
      break;
    case SRP_STATE_APPLICANT_QP:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_QO;
      /* when entering QO, set state machine to invalid */
      if(ApplicantSmPtr->RegistrarSmPtr->State == SRP_STATE_REGISTRAR_MT)
      {
        *AttributeIsValidPtr = FALSE;
      }
      else
      {
        ApplicantSmPtr->SmIsValid = FALSE;
      }
      break;
    default:
      /* do nothing */
      break;
    }
    break;
  case SRP_MRP_EVENT_R_NEW:
    /* no action, but valid event */
    break;
  case SRP_MRP_EVENT_R_JOIN_IN:
    switch(ApplicantSmPtr->State)
    {
    case SRP_STATE_APPLICANT_VO:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_AO;
      /* when entering AO, set state machine to invalid */
      if(ApplicantSmPtr->RegistrarSmPtr->State == SRP_STATE_REGISTRAR_MT)
      {
        *AttributeIsValidPtr = FALSE;
      }
      else
      {
        ApplicantSmPtr->SmIsValid = FALSE;
      }
      break;
    case SRP_STATE_APPLICANT_VP:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_AP;
      break;
    case SRP_STATE_APPLICANT_AA:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_QA;
      break;
    case SRP_STATE_APPLICANT_AO:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_QO;
      /* when entering QO, set state machine to invalid */
      if(ApplicantSmPtr->RegistrarSmPtr->State == SRP_STATE_REGISTRAR_MT)
      {
        *AttributeIsValidPtr = FALSE;
      }
      else
      {
        ApplicantSmPtr->SmIsValid = FALSE;
      }
      break;
    case SRP_STATE_APPLICANT_AP:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_QP;
      break;
    default:
      /* do nothing */
      break;
    }
    break;
  case SRP_MRP_EVENT_R_IN:
    switch(ApplicantSmPtr->State)
    {
    case SRP_STATE_APPLICANT_AA:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_QA;
      break;
    default: /* do nothing */
      break;
    }
    break;
  case SRP_MRP_EVENT_R_JOIN_MT:
  case SRP_MRP_EVENT_R_MT:
    switch(ApplicantSmPtr->State)
    {
    case SRP_STATE_APPLICANT_QA:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_AA;
      /* Request transmit opportunity */
      ApplicantSmPtr->RequestTx = TRUE;
      break;
    case SRP_STATE_APPLICANT_QO:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_AO;
      /* when entering AO, set state machine to invalid */
      if(ApplicantSmPtr->RegistrarSmPtr->State == SRP_STATE_REGISTRAR_MT)
      {
        *AttributeIsValidPtr = FALSE;
      }
      else
      {
        ApplicantSmPtr->SmIsValid = FALSE;
      }
      break;
    case SRP_STATE_APPLICANT_QP:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_AP;
      break;
    case SRP_STATE_APPLICANT_LO:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_VO;
      /* when entering VO, set state machine to invalid */
      if(ApplicantSmPtr->RegistrarSmPtr->State == SRP_STATE_REGISTRAR_MT)
      {
        *AttributeIsValidPtr = FALSE;
      }
      else
      {
        ApplicantSmPtr->SmIsValid = FALSE;
      }
      break;
    default:
      /* do nothing */
      break;
    }
    break;
  case SRP_MRP_EVENT_R_LV:
  case SRP_MRP_EVENT_R_LA:
  case SRP_MRP_EVENT_RE_DECLARE:
    switch(ApplicantSmPtr->State)
    {
    case SRP_STATE_APPLICANT_VO:
    case SRP_STATE_APPLICANT_AO:
    case SRP_STATE_APPLICANT_QO:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_LO;
      break;
    case SRP_STATE_APPLICANT_AN:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_VN;
      /* Request transmit opportunity */
      ApplicantSmPtr->RequestTx = TRUE;
      break;
    case SRP_STATE_APPLICANT_AA:
    case SRP_STATE_APPLICANT_QA:
    case SRP_STATE_APPLICANT_AP:
    case SRP_STATE_APPLICANT_QP:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_VP;
      /* Request transmit opportunity */
      ApplicantSmPtr->RequestTx = TRUE;
      break;
    default: /* do nothing */
      break;
    }
    break;
  case SRP_MRP_EVENT_PERIODIC:
    switch(ApplicantSmPtr->State)
    {
    case SRP_STATE_APPLICANT_QA:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_AA;
      /* Request transmit opportunity */
      ApplicantSmPtr->RequestTx = TRUE;
      break;
    case SRP_STATE_APPLICANT_QP:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_AP;
      /* Request transmit opportunity */
      ApplicantSmPtr->RequestTx = TRUE;
      break;
    default: /* do nothing */
      break;
    }
    break;
  case SRP_MRP_EVENT_TX:
    switch(ApplicantSmPtr->State)
    {
    case SRP_STATE_APPLICANT_VO:
    case SRP_STATE_APPLICANT_AO:
    case SRP_STATE_APPLICANT_QO:
    case SRP_STATE_APPLICANT_QP:
      if(Srp_AssembleMsgInOrMt(MrpParticipantPtr, ApplicantSmPtr, PortIdx, AttributeIdx, SRP_LEAVE_ALL_EVENT_NULL)
        != E_OK)
      {
        ApplicantSmPtr->RequestTx = TRUE;
      }
      break;
    case SRP_STATE_APPLICANT_VP:
      if(Srp_AssembleMsgJoin(MrpParticipantPtr, ApplicantSmPtr, PortIdx, AttributeIdx, SRP_LEAVE_ALL_EVENT_NULL)
        != E_OK)
      {
        ApplicantSmPtr->RequestTx = TRUE;
      }
      else
      {
        ApplicantSmPtr->State = SRP_STATE_APPLICANT_AA;
      }
      break;
    case SRP_STATE_APPLICANT_VN:
      if(Srp_AssembleMsg(MrpParticipantPtr, PortIdx, AttributeIdx, SRP_LEAVE_ALL_EVENT_NULL, SRP_ATTRIBUTE_EVENT_NEW)
        != E_OK)
      {
        ApplicantSmPtr->RequestTx = TRUE;
      }
      else
      {
        ApplicantSmPtr->State = SRP_STATE_APPLICANT_AN;
      }
      break;
    case SRP_STATE_APPLICANT_AN:
      if(Srp_AssembleMsg(MrpParticipantPtr, PortIdx, AttributeIdx, SRP_LEAVE_ALL_EVENT_NULL, SRP_ATTRIBUTE_EVENT_NEW)
        != E_OK)
      {
        ApplicantSmPtr->RequestTx = TRUE;
      }
      else
      {
        ApplicantSmPtr->State = SRP_STATE_APPLICANT_QA;
      }
      break;
    case SRP_STATE_APPLICANT_AA:
    case SRP_STATE_APPLICANT_AP:
      if(Srp_AssembleMsgJoin(MrpParticipantPtr, ApplicantSmPtr, PortIdx, AttributeIdx, SRP_LEAVE_ALL_EVENT_NULL)
        != E_OK)
      {
        ApplicantSmPtr->RequestTx = TRUE;
      }
      else
      {
        ApplicantSmPtr->State = SRP_STATE_APPLICANT_QA;
      }
      break;
    case SRP_STATE_APPLICANT_QA:
      if(Srp_AssembleMsgJoin(MrpParticipantPtr, ApplicantSmPtr, PortIdx, AttributeIdx, SRP_LEAVE_ALL_EVENT_NULL)
        != E_OK)
      {
        ApplicantSmPtr->RequestTx = TRUE;
      }
      break;
    case SRP_STATE_APPLICANT_LA:
      if(Srp_AssembleMsg(MrpParticipantPtr, PortIdx, AttributeIdx, SRP_LEAVE_ALL_EVENT_NULL, SRP_ATTRIBUTE_EVENT_LV)
        != E_OK)
      {
        ApplicantSmPtr->RequestTx = TRUE;
      }
      else
      {
        ApplicantSmPtr->State = SRP_STATE_APPLICANT_VO;
        if(ApplicantSmPtr->RegistrarSmPtr->State == SRP_STATE_REGISTRAR_MT)
        {
          *AttributeIsValidPtr = FALSE;
        }
        else
        {
          ApplicantSmPtr->SmIsValid = FALSE;
        }
      }
      break;
    case SRP_STATE_APPLICANT_LO:
      if(Srp_AssembleMsgInOrMt(MrpParticipantPtr, ApplicantSmPtr, PortIdx, AttributeIdx, SRP_LEAVE_ALL_EVENT_NULL)
        != E_OK)
      {
        ApplicantSmPtr->RequestTx = TRUE;
      }
      else
      {
        ApplicantSmPtr->State = SRP_STATE_APPLICANT_VO;
        if(ApplicantSmPtr->RegistrarSmPtr->State == SRP_STATE_REGISTRAR_MT)
        {
          *AttributeIsValidPtr = FALSE;
        }
        else
        {
          ApplicantSmPtr->SmIsValid = FALSE;
        }
      }
      break;
    default: /* do nothing */
      break;
    }
    break;
  case SRP_MRP_EVENT_TX_LA:
    switch(ApplicantSmPtr->State)
    {
    case SRP_STATE_APPLICANT_VO:
    case SRP_STATE_APPLICANT_LA:
    case SRP_STATE_APPLICANT_AO:
    case SRP_STATE_APPLICANT_QO:
      if(Srp_AssembleMsgInOrMt(MrpParticipantPtr, ApplicantSmPtr, PortIdx, AttributeIdx, SRP_LEAVE_ALL_EVENT) != E_OK)
      {
        ApplicantSmPtr->RequestTx = TRUE;
      }
      else
      {
        ApplicantSmPtr->State = SRP_STATE_APPLICANT_LO;
      }
      break;
    case SRP_STATE_APPLICANT_LO:
      if(Srp_AssembleMsgInOrMt(MrpParticipantPtr, ApplicantSmPtr, PortIdx, AttributeIdx, SRP_LEAVE_ALL_EVENT) != E_OK)
      {
        ApplicantSmPtr->RequestTx = TRUE;
      }
      break;
    case SRP_STATE_APPLICANT_VP:
      if(Srp_AssembleMsgInOrMt(MrpParticipantPtr, ApplicantSmPtr, PortIdx, AttributeIdx, SRP_LEAVE_ALL_EVENT) != E_OK)
      {
        ApplicantSmPtr->RequestTx = TRUE;
      }
      else
      {
        ApplicantSmPtr->State = SRP_STATE_APPLICANT_AA;
      }
      break;
    case SRP_STATE_APPLICANT_VN:
      if(Srp_AssembleMsg(MrpParticipantPtr, PortIdx, AttributeIdx, SRP_LEAVE_ALL_EVENT, SRP_ATTRIBUTE_EVENT_NEW) 
        != E_OK)
      {
        ApplicantSmPtr->RequestTx = TRUE;
      }
      else
      {
        ApplicantSmPtr->State = SRP_STATE_APPLICANT_AN;
      }
      break;
    case SRP_STATE_APPLICANT_AN:
      if(Srp_AssembleMsg(MrpParticipantPtr, PortIdx, AttributeIdx, SRP_LEAVE_ALL_EVENT, SRP_ATTRIBUTE_EVENT_NEW)
        != E_OK)
      {
        ApplicantSmPtr->RequestTx = TRUE;
      }
      else
      {
        ApplicantSmPtr->State = SRP_STATE_APPLICANT_QA;
      }
      break;
    case SRP_STATE_APPLICANT_AA:
    case SRP_STATE_APPLICANT_AP:
    case SRP_STATE_APPLICANT_QP:
      if(Srp_AssembleMsgJoin(MrpParticipantPtr, ApplicantSmPtr, PortIdx, AttributeIdx, SRP_LEAVE_ALL_EVENT) != E_OK)
      {
        ApplicantSmPtr->RequestTx = TRUE;
      }
      else
      {
        ApplicantSmPtr->State = SRP_STATE_APPLICANT_QA;
      }
      break;
    case SRP_STATE_APPLICANT_QA:
      if(Srp_AssembleMsgJoin(MrpParticipantPtr, ApplicantSmPtr, PortIdx, AttributeIdx, SRP_LEAVE_ALL_EVENT) != E_OK)
      {
        ApplicantSmPtr->RequestTx = TRUE;
      }
      break;
    default: /* do nothing */
      break;
    }
    break;
  case SRP_MRP_EVENT_TX_LAF:
    switch(ApplicantSmPtr->State)
    {
    case SRP_STATE_APPLICANT_VO:
    case SRP_STATE_APPLICANT_LA:
    case SRP_STATE_APPLICANT_AO:
    case SRP_STATE_APPLICANT_QO:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_LO;
      break;
    case SRP_STATE_APPLICANT_VP:
    case SRP_STATE_APPLICANT_AP:
    case SRP_STATE_APPLICANT_AA:
    case SRP_STATE_APPLICANT_QA:
    case SRP_STATE_APPLICANT_QP:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_VP;
      /* Request transmit opportunity */
      ApplicantSmPtr->RequestTx = TRUE;
      break;
    case SRP_STATE_APPLICANT_VN:
    case SRP_STATE_APPLICANT_AN:
      ApplicantSmPtr->State = SRP_STATE_APPLICANT_VN;
      /* Request transmit opportunity */
      ApplicantSmPtr->RequestTx = TRUE;
      break;
    default: /* do nothing */
      break;
    }
    break;
  default:
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_SM_APPLICANT, SRP_E_INV_EVENT);
    break;
  }

} /* Srp_ProcessSmApplicant() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
/* PRQA L: STPTH */
/* PRQA L: STCYC */

/**********************************************************************************************************************
 *  Srp_ProcessSmRegistrar()
 *********************************************************************************************************************/
/* PRQA S 6010 STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 STMIF */ /* MD_MSR_STMIF */
SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessSmRegistrar(SRP_P2VAR(Srp_MrpParticipantType) MrpParticipantPtr,
  Srp_PortIdxType PortIdx, Srp_AttributeIdxType AttributeIdx, Srp_MrpEventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  SRP_P2VAR(Srp_RegistrarStateMachineType) RegistrarSmPtr; /* PRQA S 0781 */ /* MD_Srp_5.6 */
  Srp_MsrpAttributeInfoType AttributeInfo = 
  {
    0,
    SRP_INV_PORT_IDX,
    NULL_PTR
  };

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  Srp_CheckDetErrorReturnVoid(SRP_COND_VALID_PTR(MrpParticipantPtr), SRP_SID_SM_REGISTRAR,
    SRP_E_NULL_POINTER);
  Srp_CheckDetErrorReturnVoid(SRP_COND_VALID_PORT(PortIdx), SRP_SID_SM_REGISTRAR, SRP_E_INV_PARAM);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  /* get right MRP Participant (MMRP, MVRP or MSRP) */
  switch(MrpParticipantPtr->MrpApplication)
  {
# if (SRP_MMRP_SUPPORT == STD_ON)
  case SRP_MMRP_APPLICATION:
    /* MMRP Participant */
    /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
    RegistrarSmPtr = &(((SRP_P2VAR(Srp_MmrpParticipantType))MrpParticipantPtr)->RegistrarSM[AttributeIdx]);
    break;
# endif /* (SRP_MMRP_SUPPORT == STD_ON) */

  case SRP_MVRP_APPLICATION:
    /* MVRP Participant */
    /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
    RegistrarSmPtr = &(((SRP_P2VAR(Srp_MvrpParticipantType))MrpParticipantPtr)->RegistrarSM[AttributeIdx]);
    break;

  case SRP_MSRP_APPLICATION:
    /* MSRP Participant */
    /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
    RegistrarSmPtr = &(((SRP_P2VAR(Srp_MsrpParticipantType))MrpParticipantPtr)->RegistrarSM[AttributeIdx]);
    break;

  default:
    /* invalid MRP Application */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_SM_REGISTRAR, SRP_E_INV_MRP_APPLICATION);
    return;
  }

  switch(Event)
  {
  case SRP_MRP_EVENT_BEGIN:
    Srp_InitSmRegistrar(PortIdx, RegistrarSmPtr);
    break;
  case SRP_MRP_EVENT_R_NEW:
    switch(RegistrarSmPtr->State)
    {
    case SRP_STATE_REGISTRAR_LV:
      RegistrarSmPtr->LeaveTimerRunning = FALSE; /*lint -e616 */
    case SRP_STATE_REGISTRAR_IN: /* PRQA S 2003 */ /* MD_Srp_15.2 */
    case SRP_STATE_REGISTRAR_MT: 
      RegistrarSmPtr->State = SRP_STATE_REGISTRAR_IN; /*lint +e616 */
      if(MrpParticipantPtr->MrpApplication == SRP_MSRP_APPLICATION)
      {
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        SRP_P2VAR(Srp_MsrpParticipantType) MsrpParticipantPtr = (SRP_P2VAR(Srp_MsrpParticipantType)) MrpParticipantPtr;
        /* Set Attribute Infos */
        AttributeInfo.AttributeType = MsrpParticipantPtr->Attribute[AttributeIdx].AttributeType;
        AttributeInfo.PortIdx = PortIdx;
        AttributeInfo.StreamInfoPtr = &MsrpParticipantPtr->Attribute[AttributeIdx].StreamInfo;

        if((MsrpParticipantPtr->Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_ADVERTISE) || 
          (MsrpParticipantPtr->Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_FAILED))
        {
          Srp_PortConfig[PortIdx].RegisterStreamCbkPtr(&AttributeInfo);
        }
        else if(MsrpParticipantPtr->Attribute[AttributeIdx].AttributeType == SRP_MSRP_LISTENER)
        {
          Srp_PortConfig[PortIdx].RegisterAttachCbkPtr(&AttributeInfo);
        }
        else
        {
          /* do nothing */
        }
      }
      break;
    default:
      break;
    }
    break;
  case SRP_MRP_EVENT_R_JOIN_IN:
  case SRP_MRP_EVENT_R_JOIN_MT:
    switch(RegistrarSmPtr->State)
    {
    case SRP_STATE_REGISTRAR_LV:
      RegistrarSmPtr->LeaveTimerRunning = FALSE;
      break;
    case SRP_STATE_REGISTRAR_MT:
      if(MrpParticipantPtr->MrpApplication == SRP_MSRP_APPLICATION)
      {
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        SRP_P2VAR(Srp_MsrpParticipantType) MsrpParticipantPtr = (SRP_P2VAR(Srp_MsrpParticipantType)) MrpParticipantPtr;
        /* Set Attribute Infos */
        AttributeInfo.AttributeType = MsrpParticipantPtr->Attribute[AttributeIdx].AttributeType;
        AttributeInfo.PortIdx = PortIdx;
        AttributeInfo.StreamInfoPtr = &MsrpParticipantPtr->Attribute[AttributeIdx].StreamInfo;

        if((MsrpParticipantPtr->Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_ADVERTISE) || 
          (MsrpParticipantPtr->Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_FAILED))
        {
          Srp_PortConfig[PortIdx].RegisterStreamCbkPtr(&AttributeInfo);
        }
        else if(MsrpParticipantPtr->Attribute[AttributeIdx].AttributeType == SRP_MSRP_LISTENER)
        {
          Srp_PortConfig[PortIdx].RegisterAttachCbkPtr(&AttributeInfo);
        }
        else
        {
          /* do nothing */
        }
      }
      break;
    default:
      break;
    }
    RegistrarSmPtr->State = SRP_STATE_REGISTRAR_IN;
    break;
  case SRP_MRP_EVENT_R_LV:
  case SRP_MRP_EVENT_R_LA:
  case SRP_MRP_EVENT_TX_LA:
  case SRP_MRP_EVENT_RE_DECLARE:
    switch(RegistrarSmPtr->State)
    {
    case SRP_STATE_REGISTRAR_IN:
      RegistrarSmPtr->State = SRP_STATE_REGISTRAR_LV;
      RegistrarSmPtr->LeaveTimer = Srp_PortConfig[PortIdx].LeaveTime;
      RegistrarSmPtr->LeaveTimerRunning = TRUE;
      break;
    default:
      break;
    }
    break;
  case SRP_MRP_EVENT_FLUSH:
    switch(RegistrarSmPtr->State)
    {
    case SRP_STATE_REGISTRAR_LV:
      if(MrpParticipantPtr->MrpApplication == SRP_MSRP_APPLICATION)
      {
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        SRP_P2VAR(Srp_MsrpParticipantType) MsrpParticipantPtr = (SRP_P2VAR(Srp_MsrpParticipantType)) MrpParticipantPtr;
        /* Set Attribute Infos */
        AttributeInfo.AttributeType = MsrpParticipantPtr->Attribute[AttributeIdx].AttributeType;
        AttributeInfo.PortIdx = PortIdx;
        AttributeInfo.StreamInfoPtr = &MsrpParticipantPtr->Attribute[AttributeIdx].StreamInfo;

        if((MsrpParticipantPtr->Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_ADVERTISE) || 
          (MsrpParticipantPtr->Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_FAILED))
        {
          Srp_PortConfig[PortIdx].DeregisterStreamCbkPtr(&AttributeInfo);
        }
        else if(MsrpParticipantPtr->Attribute[AttributeIdx].AttributeType == SRP_MSRP_LISTENER)
        {
          Srp_PortConfig[PortIdx].DeregisterAttachCbkPtr(&AttributeInfo);
        }
        else
        {
          /* do nothing */
        }
      }
      break;
    default:
      break;
    }
    RegistrarSmPtr->State = SRP_STATE_REGISTRAR_MT;
    break;
  case SRP_MRP_EVENT_LEAVE_TIMER:
    switch(RegistrarSmPtr->State)
    {
    case SRP_STATE_REGISTRAR_LV:
      if(MrpParticipantPtr->MrpApplication == SRP_MSRP_APPLICATION)
      {
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        SRP_P2VAR(Srp_MsrpParticipantType) MsrpParticipantPtr = (SRP_P2VAR(Srp_MsrpParticipantType)) MrpParticipantPtr;
        /* Set Attribute Infos */
        AttributeInfo.AttributeType = MsrpParticipantPtr->Attribute[AttributeIdx].AttributeType;
        AttributeInfo.PortIdx = PortIdx;
        AttributeInfo.StreamInfoPtr = &MsrpParticipantPtr->Attribute[AttributeIdx].StreamInfo;

        if((MsrpParticipantPtr->Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_ADVERTISE) || 
          (MsrpParticipantPtr->Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_FAILED))
        {
          Srp_PortConfig[PortIdx].DeregisterStreamCbkPtr(&AttributeInfo);
        }
        else if(MsrpParticipantPtr->Attribute[AttributeIdx].AttributeType == SRP_MSRP_LISTENER)
        {
          Srp_PortConfig[PortIdx].DeregisterAttachCbkPtr(&AttributeInfo);
        }
        else
        {
          /* do nothing */
        }
      }
      /*lint -e616 */
    case SRP_STATE_REGISTRAR_MT: /* PRQA S 2003 */ /* MD_Srp_15.2 */
      RegistrarSmPtr->State = SRP_STATE_REGISTRAR_MT; /*lint +e616 */
      break;
    default:
      break;
    }
    break;
  default:
    /* invalid event */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_SM_REGISTRAR, SRP_E_INV_EVENT);
    break;
  }
} /* Srp_ProcessSmRegistrar() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
/* PRQA L: STPTH */
/* PRQA L: STCYC */
/* PRQA L: STMIF */

/**********************************************************************************************************************
 *  Srp_ProcessSmLeaveAll()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessSmLeaveAll(SRP_P2VAR(Srp_MrpParticipantType) MrpParticipantPtr, 
  Srp_PortIdxType PortIdx, Srp_MrpEventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  SRP_P2VAR(Srp_LeaveAllStateMachineType) LeaveAllSmPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* get LeaveAll state machine */
  LeaveAllSmPtr = &MrpParticipantPtr->LeaveAllSM;

  switch(Event)
  {
  case SRP_MRP_EVENT_BEGIN:
    Srp_InitSmLeaveAll(PortIdx, LeaveAllSmPtr); /*lint -e616 */
  case SRP_MRP_EVENT_R_LA: /* PRQA S 2003 */ /* MD_Srp_15.2 */
    LeaveAllSmPtr->State = SRP_STATE_LEAVEL_ALL_PASSIVE; /*lint +e616 */
    LeaveAllSmPtr->LeaveAllTimer = Srp_PortConfig[PortIdx].LeaveAllTime;
    LeaveAllSmPtr->LeaveAllTimerRunning = TRUE;
    break;
  case SRP_MRP_EVENT_TX:
    switch(LeaveAllSmPtr->State)
    {
    case SRP_STATE_LEAVEL_ALL_ACTIVE:
      LeaveAllSmPtr->State = SRP_STATE_LEAVEL_ALL_PASSIVE;
      /* trigger txLA! event (sLA) for all valid Attributes */
      Srp_IterateAttributesAndProcessApplicantSm(MrpParticipantPtr, PortIdx, SRP_MRP_EVENT_TX_LA, FALSE);
      break;
    default: break;
    }
    break;
  case SRP_MRP_EVENT_LEAVE_ALL_TIMER:
    LeaveAllSmPtr->State = SRP_STATE_LEAVEL_ALL_ACTIVE;
    LeaveAllSmPtr->LeaveAllTimer = Srp_PortConfig[PortIdx].LeaveAllTime;
    LeaveAllSmPtr->LeaveAllTimerRunning = TRUE;
    /* trigger rLA! event for Applicant state machines */
    Srp_IterateAttributesAndProcessApplicantSm(MrpParticipantPtr, PortIdx, SRP_MRP_EVENT_R_LA, TRUE);
    break;
  default:
    break;
  }
} /* Srp_ProcessSmLeaveAll() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_ProcessSmPeriodicTransmission()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessSmPeriodicTransmission(SRP_P2VAR(Srp_MrpParticipantType) MrpParticipantPtr, 
  Srp_PortIdxType PortIdx, Srp_MrpEventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  SRP_P2VAR(Srp_PeridodicTransmissionStateMachineType) PeriodicTransmissionSmPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* get Periodic Transmission state machine */
  PeriodicTransmissionSmPtr = &MrpParticipantPtr->PeriodicTransmissionSM;
  switch(Event)
  {
  case SRP_MRP_EVENT_BEGIN:
    Srp_InitSmPeriodicTransmission(PeriodicTransmissionSmPtr);
    break;
  case SRP_MRP_EVENT_PERIODIC_TIMER:
    switch(PeriodicTransmissionSmPtr->State)
    {
    case SRP_STATE_PERIODIC_TRANSMISSION_ACTIVE:
      PeriodicTransmissionSmPtr->PeriodicTimer = SRP_PERIODIC_TIMER;
      PeriodicTransmissionSmPtr->PeriodicTimerRunning = TRUE;
      PeriodicTransmissionSmPtr->State = SRP_STATE_PERIODIC_TRANSMISSION_ACTIVE;
      /* generate periodic! event for all valid attributes of the MRP Participants */
      Srp_IterateAttributesAndProcessApplicantSm(MrpParticipantPtr, PortIdx, SRP_MRP_EVENT_PERIODIC, FALSE);
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
} /* Srp_ProcessSmPeriodicTransmission() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_Transmit()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(Std_ReturnType, SRP_CODE) Srp_Transmit(Srp_PortIdxType PortIdx, 
  SRP_P2VAR(Srp_MrpParticipantType) MrpParticipantPtr, uint16 FrameType, boolean TxConfirmation, 
  const Srp_MacAddressType DestinationMac)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if(MrpParticipantPtr->BufIdx == SRP_INV_BUF_IDX)
  {
    /* invalid buffer index */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_TRANSMIT, SRP_E_INV_BUF_IDX);
    return RetVal;
  }

  if(MrpParticipantPtr->BufPtr == NULL_PTR)
  {
    /* invalid buffer pointer */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_TRANSMIT, SRP_E_INV_BUF_PTR);
    return RetVal;
  }

  /* add EndMark if possible */
  if((MrpParticipantPtr->UsedLenByte + SRP_END_MARK_LENGTH) < MrpParticipantPtr->LenByte)
  {
    /* enough room for End Mark */
    /* PRQA S 0310 2 */ /* MD_Srp_11.4 */ /*lint -e572 */
    *((uint16*)&MrpParticipantPtr->BufPtr[MrpParticipantPtr->UsedLenByte]) = 
      (uint16)Srp_Htons(SRP_END_MARK); /*lint +e572 */
    MrpParticipantPtr->UsedLenByte += SRP_END_MARK_LENGTH;
  }

  RetVal = EthIf_Transmit(Srp_PortConfig[PortIdx].CtrlIdx, MrpParticipantPtr->BufIdx, FrameType, TxConfirmation, 
    MrpParticipantPtr->UsedLenByte, (SRP_P2VAR(uint8))DestinationMac); /* PRQA S 0310 */ /* MD_Srp_11.4 */

  if(RetVal == E_OK)
  {
    MrpParticipantPtr->BufIdx = SRP_INV_BUF_IDX;
    MrpParticipantPtr->BufPtr = NULL_PTR;
    MrpParticipantPtr->LenByte = 0;
  }
  else
  {
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_TRANSMIT, SRP_E_TX_FAILED);
  }

  return RetVal;
} /* Srp_Transmit() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_AssembleMsg()
 *********************************************************************************************************************/
/* PRQA S 6030 STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6060 STPAR */ /* MD_MSR_STPAR */
/* PRQA S 6080 STMIF */ /* MD_MSR_STMIF */
/* PRQA S 6010 STPTH */ /* MD_MSR_STPTH */
SRP_LOCAL FUNC(Std_ReturnType, SRP_CODE) Srp_AssembleMsg(SRP_P2VAR(Srp_MrpParticipantType) MrpParticipantPtr,
  Srp_PortIdxType PortIdx, Srp_AttributeIdxType AttributeIdx, Srp_LeaveAllType LeaveAllEvent, 
  Srp_AttributeEventType AttributeEvent)
{
  /* ----- Local Variables ---------------------------------------------- */
  SRP_P2VAR(uint8) TxBufU8Ptr;
  uint8 BufIdx; /* PRQA S 0781 */ /* MD_Srp_5.6 */
  uint16 LenByte; /* PRQA S 0781 */ /* MD_Srp_5.6 */
  uint16 Offset;
  uint16 AttributeListLength; /* PRQA S 0781 */ /* MD_Srp_5.6 */

  /* ----- Implementation ----------------------------------------------- */
  if((MrpParticipantPtr->BufIdx == SRP_INV_BUF_IDX) || (MrpParticipantPtr->BufPtr == NULL_PTR) ||
    (MrpParticipantPtr->LenByte == 0u))
  {
    /* No TxBuffer available */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_ASSEMBLE_MSG, SRP_E_NO_TX_BUFFER);
    return E_NOT_OK;
  }
  else
  {
    BufIdx = MrpParticipantPtr->BufIdx;
    TxBufU8Ptr = MrpParticipantPtr->BufPtr;
    LenByte = MrpParticipantPtr->LenByte;
  }

  switch(MrpParticipantPtr->MrpApplication)
  {
# if (SRP_MMRP_SUPPORT == STD_ON)
    /* MMRP */
  case SRP_MMRP_APPLICATION:
    /* check whether MMRP Attribute is MacAddress or ServiceRequirement */
    if(Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].AttributeType == SRP_MMRP_MAC_TYPE)
    {
      /* MMRP MAC Vector Attribute Type */
      /* check if enough buffer is available for the message */
      if( (MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) < SRP_MMRP_MAC_MSG_LENGTH) /*lint !e574 */
      {
        /* insufficient buffer. Trigger Transmit and get new TxBuffer */
        /* if possible, add EndMark to Frame */
        if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) > SRP_END_MARK_LENGTH) /*lint !e574 */
        {
          /* add EndMark */
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_CopyUint16ToBuffer(TxBufU8Ptr, MrpParticipantPtr->UsedLenByte, SRP_END_MARK) /*lint !e572 */
        }
        if(Srp_Transmit(PortIdx, MrpParticipantPtr, SRP_MMRP_ETHER_TYPE, FALSE, Srp_MmrpDestinationMac) != E_OK)
        {
          return E_NOT_OK;
        }
        LenByte = Srp_PortConfig[PortIdx].MaxFrameSize;
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        if(EthIf_ProvideTxBuffer(Srp_PortConfig[PortIdx].CtrlIdx, SRP_MMRP_ETHER_TYPE, SRP_VLAN_PRIORITY, &BufIdx, 
          (SRP_P2VAR(Eth_DataType*)) &TxBufU8Ptr, &LenByte) != BUFREQ_OK)
        {
          return E_NOT_OK;
        }
        /* successfully allocated new TxBuffer. Store new Tx Buffer information */
        MrpParticipantPtr->BufIdx = BufIdx;
        MrpParticipantPtr->BufPtr = TxBufU8Ptr;
        MrpParticipantPtr->LenByte = LenByte;
        MrpParticipantPtr->UsedLenByte = 1;

        /* write common header to buffer */
        TxBufU8Ptr[0] = SRP_PROTOCOL_VERSION;
      }
      /* write message to buffer */
      /* set message pointers */
      {
        Offset = MrpParticipantPtr->UsedLenByte;
       
        /* set common message attributes */
        /* Attribute Type */
        TxBufU8Ptr[Offset] = SRP_MMRP_MAC_TYPE;
        Offset += 1;

        /* Attribute Length */
        TxBufU8Ptr[Offset] = SRP_MMRP_MAC_LENGTH;
        Offset += 1;

        /* set MMRP message attributes */
        /* Vector Header */
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, Srp_ComposeVectorHeader(LeaveAllEvent, SRP_NUMBER_OF_VALUES))

        /* First Value */
        TxBufU8Ptr[Offset] = 
          Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].MacAddress[0];
        Offset += 1;
        TxBufU8Ptr[Offset] = 
          Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].MacAddress[1];
        Offset += 1;
        TxBufU8Ptr[Offset] = 
          Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].MacAddress[2];
        Offset += 1;
        TxBufU8Ptr[Offset] = 
          Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].MacAddress[3];
        Offset += 1;
        TxBufU8Ptr[Offset] = 
          Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].MacAddress[4];
        Offset += 1;
        TxBufU8Ptr[Offset] = 
          Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].MacAddress[5];
        Offset += 1;

        /* Vector */
        TxBufU8Ptr[Offset] = (uint8)SRP_PACK_THREE_PACKED_EVENT_SINGLE(AttributeEvent);
        Offset += 1;

        /* End Mark */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, SRP_END_MARK) /* PRQA S 0310 */ /* MD_Srp_11.4 */ /*lint !e572 */
        /* set used length */
        MrpParticipantPtr->UsedLenByte = Offset;
      }

    }
    else if(Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].AttributeType == 
      SRP_MMRP_SERVICE_REQUIREMENT_TYPE)
    {
      /* MMRP Service Requirement Vector Attribute Type */
      /* check if enough buffer is available for the message */
      /*lint -e574 */
      if( (MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) < SRP_MMRP_SERVICE_REQ_MSG_LENGTH)
      {
        /* insufficient buffer. Trigger Transmit and get new TxBuffer */
        /* if possible, add EndMark to Frame */
        if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) > SRP_END_MARK_LENGTH) /*lint +e574 */
        {
          /* add EndMark */
          /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
          Srp_CopyUint16ToBuffer(TxBufU8Ptr, MrpParticipantPtr->UsedLenByte, SRP_END_MARK) /*lint !e572 */
        }
        if(Srp_Transmit(PortIdx, MrpParticipantPtr, SRP_MMRP_ETHER_TYPE, FALSE, Srp_MmrpDestinationMac) != E_OK)
        {
          return E_NOT_OK;
        }
        LenByte = Srp_PortConfig[PortIdx].MaxFrameSize;
        if(EthIf_ProvideTxBuffer(Srp_PortConfig[PortIdx].CtrlIdx, SRP_MMRP_ETHER_TYPE, SRP_VLAN_PRIORITY, &BufIdx, 
          (SRP_P2VAR(Eth_DataType*)) &TxBufU8Ptr, &LenByte) != BUFREQ_OK) /* PRQA S 0310 */ /* MD_Srp_11.4 */
        {
          return E_NOT_OK;
        }
        /* successfully allocated new TxBuffer. Store new Tx Buffer information */
        MrpParticipantPtr->BufIdx = BufIdx;
        MrpParticipantPtr->BufPtr = TxBufU8Ptr;
        MrpParticipantPtr->LenByte = LenByte;
        MrpParticipantPtr->UsedLenByte = 1;

        /* write common header to buffer */
        TxBufU8Ptr[0] = SRP_PROTOCOL_VERSION;
      }
      /* write message to buffer */
      {
        Offset = MrpParticipantPtr->UsedLenByte;

        /* set message pointers */
       
        /* set common message attributes */
        /* Attribute Type */
        TxBufU8Ptr[Offset] = SRP_MMRP_SERVICE_REQUIREMENT_TYPE;
        Offset += 1;

        /* Attribute Length */
        TxBufU8Ptr[Offset] = SRP_MMRP_SERVICE_REQUIREMENT_LENGTH;
        Offset += 1;
       
        /* set MMRP message attributes */
        /* Vector Header */
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, Srp_ComposeVectorHeader(LeaveAllEvent, SRP_NUMBER_OF_VALUES))

        /* First Value */
        TxBufU8Ptr[Offset] = Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].ServiceReq;
        Offset += 1;

        /* Vector */
        TxBufU8Ptr[Offset] = (uint8)SRP_PACK_THREE_PACKED_EVENT_SINGLE(AttributeEvent);
        Offset += 1;

        /* End Mark */
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, SRP_END_MARK) /*lint !e572 */

        /* set used length */
        MrpParticipantPtr->UsedLenByte = Offset;
      }
    }
    else
    {
      /* this should never happen */
      break;
    }
    break;
# endif /* (SRP_MMRP_SUPPORT == STD_ON) */
    /* MVRP */
  case SRP_MVRP_APPLICATION:
    /* check if enough TxBuffer is available */
    /*lint -e574 */
    if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) < SRP_MVRP_MSG_LENGTH)
    {
      /* insufficient TxBuffer. Trigger Transmit and allocate new TxBuffer */
      /* if possible, add EndMark to Frame */
      if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) > SRP_END_MARK_LENGTH) /*lint +e574 */
      {
        /* add EndMark */
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, MrpParticipantPtr->UsedLenByte, SRP_END_MARK) /*lint !e572 */
      }
      if(Srp_Transmit(PortIdx, MrpParticipantPtr, SRP_MVRP_ETHER_TYPE, FALSE, Srp_MvrpDestinationMac) != E_OK)
      {
        return E_NOT_OK;
      }
      LenByte = Srp_PortConfig[PortIdx].MaxFrameSize;
      if(EthIf_ProvideTxBuffer(Srp_PortConfig[PortIdx].CtrlIdx, SRP_MVRP_ETHER_TYPE, SRP_VLAN_PRIORITY, &BufIdx,
        (SRP_P2VAR(Eth_DataType*)) &TxBufU8Ptr, &LenByte) != BUFREQ_OK) /* PRQA S 0310 */ /* MD_Srp_11.4 */
      {
        return E_NOT_OK;
      }
      /* successfully allocated new TxBuffer. Store new Tx Buffer information */
      MrpParticipantPtr->BufIdx = BufIdx;
      MrpParticipantPtr->BufPtr = TxBufU8Ptr;
      MrpParticipantPtr->LenByte = LenByte;
      MrpParticipantPtr->UsedLenByte = 1;

      /* write common header to buffer */
      TxBufU8Ptr[0] = SRP_PROTOCOL_VERSION;

    } /* if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) < MsgLenByte) */
    /* write message to buffer */
    {
      Offset = MrpParticipantPtr->UsedLenByte;

      /* set common message attributes */
      /* Attribute Type */
      TxBufU8Ptr[Offset] = SRP_MVRP_ATTRIBUTE_TYPE;
      Offset += 1;

      /* Attribute Length */
      TxBufU8Ptr[Offset] = SRP_MVRP_ATTRIBUTE_LENGTH;
      Offset += 1;

      /* set MVRP message attributes */
      /* Vector Header */
      /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
      Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, Srp_ComposeVectorHeader(LeaveAllEvent, SRP_NUMBER_OF_VALUES))

      /* First Value */
      /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
      Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, Srp_PortState[PortIdx].MvrpParticipant.VlanId[AttributeIdx])

      /* Vector */
      TxBufU8Ptr[Offset] = (uint8)SRP_PACK_THREE_PACKED_EVENT_SINGLE(AttributeEvent);
      Offset += 1;

      /* End Mark */
      Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, SRP_END_MARK) /* PRQA S 0310 */ /* MD_Srp_11.4 */ /*lint !e572 */

      /* set used length */
      MrpParticipantPtr->UsedLenByte = Offset;
    }
    break;
    /* MSRP */
  case SRP_MSRP_APPLICATION:
    /* get right Attribute Declaration (Talker Advertise, Talker Failed, Listener or Domain) */
    switch(Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].AttributeType)
    {
      /* Talker Advertise */
    case SRP_MSRP_TALKER_ADVERTISE:
      /* check if enough TxBuffer is available */
      /*lint -e574 */
      if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) < SRP_MSRP_TALKER_AD_MSG_LENGTH)
      {
        /* insufficient TxBuffer. Trigger Transmit and allocate new TxBuffer */
        /* if possible, add EndMark */
        if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) > SRP_END_MARK_LENGTH) /*lint +e574 */
        {
          /* add EndMark */
          /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
          Srp_CopyUint16ToBuffer(TxBufU8Ptr, MrpParticipantPtr->UsedLenByte, SRP_END_MARK) /*lint !e572 */
        }
        /* Trigger Transmit */
        if(Srp_Transmit(PortIdx, MrpParticipantPtr, SRP_MSRP_ETHER_TYPE, FALSE, Srp_MsrpDestinationMac) != E_OK)
        {
          return E_NOT_OK;
        }
        /* get new TxBuffer */
        LenByte = Srp_PortConfig[PortIdx].MaxFrameSize;
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        if(EthIf_ProvideTxBuffer(Srp_PortConfig[PortIdx].CtrlIdx, SRP_MSRP_ETHER_TYPE, SRP_VLAN_PRIORITY, &BufIdx, 
          (SRP_P2VAR(Eth_DataType*))&TxBufU8Ptr, &LenByte) != BUFREQ_OK)
        {
          return E_NOT_OK;
        }
        /* successfully allocated new TxBuffer. Store new buffer information */
        MrpParticipantPtr->BufIdx = BufIdx;
        MrpParticipantPtr->BufPtr = TxBufU8Ptr;
        MrpParticipantPtr->LenByte = LenByte;
        MrpParticipantPtr->UsedLenByte = 1;

        /* write common header to buffer */
        TxBufU8Ptr[0] = SRP_PROTOCOL_VERSION;

      } /* if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) < MsgLenByte) */
      /* write message to buffer */
      {
        AttributeListLength = 30;
        Offset = MrpParticipantPtr->UsedLenByte;

        /* set common message attributes */
        /* Attribute Type */
        TxBufU8Ptr[Offset] = SRP_MSRP_TALKER_ADVERTISE;
        Offset += 1;

        /* Attribute Length */
        TxBufU8Ptr[Offset] = SRP_MSRP_TALKER_ADVERTISE_LENGTH;
        Offset += 1;

        /* Attribute List Length */
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, AttributeListLength)

        /* set MSRP Talker Advertise message attributes */
        /* set VectorHeader */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, Srp_ComposeVectorHeader(LeaveAllEvent, SRP_NUMBER_OF_VALUES))

        /* Stream ID --> SourceAddress */
        Srp_CopyMacToBuffer(
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.StreamId.SourceAddress, TxBufU8Ptr,
          &Offset);

        /* Stream ID --> Unique Id */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset,
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.StreamId.UniqueId)

        /* DataFrameParameters --> DestinationAddress */
        Srp_CopyMacToBuffer(
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.DataFrameParameters.
                                                                                                  DestinationAddress,
                                                                                                  TxBufU8Ptr, &Offset);

        /* DataFrameParameters --> VLan Identifier */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset,
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.DataFrameParameters.
                                                                                                   VLanIdentifier)

        /* TSpec --> MaxFrameSize */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset,
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.TSpec.MaxFrameSize)

        /* TSpec --> MaxIntervalFrames */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset,
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.TSpec.MaxIntervalFrames)

        /* PriorityAndRank */
        TxBufU8Ptr[Offset] = (uint8)
          (((Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.Priority << 5u) & (0xE0u)) |
           ((Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.Rank << 4u) & (0x10u)));
        Offset += 1;

        /* AccumulatedLatency */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint32ToBuffer(TxBufU8Ptr, Offset,
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.AccumulatedLatency)

        /* Vector */
        TxBufU8Ptr[Offset] = (uint8)SRP_PACK_THREE_PACKED_EVENT_SINGLE(AttributeEvent);
        Offset += 1;

        /* EndMark */
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, SRP_END_MARK) /*lint !e572 */

        /* set used length */
        MrpParticipantPtr->UsedLenByte = Offset;
      }
      break;
      /* Talker Failed */
    case SRP_MSRP_TALKER_FAILED:
      /* check if enough TxBuffer is available */
      /*lint -e574 */
      if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) < SRP_MSRP_TALKER_FAILED_MSG_LENGTH)
      {
        /* insufficient TxBuffer. Trigger Transmit and allocate new TxBuffer */
        /* if possible, add EndMark */
        if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) > SRP_END_MARK_LENGTH) /*lint +e574 */
        {
          /* add EndMark */
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_CopyUint16ToBuffer(TxBufU8Ptr, MrpParticipantPtr->UsedLenByte, SRP_END_MARK) /*lint !e572 */
        }
        /* Trigger Transmit */
        if(Srp_Transmit(PortIdx, MrpParticipantPtr, SRP_MSRP_ETHER_TYPE, FALSE, Srp_MsrpDestinationMac) != E_OK)
        {
          return E_NOT_OK;
        }
        /* get new TxBuffer */
        LenByte = Srp_PortConfig[PortIdx].MaxFrameSize;
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        if(EthIf_ProvideTxBuffer(Srp_PortConfig[PortIdx].CtrlIdx, SRP_MSRP_ETHER_TYPE, SRP_VLAN_PRIORITY, &BufIdx, 
          (SRP_P2VAR(Eth_DataType*))&TxBufU8Ptr, &LenByte) != BUFREQ_OK)
        {
          return E_NOT_OK;
        }
        /* successfully allocated new TxBuffer. Store new buffer information */
        MrpParticipantPtr->BufIdx = BufIdx;
        MrpParticipantPtr->BufPtr = TxBufU8Ptr;
        MrpParticipantPtr->LenByte = LenByte;
        MrpParticipantPtr->UsedLenByte = 1;

        /* write common header to buffer */
        TxBufU8Ptr[0] = SRP_PROTOCOL_VERSION;

      } /* if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) < MsgLenByte) */
      /* write message to TxBuffer */
      {
        Offset = MrpParticipantPtr->UsedLenByte;
        AttributeListLength = 39;

        /* set common message attributes */
        /* Attribute Type */
        TxBufU8Ptr[Offset] = SRP_MSRP_TALKER_FAILED;
        Offset += 1;

        /* Attribute Length */
        TxBufU8Ptr[Offset] = SRP_MSRP_TALKER_FAILED_LENGTH;
        Offset += 1;

        /* Attribute List Length */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, AttributeListLength)

        /* set MSRP Talker Failed attributes */
        /* Vector Header */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, Srp_ComposeVectorHeader(LeaveAllEvent, SRP_NUMBER_OF_VALUES))

        /* Stream Id --> Source Address */
        Srp_CopyMacToBuffer(
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.StreamId.SourceAddress, TxBufU8Ptr,
          &Offset);

        /* Stream Id --> Unique Id */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset,
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.StreamId.UniqueId)

        /* DataFrameParameters --> DestinationAddress */
        Srp_CopyMacToBuffer(Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.
          DataFrameParameters.DestinationAddress, TxBufU8Ptr, &Offset);

        /* DataFrameParameters --> VLan identifier */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, Srp_PortState[PortIdx].MsrpParticipant.
                                          Attribute[AttributeIdx].StreamInfo.DataFrameParameters.VLanIdentifier)

        /* TSpec --> MaxFrameSize */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset,
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.TSpec.MaxFrameSize)

        /* TSpec --> MaxIntervalFrames */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset,
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.TSpec.MaxIntervalFrames)

        /* PriorityAndRank */
        TxBufU8Ptr[Offset] = (uint8)
          (((Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.Priority << 5) & (0xE0u)) | 
           ((Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.Rank << 4) & (0x10u)));
        Offset += 1;

        /* Accumulated Latency */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint32ToBuffer(TxBufU8Ptr, Offset,
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.AccumulatedLatency)

        /* FailureInformation --> Bridge Id */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint64ToBuffer(TxBufU8Ptr, Offset,
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.FailureInformation.BridgeID)

        /* FailureInformation --> FailureCode */
        TxBufU8Ptr[Offset] = (uint8)
          (Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.FailureInformation.
                                                                                      ReservationFailuredCode);
        Offset += 1;

        /* Vector */
        TxBufU8Ptr[Offset] = (uint8)SRP_PACK_THREE_PACKED_EVENT_SINGLE(AttributeEvent);
        Offset += 1;

        /* EndMark */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, SRP_END_MARK) /*lint !e572 */

        /* set used length */
        MrpParticipantPtr->UsedLenByte = Offset;

      }

      break;
      /* Listener */
    case SRP_MSRP_LISTENER:
      /* check if enough TxBuffer is available */
      /*lint -e574 */
      if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) < SRP_MSRP_LISTENER_MSG_LENGTH)
      {
        /* insufficient TxBuffer. Trigger Transmit and allocate new TxBuffer */
        /* if possible, add EndMark */
        if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) > SRP_END_MARK_LENGTH) /*lint +e574 */
        {
          /* add EndMark */
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_CopyUint16ToBuffer(TxBufU8Ptr, MrpParticipantPtr->UsedLenByte, SRP_END_MARK) /*lint !e572 */
        }
        /* Trigger Transmit */
        if(Srp_Transmit(PortIdx, MrpParticipantPtr, SRP_MSRP_ETHER_TYPE, FALSE, Srp_MsrpDestinationMac) != E_OK)
        {
          return E_NOT_OK;
        }
        /* get new TxBuffer */
        LenByte = Srp_PortConfig[PortIdx].MaxFrameSize;
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        if(EthIf_ProvideTxBuffer(Srp_PortConfig[PortIdx].CtrlIdx, SRP_MSRP_ETHER_TYPE, SRP_VLAN_PRIORITY, &BufIdx, 
          (SRP_P2VAR(Eth_DataType*))&TxBufU8Ptr, &LenByte) != BUFREQ_OK)
        {
          return E_NOT_OK;
        }
        /* successfully allocated new TxBuffer. Store new buffer information */
        MrpParticipantPtr->BufIdx = BufIdx;
        MrpParticipantPtr->BufPtr = TxBufU8Ptr;
        MrpParticipantPtr->LenByte = LenByte;
        MrpParticipantPtr->UsedLenByte = 1;

        /* write common header to buffer */
        TxBufU8Ptr[0] = SRP_PROTOCOL_VERSION;

      } /* if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) < MsgLenByte) */
      /* write message to buffer */
      {
        Offset = MrpParticipantPtr->UsedLenByte;
        AttributeListLength = 14;

        /* set message pointers */

        /* set common message attributes */
        /* Attribute Type */
        TxBufU8Ptr[Offset] = SRP_MSRP_LISTENER;
        Offset += 1;

        /* Attribute Length */
        TxBufU8Ptr[Offset] = SRP_MSRP_LISTENER_LENGTH;
        Offset += 1;

        /* Attribute List Length */
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, AttributeListLength)

        /* set MSRP Listener attributes */
        /* Vector Header */
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, Srp_ComposeVectorHeader(LeaveAllEvent, SRP_NUMBER_OF_VALUES))

        /* Stream Id --> SourceAddress */
        Srp_CopyMacToBuffer(
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.StreamId.SourceAddress, TxBufU8Ptr,
          &Offset);

        /* Stream Id --> Unique Id */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset,
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.StreamId.UniqueId)

        /* Vector --> ThreePackedEvent */
        TxBufU8Ptr[Offset] = (uint8)SRP_PACK_THREE_PACKED_EVENT_SINGLE(AttributeEvent);
        Offset += 1;

        /* Vector --> FourPackedEvent */
        TxBufU8Ptr[Offset] = (uint8)SRP_PACK_FOUR_PACKED_EVENT_SINGLE(
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].ListenerDeclaration);
        Offset += 1;

        /* EndMark */
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, SRP_END_MARK) /*lint !e572 */

        /* set used length */
        MrpParticipantPtr->UsedLenByte = Offset;
      }
      break;
      /* Domain */
    case SRP_MSRP_DOMAIN:
      /* check if enough TxBuffer is available */
      /*lint -e574 */
      if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) < SRP_MSRP_DOMAIN_MSG_LENGTH)
      {
        /* insufficient TxBuffer. Trigger Transmit and allocate new TxBuffer */
        /* if possible, add EndMark */
        if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) > SRP_END_MARK_LENGTH) /*lint +e574 */
        {
          /* add EndMark */
          /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
          Srp_CopyUint16ToBuffer(TxBufU8Ptr, MrpParticipantPtr->UsedLenByte, SRP_END_MARK) /*lint !e572 */
        }
        /* Trigger Transmit */
        if(Srp_Transmit(PortIdx, MrpParticipantPtr, SRP_MSRP_ETHER_TYPE, FALSE, Srp_MsrpDestinationMac) != E_OK)
        {
          return E_NOT_OK;
        }
        /* get new TxBuffer */
        LenByte = Srp_PortConfig[PortIdx].MaxFrameSize;
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        if(EthIf_ProvideTxBuffer(Srp_PortConfig[PortIdx].CtrlIdx, SRP_MSRP_ETHER_TYPE, SRP_VLAN_PRIORITY, &BufIdx, 
          (SRP_P2VAR(Eth_DataType*))&TxBufU8Ptr, &LenByte) != BUFREQ_OK)
        {
          return E_NOT_OK;
        }
        /* successfully allocated new TxBuffer. Store new buffer information */
        MrpParticipantPtr->BufIdx = BufIdx;
        MrpParticipantPtr->BufPtr = TxBufU8Ptr;
        MrpParticipantPtr->LenByte = LenByte;
        MrpParticipantPtr->UsedLenByte = 1;

        /* write common header to buffer */
        TxBufU8Ptr[0] = SRP_PROTOCOL_VERSION;

      } /* if((MrpParticipantPtr->LenByte - MrpParticipantPtr->UsedLenByte) < MsgLenByte) */
      /* write message to buffer */
      {
        Offset = MrpParticipantPtr->UsedLenByte;
        AttributeListLength = 9;

        /* set common message attributes */
        /* Attribute Type */
        TxBufU8Ptr[Offset] = SRP_MSRP_DOMAIN;
        Offset += 1;

        /* Attribute Length */
        TxBufU8Ptr[Offset] = SRP_MSRP_DOMAIN_LENGTH;
        Offset += 1;

        /* Attribute List Length */
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, AttributeListLength)

        /* set MSRP Listener attributes */
        /* Vector Header */
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, Srp_ComposeVectorHeader(LeaveAllEvent, SRP_NUMBER_OF_VALUES))

        /* FirstValue --> SRclassID */
        TxBufU8Ptr[Offset] = 
          (uint8)Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].DomainInfo.SrClassId;
        Offset += 1;

        /* FirstValue --> SRclassPriority */
        TxBufU8Ptr[Offset] = 
          (uint8)Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].DomainInfo.SrClassPriority;
        Offset += 1;

        /* FirstValue --> SRclassVID */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset,
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].DomainInfo.SrClassVid)

        /* Vector */
        TxBufU8Ptr[Offset] = (uint8)SRP_PACK_THREE_PACKED_EVENT_SINGLE(AttributeEvent);
        Offset += 1;

        /* EndMark */
        Srp_CopyUint16ToBuffer(TxBufU8Ptr, Offset, SRP_END_MARK) /* PRQA S 310 */ /* MD_Srp_11.4 */ /*lint !e572 */

        /* set used length */
        MrpParticipantPtr->UsedLenByte = Offset;
      }
      break;
    default:
      break;
    }
    break;
  default:
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_ASSEMBLE_MSG, SRP_E_INV_MRP_APPLICATION);
    return E_NOT_OK;
  }

  return E_OK;
} /* Srp_AssembleMsg() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
/* PRQA L: STCYC */
/* PRQA L: STPAR */
/* PRQA L: STMIF */
/* PRQA L: STPTH */

/**********************************************************************************************************************
 *  Srp_AssembleMsgJoin()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(Std_ReturnType, SRP_CODE) Srp_AssembleMsgJoin(SRP_P2VAR(Srp_MrpParticipantType)MrpParticipantPtr, 
  SRP_P2CONST(Srp_ApplicantStateMachineType) ApplicantSmPtr, Srp_PortIdxType PortIdx,
  Srp_AttributeIdxType AttributeIdx, Srp_LeaveAllType LeaveAllEvent)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal;

  /* ----- Implementation ----------------------------------------------- */
  if(ApplicantSmPtr->RegistrarSmPtr->State == SRP_STATE_REGISTRAR_IN)
  {
    /* add Join In message to TxBuffer */
    RetVal = Srp_AssembleMsg(MrpParticipantPtr, PortIdx, AttributeIdx, LeaveAllEvent, SRP_ATTRIBUTE_EVENT_JOIN_IN);
  }
  else
  {
    /* add Join Empty message to TxBuffer */
    RetVal = Srp_AssembleMsg(MrpParticipantPtr, PortIdx, AttributeIdx, LeaveAllEvent, SRP_ATTRIBUTE_EVENT_JOIN_MT);
  }

  return RetVal;
} /* Srp_AssembleMsgJoin() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
 *  Srp_AssembleMsgInOrMt()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(Std_ReturnType, SRP_CODE) Srp_AssembleMsgInOrMt(SRP_P2VAR(Srp_MrpParticipantType)MrpParticipantPtr, 
  SRP_P2CONST(Srp_ApplicantStateMachineType) ApplicantSmPtr, Srp_PortIdxType PortIdx,
  Srp_AttributeIdxType AttributeIdx, Srp_LeaveAllType LeaveAllEvent)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal;

  /* ----- Implementation ----------------------------------------------- */
  if(ApplicantSmPtr->RegistrarSmPtr->State == SRP_STATE_REGISTRAR_IN)
  {
    /* add In message to TxBuffer */
    RetVal = Srp_AssembleMsg(MrpParticipantPtr, PortIdx, AttributeIdx, LeaveAllEvent, SRP_ATTRIBUTE_EVENT_IN);
  }
  else
  {
    /* add Empty message to TxBuffer */
    RetVal = Srp_AssembleMsg(MrpParticipantPtr, PortIdx, AttributeIdx, LeaveAllEvent, SRP_ATTRIBUTE_EVENT_MT);
  }

  return RetVal;
} /* Srp_AssembleMsgInOrMt() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
 *  Srp_RequestTransmitOpportunity()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(Std_ReturnType, SRP_CODE) Srp_RequestTransmitOpportunity(Srp_PortIdxType PortIdx, 
  SRP_P2VAR(Srp_MrpParticipantType) MrpParticipantPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 BufIdx = SRP_INV_BUF_IDX; /* PRQA S 0781 */ /* MD_Srp_5.6 */
  SRP_P2VAR(uint8) BufPtr; /* PRQA S 0781 */ /* MD_Srp_5.6 */
  uint16 LenByte = Srp_PortConfig[PortIdx].MaxFrameSize; /* PRQA S 0781 */ /* MD_Srp_5.6 */
  Srp_EtherTypeType EtherType;

  /* ----- Implementation ----------------------------------------------- */
  /* get ether type */
  switch(MrpParticipantPtr->MrpApplication)
  {
  case SRP_MMRP_APPLICATION:
    EtherType = SRP_MMRP_ETHER_TYPE;
    break;
  case SRP_MVRP_APPLICATION:
    EtherType = SRP_MVRP_ETHER_TYPE;
    break;
  case SRP_MSRP_APPLICATION:
    EtherType = SRP_MSRP_ETHER_TYPE;
    break;
  default:
    /* invalid MRP-Application */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_REQUEST_TRANSMIT_OPPORTUNITY, SRP_E_INV_MRP_APPLICATION);
    return RetVal;
  }

  if((MrpParticipantPtr->BufIdx == SRP_INV_BUF_IDX) && (MrpParticipantPtr->BufPtr == NULL_PTR))
  {
    /* currently no buffer is allocated */
    /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
    if(EthIf_ProvideTxBuffer(Srp_PortConfig[PortIdx].CtrlIdx, EtherType, SRP_VLAN_PRIORITY, &BufIdx,
      (SRP_P2VAR(Eth_DataType*))&BufPtr, &LenByte) == BUFREQ_OK)
    {
      /* successfully allocated new TxBuffer */
      MrpParticipantPtr->BufIdx = BufIdx;
      MrpParticipantPtr->BufPtr = BufPtr;
      MrpParticipantPtr->LenByte = LenByte;
      
      /* Write common message header to buffer (ProtocolVersion) */
      BufPtr[0] = SRP_PROTOCOL_VERSION;
      MrpParticipantPtr->UsedLenByte = 1;

      RetVal = E_OK;
    }
    else
    {
      /* unable to allocate TxBuffer */
      /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
      Srp_CallDetReportError(SRP_SID_REQUEST_TRANSMIT_OPPORTUNITY, SRP_E_NO_TX_BUFFER);
    }
  }

  return RetVal;

} /* Srp_RequestTransmitOpportunity() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_UnpackThreePackedEvent()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(void, SRP_CODE) Srp_UnpackThreePackedEvent(uint8 ThreePackedEvent, SRP_P2VAR(uint8) Event1, 
  SRP_P2VAR(uint8) Event2, SRP_P2VAR(uint8) Event3)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 FirstEvent;
  uint8 SecondEvent;
  uint8 ThirdEvent;

  /* ----- Implementation ----------------------------------------------- */
  ThirdEvent = (uint8)(ThreePackedEvent % 6u);
  ThreePackedEvent = (uint8)((uint8)(ThreePackedEvent - ThirdEvent)/6u);
  SecondEvent = (uint8)(ThreePackedEvent % 6u);
  FirstEvent = (uint8)((uint8)(ThreePackedEvent - SecondEvent) / 6u);

  if(Event1 != NULL_PTR)
  {
    *Event1 = FirstEvent;

    if(Event2 != NULL_PTR)
    {
      *Event2 = SecondEvent;

      if(Event3  != NULL_PTR)
      {
        *Event3 = ThirdEvent;
      }
    }
  }

} /* Srp_UnpackThreePackedEvent() */

/**********************************************************************************************************************
 *  Srp_UnpackFourPackedEvent()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(void, SRP_CODE) Srp_UnpackFourPackedEvent(uint8 FourPackedEvent, SRP_P2VAR(uint8) Event1, 
  SRP_P2VAR(uint8) Event2, SRP_P2VAR(uint8) Event3, SRP_P2VAR(uint8) Event4)
{
  *Event1 = (uint8)((FourPackedEvent & 0xC0u) >> 6u);
  *Event2 = (uint8)((FourPackedEvent & 0x30u) >> 4u);
  *Event3 = (uint8)((FourPackedEvent & 0x0Cu) >> 2u);
  *Event4 = (uint8)(FourPackedEvent  & 0x03u);
} /* Srp_UnpackFourPackedEvent() */

/**********************************************************************************************************************
 *  Srp_EqualsMacAddress()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(boolean, SRP_CODE) Srp_EqualsMacAddress(SRP_P2CONST(uint8) FirstMacAddressPtr, 
  SRP_P2CONST(uint8) SecondMacAddressPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 Index = 6;
  boolean RetVal = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* iterate the 6-Bytes of the MAC-Address */
  while(Index > 0u)
  {
    Index--;
    if(FirstMacAddressPtr[Index] != SecondMacAddressPtr[Index])
    {
      /* first inequality found in MAC-Address --> MAC-Address are not equal. Set RetVal to FALSE and stop iteration */
      RetVal = FALSE;
      break;
    }
  }
  return RetVal;
} /* Srp_EqualsMacAddress() */

/**********************************************************************************************************************
 *  Srp_EqualsStreamId()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(boolean, SRP_CODE) Srp_EqualsStreamId(const Srp_StreamIdType FirstStreamId, 
  const Srp_StreamIdType SecondStreamId)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean RetVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if(FirstStreamId.UniqueId == SecondStreamId.UniqueId)
  {
    if(Srp_EqualsMacAddress(FirstStreamId.SourceAddress, SecondStreamId.SourceAddress) == TRUE)
    {
      RetVal = TRUE;
    }
  }

  return RetVal;

} /* Srp_EqualsStreamId */

/**********************************************************************************************************************
 *  Srp_CheckBandwidthLimitRx()
 *********************************************************************************************************************/
SRP_LOCAL_INLINE FUNC(Std_ReturnType, SRP_CODE) Srp_CheckBandwidthLimitRx(Srp_PortIdxType PortIdx,
  uint32 AdditionalBandwidth, boolean ReserveBandwidth)
{
  /* Note: On the reception path no real reservation of bandwidth is supported.
   * It is just checked here, that the max bandwidth supported by the Transceiver is not exceeded. */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if(AdditionalBandwidth == 0)
  {
    /* No further checks required */
    retVal = E_OK;
  }
  else
  {
    /* AdditionalBandwidth > 0 -> Check if sufficient bandwidth is available */
    if(Srp_PortConfig[PortIdx].MaxAllowedBandwidthRx > (Srp_PortState[PortIdx].UsedBandwithRx + AdditionalBandwidth))
    {
      /* Sufficient bandwidth available */
      retVal = E_OK;
      if(ReserveBandwidth == TRUE)
      {
        /* Mark the bandwidth as used */
        Srp_PortState[PortIdx].UsedBandwithRx += AdditionalBandwidth;
      }
    }
  }

  return retVal;
} /* Srp_CheckBandwidthLimitRx() */

/**********************************************************************************************************************
 *  Srp_CheckBandwidthLimitTx()
 *********************************************************************************************************************/
SRP_LOCAL_INLINE FUNC(Std_ReturnType, SRP_CODE) Srp_CheckBandwidthLimitTx(Srp_PortIdxType PortIdx,
  uint32 AdditionalBandwidth, uint8 FramePrio, boolean ReserveBandwidth)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
#if (SRP_TYPE == SRP_TYPE_DYNAMIC)
  uint32 currentBandwidthLimit;
#endif /* (SRP_TYPE == SRP_TYPE_DYNAMIC) */
  /* ----- Implementation ----------------------------------------------- */
#if (SRP_TYPE == SRP_TYPE_STATIC)
  if((Srp_PortConfig[PortIdx].MaxAllowedBandwidthTx - Srp_PortState[PortIdx].UsedBandwithTx) >= AdditionalBandwidth)
  {
    retVal = E_OK;
    if(ReserveBandwidth == TRUE)
    {
      Srp_PortState[PortIdx].UsedBandwithTx += AdditionalBandwidth;
    }
  }
  SRP_DUMMY_STATEMENT(FramePrio); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif /* (SRP_TYPE == SRP_TYPE_STATIC) */

#if (SRP_TYPE == SRP_TYPE_DYNAMIC)
  if(EthIf_GetBandwidthLimit(Srp_PortConfig[PortIdx].CtrlIdx,
    Srp_PortConfig[PortIdx].FramePrioMappingPtr[FramePrio].QueuePriority, &currentBandwidthLimit) == E_OK)
  {
    /* Compute new required bandwidth limit */
    uint32 newBandwidthLimit = AdditionalBandwidth + currentBandwidthLimit;

    /* Preclude possible overflow. 'Equal' is required if one bandwidth is '0' */
    if((newBandwidthLimit >= currentBandwidthLimit) && (newBandwidthLimit >= AdditionalBandwidth))
    {
      /* No overflow occurred -> try to apply the new bandwidth limit */
      if(EthIf_SetBandwidthLimit(Srp_PortConfig[PortIdx].CtrlIdx,
        Srp_PortConfig[PortIdx].FramePrioMappingPtr[FramePrio].QueuePriority, newBandwidthLimit) == E_OK)
      {
        /* successfully applied new bandwidth limit -> sufficient bandwidth available */
        retVal = E_OK;
        /* Check if applying the new bandwidth limit has to be undone */
        if(ReserveBandwidth == FALSE)
        {
          /* Set the bandwidth limit to the old value again. */
          /* It is assumed that applying the former bandwidth limit is always possible */
          (void)EthIf_SetBandwidthLimit(Srp_PortConfig[PortIdx].CtrlIdx,
            Srp_PortConfig[PortIdx].FramePrioMappingPtr[FramePrio].QueuePriority, currentBandwidthLimit);
        }
      }
    }
  }
#endif /* (SRP_TYPE == SRP_TYPE_DYNAMIC) */

  return retVal;
} /* Srp_CheckBandwidthLimitTx() */

/**********************************************************************************************************************
 *  Srp_ReleaseBandwidthRx()
 *********************************************************************************************************************/
SRP_LOCAL_INLINE FUNC(Std_ReturnType, SRP_CODE) Srp_ReleaseBandwidthRx(Srp_PortIdxType PortIdx, uint32 Bandwidth)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if(Srp_PortState[PortIdx].UsedBandwithRx >= Bandwidth)
  {
    Srp_PortState[PortIdx].UsedBandwithRx -= Bandwidth;
    retVal = E_OK;
  }

  return retVal;
} /* Srp_ReleaseBandwidthRx() */

/**********************************************************************************************************************
 *  Srp_ReleaseBandwidthTx()
 *********************************************************************************************************************/
SRP_LOCAL_INLINE FUNC(Std_ReturnType, SRP_CODE) Srp_ReleaseBandwidthTx(Srp_PortIdxType PortIdx, uint8 FramePrio,
  uint32 Bandwidth)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
#if (SRP_TYPE == SRP_TYPE_DYNAMIC)
  uint32 currentBandwidthLimit;
#endif /* (SRP_TYPE == SRP_TYPE_DYNAMIC) */

  /* ----- Implementation ----------------------------------------------- */
#if (SRP_TYPE == SRP_TYPE_STATIC)
  if(Srp_PortState[PortIdx].UsedBandwithTx >= Bandwidth)
  {
    Srp_PortState[PortIdx].UsedBandwithTx -= Bandwidth;
    retVal = E_OK;
  }
  SRP_DUMMY_STATEMENT(FramePrio); /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif /* (SRP_TYPE == SRP_TYPE_STATIC) */

#if (SRP_TYPE == SRP_TYPE_DYNAMIC)
  if(EthIf_GetBandwidthLimit(Srp_PortConfig[PortIdx].CtrlIdx,
    Srp_PortConfig[PortIdx].FramePrioMappingPtr[FramePrio].QueuePriority, &currentBandwidthLimit) == E_OK)
  {
    /* Check if current bandwidth limit is greater/equal the bandwidth to be released */
    if(currentBandwidthLimit >= Bandwidth)
    {
      uint32 newBandwidthLimit = currentBandwidthLimit - Bandwidth;
      /* Set the new Bandwidth limit */
      retVal = EthIf_SetBandwidthLimit(Srp_PortConfig[PortIdx].CtrlIdx,
        Srp_PortConfig[PortIdx].FramePrioMappingPtr[FramePrio].QueuePriority, newBandwidthLimit);
    }
  }
#endif /* (SRP_TYPE == SRP_TYPE_DYNAMIC) */

  return retVal;
} /* Srp_ReleaseBandwidthTx() */

/**********************************************************************************************************************
 *  Srp_ComputeRequiredBandwidth()
 *********************************************************************************************************************/
SRP_LOCAL_INLINE FUNC(Std_ReturnType, SRP_CODE) Srp_ComputeRequiredBandwidth(Srp_PortIdxType PortIdx,
  Srp_AttributeIdxType AttributeIdx, SRP_P2VAR(uint32) BandwidthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  float32 bandwidthMbits;

  /* ----- Implementation ----------------------------------------------- */
  /* ensure that Attribute is valid */
  if(Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[AttributeIdx] == TRUE)
  {
    retVal = E_OK;

    /* Total Frame Size */
    bandwidthMbits = (float32)((float32)SRP_PER_FRAME_OVERHEAD + 
      (float32)Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.TSpec.MaxFrameSize) * 8;

    /* Consider the Max Interval Frames */
    bandwidthMbits = (float32)(bandwidthMbits * 
      (float32)Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.TSpec.MaxIntervalFrames); 

    /* Consider the Class measurement interval */
    
    if(Srp_PortConfig[PortIdx].FramePrioMappingPtr[Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].
      StreamInfo.Priority].SrClass == SRP_SR_CLASS_A)
    {
      bandwidthMbits = (float32)(bandwidthMbits * 
        (float32)((float32)1/(float32)SRP_SR_CLASS_A_MEASUREMENT_INTERVAL));
    }
    else if(Srp_PortConfig[PortIdx].FramePrioMappingPtr[Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].
      StreamInfo.Priority].SrClass == SRP_SR_CLASS_B)
    {
      bandwidthMbits = (float32)(bandwidthMbits * 
        (float32)((float32)1/(float32)SRP_SR_CLASS_B_MEASUREMENT_INTERVAL));
    }
    else
    {
      /* Invalid SR Class. This should not happen */
      bandwidthMbits = 0;
      retVal = E_NOT_OK;
    }

    /* convert to Bit/s */
    *BandwidthPtr = SRP_MBITS_TO_BITS_UINT32(bandwidthMbits);
  }

  return retVal;
} /* Srp_ComputeRequiredBandwidth() */

/**********************************************************************************************************************
 *  Srp_MrpHasAtLeastOneValidAttribute()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(boolean, SRP_CODE) Srp_MrpHasAtLeastOneValidAttribute(Srp_PortIdxType PortIdx,
  Srp_MrpApplicationType MrpApplicationType)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType AttributeCnt;
  boolean result = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  switch(MrpApplicationType)
  {
# if (SRP_MMRP_SUPPORT == STD_ON)
  case SRP_MMRP_APPLICATION:
    for(AttributeCnt = 0; AttributeCnt < SRP_SUPPORTED_MMRP_ATTRIBUTES; AttributeCnt++)
    {
      if((Srp_PortState[PortIdx].MmrpParticipant.AttributeIsValid[AttributeCnt] == TRUE) &&
        (Srp_PortState[PortIdx].MmrpParticipant.ApplicantSM[AttributeCnt].SmIsValid == TRUE))
      {
        result = TRUE;
      }
    }
    break;
# endif /* (SRP_MMRP_SUPPORT == STD_ON) */
  case SRP_MVRP_APPLICATION:
    for(AttributeCnt = 0; AttributeCnt < SRP_SUPPORTED_MVRP_ATTRIBUTES; AttributeCnt++)
    {
      if((Srp_PortState[PortIdx].MvrpParticipant.AttributeIsValid[AttributeCnt] == TRUE) &&
        (Srp_PortState[PortIdx].MvrpParticipant.ApplicantSM[AttributeCnt].SmIsValid == TRUE))
      {
        result = TRUE;
      }
    }
    break;
  case SRP_MSRP_APPLICATION:
    for(AttributeCnt = 0; AttributeCnt < SRP_SUPPORTED_MSRP_ATTRIBUTES; AttributeCnt++)
    {
      if((Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[AttributeCnt] == TRUE) &&
        (Srp_PortState[PortIdx].MsrpParticipant.ApplicantSM[AttributeCnt].SmIsValid == TRUE))
      {
        result = TRUE;
      }
    }
    break;
  default:
    /* Nothing to do here (MISRA) */
    break;
  }

  return result;
} /* Srp_MrpHasAtLeastOneValidAttribute */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_GetStreamInfo()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(void, SRP_CODE) Srp_GetStreamInfo(SRP_P2CONST(uint8) DataPtr, SRP_P2VAR(uint16) Offset, 
  SRP_P2VAR(Srp_MsrpStreamInfoType) StreamInfo, boolean GetFailure)
{
  /* Get Source Address */
  Srp_GetMacAddress(DataPtr, Offset, &StreamInfo->StreamId.SourceAddress); /*lint !e545 */
  /* Get Unique ID */
  /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
  StreamInfo->StreamId.UniqueId = (uint16)Srp_Ntohs(*((SRP_P2CONST(uint16))&DataPtr[*Offset]));
  *Offset += 2;

  /* Get Stream Destination */
  Srp_GetMacAddress(DataPtr, Offset, &StreamInfo->DataFrameParameters.DestinationAddress); /*lint !e545 */

  /* Get VLAN ID */
  /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
  StreamInfo->DataFrameParameters.VLanIdentifier = (uint16)Srp_Ntohs(*((SRP_P2CONST(uint16))&DataPtr[*Offset]));
  *Offset += 2;

  /* Get Max Frame Size */
  /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
  StreamInfo->TSpec.MaxFrameSize = (uint16)Srp_Ntohs(*((SRP_P2CONST(uint16))&DataPtr[*Offset]));
  *Offset += 2;

  /* Get Max Frame Interval */
  /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
  StreamInfo->TSpec.MaxIntervalFrames = (uint16)Srp_Ntohs(*((SRP_P2CONST(uint16))&DataPtr[*Offset]));
  *Offset += 2;

  /* Get Priority and Rank */
  StreamInfo->Priority = (uint8)((DataPtr[*Offset] & 0xE0)>> 5);
  StreamInfo->Rank = (uint8)((DataPtr[*Offset] & 0x10)>> 4);
  *Offset += 1;

  /* Get Accumulated Latency */
  /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
  StreamInfo->AccumulatedLatency = (uint32)Srp_Ntohl(*((SRP_P2CONST(uint32))&DataPtr[*Offset]));
  *Offset += 4;

  /* Get Failure Information */
  if(GetFailure == TRUE)
  {
    /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
    StreamInfo->FailureInformation.BridgeID = (uint64)Srp_Ntoh64(*((SRP_P2CONST(uint64))&DataPtr[*Offset]));
    *Offset += 8;

    StreamInfo->FailureInformation.ReservationFailuredCode = DataPtr[*Offset];
    *Offset += 1;
  }
  else
  {
    StreamInfo->FailureInformation.BridgeID = 0u;
    StreamInfo->FailureInformation.ReservationFailuredCode = SRP_FAILURE_CODE_UNKNOWN;
  }

} /* Srp_GetStreamInfo() */

#if (SRP_MMRP_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  Srp_ProcessAttributeMac()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessAttributeMac(Srp_PortIdxType PortIdx, SRP_P2CONST(uint8) MacAddress, 
  Srp_AttributeEventType AttributeEvent, boolean LeaveAll)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType AttributeIdx = SRP_SUPPORTED_MMRP_ATTRIBUTES;
  Srp_AttributeIdxType FreeAttributeIdx = SRP_INV_ATTRIBUTE_IDX;

  /* ----- Implementation ----------------------------------------------- */

  /* check if AttributeEvent is valid */
  if(AttributeEvent > 5u)
  {
    /* invalid AttributeEvent */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_PROCESS_ATTRIBUTE_MAC, SRP_E_INV_PARAM);
    return;
  }

  /* check if attribute is existing or if a free attribute is available */
  while(AttributeIdx > 0u)
  {
    AttributeIdx--;
    if(Srp_PortState[PortIdx].MmrpParticipant.AttributeIsValid[AttributeIdx] == TRUE)
    {
      /* PRQA S 3415 3 */ /* MD_Srp_12.4 */
      if((Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].AttributeType == SRP_MMRP_MAC_TYPE) 
        && (Srp_EqualsMacAddress(Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].MacAddress, MacAddress) 
        == TRUE))
      {
        /* Attribute found --> process the state machines */
        FreeAttributeIdx = SRP_INV_ATTRIBUTE_IDX;

        if(LeaveAll == TRUE)
        {
          /* trigger rLA! first */
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmRegistrar((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant, PortIdx, 
            AttributeIdx, SRP_MRP_EVENT_R_LA);
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant, PortIdx, 
            AttributeIdx, SRP_MRP_EVENT_R_LA);
        }
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_ProcessSmRegistrar((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant, PortIdx, 
          AttributeIdx, AttributeEvent);
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant, PortIdx, 
          AttributeIdx, AttributeEvent);
        break;
      }
    }
    else
    {
      FreeAttributeIdx = AttributeIdx;
    }
  }

  /* if attribute was not found and a free attribute is available, register the attribute */
  if(FreeAttributeIdx != SRP_INV_ATTRIBUTE_IDX)
  {
    Srp_PortState[PortIdx].MmrpParticipant.AttributeIsValid[FreeAttributeIdx] = TRUE;
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].AttributeType = SRP_MMRP_MAC_TYPE;
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].MacAddress[0] = MacAddress[0];
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].MacAddress[1] = MacAddress[1];
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].MacAddress[2] = MacAddress[2];
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].MacAddress[3] = MacAddress[3];
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].MacAddress[4] = MacAddress[4];
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].MacAddress[5] = MacAddress[5];

    Srp_InitSmRegistrar(PortIdx, &Srp_PortState[PortIdx].MmrpParticipant.RegistrarSM[FreeAttributeIdx]);
  }

} /* Srp_ProcessAttributeMac() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_ProcessAttributeServiceReq()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessAttributeServiceReq(Srp_PortIdxType PortIdx, 
  Srp_MmrpServiceRequirementAttributeType ServiceReq, Srp_AttributeEventType AttributeEvent, boolean LeaveAll)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType AttributeIdx = SRP_SUPPORTED_MMRP_ATTRIBUTES;
  Srp_AttributeIdxType FreeAttributeIdx = SRP_INV_ATTRIBUTE_IDX;

  /* ----- Implementation ----------------------------------------------- */

  /* check if AttributeEvent is valid */
  if(AttributeEvent > 5u)
  {
    /* invalid AttributeEvent */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_PROCESS_ATTRIBUTE_SERVICE_REQ, SRP_E_INV_PARAM);
    return;
  }

  /* check if attribute is existing or if a free attribute is available */
  while(AttributeIdx > 0u)
  {
    AttributeIdx--;
    if(Srp_PortState[PortIdx].MmrpParticipant.AttributeIsValid[AttributeIdx] == TRUE)
    {
      if((Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].AttributeType == 
        SRP_MMRP_SERVICE_REQUIREMENT_TYPE) && 
        (Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].ServiceReq == ServiceReq))
      {
        /* Attribute found --> process the state machines */
        FreeAttributeIdx = SRP_INV_ATTRIBUTE_IDX;

        if(LeaveAll == TRUE)
        {
          /* trigger rLA! first */
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmRegistrar((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant, PortIdx, 
            AttributeIdx, SRP_MRP_EVENT_R_LA);
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant, PortIdx, 
            AttributeIdx, SRP_MRP_EVENT_R_LA);
        }
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_ProcessSmRegistrar((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant, PortIdx, 
          AttributeIdx, AttributeEvent);
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant, PortIdx, 
          AttributeIdx, AttributeEvent);
        break;
      }
    }
    else
    {
      FreeAttributeIdx = AttributeIdx;
    }
  }

  /* if attribute was not found and a free attribute is available, register the attribute */
  if(FreeAttributeIdx != SRP_INV_ATTRIBUTE_IDX)
  {
    Srp_PortState[PortIdx].MmrpParticipant.AttributeIsValid[FreeAttributeIdx] = TRUE;
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].AttributeType = 
      SRP_MMRP_SERVICE_REQUIREMENT_TYPE;
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].ServiceReq = ServiceReq;

    Srp_InitSmRegistrar(PortIdx, &Srp_PortState[PortIdx].MmrpParticipant.RegistrarSM[FreeAttributeIdx]);

  }

} /* Srp_ProcessAttributeServiceReq() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (SRP_MMRP_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  Srp_ProcessAttributeVid()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessAttributeVid(Srp_PortIdxType PortIdx, Srp_VLanIdType VID, 
  Srp_AttributeEventType AttributeEvent, boolean LeaveAll)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType AttributeIdx = SRP_SUPPORTED_MVRP_ATTRIBUTES;
  Srp_AttributeIdxType FreeAttributeIdx = SRP_INV_ATTRIBUTE_IDX;

  /* ----- Implementation ----------------------------------------------- */

  /* check if AttributeEvent is valid */
  if(AttributeEvent > 5u)
  {
    /* invalid AttributeEvent */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_PROCESS_ATTRIBUTE_V_LAN, SRP_E_INV_PARAM);
    return;
  }

  /* check if attribute is existing or if a free attribute is available */
  while(AttributeIdx > 0u)
  {
    AttributeIdx--;
    if(Srp_PortState[PortIdx].MvrpParticipant.AttributeIsValid[AttributeIdx] == TRUE)
    {
      if(Srp_PortState[PortIdx].MvrpParticipant.VlanId[AttributeIdx] == VID)
      {
        /* Attribute found --> process the state machines */
        FreeAttributeIdx = SRP_INV_ATTRIBUTE_IDX;

        if(LeaveAll == TRUE)
        {
          /* trigger rLA! first */
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmRegistrar((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MvrpParticipant, PortIdx, 
            AttributeIdx, SRP_MRP_EVENT_R_LA);
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MvrpParticipant, PortIdx, 
            AttributeIdx, SRP_MRP_EVENT_R_LA);
        }
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_ProcessSmRegistrar((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MvrpParticipant, PortIdx, 
          AttributeIdx, AttributeEvent);
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MvrpParticipant, PortIdx, 
          AttributeIdx, AttributeEvent);
        break;
      }
    }
    else
    {
      FreeAttributeIdx = AttributeIdx;
    }
  }

  /* if attribute was not found and a free attribute is available, register the attribute */
  if(FreeAttributeIdx != SRP_INV_ATTRIBUTE_IDX)
  {
    Srp_PortState[PortIdx].MvrpParticipant.AttributeIsValid[FreeAttributeIdx] = TRUE;
    Srp_PortState[PortIdx].MvrpParticipant.VlanId[FreeAttributeIdx] = VID;

    Srp_InitSmRegistrar(PortIdx, &Srp_PortState[PortIdx].MvrpParticipant.RegistrarSM[FreeAttributeIdx]);
  }
} /* Srp_ProcessAttributeVid() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_ProcessAttributeTalker()
 *********************************************************************************************************************/
/* PRQA S 6010 STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 STMIF */ /* MD_MSR_STMIF */
/* PRQA S 6050 STCAL */ /* MD_MSR_STCAL */
SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessAttributeTalker(Srp_PortIdxType PortIdx, Srp_MsrpStreamInfoType StreamInfo, 
  Srp_AttributeEventType AttributeEvent, boolean LeaveAll, Srp_MsrpAttributeType AttributeType)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType AttributeIdx = SRP_SUPPORTED_MSRP_ATTRIBUTES;
  Srp_AttributeIdxType FreeAttributeIdx = SRP_INV_ATTRIBUTE_IDX;
  Srp_AttributeIdxType ListenerIdx = SRP_INV_ATTRIBUTE_IDX;
  boolean TalkerFound = FALSE;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  Srp_CheckDetErrorReturnVoid(
    ((AttributeType == SRP_MSRP_TALKER_ADVERTISE) || (AttributeType == SRP_MSRP_TALKER_FAILED)),
    SRP_SID_PROCESS_ATTRIBUTE_TALKER, SRP_E_INV_PARAM);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  /* check if AttributeEvent is valid */
  if(AttributeEvent > 5u)
  {
    /* invalid AttributeEvent */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_PROCESS_ATTRIBUTE_TALKER, SRP_E_INV_PARAM);
    return;
  }

  /* check if attribute is existing or if a free attribute is available */
  while(AttributeIdx > 0u)
  {
    AttributeIdx--;
    if(Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[AttributeIdx] == TRUE)
    {
      if((Srp_EqualsStreamId(Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.StreamId, 
        StreamInfo.StreamId)) && 
        (Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].AttributeType != SRP_MSRP_DOMAIN))
      {
        if((Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_ADVERTISE)
          || (Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_FAILED))
        {
          if(TalkerFound == FALSE)
          {
            /* Attribute found --> Save the Attribute Index */
            FreeAttributeIdx = AttributeIdx;
            TalkerFound = TRUE;
          }
        } /* End if: Talker */
        else
        {
          /* Check if ECU is Listener of the Stream */
          if(Srp_PortState[PortIdx].MsrpParticipant.ApplicantSM[AttributeIdx].SmIsValid == TRUE)
          {
            ListenerIdx = AttributeIdx;
          }
        }
      } /* End if: equal Stream Id */
    } /* if(Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[AttributeIdx] == TRUE) */
    else
    {
      if(TalkerFound == FALSE)
      {
        FreeAttributeIdx = AttributeIdx;
      }
    }

    if((TalkerFound == TRUE) && (ListenerIdx < SRP_SUPPORTED_MSRP_ATTRIBUTES))
    {
      break;
    }
  } /* while(AttributeIdx > 0u) */

  /* if attribute was not found and a free attribute is available, register the attribute */
  if(FreeAttributeIdx < SRP_SUPPORTED_MSRP_ATTRIBUTES)
  {
    uint32 Bandwidth = 0;
    boolean TalkerFailedToAdvertise = FALSE;
    boolean TalkerAdvertiseToFailed = FALSE;

    Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[FreeAttributeIdx] = TRUE;
    /* For TalkerFound == TRUE -> FreeAttributeIdx == TalkerAttributeIdx */
    if( (TalkerFound == TRUE) && (ListenerIdx < SRP_SUPPORTED_MSRP_ATTRIBUTES) )
    {
      if( (Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].AttributeType == SRP_MSRP_TALKER_FAILED)
        && (AttributeType == SRP_MSRP_TALKER_ADVERTISE) )
      {
        TalkerFailedToAdvertise = TRUE;
        /* Listener Declaration is set later */
      }
      if( (AttributeType == SRP_MSRP_TALKER_FAILED) &&
        (Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].AttributeType == SRP_MSRP_TALKER_ADVERTISE)
        )
      {
        TalkerAdvertiseToFailed = TRUE;
        /* Listener Declaration is set later */
      }
    }

    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].AttributeType = AttributeType;

    /* set Stream Info */
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.StreamId = StreamInfo.StreamId;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.DataFrameParameters = 
      StreamInfo.DataFrameParameters;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.TSpec = StreamInfo.TSpec;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.Priority = StreamInfo.Priority;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.Rank = StreamInfo.Rank;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.AccumulatedLatency = 
      StreamInfo.AccumulatedLatency;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.FailureInformation = 
      StreamInfo.FailureInformation;

    if((LeaveAll == TRUE) && (TalkerFound == TRUE))
    {
      /* trigger rLA! first */
      /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
      Srp_ProcessSmRegistrar((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, PortIdx,
        FreeAttributeIdx, SRP_MRP_EVENT_R_LA);
    
      /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
      Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, PortIdx,
        FreeAttributeIdx, SRP_MRP_EVENT_R_LA);
    }

    /* check if bandwidth was reserved and is not needed anymore */
    if(TalkerAdvertiseToFailed == TRUE)
    {
      if(Srp_ComputeRequiredBandwidth(PortIdx, FreeAttributeIdx, &Bandwidth) == E_OK)
      {
        if(ListenerIdx < SRP_SUPPORTED_MSRP_ATTRIBUTES)
        {
          /* Set Listener declaration to listener asking failed */
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[ListenerIdx].ListenerDeclaration = 
          SRP_DECLARATION_TYPE_LISTENER_ASKING_FAILED;
          /* Release bandwidth */
          (void)Srp_ReleaseBandwidthRx(PortIdx, Bandwidth);
        }
      }
    }

    /* check and reserve bandwidth */
    if(((TalkerFound == FALSE) && (AttributeType == SRP_MSRP_TALKER_ADVERTISE)) || (TalkerFailedToAdvertise == TRUE))
    {
      if(ListenerIdx < SRP_SUPPORTED_MSRP_ATTRIBUTES)
      {
        if(Srp_CheckBandwidthLimitRx(PortIdx, Bandwidth, TRUE) == E_OK)
        {
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[ListenerIdx].ListenerDeclaration = 
            SRP_DECLARATION_TYPE_LISTENER_READY;
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[ListenerIdx].ReservedRxBandwidth = Bandwidth;
        }
        else
        {
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[ListenerIdx].ListenerDeclaration = 
            SRP_DECLARATION_TYPE_LISTENER_ASKING_FAILED;
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[ListenerIdx].ReservedRxBandwidth = 0u;
        }
      }
    }

    if(TalkerFound == FALSE)
    {
      Srp_InitSmRegistrar(PortIdx, &Srp_PortState[PortIdx].MsrpParticipant.RegistrarSM[FreeAttributeIdx]);
      /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
      Srp_ProcessSmRegistrar((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, PortIdx, 
        FreeAttributeIdx, AttributeEvent);
    }
    else
    {
      /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
      Srp_ProcessSmRegistrar((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, PortIdx, 
        FreeAttributeIdx, AttributeEvent);
      /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
      Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, PortIdx, 
        FreeAttributeIdx, AttributeEvent);
    }
  }

} /* Srp_ProcessAttributeTalker() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
/* PRQA L: STPTH */
/* PRQA L: STCYC */
/* PRQA L: STMIF */
/* PRQA L: STCAL */

/**********************************************************************************************************************
 *  Srp_ProcessAttributeListener()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessAttributeListener(Srp_PortIdxType PortIdx, Srp_StreamIdType StreamID, 
  Srp_AttributeEventType AttributeEvent, boolean LeaveAll, Srp_ListenerDeclarationType ListenerDeclaration)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType AttributeIdx = SRP_SUPPORTED_MMRP_ATTRIBUTES;
  Srp_AttributeIdxType FreeAttributeIdx = SRP_INV_ATTRIBUTE_IDX;

  /* ----- Implementation ----------------------------------------------- */

  /* check if AttributeEvent is valid */
  if(AttributeEvent > 5u)
  {
    /* invalid AttributeEvent */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_PROCESS_ATTRIBUTE_LISTENER, SRP_E_INV_PARAM);
    return;
  }

  /* check if attribute is existing or if a free attribute is available */
  while(AttributeIdx > 0u)
  {
    AttributeIdx--;
    if(Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[AttributeIdx] == TRUE)
    {
      /* PRQA S 3415 3 */ /* MD_Srp_12.4 */
      if( (Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].AttributeType == SRP_MSRP_LISTENER) &&
        (Srp_EqualsStreamId(Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.StreamId, 
        StreamID) == TRUE) )
      {
        /* Attribute found --> process the state machines */
        FreeAttributeIdx = SRP_INV_ATTRIBUTE_IDX;

        if(LeaveAll == TRUE)
        {
          /* trigger rLA! first */
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmRegistrar((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, PortIdx, 
            AttributeIdx, SRP_MRP_EVENT_R_LA);
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, PortIdx, 
            AttributeIdx, SRP_MRP_EVENT_R_LA);
        }
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_ProcessSmRegistrar((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, PortIdx, 
          AttributeIdx, AttributeEvent);
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, PortIdx, 
          AttributeIdx, AttributeEvent);
        break;
      }
    }
    else
    {
      FreeAttributeIdx = AttributeIdx;
    }
  }

  /* if attribute was not found and a free attribute is available, register the attribute */
  if(FreeAttributeIdx != SRP_INV_ATTRIBUTE_IDX)
  {
    Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[FreeAttributeIdx] = TRUE;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].AttributeType = SRP_MSRP_LISTENER;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.StreamId = StreamID;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].ListenerDeclaration = ListenerDeclaration;

    Srp_InitSmRegistrar(PortIdx, &Srp_PortState[PortIdx].MsrpParticipant.RegistrarSM[FreeAttributeIdx]);
    /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
    Srp_ProcessSmRegistrar((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, PortIdx, 
      AttributeIdx, AttributeEvent);

  }
} /* Srp_ProcessAttributeListener */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_ProcessAttributeDomain()
 *********************************************************************************************************************/
SRP_LOCAL FUNC(void, SRP_CODE) Srp_ProcessAttributeDomain(Srp_PortIdxType PortIdx, Srp_MsrpDomainInfoType DomainInfo, 
  Srp_AttributeEventType AttributeEvent, boolean LeaveAll)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType AttributeCnt = 0;

  /* ----- Implementation ----------------------------------------------- */

  /* check if AttributeEvent is valid */
  if(AttributeEvent > 5u)
  {
    /* invalid AttributeEvent */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_PROCESS_ATTRIBUTE_DOMAIN, SRP_E_INV_PARAM);
    return;
  }

  /* check if Domain Attribute is existing */
  while(AttributeCnt < SRP_SUPPORTED_MSRP_ATTRIBUTES)
  {

    if((Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[AttributeCnt] == TRUE) && 
      (Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeCnt].AttributeType == SRP_MSRP_DOMAIN) && 
      (Srp_PortState[PortIdx].MsrpParticipant.ApplicantSM[AttributeCnt].SmIsValid == TRUE))
    {
      /* Domain Attribute found --> check if the are identical */

      if( (Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeCnt].DomainInfo.SrClassId == DomainInfo.SrClassId)
        && (Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeCnt].DomainInfo.SrClassPriority == 
        DomainInfo.SrClassPriority) && 
        (Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeCnt].DomainInfo.SrClassVid == 
        DomainInfo.SrClassVid) )
      {
        /* DomainInfo equals DomainInfo of Port --> register the received DomainInfo (in Registrar SM) */

        if(LeaveAll == TRUE)
        {
          /* trigger rLA! first */
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmRegistrar((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, PortIdx, 
            AttributeCnt, SRP_MRP_EVENT_R_LA);
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, PortIdx, 
            AttributeCnt, SRP_MRP_EVENT_R_LA);
        }
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, PortIdx,
          AttributeCnt, AttributeEvent);
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_ProcessSmRegistrar((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, PortIdx,
          AttributeCnt, AttributeEvent);
        break;
      }
    }
    AttributeCnt++;
  }
} /* Srp_ProcessAttributeDomain() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_IterateAttributesAndProcessApplicantSm()
 *********************************************************************************************************************/
/* PRQA S 6030 STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 STMIF */ /* MD_MSR_STMIF */
LOCAL_INLINE FUNC(void, SRP_CODE) Srp_IterateAttributesAndProcessApplicantSm(
  SRP_P2VAR(Srp_MrpParticipantType) MrpParticipantPtr, Srp_PortIdxType PortIdx, Srp_MrpEventType Event, 
  boolean ProcessRegistrar)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType attributeCnt = 0;
  SRP_P2CONST(boolean) attributeValidPtr = NULL_PTR;

  /* ----- Implementation ----------------------------------------------- */
  switch(MrpParticipantPtr->MrpApplication)
  {
#if (SRP_MMRP_SUPPORT == STD_ON)
  case SRP_MMRP_APPLICATION:
    {
      /* MMRP Application */
      /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
      SRP_P2VAR(Srp_MmrpParticipantType) MmrpPtr = (SRP_P2VAR(Srp_MmrpParticipantType))MrpParticipantPtr;
      attributeCnt = SRP_SUPPORTED_MMRP_ATTRIBUTES;
      attributeValidPtr = MmrpPtr->AttributeIsValid;
    }
    break;
#endif /* (SRP_MMRP_SUPPORT == STD_ON) */

  case SRP_MVRP_APPLICATION:
    {
      /* MVRP Application */
      /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
      SRP_P2VAR(Srp_MvrpParticipantType) MvrpPtr = (SRP_P2VAR(Srp_MvrpParticipantType))MrpParticipantPtr;
      attributeCnt = SRP_SUPPORTED_MVRP_ATTRIBUTES;
      attributeValidPtr = MvrpPtr->AttributeIsValid;
    }
    break;

  case SRP_MSRP_APPLICATION:
    {
      /* MSRP Application */
      /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
      SRP_P2VAR(Srp_MsrpParticipantType) MsrpPtr = (SRP_P2VAR(Srp_MsrpParticipantType))MrpParticipantPtr;
      attributeCnt = SRP_SUPPORTED_MSRP_ATTRIBUTES;
      attributeValidPtr = MsrpPtr->AttributeIsValid;
    }
    break;

  default:
    /* Invalid MRP Participant -> Nothing to do here */
    break;
  }

  if((attributeCnt > 0) && (attributeValidPtr != NULL_PTR))
  {
    Srp_AttributeIdxType attributeIdx;
    for(attributeIdx = 0; attributeIdx < attributeCnt; attributeIdx++)
    {
      if(attributeValidPtr[attributeIdx] == TRUE)
      {
        Srp_ProcessSmApplicant(MrpParticipantPtr, PortIdx, attributeIdx, Event);
        if(ProcessRegistrar == TRUE)
        {
          Srp_ProcessSmRegistrar(MrpParticipantPtr, PortIdx, attributeIdx, Event);
        }
      }
    }
  }
} /* Srp_IterateAttributesAndProcessApplicantSm */
/* PRQA L: STCYC */
/* PRQA L: STMIF */

/**********************************************************************************************************************
 *  Srp_CopyMacToBuffer()
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SRP_CODE) Srp_CopyMacToBuffer(SRP_P2CONST(uint8) MacAddressPtr, 
  SRP_P2VAR(uint8) TxBufU8Ptr, SRP_P2VAR(uint16) Offset)
{
  uint8 Index = 0;

  while(Index < 6u)
  {
    TxBufU8Ptr[*Offset] = MacAddressPtr[Index];
    (*Offset)++;
    Index++;
  }
} /* Srp_CopyMacToBuffer() */

/**********************************************************************************************************************
 *  Srp_IncrementMacAddress()
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SRP_CODE) Srp_IncrementMacAddress(SRP_P2VAR(uint8) MacAddressPtr)
{
  sint8 Index = 5;

  while(Index >= 0)
  {
    /*
     * Because the MacAddresses passed here are Multi-cast Addresses (01:xx:xx:xx:xx:xx) the Address will stay within
     * the valid range
     */
    MacAddressPtr[Index] += 1;
    if(MacAddressPtr[Index] != 0x00u)
    {
      break;
    }
    Index--;
  }

} /* Srp_IncrementMacAddress() */

/**********************************************************************************************************************
 *  Srp_GetLeavAllAndNumberOfValues()
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SRP_CODE) Srp_GetLeavAllAndNumberOfValues(SRP_P2CONST(uint8) DataPtr, SRP_P2VAR(uint16) Offset,
  SRP_P2VAR(boolean) LeaveAllEvent, SRP_P2VAR(uint16) NumberOfValues)
{
  /* get Leave All Event and Number of values */
  /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
    *LeaveAllEvent = (boolean)((Srp_Ntohs(*((SRP_P2CONST(uint16))&DataPtr[*Offset])) & 0xE000u) >> 13);
    *NumberOfValues = (uint16)(Srp_Ntohs(*((SRP_P2CONST(uint16))&DataPtr[*Offset])) & 0x1FFFu);
    *Offset += 2;
} /* Srp_GetLeavAllAndNumberOfValues() */

/**********************************************************************************************************************
 *  Srp_GetMacAddress()
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, SRP_CODE) Srp_GetMacAddress(SRP_P2CONST(uint8) DataPtr, SRP_P2VAR(uint16) Offset, 
  SRP_P2VAR(Srp_MacAddressType) MacAddress)
{
  (*MacAddress)[0] = DataPtr[*Offset];
  *Offset += 1;
  (*MacAddress)[1] = DataPtr[*Offset];
  *Offset += 1;
  (*MacAddress)[2] = DataPtr[*Offset];
  *Offset += 1;
  (*MacAddress)[3] = DataPtr[*Offset];
  *Offset += 1;
  (*MacAddress)[4] = DataPtr[*Offset];
  *Offset += 1;
  (*MacAddress)[5] = DataPtr[*Offset];
  *Offset += 1;
} /* Srp_GetMacAddress() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Srp_InitMemory()
 *********************************************************************************************************************/
/*! \brief      Function for *_INIT_*-variable initialization
 *  \details    Service to initialize module global variables at power up. This function can be used to initialize the
 *              variables in *_INIT_* sections in case they are not initialized by the startup code.
 *  \pre        Module must not be initialized
 *  \pre        Function shall be called from task level
 *  \note       This function mus be called before using the module
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
#if (SRP_DEV_ERROR_DETECT == STD_ON)
  Srp_State = SRP_STATE_UNINIT;
#endif /* (SRP_DEV_ERROR_DETECT == STD_ON) */

} /* Srp_InitMemory() */

/**********************************************************************************************************************
 * Srp_Init()
 *********************************************************************************************************************/
/*! \brief      Initialization function
 *  \details    This function initializes the module Srp. It initializes all variables and sets the module state to
 *              initialized.
 *  \param[in]  ConfigPtr               Configuration structure for initializing the module
 *  \pre        Interrupts have to be disabled.
 *  \pre        The module has to be uninitialized.
 *  \pre        Srp_InitMemory has been called unless Srp_State is initialized by start-up code.
 *  \note       Specification of module initialization
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_Init(SRP_P2CONSTCFG(Srp_ConfigType) ConfigPtr)
{
  /* ----- Local Variables -----------------------------------------------*/
  Srp_PortIdxType PortIdx = SRP_PORT_COUNT; /* PRQA S 0781 */ /* MD_Srp_5.6 */

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  /* Check initialization state of the component */
  Srp_CheckDetErrorReturnVoid(SRP_COND_UNINIT, SRP_SID_INIT, SRP_E_ALREADY_INITIALIZED); 
# if (SRP_CONFIGURATION_VARIANT == SRP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
  /* Check parameter 'ConfigPtr' */
  Srp_CheckDetErrorReturnVoid(SRP_COND_VALID_PTR(ConfigPtr), SRP_SID_INIT, SRP_E_NULL_POINTER);
# endif /* (SRP_CONFIGURATION_VARIANT == SRP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE) */
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  /* PRQA S 3112 1 */ /* MD_MSR_14.2 */
  SRP_DUMMY_STATEMENT(ConfigPtr);

  /* init all ports */
  while(PortIdx > 0u)
  {
    PortIdx--;
    Srp_InitPort(PortIdx);
  }

#if (SRP_DEV_ERROR_DETECT == STD_ON)
  Srp_State = SRP_STATE_INIT;
#endif /* (SRP_DEV_ERROR_DETECT == STD_ON) */

} /* Srp_Init() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_MainFunction()
 *********************************************************************************************************************/
/*! \brief      Processing of cyclic tasks of the Srp module
 *  \param      none
 *  \return     none
 *  \pre        Module has to be initialized.
 *  \context    This function can be called in any context.
 *********************************************************************************************************************/
/* PRQA S 6010 STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6050 STCAL */ /* MD_MSR_STCAL */
/* PRQA S 6080 STMIF */ /* MD_MSR_STMIF */
FUNC(void, SRP_CODE) Srp_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_PortIdxType PortIdx = SRP_PORT_COUNT; /* PRQA S 0781 */ /* MD_Srp_5.6 */
  Srp_AttributeIdxType AttributeIndex;
  boolean TransmitOpportunity = FALSE;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  /* Check initialization state of the component */
  Srp_CheckDetErrorReturnVoid(SRP_COND_INIT, SRP_SID_MAIN_FUNCTION, SRP_E_UNINIT);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  SRP_ENTER_CRITICAL_SECTION_0();
  /* Iterate all Srp Ports */
  while(PortIdx > 0u)
  {
    PortIdx--;
    if(Srp_PortState[PortIdx].Enabled == TRUE)
    {
#     if (SRP_MMRP_SUPPORT == STD_ON)
      /* Process MMRP Participant */

      /* try to transmit 'old' message if available */
      if((Srp_PortState[PortIdx].MmrpParticipant.BufIdx != SRP_INV_BUF_IDX) && 
        (Srp_PortState[PortIdx].MmrpParticipant.BufPtr != NULL_PTR))
      {
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        (void)Srp_Transmit(PortIdx, (SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant,
          SRP_MMRP_ETHER_TYPE, FALSE, Srp_MmrpDestinationMac);
      }

      /* decrement Join counter */
      if(Srp_PortState[PortIdx].MmrpParticipant.JoinTimer > 0u)
      {
        Srp_PortState[PortIdx].MmrpParticipant.JoinTimer--;
      }
      /* decrement LeaveAll counter */
      if((Srp_PortState[PortIdx].MmrpParticipant.LeaveAllSM.LeaveAllTimer > 0u) && 
        (Srp_PortState[PortIdx].MmrpParticipant.LeaveAllSM.LeaveAllTimerRunning == TRUE))
      {
        Srp_PortState[PortIdx].MmrpParticipant.LeaveAllSM.LeaveAllTimer--;
        /* if LeaveAll counter == 0 process LeaveAll state machine */
        /* PRQA S 3415 2 */ /* MD_Srp_12.4 */
        if(Srp_PortState[PortIdx].MmrpParticipant.LeaveAllSM.LeaveAllTimer == 0u)
        {
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmLeaveAll((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant,
            PortIdx, SRP_MRP_EVENT_LEAVE_ALL_TIMER);

          if(Srp_MrpHasAtLeastOneValidAttribute(PortIdx, SRP_MMRP_APPLICATION) == TRUE)
          {
            /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
            if(Srp_RequestTransmitOpportunity(PortIdx, (
                SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant) == E_OK)
            {
              /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
              Srp_ProcessSmLeaveAll((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant,
                PortIdx, SRP_MRP_EVENT_TX);
              /* trigger transmit for MMRP "leaveAll" messages */
              /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
              (void)Srp_Transmit(PortIdx, (SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant, 
                SRP_MMRP_ETHER_TYPE, FALSE, Srp_MmrpDestinationMac);
            }
          }
        } /* EndIf: LeaveAllTimer == 0 */
      }
      
      /* decrement periodic counter */
      if((Srp_PortState[PortIdx].MmrpParticipant.PeriodicTransmissionSM.PeriodicTimer > 0u) &&
        (Srp_PortState[PortIdx].MmrpParticipant.PeriodicTransmissionSM.PeriodicTimerRunning == TRUE))
      {
        Srp_PortState[PortIdx].MmrpParticipant.PeriodicTransmissionSM.PeriodicTimer--;
        /* if periodic counter == 0 process periodic transmission state machine */
        /* PRQA S 3415 2 */ /* MD_Srp_12.4 */
        if((Srp_PortState[PortIdx].MmrpParticipant.PeriodicTransmissionSM.PeriodicTimer == 0u) && 
          (Srp_MrpHasAtLeastOneValidAttribute(PortIdx, SRP_MMRP_APPLICATION) == TRUE))
        {
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmPeriodicTransmission((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant,
            PortIdx, SRP_MRP_EVENT_PERIODIC_TIMER);
        }
      }
      
      /* Iterate all Attributes */
      AttributeIndex = SRP_SUPPORTED_MMRP_ATTRIBUTES;
      while(AttributeIndex > 0u)
      {
        AttributeIndex--;
        if(Srp_PortState[PortIdx].MmrpParticipant.AttributeIsValid[AttributeIndex] == TRUE)
        {
          /* Applicant state machine */
          if( ( (Srp_PortState[PortIdx].MmrpParticipant.JoinTimer == 0u) || 
            (Srp_PortState[PortIdx].MmrpParticipant.ApplicantSM[AttributeIndex].RequestTx == TRUE) ) &&
            (Srp_PortState[PortIdx].MmrpParticipant.ApplicantSM[AttributeIndex].SmIsValid == TRUE) )
          {
            /* PRQA S 0310 2 */ /* MD_Srp_11.4 */ /* PRQA S 3415 2 */ /* MD_Srp_12.4 */
            if((TransmitOpportunity == FALSE) && (Srp_RequestTransmitOpportunity(PortIdx, 
               (SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant) == E_OK))
            {
              TransmitOpportunity = TRUE;
            }
            if(TransmitOpportunity == TRUE)
            {
              /* Trigger tx! event */
              /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
              Srp_ProcessSmApplicant(((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant), 
                PortIdx, AttributeIndex, SRP_MRP_EVENT_TX);
            }
          }
          /* Registrar state machine */
          if(Srp_PortState[PortIdx].MmrpParticipant.RegistrarSM[AttributeIndex].LeaveTimerRunning == TRUE)
          {
            Srp_PortState[PortIdx].MmrpParticipant.RegistrarSM[AttributeIndex].LeaveTimer--;
            if(Srp_PortState[PortIdx].MmrpParticipant.RegistrarSM[AttributeIndex].LeaveTimer == 0u)
            {
              /* Trigger leavetimer! event */
              /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
              Srp_ProcessSmRegistrar(((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant), 
                PortIdx, AttributeIndex, SRP_MRP_EVENT_LEAVE_TIMER);
            }
          }
        }
      } /* END: while(AttributeIndex > 0u) */
      
      if(Srp_PortState[PortIdx].MmrpParticipant.JoinTimer == 0u)
      {
        /* Reset Join Timer */
        Srp_PortState[PortIdx].MmrpParticipant.JoinTimer = Srp_PortConfig[PortIdx].JoinTime;
      }
      if(TransmitOpportunity == TRUE)
      {
        /* Trigger Transmit for MMRP messages */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        (void)Srp_Transmit(PortIdx, (SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant, 
          SRP_MMRP_ETHER_TYPE, FALSE, Srp_MmrpDestinationMac);
        TransmitOpportunity = FALSE;
      }
#     endif /* (SRP_MMRP_SUPPORT == STD_ON) */
      
      /* Process MVRP Participant */

      /* try to transmit 'old' message if available */
      if((Srp_PortState[PortIdx].MvrpParticipant.BufIdx != SRP_INV_BUF_IDX) && 
        (Srp_PortState[PortIdx].MvrpParticipant.BufPtr != NULL_PTR))
      {
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        (void)Srp_Transmit(PortIdx, (SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MvrpParticipant,
          SRP_MVRP_ETHER_TYPE, FALSE, Srp_MvrpDestinationMac);
      }
      
      /* decrement Join counter */
      if(Srp_PortState[PortIdx].MvrpParticipant.JoinTimer > 0u)
      {
        Srp_PortState[PortIdx].MvrpParticipant.JoinTimer--;
      }
      
      /* decrement LeaveAll counter */
      if((Srp_PortState[PortIdx].MvrpParticipant.LeaveAllSM.LeaveAllTimer > 0u) && 
        (Srp_PortState[PortIdx].MvrpParticipant.LeaveAllSM.LeaveAllTimerRunning == TRUE))
      {
        Srp_PortState[PortIdx].MvrpParticipant.LeaveAllSM.LeaveAllTimer--;
        /* if LeaveAll counter == 0 process LeaveAll state machine */
        if(Srp_PortState[PortIdx].MvrpParticipant.LeaveAllSM.LeaveAllTimer == 0u)
        {
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmLeaveAll((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MvrpParticipant,
            PortIdx, SRP_MRP_EVENT_LEAVE_ALL_TIMER);

          if(Srp_MrpHasAtLeastOneValidAttribute(PortIdx, SRP_MVRP_APPLICATION) == TRUE)
          {
            /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
            if(Srp_RequestTransmitOpportunity(PortIdx,
              (SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MvrpParticipant) == E_OK)
            {
              /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
              Srp_ProcessSmLeaveAll((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MvrpParticipant,
                PortIdx, SRP_MRP_EVENT_TX);
              /* trigger transmit for MVRP "leaveAll" messages */
              /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
              (void)Srp_Transmit(PortIdx, (SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MvrpParticipant, 
                SRP_MVRP_ETHER_TYPE, FALSE, Srp_MvrpDestinationMac);
            }
          }
        }
      }
      
      /* decrement periodic counter */
      if((Srp_PortState[PortIdx].MvrpParticipant.PeriodicTransmissionSM.PeriodicTimer > 0u) &&
        (Srp_PortState[PortIdx].MvrpParticipant.PeriodicTransmissionSM.PeriodicTimerRunning == TRUE))
      {
        Srp_PortState[PortIdx].MvrpParticipant.PeriodicTransmissionSM.PeriodicTimer--;
        /* if periodic counter == 0 process periodic transmission state machine */
        /* PRQA S 3415 2 */ /* MD_Srp_12.4 */
        if((Srp_PortState[PortIdx].MvrpParticipant.PeriodicTransmissionSM.PeriodicTimer == 0u) && 
          (Srp_MrpHasAtLeastOneValidAttribute(PortIdx, SRP_MVRP_APPLICATION) == TRUE))
        {
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmPeriodicTransmission((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MvrpParticipant,
            PortIdx, SRP_MRP_EVENT_PERIODIC_TIMER);
        }
      }
      
      /* Iterate all Attributes */
      AttributeIndex = SRP_SUPPORTED_MVRP_ATTRIBUTES;
      while(AttributeIndex > 0u)
      {
        AttributeIndex--;
        if(Srp_PortState[PortIdx].MvrpParticipant.AttributeIsValid[AttributeIndex] == TRUE)
        {
          /* Applicant state machine */
          if( ( (Srp_PortState[PortIdx].MvrpParticipant.JoinTimer == 0u) || 
            (Srp_PortState[PortIdx].MvrpParticipant.ApplicantSM[AttributeIndex].RequestTx == TRUE) ) &&
            (Srp_PortState[PortIdx].MvrpParticipant.ApplicantSM[AttributeIndex].SmIsValid == TRUE) )
          {
            /* PRQA S 0310 2 */ /* MD_Srp_11.4 */ /* PRQA S 3415 2 */ /* MD_Srp_12.4 */
            if((TransmitOpportunity == FALSE) && (Srp_RequestTransmitOpportunity(PortIdx, 
               (SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MvrpParticipant) == E_OK))
            {
              TransmitOpportunity = TRUE;
            }
            if(TransmitOpportunity == TRUE)
            {
              /* trigger tx! event */
              /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
              Srp_ProcessSmApplicant(((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MvrpParticipant), 
                PortIdx, AttributeIndex, SRP_MRP_EVENT_TX);
            }
          }
          /* registrar state machine */
          if(Srp_PortState[PortIdx].MvrpParticipant.RegistrarSM[AttributeIndex].LeaveTimerRunning == TRUE)
          {
            Srp_PortState[PortIdx].MvrpParticipant.RegistrarSM[AttributeIndex].LeaveTimer--;
            if(Srp_PortState[PortIdx].MvrpParticipant.RegistrarSM[AttributeIndex].LeaveTimer == 0u)
            {
              /* Trigger leavetimer! event */
              /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
              Srp_ProcessSmRegistrar(((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MvrpParticipant), 
                PortIdx, AttributeIndex, SRP_MRP_EVENT_LEAVE_TIMER);
            }
          }
        }
      } /* END: while(AttributeIndex > 0u) */
      
      if(Srp_PortState[PortIdx].MvrpParticipant.JoinTimer == 0u)
      {
        /* reset Join Timer */
        Srp_PortState[PortIdx].MvrpParticipant.JoinTimer = Srp_PortConfig[PortIdx].JoinTime;
      }
      if(TransmitOpportunity == TRUE)
      {
        /* Trigger Transmit for MVRP messages */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        (void)Srp_Transmit(PortIdx, (SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MvrpParticipant, 
          SRP_MVRP_ETHER_TYPE, FALSE, Srp_MvrpDestinationMac);
        TransmitOpportunity = FALSE;
      }
      
      /* Process MSRP Participant */

      /* try to transmit 'old' message if available */
      if((Srp_PortState[PortIdx].MsrpParticipant.BufIdx != SRP_INV_BUF_IDX) && 
        (Srp_PortState[PortIdx].MsrpParticipant.BufPtr != NULL_PTR))
      {
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        (void)Srp_Transmit(PortIdx, (SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant,
          SRP_MSRP_ETHER_TYPE, FALSE, Srp_MsrpDestinationMac);
      }
      
      /* decrement Join counter */
      if(Srp_PortState[PortIdx].MsrpParticipant.JoinTimer > 0u)
      {
        Srp_PortState[PortIdx].MsrpParticipant.JoinTimer--;
      }
      
      /* decrement LeaveAll counter */
      if((Srp_PortState[PortIdx].MsrpParticipant.LeaveAllSM.LeaveAllTimer > 0u) && 
        (Srp_PortState[PortIdx].MsrpParticipant.LeaveAllSM.LeaveAllTimerRunning == TRUE))
      {
        Srp_PortState[PortIdx].MsrpParticipant.LeaveAllSM.LeaveAllTimer--;
        /* if LeaveAll counter == 0 process LeaveAll state machine */
        if(Srp_PortState[PortIdx].MsrpParticipant.LeaveAllSM.LeaveAllTimer == 0u)
        {
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmLeaveAll((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant,
            PortIdx, SRP_MRP_EVENT_LEAVE_ALL_TIMER);

          if(Srp_MrpHasAtLeastOneValidAttribute(PortIdx, SRP_MSRP_APPLICATION) == TRUE)
          {
            /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
            if(Srp_RequestTransmitOpportunity(PortIdx, (
                SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant) == E_OK)
            {
              /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
              Srp_ProcessSmLeaveAll((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant,
              PortIdx, SRP_MRP_EVENT_TX);
              /* trigger transmit for MSRP "leaveAll" messages */
              /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
              (void)Srp_Transmit(PortIdx, (SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, 
                SRP_MSRP_ETHER_TYPE, FALSE, Srp_MsrpDestinationMac);
            }
          }
        }
      }
      
      /* decrement periodic counter */
      if((Srp_PortState[PortIdx].MsrpParticipant.PeriodicTransmissionSM.PeriodicTimer > 0u) &&
        (Srp_PortState[PortIdx].MsrpParticipant.PeriodicTransmissionSM.PeriodicTimerRunning == TRUE))
      {
        Srp_PortState[PortIdx].MsrpParticipant.PeriodicTransmissionSM.PeriodicTimer--;
        /* if periodic counter == 0 process periodic transmission state machine */
        /* PRQA S 3415 2 */ /* MD_Srp_12.4 */
        if((Srp_PortState[PortIdx].MsrpParticipant.PeriodicTransmissionSM.PeriodicTimer == 0) && 
          (Srp_MrpHasAtLeastOneValidAttribute(PortIdx, SRP_MSRP_APPLICATION) == TRUE))
        {
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmPeriodicTransmission((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant,
            PortIdx, SRP_MRP_EVENT_PERIODIC_TIMER);
        }
      }
      
      /* Iterate all Attributes */
      AttributeIndex = SRP_SUPPORTED_MSRP_ATTRIBUTES;
      while(AttributeIndex > 0u)
      {
        AttributeIndex--;
        if(Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[AttributeIndex] == TRUE)
        {
          /* Applicant state machine */
          if( ( (Srp_PortState[PortIdx].MsrpParticipant.JoinTimer == 0u) || 
            (Srp_PortState[PortIdx].MsrpParticipant.ApplicantSM[AttributeIndex].RequestTx == TRUE) ) &&
            (Srp_PortState[PortIdx].MsrpParticipant.ApplicantSM[AttributeIndex].SmIsValid == TRUE) )
          {
            /* PRQA S 0310 2 */ /* MD_Srp_11.4 */ /* PRQA S 3415 2 */ /* MD_Srp_12.4 */
            if((TransmitOpportunity == FALSE) && (Srp_RequestTransmitOpportunity(PortIdx, 
               (SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant) == E_OK))
            {
              TransmitOpportunity = TRUE;
            }
            if(TransmitOpportunity == TRUE)
            {
              /* trigger tx! event */
              /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
              Srp_ProcessSmApplicant(((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant), 
                PortIdx, AttributeIndex, SRP_MRP_EVENT_TX);
            }
          }
          /* Registrar state machine */
          if(Srp_PortState[PortIdx].MsrpParticipant.RegistrarSM[AttributeIndex].LeaveTimerRunning == TRUE)
          {
            Srp_PortState[PortIdx].MsrpParticipant.RegistrarSM[AttributeIndex].LeaveTimer--;
            if(Srp_PortState[PortIdx].MsrpParticipant.RegistrarSM[AttributeIndex].LeaveTimer == 0u)
            {
              /* Trigger leavetimer! event */
              /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
              Srp_ProcessSmRegistrar(((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant), 
                PortIdx, AttributeIndex, SRP_MRP_EVENT_LEAVE_TIMER);
            }
          }
        }
      }
      if(Srp_PortState[PortIdx].MsrpParticipant.JoinTimer == 0u)
      {
        /* reset Join Timer */
        Srp_PortState[PortIdx].MsrpParticipant.JoinTimer = Srp_PortConfig[PortIdx].JoinTime;
      }
      if(TransmitOpportunity == TRUE)
      {
        /* Trigger Transmit for MSRP messages */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        (void)Srp_Transmit(PortIdx, (SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, 
          SRP_MSRP_ETHER_TYPE, FALSE, Srp_MsrpDestinationMac);
        TransmitOpportunity = FALSE;
      }
    } /* END: if(Srp_PortState[PortIdx].Enabled == TRUE) */
  } /* END: while(PortIdx > 0u) */

  SRP_LEAVE_CRITICAL_SECTION_0();

} /* Srp_MainFunction() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* multiple return paths exist in DET checks */
/* PRQA L: STPTH */
/* PRQA L: STCYC */
/* PRQA L: STCAL */
/* PRQA L: STMIF */

#if (SRP_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Srp_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief      Returns the version information
 *  \details    Srp_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out] VersioninfoPtr             Pointer to where to store the version information
 *  \pre        Input parameter must not be NULL.
 *  \pre        Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_GetVersionInfo(SRP_P2VAR(Std_VersionInfoType) VersioninfoPtr)
{
  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Srp_CheckDetErrorReturnVoid(NULL_PTR != VersioninfoPtr, SRP_SID_GET_VERSION_INFO, SRP_E_NULL_POINTER); 

  /* ----- Implementation ----------------------------------------------- */
  VersioninfoPtr->vendorID = (SRP_VENDOR_ID);
  VersioninfoPtr->moduleID = (SRP_MODULE_ID);
  VersioninfoPtr->sw_major_version = (SRP_SW_MAJOR_VERSION);
  VersioninfoPtr->sw_minor_version = (SRP_SW_MINOR_VERSION);
  VersioninfoPtr->sw_patch_version = (SRP_SW_PATCH_VERSION);

} /* Srp_GetVersionInfo() */  /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

/**********************************************************************************************************************
 *  Srp_RegisterStream
 *********************************************************************************************************************/
/*! \brief          Allows the Application to offer a new Stream
 *  \param[in]      PortIdx                Port Index
 *  \param[in]      StreamInfoPtr          Pointer to a structure containing the following Information of the offered 
 *                                         Stream:
 *                    StreamID               The unique identifier of the offered Stream
 *                    DataFrameParameters    Identifying all frames belonging to the same Stream
 *                    TSpec                  The Traffic Specification for the Stream containing the MaxFrameSize and
                                             the MaxIntervalFrames
 *                    DataFramePriority      The Priority of the Stream
 *                    Rank                   Marking emergency data
 *                    AccumulatedLatency     Marking worst-case latency in its path from Talker to Listener
 *  \return         E_OK                   Successfully registered new Stream
 *                  E_NOT_OK               Failed to register new Stream
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
/* PRQA S 6060 STPAR */ /* MD_MSR_STPAR */
/* PRQA S 6080 STMIF */ /* MD_MSR_STMIF */
/* PRQA S 6010 STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 STCYC */ /* MD_MSR_STCYC */
FUNC(Std_ReturnType, SRP_CODE) Srp_RegisterStream(Srp_PortIdxType PortIdx,
  SRP_P2CONST(Srp_MsrpRegisterStreamInfoType) StreamInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType AttributeIdx = SRP_SUPPORTED_MSRP_ATTRIBUTES;
  Srp_AttributeIdxType FreeAttributeIdx = SRP_INV_ATTRIBUTE_IDX;
  uint32 NecessaryBandwidth = 0;
  Std_ReturnType RetVal = E_NOT_OK;
  boolean InitRegistrarSm = TRUE;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  /* Check initialization state of the component */
  Srp_CheckDetErrorReturnValue(SRP_COND_INIT, SRP_SID_REGISTER_STREAM, SRP_E_UNINIT, RetVal);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  /* iterate all attributes */
  while(AttributeIdx > 0u)
  {
    AttributeIdx--;
    if(Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[AttributeIdx] == TRUE)
    {
      if((Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_ADVERTISE) ||
        (Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_FAILED))
      {
        if(Srp_EqualsStreamId(Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.StreamId,
          StreamInfoPtr->StreamId) == TRUE)
        {
          if(Srp_PortState[PortIdx].MsrpParticipant.ApplicantSM[AttributeIdx].SmIsValid == TRUE)
          {
            /* Stream is already registered */
          RetVal = E_NOT_OK;
          break;
          }
          else
          {
            /* Attribute for Stream is existing but the Stream is not registered yet */
            InitRegistrarSm = FALSE;
            RetVal = E_OK;
            FreeAttributeIdx = AttributeIdx;
            break; /* PRQA S 0771 */ /* MD_Srp_14.6 */
          }
        }
      }
    }
    else
    {
      /* found free attribute. Store index */
      FreeAttributeIdx = AttributeIdx;
      RetVal = E_OK;
    }
  }

  if((RetVal == E_OK) && (FreeAttributeIdx != SRP_INV_ATTRIBUTE_IDX))
  {
    /* Stream is not registered yet and a free attribute was found --> Check Bandwidth and Register the Stream */
    Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[FreeAttributeIdx] = TRUE;

    /* set stream infos */
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.StreamId = StreamInfoPtr->StreamId;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.DataFrameParameters = 
      StreamInfoPtr->DataFrameParameters;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.TSpec = StreamInfoPtr->TSpec;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.Priority = StreamInfoPtr->Priority;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.Rank = StreamInfoPtr->Rank;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.AccumulatedLatency = 
      StreamInfoPtr->AccumulatedLatency;

    /* Verify Stream Priority (has to match the configured Domain) */
    if(StreamInfoPtr->Priority == Srp_PortConfig[PortIdx].DomainInfoPtr->SrClassPriority)
    {
      /* check bandwidth */
      if(Srp_ComputeRequiredBandwidth(PortIdx, FreeAttributeIdx, &NecessaryBandwidth) == E_OK)
      {
        if(Srp_CheckBandwidthLimitTx(PortIdx, NecessaryBandwidth, StreamInfoPtr->Priority, TRUE) == E_OK)
        {
          /* enough bandwidth available --> TalkerAdvertise */
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].AttributeType = SRP_MSRP_TALKER_ADVERTISE;
        }
        else
        {
          /* insufficient bandwidth --> TalkerFailed */
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].AttributeType = SRP_MSRP_TALKER_FAILED;
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.FailureInformation.BridgeID =
            0u;
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.FailureInformation.
            ReservationFailuredCode = SRP_FAILURE_CODE_INSUFFICIENT_BANDWITH;
        }
      }
    }
    else
    {
      /* Priority mismatch -> TalkerFailed */
      Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].AttributeType = SRP_MSRP_TALKER_FAILED;
      Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.FailureInformation.BridgeID = 0u;
      Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.FailureInformation.
        ReservationFailuredCode = SRP_FAILURE_CODE_SR_CLASS_PRIORITY_MISMATCH;
    }

    /* Init state machines */
    Srp_InitSmApplicant(&Srp_PortState[PortIdx].MsrpParticipant.ApplicantSM[FreeAttributeIdx]);
    if(InitRegistrarSm == TRUE)
    {
      Srp_InitSmRegistrar(PortIdx, &Srp_PortState[PortIdx].MsrpParticipant.RegistrarSM[FreeAttributeIdx]);
    }

    Srp_PortState[PortIdx].MsrpParticipant.ApplicantSM[FreeAttributeIdx].RegistrarSmPtr = 
      &Srp_PortState[PortIdx].MsrpParticipant.RegistrarSM[FreeAttributeIdx];

    /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
    Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, PortIdx, 
      FreeAttributeIdx, SRP_MRP_EVENT_NEW);
  }
  return RetVal;
} /* Srp_RegisterStream() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
/* PRQA L: STPAR */
/* PRQA L: STMIF */
/* PRQA L: STPTH */
/* PRQA L: STCYC */

/**********************************************************************************************************************
 *  Srp_DeregisterStream
 *********************************************************************************************************************/
/**
 *  \brief          Allows the Application to withdraw the offer of a Stream
 *  \param[in]      PortIdx              Port Index
 *  \param[in]      StreamID             The unique identifier of the Stream
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
/* PRQA S 6080 STMIF */ /* MD_MSR_STMIF */
FUNC(void, SRP_CODE) Srp_DeregisterStream(Srp_PortIdxType PortIdx, Srp_StreamIdType StreamID)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType AttributeIdx = SRP_SUPPORTED_MSRP_ATTRIBUTES;
  uint32 StreamBandwidth;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  /* Check initialization state of the component */
  Srp_CheckDetErrorReturnVoid(SRP_COND_INIT, SRP_SID_DERGISTER_STREAM, SRP_E_UNINIT);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  /* iterate attributes */
  while(AttributeIdx > 0u)
  {
    AttributeIdx--;
    if((Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[AttributeIdx] == TRUE) &&
      (Srp_PortState[PortIdx].MsrpParticipant.ApplicantSM[AttributeIdx].SmIsValid == TRUE))
    {
      /* PRQA S 3415 4 */ /* MD_Srp_12.4 */
      if(((Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_ADVERTISE)
        || (Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_FAILED))
        && (Srp_EqualsStreamId(Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.StreamId, 
        StreamID) == TRUE))
      {
        /* Stream is already registered --> De-register stream and release bandwidth */
        if(Srp_ComputeRequiredBandwidth(PortIdx, AttributeIdx, &StreamBandwidth) == E_OK)
        {
          if(Srp_ReleaseBandwidthTx(PortIdx,
            Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.Priority, StreamBandwidth) ==
            E_OK)
          {
            /* Trigger Lv! event */
            /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
            Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant,
              PortIdx, AttributeIdx, SRP_MRP_EVENT_LV);
          }
        }
        break;
      }
    }
  } /* PRQA S 0771 */ /* MD_Srp_14.6 */

} /* Srp_DeregisterStream() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
/* PRQA L: STMIF */

/**********************************************************************************************************************
 *  Srp_RegisterAttach
 *********************************************************************************************************************/
/**
 *  \brief          Allows the Application to register a offered Stream
 *  \param[in]      PortIdx                    Port Index
 *  \param[in]      StreamID                   The unique identifier of the Stream
 *  \return         E_OK                       Successfully registered the offered Stream
 *                  E_NOT_OK                   Failed to register the offered Stream
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
/* PRQA S 6030 STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6080 STMIF */ /* MD_MSR_STMIF */
/* PRQA S 6010 STPTH */ /* MD_MSR_STPTH */
FUNC(Std_ReturnType, SRP_CODE) Srp_RegisterAttach(Srp_PortIdxType PortIdx, Srp_StreamIdType StreamID)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType AttributeIdx = SRP_SUPPORTED_MSRP_ATTRIBUTES;
  Srp_AttributeIdxType FreeAttributeIdx = SRP_INV_ATTRIBUTE_IDX;
  Std_ReturnType RetVal = E_NOT_OK;
  uint32 Bandwidth = 0;
  Srp_ListenerDeclarationType DeclarationType = SRP_DECLARATION_TYPE_LISTENER_IGNORE;
  boolean InitRegistrarSm = TRUE;
  boolean talkerFound = FALSE;
  boolean listenerFound = FALSE;
  boolean alreadyRegistered = FALSE;
  boolean reserveBandwidthRx = FALSE;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  /* Check initialization state of the component */
  Srp_CheckDetErrorReturnValue(SRP_COND_INIT, SRP_SID_REGISTER_ATTACH, SRP_E_UNINIT, RetVal);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  /* iterate all Attributes */
  while(AttributeIdx > 0u)
  {
    AttributeIdx--;
    if(Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[AttributeIdx] == TRUE)
    {
      if(Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].AttributeType == SRP_MSRP_LISTENER)
      {
        if(Srp_EqualsStreamId(Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.StreamId,
          StreamID) == TRUE)
        {
          if(Srp_PortState[PortIdx].MsrpParticipant.ApplicantSM[AttributeIdx].SmIsValid == TRUE)
          {
            /* Stream is already registered */
            RetVal = E_NOT_OK;
            alreadyRegistered = TRUE;
          }
          else
          {
            /* Attribute for Stream is existing, but Attach is not registered yet */
            InitRegistrarSm = FALSE;
            RetVal = E_OK;
            FreeAttributeIdx = AttributeIdx;
            listenerFound = TRUE;
          }
        }
      }
      else if(
        (Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_ADVERTISE) ||
        (Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_FAILED))
      {
        if(Srp_EqualsStreamId(Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.StreamId,
          StreamID) == TRUE)
        {
          talkerFound = TRUE;
          /* Talker Advertise/Failed for the Stream is existing. Check if it is just registered or "offered" too */
          if(Srp_PortState[PortIdx].MsrpParticipant.ApplicantSM[AttributeIdx].SmIsValid != TRUE)
          {
            /* Stream is just registered */
            if(Srp_PortState[PortIdx].MsrpParticipant.RegistrarSM[AttributeIdx].State == SRP_STATE_REGISTRAR_IN)
            {
              reserveBandwidthRx = TRUE;
            }
          }
          else
          {
            /* Stream is offered as well */
            reserveBandwidthRx = TRUE;
          }

          if(Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].AttributeType == SRP_MSRP_TALKER_FAILED)
          {
            reserveBandwidthRx = FALSE;
          }

          if(Srp_ComputeRequiredBandwidth(PortIdx, AttributeIdx, &Bandwidth) == E_OK)
          {
            if(Srp_CheckBandwidthLimitRx(PortIdx, Bandwidth, reserveBandwidthRx) == E_OK)
            {
              /* sufficient bandwidth available -> Listener Ready */
              DeclarationType = SRP_MSRP_LISTENER_READY;
            }
            else
            {
              /* insufficient bandwidth -> Listener Ready Failed */
              DeclarationType = SRP_MSRP_LISTENER_READY_FAILED;
              Bandwidth = 0u; /* Set Bandwidth to '0' because no bandwidth is reserved */
            }
          }
        }
      }
      else
      {
        /* neither Listener nor Talker Attribute --> not of interest */
      }

      if(((talkerFound == TRUE) && (listenerFound == TRUE)) || (alreadyRegistered == TRUE))
      {
        break;
      }
    }
    else
    {
      /* free attribute found. Store Index */
      FreeAttributeIdx = AttributeIdx;
      RetVal = E_OK;
    }
  }

  if((RetVal == E_OK) && (FreeAttributeIdx != SRP_INV_ATTRIBUTE_IDX))
  {
    /* Stream is not registered yet and a free attribute was found */
    Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[FreeAttributeIdx] = TRUE;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].AttributeType = SRP_MSRP_LISTENER;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].StreamInfo.StreamId = StreamID;
    Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].ListenerDeclaration = DeclarationType;
    if(reserveBandwidthRx == TRUE)
    {
      Srp_PortState[PortIdx].MsrpParticipant.Attribute[FreeAttributeIdx].ReservedRxBandwidth = Bandwidth;
    }

    Srp_InitSmApplicant(&Srp_PortState[PortIdx].MsrpParticipant.ApplicantSM[FreeAttributeIdx]);
    if(InitRegistrarSm == TRUE)
    {
      Srp_InitSmRegistrar(PortIdx, &Srp_PortState[PortIdx].MsrpParticipant.RegistrarSM[FreeAttributeIdx]);
    }

    Srp_PortState[PortIdx].MsrpParticipant.ApplicantSM[FreeAttributeIdx].RegistrarSmPtr = 
      &Srp_PortState[PortIdx].MsrpParticipant.RegistrarSM[FreeAttributeIdx];

    /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
    Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant, PortIdx,
      FreeAttributeIdx, SRP_MRP_EVENT_NEW);

  }
  return RetVal;
} /* Srp_RegisterAttach() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
/* PRQA L: STCYC */
/* PRQA L: STMIF */
/* PRQA L: STPTH */

/**********************************************************************************************************************
 *  Srp_DeregisterAttach
 *********************************************************************************************************************/
/**
 *  \brief          Allows the Application to withdraw the registration of a Stream
 *  \param[in]      PortIdx              PortIdx
 *  \param[in]      StreamID             The unique identifier of the Stream
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_DeregisterAttach(Srp_PortIdxType PortIdx, Srp_StreamIdType StreamID)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType AttributeIdx = SRP_SUPPORTED_MSRP_ATTRIBUTES;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  /* Check initialization state of the component */
  Srp_CheckDetErrorReturnVoid(SRP_COND_INIT, SRP_SID_DEREGISTER_ATTACH, SRP_E_UNINIT);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  /* iterate all Attributes */
  while(AttributeIdx > 0u)
  {
    AttributeIdx--;
    if((Srp_PortState[PortIdx].MsrpParticipant.AttributeIsValid[AttributeIdx] == TRUE) &&
      (Srp_PortState[PortIdx].MsrpParticipant.ApplicantSM[AttributeIdx].SmIsValid == TRUE))
    {
      /* PRQA S 3415 3 */ /* MD_Srp_12.4 */
      if((Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].AttributeType == SRP_MSRP_LISTENER) &&
        (Srp_EqualsStreamId(Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].StreamInfo.StreamId,
        StreamID) == TRUE))
      {
        /* found corresponding attribute. Trigger Lv! event. */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MsrpParticipant,
          PortIdx, AttributeIdx, SRP_MRP_EVENT_LV);

        /* Release Bandwidth */
        (void)Srp_ReleaseBandwidthRx(PortIdx,
          Srp_PortState[PortIdx].MsrpParticipant.Attribute[AttributeIdx].ReservedRxBandwidth);

        break;
      }
    }
  }
} /* Srp_DeregisterAttach() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_RegisterVlanMember
 *********************************************************************************************************************/
/**
 *  \brief          Allows the Application to register membership of the specified VLan
 *  \param[in]      PortIdx         Port Index
 *  \param[in]      VID             The unique identifier of the VLan
 *  \return         E_OK            Successfully declared membership of the VLan
 *                  E_NOT_OK        Failed to declare membership of the VLan
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SRP_CODE) Srp_RegisterVlanMember(Srp_PortIdxType PortIdx, Srp_VLanIdType VID)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType AttributeIdx = SRP_SUPPORTED_MVRP_ATTRIBUTES;
  Srp_AttributeIdxType FreeAttributeIdx = SRP_INV_ATTRIBUTE_IDX;
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 VidCnt = Srp_PortConfig[PortIdx].VidCnt; /* PRQA S 0781 */ /* MD_Srp_5.6 */
  boolean InitRegistrarSm = TRUE;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  /* Check initialization state of the component */
  Srp_CheckDetErrorReturnValue(SRP_COND_INIT, SRP_SID_REGISTER_VLAN_MEMBER, SRP_E_UNINIT, RetVal);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  /* iterate all attributes */
  while(AttributeIdx > 0u)
  {
    AttributeIdx--;
    if(Srp_PortState[PortIdx].MvrpParticipant.AttributeIsValid[AttributeIdx] == TRUE)
    {
      if(Srp_PortState[PortIdx].MvrpParticipant.VlanId[AttributeIdx] == VID)
      {
        if(Srp_PortState[PortIdx].MvrpParticipant.ApplicantSM[AttributeIdx].SmIsValid == TRUE)
        {
          /* VID is already registered */
          RetVal = E_NOT_OK;
        }
        else
        {
          /* Attribute for VID is existing, but not registered */
          InitRegistrarSm = FALSE;
          RetVal = E_OK;
          FreeAttributeIdx = AttributeIdx;
        }
        break;
      }
    }
    else
    {
      /* found free Attribute. Store Index */
      FreeAttributeIdx = AttributeIdx;
      RetVal = E_OK;
    }
  }

  if((RetVal == E_OK) && (FreeAttributeIdx != SRP_INV_ATTRIBUTE_IDX))
  {
    /* VID is not registered and free Attribute was found. Register VID */
    Srp_PortState[PortIdx].MvrpParticipant.AttributeIsValid[FreeAttributeIdx] = TRUE;
    Srp_PortState[PortIdx].MvrpParticipant.VlanId[FreeAttributeIdx] = VID;

    Srp_InitSmApplicant(&Srp_PortState[PortIdx].MvrpParticipant.ApplicantSM[FreeAttributeIdx]);
    if(InitRegistrarSm == TRUE)
    {
      Srp_InitSmRegistrar(PortIdx, &Srp_PortState[PortIdx].MvrpParticipant.RegistrarSM[FreeAttributeIdx]);
    }

    Srp_PortState[PortIdx].MvrpParticipant.ApplicantSM[FreeAttributeIdx].RegistrarSmPtr = 
      &Srp_PortState[PortIdx].MvrpParticipant.RegistrarSM[FreeAttributeIdx];

    /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
    Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MvrpParticipant, PortIdx,
      FreeAttributeIdx, SRP_MRP_EVENT_NEW);

    /* Check if VID is Valid for the given Port */
    RetVal = E_NOT_OK;
    while(VidCnt > 0u)
    {
      VidCnt--;
      if(Srp_PortConfig[PortIdx].VidsPtr[VidCnt] == VID)
      {
        /* VID is Valid. Return E_OK and stop iteration of VIDs */
        RetVal = E_OK;
        break;
      }
    }
  }

  return RetVal;
} /* Srp_RegisterVlanMember() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_DeregisterVlanMember
 *********************************************************************************************************************/
/**
 *  \brief          Allows the Application to withdraw the membership of the specified VLan
 *  \param[in]      PortIdx         Port Index
 *  \param[in]      VID             The unique identifier of the VLan
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_DeregisterVlanMember(Srp_PortIdxType PortIdx, Srp_VLanIdType VID)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType AttributeIdx = SRP_SUPPORTED_MVRP_ATTRIBUTES;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  /* Check initialization state of the component */
  Srp_CheckDetErrorReturnVoid(SRP_COND_INIT, SRP_SID_DEREGISTER_VLAN_MEMBER, SRP_E_UNINIT);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  /* iterate all attributes */
  while(AttributeIdx > 0u)
  {
    AttributeIdx--;
    if(Srp_PortState[PortIdx].MvrpParticipant.AttributeIsValid[AttributeIdx] == TRUE)
    {
      if(Srp_PortState[PortIdx].MvrpParticipant.VlanId[AttributeIdx] == VID)
      {
        /* VID found. Trigger Lv! event */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MvrpParticipant, 
            PortIdx, AttributeIdx, SRP_MRP_EVENT_LV);
        break;
      }
    }
  }
} /* Srp_DeregisterVlanMember() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if (SRP_MMRP_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  Srp_RegisterMacAddress
 *********************************************************************************************************************/
/**
 *  \brief          Allows the Application to register membership of the specified Multi-cast MAC Address
 *  \param[in]      MacAddressPtr          Pointer to the Multi-cast MacAddress
 *  \param[in]      PortIdx                Port Index
 *  \return         E_OK                   Successfully declared membership of the Multi-cast MAC Address
 *                  E_NOT_OK               Failed to declare membership of the Multi-cast MAC Address
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SRP_CODE) Srp_RegisterMacAddress(Srp_PortIdxType PortIdx, SRP_P2CONST(uint8) MacAddressPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_NOT_OK;
  Srp_AttributeIdxType AttributeIdx;
  Srp_AttributeIdxType FreeAttributeIdx = SRP_INV_ATTRIBUTE_IDX;
  boolean InitRegistrarSm = TRUE;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  /* Check initialization state of the component */
  Srp_CheckDetErrorReturnValue(SRP_COND_INIT, SRP_SID_REGISTER_MAC_ADDRESS, SRP_E_UNINIT, RetVal);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  /* get first free AttributeIdx and check if the MacAddress is already registered */
  AttributeIdx = SRP_SUPPORTED_MMRP_ATTRIBUTES;
  while(AttributeIdx > 0u)
  {
    AttributeIdx--;
    if(Srp_PortState[PortIdx].MmrpParticipant.AttributeIsValid[AttributeIdx] == TRUE)
    {
      /* Valid MMRP Attribute */
      if(Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].AttributeType == 
        SRP_MMRP_MAC_TYPE)
      {
        if(Srp_EqualsMacAddress(Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].MacAddress, 
          MacAddressPtr)
          == TRUE)
        {
          if(Srp_PortState[PortIdx].MmrpParticipant.ApplicantSM[AttributeIdx].SmIsValid == TRUE)
          {
            /* MAC Address is already registered */
            RetVal = E_NOT_OK;
          }
          else
          {
            /* Attribute for MAC-Address is existing, but not registered yet */
            InitRegistrarSm = FALSE;
            RetVal = E_OK;
            FreeAttributeIdx = AttributeIdx;
          }
          break;
        }
      }
    }
    else
    {
      /* found free Attribute. Store the Index */
      FreeAttributeIdx = AttributeIdx;
      RetVal = E_OK;
    }
  }

  /* If MAC-Address is not registered yet and a free Attribute was found, register the MacAddress */
  if((RetVal == E_OK) && (FreeAttributeIdx != SRP_INV_ATTRIBUTE_IDX))
  {
    Srp_PortState[PortIdx].MmrpParticipant.AttributeIsValid[FreeAttributeIdx] = TRUE;
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].AttributeType = SRP_MMRP_MAC_TYPE;
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].MacAddress[0] = MacAddressPtr[0];
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].MacAddress[1] = MacAddressPtr[1];
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].MacAddress[2] = MacAddressPtr[2];
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].MacAddress[3] = MacAddressPtr[3];
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].MacAddress[4] = MacAddressPtr[4];
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].MacAddress[5] = MacAddressPtr[5];

    Srp_InitSmApplicant(&Srp_PortState[PortIdx].MmrpParticipant.ApplicantSM[FreeAttributeIdx]);
    if(InitRegistrarSm == TRUE)
    {
      Srp_InitSmRegistrar(PortIdx, &Srp_PortState[PortIdx].MmrpParticipant.RegistrarSM[FreeAttributeIdx]);
    }

    Srp_PortState[PortIdx].MmrpParticipant.ApplicantSM[FreeAttributeIdx].RegistrarSmPtr = 
      &Srp_PortState[PortIdx].MmrpParticipant.RegistrarSM[FreeAttributeIdx];

    /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
    (void)EthIf_UpdatePhysAddrFilter(Srp_PortConfig[PortIdx].CtrlIdx, MacAddressPtr, ETH_ADD_TO_FILTER);

    /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
    Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant, PortIdx,
      FreeAttributeIdx, SRP_MRP_EVENT_NEW);
  }

  return RetVal;
} /* Srp_RegisterMacAddress() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Srp_DeregisterMacAddress
 *********************************************************************************************************************/
/**
 *  \brief          Allows the Application to withdraw membership of the specified Multi-cast MAC Address
 *  \param[in]      MacAddressPtr          Pointer to the Multi-cast MacAddress
 *  \param[in]      PortIdx                Port Index
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_DeregisterMacAddress(Srp_PortIdxType PortIdx, SRP_P2CONST(uint8) MacAddressPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType AttributeIdx = SRP_SUPPORTED_MMRP_ATTRIBUTES;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  /* Check initialization state of the component */
  Srp_CheckDetErrorReturnVoid(SRP_COND_INIT, SRP_SID_DEREGISTER_MAC_ADDRESS, SRP_E_UNINIT);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  /* iterate all Attributes of MMRP Participant */
  while(AttributeIdx > 0u)
  {
    AttributeIdx--;
    if(Srp_PortState[PortIdx].MmrpParticipant.AttributeIsValid[AttributeIdx] == TRUE)
    {
      if(Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].AttributeType == SRP_MMRP_MAC_TYPE)
      {
        if(Srp_EqualsMacAddress(MacAddressPtr, 
          Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].MacAddress) == TRUE)
        {
          /* MAC-Address found. Trigger Lv! event */
          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant, 
            PortIdx, AttributeIdx, SRP_MRP_EVENT_LV);

          /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
          (void)EthIf_UpdatePhysAddrFilter(Srp_PortConfig[PortIdx].CtrlIdx,
            MacAddressPtr, ETH_REMOVE_FROM_FILTER);
          break;
        }
      }
    }
  }
} /* Srp_DeregisterMacAddress() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_RegisterServiceRequirement
 *********************************************************************************************************************/
/**
 *  \brief          Allows the Application to register the specified Service Requirement
 *  \param[in]      ServiceRequirement             The Service Requirement
 *  \return         E_OK                           Successfully registered the Service Requirement
 *                  E_NOT_OK                       Failed to register the Service Requirement
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SRP_CODE) Srp_RegisterServiceRequirement(Srp_PortIdxType PortIdx, 
  Srp_MmrpServiceRequirementAttributeType ServiceRequirement)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType AttributeIdx = SRP_SUPPORTED_MMRP_ATTRIBUTES;
  Srp_AttributeIdxType FreeAttributeIdx = SRP_INV_ATTRIBUTE_IDX;
  Std_ReturnType RetVal = E_NOT_OK;
  boolean InitRegistrarSm = TRUE;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  /* Check initialization state of the component */
  Srp_CheckDetErrorReturnValue(SRP_COND_INIT, SRP_SID_REGISTER_SERVICE_REQUIREMENT, SRP_E_UNINIT, RetVal); 
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  /* get first free Attribute Index and check if the ServiceRequirement is already registered */
  while(AttributeIdx > 0u)
  {
    AttributeIdx--;
    if(Srp_PortState[PortIdx].MmrpParticipant.AttributeIsValid[AttributeIdx] == TRUE)
    {
      /* valid MMRP Attribute. Check Type */
      if(Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].AttributeType == 
        SRP_MMRP_SERVICE_REQUIREMENT_TYPE)
      {
        if(Srp_PortState[PortIdx].MmrpParticipant.ApplicantSM[AttributeIdx].SmIsValid == TRUE)
        {
          /* Service Requirement attribute already registered */
          RetVal = E_NOT_OK;
          break;
        }
        else
        {
          /* Attribute for Service Requirement is already existing, but not registered yet */
          InitRegistrarSm = FALSE;
          RetVal = E_OK;
          FreeAttributeIdx = AttributeIdx;
          break; /* PRQA S 0771 */ /* MD_Srp_14.6 */
        }
      }
    }
    else
    {
      /* found free attribute. Store index */
      FreeAttributeIdx = AttributeIdx;
      RetVal = E_OK;
    }
  }

  /* If Service Requirement is not registered and a free attribute was found, register the Service Requirement */
  if((RetVal == E_OK) && (FreeAttributeIdx != SRP_INV_ATTRIBUTE_IDX))
  {
    Srp_PortState[PortIdx].MmrpParticipant.AttributeIsValid[FreeAttributeIdx] = TRUE;
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].AttributeType = 
      SRP_MMRP_SERVICE_REQUIREMENT_TYPE;
    Srp_PortState[PortIdx].MmrpParticipant.Attribute[FreeAttributeIdx].ServiceReq = ServiceRequirement;

    Srp_InitSmApplicant(&Srp_PortState[PortIdx].MmrpParticipant.ApplicantSM[FreeAttributeIdx]);
    if(InitRegistrarSm == TRUE)
    {
      Srp_InitSmRegistrar(PortIdx, &Srp_PortState[PortIdx].MmrpParticipant.RegistrarSM[FreeAttributeIdx]);
    }

    Srp_PortState[PortIdx].MmrpParticipant.ApplicantSM[FreeAttributeIdx].RegistrarSmPtr = 
      &Srp_PortState[PortIdx].MmrpParticipant.RegistrarSM[FreeAttributeIdx];

    /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
    Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant, PortIdx,
      FreeAttributeIdx, SRP_MRP_EVENT_NEW);
  }

  return RetVal;

} /* Srp_RegisterServiceRequirement() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_DeregisterServiceRequirement
 *********************************************************************************************************************/
/**
 *  \brief          Allows the Application to withdraw registration of the specified Service Requirement
 *  \param[in]      PortIdx                        Port Index
 *  \param[in]      ServiceRequirement             The Service Requirement
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_DeregisterServiceRequirement(Srp_PortIdxType PortIdx, 
  Srp_MmrpServiceRequirementAttributeType ServiceRequirement)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_AttributeIdxType AttributeIdx = SRP_SUPPORTED_MMRP_ATTRIBUTES;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  /* Check initialization state of the component */
  Srp_CheckDetErrorReturnVoid(SRP_COND_INIT, SRP_SID_DEREGISTER_SERVICE_REQUIREMENT, SRP_E_UNINIT); 
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  /* iterate all attributes */
  while(AttributeIdx > 0u)
  {
    AttributeIdx--;
    if(Srp_PortState[PortIdx].MmrpParticipant.AttributeIsValid[AttributeIdx] == TRUE)
    {
      if((Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].AttributeType == 
        SRP_MMRP_SERVICE_REQUIREMENT_TYPE) && 
        (Srp_PortState[PortIdx].MmrpParticipant.Attribute[AttributeIdx].ServiceReq == ServiceRequirement))
      {
        /* corresponding Attribute found. Trigger Lv! event */
        /* PRQA S 0310 2 */ /* MD_Srp_11.4 */
        Srp_ProcessSmApplicant((SRP_P2VAR(Srp_MrpParticipantType))&Srp_PortState[PortIdx].MmrpParticipant, 
            PortIdx, AttributeIdx, SRP_MRP_EVENT_LV);
        break;
      }
    }
  }

} /* Srp_DeregisterServiceRequirement() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (SRP_MMRP_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  Srp_SetPeriodic
 *********************************************************************************************************************/
/**
 *  \brief          Allows the Application to enable or disable the PeriodicTransmission state machine
 *  \param[in]      PortIdx             Port Index
 *  \param[in]      MrpApplication      The MrpApplicationType:
 *                    SRP_MMRP_APPLICATION: MMRP Application
 *                    SRP_MVRP_APPLICATION: MVRP Application
 *                    SRP_MSRP_APPLICATION: MSRP Application
 *  \param[in]      PeriodicEnabled     Enable/Disable PeriodicTransmission state machine
 *                    TRUE:  Enable PeriodicTransmission state machine
 *                    FALSE: Disable PeriodicTransmission state machine
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_SetPeriodic(Srp_PortIdxType PortIdx, Srp_MrpApplicationType MrpApplication,
  boolean PeriodicEnabled)
{
  /* ----- Local Variables ---------------------------------------------- */
  SRP_P2VAR(Srp_PeridodicTransmissionStateMachineType) periodicTransmissionSmPtr = NULL_PTR;

  /* ----- Implementation ----------------------------------------------- */
  switch(MrpApplication)
  {
# if (SRP_MMRP_SUPPORT == STD_ON)
  case SRP_MMRP_APPLICATION:
    periodicTransmissionSmPtr = &Srp_PortState[PortIdx].MmrpParticipant.PeriodicTransmissionSM;
    break;
# endif /* (SRP_MMRP_SUPPORT == STD_ON) */

  case SRP_MVRP_APPLICATION:
    periodicTransmissionSmPtr = &Srp_PortState[PortIdx].MvrpParticipant.PeriodicTransmissionSM;
    break;

  case SRP_MSRP_APPLICATION:
    periodicTransmissionSmPtr = &Srp_PortState[PortIdx].MsrpParticipant.PeriodicTransmissionSM;
    break;

  default: break;
  }

  if(periodicTransmissionSmPtr != NULL_PTR)
  {
    if(PeriodicEnabled == TRUE)
    {
      periodicTransmissionSmPtr->PeriodicTimerRunning = TRUE;
      periodicTransmissionSmPtr->PeriodicTimer = SRP_PERIODIC_TIMER;
      periodicTransmissionSmPtr->State = SRP_STATE_PERIODIC_TRANSMISSION_ACTIVE;
    }
    else
    {
      periodicTransmissionSmPtr->PeriodicTimerRunning = FALSE;
      periodicTransmissionSmPtr->State = SRP_STATE_PERIODIC_TRANSMISSION_PASSIVE;
    }
  }

} /* Srp_SetPeriodic() */

/*******************************************************CBK FUNCTIONS*************************************************/

/**********************************************************************************************************************
 *  Srp_Cbk_TrcvLinkStateChg
 **********************************************************************************************************************/
/**
  \brief        Callback function that notifies a changed state of the transceiver link
  \param[in]    CtrlIdx         Index of the controller that changed its state
  \param[in]    TrcvLinkState   New link state of the transceiver
  \return       none
  \context      This function can be called in task context.
 **********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_Cbk_TrcvLinkStateChg(uint8 CtrlIdx, Srp_LinkStateType TrcvLinkState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_PortIdxType PortIdx = SRP_INV_PORT_IDX; /* PRQA S 0781 */ /* MD_Srp_5.6 */
  Srp_PortIdxType PortCnt = SRP_PORT_COUNT;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  /* Check initialization state of the component */
  Srp_CheckDetErrorReturnVoid(SRP_COND_INIT, SRP_SID_CBK_TRCV_LINK_STATE_CHG, SRP_E_UNINIT); 
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  /* search the port index */
  while(PortCnt > 0u)
  {
    PortCnt--;
    if(Srp_PortConfig[PortCnt].CtrlIdx == CtrlIdx)
    {
      PortIdx = PortCnt;
      break;
    }
  }

  if(PortIdx == SRP_INV_PORT_IDX)
  {
    /* no corresponding port index found */
    return;
  }

  if(TrcvLinkState == SRP_LINKE_STATE_ACTIVE)
  {
    /* link up */
    Srp_PortState[PortIdx].Enabled = TRUE;

    /* Add multi-cast frame reception */
#   if (SRP_MMRP_SUPPORT == STD_ON)
    (void)EthIf_UpdatePhysAddrFilter(Srp_PortConfig[PortIdx].CtrlIdx, Srp_MmrpDestinationMac, ETH_ADD_TO_FILTER);
#   endif
    (void)EthIf_UpdatePhysAddrFilter(Srp_PortConfig[PortIdx].CtrlIdx, Srp_MvrpDestinationMac, ETH_ADD_TO_FILTER);
    (void)EthIf_UpdatePhysAddrFilter(Srp_PortConfig[PortIdx].CtrlIdx, Srp_MsrpDestinationMac, ETH_ADD_TO_FILTER);
  }
  else
  {
    /* link down */
    Srp_InitPort(PortIdx);
    /* remove multi-cast frame reception */
#   if (SRP_MMRP_SUPPORT == STD_ON)
    (void)EthIf_UpdatePhysAddrFilter(Srp_PortConfig[PortIdx].CtrlIdx, Srp_MmrpDestinationMac, ETH_REMOVE_FROM_FILTER);
#   endif
    (void)EthIf_UpdatePhysAddrFilter(Srp_PortConfig[PortIdx].CtrlIdx, Srp_MvrpDestinationMac, ETH_REMOVE_FROM_FILTER);
    (void)EthIf_UpdatePhysAddrFilter(Srp_PortConfig[PortIdx].CtrlIdx, Srp_MsrpDestinationMac, ETH_REMOVE_FROM_FILTER);
  }

} /* Srp_Cbk_TrcvLinkStateChg() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/**********************************************************************************************************************
 *  Srp_Mmrp_RxIndication
 *********************************************************************************************************************/
/**
 *  \brief          Handles processing of received MMRP frames.
 *  \param[in]      VCtrlIdx     Index of the virtual controller that has received the frame.
 *  \param[in]      FrameType    Ethertype of the frame
 *  \param[in]      IsBroadcast  Determines that the frame was transmitted as broadcast
 *  \param[in]      PhysAddrPtr  Pointer to the physical address of the transmitted interface
 *  \param[in]      DataPtr      Pointer to the received data.
 *  \param[in]      LenByte      Byte count of the received frame.
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
/* PRQA S 6010 STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6060 STPAR */ /* MD_MSR_STPAR */
/* PRQA S 6080 STMIF */ /* MD_MSR_STMIF */
#if (SRP_MMRP_SUPPORT == STD_ON)
FUNC(void, SRP_CODE) Srp_Mmrp_RxIndication(uint8 VCtrlIdx, Eth_FrameType FrameType, boolean IsBroadcast,
  SRP_P2VAR(uint8) PhysAddrPtr, SRP_P2VAR(uint8) DataPtr, uint16 LenByte) /* PRQA S 3673 */ /* MD_Srp_16.7 */
{
# if (SRP_MMRP_SUPPORT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Srp_PortIdxType PortIdx = SRP_INV_PORT_IDX; /* PRQA S 0781 */ /* MD_Srp_5.6 */
  Srp_PortIdxType PortCnt = SRP_PORT_COUNT;
  Srp_MmrpAttributeType AttributeType; /* PRQA S 0781 */ /* MD_Srp_5.6 */
  uint16 Offset = 0;
  boolean LeaveAllEvent = FALSE;
  uint16 NumberOfValues = 0;
  Srp_MacAddressType MacFirstValue;
  uint8 ServiceReqFirstValue;
  uint8 VectorLength;
  uint16 AttributeCnt;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  /* Check initialization state of the component */
  Srp_CheckDetErrorReturnVoid(SRP_COND_INIT, SRP_SID_MMRP_RX_INDICATION, SRP_E_UNINIT); 
  Srp_CheckDetErrorReturnVoid(SRP_COND_VALID_PTR(DataPtr), SRP_SID_MMRP_RX_INDICATION, SRP_E_NULL_POINTER);
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  /* PRQA S 3112 3 */ /* MD_MSR_14.2 */
  SRP_DUMMY_STATEMENT(PhysAddrPtr);
  SRP_DUMMY_STATEMENT(IsBroadcast);
  SRP_DUMMY_STATEMENT(FrameType);

  /* search for the port index */
  while(PortCnt > 0u)
  {
    PortCnt--;
    if(Srp_PortConfig[PortCnt].CtrlIdx == VCtrlIdx)
    {
      PortIdx = PortCnt;
      break;
    }
  }

  if(PortIdx == SRP_INV_PORT_IDX)
  {
    /* no corresponding Port Index found --> Drop message */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_MMRP_RX_INDICATION, SRP_E_INV_PARAM);
    return;
  }

  if(LenByte < (SRP_COMMON_HEADER_LENGTH + SRP_COMMON_MSG_LENGTH))
  {
    /* invalid message length, message is smaller than common header length + common message length --> drop message */
    Srp_CallDetReportError(SRP_SID_MMRP_RX_INDICATION, SRP_E_INV_MSG_LENGTH);
    return;
  }

  /* check Protocol Version */
  if(DataPtr[Offset] != SRP_PROTOCOL_VERSION)
  {
    /* invalid Protocol version --> drop message */
    return;
  }
  Offset += 1;

  /* get and process all messages of the received frame */
  while(Offset < LenByte)
  {
    /* get Attribute Type */
    AttributeType = DataPtr[Offset];
    switch(AttributeType)
    {
    case SRP_MMRP_MAC_TYPE:
      if((Offset + SRP_MMRP_MAC_MSG_LENGTH) > LenByte)
      {
        /* invalid message length --> drop message */
        return;
      }
      Offset += 1;

      /* Get and Check Attribute Length */
      if(DataPtr[Offset] != SRP_MMRP_MAC_LENGTH)
      {
        /* invalid Attribute Length --> drop message */
        return;
      }
      Offset += 1;

      while(Offset < LenByte)
      {
        Srp_GetLeavAllAndNumberOfValues(DataPtr, &Offset, &LeaveAllEvent, &NumberOfValues);
        /* Number of values sanity check */
        if(NumberOfValues < 1u)
        {
          /* invalid Number of values --> drop message */
          return;
        }
        Srp_GetMacAddress(DataPtr, &Offset, &MacFirstValue); /*lint !e545 */

        /* calculate Vector Length and Check remaining length */
        VectorLength = (uint8)(NumberOfValues / 3u);
        if((NumberOfValues % 3) != 0)
        {
          VectorLength += 1;
        }
        if((Offset + VectorLength) >= LenByte)
        {
          /* invalid frame length --> drop message */
          return;
        }
       
        /* Get and Process Attributes */
        AttributeCnt = NumberOfValues;
        do
        {
          uint8 ThreePackedEvent = DataPtr[Offset];
          Offset += 1;

          if(AttributeCnt >= 3u)
          {
            /* Get 3 Events from ThreePackedEvents */
            uint8 FirstEvent = 0;
            uint8 SecondEvent = 0;
            uint8 ThirdEvent = 0;
       
            AttributeCnt -= 3;
            Srp_UnpackThreePackedEvent(ThreePackedEvent, &FirstEvent, &SecondEvent, &ThirdEvent);

            Srp_ProcessAttributeMac(PortIdx, MacFirstValue, FirstEvent, LeaveAllEvent);
            Srp_IncrementMacAddress(MacFirstValue);
            Srp_ProcessAttributeMac(PortIdx, MacFirstValue, SecondEvent, LeaveAllEvent);
            Srp_IncrementMacAddress(MacFirstValue);
            Srp_ProcessAttributeMac(PortIdx, MacFirstValue, ThirdEvent, LeaveAllEvent);
          }
          else if(AttributeCnt == 2)
          {
            /* Get 2 Events from ThreePackedEvents */
            uint8 FirstEvent = 0;
            uint8 SecondEvent = 0;
       
            AttributeCnt -= 2;
            Srp_UnpackThreePackedEvent(ThreePackedEvent, &FirstEvent, &SecondEvent, NULL_PTR);
            Srp_ProcessAttributeMac(PortIdx, MacFirstValue, FirstEvent, LeaveAllEvent);
            Srp_IncrementMacAddress(MacFirstValue);
            Srp_ProcessAttributeMac(PortIdx, MacFirstValue, SecondEvent, LeaveAllEvent);
          }
          else
          {
            /* Get 1 Event from ThreePackedEvents */
            uint8 FirstEvent = 0;
       
            AttributeCnt -= 1;
            Srp_UnpackThreePackedEvent(ThreePackedEvent, &FirstEvent, NULL_PTR, NULL_PTR);
            Srp_ProcessAttributeMac(PortIdx, MacFirstValue, FirstEvent, LeaveAllEvent);
            
          }

          Srp_IncrementMacAddress(MacFirstValue);

        }while(AttributeCnt > 0u);
       
        /* Check if End of Frame is reached */
        if((Offset + SRP_END_MARK_LENGTH) > LenByte)
        {
          /* Reached End of Frame --> Stop processing of the message */
          return;
        }
        /* check if EndMark is reached */
        if(Srp_Ntohs(*((uint16*)&DataPtr[Offset])) == SRP_END_MARK) /* PRQA S 0310 */ /* MD_Srp_11.4 */
        {
          /* reached EndMark --> Continue with next message */
          Offset += SRP_END_MARK_LENGTH;
          break;
        }
      }
      break;
    case SRP_MMRP_SERVICE_REQUIREMENT_TYPE:
      if((Offset + SRP_MMRP_SERVICE_REQ_MSG_LENGTH) > LenByte)
      {
        /* invalid message length --> drop message */
        return;
      }
      Offset += 1;

      /* Get and Check Attribute Length */
      if(DataPtr[Offset] != SRP_MMRP_SERVICE_REQUIREMENT_LENGTH)
      {
        /* invalid Attribute Length --> drop message */
        return;
      }
      Offset += 1;

      /* Iterate all Vector Attributes */
      while(Offset < LenByte)
      {
        uint8 ThreePackedEvent;
        uint8 Event;

        Srp_GetLeavAllAndNumberOfValues(DataPtr, &Offset, &LeaveAllEvent, &NumberOfValues);
        /* Number of values sanity check */
        if(NumberOfValues < 1u)
        {
          /* invalid Number of values --> drop message */
          return;
        }
        ServiceReqFirstValue = DataPtr[Offset];
        Offset += 1;

        if((ServiceReqFirstValue > 1u) || (NumberOfValues != 1u))
        {
          /* invalid First Value or NumberOfValues--> stop processing of message */
          break;
        }

        /* calculate Vector Length and Check remaining length */
        VectorLength = 1;

        if((Offset + VectorLength) >= LenByte)
        {
          /* invalid frame length --> drop message */
          return;
        }
        /* Get and Process Attributes */
        /* Get AttributeEvent */
        ThreePackedEvent = DataPtr[Offset];
        Offset +=1;

        Srp_UnpackThreePackedEvent(ThreePackedEvent, &Event, NULL_PTR, NULL_PTR);
        Srp_ProcessAttributeServiceReq(PortIdx, ServiceReqFirstValue, Event, LeaveAllEvent);
       
        /* Check if End of Frame is reached */
        if((Offset + SRP_END_MARK_LENGTH) > LenByte)
        {
          /* Reached End of Frame --> Stop processing of the message */
          return;
        }
        /* check if EndMark is reached */
        if(Srp_Ntohs(*((uint16*)&DataPtr[Offset])) == SRP_END_MARK) /* PRQA S 0310 */ /* MD_Srp_11.4 */
        {
          /* reached EndMark --> Continue with next message */
          Offset += SRP_END_MARK_LENGTH;
          break; /* PRQA S 0771 */ /* MD_Srp_14.6 */
        }
      }

      break;
    default:
      /* Invalid Attribute Type --> drop message */
      return;
    }
    /* Check if End of Frame is reached */
    if((Offset + SRP_END_MARK_LENGTH) > LenByte)
    {
      /* Reached End of Frame --> Stop processing of the message */
      return;
    }
    /* check if EndMark is reached */
    if(Srp_Ntohs(*((uint16*)&DataPtr[Offset])) == SRP_END_MARK) /* PRQA S 0310 */ /* MD_Srp_11.4 */
    {
      /* reached EndMark --> stop processing of message */
      return;
    }
  }

# else
  /* MMRP is not supported. Discard message */
/* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  Srp_CallDetReportError(SRP_SID_MMRP_RX_INDICATION, SRP_E_MMRP_NOT_SUPPORTED);
# endif /* (SRP_MMRP_SUPPORT == STD_ON) */
} /* Srp_Mmrp_RxIndication() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* (SRP_MMRP_SUPPORT == STD_ON) */
/* PRQA L: STPTH */
/* PRQA L: STCYC */
/* PRQA L: STPAR */
/* PRQA L: STMIF */

/**********************************************************************************************************************
 *  Srp_Mvrp_RxIndication
 *********************************************************************************************************************/
/**
 *  \brief          Handles processing of received MVRP frames.
 *  \param[in]      VCtrlIdx     Index of the virtual controller that has received the frame.
 *  \param[in]      FrameType    Ethertype of the frame
 *  \param[in]      IsBroadcast  Determines that the frame was transmitted as broadcast
 *  \param[in]      PhysAddrPtr  Pointer to the physical address of the transmitted interface
 *  \param[in]      DataPtr      Pointer to the received data.
 *  \param[in]      LenByte      Byte count of the received frame.
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
/* PRQA S 6010 STPTH */ /* MD_MSR_STPTH */
/* PRQA S 6030 STCYC */ /* MD_MSR_STCYC */
/* PRQA S 6060 STPAR */ /* MD_MSR_STPAR */
/* PRQA S 6080 STMIF */ /* MD_MSR_STMIF */
FUNC(void, SRP_CODE) Srp_Mvrp_RxIndication(uint8 VCtrlIdx, Eth_FrameType FrameType, boolean IsBroadcast,
  SRP_P2VAR(uint8) PhysAddrPtr, SRP_P2VAR(uint8) DataPtr, uint16 LenByte) /* PRQA S 3673 */ /* MD_Srp_16.7 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_PortIdxType PortIdx = SRP_INV_PORT_IDX; /* PRQA S 0781 */ /* MD_Srp_5.6 */
  Srp_PortIdxType PortCnt = SRP_PORT_COUNT;
  uint16 Offset = 0;
  boolean LeaveAllEvent = FALSE;
  uint16 NumberOfValues = 0;
  Srp_VLanIdType FirstValue; /* PRQA S 0781 */ /* MD_Srp_5.6 */
  uint8 VectorLength;
  uint16 AttributeCnt;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  /* Check initialization state of the component */
  Srp_CheckDetErrorReturnVoid(SRP_COND_INIT, SRP_SID_MVRP_RX_INDICATION, SRP_E_UNINIT); 
  /* PRQA L:DET */

  /* ----- Implementation ----------------------------------------------- */
  /* PRQA S 3112 3 */ /* MD_MSR_14.2 */
  SRP_DUMMY_STATEMENT(PhysAddrPtr);
  SRP_DUMMY_STATEMENT(IsBroadcast);
  SRP_DUMMY_STATEMENT(FrameType);

  /* search for the port index */
  while(PortCnt > 0u)
  {
    PortCnt--;
    if(Srp_PortConfig[PortCnt].CtrlIdx == VCtrlIdx)
    {
      PortIdx = PortCnt;
      break;
    }
  }

  if(PortIdx == SRP_INV_PORT_IDX)
  {
    /* no corresponding Port Index found --> Drop message */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_MVRP_RX_INDICATION, SRP_E_INV_PARAM);
    return;
  }

  if(LenByte < (SRP_COMMON_HEADER_LENGTH + SRP_COMMON_MSG_LENGTH))
  {
    /* invalid message length, message is smaller than common header length + common message length --> drop message */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_MVRP_RX_INDICATION, SRP_E_INV_MSG_LENGTH);
    return;
  }

  /* check Protocol Version */
  if(DataPtr[Offset] != SRP_PROTOCOL_VERSION)
  {
    /* invalid Protocol version --> drop message */
    return;
  }
  Offset += 1;

  /* get and process all messages of the received frame */
  while(Offset < LenByte)
  {
    /* check the attribute type */
    if(DataPtr[Offset] != SRP_MVRP_ATTRIBUTE_TYPE)
    {
      /* invalid attribute type --> drop message */
      return;
    }
    Offset += 1;

    /* get and check the Attribute Length */
    if(DataPtr[Offset] != SRP_MVRP_ATTRIBUTE_LENGTH)
    {
      /* invalid attribute length --> drop message */
      return;
    }
    Offset += 1;

    /* get all Vector Attributes of the message */
    while(Offset < LenByte)
    {
      Srp_GetLeavAllAndNumberOfValues(DataPtr, &Offset, &LeaveAllEvent, &NumberOfValues);
      /* Number of values sanity check */
        if(NumberOfValues < 1u)
        {
          /* invalid Number of values --> drop message */
          return;
        }
      
      /* Get FirstValue (VID) */
      FirstValue = (uint16)Srp_Ntohs(*((uint16*)&DataPtr[Offset])); /* PRQA S 0310 */ /* MD_Srp_11.4 */
      Offset += 2;

      /* Check if FirstValue is valid - according to IEEE802.1Q (11.2.3.1.7) */
      if(SRP_COND_VALID_MVRP_FIRST_VALUE(FirstValue))
      {
        /* invalid FirstValue --> drop message */
        return;
      }

      /* calculate Vector Length and Check remaining Length */
      VectorLength = (uint8)(NumberOfValues / 3u);
      if((NumberOfValues % 3) != 0u)
      {
        VectorLength += 1;
      }

      if((Offset + VectorLength) >= LenByte)
      {
        /* invalid length --> drop message */
        return;
      }

      /* Get and Process Attributes */
      AttributeCnt = NumberOfValues;
      do
      {
        uint8 ThreePackedEvent = DataPtr[Offset];
        Offset += 1;

        if(FirstValue > 4094u)
        {
          return;
        }

        if(AttributeCnt >= 3u)
        {
          /* Get 3 Events from ThreePackedEvents */
          uint8 FirstEvent = 0;
          uint8 SecondEvent = 0;
          uint8 ThirdEvent = 0;

          AttributeCnt -= 3;
          Srp_UnpackThreePackedEvent(ThreePackedEvent, &FirstEvent, &SecondEvent, &ThirdEvent);

          Srp_ProcessAttributeVid(PortIdx, FirstValue, FirstEvent, LeaveAllEvent);
          Srp_IncrementVid(FirstValue); /* PRQA S 3109 */ /* MD_MSRP_14.3 */
          Srp_ProcessAttributeVid(PortIdx, FirstValue, SecondEvent, LeaveAllEvent);
          Srp_IncrementVid(FirstValue); /* PRQA S 3109 */ /* MD_MSRP_14.3 */
          Srp_ProcessAttributeVid(PortIdx, FirstValue, ThirdEvent, LeaveAllEvent);
        }
        else if(AttributeCnt == 2)
        {
          /* Get 2 Events from ThreePackedEvents */
          uint8 FirstEvent = 0;
          uint8 SecondEvent = 0;

          AttributeCnt -= 2;
          Srp_UnpackThreePackedEvent(ThreePackedEvent, &FirstEvent, &SecondEvent, NULL_PTR);
          Srp_ProcessAttributeVid(PortIdx, FirstValue, FirstEvent, LeaveAllEvent);
          Srp_IncrementVid(FirstValue); /* PRQA S 3109 */ /* MD_MSRP_14.3 */
          Srp_ProcessAttributeVid(PortIdx, FirstValue, SecondEvent, LeaveAllEvent);
        }
        else
        {
          /* Get 1 Event from ThreePackedEvents */
          uint8 FirstEvent = 0;

          AttributeCnt -= 1;
          Srp_UnpackThreePackedEvent(ThreePackedEvent, &FirstEvent, NULL_PTR, NULL_PTR);
          Srp_ProcessAttributeVid(PortIdx, FirstValue, FirstEvent, LeaveAllEvent);
        }

        FirstValue++;

      }while(AttributeCnt > 0);

      /* Check if End of Frame is reached */
      if((Offset + SRP_END_MARK_LENGTH) > LenByte)
      {
        /* Reached End of Frame --> Stop processing of the message */
        return;
      }
      /* check if EndMark is reached */
      if(Srp_Ntohs(*((uint16*)&DataPtr[Offset])) == SRP_END_MARK) /* PRQA S 0310 */ /* MD_Srp_11.4 */
      {
        /* reached EndMark --> Continue with next message */
        Offset += SRP_END_MARK_LENGTH;
        break;
      }
    }

    /* Check if End of Frame is reached */
    if((Offset + SRP_END_MARK_LENGTH) > LenByte)
    {
      /* Reached End of Frame --> Stop processing of the message */
        return;
    }
    /* check if EndMark is reached */
    if(Srp_Ntohs(*((uint16*)&DataPtr[Offset])) == SRP_END_MARK) /* PRQA S 0310 */ /* MD_Srp_11.4 */
    {
      /* reached EndMark --> stop processing of message */
      return;
    }
  }

} /* Srp_Mvrp_RxIndication() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
/* PRQA L: STPTH */
/* PRQA L: STCYC */
/* PRQA L: STPAR */
/* PRQA L: STMIF */

/**********************************************************************************************************************
 *  Srp_Msrp_RxIndication
 *********************************************************************************************************************/
/**
 *  \brief          Handles processing of received MSRP frames.
 *  \param[in]      VCtrlIdx     Index of the virtual controller that has received the frame.
 *  \param[in]      FrameType    Ethertype of the frame
 *  \param[in]      IsBroadcast  Determines that the frame was transmitted as broadcast
 *  \param[in]      PhysAddrPtr  Pointer to the physical address of the transmitted interface
 *  \param[in]      DataPtr      Pointer to the received data.
 *  \param[in]      LenByte      Byte count of the received frame.
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
/* PRQA S 6010 PATH_COUNT */ /* MD_MSR_STPTH */
/* PRQA S 6030 CYCLO */ /* MD_MSR_STCYC */
/* PRQA S 6050 CALLS */ /* MD_MSR_STCAL */
/* PRQA S 6060 PARAM */ /* MD_MSR_STPAR */
/* PRQA S 6080 STMIF */ /* MD_MSR_STMIF */
FUNC(void, SRP_CODE) Srp_Msrp_RxIndication(uint8 VCtrlIdx, Eth_FrameType FrameType, boolean IsBroadcast,
  SRP_P2VAR(uint8) PhysAddrPtr, SRP_P2VAR(uint8) DataPtr, uint16 LenByte) /* PRQA S 3673 */ /* MD_Srp_16.7 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Srp_PortIdxType PortIdx = SRP_INV_PORT_IDX; /* PRQA S 0781 */ /* MD_Srp_5.6 */
  Srp_PortIdxType PortCnt = SRP_PORT_COUNT;
  uint16 Offset = 0;
  boolean LeaveAllEvent = FALSE;
  uint16 NumberOfValues = 0;
  uint16 AttributeListLength; /* PRQA S 0781 */ /* MD_Srp_5.6 */
  uint8 VectorLength;
  uint16 AttributeCnt;
  Srp_MsrpAttributeType AttributeType; /* PRQA S 0781 */ /* MD_Srp_5.6 */
  Srp_MsrpStreamInfoType FirstValueTalker;
  Srp_StreamIdType FirstValueListener;
  Srp_MsrpDomainInfoType FirstValueDomain;

  /* ----- Development Error Checks ------------------------------------- */
  /* PRQA S 3109 DET */ /* MD_MSR_14.3 */
  /* Check initialization state of the component */
  Srp_CheckDetErrorReturnVoid(SRP_COND_INIT, SRP_SID_MSRP_RX_INDICATION, SRP_E_UNINIT); 
  /* PRQA L:DET */
  /* ----- Implementation ----------------------------------------------- */
  /* PRQA S 3112 3 */ /* MD_MSR_14.2 */
  SRP_DUMMY_STATEMENT(PhysAddrPtr);
  SRP_DUMMY_STATEMENT(IsBroadcast);
  SRP_DUMMY_STATEMENT(FrameType);

  /* search for the port index */
  while(PortCnt > 0u)
  {
    PortCnt--;
    if(Srp_PortConfig[PortCnt].CtrlIdx == VCtrlIdx)
    {
      PortIdx = PortCnt;
      break;
    }
  }

  if(PortIdx == SRP_INV_PORT_IDX)
  {
    /* no corresponding Port Index found --> Drop message */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_MSRP_RX_INDICATION, SRP_E_INV_PARAM);
    return;
  }

  if(LenByte < (SRP_COMMON_HEADER_LENGTH + SRP_COMMON_MSG_LENGTH))
  {
    /* invalid message length, message is smaller than common header length + common message length --> drop message */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    Srp_CallDetReportError(SRP_SID_MSRP_RX_INDICATION, SRP_E_INV_MSG_LENGTH);
    return;
  }

  /* check Protocol Version */
  if(DataPtr[Offset] != SRP_PROTOCOL_VERSION)
  {
    /* invalid Protocol version --> drop message */
    return;
  }
  Offset += 1;

  /* get and process all messages of the received frame */
  while(Offset < LenByte)
  {

    boolean TalkerFailed = FALSE;
    /* get Attribute Type */
    AttributeType = DataPtr[Offset];
    Offset += 1;
    switch(AttributeType)
    {
    case SRP_MSRP_TALKER_FAILED:
      TalkerFailed = TRUE; /*lint -e616 */
    case SRP_MSRP_TALKER_ADVERTISE: /* PRQA S 2003 */ /* MD_Srp_15.2 */ /*lint +e616 */
      if( ((TalkerFailed == FALSE) && ((Offset + SRP_MSRP_TALKER_AD_MSG_LENGTH) > LenByte)) ||
          ((TalkerFailed == TRUE)  && ((Offset + SRP_MSRP_TALKER_FAILED_MSG_LENGTH) > LenByte)) )
      {
        /* invalid message length --> drop message */
        return;
      }

      /* Get and check Attribute Length */
      if( ((TalkerFailed == FALSE) && (DataPtr[Offset] != SRP_MSRP_TALKER_ADVERTISE_LENGTH)) || 
        ((TalkerFailed == TRUE)  && (DataPtr[Offset] != SRP_MSRP_TALKER_FAILED_LENGTH)) )
      {
        /* invalid Attribute Length --> drop message */
        return;
      }
      Offset += 1;

      /* Get Attribute List Length */
      AttributeListLength = (uint16)Srp_Ntohs(*((uint16*)&DataPtr[Offset])); /* PRQA S 0310 */ /* MD_Srp_11.4 */
      Offset += 2;

      /* iterate all Vector Attributes (AttributeList) */
      while(Offset < LenByte)
      {
        uint16 EndOfMessage = Offset + AttributeListLength;

        Srp_GetLeavAllAndNumberOfValues(DataPtr, &Offset, &LeaveAllEvent, &NumberOfValues);
        /* Number of values sanity check */
        if(NumberOfValues < 1u)
        {
          /* invalid Number of values --> skip message */
          Offset = EndOfMessage;
          break;
        }
        Srp_GetStreamInfo(DataPtr, &Offset, &FirstValueTalker, TalkerFailed);

        /* calculate Vector length and check remaining length */
        VectorLength = (uint8)(NumberOfValues / 3u);
        if((NumberOfValues % 3) != 0)
        {
          VectorLength += 1;
        }
        if((Offset + VectorLength) >= LenByte)
        {
          /* invalid frame length --> drop message */
          return;
        }

        /* Get and Process Attributes */
        AttributeCnt = NumberOfValues;
        do
        {
          uint8 ThreePackedEvent = DataPtr[Offset];
          Offset += 1;

          if(AttributeCnt >= 3u)
          {
            /* Get 3 Events from ThreePackedEvents */
            uint8 FirstEvent = 0;
            uint8 SecondEvent = 0;
            uint8 ThirdEvent = 0;

            AttributeCnt -= 3;
            Srp_UnpackThreePackedEvent(ThreePackedEvent, &FirstEvent, &SecondEvent, &ThirdEvent);

            Srp_ProcessAttributeTalker(PortIdx, FirstValueTalker, FirstEvent, LeaveAllEvent, 
              SRP_MSRP_TALKER_ADVERTISE);
            Srp_IncrementFirstValueTalker(FirstValueTalker.StreamId.UniqueId, 
              FirstValueTalker.DataFrameParameters.DestinationAddress);

            Srp_ProcessAttributeTalker(PortIdx, FirstValueTalker, SecondEvent, LeaveAllEvent, 
              SRP_MSRP_TALKER_ADVERTISE);
            Srp_IncrementFirstValueTalker(FirstValueTalker.StreamId.UniqueId, 
              FirstValueTalker.DataFrameParameters.DestinationAddress);

            Srp_ProcessAttributeTalker(PortIdx, FirstValueTalker, ThirdEvent, LeaveAllEvent, 
              SRP_MSRP_TALKER_ADVERTISE);
            Srp_IncrementFirstValueTalker(FirstValueTalker.StreamId.UniqueId, 
              FirstValueTalker.DataFrameParameters.DestinationAddress);
          }
          else if(AttributeCnt == 2)
          {
            /* Get 2 Events from ThreePackedEvents */
            uint8 FirstEvent = 0;
            uint8 SecondEvent = 0;

            AttributeCnt -= 2;
            Srp_UnpackThreePackedEvent(ThreePackedEvent, &FirstEvent, &SecondEvent, NULL_PTR);

            Srp_ProcessAttributeTalker(PortIdx, FirstValueTalker, FirstEvent, LeaveAllEvent, 
              SRP_MSRP_TALKER_ADVERTISE);
            Srp_IncrementFirstValueTalker(FirstValueTalker.StreamId.UniqueId, 
              FirstValueTalker.DataFrameParameters.DestinationAddress);

            Srp_ProcessAttributeTalker(PortIdx, FirstValueTalker, SecondEvent, LeaveAllEvent, 
              SRP_MSRP_TALKER_ADVERTISE);
            Srp_IncrementFirstValueTalker(FirstValueTalker.StreamId.UniqueId, 
              FirstValueTalker.DataFrameParameters.DestinationAddress);
          }
          else
          {
            /* Get 1 Events from ThreePackedEvents */
            uint8 FirstEvent = 0;

            AttributeCnt -= 1;
            Srp_UnpackThreePackedEvent(ThreePackedEvent, &FirstEvent, NULL_PTR, NULL_PTR);
            Srp_ProcessAttributeTalker(PortIdx, FirstValueTalker, FirstEvent, LeaveAllEvent, 
              SRP_MSRP_TALKER_ADVERTISE);
          }

          Srp_IncrementFirstValueTalker(FirstValueTalker.StreamId.UniqueId, 
             FirstValueTalker.DataFrameParameters.DestinationAddress);

        }while(AttributeCnt > 0u);

        /* Check if End of Frame is reached */
        if((Offset + SRP_END_MARK_LENGTH) > LenByte)
        {
          /* Reached End of Frame --> Stop processing of the message */
          return;
        }
        /* check if EndMark is reached */
        if(Srp_Ntohs(*((uint16*)&DataPtr[Offset])) == SRP_END_MARK) /* PRQA S 0310 */ /* MD_Srp_11.4 */
        {
          /* reached EndMark --> Continue with next message */
          Offset += SRP_END_MARK_LENGTH;
          break; /* PRQA S 0771 */ /* MD_Srp_14.6 */
        }
        
      } /* End: Iteration of Vector Attributes (AttributeList) */
      break;
    case SRP_MSRP_LISTENER:
      if((Offset + SRP_MSRP_LISTENER_MSG_LENGTH) > LenByte)
      {
        /* invalid message length --> drop message */
        return;
      }
      
      /* Get and Check Attribute Length */
      if(DataPtr[Offset] != SRP_MSRP_LISTENER_LENGTH)
      {
        /* invalid Attribute Length --> drop message */
        return;
      }
      Offset += 1;

      /* Get Attribute List Length */
      AttributeListLength = (uint16)Srp_Ntohs(*((uint16*)&DataPtr[Offset])); /* PRQA S 0310 */ /* MD_Srp_11.4 */
      Offset += 2;

      /* iterate all Vector Attributes (AttributeList) */
      while(Offset < LenByte)
      {
        uint16 EndOfMessage = Offset + AttributeListLength;

        uint8 ThreePackedCnt;
        uint8 FourPackedCnt;

        uint8 ThreePackedEvent;
        uint8 FourPackedEvent;

        uint8 IndexVec3Packed = 0;
        uint8 IndexVec4Packed = 0;

        uint8 ThreePackedVec[] = 
        { 
          0, 0, 0
        };
        uint8 FourPackedVec[] = 
        {
          0, 0, 0, 0
        };

        Srp_GetLeavAllAndNumberOfValues(DataPtr, &Offset, &LeaveAllEvent, &NumberOfValues);
        /* Number of values sanity check */
        if(NumberOfValues < 1u)
        {
          /* invalid Number of values --> skip message */
          Offset = EndOfMessage;
          break;
        }

        /* Get First Value */
        /*lint -e545 */
        Srp_GetMacAddress(DataPtr, &Offset, &FirstValueListener.SourceAddress); /*lint +e545 */
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        FirstValueListener.UniqueId = (uint16)Srp_Ntohs(*((uint16*)&DataPtr[Offset]));
        Offset += 2;

        /* calculate Vector Length and check remaining length */
        ThreePackedCnt = (uint8)(NumberOfValues / 3u);
        if((NumberOfValues % 3) != 0u)
        {
          ThreePackedCnt += 1;
        }
        FourPackedCnt = (uint8)(NumberOfValues / 4u);
        if((NumberOfValues % 4u) != 0u)
        {
          FourPackedCnt += 1;
        }
        VectorLength = ThreePackedCnt + FourPackedCnt;

        if((Offset + VectorLength) >= LenByte) 
        {
          /* invalid frame length --> drop message */
          return;
        }

        /* Get and Process Attributes */
        AttributeCnt = NumberOfValues;

        ThreePackedEvent = DataPtr[Offset];
        FourPackedEvent = DataPtr[Offset + ThreePackedCnt];

        Offset += 1;

        Srp_UnpackThreePackedEvent(ThreePackedEvent, &ThreePackedVec[0], &ThreePackedVec[1], &ThreePackedVec[2]);
        Srp_UnpackFourPackedEvent(FourPackedEvent, &FourPackedVec[0], &FourPackedVec[1], &FourPackedVec[2], 
          &FourPackedVec[3]);

        do
        {
          if(IndexVec3Packed > 2u)
          {
            IndexVec3Packed = 0u;
            ThreePackedEvent = DataPtr[Offset];
            Srp_UnpackThreePackedEvent(ThreePackedEvent, &ThreePackedVec[0], &ThreePackedVec[1], &ThreePackedVec[2]);
          }

          if(IndexVec4Packed > 3u)
          {
            IndexVec4Packed = 0;
            Srp_UnpackFourPackedEvent(FourPackedEvent, &FourPackedVec[0], &FourPackedVec[1], &FourPackedVec[2], 
              &FourPackedVec[3]);
            Offset += 1;
          }

          Srp_ProcessAttributeListener(PortIdx, FirstValueListener, ThreePackedVec[IndexVec3Packed], LeaveAllEvent, 
            FourPackedVec[IndexVec4Packed]);

          FirstValueListener.UniqueId++;

          IndexVec3Packed++;
          IndexVec4Packed++;
          AttributeCnt --;
        }while(AttributeCnt > 0u);
        Offset += ThreePackedCnt; /* Add VectorLength to skip the FourPackedEvents */

        /* Check if End of Frame is reached */
        if((Offset + SRP_END_MARK_LENGTH) > LenByte)
        {
          /* Reached End of Frame --> Stop processing of the message */
          return;
        }
        /* check if EndMark is reached */
        if(Srp_Ntohs(*((uint16*)&DataPtr[Offset])) == SRP_END_MARK) /* PRQA S 0310 */ /* MD_Srp_11.4 */
        {
          /* reached EndMark --> Continue with next message */
          Offset += SRP_END_MARK_LENGTH;
          break; /* PRQA S 0771 */ /* MD_Srp_14.6 */
        }
      } /* END: Iteration Vector Attributes (AttributeList) */
      break;
    case SRP_MSRP_DOMAIN:
      if((Offset + SRP_MSRP_DOMAIN_MSG_LENGTH) > LenByte)
      {
        /* invalid message length --> drop message */
        return;
      }

      /* Get and Check Attribute Length */
      if(DataPtr[Offset] != SRP_MSRP_DOMAIN_LENGTH)
      {
        /* invalid Attribute Length --> drop message */
        return;
      }
      Offset += 1;

      /* Get attribute list length */
      /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
      AttributeListLength = (uint16)Srp_Ntohs(*((uint16*)&DataPtr[Offset]));
      Offset += 2;
      
      /* iterate all Vector Attributes (AttributeList) */
      while(Offset < LenByte)
      {
        uint16 EndOfMessage = Offset + AttributeListLength;

        Srp_GetLeavAllAndNumberOfValues(DataPtr, &Offset, &LeaveAllEvent, &NumberOfValues);
        /* Number of values sanity check */
        if(NumberOfValues < 1u)
        {
          /* invalid Number of values --> skip message */
          Offset = EndOfMessage;
          break;
        }

        /* Get FirstValue */
        FirstValueDomain.SrClassId = DataPtr[Offset];
        Offset += 1;
        FirstValueDomain.SrClassPriority = DataPtr[Offset];
        Offset += 1;
        /* PRQA S 0310 1 */ /* MD_Srp_11.4 */
        FirstValueDomain.SrClassVid = (uint16)Srp_Ntohs(*((uint16*)&DataPtr[Offset]));
        Offset += 2;

        /* calculate Vector Length and check remaining length */
        VectorLength = (uint8)(NumberOfValues / 3u);
        if((NumberOfValues % 3) != 0u)
        {
          VectorLength += 1;
        }
        if((Offset + VectorLength) >= LenByte)
        {
          /* invalid frame length --> drop message */
          return;
        }

        /* Get and Process Attributes */
        AttributeCnt = NumberOfValues;
        do
        {
          uint8 ThreePackedEvent = DataPtr[Offset];
          Offset += 1;

          if(AttributeCnt >= 3u)
          {
            /* Get 3 Events from ThreePackedEvents */
            uint8 FirstEvent = 0;
            uint8 SecondEvent = 0;
            uint8 ThirdEvent = 0;
       
            AttributeCnt -= 3;
            Srp_UnpackThreePackedEvent(ThreePackedEvent, &FirstEvent, &SecondEvent, &ThirdEvent);

            Srp_ProcessAttributeDomain(PortIdx, FirstValueDomain, FirstEvent, LeaveAllEvent);
            Srp_IncrementFirstValueDomain(FirstValueDomain); /* PRQA S 3109 */ /* MD_MSRP_14.3 */
            Srp_ProcessAttributeDomain(PortIdx, FirstValueDomain, SecondEvent, LeaveAllEvent);
            Srp_IncrementFirstValueDomain(FirstValueDomain); /* PRQA S 3109 */ /* MD_MSRP_14.3 */
            Srp_ProcessAttributeDomain(PortIdx, FirstValueDomain, ThirdEvent, LeaveAllEvent);
            Srp_IncrementFirstValueDomain(FirstValueDomain); /* PRQA S 3109 */ /* MD_MSRP_14.3 */

          }
          else if(AttributeCnt == 2)
          {
            /* Get 2 Events from ThreePackedEvents */
            uint8 FirstEvent = 0;
            uint8 SecondEvent = 0;
       
            AttributeCnt -= 2;
            Srp_UnpackThreePackedEvent(ThreePackedEvent, &FirstEvent, &SecondEvent, NULL_PTR);

            Srp_ProcessAttributeDomain(PortIdx, FirstValueDomain, FirstEvent, LeaveAllEvent);
            Srp_IncrementFirstValueDomain(FirstValueDomain); /* PRQA S 3109 */ /* MD_MSRP_14.3 */
            Srp_ProcessAttributeDomain(PortIdx, FirstValueDomain, SecondEvent, LeaveAllEvent);
            Srp_IncrementFirstValueDomain(FirstValueDomain); /* PRQA S 3109 */ /* MD_MSRP_14.3 */

          }
          else
          {
            /* Get 1 Event from ThreePackedEvents */
            uint8 FirstEvent = 0;
       
            AttributeCnt -= 1;
            Srp_UnpackThreePackedEvent(ThreePackedEvent, &FirstEvent, NULL_PTR, NULL_PTR);

            Srp_ProcessAttributeDomain(PortIdx, FirstValueDomain, FirstEvent, LeaveAllEvent);
          }

          Srp_IncrementFirstValueDomain(FirstValueDomain); /* PRQA S 3109 */ /* MD_MSRP_14.3 */

        }while(AttributeCnt > 0u);

        /* Check if End of Frame is reached */
        if((Offset + SRP_END_MARK_LENGTH) > LenByte)
        {
          /* Reached End of Frame --> Stop processing of the message */
          return;
        }
        /* check if EndMark is reached */
        if(Srp_Ntohs(*((uint16*)&DataPtr[Offset])) == SRP_END_MARK) /* PRQA S 0310 */ /* MD_Srp_11.4 */
        {
          /* reached EndMark --> Continue with next message */
          Offset += SRP_END_MARK_LENGTH;
          break; /* PRQA S 0771 */ /* MD_Srp_14.6 */
        }

      } /* END: iterate all Vector Attributes (AttributeList) */
      break;
    default:
      /* Invalid Attribute Type --> drop message */
      return;
    } /* End: switch(AttributeType) */
    /* Check if End of Frame is reached */
    if((Offset + SRP_END_MARK_LENGTH) > LenByte)
    {
      /* Reached End of Frame --> Stop processing of the message */
      return;
    }
    /* check if EndMark is reached */
    if(Srp_Ntohs(*((uint16*)&DataPtr[Offset])) == SRP_END_MARK) /* PRQA S 0310 */ /* MD_Srp_11.4 */
    {
      /* reached EndMark --> stop processing of message */
      return;
    }
  } /* End: Iteration of messages */

} /* Srp_Msrp_RxIndication() */ /* PRQA S 2006 */ /* MD_MSR_14.7 */
/* PRQA L: PATH_COUNT */
/* PRQA L: CYCLO */
/* PRQA L: CALLS */
/* PRQA L: PARAM */
/* PRQA L: STMIF */

#define SRP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:
  MD_Srp_5.6:
      Reason:     Reuse of variable name improves readability.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_Srp_11.4:
      Reason:     Casting to different object pointer type.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_Srp_12.4:
      Reason:     The right hand operand of the && operator is only evaluated if the left hand operand is 'true'
      Risk:       No risk. No assignments are done in conditional statements.
      Prevention: Covered by code review.
  MD_Srp_14.6
      Reason:     Multiple break statements in iteration
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_Srp_15.2
      Reason:     Non-empty case does not end with an explicit 'break' statement.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_Srp_16.7
      Reason:     No pointer to const due to modification in a called function or required by specification.
      Risk:       No risk.
      Prevention: Covered by code review.
  MD_Srp_19.15
      Reason:     Standard include guard is present. Wrong MISRA warning.
      Risk:       No risk.
      Prevention: Covered by code review.
*/

/**********************************************************************************************************************
 *  END OF FILE: Srp.c
 *********************************************************************************************************************/
