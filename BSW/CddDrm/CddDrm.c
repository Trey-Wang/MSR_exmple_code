/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  CddDrm.c
 *        \brief  CddDrm source file
 *
 *      \details
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

#define CDDDRM_SOURCE

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "CddDrm.h"
#include "PduR_CddDrm.h"

/* critical sections handled by SchM */
#include "SchM_CddDrm.h"

#include "CddDrm_Cbk.h"

#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of CddDrm header file */
#if (  (CDDDRM_SW_MAJOR_VERSION != (1u)) \
    || (CDDDRM_SW_MINOR_VERSION != (0u)) \
    || (CDDDRM_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of CddDrm.c and CddDrm.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (CDDDRM_CFG_SW_MAJOR_VERSION != (1u)) \
    || (CDDDRM_CFG_SW_MINOR_VERSION != (0u)) )
# error "Version numbers of CddDrm.c and CddDrm_Cfg.h are inconsistent!"
#endif

/* Check the version of the type header file */
#if (  (CDDDRM_TYPES_MAJOR_VERSION != (1u)) \
    || (CDDDRM_TYPES_MINOR_VERSION != (0u)) \
    || (CDDDRM_TYPES_PATCH_VERSION != (0u)) )
#  error "Version numbers of CddDrm.c and CddDrm_Types.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#if !defined (CDDDRM_LOCAL)
# define CDDDRM_LOCAL static
#endif

#if !defined (CDDDRM_LOCAL_INLINE)
# define CDDDRM_LOCAL_INLINE LOCAL_INLINE
#endif

/* ----- Channel reference      ------------------------------------------------------------------------------------ */
#define CDDDRM_CHANNEL_INFO_FREE                                       (0xffU) /*<! Channel reference free */
#define CDDDRM_CHANNEL_INFO_NOT_FREE                                   (0xfeU) /*<! Channel reference in use */
#define CDDDRM_CHANNEL_INFO_MAX_NUMBER                                 CDDDRM_CHANNEL_INFO_NOT_FREE

/* ----- Channel States -------------------------------------------------------------------------------------------- */
#define CDDDRM_CHANNEL_STATE_INACTIVE                                  (0U)      /*<! Channel inactive */
#define CDDDRM_CHANNEL_STATE_WAIT_DELAY                                (1U)      /*<! Channel waits for delay elapsed */
#define CDDDRM_CHANNEL_STATE_REQUEST_START                             (2U)      /*<! Channel starts sending request */
#define CDDDRM_CHANNEL_STATE_REQUEST_COPY                              (3U)      /*<! Channel copies request data */
#define CDDDRM_CHANNEL_STATE_WAIT_RESPONSE                             (4U)      /*<! Channel waits for response */
#define CDDDRM_CHANNEL_STATE_RESPONSE_START                            (5U)      /*<! Channel receives response */
#define CDDDRM_CHANNEL_STATE_RESPONSE_COPY                             (6U)      /*<! Channel copies response into application buffer */
#define CDDDRM_CHANNEL_STATE_RESPONSE_DONE                             (7U)      /*<! Channel notifies Application with response data */

/* ----- Channel Allocation -------------------------------------------------------------------------------------------- */
# define CDDDRM_CHANNEL_CLIENT_ECU_DETECTION                           (0U)
# define CDDDRM_CHANNEL_CLIENT_APPLICATION                             (1U)

/* ----- Drm States ------------------------------------------------------------------------------------------------ */
#define CDDDRM_STATE_RUN                                               (0U)      /*<! Normal run mode */
#define CDDDRM_STATE_TESTER_DETECTED                                   (1U)      /*<! External tester is present */

/* ----- CDDDRM Response validation value -------------------------------------------------------------------------- */
#define CDDDRM_RESPONSE_CODE_VALIDATION                                (0x40U)   /*<! Used for validate response */

/* ----- PduR busy retry ------------------------------------------------------------------------------------------- */
#if (CDDDRM_CHANNEL_PDUR_RETRY_VALUE > 0)
# define CDDDRM_SUPPORT_CHANNEL_PDUR_RETRY                             STD_ON
#else
# define CDDDRM_SUPPORT_CHANNEL_PDUR_RETRY                             STD_OFF
#endif
/* ----- QUEUE ----------------------------------------------------------------------------------------------------- */
#define CDDDRM_QUEUE_EMPTY                                             (0xffU)
/* ----- QUEUE_ECUD ----- */
#define CDDDRM_QUEUE_ECUD_START                                        (0U)
#define CDDDRM_QUEUE_ECUD_STOP                                         (1U)
/* External tester queue */
#define CDDDRM_QUEUE_EXTERNAL_TESTER_CONNECTED                         (0U)
#define CDDDRM_QUEUE_EXTERNAL_TESTER_DISCONNECTED                      (1U)

/* ----- ECU Detection Parameters ---------------------------------------------------------------------------------- */
/* ----- Ecud_States ------ */
#define CDDDRM_ECUD_STATE_IDLE                                         (0U)
#define CDDDRM_ECUD_STATE_READY                                        (1U)
#define CDDDRM_ECUD_STATE_BUSY                                         (2U)
#define CDDDRM_ECUD_STATE_WRITE_NVM                                    (3U)
#define CDDDRM_ECUD_STATE_FINISH                                       (4U)
/* ----- Ecud result ----- */
#define CDDDRM_ECUD_RESULT_MASK                                        (0x03U)
/* ----- Dependent on configuration ------ */
#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
# define CDDDRM_CFG_ECUD_INDEX_LIMIT                                   (CDDDRM_CFG_CONNECTION_COUNT)
/* define start index of channel iteration if ECU Detection is enabled */
# define CDDDRM_CHANNEL_ECU_DETECTION_INDEX                            (0U)
/* define the channel index, that is used for ECU Detection */
# define CDDDRM_CHANNEL_APPLICATION_INDEX                              (1U)
# define CDDDRM_APPL_CHANNEL_COUNT                                     (CDDDRM_CFG_CHANNEL_COUNT - 1)
#else
/* define start index of channel iteration */
# define CDDDRM_CHANNEL_APPLICATION_INDEX                               (0U)
# define CDDDRM_APPL_CHANNEL_COUNT                                     (CDDDRM_CFG_CHANNEL_COUNT)
#endif

/* ----- UDS Services ---------------------------------------------------------------------------------------------- */
#define CDDDRM_SERVICE_10                                              (0x10U)
#define CDDDRM_SERVICE_11                                              (0x11U)
#define CDDDRM_SERVICE_14                                              (0x14U)
#define CDDDRM_SERVICE_19                                              (0x19U)
#define CDDDRM_SERVICE_22                                              (0x22U)
#define CDDDRM_SERVICE_23                                              (0x23U)
#define CDDDRM_SERVICE_24                                              (0x24U)
#define CDDDRM_SERVICE_27                                              (0x27U)
#define CDDDRM_SERVICE_28                                              (0x28U)
#define CDDDRM_SERVICE_2A                                              (0x2aU)
#define CDDDRM_SERVICE_2C                                              (0x2cU)
#define CDDDRM_SERVICE_2E                                              (0x2eU)
#define CDDDRM_SERVICE_2F                                              (0x2fU)
#define CDDDRM_SERVICE_31                                              (0x31U)
#define CDDDRM_SERVICE_34                                              (0x34U)
#define CDDDRM_SERVICE_35                                              (0x35U)
#define CDDDRM_SERVICE_36                                              (0x36U)
#define CDDDRM_SERVICE_37                                              (0x37U)
#define CDDDRM_SERVICE_38                                              (0x38U)
#define CDDDRM_SERVICE_3D                                              (0x3dU)
#define CDDDRM_SERVICE_3E                                              (0x3eU)
#define CDDDRM_SERVICE_83                                              (0x83U)
#define CDDDRM_SERVICE_84                                              (0x84U)
#define CDDDRM_SERVICE_85                                              (0x85U)
#define CDDDRM_SERVICE_86                                              (0x86U)
#define CDDDRM_SERVICE_87                                              (0x87U)

/* ----- Dedicated Service Parameters ------------------------------------------------------------------------------ */
#if (CDDDRM_CFG_SUPPORT_SERVICE_10 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_10                                     (2U)
# define CDDDRM_SERVICE_10_MIN_RESPONSE_BUFFER                         (6U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_11 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_11                                     (2U)
# define CDDDRM_SERVICE_11_MIN_RESPONSE_BUFFER                         (2U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_1902 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_1902                                   (3U)
# define CDDDRM_SERVICE_1902_MIN_RESPONSE_BUFFER                       (3U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_1904 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_1904                                   (6U)
# define CDDDRM_SERVICE_1904_MIN_RESPONSE_BUFFER                       (3U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_22 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_22                                     (3U)
# define CDDDRM_SERVICE_22_MIN_RESPONSE_BUFFER                         (4U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_27 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_27                                     (2U)
# define CDDDRM_SERVICE_27_MIN_RESPONSE_BUFFER                         (2U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_28 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_28                                     (3U)
# define CDDDRM_SERVICE_28_MIN_RESPONSE_BUFFER                         (2U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_31 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_31                                     (4U)
# define CDDDRM_SERVICE_31_MIN_RESPONSE_BUFFER                         (4U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_34 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_34                                     (3U)
# define CDDDRM_SERVICE_34_MIN_RESPONSE_BUFFER                         (4U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_36 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_36                                     (2U)
# define CDDDRM_SERVICE_36_MIN_RESPONSE_BUFFER                         (2U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_37 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_37                                     (1U)
# define CDDDRM_SERVICE_37_MIN_RESPONSE_BUFFER                         (1U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_3E == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_3E                                     (2U)
# define CDDDRM_SERVICE_3E_MIN_RESPONSE_BUFFER                         (2U)
#endif
#if (CDDDRM_CFG_SUPPORT_SERVICE_85 == STD_ON)
# define CDDDRM_PROTOCOL_LENGTH_85                                     (2U)
# define CDDDRM_SERVICE_85_MIN_RESPONSE_BUFFER                         (2U)
#endif

/* ----- Local conversion of active firewall rules ----------------------------------------------------------------- */
#if (CDDDRM_CFG_BITPOS_SERVICE_10 != 0)
#define CDDDRM_FIREWALL_SERVICE_10                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_10                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_11 != 0)
#define CDDDRM_FIREWALL_SERVICE_11                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_11                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_14 != 0)
#define CDDDRM_FIREWALL_SERVICE_14                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_14                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_19 != 0)
#define CDDDRM_FIREWALL_SERVICE_19                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_19                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_22 != 0)
#define CDDDRM_FIREWALL_SERVICE_22                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_22                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_23 != 0)
#define CDDDRM_FIREWALL_SERVICE_23                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_23                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_24 != 0)
#define CDDDRM_FIREWALL_SERVICE_24                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_24                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_27 != 0)
#define CDDDRM_FIREWALL_SERVICE_27                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_27                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_28 != 0)
#define CDDDRM_FIREWALL_SERVICE_28                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_28                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_2A != 0)
#define CDDDRM_FIREWALL_SERVICE_2A                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_2A                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_2C != 0)
#define CDDDRM_FIREWALL_SERVICE_2C                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_2C                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_2E != 0)
#define CDDDRM_FIREWALL_SERVICE_2E                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_2E                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_2F != 0)
#define CDDDRM_FIREWALL_SERVICE_2F                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_2F                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_31 != 0)
#define CDDDRM_FIREWALL_SERVICE_31                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_31                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_34 != 0)
#define CDDDRM_FIREWALL_SERVICE_34                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_34                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_35 != 0)
#define CDDDRM_FIREWALL_SERVICE_35                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_35                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_36 != 0)
#define CDDDRM_FIREWALL_SERVICE_36                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_36                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_37 != 0)
#define CDDDRM_FIREWALL_SERVICE_37                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_37                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_38 != 0)
#define CDDDRM_FIREWALL_SERVICE_38                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_38                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_3D != 0)
#define CDDDRM_FIREWALL_SERVICE_3D                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_3D                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_3E != 0)
#define CDDDRM_FIREWALL_SERVICE_3E                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_3E                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_83 != 0)
#define CDDDRM_FIREWALL_SERVICE_83                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_83                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_84 != 0)
#define CDDDRM_FIREWALL_SERVICE_84                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_84                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_85 != 0)
#define CDDDRM_FIREWALL_SERVICE_85                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_85                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_86 != 0)
#define CDDDRM_FIREWALL_SERVICE_86                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_86                                     STD_OFF
#endif
#if (CDDDRM_CFG_BITPOS_SERVICE_87 != 0)
#define CDDDRM_FIREWALL_SERVICE_87                                     STD_ON
#else
#define CDDDRM_FIREWALL_SERVICE_87                                     STD_OFF
#endif

#define CDDDRM_CFG_ISCONST_FIREWALL_BITMASK                            CDDDRM_ISDEF_FIREWALLRULELISTS

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#define CddDrm_GetChannelMessageLength(channelIndex)                   (CddDrm_ChannelInfo[(channelIndex)].messageLength)                   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetChannelMessageLength(channelIndex, val)              (CddDrm_ChannelInfo[(channelIndex)].messageLength = (val))           /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetChannelCurrentMessageIndex(channelIndex)             (CddDrm_ChannelInfo[(channelIndex)].currentMessageIndex)             /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetChannelCurrentMessageIndex(channelIndex, val)        (CddDrm_ChannelInfo[(channelIndex)].currentMessageIndex = (val))     /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_AddChannelCurrentMessageIndex(channelIndex, addVal)     (CddDrm_ChannelInfo[(channelIndex)].currentMessageIndex += (addVal)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetChannelServiceId(channelIndex)                       (CddDrm_ChannelInfo[(channelIndex)].protocolData[0])                 /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetChannelServiceId(channelIndex, val)                  (CddDrm_ChannelInfo[(channelIndex)].protocolData[0] = (val))         /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetChannelResponseCode(channelIndex)                    (CddDrm_ChannelInfo[(channelIndex)].responseCode)                    /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetChannelResponseCode(channelIndex, val)               (CddDrm_ChannelInfo[(channelIndex)].responseCode = (val))            /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetChannelPduRBusyCounter(channelIndex)                 (CddDrm_ChannelInfo[(channelIndex)].pdurBusyCounter)                 /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetChannelPduRBusyCounter(channelIndex, val)            (CddDrm_ChannelInfo[(channelIndex)].pdurBusyCounter = (val))         /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_DecChannelPduRBusyCounter(channelIndex)                 (CddDrm_ChannelInfo[(channelIndex)].pdurBusyCounter--)               /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetChannelSPRMIB(channelIndex)                          (CddDrm_ChannelInfo[(channelIndex)].useSPRMIB)                       /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetChannelSPRMIB(channelIndex, val)                     (CddDrm_ChannelInfo[(channelIndex)].useSPRMIB = (val))               /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* ----- request buffer macros ------------------------------------------------------------------------------------- */
#define CddDrm_GetChannelReqBufferInfoPtrVal(channelIndex, ptrIndex)   (CddDrm_ChannelInfo[(channelIndex)].bufferInfo.requestBufferDataPtr[(ptrIndex)])               /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetChannelReqBufferInfoPtrVal(channelIndex, ptrIndex, val) (CddDrm_ChannelInfo[(channelIndex)].bufferInfo.requestBufferDataPtr[(ptrIndex)] = (val))    /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetChannelReqBufferInfoPtr(channelIndex, pointer)       (CddDrm_ChannelInfo[(channelIndex)].bufferInfo.requestBufferDataPtr = (pointer))               /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* ----- response buffer macros ------------------------------------------------------------------------------------ */
#define CddDrm_GetChannelRespBufferInfoPtrVal(channelIndex, ptrIndex)  (CddDrm_ChannelInfo[(channelIndex)].bufferInfo.responseBufferDataPtr[(ptrIndex)])              /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetChannelRespBufferInfoPtrVal(channelIndex, ptrIndex, val) (CddDrm_ChannelInfo[(channelIndex)].bufferInfo.responseBufferDataPtr[(ptrIndex)] = (val))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetChannelRespBufferInfoPtr(channelIndex, pointer)      (CddDrm_ChannelInfo[(channelIndex)].bufferInfo.responseBufferDataPtr = (pointer))              /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetChannelRespBufferInfoSize(channelIndex)              (CddDrm_ChannelInfo[(channelIndex)].bufferInfo.responseBufferSize)                             /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetChannelRespBufferInfoSize(channelIndex, val)         (CddDrm_ChannelInfo[(channelIndex)].bufferInfo.responseBufferSize = (val))                     /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDrm_GetChannelState(channelIndex)                           (CddDrm_ChannelInfo[(channelIndex)].channelState)                                    /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetChannelState(channelIndex, val)                      (CddDrm_ChannelInfo[(channelIndex)].channelState = (val))                            /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetChannelRCRRP(channelIndex)                           (CddDrm_ChannelInfo[(channelIndex)].rcrrpCounter)                                    /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetChannelRCRRP(channelIndex, val)                      (CddDrm_ChannelInfo[(channelIndex)].rcrrpCounter = (val))                            /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_DecChannelRCRRP(channelIndex)                           (CddDrm_ChannelInfo[(channelIndex)].rcrrpCounter--)                                  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetChannelP2Timer(channelIndex)                         (CddDrm_ChannelInfo[(channelIndex)].P2Timer)                                         /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetChannelP2Timer(channelIndex, val)                    (CddDrm_ChannelInfo[(channelIndex)].P2Timer = (val))                                 /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_DecChannelP2Timer(channelIndex)                         (CddDrm_ChannelInfo[(channelIndex)].P2Timer--)                                       /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetChannelProtocolLength(channelIndex)                  (CddDrm_ChannelInfo[(channelIndex)].protocolDataLength)                              /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetChannelProtocolLength(channelIndex, val)             (CddDrm_ChannelInfo[(channelIndex)].protocolDataLength = (val))                      /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetConnectionId(channelIndex)                           (CddDrm_ChannelInfo[(channelIndex)].connectionId)                                    /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetConnectionId(channelIndex, val)                      (CddDrm_ChannelInfo[(channelIndex)].connectionId = (val))                            /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetConnectionReqPduId(channelIndex)                     (CddDrm_Cfg_GetConnectionReqPduId(CddDrm_ChannelInfo[(channelIndex)].connectionId))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetConnectionResPduId(channelIndex)                     (CddDrm_Cfg_GetConnectionResPduId(CddDrm_ChannelInfo[(channelIndex)].connectionId))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetProtocolBufferActive(channelIndex)                   (CddDrm_ChannelInfo[(channelIndex)].useProtocolBuffer)                               /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_SetProtocolBufferActive(channelIndex)                   (CddDrm_ChannelInfo[(channelIndex)].useProtocolBuffer = TRUE)                        /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_ResetProtocolBufferActive(channelIndex)                 (CddDrm_ChannelInfo[(channelIndex)].useProtocolBuffer = FALSE)                       /* PRQA S 3453 */ /* MD_MSR_19.7 */
/* ----- decompose a 32 bit (value) into a byte stream ------------------------------------------------------------- */
#define CddDrm_GetHiHiByte(value)                                      ((uint8)((uint32)(value)>>24)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetHiLoByte(value)                                      ((uint8)((uint32)(value)>>16)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetLoHiByte(value)                                      ((uint8)((uint32)(value)>>8))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetLoLoByte(value)                                      ((uint8)(value))               /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* ----- decompose a 16 bit (value) into a byte stream ------------------------------------------------------------- */
#define CddDrm_GetHiByte(value)                                        ((uint8)((uint16)(value)>>8))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CddDrm_GetLoByte(value)                                        ((uint8)((value)))             /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* ----- Suppress positive response macro functions ---------------------------------------------------------------- */
#define CDDDRM_SET_SUPPRESS_POS_RESPONSE_BIT(value)                    ((value) | (0x80U))            /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CDDDRM_RESET_SUPPRESS_POS_RESPONSE_BIT(value)                  ((value) & ~(0x80U))           /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* ----- Access Macros for ECU Detection --------------------------------------------------------------------------- */
/* The 2bit ECU detection result is stored in a byte array, where each byte contains the result for 4 connections.
 * CDDDRM_ECUD_GET_CONNECTION_INDEX returns the index in the array for a connection.
 * CDDDRM_ECUD_GET_CONNECTION_POS returns the bit position within on byte of the array for the detection result of a connection. */
#define CDDDRM_ECUD_GET_CONNECTION_INDEX(connectionId)                 ((connectionId) / 4)                                                         /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CDDDRM_ECUD_GET_CONNECTION_POS(connectionId)                   (((connectionId) % 4) * 2) /* get position of relevant two bits in a byte */ /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CDDDRM_ECUD_SET_CONNECTION_NOT_AVAILABLE(var, connId)          ((CddDrm_EcudStateType)((var) | (CDDDRM_ECUD_CONNECTION_NOT_AVAILABLE << CDDDRM_ECUD_GET_CONNECTION_POS(connId))))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define CDDDRM_ECUD_SET_CONNECTION_AVAILABLE(var, connId)              ((CddDrm_EcudStateType)((var) | (CDDDRM_ECUD_CONNECTION_AVAILABLE << CDDDRM_ECUD_GET_CONNECTION_POS(connId))))      /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define CddDrm_Ecud_GetRamBlockInitPattern()                           (CddDrm_Cfg_EcuDetectionData.InitPattern)
#define CddDrm_Ecud_SetRamBlockInitPattern(val)                        (CddDrm_Cfg_EcuDetectionData.InitPattern = (val))                            /* PRQA S 3453 */ /* MD_MSR_19.7 */

/**********************************************************************************************************************
 * INCOMPATIBLE FEATURE CHECK
 *********************************************************************************************************************/
/* separation time & request delay */
#if ((CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON) && (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON))
# error "Global delay request and connection specific delay time cannot be enabled at the same time"
#endif
/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define CDDDRM_START_SEC_VAR_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Initialization state of the module */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_INIT) CddDrm_ModuleInitialized = CDDDRM_UNINIT;                               /* PRQA S 3229 */ /* MD_CDDDRM_3229 */

#define CDDDRM_STOP_SEC_VAR_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define CDDDRM_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Drm state */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_State;
/*! Dynamic mapping: connection id to channel index */
CDDDRM_LOCAL VAR(CddDrm_ChannelIdType, CDDDRM_VAR_NOINIT) CddDrm_ChannelAssign[CDDDRM_CFG_CONNECTION_COUNT];
/*! Indicate that external tester was connected/detected */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_Queue_TesterConnected;
#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/*! Connection Scan State */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_Ecud_State;
/*! Indicates that the application want to change the ECU Detection state */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_Queue_Ecud;
/*! ECU Detection response buffer with configurable size */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_Ecud_ResponseBuffer[CDDDRM_CFG_ECUD_RESPONSE_BUFFER_SIZE]; /* PRQA S 3218 */ /* MD_CDDDRM_8.7 */
#endif

#if (CDDDRM_CFG_SUPPORT_CANCELLATION == STD_ON)
/* Flags that can be set with APIs by Application and will processed on Task */
/* Specifies which connection shall be closed */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_Queue_CancelRequest[CDDDRM_CFG_CHANNEL_COUNT];
/* Indicate that specific connection shall be closed */
CDDDRM_LOCAL VAR(boolean, CDDDRM_VAR_NOINIT) CddDrm_Queue_CancelRequestFlag;
#endif

#if (CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON)
/* Separation timer for each connection */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_SeparationTimer[CDDDRM_CFG_CONNECTION_COUNT];
#endif

/*! Counter for active connection channels */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_NOINIT) CddDrm_ActiveChannels;

#define CDDDRM_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define CDDDRM_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
/*! Timer for delay between requests, loaded to zero at startup */
CDDDRM_LOCAL VAR(CddDrm_TimerType, CDDDRM_VAR_NOINIT) CddDrm_RequestDelayTimer;
#endif

#define CDDDRM_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define CDDDRM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/*! Current processing connection index */
CDDDRM_LOCAL VAR(CddDrm_ConnectionTableIterType, CDDDRM_VAR_NOINIT) CddDrm_Ecud_ConnectionIterationIndex;
#endif

/*! Struct array with information about Connections */
CDDDRM_LOCAL VAR(CddDrm_ChannelStructType, CDDDRM_VAR_NOINIT) CddDrm_ChannelInfo[CDDDRM_CFG_CHANNEL_COUNT];

#define CDDDRM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define CDDDRM_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
/*! This was used for processing connections in CDDDRM_CHANNEL_STATE_WAIT_DELAY state, to prevent starving of connections */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_INIT) CddDrm_NextConnection;
#endif

#define CDDDRM_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define CDDDRM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Pointer to the current configuration */
CDDDRM_LOCAL P2CONST(CddDrm_ConfigPtrType, CDDDRM_VAR_INIT, CDDDRM_PBCFG) CddDrm_ConfigSet_pt; /* PRQA S 3229 */ /* MD_CDDDRM_3229 */

#define CDDDRM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CDDDRM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
  CddDrm_Cm_ChannelInit()
 *********************************************************************************************************************/
/*! \brief      Initialize channel
 *  \details    Initialize channel to default values
 *  \param[in]  ChannelId                     Channel that is assigned to connection
 *  \param[in]  ServiceId                     Service Id
 *  \param[in]  Callback                      Internal reference to Notify Function
 *  \param[in]  RequestLength                 Length of request that should send
 *  \param[in]  SPRMIB                        Indicates if positive response is expected or not 
 *  \param[in]  BufferInfo                    Information about application buffer
 *  \pre        Channel is allocated
 *  \context    TASK|ISR2
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_ChannelInit(CddDrm_ChannelIdType ChannelId,
                                                           uint8 ServiceId,
                                                           CddDrm_ResponseNotificationPtr Callback,
                                                           CddDrm_LengthType RequestLength,
                                                           boolean SPRMIB,
                                                           P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_CONST) BufferInfo);

/**********************************************************************************************************************
  CddDrm_Cm_StateTask()
 *********************************************************************************************************************/
/*! \brief       Process open connections
 *  \details     Open channel state will be handled, excluded timeouts
 *  \pre         Minimum one connection is active
 *  \context     TASK
 *  \synchronous TRUE
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_StateTask(void);

/**********************************************************************************************************************
  CddDrm_Cm_ChannelCloseAll()
 *********************************************************************************************************************/
/*! \brief       Closes immediately all open channels.
 *  \details     This function is immediately closing all open channels.
 *  \param[in]   ResponseCode          the reason why channels are closed.
 *  \pre         -
 *  \context     TASK
 *  \synchronous TRUE
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_ChannelCloseAll(CddDrm_ResponseCodeType ResponseCode);

/**********************************************************************************************************************
  CddDrm_Timer_Task()
 *********************************************************************************************************************/
/*! \brief       Handle channel timer
 *  \details     Handle DRM channel delay between requests and channel P2/P2* timeouts
 *  \pre         -
 *  \context     TASK
 *  \synchronous TRUE
 **********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_Task(void);

#if (CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON)
/**********************************************************************************************************************
  CddDrm_Timer_ConnectionDelay()
 *********************************************************************************************************************/
/*! \brief       Timer processing for the connection specific delay time
 *  \details     -
 *  \param[in]   ConnectionId unique handle of the connection
 *  \pre         -
 *  \context     TASK
 *  \synchronous TRUE
 **********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_ConnectionDelay(CddDrm_ConnectionTableIterType ConnectionId);
#endif

/**********************************************************************************************************************
  CddDrm_Timer_P2()
 *********************************************************************************************************************/
/*! \brief       Timer processing for the P2 and P2 extended time
 *  \details     -
 *  \param[in]   ConnectionId unique handle of the connection
 *  \pre         -
 *  \context     TASK
 *  \synchronous TRUE
 **********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_P2(CddDrm_ConnectionTableIterType ConnectionId);

#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
/**********************************************************************************************************************
  CddDrm_Timer_GlobalDelay()
 *********************************************************************************************************************/
/*! \brief       Timer processing for the global delay time
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \synchronous TRUE
 **********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_GlobalDelay(void);
#endif

/**********************************************************************************************************************
  CddDrm_Cm_ResponseStart()
 *********************************************************************************************************************/
/*! \brief       Gets first three bytes from PduR.
 *  \details     Copy three bytes of response from PduR to analyze if the response is a NRC or a positive response.
 *  \param[in]   ChannelId                has all information about connection to copy response
 *  \pre         Channel must be in RESPONSE_START state.
 *  \context     TASK
 *  \synchronous TRUE
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_ResponseStart(CddDrm_ChannelIdType ChannelId);

/**********************************************************************************************************************
  CddDrm_Cm_ResponseCopy()
 *********************************************************************************************************************/
/*! \brief       Copies the whole response to application buffer
 *  \details     Copeis response from PduR into application buffer
 *  \param[in]   ChannelId               has all information about connection to copy response
 *  \pre         Channel must be in RESPONSE_COPY state.
 *  \synchronous TRUE
 *  \context     TASK
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_ResponseCopy(CddDrm_ChannelIdType ChannelId);

/**********************************************************************************************************************
  CddDrm_Cm_RequestStart()
 *********************************************************************************************************************/
/*! \brief       DRM initiates the transmission to PduR.
 *  \details     DRM copy the max. possible length of channel data to PduR, if more data available than here copied
 *               the next channel state will copy them.
 *  \param[in]   ChannelId             which is allocated to a connection
 *  \pre         Channel must be in REQUEST_START state
 *  \synchronous TRUE
 *  \context     TASK
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_RequestStart(CddDrm_ChannelIdType ChannelId);

/**********************************************************************************************************************
  CddDrm_Cm_RequestCopy()
 *********************************************************************************************************************/
/*! \brief       DRM transmit on every call a piece of request data
 *  \details     While request data available DRM copies the channel data to PduR
 *  \param[in]   ChannelId             which is allocated to a connection
 *  \pre         Channel must be in REQUEST_COPY state.
 *  \context     TASK
 *  \synchronous TRUE
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_RequestCopy(CddDrm_ChannelIdType ChannelId);

/**********************************************************************************************************************
  CddDrm_Cm_ChannelClose()
 *********************************************************************************************************************/
/*! \brief       Close channel and notify application
 *  \details     Notify application about success or failure
 *  \param[in]   ChannelId               that will be used for application notification and closing the channel
 *  \pre         channel must be active
 *  \context     TASK
 *  \synchronous TRUE
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_ChannelClose(CddDrm_ChannelIdType ChannelId);

/**********************************************************************************************************************
  CddDrm_Cm_ChannelGet()
 *********************************************************************************************************************/
/*! \brief       Look for free channel
 *  \details     Returns the first free channel that is found
 *  \param[in]   ConnectionId                 connection related to the ECU to which the request shall be send
 *  \param[in]   ChannelClient                The client that needs to allocate a channel
 *                                            Application or ECU Detection
 *  \return      0-253:                       Possible channel IDs
 *  \return      CDDDRM_CHANNEL_INFO_MAX_NUMBER: No channel found
 *  \pre         -
 *  \synchronous TRUE
 *  \context     TASK
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(CddDrm_ChannelIdType, CDDDRM_CODE) CddDrm_Cm_ChannelGet(CddDrm_ConnectionIdType ConnectionId,
                                                                          uint8 ChannelClient);

/**********************************************************************************************************************
  CddDrm_Cm_ChannelAllocate()
 *********************************************************************************************************************/
/*! \brief       Assign a connection to a channel
 *  \details     Initialize the channel state and assign a connection to a channel
 *  \param[in]   ChannelId                    channel with will be assigned and initialized
 *  \param[in]   ConnectionId                 connection related to the ECU to which the request shall be send
 *  \return      -
 *  \pre         Interrupts must be disabled
 *  \synchronous TRUE
 *  \context     TASK
 *********************************************************************************************************************/
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_Cm_ChannelAllocate(CddDrm_ChannelIdType ChannelId,
                                                                      CddDrm_ConnectionIdType ConnectionId);

#if (CDDDRM_CFG_SUPPORT_RCRRP_PROCESSING == STD_ON)
/**********************************************************************************************************************
  CddDrm_Cm_ProcessRCRRP()
 *********************************************************************************************************************/
/*! \brief       Processing RCRRP counter.
 *  \details     Processing RCRRP counter and returns new channel state.
 *  \param[in]   ChannelId                   the channel which has to be processed
 *  \return      Channel state
 *  \pre         channel must be active
 *  \synchronous TRUE
 *  \context     TASK
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(CddDrm_ChannelStateType, CDDDRM_CODE) CddDrm_Cm_ProcessRCRRP(CddDrm_ChannelIdType ChannelId);
#endif

#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
/**********************************************************************************************************************
  CddDrm_Cfg_CheckFirewallRule()
 *********************************************************************************************************************/
/*! \brief       Check firewall rule for connection.
 *  \details     Check if service is allowed to the connection.
 *  \param[in]   ConnectionId   connection related to the ECU to which the request shall be send
 *  \param[in]   ServiceId      Service Id, which has to be checked
 *  \return      TRUE:  The ServiceId is allowed for the given connection
 *               FALSE: The ServiceId is not allwoed for the given connection
 *  \pre         channel must be active
 *  \synchronous TRUE
 *  \context     TASK
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(boolean, CDDDRM_CODE) CddDrm_Cfg_CheckFirewallRule(CddDrm_ConnectionIdType ConnectionId, uint8 ServiceId);
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
  CddDrm_Ecud_SetupRequest()
 *********************************************************************************************************************/
/*! \brief       Setup a request
 *  \details     Allocate and initialize the current connection index
 *  \return      -
 *  \pre         External tester is not active. ECU Detection is running.
 *  \synchronous TRUE
 *  \context     TASK
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Ecud_SetupRequest(void);
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
  CddDrm_Ecud_ResponseNotification()
 *********************************************************************************************************************/
/*! \brief       Notification handler for ECU Detection
 *  \details     Drm internal response notification that is called as soon as a response for the ECU detection request
 *               was received -> Similar to the application response notification
 *  \param[in]   Response   contains the relevant information regarding the received response message.
 *  \return      -
 *  \pre         -
 *  \synchronous TRUE
 *  \context     TASK
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Ecud_ResponseNotification(P2CONST(CddDrm_RespInfoStructType, AUTOMATIC, CDDDRM_CONST) Response);
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
  CddDrm_Ecud_StateTask()
 *********************************************************************************************************************/
/*! \brief       Handle the ECU Detection
 *  \details     Automatically setup requests to connections that not been discovered.
 *  \return      -
 *  \pre         External tester is not active
 *  \synchronous TRUE
 *  \context     TASK
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Ecud_StateTask(void);
#endif

/**********************************************************************************************************************
  CddDrm_Queue_ExternalTester()
 *********************************************************************************************************************/
/*! \brief       Handles the External Tester Queue
 *  \details     Closes open connections if an external tester is detected or set the Drm back to normal working mode.
 *  \return      -
 *  \pre         -
 *  \synchronous TRUE
 *  \context     TASK
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Queue_ExternalTester(void);

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
  CddDrm_Queue_EcuDetection()
 *********************************************************************************************************************/
/*! \brief       Handles the ECU Detection Queue
 *  \details     Starts and stops the ECU Detection.
 *  \return      -
 *  \pre         -
 *  \synchronous TRUE
 *  \context     TASK
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Queue_EcuDetection(void);
#endif

#if (CDDDRM_CFG_SUPPORT_CANCELLATION == STD_ON)
/**********************************************************************************************************************
  CddDrm_Queue_RequestCancellation()
 *********************************************************************************************************************/
/*! \brief       Handles the cancel request Queue
 *  \details     Cancels connections the indicated to close from application.
 *  \return      -
 *  \pre         -
 *  \synchronous TRUE
 *  \context     TASK
 *********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Queue_RequestCancellation(void);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Ecud_ResponseNotification()
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
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Ecud_ResponseNotification(P2CONST(CddDrm_RespInfoStructType, AUTOMATIC, CDDDRM_CONST) Response)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_EcudStateType lEcuState = CddDrm_Cfg_EcuDetectionData.EcuList[CDDDRM_ECUD_GET_CONNECTION_INDEX(Response->connectionId)];
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check for expected call of response notification */
  if (CddDrm_Ecud_State == CDDDRM_ECUD_STATE_BUSY)
  {
    /* #20 Convert response code to ECU detected state */
    switch(Response->responseCode)
    {
# if (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON)
      case CDDDRM_RESPONSE_PDUR_RX_CANCELED:
# endif
      case CDDDRM_RESPONSE_PDUR_TX_ERROR:
      case CDDDRM_RESPONSE_PDUR_RX_ERROR:
        /* ECU detection state initialized to not discovered -> nothing to do here */
        break;
      case CDDDRM_RESPONSE_TIMEOUT:
        /* ECU detection state: not available */
        lEcuState = CDDDRM_ECUD_SET_CONNECTION_NOT_AVAILABLE(lEcuState, Response->connectionId);
        break;
      case CDDDRM_RESPONSE_BUFFER_TOO_SMALL:
      case CDDDRM_RESPONSE_INVALID_NRC_LENGTH:
      case CDDDRM_RESPONSE_POSITIVE:
# if (CDDDRM_CFG_SUPPORT_RCRRP_PROCESSING == STD_ON)
      case CDDDRM_RESPONSE_RCRRP_LIMIT_REACHED:
# endif
      case CDDDRM_RESPONSE_WRONG_SERVICE:
      default:
        /* ECU detection state: available */
        lEcuState = CDDDRM_ECUD_SET_CONNECTION_AVAILABLE(lEcuState, Response->connectionId);
        break;
    }
    /* #30 Write result to detection list */
    CddDrm_Cfg_EcuDetectionData.EcuList[CDDDRM_ECUD_GET_CONNECTION_INDEX(Response->connectionId)] = lEcuState;
    CddDrm_Ecud_State = CDDDRM_ECUD_STATE_READY;
  }
}
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Ecud_SetupRequest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Ecud_SetupRequest(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_ConnectionTableIterType lConnectionId;
  CddDrm_ConnectionTableIterType lNextConnectionId;
  /* ----- Implementation ----------------------------------------------- */
  lNextConnectionId = CddDrm_Ecud_ConnectionIterationIndex;
  /* #10 Iterate through all configured connections, skip connections that are blocked by firewall */
  for (lConnectionId = CddDrm_Ecud_ConnectionIterationIndex; lConnectionId < CDDDRM_CFG_ECUD_INDEX_LIMIT; lConnectionId++)
  {
# if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
    /* Check if configured service is allowed to the connection */
    if (CddDrm_Cfg_CheckFirewallRule((CddDrm_ConnectionIdType)lConnectionId, CddDrm_Cfg_GetEcuDetectionRequestByte(0)) == TRUE)
# endif
    {
      CddDrm_ChannelIdType lFreeChannel;
      CddDrm_BufferStructType lBufferInfo;

      lBufferInfo.requestBufferDataPtr = (CddDrm_BufferDataPtrType)&CddDrm_Cfg_GetEcuDetectionRequestByte(0); /* PRQA S 0311 */ /* MD_CDDDRM_11.5 */
      lBufferInfo.responseBufferDataPtr = &CddDrm_Ecud_ResponseBuffer[0];
      lBufferInfo.responseBufferSize = CDDDRM_CFG_ECUD_RESPONSE_BUFFER_SIZE;
      /* #20 Get the channel exclusive reserved for ECU detection and initialize the channel parameters */
      lFreeChannel = CddDrm_Cm_ChannelGet((CddDrm_ConnectionIdType)lConnectionId, CDDDRM_CHANNEL_CLIENT_ECU_DETECTION);
      /* got a valid channel? */
      if (lFreeChannel < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
      {
        CddDrm_Ecud_State = CDDDRM_ECUD_STATE_BUSY;
        /* exclusive channel is free */
        CddDrm_Cm_ChannelInit(lFreeChannel,
                              lBufferInfo.requestBufferDataPtr[0],
                              (CddDrm_ResponseNotificationPtr)CddDrm_Ecud_ResponseNotification,
                              CDDDRM_CFG_ECUD_REQUEST_LENGTH,
                              FALSE,
                              &lBufferInfo);
        lNextConnectionId = lConnectionId + 1;
        break;
      }
    } /* else: nothing to do, service id is blocked by firewall, try next connection */
  } /* for-loop end */
  CddDrm_Ecud_ConnectionIterationIndex = lNextConnectionId;
}
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Ecud_StateTask()
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
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Ecud_StateTask(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Proceed if ECU Detection is ready */
  if (CddDrm_Ecud_State == CDDDRM_ECUD_STATE_READY)
  {
    /* #20 If the end of configured connection list reached teh ECU detection has finished */
    if (CddDrm_Ecud_ConnectionIterationIndex == CDDDRM_CFG_ECUD_INDEX_LIMIT)
    {
      /* #30 If ECU detection is finished write the detection list to NvRam */
# if (CDDDRM_CFG_SUPPORT_NVRAM == STD_ON)
      if (NvM_WriteBlock((NvM_BlockIdType)CDDDRM_CFG_NVM_BLOCK_ECU_DETECTION, (uint8*)&CddDrm_Cfg_EcuDetectionData) == E_OK) /* PRQA S 0310 */ /* MD_CDDDRM_11.4 */
      {
        CddDrm_Ecud_State = CDDDRM_ECUD_STATE_WRITE_NVM;
      }
      else
      { /* End ECU detection by notifing the application and setting the state to idle */
#  if (CDDDRM_CFG_SUPPORT_ECUD_FINISHED == STD_ON)
        CddDrm_Cfg_Ecud_Finished();
#  endif
        CddDrm_Ecud_State = CDDDRM_ECUD_STATE_IDLE;
      }
# else
#  if (CDDDRM_CFG_SUPPORT_ECUD_FINISHED == STD_ON)
      CddDrm_Cfg_Ecud_Finished();
#  endif
      CddDrm_Ecud_State = CDDDRM_ECUD_STATE_IDLE;
# endif
    }
    else
    {
      /* #40 Otherwise: Setup the next request */
      CddDrm_Ecud_SetupRequest();
    }
  }
  else
# if (CDDDRM_CFG_SUPPORT_ECUD_FINISHED == STD_ON)
  /* #50 Otherwise: If configured notify the application that the ECU detection has finished and set the ECU detection state to idle */
  if (CddDrm_Ecud_State == CDDDRM_ECUD_STATE_FINISH)
  {
    CddDrm_Cfg_Ecud_Finished();
    CddDrm_Ecud_State = CDDDRM_ECUD_STATE_IDLE;
  }
  else
# endif
  {
    /* nothing to do */
  }
}
#endif

#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
/**********************************************************************************************************************
 * CddDrm_Cfg_CheckFirewallRule()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(boolean, CDDDRM_CODE) CddDrm_Cfg_CheckFirewallRule(CddDrm_ConnectionIdType ConnectionId, uint8 ServiceId)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean lResult = TRUE;
  /* ----- Implementation ----------------------------------------------- */
  
# if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_ON)
  CDDDRM_DUMMY_STATEMENT(ConnectionId);                                                                          /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
# if (CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON)
#  if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
  /* #10 If a rule is assigned to the connection */
  if (CddDrm_Cfg_GetFirewallRule(ConnectionId) > 0)
#  endif
  {
    /* #20 if the requested service id is blocked return FALSE, otherwise TRUE */
    switch(ServiceId)
    {
#  if (CDDDRM_FIREWALL_SERVICE_10 == STD_ON)
      case CDDDRM_SERVICE_10:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_10) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_11 == STD_ON)
      case CDDDRM_SERVICE_11:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_11) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_14 == STD_ON)
      case CDDDRM_SERVICE_14:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_14) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_19 == STD_ON)
      case CDDDRM_SERVICE_19:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_19) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_22 == STD_ON)
      case CDDDRM_SERVICE_22:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_22) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_23 == STD_ON)
      case CDDDRM_SERVICE_23:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_23) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_24 == STD_ON)
      case CDDDRM_SERVICE_24:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_24) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_27 == STD_ON)
      case CDDDRM_SERVICE_27:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_27) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_28 == STD_ON)
      case CDDDRM_SERVICE_28:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_28) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_2A == STD_ON)
      case CDDDRM_SERVICE_2A:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_2A) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_2C == STD_ON)
      case CDDDRM_SERVICE_2C:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_2C) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_2E == STD_ON)
      case CDDDRM_SERVICE_2E:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_2E) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_2F == STD_ON)
      case CDDDRM_SERVICE_2F:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_2F) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_31 == STD_ON)
      case CDDDRM_SERVICE_31:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_31) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_34 == STD_ON)
      case CDDDRM_SERVICE_34:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_34) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_35 == STD_ON)
      case CDDDRM_SERVICE_35:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_35) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_36 == STD_ON)
      case CDDDRM_SERVICE_36:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_36) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_37 == STD_ON)
      case CDDDRM_SERVICE_37:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_37) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_38 == STD_ON)
      case CDDDRM_SERVICE_38:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_38) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_3D == STD_ON)
      case CDDDRM_SERVICE_3D:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_3D) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_3E == STD_ON)
      case CDDDRM_SERVICE_3E:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_3E) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_83 == STD_ON)
      case CDDDRM_SERVICE_83:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_83) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_84 == STD_ON)
      case CDDDRM_SERVICE_84:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_84) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_85 == STD_ON)
      case CDDDRM_SERVICE_85:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_85) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_86 == STD_ON)
      case CDDDRM_SERVICE_86:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_86) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
#  if (CDDDRM_FIREWALL_SERVICE_87 == STD_ON)
      case CDDDRM_SERVICE_87:
#   if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
        if ((CddDrm_Cfg_GetFirewallRule(ConnectionId) & CDDDRM_CFG_BITPOS_SERVICE_87) > 0)
#   endif
        {
          lResult = FALSE;
        }
        break;
#  endif
      default:
        break;
    } /* End switch */
  } /* End if */
#endif
# if (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON)
#  if (CDDDRM_CFG_ISCONST_FIREWALL_BITMASK == STD_OFF)
  /* #30 Otherwise: return the result of the user firewall check (if configured) */
  if (lResult == TRUE)
#  endif
  {
    if ((*(CddDrm_FirewallUserCallbackPtr)CddDrm_Cfg_FirewallUserCallback)(ConnectionId, ServiceId) != TRUE)
    {
      lResult = FALSE;
    } /* else not needed, value can be set before to FALSE and is initially set to TRUE */
  }
# endif
  return lResult;
} /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
#endif

/**********************************************************************************************************************
 * CddDrm_Cm_ChannelClose()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_ChannelClose(CddDrm_ChannelIdType ChannelId)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_RespInfoStructType lNotificationInfo;
  /* ----- Implementation ----------------------------------------------- */
  /* Prepare application notification data */
  lNotificationInfo.connectionId = CddDrm_GetConnectionId(ChannelId);
  lNotificationInfo.responseCode = CddDrm_GetChannelResponseCode(ChannelId);

  /* #10 Set the response length dependent on the response code */
  if (CddDrm_GetChannelResponseCode(ChannelId) == CDDDRM_RESPONSE_POSITIVE)
  {
    lNotificationInfo.responseLength = CddDrm_GetChannelMessageLength(ChannelId);
  }
  else
  {
    lNotificationInfo.responseLength = 0;
  }

  /* #20 Perform channel closing actions dependent on channel state */
  switch (CddDrm_GetChannelState(ChannelId))
  {
    case CDDDRM_CHANNEL_STATE_REQUEST_COPY:
      PduR_CddDrmRxIndication(CddDrm_GetConnectionReqPduId(ChannelId), E_NOT_OK);
      /* Note: Not needed in state CDDDRM_CHANNEL_STATE_REQUEST_START,
       * because PduR_DrmStartOfReception has not yet been called e.g. if an external tester has been detected
       */
      break;
    case CDDDRM_CHANNEL_STATE_RESPONSE_START:
    case CDDDRM_CHANNEL_STATE_RESPONSE_COPY:
    case CDDDRM_CHANNEL_STATE_RESPONSE_DONE:
      if (CddDrm_GetChannelCurrentMessageIndex(ChannelId) < CddDrm_GetChannelMessageLength(ChannelId))
      {
        PduR_CddDrmTxConfirmation(CddDrm_GetConnectionResPduId(ChannelId), E_NOT_OK);
      }
      break;
    default:
      /* nothing to do for INACTIVE | WAITDELAY states */
      break;
  }

  /* #30 Inform application or in case of ECU detection the DRM about finished request processing */
  CddDrm_ChannelInfo[ChannelId].notificationFunction(&lNotificationInfo);

#if (CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON)
  /* reload separation timer for this connection */
  CddDrm_SeparationTimer[lNotificationInfo.connectionId] = CddDrm_Cfg_GetConnectionSeparationTime(lNotificationInfo.connectionId);
#endif

  /* Note: No exclusive area needed because at CddDrm_Transmit no state change possible if channels inactive */
  /* #40 Close the channel and update active channel counter */
  CddDrm_SetChannelState(ChannelId, CDDDRM_CHANNEL_STATE_INACTIVE);
  CddDrm_ChannelAssign[lNotificationInfo.connectionId] = CDDDRM_CHANNEL_INFO_FREE;
  CddDrm_ActiveChannels = CddDrm_ActiveChannels - 1;
}

/**********************************************************************************************************************
 * CddDrm_Timer_Task()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_Task(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_ConnectionTableIterType lConnectionIndex;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate through all configured connections */
  for (lConnectionIndex = 0; lConnectionIndex < CDDDRM_CFG_CONNECTION_COUNT; lConnectionIndex++)
  {
#if (CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON)
    CddDrm_Timer_ConnectionDelay(lConnectionIndex);
#endif
    CddDrm_Timer_P2(lConnectionIndex);
  } /* End Loop */

#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
  CddDrm_Timer_GlobalDelay();
#endif
}

#if (CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Timer_ConnectionDelay()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_ConnectionDelay(CddDrm_ConnectionTableIterType ConnectionId)
{
  CddDrm_ChannelIdType lAssignedChannel = CddDrm_ChannelAssign[ConnectionId];

  /* Process connection specific delay timer */
  if (CddDrm_SeparationTimer[ConnectionId] > 0)
  {
    CddDrm_SeparationTimer[ConnectionId]--;
  }

  /* Update channel state of connections that wait for processing */
  if (lAssignedChannel == CDDDRM_CHANNEL_INFO_FREE)
  {
    /* no channel assigned to this connection */
  }
  else
  {
    if (CddDrm_GetChannelState(lAssignedChannel) == CDDDRM_CHANNEL_STATE_WAIT_DELAY)
    {
      if (CddDrm_SeparationTimer[ConnectionId] == 0)
      {
        /* Set the next state, so connection can send the request */
        CddDrm_SetChannelState(lAssignedChannel, CDDDRM_CHANNEL_STATE_REQUEST_START);
      }
    }
  }
}
#endif

/**********************************************************************************************************************
 * CddDrm_Timer_P2()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_P2(CddDrm_ConnectionTableIterType ConnectionId)
{
  CddDrm_ChannelIdType lAssignedChannel = CddDrm_ChannelAssign[ConnectionId];

  /* Process channel specific P2 and P2* timeout */
  if (lAssignedChannel < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
  {
    /* Check channel for open connection and channel state, to decrease timeout counter */
    if (CddDrm_GetChannelState(lAssignedChannel) == CDDDRM_CHANNEL_STATE_WAIT_RESPONSE)
    {
      /* no need to check for timeout > 0 as of in state wait response it is always greater than 0 */
      /* Timeout not elapsed */
      CddDrm_DecChannelP2Timer(lAssignedChannel);
      /* If timeout has exceeded: Close channel */
      if (CddDrm_GetChannelP2Timer(lAssignedChannel) == 0)
      {
        /* Timeout elapsed, notify application and close connection channel */
        if ( (CddDrm_GetChannelSPRMIB(lAssignedChannel) == TRUE)
          && (CddDrm_GetChannelRCRRP(lAssignedChannel) == CddDrm_Cfg_GetConnectionRCRRP(CddDrm_GetConnectionId(lAssignedChannel))) )
        { /* If SPRMIB bit is set no response is expected within P2 time. As of RCRRP counter has still is initial value the P2 timer has exceeded and not the 2 extended timer  */
          /* No need to set the positive response code as it was set during channel initialization */
        }
        else
        {
          CddDrm_SetChannelResponseCode(lAssignedChannel, CDDDRM_RESPONSE_TIMEOUT);
        }
        CddDrm_Cm_ChannelClose(lAssignedChannel);
      }
    }
  }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Timer_GlobalDelay()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Timer_GlobalDelay(void)
{
  CddDrm_ConnectionTableIterType lConnectionIndex;

  /* Delay be reloaded when a request was sent */
  if (CddDrm_RequestDelayTimer > 0)
  {
    /* Decrease global delay timer */
    CddDrm_RequestDelayTimer--;
    if (CddDrm_RequestDelayTimer == 0)
    {
      for(lConnectionIndex = 0; lConnectionIndex <= CDDDRM_CFG_CONNECTION_COUNT; lConnectionIndex++)
      {
        if (CddDrm_ChannelAssign[CddDrm_NextConnection] != CDDDRM_CHANNEL_INFO_FREE)
        {
          if (CddDrm_GetChannelState(CddDrm_ChannelAssign[CddDrm_NextConnection]) == CDDDRM_CHANNEL_STATE_WAIT_DELAY)
          {
            /* Update channel state of connections that wait for processing */
            CddDrm_SetChannelState(CddDrm_ChannelAssign[CddDrm_NextConnection], CDDDRM_CHANNEL_STATE_REQUEST_START);
            break;
          }
        }
        CddDrm_NextConnection = (CddDrm_NextConnection + 1) % CDDDRM_CFG_CONNECTION_COUNT;
      }
    }
  }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

/**********************************************************************************************************************
 * CddDrm_Cm_StateTask()
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
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_StateTask(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_ChannelIdType lChannelId;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate through all configured channels */
  for(lChannelId = 0; lChannelId < CDDDRM_CFG_CHANNEL_COUNT; lChannelId++)
  {
    /* #20 Process request dependent on current channel state */
    switch (CddDrm_GetChannelState(lChannelId))
    {
      case CDDDRM_CHANNEL_STATE_REQUEST_START:
      {
        CddDrm_Cm_RequestStart(lChannelId);
        break;
      }
      case CDDDRM_CHANNEL_STATE_REQUEST_COPY:
      {
        CddDrm_Cm_RequestCopy(lChannelId);
        break;
      }
      case CDDDRM_CHANNEL_STATE_RESPONSE_START:
      {
        CddDrm_Cm_ResponseStart(lChannelId);
        break;
      }
      case CDDDRM_CHANNEL_STATE_RESPONSE_COPY:
      {
        CddDrm_Cm_ResponseCopy(lChannelId);
        break;
      }
      case CDDDRM_CHANNEL_STATE_RESPONSE_DONE:
      {
        /* start of reception not called and no data copied, so it is not necessary to call TxConfirmation */
        if (CddDrm_GetChannelCurrentMessageIndex(lChannelId) == CddDrm_GetChannelMessageLength(lChannelId))
        {
          PduR_CddDrmTxConfirmation(CddDrm_GetConnectionResPduId(lChannelId), E_OK);
        }
        else
        {
          /* receiving data was aborted, so only the copied data can be used
            * (prevent that application interpret wrong old/data)
            */
          CddDrm_SetChannelMessageLength(lChannelId, CddDrm_GetChannelCurrentMessageIndex(lChannelId));
          PduR_CddDrmTxConfirmation(CddDrm_GetConnectionResPduId(lChannelId), E_NOT_OK);
        }
        CddDrm_Cm_ChannelClose(lChannelId);
        break;
      }
      default:
        /* nothing to do for this channel */
        break;
    }
  }
}

/**********************************************************************************************************************
 * CddDrm_Cm_ResponseStart()
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
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_ResponseStart(CddDrm_ChannelIdType ChannelId)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType lPduInfo;
  PduLengthType lBufferSize = 0;
  BufReq_ReturnType lBufferStatus;
  uint8 lResponseBuffer[3] = { 0, 0, 0 };
  CddDrm_ChannelStateType lChannelState = CDDDRM_CHANNEL_STATE_RESPONSE_DONE;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if response data fits into buffer */
  if (CddDrm_GetChannelRespBufferInfoSize(ChannelId) >= CddDrm_GetChannelMessageLength(ChannelId))
  {
    lPduInfo.SduDataPtr = &lResponseBuffer[0];
    /*
     *  Diag responses may have more than 3 byte.
     *  At max 3 byte shall be copied to identify type of response.
     */
    if(CddDrm_GetChannelMessageLength(ChannelId) > 3)
    {
      lPduInfo.SduLength = 3; /* Channel buffer is configured for 3 byte */
    }
    else
    {
      lPduInfo.SduLength = CddDrm_GetChannelMessageLength(ChannelId);
    }
    /* #20 Copy first three bytes of response to detect the response type (NRC/PRC) */
    lBufferStatus = PduR_CddDrmCopyTxData(CddDrm_GetConnectionResPduId(ChannelId),
                                          &lPduInfo,
                                          NULL_PTR,
                                          &lBufferSize);
    if (lBufferStatus == BUFREQ_OK)
    {
#if (CDDDRM_SUPPORT_CHANNEL_PDUR_RETRY == STD_ON)
      /* reload PduR busy counter */
      CddDrm_SetChannelPduRBusyCounter(ChannelId, CDDDRM_CHANNEL_PDUR_RETRY_VALUE);
#endif
      CddDrm_AddChannelCurrentMessageIndex(ChannelId, lPduInfo.SduLength);
      /* #30 In case of a negative respons */
      if (lResponseBuffer[0] == 0x7fU)
      {
        /* #40 Verify that response length has three bytes */
        if (CddDrm_GetChannelMessageLength(ChannelId) == 3)
        {
          /* #50 Check if service id is the same as send */
          if (lResponseBuffer[1] == CddDrm_GetChannelServiceId(ChannelId))
          {
#if (CDDDRM_CFG_SUPPORT_RCRRP_PROCESSING == STD_ON)
            /* #60 If RCRRP: Handle P2/P2* and RCRRP counter */
            if (lResponseBuffer[2] == 0x78U)
            {
                lChannelState = CddDrm_Cm_ProcessRCRRP(ChannelId);
            }
            else
#endif
            {
              /* channel state is initial RESPONSE_COMPLETE, so it is not needed to set here */
              CddDrm_SetChannelResponseCode(ChannelId, lResponseBuffer[2]);
            }
          }
          else
          {
            /* #70 Otherwise: Notify application for incorrect service id received */
            CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_WRONG_SERVICE);
          }
        }
        else
        {
          /* #80 Otherwise: Notify application for incorrect NRC length */
          CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_INVALID_NRC_LENGTH);
        }
      }
      /* #90  In case of a postive response, verify that received service id equals requested SID + 0x40 */
      else if (lResponseBuffer[0] == (CddDrm_GetChannelServiceId(ChannelId) | CDDDRM_RESPONSE_CODE_VALIDATION))
      {
        /* Intentionally copy the 3 byte local buffer to application buffer */
        CddDrm_SetChannelRespBufferInfoPtrVal(ChannelId, 0, lResponseBuffer[0]);
        CddDrm_SetChannelRespBufferInfoPtrVal(ChannelId, 1, lResponseBuffer[1]);
        CddDrm_SetChannelRespBufferInfoPtrVal(ChannelId, 2, lResponseBuffer[2]);

        CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_POSITIVE);

        if (CddDrm_GetChannelMessageLength(ChannelId) > lPduInfo.SduLength)
        {
          /* More response data left */
          CddDrm_SetChannelCurrentMessageIndex(ChannelId, lPduInfo.SduLength);
          lChannelState = CDDDRM_CHANNEL_STATE_RESPONSE_COPY;
        } /* else: nothing to be done, RESPONSE_DONE state is already set above, no more data available to copy */
      }
      else
      {
        /* channel state is initial RESPONSE_COMPLETE, so its not needed to set here */
        CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_WRONG_SERVICE);
      }
    } /* end if: lBufferStatus == BUFREQ_OK */
    else
    {
      /* channel state is initial RESPONSE_COMPLETE, so its not needed to set here */
      CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_PDUR_RX_ERROR);
    }
  } /* end if: compare response length and response buffer size */
  else
  {
    /* #100 Otherwise: Notify application for insufficient buffer size */
    CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_BUFFER_TOO_SMALL);
  }

  CddDrm_SetChannelState(ChannelId, lChannelState);
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * CddDrm_Cm_ResponseCopy()
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
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_ResponseCopy(CddDrm_ChannelIdType ChannelId)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType lPduInfo;
  PduLengthType lBufferSize = 0;
  BufReq_ReturnType lBufferStatus;
  CddDrm_ChannelStateType lChannelState = CDDDRM_CHANNEL_STATE_RESPONSE_DONE;

  /* ----- Implementation ----------------------------------------------- */
  lPduInfo.SduDataPtr = &CddDrm_GetChannelRespBufferInfoPtrVal(ChannelId, CddDrm_GetChannelCurrentMessageIndex(ChannelId));
  lPduInfo.SduLength = 0;
  /* #10 Get remaining length of response data */
  lBufferStatus = PduR_CddDrmCopyTxData(CddDrm_GetConnectionResPduId(ChannelId),
                                        &lPduInfo,
                                        NULL_PTR,
                                        &lBufferSize);
  /* #20 Check if PduR has returned a valid buffer size */
  if (lBufferStatus == BUFREQ_OK)
  {
    if (lBufferSize > 0)
    {
      /* fetch data from PduR */
      if (lBufferSize > (CddDrm_GetChannelMessageLength(ChannelId) - CddDrm_GetChannelCurrentMessageIndex(ChannelId)))
      {
        lPduInfo.SduLength = CddDrm_GetChannelMessageLength(ChannelId) - CddDrm_GetChannelCurrentMessageIndex(ChannelId);
      }
      else
      {
        lPduInfo.SduLength = lBufferSize;
      }
      /* #30 Copy data from PduR into application buffer */
      lBufferStatus = PduR_CddDrmCopyTxData(CddDrm_GetConnectionResPduId(ChannelId),
                                            &lPduInfo,
                                            NULL_PTR,
                                            &lBufferSize);
      /* #40 If PduR has copied the data */
      if (lBufferStatus == BUFREQ_OK)
      {
#if (CDDDRM_SUPPORT_CHANNEL_PDUR_RETRY == STD_ON)
        /* data in PduR available */
        CddDrm_SetChannelPduRBusyCounter(ChannelId, CDDDRM_CHANNEL_PDUR_RETRY_VALUE);
#endif
        CddDrm_AddChannelCurrentMessageIndex(ChannelId, lPduInfo.SduLength);
        /* #50 If more data available to copy, stay in the current state */
        if (CddDrm_GetChannelCurrentMessageIndex(ChannelId) < CddDrm_GetChannelMessageLength(ChannelId))
        {
          /* response code already set to positive response, stay in current state to copy  */
          lChannelState = CddDrm_GetChannelState(ChannelId);
        }
      }
      /* #60 Else if PduR has not received enough data yet, stay in the current state */
      else if(lBufferStatus == BUFREQ_E_BUSY)
      {
#if (CDDDRM_SUPPORT_CHANNEL_PDUR_RETRY == STD_ON)
        CddDrm_DecChannelPduRBusyCounter(ChannelId);
        if (CddDrm_GetChannelPduRBusyCounter(ChannelId) == 0)
        {
          CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_PDUR_RX_ERROR);
        }
        else
#endif
        {
          lChannelState = CddDrm_GetChannelState(ChannelId);
        }
      }
      else
      {
        CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_PDUR_RX_ERROR);
      }
    } /* else no data available, retry at next task */
    else
    {
#if (CDDDRM_SUPPORT_CHANNEL_PDUR_RETRY == STD_ON)
      CddDrm_DecChannelPduRBusyCounter(ChannelId);
      if (CddDrm_GetChannelPduRBusyCounter(ChannelId) == 0)
      {
        CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_PDUR_RX_ERROR);
      }
      else
#endif
      {
        lChannelState = CddDrm_GetChannelState(ChannelId);
      }
    }
  }
  /* #70 Else if PduR has not received enough data yet, stay in the current state */
  else if(lBufferStatus == BUFREQ_E_BUSY)
  {
#if (CDDDRM_SUPPORT_CHANNEL_PDUR_RETRY == STD_ON)
    CddDrm_DecChannelPduRBusyCounter(ChannelId);
    if (CddDrm_GetChannelPduRBusyCounter(ChannelId) == 0)
    {
      CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_PDUR_RX_ERROR);
    }
    else
#endif
    {
      lChannelState = CddDrm_GetChannelState(ChannelId);
    }
  }
  /* #80 Otherwise: Close channel, buffer error occured */
  else
  {
    CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_PDUR_RX_ERROR);
  }

  CddDrm_SetChannelState(ChannelId, lChannelState);
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

#if (CDDDRM_CFG_SUPPORT_RCRRP_PROCESSING == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Cm_ProcessRCRRP()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(CddDrm_ChannelStateType, CDDDRM_CODE) CddDrm_Cm_ProcessRCRRP(CddDrm_ChannelIdType ChannelId)
{
  CddDrm_ChannelStateType lResult = CDDDRM_CHANNEL_STATE_RESPONSE_DONE;
  /* #10 If RCRRP limit not reached: decrement counter, re-set channel state to CDDDRM_CHANNEL_STATE_WAIT_RESPONSE */
  if (CddDrm_ChannelInfo[ChannelId].rcrrpCounter > 0)
  {
    /* Reload connection timer and count the received RCRRP message */
    CddDrm_SetChannelP2Timer(ChannelId, CddDrm_Cfg_GetConnectionP2ExtendedTime(CddDrm_ChannelInfo[ChannelId].connectionId));
    CddDrm_DecChannelRCRRP(ChannelId);
    PduR_CddDrmTxConfirmation(CddDrm_GetConnectionResPduId(ChannelId), E_OK);
    /* back to start of buffer */
    CddDrm_SetChannelCurrentMessageIndex(ChannelId, 0);
    lResult = CDDDRM_CHANNEL_STATE_WAIT_RESPONSE;
  }
  /* #20 Otherwise: Limit reached, set limit reached response code for application and channel state CDDDRM_CHANNEL_STATE_RESPONSE_DONE */
  else
  {
    CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_RCRRP_LIMIT_REACHED);
  }
  /* #30 Return the next channel state */
  return lResult;
}
#endif
/**********************************************************************************************************************
 * CddDrm_Cm_RequestStart()
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
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_RequestStart(CddDrm_ChannelIdType ChannelId)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType lPduRBufferStatus;
  PduLengthType lPduRBufferSize;
  PduInfoType lPduInfo;

  /* ----- Implementation ----------------------------------------------- */
#if (CDDDRM_CFG_SUPPORT_DEDICATED_APIS == STD_ON)
  /* #10 Choose if application or internal buffer must be used. Internal buffer is only used by dedicated service APIs */
  if (CddDrm_GetProtocolBufferActive(ChannelId) == TRUE)
  {
    lPduInfo.SduDataPtr = &CddDrm_ChannelInfo[ChannelId].protocolData[0];
  }
  else
#endif
  {
    lPduInfo.SduDataPtr = &CddDrm_GetChannelReqBufferInfoPtrVal(ChannelId, 0);
  }
  /* First time for this channel and no data has been copied */
  lPduInfo.SduLength = CddDrm_GetChannelMessageLength(ChannelId);
  /* #20 Setup PduR by requesting a buffer */
  lPduRBufferStatus = PduR_CddDrmStartOfReception(CddDrm_Cfg_GetConnectionReqPduId(CddDrm_ChannelInfo[ChannelId].connectionId),
                                                  NULL_PTR,
                                                  lPduInfo.SduLength,
                                                  &lPduRBufferSize);
  
  /* #30 If PduR buffer size is large enougth and return status is ok */
  if ((lPduRBufferStatus == BUFREQ_OK) && (lPduRBufferSize > 0))
  {
    /* #40 Select application or internal buffer: get the number of bytes that can be copied into the buffer provided from PduR */
#if (CDDDRM_CFG_SUPPORT_DEDICATED_APIS == STD_ON)
    if (CddDrm_GetProtocolBufferActive(ChannelId) == TRUE)
    {
      /* Dedicated API */
      if (CddDrm_GetChannelProtocolLength(ChannelId) > lPduRBufferSize)
      {
        lPduInfo.SduLength = lPduRBufferSize;
      }
      else
      {
        lPduInfo.SduLength = CddDrm_GetChannelProtocolLength(ChannelId);
      }
    }
    else
#endif
    {
      /* application buffer is used (generic API) */
      if (CddDrm_GetChannelMessageLength(ChannelId) > lPduRBufferSize)
      {
        lPduInfo.SduLength = lPduRBufferSize;
      }
      else
      {
        lPduInfo.SduLength = CddDrm_GetChannelMessageLength(ChannelId);
      }
    }
    /* #50 Copy data to PduR */
    lPduRBufferStatus = PduR_CddDrmCopyRxData(CddDrm_Cfg_GetConnectionReqPduId(CddDrm_ChannelInfo[ChannelId].connectionId), 
                                              &lPduInfo,
                                              &lPduRBufferSize);
    /* #60 If data copy was successful: Switch to next channel state CDDDRM_CHANNEL_STATE_REQUEST_COPY if more 
     *     data needs to be copied or CDDDRM_CHANNEL_STATE_WAIT_RESPONSE if all data was copied to PduR */
    if (lPduRBufferStatus == BUFREQ_OK)
    {
      CddDrm_SetChannelCurrentMessageIndex(ChannelId, lPduInfo.SduLength);
      if (CddDrm_GetChannelMessageLength(ChannelId) > CddDrm_GetChannelCurrentMessageIndex(ChannelId))
      {
        CddDrm_SetChannelState(ChannelId, CDDDRM_CHANNEL_STATE_REQUEST_COPY);
      }
      else
      {
        /* request completely sent, channel has to wait for response */
        CddDrm_SetChannelState(ChannelId, CDDDRM_CHANNEL_STATE_WAIT_RESPONSE);
        CddDrm_SetChannelMessageLength(ChannelId, 0); /* zero the message length so we can determine later if a response was received or not */
        PduR_CddDrmRxIndication(CddDrm_Cfg_GetConnectionReqPduId(CddDrm_ChannelInfo[ChannelId].connectionId), E_OK);
#if (CDDDRM_CFG_SUPPORT_TXCONFIRMATION == STD_ON)
        /* notify the application, that DRM sent the request to PduR */
# if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
        if (ChannelId == CDDDRM_CHANNEL_ECU_DETECTION_INDEX)
        {
          /* do not call TxConfirmation, if Ecu detection request is in progress */
        }
        else
# endif
        {
          (*(CddDrm_TxConfigurationPtr)CddDrm_Cfg_TxConfirmation)(CddDrm_ChannelInfo[ChannelId].connectionId);
        }

#endif
#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
        CddDrm_RequestDelayTimer = CDDDRM_TIMER_REQUEST_DELAY;
        /* Prevent that other channels are blocked */
        CddDrm_NextConnection = (CddDrm_NextConnection + 1) % CDDDRM_CFG_CONNECTION_COUNT;
#endif
      }
    }
    /* #70 Otherwise: Close channel and set state to CDDDRM_CHANNEL_STATE_REQUEST_COPY. Here RxIndication will be called. */
    else
    {
      /* Channel will be closed and RxIndication called therefore set state to CDDDRM_CHANNEL_STATE_REQUEST_COPY */
      CddDrm_SetChannelState(ChannelId, CDDDRM_CHANNEL_STATE_REQUEST_COPY);
      CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_PDUR_TX_ERROR);
      CddDrm_Cm_ChannelClose(ChannelId);
    }
  }
  /* Note: BUFREQ_E_BUSY is not supported for StartOfReception and CopyRxData will not return BUFREQ_E_BUSY if StartOfReception was successful --> will be handled like BUFREQ_E_NOT_OK */
  /* #80 Otherwise: Close channel and set state to CDDDRM_CHANNEL_STATE_REQUEST_START. Here RxIndication is not called which is intended. */
  else
  {
    /* Channel will be closed and RxIndication call is not necessary */
    CddDrm_SetChannelState(ChannelId, CDDDRM_CHANNEL_STATE_REQUEST_START);
    CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_PDUR_TX_ERROR);
    CddDrm_Cm_ChannelClose(ChannelId);
  }
}

/**********************************************************************************************************************
 * CddDrm_Cm_RequestCopy()
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
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_RequestCopy(CddDrm_ChannelIdType ChannelId)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType lPduRBufferStatus;
  PduLengthType lPduRBufferSize;
  PduInfoType lPduInfo;

  /* ----- Implementation ----------------------------------------------- */
#if (CDDDRM_CFG_SUPPORT_DEDICATED_APIS == STD_ON)
  /* #10 Choose if application or internal request buffer must be used */
  if (CddDrm_GetProtocolBufferActive(ChannelId) == TRUE)
  {
    if (CddDrm_GetChannelCurrentMessageIndex(ChannelId) < CddDrm_ChannelInfo[ChannelId].protocolDataLength)
    {
      lPduInfo.SduDataPtr = &CddDrm_ChannelInfo[ChannelId].protocolData[CddDrm_GetChannelCurrentMessageIndex(ChannelId)];
    }
    else
    {
      lPduInfo.SduDataPtr = &CddDrm_GetChannelReqBufferInfoPtrVal(ChannelId,
                                                                  CddDrm_GetChannelCurrentMessageIndex(ChannelId)
                                                                  - CddDrm_ChannelInfo[ChannelId].protocolDataLength);
    }
  }
  else
#endif
  {
    lPduInfo.SduDataPtr = &CddDrm_GetChannelReqBufferInfoPtrVal(ChannelId,
                                                                CddDrm_GetChannelCurrentMessageIndex(ChannelId));
  }
  /* Request was accepted by PduR and data has already been copied, but there are still more available */
  /* Get available PduR buffersize */
  lPduInfo.SduLength = 0;
  /* #20 Copy 0 byte data to PduR to get the remaining size of buffer provided by PduR */
  lPduRBufferStatus = PduR_CddDrmCopyRxData(CddDrm_Cfg_GetConnectionReqPduId(CddDrm_ChannelInfo[ChannelId].connectionId),
                                            &lPduInfo,
                                            &lPduRBufferSize);
  /* #30 If PduR buffer size is greater than zero and status is ok */
  if ( (lPduRBufferStatus == BUFREQ_OK) && (lPduRBufferSize > 0) )
  {
    /* #40 Get the number of bytes that can be copied to PduR */
    if ((CddDrm_GetChannelMessageLength(ChannelId) - CddDrm_GetChannelCurrentMessageIndex(ChannelId)) > lPduRBufferSize)
    {
      /* PduR buffer too small for all remaining data */
      lPduInfo.SduLength = lPduRBufferSize;
    }
    else
    {
      /* all remaining data can be copied to PduR */
      lPduInfo.SduLength = CddDrm_GetChannelMessageLength(ChannelId) - CddDrm_GetChannelCurrentMessageIndex(ChannelId);
    }
    lPduRBufferStatus = PduR_CddDrmCopyRxData(CddDrm_Cfg_GetConnectionReqPduId(CddDrm_ChannelInfo[ChannelId].connectionId), &lPduInfo, &lPduRBufferSize);
  } /* else invalid buffer or still no buffer available */

  /* #50  If returned buffer status is ok */
  if (lPduRBufferStatus == BUFREQ_OK)
  {
#if (CDDDRM_SUPPORT_CHANNEL_PDUR_RETRY == STD_ON)
    /* Reset Channel PduR Busy Counter */
    CddDrm_SetChannelPduRBusyCounter(ChannelId, CDDDRM_CHANNEL_PDUR_RETRY_VALUE);
#endif
    CddDrm_AddChannelCurrentMessageIndex(ChannelId, lPduInfo.SduLength);
    /* Check if request has completely been copied to PduR */
    /* #60 If more data to copy available: stay in current channel state */
    if (CddDrm_GetChannelMessageLength(ChannelId) > CddDrm_GetChannelCurrentMessageIndex(ChannelId))
    {
      CddDrm_SetChannelState(ChannelId, CddDrm_GetChannelState(ChannelId));
    }
    /* #70 Otherwise: No remaining data to copy. Switch to next channel state CDDDRM_CHANNEL_STATE_WAIT_RESPONSE. Notify application about successful send request */
    else
    {
      /* request completely sent, channel has to wait for response */
      CddDrm_SetChannelState(ChannelId, CDDDRM_CHANNEL_STATE_WAIT_RESPONSE);
      CddDrm_SetChannelMessageLength(ChannelId, 0); /* zero the message length so we can determine later if a response was received or not */
      PduR_CddDrmRxIndication(CddDrm_Cfg_GetConnectionReqPduId(CddDrm_ChannelInfo[ChannelId].connectionId), E_OK);
#if (CDDDRM_CFG_SUPPORT_TXCONFIRMATION == STD_ON)
      /* notify that DRM sent the request to PduR */
      (*(CddDrm_TxConfigurationPtr)CddDrm_Cfg_TxConfirmation)(CddDrm_ChannelInfo[ChannelId].connectionId);
#endif
#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
      CddDrm_RequestDelayTimer = CDDDRM_TIMER_REQUEST_DELAY;
      /* Prevent that other channels are blocked */
      CddDrm_NextConnection = (CddDrm_NextConnection + 1) % CDDDRM_CFG_CONNECTION_COUNT;
#endif
    }
  }
  /* #80 Else if PduR buffer is not ready to receive more data and PduR busy-retry-count exceeds -> set channel state CDDDRM_CHANNEL_STATE_REQUEST_COPY and close channel */
  else if (lPduRBufferStatus == BUFREQ_E_BUSY)
  {
#if (CDDDRM_SUPPORT_CHANNEL_PDUR_RETRY == STD_ON)
    CddDrm_DecChannelPduRBusyCounter(ChannelId);
    if (CddDrm_GetChannelPduRBusyCounter(ChannelId) == 0)
    {
      CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_PDUR_TX_ERROR);
      CddDrm_SetChannelState(ChannelId, CDDDRM_CHANNEL_STATE_REQUEST_COPY);
      CddDrm_Cm_ChannelClose(ChannelId);
    }
    else
#endif
    {
      CddDrm_SetChannelState(ChannelId, CddDrm_GetChannelState(ChannelId));
    }
  }
  /* #90 Otherwise: An unknown PduR buffer error has occured, notify application about canceled request processing */
  else
  {
    /* error occured, handling in REQUEST_DONE state */
    /* Channel be closed and PduR_DrmRxIndication called in REQUEST_COPY state */
    CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_PDUR_TX_ERROR);
    CddDrm_Cm_ChannelClose(ChannelId);
  }
}

/**********************************************************************************************************************
 * CddDrm_Cm_ChannelGet()
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
CDDDRM_LOCAL FUNC(CddDrm_ChannelIdType, CDDDRM_CODE) CddDrm_Cm_ChannelGet(CddDrm_ConnectionIdType ConnectionId,
                                                                          uint8 ChannelClient)                                   /* PRQA S 3206 */ /* MD_CDDDRM_3206 */
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_ChannelIdType lResult = CDDDRM_CHANNEL_INFO_NOT_FREE;
  CddDrm_ChannelIdType lChannelId;
  /* ----- Implementation ----------------------------------------------- */

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_OFF)
  CDDDRM_DUMMY_STATEMENT(ChannelClient);                                                                                         /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  
  /* Prevent that more applications allocate the same channel */
/* >>>> -------------------------------- Enter Critical Section: ChannelManager */
  CddDrm_EnterCritical_ChannelManager();
  /* #10 If connection is not assigned to channel */
  if (CddDrm_ChannelAssign[ConnectionId] == CDDDRM_CHANNEL_INFO_FREE)
  {
#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
    /* #20 If ECU detection enabled and requesting the channel, then assign the connection to the reserved ECU 
     *     detection channel */
    if (ChannelClient == CDDDRM_CHANNEL_CLIENT_ECU_DETECTION)
    {
      /* not necessary to check if channel is in use as of ECU-Detection state machine prevents from 
       * a parallel running ECU detection */
      lChannelId = CDDDRM_CHANNEL_ECU_DETECTION_INDEX;
      
      /* allocate the channel */
      CddDrm_Cm_ChannelAllocate(lChannelId, ConnectionId);
      lResult = lChannelId;
    } 
    else
#endif
    { /* #30 Otherwise: Application needs a channel, therefore iterate through all channels and allocate the first unassigned */
      for (lChannelId = CDDDRM_CHANNEL_APPLICATION_INDEX; lChannelId < CDDDRM_CFG_CHANNEL_COUNT; lChannelId++)
      {
        if (CddDrm_GetChannelState(lChannelId) == CDDDRM_CHANNEL_STATE_INACTIVE)
        { 
          lResult = lChannelId;
          CddDrm_Cm_ChannelAllocate(lChannelId, ConnectionId);
          /* inactive channel found, leave loop */
          break;
        } /* else: nothing to do, current channel is not inactive */
      } /* end for-loop */
    } /* end if-GetEcuDetectionChannel */
  } /* #40 Otherwise: return that no channel is available */
/* >>>> -------------------------------- Leave Critical Section: ChannelManager */
  CddDrm_LeaveCritical_ChannelManager();
  return lResult;
}

/**********************************************************************************************************************
 * CddDrm_Cm_ChannelAllocate()
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
 */
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_Cm_ChannelAllocate(CddDrm_ChannelIdType ChannelId,
                                                                      CddDrm_ConnectionIdType ConnectionId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Allocate the free channel */
  CddDrm_ChannelInfo[ChannelId].connectionId = ConnectionId;
#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
  CddDrm_ChannelInfo[ChannelId].channelState = CDDDRM_CHANNEL_STATE_WAIT_DELAY;
  if (CddDrm_RequestDelayTimer == 0)
  {
    CddDrm_RequestDelayTimer = 1;
  }
#elif (CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON)
  /* set channel state depend on seperation time */
  if (CddDrm_Cfg_GetConnectionSeparationTime(ConnectionId) == 0)
  {
    CddDrm_ChannelInfo[ChannelId].channelState = CDDDRM_CHANNEL_STATE_REQUEST_START;
  }
  else
  {
    CddDrm_ChannelInfo[ChannelId].channelState = CDDDRM_CHANNEL_STATE_WAIT_DELAY;
  }
#else
  CddDrm_ChannelInfo[ChannelId].channelState = CDDDRM_CHANNEL_STATE_REQUEST_START;
#endif

  CddDrm_ChannelAssign[ConnectionId] = ChannelId;
  CddDrm_ActiveChannels = CddDrm_ActiveChannels + 1;
}

/**********************************************************************************************************************
 * CddDrm_Cm_ChannelInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_ChannelInit(CddDrm_ChannelIdType ChannelId,
                                                           uint8 ServiceId,
                                                           CddDrm_ResponseNotificationPtr Callback,
                                                           CddDrm_LengthType RequestLength,
                                                           boolean SPRMIB,
                                                           P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_CONST) BufferInfo)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Set channel to initial values */
  CddDrm_SetChannelServiceId(ChannelId, ServiceId);
  CddDrm_SetChannelP2Timer(ChannelId, CddDrm_Cfg_GetConnectionP2Time(CddDrm_ChannelInfo[ChannelId].connectionId));
  CddDrm_SetChannelRCRRP(ChannelId, CddDrm_Cfg_GetConnectionRCRRP(CddDrm_ChannelInfo[ChannelId].connectionId));
  CddDrm_SetChannelSPRMIB(ChannelId, SPRMIB);

  /* request buffer */
  CddDrm_SetChannelReqBufferInfoPtr(ChannelId, &BufferInfo->requestBufferDataPtr[0]);
  /* response buffer */
  CddDrm_SetChannelRespBufferInfoPtr(ChannelId, &BufferInfo->responseBufferDataPtr[0]);
  CddDrm_SetChannelRespBufferInfoSize(ChannelId, BufferInfo->responseBufferSize);

  CddDrm_SetChannelMessageLength(ChannelId, RequestLength);
  CddDrm_SetChannelCurrentMessageIndex(ChannelId, 0);
  CddDrm_SetChannelResponseCode(ChannelId, CDDDRM_RESPONSE_POSITIVE);
  CddDrm_SetChannelPduRBusyCounter(ChannelId, CDDDRM_CHANNEL_PDUR_RETRY_VALUE);
  CddDrm_ResetProtocolBufferActive(ChannelId);

  CddDrm_ChannelInfo[ChannelId].notificationFunction = Callback;
} /* PRQA S 6060 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
 * CddDrm_Cm_ChannelCloseAll()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Cm_ChannelCloseAll(CddDrm_ResponseCodeType ResponseCode)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If at least one channel active */
  if (CddDrm_ActiveChannels > 0)
  {
    CddDrm_ConnectionTableIterType lConnectionIndex;
    /* #20 Close all active channels by iterating over all connections. As of the application needs to be notified 
     *     with the reason of the forced channel closing the response code is also set here. */
    for (lConnectionIndex = 0; lConnectionIndex < CDDDRM_CFG_CONNECTION_COUNT; lConnectionIndex++)
    {
      if (CddDrm_ChannelAssign[lConnectionIndex] < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
      {
        CddDrm_ChannelIdType lChannelId = CddDrm_ChannelAssign[lConnectionIndex];

        CddDrm_SetChannelResponseCode(lChannelId, ResponseCode);
        CddDrm_Cm_ChannelClose(lChannelId);
      }
    }
  }
}

/**********************************************************************************************************************
 * CddDrm_Queue_ExternalTester()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Queue_ExternalTester(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If external tester detected queue reports that an external tester was detected */
  if (CddDrm_Queue_TesterConnected == CDDDRM_QUEUE_EXTERNAL_TESTER_CONNECTED)
  {
    /* #20 Set DRM state to external tester mode, Stop ECU Detection, cancel all processing requests */
    /* first change state, so application is not allowed to send request anymore */
    CddDrm_State = CDDDRM_STATE_TESTER_DETECTED;
#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
    CddDrm_Queue_Ecud = CDDDRM_QUEUE_EMPTY;
    CddDrm_Ecud_State = CDDDRM_ECUD_STATE_IDLE;
# if (CDDDRM_CFG_SUPPORT_ECUD_FINISHED == STD_ON)
    (*(CddDrm_EcuDetectionFinishPtr)CddDrm_Cfg_Ecud_Finished)();
# endif
#endif
    CddDrm_Cm_ChannelCloseAll(CDDDRM_RESPONSE_TESTER_DETECTED);
  }
  /* #30 Else if external tester queue reports that external tester is disconnected: Set DRM to normal run mode */
  else if (CddDrm_Queue_TesterConnected == CDDDRM_QUEUE_EXTERNAL_TESTER_DISCONNECTED)
  {
    CddDrm_State = CDDDRM_STATE_RUN;
  }
  else
  {
    /* nothing to do */
  }
  CddDrm_Queue_TesterConnected = CDDDRM_QUEUE_EMPTY;
}

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Queue_EcuDetection()
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
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Queue_EcuDetection(void)
{
  uint8_least lElementIndex;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check which action shall performed to ecu detection */
  if (CddDrm_Queue_Ecud == CDDDRM_QUEUE_ECUD_START)
  {
    /* Queue is only set to ECUD_START, if external tester is not present and
     * Ecu detection is not running. Handled in StartEcuDetection API, so it is
     * not necessary here.
     */
    CddDrm_Queue_Ecud = CDDDRM_QUEUE_EMPTY;
    /* prevent that ecu detection was started, while external tester is active */
    CddDrm_Ecud_State = CDDDRM_ECUD_STATE_READY;
    CddDrm_Ecud_ConnectionIterationIndex = 0;
    /* reset ECU Detection states for all connections */
    /* #20 Initialize ECU detection list */
    for (lElementIndex = 0; lElementIndex < CDDDRM_CFG_ECUD_LIST_SIZE; lElementIndex++)
    {
      CddDrm_Cfg_EcuDetectionData.EcuList[lElementIndex] = 0;
    }
  }
  /* #30 If application requested stop of ECU detection */
  if (CddDrm_Queue_Ecud == CDDDRM_QUEUE_ECUD_STOP)
  {
    CddDrm_Queue_Ecud = CDDDRM_QUEUE_EMPTY;
    /* #40 If detection is busy: close exclusive channel, stop detection */
    if (CddDrm_Ecud_State == CDDDRM_ECUD_STATE_BUSY)
    {
      CddDrm_Cm_ChannelClose(CDDDRM_CHANNEL_ECU_DETECTION_INDEX);
      CddDrm_Ecud_State = CDDDRM_ECUD_STATE_IDLE;
    }
    /* #50 If detection is ready: stop detection */
    else if (CddDrm_Ecud_State == CDDDRM_ECUD_STATE_READY)
    {
      CddDrm_Ecud_State = CDDDRM_ECUD_STATE_IDLE;
    }
    else
    {
      /* nothing to do */
    }
  }
}
#endif

#if (CDDDRM_CFG_SUPPORT_CANCELLATION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_Queue_RequestCancellation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_Queue_RequestCancellation(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* application requested close of channel(s) */
  /* #10 If application requested to cancel a processing request */
  if (CddDrm_Queue_CancelRequestFlag == TRUE)
  {
    CddDrm_ChannelIdType lChannelId;
    
    CddDrm_Queue_CancelRequestFlag = FALSE;
    /* #20 Iterate through all channels and close the active channels for which cancellation has been requested */
    for (lChannelId = 0; lChannelId < CDDDRM_CFG_CHANNEL_COUNT; lChannelId++)
    {
      if (CddDrm_Queue_CancelRequest[lChannelId] != CDDDRM_QUEUE_EMPTY)
      {
        CddDrm_SetChannelResponseCode(lChannelId, CddDrm_Queue_CancelRequest[lChannelId]);
        CddDrm_Cm_ChannelClose(lChannelId);
        CddDrm_Queue_CancelRequest[lChannelId] = CDDDRM_QUEUE_EMPTY;
      }
    }
  }
}
#endif
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_NvM_InitEcuDetectionData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_NvM_InitEcuDetectionData(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lReturnValue = E_OK;
  uint8 lElementIndex;
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
    lReturnValue = E_NOT_OK;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* initialize ECU Detection data */
    for (lElementIndex = 0; lElementIndex < CDDDRM_CFG_ECUD_LIST_SIZE; lElementIndex++)
    {
      CddDrm_Cfg_EcuDetectionData.EcuList[lElementIndex] = 0;
    }
    CddDrm_Ecud_SetRamBlockInitPattern(CDDDRM_CFG_ECUD_INIT_PATTERN);
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_INIT_ECU_DETECTION_DATA, lErrorId);
  }
# else
    CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  return lReturnValue;
}
#endif

#if ((CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON) && (CDDDRM_CFG_SUPPORT_NVRAM == STD_ON))
/**********************************************************************************************************************
 * CddDrm_NvM_JobFinished()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_NvM_JobFinished(uint8 ServiceId,
                                                         NvM_RequestResultType JobResult) /* PRQA S 3206 */ /* MD_CDDDRM_3206 */
{
  /* ----- Implementation ----------------------------------------------- */
  /* JobResult is not used, because next time Ecu Detection is started,
   * Nvm is triggered to write again and before detection list was initialized again.
   */
  CDDDRM_DUMMY_STATEMENT(JobResult);                                                                             /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  if ( (ServiceId == NVM_WRITE_BLOCK)
    && (CddDrm_Ecud_State == CDDDRM_ECUD_STATE_WRITE_NVM) )
  {
# if (CDDDRM_CFG_SUPPORT_ECUD_FINISHED == STD_ON)
    CddDrm_Ecud_State = CDDDRM_ECUD_STATE_FINISH;
# else
    CddDrm_Ecud_State = CDDDRM_ECUD_STATE_IDLE;
# endif
  }
  return E_OK;
}
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_10 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_10()
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
 */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_10(CddDrm_ConnectionIdType ConnectionId,
                                                              uint8 SessionType,
                                                              boolean SPRMIB,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (BufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferSize < CDDDRM_SERVICE_10_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if ( (SessionType == 0x00U)
       || ( (SessionType > 0x04U) && (SessionType < 0x40U) )
       ||   (SessionType > 0x7eU) ) /* ISOSAE Reserved sub-functions */
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (ConnectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If external tester is active: return that tester is active */
    if (CddDrm_State == CDDDRM_STATE_TESTER_DETECTED)
    {
      lResult = CDDDRM_REQUEST_TESTER_ACTIVE;
    }
    /* #30 Otherwise: external tester is inactive */
    else
    {
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #40 If requested service is allowed for this connection */
      if (CddDrm_Cfg_CheckFirewallRule(ConnectionId, CDDDRM_SERVICE_10) == TRUE)
#endif
      {
        CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(ConnectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION);
        /* #50 If a channel was allocated: initialize channel parameter, set request length and assemble the request 
         *     message by setting index position
        *      [0]: 0x10 (SID)
        *      [1]: SessionType | SPRMIB (SubFunction) 
        */
        if (lChannelId < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
        {
          CddDrm_Cm_ChannelInit(lChannelId,
                                CDDDRM_SERVICE_10,
                                (CddDrm_ResponseNotificationPtr)CddDrm_Cfg_ResponseNotification,
                                CDDDRM_PROTOCOL_LENGTH_10,
                                SPRMIB,
                                BufferInfo);
          
          /* set protocol buffer data, service id at index 0 already set */
          if (SPRMIB == TRUE)
          {
            CddDrm_ChannelInfo[lChannelId].protocolData[1] = CDDDRM_SET_SUPPRESS_POS_RESPONSE_BIT(SessionType);
          }
          else
          {
            CddDrm_ChannelInfo[lChannelId].protocolData[1] = CDDDRM_RESET_SUPPRESS_POS_RESPONSE_BIT(SessionType);
          }
          CddDrm_SetChannelProtocolLength(lChannelId, CDDDRM_PROTOCOL_LENGTH_10);
          
          /* set bit for using protocol buffer in channel state */
          CddDrm_SetProtocolBufferActive(lChannelId);
          lResult = CDDDRM_REQUEST_OK;
        }
        /* #60 Else if channel was not allocated, but free chnnels are available: return to application that connection is busy */
        else if (CddDrm_ActiveChannels < CDDDRM_APPL_CHANNEL_COUNT)
        {
          lResult = CDDDRM_REQUEST_CONNECTION_BUSY;
        }
        /* #70 Otherwise: return to application that no channel is available */
        else
        {
          /* lResult already set to NO_CHANNEL */
        }
      }
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #80 Return that firewall blocked the requested service */
      else
      {
        lResult = CDDDRM_REQUEST_FIREWALL_BLOCKED;
      }
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_10, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lResult;
} /* CddDrm_SvcSend_10() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_11 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_11()
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
 */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_11(CddDrm_ConnectionIdType ConnectionId,
                                                              uint8 ResetType,
                                                              boolean SPRMIB,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  /* #10 If module is initialized and all parameter checks passed */
  else if (BufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferSize < CDDDRM_SERVICE_11_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if ( (ResetType == 0x00U)
       || ( (ResetType > 0x05U) && (ResetType < 0x40U) )
       ||   (ResetType > 0x7eU) ) /* ISOSAE Reserved sub-functions */
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (ConnectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If external tester is active: return that tester is active */
    if (CddDrm_State == CDDDRM_STATE_TESTER_DETECTED)
    {
      lResult = CDDDRM_REQUEST_TESTER_ACTIVE;
    }
    /* #30 Otherwise: external tester is inactive */
    else
    {
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #40 If requested service is allowed for this connection */
      if (CddDrm_Cfg_CheckFirewallRule(ConnectionId, CDDDRM_SERVICE_11) == TRUE)
#endif
      {
        CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(ConnectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION);
        /* #50 If a channel was allocated: initialize channel parameter, set request length and assemble the request 
         *     message by setting index position
         *     [0]: 0x11 (SID)
         *     [1]: ResetType | SPRMIB (SubFunction) 
         */
        if (lChannelId < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
        {
          CddDrm_Cm_ChannelInit(lChannelId,
                                CDDDRM_SERVICE_11,
                                (CddDrm_ResponseNotificationPtr)CddDrm_Cfg_ResponseNotification,
                                CDDDRM_PROTOCOL_LENGTH_11,
                                SPRMIB,
                                BufferInfo);
         
          /* set protocol buffer data, service id at index 0 already set */
          if (SPRMIB == TRUE)
          {
            CddDrm_ChannelInfo[lChannelId].protocolData[1] = CDDDRM_SET_SUPPRESS_POS_RESPONSE_BIT(ResetType);
          }
          else
          {
            CddDrm_ChannelInfo[lChannelId].protocolData[1] = CDDDRM_RESET_SUPPRESS_POS_RESPONSE_BIT(ResetType);
          }
         
          CddDrm_SetChannelProtocolLength(lChannelId, CDDDRM_PROTOCOL_LENGTH_11);
          
          /* set bit for using protocol buffer in channel state */
          CddDrm_SetProtocolBufferActive(lChannelId);
          lResult = CDDDRM_REQUEST_OK;
        }
        /* #60 Else if channel was not allocated, but free chnnels are available: return to application that connection is busy */
        else if (CddDrm_ActiveChannels < CDDDRM_APPL_CHANNEL_COUNT)
        {
          lResult = CDDDRM_REQUEST_CONNECTION_BUSY;
        }
        /* #70 Otherwise: return to application that no channel is available */
        else
        {
          /* lResult already set to NO_CHANNEL */
        }
      }
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #80 Return that firewall blocked the requested service */
      else
      {
        lResult = CDDDRM_REQUEST_FIREWALL_BLOCKED;
      }
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_11, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lResult;
} /* CddDrm_SvcSend_11() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_1902 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_1902()
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
 */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_1902(CddDrm_ConnectionIdType ConnectionId,
                                                                uint8 DTCStatusMask,
                                                                P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (BufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferSize < CDDDRM_SERVICE_1902_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if (ConnectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If external tester is active: return that tester is active */
    if (CddDrm_State == CDDDRM_STATE_TESTER_DETECTED)
    {
      lResult = CDDDRM_REQUEST_TESTER_ACTIVE;
    }
    /* #30 Otherwise: external tester is inactive */
    else
    {
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #40 If requested service is allowed for this connection */
      if (CddDrm_Cfg_CheckFirewallRule(ConnectionId, CDDDRM_SERVICE_19) == TRUE)
#endif
      {
        CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(ConnectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION);
        /* #50 If a channel was allocated: initialize channel parameter, set request length and assemble the request 
         *     message by setting index position
         *     [0]: 0x19 (SID)
         *     [1]: 0x02 (SubFunction)
         *     [2]: DTCStatusMask 
         */
        if (lChannelId < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
        {
          CddDrm_Cm_ChannelInit(lChannelId,
                                CDDDRM_SERVICE_19,
                                (CddDrm_ResponseNotificationPtr)CddDrm_Cfg_ResponseNotification,
                                CDDDRM_PROTOCOL_LENGTH_1902,
                                FALSE,
                                BufferInfo);
          /* set protocol buffer data, service id at index 0 already set */
          CddDrm_ChannelInfo[lChannelId].protocolData[1] = 0x02U; /* sub-function */
          CddDrm_ChannelInfo[lChannelId].protocolData[2] = DTCStatusMask;
          CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_1902);
          /* set bit for using protocol buffer in channel state */
          CddDrm_SetProtocolBufferActive(lChannelId);
          lResult = CDDDRM_REQUEST_OK;
        }
        /* #60 Else if channel was not allocated, but free chnnels are available: return to application that connection is busy */
        else if (CddDrm_ActiveChannels < CDDDRM_APPL_CHANNEL_COUNT)
        {
          lResult = CDDDRM_REQUEST_CONNECTION_BUSY;
        }
        /* #70 Otherwise: return to application that no channel is available */
        else
        {
          /* lResult already set to NO_CHANNEL */
        }
      }
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #80 Return that firewall blocked the requested service */
      else
      {
        lResult = CDDDRM_REQUEST_FIREWALL_BLOCKED;
      }
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_1902, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lResult;
} /* CddDrm_SvcSend_1902() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_1904 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_1904()
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
 */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_1904(CddDrm_ConnectionIdType ConnectionId,
                                                                uint32 DTC,
                                                                uint8 RecordNumber,
                                                                P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (BufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferSize < CDDDRM_SERVICE_1904_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if (DTC > (0x00ffffffU))
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (ConnectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If external tester is active: return that tester is active */
    if (CddDrm_State == CDDDRM_STATE_TESTER_DETECTED)
    {
      lResult = CDDDRM_REQUEST_TESTER_ACTIVE;
    }
    /* #30 Otherwise: external tester is inactive */
    else
    {
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #40 If requested service is allowed for this connection */
      if (CddDrm_Cfg_CheckFirewallRule(ConnectionId, CDDDRM_SERVICE_19) == TRUE)
#endif
      {
        CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(ConnectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION);
        /* #50 If a channel was allocated: initialize channel parameter, set request length and assemble the request 
         *     message by setting index position
         *     [0]: 0x19 (SID)
         *     [1]: 0x04 (SubFunction)
         *     [2]: DTC  (High Low Byte)
         *     [3]: DTC  (Low High Byte)
         *     [4]: DTC  (Low Low byte)
         *     [5]: RecordNumber 
         */
        if (lChannelId < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
        {
          CddDrm_Cm_ChannelInit(lChannelId,
                                CDDDRM_SERVICE_19,
                                (CddDrm_ResponseNotificationPtr)CddDrm_Cfg_ResponseNotification,
                                CDDDRM_PROTOCOL_LENGTH_1904,
                                FALSE,
                                BufferInfo);
          /* set protocol buffer data, service id at index 0 already set */
          CddDrm_ChannelInfo[lChannelId].protocolData[1] = 0x04U; /* sub-function */
          CddDrm_ChannelInfo[lChannelId].protocolData[2] = CddDrm_GetHiLoByte(DTC);
          CddDrm_ChannelInfo[lChannelId].protocolData[3] = CddDrm_GetLoHiByte(DTC);
          CddDrm_ChannelInfo[lChannelId].protocolData[4] = CddDrm_GetLoLoByte(DTC);
          CddDrm_ChannelInfo[lChannelId].protocolData[5] = RecordNumber;
          CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_1904);
          /* set bit for using protocol buffer in channel state */
          CddDrm_SetProtocolBufferActive(lChannelId);
          lResult = CDDDRM_REQUEST_OK;
        }
        /* #60 Else if channel was not allocated, but free chnnels are available: return to application that connection is busy */
        else if (CddDrm_ActiveChannels < CDDDRM_APPL_CHANNEL_COUNT)
        {
          lResult = CDDDRM_REQUEST_CONNECTION_BUSY;
        }
        /* #70 Otherwise: return to application that no channel is available */
        else
        {
          /* lResult already set to NO_CHANNEL */
        }
      }
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #80 Return that firewall blocked the requested service */
      else
      {
        lResult = CDDDRM_REQUEST_FIREWALL_BLOCKED;
      }
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_1904, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lResult;
} /* CddDrm_SvcSend_1904() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_22 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_22()
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
 */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_22(CddDrm_ConnectionIdType ConnectionId,
                                                              uint16 DID,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (BufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferSize < CDDDRM_SERVICE_22_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if (ConnectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If external tester is active: return that tester is active */
    if (CddDrm_State == CDDDRM_STATE_TESTER_DETECTED)
    {
      lResult = CDDDRM_REQUEST_TESTER_ACTIVE;
    }
    /* #30 Otherwise: external tester is inactive */
    else
    {
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #40 If requested service is allowed for this connection */
      if (CddDrm_Cfg_CheckFirewallRule(ConnectionId, CDDDRM_SERVICE_22) == TRUE)
#endif
      {
        CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(ConnectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION);
        /* #50 If a channel was allocated: initialize channel parameter, set request length and assemble the request 
         *     message by setting index position
         *     [0]: 0x22 (SID)
         *     [1]: DID (High Byte) 
         *     [2]: DID (Low Byte) 
         */
        if (lChannelId < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
        {
          CddDrm_Cm_ChannelInit(lChannelId,
                                CDDDRM_SERVICE_22,
                                (CddDrm_ResponseNotificationPtr)CddDrm_Cfg_ResponseNotification,
                                CDDDRM_PROTOCOL_LENGTH_22,
                                FALSE,
                                BufferInfo);
          /* set protocol buffer data, service id at index 0 already set */
          CddDrm_ChannelInfo[lChannelId].protocolData[1] = CddDrm_GetHiByte(DID);
          CddDrm_ChannelInfo[lChannelId].protocolData[2] = CddDrm_GetLoByte(DID);
          CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_22);
          /* set bit for using protocol buffer in channel state */
          CddDrm_SetProtocolBufferActive(lChannelId);
          lResult = CDDDRM_REQUEST_OK;
        }
        /* #60 Else if channel was not allocated, but free chnnels are available: return to application that connection is busy */
        else if (CddDrm_ActiveChannels < CDDDRM_APPL_CHANNEL_COUNT)
        {
          lResult = CDDDRM_REQUEST_CONNECTION_BUSY;
        }
        /* #70 Otherwise: return to application that no channel is available */
        else
        {
          /* lResult already set to NO_CHANNEL */
        }
      }
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #80 Return that firewall blocked the requested service */
      else
      {
        lResult = CDDDRM_REQUEST_FIREWALL_BLOCKED;
      }
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_22, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lResult;
} /* CddDrm_SvcSend_22() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_27 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_27()
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
 */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_27(CddDrm_ConnectionIdType ConnectionId,
                                                              uint8 SubFunction,
                                                              CddDrm_LengthType DataLength,
                                                              boolean SPRMIB,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (BufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferSize < CDDDRM_SERVICE_27_MIN_RESPONSE_BUFFER) /* minimum response length */
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if ((BufferInfo->requestBufferDataPtr == NULL_PTR) && (DataLength > 0))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ( (SubFunction == 0x00U)
       || ( (SubFunction > 0x42U) && (SubFunction < 0x5fU) )
       ||   (SubFunction > 0x7eU) ) /* ISOSAE Reserved sub-functions */
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  /* if subfunction is a even number (security access send key) it must have a minimum request length of 3 byte */
  else if ( ((SubFunction % 2) == 0) && ((CDDDRM_PROTOCOL_LENGTH_27 + DataLength) < 3) )
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  } /* else: minimum length is protocol length */
  else if (ConnectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If external tester is active: return that tester is active */
    if (CddDrm_State == CDDDRM_STATE_TESTER_DETECTED)
    {
      lResult = CDDDRM_REQUEST_TESTER_ACTIVE;
    }
    /* #30 Otherwise: external tester is inactive */
    else
    {
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #40 If requested service is allowed for this connection */
      if (CddDrm_Cfg_CheckFirewallRule(ConnectionId, CDDDRM_SERVICE_27) == TRUE)
#endif
      {
        CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(ConnectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION);
        /* #50 If a channel was allocated: initialize channel parameter, set request length and assemble the request 
         *     message by setting index position
         *     [0]: 0x27 (SID)
         *     [1]: SubFunction | SPRMIB  
         *     [2]-[x]: securityAccessDataRecord - application payload with given length
         */
        if (lChannelId < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
        {
          CddDrm_Cm_ChannelInit(lChannelId,
                                CDDDRM_SERVICE_27,
                                (CddDrm_ResponseNotificationPtr)CddDrm_Cfg_ResponseNotification,
                                (CDDDRM_PROTOCOL_LENGTH_27 + DataLength),
                                SPRMIB,
                                BufferInfo);
          /* set protocol buffer data, service id at index 0 already set */
          if (SPRMIB == TRUE)                                                                       /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
          {                                                                                                      /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            CddDrm_ChannelInfo[lChannelId].protocolData[1] = CDDDRM_SET_SUPPRESS_POS_RESPONSE_BIT(SubFunction);
          }
          else
          {                                                                                                      /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            CddDrm_ChannelInfo[lChannelId].protocolData[1] = CDDDRM_RESET_SUPPRESS_POS_RESPONSE_BIT(SubFunction);
          }
          CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_27);
          /* set bit for using protocol buffer in channel state */
          CddDrm_SetProtocolBufferActive(lChannelId);
          lResult = CDDDRM_REQUEST_OK;
        }
        /* #60 Else if channel was not allocated, but free chnnels are available: return to application that connection is busy */
        else if (CddDrm_ActiveChannels < CDDDRM_APPL_CHANNEL_COUNT)
        {                                                                                                        /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
          lResult = CDDDRM_REQUEST_CONNECTION_BUSY;
        }
        /* #70 Otherwise: return to application that no channel is available */
        else
        {                                                                                                        /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
          /* lResult already set to NO_CHANNEL */
        }
      }
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #80 Return that firewall blocked the requested service */
      else
      {
        lResult = CDDDRM_REQUEST_FIREWALL_BLOCKED;
      }
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_27, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lResult;
} /* CddDrm_SvcSend_27() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_28 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_28()
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
 */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_28(CddDrm_ConnectionIdType ConnectionId,
                                                              uint8 SubFunction,
                                                              uint8 CommunicationType,
                                                              uint16 NodeIdNumber,
                                                              boolean SPRMIB,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (BufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferSize < CDDDRM_SERVICE_28_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if ( ( (SubFunction > 0x05U)
           && (SubFunction < 0x40U) )
       ||   (SubFunction > 0x7eU) ) /* ISOSAE Reserved sub-functions */
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (ConnectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If external tester is active: return that tester is active */
    if (CddDrm_State == CDDDRM_STATE_TESTER_DETECTED)
    {
      lResult = CDDDRM_REQUEST_TESTER_ACTIVE;
    }
    /* #30 Otherwise: external tester is inactive */
    else
    {
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #40 If requested service is allowed for this connection */
      if (CddDrm_Cfg_CheckFirewallRule(ConnectionId, CDDDRM_SERVICE_28) == TRUE)
#endif
      {
        CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(ConnectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION);
        /* #50 If a channel was allocated: initialize channel parameter, set request length and assemble the request 
         *     message by setting index position
         *     [0]: 0x28 (SID)
         *     [1]: SubFunction | SPRMIB 
         *     [2]: CommunicationType
         *     [3]: NodeNumber (High Byte) for SubFunction 0x04 or 0x05
         *     [4]: NodeNumber (Low Byte) for SubFunction 0x04 or 0x05 
         */
        if (lChannelId < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
        {
          /* if sub-function is 0x04 or 0x05 NodeIdNumber must be set, so request length increases */
          if ( (SubFunction == 0x04U)
            || (SubFunction == 0x05U) )
          {
            CddDrm_Cm_ChannelInit(lChannelId,
                                  CDDDRM_SERVICE_28,
                                  (CddDrm_ResponseNotificationPtr)CddDrm_Cfg_ResponseNotification,
                                  CDDDRM_PROTOCOL_LENGTH_28 + 2,
                                  SPRMIB,
                                  BufferInfo);
            CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_28 + 2);
            CddDrm_ChannelInfo[lChannelId].protocolData[3] = CddDrm_GetHiByte(NodeIdNumber);
            CddDrm_ChannelInfo[lChannelId].protocolData[4] = CddDrm_GetLoByte(NodeIdNumber);
          }
          else
          {
            CddDrm_Cm_ChannelInit(lChannelId,
                                  CDDDRM_SERVICE_28,
                                  (CddDrm_ResponseNotificationPtr)CddDrm_Cfg_ResponseNotification,
                                  CDDDRM_PROTOCOL_LENGTH_28,
                                  SPRMIB,
                                  BufferInfo);
            CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_28);
          }
          /* set protocol buffer data, service id at index 0 already set */
          if (SPRMIB == TRUE)
          {
            CddDrm_ChannelInfo[lChannelId].protocolData[1] = CDDDRM_SET_SUPPRESS_POS_RESPONSE_BIT(SubFunction);
          }
          else
          {
            CddDrm_ChannelInfo[lChannelId].protocolData[1] = CDDDRM_RESET_SUPPRESS_POS_RESPONSE_BIT(SubFunction);
          }
          CddDrm_ChannelInfo[lChannelId].protocolData[2] = CommunicationType;
          /* set bit in channel state */
          CddDrm_SetProtocolBufferActive(lChannelId);
          lResult = CDDDRM_REQUEST_OK;
        }
        /* #60 Else if channel was not allocated, but free chnnels are available: return to application that connection is busy */
        else if (CddDrm_ActiveChannels < CDDDRM_APPL_CHANNEL_COUNT)
        {
          lResult = CDDDRM_REQUEST_CONNECTION_BUSY;
        }
        /* #70 Otherwise: return to application that no channel is available */
        else
        {
          /* lResult already set to NO_CHANNEL */
        }
      }
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #80 Return that firewall blocked the requested service */
      else
      {
        lResult = CDDDRM_REQUEST_FIREWALL_BLOCKED;
      }
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_28, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lResult;
} /* CddDrm_SvcSend_28() */ /* PRQA S 6030, 6060, 6080 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_31 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_31()
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
 */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_31(CddDrm_ConnectionIdType ConnectionId,
                                                              uint8 SubFunction,
                                                              uint16 RoutineId,
                                                              CddDrm_LengthType RoutineOptionLength,
                                                              boolean SPRMIB,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (BufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferSize < CDDDRM_SERVICE_31_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if ((BufferInfo->requestBufferDataPtr == NULL_PTR) && (RoutineOptionLength > 0))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ( (SubFunction == 0x00U)
         || (SubFunction > 0x03U) ) /* ISOSAE Reserved sub-functions */
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (ConnectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If external tester is active: return that tester is active */
    if (CddDrm_State == CDDDRM_STATE_TESTER_DETECTED)
    {
      lResult = CDDDRM_REQUEST_TESTER_ACTIVE;
    }
    /* #30 Otherwise: external tester is inactive */
    else
    {
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #40 If requested service is allowed for this connection */
      if (CddDrm_Cfg_CheckFirewallRule(ConnectionId, CDDDRM_SERVICE_31) == TRUE)
#endif
      {
        CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(ConnectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION);
        /* #50 If a channel was allocated: initialize channel parameter, set request length and assemble the request 
         *     message by setting index position
         *     [0]: 0x31 (SID)
         *     [1]: SubFunction | SPRMIB 
         *     [2]: RoutineId (High Byte)
         *     [3]: RoutineId (Low Byte) 
         *     [4]-[x]: routineControlOptionRecord - application payload with given length
         */
        if (lChannelId < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
        {
          /* routine control options are optional if sub-function is start-/stop routine */
          if ( (SubFunction == 0x01U) || (SubFunction == 0x02U) )
          {                                                                                                      /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            CddDrm_Cm_ChannelInit(lChannelId,
                                  CDDDRM_SERVICE_31,
                                  (CddDrm_ResponseNotificationPtr)CddDrm_Cfg_ResponseNotification,
                                  CDDDRM_PROTOCOL_LENGTH_31 + RoutineOptionLength,
                                  SPRMIB,
                                  BufferInfo);
          }
          else
          {                                                                                                      /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            CddDrm_Cm_ChannelInit(lChannelId,
                                  CDDDRM_SERVICE_31,
                                  (CddDrm_ResponseNotificationPtr)CddDrm_Cfg_ResponseNotification,
                                  CDDDRM_PROTOCOL_LENGTH_31,
                                  SPRMIB,
                                  BufferInfo);
          }

          CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_31);
          /* set protocol buffer data, service id at index 0 already set */
          if (SPRMIB == TRUE)
          {                                                                                                      /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            CddDrm_ChannelInfo[lChannelId].protocolData[1] = CDDDRM_SET_SUPPRESS_POS_RESPONSE_BIT(SubFunction);
          }
          else
          {                                                                                                      /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            CddDrm_ChannelInfo[lChannelId].protocolData[1] = CDDDRM_RESET_SUPPRESS_POS_RESPONSE_BIT(SubFunction);
          }
          CddDrm_ChannelInfo[lChannelId].protocolData[2] = CddDrm_GetHiByte(RoutineId);
          CddDrm_ChannelInfo[lChannelId].protocolData[3] = CddDrm_GetLoByte(RoutineId);
          /* set bit in channel state */
          CddDrm_SetProtocolBufferActive(lChannelId);
          lResult = CDDDRM_REQUEST_OK;
        }
        /* #60 Else if channel was not allocated, but free chnnels are available: return to application that connection is busy */
        else if (CddDrm_ActiveChannels < CDDDRM_APPL_CHANNEL_COUNT)
        {
          lResult = CDDDRM_REQUEST_CONNECTION_BUSY;
        }
        /* #70 Otherwise: return to application that no channel is available */
        else
        {
          /* lResult already set to NO_CHANNEL */
        }
      }
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #80 Return that firewall blocked the requested service */
      else
      {
        lResult = CDDDRM_REQUEST_FIREWALL_BLOCKED;
      }
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_31, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lResult;
} /* CddDrm_SvcSend_31() */ /* PRQA S 6030, 6060, 6080 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_34 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_34()
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
 */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_34(CddDrm_ConnectionIdType ConnectionId,
                                                              uint8 DataFormatId,
                                                              uint8 AddressAndLength,
                                                              CddDrm_LengthType DataLength,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (BufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferSize < CDDDRM_SERVICE_34_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if ((BufferInfo->requestBufferDataPtr == NULL_PTR) && (DataLength > 0))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ( (DataLength + CDDDRM_PROTOCOL_LENGTH_34) < 5)
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (ConnectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If external tester is active: return that tester is active */
    if (CddDrm_State == CDDDRM_STATE_TESTER_DETECTED)
    {
      lResult = CDDDRM_REQUEST_TESTER_ACTIVE;
    }
    /* #30 Otherwise: external tester is inactive */
    else
    {
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #40 If requested service is allowed for this connection */
      if (CddDrm_Cfg_CheckFirewallRule(ConnectionId, CDDDRM_SERVICE_34) == TRUE)
#endif
      {
        CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(ConnectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION);
        /* #50 If a channel was allocated: initialize channel parameter, set request length and assemble the request 
         *     message by setting index position
         *     [0]: 0x34 (SID)
         *     [1]: DataFormatId 
         *     [2]: AdressAndLength 
         *     [3]-[x]: MemoryAddress and MemorySize - application payload with given length
         */
        if (lChannelId < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
        {
            CddDrm_Cm_ChannelInit(lChannelId,
                                  CDDDRM_SERVICE_34,
                                  (CddDrm_ResponseNotificationPtr)CddDrm_Cfg_ResponseNotification,
                                  CDDDRM_PROTOCOL_LENGTH_34 + DataLength,
                                  FALSE,
                                  BufferInfo);

          CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_34);
          /* set protocol buffer data, service id at index 0 already set */
          CddDrm_ChannelInfo[lChannelId].protocolData[1] = DataFormatId;
          CddDrm_ChannelInfo[lChannelId].protocolData[2] = AddressAndLength;
          /* set bit in channel state */
          CddDrm_SetProtocolBufferActive(lChannelId);
          lResult = CDDDRM_REQUEST_OK;
        }
        /* #60 Else if channel was not allocated, but free chnnels are available: return to application that connection is busy */
        else if (CddDrm_ActiveChannels < CDDDRM_APPL_CHANNEL_COUNT)
        {
          lResult = CDDDRM_REQUEST_CONNECTION_BUSY;
        }
        /* #70 Otherwise: return to application that no channel is available */
        else
        {
          /* lResult already set to NO_CHANNEL */
        }
      }
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #80 Return that firewall blocked the requested service */
      else
      {
        lResult = CDDDRM_REQUEST_FIREWALL_BLOCKED;
      }
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_34, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lResult;
} /* CddDrm_SvcSend_34() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_36 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_36()
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
 */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_36(CddDrm_ConnectionIdType ConnectionId,
                                                              uint8 BlockSeqCounter,
                                                              CddDrm_LengthType TransferDataLength,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (BufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((BufferInfo->requestBufferDataPtr == NULL_PTR) && (TransferDataLength > 0))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferSize < CDDDRM_SERVICE_36_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if (ConnectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If external tester is active: return that tester is active */
    if (CddDrm_State == CDDDRM_STATE_TESTER_DETECTED)
    {
      lResult = CDDDRM_REQUEST_TESTER_ACTIVE;
    }
    /* #30 Otherwise: external tester is inactive */
    else
    {
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #40 If requested service is allowed for this connection */
      if (CddDrm_Cfg_CheckFirewallRule(ConnectionId, CDDDRM_SERVICE_36) == TRUE)
#endif
      {
        CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(ConnectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION);
        /* #50 If a channel was allocated: initialize channel parameter, set request length and assemble the request 
         *     message by setting index position
         *     [0]: 0x36 (SID)
         *     [1]: BlockSequenceCounter  
         *     [2]-[x]: TransferRequestParameterRecord - application payload with given length
         */
        if (lChannelId < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
        {
          CddDrm_Cm_ChannelInit(lChannelId,
                                CDDDRM_SERVICE_36,
                                (CddDrm_ResponseNotificationPtr)CddDrm_Cfg_ResponseNotification,
                                CDDDRM_PROTOCOL_LENGTH_36 + TransferDataLength,
                                FALSE,
                                BufferInfo);

          CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_36);
          CddDrm_ChannelInfo[lChannelId].protocolData[1] = BlockSeqCounter;
          /* set protocol buffer data, service id at index 0 already set */
          /* set bit in channel state */
          CddDrm_SetProtocolBufferActive(lChannelId);
          lResult = CDDDRM_REQUEST_OK;
        }
        /* #60 Else if channel was not allocated, but free chnnels are available: return to application that connection is busy */
        else if (CddDrm_ActiveChannels < CDDDRM_APPL_CHANNEL_COUNT)
        {
          lResult = CDDDRM_REQUEST_CONNECTION_BUSY;
        }
        /* #70 Otherwise: return to application that no channel is available */
        else
        {
          /* lResult already set to NO_CHANNEL */
        }
      }
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #80 Return that firewall blocked the requested service */
      else
      {
        lResult = CDDDRM_REQUEST_FIREWALL_BLOCKED;
      }
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_36, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lResult;
} /* CddDrm_SvcSend_36() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_37 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_37()
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
 */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_37(CddDrm_ConnectionIdType ConnectionId,
                                                              CddDrm_LengthType TransferDataLength,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (BufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((BufferInfo->requestBufferDataPtr == NULL_PTR) && (TransferDataLength > 0))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferSize < CDDDRM_SERVICE_37_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if (ConnectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If external tester is active: return that tester is active */
    if (CddDrm_State == CDDDRM_STATE_TESTER_DETECTED)
    {
      lResult = CDDDRM_REQUEST_TESTER_ACTIVE;
    }
    /* #30 Otherwise: external tester is inactive */
    else
    {
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #40 If requested service is allowed for this connection */
      if (CddDrm_Cfg_CheckFirewallRule(ConnectionId, CDDDRM_SERVICE_37) == TRUE)
#endif
      {
        CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(ConnectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION);
        /* #50 If a channel was allocated: initialize channel parameter, set request length and assemble the request 
         *     message by setting index position
         *     [0]: 0x37 (SID) 
         *     [1]-[x]: TransferRequestParameterRecord - application payload with given length
         */
        if (lChannelId < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
        {
          CddDrm_Cm_ChannelInit(lChannelId,
                                CDDDRM_SERVICE_37,
                                (CddDrm_ResponseNotificationPtr)CddDrm_Cfg_ResponseNotification,
                                CDDDRM_PROTOCOL_LENGTH_37 + TransferDataLength,
                                FALSE,
                                BufferInfo);

          /* service id at index 0 already set */
          CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_37);
          /* set bit in channel state */
          CddDrm_SetProtocolBufferActive(lChannelId);
          lResult = CDDDRM_REQUEST_OK;
        }
        /* #60 Else if channel was not allocated, but free chnnels are available: return to application that connection is busy */
        else if (CddDrm_ActiveChannels < CDDDRM_APPL_CHANNEL_COUNT)
        {
          lResult = CDDDRM_REQUEST_CONNECTION_BUSY;
        }
        /* #70 Otherwise: return to application that no channel is available */
        else
        {
          /* lResult already set to NO_CHANNEL */
        }
      }
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #80 Return that firewall blocked the requested service */
      else
      {
        lResult = CDDDRM_REQUEST_FIREWALL_BLOCKED;
      }
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_37, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lResult;
} /* CddDrm_SvcSend_37() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_3E == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_3E()
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
 */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_3E(CddDrm_ConnectionIdType ConnectionId,
                                                              boolean SPRMIB,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (BufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferSize < CDDDRM_SERVICE_3E_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if (ConnectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If external tester is active: return that tester is active */
    if (CddDrm_State == CDDDRM_STATE_TESTER_DETECTED)
    {
      lResult = CDDDRM_REQUEST_TESTER_ACTIVE;
    }
    /* #30 Otherwise: external tester is inactive */
    else
    {
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #40 If requested service is allowed for this connection */
      if (CddDrm_Cfg_CheckFirewallRule(ConnectionId, CDDDRM_SERVICE_3E) == TRUE)
#endif
      {
        CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(ConnectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION);
        /* #50 If a channel was allocated: initialize channel parameter, set request length and assemble the request 
         *     message by setting index position
         *     [0]: 0x3E (SID)
         *     [1]: SPRMIB 
         */
        if (lChannelId < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
        {
          CddDrm_Cm_ChannelInit(lChannelId,
                                CDDDRM_SERVICE_3E,
                                (CddDrm_ResponseNotificationPtr)CddDrm_Cfg_ResponseNotification,
                                CDDDRM_PROTOCOL_LENGTH_3E,
                                SPRMIB,
                                BufferInfo);

          CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_3E);
          /* service id at index 0 already set */
          if (SPRMIB == TRUE)
          {
            CddDrm_ChannelInfo[lChannelId].protocolData[1] = CDDDRM_SET_SUPPRESS_POS_RESPONSE_BIT(0U);
          }
          else
          {
            CddDrm_ChannelInfo[lChannelId].protocolData[1] = CDDDRM_RESET_SUPPRESS_POS_RESPONSE_BIT(0U);
          }
          /* set bit in channel state */
          CddDrm_SetProtocolBufferActive(lChannelId);
          lResult = CDDDRM_REQUEST_OK;
        }
        /* #60 Else if channel was not allocated, but free chnnels are available: return to application that connection is busy */
        else if (CddDrm_ActiveChannels < CDDDRM_APPL_CHANNEL_COUNT)
        {
          lResult = CDDDRM_REQUEST_CONNECTION_BUSY;
        }
        /* #70 Otherwise: return to application that no channel is available */
        else
        {
          /* lResult already set to NO_CHANNEL */
        }
      }
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #80 Return that firewall blocked the requested service */
      else
      {
        lResult = CDDDRM_REQUEST_FIREWALL_BLOCKED;
      }
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_3E, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lResult;
} /* CddDrm_SvcSend_3E() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (CDDDRM_CFG_SUPPORT_SERVICE_85 == STD_ON)
/**********************************************************************************************************************
 * CddDrm_SvcSend_85()
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
 */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_85(CddDrm_ConnectionIdType ConnectionId,
                                                              uint8 SubFunction,
                                                              CddDrm_LengthType RecordLength,
                                                              boolean SPRMIB,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (BufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if ((BufferInfo->requestBufferDataPtr == NULL_PTR) && (RecordLength > 0))
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferSize < CDDDRM_SERVICE_85_MIN_RESPONSE_BUFFER)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if ( (SubFunction == 0)
       || ( (SubFunction > 0x02U) && (SubFunction < 0x40U) ) /* ISOSAE Reserved sub-functions */
       || (SubFunction > 0x7eU) )
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (ConnectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If external tester is active: return that tester is active */
    if (CddDrm_State == CDDDRM_STATE_TESTER_DETECTED)
    {
      lResult = CDDDRM_REQUEST_TESTER_ACTIVE;
    }
    /* #30 Otherwise: external tester is inactive */
    else
    {
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #40 If requested service is allowed for this connection */
      if (CddDrm_Cfg_CheckFirewallRule(ConnectionId, CDDDRM_SERVICE_85) == TRUE)
#endif
      {
        CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(ConnectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION);
        /* #50 If a channel was allocated: initialize channel parameter, set request length and assemble the request 
         *     message by setting index position
         *     [0]: 0x85 (SID)
         *     [1]: SubFunction | SPRMIB 
         *     [2]-[x]: DTCSettingControlOptionRecord - application payload with given length
         */
        if (lChannelId < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
        {
          CddDrm_Cm_ChannelInit(lChannelId,
                                CDDDRM_SERVICE_85,
                                (CddDrm_ResponseNotificationPtr)CddDrm_Cfg_ResponseNotification,
                                CDDDRM_PROTOCOL_LENGTH_85 + RecordLength,
                                SPRMIB,
                                BufferInfo);

          CddDrm_SetChannelProtocolLength(lChannelId,  CDDDRM_PROTOCOL_LENGTH_85);
          /* service id at index 0 already set */
          if (SPRMIB == TRUE)
          {                                                                                                      /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            CddDrm_ChannelInfo[lChannelId].protocolData[1] = CDDDRM_SET_SUPPRESS_POS_RESPONSE_BIT(SubFunction);
          }
          else
          {                                                                                                      /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
            CddDrm_ChannelInfo[lChannelId].protocolData[1] = CDDDRM_RESET_SUPPRESS_POS_RESPONSE_BIT(SubFunction);
          }
          /* set bit in channel state */
          CddDrm_SetProtocolBufferActive(lChannelId);
          lResult = CDDDRM_REQUEST_OK;
        }
        /* #60 Else if channel was not allocated, but free chnnels are available: return to application that connection is busy */
        else if (CddDrm_ActiveChannels < CDDDRM_APPL_CHANNEL_COUNT)
        {
          lResult = CDDDRM_REQUEST_CONNECTION_BUSY;
        }
        /* #70 Otherwise: return to application that no channel is available */
        else
        {
          /* lResult already set to NO_CHANNEL */
        }
      }
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #80 Return that firewall blocked the requested service */
      else
      {
        lResult = CDDDRM_REQUEST_FIREWALL_BLOCKED;
      }
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND_85, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lResult;
} /* CddDrm_SvcSend_85() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#if (CDDDRM_CFG_SUPPORT_CANCELLATION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_CancelRequest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_CancelRequest(CddDrm_ConnectionIdType ConnectionId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  Std_ReturnType lReturnValue = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and parameter checks are successful */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (ConnectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If connection is assigned to a channel: queue that the channel linked to the given connection shall be cancelled */
    if (CddDrm_ChannelAssign[ConnectionId] < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
    {
      CddDrm_Queue_CancelRequestFlag = TRUE;
      CddDrm_Queue_CancelRequest[CddDrm_ChannelAssign[ConnectionId]] = CDDDRM_RESPONSE_CHANNELS_CLOSED;
      lReturnValue = E_OK;
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_CANCEL_REQUEST, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lReturnValue;
} /* CddDrm_CancelRequest() */
#endif

/**********************************************************************************************************************
 * CddDrm_SvcSend()
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
 */
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend(CddDrm_ConnectionIdType ConnectionId,
                                                           boolean SPRMIB,
                                                           P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) BufferInfo,
                                                           CddDrm_LengthType RequestLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_RequestReturnType lResult = CDDDRM_REQUEST_NO_CHANNEL;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and all parameter checks passed */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (BufferInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->requestBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferDataPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (BufferInfo->responseBufferSize < 3)
  {
    lErrorId = CDDDRM_E_INVALID_BUFFER_LENGTH;
  }
  else if (RequestLength == 0)
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if ( (BufferInfo->requestBufferDataPtr[0] & CDDDRM_RESPONSE_CODE_VALIDATION) > 0) /* check for valid UDS Service (response bit not set) */
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (BufferInfo->requestBufferDataPtr[0] == (0x3fU)) /* 0x3F | 0x40 = 0x7F -> NRC */
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (ConnectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If external tester is active: return that tester is active */
    if (CddDrm_State == CDDDRM_STATE_TESTER_DETECTED)
    {
      lResult = CDDDRM_REQUEST_TESTER_ACTIVE;
    }
    /* #30 Otherwise: external tester is inactive */
    else
    {
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #40 If requested service is allowed for this connection */
      if (CddDrm_Cfg_CheckFirewallRule(ConnectionId, BufferInfo->requestBufferDataPtr[0]) == TRUE)
#endif
      {
        CddDrm_ChannelIdType lChannelId = CddDrm_Cm_ChannelGet(ConnectionId, CDDDRM_CHANNEL_CLIENT_APPLICATION);
        /* #50 If a channel was allocated: initialize channel parameter */
        if (lChannelId < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
        {                                                                                                        /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
          CddDrm_Cm_ChannelInit(lChannelId,
                                BufferInfo->requestBufferDataPtr[0],
                                (CddDrm_ResponseNotificationPtr)CddDrm_Cfg_ResponseNotification,
                                RequestLength,
                                SPRMIB,
                                BufferInfo);

          CddDrm_ResetProtocolBufferActive(lChannelId);
          lResult = CDDDRM_REQUEST_OK;
        }
        /* #60 Else if return to application that connection is busy */
        else if (CddDrm_ActiveChannels < CDDDRM_APPL_CHANNEL_COUNT)                                              /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
        {                                                                                                        /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
          /* free channels available but cannot assign to a connection */
          lResult = CDDDRM_REQUEST_CONNECTION_BUSY;
        }
        /* #70 Otherwise: return to application that no channel is available */
        else
        {                                                                                                        /* PRQA S 0715 */ /* MD_MSR_1.1_715 */
          /* lResult already set to NO_CHANNEL */
        }
      }
#if ((CDDDRM_CFG_SUPPORT_FIREWALL == STD_ON) || (CDDDRM_CFG_SUPPORT_USER_FIREWALL == STD_ON))
      /* #80 Otherwise return that firewall blocked the requested service */
      else
      {
        lResult = CDDDRM_REQUEST_FIREWALL_BLOCKED;
      }
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_SVC_SEND, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lResult;
} /* CddDrm_SvcSend() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 * CddDrm_Transmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_Transmit(PduIdType TxPduId,
                                                  P2CONST(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA)PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lResult = E_NOT_OK;
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and parameter checks are successful */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if(PduInfoPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if(TxPduId >= CDDDRM_CFG_TXPDU_COUNT)
  {
    lErrorId = CDDDRM_E_PDU_ID_TX_OUT_OF_RANGE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CDDDRM_DUMMY_STATEMENT(TxPduId);                                                                          /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
    /* #20 If at least one channel is active and Pdu id is assigned to an active channel */
    if(CddDrm_ActiveChannels > 0)
    {
      /* Check if open connection has the received pduId and the connection is waiting for response,
       * store received data and change connection state to fetch data
       */
      /* Prevent that channel is closed from main task */

      /* >>>> -------------------------------- Enter Critical Section: ChannelManager */
      CddDrm_EnterCritical_ChannelManager();
      if (CddDrm_ChannelAssign[CddDrm_Cfg_GetConnectionIdByTxPdu(TxPduId)] < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
      {
        /* #30 If assigned channel is waiting for response */
        if (CddDrm_GetChannelState(CddDrm_ChannelAssign[CddDrm_Cfg_GetConnectionIdByTxPdu(TxPduId)]) == CDDDRM_CHANNEL_STATE_WAIT_RESPONSE)
        {
          /* #40 Switch to next channel state CDDDRM_CHANNEL_STATE_RESPONSE_START */
          CddDrm_SetChannelState(CddDrm_ChannelAssign[CddDrm_Cfg_GetConnectionIdByTxPdu(TxPduId)], CDDDRM_CHANNEL_STATE_RESPONSE_START);
          CddDrm_SetChannelMessageLength(CddDrm_ChannelAssign[CddDrm_Cfg_GetConnectionIdByTxPdu(TxPduId)], PduInfoPtr->SduLength);
          CddDrm_SetChannelCurrentMessageIndex(CddDrm_ChannelAssign[CddDrm_Cfg_GetConnectionIdByTxPdu(TxPduId)], 0);
          lResult = E_OK;
        }
      }
/* >>>> -------------------------------- Leave Critical Section: ChannelManager */
      CddDrm_LeaveCritical_ChannelManager();
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_TRANSMIT, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lResult;
} /* CddDrm_Transmit() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

#if (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON)
/**********************************************************************************************************************
 * CddDrm_CancelTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_CancelTransmit(PduIdType TxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lResult = E_NOT_OK;
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 If module is initialized and parameter checks are successful */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if(TxPduId >= CDDDRM_CFG_TXPDU_COUNT)
  {
    lErrorId = CDDDRM_E_PDU_ID_TX_OUT_OF_RANGE;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If at least one channel is active and Pdu id is assigned to an active channel */
    if(CddDrm_ActiveChannels > 0)
    {
      /* Check if connection related to PduId is open */
      if (CddDrm_ChannelAssign[CddDrm_Cfg_GetConnectionIdByTxPdu(TxPduId)] < CDDDRM_CHANNEL_INFO_MAX_NUMBER)
      {
        CddDrm_ChannelIdType lChannelId = CddDrm_ChannelAssign[CddDrm_Cfg_GetConnectionIdByTxPdu(TxPduId)];
        /* #30 If assigned channel is processing response */
        if ( (CddDrm_GetChannelState(lChannelId) == CDDDRM_CHANNEL_STATE_RESPONSE_START)
          || (CddDrm_GetChannelState(lChannelId) == CDDDRM_CHANNEL_STATE_RESPONSE_COPY) )
        {
          /* #40 Switch state to processing done. Notify application about cancelled response processing */
          CddDrm_SetChannelResponseCode(CddDrm_ChannelAssign[CddDrm_Cfg_GetConnectionIdByTxPdu(TxPduId)], CDDDRM_RESPONSE_PDUR_RX_CANCELED);
          CddDrm_SetChannelState(CddDrm_ChannelAssign[CddDrm_Cfg_GetConnectionIdByTxPdu(TxPduId)], CDDDRM_CHANNEL_STATE_RESPONSE_DONE);
          lResult = E_OK;
        }
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_CANCELTRANSMIT, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  return lResult;
} /* CddDrm_CancelTransmit() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (CDDDRM_CFG_SUPPORT_CHANGEPARAM == STD_ON)
/**********************************************************************************************************************
 * CddDrm_ChangeParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_ChangeParameter(PduIdType id,                                           /* PRQA S 3206 */ /* MD_CDDDRM_3206 */
                                                         TPParameterType parameter,                              /* PRQA S 3206 */ /* MD_CDDDRM_3206 */
                                                         uint16 value)                                           /* PRQA S 3206 */ /* MD_CDDDRM_3206 */
{
  /* ----- Implementation ----------------------------------------------- */
  /* Dummy function, nothing to do here */
  CDDDRM_DUMMY_STATEMENT(id);                                                                                    /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  CDDDRM_DUMMY_STATEMENT(parameter);                                                                             /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  CDDDRM_DUMMY_STATEMENT(value);                                                                                 /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_CHANGEPARAMETER, CDDDRM_E_API_ERROR);
# endif
  return E_NOT_OK;
} /* CddDrm_ChangeParameter() */
#endif

#if (CDDDRM_CFG_SUPPORT_CANCELRECEIVE == STD_ON)
/**********************************************************************************************************************
 * CddDrm_CancelReceive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_CancelReceive(PduIdType CddDrmRxSduId)                                  /* PRQA S 3206 */ /* MD_CDDDRM_3206 */
{
  /* ----- Implementation ----------------------------------------------- */
  /* Dummy function, nothing to do here */
  CDDDRM_DUMMY_STATEMENT(CddDrmRxSduId);                                                                         /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_CANCELRECEIVE, CDDDRM_E_API_ERROR);
# endif
  return E_NOT_OK;
} /* CddDrm_CancelReceive() */
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_StartEcuDetection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_StartEcuDetection(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lResult = E_NOT_OK;
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else
#endif
  /* ----- Implementation ----------------------------------------------- */
  {
    /* Ecu detection is not running and external tester is not present */
    if ( (CddDrm_Ecud_State == CDDDRM_ECUD_STATE_IDLE)
      && (CddDrm_State == CDDDRM_STATE_RUN) )
    {
      CddDrm_Queue_Ecud = CDDDRM_QUEUE_ECUD_START;
      lResult = E_OK;
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_ECU_DETECTION_START, lErrorId);
  }
# else
    CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  return lResult;
} /* CddDrm_StartEcuDetection() */
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_StopEcuDetection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_StopEcuDetection(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_Queue_Ecud = CDDDRM_QUEUE_ECUD_STOP;
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_ECU_DETECTION_STOP, lErrorId);
  }
# else
    CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  return E_OK;
} /* CddDrm_StartEcuDetection() */
#endif

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_GetEcuDetectionResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(CddDrm_EcudStateType, CDDDRM_CODE) CddDrm_GetEcuDetectionResult(CddDrm_ConnectionIdType ConnectionId)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_EcudStateType lResult;
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  lResult = CDDDRM_ECUD_CONNECTION_NOT_DISCOVERED;
  /* #10 If module is initialized */
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (ConnectionId >= CDDDRM_CFG_CONNECTION_COUNT)
  {
    lErrorId = CDDDRM_E_INVALID_CONNECTION;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Get detection result for given connection */
    /* read a byte from ECU Detection data array and extract relevant ECU state */
    lResult = CddDrm_Cfg_EcuDetectionData.EcuList[CDDDRM_ECUD_GET_CONNECTION_INDEX(ConnectionId)];
    lResult = (CddDrm_EcudStateType)(lResult >> CDDDRM_ECUD_GET_CONNECTION_POS(ConnectionId)) & CDDDRM_ECUD_RESULT_MASK;
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_ECU_DETECTION_RESULT, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  /* #30 Return connection specific detection result */
  return lResult;
} /* CddDrm_StartEcuDetection() */
#endif

/**********************************************************************************************************************
 *  CddDrm_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, CDDDRM_CODE) CddDrm_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  CddDrm_ModuleInitialized = CDDDRM_UNINIT;
  CddDrm_ConfigSet_pt = NULL_PTR;
} /* CddDrm_InitMemory() */

/**********************************************************************************************************************
 * CddDrm_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, CDDDRM_CODE) CddDrm_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  if (CddDrm_ModuleInitialized == (uint8) CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
/**************************************************************
 * External tester detection Queue
 *************************************************************/
    CddDrm_Queue_ExternalTester();

/**************************************************************
 * Ecu Detection Start/Stop Queue
 *************************************************************/
#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
    CddDrm_Queue_EcuDetection();
#endif

/**************************************************************
 * Request cancellation Queue
 *************************************************************/
#if (CDDDRM_CFG_SUPPORT_CANCELLATION == STD_ON)
    CddDrm_Queue_RequestCancellation();
#endif
#if ((CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON) || (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON))
/**************************************************************
 * [Separation time]/[global request delay] and P2/P2* handling
 *************************************************************/
    CddDrm_Timer_Task();
#endif

    if (CddDrm_ActiveChannels > 0)
    {
#if ((CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_OFF) && (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_OFF))
/**************************************************************
 * Only P2/P2* handling
 *************************************************************/
      CddDrm_Timer_Task();
#endif
/**************************************************************
 * Channel state machine handler
 *************************************************************/
      CddDrm_Cm_StateTask();
    }

#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**************************************************************
 * ECU Detection state machine handler
 *************************************************************/
    CddDrm_Ecud_StateTask();
#endif
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_MAINFUNCTION, lErrorId);
  }
#else
  CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
} /* CddDrm_MainFunction() */

/**********************************************************************************************************************
 * CddDrm_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CDDDRM_CODE) CddDrm_Init(P2CONST(CddDrm_ConfigPtrType, AUTOMATIC, CDDDRM_PBCFG) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  CddDrm_ConnectionIdType lConnectionIndex;
  CddDrm_ChannelIdType lChannelId;
  /* ----- Development Error Checks ------------------------------------- */
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_INIT)
  {
    lErrorId = CDDDRM_E_ALREADY_INITIALIZED;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    CddDrm_ConfigSet_pt             = ConfigPtr;
    CddDrm_ModuleInitialized        = (uint8)CDDDRM_INIT;
    /* Initial DRM State */
    CddDrm_State                    = CDDDRM_STATE_RUN;
    CddDrm_Queue_TesterConnected    = CDDDRM_QUEUE_EMPTY;
    CddDrm_ActiveChannels           = 0;
#if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
    CddDrm_Ecud_State               = CDDDRM_ECUD_STATE_IDLE;
    CddDrm_Queue_Ecud               = CDDDRM_QUEUE_EMPTY;
    
    if(CddDrm_Ecud_GetRamBlockInitPattern() != CDDDRM_CFG_ECUD_INIT_PATTERN)
    {
      (void)CddDrm_NvM_InitEcuDetectionData();
    }
#endif
#if (CDDDRM_CFG_SUPPORT_CANCELLATION == STD_ON)
    CddDrm_Queue_CancelRequestFlag = FALSE;
#endif
    /* At startup there must be no delay between requests */
#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
    CddDrm_RequestDelayTimer        = 0;
#endif
#if (CDDDRM_CFG_SUPPORT_REQUEST_DELAY == STD_ON)
    CddDrm_NextConnection           = 0;
#endif
    /* Initialize list, so connection channels can be assigned */
    for (lConnectionIndex = 0; lConnectionIndex < CDDDRM_CFG_CONNECTION_COUNT; lConnectionIndex++)
    {
      CddDrm_ChannelAssign[lConnectionIndex] = CDDDRM_CHANNEL_INFO_FREE;
#if (CDDDRM_CFG_SUPPORT_SEPARATION_TIME == STD_ON)
      CddDrm_SeparationTimer[lConnectionIndex] = 0;
#endif
    }

    for(lChannelId = 0; lChannelId < CDDDRM_CFG_CHANNEL_COUNT; lChannelId++)
    {
#if (CDDDRM_CFG_SUPPORT_CANCELLATION == STD_ON)
      CddDrm_Queue_CancelRequest[lChannelId] = CDDDRM_QUEUE_EMPTY;
#endif
      CddDrm_SetChannelState(lChannelId, CDDDRM_CHANNEL_STATE_INACTIVE);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_INIT, lErrorId);
  }
#else
    CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
} /* CddDrm_Init() */

/**********************************************************************************************************************
 * CddDrm_ExternalTesterConnected()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_ExternalTesterConnected(boolean Present)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lReturnValue = E_OK;
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
#if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if module is uninitialized */
  if (CddDrm_ModuleInitialized == (uint8)CDDDRM_UNINIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
    lReturnValue = E_NOT_OK;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Drm have immediately go to TesterPresent mode and close all open connections */
    /* #20 Check if external tester is connected or disconnected */
    if (Present == TRUE)
    {
      CddDrm_Queue_TesterConnected = CDDDRM_QUEUE_EXTERNAL_TESTER_CONNECTED;
    }
    else
    {
      CddDrm_Queue_TesterConnected = CDDDRM_QUEUE_EXTERNAL_TESTER_DISCONNECTED;
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_EXTERNAL_TESTER_CONNECTED, lErrorId);
  }
# else
    CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
  return lReturnValue;
} /* CddDrm_ExternalTesterConnected() */

#if (CDDDRM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CDDDRM_CODE) CddDrm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CDDDRM_APPL_DATA) VersionInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = CDDDRM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  if (VersionInfo == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    VersionInfo->vendorID = (CDDDRM_VENDOR_ID);
    VersionInfo->moduleID = (CDDDRM_MODULE_ID);
    VersionInfo->sw_major_version = (CDDDRM_SW_MAJOR_VERSION);
    VersionInfo->sw_minor_version = (CDDDRM_SW_MINOR_VERSION);
    VersionInfo->sw_patch_version = (CDDDRM_SW_PATCH_VERSION);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void) Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_GET_VERSION_INFO, lErrorId);
  }
# else
    CDDDRM_DUMMY_STATEMENT(lErrorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif
}
#endif

#define CDDDRM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:
  MD_CDDDRM_8.7:
    Description: Rule 8.7
                 Objects shall be defined at block scope if they are only accessed from within a single function.
    Reason:      Vector style guide prevents usage of static variables in function scope
    Risk:        None.
    Prevention:  None.

   MD_CDDDRM_11.4:
     Description: Rule 11.4
                  A cast should not be performed between a pointer to object type and a different pointer to object
                  type.
     Reason:      Mechanism for Nv handling requires common pointer type
     Risk:        None. Pointers are initially aligned and only cast due to common API later on.
     Prevention:  Covered by descriptive pointer types and code review for correct usage.

   MD_CDDDRM_11.5:
     Description: Rule 11.5
                  A cast shall not be performed that removes any const or volatile qualification from the type
                  addressed by a pointer.
     Reason:      Common pointer type used that needs to be not constant for other usage.
     Risk:        None. Pointer to ROM value.
     Prevention:  Safe access of shared data is verified during code review.

   MD_CDDDRM_3206:
     Description: Rule 3206
     Reason:      Parameter is required by module that declares the function head.
     Risk:        None.
     Prevention:  None.
                  
   MD_CDDDRM_3229:
     Description: Rule 3229
     Reason:      Variable was not used in current version.
     Risk:        Poor readability of the code, unpredicted program flow.
     Prevention:  Program flow has been verified by component tests and review.
 */

/**********************************************************************************************************************
 *  END OF FILE: CddDrm.c
 *********************************************************************************************************************/