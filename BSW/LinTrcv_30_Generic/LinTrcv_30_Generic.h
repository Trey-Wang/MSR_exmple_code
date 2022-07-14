/**********************************************************************************************************************
 * COPYRIGHT
 * -------------------------------------------------------------------------------------------------------------------
 * \verbatim
 * Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *               This software is copyright protected and proprietary to Vector Informatik GmbH.
 *               Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *               All other rights remain with Vector Informatik GmbH.
 * \endverbatim
 * -------------------------------------------------------------------------------------------------------------------
 * FILE DESCRIPTION
 * -------------------------------------------------------------------------------------------------------------------
 *         File:  LinTrcv_30_Generic.h
 *      Project:  MICROSAR
 *       Module:  LIN Transceiver Driver
 *  Description:  Header of the MICROSAR LIN Transceiver Driver
 **********************************************************************************************************************/
/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Edgar Tongoona                viseta        Vector Informatik GmbH
 *  Nessim Ben Ammar              visneb        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY CORE PART
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  02.00.00  2012-07-20  viseta  ESCAN00060196 Implementation of High Level AUTOSAR 4 LIN Transceiver Driver
 *  02.00.01  2012-11-28  viseta  ESCAN00063441 AR4-220: Remove STATIC
 *  02.01.00  2013-04-05  viseta  ESCAN00066634 Support mode transitions from any to any mode
 *  02.02.00  2013-09-26  viseta  ESCAN00070736 AR4-260: Remove ProdErrorDetection configuration switch
 *                                ESCAN00070747 Report any wakeup and not wakeup by bus only
 *  02.03.00  2014-02-07  viseta  ESCAN00073533 Wakeup notification - Use ICU APIs to enable / disable wakeup event notifications
 *                                ESCAN00071360 AR4-569: [LinTrcv]  The  Lin_GeneralTypes.h, Icu.h and Spi.h are missing in header
 *                                ESCAN00073755 Wrong wakeup reason set in case of wakeup by PIN
 *  03.00.00  2014-09-23  viseta  ESCAN00076483 AR4-832: External wake up via LIN transceiver
 *                                ESCAN00076514 AR4-698: Post-Build Selectable (Identity Manager)
 *  03.01.00  2014-09-23  viseta  ESCAN00084179 Support for LIN Transceiver on System-Basis Chips (SBCs)
 *  03.01.01  2015-10-20  viseta  ESCAN00085981 Compiler warnings when using different transceiver types within the same stack
 *  04.00.00  2015-07-08  viseta  ESCAN00080311 FEAT-427: SafeBSW Step I
 *                                ESCAN00079318 ASR4: If Tx pin is low transition from standby to normal mode not possible
 *                                ESCAN00081796 FEAT-1275: SafeBSW Step 2
 *                                ESCAN00084179 Support for LIN Transceiver on System-Basis Chips (SBCs)
 *  05.00.00  2015-11-12  viseta  ESCAN00085981 Compiler warnings when using different transceiver types within the same stack
 *                                ESCAN00086471 Adapt to updated R14 generators (breaking change and CSL)
 *  05.01.00  2016-03-21  viseta  ESCAN00087815 FEAT-1688: SafeBSW Step 4
 *                                ESCAN00089405 Wrong data type used in LinTrcv_SetWakeupMode
 *  05.01.01  2016-11-24  visneb  ESCAN00092871 ECU does not send the wakeup frame
 *  05.02.00  2017-02-23  visneb  ESCAN00094135 Cannot store wakeup events unless wakeup notification is enabled
 *                                ESCAN00094136 Added channel specific check for wakeup by bus
 *                                ESCAN00094144 Added channel specific check for wakeup by pin
 *                                ESCAN00094137 Extended invalid network DETs to include unused transceiver channels
 *                                ESCAN00094140 Removed support for standby mode
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Edgar Tongoona                viseta        Vector Informatik GmbH
 *  Nessim Ben Ammar              visneb        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  02.00.00  2013-04-05  viseta  ESCAN00066635  Implement new AUTOSAR 4 Generic LIN transceiver driver
 *  02.01.00  2013-09-26  viseta                 HL Part was updated
 *  02.02.00  2014-02-07  viseta                 HL Part was updated
 *  03.00.00  2014-10-10  viseta  ESCAN00078866  AR4-698: Post-Build Selectable (Identity Manager)
 *  04.00.00  2015-07-08  viseta                 HL Part was updated
 *  05.00.00  2015-11-12  viseta                 Adapt to updated R14 generators (breaking change and CSL)
 *  05.01.00  2017-03-27  visneb                 HL Part was updated
 *                                ESCAN00094516  Exchanged all place holders with Generic / GENERIC
 *********************************************************************************************************************/

#ifndef LINTRCV_30_GENERIC_H
#define LINTRCV_30_GENERIC_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "ComStack_Types.h"
#include "Lin_GeneralTypes.h"
#include "LinTrcv_30_Generic_Cfg.h"

/* ADD YOUR ADDITIONAL INCLUDES HERE */


/* DO NOT CHANGE THE CODE BELOW */


/**********************************************************************************************************************
 * MODULE VERSION
 *********************************************************************************************************************/
#define LINTRCV_30_GENERIC_AR_RELEASE_MAJOR_VERSION       0x04u
#define LINTRCV_30_GENERIC_AR_RELEASE_MINOR_VERSION       0x00u
#define LINTRCV_30_GENERIC_AR_RELEASE_REVISION_VERSION    0x03u

#define DRVTRANS_GENERICDIOASR_VERSION                   0x0501u
#define DRVTRANS_GENERICDIOASR_RELEASE_VERSION           0x00u

#define LINTRCV_30_GENERIC_SW_MAJOR_VERSION                 ((DRVTRANS_GENERICDIOASR_VERSION & 0xFF00u) >> 8)
#define LINTRCV_30_GENERIC_SW_MINOR_VERSION                 (DRVTRANS_GENERICDIOASR_VERSION & 0x00FFu)
#define LINTRCV_30_GENERIC_SW_PATCH_VERSION                 (DRVTRANS_GENERICDIOASR_RELEASE_VERSION)

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define LINTRCV_30_GENERIC_VENDOR_ID                     30u
#define LINTRCV_30_GENERIC_MODULE_ID                     64u

/* ApiIDs for Error Reporting */
#define LINTRCV_30_GENERIC_SID_INIT                      0x00u
#define LINTRCV_30_GENERIC_SID_SETOPMODE                 0x01u
#define LINTRCV_30_GENERIC_SID_GETOPMODE                 0x02u
#define LINTRCV_30_GENERIC_SID_GETBUSWUREASON            0x03u
#define LINTRCV_30_GENERIC_SID_GETVERSIONINFO            0x04u
#define LINTRCV_30_GENERIC_SID_SETWAKEUPMODE             0x05u
#define LINTRCV_30_GENERIC_SID_CHECKWAKEUP               0x07u


#define LINTRCV_30_GENERIC_OP_MODE_NORMAL                LINTRCV_TRCV_MODE_NORMAL
#define LINTRCV_30_GENERIC_OP_MODE_STANDBY               LINTRCV_TRCV_MODE_STANDBY
#define LINTRCV_30_GENERIC_OP_MODE_SLEEP                 LINTRCV_TRCV_MODE_SLEEP

/* Development error codes */
#define LINTRCV_30_GENERIC_E_NO_ERROR                    0x00u
#define LINTRCV_30_GENERIC_E_INVALID_LIN_NETWORK         0x01u
#define LINTRCV_30_GENERIC_E_PARAM_POINTER               0x02u
#define LINTRCV_30_GENERIC_E_UNINIT                      0x11u
#define LINTRCV_30_GENERIC_E_TRCV_NOT_SLEEP              0x21u
#define LINTRCV_30_GENERIC_E_TRCV_NOT_NORMAL             0x22u
#define LINTRCV_30_GENERIC_E_PARAM_TRCV_WAKEUP_MODE      0x23u
#define LINTRCV_30_GENERIC_E_PARAM_TRCV_OPMODE           0x24u
#define LINTRCV_30_GENERIC_E_INVALID_TRCV_OPMODE         0x25u

/* ADD YOUR ADDITIONAL GLOBAL CONSTANT MACROS HERE */


/* DO NOT CHANGE THE CODE BELOW */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* ADD YOUR ADDITIONAL GLOBAL FUNCTION MACROS HERE */


/* DO NOT CHANGE THE CODE BELOW */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* ADD YOUR ADDITIONAL GLOBAL DATA TYPES AND STRUCTURES HERE */


/* DO NOT CHANGE THE CODE BELOW */

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define LINTRCV_30_GENERIC_START_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
extern CONST(uint8, LINTRCV_30_GENERIC_CONST) LinTrcv_30_Generic_MainVersion;
extern CONST(uint8, LINTRCV_30_GENERIC_CONST) LinTrcv_30_Generic_SubVersion;
extern CONST(uint8, LINTRCV_30_GENERIC_CONST) LinTrcv_30_Generic_ReleaseVersion;
#define LINTRCV_30_GENERIC_STOP_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ADD YOUR ADDITIONAL GLOBAL DATA PROTOTYPES HERE */


/* DO NOT CHANGE THE CODE BELOW */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define LINTRCV_30_GENERIC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  LinTrcv_30_Generic_InitMemory()
 **********************************************************************************************************************/
/*!
 * \brief       Initialize all necessary variables for the LIN transceiver.
 * \details     Service to initialize module global variables at power up. This function initializes the
 *              variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 * \pre         Module is uninitialized.
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
 * \note        Function must be called in case variables are not initialized after reset (i.e. by startup code).
 *              This service function has to be called before LinTrcv_30_Generic_Init() function.
 **********************************************************************************************************************/
extern FUNC(void, LINTRCV_30_GENERIC_CODE) LinTrcv_30_Generic_InitMemory
(
  void
);

/***********************************************************************************************************************
 *  LinTrcv_30_Generic_Init()
 **********************************************************************************************************************/
/*!
 * \brief       Initialization of the LinTrcv module.
 * \details     This function initializes the module LinTrcv. It initializes all variables and sets the module state to
 *              initialized.
 * \param[in]   ConfigPtr               Pointer to the LIN Transceiver configuration
 * \pre         Module is uninitialized.
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
 * \note        This function has the service id 0x00.
 **********************************************************************************************************************/
extern FUNC(void, LINTRCV_30_GENERIC_CODE) LinTrcv_30_Generic_Init
(
  P2CONST(LinTrcv_30_Generic_ConfigType, AUTOMATIC, LINTRCV_30_GENERIC_INIT_DATA) ConfigPtr
);

/***********************************************************************************************************************
 *  LinTrcv_30_Generic_SetOpMode()
 **********************************************************************************************************************/
/*!
 * \brief       Sets the transceiver to requested mode.
 * \details     This function requests the transceiver on the selected channel to set the given mode.
 * \param[in]   LinTrcvIndex            Identification of the network channel
 * \param[in]   OpMode                  Requested transceiver mode
 * \return      E_OK                    Request of transceiver state changed accepted.
 * \return      E_NOT_OK                Request has not been accepted, previous state not changed.
 * \pre         This module must be initialized before this service function is called.
 * \context     TASK
 * \reentrant   TRUE for different LinTrcvIndex
 * \synchronous TRUE
 * \note        This function has the service id 0x01.
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, LINTRCV_30_GENERIC_CODE) LinTrcv_30_Generic_SetOpMode
(
  VAR(uint8, AUTOMATIC) LinTrcvIndex,
  VAR(LinTrcv_TrcvModeType, AUTOMATIC) OpMode
);

/***********************************************************************************************************************
 *  LinTrcv_30_Generic_GetOpMode()
 **********************************************************************************************************************/
/*!
 * \brief       Gets the transceiver mode.
 * \details     This function gets the current transceiver mode on the selected channel.
 * \param[in]   LinTrcvIndex            Identification of the network channel
 * \param[out]  OpMode                  Pointer to memory location for requested transceiver mode to be stored.
 *                                      Parameter must not be NULL.
 * \return      E_OK                    Request has been accepted and transceiver mode is stored in OpMode.
 * \return      E_NOT_OK                Request has not been accepted, OpMode is not valid.
 * \pre         This module must be initialized before this service function is called.
 * \context     TASK
 * \reentrant   TRUE for different LinTrcvIndex
 * \synchronous TRUE
 * \note        This function has the service id 0x02.
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, LINTRCV_30_GENERIC_CODE) LinTrcv_30_Generic_GetOpMode
(
  VAR(uint8, AUTOMATIC) LinTrcvIndex,
  P2VAR(LinTrcv_TrcvModeType, AUTOMATIC, LINTRCV_30_GENERIC_APPL_VAR) OpMode
);

/***********************************************************************************************************************
 *  LinTrcv_30_Generic_GetBusWuReason()
 **********************************************************************************************************************/
/*!
 * \brief       Gets the transceiver WakeupReason.
 * \details     This function gets the reason for Wakeup detected by transceiver on the selected channel.
 * \param[in]   LinTrcvIndex            Identification of the network channel
 * \param[out]  Reason                  Pointer to memory location for transceiver WakeupReason to be stored.
 *                                      Parameter must not be NULL.
 * \return      E_OK                    Request has been accepted and WakeupReason is stored in Reason.
 * \return      E_NOT_OK                Request has not been accepted, Reason is not valid.
 * \pre         This module must be initialized before this service function is called.
 * \context     TASK
 * \reentrant   TRUE for different LinTrcvIndex
 * \synchronous TRUE
 * \note        This function has the service id 0x03.
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, LINTRCV_30_GENERIC_CODE) LinTrcv_30_Generic_GetBusWuReason
(
  VAR(uint8, AUTOMATIC) LinTrcvIndex,
  P2VAR(LinTrcv_TrcvWakeupReasonType, AUTOMATIC, LINTRCV_30_GENERIC_APPL_VAR) Reason
);

/***********************************************************************************************************************
 *  LinTrcv_30_Generic_SetWakeupMode()
 **********************************************************************************************************************/
/*!
 * \brief       Sets the transceiver Wakeup mode to requested mode.
 * \details     This function sets the Wakeup mode of the transceiver on the selected channel.
 * \param[in]   LinTrcvIndex            Identification of the network channel
 * \param[in]   TrcvWakeupMode          Requested transceiver Wakeup mode
 * \return      E_OK                    Request of transceiver Wakeup mode change has been accepted.
 * \return      E_NOT_OK                Request has not been accepted, transceiver Wakeup mode is not changed.
 * \pre         This module must be initialized before this service function is called.
 * \context     TASK
 * \reentrant   TRUE for different LinTrcvIndex
 * \synchronous TRUE
 * \note        This function has the service id 0x05
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, LINTRCV_30_GENERIC_CODE) LinTrcv_30_Generic_SetWakeupMode
(
  VAR(uint8, AUTOMATIC) LinTrcvIndex,
  VAR(LinTrcv_TrcvWakeupModeType, AUTOMATIC) TrcvWakeupMode
);

/***********************************************************************************************************************
 *  LinTrcv_30_Generic_CheckWakeup()
 **********************************************************************************************************************/
/*!
 * \brief       This API is called when the EcuM has been notified about a Wakeup on a specific channel.
 * \details     This function is called when the EcuM has been notified about a Wakeup on a specific channel. The source
 *              of the Wakeup is recognized and LinTrcv is called for Wakeup validation.
 * \param[in]   LinTrcvIndex             Identification of the network channel
 * \return      E_OK                     Request has been accepted, source of Wakeup belongs to a LIN channel.
 * \return      E_NOT_OK                 Request has not been accepted, Wakeup source invalid.
 * \pre         This module must be initialized before this service function is called.
 * \context     TASK|ISR2
 * \reentrant   TRUE for different LinTrcvIndex
 * \synchronous TRUE
 * \note        Wakeup support has to be enabled for LIN channel. This function has the service id 0x07.
 **********************************************************************************************************************/
extern FUNC(Std_ReturnType, LINTRCV_30_GENERIC_CODE) LinTrcv_30_Generic_CheckWakeup
(
  VAR(uint8, AUTOMATIC) LinTrcvIndex
);

#if (LINTRCV_30_GENERIC_GET_VERSION_INFO == STD_ON)
/***********************************************************************************************************************
 *  LinTrcv_30_Generic_GetVersionInfo()
 **********************************************************************************************************************/
/*!
 * \brief       Returns the version information.
 * \details     This function returns version information, vendor ID and AUTOSAR module ID of the component.
 * \param[out]  VersionInfo               Pointer to where to store the version information. Parameter must not be NULL.
 *                                        The version information are in decimal.
 * \pre         --
 * \context     TASK
 * \reentrant   TRUE for different LinTrcvIndex
 * \synchronous TRUE
 * \config      LINTRCV_30_GENERIC_GET_VERSION_INFO
 * \note        Configuration parameter LinTrcvGetVersionInfo must be enabled. This function has the service id 0x04.
 **********************************************************************************************************************/
extern FUNC(void, LINTRCV_30_GENERIC_CODE) LinTrcv_30_Generic_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, LINTRCV_30_GENERIC_APPL_VAR) VersionInfo
);
#endif

#if(LINTRCV_30_GENERIC_WAIT_COUNT_USED  == STD_ON)
/***********************************************************************************************************************
 *  Appl_LinTrcv_30_Generic_Wait()
 **********************************************************************************************************************/
/*!
 * \brief       Is called duration transition between states to add a delay between the state transitions
 * \details     Must be implemented by the user.
 * \param[in]   LinTrcvWaitCount    Wait count for transceiver state changes.
 * \pre         This module must be initialized before this service function is called and LinTrcv_30_Generic_SetOpMode()
 *              is called normal mode as requested mode
 * \context     TASK
 * \reentrant   TRUE for different LinTrcvIndex
 * \synchronous TRUE
 * \config      LINTRCV_30_GENERIC_WAIT_COUNT_USED
 * \note        Configuration parameter LinTrcvWaitCount must have a value greater than zero
 **********************************************************************************************************************/
extern FUNC(void, LINTRCV_30_GENERIC_CODE) Appl_LinTrcv_30_Generic_Wait
(
  VAR(uint32, AUTOMATIC) LinTrcvWaitCount
);
#endif

/* ADD YOUR ADDITIONAL GLOBAL FUNCTION PROTOTYPES HERE */


/* DO NOT CHANGE THE CODE BELOW */

#define LINTRCV_30_GENERIC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif

/*! \exclusivearea LINTRCV_30_GENERIC_EXCLUSIVE_AREA_0
 * Ensures that the HW and SW states remain consistent with oneanother,
 * prohibits that the HW is simultaniously accessed by two threads
 * \protects LinTrcv_30_Generic_ChannelData and  the HW access of the transceiver
 * \usedin LinTrcv_30_Generic_SetOpMode, LinTrcv_30_Generic_CheckWakeup
 * \exclude All functions provided by LINTRCV_30_GENERIC
 * \recommendation Do not use global locks
 * \length LONG Code will propagate though other layers and drivers.
 * \endexclusivearea
 */

 /**********************************************************************************************************************
 *  END OF FILE: LinTrcv_30_Generic.h
 **********************************************************************************************************************/
