/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 Vector Informatik GmbH.                                                     All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  J1939Nm_NvM.h
 *       Module:  SAE J1939 Network Management (J1939Nm)
 *
 *  Description:  Header for the NvM interface of the SAE J1939 Network Management module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (J1939NM_NVM_H)
# define J1939NM_NVM_H


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "J1939Nm_Cfg.h"
# include "J1939Nm_Lcfg.h"
# include "J1939Nm_PBcfg.h"


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*============================================== J1939NM_START_SEC_CODE =============================================*/
# define J1939NM_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if (J1939NM_ADDRESSSTORAGEBLOCK == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_NvMInit_CurrentNodeAddresses
 *********************************************************************************************************************/
/*! \brief      Resets the current address table to the preferred addresses of the nodes.
 *
 *  \param      none
 *  \return     E_OK:           The table was successfully reset.
 *              E_NOT_OK:       The table reset failed.
 *  \pre        The module must not be in the initialized state.
 *  \context    May be called from interrupt or task level.
 *  \note       This function is called by NvM to initialize the table if ReadAll fails to restore saved values.
 **********************************************************************************************************************/
FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_NvMInit_CurrentNodeAddresses(void);
# endif

# if (J1939NM_NAMESTORAGEBLOCK == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_NvMInit_CurrentNodeNames
 *********************************************************************************************************************/
/*! \brief      Resets the current NAME table to the configured NAMEs of the nodes.
 *
 *  \param      none
 *  \return     E_OK:           The table was successfully reset.
 *              E_NOT_OK:       The table reset failed.
 *  \pre        The module must not be in the initialized state.
 *  \context    May be called from interrupt or task level.
 *  \note       This function is called by NvM to initialize the table if ReadAll fails to restore saved values.
 **********************************************************************************************************************/
FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_NvMInit_CurrentNodeNames(void);
# endif

# define J1939NM_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*============================================== J1939NM_STOP_SEC_CODE =============================================*/

#endif /* J1939NM_NVM_H */

/**********************************************************************************************************************
 *  END OF FILE: J1939Nm_NvM.h
 *********************************************************************************************************************/
