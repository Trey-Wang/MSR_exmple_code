/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  CddDrm_Cbk.h
 *        \brief  CddDrm Callback functions header file
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#ifndef CDDDRM_CBK_H
# define CDDDRM_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "CddDrm_Cfg.h"
# if (CDDDRM_CFG_SUPPORT_NVRAM == STD_ON)
#  include "NvM.h"
# endif
/* ********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CDDDRM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if ((CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON) && (CDDDRM_CFG_SUPPORT_NVRAM == STD_ON))
/**********************************************************************************************************************
 CddDrm_NvM_JobFinished
 **********************************************************************************************************************/
/*! \brief         Callback routine to notify the upper layer that an asynchronous single block request has been finished.
 *  \details       This function has to be called by the NvM after a write operation has finished.
 *                 (See NvMBlockDescriptor/NvMSingleBlockCallback)
 *  \param[in]     ServiceId    The service identifier of the completed request.
 *  \param[in]     JobResult    Result of the single block job.
 *  \return        E_OK:        Always returned.
 *  \return        E_NOT_OK:    Never returned.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          The function is called by the NvM.
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_NvM_JobFinished(uint8 ServiceId, NvM_RequestResultType JobResult);
# endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_NvM_InitEcuDetectionData()
 *********************************************************************************************************************/
/*! \brief         Initializes RAM for ECU Detection
 *  \details       This function is supposed to be called by the NvM in order to (re)initialize the data in case the 
 *                 non-volatile memory has never been stored, or was corrupted.
 *                 (See NvMBlockDescriptor/NvMInitBlockCallback)
 *                 It can also be used to force a reinitialization of the ECU detection data triggered by the 
 *                 application (e.g. after a new software version has been flashed to the ECU). In the latter case, 
 *                 make sure the function is not called while the Drm is active.
 *  \return        E_OK:       always returned
 *  \return        E_NOT_OK:   only in case of DET
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \pre           -
 *  \context       TASK|ISR2
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_NvM_InitEcuDetectionData(void);
#endif

/**********************************************************************************************************************
 *  CddDrm_ExternalTesterConnected()
 *********************************************************************************************************************/
/*! \brief         Close all open connections.
 *  \details       Force close of all open connections and prevent that new connections can be opened
 *  \param[in]     Present        sets the present state of an external tester
 *  \return        E_OK:          always returned
 *  \return        E_NOT_OK:      only if DET is enabled and an error is detected
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \pre           Module is initialized.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_ExternalTesterConnected(boolean Present);

#define CDDDRM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* CDDDRM_CBK_H */
/* ********************************************************************************************************************
 *  END OF FILE: CddDrm_Cbk.h
 *********************************************************************************************************************/
