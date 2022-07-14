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
/*!        \file  FrTSyn.h
 *        \brief  Vector AUTOSAR FrTSyn header file
 *
 *      \details  This is the main header file of the Vector AUTOSAR module FrTSyn.

 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Stephanie Schaaf              visssf        Vector Informatik GmbH
 *  Thilo Rachlitz                vistra        Vector Informatik GmbH
 *  Martin Nonnenmann             vismno        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2014-12-04  visssf  ESCAN00077543 FEAT-971: Provide module FrTSyn for global time synchronization over FlexRay
 *  01.00.01  2015-03-20  visssf  ESCAN00081941 Wrong calculation of current local time of Time Slave
 *  02.00.00  2015-09-22  visssf  -             Updated Generator Version to 2.0 (Java 8 and R14 CFG5 Breaking Changes)
 *  02.01.00  2016-02-26  visssf  ESCAN00088578 FEAT-1694: Support multiple masters per Time Domain
 *            2016-03-01  visssf  ESCAN00087839 FEAT-1631: Trigger Transmit API with SduLength In/Out according to ASR4.2.2
 *            2016-03-15  visssf  ESCAN00088704 Wrong calculation and validation of Sequence Counter
 *            2016-03-22  visssf  ESCAN00087328 MISRA deviation: MISRA-C:2004 Rules 1.1, 5.6, 14.3, 14.1, 19.7, 10.1 and HIS Metrics
 *  03.00.00  2017-04-13  vistra  STORYC-252    Update BSWMD Files to AR 4.3
 *  03.01.00  2017-05-15  visssf  STORYC-129    Tx state machine shall support Multiple Time Domains
 *  03.02.00  2017-06-19  visssf  ESCAN00090441 Wrong CRC calculation and validation
 *            2017-06-20  vismno  STORYC-115    Debounce Time introduction
 *            2017-06-20  vismno  ESCAN00095192 Wrong validation of Sequence Counter after overflow
 *            2017-06-22  visssf  STORYC-1210   Global Time Precision Measurement Support
 *            2017-06-23  vistra  ESCAN00095253 Missing initialization of FrTSyn_LastTransmitFrCycle
 *            2017-06-26  vistra  ESCAN00095418 Time slave not correctly synchronized to the time master
 *  03.03.00  2017-08-03  vistra  STORYC-112    Immediate Time Synchronization
 *            2017-08-23  visssf  ESCAN00090314 OS API called with disabled interrupts
 *********************************************************************************************************************/

#if !defined (FRTSYN_H)
# define FRTSYN_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "FrTSyn_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define FRTSYN_VENDOR_ID                        (30U)
# define FRTSYN_MODULE_ID                        (0xA3U)

/* AUTOSAR Software specification version information */
# define FRTSYN_AR_RELEASE_MAJOR_VERSION         (0x04U)
# define FRTSYN_AR_RELEASE_MINOR_VERSION         (0x03U)
# define FRTSYN_AR_RELEASE_REVISION_VERSION      (0x00U)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define FRTSYN_SW_MAJOR_VERSION                 (3U)
# define FRTSYN_SW_MINOR_VERSION                 (3U)
# define FRTSYN_SW_PATCH_VERSION                 (0U)

# define FRTSYN_INSTANCE_ID_DET                  (0x00U)

/* ----- API service IDs ----- */
# define FRTSYN_SID_INIT                         (0x01U) /*!< Service ID: FrTSyn_Init() */
# define FRTSYN_SID_GET_VERSION_INFO             (0x02U) /*!< Service ID: FrTSyn_GetVersionInfo() */
# define FRTSYN_SID_SET_TRANSMISSION_MODE        (0x03U) /*!< Service ID: FrTSyn_SetTransmissionMode() */
# define FRTSYN_SID_MAIN_FUNCTION                (0x04U) /*!< Service ID: FrTSyn_MainFunction() */
# define FRTSYN_SID_TRIGGER_TRANSMIT             (0x41U) /*!< Service ID: FrTSyn_TriggerTransmit() */
# define FRTSYN_SID_RX_INDICATION                (0x42U) /*!< Service ID: FrTSyn_RxIndication() */

# define FRTSYN_SID_TIMESTAMP_PLUS_TIMESTAMP     (0xA1U) /*!< Service ID: FrTSyn_TimestampPlusTimestamp() */
# define FRTSYN_SID_TIMESTAMP_MINUS_TIMESTAMP    (0xA2U) /*!< Service ID: FrTSyn_TimestampMinusTimestamp() */
# define FRTSYN_SID_CONVERT_FRTICKS_TO_TIMESTAMP (0xA3U) /*!< Service ID: FrTSyn_ConvertFrTicksToTimestamp() */

/* ----- Error codes ----- */
# define FRTSYN_E_NO_ERROR                       (0x00U) /*!< used to check if no error occurred */
# define FRTSYN_E_INVALID_PDUID                  (0x01U) /*!< Error code: API service called with wrong PDU or SDU  */
# define FRTSYN_E_NOT_INITIALIZED                (0x20U) /*!< Error code: API service used in un-initialized state */
# define FRTSYN_E_NULL_POINTER                   (0x21U) /*!< Error code: API service called with NULL_PTR as parameter  */

/* ----- Modes ----- */
# define FRTSYN_UNINIT                           (0x00U)
# define FRTSYN_INIT                             (0x01U)

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

# define FRTSYN_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  FrTSyn_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Function for *_INIT_*-variable initialization
 *  \details     Service to initialize module global variables at power up. This function can be used to initialize the
 *               variables in *_INIT_* sections in case they are not initialized by the startup code.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE 
 *  \synchronous TRUE
 **********************************************************************************************************************/
FUNC(void, FRTSYN_CODE) FrTSyn_InitMemory(void); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  FrTSyn_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization function
 *  \details     This function initializes the module FrTSyn. It initializes all variables and sets the module state to
 *               initialized.
 *  \param[in]   configPtr               Configuration structure for initializing the module.
 *  \pre         Interrupts are disabled.
 *  \pre         FrTSyn_InitMemory has been called unless FrTSyn_ModuleInitialized is initialized by start-up code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Specification of module initialization
 *********************************************************************************************************************/
FUNC(void, FRTSYN_CODE) FrTSyn_Init(P2CONST(FrTSyn_ConfigType, AUTOMATIC, FRTSYN_APPL_DATA) configPtr); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  FrTSyn_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     FrTSyn_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, FRTSYN_CODE) FrTSyn_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, FRTSYN_APPL_VAR) versioninfo); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  FrTSyn_MainFunction()
 *********************************************************************************************************************/
/*! \brief       Main function
 *  \details     This function processes cyclic tasks of the FrTSyn module.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, FRTSYN_CODE) FrTSyn_MainFunction(void); /* PRQA S 0850 */ /* MD_MSR_19.8 */

/**********************************************************************************************************************
 *  FrTSyn_SetTransmissionMode()
 *********************************************************************************************************************/
/*! \brief       Set transmission mode
 *  \details     This function turns the TX capabilities of the FrTSyn on and off.
 *  \param[in]   CtrlIdx                 Index of the FlexRay channel.
 *  \param[in]   Mode                    Transmission on/off.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, FRTSYN_CODE) FrTSyn_SetTransmissionMode(
  uint8 CtrlIdx,
  FrTSyn_TransmissionModeType Mode); /* PRQA S 0850 */ /* MD_MSR_19.8 */

# define FRTSYN_STOP_SEC_CODE
# include "MemMap.h"


#endif  /* FRTSYN_H */

/**********************************************************************************************************************
 *  END OF FILE: FrTSyn.h
 *********************************************************************************************************************/
