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

#if !defined (VIPCMEMIFCP_TXINT_H)                                                                                      /* PRQA S 0883 */ /* MD_vIpcMemIfCp_0883 */
# define VIPCMEMIFCP_TXINT_H

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
 *  vIpcMemIfCp_Tx_GetChannel()
 *********************************************************************************************************************/
/*! \brief       Returns the channel state data structure associated with the channel ID.
 *  \details     -
 *  \param[in]   ChannelId                 Channel index. It has to be valid.
 *  \return      Channel state data structure.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(P2VAR(vIpcMemIfCp_TxChannelStateType, AUTOMATIC, VIPC_APPL_VAR), VIPCMEMIFCP_CODE)
vIpcMemIfCp_Tx_GetChannel
(
   PduIdType ChannelId
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_GetNotificationCallback()
 *********************************************************************************************************************/
/*! \brief       Returns the configured user's notification callback function pointer.
 *  \details     -
 *  \return      The function pointer to the callback.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(vIpcMemIfCp_UserTxNotificationCallbackType, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Tx_GetNotificationCallback
(
   void
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_GetNrOfChannels()
 *********************************************************************************************************************/
/*! \brief       Returns the number of the configured Tx channels.
 *  \details     -
 *  \return      Number of Tx channels.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(PduIdType, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Tx_GetNrOfChannels
(
   void
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_GetLLId()
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
vIpcMemIfCp_Tx_GetLLId
(
   PduIdType ChannelId
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_GetULId()
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
vIpcMemIfCp_Tx_GetULId
(
   PduIdType ChannelId
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_InitChannel()
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
vIpcMemIfCp_Tx_InitChannel
(
   P2VAR(vIpcMemIfCp_TxChannelStateType, AUTOMATIC, VIPC_APPL_VAR) Channel
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_StartTransmission()
 *********************************************************************************************************************/
/*! \brief       Attempts to start a transmission of a given size..
 *  \details     -
 *  \param[in]   ChannelId                 Channel index. It has to be valid.
 *  \param[in]   DataSize                  Message size in bytes.
 *  \return      E_OK                      Transmission request accepted.
 *  \return      E_NOT_OK                  Transmission request failed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels.
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(Std_ReturnType, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Tx_StartTransmission
(
   PduIdType ChannelId,
   PduLengthType DataSize
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_Process()
 *********************************************************************************************************************/
/*! \brief       Performs a full processing of a transmission on the channel.
 *  \details     If the function is called NOT from a MainFunction, the confirmation to the upper layer will be
 *               delayed until next MainFunction call.
 *  \param[in]   ChannelId                 Channel index. It has to be valid.
 *  \param[in]   MainFunctionContext       TRUE  if the function was called from a MainFunction.
 *                                         FALSE otherwise.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE for different channels.
 *  \synchronous TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(void, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Tx_Process
(
   PduIdType ChannelId,
   boolean MainFunctionContext
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_ProcessCopyState()
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
vIpcMemIfCp_Tx_ProcessCopyState
(
   PduIdType ChannelId
);

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_ProcessConfirmationState()
 *********************************************************************************************************************/
/*! \brief       Performs the step of the processing associated with the pending confirmation to the upper layer.
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
vIpcMemIfCp_Tx_ProcessConfirmationState
(
   PduIdType ChannelId
);

/**********************************************************************************************************************
 * vIpcMemIfCp_Tx_SendNotification()
 *********************************************************************************************************************/
/*! \brief         Calls the configured notification callback after a successful transmission.
 *  \details       -
 *  \param[in]     ChannelId               Channel number of the upper layer to identify the transmission.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE for different channels
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(void, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Tx_SendNotification
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
 *  vIpcMemIfCp_Tx_GetChannel()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(P2VAR(vIpcMemIfCp_TxChannelStateType, AUTOMATIC, VIPC_APPL_VAR), VIPCMEMIFCP_CODE)
vIpcMemIfCp_Tx_GetChannel
(
   PduIdType ChannelId
)
{
   return &(vIpcMemIfCp_GetTxChannelData(ChannelId));
} /* vIpcMemIfCp_Tx_GetChannel */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_GetNotificationCallback()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(vIpcMemIfCp_UserTxNotificationCallbackType, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Tx_GetNotificationCallback
(
   void
)
{
   return vIpcMemIfCp_GetUserTxNotificationFctPtr();
} /* vIpcMemIfCp_Tx_GetNotificationCallback */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_GetNrOfChannels()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(PduIdType, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Tx_GetNrOfChannels
(
   void
)
{
   return vIpcMemIfCp_GetSizeOfTxConfig();
} /* vIpcMemIfCp_Tx_GetNrOfChannels */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_GetLLId()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(vIpcMemCp_ChannelIndexType, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Tx_GetLLId
(
   PduIdType ChannelId
)
{
   return vIpcMemIfCp_GetLowerLayerIdOfTxConfig(ChannelId);
} /* vIpcMemIfCp_Tx_GetLLId */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_GetULId()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(PduIdType, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Tx_GetULId
(
   PduIdType ChannelId
)
{
   return vIpcMemIfCp_GetUpperLayerIdOfTxConfig(ChannelId);
} /* vIpcMemIfCp_Tx_GetULId */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_InitChannel()
 *********************************************************************************************************************/
VIPCMEMIFCP_LOCAL_INLINE FUNC(void, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Tx_InitChannel
(
   P2VAR(vIpcMemIfCp_TxChannelStateType, AUTOMATIC, VIPC_APPL_VAR) Channel
)
{
   Channel->State = VIPCMEMIFCP_TX_IDLE;
} /* vIpcMemIfCp_Tx_InitChannel */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_StartTransmission()
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
VIPCMEMIFCP_LOCAL_INLINE FUNC(Std_ReturnType, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Tx_StartTransmission
(
   PduIdType ChannelId,
   PduLengthType DataSize
)
{
   Std_ReturnType status = E_NOT_OK;
   P2VAR(vIpcMemIfCp_TxChannelStateType, AUTOMATIC, VIPC_APPL_VAR) channel = vIpcMemIfCp_Tx_GetChannel(ChannelId);

   /* #10 If the channel is idle: */
   if (channel->State == VIPCMEMIFCP_TX_IDLE)
   {
      /* #20 Request the transmission from lower layer. */
      Std_ReturnType transmitResult = vIpcMemIfCp_LL_Tx_BeginTransmit(vIpcMemIfCp_Tx_GetLLId(ChannelId), DataSize);

      /* #30 If the transmission request was successful: */
      if (transmitResult == E_OK)
      {
         /* #40 Request the buffer segment from the lower layer. */
         channel->RestSize = DataSize;
         (void)vIpcMemIfCp_LL_Tx_GetBuffer(vIpcMemIfCp_Tx_GetLLId(ChannelId), channel->RestSize, &(channel->BufferDesc));

         /* #50 Update the channel state. */
         channel->State = VIPCMEMIFCP_TX_COPY_PENDING;

         status = E_OK;
      }
   }

   return status;
} /* vIpcMemIfCp_Tx_StartTransmission */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_Process()
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
VIPCMEMIFCP_LOCAL_INLINE FUNC(void, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Tx_Process
(
   PduIdType ChannelId,
   boolean MainFunctionContext
)
{
   P2VAR(vIpcMemIfCp_TxChannelStateType, AUTOMATIC, VIPC_APPL_VAR) channel = vIpcMemIfCp_Tx_GetChannel(ChannelId);
   boolean continueProcessing = TRUE;

   do
   {
      switch(channel->State)
      {
      case VIPCMEMIFCP_TX_IDLE:
         /* #10 The channel in idle state can be ignored. */
         continueProcessing = FALSE;
         break;

      case VIPCMEMIFCP_TX_COPY_PENDING:
         /* #20 Process the copy pending state. */
         continueProcessing = vIpcMemIfCp_Tx_ProcessCopyState(ChannelId);
         break;

      case VIPCMEMIFCP_TX_CONFIRMATION_PENDING:
         /* #30 If the function was called from MainFunction context: */
         if (MainFunctionContext == TRUE)
         {
            /* #40 Process the confirmation pending state. */
            (void)vIpcMemIfCp_Tx_ProcessConfirmationState(ChannelId);
         }

         continueProcessing = FALSE;
         break;

      default:
         /* Invalid state. Reset to idle state. */
         channel->State = VIPCMEMIFCP_TX_IDLE;
         continueProcessing = FALSE;
         break;
      }
      /* #50 Loop while further processing is possible. */
   } while (continueProcessing == TRUE);
} /* vIpcMemIfCp_Tx_Process */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_ProcessCopyState()
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
vIpcMemIfCp_Tx_ProcessCopyState
(
   PduIdType ChannelId
)
{
   PduInfoType pduInfo;
   PduLengthType availableData;
   BufReq_ReturnType copyResult;
   boolean continueProcessing = TRUE;
   P2VAR(vIpcMemIfCp_TxChannelStateType, AUTOMATIC, VIPC_APPL_VAR) channel = vIpcMemIfCp_Tx_GetChannel(ChannelId);

   /* #10 Call CopyTxData of the upper layer. */
   pduInfo.SduDataPtr  = channel->BufferDesc.Ptr;
   pduInfo.SduLength   = (PduLengthType)channel->BufferDesc.Size;
   copyResult = vIpcMemIfCp_UL_CopyTxData(vIpcMemIfCp_Tx_GetULId(ChannelId), &pduInfo, NULL_PTR, &availableData);

   /* #20 If an error has occurred: */
   if ((copyResult != BUFREQ_OK) && (copyResult != BUFREQ_E_BUSY))
   {
      /* #30 Signal the transmission abort to the lower layer. */
      (void)vIpcMemIfCp_LL_Tx_Cancel(vIpcMemIfCp_Tx_GetLLId(ChannelId));

      /* #40 Change the channel state to idle and abort processing. */
      channel->State = VIPCMEMIFCP_TX_IDLE;
      continueProcessing = FALSE;
   }
   /* #50 If the upper layer is not yet ready: */
   else if (copyResult == BUFREQ_E_BUSY)
   {
      /* #60 Suspend processing. */
      continueProcessing = FALSE;
   }
   /* #70 If the call was successful: */
   else
   {
      /* #80 Decrease the rest size of the message. */
      channel->RestSize -= channel->BufferDesc.Size;

      /* #90 If the message was not fully copied: */
      if (channel->RestSize != 0)
      {
         /* #100 Request next buffer segment from the lower layer. */
         (void)vIpcMemIfCp_LL_Tx_GetBuffer(vIpcMemIfCp_Tx_GetLLId(ChannelId), channel->RestSize, &(channel->BufferDesc));
      }
      /* #110 Otherwise: */
      else
      {
         /* #120 Confirm the transmission to the lower layer. */
         (void)vIpcMemIfCp_LL_Tx_Confirm(vIpcMemIfCp_Tx_GetLLId(ChannelId));

         /* #130 Change the channel state to queue confirmation to the upper layer. */
         channel->State = VIPCMEMIFCP_TX_CONFIRMATION_PENDING;

         /* #140 Call the notification callback. */
         vIpcMemIfCp_Tx_SendNotification(vIpcMemIfCp_Tx_GetULId(ChannelId));
      }
   }

   return continueProcessing;
} /* vIpcMemIfCp_Tx_ProcessCopyState */                                                                                 /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_ProcessConfirmationState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VIPCMEMIFCP_LOCAL_INLINE FUNC(boolean, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Tx_ProcessConfirmationState
(
   PduIdType ChannelId
)
{
   boolean continueProcessing = FALSE;
   P2VAR(vIpcMemIfCp_TxChannelStateType, AUTOMATIC, VIPC_APPL_VAR) channel = vIpcMemIfCp_Tx_GetChannel(ChannelId);

   /* #10 Change the channel state to idle. */
   channel->State = VIPCMEMIFCP_TX_IDLE;

   /* #20 Signal the upper layer the end of the transmission. */
   vIpcMemIfCp_UL_TxConfirmation(vIpcMemIfCp_Tx_GetULId(ChannelId), E_OK);

   return continueProcessing;
} /* vIpcMemIfCp_Tx_ProcessConfirmationState */

/**********************************************************************************************************************
 *  vIpcMemIfCp_Tx_SendNotification()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VIPCMEMIFCP_LOCAL_INLINE FUNC(void, VIPCMEMIFCP_CODE)
vIpcMemIfCp_Tx_SendNotification
(
   PduIdType ChannelId
)
{
   /* #10 Query the user's callback. */
   vIpcMemIfCp_UserTxNotificationCallbackType userCallback = vIpcMemIfCp_Tx_GetNotificationCallback();

   /* #20 If the callback is not null: */
   if (userCallback != NULL_PTR)
   {
      /* #30 Execute the callback. */
      userCallback(ChannelId);
   }
} /* vIpcMemIfCp_Tx_SendNotification */

# define VIPCMEMIFCP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* VIPCMEMIFCP_TXINT_H */

/**********************************************************************************************************************
 *  END OF FILE: vIpcMemIfCp_TxInt.h
 *********************************************************************************************************************/
