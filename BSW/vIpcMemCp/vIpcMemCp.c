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
 *        \brief  vIpcMemCp file
 *
 *      \details  Contains the implementation of the module's API functions with external visibility.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to vIpcMemCp.h.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define VIPCMEMCP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vIpcMemCp_Int.h"
#include "vIpcMemCp_TxInt.h"
#include "vIpcMemCp_RxInt.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of vIpcMemCp header file */
#if (  (VIPCMEMCP_SW_MAJOR_VERSION != (1u)) \
    || (VIPCMEMCP_SW_MINOR_VERSION != (1u)) \
    || (VIPCMEMCP_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of vIpcMemCp.c and vIpcMemCp.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (VIPCMEMCP_CFG_MAJOR_VERSION != (1u)) \
    || (VIPCMEMCP_CFG_MINOR_VERSION != (1u)) )
# error "Version numbers of vIpcMemCp.c and vIpcMemCp_Cfg.h are inconsistent!"
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

# define VIPCMEMCP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

P2CONST(vIpcMemCp_ConfigType, VIPCMEMCP_VAR_ZERO_INIT, VIPCMEMCP_PBCFG) vIpcMemCp_ConfigDataPtr = NULL_PTR;

# define VIPCMEMCP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VIPCMEMCP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  vIpcMemCp_InitMemory()
 *********************************************************************************************************************/
FUNC(void, VIPCMEMCP_CODE) vIpcMemCp_InitMemory(void)
{
   vIpcMemCp_ErrorCodeType status = VIPCMEMCP_E_NO_ERROR;

   (void) vIpcMemCp_ReportError(VIPCMEMCP_SID_INIT_MEMORY, status);
} /* vIpcMemCp_InitMemory */

/**********************************************************************************************************************
 *  vIpcMemCp_Init()
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
FUNC(void, VIPCMEMCP_CODE) vIpcMemCp_Init(P2CONST(vIpcMemCp_ConfigType, AUTOMATIC, VIPCMEMCP_INIT_DATA) ConfigPtr)
{
   vIpcMemCp_ErrorCodeType status = VIPCMEMCP_E_NO_ERROR;
   vIpcMemCp_ChannelIndexType i;
   vIpcMemCp_QueuePtrType queue;

   /* #10 If UseInitPoiter is on: */
   if (vIpcMemCp_UseInitPointer() == TRUE)
   {
      /* #20 Initialize the global configuration pointer. */
      vIpcMemCp_ConfigDataPtr = ConfigPtr;

      /* #30 Check that the pointer is not null. */
      if (vIpcMemCp_ConfigDataPtr == NULL_PTR)
      {
         if (vIpcMemCp_UseEcumBswErrorHook() == TRUE)
         {
            EcuM_BswErrorHook(VIPCMEMCP_MODULE_ID, ECUM_BSWERROR_NULLPTR);
         }
         status = VIPCMEMCP_E_INV_PTR;
      }
      /* #40 Check final magic number. */
      else if (vIpcMemCp_CheckFinalMagicNumber() == TRUE)
      {
         vIpcMemCp_ConfigDataPtr = NULL_PTR;
         if (vIpcMemCp_UseEcumBswErrorHook() == TRUE)
         {
            EcuM_BswErrorHook(VIPCMEMCP_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
         }
         status = VIPCMEMCP_E_INV_PTR;
      }
      else
      {
         /* No action required in any other case. MISRA 14.10 */
      }
   }
   /* #50 Otherwise check that pointer is null. */
   else if (ConfigPtr != NULL_PTR)
   {
      status = VIPCMEMCP_E_INV_PTR;
   }
   else
   {
      /* No action required in any other case. MISRA 14.10 */
   }

   /* #60 If no error has occured: */
   if (status == VIPCMEMCP_E_NO_ERROR)
   {
      /* #70 For each configured Tx channel: */
      for (i = 0; i < vIpcMemCp_Tx_GetNrOfChannels(); i++)
      {
         /* #80 Initialize the Send-Part of the Sender-Queue. */
         queue = vIpcMemCp_Tx_GetQueue(i);
         vIpcMemCp_WriteQueuePointer(&(queue->Head), 0);
         queue->HeadReserved = 0;
         queue->HeadInitialized = TRUE;
      }

      /* #90 For each configured Rx channel: */
      for (i = 0; i < vIpcMemCp_Rx_GetNrOfChannels(); i++)
      {
         /* #100 Initialize the Receive-Part of the Receiver-Queue. */
         queue = vIpcMemCp_Rx_GetQueue(i);
         vIpcMemCp_WriteQueuePointer(&(queue->Tail), 0);
         queue->TailReserved = 0;
         queue->TailInitialized = TRUE;
      }
   }
   (void) vIpcMemCp_ReportError(VIPCMEMCP_SID_INIT, status);
} /* vIpcMemCp_Init */                                                                                                  /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (VIPCMEMCP_VERSION_INFO_API == STD_ON)

/**********************************************************************************************************************
 *  vIpcMemCp_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, VIPCMEMCP_CODE) vIpcMemCp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) VersionInfo)
{
   vIpcMemCp_ErrorCodeType status = VIPCMEMCP_E_NO_ERROR;
   boolean isDetEnabled = vIpcMemCp_DevErrorDetect();

   /* #10 Check if the pointer is valid. */
   if (isDetEnabled && (VersionInfo == NULL_PTR))
   {
      status = VIPCMEMCP_E_PARAM;
   }
   else
   {
      /* #20 Set the version info to the values from the component header. */
      VersionInfo->vendorID         = (VIPCMEMCP_VENDOR_ID);
      VersionInfo->moduleID         = (VIPCMEMCP_MODULE_ID);
      VersionInfo->sw_major_version = (VIPCMEMCP_SW_MAJOR_VERSION);
      VersionInfo->sw_minor_version = (VIPCMEMCP_SW_MINOR_VERSION);
      VersionInfo->sw_patch_version = (VIPCMEMCP_SW_PATCH_VERSION);
   }

   (void) vIpcMemCp_ReportError(VIPCMEMCP_SID_GET_VERSION_INFO, status);
} /* vIpcMemCp_GetVersionInfo */

#endif /* (VIPCMEMCP_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 *  vIpcMemCp_Tx_BeginTransmit()
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
 *
 */
FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_Tx_BeginTransmit
(
   vIpcMemCp_ChannelIndexType ChannelNr,
   uint32 DataSize
)
{
   Std_ReturnType retValue = E_NOT_OK;
   vIpcMemCp_ErrorCodeType status = VIPCMEMCP_E_NO_ERROR;
   vIpcMemCp_QueuePtrType queue;
   boolean isDetEnabled = vIpcMemCp_DevErrorDetect();

   /* #10 Check if the channel index is correct. */
   if (isDetEnabled && (ChannelNr >= vIpcMemCp_Tx_GetNrOfChannels()))                                                   /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
   {
      status = VIPCMEMCP_E_PARAM;
   }
   /* #20 Check if the size is valid. */
   else if (isDetEnabled && (DataSize == 0))
   {
      status = VIPCMEMCP_E_PARAM;
   }
   else
   {
      /* #30 Retrieve the pointer to the queue management data. */
      queue = vIpcMemCp_Tx_GetQueue(ChannelNr);

      /* #40 Check if the channel was fully initialized. */
      if (isDetEnabled && (vIpcMemCp_QueueIsInitialized(queue) == FALSE))                                               /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_UNINIT;
      }
      /* #50 Check if a transmission already in progress. */
      else if (isDetEnabled && (vIpcMemCp_Tx_QueueIsPending(queue)))                                                     /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_TX_ALREADY_PENDING;
      }
      else
      {
         /* #60 Calculate the total data size. */
         uint32 totalDataSize = DataSize + sizeof(vIpcMemCp_MessageHeaderType);

         /* #70 If enough space is available: */
         if (vIpcMemCp_QueueLeft(queue, vIpcMemCp_Tx_GetBufferSize(ChannelNr)) >= totalDataSize)
         {
            vIpcMemCp_MessageHeaderType header;
            vIpcMemCp_QueuePosType headerPos;

            /* #80 Prepare the header. */
            header.Size = DataSize;

            /* #90 Get the corrected header position. */
            headerPos = vIpcMemCp_QueueGetCorrectedPos(&(queue->Head), vIpcMemCp_Tx_GetBufferSize(ChannelNr));

            /* #100 Write header into the buffer. */
            /*!
            Internal comment removed.
 *
 *
 *
            */
            vIpcMemCp_WriteMessageHeader(vIpcMemCp_Tx_GetBufferStart(ChannelNr) + headerPos, &header);                  /* PRQA S 0488 */ /* MD_vIpcMemCp_0488 */

            /* #110 Advance the reserved queue pointer. */
            queue->HeadReserved = headerPos + sizeof(vIpcMemCp_MessageHeaderType);

            retValue = E_OK;
         }
      }
   }
   retValue |= vIpcMemCp_ReportError(VIPCMEMCP_SID_TX_BEGINTRANSMIT, status);
   return retValue;
} /* vIpcMemCp_Tx_BeginTransmit */                                                                                      /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  vIpcMemCp_Tx_GetBuffer()
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
 *
 *
 *
 */
FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_Tx_GetBuffer
(
   vIpcMemCp_ChannelIndexType ChannelNr,
   uint32 DataSize,
   P2VAR(vIpcMemCp_BufferDescriptionType, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) BufferDesc
)
{
   vIpcMemCp_ErrorCodeType status = VIPCMEMCP_E_NO_ERROR;
   vIpcMemCp_QueuePtrType queue;
   boolean isDetEnabled = vIpcMemCp_DevErrorDetect();

   /* #10 Check if the channel index is correct. */
   if (isDetEnabled && (ChannelNr >= vIpcMemCp_Tx_GetNrOfChannels()))                                                   /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
   {
      status = VIPCMEMCP_E_PARAM;
   }
   /* #20 Check if the size is valid. */
   else if (isDetEnabled && (DataSize == 0))
   {
      status = VIPCMEMCP_E_PARAM;
   }
   /* #30 Check if the parameter pointer is valid. */
   else if (isDetEnabled && (BufferDesc == NULL_PTR))
   {
      status = VIPCMEMCP_E_INV_PTR;
   }
   else
   {
      /* #40 Retrieve the pointer to the queue management data. */
      queue = vIpcMemCp_Tx_GetQueue(ChannelNr);

      /* #50 Check if the channel was fully initialized. */
      if (isDetEnabled && (vIpcMemCp_QueueIsInitialized(queue) == FALSE))                                               /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_UNINIT;
      }
      /* #60 Check if a transmission was started. */
      else if (isDetEnabled && (!(vIpcMemCp_Tx_QueueIsPending(queue))))                                                  /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_TX_NOT_PENDING;
      }
      /* #70 Check if the requested size does not exceeds the message size. */
      else if (isDetEnabled && (vIpcMemCp_Tx_CheckTotalDataSize(ChannelNr, DataSize) == TRUE))                          /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_INCONSISTENT_SIZE;
      }
      else
      {
         /* #80 Calculate the size from "reserved" pointer until end of the buffer. */
         uint32 sizeUntilEnd = vIpcMemCp_Tx_GetBufferSize(ChannelNr) - queue->HeadReserved;

         /* #90 Wrap-around the "reserved" pointer if it's currently at the end. */
         if (sizeUntilEnd == 0)
         {
            queue->HeadReserved = 0;
            sizeUntilEnd = DataSize;
         }

         /* #100 Store the start address of the buffer segment. */
         BufferDesc->Ptr = vIpcMemCp_Tx_GetBufferStart(ChannelNr) + queue->HeadReserved;                                /* PRQA S 0488 */ /* MD_vIpcMemCp_0488 */

         /* #110 Store the size until end of the buffer if it's smaller than requested size. */
         if (DataSize > sizeUntilEnd)
         {
            BufferDesc->Size = sizeUntilEnd;
         }
         /* #120 Store the full size otherwise. */
         else
         {
            BufferDesc->Size = DataSize;
         }

         /* #130 Advance the "reserved" pointer by buffer segment's size. */
         queue->HeadReserved += BufferDesc->Size;
      }
   }

   return vIpcMemCp_ReportError(VIPCMEMCP_SID_TX_GETBUFFER, status);
} /* vIpcMemCp_Tx_GetBuffer */                                                                                          /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  vIpcMemCp_Tx_Confirm()
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
FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_Tx_Confirm
(
   vIpcMemCp_ChannelIndexType ChannelNr
)
{
   vIpcMemCp_ErrorCodeType status = VIPCMEMCP_E_NO_ERROR;
   vIpcMemCp_QueuePtrType queue;
   boolean isDetEnabled = vIpcMemCp_DevErrorDetect();

   /* #10 Check if the channel index is correct. */
   if (isDetEnabled && (ChannelNr >= vIpcMemCp_Tx_GetNrOfChannels()))                                                   /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
   {
      status = VIPCMEMCP_E_PARAM;
   }
   else
   {
      /* #20 Retrieve the pointer to the queue management data. */
      queue = vIpcMemCp_Tx_GetQueue(ChannelNr);

      /* #30 Check if the channel was fully initialized. */
      if (isDetEnabled && (vIpcMemCp_QueueIsInitialized(queue) == FALSE))                                               /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_UNINIT;
      }
      /* #40 Check if a transmission was started. */
      else if (isDetEnabled && (!(vIpcMemCp_Tx_QueueIsPending(queue))))                                                  /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_TX_NOT_PENDING;
      }
      else
      {
         /* #50 Retrieve the pending message size from the buffer. */
         uint32 messageSize = vIpcMemCp_Tx_GetPendingMessageSize(ChannelNr);

         /* #60 Calculate the size of reserved data. */
         uint32 reservedSize = vIpcMemCp_Tx_GetReservedSize(queue, vIpcMemCp_Tx_GetBufferSize(ChannelNr));

         /* #70 If the calculated size doesn't correspond with the message size: */
         if (isDetEnabled && (reservedSize != (messageSize + sizeof(vIpcMemCp_MessageHeaderType))))
         {
            /* #80 Rollback the active transmit. */
            queue->HeadReserved = vIpcMemCp_ReadQueuePointer(&(queue->Head));

            status = VIPCMEMCP_E_INCONSISTENT_SIZE;
         }
         /* #90 Otherwise: */
         else
         {
            /* #100 Apply the "reserved" pointer into the normal. */
            vIpcMemCp_WriteQueuePointer(&(queue->Head), queue->HeadReserved);
         }
      }
   }
   return vIpcMemCp_ReportError(VIPCMEMCP_SID_TX_CONFIRM, status);
} /* vIpcMemCp_Tx_Confirm */                                                                                            /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * vIpcMemCp_Tx_Cancel()
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
FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_Tx_Cancel
(
   vIpcMemCp_ChannelIndexType ChannelNr
)
{
   vIpcMemCp_ErrorCodeType status = VIPCMEMCP_E_NO_ERROR;
   vIpcMemCp_QueuePtrType queue;
   boolean isDetEnabled = vIpcMemCp_DevErrorDetect();

   /* #10 Check if the channel index is correct. */
   if (isDetEnabled && (ChannelNr >= vIpcMemCp_Tx_GetNrOfChannels()))                                                   /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
   {
      status = VIPCMEMCP_E_PARAM;
   }
   else
   {
      /* #20 Retrieve the pointer to the queue management data. */
      queue = vIpcMemCp_Tx_GetQueue(ChannelNr);

      /* #30 Check if the channel was fully initialized. */
      if (isDetEnabled && (vIpcMemCp_QueueIsInitialized(queue) == FALSE))                                               /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_UNINIT;
      }
      /* #40 Check if a transmission was started. */
      else if (isDetEnabled && (!(vIpcMemCp_Tx_QueueIsPending(queue))))                                                  /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_TX_NOT_PENDING;
      }
      else
      {
         /* #50 Rollback the active transmit. */
         queue->HeadReserved = vIpcMemCp_ReadQueuePointer(&(queue->Head));
      }
   }
   return vIpcMemCp_ReportError(VIPCMEMCP_SID_TX_CANCEL, status);
} /* vIpcMemCp_Tx_Cancel */

/**********************************************************************************************************************
 *  vIpcMemCp_Rx_BeginReceive()
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
FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_Rx_BeginReceive
(
   vIpcMemCp_ChannelIndexType ChannelNr,
   P2VAR(uint32, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) DataSize
)
{
   Std_ReturnType retValue = E_NOT_OK;
   vIpcMemCp_ErrorCodeType status = VIPCMEMCP_E_NO_ERROR;
   vIpcMemCp_QueuePtrType queue;
   boolean isDetEnabled = vIpcMemCp_DevErrorDetect();

   /* #10 Check if the channel index is correct. */
   if (isDetEnabled && (ChannelNr >= vIpcMemCp_Rx_GetNrOfChannels()))                                                   /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
   {
      status = VIPCMEMCP_E_PARAM;
   }
   /* #20 Check if the parameter pointer is valid. */
   else if (isDetEnabled && (DataSize == NULL_PTR))
   {
      status = VIPCMEMCP_E_INV_PTR;
   }
   else
   {
      /* #30 Retrieve the pointer to the queue management data. */
      queue = vIpcMemCp_Rx_GetQueue(ChannelNr);

      /* #40 Check if the channel was fully initialized. */
      if (isDetEnabled && (vIpcMemCp_QueueIsInitialized(queue) == FALSE))                                               /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_UNINIT;
      }
      /* #50 Check if a receiving already in progress. */
      else if (isDetEnabled && (vIpcMemCp_Rx_QueueIsPending(queue)))                                                     /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_RX_ALREADY_PENDING;
      }
      else
      {
         /* #60 If the queue is not empty: */
         if (vIpcMemCp_QueueIsEmpty(queue) == FALSE)
         {
            vIpcMemCp_MessageHeaderType header;
            vIpcMemCp_QueuePosType headerPos;

            /* #70 Get the corrected header position. */
            headerPos = vIpcMemCp_QueueGetCorrectedPos(&(queue->Tail), vIpcMemCp_Rx_GetBufferSize(ChannelNr));

            /* #80 Read the header from the buffer. */
            vIpcMemCp_ReadMessageHeader(vIpcMemCp_Rx_GetBufferStart(ChannelNr) + headerPos, &header);                   /* PRQA S 0488 */ /* MD_vIpcMemCp_0488 */

            /* #90 Store the message size. */
            *DataSize = header.Size;

            /* #100 Advance the reserved queue pointer. */
            queue->TailReserved = headerPos + sizeof(vIpcMemCp_MessageHeaderType);

            retValue = E_OK;
         }
      }
   }
   retValue |= vIpcMemCp_ReportError(VIPCMEMCP_SID_RX_BEGINRECEIVE, status);
   return retValue;
} /* vIpcMemCp_Rx_BeginReceive */                                                                                       /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  vIpcMemCp_Rx_GetBuffer()
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
 *
 *
 *
 */
FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_Rx_GetBuffer
(
   vIpcMemCp_ChannelIndexType ChannelNr,
   uint32 DataSize,
   P2VAR(vIpcMemCp_BufferDescriptionType, AUTOMATIC, VIPCMEMCP_VAR_NOINIT) BufferDesc
)
{
   vIpcMemCp_ErrorCodeType status = VIPCMEMCP_E_NO_ERROR;
   vIpcMemCp_QueuePtrType queue;
   boolean isDetEnabled = vIpcMemCp_DevErrorDetect();

   /* #10 Check if the channel index is correct. */
   if (isDetEnabled && (ChannelNr >= vIpcMemCp_Rx_GetNrOfChannels()))                                                   /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
   {
      status = VIPCMEMCP_E_PARAM;
   }
   /* #20 Check if the size is valid. */
   else if (isDetEnabled && (DataSize == 0))
   {
      status = VIPCMEMCP_E_PARAM;
   }
   /* #30 Check if the parameter pointer is valid. */
   else if (isDetEnabled && (BufferDesc == NULL_PTR))
   {
      status = VIPCMEMCP_E_INV_PTR;
   }
   else
   {
      /* #40 Retrieve the pointer to the queue management data. */
      queue = vIpcMemCp_Rx_GetQueue(ChannelNr);

      /* #50 Check if the channel was fully initialized. */
      if (isDetEnabled && (vIpcMemCp_QueueIsInitialized(queue) == FALSE))                                               /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_UNINIT;
      }
      /* #60 Check if a receiving was started already. */
      else if (isDetEnabled && (!(vIpcMemCp_Rx_QueueIsPending(queue))))                                                  /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_RX_NOT_PENDING;
      }
      /* #70 Check if the requested size does not exceeds the message size. */
      else if (isDetEnabled && (vIpcMemCp_Rx_CheckTotalDataSize(ChannelNr, DataSize) == TRUE))                          /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_INCONSISTENT_SIZE;
      }
      else
      {
         /* #80 Calculate the size from "reserved" pointer until end of the buffer. */
         uint32 sizeUntilEnd = vIpcMemCp_Rx_GetBufferSize(ChannelNr) - queue->TailReserved;

         /* #90 Wrap-around the "reserved" pointer if it's currently at the end. */
         if (sizeUntilEnd == 0)
         {
            queue->TailReserved = 0;
            sizeUntilEnd = DataSize;
         }

         /* #100 Store the start address of the buffer segment. */
         BufferDesc->Ptr = vIpcMemCp_Rx_GetBufferStart(ChannelNr) + queue->TailReserved;                                /* PRQA S 0488 */ /* MD_vIpcMemCp_0488 */

         /* #110 Store the size until end of the buffer if it's smaller than requested size. */
         if (DataSize > sizeUntilEnd)
         {
            BufferDesc->Size = sizeUntilEnd;
         }
         /* #120 Store the full size otherwise. */
         else
         {
            BufferDesc->Size = DataSize;
         }

         /* #130 Advance the "reserved" pointer by buffer segment's size. */
         queue->TailReserved += BufferDesc->Size;
      }
   }

   return vIpcMemCp_ReportError(VIPCMEMCP_SID_RX_GETBUFFER, status);
} /* vIpcMemCp_Rx_GetBuffer */                                                                                          /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  vIpcMemCp_Rx_Confirm()
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
FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_Rx_Confirm
(
   vIpcMemCp_ChannelIndexType ChannelNr
)
{
   vIpcMemCp_ErrorCodeType status = VIPCMEMCP_E_NO_ERROR;
   vIpcMemCp_QueuePtrType queue;
   boolean isDetEnabled = vIpcMemCp_DevErrorDetect();

   /* #10 Check if the channel index is correct. */
   if (isDetEnabled && (ChannelNr >= vIpcMemCp_Rx_GetNrOfChannels()))                                                   /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
   {
      status = VIPCMEMCP_E_PARAM;
   }
   else
   {
      /* #20 Retrieve the pointer to the queue management data. */
      queue = vIpcMemCp_Rx_GetQueue(ChannelNr);

      /* #30 Check if the channel was fully initialized. */
      if (isDetEnabled && (vIpcMemCp_QueueIsInitialized(queue) == FALSE))                                               /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_UNINIT;
      }
      /* #40 Check if a receiving was started. */
      else if (isDetEnabled && (!(vIpcMemCp_Rx_QueueIsPending(queue))))                                                  /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_RX_NOT_PENDING;
      }
      else
      {
         /* #50 Retrieve the pending message size from the buffer. */
         uint32 messageSize = vIpcMemCp_Rx_GetPendingMessageSize(ChannelNr);

         /* #60 Calculate the size of reserved data. */
         uint32 reservedSize = vIpcMemCp_Rx_GetReservedSize(queue, vIpcMemCp_Rx_GetBufferSize(ChannelNr));

         /* #70 If the calculated size doesn't correspond with the message size: */
         if (isDetEnabled && (reservedSize != (messageSize + sizeof(vIpcMemCp_MessageHeaderType))))
         {
            /* #80 Rollback the active receive. */
            queue->TailReserved = vIpcMemCp_ReadQueuePointer(&(queue->Tail));

            status = VIPCMEMCP_E_INCONSISTENT_SIZE;
         }
         /* #90 Otherwise: */
         else
         {
            /* #100 Apply the "reserved" pointer into the normal. */
            vIpcMemCp_WriteQueuePointer(&(queue->Tail), queue->TailReserved);
         }

      }
   }
   return vIpcMemCp_ReportError(VIPCMEMCP_SID_RX_CONFIRM, status);
} /* vIpcMemCp_Rx_Confirm */                                                                                            /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * vIpcMemCp_Rx_Cancel()
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
 *
 */
FUNC(Std_ReturnType, VIPCMEMCP_CODE)
vIpcMemCp_Rx_Cancel
(
   vIpcMemCp_ChannelIndexType ChannelNr
)
{
   vIpcMemCp_ErrorCodeType status = VIPCMEMCP_E_NO_ERROR;
   vIpcMemCp_QueuePtrType queue;
   boolean isDetEnabled = vIpcMemCp_DevErrorDetect();

   /* #10 Check if the channel index is correct. */
   if (isDetEnabled && (ChannelNr >= vIpcMemCp_Rx_GetNrOfChannels()))                                                   /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
   {
      status = VIPCMEMCP_E_PARAM;
   }
   else
   {
      /* #20 Retrieve the pointer to the queue management data. */
      queue = vIpcMemCp_Rx_GetQueue(ChannelNr);

      /* #30 Check if the channel was fully initialized. */
      if (isDetEnabled && (vIpcMemCp_QueueIsInitialized(queue) == FALSE))                                               /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_UNINIT;
      }
      /* #40 Check if a receiving was started. */
      else if (isDetEnabled && (!(vIpcMemCp_Rx_QueueIsPending(queue))))                                                  /* PRQA S 3415 */ /* MD_vIpcMemCp_3415 */
      {
         status = VIPCMEMCP_E_RX_NOT_PENDING;
      }
      else
      {
         vIpcMemCp_QueuePosType curTail;
         vIpcMemCp_QueuePosType tailAfterMsg;
         vIpcMemCp_MessageHeaderType header;
         uint32 fullSize;
         uint32 sizeUntilEnd;

         /* #50 Read current tail value. */
         curTail = vIpcMemCp_QueueGetCorrectedPos(&(queue->Tail), vIpcMemCp_Rx_GetBufferSize(ChannelNr));

         /* #60 Read the header from the buffer. */
         vIpcMemCp_ReadMessageHeader(vIpcMemCp_Rx_GetBufferStart(ChannelNr) + curTail, &header);                        /* PRQA S 0488 */ /* MD_vIpcMemCp_0488 */

         /* #70 Calculate the full data size. */
         fullSize = header.Size + sizeof(vIpcMemCp_MessageHeaderType);

         /* #80 Calculate the size until end of the buffer. */
         sizeUntilEnd = vIpcMemCp_Rx_GetBufferSize(ChannelNr) - curTail;

         /* #90 Calculate the wrap-around position if the message does not fit until end of the buffer. */
         if (sizeUntilEnd < fullSize)
         {
            tailAfterMsg = fullSize - sizeUntilEnd;
         }
         /* #100 Add to the current position otherwise. */
         else
         {
            tailAfterMsg = curTail + fullSize;
         }

         /* #110 Change the both pointers. */
         vIpcMemCp_WriteQueuePointer(&(queue->Tail), tailAfterMsg);
         queue->TailReserved = tailAfterMsg;
      }
   }
   return vIpcMemCp_ReportError(VIPCMEMCP_SID_RX_CANCEL, status);
} /* vIpcMemCp_Rx_Cancel */                                                                                             /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

#define VIPCMEMCP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for modules-specific MISRA deviations:
  MD_vIpcMemCp_0310: MISRA rule 11.4
      Reason:     In order to operate on generic memory buffer the casting to a different pointer type is necessary.
      Risk:       Using a pointer to access data with invalid alignment.
      Prevention: Make sure that the alignment requirements are always fullfilled.
  MD_vIpcMemCp_0488: MISRA rule 17.4
      Reason:     In order to operate on generic memory buffer pointer arithmetic is needed.
      Risk:       Accessing the data beyond the end of the buffer.
      Prevention: Always check for the buffer boundaries.
  MD_vIpcMemCp_0883: MISRA rule 19.15
      Reason:     The protection against repeated inclusion is complete and correct. The MISRA tool is unable to
                  detect this.
      Risk:       None.
      Prevention: None.
  MD_vIpcMemCp_3415: MISRA rule 12.4
      Reason:     The called function is pure reading function and has no side effects.
      Risk:       None.
      Prevention: None.
 */

/**********************************************************************************************************************
 *  END OF FILE: vIpcMemCp.c
 *********************************************************************************************************************/
