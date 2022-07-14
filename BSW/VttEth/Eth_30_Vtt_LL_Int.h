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
 *         File:  Eth_30_Vtt_LL_Int.h
 *      Project:  TcpIp-Stack
 *       Module:  Ethernet Driver for VTT
 *    Generator:  DrvEth_Spc58xxEthAsr.jar
 *
 *  Description:  internal header
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
#ifndef ETH_30_VTT_LL_INT_H
# define ETH_30_VTT_LL_INT_H
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* only one controller is supported */
#define ETH_30_VTT_MAX_CFGS_TOTAL                          1U
/* Only one descriptor ring supported */          
#define ETH_30_VTT_RING_IDX                                0U
#define ETH_30_VTT_MAX_MII_REG_TOTAL                      32U

/* ETH Frame offsets (including offset) */
#define ETH_30_VTT_RX_RSV4BUFIDX_OFFSET_U8                    0U
#define ETH_30_VTT_RX_RSV4STATUS_OFFSET_U8                    1U
#define ETH_30_VTT_RX_DST_OFFSET_U8                           2U
#define ETH_30_VTT_RX_DST_OFFSET_U16                          1U
#define ETH_30_VTT_RX_SRC_OFFSET_U8                           8U
#define ETH_30_VTT_RX_SRC_OFFSET_U16                          4U
#define ETH_30_VTT_RX_SRC_OFFSET_U32                          2U
#define ETH_30_VTT_RX_TYPE_OFFSET_U8                         14U
#define ETH_30_VTT_RX_TYPE_OFFSET_U16                         7U
#define ETH_30_VTT_RX_DATA_OFFSET_U8                         16U
#define ETH_30_VTT_RX_DATA_OFFSET_U16                         8U
#define ETH_30_VTT_RX_DATA_OFFSET_U32                         4U
#define ETH_30_VTT_TX_DST_OFFSET_U8                           0U
#define ETH_30_VTT_TX_DST_OFFSET_U16                          0U
#define ETH_30_VTT_TX_SRC_OFFSET_U8                           6U
#define ETH_30_VTT_TX_SRC_OFFSET_U16                          3U
#define ETH_30_VTT_TX_TYPE_OFFSET_U8                         12U
#define ETH_30_VTT_TX_TYPE_OFFSET_U16                         6U
#define ETH_30_VTT_TX_DATA_OFFSET_U8                         14U
#define ETH_30_VTT_TX_DATA_OFFSET_U16                         7U

#define ETH_30_VTT_BUF_ALIGN_LEN_BYTE                         2U
#define ETH_30_VTT_HDR_LEN_BYTE                              14U

/* Buffer status */
# define ETH_30_VTT_BUFFER_NOT_BUSY                                            (0U)
# define ETH_30_VTT_BUFFER_BUSY                                                (1U)

/* Descriptor status */
#define ETH_30_VTT_DESCRIPTOR_NOT_READY                                        (0U)
#define ETH_30_VTT_DESCRIPTOR_READY                                            (1U)
#define ETH_30_VTT_DESCRIPTOR_TRANSMITTED                                      (2U)

/* Tx Confirmation */
# define ETH_30_VTT_TX_STATE_CONFIRMATION_NOT_PENDING                          (0U)
# define ETH_30_VTT_TX_STATE_CONFIRMATION_PENDING                              (1U)

#define ETH_30_VTT_CONFIRMATION_NOT_CALLED                 0x01U
#define ETH_30_VTT_CONFIRMATION_CALLED                     0x00U
/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453,3458 FCT_LIKE_MACROS *//* MISRA 19.4,19.7: Macros improve code readability */

/* Development Error Tracer */
#if ( ETH_30_VTT_DEV_ERROR_REPORT == STD_ON )
/* PRQA S 3453 4 */ /* MD_MSR_19.7 */
#  define Eth_30_Vtt_CheckDetErrorReturnVoid(  CONDITION, INSTANCE_ID, API_ID, ERROR_CODE )          if(!(CONDITION)) { (void)Det_ReportError( ETH_MODULE_ID, (INSTANCE_ID), (API_ID), (ERROR_CODE)); return; }
#  define Eth_30_Vtt_CheckDetErrorReturnValue( CONDITION, INSTANCE_ID, API_ID, ERROR_CODE, RET_VAL)  if(!(CONDITION)) { (void)Det_ReportError( ETH_MODULE_ID, (INSTANCE_ID), (API_ID), (ERROR_CODE)); return (RET_VAL); }
#  define Eth_30_Vtt_CheckDetErrorContinue(    CONDITION, INSTANCE_ID, API_ID, ERROR_CODE )          if(!(CONDITION)) { (void)Det_ReportError( ETH_MODULE_ID, (INSTANCE_ID), (API_ID), (ERROR_CODE)); }
#  define Eth_30_Vtt_CallDetReportError( INSTANCE_ID, API_ID, ERROR_CODE )                           (void)Det_ReportError( ETH_MODULE_ID, (INSTANCE_ID), (API_ID), (ERROR_CODE))
#else
/* PRQA S 3453 4 */ /* MD_MSR_19.7 */
#  define Eth_30_Vtt_CheckDetErrorReturnVoid(  CONDITION, INSTANCE_ID, API_ID, ERROR_CODE )
#  define Eth_30_Vtt_CheckDetErrorReturnValue( CONDITION, INSTANCE_ID, API_ID, ERROR_CODE, RET_VAL )
#  define Eth_30_Vtt_CheckDetErrorContinue(    CONDITION, INSTANCE_ID, API_ID, ERROR_CODE )
#  define Eth_30_Vtt_CallDetReportError( INSTANCE_ID, API_ID, ERROR_CODE )
#endif /* ETH_30_VTT_DEV_ERROR_DETECT */

# define Eth_30_Vtt_GetTxBufferOffset(CtrlIdx)                         ((0 < CtrlIdx) ? Eth_30_Vtt_VCfgGetTxBufferNum(CtrlIdx-1) : 0)
# define Eth_30_Vtt_VCfgGetTxConfirmationCalled(CtrlIdx, BufIdx)       (Eth_30_Vtt_TxConfirmationCalled[Eth_30_Vtt_GetTxBufferOffset(CtrlIdx) + BufIdx])
# define Eth_30_Vtt_VCfgGetRxBuf4Ring(CtrlIdx, RingIdx)                (Eth_30_Vtt_Ctrl2RxRingMap[CtrlIdx][RingIdx])

# endif /* ETH_30_VTT_LL_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: Eth_30_Vtt_LL_Int.h
 *********************************************************************************************************************/
