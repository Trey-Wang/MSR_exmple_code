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
/**        \file  Eth_30_Vtt.h
 *        \brief  Public header file of the Eth_30_Vtt Ethernet controller driver
 *
 *      \details  Provides access to the public API of the Ethernet controller driver.
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  David Fessler                 visfer        Vector Informatik GmbH
 *  Mark Harsch                   vismha        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2016-10-04  visfer  -             Initial creation
 *  01.00.01  2016-11-28  visfer  ESCAN00093063 Transmission stops when upper layers request to free several 
 *                                              provided buffers
 *  01.00.01  2016-11-28  visfer  ESCAN00093163 Compiler error due to wrong API infixing in compiler_cfg.h and memmap.h
 *  02.00.00  2017-02-15  vismha  ESCAN00094012 Changes due to DrvEth_VttEthAsr migration
 *                        visfer  ESCAN00093414 Pre- and Post-ControllerInit Callouts are missing
 *                        visfer  ESCAN00093412 Review Integration and refacturing of code, comments and detailed design
 *                        visfer  ESCAN00093704 Compiler error: undeclared identifiers 
 *                                              Eth_30_Vtt_MulticastBucketCounters, Eth_30_Vtt_PromiscuousModeCounter
 *********************************************************************************************************************/

# if !defined(ETH_30_VTT_H)
# define     ETH_30_VTT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Eth_30_Vtt_Types.h"
# if (ETH_30_VTT_ENABLE_PTP == STD_ON)
#  include "Eth_30_Vtt_Time.h"
# endif /* ETH_30_VTT_ENABLE_PTP */

# include "SchM_Eth_30_Vtt.h"
/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */
/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* AUTOSAR 4.x Software Specification Version Information */
# define ETH_AR_RELEASE_MAJOR_VERSION           (0x04U)
# define ETH_AR_RELEASE_MINOR_VERSION           (0x01U)
# define ETH_AR_RELEASE_REVISION_VERSION        (0x01U)

/* ETH ModuleId */
/* Vector ID */
# define ETH_VENDOR_ID                          (30U)
/* Ethernet Controller Driver ID */
# define ETH_MODULE_ID                          (88U)

/* ETH ApiIds */
# define ETH_30_VTT_SID_INIT                        (0x01U) /*!< Service ID: Eth_30_Vtt_Init() */
# define ETH_30_VTT_SID_CONTROLLER_INIT             (0x02U) /*!< Service ID: Eth_30_Vtt_ControllerInit() */
# define ETH_30_VTT_SID_SET_CONTROLLER_MODE         (0x03U) /*!< Service ID: Eth_30_Vtt_SetControllerMode() */
# define ETH_30_VTT_SID_GET_CONTROLLER_MODE         (0x04U) /*!< Service ID: Eth_30_Vtt_GetControllerMode() */
# define ETH_30_VTT_SID_WRITE_MII                   (0x05U) /*!< Service ID: Eth_30_Vtt_WriteMii() */
# define ETH_30_VTT_SID_READ_MII                    (0x06U) /*!< Service ID: Eth_30_Vtt_ReadMii() */
# define ETH_30_VTT_SID_GET_COUNTER_STATE           (0x07U) /*!< Service ID: Eth_30_Vtt_GetCounterState() */
# define ETH_30_VTT_SID_GET_PHYS_ADDR               (0x08U) /*!< Service ID: Eth_30_Vtt_GetPhysAddr() */
# define ETH_30_VTT_SID_PROVIDE_TX_BUFFER           (0x09U) /*!< Service ID: Eth_30_Vtt_ProvideTxBuffer() */
# define ETH_30_VTT_SID_TRANSMIT                    (0x0AU) /*!< Service ID: Eth_30_Vtt_Transmit() */
# define ETH_30_VTT_SID_UPDATE_PHYS_ADDR_FILTER     (0x12U) /*!< Service ID: Eth_30_Vtt_UpdatePhysAddrFilter() */
# define ETH_30_VTT_SID_SET_PHYS_ADDR               (0x13U) /*!< Service ID: Eth_30_Vtt_SetPhysAddr() */
# define ETH_30_VTT_SID_RECEIVE                     (0x0BU) /*!< Service ID: Eth_30_Vtt_Receive() */
# define ETH_30_VTT_SID_TX_CONFIRMATION             (0x0CU) /*!< Service ID: Eth_30_Vtt_TxConfirmation() */
# define ETH_30_VTT_SID_GET_VERSION_INFO            (0x0DU) /*!< Service ID: Eth_30_Vtt_GetVersionInfo() */

/* ETH DET errors */
# define ETH_30_VTT_E_NO_ERROR                       (0x00U) /*!< used to check if no error occurred - use a value unequal to any error code */
# define ETH_30_VTT_E_INV_CTRL_IDX                   (0x01U) /*!< Error code: API called with wrong controller index */
# define ETH_30_VTT_E_NOT_INITIALIZED                (0x02U) /*!< Error code: API called while module was not initialized correctly */
# define ETH_30_VTT_E_INV_POINTER                    (0x03U) /*!< Error code: API called with wrong pointer parameter (NULL_PTR) */
# define ETH_30_VTT_E_INV_PARAM                      (0x04U) /*!< Error code: API called with invalid parameter */
# define ETH_30_VTT_E_INV_CONFIG                     (0x05U)
# define ETH_30_VTT_E_INV_MODE                       (0x06U) /*!< Error code: API called while module was in an invalid mode */
# define ETH_30_VTT_E_FRAMES_LOST                    (0x07U)

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETH_30_VTT_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  Eth_30_Vtt_InitMemory
 **********************************************************************************************************************/
/*! \brief      This function initializes global variables. It has to be called
 *              before any other calls to the Eth API
 *  \param      void
 *  \return     void
 *  \context    Initialization
 *  \warning    Has to be called before usage of the module
 *  \note       NOT Re-entrant, synchronous
 **********************************************************************************************************************/
FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_InitMemory( void );

/***********************************************************************************************************************
 *  Eth_30_Vtt_Init
 **********************************************************************************************************************/
/*! \brief      This API call stores the start address of the post build time configuration
 *              of the Ethernet Controller driver, resets all transceivers controlled by the
 *              driver and may be used to initialize the data structures.
 *  \param[in]  CfgPtr                  Pointer to post-build configuration or null pointer
 *  \return     void
 *  \warning    Has to be called before usage of the module
 *  \context    Initialization
 *  \note       Re-entrant, synchronous
 **********************************************************************************************************************/
FUNC(void, ETH_30_VTT_CODE)Eth_30_Vtt_Init(
  P2CONST(Eth_ConfigType, ETH_30_VTT_CONST, ETH_30_VTT_CONST) CfgPtr);
  
/***********************************************************************************************************************
 *  Eth_30_Vtt_ControllerInit
 **********************************************************************************************************************/
/*! \brief      This API call of a specific Eth driver initializes the Ethernet Driver with index CtrlIdx.\n
 *              The following actions are performed:\n
 *              - Configuration of low level parameters\n
 *              - Initialization of descriptors
 *  \param[in]  CtrlIdx                  Controller index
 *  \param[in]  CfgIdx                   Configuration index
 *  \return     E_OK                     : Controller configured\n
 *              E_NOT_OK                 : Controller configuration failed
 *  \warning    Has to be called before usage of the module
 *  \context    Initialization
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter TrcvIdx is void
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_ControllerInit(
  uint8 CtrlIdx,
  uint8 CfgIdx);

/***********************************************************************************************************************
 *  Eth_30_Vtt_SetControllerMode
 **********************************************************************************************************************/
/*! \brief      Set controller mode
 *  \param[in]  CtrlIdx                  Controller index
 *  \param[in]  CtrlMode                 Operation mode
 *  \return     E_OK                     : Controller mode changed\n
 *              E_NOT_OK                 : Controller mode change failed
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter CtrlIdx is void
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_SetControllerMode(
  uint8        CtrlIdx,
  Eth_ModeType CtrlMode);

/***********************************************************************************************************************
 *  Eth_30_Vtt_GetControllerMode
 **********************************************************************************************************************/
/*! \brief      Get controller mode
 *  \param[in]  CtrlIdx                  Controller index
 *  \param[out] CtrlModePtr              Operation mode
 *  \return     E_OK                     : Controller mode evaluated\n
 *              E_NOT_OK                 : Controller mode evaluation failed
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter CtrlIdx is void
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_GetControllerMode(
        uint8                                          CtrlIdx,
  P2VAR(Eth_ModeType, AUTOMATIC, ETH_30_VTT_APPL_DATA) CtrlModePtr);

/***********************************************************************************************************************
 *  Eth_30_Vtt_GetPhysAddr
 **********************************************************************************************************************/
/*! \brief      Get physical address (MAC address)
 *  \param[in]  CtrlIdx                  Controller index
 *  \param[out] PhysAddrPtr              Physical address as byte array.
 *  \return     void
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter CtrlIdx is void
 **********************************************************************************************************************/
FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_GetPhysAddr(
        uint8                                   CtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETH_30_VTT_APPL_DATA) PhysAddrPtr);

/**********************************************************************************************************************
 *  Eth_30_Vtt_SetPhysAddr
 *********************************************************************************************************************/
/*! \brief      Sets the physical source address used by the indexed controller.\n
 *              If "MAC Write Access" is enabled the function also persistently writes the MAC address into NVM and
 *              sets the address on next ControllerInit.
 *  \param[in]  CtrlIdx               Controller index
 *  \param[in]  PhysAddrPtr           Pointer to the physical address
 *  \return     void
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter CtrlIdx is void
 *********************************************************************************************************************/
FUNC (void, ETH_30_VTT_CODE) Eth_30_Vtt_SetPhysAddr(
          uint8                                   CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETH_30_VTT_APPL_DATA) PhysAddrPtr);

# if (ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER == STD_ON)
/**********************************************************************************************************************
 *  Eth_30_Vtt_UpdatePhysAddrFilter
 *********************************************************************************************************************/
/*! \brief      Updated the physical source address to/from the indexed controller filter.
 *  \param[in]  CtrlIdx               Controller index
 *  \param[in]  PhysAddrPtr           Pointer to memory containing the physical source address (MAC address)
 *                                    in network byte order.
 *  \param[in]  Eth_FilterActionType  Add or remove the address from the Ethernet controllers filter
 *  \return     E_OK                  : Filter was successfully changed\n
 *              E_NOT_OK              : Filter could not be changed
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter CtrlIdx is void
 *********************************************************************************************************************/
FUNC (Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_UpdatePhysAddrFilter(
          uint8                                   CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETH_30_VTT_APPL_DATA) PhysAddrPtr,
          Eth_FilterActionType                    Action);
# endif /* ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER */

# if ( ETH_30_VTT_MII_INTERFACE == STD_ON )
/***********************************************************************************************************************
 *  Eth_30_Vtt_WriteMii
 **********************************************************************************************************************/
/*! \brief      Write a transceiver register via MII
 *  \param[in]  CtrlIdx               Controller index
 *  \param[in]  TrcvIdx               Tansceiver index (MII address)
 *  \param[in]  RegIdx                Transceiver register index
 *  \param[in]  RegVal                Transceiver register value
 *  \return     ETH_OK                : MII register written\n
 *              ETH_E_NOT_OK          : MII register write failure\n
 *              ETH_E_NO_ACCESS       : Ethernet transceiver access failure
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter CtrlIdx is void
 **********************************************************************************************************************/
FUNC(Eth_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_WriteMii(
  uint8  CtrlIdx,
  uint8  TrcvIdx,
  uint8  RegIdx,
  uint16 RegVal);
# endif /* ETH_30_VTT_MII_INTERFACE */

# if ( ETH_30_VTT_MII_INTERFACE == STD_ON )
/***********************************************************************************************************************
 *  Eth_30_Vtt_ReadMii
 **********************************************************************************************************************/
/*! \brief      Read transceiver register via MII
 *  \param[in]  CtrlIdx               Controller index
 *  \param[in]  TrcvIdx               Transceiver index (MII address)
 *  \param[in]  RegIdx                Transceiver register index
 *  \param[out] RegValPtr             Pointer for transceiver register value
 *  \return     ETH_OK                : MII register read\n
 *              ETH_E_NOT_OK          : MII register read failure\n
 *              ETH_E_NO_ACCESS       : Ethernet transceiver access failure
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter CtrlIdx is void
 **********************************************************************************************************************/
FUNC(Eth_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_ReadMii(
         uint8                                   CtrlIdx,
         uint8                                   TrcvIdx,
         uint8                                   RegIdx,
  P2VAR(uint16, AUTOMATIC, ETH_30_VTT_APPL_DATA) RegValPtr);
# endif /* ETH_30_VTT_MII_INTERFACE */

# if ( ETH_30_VTT_ENABLE_MIB_COUNTER == STD_ON )
/***********************************************************************************************************************
 *  Eth_30_Vtt_GetCounterState
 **********************************************************************************************************************/
/*! \brief      Returns a MAC management counter value.
 *  \param[in]  CtrlIdx                 Controller index
 *  \param[in]  CtrlCtrOffs             Counter offset into the Mac Management Counter block.
 *  \param[out] CtrValPtr               Counter value
 *  \return     E_NOT_OK                : Error\n
 *              E_OK                    : Success
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter CtrlIdx is void
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_GetCounterState(
         uint8                                   CtrlIdx,
        uint16                                   CtrOffs,
  P2VAR(uint32, AUTOMATIC, ETH_30_VTT_APPL_DATA) CtrValPtr);
# endif /* ETH_30_VTT_ENABLE_MIB_COUNTER */

/***********************************************************************************************************************
 *  Eth_30_Vtt_ProvideTxBuffer
 **********************************************************************************************************************/
/*! \brief      Provide a buffer for frame transmission. The buffer is locked until Eth_TxConfirmation is called by
*               interrupt. Alternatively the user may call the Eth_Transmit with LenByte=0 to release the
*               buffer.
 *  \param[in]  CtrlIdx                 Controller index
 *  \param[out] BufIdxPtr               Buffer index
 *  \param[out] BufPtr                  Pointer to buffer area
 *  \param      LenBytePtr              LenBytePtr is an in/out parameter.\n
 *                                       [in]  The requested buffer length. The requested length need to be
 *                                             Ethernet header length + payload length. \n
 *                                       [out] The actual buffer length reduced by Ethernet header length. The
 *                                             Ethernet header is written by Eth_Transmit. The actual buffer length is
 *                                             equal or bigger than the requested payload length as far as the return
 *                                             value is BUFREQ_OK.
 *  \return     BUFREQ_OK               : Buffer locked and ready to use\n
 *              BUFREQ_E_NOT_OK         : Development error check failed\n
 *              BUFREQ_E_BUSY           : All buffers in use. Try later\n
 *              BUFREQ_E_OVFL           : Requested buffer is too large
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter CtrlIdx is void
 **********************************************************************************************************************/
FUNC(BufReq_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_ProvideTxBuffer(
        uint8                                           CtrlIdx,
  P2VAR(uint8,        AUTOMATIC, ETH_30_VTT_APPL_DATA)  BufIdxPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETH_30_VTT_APPL_DATA) *BufPtr,
  P2VAR(uint16,       AUTOMATIC, ETH_30_VTT_APPL_DATA)  LenBytePtr);

/***********************************************************************************************************************
 *  Eth_30_Vtt_Transmit
 **********************************************************************************************************************/
/*! \brief      Transmit the locked buffer provided by Eth_ProvideTxBuffer and identified by BufIdx
 *  \param[in]  CtrlIdx                 Controller index
 *  \param[in]  BufIdx                  Buffer index
 *  \param[in]  FrameType               Ethernet frame type, according to type field of IEEE802.3
 *  \param[in]  TxConfirmation          True if a transmit confirmation is desired, otherwise false
 *  \param[in]  LenByte                 Payload length (no Ethernet header length included)
 *  \param[in]  PhysAddrPtr             Destination MAC address as byte array.
 *  \return     E_NOT_OK                : Frame transmission not successful\n
 *              E_OK                    : Frame handed over to transmission ring buffer
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, asynchronous\n
 *              - If API optimization is enabled, parameter CtrlIdx is void
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_Transmit(
          uint8                               CtrlIdx,
          uint8                               BufIdx,
          Eth_FrameType                       FrameType,
          boolean                             TxConfirmation,
          uint16                              LenByte,
  P2CONST(uint8, AUTOMATIC, ETH_30_VTT_CONST) PhysAddrPtr);

/***********************************************************************************************************************
 *  Eth_30_Vtt_Receive
 **********************************************************************************************************************/
/*! \brief      Calls the reception callback of all fully received Ethernet frames.
 *  \param[in]  CtrlIdx                      Controller index
 *  \param[out] RxStatusPtr                  Indicates whether a frame has been received and if so, whether more frames
 *                                           are available or frames got lost
 *  \return     void
 *  \context    Interrupt or task level
 *  \note       - NOT Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter CtrlIdx is void
 *              - RxStatusPtr is in interrupt mode unused because information is not used by interrupt handler
 *              - When interrupt mode is enabled this function must not be called except from the interrupt handler
 **********************************************************************************************************************/
FUNC(void, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_Receive(
        uint8                                             CtrlIdx,
  P2VAR(Eth_RxStatusType, AUTOMATIC, ETH_30_VTT_APPL_VAR) RxStatusPtr);

/***********************************************************************************************************************
 *  Eth_30_Vtt_VTransmit
 **********************************************************************************************************************/
/*! \brief      Transmit the locked buffer provided by Eth_ProvideTxBuffer and identified by BufIdx
 *  \param[in]  CtrlIdx                 Controller index
 *  \param[in]  BufIdx                  Buffer index
 *  \param[in]  FrameType               Ethernet frame type, according to type field of IEEE802.3
 *  \param[in]  TxConfirmation          True if a transmit confirmation is desired, otherwise false
 *  \param[in]  LenByte                 Payload length (no Ethernet header length included)
 *  \param[in]  PhysAddrPtrDst          Destination MAC address as byte array.
 *  \param[in]  PhysAddrPtrSrc          Source MAC address as byte array.
 *  \return     E_NOT_OK                : Frame transmission not successful\n
 *              E_OK                    : Frame handed over to transmission ring buffer
 *  \context    Interrupt or task level
 *  \note       - Re-entrant, asynchronous\n
 *              - If API optimization is enabled, parameter CtrlIdx is void
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_VTransmit(
  uint8                                    CtrlIdx,
  uint8                                    BufIdx,
  Eth_FrameType                            FrameType,
  boolean                                  TxConfirmation,
  uint16                                   LenByte,
  P2CONST(uint8, AUTOMATIC, ETH_30_VTT_CONST) PhysAddrPtrDst,
  P2CONST(uint8, AUTOMATIC, ETH_30_VTT_CONST) PhysAddrPtrSrc);

/***********************************************************************************************************************
 *  Eth_30_Vtt_TxConfirmation
 **********************************************************************************************************************/
/*! \brief      Unlocks the buffers of fully transmitted frames. Eth_TxConfirmation must not be used when interrupts
 *              are enabled
 *  \param[in]  CtrlIdx                Controller index
 *  \return     void
 *  \context    Interrupt or task level
 *  \note       - NOT Re-entrant, synchronous\n
 *              - If API optimization is enabled, parameter CtrlIdx is void
 *              - When interrupt mode is enabled this function must not be called except from the interrupt handler
 **********************************************************************************************************************/
FUNC(void, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_TxConfirmation(
  uint8 CtrlIdx );

# if ( ETH_30_VTT_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  Eth_30_Vtt_GetVersionInfo
 **********************************************************************************************************************/
/*! \brief      Returns the version information, vendor ID and AUTOSAR module ID of the component.
 *              The versions are BCD-coded.
 *  \param[out] VersionInfoPtr        Returns the following version information:\n
 *                                     - Vendor ID\n
 *                                     - Module ID\n
 *                                     - Software major version\n
 *                                     - Software minor version\n
 *                                     - Software patch version
 *  \return     void
 *  \context    Interrupt or task level
 *  \note       Re-entrant, synchronous
 **********************************************************************************************************************/
FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_GetVersionInfo(
  P2VAR(Std_VersionInfoType, ETH_30_VTT_APPL_DATA, ETH_30_VTT_APPL_DATA) VersionInfoPtr);
# endif /* ETH_30_VTT_VERSION_INFO_API */

# define ETH_30_VTT_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* ETH_30_VTT_H */

/**********************************************************************************************************************
 *  END OF FILE: Eth_30_Vtt.h
 *********************************************************************************************************************/

