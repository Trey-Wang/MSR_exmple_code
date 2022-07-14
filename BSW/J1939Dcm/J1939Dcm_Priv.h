/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  J1939Dcm_Priv.h
 *    Component:  MICROSAR Diagnostic Communication Manager for SAE J1939
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  module internal definitions and macros for J1939 DCM
 *
 *********************************************************************************************************************/

#if !defined (J1939DCM_PRIV_H)
#define J1939DCM_PRIV_H

/* inclusion only allowed from J1939Dcm.c */
#if defined (J1939DCM_SOURCE)

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CONSTANT MACROS
 *********************************************************************************************************************/
/* --- message processing states ------------------------------------------------------------------------------------*/
#define J1939DCM_MSG_STATE_IDLE          0x00u       /* no message is currently processed                            */
#define J1939DCM_MSG_STATE_WAIT          0x01u       /* wait for next periodic message transmission                  */
#define J1939DCM_MSG_STATE_PREP_START    0x10u       /* message preparation start                                    */
#define J1939DCM_MSG_STATE_PREP_NUMBER   0x11u       /* message preparation in progress: get number of DTCs          */
#define J1939DCM_MSG_STATE_PREP_DTCS     0x12u       /* message preparation in progress: get DTCs                    */
#define J1939DCM_MSG_STATE_PREP_LAMPDTC  0x13u       /* message preparation in progress: get DTCs with lamp status   */
#define J1939DCM_MSG_STATE_PREP_FF       0x14u       /* message preparation in progress: get FreezeFrame             */
#define J1939DCM_MSG_STATE_CLEAR         0x15u       /* message preparation in progress: clear DTCs                  */
#define J1939DCM_MSG_STATE_TX            0x20u       /* trigger transmission of messages                             */
#define J1939DCM_MSG_STATE_TXCONF        0x21u       /* wait for TxConfirmation                                      */
#define J1939DCM_MSG_STATE_SENDACK       0x22u       /* send ACK / NACK                                              */

#define J1939DCM_QUEUE_EMPTY             0x00u       /* nothing have been queued                                     */
#define J1939DCM_QUEUE_STATE             0x01u       /* state change has been queued                                 */
#define J1939DCM_QUEUE_REQ               0x02u       /* request for DM transmission has been queued                  */
#define J1939DCM_QUEUE_DTC               0x04u       /* DTC status change has been queued (DM1 only)                 */
#define J1939DCM_QUEUE_TXCONF            0x08u       /* TxConfirmation detected; DM transmission is finished         */
#define J1939DCM_QUEUE_TXFAIL            0x10u       /* TxConfirmation detected, but transmission failed             */

#define J1939DCM_METADATA_SA_OFFSET          0       /* meta-data offset for source address                          */
#define J1939DCM_METADATA_DA_OFFSET          1       /* meta-data offset for destination address                     */
#define J1939DCM_METADATA_PDUF_OFFSET        2       /* meta-data offset for PDU-format                              */
#define J1939DCM_METADATA_PRIO_OFFSET        3       /* meta-data offset for priority                                */

#define J1939DCM_INVALID_NODE_HDL        0xFFu
#define J1939DCM_INVALID_CHANNEL_HDL     0xFFu
#define J1939DCM_INVALID_MESSAGE_HDL     0xFFu
#define J1939DCM_INVALID_SPN             0xFFFFFFFFUL
#define J1939DCM_INVALID_MEM_ADDRESS     0xFFu

#define J1939DCM_ADDRESS_BROADCAST       0xFFu


/* --- helper switches ----------------------------------------------------------------------------------------------*/
#if (J1939DCM_MAX_NUM_NODES > 1)
# define J1939DCM_SINGLE_NODE_OPT         STD_OFF
#else
# define J1939DCM_SINGLE_NODE_OPT         STD_ON
#endif

#if (J1939DCM_MAX_NUM_CHANNELS > 1)
# define J1939DCM_SINGLE_CHANNEL_OPT      STD_OFF
#else
# define J1939DCM_SINGLE_CHANNEL_OPT      STD_ON
#endif

/* support for any DM which needs SPNs / DTCs */
#if ( (J1939DCM_DM1_SUPPORT  == STD_ON) || \
      (J1939DCM_DM2_SUPPORT  == STD_ON) || \
      (J1939DCM_DM12_SUPPORT == STD_ON) || \
      (J1939DCM_DM23_SUPPORT == STD_ON) || \
      (J1939DCM_DM27_SUPPORT == STD_ON) || \
      (J1939DCM_DM28_SUPPORT == STD_ON) || \
      (J1939DCM_DM35_SUPPORT == STD_ON) || \
      (J1939DCM_DM53_SUPPORT == STD_ON) || \
      (J1939DCM_DM54_SUPPORT == STD_ON))
# define J1939DCM_NEED_DTC_HANDLER_SPN  STD_ON
#else
# define J1939DCM_NEED_DTC_HANDLER_SPN  STD_OFF
#endif

/* support for any DM which needs number of DTCs */
#if (J1939DCM_DM29_SUPPORT  == STD_ON)
# define J1939DCM_NEED_DTC_HANDLER_NUM  STD_ON
#else
# define J1939DCM_NEED_DTC_HANDLER_NUM  STD_OFF
#endif

/* support for any DM which needs Freeze Frames / DTCs */
#if ( (J1939DCM_DM4_SUPPORT  == STD_ON) || \
      (J1939DCM_DM24_SUPPORT == STD_ON) || \
      (J1939DCM_DM25_SUPPORT == STD_ON) )
# define J1939DCM_NEED_DTC_HANDLER_FF  STD_ON
#else
# define J1939DCM_NEED_DTC_HANDLER_FF  STD_OFF
#endif

/* support for any DM which needs Lamps / DTCs */
#if (J1939DCM_DM31_SUPPORT == STD_ON)
# define J1939DCM_NEED_DTC_HANDLER_LAMP  STD_ON
#else
# define J1939DCM_NEED_DTC_HANDLER_LAMP  STD_OFF
#endif

/* support for any DM which needs OBD */
#if ( (J1939DCM_DM5_SUPPORT  == STD_ON) || \
      (J1939DCM_DM20_SUPPORT == STD_ON) || \
      (J1939DCM_DM21_SUPPORT == STD_ON) || \
      (J1939DCM_DM26_SUPPORT == STD_ON) )
# define J1939DCM_NEED_DTC_HANDLER_OBD  STD_ON
#else
# define J1939DCM_NEED_DTC_HANDLER_OBD  STD_OFF
#endif


/* support for any DM which needs the DTC handler */
#if ( (J1939DCM_NEED_DTC_HANDLER_SPN  == STD_ON) || \
      (J1939DCM_NEED_DTC_HANDLER_NUM  == STD_ON) || \
      (J1939DCM_NEED_DTC_HANDLER_FF   == STD_ON) || \
      (J1939DCM_NEED_DTC_HANDLER_LAMP == STD_ON) || \
      (J1939DCM_NEED_DTC_HANDLER_OBD  == STD_ON) )
# define J1939DCM_NEED_DTC_HANDLER    STD_ON
#else
# define J1939DCM_NEED_DTC_HANDLER    STD_OFF
#endif


/* support for any DM which needs the clear handler */
#if ( (J1939DCM_DM3_SUPPORT  == STD_ON) || \
      (J1939DCM_DM11_SUPPORT == STD_ON) || \
      (J1939DCM_DM55_SUPPORT == STD_ON) )
# define J1939DCM_NEED_CLEAR_HANDLER    STD_ON
#else
# define J1939DCM_NEED_CLEAR_HANDLER    STD_OFF
#endif

/* support for any DM which needs is handled by the DMx processor */
#if ( (J1939DCM_DM2_SUPPORT  == STD_ON) || \
      (J1939DCM_DM3_SUPPORT  == STD_ON) || \
      (J1939DCM_DM4_SUPPORT  == STD_ON) || \
      (J1939DCM_DM5_SUPPORT  == STD_ON) || \
      (J1939DCM_DM11_SUPPORT == STD_ON) || \
      (J1939DCM_DM12_SUPPORT == STD_ON) || \
      (J1939DCM_DM19_SUPPORT == STD_ON) || \
      (J1939DCM_DM20_SUPPORT == STD_ON) || \
      (J1939DCM_DM21_SUPPORT == STD_ON) || \
      (J1939DCM_DM23_SUPPORT == STD_ON) || \
      (J1939DCM_DM24_SUPPORT == STD_ON) || \
      (J1939DCM_DM25_SUPPORT == STD_ON) || \
      (J1939DCM_DM26_SUPPORT == STD_ON) || \
      (J1939DCM_DM27_SUPPORT == STD_ON) || \
      (J1939DCM_DM28_SUPPORT == STD_ON) || \
      (J1939DCM_DM29_SUPPORT == STD_ON) || \
      (J1939DCM_DM31_SUPPORT == STD_ON) || \
      (J1939DCM_DM53_SUPPORT == STD_ON) || \
      (J1939DCM_DM54_SUPPORT == STD_ON) || \
      (J1939DCM_DM55_SUPPORT == STD_ON))
# define J1939DCM_NEED_DMX_PROCESSOR    STD_ON
#else
# define J1939DCM_NEED_DMX_PROCESSOR    STD_OFF
#endif

/* DM1 support needs separate processor */
#if ( J1939DCM_DM1_SUPPORT  == STD_ON )
# define J1939DCM_NEED_DM1_PROCESSOR    STD_ON
#else
# define J1939DCM_NEED_DM1_PROCESSOR    STD_OFF
#endif

/* DM13 support needs separate processor */
#if ( J1939DCM_DM13_SUPPORT  == STD_ON )
# define J1939DCM_NEED_DM13_PROCESSOR    STD_ON
#else
# define J1939DCM_NEED_DM13_PROCESSOR    STD_OFF
#endif

/* DM35 support needs separate processor */
#if ( J1939DCM_DM35_SUPPORT  == STD_ON )
# define J1939DCM_NEED_DM35_PROCESSOR    STD_ON
#else
# define J1939DCM_NEED_DM35_PROCESSOR    STD_OFF
#endif


/* DMs which are received via IF API */
#if ( (J1939DCM_DM13_SUPPORT  == STD_ON) || \
      (J1939DCM_DM14_SUPPORT == STD_ON) || \
      (J1939DCM_DM17_SUPPORT == STD_ON) )
# define J1939DCM_NEED_IF_RECEPTION    STD_ON
#else
# define J1939DCM_NEED_IF_RECEPTION    STD_OFF
#endif

/* DMs which are received via TP APIs */
#if ( (J1939DCM_DM16_SUPPORT  == STD_ON) || \
      (J1939DCM_DM18_SUPPORT == STD_ON) )
# define J1939DCM_NEED_TP_RECEPTION    STD_ON
#else
# define J1939DCM_NEED_TP_RECEPTION    STD_OFF
#endif

/* DMs which are transmitted via IF API */
#if ( (J1939DCM_DM5_SUPPORT  == STD_ON) || \
      (J1939DCM_DM15_SUPPORT == STD_ON) || \
      (J1939DCM_DM21_SUPPORT == STD_ON) || \
      (J1939DCM_DM26_SUPPORT == STD_ON) )
# define J1939DCM_NEED_IF_TRANSMISSION    STD_ON
#else
# define J1939DCM_NEED_IF_TRANSMISSION    STD_OFF
#endif

/* DMs which are transmitted via TP APIs */
#if ( (J1939DCM_DM1_SUPPORT  == STD_ON) || \
      (J1939DCM_DM2_SUPPORT  == STD_ON) || \
      (J1939DCM_DM4_SUPPORT  == STD_ON) || \
      (J1939DCM_DM16_SUPPORT == STD_ON) || \
      (J1939DCM_DM18_SUPPORT == STD_ON) || \
      (J1939DCM_DM12_SUPPORT == STD_ON) || \
      (J1939DCM_DM19_SUPPORT == STD_ON) || \
      (J1939DCM_DM20_SUPPORT == STD_ON) || \
      (J1939DCM_DM23_SUPPORT == STD_ON) || \
      (J1939DCM_DM24_SUPPORT == STD_ON) || \
      (J1939DCM_DM25_SUPPORT == STD_ON) || \
      (J1939DCM_DM27_SUPPORT == STD_ON) || \
      (J1939DCM_DM28_SUPPORT == STD_ON) || \
      (J1939DCM_DM29_SUPPORT == STD_ON) || \
      (J1939DCM_DM31_SUPPORT == STD_ON) || \
      (J1939DCM_DM35_SUPPORT == STD_ON) || \
      (J1939DCM_DM53_SUPPORT == STD_ON) || \
      (J1939DCM_DM54_SUPPORT == STD_ON))
# define J1939DCM_NEED_TP_TRANSMISSION    STD_ON
#else
# define J1939DCM_NEED_TP_TRANSMISSION    STD_OFF
#endif

/* DMs which are used for memory access */
#if ( (J1939DCM_DM14_SUPPORT  == STD_ON) || \
      (J1939DCM_DM15_SUPPORT == STD_ON) || \
      (J1939DCM_DM16_SUPPORT == STD_ON) || \
      (J1939DCM_DM17_SUPPORT == STD_ON) || \
      (J1939DCM_DM18_SUPPORT == STD_ON) )
# define J1939DCM_NEED_MEM_ACCESS      STD_ON
#else
# define J1939DCM_NEED_MEM_ACCESS      STD_OFF
#endif


/* --- DM1 constants ------------------------------------------------------------------------------------------------*/
#define J1939DCM_DM1_TIMEOUT              J1939Dcm_UtiMSecToCycles(1000)  /* default DM1 timeout (1sec)              */
#define J1939DCM_DM1_MAX_DTCS             20                              /* SWS_J1939Dcm_00116                      */

/* --- DM13 constants -----------------------------------------------------------------------------------------------*/
#define J1939DCM_DM13_TIMEOUT             J1939Dcm_UtiMSecToCycles(6000)  /* default DM13 timeout (6sec)             */
#define J1939DCM_DM13_BROADCAST_STOP      0x00u                           /* request to stop broadcast               */
#define J1939DCM_DM13_BROADCAST_START     0x01u                           /* request to start broadcast              */
#define J1939DCM_DM13_BROADCAST_RES       0x02u                           /* reserved                                */
#define J1939DCM_DM13_BROADCAST_DC        0x03u                           /* don't care 7 no action                  */
#define J1939DCM_DM13_HOLD_ALL            0x00u                           /* hold signal set for all devices         */
#define J1939DCM_DM13_HOLD_NA             0xF0u                           /* hold signal not available               */

/* --- DM35 constants -----------------------------------------------------------------------------------------------*/
#define J1939DCM_DM35_TIMEOUT             J1939Dcm_UtiMSecToCycles(1000) /* default DM35 timeout (1sec)              */


/* --- DM DTC constants ---------------------------------------------------------------------------------------------*/
#define J1939DCM_DM_MAX_DTCS            0xFFFFu       /* number of DTCs for non-DM1 message only limited by 16Bit    */
#define J1939DCM_DM_PRIORITY_DEFAULT          6       /* default priority for most diagnostic messages               */

/* --- DM FF constants ----------------------------------------------------------------------------------------------*/
#define J1939DCM_FF_MAX_DATA_LENGTH         251       /* max length of one freeze frame (data added by DEM)          */
#define J1939DCM_FF_SPN_OCC_LENGTH            4       /* length of SPN + occurrence counter (added by J1939Dcm)      */
#define J1939DCM_FF_HEADER_LENGTH             5       /* length of LEN + SPN + occurrence counter (added by J1939Dcm)*/
#define J1939DCM_FF_LENGTH_OFFSET             0       /* offset of length byte within a reported freeze frame        */
#define J1939DCM_FF_DTC_OFFSET                1       /* offset of 1st DTC byte within a reported freeze frame       */
#define J1939DCM_FF_OCC_OFFSET                4       /* offset of occurrence counter within a reported freeze frame */
#define J1939DCM_FF_DATA_OFFSET               5       /* offset of 1st data byte within a reported freeze frame      */
/*          +---+---+---+---+---+---+---+- ~ -++---+--
 *          |LEN|   S P N   |Occ| Data .....  ||LEN| ...
 *          +---+---+---+---+---+---+---+- ~ -++---+--
 * Offset:    0   1   2   3   4   5   ..         n
 */

/* --- Memory Access constants --------------------------------------------------------------------------------------*/
#define J1939DCM_MEM_ACCESS_MSG_LENGTH       8        /* default length for DM14 and DM15                            */



/**********************************************************************************************************************
 *  FUNCTION MACROS
 *********************************************************************************************************************/
/* --- optimization -------------------------------------------------------------------------------------------------*/
#if (J1939DCM_SINGLE_NODE_OPT != STD_ON)
# define J1939Dcm_ForEachNode_Start                        lNodeHdl = J1939Dcm_Cfg_GetNumberOfNodes(); do              /* PRQA S 3412 */ /* MD_MSR_19.4 */
# define J1939Dcm_ForEachNode_Next                         lNodeHdl--
# define J1939Dcm_ForEachNode_End                          while (lNodeHdl > 0);                                       /* PRQA S 3412 */ /* MD_MSR_19.4 */
# define J1939Dcm_Cfg_GetNodeHdlById(NodeId)               J1939Dcm_GetNodeHdlById(NodeId)                             /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define J1939Dcm_Cfg_GetNodeHdlByAddr(NodeAddr)           J1939Dcm_GetNodeHdlByAddr(NodeAddr)                         /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
# define J1939Dcm_ForEachNode_Start
# define J1939Dcm_ForEachNode_Next                         lNodeHdl = 0
# define J1939Dcm_ForEachNode_End
# define J1939Dcm_Cfg_GetNodeHdlById(NodeId)               (J1939Dcm_NodeHdlType)( ((NodeId) == J1939Dcm_Cfg_GetNodeId(0))       ? 0 : J1939DCM_INVALID_NODE_HDL )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define J1939Dcm_Cfg_GetNodeHdlByAddr(NodeAddr)           (J1939Dcm_NodeHdlType)( ((NodeAddr) == J1939Dcm_Cfg_GetNodeAddress(0))? 0 : J1939DCM_INVALID_NODE_HDL )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

#if (J1939DCM_SINGLE_CHANNEL_OPT != STD_ON)
# define J1939Dcm_ForEachChannel_Start                     lChannelHdl = J1939Dcm_Cfg_GetNumberOfChannels(); do        /* PRQA S 3412 */ /* MD_MSR_19.4 */
# define J1939Dcm_ForEachChannel_Next                      lChannelHdl--
# define J1939Dcm_ForEachChannel_End                       while (lChannelHdl > 0);                                    /* PRQA S 3412 */ /* MD_MSR_19.4 */
# define J1939Dcm_Cfg_GetChannelHdlById(ChannelId)         J1939Dcm_GetChannelHdlById(ChannelId)                       /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
# define J1939Dcm_ForEachChannel_Start
# define J1939Dcm_ForEachChannel_Next                      lChannelHdl = 0
# define J1939Dcm_ForEachChannel_End
# define J1939Dcm_Cfg_GetChannelHdlById(ChannelId)         (J1939Dcm_ChannelHdlType)( ((ChannelId) == J1939Dcm_Cfg_GetChannelId(0)) ? 0 : J1939DCM_INVALID_CHANNEL_HDL )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif


/* --- helper macros ------------------------------------------------------------------------------------------------*/
#define J1939Dcm_UtiSduDataTo32Bit(dataPtr)    (uint32)( (uint32)((uint32)((dataPtr)[0]) << 24) \
                                                       | (uint32)((uint32)((dataPtr)[1]) << 16) \
                                                       | (uint32)((uint32)((dataPtr)[2]) << 8) \
                                                       | (uint32)((uint32)((dataPtr)[3])) )                            /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_Uti24BitGetHiByte(value)      (uint8)(((uint32)(value) >> 16) & 0x000000FFUL)                         /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_Uti24BitGetMidByte(value)     (uint8)(((uint32)(value) >> 8)  & 0x000000FFUL)                         /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_Uti24BitGetLoByte(value)      (uint8)( (uint32)(value)        & 0x000000FFUL)                         /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define J1939Dcm_Uti16BitGetHiByte(value)      (uint8)(((uint16)(value) >> 8) & 0x00FFu)                               /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_Uti16BitGetLoByte(value)      (uint8)( (uint16)(value)       & 0x00FFu)                               /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define J1939Dcm_UtiMSecToCycles(msec)         ((uint16)((msec)/J1939DCM_TASK_CYCLE))                                  /* PRQA S 3453 */ /* MD_MSR_19.7 */


#if (J1939DCM_USE_DUMMY_STATEMENT == STD_ON)
# define J1939Dcm_IgnoreUnusedArgument(argument)          J1939DCM_DUMMY_STATEMENT(argument);                          /* PRQA S 3412, 3453 */ /* MD_MSR_19.4, MD_MSR_19.7 */
# define J1939Dcm_IgnoreUnusedArgument_CONST(argument)    J1939DCM_DUMMY_STATEMENT_CONST(argument);                    /* PRQA S 3412, 3453 */ /* MD_MSR_19.4, MD_MSR_19.7 */
#else
# define J1939Dcm_IgnoreUnusedArgument(argument)                                                                       /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define J1939Dcm_IgnoreUnusedArgument_CONST(argument)                                                                 /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

#define J1939Dcm_DemSemaphoreGetLocked()          J1939Dcm_DemAccessSemaphore              /* FALSE: access to DEM is possible; TRUE: DEM in use */
#define J1939Dcm_DemSemaphoreSetLocked(status)    J1939Dcm_DemAccessSemaphore = (status)                               /* PRQA S 3453 */ /* MD_MSR_19.7 */


/* --- ComState Access macros --------------------------------------------------------------------------------------*/
#define J1939Dcm_ComStateGetChannelMask(Channel)      ((J1939Dcm_ChannelSupportType)(0x1 << (Channel)))                                      /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_ComStateIsOnline(Node, Channel)      (0 != (J1939Dcm_ComState[(Node)] & J1939Dcm_ComStateGetChannelMask(Channel)))          /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_ComStateSetOnline(Node, Channel)     (J1939Dcm_ComState[(Node)] |= J1939Dcm_ComStateGetChannelMask(Channel))                /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_ComStateSetOffline(Node, Channel)    (J1939Dcm_ComState[(Node)] &= (J1939Dcm_ChannelSupportType)~J1939Dcm_ComStateGetChannelMask(Channel))        /* PRQA S 3453 */ /* MD_MSR_19.7 */


/* --- DM queue macros ---------------------------------------------------------------------------------------------*/
#define J1939Dcm_QueueGetStateChange(Queue)           ((Queue).RequestPending & J1939DCM_QUEUE_STATE)                  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_QueueGetDTCChange(Queue)             ((Queue).RequestPending & J1939DCM_QUEUE_DTC)                    /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_QueueGetMsgRequest(Queue)            ((Queue).RequestPending & J1939DCM_QUEUE_REQ)                    /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_QueueGetTxConf(Queue)                ((Queue).RequestPending & J1939DCM_QUEUE_TXCONF)                 /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_QueueGetTxFail(Queue)                ((Queue).RequestPending & J1939DCM_QUEUE_TXFAIL)                 /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define J1939Dcm_QueueSetStateChange(Queue)           ((Queue).RequestPending |= J1939DCM_QUEUE_STATE)                 /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_QueueSetDTCChange(Queue)             ((Queue).RequestPending |= J1939DCM_QUEUE_DTC)                   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_QueueSetMsgRequest(Queue)            ((Queue).RequestPending |= J1939DCM_QUEUE_REQ)                   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_QueueSetTxConf(Queue)                ((Queue).RequestPending |= J1939DCM_QUEUE_TXCONF)                /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_QueueSetTxFail(Queue)                ((Queue).RequestPending |= J1939DCM_QUEUE_TXFAIL)                /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define J1939Dcm_QueueClearStateChange(Queue)         ((Queue).RequestPending &= (uint8)(~J1939DCM_QUEUE_STATE))       /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_QueueClearDTCChange(Queue)           ((Queue).RequestPending &= (uint8)(~J1939DCM_QUEUE_DTC))         /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_QueueClearMsgRequest(Queue)          ((Queue).RequestPending &= (uint8)(~J1939DCM_QUEUE_REQ))         /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_QueueClearTxConf(Queue)              ((Queue).RequestPending &= (uint8)(~J1939DCM_QUEUE_TXCONF))      /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_QueueClearAll(Queue)                 ((Queue).RequestPending = J1939DCM_QUEUE_EMPTY)                  /* PRQA S 3453 */ /* MD_MSR_19.7 */


/* --- DM specific macros ------------------------------------------------------------------------------------------*/
#define J1939Dcm_DM13DataGetBroadcastStatus(Dm13Data, NetworkType)   (uint8)((uint32)((Dm13Data) & (uint32)(0xC0000000UL >> (2*(NetworkType)))) >> (30 - (2*(NetworkType))))               /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_DM13DataGetHoldSignal(Dm13Data)                     (uint8)((Dm13Data) & 0x000000F0UL)                                                                            /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_DM13BroadcastStatusMask(ChannelHdl)                 (uint16)((uint16)0x1u << J1939Dcm_Cfg_GetChannelId((ChannelHdl)))                                             /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_DM13SetBroadcastStatusActive(ChannelHdl)            (J1939Dcm_DM13State.BroadcastStatusInformation |= J1939Dcm_DM13BroadcastStatusMask((ChannelHdl)))             /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_DM13ClearBroadcastStatusActive(ChannelHdl)          (J1939Dcm_DM13State.BroadcastStatusInformation &= (uint16)(~J1939Dcm_DM13BroadcastStatusMask((ChannelHdl))))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_DM13GetBroadcastStatusActive(ChannelHdl)            ( 0 != (J1939Dcm_DM13State.BroadcastStatusInformation & J1939Dcm_DM13BroadcastStatusMask((ChannelHdl))))      /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define J1939Dcm_DM14GetCommand(MsgDataPtr)                (J1939Dcm_MemCommandType)(((MsgDataPtr)[1] & 0x0Eu) >> 1)                                                               /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_DM14GetKey(MsgDataPtr)                    (uint16)(((uint16)(MsgDataPtr)[7] << 8) | (uint16)(MsgDataPtr)[6])                                                      /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_DM14GetLength(MsgDataPtr)                 (uint16)((((uint16)((MsgDataPtr)[1] & 0xE0u)) << 3) | (uint16)(MsgDataPtr)[0])                                          /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_DM14GetPointer(MsgDataPtr)                (uint32)(((uint32)(MsgDataPtr)[4] << 16) | ((uint32)(MsgDataPtr)[3] << 8) | (uint32)(MsgDataPtr)[2])                    /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_DM14GetPointerExt(MsgDataPtr)             (MsgDataPtr)[5]                                                                                                         /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_DM14GetPointerType(MsgDataPtr)            (J1939Dcm_MemPointerType)(((MsgDataPtr)[1] & 0x10u) >> 4)                                                               /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define J1939Dcm_DM24GetLength(NodeHdl)                    ((PduLengthType)(J1939Dcm_GetCfg_SPNArrayEndIdxOfCfg_NodeTable((NodeHdl)) - J1939Dcm_GetCfg_SPNArrayStartIdxOfCfg_NodeTable((NodeHdl)))) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define J1939Dcm_DM35SetChannelActive(ChannelMask, ChannelHdl) ((ChannelMask) |= J1939Dcm_ComStateGetChannelMask(ChannelHdl))                    /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_DM35GetChannelActive(ChannelMask, ChannelHdl) ((ChannelMask) &  J1939Dcm_ComStateGetChannelMask(ChannelHdl))                    /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_DM35ChannelIsActive(ChannelMask, ChannelHdl)  (0 != J1939Dcm_DM35GetChannelActive((ChannelMask), (ChannelHdl)))                 /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define J1939Dcm_Data_ConfigPtr()                              J1939Dcm_ConfigDataPtr


/**********************************************************************************************************************
 *  DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* --- General status structures ------------------------------------------------------------------------------------*/
typedef struct
{
  P2VAR(PduInfoType, AUTOMATIC, J1939DCM_INTERNAL_DATA) Buffer;  /* message to be transmitted                            */
  PduLengthType        BufferIndex[J1939DCM_MAX_NUM_CHANNELS];   /* position within buffer for each channel              */
  uint16               TxTimer;                                  /* timeout counter                                      */
  uint8                MetaData[J1939DCM_METADATA_LEN];          /* meta data bytes                                      */
  uint8                DM;                                       /* message to be transmitted                            */
  uint8                UnconfirmedChannels;                      /* number of channels waiting for TxConf                */
  J1939Dcm_TxPathType  TransmitPath;                             /* IF or TP transmission                                */
  J1939Dcm_NodeHdlType NodeHdl;                                  /* node which requested the transmission                */
} J1939Dcm_TxHandlerType;

typedef struct
{
  J1939Dcm_AddressInfoType RxAddressInfo;   /* addressing info of currently active Rx connection */
  PduIdType                RxPduId;         /* RxPdu which is currently processed                */
} J1939Dcm_RxHandlerType;

typedef struct
{
  P2VAR(PduInfoType, AUTOMATIC, J1939DCM_INTERNAL_DATA) MsgData; /* buffer and size of DM to be transmitted              */
  uint16      NumDTCs;                                       /* overall number of DTCs already retrieved             */
  uint8       NumDTCsInCycle;                                /* number of DTCs retrieved during current task cycle   */
} J1939Dcm_DtcHandlerType;

typedef struct
{
  Dem_J1939DcmSetClearFilterType    ClearFilter;         /* filter for clear requests                                */
  uint8                             ClearNode;           /* node address for clear requests                          */
	Dem_DTCOriginType                 DTCOrigin;           /* Memory origin primary or secondary                       */
} J1939Dcm_ClearHandlerType;

typedef struct
{
  Dem_J1939DcmDTCStatusFilterType    StatusFilter;       /* Active previouslyActive Pending  DTCs                    */
  Dem_DTCKindType                    DTCKind;            /* EmissionRelated NonEmissionRelated All                   */
  Dem_DTCOriginType                  DTCOrigin;          /* Primary: Normation DTC or Secondary: ServiceOnly DTC     */
  J1939Dcm_NodeHdlType               FiltredNode;        /* Node address                                             */
} J1939Dcm_FilterHandlerType;

typedef struct
{
  PduInfoType             TxData;                        /* data to be transmitted                                   */
  PduIdType               TxPduId;                       /* Pdu to be transmitted                                    */
  uint8                   DiagMsg;                       /* message to be transmitted (J1939_DM_UNDEF if none)       */
  uint8                   DestinationAddress;            /* destination address of pending transmission              */
  uint8                   DM15Buffer[J1939DCM_MEM_ACCESS_MSG_LENGTH + J1939DCM_METADATA_LEN]; /* DM15 data           */
  J1939Dcm_NodeHdlType    NodeHdl;                       /* node for which transmission shall be triggered           */
  J1939Dcm_ChannelHdlType ChannelHdl;                    /* channel for which transmission shall be triggered        */
} J1939Dcm_MemHandlerType;

/* --- DM processing structures -------------------------------------------------------------------------------------*/
/* message processing context for DMs with DTCs */
typedef struct
{
  uint8                    ProcessingState;         /* current status of message processing                          */
  boolean                  HasDemAccess;            /* TRUE: DEM access in progress, semaphore is locked             */
  J1939Dcm_ChannelHdlType  ChannelHdl;              /* Channel for SendAck                                           */
  J1939Dcm_MessageHdlType  MessageHdl;              /* PGN for SendAck, TxPdu for Transmit                           */
  uint8                    ResponseDA;              /* address for response (Transmit) or ACK (SendAck)              */
  J1939Rm_AckCode          Acknowledge;             /* acknowledge code for SendAck                                  */
  uint8   Buffer[J1939DCM_BUFFERSIZE_COMMON];       /* buffer for DM message                                         */
  PduInfoType              BufferInfo;              /* control structure for message buffer                          */
} J1939Dcm_DMxStateType;

typedef struct
{
  uint8 RequestPending;                             /* bitmask with events affecting DM processing                   */
  uint8 ResponseDA;                                 /* QUEUE_REQ: destination address for response                   */
  J1939Dcm_ChannelHdlType ChannelHdl;               /* QUEUE_REQ: channel for response                               */
  J1939Dcm_MessageHdlType MessageHdl;               /* QUEUE_REQ: DM to be transmitted                               */
  J1939Dcm_StateType StateChangeRequest;            /* QUEUE_STATE: new communication state                          */
} J1939Dcm_DMxQueueType;



/* message processing context for DM1 */
typedef struct
{
  PduInfoType  BufferInfo;                          /* control structure for message buffer                          */
  uint16       Timer;                               /* timeout counter for periodic transmission                     */
  uint8        ProcessingState;                     /* current status of DM1 processing                              */
  uint8        ResponseDA;                          /* destination address for next DM1 transmission                 */
  uint8        Buffer[J1939DCM_BUFFERSIZE_DM1];     /* buffer for DM1 message                                        */
  J1939Dcm_ChannelHdlType ChannelHdl;               /* Channel for single DM1 request                                */
  PduIdType    TxPdus[J1939DCM_MAX_NUM_CHANNELS];   /* configured DM1 TxPdus for each channel                        */
  boolean      DTCSTatusChanged;                    /* TRUE: DTC status already changed since last periodic Tx       */
  boolean      HasDemAccess;                        /* TRUE: DEM access in progress, semaphore is locked             */
  boolean      IsTimerExpired;                      /* TRUE: Timer was expired                                       */
} J1939Dcm_DM1StateType;

typedef struct
{
  uint8 RequestPending;                             /* bitmask with DM1 triggers                                     */
  J1939Dcm_StateType StateChangeRequest;            /* QUEUE_STATE: new communication state                          */
  uint8 ResponseDA;                                 /* QUEUE_REQ: destination address for DM1 transmission           */
  J1939Dcm_ChannelHdlType ChannelHdl;               /* QUEUE_REQ: channel for DM1 transmission                       */
} J1939Dcm_DM1QueueType;


/* message processing context for DM13 */
typedef struct
{
  uint16  BroadcastDisableTimer;                        /* timeout counter while broadcast is disabled               */
  uint16  BroadcastStatusInformation;                   /* bit mask provided to the BSWM                             */
  uint8   NumActiveBroadcastChannels;                   /* channels with active broadcast; if 0, stop DM1 & DM35     */
} J1939Dcm_DM13StateType;

typedef struct
{
  boolean RequestPending;                               /* TRUE: DM13 have been received but not yet processed       */
  uint8   BroadcastCommand[J1939DCM_MAX_NUM_CHANNELS];  /* broadcast command of last DM13 (start, stop, none)        */
  uint8   HoldSignal;                                   /* hold signal of last DM13                                  */
} J1939Dcm_DM13QueueType;


/* message processing context for DM35 */
typedef struct
{
  PduInfoType  BufferInfo;                          /* control structure for message buffer                          */
  uint16       Timer;                               /* timeout counter for periodic transmission                     */
  uint8        ProcessingState;                     /* current status of DM35 processing                             */
  uint8        Buffer[J1939DCM_BUFFERSIZE_COMMON];  /* buffer for DM35 message                                       */
  PduIdType    TxPdus[J1939DCM_MAX_NUM_CHANNELS];   /* TxPdus for each channel                                       */
  J1939Dcm_ChannelSupportType ActiveChannels;       /* bitmask of channels which transmit DM35                       */
  boolean      HasDemAccess;                        /* TRUE: DEM access in progress, semaphore is locked             */
} J1939Dcm_DM35StateType;

typedef struct
{
  uint8 RequestPending;                             /* bitmask with DM35 triggers                                    */
  J1939Dcm_StateType StateChangeRequest;            /* QUEUE_STATE: new communication state                          */
  J1939Dcm_ChannelSupportType RequestedChannels;    /* QUEUE_REQ: channels for which DM35 shall be transmitted       */
} J1939Dcm_DM35QueueType;




#endif  /* J1939DCM_SOURCE */
#endif  /* J1939DCM_PRIV_H */

/**********************************************************************************************************************
 *  END OF FILE: J1939Dcm_Priv.h
 *********************************************************************************************************************/
