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
 *  -------------------------------------------------------------------------------------------------------------------
 *        \file  Ea_InstanceHandler.h
 *         \brief  Ea Sub-component InstanceHandler source file
 *
 *       \details  InstanceHandler sub-component is responsible for management and initialization of instance relevant
 *                 data. InstanceHandler provides public services to other EA sub-components to create instance objects
 *                 and access their data.
 *
 *********************************************************************************************************************/

/**** Protection against multiple inclusion **************************************************************************/
#ifndef EA_INSTANCE_HANDLER_H
# define EA_INSTANCE_HANDLER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Ea.h"

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

# define EA_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

typedef enum
{
  INSTANCE_STATUS_ERASED = 0,
  INSTANCE_STATUS_OK,
  INSTANCE_STATUS_INVALID,
  INSTANCE_STATUS_INCONSISTENT
}EaIh_InstanceStatusType;

typedef P2VAR(uint8, AUTOMATIC, EA_APPL_DATA) EaIh_BufferPointerType;

typedef struct
{
  Ea_AddressType EaIh_CompareAddress;
  uint16 EaIh_CompareLength;
  EaIh_BufferPointerType EaIh_CompareBufferPtr;
}EaIh_CompareParameterType;

typedef struct
{
  Ea_AddressType HeaderAddress;
  Ea_AddressType TrailerAddress;
  uint16 DataLength;
  uint16 InstanceLength;
  uint8 HeaderValue;
  uint8 TrailerValue;
  uint8 CycleCounter;
  EaIh_InstanceStatusType InstanceStatus;
}EaIh_InstanceType;

typedef P2VAR(EaIh_CompareParameterType, AUTOMATIC, EA_APPL_DATA) EaIh_CompareParameterPointerType;
typedef P2VAR(EaIh_InstanceType, AUTOMATIC, EA_APPL_DATA) EaIh_InstanceVarPointerType;
typedef P2CONST(EaIh_InstanceType, AUTOMATIC, EA_APPL_DATA) EaIh_InstanceConstPointerType;


# define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define EA_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EaIh_CreateInstance()
 *********************************************************************************************************************/
/*!
 * \brief      Creates initialized instance object
 * \details    Creates and stores initialized instance values to instance object referenced by parameter Instance
 * \param[in]  Instance
 * \param[in]  InstanceStartAddress
 * \param[in]  DataLength
 * \param[in]  CycleCounter
 * \return     Instance object
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (void, EA_PUBLIC_CODE) EaIh_CreateInstance(EaIh_InstanceVarPointerType Instance,
        Ea_AddressType InstanceStartAddress, uint16 DataLength, uint8 CycleCounter);

/**********************************************************************************************************************
 *  EaIh_InitInstance()
 *********************************************************************************************************************/
/*!
 * \brief      Initializes instance which is passed via input parameter
 * \details    Initializes instance which is passed via input parameter
 * \param[in]  Instance
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (void, EA_PUBLIC_CODE) EaIh_InitInstance(EaIh_InstanceVarPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_GetInstanceStartAddress()
 *********************************************************************************************************************/
/*!
 * \brief      Returns start address of instance
 * \details    Returns start address of instance
 * \param[in]  Instance
 * \return     InstanceStartAddress
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (Ea_AddressType, EA_PUBLIC_CODE) EaIh_GetInstanceStartAddress(EaIh_InstanceConstPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_GetTrailerAddressFromHeaderAddress()
 *********************************************************************************************************************/
/*!
 * \brief      Calculates address of trailer management byte based on address of header byte
 * \details    Calculates address of trailer management byte based on address of header byte
 * \param[in]  Instance
 * \param[in]  HeaderAddress
 * \return     Address of trailer
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(Ea_AddressType, EA_PRIVATE_CODE) EaIh_GetTrailerAddressFromHeaderAddress(EaIh_InstanceConstPointerType Instance,
        Ea_AddressType HeaderAddress);

/**********************************************************************************************************************
 *  EaIh_GetInstanceLength()
 *********************************************************************************************************************/
/*!
 * \brief      Returns length of instance
 * \details    Returns length of instance, including the data length and two times the size of EA Management part
 *             (Header and Trailer)
 * \param[in]  Instance
 * \return     Length of instance
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (uint16, EA_PUBLIC_CODE) EaIh_GetInstanceLength(EaIh_InstanceConstPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_GetInstanceStatus()
 *********************************************************************************************************************/
/*!
 * \brief      Returns status of instance
 * \details    Evaluates and returns status of instance based on header and trailer values
 * \param[in]  Instance
 * \return     InstanceStatus
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (EaIh_InstanceStatusType, EA_PUBLIC_CODE) EaIh_GetInstanceStatus(EaIh_InstanceConstPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_GetHeaderValue()
 *********************************************************************************************************************/
/*!
 * \brief      Returns value of header management byte
 * \details    Returns value of header management byte
 * \param[in]  Instance
 * \return     Value of header
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(uint8, EA_PRIVATE_CODE) EaIh_GetHeaderValue(EaIh_InstanceConstPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_GetTrailerValue()
 *********************************************************************************************************************/
/*!
 * \brief      Returns value of trailer management byte
 * \details    Returns value of trailer management byte
 * \param[in]  Instance
 * \return     Value of trailer
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(uint8, EA_PRIVATE_CODE) EaIh_GetTrailerValue(EaIh_InstanceConstPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_StoreHeaderValueFromBuffer()
 *********************************************************************************************************************/
/*!
 * \brief      Stores recently read value of header management byte from read buffer to Instance object
 * \details    Stores recently read value of header management byte from read buffer to Instance object
 *             This service is called after ReadManagementBytes sub-component stored header value in buffer.
 * \param[in]  Instance
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PRIVATE_CODE) EaIh_StoreHeaderValueFromBuffer(EaIh_InstanceVarPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_StoreTrailerValueFromBuffer()
 *********************************************************************************************************************/
/*!
 * \brief      Stores recently read value of trailer management byte from read buffer to Instance
 * \details    Stores recently read value of trailer management byte from read buffer to Instance
 *             This service is called after ReadManagementBytes sub-component stored trailer value in buffer.
 * \param[in]  Instance
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PRIVATE_CODE) EaIh_StoreTrailerValueFromBuffer(EaIh_InstanceVarPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_StoreHeaderAndTrailerValueFromBuffer()
 *********************************************************************************************************************/
/*!
 * \brief      Stores recently read value of header and trailer management byte from read buffer to passed instance
 * \details    Stores recently read value of header and trailer management byte from read buffer to passed instance.
 *             This service is called after ReadManagementBytes sub-component stored both header and trailer in buffer.
 * \param[in]  Instance
 * \return     Value of trailer
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(void, EA_PRIVATE_CODE) EaIh_StoreHeaderAndTrailerValueFromBuffer(EaIh_InstanceVarPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_IsValidOrInvalidManagement()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if passed ManagementByte is either one of valid management byte values or
 *             invalid management byte values
 * \details    Checks if passed ManagementByte is either one of valid management byte values or
 *             invalid management byte values
 * \param[in]  HeaderByte
 * \param[in]  TrailerByte
 * \return     TRUE: One of ManagementBytes is either valid or invalid
 *             FALSE: ManagementBytes are "inconsistent"
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(boolean, EA_PRIVATE_CODE) EaIh_IsValidOrInvalidManagement(uint8 HeaderByte, uint8 TrailerByte);

/**********************************************************************************************************************
 *  EaIh_IsStatusOkOrInvalid()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if instance's status is either OK or Invalid
 * \details    Used for invalidating or writing an instance. In case of an invalid or OK instance,
 *             next instance will be written/invalidated.
 * \param[in]  Instance
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC(boolean, EA_PRIVATE_CODE) EaIh_IsStatusOkOrInvalid(EaIh_InstanceConstPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_SetInstanceStatus()
 *********************************************************************************************************************/
/*!
 * \brief      Sets status of passed instance
 * \details    Sets status of passed instance
 * \param[in]  Instance
 * \param[in]  status
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (void, EA_PUBLIC_CODE) EaIh_SetInstanceStatus(EaIh_InstanceVarPointerType Instance, EaIh_InstanceStatusType status);

/**********************************************************************************************************************
 *  EaIh_SetHeaderAndTrailerAddress()
 *********************************************************************************************************************/
/*!
 * \brief      Sets values of header and trailer address
 * \details    Sets values of header and trailer address
 * \param[in]  Instance
 * \param[in]  HeaderAddress
 * \param[in]  TrailerAddress
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (void, EA_PUBLIC_CODE) EaIh_SetHeaderAndTrailerAddress(EaIh_InstanceVarPointerType Instance,
        Ea_AddressType HeaderAddress, Ea_AddressType TrailerAddress);

/**********************************************************************************************************************
 *  EaIh_SetManagementValuesAccordingToStatus()
 *********************************************************************************************************************/
/*!
 * \brief      Sets management bytes with either valid or invalid values
 * \details    Sets both header and trailer values of passed instance with either valid (write job) or
 *             invalid values (invalidate job). Values are calculated from CycleCounter,
 *             which is stored in instance object.
 * \param[in]  Instance
 * \param[in]  IsValid
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (void, EA_PUBLIC_CODE) EaIh_SetManagementValuesAccordingToStatus(EaIh_InstanceVarPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_SetNegativeJobResultAccordingToStatus()
 *********************************************************************************************************************/
/*!
 * \brief      Returns job result according to instance's status
 * \details    Returns job result according to instance's status
 * \param[in]  Instance
 * \return     MemIf_JobResult
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (MemIf_JobResultType, EA_PUBLIC_CODE) EaIh_SetNegativeJobResultAccordingToStatus(EaIh_InstanceConstPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_SetInstanceProperties()
 *********************************************************************************************************************/
/*!
 * \brief      Sets status and cycle counter of passed instance
 * \details    Sets status and cycle counter of passed instance based on management values
 * \param[in]  Instance
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (void, EA_PUBLIC_CODE) EaIh_SetInstanceProperties(EaIh_InstanceVarPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_IncrementCycleCounter()
 *********************************************************************************************************************/
/*!
 * \brief      Increments Cycle Counter of Instance
 * \details    Increments Cycle Counter of Instance considering a wrap around of cycle counter's value
 * \param[in]  Instance
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (void, EA_PUBLIC_CODE) EaIh_IncrementCycleCounter(EaIh_InstanceVarPointerType Instance);

# if (EA_CORRECT_SINGLE_BIT_FLIPS == STD_ON)
/**********************************************************************************************************************
 *  EaIh_CorrectSingleBitFlips()
 *********************************************************************************************************************/
/*!
 * \brief      Performs single bit flip correction on passed ManagementByte
 * \details    Performs single bit flip correction on passed ManagementByte
 * \param[in]  ManagementByte
 * \pre        -
 * \config     Pre-compile switch "CorrectSingleBitFlips" is enabled
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace DSGN-Ea22978
 */
FUNC (void, EA_PUBLIC_CODE) EaIh_CorrectSingleBitFlips(EaIh_BufferPointerType ManagementByte);
# endif

/**********************************************************************************************************************
 *  EaIh_CompareTo()
 *********************************************************************************************************************/
/*!
 * \brief      Compares prevInstance with Instance and determines which one's more up to date
 * \details    Compares prevInstance with Instance and determines which one's more up to date
 * \param[in]  prevInstance
 * \param[in]  Instance
 * \return     TRUE :  both instances have same age
 * \return     FALSE : prevInstance is more up to date than Instance
 * \pre        Pointer to parameters PrevInstance and Instance of type EaIh_InstanceType are valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (boolean, EA_PUBLIC_CODE) EaIh_CompareTo(EaIh_InstanceConstPointerType PrevInstance, EaIh_InstanceConstPointerType Instance);


/**********************************************************************************************************************
 *  EaIh_ReadHeader()
 *********************************************************************************************************************/
/*!
 * \brief      Requests reading header byte of instance
 * \details    Requests reading header byte of instance by calling read service of EEP driver
 * \param[in]  Instance
 * \return     E_OK: Request to EEP driver was successful
 * \return     E_NOT_OK: Request to EEP driver failed
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_ReadHeader(EaIh_InstanceConstPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_ReadTrailer()
 *********************************************************************************************************************/
/*!
 * \brief      Requests reading trailer byte of instance
 * \details    Requests reading trailer byte of instance by calling read service of EEP driver
 * \param[in]  Instance
 * \return     E_OK: Request to EEP driver was successful
 * \return     E_NOT_OK: Request to EEP driver failed
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_ReadTrailer(EaIh_InstanceConstPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_ReadHeaderAndTrailer()
 *********************************************************************************************************************/
/*!
 * \brief      Requests reading header and trailer byte of instance
 * \details    Requests reading header and trailer byte of instance to read buffer by calling read service of EEP driver
 * \param[in]  Instance
 * \return     E_OK: Request to EEP driver was successful
 * \return     E_NOT_OK: Request to EEP driver failed
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_ReadHeaderAndTrailer(EaIh_InstanceConstPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_ReadData()
 *********************************************************************************************************************/
/*!
 * \brief      Requests reading data bytes of instance
 * \details    Requests reading data bytes of instance by calling read service of EEP driver
 * \param[in]  InstanceHeaderAddress
 * \param[in]  DataBufferPtr
 * \param[in]  BlockOffset
 * \param[in]  ReadLength
 * \return     E_OK: Request to EEP driver was successful
 * \return     E_NOT_OK: Request to EEP driver failed
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_ReadData(EaIh_InstanceConstPointerType Instance,
                                                    EaIh_BufferPointerType DataBufferPtr,
                                                    uint16 BlockOffset,
                                                    uint16 ReadLength);

/**********************************************************************************************************************
 *  EaIh_WriteHeader()
 *********************************************************************************************************************/
/*!
 * \brief      Writes first part of data, including header byte
 * \details    Writes first part of data to instance, including header byte. The length of this write job is always
 *             equal to size of write alignment. Internally, a temporary write buffer is prepared with the header byte
 *             and with some data bytes. If write alignment is equal to 1, only the header byte is written.
 * \param[in]  Instance
 * \param[in]  UserJobParameter
 * \param[in]  CompareParameter
 * \return     E_OK: Request to EEP driver was successful
 * \return     E_NOT_OK: Request to EEP driver failed
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_WriteHeader(EaIh_InstanceConstPointerType Instance,
        Ea_UserJobParameterType UserJobParameter, EaIh_CompareParameterPointerType CompareParameter);

/**********************************************************************************************************************
 *  EaIh_WriteMiddlePart()
 *********************************************************************************************************************/
/*!
 * \brief      Writes middle part of data
 * \details    Writes middle part of data to instance. This service is used if instance size is big enough to
 *             write header, data and trailer bytes separately. Write parameters are saved to CompareParameter structure
 *             in order to provide all information for following verification step
 * \param[in]  Instance
 * \param[in]  UserJobParameter
 * \param[in]  CompareParameter
 * \return     E_OK: Request to EEP driver was successful
 * \return     E_NOT_OK: Request to EEP driver failed
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_WriteMiddlePart(EaIh_InstanceConstPointerType Instance,
        Ea_UserJobParameterType UserJobParameter, EaIh_CompareParameterPointerType CompareParameter);

/**********************************************************************************************************************
 *  EaIh_WriteTrailer()
 *********************************************************************************************************************/
/*!
 * \brief      Writes second part of data, including trailer byte
 * \details    Writes second part of data to instance, including trailer byte. This service is used if instance size is
 *             big enough to write header and trailer bytes separately.
 * \param[in]  Instance
 * \param[in]  UserJobParameter
 * \param[in]  CompareParameter
 * \return     E_OK: Request to EEP driver was successful
 * \return     E_NOT_OK: Request to EEP driver failed
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_WriteTrailer(EaIh_InstanceConstPointerType Instance,
        Ea_UserJobParameterType UserJobParameter, EaIh_CompareParameterPointerType CompareParameter);

/**********************************************************************************************************************
 *  EaIh_WriteEntireData()
 *********************************************************************************************************************/
/*!
 * \brief      Writes entire data from buffer to EEPROM
 * \details    Writes entire data from buffer to EEPROM. This service is used if Instance size is quite small
 * \param[in]  Instance
 * \param[in]  UserJobParameter
 * \param[in]  CompareParameter
 * \return     E_OK: Request to EEP driver was successful
 * \return     E_NOT_OK: Request to EEP driver failed
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_WriteEntireData(EaIh_InstanceConstPointerType Instance,
        Ea_UserJobParameterType UserJobParameter, EaIh_CompareParameterPointerType CompareParameter);

/**********************************************************************************************************************
 *  EaIh_VerifyData()
 *********************************************************************************************************************/
/*!
 * \brief      Verifies recently written data
 * \details    Verifies recently written data. Information about recently written data was stored in passed structure.
 * \param[in]  CompareParameter
 * \return     E_OK: Request to EEP driver was successful
 * \return     E_NOT_OK: Request to EEP driver failed
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 * \trace DSGN-Ea22975
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_VerifyData(EaIh_CompareParameterType CompareParameter);

/**********************************************************************************************************************
 *  EaIh_InvalidateHeader()
 *********************************************************************************************************************/
/*!
 * \brief      Invalidates part of instance including Header management
 * \details    Invalidates part of instance including Header management
 * \param[in]  Instance
 * \return     E_OK: Header invalidation was requested successfully
 * \return     E_NOT_OK: Request for header invalidation failed
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_InvalidateHeader(EaIh_InstanceConstPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_InvalidateTrailer()
 *********************************************************************************************************************/
/*!
 * \brief      Invalidates part of instance including Trailer management
 * \details    Invalidates part of instance including Trailer management
 * \param[in]  Instance
 * \return     E_OK: Trailer invalidation was requested successfully
 * \return     E_NOT_OK: Request for trailer invalidation failed
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_InvalidateTrailer(EaIh_InstanceConstPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_InvalidateEntireInstance()
 *********************************************************************************************************************/
/*!
 * \brief      Invalidates entire instance, including Header and Trailer management
 * \details    Invalidates entire instance, including Header and Trailer management
 * \param[in]  Instance
 * \return     E_OK: Invalidation was requested successfully
 * \return     E_NOT_OK: Request for invalidation failed
 * \pre        Pointer to parameter Instance of type EaIh_InstanceType is valid
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_InvalidateEntireInstance(EaIh_InstanceConstPointerType Instance);


# define EA_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* EA_INSTANCE_HANDLER_H */

/**********************************************************************************************************************
 *  END OF FILE: Ea_InstanceHandler.h
 *********************************************************************************************************************/
