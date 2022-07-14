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
/*! \file     CddDes.h
 *  \brief    CddDes header file
 *
 *  \details  Diagnostic event synchronizer main header file
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Alexander Ditte               visade        Vector Informatik GmbH
 *  Sebastian Kobbe               vissko        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2017-01-23  visade   -             
 *                        vissko   -             initial version
 *********************************************************************************************************************/

#ifndef CDDDES_H
# define CDDDES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* Communication types */
#include "ComStack_Types.h"

/* PduR Transmit Service */
# include "PduR_CddDes.h"

/* Configuration */
# include "CddDes_Cfg.h"


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define CDDDES_VENDOR_ID                                       (30u)
# define CDDDES_MODULE_ID                                       (255u)

/* AUTOSAR Software specification version information */
# define CDDDES_AR_RELEASE_MAJOR_VERSION                        (4u)
# define CDDDES_AR_RELEASE_MINOR_VERSION                        (0u)
# define CDDDES_AR_RELEASE_REVISION_VERSION                     (3u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define CDDDES_SW_MAJOR_VERSION                                (1u)
# define CDDDES_SW_MINOR_VERSION                                (0u)
# define CDDDES_SW_PATCH_VERSION                                (0u)

# define CDDDES_INSTANCE_ID_DET                                 (0x00u)

/* ----- Protocol version information ----- */
# define CDDDES_PROTOCOL_VERSION                                (1u)

/* ----- API service IDs ----- */
# define CDDDES_SID_INIT                             (0x00U) /*!< Service ID: ::CddDes_Init()                      */
# define CDDDES_SID_GET_VERSION_INFO                 (0x01U) /*!< Service ID: ::CddDes_GetVersionInfo()            */
# define CDDDES_SID_MEMORY_INIT                      (0x02U) /*!< Service ID: ::CddDes_InitMemory()                */
# define CDDDES_SID_MAINFUNCTION                     (0x03U) /*!< Service ID: ::CddDes_MainFunction()              */

# define CDDDES_SID_COPY_TX_DATA                     (0x04U) /*!< Service ID: ::CddDes_CopyTxData()                */
# define CDDDES_SID_TX_CONFIRMATION                  (0x05U) /*!< Service ID: ::CddDes_TxConfirmation()            */
# define CDDDES_SID_START_OF_RECEPTION               (0x06U) /*!< Service ID: ::CddDes_StartOfReception()          */
# define CDDDES_SID_COPY_RX_DATA                     (0x07U) /*!< Service ID: ::CddDes_CopyRxData()                */
# define CDDDES_SID_RX_INDICATION                    (0x08U) /*!< Service ID: ::CddDes_RxIndication()              */

# define CDDDES_SID_DTC_STATUS_CHANGED               (0x09U) /*!< Service ID: ::CddDes_DtcStatusChanged()          */
# define CDDDES_SID_DTC_CLEAR_NOTIFICATION           (0x0AU) /*!< Service ID: ::CddDes_CddDes_ClearDtcNotification() */

# define CDDDES_SID_INTERNAL                         (0xFFU) /*!< Service ID: Internal Functions                   */

/* ----- Error codes ----------------------------------------------------------------------------------------------- */
# define CDDDES_E_NO_ERROR                          (0x00U) /*!< used to check if no error occurred - use a value unequal to any error code */
# define CDDDES_E_PARAM_POINTER                     (0x10U) /*!< Error code: API service used with invalid pointer parameter (\c NULL_PTR) */
# define CDDDES_E_PARAM_VALUE                       (0x11U) /*!< Error code: API service used with invalid parameter value */
# define CDDDES_E_PARAM_CONFIG                      (0x12U) /*!< Error code: API service used with invalid or inconsistent configuration */
# define CDDDES_E_UNINIT                            (0x20U) /*!< Error code: API service used without module initialization */
# define CDDDES_E_ALREADY_INITIALIZED               (0x21U) /*!< Error code: The service CddDes_Init() is called while the module is already initialized  */
# define CDDDES_E_CHANNEL_BUFFER_OUT_OF_RANGE       (0x30U) /*!< Error code: API service used with invalid buffer length */
# define CDDDES_E_PDU_ID_RX_OUT_OF_RANGE            (0x31U) /*!< Error code: API service used with Rx-PDU-Id that is not available */
# define CDDDES_E_PDU_ID_TX_OUT_OF_RANGE            (0x32U) /*!< Error code: API service used with Tx-PDU-Id that is not available */
# define CDDDES_E_ILLEGAL_STATE                     (0x40U) /*!< Error code: API service used in invalid state */
# define CDDDES_E_COLLECTION_BUFFER_OVERFLOW        (0x50U) /*!< Error code: There was an overflow in the Status collection buffer and at least one event could not be written */
# define CDDDES_E_RECEIVED_MESSAGE_INVALID          (0x60U) /*!< Error code: The received message could not be processed because it did not contain the expected payload (insufficient payload or unknown event type) */

/* ----- Modes ----------------------------------------------------------------------------------------------------- */
# define CDDDES_UNINIT                               (0x00u) /*!< Signifies module is not initialized. */
# define CDDDES_INIT                                 (0x01u) /*!< Signifies module is initialized. */

/* ----- PduR States ----------------------------------------------------------------------------------------------- */
# define CDDDES_PDUR_IDLE                            (0x00u)  /*!< No reception/transmission in progress */
# define CDDDES_PDUR_RX_COPY                         (0x01u)  /*!< Reception in progress */
# define CDDDES_PDUR_TX_WAIT                         (0x02u)  /*!< Wait for start of a transmission */
# define CDDDES_PDUR_TX_COPY                         (0x03u)  /*!< Transmission in progress */

/* ----- PduR Handles ---------------------------------------------------------------------------------------------- */
# define CDDDES_INVALID_PDU_ID                       (0xFFu) /*!< Identifier for an invalid Pdu handle */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# define CddDes_EnterExclusiveArea_CollectionBuffer() SchM_Enter_CddDes_CDDDES_EXCLUSIVE_AREA_0()    /* PRQA S 3453,3335 */ /* MD_MSR_19.7 */
# define CddDes_LeaveExclusiveArea_CollectionBuffer() SchM_Exit_CddDes_CDDDES_EXCLUSIVE_AREA_0()     /* PRQA S 3453,3335 */ /* MD_MSR_19.7 */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define CDDDES_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  CddDes_InitMemory()
 *********************************************************************************************************************/
/*! \brief        Function for *_INIT_*-variable initialization
 *  \details      Service to initialize module global variables at power up. This function initializes the
 *                variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \pre          Module is uninitialized.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22475
 *********************************************************************************************************************/
FUNC(void, CDDDES_CODE) CddDes_InitMemory(
  void
  );

/**********************************************************************************************************************
 *  CddDes_Init()
 *********************************************************************************************************************/
/*! \brief        Initialization function
 *  \details      This function initializes the module CDDDES. It initializes all variables and sets the module state to
 *                initialized.
 *  \param[in]    ConfigPtr   Configuration structure for initializing the module. Not used and must be NULL_PTR.
 *  \pre          Interrupts are disabled.
 *  \pre          Module is uninitialized.
 *  \pre          CddDes_InitMemory has been called unless CddDes_Init_State is initialized by start-up code.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \note         Specification of module initialization
 *  \trace        DSGN-CddDes22475
 *********************************************************************************************************************/
FUNC(void, CDDDES_CODE) CddDes_Init(
  P2CONST(CddDes_ConfigType, AUTOMATIC, CDDDES_PBCFG) ConfigPtr
  );

/**********************************************************************************************************************
 *  CddDes_MainFunction()
 *********************************************************************************************************************/
/*! \brief        Cyclic CddDes task
 *  \details      Executes all internal used timers and state machines
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22475
 *********************************************************************************************************************/
FUNC(void, CDDDES_CODE) CddDes_MainFunction(
  void
  );

# if (CDDDES_SUPPORT_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  CddDes_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief        Returns the version information
 *  \details      CddDes_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]   VersionInfo   Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        DSGN-CddDes22475
 *********************************************************************************************************************/
FUNC(void, CDDDES_CODE) CddDes_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CDDDES_APPL_VAR) VersionInfo
  );
# endif

# define CDDDES_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  EXCLUSIVE AREA DEFINITION
 **********************************************************************************************************************/
/*!
 * \exclusivearea CDDDES_EXCLUSIVE_AREA_0
 * Ensures that no two write-transactions (triggered by external interfaces) may happen at the same time
 * and that the WriteIndex is only updated after completing a write-transaction.
 * \protects CddDes_CollectionBuffer.WriteIndex, CddDes_CollectionBuffer.ReadIndex
 * \usedin
 *   - CddDes_Core_Buffer_GetReadAvailable()
 *   - CddDes_Core_Buffer_IncrementReadIndex()
 *   - CddDes_Core_Buffer_WriteEvent_*()
 * \exclude All other functions provided by CddDes.
 * \length SHORT The status change event is copied to the buffer and write index is incremented.
 * \endexclusivearea
 */

#endif /* CDDDES_H */

/**********************************************************************************************************************
 *  END OF FILE: CddDes.h
 *********************************************************************************************************************/