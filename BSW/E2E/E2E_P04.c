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
 *         \file  E2E_P04.c
 *        \brief  E2E source file for AUTOSAR E2E Library Profile 4
 *
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults within
 *                the communication link. E2E Library provides mechanisms for E2E protection, adequate for safety-related
 *                communication having requirements up to ASIL D.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "E2E_P04.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

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
 *  E2E_P04ProtectVerifyInputs()
 *********************************************************************************************************************/
/*!
 * \brief      Checks the input data of E2E_P04Protect
 * \details    All relevant input parameters are checked in order to proceed with E2E_P04Protect service
 * \param[in]  ConfigPtr  Pointer to the config data structure
 * \param[in]  StatePtr   Pointer to the state data structure
 * \param[in]  DataPtr    Pointer to the data to be protected
 * \param[in]  Length     length of the data in bytes
 * \return     Standard return value, indicating valid/invalid input
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (Std_ReturnType, E2E_CODE) E2E_P04ProtectVerifyInputs(P2CONST (E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                                     P2CONST (E2E_P04ProtectStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                                                     P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
                                                                     VAR (uint16, AUTOMATIC ) Length);

/**********************************************************************************************************************
 *  E2E_P04CheckVerifyInputs()
 *********************************************************************************************************************/
/*!
 * \brief         Checks the input data of E2E_P04Check
 * \details       All relevant input parameters are checked in order to proceed with E2E_P04Check service
 * \param[in]     ConfigPtr            Pointer to the config data structure
 * \param[in]     StatePtr             Pointer to the state data structure
 * \param[in]     DataPtr              Pointer to the data to be protected
 * \param[in]     Length               Length of the data in bytes
 * \param[in,out] NewDataAvailablePtr  Pointer to variable where new data available indication shall be stored
 * \return        Standard return value, indicating valid/invalid input
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
E2E_LOCAL FUNC (Std_ReturnType, E2E_CODE) E2E_P04CheckVerifyInputs(P2CONST (E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                                   P2CONST (E2E_P04CheckStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                                                   P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
                                                                   VAR (uint16, AUTOMATIC ) Length,
                                                                   P2VAR (boolean, AUTOMATIC, E2E_APPL_DATA ) NewDataAvailablePtr);

/**********************************************************************************************************************
 *  E2E_P04ProtectWriteLength()
 *********************************************************************************************************************/
/*!
 * \brief      Write Length of message to the E2E Header
 * \details    Write Length of message to the E2E Header in big endian format
 * \param[in]  DataPtr  Pointer to the data to be protected
 * \param[in]  Length   Length of the data in bytes
 * \param[in]  Offset   Offset of the E2E header in DataPtr in bytes
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_P04ProtectWriteLength(P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
                                                          VAR (uint16, AUTOMATIC ) Length,
                                                          VAR (uint16, AUTOMATIC ) Offset);

/**********************************************************************************************************************
 *  E2E_P04CheckReadLength()
 *********************************************************************************************************************/
/*!
 * \brief      Return the length field of the E2E header
 * \details    Return the length field of the E2E header which was stored in big endian format
 * \param[in]  DataPtr  Pointer to the data to be protected
 * \param[in]  Offset   Offset of the E2E header in DataPtr in bytes
 * \return     uint16   The value of the Length field
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (uint16, E2E_CODE) E2E_P04CheckReadLength(P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                         VAR (uint16, AUTOMATIC ) Offset);

/**********************************************************************************************************************
 *  E2E_P04ProtectWriteCounter()
 *********************************************************************************************************************/
/*!
 * \brief      Write the counter value to the E2E header
 * \details    Write the counter value to the E2E header in big endian format
 * \param[in]  StatePtr  Pointer to the state data structure
 * \param[in]  DataPtr   Pointer to the data to be protected
 * \param[in]  Offset    Offset of the E2E header in DataPtr in bytes
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_P04ProtectWriteCounter(P2CONST (E2E_P04ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
                                                           P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                           VAR (uint16, AUTOMATIC ) Offset);

/**********************************************************************************************************************
 *  E2E_P04CheckReadCounter()
 *********************************************************************************************************************/
/*!
 * \brief      Return the Counter field of the E2E header
 * \details    Return the Counter field of the E2E header which was stored in big endian format
 * \param[in]  DataPtr  Pointer to the data to be protected
 * \param[in]  Offset   Offset of the E2E header in DataPtr in bytes
 * \return     uint16   The value of the Counter field
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (uint16, E2E_CODE) E2E_P04CheckReadCounter(P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                          VAR (uint16, AUTOMATIC ) Offset);

/**********************************************************************************************************************
 *  E2E_P04ProtectWriteDataID()
 *********************************************************************************************************************/
/*!
 * \brief      Write the DataID to the E2E header
 * \details    Write the DataID to the E2E header in big endian format
 * \param[in]  ConfigPtr  Pointer to the config data structure
 * \param[in]  DataPtr    Pointer to the data to be protected
 * \param[in]  Offset     Offset of the E2E header in DataPtr in bytes
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_P04ProtectWriteDataID(P2CONST (E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                          P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
                                                          VAR (uint16, AUTOMATIC ) Offset);

/**********************************************************************************************************************
 *  E2E_P04CheckReadDataID()
 *********************************************************************************************************************/
/*!
 * \brief      Return the DataID field of the E2E header
 * \details    Return the DataID field of the E2E header which was stored in big endian format
 * \param[in]  DataPtr  Pointer to the data to be protected
 * \param[in]  Offset   Offset of the E2E header in DataPtr in bytes
 * \return     uint32 The value of the DataID field
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (uint32, E2E_CODE) E2E_P04CheckReadDataID(P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                         VAR (uint16, AUTOMATIC ) Offset);

/**********************************************************************************************************************
 *  E2E_P04ComputeCRC()
 *********************************************************************************************************************/
/*!
 * \brief      Compute the CRC over payload and header
 * \details    Compute the CRC over payload and header using CRC32 Profile 4 service
 * \param[in]  DataPtr  Pointer to the data to be protected
 * \param[in]  Length   length of the data in bytes
 * \param[in]  Offset   Offset of the E2E header in DataPtr in bytes
 * \return     uint32   The calculated CRC value
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (uint32, E2E_CODE) E2E_P04ComputeCRC(P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                    VAR (uint16, AUTOMATIC ) Length,
                                                    VAR (uint16, AUTOMATIC ) Offset);

/**********************************************************************************************************************
 *  E2E_P04ProtectWriteCRC()
 *********************************************************************************************************************/
/*!
 * \brief      Write the computed CRC value to the E2E header
 * \details    Write the computed CRC value to the E2E header in big endian format
 * \param[in]  DataPtr      Pointer to the data to be protected
 * \param[in]  Offset       Offset of the E2E header in DataPtr in bytes
 * \param[in]  ComputedCRC  The computed CRC value
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_P04ProtectWriteCRC(P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                       VAR (uint16, AUTOMATIC ) Offset,
                                                       VAR (uint32, AUTOMATIC ) ComputedCRC);

/**********************************************************************************************************************
 *  E2E_P04CheckReadCRC()
 *********************************************************************************************************************/
/*!
 * \brief      Return the CRC value of the E2E header
 * \details    Return the CRC value of the E2E header which was stored in big endian format
 * \param[in]  DataPtr  Pointer to the data to be protected
 * \param[in]  Offset   Offset of the E2E header in DataPtr in bytes
 * \return     uint32   The CRC value of the E2E header
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (uint32, E2E_CODE) E2E_P04CheckReadCRC(P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                      VAR (uint16, AUTOMATIC ) Offset);

/**********************************************************************************************************************
 *  E2E_P04ProtectIncrementCounter()
 *********************************************************************************************************************/
/*!
 * \brief      Increment the Counter value of the state data structure
 * \details    Increment the Counter value of the state data structure
 * \param[in]  StatePtr  Pointer to the state data structure
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_P04ProtectIncrementCounter(P2VAR (E2E_P04ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);

/**********************************************************************************************************************
 *  E2E_P04CheckDoChecks()
 *********************************************************************************************************************/
/*!
 * \brief       Determine the E2E status based on received and expected values
 * \details     Determine the E2E status based on received and expected values
 * \param[in]   ConfigPtr         Pointer to the config data structure
 * \param[out]  StatePtr          Pointer to the state data structure
 * \param[in]   Length            The length of the received data in bytes
 * \param[in]   NewDataAvailable  Indicating if new data was received
 * \param[in]   ReceivedLength    The value of the Length field of the E2E header
 * \param[in]   ReceivedCounter   The value of the Counter field of the E2E header
 * \param[in]   ReceivedDataID    The value of the DataID field of the E2E header
 * \param[in]   ReceivedCRC       The value of the CRC field of the E2E header
 * \param[in]   ComputedCRC       The value of the CRC calculated over the data
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_P04CheckDoChecks(P2CONST (E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                     P2VAR (E2E_P04CheckStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                                     VAR (uint16, AUTOMATIC ) Length,
                                                     VAR (boolean, AUTOMATIC ) NewDataAvailable,
                                                     VAR (uint16, AUTOMATIC ) ReceivedLength,
                                                     VAR (uint16, AUTOMATIC ) ReceivedCounter,
                                                     VAR (uint32, AUTOMATIC ) ReceivedDataID,
                                                     VAR (uint32, AUTOMATIC ) ReceivedCRC,
                                                     VAR (uint32, AUTOMATIC ) ComputedCRC);

/**********************************************************************************************************************
 *  E2E_P04CheckDoChecksCounter()
 *********************************************************************************************************************/
/*!
 * \brief      Determine the E2E status based on received and expected Counter
 * \details    Determine the E2E status based on received and expected Counter
 * \param[in]  ConfigPtr        Pointer to the config data structure
 * \param[in]  StatePtr         Pointer to the state data structure
 * \param[in]  ReceivedCounter  The value of the Counter field of the E2E header
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_P04CheckDoChecksCounter(P2CONST (E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                            P2VAR (E2E_P04CheckStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                                            VAR (uint16, AUTOMATIC ) ReceivedCounter);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P04ProtectVerifyInputs()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
E2E_LOCAL FUNC (Std_ReturnType, E2E_CODE) E2E_P04ProtectVerifyInputs(P2CONST (E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                                     P2CONST (E2E_P04ProtectStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                                                     P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
                                                                     VAR (uint16, AUTOMATIC ) Length)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* #10 Check if input pointer parameters are not NULL and Length is greater than 0 */
    if (  (ConfigPtr != NULL_PTR)
       && (StatePtr != NULL_PTR)
       && (DataPtr != NULL_PTR)
       )
    {
        /* #20 Check if MinDataLength <= Length <= MaxDataLength */
        if (  (Length >= (ConfigPtr->MinDataLength >> 3u))
           && (Length <= (ConfigPtr->MaxDataLength >> 3u))
           )
        {
            ret = E2E_E_OK;
        }
        else
        {
          ret = E2E_E_INPUTERR_WRONG;
        }
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P04CheckVerifyInputs()
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
E2E_LOCAL FUNC (Std_ReturnType, E2E_CODE) E2E_P04CheckVerifyInputs(P2CONST (E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                                   P2CONST (E2E_P04CheckStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                                                   P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
                                                                   VAR (uint16, AUTOMATIC ) Length,
                                                                   P2VAR (boolean, AUTOMATIC, E2E_APPL_DATA ) NewDataAvailablePtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_WRONG;

    /* #10 Variable is defined in Check service, no need to store it in the State structure */
    *NewDataAvailablePtr = FALSE; /* SBSW_E2E_04 */

    /* #20 Check if input pointer parameter are valid */
    if ((ConfigPtr != NULL_PTR) && (StatePtr != NULL_PTR))
    {
        /* #30 Check if both the DataPtr and Length signal that a new message is available */
        if ((DataPtr != NULL_PTR) && (Length != 0u))
        {
            /* #35 Check if Length is valid */
            if (  (Length >= (ConfigPtr->MinDataLength >> 3u))
               && (Length <= (ConfigPtr->MaxDataLength >> 3u)) )
            {
                *NewDataAvailablePtr = TRUE; /* SBSW_E2E_04 */
                ret = E2E_E_OK;
            }
        }
        /* #40 This path may happen at runtime if queued communication is used and no data is available (in this case both Data is NULL and length is 0) */
        else if ( (DataPtr == NULL_PTR) && (Length == 0u) )
             {
                ret = E2E_E_OK;
             }
        else
        {
            ret = E2E_E_INPUTERR_WRONG;
        }
    }
    else
    {
      ret = E2E_E_INPUTERR_NULL;
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P04ProtectWriteLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_P04ProtectWriteLength(P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
                                                          VAR (uint16, AUTOMATIC ) Length,
                                                          VAR (uint16, AUTOMATIC ) Offset)
{
    /* #10 Both Length bytes are written to DataPtr in big endian order (high byte first) */
    DataPtr[Offset + 0u] = (uint8) (Length >> 8u); /* SBSW_E2E_03 */
    DataPtr[Offset + 1u] = (uint8) (Length); /* SBSW_E2E_03 */
}

/**********************************************************************************************************************
 *  E2E_P04CheckReadLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2E_LOCAL FUNC (uint16, E2E_CODE) E2E_P04CheckReadLength(P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                         VAR (uint16, AUTOMATIC ) Offset)
{
    /* #10 Stores Length bytes in big endian (high byte first) order to local variable ReceivedLength */
    return (((uint16) DataPtr[Offset + 0u]) << 8u)
                   | (DataPtr[Offset + 1u]);
}

/**********************************************************************************************************************
 *  E2E_P04ProtectWriteCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_P04ProtectWriteCounter(P2CONST (E2E_P04ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
                                                           P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                           VAR (uint16, AUTOMATIC ) Offset)
{
  /* #10 Both Counter bytes are written to DataPtr in big endian order (high byte first) */
    DataPtr[Offset + 2u] = (uint8) (StatePtr->Counter >> 8u); /* SBSW_E2E_03 */
    DataPtr[Offset + 3u] = (uint8) (StatePtr->Counter); /* SBSW_E2E_03 */
}

/**********************************************************************************************************************
 *  E2E_P04CheckReadCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2E_LOCAL FUNC (uint16, E2E_CODE) E2E_P04CheckReadCounter(P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                          VAR (uint16, AUTOMATIC ) Offset)
{
    /* #10 Stores Counter bytes in big endian (high byte first) order to local variable ReceivedCounter */
    return (((uint16) DataPtr[Offset + 2]) << 8u)
                    | (DataPtr[Offset + 3u]);
}

/**********************************************************************************************************************
 *  E2E_P04ProtectWriteDataID()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_P04ProtectWriteDataID(P2CONST (E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                          P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
                                                          VAR (uint16, AUTOMATIC ) Offset)
{
    /* #10 All four DataID bytes are written to DataPtr in big endian order (high byte first) */
    DataPtr[Offset + 4u] = (uint8) (ConfigPtr->DataID >> 24u); /* SBSW_E2E_03 */
    DataPtr[Offset + 5u] = (uint8) (ConfigPtr->DataID >> 16u); /* SBSW_E2E_03 */
    DataPtr[Offset + 6u] = (uint8) (ConfigPtr->DataID >>  8u); /* SBSW_E2E_03 */
    DataPtr[Offset + 7u] = (uint8) (ConfigPtr->DataID); /* SBSW_E2E_03 */
}

/**********************************************************************************************************************
 *  E2E_P04CheckReadDataID()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2E_LOCAL FUNC (uint32, E2E_CODE) E2E_P04CheckReadDataID(P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                         VAR (uint16, AUTOMATIC ) Offset)
 {
    /* #10 Stores DataID bytes in big endian (high byte first) order to local variable ReceivedDataID */
    return (((uint32) DataPtr[Offset + 4u]) << 24u)
                   | (((uint32) DataPtr[Offset + 5u]) << 16u)
                   | (((uint32) DataPtr[Offset + 6u]) <<  8u)
                   | (((uint32) DataPtr[Offset + 7u]) <<  0u);
}

/**********************************************************************************************************************
 *  E2E_P04ComputeCRC()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
E2E_LOCAL FUNC (uint32, E2E_CODE) E2E_P04ComputeCRC(P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                    VAR (uint16, AUTOMATIC ) Length,
                                                    VAR (uint16, AUTOMATIC ) Offset)
{
    /* #10 Compute CRC over bytes that are before CRC */
    uint32 ComputedCRC = Crc_CalculateCRC32P4 (DataPtr, Offset + 8u, 0xFFFFFFFFu, TRUE); /* SBSW_E2E_03 */

    /* #20 Compute CRC over bytes that are after CRC (if any) */
    if ((Offset + 12u) < Length) {
        ComputedCRC = Crc_CalculateCRC32P4 (&(DataPtr[Offset + 12u]), Length - Offset - 12u, ComputedCRC, FALSE); /* SBSW_E2E_03 */
    }
    return ComputedCRC;
}

/**********************************************************************************************************************
 *  E2E_P04ProtectWriteCRC()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_P04ProtectWriteCRC(P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                       VAR (uint16, AUTOMATIC ) Offset,
                                                       VAR (uint32, AUTOMATIC ) ComputedCRC)
{
    /* #10 All four CRC bytes are written to DataPtr in big endian order (high byte first) */
    DataPtr[Offset + 8u] =  (uint8) (ComputedCRC  >> 24u); /* SBSW_E2E_03 */
    DataPtr[Offset + 9u] =  (uint8) (ComputedCRC  >> 16u); /* SBSW_E2E_03 */
    DataPtr[Offset + 10u] = (uint8) (ComputedCRC  >>  8u); /* SBSW_E2E_03 */
    DataPtr[Offset + 11u] = (uint8) (ComputedCRC); /* SBSW_E2E_03 */
}

/**********************************************************************************************************************
 *  E2E_P04CheckReadCRC()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2E_LOCAL FUNC (uint32, E2E_CODE) E2E_P04CheckReadCRC(P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                      VAR (uint16, AUTOMATIC ) Offset)
{
    /* #10 Stores CRC32 bytes from DataPtr in big endian (high byte first) order to local variable ReceivedCRC */
    return (((uint32) DataPtr[Offset + 8u])  << 24u)
                | (((uint32) DataPtr[Offset + 9u])  << 16u)
                | (((uint32) DataPtr[Offset + 10u]) <<  8u)
                | (((uint32) DataPtr[Offset + 11u]) <<  0u);
}

/**********************************************************************************************************************
 *  E2E_P04ProtectIncrementCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_P04ProtectIncrementCounter(P2VAR (E2E_P04ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr)
{
    /* #10 Counter is incremented. Valid values are 0..0xFFFF. Thus there is no need to care about overflow */
    StatePtr->Counter++; /* SBSW_E2E_03 */
}

/**********************************************************************************************************************
 *  E2E_P04CheckDoChecks()
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
E2E_LOCAL FUNC (void, E2E_CODE) E2E_P04CheckDoChecks(P2CONST (E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                     P2VAR (E2E_P04CheckStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                                     VAR (uint16, AUTOMATIC ) Length,
                                                     VAR (boolean, AUTOMATIC ) NewDataAvailable,
                                                     VAR (uint16, AUTOMATIC ) ReceivedLength,
                                                     VAR (uint16, AUTOMATIC ) ReceivedCounter,
                                                     VAR (uint32, AUTOMATIC ) ReceivedDataID,
                                                     VAR (uint32, AUTOMATIC ) ReceivedCRC,
                                                     VAR (uint32, AUTOMATIC ) ComputedCRC)
{
    /* #10 Check if new data is available */
    if (NewDataAvailable == TRUE)
    {
        /* #20 Check if received CRC matches the computed one */
        if (ReceivedCRC == ComputedCRC)
        {
            /* #30 Check if received DataID matches with the one in configuration */
            if (ReceivedDataID == ConfigPtr->DataID)
            {
                /* #40 Check if Received length matches with Length */
                if (ReceivedLength == Length)
                {
                    /* #50 Evaluate counter value */
                    E2E_P04CheckDoChecksCounter (ConfigPtr, StatePtr,  ReceivedCounter); /* SBSW_E2E_03 */
                }
                else
                {
                    StatePtr->Status = E2E_P04STATUS_ERROR; /* SBSW_E2E_03 */
                }
            }
            else
            {
                StatePtr->Status = E2E_P04STATUS_ERROR; /* SBSW_E2E_03 */
            }
        }
        else
        {
            StatePtr->Status = E2E_P04STATUS_ERROR; /* SBSW_E2E_03 */
        }
    }
    else
    {
        StatePtr->Status = E2E_P04STATUS_NONEWDATA; /* SBSW_E2E_03 */
    }
} /* PRQA S 6060 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
 *  E2E_P04CheckDoChecksCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
E2E_LOCAL FUNC (void, E2E_CODE) E2E_P04CheckDoChecksCounter(P2CONST (E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                            P2VAR (E2E_P04CheckStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                                            VAR (uint16, AUTOMATIC ) ReceivedCounter)
{
    /* #10 Compute DeltaCounter. Wrap around is considered */
    uint16 DeltaCounter = (uint16)(ReceivedCounter - StatePtr->Counter);

    /* #20 Check if DeltaCounter is within acceptable range */
    if (DeltaCounter <= ConfigPtr->MaxDeltaCounter)
    {
        /* #30 Check if message was not repeated */
        if (DeltaCounter > 0u)
        {
            /* #40 Check if correct subsequent message was received. No data was lost in between communication */
            if (DeltaCounter == 1u)
            {
                StatePtr->Status = E2E_P04STATUS_OK; /* SBSW_E2E_03 */
            }
            else
            {
                StatePtr->Status = E2E_P04STATUS_OKSOMELOST; /* SBSW_E2E_03 */
            }
        }
        else
        {
            StatePtr->Status = E2E_P04STATUS_REPEATED; /* SBSW_E2E_03 */
        }
    }
    else
    {
        StatePtr->Status = E2E_P04STATUS_WRONGSEQUENCE; /* SBSW_E2E_03 */
    }

    StatePtr->Counter = ReceivedCounter; /* SBSW_E2E_03 */
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  E2E_P04Protect()
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
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P04Protect( P2CONST (E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                P2VAR (E2E_P04ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
                                                P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                                VAR (uint16, AUTOMATIC) Length)
{
    /* #10 Check input parameter */
    Std_ReturnType ret = E2E_P04ProtectVerifyInputs (ConfigPtr, StatePtr, DataPtr, Length); /* SBSW_E2E_03 */

    if (ret == E2E_E_OK)
    {
        uint32 ComputedCRC;
        /* #20 Compute Offset */
        uint16 NewOffset = ConfigPtr->Offset >> 3u;
        /* #21 Write Length to DataPtr */
        E2E_P04ProtectWriteLength (DataPtr, Length, NewOffset); /* SBSW_E2E_03 */
        /* #22 Write Counter to DataPtr */
        E2E_P04ProtectWriteCounter (StatePtr, DataPtr, NewOffset); /* SBSW_E2E_03 */
        /* #23 Write DataID to DataPtr */
        E2E_P04ProtectWriteDataID (ConfigPtr, DataPtr, NewOffset); /* SBSW_E2E_03 */
        /* #24 Compute CRC */
        ComputedCRC = E2E_P04ComputeCRC (DataPtr, Length, NewOffset); /* SBSW_E2E_03 */
        /* #25 Write CRC to DataPtr */
        E2E_P04ProtectWriteCRC (DataPtr, NewOffset, ComputedCRC); /* SBSW_E2E_03 */
        /* #26 Increment Counter */
        E2E_P04ProtectIncrementCounter (StatePtr); /* SBSW_E2E_03 */
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P04ProtectInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P04ProtectInit( P2VAR (E2E_P04ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr)
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
 *  E2E_P04Check()
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
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P04Check( P2CONST (E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                              P2VAR (E2E_P04CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
                                              P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
                                              VAR (uint16, AUTOMATIC) Length)
{

    boolean NewDataAvailable = FALSE;
    /* #10 Check input parameter */
    Std_ReturnType ret = E2E_P04CheckVerifyInputs ( ConfigPtr /* SBSW_E2E_02 */
                                   , StatePtr
                                   , DataPtr
                                   , Length
                                   , &NewDataAvailable
                                   );

    if (ret == E2E_E_OK) {
        uint16 NewOffset;
        uint16 ReceivedLength   = 0u;
        uint16 ReceivedCounter  = 0u;
        uint32 ReceivedDataID   = 0u;
        uint32 ReceivedCRC      = 0u;
        uint32 ComputedCRC      = 0u;

        if (NewDataAvailable == TRUE) {
            /* #20 Compute Offset */
            NewOffset = ConfigPtr->Offset >> 3u;
            /* #21 Read Length */
            ReceivedLength  = E2E_P04CheckReadLength (DataPtr, NewOffset); /* SBSW_E2E_03 */
            /* #22 Read Counter */
            ReceivedCounter = E2E_P04CheckReadCounter (DataPtr, NewOffset); /* SBSW_E2E_03 */
            /* #23 Read DataID */
            ReceivedDataID  = E2E_P04CheckReadDataID (DataPtr, NewOffset); /* SBSW_E2E_03 */
            /* #24 Read CRC */
            ReceivedCRC     = E2E_P04CheckReadCRC (DataPtr, NewOffset); /* SBSW_E2E_03 */
            /* #25 Compute CRC */
            ComputedCRC     = E2E_P04ComputeCRC (DataPtr, Length, NewOffset); /* SBSW_E2E_03 */
        }
        /* #30 Do checks */
        E2E_P04CheckDoChecks (                  /* SBSW_E2E_03 */
                               ConfigPtr
                             , StatePtr
                             , Length
                             , NewDataAvailable
                             , ReceivedLength
                             , ReceivedCounter
                             , ReceivedDataID
                             , ReceivedCRC
                             , ComputedCRC
                             );
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P04CheckInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P04CheckInit( P2VAR (E2E_P04CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* #10 Reset State structure in case StatePtr is valid */
    if (StatePtr != NULL_PTR)
    {
        StatePtr->Counter = 0xFFFFu; /* SBSW_E2E_03 */
        StatePtr->Status = E2E_P04STATUS_ERROR; /* SBSW_E2E_03 */
        ret = E2E_E_OK;
    }

    return ret;
}

/**********************************************************************************************************************
 *  E2E_P04MapStatusToSM()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC (E2E_PCheckStatusType, E2E_CODE) E2E_P04MapStatusToSM (VAR (Std_ReturnType, AUTOMATIC) CheckReturn,
                                                            VAR(E2E_P04CheckStatusType, AUTOMATIC) Status)
{
    E2E_PCheckStatusType ret = E2E_P_ERROR;

    /* #10 Status is only mapped to state machine if check-function returned successfully */
    if (CheckReturn == E2E_E_OK)
    {
        /* #20 Status of Profile 4 is mapped to state machine status, which is not that fine-grained */
        switch (Status)
        {
            case E2E_P04STATUS_OK           : ret = E2E_P_OK; break;
            case E2E_P04STATUS_OKSOMELOST   : ret = E2E_P_OK; break;
            case E2E_P04STATUS_ERROR        : ret = E2E_P_ERROR; break;
            case E2E_P04STATUS_REPEATED     : ret = E2E_P_REPEATED; break;
            case E2E_P04STATUS_NONEWDATA    : ret = E2E_P_REPEATED; break;
            case E2E_P04STATUS_WRONGSEQUENCE: ret = E2E_P_WRONGSEQUENCE; break;
            default: break; /* PRQA S 2018 */ /* MD_EtoE_14.1 */
        }
    }

    return ret;
}

#define E2E_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: E2E_P04.c
 *********************************************************************************************************************/
