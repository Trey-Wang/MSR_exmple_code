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
 *      \details  Contains the declaration and definition of the module's internal functions.
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to vIpcMemIfCp.h.
 *********************************************************************************************************************/

#if !defined (VIPCMEMIFCP_RXINT_H)                                                                                      /* PRQA S 0883 */ /* MD_vIpcMemIfCp_0883 */
# define VIPCMEMIFCP_RXINT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "vIpcMemIfCp_Types.h"
# include "vIpcMemIfCp_PBcfg.h"

# include "vIpcMemCp.h"
# include "vIpc.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# if !defined (VIPCMEMIFCP_LOCAL)
#  define VIPCMEMIFCP_LOCAL static
# endif

# if !defined (VIPCMEMIFCP_LOCAL_INLINE)
#  define VIPCMEMIFCP_LOCAL_INLINE LOCAL_INLINE
# endif

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

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VIPCMEMIFCP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_GetChannel()
 *********************************************************************************************************************/
/*! \brief       Returns the channel state data structure associated with the channel ID.
 *  \details     -
 *  \pre         -
 *  \param[in]   ChannelId                 Channel index. It has to be valid.
 *  \return      Channel state data structure.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(P2VAR(vIpcMemIfCp_RxChannelStateType, AUTOMATIC, VIPC_APPL_VAR), VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_GetChannel
(
   PduIdType ChannelId
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_GetNrOfChannels()
 *********************************************************************************************************************/
/*! \brief       Returns the number of the configured Rx channels.
 *  \details     -
 *  \return      Number of Rx channels.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(PduIdType, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_GetNrOfChannels
(
   void
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_GetLLId()
 *********************************************************************************************************************/
/*! \brief       Returns the mapping of the vIpcMemIfCp channel ID to the vIpcMemCp channel ID.
 *  \details     -
 *  \param[in]   ChannelId                 Channel index. It has to be valid.
 *  \return      vIpcMemCp channel ID
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(vIpcMemCp_ChannelIndexType, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_GetLLId
(
   PduIdType ChannelId
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_GetULId()
 *********************************************************************************************************************/
/*! \brief       Returns the mapping of the vIpcMemIfCp channel ID to the vIpc channel ID.
 *  \details     -
 *  \param[in]   ChannelId                 Channel index. It has to be valid.
 *  \return      vIpc channel ID
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(PduIdType, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_GetULId
(
   PduIdType ChannelId
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_InitChannel()
 *********************************************************************************************************************/
/*! \brief       Performs initialization for a single channel.
 *  \details     -
 *  \param[in]   Channel                  Channel to initialized. Must not be NULL_PTR.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels.
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(void, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_InitChannel
(
   P2VAR(vIpcMemIfCp_RxChannelStateType, AUTOMATIC, VIPC_APPL_VAR) Channel
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_Process()
 *********************************************************************************************************************/
/*! \brief       Performs a full processing of a reception on the channel.
 *  \details     -
 *  \param[in]   ChannelId                 Channel index. It has to be valid.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels.
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(void, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_Process
(
   PduIdType ChannelId
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_ProcessIdleState()
 *********************************************************************************************************************/
/*! \brief       Performs the step of the processing associated with the idle state of the channel.
 *  \details     -
 *  \param[in]   ChannelId                 Channel index. It has to be valid.
 *  \return      TRUE                      Further processing is possible.
 *  \return      FALSE                     The result of the operation indicates the further processing should be 
 *                                         postponed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels.
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_ProcessIdleState
(
   PduIdType ChannelId
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_ProcessWaitForBufferState()
 *********************************************************************************************************************/
/*! \brief       Performs the step of the processing associated with the waiting operation.
 *  \details     -
 *  \param[in]   ChannelId                 Channel index. It has to be valid.
 *  \return      TRUE                      Further processing is possible.
 *  \return      FALSE                     The result of the operation indicates the further processing should be 
 *                                         postponed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels.
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_ProcessWaitForBufferState
(
   PduIdType ChannelId
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_ProcessCopyState()
 *********************************************************************************************************************/
/*! \brief       Performs the step of the processing associated with the pending copy operation.
 *  \details     -
 *  \param[in]   ChannelId                 Channel index. It has to be valid.
 *  \return      TRUE                      Further processing is possible.
 *  \return      FALSE                     The result of the operation indicates the further processing should be 
 *                                         postponed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels.
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_ProcessCopyState
(
   PduIdType ChannelId
);

# define VIPCMEMIFCP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define VIPCMEMIFCP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_GetChannel()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(P2VAR(vIpcMemIfCp_RxChannelStateType, AUTOMATIC, VIPC_APPL_VAR), VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_GetChannel
(
   PduIdType ChannelId
)
{
   return &(vIpcMemIfCp_GetRxChannelData(ChannelId));
} /* vIpcMemIfCp_Rx_GetChannel */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_GetNrOfChannels()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(PduIdType, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_GetNrOfChannels
(
   void
)
{
   return vIpcMemIfCp_GetSizeOfRxConfig();
} /* vIpcMemIfCp_Rx_GetNrOfChannels */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_GetLLId()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(vIpcMemCp_ChannelIndexType, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_GetLLId
(
   PduIdType ChannelId
)
{
   return vIpcMemIfCp_GetLowerLayerIdOfRxConfig(ChannelId);
} /* vIpcMemIfCp_Rx_GetLLId */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_GetULId()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(PduIdType, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_GetULId
(
   PduIdType ChannelId
)
{
   return vIpcMemIfCp_GetUpperLayerIdOfRxConfig(ChannelId);
} /* vIpcMemIfCp_Rx_GetULId */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_InitChannel()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(void, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_InitChannel
(
      P2VAR(vIpcMemIfCp_RxChannelStateType, AUTOMATIC, VIPC_APPL_VAR) Channel
)
{
   Channel->State = VIPCMEMIFCP_RX_IDLE;
} /* vIpcMemIfCp_Rx_InitChannel */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_Process()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VIPCMEMIFCP_LOCAL_INLINE FUNC(void, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_Process
(
   PduIdType ChannelId
)
{
   P2VAR(vIpcMemIfCp_RxChannelStateType, AUTOMATIC, VIPC_APPL_VAR) channel = vIpcMemIfCp_Rx_GetChannel(ChannelId);
   boolean continueProcessing = TRUE;

   do
   {
      switch(channel->State)
      {
      case VIPCMEMIFCP_RX_IDLE:
         /* #10 Process the idle state. */
         continueProcessing = vIpcMemIfCp_Rx_ProcessIdleState(ChannelId);
         break;

      case VIPCMEMIFCP_RX_WAIT_FOR_BUFFER:
         /* #20 Process the waiting for buffer state. */
         continueProcessing = vIpcMemIfCp_Rx_ProcessWaitForBufferState(ChannelId);
         break;

      case VIPCMEMIFCP_RX_COPY_PENDING:
         /* #30 Process the copy pending state. */
         continueProcessing = vIpcMemIfCp_Rx_ProcessCopyState(ChannelId);
         break;

      default:
         /* Invalid state. Reset to idle state. */
         channel->State = VIPCMEMIFCP_RX_IDLE;
         continueProcessing = FALSE;
         break;
      }
      /* #40 Loop while further processing is possible. */
   } while (continueProcessing == TRUE);
} /* vIpcMemIfCp_Rx_Process */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_ProcessIdleState()
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
 */
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_ProcessIdleState
(
   PduIdType ChannelId
)
{
   boolean continueProcessing = TRUE;
   P2VAR(vIpcMemIfCp_RxChannelStateType, AUTOMATIC, VIPC_APPL_VAR) channel = vIpcMemIfCp_Rx_GetChannel(ChannelId);

   /* #10 Request a reception from the lower layer. */
   Std_ReturnType receiveResult = vIpcMemIfCp_LL_Rx_BeginReceive(vIpcMemIfCp_Rx_GetLLId(ChannelId), &(channel->RestSize));

   /* #20 If no receivable message is available, abort processing. */
   if (receiveResult != E_OK)
   {
      continueProcessing = FALSE;
   }
   else
   {
      BufReq_ReturnType startOfReceptionResult;
      PduInfoType pduInfo;
      PduLengthType availableSize;

      /* #30 Request the buffer segment from the lower layer. */
      (void)vIpcMemIfCp_LL_Rx_GetBuffer(vIpcMemIfCp_Rx_GetLLId(ChannelId), channel->RestSize, &(channel->BufferDesc));

      /* #40 Signal the start of reception to the upper layer. */
      pduInfo.SduDataPtr  = channel->BufferDesc.Ptr;
      pduInfo.SduLength   = (PduLengthType)channel->BufferDesc.Size;
      startOfReceptionResult = vIpcMemIfCp_UL_StartOfReception(vIpcMemIfCp_Rx_GetULId(ChannelId), &pduInfo, (PduLengthType)channel->RestSize, &availableSize);

      /* #50 If an error has occurred: */
      if (startOfReceptionResult != BUFREQ_OK)
      {
         /* #60 Signal the reception abort to the lower layer. */
         (void)vIpcMemIfCp_LL_Rx_Cancel(vIpcMemIfCp_Rx_GetLLId(ChannelId));

         /* #70 Abort processing. */
         continueProcessing = FALSE;
      }
      /* #80 If the call was successful: */
      else
      {
         /* #90 If the upper layer does not have enough space: */
         if (availableSize == 0)
         {
            /* #100 Change the channel state to waiting for buffer and suspend processing. */
            channel->State = VIPCMEMIFCP_RX_WAIT_FOR_BUFFER;
            continueProcessing = FALSE;
         }
         /* #110 Otherwise: */
         else
         {
            /* #120 Change the channel state to queue copy request to the upper layer. */
            channel->State = VIPCMEMIFCP_RX_COPY_PENDING;
         }
      }
   }
   return continueProcessing;
} /* vIpcMemIfCp_Rx_ProcessIdleState */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_ProcessWaitForBufferState()
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
 */
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_ProcessWaitForBufferState
(
   PduIdType ChannelId
)
{
   PduInfoType pduInfo;
   PduLengthType availableBufferSize;
   BufReq_ReturnType copyResult;
   boolean continueProcessing = TRUE;
   P2VAR(vIpcMemIfCp_RxChannelStateType, AUTOMATIC, VIPC_APPL_VAR) channel = vIpcMemIfCp_Rx_GetChannel(ChannelId);

   /* #10 Request the available buffer size from the upper layer. */
   pduInfo.SduDataPtr  = NULL_PTR;
   pduInfo.SduLength   = 0;
   copyResult = vIpcMemIfCp_UL_CopyRxData(vIpcMemIfCp_Rx_GetULId(ChannelId), &pduInfo, &availableBufferSize);

   /* #20 If an error has occurred: */
   if (copyResult != BUFREQ_OK)
   {
      /* #30 Signal the reception abort to the lower layer. */
      (void)vIpcMemIfCp_LL_Rx_Cancel(vIpcMemIfCp_Rx_GetLLId(ChannelId));

      /* #40 Change the channel state to idle and abort processing. */
      channel->State = VIPCMEMIFCP_RX_IDLE;
      continueProcessing = FALSE;
   }
   /* #50 If the call was successful: */
   else
   {
      /* #60 If the available buffer size would not fit the next data segment: */
      if (availableBufferSize < channel->BufferDesc.Size)
      {
         /* #70 Suspend processing. */
         continueProcessing = FALSE;
      }
      /* #80 Otherwise: */
      {
         /* #90 Change the channel state to queue copy request to the upper layer. */
         channel->State = VIPCMEMIFCP_RX_COPY_PENDING;
      }
   }

   return continueProcessing;
} /* vIpcMemIfCp_Rx_ProcessWaitForBufferState */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Rx_ProcessCopyState()
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
 *
 */
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Rx_ProcessCopyState
(
   PduIdType ChannelId
)
{
   PduInfoType pduInfo;
   PduLengthType availableBufferSize;
   BufReq_ReturnType copyResult;
   boolean continueProcessing = TRUE;
   P2VAR(vIpcMemIfCp_RxChannelStateType, AUTOMATIC, VIPC_APPL_VAR) channel = vIpcMemIfCp_Rx_GetChannel(ChannelId);

   /* #10 Call CopyRxData of the upper layer. */
   pduInfo.SduDataPtr  = channel->BufferDesc.Ptr;
   pduInfo.SduLength   = (PduLengthType)channel->BufferDesc.Size;
   copyResult = vIpcMemIfCp_UL_CopyRxData(vIpcMemIfCp_Rx_GetULId(ChannelId), &pduInfo, &availableBufferSize);

   /* #20 If an error has occurred: */
   if (copyResult != BUFREQ_OK)
   {
      /* #30 Signal the reception abort to the lower layer. */
      (void)vIpcMemIfCp_LL_Rx_Cancel(vIpcMemIfCp_Rx_GetLLId(ChannelId));

      /* #40 Change the channel state to idle and abort processing. */
      channel->State = VIPCMEMIFCP_RX_IDLE;
      continueProcessing = FALSE;
   }
   /* #50 If the call was successful: */
   else
   {
      /* #60 Decrease the rest size. */
      channel->RestSize -= channel->BufferDesc.Size;

      /* #70 If the message was not fully copied yet: */
      if (channel->RestSize != 0)
      {
         /* #80 Request next buffer segment from lower layer. */
         (void)vIpcMemIfCp_LL_Rx_GetBuffer(vIpcMemIfCp_Rx_GetLLId(ChannelId), channel->RestSize, &(channel->BufferDesc));

         /* #90 If the upper layer has not sufficient space for the next call: */
         if (availableBufferSize < channel->BufferDesc.Size)
         {
            /* #100 Change the channel state to waiting for buffer and suspend processing. */
            channel->State = VIPCMEMIFCP_RX_WAIT_FOR_BUFFER;
            continueProcessing = FALSE;
         }
      }
      /* #110 Otherwise: */
      else
      {
         /* #120 Confirm the reception to the lower layer. */
         (void)vIpcMemIfCp_LL_Rx_Confirm(vIpcMemIfCp_Rx_GetLLId(ChannelId));

         /* #130 Indicate to reception end to the upper layer. */
         vIpcMemIfCp_UL_RxIndication(vIpcMemIfCp_Rx_GetULId(ChannelId), E_OK);

         /* #140 Change the channel state to idle and finish processing. */
         channel->State = VIPCMEMIFCP_RX_IDLE;
         continueProcessing = FALSE;
      }
   }
   return continueProcessing;
} /* vIpcMemIfCp_Rx_ProcessCopyState */                                                                                 /* PRQA S 6050 */ /* MD_MSR_STCAL */

# define VIPCMEMIFCP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* VIPCMEMIFCP_RXINT_H */

/**********************************************************************************************************************
 *  END OF FILE: vIpcMemIfCp_RxInt.h
 *********************************************************************************************************************/
