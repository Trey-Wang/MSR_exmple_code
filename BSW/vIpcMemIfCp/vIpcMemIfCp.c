/*********************************************************************************************************************
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
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file
 *        \brief  vIpcMemIfCp file
 *
 *      \details  Contains the implementation of the module's API functions with external visibility.
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to vIpcMemIfCp.h.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define VIPCMEMIFCP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vIpcMemIfCp_Int.h"
#include "vIpcMemIfCp_TxInt.h"
#include "vIpcMemIfCp_RxInt.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of vIpcMemIfCp header file */
#if (  (VIPCMEMIFCP_SW_MAJOR_VERSION != (1u)) \
    || (VIPCMEMIFCP_SW_MINOR_VERSION != (0u)) \
    || (VIPCMEMIFCP_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of vIpcMemIfCp.c and vIpcMemIfCp.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (VIPCMEMIFCP_CFG_MAJOR_VERSION != (1u)) \
    || (VIPCMEMIFCP_CFG_MINOR_VERSION != (0u)) )
# error "Version numbers of vIpcMemIfCp.c and vIpcMemIfCp_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

# define VIPCMEMIFCP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

P2CONST(vIpcMemIfCp_ConfigType, VIPCMEMIFCP_VAR_ZERO_INIT, VIPCMEMIFCP_PBCFG) vIpcMemIfCp_ConfigDataPtr = NULL_PTR;
VAR(vIpcMemIfCp_InitStatusType, VIPCMEMIFCP_VAR_ZERO_INIT) vIpcMemIfCp_InitStatus = VIPCMEMIFCP_NOT_INITIALIZED;

# define VIPCMEMIFCP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# define VIPCMEMIFCP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  vIpcMemIfCp_InitMemory()
 *********************************************************************************************************************/
FUNC(void, VIPCMEMIFCP_CODE) vIpcMemIfCp_InitMemory(void)
{
   vIpcMemIfCp_ErrorCodeType status = VIPCMEMIFCP_E_NO_ERROR;

   vIpcMemIfCp_InitStatus = VIPCMEMIFCP_NOT_INITIALIZED;

   (void) vIpcMemIfCp_ReportError(VIPCMEMIFCP_SID_INIT_MEMORY, status);
} /* vIpcMemIfCp_InitMemory */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Init()
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
FUNC(void, VIPCMEMIFCP_CODE) vIpcMemIfCp_Init(P2CONST(vIpcMemIfCp_ConfigType, AUTOMATIC, VIPCMEMIFCP_INIT_DATA) ConfigPtr)
{
   vIpcMemIfCp_ErrorCodeType status = VIPCMEMIFCP_E_NO_ERROR;

   /* #10 If UseInitPoiter is on: */
   if (vIpcMemIfCp_UseInitPointer() == TRUE)
   {
      /* #20 Initialize the global configuration pointer. */
      vIpcMemIfCp_ConfigDataPtr = ConfigPtr;

      /* #30 Check that the pointer is not null. */
      if (vIpcMemIfCp_ConfigDataPtr == NULL_PTR)
      {
         if (vIpcMemIfCp_UseEcumBswErrorHook() == TRUE)
         {
            EcuM_BswErrorHook(VIPCMEMIFCP_MODULE_ID, ECUM_BSWERROR_NULLPTR);
         }
         status = VIPCMEMIFCP_E_INV_PTR;
      }
      /* #40 Check final magic number. */
      else if (vIpcMemIfCp_CheckFinalMagicNumber() == TRUE)
      {
         vIpcMemIfCp_ConfigDataPtr = NULL_PTR;
         if (vIpcMemIfCp_UseEcumBswErrorHook() == TRUE)
         {
            EcuM_BswErrorHook(VIPCMEMIFCP_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
         }
         status = VIPCMEMIFCP_E_INV_PTR;
      }
      else
      {
         /* No action required in any other case. MISRA 14.10 */
      }
   }
   /* #50 Otherwise check that pointer is null. */
   else if (ConfigPtr != NULL_PTR)
   {
      status = VIPCMEMIFCP_E_INV_PTR;
   }
   else
   {
      /* No action required in any other case. MISRA 14.10 */
   }

   /* #60 If no error has occured: */
   if (status == VIPCMEMIFCP_E_NO_ERROR)
   {
      PduIdType channelId;

      /* #70 Iterate over all Tx channels. */
      for (channelId = 0; channelId < vIpcMemIfCp_Tx_GetNrOfChannels(); channelId++)
      {
         /* #80 Initialize a Tx channel. */
         vIpcMemIfCp_Tx_InitChannel(vIpcMemIfCp_Tx_GetChannel(channelId));
      }

      /* #90 Iterate over all Rx channels. */
      for (channelId = 0; channelId < vIpcMemIfCp_Rx_GetNrOfChannels(); channelId++)
      {
         /* #100 Initialize a Tx channel. */
         vIpcMemIfCp_Rx_InitChannel(vIpcMemIfCp_Rx_GetChannel(channelId));
      }

      vIpcMemIfCp_InitStatus = VIPCMEMIFCP_INITIALIZED;
   }
   (void) vIpcMemIfCp_ReportError(VIPCMEMIFCP_SID_INIT, status);
} /* vIpcMemIfCp_Init() */                                                                                              /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (VIPCMEMIFCP_VERSION_INFO_API == STD_ON)

/**********************************************************************************************************************
 *  vIpcMemIfCp_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, VIPCMEMIFCP_CODE) vIpcMemIfCp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, VIPCMEMIFCP_VAR_NOINIT) VersionInfo)
{
   vIpcMemIfCp_ErrorCodeType status = VIPCMEMIFCP_E_NO_ERROR;
   boolean isDetEnabled = vIpcMemIfCp_DevErrorDetect();

   /* #10 Check if the pointer is valid. */
   if (isDetEnabled && (VersionInfo == NULL_PTR))
   {
      status = VIPCMEMIFCP_E_PARAM;
   }
   else
   {
      /* #20 Set the version info to the values from the component header. */
      VersionInfo->vendorID         = (VIPCMEMIFCP_VENDOR_ID);
      VersionInfo->moduleID         = (VIPCMEMIFCP_MODULE_ID);
      VersionInfo->sw_major_version = (VIPCMEMIFCP_SW_MAJOR_VERSION);
      VersionInfo->sw_minor_version = (VIPCMEMIFCP_SW_MINOR_VERSION);
      VersionInfo->sw_patch_version = (VIPCMEMIFCP_SW_PATCH_VERSION);
   }

   (void) vIpcMemIfCp_ReportError(VIPCMEMIFCP_SID_GET_VERSION_INFO, status);
}

#endif /* (VIPCMEMIFCP_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Transmit()
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
FUNC(Std_ReturnType, VIPCMEMIFCP_CODE) vIpcMemIfCp_Transmit(PduIdType ChannelId, const PduInfoType* PduInfoPtr)
{
   Std_ReturnType retValue = E_NOT_OK;
   vIpcMemIfCp_ErrorCodeType errId = VIPCMEMIFCP_E_NO_ERROR;

   boolean isDetEnabled = vIpcMemIfCp_DevErrorDetect();

   /* #10 Check if the ChannelId is valid. */
   if (isDetEnabled && (ChannelId >= vIpcMemIfCp_Tx_GetNrOfChannels()))                                                 /* PRQA S 3415 */ /* MD_vIpcMemIfCp_3415 */
   {
      errId = VIPCMEMIFCP_E_PARAM;
   }
   /* #20 Check if the pointer is valid. */
   else if (isDetEnabled && (PduInfoPtr == NULL_PTR))
   {
      errId = VIPCMEMIFCP_E_INV_PTR;
   }
   /* #30 Check if the module is initialized. */
   else if (isDetEnabled && (vIpcMemIfCp_IsInitialized() == FALSE))                                                     /* PRQA S 3415 */ /* MD_vIpcMemIfCp_3415 */
   {
      errId = VIPCMEMIFCP_E_UNINIT;
   }
   else
   {
      /* #40 Start a transmission. */
      retValue = vIpcMemIfCp_Tx_StartTransmission(ChannelId, PduInfoPtr->SduLength);

      /* #50 If starting was successful: */
      if (retValue == E_OK)
      {
         /* #60 Process the channel. */
         vIpcMemIfCp_Tx_Process(ChannelId, FALSE);
      }
   }

   retValue |= vIpcMemIfCp_ReportError(VIPCMEMIFCP_SID_TRANSMIT, errId);
   return retValue;
} /* vIpcMemIfCp_Transmit */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Receive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, VIPCMEMIFCP_CODE) vIpcMemIfCp_Receive(PduIdType ChannelId)
{
   Std_ReturnType retValue = E_NOT_OK;
   vIpcMemIfCp_ErrorCodeType errId = VIPCMEMIFCP_E_NO_ERROR;

   boolean isDetEnabled = vIpcMemIfCp_DevErrorDetect();

   /* #10 Check if the ChannelId is valid. */
   if (isDetEnabled && (ChannelId >= vIpcMemIfCp_Rx_GetNrOfChannels()))                                                 /* PRQA S 3415 */ /* MD_vIpcMemIfCp_3415 */
   {
      errId = VIPCMEMIFCP_E_PARAM;
   }
   /* #20 Check if the module is initialized. */
   else if (isDetEnabled && (vIpcMemIfCp_IsInitialized() == FALSE))                                                     /* PRQA S 3415 */ /* MD_vIpcMemIfCp_3415 */
   {
      errId = VIPCMEMIFCP_E_UNINIT;
   }
   else
   {
      /* #30 Process the channel. */
      vIpcMemIfCp_EnterExclusiveArea();
      vIpcMemIfCp_Rx_Process(ChannelId);
      vIpcMemIfCp_ExitExclusiveArea();

      retValue = E_OK;
   }

   retValue |= vIpcMemIfCp_ReportError(VIPCMEMIFCP_SID_RECEIVE, errId);
   return retValue;
} /* vIpcMemIfCp_Receive */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, VIPCMEMIFCP_CODE) vIpcMemIfCp_Tx_MainFunction(void)
{
   PduIdType channelId;

   if (vIpcMemIfCp_IsInitialized() == TRUE)
   {
      /* #10 For each Tx channel. */
      for (channelId = 0; channelId < vIpcMemIfCp_Tx_GetNrOfChannels(); channelId++)
      {
         /* #20 Process a Tx channel. */
         vIpcMemIfCp_Tx_Process(channelId, TRUE);
      }
   }
} /* vIpcMemIfCp_Tx_MainFunction */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, VIPCMEMIFCP_CODE) vIpcMemIfCp_Rx_MainFunction(void)
{
   PduIdType channelId;

   if (vIpcMemIfCp_IsInitialized() == TRUE)
   {
      /* #10 For each Rx channel. */
      for (channelId = 0; channelId < vIpcMemIfCp_Rx_GetNrOfChannels(); channelId++)
      {
         /* #20 Process a Rx channel. */
         vIpcMemIfCp_EnterExclusiveArea();
         vIpcMemIfCp_Rx_Process(channelId);
         vIpcMemIfCp_ExitExclusiveArea();
      }
   }
} /* vIpcMemIfCp_Rx_MainFunction */

#define VIPCMEMIFCP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for modules-specific MISRA deviations:
  MD_vIpcMemIfCp_0883: MISRA rule 19.15
      Reason:     The protection against repeated inclusion is complete and correct. The MISRA tool is unable to
                  detect this.
      Risk:       None.
      Prevention: None.
  MD_vIpcMemIfCp_3415: MISRA rule 12.4
      Reason:     The called function is pure reading function and has no side effects.
      Risk:       None.
      Prevention: None.
 */

/**********************************************************************************************************************
 *  END OF FILE: vIpcMemIfCp.c
 *********************************************************************************************************************/
