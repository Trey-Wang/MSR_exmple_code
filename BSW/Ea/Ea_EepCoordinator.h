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
 *        \file  Ea_EepCoordinator.h
 *        \brief  EepCoordinator provides EEP specific services to all EA sub-components
 *
 *      \details  EepCoordinator provides EEP interface to all EA sub-components. EepCoordinator is responsible for
 *                PollingMode and Callback notifications of EEPROM driver.
 *
 *********************************************************************************************************************/


#ifndef EA_EEP_COORDINATOR_H
# define EA_EEP_COORDINATOR_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "MemIf_Types.h"


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define EA_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
  Ea_Eep_Read()
**********************************************************************************************************************/
/*!
 * \brief      Reads Length bytes from Address in EEPROM to BufferPtr
 * \details    Reads Length bytes from Address in EEPROM to BufferPtr
 * \param[in]  Address
 * \param[in]  BufferPtr
 * \param[in]  Length
 * \return     E_OK: Read request was accepted by EEPROM driver
 * \return     E_NOT_OK: Read request was rejected by EEPROM driver
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) Ea_Eep_Read(Ea_AddressType Address,
        P2VAR(uint8, AUTOMATIC, EA_APPL_DATA) BufferPtr, uint16 Length);

/**********************************************************************************************************************
  Ea_Eep_Write()
**********************************************************************************************************************/
/*!
 * \brief      Writes Length bytes from Buffer pointed to by BufferPtr to EEPROM at Address
 * \details    Writes Length bytes from Buffer pointed to by BufferPtr to EEPROM at Address
 * \param[in]  Address
 * \param[in]  BufferPtr
 * \param[in]  Length
 * \return     E_OK: Write request was accepted by EEPROM driver
 * \return     E_NOT_OK: Write request was rejected by EEPROM driver
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) Ea_Eep_Write(Ea_AddressType Address,
        P2VAR(uint8, AUTOMATIC, EA_APPL_DATA) BufferPtr, uint16 Length);

/**********************************************************************************************************************
  Ea_Eep_Compare()
**********************************************************************************************************************/
/*!
 * \brief      Compares Length bytes from EEPROM at Address with Buffer pointed to by BufferPtr
 * \details    Compares Length bytes from EEPROM at Address with Buffer pointed to by BufferPtr
 * \param[in]  Address
 * \param[in]  BufferPtr
 * \param[in]  Length
 * \return     E_OK: Compare request was accepted by EEPROM driver
 * \return     E_NOT_OK: Compare request was rejected by EEPROM driver
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) Ea_Eep_Compare(Ea_AddressType Address,
        P2VAR(uint8, AUTOMATIC, EA_APPL_DATA) BufferPtr, uint16 Length);

/**********************************************************************************************************************
  Ea_Eep_Erase()
**********************************************************************************************************************/
/*!
 * \brief      Erases Length bytes in EEPROM at Address
 * \details    Erases Length bytes in EEPROM at Address
 * \param[in]  Address
 * \param[in]  Length
 * \return     E_OK: Erase request was accepted by EEPROM driver
 * \return     E_NOT_OK: Erase request was rejected by EEPROM driver
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous FALSE
 */
FUNC (Std_ReturnType, EA_PUBLIC_CODE) Ea_Eep_Erase(Ea_AddressType Address, uint16 Length);

/**********************************************************************************************************************
  Ea_EepSetJobResult()
**********************************************************************************************************************/
/*!
 * \brief      Sets job result of EEP module
 * \details    Sets job result of EEP module upon call of EA's callback functions by EEP driver
 * \param[in]  JobResult
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (void, EA_PUBLIC_CODE) Ea_Eep_SetJobResult(MemIf_JobResultType JobResult);

/**********************************************************************************************************************
  Ea_EepGetJobResult()
**********************************************************************************************************************/
/*!
 * \brief      Returns current job result of active EEP module
 * \details    Returns current job result of active EEP module. Service distinguishes between Polling Mode and
 *             callback service usage.
 * \return     Job result of EEP
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 * \trace DSGN-Ea22976
 */
FUNC (MemIf_JobResultType, EA_PUBLIC_CODE) Ea_Eep_GetJobResult(void);


/**********************************************************************************************************************
  Ea_Eep_GetStatus()
**********************************************************************************************************************/
/*!
 * \brief      Returns current status of active EEP module
 * \details    Returns current status of active EEP module.
 * \return     Status of EEP
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (MemIf_StatusType, EA_PUBLIC_CODE) Ea_Eep_GetStatus(void);

/**********************************************************************************************************************
  Ea_Eep_Cancel()
**********************************************************************************************************************/
/*!
 * \brief      Cancels running EEP job
 * \details    Cancels running EEP job
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
FUNC (void, EA_PUBLIC_CODE) Ea_Eep_Cancel(void);


# define EA_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* EA_EEP_COORDINATOR_H */

/**********************************************************************************************************************
 *  END OF FILE: Ea_EepCoordinator.h
 *********************************************************************************************************************/
