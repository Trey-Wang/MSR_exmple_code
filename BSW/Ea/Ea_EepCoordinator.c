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
 *  -------------------------------------------------------------------------------------------------------------------
 *        \file  Ea_EepCoordinator.c
 *        \brief  EepCoordinator provides EEP specific services to all EA sub-components
 *
 *      \details  EepCoordinator provides EEP interface to all EA sub-components. EepCoordinator is responsible for
 *                PollingMode and Callback notifications of EEPROM driver.
 *
 *********************************************************************************************************************/

#define EA_IMPLEMENTATION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Ea.h"
#include "Ea_EepCoordinator.h"
#include "Ea_PartitionHandler.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#ifndef EA_LOCAL /* COV_EA_COMPATIBILITY */
# define EA_LOCAL static
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#if (EA_EEP_POLLING_MODE == STD_OFF)

# define EA_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

EA_LOCAL VAR(MemIf_JobResultType, EA_APPL_DATA) Ea_Eep_GlobalJobResult; /* PRQA S 3218 */ /* MD_EA_8.7 */

# define EA_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#if (EA_EEP_POLLING_MODE == STD_ON)

# define EA_START_SEC_CODE
# include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
  Ea_Eep_GetJobResultFromEEPROM()
**********************************************************************************************************************/
/*!
 * \brief      Returns current job result of active EEP module
 * \details    Returns current job result of active EEP module by calling EEP's GetJobResult service.
 *             This service is used by Ea_JobEndNotification and by Ea_Eep_GetJobResult() in case of
 *             EEP Polling Mode enabled.
 * \return     Job result of EEP
 * \pre        Polling mode of EEP is enabled via pre-compile configuration
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
EA_LOCAL FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) Ea_Eep_GetJobResultFromEEPROM(void);

# define EA_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (EA_EEP_POLLING_MODE == STD_ON)

# define EA_START_SEC_CODE
# include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
  Ea_Eep_GetJobResultFromEEPROM()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
EA_LOCAL FUNC(MemIf_JobResultType, EA_PUBLIC_CODE) Ea_Eep_GetJobResultFromEEPROM(void)
{
  return EaPh_GetEepApiPtr()->GetJobResult(); /* SBSW_EA_31 */
}

# define EA_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define EA_START_SEC_CODE
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
  Ea_Eep_Read()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) Ea_Eep_Read(Ea_AddressType Address,
        P2VAR(uint8, AUTOMATIC, EA_APPL_DATA) BufferPtr, uint16 Length)
{
  return EaPh_GetEepApiPtr()->Read(Address, BufferPtr, Length); /* SBSW_EA_31 */
}

/**********************************************************************************************************************
  Ea_Eep_Write()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) Ea_Eep_Write(Ea_AddressType Address,
        P2VAR(uint8, AUTOMATIC, EA_APPL_DATA) BufferPtr, uint16 Length)
{
  return EaPh_GetEepApiPtr()->Write(Address, BufferPtr, Length); /* SBSW_EA_31 */
}

/**********************************************************************************************************************
  Ea_Eep_Compare()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) Ea_Eep_Compare(Ea_AddressType Address,
        P2VAR(uint8, AUTOMATIC, EA_APPL_DATA) BufferPtr, uint16 Length)
{
  return EaPh_GetEepApiPtr()->Compare(Address, BufferPtr, Length); /* SBSW_EA_31 */
}

/**********************************************************************************************************************
  Ea_Eep_Erase()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) Ea_Eep_Erase(Ea_AddressType Address, uint16 Length)
{
  return EaPh_GetEepApiPtr()->Erase(Address, Length); /* SBSW_EA_31 */
}

#if (EA_EEP_POLLING_MODE == STD_OFF)
/**********************************************************************************************************************
  Ea_Eep_SetJobResult()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (void, EA_PUBLIC_CODE) Ea_Eep_SetJobResult(MemIf_JobResultType JobResult)
{
  Ea_Eep_GlobalJobResult = JobResult;
}
#endif

/**********************************************************************************************************************
  Ea_Eep_GetJobResult()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC (MemIf_JobResultType, EA_PUBLIC_CODE) Ea_Eep_GetJobResult(void)
{
#if (EA_EEP_POLLING_MODE == STD_ON)
  /* #10 EA module polls EEP module */
  return Ea_Eep_GetJobResultFromEEPROM();

#else
  /* #20 EEP module reports job result to EA via callback services */
  return Ea_Eep_GlobalJobResult;

#endif
}

/**********************************************************************************************************************
  Ea_Eep_GetStatus()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (MemIf_StatusType, EA_PUBLIC_CODE) Ea_Eep_GetStatus(void)
{
  return EaPh_GetEepApiPtr()->GetStatus(); /* SBSW_EA_31 */
}

/**********************************************************************************************************************
  Ea_Eep_Cancel()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC (void, EA_PUBLIC_CODE) Ea_Eep_Cancel(void)
{
  EaPh_GetEepApiPtr()->Cancel(); /* SBSW_EA_31 */
}

#define EA_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* SBSW_JUSTIFICATION_BEGIN
    \ID SBSW_EA_31
      \DESCRIPTION Function pointer from function pointer structure is called
      \COUNTERMEASURE \N  The compiler performs type check and therefore assures that valid function pointer is called.
                          Reference of pointer structure is retrieved by array access in Ea_PartitionHandler.c.
                          Used index EaPh_PartitionIndex is never out of bounds, because it is initialized with
                          maximum value EA_NUMBER_OF_PARTITIONS - 1, which also initializes the size of accessed
                          array Ea_PartitionConfigList.
SBSW_JUSTIFICATION_END */
