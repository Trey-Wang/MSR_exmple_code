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
/**        \file  EthIf.h
 *        \brief  Ethernet Interface header file
 *
 *      \details  Component to abstract the underlying Ethernet hardware for the upper layers to provide a generic
 *                interface to be used for controlling and communicate with the help of the Ethernet hardware
 *                provided by an ECU (Ethernet controller, Ethernet transceiver, Ethernet switch).
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Alex Lunkenheimer             visalr        Vector Informatik GmbH
 *  Harald Walter                 vishaw        Vector Informatik GmbH
 *  Frederik Dornemann            visfdn        Vector Informatik GmbH
 *  Mark Harsch                   vismha        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2008-08-04  visalr  -             created
 *  01.00.01  2008-11-20  visalr  -             bugfix: switch off transceiver
 *  01.00.02  2008-11-25  visalr  -             bugfix: tx confirmation without request
 *  01.00.03  2008-12-08  visalr  -             EthIf_Types.h introduced
 *  01.02.00  2009-01-14  visalr  -             version inconsistencies
 *  01.03.00  2009-01-15  visalr  -             separate MainFunctions for Rx and Tx
 *  01.04.00  2009-03-19  visalr  -             EthTrcv_GetLinkState param CtrlIdxPtr removed
 *  01.04.01  2009-03-26  visalr  -             SoAd: bugfix for DET error if ConfigVariant != 3
 *  01.04.02  2009-07-31  visalr  -             include Det.h only if configured
 *                                -             new DET macros
 *                                -             API optimization support (single channel API)
 *                                -             BRS Time Measurement support added
 *                                -             Config variant check for library build
 *                                -             optional separate MainFunction for state
 *  01.04.03  2009-09-15  visalr  -             bugfix: Eth_Transmit with more than one buffer
 *                                -             bugfix: reset stored transceiver mode in EthIf_SetTransceiverMode
 *                                -             Config version check
 *  01.04.04  2009-09-28  visalr  -             GENy release version
 *  01.04.05  2009-09-29  visalr  -             Memory Abstraction for stack local pointer
 *                                -             bugfix support all configuration variants
 *  01.04.06  2009-10-02  visalr  -             bugfix include files adapted
 *                                -             single module header introduced (single source)
 *  01.04.07  2009-10-05  visalr  -             no changes
 *  01.04.08  2009-10-05  visalr  -             no changes
 *  01.04.09  2009-10-08  visalr  -             bugfix missing ETHIF_STOP_SEC_CODE in EthIf_Lcfg.h
 *  02.00.00  2009-10-08  visalr  -             no changes
 *  02.00.01  2009-10-08  visalr  -             no changes
 *  02.00.02  2009-10-13  visalr  -             comment format updated
 *  02.01.00  2009-11-17  visalr  -             bugfix during optimization
 *                                -             VendorId added
 *                                -             bugfix DET checks without actual condition did not fire
 *                                -             validate post-build configuration
 *                                -             DET check bugfixing
 *                                -             support multiple calls of Eth_Init
 *  02.01.01  2010-01-12  visalr  ESCAN00040063 EthIf: Compiler Error without DET
 *  02.01.02  2010-02-11  visalr  ESCAN00040761 Wrong module id returned
 *  02.01.03  2010-09-16  visalr  ESCAN00045366 DET error by EthTrcv in EthIf_MainFunction if initial COM_REQUEST is
 *                                              missing
 *  02.01.04  2010-12-23  visalr  ESCAN00047335 Adapt MainFunction for usage with IdentityManagerConfig
 *            2011-02-11  visalr  ESCAN00048017 MemMap.h: Missing Start/Stop Sections (PBCFG_ROOT)
 *            2011-02-16  visalr  -             compiler abstraction adapted (use of AUTOMATIC)
 *  02.01.05  2011-03-04  visalr                missing comment within Eth_Compiler_Cfg.inc
 *  02.01.06  2011-03-18  visalr                wrong component version IF_ASRIFETH_VERSION,
 *                                              IF_ASRIFETH_RELEASE_VERSION
 *  02.01.07  2011-05-31  visalr  ESCAN00050530 Generate EthIf  EcuC Global Configuration Container Name
 *  02.01.08  2011-09-07  visalr  ESCAN00053407 Addtional preprocessor argument for macro without DEM has to be removed
 *  02.01.09  2011-09-23  visalr                GHS compiler warnings resolved (undefined preprocessing identifier)
 *  02.01.10  2011-11-07  visalr  ESCAN00054715 Vector coding rules compliance
 *  03.00.00  2011-12-07  visalr                Add IpV6 Frame Type support. GENy only, no code changes but version
 *                                              check adapted
 *  03.00.01  2011-12-07  visalr  ESCAN00055494 Wrong memory section mapping in Ethernet Interface
 *  03.00.02  2012-02-10  visalr  ESCAN00056744 Set Transceiver Mode does not trigger Transceiver Link State callback
 *                                              when switched off
 *            2012-02-13  visalr  ESCAN00056356 VAR_INIT / VAR_ZERO_INIT Memory Mapping sections
 *            2012-02-14  visalr  ESCAN00056357 Remove Macro support for Version Info API
 *            2012-02-14  visalr  ESCAN00056599 Compiler Abstractions are partly wrong
 *            2012-02-22  visalr  ESCAN00051712 Remove the AUTOSAR Release Version Check
 *  03.00.03  2012-04-23  visalr  ESCAN00056356 VAR_INIT / VAR_ZERO_INIT Memory Mapping sections
 *  03.01.00  2012-08-31  vishaw  -             Discard VLAN Tags within received Ethernet frames originated by PLC
 *                                              chips
 *  03.01.01  2012-10-02  visalr  ESCAN00061909 ASR4 Enhancement
 *            2012-10-02  visalr  ESCAN00058884 AR4-125: Remove support for v_cfg.h in Cfg5 systems
 *            2012-10-18  vishaw  ESCAN00062248 AR4-220: Remove STATIC
 *  04.00.00  2013-03-19  vishaw  ESCAN00066018 ASR4.1.1 extensions
 *                                ESCAN00068224 AR4-385: VLAN Support (multi home)
 *                                ESCAN00060125 Optimization of Transceiver Linke State Change Callback
 *  04.00.01  2013-11-05  vishaw  ESCAN00066018 Resubmitted: ASR4.1.1 extensions
 *  04.01.00  2014-01-23  vishaw  ESCAN00073154 Implemented zero-copy extensions
 *  04.01.01  2014-02-12  visfdn  ESCAN00073610 Usage of uninitialized variable 'VirtualCtrl' in EthIf_Transmit()
 *  04.02.00  2014-03-04  vishaw  ESCAN00074065 Ethernet transceiver is initialized multiple times in a row
 *                                ESCAN00074060 Introduced PTP support
 *  04.02.01  2014-03-28  vishaw  ESCAN00074620 Multiple transceiver configurations of one transceiver driver aren't
 *                                              initialized properly
 *                                ESCAN00074611 Ethernet Driver may throw a DET error in Eth_Receive when interrupts
 *                                              are disabled (polling mode)
 *                                ESCAN00074869 EthIf may return the wrong Ethernet Controller or Transceiver mode 
 *  04.02.02  2014-03-14  vishaw  ESCAN00074979 Some RAM variables aren't initialized. EthIf initialization may fail
 *                                              and communication does not take in place
 *                                ESCAN00074980 Avoid compiler faults by using pointers to ROM structures instead
 *                                              of loading the structures onto the stack
 *                                ESCAN00074981 PTP APIs may call the Ethernet controller driver with wrong
 *                                              Controller Indexes, causing DETS in the Ethernet controller driver
 *  04.02.03  2014-06-17  vishaw  ESCAN00075364 Compiler error in PTP submodule when API optimization is enabled
 *                                ESCAN00072342 Eth_ProvideTxBuffer succeeds although data length is too long
 *  04.02.04  2014-07-04  vishaw  ESCAN00076449 TrcvLinkStateChg callbacks of upper layer modules are called although
 *                                              Eth-Controller is not initialized
 *  04.04.00  2014-07-17  vishaw  ESCAN00073605 AR4-500: AUTOSAR CONC_600_SwitchConfiguration
 *  04.05.00  2014-09-19  vishaw  ESCAN00076708 AR4-897: Support overwriting of the Ethernet header during
 *                                              transmission and support reception of Ethernet header information for
 *                                              upper layers of the Ethernet Interface
 *                                ESCAN00069414 Transceiver link state is checked with too slow frequency in
 *                                              EthIf_MainFunctionState
 *                                ESCAN00069310 Vehicle Announcement messages are not sent after transceiver
 *                                              link state changes to active
 *                                ESCAN00069413 Improve MainFunctionState handling by performing reload handling not
 *                                              within the state function itself
 *            2015-02-03  vismha  ESCAN00079196 Compiler error: EthIf_MainFunctionRx undefined if API optimization
 *                                              and polling mode enabled
 *  04.06.00  2015-02-16  vismha  ESCAN00080072 FEAT-705: Ethernet wakeup based on Activation Line [AR4-1006]
 *            2015-03-02  vismha  ESCAN00078574 EthIf_SetCorrectionTime uses sint32 instead of Eth_TimeDiffType for
 *                                              parameter OffsetTimePtr
 *                                ESCAN00079195 EthIf polls the Eth_Receive method without using the RxStatus
 *                                              parameter as a stop criterion
 *  04.06.01  2015-03-26  vismha  ESCAN00078574 EthIf_SetCorrectionTime uses sint32 instead of Eth_TimeDiffType for
 *                                              parameter OffsetTimePtr
 *                                              (Re-Submitted due to wrong description of Ticket, type name should be
 *                                               Eth_TimediffType)
 *  04.07.00  2015-05-22  vismha  ESCAN00083137 Use AUTOSAR timestamp type instead of Vector proprietary type
 *  04.08.00  2015-06-15  vismha  ESCAN00082697 FEAT-1457: SRP module development
 *  04.09.00  2015-07-06  vismha  ESCAN00083818 Receive polling processes a high amount of time under high traffic
 *                                              situations
 *            2015-07-17  vismha  ESCAN00083367 Support of Mirroring/Gateway functionality
 *                                ESCAN00084076 Support multi derivative Eth controllers and Eth controller API
 *                                              infixing
 *  04.09.01  2015-10-29  vismha  ESCAN00086158 Destination/Source MAC address corrupt if VLAN tagged frame is routed
 *                                              by traffic gateway
 *                                ESCAN00086077 Compiler error: Unresolved symbol EthIf_VTransmit in
 *                                              EthIf_ExtndTrafficHndl.c
 *  05.00.00  2015-11-05  vismha  ESCAN00080843 Handling of data transmission and reception is inconsistent for VLAN
 *                                              and no VLAN virtual controllers
 *                                ESCAN00082029 Compiler warning: "no function prototype given: converting '()' to
 *                                              '(void)'
 *                                ESCAN00083601 Ethernet Tx Buffers for a VLAN EthIf controller aren't released by
 *                                              call to EthIf_Transmit()
 *                                ESCAN00083605 EthIf_GetRxHeaderPtr() returns wrong length when requesting for VLAN
 *                                              EthIf controller
 *                                ESCAN00084341 EthIf_GetTxHeaderPtr() returns wrong length when requesting for VLAN
 *                                              EthIf controller
 *            2015-11-25  vismha  ESCAN00086738 FEAT-1529: Support Ethernet Switches for Ethernet Time Sync
 *            2015-12-08  vismha  ESCAN00086980 Undesired TxConfirmation for a canceled Frame Transmission
 *  05.00.01  2016-01-15  vismha  ESCAN00087503 NULL_PTR access during EthIf_SetControllerMode()
 *            2016-03-02  vismha  ESCAN00087559 NULL_PTR access instead of DET error in EthIf_GetRxHeaderPtr()
 *  05.00.02  2016-05-04  vismha  ESCAN00089656 Compiler error: identifier "EthIf_TxBufferNum" is undefined when two
 *                                              Ethernet controllers are used
 *            2016-05-09  vismha  ESCAN00089912 Transmit fails although buffer was successfully acquired
 *  05.00.03  2016-06-07  vismha  ESCAN00089534 Semaphore IsLocked of EthIf_ControllerInit() and
 *                                              EthIf_SetControllerMode() shall be module local
 *  06.00.00  2016-06-15  vismha  ESCAN00089373 FEAT-1845: Processmigration: ETH Stack
 *                                ESCAN00089982 EthIf_Init DET Check
 *                                ESCAN00089983 EthIf_UpdatePhysAddrFilter DET Check
 *                                ESCAN00089984 EthIf_ProvideTxBuffer DET Check
 *                                ESCAN00089985 EthIf_Transmit DET Check
 *                                ESCAN00089986 EthIf_RxIndication DET Check
 *                                ESCAN00089991 EthIf_GetDropCount DET Check
 *                                ESCAN00089992 EthIf_StoreConfiguration DET Check
 *                                ESCAN00089993 EthIf_ResetConfiguration DET Check
 *                                ESCAN00089996 EthIf_ProvideExtTxBuffer DET Check
 *                                ESCAN00089998 EthIf_GetBandwidthLimit DET Check
 *                                ESCAN00090075 EthIf_SetControllerMode DET Check
 *            2016-06-23  vismha  ESCAN00089365 FEAT-1738: Ethernet Receive Buffer Segmentation
 *            2016-06-27  vismha  ESCAN00090679 EthIf_GetPortMacAddr() returns a wrong Switch Index
 *            2016-07-18  vismha  ESCAN00090397 Prohibit triggering of Frame Transmission without previous buffer
 *                                              provision
 *  06.00.01  2016-08-22  vismha  ESCAN00091244 Compiler error: 'EthIf_CfgAccess_TrcvLinkStateChgReload' undefined
 *                                ESCAN00091548 Correct findings of review of version 6.00.00
 *  07.00.00  2016-10-13  vismha  FEATC-249     FEAT-1998: Support of HW Time Stamping for Switch in EthIf
 *  07.01.00  2016-12-20  vismha  FEATC-914     FEAT-2119: Introduce API for SRP Server in EthIf
 *  07.01.01  2017-01-27  vismha  ESCAN00093756 Timestamp queue in switch driver can reach an inconsistent state,
 *                                              resulting in loss of egress timestamps due to wrong API design
 *                                ESCAN00093271 Missing include of BswM_EthIf.h
 *                                ESCAN00093282 Link State Change Indication to DOWN called although link wasn't UP
 *                                              before
 *            2017-02-03  vismha  ESCAN00093833 Misalignment exception during reception of a frame with odd length on
 *                                              RH850/V850 platform
 *  07.02.00  2017-01-30  vismha  FEATC-890     FEAT-2354: Firewall concept for Ethernet. Implementation of EthIf
 *                                              callout.
 *            2017-02-06  vismha  ESCAN00093876 Compiler error: Unresolved symbol EthIf_ExitExclusiveAre() in
 *                                              EthIf_ExtndTrafficHndl.c
 *                                ESCAN00093878 Compiler error: Unresolved symbol SrcFrameLen in
 *                                              EthIf_Mirroring_ProvideTxMirroringBuffer() of  EthIf_ExtndTrafficHndl.c
 *                                ESCAN00093713 Compiler error: EthIf_ExtndTrafficHndl.c: Cannot open include file:
 *                                              'Det.h': No such file or directory
 *  07.02.01  2017-02-16  vismha  ESCAN00093258 EthIf Switching Independent Port Groups (controlled by BswM) does not
 *                                              work correctly
 *                                ESCAN00094059 Integrated review findings of initial MSR4-R17 release
 *  08.00.00  2017-02-20  vismha  FEATC-1167    FEAT-2279: Time Synchronization acc. AR 4.3 for Eth_If
 *                                FEATC-713     FEAT-2151: EthIf - Extended Ethernet Bus Diagnostics
 *            2017-02-21  vismha  ESCAN00094071 EthIf Main-Function declarations provided by SchM
 *                                ESCAN00094070 Implement Dev error IDs according to ASR4.3
 *            2017-02-28  vismha  ESCAN00094199 EthIf allows transmission for same buffer multiple times
 *  08.00.01  2017-05-15  vismha  ESCAN00095136 Transmission of frames stops under high traffic conditions
 *********************************************************************************************************************/

#if !defined (ETHIF_H)
# define ETHIF_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthIf_Types.h"
# include "EthIf_Cfg.h"
# include "EthIf_Switch.h"
# include "EthIf_Time.h"
# include "EthIf_ZeroCopy.h"
# include "SchM_EthIf.h" /* Provides access to the EthIf mainfunction declarations */
# if ( ETHIF_DEV_ERROR_REPORT == STD_ON )
#  include "Det.h"
# endif /* ETHIF_DEV_ERROR_DETECT */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Vendor and module identification */
# define ETHIF_VENDOR_ID                           (30U)
# define ETHIF_MODULE_ID                           (0x41U)  /* 65 decimal */

/* AUTOSAR Software specification version information */
/* \trace SPEC-2393556 */
# define ETHIF_AR_RELEASE_MAJOR_VERSION            (4U)
# define ETHIF_AR_RELEASE_MINOR_VERSION            (1U)
# define ETHIF_AR_RELEASE_REVISION_VERSION         (1U)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
/* \trace SPEC-2393556 */
# define ETHIF_SW_MAJOR_VERSION                    (8U)
# define ETHIF_SW_MINOR_VERSION                    (0U)
# define ETHIF_SW_PATCH_VERSION                    (1U)

# define ETHIF_INSTANCE_ID_DET                     (0x00U)

/* ----- API service IDs ----- */
# define ETHIF_SID_INIT                            (0x01U)
# define ETHIF_SID_CONTROLLER_INIT                 (0x02U)
# define ETHIF_SID_SET_CONTROLLER_MODE             (0x03U)
# define ETHIF_SID_GET_CONTROLLER_MODE             (0x04U)
# define ETHIF_SID_TRANSCEIVER_INIT                (0x05U)
# define ETHIF_SID_SET_TRANSCEIVER_MODE            (0x06U)
# define ETHIF_SID_GET_TRANSCEIVER_MODE            (0x07U)
# define ETHIF_SID_SET_TRANSCEIVER_WAKEUP_MODE     (0x2EU)
# define ETHIF_SID_GET_TRANSCEIVER_WAKEUP_MODE     (0x2FU)
# define ETHIF_SID_CHECK_WAKEUP                    (0x30U)
# define ETHIF_SID_GET_PHYS_ADDR                   (0x08U)
# define ETHIF_SID_SET_PHYS_ADDR                   (0x0DU)
# define ETHIF_SID_UPDATE_PHYS_ADDR_FILTER         (0x0CU)
/* Switch driver related API uses SIDs starting with 0x28 (contained in Eth_Switch.h) */
/* Time manipulation API uses SIDs starting with 0x22 (contained in EthIf_Time.h) */
# define ETHIF_SID_PROVIDE_TX_BUFFER               (0x09U)
# define ETHIF_SID_TRANSMIT                        (0x0AU)
# define ETHIF_SID_GET_VERSION_INFO                (0x0BU)
# define ETHIF_SID_RX_INDICATION                   (0x10U)
# define ETHIF_SID_TX_CONFIRMATION                 (0x11U)
# define ETHIF_SID_MAIN_FUNCTION_RX                (0x20U)
# define ETHIF_SID_MAIN_FUNCTION_TX                (0x21U)
/* Zero Copy/Header Access API uses SIDs starting with 0x0E (contained in EthIf_ZeroCopy.h) */
# define ETHIF_SID_SET_BANDWIDTH_LIMIT             (0x60U)
# define ETHIF_SID_GET_BANDWIDTH_LIMIT             (0x61U)
/* Extended traffic handling API uses SIDs starting with 0x70 (contained in EthIf_ExtndTrafficHndl.h) */
/* Switch driver timestamping API uses SIDs starting with 0x80 (contained in EthIf_Switch.h) */
# define ETHIF_SID_SWITCH_PORT_GROUP_REQUEST_MODE  (0x90U)
/* Switch driver dynamic SRP related API uses SID starting with 0xA0 */
# define ETHIF_SID_GET_TX_STATS                    (0xB0U)
# define ETHIF_SID_GET_RX_STATS                    (0xB1U)

/* ----- Error codes ----- */ /* \trace SPEC-2393570 */
# define ETHIF_E_NO_ERROR                          (0x00U) /*!< no error occurred */
# define ETHIF_E_INV_CTRL_IDX                      (0x01U) /*!< Error code: API service was called with invalid controller index */
# define ETHIF_E_INV_TRCV_IDX                      (0x02U) /*!< Error code: API service was called with invalid transceiver index */
# define ETHIF_E_INV_PORT_GROUP_IDX                (0x03U) /*!< Error code: API service was called with invalid port group index */
# define ETHIF_E_NOT_INITIALIZED                   (0x04U) /*!< Error code: API service used without module initialization */
# define ETHIF_E_INV_PARAM_POINTER                 (0x05U) /*!< Error code: API service used with invalid pointer parameter (NULL_PTR) */
# define ETHIF_E_INV_PARAM                         (0x06U) /*!< Error code: API service used with invalid value for parameter */
# define ETHIF_E_INIT_FAILED                       (0x07U) /*!< Error code: The service EthIf_Init() was called with an invalid configuration */
# define ETHIF_E_INV_SWITCH_IDX                    (0x08U) /*!< Error code: API service was called with invalid switch index */
# define ETHIF_E_INV_DRIVER_API_CALL               (0x09U) /*!< Error code: API service can't be redirected to driver due to either configuration related lack or unsupported API */
# define ETHIF_E_INV_STATE                         (0x0AU) /*!< Error code: API service processing leads to an invalid state of the module */
# define ETHIF_E_INTERNAL_ERROR                    (0xFFU) /*!< Error code: Internal error occurred */
/* Extended traffic handling DET errors starting with 0x10u (contained in EthIf_ExtndTrafficHndl.h) */

/* ----- Modes ----- */
# define ETHIF_STATE_UNINIT                        (0x00U)
# define ETHIF_STATE_INIT                          (0x01U)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETHIF_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EthIf_InitMemory
 *********************************************************************************************************************/
/*! \brief       Function for *_INIT_*-variable initialization
 *  \details     Service to initialize module global variables at power up. This function initializes the
 *               variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace CREQ-111162
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_InitMemory( void );

/**********************************************************************************************************************
 *  EthIf_Init
 *********************************************************************************************************************/
 /*! \brief       Initializes the EthIf module
  *  \details     Function initializes the module EthIf. It initializes all variables and sets the module state to
  *               initialized.
  *  \param[in]   CfgPtr               Configuration structure for initializing the module
  *  \pre         Interrupts are disabled.
  *  \pre         Module is uninitialized.
  *  \pre         EthIf_InitMemory has been called unless EthIf_ModuleInitialized is initialized by start-up code.
  *  \context     TASK
  *  \reentrant   FALSE
  *  \synchronous TRUE
  *  \note        Specification of module initialization
  *  \trace CREQ-111162
  *  \trace SPEC-2393574
  *  \trace SPEC-2393566
  *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_Init(
  P2CONST(EthIf_ConfigType, AUTOMATIC, ETHIF_CONST) CfgPtr);

/**********************************************************************************************************************
 *  EthIf_ControllerInit
 *********************************************************************************************************************/
/*! \brief       Initializes a EthIf controller
 *  \details     Function initializes the EthIf controller addressed and redirects the call to the underlying
 *               hardware drivers mapped to the EthIf controller.
 *  \param[in]   CtrlIdx               EthIf controller index
 *  \param[in]   CfgIdx                Configuration index
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters or call interrupted pending operation
 *  \pre         Module and drivers have been initialized
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111165
 *  \trace SPEC-2393581
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_ControllerInit(
  uint8  CtrlIdx,
  uint8  CfgIdx);

/**********************************************************************************************************************
 *  EthIf_SetControllerMode
 *********************************************************************************************************************/
/*! \brief       Modifies the EthIf controller mode
 *  \details     Function alters the EthIf controller mode and redirects the call to the underlying hardware
 *               drivers mapped to the EthIf controller.
 *  \param[in]   CtrlIdx               EthIf controller index
 *  \param[in]   CtrlMode              Mode that shall be applied:
 *                                     ETH_MODE_DOWN - shut down the EthIf controller
 *                                     ETH_MODE_ACTIVE - activate the EthIf controller
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters or call interrupted pending operation
 *  \pre         Module and drivers have been initialized
 *  \pre         EthIf_ControllerInit() was called for the respective EthIf controller before
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111163
 *  \trace SPEC-2393586
 *  \trace SPEC-2393602
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetControllerMode(
  uint8         CtrlIdx,
  Eth_ModeType  CtrlMode);

/**********************************************************************************************************************
 *  EthIf_GetControllerMode
 *********************************************************************************************************************/
/*! \brief       Retrieves the EthIf controller mode
 *  \details     Function retrieves the current EthIf controller mode.
 *  \param[in]   CtrlIdx               EthIf controller index
 *  \param[out]  CtrlModePtr           Retrieved mode:
 *                                     ETH_MODE_DOWN - EthIf controller is turned of
 *                                     ETH_MODE_ACTIVE - EthIf controller is active
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111164
 *  \trace SPEC-2393591
 *  \trace SPEC-2393607
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetControllerMode(
        uint8                                      CtrlIdx,
  P2VAR(Eth_ModeType, AUTOMATIC, ETHIF_APPL_DATA)  CtrlModePtr);

/***********************************************************************************************************************
 *  EthIf_GetPhysAddr
 **********************************************************************************************************************/
/*! \brief       Retrieves the MAC address related to the EthIf controller
 *  \details     Function retrieves the MAC address that is used as source MAC address by the Ethernet controller
 *               mapped to the EthIf controller.
 *  \param[in]   CtrlIdx               EthIf controller index
 *  \param[out]  PhysAddrPtr           MAC address retrieved
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111166
 *  \trace SPEC-2393613
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_GetPhysAddr(
        uint8                               CtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA)  PhysAddrPtr);

/**********************************************************************************************************************
 *  EthIf_SetPhysAddr
 *********************************************************************************************************************/
/*! \brief       Sets the MAC address related to the EthIf controller
 *  \details     Function alters the MAC address that is used as source MAC address by the Ethernet controller
 *               mapped to the EthIf controller.
 *  \param[in]   CtrlIdx               EthIf controller index
 *  \param[in]   PhysAddrPtr           MAC address to use as source MAC address
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111167
 *  \trace SPEC-2393619
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_SetPhysAddr(
          uint8                               CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHIF_APPL_DATA)  PhysAddrPtr);

# if ( ETHIF_ENABLE_UPDATE_PHYS_ADDR_FILTER == STD_ON )
/**********************************************************************************************************************
 *  EthIf_UpdatePhysAddrFilter
 *********************************************************************************************************************/
/*! \brief       Modifies the receive MAC address filter related to the EthIf Controller
 *  \details     Function modifies the receive MAC address filter of the Ethernet controller mapped to the EthIf
 *               controller by adding/removing the MAC address to/from the receive MAC address filter.
 *  \param[in]   CtrlIdx               EthIf controller index
 *  \param[in]   PhysAddrPtr           MAC address that shall be added/removed
 *  \param[in]   Action                Action that shall be applied on the MAC address filter:
 *                                     ETH_ADD_TO_FILTER - adapt filter to be able to receive frames with the given
 *                                                         MAC address as destination MAC address
 *                                     ETH_REMOVE_FROM_FILTER - adapt filter to prevent reception of frames with the
 *                                                              given MAC address as destination MAC address
 *  \return      E_OK - success
 *               E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111168
 *  \trace SPEC-2393625
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_UpdatePhysAddrFilter(
          uint8                                             CtrlIdx,
  P2CONST(uint8,               AUTOMATIC, ETHIF_APPL_DATA)  PhysAddrPtr,
          Eth_FilterActionType                              Action);
# endif /* ETHIF_ENABLE_UPDATE_PHYS_ADDR_FILTER */

/**********************************************************************************************************************
 *  EthIf_ProvideTxBuffer
 *********************************************************************************************************************/
/*! \brief          Provides a transmission buffer for an Ethernet frame
 *  \details        Function allows to acquire a buffer where a upper layer is able to insert the payload for the
 *                  Ethernet frame.
 *  \param[in]      CtrlIdx               EthIf controller index
 *  \param[in]      FrameType             EtherType to insert into the Ethernet frame header
 *  \param[in]      Priority              Priority of the Ethernet frame, which is coded into the PCP of the IEEE802.3Q
 *                                        VLAN tag. If EthIf controller represents a physical data connection the
 *                                        priority is ignored.
 *  \param[out]     BufIdxPtr             Index to identify the acquired buffer
 *  \param[out]     BufPtr                Buffer the payload can be written to
 *  \param[in,out]  LenBytePtr            Buffer length:
 *                                        [in] - Length in byte needed for the payload, which shall be transmitted
 *                                        [out] - Length of the buffer that is provided in byte (has at least the size of
 *                                                the requested length needed for the payload)
 *  \return         BUFREQ_OK - success
 *  \return         BUFREQ_E_NOT_OK - function has been called with invalid parameters
 *  \return         BUFREQ_E_BUSY - all buffers are in use
 *  \return         BUFREQ_E_OVFL - requested length is too large
 *  \pre            Module and drivers have been initialized
 *  \context        ANY
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace CREQ-111169
 *  \trace SPEC-2393631
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, ETHIF_CODE) EthIf_ProvideTxBuffer(
        uint8                                       CtrlIdx,
        Eth_FrameType                               FrameType,
        uint8                                       Priority,
  P2VAR(uint8,         AUTOMATIC, ETHIF_APPL_DATA)  BufIdxPtr,
  P2VAR(Eth_DataType*, AUTOMATIC, ETHIF_APPL_DATA)  BufPtr,
  P2VAR(uint16,        AUTOMATIC, ETHIF_APPL_DATA)  LenBytePtr);


/**********************************************************************************************************************
 *  EthIf_VTransmit
 *********************************************************************************************************************/
/*! \brief       Triggers transmission of an Ethernet frame with a given source MAC address
 *  \details     Function triggers the transmission of an Ethernet frame identified by the buffer and using the
 *               provided MAC address as source MAC address of the Ethernet frame.
 *  \param[in]   CtrlIdx               EthIf controller index
 *  \param[in]   BufIdx                Index to identify the buffer for frame transmission
 *  \param[in]   FrameType             EtherType to insert into the Ethernet frame header
 *  \param[in]   TxConfirmation        Request for a transmission confirmation:
 *                                     FALSE - no confirmation desired
 *                                     TRUE - confirmation desired
 *  \param[in]   LenByte               Payload length to be transmitted
 *  \param[in]   DstMacAddrPtr         Destination MAC address
 *  \param[in]   SrcMacAddrPtr         Source MAC address:
 *                                     MAC address as defined by IEEE802.3 - using this MAC address as source MAC address
 *                                     NULL_PTR - using the Ethernet controllers MAC address as source MAC address
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \pre         EthIf controller was set in ACTIVE state by EthIf_SetControllerMode()
 *  \pre         Buffer to be transmitted must be previously acquired by
 *               EthIf_ProvideTxBuffer()/EthIf_ProvideExtTxBuffer()
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111170
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_VTransmit(
        uint8                                      CtrlIdx,
        uint8                                      BufIdx,
        Eth_FrameType                              FrameType,
        boolean                                    TxConfirmation,
        uint16                                     LenByte,
  P2VAR(uint8,        AUTOMATIC, ETHIF_APPL_DATA)  DstMacAddrPtr,
  P2VAR(uint8,        AUTOMATIC, ETHIF_APPL_DATA)  SrcMacAddrPtr);

/**********************************************************************************************************************
 *  EthIf_Transmit
 *********************************************************************************************************************/
/*! \brief       Triggers transmission of an Ethernet frame with the Ethernet controllers source MAC address
 *  \details     Function triggers the transmission of an Ethernet frame identified by the buffer and using the
 *               MAC address of the Ethernet controller as source MAC address of the Ethernet frame.
 *  \param[in]   CtrlIdx               EthIf controller index
 *  \param[in]   BufIdx                Index to identify the buffer for frame transmission
 *  \param[in]   FrameType             EtherType to insert into the Ethernet frame header
 *  \param[in]   TxConfirmation        Request for a transmission confirmation:
 *                                     FALSE - no confirmation desired
 *                                     TRUE - confirmation desired
 *  \param[in]   LenByte               Payload length to be transmitted
 *  \param[in]   PhysAddrPtr           Destination MAC address
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \pre         EthIf controller was set in ACTIVE state by EthIf_SetControllerMode()
 *  \pre         Buffer to be transmitted must be previously acquired by
 *               EthIf_ProvideTxBuffer()/EthIf_ProvideExtTxBuffer()
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111170
 *  \trace SPEC-2393641
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_Transmit(
        uint8                                      CtrlIdx,
        uint8                                      BufIdx,
        Eth_FrameType                              FrameType,
        boolean                                    TxConfirmation,
        uint16                                     LenByte,
  P2VAR(uint8,        AUTOMATIC, ETHIF_APPL_DATA)  PhysAddrPtr);

#if ( ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  EthIf_SwitchPortGroupRequestMode
 *********************************************************************************************************************/
/*! \brief       Requests a mode for the EthIfSwtPortGroup
 *  \details     Function requests a mode for the EthIfSwtPortGroup. The call shall be forwarded to EthSwt by
 *               calling EthSwt_SetSwitchPortMode for all EthSwtPorts referenced by the port group.
 *  \param[in]   PortGroupIdx          Index of the port group within the context of the Ethernet Interface
 *  \param[in]   PortMode              Request for the EthIfSwtPortGroup
 *                                     ETHTRCV_MODE_DOWN - disable the port group
 *                                     ETHTRCV_MODE_ACTIVE - enable the port group
 *  \return      E_OK - success
 *  \return      E_NOT_OK - port group mode could not be changed
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace CREQ-117384
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SwitchPortGroupRequestMode(
  EthIf_SwitchPortGroupIdxType PortGroupIdx,
  EthTrcv_ModeType PortMode);
#endif /* ETHIF_SWT_PORT_GROUP_MODE_REQ_SUPPORT */

# if ( ETHIF_WAKEUP_SUPPORT == STD_ON )
/**********************************************************************************************************************
 * EthIf_SetTransceiverWakeupMode
 **********************************************************************************************************************/
/*! \brief       Changes the wakeup mode of the related hardware driver.
 *  \details     Function allows to change the wakeup mode of the related hardware driver by redirecting the call
 *               depending on the passed EcuM wakeup source.
 *  \param[in]   WakeupSource    EcuM wakeup source
 *  \param[in]   WakeupMode      Wakeup mode to set
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111208
 *  \trace SPEC-2393948
 *  \trace SPEC-2393863
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetTransceiverWakeupMode(
  EcuM_WakeupSourceType   WakeupSource,
  EthTrcv_WakeupModeType  WakeupMode);
 
/**********************************************************************************************************************
 * EthIf_GetTransceiverWakeupMode
 **********************************************************************************************************************/
/*! \brief       Retrieves the wakeup mode of the related hardware driver.
 *  \details     Function allows to retrieve the wakeup mode of the related hardware driver by redirecting the call
 *               depending on the passed EcuM wakeup source.
 *  \param[in]   WakeupSource    EcuM wakeup source
 *  \param[out]  WakeupModePtr   Pointer pointing to variable where the wakeup mode is stored to
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111209
 *  \trace SPEC-2393950
 *  \trace SPEC-2393867
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetTransceiverWakeupMode(
        EcuM_WakeupSourceType                               WakeupSource,
  P2VAR(EthTrcv_WakeupModeType, AUTOMATIC, ETHIF_APPL_VAR)  WakeupModePtr);

/**********************************************************************************************************************
 * EthIf_CheckWakeup
 **********************************************************************************************************************/
/*! \brief       Initiates the wakeup check.
 *  \details     Function allows to initiate the wakeup check of the related hardware driver by redirecting the call
 *               depending on the passed EcuM wakeup source.
 *  \param[in]   WakeupSource    EcuM wakeup source
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111210
 *  \trace SPEC-2393868
 *  \trace SPEC-2393871
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_CheckWakeup(
  EcuM_WakeupSourceType  WakeupSource);

/**********************************************************************************************************************
 * EthIf_VSetTransceiverWakeupMode
 **********************************************************************************************************************/
/*! \brief       Changes the wakeup mode of the related hardware drivers.
 *  \details     Function allows to change the wakeup mode of the related hardware drivers by redirecting the call
 *               depending on the passed EcuM wakeup source.
 *  \param[in]   CtrlIdx         EthIf controller index
 *  \param[in]   WakeupMode      Wakeup mode to set
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111208
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_VSetTransceiverWakeupMode(
  uint8                   CtrlIdx,
  EthTrcv_WakeupModeType  WakeupMode);

/**********************************************************************************************************************
 * EthIf_VGetTransceiverWakeupMode
 **********************************************************************************************************************/
/*! \brief       Retrieves the wakeup mode of the related hardware driver.
 *  \details     Function allows to retrieve the wakeup mode of the related hardware driver by redirecting the call
 *               depending on the passed EcuM wakeup source.
 *  \param[in]   CtrlIdx         EthIf controller index
 *  \param[out]  WakeupModePtr   Pointer pointing to variable where the wakeup mode is stored to
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111209
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_VGetTransceiverWakeupMode(
        uint8                                               CtrlIdx,
  P2VAR(EthTrcv_WakeupModeType, AUTOMATIC, ETHIF_APPL_VAR)  WakeupModePtr);

/**********************************************************************************************************************
 * EthIf_VCheckWakeup
 **********************************************************************************************************************/
/*! \brief       Initiates the wakeup check.
 *  \details     Function allows to initiate the wakeup check of the related hardware drivers by redirecting the call
 *               depending on the passed EthIf controller index.
 *  \param[in]   CtrlIdx    EthIf controller index
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111210
 **********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_VCheckWakeup(
  uint8  CtrlIdx);

# endif /* ETHIF_WAKEUP_SUPPORT */

# if ( ETHIF_DYNAMIC_TRAFFIC_SHAPING_SUPPORT == STD_ON )
/**********************************************************************************************************************
 * EthIf_SetBandwidthLimit
 **********************************************************************************************************************/
/*! \brief       Manipulates the maximum bandwidth of a traffic queue.
 *  \details     Function allows to manipulate the maximum amount of bandwidth the indexed traffic queue is allowed
 *               to acquire.
 *  \param[in]   CtrlIdx         EthIf virtual controller index
 *  \param[in]   QueuePrio       Traffic queue index
 *  \param[in]   BandwidthLimit  New bandwidth limit [bit/s]
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111285
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_SetBandwidthLimit(
  uint8   CtrlIdx,
  uint8   QueuePrio,
  uint32  BandwidthLimit);

/**********************************************************************************************************************
 * EthIf_GetBandwidthLimit
 **********************************************************************************************************************/
/*! \brief       Retrieves the current maximum bandwidth of a traffic queue.
 *  \details     Function allows to retrieve the maximum amount of bandwidth the indexed traffic queue is allowed
 *               to acquire currently.
 *  \param[in]   CtrlIdx            EthIf virtual controller index
 *  \param[in]   QueuePrio          Traffic queue index
 *  \param[out]  BandwidthLimitPtr  Current bandwidth limit [bit/s]
 *  \return      E_OK - success
 *  \return      E_NOT_OK - function has been called with invalid parameters
 *  \pre         Module and drivers have been initialized
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111286
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetBandwidthLimit(
        uint8                               CtrlIdx,
        uint8                               QueuePrio,
  P2VAR(uint32, AUTOMATIC, ETHIF_APPL_VAR)  BandwidthLimitPtr);

# endif /* ETHIF_DYNAMIC_TRAFFIC_SHAPING_SUPPORT */

/**********************************************************************************************************************
 * EthIf_GetTxStats
 **********************************************************************************************************************/
/*! \brief       Retrieves the transmission statistic counters.
 *  \details     Function redirects call to the Eth-driver to retrieve the transmission statistic counters defined
 *               by AUTOSAR (see Eth-driver for more details). EthIf extends these statistics by additional counters for
 *               the respective EthIf-controller. The counters provided by EthIf within the EthIf_TxStatsType are cleared
 *               on read.
 *  \param[in]   CtrlIdx       EthIf controller identifier
 *  \param[out]  EthTxStats    Transmission statistics of the respective Eth-controller
 *  \param[out]  EthIfTxtSats  Transmission statistics of the EthIf-controller
 *  \return      E_NOT_OK - Wrong parameters or Eth-driver call isn't successful
 *  \return      E_OK - Statistics could be retrieved
 *  \pre         Module is initiliazed
 *  \pre         Respective EthIf controller is initialized
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-123106
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetTxStats(
        uint8                                    CtrlIdx,
  P2VAR(Eth_TxStatsType,   AUTOMATIC, AUTOMATIC) EthTxStats,
  P2VAR(EthIf_TxStatsType, AUTOMATIC, AUTOMATIC) EthIfTxStats);

/**********************************************************************************************************************
 * EthIf_GetTxStats
 **********************************************************************************************************************/
/*! \brief       Retrieves the reception statistic counters.
 *  \details     Function redirects call to the Eth-driver to retrieve the reception statistic counters defined
 *               by AUTOSAR (see Eth-driver for more details). EthIf extends these statistics by additional counters for
 *               the respective EthIf-controller. The counters provided by EthIf within the EthIf_RxStatsType are cleared
 *               on read.
 *  \param[in]   CtrlIdx       EthIf controller identifier
 *  \param[out]  EthRxStats    Reception statistics of the respective Eth-controller
 *  \param[out]  EthIfRxtSats  Reception statistics of the EthIf-controller
 *  \return      E_NOT_OK - Wrong parameters or Eth-driver call isn't successful
 *  \return      E_OK - Statistics could be retrieved
 *  \pre         Module is initiliazed
 *  \pre         Respective EthIf controller is initialized
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-123106
 **********************************************************************************************************************/
FUNC(Std_ReturnType, ETHIF_CODE) EthIf_GetRxStats(
        uint8                                    CtrlIdx,
  P2VAR(Eth_RxStatsType,   AUTOMATIC, AUTOMATIC) EthRxStats,
  P2VAR(EthIf_RxStatsType, AUTOMATIC, AUTOMATIC) EthIfRxStats);

/**********************************************************************************************************************
 *  EthIf_MainFunctionRx
 *********************************************************************************************************************/
/*! \brief       Reception Main Function
 *  \details     Main function to handle Ethernet frame reception in polling mode.
 *  \pre         none
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace SPEC-2393554
 *  \trace SPEC-2393665
 *********************************************************************************************************************/
/* Function declaration is provided by SchM */

/**********************************************************************************************************************
 *  EthIf_MainFunctionTx
 *********************************************************************************************************************/
/*! \brief       Transmission confirmation Main Function
 *  \details     Main function to handle Ethernet frame transmission confirmation in polling mode.
 *  \pre         none
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace SPEC-2393554
 *  \trace SPEC-2393669
 *********************************************************************************************************************/
/* Function declaration is provided by SchM */

/**********************************************************************************************************************
 *  EthIf_MainFunctionState
 *********************************************************************************************************************/
/*! \brief       Link state supervision Main Function
 *  \details     Main function to monitor link state changes of the managed hardware elements.
 *  \pre         none
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace
 *********************************************************************************************************************/
# if ( ETHIF_ENABLE_MAIN_FUNCTION_STATE == STD_ON )
/* Function declaration is provided by SchM */
# else
FUNC(void, ETHIF_CODE) EthIf_MainFunctionState( void );
# endif /* ETHIF_ENABLE_MAIN_FUNCTION_STATE */

# if ( ETHIF_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  EthIf_GetVersionInfo
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     EthIf_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  VersionInfoPtr        Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         none
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace CREQ-111173
 *  \trace SPEC-2393649
 *********************************************************************************************************************/
FUNC(void, ETHIF_CODE) EthIf_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ETHIF_APPL_DATA)  VersionInfoPtr);
# endif /* ETHIF_VERSION_INFO_API */

# define ETHIF_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* ETHIF_H */

/**********************************************************************************************************************
 *  END OF FILE: EthIf.h
 *********************************************************************************************************************/

