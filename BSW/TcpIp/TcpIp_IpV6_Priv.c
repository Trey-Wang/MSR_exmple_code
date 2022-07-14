/**********************************************************************************************************************
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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  TcpIp_IpV6_Priv.c
 *        \brief  Implementation of Internet Protocol version 6 (IPv6) - Internal Functions
 *
 *      \details  This file is part of the TcpIp IPv6 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv6 submodule. >> TcpIp_IpV6.h
 *********************************************************************************************************************/

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* PRQA S 0828 EOF */ /* MD_MSR_1.1_828 */

#define TCPIP_IPV6_PRIV_SOURCE

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_IPV6 == STD_ON)
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "TcpIp_IpV6_Priv.h"

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

#if defined (STATIC)
#else
# define STATIC static
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
/* PRQA S 3453 FctLikeMacros */ /* MD_MSR_19.7 */

/* PRQA L:FctLikeMacros */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/* ... */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

#define IPV6_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define IPV6_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/* ... */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define IPV6_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (IPV6_ENABLE_DLISTS == STD_ON)
/* Doubly Linked List (DList) Data Structure */

/* DList raw functions */

/**********************************************************************************************************************
 *  IpV6_DListRawAddAfter()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_DListRawAddAfter(
  IPV6_P2V(IpV6_DListNodeType) NodeSetPtr,
  IPV6_P2V(IpV6_DListIdxType)  FirstIdxPtr,
  IPV6_P2V(IpV6_DListIdxType)  LastIdxPtr,
  IpV6_DListIdxType            NewIdx,
  IpV6_DListIdxType            AfterIdx);

/**********************************************************************************************************************
 *  IpV6_DListRawAddRangeAfter()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_DListRawAddRangeAfter(
  IPV6_P2V(IpV6_DListNodeType) NodeSetPtr,
  IPV6_P2V(IpV6_DListIdxType)  FirstIdxPtr,
  IPV6_P2V(IpV6_DListIdxType)  LastIdxPtr,
  IpV6_DListIdxType            NewStartIdx,
  IpV6_DListIdxType            NewEndIdx,
  IpV6_DListIdxType            AfterIdx);

/**********************************************************************************************************************
 *  IpV6_DListRawRemove()
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_DListRawRemove(
  IPV6_P2V(IpV6_DListNodeType) NodeSetPtr,
  IPV6_P2V(IpV6_DListIdxType)  FirstIdxPtr,
  IPV6_P2V(IpV6_DListIdxType)  LastIdxPtr,
  IpV6_DListIdxType            RemIdx);

/**********************************************************************************************************************
 *  IpV6_DListRawRemoveRange()
 **********************************************************************************************************************/
STATIC FUNC(uint8, IPV6_CODE) IpV6_DListRawRemoveRange(
  IPV6_P2V(IpV6_DListNodeType)  NodeSetPtr,
  IPV6_P2V(IpV6_DListIdxType)   FirstIdxPtr,
  IPV6_P2V(IpV6_DListIdxType)   LastIdxPtr,
  IpV6_DListIdxType             RemStartIdx,
  IpV6_DListIdxType             RemEndIdx);

#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

#if (IPV6_ENABLE_DLISTS == STD_ON)

/**********************************************************************************************************************
 *  IpV6_DListRawAddAfter()
 **********************************************************************************************************************/
/*! \brief Insert a node into a node chain.
 *  \param[in]    NodeSetPtr  Storage array of the elements of the node chain.
 *  \param[inout] FirstIdxPtr Index of the first element of the node chain.
 *  \param[inout] LastIdxPtr  Index of the last element of the node chain.
 *  \param[in]    NewIdx      Index of the new element that shall be insterted into the node chain
 *  \param[in]    AfterIdx    Index of the pedecessor of the newly inserted element. (must be in chain FirstIdx...LastIdx)
 *                            If AfterIdx is IPV6_DLIST_END_IDX the new item will be inserted at the beginning.
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_DListRawAddAfter(
  IPV6_P2V(IpV6_DListNodeType) NodeSetPtr,
  IPV6_P2V(IpV6_DListIdxType)  FirstIdxPtr,
  IPV6_P2V(IpV6_DListIdxType)  LastIdxPtr,
  IpV6_DListIdxType            NewIdx,
  IpV6_DListIdxType            AfterIdx)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != NodeSetPtr,  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != FirstIdxPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != LastIdxPtr,  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  NodeSetPtr[NewIdx].PrevIdx = AfterIdx;

  if (IPV6_DLIST_END_IDX != AfterIdx)
  {
    /* insert after existing node */
    NodeSetPtr[NewIdx].NextIdx = NodeSetPtr[AfterIdx].NextIdx;
    NodeSetPtr[AfterIdx].NextIdx = NewIdx;
  }
  else
  {
    /* insert as first node */
    NodeSetPtr[NewIdx].NextIdx = (*FirstIdxPtr);
    (*FirstIdxPtr) = NewIdx;
  }

  if (IPV6_DLIST_END_IDX != NodeSetPtr[NewIdx].NextIdx)
  {
    /* new node is not last node */
    NodeSetPtr[NodeSetPtr[NewIdx].NextIdx].PrevIdx = NewIdx;
  }
  else
  {
    /* new node is last node */
    (*LastIdxPtr) = NewIdx;
  }

} /* End of IpV6_DListRawAddAfter() */

/**********************************************************************************************************************
 *  IpV6_DListRawAddRangeAfter()
 **********************************************************************************************************************/
/*! \brief        Insert a node chain into another node chain.
 *  \param[in]    NodeSetPtr    Storage array of the elements of the node chain.
 *  \param[inout] FirstIdxPtr   Index of the first element of the node chain.
 *  \param[inout] LastIdxPtr    Index of the last element of the node chain.
 *  \param[in]    NewStartIdx   Index of the first element of the node chain that shall be inserted
 *  \param[in]    NewEndIdx     Index of the last element of the node chain that shall be inserted
 *  \param[in]    AfterIdx      Index of the pedecessor of the newly inserted node chain.
 *                              If AfterIdx is IPV6_DLIST_END_IDX the chain will be inserted at the beginning.
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_DListRawAddRangeAfter(
  IPV6_P2V(IpV6_DListNodeType) NodeSetPtr,
  IPV6_P2V(IpV6_DListIdxType)  FirstIdxPtr,
  IPV6_P2V(IpV6_DListIdxType)  LastIdxPtr,
  IpV6_DListIdxType            NewStartIdx,
  IpV6_DListIdxType            NewEndIdx,
  IpV6_DListIdxType            AfterIdx)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != NodeSetPtr,  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != FirstIdxPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != LastIdxPtr,  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);

  if (E_OK != IpV6_VDbgDListCheckChain(NodeSetPtr, NewStartIdx, NewEndIdx, (IPV6_P2V(uint8))NULL_PTR))
  {
    IpV6_DetReportError(IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM);
    return;
  }
#endif

  NodeSetPtr[NewStartIdx].PrevIdx = AfterIdx;

  if (IPV6_DLIST_END_IDX != AfterIdx)
  {
    /* insert after existing node */
    NodeSetPtr[NewEndIdx].NextIdx = NodeSetPtr[AfterIdx].NextIdx;
    NodeSetPtr[AfterIdx].NextIdx  = NewStartIdx;
  }
  else
  {
    /* insert as first node */
    NodeSetPtr[NewEndIdx].NextIdx = (*FirstIdxPtr);
    (*FirstIdxPtr) = NewStartIdx;
  }

  if (IPV6_DLIST_END_IDX != NodeSetPtr[NewEndIdx].NextIdx)
  {
    /* new node is not last node */
    NodeSetPtr[NodeSetPtr[NewEndIdx].NextIdx].PrevIdx = NewEndIdx;
  }
  else
  {
    /* new node is last node */
    (*LastIdxPtr) = NewEndIdx;
  }

} /* End of IpV6_DListRawAddRangeAfter() */

/**********************************************************************************************************************
 *  IpV6_DListRawRemove()
 **********************************************************************************************************************/
/*! \brief      Removes a node from a node chain.
 *  \param[in]  NodeSetPtr    Storage array of the elements of the node chain.
 *  \param[in]  FirstIdxPtr   Index of the first element of the node chain.
 *  \param[in]  LastIdxPtr    Index of the last element of the node chain.
 *  \param[in]  RemIdx        Index of the element that shall be removed.
 **********************************************************************************************************************/
STATIC FUNC(void, IPV6_CODE) IpV6_DListRawRemove(
  IPV6_P2V(IpV6_DListNodeType)  NodeSetPtr,
  IPV6_P2V(IpV6_DListIdxType)   FirstIdxPtr,
  IPV6_P2V(IpV6_DListIdxType)   LastIdxPtr,
  IpV6_DListIdxType             RemIdx)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != NodeSetPtr,               IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != FirstIdxPtr,              IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != LastIdxPtr,               IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(IPV6_DLIST_END_IDX != (*FirstIdxPtr), IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(IPV6_DLIST_END_IDX != (*LastIdxPtr),  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(IPV6_DLIST_END_IDX != RemIdx,         IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  if (IPV6_DLIST_END_IDX != NodeSetPtr[RemIdx].PrevIdx)
  {
    /* node is not first node */
    NodeSetPtr[NodeSetPtr[RemIdx].PrevIdx].NextIdx = NodeSetPtr[RemIdx].NextIdx;
  }
  else
  {
    /* remove first node */
    (*FirstIdxPtr) = NodeSetPtr[RemIdx].NextIdx;
  }

  if (IPV6_DLIST_END_IDX != NodeSetPtr[RemIdx].NextIdx)
  {
    /* node is not last node */
    NodeSetPtr[NodeSetPtr[RemIdx].NextIdx].PrevIdx = NodeSetPtr[RemIdx].PrevIdx;
  }
  else
  {
    /* remove last node */
    (*LastIdxPtr) = NodeSetPtr[RemIdx].PrevIdx;
  }

  NodeSetPtr[RemIdx].PrevIdx = IPV6_DLIST_END_IDX;
  NodeSetPtr[RemIdx].NextIdx = IPV6_DLIST_END_IDX;

} /* End of IpV6_DListRawRemove() */

/**********************************************************************************************************************
 *  IpV6_DListRawRemoveRange()
 **********************************************************************************************************************/
/*! \brief        Removes a sub chain from a node chain.
 *  \param[in]    NodeSetPtr    Storage array of the elements of the node chain.
 *  \param[inout] FirstIdxPtr   Index of the first element of the node chain.
 *  \param[inout] LastIdxPtr    Index of the last element of the node chain.
 *  \param[in]    RemStartIdx   Index of the first element that shall be removed.
 *  \param[in]    RemEndIdx     Index of the last element that shall be removed.
 **********************************************************************************************************************/
STATIC FUNC(uint8, IPV6_CODE) IpV6_DListRawRemoveRange(
  IPV6_P2V(IpV6_DListNodeType)  NodeSetPtr,
  IPV6_P2V(IpV6_DListIdxType)   FirstIdxPtr,
  IPV6_P2V(IpV6_DListIdxType)   LastIdxPtr,
  IpV6_DListIdxType             RemStartIdx,
  IpV6_DListIdxType             RemEndIdx)
{
  IpV6_DListIdxType Idx;
  uint8 ChainLength = 1;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 3 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != NodeSetPtr,  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, 0);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != FirstIdxPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, 0);
  IpV6_CheckDetErrorReturnValue(NULL_PTR != LastIdxPtr,  IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, 0);
#endif

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  if (E_OK != IpV6_VDbgDListCheckChain(NodeSetPtr, RemStartIdx, RemEndIdx, (IPV6_P2V(uint8))NULL_PTR))
  {
    IpV6_DetReportError(IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM);
    return 0;
  }
#endif

  /* count number of elements in chain */
  Idx = RemStartIdx;

  while (RemEndIdx != Idx)
  {
    ChainLength++;
    Idx = NodeSetPtr[Idx].NextIdx;
  }

  if (IPV6_DLIST_END_IDX != NodeSetPtr[RemStartIdx].PrevIdx)
  {
    /* node is not first node */
    NodeSetPtr[NodeSetPtr[RemStartIdx].PrevIdx].NextIdx = NodeSetPtr[RemEndIdx].NextIdx;
  }
  else
  {
    /* remove first node */
    (*FirstIdxPtr) = NodeSetPtr[RemEndIdx].NextIdx;
  }

  if (IPV6_DLIST_END_IDX != NodeSetPtr[RemEndIdx].NextIdx)
  {
    /* node is not last node */
    NodeSetPtr[NodeSetPtr[RemEndIdx].NextIdx].PrevIdx = NodeSetPtr[RemStartIdx].PrevIdx;
  }
  else
  {
    /* remove last node */
    (*LastIdxPtr) = NodeSetPtr[RemStartIdx].PrevIdx;
  }

  NodeSetPtr[RemStartIdx].PrevIdx = IPV6_DLIST_END_IDX;
  NodeSetPtr[RemEndIdx].NextIdx   = IPV6_DLIST_END_IDX;

  return ChainLength;
} /* End of IpV6_DListRawRemoveRange() */

/**********************************************************************************************************************
 *  IpV6_DListInitDesc()
 **********************************************************************************************************************/
/*! \brief        Initializes a Dlist descriptor. All nodes from the node storage array will be linked and inserted
 *                into the free list. This descriptor can be used by multiple DList instances. See IpV6_DListInit().
 *  \param[in]    DescPtr       Descriptor to be initialized.
 *  \param[in]    NodePtr       Node storage array
 *  \param[in]    NodeCount     Number of nodes in node storage array
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_DListInitDesc(
  IPV6_P2V(IpV6_DListDescType) DescPtr,
  IPV6_P2V(IpV6_DListNodeType) NodePtr,
  uint8                        NodeCount)
{
  uint8 DataIdx = NodeCount;
  DescPtr->NodePtr = NodePtr;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DescPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != NodePtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  if (0 < NodeCount)
  {
    DescPtr->FirstFreeIdx = 0;
    DescPtr->LastFreeIdx  = (IpV6_DListIdxType)(NodeCount - 1);

    while (0 < DataIdx)
    {
      DataIdx--;

      if (DescPtr->FirstFreeIdx < DataIdx)
      {
        NodePtr[DataIdx].PrevIdx = (IpV6_DListIdxType)(DataIdx - 1);
      }
      else
      {
        NodePtr[DataIdx].PrevIdx = IPV6_DLIST_END_IDX;
      }

      if (DescPtr->LastFreeIdx > DataIdx)
      {
        NodePtr[DataIdx].NextIdx = (IpV6_DListIdxType)(DataIdx + 1);
      }
      else
      {
        NodePtr[DataIdx].NextIdx = IPV6_DLIST_END_IDX;
      }
    }
  }
  else
  {
    DescPtr->FirstFreeIdx = IPV6_DLIST_END_IDX;
    DescPtr->LastFreeIdx  = IPV6_DLIST_END_IDX;
  }

  DescPtr->ElementCount = NodeCount;

} /* End of IpV6_DListInitDesc() */

/**********************************************************************************************************************
 *  IpV6_DListInit()
 **********************************************************************************************************************/
/*! \brief        Initializes a DList instance. After initialization the list is empty.
 *                New elements can be inserted by IpV6_DListAddElement().
 *  \param[in]    ListPtr       DList instnace
 *  \param[in]    DescPtr       DList descriptor that will by used by this instnace.
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_DListInit(
  IPV6_P2V(IpV6_DListType)     ListPtr,
  IPV6_P2V(IpV6_DListDescType) DescPtr)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != ListPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DescPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  ListPtr->DescPtr      = DescPtr;
  ListPtr->FirstIdx     = IPV6_DLIST_END_IDX;
  ListPtr->LastIdx      = IPV6_DLIST_END_IDX;
  ListPtr->ElementCount = 0;

} /* End of IpV6_DListInit() */

/**********************************************************************************************************************
 *  IpV6_DListAddElement()
 **********************************************************************************************************************/
/*! \brief      Adds an element to a DList instnace by moving it from the free list.
 *  \param[in]  DListPtr   Handle to the list into that the new element should be inserted.
 *  \param[in]  AfterIdx   Index of a list item that shall be the predecessor of the new item.
 *                         If the index is IPV6_DLIST_END_IDX the new item will be inserted at the beginning of the list.
 *  \param[out] NewIdxPtr  Index of the inserted item. Only valid if result is E_OK.
 *  \retval     E_OK       success
 *  \retval     E_NOT_OK   No elements in free list. (NewIdxPtr not modified)
 **********************************************************************************************************************/
FUNC(Std_ReturnType, IPV6_CODE) IpV6_DListAddElement(
  IPV6_P2V(IpV6_DListType)    DListPtr,
  IpV6_DListIdxType           AfterIdx,
  IPV6_P2V(IpV6_DListIdxType) NewIdxPtr)
{
  /* get first element from free list */
  IpV6_DListIdxType NewIdx = DListPtr->DescPtr->FirstFreeIdx;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnValue(NULL_PTR != DListPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER, E_NOT_OK);
#endif

  if (IPV6_DLIST_END_IDX == DListPtr->DescPtr->FirstFreeIdx)
  {
    /* no free elements */
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
    IpV6_DetReportError(IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_DLIST_BUFFER_EMPTY);
#endif
    return E_NOT_OK;
  }

  IpV6_DListRawRemove(DListPtr->DescPtr->NodePtr, &DListPtr->DescPtr->FirstFreeIdx, &DListPtr->DescPtr->LastFreeIdx, NewIdx);
  DListPtr->DescPtr->ElementCount--;

  /* add new element to list */
  IpV6_DListRawAddAfter(DListPtr->DescPtr->NodePtr, &DListPtr->FirstIdx, &DListPtr->LastIdx, NewIdx, AfterIdx);
  DListPtr->ElementCount++;

  (*NewIdxPtr) = NewIdx;

  return E_OK;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_DListAddElement() */

/**********************************************************************************************************************
 *  IpV6_DListRemoveElementRange()
 **********************************************************************************************************************/
/*! \brief      Removes a chain of linked elements from the list and adds them to the free list of the descriptor.
 *  \param[in]  DListPtr    Handle to the DList instnace containing the elements to be removed.
 *  \param[in]  RemStartIdx Index of the first item of the element chain that shall be removed.
 *  \param[in]  RemEndIdx   Index of the lest item of the element chain that shall be removed.
 *  \note       RemStartIdx and RemEndIdx may be equal but for performance reasons IpV6_DListRemoveElement() should be
 *              used for removing single elements.
 *  \warning    For performance reasons this function does not check if the elements to be removed are correctly linked
 *              when IPV6_VENABLE_INTERNAL_DET_CHECKS are not enabled!
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_DListRemoveElementRange(
  IPV6_P2V(IpV6_DListType) DListPtr,
  IpV6_DListIdxType        RemStartIdx,
  IpV6_DListIdxType        RemEndIdx)
{
  uint8 ChainCount;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DListPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  /* remove element from list */
  ChainCount = IpV6_DListRawRemoveRange(DListPtr->DescPtr->NodePtr, &DListPtr->FirstIdx, &DListPtr->LastIdx, RemStartIdx, RemEndIdx);
  DListPtr->ElementCount -= ChainCount;

  /* add removed element to beginning of free list */
  IpV6_DListRawAddRangeAfter(DListPtr->DescPtr->NodePtr, &DListPtr->DescPtr->FirstFreeIdx, &DListPtr->DescPtr->LastFreeIdx, RemStartIdx, RemEndIdx, DListPtr->DescPtr->FirstFreeIdx);
  DListPtr->DescPtr->ElementCount += ChainCount;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_DListRemoveElementRange() */

/**********************************************************************************************************************
 *  IpV6_DListRemoveElement()
 **********************************************************************************************************************/
/*! \brief      Removes a single element from a DList instance and adds it to the free list of the descriptor
 *  \param[in]  DListPtr  Handle to the DList instnace containing the element to be removed.
 *  \param[in]  RemIdx    Index of the element that shall be removed.
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_DListRemoveElement(
  IPV6_P2V(IpV6_DListType) DListPtr,
  IpV6_DListIdxType        RemIdx)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DListPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  /* remove element from list */
  IpV6_DListRawRemove(DListPtr->DescPtr->NodePtr, &DListPtr->FirstIdx, &DListPtr->LastIdx, RemIdx);
  DListPtr->ElementCount--;

  /* add removed element to beginning of free list */
  IpV6_DListRawAddAfter(DListPtr->DescPtr->NodePtr, &DListPtr->DescPtr->FirstFreeIdx, &DListPtr->DescPtr->LastFreeIdx, RemIdx, DListPtr->DescPtr->FirstFreeIdx);
  DListPtr->DescPtr->ElementCount++;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_DListRemoveElement() */

/**********************************************************************************************************************
 *  IpV6_DListMoveElement()
 **********************************************************************************************************************/
/*! \brief      Moves a single element from a DList instance into another DList instance.
 *  \note       Both DList instances MUST have been initialized with the same DList descriptor.
 *  \param[in]  SrcListPtr Handle to the DList instance containing the element to be moved.
 *  \param[in]  SrcListIdx Index of the element that shall be moved.
 *  \param[in]  TgtListPtr Handle to the DList instance to which the element shall be added.
 *  \param[in]  TgtPrevIdx Index of the new predecessor of the moved element in the target list.
 *                         If AfterIdx is IPV6_DLIST_END_IDX the new item will be inserted at the beginning.
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_DListMoveElement(
  IPV6_P2V(IpV6_DListType) SrcListPtr,
  IpV6_DListIdxType        SrcListIdx,
  IPV6_P2V(IpV6_DListType) TgtListPtr,
  IpV6_DListIdxType        TgtPrevIdx)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != SrcListPtr,                     IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != TgtListPtr,                     IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(IPV6_DLIST_END_IDX != SrcListIdx,           IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(SrcListPtr->DescPtr == TgtListPtr->DescPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM);
#endif

  /* remove element from source list */
  IpV6_DListRawRemove(SrcListPtr->DescPtr->NodePtr, &SrcListPtr->FirstIdx, &SrcListPtr->LastIdx, SrcListIdx);
  SrcListPtr->ElementCount--;

  /* add removed element to beginning of free list */
  IpV6_DListRawAddAfter(TgtListPtr->DescPtr->NodePtr, &TgtListPtr->FirstIdx, &TgtListPtr->LastIdx, SrcListIdx, TgtPrevIdx);
  TgtListPtr->ElementCount++;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_DListMoveElement() */

/**********************************************************************************************************************
 *  IpV6_DListMoveElementRange()
 **********************************************************************************************************************/
/*! \brief      Moves an element chain from one DList instance into another DList instance
 *  \note       Both DList instances MUST have been initialized with the same DList descriptor.
 *  \param[in]  SrcListPtr      Handle to the DList instance containing the element chain to be moved.
 *  \param[in]  SrcListStartIdx Index of the first element that shall be moved.
 *  \param[in]  SrcListEndIdx   Index of the last element that shall be moved.
 *  \param[in]  TgtListPtr      Handle to the DList instance to which the element chain shall be added.
 *  \param[in]  TgtPrevIdx      Index of the new predecessor of the moved element chain in the target list.
 *                              If AfterIdx is IPV6_DLIST_END_IDX the chain will be inserted at the beginning.
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_DListMoveElementRange(
  IPV6_P2V(IpV6_DListType) SrcListPtr,
  IpV6_DListIdxType        SrcListStartIdx,
  IpV6_DListIdxType        SrcListEndIdx,
  IPV6_P2V(IpV6_DListType) TgtListPtr,
  IpV6_DListIdxType        TgtPrevIdx)
{
  uint8 ChainCount;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 4 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != SrcListPtr,                     IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != TgtListPtr,                     IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(IPV6_DLIST_END_IDX != SrcListStartIdx,      IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(IPV6_DLIST_END_IDX != SrcListEndIdx,        IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(SrcListPtr->DescPtr == TgtListPtr->DescPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM);
#endif

  /* remove element from source list */
  ChainCount = IpV6_DListRawRemoveRange(SrcListPtr->DescPtr->NodePtr, &SrcListPtr->FirstIdx, &SrcListPtr->LastIdx, SrcListStartIdx, SrcListEndIdx);
  SrcListPtr->ElementCount -= ChainCount;

  /* add removed element to beginning of free list */
  IpV6_DListRawAddRangeAfter(             TgtListPtr->DescPtr->NodePtr, &TgtListPtr->FirstIdx, &TgtListPtr->LastIdx, SrcListStartIdx, SrcListEndIdx, TgtPrevIdx);
  TgtListPtr->ElementCount += ChainCount;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_DListMoveElementRange() */

/**********************************************************************************************************************
 *  IpV6_DListClear()
 **********************************************************************************************************************/
/*! \brief      Clears a DList instance by moving all elements to the free list of the descriptor.
 *  \param[in]  DListPtr  Handle to the DList instance
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_DListClear(
  IPV6_P2V(IpV6_DListType) DListPtr)
{
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != DListPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  if (!IPV6_DLIST_IS_EMPTY(*DListPtr))
  {
    IpV6_DListRemoveElementRange(DListPtr, DListPtr->FirstIdx, DListPtr->LastIdx);
  }

} /* End of IpV6_DListClear() */

/**********************************************************************************************************************
 *  IpV6_DListSwap()
 **********************************************************************************************************************/
/*! \brief      Swaps the content of two DList instances.
 *  \note       Both DList instances MUST have been initialized with the same DList descriptor.
 *  \param[in]  ListAPtr Handle to DList instance
 *  \param[in]  ListBPtr Handle to DList instance
 **********************************************************************************************************************/
FUNC(void, IPV6_CODE) IpV6_DListSwap(
  IPV6_P2V(IpV6_DListType) ListAPtr,
  IPV6_P2V(IpV6_DListType) ListBPtr)
{
  IpV6_DListIdxType TmpFirstIdx, TmpLastIdx;
  uint8 TmpCount;

#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
  /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != ListAPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
  IpV6_CheckDetErrorReturnVoid(NULL_PTR != ListBPtr, IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_POINTER);
#endif

  if (ListAPtr->DescPtr != ListBPtr->DescPtr)
  {
    /* swapping lists is only allowd for lists with same descriptor */
#if (IPV6_VENABLE_INTERNAL_DET_CHECKS == STD_ON)
    IpV6_DetReportError(IPV6_API_ID_V_INTERNAL_FUNCTION, IPV6_E_INV_PARAM);
#endif
    return;
  }

  TmpFirstIdx = ListAPtr->FirstIdx;
  TmpLastIdx  = ListAPtr->LastIdx;
  TmpCount    = ListAPtr->ElementCount;

  ListAPtr->FirstIdx     = ListBPtr->FirstIdx;
  ListAPtr->LastIdx      = ListBPtr->LastIdx;
  ListAPtr->ElementCount = ListBPtr->ElementCount;

  ListBPtr->FirstIdx     = TmpFirstIdx;
  ListBPtr->LastIdx      = TmpLastIdx;
  ListBPtr->ElementCount = TmpCount;

  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
} /* End of IpV6_DListSwap() */

#endif

#define IPV6_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* (TCPIP_SUPPORT_IPV6 == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: IpV6_Priv.c
 *********************************************************************************************************************/
