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
/**     \file     Mirror.c
 *      \brief    MIRROR source file
 *
 *      \details  Implementation of Mirror component.
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
#define MIRROR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Mirror.h"
#include "Mirror_Cbk.h"
#include "SchM_Mirror.h"

#include "Can.h"

#if (MIRROR_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

#if (MIRROR_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
# include "EcuM_Error.h"
#endif

#include "vstdlib.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of Mirror header file */
#if (  (MIRROR_SW_MAJOR_VERSION != (2u)) \
    || (MIRROR_SW_MINOR_VERSION != (0u)) \
    || (MIRROR_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of <Mirror>.c and <Mirror>.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (MIRROR_CFG_MAJOR_VERSION != (1u)) \
    || (MIRROR_CFG_MINOR_VERSION != (1u)))
# error "Version numbers of <Mirror>.c and <Mirror>_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Indexes to DoIP protocol fields. */
#define MIRROR_IDX_DOIP_PROTOCOL_VERSION        (0x00u)
#define MIRROR_IDX_DOIP_INV_PROT_VERSION        (0x01u)
#define MIRROR_IDX_DOIP_PAYLOAD_TYPE            (0x02u)
#define MIRROR_IDX_DOIP_PAYLOAD_LEN             (0x04u)

/*! Indexes to Mirror header protocol fields. */
#define MIRROR_IDX_HDR_PROTOCOL_VERSION         (0x08u)
#define MIRROR_IDX_HDR_SEQUENCE_NUMBER          (0x09u)
#define MIRROR_IDX_HDR_BASE_TIMESTAMP           (0x0Au)
#define MIRROR_IDX_HDR_LENGTH                   (0x0Eu)

/*! Indexes to Mirror data protocol fields. */
#define MIRROR_IDX_DATA_REL_TIMESTAMP           (0x00u)
#define MIRROR_IDX_DATA_NETWORK_TYPE            (0x02u)
#define MIRROR_IDX_DATA_NETWORK_ID              (0x03u)

/*! IDX if MIRROR_IDX_DATA_STATE is not used (!= CAN-FD) */
#define MIRROR_IDX_DATA_FRAME_ID                (0x04u)
#define MIRROR_IDX_DATA_PAYLOAD_LEN             (0x08u)
#define MIRROR_IDX_DATA_PAYLOAD                 (0x0Au)

#define MIRROR_IDX_FIRST_DATA_PACKET            (0x10u)

/* size of data packet with its data header according to specified Ford protocol configuration. */
/* future considerations: generated according to configured protocol. */
/* If there are more than one bus type that is mirrored, then this size should be the maximum possible value. */
#define MIRROR_DATA_SIZE                        (0x12u) /* without optional state field! 18 Byte */
#define MIRROR_DATA_HEADER_SIZE                 (0x0Au) /* without payload 10 Byte */

#define MIRROR_BUSTYPE_CAN                      (0u)

#define MIRROR_SOURCECHANNEL_CAN_FILTER_ACTIVE  (1u)
#define MIRROR_NANOSECONDS_TO_MICROSECONDS      (1000u)

#define MIRROR_DESTCHANNELCAN_METADATA_LENGTH                   (4u)
#define MIRROR_DESTCHANNELCAN_QUEUEELEMENT_LENGTHFIELD_SIZE     (1u)
#define MIRROR_DESTCHANNELCAN_QUEUEELEMENT_LENGTHFIELD_POS      (0u)
#define MIRROR_DESTCHANNEL_QUEUE_WRITEIDX_INIT                  (0u)
#define MIRROR_DESTCHANNEL_QUEUE_READIDX_INIT                   (0u)

#if !defined (MIRROR_LOCAL)
# define MIRROR_LOCAL static
#endif

#if !defined (MIRROR_LOCAL_INLINE)
# define MIRROR_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FUNCTIONLIKEMACRO */ /* MD_MSR_19.7 */
# define SchM_Enter_Mirror_QueueLock() SchM_Enter_Mirror_MIRROR_EXCLUSIVE_AREA_QUEUE()
# define SchM_Exit_Mirror_QueueLock()  SchM_Exit_Mirror_MIRROR_EXCLUSIVE_AREA_QUEUE()

# define SchM_Enter_Mirror_TxPduLock() SchM_Enter_Mirror_MIRROR_EXCLUSIVE_AREA_TXPDU()
# define SchM_Exit_Mirror_TxPduLock()  SchM_Exit_Mirror_MIRROR_EXCLUSIVE_AREA_TXPDU()

# define SchM_Enter_Mirror_SourceChannelLock() SchM_Enter_Mirror_MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL()
# define SchM_Exit_Mirror_SourceChannelLock()  SchM_Exit_Mirror_MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL()

#define Mirror_DestChannelQueue_HasFreeElements()       (boolean)(Mirror_DestChannelState.queueFreeElements > 0)
#define Mirror_DestChannelQueue_WriteIdxWrapAround()    (boolean)(Mirror_DestChannelState.queueWriteIdx >= Mirror_GetQueueSizeOfDestChannel(Mirror_CurrentDestChannelIndex))
#define Mirror_DestChannelQueue_ReadIdxWrapAround()     (boolean)(Mirror_DestChannelState.queueReadIdx >= Mirror_GetQueueSizeOfDestChannel(Mirror_CurrentDestChannelIndex))
#define Mirror_DestChannelQueue_IsEmpty()               (boolean)(Mirror_DestChannelState.queueFreeElements == Mirror_GetQueueSizeOfDestChannel(Mirror_CurrentDestChannelIndex))

#if ((MIRROR_DESTINATION_CHANNEL_IP == STD_ON) && ((!defined Mirror_WriteBuffer_HTONL) || (!defined Mirror_WriteBuffer_HTONS))) || (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON)
# define Mirror_Write_Buffer(destBuffer, value)  (*(destBuffer)) = ((uint8)(value))
#endif

#if (MIRROR_DESTINATION_CHANNEL_IP == STD_ON)
/*! Get the size in bytes of the DoIP protocol header. */
# define Mirror_GetDoipHdrSize()                         (0x8u)

/*! Get the DoIP protocol version. */
# define Mirror_GetDoipProtVersion()                     (0x02u)

/*! Get the inverse DoIP protocol version. */
# define Mirror_GetDoipInvProtVersion()                  (0xFDu)

/*! Get the Ford specific DoIP payload type of a mirror message. */
# define Mirror_GetDoipPayloadType()                     ((uint16)0xF002u)

/*! Get the Ford specific DoIP payload type of a mirror message. */
# define Mirror_GetDoipPayloadLen()                      ((uint32)0x0u)

/*! Get the Ford specific Mirror protocol version. */
# define Mirror_GetMirrorProtVersion()                   (0x1u)

/*! Initialize the sequence number. */
# define Mirror_InitSeqNumber()                          ((uint8)0x0u)

/*! Initialize the base time stamp. */
# define Mirror_InitBaseTimestamp()                      ((uint32)0x0u)

/*! Initialize the length of the mirror header. (Only header, no data packets) */
# define Mirror_InitHdrLen()                             ((uint16)0x8u)

# define Mirror_DestChannelIp_QueueHasFreeElements(a)    ((Mirror_DestChannelQueue_HasFreeElements()) || ((Mirror_DestChannelState.queueElementWriteIdx + (a)) <= Mirror_GetTxPduLengthOfDestChannel(Mirror_CurrentDestChannelIndex)))
# define Mirror_DestChannelIp_QueueWriteIdxWrapAround()  Mirror_DestChannelQueue_WriteIdxWrapAround()
# define Mirror_DestChannelIp_QueueReadIdxWrapAround()   Mirror_DestChannelQueue_ReadIdxWrapAround()
# define Mirror_DestChannelIp_QueueIsEmpty()             Mirror_DestChannelQueue_IsEmpty()
# define Mirror_DestChannelIp_QueueGetBufferIdx(a)       (uint16)(Mirror_GetTxPduLengthOfDestChannel(Mirror_CurrentDestChannelIndex) * (a))
# define Mirror_DestChannelIp_QueueGetReadIdx()          Mirror_DestChannelIp_QueueGetBufferIdx(Mirror_DestChannelState.queueReadIdx)
# define Mirror_DestChannelIp_QueueGetWriteIdx()         Mirror_DestChannelIp_QueueGetBufferIdx(Mirror_DestChannelState.queueWriteIdx)

# if (!defined Mirror_WriteBuffer_HTONL)
#  define Mirror_WriteBuffer_HTONL(value, destBuffer)    Mirror_Swap32(value, destBuffer)
# endif

# if (!defined Mirror_WriteBuffer_HTONS)
#  define Mirror_WriteBuffer_HTONS(value, destBuffer)    Mirror_Swap16(value, destBuffer)
# endif
#endif /* (MIRROR_DESTINATION_CHANNEL_IP == STD_ON) */

#if (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON)
# define Mirror_DestChannelCan_QueueHasFreeElements()    Mirror_DestChannelQueue_HasFreeElements()
# define Mirror_DestChannelCan_QueueWriteIdxWrapAround() Mirror_DestChannelQueue_WriteIdxWrapAround()
# define Mirror_DestChannelCan_QueueReadIdxWrapAround()  Mirror_DestChannelQueue_ReadIdxWrapAround()
# define Mirror_DestChannelCan_QueueIsEmpty()            Mirror_DestChannelQueue_IsEmpty()
# define Mirror_DestChannelCan_QueueGetBufferIdx(a)      (uint16)(Mirror_GetTxPduLengthOfDestChannel(Mirror_CurrentDestChannelIndex) + MIRROR_DESTCHANNELCAN_METADATA_LENGTH + \
                                                                  MIRROR_DESTCHANNELCAN_QUEUEELEMENT_LENGTHFIELD_SIZE) * (a)
# define Mirror_DestChannelCan_QueueGetReadIdx()         Mirror_DestChannelCan_QueueGetBufferIdx(Mirror_DestChannelState.queueReadIdx)
# define Mirror_DestChannelCan_QueueGetWriteIdx()        Mirror_DestChannelCan_QueueGetBufferIdx(Mirror_DestChannelState.queueWriteIdx)
#endif /* (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON) */
/* PRQA L:FUNCTIONLIKEMACRO */ /* MD_MSR_19.7 */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define MIRROR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Mode variable */
MIRROR_LOCAL VAR(Mirror_ModeType, MIRROR_VAR_NOINIT) Mirror_GlobalMode;
MIRROR_LOCAL VAR(uint8, MIRROR_VAR_NOINIT) Mirror_CurrentDestChannelIndex;
MIRROR_LOCAL VAR(Mirror_DestChannelStateType, MIRROR_VAR_NOINIT) Mirror_DestChannelState;
MIRROR_LOCAL VAR(boolean, MIRROR_VAR_NOINIT) Mirror_DestChannel_TxPduLocked;
P2CONST(Mirror_ConfigType, AUTOMATIC, MIRROR_PBCFG) Mirror_ConfigDataPtr;

#if (MIRROR_DESTINATION_CHANNEL_IP == STD_ON)
MIRROR_LOCAL VAR(boolean, MIRROR_VAR_NOINIT) Mirror_DestChannelIp_IsTransmit;
#endif /* (MIRROR_DESTINATION_CHANNEL_IP == STD_ON) */

#define MIRROR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define MIRROR_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

MIRROR_LOCAL VAR(uint16, MIRROR_VAR_NOINIT) Mirror_TxConfTimeout;

#if (MIRROR_DESTINATION_CHANNEL_IP == STD_ON)
MIRROR_LOCAL VAR(uint16, MIRROR_VAR_NOINIT) Mirror_DestChannelIp_TxPduTimeout;
#endif /* (MIRROR_DESTINATION_CHANNEL_IP == STD_ON) */

#define MIRROR_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define MIRROR_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (MIRROR_DEV_ERROR_DETECT == STD_ON)
/*! Initialization state of the module */
MIRROR_LOCAL VAR(uint8, MIRROR_VAR_ZERO_INIT) Mirror_ModuleInitialized = MIRROR_UNINIT;
#endif

#if (MIRROR_DESTINATION_CHANNEL_IP == STD_ON)
MIRROR_LOCAL VAR(uint8, MIRROR_VAR_ZERO_INIT) Mirror_DestChannelIp_SeqNumber;
#endif  /* (MIRROR_DESTINATION_CHANNEL_IP == STD_ON) */

#define MIRROR_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define MIRROR_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  Mirror_GetActiveFilterCnt()
 *********************************************************************************************************************/
/*! \brief       Function returns the number of active can filter for the given list.
 *  \details     Only for one source channel.
 *  \param[in]   isFilterActive
 *  \return      Number of active can filter
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
MIRROR_LOCAL_INLINE FUNC(uint8, MIRROR_CODE) Mirror_GetActiveFilterCnt(P2CONST(boolean, AUTOMATIC, MIRROR_APPL_DATA) isFilterActive);

/**********************************************************************************************************************
 *  Mirror_SourceChannel_ProcessFilter()
 *********************************************************************************************************************/
/*! \brief       Function searches for matching filter.
 *  \details     Only active filters will be evaluated.
 *  \param[in]   Controller
 *  \param[in]   dataPtr
 *  \return      TRUE if filter matched, FALSE if not
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
MIRROR_LOCAL_INLINE FUNC(boolean, MIRROR_CODE) Mirror_SourceChannel_ProcessFilter(uint8 Controller, P2CONST(Can_PduType, AUTOMATIC, MIRROR_APPL_DATA) dataPtr);

#if (MIRROR_DESTINATION_CHANNEL_IP == STD_ON)
/**********************************************************************************************************************
 *  Mirror_GetTime()
 *********************************************************************************************************************/
/*! \brief       Returns the current time. 
 *  \details     Time source can be StbM or a user defined function.
 *  \return      Current time in micro seconds
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_IP == STD_ON.
 *********************************************************************************************************************/
MIRROR_LOCAL FUNC(uint32, MIRROR_CODE) Mirror_GetTime(void);

/**********************************************************************************************************************
 *  Mirror_Swap16()
 *********************************************************************************************************************/
/*! \brief       Function converts the given value from host byte order to network byte order.
 *  \details     Can be used for two byte values.
 *  \param[in]   value
 *  \param[out]  destBuffer
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \return      E_OK - success
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_IP == STD_ON.
 *********************************************************************************************************************/
MIRROR_LOCAL FUNC(void, MIRROR_CODE)  Mirror_Swap16(uint16 value, P2VAR(uint8, AUTOMATIC, MIRROR_APPL_DATA) destBuffer);

/**********************************************************************************************************************
 *  Mirror_Swap32()
 *********************************************************************************************************************/
/*! \brief       Function converts the given value from host byte order to network byte order.
 *  \details     Can be used for four byte values.
 *  \param[in]   value
 *  \param[out]  destBuffer
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \return      E_OK - success
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_IP == STD_ON.
 *********************************************************************************************************************/
MIRROR_LOCAL FUNC(void, MIRROR_CODE)  Mirror_Swap32(uint32 value, P2VAR(uint8, AUTOMATIC, MIRROR_APPL_DATA) destBuffer);

/**********************************************************************************************************************
 *  Mirror_DestChannelIp_MainFunction()
 *********************************************************************************************************************/
/*! \brief       Internal main function for destination channel IP.
 *  \details     Handles Tx Confirmation timeout and max PDU delay timeout.
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_IP == STD_ON.
 *********************************************************************************************************************/
MIRROR_LOCAL_INLINE FUNC(void, MIRROR_APPL_CODE)Mirror_DestChannelIp_MainFunction(void);

/**********************************************************************************************************************
 *  Mirror_DestChannelIp_QueueInit()
 *********************************************************************************************************************/
 /*! \brief       Initializes the destination buffers.
  *  \details     Initializes the header and data fields of the destination buffers with their default values.
  *  \pre         This function must be called with interrupt disabled.
  *  \context     TASK
  *  \reentrant   FALSE
  *  \synchronous TRUE
  *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_IP == STD_ON.
  *********************************************************************************************************************/
MIRROR_LOCAL FUNC(void, MIRROR_CODE) Mirror_DestChannelIp_QueueInit(void);

/**********************************************************************************************************************
 *  Mirror_DestChannelIp_QueueWriteUpdateState()
 *********************************************************************************************************************/
/*! \brief       Function checks whether another CAN frame fits into the current used queue element 
                 and prepears all queue handler.
 *  \details     Function marks the current queue element as "ready to send" if necessary.
 *  \param[in]   length
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \return      E_OK - success
 *  \pre         Module is initialized. This function must be called with interrupt disabled.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_IP == STD_ON.
 *********************************************************************************************************************/
MIRROR_LOCAL FUNC(Std_ReturnType, MIRROR_CODE) Mirror_DestChannelIp_QueueWriteUpdateState(uint16 length);

/**********************************************************************************************************************
 *  Mirror_DestChannelIp_QueueUpdateElement()
 *********************************************************************************************************************/
/*! \brief       Function writes data to a queue element.
 *  \details     Write data according to Ford Mirror protocol.
 *  \param[in]   queueElement
 *  \param[in]   canId
 *  \param[in]   controllerId
 *  \param[in]   data
 *  \param[in]   dataLength
 *  \param[in]   channelType
 *  \pre         Module is initialized. This function must be called with interrupt disabled.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_IP == STD_ON.
 *********************************************************************************************************************/
MIRROR_LOCAL_INLINE FUNC(void, MIRROR_CODE) Mirror_DestChannelIp_QueueUpdateElement(P2VAR(Mirror_DestinationQueueBufferType, AUTOMATIC, MIRROR_APPL_DATA) queueElement, uint32 canId,
                                                                                    uint8 controllerId, P2CONST(uint8, AUTOMATIC, MIRROR_APPL_DATA) data, uint8 dataLength,
                                                                                    Mirror_ChannelType channelType);

/**********************************************************************************************************************
 *  Mirror_DestChannelIp_QueueWrite()
 *********************************************************************************************************************/
/*! \brief       Write a can frame into the queue.
 *  \details     The queue implements a FIFO.
 *  \param[in]   canId
 *  \param[in]   controllerId
 *  \param[in]   data
 *  \param[in]   dataLength
 *  \param[in]   channelType
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \return      E_OK - success
 *  \pre         Module is initialized. This function must be called with interrupt disabled.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_IP == STD_ON.
 *********************************************************************************************************************/
MIRROR_LOCAL_INLINE FUNC(Std_ReturnType, MIRROR_CODE) Mirror_DestChannelIp_QueueWrite(uint32 canId, uint8 controllerId, P2CONST(uint8, AUTOMATIC, MIRROR_APPL_DATA) data, uint8 dataLength,
                                                                                      Mirror_ChannelType channelType);

/**********************************************************************************************************************
 *  Mirror_DestChannelIp_QueueReadUpdateState()
 *********************************************************************************************************************/
/*! \brief       Function updates the queue pointer after a read operation.
 *  \details     For read type delay, also the write pointer will be updated.
 *  \param[in]   readTypeDelay
 *  \pre         Module is initialized. This function must be called with interrupt disabled.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_IP == STD_ON.
 *********************************************************************************************************************/
MIRROR_LOCAL_INLINE FUNC(void, MIRROR_CODE) Mirror_DestChannelIp_QueueReadUpdateState(boolean readTypeDelay);

/**********************************************************************************************************************
 *  Mirror_DestChannelIp_QueueRead()
 *********************************************************************************************************************/
/*! \brief       Function picks the next element to send from the queue.
 *  \details     The queue implements a FIFO. Function reads the oldest queue element.
 *  \param[out]  buffer
 *  \param[in]   readTypeDelay
 *  \return      Length of the queue element
 *  \pre         Module is initialized. This function must be called with interrupt disabled.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_IP == STD_ON.
 *********************************************************************************************************************/
MIRROR_LOCAL_INLINE FUNC(uint16, MIRROR_CODE) Mirror_DestChannelIp_QueueRead(P2VAR(uint8, AUTOMATIC, MIRROR_APPL_DATA) buffer, boolean readTypeDelay);

/**********************************************************************************************************************
 *  Mirror_DestChannelIp_QueueTransmit()
 *********************************************************************************************************************/
/*! \brief       Function transmits a frame of the IP queue.
 *  \details     Function read the next element to send from the queue and triggers the send function.
 *  \param[in]   transmitTypeDelay
 *  \param[in]   destChannelIndex
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_IP == STD_ON.
 *********************************************************************************************************************/
MIRROR_LOCAL FUNC(void, MIRROR_CODE) Mirror_DestChannelIp_QueueTransmit(boolean transmitTypeDelay, uint8 destChannelIndex);
#endif /* (MIRROR_DESTINATION_CHANNEL_IP == STD_ON) */

#if (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON)
/**********************************************************************************************************************
 *  Mirror_MetaData_Write()
 *********************************************************************************************************************/
/*! \brief       Function writes the CAN ID in the meta data buffer.
 *  \details     Use the byte order expected from CanIf.
 *  \param[out]  destBuffer  Destination buffer for the meta data.
 *  \param[in]   canId       CAN ID of a CAN frame.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_CAN == STD_ON.
 *********************************************************************************************************************/
MIRROR_LOCAL_INLINE FUNC(void, MIRROR_CODE) Mirror_MetaData_Write(P2VAR(uint8, AUTOMATIC, MIRROR_APPL_DATA) destBuffer, uint32 canId);

/**********************************************************************************************************************
 *  Mirror_DestChannelCan_MainFunction()
 *********************************************************************************************************************/
/*! \brief       Internal main function for destination channel CAN.
 *  \details     Handles Tx Confirmation timeout.
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_CAN == STD_ON.
 *********************************************************************************************************************/
MIRROR_LOCAL_INLINE FUNC(void, MIRROR_APPL_CODE)Mirror_DestChannelCan_MainFunction(void);

/**********************************************************************************************************************
 *  Mirror_DestChannelIp_QueueInit()
 *********************************************************************************************************************/
 /*! \brief       Initializes the CAN destination queue.
  *  \details     Initializes all pointer and the queue buffer.
  *  \pre         This function must be called with interrupt disabled.
  *  \context     TASK
  *  \reentrant   FALSE
  *  \synchronous TRUE
  *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_CAN == STD_ON.
  *********************************************************************************************************************/
MIRROR_LOCAL FUNC(void, MIRROR_CODE) Mirror_DestChannelCan_QueueInit(void);

/**********************************************************************************************************************
 *  Mirror_DestChannelCan_QueueWrite()
 *********************************************************************************************************************/
/*! \brief       Write a incoming can frame into the queue.
 *  \details     The queue implements a FIFO.
 *  \param[in]   newQueueElementData      Data to be stored in the queue.
 *  \param[in]   newQueueElementDataSize  Size of the given data.
 *  \param[in]   canId                    ID of the can frame.
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \return      E_OK - success
 *  \pre         Module is initialized. This function must be called with interrupt disabled.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_CAN == STD_ON.
 *********************************************************************************************************************/
MIRROR_LOCAL_INLINE FUNC(Std_ReturnType, MIRROR_CODE) Mirror_DestChannelCan_QueueWrite(P2CONST(uint8, AUTOMATIC, MIRROR_APPL_DATA) newQueueElementData, uint8 newQueueElementDataSize,
                                                                                       uint32 canId);

/**********************************************************************************************************************
 *  Mirror_DestChannelCan_QueueRead()
 *********************************************************************************************************************/
/*! \brief       Read a can frame from the queue.
 *  \details     The queue implements a FIFO. Function reads the oldest queue element.
 *  \param[out]  buffer      Buffer for the data to be read of the queue.
 *  \return      Length of the copied data.    
 *  \pre         Module is initialized. This function must be called with interrupt disabled.
 *  \context     ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_CAN == STD_ON.
 *********************************************************************************************************************/
MIRROR_LOCAL_INLINE FUNC(uint8, MIRROR_CODE) Mirror_DestChannelCan_QueueRead(P2VAR(uint8, AUTOMATIC, MIRROR_APPL_DATA) buffer);

/**********************************************************************************************************************
 *  Mirror_DestChannelCan_QueueTransmit()
 *********************************************************************************************************************/
/*! \brief       Function transmits a frame of the CAN queue.
 *  \details     Function read the next element to send from the queue and triggers the send function.
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \param[in]   destChannelIndex
 *  \return      E_OK - success
 *  \pre         Module is initialized.
 *  \context     ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_CAN == STD_ON.
 *********************************************************************************************************************/
MIRROR_LOCAL_INLINE FUNC(void, MIRROR_CODE) Mirror_DestChannelCan_QueueTransmit(uint8 destChannelIndex);

/**********************************************************************************************************************
 *  Mirror_DestChannelCan_DirectTransmit()
 *********************************************************************************************************************/
/*! \brief       Fast transmit function for CAN - CAN mirroring.
 *  \details     Function is used when queuing is not necessary.
 *  \param[in]   DataPtr    Pointer to the data of the CAN message.
 *  \param[in]   destChannelIndex
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \return      E_OK - success
 *  \pre         Module is initialized.
 *  \context     ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This service is only available if MIRROR_DESTINATION_CHANNEL_CAN == STD_ON.
 *********************************************************************************************************************/
MIRROR_LOCAL_INLINE FUNC(Std_ReturnType, MIRROR_CODE) Mirror_DestChannelCan_DirectTransmit(CONST(Can_PduInfoPtrType, AUTOMATIC) DataPtr, uint8 destChannelIndex);
#endif /* (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON) */

/**********************************************************************************************************************
 *  Mirror_DestChannel_Transmit()
 *********************************************************************************************************************/
/*! \brief       Function transmits a PDU on the destination channel.
 *  \details     Function is used from IP and CAN destination channel.
 *  \param[in]   mirrorPdu  PDU to transmit.
 *  \param[in]   destChannelIndex
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \return      E_OK - success
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
  *********************************************************************************************************************/
MIRROR_LOCAL FUNC(Std_ReturnType, MIRROR_CODE) Mirror_DestChannel_Transmit(PduInfoType mirrorPdu, uint8 destChannelIndex);

/**********************************************************************************************************************
 *  Mirror_ProcessCanFrame()
 *********************************************************************************************************************/
/*! \brief       Handles the destination bus specific actions to process the incoming frame.
 *  \details     Function is only called for can frames that passed the filter.
 *  \param[in]   Controller    Reference to a CAN controller.
 *  \param[in]   DataPtr       Pointer to the data of the CAN message.
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \return      E_OK - success
 *  \pre         Module is initialized.
 *  \context     ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
MIRROR_LOCAL_INLINE FUNC(Std_ReturnType, MIRROR_CODE) Mirror_ProcessCanFrame(uint8 Controller, CONST(Can_PduInfoPtrType, AUTOMATIC) DataPtr);

/**********************************************************************************************************************
 *  Mirror_ReportCanFrame()
 *********************************************************************************************************************/
/*! \brief       Accepts a CAN data packet.
 *  \details     Accepts a CAN data packet and stores it in the destination buffer.
 *  \param[in]   Controller    Reference to a CAN controller.
 *  \param[in]   DataPtr       Pointer to the data of the CAN message.
 *  \return      CAN_NOT_OK - Data packet was not accepted.
 *  \return      CAN_OK - success
 *  \pre         -
 *  \context     ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Can_ReturnType, MIRROR_CODE) Mirror_ReportCanFrame(uint8 Controller, P2CONST(Can_PduType, AUTOMATIC, MIRROR_APPL_DATA) DataPtr); /* PRQA S 3451 */ /* MD_Mirror_8.8 */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  Mirror_GetActiveFilterCnt()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
MIRROR_LOCAL_INLINE FUNC(uint8, MIRROR_CODE) Mirror_GetActiveFilterCnt(P2CONST(boolean, AUTOMATIC, MIRROR_APPL_DATA) isFilterActive)
{
  uint8_least i;
  uint8 retVal = 0;

  /* #10 Go through the list of given filters and count the number of active filters. */
  for(i = 0; i < MIRROR_SOURCECHANNELCAN_NUMBER_OF_FILTERS; i++)
  {
    if(isFilterActive[i] == MIRROR_SOURCECHANNEL_CAN_FILTER_ACTIVE)
    {
      retVal++;
    }
  }

  /* #20 Return the number of active filters. */
  return retVal;
} /* Mirror_GetActiveFilterCnt() */

/**********************************************************************************************************************
 * Mirror_SourceChannel_ProcessFilter()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
MIRROR_LOCAL_INLINE FUNC(boolean, MIRROR_CODE) Mirror_SourceChannel_ProcessFilter(uint8 Controller, P2CONST(Can_PduType, AUTOMATIC, MIRROR_APPL_DATA) dataPtr)
{
  uint8_least i;
  boolean filterMatched = FALSE;

  /* #10 Get the list of filter for the source channel of the given message. */
  Mirror_FilterStateType tmpFilterOfController = Mirror_GetCurrentCanFilterState(Mirror_GetCurrentCanFilterStateIdxOfCanController(Controller));  /* PRQA S 1031, 0694 */ /* MD_MSR_1.1, MD_Mirror_9.2 */
  uint8 activeFilterCnt = Mirror_GetActiveFilterCnt(tmpFilterOfController.isFilterActive);

  /* #20 If there is a active filter, search for a filter that matches the CAN ID of the given message: */
  if(activeFilterCnt > 0)
  {
    for(i = 0; i < MIRROR_SOURCECHANNELCAN_NUMBER_OF_FILTERS; i++)
    {
      if(tmpFilterOfController.isFilterActive[i] == TRUE)
      {
        /* #30 If a filter is matching the CAN ID of the message, return TRUE otherwise return FALSE. */
#if (MIRROR_SOURCE_CHANNEL_FILTER_ID == STD_ON)
        if((dataPtr->id & tmpFilterOfController.filter[i].filterValueHighOrMask.id32) == (tmpFilterOfController.filter[i].filterValueLowOrId.id32 & tmpFilterOfController.filter[i].filterValueHighOrMask.id32))
#else
        if((dataPtr->id >= tmpFilterOfController.filter[i].filterValueLowOrId.id32) && (dataPtr->id <= tmpFilterOfController.filter[i].filterValueHighOrMask.id32))
#endif
        {
          filterMatched = TRUE;
          break;
        }
      }
    }
  }
  /* #40 Otherwise, return TRUE (No filter, Message pass). */
  else
  {
    filterMatched = TRUE;
  }

  return filterMatched;
} /* Mirror_SourceChannel_ProcessFilter() */

#if (MIRROR_DESTINATION_CHANNEL_IP == STD_ON)
/**********************************************************************************************************************
 *  Mirror_GetTime()
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
MIRROR_LOCAL FUNC(uint32, MIRROR_CODE) Mirror_GetTime(void)
{
  uint32 retVal = 0;

/* #10 If StbM is enabled: */
# if(MIRROR_STBMID == STD_ON)
  StbM_TimeStampType timeStamp;
  StbM_UserDataType userData;

  /* #20 Get the current system time from StbM. */
  if(StbM_GetCurrentTime(Mirror_GetStbMId(), &timeStamp, &userData) == E_OK)
  {
    retVal = timeStamp.nanoseconds / MIRROR_NANOSECONDS_TO_MICROSECONDS;
  }
# endif

/* #30 If the timing user callout is enabled: */
# if(MIRROR_TIMINGUSERCALLOUT == STD_ON)
  /* #40 Call the user callout to get the current time. */
  retVal = Mirror_GetTimingUserCallout()();
# endif

  return retVal;
} /* Mirror_GetTime() */

/**********************************************************************************************************************
 *  Mirror_Swap16()
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
MIRROR_LOCAL FUNC(void, MIRROR_CODE)Mirror_Swap16(uint16 value, P2VAR(uint8, AUTOMATIC, MIRROR_APPL_DATA) destBuffer)
{
/* #10 If the byte order of the system is "LOW_BYTE_FIRST": */
# if(CPU_BYTE_ORDER ==  LOW_BYTE_FIRST)
  /* #20 Copy two bytes from the source pointer to the destination pointer and perform endianness conversion. */
  Mirror_Write_Buffer(&destBuffer[0], (uint8) (value >> 8));
  Mirror_Write_Buffer(&destBuffer[1], (uint8) value);
/* #30 If the byte order of the system is "HIGH_BYTE_FIRST": */
# else
  /* #40 Copy two bytes from the source pointer to the destination pointer. */
  Mirror_Write_Buffer(&destBuffer[0], (uint8) value);
  Mirror_Write_Buffer(&destBuffer[1], (uint8) (value >> 8));
# endif
} /* Mirror_Swap16() */

/**********************************************************************************************************************
 *  Mirror_Swap32()
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
MIRROR_LOCAL FUNC(void, MIRROR_CODE) Mirror_Swap32(uint32 value, P2VAR(uint8, AUTOMATIC, MIRROR_APPL_DATA) destBuffer)
{
/* #10 If the byte order of the system is "LOW_BYTE_FIRST": */
# if(CPU_BYTE_ORDER ==  LOW_BYTE_FIRST)
  /* #20 Copy four bytes from the source pointer to the destination pointer and perform endianness conversion. */
  Mirror_Write_Buffer(&destBuffer[0], (uint8) (value >> 24));
  Mirror_Write_Buffer(&destBuffer[1], (uint8) (value >> 16));
  Mirror_Write_Buffer(&destBuffer[2], (uint8) (value >> 8));
  Mirror_Write_Buffer(&destBuffer[3], (uint8) value);
/* #30 If the byte order of the system is "HIGH_BYTE_FIRST": */
# else
  /* #40 Copy four bytes from the source pointer to the destination pointer. */
  Mirror_Write_Buffer(&destBuffer[0], (uint8) value);
  Mirror_Write_Buffer(&destBuffer[1], (uint8) (value >> 8));
  Mirror_Write_Buffer(&destBuffer[2], (uint8) (value >> 16));
  Mirror_Write_Buffer(&destBuffer[3], (uint8) (value >> 24));
# endif
} /* Mirror_Swap32() */

/**********************************************************************************************************************
 *  Mirror_DestChannelIp_MainFunction()
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
 *
 *
 *
 *
 */
MIRROR_LOCAL_INLINE FUNC(void, MIRROR_APPL_CODE) Mirror_DestChannelIp_MainFunction(void)
{
  uint8 destChannelIndex = 0;

  /* #10 Enter MIRROR_EXCLUSIVE_AREA_TXPDU */
  SchM_Enter_Mirror_TxPduLock();

  if(Mirror_GetChannelTypeOfDestChannel(Mirror_CurrentDestChannelIndex) == MIRROR_E_MIRROR_CHANNEL_IP_CHANNELTYPEOFDESTCHANNEL)
  {
    destChannelIndex = Mirror_CurrentDestChannelIndex;

    /* #20 If at least one source bus is active, update the PDU delay timeout counter. */
    ++Mirror_DestChannelIp_TxPduTimeout;

    /* #30 If the timeout counter reaches the configured maximum delay and
     *     the Mirror Tx PDU is ready for a new transmission: */
    if(Mirror_DestChannelIp_TxPduTimeout == Mirror_GetTxPduMaxDelayOfDestChannel(destChannelIndex))
    {
      Mirror_DestChannelIp_TxPduTimeout = 0;

      if(!Mirror_DestChannel_TxPduLocked)
      {
        /* #40 Exit MIRROR_EXCLUSIVE_AREA_TXPDU */
        SchM_Exit_Mirror_TxPduLock();

        /* #50 Trigger the transmission of the current queued messages. */
        Mirror_DestChannelIp_QueueTransmit(TRUE, destChannelIndex);
      }
      /* #60 Otherwise, Exit MIRROR_EXCLUSIVE_AREA_TXPDU */
      else
      {
        SchM_Exit_Mirror_TxPduLock();
      }
    }
    /* #70 Otherwise, Exit MIRROR_EXCLUSIVE_AREA_TXPDU */
    else
    {
      SchM_Exit_Mirror_TxPduLock();
    }

    /* #80 Enter MIRROR_EXCLUSIVE_AREA_TXPDU */
    SchM_Enter_Mirror_TxPduLock();

    /* #90 If the Tx confirmation timeout must be handled (Transmission in progress) and
     *      the Tx confirmation for the current transmission was not received in time: */
    if(Mirror_DestChannel_TxPduLocked == TRUE)
    {
      --Mirror_TxConfTimeout;

      if(Mirror_TxConfTimeout == 0)
      {
        /* #100 Reset the Tx PDU. */
        Mirror_DestChannel_TxPduLocked = FALSE;

        /* #110 Exit MIRROR_EXCLUSIVE_AREA_TXPDU */
        SchM_Exit_Mirror_TxPduLock();

        /* #120 Enter MIRROR_EXCLUSIVE_AREA_QUEUE */
        SchM_Enter_Mirror_QueueLock();

        /* #130 Flush the queue. */
        Mirror_DestChannelIp_QueueInit();

        /* #140 Exit MIRROR_EXCLUSIVE_AREA_QUEUE */
        SchM_Exit_Mirror_QueueLock();
# if (MIRROR_DEV_ERROR_REPORT == STD_ON)
        (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_MAINFUNCTION, MIRROR_E_TIMEOUT_TXCONF);
# endif
      }
      /* #150 Otherwise, Exit MIRROR_EXCLUSIVE_AREA_TXPDU */
      else
      {
        SchM_Exit_Mirror_TxPduLock();
      }
    }
    /* #160 Otherwise, Exit MIRROR_EXCLUSIVE_AREA_TXPDU */
    else
    {
      SchM_Exit_Mirror_TxPduLock();
    }
  }
  else
  {
    SchM_Exit_Mirror_TxPduLock();
  }
} /* Mirror_DestChannelIp_MainFunction() */

/**********************************************************************************************************************
 *  Mirror_DestChannelIp_QueueInit()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
MIRROR_LOCAL FUNC(void, MIRROR_CODE) Mirror_DestChannelIp_QueueInit(void)
{
  uint16_least i;
  uint8* destChannelQueue = &Mirror_GetDestinationQueueBuffer(0);

  /* #10 Reset the queue buffer and set the queue state variable to the initial values. */
  VStdLib_MemSet(destChannelQueue, 0x00, (Mirror_GetTxPduLengthOfDestChannel(Mirror_CurrentDestChannelIndex) * Mirror_GetQueueSizeOfDestChannel(Mirror_CurrentDestChannelIndex)));

  Mirror_DestChannelState.queueFreeElements = Mirror_GetQueueSizeOfDestChannel(Mirror_CurrentDestChannelIndex);
  Mirror_DestChannelState.queueReadIdx = MIRROR_DESTCHANNEL_QUEUE_READIDX_INIT;
  Mirror_DestChannelState.queueWriteIdx = MIRROR_DESTCHANNEL_QUEUE_WRITEIDX_INIT;
  Mirror_DestChannelState.queueElementWriteIdx = MIRROR_DESTCHANNEL_QUEUE_WRITEIDX_INIT;
  Mirror_DestChannelState.writeTimeStamp = 0;
  Mirror_DestChannelIp_IsTransmit = FALSE;
  Mirror_DestChannel_TxPduLocked = FALSE;

  /* #20 Initialize the static part of all queue elements. */
  for(i = 0; i < Mirror_GetQueueSizeOfDestChannel(Mirror_CurrentDestChannelIndex); ++i)
  {
    uint16_least tempWriteIdx = Mirror_GetTxPduLengthOfDestChannel(Mirror_CurrentDestChannelIndex) * i;

    destChannelQueue[tempWriteIdx + MIRROR_IDX_DOIP_PROTOCOL_VERSION] = Mirror_GetDoipProtVersion();
    destChannelQueue[tempWriteIdx + MIRROR_IDX_DOIP_INV_PROT_VERSION] = Mirror_GetDoipInvProtVersion();
    Mirror_WriteBuffer_HTONS(Mirror_GetDoipPayloadType(), &destChannelQueue[tempWriteIdx + MIRROR_IDX_DOIP_PAYLOAD_TYPE]);
    destChannelQueue[tempWriteIdx + MIRROR_IDX_HDR_PROTOCOL_VERSION] = Mirror_GetMirrorProtVersion();
  }
} /* Mirror_DestChannelIp_QueueInit() */

/**********************************************************************************************************************
 * Mirror_DestChannelIp_QueueWriteUpdateState()
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
 */
MIRROR_LOCAL FUNC(Std_ReturnType, MIRROR_CODE) Mirror_DestChannelIp_QueueWriteUpdateState(uint16 length)
{
  Std_ReturnType retVal = E_NOT_OK;
  boolean updatePointer = FALSE;

  /* #10 If the queue state must be updated before a write operation: */
  if(length > 0)
  {
    /* #20 If there is a free element in the queue: */
    if(Mirror_DestChannelIp_QueueHasFreeElements(length) == TRUE)
    {
      if(Mirror_DestChannelIp_QueueIsEmpty() == TRUE)
      {
        Mirror_DestChannelState.queueFreeElements--;
      }
      else
      {
        /* #30 If there is not enough space in the current queue element to store the new data: */
        if((Mirror_DestChannelState.queueElementWriteIdx + length) >= Mirror_GetTxPduLengthOfDestChannel(Mirror_CurrentDestChannelIndex))
        {
          /* #40 Indicate that a new queue element is needed and that the old queue element is ready for transmission.  */
          updatePointer = TRUE;
          Mirror_DestChannelIp_IsTransmit = TRUE;
        }
      }

      retVal = E_OK;
    }
  }
  /* #50 Otherwise, the queue state must be updated after a delayed transmit triggered from the main function. */
  else
  {
    updatePointer = TRUE;

    retVal = E_OK;
  }

  /* #60 If a delayed transmit was performed or a new queue element is needed: */
  if(updatePointer == TRUE)
  {
    /* #70 Update the queue state. */
    Mirror_DestChannelState.queueFreeElements--;
    Mirror_DestChannelState.queueWriteIdx++;
    Mirror_DestChannelState.queueElementWriteIdx = MIRROR_DESTCHANNEL_QUEUE_WRITEIDX_INIT;

    if(Mirror_DestChannelIp_QueueWriteIdxWrapAround() == TRUE)
    {
      Mirror_DestChannelState.queueWriteIdx = MIRROR_DESTCHANNEL_QUEUE_WRITEIDX_INIT;
    }
  }

  return retVal;
} /* Mirror_DestChannelIp_QueueWriteUpdateState() */

/**********************************************************************************************************************
 * Mirror_DestChannelIp_QueueUpdateElement()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
MIRROR_LOCAL_INLINE FUNC(void, MIRROR_CODE)Mirror_DestChannelIp_QueueUpdateElement(P2VAR(Mirror_DestinationQueueBufferType, AUTOMATIC, MIRROR_APPL_DATA) queueElement, uint32 canId, uint8 controllerId,
                                                                                   P2CONST(uint8, AUTOMATIC, MIRROR_APPL_DATA) data, uint8 dataLength, Mirror_ChannelType channelType)
{
  /* #10 If it is a new queue element with no data: */
  if(Mirror_DestChannelState.queueElementWriteIdx == MIRROR_DESTCHANNEL_QUEUE_WRITEIDX_INIT)
  {
    /* #20 Initialize the new queue element by inserting the time stamp and sequence number. */
    Mirror_DestChannelState.queueElementWriteIdx = MIRROR_IDX_FIRST_DATA_PACKET;

    Mirror_DestChannelState.writeTimeStamp = Mirror_GetTime() - Mirror_DestChannelState.startTimeStamp;
    Mirror_WriteBuffer_HTONL(Mirror_DestChannelState.writeTimeStamp, &queueElement[MIRROR_IDX_HDR_BASE_TIMESTAMP]);

    queueElement[MIRROR_IDX_HDR_SEQUENCE_NUMBER] = Mirror_DestChannelIp_SeqNumber;
    Mirror_DestChannelIp_SeqNumber++;
  }

  /* #30 Write the new data to queue element. */
  Mirror_WriteBuffer_HTONS((uint16) (Mirror_GetTime() - Mirror_DestChannelState.writeTimeStamp), &queueElement[Mirror_DestChannelState.queueElementWriteIdx + MIRROR_IDX_DATA_REL_TIMESTAMP]);
  queueElement[Mirror_DestChannelState.queueElementWriteIdx + MIRROR_IDX_DATA_NETWORK_TYPE] = (uint8) channelType;
  queueElement[Mirror_DestChannelState.queueElementWriteIdx + MIRROR_IDX_DATA_NETWORK_ID] = Mirror_GetUserChannelIdOfSourceChannel(Mirror_GetSourceChannelIdxOfCanController(controllerId));
  Mirror_WriteBuffer_HTONL(canId, &queueElement[Mirror_DestChannelState.queueElementWriteIdx + MIRROR_IDX_DATA_FRAME_ID]);
  Mirror_WriteBuffer_HTONS(dataLength, &queueElement[Mirror_DestChannelState.queueElementWriteIdx + MIRROR_IDX_DATA_PAYLOAD_LEN]);
  VStdLib_MemCpy((&queueElement[Mirror_DestChannelState.queueElementWriteIdx + MIRROR_IDX_DATA_PAYLOAD]), data, dataLength);

  Mirror_DestChannelState.queueElementWriteIdx += dataLength + MIRROR_DATA_HEADER_SIZE;
  Mirror_WriteBuffer_HTONL((Mirror_DestChannelState.queueElementWriteIdx - (uint16) Mirror_GetDoipHdrSize()), &queueElement[MIRROR_IDX_DOIP_PAYLOAD_LEN]);
  Mirror_WriteBuffer_HTONS((uint16) (Mirror_DestChannelState.queueElementWriteIdx - (uint16) Mirror_GetDoipHdrSize()), &queueElement[MIRROR_IDX_HDR_LENGTH]);
} /* Mirror_DestChannelIp_QueueUpdateElement() */ /* PRQA S 6060 1 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
 * Mirror_DestChannelIp_QueueWrite()
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
MIRROR_LOCAL_INLINE FUNC(Std_ReturnType, MIRROR_CODE) Mirror_DestChannelIp_QueueWrite(uint32 canId, uint8 controllerId, P2CONST(uint8, AUTOMATIC, MIRROR_APPL_DATA) data, uint8 dataLength,
                                                                                      Mirror_ChannelType channelType)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8 queueElementLength = dataLength + MIRROR_DATA_HEADER_SIZE;

  /* #10 Check if the new data can be stored in the queue. */
  if(queueElementLength <= MIRROR_DATA_SIZE)
  {
    retVal = Mirror_DestChannelIp_QueueWriteUpdateState(queueElementLength);

    /* #20 If the queue is ready for new data: */
    if(retVal == E_OK)
    {
      /* #30 Write the new data to the queue. */
      Mirror_DestinationQueueBufferType *queueElement = &Mirror_GetDestinationQueueBuffer(Mirror_DestChannelIp_QueueGetWriteIdx());

      Mirror_DestChannelIp_QueueUpdateElement(queueElement, canId, controllerId, data, dataLength, channelType);
    }
    /* #40 Otherwise, the queue must be full, flush the queue. */
    else
    {
      Mirror_DestChannelIp_QueueInit();
    }
  }

  return retVal;
} /* Mirror_DestChannelIp_QueueWrite() */

/**********************************************************************************************************************
 * Mirror_DestChannelIp_QueueReadUpdateState()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
MIRROR_LOCAL_INLINE FUNC(void, MIRROR_CODE) Mirror_DestChannelIp_QueueReadUpdateState(boolean readTypeDelay)
{
  /* #10 Update the state of the queue after a read operation. Release one used element and set the read pointer to the next element
   *     and handle a queue wrap around if necessary. */
  Mirror_DestChannelState.queueFreeElements++;
  Mirror_DestChannelState.queueReadIdx++;

  if(Mirror_DestChannelIp_QueueReadIdxWrapAround() == TRUE)
  {
    Mirror_DestChannelState.queueReadIdx = MIRROR_DESTCHANNEL_QUEUE_READIDX_INIT;
  }

  /* #20 If the read operation is triggered for a delayed transmission (timeout), also the write state of the queue must be updated. */
  if(readTypeDelay == TRUE)
  {
    (void)Mirror_DestChannelIp_QueueWriteUpdateState(0);
  }
} /* Mirror_DestChannelIp_QueueReadUpdateState() */

/**********************************************************************************************************************
 * Mirror_DestChannelIp_QueueRead()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
MIRROR_LOCAL_INLINE FUNC(uint16, MIRROR_CODE) Mirror_DestChannelIp_QueueRead(P2VAR(uint8, AUTOMATIC, MIRROR_APPL_DATA) buffer, boolean readTypeDelay)
{
  uint32 retVal = 0;

  /* #10 If there are data in the queue: */
  if(!Mirror_DestChannelIp_QueueIsEmpty())
  {
    /* #20 Get the data from the queue and copy it to the destination buffer.  */
    Mirror_DestinationQueueBufferType *queueElement = &Mirror_GetDestinationQueueBuffer(Mirror_DestChannelIp_QueueGetReadIdx());
    uint32 queueElementDataLength = 0;

    VStdLib_MemCpy(&queueElementDataLength, &queueElement[MIRROR_IDX_DOIP_PAYLOAD_LEN], sizeof(queueElementDataLength));
    Mirror_WriteBuffer_HTONL(queueElementDataLength, (uint8*)&retVal); /* PRQA S 0310 */ /* MD_Mirror_0310 */

    if(retVal > 0)
    {
      retVal += (uint32) Mirror_GetDoipHdrSize();
      VStdLib_MemCpy(buffer, queueElement, retVal);

      /* #30 Update the queue state after the read operation. */
      Mirror_DestChannelIp_QueueReadUpdateState(readTypeDelay);
    }
  }

  return (uint16)retVal;
} /* Mirror_DestChannelIp_QueueRead() */

/**********************************************************************************************************************
 * Mirror_DestChannelIp_QueueTransmit()
 **********************************************************************************************************************/
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
MIRROR_LOCAL FUNC(void, MIRROR_CODE) Mirror_DestChannelIp_QueueTransmit(boolean transmitTypeDelay, uint8 destChannelIndex)
{
  PduInfoType mirror_IpPdu;
  uint8 messageBuffer[MIRROR_DESTINATION_CHANNEL_IP_QUEUEELEMENT_MAX_SIZE];

  /* #10 If the Mirror Tx PDU is ready for a new transmission: */
  if(!Mirror_DestChannel_TxPduLocked)
  {
    /* #20 Enter MIRROR_EXCLUSIVE_AREA_QUEUE */
    SchM_Enter_Mirror_QueueLock();

    /* #30 Get the next element to transmit from the queue. */
    mirror_IpPdu.SduDataPtr = (SduDataPtrType) messageBuffer;
    mirror_IpPdu.SduLength = Mirror_DestChannelIp_QueueRead(messageBuffer, transmitTypeDelay);

    /* #40 Exit MIRROR_EXCLUSIVE_AREA_QUEUE */
    SchM_Exit_Mirror_QueueLock();

    if(mirror_IpPdu.SduLength > 0)
    {
      /* #50 Transmit the data and reset the delay timeout. */
      (void)Mirror_DestChannel_Transmit(mirror_IpPdu, destChannelIndex);

      Mirror_DestChannelIp_TxPduTimeout = 0;
      Mirror_DestChannelIp_IsTransmit = FALSE;
    }
  }
} /* Mirror_DestChannelIp_QueueTransmit() */
#endif /* (MIRROR_DESTINATION_CHANNEL_IP == STD_ON) */

#if (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON)
/**********************************************************************************************************************
 *  Mirror_MetaData_Write()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
MIRROR_LOCAL_INLINE FUNC(void, MIRROR_CODE) Mirror_MetaData_Write(P2VAR(uint8, AUTOMATIC, MIRROR_APPL_DATA) destBuffer, uint32 canId)
{
  /* #10 Write the CAN ID in the meta data buffer. Use the format required from CAN. */
  Mirror_Write_Buffer(&destBuffer[0], (uint8) canId);
  Mirror_Write_Buffer(&destBuffer[1], (uint8) (canId >> 8));
  Mirror_Write_Buffer(&destBuffer[2], (uint8) (canId >> 16));
  Mirror_Write_Buffer(&destBuffer[3], (uint8) (canId >> 24));
} /* Mirror_MetaData_Write() */

/**********************************************************************************************************************
 *  Mirror_DestChannelCan_MainFunction()
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
MIRROR_LOCAL_INLINE FUNC(void, MIRROR_APPL_CODE) Mirror_DestChannelCan_MainFunction(void)
{
  /* #10 If the Tx confirmation timeout must be handled (Transmission in progress): */
  if(Mirror_DestChannel_TxPduLocked == TRUE)
  {
    /* #20 Enter MIRROR_EXCLUSIVE_AREA_TXPDU */
    SchM_Enter_Mirror_TxPduLock();

    --Mirror_TxConfTimeout;

    /* #30 If the Tx confirmation for the current transmission was not received in time: */
    if(Mirror_TxConfTimeout == 0)
    {
      /* #40 Reset the Tx PDU. */
      Mirror_DestChannel_TxPduLocked = FALSE;

      /* #50 Exit MIRROR_EXCLUSIVE_AREA_TXPDU */
      SchM_Exit_Mirror_TxPduLock();

      /* #60 Enter MIRROR_EXCLUSIVE_AREA_QUEUE */
      SchM_Enter_Mirror_QueueLock();

      /* #70 Flush the queue. */
      Mirror_DestChannelCan_QueueInit();

      /* #80 Exit MIRROR_EXCLUSIVE_AREA_QUEUE */
      SchM_Exit_Mirror_QueueLock();

# if (MIRROR_DEV_ERROR_REPORT == STD_ON)
      (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_MAINFUNCTION, MIRROR_E_TIMEOUT_TXCONF);
# endif
    }
    /* #90 Otherwise, Exit MIRROR_EXCLUSIVE_AREA_TXPDU */
    else
    {
      SchM_Exit_Mirror_TxPduLock();
    }
  }
} /* Mirror_DestChannelCan_MainFunction() */

/**********************************************************************************************************************
 * Mirror_DestChannelCan_QueueInit()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
MIRROR_LOCAL FUNC(void, MIRROR_CODE) Mirror_DestChannelCan_QueueInit(void)
{
  /* #10 Set the queue state variable to the initial values. */
  Mirror_DestChannelState.queueFreeElements = Mirror_GetQueueSizeOfDestChannel(Mirror_CurrentDestChannelIndex);
  Mirror_DestChannelState.queueReadIdx = MIRROR_DESTCHANNEL_QUEUE_READIDX_INIT;
  Mirror_DestChannelState.queueWriteIdx = MIRROR_DESTCHANNEL_QUEUE_WRITEIDX_INIT;
  Mirror_DestChannel_TxPduLocked = FALSE;
} /* Mirror_DestChannelCan_QueueInit() */

/**********************************************************************************************************************
 * Mirror_DestChannelCan_QueueWrite()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
MIRROR_LOCAL_INLINE FUNC(Std_ReturnType, MIRROR_CODE) Mirror_DestChannelCan_QueueWrite(P2CONST(uint8, AUTOMATIC, MIRROR_APPL_DATA) newQueueElementData, uint8 newQueueElementDataSize, uint32 canId)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* #10 If there is a free element in the queue: */
  if(Mirror_DestChannelCan_QueueHasFreeElements() == TRUE)
  {
    Mirror_DestinationQueueBufferType* queueElement = &Mirror_GetDestinationQueueBuffer(Mirror_DestChannelCan_QueueGetWriteIdx());

    /* #20 Store the data in a new queue element. */
    queueElement[MIRROR_DESTCHANNELCAN_QUEUEELEMENT_LENGTHFIELD_POS] = newQueueElementDataSize + MIRROR_DESTCHANNELCAN_METADATA_LENGTH;
    VStdLib_MemCpy(&queueElement[MIRROR_DESTCHANNELCAN_QUEUEELEMENT_LENGTHFIELD_SIZE], newQueueElementData, newQueueElementDataSize);
    Mirror_MetaData_Write((&queueElement[MIRROR_DESTCHANNELCAN_QUEUEELEMENT_LENGTHFIELD_SIZE + newQueueElementDataSize]), canId);

    /* #30 Update the queue state. Block one unused element and set the write pointer to the next element and handle a queue wrap around if necessary. */
    Mirror_DestChannelState.queueFreeElements--;
    Mirror_DestChannelState.queueWriteIdx++;

    if(Mirror_DestChannelCan_QueueWriteIdxWrapAround() == TRUE)
    {
      Mirror_DestChannelState.queueWriteIdx = MIRROR_DESTCHANNEL_QUEUE_WRITEIDX_INIT;
    }

    retVal = E_OK;
  }
  /* #40 Otherwise, flush the queue. */
  else
  {
    Mirror_DestChannelCan_QueueInit();
  }

  return retVal;
} /* Mirror_DestChannelCan_QueueWrite() */

/**********************************************************************************************************************
 * Mirror_DestChannelCan_QueueRead()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
MIRROR_LOCAL_INLINE FUNC(uint8, MIRROR_CODE) Mirror_DestChannelCan_QueueRead(P2VAR(uint8, AUTOMATIC, MIRROR_APPL_DATA) buffer)
{
  uint8 retVal = 0;

  /* #10 If there is a used queue element: */
  if(!Mirror_DestChannelCan_QueueIsEmpty())
  {
    /* #20 Get the data from the queue element and copy it to the destination buffer. */
    Mirror_DestinationQueueBufferType* queueElement = &Mirror_GetDestinationQueueBuffer(Mirror_DestChannelCan_QueueGetReadIdx());
    uint8 queueElementDataLength = queueElement[MIRROR_DESTCHANNELCAN_QUEUEELEMENT_LENGTHFIELD_POS];

    VStdLib_MemCpy(buffer, &queueElement[MIRROR_DESTCHANNELCAN_QUEUEELEMENT_LENGTHFIELD_SIZE], queueElementDataLength);
    retVal = queueElementDataLength;

    /* #30 Update the state of the queue after a read operation. Release one used element and set the read pointer to the next element
     *     and handle a queue wrap around if necessary. */
    Mirror_DestChannelState.queueFreeElements++;
    Mirror_DestChannelState.queueReadIdx++;

    if(Mirror_DestChannelCan_QueueReadIdxWrapAround() == TRUE)
    {
      Mirror_DestChannelState.queueReadIdx = MIRROR_DESTCHANNEL_QUEUE_READIDX_INIT;
    }
  }

  return retVal;
} /* Mirror_DestChannelCan_QueueRead() */

/**********************************************************************************************************************
 * Mirror_DestChannelCan_QueueTransmit()
 **********************************************************************************************************************/
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
MIRROR_LOCAL_INLINE FUNC(void, MIRROR_CODE) Mirror_DestChannelCan_QueueTransmit(uint8 destChannelIndex)
{
  PduInfoType Mirror_CanPdu;
  uint8 messageBuffer[MIRROR_DESTINATION_CHANNEL_CAN_QUEUEELEMENT_MAX_SIZE];

  /* #10 If the Mirror Tx PDU is ready for a new transmission: */
  if(!Mirror_DestChannel_TxPduLocked)
  {
    /* #20 Enter MIRROR_EXCLUSIVE_AREA_QUEUE */
    SchM_Enter_Mirror_QueueLock();

    /* #30 Get the next element to transmit from the queue. */
    Mirror_CanPdu.SduDataPtr = (SduDataPtrType) messageBuffer;
    Mirror_CanPdu.SduLength = Mirror_DestChannelCan_QueueRead(messageBuffer);

    /* #40 Exit MIRROR_EXCLUSIVE_AREA_QUEUE */
    SchM_Exit_Mirror_QueueLock();

    if(Mirror_CanPdu.SduLength > 0)
    {
      /* #50 Transmit the data. */
      (void)Mirror_DestChannel_Transmit(Mirror_CanPdu, destChannelIndex);
    }
  }
} /* Mirror_DestChannelCan_QueueTransmit() */

/**********************************************************************************************************************
 * Mirror_DestChannelCan_DirectTransmit()
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
MIRROR_LOCAL_INLINE FUNC(Std_ReturnType, MIRROR_CODE) Mirror_DestChannelCan_DirectTransmit(CONST(Can_PduInfoPtrType, AUTOMATIC) DataPtr, uint8 destChannelIndex)
{
  Std_ReturnType retVal = E_NOT_OK;
  PduInfoType mirror_CanPdu;
  uint8 messageBuffer[MIRROR_DESTINATION_CHANNEL_CAN_QUEUEELEMENT_MAX_SIZE];

  /* #10 Copy the data and the meta data to the destination buffer. */
  VStdLib_MemCpy(messageBuffer, DataPtr->sdu, DataPtr->length);
  Mirror_MetaData_Write((&messageBuffer[DataPtr->length]), DataPtr->id);

  mirror_CanPdu.SduDataPtr = messageBuffer;
  mirror_CanPdu.SduLength = DataPtr->length + MIRROR_DESTCHANNELCAN_METADATA_LENGTH;

  /* #20 Transmit the data. */
  retVal = Mirror_DestChannel_Transmit(mirror_CanPdu, destChannelIndex);

  return retVal;
} /* Mirror_DestChannelCan_DirectTransmit() */
#endif /* (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON) */

/**********************************************************************************************************************
 * Mirror_DestChannel_Transmit()
 **********************************************************************************************************************/
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
MIRROR_LOCAL FUNC(Std_ReturnType, MIRROR_CODE) Mirror_DestChannel_Transmit(PduInfoType mirrorPdu, uint8 destChannelIndex)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* #10 Enter MIRROR_EXCLUSIVE_AREA_TXPDU */
  SchM_Enter_Mirror_TxPduLock();

  /* #20 If the Mirror Tx PDU is ready for a new transmission: */
  if(!Mirror_DestChannel_TxPduLocked)
  {
    /* #30 Lock the Tx PDU and initialize the Tx confirmation timeout. */
    Mirror_DestChannel_TxPduLocked = TRUE;
    Mirror_TxConfTimeout = Mirror_GetTxConfirmationTimeout();

    /* #40 Exit MIRROR_EXCLUSIVE_AREA_TXPDU */
    SchM_Exit_Mirror_TxPduLock();

    /* #50 Call the PduR transmit function. */
    if(PduR_MirrorTransmit(Mirror_GetPduRTxPduIdOfDestChannel(destChannelIndex), &mirrorPdu) == E_OK)
    {
      retVal = E_OK;
    }
    else
    {
      Mirror_DestChannel_TxPduLocked = FALSE;
    }
  }
  /* #60 Otherwise, Exit MIRROR_EXCLUSIVE_AREA_TXPDU */
  else
  {
    SchM_Exit_Mirror_TxPduLock();
  }

  return retVal;
} /* Mirror_DestChannel_Transmit() */

/**********************************************************************************************************************
 * Mirror_ProcessCanFrame()
 **********************************************************************************************************************/
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
MIRROR_LOCAL_INLINE FUNC(Std_ReturnType, MIRROR_CODE) Mirror_ProcessCanFrame(uint8 Controller, CONST(Can_PduInfoPtrType, AUTOMATIC) DataPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8 destChannelIndex = 0;

#if (MIRROR_DESTINATION_CHANNEL_IP == STD_OFF)
  MIRROR_DUMMY_STATEMENT(Controller); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  /* #10 Enter MIRROR_EXCLUSIVE_AREA_QUEUE */
  SchM_Enter_Mirror_QueueLock();

  destChannelIndex = Mirror_CurrentDestChannelIndex;

/* #20 If MIRROR_DESTINATION_CHANNEL_IP is enabled and the current active destination channel is a IP channel: */
#if (MIRROR_DESTINATION_CHANNEL_IP == STD_ON)
  if(Mirror_GetChannelTypeOfDestChannel(destChannelIndex) == MIRROR_E_MIRROR_CHANNEL_IP_CHANNELTYPEOFDESTCHANNEL)
  {
    /* #30 Write the incoming data to the queue. */
    retVal = Mirror_DestChannelIp_QueueWrite((uint32) DataPtr->id, Controller, (uint8*) DataPtr->sdu, DataPtr->length, MIRROR_CHANNEL_TYPE_CAN);

    /* #40 Exit MIRROR_EXCLUSIVE_AREA_QUEUE */
    SchM_Exit_Mirror_QueueLock();

    /* #50 If a queue element is full and the Tx PDU is ready for a new transmission: */
    if(Mirror_DestChannelIp_IsTransmit == TRUE)
    {
      /* #60 Transmit the queue element. */
      Mirror_DestChannelIp_QueueTransmit(FALSE, destChannelIndex);
    }

# if (MIRROR_DEV_ERROR_REPORT == STD_ON)
    if(retVal != E_OK)
    {
      (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_CAN_FRAME, MIRROR_E_QUEUE_OVERRUN);
    }
# endif
  }
#endif /* (MIRROR_DESTINATION_CHANNEL_IP == STD_ON) */

/* #70 If MIRROR_DESTINATION_CHANNEL_CAN is enabled and the current active destination channel is a CAN channel: */
#if (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON)
  if(Mirror_GetChannelTypeOfDestChannel(destChannelIndex) == MIRROR_E_MIRROR_CHANNEL_CAN_CHANNELTYPEOFDESTCHANNEL)
  {
    /* #80 If the incoming data length is not to big: */
    if(DataPtr->length <= Mirror_GetTxPduLengthOfDestChannel(destChannelIndex))
    {
      /* #90 If the there are no queued data and the Tx PDU is ready for a new transmission: */
      if((!Mirror_DestChannel_TxPduLocked) && (Mirror_DestChannelCan_QueueIsEmpty() == TRUE))
      {
        /* #100 Exit MIRROR_EXCLUSIVE_AREA_QUEUE */
        SchM_Exit_Mirror_QueueLock();

        /* #110 Do a direct transmission. */
        retVal = Mirror_DestChannelCan_DirectTransmit(DataPtr, destChannelIndex);
      }
      /* #120 Otherwise, queue the data and Exit MIRROR_EXCLUSIVE_AREA_QUEUE */
      else
      {
        retVal = Mirror_DestChannelCan_QueueWrite(DataPtr->sdu, DataPtr->length, DataPtr->id);

        SchM_Exit_Mirror_QueueLock();

# if (MIRROR_DEV_ERROR_REPORT == STD_ON)
        if(retVal != E_OK)
        {
          (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_CAN_FRAME, MIRROR_E_QUEUE_OVERRUN);
        }
# endif
      }
    }
    /* #130 Otherwise, Exit MIRROR_EXCLUSIVE_AREA_QUEUE */
    else
    {
      SchM_Exit_Mirror_QueueLock();
    }
  }
#endif /* (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON) */

  return retVal;
} /* Mirror_ProcessCanFrame() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  Mirror_InitMemory()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, MIRROR_CODE)Mirror_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
/* #10 If MIRROR_DEV_ERROR_DETECT is enabled: */
#if (MIRROR_DEV_ERROR_DETECT == STD_ON)
  /* #20 Initialize the variable Mirror_ModuleInitialized to the value MIRROR_UNINIT. */
  Mirror_ModuleInitialized = MIRROR_UNINIT;
#endif
} /* Mirror_InitMemory() */

/**********************************************************************************************************************
 *  Mirror_Init()
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
FUNC(void, MIRROR_CODE) Mirror_Init(P2CONST(Mirror_ConfigType, AUTOMATIC, MIRROR_PBCFG) configPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = MIRROR_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
#if (MIRROR_USE_INIT_POINTER == STD_ON)
  /* #10 Check the validity of the configPtr given as input. */
  if(configPtr == NULL_PTR)
  {
# if (MIRROR_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
    EcuM_BswErrorHook(MIRROR_MODULE_ID, ECUM_BSWERROR_NULLPTR);
# endif
    errorId = MIRROR_E_PARAM_POINTER;
  }
  else
# if (MIRROR_FINALMAGICNUMBER == STD_ON)
  /* #20 Use the magic number to proof the validity of the configuration given as input. */
  if(configPtr->FinalMagicNumberOfPBConfig != MIRROR_FINAL_MAGIC_NUMBER)
  {
#  if (MIRROR_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
    EcuM_BswErrorHook(MIRROR_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
#  endif
    errorId = MIRROR_E_INIT_FAILED;
  }
  else
# endif
#else
  MIRROR_DUMMY_STATEMENT(configPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
  /* #30 If the initial checks are passed or not required: */
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8_least i, m;

    /* #40 Set the global Mirror mode to inactive. */
    Mirror_GlobalMode = MIRROR_INACTIVE;

    Mirror_ConfigDataPtr = configPtr;

#if (MIRROR_DESTINATION_CHANNEL_IP == STD_ON)
    Mirror_DestChannelIp_TxPduTimeout = 0;
#endif /* (MIRROR_DESTINATION_CHANNEL_IP == STD_ON) */

    /* #50 If MIRROR_CANFILTERCFG is enabled and a filter is configured,
     *     initialize the filter with the values defined in the configuration tool.
     *     Otherwise initialize the filter with 0. */
    for(i = 0; i < Mirror_GetSizeOfCanController(); i++)
    {
#if (MIRROR_INVALIDHNDOFCANCONTROLLER == STD_ON)
      if(!Mirror_IsInvalidHndOfCanController(i))
#endif
      {
#if (MIRROR_CANFILTERCFG == STD_ON)
        uint8_least k = Mirror_GetCanFilterCfgIndStartIdxOfCanController(i);

        for(m = 0; m < MIRROR_SOURCECHANNELCAN_NUMBER_OF_FILTERS; m++)
        {
          if(k < Mirror_GetCanFilterCfgIndEndIdxOfCanController(i))
          {
            Mirror_GetCurrentCanFilterState(Mirror_GetCurrentCanFilterStateIdxOfCanController(i)).isFilterActive[m] = TRUE;
            Mirror_GetCurrentCanFilterState(Mirror_GetCurrentCanFilterStateIdxOfCanController(i)).filter[m].filterValueLowOrId.id32 = Mirror_GetFilterValueLowOrIdOfCanFilterCfg(k);
            Mirror_GetCurrentCanFilterState(Mirror_GetCurrentCanFilterStateIdxOfCanController(i)).filter[m].filterValueHighOrMask.id32 = Mirror_GetFilterValueHighOrMaskOfCanFilterCfg(k);
          }
          else
          {
            Mirror_GetCurrentCanFilterState(Mirror_GetCurrentCanFilterStateIdxOfCanController(i)).isFilterActive[m] = FALSE;
            Mirror_GetCurrentCanFilterState(Mirror_GetCurrentCanFilterStateIdxOfCanController(i)).filter[m].filterValueLowOrId.id32 = MIRROR_SOURCE_CHANNEL_FILTER_VALUE_LOWORID_INIT;
            Mirror_GetCurrentCanFilterState(Mirror_GetCurrentCanFilterStateIdxOfCanController(i)).filter[m].filterValueHighOrMask.id32 = MIRROR_SOURCE_CHANNEL_FILTER_VALUE_HIGHORMASK_INIT;
          }
          k++;
        }
#else
        for(m = 0; m < MIRROR_SOURCECHANNELCAN_NUMBER_OF_FILTERS; m++)
        {
          Mirror_GetCurrentCanFilterState(Mirror_GetCurrentCanFilterStateIdxOfCanController(i)).isFilterActive[m] = FALSE;
          Mirror_GetCurrentCanFilterState(Mirror_GetCurrentCanFilterStateIdxOfCanController(i)).filter[m].filterValueLowOrId.id32 = MIRROR_SOURCE_CHANNEL_FILTER_VALUE_LOWORID_INIT;
          Mirror_GetCurrentCanFilterState(Mirror_GetCurrentCanFilterStateIdxOfCanController(i)).filter[m].filterValueHighOrMask.id32 = MIRROR_SOURCE_CHANNEL_FILTER_VALUE_HIGHORMASK_INIT;
        }
#endif
      }
    }

    /* #60 Set all source channels initially to inactive. */
    for(i = 0; i < Mirror_GetSizeOfSourceChannelState(); i++)
    {
      Mirror_SetSourceChannelState(i, MIRROR_INACTIVE);
    }

    /* #70 Set the default destination channel. */
    for(i = 0; i < Mirror_GetSizeOfDestChannel(); i++)
    {
#if MIRROR_DEFAULTDESTCHANNELOFDESTCHANNEL == STD_ON
      if(Mirror_IsDefaultDestChannelOfDestChannel(i))
#endif
      {
        Mirror_CurrentDestChannelIndex = (uint8) i;

        break;
      }
    }

/* #80 If MIRROR_DESTINATION_CHANNEL_IP is enabled and the current destination channel is of the type MIRROR_E_MIRROR_CHANNEL_IP_CHANNELTYPEOFDESTCHANNEL: */
#if (MIRROR_DESTINATION_CHANNEL_IP == STD_ON)
    if(Mirror_GetChannelTypeOfDestChannel(Mirror_CurrentDestChannelIndex) == MIRROR_E_MIRROR_CHANNEL_IP_CHANNELTYPEOFDESTCHANNEL)
    {
      /* #90 Initialize the destination channel IP queue. */
      Mirror_DestChannelIp_QueueInit();

      Mirror_DestChannelIp_SeqNumber = 0;
    }
#endif /* (MIRROR_DESTINATION_CHANNEL_IP == STD_ON) */

/* #100 If MIRROR_DESTINATION_CHANNEL_CAN is enabled and the current destination channel is of the type MIRROR_E_MIRROR_CHANNEL_CAN_CHANNELTYPEOFDESTCHANNEL: */
#if (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON)
    if(Mirror_GetChannelTypeOfDestChannel(Mirror_CurrentDestChannelIndex) == MIRROR_E_MIRROR_CHANNEL_CAN_CHANNELTYPEOFDESTCHANNEL)
    {
      /* #110 Initialize the destination channel CAN queue. */
      Mirror_DestChannelCan_QueueInit();
    }
#endif /* (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON) */

#if (MIRROR_DEV_ERROR_DETECT == STD_ON)
    Mirror_ModuleInitialized = (uint8) MIRROR_INIT;
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (MIRROR_DEV_ERROR_REPORT == STD_ON) && (MIRROR_USE_INIT_POINTER == STD_ON)
  if(errorId != MIRROR_E_NO_ERROR)
  {
    (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_INIT, errorId);
  }
#else
  MIRROR_DUMMY_STATEMENT(errorId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
  MIRROR_DUMMY_STATEMENT(Mirror_ConfigDataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif
} /* Mirror_Init() */ /* PRQA S 6030, 6080, 6010  */ /* MD_MSR_STCYC, MD_MSR_STMIF, MD_MSR_STPTH */

/**********************************************************************************************************************
 * Mirror_DeInit()
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
FUNC(void, MIRROR_CODE) Mirror_DeInit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = MIRROR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
/* #10 If MIRROR_DEV_ERROR_DETECT is enabled: */
#if (MIRROR_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check that the Mirror is initialized. */
  if(Mirror_ModuleInitialized != MIRROR_INIT)
  {
    errorId = MIRROR_E_UNINIT;
  }
  /* #30 If the initial checks are passed: */
  else
  {
    /* #40 Set state of Mirror to not initialized. */
    Mirror_ModuleInitialized = MIRROR_UNINIT;
  }
#endif

  /* ----- Development Error Report ------------------------------------- */
#if (MIRROR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != MIRROR_E_NO_ERROR) /* PRQA S 3356, 3359 */ /* MD_MSR_14.1 */
  { /* PRQA S 3201 2 */ /* MD_MSR_14.1 */
    (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_DEINIT, errorId); 
  }
#else
  MIRROR_DUMMY_STATEMENT(errorId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif 
} /* Mirror_DeInit() */

#if (MIRROR_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Mirror_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, MIRROR_CODE)Mirror_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, MIRROR_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = MIRROR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (MIRROR_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check the validity of the versioninfo given as input. */
  if(versioninfo == NULL_PTR)
  {
    errorId = MIRROR_E_PARAM_POINTER;
  }
  else
# endif
  /* #20 If the initial checks are passed or not required: */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Set the version info to given versioninfo pointer. */
    versioninfo->vendorID = (MIRROR_VENDOR_ID);
    versioninfo->moduleID = (MIRROR_MODULE_ID);
    versioninfo->sw_major_version = (MIRROR_SW_MAJOR_VERSION);
    versioninfo->sw_minor_version = (MIRROR_SW_MINOR_VERSION);
    versioninfo->sw_patch_version = (MIRROR_SW_PATCH_VERSION);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (MIRROR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != MIRROR_E_NO_ERROR)
  {
    (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_GET_VERSION_INFO, errorId);
  }
# else
  MIRROR_DUMMY_STATEMENT(errorId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
# endif
} /* Mirror_GetVersionInfo() */
#endif /* (MIRROR_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 * Mirror_SetCanFilter()
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
FUNC(Std_ReturnType, MIRROR_CODE) Mirror_SetCanFilter(CONST(NetworkHandleType, AUTOMATIC) channel, uint8 filterId, P2CONST(Mirror_CanFilterType, AUTOMATIC, MIRROR_APPL_DATA) filter)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = MIRROR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (MIRROR_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the Mirror module is in the state "Initialized". */
  if(Mirror_ModuleInitialized == (uint8) MIRROR_UNINIT)
  {
    errorId = MIRROR_E_UNINIT;
  }
  /* #20 Check the validity of the given channel. */
  else if(channel >= Mirror_GetSizeOfSourceComMChannel())
  {
    errorId = MIRROR_E_PARAM_CHANNEL;
  }
# if (MIRROR_INVALIDHNDOFSOURCECOMMCHANNEL == STD_ON)
  else if(Mirror_IsInvalidHndOfSourceComMChannel(channel))
  {
    errorId = MIRROR_E_PARAM_CHANNEL;
  }
# endif
  /* #30 Check the validity of the given filterId. */
  else if(filterId >= MIRROR_SOURCECHANNELCAN_NUMBER_OF_FILTERS)
  {
    errorId = MIRROR_E_CANFILTER_ID;
  }
  /* #40 Check the validity of the given filter. */
  else if(filter == NULL_PTR)
  {
    errorId = MIRROR_E_PARAM_POINTER;
  }
  else
#endif
  /* #50 If the initial checks are passed or not required: */
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 srcBus = (uint8) Mirror_GetSourceChannelIdxOfSourceComMChannel(channel);
    Mirror_FilterStateType *canFilters;

    /* #60 Enter MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
    SchM_Enter_Mirror_SourceChannelLock();

    /* #70 Set the filter. */
    canFilters = &Mirror_GetCurrentCanFilterState(Mirror_GetCurrentCanFilterStateIdxOfCanController(srcBus));

    canFilters->filter[filterId].filterValueLowOrId.id32 = filter->filterValueLowOrId;
    canFilters->filter[filterId].filterValueHighOrMask.id32 = filter->filterValueHighOrMask;
    canFilters->isFilterActive[filterId] = TRUE;

    /* #80 Exit MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
    SchM_Exit_Mirror_SourceChannelLock();

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (MIRROR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != MIRROR_E_NO_ERROR)
  {
    (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_SETFILTER, errorId);
  }
#else
  MIRROR_DUMMY_STATEMENT(errorId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* Mirror_SetCanFilter() */ /* PRQA S 6080 1 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * Mirror_SetCanFilterState()
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
 */
FUNC(Std_ReturnType, MIRROR_CODE) Mirror_SetCanFilterState(NetworkHandleType channel, uint8 filterId, boolean isActive)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = MIRROR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (MIRROR_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the Mirror module is in the state "Initialized". */
  if(Mirror_ModuleInitialized == (uint8) MIRROR_UNINIT)
  {
    errorId = MIRROR_E_UNINIT;
  }
  /* #20 Check the validity of the given channel. */
  else if(channel >= Mirror_GetSizeOfSourceComMChannel())
  {
    errorId = MIRROR_E_PARAM_CHANNEL;
  }
# if (MIRROR_INVALIDHNDOFSOURCECOMMCHANNEL == STD_ON)
  else if(Mirror_IsInvalidHndOfSourceComMChannel(channel))
  {
    errorId = MIRROR_E_PARAM_CHANNEL;
  }
# endif
  /* #30 Check the validity of the given filterId. */
  else if(filterId >= MIRROR_SOURCECHANNELCAN_NUMBER_OF_FILTERS)
  {
    errorId = MIRROR_E_CANFILTER_ID;
  }
  else
#endif
  /* #40 If the initial checks are passed or not required: */
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 srcBus = (uint8) Mirror_GetSourceChannelIdxOfSourceComMChannel(channel);
    Mirror_FilterStateType *canFilters;

    /* #50 Enter MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
    SchM_Enter_Mirror_SourceChannelLock();

    /* #60 Set the filter state for the given filter ID. */
    canFilters = &Mirror_GetCurrentCanFilterState(Mirror_GetCurrentCanFilterStateIdxOfCanController(srcBus));
    canFilters->isFilterActive[filterId] = isActive;

    /* #70 Exit MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
    SchM_Exit_Mirror_SourceChannelLock();

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (MIRROR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != MIRROR_E_NO_ERROR)
  {
    (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_SET_CAN_FILTER_STATE, errorId);
  }
#else
  MIRROR_DUMMY_STATEMENT(errorId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif 

  return retVal;
} /* Mirror_SetCanFilterState() */

/**********************************************************************************************************************
 * Mirror_GetCanFilter()
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
FUNC(Std_ReturnType, MIRROR_CODE) Mirror_GetCanFilter(NetworkHandleType channel, uint8 filterId, P2VAR(Mirror_CanFilterType, AUTOMATIC, MIRROR_APPL_DATA) filter)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = MIRROR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (MIRROR_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the Mirror module is in the state "Initialized". */
  if(Mirror_ModuleInitialized == (uint8) MIRROR_UNINIT)
  {
    errorId = MIRROR_E_UNINIT;
  }
  /* #20 Check the validity of the given channel. */
  else if(channel >= Mirror_GetSizeOfSourceComMChannel())
  {
    errorId = MIRROR_E_PARAM_CHANNEL;
  }
# if (MIRROR_INVALIDHNDOFSOURCECOMMCHANNEL == STD_ON)
  else if(Mirror_IsInvalidHndOfSourceComMChannel(channel))
  {
    errorId = MIRROR_E_PARAM_CHANNEL;
  }
# endif
  /* #30 Check the validity of the given filterId. */
  else if(filterId >= MIRROR_SOURCECHANNELCAN_NUMBER_OF_FILTERS)
  {
    errorId = MIRROR_E_CANFILTER_ID;
  }
  /* #40 Check the validity of the given filter. */
  else if(filter == NULL_PTR)
  {
    errorId = MIRROR_E_PARAM_POINTER;
  }
  else
#endif
  /* #50 If the initial checks are passed or not required: */
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 srcBus = (uint8) Mirror_GetSourceChannelIdxOfSourceComMChannel(channel);
    Mirror_FilterStateType *canFilters;

    /* #60 Enter MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
    SchM_Enter_Mirror_SourceChannelLock();

    /* #70 Copy the filter values to the given filter buffer. */
    canFilters = &Mirror_GetCurrentCanFilterState(Mirror_GetCurrentCanFilterStateIdxOfCanController(srcBus));
    filter->filterValueLowOrId = canFilters->filter[filterId].filterValueLowOrId.id32;
    filter->filterValueHighOrMask = canFilters->filter[filterId].filterValueHighOrMask.id32;

    /* #80 Exit MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
    SchM_Exit_Mirror_SourceChannelLock();

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (MIRROR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != MIRROR_E_NO_ERROR)
  {
    (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_GET_CAN_FILTER, errorId);
  }
#else
  MIRROR_DUMMY_STATEMENT(errorId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif 

  return retVal;
} /* Mirror_GetCanFilter() */ /* PRQA S 6080 1 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * Mirror_GetCanFilterState()
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
FUNC(Std_ReturnType, MIRROR_CODE) Mirror_GetCanFilterState(NetworkHandleType channel, uint8 filterId, P2VAR(boolean, AUTOMATIC, MIRROR_APPL_DATA) isActive)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = MIRROR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (MIRROR_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the Mirror module is in the state "Initialized". */
  if(Mirror_ModuleInitialized == (uint8) MIRROR_UNINIT)
  {
    errorId = MIRROR_E_UNINIT;
  }
  /* #20 Check the validity of the given channel. */
  else if(channel >= Mirror_GetSizeOfSourceComMChannel())
  {
    errorId = MIRROR_E_PARAM_CHANNEL;
  }
# if (MIRROR_INVALIDHNDOFSOURCECOMMCHANNEL == STD_ON)
  else if(Mirror_IsInvalidHndOfSourceComMChannel(channel))
  {
    errorId = MIRROR_E_PARAM_CHANNEL;
  }
# endif
  /* #30 Check the validity of the given filterId. */
  else if(filterId >= MIRROR_SOURCECHANNELCAN_NUMBER_OF_FILTERS)
  {
    errorId = MIRROR_E_CANFILTER_ID;
  }
  /* #40 Check the validity of the given isActive pointer. */
  else if(isActive == NULL_PTR)
  {
    errorId = MIRROR_E_PARAM_POINTER;
  }
  else
#endif
  /* #50 If the initial checks are passed or not required: */
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 srcBus = (uint8) Mirror_GetSourceChannelIdxOfSourceComMChannel(channel);
    Mirror_FilterStateType *canFilters;

    /* #60 Enter MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
    SchM_Enter_Mirror_SourceChannelLock();

    /* #70 Copy the filter state of the given filterId to the given buffer (isActive). */
    canFilters = &Mirror_GetCurrentCanFilterState(Mirror_GetCurrentCanFilterStateIdxOfCanController(srcBus));
    *isActive = canFilters->isFilterActive[filterId];

    /* #80 Exit MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
    SchM_Exit_Mirror_SourceChannelLock();

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (MIRROR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != MIRROR_E_NO_ERROR)
  {
    (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_GET_CAN_FILTER_STATE, errorId);
  }
#else
  MIRROR_DUMMY_STATEMENT(errorId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif 

  return retVal;
} /* Mirror_GetCanFilterState() */ /* PRQA S 6080 1 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 * Mirror_GetDestChannel()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(NetworkHandleType, MIRROR_CODE) Mirror_GetDestChannel(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  NetworkHandleType retVal = 0;
  uint8 errorId = MIRROR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (MIRROR_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the Mirror module is in the state "Initialized". */
  if(Mirror_ModuleInitialized == (uint8) MIRROR_UNINIT)
  {
    errorId = MIRROR_E_UNINIT;
  }
  else
#endif
  /* #20 If the initial checks are passed or not required: */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Set the return value to the current active destination channel. */
    retVal = (NetworkHandleType) Mirror_GetComMChannelIdOfDestChannel(Mirror_CurrentDestChannelIndex);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (MIRROR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != MIRROR_E_NO_ERROR)
  {
    (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_GET_DEST_CHANNEL, errorId);
  }
#else
  MIRROR_DUMMY_STATEMENT(errorId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif 

  return retVal;
} /* Mirror_GetDestChannel() */

/**********************************************************************************************************************
 * Mirror_GetChannelType()
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
FUNC(Mirror_ChannelType, MIRROR_CODE) Mirror_GetChannelType(NetworkHandleType channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  Mirror_ChannelType retVal = MIRROR_CHANNEL_TYPE_ERROR;
  uint8 channelType;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If the given channel is a valid source channel: */
  if(channel < Mirror_GetSizeOfSourceComMChannel())
  {
#if (MIRROR_INVALIDHNDOFSOURCECOMMCHANNEL == STD_ON)
    if(!Mirror_IsInvalidHndOfSourceComMChannel(channel))
#endif
    {
      /* #20 Set the return value to channel type for the given channel. */
      channelType = (uint8) Mirror_GetChannelTypeOfSourceChannel(Mirror_GetSourceChannelIdxOfSourceComMChannel(channel));

      if(channelType == MIRROR_E_MIRROR_CHANNEL_CAN_CHANNELTYPEOFSOURCECHANNEL)
      {
        retVal = MIRROR_CHANNEL_TYPE_CAN;
      }
    }
  }

  /* #30 If the given channel was not a valid source channel: */
  if(retVal == MIRROR_CHANNEL_TYPE_ERROR)
  {
    /* #40 If the given channel is a valid destination channel: */
    if(channel < Mirror_GetSizeOfDestComMChannel())
    {
#if (MIRROR_INVALIDHNDOFDESTCOMMCHANNEL == STD_ON)
      if(!Mirror_IsInvalidHndOfDestComMChannel(channel))
#endif
      {
        /* #50 Set the return value to channel type for the given channel. */
        channelType = (uint8) Mirror_GetChannelTypeOfDestChannel(Mirror_GetDestChannelIdxOfDestComMChannel(channel));

#if(MIRROR_DESTINATION_CHANNEL_CAN == STD_ON)
        if(channelType == MIRROR_E_MIRROR_CHANNEL_CAN_CHANNELTYPEOFDESTCHANNEL)
        {
          retVal = MIRROR_CHANNEL_TYPE_CAN;
        }
#endif

#if(MIRROR_DESTINATION_CHANNEL_IP == STD_ON)
        if(channelType == MIRROR_E_MIRROR_CHANNEL_IP_CHANNELTYPEOFDESTCHANNEL)
        {
          retVal = MIRROR_CHANNEL_TYPE_IP;
        }
#endif
      }
    }
  }

  return retVal;
} /* Mirror_GetChannelType() */

/**********************************************************************************************************************
 * Mirror_SwitchDestChannel()
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
 *
 */
FUNC(Std_ReturnType, MIRROR_CODE) Mirror_SwitchDestChannel(NetworkHandleType channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = MIRROR_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;
  uint8_least i;

  /* ----- Development Error Checks ------------------------------------- */
#if (MIRROR_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the Mirror module is in the state "Initialized". */
  if(Mirror_ModuleInitialized == (uint8) MIRROR_UNINIT)
  {
    errorId = MIRROR_E_UNINIT;
  }
  /* #20 Check the validity of the given channel. */
  else if(channel >= Mirror_GetSizeOfDestComMChannel())
  {
    errorId = MIRROR_E_PARAM_CHANNEL;
  }
# if (MIRROR_INVALIDHNDOFDESTCOMMCHANNEL == STD_ON)
  else if(Mirror_IsInvalidHndOfDestComMChannel(channel))
  {
    errorId = MIRROR_E_PARAM_CHANNEL;
  }
# endif
  else
#endif
  /* #30 If the initial checks are passed or not required: */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If the given channel is not already the current active destination channel: */
    if(Mirror_CurrentDestChannelIndex != Mirror_GetDestChannelIdxOfDestComMChannel(channel))
    {
      /* #50 If the Mirror module is active, deactivate all source channels. */
      if(Mirror_GlobalMode == MIRROR_ACTIVE)
      {
        for(i = 0; i < Mirror_GetSizeOfSourceChannel(); i++)
        {
          (void)Mirror_StopSourceChannel(Mirror_GetComMChannelIdOfSourceChannel(i));
        }
      }

      /* #60 If the Mirror module is inactive: */
      if(Mirror_GlobalMode == MIRROR_INACTIVE)
      {
        /* #70 Set the current active destination channel to the given channel. */
        Mirror_CurrentDestChannelIndex = (uint8) Mirror_GetDestChannelIdxOfDestComMChannel(channel);

/* #80 if MIRROR_DESTINATION_CHANNEL_IP is enabled and the current active destination channel is of the type MIRROR_E_MIRROR_CHANNEL_IP_CHANNELTYPEOFDESTCHANNEL: */
#if (MIRROR_DESTINATION_CHANNEL_IP == STD_ON)
        if(Mirror_GetChannelTypeOfDestChannel(Mirror_CurrentDestChannelIndex) == MIRROR_E_MIRROR_CHANNEL_IP_CHANNELTYPEOFDESTCHANNEL)
        {
          /* #90 Enter MIRROR_EXCLUSIVE_AREA_QUEUE */
          SchM_Enter_Mirror_QueueLock();

          /* #100 Initialize the IP destination channel. */
          Mirror_DestChannelIp_QueueInit();

          /* #110 Exit MIRROR_EXCLUSIVE_AREA_QUEUE */
          SchM_Exit_Mirror_QueueLock();

          Mirror_DestChannelIp_TxPduTimeout = 0;
          Mirror_DestChannelIp_SeqNumber = 0;
          Mirror_TxConfTimeout = 0;
        }
#endif /* (MIRROR_DESTINATION_CHANNEL_IP == STD_ON) */

/* #120 if MIRROR_DESTINATION_CHANNEL_CAN is enabled and the current active destination channel is of the type MIRROR_E_MIRROR_CHANNEL_CAN_CHANNELTYPEOFDESTCHANNEL: */
#if (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON)
        if(Mirror_GetChannelTypeOfDestChannel(Mirror_CurrentDestChannelIndex) == MIRROR_E_MIRROR_CHANNEL_CAN_CHANNELTYPEOFDESTCHANNEL)
        {
          /* #130 Enter MIRROR_EXCLUSIVE_AREA_QUEUE */
          SchM_Enter_Mirror_QueueLock();

          /* #140 Initialize the CAN destination channel. */
          Mirror_DestChannelCan_QueueInit();

          /* #150 Exit MIRROR_EXCLUSIVE_AREA_QUEUE */
          SchM_Exit_Mirror_QueueLock();

          Mirror_TxConfTimeout = 0;
        }
#endif /* (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON) */

        retVal = E_OK;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (MIRROR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != MIRROR_E_NO_ERROR)
  {
    (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_SWITCH_DEST_CHANNEL, errorId);
  }
#else
  MIRROR_DUMMY_STATEMENT(errorId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* Mirror_SwitchDestChannel() */ /* PRQA S 6080, 6030 */ /* MD_MSR_STMIF, MD_MSR_STCYC */

/**********************************************************************************************************************
 * Mirror_IsMirrorActive()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, MIRROR_CODE) Mirror_IsMirrorActive(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = FALSE;
  uint8 errorId = MIRROR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (MIRROR_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the Mirror module is in the state "Initialized". */
  if(Mirror_ModuleInitialized == (uint8) MIRROR_UNINIT)
  {
    errorId = MIRROR_E_UNINIT;
  }
  else
#endif
  /* #20 If the initial checks are passed or not required: */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Set the return value to the global Mirror state. */
    retVal = Mirror_GlobalMode;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (MIRROR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != MIRROR_E_NO_ERROR)
  {
    (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_IS_MIRROR_ACTIVE, errorId);
  }
#else
  MIRROR_DUMMY_STATEMENT(errorId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif 

  return retVal;
} /* Mirror_IsMirrorActive() */

/**********************************************************************************************************************
 * Mirror_IsSourceChannelStarted()
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
FUNC(boolean, MIRROR_CODE) Mirror_IsSourceChannelStarted(NetworkHandleType channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = FALSE;
  uint8 errorId = MIRROR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (MIRROR_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the Mirror module is in the state "Initialized". */
  if(Mirror_ModuleInitialized == (uint8) MIRROR_UNINIT)
  {
    errorId = MIRROR_E_UNINIT;
  }
  /* #20 Check the validity of the given channel. */
  else if(channel >= Mirror_GetSizeOfSourceComMChannel())
  {
    errorId = MIRROR_E_PARAM_CHANNEL;
  }
# if (MIRROR_INVALIDHNDOFSOURCECOMMCHANNEL == STD_ON)
  else if(Mirror_IsInvalidHndOfSourceComMChannel(channel))
  {
    errorId = MIRROR_E_PARAM_CHANNEL;
  }
# endif
  else
#endif
  /* #30 If the initial checks are passed or not required: */
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 srcBus = (uint8) Mirror_GetSourceChannelIdxOfSourceComMChannel(channel);

    /* #40 Enter MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
    SchM_Enter_Mirror_SourceChannelLock();

    /* #50 If the given source channel is active: */
    if(Mirror_IsSourceChannelState(Mirror_GetSourceChannelStateIdxOfSourceChannel(srcBus)))
    {
      /* #60 Exit MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
      SchM_Exit_Mirror_SourceChannelLock();

      /* #70 Set the return value to TRUE. */
      retVal = TRUE;
    }
    /* #80 Otherwise, Exit MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
    else
    {
      SchM_Exit_Mirror_SourceChannelLock();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (MIRROR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != MIRROR_E_NO_ERROR)
  {
    (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_IS_SOURCE_CHANNEL_STARTED, errorId);
  }
#else
  MIRROR_DUMMY_STATEMENT(errorId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif 

  return retVal;
} /* Mirror_IsSourceChannelStarted() */

/**********************************************************************************************************************
 * Mirror_StartSourceChannel()
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
FUNC(Std_ReturnType, MIRROR_CODE) Mirror_StartSourceChannel(CONST(NetworkHandleType, AUTOMATIC) channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = MIRROR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (MIRROR_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the Mirror module is in the state "Initialized". */
  if(Mirror_ModuleInitialized == (uint8) MIRROR_UNINIT)
  {
    errorId = MIRROR_E_UNINIT;
  }
  /* #20 Check the validity of the given channel. */
  else if(channel >= Mirror_GetSizeOfSourceComMChannel())
  {
    errorId = MIRROR_E_PARAM_CHANNEL;
  }
# if (MIRROR_INVALIDHNDOFSOURCECOMMCHANNEL == STD_ON)
  else if(Mirror_IsInvalidHndOfSourceComMChannel(channel))
  {
    errorId = MIRROR_E_PARAM_CHANNEL;
  }
# endif
  else
#endif
  /* #30 If the initial checks are passed or not required: */
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 srcBus = (uint8) Mirror_GetSourceChannelIdxOfSourceComMChannel(channel);

    /* #40 Enter MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
    SchM_Enter_Mirror_SourceChannelLock();

    /* #50 If the mirroring is already active for the given channel: */
    if(Mirror_IsSourceChannelState(Mirror_GetSourceChannelStateIdxOfSourceChannel(srcBus)))
    {
      /* #60 Exit MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
      SchM_Exit_Mirror_SourceChannelLock();
    }
    /* #70 Otherwise activate mirroring for this channel: */
    else
    {
      Mirror_SetSourceChannelState(Mirror_GetSourceChannelStateIdxOfSourceChannel(srcBus), MIRROR_ACTIVE);

      /* #80 If the global mirror state is inactive, set the state to active: */
      if(Mirror_GlobalMode == MIRROR_INACTIVE)
      {
        Mirror_GlobalMode = MIRROR_ACTIVE;

#if (MIRROR_DESTINATION_CHANNEL_IP == STD_ON)
        if(Mirror_GetChannelTypeOfDestChannel(Mirror_CurrentDestChannelIndex) == MIRROR_E_MIRROR_CHANNEL_IP_CHANNELTYPEOFDESTCHANNEL)
        {
          Mirror_DestChannelState.startTimeStamp = Mirror_GetTime();
        }
#endif /* (MIRROR_DESTINATION_CHANNEL_IP == STD_ON) */

        /* #90 Exit MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
        SchM_Exit_Mirror_SourceChannelLock();
      }
      /* #100 Otherwise, Exit MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
      else
      {
        SchM_Exit_Mirror_SourceChannelLock();
      }

      /* #110 Set the mirror mode for the given channel in the CAN module. */
      Can_SetMirrorMode((uint8) Mirror_GetCanControllerIdOfSourceChannel(srcBus), MIRROR_ACTIVE);

      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (MIRROR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != MIRROR_E_NO_ERROR)
  {
    (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_START, errorId);
  }
#else
  MIRROR_DUMMY_STATEMENT(errorId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* Mirror_StartSourceChannel() */ /* PRQA S 6080 1 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Mirror_StopSourceChannel()
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
FUNC(Std_ReturnType, MIRROR_CODE) Mirror_StopSourceChannel(CONST(NetworkHandleType, AUTOMATIC) channel)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = MIRROR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (MIRROR_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the Mirror module is in the state "Initialized". */
  if(Mirror_ModuleInitialized == (uint8) MIRROR_UNINIT)
  {
    errorId = MIRROR_E_UNINIT;
  }
  /* #20 Check the validity of the given channel. */
  else if(channel >= Mirror_GetSizeOfSourceComMChannel())
  {
    errorId = MIRROR_E_PARAM_CHANNEL;
  }
# if (MIRROR_INVALIDHNDOFSOURCECOMMCHANNEL == STD_ON)
  else if(Mirror_IsInvalidHndOfSourceComMChannel(channel))
  {
    errorId = MIRROR_E_PARAM_CHANNEL;
  }
# endif
  else
#endif
  /* #30 If the initial checks are passed or not required: */
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8_least i;
    uint8 srcBus = (uint8) Mirror_GetSourceChannelIdxOfSourceComMChannel(channel);
    uint8 tmpGlobalState = MIRROR_INACTIVE;

    /* #40 Enter MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
    SchM_Enter_Mirror_SourceChannelLock();

    /* #50 If the source channel is already inactive: */
    if(!Mirror_IsSourceChannelState(Mirror_GetSourceChannelStateIdxOfSourceChannel(srcBus)))
    {
      /* #60 Exit MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
      SchM_Exit_Mirror_SourceChannelLock();
    }
    /* #70 Otherwise deactivate mirroring for this source channel and update the global mirror state. */
    else
    {
      Mirror_SetSourceChannelState(Mirror_GetSourceChannelStateIdxOfSourceChannel(srcBus), MIRROR_INACTIVE);

      /* #80 Exit MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
      SchM_Exit_Mirror_SourceChannelLock();

      Can_SetMirrorMode(Mirror_GetCanControllerIdOfSourceChannel(srcBus), MIRROR_INACTIVE);

      for(i = 0; i < Mirror_GetSizeOfSourceChannel(); i++)
      {
        tmpGlobalState |= (uint8) Mirror_IsSourceChannelState(Mirror_GetSourceChannelStateIdxOfSourceChannel(i));
      }

      /* #90 If the global mirror state is inactive: */
      if(tmpGlobalState == MIRROR_INACTIVE)
      {
        /* #100 Reset the destination queue, the global Mirror mode and the timeout counter to its default values. */
#if (MIRROR_DESTINATION_CHANNEL_IP == STD_ON)
        if(Mirror_GetChannelTypeOfDestChannel(Mirror_CurrentDestChannelIndex) == MIRROR_E_MIRROR_CHANNEL_IP_CHANNELTYPEOFDESTCHANNEL)
        {
          /* #110 Enter MIRROR_EXCLUSIVE_AREA_QUEUE */
          SchM_Enter_Mirror_QueueLock();

          Mirror_DestChannelIp_QueueInit();

          /* #120 Exit MIRROR_EXCLUSIVE_AREA_QUEUE */
          SchM_Exit_Mirror_QueueLock();

          Mirror_DestChannelIp_TxPduTimeout = 0;
          Mirror_TxConfTimeout = 0;
        }
#endif /* (MIRROR_DESTINATION_CHANNEL_IP == STD_ON) */

#if (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON)
        if(Mirror_GetChannelTypeOfDestChannel(Mirror_CurrentDestChannelIndex) == MIRROR_E_MIRROR_CHANNEL_CAN_CHANNELTYPEOFDESTCHANNEL)
        {
          /* #130 Enter MIRROR_EXCLUSIVE_AREA_QUEUE */
          SchM_Enter_Mirror_QueueLock();

          Mirror_DestChannelCan_QueueInit();

          /* #140 Exit MIRROR_EXCLUSIVE_AREA_QUEUE */
          SchM_Exit_Mirror_QueueLock();

          Mirror_TxConfTimeout = 0;
        }
#endif /* (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON) */

        Mirror_GlobalMode = MIRROR_INACTIVE;
      }

      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (MIRROR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != MIRROR_E_NO_ERROR)
  {
    (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_STOP, errorId);
  }
#else
  MIRROR_DUMMY_STATEMENT(errorId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif 

  return retVal;
} /* Mirror_StopSourceChannel() */ /* PRQA S 6080 1 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
* Mirror_MainFunction()
**********************************************************************************************************************/
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
FUNC(void, MIRROR_APPL_CODE)Mirror_MainFunction(void)
{
  /* #10 If the mirror module is active: */
  if(Mirror_GlobalMode == MIRROR_ACTIVE)
  {
    /* #20 If MIRROR_DESTINATION_CHANNEL_IP is enabled and the current destination channel type is MIRROR_E_MIRROR_CHANNEL_IP_CHANNELTYPEOFDESTCHANNEL: */
#if (MIRROR_DESTINATION_CHANNEL_IP == STD_ON)
    if(Mirror_GetChannelTypeOfDestChannel(Mirror_CurrentDestChannelIndex) == MIRROR_E_MIRROR_CHANNEL_IP_CHANNELTYPEOFDESTCHANNEL)
    {
      /* #30 Call the destination channel IP main function. */
      Mirror_DestChannelIp_MainFunction();
    }
#endif /* (MIRROR_DESTINATION_CHANNEL_IP == STD_ON) */

    /* #40 If MIRROR_DESTINATION_CHANNEL_CAN is enabled and the current destination channel type is MIRROR_E_MIRROR_CHANNEL_CAN_CHANNELTYPEOFDESTCHANNEL: */
#if (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON)
    if(Mirror_GetChannelTypeOfDestChannel(Mirror_CurrentDestChannelIndex) == MIRROR_E_MIRROR_CHANNEL_CAN_CHANNELTYPEOFDESTCHANNEL)
    {
      /* #50 Call the destination channel CAN main function. */
      Mirror_DestChannelCan_MainFunction();
    }
#endif /* (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON) */
  }
} /* Mirror_MainFunction() */

/**********************************************************************************************************************
 * Mirror_ReportCanFrame()
 **********************************************************************************************************************/
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
FUNC(Can_ReturnType, MIRROR_CODE) Mirror_ReportCanFrame(uint8 Controller, P2CONST(Can_PduType, AUTOMATIC, MIRROR_APPL_DATA) DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = MIRROR_E_NO_ERROR;
  Can_ReturnType retVal = CAN_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if (MIRROR_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the Mirror module is in the state "Initialized". */
  if(Mirror_ModuleInitialized == MIRROR_UNINIT)
  {
    errorId = MIRROR_E_UNINIT;
  }
  /* #20 Check the validity of the given Controller. */
  else if(Controller >= Mirror_GetSizeOfCanController())
  {
    errorId = MIRROR_E_PARAM_CTRL;
  }
  /* #30 Check the validity of the given DataPtr. */
  else if((DataPtr == NULL_PTR) || (DataPtr->sdu == NULL_PTR))
  {
    errorId = MIRROR_E_PARAM_POINTER;
  }
  else
#endif
  /* #40 If the initial checks are passed or not required: */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Enter MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
    SchM_Enter_Mirror_SourceChannelLock();

    /* #60 If the source channel for the given Controller is active: */
    if((Mirror_GlobalMode == MIRROR_ACTIVE) && (Mirror_IsSourceChannelState(Mirror_GetSourceChannelIdxOfCanController(Controller))))
    {
      boolean filterMatched = Mirror_SourceChannel_ProcessFilter(Controller, DataPtr);

      /* #70 Exit MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
      SchM_Exit_Mirror_SourceChannelLock();

      /* #80 If no filter is configured or the source bus has an active matching filter: */
      if(filterMatched == TRUE)
      {
        /* #90 Call the internal function to process the incoming CAN frame. */
        if(Mirror_ProcessCanFrame(Controller, DataPtr) == E_OK)
        {
          retVal = CAN_OK;
        }
      }
      /* #100 Otherwise, CAN frame is not mirrored (return with CAN_OK). */
      else
      {
        retVal = CAN_OK;
      }
    }
    /* #110 Otherwise, Exit MIRROR_EXCLUSIVE_AREA_SOURCECHANNEL */
    else
    {
      SchM_Exit_Mirror_SourceChannelLock();

#if (MIRROR_DEV_ERROR_DETECT == STD_ON)
      errorId = MIRROR_E_MODE_INV;
#endif
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (MIRROR_DEV_ERROR_REPORT == STD_ON)
  if(errorId != MIRROR_E_NO_ERROR)
  {
    (void)Det_ReportError(MIRROR_MODULE_ID, MIRROR_INSTANCE_ID_DET, MIRROR_SID_CAN_FRAME, errorId);
  }
#else
  MIRROR_DUMMY_STATEMENT(errorId);  /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint !e438 */
#endif

  return retVal;
} /* Mirror_ReportCanFrame() */ /* PRQA S 6080 1 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
* Mirror_CanRxFrame()
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Can_ReturnType, MIRROR_APPL_CODE) Mirror_CanRxFrame(uint8 Controller, Can_IdType ID, uint8 DLC, Can_DataPtrType DataPtr)
{
  Can_PduType CanData;

  /* #10 Convert the incoming data to the format used intern to process incoming CAN frames. */
  CanData.id = ID;
  CanData.length = DLC;
  CanData.sdu = (Can_SduPtrType) DataPtr; /* PRQA S 0311 */ /* MD_Mirror_11.5 */
  CanData.swPduHandle = 0; /* not used */

  /* #20 Call the report function for incoming CAN frames. */
  return Mirror_ReportCanFrame(Controller, &CanData);
} /* Mirror_CanRxFrame() */

/**********************************************************************************************************************
 * Mirror_TxConfirmation()
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
FUNC(void, MIRROR_CODE) Mirror_TxConfirmation(PduIdType TxPduId)
{
  uint8 destChannelIndex = 0;

  /* #10 Enter MIRROR_EXCLUSIVE_AREA_TXPDU */
  SchM_Enter_Mirror_TxPduLock();

  destChannelIndex = Mirror_CurrentDestChannelIndex;

  /* #20 If the given Tx PDU ID is matching the ID of the current destination channel: */
  if(TxPduId == Mirror_GetMirrorConfTxPduIdOfDestChannel(destChannelIndex))
  {
    /* #30 Reset the Tx PDU. */
    Mirror_DestChannel_TxPduLocked = FALSE;

#if (MIRROR_DESTINATION_CHANNEL_CAN == STD_ON)
    /* #40 If the current destination channel is of the type MIRROR_E_MIRROR_CHANNEL_CAN_CHANNELTYPEOFDESTCHANNEL: */
    if(Mirror_GetChannelTypeOfDestChannel(destChannelIndex) == MIRROR_E_MIRROR_CHANNEL_CAN_CHANNELTYPEOFDESTCHANNEL)
    {
      /* #50 Exit MIRROR_EXCLUSIVE_AREA_TXPDU */
      SchM_Exit_Mirror_TxPduLock();
      
      /* #60 Try to transmit the next pending CAN frame from the queue. */
      Mirror_DestChannelCan_QueueTransmit(destChannelIndex);
    }
    /* #70 Otherwise, Exit MIRROR_EXCLUSIVE_AREA_TXPDU */
    else
#endif
    {
      SchM_Exit_Mirror_TxPduLock();
    }
  }
  /* #80 Otherwise, Exit MIRROR_EXCLUSIVE_AREA_TXPDU */
  else
  {
    SchM_Exit_Mirror_TxPduLock();
  }
} /* Mirror_TxConfirmation() */

#define MIRROR_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Justification for module-specific MISRA deviations:
 MD_Mirror_0310: rule 11.4
      Reason:     Casting to different object pointer type / A cast should not be performed between a pointer to object
                  type and a different pointer to object type.
      Risk:       There is no risk because the start address of an array is the same, no matter if it is of type uint8 or uint16.
      Prevention: Covered by code review.

 MD_Mirror_9.2: rule 9.2
      Reason:     Array initializer is missing the optional {.
      Risk:       There is no risk because the macro returns the complete complex structure.
      Prevention: Covered by code review.

 MD_Mirror_11.5:  rule 11.5
      Reason:     Dangerous pointer cast results in loss of const qualification.
      Risk:       There is no risk because the struct is only used internal and the data are not modified.
      Prevention: Covered by code review.

 MD_Mirror_8.8:  rule 8.8
      Reason:     The global identifier '-identifier-' has been declared in more than one file.
      Risk:       There is no risk.
      Prevention: Covered by code review.
*/

/**********************************************************************************************************************
 *  END OF FILE: Mirror.c
 *********************************************************************************************************************/
