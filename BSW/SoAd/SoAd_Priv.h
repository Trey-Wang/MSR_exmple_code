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
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  SoAd_Priv.h
 *        \brief  Socket Adaptor private header file
 *
 *      \details  Vector static private header file for AUTOSAR Socket Adaptor module. This header file contains
 *                module internal declarations.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined(SOAD_PRIV_H)
# define SOAD_PRIV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "SoAd_Types.h"
# include "SchM_SoAd.h"

/**********************************************************************************************************************
 * DATA TYPE MACROS
 *********************************************************************************************************************/
/* PRQA S 3453,3458 FCT_LIKE_MACROS */ /* MD_MSR_19.4, MD_MSR_19.7 */
# define SOAD_A_P2VAR(Type)                (P2VAR(Type, AUTOMATIC, AUTOMATIC))
# define SOAD_A_P2CONST(Type)              (P2CONST(Type, AUTOMATIC, AUTOMATIC))

# define SOAD_D_P2VAR(Type)                (P2VAR(Type, AUTOMATIC, SOAD_APPL_DATA))
# define SOAD_D_P2CONST(Type)              (P2CONST(Type, AUTOMATIC, SOAD_APPL_DATA))

# define SOAD_V_P2VAR(Type)                (P2VAR(Type, AUTOMATIC, SOAD_APPL_VAR))
# define SOAD_V_P2CONST(Type)              (P2CONST(Type, AUTOMATIC, SOAD_APPL_VAR))

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# define SOAD_BEGIN_CRITICAL_SECTION() \
  { \
    if ( 0xFFu > SoAd_CriticalSectionCount ) \
    { \
      if ( 0u == SoAd_CriticalSectionCount ) \
      { \
        SchM_Enter_SoAd_SOAD_EXCLUSIVE_AREA_0(); \
      } \
      SoAd_CriticalSectionCount++; \
    } \
  }
# define SOAD_END_CRITICAL_SECTION() \
  { \
    if ( 0x00u < SoAd_CriticalSectionCount ) \
    { \
      if ( 1u == SoAd_CriticalSectionCount ) \
      { \
        SchM_Exit_SoAd_SOAD_EXCLUSIVE_AREA_0(); \
      } \
      SoAd_CriticalSectionCount--; \
    } \
  }

# define SOAD_DUMMY_STATEMENT_TYPE_VOID  0u
# define SOAD_DUMMY_STATEMENT_TYPE_VAR   1u

# if ( !defined(SOAD_DUMMY_STATEMENT_TYPE) )
#  define SOAD_DUMMY_STATEMENT_VAR(v)         SOAD_DUMMY_STATEMENT(v)
#  define SOAD_DUMMY_STATEMENT_CONST_VAR(v)   SOAD_DUMMY_STATEMENT_CONST(v)
# else
#  if ( SOAD_DUMMY_STATEMENT_TYPE_VOID == SOAD_DUMMY_STATEMENT_TYPE )
#   define SOAD_DUMMY_STATEMENT_VAR(v)        (void)(v);
#   define SOAD_DUMMY_STATEMENT_CONST_VAR(v)  (void)(v);
#  else
#   define SOAD_DUMMY_STATEMENT_VAR(v)        (v)=(v);
#   define SOAD_DUMMY_STATEMENT_CONST_VAR(v)  SOAD_DUMMY_STATEMENT_CONST(v)
#  endif /* SOAD_DUMMY_STATEMENT_TYPE_VOID == SOAD_DUMMY_STATEMENT_TYPE */
# endif /* !defined(SOAD_DUMMY_STATEMENT_VAR) */

/* --- Pointers always present ------------------------------------------------------------------------------------- */
# define SoAd_EventQueueFlags(HandleIdx)               (SoAd_ConfigPtr->EventQueueFlagsListPtr[(HandleIdx)])
                                                      /* SoAd_VEventQueueFlags[] */
# define SoAd_EventQueues(EveQueIdx)                   (SoAd_ConfigPtr->EventQueuesListPtr[(EveQueIdx)])
                                                      /* SoAd_VEventQueues[] */
# define SoAd_SockConIdx2RemSockDyn(SoConIdx)          (SoAd_ConfigPtr->SockConIdx2RemSockDynListPtr[(SoConIdx)])
                                                      /* SoAd_VSockConIdx2RemSockDyn[] */
# define SoAd_SockConIdx2State(SoConIdx)               (SoAd_ConfigPtr->SockConIdx2StateListPtr[(SoConIdx)])
                                                      /* SoAd_VSockConIdx2State[] */
# define SoAd_SockConIdx2IpAddrState(SoConIdx)         (SoAd_ConfigPtr->SockConIdx2IpAddrStateListPtr[(SoConIdx)])
                                                      /* SoAd_VSockConIdx2IpAddrState[] */
# define SoAd_SockIdx2LocPortDyn(SockIdx)              (SoAd_ConfigPtr->SockIdx2LocPortDynListPtr[(SockIdx)])
                                                      /* SoAd_VSockIdx2LocPortDyn[] */
# define SoAd_SockIdx2SocketId(SockIdx)                (SoAd_ConfigPtr->SockIdx2SocketIdListPtr[(SockIdx)])
                                                      /* SoAd_VSockIdx2SocketId[] */
# define SoAd_SockConIdx2FlagAbort(SoConIdx)           (SoAd_ConfigPtr->SockConIdx2FlagAbortListPtr[(SoConIdx)])
                                                      /* SoAd_VSockConIdx2FlagAbort[] */
# define SoAd_SockConIdx2CloseMode(SoConIdx)           (SoAd_ConfigPtr->SockConIdx2CloseModeListPtr[(SoConIdx)])
                                                      /* SoAd_VSockConIdx2CloseMode[] */
# define SoAd_SockIdx2SockState(SockIdx)               (SoAd_ConfigPtr->SockIdx2SockStateListPtr[(SockIdx)])
                                                      /* SoAd_VSockIdx2SockState[] */
# define SoAd_SockConIdx2FlagCntOpCl(SoConIdx)         (SoAd_ConfigPtr->SockConIdx2FlagCntOpClListPtr[(SoConIdx)])
                                                      /* SoAd_VSockConIdx2FlagCntOpCl[] */
# define SoAd_SockConIdx2RemAddrState(SoConIdx)        (SoAd_ConfigPtr->SockConIdx2RemAddrStateListPtr[(SoConIdx)])
                                                      /* SoAd_VSockConIdx2RemAddrState[] */
# define SoAd_SockCons(SoConIdx)                       (SoAd_ConfigPtr->SockConsListPtr[(SoConIdx)])
                                                      /* SoAd_VSockCons[] */
# define SoAd_SockIdx2SoConIdxCol(SockIdx)             (SoAd_ConfigPtr->SockIdx2SoConIdxColListPtr[(SockIdx)])
                                                      /* SoAd_VSockIdx2SoConIdxCol[] */
# define SoAd_SockConId2SockConIdx(SoConId)            (SoAd_ConfigPtr->SockConId2SockConIdxMapPtr[(SoConId)])
                                                      /* SoAd_VSockConId2SockConIdx[] */
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Pointers present if SoAdPduRoutes configured ---------------------------------------------------------------- */
/* Check by SoAd_PduRouteTotal() and SoAd_PduRouteTotalWithInv() or SoAd_IsPduRoutePresent() */
# define SoAd_IsPduRoutePresent()                      (0 != SoAd_PduRouteTotal())
# define SoAd_SockConIdx2TxMgt(SoConIdx)               (SoAd_ConfigPtr->SockConIdx2TxMgtListPtr[(SoConIdx)])
                                                      /* SoAd_VSockConIdx2TxMgt[] */
# define SoAd_SockIdx2ActiveSoConIdx(SockIdx)          (SoAd_ConfigPtr->SockIdx2ActiveSoConIdxListPtr[(SockIdx)])
                                                      /* SoAd_VSockIdx2ActiveSoConIdx[] */
# define SoAd_TxPduIdx2PendingTxConf(TxPduIdx)         (SoAd_ConfigPtr->TxPduIdx2PendingTxConfListPtr[(TxPduIdx)])
                                                      /* SoAd_VTxPduIdx2PendingTxConf[] */
# define SoAd_SockConIdx2PduRouteDestCol(SoConIdx)     (SoAd_ConfigPtr->SockConIdx2PduRouteDestColListPtr[(SoConIdx)])
                                                      /* SoAd_VSockConIdx2PduRouteDestCol[] */
# define SoAd_TxPduIdx2PduRoute(TxPduIdx)              (SoAd_ConfigPtr->TxPduIdx2PduRouteListPtr[(TxPduIdx)])
                                                      /* SoAd_VTxPduIdx2PduRoute[] */
# define SoAd_TxPduIdx2TxConfPduId(TxPduIdx)           (SoAd_ConfigPtr->TxPduIdx2TxConfPduIdListPtr[(TxPduIdx)])
                                                      /* SoAd_VTxPduIdx2TxConfPduId[] */
# define SoAd_SockIdx2TxPduIdCol(SockIdx)              (SoAd_ConfigPtr->SockIdx2TxPduIdColListPtr[(SockIdx)])
                                                      /* SoAd_VSockIdx2TxPduIdCol[] */
# define SoAd_TxPduId2PduRouteIdx(TxPduId)             (SoAd_ConfigPtr->TxPduId2PduRouteIdxMapPtr[(TxPduId)])
                                                      /* SoAd_VTxPduId2PduRouteIdx[] */
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Pointers present if SoAdSocketRoutes configured ------------------------------------------------------------- */
/* Check by SoAd_SockRouteTotal(), SoAd_SockRouteTotalWithInv() or SoAd_IsSockRoutePresent() */
# define SoAd_IsSockRoutePresent()                     (0 != SoAd_SockRouteTotal())
# define SoAd_SockConIdx2RxMgt(SoConIdx)               (SoAd_ConfigPtr->SockConIdx2RxMgtListPtr[(SoConIdx)])
                                                      /* SoAd_VSockConIdx2RxMgt[] */
# define SoAd_SockIdx2SkipBytes(SockIdx)               (SoAd_ConfigPtr->SockIdx2SkipBytesListPtr[(SockIdx)])
                                                      /* SoAd_VSockIdx2SkipBytes[] */
# define SoAd_SockConIdx2RxBuffer(SoConIdx)            (SoAd_ConfigPtr->SockConIdx2RxBufferListPtr[(SoConIdx)])
                                                      /* SoAd_VSockConIdx2RxBuffer[] */
# define SoAd_SockRoutes(RouteIdx)                     (SoAd_ConfigPtr->SockRoutesListPtr[(RouteIdx)])
                                                      /* SoAd_VSockRoutes[] */
# define SoAd_SockConIdx2SockRouteCol(SoConIdx)        (SoAd_ConfigPtr->SockConIdx2SockRouteColListPtr[(SoConIdx)])
                                                      /* SoAd_VSockConIdx2SockRouteCol[] */
# define SoAd_RxPduId2SockRouteIdx(RxPduId)            (SoAd_ConfigPtr->RxPduId2SockRouteIdxMapPtr[(RxPduId)])
                                                      /* SoAd_VRxPduId2SockRouteIdx[] */
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Pointers present if NPDU UDP Tx SoCons configured ----------------------------------------------------------- */
/* Check by SoAd_SockConIdx2NPduUdpTxIdxSize() or SoAd_IsNPduUdpTxPresent() */
# define SoAd_IsNPduUdpTxPresent()                     (0 != SoAd_ConfigPtr->SockConIdx2NPduUdpTxIdxSize)
# define SoAd_SockConIdx2NPduUdpTxIdx(SoConIdx)        (SoAd_ConfigPtr->SockConIdx2NPduUdpTxIdxListPtr[(SoConIdx)])
                                                      /* SoAd_VSockConIdx2NPduUdpTxIdx[] */
# define SoAd_NPduUdpTx(NPduUdpTxIdx)                  (SoAd_ConfigPtr->NPduUdpTxListPtr[(NPduUdpTxIdx)])
                                                      /* SoAd_VNPduUdpTx[] */
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Pointers present if TCP IF Trigger Transmit SoCons configured ----------------------------------------------- */
# define SoAd_SockConIdx2IfTriggerTransmitBufIdx(SoConIdx) \
                                                (SoAd_ConfigPtr->SockConIdx2IfTriggerTransmitBufIdxListPtr[(SoConIdx)])
                                                      /* SoAd_VSockConIdx2IfTriggerTransmitBufIdx[] */
# define SoAd_IfTriggerTransmitBuffer(BufIdx)          (SoAd_ConfigPtr->IfTriggerTransmitBufferListPtr[(BufIdx)])
                                                      /* SoAd_VTriggerTransmitBuffer[] */
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Pointers present if Tp UDP SockCons configured --------------------------------------------------------------- */
# define SoAd_IsSockConIdx2TxBufferPresent()           (NULL_PTR != SoAd_ConfigPtr->SockConIdx2TxBufferListPtr)
# define SoAd_SockConIdx2TxBuffer(SoConIdx)            (SoAd_ConfigPtr->SockConIdx2TxBufferListPtr[(SoConIdx)])
                                                      /* SoAd_VSockConIdx2TxBuffer[] */
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Pointers present if SoAdRoutingGroups configured ------------------------------------------------------------- */
/* Check by SoAd_RouteGrpTotal() and SoAd_RouteGrpTotalWithInv() */
# define SoAd_RouteGrpIdx2RoutingGroup(RoutGrpIdx)     (SoAd_ConfigPtr->RouteGrpIdx2RoutingGroupListPtr[(RoutGrpIdx)])
                                                      /* SoAd_VRouteGrpIdx2RoutingGroup[] */
# define SoAd_RouteGrpId2RouteGrpIdx(RouteGrpId)       (SoAd_ConfigPtr->RouteGrpId2RouteGrpIdxMapPtr[(RouteGrpId)])
                                                      /* SoAd_VRouteGrpId2RouteGrpIdx[] */
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Pointers present if SoAdPduRoutes with configured SoAdRoutingGroups exists ----------------------------------- */
# define SoAd_IsRouteGrpIdx2PduRouteDestColPresent()   (NULL_PTR != SoAd_ConfigPtr->RouteGrpIdx2PduRouteDestColListPtr)
# define SoAd_RouteGrpIdx2PduRouteDestCol(RoutGrpIdx)  (SoAd_ConfigPtr->RouteGrpIdx2PduRouteDestColListPtr[\
                                                      (RoutGrpIdx)])
                                                      /* SoAd_VRouteGrpIdx2PduRouteDestCol[] */
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Pointers present if SoAdSocketRoutes with configured SoAdRoutingGroups exists -------------------------------- */
# define SoAd_IsRouteGrpIdx2SocketRouteColPresent()    (NULL_PTR != SoAd_ConfigPtr->RouteGrpIdx2SocketRouteColListPtr)
# define SoAd_RouteGrpIdx2SocketRouteCol(RoutGrpIdx)   (SoAd_ConfigPtr->RouteGrpIdx2SocketRouteColListPtr[(RoutGrpIdx)])
                                                      /* SoAd_VRouteGrpIdx2SocketRouteCol[] */
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Pointers present if TriggerTransmit configured --------------------------------------------------------------- */
# define SoAd_IsIfRouteGrpTransmitBufPtrPresent()      (NULL_PTR != SoAd_ConfigPtr->IfRouteGrpTransmitBufPtr)
# define SoAd_IfRouteGrpTransmitBufPtr()               (&(SoAd_ConfigPtr->IfRouteGrpTransmitBufPtr[0]))
                                                      /* SoAd_VIfRouteGrpTransmitBuf[] */
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Pointers present if any TimeoutList is configured -------------------------------------------------------------*/
# define SoAd_IsTimeoutListsPtrPresent()               (0u != SoAd_ConfigPtr->TimeoutListTotal)
# define SoAd_TimeoutLists(TimeListIdx)                (SoAd_ConfigPtr->TimeoutListsListPtr[(TimeListIdx)])
                                                      /* SoAd_VTimeoutLists[] */
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Pointers present if VerifyRxPduCallback is configured ------------------------------------------------------- */
# define SoAd_VerifyRxPduBuffer()                      (SoAd_ConfigPtr->VerifyRxPduBufferPtr)
                                                      /* SoAd_VVerifyRxPduBuffer[] */
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Pointers present if RxMetaData are configured --------------------------------------------------------------- */
# define SoAd_MetaDataRxBufferSize()                   (SoAd_ConfigPtr->MetaDataRxBufferSize)
# define SoAd_MetaDataRxBuffer()                       (SoAd_ConfigPtr->MetaDataRxBufferPtr)
                                                      /* SoAd_VMetaDataRxBuffer[] */
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Pointers present if BSD Socket API is disabled -------------------------------------------------------------- */
# define SoAd_AddrId2TcpIpCtrl(AddrId)                 (SoAd_ConfigPtr->AddrId2TcpIpCtrlPtr[(AddrId)])
                                                      /* SoAd_VAddrId2TcpIpCtrl[] */
# define SoAd_TcpIpCtrl(TcpIpCtrl)                     (SoAd_ConfigPtr->TcpIpCtrlPtr[(TcpIpCtrl)])
                                                      /* SoAd_VTcpIpCtrl[] */
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Pointers present if BSD Socket API is enabled (pre-compile) ------------------------------------------------- */
# define SoAd_LocalAddr(AddrId)                        (SoAd_ConfigPtr->LocalAddrPtr[(AddrId)])
                                                      /* SoAd_VLocalAddr[][] */
# define SoAd_AddrId2LocalIpAddr(AddrId)               (SoAd_ConfigPtr->AddrId2LocalIpAddrPtr[(AddrId)])
                                                      /* SoAd_VAddrId2LocalIpAddr[] */
# define SoAd_AddrId2LocalIpAddrDyn(AddrId)            (SoAd_ConfigPtr->AddrId2LocalIpAddrDynPtr[(AddrId)])
                                                      /* SoAd_VAddrId2LocalIpAddrDyn[] */
# define SoAd_AddrId2LocalIpAddrReqState(AddrId)       (SoAd_ConfigPtr->AddrId2LocalIpAddrReqStatePtr[(AddrId)])
                                                      /* SoAd_VAddrId2LocalIpAddrReqState[] */
# define SoAd_AddrId2SockIdxCol(AddrId)                (SoAd_ConfigPtr->AddrId2SockIdxColPtr[(AddrId)])
                                                      /* SoAd_VAddrId2SockIdxCol[] */
# define SoAd_SockIdx2MulticastAddr(SockIdx)           (SoAd_ConfigPtr->SockIdx2MulticastAddrPtr[(SockIdx)])
                                                      /* SoAd_VSockIdx2MulticastAddr[] */
# define SoAd_SockTxBufIdx2Mgt(SockTxBufIdx)           (SoAd_ConfigPtr->SockTxBufIdx2MgtPtr[(SockTxBufIdx)])
                                                      /* SoAd_VSockTxBufIdx2Mgt[] */
# define SoAd_SockIdx2RxMgt(SockIdx)                   (SoAd_ConfigPtr->SockIdx2RxMgtPtr[(SockIdx)])
                                                      /* SoAd_VSockIdx2RxMgt[] */
# define SoAd_SockIdx2RxBuf(SockIdx)                   (SoAd_ConfigPtr->SockIdx2RxBufPtr[(SockIdx)])
                                                      /* SoAd_VSockIdx2RxBuf[] */
# define SoAd_SocketTxBuffer()                         (SoAd_ConfigPtr->SocketTxBufferPtr)
                                                      /* SoAd_VSocketTxBuffer[] */
# define SoAd_SocketRxBuffer()                         (SoAd_ConfigPtr->SocketRxBufferPtr)
                                                      /* SoAd_VSocketRxBuffer[] */
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Sizes ------------------------------------------------------------------------------------------------------- */
# define SoAd_EventQueueFlagsArraySize()               (SoAd_ConfigPtr->EventQueueFlagsArraySize)
# define SoAd_EventQueueTotal()                        (SoAd_ConfigPtr->EventQueueTotal)
# define SoAd_TimeoutListTotal()                       (SoAd_ConfigPtr->TimeoutListTotal)
# define SoAd_SockConTotal()                           (SoAd_ConfigPtr->SockConTotal)
# define SoAd_SockConTotalWithInv()                    (SoAd_ConfigPtr->SockConTotalWithInv)
# define SoAd_RouteGrpTotal()                          (SoAd_ConfigPtr->RouteGrpTotal)
# define SoAd_RouteGrpTotalWithInv()                   (SoAd_ConfigPtr->RouteGrpTotalWithInv)
# define SoAd_SockRouteTotal()                         (SoAd_ConfigPtr->SockRouteTotal)
# define SoAd_SockRouteTotalWithInv()                  (SoAd_ConfigPtr->SockRouteTotalWithInv)
# define SoAd_PduRouteTotal()                          (SoAd_ConfigPtr->PduRouteTotal)
# define SoAd_PduRouteTotalWithInv()                   (SoAd_ConfigPtr->PduRouteTotalWithInv)
# define SoAd_TcpIpSocketCnt()                         (SoAd_ConfigPtr->TcpIpSocketCnt)
# define SoAd_IfRouteGrpTransmitBufSize()              (SoAd_ConfigPtr->IfRouteGrpTransmitBufSize)
# define SoAd_MaxIfRouteGrpTransmitPduSize()           (SoAd_ConfigPtr->MaxIfRouteGrpTransmitPduSize)
# define SoAd_SockConIdx2NPduUdpTxIdxSize()            (SoAd_ConfigPtr->SockConIdx2NPduUdpTxIdxSize)
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Indexs ------------------------------------------------------------------------------------------------------ */
/* check presence by following defines */
# define SoAd_IsEventQueueIdxSockIdxPresent()          (SoAd_ConfigPtr->EventQueueIdxSockIdx != 0xFFu)
# define SoAd_IsEventQueueIdxTpRxSoConPresent()        (SoAd_ConfigPtr->EventQueueIdxTpRxSoCon != 0xFFu)
# define SoAd_IsEventQueueIdxLocalAddrPresent()        (SoAd_ConfigPtr->EventQueueIdxLocalAddr != 0xFFu)
# define SoAd_IsEventQueueIdxStateSoConPresent()       (SoAd_ConfigPtr->EventQueueIdxStateSoCon != 0xFFu)
# define SoAd_IsEventQueueIdxTpTxSoConPresent()        (SoAd_ConfigPtr->EventQueueIdxTpTxSoCon != 0xFFu)
# define SoAd_IsEventQueueIdxTcpTxSoConPresent()       (SoAd_ConfigPtr->EventQueueIdxTcpTxSoCon != 0xFFu)
# define SoAd_IsEventQueueIdxIfUdpPduRoutePresent()    (SoAd_ConfigPtr->EventQueueIdxIfUdpPduRoute != 0xFFu)
# define SoAd_IsEventQueueIdxIfTxRouteGrpPresent()     (SoAd_ConfigPtr->EventQueueIdxIfTxRouteGrp != 0xFFu)
# define SoAd_IsTimeoutListIdxUdpAlivePresent()        (SoAd_ConfigPtr->TimeoutListIdxUdpAlive != 0xFFu)
# define SoAd_IsTimeoutListIdxNPduUdpTxPresent()       (SoAd_ConfigPtr->TimeoutListIdxNPduUdpTx != 0xFFu)

# define SoAd_EventQueueIdxSockIdx()                   (SoAd_ConfigPtr->EventQueueIdxSockIdx)
# define SoAd_EventQueueIdxTpRxSoCon()                 (SoAd_ConfigPtr->EventQueueIdxTpRxSoCon)
# define SoAd_EventQueueIdxLocalAddr()                 (SoAd_ConfigPtr->EventQueueIdxLocalAddr)
# define SoAd_EventQueueIdxStateSoCon()                (SoAd_ConfigPtr->EventQueueIdxStateSoCon)
# define SoAd_EventQueueIdxTpTxSoCon()                 (SoAd_ConfigPtr->EventQueueIdxTpTxSoCon)
# define SoAd_EventQueueIdxTcpTxSoCon()                (SoAd_ConfigPtr->EventQueueIdxTcpTxSoCon)
# define SoAd_EventQueueIdxIfUdpPduRoute()             (SoAd_ConfigPtr->EventQueueIdxIfUdpPduRoute)
# define SoAd_EventQueueIdxIfTxRouteGrp()              (SoAd_ConfigPtr->EventQueueIdxIfTxRouteGrp)
# define SoAd_TimeoutListIdxUdpAlive()                 (SoAd_ConfigPtr->TimeoutListIdxUdpAlive)
# define SoAd_TimeoutListIdxNPduUdpTx()                (SoAd_ConfigPtr->TimeoutListIdxNPduUdpTx)
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Sizes if BSD Socket API is enabled (pre-Compile) ------------------------------------------------------------ */
# define SoAd_AddrIdTotal()                            (SoAd_ConfigPtr->AddrIdTotal)
# define SoAd_SocketTxBufferSize()                     (SoAd_ConfigPtr->SocketTxBufferSize)
# define SoAd_SocketTxBufferNum()                      (SoAd_ConfigPtr->SocketTxBufferNum)
# define SoAd_SocketRxBufferSize()                     (SoAd_ConfigPtr->SocketRxBufferSize)
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Parameters -------------------------------------------------------------------------------------------------- */
# define SoAd_TcpKeepAliveEnabled()                    (SoAd_ConfigPtr->TcpKeepAliveEnabled)
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Parameters if BSD Socket API is enabled (pre-Compile) ------------------------------------------------------- */
# define SoAd_LingerTimeoutEnabled()                   (SoAd_ConfigPtr->LingerTimeoutEnabled)
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Validate SoConId -------------------------------------------------------------------------------------------- */
# define SoAd_IsSoConIdValid(SOCONID) \
        (((SOCONID) < SoAd_SockConTotalWithInv()) && \
          (SoAd_SockConId2SockConIdx((SOCONID)) != SOAD_INV_SO_CON_IDX))
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Validate RouteGrpId ----------------------------------------------------------------------------------------- */
# define SoAd_IsRouteGrpIdValid(ROUTEGRPID) \
        (((ROUTEGRPID) < SoAd_RouteGrpTotalWithInv()) && \
          (SoAd_RouteGrpId2RouteGrpIdx((ROUTEGRPID)) != SOAD_INV_ROUT_GRP_IDX))
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Validate RxPduId -------------------------------------------------------------------------------------------- */
# define SoAd_IsRxPduIdValid(RXPDUID) \
        (((RXPDUID) < SoAd_SockRouteTotalWithInv()) && \
          (SoAd_RxPduId2SockRouteIdx((RXPDUID)) != SOAD_INV_SOCKROUTE_IDX))
/* ----------------------------------------------------------------------------------------------------------------- */

/* --- Validate TxPduId -------------------------------------------------------------------------------------------- */
# define SoAd_IsTxPduIdValid(TXPDUID) \
        (((TXPDUID) < SoAd_PduRouteTotalWithInv()) && \
          (SoAd_TxPduId2PduRouteIdx((TXPDUID)) != SOAD_INV_PDUROUTE_IDX))
/* ----------------------------------------------------------------------------------------------------------------- */

# if !defined(SOAD_CONFIGURATION_VARIANT)
#  error "SoAd_Priv.h: Missing SOAD_CONFIGURATION_VARIANT"
# endif /* defined(SOAD_CONFIGURATION_VARIANT) */

/* PRQA L:FCT_LIKE_MACROS */

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SoAd_AssignUdpSocket()
 *********************************************************************************************************************/
/*! \brief      Assigns a UDP socket to a socket connection group
 *  \details    -
 *  \param[in]  SoConIdx    Socket connection index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_AssignUdpSocket(SoAd_SoConIdxType SoConIdx);

# if ( SOAD_VSOCKET_ROUTES == STD_ON )
#  if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_WriteBufSegment()
 *********************************************************************************************************************/
/*! \brief        Writes buffer segment to a segmented buffer management struct
 *  \details      -
 *  \param[in]    SrcBufPtr     Pointer to source buffer
 *  \param[in]    SrcBufLen     Length of available data
 *  \param[out]   TgtBufSegPtr  Pointer to target segmented buffer management struct
 *  \return       E_OK          Segment written into segmented buffer management struct.
 *  \return       E_NOT_OK      Segment not written into segmented buffer management struct.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \config       SOAD_VSOCKET_ROUTES & SOAD_VTCP_USED
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_WriteBufSegment(
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) SrcBufPtr,
  uint16 SrcBufLen,
  P2VAR(SoAd_BufSegMgtType, AUTOMATIC, SOAD_APPL_DATA) TgtBufSegPtr);

/**********************************************************************************************************************
 *  SoAd_GetBufSegment()
 *********************************************************************************************************************/
/*! \brief        Gets a buffer segment from a segmented buffer management struct
 *  \details      -
 *  \param[in]    BufSegPtr     Pointer to segmented buffer management struct
 *  \param[in]    StartPos      Start position in segment
 *  \param[out]   SegPtr        Pointer to start of segment
 *  \param[out]   SegLen        Pointer to length of segment
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \config       SOAD_VSOCKET_ROUTES & SOAD_VTCP_USED
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_GetBufSegment(
  P2CONST(SoAd_BufSegMgtType, AUTOMATIC, SOAD_APPL_DATA) BufSegPtr,
  uint32 StartPos,
  P2VAR(uint8*, AUTOMATIC, SOAD_APPL_VAR) SegPtr,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) SegLen);

#   if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_GetBufSegmentTotLen()
 *********************************************************************************************************************/
/*! \brief      Gets total length of a segmented buffer management struct
 *  \details    -
 *  \param[in]  BufSegPtr   Pointer to segmented buffer management struct
 *  \param[in]  StartPos    Start position in segment
 *  \return     Length      Total Length of segmented buffer management struct
 *  \pre        -
 *  \context    TASK|ISR
 *  \reentrant  TRUE
 *  \config     SOAD_VSOCKET_ROUTES & SOAD_VTCP_USED & SOAD_VPDU_HDR_ENABLED
 *********************************************************************************************************************/
FUNC(uint32, SOAD_CODE) SoAd_GetBufSegmentTotLen(
  P2CONST(SoAd_BufSegMgtType, AUTOMATIC, SOAD_APPL_DATA) BufSegPtr,
  uint32 StartPos);
#   endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_ReleaseBufSegment()
 *********************************************************************************************************************/
/*! \brief      Releases a specific length of a segmented buffer management struct
 *  \details    -
 *  \param[in]  BufSegPtr   Pointer to segmented buffer management struct
 *  \param[in]  Len         Length to be released
 *  \pre        -
 *  \context    TASK|ISR
 *  \reentrant  TRUE
 *  \config     SOAD_VSOCKET_ROUTES & SOAD_VTCP_USED
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_ReleaseBufSegment(
  P2VAR(SoAd_BufSegMgtType, AUTOMATIC, AUTOMATIC) BufSegPtr,
  uint32 Len);
#  endif /* SOAD_VTCP_USED == STD_ON */
# endif /* SOAD_VSOCKET_ROUTES == STD_ON */

/**********************************************************************************************************************
 *  SoAd_CheckForWildcards()
 *********************************************************************************************************************/
/*! \brief        Checks socket address for wildcards
 *  \details      -
 *  \param[in]    AddrCheck     Pointer to socket address to be checked
 *  \param[out]   IpAddrIsAny   Pointer to variable indicating if IP is wildcard
 *  \param[out]   PortIsAny     Pointer to variable indicating if port is wildcard
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_CheckForWildcards(
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) AddrCheck,
  P2VAR(boolean, AUTOMATIC, SOAD_APPL_DATA) IpAddrIsAny,
  P2VAR(boolean, AUTOMATIC, SOAD_APPL_DATA) PortIsAny);

/**********************************************************************************************************************
 *  SoAd_CopyIpAddr()
 *********************************************************************************************************************/
/*! \brief        Copies a socket address to another socket address
 *  \details      -
 *  \param[in]    TgtIpAddr       Pointer to target socket address
 *  \param[out]   SrcIpAddr       Pointer to source socket address
 *  \param[in]    OverwriteAll    Overwrite all elements of socket address or only elements containing wildcard
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_CopyIpAddr(
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) TgtIpAddr,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) SrcIpAddr,
  boolean OverwriteAll);

# if ( SOAD_VPDU_HDR_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_CopyPduHdr2Buf()
 *********************************************************************************************************************/
/*! \brief          Copies a PDU header to a buffer
 *  \details        -
 *  \param[in]      PduHeaderId   PDU header identifier
 *  \param[in]      PduHeaderLen  PDU header length
 *  \param[in,out]  BufPtr        Pointer to buffer
 *  \param[in]      Offset        Offset in PDU header
 *  \param[in]      Length        Length of PDU header segment to be copied
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \config         SOAD_VPDU_HDR_ENABLED
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_CopyPduHdr2Buf(
  SoAd_PduHdrIdType PduHeaderId,
  SoAd_PduHdrLenType PduHeaderLen,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  PduLengthType Offset,
  PduLengthType Length);

#  if ( SOAD_VSOCKET_ROUTES == STD_ON )
#   if ( SOAD_VTCP_USED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_CopyBufSeg2Buf()
 *********************************************************************************************************************/
/*! \brief          Copies segmented buffer management struct segment to buffer
 *  \details        -
 *  \param[in,out]  BufPtr        Pointer to buffer
 *  \param[in]      BufSegPtr     Pointer to segmented buffer management struct
 *  \param[in]      BufLen        Length to be copied
 *  \param[in]      StartPos      Start position in segmented buffer management struct
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \config         SOAD_VPDU_HDR_ENABLED & SOAD_VSOCKET_ROUTES & SOAD_VTCP_USED
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_CopyBufSeg2Buf(
  P2VAR(uint8, SOAD_APPL_DATA, SOAD_APPL_DATA) BufPtr,
  P2CONST(SoAd_BufSegMgtType, SOAD_APPL_DATA, SOAD_APPL_DATA) BufSegPtr,
  uint32 BufLen,
  uint32 StartPos);
#   endif /* SOAD_VTCP_USED == STD_ON */
#  endif /* SOAD_VSOCKET_ROUTES == STD_ON */
# endif /* SOAD_VPDU_HDR_ENABLED == STD_ON */

#endif /* !defined(SOAD_PRIV_H) */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_Priv.h
 *********************************************************************************************************************/
