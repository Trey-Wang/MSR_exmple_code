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
/**        \file  EthIf_Int.h
 *        \brief  Ethernet Interface internal header file
 *
 *      \details  Component to abstract the underlying Ethernet hardware for the upper layers to provide a generic
 *                interface to be used for controlling and communicate with the help of the Ethernet hardware
 *                provided by an ECU (Ethernet controller, Ethernet transceiver, Ethernet switch).
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined(ETHIF_INT_H)
# define ETHIF_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthIf_Cfg.h"
# include "EthIf_Lcfg.h"
# include "EthIf_Types.h"
# include "SchM_EthIf.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

#if !defined (ETHIF_INLINE)
# define ETHIF_INLINE  static INLINE
#endif

/* ----- VLAN Tag structure ----- */
/* TCI structure according to IEEE802.1Q */
# define ETHIF_VLAN_TCI_VID_SHIFT               (0U)
# define ETHIF_VLAN_TCI_DEI_SHIFT               (12U)
# define ETHIF_VLAN_TCI_PCP_SHIFT               (13U)
# define ETHIF_VLAN_TCI_VID_MASK                (0x0FFFU)
# define ETHIF_VLAN_TCI_DEI_MASK                (0x1000U)
# define ETHIF_VLAN_TCI_PCP_MASK                (0xE000U)
/* VLAN TPID according to IEEE802.1Q */
# define ETHIF_VLAN_TPID_UB                     (0x81U)
# define ETHIF_VLAN_TPID_LB                     (0x00U)
# define ETHIF_VLAN_TPID                        (0x8100U)
/* VLAN Tag offsets into Ethernet frame */
# define ETHIF_VLAN_TCI_OFFSET                  (0U)
# define ETHIF_VLAN_FRAME_TYPE_OFFSET           (2U)
/* Length of the VLAN-Tag */
# define ETHIF_VLAN_TAG_LEN_BYTE                (4U)
/* PCP according to IEEE802.1Q */
# define ETHIF_VLAN_PCP_MAX_VAL                 (7U)
/* VLAN ID according to IEEE802.1Q */
# define ETHIF_VLAN_VLANID_PRIO_VLANID          (0U)
# define ETHIF_VLAN_VLANID_INV_VLANID           (ETHIF_INV_VLAN_ID)

/* ---- Parallel processing defines ----- */
/* Value defines the maximum amount of parallel receive processing contexts per Ethernet controller.
 * The value is defined by assuming that interrupts can be generated for each PCP of the VLAN tag
 * plus one for not VLAN tagged traffic which result in the maximum of 9 reception contexts that can
 * interrupt each other. */
# define ETHIF_MAX_PARALELL_RX_CONTEXT_PER_ETHCTRL  (9U)

/* ----- Inline function availability ----- */

/* -- EthIf_GetFrameId() -- */
# if (                                                                                  \
       (defined(ETHIF_SOURCE) && (ETHIF_ETHSWT_TIME_STAMP_SUPPORT == STD_ON)       ) || \
       (defined(ETHIF_SOURCE) && (ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT == STD_ON) )    \
     )
#  define ETHIF_INLINE_API_GET_FRAME_ID_ENABLED  STD_ON
# else
#  define ETHIF_INLINE_API_GET_FRAME_ID_ENABLED  STD_OFF
# endif /* EthIf_GetFrameId() availability check */

/* -- EthIf_QueryForFrameOwner() -- */
# if (                                                                                        \
       (defined(ETHIF_SOURCE)                                                           )  || \
       (defined(ETHIF_ZERO_COPY_SOURCE) && (ETHIF_ENABLE_ZERO_COPY_EXTENSIONS == STD_ON))     \
     )
#  define ETHIF_INLINE_API_QUERY_FOR_FRAME_OWNER_ENABLED  STD_ON
# else
#  define ETHIF_INLINE_API_QUERY_FOR_FRAME_OWNER_ENABLED  STD_OFF
# endif /* EthIf_QueryForFrameOwner() availability check */

/* -- EthIf_PrepareVlanTag() -- */
# if (                                                                                        \
       (defined(ETHIF_SOURCE)                                                           )  || \
       (defined(ETHIF_ZERO_COPY_SOURCE) && (ETHIF_ENABLE_ZERO_COPY_EXTENSIONS == STD_ON))     \
     )
#  define ETHIF_INLINE_API_PREPARE_VLAN_TAG_ENABLED  STD_ON
# else
#  define ETHIF_INLINE_API_PREPARE_VLAN_TAG_ENABLED  STD_OFF
# endif /* EthIf_PrepareVlanTag() availability check */

/* -- EthIf_PushToMgmtRxCtxtStack() -- */
# if(                                                                              \
      (defined(ETHIF_SOURCE) && (ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT == STD_ON)) \
    )
#  define ETHIF_INLINE_API_MGMT_RX_CTXT_PUSH_STACK_ENABLED  STD_ON
# else
#  define ETHIF_INLINE_API_MGMT_RX_CTXT_PUSH_STACK_ENABLED  STD_OFF
# endif /* EthIf_PushToMgmtRxCtxtStack() availability check */

/* -- EthIf_PopFromMgmtRxCtxtStack() -- */
# if(                                                                                    \
      (defined(ETHIF_SWITCH_SOURCE) && (ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT == STD_ON)) \
    )
#  define ETHIF_INLINE_API_MGMT_RX_CTXT_POP_STACK_ENABLED  STD_ON
# else
#  define ETHIF_INLINE_API_MGMT_RX_CTXT_POP_STACK_ENABLED  STD_OFF
# endif /* EthIf_PopFromMgmtRxCtxtStack() availability check */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453,3458 FCT_LIKE_MACROS */ /* MISRA 19.4,19.7: Macros improve code readability */

# define EthIf_EnterExclusiveArea(ExclusiveArea)  SchM_Enter_EthIf_ETHIF_EXCLUSIVE_AREA_##ExclusiveArea() /* PRQA S 0342 */ /* MD_MSR_19.13_0342 */
# define EthIf_ExitExclusiveArea(ExclusiveArea)   SchM_Exit_EthIf_ETHIF_EXCLUSIVE_AREA_##ExclusiveArea()  /* PRQA S 0342 */ /* MD_MSR_19.13_0342 */

/* PRQA L:FCT_LIKE_MACROS */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
# define ETHIF_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern VAR(EthIf_StateType, ETHIF_VAR_INIT)  EthIf_ModuleInitialized;

# define ETHIF_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETHIF_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if ( ETHIF_INLINE_API_GET_FRAME_ID_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_GetFrameId
 *********************************************************************************************************************/
/*! \brief      Retrieves the frame identifier contained in the Ethernet frame buffer
 *  \param[in]  DataPtr     Pointer to the Ethernet frame buffer
 *  \param[in]  PayloadLen  Length of the Ethernet frame payload
 *  \return     Frame identifier contained in the Ethernet frame buffer
 *********************************************************************************************************************/
ETHIF_INLINE FUNC(Eth_FrameIdType, ETHIF_CODE) EthIf_GetFrameId(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) DataPtr,
          uint16                       PayloadLen);
# endif /* ETHIF_INLINE_API_GET_FRAME_ID_ENABLED */

# if ( ETHIF_INLINE_API_QUERY_FOR_FRAME_OWNER_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_QueryForFrameOwner
 *********************************************************************************************************************/
/*! \brief      Queries for the owner of the EtherType
 *  \details    This function queries for the owner of the EtherType asked for.
 *  \param[in]  etherType   EtherType the owner shall be found for
 *  \return     EthIf_CfgAccess_InvalidFrameOwnerIdx() - represents the first invalid owner index
 *                                                       -> no owner for EtherType found
 *  \return     other values - owner index
 *********************************************************************************************************************/
ETHIF_INLINE FUNC(uint8, ETHIF_CODE) EthIf_QueryForFrameOwner(
  uint16 etherType);
# endif /* ETHIF_INLINE_API_QUERY_FOR_FRAME_OWNER_ENABLED */

# if ( ETHIF_INLINE_API_PREPARE_VLAN_TAG_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_PrepareVlanTag
 *********************************************************************************************************************/
/*! \brief      Prepares the VLAN tag in the location provided
 *  \details    This function inserts the VLAN tag with its TCI and the desired EtherType at the data location
 *              provided and returns the new location of the payload.
 *  \param[in]  frameHdr    Ethernet header information
 *  \param[in]  tagPtr      Pointer pointing to the position where the VLAN tag shall be insert
 *  \param[in]  lenPtr      Length that shall be adapted for VLAN tag insertion
 *  \return     New location of the payload
 *********************************************************************************************************************/
ETHIF_INLINE FUNC(P2VAR(uint8, AUTOMATIC, ETHIF_APPL_VAR), ETHIF_CODE) EthIf_PrepareVlanTag(
  P2CONST(EthIf_FrameHdrType, AUTOMATIC, ETHIF_CONST)     frameHdr,
    P2VAR(uint8,              AUTOMATIC, ETHIF_APPL_VAR)  tagPtr,
    P2VAR(uint16,             AUTOMATIC, ETHIF_APPL_VAR)  lenPtr);
# endif /* ETHIF_INLINE_API_PREPARE_VLAN_TAG_ENABLED */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
 
 # if ( ETHIF_INLINE_API_GET_FRAME_ID_ENABLED == STD_ON )
 /**********************************************************************************************************************
 *  EthIf_GetFrameId
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
ETHIF_INLINE FUNC(Eth_FrameIdType, ETHIF_CODE) EthIf_GetFrameId(
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) DataPtr,
          uint16                       PayloadLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) frameIdPtr;
        Eth_FrameIdType              frameId;
        uint8_least                  frameIdByteIdx;
  
  /* ----- Implementation ----------------------------------------------- */
  frameIdPtr    = (P2VAR(uint8, AUTOMATIC, AUTOMATIC))&frameId; /* PRQA S 0310 */ /* Byte access needed to avoid mis-aligned access on specific platforms. */
  for( frameIdByteIdx = 0; frameIdByteIdx < sizeof(Eth_FrameIdType); frameIdByteIdx++ )
  {
    frameIdPtr[frameIdByteIdx] = DataPtr[PayloadLen + frameIdByteIdx];
  }
  
  return frameId;
} /* EthIf_GetFrameId() */
# endif /* ETHIF_INLINE_API_GET_FRAME_ID_ENABLED */
 
# if ( ETHIF_INLINE_API_QUERY_FOR_FRAME_OWNER_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_QueryForFrameOwner
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_INLINE FUNC(uint8, ETHIF_CODE) EthIf_QueryForFrameOwner(
  uint16 etherType)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8        queriedOwnerIdx;
  uint8_least  loopIdx;

  /* ----- Implementation ----------------------------------------------- */
  queriedOwnerIdx = EthIf_CfgAccess_InvalidFrameOwnerIdx();

  /* #10 For each owner */
  for( loopIdx = 0u; loopIdx < EthIf_CfgAccess_FrameOwnerNum(); loopIdx++ )
  {
    /* #100 Check if EtherType of owner matches the EtherType queried for */
    if ( EthIf_CfgAccess_FrameOwner(loopIdx) == etherType )
    {
      /* #1000 EtherType matched -> owner found */
      queriedOwnerIdx = (uint8)loopIdx;
      break;
    }
  }

  return queriedOwnerIdx;
} /* EthIf_QueryForFrameOwner() */
# endif /* ETHIF_INLINE_API_QUERY_FOR_FRAME_OWNER_ENABLED */

# if ( ETHIF_INLINE_API_PREPARE_VLAN_TAG_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  EthIf_PrepareVlanTag
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
ETHIF_INLINE FUNC(P2VAR(uint8, AUTOMATIC, ETHIF_APPL_VAR), ETHIF_CODE) EthIf_PrepareVlanTag(
    P2CONST(EthIf_FrameHdrType, AUTOMATIC, ETHIF_CONST)     frameHdr,
      P2VAR(uint8,              AUTOMATIC, ETHIF_APPL_VAR)  tagPtr,
      P2VAR(uint16,             AUTOMATIC, ETHIF_APPL_VAR)  lenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 tci;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reduce the buffer length provided by the driver for tag insertion */
  (*lenPtr) -= ETHIF_VLAN_TAG_LEN_BYTE;

  /* #20 Prepare TCI */
  tci  = (uint16)((uint32)frameHdr->Priority << ETHIF_VLAN_TCI_PCP_SHIFT);
  tci |= (uint16)((uint32)frameHdr->VlanId   << ETHIF_VLAN_TCI_VID_SHIFT);
  tagPtr[0] = (uint8)((tci & 0xFF00U) >> 8U);
  tagPtr[1] = (uint8)( tci & 0x00FFU);

  /* #30 Prepare original EtherType */
  tagPtr[2] = (uint8)((frameHdr->EtherType & 0xFF00U) >> 8U);
  tagPtr[3] = (uint8)( frameHdr->EtherType & 0x00FFU);

  /* #30 Relocate buffer position for caller and return it */
  return &tagPtr[ETHIF_VLAN_TAG_LEN_BYTE];
} /* EthIf_PrepareVlanTag() */
# endif /* ETHIF_INLINE_API_PREPARE_VLAN_TAG_ENABLED */

# define ETHIF_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* ETHIF_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_Int.h
 *********************************************************************************************************************/

