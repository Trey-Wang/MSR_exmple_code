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
/**        \file  EthIf_CfgAccess_Int.h
 *        \brief  Ethernet Interface internal header file to access configuration data
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

#if !defined(ETHIF_CFG_ACCESS_INT_H)
# define ETHIF_CFG_ACCESS_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthIf_Cfg.h"
# include "EthIf_Lcfg.h"
# include "EthIf_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

#if !defined (ETHIF_INLINE)
# define ETHIF_INLINE  static INLINE
#endif

/* ----- Inline function availability ----- */
/* -- EthIf_CfgAccess_GetTxContextByIndex() -- */
# if (                                                                                            \
       (defined(ETHIF_SOURCE)                                                                ) || \
       (defined(ETHIF_ZERO_COPY_SOURCE) && (ETHIF_ENABLE_ZERO_COPY_EXTENSIONS     == STD_ON) ) || \
       (defined(ETHIF_SWITCH_SOURCE)    && (ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT == STD_ON) ) || \
       (defined(ETHIF_SWITCH_SOURCE)    && (ETHIF_ETHSWT_TIME_STAMP_SUPPORT       == STD_ON) )    \
     )
#  define ETHIF_INLINE_API_CFG_GET_TX_CONTEXT_BY_INDEX  STD_ON
# else
#  define ETHIF_INLINE_API_CFG_GET_TX_CONTEXT_BY_INDEX  STD_OFF
# endif /* EthIf_CfgAccess_GetTxContextByIndex() availability check */

/* -- EthIf_CfgAccess_GetTxContextByContextTuple() -- */
# if (                                                                                            \
       (defined(ETHIF_SOURCE)                                                                ) || \
       (defined(ETHIF_ZERO_COPY_SOURCE) && (ETHIF_ENABLE_ZERO_COPY_EXTENSIONS     == STD_ON) ) || \
       (defined(ETHIF_SWITCH_SOURCE)    && (ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT == STD_ON) ) || \
       (defined(ETHIF_SWITCH_SOURCE)    && (ETHIF_ETHSWT_TIME_STAMP_SUPPORT       == STD_ON) )    \
     )
#  define ETHIF_INLINE_API_CFG_GET_TX_CONTEXT_BY_CONTEXT_TUPLE  STD_ON
# else
#  define ETHIF_INLINE_API_CFG_GET_TX_CONTEXT_BY_CONTEXT_TUPLE  STD_OFF
# endif /* EthIf_CfgAccess_GetTxContextByContextTuple() availability check */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453,3458 FCT_LIKE_MACROS */ /* MISRA 19.4,19.7: Macros improve code readability */

/* Configuration access (depending on configuration variant)*/
/* EthIf related */
# define EthIf_CfgAccess_FrameOwnerNum()                         (ETHIF_FRAME_OWNER_NUM)
# define EthIf_CfgAccess_MaxValidFrameOwnerIdx()                 (ETHIF_FRAME_OWNER_NUM - 1U)
# define EthIf_CfgAccess_InvalidFrameOwnerIdx()                  (ETHIF_FRAME_OWNER_NUM)
# define EthIf_CfgAccess_EthIfCtrlNum()                          (ETHIF_ETHIF_CTRL_NUM)
# define EthIf_CfgAccess_MaxValidEthIfCtrlIdx()                  (ETHIF_ETHIF_CTRL_NUM - 1U)
# define EthIf_CfgAccess_InvalidEthIfCtrlIdx()                   (ETHIF_ETHIF_CTRL_NUM)
# define EthIf_CfgAccess_MaxTxBufNum()                           (ETHIF_MAX_TX_BUFS_NUM)
# define EthIf_CfgAccess_InvalidTxBufIdx()                       (ETHIF_MAX_TX_BUFS_NUM)
# define EthIf_CfgAccess_EthIfCtrlMode(EthIfCtrlIdx)             (EthIf_EthIfCtrlMode[(EthIfCtrlIdx)])
# define EthIf_CfgAccess_EthIfCtrlInitMode(EthIfCtrlIdx)         (EthIf_EthIfCtrlInitMode[(EthIfCtrlIdx)])
# define EthIf_CfgAccess_EthIfCtrlLinkState(EthIfCtrlIdx)        (EthIf_EthIfCtrlLinkState[(EthIfCtrlIdx)])
# define EthIf_CfgAccess_IsCtrlInitLocked()                      (EthIf_IsCtrlInitLocked)
# define EthIf_CfgAccess_IsSetCtrlModeLocked()                   (EthIf_IsSetCtrlModeLocked)
# define EthIf_CfgAccess_EthIfCtrlLinkCnts(EthIfCtrlIdx)         (EthIf_EthIfCtrlLinkCnts[(EthIfCtrlIdx)])
# define EthIf_CfgAccess_LinkAggregationThreshold(EthIfCtrlIdx)  (EthIf_LinkAggregationThreshold[(EthIfCtrlIdx)])
# define EthIf_CfgAccess_TrcvLinkStateChgReload()                (ETHIF_ETHTRCV_LINK_STATE_CHG_MAIN_RELOAD)
# if ( ETHIF_DEV_ERROR_DETECT == STD_ON )
# define EthIf_CfgAccess_Mtu(EthIfCtrlIdx)                       (EthIf_Mtu[(EthIfCtrlIdx)])
# endif /* ETHIF_DEV_ERROR_DETECT */
# if ( ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT == STD_ON )
#  define EthIf_CfgAccess_MgmtRxCtxtHandling(EthCtrlIdx)         (EthIf_MgmtRxCtxtHandling[(EthCtrlIdx)])
# endif /* ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT */
# if ( ETHIF_ETHSWT_TIME_STAMP_SUPPORT == STD_ON )
#  define EthIf_CfgAccess_SwtTimeStampTxCtxtHandling(EthCtrlIdx) (EthIf_SwtTimeStampTxCtxtHandling[(EthCtrlIdx)])
#  define EthIf_CfgAccess_SwtTimeStampRxCtxtHandling(EthCtrlIdx) (EthIf_SwtTimeStampRxCtxtHandling[(EthCtrlIdx)])
# endif /* ETHIF_ETHSWT_TIME_STAMP_SUPPORT */

/* EthIf users related */
# define EthIf_CfgAccess_RxIndInfo(OwnerIdx)                     (EthIf_OwnerRxIndInfos[(OwnerIdx)])
# define EthIf_CfgAccess_TxConfInfo(OwnerIdx)                    (EthIf_OwnerTxConfInfos[(OwnerIdx)])
# define EthIf_CfgAccess_UserLinkChgCbkInfo()                    (EthIf_UserLinkChgCbkInfo)
# if ( ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT == STD_ON )
#  define EthIf_CfgAccess_OwnerMgmtInfoIndInfo(OwnerIdx)         (EthIf_OwnerMgmtInfoIndInfos[(OwnerIdx)])
# endif /* ETHIF_ETHSWT_FRAME_MANAGEMENT_SUPPORT */
#  define EthIf_CfgAccess_OwnerSwtTsIndsInfo(OwnerIdx)           (EthIf_OwnerSwtTsIndsInfos[(OwnerIdx)])

/* EthCtrl related */
# define EthIf_CfgAccess_EthCtrlNum()                            (ETHIF_ETHCTRL_NUM)
# define EthIf_CfgAccess_EthDrvNum()                             (ETHIF_ETHCTRL_DRIVERS_NUM)
# define EthIf_CfgAccess_EthCtrlSummary(EthCtrlIdx)              (EthIf_EthCtrlSummary[(EthCtrlIdx)])

# if ( ETHIF_ETHSWT_USED == STD_ON )
/* EthSwt related */
#  define EthIf_CfgAccess_EthSwtNum()                             (ETHIF_ETHSWT_NUM)
#  define EthIf_CfgAccess_EthSwtPortNum()                         (ETHIF_ETHSWTPORT_NUM)
#  define EthIf_CfgAccess_EthSwtDrvNum()                          (ETHIF_ETHSWT_DRIVERS_NUM)
#  define EthIf_CfgAccess_EthSwtSummary(EthSwtIdx)                (EthIf_EthSwtSummary[(EthSwtIdx)])
#  define EthIf_CfgAccess_EthSwtPortSummary(EthSwtPortIdx)        (EthIf_EthSwtPortSummary[(EthSwtPortIdx)])
#  define EthIf_CfgAccess_EthSwtDrvSummary(EthSwtDrvIdx)          (EthIf_EthSwtDrvSummary[(EthSwtDrvIdx)])
#  define EthIf_CfgAccess_EthIfSwtPortGroupNum()                  (ETHIF_SWT_PORT_GROUP_NUM)
#  define EthIf_CfgAccess_EthIfSwtPortGroupSummary(EthIfSwtPortGroupIdx)                  \
              (EthIf_SwtPortGroupSummary[(EthIfSwtPortGroupIdx)])
#  define EthIf_CfgAccess_EthIfSwtPortGroupMode(EthIfSwtPortGroupIdx)                     \
              (EthIf_EthIfSwtPortGroupMode[(EthIfSwtPortGroupIdx)])
#  define EthIf_CfgAccess_EthIfSwtPortGroupLinkState(EthIfSwtPortGroupIdx)                \
              (EthIf_EthIfSwtPortGroupLinkState[(EthIfSwtPortGroupIdx)])
#  define EthIf_CfgAccess_EthIfSwtPortGroupLinkCnts(EthIfSwtPortGroupIdx)                 \
              (EthIf_EthIfSwtPortGroupLinkCnts[(EthIfSwtPortGroupIdx)])
#  define EthIf_CfgAccess_EthIfSwtPortGroupLinkAggregationThreshold(EthIfSwtPortGroupIdx) \
              (EthIf_SwtPortGroupLinkAggregationThreshold[(EthIfSwtPortGroupIdx)])
# endif /* ETHIF_ETHSWT_USED */

# if ( ETHIF_ETHTRCV_USED == STD_ON )
/* EthTrcv related */
#  define EthIf_CfgAccess_EthTrcvNum()                            (ETHIF_ETHTRCV_NUM)
#  define EthIf_CfgAccess_EthTrcvLinkStateChgFctNum()             (ETHIF_ETHTRCV_LINK_STATE_CHG_FCT_NUM)
#  define EthIf_CfgAccess_EthTrcvLinkStateChgReload()             (ETHIF_ETHTRCV_LINK_STATE_CHG_MAIN_RELOAD)
#  define EthIf_CfgAccess_EthTrcvSummary(EthTrcvIdx)              (EthIf_EthTrcvSummary[(EthTrcvIdx)])
# endif /* ETHIF_ETHTRCV_USED */

# define EthIf_CfgAccess_FrameOwner(OwnerIdx)                     (EthIf_FrameOwners[(OwnerIdx)])
# define EthIf_CfgAccess_VirtCtrl(EthIfCtrlIdx)                   (EthIf_VirtCtrls[(EthIfCtrlIdx)])
# define EthIf_CfgAccess_VirtCtrlBase4EthCtrl(EthCtrlIdx)         (EthIf_VirtCtrls[EthIf_EthCtrlSummary[(EthCtrlIdx)]->VirtCtrlOffset])

# if ( ETHIF_ENABLE_ZERO_COPY_EXTENSIONS == STD_ON )
#  define EthIf_CfgAccess_ExplicitBufferReleaseFilterNum()        (ETHIF_EXPLICIT_BUFERF_RELEASE_FILTER_NUM)
#  define EthIf_CfgAccess_ExplicitBufferReleaseFilter(FilterIdx)  (EthIf_ExplicitBufferReleaseFilters[(FilterIdx)])
# endif /* ETHIF_ENABLE_ZERO_COPY_EXTENSIONS */

# if ( ETHIF_WAKEUP_SUPPORT == STD_ON )
#  define EthIf_CfgAccess_WakeupMapNum()                          (ETHIF_WAKEUPMAP_NUM)
#  define EthIf_CfgAccess_MaxValidWakeupMapIdx()                  (ETHIF_WAKEUPMAP_NUM - 1U)
#  define EthIf_CfgAccess_InvalidWakeupMapIdx()                   (ETHIF_WAKEUPMAP_NUM)
#  define EthIf_CfgAccess_WakeupMap(WakeupMapIdx)                 (EthIf_WakeupMap[(WakeupMapIdx)])
# endif /* ETHIF_WAKEUP_SUPPORT */

/* PRQA L:FCT_LIKE_MACROS */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETHIF_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if ( ETHIF_INLINE_API_CFG_GET_TX_CONTEXT_BY_INDEX == STD_ON )
/**********************************************************************************************************************
 *  EthIf_CfgAccess_GetTxContextByIndex
 *********************************************************************************************************************/
/*! \brief       Gains access to the tx context data structure of config data
 *  \param[in]   txContextIdx  Index of the tx context element that shall be returned
 *  \return      Tx context element
 *********************************************************************************************************************/
ETHIF_INLINE FUNC(P2VAR(EthIf_TxContextType, AUTOMATIC, AUTOMATIC), ETHIF_CODE) EthIf_CfgAccess_GetTxContextByIndex(
  uint8  txContextIdx);
# endif /* ETHIF_INLINE_API_CFG_GET_TX_CONTEXT_BY_INDEX */

# if ( ETHIF_INLINE_API_CFG_GET_TX_CONTEXT_BY_CONTEXT_TUPLE == STD_ON )
/**********************************************************************************************************************
 *  EthIf_CfgAccess_GetTxContextByContextTuple
 *********************************************************************************************************************/
/*! \brief       Gains access to the tx context data structure of config data
 *  \param[in]   ethCtrlIdx    Eth controller index defining the tx context tuple
 *  \param[in]   bufIdx        Buffer index related to Eth controller defining the tx context tuple
 *  \return      Tx context element
 *********************************************************************************************************************/
ETHIF_INLINE FUNC(P2VAR(EthIf_TxContextType, AUTOMATIC, AUTOMATIC), ETHIF_CODE) EthIf_CfgAccess_GetTxContextByContextTuple(
  uint8  ethCtrlIdx,
  uint8  bufIdx);
# endif /* ETHIF_INLINE_API_CFG_GET_TX_CONTEXT_BY_CONTEXT_TUPLE */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# if ( ETHIF_INLINE_API_CFG_GET_TX_CONTEXT_BY_INDEX == STD_ON )
/**********************************************************************************************************************
 *  EthIf_CfgAccess_GetTxContextByIndex
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
ETHIF_INLINE FUNC(P2VAR(EthIf_TxContextType, AUTOMATIC, AUTOMATIC), ETHIF_CODE) EthIf_CfgAccess_GetTxContextByIndex(
  uint8  txContextIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(EthIf_TxContextType, AUTOMATIC, AUTOMATIC) result = NULL_PTR;

  /* ----- Implementation ----------------------------------------------- */
  if( txContextIdx < EthIf_CfgAccess_MaxTxBufNum() )
  {
    result = &EthIf_TxContext[txContextIdx];
  }

  return result;
} /* EthIf_CfgAccess_GetTxContextByIndex() */
# endif /* ETHIF_INLINE_API_CFG_GET_TX_CONTEXT_BY_INDEX */

# if ( ETHIF_INLINE_API_CFG_GET_TX_CONTEXT_BY_CONTEXT_TUPLE == STD_ON )
/**********************************************************************************************************************
 *  EthIf_CfgAccess_GetTxContextByContextTuple
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
ETHIF_INLINE FUNC(P2VAR(EthIf_TxContextType, AUTOMATIC, AUTOMATIC), ETHIF_CODE) EthIf_CfgAccess_GetTxContextByContextTuple(
  uint8  ethCtrlIdx,
  uint8  bufIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 txContextIdx = EthIf_CfgAccess_EthCtrlSummary(ethCtrlIdx)->TxBufOwnerOffset + bufIdx;

  /* ----- Implementation ----------------------------------------------- */
  return EthIf_CfgAccess_GetTxContextByIndex(txContextIdx);
} /* EthIf_CfgAccess_GetTxContextByContextTuple() */
# endif /* ETHIF_INLINE_API_CFG_GET_TX_CONTEXT_BY_CONTEXT_TUPLE */

# define ETHIF_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* ETHIF_CFG_ACCESS_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthIf_CfgAccess_Int.h
 *********************************************************************************************************************/

