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
 *      \details  Contains the declaration and definition of the module's internal Tx functions.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to vIpcMemCp.h.
 *********************************************************************************************************************/

#if !defined (VIPCMEMCP_TXINT_H)                                                                                        /* PRQA S 0883 */ /* MD_vIpcMemCp_0883 */
# define VIPCMEMCP_TXINT_H

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
 * vIpcMemCp_Tx_GetNrOfChannels()
 *********************************************************************************************************************/
/*! \brief         Returns the number of channels from the transmit configuration.
 *  \details       -
 *  \return        Number of channels.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(vIpcMemCp_ChannelIndexType, VIPCMEMCP_CODE)
vIpcMemCp_Tx_GetNrOfChannels
(
   void
);

/**********************************************************************************************************************
 * vIpcMemCp_Tx_GetBufferStart()
 *********************************************************************************************************************/
/*! \brief         Returns the pointer to the start of the transmit queue buffer.
 *  \details       -
 *  \param[in]     Channel                  Channel to query.
 *  \return        Pointer to the start of the buffer.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(P2VAR(uint8, AUTOMATIC, VIPCMEMCP_VAR_NOINIT), VIPCMEMCP_CODE)
vIpcMemCp_Tx_GetBufferStart
(
   vIpcMemCp_ChannelIndexType Channel
);

/**********************************************************************************************************************
 * vIpcMemCp_Tx_GetBufferSize()
 *********************************************************************************************************************/
/*! \brief         Returns the size of the transmit queue buffer.
 *  \details       -
 *  \param[in]     Channel                  Channel to query.
 *  \return        Size of the buffer.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(uint32, VIPCMEMCP_CODE)
vIpcMemCp_Tx_GetBufferSize
(
   vIpcMemCp_ChannelIndexType Channel
);

/**********************************************************************************************************************
 * vIpcMemCp_Tx_GetQueue()
 *********************************************************************************************************************/
/*! \brief         Returns the pointer to the management data of the transmit queue.
 *  \details       -
 *  \param[in]     Channel                  Channel to query.
 *  \return        Pointer to the queue control structure.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(vIpcMemCp_QueuePtrType, VIPCMEMCP_CODE)
vIpcMemCp_Tx_GetQueue
(
   vIpcMemCp_ChannelIndexType Channel
);

/**********************************************************************************************************************
 * vIpcMemCp_Tx_QueueIsPending()
 *********************************************************************************************************************/
/*! \brief         Returns if the transmit is already pending.
 *  \details       -
 *  \param[in]     Queue                   Queue to check. Parameter must not be NULL_PTR.
 *  \return        TRUE if transmit is already pending, FALSE otherwise.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE for different channels
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_Tx_QueueIsPending
(
   P2CONST(vIpcMemCp_QueueType, AUTOMATIC, VIPCMEMCP_CONST) Queue
);

/**********************************************************************************************************************
 * vIpcMemCp_Tx_GetReservedSize()
 *********************************************************************************************************************/
/*! \brief         Returns the size of reserved transmission segment.
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
vIpcMemCp_Tx_GetReservedSize
(
   P2CONST(vIpcMemCp_QueueType, AUTOMATIC, VIPCMEMCP_CONST) Queue,
   vIpcMemCp_BufferSizeType BufferSize
);

/**********************************************************************************************************************
 * vIpcMemCp_Tx_GetPendingMessageSize()
 *********************************************************************************************************************/
/*! \brief         Returns the message size which was stored into the buffer during vIpcMemCp_Tx_BeginTransmit.
 *  \details       -
 *  \param[in]     ChannelNr               Channel index. The plausibility checks must be already done before.
 *  \return        Size of the pending message.
 *  \pre           The channel index was already checked to be valid.
 *  \context       ANY
 *  \reentrant     TRUE for different channels
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(uint32, VIPCMEMCP_CODE)
vIpcMemCp_Tx_GetPendingMessageSize
(
   vIpcMemCp_ChannelIndexType ChannelNr
);

/**********************************************************************************************************************
 * vIpcMemCp_Tx_CheckTotalDataSize()
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
vIpcMemCp_Tx_CheckTotalDataSize
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
 * vIpcMemCp_Tx_GetNrOfChannels()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(vIpcMemCp_ChannelIndexType, VIPCMEMCP_CODE)
vIpcMemCp_Tx_GetNrOfChannels
(
   void
)
{
   return vIpcMemCp_GetSizeOfTxConfig();
} /* vIpcMemCp_Tx_GetNrOfChannels */

/**********************************************************************************************************************
 * vIpcMemCp_Tx_GetBufferStart()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(vIpcMemCp_BufferPtrType, VIPCMEMCP_CODE)
vIpcMemCp_Tx_GetBufferStart
(
   vIpcMemCp_ChannelIndexType Channel
)
{
   return (vIpcMemCp_BufferPtrType)vIpcMemCp_GetQueueOfTxConfig(Channel) + sizeof(vIpcMemCp_QueueType);                 /* PRQA S 0488 */ /* MD_vIpcMemCp_0488 */
} /* vIpcMemCp_Tx_GetBufferStart */

/**********************************************************************************************************************
 * vIpcMemCp_Tx_GetBufferSize()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(uint32, VIPCMEMCP_CODE)
vIpcMemCp_Tx_GetBufferSize
(
   vIpcMemCp_ChannelIndexType Channel
)
{
   return (uint32)(vIpcMemCp_GetBufferSizeOfTxConfig(Channel) - sizeof(vIpcMemCp_QueueType));
} /* vIpcMemCp_Tx_GetBufferSize */

/**********************************************************************************************************************
 * vIpcMemCp_Tx_GetQueue()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(vIpcMemCp_QueuePtrType, VIPCMEMCP_CODE)
vIpcMemCp_Tx_GetQueue
(
   vIpcMemCp_ChannelIndexType Channel
)
{
   return (vIpcMemCp_QueuePtrType)vIpcMemCp_GetQueueOfTxConfig(Channel);                                                /* PRQA S 0310 */ /* MD_vIpcMemCp_0310 */
} /* vIpcMemCp_Tx_GetQueue */

/**********************************************************************************************************************
 * vIpcMemCp_Tx_QueueIsPending()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_Tx_QueueIsPending
(
   P2CONST(vIpcMemCp_QueueType, AUTOMATIC, VIPCMEMCP_CONST) Queue
)
{
   vIpcMemCp_QueuePosType pos = vIpcMemCp_ReadQueuePointer(&(Queue->Head));

   return (boolean)(pos != Queue->HeadReserved);
} /* vIpcMemCp_Tx_QueueIsPending */

/**********************************************************************************************************************
 * vIpcMemCp_Tx_GetReservedSize()
 *********************************************************************************************************************/
VIPCMEMCP_LOCAL_INLINE FUNC(uint32, VIPCMEMCP_CODE)
vIpcMemCp_Tx_GetReservedSize
(
   P2CONST(vIpcMemCp_QueueType, AUTOMATIC, VIPCMEMCP_CONST) Queue,
   vIpcMemCp_BufferSizeType BufferSize
)
{
   vIpcMemCp_QueuePosType pos = vIpcMemCp_ReadQueuePointer(&(Queue->Head));

   return vIpcMemCp_QueueGetDistance(Queue->HeadReserved, pos, BufferSize);
} /* vIpcMemCp_Tx_GetReservedSize */

/**********************************************************************************************************************
 * vIpcMemCp_Tx_GetPendingMessageSize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VIPCMEMCP_LOCAL_INLINE FUNC(uint32, VIPCMEMCP_CODE)
vIpcMemCp_Tx_GetPendingMessageSize
(
   vIpcMemCp_ChannelIndexType ChannelNr
)
{
   vIpcMemCp_MessageHeaderType header;

   /* #10 Get the queue. */
   vIpcMemCp_QueuePtrType queue   = vIpcMemCp_Tx_GetQueue(ChannelNr);

   /* #20 Get the current Head position from the queue. */
   vIpcMemCp_QueuePosType curHead = vIpcMemCp_QueueGetCorrectedPos(&(queue->Head), vIpcMemCp_Tx_GetBufferSize(ChannelNr));

   /* #30 Read the header of the pending message. */
   vIpcMemCp_ReadMessageHeader(vIpcMemCp_Tx_GetBufferStart(ChannelNr) + curHead, &header);                              /* PRQA S 0488 */ /* MD_vIpcMemCp_0488 */

   return header.Size;
} /* vIpcMemCp_Tx_GetPendingMessageSize */

/**********************************************************************************************************************
 * vIpcMemCp_Tx_CheckTotalDataSize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VIPCMEMCP_LOCAL_INLINE FUNC(boolean, VIPCMEMCP_CODE)
vIpcMemCp_Tx_CheckTotalDataSize
(
   vIpcMemCp_ChannelIndexType ChannelNr,
   uint32 DataSize
)
{
   /* #10 Calculate the reserved size of the current message. */
   uint32 reservedSize = vIpcMemCp_Tx_GetReservedSize(vIpcMemCp_Tx_GetQueue(ChannelNr), vIpcMemCp_Tx_GetBufferSize(ChannelNr));

   /* #20 Get the message size from the buffer. */
   uint32 messageSize   = vIpcMemCp_Tx_GetPendingMessageSize(ChannelNr);

   /* #30 Return if the increment would exceed the message size. */
   return (boolean)(((reservedSize + DataSize) - sizeof(vIpcMemCp_MessageHeaderType)) > (messageSize));
} /* vIpcMemCp_Tx_CheckTotalDataSize */

# define VIPCMEMCP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* VIPCMEMCP_TXINT_H */

/**********************************************************************************************************************
 *  END OF FILE: vIpcMemCp_TxInt.h
 *********************************************************************************************************************/
