/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  E2E_P02.h
 *        \brief  E2E header file for AUTOSAR E2E Library Profile 2
 *
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults within
 *                the communication link. E2E Library provides mechanisms for E2E protection, adequate for safety-related
 *                communication having requirements up to ASIL D.
 *
 *********************************************************************************************************************/

/**** Protection against multiple inclusion **************************************************************************/
#if !defined (E2E_P02_H)
# define E2E_P02_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "E2E.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef struct {
    uint16 DataLength; /*!< Length of Data, in bits */
    uint8  DataIDList[16]; /*!< Array of appropriately chosen Data IDs for protection against masquerading */
    uint8  MaxDeltaCounterInit; /*!< Initial maximum allowed gap between two counter values of two consecutively received valid Data */
    uint8  MaxNoNewOrRepeatedData; /*!< Maximum amount of missing or repeated Data */
    uint8  SyncCounterInit;/*!< Number of Data required for validating the consistency of the counter
                                            after detection of an unexpected behavior of a received counter */
    uint16 Offset; /*!< Offset of the E2E header in the Data[] array, in bits */
} E2E_P02ConfigType;

typedef struct {
    uint8  Counter; /*!< Counter to be used for protecting the next Data */
} E2E_P02ProtectStateType;

typedef enum {
    E2E_P02STATUS_OK            = 0x00u,
    E2E_P02STATUS_NONEWDATA     = 0x01u,
    E2E_P02STATUS_WRONGCRC      = 0x02u,
    E2E_P02STATUS_SYNC          = 0x03u,
    E2E_P02STATUS_INITIAL       = 0x04u, 
    E2E_P02STATUS_REPEATED      = 0x08u,
    E2E_P02STATUS_OKSOMELOST    = 0x20u,
    E2E_P02STATUS_WRONGSEQUENCE = 0x40u
} E2E_P02CheckStatusType;

typedef struct {
    uint8 LastValidCounter; /*!< Counter value most recently received */
    uint8 MaxDeltaCounter; /*!< Maximum allowed difference between two counter values of consecutively received valid messages */
    boolean WaitForFirstData; /*!< If true means that no correct Data has been received yet */
    boolean NewDataAvailable; /*!< Indicates that new Data is available for E2E to be checked */
    uint8 LostData; /*!< Number of Data lost since reception of last valid one */
    E2E_P02CheckStatusType Status; /*!< Result of the verification of the Data */
    uint8 SyncCounter; /*!< Number of Data required for validating the consistency of the counter after detection of an unexpected behavior of a received counter*/
    uint8 NoNewOrRepeatedDataCounter; /*!< Amount of consecutive reception cycles in which there was either no or repeated Data */
} E2E_P02CheckStateType;


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define E2E_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  E2E_P02Protect()
 *********************************************************************************************************************/
/*!
 * \brief           Protection function of E2Elib Profile 2
 * \details         Protects the array/buffer to be transmitted using the E2E Profile 2. This includes checksum calculation,
 *                  handling of counter and Data ID.
 * \param[in]       ConfigPtr Pointer to profile configuration
 * \param[in,out]   StatePtr  Pointer to communication state
 * \param[in,out]   DataPtr   Pointer to data to be protected
 * \return          E2E_E_INPUTERR_NULL: A pointer parameter is NULL
 * \return          E2E_E_INPUTERR_WRONG: One parameter is erroneous
 * \return          E2E_E_INTERR: Internal library error
 * \return          E2E_E_OK: Protection successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1086
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P02Protect (P2CONST (E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                P2VAR (E2E_P02ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
                                                P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr);


/**********************************************************************************************************************
 *  E2E_P02ProtectInit()
 *********************************************************************************************************************/
/*!
 * \brief           Protection initialization function of E2Elib Profile 2
 * \details         Initializes the protection state by resetting the counter
 * \param[in,out]   StatePtr  Pointer to communication state
 * \return          E2E_E_INPUTERR_NULL: Pointer parameter is NULL
 * \return          E2E_E_OK: Initialization successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1087
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P02ProtectInit (P2VAR (E2E_P02ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);


/**********************************************************************************************************************
 *  E2E_P02Check()
 *********************************************************************************************************************/
/*!
 * \brief           Check function of E2Elib Profile 2
 * \details         Checks the Data received using the E2E Profile 2. This includes CRC calculation, handling of
 *                  the counter and Data ID.
 * \param[in]       ConfigPtr Pointer to profile configuration
 * \param[in,out]   StatePtr  Pointer to communication state
 * \param[in,out]   DataPtr   Pointer to data to be protected
 * \return          E2E_E_INPUTERR_NULL: A pointer parameter is NULL
 * \return          E2E_E_INPUTERR_WRONG: One parameter is erroneous
 * \return          E2E_E_INTERR: Internal library error
 * \return          E2E_E_OK: Protection successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1088
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P02Check (P2CONST (E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                              P2VAR (E2E_P02CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
                                              P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr);


/**********************************************************************************************************************
 *  E2E_P02CheckInit()
 *********************************************************************************************************************/
/*!
 * \brief           Check initialization function of E2Elib Profile 2
 * \details         Check initialization function of E2Elib Profile 2
 * \param[in,out]   StatePtr  Pointer to communication state
 * \return          E2E_E_INPUTERR_NULL: Pointer parameter is NULL
 * \return          E2E_E_OK: Initialization successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1089
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P02CheckInit (P2VAR (E2E_P02CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);


/**********************************************************************************************************************
 *  E2E_P02MapStatusToSM()
 *********************************************************************************************************************/
/*!
 * \brief           State mapping function of E2Elib Profile 2
 * \details         Maps the check status of Profile 2 to a generic check status, which can be used by E2E state machine
 *                  check function. The E2E Profile 2 delivers a more fine-grained status, but this is not relevant for
 *                  the E2E state machine.
 * \param[in]       CheckReturn     Return value of E2E_P02Check
 * \param[in]       Status          communication state
 * \param[in]       profileBehavior FALSE: check has the legacy behavior, before R4.2
 *                                  TRUE:  check behaves like new P4/P5/P6 profiles introduced in R4.2
 * \return          Standard state value to be used in E2Elib state machine
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1175
 * \trace           SPEC-2056622
 */
FUNC (E2E_PCheckStatusType, E2E_CODE) E2E_P02MapStatusToSM (VAR (Std_ReturnType, AUTOMATIC) CheckReturn,
                                                            VAR (E2E_P02CheckStatusType, AUTOMATIC) Status,
                                                            VAR (boolean, AUTOMATIC) profileBehavior);

# define E2E_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* E2E_P02_H */

/**********************************************************************************************************************
 *  END OF FILE: E2E_P02.h
 *********************************************************************************************************************/
