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
/**        \file  LinSM.h
 *        \brief  LIN State Manager ASR4
 *
 *      \details  Implementation of the LIN State Manager AUTOSAR Release 4.0
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Marco Pfalzgraf               vismpf        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2012-04-18  vismpf  ESCAN00058512: Initial Creation
 *  01.00.01  2012-11-08  vismpf  ESCAN00062869: Corrected call of LinIf_Wakeup
 *                                ESCAN00063508: AR4-220: Added local STATIC define
 *  01.01.00  2013-04-10  vismpf  ESCAN00066531: Removed unused memory sections
 *  02.00.00  2013-07-22  vismpf  ESCAN00060620: Optimization in case of no sleep support
 *                                ESCAN00069228: Adapted general defines
 *  02.01.00  2014-05-08  vismpf  ESCAN00074303: AR4-612: Retry change to FULL_COM
 *  03.00.00  2014-08-05  vismpf  ESCAN00077623: AR4-698: Post-Build Selectable (Identity Manager)
 *  03.01.00  2014-12-03  vismpf  ESCAN00080029: FEAT-690: LIN schedule end notification [AR4-1001]
 *  04.00.00  2015-01-02  vismpf  ESCAN00080298: FEAT-427: SafeBSW Step I
 *                                ESCAN00081786: FEAT-1275: SafeBSW Step II
 *  05.00.00  2016-01-11  vismpf  ESCAN00087370: Adapted detailed design according to doxygen checker results
 *  05.01.00  2016-03-17  vismpf  ESCAN00087804: FEAT-1688: SafeBSW Step 4
 *                                ESCAN00088956: Added MISRA justification
 *  05.02.00  2017-01-05  vismpf  FEATC-530: FEAT-2362: AUTOSAR 4.2.2 SWS Traceability
 *********************************************************************************************************************/

#if !defined ( LINSM_H )
#define LINSM_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "LinSM_Cfg.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
#define LINSM_VENDOR_ID                           (30u)
#define LINSM_MODULE_ID                           (141u)

/* AUTOSAR Software Specification Version Information */
#define LINSM_AR_RELEASE_MAJOR_VERSION            (0x04u)
#define LINSM_AR_RELEASE_MINOR_VERSION            (0x00u)
#define LINSM_AR_RELEASE_PATCH_VERSION            (0x03u)

/* ----- Component version information (BCD coded) ----- */
#define LINSM_SW_MAJOR_VERSION                    (0x05u)
#define LINSM_SW_MINOR_VERSION                    (0x02u)
#define LINSM_SW_PATCH_VERSION                    (0x00u)

#define LINSM_INSTANCE_ID_DET                     (0x00u)

/* ----- API service IDs ----- */
#define LINSM_SID_INIT                            (0x01u)
#define LINSM_SID_GET_VERSION_INFO                (0x02u)
#define LINSM_SID_SCHEDULE_REQUEST                (0x10u)
#define LINSM_SID_GET_CURRENT_COM_MODE            (0x11u)
#define LINSM_SID_REQUEST_COM_MODE                (0x12u)
#define LINSM_SID_SCHEDULE_REQUEST_CONFIRMATION   (0x20u)
#define LINSM_SID_WAKEUP_CONFIRMATION             (0x21u)
#define LINSM_SID_GOTO_SLEEP_CONFIRMATION         (0x22u)
#define LINSM_SID_SCHEDULE_END_NOTIFICATION       (0x23u)
#define LINSM_SID_MAIN_FUNCTION                   (0x30u)
#define LINSM_SID_SWITCH_TRANSCEIVER              (0x40u)

/* ----- Error codes ----- */
#define LINSM_E_NO_ERROR                          (0xFFu) /*!< used to check if no error occurred - use a value unequal to any error code */
#define LINSM_E_UNINIT                            (0x00u) /*!< Error code: API service used without module initialization */
#define LINSM_E_PARAM_CONFIG                      (0x0Au) /*!< Error code: API service LinSM_Init() called with wrong parameter */
#define LINSM_E_NONEXISTENT_NETWORK               (0x20u) /*!< Error code: API service used with invalid channel identifier or channel was not configured for the functionality of the calling API */
#define LINSM_E_PARAMETER                         (0x30u) /*!< Error code: API service used with invalid parameter */
#define LINSM_E_PARAM_POINTER                     (0x40u) /*!< Error code: API service used with invalid pointer parameter (NULL) */
#define LINSM_E_CONFIRMATION_TIMEOUT              (0x50u) /*!< Error code: A LinIf confirmation timeout occurred (wake-up, go to sleep or schedule request confirmation timeout) */
#define LINSM_E_INIT_FAILED                       (0x60u) /*!< Error code: The service LinSM_Init() is called while the module is already initialized */
#define LINSM_E_GETTRCVMODE_FAILED                (0x61u) /*!< Error code: LinIf_GetTrcvMode() returned E_NOT_OK */
#define LINSM_E_SETTRCVMODE_FAILED                (0x62u) /*!< Error code: LinIf_SetTrcvMode() returned E_NOT_OK */
#define LINSM_E_NOT_IN_FULL_COM                   (0x63u) /*!< Error code: LinSM is not in expected state LINSM_FULL_COM */
#define LINSM_E_REQUEST_IN_PROCESS                (0x64u) /*!< Error code: Schedule switch requested when a communication request is still in process */
#define LINSM_E_MODE_ALREADY_ACTIVE               (0x65u) /*!< Error code: The requested mode is already active */
#define LINSM_E_FULLCOM_NOT_REQUESTED             (0x66u) /*!< Error code: The wake-up confirmation function is called without a preceding full communication request */
#define LINSM_E_NOCOM_NOT_REQUESTED               (0x67u) /*!< Error code: The go-to-sleep confirmation is called without a preceding no communication request */
#define LINSM_E_TIMEOUT_IN_NO_COM                 (0x68u) /*!< Error code: A confirmation timeout occurred in state LINSM_NO_COM when full communication is not requested */
#define LINSM_E_WAKEUPCONF_NO_SUCCESS             (0x69u) /*!< Error code: LinIf reported a negative wake-up confirmation (success == FALSE) */

/* ----- LinSM network sub-states ----- */
#define LINSM_STATE_NO_COM                        COMM_NO_COMMUNICATION
#define LINSM_STATE_FULL_COM                      COMM_FULL_COMMUNICATION


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define LINSM_START_SEC_CONST_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* LinSM version constants */
extern CONST(uint8, LINSM_CONST) LinSM_MainVersion;
extern CONST(uint8, LINSM_CONST) LinSM_SubVersion;
extern CONST(uint8, LINSM_CONST) LinSM_ReleaseVersion;

#define LINSM_STOP_SEC_CONST_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */



/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define LINSM_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Service Functions */
/**********************************************************************************************************************
 * LinSM_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Power-up memory initialization
 *  \details     Initializes the variables in *_INIT_* sections at power-up.
 *  \pre         Module is uninitialized
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-1094
 *  \note        Use this function in case these variables are not initialized by the startup code.
**********************************************************************************************************************/
extern FUNC(void, LINSM_CODE) LinSM_InitMemory( void );

/**********************************************************************************************************************
 * LinSM_Init()
 *********************************************************************************************************************/
/*! \brief       Initializes the LinSM
 *  \details     All variables are set to default values. The LinSM global state is set to initialized, the sub-state
 *               (communication state) is set to LINSM_NO_COM.
 *  \param[in]   ConfigPtr       Pointer to component configuration structure
 *  \pre         Interrupts are disabled
 *  \pre         Module is uninitialized
 *  \pre         LinSM_InitMemory has been called unless LinSM_Initialized is initialized by start-up code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-1094
 *  \trace       SPEC-10376, SPEC-10401, SPEC-10406, SPEC-10408, SPEC-10458, SPEC-10492, SPEC-17351, SPEC-17352,
 *               SPEC-17365, SPEC-17381, SPEC-17394
**********************************************************************************************************************/
extern FUNC(void, LINSM_CODE) LinSM_Init( P2CONST(LinSM_ConfigType, AUTOMATIC, LINSM_INIT_DATA) ConfigPtr );

/**********************************************************************************************************************
 * LinSM_MainFunction()
 *********************************************************************************************************************/
/*! \brief       Periodic main function
 *  \details     This function handles the confirmation timeout handling of communication and schedule requests
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \trace       SPEC-17431, SPEC-17434
**********************************************************************************************************************/
extern FUNC(void, LINSM_CODE) LinSM_MainFunction( void );

#if( LINSM_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 * LinSM_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     Returns the version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versionInfo     Pointer where the version information of this module is copied to.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINSM_VERSION_INFO_API == STD_ON
 *  \trace       CREQ-1096
 *  \trace       SPEC-10402, SPEC-10492, SPEC-17364, SPEC-17395
**********************************************************************************************************************/
extern FUNC(void, LINSM_CODE) LinSM_GetVersionInfo( P2VAR(Std_VersionInfoType, AUTOMATIC, LINSM_APPL_DATA) versionInfo );
#endif

/**********************************************************************************************************************
 * LinSM_RequestComMode
 *********************************************************************************************************************/
/*! \brief       Handles a communication mode request
 *  \details     This function is used by ComM to request a communication mode
 *  \param[in]   network         System channel handle
 *  \param[in]   mode            Requested communication mode. Only COMM_FULL_COMMUNICATION or COMM_NO_COMMUNICATION is
 *                               accepted
 *  \return      E_OK     - Request accepted
 *               E_NOT_OK - Request has not been accepted
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \trace       SPEC-10492, SPEC-17348, SPEC-17353, SPEC-17354, SPEC-17355, SPEC-17356, SPEC-17383, SPEC-17425, 
 *               SPEC-17426, SPEC-17427, SPEC-17428, SPEC-2618835, SPEC-2618836
**********************************************************************************************************************/
extern FUNC(Std_ReturnType, LINSM_CODE) LinSM_RequestComMode( VAR(NetworkHandleType, AUTOMATIC) network,
                                                              VAR(ComM_ModeType, AUTOMATIC) mode );

/**********************************************************************************************************************
 * LinSM_GetCurrentComMode
 *********************************************************************************************************************/
/*! \brief       Get the current communication mode
 *  \details     Function to query the current communication mode.
 *  \param[in]   network         System channel handle
 *  \param[out]  mode            Pointer where the communication mode information is copied to. Possible value are:
 *                               COMM_FULL_COMMUNICATION and COMM_NO_COMMUNICATION
 *  \return      E_OK     - No error
 *               E_NOT_OK - Getting the current mode has failed
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-1100
 *  \trace       SPEC-10425, SPEC-10492, SPEC-17362, SPEC-17374, SPEC-17382, SPEC-17412, SPEC-17432, SPEC-17439
**********************************************************************************************************************/
extern FUNC(Std_ReturnType, LINSM_CODE) LinSM_GetCurrentComMode(VAR(NetworkHandleType, AUTOMATIC) network, P2VAR(ComM_ModeType, AUTOMATIC, LINSM_APPL_DATA) mode );

/**********************************************************************************************************************
 * LinSM_ScheduleRequest()
 *********************************************************************************************************************/
/*! \brief       Handles a schedule request
 *  \details     The upper layer (BswM) requests a schedule table to be changed on the given LIN network.
 *  \param[in]   network         System channel handle
 *  \param[in]   schedule        Index of the new schedule table
 *  \return      E_OK     - Schedule table request has been accepted
 *               E_NOT_OK - Schedule table switch request has not been accepted
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \note        A schedule request is only accepted in state LINSM_FULL_COM.
 *  \trace       SPEC-10447, SPEC-10450, SPEC-10479, SPEC-10487, SPEC-10492, SPEC-17347, SPEC-17358, SPEC-17378, 
 *               SPEC-17384, SPEC-17423, SPEC-17427, SPEC-17429, SPEC-17430
**********************************************************************************************************************/
extern FUNC(Std_ReturnType, LINSM_CODE) LinSM_ScheduleRequest( VAR(NetworkHandleType, AUTOMATIC) network,
                                                               VAR(LinIf_SchHandleType, AUTOMATIC) schedule );


#define LINSM_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*!
 * \exclusivearea LINSM_EXCLUSIVE_AREA_0
 * Protects LinSM_ModeManager related variables from being modified while a communication mode request is processed.
 * \protects LinSM_ComState, LinSM_RequestedComMode, LinSM_ConfirmationTimer
 * \usedin LinSM_RequestComMode()
 * \exclude LinSM_MainFunction(), LinIf_MainFunction() and BswMLinScheduleSwitch action (call context depends on configuration)
 * \recommendation Must lock task interrupts, only if the priority level of LinIf_MainFunction(), LinSM_MainFunction() or BswM action BswMLinScheduleSwitch is higher than the level of ComM_MainFunction(). Otherwise no locking mechanism is required.
 * \length LONG This exclusive area calls to other BSW modules (LinIf, ComM and BswM).
 * \endexclusivearea
 *
 * \exclusivearea LINSM_EXCLUSIVE_AREA_1
 * Protects LinSM_ModeManager and LinSM_ScheduleManager related variables from being modified while a LinIf request confirmation is processed.
 * \protects LinSM_ComState, LinSM_RequestedComMode, LinSM_ConfirmationTimer, LinSM_WakeUpRetryCounter, LinSM_NegativeConfirmation
 * \usedin LinSM_WakeupConfirmation(), LinSM_GotoSleepConfirmation() and LinSM_ScheduleRequestConfirmation()
 * \exclude ComM_MainFunction(), LinSM_MainFunction() and BswMLinScheduleSwitch action (call context depends on configuration)
 * \recommendation Must lock task interrupts, only if the priority of ComM_MainFunction(), LinSM_MainFunction() or BswM action BswMLinScheduleSwitch is higher than the level of LinIf_MainFunction(). Otherwise no locking mechanism is required.
 * \length LONG This exclusive area calls to other BSW modules (ComM and BswM).
 * \endexclusivearea
 *
 * \exclusivearea LINSM_EXCLUSIVE_AREA_2
 * Protects LinSM_ModeManager and LinSM_ScheduleManager related variables from being modified while the LinSM_MainFunction is processed.
 * \protects LinSM_ComState, LinSM_RequestedComMode, LinSM_ConfirmationTimer, LinSM_WakeUpRetryCounter, LinSM_NegativeConfirmation
 * \usedin LinSM_MainFunction()
 * \exclude ComM_MainFunction(), LinIf_MainFunction() and BswMLinScheduleSwitch action (call context depends on configuration)
 * \recommendation Must lock task interrupts, only if the priority of ComM_MainFunction(), LinIf_MainFunction() or BswM action BswMLinScheduleSwitch is higher than the level of LinSM_MainFunction(). Otherwise no locking mechanism is required.
 * \length LONG This exclusive area calls to other BSW modules (LinIf, ComM and BswM).
 * \endexclusivearea
 *
 * \exclusivearea LINSM_EXCLUSIVE_AREA_3
 * Protects LinSM_ModeManager related variables from being modified while a schedule request is processed.
 * \protects LinSM_ComState, LinSM_RequestedComMode, LinSM_ConfirmationTimer
 * \usedin LinSM_ScheduleRequest()
 * \exclude ComM_MainFunction(), LinIf_MainFunction() and LinSM_MainFunction()
 * \recommendation Must lock task interrupts, if the priority of ComM_MainFunction(), LinIf_MainFunction() or LinSM_MainFunction() is higher than the level of the BswM action BswMLinScheduleSwitch (call context depends on configuration). Otherwise no locking mechanism is required.
 * \length LONG This exclusive area calls to other BSW modules (LinIf).
 * \endexclusivearea
 */

#endif  /* LINSM_H */

/**********************************************************************************************************************
 *  END OF FILE: LinSM.h
 *********************************************************************************************************************/
