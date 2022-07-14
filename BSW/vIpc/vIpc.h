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
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  
 *        \brief  VIPC Template header file
 *
 *      \details 
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Patrick Junger                vispju        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  00.00.00  2017-05-19  vispju                Create boilerplate module vIpc
 *  00.01.00  2017-06-12  vispju  STORYC-1404   Usecase: Last-is-best with unsegmented PDUs
 *  00.02.00  2017-07-10  vispju  STORYC-1453   Usecase: Queued transmissions with unsegmented PDUs and equal priority
 *  00.03.00  2017-08-09  vispju  STORYC-1454   Usecase: Queued transmissions with un-/segmented prioritized PDUs
 *********************************************************************************************************************/

#ifndef VIPC_H
# define VIPC_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vIpc_Cbk.h"
#include "vIpc_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
#define VIPC_VENDOR_ID (30u)
#define VIPC_MODULE_ID (uint16)(0xFFu)

/* AUTOSAR Software specification version information */
#define VIPC_AR_RELEASE_MAJOR_VERSION (4u)
#define VIPC_AR_RELEASE_MINOR_VERSION (3u)
#define VIPC_AR_RELEASE_REVISION_VERSION (0u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
#define VIPC_SW_MAJOR_VERSION (0u)
#define VIPC_SW_MINOR_VERSION (3u)
#define VIPC_SW_PATCH_VERSION (0u)


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define VIPC_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define VIPC_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define VIPC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  vIpc_Transmit()
 *********************************************************************************************************************/
/*! \brief       Receives and enqueues transmission requests.
 *  \details     If the Tx channel is idle, it forwards the transmission request to the lower layer.
 *  \param[in]   TxSduId                 Id of the L-SDU for which a transmit is wanted.
 *  \param[in]   PduInfoPtr              Data to transmit including length.
 *  \return      E_OK                    Transmission request accepted.
 *  \return      E_NOT_OK                Transmission request failed.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         -
 *  \ingroup     tx
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, VIPC_CODE) vIpc_Transmit(PduIdType TxSduId,
                                                     P2CONST(PduInfoType, AUTOMATIC, VIPC_APPL_CONST) PduInfoPtr);

/**********************************************************************************************************************
 *  vIpc_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Function for *_INIT_*-variable initialization
 *  \details     Service to initialize module global variables at power up. This function initializes the
 *               variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         Module is uninitialized.
 *  \ingroup     general
 *********************************************************************************************************************/
extern FUNC(void, VIPC_CODE) vIpc_InitMemory(void);

/**********************************************************************************************************************
 * vIpc_Init()
 *********************************************************************************************************************/
/*! \brief       Initializes the vIpc.
 *  \details     This function initializes the module vIpc. It initializes all variables and sets the module state to
 *               initialized.
 *  \param[in]   ConfigPtr               Configuration structure for initializing the module
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         -
 *  \ingroup     general
 *********************************************************************************************************************/
extern FUNC(void, VIPC_CODE) vIpc_Init(P2CONST(vIpc_ConfigType, AUTOMATIC, VIPC_PBCFG) ConfigPtr);

/**********************************************************************************************************************
 *  vIpc_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     vIpc_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  VersionInfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \pre         -
 *  \ingroup     general
 *********************************************************************************************************************/
extern FUNC(void, VIPC_CODE) vIpc_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, VIPC_APPL_VAR) VersionInfo);

/**********************************************************************************************************************
  vIpc_MainFunction
**********************************************************************************************************************/
/*!
 *
 * \brief        Monitors and controls the continuous execution of the vIpc Transmission & Reception.
 * \details      -
 * \pre          -
 * \context      TASK|ISR
 * \reentrant    FALSE
 * \synchronous  TRUE
 * \ingroup      general
 *********************************************************************************************************************/
/*extern FUNC(void, VIPC_CODE) vIpc_MainFunction(void);*/


#define VIPC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#endif /* VIPC_H */

/**********************************************************************************************************************
 *  END OF FILE: vIpc.h
 *********************************************************************************************************************/
