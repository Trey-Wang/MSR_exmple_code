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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  J1939Nm_Dynamic.c
 *       Module:  SAE J1939 Network Management (J1939Nm)
 *
 *  Description:  Implementation of the dynamic sub-module of the SAE J1939 Network Management module.
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

/* PRQA S 0715 EOF */ /* MD_MSR_1.1_715 */
/* PRQA S 0779 EOF */ /* MD_MSR_5.1_777 */

/* *INDENT-ON* */


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#define J1939NM_DYNAMIC_C

#include "J1939Nm.h"
#include "J1939Nm_Cbk.h"
#include "J1939Nm_NvM.h"

#include "J1939Nm_Cfg.h"
#include "J1939Nm_Lcfg.h"
#include "J1939Nm_PBcfg.h"

#include "J1939Nm_Int.h"
#include "J1939Nm_Dynamic.h"

#include "BswM_J1939Nm.h"
#include "Nm_Cbk.h"
#include "J1939Rm.h"

#if ((J1939NM_ADDRESSSTORAGEBLOCK == STD_ON) || (J1939NM_NAMESTORAGEBLOCK == STD_ON))
# include "NvM.h"
#endif

#include "SchM_J1939Nm.h"


/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Vendor specific BSW module version information */
#define J1939NM_MAJOR_SOURCE_VERSION            (3u)
#define J1939NM_MINOR_SOURCE_VERSION            (0u)
#define J1939NM_PATCH_SOURCE_VERSION            (0u)

/* Check the version of J1939Nm module header file */
#if ((J1939NM_SW_MAJOR_VERSION != J1939NM_MAJOR_SOURCE_VERSION) || (J1939NM_SW_MINOR_VERSION != J1939NM_MINOR_SOURCE_VERSION) || (J1939NM_SW_PATCH_VERSION != J1939NM_PATCH_SOURCE_VERSION))
# error "Vendor specific version numbers of J1939Nm.c and J1939Nm.h are inconsistent"
#endif

/* Check the PDU length against the Name Management queue data length */
#if (J1939NM_NM_PDU_LENGTH != J1939NM_NM_QUEUE_DATA_SIZE)
# error "Internal error: Inconsistent definition of J1939NM_NM_PDU_LENGTH and J1939NM_NM_QUEUE_DATA_SIZE."
#endif


/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
#if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) && (J1939NM_CAPDU == STD_ON))
typedef struct
{
  uint8 Buffer[J1939NM_CA_FRAME_LENGTH];
  PduIdType PduId;
  boolean Locked;
  uint8 Position;
} CaMgmtType;
#endif

typedef enum
{
  J1939NM_NM_IDLE = 0,
  J1939NM_NM_PENDING = 1
} J1939Nm_NmNodeStateType;

typedef enum
{
  J1939NM_NM_MODE_SET_PENDING_NAME = 0,
  J1939NM_NM_MODE_PENDING_NAME_RESPONSE = 1,
  J1939NM_NM_MODE_CURRENT_NAME_RESPONSE = 2,
  J1939NM_NM_MODE_SET_PENDING_NAME_ACK = 3,
  J1939NM_NM_MODE_SET_PENDING_NAME_NACK = 4,
  J1939NM_NM_MODE_REQUEST_PENDING_NAME = 5,
  J1939NM_NM_MODE_REQUEST_CURRENT_NAME = 6,
  J1939NM_NM_MODE_ADOPT_PENDING_NAME = 7
} J1939Nm_NmModeType;

typedef enum
{
  J1939NM_NM_ACK_CODE_DIFFERENT_SA = 0,
  J1939NM_NM_ACK_CODE_CHANGE_NOT_ALLOWED = 1,
  J1939NM_NM_ACK_CODE_ITEM_CONFLICT = 2,
  J1939NM_NM_ACK_CODE_INVALID_CHECKSUM = 3,
  J1939NM_NM_ACK_CODE_PENDING_NAME_NOT_SET = 4,
  J1939NM_NM_ACK_CODE_OTHER = 5,
  J1939NM_NM_ACK_CODE_UNSUED = 6,
  J1939NM_NM_ACK_CODE_NOT_AVAILABLE = 255
} J1939Nm_NmAckCodeType;

typedef struct
{
  J1939Nm_NameType PendingName;
  J1939Nm_NmNodeStateType State;
  uint16 Timer;
  uint8 QualifierFlags;
  uint8 Checksum;
  uint8 TesterAddr;
  uint8 ChannelId;
  uint8 NodeId;
} J1929Nm_NmMgmtType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/*====================================== J1939NM_START_SEC_VAR_NOINIT_UNSPECIFIED ====================================*/
#define J1939NM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) && (J1939NM_CAPDU == STD_ON))
/*! \brief Buffer and control structure for CommandedAddress */
static VAR(CaMgmtType, J1939NM_VAR_NOINIT) caMgmt;
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/*! \brief Buffer and control structure for CommandedAddress */
static VAR(J1929Nm_NmMgmtType, J1939NM_VAR_NOINIT) nmMgmt;
#endif



#define J1939NM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*======================================= J1939NM_STOP_SEC_VAR_NOINIT_UNSPECIFIED ====================================*/


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*================================================ J1939NM_START_SEC_CODE ===========================================*/
#define J1939NM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) && (J1939NM_EXISTS_REMOTE_NODETYPEOFEXTERNALNODE == STD_ON))
static FUNC(void, J1939NM_CODE) J1939Nm_InternalGetNameFilter(J1939Nm_NameType nodeNameFilter,
                                                              const J1939Nm_SizeOfExternalNodeNameFilterType
                                                              extNodeNameFilterIdx);
#endif
#if (((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) && (J1939NM_EXISTS_REMOTE_NODETYPEOFEXTERNALNODE == STD_ON)) || (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON))
static FUNC(boolean, J1939NM_CODE) J1939Nm_InternalFilteredNameCompare(J1939Nm_ConstNameType ownNodeName,
                                                                       J1939Nm_ConstNameType otherNodeName,
                                                                       J1939Nm_ConstNameType nodeNameFilter);
#endif
#if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) && ((J1939NM_CAPDU == STD_ON) || (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON)))
static FUNC(boolean, J1939NM_CODE) J1939Nm_InternalSetAddress(J1939Nm_SizeOfNodeType nodeIdx,
                                                              J1939Nm_SizeOfNodeChannelType nodeChannelIdx,
                                                              J1939Nm_SizeOfChannelType channelIdx, uint8 newAddress);
#endif

#if ((J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON) || (J1939NM_NM_PG_SUPPORT == STD_ON))
/**********************************************************************************************************************
 *  J1939Nm_InternalSetName()
 *********************************************************************************************************************/
/*! \brief       This function updates the current name of a node.
 *  \details     If enabled the name is stored in NvM.
 *  \param[in]   channelIdx  Internal Channel index.
 *  \param[in]   nodeId      Internal Node index.
 *  \param[in]   name        New name for the node.
 *  \return      E_OK - If the set operation was sucessfull
 *  \return      E_NOT_OK - If there was a name conflict
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config
  *********************************************************************************************************************/
static FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_InternalSetName(J1939Nm_ChannelIdxOfComMChannelType channelIdx,
                                                                  uint16 nodeId, P2CONST(uint8, AUTOMATIC,
                                                                                         J1939NM_APPL_DATA) name);
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalCalcNmCheckSum()
 *********************************************************************************************************************/
/*! \brief       Calculates the checksum of the current pending name
 *  \details
 *  \param[in]   name  Node name to calculate the checksum.
 *  \return      The checksum of the current pending name
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config
  *********************************************************************************************************************/
static FUNC(uint8, J1939NM_CODE) J1939Nm_InternalCalcNmCheckSum(J1939Nm_ConstNameType name);
#endif

#if ((J1939NM_NM_PG_SUPPORT == STD_ON) && (J1939NM_NMQUEUE == STD_ON))
/**********************************************************************************************************************
 *  J1939Nm_InternalCopyNmQueueData()
 *********************************************************************************************************************/
/*! \brief       Copies data from a source to destination buffer
 *  \details     Can only be used for Nm Queue to copy 10 Byte of data.
 *  \param[in]   dataSrc   Source data buffer
 *  \param[in]   dataDest  Destination data buffer
 *  \return      The checksum of the current pending name
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config
  *********************************************************************************************************************/
static FUNC(void, J1939NM_CODE) J1939Nm_InternalCopyNmQueueData(const uint8 * dataSrc, uint8 * dataDest);
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalUpdatePendingName()
 *********************************************************************************************************************/
/*! \brief       Updates the pending name.
 *  \details     Only the fields where the qualifier flag is set to 0 are updated.
 *  \param[in]   info  Pointer to the data.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config
  *********************************************************************************************************************/
static FUNC(void,
            J1939NM_CODE) J1939Nm_InternalUpdatePendingName(P2CONST(PduInfoType, AUTOMATIC, J1939NM_APPL_DATA) info);
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalSendNm()
 *********************************************************************************************************************/
/*! \brief       This function sents a NM message with the pending or current name of a node
 *  \details
 *  \param[in]   channelIdx     Internal channel index.
 *  \param[in]   nodeIdx        Internal node index.
 *  \param[in]   data           Pointer to the data.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config
  *********************************************************************************************************************/
static FUNC(void, J1939NM_CODE) J1939Nm_InternalSendNm(J1939Nm_ChannelChannelIdxOfRxPduType channelIdx,
                                                       J1939Nm_NodeIdxOfNodeChannelType nodeIdx, uint8 * data);
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalNmSendName()
 *********************************************************************************************************************/
/*! \brief       This function sents a NM message with the pending or current name of a node
 *  \details
 *  \param[in]   channelIdx     Internal channel index.
 *  \param[in]   node           Internal node index.
 *  \param[in]   sourceAddress  Source address.
 *  \param[in]   destAddress    Destination address
 *  \param[in]   name           Name value to send.
 *  \param[in]   mode           Value of the mode field.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config
  *********************************************************************************************************************/
static FUNC(void, J1939NM_CODE) J1939Nm_InternalNmSendName(J1939Nm_ChannelChannelIdxOfRxPduType channelIdx,
                                                           J1939Nm_NodeIdxOfNodeChannelType node, uint8 sourceAddress,
                                                           uint8 destAddress, uint8 * name, J1939Nm_NmModeType mode);
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalNmSendAck()
 *********************************************************************************************************************/
/*! \brief       This function sents a acknowledgment message.
 *  \details     This function can only be used for name management acknowledgments.
 *  \param[in]   channelIdx     Internal channel index.
 *  \param[in]   nodeIdx        Internal node index.
 *  \param[in]   sourceAddress  Source address.
 *  \param[in]   destAddress    Destination address.
 *  \param[in]   mode           Value of the mode field.
 *  \param[in]   ackCode        Acknowledgment code.
 *  \param[in]   qfValue        Qualifier value.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config
  *********************************************************************************************************************/
static FUNC(void, J1939NM_CODE) J1939Nm_InternalNmSendAck(J1939Nm_ChannelChannelIdxOfRxPduType channelIdx,
                                                          J1939Nm_NodeIdxOfNodeChannelType nodeIdx,
                                                          uint8 sourceAddress, uint8 destAddress,
                                                          J1939Nm_NmModeType mode, J1939Nm_NmAckCodeType ackCode,
                                                          uint8 qfValue);
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalNmHandleSetPendingName()
 *********************************************************************************************************************/
/*! \brief       This functions handles a set pending name request.
 *  \details     Updates the pending name and sent the confirmation.
 *  \param[in]   info            Pointer to the data.
 *  \param[in]   channelIdx      Internal channel index.
 *  \param[in]   nodeIdx         Internal node index.
 *  \param[in]   sourceAddress   Source address.
 *  \param[in]   destAddress     Destination address.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config
  *********************************************************************************************************************/
static FUNC(void,
            J1939NM_CODE) J1939Nm_InternalNmHandleSetPendingName(P2CONST(PduInfoType, AUTOMATIC,
                                                                         J1939NM_APPL_DATA) info,
                                                                 J1939Nm_ChannelChannelIdxOfRxPduType channelIdx,
                                                                 J1939Nm_NodeIdxOfNodeChannelType nodeIdx,
                                                                 uint8 sourceAddress, uint8 destAddress);
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalNmHandleRequestPendingName()
 *********************************************************************************************************************/
/*! \brief       This function handles a request for the pending name.
 *  \details
 *  \param[in]   channelIdx       Internal channel index.
 *  \param[in]   nodeIdx          Internal node index.
 *  \param[in]   sourceAddress    Source address.
 *  \param[in]   destAddress      Destination address.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config
  *********************************************************************************************************************/
static FUNC(void,
            J1939NM_CODE) J1939Nm_InternalNmHandleRequestPendingName(J1939Nm_ChannelChannelIdxOfRxPduType channelIdx,
                                                                     J1939Nm_NodeIdxOfNodeChannelType nodeIdx,
                                                                     uint8 sourceAddress, uint8 destAddress);
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalNmHandleRequestCurrentName()
 *********************************************************************************************************************/
/*! \brief       This function handles a request for the current name.
 *  \details
 *  \param[in]   channelIdx      Internal channel index.
 *  \param[in]   nodeIdx         Internal node index.
 *  \param[in]   sourceAddress   Source address.
 *  \param[in]   destAddress     Destination address.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config
  *********************************************************************************************************************/
static FUNC(void,
            J1939NM_CODE) J1939Nm_InternalNmHandleRequestCurrentName(J1939Nm_ChannelChannelIdxOfRxPduType channelIdx,
                                                                     J1939Nm_NodeIdxOfNodeChannelType nodeIdx,
                                                                     uint8 sourceAddress, uint8 destAddress);
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalNmHandleAdoptPendingName()
 *********************************************************************************************************************/
/*! \brief       This function handles a request to adopt the pending name.
 *  \details     The pending name is set as current name.
 *  \param[in]   channelIdx     Internal channel index.
 *  \param[in]   nodeIdx        Internal node index.
 *  \param[in]   sourceAddress  Source address.
 *  \param[in]   destAddress    Destination address.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config
  *********************************************************************************************************************/
static FUNC(void,
            J1939NM_CODE) J1939Nm_InternalNmHandleAdoptPendingName(J1939Nm_ChannelChannelIdxOfRxPduType channelIdx,
                                                                   J1939Nm_NodeIdxOfNodeChannelType nodeIdx,
                                                                   uint8 sourceAddress, uint8 destAddress);
#endif

#if ((J1939NM_NM_PG_SUPPORT == STD_ON) && (J1939NM_NMQUEUE == STD_ON))
/**********************************************************************************************************************
 *  J1939Nm_InternalProcessNmQueue()
 *********************************************************************************************************************/
/*! \brief       Send next element from the Name Management queue.
 *  \details
 *  \param[in]   channelIdx  Internal index of the channel on which the Name Management PG shall be sent.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config
  *********************************************************************************************************************/
static FUNC(void, J1939NM_CODE) J1939Nm_InternalProcessNmQueue(J1939Nm_SizeOfChannelType channelIdx);
#endif

#define J1939NM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*================================================ J1939NM_STOP_SEC_CODE ===========================================*/


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/*================================================ J1939NM_START_SEC_CODE ===========================================*/
#define J1939NM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) && (J1939NM_EXISTS_REMOTE_NODETYPEOFEXTERNALNODE == STD_ON))
/**********************************************************************************************************************
 *  J1939Nm_InternalGetNameFilter
 *********************************************************************************************************************/
/*! \brief      Get a NAME filter from the configuration.
 *
 *  \param      extNodeNameFilterIdx  Index of NAME filter of external node.
 *  \param      nodeNameFilter  Byte array where to store the NAME filter.
 *  \return     none
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/

static FUNC(void, J1939NM_CODE) J1939Nm_InternalGetNameFilter(J1939Nm_NameType nodeNameFilter,
                                                              const J1939Nm_SizeOfExternalNodeNameFilterType
                                                              extNodeNameFilterIdx)
{
  nodeNameFilter[J1939NM_NAME_BYTE8] = J1939Nm_GetNodeNameFilterByte8OfExternalNodeNameFilter(extNodeNameFilterIdx);
  nodeNameFilter[J1939NM_NAME_BYTE7] = J1939Nm_GetNodeNameFilterByte7OfExternalNodeNameFilter(extNodeNameFilterIdx);
  nodeNameFilter[J1939NM_NAME_BYTE6] = J1939Nm_GetNodeNameFilterByte6OfExternalNodeNameFilter(extNodeNameFilterIdx);
  nodeNameFilter[J1939NM_NAME_BYTE5] = J1939Nm_GetNodeNameFilterByte5OfExternalNodeNameFilter(extNodeNameFilterIdx);
  nodeNameFilter[J1939NM_NAME_BYTE4] = J1939Nm_GetNodeNameFilterByte4OfExternalNodeNameFilter(extNodeNameFilterIdx);
  nodeNameFilter[J1939NM_NAME_BYTE3] = J1939Nm_GetNodeNameFilterByte3OfExternalNodeNameFilter(extNodeNameFilterIdx);
  nodeNameFilter[J1939NM_NAME_BYTE2] = J1939Nm_GetNodeNameFilterByte2OfExternalNodeNameFilter(extNodeNameFilterIdx);
  nodeNameFilter[J1939NM_NAME_BYTE1] = J1939Nm_GetNodeNameFilterByte1OfExternalNodeNameFilter(extNodeNameFilterIdx);
}
#endif


#if (((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) && (J1939NM_EXISTS_REMOTE_NODETYPEOFEXTERNALNODE == STD_ON)) || (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON))
/**********************************************************************************************************************
 *  J1939Nm_InternalFilteredNameCompare
 *********************************************************************************************************************/
/*! \brief      Compare the NAMEs of two external nodes, applying a filter to both names.
 *
 *  \param      ownNodeName     Current NAME of external node.
 *  \param      otherNodeName   NAME received with AC.
 *  \param      nodeNameFilter  NAME filter of external node.
 *  \return     TRUE:           Both NAMEs are equal,
 *              FALSE:          NAMEs are not equal.
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/

static FUNC(boolean, J1939NM_CODE) J1939Nm_InternalFilteredNameCompare(J1939Nm_ConstNameType ownNodeName,
                                                                       J1939Nm_ConstNameType otherNodeName,
                                                                       J1939Nm_ConstNameType nodeNameFilter)
{
  boolean cmp = FALSE;

  if (((ownNodeName[J1939NM_NAME_BYTE8] & nodeNameFilter[J1939NM_NAME_BYTE8]) ==
       (otherNodeName[J1939NM_NAME_BYTE8] & nodeNameFilter[J1939NM_NAME_BYTE8])) &&
      ((ownNodeName[J1939NM_NAME_BYTE7] & nodeNameFilter[J1939NM_NAME_BYTE7]) ==
       (otherNodeName[J1939NM_NAME_BYTE7] & nodeNameFilter[J1939NM_NAME_BYTE7])) &&
      ((ownNodeName[J1939NM_NAME_BYTE6] & nodeNameFilter[J1939NM_NAME_BYTE6]) ==
       (otherNodeName[J1939NM_NAME_BYTE6] & nodeNameFilter[J1939NM_NAME_BYTE6])) &&
      ((ownNodeName[J1939NM_NAME_BYTE5] & nodeNameFilter[J1939NM_NAME_BYTE5]) ==
       (otherNodeName[J1939NM_NAME_BYTE5] & nodeNameFilter[J1939NM_NAME_BYTE5])) &&
      ((ownNodeName[J1939NM_NAME_BYTE4] & nodeNameFilter[J1939NM_NAME_BYTE4]) ==
       (otherNodeName[J1939NM_NAME_BYTE4] & nodeNameFilter[J1939NM_NAME_BYTE4])) &&
      ((ownNodeName[J1939NM_NAME_BYTE3] & nodeNameFilter[J1939NM_NAME_BYTE3]) ==
       (otherNodeName[J1939NM_NAME_BYTE3] & nodeNameFilter[J1939NM_NAME_BYTE3])) &&
      ((ownNodeName[J1939NM_NAME_BYTE2] & nodeNameFilter[J1939NM_NAME_BYTE2]) ==
       (otherNodeName[J1939NM_NAME_BYTE2] & nodeNameFilter[J1939NM_NAME_BYTE2])) &&
      ((ownNodeName[J1939NM_NAME_BYTE1] & nodeNameFilter[J1939NM_NAME_BYTE1]) ==
       (otherNodeName[J1939NM_NAME_BYTE1] & nodeNameFilter[J1939NM_NAME_BYTE1])))
  {
    cmp = TRUE;
  }

  return cmp;
}
#endif


#if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) && ((J1939NM_CAPDU == STD_ON) || (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON)))
/**********************************************************************************************************************
 *  J1939Nm_InternalSetAddress
 *********************************************************************************************************************/
/*! \brief      Change the address of an internal node.
 *
 *  \param      nodeIdx         Index of the internal node for which an address shall be changed.
 *  \param      nodeChannelIdx  Index of the internal node channel that corresponds to nodeIdx and channelIdx.
 *  \param      channelIdx      Internal index of the channel for which the node shall change its address.
 *  \param      newAddress      New address.
 *  \return     TRUE:           New address could be set,
 *              FALSE:          Address change failed.
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/

static FUNC(boolean, J1939NM_CODE) J1939Nm_InternalSetAddress(J1939Nm_SizeOfNodeType nodeIdx,
                                                              J1939Nm_SizeOfNodeChannelType nodeChannelIdx,
                                                              J1939Nm_SizeOfChannelType channelIdx, uint8 newAddress)
{
  Std_ReturnType result = E_NOT_OK;

  if (J1939Nm_GetNodeAddressConfigurationCapabilityOfNode(nodeIdx) !=
      J1939NM_NCA_NODEADDRESSCONFIGURATIONCAPABILITYOFNODE)
  {
    J1939Nm_NodeChannelStateType *nodeChannelState =
      &J1939Nm_GetNodeChannelState(J1939Nm_GetNodeChannelStateIdxOfNodeChannel(nodeChannelIdx));
    J1939Nm_CurrentNodeAddressIdxOfNodeChannelType currentAddressIdx =
      J1939Nm_GetCurrentNodeAddressIdxOfNodeChannel(nodeChannelIdx);
    uint8 currentAddress = J1939Nm_GetCurrentNodeAddress(currentAddressIdx);

    if ((nodeChannelState->NodeState == J1939NM_NODESTATE_NORMAL) ||
        (nodeChannelState->NodeState == J1939NM_NODESTATE_CLAIMING) ||
        (nodeChannelState->NodeState == J1939NM_NODESTATE_PRE_CLAIMING) ||
        (nodeChannelState->NodeState == J1939NM_NODESTATE_PRE_NORMAL) ||
        (nodeChannelState->NodeState == J1939NM_NODESTATE_ACLOST))
    {
      /* \trace SPEC-2168275, SPEC-2168276, SPEC-2168277 */
      nodeChannelState->AcPending = TRUE;

      if (currentAddress != newAddress)
      {
        J1939Nm_ChannelStateType *channelState = &J1939Nm_GetChannelState(channelIdx);

        J1939Nm_ClearBusAddressAllocated(channelState, currentAddress);

        if (J1939Nm_IsBusAddressAllocated(channelState, newAddress))
        {
          J1939Nm_NodeChannelChannelIndEndIdxOfChannelType localNodeChannelIndIdx;
          boolean addressFound = FALSE;

          for (localNodeChannelIndIdx = J1939Nm_GetNodeChannelChannelIndStartIdxOfChannel(channelIdx);
               localNodeChannelIndIdx < J1939Nm_GetNodeChannelChannelIndEndIdxOfChannel(channelIdx);
               localNodeChannelIndIdx++)
          {
            J1939Nm_NodeChannelChannelIndType localNodeChannelIdx =
              J1939Nm_GetNodeChannelChannelInd(localNodeChannelIndIdx);
            J1939Nm_NodeChannelStateType *localNodeChannelState =
              &J1939Nm_GetNodeChannelState(J1939Nm_GetNodeChannelStateIdxOfNodeChannel(localNodeChannelIdx));

            if ((localNodeChannelState->NodeState == J1939NM_NODESTATE_NORMAL) ||
                (localNodeChannelState->NodeState == J1939NM_NODESTATE_CLAIMING))
            {
              uint8 nodeAddress =
                J1939Nm_GetCurrentNodeAddress(J1939Nm_GetCurrentNodeAddressIdxOfNodeChannel(localNodeChannelIdx));

              if (nodeAddress == newAddress)
              {
                J1939Nm_NodeIdxOfNodeChannelType localNodeIdx = J1939Nm_GetNodeIdxOfNodeChannel(localNodeChannelIdx);

                CanIf_ResetAddressTableEntry(J1939Nm_GetCanIfCtrlIdOfChannel(channelIdx),
                                             J1939Nm_GetNodeAddressOfNode(localNodeIdx));
                J1939Nm_SetPreviousNodeAddress(J1939Nm_GetPreviousNodeAddressIdxOfNodeChannel(localNodeChannelIdx),
                                               J1939NM_NULL_ADDRESS);

                addressFound = TRUE;
                break;
              }
            }
          }

          if (!addressFound)
          {
            J1939Nm_ExternalNodeChannelChannelIndEndIdxOfChannelType extNodeChannelIndIdx;

            for (extNodeChannelIndIdx = J1939Nm_GetExternalNodeChannelChannelIndStartIdxOfChannel(channelIdx);
                 extNodeChannelIndIdx < J1939Nm_GetExternalNodeChannelChannelIndEndIdxOfChannel(channelIdx);
                 extNodeChannelIndIdx++)
            {
              J1939Nm_ExternalNodeChannelChannelIndType extNodeChannelIdx =
                J1939Nm_GetExternalNodeChannelChannelInd(extNodeChannelIndIdx);
              J1939Nm_ExternalNodeChannelStateIdxOfExternalNodeChannelType extNodeChannelStateIdx =
                J1939Nm_GetExternalNodeChannelStateIdxOfExternalNodeChannel(extNodeChannelIdx);
              J1939Nm_ExternalNodeChannelStateType *extNodeChannelState =
                &J1939Nm_GetExternalNodeChannelState(extNodeChannelStateIdx);

              if (extNodeChannelState->ExtNodeState != J1939NM_EXTNODESTATE_UNKNOWN)
              {
                J1939Nm_CurrentExtNodeAddressIdxOfExternalNodeChannelType currentExtNodeAddressIdx =
                  J1939Nm_GetCurrentExtNodeAddressIdxOfExternalNodeChannel(extNodeChannelIdx);
                uint8 extNodeAddress = J1939Nm_GetCurrentExtNodeAddress(currentExtNodeAddressIdx);

                if (extNodeAddress == newAddress)
                {
                  J1939Nm_ExternalNodeIdxOfExternalNodeChannelType extNodeIdx =
                    J1939Nm_GetExternalNodeIdxOfExternalNodeChannel(extNodeChannelIdx);

                  extNodeChannelState->ExtNodeState = J1939NM_EXTNODESTATE_CANNOT_CLAIM;
                  J1939Nm_SetCurrentExtNodeAddress(currentExtNodeAddressIdx, J1939NM_NULL_ADDRESS);
                  CanIf_ResetAddressTableEntry(J1939Nm_GetCanIfCtrlIdOfChannel(channelIdx),
                                               J1939Nm_GetNodeAddressOfExternalNode(extNodeIdx));

                  break;
                }
              }
            }
          }
        }
        else
        {
          J1939Nm_SetBusAddressAllocated(channelState, newAddress);
        }
      }
    }

    if (currentAddress != newAddress)
    {
      J1939Nm_SetCurrentNodeAddress(currentAddressIdx, newAddress);

# if (J1939NM_ADDRESSSTORAGEBLOCK == STD_ON)
      (void) NvM_SetRamBlockStatus(NvMConf_NvMBlockDescriptor_J1939Nm_CurrentNodeAddresses, TRUE);
# endif
    }

    if (nodeChannelState->NodeState == J1939NM_NODESTATE_ACLOST)
    {
# if (J1939NM_NODESTARTUPDELAYOFNODE == STD_ON)
      if (J1939Nm_IsNodeStartUpDelayOfNode(nodeIdx) && (newAddress >= J1939NM_MIN_DELAY_ADDRESS) &&
          (newAddress <= J1939NM_MAX_DELAY_ADDRESS))
      {
        nodeChannelState->NodeState = J1939NM_NODESTATE_PRE_CLAIMING;
      }
      else
# endif
      {
        nodeChannelState->NodeState = J1939NM_NODESTATE_PRE_NORMAL;
# if (J1939NM_J1939NM_E_ADDRESS_LOST == STD_ON)
        /* \trace SPEC-2168286, SPEC-2168305 */
        Dem_ReportErrorStatus(J1939Nm_GetJ1939NM_E_ADDRESS_LOST(), DEM_EVENT_STATUS_PREPASSED);
# endif
      }
    }

    result = E_OK;
  }

  return result;
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
#endif

/**********************************************************************************************************************
 *  J1939Nm_InternalSetName()
 *********************************************************************************************************************/
#if ((J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON) || (J1939NM_NM_PG_SUPPORT == STD_ON))
static FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_InternalSetName(J1939Nm_ChannelIdxOfComMChannelType channelIdx,
                                                                  uint16 nodeId, P2CONST(uint8, AUTOMATIC,
                                                                                         J1939NM_APPL_DATA) name)
{
  Std_ReturnType retVal = E_NOT_OK;
  J1939Nm_NodeChannelChannelIndEndIdxOfChannelType otherNodeChannelIndIdx;

# if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
  J1939Nm_ExternalNodeChannelChannelIndEndIdxOfChannelType extNodeChannelIndIdx;
# endif
  boolean nameNonUnique = FALSE;

  for (otherNodeChannelIndIdx = J1939Nm_GetNodeChannelChannelIndStartIdxOfChannel(channelIdx);
       (!nameNonUnique) && (otherNodeChannelIndIdx < J1939Nm_GetNodeChannelChannelIndEndIdxOfChannel(channelIdx));
       otherNodeChannelIndIdx++)
  {
    J1939Nm_NodeChannelNodeIndType otherNodeChannelIdx = J1939Nm_GetNodeChannelChannelInd(otherNodeChannelIndIdx);
    J1939Nm_NodeIdxOfNodeChannelType otherNodeIdx = J1939Nm_GetNodeIdxOfNodeChannel(otherNodeChannelIdx);
    /* TODO: Move the following assignment below the following if statement after QAC was fixed */
    uint8 *otherNodeName = J1939Nm_GetCurrentNodeName(J1939Nm_GetCurrentNodeNameIdxOfNode(otherNodeIdx));

    if (otherNodeIdx != nodeId)
    {
      SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      if (J1939Nm_InternalNameCompare(otherNodeName, name) == 0)
      {
        nameNonUnique = TRUE;
      }
      SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }

# if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
  for (extNodeChannelIndIdx = J1939Nm_GetExternalNodeChannelChannelIndStartIdxOfChannel(channelIdx);
       (!nameNonUnique) &&
       (extNodeChannelIndIdx < J1939Nm_GetExternalNodeChannelChannelIndEndIdxOfChannel(channelIdx));
       extNodeChannelIndIdx++)
  {
    J1939Nm_ExternalNodeChannelNodeIndType extNodeChannelIdx =
      J1939Nm_GetExternalNodeChannelChannelInd(extNodeChannelIndIdx);
    J1939Nm_ExternalNodeChannelStateIdxOfExternalNodeChannelType extNodeChannelStateIdx =
      J1939Nm_GetExternalNodeChannelStateIdxOfExternalNodeChannel(extNodeChannelIdx);
    J1939Nm_ExternalNodeChannelStateType *extNodeChannelState =
      &J1939Nm_GetExternalNodeChannelState(extNodeChannelStateIdx);

    if (extNodeChannelState->ExtNodeState != J1939NM_EXTNODESTATE_UNKNOWN)
    {
      J1939Nm_ExternalNodeIdxOfExternalNodeChannelType extNodeIdx =
        J1939Nm_GetExternalNodeIdxOfExternalNodeChannel(extNodeChannelIdx);
      uint8 *extNodeName = J1939Nm_GetCurrentExtNodeName(J1939Nm_GetCurrentExtNodeNameIdxOfExternalNode(extNodeIdx));

      SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      if (J1939Nm_InternalNameCompare(extNodeName, name) == 0)
      {
        nameNonUnique = TRUE;
      }
      SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
# endif

  if (nameNonUnique) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
  {
    J1939Nm_ReportDetError(J1939NM_SID_SETNAME, J1939NM_E_DUPLICATE_NAME);
  }
  else
  {
    uint8 *currentNodeName = J1939Nm_GetCurrentNodeName(J1939Nm_GetCurrentNodeNameIdxOfNode(nodeId));

    SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    J1939Nm_InternalCopyName(currentNodeName, name);
    SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

# if (J1939NM_ADDRESSSTORAGEBLOCK == STD_ON)
    (void) NvM_SetRamBlockStatus(NvMConf_NvMBlockDescriptor_J1939Nm_CurrentNodeNames, TRUE);
# endif

# if (J1939NM_USESADDRESSARBITRATIONOFCHANNEL == STD_ON)
    if (J1939Nm_IsUsesAddressArbitrationOfChannel(channelIdx))
    {
      J1939Nm_NodeChannelNodeIndEndIdxOfNodeType nodeChannelIndIdx;

      for (nodeChannelIndIdx = J1939Nm_GetNodeChannelNodeIndStartIdxOfNode(nodeId);
           nodeChannelIndIdx < J1939Nm_GetNodeChannelNodeIndEndIdxOfNode(nodeId); nodeChannelIndIdx++)
      {
        J1939Nm_NodeChannelNodeIndType nodeChannelIdx = J1939Nm_GetNodeChannelNodeInd(nodeChannelIndIdx);
        J1939Nm_NodeChannelStateType *nodeChannelState =
          &J1939Nm_GetNodeChannelState(J1939Nm_GetNodeChannelStateIdxOfNodeChannel(nodeChannelIdx));

        if ((nodeChannelState->NodeState == J1939NM_NODESTATE_NORMAL) ||
            (nodeChannelState->NodeState == J1939NM_NODESTATE_CLAIMING))
        {
          /* \trace SPEC-2168275, SPEC-2168276, SPEC-2168277 */
          nodeChannelState->AcPending = TRUE;
        }
      }
    }
# endif

    retVal = E_OK;
  }

  return retVal;
} /* J1939Nm_InternalSetName() */ /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalDropPendingName()
 *********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_InternalDropPendingName(void)
{
  nmMgmt.State = J1939NM_NM_IDLE;
} /* J1939Nm_InternalDropPendingName() */
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalCalcNmCheckSum()
 *********************************************************************************************************************/
static FUNC(uint8, J1939NM_CODE) J1939Nm_InternalCalcNmCheckSum(J1939Nm_ConstNameType name)
{
  uint32 checkSum = 0;

  checkSum += name[J1939NM_NAME_BYTE1];
  checkSum += name[J1939NM_NAME_BYTE2];
  checkSum += name[J1939NM_NAME_BYTE3];
  checkSum += name[J1939NM_NAME_BYTE4];
  checkSum += name[J1939NM_NAME_BYTE5];
  checkSum += name[J1939NM_NAME_BYTE6];
  checkSum += name[J1939NM_NAME_BYTE7];
  checkSum += name[J1939NM_NAME_BYTE8];

  return (uint8) (checkSum % J1939NM_NM_CHECKSUM_MOD);
} /* J1939Nm_InternalCalcNmCheckSum() */
#endif

#if ((J1939NM_NM_PG_SUPPORT == STD_ON) && (J1939NM_NMQUEUE == STD_ON))
/**********************************************************************************************************************
 *  J1939Nm_InternalCopyNmQueueData()
 *********************************************************************************************************************/
static FUNC(void, J1939NM_CODE) J1939Nm_InternalCopyNmQueueData(const uint8 * dataSrc, uint8 * dataDest)
{
  dataDest[J1939NM_NM_QUEUE_DATA_POS1] = dataSrc[J1939NM_NM_QUEUE_DATA_POS1];
  dataDest[J1939NM_NM_QUEUE_DATA_POS2] = dataSrc[J1939NM_NM_QUEUE_DATA_POS2];
  dataDest[J1939NM_NM_QUEUE_DATA_POS3] = dataSrc[J1939NM_NM_QUEUE_DATA_POS3];
  dataDest[J1939NM_NM_QUEUE_DATA_POS4] = dataSrc[J1939NM_NM_QUEUE_DATA_POS4];
  dataDest[J1939NM_NM_QUEUE_DATA_POS5] = dataSrc[J1939NM_NM_QUEUE_DATA_POS5];
  dataDest[J1939NM_NM_QUEUE_DATA_POS6] = dataSrc[J1939NM_NM_QUEUE_DATA_POS6];
  dataDest[J1939NM_NM_QUEUE_DATA_POS7] = dataSrc[J1939NM_NM_QUEUE_DATA_POS7];
  dataDest[J1939NM_NM_QUEUE_DATA_POS8] = dataSrc[J1939NM_NM_QUEUE_DATA_POS8];
  dataDest[J1939NM_NM_QUEUE_DATA_POS9] = dataSrc[J1939NM_NM_QUEUE_DATA_POS9];
  dataDest[J1939NM_NM_QUEUE_DATA_POS10] = dataSrc[J1939NM_NM_QUEUE_DATA_POS10];
} /* J1939Nm_InternalCopyNmQueueData() */
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalUpdatePendingName()
 *********************************************************************************************************************/
static FUNC(void,
            J1939NM_CODE) J1939Nm_InternalUpdatePendingName(P2CONST(PduInfoType, AUTOMATIC, J1939NM_APPL_DATA) info)
{
  nmMgmt.QualifierFlags &= J1939Nm_NM_GetQf(info->SduDataPtr);

  /* #-- If the qualifier flag for one of the parts of the name is set to 0, update this part of the pending name */
  if (J1939Nm_NM_IsQfMCEnabled(info->SduDataPtr)) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
  {
    J1939Nm_NM_SetMC_LO(nmMgmt.PendingName, info->SduDataPtr);
    J1939Nm_NM_SetMC_HI(nmMgmt.PendingName, info->SduDataPtr);
  }

  if (J1939Nm_NM_IsQfEIEnabled(info->SduDataPtr)) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
  {
    J1939Nm_NM_SetEI(nmMgmt.PendingName, info->SduDataPtr);
  }

  if (J1939Nm_NM_IsQfFIEnabled(info->SduDataPtr)) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
  {
    J1939Nm_NM_SetFI(nmMgmt.PendingName, info->SduDataPtr);
  }

  if (J1939Nm_NM_IsQfFEnabled(info->SduDataPtr)) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
  {
    J1939Nm_NM_SetF(nmMgmt.PendingName, info->SduDataPtr);
  }

  if (J1939Nm_NM_IsQfVSEnabled(info->SduDataPtr)) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
  {
    J1939Nm_NM_SetVS(nmMgmt.PendingName, info->SduDataPtr);
  }

  if (J1939Nm_NM_IsQfVSIEnabled(info->SduDataPtr)) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
  {
    J1939Nm_NM_SetVSI(nmMgmt.PendingName, info->SduDataPtr);
  }

  if (J1939Nm_NM_IsQfIGEnabled(info->SduDataPtr)) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
  {
    J1939Nm_NM_SetIG(nmMgmt.PendingName, info->SduDataPtr);
  }

  if (J1939Nm_NM_IsQfAACEnabled(info->SduDataPtr)) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
  {
    J1939Nm_NM_SetAAC(nmMgmt.PendingName, info->SduDataPtr);
  }
} /* J1939Nm_InternalUpdatePendingName() */ /* PRQA S 6010 */ /* MD_MSR_STPTH */
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalSendNm()
 *********************************************************************************************************************/
static FUNC(void, J1939NM_CODE) J1939Nm_InternalSendNm(J1939Nm_ChannelChannelIdxOfRxPduType channelIdx,
                                                       J1939Nm_NodeIdxOfNodeChannelType nodeIdx, uint8 * data)
{
  Std_ReturnType result = E_NOT_OK;
  PduInfoType pdu;
  J1939Nm_TxPduNmIdxOfChannelType txPduIdx = J1939Nm_GetTxPduNmIdxOfChannel(channelIdx);
  J1939Nm_TxPduStateType *pduState = &J1939Nm_GetTxPduState(J1939Nm_GetTxPduStateIdxOfTxPdu(txPduIdx));

  /* #-- If the Tx Pdu is ready for a new transmission, enter J1939NM_EXCLUSIVE_AREA_NMACTIVE: */
  SchM_Enter_J1939Nm_TxPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if (!pduState->Locked)
  {
    pduState->Locked = TRUE;

    /* #-- Leave J1939NM_EXCLUSIVE_AREA_NMACTIVE */
    SchM_Exit_J1939Nm_TxPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    pduState->NodeId = (uint8) nodeIdx;
    pduState->ConfTimeout = J1939Nm_GetTxConfTimeout();

    pdu.SduDataPtr = data;
    pdu.SduLength = J1939NM_NM_FRAME_LENGTH + J1939Nm_GetMetaDataLengthOfTxPdu(txPduIdx);

    /* #-- Call the CanIf transmit function */
    result = CanIf_Transmit(J1939Nm_GetPduIdOfTxPdu(txPduIdx), &pdu);

    if (result != E_OK)
    {
      pduState->Locked = FALSE;
    }
  }
  else
  {
    /* #-- Leave J1939NM_EXCLUSIVE_AREA_NMACTIVE */
    SchM_Exit_J1939Nm_TxPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

# if (J1939NM_NMQUEUE == STD_ON)
  if (result == E_NOT_OK)
  {
    J1939Nm_ChannelStateType *channelState = &J1939Nm_GetChannelState(channelIdx);

    /* #-- Enter J1939NM_EXCLUSIVE_AREA_NMQUEUE */
    SchM_Enter_J1939Nm_NmQueueLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #-- If there is an free element in the acknowledgment queue: */
    if (channelState->NmQueueFree > 0)
    {
      /* #-- Get an element of the Name Management queue and set the data to transmit */
      J1939Nm_NmQueueElementType *elem = &J1939Nm_GetNmQueue(channelState->NmQueueWrite);

      elem->ChannelId = (uint8) channelIdx;
      elem->NodeId = (uint8) nodeIdx;
      J1939Nm_InternalCopyNmQueueData(data, elem->Data);

      /* #-- If the queue is full now, handle queue wrap around */
      channelState->NmQueueWrite++;

      if (channelState->NmQueueWrite == J1939Nm_GetNmQueueIndEndIdxOfChannel(channelIdx))
      {
        channelState->NmQueueWrite = J1939Nm_GetNmQueueIndStartIdxOfChannel(channelIdx);
      }

      channelState->NmQueueFree--;
    }

    /* #-- Leave J1939NM_EXCLUSIVE_AREA_NMQUEUE */
    SchM_Exit_J1939Nm_NmQueueLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
# endif
} /* J1939Nm_InternalSendNm() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalNmSendName()
 *********************************************************************************************************************/
/* *INDENT-OFF* */
/* PRQA S 3673 5 */ /* MD_J1939Nm_16.7 */
/* *INDENT-ON* */
static FUNC(void, J1939NM_CODE) J1939Nm_InternalNmSendName(J1939Nm_ChannelChannelIdxOfRxPduType channelIdx,
                                                           J1939Nm_NodeIdxOfNodeChannelType node, uint8 sourceAddress,
                                                           uint8 destAddress, uint8 * name, J1939Nm_NmModeType mode)
{
  uint8 data[J1939NM_NM_PDU_LENGTH];

  /* #-- Set the meta data */
  data[J1939NM_NM_SA_POS] = sourceAddress;
  data[J1939NM_NM_DA_POS] = destAddress;

  /* #-- Set the name to send */
  data[J1939NM_NM_BYTE1] = J1939NM_NM_SNA;
  data[J1939NM_NM_BYTE2] = J1939NM_NM_SNA;
  data[J1939NM_NM_BYTE3] =
    (uint8) mode | J1939NM_NM_RESERVED_BYTE3 | (name[J1939NM_NAME_BYTE3] & J1939NM_NAME_BYTE3_MASK_MC);
  data[J1939NM_NM_BYTE4] = name[J1939NM_NAME_BYTE4];
  data[J1939NM_NM_BYTE5] = (name[J1939NM_NAME_BYTE5] & J1939NM_NAME_BYTE5_MASK_EI) |
    (name[J1939NM_NAME_BYTE5] & J1939NM_NAME_BYTE5_MASK_FI);
  data[J1939NM_NM_BYTE6] = name[J1939NM_NAME_BYTE6];
  data[J1939NM_NM_BYTE7] = (name[J1939NM_NAME_BYTE7] & J1939NM_NAME_BYTE7_MASK_VS) | J1939NM_NM_RESERVED_BYTE7;
  data[J1939NM_NM_BYTE8] = (name[J1939NM_NAME_BYTE8] & J1939NM_NAME_BYTE8_MASK_VSI) |
    (name[J1939NM_NAME_BYTE8] & J1939NM_NAME_BYTE8_MASK_IG) | (name[J1939NM_NAME_BYTE8] & J1939NM_NAME_BYTE8_MASK_AAC);

  /* #-- Send the name */
  J1939Nm_InternalSendNm(channelIdx, node, data);
} /* J1939Nm_InternalNmSendName() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalNmSendAck()
 *********************************************************************************************************************/
static FUNC(void, J1939NM_CODE) J1939Nm_InternalNmSendAck(J1939Nm_ChannelChannelIdxOfRxPduType channelIdx,
                                                          J1939Nm_NodeIdxOfNodeChannelType nodeIdx,
                                                          uint8 sourceAddress, uint8 destAddress,
                                                          J1939Nm_NmModeType mode, J1939Nm_NmAckCodeType ackCode,
                                                          uint8 qfValue)
{
  uint8 data[J1939NM_NM_PDU_LENGTH];

  /* #-- Set the meta data */
  data[J1939NM_NM_SA_POS] = sourceAddress;
  data[J1939NM_NM_DA_POS] = destAddress;

  /* #-- Set the payload of the acknowledgment according to the acknowledgment type */
  if (mode == J1939NM_NM_MODE_SET_PENDING_NAME_ACK)
  {
    /* #-- Enter J1939NM_EXCLUSIVE_AREA_NMACTIVE */
    SchM_Enter_J1939Nm_NmActiveLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    data[J1939NM_NM_BYTE1] = J1939NM_NM_SNA;
    data[J1939NM_NM_BYTE2] = J1939NM_NM_SNA;
    data[J1939NM_NM_BYTE3] = (uint8) mode | J1939NM_NM_RESERVED_BYTE3 |
      (nmMgmt.PendingName[J1939NM_NAME_BYTE3] & J1939NM_NAME_BYTE3_MASK_MC);
    data[J1939NM_NM_BYTE4] = nmMgmt.PendingName[J1939NM_NAME_BYTE4];
    data[J1939NM_NM_BYTE5] = (nmMgmt.PendingName[J1939NM_NAME_BYTE5] & J1939NM_NAME_BYTE5_MASK_EI) |
      (nmMgmt.PendingName[J1939NM_NAME_BYTE5] & J1939NM_NAME_BYTE5_MASK_FI);
    data[J1939NM_NM_BYTE6] = nmMgmt.PendingName[J1939NM_NAME_BYTE6];
    data[J1939NM_NM_BYTE7] =
      (nmMgmt.PendingName[J1939NM_NAME_BYTE7] & J1939NM_NAME_BYTE7_MASK_VS) | J1939NM_NM_RESERVED_BYTE7;
    data[J1939NM_NM_BYTE8] =
      (nmMgmt.PendingName[J1939NM_NAME_BYTE8] & J1939NM_NAME_BYTE8_MASK_VSI) | (nmMgmt.PendingName[J1939NM_NAME_BYTE8] &
                                                                                J1939NM_NAME_BYTE8_MASK_IG) |
      (nmMgmt.PendingName[J1939NM_NAME_BYTE8] & J1939NM_NAME_BYTE8_MASK_AAC);

    /* #-- Leave J1939NM_EXCLUSIVE_AREA_NMACTIVE */
    SchM_Exit_J1939Nm_NmActiveLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  else
  {
    data[J1939NM_NM_BYTE1] = ackCode;
    data[J1939NM_NM_BYTE2] = qfValue;
    data[J1939NM_NM_BYTE3] = (uint8) mode | 0xF0u;
    data[J1939NM_NM_BYTE4] = J1939NM_NM_SNA;
    data[J1939NM_NM_BYTE5] = J1939NM_NM_SNA;
    data[J1939NM_NM_BYTE6] = J1939NM_NM_SNA;
    data[J1939NM_NM_BYTE7] = J1939NM_NM_SNA;
    data[J1939NM_NM_BYTE8] = J1939NM_NM_SNA;
  }

  /* #-- Send the acknowledgment */
  J1939Nm_InternalSendNm(channelIdx, nodeIdx, data);
} /* J1939Nm_InternalNmSendAck() */ /* PRQA S 6060 */ /* MD_MSR_STPAR */
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalNmHandleSetPendingName()
 *********************************************************************************************************************/
static FUNC(void,
            J1939NM_CODE) J1939Nm_InternalNmHandleSetPendingName(P2CONST(PduInfoType, AUTOMATIC,
                                                                         J1939NM_APPL_DATA) info,
                                                                 J1939Nm_ChannelChannelIdxOfRxPduType channelIdx,
                                                                 J1939Nm_NodeIdxOfNodeChannelType nodeIdx,
                                                                 uint8 sourceAddress, uint8 destAddress)
{
  J1939Nm_NmModeType ackMode;
  J1939Nm_NmAckCodeType ackCode;

  /* #-- Enter J1939NM_EXCLUSIVE_AREA_NMACTIVE */
  SchM_Enter_J1939Nm_NmActiveLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #-- If the Nm resquest is in process and the incoming request is for a different node: */
  if ((nmMgmt.State == J1939NM_NM_PENDING) && ((nmMgmt.NodeId != nodeIdx) || (nmMgmt.ChannelId != channelIdx)))
  {
    /* #-- Leave J1939NM_EXCLUSIVE_AREA_NMACTIVE */
    SchM_Exit_J1939Nm_NmActiveLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #-- Send a negative acknowledgment with code 5 */
    ackMode = J1939NM_NM_MODE_SET_PENDING_NAME_NACK;
    ackCode = J1939NM_NM_ACK_CODE_OTHER;
  }
  /* #-- Otherwise, if the NM request state for the node is "J1939NM_NM_IDLE" or a new request from a different node
     was received: */
  /* PRQA S 3355 2 */ /* MD_J1939Nm_13.7 */
  else if ((nmMgmt.State == J1939NM_NM_IDLE) ||
           ((nmMgmt.State == J1939NM_NM_PENDING) && (sourceAddress != nmMgmt.TesterAddr)))
  {
    uint8 *currentNodeName = J1939Nm_GetCurrentNodeName(J1939Nm_GetCurrentNodeNameIdxOfNode(nodeIdx));

    SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    J1939Nm_InternalCopyName(nmMgmt.PendingName, currentNodeName);

    SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    nmMgmt.Checksum = J1939Nm_InternalCalcNmCheckSum(nmMgmt.PendingName);

    /* #-- Check if the recieved request is for this node */
    if (nmMgmt.Checksum == J1939Nm_NM_GetChecksum(info->SduDataPtr))
    {
      J1939Nm_InternalUpdatePendingName(info);

      /* #-- Save the address of the requesting node and update the pending name */
      nmMgmt.ChannelId = (uint8) channelIdx;
      nmMgmt.NodeId = (uint8) nodeIdx;
      nmMgmt.TesterAddr = sourceAddress;
      nmMgmt.State = J1939NM_NM_PENDING;
      nmMgmt.Timer = J1939Nm_GetNameManagementTimeout();

      /* #-- Send a acknowledgment to confirm the new pending name */
      ackMode = J1939NM_NM_MODE_SET_PENDING_NAME_ACK;
      ackCode = J1939NM_NM_ACK_CODE_UNSUED;
    }
    /* #-- Otherwiese send a negative acknowledgment with code 3 */
    else
    {
      nmMgmt.State = J1939NM_NM_IDLE;

      ackMode = J1939NM_NM_MODE_SET_PENDING_NAME_NACK;
      ackCode = J1939NM_NM_ACK_CODE_INVALID_CHECKSUM;
    }

    /* #-- Leave J1939NM_EXCLUSIVE_AREA_NMACTIVE */
    SchM_Exit_J1939Nm_NmActiveLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  /* #-- Otherwise: */
  else
  {
    /* If the checksum is correct: */
    if (nmMgmt.Checksum == J1939Nm_NM_GetChecksum(info->SduDataPtr))
    {
      /* #-- Update the pending name and send a acknowledgment to confirm the new pending name */
      J1939Nm_InternalUpdatePendingName(info);

      nmMgmt.Timer = J1939Nm_GetNameManagementTimeout();

      ackMode = J1939NM_NM_MODE_SET_PENDING_NAME_ACK;
      ackCode = J1939NM_NM_ACK_CODE_UNSUED;
    }
    /* #-- Otherwise send a negative acknowledgment with code 3 */
    else
    {
      ackMode = J1939NM_NM_MODE_SET_PENDING_NAME_NACK;
      ackCode = J1939NM_NM_ACK_CODE_INVALID_CHECKSUM;

      nmMgmt.State = J1939NM_NM_IDLE;
    }

    /* #-- Leave J1939NM_EXCLUSIVE_AREA_NMACTIVE */
    SchM_Exit_J1939Nm_NmActiveLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  J1939Nm_InternalNmSendAck(channelIdx, nodeIdx, destAddress, sourceAddress, ackMode, ackCode, J1939NM_NM_SNA);
} /* J1939Nm_InternalNmHandleSetPendingName() */ /* PRQA S 6050, 6060 */ /* MD_MSR_STCAL, MD_MSR_STPAR */
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalNmHandleRequestPendingName()
 *********************************************************************************************************************/
static FUNC(void,
            J1939NM_CODE) J1939Nm_InternalNmHandleRequestPendingName(J1939Nm_ChannelChannelIdxOfRxPduType channelIdx,
                                                                     J1939Nm_NodeIdxOfNodeChannelType nodeIdx,
                                                                     uint8 sourceAddress, uint8 destAddress)
{
  if ((nmMgmt.State == J1939NM_NM_PENDING) && (nmMgmt.NodeId == nodeIdx))
  {
    uint8 name[J1939NM_NAME_SIZE];

    SchM_Enter_J1939Nm_NmActiveLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    J1939Nm_InternalCopyName(name, nmMgmt.PendingName);

    SchM_Exit_J1939Nm_NmActiveLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    J1939Nm_InternalNmSendName(channelIdx, nodeIdx, destAddress, sourceAddress, name,
                               J1939NM_NM_MODE_PENDING_NAME_RESPONSE);
  }
  else
  {
    J1939Nm_InternalNmSendAck(channelIdx, nodeIdx, destAddress, sourceAddress,
                              J1939NM_NM_MODE_SET_PENDING_NAME_NACK, J1939NM_NM_ACK_CODE_PENDING_NAME_NOT_SET,
                              J1939NM_NM_SNA);
  }
} /* J1939Nm_InternalNmHandleRequestPendingName() */
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalNmHandleRequestCurrentName()
 *********************************************************************************************************************/
static FUNC(void,
            J1939NM_CODE) J1939Nm_InternalNmHandleRequestCurrentName(J1939Nm_ChannelChannelIdxOfRxPduType channelIdx,
                                                                     J1939Nm_NodeIdxOfNodeChannelType nodeIdx,
                                                                     uint8 sourceAddress, uint8 destAddress)
{
  uint8 name[J1939NM_NAME_SIZE];

  SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  J1939Nm_InternalCopyName(name, J1939Nm_GetCurrentNodeName(J1939Nm_GetCurrentNodeNameIdxOfNode(nodeIdx)));

  SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  J1939Nm_InternalNmSendName(channelIdx, nodeIdx, destAddress, sourceAddress, name,
                             J1939NM_NM_MODE_CURRENT_NAME_RESPONSE);
} /* J1939Nm_InternalNmHandleRequestCurrentName() */
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalNmHandleAdoptPendingName()
 *********************************************************************************************************************/
static FUNC(void,
            J1939NM_CODE) J1939Nm_InternalNmHandleAdoptPendingName(J1939Nm_ChannelChannelIdxOfRxPduType channelIdx,
                                                                   J1939Nm_NodeIdxOfNodeChannelType nodeIdx,
                                                                   uint8 sourceAddress, uint8 destAddress)
{
  /* #-- Enter J1939NM_EXCLUSIVE_AREA_NMACTIVE */
  SchM_Enter_J1939Nm_NmActiveLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #-- If the Nm resquest is in process and the incoming request is for a different node: */
  if ((nmMgmt.State == J1939NM_NM_PENDING) &&
      (((nmMgmt.NodeId == nodeIdx) || (destAddress == J1939NM_BROADCAST_ADDRESS)) &&
       (nmMgmt.ChannelId == channelIdx)) && (nmMgmt.TesterAddr == sourceAddress))
  {
    uint8 *currentNodeName = J1939Nm_GetCurrentNodeName(J1939Nm_GetCurrentNodeNameIdxOfNode(nodeIdx));
    uint8 checksum;

    SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    checksum = J1939Nm_InternalCalcNmCheckSum(currentNodeName);

    SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    if (checksum == nmMgmt.Checksum)
    {
      /* #-- Set the pending name as new node name and trigger a AC message */
      (void) J1939Nm_InternalSetName(channelIdx, nodeIdx, nmMgmt.PendingName);
    }

    nmMgmt.State = J1939NM_NM_IDLE;
  }

  /* #-- Leave J1939NM_EXCLUSIVE_AREA_NMACTIVE */
  SchM_Exit_J1939Nm_NmActiveLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* J1939Nm_InternalNmHandleAdoptPendingName() */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if ((J1939NM_NM_PG_SUPPORT == STD_ON) && (J1939NM_NMQUEUE == STD_ON))
/**********************************************************************************************************************
 *  J1939Nm_InternalProcessNmQueue()
 *********************************************************************************************************************/
static FUNC(void, J1939NM_CODE) J1939Nm_InternalProcessNmQueue(J1939Nm_SizeOfChannelType channelIdx)
{
  J1939Nm_ChannelStateType *channelState = &J1939Nm_GetChannelState(channelIdx);

  /* #-- Enter J1939NM_EXCLUSIVE_AREA_NMQUEUE */
  SchM_Enter_J1939Nm_NmQueueLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  /* #-- If there is an element in the Name Management queue: */
  if (channelState->NmQueueFree < J1939Nm_GetNmQueueSizeOfChannel(channelIdx))
  {
    /* #-- Get the oldest queue element from the Name Management queue */
    J1939Nm_NmQueueElementType *elem = &J1939Nm_GetNmQueue(channelState->NmQueueRead);
    uint8 data[J1939NM_NM_PDU_LENGTH];

    uint8 channelId = elem->ChannelId;
    uint8 nodeId = elem->NodeId;
    J1939Nm_InternalCopyNmQueueData(elem->Data, data);

    channelState->NmQueueRead++;

    /* #-- If the removed element was the last queue entry, handle queue wrap around */
    if (channelState->NmQueueRead == J1939Nm_GetNmQueueIndEndIdxOfChannel(channelIdx))
    {
      channelState->NmQueueRead = J1939Nm_GetNmQueueIndStartIdxOfChannel(channelIdx);
    }

    channelState->NmQueueFree++;

    /* #-- Leave J1939NM_EXCLUSIVE_AREA_NMQUEUE */
    SchM_Exit_J1939Nm_NmQueueLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #-- Send the data from the queue */
    J1939Nm_InternalSendNm(channelId, nodeId, data);
  }
  else
  {
    /* #-- Leave J1939NM_EXCLUSIVE_AREA_NMQUEUE */
    SchM_Exit_J1939Nm_NmQueueLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
} /* J1939Nm_InternalProcessNmQueue() */
#endif

#define J1939NM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*============================================== J1939NM_STOP_SEC_CODE ==============================================*/


/**********************************************************************************************************************
 *  INTERNAL FUNCTIONS
 *********************************************************************************************************************/

/*============================================== J1939NM_START_SEC_CODE =============================================*/
#define J1939NM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) || (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON))
/**********************************************************************************************************************
 *  J1939Nm_DynamicInit
 *********************************************************************************************************************/
/*! \brief      Initialize the dynamic network management.
 *
 *  \param      none
 *  \return     none
 *  \context    Called by J1939Nm_Init.
 *********************************************************************************************************************/

FUNC(void, J1939NM_CODE) J1939Nm_DynamicInit(void)
{
# if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) || ((J1939NM_NM_PG_SUPPORT == STD_ON) && (J1939NM_NMQUEUE == STD_ON)))
  {
    J1939Nm_SizeOfChannelType channelIdx;

    for (channelIdx = 0; channelIdx < J1939Nm_GetSizeOfChannel(); channelIdx++)
    {
#  if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
      if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
      {
        J1939Nm_NodeChannelChannelIndEndIdxOfChannelType nodeChannelIndIdx;

        for (nodeChannelIndIdx = J1939Nm_GetNodeChannelChannelIndStartIdxOfChannel(channelIdx);
             nodeChannelIndIdx < J1939Nm_GetNodeChannelChannelIndEndIdxOfChannel(channelIdx); nodeChannelIndIdx++)
        {
          J1939Nm_NodeChannelChannelIndType nodeChannelIdx = J1939Nm_GetNodeChannelChannelInd(nodeChannelIndIdx);

#   if (J1939NM_ADDRESSSTORAGEBLOCK == STD_OFF)
          J1939Nm_SetCurrentNodeAddress(J1939Nm_GetCurrentNodeAddressIdxOfNodeChannel(nodeChannelIdx),
                                        J1939NM_NULL_ADDRESS);
#   endif
          J1939Nm_SetPreviousNodeAddress(J1939Nm_GetPreviousNodeAddressIdxOfNodeChannel(nodeChannelIdx),
                                         J1939NM_NULL_ADDRESS);
        }

        if (J1939Nm_GetExternalNodeChannelChannelIndStartIdxOfChannel(channelIdx) !=
            J1939NM_NO_EXTERNALNODECHANNELCHANNELINDSTARTIDXOFCHANNEL)
        {
          J1939Nm_ExternalNodeChannelChannelIndEndIdxOfChannelType extNodeChannelIndIdx;

          for (extNodeChannelIndIdx = J1939Nm_GetExternalNodeChannelChannelIndStartIdxOfChannel(channelIdx);
               extNodeChannelIndIdx < J1939Nm_GetExternalNodeChannelChannelIndEndIdxOfChannel(channelIdx);
               extNodeChannelIndIdx++)
          {
            J1939Nm_ExternalNodeChannelChannelIndType extNodeChannelIdx =
              J1939Nm_GetExternalNodeChannelChannelInd(extNodeChannelIndIdx);
            J1939Nm_ExternalNodeChannelStateIdxOfExternalNodeChannelType extNodeChannelStateIdx =
              J1939Nm_GetExternalNodeChannelStateIdxOfExternalNodeChannel(extNodeChannelIdx);
            J1939Nm_ExternalNodeChannelStateType *extNodeChannelState =
              &J1939Nm_GetExternalNodeChannelState(extNodeChannelStateIdx);
            J1939Nm_CurrentExtNodeAddressIdxOfExternalNodeChannelType currentExtNodeAddressIdx =
              J1939Nm_GetCurrentExtNodeAddressIdxOfExternalNodeChannel(extNodeChannelIdx);

            extNodeChannelState->ExtNodeState = J1939NM_EXTNODESTATE_UNKNOWN;
            extNodeChannelState->Confirmed = FALSE;

            J1939Nm_SetCurrentExtNodeAddress(currentExtNodeAddressIdx, J1939NM_NULL_ADDRESS);
          }

          J1939Nm_GetChannelState(channelIdx).NetworkState = J1939NM_NETWORKSTATE_DOWN;
        }
      }
#  endif

#  if ((J1939NM_NM_PG_SUPPORT == STD_ON) && (J1939NM_NMQUEUE == STD_ON))
      J1939Nm_GetChannelState(channelIdx).NmQueueFree = J1939Nm_GetNmQueueSizeOfChannel(channelIdx);
      J1939Nm_GetChannelState(channelIdx).NmQueueRead = (uint16) J1939Nm_GetNmQueueIndStartIdxOfChannel(channelIdx);
      J1939Nm_GetChannelState(channelIdx).NmQueueWrite = (uint16) J1939Nm_GetNmQueueIndStartIdxOfChannel(channelIdx);
#  endif
    }

#  if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) && (J1939NM_CAPDU == STD_ON))
    caMgmt.Locked = FALSE;
#  endif
  }
# endif

# if (J1939NM_NM_PG_SUPPORT == STD_ON)
  nmMgmt.State = J1939NM_NM_IDLE;
# endif

# if ((J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON) && (J1939NM_NAMESTORAGEBLOCK == STD_OFF))
  {
    J1939Nm_SizeOfNodeType nodeIdx;

    for (nodeIdx = 0; nodeIdx < J1939Nm_GetSizeOfNode(); nodeIdx++)
    {
      J1939Nm_NodeNameIdxOfNodeType nodeNameIdx = J1939Nm_GetNodeNameIdxOfNode(nodeIdx);
      uint8 *currentNodeName = J1939Nm_GetCurrentNodeName(J1939Nm_GetCurrentNodeNameIdxOfNode(nodeIdx));

      SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      J1939Nm_InternalGetName(currentNodeName, nodeNameIdx);
      SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
# endif
}
#endif


#if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) || (J1939NM_NM_PG_SUPPORT == STD_ON))
/**********************************************************************************************************************
 *  J1939Nm_DynamicMainFunction
 *********************************************************************************************************************/
/*! \brief      Handle the dynamic network management modes.
 *
 *  \param      none
 *  \return     none
 *  \context    Called by J1939Nm_MainFunction.
 *********************************************************************************************************************/

FUNC(void, J1939NM_CODE) J1939Nm_DynamicMainFunction(void)
{
# if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) || ((J1939NM_NM_PG_SUPPORT == STD_ON) && (J1939NM_NMQUEUE == STD_ON)))
  J1939Nm_SizeOfChannelType channelIdx;

  for (channelIdx = 0; channelIdx < J1939Nm_GetSizeOfChannel(); channelIdx++)
  {
#  if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
    if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
    {
      J1939Nm_ChannelStateType *channelState = &J1939Nm_GetChannelState(channelIdx);
#   if (J1939NM_NM_STATECHANGENOTIF == STD_ON)
      Nm_StateType oldState = channelState->NmState;
#   endif

      if ((channelState->NetworkState == J1939NM_NETWORKSTATE_SCANNING) ||
          (channelState->NetworkState == J1939NM_NETWORKSTATE_REFRESH))
      {
        --channelState->Timer;
        if (channelState->Timer == 0)
        {
          J1939Nm_NodeChannelChannelIndEndIdxOfChannelType nodeChannelIndIdx;

          if (channelState->NetworkState == J1939NM_NETWORKSTATE_SCANNING)
          {
            for (nodeChannelIndIdx = J1939Nm_GetNodeChannelChannelIndStartIdxOfChannel(channelIdx);
                 nodeChannelIndIdx < J1939Nm_GetNodeChannelChannelIndEndIdxOfChannel(channelIdx); nodeChannelIndIdx++)
            {
              J1939Nm_NodeChannelChannelIndType nodeChannelIdx = J1939Nm_GetNodeChannelChannelInd(nodeChannelIndIdx);
              J1939Nm_NodeIdxOfNodeChannelType nodeIdx = J1939Nm_GetNodeIdxOfNodeChannel(nodeChannelIdx);
              J1939Nm_NodeChannelStateType *nodeChannelState =
                &J1939Nm_GetNodeChannelState(J1939Nm_GetNodeChannelStateIdxOfNodeChannel(nodeChannelIdx));
              J1939Nm_NodeAddressSearchAlgorithmOfNodeType searchAlgorithm =
                J1939Nm_GetNodeAddressSearchAlgorithmOfNode(nodeIdx);

              if (((searchAlgorithm == J1939NM_REQUEST_NODEADDRESSSEARCHALGORITHMOFNODE) ||
                   (searchAlgorithm == J1939NM_ISO_GLOBAL_NODEADDRESSSEARCHALGORITHMOFNODE)) &&
                  (nodeChannelState->NodeState == J1939NM_NODESTATE_WAIT_NETWORK))
              {
                J1939Nm_CurrentNodeAddressIdxOfNodeChannelType currentAddressIdx =
                  J1939Nm_GetCurrentNodeAddressIdxOfNodeChannel(nodeChannelIdx);
                uint8 nodeAddress = J1939Nm_GetCurrentNodeAddress(currentAddressIdx);

                if (J1939Nm_IsBusAddressAllocated(channelState, nodeAddress))
                {
                  nodeAddress = J1939Nm_DynamicFindFreeAddress(nodeAddress, nodeIdx, channelState);
                }

                J1939Nm_SetCurrentNodeAddress(currentAddressIdx, nodeAddress);

#   if (J1939NM_ADDRESSSTORAGEBLOCK == STD_ON)
                (void) NvM_SetRamBlockStatus(NvMConf_NvMBlockDescriptor_J1939Nm_CurrentNodeAddresses, TRUE);
#   endif

                J1939Nm_SetBusAddressAllocated(channelState, nodeAddress);

                /* Insert random delay to avoid BusOff in case several nodes decide to claim the same unclaimed address */
                nodeChannelState->AcDelayed = TRUE;

#   if (J1939NM_NODESTARTUPDELAYOFNODE == STD_ON)
                if (J1939Nm_IsNodeStartUpDelayOfNode(nodeIdx) && (nodeAddress >= J1939NM_MIN_DELAY_ADDRESS) &&
                    (nodeAddress <= J1939NM_MAX_DELAY_ADDRESS))
                {
                  nodeChannelState->NodeState = J1939NM_NODESTATE_PRE_CLAIMING;
                }
                else
#   endif
                {
                  nodeChannelState->NodeState = J1939NM_NODESTATE_PRE_NORMAL;
#   if (J1939NM_J1939NM_E_ADDRESS_LOST == STD_ON)
                  /* \trace SPEC-2168286, SPEC-2168305 */
                  Dem_ReportErrorStatus(J1939Nm_GetJ1939NM_E_ADDRESS_LOST(), DEM_EVENT_STATUS_PREPASSED);
#   endif
                }
              }
            }
          }

#   if (J1939NM_REFRESHTIME == STD_ON)
          if (J1939Nm_GetExternalNodeChannelChannelIndStartIdxOfChannel(channelIdx) !=
              J1939NM_NO_EXTERNALNODECHANNELCHANNELINDSTARTIDXOFCHANNEL)
          {
            J1939Nm_ExternalNodeChannelChannelIndEndIdxOfChannelType extNodeChannelIndIdx;

            for (extNodeChannelIndIdx = J1939Nm_GetExternalNodeChannelChannelIndStartIdxOfChannel(channelIdx);
                 extNodeChannelIndIdx < J1939Nm_GetExternalNodeChannelChannelIndEndIdxOfChannel(channelIdx);
                 extNodeChannelIndIdx++)
            {
              J1939Nm_ExternalNodeChannelChannelIndType extNodeChannelIdx =
                J1939Nm_GetExternalNodeChannelChannelInd(extNodeChannelIndIdx);
              J1939Nm_ExternalNodeChannelStateIdxOfExternalNodeChannelType extNodeChannelStateIdx =
                J1939Nm_GetExternalNodeChannelStateIdxOfExternalNodeChannel(extNodeChannelIdx);
              J1939Nm_ExternalNodeChannelStateType *extNodeChannelState =
                &J1939Nm_GetExternalNodeChannelState(extNodeChannelStateIdx);

              if ((extNodeChannelState->ExtNodeState != J1939NM_EXTNODESTATE_UNKNOWN) &&
                  (extNodeChannelState->Confirmed == FALSE))
              {
                if (extNodeChannelState->ExtNodeState == J1939NM_EXTNODESTATE_CLAIMED)
                {
                  J1939Nm_CurrentExtNodeAddressIdxOfExternalNodeChannelType currentExtNodeAddressIdx =
                    J1939Nm_GetCurrentExtNodeAddressIdxOfExternalNodeChannel(extNodeChannelIdx);
                  J1939Nm_ExternalNodeIdxOfExternalNodeChannelType extNodeIdx =
                    J1939Nm_GetExternalNodeIdxOfExternalNodeChannel(extNodeChannelIdx);

                  J1939Nm_SetCurrentExtNodeAddress(currentExtNodeAddressIdx, J1939NM_NULL_ADDRESS);

                  CanIf_ResetAddressTableEntry(J1939Nm_GetCanIfCtrlIdOfChannel(channelIdx),
                                               J1939Nm_GetNodeAddressOfExternalNode(extNodeIdx));
                }

                extNodeChannelState->ExtNodeState = J1939NM_EXTNODESTATE_UNKNOWN;
              }
            }
          }

          channelState->Timer = J1939Nm_GetRefreshTime();
#   endif
          channelState->NetworkState = J1939NM_NETWORKSTATE_STABLE;
        }
      }

#   if (J1939NM_REFRESHTIME == STD_ON)
      else if (channelState->NetworkState == J1939NM_NETWORKSTATE_STABLE)
      {
        --channelState->Timer;
        if (channelState->Timer == 0)
        {
          if (J1939Nm_GetExternalNodeChannelChannelIndStartIdxOfChannel(channelIdx) !=
              J1939NM_NO_EXTERNALNODECHANNELCHANNELINDSTARTIDXOFCHANNEL)
          {
            J1939Nm_ExternalNodeChannelChannelIndEndIdxOfChannelType extNodeChannelIndIdx;

            for (extNodeChannelIndIdx = J1939Nm_GetExternalNodeChannelChannelIndStartIdxOfChannel(channelIdx);
                 extNodeChannelIndIdx < J1939Nm_GetExternalNodeChannelChannelIndEndIdxOfChannel(channelIdx);
                 extNodeChannelIndIdx++)
            {
              J1939Nm_ExternalNodeChannelChannelIndType extNodeChannelIdx =
                J1939Nm_GetExternalNodeChannelChannelInd(extNodeChannelIndIdx);
              J1939Nm_ExternalNodeChannelStateIdxOfExternalNodeChannelType extNodeChannelStateIdx =
                J1939Nm_GetExternalNodeChannelStateIdxOfExternalNodeChannel(extNodeChannelIdx);
              J1939Nm_ExternalNodeChannelStateType *extNodeChannelState =
                &J1939Nm_GetExternalNodeChannelState(extNodeChannelStateIdx);

              if (extNodeChannelState->ExtNodeState != J1939NM_EXTNODESTATE_UNKNOWN)
              {
                extNodeChannelState->Confirmed = FALSE;
              }
            }
          }

          /* TODO: Use J1939Nm_GetNodeStartupDelay in case of isobus channel (see ALM+ issue 79740) */
          channelState->Timer = J1939Nm_GetRequestResponseDelay();
          channelState->NetworkState = J1939NM_NETWORKSTATE_REFRESH;
          (void) J1939Rm_SendRequest(J1939RM_NULLADDRESS_USER_ID, J1939Nm_GetComMChannelIdOfChannel(channelIdx),
                                     J1939NM_PGN_AC, NULL_PTR, J1939NM_BROADCAST_ADDRESS, J1939NM_DEFAULT_PRIORITY,
                                     FALSE);
        }
      }
      else
      {
        /* MISRA */
      }
#   endif

#   if (J1939NM_NM_STATECHANGENOTIF == STD_ON)
      {
        Nm_StateType newState = channelState->NmState;

        if (oldState != newState)
        {
          /* \trace SPEC-2168305 */
          Nm_StateChangeNotification(J1939Nm_GetComMChannelIdOfChannel(channelIdx), oldState, newState);
        }
      }
#   endif
    }
#  endif

#  if ((J1939NM_NM_PG_SUPPORT == STD_ON) && (J1939NM_NMQUEUE == STD_ON))
    J1939Nm_InternalProcessNmQueue(channelIdx);
#  endif
  }
# endif

# if (J1939NM_NM_PG_SUPPORT == STD_ON)
  if (nmMgmt.State == J1939NM_NM_PENDING)
  {
    nmMgmt.Timer--;

    if (nmMgmt.Timer == 0)
    {
      nmMgmt.State = J1939NM_NM_IDLE;
    }
  }
# endif
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_DynamicFindFreeAddress
 *********************************************************************************************************************/
/*! \brief      Search for a free address within the configured range. The configured address is preferred.
 *
 *  \param      startAddress    Current address of the node.
 *  \param      nodeIdx         Index of the node which needs a new address.
 *  \param      channelState    State variable of the channel for which a new address is needed.
 *  \return                     New address, or the NULL address if no free address is found.
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/

FUNC(uint8, J1939NM_CODE) J1939Nm_DynamicFindFreeAddress(uint8 startAddress, J1939Nm_SizeOfNodeType nodeIdx,
                                                         P2CONST(J1939Nm_ChannelStateType, AUTOMATIC,
                                                                 J1939NM_APPL_DATA) channelState)
{
  uint8 preferredAddress = J1939Nm_GetNodeAddressOfNode(nodeIdx);
  uint8 nodeAddress;

  if (!J1939Nm_IsBusAddressAllocated(channelState, preferredAddress))
  {
    nodeAddress = preferredAddress;
  }
  else
  {
    uint8 firstAddress = J1939Nm_GetFirstDynamicAddressOfNode(nodeIdx);
    uint8 lastAddress = J1939Nm_GetLastDynamicAddressOfNode(nodeIdx);

    if (startAddress < firstAddress)
    {
      startAddress = firstAddress;
    }

    nodeAddress = startAddress;

    while (J1939Nm_IsBusAddressAllocated(channelState, nodeAddress))
    {
      nodeAddress++;

      if (nodeAddress > lastAddress)
      {
        nodeAddress = firstAddress;
      }

      if (nodeAddress == startAddress)
      {
        /* no free address found, go offline */
        nodeAddress = J1939NM_NULL_ADDRESS;

        break;
      }
    }
  }

  return nodeAddress;
}
#endif


#if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) && ((J1939NM_EXISTS_REMOTE_NODETYPEOFEXTERNALNODE == STD_ON) || (J1939NM_EXISTS_ANONYMOUS_NODETYPEOFEXTERNALNODE == STD_ON)))
/**********************************************************************************************************************
 *  J1939Nm_DynamicAddressClaimedNotification
 *********************************************************************************************************************/
/*! \brief      Handle the effects of a received AddressClaimed message on the dynamic network management.
 *
 *  \param      channelIdx      Internal index of the channel on which the AC was received.
 *  \param      info            PduInfoType structure containing the received AC.
 *  \param      channelState    State variable of the channel on which the AC was received.
 *  \return     none
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/

FUNC(void, J1939NM_CODE) J1939Nm_DynamicAddressClaimedNotification(J1939Nm_SizeOfChannelType channelIdx,
                                                                   P2CONST(PduInfoType, AUTOMATIC,
                                                                           J1939NM_APPL_DATA) info,
                                                                   J1939Nm_ChannelStateType * channelState)
{
  if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx) &&
      (J1939Nm_GetExternalNodeChannelChannelIndStartIdxOfChannel(channelIdx) !=
       J1939NM_NO_EXTERNALNODECHANNELCHANNELINDSTARTIDXOFCHANNEL))
  {
    J1939Nm_ExternalNodeChannelChannelIndEndIdxOfChannelType extNodeChannelIndIdx;
    J1939Nm_ExternalNodeChannelChannelIndType knownExtNodeChannelIdx = J1939NM_NO_EXTERNALNODECHANNELCHANNELIND;
    J1939Nm_ExternalNodeChannelChannelIndType conflictingExtNodeChannelIdx = J1939NM_NO_EXTERNALNODECHANNELCHANNELIND;
    J1939Nm_ExternalNodeChannelChannelIndType freeExtNodeChannelIdx = J1939NM_NO_EXTERNALNODECHANNELCHANNELIND;
    uint8 sourceAddress = info->SduDataPtr[J1939NM_AC_SA_POS];

    for (extNodeChannelIndIdx = J1939Nm_GetExternalNodeChannelChannelIndStartIdxOfChannel(channelIdx);
         extNodeChannelIndIdx < J1939Nm_GetExternalNodeChannelChannelIndEndIdxOfChannel(channelIdx);
         extNodeChannelIndIdx++)
    {
      J1939Nm_ExternalNodeChannelChannelIndType extNodeChannelIdx =
        J1939Nm_GetExternalNodeChannelChannelInd(extNodeChannelIndIdx);
      J1939Nm_ExternalNodeIdxOfExternalNodeChannelType extNodeIdx =
        J1939Nm_GetExternalNodeIdxOfExternalNodeChannel(extNodeChannelIdx);
      J1939Nm_ExternalNodeChannelStateIdxOfExternalNodeChannelType extNodeChannelStateIdx =
        J1939Nm_GetExternalNodeChannelStateIdxOfExternalNodeChannel(extNodeChannelIdx);
      J1939Nm_ExternalNodeChannelStateType *extNodeChannelState =
        &J1939Nm_GetExternalNodeChannelState(extNodeChannelStateIdx);

      if (extNodeChannelState->ExtNodeState != J1939NM_EXTNODESTATE_UNKNOWN)
      {
        sint16_least cmp = -1;

        if (knownExtNodeChannelIdx == J1939NM_NO_EXTERNALNODECHANNELCHANNELIND)
        {
          uint8 *currentNodeName =
            J1939Nm_GetCurrentExtNodeName(J1939Nm_GetCurrentExtNodeNameIdxOfExternalNode(extNodeIdx));

          SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          cmp = J1939Nm_InternalNameCompare(currentNodeName, &(info->SduDataPtr[J1939NM_AC_NAME_BYTE1]));
          SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

          if (cmp == 0)
          {
            knownExtNodeChannelIdx = extNodeChannelIdx;
          }
        }

        if ((sourceAddress != J1939NM_NULL_ADDRESS) &&
            (conflictingExtNodeChannelIdx == J1939NM_NO_EXTERNALNODECHANNELCHANNELIND))
        {
          if (cmp != 0)
          {
            J1939Nm_CurrentExtNodeAddressIdxOfExternalNodeChannelType currentExtNodeAddressIdx =
              J1939Nm_GetCurrentExtNodeAddressIdxOfExternalNodeChannel(extNodeChannelIdx);
            uint8 currentAddress = J1939Nm_GetCurrentExtNodeAddress(currentExtNodeAddressIdx);

            if (sourceAddress == currentAddress)
            {
              conflictingExtNodeChannelIdx = extNodeChannelIdx;
            }
          }
        }
      }
      else
      {
        if (freeExtNodeChannelIdx == J1939NM_NO_EXTERNALNODECHANNELCHANNELIND)
        {
# if (J1939NM_EXISTS_REMOTE_NODETYPEOFEXTERNALNODE == STD_ON)
          if (J1939Nm_GetNodeTypeOfExternalNode(extNodeIdx) == J1939NM_REMOTE_NODETYPEOFEXTERNALNODE)
          {
            J1939Nm_NodeNameIdxOfExternalNodeType nodeNameIdx = J1939Nm_GetNodeNameIdxOfExternalNode(extNodeIdx);
            J1939Nm_ExternalNodeNameFilterIdxOfExternalNodeType extNodeNameFilterIdx =
              J1939Nm_GetExternalNodeNameFilterIdxOfExternalNode(extNodeIdx);
            uint8 name[J1939NM_NAME_LENGTH], filter[J1939NM_NAME_LENGTH];

            J1939Nm_InternalGetName(name, nodeNameIdx);
            J1939Nm_InternalGetNameFilter(filter, extNodeNameFilterIdx);

            /* PRQA S 3344 1 */ /* MD_J1939Nm_13.2 */
            if (J1939Nm_InternalFilteredNameCompare(name, &(info->SduDataPtr[J1939NM_AC_NAME_BYTE1]), filter))
            {
              freeExtNodeChannelIdx = extNodeChannelIdx;
            }
          }
# endif
# if ((J1939NM_EXISTS_REMOTE_NODETYPEOFEXTERNALNODE == STD_ON) && (J1939NM_EXISTS_ANONYMOUS_NODETYPEOFEXTERNALNODE == STD_ON))
          else
# endif
# if (J1939NM_EXISTS_ANONYMOUS_NODETYPEOFEXTERNALNODE == STD_ON)
          {
            freeExtNodeChannelIdx = extNodeChannelIdx;
          }
# endif
        }
      }
    }

    if (conflictingExtNodeChannelIdx != J1939NM_NO_EXTERNALNODECHANNELCHANNELIND)
    {
      J1939Nm_ExternalNodeIdxOfExternalNodeChannelType extNodeIdx =
        J1939Nm_GetExternalNodeIdxOfExternalNodeChannel(conflictingExtNodeChannelIdx);
      uint8 *currentNodeName =
        J1939Nm_GetCurrentExtNodeName(J1939Nm_GetCurrentExtNodeNameIdxOfExternalNode(extNodeIdx));
      sint16_least cmp;

      SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      cmp = J1939Nm_InternalNameCompare(currentNodeName, &(info->SduDataPtr[J1939NM_AC_NAME_BYTE1]));
      SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      if (cmp > 0)
      {
        J1939Nm_CurrentExtNodeAddressIdxOfExternalNodeChannelType currentExtNodeAddressIdx =
          J1939Nm_GetCurrentExtNodeAddressIdxOfExternalNodeChannel(conflictingExtNodeChannelIdx);
        J1939Nm_ExternalNodeChannelStateIdxOfExternalNodeChannelType extNodeChannelStateIdx =
          J1939Nm_GetExternalNodeChannelStateIdxOfExternalNodeChannel(conflictingExtNodeChannelIdx);
        J1939Nm_ExternalNodeChannelStateType *extNodeChannelState =
          &J1939Nm_GetExternalNodeChannelState(extNodeChannelStateIdx);
        J1939Nm_NodeAddressOfExternalNodeType nodeAddress = J1939Nm_GetNodeAddressOfExternalNode(extNodeIdx);

        J1939Nm_SetCurrentExtNodeAddress(currentExtNodeAddressIdx, J1939NM_NULL_ADDRESS);

        CanIf_ResetAddressTableEntry(J1939Nm_GetCanIfCtrlIdOfChannel(channelIdx), nodeAddress);

        extNodeChannelState->ExtNodeState = J1939NM_EXTNODESTATE_CANNOT_CLAIM;
      }
      else
      {
        if (cmp < 0)
        {
          /* the node of the received AC lost against a known node, and shall therefore not reserve its address */
          sourceAddress = J1939NM_NULL_ADDRESS;
        }
      }
    }

    if (knownExtNodeChannelIdx != J1939NM_NO_EXTERNALNODECHANNELCHANNELIND)
    {
      J1939Nm_CurrentExtNodeAddressIdxOfExternalNodeChannelType currentExtNodeAddressIdx =
        J1939Nm_GetCurrentExtNodeAddressIdxOfExternalNodeChannel(knownExtNodeChannelIdx);
      J1939Nm_ExternalNodeChannelStateIdxOfExternalNodeChannelType extNodeChannelStateIdx =
        J1939Nm_GetExternalNodeChannelStateIdxOfExternalNodeChannel(knownExtNodeChannelIdx);
      J1939Nm_ExternalNodeChannelStateType *extNodeChannelState =
        &J1939Nm_GetExternalNodeChannelState(extNodeChannelStateIdx);
      uint8 currentAddress = J1939Nm_GetCurrentExtNodeAddress(currentExtNodeAddressIdx);

      if (sourceAddress != currentAddress)
      {
        J1939Nm_ExternalNodeIdxOfExternalNodeChannelType extNodeIdx =
          J1939Nm_GetExternalNodeIdxOfExternalNodeChannel(knownExtNodeChannelIdx);
        J1939Nm_NodeAddressOfExternalNodeType nodeAddress = J1939Nm_GetNodeAddressOfExternalNode(extNodeIdx);

        if (currentAddress != J1939NM_NULL_ADDRESS)
        {
          J1939Nm_ClearBusAddressAllocated(channelState, currentAddress);
        }

        /* J1939Nm_SetBusAddressAllocated has already been done by the calling function */

        J1939Nm_SetCurrentExtNodeAddress(currentExtNodeAddressIdx, sourceAddress);

        if (sourceAddress == J1939NM_NULL_ADDRESS)
        {
          CanIf_ResetAddressTableEntry(J1939Nm_GetCanIfCtrlIdOfChannel(channelIdx), nodeAddress);

          extNodeChannelState->ExtNodeState = J1939NM_EXTNODESTATE_CANNOT_CLAIM;
        }
        else
        {
          CanIf_SetAddressTableEntry(J1939Nm_GetCanIfCtrlIdOfChannel(channelIdx), nodeAddress, sourceAddress);

          extNodeChannelState->ExtNodeState = J1939NM_EXTNODESTATE_CLAIMED;
        }
      }

# if (J1939NM_REFRESHTIME == STD_ON)
      extNodeChannelState->Confirmed = TRUE;
# endif
    }
    else
    {
      if (freeExtNodeChannelIdx != J1939NM_NO_EXTERNALNODECHANNELCHANNELIND)
      {
        J1939Nm_CurrentExtNodeAddressIdxOfExternalNodeChannelType currentExtNodeAddressIdx =
          J1939Nm_GetCurrentExtNodeAddressIdxOfExternalNodeChannel(freeExtNodeChannelIdx);
        J1939Nm_ExternalNodeIdxOfExternalNodeChannelType extNodeIdx =
          J1939Nm_GetExternalNodeIdxOfExternalNodeChannel(freeExtNodeChannelIdx);
        uint8 *currentNodeName =
          J1939Nm_GetCurrentExtNodeName(J1939Nm_GetCurrentExtNodeNameIdxOfExternalNode(extNodeIdx));
        J1939Nm_ExternalNodeChannelStateIdxOfExternalNodeChannelType extNodeChannelStateIdx =
          J1939Nm_GetExternalNodeChannelStateIdxOfExternalNodeChannel(freeExtNodeChannelIdx);
        J1939Nm_ExternalNodeChannelStateType *extNodeChannelState =
          &J1939Nm_GetExternalNodeChannelState(extNodeChannelStateIdx);

        SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        J1939Nm_InternalCopyName(currentNodeName, &(info->SduDataPtr[J1939NM_AC_NAME_BYTE1]));
        SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        /* J1939Nm_SetBusAddressAllocated has already been done by the calling function */

        J1939Nm_SetCurrentExtNodeAddress(currentExtNodeAddressIdx, sourceAddress);

        if (sourceAddress == J1939NM_NULL_ADDRESS)
        {
          extNodeChannelState->ExtNodeState = J1939NM_EXTNODESTATE_CANNOT_CLAIM;
        }
        else
        {
          CanIf_SetAddressTableEntry(J1939Nm_GetCanIfCtrlIdOfChannel(channelIdx),
                                     J1939Nm_GetNodeAddressOfExternalNode(extNodeIdx), sourceAddress);

          extNodeChannelState->ExtNodeState = J1939NM_EXTNODESTATE_CLAIMED;
        }

# if (J1939NM_REFRESHTIME == STD_ON)
        extNodeChannelState->Confirmed = TRUE;
# endif
      }
    }
  }
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif


#if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_DynamicNetworkRequest
 *********************************************************************************************************************/
/*! \brief      Handle the effects of a network request on the dynamic network state of a node.
 *
 *  \param      nodeIdx         Index of the node which received the network request.
 *  \param      nodeChannelIdx  Index of the internal node channel that corresponds to nodeIdx and channelState.
 *  \param      channelState    State variable of the channel which received the network request.
 *  \param      nodeAddress     Configured address of the node.
 *  \return                     Node address. NULL address if no free address could be found.
 *  \context    Called by J1939Nm_NetworkRequest.
 *********************************************************************************************************************/

FUNC(uint8, J1939NM_CODE) J1939Nm_DynamicNetworkRequest(J1939Nm_SizeOfNodeType nodeIdx,
                                                        J1939Nm_SizeOfNodeChannelType nodeChannelIdx,
                                                        const J1939Nm_ChannelStateType * channelState,
                                                        uint8 nodeAddress)
{
  J1939Nm_CurrentNodeAddressIdxOfNodeChannelType currentAddressIdx =
    J1939Nm_GetCurrentNodeAddressIdxOfNodeChannel(nodeChannelIdx);
  uint8 currentNodeAddress = J1939Nm_GetCurrentNodeAddress(currentAddressIdx);

  if (currentNodeAddress != J1939NM_NULL_ADDRESS)
  {
    nodeAddress = currentNodeAddress;
  }
  else
  {
    J1939Nm_NodeAddressConfigurationCapabilityOfNodeType nodeAcc =
      J1939Nm_GetNodeAddressConfigurationCapabilityOfNode(nodeIdx);

    if ((nodeAcc == J1939NM_AAC_NODEADDRESSCONFIGURATIONCAPABILITYOFNODE) &&
        (J1939Nm_IsBusAddressAllocated(channelState, nodeAddress)))
    {
      nodeAddress = J1939Nm_DynamicFindFreeAddress(nodeAddress, nodeIdx, channelState);
    }

    J1939Nm_SetCurrentNodeAddress(currentAddressIdx, nodeAddress);

# if (J1939NM_ADDRESSSTORAGEBLOCK == STD_ON)
    (void) NvM_SetRamBlockStatus(NvMConf_NvMBlockDescriptor_J1939Nm_CurrentNodeAddresses, TRUE);
# endif
  }

  return nodeAddress;
}
#endif


#if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) && ((J1939NM_EXISTS_REMOTE_NODETYPEOFEXTERNALNODE == STD_ON) || (J1939NM_EXISTS_ANONYMOUS_NODETYPEOFEXTERNALNODE == STD_ON)))
/**********************************************************************************************************************
 *  J1939Nm_DynamicNetworkRelease
 *********************************************************************************************************************/
/*! \brief      Handle the effects of a network release on the dynamic network state of a channel.
 *
 *  \param      channelIdx      Internal index of the channel which received the network release.
 *  \return     none
 *  \context    Called by J1939Nm_NetworkRelease.
 *********************************************************************************************************************/

FUNC(void, J1939NM_CODE) J1939Nm_DynamicNetworkRelease(J1939Nm_SizeOfChannelType channelIdx)
{
  if (J1939Nm_GetExternalNodeChannelChannelIndStartIdxOfChannel(channelIdx) !=
      J1939NM_NO_EXTERNALNODECHANNELCHANNELINDSTARTIDXOFCHANNEL)
  {
    J1939Nm_ExternalNodeChannelChannelIndStartIdxOfChannelType extNodeChannelIndIdx;

    for (extNodeChannelIndIdx = J1939Nm_GetExternalNodeChannelChannelIndStartIdxOfChannel(channelIdx);
         extNodeChannelIndIdx < J1939Nm_GetExternalNodeChannelChannelIndEndIdxOfChannel(channelIdx);
         extNodeChannelIndIdx++)
    {
      J1939Nm_ExternalNodeChannelChannelIndType extNodeChannelIdx =
        J1939Nm_GetExternalNodeChannelChannelInd(extNodeChannelIndIdx);
      J1939Nm_ExternalNodeChannelStateIdxOfExternalNodeChannelType extNodeChannelStateIdx =
        J1939Nm_GetExternalNodeChannelStateIdxOfExternalNodeChannel(extNodeChannelIdx);
      J1939Nm_ExternalNodeChannelStateType *extNodeChannelState =
        &J1939Nm_GetExternalNodeChannelState(extNodeChannelStateIdx);

      if (extNodeChannelState->ExtNodeState == J1939NM_EXTNODESTATE_CLAIMED)
      {
        J1939Nm_ExternalNodeIdxOfExternalNodeChannelType extNodeIdx =
          J1939Nm_GetExternalNodeIdxOfExternalNodeChannel(extNodeChannelIdx);

        CanIf_ResetAddressTableEntry(J1939Nm_GetCanIfCtrlIdOfChannel(channelIdx),
                                     J1939Nm_GetNodeAddressOfExternalNode(extNodeIdx));
      }
      extNodeChannelState->ExtNodeState = J1939NM_EXTNODESTATE_UNKNOWN;
    }
  }
}
#endif


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (J1939NM_ADDRESSSTORAGEBLOCK == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_NvMInit_CurrentNodeAddresses
 *********************************************************************************************************************/
FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_NvMInit_CurrentNodeAddresses(void)
{
# if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
  J1939Nm_SizeOfCurrentNodeAddressType currentNodeAddressIdx;

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168258 */
  J1939Nm_AssertRetVal(J1939Nm_ModuleInitialized == J1939NM_UNINIT, J1939NM_SID_NVMINIT_CURRENTNODEADDRESSES,
                       J1939NM_E_RUNNING);
  /* PRQA L:PC */

  for (currentNodeAddressIdx = 0; currentNodeAddressIdx < J1939Nm_GetSizeOfCurrentNodeAddress();
       currentNodeAddressIdx++)
  {
    J1939Nm_SetCurrentNodeAddress(currentNodeAddressIdx, J1939NM_NULL_ADDRESS);
  }
# endif

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif


#if (J1939NM_NAMESTORAGEBLOCK == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_NvMInit_CurrentNodeNames
 *********************************************************************************************************************/
FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_NvMInit_CurrentNodeNames(void)
{
# if (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON)
  J1939Nm_SizeOfNodeType nodeIdx;

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168258 */
  J1939Nm_AssertRetVal(J1939Nm_ModuleInitialized == J1939NM_UNINIT, J1939NM_SID_NVMINIT_CURRENTNODENAMES,
                       J1939NM_E_RUNNING);
  /* PRQA L:PC */

  for (nodeIdx = 0; nodeIdx < J1939Nm_GetSizeOfNode(); nodeIdx++)
  {
    J1939Nm_NodeNameIdxOfNodeType nodeNameIdx = J1939Nm_GetNodeNameIdxOfNode(nodeIdx);
    uint8 *currentNodeName = J1939Nm_GetCurrentNodeName(J1939Nm_GetCurrentNodeNameIdxOfNode(nodeIdx));

    SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    J1939Nm_InternalGetName(currentNodeName, nodeNameIdx);
    SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
# endif

  return E_OK;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

#if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) && (J1939NM_CAPDU == STD_ON))
/**********************************************************************************************************************
 *  J1939Nm_StartOfReception
 *********************************************************************************************************************/

/* *INDENT-OFF* */
/* PRQA S 3673 6 */ /* MD_J1939Nm_16.7 */
/* *INDENT-ON* */
FUNC(BufReq_ReturnType, J1939NM_CODE) J1939Nm_StartOfReception(PduIdType CaPduId,
                                                               P2VAR(PduInfoType, AUTOMATIC, J1939NM_APPL_VAR) info,
                                                               PduLengthType TpSduLength,
                                                               P2VAR(PduLengthType, AUTOMATIC,
                                                                     J1939NM_APPL_VAR) bufferSizePtr)
{
  BufReq_ReturnType tpResult = BUFREQ_E_NOT_OK;

# if (J1939NM_USE_DUMMY_STATEMENT == STD_ON)
  /* PRQA S 3112 1 */ /* MD_J1939Nm_14.2 */
  (void) info;
# endif

  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_STARTOFRECEPTION, J1939NM_E_UNINIT);
    return BUFREQ_E_NOT_OK;
  }

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168285, SPEC-2168287 */
  J1939Nm_AssertRetBrtVal(CaPduId < J1939Nm_GetSizeOfCaPdu(), J1939NM_SID_STARTOFRECEPTION,
                          J1939NM_E_INVALID_PDU_SDU_ID);
# if (J1939NM_INVALIDHNDOFCAPDU == STD_ON)
  J1939Nm_AssertRetBrtVal(!J1939Nm_IsInvalidHndOfCaPdu(CaPduId), J1939NM_SID_STARTOFRECEPTION,
                          J1939NM_E_INVALID_PDU_SDU_ID);
# endif
  /* \trace SPEC-2168285 */
  J1939Nm_AssertRetBrtVal(info != NULL_PTR, J1939NM_SID_STARTOFRECEPTION, J1939NM_E_PARAM_POINTER);
  J1939Nm_AssertRetBrtVal(info->SduDataPtr != NULL_PTR, J1939NM_SID_STARTOFRECEPTION, J1939NM_E_PARAM_POINTER);
  J1939Nm_AssertRetBrtVal(bufferSizePtr != NULL_PTR, J1939NM_SID_STARTOFRECEPTION, J1939NM_E_PARAM_POINTER);
  /* PRQA L:PC */

  if (TpSduLength == J1939NM_CA_FRAME_LENGTH)
  {
    if (J1939Nm_IsUsesDynamicAddressingOfChannel(J1939Nm_GetChannelChannelIdxOfCaPdu(CaPduId)))
    {
      SchM_Enter_J1939Nm_RxPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      if (!caMgmt.Locked)
      {
        caMgmt.Locked = TRUE;
        SchM_Exit_J1939Nm_RxPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        caMgmt.PduId = CaPduId;
        caMgmt.Position = 0;

        *bufferSizePtr = J1939NM_CA_FRAME_LENGTH;

        tpResult = BUFREQ_OK;
      }
      else
      {
        SchM_Exit_J1939Nm_RxPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
    }
  }
  else
  {
    J1939Nm_ReportDetError(J1939NM_SID_STARTOFRECEPTION, J1939NM_E_INVALID_PDU_SIZE);
  }

  return tpResult;
} /* PRQA S 2006, 6010 */ /* MD_MSR_14.7, MD_MSR_STPTH */


/**********************************************************************************************************************
 *  J1939Nm_CopyRxData
 *********************************************************************************************************************/

/* *INDENT-OFF* */
/* PRQA S 3673 5 */ /* MD_J1939Nm_16.7 */
/* *INDENT-ON* */
FUNC(BufReq_ReturnType, J1939NM_CODE) J1939Nm_CopyRxData(PduIdType CaPduId,
                                                         P2VAR(PduInfoType, AUTOMATIC, J1939NM_APPL_VAR) info,
                                                         P2VAR(PduLengthType, AUTOMATIC,
                                                               J1939NM_APPL_VAR) availableDataPtr)
{
  BufReq_ReturnType tpResult = BUFREQ_E_NOT_OK;

  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_COPYRXDATA, J1939NM_E_UNINIT);
    return BUFREQ_E_NOT_OK;
  }

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168285, SPEC-2168287 */
  J1939Nm_AssertRetBrtVal(CaPduId < J1939Nm_GetSizeOfCaPdu(), J1939NM_SID_COPYRXDATA, J1939NM_E_INVALID_PDU_SDU_ID);
# if (J1939NM_INVALIDHNDOFCAPDU == STD_ON)
  J1939Nm_AssertRetBrtVal(!J1939Nm_IsInvalidHndOfCaPdu(CaPduId), J1939NM_SID_COPYRXDATA, J1939NM_E_INVALID_PDU_SDU_ID);
# endif
  /* \trace SPEC-2168285 */
  J1939Nm_AssertRetBrtVal(info != NULL_PTR, J1939NM_SID_COPYRXDATA, J1939NM_E_PARAM_POINTER);
  J1939Nm_AssertRetBrtVal(info->SduLength <= (J1939NM_CA_FRAME_LENGTH - caMgmt.Position), J1939NM_SID_COPYRXDATA,
                          J1939NM_E_INVALID_PDU_SIZE);
  J1939Nm_AssertRetBrtVal(info->SduDataPtr != NULL_PTR, J1939NM_SID_COPYRXDATA, J1939NM_E_PARAM_POINTER);
  J1939Nm_AssertRetBrtVal(availableDataPtr != NULL_PTR, J1939NM_SID_COPYRXDATA, J1939NM_E_PARAM_POINTER);
  /* PRQA L:PC */

  if ((caMgmt.Locked) && (caMgmt.PduId == CaPduId))
  {
    uint8 i;

    for (i = 0; i < info->SduLength; i++)
    {
      caMgmt.Buffer[caMgmt.Position] = info->SduDataPtr[i];
      caMgmt.Position++;
    }

    *availableDataPtr = J1939NM_CA_FRAME_LENGTH - caMgmt.Position;

    tpResult = BUFREQ_OK;
  }

  return tpResult;
} /* PRQA S 2006, 6010 */ /* MD_MSR_14.7, MD_MSR_STPTH */


/**********************************************************************************************************************
 *  J1939Nm_TpRxIndication
 *********************************************************************************************************************/

FUNC(void, J1939NM_CODE) J1939Nm_TpRxIndication(PduIdType CaPduId, Std_ReturnType result)
{
  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_TPRXINDICATION, J1939NM_E_UNINIT);
    return;
  }

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168285, SPEC-2168287 */
  J1939Nm_AssertRet(CaPduId < J1939Nm_GetSizeOfCaPdu(), J1939NM_SID_TPRXINDICATION, J1939NM_E_INVALID_PDU_SDU_ID);
# if (J1939NM_INVALIDHNDOFCAPDU == STD_ON)
  J1939Nm_AssertRet(!J1939Nm_IsInvalidHndOfCaPdu(CaPduId), J1939NM_SID_TPRXINDICATION, J1939NM_E_INVALID_PDU_SDU_ID);
# endif
  /* PRQA L:PC */

  if ((caMgmt.Locked) && (caMgmt.PduId == CaPduId))
  {
    if ((result == E_OK) && (caMgmt.Position == J1939NM_CA_FRAME_LENGTH))
    {
      J1939Nm_ChannelChannelIdxOfCaPduType channelIdx = J1939Nm_GetChannelChannelIdxOfCaPdu(CaPduId);
# if (J1939NM_NM_STATECHANGENOTIF == STD_ON)
      J1939Nm_ChannelStateType *channelState = &J1939Nm_GetChannelState(channelIdx);
      Nm_StateType oldState = channelState->NmState;
# endif

      {
        J1939Nm_NodeChannelChannelIndEndIdxOfChannelType nodeChannelIndIdx;

        for (nodeChannelIndIdx = J1939Nm_GetNodeChannelChannelIndStartIdxOfChannel(channelIdx);
             nodeChannelIndIdx < J1939Nm_GetNodeChannelChannelIndEndIdxOfChannel(channelIdx); nodeChannelIndIdx++)
        {
          J1939Nm_NodeChannelChannelIndType nodeChannelIdx = J1939Nm_GetNodeChannelChannelInd(nodeChannelIndIdx);
          J1939Nm_NodeIdxOfNodeChannelType nodeIdx = J1939Nm_GetNodeIdxOfNodeChannel(nodeChannelIdx);
# if (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON)
          uint8 *currentNodeName = J1939Nm_GetCurrentNodeName(J1939Nm_GetCurrentNodeNameIdxOfNode(nodeIdx));
# else
          J1939Nm_NodeNameIdxOfNodeType nodeNameIdx = J1939Nm_GetNodeNameIdxOfNode(nodeIdx);
          uint8 currentNodeName[J1939NM_NAME_LENGTH];
# endif
          sint16_least cmp;

# if (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON)
          SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
# else
          J1939Nm_InternalGetName(currentNodeName, nodeNameIdx);
# endif

          cmp = J1939Nm_InternalNameCompare(currentNodeName, &(caMgmt.Buffer[J1939NM_CA_NAME_BYTE1]));

# if (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON)
          SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
# endif

          if (cmp == 0)
          {
            uint8 nodeAddress = caMgmt.Buffer[J1939NM_CA_ADDRESS];

            if (nodeAddress < J1939NM_NULL_ADDRESS)
            {
              (void) J1939Nm_InternalSetAddress(nodeIdx, nodeChannelIdx, channelIdx, nodeAddress);
            }
            else
            {
              /* \trace SPEC-2168285 */
              J1939Nm_ReportDetError(J1939NM_SID_TPRXINDICATION, J1939NM_E_INVALID_ADDRESS);
            }

            break;
          }
        }
      }

# if (J1939NM_NM_STATECHANGENOTIF == STD_ON)
      {
        Nm_StateType newState = channelState->NmState;

        if (oldState != newState)
        {
          /* \trace SPEC-2168305 */
          Nm_StateChangeNotification(J1939Nm_GetComMChannelIdOfChannel(channelIdx), oldState, newState);
        }
      }
# endif
    }

    caMgmt.Locked = FALSE;
  }
} /* PRQA S 2006, 6080 */ /* MD_MSR_14.7, MD_MSR_STMIF */
#endif


#if (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_GetNode
 *********************************************************************************************************************/

FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_GetNode(NetworkHandleType channel, uint16 nodeId, boolean external,
                                                   P2VAR(uint8, AUTOMATIC, J1939NM_APPL_VAR) name,
                                                   P2VAR(uint8, AUTOMATIC, J1939NM_APPL_VAR) virtAddr,
                                                   P2VAR(uint8, AUTOMATIC, J1939NM_APPL_VAR) busAddr)
{
  Std_ReturnType result = E_NOT_OK;

  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_GETNODE, J1939NM_E_UNINIT);
    return E_NOT_OK;
  }

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168285, SPEC-2168288 */
  J1939Nm_AssertRetVal(J1939Nm_Channel_Valid(channel), J1939NM_SID_GETNODE, J1939NM_E_INVALID_NETWORK_ID);
# if (J1939NM_DEV_ERROR_DETECT == STD_ON)
  if (external) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
  {
#  if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
    /* \trace SPEC-2168285 */
    J1939Nm_AssertRetVal(nodeId < J1939Nm_GetSizeOfExternalNode(), J1939NM_SID_GETNODE, J1939NM_E_INVALID_NODE);
#   if (J1939NM_INVALIDHNDOFEXTERNALNODE == STD_ON)
    J1939Nm_AssertRetVal(!J1939Nm_IsInvalidHndOfExternalNode(nodeId), J1939NM_SID_GETNODE, J1939NM_E_INVALID_NODE);
#   endif
#  endif
  }
  else
  {
    /* \trace SPEC-2168285 */
    J1939Nm_AssertRetVal(nodeId < J1939Nm_GetSizeOfNode(), J1939NM_SID_GETNODE, J1939NM_E_INVALID_NODE);
  }
# endif
  /* \trace SPEC-2168285 */
  J1939Nm_AssertRetVal(virtAddr != NULL_PTR, J1939NM_SID_GETNODE, J1939NM_E_PARAM_POINTER);
  J1939Nm_AssertRetVal(busAddr != NULL_PTR, J1939NM_SID_GETNODE, J1939NM_E_PARAM_POINTER);
  /* PRQA L:PC */

  if (external) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
  {
# if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
    J1939Nm_ExternalNodeChannelNodeIndEndIdxOfExternalNodeType extNodeChannelIndIdx;

    for (extNodeChannelIndIdx = J1939Nm_GetExternalNodeChannelNodeIndStartIdxOfExternalNode(nodeId);
         extNodeChannelIndIdx < J1939Nm_GetExternalNodeChannelNodeIndEndIdxOfExternalNode(nodeId);
         extNodeChannelIndIdx++)
    {
      J1939Nm_ExternalNodeChannelNodeIndType extNodeChannelIdx =
        J1939Nm_GetExternalNodeChannelNodeInd(extNodeChannelIndIdx);
      J1939Nm_ExternalNodeChannelStateIdxOfExternalNodeChannelType extNodeChannelStateIdx =
        J1939Nm_GetExternalNodeChannelStateIdxOfExternalNodeChannel(extNodeChannelIdx);
      J1939Nm_ExternalNodeChannelStateType *extNodeChannelState =
        &J1939Nm_GetExternalNodeChannelState(extNodeChannelStateIdx);

      if (extNodeChannelState->ExtNodeState != J1939NM_EXTNODESTATE_UNKNOWN)
      {
        J1939Nm_ChannelIdxOfNodeChannelType channelIdx = J1939Nm_GetChannelIdxOfNodeChannel(extNodeChannelIdx);

        if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx) &&
            (channel == J1939Nm_GetComMChannelIdOfChannel(channelIdx)))
        {
          J1939Nm_CurrentExtNodeAddressIdxOfExternalNodeChannelType currentExtNodeAddressIdx =
            J1939Nm_GetCurrentExtNodeAddressIdxOfExternalNodeChannel(extNodeChannelIdx);

          *virtAddr = J1939Nm_GetNodeAddressOfExternalNode(nodeId);
          *busAddr = J1939Nm_GetCurrentExtNodeAddress(currentExtNodeAddressIdx);

          if (name != NULL_PTR)
          {
            uint8 *currentNodeName =
              J1939Nm_GetCurrentExtNodeName(J1939Nm_GetCurrentExtNodeNameIdxOfExternalNode(nodeId));

            SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            J1939Nm_InternalCopyName(name, currentNodeName);
            SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          }

          result = E_OK;
        }
      }
    }
# endif
  }
  else
  {
    J1939Nm_NodeChannelNodeIndEndIdxOfNodeType nodeChannelIndIdx;

    for (nodeChannelIndIdx = J1939Nm_GetNodeChannelNodeIndStartIdxOfNode(nodeId);
         nodeChannelIndIdx < J1939Nm_GetNodeChannelNodeIndEndIdxOfNode(nodeId); nodeChannelIndIdx++)
    {
      J1939Nm_NodeChannelNodeIndType nodeChannelIdx = J1939Nm_GetNodeChannelNodeInd(nodeChannelIndIdx);
      J1939Nm_ChannelIdxOfNodeChannelType channelIdx = J1939Nm_GetChannelIdxOfNodeChannel(nodeChannelIdx);

      if (channel == J1939Nm_GetComMChannelIdOfChannel(channelIdx))
      {
        *virtAddr = J1939Nm_GetNodeAddressOfNode(nodeId);

# if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
        if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
        {
          *busAddr = J1939Nm_GetCurrentNodeAddress(J1939Nm_GetCurrentNodeAddressIdxOfNodeChannel(nodeChannelIdx));
        }
        else
# endif
        {
          *busAddr = *virtAddr;
        }

        if (name != NULL_PTR)
        {
          uint8 *currentNodeName = J1939Nm_GetCurrentNodeName(J1939Nm_GetCurrentNodeNameIdxOfNode(nodeId));

          SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
          J1939Nm_InternalCopyName(name, currentNodeName);
          SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        }

        result = E_OK;
      }
    }
  }

  return result;
} /* PRQA S 2006, 6010, 6030, 6060, 6080 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */


/**********************************************************************************************************************
 *  J1939Nm_GetFirstUnknownNameIdx
 *********************************************************************************************************************/

FUNC(uint16, J1939NM_CODE) J1939Nm_GetFirstUnknownNameIdx(NetworkHandleType channel)
{
  uint16 nodeIdx = J1939NM_INVALID_NODE;

  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_GETFIRSTUNKNOWNNAMEIDX, J1939NM_E_UNINIT);
    return nodeIdx;
  }

  if (J1939Nm_Channel_Valid(channel))
  {
    nodeIdx = J1939Nm_GetFirstAnonymousNodeIdOfChannel(J1939Nm_GetChannelIdxOfComMChannel(channel));
  }
# if (J1939NM_DEV_ERROR_DETECT == STD_ON)
  else
  {
    /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
    /* \trace SPEC-2168285, SPEC-2168288 */
    J1939Nm_ReportDetError(J1939NM_SID_GETFIRSTUNKNOWNNAMEIDX, J1939NM_E_INVALID_NETWORK_ID);
    /* PRQA L:PC */
  }
# endif

  return nodeIdx;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/**********************************************************************************************************************
 *  J1939Nm_GetLastNodeIdx
 *********************************************************************************************************************/

FUNC(uint16, J1939NM_CODE) J1939Nm_GetLastNodeIdx(NetworkHandleType channel)
{
  uint16 nodeIdx = 0;

  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_GETLASTNODEIDX, J1939NM_E_UNINIT);
    return nodeIdx;
  }

  if (J1939Nm_Channel_Valid(channel))
  {
    nodeIdx = J1939Nm_GetLastNodeIdOfChannel(J1939Nm_GetChannelIdxOfComMChannel(channel));
  }
# if (J1939NM_DEV_ERROR_DETECT == STD_ON)
  else
  {
    /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
    /* \trace SPEC-2168285, SPEC-2168288 */
    J1939Nm_ReportDetError(J1939NM_SID_GETLASTNODEIDX, J1939NM_E_INVALID_NETWORK_ID);
    /* PRQA L:PC */
  }
# endif

  return nodeIdx;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/**********************************************************************************************************************
 *  J1939Nm_FindNodeByName
 *********************************************************************************************************************/

FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_FindNodeByName(NetworkHandleType channel, boolean external,
                                                          P2CONST(uint8, AUTOMATIC, J1939NM_APPL_DATA) name,
                                                          P2CONST(uint8, AUTOMATIC, J1939NM_APPL_DATA) mask,
                                                          P2VAR(uint16, AUTOMATIC, J1939NM_APPL_VAR) nodeId)
{
  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_FINDNODEBYNAME, J1939NM_E_UNINIT);
    return E_NOT_OK;
  }

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168285, SPEC-2168288 */
  J1939Nm_AssertRetVal(J1939Nm_Channel_Valid(channel), J1939NM_SID_FINDNODEBYNAME, J1939NM_E_INVALID_NETWORK_ID);
  /* \trace SPEC-2168285 */
  J1939Nm_AssertRetVal(name != NULL_PTR, J1939NM_SID_FINDNODEBYNAME, J1939NM_E_PARAM_POINTER);
  J1939Nm_AssertRetVal(mask != NULL_PTR, J1939NM_SID_FINDNODEBYNAME, J1939NM_E_PARAM_POINTER);
  J1939Nm_AssertRetVal(nodeId != NULL_PTR, J1939NM_SID_FINDNODEBYNAME, J1939NM_E_PARAM_POINTER);
  /* PRQA L:PC */

  if (external) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
  {
# if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
    J1939Nm_SizeOfExternalNodeType extNodeIdx;

    for (extNodeIdx = (J1939Nm_SizeOfExternalNodeType) * nodeId; extNodeIdx < J1939Nm_GetSizeOfExternalNode();
         extNodeIdx++)
    {
#  if (J1939NM_INVALIDHNDOFEXTERNALNODE == STD_ON)
      if (!J1939Nm_IsInvalidHndOfExternalNode(extNodeIdx))
#  endif
      {
        J1939Nm_ExternalNodeChannelNodeIndEndIdxOfExternalNodeType extNodeChannelIndIdx;

        for (extNodeChannelIndIdx = J1939Nm_GetExternalNodeChannelNodeIndStartIdxOfExternalNode(extNodeIdx);
             extNodeChannelIndIdx < J1939Nm_GetExternalNodeChannelNodeIndEndIdxOfExternalNode(extNodeIdx);
             extNodeChannelIndIdx++)
        {
          J1939Nm_ExternalNodeChannelNodeIndType extNodeChannelIdx =
            J1939Nm_GetExternalNodeChannelNodeInd(extNodeChannelIndIdx);
          J1939Nm_ExternalNodeChannelStateIdxOfExternalNodeChannelType extNodeChannelStateIdx =
            J1939Nm_GetExternalNodeChannelStateIdxOfExternalNodeChannel(extNodeChannelIdx);
          J1939Nm_ExternalNodeChannelStateType *extNodeChannelState =
            &J1939Nm_GetExternalNodeChannelState(extNodeChannelStateIdx);

          if (extNodeChannelState->ExtNodeState != J1939NM_EXTNODESTATE_UNKNOWN)
          {
            uint8 *currentExtNodeName =
              J1939Nm_GetCurrentExtNodeName(J1939Nm_GetCurrentExtNodeNameIdxOfExternalNode(extNodeIdx));

            SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

            if (J1939Nm_InternalFilteredNameCompare(currentExtNodeName, name, mask)) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
            {
              J1939Nm_ChannelIdxOfExternalNodeChannelType channelIdx =
                J1939Nm_GetChannelIdxOfExternalNodeChannel(extNodeChannelIdx);

              SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

              if (channel == J1939Nm_GetComMChannelIdOfChannel(channelIdx))
              {
                *nodeId = extNodeIdx;

                return E_OK;
              }
            }
            else
            {
              SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            }
          }
        }
      }
    }
# endif
  }
  else
  {
    J1939Nm_SizeOfNodeType nodeIdx;

    for (nodeIdx = (J1939Nm_SizeOfNodeType) * nodeId; nodeIdx < J1939Nm_GetSizeOfNode(); nodeIdx++)
    {
      uint8 *currentNodeName = J1939Nm_GetCurrentNodeName(J1939Nm_GetCurrentNodeNameIdxOfNode(nodeIdx));

      SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      if (J1939Nm_InternalFilteredNameCompare(currentNodeName, name, mask)) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
      {
        J1939Nm_NodeChannelNodeIndEndIdxOfNodeType nodeChannelIndIdx;

        SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

        for (nodeChannelIndIdx = J1939Nm_GetNodeChannelNodeIndStartIdxOfNode(nodeIdx);
             nodeChannelIndIdx < J1939Nm_GetNodeChannelNodeIndEndIdxOfNode(nodeIdx); nodeChannelIndIdx++)
        {
          J1939Nm_NodeChannelNodeIndType nodeChannelIdx = J1939Nm_GetNodeChannelNodeInd(nodeChannelIndIdx);
          J1939Nm_ChannelIdxOfNodeChannelType channelIdx = J1939Nm_GetChannelIdxOfNodeChannel(nodeChannelIdx);

          if (channel == J1939Nm_GetComMChannelIdOfChannel(channelIdx))
          {
            *nodeId = nodeIdx;

            return E_OK;
          }
        }
      }
      else
      {
        SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
    }
  }

  return E_NOT_OK;
} /* PRQA S 2006, 6010, 6030, 6080 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */


/**********************************************************************************************************************
 *  J1939Nm_FindNodeByAddress
 *********************************************************************************************************************/

FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_FindNodeByAddress(NetworkHandleType channel, boolean external,
                                                             uint8 virtAddr,
                                                             P2VAR(uint16, AUTOMATIC, J1939NM_APPL_VAR) nodeId)
{
  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_FINDNODEBYADDRESS, J1939NM_E_UNINIT);
    return E_NOT_OK;
  }

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168285, SPEC-2168288 */
  J1939Nm_AssertRetVal(J1939Nm_Channel_Valid(channel), J1939NM_SID_FINDNODEBYADDRESS, J1939NM_E_INVALID_NETWORK_ID);
  /* \trace SPEC-2168285 */
  J1939Nm_AssertRetVal(J1939Nm_SA_ValidNonNull(virtAddr), J1939NM_SID_FINDNODEBYADDRESS, J1939NM_E_INVALID_ADDRESS);
  /* \trace SPEC-2168285 */
  J1939Nm_AssertRetVal(nodeId != NULL_PTR, J1939NM_SID_FINDNODEBYADDRESS, J1939NM_E_PARAM_POINTER);
  /* PRQA L:PC */

  if (external) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
  {
# if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
    J1939Nm_SizeOfExternalNodeType extNodeIdx;

    for (extNodeIdx = 0; extNodeIdx < J1939Nm_GetSizeOfExternalNode(); extNodeIdx++)
    {
#  if (J1939NM_INVALIDHNDOFEXTERNALNODE == STD_ON)
      if (!J1939Nm_IsInvalidHndOfExternalNode(extNodeIdx))
#  endif
      {
        if (virtAddr == J1939Nm_GetNodeAddressOfExternalNode(extNodeIdx))
        {
          J1939Nm_ExternalNodeChannelNodeIndEndIdxOfExternalNodeType extNodeChannelIndIdx;

          for (extNodeChannelIndIdx = J1939Nm_GetExternalNodeChannelNodeIndStartIdxOfExternalNode(extNodeIdx);
               extNodeChannelIndIdx < J1939Nm_GetExternalNodeChannelNodeIndEndIdxOfExternalNode(extNodeIdx);
               extNodeChannelIndIdx++)
          {
            J1939Nm_ExternalNodeChannelNodeIndType extNodeChannelIdx =
              J1939Nm_GetExternalNodeChannelNodeInd(extNodeChannelIndIdx);
            J1939Nm_ExternalNodeChannelStateIdxOfExternalNodeChannelType extNodeChannelStateIdx =
              J1939Nm_GetExternalNodeChannelStateIdxOfExternalNodeChannel(extNodeChannelIdx);
            J1939Nm_ExternalNodeChannelStateType *extNodeChannelState =
              &J1939Nm_GetExternalNodeChannelState(extNodeChannelStateIdx);

            if (extNodeChannelState->ExtNodeState != J1939NM_EXTNODESTATE_UNKNOWN)
            {
              J1939Nm_ChannelIdxOfExternalNodeChannelType channelIdx =
                J1939Nm_GetChannelIdxOfExternalNodeChannel(extNodeChannelIdx);

              if (channel == J1939Nm_GetComMChannelIdOfChannel(channelIdx))
              {
                *nodeId = extNodeIdx;

                return E_OK;
              }
            }
          }
        }
      }
    }
# endif
  }
  else
  {
    J1939Nm_SizeOfNodeType nodeIdx;

    for (nodeIdx = 0; nodeIdx < J1939Nm_GetSizeOfNode(); nodeIdx++)
    {
      if (virtAddr == J1939Nm_GetNodeAddressOfNode(nodeIdx))
      {
        J1939Nm_NodeChannelNodeIndEndIdxOfNodeType nodeChannelIndIdx;

        for (nodeChannelIndIdx = J1939Nm_GetNodeChannelNodeIndStartIdxOfNode(nodeIdx);
             nodeChannelIndIdx < J1939Nm_GetNodeChannelNodeIndEndIdxOfNode(nodeIdx); nodeChannelIndIdx++)
        {
          J1939Nm_NodeChannelNodeIndType nodeChannelIdx = J1939Nm_GetNodeChannelNodeInd(nodeChannelIndIdx);
          J1939Nm_ChannelIdxOfNodeChannelType channelIdx = J1939Nm_GetChannelIdxOfNodeChannel(nodeChannelIdx);

          if (channel == J1939Nm_GetComMChannelIdOfChannel(channelIdx))
          {
            *nodeId = nodeIdx;

            return E_OK;
          }
        }
      }
    }
  }

  return E_NOT_OK;
} /* PRQA S 2006, 6010, 6030, 6080 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */


/**********************************************************************************************************************
 *  J1939Nm_SetName
 *********************************************************************************************************************/

FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_SetName(NetworkHandleType channel, uint16 nodeId, boolean external,
                                                   P2CONST(uint8, AUTOMATIC, J1939NM_APPL_DATA) name)
{
  Std_ReturnType result = E_NOT_OK;

  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_SETNAME, J1939NM_E_UNINIT);
    return E_NOT_OK;
  }

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168285, SPEC-2168288 */
  J1939Nm_AssertRetVal(J1939Nm_Channel_Valid(channel), J1939NM_SID_SETNAME, J1939NM_E_INVALID_NETWORK_ID);
  /* \trace SPEC-2168285 */
  J1939Nm_AssertRetVal(nodeId < J1939Nm_GetSizeOfNode(), J1939NM_SID_SETNAME, J1939NM_E_INVALID_NODE);
  /* \trace SPEC-2168285 */
  J1939Nm_AssertRetVal(name != NULL_PTR, J1939NM_SID_SETNAME, J1939NM_E_PARAM_POINTER);
  /* PRQA L:PC */

# if (J1939NM_NM_PG_SUPPORT == STD_ON)
  if (nmMgmt.State == J1939NM_NM_IDLE)
# endif
  {
    if (!external) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
    {
      J1939Nm_ChannelIdxOfComMChannelType channelIdx = J1939Nm_GetChannelIdxOfComMChannel(channel);
      J1939Nm_NodeChannelNodeIndEndIdxOfNodeType nodeChannelIndIdx;

      for (nodeChannelIndIdx = J1939Nm_GetNodeChannelNodeIndStartIdxOfNode(nodeId);
           nodeChannelIndIdx < J1939Nm_GetNodeChannelNodeIndEndIdxOfNode(nodeId); nodeChannelIndIdx++)
      {
        J1939Nm_NodeChannelNodeIndType nodeChannelIdx = J1939Nm_GetNodeChannelNodeInd(nodeChannelIndIdx);

        if (channelIdx == J1939Nm_GetChannelIdxOfNodeChannel(nodeChannelIdx))
        {
          result = J1939Nm_InternalSetName(channelIdx, nodeId, name);

          break;
        }
      }
    }
    else
    {
      J1939Nm_ReportDetError(J1939NM_SID_SETNAME, J1939NM_E_EXTERNAL_NODE);
    }
  }

  return result;
} /* PRQA S 2006, 6010, 6030, 6080 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  J1939Nm_SetAddress
 *********************************************************************************************************************/

FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_SetAddress(NetworkHandleType channel, uint16 nodeId, boolean external,
                                                      uint8 busAddr)
{
  Std_ReturnType result = E_NOT_OK;

  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_SETADDRESS, J1939NM_E_UNINIT);
    return E_NOT_OK;
  }

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168285, SPEC-2168288 */
  J1939Nm_AssertRetVal(J1939Nm_Channel_Valid(channel), J1939NM_SID_SETADDRESS, J1939NM_E_INVALID_NETWORK_ID);
  /* \trace SPEC-2168285 */
  J1939Nm_AssertRetVal(nodeId < J1939Nm_GetSizeOfNode(), J1939NM_SID_SETADDRESS, J1939NM_E_INVALID_NODE);
  /* \trace SPEC-2168285 */
  J1939Nm_AssertRetVal(busAddr < J1939NM_NULL_ADDRESS, J1939NM_SID_SETADDRESS, J1939NM_E_INVALID_ADDRESS);
  /* PRQA L:PC */

  if (!external) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
  {
# if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
    J1939Nm_ChannelIdxOfComMChannelType channelIdx = J1939Nm_GetChannelIdxOfComMChannel(channel);

    if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
    {
#  if (J1939NM_NM_STATECHANGENOTIF == STD_ON)
      J1939Nm_ChannelStateType *channelState = &J1939Nm_GetChannelState(channelIdx);
      Nm_StateType oldState = channelState->NmState;
#  endif

      {
        J1939Nm_NodeChannelChannelIndEndIdxOfChannelType nodeChannelIndIdx;

        for (nodeChannelIndIdx = J1939Nm_GetNodeChannelChannelIndStartIdxOfChannel(channelIdx);
             nodeChannelIndIdx < J1939Nm_GetNodeChannelChannelIndEndIdxOfChannel(channelIdx); nodeChannelIndIdx++)
        {
          J1939Nm_NodeChannelNodeIndType nodeChannelIdx = J1939Nm_GetNodeChannelChannelInd(nodeChannelIndIdx);

          if (nodeId == J1939Nm_GetNodeIdxOfNodeChannel(nodeChannelIdx))
          {
            result = J1939Nm_InternalSetAddress((J1939Nm_SizeOfNodeType) nodeId, nodeChannelIdx, channelIdx, busAddr);

            break;
          }
        }
      }

#  if (J1939NM_NM_STATECHANGENOTIF == STD_ON)
      {
        Nm_StateType newState = channelState->NmState;

        if (oldState != newState)
        {
          /* \trace SPEC-2168305 */
          Nm_StateChangeNotification(channel, oldState, newState);
        }
      }
#  endif
    }
# endif
  }
  else
  {
    J1939Nm_ReportDetError(J1939NM_SID_SETADDRESS, J1939NM_E_EXTERNAL_NODE);
  }

  return result;
} /* PRQA S 2006, 6010 */ /* MD_MSR_14.7, MD_MSR_STPTH */
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalNmRequestIndication()
 *********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_InternalNmRequestIndication(J1939Nm_ChannelIdxOfComMChannelType channel, uint8 node,
                                                             uint8 sourceAddress, uint8 destAddress)
{
  uint8 name[J1939NM_NAME_SIZE];
  J1939Nm_NmModeType mode = J1939NM_NM_MODE_CURRENT_NAME_RESPONSE;

  if (J1939Nm_IsTxPduNmUsedOfChannel(channel))
  {
    /* #-- If a pending name for the requested node is avaliable, send the pending name */
    if ((nmMgmt.State == J1939NM_NM_PENDING) && (nmMgmt.NodeId == node))
    {
      mode = J1939NM_NM_MODE_PENDING_NAME_RESPONSE;

      SchM_Enter_J1939Nm_NmActiveLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      J1939Nm_InternalCopyName(name, nmMgmt.PendingName);

      SchM_Enter_J1939Nm_NmActiveLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
    /* #-- Otherwise send the current name of the node */
    else
    {
      SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      J1939Nm_InternalCopyName(name, J1939Nm_GetCurrentNodeName(J1939Nm_GetCurrentNodeNameIdxOfNode(node)));

      SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

    J1939Nm_InternalNmSendName(channel, node, destAddress, sourceAddress, name, mode);
  }
  else
  {
    (void) J1939Rm_SendAck(J1939Nm_GetJ1939RmUserIdOfNode(node), J1939Nm_GetComMChannelIdOfChannel(channel),
                           J1939NM_PGN_NM, NULL_PTR, J1939RM_ACK_NEGATIVE, sourceAddress, J1939NM_DEFAULT_PRIORITY,
                           destAddress == J1939NM_BROADCAST_ADDRESS);
  }
} /* J1939Nm_InternalNmRequestIndication() */
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalHandleNm()
 *********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_InternalHandleNm(PduIdType RxPduId,
                                                  P2CONST(PduInfoType, AUTOMATIC, J1939NM_APPL_DATA) info)
{
  uint8 nodeAddress;
  J1939Nm_NodeChannelChannelIndEndIdxOfChannelType nodeChannelIndIdx;
  J1939Nm_ChannelChannelIdxOfRxPduType channelIdx = J1939Nm_GetChannelChannelIdxOfRxPdu(RxPduId);
  uint8 sourceAddress = info->SduDataPtr[J1939NM_NM_SA_POS];
  uint8 destAddress = info->SduDataPtr[J1939NM_NM_DA_POS];
  uint8 mode = J1939Nm_NM_GetMode(info->SduDataPtr);

  if (destAddress == J1939NM_BROADCAST_ADDRESS)
  {
    if (mode == J1939NM_NM_MODE_ADOPT_PENDING_NAME)
    {
      J1939Nm_InternalNmHandleAdoptPendingName(channelIdx, J1939NM_NM_NODE_ANY, sourceAddress, destAddress);
    }
  }
  else
  {
    /* #-- Go through all nodes of the channel the NM request was received on */
    for (nodeChannelIndIdx = J1939Nm_GetNodeChannelChannelIndStartIdxOfChannel(channelIdx);
         nodeChannelIndIdx < J1939Nm_GetNodeChannelChannelIndEndIdxOfChannel(channelIdx); nodeChannelIndIdx++)
    {
      J1939Nm_NodeChannelChannelIndType nodeChannelIdx = J1939Nm_GetNodeChannelChannelInd(nodeChannelIndIdx);
      J1939Nm_NodeIdxOfNodeChannelType nodeIdx = J1939Nm_GetNodeIdxOfNodeChannel(nodeChannelIdx);

# if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
      if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
      {
        nodeAddress = J1939Nm_GetCurrentNodeAddress(J1939Nm_GetCurrentNodeAddressIdxOfNodeChannel(nodeChannelIdx));
      }
      else
# endif
      {
        nodeAddress = J1939Nm_GetNodeAddressOfNode(nodeIdx);
      }

      if (destAddress == nodeAddress)
      {
        /* #-- Process the NM request depending on the mode: Mode 0: Set pending request Mode 5: Request for pending
           name Mode 6: Request for current name Mode 7: Adopt pending name */
        switch (mode)
        {
        case J1939NM_NM_MODE_SET_PENDING_NAME:
          J1939Nm_InternalNmHandleSetPendingName(info, channelIdx, nodeIdx, sourceAddress, destAddress);
          break;

        case J1939NM_NM_MODE_REQUEST_PENDING_NAME:
          J1939Nm_InternalNmHandleRequestPendingName(channelIdx, nodeIdx, sourceAddress, destAddress);
          break;

        case J1939NM_NM_MODE_REQUEST_CURRENT_NAME:
          J1939Nm_InternalNmHandleRequestCurrentName(channelIdx, nodeIdx, sourceAddress, destAddress);
          break;

        case J1939NM_NM_MODE_ADOPT_PENDING_NAME:
          J1939Nm_InternalNmHandleAdoptPendingName(channelIdx, nodeIdx, sourceAddress, destAddress);
          break;

        default:
          break;
        }

        break;
      }
    }
  }
} /* J1939Nm_InternalHandleNm() */ /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif

#if ((J1939NM_NM_PG_SUPPORT == STD_ON) && (J1939NM_NMQUEUE == STD_ON))
/**********************************************************************************************************************
 *  J1939Nm_InternalHandleNmTxConfirmation()
 *********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_InternalHandleNmTxConfirmation(J1939Nm_ChannelChannelIdxOfTxPduType channelId)
{
  J1939Nm_InternalProcessNmQueue(channelId);
} /* J1939Nm_InternalHandleNmTxConfirmation() */
#endif

#define J1939NM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*============================================== J1939NM_STOP_SEC_CODE ==============================================*/


/* Module specific MISRA deviations:

 MD_J1939Nm_13.2
      Reason:     No deviation: MISRA rule 13.2 explicitly allows to use Boolean checks when it is ensured that the tested variable is only used as a Boolean value.
      Risk:       None
      Prevention: N/A

 MD_J1939Nm_13.7
      Reason:     Theoretically, an enum parameter cannot lie outside the values defined for the enum, but practically,
                  this is possible. Therefore, these cases must be checked, and the error detection code is then
                  detected as always false/true.
      Risk:       None
      Prevention: N/A

 MD_J1939Nm_14.2
      Reason:     Dummy statement to avoid compiler warnings.
      Risk:       None
      Prevention: N/A

 MD_J1939Nm_16.7
      Reason:     Parameters of standardized APIs must adhere to their definition.
      Risk:       None
      Prevention: N/A
*/
