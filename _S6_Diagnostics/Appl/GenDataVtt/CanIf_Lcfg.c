/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                 Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                 All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  LICENSE
 *  -------------------------------------------------------------------------------------------------------------------
 *            Module: CanIf
 *           Program: 
 *          Customer: N/A
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: 
 *    License Scope : The usage is restricted to CBD0000000_D80
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: CanIf_Lcfg.c
 *   Generation Time: 2020-03-02 16:11:31
 *           Project: MyECU - Version 1.0
 *          Delivery: CBD0000000_D80
 *      Tool Version: DaVinci Configurator  5.16.50 SP5
 *
 *
 *********************************************************************************************************************/

#define CANIF_LCFG_SOURCE

/* PRQA S 0850 EOF */ /* MD_MSR_19.8 */
/* PRQA S 0857 EOF */ /* MD_CanIf_1.1_NumberOfMacroDefinitions */
/* PRQA S 0779 EOF */ /* MD_CanIf_5.1 */
/* PRQA S 0777 EOF */ /* MD_CanIf_5.1 */
/* PRQA S 1330, 1334 EOF */ /* MD_CanIf_16.4, MD_CanIf_16.4 */

/**********************************************************************************************************************
  \file  Includes
**********************************************************************************************************************/
/** 
  \brief  Required external files.
*/ 

#include "CanIf_Cfg.h"

 /*  CanNm Header Files  */ 
#include "CanNm_Cfg.h"
#include "CanNm_Cbk.h"
 /*  CanTp Header Files  */ 
#include "CanTp_Cfg.h"
#include "CanTp_Cbk.h"
 /*  PduR Header Files  */ 
#include "PduR_Cfg.h"
#include "PduR_CanIf.h"
 /*  CanSM Header Files  */ 
#include "CanSM_Cbk.h"

#define CANIF_START_SEC_APPL_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/**********************************************************************************************************************
  \var  Prototypes of callback functions
**********************************************************************************************************************/

/** 
  \brief  Tx confirmation functions.
*/



/** 
  \brief  Rx indication functions.
*/





#define CANIF_STOP_SEC_APPL_CODE
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
#include "MemMap.h"

/**********************************************************************************************************************
  ComStackLib
**********************************************************************************************************************/
/**********************************************************************************************************************
  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/


/**********************************************************************************************************************
  LOCAL DATA
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: LOCAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: LOCAL DATA
**********************************************************************************************************************/


/**********************************************************************************************************************
  GLOBAL DATA
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL DATA
**********************************************************************************************************************/
/**********************************************************************************************************************
  CanIf_BusOffNotificationFctPtr
**********************************************************************************************************************/
#define CANIF_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanIf_BusOffNotificationFctType, CANIF_CONST) CanIf_BusOffNotificationFctPtr = CanSM_ControllerBusOff;
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanIf_CanIfCtrlId2MappedTxBuffersConfig
**********************************************************************************************************************/
/** 
  \var    CanIf_CanIfCtrlId2MappedTxBuffersConfig
  \brief  CAN controller configuration - mapped Tx-buffer(s).
  \details
  Element                          Description
  MappedTxBuffersConfigEndIdx      the end index of the 1:n relation pointing to CanIf_MappedTxBuffersConfig
  MappedTxBuffersConfigStartIdx    the start index of the 1:n relation pointing to CanIf_MappedTxBuffersConfig
*/ 
#define CANIF_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanIf_CanIfCtrlId2MappedTxBuffersConfigType, CANIF_CONST) CanIf_CanIfCtrlId2MappedTxBuffersConfig[1] = {
    /* Index    MappedTxBuffersConfigEndIdx                                                 MappedTxBuffersConfigStartIdx                                                       Referable Keys */
  { /*     0 */                         1UL  /* /ActiveEcuC/CanIf/Can/CT_CAN00_e98c47e6 */,                           0UL  /* /ActiveEcuC/CanIf/Can/CT_CAN00_e98c47e6 */ }   /* [/ActiveEcuC/CanIf/Can/CT_CAN00_e98c47e6] */
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanIf_CtrlModeIndicationFctPtr
**********************************************************************************************************************/
#define CANIF_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanIf_CtrlModeIndicationFctType, CANIF_CONST) CanIf_CtrlModeIndicationFctPtr = CanSM_ControllerModeIndication;
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanIf_MailBoxConfig
**********************************************************************************************************************/
/** 
  \var    CanIf_MailBoxConfig
  \brief  Mailbox table.
  \details
  Element                 Description
  CtrlStatesIdx           the index of the 1:1 relation pointing to CanIf_CtrlStates
  PduIdFirst              "First" PDU mapped to mailbox.
  PduIdLast               "Last" PDU mapped to mailbox.
  TxBufferCfgIdx          the index of the 0:1 relation pointing to CanIf_TxBufferPrioByCanIdByteQueueConfig
  TxBufferHandlingType
  MailBoxType             Type of mailbox: Rx-/Tx- BasicCAN/FullCAN/unused.
*/ 
#define CANIF_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanIf_MailBoxConfigType, CANIF_CONST) CanIf_MailBoxConfig[2] = {
    /* Index    CtrlStatesIdx                                                 PduIdFirst                           PduIdLast                           TxBufferCfgIdx                                                                                        TxBufferHandlingType                     MailBoxType                    Referable Keys */
  { /*     0 */           0UL  /* /ActiveEcuC/CanIf/Can/CT_CAN00_e98c47e6 */,         0U  /* Unused, TxPduId 0 */,        0U  /* Unused, TxPduId 2 */,                                    0UL  /* /ActiveEcuC/CanIf/CanIfInitCfg/CanIfBufferCfg_0fb271d2 */, CANIF_TXBUFFER_HANDLINGTYPE_PRIOBYCANID, CANIF_TxBasicCANMailbox },  /* [/ActiveEcuC/Can/CanConfigSet/CN_CAN00_e98c47e6_Tx] */
  { /*     1 */           0UL  /* /ActiveEcuC/CanIf/Can/CT_CAN00_e98c47e6 */,         1U  /* RxPduId */          ,        3U  /* RxPduId  */         , CANIF_NO_TXBUFFERCFGIDXOFMAILBOXCONFIG  /* unusedIndex1 */                                          , CANIF_TXBUFFER_HANDLINGTYPE_NONE       , CANIF_RxBasicCANMailbox }   /* [/ActiveEcuC/Can/CanConfigSet/CN_CAN00_e98c47e6_Rx_Std] */
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanIf_MappedTxBuffersConfig
**********************************************************************************************************************/
/** 
  \var    CanIf_MappedTxBuffersConfig
  \brief  Mapped Tx-buffer(s)
  \details
  Element             Description
  MailBoxConfigIdx    the index of the 1:1 relation pointing to CanIf_MailBoxConfig
*/ 
#define CANIF_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanIf_MappedTxBuffersConfigType, CANIF_CONST) CanIf_MappedTxBuffersConfig[1] = {
    /* Index    MailBoxConfigIdx                                                                 Referable Keys */
  { /*     0 */              0UL  /* /ActiveEcuC/Can/CanConfigSet/CN_CAN00_e98c47e6_Tx */ }   /* [/ActiveEcuC/CanIf/Can/CT_CAN00_e98c47e6] */
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanIf_RxIndicationFctList
**********************************************************************************************************************/
/** 
  \var    CanIf_RxIndicationFctList
  \brief  Rx indication functions table.
  \details
  Element               Description
  RxIndicationFct       Rx indication function.
  RxIndicationLayout    Layout of Rx indication function.
*/ 
#define CANIF_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanIf_RxIndicationFctListType, CANIF_CONST) CanIf_RxIndicationFctList[4] = {
    /* Index    RxIndicationFct                                               RxIndicationLayout                                                            Referable Keys */
  { /*     0 */  { (CanIf_SimpleRxIndicationFctType)NULL_PTR }              , CanIf_SimpleRxIndicationLayout    /* PRQA S 313 */ /* MD_CanIf_11.1 */ },  /* [NULL_PTR] */
  { /*     1 */  { (CanIf_SimpleRxIndicationFctType)CanNm_RxIndication }    , CanIf_AdvancedRxIndicationLayout  /* PRQA S 313 */ /* MD_CanIf_11.1 */ },  /* [CanNm_RxIndication] */
  { /*     2 */  { (CanIf_SimpleRxIndicationFctType)CanTp_RxIndication }    , CanIf_AdvancedRxIndicationLayout  /* PRQA S 313 */ /* MD_CanIf_11.1 */ },  /* [CanTp_RxIndication] */
  { /*     3 */  { (CanIf_SimpleRxIndicationFctType)PduR_CanIfRxIndication }, CanIf_AdvancedRxIndicationLayout  /* PRQA S 313 */ /* MD_CanIf_11.1 */ }   /* [PduR_CanIfRxIndication] */
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanIf_RxPduConfig
**********************************************************************************************************************/
/** 
  \var    CanIf_RxPduConfig
  \brief  Rx-PDU configuration table.
  \details
  Element                   Description
  RxPduCanId                Rx-PDU: CAN identifier.
  RxPduMask                 Rx-PDU: CAN identifier mask.
  UpperPduId                PDU ID defined by upper layer.
  Dlc                       Data length code.
  RxIndicationFctListIdx    the index of the 1:1 relation pointing to CanIf_RxIndicationFctList
*/ 
#define CANIF_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanIf_RxPduConfigType, CANIF_CONST) CanIf_RxPduConfig[4] = {
    /* Index    RxPduCanId                                                                           RxPduMask                                                           UpperPduId                                              Dlc  RxIndicationFctListIdx                                      Comment                                                                       Referable Keys */
  { /*     0 */    0x0400U  /* Lower CanId of range: MyECU_5faa4e95_NMCanIDRange, 2.0- or FD-PDU */,   0x643FU  /* Upper CanId of range: MyECU_5faa4e95_NMCanIDRange */, CanNmConf_CanNmRxPdu_CAN00_CAN_17d0077e_RxPdu_2d241a48,  8U,                    1UL  /* CanNm_RxIndication */     },  /* [PDU: MyECU_5faa4e95_NMCanIDRange, Lower CanId of range: 0x400]   */  /* [CanIfConf_CanIfRxPduCfg_MyECU_5faa4e95_NMCanIDRange] */
  { /*     1 */    0x0614U  /* msg_diag_RequestGlobal_Tp_oCAN00_Rx_3abd61be, 2.0- or FD-PDU */     ,   0x07FFU  /* msg_diag_RequestGlobal_Tp_oCAN00_Rx_3abd61be */     , CanTpConf_CanTpRxNPdu_CanTpRxNPdu_f2ee9cdf            ,  8U,                    2UL  /* CanTp_RxIndication */     },  /* [PDU: msg_diag_RequestGlobal_Tp_oCAN00_Rx_3abd61be, CanId: 0x614] */  /* [CanIfConf_CanIfRxPduCfg_msg_diag_RequestGlobal_Tp_oCAN00_Rx_3abd61be] */
  { /*     2 */    0x0610U  /* msg_diag_Request_MyECU_Tp_oCAN00_Rx_3abd61be, 2.0- or FD-PDU */     ,   0x07FFU  /* msg_diag_Request_MyECU_Tp_oCAN00_Rx_3abd61be */     , CanTpConf_CanTpRxNPdu_CanTpRxNPdu_e72e672f            ,  8U,                    2UL  /* CanTp_RxIndication */     },  /* [PDU: msg_diag_Request_MyECU_Tp_oCAN00_Rx_3abd61be, CanId: 0x610] */  /* [CanIfConf_CanIfRxPduCfg_msg_diag_Request_MyECU_Tp_oCAN00_Rx_3abd61be] */
  { /*     3 */    0x0210U  /* msg_Receive_oCAN00_Rx_3abd61be, 2.0- or FD-PDU */                   ,   0x07FFU  /* msg_Receive_oCAN00_Rx_3abd61be */                   , PduRConf_PduRSrcPdu_SRC_Can_PT_msg_Receive_0fb271d2   ,  1U,                    3UL  /* PduR_CanIfRxIndication */ }   /* [PDU: msg_Receive_oCAN00_Rx_3abd61be, CanId: 0x210]               */  /* [CanIfConf_CanIfRxPduCfg_msg_Receive_oCAN00_Rx_3abd61be] */
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanIf_TxBufferPrioByCanIdByteQueueConfig
**********************************************************************************************************************/
/** 
  \var    CanIf_TxBufferPrioByCanIdByteQueueConfig
  \brief  Tx-buffer: PRIO_BY_CANID as BYTE_QUEUE
  \details
  Element                                             Description
  TxBufferPrioByCanIdBaseIdx                          the index of the 1:1 relation pointing to CanIf_TxBufferPrioByCanIdBase
  TxBufferPrioByCanIdByteQueueMappedTxPdusEndIdx      the end index of the 1:n relation pointing to CanIf_TxBufferPrioByCanIdByteQueueMappedTxPdus
  TxBufferPrioByCanIdByteQueueMappedTxPdusLength      the number of relations pointing to CanIf_TxBufferPrioByCanIdByteQueueMappedTxPdus
  TxBufferPrioByCanIdByteQueueMappedTxPdusStartIdx    the start index of the 1:n relation pointing to CanIf_TxBufferPrioByCanIdByteQueueMappedTxPdus
*/ 
#define CANIF_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanIf_TxBufferPrioByCanIdByteQueueConfigType, CANIF_CONST) CanIf_TxBufferPrioByCanIdByteQueueConfig[1] = {
    /* Index    TxBufferPrioByCanIdBaseIdx                                                                TxBufferPrioByCanIdByteQueueMappedTxPdusEndIdx                                                                TxBufferPrioByCanIdByteQueueMappedTxPdusLength                                                                TxBufferPrioByCanIdByteQueueMappedTxPdusStartIdx                                                                      Referable Keys */
  { /*     0 */                        0UL  /* /ActiveEcuC/CanIf/CanIfInitCfg/CanIfBufferCfg_0fb271d2 */,                                            3UL  /* /ActiveEcuC/CanIf/CanIfInitCfg/CanIfBufferCfg_0fb271d2 */,                                            3UL  /* /ActiveEcuC/CanIf/CanIfInitCfg/CanIfBufferCfg_0fb271d2 */,                                              0UL  /* /ActiveEcuC/CanIf/CanIfInitCfg/CanIfBufferCfg_0fb271d2 */ }   /* [/ActiveEcuC/CanIf/CanIfInitCfg/CanIfBufferCfg_0fb271d2] */
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanIf_TxBufferPrioByCanIdByteQueueMappedTxPdus
**********************************************************************************************************************/
/** 
  \var    CanIf_TxBufferPrioByCanIdByteQueueMappedTxPdus
  \brief  Tx-buffer: PRIO_BY_CANID as BYTE_QUEUE: Mapped Tx-PDUs
  \details
  Element           Description
  TxPduConfigIdx    the index of the 1:1 relation pointing to CanIf_TxPduConfig
*/ 
#define CANIF_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanIf_TxBufferPrioByCanIdByteQueueMappedTxPdusType, CANIF_CONST) CanIf_TxBufferPrioByCanIdByteQueueMappedTxPdus[3] = {
    /* Index    TxPduConfigIdx                                                                                            Referable Keys */
  { /*     0 */            0UL  /* /ActiveEcuC/CanIf/CanIfInitCfg/msg_diag_Response_MyECU_Tp_oCAN00_Tx_3abd61be */ },  /* [/ActiveEcuC/CanIf/CanIfInitCfg/CanIfBufferCfg_0fb271d2] */
  { /*     1 */            1UL  /* /ActiveEcuC/CanIf/CanIfInitCfg/msg_nm_MyECU_oCAN00_Tx_3abd61be */               },  /* [/ActiveEcuC/CanIf/CanIfInitCfg/CanIfBufferCfg_0fb271d2] */
  { /*     2 */            2UL  /* /ActiveEcuC/CanIf/CanIfInitCfg/msg_Transmit_oCAN00_Tx_3abd61be */               }   /* [/ActiveEcuC/CanIf/CanIfInitCfg/CanIfBufferCfg_0fb271d2] */
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanIf_TxConfirmationFctList
**********************************************************************************************************************/
/** 
  \var    CanIf_TxConfirmationFctList
  \brief  Tx confirmation functions table.
*/ 
#define CANIF_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanIf_TxConfirmationFctType, CANIF_CONST) CanIf_TxConfirmationFctList[3] = {
  /* Index     TxConfirmationFctList                      Referable Keys */
  /*     0 */ (CanIf_TxConfirmationFctType)NULL_PTR ,  /* [NULL_PTR] */
  /*     1 */ CanNm_TxConfirmation                  ,  /* [CanNm_TxConfirmation] */
  /*     2 */ CanTp_TxConfirmation                     /* [CanTp_TxConfirmation] */
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanIf_TxPduConfig
**********************************************************************************************************************/
/** 
  \var    CanIf_TxPduConfig
  \brief  Tx-PDUs - configuration.
  \details
  Element                     Description
  CanId                       CAN identifier (16bit / 32bit).
  UpperLayerTxPduId           Upper layer handle ID (8bit / 16bit).
  CtrlStatesIdx               the index of the 1:1 relation pointing to CanIf_CtrlStates
  Dlc                         Data length code.
  MailBoxConfigIdx            the index of the 1:1 relation pointing to CanIf_MailBoxConfig
  TxConfirmationFctListIdx    the index of the 1:1 relation pointing to CanIf_TxConfirmationFctList
*/ 
#define CANIF_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanIf_TxPduConfigType, CANIF_CONST) CanIf_TxPduConfig[3] = {
    /* Index    CanId    UpperLayerTxPduId                                        CtrlStatesIdx                                                 Dlc  MailBoxConfigIdx                                                           TxConfirmationFctListIdx                                    Comment                                                          Referable Keys */
  { /*     0 */ 0x0612U, CanTpConf_CanTpTxFcNPdu_CanTpTxFcNPdu_e72e672f         ,           0UL  /* /ActiveEcuC/CanIf/Can/CT_CAN00_e98c47e6 */,  8U,              0UL  /* /ActiveEcuC/Can/CanConfigSet/CN_CAN00_e98c47e6_Tx */,                      2UL  /* CanTp_TxConfirmation */ },  /* [PDU: msg_diag_Response_MyECU_Tp_oCAN00_Tx_3abd61be] */  /* [/ActiveEcuC/CanIf/CanIfInitCfg/msg_diag_Response_MyECU_Tp_oCAN00_Tx_3abd61be] */
  { /*     1 */ 0x0400U, CanNmConf_CanNmTxPdu_msg_nm_MyECU_oCAN00_TxPdu_3abd61be,           0UL  /* /ActiveEcuC/CanIf/Can/CT_CAN00_e98c47e6 */,  8U,              0UL  /* /ActiveEcuC/Can/CanConfigSet/CN_CAN00_e98c47e6_Tx */,                      1UL  /* CanNm_TxConfirmation */ },  /* [PDU: msg_nm_MyECU_oCAN00_Tx_3abd61be]               */  /* [/ActiveEcuC/CanIf/CanIfInitCfg/msg_nm_MyECU_oCAN00_Tx_3abd61be] */
  { /*     2 */ 0x0200U, PduRConf_PduRDestPdu_DST_Can_PT_msg_Transmit_0fb271d2  ,           0UL  /* /ActiveEcuC/CanIf/Can/CT_CAN00_e98c47e6 */,  1U,              0UL  /* /ActiveEcuC/Can/CanConfigSet/CN_CAN00_e98c47e6_Tx */,                      0UL  /* NULL_PTR */             }   /* [PDU: msg_Transmit_oCAN00_Tx_3abd61be]               */  /* [/ActiveEcuC/CanIf/CanIfInitCfg/msg_Transmit_oCAN00_Tx_3abd61be] */
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanIf_TxPduQueueIndex
**********************************************************************************************************************/
/** 
  \var    CanIf_TxPduQueueIndex
  \brief  Indirection table: Tx-PDU handle ID to corresponding Tx buffer handle ID. NOTE: Only BasicCAN Tx-PDUs have a valid indirection into the Tx buffer.
  \details
  Element       Description
  TxQueueIdx    the index of the 0:1 relation pointing to CanIf_TxQueue
*/ 
#define CANIF_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanIf_TxPduQueueIndexType, CANIF_CONST) CanIf_TxPduQueueIndex[3] = {
    /* Index    TxQueueIdx                                                                                            Comment                                                                                    Referable Keys */
  { /*     0 */        0UL  /* /ActiveEcuC/CanIf/CanIfInitCfg/msg_diag_Response_MyECU_Tp_oCAN00_Tx_3abd61be */ },  /* [msg_diag_Response_MyECU_Tp_oCAN00_Tx_3abd61be, BasicCAN TxPdu with Tx-buffer] */  /* [/ActiveEcuC/CanIf/CanIfInitCfg/msg_diag_Response_MyECU_Tp_oCAN00_Tx_3abd61be, /ActiveEcuC/CanIf/Can/CT_CAN00_e98c47e6] */
  { /*     1 */        1UL  /* /ActiveEcuC/CanIf/CanIfInitCfg/msg_nm_MyECU_oCAN00_Tx_3abd61be */               },  /* [msg_nm_MyECU_oCAN00_Tx_3abd61be, BasicCAN TxPdu with Tx-buffer]               */  /* [/ActiveEcuC/CanIf/CanIfInitCfg/msg_nm_MyECU_oCAN00_Tx_3abd61be, /ActiveEcuC/CanIf/Can/CT_CAN00_e98c47e6] */
  { /*     2 */        2UL  /* /ActiveEcuC/CanIf/CanIfInitCfg/msg_Transmit_oCAN00_Tx_3abd61be */               }   /* [msg_Transmit_oCAN00_Tx_3abd61be, BasicCAN TxPdu with Tx-buffer]               */  /* [/ActiveEcuC/CanIf/CanIfInitCfg/msg_Transmit_oCAN00_Tx_3abd61be, /ActiveEcuC/CanIf/Can/CT_CAN00_e98c47e6] */
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanIf_WakeUpConfig
**********************************************************************************************************************/
/** 
  \var    CanIf_WakeUpConfig
  \brief  Wake-up source configuration
  \details
  Element                Description
  Controller             CAN controller handle ID
  WakeUpSource           Wake-up source identifier
  WakeUpTargetAddress    Logical handle ID of target (CAN controller / transceiver)
  WakeUpTargetModule     Target for wake-up source: CAN controller / transceiver
*/ 
#define CANIF_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanIf_WakeUpConfigType, CANIF_CONST) CanIf_WakeUpConfig[1] = {
    /* Index    Controller  WakeUpSource  WakeUpTargetAddress  WakeUpTargetModule             Comment */
  { /*     0 */         0U,         64UL,                  0U, CANIF_WAKEUPREQUEST_CAN } 
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanIf_CtrlStates
**********************************************************************************************************************/
/** 
  \var    CanIf_CtrlStates
  \details
  Element     Description
  CtrlMode    Controller mode.
  PduMode     PDU mode state.
*/ 
#define CANIF_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(CanIf_CtrlStatesUType, CANIF_VAR_NOINIT) CanIf_CtrlStates;  /* PRQA S 0759 */  /* MD_CSL_18.4 */
  /* Index        Referable Keys  */
  /*     0 */  /* [/ActiveEcuC/CanIf/Can/CT_CAN00_e98c47e6] */

#define CANIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanIf_TxBufferPrioByCanIdBase
**********************************************************************************************************************/
/** 
  \var    CanIf_TxBufferPrioByCanIdBase
  \brief  Variable declaration - Tx-buffer: PRIO_BY_CANID as byte/bit-queue. Stores at least the QueueCounter.
*/ 
#define CANIF_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(CanIf_TxBufferPrioByCanIdBaseUType, CANIF_VAR_NOINIT) CanIf_TxBufferPrioByCanIdBase;  /* PRQA S 0759 */  /* MD_CSL_18.4 */
  /* Index        Referable Keys  */
  /*     0 */  /* [/ActiveEcuC/CanIf/CanIfInitCfg/CanIfBufferCfg_0fb271d2] */

#define CANIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanIf_TxQueue
**********************************************************************************************************************/
/** 
  \var    CanIf_TxQueue
  \brief  Variable declaration - Tx byte queue.
*/ 
#define CANIF_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
VAR(CanIf_TxQueueUType, CANIF_VAR_NOINIT) CanIf_TxQueue;  /* PRQA S 0759 */  /* MD_CSL_18.4 */
  /* Index        Referable Keys  */
  /*     0 */  /* [/ActiveEcuC/CanIf/CanIfInitCfg/msg_diag_Response_MyECU_Tp_oCAN00_Tx_3abd61be] */
  /*     1 */  /* [/ActiveEcuC/CanIf/CanIfInitCfg/msg_nm_MyECU_oCAN00_Tx_3abd61be] */
  /*     2 */  /* [/ActiveEcuC/CanIf/CanIfInitCfg/msg_Transmit_oCAN00_Tx_3abd61be] */

#define CANIF_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */


/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: GLOBAL DATA
**********************************************************************************************************************/



