/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Srp.h
 *        \brief  SRP header file
 *
 *      \details  Header File of the Stream Registration Protocol (SRP)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Michael Seidenspinner         vissem        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2015-05-13  vissem  ESCAN00082698 FEAT-1457: SRP module development
 *  01.01.00  2015-06-30  vissem  ESCAN00083718 Removed DET Instance ID from Srp.h
 *  01.02.00  2015-07-03  vissem  ESCAN00083775 ReservationFailureCode is wrong within Talker Failed messages
 *                                ESCAN00083789 SRP transmits empty messages
 *                                ESCAN00083790 Deregister Stream Notification Callback is not called
 *                                ESCAN00083792 AttributeListLength of MSRP Listener message is incorrect
 *            2015-07-22  vissem  ESCAN00084092 No LeaveAll messages are transmitted for MVRP
 *            2015-07-23  vissem  ESCAN00083861 Adopt Type of DataPtr in RxIndication-functions
 *            2015-10-21  vissem  ESCAN00085984 Misaligned memory/Ethernet-Buffer access
 *  2.00.00   2016-02-16  vissem  -             FEAT-1372: Support Java 8 and R14 CFG5 Breaking Changes
 *  3.00.00   2016-12-08  vissem  ESCAN00093206 Integrated findings of the Review
 *  3.01.00   2016-12-23  vissem  FEATC-246     FEAT-2068: Finalize SRP Client
 *                                ESCAN00093285 Dynamic Srp is not working
 *********************************************************************************************************************/
#ifndef SRP_H
# define SRP_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "Srp_Types.h"
# include "Srp_Cfg.h"
# include "Srp_Lcfg.h"
# include "Srp_Cbk.h"
# include "Srp_Priv.h"

# if (SRP_DEV_ERROR_DETECT == STD_ON)
#  include "Det.h"       /* Include Development Error Tracer */
# endif

/* PRQA S 0857 MACRO_LIMIT */ /* MD_MSR_1.1_857 */
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
 /* ##V_CFG_MANAGEMENT ##CQProject :   SysService_Srp8021Q CQComponent : Implementation */
/* BCD coded version number */
#define SYSSERVICE_SRP8021Q_VERSION              (0x0301U)
/* BCD coded release version number */
#define SYSSERVICE_SRP8021Q_RELEASE_VERSION      (0x00U)

/* Vendor and module identification */
# define SRP_VENDOR_ID                           (30u)
# define SRP_MODULE_ID                           (0xFFu)

/* AUTOSAR Software specification version information */
# define SRP_AR_RELEASE_MAJOR_VERSION            (0x04u)
# define SRP_AR_RELEASE_MINOR_VERSION            (0x02u)
# define SRP_AR_RELEASE_REVISION_VERSION         (0x01u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define SRP_SW_MAJOR_VERSION                    (SYSSERVICE_SRP8021Q_VERSION >> 8u)
# define SRP_SW_MINOR_VERSION                    (SYSSERVICE_SRP8021Q_VERSION & 0x00FF)
# define SRP_SW_PATCH_VERSION                    (SYSSERVICE_SRP8021Q_RELEASE_VERSION)

/* ----- API service IDs ----- */
/*! \brief Service ID: Srp_MainFunction() */
# define SRP_SID_MAIN_FUNCTION                   (0x01u)
/*! \brief Service ID: Srp_GetVersionInfo() */
# define SRP_SID_GET_VERSION_INFO                (0x02u)
/*! \brief Service ID: Srp_Init() */
# define SRP_SID_INIT                            (0x03u)
/*! \brief Service ID: Srp_InitPort() */
# define SRP_SID_INIT_PORT                       (0x04u)
/*! \brief Service ID: Srp_InitSmLeaveAll() */
# define SRP_SID_INIT_LEAVE_ALL_SM               (0x05u)
/*! \brief Service ID: Srp_InitSmPeriodicTransmission() */
# define SRP_SID_INIT_PERIODIC_TRANSMISSION_SM   (0x06u)
/*! \brief Service ID: Srp_InitSmApplicant() */
# define SRP_SID_INIT_APPLICANT_SM               (0x07u)
/*! \brief Service ID: Srp_InitSmRegistrar() */
# define SRP_SID_INIT_REGISTRAR_SM               (0x08u)

/*! \brief Service ID: Srp_RegisterStream() */
# define SRP_SID_REGISTER_STREAM                 (0x10u)
/*! \brief Service ID: Srp_DeregisterStream() */
# define SRP_SID_DERGISTER_STREAM                (0x11u)
/*! \brief Service ID: Srp_RegisterAttach() */
# define SRP_SID_REGISTER_ATTACH                 (0x12u)
/*! \brief Service ID: Srp_DeregisterAttach() */
# define SRP_SID_DEREGISTER_ATTACH               (0x13u)
/*! \brief Service ID: Srp_RegisterMacAddress() */
# define SRP_SID_REGISTER_MAC_ADDRESS            (0x14u)
/*! \brief Service ID: Srp_DeregisterMacAddress() */
# define SRP_SID_DEREGISTER_MAC_ADDRESS          (0x15u)
/*! \brief Service ID: Srp_RegisterServiceRequirement() */
# define SRP_SID_REGISTER_SERVICE_REQUIREMENT    (0x16u)
/*! \brief Service ID: Srp_DeregisterServiceRequirement() */
# define SRP_SID_DEREGISTER_SERVICE_REQUIREMENT  (0x17u)
/*! \brief Service ID: Srp_RegisterVlanMember() */
# define SRP_SID_REGISTER_VLAN_MEMBER            (0x18u)
/*! \brief Service ID: Srp_DeregisterVlanMember() */
# define SRP_SID_DEREGISTER_VLAN_MEMBER          (0x19u)

/*! \brief Service ID: Srp_Msrp_RxIndication() */
# define SRP_SID_MSRP_RX_INDICATION              (0x20u)
/*! \brief Service ID: Srp_Mvrp_RxIndication() */
# define SRP_SID_MVRP_RX_INDICATION              (0x21u)
/*! \brief Service ID: Srp_Mmrp_RxIndication() */
# define SRP_SID_MMRP_RX_INDICATION              (0x22u)
/*! \brief Service ID: Srp_TxConfirmation() */
# define SRP_SID_TX_CONFIRMATION                 (0x23u)
/*! \brief Service ID: Srp_Cbk_TrcvLinkStateChg */
# define SRP_SID_CBK_TRCV_LINK_STATE_CHG         (0x24u)

/*! \brief Service ID: Srp_ProcessSmApplicant() */
# define SRP_SID_SM_APPLICANT                    (0x30u)
/*! \brief Service ID: Srp_ProcessSmRegistrar() */
# define SRP_SID_SM_REGISTRAR                    (0x31u)
/*! \brief Service ID: Srp_ProcessSmLeaveAll() */
# define SRP_SID_SM_LEAVE_ALL                    (0x32u)
/*! \brief Service ID: Srp_ProcessSmPeriodicTransmission() */
# define SRP_SID_SM_PERIODIC_TRANSMISSION        (0x33u)

/*! \brief Service ID: Srp_Transmit() */
# define SRP_SID_TRANSMIT                        (0x40u)
/*! \brief Service ID: Srp_AssembleMsg() */
# define SRP_SID_ASSEMBLE_MSG                    (0x41u)
/*! \brief Service ID: Srp_AssembleMsgInOrMt() */
# define SRP_SID_ASSEMBLE_MSG_IN_OR_MT           (0x42u)
/*! \brief Service ID: Srp_AssembleMsgJoin() */
# define SRP_SID_ASSEMBLE_MSG_JOIN               (0x43u)
/*! \brief Service ID: Srp_RequestTransmitOpportunity() */
# define SRP_SID_REQUEST_TRANSMIT_OPPORTUNITY    (0x44u)

/*! \brief Service ID: Srp_ProcessAttributeMac() */
# define SRP_SID_PROCESS_ATTRIBUTE_MAC           (0x50u)
/*! \brief Service ID: Srp_ProcessAttributeServiceReq() */
# define SRP_SID_PROCESS_ATTRIBUTE_SERVICE_REQ   (0x51u)
/*! \brief Service ID: Srp_ProcessAttributeVid() */
# define SRP_SID_PROCESS_ATTRIBUTE_V_LAN         (0x52u)
/*! \brief Service ID: Srp_ProcessAttributeTalker() */
# define SRP_SID_PROCESS_ATTRIBUTE_TALKER        (0x53u)
/*! \brief Service ID: Srp_ProcessAttributeListener() */
# define SRP_SID_PROCESS_ATTRIBUTE_LISTENER      (0x54u)
/*! \brief Service ID: Srp_ProcessAttributeDomain() */
# define SRP_SID_PROCESS_ATTRIBUTE_DOMAIN        (0x55u)

/*! \brief Service ID: Srp_MrpHasAtLeastOneValidAttribute */
# define SRP_SID_MRP_VALID_ATTRIBUTE             (0x60u)

/* ----- Error codes ----- */
/*! \brief Error code: API service used without module initialization */
# define SRP_E_UNINIT                            (0x01u)
/*! \brief Error code: The service Srp_Init() is called while the module is already initialized  */
# define SRP_E_ALREADY_INITIALIZED               (0x2u)
/*! \brief Error code: API service called with NULL_PTR as parameter  */
# define SRP_E_NULL_POINTER                      (0x03u)
/*! \brief Error code: API service called with invalid parameter */
# define SRP_E_INV_PARAM                         (0x04u)
/*! \brief Error code: ProcessSm called with invalid state machine */
# define SRP_E_INV_STATE_MACHINE                 (0x05u)
/*! \brief Error code: Transmission failed */
# define SRP_E_TX_FAILED                         (0x06u)
/*! \brief Error code: Invalid MRP Application */
# define SRP_E_INV_MRP_APPLICATION               (0x07u)
/*! \brief Error code: Invalid Event */
# define SRP_E_INV_EVENT                         (0x08u)
/*! \brief Error code: MMRP API service called but MMRP is not supported */
# define SRP_E_MMRP_NOT_SUPPORTED                (0x09u)
/*! \brief Error code: Invalid message length */
# define SRP_E_INV_MSG_LENGTH                    (0x0Au)
/*! \brief Error code: Invalid buffer index */
# define SRP_E_INV_BUF_IDX                       (0x0Bu)
/*! \brief Error code: Invalid buffer Pointer */
# define SRP_E_INV_BUF_PTR                       (0x0Cu)
/*! \brief Error code: Unable to get TxBuffer from Ethernet Interface */
# define SRP_E_NO_TX_BUFFER                      (0x0Du)
/*! \brief Error code: Not supported Protocol version */
# define SRP_E_NOT_SUPPORTED_PROTOCOL_VERSION    (0x0Eu)

/*! \brief Invalid Port Index */
# define SRP_INV_PORT_IDX                        ((Srp_PortIdxType)(255u))
/*! \brief Invalid Buffer Index */
# define SRP_INV_BUF_IDX                         ((uint8)(255u))
/*! \brief Invalid Attribute Index */
# define SRP_INV_ATTRIBUTE_IDX                   ((Srp_AttributeIdxType)(255u))

/* ----- Mapping of error reporting macro ----- */
# if (SRP_DEV_ERROR_DETECT == STD_ON)
#  define Srp_CheckDetErrorReturnVoid(CONDITION, API_ID, ERROR_CODE) \
  {  \
    if (!(CONDITION)) \
    {  \
      (void)Det_ReportError((uint16)SRP_MODULE_ID, (uint8)SRP_INSTANCE_ID_DET, (uint8)(API_ID), (uint8)(ERROR_CODE)); \
      return; \
    } \
  } /* PRQA S 3458, 3109 */ /* MD_MSR_19.4, MD_MSR_14.3 */
#  define Srp_CheckDetErrorReturnValue(CONDITION, API_ID, ERROR_CODE, RET_VAL) \
  { \
    if (!(CONDITION)) \
    { \
      (void)Det_ReportError((uint16)SRP_MODULE_ID, (uint8)SRP_INSTANCE_ID_DET, (uint8)(API_ID), (uint8)(ERROR_CODE)); \
      return (RET_VAL); \
    } \
  } /* PRQA S 3458 */ /* MD_MSR_19.4 */
#  define Srp_CheckDetErrorContinue(CONDITION, API_ID, ERROR_CODE) \
  { \
    if (!(CONDITION)) \
    { \
      (void)Det_ReportError((uint16)SRP_MODULE_ID, (uint8)SRP_INSTANCE_ID_DET, (uint8)(API_ID), (uint8)(ERROR_CODE)); \
    } \
  } /* PRQA S 3458 */ /* MD_MSR_19.4 */

   /* PRQA S 3458 4 */ /* MD_MSR_19.4 */
   /* PRQA S 3453 5 */ /* MD_MSR_19.7 */
#  define Srp_CallDetReportError(API_ID, ERROR_CODE) \
  ((void)Det_ReportError((uint16)SRP_MODULE_ID, (uint8)SRP_INSTANCE_ID_DET, (uint8)(API_ID), (uint8)(ERROR_CODE)))
# else
#  define Srp_CheckDetErrorReturnVoid(CONDITION, API_ID, ERROR_CODE)
#  define Srp_CheckDetErrorReturnValue(CONDITION, API_ID, ERROR_CODE, RET_VAL)
#  define Srp_CheckDetErrorContinue(CONDITION, API_ID, ERROR_CODE)
#  define Srp_CallDetReportError(API_ID, ERROR_CODE)
# endif

/* ----- Mapping of DET condition macros ----- */
/* PRQA S 3453 5 */ /* MD_MSR_19.7 */
# define SRP_COND_INIT                            ( Srp_State == SRP_STATE_INIT )   /* SRP_E_UNINIT */
# define SRP_COND_UNINIT                          ( Srp_State == SRP_STATE_UNINIT ) /* SRP_E_ALREADY_INITIALIZED */
# define SRP_COND_VALID_PTR(Pointer)              ( (Pointer) != NULL_PTR )         /* SRP_E_NULL_POINTER */
# define SRP_COND_VALID_PORT(Port)                ( (Port) < SRP_PORT_COUNT )       /* SRP_E_INV_PARAM */
# define SRP_COND_VALID_MVRP_FIRST_VALUE(FV)      ( ((FV) < 1u) || ((FV) > 4094u) )

/* ----- Constants ------------------------------------------------------ */

/*! \brief      Protocol Version */
# define SRP_PROTOCOL_VERSION                    (0x00u)

/*! \brief      Srp VLan Priority */
# define SRP_VLAN_PRIORITY                       (0x00)

/*! \brief      Srp Class Measurement Intervals */
# define SRP_SR_CLASS_A_MEASUREMENT_INTERVAL     (125u) /* us */
# define SRP_SR_CLASS_B_MEASUREMENT_INTERVAL     (250u) /* us */

/*! \brief      Srp SR class IDs */
# define SRP_SR_CLASS_INV_ID                     (0u)
# define SRP_SR_CLASS_A_ID                       (6u)
# define SRP_SR_CLASS_B_ID                       (5u)

/*! \brief      Frame Preamble Size */
# define SRP_FRAME_PREAMBLE_SIZE                 (8u) /* Byte */
/*! \brief      Frame Header Size */
# define SRP_FRAME_HEADER_SIZE                   (14u) /* Byte */
/*! \brief      Frame CRC Size */
# define SRP_FRAME_CRC_SIZE                      (4u) /* Byte */
/*! \brief      Inter-Frame-Gap (IFG) Size */
# define SRP_IFG_SIZE                            (12u) /* Byte */
/*! \brief      Srp VLan Tag Size */
# define SRP_V_LAN_TAG_SIZE                      (4u) /* Byte */
/*! \brief      Srp Per Frame Overhead */
# define SRP_PER_FRAME_OVERHEAD                  (SRP_FRAME_PREAMBLE_SIZE + SRP_FRAME_HEADER_SIZE\
                                                  + SRP_V_LAN_TAG_SIZE + SRP_FRAME_CRC_SIZE + SRP_IFG_SIZE) /* Byte */

/* ----- Header/Message Lengths ----------------------------------------- */
# define SRP_COMMON_HEADER_LENGTH                (1u)
# define SRP_COMMON_MSG_LENGTH                   (2u)

# define SRP_END_MARK_LENGTH                     (2u)
#if (SRP_MMRP_SUPPORT == STD_ON)
# define SRP_MMRP_MAC_MSG_LENGTH                 ( (SRP_COMMON_MSG_LENGTH + 11u) )
# define SRP_MMRP_SERVICE_REQ_MSG_LENGTH         ( (SRP_COMMON_MSG_LENGTH + 6u) )
#endif /* (SRP_MMRP_SUPPORT == STD_ON) */

# define SRP_MVRP_MSG_LENGTH                     ( (SRP_COMMON_MSG_LENGTH + 7u))

# define SRP_MSRP_MSG_LENGTH                     ( (SRP_COMMON_MSG_LENGTH + 2u) )
# define SRP_MSRP_TALKER_AD_MSG_LENGTH           ( (SRP_MSRP_MSG_LENGTH + 30u) )
# define SRP_MSRP_TALKER_FAILED_MSG_LENGTH       ( (SRP_MSRP_MSG_LENGTH + 39u) )
# define SRP_MSRP_LISTENER_MSG_LENGTH            ( (SRP_MSRP_MSG_LENGTH + 14u) )
# define SRP_MSRP_DOMAIN_MSG_LENGTH              ( (SRP_MSRP_MSG_LENGTH + 9u) )

/* ----- Attribute Types ------------------------------------------------ */
# define SRP_MVRP_ATTRIBUTE_TYPE                 (1u)

/* ----- Attribute Lengths ---------------------------------------------- */
#if (SRP_MMRP_SUPPORT == STD_ON)
# define SRP_MMRP_MAC_LENGTH                     (0x06u) /* 6 */
# define SRP_MMRP_SERVICE_REQUIREMENT_LENGTH     (0x01u) /* 1 */
#endif /* (SRP_MMRP_SUPPORT == STD_ON) */
# define SRP_MVRP_ATTRIBUTE_LENGTH               (0x02u) /* 2 */
# define SRP_MSRP_TALKER_ADVERTISE_LENGTH        (0x19u) /* 25 */
# define SRP_MSRP_TALKER_FAILED_LENGTH           (0x22u) /* 34 */
# define SRP_MSRP_LISTENER_LENGTH                (0x08u) /* 8 */
# define SRP_MSRP_DOMAIN_LENGTH                  (0x04u) /* 4 */

/* ----- MSRP FourPackedEvents ------------------------------------------ */
# define SRP_MSRP_LISTENER_IGNORE                (0x00u) /* 0 */
# define SRP_MSRP_LISTENER_ASKING_FAILED         (0x01u) /* 1 */
# define SRP_MSRP_LISTENER_READY                 (0x02u) /* 2 */
# define SRP_MSRP_LISTENER_READY_FAILED          (0x03u) /* 3 */

/* ----- End Mark ------------------------------------------------------- */
# define SRP_END_MARK                            (0x0000u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/* PRQA L: MACRO_LIMIT */
/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define SRP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Service functions */
/**********************************************************************************************************************
 *  Srp_InitMemory()
 *********************************************************************************************************************/
/*! \brief      Function for *_INIT_*-variable initialization
 *  \details    Service to initialize module global variables at power up. This function can be used to initialize the
 *              variables in *_INIT_* sections in case they are not initialized by the startup code.
 *  \param      none
 *  \return     none
 *  \pre        Module must not be initialized
 *  \pre        Function shall be called from task level
 *  \note       This function must be called before using the module
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_InitMemory(void);

/**********************************************************************************************************************
 * Srp_Init()
 *********************************************************************************************************************/
/*! \brief      Initialization function
 *  \details    This function initializes the module Srp. It initializes all variables and sets the module state to
 *              initialized.
 *  \param[in]  ConfigPtr               Configuration structure for initializing the module
 *  \return     none
 *  \pre        Interrupts have to be disabled.
 *  \pre        The module has to be uninitialized.
 *  \pre        Srp_InitMemory has been called unless Srp_ModuleInitialized is initialized by start-up code.
 *  \note       Specification of module initialization
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_Init(SRP_P2CONSTCFG(Srp_ConfigType) ConfigPtr);

/**********************************************************************************************************************
 *  Srp_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief      Returns the version information
 *  \details    Srp_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out] VersioninfoPtr             Pointer to where to store the version information
 *  \return     none
 *  \pre        Input parameter must not be NULL.
 *  \pre        Function shall be called from task level
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_GetVersionInfo(SRP_P2VAR(Std_VersionInfoType) VersioninfoPtr);

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
FUNC(Std_ReturnType, SRP_CODE) Srp_RegisterStream(
  Srp_PortIdxType PortIdx, SRP_P2CONST(Srp_MsrpRegisterStreamInfoType) StreamInfoPtr);

/**********************************************************************************************************************
 *  Srp_DeregisterStream
 *********************************************************************************************************************/
/*! \brief          Allows the Application to withdraw the offer of a Stream
 *  \param[in]      PortIdx              Port Index
 *  \param[in]      StreamID             The unique identifier of the Stream
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_DeregisterStream(
  Srp_PortIdxType PortIdx, Srp_StreamIdType StreamID);

/**********************************************************************************************************************
 *  Srp_RegisterAttach
 *********************************************************************************************************************/
/*! \brief          Allows the Application to register a offered Stream
 *  \param[in]      PortIdx                    Port Index
 *  \param[in]      StreamID                   The unique identifier of the Stream
 *  \return         E_OK                       Successfully registered the offered Stream
 *                  E_NOT_OK                   Failed to register the offered Stream
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SRP_CODE) Srp_RegisterAttach(
  Srp_PortIdxType PortIdx, Srp_StreamIdType StreamID);

/**********************************************************************************************************************
 *  Srp_DeregisterAttach
 *********************************************************************************************************************/
/*! \brief          Allows the Application to withdraw the registration of a Stream
 *  \param[in]      StreamID             The unique identifier of the Stream
 *  \param[in]      PortIdx              Port Index
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_DeregisterAttach(
  Srp_PortIdxType PortIdx, Srp_StreamIdType StreamID);

/**********************************************************************************************************************
 *  Srp_RegisterVlanMember
 *********************************************************************************************************************/
/*! \brief          Allows the Application to register membership of the specified VLan
 *  \param[in]      VID             The unique identifier of the VLan
 *  \param[in]      PortIdx                Port Index
 *  \return         E_OK            Successfully declared membership of the VLan
 *                  E_NOT_OK        Failed to declare membership of the VLan
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SRP_CODE) Srp_RegisterVlanMember(
  Srp_PortIdxType PortIdx, Srp_VLanIdType VID);

/**********************************************************************************************************************
 *  Srp_DeregisterVlanMember
 *********************************************************************************************************************/
/*! \brief          Allows the Application to withdraw the membership of the specified VLan
 *  \param[in]      VID             The unique identifier of the VLan
 *  \param[in]      PortIdx                Port Index
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_DeregisterVlanMember(
  Srp_PortIdxType PortIdx, Srp_VLanIdType VID);

#if (SRP_MMRP_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  Srp_RegisterMacAddress
 *********************************************************************************************************************/
/*! \brief          Allows the Application to register membership of the specified Multi-cast MAC Address
 *  \param[in]      MacAddressPtr          Pointer to the Multi-cast MacAddress
 *  \param[in]      PortIdx                Port Index
 *  \return         E_OK                   Successfully declared membership of the Multi-cast MAC Address
 *                  E_NOT_OK               Failed to declare membership of the Multi-cast MAC Address
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SRP_CODE) Srp_RegisterMacAddress(
  Srp_PortIdxType PortIdx, SRP_P2CONST(uint8) MacAddressPtr);

/**********************************************************************************************************************
 *  Srp_DeregisterMacAddress
 *********************************************************************************************************************/
/*! \brief          Allows the Application to withdraw membership of the specified Multi-cast MAC Address
 *  \param[in]      MacAddressPtr          Pointer to the Multi-cast MacAddress
 *  \param[in]      PortIdx                Port Index
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_DeregisterMacAddress(
  Srp_PortIdxType PortIdx, SRP_P2CONST(uint8) MacAddressPtr);

/**********************************************************************************************************************
 *  Srp_RegisterServiceRequirement
 *********************************************************************************************************************/
/*! \brief          Allows the Application to register the specified Service Requirement
 *  \param[in]      PortIdx                        Port Index
 *  \param[in]      ServiceRequirement             The Service Requirement
 *  \return         E_OK                           Successfully registered the Service Requirement
 *                  E_NOT_OK                       Failed to register the Service Requirement
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SRP_CODE) Srp_RegisterServiceRequirement(
  Srp_PortIdxType PortIdx, Srp_MmrpServiceRequirementAttributeType ServiceRequirement);

/**********************************************************************************************************************
 *  Srp_DeregisterServiceRequirement
 *********************************************************************************************************************/
/*! \brief          Allows the Application to withdraw registration of the specified Service Requirement
 *  \param[in]      PortIdx                        Port Index
 *  \param[in]      ServiceRequirement             The Service Requirement
 *  \return         none
 *  \context        This function can be called in any context.
 *********************************************************************************************************************/
FUNC(void, SRP_CODE) Srp_DeregisterServiceRequirement(
  Srp_PortIdxType PortIdx, Srp_MmrpServiceRequirementAttributeType ServiceRequirement);
#endif /* (SRP_MMRP_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  Srp_SetPeriodic
 *********************************************************************************************************************/
/*! \brief          Allows the Application to enable or disable the PeriodicTransmission state machine
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
FUNC(void, SRP_CODE) Srp_SetPeriodic(
  Srp_PortIdxType PortIdx, Srp_MrpApplicationType MrpApplication, boolean PeriodicEnabled);

# define SRP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* SRP_H */
/**********************************************************************************************************************
 *  END OF FILE: Srp.h
 *********************************************************************************************************************/
