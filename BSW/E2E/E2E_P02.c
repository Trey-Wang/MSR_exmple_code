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
 *         \file  E2E_P02.c
 *        \brief  E2E source file for AUTOSAR E2E Library Profile 2
 *
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults within
 *                the communication link. E2E Library provides mechanisms for E2E protection, adequate for safety-related
 *                communication having requirements up to ASIL D.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "E2E_P02.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

#define E2E_P02_MAX_COUNTER_VALUE 15u

#if !defined (E2E_LOCAL) /* COV_E2E_COMPATIBILITY */
# define E2E_LOCAL static
#endif

#if !defined (E2E_LOCAL_INLINE) /* COV_E2E_COMPATIBILITY */
# define E2E_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define E2E_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  E2E_P02ProtectVerifyInputs()
 *********************************************************************************************************************/
/*!
 * \brief      Checks the input data of E2E_P02Protect
 * \details    Checks if input pointer parameters are valid and if counter value is within range
 * \param[in]  ConfigPtr  Pointer to the config data structure
 * \param[in]  StatePtr   Pointer to the state data structure
 * \param[in]  DataPtr    Pointer to the data to be protected
 * \return     Standard return value, indicating valid/invalid input
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (Std_ReturnType, E2E_CODE) E2E_P02ProtectVerifyInputs(P2CONST (E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                                     P2CONST (E2E_P02ProtectStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                                                     P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr);

/**********************************************************************************************************************
 *  E2E_P02CheckVerifyInputs()
 *********************************************************************************************************************/
/*!
 * \brief      Checks the input data of E2E_P02Check
 * \details    Checks if input pointer parameters are valid and if counter values are within range
 * \param[in]  ConfigPtr  Pointer to the config data structure
 * \param[in]  StatePtr   Pointer to the state data structure
 * \param[in]  DataPtr    Pointer to the data to be protected
 * \return     Standard return value, indicating valid/invalid input
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (Std_ReturnType, E2E_CODE) E2E_P02CheckVerifyInputs(P2CONST (E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                                   P2CONST (E2E_P02CheckStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                                                   P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr);

/**********************************************************************************************************************
 *  E2E_P02ComputeCRC()
 *********************************************************************************************************************/
/*!
 * \brief      Computes the CRC over the given data, including the DataID
 * \details    Computes the CRC over the given data, including the DataID
 * \param[in]  DataPtr Pointer to the data to be protected
 * \param[in]  Length  Length of the data in bytes
 * \param[in]  Offset  Offset of the E2E header in bytes
 * \param[in]  LocalDataID  The DataID corresponding to the data
 * \return     uint8   The computed CRC
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (uint8, E2E_CODE) E2E_P02ComputeCRC(P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                   VAR (uint16, AUTOMATIC ) Length,
                                                   VAR (uint16, AUTOMATIC ) Offset,
                                                   VAR (uint8, AUTOMATIC ) LocalDataID);

/**********************************************************************************************************************
 *  E2E_CheckDeltaCounter()
 *********************************************************************************************************************/
/*!
 * \brief      Applies further checks regarding delta counter
 * \details    Applies further checks regarding delta counter
 * \param[in]  ConfigPtr        Pointer to the config data structure
 * \param[in]  StatePtr         Pointer to the state data structure
 * \param[in]  ReceivedCounter  Received counter value
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_CheckDeltaCounter(P2CONST (E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                      P2VAR (E2E_P02CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
                                                      VAR (uint8, AUTOMATIC) ReceivedCounter);

/**********************************************************************************************************************
 *  E2E_CheckSync()
 *********************************************************************************************************************/
/*!
 * \brief          Applies further checks regarding sync, may change E2E status
 * \details        Applies further checks regarding sync, may change E2E status
 * \param[in]      ConfigPtr  Pointer to the config data structure
 * \param[in,out]  StatePtr   Pointer to the state data structure
 * \pre            -
 * \context        TASK
 * \reentrant      FALSE
 * \synchronous    TRUE
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_CheckSync(P2CONST (E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                              P2VAR (E2E_P02CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);
    
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P02ProtectVerifyInputs()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
E2E_LOCAL FUNC (Std_ReturnType, E2E_CODE) E2E_P02ProtectVerifyInputs(P2CONST (E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                                     P2CONST (E2E_P02ProtectStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                                                     P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;
    
    /* #10 Check if input pointer parameters are not NULL */
    if ( (ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR) )
    {
        /* #20 Check if Counter is within valid range */
        if (StatePtr->Counter > E2E_P02_MAX_COUNTER_VALUE)
        {
            ret = E2E_E_INPUTERR_WRONG;
        }
        else
        {
            ret = E2E_E_OK;
        }
    }
    return ret;
}

/**********************************************************************************************************************
 *  E2E_P02CheckVerifyInputs()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2E_LOCAL FUNC (Std_ReturnType, E2E_CODE) E2E_P02CheckVerifyInputs(P2CONST (E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                                   P2CONST (E2E_P02CheckStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                                                   P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* #10 Check if input pointer parameters are valid and if state parameter are within range */
    if ( (ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR) && (DataPtr != NULL_PTR) )
    {
        if (  (StatePtr->LastValidCounter > E2E_P02_MAX_COUNTER_VALUE)
           || (StatePtr->MaxDeltaCounter > E2E_P02_MAX_COUNTER_VALUE)
           || (StatePtr->SyncCounter > ConfigPtr->SyncCounterInit)
           || (StatePtr->NoNewOrRepeatedDataCounter > E2E_P02_MAX_COUNTER_VALUE)
           )
        {
            ret = E2E_E_INPUTERR_WRONG;
        }
        else
        {
            ret = E2E_E_OK;
        }
    }
    return ret;
}
 
/**********************************************************************************************************************
 *  E2E_P02ComputeCRC()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
E2E_LOCAL FUNC (uint8, E2E_CODE) E2E_P02ComputeCRC(P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                   VAR (uint16, AUTOMATIC ) Length,
                                                   VAR (uint16, AUTOMATIC ) Offset,
                                                   VAR (uint8, AUTOMATIC ) LocalDataID)
{
    uint8 crc;

    /* #10 Check if CRC is not positioned in first byte of DataPtr */
    if (Offset > 0u)
    {
        /* #12 Calculate CRC over Data bytes before CRC position */
        crc = Crc_CalculateCRC8H2F ( DataPtr        /* SBSW_E2E_03 */
                                   , Offset
                                   , 0xffu
                                   , TRUE
                                   );
        /* #14 Calculate CRC over Data bytes after CRC position */
        crc = Crc_CalculateCRC8H2F ( &DataPtr[Offset + 1u]      /* SBSW_E2E_03 */
                                   , Length - Offset - 1u
                                   , crc
                                   , FALSE
                                   );
    }
    else
    {
        /* #16 CRC is positioned in first byte of DataPtr. Calculate CRC over all residual bytes */
        crc = Crc_CalculateCRC8H2F ( &DataPtr[1u]       /* SBSW_E2E_03 */
                                   , Length - 1u
                                   , 0xffu
                                   , TRUE
                                   );
    }
    /* #20 Append CRC over DataID at the end */
    crc = Crc_CalculateCRC8H2F (&LocalDataID, 1u, crc, FALSE); /* SBSW_E2E_02 */

    return crc;
}

/**********************************************************************************************************************
 *  E2E_CheckDeltaCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_CheckDeltaCounter(P2CONST (E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                      P2VAR (E2E_P02CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
                                                      VAR (uint8, AUTOMATIC) ReceivedCounter)
{
    /* #10 Calculate delta between received and last valid counter. Overflow is also considered */
    uint8 DeltaCounter = (uint8)(ReceivedCounter - StatePtr->LastValidCounter) & 0x0fu;
    
    /* #20 Check if counter didn't change, thus no new data was received*/
    if (DeltaCounter == 0u)
    {
        if (StatePtr->NoNewOrRepeatedDataCounter < E2E_P02_MAX_COUNTER_VALUE)
        {
            StatePtr->NoNewOrRepeatedDataCounter++; /* SBSW_E2E_03 */
        }
        StatePtr->Status = E2E_P02STATUS_REPEATED; /* SBSW_E2E_03 */
    }
    else
    {
        /* #30 If DeltaCounter equals 1, the correct data was received. No message was lost in between */
        if (DeltaCounter == 1u)
        {
            StatePtr->MaxDeltaCounter = ConfigPtr->MaxDeltaCounterInit; /* SBSW_E2E_03 */
            StatePtr->LastValidCounter = ReceivedCounter; /* SBSW_E2E_03 */
            StatePtr->LostData = 0u; /* SBSW_E2E_03 */
            StatePtr->Status = E2E_P02STATUS_OK; /* SBSW_E2E_03 */
            E2E_CheckSync (ConfigPtr, StatePtr); /* SBSW_E2E_03 */
        }
        else
        {
            /* #40 Received data is not subsequent data, but counter is still tolerable */
            if (DeltaCounter <= StatePtr->MaxDeltaCounter)
            {
                StatePtr->MaxDeltaCounter = ConfigPtr->MaxDeltaCounterInit; /* SBSW_E2E_03 */
                StatePtr->LastValidCounter = ReceivedCounter; /* SBSW_E2E_03 */
                StatePtr->LostData = DeltaCounter - 1u; /* SBSW_E2E_03 */
                StatePtr->Status = E2E_P02STATUS_OKSOMELOST; /* SBSW_E2E_03 */
                E2E_CheckSync (ConfigPtr, StatePtr); /* SBSW_E2E_03 */
            }
            /* #50 Counter of received data is out of range */
            else
            {
                StatePtr->NoNewOrRepeatedDataCounter = 0u; /* SBSW_E2E_03 */
                StatePtr->SyncCounter = ConfigPtr->SyncCounterInit; /* SBSW_E2E_03 */
                if (StatePtr->SyncCounter > 0u)
                {
                    StatePtr->MaxDeltaCounter = ConfigPtr->MaxDeltaCounterInit; /* SBSW_E2E_03 */
                    StatePtr->LastValidCounter = ReceivedCounter; /* SBSW_E2E_03 */
                }
                StatePtr->Status = E2E_P02STATUS_WRONGSEQUENCE; /* SBSW_E2E_03 */
            }
        }
    }
}

/**********************************************************************************************************************
 *  E2E_CheckSync()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_CheckSync(P2CONST (E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                              P2VAR (E2E_P02CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr)
{
    /* #10 Check if NoNewOrRepeatedDataCounter didn't reach its limit */
    if (StatePtr->NoNewOrRepeatedDataCounter <= ConfigPtr->MaxNoNewOrRepeatedData)
    {
        StatePtr->NoNewOrRepeatedDataCounter = 0u; /* SBSW_E2E_03 */

        /* #20 Status is STATUS_SYNC in case SyncCounter is greater than 0 */
        if (StatePtr->SyncCounter > 0u)
        {
            StatePtr->SyncCounter--; /* SBSW_E2E_03 */
            StatePtr->Status = E2E_P02STATUS_SYNC; /* SBSW_E2E_03 */
        }
    }
    else
    {
        StatePtr->NoNewOrRepeatedDataCounter = 0u; /* SBSW_E2E_03 */
        StatePtr->SyncCounter = ConfigPtr->SyncCounterInit; /* SBSW_E2E_03 */
        StatePtr->Status = E2E_P02STATUS_SYNC; /* SBSW_E2E_03 */
    }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P02Protect()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P02Protect (P2CONST (E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                P2VAR (E2E_P02ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
                                                P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr)
{
    /* #10 Check input parameter */
    Std_ReturnType ret = E2E_P02ProtectVerifyInputs ( ConfigPtr, StatePtr, DataPtr); /* SBSW_E2E_03 */

    if (ret == E2E_E_OK)
    {
        uint8 ComputedCRC;
        uint16 CounterOffset;
        uint16 CRCOffset;
        /* #20 Increment Counter: Valid values are 0..15 */
        StatePtr->Counter = (StatePtr->Counter + 1u) & 0x0fu; /* SBSW_E2E_03 */
        CRCOffset = ConfigPtr->Offset >> 3u;
        /* #25 Counter is always written to subsequent byte of CRC in DataPtr */
        CounterOffset = CRCOffset + 1u;
        /* #30 Counter is written to low nibble in DataPtr, where CounterOffset is pointing to */
        DataPtr[CounterOffset] = (DataPtr[CounterOffset] & 0xf0u)   /* SBSW_E2E_03 */
                               | StatePtr->Counter;

        /* #40 Compute CRC */
        ComputedCRC = E2E_P02ComputeCRC ( DataPtr   /* SBSW_E2E_03 */
                                        , ConfigPtr->DataLength >> 3u
                                        , CRCOffset
                                        , ConfigPtr->DataIDList [StatePtr->Counter]
                                        );
        /* #50 Write computed CRC to position in DataPtr, CRCOffset is pointing to */
        DataPtr[CRCOffset] = ComputedCRC; /* SBSW_E2E_03 */
    }
    return ret;
}
 
/**********************************************************************************************************************
 *  E2E_P02ProtectInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P02ProtectInit (P2VAR (E2E_P02ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* #10 Initialize Counter in case StatePtr is valid */
    if (StatePtr != NULL_PTR)
    {
        StatePtr->Counter = 0u; /* SBSW_E2E_03 */
        ret = E2E_E_OK;
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P02Check()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
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
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P02Check (P2CONST (E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                              P2VAR (E2E_P02CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
                                              P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr)
{
    /* #10 Check input parameter */
    Std_ReturnType ret = E2E_P02CheckVerifyInputs ( ConfigPtr, StatePtr, DataPtr); /* SBSW_E2E_03 */

    if (ret == E2E_E_OK)
    {
        /* #20 Maximum allowed delta between previous and current counter is incremented every function invocation */
        if (StatePtr->MaxDeltaCounter < E2E_P02_MAX_COUNTER_VALUE)
        {
            StatePtr->MaxDeltaCounter++; /* SBSW_E2E_03 */
        }
        /* #30 Check if new data is available */
        if (StatePtr->NewDataAvailable == TRUE)
        {
            uint16 ComputedOffset  = ConfigPtr->Offset >> 3u;
            /* #40 CRC is read from received data */
            uint8 ReceivedCRC = DataPtr[ComputedOffset];
            /* #41 Counter is read from received data */
            uint8 ReceivedCounter = DataPtr[ComputedOffset + 1u] & ((uint8)0x0fu);
            /* #42 DataID is determined from counter value in received data */
            uint8 DataID = ConfigPtr->DataIDList[ReceivedCounter];
            /* #45 Calculate CRC in the same way as the Sender did */
            uint8 ComputedCRC = E2E_P02ComputeCRC ( DataPtr       /* SBSW_E2E_03 */
                                            , ConfigPtr->DataLength >> 3u
                                            , ComputedOffset
                                            , DataID
                                            );

            /* #50 Check if received CRC matches the computed one */
            if (ReceivedCRC == ComputedCRC)
            {
                /* #60 Check if any correct data has already been received */
                if (StatePtr->WaitForFirstData == FALSE)
                {
                    /* #65 Evaluate counter value of subsequent data */
                    E2E_CheckDeltaCounter (ConfigPtr, StatePtr, ReceivedCounter); /* SBSW_E2E_03 */
                }
                else
                {
                    StatePtr->WaitForFirstData = FALSE; /* SBSW_E2E_03 */
                    StatePtr->MaxDeltaCounter = ConfigPtr->MaxDeltaCounterInit; /* SBSW_E2E_03 */
                    StatePtr->LastValidCounter = ReceivedCounter; /* SBSW_E2E_03 */
                    StatePtr->Status = E2E_P02STATUS_INITIAL; /* SBSW_E2E_03 */
                }
            }
            else
            {
                StatePtr->Status = E2E_P02STATUS_WRONGCRC; /* SBSW_E2E_03 */
            }
        }
        else
        {
            if (StatePtr->NoNewOrRepeatedDataCounter < E2E_P02_MAX_COUNTER_VALUE)
            {
                StatePtr->NoNewOrRepeatedDataCounter++; /* SBSW_E2E_03 */
            }
            StatePtr->Status = E2E_P02STATUS_NONEWDATA; /* SBSW_E2E_03 */
        }
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P02CheckInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P02CheckInit (P2VAR (E2E_P02CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* #10 Reset State structure in case StatePtr is valid */
    if (StatePtr != NULL_PTR)
    {
        StatePtr->LastValidCounter           = 0u; /* SBSW_E2E_03 */
        StatePtr->MaxDeltaCounter            = 0u; /* SBSW_E2E_03 */
        StatePtr->WaitForFirstData           = TRUE; /* SBSW_E2E_03 */
        StatePtr->NewDataAvailable           = FALSE; /* SBSW_E2E_03 */
        StatePtr->LostData                   = 0u; /* SBSW_E2E_03 */
        StatePtr->Status                     = E2E_P02STATUS_NONEWDATA; /* SBSW_E2E_03 */
        StatePtr->NoNewOrRepeatedDataCounter = 0u; /* SBSW_E2E_03 */
        StatePtr->SyncCounter                = 0u; /* SBSW_E2E_03 */
        ret = E2E_E_OK;
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P02MapStatusToSM()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC (E2E_PCheckStatusType, E2E_CODE) E2E_P02MapStatusToSM (VAR (Std_ReturnType, AUTOMATIC) CheckReturn,
                                                            VAR (E2E_P02CheckStatusType, AUTOMATIC) Status,
                                                            VAR (boolean, AUTOMATIC) profileBehavior)
{
    E2E_PCheckStatusType ret = E2E_P_ERROR;

    /* #10 Status is only mapped to state machine if check-function returned successfully */
    if (CheckReturn == E2E_E_OK)
    {
      /* #20 SYNC and INITIAL status are set according to profileBehavior. Either the legacy behavior (before R4.2) or new behavior is chosen */
        E2E_PCheckStatusType sync_ret = E2E_P_OK;
        E2E_PCheckStatusType init_ret = E2E_P_WRONGSEQUENCE;
        if (profileBehavior == FALSE)
        {
            sync_ret = E2E_P_WRONGSEQUENCE;        
            init_ret = E2E_P_OK;
        }
    
        /* #30 Status of Profile 2 is mapped to state machine status, which is not that fine-grained */
        switch (Status)
        {
            case E2E_P02STATUS_OK           : ret = E2E_P_OK;            break;
            case E2E_P02STATUS_NONEWDATA    : ret = E2E_P_REPEATED;      break;
            case E2E_P02STATUS_WRONGCRC     : ret = E2E_P_ERROR;         break;
            case E2E_P02STATUS_SYNC         : ret = sync_ret;            break;
            case E2E_P02STATUS_INITIAL      : ret = init_ret;            break;
            case E2E_P02STATUS_REPEATED     : ret = E2E_P_REPEATED;      break;
            case E2E_P02STATUS_OKSOMELOST   : ret = E2E_P_OK;            break;
            case E2E_P02STATUS_WRONGSEQUENCE: ret = E2E_P_WRONGSEQUENCE; break;
            default: break; /* PRQA S 2018 */ /* MD_EtoE_14.1 */
        }
    }

    return ret;    
} /* PRQA S 6030 */ /* MD_MSR_STCYC */

#define E2E_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: E2E_P02.c
 *********************************************************************************************************************/
