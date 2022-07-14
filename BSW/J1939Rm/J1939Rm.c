/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 Vector Informatik GmbH.                                                     All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*! \file   J1939Rm.c
 *  \brief  SAE J1939 Request Manager (J1939Rm)
 *
 *  \details  Implementation of the SAE J1939 Request Manager.
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
/**********************************************************************************************************************
 *  MISRA / PClint JUSTIFICATIONS
 *********************************************************************************************************************/
/* *INDENT-OFF* */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#define J1939RM_C

#include "J1939Rm.h"
#include "J1939Rm_Cbk.h"

#include "J1939Rm_Cfg.h"
#include "J1939Rm_Lcfg.h"
#include "J1939Rm_PBcfg.h"

#include "J1939Rm_Int.h"

#include "PduR_J1939Rm.h"
#if (J1939RM_SUPPORT_COM_USER == STD_ON)
# include "Com.h"
#endif

#include "SchM_J1939Rm.h"

#if (J1939RM_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
# include "EcuM_Error.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Vendor specific BSW module version information */
#define J1939RM_MAJOR_SOURCE_VERSION            (2u)
#define J1939RM_MINOR_SOURCE_VERSION            (2u)
#define J1939RM_PATCH_SOURCE_VERSION            (4u)

/* Vendor specific BSW generator version information */
#define J1939RM_MAJOR_GENERATOR_VERSION         (2u)
#define J1939RM_MINOR_GENERATOR_VERSION         (3u)

/* Check the vendor specific version of J1939Rm module header file */
#if ((J1939RM_SW_MAJOR_VERSION != J1939RM_MAJOR_SOURCE_VERSION) || (J1939RM_SW_MINOR_VERSION != J1939RM_MINOR_SOURCE_VERSION) || (J1939RM_SW_PATCH_VERSION != J1939RM_PATCH_SOURCE_VERSION))
# error "Vendor specific version numbers of J1939Rm.c and J1939Rm.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if ((J1939RM_CFG_MAJOR_VERSION != J1939RM_MAJOR_GENERATOR_VERSION) || (J1939RM_CFG_MINOR_VERSION != J1939RM_MINOR_GENERATOR_VERSION))
# error "Vendor specific version numbers of J1939Rm.c and J1939Rm_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Global module state */
#define J1939RM_UNINIT (0)
#define J1939RM_INIT (1)

#if !defined (J1939RM_LOCAL)
# define J1939RM_LOCAL static
#endif

#if !defined (J1939RM_LOCAL_INLINE)
# define J1939RM_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
/*======================================= J1939RM_START_SEC_VAR_ZERO_INIT_8BIT ======================================*/
#define J1939RM_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Initialization state of the J1399Rm */
J1939RM_LOCAL VAR(uint8, J1939RM_VAR_ZERO_INIT) J1939Rm_ModuleInitialized = J1939RM_UNINIT;

#define J1939RM_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*======================================== J1939RM_STOP_SEC_VAR_ZERO_INIT_8BIT ======================================*/
/**********************************************************************************************************************
 *  INTERNAL DATA
 *********************************************************************************************************************/
/*=================================== J1939RM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED ===================================*/
#define J1939RM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Post-build configuration data of the J1399Rm */
#if (J1939RM_USE_INIT_POINTER == STD_ON)
J1939RM_LOCAL P2CONST(J1939Rm_ConfigType, J1939RM_VAR_NOINIT, J1939RM_PBCFG) J1939Rm_ConfigDataPtr;
#endif

#define J1939RM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*==================================== J1939RM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED ===================================*/
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/*================================================ J1939RM_START_SEC_CODE ===========================================*/
#define J1939RM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ((J1939RM_SUPPORT_COM_USER == STD_ON) && (J1939RM_EXTIDUSAGEOFCOMIPDU == STD_ON))
/**********************************************************************************************************************
 *  J1939Rm_InternalCheckExtIdUsage
 *********************************************************************************************************************/
/*! \brief       Checks if the extended id bytes are used
 *  \details     -
 *  \param[in]   comIPduIdx     Index of the comIPdu.
 *  \param[in]   extIdInfo      Extended identifier bytes.
 *  \return      TRUE if the extended id bytes are used, FALSE if not.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_SUPPORT_COM_USER == STD_ON and 
                 J1939RM_EXTIDUSAGEOFCOMIPDU == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(boolean,
                          J1939RM_CODE) J1939Rm_InternalCheckExtIdUsage(J1939Rm_ComIPduIndType comIPduIdx,
                                                                        const J1939Rm_ExtIdInfoType * extIdInfo);
#endif

#if (J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalProcessRequestIndication
 *********************************************************************************************************************/
/*! \brief       Processes a received Request or Request2 PG.
 *  \details     -
 *  \param[in]   channelIdx        Internal index of the channel on which the Request or Request2 PG was received.
 *  \param[in]   nodeIdx           Index of the node
 *  \param[in]   userIdx           Internal index of the user
 *  \param[in]   requestedPgn      PGN of the requested PG.
 *  \param[in]   extIdInfo         Extended identifier bytes.
 *  \param[in]   sourceAddress     Source address of the received Request PG.
 *  \param[in]   destAddress       Address of the receiving node or 0xFF for broadcast.
 *  \param[in]   priority          Priority of the Request PG.
 *  \return      TRUE if a node is found for the requested PGN, FALSE if not.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(boolean,
                          J1939RM_CODE) J1939Rm_InternalProcessRequestIndication(J1939Rm_ChannelIdxOfRxPduType
                                                                                 channelIdx,
                                                                                 J1939Rm_NodeNodeIdxOfChannelNodeType
                                                                                 nodeIdx,
                                                                                 J1939Rm_UserUserIdxOfNodeUserType
                                                                                 userIdx, uint32 requestedPgn,
                                                                                 const J1939Rm_ExtIdInfoType *
                                                                                 extIdInfo, uint8 sourceAddress,
                                                                                 uint8 destAddress, uint8 priority);

#endif

#if (J1939RM_SUPPORT_COM_USER == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalProcessRequestComUserTriggerSend
 *********************************************************************************************************************/
/*! \brief       Trigger the IPDU send function for a COM user Request
 *  \details     -
 *  \param[in]   comIPduIdx          Internal index of the ComIPdu
 *  \param[in]   nodeAddr            Address of the node
 *  \param[in]   destAddress         Destination address of the received message
 *  \param[in]   sourceAddress       Source address of the received message
 *  \param[in]   priority            Priority of the received message
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_SUPPORT_COM_USER == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(void,
                          J1939RM_CODE)
J1939Rm_InternalProcessRequestComUserTriggerSend(J1939Rm_ComIPduIndType comIPduIdx, uint8 nodeAddr, uint8 destAddress,
                                                 uint8 sourceAddress, uint8 priority);
#endif

#if (J1939RM_SUPPORT_COM_USER == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalProcessRequestForComIPdu
 *********************************************************************************************************************/
/*! \brief       Processes a received COM user Request or Request2 PG.
 *  \details     -
 *  \param[in]   channelIdx        Internal index of the channel on which the Request or Request2 PG was received.
 *  \param[in]   userIdx           Internal index of the user
 *  \param[in]   requestedPgn      PGN of the requested PG.
 *  \param[in]   extIdInfo         Extended identifier bytes.
 *  \param[in]   nodeAddr          Address of the node
 *  \param[in]   sourceAddress     Source address of the received Request PG.
 *  \param[in]   destAddress       Address of the receiving node or 0xFF for broadcast.
 *  \param[in]   priority          Priority of the Request PG.
 *  \return      TRUE if a user is found for the requested PGN, FALSE if not.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_SUPPORT_COM_USER == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(boolean,
                          J1939RM_CODE) J1939Rm_InternalProcessRequestForComIPdu(J1939Rm_ChannelIdxOfRxPduType
                                                                                 channelIdx,
                                                                                 J1939Rm_UserUserIdxOfNodeUserType
                                                                                 userIdx, uint32 requestedPgn,
                                                                                 const J1939Rm_ExtIdInfoType *
                                                                                 extIdInfo, uint8 nodeAddr,
                                                                                 uint8 sourceAddress, uint8 destAddress,
                                                                                 uint8 priority);
#endif

#if (J1939RM_SUPPORT_ACK_INDICATION == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalCallAckIndication
 *********************************************************************************************************************/
/*! \brief       Trigger the Ack indication for a received Ack.
 *  \details     -
 *  \param[in]   channelIdx    Index of the channel.
 *  \param[in]   nodeIdx       Index of the node
 *  \param[in]   userIdx       Index of the user.
 *  \param[in]   extIdInfo     Extended Id information
 *  \param[in]   ackAddress    Ack address
 *  \param[in]   sa            Source address
 *  \param[in]   priority      Priority
 *  \param[in]   ackCode       Ack Code
 *  \param[in]   pgn           PGN of the received Ack.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_SUPPORT_ACK_INDICATION == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE)
J1939Rm_InternalCallAckIndication(J1939Rm_ChannelIdxOfRxPduType channelIdx,
                                  J1939Rm_NodeNodeIdxOfChannelNodeType nodeIdx,
                                  J1939Rm_UserUserIdxOfNodeUserType userIdx,
                                  J1939Rm_ExtIdInfoType extIdInfo,
                                  uint8 ackAddress, uint8 sa, uint8 priority, J1939Rm_AckCode ackCode, uint32 pgn);
#endif

#if (J1939RM_SUPPORT_ACK_INDICATION == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalCreateExtIdInfo
 *********************************************************************************************************************/
/*! \brief      Initialization of the ExtendedIdInfo struct for an received acknowledgment.
 *  \details     -
 *  \param[out]  extIdInfo      ExtendedIdInfo struct.
 *  \param[in]   extIdCode      Extended acknowledgment code.
 *  \param[in]   extId1         Extended Id Byte 1.
 *  \param[in]   extId2         Extended Id Byte 2.
 *  \param[in]   extId3         Extended Id Byte 3.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_SUPPORT_ACK_INDICATION == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalCreateExtIdInfo(J1939Rm_ExtIdInfoType* extIdInfo,
                                                                              uint8 extIdCode, uint8 extId1,
                                                                              uint8 extId2, uint8 extId3);
#endif

#if ((J1939RM_ACKQUEUEINDUSEDOFCHANNEL == STD_ON) || (J1939RM_SUPPORT_ACK_TRANSMISSION == STD_ON) || (J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON) || (J1939RM_SUPPORT_COM_USER == STD_ON))
/**********************************************************************************************************************
 *  J1939Rm_InternalTransmitAck
 *********************************************************************************************************************/
/*! \brief       Transmits or queues an Acknowledgment PG.
 *  \details     -
 *  \param[in]   channelIdx           Internal index of the channel on which the Acknowledgment PG shall be sent.
 *  \param[in]   ackPgn               Acknowledged PGN.
 *  \param[in]   ackCb                Acknowledgment control byte according to SAE J1939-21.
 *  \param[in]   extId1               Extended Id Byte 1.
 *  \param[in]   extId2               Extended Id Byte 2.
 *  \param[in]   extId3               Extended Id Byte 3.
 *  \param[in]   ackAddress           Address of the node that sent the Request.
 *  \param[in]   sourceAddress        Source address of the transmitted Acknowledgment PG.
 *  \param[in]   priority             Priority of the transmitted Acknowledgment PG.
 *  \param[in]   broadcast            Indicates whether the Acknowledgment PG is a response to a broadcast Request.
 *  \param[in]   nodeChannelStateIdx  Index of node state of a channel.
 *  \return      E_OK:                Acknowledgment PG was transmitted or queued.
 *  \return      E_NOT_OK:            Acknowledgment PG was discarded.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_ACKQUEUEINDUSEDOFCHANNEL == STD_ON or 
                 J1939RM_SUPPORT_ACK_TRANSMISSION == STD_ON or J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON or 
                 J1939RM_SUPPORT_COM_USER == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_InternalTransmitAck(J1939Rm_ChannelIterType channelIdx,
                                                                             uint32 ackPgn, uint8 ackCb, uint8 extId1,
                                                                             uint8 extId2, uint8 extId3,
                                                                             uint8 ackAddress, uint8 sourceAddress,
                                                                             uint8 priority, boolean broadcast,
                                                                             uint16 nodeChannelStateIdx);
#endif

#if (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalTransmitRequest
 *********************************************************************************************************************/
/*! \brief       Transmits or queues a Request PG.
 *  \details     -
 *  \param[in]   channelIdx           Internal index of the channel on which the Request PG shall be sent.
 *  \param[in]   requestedPgn         PGN of the requested PG.
 *  \param[in]   sourceAddress        Source address of the transmitted Request PG.
 *  \param[in]   destAddress          Address of the destination node or 0xFF for broadcast.
 *  \param[in]   priority             Priority of the Request PG.
 *  \param[in]   nodeChannelStateIdx  Index of node state of a channel.
 *  \return      E_OK:                Request PG was transmitted or queued.
 *  \return      E_NOT_OK:            Request PG was discarded.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_InternalTransmitRequest(J1939Rm_ChannelIterType channelIdx,
                                                                                 uint32 requestedPgn,
                                                                                 uint8 sourceAddress, uint8 destAddress,
                                                                                 uint8 priority,
                                                                                 uint16 nodeChannelStateIdx);
#endif

# if ((J1939RM_SUPPORT_REQUEST2 == STD_ON) && (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON))
/**********************************************************************************************************************
 *  J1939Rm_InternalTransmitRequest2
 *********************************************************************************************************************/
/*! \brief       Transmits or queues a Request2 PG.
 *  \details     -
 *  \param[in]   channelIdx           Internal index of the channel on which the Request PG shall be sent.
 *  \param[in]   requestedPgn         PGN of the requested PG.
 *  \param[in]   specInstr            Special instructions byte according to SAE J1939-21.
 *  \param[in]   extId1               First extended identifier byte.
 *  \param[in]   extId2               Second extended identifier byte.
 *  \param[in]   extId3               Third extended identifier byte.
 *  \param[in]   sourceAddress        Source address of the transmitted Request PG.
 *  \param[in]   destAddress          Address of the destination node or 0xFF for broadcast.
 *  \param[in]   priority             Priority of the Request PG.
 *  \param[in]   nodeChannelStateIdx  Index of node state of a channel.
 *  \return      E_OK:                Request PG was transmitted or queued,
 *  \return      E_NOT_OK:            Request PG was discarded.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON and
                 J1939RM_SUPPORT_REQUEST2 == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_InternalTransmitRequest2(J1939Rm_ChannelIterType channelIdx,
                                                                                  uint32 requestedPgn, uint8 specInstr,
                                                                                  uint8 extId1, uint8 extId2,
                                                                                  uint8 extId3, uint8 sourceAddress,
                                                                                  uint8 destAddress, uint8 priority,
                                                                                  uint16 nodeChannelStateIdx);
#endif

#if ((J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON) || (J1939RM_SUPPORT_COM_USER == STD_ON))
/**********************************************************************************************************************
 *  J1939Rm_InternalProcessRequest
 *********************************************************************************************************************/
/*! \brief       Processes a received Request or Request2 PG.
 *  \details     -
 *  \param[in]   channelIdx      Internal index of the channel on which the Request or Request2 PG was received.
 *  \param[in]   requestedPgn    PGN of the requested PG.
 *  \param[in]   extIdInfo       Extended identifier bytes.
 *  \param[in]   sourceAddress   Source address of the received Request PG.
 *  \param[in]   destAddress     Address of the receiving node or 0xFF for broadcast.
 *  \param[in]   priority        Priority of the Request PG.
 *  \param[in]   ackCb           Acknowledgment control byte according to SAE J1939-21, used if Request failed.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON or 
                 J1939RM_SUPPORT_COM_USER == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL FUNC(void, J1939RM_CODE) J1939Rm_InternalProcessRequest(J1939Rm_ChannelIdxOfRxPduType channelIdx,
                                                                      uint32 requestedPgn,
                                                                      const J1939Rm_ExtIdInfoType * extIdInfo,
                                                                      uint8 sourceAddress, uint8 destAddress,
                                                                      uint8 priority, uint8 ackCb);
#endif

#if ((J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON) || (J1939RM_SUPPORT_COM_USER == STD_ON))
/**********************************************************************************************************************
 *  J1939Rm_InternalReceiveRequest
 *********************************************************************************************************************/
/*! \brief       Indicates reception of a Request PG.
 *  \details     -
 *  \param[in]   channelIdx      Internal index of the channel on which the Request PG was received.
 *  \param[in]   PduInfoPtr      Contains the length (SduLength) of the received I-PDU and a pointer to a buffer
 *                               (SduDataPtr) containing the I-PDU and MetaData.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON or 
                 J1939RM_SUPPORT_COM_USER == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalReceiveRequest(J1939Rm_ChannelIdxOfRxPduType channelIdx,
                                                                             P2CONST(PduInfoType, AUTOMATIC,
                                                                                     J1939RM_APPL_DATA) PduInfoPtr);
#endif

#if (((J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON) || (J1939RM_SUPPORT_COM_USER == STD_ON)) && (J1939RM_SUPPORT_REQUEST2 == STD_ON))
/**********************************************************************************************************************
 *  J1939Rm_InternalReceiveRequest2
 *********************************************************************************************************************/
/*! \brief       Indicates reception of a Request2 PG.
 *  \details     -
 *  \param[in]   channelIdx      Internal index of the channel on which the Request2 PG was received.
 *  \param[in]   PduInfoPtr      Contains the length (SduLength) of the received I-PDU and a pointer to a buffer
 *                               (SduDataPtr) containing the I-PDU and MetaData.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON or 
                 J1939RM_SUPPORT_COM_USER == STD_ON) and J1939RM_SUPPORT_REQUEST2 == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalReceiveRequest2(J1939Rm_ChannelIdxOfRxPduType channelIdx,
                                                                              P2CONST(PduInfoType, AUTOMATIC,
                                                                                      J1939RM_APPL_DATA) PduInfoPtr);
#endif

#if (J1939RM_SUPPORT_ACK_INDICATION == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalProcessAck
 *********************************************************************************************************************/
/*! \brief       Process a received Acknowledgment.
 *  \details     -
 *  \param[in]   channelIdx      Internal index of the channel on which the Acknowledgment PG was received.
 *  \param[in]   PduInfoPtr      Contains the length (SduLength) of the received I-PDU and a pointer to a buffer
 *                               (SduDataPtr) containing the I-PDU and MetaData.
 *  \param[in]   ackCode         Acknowledgment code.
 *  \param[in]   extIdCode       Extended Id code.
 *  \param[in]   pgn             PGN that was the Acknowledgment send for.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_SUPPORT_ACK_INDICATION == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalProcessAck(J1939Rm_ChannelIdxOfRxPduType channelIdx,
                                                                         P2CONST(PduInfoType, AUTOMATIC,
                                                                                 J1939RM_APPL_DATA) PduInfoPtr,
                                                                         J1939Rm_AckCode ackCode, uint8 extIdCode,
                                                                         uint32 pgn);
#endif

#if (J1939RM_SUPPORT_ACK_INDICATION == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalReceiveAck
 *********************************************************************************************************************/
/*! \brief       Indicates the reception of an Acknowledgment PG.
 *  \details     -
 *  \param[in]   channelIdx      Internal index of the channel on which the Acknowledgment PG was received.
 *  \param[in]   PduInfoPtr      Contains the length (SduLength) of the received I-PDU and a pointer to a buffer
 *                               (SduDataPtr) containing the I-PDU and MetaData.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_SUPPORT_ACK_INDICATION == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalReceiveAck(J1939Rm_ChannelIdxOfRxPduType channelIdx,
                                                                         P2CONST(PduInfoType, AUTOMATIC,
                                                                                 J1939RM_APPL_DATA) PduInfoPtr);
#endif

#if (J1939RM_ACKQUEUEINDUSEDOFCHANNEL == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalProcessAckQueue
 *********************************************************************************************************************/
/*! \brief       Send next element from the Acknowledgement queue.
 *  \details     -
 *  \param[in]   channelIdx      Internal index of the channel on which the Acknowledgment PG shall be sent.
 *  \param[in]   serviceId       ID of the service that called this API.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_ACKQUEUEINDUSEDOFCHANNEL == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL FUNC(void, J1939RM_CODE) J1939Rm_InternalProcessAckQueue(J1939Rm_ChannelIterType channelIdx,
                                                                       uint8 serviceId);
#endif

#if ((J1939RM_REQQUEUEINDUSEDOFCHANNEL == STD_ON) && (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON))
/**********************************************************************************************************************
 *  J1939Rm_InternalProcessRqstQueue
 *********************************************************************************************************************/
/*! \brief       Send next element from the Request queue.
 *  \details     -
 *  \param[in]   channelIdx      Internal index of the channel on which the Request PG shall be sent.
 *  \param[in]   serviceId       ID of the service that called this API.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_REQQUEUEINDUSEDOFCHANNEL == STD_ON and 
                 J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL FUNC(void, J1939RM_CODE) J1939Rm_InternalProcessRqstQueue(J1939Rm_ChannelIterType channelIdx,
                                                                        uint8 serviceId);
#endif

#if ((J1939RM_REQ2QUEUEINDUSEDOFCHANNEL == STD_ON) && (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON) && (J1939RM_SUPPORT_REQUEST2 == STD_ON))
/**********************************************************************************************************************
 *  J1939Rm_InternalProcessRqst2Queue
 *********************************************************************************************************************/
/*! \brief       Send next element from the Request2 queue.
 *  \details     -
 *  \param[in]   channelIdx      Internal index of the channel on which the Request2 PG shall be sent.
 *  \param[in]   serviceId       ID of the service that called this API.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_REQ2QUEUEINDUSEDOFCHANNEL == STD_ON and 
                 J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON and J1939RM_SUPPORT_REQUEST2 == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL FUNC(void, J1939RM_CODE) J1939Rm_InternalProcessRqst2Queue(J1939Rm_ChannelIterType channelIdx,
                                                                         uint8 serviceId);
#endif

/**********************************************************************************************************************
 *  J1939Rm_InternalInit
 *********************************************************************************************************************/
/*! \brief       Internal funtion to initialize the J1939 Request Manager.
 *  \details     -
 *  \pre         The module must be in the uninitialized state.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalInit(void);

/**********************************************************************************************************************
 *  J1939Rm_InternalMainFunction
 *********************************************************************************************************************/
/*! \brief       Internal main function of the J1939 Request Manager.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalMainFunction(void);

#if (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalSendRequest
 *********************************************************************************************************************/
/*! \brief       Internal function to request the transmission of a Request or Request2 PG.
 *  \details     -
 *  \param[in]   userId          Identification of the calling module.
 *  \param[in]   channel         Channel on which the Request shall be sent.
 *  \param[in]   requestedPgn    PGN of the requested PG.
 *  \param[in]   extIdInfo       Extended identifier bytes.
 *  \param[in]   destAddress     Address of the destination node or 0xFF for broadcast.
 *  \param[in]   priority        Priority of the Request PG.
 *  \param[in]   checkTimeout    TRUE: Timeout supervision will be performed,
 *                               FALSE: No timeout supervision will be started
 *  \return      E_OK:           Transmission request is accepted.
 *  \return      E_NOT_OK:       Transmission request is not accepted.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_InternalSendRequest(uint8 userId,
                                                                                    NetworkHandleType channel,
                                                                                    uint32 requestedPgn,
                                                                                    P2CONST(J1939Rm_ExtIdInfoType,
                                                                                            AUTOMATIC,
                                                                                            J1939RM_APPL_DATA)
                                                                                    extIdInfo, uint8 destAddress,
                                                                                    uint8 priority,
                                                                                    boolean checkTimeout);
#endif

#if (J1939RM_SUPPORT_ACK_TRANSMISSION == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalSendAck
 *********************************************************************************************************************/
/*! \brief       Internal function to Request the transmission of an Acknowledgment PG.
 *  \details     -
 *  \param[in]   userId          Identification of the calling module.
 *  \param[in]   channel         Channel on which the acknowledgment shall be sent.
 *  \param[in]   ackPgn          Acknowledged PGN.
 *  \param[in]   extIdInfo       Extended identifier bytes.
 *  \param[in]   ackCode         Type of acknowledgment, see definition of J1939Rm_AckCode for available codes.
 *  \param[in]   ackAddress      Address of the node that sent the Request.
 *  \param[in]   priority        Priority of the transmitted Acknowledgment PG.
 *  \param[in]   broadcast       Indicates whether the Acknowledgment PG is a response to a broadcast Request.
 *  \return      E_OK:           Transmission request is accepted,
 *  \return      E_NOT_OK:       Transmission request is not accepted.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if J1939RM_SUPPORT_ACK_TRANSMISSION == STD_ON.
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_InternalSendAck(uint8 userId, NetworkHandleType channel,
                                                                                uint32 ackPgn,
                                                                                P2CONST(J1939Rm_ExtIdInfoType,
                                                                                        AUTOMATIC,
                                                                                        J1939RM_APPL_DATA) extIdInfo,
                                                                                J1939Rm_AckCode ackCode,
                                                                                uint8 ackAddress, uint8 priority,
                                                                                boolean broadcast);
#endif

/**********************************************************************************************************************
 *  J1939Rm_InternalSetState
 *********************************************************************************************************************/
/*! \brief       Internal function to change the communication state of J1939Rm to offline or online.
 *  \details     -
 *  \param[in]   channel         Channel for which the state shall be changed.
 *  \param[in]   node            Node for which the state shall be changed.
 *  \param[in]   newState        New state the J1939Rm shall enter, see definition of J1939Rm_StateType
 *                               for available states.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalSetState(NetworkHandleType channel, uint8 node,
                                                                       J1939Rm_StateType newState);

/**********************************************************************************************************************
 *  J1939Rm_InternalRxIndication
 *********************************************************************************************************************/
/*! \brief       Internal function that indicates the reception of Pdus
 *  \details     -
 *  \param[in]   RxPduId      Id of the Pdu
 *  \param[in]   PduInfoPtr   Contains the length (SduLength) of the received I-PDU and a pointer to a buffer
 *                            (SduDataPtr) containing the I-PDU and MetaData.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalRxIndication(PduIdType RxPduId,
                                                                           P2CONST(PduInfoType, AUTOMATIC,
                                                                                   J1939RM_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  J1939Rm_InternalTxConfirmation
 *********************************************************************************************************************/
/*! \brief       Internal function that confirms the successful transmission of a J1939Rm PDU by the PduR.
 *  \details     -
 *  \param[in]   TxPduId         The id of the transmitted PDU
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalTxConfirmation(PduIdType TxPduId);

#define J1939RM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*================================================ J1939RM_STOP_SEC_CODE ============================================*/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/*================================================ J1939RM_START_SEC_CODE ===========================================*/
#define J1939RM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ((J1939RM_SUPPORT_COM_USER == STD_ON) && (J1939RM_EXTIDUSAGEOFCOMIPDU == STD_ON))
/**********************************************************************************************************************
 *  J1939Rm_InternalCheckExtIdUsage
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
J1939RM_LOCAL_INLINE FUNC(boolean,
                          J1939RM_CODE) J1939Rm_InternalCheckExtIdUsage(J1939Rm_ComIPduIndType comIPduIdx,
                                                                        const J1939Rm_ExtIdInfoType * extIdInfo)
{
  boolean extIdMatch = FALSE;

  /* #10 Check if the extended id bytes of the received Request match the configured extended id bytes for the ComIPdu. */
  switch (extIdInfo->ExtIdType)
  {
  case J1939RM_EXTID_NONE:
    if (J1939Rm_GetExtIdUsageOfComIPdu(comIPduIdx) == J1939RM_EXT_ID_NONE_EXTIDUSAGEOFCOMIPDU)
    {
      extIdMatch = TRUE;
    }
    break;

  case J1939RM_EXTID_ONE:
    if ((J1939Rm_GetExtIdUsageOfComIPdu(comIPduIdx) == J1939RM_EXT_ID_ONE_EXTIDUSAGEOFCOMIPDU) &&
        (extIdInfo->ExtId1 == J1939Rm_GetExtId1OfComIPdu(comIPduIdx)))
    {
      extIdMatch = TRUE;
    }
    break;

  case J1939RM_EXTID_TWO:
    if ((J1939Rm_GetExtIdUsageOfComIPdu(comIPduIdx) == J1939RM_EXT_ID_TWO_EXTIDUSAGEOFCOMIPDU) &&
        (extIdInfo->ExtId1 == J1939Rm_GetExtId1OfComIPdu(comIPduIdx)) &&
        (extIdInfo->ExtId2 == J1939Rm_GetExtId2OfComIPdu(comIPduIdx)))
    {
      extIdMatch = TRUE;
    }
    break;

  case J1939RM_EXTID_THREE:
    if ((J1939Rm_GetExtIdUsageOfComIPdu(comIPduIdx) == J1939RM_EXT_ID_THREE_EXTIDUSAGEOFCOMIPDU)
        && (extIdInfo->ExtId1 == J1939Rm_GetExtId1OfComIPdu(comIPduIdx))
        && (extIdInfo->ExtId2 == J1939Rm_GetExtId2OfComIPdu(comIPduIdx))
        && (extIdInfo->ExtId3 == J1939Rm_GetExtId3OfComIPdu(comIPduIdx)))
    {
      extIdMatch = TRUE;
    }
    break;

  default: /* MISRA */
    break;
  }

  return extIdMatch;
} /* J1939Rm_InternalCheckExtIdUsage() */
#endif

#if (J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalProcessRequestIndication
 *********************************************************************************************************************/
/*!
 *
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
J1939RM_LOCAL_INLINE FUNC(boolean,
                          J1939RM_CODE) J1939Rm_InternalProcessRequestIndication(J1939Rm_ChannelIdxOfRxPduType
                                                                                 channelIdx,
                                                                                 J1939Rm_NodeNodeIdxOfChannelNodeType
                                                                                 nodeIdx,
                                                                                 J1939Rm_UserUserIdxOfNodeUserType
                                                                                 userIdx, uint32 requestedPgn,
                                                                                 const J1939Rm_ExtIdInfoType *
                                                                                 extIdInfo, uint8 sourceAddress,
                                                                                 uint8 destAddress, uint8 priority)
{
  boolean found = FALSE;

  /* #10 If the request indication is configured for the given user and the user is of the type
   *      'J1939RM_USER_J1939NM_USERTYPEOFUSER' or the source address of the received Request is not the NULL address: */
  if (J1939Rm_IsUserRequestIndicationOfUser(userIdx) &&
      ((J1939Rm_GetUserTypeOfUser(userIdx) == J1939RM_USER_J1939NM_USERTYPEOFUSER) ||
       (sourceAddress != J1939RM_NULL_ADDRESS)))
  {
    J1939Rm_UserPGNIndIterType reqPgnIdx;

    /* #20 Go through the list of configured Request PGNs for the given user: */
    for (reqPgnIdx = J1939Rm_GetUserPGNIndStartIdxOfUser(userIdx);
         reqPgnIdx < J1939Rm_GetUserPGNIndEndIdxOfUser(userIdx); reqPgnIdx++)
    {
      /* #30 If the PGN of the received Request is in the list: */
      if (requestedPgn == (uint32) J1939Rm_GetPGNOfUserPGN(J1939Rm_GetUserPGNInd(reqPgnIdx)))
      {
        found = TRUE;

# if (J1939RM_RTEREQUESTINDICATION == STD_ON)
        /* #40 If the RTE request indication is configured and it is an RTE user: \trace SPEC-2167408 */
        if (J1939Rm_GetUserTypeOfUser(userIdx) == J1939RM_USER_RTE_USERTYPEOFUSER)
        {
          /* #50 Use the RTE API to inform the user about the received Request. */
          (void) J1939Rm_GetRteRequestIndication(J1939Rm_GetRteRequestIndicationIdxOfUser(userIdx))
            ((uint8) J1939Rm_GetNodeIdOfNode(nodeIdx), J1939Rm_GetComMChannelIdOfChannel(channelIdx),
             requestedPgn, extIdInfo, sourceAddress, destAddress, priority);
        }
# endif
# if ((J1939RM_REQUESTINDICATION == STD_ON) && (J1939RM_RTEREQUESTINDICATION == STD_ON))
        else
# endif
        /* #60 If it is not an RTE user: \trace SPEC-2167407 */
# if (J1939RM_REQUESTINDICATION == STD_ON)
        {
          /* #70 Use the request indication API of the user to inform the user about the received Request. */
          J1939Rm_GetRequestIndication(J1939Rm_GetRequestIndicationIdxOfUser(userIdx))
            ((uint8) J1939Rm_GetNodeIdOfNode(nodeIdx), J1939Rm_GetComMChannelIdOfChannel(channelIdx),
             requestedPgn, extIdInfo, sourceAddress, destAddress, priority);
        }
# endif
      }
    }
  }

  return found;
} /* J1939Rm_InternalProcessRequestIndication() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif

#if (J1939RM_SUPPORT_COM_USER == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalProcessRequestComUserTriggerSend
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
J1939RM_LOCAL_INLINE FUNC(void,
                          J1939RM_CODE)
J1939Rm_InternalProcessRequestComUserTriggerSend(J1939Rm_ComIPduIndType comIPduIdx, uint8 nodeAddr, uint8 destAddress,
                                                 uint8 sourceAddress, uint8 priority)
{
  /* #10 Calculate the meta data for the requested PG. */
  uint8 metaData[J1939RM_MDL_SIZE];

  metaData[J1939RM_MDL_SA_POS] = nodeAddr;
  if (destAddress == J1939RM_BROADCAST_ADDRESS)
  {
    metaData[J1939RM_MDL_DA_POS] = J1939RM_BROADCAST_ADDRESS;
  }
  else
  {
    metaData[J1939RM_MDL_DA_POS] = sourceAddress;
  }
  metaData[J1939RM_MDL_PRI_POS] = J1939Rm_Make_Priority(priority);

  /* #20 Call the COM send API with meta data. \trace SPEC-2167409 */
  Com_TriggerIPDUSendWithMetaData(J1939Rm_GetPduIdOfComIPdu(comIPduIdx), metaData);
} /* J1939Rm_InternalProcessRequestComUserTriggerSend() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
 *  J1939Rm_InternalProcessRequestForComIPdu
 *********************************************************************************************************************/
/*!
 *
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
J1939RM_LOCAL_INLINE FUNC(boolean,
                          J1939RM_CODE) J1939Rm_InternalProcessRequestForComIPdu(J1939Rm_ChannelIdxOfRxPduType
                                                                                 channelIdx,
                                                                                 J1939Rm_UserUserIdxOfNodeUserType
                                                                                 userIdx, uint32 requestedPgn,
                                                                                 const J1939Rm_ExtIdInfoType *
                                                                                 extIdInfo, uint8 nodeAddr,
                                                                                 uint8 sourceAddress, uint8 destAddress,
                                                                                 uint8 priority)
{
  boolean found = FALSE;

  /* #10 If the source address for the requested PG in valid: */
  if (sourceAddress != J1939RM_NULL_ADDRESS)
  {
    J1939Rm_ComIPduIndIterType userComIPduIdx;

    /* #20 Search for the requested PGN in the list of requestable PGNs (ComIPdus) of the given user: */
    for (userComIPduIdx = J1939Rm_GetComIPduIndStartIdxOfUser(userIdx);
         userComIPduIdx < J1939Rm_GetComIPduIndEndIdxOfUser(userIdx); userComIPduIdx++)
    {
      J1939Rm_ComIPduIndType comIPduIdx = J1939Rm_GetComIPduInd(userComIPduIdx);

      /* #30 If the current evaluated ComIPdu is configured for the given channel to send the requested PG and the
       *     ComIPdu has the requested PGN: */
      if ((channelIdx == J1939Rm_GetChannelIdxOfComIPdu(comIPduIdx)) &&
          (requestedPgn == (uint32) J1939Rm_GetPGNOfComIPdu(comIPduIdx)))
      {
# if (J1939RM_EXTIDUSAGEOFCOMIPDU == STD_ON)
        boolean extIdMatch = J1939Rm_InternalCheckExtIdUsage(comIPduIdx, extIdInfo);

        /* #40 If the extended id bytes from the Request and the current evaluated ComIPdu match: */
        if (extIdMatch) /* PRQA S 3344 */ /* MD_J1939Rm_13.2 */
# else
        if (extIdInfo->ExtIdType == J1939RM_EXTID_NONE)
# endif
        {
          uint8 pduDa = J1939Rm_GetDAOfComIPdu(comIPduIdx);

          /* #50 If the destination of the PDU is the null address, it is either freely choosable or immutable but OK
           *     (PDU2) - in the other cases, it must match the type and the source address of the Request: */
          if ((pduDa == J1939RM_NULL_ADDRESS) ||
              ((destAddress == J1939RM_BROADCAST_ADDRESS) && (pduDa == J1939RM_BROADCAST_ADDRESS)) ||
              ((destAddress != J1939RM_BROADCAST_ADDRESS) && (pduDa == sourceAddress)))
          {
            found = TRUE;

            /* #60 Use the COM API to send the requested PG message. */
            J1939Rm_InternalProcessRequestComUserTriggerSend(comIPduIdx, nodeAddr, destAddress, sourceAddress,
                                                             priority);

            break;
          }
        }
      }
    }
  }

  return found;
} /* J1939Rm_InternalProcessRequestForComIPdu() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif

#if (J1939RM_SUPPORT_ACK_INDICATION == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalCallAckIndication
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
J1939RM_LOCAL_INLINE FUNC(void,
                          J1939RM_CODE) J1939Rm_InternalCallAckIndication(J1939Rm_ChannelIdxOfRxPduType channelIdx,
                                                                          J1939Rm_NodeNodeIdxOfChannelNodeType nodeIdx,
                                                                          J1939Rm_UserUserIdxOfNodeUserType userIdx,
                                                                          J1939Rm_ExtIdInfoType extIdInfo,
                                                                          uint8 ackAddress, uint8 sa, uint8 priority,
                                                                          J1939Rm_AckCode ackCode, uint32 pgn)
{
  J1939Rm_UserAckPGNIndIterType ackPgnIdx;

  /* #10 Go through the list of all configured acknowledgment PGNs for the given user: */
  for (ackPgnIdx = J1939Rm_GetUserAckPGNIndStartIdxOfUser(userIdx);
       ackPgnIdx < J1939Rm_GetUserAckPGNIndEndIdxOfUser(userIdx); ackPgnIdx++)
  {
    /* #20 If the PGN of the received acknowledgment is in the list: */
    if (pgn == (uint32) J1939Rm_GetAckPGNOfUserAckPGN(J1939Rm_GetUserAckPGNInd(ackPgnIdx)))
    {
# if (J1939RM_RTEACKINDICATION == STD_ON)
      /* #30 If the given user is an RTE user: */
      if (J1939Rm_GetUserTypeOfUser(userIdx) == J1939RM_USER_RTE_USERTYPEOFUSER)
      {
        /* #40 Use the ack indication service API to inform the user about the received acknowledgment. \trace SPEC-2167424 */
        (void) J1939Rm_GetRteAckIndication(J1939Rm_GetRteAckIndicationIdxOfUser(userIdx))
          ((uint8) J1939Rm_GetNodeIdOfNode(nodeIdx), J1939Rm_GetComMChannelIdOfChannel(channelIdx), pgn,
           &extIdInfo, ackCode, ackAddress, sa, priority);
      }
# endif
# if ((J1939RM_ACKINDICATION == STD_ON) && (J1939RM_RTEACKINDICATION == STD_ON))
      else
# endif
        /* #50 If it is not an RTE user: */
# if (J1939RM_ACKINDICATION == STD_ON)
      {
        /* #60 Use the ack indication API of the user to inform the user about the received acknowledgment. \trace SPEC-2167423 */
        J1939Rm_GetAckIndication(J1939Rm_GetAckIndicationIdxOfUser(userIdx))
          ((uint8) J1939Rm_GetNodeIdOfNode(nodeIdx), J1939Rm_GetComMChannelIdOfChannel(channelIdx), pgn,
           &extIdInfo, ackCode, ackAddress, sa, priority);
      }
# endif
      break;
    }
  }
} /* J1939Rm_InternalCallAckIndication() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
 *  J1939Rm_InternalCreateExtIdInfo
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
J1939RM_LOCAL_INLINE FUNC(void,
                          J1939RM_CODE) J1939Rm_InternalCreateExtIdInfo(J1939Rm_ExtIdInfoType* extIdInfo,
                                                                        uint8 extIdCode, uint8 extId1, uint8 extId2,
                                                                        uint8 extId3)
{
  extIdInfo->ExtId1 = extId1;
  extIdInfo->ExtId2 = extId2;
  extIdInfo->ExtId3 = extId3;

  /* #10 Initialize the extended id info struct with the extended id info type of the received Acknowledgment. */
  switch (extIdCode)
  {
  case J1939RM_ACKM_CODE_EXTID_THREE:
    extIdInfo->ExtIdType = J1939RM_EXTID_THREE;
    break;

  case J1939RM_ACKM_CODE_EXTID_TWO:
    extIdInfo->ExtIdType = J1939RM_EXTID_TWO;
    break;

  case J1939RM_ACKM_CODE_EXTID_ONE:
    extIdInfo->ExtIdType = J1939RM_EXTID_ONE;
    break;

  case J1939RM_ACKM_CODE_EXTID_NONE:
  default:
    extIdInfo->ExtIdType = J1939RM_EXTID_NONE;
    break;
  }
} /* J1939Rm_InternalCreateExtIdInfo() */
#endif

#if ((J1939RM_ACKQUEUEINDUSEDOFCHANNEL == STD_ON) || (J1939RM_SUPPORT_ACK_TRANSMISSION == STD_ON) || (J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON) || (J1939RM_SUPPORT_COM_USER == STD_ON))
/**********************************************************************************************************************
 *  J1939Rm_InternalTransmitAck
 *********************************************************************************************************************/
/*!
 *
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
J1939RM_LOCAL FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_InternalTransmitAck(J1939Rm_ChannelIterType channelIdx,
                                                                             uint32 ackPgn, uint8 ackCb, uint8 extId1,
                                                                             uint8 extId2, uint8 extId3,
                                                                             uint8 ackAddress, uint8 sourceAddress,
                                                                             uint8 priority, boolean broadcast,
                                                                             uint16 nodeChannelStateIdx)
{
  Std_ReturnType result = E_NOT_OK;

  J1939Rm_TxPduAckmIdxOfChannelType txPduIdx = J1939Rm_GetTxPduAckmIdxOfChannel(channelIdx);
  J1939Rm_TxPduStateType *pduState = &J1939Rm_GetTxPduState(J1939Rm_GetTxPduStateIdxOfTxPdu(txPduIdx));

  J1939RM_DUMMY_STATEMENT(broadcast); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  J1939RM_DUMMY_STATEMENT(nodeChannelStateIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  /* #10 Enter J1939RM_EXCLUSIVE_AREA_TXPDULOCK. */
  SchM_Enter_J1939Rm_J1939RM_EXCLUSIVE_AREA_TXPDULOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Check if the Pdu to use is free: */
  if (!pduState->Locked) /* PRQA S 3344 */ /* MD_J1939Rm_13.2 */
  {
    /* #30 Prepare the Pdu with the data to transmit. */
    uint8 data[J1939RM_ACKM_PDU_LENGTH];
    PduInfoType pdu;

    pduState->ConfTimeout = J1939Rm_GetTxConfTimeout();
    pduState->Locked = TRUE;

    /* #40 Leave J1939RM_EXCLUSIVE_AREA_TXPDULOCK. */
    SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_TXPDULOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    pdu.SduDataPtr = data;
    pdu.SduLength = J1939RM_ACKM_PDU_LENGTH;

    data[J1939RM_ACKM_CB] = ackCb;
    data[J1939RM_ACKM_GF1] = extId1;
    data[J1939RM_ACKM_GF2] = extId2;
    data[J1939RM_ACKM_GF3] = extId3;
    data[J1939RM_ACKM_ADDR] = ackAddress;
    data[J1939RM_ACKM_PGN_LO] = J1939Rm_Make_PGN_LO(ackPgn);
    data[J1939RM_ACKM_PGN_MD] = J1939Rm_Make_PGN_MD(ackPgn);
    data[J1939RM_ACKM_PGN_HI] = J1939Rm_Make_PGN_HI(ackPgn);
    data[J1939RM_ACKM_SA_POS] = sourceAddress;
# if (J1939RM_ISOBUSCHANNELOFCHANNEL == STD_ON)
    if (J1939Rm_IsIsobusChannelOfChannel(channelIdx) && (!broadcast))
    {
      data[J1939RM_ACKM_DA_POS] = ackAddress;
    }
    else
# endif
    {
      data[J1939RM_ACKM_DA_POS] = J1939RM_BROADCAST_ADDRESS;
    }
    data[J1939RM_ACKM_PRI_POS] = J1939Rm_Make_Priority(priority);

    /* #50 Call the PduR transmit function. \trace SPEC-2167411 */
    result = PduR_J1939RmTransmit(J1939Rm_GetPduIdOfTxPdu(txPduIdx), &pdu);

    /* #60 If the transmit was successful report the transmitted Acknowledgment internal, 
     *     otherwise release the PDU lock. */
    if (result == E_OK)
    {
# if (J1939RM_ACKINDICATION == STD_ON)
      J1939Rm_RxIndication((PduIdType) J1939Rm_GetRxPduAckmIdxOfChannel(channelIdx), &pdu);
# endif
    }
    else
    {
      pduState->Locked = FALSE;
    }
  }
  /* #70 Otherwise, leave J1939RM_EXCLUSIVE_AREA_TXPDULOCK. */
  else
  {
    SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_TXPDULOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  /* #80 If the transmit has failed and the queuing of Acknowledgments is enabled: */
# if (J1939RM_ACKQUEUEINDUSEDOFCHANNEL == STD_ON)
  if (result != E_OK)
  {
    /* #90 Enter J1939RM_EXCLUSIVE_AREA_ACKQUEUELOCK. */
    SchM_Enter_J1939Rm_J1939RM_EXCLUSIVE_AREA_ACKQUEUELOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #100 If there is a free element in the Acknowledgment queue: */
    if (J1939Rm_GetChannelState(channelIdx).AckQueueFree > 0)
    {
      /* #110 Get an element of the Acknowledgment queue and set the data to transmit. */
      J1939Rm_AckQueueElementType *elem = &J1939Rm_GetAckQueue(J1939Rm_GetChannelState(channelIdx).AckQueueWrite);

      elem->AckPgn = ackPgn;
      elem->AckCb = ackCb;
      elem->ExtId1 = extId1;
      elem->ExtId2 = extId2;
      elem->ExtId3 = extId3;
      elem->AckAddress = ackAddress;
      elem->SourceAddress = sourceAddress;
      elem->Priority = priority;
      elem->Broadcast = broadcast;
      elem->NodeChannelStateIndex = nodeChannelStateIdx;

      /* #120 If the queue is full now, handle queue pointer wrap around. */
      J1939Rm_GetChannelState(channelIdx).AckQueueWrite++;

      if (J1939Rm_GetChannelState(channelIdx).AckQueueWrite == J1939Rm_GetAckQueueIndEndIdxOfChannel(channelIdx))
      {
        J1939Rm_GetChannelState(channelIdx).AckQueueWrite = J1939Rm_GetAckQueueIndStartIdxOfChannel(channelIdx);
      }
      J1939Rm_GetChannelState(channelIdx).AckQueueFree--;

      result = E_OK;
    }

    /* #130 Leave J1939RM_EXCLUSIVE_AREA_ACKQUEUELOCK. */
    SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_ACKQUEUELOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
# endif

  return result;
} /* J1939Rm_InternalTransmitAck() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */
#endif

#if (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalTransmitRequest
 *********************************************************************************************************************/
/*!
 *
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
J1939RM_LOCAL FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_InternalTransmitRequest(J1939Rm_ChannelIterType channelIdx,
                                                                                 uint32 requestedPgn,
                                                                                 uint8 sourceAddress, uint8 destAddress,
                                                                                 uint8 priority,
                                                                                 uint16 nodeChannelStateIdx)
{
  Std_ReturnType result = E_NOT_OK;

  J1939Rm_TxPduRqstIdxOfChannelType txPduIdx = J1939Rm_GetTxPduRqstIdxOfChannel(channelIdx);
  J1939Rm_TxPduStateType *pduState = &J1939Rm_GetTxPduState(J1939Rm_GetTxPduStateIdxOfTxPdu(txPduIdx));

  J1939RM_DUMMY_STATEMENT(nodeChannelStateIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  /* #10 Enter J1939RM_EXCLUSIVE_AREA_TXPDULOCK. */
  SchM_Enter_J1939Rm_J1939RM_EXCLUSIVE_AREA_TXPDULOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Check if the Pdu to use is free: */
  if (!pduState->Locked) /* PRQA S 3344 */ /* MD_J1939Rm_13.2 */
  {
    /* #30 Prepare the Pdu with the data to transmit. */
    uint8 data[J1939RM_RQST_PDU_LENGTH];
    PduInfoType pdu;

    pduState->ConfTimeout = J1939Rm_GetTxConfTimeout();
    pduState->Locked = TRUE;

    /* #40 Leave J1939RM_EXCLUSIVE_AREA_TXPDULOCK. */
    SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_TXPDULOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    pdu.SduDataPtr = data;
    pdu.SduLength = J1939RM_RQST_PDU_LENGTH;

    data[J1939RM_RQST_PGN_LO] = J1939Rm_Make_PGN_LO(requestedPgn);
    data[J1939RM_RQST_PGN_MD] = J1939Rm_Make_PGN_MD(requestedPgn);
    data[J1939RM_RQST_PGN_HI] = J1939Rm_Make_PGN_HI(requestedPgn);
    data[J1939RM_RQST_SA_POS] = sourceAddress;
    data[J1939RM_RQST_DA_POS] = destAddress;
    data[J1939RM_RQST_PRI_POS] = J1939Rm_Make_Priority(priority);

    /* #50 Call the PduR transmit function. */
    result = PduR_J1939RmTransmit(J1939Rm_GetPduIdOfTxPdu(txPduIdx), &pdu);

    /* #60 If the transmit was successful, report the transmitted request internal,
     *     otherwise release the PDU lock. \trace SPEC-2167420 */
    if (result == E_OK)
    {
      J1939Rm_RxIndication((PduIdType) J1939Rm_GetRxPduRqstIdxOfChannel(channelIdx), &pdu);
    }
    else
    {
      pduState->Locked = FALSE;
    }
  }
  /* #70 Otherwise, leave J1939RM_EXCLUSIVE_AREA_TXPDULOCK. */
  else
  {
    SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_TXPDULOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  /* #80 If the transmit has failed and the queuing of Requests is enabled: */
# if (J1939RM_REQQUEUEINDUSEDOFCHANNEL == STD_ON)
  if (result != E_OK)
  {
    /* #90 Enter J1939RM_EXCLUSIVE_AREA_REQQUEUELOCK. */
    SchM_Enter_J1939Rm_J1939RM_EXCLUSIVE_AREA_REQQUEUELOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #100 If there is an free element in the acknowledgment queue: */
    if (J1939Rm_GetChannelState(channelIdx).ReqQueueFree > 0)
    {
      /* #110 Get an element of the Request queue and set the data to transmit. */
      J1939Rm_ReqQueueElementType *elem = &J1939Rm_GetReqQueue(J1939Rm_GetChannelState(channelIdx).ReqQueueWrite);

      elem->RequestedPgn = requestedPgn;
      elem->SourceAddress = sourceAddress;
      elem->DestAddress = destAddress;
      elem->Priority = priority;
      elem->NodeChannelStateIndex = nodeChannelStateIdx;

      /* #120 If the queue is full now, handle queue pointer wrap around. */
      J1939Rm_GetChannelState(channelIdx).ReqQueueWrite++;
      if (J1939Rm_GetChannelState(channelIdx).ReqQueueWrite == J1939Rm_GetReqQueueIndEndIdxOfChannel(channelIdx))
      {
        J1939Rm_GetChannelState(channelIdx).ReqQueueWrite = J1939Rm_GetReqQueueIndStartIdxOfChannel(channelIdx);
      }
      J1939Rm_GetChannelState(channelIdx).ReqQueueFree--;

      result = E_OK;
    }

    /* #130 Leave J1939RM_EXCLUSIVE_AREA_REQQUEUELOCK. */
    SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_REQQUEUELOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
# endif

  return result;
} /* J1939Rm_InternalTransmitRequest() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */

# if (J1939RM_SUPPORT_REQUEST2 == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalTransmitRequest2
 *********************************************************************************************************************/
/*!
 *
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
J1939RM_LOCAL FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_InternalTransmitRequest2(J1939Rm_ChannelIterType channelIdx,
                                                                                  uint32 requestedPgn, uint8 specInstr,
                                                                                  uint8 extId1, uint8 extId2,
                                                                                  uint8 extId3, uint8 sourceAddress,
                                                                                  uint8 destAddress, uint8 priority,
                                                                                  uint16 nodeChannelStateIdx)
{
  Std_ReturnType result = E_NOT_OK;

  J1939Rm_TxPduRqst2IdxOfChannelType txPduIdx = J1939Rm_GetTxPduRqst2IdxOfChannel(channelIdx);
  J1939Rm_TxPduStateType *pduState = &J1939Rm_GetTxPduState(J1939Rm_GetTxPduStateIdxOfTxPdu(txPduIdx));

  J1939RM_DUMMY_STATEMENT(nodeChannelStateIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  /* #10 Enter J1939RM_EXCLUSIVE_AREA_TXPDULOCK. */
  SchM_Enter_J1939Rm_J1939RM_EXCLUSIVE_AREA_TXPDULOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #20 Check if the Pdu to use is free: */
  if (!pduState->Locked) /* PRQA S 3344 */ /* MD_J1939Rm_13.2 */
  {
    /* #30 Prepare the Pdu with the data to transmit. */
    uint8 data[J1939RM_RQST2_PDU_LENGTH];
    PduInfoType pdu;

    pduState->ConfTimeout = J1939Rm_GetTxConfTimeout();
    pduState->Locked = TRUE;

    /* #40 Leave J1939RM_EXCLUSIVE_AREA_TXPDULOCK. */
    SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_TXPDULOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    pdu.SduDataPtr = data;
    pdu.SduLength = J1939RM_RQST2_PDU_LENGTH;

    data[J1939RM_RQST2_PGN_LO] = J1939Rm_Make_PGN_LO(requestedPgn);
    data[J1939RM_RQST2_PGN_MD] = J1939Rm_Make_PGN_MD(requestedPgn);
    data[J1939RM_RQST2_PGN_HI] = J1939Rm_Make_PGN_HI(requestedPgn);
    data[J1939RM_RQST2_SI] = specInstr;
    data[J1939RM_RQST2_EIB1] = extId1;
    data[J1939RM_RQST2_EIB2] = extId2;
    data[J1939RM_RQST2_EIB3] = extId3;
    data[J1939RM_RQST2_RES] = J1939RM_SNA_VALUE;
    data[J1939RM_RQST2_SA_POS] = sourceAddress;
    data[J1939RM_RQST2_DA_POS] = destAddress;
    data[J1939RM_RQST2_PRI_POS] = J1939Rm_Make_Priority(priority);

    /* #50 Call the PduR transmit function. */
    result = PduR_J1939RmTransmit(J1939Rm_GetPduIdOfTxPdu(txPduIdx), &pdu);

    /* #60 If the transmit was successful report the transmitted Request2 internal, 
     *     otherwise release the state of the Pdu. */
    if (result == E_OK)
    {
      J1939Rm_RxIndication((PduIdType) J1939Rm_GetRxPduRqst2IdxOfChannel(channelIdx), &pdu);
    }
    else
    {
      pduState->Locked = FALSE;
    }
  }
  /* #70 Otherwise, leave J1939RM_EXCLUSIVE_AREA_TXPDULOCK. */
  else
  {
    SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_TXPDULOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  /* #80 If the transmit has failed and the queuing of Request2 is enabled: */
#  if (J1939RM_REQ2QUEUEINDUSEDOFCHANNEL == STD_ON)
  if (result != E_OK)
  {
    /* #90 Enter J1939RM_EXCLUSIVE_AREA_REQ2QUEUELOCK. */
    SchM_Enter_J1939Rm_J1939RM_EXCLUSIVE_AREA_REQ2QUEUELOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #100 If there is an free element in the Request2 queue: */
    if (J1939Rm_GetChannelState(channelIdx).Req2QueueFree > 0)
    {
      /* #110 Get an element of the Request2 queue and set the data to transmit. */
      J1939Rm_Req2QueueElementType *elem = &J1939Rm_GetReq2Queue(J1939Rm_GetChannelState(channelIdx).Req2QueueWrite);

      elem->RequestedPgn = requestedPgn;
      elem->SpecInstr = specInstr;
      elem->ExtId1 = extId1;
      elem->ExtId2 = extId2;
      elem->ExtId3 = extId3;
      elem->SourceAddress = sourceAddress;
      elem->DestAddress = destAddress;
      elem->Priority = priority;
      elem->NodeChannelStateIndex = nodeChannelStateIdx;

      /* #120 If the queue is full now, handle queue pointer wrap around. */
      J1939Rm_GetChannelState(channelIdx).Req2QueueWrite++;
      if (J1939Rm_GetChannelState(channelIdx).Req2QueueWrite == J1939Rm_GetReq2QueueIndEndIdxOfChannel(channelIdx))
      {
        J1939Rm_GetChannelState(channelIdx).Req2QueueWrite = J1939Rm_GetReq2QueueIndStartIdxOfChannel(channelIdx);
      }
      J1939Rm_GetChannelState(channelIdx).Req2QueueFree--;

      result = E_OK;
    }

    /* #130 Leave J1939RM_EXCLUSIVE_AREA_REQ2QUEUELOCK. */
    SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_REQ2QUEUELOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
#  endif

  return result;
} /* J1939Rm_InternalTransmitRequest2() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */
# endif
#endif

#if ((J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON) || (J1939RM_SUPPORT_COM_USER == STD_ON))
/**********************************************************************************************************************
 *  J1939Rm_InternalProcessRequest
 *********************************************************************************************************************/
/*!
 *
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
J1939RM_LOCAL FUNC(void,
                   J1939RM_CODE) J1939Rm_InternalProcessRequest(J1939Rm_ChannelIdxOfRxPduType channelIdx,
                                                                uint32 requestedPgn,
                                                                const J1939Rm_ExtIdInfoType * extIdInfo,
                                                                uint8 sourceAddress,
                                                                uint8 destAddress, uint8 priority, uint8 ackCb)
{
  /* ----- Local Variables ---------------------------------------------- */
  J1939Rm_ChannelNodeNodeTableIndIterType channelNodeIdx;

  /* #10 Check if the PGN of the received Request is valid: */
  if (J1939Rm_PGN_Valid(requestedPgn))
  {
    /* #20 Go through all nodes of the channel the Request was received on: */
    for (channelNodeIdx = J1939Rm_GetChannelNodeNodeTableIndStartIdxOfChannel(channelIdx);
         channelNodeIdx < J1939Rm_GetChannelNodeNodeTableIndEndIdxOfChannel(channelIdx); channelNodeIdx++)
    {
      J1939Rm_NodeNodeIdxOfChannelNodeType nodeIdx = J1939Rm_GetNodeNodeIdxOfChannelNode(channelNodeIdx);
      uint8 nodeAddr = J1939Rm_GetNodeAddressOfNode(nodeIdx);
      J1939Rm_NodeChannelStateIdxOfChannelNodeType nodeChannelStateIdx =
        J1939Rm_GetNodeChannelStateIdxOfChannelNode(channelNodeIdx);
      J1939Rm_NodeChannelStateType nodeChannelState = J1939Rm_GetNodeChannelState(nodeChannelStateIdx);

      /* #30 Check if the Request is addressed to the current explored node: */
      if (((nodeChannelState == J1939RM_STATE_ONLINE) || (requestedPgn == J1939RM_PGN_AC)) &&
          ((destAddress == J1939RM_BROADCAST_ADDRESS) || (destAddress == nodeAddr)))
      {
        boolean found = FALSE;
        J1939Rm_NodeUserUserTableIndIterType nodeUserIdx;

        /* #40 The Request is addressed to the current explored node, go through the list of its user: */
        for (nodeUserIdx = J1939Rm_GetNodeUserUserTableIndStartIdxOfNode(nodeIdx);
             nodeUserIdx < J1939Rm_GetNodeUserUserTableIndEndIdxOfNode(nodeIdx); nodeUserIdx++)
        {
          J1939Rm_UserUserIdxOfNodeUserType userIdx = J1939Rm_GetUserUserIdxOfNodeUser(nodeUserIdx);

# if (J1939RM_SUPPORT_COM_USER == STD_ON)
          /* #50 If the current explored user is a COM user and the received Request is configured for this user,
           *     process the incoming Request on the COM path. Otherwise use the default path to inform about a incoming
           *     Request. */
          if (J1939Rm_GetUserTypeOfUser(userIdx) == J1939RM_USER_COM_USERTYPEOFUSER)
          {
            found |=
              J1939Rm_InternalProcessRequestForComIPdu(channelIdx, userIdx, requestedPgn, extIdInfo,
                                                       nodeAddr, sourceAddress, destAddress, priority);
          }
# endif
# if ((J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON) && (J1939RM_SUPPORT_COM_USER == STD_ON))
          else
# endif
# if (J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON)
          {
            found |=
              J1939Rm_InternalProcessRequestIndication(channelIdx, nodeIdx, userIdx, requestedPgn,
                                                       extIdInfo, sourceAddress, destAddress, priority);
          }
# endif
        }

        /* #60 If no user is found to process the Request and it is destination specific, send an negative
         *     acknowledgment for the Request. \trace SPEC-2167410 */
        if ((!found) && (destAddress != J1939RM_BROADCAST_ADDRESS))
        {
          uint8 extId1, extId2, extId3;
          Std_ReturnType result;

          if (extIdInfo != NULL_PTR)
          {
            extId1 = extIdInfo->ExtId1;
            extId2 = extIdInfo->ExtId2;
            extId3 = extIdInfo->ExtId3;
          }
          else
          {
            extId1 = J1939RM_EXTID_UNUSED;
            extId2 = J1939RM_EXTID_UNUSED;
            extId3 = J1939RM_EXTID_UNUSED;
          }

          result = J1939Rm_InternalTransmitAck(channelIdx, requestedPgn, ackCb, extId1, extId2, extId3, sourceAddress,
                                               nodeAddr, J1939RM_DEFAULT_PRIORITY, FALSE, nodeChannelStateIdx);

# if (J1939RM_RUNTIME_ERROR_REPORT == STD_ON)
          if (result != E_OK)
          {
            J1939Rm_ReportRuntimeError(J1939RM_SID_RXINDICATION, J1939RM_E_ACK_QUEUE_OVERRUN);
          }
# endif
        }
      }
    }
  }
# if (J1939RM_RUNTIME_ERROR_REPORT == STD_ON)
  else
  {
    J1939Rm_ReportRuntimeError(J1939RM_SID_RXINDICATION, J1939RM_E_BAD_PGN);
  }
# endif
} /* J1939Rm_InternalProcessRequest() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif

#if ((J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON) || (J1939RM_SUPPORT_COM_USER == STD_ON))
/**********************************************************************************************************************
 *  J1939Rm_InternalReceiveRequest
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalReceiveRequest(J1939Rm_ChannelIdxOfRxPduType channelIdx,
                                                                             P2CONST(PduInfoType, AUTOMATIC,
                                                                                     J1939RM_APPL_DATA) PduInfoPtr)
{
# if (J1939RM_CANIF_DLCCHECK == STD_OFF)
  /* #10 If CanIf DLC check is disabled, check the length of the received Request and stop processing on error. */
  if (PduInfoPtr->SduLength == J1939RM_RQST_PDU_LENGTH)
# endif
  {
    /* #20 Get the data from the received Request. */
    uint32 requestedPgn = J1939Rm_Get_PGN(PduInfoPtr->SduDataPtr[J1939RM_RQST_PGN_LO],
                                          PduInfoPtr->SduDataPtr[J1939RM_RQST_PGN_MD],
                                          PduInfoPtr->SduDataPtr[J1939RM_RQST_PGN_HI]);
    uint8 sourceAddress = PduInfoPtr->SduDataPtr[J1939RM_RQST_SA_POS];
    uint8 destAddress = PduInfoPtr->SduDataPtr[J1939RM_RQST_DA_POS];
    uint8 priority = J1939Rm_Get_Priority(PduInfoPtr->SduDataPtr[J1939RM_RQST_PRI_POS]);

    if (((sourceAddress != J1939RM_NULL_ADDRESS) || (requestedPgn == J1939RM_PGN_AC)) &&
        (sourceAddress != J1939RM_BROADCAST_ADDRESS))
    {
      J1939Rm_ExtIdInfoType extIdInfo;

      extIdInfo.ExtIdType = J1939RM_EXTID_NONE;
      extIdInfo.ExtId1 = J1939RM_EXTID_UNUSED;
      extIdInfo.ExtId2 = J1939RM_EXTID_UNUSED;
      extIdInfo.ExtId3 = J1939RM_EXTID_UNUSED;

      /* #30 Process the Request. */
      J1939Rm_InternalProcessRequest(channelIdx, requestedPgn, &extIdInfo, sourceAddress, destAddress, priority,
                                     J1939RM_ACK_NEGATIVE);
    }
# if (J1939RM_RUNTIME_ERROR_REPORT == STD_ON)
    else
    {
      J1939Rm_ReportRuntimeError(J1939RM_SID_RXINDICATION, J1939RM_E_BAD_SA);
    }
# endif
  }
# if ((J1939RM_CANIF_DLCCHECK == STD_OFF) && (J1939RM_RUNTIME_ERROR_REPORT == STD_ON))
  else
  {
    J1939Rm_ReportRuntimeError(J1939RM_SID_RXINDICATION, J1939RM_E_INVALID_PDU_SIZE);
  }
# endif
} /* J1939Rm_InternalReceiveRequest() */
#endif

#if (((J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON) || (J1939RM_SUPPORT_COM_USER == STD_ON)) && (J1939RM_SUPPORT_REQUEST2 == STD_ON))
/**********************************************************************************************************************
 *  J1939Rm_InternalReceiveRequest2
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalReceiveRequest2(J1939Rm_ChannelIdxOfRxPduType channelIdx,
                                                                              P2CONST(PduInfoType, AUTOMATIC,
                                                                                      J1939RM_APPL_DATA) PduInfoPtr)
{
# if (J1939RM_CANIF_DLCCHECK == STD_OFF)
  /* #10 If CanIf DLC check is disabled, check the length of the received Request2 and stop processing on error. */
  if (PduInfoPtr->SduLength == J1939RM_RQST2_PDU_LENGTH)
# endif
  {
    /* #20 Get the data from the received Request2. */
    uint32 requestedPgn = J1939Rm_Get_PGN(PduInfoPtr->SduDataPtr[J1939RM_RQST2_PGN_LO],
                                          PduInfoPtr->SduDataPtr[J1939RM_RQST2_PGN_MD],
                                          PduInfoPtr->SduDataPtr[J1939RM_RQST2_PGN_HI]);
    uint8 specInstr = PduInfoPtr->SduDataPtr[J1939RM_RQST2_SI];
    uint8 extId1 = PduInfoPtr->SduDataPtr[J1939RM_RQST2_EIB1];
    uint8 extId2 = PduInfoPtr->SduDataPtr[J1939RM_RQST2_EIB2];
    uint8 extId3 = PduInfoPtr->SduDataPtr[J1939RM_RQST2_EIB3];
    uint8 sourceAddress = PduInfoPtr->SduDataPtr[J1939RM_RQST2_SA_POS];
    uint8 destAddress = PduInfoPtr->SduDataPtr[J1939RM_RQST2_DA_POS];
    uint8 priority = J1939Rm_Get_Priority(PduInfoPtr->SduDataPtr[J1939RM_RQST2_PRI_POS]);

    if (J1939Rm_ExtIdType_Valid(specInstr) && J1939Rm_TransferType_Valid(specInstr) &&
        (J1939Rm_GetTransferType(specInstr) != J1939RM_RQST2_TRANSFER_YES))
    {
      if (((sourceAddress != J1939RM_NULL_ADDRESS) || (requestedPgn == J1939RM_PGN_AC)) &&
          (sourceAddress != J1939RM_BROADCAST_ADDRESS))
      {
        J1939Rm_ExtIdInfoType extIdInfo;
        uint8 ackCb = J1939RM_ACK_NEGATIVE;

        extIdInfo.ExtId1 = J1939RM_EXTID_UNUSED;
        extIdInfo.ExtId2 = J1939RM_EXTID_UNUSED;
        extIdInfo.ExtId3 = J1939RM_EXTID_UNUSED;

        switch (J1939Rm_GetExtIdType(specInstr))
        {
        case J1939RM_RQST2_EXTIDTYPE_ONE:
          extIdInfo.ExtIdType = J1939RM_EXTID_ONE;
          extIdInfo.ExtId1 = extId1;
          ackCb = J1939Rm_Make_AckCb(J1939RM_ACKM_CODE_EXTID_ONE, ackCb);
          break;

        case J1939RM_RQST2_EXTIDTYPE_TWO:
          extIdInfo.ExtIdType = J1939RM_EXTID_TWO;
          extIdInfo.ExtId1 = extId1;
          extIdInfo.ExtId2 = extId2;
          ackCb = J1939Rm_Make_AckCb(J1939RM_ACKM_CODE_EXTID_TWO, ackCb);
          break;

        case J1939RM_RQST2_EXTIDTYPE_THREE:
          extIdInfo.ExtIdType = J1939RM_EXTID_THREE;
          extIdInfo.ExtId1 = extId1;
          extIdInfo.ExtId2 = extId2;
          extIdInfo.ExtId3 = extId3;
          ackCb = J1939Rm_Make_AckCb(J1939RM_ACKM_CODE_EXTID_THREE, ackCb);
          break;

        case J1939RM_RQST2_EXTIDTYPE_NA:
        case J1939RM_RQST2_EXTIDTYPE_NONE:
        default: /* MISRA */
          extIdInfo.ExtIdType = J1939RM_EXTID_NONE;
          break;
        }

        /* #30 Process the Request. */
        J1939Rm_InternalProcessRequest(channelIdx, requestedPgn, &extIdInfo, sourceAddress, destAddress, priority,
                                       ackCb);
      }
# if (J1939RM_RUNTIME_ERROR_REPORT == STD_ON)
      else
      {
        J1939Rm_ReportRuntimeError(J1939RM_SID_RXINDICATION, J1939RM_E_BAD_SA);
      }
# endif
    }
# if (J1939RM_RUNTIME_ERROR_REPORT == STD_ON)
    else
    {
      J1939Rm_ReportRuntimeError(J1939RM_SID_RXINDICATION, J1939RM_E_INVALID_SPEC_INSTR);
    }
# endif
  }
# if ((J1939RM_CANIF_DLCCHECK == STD_OFF) && (J1939RM_RUNTIME_ERROR_REPORT == STD_ON))
  else
  {
    J1939Rm_ReportRuntimeError(J1939RM_SID_RXINDICATION, J1939RM_E_INVALID_PDU_SIZE);
  }
# endif
} /* J1939Rm_InternalReceiveRequest2() */
#endif

#if (J1939RM_SUPPORT_ACK_INDICATION == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalProcessAck
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalProcessAck(J1939Rm_ChannelIdxOfRxPduType channelIdx,
                                                                         P2CONST(PduInfoType, AUTOMATIC,
                                                                                 J1939RM_APPL_DATA) PduInfoPtr,
                                                                         J1939Rm_AckCode ackCode, uint8 extIdCode,
                                                                         uint32 pgn)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 extId1 = PduInfoPtr->SduDataPtr[J1939RM_ACKM_GF1];
  uint8 extId2 = PduInfoPtr->SduDataPtr[J1939RM_ACKM_GF2];
  uint8 extId3 = PduInfoPtr->SduDataPtr[J1939RM_ACKM_GF3];
  uint8 ackAddress = PduInfoPtr->SduDataPtr[J1939RM_ACKM_ADDR];
  uint8 sa = PduInfoPtr->SduDataPtr[J1939RM_ACKM_SA_POS];
  uint8 priority = J1939Rm_Get_Priority(PduInfoPtr->SduDataPtr[J1939RM_ACKM_PRI_POS]);

  /* #10 Check if the source address of the received Acknowledgment is valid. */
  if ((sa != J1939RM_NULL_ADDRESS) && (sa != J1939RM_BROADCAST_ADDRESS))
  {
    J1939Rm_ChannelNodeNodeTableIndIterType channelNodeIdx;
    J1939Rm_ExtIdInfoType extIdInfo;

    J1939Rm_InternalCreateExtIdInfo(&extIdInfo, extIdCode, extId1, extId2, extId3);

    /* #20 Search for nodes that are online on the channel where the Acknowledgment was received. */
    for (channelNodeIdx = J1939Rm_GetChannelNodeNodeTableIndStartIdxOfChannel(channelIdx);
         channelNodeIdx < J1939Rm_GetChannelNodeNodeTableIndEndIdxOfChannel(channelIdx); channelNodeIdx++)
    {
      J1939Rm_NodeNodeIdxOfChannelNodeType nodeIdx = J1939Rm_GetNodeNodeIdxOfChannelNode(channelNodeIdx);
      J1939Rm_NodeChannelStateType nodeChannelState =
        J1939Rm_GetNodeChannelState(J1939Rm_GetNodeChannelStateIdxOfChannelNode(channelNodeIdx));

      if (nodeChannelState == J1939RM_STATE_ONLINE)
      {
        J1939Rm_NodeUserUserTableIndIterType nodeUserIdx;

        /* #30 Go through all user of the current explored node. */
        for (nodeUserIdx = J1939Rm_GetNodeUserUserTableIndStartIdxOfNode(nodeIdx);
             nodeUserIdx < J1939Rm_GetNodeUserUserTableIndEndIdxOfNode(nodeIdx); nodeUserIdx++)
        {
          J1939Rm_UserUserIdxOfNodeUserType userIdx = J1939Rm_GetUserUserIdxOfNodeUser(nodeUserIdx);

          /* #40 If the current explored user is configured to receive this Acknowledgment, process it. */
          if (J1939Rm_IsUserAckIndicationOfUser(userIdx))
          {
            J1939Rm_InternalCallAckIndication(channelIdx, nodeIdx, userIdx, extIdInfo, ackAddress, sa, priority,
                                              ackCode, pgn);
          }
        }
      }
    }
  }
} /* J1939Rm_InternalProcessAck() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (J1939RM_SUPPORT_ACK_INDICATION == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalReceiveAck
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalReceiveAck(J1939Rm_ChannelIdxOfRxPduType channelIdx,
                                                                         P2CONST(PduInfoType, AUTOMATIC,
                                                                                 J1939RM_APPL_DATA) PduInfoPtr)
{
# if (J1939RM_CANIF_DLCCHECK == STD_OFF)
  /* #10 If CanIf DLC check is disabled, check the length of the received acknowledgment and stop processing on error. */
  if (PduInfoPtr->SduLength == J1939RM_ACKM_PDU_LENGTH)
# endif
  {
    uint8 ackCb = PduInfoPtr->SduDataPtr[J1939RM_ACKM_CB];
    J1939Rm_AckCode ackCode = J1939Rm_Get_BaseAckCode(ackCb);
    uint8 extIdCode = J1939Rm_Get_ExtIdAckCode(ackCb);
# if ((J1939RM_RUNTIME_ERROR_REPORT == STD_ON) && (J1939RM_ISOBUSCHANNELOFCHANNEL == STD_ON))
    uint8 ackAddress = PduInfoPtr->SduDataPtr[J1939RM_ACKM_ADDR];
# endif
    uint32 pgn = J1939Rm_Get_PGN(PduInfoPtr->SduDataPtr[J1939RM_ACKM_PGN_LO],
                                 PduInfoPtr->SduDataPtr[J1939RM_ACKM_PGN_MD],
                                 PduInfoPtr->SduDataPtr[J1939RM_ACKM_PGN_HI]);
# if (J1939RM_RUNTIME_ERROR_REPORT == STD_ON)
    uint8 da = PduInfoPtr->SduDataPtr[J1939RM_ACKM_DA_POS];
# endif

# if (J1939RM_RUNTIME_ERROR_REPORT == STD_ON)
    /* #20 Check if the Acknowledgment code is valid. */
    if ((!J1939Rm_AckCode_Valid(ackCode)) || (!J1939Rm_AckExtIdCode_Valid(extIdCode)))
    {
      J1939Rm_ReportRuntimeError(J1939RM_SID_RXINDICATION, J1939RM_E_INVALID_ACK_CB);
    }
# endif

    /* #30 Check if the PGN is valid: */
    if (J1939Rm_PGN_Valid(pgn))
    {
      /* #40 If runtime error reporting is enabled, check if the destination address is not valid: */
# if (J1939RM_RUNTIME_ERROR_REPORT == STD_ON)
      if ((da != J1939RM_BROADCAST_ADDRESS)
#  if (J1939RM_ISOBUSCHANNELOFCHANNEL == STD_ON)
          && ((!J1939Rm_IsIsobusChannelOfChannel(channelIdx)) || (da != ackAddress))
#  endif
         )
      {
        /* #50 Report a runtime error. */
        J1939Rm_ReportRuntimeError(J1939RM_SID_RXINDICATION, J1939RM_E_ISOBUS_ACKM_ADDR);
      }
      else
# endif
      /* #60 Otherwise, process the Acknowledgment. */
      {
        J1939Rm_InternalProcessAck(channelIdx, PduInfoPtr, ackCode, extIdCode, pgn);
      }
    }
# if (J1939RM_RUNTIME_ERROR_REPORT == STD_ON)
    else
    {
      J1939Rm_ReportRuntimeError(J1939RM_SID_RXINDICATION, J1939RM_E_BAD_PGN);
    }
# endif
  }
# if ((J1939RM_CANIF_DLCCHECK == STD_OFF) && (J1939RM_RUNTIME_ERROR_REPORT == STD_ON))
  else
  {
    J1939Rm_ReportRuntimeError(J1939RM_SID_RXINDICATION, J1939RM_E_INVALID_PDU_SIZE);
  }
# endif
} /* J1939Rm_InternalReceiveAck() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (J1939RM_ACKQUEUEINDUSEDOFCHANNEL == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalProcessAckQueue
 *********************************************************************************************************************/
/*!
 *
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
J1939RM_LOCAL FUNC(void, J1939RM_CODE) J1939Rm_InternalProcessAckQueue(J1939Rm_ChannelIterType channelIdx,
                                                                       uint8 serviceId)
{
  J1939RM_DUMMY_STATEMENT(serviceId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  /* #10 Find first active message in the queue while discarding all queued messages of offline nodes: */
  while (J1939Rm_GetChannelState(channelIdx).AckQueueFree < J1939Rm_GetAckQueueSizeOfChannel(channelIdx))
  {
    /* #20 Enter J1939RM_EXCLUSIVE_AREA_ACKQUEUELOCK. */
    SchM_Enter_J1939Rm_J1939RM_EXCLUSIVE_AREA_ACKQUEUELOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #30 If active message was found: */
    if (J1939Rm_GetChannelState(channelIdx).AckQueueFree < J1939Rm_GetAckQueueSizeOfChannel(channelIdx))
    {
      /* #40 Get the oldest element from the acknowledgment queue. */
      J1939Rm_AckQueueElementType *elem = &J1939Rm_GetAckQueue(J1939Rm_GetChannelState(channelIdx).AckQueueRead);

      uint32 ackPgn = elem->AckPgn;
      uint8 ackCb = elem->AckCb;
      uint8 extId1 = elem->ExtId1;
      uint8 extId2 = elem->ExtId2;
      uint8 extId3 = elem->ExtId3;
      uint8 ackAddress = elem->AckAddress;
      uint8 sourceAddress = elem->SourceAddress;
      uint8 priority = elem->Priority;
      boolean broadcast = elem->Broadcast;
      uint16 nodeChannelStateIdx = elem->NodeChannelStateIndex;

      Std_ReturnType result;

      J1939Rm_GetChannelState(channelIdx).AckQueueRead++;

      /* #50 If the removed element was on the last queue position, handle queue pointer wrap around. */
      if (J1939Rm_GetChannelState(channelIdx).AckQueueRead == J1939Rm_GetAckQueueIndEndIdxOfChannel(channelIdx))
      {
        J1939Rm_GetChannelState(channelIdx).AckQueueRead = J1939Rm_GetAckQueueIndStartIdxOfChannel(channelIdx);
      }

      J1939Rm_GetChannelState(channelIdx).AckQueueFree++;

      /* #60 Leave J1939RM_EXCLUSIVE_AREA_ACKQUEUELOCK. */
      SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_ACKQUEUELOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #70 If the node which wants to send the message is still online: */
      if (J1939Rm_GetNodeChannelState(nodeChannelStateIdx) == J1939RM_STATE_ONLINE)
      {
        /* #80 Send the Acknowledgment. */
        result = J1939Rm_InternalTransmitAck(channelIdx, ackPgn, ackCb, extId1, extId2, extId3, ackAddress,
                                             sourceAddress, priority, broadcast, nodeChannelStateIdx);

# if (J1939RM_RUNTIME_ERROR_REPORT == STD_ON)
        if (result != E_OK)
        {
          J1939Rm_ReportRuntimeError(serviceId, J1939RM_E_ACK_QUEUE_OVERRUN);
        }
# endif

        break;
      }
    }
    /* #90 Otherwise, leave J1939RM_EXCLUSIVE_AREA_ACKQUEUELOCK. */
    else
    {
      SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_ACKQUEUELOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
} /* J1939Rm_InternalProcessAckQueue() */
#endif

#if ((J1939RM_REQQUEUEINDUSEDOFCHANNEL == STD_ON) && (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON))
/**********************************************************************************************************************
 *  J1939Rm_InternalProcessRqstQueue
 *********************************************************************************************************************/
/*!
 *
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
J1939RM_LOCAL FUNC(void, J1939RM_CODE) J1939Rm_InternalProcessRqstQueue(J1939Rm_ChannelIterType channelIdx,
                                                                        uint8 serviceId)
{
  J1939RM_DUMMY_STATEMENT(serviceId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  /* #10 Find first active message in the queue while discarding all queued messages of offline nodes: */
  while (J1939Rm_GetChannelState(channelIdx).ReqQueueFree < J1939Rm_GetRequestQueueSizeOfChannel(channelIdx))
  {
    /* #20 Enter J1939RM_EXCLUSIVE_AREA_REQQUEUELOCK. */
    SchM_Enter_J1939Rm_J1939RM_EXCLUSIVE_AREA_REQQUEUELOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #30 If active message was found: */
    if (J1939Rm_GetChannelState(channelIdx).ReqQueueFree < J1939Rm_GetRequestQueueSizeOfChannel(channelIdx))
    {
      /* #40 Get the oldest queue element from the Request queue. */
      J1939Rm_ReqQueueElementType *elem = &J1939Rm_GetReqQueue(J1939Rm_GetChannelState(channelIdx).ReqQueueRead);

      uint32 requestedPgn = elem->RequestedPgn;
      uint8 sourceAddress = elem->SourceAddress;
      uint8 destAddress = elem->DestAddress;
      uint8 priority = elem->Priority;
      uint16 nodeChannelStateIdx = elem->NodeChannelStateIndex;

      Std_ReturnType result;

      J1939Rm_GetChannelState(channelIdx).ReqQueueRead++;

      /* #50 If the removed element was on the last queue position, handle queue pointer wrap around. */
      if (J1939Rm_GetChannelState(channelIdx).ReqQueueRead == J1939Rm_GetReqQueueIndEndIdxOfChannel(channelIdx))
      {
        J1939Rm_GetChannelState(channelIdx).ReqQueueRead = J1939Rm_GetReqQueueIndStartIdxOfChannel(channelIdx);
      }

      J1939Rm_GetChannelState(channelIdx).ReqQueueFree++;

      /* #60 Leave J1939RM_EXCLUSIVE_AREA_REQQUEUELOCK. */
      SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_REQQUEUELOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #70 If the queued element is an Request for address claim from the J1939Nm or if not, 
       *     check if the node which wants to send the message is still online: */
      if ((nodeChannelStateIdx == J1939RM_NULLADDRESS_NODE_CHANNEL_STATE_IDX) ||
          (J1939Rm_GetNodeChannelState(nodeChannelStateIdx) == J1939RM_STATE_ONLINE))
      {
        /* #80 Send a Request message. */
        result =
          J1939Rm_InternalTransmitRequest(channelIdx, requestedPgn, sourceAddress, destAddress, priority,
                                          nodeChannelStateIdx);

# if (J1939RM_RUNTIME_ERROR_REPORT == STD_ON)
        if (result != E_OK)
        {
          J1939Rm_ReportRuntimeError(serviceId, J1939RM_E_REQ_QUEUE_OVERRUN);
        }
# endif

        break;
      }
    }
    /* #90 Otherwise, leave J1939RM_EXCLUSIVE_AREA_REQQUEUELOCK. */
    else
    {
      SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_REQQUEUELOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
} /* J1939Rm_InternalProcessRqstQueue() */
#endif

#if ((J1939RM_REQ2QUEUEINDUSEDOFCHANNEL == STD_ON) && (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON) && (J1939RM_SUPPORT_REQUEST2 == STD_ON))
/**********************************************************************************************************************
 *  J1939Rm_InternalProcessRqst2Queue
 *********************************************************************************************************************/
/*!
 *
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
J1939RM_LOCAL FUNC(void, J1939RM_CODE) J1939Rm_InternalProcessRqst2Queue(J1939Rm_ChannelIterType channelIdx,
                                                                         uint8 serviceId)
{
  J1939RM_DUMMY_STATEMENT(serviceId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  /* #10 Find first active message in the queue while discarding all queued messages of offline nodes: */
  while (J1939Rm_GetChannelState(channelIdx).Req2QueueFree < J1939Rm_GetRequest2QueueSizeOfChannel(channelIdx))
  {
    /* #20 Enter J1939RM_EXCLUSIVE_AREA_REQ2QUEUELOCK. */
    SchM_Enter_J1939Rm_J1939RM_EXCLUSIVE_AREA_REQ2QUEUELOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #30 If active message was found: */
    if (J1939Rm_GetChannelState(channelIdx).Req2QueueFree < J1939Rm_GetRequest2QueueSizeOfChannel(channelIdx))
    {
      /* #40 Get the oldest queue element from the Request2 queue. */
      J1939Rm_Req2QueueElementType *elem = &J1939Rm_GetReq2Queue(J1939Rm_GetChannelState(channelIdx).Req2QueueRead);

      uint32 requestedPgn = elem->RequestedPgn;
      uint8 specInstr = elem->SpecInstr;
      uint8 extId1 = elem->ExtId1;
      uint8 extId2 = elem->ExtId2;
      uint8 extId3 = elem->ExtId3;
      uint8 sourceAddress = elem->SourceAddress;
      uint8 destAddress = elem->DestAddress;
      uint8 priority = elem->Priority;
      uint16 nodeChannelStateIdx = elem->NodeChannelStateIndex;

      Std_ReturnType result;

      J1939Rm_GetChannelState(channelIdx).Req2QueueRead++;

      /* #50 If the removed element was on the last queue position, handle queue pointer wrap around. */
      if (J1939Rm_GetChannelState(channelIdx).Req2QueueRead == J1939Rm_GetReq2QueueIndEndIdxOfChannel(channelIdx))
      {
        J1939Rm_GetChannelState(channelIdx).Req2QueueRead = J1939Rm_GetReq2QueueIndStartIdxOfChannel(channelIdx);
      }

      J1939Rm_GetChannelState(channelIdx).Req2QueueFree++;

      /* #60 Leave J1939RM_EXCLUSIVE_AREA_REQ2QUEUELOCK. */
      SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_REQ2QUEUELOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /* #70 If the queued element is an Request2 for address claim from the J1939Nm or if not,
       *     check if the node which wants to send the message is still online: */
      if ((nodeChannelStateIdx == J1939RM_NULLADDRESS_NODE_CHANNEL_STATE_IDX) ||
          (J1939Rm_GetNodeChannelState(nodeChannelStateIdx) == J1939RM_STATE_ONLINE))
      {
        /* #80 Send a Request2 message. */
        result = J1939Rm_InternalTransmitRequest2(channelIdx, requestedPgn, specInstr, extId1, extId2, extId3,
                                                  sourceAddress, destAddress, priority, nodeChannelStateIdx);

# if (J1939RM_RUNTIME_ERROR_REPORT == STD_ON)
        if (result != E_OK)
        {
          J1939Rm_ReportRuntimeError(serviceId, J1939RM_E_REQ2_QUEUE_OVERRUN);
        }
# endif

        break;
      }
    }
    /* #90 Otherwise, leave J1939RM_EXCLUSIVE_AREA_REQ2QUEUELOCK. */
    else
    {
      SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_REQ2QUEUELOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
} /* J1939Rm_InternalProcessRqst2Queue() */
#endif

/**********************************************************************************************************************
 *  J1939Rm_InternalInit
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalInit(void)
{
  J1939Rm_TxPduStateIterType txPduStateIdx;
  J1939Rm_NodeChannelStateIterType nodeChannelStateIdx;
#if ((J1939RM_ACKQUEUEINDUSEDOFCHANNEL == STD_ON) || (J1939RM_REQQUEUEINDUSEDOFCHANNEL == STD_ON))
  J1939Rm_ChannelStateIterType channelStateIdx;
#endif

  /* #10 Initialize the state of all transmit PDUs. */
  for (txPduStateIdx = 0; txPduStateIdx < J1939Rm_GetSizeOfTxPduState(); txPduStateIdx++)
  {
    J1939Rm_GetTxPduState(txPduStateIdx).Locked = FALSE;
  }

  /* #20 If the acknowledgment or the Request queue are configured: */
#if ((J1939RM_ACKQUEUEINDUSEDOFCHANNEL == STD_ON) || (J1939RM_REQQUEUEINDUSEDOFCHANNEL == STD_ON))
  /* #30 Initialize the configured queue. */
  for (channelStateIdx = 0; channelStateIdx < J1939Rm_GetSizeOfChannelState(); channelStateIdx++)
  {
    uint16 startPos;

# if (J1939RM_ACKQUEUEINDUSEDOFCHANNEL == STD_ON)
    J1939Rm_GetChannelState(channelStateIdx).AckQueueFree = J1939Rm_GetAckQueueSizeOfChannel(channelStateIdx);
    startPos = (uint16) J1939Rm_GetAckQueueIndStartIdxOfChannel(channelStateIdx);
    J1939Rm_GetChannelState(channelStateIdx).AckQueueRead = startPos;
    J1939Rm_GetChannelState(channelStateIdx).AckQueueWrite = startPos;
# endif

# if (J1939RM_REQQUEUEINDUSEDOFCHANNEL == STD_ON)
    J1939Rm_GetChannelState(channelStateIdx).ReqQueueFree = J1939Rm_GetRequestQueueSizeOfChannel(channelStateIdx);
    startPos = (uint16) J1939Rm_GetReqQueueIndStartIdxOfChannel(channelStateIdx);
    J1939Rm_GetChannelState(channelStateIdx).ReqQueueRead = startPos;
    J1939Rm_GetChannelState(channelStateIdx).ReqQueueWrite = startPos;
# endif
  }
#endif

  /* #40 Set the state of all configured channels to 'J1939RM_STATE_OFFLINE'. \trace SPEC-2167403 */
  for (nodeChannelStateIdx = 0; nodeChannelStateIdx < J1939Rm_GetSizeOfNodeChannelState(); nodeChannelStateIdx++)
  {
    J1939Rm_SetNodeChannelState(nodeChannelStateIdx, J1939RM_STATE_OFFLINE);
  }

  /* #50 Set the module state to initialized. \trace SPEC-2167398 */
  J1939Rm_ModuleInitialized = J1939RM_INIT;
} /* J1939Rm_InternalInit() */

/**********************************************************************************************************************
 *  J1939Rm_InternalMainFunction
 *********************************************************************************************************************/
/*!
 *
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
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalMainFunction(void)
{
  J1939Rm_TxPduStateIterType txPduStateIdx;

  /* #10 Go through all PDUs: */
  for (txPduStateIdx = 0; txPduStateIdx < J1939Rm_GetSizeOfTxPduState(); txPduStateIdx++)
  {
    J1939Rm_TxPduStateType *pduState = &J1939Rm_GetTxPduState(txPduStateIdx);

    /* #20 Enter J1939RM_EXCLUSIVE_AREA_TXPDULOCK. */
    SchM_Enter_J1939Rm_J1939RM_EXCLUSIVE_AREA_TXPDULOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #30 If Tx PDU is free for use: */
    if (pduState->Locked) /* PRQA S 3344 */ /* MD_J1939Rm_13.2 */
    {
      --pduState->ConfTimeout;

      /* #40 Check if a timeout for the current explored PDU occurred. \trace SPEC-2167402 */
      if (pduState->ConfTimeout == 0)
      {
        /* #50 Release the PDU lock. */
        pduState->Locked = FALSE;

        /* #60 Leave J1939RM_EXCLUSIVE_AREA_TXPDULOCK. */
        SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_TXPDULOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        J1939Rm_ReportRuntimeError(J1939RM_SID_MAINFUNCTION, J1939RM_E_TIMEOUT_TXCONF);
      }
      /* #70 Otherwise, leave J1939RM_EXCLUSIVE_AREA_TXPDULOCK. */
      else
      {
        SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_TXPDULOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
    }
    /* #80 Otherwise, leave J1939RM_EXCLUSIVE_AREA_TXPDULOCK. */
    else
    {
      SchM_Exit_J1939Rm_J1939RM_EXCLUSIVE_AREA_TXPDULOCK(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

  /* #90 If queuing is enabled for Acknowledgment or Request messages: */
#if ((J1939RM_ACKQUEUEINDUSEDOFCHANNEL == STD_ON) || (((J1939RM_REQQUEUEINDUSEDOFCHANNEL == STD_ON) || (J1939RM_REQ2QUEUEINDUSEDOFCHANNEL == STD_ON)) && (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON)))
  {
    J1939Rm_ChannelIterType channelIdx;

    /* #100 Go through all channels: */
    for (channelIdx = 0; channelIdx < J1939Rm_GetSizeOfChannel(); channelIdx++)
    {
# if (J1939RM_ACKQUEUEINDUSEDOFCHANNEL == STD_ON)
      /* #110 Process the queued Acknowledgment messages. */
      J1939Rm_InternalProcessAckQueue(channelIdx, J1939RM_SID_MAINFUNCTION);
# endif

# if ((J1939RM_REQQUEUEINDUSEDOFCHANNEL == STD_ON) && (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON))
      /* #120 Process the queued Request messages. */
      J1939Rm_InternalProcessRqstQueue(channelIdx, J1939RM_SID_MAINFUNCTION);
# endif

# if ((J1939RM_REQ2QUEUEINDUSEDOFCHANNEL == STD_ON) && (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON) && (J1939RM_SUPPORT_REQUEST2 == STD_ON))
      /* #130 Process the queued Request2 messages. */
      J1939Rm_InternalProcessRqst2Queue(channelIdx, J1939RM_SID_TXCONFIRMATION);
# endif
    }
  }
#endif
} /* J1939Rm_InternalMainFunction() */

#if (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalSendRequest
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
J1939RM_LOCAL_INLINE FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_InternalSendRequest(uint8 userId,
                                                                                    NetworkHandleType channel,
                                                                                    uint32 requestedPgn,
                                                                                    P2CONST(J1939Rm_ExtIdInfoType,
                                                                                            AUTOMATIC,
                                                                                            J1939RM_APPL_DATA)
                                                                                    extIdInfo, uint8 destAddress,
                                                                                    uint8 priority,
                                                                                    boolean checkTimeout)
{
  Std_ReturnType result = E_NOT_OK;

  J1939RM_DUMMY_STATEMENT(userId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  J1939RM_DUMMY_STATEMENT(extIdInfo); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  J1939RM_DUMMY_STATEMENT(checkTimeout); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  {
    uint8 sourceAddress;
# if (J1939RM_USERSENDREQUESTOFUSER == STD_ON)
    J1939Rm_NodeIdxOfUserType nodeIdx;
# endif

# if (J1939RM_USERSENDREQUESTOFUSER == STD_ON)
    if (userId == J1939RM_NULLADDRESS_USER_ID)
# endif
    {
# if (J1939RM_USERSENDREQUESTOFUSER == STD_ON)
      nodeIdx = J1939RM_NO_NODEIDOFNODE;
# endif
      sourceAddress = J1939RM_NULL_ADDRESS;
    }
# if (J1939RM_USERSENDREQUESTOFUSER == STD_ON)
    else
    {
      nodeIdx = J1939Rm_GetNodeIdxOfUser(userId);
      sourceAddress = J1939Rm_GetNodeAddressOfNode(nodeIdx);
    }
# endif

    {
      J1939Rm_ChannelIdxOfComMChannelType channelIdx = J1939Rm_GetChannelIdxOfComMChannel(channel);
# if (J1939RM_USERSENDREQUESTOFUSER == STD_ON)
      J1939Rm_ChannelNodeNodeTableIndIterType channelNodeIdx;

      /* #10 Go through all nodes of the channel to send the Request on: */
      for (channelNodeIdx = J1939Rm_GetChannelNodeNodeTableIndStartIdxOfChannel(channelIdx);
           channelNodeIdx < J1939Rm_GetChannelNodeNodeTableIndEndIdxOfChannel(channelIdx); channelNodeIdx++)
# endif
      {
# if (J1939RM_USERSENDREQUESTOFUSER == STD_ON)
        /* #20 If it is a J1939Nm request for address claim with the NULL address or the transmitting node is found: */
        if ((userId == J1939RM_NULLADDRESS_USER_ID) || (nodeIdx == J1939Rm_GetNodeNodeIdxOfChannelNode(channelNodeIdx)))
# endif
        {
          uint16 nodeChannelStateIdx;

# if (J1939RM_USERSENDREQUESTOFUSER == STD_ON)
          if (userId == J1939RM_NULLADDRESS_USER_ID)
          {
            nodeChannelStateIdx = J1939RM_NULLADDRESS_NODE_CHANNEL_STATE_IDX;
          }
          else
          {
            nodeChannelStateIdx = (uint16) J1939Rm_GetNodeChannelStateIdxOfChannelNode(channelNodeIdx);
          }

          /* #30 If the current explored node is online or it is a address claim Request: */
          if ((requestedPgn == J1939RM_PGN_AC) ||
              ((nodeChannelStateIdx == J1939RM_NULLADDRESS_NODE_CHANNEL_STATE_IDX) ||
               (J1939Rm_GetNodeChannelState(nodeChannelStateIdx) == J1939RM_STATE_ONLINE)))
# else
          nodeChannelStateIdx = J1939RM_NULLADDRESS_NODE_CHANNEL_STATE_IDX;
# endif
          {
# if (J1939RM_SUPPORT_REQUEST2 == STD_ON)
            /* #40 If the Request should be send as Request2 PG, trigger the transmission as Request2 message. */
            if ((extIdInfo != NULL_PTR) && (extIdInfo->ExtIdType > J1939RM_EXTID_NONE) &&
                (extIdInfo->ExtIdType < J1939RM_EXTID_GF))
            {
              uint8 specInstr = J1939RM_RQST2_EXTIDTYPE_NA;
              uint8 extId1 = J1939RM_EXTID_UNUSED;
              uint8 extId2 = J1939RM_EXTID_UNUSED;
              uint8 extId3 = J1939RM_EXTID_UNUSED;

              switch (extIdInfo->ExtIdType)
              {
              case J1939RM_EXTID_ONE:
                specInstr = J1939RM_RQST2_EXTIDTYPE_ONE;
                extId1 = extIdInfo->ExtId1;
                break;

              case J1939RM_EXTID_TWO:
                specInstr = J1939RM_RQST2_EXTIDTYPE_TWO;
                extId1 = extIdInfo->ExtId1;
                extId2 = extIdInfo->ExtId2;
                break;

              case J1939RM_EXTID_THREE:
                specInstr = J1939RM_RQST2_EXTIDTYPE_THREE;
                extId1 = extIdInfo->ExtId1;
                extId2 = extIdInfo->ExtId2;
                extId3 = extIdInfo->ExtId3;
                break;

              default: /* MISRA */
                break;
              }

              specInstr |= J1939RM_RQST2_TRANSFER_NA;

              result = J1939Rm_InternalTransmitRequest2(channelIdx, requestedPgn, specInstr, extId1, extId2, extId3,
                                                        sourceAddress, destAddress, priority, nodeChannelStateIdx);
#  if (J1939RM_RUNTIME_ERROR_REPORT == STD_ON)
              if (result != E_OK)
              {
                J1939Rm_ReportRuntimeError(J1939RM_SID_SENDREQUEST, J1939RM_E_REQ2_QUEUE_OVERRUN);
              }
#  endif
            }
            else
# endif
            /* #50 Otherwise, trigger the transmission as Request message. \trace SPEC-2167415 */
            {
              result =
                J1939Rm_InternalTransmitRequest(channelIdx, requestedPgn, sourceAddress, destAddress, priority,
                                                nodeChannelStateIdx);
# if (J1939RM_RUNTIME_ERROR_REPORT == STD_ON)
              if (result != E_OK)
              {
                J1939Rm_ReportRuntimeError(J1939RM_SID_SENDREQUEST, J1939RM_E_REQ_QUEUE_OVERRUN);
              }
# endif
            }
          }

# if (J1939RM_USERSENDREQUESTOFUSER == STD_ON)
          break;
# endif
        }
      }
    }
  }

  return result;
} /* J1939Rm_InternalSendRequest() */ /* PRQA S 6030, 6060, 6080 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */
#endif

#if (J1939RM_SUPPORT_ACK_TRANSMISSION == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_InternalSendAck
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
J1939RM_LOCAL_INLINE FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_InternalSendAck(uint8 userId, NetworkHandleType channel,
                                                                                uint32 ackPgn,
                                                                                P2CONST(J1939Rm_ExtIdInfoType,
                                                                                        AUTOMATIC,
                                                                                        J1939RM_APPL_DATA) extIdInfo,
                                                                                J1939Rm_AckCode ackCode,
                                                                                uint8 ackAddress, uint8 priority,
                                                                                boolean broadcast)
{
  Std_ReturnType result = E_NOT_OK;
  J1939Rm_NodeIdxOfUserType nodeIdx = J1939Rm_GetNodeIdxOfUser(userId);
  J1939Rm_ChannelIdxOfComMChannelType channelIdx = J1939Rm_GetChannelIdxOfComMChannel(channel);
  J1939Rm_ChannelNodeNodeTableIndIterType channelNodeIdx;

  /* #10 Go through all nodes of the channel the Acknowledgment shall be send on: */
  for (channelNodeIdx = J1939Rm_GetChannelNodeNodeTableIndStartIdxOfChannel(channelIdx);
       channelNodeIdx < J1939Rm_GetChannelNodeNodeTableIndEndIdxOfChannel(channelIdx); channelNodeIdx++)
  {
    /* #20 If the given node and the current explored node are equal and the channel of the node is online: */
    if (nodeIdx == J1939Rm_GetNodeNodeIdxOfChannelNode(channelNodeIdx))
    {
      J1939Rm_NodeChannelStateIdxOfChannelNodeType nodeChannelStateIdx =
        J1939Rm_GetNodeChannelStateIdxOfChannelNode(channelNodeIdx);

      if (J1939Rm_GetNodeChannelState(nodeChannelStateIdx) == J1939RM_STATE_ONLINE)
      {
        uint8 ackCb = (uint8) ackCode;
        uint8 extId1 = J1939RM_EXTID_UNUSED;
        uint8 extId2 = J1939RM_EXTID_UNUSED;
        uint8 extId3 = J1939RM_EXTID_UNUSED;

        if (extIdInfo != NULL_PTR)
        {
          switch (extIdInfo->ExtIdType)
          {
          case J1939RM_EXTID_THREE:
            ackCb = J1939Rm_Make_AckCb(J1939RM_ACKM_CODE_EXTID_THREE, ackCb);
            extId1 = extIdInfo->ExtId1;
            extId2 = extIdInfo->ExtId2;
            extId3 = extIdInfo->ExtId3;
            break;

          case J1939RM_EXTID_TWO:
            ackCb = J1939Rm_Make_AckCb(J1939RM_ACKM_CODE_EXTID_TWO, ackCb);
            extId1 = extIdInfo->ExtId1;
            extId2 = extIdInfo->ExtId2;
            break;

          case J1939RM_EXTID_ONE:
            ackCb = J1939Rm_Make_AckCb(J1939RM_ACKM_CODE_EXTID_ONE, ackCb);
            extId1 = extIdInfo->ExtId1;
            break;

          case J1939RM_EXTID_GF:
            extId1 = extIdInfo->ExtId1;
            break;

          case J1939RM_EXTID_NONE:
          default: /* MISRA */
            break;
          }
        }

        /* #30 Trigger the transmission of the Acknowledgment message. */
        result = J1939Rm_InternalTransmitAck(channelIdx, ackPgn, ackCb, extId1, extId2, extId3, ackAddress,
                                             J1939Rm_GetNodeAddressOfNode(nodeIdx), priority, broadcast,
                                             nodeChannelStateIdx);

# if (J1939RM_RUNTIME_ERROR_REPORT == STD_ON)
        if (result != E_OK)
        {
          J1939Rm_ReportRuntimeError(J1939RM_SID_SENDACK, J1939RM_E_ACK_QUEUE_OVERRUN);
        }
# endif
      }

      break;
    }
  }

  return result;
} /* J1939Rm_InternalSendAck() */ /* PRQA S 6060, 6080 */ /* MD_MSR_STPAR, MD_MSR_STMIF */
#endif

/**********************************************************************************************************************
 *  J1939Rm_InternalSetState
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalSetState(NetworkHandleType channel, uint8 node,
                                                                       J1939Rm_StateType newState)
{
  J1939Rm_NodeIdxOfNmNodeType nodeIdx = J1939Rm_GetNodeIdxOfNmNode(node);
  J1939Rm_ChannelIdxOfComMChannelType channelIdx = J1939Rm_GetChannelIdxOfComMChannel(channel);
  J1939Rm_ChannelNodeNodeTableIndIterType channelNodeIdx;

  /* #10 Search for the node to set a new communication state in the list of nodes of the given channel. */
  for (channelNodeIdx = J1939Rm_GetChannelNodeNodeTableIndStartIdxOfChannel(channelIdx);
       channelNodeIdx < J1939Rm_GetChannelNodeNodeTableIndEndIdxOfChannel(channelIdx); channelNodeIdx++)
  {
    if (nodeIdx == J1939Rm_GetNodeNodeIdxOfChannelNode(channelNodeIdx))
    {
      /* #20 If the node is found, change the state. \trace SPEC-2167404 */
      J1939Rm_NodeChannelStateIdxOfChannelNodeType nodeChannelStateIdx =
        J1939Rm_GetNodeChannelStateIdxOfChannelNode(channelNodeIdx);
      J1939Rm_SetNodeChannelState(nodeChannelStateIdx, newState);

      break;
    }
  }
} /* J1939Rm_InternalSetState() */

/**********************************************************************************************************************
 *  J1939Rm_InternalRxIndication
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalRxIndication(PduIdType RxPduId,
                                                                           P2CONST(PduInfoType, AUTOMATIC,
                                                                                   J1939RM_APPL_DATA) PduInfoPtr)
{
  J1939Rm_ChannelIdxOfRxPduType channelIdx = J1939Rm_GetChannelIdxOfRxPdu(RxPduId);

  /* #10 Check the type of the received message. */
  switch (J1939Rm_GetPduTypeOfRxPdu(RxPduId))
  {
#if ((J1939RM_SUPPORT_REQUEST_INDICATION == STD_ON) || (J1939RM_SUPPORT_COM_USER == STD_ON))
  case J1939RM_PDU_RQST_PDUTYPEOFRXPDU:
    /* #20 If it is a Request message, call the receive Request function. */
    J1939Rm_InternalReceiveRequest(channelIdx, PduInfoPtr);
    break;

# if (J1939RM_SUPPORT_REQUEST2 == STD_ON)
  case J1939RM_PDU_RQST2_PDUTYPEOFRXPDU:
    /* #30 If it is a Request2 message, call the receive Request2 function. */
    J1939Rm_InternalReceiveRequest2(channelIdx, PduInfoPtr);
    break;
# endif
#endif

#if (J1939RM_SUPPORT_ACK_INDICATION == STD_ON)
  case J1939RM_PDU_ACKM_PDUTYPEOFRXPDU:
    /* #40 If it is a Acknowledgment message, call the receive Acknowledgment function. */
    J1939Rm_InternalReceiveAck(channelIdx, PduInfoPtr);
    break;

#endif

  default: /* MISRA */
    break;
  }
} /* J1939Rm_InternalRxIndication() */

/**********************************************************************************************************************
 *  J1939Rm_InternalTxConfirmation
 *********************************************************************************************************************/
/*!
 *
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
J1939RM_LOCAL_INLINE FUNC(void, J1939RM_CODE) J1939Rm_InternalTxConfirmation(PduIdType TxPduId)
{
#if ((J1939RM_ACKQUEUEINDUSEDOFCHANNEL == STD_ON) || (((J1939RM_REQQUEUEINDUSEDOFCHANNEL == STD_ON) || ((J1939RM_REQ2QUEUEINDUSEDOFCHANNEL == STD_ON) && (J1939RM_SUPPORT_REQUEST2 == STD_ON))) && (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON)))
  J1939Rm_ChannelIterType channelIdx = J1939Rm_GetChannelIdxOfTxPdu(TxPduId);
#endif
  J1939Rm_TxPduStateType *pduState = &J1939Rm_GetTxPduState(J1939Rm_GetTxPduStateIdxOfTxPdu(TxPduId));

  /* #10 Release the state of the confirmed Tx Pdu. */
  pduState->Locked = FALSE;

#if ((J1939RM_ACKQUEUEINDUSEDOFCHANNEL == STD_ON) || (((J1939RM_REQQUEUEINDUSEDOFCHANNEL == STD_ON) || ((J1939RM_REQ2QUEUEINDUSEDOFCHANNEL == STD_ON) && (J1939RM_SUPPORT_REQUEST2 == STD_ON))) && (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON)))
  /* #20 Identify the type of the Pdu */
  switch (J1939Rm_GetPduTypeOfTxPdu(TxPduId))
  {
# if (J1939RM_ACKQUEUEINDUSEDOFCHANNEL == STD_ON)
  case J1939RM_PDU_ACKM_PDUTYPEOFTXPDU:
    /* #30 If it is a acknowledgment Pdu a acknowledgment was successfully transmitted and the Tx Pdu is free to use
     *     now, process the queued Acknowledgment messages if queuing is enabled. \trace SPEC-2167413 */
    J1939Rm_InternalProcessAckQueue(channelIdx, J1939RM_SID_TXCONFIRMATION);
    break;
# endif

# if ((J1939RM_REQQUEUEINDUSEDOFCHANNEL == STD_ON) && (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON))
  case J1939RM_PDU_RQST_PDUTYPEOFTXPDU:
   /* #40 If it is a Request Pdu a Request was successfully transmitted and the Request Tx Pdu is free to use now,
    *     process the queued Request messages if queuing is enabled. \trace SPEC-2167417 */
    J1939Rm_InternalProcessRqstQueue(channelIdx, J1939RM_SID_TXCONFIRMATION);
    break;
# endif
# if ((J1939RM_REQ2QUEUEINDUSEDOFCHANNEL == STD_ON) && (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON) && (J1939RM_SUPPORT_REQUEST2 == STD_ON))
  case J1939RM_PDU_RQST2_PDUTYPEOFTXPDU:
    /* #50 If it is a Request2 Pdu a Request2 was successfully transmitted and the Request2 Tx Pdu is free to use now,
     *     process the queued Request2 messages if queuing is enabled. */
    J1939Rm_InternalProcessRqst2Queue(channelIdx, J1939RM_SID_TXCONFIRMATION);
    break;
# endif
  default: /* MISRA */
    break;
  }
#endif
} /* J1939Rm_InternalTxConfirmation() */

#define J1939RM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*============================================== J1939RM_STOP_SEC_CODE ==============================================*/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/*============================================== J1939RM_START_SEC_CODE =============================================*/
#define J1939RM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  J1939Rm_InitMemory
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, J1939RM_CODE) J1939Rm_InitMemory(void)
{
  /* #10 Set the J1939Rm to not initialized. */
  J1939Rm_ModuleInitialized = J1939RM_UNINIT;
} /* J1939Rm_InitMemory() */

/**********************************************************************************************************************
 *  J1939Rm_Init
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, J1939RM_CODE) J1939Rm_Init(P2CONST(J1939Rm_ConfigType, AUTOMATIC, J1939RM_INIT_DATA) config)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = J1939RM_E_NO_ERROR;
#endif

#if (J1939RM_USE_INIT_POINTER == STD_ON)
  J1939Rm_ConfigDataPtr = config;
#endif
  J1939RM_DUMMY_STATEMENT(config); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  /* ----- Development Error Checks ------------------------------------- */

  /* #10 Check that the J1939Rm is not initialized yet. \trace SPEC-2167401 */
  if (J1939Rm_ModuleInitialized != J1939RM_UNINIT)
  {
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
    errorId = J1939RM_E_REINIT;
#endif
  }
#if (J1939RM_USE_INIT_POINTER == STD_ON)
  /* #20 If "J1939RM_USE_INIT_POINTER" is enabled, Check that the configuration data pointer is set. */
  else if (J1939Rm_ConfigDataPtr == NULL_PTR)
  {
# if (J1939RM_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
    EcuM_BswErrorHook(J1939RM_MODULE_ID, ECUM_BSWERROR_NULLPTR);
# endif
# if (J1939RM_DEV_ERROR_DETECT == STD_ON)
    errorId = J1939RM_E_PARAM_POINTER;
# endif
  }
# if (J1939RM_FINALMAGICNUMBER == STD_ON)
  /* #30 Check the final magic number is correct. */
  else if (J1939Rm_GetFinalMagicNumber() != J1939RM_FINAL_MAGIC_NUMBER)
  {
#  if (J1939RM_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
    EcuM_BswErrorHook(J1939RM_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
#  endif
#  if (J1939RM_DEV_ERROR_DETECT == STD_ON)
    errorId = J1939RM_E_INIT_FAILED;
#  endif
  }
# endif
#endif
  /* #40 If the initial checks are passed or not required: */
  else
  {
    /* #50 If "J1939RM_USE_INIT_POINTER" is disabled, check that the configuration data pointer is not set. */
#if ((J1939RM_USE_INIT_POINTER == STD_OFF) && (J1939RM_DEV_ERROR_DETECT == STD_ON))
    if (config != NULL_PTR)
    {
      errorId = J1939RM_E_PARAM_POINTER;
    }
#endif
    /* ----- Implementation ----------------------------------------------- */
    /* #60 Proceed with initialization. */
    J1939Rm_InternalInit();
  }

  /* ----- Development Error Report --------------------------------------- */
#if (J1939RM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != J1939RM_E_NO_ERROR)
  {
    J1939Rm_ReportDetError(J1939RM_SID_INIT, errorId);
  }
#endif
} /* J1939Rm_Init() */

/**********************************************************************************************************************
 *  J1939Rm_DeInit
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, J1939RM_CODE) J1939Rm_DeInit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = J1939RM_E_NO_ERROR;
#endif

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check that the J1939Rm is initialized. */
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
  if (J1939Rm_ModuleInitialized != J1939RM_INIT)
  {
    errorId = J1939RM_E_UNINIT;
  }
#endif

  /* #20 Set state of J1939Rm to not initialized. \trace SPEC-2167399 */
  J1939Rm_ModuleInitialized = J1939RM_UNINIT;

  /* ----- Development Error Report ------------------------------------- */
#if (J1939RM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != J1939RM_E_NO_ERROR)
  {
    J1939Rm_ReportDetError(J1939RM_SID_DEINIT, errorId);
  }
#endif
} /* J1939Rm_DeInit() */

#if (J1939RM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  J1939Rm_GetVersionInfo
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, J1939RM_CODE) J1939Rm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, J1939RM_APPL_DATA) versionInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
# if (J1939RM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = J1939RM_E_NO_ERROR;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (J1939RM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if version information pointer is set. */
  if (versionInfo == NULL_PTR)
  {
    errorId = J1939RM_E_PARAM_POINTER;
  }
  /* #20 If the initial checks are passed or not required: */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Copy required information. */
    versionInfo->vendorID = J1939RM_VENDOR_ID;
    versionInfo->moduleID = J1939RM_MODULE_ID;
    versionInfo->sw_major_version = J1939RM_SW_MAJOR_VERSION;
    versionInfo->sw_minor_version = J1939RM_SW_MINOR_VERSION;
    versionInfo->sw_patch_version = J1939RM_SW_PATCH_VERSION;
  }

  /* ----- Development Error Report ------------------------------------- */
# if (J1939RM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != J1939RM_E_NO_ERROR)
  {
    J1939Rm_ReportDetError(J1939RM_SID_GETVERSIONINFO, errorId);
  }
# endif
} /* J1939Rm_GetVersionInfo() */
#endif

/**********************************************************************************************************************
 *  J1939Rm_MainFunction
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, J1939RM_CODE) J1939Rm_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = J1939RM_E_NO_ERROR;
#endif

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check that the J1939Rm is initialized. */
  if (J1939Rm_ModuleInitialized != J1939RM_INIT)
  {
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
    errorId = J1939RM_E_UNINIT;
#endif
  }
  /* #20 If the initial checks are passed: */
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Proceed with internal main function. */
    J1939Rm_InternalMainFunction();
  }

/* ----- Development Error Report ------------------------------------- */
#if (J1939RM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != J1939RM_E_NO_ERROR)
  {
    J1939Rm_ReportDetError(J1939RM_SID_MAINFUNCTION, errorId);
  }
#endif
} /* J1939Rm_MainFunction() */

/**********************************************************************************************************************
 *  J1939Rm_SendRequest
 *********************************************************************************************************************/
/*!
 *
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
FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_SendRequest(uint8 userId, NetworkHandleType channel, uint32 requestedPgn,
                                                       P2CONST(J1939Rm_ExtIdInfoType, AUTOMATIC, J1939RM_APPL_DATA)
                                                       extIdInfo, uint8 destAddress, uint8 priority,
                                                       boolean checkTimeout)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType result = E_NOT_OK;

#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = J1939RM_E_NO_ERROR;
#endif

  J1939RM_DUMMY_STATEMENT(userId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  J1939RM_DUMMY_STATEMENT(channel); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  J1939RM_DUMMY_STATEMENT(requestedPgn); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  J1939RM_DUMMY_STATEMENT(extIdInfo); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  J1939RM_DUMMY_STATEMENT(destAddress); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  J1939RM_DUMMY_STATEMENT(priority); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  J1939RM_DUMMY_STATEMENT(checkTimeout); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check that the J1939Rm is initialized. */
  if (J1939Rm_ModuleInitialized != J1939RM_INIT)
  {
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
    errorId = J1939RM_E_UNINIT;
#endif
  }
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check that the user id is valid. */
  else if (((userId >= J1939Rm_GetSizeOfUser())
# if (J1939RM_INVALIDHNDOFUSER == STD_ON)
            || J1939Rm_IsInvalidHndOfUser(userId)
# endif
           ) && (userId != J1939RM_NULLADDRESS_USER_ID))
  {
    errorId = J1939RM_E_INVALID_USER;
  }
  /* #30 If required check that the user is the special case to send a Request with the NULL address as source
   *      address or that the user is allowed to send Request messages. */
  else if ((userId != J1939RM_NULLADDRESS_USER_ID)
# if (J1939RM_USERSENDREQUESTOFUSER == STD_ON)
           && (!J1939Rm_IsUserSendRequestOfUser(userId))
# endif
    )
  {
    errorId = J1939RM_E_INVALID_API;
  }
  /* #40 Check that the channel is valid. */
  else if (!J1939Rm_Channel_Valid(channel))
  {
    errorId = J1939RM_E_INVALID_NETWORK_ID;
  }
  /* #50 Check that the PGN is valid. */
  else if (!J1939Rm_PGN_Valid(requestedPgn))
  {
    errorId = J1939RM_E_INVALID_PGN;
  }
  /* #60 Check that the extended id information are valid. */
  else if (!J1939Rm_ReqExtIdInfo_Valid(extIdInfo))
  {
    errorId = J1939RM_E_INVALID_EXTID_INFO;
  }
  /* #70 Check that the destination address is valid. */
  else if (!J1939Rm_DA_Valid(destAddress))
  {
    errorId = J1939RM_E_INVALID_ADDRESS;
  }
  /* #80 Check that the priority is valid. */
  else if (!J1939Rm_Priority_Valid(priority))
  {
    errorId = J1939RM_E_INVALID_PRIO;
  }
  /* #90 Check that the timeout check parameter is valid. */
  else if (checkTimeout > TRUE)
  {
    errorId = J1939RM_E_INVALID_OPTION;
  }
#endif
  /* #100 If the initial checks are passed or not required: */
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#if (J1939RM_SUPPORT_REQUEST_TRANSMISSION == STD_ON)
    /* #110 Proceed with internal send Request function. */
    result = J1939Rm_InternalSendRequest(userId, channel, requestedPgn, extIdInfo, destAddress, priority, checkTimeout);
#endif
  }

  /* ----- Development Error Report ------------------------------------- */
#if (J1939RM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != J1939RM_E_NO_ERROR)
  {
    J1939Rm_ReportDetError(J1939RM_SID_SENDREQUEST, errorId);
  }
#endif

  return result;
} /* J1939Rm_SendRequest() */ /* PRQA S 6030, 6060, 6080 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  J1939Rm_SendAck
 *********************************************************************************************************************/
/*!
 *
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
FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_SendAck(uint8 userId, NetworkHandleType channel, uint32 ackPgn,
                                                   P2CONST(J1939Rm_ExtIdInfoType, AUTOMATIC, J1939RM_APPL_DATA)
                                                   extIdInfo, J1939Rm_AckCode ackCode, uint8 ackAddress,
                                                   uint8 priority, boolean broadcast)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType result = E_NOT_OK;
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = J1939RM_E_NO_ERROR;
#endif

  J1939RM_DUMMY_STATEMENT(userId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  J1939RM_DUMMY_STATEMENT(channel); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  J1939RM_DUMMY_STATEMENT(ackPgn); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  J1939RM_DUMMY_STATEMENT(ackCode); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  J1939RM_DUMMY_STATEMENT(ackAddress); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  J1939RM_DUMMY_STATEMENT(priority); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  J1939RM_DUMMY_STATEMENT(broadcast); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  J1939RM_DUMMY_STATEMENT(extIdInfo); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check that the J1939Rm is initialized. */
  if (J1939Rm_ModuleInitialized != J1939RM_INIT)
  {
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
    errorId = J1939RM_E_UNINIT;
#endif
  }
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check if user id is valid. */
  else if ((userId >= J1939Rm_GetSizeOfUser())
# if (J1939RM_INVALIDHNDOFUSER == STD_ON)
           || J1939Rm_IsInvalidHndOfUser(userId)
# endif
    )
  {
    errorId = J1939RM_E_INVALID_USER;
  }
# if (J1939RM_USERSENDACKOFUSER == STD_ON)
  /* #30 Check that the user is allowed to send Acknowledgment messages. */
  else if (!J1939Rm_IsUserSendAckOfUser(userId))
  {
    errorId = J1939RM_E_INVALID_API;
  }
# endif
  /* #40 Check that the channel is valid. */
  else if (!J1939Rm_Channel_Valid(channel))
  {
    errorId = J1939RM_E_INVALID_NETWORK_ID;
  }
  /* #50 Check that the PGN is valid. */
  else if (!J1939Rm_PGN_Valid(ackPgn))
  {
    errorId = J1939RM_E_INVALID_PGN;
  }
  /* #60 Check that the extended id information is valid. */
  else if (!J1939Rm_AckExtIdInfo_Valid(extIdInfo))
  {
    errorId = J1939RM_E_INVALID_EXTID_INFO;
  }
  /* #70 Check that the Acknowledgment code is valid. */
  else if (!J1939Rm_AckCode_Valid(ackCode))
  {
    errorId = J1939RM_E_INVALID_ACK_CODE;
  }
  /* #80 Check that the source address is valid. */
  else if (!J1939Rm_SA_Valid(ackAddress))
  {
    errorId = J1939RM_E_INVALID_ADDRESS;
  }
  /* #90 Check that the priority is valid. */
  else if (!J1939Rm_Priority_Valid(priority))
  {
    errorId = J1939RM_E_INVALID_PRIO;
  }
#endif
  /* #100 If the initial checks are passed or not required: */
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#if (J1939RM_SUPPORT_ACK_TRANSMISSION == STD_ON)
    /* #110 Proceed with internal send Acknowledgment function. */
    result = J1939Rm_InternalSendAck(userId, channel, ackPgn, extIdInfo, ackCode, ackAddress, priority, broadcast);
#endif
  }

  /* ----- Development Error Report ------------------------------------- */
#if (J1939RM_DEV_ERROR_REPORT == STD_ON)
  if ((errorId != J1939RM_E_NO_ERROR))
  {
    J1939Rm_ReportDetError(J1939RM_SID_SENDACK, errorId);
  }
#endif

  return result;
} /* J1939Rm_SendAck() */ /* PRQA S 6030, 6060, 6080 */ /* MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  J1939Rm_SetState
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, J1939RM_CODE) J1939Rm_SetState(NetworkHandleType channel, uint8 node, J1939Rm_StateType newState)
{
  Std_ReturnType ret = E_NOT_OK;
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = J1939RM_E_NO_ERROR;
#endif

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check that the J1939Rm is initialized. */
  if (J1939Rm_ModuleInitialized != J1939RM_INIT)
  {
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
    errorId = J1939RM_E_UNINIT;
#endif
  }
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check that the given channel is valid. */
  else if (!J1939Rm_Channel_Valid(channel))
  {
    errorId = J1939RM_E_INVALID_NETWORK_ID;
  }
  /* #30 Check that the given node is valid (Node id smaller than count of configured nodes). */
  else if (node >= J1939Rm_GetSizeOfNmNode())
  {
    errorId = J1939RM_E_INVALID_NODE;
  }
  /* #40 Check that the state to set is valid. */
  else if (newState > J1939RM_STATE_ONLINE) /* PRQA S 3356, 3359 */ /* MD_J1939Rm_13.7, MD_J1939Rm_13.7 */
  { /* PRQA S 3201 */ /* MD_J1939Rm_14.1 */
    errorId = J1939RM_E_INVALID_STATE;
  }
#endif
  /* #50 If the initial checks are passed or not required: */
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    ret = E_OK;

    /* #60 Proceed with internal set State function. */
    J1939Rm_InternalSetState(channel, node, newState);
  }

  /* ----- Development Error Report ------------------------------------- */
#if (J1939RM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != J1939RM_E_NO_ERROR)
  {
    J1939Rm_ReportDetError(J1939RM_SID_SETSTATE, errorId);
  }
#endif

  return ret;
} /* J1939Rm_SetState() */

/**********************************************************************************************************************
 *  J1939Rm_RxIndication
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, J1939RM_CODE) J1939Rm_RxIndication(PduIdType RxPduId,
                                              P2CONST(PduInfoType, AUTOMATIC, J1939RM_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = J1939RM_E_NO_ERROR;
#endif

  J1939RM_DUMMY_STATEMENT(PduInfoPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check that the J1939Rm is initialized. */
  if (J1939Rm_ModuleInitialized != J1939RM_INIT)
  {
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
    errorId = J1939RM_E_UNINIT;
#endif
  }
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check if the ID of the received PDU is valid. */
  else if ((RxPduId >= J1939Rm_GetSizeOfRxPdu())
# if (J1939RM_INVALIDHNDOFRXPDU == STD_ON)
           || (J1939Rm_IsInvalidHndOfRxPdu(RxPduId))
# endif
    )
  {
    errorId = J1939RM_E_INVALID_PDU_SDU_ID;
  }
  /* #30 Check if the Pdu info pointer is set. */
  else if (PduInfoPtr == NULL_PTR)
  {
    errorId = J1939RM_E_PARAM_POINTER;
  }
  /* #40 Check if the Sdu data pointer is set. */
  else if (PduInfoPtr->SduDataPtr == NULL_PTR)
  {
    errorId = J1939RM_E_PARAM_POINTER;
  }
#endif
  /* #50 If the initial checks are passed or not required: */
  else
  {
    /* ----- Implementation ----------------------------------------------- */

    /* #60 Proceed with the internal RxIndication function. */
    J1939Rm_InternalRxIndication(RxPduId, PduInfoPtr);
  }

  /* ----- Development Error Report ------------------------------------- */
#if (J1939RM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != J1939RM_E_NO_ERROR)
  {
    J1939Rm_ReportDetError(J1939RM_SID_RXINDICATION, errorId);
  }
#endif
} /* J1939Rm_RxIndication() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  J1939Rm_TxConfirmation
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, J1939RM_CODE) J1939Rm_TxConfirmation(PduIdType TxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
  uint8 errorId = J1939RM_E_NO_ERROR;
#endif

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check that the J1939Rm is initialized. */
  if (J1939Rm_ModuleInitialized != J1939RM_INIT)
  {
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
    errorId = J1939RM_E_UNINIT;
#endif
  }
#if (J1939RM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check if the ID of the transmitted PDU is valid. */
  else if ((TxPduId >= J1939Rm_GetSizeOfTxPdu())
# if (J1939RM_INVALIDHNDOFTXPDU == STD_ON)
           || (J1939Rm_IsInvalidHndOfTxPdu(TxPduId))
# endif
    )
  {
    errorId = J1939RM_E_INVALID_PDU_SDU_ID;
  }
#endif
  /* #30 If the initial checks are passed or not required: */
  else
  {
    /* ----- Implementation ----------------------------------------------- */

    /* #40 Proceed with the internal TxConfirmation function. */
    J1939Rm_InternalTxConfirmation(TxPduId);
  }

  /* ----- Development Error Report ------------------------------------- */
#if (J1939RM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != J1939RM_E_NO_ERROR)
  {
    J1939Rm_ReportDetError(J1939RM_SID_TXCONFIRMATION, errorId);
  }
#endif
} /* J1939Rm_TxConfirmation() */

#define J1939RM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*================================================ J1939RM_STOP_SEC_CODE ===========================================*/

/* Module specific MISRA deviations:

 MD_J1939Rm_13.2
      Reason:     No deviation: MISRA rule 13.2 explicitly allows to use Boolean checks when it is ensured that the
                  tested variable is only used as a Boolean value.
      Risk:       None
      Prevention: N/A

 MD_J1939Rm_13.7
      Reason:     Theoretically, an enum parameter cannot lie outside the values defined for the enum, but practically,
                  this is possible. Therefore, these cases must be checked, and the error detection code is then
                  detected as always false/true.
      Risk:       None
      Prevention: N/A

 MD_J1939Rm_14.1
      Reason:     Theoretically, an enum parameter cannot lie outside the values defined for the enum, but practically,
                  this is possible. Therefore, these cases must be checked, and the error handling code is then
                  detected as unreachable.
      Risk:       None
      Prevention: N/A
*/
