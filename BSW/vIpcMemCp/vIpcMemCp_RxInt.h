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
 *      \details  Contains the declaration and definition of the module's internal functions.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to vIpcMemCp.h.
 *********************************************************************************************************************/

#if !defined (VIPCMEMCP_RXINT_H)                                                                                        /* PRQA S 0883 */ /* MD_vIpcMemCp_0883 */
# define VIPCMEMCP_RXINT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "vIpcMemCp_Int.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# if !defined (VIPCMEMCP_LOCAL)
#  define VIPCMEMCP_LOCAL static
# endif

# if !defined (VIPCMEMCP_LOCAL_INLINE)
#  define VIPCMEMCP_LOCAL_INLINE LOCAL_INLINE
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

# define VIPCMEMCP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * vIpcMemCp_Rx_GetNrOfChannels()
 *********************************************************************************************************************/
/*! \brief         Returns the number of channels from the receive configuration.
 *  \details       -
 *  \return        Number of channels.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(vIpcMemCp_ChannelIndexType, VIPCMEMCP_CODE)
vIpcMemCp_Rx_GetNrOfChannels
(
   void
);

/**********************************************************************************************************************
 * vIpcMemCp_Rx_GetBufferStart()
 *********************************************************************************************************************/
/*! \brief         Returns the pointer to the start of the receive queue buffer.
 *  \details       -
 *  \param[in]     Channel                  Channel to query.
 *  \return        Pointer to the start of the buffer.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(P2VAR(uint8, AUTOMATIC, VIPCMEMCP_VAR_NOINIT), VIPCMEMCP_CODE)
vIpcMemCp_Rx_GetBufferStart
(
   vIpcMemCp_ChannelIndexType Channel
);

/**********************************************************************************************************************
 * vIpcMemCp_Rx_GetBufferSize()
 *********************************************************************************************************************/
/*! \brief         Returns the size of the receive queue buffer.
 *  \details       -
 *  \param[in]     Channel                  Channel to query.
 *  \return        Size of the buffer.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(uint32, VIPCMEMCP_CODE)
vIpcMemCp_Rx_GetBufferSize
(
   vIpcMemCp_ChannelIndexType Channel
);

/**********************************************************************************************************************
 * vIpcMemCp_Rx_GetQueue()
 *********************************************************************************************************************/
/*! \brief         Returns the pointer to the management data of the receive queue.
 *  \details       -
 *  \param[in]     Channel                  Channel to query.
 *  \return        Pointer to the queue control structure.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(vIpcMemCp_QueuePtrType, VIPCMEMCP_CODE)
vIpcMemCp_Rx_GetQueue
(
   vIpcMemCp_ChannelIndexType Channel
);

/**********************************************************************************************************************
 * vIpcMemCp_Rx_QueueIsPending()
 *********************************************************************************************************************/
/*! \brief         Returns if the receive is already pending.
 *  \details       -
 *  \param[in]     Queue                   Queue to check. Parameter must not be NULL_PTR.
 *  \return        TRUE if receive is already pending, FALSE otherwise.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE for different channels
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_Rx_QueueIsPending
(
   P2CONST(vIpcMemCp_QueueType, AUTOMATIC, VIPCMEMCP_CONST) Queue
);

/**********************************************************************************************************************
 * vIpcMemCp_Rx_GetReservedSize()
 *********************************************************************************************************************/
/*! \brief         Returns the size of reserved reception segment.
 *  \details       -
 *  \param[in]     Queue                   Queue to check. Parameter must not be NULL_PTR.
 *  \param[in]     BufferSize              Size of the underlying buffer.
 *  \return        Reserved size in bytes.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE for different channels
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(uint32, VIPCMEMCP_CODE)
vIpcMemCp_Rx_GetReservedSize
(
   P2CONST(vIpcMemCp_QueueType, AUTOMATIC, VIPCMEMCP_CONST) Queue,
   vIpcMemCp_BufferSizeType BufferSize
);

/**********************************************************************************************************************
 * vIpcMemCp_Rx_GetPendingMessageSize()
 *********************************************************************************************************************/
/*! \brief         Returns the message size which was stored into the buffer during vIpcMemCp_Rx_BeginReceive.
 *  \details       -
 *  \param[in]     ChannelNr               Channel index. The plausibility checks must be already done before.
 *  \return        Size of the pending message.
 *  \pre           The channel index was already checked to be valid.
 *  \context       ANY
 *  \reentrant     TRUE for different channels
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(uint32, VIPCMEMCP_CODE)
vIpcMemCp_Rx_GetPendingMessageSize
(
   vIpcMemCp_ChannelIndexType ChannelNr
);

/**********************************************************************************************************************
 * vIpcMemCp_Rx_CheckTotalDataSize()
 *********************************************************************************************************************/
/*! \brief         Checks that the total accumulated reserved size does not exceeds the message size.
 *  \details       -
 *  \param[in]     ChannelNr               Channel index. The plausibility checks must be already done before.
 *  \param[in]     DataSize                The reserved size increment to check.
 *  \return        TRUE if the check fails, FALSE otherwise.
 *  \pre           The channel index was already checked to be valid.
 *  \context       ANY
 *  \reentrant     TRUE for different channels
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_Rx_CheckTotalDataSize
(
   vIpcMemCp_ChannelIndexType ChannelNr,
   uint32 DataSize
);

# define VIPCMEMCP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define VIPCMEMCP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * vIpcMemCp_Rx_GetNrOfChannels()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(vIpcMemCp_ChannelIndexType, VIPCMEMCP_CODE)
vIpcMemCp_Rx_GetNrOfChannels
(
   void
)
{
   return vIpcMemCp_GetSizeOfRxConfig();
} /* vIpcMemCp_Rx_GetNrOfChannels */

/**********************************************************************************************************************
 * vIpcMemCp_Rx_GetBufferStart()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(vIpcMemCp_BufferPtrType, VIPCMEMCP_CODE)
vIpcMemCp_Rx_GetBufferStart
(
   vIpcMemCp_ChannelIndexType Channel
)
{
   return (vIpcMemCp_BufferPtrType)vIpcMemCp_GetQueueOfRxConfig(Channel) + sizeof(vIpcMemCp_QueueType);                 /* PRQA S 0488 */ /* MD_vIpcMemCp_0488 */
} /* vIpcMemCp_Rx_GetBufferStart */

/**********************************************************************************************************************
 * vIpcMemCp_Rx_GetBufferSize()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(uint32, VIPCMEMCP_CODE)
vIpcMemCp_Rx_GetBufferSize
(
   vIpcMemCp_ChannelIndexType Channel
)
{
   return (uint32)(vIpcMemCp_GetBufferSizeOfRxConfig(Channel) - sizeof(vIpcMemCp_QueueType));
} /* vIpcMemCp_Rx_GetBufferSize */

/**********************************************************************************************************************
 * vIpcMemCp_Rx_GetQueue()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(vIpcMemCp_QueuePtrType, VIPCMEMCP_CODE)
vIpcMemCp_Rx_GetQueue
(
   vIpcMemCp_ChannelIndexType Channel
)
{
   return (vIpcMemCp_QueuePtrType)vIpcMemCp_GetQueueOfRxConfig(Channel);                                                /* PRQA S 0310 */ /* MD_vIpcMemCp_0310 */
} /* vIpcMemCp_Rx_GetQueue */

/**********************************************************************************************************************
 * vIpcMemCp_Rx_QueueIsPending()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_Rx_QueueIsPending
(
   P2CONST(vIpcMemCp_QueueType, AUTOMATIC, VIPCMEMCP_CONST) Queue
)
{
   vIpcMemCp_QueuePosType pos = vIpcMemCp_ReadQueuePointer(&(Queue->Tail));

   return (boolean)(pos != Queue->TailReserved);
} /* vIpcMemCp_Rx_QueueIsPending */

/**********************************************************************************************************************
 * vIpcMemCp_Rx_GetReservedSize()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(uint32, VIPCMEMCP_CODE)
vIpcMemCp_Rx_GetReservedSize
(
   P2CONST(vIpcMemCp_QueueType, AUTOMATIC, VIPCMEMCP_CONST) Queue,
   vIpcMemCp_BufferSizeType BufferSize
)
{
   vIpcMemCp_QueuePosType pos = vIpcMemCp_ReadQueuePointer(&(Queue->Tail));

   return vIpcMemCp_QueueGetDistance(Queue->TailReserved, pos, BufferSize);
} /* vIpcMemCp_Rx_GetReservedSize */

/**********************************************************************************************************************
 * vIpcMemCp_Rx_GetPendingMessageSize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VIPCMEMCP_LOCAL_INLINE FUNC(uint32, VIPCMEMCP_CODE)
vIpcMemCp_Rx_GetPendingMessageSize
(
   vIpcMemCp_ChannelIndexType ChannelNr
)
{
   vIpcMemCp_MessageHeaderType header;

   /* #10 Get the queue. */
   vIpcMemCp_QueuePtrType queue   = vIpcMemCp_Rx_GetQueue(ChannelNr);

   /* #20 Get the current Tail position from the queue. */
   vIpcMemCp_QueuePosType curTail = vIpcMemCp_QueueGetCorrectedPos(&(queue->Tail), vIpcMemCp_Rx_GetBufferSize(ChannelNr));

   /* #30 Read the header of the pending message. */
   vIpcMemCp_ReadMessageHeader(vIpcMemCp_Rx_GetBufferStart(ChannelNr) + curTail, &header);                              /* PRQA S 0488 */ /* MD_vIpcMemCp_0488 */

   return header.Size;
} /* vIpcMemCp_Rx_GetPendingMessageSize */

/**********************************************************************************************************************
 * vIpcMemCp_Rx_CheckTotalDataSize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_Rx_CheckTotalDataSize
(
   vIpcMemCp_ChannelIndexType ChannelNr,
   uint32 DataSize
)
{
   /* #10 Calculate the reserved size of the current message. */
   uint32 reservedSize = vIpcMemCp_Rx_GetReservedSize(vIpcMemCp_Rx_GetQueue(ChannelNr), vIpcMemCp_Rx_GetBufferSize(ChannelNr));

   /* #20 Get the message size from the buffer. */
   uint32 messageSize   = vIpcMemCp_Rx_GetPendingMessageSize(ChannelNr);

   /* #30 Return if the increment would exceed the message size. */
   return (boolean)(((reservedSize + DataSize) - sizeof(vIpcMemCp_MessageHeaderType)) > (messageSize));
} /* vIpcMemCp_Rx_CheckTotalDataSize */

# define VIPCMEMCP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* VIPCMEMCP_RXINT_H */

/**********************************************************************************************************************
 *  END OF FILE: vIpcMemCp_RxInt.h
 *********************************************************************************************************************/
