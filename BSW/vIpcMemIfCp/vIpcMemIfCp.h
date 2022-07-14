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
 *        \brief  vIpcMemIfCp header file
 *
 *      \details  Contains the declaration of the module's API functions.
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Ivan Begert                   virbiv        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2017-07-26  virbiv  -             Initial version
 *********************************************************************************************************************/

#if !defined (VIPCMEMIFCP_H)                                                                                            /* PRQA S 0883 */ /* MD_vIpcMemIfCp_0883 */
# define VIPCMEMIFCP_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "vIpcMemIfCp_PBcfg.h"
# include "vIpcMemIfCp_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define VIPCMEMIFCP_VENDOR_ID   (30u)
# define VIPCMEMIFCP_MODULE_ID   (255u)
# define VIPCMEMIFCP_INSTANCE_ID (0u)

/* AUTOSAR Software specification version information */
# define VIPCMEMIFCP_AR_RELEASE_MAJOR_VERSION            (4u)
# define VIPCMEMIFCP_AR_RELEASE_MINOR_VERSION            (3u)
# define VIPCMEMIFCP_AR_RELEASE_REVISION_VERSION         (0u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define VIPCMEMIFCP_SW_MAJOR_VERSION                    (1u)
# define VIPCMEMIFCP_SW_MINOR_VERSION                    (0u)
# define VIPCMEMIFCP_SW_PATCH_VERSION                    (0u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# define VIPCMEMIFCP_SID_INIT                (vIpcMemIfCp_ServiceIdType)(0u)    /*!< Service ID: vIpcMemIfCp_Init() */
# define VIPCMEMIFCP_SID_INIT_MEMORY         (vIpcMemIfCp_ServiceIdType)(1u)    /*!< Service ID: vIpcMemIfCp_InitMemory() */
# define VIPCMEMIFCP_SID_GET_VERSION_INFO    (vIpcMemIfCp_ServiceIdType)(2u)    /*!< Service ID: vIpcMemIfCp_GetVersionInfo() */
# define VIPCMEMIFCP_SID_TRANSMIT            (vIpcMemIfCp_ServiceIdType)(3u)    /*!< Service ID: vIpcMemIfCp_Transmit() */
# define VIPCMEMIFCP_SID_RECEIVE             (vIpcMemIfCp_ServiceIdType)(4u)    /*!< Service ID: vIpcMemIfCp_Receive() */

# define VIPCMEMIFCP_E_NO_ERROR              (vIpcMemIfCp_ErrorCodeType)(0xFFu) /*!< No error. */
# define VIPCMEMIFCP_E_INV_PTR               (vIpcMemIfCp_ErrorCodeType)(1u)    /*!< Error code: Invalid pointer. */
# define VIPCMEMIFCP_E_PARAM                 (vIpcMemIfCp_ErrorCodeType)(2u)    /*!< Error code: Invalid parameter. */
# define VIPCMEMIFCP_E_UNINIT                (vIpcMemIfCp_ErrorCodeType)(3u)    /*!< Error code: API service used without module initialization. */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VIPCMEMIFCP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  vIpcMemIfCp_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Function for *_INIT_*-variable initialization
 *  \details     Service to initialize module global variables at power up. This function initializes the
 *               variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \pre         Module is uninitialized.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-135194
 *********************************************************************************************************************/
FUNC(void, VIPCMEMIFCP_CODE) vIpcMemIfCp_InitMemory(void);


/**********************************************************************************************************************
 * vIpcMemIfCp_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization function
 *  \details     This function initializes the module vIpcMemIfCp. It initializes all intern variables.
 *  \param[in]   ConfigPtr                 Pointer to the PostBuildLoadable configuration
 *  \pre         Module is uninitialized.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Specification of module initialization
 *  \trace       CREQ-135194
 *********************************************************************************************************************/
FUNC(void, VIPCMEMIFCP_CODE) vIpcMemIfCp_Init(P2CONST(vIpcMemIfCp_ConfigType, AUTOMATIC, VIPCMEMIFCP_INIT_DATA) ConfigPtr);


#if (VIPCMEMIFCP_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  vIpcMemIfCp_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     This function returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  VersionInfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \config      VIPCMEMIFCP_VERSION_INFO_API
 *  \synchronous TRUE
 *  \trace       CREQ-135193
 *********************************************************************************************************************/
FUNC(void, VIPCMEMIFCP_CODE) vIpcMemIfCp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, VIPCMEMIFCP_VAR_NOINIT) VersionInfo);
#endif /* (VIPCMEMIFCP_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 * vIpcMemIfCp_Transmit()
 *********************************************************************************************************************/
/*! \brief       Requests a new transmission on the channel.
 *  \details     If the channel is idle, the transmission is accepted and processed.
 *  \param[in]   ChannelId                 Id to identify the channel.
 *  \param[in]   PduInfoPtr                Pdu description.
 *  \return      E_OK                      Transmission request accepted.
 *  \return      E_NOT_OK                  Transmission request failed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels.
 *  \synchronous TRUE
 *  \trace       CREQ-135195
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VIPCMEMIFCP_CODE) vIpcMemIfCp_Transmit(PduIdType ChannelId, P2CONST(PduInfoType, AUTOMATIC, VIPCMEMIFCP_CONST) PduInfoPtr);


/**********************************************************************************************************************
 *  vIpcMemIfCp_Receive()
 *********************************************************************************************************************/
/*! \brief       Triggers processing of a specific channel.
 *  \details     -
 *  \param[in]   ChannelId                 Id to identify the channel.
 *  \return      E_OK                      Processing request accepted.
 *  \return      E_NOT_OK                  Processing request failed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels.
 *  \synchronous TRUE
 *  \trace       CREQ-135201
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VIPCMEMIFCP_CODE) vIpcMemIfCp_Receive(PduIdType ChannelId);


/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_MainFunction()
 *********************************************************************************************************************/
/*! \brief       Controls the continuous processing of all transmission channels.
 *  \details     -
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-135196
 *********************************************************************************************************************/
FUNC(void, VIPCMEMIFCP_CODE) vIpcMemIfCp_Tx_MainFunction(void);


/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_MainFunction()
 *********************************************************************************************************************/
/*! \brief       Controls the continuous processing of all reception channels.
 *  \details     -
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-135202
 *********************************************************************************************************************/
FUNC(void, VIPCMEMIFCP_CODE) vIpcMemIfCp_Rx_MainFunction(void);


# define VIPCMEMIFCP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */


/*!
 * \exclusivearea VIPCMEMIFCP_EXCLUSIVE_AREA_0
 * Ensures consistency of internal data while processing a single channel,
 * i.e. if the vIpcMemIfCp_Receive is called while vIpcMemIfCp_Rx_MainFunction is active.
 * Entering/Leaving is wrapped by vIpcMemIfCp_EnterExclusiveArea, vIpcMemIfCp_ExitExclusiveArea.
 *
 * \protects The internal channel state used during processing.
 *
 * \usedin ALL functions called during reception processing.
 * \exclude N/A
 * \length LONG The exclusive area covers the whole channel processing of reception.
 * \endexclusivearea
 */


#endif /* VIPCMEMIFCP_H */

/**********************************************************************************************************************
 *  END OF FILE: vIpcMemIfCp.h
 *********************************************************************************************************************/
