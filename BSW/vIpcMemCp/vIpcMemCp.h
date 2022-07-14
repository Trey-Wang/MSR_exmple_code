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
 *        \brief  vIpcMemCp header file
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
 *  01.00.00  2017-05-18  virbiv  -             Initial version.
 *  01.01.00  2017-08-04  virbiv  -             Tx_BeginTransmit, Rx_BeginReceive: return value condition reworked.
 *                                              Tx_Confirm, Rx_Confirm: DataSize parameter removed.
 *********************************************************************************************************************/
#if !defined (VIPCMEMCP_H)                                                                                              /* PRQA S 0883 */ /* MD_vIpcMemCp_0883 */
# define VIPCMEMCP_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "vIpcMemCp_PBcfg.h"
# include "vIpcMemCp_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define VIPCMEMCP_VENDOR_ID   (30u)
# define VIPCMEMCP_MODULE_ID   (255u)
# define VIPCMEMCP_INSTANCE_ID (0u)

/* AUTOSAR Software specification version information */
# define VIPCMEMCP_AR_RELEASE_MAJOR_VERSION            (4u)
# define VIPCMEMCP_AR_RELEASE_MINOR_VERSION            (3u)
# define VIPCMEMCP_AR_RELEASE_REVISION_VERSION         (0u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define VIPCMEMCP_SW_MAJOR_VERSION                    (1u)
# define VIPCMEMCP_SW_MINOR_VERSION                    (1u)
# define VIPCMEMCP_SW_PATCH_VERSION                    (0u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# define VIPCMEMCP_SID_INIT                (vIpcMemCp_ServiceIdType)(0u)    /*!< Service ID: vIpcMemCp_Init() */
# define VIPCMEMCP_SID_INIT_MEMORY         (vIpcMemCp_ServiceIdType)(1u)    /*!< Service ID: vIpcMemCp_InitMemory() */
# define VIPCMEMCP_SID_GET_VERSION_INFO    (vIpcMemCp_ServiceIdType)(2u)    /*!< Service ID: vIpcMemCp_GetVersionInfo() */
# define VIPCMEMCP_SID_TX_BEGINTRANSMIT    (vIpcMemCp_ServiceIdType)(3u)    /*!< Service ID: vIpcMemCp_Tx_BeginTransmit() */
# define VIPCMEMCP_SID_TX_GETBUFFER        (vIpcMemCp_ServiceIdType)(4u)    /*!< Service ID: vIpcMemCp_Tx_GetBuffer() */
# define VIPCMEMCP_SID_TX_CONFIRM          (vIpcMemCp_ServiceIdType)(5u)    /*!< Service ID: vIpcMemCp_Tx_Confirm() */
# define VIPCMEMCP_SID_TX_CANCEL           (vIpcMemCp_ServiceIdType)(6u)    /*!< Service ID: vIpcMemCp_Tx_Cancel() */
# define VIPCMEMCP_SID_RX_BEGINRECEIVE     (vIpcMemCp_ServiceIdType)(7u)    /*!< Service ID: vIpcMemCp_Rx_BeginReceive() */
# define VIPCMEMCP_SID_RX_GETBUFFER        (vIpcMemCp_ServiceIdType)(8u)    /*!< Service ID: vIpcMemCp_Rx_GetBuffer() */
# define VIPCMEMCP_SID_RX_CONFIRM          (vIpcMemCp_ServiceIdType)(9u)    /*!< Service ID: vIpcMemCp_Rx_Confirm() */
# define VIPCMEMCP_SID_RX_CANCEL           (vIpcMemCp_ServiceIdType)(10u)   /*!< Service ID: vIpcMemCp_Rx_Cancel() */

# define VIPCMEMCP_E_NO_ERROR              (vIpcMemCp_ErrorCodeType)(0xFFu) /*!< No error. */
# define VIPCMEMCP_E_INV_PTR               (vIpcMemCp_ErrorCodeType)(1u)    /*!< Error code: Invalid pointer. */
# define VIPCMEMCP_E_PARAM                 (vIpcMemCp_ErrorCodeType)(2u)    /*!< Error code: Invalid parameter. */
# define VIPCMEMCP_E_UNINIT                (vIpcMemCp_ErrorCodeType)(3u)    /*!< Error code: API service used without module initialization. */
# define VIPCMEMCP_E_INCONSISTENT_SIZE     (vIpcMemCp_ErrorCodeType)(4u)    /*!< Error code: Size is inconsistent with previously requested. */
# define VIPCMEMCP_E_TX_ALREADY_PENDING    (vIpcMemCp_ErrorCodeType)(5u)    /*!< Error code: Transmit is not possible due to another one still pending. */
# define VIPCMEMCP_E_TX_NOT_PENDING        (vIpcMemCp_ErrorCodeType)(6u)    /*!< Error code: API service is used before transmit was started. */
# define VIPCMEMCP_E_RX_ALREADY_PENDING    (vIpcMemCp_ErrorCodeType)(7u)    /*!< Error code: Receive is not possible due to another one still pending. */
# define VIPCMEMCP_E_RX_NOT_PENDING        (vIpcMemCp_ErrorCodeType)(8u)    /*!< Error code: API service is used before receive was started. */


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

# define VIPCMEMCP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */



/**********************************************************************************************************************
 *  vIpcMemCp_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Function for *_INIT_*-variable initialization
 *  \details     Service to initialize module global variables at power up. This function initializes the
 *               variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \pre         Module is uninitialized.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-129713
 *********************************************************************************************************************/
FUNC(void, VIPCMEMCP_CODE) vIpcMemCp_InitMemory(void);


/**********************************************************************************************************************
 * vIpcMemCp_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization function
 *  \details     This function initializes the module vIpcMemCp. It initializes all intern variables.
 *  \param[in]   ConfigPtr                 Pointer to the PostBuildLoadable configuration
 *  \pre         Module is uninitialized.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Specification of module initialization
 *  \trace       CREQ-129713
 *********************************************************************************************************************/
FUNC(void, VIPCMEMCP_CODE) vIpcMemCp_Init(P2CONST(vIpcMemCp_ConfigType, AUTOMATIC, VIPCMEMCP_INIT_DATA) ConfigPtr);


#if (VIPCMEMCP_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  vIpcMemCp_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     This function returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  VersionInfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \config      VIPCMEMCP_VERSION_INFO_API
 *  \synchronous TRUE
 *  \trace       CREQ-129973
 *********************************************************************************************************************/
FUNC(void, VIPCMEMCP_CODE) vIpcMemCp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) VersionInfo);
#endif /* (VIPCMEMCP_VERSION_INFO_API == STD_ON) */


/**********************************************************************************************************************
 * vIpcMemCp_Tx_BeginTransmit()
 *********************************************************************************************************************/
/*! \brief       Function to initiate a sending transmission.
 *  \details     This functions checks if sufficient free memory is available and prepares the transmission.
 *  \param[in]   ChannelNr               The channel index to identify the used channel.
 *  \param[in]   DataSize                Length of the user's data in bytes.
 *  \return      E_OK                    Transmission request accepted.
 *  \return      E_NOT_OK                Transmission request failed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \trace       CREQ-129714
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_Tx_BeginTransmit
(
   vIpcMemCp_ChannelIndexType ChannelNr,
   uint32 DataSize
);


/**********************************************************************************************************************
 * vIpcMemCp_Tx_GetBuffer()
 *********************************************************************************************************************/
/*! \brief       Function to retrieve a buffer description of a single continuous data block.
 *  \details     The resulted buffer may be smaller than requested data size. In this case a further call of the
 *               same function is expected.
 *  \param[in]   ChannelNr               The channel index to identify the used channel.
 *  \param[in]   DataSize                Length of the user's data in bytes.
 *  \param[out]  BufferDesc              Buffer description to be used for data copy.
 *  \return      E_OK                    Buffer request was successful.
 *  \return      E_NOT_OK                Buffer request has failed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \trace       CREQ-133054
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_Tx_GetBuffer
(
   vIpcMemCp_ChannelIndexType ChannelNr,
   uint32 DataSize,
   P2VAR(vIpcMemCp_BufferDescriptionType, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) BufferDesc
);


/**********************************************************************************************************************
 * vIpcMemCp_Tx_Confirm()
 *********************************************************************************************************************/
/*! \brief       Function marks the active transmission as finished.
 *  \details     After the call the message will be ready to be read by the receiver.
 *  \param[in]   ChannelNr               The channel index to identify the used channel.
 *  \return      E_OK                    Confirmation was successful.
 *  \return      E_NOT_OK                Confirmation has failed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \trace       CREQ-133055
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_Tx_Confirm
(
   vIpcMemCp_ChannelIndexType ChannelNr
);


/**********************************************************************************************************************
 * vIpcMemCp_Tx_Cancel()
 *********************************************************************************************************************/
/*! \brief       Function cancels the currently active transmission.
 *  \details     Previously copied data is discarded.
 *  \param[in]   ChannelNr               The channel index to identify the used channel.
 *  \return      E_OK                    Cancel was successful.
 *  \return      E_NOT_OK                Cancel has failed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \trace       CREQ-133056
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_Tx_Cancel
(
   vIpcMemCp_ChannelIndexType ChannelNr
);

/**********************************************************************************************************************
 * vIpcMemCp_Rx_BeginReceive()
 *********************************************************************************************************************/
/*! \brief       Function to initiate a reception.
 *  \details     This functions checks if messages are available for reception.
 *  \param[in]   ChannelNr               The channel index to identify the used channel.
 *  \param[out]  DataSize                Length of the user's data in bytes.
 *  \return      E_OK                    Reception request accepted.
 *  \return      E_NOT_OK                Reception request failed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \trace       CREQ-129715
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_Rx_BeginReceive
(
   vIpcMemCp_ChannelIndexType ChannelNr,
   P2VAR(uint32, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) DataSize
);


/**********************************************************************************************************************
 * vIpcMemCp_Rx_GetBuffer()
 *********************************************************************************************************************/
/*! \brief       Function to retrieve a buffer description of a single continuous data block.
 *  \details     The resulted buffer may be smaller than requested data size. In this case a further call of the
 *               same function is expected.
 *  \param[in]   ChannelNr               The channel index to identify the used channel.
 *  \param[in]   DataSize                Length of the user's data in bytes.
 *  \param[out]  BufferDesc              Buffer description to be used for data copy.
 *  \return      E_OK                    Buffer request was successful.
 *  \return      E_NOT_OK                Buffer request has failed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \trace       CREQ-129716
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_Rx_GetBuffer
(
   vIpcMemCp_ChannelIndexType ChannelNr,
   uint32 DataSize,
   P2VAR(vIpcMemCp_BufferDescriptionType, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) BufferDesc
);


/**********************************************************************************************************************
 * vIpcMemCp_Rx_Confirm()
 *********************************************************************************************************************/
/*! \brief       Function marks the active transmission as finished.
 *  \details     After the call the message space is freed and can be used to transmit further data.
 *  \param[in]   ChannelNr               The channel index to identify the used channel.
 *  \return      E_OK                    Confirmation was successful.
 *  \return      E_NOT_OK                Confirmation has failed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \trace       CREQ-133057
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_Rx_Confirm
(
   vIpcMemCp_ChannelIndexType ChannelNr
);

/**********************************************************************************************************************
 * vIpcMemCp_Rx_Cancel()
 *********************************************************************************************************************/
/*! \brief       Function cancels the currently active reception.
 *  \details     The unreceived message is discarded.
 *  \param[in]   ChannelNr               The channel index to identify the used channel.
 *  \return      E_OK                    Cancel was successful.
 *  \return      E_NOT_OK                Cancel has failed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \trace       CREQ-133058
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_Rx_Cancel
(
   vIpcMemCp_ChannelIndexType ChannelNr
);

# define VIPCMEMCP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* VIPCMEMCP_H */

/**********************************************************************************************************************
 *  END OF FILE: vIpcMemCp.h
 *********************************************************************************************************************/
