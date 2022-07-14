/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector
 * Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set
 * out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file
 *        \brief  vIpc_PQ source file
 *
 *      \details  Todo: Fill in details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define VIPC_PQ_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vIpc_PQ.h"
#include "SchM_vIpc.h"
#include "vstdlib.h"

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

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vIpc_Prio_GetPriority()
 *********************************************************************************************************************/
/*! \brief       Calculates and returns the priority of the specified SDU.
 *  \details     -
 *  \return      The priority of the specified SDU.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         The requested TxSduId is valid.
 *  \ingroup     priority
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(vIpc_PrioType, VIPC_CODE) vIpc_Prio_GetPriority(PduIdType TxSduId);

/**********************************************************************************************************************
 *  vIpc_Prio_HasFirstConHigherPrio()
 *********************************************************************************************************************/
/*! \brief       Compares two Id connection about their priority.
 *  \details     Compares two Id connection about their priority and indicates if the first connection Id has a
 *               higher priority.
 *  \param       FirstTxConId      The first Tx connection Id.
 *               SecondTxConId     The second Tx connection Id.
 *  \return      FALSE             The second Tx connection Id has equal or higher priority.
 *               TRUE              The first Tx connection Id has higher priority.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         -
 *  \ingroup     priority
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(boolean, VIPC_CODE) vIpc_Prio_HasFirstConHigherPrio(PduIdType FirstTxConId,
                                                                           PduIdType SecondTxConId);

/**********************************************************************************************************************
 *  vIpc_PQ_IsQueueEmpty()
 *********************************************************************************************************************/
/*! \brief       Checks if the queue of the specified Tx channel is currently empty.
 *  \details     -
 *  \param       TxChanId          The Tx channel for which the queue is checked.
 *  \return      FALSE             The queue is not empty.
 *               TRUE              The queue is empty.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         -
 *  \ingroup     priority
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(boolean, VIPC_CODE) vIpc_PQ_IsQueueEmpty(PduIdType TxChanId);

/**********************************************************************************************************************
 *  vIpc_PQ_Insert_FifoBased()
 *********************************************************************************************************************/
/*! \brief       Inserts the specified connection to FIFO queue.
 *  \details     -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         The requested TxSduId is valid.
 *  \ingroup     PQ
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_PQ_Insert_FifoBased(PduIdType ConnectionId);

/**********************************************************************************************************************
 *  vIpc_PQ_Insert_PrioBased()
 *********************************************************************************************************************/
/*! \brief       Inserts the specified connection to priority based queue.
 *  \details     -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         The requested TxSduId is valid.
 *  \ingroup     PQ
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_PQ_Insert_PrioBased(PduIdType ConnectionId);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#define VIPC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * vIpc_Prio_GetPriority()
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(vIpc_PrioType, VIPC_CODE) vIpc_Prio_GetPriority(PduIdType TxSduId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return the priority of specified connection as configured. */
  return vIpc_XCfg_GetPriorityOfTxConnections(TxSduId);
} /* vIpc_Prio_GetPriority */

/**********************************************************************************************************************
 * vIpc_PQ_IsQueueEmpty()
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(boolean, VIPC_CODE) vIpc_PQ_IsQueueEmpty(PduIdType TxChanId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Indicates if the queue of specified Tx channel is empty. */
  return (boolean)(vIpc_XCfg_TxQI_GetHighPrioIdx(TxChanId) >= vIpc_XCfg_GetSizeOfTxConnections());
} /* vIpc_PQ_IsQueueEmpty */

/**********************************************************************************************************************
 * vIpc_Prio_HasFirstConHigherPrio()
 *********************************************************************************************************************/
VIPC_LOCAL_INLINE FUNC(boolean, VIPC_CODE) vIpc_Prio_HasFirstConHigherPrio(PduIdType FirstTxConId,
                                                                           PduIdType SecondTxConId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Indicates if the first specified Tx connection Id has a greater priority (higher value) than the second
   *     specified Tx connection Id. */
  return (boolean)(vIpc_Prio_GetPriority(FirstTxConId) > vIpc_XCfg_TxQ_GetPrio(SecondTxConId));
} /* vIpc_Prio_HasFirstConHigherPrio */

/**********************************************************************************************************************
 * vIpc_PQ_Insert_PrioBased()
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
 *
 *
 *
 */
VIPC_LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_PQ_Insert_PrioBased(PduIdType ConnectionId)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType channelId = vIpc_XCfg_GetTxChannelIdxOfTxConnections(ConnectionId);
  CONST(uint32, VIPC_CONST) numConIdsOfChan = vIpc_XCfg_GetNumConnectionsOfTxChannel(channelId);
  CONST(uint32, VIPC_CONST) invalidConId = vIpc_XCfg_GetSizeOfTxConnections();
  vIpc_PrioType newPrio;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section. */
  SchM_Enter_vIpc_VIPC_EXCLUSIVE_AREA_TX_QUEUE();

  newPrio = vIpc_Prio_GetPriority(ConnectionId);

  /* #20 If the queue is currently empty,
   *       Enqueue current connection ID and set the high and low indices to this entry. */
  if (vIpc_PQ_IsQueueEmpty(channelId) == TRUE)
  {
    vIpc_XCfg_TxQ_SetNextLowerIdx(ConnectionId, invalidConId);
    vIpc_XCfg_TxQ_SetNextHigherIdx(ConnectionId, invalidConId);
    vIpc_XCfg_TxQ_SetPrio(ConnectionId, newPrio);
    vIpc_XCfg_TxQI_SetHighPrioIdx(channelId, ConnectionId);
    vIpc_XCfg_TxQI_SetLowPrioIdx(channelId, ConnectionId);
  }
  /* #30 Otherwise: */
  else
  {
    uint32 currentQueuedConIdx = vIpc_XCfg_TxQI_GetHighPrioIdx(channelId);
    uint32 priorQueuedConIdx   = currentQueuedConIdx;
    vIpc_ConAddressType i;

    /* #40 Iterate over all enqueued connections of current channel from highest to lowest priority. */
    for (i = 0; i < numConIdsOfChan; i++)
    {
      /* #50 If the enqueued connection has lower priority than the new connection,
       *       enqueue prior the enqueued connection. */
      if (vIpc_Prio_HasFirstConHigherPrio(ConnectionId, (PduIdType)currentQueuedConIdx) == TRUE)
      {
        uint32 nextHigherIdxForConId;
        /* #60 If this is the first queued connection, set higher prio index of new connection to invalid.
         *     Otherwise, set higher prio index of new connection to prior enqueued connection. */
        if (priorQueuedConIdx == currentQueuedConIdx)
        {
          nextHigherIdxForConId = invalidConId;
        }
        else
        {
          nextHigherIdxForConId = priorQueuedConIdx;
        }
        vIpc_XCfg_TxQ_SetNextHigherIdx(ConnectionId, nextHigherIdxForConId);
        vIpc_XCfg_TxQ_SetNextHigherIdx((PduIdType)currentQueuedConIdx, ConnectionId);
        vIpc_XCfg_TxQ_SetNextLowerIdx(ConnectionId, currentQueuedConIdx);
        vIpc_XCfg_TxQ_SetPrio(ConnectionId, newPrio);

        /* #70 If the new enqueued connection has highest priority,
         *       move high prio pointer to this connection.
         *     Otherwise, set lower prio index of previous queued connection to new enqueued connection. */
        if (currentQueuedConIdx == vIpc_XCfg_TxQI_GetHighPrioIdx(channelId))
        {
          vIpc_XCfg_TxQI_SetHighPrioIdx(channelId, (uint32)ConnectionId);
        }
        else
        {
          vIpc_XCfg_TxQ_SetNextLowerIdx((PduIdType)priorQueuedConIdx, (uint32)ConnectionId);
        }

        break;
      }
      else
      {
        /* #80 Otherwise:
         *       If the current queued connection is the last element in queue,
         *         enqueue new connection as new last element.
         *       Otherwise, remember previous enqueued connection and get next lower prior connection. */
        priorQueuedConIdx = currentQueuedConIdx;
        currentQueuedConIdx = vIpc_XCfg_TxQ_GetNextLowerIdx((PduIdType)currentQueuedConIdx);

        if (currentQueuedConIdx >= invalidConId)
        {
          vIpc_XCfg_TxQ_SetNextLowerIdx((PduIdType)priorQueuedConIdx, ConnectionId);
          vIpc_XCfg_TxQ_SetNextLowerIdx((PduIdType)ConnectionId, invalidConId);
          vIpc_XCfg_TxQ_SetNextHigherIdx((PduIdType)ConnectionId, priorQueuedConIdx);
          vIpc_XCfg_TxQ_SetPrio((PduIdType)ConnectionId, newPrio);

          vIpc_XCfg_TxQI_SetLowPrioIdx(channelId, ConnectionId);
          break;
        }
      }
    }
  }

  /* #90 Leave critical section. */
  SchM_Exit_vIpc_VIPC_EXCLUSIVE_AREA_TX_QUEUE();
} /* vIpc_PQ_Insert_PrioBased */  /* PRQA 6050 1 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 * vIpc_PQ_Insert_FifoBased()
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
VIPC_LOCAL_INLINE FUNC(void, VIPC_CODE) vIpc_PQ_Insert_FifoBased(PduIdType ConnectionId)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType channelId = vIpc_XCfg_GetTxChannelIdxOfTxConnections(ConnectionId);
  CONST(uint32, VIPC_CONST) invalidConId = vIpc_XCfg_GetSizeOfTxConnections();
  uint32 nextHigherIdxForConId = invalidConId;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section. */
  SchM_Enter_vIpc_VIPC_EXCLUSIVE_AREA_TX_QUEUE();

  /* #20 If the queue is currently empty */
  if (vIpc_PQ_IsQueueEmpty(channelId))
  {
    /* #21 Then Enqueue current connection ID and set the high and low indices to this entry. */
    vIpc_XCfg_TxQI_SetHighPrioIdx(channelId, ConnectionId);
  }
  else
  {
    /* #30 Otherwise add the new connection at the end of queue. */
    nextHigherIdxForConId = vIpc_XCfg_TxQI_GetLowPrioIdx(channelId);

    vIpc_XCfg_TxQ_SetNextLowerIdx((PduIdType)nextHigherIdxForConId, ConnectionId);
  }

  vIpc_XCfg_TxQ_SetNextHigherIdx(ConnectionId, nextHigherIdxForConId);
  vIpc_XCfg_TxQ_SetNextLowerIdx(ConnectionId, invalidConId);
  vIpc_XCfg_TxQI_SetLowPrioIdx(channelId, ConnectionId);

  /* #40 Leave critical section. */
  SchM_Exit_vIpc_VIPC_EXCLUSIVE_AREA_TX_QUEUE();
} /* vIpc_PQ_Insert_FifoBased */  /* PRQA 6050 1 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * vIpc_PQ_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, VIPC_CODE) vIpc_PQ_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType channelId;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all queues and their indices. */
  for (channelId = 0; channelId < vIpc_XCfg_GetSizeOfTxChannel(); channelId++)
  {
    vIpc_XCfg_TxQI_InitHighAndLowPrioIdx(channelId);
  }
  vIpc_XCfg_TxQ_InitData();
} /* vIpc_PQ_Init */

/**********************************************************************************************************************
 * vIpc_PQ_Insert()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, VIPC_CODE) vIpc_PQ_Insert(PduIdType ConnectionId)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType channelId = vIpc_XCfg_GetTxChannelIdxOfTxConnections(ConnectionId);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Trigger queuing of connection depending on the corresponding TxChannel's queuing algorithm. */
  if (vIpc_XCfg_GetAlgorithmOfTxChannel(channelId) == VIPC_FIFO)
  {
    vIpc_PQ_Insert_FifoBased(ConnectionId);
  }
  else
  {
    vIpc_PQ_Insert_PrioBased(ConnectionId);
  }
} /* vIpc_PQ_Insert */

/**********************************************************************************************************************
 * vIpc_PQ_Peek()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, VIPC_CODE) vIpc_PQ_Peek(PduIdType TxChanId,
                                             CONSTP2VAR(PduIdType, AUTOMATIC, VIPC_APPL_VAR) TxConIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint32 highPrioConId = (PduIdType)vIpc_XCfg_TxQI_GetHighPrioIdx(TxChanId);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If there is at least one connection enqueued, get the connection with highest priority. */
  if (highPrioConId < vIpc_XCfg_GetSizeOfTxConnections())
  {
    *TxConIdPtr = (PduIdType)highPrioConId;
    retVal = E_OK;
  }

  return retVal;
} /* vIpc_PQ_Peek */

/**********************************************************************************************************************
 * vIpc_PQ_Remove()
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
FUNC(void, VIPC_CODE) vIpc_PQ_Remove(PduIdType TxConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduIdType channelId = vIpc_XCfg_GetTxChannelIdxOfTxConnections(TxConId);
  uint32 invalidConId = vIpc_XCfg_GetSizeOfTxConnections();
  uint32 nextHigherIdx;
  uint32 nextLowerIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section. */
  SchM_Enter_vIpc_VIPC_EXCLUSIVE_AREA_TX_QUEUE();

  nextHigherIdx = vIpc_XCfg_TxQ_GetNextHigherIdx(TxConId);
  nextLowerIdx = vIpc_XCfg_TxQ_GetNextLowerIdx(TxConId);

  /* #20 If there is only one enqueued connection, set queue pointers to invalid. */
  if ((nextLowerIdx == invalidConId)
   && (nextHigherIdx == invalidConId))
  {
    vIpc_XCfg_TxQI_SetHighPrioIdx(channelId, invalidConId);
    vIpc_XCfg_TxQI_SetLowPrioIdx(channelId, invalidConId);
  }
  /* #30 Otherwise if it is the first enqueued connection, set next highest connection as highest. */
  else if (nextHigherIdx == invalidConId)
  {
    vIpc_XCfg_TxQI_SetHighPrioIdx(channelId, vIpc_XCfg_TxQ_GetNextLowerIdx(TxConId));
    vIpc_XCfg_TxQ_SetNextHigherIdx((PduIdType)vIpc_XCfg_TxQ_GetNextLowerIdx(TxConId), invalidConId);
  }
  /* #40 Otherwise if it is the last enqueued connection, set next lower connection as lowest. */
  else if (nextLowerIdx == invalidConId)
  {
    vIpc_XCfg_TxQI_SetLowPrioIdx(channelId, vIpc_XCfg_TxQ_GetNextHigherIdx(TxConId));
    vIpc_XCfg_TxQ_SetNextLowerIdx((PduIdType)vIpc_XCfg_TxQ_GetNextHigherIdx(TxConId), invalidConId);
  }
  /* #50 Otherwise, it is a connection in the middle of the queue, let the adjacent connection point to each other. */
  else
  {
    /* Move lower connections high pointer to higher connection. */
    vIpc_XCfg_TxQ_SetNextHigherIdx((PduIdType)vIpc_XCfg_TxQ_GetNextLowerIdx(TxConId),
                                   vIpc_XCfg_TxQ_GetNextHigherIdx(TxConId));
    /* Move higher connections lower pointer to lower connection. */
    vIpc_XCfg_TxQ_SetNextLowerIdx((PduIdType)vIpc_XCfg_TxQ_GetNextHigherIdx(TxConId),
                                  vIpc_XCfg_TxQ_GetNextLowerIdx(TxConId));
  }

  /* #60 Leave critical section. */
  SchM_Exit_vIpc_VIPC_EXCLUSIVE_AREA_TX_QUEUE();
} /* vIpc_PQ_Remove */

#define VIPC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */ /* PRQA 6050 1 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  END OF FILE: vIpc_Rx.c
 *********************************************************************************************************************/
