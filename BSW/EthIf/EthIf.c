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
/**        \file  EthIf.c
 *        \brief  Ethernet Interface main source file
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

#define ETHIF_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthIf.h"
#include "EthIf_Cbk.h"
/* \trace SPEC-2393859 */
#include "EthIf_CfgAccess_Int.h"
#include "EthIf_Int.h"
#include "IpBase.h"
#include "EthIf_ExtndTrafficHndl_Int.h"
#include "EthSM.h"
#if ( ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT == STD_ON )
# include "BswM_EthIf.h"
#endif /* ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT */
#if ( ETHIF_FIREWALL_SUPPORT == STD_ON )
# include "EthFw.h"
#endif /* ETHIF_FIREWALL_SUPPORT */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of EthIf header file */
/* \trace SPEC-2393557 */
#if (  (ETHIF_SW_MAJOR_VERSION != (8u)) \
    || (ETHIF_SW_MINOR_VERSION != (0u)) \
    || (ETHIF_SW_PATCH_VERSION != (1u)) )
# error "Vendor specific version numbers of EthIf.c and EthIf.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Invalid wakeup Ethernet transceiver */
#define ETHIF_INVALID_WAKEUP_ETHTRCV  (0xFFU)

/*! Maximum amount of simultaneous initialization requests for one EthIf controller */
#define ETHIF_MAX_INIT_CNT   (0xFFU)
/*! Maximum amount of simultaneous ACTIVE mode requests for one EthIf controller */
#define ETHIF_MAX_MODE_CNT   (0xFFU)
/*! Maximum amount of simultaneous initialization requests for a hardware element */
#define ETHIF_MAX_HW_ELEM_INIT_CNT   (0xFFU)
/*! Maximum amount of simultaneous ACTIVE mode requests for a hardware element */
#define ETHIF_MAX_HW_ELEM_MODE_CNT   (0xFFU)

/*! Length of the Ethernet header without VLAN tag consideration */
#define ETHIF_HDR_LEN_BYTE            (14U)
/*! Offset into the Ethernet frame to EtherType location */
#define ETHIF_FRAME_TYPE_OFFSET       (12U)
/*! Minimum Ethernet Frame lenght */
#define ETHIF_MIN_ETH_FRAME_LEN_BYTE  (60U)

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

typedef struct
{
  P2VAR(uint8, AUTOMATIC, AUTOMATIC)  SrcMacPtr;
  P2VAR(uint8, AUTOMATIC, AUTOMATIC)  PayloadPtr;
        uint16                        PayloadLen;
        uint16                        EtherType;
        uint8                         EthIfCtrlIdx;
        boolean                       IsBroadcast;
} EthIf_UlRxIndiactionDataType;

typedef struct
{
  uint8    SrcMacAddr[ETH_PHYS_ADDR_LEN_BYTE];
  boolean  IsValid;
} EthIf_SrcMacAddrBufferType;

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define ETHIF_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(EthIf_StateType, ETHIF_VAR_INIT)  EthIf_ModuleInitialized = ETHIF_STATE_UNINIT;

#define ETHIF_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/
#define ETHIF_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( ETHIF_ENABLE_MAIN_FUNCTION_STATE == STD_OFF )
ETHIF_LOCAL VAR(uint8, ETHIF_VAR_NOINIT)  EthIf_MainFuncStateFreqCnt;
#endif /* ETHIF_ENABLE_MAIN_FUNCTION_STATE */

#define ETHIF_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define ETHIF_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

ETHIF_LOCAL VAR(EthIf_SrcMacAddrBufferType, ETHIF_VAR_NOINIT) EthIf_SrcMacAddrBuffer[ETHIF_ETHCTRL_NUM];

#define ETHIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define ETHIF_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_IsModeRequestPossible
 *********************************************************************************************************************/
/*! \brief      Checks if a mode request on the EthIf controller is possible.
 *  \details    Function checks if an additional mode request for the given target mode can be performed.
 *  \param[in]  virtCtrl    EthIf controller object
 *  \param[in]  trgtMode    Mode that the request is targeting for
 *  \return     TRUE - mode request can be processed
 *  \return     FALSE - mode request can not be processed
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_IsModeRequestPossible(
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST)  virtCtrl,
          Eth_ModeType                                 trgtMode);

#if ( ETHIF_ETHSWT_USED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_IsModeRequestPossibleForSwtPortGroup
 *********************************************************************************************************************/
/*! \brief      Checks if a mode request on the EthIf switch port group is possible.
 *  \details    Function checks if an additional mode request for the given target mode can be performed.
 *  \param[in]  ethIfSwtPortGroup    EthIf switch port group object
 *  \param[in]  trgtMode             Mode that the request is targeting for
 *  \return     TRUE - mode request can be processed
 *  \return     FALSE - mode request can not be processed
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_IsModeRequestPossibleForSwtPortGroup(
  P2CONST(EthIf_EthSwtPortGroupType, AUTOMATIC, ETHIF_CONST)  ethIfSwtPortGroup,
          Eth_ModeType                                        trgtMode);
#endif /* ETHIF_ETHSWT_USED */
#endif /* ETHIF_DEV_ERROR_DETECT */

/**********************************************************************************************************************
 *  EthIf_SetHardwareModeActive
 *********************************************************************************************************************/
/*! \brief      Sets the hardware to ACTIVE state
 *  \details    This functions sets the hardware elements mapped to the EthIf controller to state ACTIVE by utilizing
 *              the respective driver.
 *  \param[in]  virtCtrl    EthIf controller object
 *  \return     E_OK - success
 *  \return     E_NOT_OK - error occurred
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetHardwareModeActive(
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST)  virtCtrl);

/**********************************************************************************************************************
 *  EthIf_SetHardwareModeDown
 *********************************************************************************************************************/
/*! \brief      Sets the hardware to DOWN state
 *  \details    This functions sets the hardware elements mapped to the EthIf controller to state DOWN by utilizing
 *              the respective driver.
 *  \param[in]  virtCtrl    EthIf controller object
 *  \return     E_OK - success
 *  \return     E_NOT_OK - error occurred
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetHardwareModeDown(
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST)  virtCtrl);

#if ( ETHIF_ETHSWT_USED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_SetSwitchPortGroupMode
 *********************************************************************************************************************/
/*! \brief      Sets the Port Group Ports to ACTIVE or DOWN mode
 *  \details    This functions sets the hardware elements mapped to the EthIf controller to state ACTIVE by utilizing
 *              the respective driver.
 *  \param[in]  ethSwtPortGroup    EthSwtPortGroup object
 *  \param[in]  PortMode           Mode to set for the EthIfSwtPortGroup
 *                                 ETHTRCV_MODE_DOWN - disable the port group
 *                                 ETHTRCV_MODE_ACTIVE - enable the port group
 *  \return     E_OK - success
 *  \return     E_NOT_OK - error occurred
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetSwitchPortGroupMode(
  P2CONST(EthIf_EthSwtPortGroupType, AUTOMATIC, ETHIF_CONST)  ethSwtPortGroup,
          EthTrcv_ModeType                                    portMode);

/**********************************************************************************************************************
 *  EthIf_AdjustSwitchPortGroupModeCount
 *********************************************************************************************************************/
/*! \brief      Increments or decrements the Port Group Ports mode counter values
 *  \details    This functions sets the hardware elements mapped to the EthIf controller to state ACTIVE by utilizing
 *              the respective driver.
 *  \param[in]  ethSwtPortGroup    EthSwtPortGroup object
 *  \param[in]  PortMode           Increment or decrement the Port Group Ports Mode counter values
 *                                 ETHTRCV_MODE_DOWN - decrement the Port Group Ports Mode counter values
 *                                 ETHTRCV_MODE_ACTIVE - increments the Port Group Ports Mode counter values
 *  \return     E_OK - success
 *  \return     E_NOT_OK - error occurred
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_AdjustSwitchPortGroupModeCount(
  P2CONST(EthIf_EthSwtPortGroupType, AUTOMATIC, ETHIF_CONST)  ethSwtPortGroup,
          EthTrcv_ModeType                                    portMode);
#endif /* ETHIF_ETHSWT_USED */

/**********************************************************************************************************************
 *  EthIf_ProcessLinkStateChange
 *********************************************************************************************************************/
/*! \brief      Processes the link state change of a EthIf controller
 *  \details    This function checks for a link state change on the EthIf controller and notifies the EthIf users
 *              registered for a link state change notification about the change.
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_ProcessLinkStateChange(
  uint8 ethIfCtrlIdx);

#if ( ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_ProcessSwtPortGroupLinkStateChange
 *********************************************************************************************************************/
/*! \brief      Processes the link state changes for a EthIf switch port group
 *  \details    This function checks for a link state change on the EthIf switch port group and notifies the BswM
 *              about the change.
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_ProcessSwtPortGroupLinkStateChange(
  uint8 ethIfSwtPortGroupIdx);
#endif /* ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT */

/**********************************************************************************************************************
 *  EthIf_ProcessLinkStateChangeForAllEthIfCtrls
 *********************************************************************************************************************/
/*! \brief      Processes the link state changes for all EthIf controllers
 *  \details    This function checks for link state changes on all EthIf controllers and notifies the EthIf users
 *              registered for a link state change notification about changes.
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_ProcessLinkStateChangeForAllEthIfCtrls( void );

#if ( ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_ProcessAllEthIfSwtPortGroupLinkStateChanges
 *********************************************************************************************************************/
/*! \brief      Processes the link state changes for all EthIf switch port groups
 *  \details    This function checks for link state changes on all EthIf switch port groups and notifies the BswM
 *              about the change.
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_ProcessAllEthIfSwtPortGroupLinkStateChanges( void );
#endif /* ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT */

/**********************************************************************************************************************
 *  EthIf_ReportLinkStateChange
 *********************************************************************************************************************/
/*! \brief      Report a link state change to the users
 *  \details    This function reports the passed link state for an EthIf controller to the EthIf users.
 *  \param[in]  ethIfCtrlIdx    EthIf controller identifier
 *  \param[in]  linkState       Link state to be reported
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_ReportLinkStateChange(
  uint8                  ethIfCtrlIdx,
  EthTrcv_LinkStateType  linkState);

#if ( ETHIF_ETHTRCV_USED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_PollTrcvLinks
 *********************************************************************************************************************/
/*! \brief      Poll the link state of Ethernet transceivers
 *  \details    This function polls the link state of the Ethernet transceivers by utilizing the respective
 *              hardware driver.
 *  \context    Only allowed to be called within EthIf_DetectLinkStateChange().
 *  \pre        EthIf_DetectLinkStateChange() has to initialize all EthIf_EthIfCtrlLinkCnts to 0 before calling
 *              EthIf_PollTrcvLinks() and EthIf_PollPortLinks().
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_PollTrcvLinks( void );
#endif /* ETHIF_ETHTRCV_USED */

#if ( ETHIF_ETHSWT_USED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_PollPortLinks
 *********************************************************************************************************************/
/*! \brief      Poll the link state of Ethernet switch ports
 *  \details    This function polls the link state of the Ethernet switch ports by utilizing the respective
 *              hardware driver.
 *  \context    Only allowed to be called within EthIf_DetectLinkStateChange().
 *  \pre        EthIf_DetectLinkStateChange() has to initialize all EthIf_EthIfCtrlLinkCnts to 0 before calling
 *              EthIf_PollTrcvLinks() and EthIf_PollPortLinks().
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_PollPortLinks( void );

/**********************************************************************************************************************
 *  EthIf_PollPortGroupLinks
 *********************************************************************************************************************/
/*! \brief      Poll the link state of Ethernet switch port groups
 *  \details    This function polls the link state of the Ethernet switch port groups.
 *  \context    Only allowed to be called within EthIf_DetectLinkStateChange().
 *  \pre        EthIf_DetectLinkStateChange() has to initialize all EthIf_EthIfCtrlLinkCnts to 0 before calling
 *              EthIf_PollPortLinks() and thereafter EthIf_PollPortGroupLinks().
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_PollPortGroupLinks( void );
#endif /* ETHIF_ETHSWT_USED */

/**********************************************************************************************************************
 *  EthIf_DetectLinkStateChange
 *********************************************************************************************************************/
/*! \brief      Detects if a link change for the EthIf controllers has occurred
 *  \details    This functions detects if a link state change for the EthIf controllers has occurred.
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_DetectLinkStateChange(void);

/**********************************************************************************************************************
 *  EthIf_QueryForEthIfController
 *********************************************************************************************************************/
/*! \brief      Queries for the EthIf controller
 *  \details    This function queries for the EthIf controller by taking the related Ethernet controller, information
 *              if EthIf controller shall represent a non-tagged or tagged data path and in case of a tagged data path
 *              by taking the VLAN ID.
 *  \param[in]  ethCtrlIdx   Ethernet controller index
 *  \param[in]  vlanId       VLAN ID that shall be used in case no VLAN is used ETHIF_INV_VLAN_ID
 *  \return     EthIf_CfgAccess_InvalidEthIfCtrlIdx() - represents the first invalid EthIf controller index
 *                                                      -> no EthIf controller found
 *  \return     other values - EthIf controller index
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(uint8, ETHIF_CODE) EthIf_QueryForEthIfController(
  uint8    ethCtrlIdx,
  uint16   vlanId);

/**********************************************************************************************************************
 *  EthIf_IsRxAllowed
 *********************************************************************************************************************/
/*! \brief       Checks if the frame represented by the passed information is allowed to be received.
 *  \param[in]   ethCtrlIdx    Eth controller index the frame was received on
 *  \param[out]  ethIfCtrlIdx  EthIf controller index representing a valid data path for the frame (only valid if TRUE
 *                             is returned)
 *  \param[in]   frameHdr      Ethernet frame header information
 *  \param[in]   payloadPtr    Pointer to the frames payload
 *  \param[in]   payloadLen    Length of the frames payload
 *  \return      FALSE - Frame is not allowed to be received
 *  \return      TRUE - Frame is allowed to be received
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_IsRxAllowed(
          uint8                                     ethCtrlIdx,
    P2VAR(uint8,              AUTOMATIC, AUTOMATIC) ethIfCtrlIdx,
  P2CONST(EthIf_FrameHdrType, AUTOMATIC, AUTOMATIC) frameHdr,
  P2CONST(uint8,              AUTOMATIC, AUTOMATIC) payloadPtr,
          uint16                                    payloadLen);

/**********************************************************************************************************************
 *  EthIf_HandleUlRxIndication
 *********************************************************************************************************************/
/*! \brief      Handles the Rx indication of the upper layers
 *  \details    This function informs the upper layers about the reception of a frame they have registered for.
 *  \param[in]  ownerIdx             Frame owner index
 *  \param[in]  ulRxIndicationData   Data to retrieve the upper layers to inform and information about the frame.
 *  \return     FALSE - no upper layer registered for reception -> frame not received
 *  \return     TRUE - frame passed to upper layer -> frame received
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_HandleUlRxIndication(
          uint8                                                ownerIdx,
  P2CONST(EthIf_UlRxIndiactionDataType, AUTOMATIC, AUTOMATIC)  ulRxIndicationData);

/**********************************************************************************************************************
 *  EthIf_IsTxAllowed
 *********************************************************************************************************************/
/*! \brief       Checks if the frame represented by the passed information is allowed to be transmitted.
 *  \param[in]   txContext
 *  \return      FALSE - Frame is not allowed to be transmitted
 *  \return      TRUE - Frame is allowed to be transmitted
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_IsTxAllowed(
  P2CONST(EthIf_TxContextType, AUTOMATIC, AUTOMATIC) txContext);

/**********************************************************************************************************************
 *  EthIf_RetrieveSrcMacAddr
 *********************************************************************************************************************/
/*! \brief          Retrieves the source MAC address of the Ethernet controller used in the tx context.
 *  \param[in,out]  txContext  Tx context holding information about the controller the frame shall be transmitted on
 *                             and is used to return the retrieved source MAC address
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_RetrieveSrcMacAddr(
  P2VAR(EthIf_TxContextType, AUTOMATIC, AUTOMATIC) txContext);

/**********************************************************************************************************************
 *  EthIf_TearDownTransmission
 *********************************************************************************************************************/
/*! \brief      Tears down the transmission for the Ethernet frame silently
 *  \details    This function tears down the transmission of a Ethernet frame silently so upper layers don't notice
 *              the discard of the frame and the buffer used for frame transmission is available again.
 *  \param[in]  txContext          Information about the tx context
 *  \param[in]  bufIdx             Buffer index of the tx context
 *  \param[in]  isTxConfRequested  Information about if a Tx confirmation was requested for the tx context
 *  \return     E_NOT_OK - Transmission tear down wasn't successful
 *  \return     E_OK - Transmission tear down successful
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(Std_ReturnType, ETHIF_CODE) EthIf_TearDownTransmission(
  P2VAR(EthIf_TxContextType, AUTOMATIC, AUTOMATIC) txContext,
        uint8                                      bufIdx,
        boolean                                    isTxConfRequested);

#if ( ETHIF_WAKEUP_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_QueryForWakeupMap
 *********************************************************************************************************************/
/*! \brief      Queries for a wakeup map
 *  \details    This function queries for the wakeup map by taking the wakeup source passed.
 *  \param[in]  wakeupSource    Wakeup source of EcuM
 *  \return     ETHIF_INVALID_WAKEUP_MAP - no wakeup map found
 *  \return     other values - wakeup map index
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(uint8, ETHIF_CODE) EthIf_QueryForWakeupMap(
  EcuM_WakeupSourceType wakeupSource);

/**********************************************************************************************************************
 *  EthIf_QueryForWakeupTransceiver
 *********************************************************************************************************************/
/*! \brief      Queries for the wakeup Ethernet transceiver
 *  \details    This function queries for the wakeup Ethernet transceiver belonging to the EthIf controller.
 *  \param[in]  ethIfCtrlIdx    EthIf controller index
 *  \return     ETHIF_INVALID_WAKEUP_ETHTRCV - no wakeup transceiver found
 *  \return     other values - index of the transceiver within the physical layer elements of the EthIf controller
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(uint8, ETHIF_CODE) EthIf_QueryForWakeupTransceiver(
  uint8 ethIfCtrlIdx);

#endif /* ETHIF_WAKEUP_SUPPORT */

#if ( ETHIF_ENABLE_ZERO_COPY_EXTENSIONS == STD_ON )
/**********************************************************************************************************************
 *  EthIf_IsImplicitBufferReleaseNeeded
 *********************************************************************************************************************/
/*! \brief      Checks if a implicit buffer release is needed
 *  \details    This function checks if a implicit buffer release is needed for the frame received and designeted
 *              for the given EthIf controller and EtherType.
 *  \param[in]  ethIfCtrlIdx    EthIf controller index
 *  \param[in]  etherType       EtherType of the frame
 *  \return     FALSE - buffer must be explicitly released by upper layer
 *  \return     TRUE - buffer must implicitly released by EthIf
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_IsImplicitBufferReleaseNeeded(
  uint8          ethIfCtrlIdx,
  Eth_FrameType  etherType);
#endif /* ETHIF_ENABLE_ZERO_COPY_EXTENSIONS */

#if ( ETHIF_ETHSWT_TIME_STAMP_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_IsOwnerRegisteredForSwtTimeStamping
 *********************************************************************************************************************/
/*! \brief      Checks if the owner is register for switch time stamping
 *  \details    This function checks if the passed owner is registered for switch time stamping information.
 *  \param[in]  ownerIdx  Owner identifier
 *  \return     FALSE - owner hasn't registered for switch time stamping information
 *  \return     TRUE - owner has registered for switch time stamp information
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_IsOwnerRegisteredForSwtTimeStamping(
  uint8 ownerIdx);

/**********************************************************************************************************************
 *  EthIf_StartTxSwtTimeStampInfoProc
 *********************************************************************************************************************/
/*! \brief      Initiates the switch time stamp processing for the passed frame buffer
 *  \details    This function sets up the internal data structures for being able to handle switch time stamping
 *              processing during transmission for the passed frame buffer
 *  \param[in]  ethCtrlIdx    Eth controller the frame buffer is related to
 *  \param[in]  ethIfCtrlIdx  EthIf controller the frame shall be sent on
 *  \param[in]  bufIdx        Frame buffer identifier in context of the Eth/EthIf controller
 *  \param[in]  bufLen        Frame buffer length
 *  \param[in]  dataPtr       Pointer to the payload portion of the frame buffer
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_StartTxSwtTimeStampInfoProc(
        uint8                               ethCtrlIdx,
        uint8                               ethIfCtrlIdx,
        uint8                               bufIdx,
        uint16                              bufLen,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA)  dataPtr);

/**********************************************************************************************************************
 *  EthIf_FinishTxSwtTimeStampInfoProc
 *********************************************************************************************************************/
/*! \brief      Complete the switch time stamp processing for the passed frame buffer
 *  \details    This function completes the switch time stamp processing for the passed frame buffer in context of the
 *              common transmission procedure (EthIf_ProvideTxBuffer()/EthIf_Transmit()). It retains the information
 *              related to the frame in case switch time stamping was requested for the frame, otherwise the
 *              information gets discarded.
 *  \param[in]  ethCtrlIdx     Eth controller the frame buffer is related to
 *  \param[in]  bufIdx         Frame buffer identifier in context of the Eth/EthIf controller
 *  \param[in]  frameOwnerIdx  Owner the frame is related to
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_FinishTxSwtTimeStampInfoProc(
  uint8  ethCtrlIdx,
  uint8  bufIdx,
  uint8  frameOwnerIdx);

/**********************************************************************************************************************
 *  EthIf_InitiateRxSwtTimeStampInfoProc
 *********************************************************************************************************************/
/*! \brief      Initiates the switch time stamp processing for a received frame
 *  \details    This function sets up the internal data structures for being able to handle switch time stamping
 *              processing during reception for the passed frame
 *  \param[in]  ethCtrlIdx          Eth controller the frame is related to
 *  \param[in]  ownerIdx            Owner the frame is related to
 *  \param[in]  ulRxIndicationData  Rx indication data related to the frame
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_InitiateRxSwtTimeStampInfoProc(
          uint8                                                ethCtrlIdx,
          uint8                                                ownerIdx,
  P2CONST(EthIf_UlRxIndiactionDataType, AUTOMATIC, AUTOMATIC)  ulRxIndicationData);
#endif /* ETHIF_ETHSWT_TIME_STAMP_SUPPORT */

#if ( ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_IsOwnerRegisteredForSwtMgmtInfo
 *********************************************************************************************************************/
/*! \brief      Checks if the owner is register for switch frame management information
 *  \details    This function checks if the passed owner is registered for switch frame management information.
 *  \param[in]  ownerIdx  Owner identifier
 *  \return     FALSE - owner hasn't registered for switch frame management information
 *  \return     TRUE - owner has registered for switch frame management information
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_IsOwnerRegisteredForSwtMgmtInfo(
  uint8 ownerIdx);

/**********************************************************************************************************************
 *  EthIf_InitiateRxSwtMgmtInfoProc
 *********************************************************************************************************************/
/*! \brief      Initiates the switch frame management information processing for a received frame
 *  \details    This function sets up the internal data structures for being able to handle switch frame management
 *              information processing during reception for the passed frame
 *  \param[in]  ethCtrlIdx          Eth controller the frame is related to
 *  \param[in]  ownerIdx            Owner the frame is related to
 *  \param[in]  ulRxIndicationData  Rx indication data related to the frame
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_InitiateRxSwtMgmtInfoProc(
          uint8                                                ethCtrlIdx,
          uint8                                                ownerIdx,
  P2CONST(EthIf_UlRxIndiactionDataType, AUTOMATIC, AUTOMATIC)  ulRxIndicationData);
#endif /* ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT */

/**********************************************************************************************************************
 *  EthIf_UpdateTxStats
 *********************************************************************************************************************/
/*! \brief      Updates the tx statistic counters of the EthIf-controller
 *  \param[in]  virtCtrl  EthIf-controller
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_UpdateTxStats(
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl,
          uint16                                      payloadLen);

/**********************************************************************************************************************
 *  EthIf_ClearTxStats
 *********************************************************************************************************************/
/*! \brief      Clears the tx statistic counters of the EthIf-controller
 *  \param[in]  virtCtrl  EthIf-controller
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_ClearTxStats(
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl);

/**********************************************************************************************************************
 *  EthIf_UpdateRxStats
 *********************************************************************************************************************/
/*! \brief      Updates the rx statistic counters of the EthIf-controller
 *  \param[in]  virtCtrl  EthIf-controller
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_UpdateRxStats(
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl,
          uint16                                      payloadLen);

/**********************************************************************************************************************
 *  EthIf_ClearRxStats
 *********************************************************************************************************************/
/*! \brief      Clears the rx statistic counters of the EthIf-controller
 *  \param[in]  virtCtrl  EthIf-controller
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_ClearRxStats(
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
 *  EthIf_IsModeRequestPossible
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_IsModeRequestPossible(
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST)  virtCtrl,
          Eth_ModeType                                 trgtMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean  result;
  uint8    thresholdCnt;

  /* ----- Implementation ----------------------------------------------- */
  result       = TRUE;

  /* #10 Set the pass error criteria for possibility check dependent on the targeted mode */
  thresholdCnt = 0U; /* for trgtMode == ETH_MODE_DOWN the threshold is 0 */

  if( trgtMode == ETH_MODE_ACTIVE )
  {
    thresholdCnt = ETHIF_MAX_HW_ELEM_MODE_CNT;
  }

  /* #20 Check for the possibility to process the mode request */
  if( virtCtrl->EthCtrl->ModeCnt[0] == thresholdCnt )
  {
    result = FALSE;
  }
  else
  {
    uint8_least phyElementIdx;

    for( phyElementIdx = 0; phyElementIdx < virtCtrl->PhyLayer->ModeElementNum; phyElementIdx++ )
    {
      P2CONST(EthIf_AbstractPhyElementType, AUTOMATIC, ETHIF_CONST) phyElement;

      phyElement = &virtCtrl->PhyLayer->PhyLayerModeElements[phyElementIdx];

      /* Is physical layer element a transceiver or a switch port? */
      switch( phyElement->PhyType )
      {
#if ( ETHIF_ETHTRCV_USED == STD_ON )
      case ETHIF_PHYLAYER_ETHTRCV:
        {
          /* PRQA S 310 1 */ /* MD_EthIf_310_PhyLayer */
          P2CONST(EthIf_EthTrcvType, AUTOMATIC, ETHIF_CONST) ethTrcv = ((P2CONST(EthIf_PhyEthTrcvType, AUTOMATIC, ETHIF_CONST))phyElement)->EthTrcv;
          if( ethTrcv->ModeCnt[0] == thresholdCnt )
          {
            result = FALSE;
          }
        }
        break;
#endif /* ETHIF_ETHTRCV_USED */
#if ( ETHIF_ETHSWT_USED == STD_ON )
      case ETHIF_PHYLAYER_ETHSWTPORTGROUP:
        {
          /* PRQA S 310 2 */ /* MD_EthIf_310_PhyLayer */
          P2CONST(EthIf_EthSwtPortGroupType, AUTOMATIC, ETHIF_CONST) ethSwtPortGroup =
              ((P2CONST(EthIf_PhyEthSwtPortGroupType, AUTOMATIC, ETHIF_CONST))phyElement)->EthSwtPortGroup;
          result = EthIf_IsModeRequestPossibleForSwtPortGroup(ethSwtPortGroup, trgtMode);
        }
        break;
#endif /* ETHIF_ETHSWT_USED */
      default:
        /* shouldn't ever occur */
        break;
      }

      if( result == FALSE )
      {
        break;
      }
    }
  }

  return result;
} /* EthIf_IsModeRequestPossible() */

#if ( ETHIF_ETHSWT_USED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_IsModeRequestPossibleForSwtPortGroup
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_IsModeRequestPossibleForSwtPortGroup(
  P2CONST(EthIf_EthSwtPortGroupType, AUTOMATIC, ETHIF_CONST)  ethIfSwtPortGroup,
          Eth_ModeType                                        trgtMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean  result;
  uint8    thresholdCnt;

  /* ----- Implementation ----------------------------------------------- */
  result = TRUE;

  /* #10 Set the pass error criteria for possibility check dependent on the targeted mode */
  thresholdCnt = 0U; /* for trgtMode == ETH_MODE_DOWN the threshold is 0 */

  if( trgtMode == ETH_MODE_ACTIVE )
  {
    thresholdCnt = ETHIF_MAX_HW_ELEM_MODE_CNT;
  }

  /* #20 Check for the possibility to process the mode request */
  if( (ethIfSwtPortGroup->PortGroupSemantic == ETHIF_SWTPORTGROUP_SEMANTIC_CONTROL) ||
      (ethIfSwtPortGroup->PortGroupSemantic == ETHIF_SWTPORTGROUP_SEMANTIC_INDEPENDENT) )
  {
    uint8_least portNum;

    for ( portNum = 0; portNum < ethIfSwtPortGroup->RelatedPortsNum; portNum++ )
    {
      if( ethIfSwtPortGroup->RelatedPorts[portNum]->ModeCnt[0] == thresholdCnt )
      {
        result = FALSE;
        break;
      }
    }
  }
  else
  {
    /* nothing to check for EthIf switch port groups with semantic unequal to control or independent type */
  }

  return result;
} /* EthIf_IsModeRequestPossibleForSwtPortGroup() */
#endif /* ETHIF_ETHSWT_USED */
#endif /* ETHIF_DEV_ERROR_DETECT */

/**********************************************************************************************************************
 *  EthIf_SetHardwareModeActive
 *********************************************************************************************************************/
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
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetHardwareModeActive(
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST)  virtCtrl)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enable Ethernet Controller if no previous ACTIVE request for it was triggered */
  if( virtCtrl->EthCtrl->ModeCnt[0] == 0u )
  {
    /* call only when Ethernet Controller wasn't enabled before */
    /* \trace SPEC-2393587 */
    retVal = virtCtrl->EthCtrl->DrvApi->SetControllerMode(virtCtrl->EthCtrl->CtrlIdx,
                                                          ETH_MODE_ACTIVE);
  }
  else
  {
    /* Ethernet Controller was initialized before by other EthIf controller */
    retVal = E_OK;
  }

  /* #20 If setting the mode for the Ethernet controller was successful */
  if( retVal == E_OK )
  {
    uint8_least phyElementIdx;

    /* #200 For each physical layer element (PHY, Switch-Port) */
    for( phyElementIdx = 0; phyElementIdx < virtCtrl->PhyLayer->ModeElementNum; phyElementIdx++ )
    {
      P2CONST(EthIf_AbstractPhyElementType, AUTOMATIC, ETHIF_CONST) phyElement;

      phyElement = &virtCtrl->PhyLayer->PhyLayerModeElements[phyElementIdx];
      retVal     = E_NOT_OK;

      /* #2000 Select type of physical layer element */
      switch( phyElement->PhyType )
      {
#if ( ETHIF_ETHTRCV_USED == STD_ON )
      case ETHIF_PHYLAYER_ETHTRCV:
        {
          /* #20000 Element is a PHY -> Activate PHY if no previous ACTIVE request was triggered for it */
          /* PRQA S 310 1 */ /* MD_EthIf_310_PhyLayer */
          P2CONST(EthIf_EthTrcvType, AUTOMATIC, ETHIF_CONST) ethTrcv = ((P2CONST(EthIf_PhyEthTrcvType, AUTOMATIC, ETHIF_CONST))phyElement)->EthTrcv;
          if( ethTrcv->ModeCnt[0] == 0u )
          {
            /* Check if API is available */
            if( ethTrcv->DrvApi->SetTransceiverMode != NULL_PTR )
            {
              retVal = ethTrcv->DrvApi->SetTransceiverMode(ethTrcv->TrcvIdx,
                                                           ETHTRCV_MODE_ACTIVE);
            }
            else
            {
              /* EthTrcv driver doesn't provide the API, so for EthIf operation was successful
               * -> correct operation mode must be handled by driver itself e.g. by having correct mode due to initial configuration (always on PHY) */
              retVal = E_OK;
            }
          }
          else
          {
            retVal = E_OK;
          }
        }
        break;
#endif /* ETHIF_ETHTRCV_USED */
#if ( ETHIF_ETHSWT_USED == STD_ON )
      case ETHIF_PHYLAYER_ETHSWTPORTGROUP:
        {
          /* #20010 Element is a Switch-Port -> Activate Switch-Port if no previous ACTIVE request was triggered for it */
          /* PRQA S 310 2 */ /* MD_EthIf_310_PhyLayer */
          P2CONST(EthIf_EthSwtPortGroupType, AUTOMATIC, ETHIF_CONST) ethSwtPortGroup =
              ((P2CONST(EthIf_PhyEthSwtPortGroupType, AUTOMATIC, ETHIF_CONST))phyElement)->EthSwtPortGroup;
          retVal = EthIf_SetSwitchPortGroupMode(ethSwtPortGroup, ETHTRCV_MODE_ACTIVE);
        }
        break;
#endif /* ETHIF_ETHSWT_USED */
      default:
        /* shouldn't ever occur */
        break;
      }

      /* only proceed if previous call was successful */
      if( retVal == E_NOT_OK )
      {
        break;
      }
    }
  }

  /* #30 If setting the mode for the physical layer was successful */
  if( retVal == E_OK )
  {
    uint8_least phyElementIdx;

    /* #300 Set EthIf controller in active state */
    EthIf_CfgAccess_EthIfCtrlMode(virtCtrl->EthIfCtrlIdx) = ETH_MODE_ACTIVE;

    /* #310 Update the amount of ACTIVE requests for the Ethernet Controller */
    virtCtrl->EthCtrl->ModeCnt[0]++;

    /* #320 Update the amount of ACTIVE requests for the physical layer elements (PHYs, Switch-Ports) */
    for( phyElementIdx = 0; phyElementIdx < virtCtrl->PhyLayer->ModeElementNum; phyElementIdx++ )
    {
      P2CONST(EthIf_AbstractPhyElementType, AUTOMATIC, ETHIF_CONST) phyElement;

      phyElement = &virtCtrl->PhyLayer->PhyLayerModeElements[phyElementIdx];

      /* Is physical layer element a transceiver or a switch port? */
      switch( phyElement->PhyType )
      {
#if ( ETHIF_ETHTRCV_USED == STD_ON )
      case ETHIF_PHYLAYER_ETHTRCV:
        {
          /* PRQA S 310 1 */ /* MD_EthIf_310_PhyLayer */
          P2CONST(EthIf_EthTrcvType, AUTOMATIC, ETHIF_CONST) ethTrcv = ((P2CONST(EthIf_PhyEthTrcvType, AUTOMATIC, ETHIF_CONST))phyElement)->EthTrcv;
          ethTrcv->ModeCnt[0]++;
        }
        break;
#endif /* ETHIF_ETHTRCV_USED */
#if ( ETHIF_ETHSWT_USED == STD_ON )
      case ETHIF_PHYLAYER_ETHSWTPORTGROUP:
        {
          /* PRQA S 310 2 */ /* MD_EthIf_310_PhyLayer */
          P2CONST(EthIf_EthSwtPortGroupType, AUTOMATIC, ETHIF_CONST) ethSwtPortGroup =
              ((P2CONST(EthIf_PhyEthSwtPortGroupType, AUTOMATIC, ETHIF_CONST))phyElement)->EthSwtPortGroup;
          EthIf_AdjustSwitchPortGroupModeCount(ethSwtPortGroup, ETHTRCV_MODE_ACTIVE);
        }
        break;
#endif /* ETHIF_ETHSWT_USED */
      default:
        /* shouldn't ever occur */
        break;
      }
    }
  }

  return retVal;
} /* EthIf_SetHardwareModeActive() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  EthIf_SetHardwareModeDown
 *********************************************************************************************************************/
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
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetHardwareModeDown(
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST)  virtCtrl)
{
  /* ----- Local Variables ---------------------------------------------- */
    Std_ReturnType  retVal;

  /* ----- Implementation ----------------------------------------------- */
    /* #10 Disable Ethernet Controller if no ACTIVE request is pending anymore */
  if( virtCtrl->EthCtrl->ModeCnt[0] == 1u )
  {
    /* call only when one active mode for the Ethernet Controller is pending */
    /* \trace SPEC-2393587 */
    retVal = virtCtrl->EthCtrl->DrvApi->SetControllerMode(virtCtrl->EthCtrl->CtrlIdx,
                                                          ETH_MODE_DOWN);
  }
  else
  {
    /* There are still active mode requests pending on other EthIf controllers */
    retVal = E_OK;
  }

  /* #20 If setting the mode for the Ethernet controller was successful */
  if( retVal == E_OK )
  {
    uint8_least phyElementIdx;

    /* #200 For each physical layer element (PHY, Switch-Port) */
    for( phyElementIdx = 0; phyElementIdx < virtCtrl->PhyLayer->ModeElementNum; phyElementIdx++ )
    {
      P2CONST(EthIf_AbstractPhyElementType, AUTOMATIC, ETHIF_CONST) phyElement;

      phyElement = &virtCtrl->PhyLayer->PhyLayerModeElements[phyElementIdx];
      retVal = E_NOT_OK;

      /* #2000 Select type of physical layer element */
      switch( phyElement->PhyType )
      {
#if ( ETHIF_ETHTRCV_USED == STD_ON )
      case ETHIF_PHYLAYER_ETHTRCV:
        {
          /* #20000 Element is a PHY -> Disable PHY if no ACTIVE request is pending anymore */
          /* PRQA S 310 1 */ /* MD_EthIf_310_PhyLayer */
          P2CONST(EthIf_EthTrcvType, AUTOMATIC, ETHIF_CONST) ethTrcv = ((P2CONST(EthIf_PhyEthTrcvType, AUTOMATIC, ETHIF_CONST))phyElement)->EthTrcv;
          if( ethTrcv->ModeCnt[0] == 1u )
          {
            /* Check if API is available */
            if( ethTrcv->DrvApi->SetTransceiverMode != NULL_PTR )
            {
              /* \trace SPEC-2393603 */
              retVal = ethTrcv->DrvApi->SetTransceiverMode(ethTrcv->TrcvIdx,
                                                           ETHTRCV_MODE_DOWN);
            }
            else
            {
              /* EthTrcv driver doesn't provide the API, so for EthIf operation was successful
               * -> correct operation mode must be handled by driver itself e.g. by having correct mode due to initial configuration (always on PHY) */
              retVal = E_OK;
            }
          }
          else
          {
            retVal = E_OK;
          }
        }
        break;
#endif /* ETHIF_ETHTRCV_USED */
#if ( ETHIF_ETHSWT_USED == STD_ON )
      case ETHIF_PHYLAYER_ETHSWTPORTGROUP:
        {
          /* #20010 Element is a Switch-Port -> Disable Switch-Port if no previous ACTIVE request is pending anymore */
          /* PRQA S 310 2 */ /* MD_EthIf_310_PhyLayer */
          P2CONST(EthIf_EthSwtPortGroupType, AUTOMATIC, ETHIF_CONST) ethSwtPortGroup =
              ((P2CONST(EthIf_PhyEthSwtPortGroupType, AUTOMATIC, ETHIF_CONST))phyElement)->EthSwtPortGroup;
          retVal = EthIf_SetSwitchPortGroupMode(ethSwtPortGroup, ETHTRCV_MODE_DOWN);
        }
        break;
#endif /* ETHIF_ETHSWT_USED */
      default:
        /* shouldn't ever occur */
        break;
      }

      /* only proceed if previous call was successful */
      if( retVal == E_NOT_OK )
      {
        break;
      }
    }
  }

  /* #30 If setting the mode for the physical layer was successful */
  if( retVal == E_OK )
  {
    uint8_least phyElementIdx;

    /* #300 Set EthIf controller mode to DOWN */
    EthIf_CfgAccess_EthIfCtrlMode(virtCtrl->EthIfCtrlIdx)      = ETH_MODE_DOWN;
    EthIf_CfgAccess_EthIfCtrlInitMode(virtCtrl->EthIfCtrlIdx)  = ETHIF_NOT_INITIALIZED;

    /* #310 Set EthIf controller link state to DOWN */
    if( EthIf_CfgAccess_EthIfCtrlLinkState(virtCtrl->EthIfCtrlIdx) == ETHIF_ETHIFCTRL_LINK_STATE_LINK )
    {
      EthIf_CfgAccess_EthIfCtrlLinkState(virtCtrl->EthIfCtrlIdx) = ETHIF_ETHIFCTRL_LINK_STATE_LINK_CHANGE_DOWN;
    }

    /* #320 Update the amount of ACTIVE requests for the Ethernet Controller */
    virtCtrl->EthCtrl->ModeCnt[0]--;
    virtCtrl->EthCtrl->InitCnt[0]--;

    /* #330 Update the amount of ACTIVE requests for the pyhsical layer elements (PHYs, Switch-Ports) */
    for( phyElementIdx = 0; phyElementIdx < virtCtrl->PhyLayer->ModeElementNum; phyElementIdx++ )
    {
      P2CONST(EthIf_AbstractPhyElementType, AUTOMATIC, ETHIF_CONST) phyElement;

      phyElement = &virtCtrl->PhyLayer->PhyLayerModeElements[phyElementIdx];

      /* Is physical layer element a transceiver or a switch port? */
      switch( phyElement->PhyType )
      {
#if ( ETHIF_ETHTRCV_USED == STD_ON )
      case ETHIF_PHYLAYER_ETHTRCV:
        {
          /* PRQA S 310 1 */ /* MD_EthIf_310_PhyLayer */
          P2CONST(EthIf_EthTrcvType, AUTOMATIC, ETHIF_CONST) ethTrcv = ((P2CONST(EthIf_PhyEthTrcvType, AUTOMATIC, ETHIF_CONST))phyElement)->EthTrcv;
          ethTrcv->ModeCnt[0]--;
        }
        break;
#endif /* ETHIF_ETHTRCV_USED */
#if ( ETHIF_ETHSWT_USED == STD_ON )
      case ETHIF_PHYLAYER_ETHSWTPORTGROUP:
        {
          /* PRQA S 310 2 */ /* MD_EthIf_310_PhyLayer */
          P2CONST(EthIf_EthSwtPortGroupType, AUTOMATIC, ETHIF_CONST) ethSwtPortGroup =
              ((P2CONST(EthIf_PhyEthSwtPortGroupType, AUTOMATIC, ETHIF_CONST))phyElement)->EthSwtPortGroup;
          EthIf_AdjustSwitchPortGroupModeCount(ethSwtPortGroup, ETHTRCV_MODE_DOWN);
        }
        break;
#endif /* ETHIF_ETHSWT_USED */
      default:
        /* shouldn't ever occur */
        break;
      }
    }

    /* Decrease the init count for every physical layer element */
    for( phyElementIdx = 0; phyElementIdx < virtCtrl->PhyLayer->InitElementNum; phyElementIdx++ )
    {
      P2CONST(EthIf_AbstractPhyElementType, AUTOMATIC, ETHIF_CONST) phyElement;

      phyElement = &virtCtrl->PhyLayer->PhyLayerInitElements[phyElementIdx];

      /* Is physical layer element a transceiver or a switch? */
      switch( phyElement->PhyType )
      {
#if ( ETHIF_ETHTRCV_USED == STD_ON )
      case ETHIF_PHYLAYER_ETHTRCV:
        {
          /* PRQA S 310 1 */ /* MD_EthIf_310_PhyLayer */
          P2CONST(EthIf_EthTrcvType, AUTOMATIC, ETHIF_CONST) ethTrcv = ((P2CONST(EthIf_PhyEthTrcvType, AUTOMATIC, ETHIF_CONST))phyElement)->EthTrcv;
          ethTrcv->InitCnt[0]--;
        }
        break;
#endif /* ETHIF_ETHTRCV_USED */
#if ( ETHIF_ETHSWT_USED == STD_ON )
      case ETHIF_PHYLAYER_ETHSWT:
        {
          /* PRQA S 310 1 */ /* MD_EthIf_310_PhyLayer */
          P2CONST(EthIf_EthSwtType, AUTOMATIC, ETHIF_CONST) ethSwt = ((P2CONST(EthIf_PhyEthSwtType, AUTOMATIC, ETHIF_CONST))phyElement)->EthSwt;
          ethSwt->InitCnt[0]--;
        }
        break;
#endif /* ETHIF_ETHSWT_USED */
      default:
        /* shouldn't ever occur */
        break;
      }
    }
  }

  return retVal;
} /* EthIf_SetHardwareModeDown() */ /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

#if ( ETHIF_ETHSWT_USED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_SetSwitchPortGroupMode
 *********************************************************************************************************************/
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
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetSwitchPortGroupMode(
  P2CONST(EthIf_EthSwtPortGroupType, AUTOMATIC, ETHIF_CONST)  ethSwtPortGroup,
          EthTrcv_ModeType                                    portMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_NOT_OK;
  uint8_least portNum;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if ethSwtPortGroup has semantic type link info */
  if ( ethSwtPortGroup->PortGroupSemantic == ETHIF_SWTPORTGROUP_SEMANTIC_LINK_INFO )
  {
    retVal = E_OK;
  }
  /* #20 Check if ethSwtPortGroup has semantic type control or independent */
  else if ( (ethSwtPortGroup->PortGroupSemantic == ETHIF_SWTPORTGROUP_SEMANTIC_CONTROL) ||
            (ethSwtPortGroup->PortGroupSemantic == ETHIF_SWTPORTGROUP_SEMANTIC_INDEPENDENT) )
  {
    /* #200 Check if action for ETHTRCV_MODE_ACTIVE should be done */
    if ( portMode == ETHTRCV_MODE_ACTIVE )
    {
      /* #2000 Switch port mode to ETHTRCV_MODE_ACTIVE for each still inactive port of the ethSwtPortGroup */
      for ( portNum = 0; portNum < ethSwtPortGroup->RelatedPortsNum; portNum++ )
      {
        if( ethSwtPortGroup->RelatedPorts[portNum]->ModeCnt[0] == 0u )
        {
          retVal = ethSwtPortGroup->RelatedPorts[portNum]->EthSwt->DrvApi->SetSwitchPortMode(
                      ethSwtPortGroup->RelatedPorts[portNum]->EthSwt->SwitchIdx,
                      ethSwtPortGroup->RelatedPorts[portNum]->PortIdx,
                      ETHTRCV_MODE_ACTIVE);
        }
        else
        {
          retVal = E_OK;
        }

        if ( retVal != E_OK )
        {
          break;
        }
      }

      /* #2010 If switching of port mode was successful and port group semantic is independent set port group mode to ACTIVE */
      if ( (retVal == E_OK) &&
           (ethSwtPortGroup->PortGroupSemantic == ETHIF_SWTPORTGROUP_SEMANTIC_INDEPENDENT) )
      {
        EthIf_CfgAccess_EthIfSwtPortGroupMode(ethSwtPortGroup->PortGroupIdx) = ETHTRCV_MODE_ACTIVE;
      }
    }
    /* #300 Check if action for ETHTRCV_MODE_DOWN should be done */
    else if ( portMode == ETHTRCV_MODE_DOWN )
    {
      /* #3000 Switch port mode to ETHTRCV_MODE_DOWN for each still active port of the ethSwtPortGroup */
      for ( portNum = 0; portNum < ethSwtPortGroup->RelatedPortsNum; portNum++ )
      {
        if( ethSwtPortGroup->RelatedPorts[portNum]->ModeCnt[0] == 1u )
        {
          retVal = ethSwtPortGroup->RelatedPorts[portNum]->EthSwt->DrvApi->SetSwitchPortMode(
                      ethSwtPortGroup->RelatedPorts[portNum]->EthSwt->SwitchIdx,
                      ethSwtPortGroup->RelatedPorts[portNum]->PortIdx,
                      ETHTRCV_MODE_DOWN);
        }
        else
        {
          retVal = E_OK;
        }

        if ( retVal != E_OK )
        {
          break;
        }
      }

      /* #3010 If switching of port mode was successful and port group semantic is independent set port group mode to DOWN */
      if ( (retVal == E_OK) &&
           (ethSwtPortGroup->PortGroupSemantic == ETHIF_SWTPORTGROUP_SEMANTIC_INDEPENDENT) )
      {
        EthIf_CfgAccess_EthIfSwtPortGroupMode(ethSwtPortGroup->PortGroupIdx) = ETHTRCV_MODE_DOWN;
        if( EthIf_CfgAccess_EthIfSwtPortGroupLinkState(ethSwtPortGroup->PortGroupIdx)
                == ETHIF_ETHIFSWTPORTGROUP_LINK_STATE_LINK )
        {
          EthIf_CfgAccess_EthIfSwtPortGroupLinkState(ethSwtPortGroup->PortGroupIdx) =
              ETHIF_ETHIFSWTPORTGROUP_LINK_STATE_LINK_CHANGE_DOWN;
        }
      }
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
} /* EthIf_SetSwitchPortGroupMode() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  EthIf_AdjustSwitchPortGroupModeCount
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_AdjustSwitchPortGroupModeCount(
  P2CONST(EthIf_EthSwtPortGroupType, AUTOMATIC, ETHIF_CONST)  ethSwtPortGroup,
          EthTrcv_ModeType                                    portMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least portNum;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if EthIf switch port group is of semantic type control or independent */
  if ( (ethSwtPortGroup->PortGroupSemantic == ETHIF_SWTPORTGROUP_SEMANTIC_CONTROL) ||
       (ethSwtPortGroup->PortGroupSemantic == ETHIF_SWTPORTGROUP_SEMANTIC_INDEPENDENT) )
  {
    /* #110 Check if action for ETHTRCV_MODE_ACTIVE should be done */
    if ( portMode == ETHTRCV_MODE_ACTIVE )
    {
      /* #1100 Increment mode counter of each port in the ethSwtPortGroup */
      for ( portNum = 0; portNum < ethSwtPortGroup->RelatedPortsNum; portNum++ )
      {
        ethSwtPortGroup->RelatedPorts[portNum]->ModeCnt[0]++;
      }
    }
    /* #120 Check if action for ETHTRCV_MODE_DOWN should be done */
    else if ( portMode == ETHTRCV_MODE_DOWN )
    {
      /* #1200 Decrement mode counter of each port in the ethSwtPortGroup */
      for ( portNum = 0; portNum < ethSwtPortGroup->RelatedPortsNum; portNum++ )
      {
        ethSwtPortGroup->RelatedPorts[portNum]->ModeCnt[0]--;
      }
    }
    else
    {
      /* MISRA compliance */
    }
  }
  else
  {
    /* No adjustment if EthIf switch port group is not of semantic type control or independent */
  }
} /* EthIf_AdjustSwitchPortGroupModeCount() */
#endif /* ETHIF_ETHSWT_USED */

/**********************************************************************************************************************
 *  EthIf_ProcessLinkStateChange
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_ProcessLinkStateChange(
  uint8 ethIfCtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if a link state change to link active occurred on the EthIf Controller */
  if( EthIf_CfgAccess_EthIfCtrlLinkState(ethIfCtrlIdx) == ETHIF_ETHIFCTRL_LINK_STATE_LINK_CHANGE_UP )
  {
    /* #100 Issue a link up for the EthIf Controller */
    EthIf_ReportLinkStateChange(ethIfCtrlIdx, ETHTRCV_LINK_STATE_ACTIVE);
    EthIf_CfgAccess_EthIfCtrlLinkState(ethIfCtrlIdx) = ETHIF_ETHIFCTRL_LINK_STATE_LINK;
  }
  /* #20 Check if a link state change to link down occurred on the EthIf Controller */
  else if( EthIf_CfgAccess_EthIfCtrlLinkState(ethIfCtrlIdx) == ETHIF_ETHIFCTRL_LINK_STATE_LINK_CHANGE_DOWN )
  {
    /* #200 Issue a link down for the EthIf Controller */
    EthIf_ReportLinkStateChange(ethIfCtrlIdx, ETHTRCV_LINK_STATE_DOWN);
    EthIf_CfgAccess_EthIfCtrlLinkState(ethIfCtrlIdx) = ETHIF_ETHIFCTRL_LINK_STATE_NO_LINK;
  }
  else
  {
    /* no link change occurred */
  }
} /* EthIf_ProcessLinkStateChange() */

#if ( ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_ProcessSwtPortGroupLinkStateChange
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_ProcessSwtPortGroupLinkStateChange(
  uint8 ethIfSwtPortGroupIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if a link state change to link active occurred on the EthIf switch port group */
  if( EthIf_CfgAccess_EthIfSwtPortGroupLinkState(ethIfSwtPortGroupIdx) ==
        ETHIF_ETHIFSWTPORTGROUP_LINK_STATE_LINK_CHANGE_UP )
  {
    /* #100 Issue a link up for the EthIf switch port group */
    EthIf_CfgAccess_EthIfSwtPortGroupLinkState(ethIfSwtPortGroupIdx) = ETHIF_ETHIFSWTPORTGROUP_LINK_STATE_LINK;
    BswM_EthIf_PortGroupLinkStateChg(ethIfSwtPortGroupIdx, ETHTRCV_LINK_STATE_ACTIVE);
  }
  /* #20 Check if a link state change to link down occurred on the EthIf switch port group */
  else if( EthIf_CfgAccess_EthIfSwtPortGroupLinkState(ethIfSwtPortGroupIdx) ==
             ETHIF_ETHIFSWTPORTGROUP_LINK_STATE_LINK_CHANGE_DOWN )
  {
    /* #200 Issue a link down for the EthIf switch port group */
    EthIf_CfgAccess_EthIfSwtPortGroupLinkState(ethIfSwtPortGroupIdx) = ETHIF_ETHIFSWTPORTGROUP_LINK_STATE_NO_LINK;
    BswM_EthIf_PortGroupLinkStateChg(ethIfSwtPortGroupIdx, ETHTRCV_LINK_STATE_DOWN);
  }
  else
  {
    /* no link change occurred */
  }
} /* EthIf_ProcessSwtPortGroupLinkStateChange() */
#endif /* ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT */

/**********************************************************************************************************************
 *  EthIf_ProcessLinkStateChangeForAllEthIfCtrls
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_ProcessLinkStateChangeForAllEthIfCtrls( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least ethIfCtrlIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check each EthIf controller for a link change */
  /* \trace SPEC-2393938 */
  for( ethIfCtrlIdx = 0; ethIfCtrlIdx < EthIf_CfgAccess_EthIfCtrlNum(); ethIfCtrlIdx++ )
  {
    EthIf_ProcessLinkStateChange((uint8)ethIfCtrlIdx);
  }
} /* EthIf_ProcessLinkStateChangeForAllEthIfCtrls() */

#if ( ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_ProcessAllEthIfSwtPortGroupLinkStateChanges
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_ProcessAllEthIfSwtPortGroupLinkStateChanges( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least ethIfSwtPortGroupIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check each EthIf switch port group with semantic type independent for a link change */
  for( ethIfSwtPortGroupIdx = 0;
       ethIfSwtPortGroupIdx < EthIf_CfgAccess_EthIfSwtPortGroupNum();
       ethIfSwtPortGroupIdx++ )
  {
    if ( EthIf_SwtPortGroupSummary[ethIfSwtPortGroupIdx]->PortGroupSemantic ==
          ETHIF_SWTPORTGROUP_SEMANTIC_INDEPENDENT )
    {
      EthIf_ProcessSwtPortGroupLinkStateChange((uint8)ethIfSwtPortGroupIdx);
    }
  }
} /* EthIf_ProcessAllEthIfSwtPortGroupLinkStateChanges() */
#endif /* ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT */

/**********************************************************************************************************************
 *  EthIf_ReportLinkStateChange
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_ReportLinkStateChange(
  uint8                  ethIfCtrlIdx,
  EthTrcv_LinkStateType  linkState)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least linkChgCbkIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Report link state change to EthSM if state changes to ACTIVE */
  if( linkState == ETHTRCV_LINK_STATE_ACTIVE )
  {
    EthSM_VTrcvLinkStateChg(ethIfCtrlIdx, linkState);
  }

  /* #20 Report link state change to users */
  for( linkChgCbkIdx = 0; linkChgCbkIdx < EthIf_CfgAccess_UserLinkChgCbkInfo().UserLinkChgCbkNum; linkChgCbkIdx++)
  {
    EthIf_CfgAccess_UserLinkChgCbkInfo().UserLinkChgCbks[linkChgCbkIdx](ethIfCtrlIdx, linkState);
  }

  /* #30 Report link state change to EthSM if state changes to DOWN */
  if( linkState == ETHTRCV_LINK_STATE_DOWN )
  {
    EthSM_VTrcvLinkStateChg(ethIfCtrlIdx, linkState);
  }
} /* EthIf_ReportLinkStateChange() */

#if ( ETHIF_ETHTRCV_USED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_PollTrcvLinks
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_PollTrcvLinks( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(EthIf_EthTrcvType, AUTOMATIC, ETHIF_CONST) trcv;
          uint8_least                                ethIfTrcvIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 For each transceiver */
  for( ethIfTrcvIdx = 0U; ethIfTrcvIdx < EthIf_CfgAccess_EthTrcvNum(); ethIfTrcvIdx++ )
  {
    trcv = EthIf_CfgAccess_EthTrcvSummary(ethIfTrcvIdx);

    /* #100 If the transceiver is in state ACTIVE */
    if( (*trcv->ModeCnt) > 0U )
    {
      if( trcv->DrvApi->GetLinkState != NULL_PTR )
      {
        EthTrcv_LinkStateType linkState;

        /* #1000 Retrieve the link and if link state is ACTIVE map it to the related EthIf controllers */
        if( trcv->DrvApi->GetLinkState(trcv->TrcvIdx,
                                       &linkState) == E_OK )
        {
          if( linkState == ETHTRCV_LINK_STATE_ACTIVE )
          {
            uint8_least trcvRelEthIfSummaryIdx;

            for( trcvRelEthIfSummaryIdx = 0U; trcvRelEthIfSummaryIdx < trcv->RelatedEthIfCtrlNum; trcvRelEthIfSummaryIdx++ )
            {
              EthIf_CfgAccess_EthIfCtrlLinkCnts(trcv->RelatedEthIfCtrls[trcvRelEthIfSummaryIdx])++;
            }
          }
        }
      }
    }
  }
} /* EthIf_PollTrcvLinks() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* ETHIF_ETHTRCV_USED */

#if ( ETHIF_ETHSWT_USED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_PollPortLinks
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_PollPortLinks( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(EthIf_EthSwtPortType, AUTOMATIC, ETHIF_CONST) swtPort;
          uint8_least                                   ethIfPortIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 For each switch port */
  for( ethIfPortIdx = 0U; ethIfPortIdx < EthIf_CfgAccess_EthSwtPortNum(); ethIfPortIdx++ )
  {
    swtPort = EthIf_CfgAccess_EthSwtPortSummary(ethIfPortIdx);

    /* #If the switch port is in state ACTIVE */
    if( (*swtPort->ModeCnt) > 0U )
    {
      EthTrcv_LinkStateType linkState;
        
      /* #1000 Retrieve the link and if link state is ACTIVE map it to the related EthIf switch port groups */
      if( swtPort->EthSwt->DrvApi->GetLinkState(swtPort->EthSwt->SwitchIdx,
                                                swtPort->PortIdx,
                                                &linkState) == E_OK )
      {
        if( linkState == ETHTRCV_LINK_STATE_ACTIVE )
        {
          uint8_least portRelEthIfSwtPortGroupSummaryIdx;

          for( portRelEthIfSwtPortGroupSummaryIdx = 0U;
               portRelEthIfSwtPortGroupSummaryIdx < swtPort->RelatedEthIfPortGroupNum;
               portRelEthIfSwtPortGroupSummaryIdx++ )
          {
            EthIf_CfgAccess_EthIfSwtPortGroupLinkCnts(
                swtPort->RelatedEthIfPortGroups[portRelEthIfSwtPortGroupSummaryIdx])++;
          }
        }
      }
    }
  }
} /* EthIf_PollPortLinks() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  EthIf_PollPortGroupLinks
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_PollPortGroupLinks( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least ethIfSwtPortGroupIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 For each switch port group */
  for( ethIfSwtPortGroupIdx = 0U; ethIfSwtPortGroupIdx < EthIf_CfgAccess_EthIfSwtPortGroupNum();
       ethIfSwtPortGroupIdx++ )
  {
    if ( (EthIf_CfgAccess_EthIfSwtPortGroupSummary(ethIfSwtPortGroupIdx)->PortGroupSemantic ==
            ETHIF_SWTPORTGROUP_SEMANTIC_CONTROL) ||
         (EthIf_CfgAccess_EthIfSwtPortGroupSummary(ethIfSwtPortGroupIdx)->PortGroupSemantic ==
            ETHIF_SWTPORTGROUP_SEMANTIC_LINK_INFO) )
    {
      if( EthIf_CfgAccess_EthIfSwtPortGroupLinkCnts(ethIfSwtPortGroupIdx) >=
          EthIf_CfgAccess_EthIfSwtPortGroupLinkAggregationThreshold(ethIfSwtPortGroupIdx) )
      {
        uint8_least portGroupRelEthIfSummaryIdx;

        for( portGroupRelEthIfSummaryIdx = 0U;
             portGroupRelEthIfSummaryIdx <
               EthIf_CfgAccess_EthIfSwtPortGroupSummary(ethIfSwtPortGroupIdx)->RelatedEthIfCtrlsNum;
             portGroupRelEthIfSummaryIdx++ )
        {
          EthIf_CfgAccess_EthIfCtrlLinkCnts(EthIf_CfgAccess_EthIfSwtPortGroupSummary(ethIfSwtPortGroupIdx)
              ->RelatedEthIfCtrls[portGroupRelEthIfSummaryIdx])++;
        }
      }
    }
    else
    {
    }
  }
} /* EthIf_PollPortGroupLinks() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* ETHIF_ETHSWT_USED */

/**********************************************************************************************************************
 *  EthIf_DetectLinkStateChange
 *********************************************************************************************************************/
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
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_DetectLinkStateChange(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least  loopIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize link counts of the EthIf controllers */
  for( loopIdx = 0; loopIdx < EthIf_CfgAccess_EthIfCtrlNum(); loopIdx++ )
  {
    EthIf_CfgAccess_EthIfCtrlLinkCnts(loopIdx) = 0U;
  }
#if ( ETHIF_ETHSWT_USED == STD_ON )
  for( loopIdx = 0; loopIdx < EthIf_CfgAccess_EthIfSwtPortGroupNum(); loopIdx++ )
  {
    EthIf_CfgAccess_EthIfSwtPortGroupLinkCnts(loopIdx) = 0U;
  }
#endif /* ETHIF_ETHSWT_USED */

  /* #20 Retrieve the link states of all physical layer elements */
#if ( ETHIF_ETHTRCV_USED == STD_ON )
  EthIf_PollTrcvLinks();
#endif /* ETHIF_ETHTRCV_USED */
#if ( ETHIF_ETHSWT_USED == STD_ON )
  EthIf_PollPortLinks();
  EthIf_PollPortGroupLinks();
#endif /* ETHIF_ETHSWT_USED */

  /* #30 For each EthIf controller */
  for( loopIdx = 0; loopIdx < EthIf_CfgAccess_EthIfCtrlNum(); loopIdx++ )
  {
    /* #300 If EthIf controller is ACTIVE */
    if( EthIf_CfgAccess_EthIfCtrlMode(loopIdx) == ETH_MODE_ACTIVE )
    {
      /* #3000 If link is established for the needed amount of phy layer elements */
      if( EthIf_CfgAccess_EthIfCtrlLinkCnts(loopIdx) >= EthIf_CfgAccess_LinkAggregationThreshold(loopIdx) )
      {
        /* amount of phy elements needed for a link is over or equal to the threshold */

        /* #30000 Issue a link state to UP if EthIf controller hasn't established a link before */
        if( EthIf_CfgAccess_EthIfCtrlLinkState(loopIdx) != ETHIF_ETHIFCTRL_LINK_STATE_LINK )
        {
          /* issue a link up on the EthIf controller */
          EthIf_CfgAccess_EthIfCtrlLinkState(loopIdx) = ETHIF_ETHIFCTRL_LINK_STATE_LINK_CHANGE_UP;
        }
      }
      /* #3010 Otherwise (if link isn't established on the needed amount of phy layer elements) */
      else
      {
        /* amount of phy elements needed for a link is under the threshold */

        /* #30100 Issue a link state to DOWN if EthIf controller has established a link before */
        if( EthIf_CfgAccess_EthIfCtrlLinkState(loopIdx) != ETHIF_ETHIFCTRL_LINK_STATE_NO_LINK )
        {
          /* issue a link down on the EthIf controller */
          EthIf_CfgAccess_EthIfCtrlLinkState(loopIdx) = ETHIF_ETHIFCTRL_LINK_STATE_LINK_CHANGE_DOWN;
        }
      }
    }
  }

#if ( ETHIF_ETHSWT_USED == STD_ON )
  /* #40 For each EthIf switch port group */
  for( loopIdx = 0; loopIdx < EthIf_CfgAccess_EthIfSwtPortGroupNum(); loopIdx++ )
  {
    /* #400 If EthIf switch port group is active and of semantic type independent */
    if ( (EthIf_CfgAccess_EthIfSwtPortGroupMode(loopIdx) == ETHTRCV_MODE_ACTIVE) &&
         (EthIf_CfgAccess_EthIfSwtPortGroupSummary(loopIdx)->PortGroupSemantic ==
                ETHIF_SWTPORTGROUP_SEMANTIC_INDEPENDENT) )
    {
      /* #4000 If link is established for the needed amount of phy layer elements */
      if( EthIf_CfgAccess_EthIfSwtPortGroupLinkCnts(loopIdx) >=
          EthIf_CfgAccess_EthIfSwtPortGroupLinkAggregationThreshold(loopIdx) )
      {
        /* amount of phy elements needed for a link is over or equal to the threshold */

        /* #40000 Issue a link state to UP if EthIf switch port group hasn't established a link before */
        if( EthIf_CfgAccess_EthIfSwtPortGroupLinkState(loopIdx) ==
              ETHIF_ETHIFSWTPORTGROUP_LINK_STATE_NO_LINK )
        {
          /* issue a link up on the EthIf switch port group */
          EthIf_CfgAccess_EthIfSwtPortGroupLinkState(loopIdx) =
              ETHIF_ETHIFSWTPORTGROUP_LINK_STATE_LINK_CHANGE_UP;
        }
      }
      /* #4010 Otherwise (if link isn't established on the needed amount of phy layer elements) */
      else
      {
        /* amount of phy elements needed for a link is under the threshold */

        /* #40100 Issue a link state to DOWN if EthIf switch port group has established a link before */
        if( EthIf_CfgAccess_EthIfSwtPortGroupLinkState(loopIdx) ==
              ETHIF_ETHIFSWTPORTGROUP_LINK_STATE_LINK )
        {
          /* issue a link down on the EthIf switch port group */
          EthIf_CfgAccess_EthIfSwtPortGroupLinkState(loopIdx) =
              ETHIF_ETHIFSWTPORTGROUP_LINK_STATE_LINK_CHANGE_DOWN;
        }
      }
    }
    else
    {
    }
  }
#endif /* ETHIF_ETHSWT_USED */
} /* EthIf_DetectLinkStateChange() */ /* PRQA S 6010,6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */

/**********************************************************************************************************************
 *  EthIf_QueryForEthIfController
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(uint8, ETHIF_CODE) EthIf_QueryForEthIfController(
  uint8    ethCtrlIdx,
  uint16   vlanId)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST)  virtCtrl;
          uint8                                        queriedEthIfCtrlIdx;

  /* ----- Implementation ----------------------------------------------- */
  queriedEthIfCtrlIdx = EthIf_CfgAccess_InvalidEthIfCtrlIdx();

  /* #10 Check if controller represents un-tagged data path */
  if( vlanId == ETHIF_INV_VLAN_ID )
  {
    /* #100 If un-tagged data path EthIf controller exists it is placed at first position of the EthIf controller
     *      group for the Eth controller queried for -> get the element and check if it is the one searched for */
    virtCtrl = &EthIf_CfgAccess_VirtCtrlBase4EthCtrl(ethCtrlIdx);

    if( virtCtrl->Type == ETHIF_Q_TAG_NOT_TAGGED )
    {
      queriedEthIfCtrlIdx = virtCtrl->EthIfCtrlIdx;
    }

  }
  /* #20 Otherwise (tagged data path) */
  else
  {
    uint8_least rangeIdx;
    uint8_least rangeEndIdx;

    /* #200 Query within the EthIf controller group for the Ethernet controller queried for for a matching VLAN ID
     *      which results in retrieving the searched EthIf controller */
    rangeEndIdx = EthIf_CfgAccess_EthIfCtrlNum();
#if ( ETHIF_ETHCTRL_NUM > 1U )
    if( ethCtrlIdx < (EthIf_CfgAccess_EthCtrlNum() - 1U) )
    {
      rangeEndIdx = EthIf_CfgAccess_VirtCtrlBase4EthCtrl(ethCtrlIdx + 1U).EthIfCtrlIdx;
    }
#endif /* ETHIF_ETHCTRL_NUM */

    /* search for a matching EthIf controller within the EthIf controller group related to the Ethernet controller */
    for( rangeIdx = EthIf_CfgAccess_VirtCtrlBase4EthCtrl(ethCtrlIdx).EthIfCtrlIdx; rangeIdx < rangeEndIdx; rangeIdx++ )
    {
      virtCtrl = &EthIf_CfgAccess_VirtCtrl(rangeIdx);

      if( virtCtrl->Type == ETHIF_Q_TAG_TAGGED )
      {
        if( virtCtrl->VID == vlanId )
        {
          /* match */
          queriedEthIfCtrlIdx = virtCtrl->EthIfCtrlIdx;
          break;
        }
      }
    }
  }

  return queriedEthIfCtrlIdx;
} /* EthIf_QueryForEthIfController() */

/**********************************************************************************************************************
 *  EthIf_IsRxAllowed
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_IsRxAllowed(
          uint8                                     ethCtrlIdx,
    P2VAR(uint8,              AUTOMATIC, AUTOMATIC) ethIfCtrlIdx,
  P2CONST(EthIf_FrameHdrType, AUTOMATIC, AUTOMATIC) frameHdr,
  P2CONST(uint8,              AUTOMATIC, AUTOMATIC) payloadPtr,
          uint16                                    payloadLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean result = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  (*ethIfCtrlIdx) = EthIf_QueryForEthIfController(ethCtrlIdx, frameHdr->VlanId);

  if( (*ethIfCtrlIdx) <= EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    /* \trace SPEC-2393656 */
    if( EthIf_CfgAccess_EthIfCtrlMode((*ethIfCtrlIdx)) == ETH_MODE_ACTIVE )
    {
#if ( ETHIF_FIREWALL_SUPPORT == STD_ON )
      if( EthFw_IsFrameRxAllowed((*ethIfCtrlIdx), frameHdr, payloadPtr, payloadLen) == TRUE )
#else
      ETHIF_DUMMY_STATEMENT_CONST(payloadPtr);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
      ETHIF_DUMMY_STATEMENT(payloadLen);        /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* ETHIF_FIREWALL_SUPPORT */
      {
        result = TRUE;
      }
    }
  }

  return result;
} /* EthIf_IsRxAllowed() */

/**********************************************************************************************************************
 *  EthIf_HandleUlRxIndication
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_HandleUlRxIndication(
          uint8                                                ownerIdx,
  P2CONST(EthIf_UlRxIndiactionDataType, AUTOMATIC, AUTOMATIC)  ulRxIndicationData)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean     result;
  uint8_least loopIdx;

  /* ----- Implementation ----------------------------------------------- */
  result = FALSE;

  /* #10 If there is an owner for the frame */
  if( ownerIdx <= EthIf_CfgAccess_MaxValidFrameOwnerIdx() )
  {
    /* #100 Pass frame to upper layers */
    for( loopIdx = 0; loopIdx < EthIf_CfgAccess_RxIndInfo(ownerIdx).OwnerRxIndicationNum; loopIdx++ )
    {
      EthIf_CfgAccess_RxIndInfo(ownerIdx).OwnerRxIndications[loopIdx](ulRxIndicationData->EthIfCtrlIdx,
                                                                      ulRxIndicationData->EtherType,
                                                                      ulRxIndicationData->IsBroadcast,
                                                                      ulRxIndicationData->SrcMacPtr,
                                                                      ulRxIndicationData->PayloadPtr,
                                                                      ulRxIndicationData->PayloadLen);
      result = TRUE;
    }
  }

  return result;
} /* EthIf_HandleUlRxIndication() */

/**********************************************************************************************************************
 *  EthIf_IsTxAllowed
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_IsTxAllowed(
  P2CONST(EthIf_TxContextType, AUTOMATIC, AUTOMATIC) txContext)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean result;

  /* ----- Implementation ----------------------------------------------- */
#if ( ETHIF_FIREWALL_SUPPORT == STD_ON )
  result = EthFw_IsFrameTxAllowed(txContext->EthIfCtrlIdx, &txContext->FrameHdr, txContext->PayloadPtr, txContext->PayloadLen);
#else
  result = TRUE;
  ETHIF_DUMMY_STATEMENT_CONST(txContext); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* ETHIF_FIREWALL_SUPPORT */

  return result;
} /* EthIf_IsTxAllowed() */

/**********************************************************************************************************************
 *  EthIf_RetrieveSrcMacAddr
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_RetrieveSrcMacAddr(
  P2VAR(EthIf_TxContextType, AUTOMATIC, AUTOMATIC) txContext)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, AUTOMATIC) virtCtrl;

  /* ----- Implementation ----------------------------------------------- */
  virtCtrl = &EthIf_CfgAccess_VirtCtrl(txContext->EthIfCtrlIdx);

  /* if source MAC address wasn't stored in buffer yet, retrieve it */
  if( EthIf_SrcMacAddrBuffer[virtCtrl->EthCtrl->CtrlIdx].IsValid == FALSE )
  {
    virtCtrl->EthCtrl->DrvApi->GetPhysAddr(virtCtrl->EthCtrl->CtrlIdx,
                                           EthIf_SrcMacAddrBuffer[virtCtrl->EthCtrl->CtrlIdx].SrcMacAddr);

    EthIf_SrcMacAddrBuffer[virtCtrl->EthCtrl->CtrlIdx].IsValid = TRUE;
  }

  /* set source MAC address for tx context */
  txContext->FrameHdr.SrcMacAddrPtr = EthIf_SrcMacAddrBuffer[virtCtrl->EthCtrl->CtrlIdx].SrcMacAddr;
} /* EthIf_RetrieveSrcMacAddr() */

/**********************************************************************************************************************
 *  EthIf_TearDownTransmission
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(Std_ReturnType, ETHIF_CODE) EthIf_TearDownTransmission(
  P2VAR(EthIf_TxContextType, AUTOMATIC, AUTOMATIC) txContext,
          uint8                                    bufIdx,
          boolean                                  isTxConfRequested)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, AUTOMATIC) virtCtrl;
          Std_ReturnType                            retVal;
          Eth_FrameType                             llEtherType;

  /* ----- Implementation ----------------------------------------------- */
  /* set EtherType for driver API call */
  llEtherType = txContext->FrameHdr.EtherType;
  if( txContext->FrameHdr.VlanId != ETHIF_INV_VLAN_ID )
  {
    llEtherType = ETHIF_VLAN_TPID;
  }

  /* release the buffer in driver */
  virtCtrl = &EthIf_CfgAccess_VirtCtrl(txContext->EthIfCtrlIdx);
  retVal = virtCtrl->EthCtrl->DrvApi->Transmit(virtCtrl->EthCtrl->CtrlIdx,
                                               bufIdx,
                                               llEtherType,
                                               isTxConfRequested,
                                               0,
                                               txContext->FrameHdr.DstMacAddrPtr);

  if( retVal == E_OK )
  {
    /* handle possible Tx confirmation request of upper layer */
    if( isTxConfRequested == TRUE )
    {
      EthIf_TxConfirmation(virtCtrl->EthCtrl->CtrlIdx, bufIdx);
    }
    else
    {
      txContext->IsTxPending = FALSE;
      txContext->IsValid     = FALSE;
    }
  }

  return retVal;
} /* EthIf_TearDownTransmission() */

#if ( ETHIF_WAKEUP_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_QueryForWakeupMap
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(uint8, ETHIF_CODE) EthIf_QueryForWakeupMap(
  EcuM_WakeupSourceType wakeupSource)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8       queriedWakeupMapIdx;
  uint8_least loopIdx;

  /* ----- Implementation ----------------------------------------------- */
  queriedWakeupMapIdx = EthIf_CfgAccess_InvalidWakeupMapIdx();

  /* #10 For each wakeup map */
  for( loopIdx = 0; loopIdx < EthIf_CfgAccess_WakeupMapNum(); loopIdx++ )
  {
    /* #100 Check if wakeup source queried for matches the one of the map */
    if( wakeupSource == EthIf_CfgAccess_WakeupMap(loopIdx).WakeupSource )
    {
      /* #1000 Wakeup source matched -> wakeup map found */
      queriedWakeupMapIdx = (uint8)loopIdx;
      break;
    }
  }

  return queriedWakeupMapIdx;
} /* EthIf_QueryForWakeupMap() */

/**********************************************************************************************************************
 *  EthIf_QueryForWakeupTransceiver
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(uint8, ETHIF_CODE) EthIf_QueryForWakeupTransceiver(
  uint8 ethIfCtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl;
          uint8                                       queriedWakeupEthTrcvIdx;
          uint8_least                                 loopIdx;

  /* ----- Implementation ----------------------------------------------- */
  queriedWakeupEthTrcvIdx = ETHIF_INVALID_WAKEUP_ETHTRCV;
  virtCtrl                = &EthIf_CfgAccess_VirtCtrl(ethIfCtrlIdx);

  /* #10 For each physical layer element */
  for( loopIdx = 0; loopIdx < (uint8_least)virtCtrl->PhyLayer->InitElementNum; loopIdx++ )
  {
    /* #100 Check if physical layer element is a transceiver */
    if( virtCtrl->PhyLayer->PhyLayerInitElements[loopIdx].PhyType == (EthIf_PhyTypeType)ETHIF_PHYLAYER_ETHTRCV )
    {
      /* #1000 Is transceiver -> element found */
      queriedWakeupEthTrcvIdx = (uint8)loopIdx;
      break;
    }
  }

  return queriedWakeupEthTrcvIdx;
} /* EthIf_QueryForWakeupTransceiver() */

#endif /* ETHIF_WAKEUP_SUPPORT */

#if ( ETHIF_ENABLE_ZERO_COPY_EXTENSIONS == STD_ON )
/**********************************************************************************************************************
 *  EthIf_IsImplicitBufferReleaseNeeded
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_IsImplicitBufferReleaseNeeded(
  uint8          ethIfCtrlIdx,
  Eth_FrameType  etherType)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean      retVal;
  uint8_least  filterIdx;

  /* ----- Implementation ----------------------------------------------- */
  retVal = TRUE;

  /* #10 For each explicit buffer release filter rule */
  for( filterIdx = 0U; filterIdx < EthIf_CfgAccess_ExplicitBufferReleaseFilterNum(); filterIdx++ )
  {
    /* #100 If filter rule's EthIf controller matches */
    if( EthIf_CfgAccess_ExplicitBufferReleaseFilter(filterIdx).EthIfCtrlIdx == ethIfCtrlIdx )
    {
      /* #1000 If filter rule's EtherType matches or EtherType is a 'don't care' */
      if( (EthIf_CfgAccess_ExplicitBufferReleaseFilter(filterIdx).EtherType == etherType) ||
          (EthIf_CfgAccess_ExplicitBufferReleaseFilter(filterIdx).EtherType == ETHIF_EXPLICIT_BUFFER_RELEASE_FRAME_TYPE_DONT_CARE) )
      {
        /* #10000 Explicit release of buffer is needed -> abort search */
        retVal = FALSE;
        break;
      }
    }
  }

  return retVal;
} /* EthIf_IsImplicitBufferReleaseNeeded() */
#endif /* ETHIF_ENABLE_ZERO_COPY_EXTENSIONS */

#if ( ETHIF_ETHSWT_TIME_STAMP_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_IsOwnerRegisteredForSwtTimeStamping
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_IsOwnerRegisteredForSwtTimeStamping(
  uint8 ownerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean result;

  /* ----- Implementation ----------------------------------------------- */
  result = FALSE;

  /* #10 Retrieve if owner registered for switch time stamping information */
  if( EthIf_CfgAccess_OwnerSwtTsIndsInfo(ownerIdx).TimeStampIndicationNum > 0)
  {
    result = TRUE;
  }

  return result;
} /* EthIf_IsOwnerRegisteredForSwtTimeStamping() */

/**********************************************************************************************************************
 *  EthIf_StartTxSwtTimeStampInfoProc
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_StartTxSwtTimeStampInfoProc(
        uint8                               ethCtrlIdx,
        uint8                               ethIfCtrlIdx,
        uint8                               bufIdx,
        uint16                              bufLen,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA)  dataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(EthIf_SwtTimeStampTxCtxtHandlingType, AUTOMATIC, ETHIF_CONST) txCtxtHndl;
          uint8                                                         poolElemIdx;

  /* ----- Implementation ----------------------------------------------- */
  txCtxtHndl = &EthIf_CfgAccess_SwtTimeStampTxCtxtHandling(ethCtrlIdx);

  /* #10 Enter exclusive area (Reason: Ensure consistency of switch transmission timestamp elements) */
  EthIf_EnterExclusiveArea(SWT_TIME_STAMP_TX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Query for a free element to handle the transmission switch time stamping processing */
  for( poolElemIdx = 0; poolElemIdx < txCtxtHndl->PoolSize; poolElemIdx++ )
  {
    if( txCtxtHndl->Pool[poolElemIdx].FrameId == ETH_INVALID_FRAME_ID )
    {
      /* #200 Entry found -> set the information needed to handle switch time stamping processing */
      txCtxtHndl->Pool[poolElemIdx].DataPtr       = dataPtr;
      txCtxtHndl->Pool[poolElemIdx].BufLen        = bufLen;
      txCtxtHndl->Pool[poolElemIdx].FrameId       = EthIf_GetFrameId(dataPtr, bufLen); /* PRQA S 0310, 3305 */ /* MD_EthIf_310_Buffer, MD_EthIf_3305_Buffer */
      txCtxtHndl->Pool[poolElemIdx].EthIfCtrlIdx  = ethIfCtrlIdx;
      txCtxtHndl->Pool[poolElemIdx].BufIdx        = bufIdx;
      txCtxtHndl->Pool[poolElemIdx].FrameOwnerIdx = EthIf_CfgAccess_InvalidFrameOwnerIdx();
      txCtxtHndl->Pool[poolElemIdx].ProcFlags     = 0x00u;
      break;
    }
  }

  /* #30 Leave exclusive area -> Processing on elements finished */
  EthIf_ExitExclusiveArea(SWT_TIME_STAMP_TX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* EthIf_StartTxSwtTimeStampInfoProc() */

/**********************************************************************************************************************
 *  EthIf_FinishTxSwtTimeStampInfoProc
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_FinishTxSwtTimeStampInfoProc(
  uint8  ethCtrlIdx,
  uint8  bufIdx,
  uint8  frameOwnerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(EthIf_SwtTimeStampTxCtxtHandlingType, AUTOMATIC, ETHIF_CONST) txCtxtHndl;
          uint8                                                         poolElemIdx;

  /* ----- Implementation ----------------------------------------------- */
  txCtxtHndl = &EthIf_CfgAccess_SwtTimeStampTxCtxtHandling(ethCtrlIdx);

  /* #10 Enter exclusive area (Reason: Ensure consistency of switch transmission timestamp elements) */
  EthIf_EnterExclusiveArea(SWT_TIME_STAMP_TX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Query for the information related to the frame buffer */
  for( poolElemIdx = 0; poolElemIdx < txCtxtHndl->PoolSize; poolElemIdx++ )
  {
    if( txCtxtHndl->Pool[poolElemIdx].BufIdx == bufIdx )
    {
      /* check if buffer index isn't used more than once (indicated by a changed frame id) */
      /* PRQA S 0310, 3305 1 */ /* MD_EthIf_310_Buffer, MD_EthIf_3305_Buffer */
      if( EthIf_GetFrameId(txCtxtHndl->Pool[poolElemIdx].DataPtr, txCtxtHndl->Pool[poolElemIdx].BufLen) == txCtxtHndl->Pool[poolElemIdx].FrameId )
      {
        /* #200 Entry found -> Check if switch time stamping was requested for the frame.
         *                     If so, store the owner information and keep the information for later usage in
         *                     EthIf_SwitchIngressTimeStampIndication()/EthIf_SwitchEgressTimeStampIndication().
         *                     Otherwise invalidate the information so it can be used for another frame. */
        if( txCtxtHndl->Pool[poolElemIdx].ProcFlags == 0x00U )
        {
          txCtxtHndl->Pool[poolElemIdx].DataPtr = NULL_PTR;
          txCtxtHndl->Pool[poolElemIdx].FrameId = ETH_INVALID_FRAME_ID;
          txCtxtHndl->Pool[poolElemIdx].BufIdx  = EthIf_CfgAccess_InvalidTxBufIdx();
        }
        else
        {
          txCtxtHndl->Pool[poolElemIdx].FrameOwnerIdx = frameOwnerIdx;
        }
        break;
      }
    }
  }

  /* #30 Leave exclusive area -> Processing of elements finished */
  EthIf_ExitExclusiveArea(SWT_TIME_STAMP_TX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* EthIf_FinishTxSwtTimeStampInfoProc() */

/**********************************************************************************************************************
 *  EthIf_InitiateRxSwtTimeStampInfoProc
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_InitiateRxSwtTimeStampInfoProc(
          uint8                                                ethCtrlIdx,
          uint8                                                ownerIdx,
  P2CONST(EthIf_UlRxIndiactionDataType, AUTOMATIC, AUTOMATIC)  ulRxIndicationData)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(EthIf_SwtTimeStampRxCtxtHandlingType, AUTOMATIC, ETHIF_CONST) rxCtxtHndl;
          uint8                                                         poolElemIdx;

  /* ----- Implementation ----------------------------------------------- */
  rxCtxtHndl = &EthIf_CfgAccess_SwtTimeStampRxCtxtHandling(ethCtrlIdx);

  /* #10 Enter exclusive area (Reason: Ensure consistency of switch reception timestamp elements) */
  EthIf_EnterExclusiveArea(SWT_TIME_STAMP_RX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Query for a free element to handle the reception switch time stamping processing */
  for( poolElemIdx = 0; poolElemIdx < rxCtxtHndl->PoolSize; poolElemIdx++ )
  {
    if( rxCtxtHndl->Pool[poolElemIdx].FrameId == ETH_INVALID_FRAME_ID )
    {
      /* #200 Entry found -> set the information needed to handle switch time stamping processing */
      rxCtxtHndl->Pool[poolElemIdx].DataPtr       = ulRxIndicationData->PayloadPtr;
      rxCtxtHndl->Pool[poolElemIdx].FrameId       = EthIf_GetFrameId(ulRxIndicationData->PayloadPtr, ulRxIndicationData->PayloadLen);  /* PRQA S 0310, 3305 */ /* MD_EthIf_310_Buffer, MD_EthIf_3305_Buffer */
      rxCtxtHndl->Pool[poolElemIdx].EthIfCtrlIdx  = ulRxIndicationData->EthIfCtrlIdx;
      rxCtxtHndl->Pool[poolElemIdx].FrameOwnerIdx = ownerIdx;
      rxCtxtHndl->Pool[poolElemIdx].ProcFlags     = (ETHIF_SWT_TIMESTAMP_PROC_FLAG_INGRESS_TS_PENDING |
                                                     ETHIF_SWT_TIMESTAMP_PROC_FLAG_EGRESS_TS_PENDING    );
      break;
    }
  }

  /* #30 Leave exclusive area -> Processing of elements finished */
  EthIf_ExitExclusiveArea(SWT_TIME_STAMP_RX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* EthIf_InitiateRxSwtTimeStampInfoProc() */
#endif /* ETHIF_ETHSWT_TIME_STAMP_SUPPORT */

#if ( ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_IsOwnerRegisteredForSwtMgmtInfo
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(boolean, ETHIF_CODE) EthIf_IsOwnerRegisteredForSwtMgmtInfo(
  uint8 ownerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean result;

  /* ----- Implementation ----------------------------------------------- */
  result = FALSE;

  /* #10 Retrieve if owner registered for switch frame management information */
  if( EthIf_CfgAccess_OwnerMgmtInfoIndInfo(ownerIdx).MgmtInfoIndicationNum > 0)
  {
    result = TRUE;
  }

  return result;
} /* EthIf_IsOwnerRegisteredForSwtMgmtInfo() */

/**********************************************************************************************************************
 *  EthIf_InitiateRxSwtMgmtInfoProc
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_InitiateRxSwtMgmtInfoProc(
          uint8                                                ethCtrlIdx,
          uint8                                                ownerIdx,
  P2CONST(EthIf_UlRxIndiactionDataType, AUTOMATIC, AUTOMATIC)  ulRxIndicationData)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(EthIf_MgmtRxCtxtPoolElemType, AUTOMATIC, ETHIF_VAR_NOINIT) rxCtxtHndlPool;
          uint8                                                    poolElemIdx;

  /* ----- Implementation ----------------------------------------------- */
  rxCtxtHndlPool = EthIf_CfgAccess_MgmtRxCtxtHandling(ethCtrlIdx);

  /* #10 Enter exclusive area (Reason: Ensure consistency of switch management information elements) */
  EthIf_EnterExclusiveArea(MGMT_RX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Query for a free element to handle the reception switch frame management information processing */
  for( poolElemIdx = 0; poolElemIdx < ETHIF_MAX_PARALELL_RX_CONTEXT_PER_ETHCTRL; poolElemIdx++ )
  {
    if( rxCtxtHndlPool[poolElemIdx].FrameId == ETH_INVALID_FRAME_ID )
    {
      /* #200 Entry found -> set the information needed to handle switch frame management information processing */
      rxCtxtHndlPool[poolElemIdx].DataPtr       = ulRxIndicationData->PayloadPtr;
      rxCtxtHndlPool[poolElemIdx].FrameId       = EthIf_GetFrameId(ulRxIndicationData->PayloadPtr, ulRxIndicationData->PayloadLen);  /* PRQA S 0310, 3305 */ /* MD_EthIf_310_Buffer, MD_EthIf_3305_Buffer */
      rxCtxtHndlPool[poolElemIdx].EthIfCtrlIdx  = ulRxIndicationData->EthIfCtrlIdx;
      rxCtxtHndlPool[poolElemIdx].FrameOwnerIdx = ownerIdx;
      break;
    }
  }

  /* #30 Leave exclusive area -> Processing of elements finished */
  EthIf_ExitExclusiveArea(MGMT_RX_CTXT_POOL); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* EthIf_InitiateRxSwtMgmtInfoProc() */
#endif /* ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT */

/**********************************************************************************************************************
 *  EthIf_UpdateTxStats
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_UpdateTxStats(
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl,
          uint16                                      payloadLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 updateBytes;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate the bytes transmitted with this frame */
  updateBytes = (payloadLen + ETH_HEADER_LEN_BYTE);
  if( virtCtrl->Type == ETHIF_Q_TAG_TAGGED )
  {
    updateBytes += ETHIF_VLAN_TAG_LEN_BYTE;
  }
  if( updateBytes < ETHIF_MIN_ETH_FRAME_LEN_BYTE )
  {
    updateBytes = ETHIF_MIN_ETH_FRAME_LEN_BYTE;
  }

  /* #20 Enter exclusive area (Reason: Update statistic counters in a consistent manner) */
  EthIf_EnterExclusiveArea(RXTX_STATS); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if( (virtCtrl->TxStats->NumTxBytes + updateBytes) < ETH_RXTX_STATS_MAX_COUNTER_VAL )
  {
    /* #30 If the transmitted bytes counter is able to hold the update value set it */
    virtCtrl->TxStats->NumTxBytes += updateBytes;
  }
  else
  {
    /* #40 Otherwise set it to the special overflow value */
    virtCtrl->TxStats->NumTxBytes = ETH_RXTX_STATS_COUNTER_OVERFLOW_VAL;
  }

  /* #50 If the transmitted frames counter hasn't overflown since last update, update it */
  if( virtCtrl->TxStats->NumTxPkts != ETH_RXTX_STATS_COUNTER_OVERFLOW_VAL )
  {
    virtCtrl->TxStats->NumTxPkts++;
  }

  /* #60 Leave exclusive area */
  EthIf_ExitExclusiveArea(RXTX_STATS); /* PRQA S 3109 */ /* MD_MSR_14.3 */

} /* EthIf_UpdateTxStats() */

/**********************************************************************************************************************
 *  EthIf_ClearTxStats
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_ClearTxStats(
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Clear tx statistic counters */
  virtCtrl->TxStats->NumTxPkts  = 0;
  virtCtrl->TxStats->NumTxBytes = 0;
} /* EthIf_ClearTxStats() */

/**********************************************************************************************************************
 *  EthIf_UpdateRxStats
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_UpdateRxStats(
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl,
          uint16                                      payloadLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 updateBytes;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate the bytes received with this frame */
  updateBytes = (payloadLen + ETH_HEADER_LEN_BYTE);
  if( virtCtrl->Type == ETHIF_Q_TAG_TAGGED )
  {
    updateBytes += ETHIF_VLAN_TAG_LEN_BYTE;
  }

  /* #20 Enter exclusive area (Reason: Update statistic counters in a consistent manner) */
  EthIf_EnterExclusiveArea(RXTX_STATS); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if( (virtCtrl->RxStats->NumRxBytes + updateBytes) < ETH_RXTX_STATS_MAX_COUNTER_VAL )
  {
    /* #30 If the received bytes counter is able to hold the update value set it */
    virtCtrl->RxStats->NumRxBytes += updateBytes;
  }
  else
  {
    /* #40 Otherwise set it to the special overflow value */
    virtCtrl->RxStats->NumRxBytes = ETH_RXTX_STATS_COUNTER_OVERFLOW_VAL;
  }

  /* #50 If the received frames counter hasn't overflown since last update, update it */
  if( virtCtrl->RxStats->NumRxPkts != ETH_RXTX_STATS_COUNTER_OVERFLOW_VAL )
  {
    virtCtrl->RxStats->NumRxPkts++;
  }

  /* #60 Leave exclusive area */
  EthIf_ExitExclusiveArea(RXTX_STATS); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* EthIf_UpdateRxStats() */

/**********************************************************************************************************************
 *  EthIf_ClearRxStats
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
ETHIF_LOCAL_INLINE FUNC(void, ETHIF_CODE) EthIf_ClearRxStats(
  P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Clear rx statistic counters */
  virtCtrl->RxStats->NumRxPkts  = 0;
  virtCtrl->RxStats->NumRxBytes = 0;
} /* EthIf_ClearRxStats() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EthIf_InitMemory
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_InitMemory( void )
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Development Error Checks ------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set module initialization state to UNINIT */
  EthIf_ModuleInitialized = ETHIF_STATE_UNINIT;
} /* EthIf_InitMemory() */

/**********************************************************************************************************************
 *  EthIf_Init
 *********************************************************************************************************************/
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
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_Init(
  P2CONST(EthIf_ConfigType, AUTOMATIC, ETHIF_CONST) CfgPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  /* Check initialization state of the component */
  if (EthIf_ModuleInitialized != ETHIF_STATE_UNINIT)
  {
    errorId = ETHIF_E_INIT_FAILED;
  }
  /* #20 Check input parameters for plausibility */
  /* Check parameter 'ConfigPtr' */
  else if (CfgPtr != NULL_PTR)
  {
    /* \trace SPEC-2393577 */
    errorId = ETHIF_E_INIT_FAILED;
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    uint16_least  loopIdx;

#if (ETHIF_DEV_ERROR_DETECT == STD_OFF)
    /* dummy statement to avoid compiler warnings */
    ETHIF_DUMMY_STATEMENT(CfgPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

    /* #30 Initialize module related data */
    EthIf_CfgAccess_IsCtrlInitLocked()    = FALSE;
    EthIf_CfgAccess_IsSetCtrlModeLocked() = FALSE;
#if ( ETHIF_ENABLE_MAIN_FUNCTION_STATE == STD_OFF )
    EthIf_MainFuncStateFreqCnt = EthIf_CfgAccess_TrcvLinkStateChgReload();
#endif /* ETHIF_ENABLE_MAIN_FUNCTION_STATE */

    /* #40 Initialize EthIf Controller related data */
    for( loopIdx = 0; loopIdx < EthIf_CfgAccess_EthIfCtrlNum(); loopIdx++ )
    {
      EthIf_CfgAccess_EthIfCtrlMode(loopIdx)      = ETH_MODE_DOWN;
      EthIf_CfgAccess_EthIfCtrlInitMode(loopIdx)  = ETHIF_NOT_INITIALIZED;
      EthIf_CfgAccess_EthIfCtrlLinkState(loopIdx) = ETHIF_ETHIFCTRL_LINK_STATE_NO_LINK;
    }

    /* #50 Initialize transmission related state variables */
    for( loopIdx = 0; loopIdx < EthIf_CfgAccess_MaxTxBufNum(); loopIdx++ )
    {
      EthIf_CfgAccess_GetTxContextByIndex((uint8)loopIdx)->IsTxPending = FALSE;
      EthIf_CfgAccess_GetTxContextByIndex((uint8)loopIdx)->IsValid     = FALSE;
    }

    /* #60 Initialize Ethernet Controller related data */
    for( loopIdx = 0; loopIdx < EthIf_CfgAccess_EthCtrlNum(); loopIdx++ )
    {
      /* Initialize init and mode counts */
      EthIf_CfgAccess_EthCtrlSummary(loopIdx)->InitCnt[0] = 0u;
      EthIf_CfgAccess_EthCtrlSummary(loopIdx)->ModeCnt[0] = 0u;
      EthIf_SrcMacAddrBuffer[loopIdx].IsValid             = FALSE;
    }

#if ( ETHIF_ETHTRCV_USED == STD_ON )
    /* #70 Initialize Ethernet Transceiver related data */
    for( loopIdx = 0; loopIdx < EthIf_CfgAccess_EthTrcvNum(); loopIdx++ )
    {
      /* Initialize init and mode counts */
      EthIf_CfgAccess_EthTrcvSummary(loopIdx)->InitCnt[0] = 0u;
      EthIf_CfgAccess_EthTrcvSummary(loopIdx)->ModeCnt[0] = 0u;
    }
#endif /* ETHIF_ETHTRCV_USED */

#if ( ETHIF_ETHSWT_USED == STD_ON )
    /* #80 Initialize Ethernet Switch related data */
    for( loopIdx = 0; loopIdx < EthIf_CfgAccess_EthSwtNum(); loopIdx++ )
    {
      /* Initialize init counts */
      EthIf_CfgAccess_EthSwtSummary(loopIdx)->InitCnt[0] = 0u;
    }

    /* #90 Initialize Ethernet Switch Port related data */
    for( loopIdx = 0; loopIdx < EthIf_CfgAccess_EthSwtPortNum(); loopIdx++ )
    {
      /* Initialize mode counts */
      EthIf_CfgAccess_EthSwtPortSummary(loopIdx)->ModeCnt[0] = 0u;
    }
    /* #91 Initialize EthIf switch port group related data */
    for( loopIdx = 0; loopIdx < EthIf_CfgAccess_EthIfSwtPortGroupNum(); loopIdx++ )
    {
      EthIf_CfgAccess_EthIfSwtPortGroupMode(loopIdx)      = ETHTRCV_MODE_DOWN;
      EthIf_CfgAccess_EthIfSwtPortGroupLinkState(loopIdx) = ETHIF_ETHIFSWTPORTGROUP_LINK_STATE_NO_LINK;
    }
#endif /* ETHIF_ETHSWT_USED */

#if   ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_MIRRORING )
    /* #100 Initialize mirroring related data */
    for( loopIdx = 0; loopIdx < ETHIF_TRAFFIC_MIRRORING_MAX_PARALLEL_TX; loopIdx++ )
    {
        /* only invalidate the tx traffic info object (all other values will be considered invalid too) */
        EthIf_Mirroring_TxInfoObjects[loopIdx].IsInfoValid = FALSE;
        EthIf_Mirroring_TxInfoObjects[loopIdx].IsUsed      = FALSE;
    }
#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */

    /* #110 Set module state to initialized */
    EthIf_ModuleInitialized = ETHIF_STATE_INIT; /* \trace SPEC-2393576 */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #120 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_INIT, errorId);
  }
#else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
} /* EthIf_Init() */ /* PRQA S 6010,6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */

/**********************************************************************************************************************
 *  EthIf_ControllerInit
 *********************************************************************************************************************/
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
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_ControllerInit(
  uint8  CtrlIdx,
  uint8  CfgIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_NOT_OK;
  uint8           errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393583 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    /* \trace SPEC-2393584 */
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST)  virtCtrl;
    virtCtrl = &EthIf_CfgAccess_VirtCtrl(CtrlIdx);
  
    /* #30 Enter exclusive area (Reason: Check and take semaphore that protects the function from executing
     *                                   multiple hardware initialization processes in parallel) */
    EthIf_EnterExclusiveArea(CTRL_INIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #40 Check if initialization protection semaphore is taken already */
    if( EthIf_CfgAccess_IsCtrlInitLocked() == FALSE )
    {
      /* #400 Take initialization protection semaphore */
      EthIf_CfgAccess_IsCtrlInitLocked() = TRUE;

      /* #410 Leave exclusive area -> semaphore was taken and initialization can be processed further */
      EthIf_ExitExclusiveArea(CTRL_INIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #420 Check if EthIf controller is not initialized */
      if( EthIf_CfgAccess_EthIfCtrlInitMode(CtrlIdx) == ETHIF_NOT_INITIALIZED )
      {
        /* #4200 Initialize Ethernet controller if needed by utilizing the Ethernet driver */
        if( virtCtrl->EthCtrl->InitCnt[0] == 0u )
        {
          /* \trace SPEC-2393582 */
          retVal = virtCtrl->EthCtrl->DrvApi->ControllerInit(virtCtrl->EthCtrl->CtrlIdx,
                                                             CfgIdx);
#if ( ETHIF_ETHSWT_USED == STD_ON )
# if ( ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT == STD_ON )
          {
            /* #42000 initialize variables related to the switch frame management feature */
            uint8_least poolElemIdx;
            if( EthIf_CfgAccess_MgmtRxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx) != NULL_PTR )
            {
              for( poolElemIdx = 0U; poolElemIdx < ETHIF_MAX_PARALELL_RX_CONTEXT_PER_ETHCTRL; poolElemIdx++ )
              {
                EthIf_CfgAccess_MgmtRxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx)[poolElemIdx].DataPtr       = NULL_PTR;
                EthIf_CfgAccess_MgmtRxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx)[poolElemIdx].FrameId       = ETH_INVALID_FRAME_ID;
                EthIf_CfgAccess_MgmtRxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx)[poolElemIdx].EthIfCtrlIdx  = EthIf_CfgAccess_InvalidEthIfCtrlIdx();
                EthIf_CfgAccess_MgmtRxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx)[poolElemIdx].FrameOwnerIdx = EthIf_CfgAccess_InvalidFrameOwnerIdx();
              }
            }
          }
# endif /* ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT */
# if ( ETHIF_ETHSWT_TIME_STAMP_SUPPORT == STD_ON )
          {
            /* #42010 initialize variables related to the switch timestamping feature */
            uint8_least poolElemIdx;
            for( poolElemIdx = 0; poolElemIdx < EthIf_CfgAccess_SwtTimeStampTxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx).PoolSize; poolElemIdx++ )
            {
              EthIf_CfgAccess_SwtTimeStampTxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx).Pool[poolElemIdx].DataPtr        = NULL_PTR;
              EthIf_CfgAccess_SwtTimeStampTxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx).Pool[poolElemIdx].FrameId        = ETH_INVALID_FRAME_ID;
              EthIf_CfgAccess_SwtTimeStampTxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx).Pool[poolElemIdx].EthIfCtrlIdx   = EthIf_CfgAccess_InvalidEthIfCtrlIdx();
              EthIf_CfgAccess_SwtTimeStampTxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx).Pool[poolElemIdx].BufIdx         = EthIf_CfgAccess_InvalidTxBufIdx();
              EthIf_CfgAccess_SwtTimeStampTxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx).Pool[poolElemIdx].FrameOwnerIdx  = EthIf_CfgAccess_InvalidFrameOwnerIdx();
              EthIf_CfgAccess_SwtTimeStampTxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx).Pool[poolElemIdx].ProcFlags      = 0x00u;
            }

            for( poolElemIdx = 0; poolElemIdx < EthIf_CfgAccess_SwtTimeStampRxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx).PoolSize; poolElemIdx++ )
            {
              EthIf_CfgAccess_SwtTimeStampRxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx).Pool[poolElemIdx].DataPtr        = NULL_PTR;
              EthIf_CfgAccess_SwtTimeStampRxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx).Pool[poolElemIdx].FrameId        = ETH_INVALID_FRAME_ID;
              EthIf_CfgAccess_SwtTimeStampRxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx).Pool[poolElemIdx].EthIfCtrlIdx   = EthIf_CfgAccess_InvalidEthIfCtrlIdx();
              EthIf_CfgAccess_SwtTimeStampRxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx).Pool[poolElemIdx].FrameOwnerIdx  = EthIf_CfgAccess_InvalidFrameOwnerIdx();
              EthIf_CfgAccess_SwtTimeStampRxCtxtHandling(virtCtrl->EthCtrl->CtrlIdx).Pool[poolElemIdx].ProcFlags      = 0x00u;
            }
          }
# endif /* ETHIF_ETHSWT_TIME_STAMP_SUPPORT */
#endif /* ETHIF_ETHSWT_USED */
        }
        else
        {
          retVal = E_OK;
        }

        /* #4300 Check if Ethernet controller initialization was successful */
        if( retVal == E_OK )
        {
          uint8_least phyElementIdx;

          /* #43000 Initialization of all physical layer elements (Transceivers, Switches) if no previous initialization
           *        took place by utilizing the respective lower layer driver */
          for( phyElementIdx = 0; phyElementIdx < virtCtrl->PhyLayer->InitElementNum; phyElementIdx++ )
          {
            P2CONST(EthIf_AbstractPhyElementType, AUTOMATIC, ETHIF_CONST) phyElement;

            if( retVal == E_NOT_OK ) { break; } /* only proceed if previous call was successful */

            phyElement = &virtCtrl->PhyLayer->PhyLayerInitElements[phyElementIdx];
            retVal = E_NOT_OK;

            switch( phyElement->PhyType )
            {
#if ( ETHIF_ETHTRCV_USED == STD_ON )
            case ETHIF_PHYLAYER_ETHTRCV:
              {
                /* PRQA S 310 1 */ /* MD_EthIf_310_PhyLayer */
                P2CONST(EthIf_EthTrcvType, AUTOMATIC, ETHIF_CONST) ethTrcv = ((P2CONST(EthIf_PhyEthTrcvType, AUTOMATIC, ETHIF_CONST))phyElement)->EthTrcv;
                if( ethTrcv->InitCnt[0] == 0u )
                { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
                  retVal = ethTrcv->DrvApi->TransceiverInit(ethTrcv->TrcvIdx,
                                                            CfgIdx);
                }
                else
                { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
                  retVal = E_OK;
                }
              }
              break;
#endif /* ETHIF_ETHTRCV_USED */
#if ( ETHIF_ETHSWT_USED == STD_ON )
            case ETHIF_PHYLAYER_ETHSWT:
              {
                /* PRQA S 310 1 */ /* MD_EthIf_310_PhyLayer */
                P2CONST(EthIf_EthSwtType, AUTOMATIC, ETHIF_CONST) ethSwt = ((P2CONST(EthIf_PhyEthSwtType, AUTOMATIC, ETHIF_CONST))phyElement)->EthSwt;
                if( ethSwt->InitCnt[0] == 0u )
                { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
                  retVal = ethSwt->DrvApi->SwitchInit(ethSwt->SwitchIdx);
                }
                else
                { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
                  retVal = E_OK;
                }
              }
              break;
#endif /* ETHIF_ETHSWT_USED */
            default:
              /* shouldn't ever occur -> if this case occurs retVal was already set to E_NOT_OK and processing is aborted */
              break;
            }
          }
        }
      }
      else
      {
        retVal = E_OK;
      }

      /* #440 Check if initialization was successful */
      if( retVal == E_OK )
      {
        /* #4400 Memorize the initialization calls for the EthIf controller, Ethernet controller and physical layer elements
         *       to prevent unwanted re-initialization */
        uint8_least phyElementIdx;

        /* memorize EthIf controller init state */
        EthIf_CfgAccess_EthIfCtrlInitMode(CtrlIdx) = ETHIF_INITIALIZED;

        /* increase Eth Controller init count */
        virtCtrl->EthCtrl->InitCnt[0]++;

        /* clear statistic counters */
        EthIf_ClearTxStats(virtCtrl);
        EthIf_ClearRxStats(virtCtrl);

        /* increase all physical layer elements init count */
        for( phyElementIdx = 0; phyElementIdx < virtCtrl->PhyLayer->InitElementNum; phyElementIdx++ )
        {
          P2CONST(EthIf_AbstractPhyElementType, AUTOMATIC, ETHIF_CONST) phyElement;

          phyElement = &virtCtrl->PhyLayer->PhyLayerInitElements[phyElementIdx];

          switch( phyElement->PhyType )
          {
#if ( ETHIF_ETHTRCV_USED == STD_ON )
          case ETHIF_PHYLAYER_ETHTRCV:
            {
              /* PRQA S 310 1 */ /* MD_EthIf_310_PhyLayer */
              P2CONST(EthIf_EthTrcvType, AUTOMATIC, ETHIF_CONST) ethTrcv = ((P2CONST(EthIf_PhyEthTrcvType, AUTOMATIC, ETHIF_CONST))phyElement)->EthTrcv;
              ethTrcv->InitCnt[0]++;
            }
            break;
#endif /* ETHIF_ETHTRCV_USED */
#if ( ETHIF_ETHSWT_USED == STD_ON )
          case ETHIF_PHYLAYER_ETHSWT:
            {
              /* PRQA S 310 1 */ /* MD_EthIf_310_PhyLayer */
              P2CONST(EthIf_EthSwtType, AUTOMATIC, ETHIF_CONST) ethSwt = ((P2CONST(EthIf_PhyEthSwtType, AUTOMATIC, ETHIF_CONST))phyElement)->EthSwt;
              ethSwt->InitCnt[0]++;
            }
            break;
#endif /* ETHIF_ETHSWT_USED */
          default:
            /* shouldn't ever occur */
            break;
          }
        }
      }

      /* #450 Release initialization protection semaphore */
      EthIf_CfgAccess_IsCtrlInitLocked() = FALSE;
    }
    /* #50 Otherwise (initialization protection semaphore already taken) */
    else
    {
      /* #500 Leave exclusive area -> semaphore was already taken */
      EthIf_ExitExclusiveArea(CTRL_INIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #60 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_CONTROLLER_INIT, errorId);
  }
#else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* EthIf_ControllerInit() */ /* PRQA S 6010,6030,6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  EthIf_SetControllerMode
 *********************************************************************************************************************/
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
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetControllerMode(
  uint8         CtrlIdx,
  Eth_ModeType  CtrlMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_NOT_OK;
  uint8           errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393588 */
    /* \trace SPEC-2393590 */
    /* \trace SPEC-2393604 */
    /* \trace SPEC-2393606 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    /* \trace SPEC-2393589 */
    /* \trace SPEC-2393605 */
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( (CtrlMode != ETH_MODE_ACTIVE) &&
           (CtrlMode != ETH_MODE_DOWN)      )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Enter exclusive area (Reason: Check and take semaphore that protects the function from executing
     *                                   multiple hardware mode change processes in parallel) */
    EthIf_EnterExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #40 Check if mode change protection semaphore is taken already */
    if( EthIf_CfgAccess_IsSetCtrlModeLocked() == FALSE )
    {
      /* #400 Take mode change protection semaphore */
      EthIf_CfgAccess_IsSetCtrlModeLocked() = TRUE;

      /* #410 Leave exclusive area -> semaphore was taken and mode change can be processed further */
      EthIf_ExitExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #420 Check if EthIf controller is initialized */
      if( EthIf_CfgAccess_EthIfCtrlInitMode(CtrlIdx) == ETHIF_INITIALIZED )
      {
        /* #4200 Check if a mode change to ACTIVE must be performed for the hardware elements */
        if( (CtrlMode == ETH_MODE_ACTIVE) &&
            (EthIf_CfgAccess_EthIfCtrlMode(CtrlIdx) == ETH_MODE_DOWN) )
        {
          /* #42000 Perform mode change from DOWN to ACTIVE */
#if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
          if( EthIf_IsModeRequestPossible(&EthIf_CfgAccess_VirtCtrl(CtrlIdx), ETH_MODE_ACTIVE) == TRUE )
#endif /* ETHIF_DEV_ERROR_DETECT */
          {
            retVal = EthIf_SetHardwareModeActive(&EthIf_CfgAccess_VirtCtrl(CtrlIdx));
          }
#if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
          else
          {
            errorId = ETHIF_E_INV_STATE;
          }
#endif /* ETHIF_DEV_ERROR_DETECT */
        }
        /* #4210 Else check if a mode change to DOWN must be performed for the hardware elements */
        else if( (CtrlMode == ETH_MODE_DOWN) &&
                 (EthIf_CfgAccess_EthIfCtrlMode(CtrlIdx) == ETH_MODE_ACTIVE) )
        {
          /* #42100 Perform mode change from ACTIVE to DOWN */
#if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
          if( EthIf_IsModeRequestPossible(&EthIf_CfgAccess_VirtCtrl(CtrlIdx), ETH_MODE_DOWN) == TRUE )
#endif /* ETHIF_DEV_ERROR_DETECT */
          {
            retVal = EthIf_SetHardwareModeDown(&EthIf_CfgAccess_VirtCtrl(CtrlIdx));
          }
#if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
          else
          {
            errorId = ETHIF_E_INV_STATE;
          }
#endif /* ETHIF_DEV_ERROR_DETECT */
        }
        /* #4220 Otherwise (no mode change) */
        else
        {
          /* #42200 No mode change necessary -> Mode ask for and current mode are equal */
          retVal = E_OK;
        }
      }

      /* #430 Release mode change protection semaphore */
      EthIf_CfgAccess_IsSetCtrlModeLocked() = FALSE;
    }
    /* #50 Otherwise (mode change protection semaphore already taken) */
    else
    {
      /* #500 Leave exclusive area -> semaphore was already taken */
      EthIf_ExitExclusiveArea(SET_CTRL_MODE); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

    /* #60 Check if mode change was successful and mode ask for was down */
    if( (retVal == E_OK) &&
        (CtrlMode == ETH_MODE_DOWN) )
    {
      /* #600 Immediately propagate the link state change to DOWN the upper layers */
      EthIf_ProcessLinkStateChange(CtrlIdx);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #70 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SET_CONTROLLER_MODE, errorId);
  }
#else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* EthIf_SetControllerMode() */ /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */

/**********************************************************************************************************************
 *  EthIf_GetControllerMode
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetControllerMode(
        uint8                                      CtrlIdx,
  P2VAR(Eth_ModeType, AUTOMATIC, ETHIF_APPL_DATA)  CtrlModePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_NOT_OK;
  uint8           errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393593 */
    /* \trace SPEC-2393596 */
    /* \trace SPEC-2393609 */
    /* \trace SPEC-2393612 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    /* \trace SPEC-2393594 */
    /* \trace SPEC-2393610 */
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( CtrlModePtr == NULL_PTR)
  {
    /* \trace SPEC-2393595 */
    /* \trace SPEC-2393611 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Retrieve the current EthIf controller mode and pass it to the caller */
    /* \trace SPEC-2393592 */
    /* \trace SPEC-2393608 */
    *CtrlModePtr = EthIf_CfgAccess_EthIfCtrlMode(CtrlIdx);
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_CONTROLLER_MODE, errorId);
  }
#else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* EthIf_GetControllerMode() */

/***********************************************************************************************************************
 *  EthIf_GetPhysAddr
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_GetPhysAddr(
        uint8                               CtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA)  PhysAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
    uint8 errorId = ETHIF_E_NO_ERROR;

    /* ----- Development Error Checks ------------------------------------- */
#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393615 */
    /* \trace SPEC-2393618 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    /* \trace SPEC-2393616 */
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( PhysAddrPtr == NULL_PTR )
  {
    /* \trace SPEC-2393617 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Redirect the call to the addressed Ethernet driver */
    P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl;

    virtCtrl = &EthIf_CfgAccess_VirtCtrl(CtrlIdx);

    /* \trace SPEC-2393614 */
    virtCtrl->EthCtrl->DrvApi->GetPhysAddr(virtCtrl->EthCtrl->CtrlIdx,
                                           PhysAddrPtr);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_PHYS_ADDR, errorId);
  }
#else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

} /* EthIf_GetPhysAddr() */

/**********************************************************************************************************************
 *  EthIf_SetPhysAddr
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_SetPhysAddr(
          uint8                               CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHIF_APPL_DATA)  PhysAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393621 */
    /* \trace SPEC-2393624 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    /* \trace SPEC-2393622 */
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( PhysAddrPtr == NULL_PTR )
  {
    /* \trace SPEC-2393623 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Redirect the call to the addressed Ethernet driver */
    P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl;

    virtCtrl = &EthIf_CfgAccess_VirtCtrl(CtrlIdx);

    EthIf_SrcMacAddrBuffer[virtCtrl->EthCtrl->CtrlIdx].IsValid = FALSE;

    /* \trace SPEC-2393620 */
    virtCtrl->EthCtrl->DrvApi->SetPhysAddr(virtCtrl->EthCtrl->CtrlIdx,
                                           PhysAddrPtr);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SET_PHYS_ADDR, errorId);
  }
#else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

} /* EthIf_SetPhysAddr() */

#if ( ETHIF_ENABLE_UPDATE_PHYS_ADDR_FILTER == STD_ON )
/**********************************************************************************************************************
 *  EthIf_UpdatePhysAddrFilter
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_UpdatePhysAddrFilter(
          uint8                                             CtrlIdx,
  P2CONST(uint8,               AUTOMATIC, ETHIF_APPL_DATA)  PhysAddrPtr,
          Eth_FilterActionType                              Action)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_NOT_OK;
  uint8           errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393627 */
    /* \trace SPEC-2393630 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    /* \trace SPEC-2393628 */
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( PhysAddrPtr == NULL_PTR)
  {
    /* \trace SPEC-2393629 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( (Action != ETH_ADD_TO_FILTER)      &&
           (Action != ETH_REMOVE_FROM_FILTER)    )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl;

    virtCtrl = &EthIf_CfgAccess_VirtCtrl(CtrlIdx);

    /* #30 Check if driver provides the API */
    if( virtCtrl->EthCtrl->DrvApi->UpdatePhysAddrFilter != NULL_PTR )
    {
      /* #300 Redirect call to the driver */
      /* \trace SPEC-2393626 */
      retVal = virtCtrl->EthCtrl->DrvApi->UpdatePhysAddrFilter(virtCtrl->EthCtrl->CtrlIdx,
                                                               PhysAddrPtr,
                                                               Action);
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
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_UPDATE_PHYS_ADDR_FILTER, errorId);
  }
#else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* EthIf_UpdatePhysAddrFilter() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif/* ETHIF_ENABLE_UPDATE_PHYS_ADDR_FILTER */

/**********************************************************************************************************************
 *  EthIf_ProvideTxBuffer
 *********************************************************************************************************************/
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
 *
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, ETHIF_CODE) EthIf_ProvideTxBuffer(
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
#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393635 */
    /* \trace SPEC-2393640 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    /* \trace SPEC-2393636 */
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
  else if( (EthIf_CfgAccess_VirtCtrl(CtrlIdx).Type == ETHIF_Q_TAG_NOT_TAGGED) &&
           (Priority                               != 0)                         )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else if( BufIdxPtr == NULL_PTR)
  {
    /* \trace SPEC-2393637 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( BufPtr == NULL_PTR)
  {
    /* \trace SPEC-2393638 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( LenBytePtr == NULL_PTR)
  {
    /* \trace SPEC-2393639 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( (*LenBytePtr) > EthIf_CfgAccess_Mtu(CtrlIdx) )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check if the EthIf controller is active */
    if( EthIf_CfgAccess_EthIfCtrlMode(CtrlIdx) == ETH_MODE_ACTIVE )
    {
      P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST)  virtCtrl;
#if ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_MIRRORING )
              uint16                                       llPayloadLen;
#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */

      virtCtrl = &EthIf_CfgAccess_VirtCtrl(CtrlIdx);

      /* #40 Check the EthIf controller type (non-tagged or tagged) and increase the needed buffer length
       *     by the amount of the tag if necessary */
      if( virtCtrl->Type == ETHIF_Q_TAG_TAGGED )
      {
        /* \trace SPEC-2393633 */
        (*LenBytePtr) += ETHIF_VLAN_TAG_LEN_BYTE;
      }

      /* #50 Redirect the buffer request call to the driver */
      /* \trace SPEC-2393634 */
      retVal = virtCtrl->EthCtrl->DrvApi->ProvideTxBuffer(virtCtrl->EthCtrl->CtrlIdx,
                                                          BufIdxPtr,
                                                          BufPtr,
                                                          LenBytePtr);

      /* #60 Check if buffer was successfully provided by the driver */
      if( retVal == BUFREQ_OK )
      {
        P2VAR(EthIf_TxContextType, AUTOMATIC, AUTOMATIC) txContext;
        /* #600 If traffic mirroring feature is enabled */
#if   ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_MIRRORING )
        P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) mirroringSrcPayloadPtr;
        /* #6000 Store needed data for mirroring the frame later on */
        mirroringSrcPayloadPtr = *BufPtr;
        llPayloadLen = *LenBytePtr;
#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */

        /* #610 Store Tx context for later use in EthIf_Transmit() and EthIf_TxConfirmation() */
        txContext = EthIf_CfgAccess_GetTxContextByContextTuple(virtCtrl->EthCtrl->CtrlIdx, (*BufIdxPtr));
        txContext->EthIfCtrlIdx       = CtrlIdx;
        txContext->FrameHdr.EtherType = FrameType;
        txContext->FrameHdr.Priority  = Priority;
        txContext->FrameHdr.VlanId    = virtCtrl->VID;
        txContext->IsTxPending        = FALSE;
        txContext->IsValid            = TRUE;

        /* #620 Check if the EthIf controller represents a tagged data path */
        /* \trace SPEC-2393632 */
        if( virtCtrl->Type == ETHIF_Q_TAG_TAGGED )
        { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
          /* #6210 Prepare VLAN tag */
          /* \trace SPEC-2393633 */
          /* \trace SPEC-2393569 */
          /* PRQA S 0310,3305 3 */ /* MD_EthIf_310_Buffer, MD_EthIf_3305_Buffer */
          (*BufPtr) = (P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA))EthIf_PrepareVlanTag((P2CONST(EthIf_FrameHdrType, AUTOMATIC, AUTOMATIC))&txContext->FrameHdr,
                                                                                            (P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA))*BufPtr,
                                                                                            LenBytePtr);
        }

        txContext->PayloadPtr = (P2VAR(uint8, AUTOMATIC, AUTOMATIC))(*BufPtr); /* PRQA S 0310,3305 */ /* MD_EthIf_310_Buffer, MD_EthIf_3305_Buffer */
        txContext->PayloadLen = (*LenBytePtr);

        /* #630 If switch time stamping feature is enabled */
#if ( ETHIF_ETHSWT_TIME_STAMP_SUPPORT == STD_ON )
        /* #6300 Store information of tx buffer provision to be able to restore context in
         *       EthIf_EnableSwitchEgressTimeStamp()/EthIf_SwitchIngressTimeStampIndication()/EthIf_SwitchEgressTimeStampIndication() */
        EthIf_StartTxSwtTimeStampInfoProc(virtCtrl->EthCtrl->CtrlIdx,
                                          CtrlIdx,
                                          (*BufIdxPtr),
                                          (*LenBytePtr),
                                          (P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA))(*BufPtr)); /* PRQA S 0310 */ /* MD_EthIf_310_Buffer */
#endif /* ETHIF_ETHSWT_TIME_STAMP_SUPPORT */

        /* #640 If traffic mirroring feature is enabled */
#if   ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_MIRRORING )
        /* #6400 Pass provide buffer request to mirroring sub component */
        EthIf_Mirroring_ProvideTxMirroringBuffer(CtrlIdx,
                                                 mirroringSrcPayloadPtr,
                                                 (*BufIdxPtr),
                                                 llPayloadLen);
#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #70 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_PROVIDE_TX_BUFFER, errorId);
  }
#else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* EthIf_ProvideTxBuffer() */ /* PRQA S 6030, 6060, 6080 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  EthIf_VTransmit
 *********************************************************************************************************************/
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
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_VTransmit(
        uint8                                      CtrlIdx,
        uint8                                      BufIdx,
        Eth_FrameType                              FrameType,
        boolean                                    TxConfirmation,
        uint16                                     LenByte,
  P2VAR(uint8,        AUTOMATIC, ETHIF_APPL_DATA)  DstMacAddrPtr,  /* PRQA S 3673 */ /* MD_EthIf_3673 */
  P2VAR(uint8,        AUTOMATIC, ETHIF_APPL_DATA)  SrcMacAddrPtr)  /* PRQA S 3673 */ /* MD_EthIf_3673 */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal   = E_NOT_OK;
  uint8           errorId  = ETHIF_E_NO_ERROR;
  uint8           ownerIdx = EthIf_QueryForFrameOwner(FrameType);

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393644 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx > EthIf_CfgAccess_MaxValidEthIfCtrlIdx() )
  {
    /* \trace SPEC-2393645 */
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( BufIdx >= EthIf_CfgAccess_MaxTxBufNum() )
  {
    /* \trace SPEC-2393646 */
    errorId = ETHIF_E_INV_PARAM;
  }
  else if( ownerIdx > EthIf_CfgAccess_MaxValidFrameOwnerIdx() )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else if( (TxConfirmation != TRUE) &&
           (TxConfirmation != FALSE)   )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else if( LenByte > EthIf_CfgAccess_Mtu(CtrlIdx) )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else if( DstMacAddrPtr == NULL_PTR )
  {
    /* \trace SPEC-2393647 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check if the EthIf controller is active */
    if( EthIf_CfgAccess_EthIfCtrlMode(CtrlIdx) == (Eth_ModeType)ETH_MODE_ACTIVE )
    {
      P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST)  virtCtrl;
      P2VAR(EthIf_TxContextType, AUTOMATIC, AUTOMATIC)     txContext;

      virtCtrl = &EthIf_CfgAccess_VirtCtrl(CtrlIdx);
      txContext = EthIf_CfgAccess_GetTxContextByContextTuple(virtCtrl->EthCtrl->CtrlIdx, BufIdx);

      /* \trace SPEC-2393648 */
      /* #40 Check the consistency of the tuple <CtrlIdx, BufIdx> is as returned by EthIf_ProvideTxBuffer() */
      if ( (txContext->IsValid == TRUE)         &&
           (txContext->IsTxPending == FALSE)    &&
           (txContext->EthIfCtrlIdx == CtrlIdx)    )
      {
        uint16             llEtherType;
        uint16             llPayloadLen;

        llEtherType            = FrameType;
        llPayloadLen           = LenByte;

        txContext->PayloadLen = LenByte;
        txContext->FrameHdr.DstMacAddrPtr = DstMacAddrPtr;
        if( SrcMacAddrPtr != NULL_PTR )
        {
          txContext->FrameHdr.SrcMacAddrPtr = SrcMacAddrPtr;
        }
        else
        {
          EthIf_RetrieveSrcMacAddr(txContext);
        }

        /* #400 Override Tx Confirmation call if buffer release is requested */
        if( LenByte == 0u )
        {
          TxConfirmation = FALSE;
        }

        /* #410 Check if Tx Confirmation is requested */
        if ( TxConfirmation == TRUE )
        {
# if (ETHIF_DEV_ERROR_DETECT == STD_OFF)
          /* #4100 Check if an owner was found */
          if ( ownerIdx <= EthIf_CfgAccess_MaxValidFrameOwnerIdx() )
# endif /* ETHIF_DEV_ERROR_DETECT */
          {
            /* #41000 Store owner context to be able to call correct user TxConfirmations in Tx Confirmation context */
            txContext->FrameOwnerIdx = ownerIdx;
          }
        }

        /* #420 Check the EthIf controller type (non-tagged or tagged) and if buffer release is requested and
         *      adapt the EtherType and payload length passed to driver if necessary */
        if( ( virtCtrl->Type == (EthIf_CtrlType)ETHIF_Q_TAG_TAGGED ) &&
            ( LenByte != (uint16)0u )                                            )
        {
          /* \trace SPEC-2393642 */
          llEtherType   = (uint16)ETHIF_VLAN_TPID;
          llPayloadLen += (uint16)ETHIF_VLAN_TAG_LEN_BYTE;
        }

        if( EthIf_IsTxAllowed(txContext) == TRUE )
        {
          /* #430 Setup tx context data for possible TxConfirmation. If not requested discard tx context. */
          if( TxConfirmation == TRUE )
          { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            txContext->IsTxPending = TRUE;
          }
          else
          { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            txContext->IsTxPending = FALSE;
            txContext->IsValid = FALSE;
          }

          /* #440 Redirect call to the driver */
          if( (virtCtrl->EthCtrl->DrvApi->VTransmit != NULL_PTR) &&
              (SrcMacAddrPtr != NULL_PTR)                              )
          { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            /* Vector specific VTransmit API (with ability to pass Source MAC address) is used */
            /* \trace SPEC-2393643 */
            retVal = virtCtrl->EthCtrl->DrvApi->VTransmit(virtCtrl->EthCtrl->CtrlIdx,
                                                          BufIdx,
                                                          llEtherType,
                                                          TxConfirmation,
                                                          llPayloadLen,
                                                          DstMacAddrPtr,
                                                          SrcMacAddrPtr);
          }
          else
          { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            /* AUTOSAR transmit API is used */
            /* \trace SPEC-2393643 */
            retVal = virtCtrl->EthCtrl->DrvApi->Transmit(virtCtrl->EthCtrl->CtrlIdx,
                                                         BufIdx,
                                                         llEtherType,
                                                         TxConfirmation,
                                                         llPayloadLen,
                                                         DstMacAddrPtr);
          }

          if( retVal == E_OK )
          { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            /* #450 Update transmission statistics */
            EthIf_UpdateTxStats(virtCtrl, LenByte);
            /* #460 If traffic mirroring feature is enabled */
#if   ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_MIRRORING )
            /* #4600 Pass frame to the mirroring sub component */
            EthIf_Mirroring_MirrorTxTraffic(CtrlIdx,
                                            BufIdx,
                                            llEtherType,
                                            llPayloadLen,
                                            DstMacAddrPtr);
#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */
          }
          else
          { /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            /* #470 Revert discard of tx context if triggering transmission failed */
            txContext->IsTxPending = FALSE;
            txContext->IsValid = TRUE;
          }
        }
        else
        {
          /* Transmission wasn't allowed - silently discard transmission */
          retVal = EthIf_TearDownTransmission(txContext, BufIdx, TxConfirmation);
        }

        if( retVal == E_OK )
        {
          /* #480 If switch time stamp feature is enabled */
#if ( ETHIF_ETHSWT_TIME_STAMP_SUPPORT == STD_ON )
          /* #4800 Store information of transmit trigger to be able to restore context in
           *       EthIf_SwitchIngressTimeStampIndication()/EthIf_SwitchEgressTimeStampIndication() */
          EthIf_FinishTxSwtTimeStampInfoProc(virtCtrl->EthCtrl->CtrlIdx,
                                             BufIdx,
                                             ownerIdx);
#endif /* ETHIF_ETHSWT_TIME_STAMP_SUPPORT */
        }
      }
#if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
      else
      {
        errorId = ETHIF_E_INV_PARAM;
      }
#endif /* ETHIF_DEV_ERROR_DETECT */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_TRANSMIT, errorId);
  }
#else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* EthIf_VTransmit() */ /* PRQA S 6010, 6030, 6050, 6060, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCAL, MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  EthIf_Transmit
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_Transmit(
        uint8                                      CtrlIdx,
        uint8                                      BufIdx,
        Eth_FrameType                              FrameType,
        boolean                                    TxConfirmation,
        uint16                                     LenByte,
  P2VAR(uint8,        AUTOMATIC, ETHIF_APPL_DATA)  PhysAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Redirect call to generalized transmit API */
  return EthIf_VTransmit(CtrlIdx,
                         BufIdx,
                         FrameType,
                         TxConfirmation,
                         LenByte,
                         PhysAddrPtr,
                         NULL_PTR);
} /* EthIf_Transmit() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  EthIf_RxIndication
 *********************************************************************************************************************/
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
 *
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_RxIndication(
        uint8                                      CtrlIdx,
        Eth_FrameType                              FrameType,
        boolean                                    IsBroadcast,
  P2VAR(uint8,        AUTOMATIC, ETHIF_APPL_DATA)  PhysAddrPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA)  DataPtr,
        uint16                                     LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393652 */
    /* \trace SPEC-2393657 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx >= EthIf_CfgAccess_EthCtrlNum() )
  {
    /* \trace SPEC-2393653 */
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( (IsBroadcast != FALSE) &&
           (IsBroadcast != TRUE)     )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else if( PhysAddrPtr == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( DataPtr == NULL_PTR )
  {
    /* \trace SPEC-2393654 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA)  u8DataPtr;
          uint8                               ethIfCtrlIdx;
          uint8                               ownerIdx;
          uint8                               additionalHdrLen;
          boolean                             isReceivedByUser;
          EthIf_FrameHdrType                  frameHdr;

#if ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_MIRRORING )
          Eth_FrameType                       llFrameType;
          uint16                              llPayloadLen;

    llFrameType  = FrameType;
    llPayloadLen = LenByte;
#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */

    /* PRQA S 0310,3305 1 */ /* MD_EthIf_310_Buffer, MD_EthIf_3305_Buffer */
    u8DataPtr              = (P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA))DataPtr;
    isReceivedByUser       = FALSE;
    frameHdr.SrcMacAddrPtr = (P2CONST(uint8, AUTOMATIC, AUTOMATIC))PhysAddrPtr;
    frameHdr.DstMacAddrPtr = &((P2CONST(uint8, AUTOMATIC, AUTOMATIC))PhysAddrPtr)[-(sint16)ETH_PHYS_ADDR_LEN_BYTE]; /* PRQA S 3382 */ /* MD_EthIf_3382 */
    frameHdr.EtherType     = FrameType;
    frameHdr.VlanId        = ETHIF_INV_VLAN_ID;
    additionalHdrLen       = 0;

    /* #30 Check if frame is a tagged frame */
    if( frameHdr.EtherType == ETHIF_VLAN_TPID )
    {
      /* #300 Extract tag information out of frame */
      frameHdr.VlanId  = (uint16)(((uint32)u8DataPtr[ETHIF_VLAN_TCI_OFFSET]) << 8U);
      frameHdr.VlanId |= (uint16)u8DataPtr[ETHIF_VLAN_TCI_OFFSET + 1U];
      frameHdr.VlanId &= (uint16)ETHIF_VLAN_TCI_VID_MASK;

      frameHdr.Priority = (uint8)(u8DataPtr[ETHIF_VLAN_TCI_OFFSET] >> ((sint8)ETHIF_VLAN_TCI_PCP_SHIFT - (sint8)8));

      frameHdr.EtherType  = (uint16)((uint32)u8DataPtr[ETHIF_VLAN_FRAME_TYPE_OFFSET] << 8U);
      frameHdr.EtherType |= (uint16)u8DataPtr[ETHIF_VLAN_FRAME_TYPE_OFFSET + 1U];

      additionalHdrLen = ETHIF_VLAN_TAG_LEN_BYTE;
    }

    if( EthIf_IsRxAllowed(CtrlIdx,
                          &ethIfCtrlIdx,
                          (P2CONST(EthIf_FrameHdrType, AUTOMATIC, AUTOMATIC))&frameHdr,
                          &u8DataPtr[additionalHdrLen],
                          (LenByte - additionalHdrLen)) == TRUE )
    {
#if ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_MIRRORING )
      /* #400 Pass the frame to the mirroring sub component if traffic mirroring feature is enabled */
      EthIf_Mirroring_MirrorRxTraffic(ethIfCtrlIdx,
                                      llFrameType,
                                      llPayloadLen,
                                      DataPtr);

#elif ( ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE == ETHIF_TRAFFIC_GATEWAY )
      /* #410 Pass the frame to the gateway sub component and evaluate if frame shall also be passed to upper layers
       *       if traffic gateway feature is enabled. */
      if( TRUE != EthIf_Gateway_GatewayTraffic(ethIfCtrlIdx,
                                               frameHdr.EtherType,
                                               LenByte - additionalHdrLen,
                                               DataPtr) )
#endif /* ETHIF_EXTENDED_TRAFFIC_HANDLING_TYPE */
      {
        /* #420 Pass frame to upper layers (in gateway feature variant dependent on the forwarding evaluation) */
        EthIf_UlRxIndiactionDataType rxIndData;

        rxIndData.EthIfCtrlIdx = ethIfCtrlIdx;
        rxIndData.EtherType    = frameHdr.EtherType;
        rxIndData.IsBroadcast  = IsBroadcast;
        rxIndData.SrcMacPtr    = PhysAddrPtr;
        rxIndData.PayloadPtr   = &u8DataPtr[additionalHdrLen];
        rxIndData.PayloadLen   = (LenByte - additionalHdrLen);

        ownerIdx = EthIf_QueryForFrameOwner(rxIndData.EtherType);

        isReceivedByUser = EthIf_HandleUlRxIndication(ownerIdx, &rxIndData);

        if( isReceivedByUser == TRUE )
        {
          /* #430 Update reception statistics */
          EthIf_UpdateRxStats(&EthIf_CfgAccess_VirtCtrl(ethIfCtrlIdx), rxIndData.PayloadLen);
# if ( ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT == STD_ON )
          /* #4300 If the upper layer registered for switch management information, initiate the management
           *       information provision process */
          if( (EthIf_IsOwnerRegisteredForSwtMgmtInfo(ownerIdx) == TRUE) &&
              (EthIf_CfgAccess_VirtCtrl(ethIfCtrlIdx).EthCtrl->IsHandlingSwtMgmtFrames == TRUE) )
          {
            EthIf_InitiateRxSwtMgmtInfoProc(CtrlIdx,
                                            ownerIdx,
                                            &rxIndData);
          }
# endif /* ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT */
# if ( ETHIF_ETHSWT_TIME_STAMP_SUPPORT == STD_ON )
          /* #4310 If the upper layer registered for switch timestamp information, initiate the timestamp
           *       information provision process */
          if( (EthIf_IsOwnerRegisteredForSwtTimeStamping(ownerIdx) == TRUE) &&
              (EthIf_CfgAccess_VirtCtrl(ethIfCtrlIdx).EthCtrl->IsHandlingSwtTsFrames == TRUE) )
          {
            EthIf_InitiateRxSwtTimeStampInfoProc(CtrlIdx,
                                                 ownerIdx,
                                                 &rxIndData);
          }
# endif /* ETHIF_ETHSWT_TIME_STAMP_SUPPORT */
        }
      }
    }

#if ( ETHIF_ENABLE_ZERO_COPY_EXTENSIONS == STD_ON )
    /* #50 Check if implicit buffer release must be triggered (if zero copy feature is enabled) */
    /* PRQA S 3415 2 */ /* MD_EthIf_3415 */
    if( (isReceivedByUser == FALSE) ||
        (EthIf_IsImplicitBufferReleaseNeeded(ethIfCtrlIdx, frameHdr.EtherType) == TRUE) )
    {
# if (ETHIF_DEV_ERROR_DETECT == STD_OFF)
      /* #500 Check if the Ethernet controller index passed by the function call is valid */
      if( CtrlIdx < EthIf_CfgAccess_EthCtrlNum() )
# endif /* ETHIF_DEV_ERROR_DETECT */
      {
        /* #5000 Release the RX buffer implicitly if Ethernet driver supports the feature too */
        P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST)  virtCtrl;

        virtCtrl = &EthIf_CfgAccess_VirtCtrlBase4EthCtrl(CtrlIdx);

        /* Check if driver supports the API */
        if( virtCtrl->EthCtrl->DrvApi->ReleaseRxBuffer != NULL_PTR )
        {
          /* PRQA S 0310, 3305 2 */ /* MD_EthIf_310_Buffer, MD_EthIf_3305_Buffer */
          (void)virtCtrl->EthCtrl->DrvApi->ReleaseRxBuffer(CtrlIdx,
                                                           (P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA))u8DataPtr);
        }
      }
    }
#else
    ETHIF_DUMMY_STATEMENT(isReceivedByUser); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif /* ETHIF_ENABLE_ZERO_COPY_EXTENSIONS */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #60 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_RX_INDICATION, errorId);
  }
#else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

} /* EthIf_RxIndication() */ /* PRQA S 6010, 6030, 6050, 6060, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  EthIf_TxConfirmation
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_TxConfirmation(
    uint8  CtrlIdx,
    uint8  BufIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393660 */
    /* \trace SPEC-2393663 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( CtrlIdx >= EthIf_CfgAccess_EthCtrlNum() )
  {
    /* \trace SPEC-2393661 */
    errorId = ETHIF_E_INV_CTRL_IDX;
  }
  else if( BufIdx >= EthIf_CfgAccess_MaxTxBufNum() )
  {
    /* \trace SPEC-2393662 */
    errorId = ETHIF_E_INV_PARAM;
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(EthIf_TxContextType, AUTOMATIC, AUTOMATIC) txContext;

    txContext = EthIf_CfgAccess_GetTxContextByContextTuple(CtrlIdx, BufIdx);

    /* #30 Check if a Tx confirmation was requested for the buffer */
    if ( txContext->IsValid == TRUE )
    {
      uint8_least loopIdx;

      /* #300 Call Tx confirmation of users */
      /* \trace SPEC-2393682 */
      for( loopIdx = 0; loopIdx < EthIf_CfgAccess_TxConfInfo(txContext->FrameOwnerIdx).OwnerTxConfirmationNum; loopIdx++)
      {
        EthIf_CfgAccess_TxConfInfo(txContext->FrameOwnerIdx).OwnerTxConfirmations[loopIdx](txContext->EthIfCtrlIdx, BufIdx);
      }
      /* #310 Invalidate the Tx context */
      txContext->IsTxPending = FALSE;
      txContext->IsValid = FALSE;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_TX_CONFIRMATION, errorId);
  }
#else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

} /* EthIf_TxConfirmation() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if ( ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_SwitchPortGroupRequestMode
 *********************************************************************************************************************/
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
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchPortGroupRequestMode(
  EthIf_SwitchPortGroupIdxType PortGroupIdx,
  EthTrcv_ModeType PortMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393861 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( PortGroupIdx >= EthIf_CfgAccess_EthIfSwtPortGroupNum() )
  {
    errorId = ETHIF_E_INV_PORT_GROUP_IDX;
  }
  else if ( EthIf_SwtPortGroupSummary[PortGroupIdx]->PortGroupSemantic != ETHIF_SWTPORTGROUP_SEMANTIC_INDEPENDENT )
  {
    errorId = ETHIF_E_INV_PORT_GROUP_IDX;
  }
  else if( (PortMode != ETHTRCV_MODE_DOWN) &&
           (PortMode != ETHTRCV_MODE_ACTIVE) )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else if( EthIf_IsModeRequestPossibleForSwtPortGroup(EthIf_CfgAccess_EthIfSwtPortGroupSummary(PortGroupIdx),
              ((PortMode == ETHTRCV_MODE_ACTIVE) ? ETH_MODE_ACTIVE : ETH_MODE_DOWN)) != TRUE )
  {
    errorId = ETHIF_E_INV_STATE;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check if a mode change to ACTIVE or DOWN has to be performed */
    if ( PortMode != EthIf_CfgAccess_EthIfSwtPortGroupMode(PortGroupIdx) )
    {
      /* #300 Switch mode of ports in Port Group with related PortGroupIdx */
      CONSTP2CONST(EthIf_EthSwtPortGroupType, AUTOMATIC, ETHIF_CONST) ethSwtPortGroup =
          EthIf_SwtPortGroupSummary[PortGroupIdx];
      retVal = EthIf_SetSwitchPortGroupMode(ethSwtPortGroup, PortMode);

      /* #310 Check if switching of mode for the Port Group Ports was successful */
      if ( retVal == E_OK )
      {
        /* #3000 Adjusting mode counter values of the ports in the Port Group */
        EthIf_AdjustSwitchPortGroupModeCount(ethSwtPortGroup, PortMode);

        /* #3100 Check if requested mode was DOWN */
        if( PortMode == ETHTRCV_MODE_DOWN )
        {
          /* #31000 Immediately propagate the link state change to DOWN the upper layers */
          EthIf_ProcessSwtPortGroupLinkStateChange(PortGroupIdx);
        }
      }
    }
    else
    {
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SWITCH_PORT_GROUP_REQUEST_MODE, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* EthIf_SwitchPortGroupRequestMode() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT */

#if ( ETHIF_WAKEUP_SUPPORT == STD_ON )
/**********************************************************************************************************************
 * EthIf_SetTransceiverWakeupMode
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetTransceiverWakeupMode(
  EcuM_WakeupSourceType   WakeupSource,
  EthTrcv_WakeupModeType  WakeupMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal       = E_NOT_OK;
  uint8          errorId      = ETHIF_E_NO_ERROR;
  uint8          wakeupMapIdx = EthIf_QueryForWakeupMap(WakeupSource);

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393861 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( wakeupMapIdx > EthIf_CfgAccess_MaxValidWakeupMapIdx() )
  {
    /* \trace SPEC-2393862 */
    errorId = ETHIF_E_INV_PARAM;
  }
  else if( (WakeupMode != ETHTRCV_WUM_DISABLE) &&
           (WakeupMode != ETHTRCV_WUM_ENABLE)  &&
           (WakeupMode != ETHTRCV_WUM_CLEAR)      )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_EthTrcvType, ETHIF_CONST, ETHIF_CONST) ethTrcv;

    ethTrcv = EthIf_CfgAccess_WakeupMap(wakeupMapIdx).EthTrcv;

    /* #30 Check if driver provides the API */
    if( ethTrcv->DrvApi->SetTransceiverWakeupMode != NULL_PTR )
    {
      /* #300 Redirect call to the driver */
      /* \trace SPEC-2393949 */
      retVal = ethTrcv->DrvApi->SetTransceiverWakeupMode(ethTrcv->TrcvIdx,
                                                         WakeupMode);
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
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SET_TRANSCEIVER_WAKEUP_MODE, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* EthIf_SetTransceiverWakeupMode() */
 
/**********************************************************************************************************************
 * EthIf_GetTransceiverWakeupMode
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetTransceiverWakeupMode(
        EcuM_WakeupSourceType                               WakeupSource,
  P2VAR(EthTrcv_WakeupModeType, AUTOMATIC, ETHIF_APPL_VAR)  WakeupModePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal       = E_NOT_OK;
  uint8          errorId      = ETHIF_E_NO_ERROR;
  uint8          wakeupMapIdx = EthIf_QueryForWakeupMap(WakeupSource);

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393864 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( wakeupMapIdx > EthIf_CfgAccess_MaxValidWakeupMapIdx() )
  {
    /* \trace SPEC-2393865 */
    errorId = ETHIF_E_INV_PARAM;
  }
  else if( WakeupModePtr == NULL_PTR )
  {
    /* \trace SPEC-2393866 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_EthTrcvType, ETHIF_CONST, ETHIF_CONST) ethTrcv;

    ethTrcv = EthIf_CfgAccess_WakeupMap(wakeupMapIdx).EthTrcv;

    /* #30 Check if driver provides the API */
    if( ethTrcv->DrvApi->GetTransceiverWakeupMode != NULL_PTR )
    {
      /* #300 Redirect call to the driver */
      /* \trace SPEC-2393951 */
      retVal = ethTrcv->DrvApi->GetTransceiverWakeupMode(ethTrcv->TrcvIdx,
                                                         WakeupModePtr);
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
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_TRANSCEIVER_WAKEUP_MODE, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
  
  return retVal;
} /* EthIf_GetTransceiverWakeupMode() */

/**********************************************************************************************************************
 * EthIf_CheckWakeup
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_CheckWakeup(
  EcuM_WakeupSourceType  WakeupSource)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8          errorId      = ETHIF_E_NO_ERROR;
  uint8          wakeupMapIdx = EthIf_QueryForWakeupMap(WakeupSource);

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check module states */
  if( EthIf_ModuleInitialized == ETHIF_STATE_UNINIT )
  {
    /* \trace SPEC-2393869 */
    /* \trace SPEC-2393872 */
    errorId = ETHIF_E_NOT_INITIALIZED;
  }
  /* #20 Check input parameters for plausibility */
  else if( wakeupMapIdx > EthIf_CfgAccess_MaxValidWakeupMapIdx() )
  {
    /* \trace SPEC-2393870 */
    errorId = ETHIF_E_INV_PARAM;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_EthTrcvType, ETHIF_CONST, ETHIF_CONST) ethTrcv;

    ethTrcv = EthIf_CfgAccess_WakeupMap(wakeupMapIdx).EthTrcv;

    /* #30 Check if driver provides the API */
    if( ethTrcv->DrvApi->CheckWakeup != NULL_PTR )
    {
      /* #300 Redirect call to the driver */
      /* \trace SPEC-2393952 */
      (void)ethTrcv->DrvApi->CheckWakeup(ethTrcv->TrcvIdx);
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
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_CHECK_WAKEUP, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

} /* EthIf_CheckWakeup() */

/**********************************************************************************************************************
 * EthIf_VSetTransceiverWakeupMode
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_VSetTransceiverWakeupMode(
  uint8                   CtrlIdx,
  EthTrcv_WakeupModeType  WakeupMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

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
  else if( (WakeupMode != ETHTRCV_WUM_DISABLE) &&
           (WakeupMode != ETHTRCV_WUM_ENABLE)  &&
           (WakeupMode != ETHTRCV_WUM_CLEAR)      )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 ethTrcvPhyElemIdx = EthIf_QueryForWakeupTransceiver(CtrlIdx);

    /* #30 Check if wakeup transceiver for EthIf controller exists */
    if( ethTrcvPhyElemIdx != ETHIF_INVALID_WAKEUP_ETHTRCV )
    {
      P2CONST(EthIf_VirtCtrlType, AUTOMATIC,   ETHIF_CONST) virtCtrl;
      P2CONST(EthIf_EthTrcvType,  ETHIF_CONST, ETHIF_CONST) ethTrcv;

      virtCtrl = &EthIf_CfgAccess_VirtCtrl(CtrlIdx);
      /* PRQA S 310 1 */ /* MD_EthIf_310_PhyLayer */
      ethTrcv = ((P2CONST(EthIf_PhyEthTrcvType, ETHIF_CONST, ETHIF_CONST))&virtCtrl->PhyLayer->PhyLayerInitElements[ethTrcvPhyElemIdx])->EthTrcv;

      /* #300 Check if driver provides the API */
      if( ethTrcv->DrvApi->SetTransceiverWakeupMode != NULL_PTR )
      {
        /* #3000 Redirect call to the driver */
        retVal = ethTrcv->DrvApi->SetTransceiverWakeupMode(ethTrcv->TrcvIdx,
                                                           WakeupMode);
      }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
      else
      {
        errorId = ETHIF_E_INV_DRIVER_API_CALL;
      }
# endif /* ETHIF_DEV_ERROR_DETECT */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_PARAM;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SET_TRANSCEIVER_WAKEUP_MODE, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* EthIf_VSetTransceiverWakeupMode() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * EthIf_VGetTransceiverWakeupMode
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
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_VGetTransceiverWakeupMode(
        uint8                                               CtrlIdx,
  P2VAR(EthTrcv_WakeupModeType, AUTOMATIC, ETHIF_APPL_VAR)  WakeupModePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

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
  else if( WakeupModePtr == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 ethTrcvPhyElemIdx = EthIf_QueryForWakeupTransceiver(CtrlIdx);

    /* #30 Check if wakeup transceiver for EthIf controller exists */
    if( ethTrcvPhyElemIdx != ETHIF_INVALID_WAKEUP_ETHTRCV )
    {
      P2CONST(EthIf_VirtCtrlType, AUTOMATIC,   ETHIF_CONST) virtCtrl;
      P2CONST(EthIf_EthTrcvType,  ETHIF_CONST, ETHIF_CONST) ethTrcv;

      virtCtrl = &EthIf_CfgAccess_VirtCtrl(CtrlIdx);
      /* PRQA S 310 1 */ /* MD_EthIf_310_PhyLayer */
      ethTrcv = ((P2CONST(EthIf_PhyEthTrcvType, ETHIF_CONST, ETHIF_CONST))&virtCtrl->PhyLayer->PhyLayerInitElements[ethTrcvPhyElemIdx])->EthTrcv;

      /* #300 Check if driver provides the API */
      if( ethTrcv->DrvApi->GetTransceiverWakeupMode != NULL_PTR )
      {
        /* #3000 Redirect call to the driver */
        retVal = ethTrcv->DrvApi->GetTransceiverWakeupMode(ethTrcv->TrcvIdx,
                                                           WakeupModePtr);
      }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
      else
      {
        errorId = ETHIF_E_INV_DRIVER_API_CALL;
      }
# endif /* ETHIF_DEV_ERROR_DETECT */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_PARAM;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_TRANSCEIVER_WAKEUP_MODE, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* EthIf_VGetTransceiverWakeupMode() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * EthIf_VCheckWakeup
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
FUNC(void, ETHIF_CODE) EthIf_VCheckWakeup(
  uint8  CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8          errorId = ETHIF_E_NO_ERROR;

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
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 ethTrcvPhyElemIdx = EthIf_QueryForWakeupTransceiver(CtrlIdx);

    /* #30 Check if wakeup transceiver for EthIf controller exists */
    if( ethTrcvPhyElemIdx != ETHIF_INVALID_WAKEUP_ETHTRCV )
    {
      P2CONST(EthIf_VirtCtrlType, AUTOMATIC,   ETHIF_CONST) virtCtrl;
      P2CONST(EthIf_EthTrcvType,  ETHIF_CONST, ETHIF_CONST) ethTrcv;

      virtCtrl = &EthIf_CfgAccess_VirtCtrl(CtrlIdx);
      /* PRQA S 310 1 */ /* MD_EthIf_310_PhyLayer */
      ethTrcv = ((P2CONST(EthIf_PhyEthTrcvType, ETHIF_CONST, ETHIF_CONST))&virtCtrl->PhyLayer->PhyLayerInitElements[ethTrcvPhyElemIdx])->EthTrcv;

      /* #300 Check if driver provides the API */
      if( ethTrcv->DrvApi->CheckWakeup != NULL_PTR )
      {
        /* #3000 Redirect call to the driver */
        (void)ethTrcv->DrvApi->CheckWakeup(ethTrcv->TrcvIdx);
      }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
      else
      {
        errorId = ETHIF_E_INV_DRIVER_API_CALL;
      }
# endif /* ETHIF_DEV_ERROR_DETECT */
    }
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      errorId = ETHIF_E_INV_PARAM;
    }
# endif /* ETHIF_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_CHECK_WAKEUP, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

} /* EthIf_VCheckWakeup() */

#endif /* ETHIF_WAKEUP_SUPPORT */

#if ( ETHIF_DYNAMIC_TRAFFIC_SHAPING_SUPPORT == STD_ON )
/**********************************************************************************************************************
 * EthIf_SetBandwidthLimit
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetBandwidthLimit(
  uint8   CtrlIdx,
  uint8   QueuePrio,
  uint32  BandwidthLimit)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

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
  else if( QueuePrio > ETHIF_VLAN_PCP_MAX_VAL )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl;

    virtCtrl = &EthIf_CfgAccess_VirtCtrl(CtrlIdx);

    /* #30 Check if driver provides the API */
    if( virtCtrl->EthCtrl->DrvApi->SetBandwidthLimit != NULL_PTR )
    {
      /* #3000 Redirect call to the driver */
      retVal = virtCtrl->EthCtrl->DrvApi->SetBandwidthLimit(virtCtrl->EthCtrl->CtrlIdx,
                                                            QueuePrio,
                                                            BandwidthLimit);
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
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_SET_BANDWIDTH_LIMIT, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* EthIf_SetBandwidthLimit() */

/**********************************************************************************************************************
 * EthIf_GetBandwidthLimit
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetBandwidthLimit(
        uint8                               CtrlIdx,
        uint8                               QueuePrio,
  P2VAR(uint32, AUTOMATIC, ETHIF_APPL_VAR)  BandwidthLimitPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

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
  else if( QueuePrio > ETHIF_VLAN_PCP_MAX_VAL )
  {
    errorId = ETHIF_E_INV_PARAM;
  }
  else if( BandwidthLimitPtr == NULL_PTR )
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
    if( virtCtrl->EthCtrl->DrvApi->GetBandwidthLimit != NULL_PTR )
    {
      /* #3000 Redirect call to the driver */
      retVal = virtCtrl->EthCtrl->DrvApi->GetBandwidthLimit(virtCtrl->EthCtrl->CtrlIdx,
                                                            QueuePrio,
                                                            BandwidthLimitPtr);
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
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_BANDWIDTH_LIMIT, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* EthIf_GetBandwidthLimit() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

#endif /* ETHIF_DYNAMIC_TRAFFIC_SHAPING_SUPPORT */

/**********************************************************************************************************************
 * EthIf_GetTxStats
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
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetTxStats(
        uint8                                    CtrlIdx,
  P2VAR(Eth_TxStatsType,   AUTOMATIC, AUTOMATIC) EthTxStats,
  P2VAR(EthIf_TxStatsType, AUTOMATIC, AUTOMATIC) EthIfTxStats)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

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
  else if( EthTxStats == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( EthIfTxStats == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl;

    virtCtrl = &EthIf_CfgAccess_VirtCtrl(CtrlIdx);

    /* #30 Enter exclusive area (Reason: Retrieval of tx statistic counters in a consistent manner) */
    EthIf_EnterExclusiveArea(RXTX_STATS); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #40 Retrieve the statistic counters of the EthIf-controller and clear them */
    EthIfTxStats->NumTxPkts  = virtCtrl->TxStats->NumTxPkts;
    EthIfTxStats->NumTxBytes = virtCtrl->TxStats->NumTxBytes;
    EthIf_ClearTxStats(virtCtrl);

    /* #50 Leave exclusive area */
    EthIf_ExitExclusiveArea(RXTX_STATS); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    if( virtCtrl->EthCtrl->DrvApi->GetTxStats != NULL_PTR )
    {
      /* #60 If the driver supports the API, retrieve the statistic counters of the respective
       *     Eth-controller by calling the driver*/
      retVal = virtCtrl->EthCtrl->DrvApi->GetTxStats(virtCtrl->EthCtrl->CtrlIdx, EthTxStats);
    }
    else
    {
      /* #70 Otherwise invalidate the driver specific counters */
      EthTxStats->TxNumberOfOctets = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthTxStats->TxNUcastPkts     = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthTxStats->TxUniCastPkts    = ETH_RXTX_STATS_INV_COUNTER_VAL;
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #80 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_TX_STATS, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* EthIf_GetTxStats() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * EthIf_GetRxStats
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
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetRxStats(
        uint8                                    CtrlIdx,
  P2VAR(Eth_RxStatsType,   AUTOMATIC, AUTOMATIC) EthRxStats,
  P2VAR(EthIf_RxStatsType, AUTOMATIC, AUTOMATIC) EthIfRxStats)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = ETHIF_E_NO_ERROR;

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
  else if( EthRxStats == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else if( EthIfRxStats == NULL_PTR )
  {
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
# endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2CONST(EthIf_VirtCtrlType, AUTOMATIC, ETHIF_CONST) virtCtrl;

    virtCtrl = &EthIf_CfgAccess_VirtCtrl(CtrlIdx);

    /* #30 Enter exclusive area (Reason: Retrieval of rx statistic counters in a consistent manner) */
    EthIf_EnterExclusiveArea(RXTX_STATS); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #40 Retrieve the statistic counters of the EthIf-controller*/
    EthIfRxStats->NumRxPkts  = virtCtrl->RxStats->NumRxPkts;
    EthIfRxStats->NumRxBytes = virtCtrl->RxStats->NumRxBytes;
    EthIf_ClearRxStats(virtCtrl);

    /* #50 Leave exclusive area */
    EthIf_ExitExclusiveArea(RXTX_STATS); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    if( virtCtrl->EthCtrl->DrvApi->GetRxStats != NULL_PTR )
    {
      /* #60 If the driver supports the API, retrieve the statistic counters of the respective
       *     Eth-controller by calling the driver*/
      retVal = virtCtrl->EthCtrl->DrvApi->GetRxStats(virtCtrl->EthCtrl->CtrlIdx, EthRxStats);
    }
    else
    {
      /* #70 Otherwise invalidate the driver specific counters */
      EthRxStats->RxStatsDropEvents           = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxStatsOctets               = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxStatsPkts                 = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxStatsBroadcastPkts        = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxStatsMulticastPkts        = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxStatsCrcAlignErrors       = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxStatsUndersizePkts        = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxStatsOversizePkts         = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxStatsFragments            = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxStatsJabbers              = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxStatsCollisions           = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxStatsPkts64Octets         = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxStatsPkts65to127Octets    = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxStatsPkts128to255Octets   = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxStatsPkts256to511Octets   = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxStatsPkts512to1023Octets  = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxStatsPkts1024to1518Octets = ETH_RXTX_STATS_INV_COUNTER_VAL;
      EthRxStats->RxUnicastFrames             = ETH_RXTX_STATS_INV_COUNTER_VAL;
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #80 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_RX_STATS, errorId);
  }
# else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif

  return retVal;
} /* EthIf_GetRxStats() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  EthIf_MainFunctionRx
 *********************************************************************************************************************/
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
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_MainFunctionRx( void )
{
  /* #10 If feature receive polling is enabled */
  /* \trace SPEC-2393667 */
#if ( ETHIF_ENABLE_RX_INTERRUPT == STD_OFF )
  /* #20 Check if module is initialized */
  if ( EthIf_ModuleInitialized == ETHIF_STATE_INIT )
  {
    /* maximum amount of frames to be received during this run */
    uint16  maxFrameRx;
    /* abort criterion if no additional frames are present */
    boolean moreFramesPresent;

    maxFrameRx = ETHIF_RX_INDICATION_ITERATIONS;

    /* #30 Poll for frames to be received until no more frames are available
     *     or the maximum count of frames per main function is received */
    do
    {
      uint8_least  ethCtrlIdx;

      moreFramesPresent = FALSE;
      maxFrameRx--;

      /* #300 Receive polling for each Eth controller used */
      /* (this is done for each run of the outer loop to serve the controllers equally and
       *  prevent one controller from stressing the reception so the others wouldn't be served)*/
      for( ethCtrlIdx = 0; ethCtrlIdx < EthIf_CfgAccess_EthCtrlNum(); ethCtrlIdx++ )
      {
        P2CONST(EthIf_EthCtrlType, AUTOMATIC, ETHIF_CONST) ethCtrl;

        ethCtrl = EthIf_CfgAccess_EthCtrlSummary(ethCtrlIdx);

        /* #3000 Check if controller is active */
        if( ethCtrl->ModeCnt[0] > 0u )
        {
          Eth_RxStatusType rxStatus = ETH_NOT_RECEIVED;

          /* #30000 Trigger frame reception */
          ethCtrl->DrvApi->Receive(ethCtrl->CtrlIdx,
                                   &rxStatus);

          /* #30010 Check if an additional run must be performed and trigger run if necessary */
          if( rxStatus == ETH_RECEIVED_MORE_DATA_AVAILABLE )
          {
              /* there are more frames present */
              moreFramesPresent = TRUE;
          }
        }
      }
    }
    while (    (maxFrameRx > 0u)              /* max amount of frames to be received exceeded yet? */
            && (moreFramesPresent == TRUE) ); /* more frames available for reception? */
    /* \trace SPEC-2393668 */
  }
#endif /* ETHIF_ENABLE_RX_INTERRUPT */
} /* EthIf_MainFunctionRx() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  EthIf_MainFunctionTx
 *********************************************************************************************************************/
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
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_MainFunctionTx( void )
{
  /* #10 If feature transmission confirmation polling is enabled */
  /* \trace SPEC-2393671 */
#if ( ETHIF_ENABLE_TX_INTERRUPT == STD_OFF )
  /* #20 Check if module is initialized */

  if ( EthIf_ModuleInitialized == ETHIF_STATE_INIT )
  {
    uint8_least  ethCtrlIdx;

    /* #200 Tx confirmation polling for each Eth controller used */
    /* \trace SPEC-2393681 */
    for( ethCtrlIdx = 0; ethCtrlIdx < EthIf_CfgAccess_EthCtrlNum(); ethCtrlIdx++ )
    {
      P2CONST(EthIf_EthCtrlType, AUTOMATIC, ETHIF_CONST) ethCtrl;

      ethCtrl = EthIf_CfgAccess_EthCtrlSummary(ethCtrlIdx);

      /* #2000 Check if controller is active */
      if( ethCtrl->ModeCnt[0] > 0u )
      {
        /* #20000 Trigger tx confirmation */
        ethCtrl->DrvApi->TxConfirmation(ethCtrl->CtrlIdx);
      }
    }
  }
#endif /* ETHIF_ENABLE_TX_INTERRUPT */

  /* #30 If feauture explicit main function state is disabled */
#if ( ETHIF_ENABLE_MAIN_FUNCTION_STATE == STD_OFF )
  /* #40 Check if main function state has to be called */
  /* \trace SPEC-2393672 */
  if( EthIf_MainFuncStateFreqCnt == 0 )
  {
    /* #400 Call main function state and reset polling frequency counter */
    EthIf_MainFunctionState();
    /* Restart counter */
    EthIf_MainFuncStateFreqCnt = EthIf_CfgAccess_TrcvLinkStateChgReload();
  }
  /* #50 Decrease polling frequency counter */
  EthIf_MainFuncStateFreqCnt--;
#endif /* ETHIF_ENABLE_MAIN_FUNCTION_STATE */
} /* EthIf_MainFunctionTx() */

/**********************************************************************************************************************
 *  EthIf_MainFunctionState
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_MainFunctionState( void )
{
  /* #10 Check if module is initialized */
  if( EthIf_ModuleInitialized == ETHIF_STATE_INIT )
  {
    /* #100 Get all relevant links of transceivers and switch ports */
    EthIf_DetectLinkStateChange();

    /* #110 Process possible link changes */
    EthIf_ProcessLinkStateChangeForAllEthIfCtrls();

#if ( ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT == STD_ON )
    /* #120 Process possible link changes for EthIf switch port groups */
    EthIf_ProcessAllEthIfSwtPortGroupLinkStateChanges();
#endif /* ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT */
  }
} /* EthIf_MainFunctionState() */

#if ( ETHIF_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  EthIf_GetVersionInfo
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ETHIF_APPL_DATA)  VersionInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check input parameters for plausibility */
  if( VersionInfoPtr == NULL_PTR )
  {
    /* \trace SPEC-2393650 */
    errorId = ETHIF_E_INV_PARAM_POINTER;
  }
  else
#endif /* ETHIF_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Retrieve version information */
    VersionInfoPtr->vendorID = ETHIF_VENDOR_ID;
    VersionInfoPtr->moduleID = ETHIF_MODULE_ID;
    VersionInfoPtr->sw_major_version = ETHIF_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = ETHIF_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = ETHIF_SW_PATCH_VERSION;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (ETHIF_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report default errors if any occurred */
  if (errorId != ETHIF_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_INSTANCE_ID_DET, ETHIF_SID_GET_VERSION_INFO, errorId);
  }
#else
  ETHIF_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

} /* EthIf_GetVersionInfo() */
#endif /* ETHIF_VERSION_INFO_API */

#define ETHIF_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* module specific MISRA deviations:

  MD_EthIf_3673:
      Reason:     Type of parameter is specified by AUTOSAR and cannot be changed to const.
      Risk:       Caller of function isn't able to optimize memory usage by placing the respective variable in
                  ROM.
      Prevention: No need for prevention.

  MD_EthIf_310_Buffer:
      Reason:     Cast from uint32 to uint8 for easer access to byte fields.
      Risk:       Depending on the buffer, an unaligned memory exception may occur on certain platforms.
      Prevention: Buffers passed to the function need to be 4 byte aligned.

  MD_EthIf_310_PhyLayer:
      Reason:     In the physical layer array multiple data objects of different types are hold. For accessing the
                  desired data the data object must be cast into the correct type.
      Risk:       Risk of accessing and interpreting data not intended.
      Prevention: One member of the 'abstract' structure denotes what is the type of the underlying data. The combination
                  of retrieving the type by the structure member and the subsequent cast is checked by code review.

  MD_EthIf_3305_Buffer:
      Reason:     API of lower layer uses Eth_DataType as type of pointer to the data buffer and therefore the cast
                  is needed to pass the data.
      Risk:       Risk of an unaligned memory exception.
      Prevention: Covered by code review.

  MD_EthIf_3415:
     Reason:     Optimization of evaluation of if condition. The function call shall only be done if first condition
                 isn't true.
     Risk:       Because function doesn't alter any state of the module but is a second condition that must only be
                 checked if the first one isn't fulfilled the lazy evaluation applied by the compiler has no effect.
     Prevention: No need for prevention.

  MD_EthIf_3382:
     Reason:     AUTOSAR API passes the pointer to the payload portion relevant for the upper layer. EthIf however
                 must pass the pointer to the Ethernet payload portion to the Ethernet driver and must therefore
                 take the VLAN-tag (which is transparent for the upper layer) into account. This results in the need
                 to adapt the pointer in case of a included Q-Tag by negative pointer arithmetic.
     Risk:       If upper layer passes a pointer to a payload embedded into an Ethernet frame not including a Q-Tag
                 will result in passing a wrong pointer to the Ethernet driver.
     Prevention: Must be covered by code review/run time tests of upper layer.
*/

/**********************************************************************************************************************
 *  END OF FILE: EthIf.c
 *********************************************************************************************************************/

