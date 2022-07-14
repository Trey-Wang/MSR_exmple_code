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
 *        \file  Ea_InstanceHandler.c
 *         \brief  Ea Sub-component InstanceHandler source file
 *
 *       \details  InstanceHandler sub-component is responsible for management and initialization of instance relevant
 *                 data. InstanceHandler provides public services to other EA sub-components to create instance objects
 *                 and access their data.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define EA_IMPLEMENTATION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Ea_InstanceHandler.h"
#include "Ea_PartitionHandler.h"
#include "Ea_EepCoordinator.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#define EA_CYCLE_COUNTER_BIT_MASK  (0x3U)

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#ifndef EA_LOCAL /* COV_EA_COMPATIBILITY */
# define EA_LOCAL static
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL uint8 EaIh_ReadBuffer[EA_MAX_READ_ALIGNMENT];
EA_LOCAL uint8 EaIh_WriteBuffer[EA_MAX_WRITE_ALIGNMENT];

#define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define EA_START_SEC_CONST_8BIT
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL const uint8 EaIh_ValidManagementValues[4u] = {0xF0u, 0x69u, 0x5Au, 0xC3u};
EA_LOCAL const uint8 EaIh_InvalidManagementValues[4u] = {0x3Cu, 0xA5u, 0x96u, 0x0Fu};

#define EA_STOP_SEC_CONST_8BIT
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define EA_START_SEC_CODE
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  EaIh_GetValidMgmtByteFromCycleCounter()
 *********************************************************************************************************************/
/*!
 * \brief      Returns valid management byte which corresponds to value of cycle counter
 * \details    Returns valid management byte which corresponds to value of cycle counter
 * \param[in]  CycleCounter
 * \return     Element of IH_ValidManagementValues array
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(uint8, EA_PRIVATE_CODE) EaIh_GetValidMgmtByteFromCycleCounter(uint8 CycleCounter);

/**********************************************************************************************************************
 *  EaIh_GetInvalidMgmtByteFromCycleCounter()
 *********************************************************************************************************************/
/*!
 * \brief      Returns invalid management byte which corresponds to value of cycle counter
 * \details    Returns invalid management byte which corresponds to value of cycle counter
 * \param[in]  CycleCounter
 * \return     Element of IH_InvalidManagementValues array
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(uint8, EA_PRIVATE_CODE) EaIh_GetInvalidMgmtByteFromCycleCounter(uint8 CycleCounter);

/**********************************************************************************************************************
 *  EaIh_GetTrailerAddress()
 *********************************************************************************************************************/
/*!
 * \brief      Returns address of trailer management byte
 * \details    Returns address of trailer management byte
 * \param[in]  Instance
 * \return     Address of trailer
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(Ea_AddressType, EA_PRIVATE_CODE) EaIh_GetTrailerAddress(EaIh_InstanceConstPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_GetInstanceDataLength()
 *********************************************************************************************************************/
/*!
 * \brief      Returns length of instance's data
 * \details    Returns length of instance's data. Length of EA Management part is NOT included
 * \param[in]  Instance
 * \return     Data length of instance
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC (uint16, EA_PUBLIC_CODE) EaIh_GetInstanceDataLength(EaIh_InstanceConstPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_GetCycleCounter()
 *********************************************************************************************************************/
/*!
 * \brief      Returns cycle counter of instance
 * \details    Returns cycle counter of instance
 * \param[in]  Instance
 * \return     Cycle counter
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC (uint8, EA_PUBLIC_CODE) EaIh_GetCycleCounter(EaIh_InstanceConstPointerType Instance);

/**********************************************************************************************************************
 *  EaIh_IsValidManagement()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if passed ManagementByte is one of valid management byte values
 * \details    Checks if passed ManagementByte is one of valid management byte values
 * \param[in]  ManagementByte
 * \return     TRUE: ManagementByte is valid
 * \return     FALSE: ManagementByte is not valid
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(boolean, EA_PRIVATE_CODE) EaIh_IsValidManagement(uint8 ManagementByte);

/**********************************************************************************************************************
 *  EaIh_IsInvalidManagement()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if passed ManagementByte is one of invalid management byte values
 * \details    Checks if passed ManagementByte is one of invalid management byte values
 * \param[in]  ManagementByte
 * \return     TRUE: ManagementByte is invalid
 * \return     FALSE: ManagementByte is not invalid. Doesn't have to be "valid" either.
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(boolean, EA_PRIVATE_CODE) EaIh_IsInvalidManagement(uint8 ManagementByte);

#if (EA_CORRECT_SINGLE_BIT_FLIPS == STD_ON)
/**********************************************************************************************************************
 *  EaIh_HasEvenParity()
 *********************************************************************************************************************/
/*!
 * \brief      Checks if passed nibble has even parity
 * \details    Checks if passed nibble has even parity
 * \param[in]  ManagementNibble
 * \return     TRUE: Nibble has even parity
 * \return     FALSE: Nibble has odd parity
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(boolean, EA_PRIVATE_CODE) EaIh_HasEvenParity(uint8 ManagementNibble);
#endif

/**********************************************************************************************************************
 *  EaIh_SetCycleCounter()
 *********************************************************************************************************************/
/*!
 * \brief      Setter for cycle counter parameter of instance
 * \details    Setter for cycle counter parameter of instance
 * \param[in]  Instance
 * \param[in]  CycleCounter
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC (void, EA_PUBLIC_CODE) EaIh_SetCycleCounter(EaIh_InstanceVarPointerType Instance, uint8 CycleCounter);

/**********************************************************************************************************************
 *  EaIh_CheckHeaderTrailerForCycleCounter()
 *********************************************************************************************************************/
/*!
 * \brief      Checks both header and trailer and returns containing cycle counter value
 * \details    Checks both header and trailer and returns containing cycle counter value. If neither header nor trailer
 *             do contain a valid cycle counter, zero is returned. If both header and trailer contain
 *             different cycle counters, value of header is dominant.
 * \param[in]  Header
 * \param[in]  Trailer
 * \return     cycle counter contained in header or trailer
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC (uint8, EA_PUBLIC_CODE) EaIh_CheckHeaderTrailerForCycleCounter(uint8 Header, uint8 Trailer);

/**********************************************************************************************************************
 *  EaIh_CalcCycleCounterFromMgmt()
 *********************************************************************************************************************/
/*!
 * \brief      Returns cycle counter according to passed Mgmt bytes
 * \details    Returns cycle counter according to passed Mgmt bytes
 * \param[in]  Header
 * \param[in]  Trailer
 * \return     Cycle counter value
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC (uint8, EA_PUBLIC_CODE) EaIh_CalcCycleCounterFromMgmt(uint8 Header, uint8 Trailer);

/**********************************************************************************************************************
 *  EaIh_FillBufferWithEraseValue()
 *********************************************************************************************************************/
/*!
 * \brief      Fills temporary buffer with configured erase value
 * \details    Fills temporary buffer (write buffer) with EA_EEP_ERASE_VALUE
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaIh_FillBufferWithEraseValue(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EaIh_GetValidMgmtByteFromCycleCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(uint8, EA_PRIVATE_CODE) EaIh_GetValidMgmtByteFromCycleCounter(uint8 CycleCounter)
{
  return EaIh_ValidManagementValues[CycleCounter];
}

/**********************************************************************************************************************
 *  EaIh_GetInvalidMgmtByteFromCycleCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(uint8, EA_PRIVATE_CODE) EaIh_GetInvalidMgmtByteFromCycleCounter(uint8 CycleCounter)
{
  return EaIh_InvalidManagementValues[CycleCounter];
}

/**********************************************************************************************************************
 *  EaIh_GetTrailerAddress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(Ea_AddressType, EA_PRIVATE_CODE) EaIh_GetTrailerAddress(EaIh_InstanceConstPointerType Instance)
{
  return Instance->TrailerAddress;
}

/**********************************************************************************************************************
 *  EaIh_GetInstanceDataLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC (uint16, EA_PUBLIC_CODE) EaIh_GetInstanceDataLength(EaIh_InstanceConstPointerType Instance)
{
  return Instance->DataLength;
}

/**********************************************************************************************************************
 *  EaIh_GetCycleCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC (uint8, EA_PUBLIC_CODE) EaIh_GetCycleCounter(EaIh_InstanceConstPointerType Instance)
{
  return Instance->CycleCounter;
}

/**********************************************************************************************************************
 *  EaIh_IsValidManagement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(boolean, EA_PRIVATE_CODE) EaIh_IsValidManagement(uint8 ManagementByte)
{
  boolean retVal = FALSE;
  uint8 LoopCounter;
  uint16 LoopSize = sizeof(EaIh_ValidManagementValues) / sizeof(EaIh_ValidManagementValues[0]);

  for (LoopCounter = 0; LoopCounter < LoopSize; LoopCounter++)
  {
    /* #10 Check if ManagementByte is contained in ValidManagementValues array */
    if (ManagementByte == EaIh_ValidManagementValues[LoopCounter])
    {
      retVal = TRUE;
      break;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  EaIh_IsInvalidManagement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(boolean, EA_PRIVATE_CODE) EaIh_IsInvalidManagement(uint8 ManagementByte)
{
  boolean retVal = FALSE;
  uint8 LoopCounter;
  uint16 LoopSize = sizeof(EaIh_InvalidManagementValues) / sizeof(EaIh_InvalidManagementValues[0]);

  for (LoopCounter = 0; LoopCounter < LoopSize; LoopCounter++)
  {
    /* #10 Check if ManagementByte is contained in InvalidManagementValues array */
    if (ManagementByte == EaIh_InvalidManagementValues[LoopCounter])
    {
      retVal = TRUE;
      break;
    }
  }

  return retVal;
}

#if (EA_CORRECT_SINGLE_BIT_FLIPS == STD_ON)
/**********************************************************************************************************************
 *  EaIh_HasEvenParity()
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
EA_LOCAL FUNC(boolean, EA_PRIVATE_CODE) EaIh_HasEvenParity(uint8 ManagementNibble)
{
  boolean retVal;

  /* #10 All bits of an 8bit integer can be seen as: (a b c d e f g h) */

  /* #11 Only the lower nibble of ManagementNibble character is considered */
  ManagementNibble = ManagementNibble & 0x0Fu;
  /* #12 ManagementNibble = (0 0 0 0 e f g h) */

  ManagementNibble ^= (uint8) (ManagementNibble >> 2u);
  /* #13 ManagementNibble = (0 0 0 0 e f eg hf) - "eg" means the bit has the value "e XOR g" */

  ManagementNibble ^= (uint8) (ManagementNibble >> 1u);
  /* #14 ManagementNibble = (0 0 0 0 e ef efg efgh) */
  /* #15 All bits of the original word are XOR'd together and accumulated in the least-significant bit */

  /* #16 If the least-significant bit is zero, the original word "ManagementNibble" has even parity, */
  /* #17 otherwise odd parity */
  if ((ManagementNibble & 0x01u) == 0x00u)
  {
    retVal = TRUE;
  }
  else
  {
    retVal = FALSE;
  }

  return retVal;
}
#endif

/**********************************************************************************************************************
 *  EaIh_SetCycleCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC (void, EA_PUBLIC_CODE) EaIh_SetCycleCounter(EaIh_InstanceVarPointerType Instance, uint8 CycleCounter)
{
  Instance->CycleCounter = CycleCounter; /* SBSW_EA_21 */
}

/**********************************************************************************************************************
 *  EaIh_CheckHeaderTrailerForCycleCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC (uint8, EA_PUBLIC_CODE) EaIh_CheckHeaderTrailerForCycleCounter(uint8 Header, uint8 Trailer)
{
  uint8 cycleCounter;

  if (EaIh_IsValidOrInvalidManagement(Header, Trailer) == TRUE)
  {
    cycleCounter = EaIh_CalcCycleCounterFromMgmt(Header, Trailer);
  }
  else
  {
    cycleCounter = 0;
  }

  return cycleCounter;
}

/**********************************************************************************************************************
 *  EaIh_CalcCycleCounterFromMgmt()
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
EA_LOCAL FUNC (uint8, EA_PUBLIC_CODE) EaIh_CalcCycleCounterFromMgmt(uint8 Header, uint8 Trailer)
{
  /* #05 Service is only called if either one of the management bytes or both management bytes are either valid or invalid */
  /* #06 Service is never called if both header and trailer do not match with any valid or invalid values */

  /* #10 Check both header and trailer and get cycle counter */
  /* #11 Both header and trailer could be valid or invalid. On the other side it is possible that only one of them */
  /* #12 is valid or invalid */
  uint8 tempMgmt;

  if (EaIh_IsValidManagement(Header) == TRUE)
  {
    tempMgmt = Header;
  }
  else if (EaIh_IsInvalidManagement(Header) == TRUE)
  {
    tempMgmt = Header;
  }
  else if (EaIh_IsValidManagement(Trailer) == TRUE)
  {
    tempMgmt = Trailer;
  }
  /* #20 Trailer has to be invalid */
  else /* EaIh_IsInvalidManagement(Trailer) == TRUE */
  {
    tempMgmt = Trailer;
  }

  return tempMgmt & EA_CYCLE_COUNTER_BIT_MASK;
}

/**********************************************************************************************************************
 *  EaIh_FillBufferWithEraseValue()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(void, EA_PRIVATE_CODE) EaIh_FillBufferWithEraseValue(void)
{
  uint8 LoopCounter;

  for (LoopCounter = 0u; LoopCounter < EA_MAX_WRITE_ALIGNMENT; LoopCounter++)
  {
    EaIh_WriteBuffer[LoopCounter] = EaPh_GetEepEraseValue(); /* SBSW_EA_22 */
  }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EaIh_IsValidOrInvalidManagement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, EA_PRIVATE_CODE) EaIh_IsValidOrInvalidManagement(uint8 HeaderByte, uint8 TrailerByte)
{
  boolean retVal = FALSE;

  if (EaIh_IsValidManagement(TrailerByte) == TRUE)
  {
    retVal = TRUE;
  }

  if (EaIh_IsInvalidManagement(TrailerByte) == TRUE)
  {
    retVal = TRUE;
  }

  if (EaIh_IsValidManagement(HeaderByte) == TRUE)
  {
    retVal = TRUE;
  }

  if (EaIh_IsInvalidManagement(HeaderByte) == TRUE)
  {
    retVal = TRUE;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  EaIh_IsStatusOkOrInvalid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, EA_PRIVATE_CODE) EaIh_IsStatusOkOrInvalid(EaIh_InstanceConstPointerType Instance)
{
  EaIh_InstanceStatusType status = EaIh_GetInstanceStatus(Instance); /* SBSW_EA_203 */
  boolean retVal;

  if (status == INSTANCE_STATUS_OK)
  {
    retVal = TRUE;
  }
  else if (status == INSTANCE_STATUS_INVALID)
  {
    retVal = TRUE;
  }
  else
  {
    retVal = FALSE;
  }
  return retVal;
}

/**********************************************************************************************************************
 *  EaIh_CreateInstance()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (void, EA_PUBLIC_CODE) EaIh_CreateInstance(EaIh_InstanceVarPointerType Instance,
        Ea_AddressType InstanceStartAddress, uint16 DataLength, uint8 CycleCounter)
{
  Instance->HeaderAddress = InstanceStartAddress; /* SBSW_EA_21 */
  Instance->DataLength = DataLength; /* SBSW_EA_21 */
  Instance->InstanceLength = DataLength + (EA_MANAGEMENT_SIZE * 2); /* SBSW_EA_21 */
  Instance->TrailerAddress = InstanceStartAddress + DataLength + (EA_MANAGEMENT_SIZE); /* SBSW_EA_21 */
  Instance->CycleCounter = CycleCounter; /* SBSW_EA_21 */

  Instance->HeaderValue = EaPh_GetEepEraseValue(); /* SBSW_EA_21 */
  Instance->TrailerValue = EaPh_GetEepEraseValue(); /* SBSW_EA_21 */
  Instance->InstanceStatus = INSTANCE_STATUS_ERASED; /* SBSW_EA_21 */
}

/**********************************************************************************************************************
 *  EaIh_InitInstance()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (void, EA_PUBLIC_CODE) EaIh_InitInstance(EaIh_InstanceVarPointerType Instance)
{
  Instance->DataLength = 0x0000U; /* SBSW_EA_21 */
  Instance->HeaderAddress = 0x0000U; /* SBSW_EA_21 */
  Instance->TrailerAddress = 0x0000U; /* SBSW_EA_21 */
  Instance->HeaderValue = 0x00U; /* SBSW_EA_21 */
  Instance->TrailerValue = 0x00U; /* SBSW_EA_21 */
  Instance->CycleCounter = 0x00U; /* SBSW_EA_21 */
  Instance->InstanceStatus = INSTANCE_STATUS_ERASED; /* SBSW_EA_21 */
}

/**********************************************************************************************************************
 *  EaIh_GetInstanceStartAddress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Ea_AddressType, EA_PUBLIC_CODE) EaIh_GetInstanceStartAddress(EaIh_InstanceConstPointerType Instance)
{
  return Instance->HeaderAddress;
}

/**********************************************************************************************************************
 *  EaIh_GetTrailerAddressFromHeaderAddress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Ea_AddressType, EA_PRIVATE_CODE) EaIh_GetTrailerAddressFromHeaderAddress(EaIh_InstanceConstPointerType Instance,
        Ea_AddressType HeaderAddress)
{
  /* #10 HeaderAddress is passed to service because this is not Instance's current header address but */
  /* #11 header address of following instance */
  return HeaderAddress + EaIh_GetInstanceDataLength(Instance) + EA_MANAGEMENT_SIZE; /* SBSW_EA_203 */
}

/**********************************************************************************************************************
 *  EaIh_GetInstanceLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (uint16, EA_PUBLIC_CODE) EaIh_GetInstanceLength(EaIh_InstanceConstPointerType Instance)
{
  return Instance->InstanceLength;
}

/**********************************************************************************************************************
 *  EaIh_GetInstanceStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (EaIh_InstanceStatusType, EA_PUBLIC_CODE) EaIh_GetInstanceStatus(EaIh_InstanceConstPointerType Instance)
{
  return Instance->InstanceStatus;
}

/**********************************************************************************************************************
 *  EaIh_GetHeaderValue()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, EA_PRIVATE_CODE) EaIh_GetHeaderValue(EaIh_InstanceConstPointerType Instance)
{
  return Instance->HeaderValue;
}

/**********************************************************************************************************************
 *  EaIh_GetTrailerValue()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, EA_PRIVATE_CODE) EaIh_GetTrailerValue(EaIh_InstanceConstPointerType Instance)
{
  return Instance->TrailerValue;
}

/**********************************************************************************************************************
 *  EaIh_StoreHeaderValueFromBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PRIVATE_CODE) EaIh_StoreHeaderValueFromBuffer(EaIh_InstanceVarPointerType Instance)
{
  Instance->HeaderValue = EaIh_ReadBuffer[0u]; /* SBSW_EA_21 */
}

/**********************************************************************************************************************
 *  EaIh_StoreTrailerValueFromBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, EA_PRIVATE_CODE) EaIh_StoreTrailerValueFromBuffer(EaIh_InstanceVarPointerType Instance)
{
  Instance->TrailerValue = EaIh_ReadBuffer[EaPh_GetPartitionReadAlignment() - 1u]; /* SBSW_EA_21 */
}

/**********************************************************************************************************************
 *  EaIh_StoreHeaderAndTrailerValueFromBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, EA_PRIVATE_CODE) EaIh_StoreHeaderAndTrailerValueFromBuffer(EaIh_InstanceVarPointerType Instance)
{
  /* #10 Both header and trailer will be stored in instance object */
  EaIh_StoreHeaderValueFromBuffer(Instance); /* SBSW_EA_29 */
  Instance->TrailerValue = EaIh_ReadBuffer[EaIh_GetInstanceDataLength(Instance) + EA_MANAGEMENT_SIZE]; /* SBSW_EA_21 */ /* SBSW_EA_203 */
}

/**********************************************************************************************************************
 *  EaIh_SetInstanceStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (void, EA_PUBLIC_CODE) EaIh_SetInstanceStatus(EaIh_InstanceVarPointerType Instance, EaIh_InstanceStatusType status)
{
  Instance->InstanceStatus = status; /* SBSW_EA_21 */
}

/**********************************************************************************************************************
 *  EaIh_SetHeaderAndTrailerAddress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (void, EA_PUBLIC_CODE) EaIh_SetHeaderAndTrailerAddress(EaIh_InstanceVarPointerType Instance,
        Ea_AddressType HeaderAddress, Ea_AddressType TrailerAddress)
{
  Instance->HeaderAddress = HeaderAddress; /* SBSW_EA_21 */
  Instance->TrailerAddress = TrailerAddress; /* SBSW_EA_21 */
}

/**********************************************************************************************************************
 *  EaIh_SetManagementValuesAccordingToStatus()
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
FUNC (void, EA_PUBLIC_CODE) EaIh_SetManagementValuesAccordingToStatus(EaIh_InstanceVarPointerType Instance)
{
  /* #5 Service is only called after Instance's status was set */
  /* #6 Service is only called with Instance object whose status is either */
  /* #7 INSTANCE_STATUS_OK or INSTANCE_STATUS_INVALID */
  if (EaIh_GetInstanceStatus(Instance) == INSTANCE_STATUS_OK) /* SBSW_EA_203 */
  {
    /* #10 Management Bytes are set with valid values for a WRITE job */
    Instance->HeaderValue = EaIh_GetValidMgmtByteFromCycleCounter(EaIh_GetCycleCounter(Instance)); /* SBSW_EA_21 */ /* SBSW_EA_203 */
    Instance->TrailerValue = EaIh_GetHeaderValue(Instance); /* SBSW_EA_21 */ /* SBSW_EA_203 */
  }
  else /* #20 status == INSTANCE_STATUS_INVALID */
  {
    /* #21 Management Bytes are set with invalid values for an INVALIDATE job */
    Instance->HeaderValue = EaIh_GetInvalidMgmtByteFromCycleCounter(EaIh_GetCycleCounter(Instance)); /* SBSW_EA_21 */ /* SBSW_EA_203 */
    Instance->TrailerValue = EaIh_GetHeaderValue(Instance); /* SBSW_EA_21 */ /* SBSW_EA_203 */
  }
}

/**********************************************************************************************************************
 *  EaIh_SetNegativeJobResultAccordingToStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC (MemIf_JobResultType, EA_PUBLIC_CODE) EaIh_SetNegativeJobResultAccordingToStatus(EaIh_InstanceConstPointerType Instance)
{
  MemIf_JobResultType retVal;

  /* #20 Service is never called with positive instance status (INSTANCE_STATUS_OK) */
  switch (EaIh_GetInstanceStatus(Instance)) /* SBSW_EA_203 */
  {
    case INSTANCE_STATUS_INVALID:
      retVal = MEMIF_BLOCK_INVALID;
      break;
    case INSTANCE_STATUS_ERASED:
      retVal = MEMIF_BLOCK_INVALID;
      break;
    default:
      retVal = MEMIF_BLOCK_INCONSISTENT;
      break;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  EaIh_SetInstanceProperties()
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
FUNC (void, EA_PUBLIC_CODE) EaIh_SetInstanceProperties(EaIh_InstanceVarPointerType Instance)
{
  EaIh_InstanceStatusType status;
  uint8 cycleCounter;

  uint8 header = EaIh_GetHeaderValue(Instance); /* SBSW_EA_203 */
  uint8 trailer = EaIh_GetTrailerValue(Instance); /* SBSW_EA_203 */

  /* #10 Instance's status and cycle counter are set according to values of both header and trailer bytes */

  if (header == EaPh_GetEepEraseValue())
  {
    /* #20 For example:  |FF ... FF| or |FF ... 69| -> status = Erased*/
    status = INSTANCE_STATUS_ERASED;
    cycleCounter = 0u;
  }
  else if (header == trailer)
  {
    if (EaIh_IsValidManagement(header) == TRUE)
    {
      /* #30 For example:  |F0 ... F0| or |69 ... 69| -> status = OK*/
      status = INSTANCE_STATUS_OK;
      cycleCounter = EaIh_CalcCycleCounterFromMgmt(header, trailer);
    }
    else if (EaIh_IsInvalidManagement(header) == TRUE)
    {
      /* #40 For example:  |3C ... 3C| or |0F ... 0F| -> status = Invalid*/
      status = INSTANCE_STATUS_INVALID;
      cycleCounter = EaIh_CalcCycleCounterFromMgmt(header, trailer);
    }
    else
    {
      /* #50 For example:  |AA ... AA| or |33 ... 33| -> status = Inconsistent*/
      status = INSTANCE_STATUS_INCONSISTENT;
      cycleCounter = 0u;
    }
  }
  else
  {
    /* #60 For example:  |F0 ... AA| or |CC ... DD| -> status = Inconsistent*/
    status = INSTANCE_STATUS_INCONSISTENT;
    cycleCounter = EaIh_CheckHeaderTrailerForCycleCounter(header,trailer);
  }

  Instance->CycleCounter = cycleCounter; /* SBSW_EA_21 */
  Instance->InstanceStatus = status; /* SBSW_EA_21 */
}

/**********************************************************************************************************************
 *  EaIh_IncrementCycleCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC (void, EA_PUBLIC_CODE) EaIh_IncrementCycleCounter(EaIh_InstanceVarPointerType Instance)
{
  uint8 cycleCounter = EaIh_GetCycleCounter(Instance); /* SBSW_EA_203 */
  /* #10 CycleCounter is incremented, also considering a wrap around */
  /* #11 Possible CycleCounter values are: 0, 1, 2, 3 */
  cycleCounter = ( cycleCounter + 1u ) & EA_CYCLE_COUNTER_BIT_MASK;

  EaIh_SetCycleCounter(Instance, cycleCounter); /* SBSW_EA_29 */
}

#if (EA_CORRECT_SINGLE_BIT_FLIPS == STD_ON)
/**********************************************************************************************************************
 *  EaIh_CorrectSingleBitFlips()
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
FUNC (void, EA_PUBLIC_CODE) EaIh_CorrectSingleBitFlips(EaIh_BufferPointerType ManagementByte)
{
  uint8 correctNibble;
  uint8 LowerNibble = (*ManagementByte) & 0x0Fu;
  uint8 HigherNibbleInverted = ((   (uint8)( ~(*ManagementByte) )   ) >> 4u) & 0x0Fu;
  uint8 XorLowerHigherNibble = LowerNibble ^ HigherNibbleInverted;

  if (LowerNibble != HigherNibbleInverted)
  {
    /* #10 Lower Nibble and inverted higher nibble do not match, thus at least one single bit flip occurred */

    if ((XorLowerHigherNibble & (XorLowerHigherNibble - 1u)) == 0u)
    {
      /* #20 Difference of bit flips between LowerNibble and HigherNibble is one. Most likely exactly */
      /* #21 one bit flip occurred, but for example it is also possible that in one nibble two bit flips */
      /* #22 and in the other nibble one bit flip occurred. This case is ignored. */

      /* #23 In the next step, the nibble with valid (even) parity is determined, because it is assumed that */
      /* #24 this nibble does not contain any bit flips */
      if (EaIh_HasEvenParity(LowerNibble) == TRUE)
      {
        correctNibble = LowerNibble;
      }
      else
      {
        correctNibble = HigherNibbleInverted;
      }

      /* #25 Correct nibble was found. Depending on invalidation bit in nibble, either valid management byte or */
      /* #26 invalid management byte is retrieved */
      if ((correctNibble & 4u) == 0u)
      {
        /* #30 Invalidation bit is not set */
        *ManagementByte = EaIh_GetValidMgmtByteFromCycleCounter(correctNibble & EA_CYCLE_COUNTER_BIT_MASK); /* SBSW_EA_23 */
      }
      else
      {
        /* #31 Invalidation bit is set */
        *ManagementByte = EaIh_GetInvalidMgmtByteFromCycleCounter(correctNibble & EA_CYCLE_COUNTER_BIT_MASK); /* SBSW_EA_23 */
      }
    }
  }
}
#endif

/**********************************************************************************************************************
 *  EaIh_CompareTo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC (boolean, EA_PUBLIC_CODE) EaIh_CompareTo(EaIh_InstanceConstPointerType PrevInstance, EaIh_InstanceConstPointerType Instance)
{
  boolean retVal;

  if (EaIh_GetInstanceStatus(PrevInstance) == INSTANCE_STATUS_ERASED) /* SBSW_EA_203 */
  {
    /* #5 Previous instance was not yet updated, it's still initialized with erased value . Thus "Instance" is more up to date */
    retVal = TRUE;
  }
  else if (EaIh_GetCycleCounter(PrevInstance) == EaIh_GetCycleCounter(Instance)) /* SBSW_EA_203 */ /* SBSW_EA_203 */
  {
    /* #10 CycleCounters of both instances are equal, E.g. |F0 .. F0| |F0 .. F0| */
    retVal = TRUE;
  }
  else
  {
    /* #20 "PrevInstance" is more up to date, E.g.: |69 .. 69| |F0 .. F0| */
    retVal = FALSE;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  EaIh_ReadHeader()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_ReadHeader(EaIh_InstanceConstPointerType Instance)
{
  /* #10 First byte in read buffer will contain the header value */
  return Ea_Eep_Read(EaIh_GetInstanceStartAddress(Instance), EaIh_ReadBuffer, EaPh_GetPartitionReadAlignment()); /* SBSW_EA_29 */ /* SBSW_EA_201 */
}

/**********************************************************************************************************************
 *  EaIh_ReadTrailer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_ReadTrailer(EaIh_InstanceConstPointerType Instance)
{
  Ea_AddressType readAddress = (EaIh_GetTrailerAddress(Instance) -  EaPh_GetPartitionReadAlignment()) + 1u; /* SBSW_EA_203 */
  /* #10 Trailer value will be stored to position in read buffer, which matches read alignment */
  return Ea_Eep_Read(readAddress, EaIh_ReadBuffer, EaPh_GetPartitionReadAlignment()); /* SBSW_EA_201 */
}

/**********************************************************************************************************************
 *  EaIh_ReadHeaderAndTrailer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_ReadHeaderAndTrailer(EaIh_InstanceConstPointerType Instance)
{
  /* #10 Job length (Read Alignment) is at least as big as instance length, thus both header and trailer will be read */
  return Ea_Eep_Read(EaIh_GetInstanceStartAddress(Instance), EaIh_ReadBuffer, EaPh_GetPartitionReadAlignment()); /* SBSW_EA_29 */ /* SBSW_EA_201 */
}

/**********************************************************************************************************************
 *  EaIh_ReadData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_ReadData(EaIh_InstanceConstPointerType Instance,
                                                    EaIh_BufferPointerType DataBufferPtr,
                                                    uint16 BlockOffset,
                                                    uint16 ReadLength)
{
  /* #10 Only data contained in instance is read. No management bytes are read */
  Ea_AddressType readAddress = EaIh_GetInstanceStartAddress(Instance) + EA_MANAGEMENT_SIZE + BlockOffset; /* SBSW_EA_203 */
  return Ea_Eep_Read(readAddress, DataBufferPtr, ReadLength); /* SBSW_EA_202 */
}

/**********************************************************************************************************************
 *  EaIh_WriteHeader()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_WriteHeader(EaIh_InstanceConstPointerType Instance,
        Ea_UserJobParameterType UserJobParameter, EaIh_CompareParameterPointerType CompareParameter)
{
  uint16 LoopCounter;
  uint16 BufferIndex = 0;

  /* #10 Prepare write buffer to write header byte and some data bytes. */
  /* #11 Amount of data bytes depend on write alignment and EA_MANAGEMENT_SIZE */
  EaIh_FillBufferWithEraseValue();

  EaIh_WriteBuffer[0u] = EaIh_GetHeaderValue(Instance); /* SBSW_EA_24 */ /* SBSW_EA_203 */

  for (LoopCounter = EA_MANAGEMENT_SIZE; LoopCounter < EaPh_GetPartitionWriteAlignment(); LoopCounter++)
  {
    EaIh_WriteBuffer[LoopCounter] = UserJobParameter.DataBufferPtr[BufferIndex]; /* SBSW_EA_28 */
    BufferIndex++;
  }

  /* #20 Save write parameters for compare job */
  CompareParameter->EaIh_CompareAddress = EaIh_GetInstanceStartAddress(Instance); /* SBSW_EA_25 */ /* SBSW_EA_203 */
  CompareParameter->EaIh_CompareBufferPtr = EaIh_WriteBuffer; /* SBSW_EA_25 */
  CompareParameter->EaIh_CompareLength = EaPh_GetPartitionWriteAlignment(); /* SBSW_EA_25 */

  return Ea_Eep_Write(EaIh_GetInstanceStartAddress(Instance), EaIh_WriteBuffer, EaPh_GetPartitionWriteAlignment()); /* SBSW_EA_29 */ /* SBSW_EA_201 */
}

/**********************************************************************************************************************
 *  EaIh_WriteMiddlePart()
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
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_WriteMiddlePart(EaIh_InstanceConstPointerType Instance,
        Ea_UserJobParameterType UserJobParameter, EaIh_CompareParameterPointerType CompareParameter)
{
  /* #10 No internal write buffer is used for writing the middle part */
  Ea_AddressType middlePartStartAddress = EaIh_GetInstanceStartAddress(Instance) + EaPh_GetPartitionWriteAlignment(); /* SBSW_EA_203 */

  /* #20 BufferIndex starts from the position, IH_WriteHeader stopped before */
  uint16 BufferIndex = EaPh_GetPartitionWriteAlignment() - EA_MANAGEMENT_SIZE;

  /* #30 IH_WriteHeader and IH_WriteTrailer are writing two times the size of write alignment. */
  /* #31 Thus, length of middle part is rest of instance length */
  uint16 middlePartLength = Ea_AlignValue(EaIh_GetInstanceLength(Instance), EaPh_GetPartitionWriteAlignment()) -
          ( 2 * EaPh_GetPartitionWriteAlignment() ); /* SBSW_EA_203 */

  /* #40 Save write parameters for compare job */
  CompareParameter->EaIh_CompareAddress = middlePartStartAddress; /* SBSW_EA_25 */
  CompareParameter->EaIh_CompareBufferPtr = &(UserJobParameter.DataBufferPtr[BufferIndex]); /* SBSW_EA_25 */
  CompareParameter->EaIh_CompareLength = middlePartLength; /* SBSW_EA_25 */

  return Ea_Eep_Write(middlePartStartAddress, &(UserJobParameter.DataBufferPtr[BufferIndex]), middlePartLength ); /* SBSW_EA_202 */
}

/**********************************************************************************************************************
 *  EaIh_WriteTrailer()
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
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_WriteTrailer(EaIh_InstanceConstPointerType Instance,
        Ea_UserJobParameterType UserJobParameter, EaIh_CompareParameterPointerType CompareParameter)
{
  uint16 LoopCounter;
  uint16 BufferLength;
  /* #10 StartAddress is calculated from instance's start address, its length is aligned to write alignment and */
  /* #11 the amount of bytes which are written in the trailer part */
  Ea_AddressType trailerPartStartAddress = ( EaIh_GetInstanceStartAddress(Instance) +
          Ea_AlignValue(EaIh_GetInstanceLength(Instance), EaPh_GetPartitionWriteAlignment()) ) -
                  EaPh_GetPartitionWriteAlignment(); /* SBSW_EA_203 */ /* SBSW_EA_203 */

  /* #20 Entire write buffer is filled with ERASED_VALUE in order to prepare possible padding bytes */
  EaIh_FillBufferWithEraseValue();

  BufferLength = EaPh_GetPartitionWriteAlignment() - EA_MANAGEMENT_SIZE -
          (Ea_AlignValue(EaIh_GetInstanceLength(Instance), EaPh_GetPartitionWriteAlignment()) -
                  EaIh_GetInstanceLength(Instance)); /* SBSW_EA_203 */ /* SBSW_EA_203 */

  /* #30 Limit of for-loop is equal to the number of data bytes which are to be written in trailer part */
  for (LoopCounter = 0; LoopCounter < BufferLength; LoopCounter++)
  {
    EaIh_WriteBuffer[LoopCounter] = UserJobParameter.DataBufferPtr[
        (trailerPartStartAddress - EaIh_GetInstanceStartAddress(Instance) - EA_MANAGEMENT_SIZE)
        + LoopCounter]; /* SBSW_EA_28 */ /* SBSW_EA_203 */
  }
  EaIh_WriteBuffer[LoopCounter] = EaIh_GetTrailerValue(Instance); /* SBSW_EA_28 */ /* SBSW_EA_203 */

  /* #40 Save write parameters for compare job */
  CompareParameter->EaIh_CompareAddress = trailerPartStartAddress; /* SBSW_EA_25 */
  CompareParameter->EaIh_CompareBufferPtr = EaIh_WriteBuffer; /* SBSW_EA_25 */
  CompareParameter->EaIh_CompareLength = EaPh_GetPartitionWriteAlignment(); /* SBSW_EA_25 */

  return Ea_Eep_Write(trailerPartStartAddress, EaIh_WriteBuffer, EaPh_GetPartitionWriteAlignment()); /* SBSW_EA_201 */
}

/**********************************************************************************************************************
 *  EaIh_WriteEntireData()
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
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_WriteEntireData(EaIh_InstanceConstPointerType Instance,
        Ea_UserJobParameterType UserJobParameter, EaIh_CompareParameterPointerType CompareParameter)
{
  uint16 LoopCounter;
  /* #10 Clear write buffer */
  EaIh_FillBufferWithEraseValue();

  /* #20 Store Header value in write buffer */
  EaIh_WriteBuffer[0u] = EaIh_GetHeaderValue(Instance); /* SBSW_EA_24 */ /* SBSW_EA_203 */

  /* #30 Store Data in write buffer */
  for(LoopCounter = 0; LoopCounter < EaIh_GetInstanceDataLength(Instance); LoopCounter++) /* SBSW_EA_203 */
  {
    EaIh_WriteBuffer[LoopCounter + EA_MANAGEMENT_SIZE] = UserJobParameter.DataBufferPtr[LoopCounter]; /* SBSW_EA_26 */
  }
  /* #40 Store Trailer value in write buffer */
  EaIh_WriteBuffer[LoopCounter + EA_MANAGEMENT_SIZE] = EaIh_GetTrailerValue(Instance); /* SBSW_EA_26 */ /* SBSW_EA_203 */

  /* #50 Save write parameters for compare job */
  CompareParameter->EaIh_CompareAddress = EaIh_GetInstanceStartAddress(Instance); /* SBSW_EA_25 */ /* SBSW_EA_203 */
  CompareParameter->EaIh_CompareBufferPtr = EaIh_WriteBuffer; /* SBSW_EA_25 */
  CompareParameter->EaIh_CompareLength = EaPh_GetPartitionWriteAlignment(); /* SBSW_EA_25 */

  /* #60 Entire instance is written in one step. Job Length (WriteAlignment) is greater or equal than instance length */
  return Ea_Eep_Write(EaIh_GetInstanceStartAddress(Instance), EaIh_WriteBuffer, EaPh_GetPartitionWriteAlignment()); /* SBSW_EA_201 */ /* SBSW_EA_203 */
}

/**********************************************************************************************************************
 *  EaIh_VerifyData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_VerifyData(EaIh_CompareParameterType CompareParameter)
{
  return Ea_Eep_Compare(CompareParameter.EaIh_CompareAddress, CompareParameter.EaIh_CompareBufferPtr,
          CompareParameter.EaIh_CompareLength); /* SBSW_EA_202 */
}

/**********************************************************************************************************************
 *  EaIh_InvalidateHeader()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_InvalidateHeader(EaIh_InstanceConstPointerType Instance)
{
  /* #10 Clear write buffer */
  EaIh_FillBufferWithEraseValue();

  /* #20 Header byte is stored in first position of write buffer */
  EaIh_WriteBuffer[0u] = EaIh_GetHeaderValue(Instance); /* SBSW_EA_24 */ /* SBSW_EA_203 */

  return Ea_Eep_Write(EaIh_GetInstanceStartAddress(Instance), EaIh_WriteBuffer, EaPh_GetPartitionWriteAlignment()); /* SBSW_EA_201 */ /* SBSW_EA_203 */
}

/**********************************************************************************************************************
 *  EaIh_InvalidateTrailer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_InvalidateTrailer(EaIh_InstanceConstPointerType Instance)
{
  Ea_AddressType invalidationAddress = (EaIh_GetTrailerAddress(Instance) - EaPh_GetPartitionWriteAlignment()) + 1u ; /* SBSW_EA_203 */
  EaIh_FillBufferWithEraseValue();

  EaIh_WriteBuffer[EaPh_GetPartitionWriteAlignment() - 1u] = EaIh_GetTrailerValue(Instance); /* SBSW_EA_28 */ /* SBSW_EA_203 */

  /* #10 Write address is chosen in a way that the last byte of write buffer (contains Trailer value) */
  /* #11 aligns with TrailerAddress of instance */
  return Ea_Eep_Write(invalidationAddress, EaIh_WriteBuffer, EaPh_GetPartitionWriteAlignment()); /* SBSW_EA_201 */
}

/**********************************************************************************************************************
 *  EaIh_InvalidateEntireInstance()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) EaIh_InvalidateEntireInstance(EaIh_InstanceConstPointerType Instance)
{
  EaIh_FillBufferWithEraseValue();

  EaIh_WriteBuffer[0u] = EaIh_GetHeaderValue(Instance); /* SBSW_EA_24 */ /* SBSW_EA_203 */
  EaIh_WriteBuffer[EaIh_GetTrailerAddress(Instance) - EaIh_GetInstanceStartAddress(Instance)] =
          EaIh_GetTrailerValue(Instance); /* SBSW_EA_27 */ /* SBSW_EA_203 */ /* SBSW_EA_203 */ /* SBSW_EA_203 */

  /* #10 Entire instance is invalidated in one step. */
  /* #11 Job length (Write Alignment) is greater or equal than instance length */
  return Ea_Eep_Write(EaIh_GetInstanceStartAddress(Instance), EaIh_WriteBuffer, EaPh_GetPartitionWriteAlignment()); /* SBSW_EA_201 */ /* SBSW_EA_203 */
}

#define EA_STOP_SEC_CODE
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* SBSW_JUSTIFICATION_BEGIN
    \ID SBSW_EA_21
      \DESCRIPTION The function writes to the object member referenced by parameter Instance of structure type
                   EaIh_InstanceType.
      \COUNTERMEASURE \N The compiler performs type check and therefore assures that write access to member of
                         referenced parameter Instance is valid.
    \ID SBSW_EA_22
      \DESCRIPTION Array write access of parameter EaIh_WriteBuffer in for-loop.
      \COUNTERMEASURE \N Array write access is not critical because both the upper limit of the for-loop and the size
                         of the array parameter is determined by #define EA_MAX_WRITE_ALIGNMENT. The lower limit (0)
                         of the for-loop is legitimate as well.
    \ID SBSW_EA_23
      \DESCRIPTION The function EaIh_CorrectSingleBitFlips writes to the object referenced by parameter ManagementByte
      \COUNTERMEASURE \T TCASE24265
                         The test ensures that single bit flip correction of header and trailer bytes at valid
                         addresses is performed. Nevertheless the address of a structure (EaIh_InstanceType) member
                         is passed to function EaIh_CorrectSingleBitFlips upon call.
    \ID SBSW_EA_24
      \DESCRIPTION Array parameter EaIh_WriteBuffer is accessed at index 0
      \COUNTERMEASURE \N Index 0 is always valid for accessing an array.
    \ID SBSW_EA_25
      \DESCRIPTION The function writes to the object member referenced by parameter CompareParameter of structure type
                   EaIh_CompareParameterType.
      \COUNTERMEASURE \N The compiler performs type check and therefore assures that write access to member of
                         referenced parameter CompareParameter is valid.
    \ID SBSW_EA_26
      \DESCRIPTION Array EaIh_WriteBuffer of size EA_MAX_WRITE_ALIGNMENT is accessed with max index equal to Instance's
                   DataLength plus EA_MANAGEMENT_SIZE. The function EaIh_WriteEntireData is only called in function
                   EaWrInst_ProcessStateIdle if current Instance's length (Instance's DataLength plus two times
                   EA_MANAGEMENT_SIZE) is equal or smaller than current partition's write alignment.
                   Thus it is sufficient to proof that any configured write alignment is equal or smaller than
                   EA_MAX_WRITE_ALIGNMENT.
      \COUNTERMEASURE \M [CM_EA_1]
    \ID SBSW_EA_27
      \DESCRIPTION Array EaIh_WriteBuffer of size EA_MAX_WRITE_ALIGNMENT is accessed with index equal to the difference
                   of Instance's TrailerAddress and StartAddress, which is equal to Instance's DataLength plus
                   EA_MANAGEMENT_SIZE. The function EaIh_InvalidateEntireInstance is only called in function
                   EaInvInst_ProcessStateIdle if current Instance's length (Instance's DataLength plus two times
                   EA_MANAGEMENT_SIZE) is equal or smaller than current partition's write alignment.
                   Thus it is sufficient to proof that any configured write alignment is equal or smaller than
                   EA_MAX_WRITE_ALIGNMENT.
      \COUNTERMEASURE \M [CM_EA_1]
    \ID SBSW_EA_28
      \DESCRIPTION Array EaIh_WriteBuffer of size EA_MAX_WRITE_ALIGNMENT is accessed with max index equal to
                   any configured partition's write alignment.
      \COUNTERMEASURE \M [CM_EA_1]
    \ID SBSW_EA_29
      \DESCRIPTION Function is called with pointer to an instance object of type EaIh_InstanceVarPointerType
      \COUNTERMEASURE \N Caller ensures that pointer passed to service is valid
    \ID SBSW_EA_201
      \DESCRIPTION Address of first element of local array parameter is passed to a function
      \COUNTERMEASURE \N Reference to local parameter is valid
    \ID SBSW_EA_202
      \DESCRIPTION Function is called with pointer to a data buffer
      \COUNTERMEASURE \N Caller ensures that pointer passed to service is valid
    \ID SBSW_EA_203
      \DESCRIPTION Function is called with pointer to an instance object of type EaIh_InstanceConstPointerType or EaIh_InstanceVarPointerType
      \COUNTERMEASURE \N Instance parameter is passed to function via input parameter, thus caller ensures that parameter referenced
                         by pointer is valid.
SBSW_JUSTIFICATION_END */

/*
\CM CM_EA_1 Verify that size of all configured write alignments is equal or smaller than EA_MAX_WRITE_ALIGNMENT.
 */

