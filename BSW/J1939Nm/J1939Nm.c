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
 *         File:  J1939Nm.c
 *       Module:  SAE J1939 Network Management (J1939Nm)
 *
 *  Description:  Implementation of the SAE J1939 Network Management module.
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
/* PRQA S 0850 EOF */ /* MD_MSR_19.8 */
/* PRQA S 1503 EOF */ /* MD_MSR_14.1 */

/* *INDENT-ON* */


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#define J1939NM_C

#include "J1939Nm.h"
#include "J1939Nm_Cbk.h"

#include "J1939Nm_Cfg.h"
#include "J1939Nm_Lcfg.h"
#include "J1939Nm_PBcfg.h"

#include "J1939Nm_Int.h"
#if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) || (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON))
# include "J1939Nm_Dynamic.h"
#endif

#include "BswM_J1939Nm.h"
#include "CanIf.h"
#include "Nm_Cbk.h"
#include "J1939Rm.h"

#if ((J1939NM_ADDRESSSTORAGEBLOCK == STD_ON) || (J1939NM_NAMESTORAGEBLOCK == STD_ON))
# include "NvM.h"
#endif

#include "SchM_J1939Nm.h"

#if (J1939NM_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
# include "EcuM_Error.h"
#endif


/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Vendor specific BSW module version information */
#define J1939NM_MAJOR_SOURCE_VERSION            (3u)
#define J1939NM_MINOR_SOURCE_VERSION            (0u)
#define J1939NM_PATCH_SOURCE_VERSION            (0u)

/* Vendor specific BSW generator version information */
#define J1939NM_MAJOR_GENERATOR_VERSION         (2u)
#define J1939NM_MINOR_GENERATOR_VERSION         (0u)

/* Check the vendor specific version of J1939Nm module header file */
#if ((J1939NM_SW_MAJOR_VERSION != J1939NM_MAJOR_SOURCE_VERSION) || (J1939NM_SW_MINOR_VERSION != J1939NM_MINOR_SOURCE_VERSION) || (J1939NM_SW_PATCH_VERSION != J1939NM_PATCH_SOURCE_VERSION))
# error "Vendor specific version numbers of J1939Nm.c and J1939Nm.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if ((J1939NM_CFG_MAJOR_VERSION != J1939NM_MAJOR_GENERATOR_VERSION) || (J1939NM_CFG_MINOR_VERSION != J1939NM_MINOR_GENERATOR_VERSION))
# error "Vendor specific version numbers of J1939Nm.c and J1939Nm_Cfg.h are inconsistent!"
#endif


/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/*======================================= J1939NM_START_SEC_VAR_ZERO_INIT_8BIT ======================================*/
#define J1939NM_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! \brief Initialization state of the J1399Rm */
VAR(uint8, J1939NM_VAR_ZERO_INIT) J1939Nm_ModuleInitialized = J1939NM_UNINIT;

#define J1939NM_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*======================================== J1939NM_STOP_SEC_VAR_ZERO_INIT_8BIT ======================================*/


/**********************************************************************************************************************
 *  INTERNAL DATA
 *********************************************************************************************************************/

/*=================================== J1939NM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED ===================================*/
#define J1939NM_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! \brief Initialization state of the J1399Nm */
#if (J1939NM_USE_INIT_POINTER == STD_ON)
P2CONST(J1939Nm_ConfigType, J1939NM_VAR_ZERO_INIT, J1939NM_PBCFG) J1939Nm_ConfigDataPtr = NULL_PTR;
#endif

#define J1939NM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*==================================== J1939NM_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED ===================================*/


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*================================================ J1939NM_START_SEC_CODE ===========================================*/
#define J1939NM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (J1939NM_USESADDRESSARBITRATIONOFCHANNEL == STD_ON)
static FUNC(boolean, J1939NM_CODE) J1939Nm_InternalCheckAcCollision(J1939Nm_ChannelChannelIdxOfRxPduType channelIdx,
                                                                    P2CONST(PduInfoType, AUTOMATIC,
                                                                            J1939NM_APPL_DATA) info,
                                                                    uint16 localNodeIdx);
#endif

static FUNC(void, J1939NM_CODE) J1939Nm_InternalSendAc(const J1939Nm_SizeOfChannelType channelIdx,
                                                       const J1939Nm_SizeOfNodeType nodeIdx, const uint8 sourceAddress);

static FUNC(uint8, J1939NM_CODE) J1939Nm_InternalCalcDelay(const J1939Nm_SizeOfNodeType nodeIdx, const boolean intern);

#define J1939NM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*================================================ J1939NM_STOP_SEC_CODE ===========================================*/


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/*================================================ J1939NM_START_SEC_CODE ===========================================*/
#define J1939NM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (J1939NM_USESADDRESSARBITRATIONOFCHANNEL == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalCheckAcCollision
 *********************************************************************************************************************/
/*! \brief      Checks for NAME and address collisions of received/transmitted AddressClaimed (AC) messages.
 *
 *  \param      channelIdx      Internal index of the channel on which the AC has been received/transmitted.
 *  \param      info            AC PDU
 *  \param      localNodeIdx    Node that transmitted the AC, or J1939NM_INVALID_NODE if the AC was received.
 *  \return     TRUE:           NAME was identical to an internal node's NAME,
 *              FALSE:          No NAME conflict detected.
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/

static FUNC(boolean, J1939NM_CODE) J1939Nm_InternalCheckAcCollision(J1939Nm_ChannelChannelIdxOfRxPduType channelIdx,
                                                                    P2CONST(PduInfoType, AUTOMATIC,
                                                                            J1939NM_APPL_DATA) info,
                                                                    uint16 localNodeIdx)
{
  J1939Nm_ChannelStateType *channelState = &J1939Nm_GetChannelState(channelIdx);
  J1939Nm_NodeChannelChannelIndEndIdxOfChannelType nodeChannelIndIdx;
  Nm_StateType nmState = NM_STATE_OFFLINE;
  boolean nameConflict = FALSE;

  for (nodeChannelIndIdx = J1939Nm_GetNodeChannelChannelIndStartIdxOfChannel(channelIdx);
       nodeChannelIndIdx < J1939Nm_GetNodeChannelChannelIndEndIdxOfChannel(channelIdx); nodeChannelIndIdx++)
  {
    J1939Nm_NodeChannelChannelIndType nodeChannelIdx = J1939Nm_GetNodeChannelChannelInd(nodeChannelIndIdx);
    J1939Nm_NodeIdxOfNodeChannelType nodeIdx = J1939Nm_GetNodeIdxOfNodeChannel(nodeChannelIdx);
    J1939Nm_NodeChannelStateType *nodeChannelState =
      &J1939Nm_GetNodeChannelState(J1939Nm_GetNodeChannelStateIdxOfNodeChannel(nodeChannelIdx));

    if (nodeIdx != localNodeIdx)
    {
      if ((nodeChannelState->NodeState == J1939NM_NODESTATE_NORMAL) ||
          (nodeChannelState->NodeState == J1939NM_NODESTATE_CLAIMING) ||
          (nodeChannelState->NodeState == J1939NM_NODESTATE_PRE_CLAIMING) ||
          (nodeChannelState->NodeState == J1939NM_NODESTATE_PRE_NORMAL))
      {
        uint8 nodeAddress;
# if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
        J1939Nm_CurrentNodeAddressIdxOfNodeChannelType currentNodeAddressIdx =
          J1939Nm_GetCurrentNodeAddressIdxOfNodeChannel(nodeChannelIdx);

        if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
        {
          nodeAddress = J1939Nm_GetCurrentNodeAddress(currentNodeAddressIdx);
        }
        else
# endif
        {
          nodeAddress = J1939Nm_GetNodeAddressOfNode(nodeIdx);
        }

        /* TODO: Always compare NAME and report if DET is enabled (see ALM+ issue 84909) */
        if (nodeAddress == info->SduDataPtr[J1939NM_AC_SA_POS])
        {
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

          cmp = J1939Nm_InternalNameCompare(currentNodeName, &(info->SduDataPtr[J1939NM_AC_NAME_BYTE1]));

# if (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON)
          SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
# endif

          if (cmp == 0)
          {
            nameConflict = TRUE;

            /* \trace SPEC-2168275, SPEC-2168276, SPEC-2168277 */
            nodeChannelState->AcPending = TRUE;

            if (nodeChannelState->NodeState == J1939NM_NODESTATE_NORMAL)
            {
              nmState = NM_STATE_NORMAL_OPERATION;
            }
          }
          else if (cmp > 0)
          {
# if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
            /* J1939Nm_ClearBusAddressAllocated must not be called because the previously claimed address has been
               claimed by another node */

            if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx) &&
                (J1939Nm_GetNodeAddressConfigurationCapabilityOfNode(nodeIdx) ==
                 J1939NM_AAC_NODEADDRESSCONFIGURATIONCAPABILITYOFNODE))
            {
              nodeAddress = J1939Nm_DynamicFindFreeAddress(nodeAddress, nodeIdx, channelState);
            }
            else
            {
              nodeAddress = J1939NM_NULL_ADDRESS;
            }

            if (nodeAddress == J1939NM_NULL_ADDRESS)
# endif
            {
              /* \trace SPEC-2168271, SPEC-2168281 */
              nodeChannelState->NodeState = J1939NM_NODESTATE_ACLOST;
# if (J1939NM_J1939NM_E_ADDRESS_LOST == STD_ON)
              /* \trace SPEC-2168286, SPEC-2168305 */
              Dem_ReportErrorStatus(J1939Nm_GetJ1939NM_E_ADDRESS_LOST(), DEM_EVENT_STATUS_PREFAILED);
# endif
              /* \trace SPEC-2168272, SPEC-2168305 */
              BswM_J1939Nm_StateChangeNotification(J1939Nm_GetComMChannelIdOfChannel(channelIdx), (uint8) nodeIdx,
                                                   NM_STATE_OFFLINE);

              /* Insert random delay to avoid BusOff in case several nodes fail to claim an address at the same time */
              nodeChannelState->AcDelayed = TRUE;
            }
# if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
            else
            {
              if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
              {
                J1939Nm_SetBusAddressAllocated(channelState, nodeAddress);
              }

              /* \trace SPEC-2168275, SPEC-2168276, SPEC-2168277 */
              nodeChannelState->AcPending = TRUE;

              if (nodeChannelState->NodeState == J1939NM_NODESTATE_NORMAL)
              {
                nmState = NM_STATE_NORMAL_OPERATION;
              }
            }

            if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
            {
              /* save NULL or new address */
              J1939Nm_SetCurrentNodeAddress(currentNodeAddressIdx, nodeAddress);

#  if (J1939NM_ADDRESSSTORAGEBLOCK == STD_ON)
              (void) NvM_SetRamBlockStatus(NvMConf_NvMBlockDescriptor_J1939Nm_CurrentNodeAddresses, TRUE);
#  endif

              CanIf_ResetAddressTableEntry(J1939Nm_GetCanIfCtrlIdOfChannel(channelIdx),
                                           J1939Nm_GetNodeAddressOfNode(nodeIdx));
            }
# endif
          }
          else
          {
            /* \trace SPEC-2168275, SPEC-2168276, SPEC-2168277, SPEC-2168280 */
            nodeChannelState->AcPending = TRUE;

            /* the node of the received AC lost against a local node, and shall therefore not reserve its address */
            info->SduDataPtr[J1939NM_AC_SA_POS] = J1939NM_NULL_ADDRESS;

            if (nodeChannelState->NodeState == J1939NM_NODESTATE_NORMAL)
            {
              nmState = NM_STATE_NORMAL_OPERATION;
            }
          }
        }
        else
        {
          if (nodeChannelState->NodeState == J1939NM_NODESTATE_NORMAL)
          {
            nmState = NM_STATE_NORMAL_OPERATION;
          }
        }
      }
    }
    else
    {
      if (nodeChannelState->NodeState == J1939NM_NODESTATE_NORMAL)
      {
        nmState = NM_STATE_NORMAL_OPERATION;
      }
    }
  }

  if (nmState != channelState->NmState)
  {
    /* \trace SPEC-2168273 */
    channelState->NmState = nmState;
  }

  return nameConflict;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif


/**********************************************************************************************************************
 *  J1939Nm_InternalSendAc
 *********************************************************************************************************************/
/*! \brief      Transmits an AddressClaimed (AC) message.
 *
 *  \param      channelIdx      Internal index of the channel on which the AC shall be transmitted.
 *  \param      nodeIdx         Index of the node that sends the AC.
 *  \param      sourceAddress   The address that shall be claimed.
 *  \return     none
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/

static FUNC(void, J1939NM_CODE) J1939Nm_InternalSendAc(const J1939Nm_SizeOfChannelType channelIdx,
                                                       const J1939Nm_SizeOfNodeType nodeIdx, const uint8 sourceAddress)
{
  J1939Nm_TxPduAcIdxOfChannelType txPduIdx = J1939Nm_GetTxPduAcIdxOfChannel(channelIdx);
  J1939Nm_TxPduStateType *pduState = &J1939Nm_GetTxPduState(J1939Nm_GetTxPduStateIdxOfTxPdu(txPduIdx));

  SchM_Enter_J1939Nm_TxPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (!pduState->Locked)
  {
    pduState->Locked = TRUE;
    SchM_Exit_J1939Nm_TxPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    pduState->NodeId = (uint8) nodeIdx;
    /* \trace SPEC-2168279 */
    pduState->ConfTimeout = J1939Nm_GetTxConfTimeout();

    {
      uint8 data[J1939NM_AC_PDU_LENGTH];
      PduInfoType pdu;

      pdu.SduDataPtr = data;
      pdu.SduLength = J1939NM_AC_FRAME_LENGTH + J1939Nm_GetMetaDataLengthOfTxPdu(txPduIdx);

      {
#if (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON)
        uint8 *currentNodeName = J1939Nm_GetCurrentNodeName(J1939Nm_GetCurrentNodeNameIdxOfNode(nodeIdx));

        SchM_Enter_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        J1939Nm_InternalCopyName(&(data[J1939NM_AC_NAME_BYTE1]), currentNodeName);
        SchM_Exit_J1939Nm_NameLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
#else
        J1939Nm_NodeNameIdxOfNodeType nodeNameIdx = J1939Nm_GetNodeNameIdxOfNode(nodeIdx);

        J1939Nm_InternalGetName(data, nodeNameIdx);
#endif
        data[J1939NM_AC_SA_POS] = sourceAddress;
      }

      /* \trace SPEC-2168275, SPEC-2168305 */
      if (CanIf_Transmit(J1939Nm_GetPduIdOfTxPdu(txPduIdx), &pdu) != E_OK)
      {
        pduState->Locked = FALSE;
      }
#if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) || (J1939NM_USERCALLOUT == STD_ON))
      else
      {
# if (J1939NM_USERCALLOUT == STD_ON)
        J1939Nm_GetUserCallout()(J1939Nm_GetComMChannelIdOfChannel(channelIdx), data[8], data);
# endif

# if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
        if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
        {
          /* Cast to void because it does not matter if there is a NAME conflict in this case. */
          (void) J1939Nm_InternalCheckAcCollision(channelIdx, &pdu, nodeIdx);
        }
# endif
      }
#endif
    }
  }
  else
  {
    SchM_Exit_J1939Nm_TxPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    /* \trace SPEC-2168276 */
  }
} /* PRQA S 6050 */ /* MD_MSR_STCAL */


/**********************************************************************************************************************
 *  J1939Nm_InternalCalcDelay
 *********************************************************************************************************************/
/*! \brief      Calculate a random number of MainFunction cycles to delay BusOff recovery or transmission of AC.
 *
 *  \param      nodeIdx         Index of the node for which the delay shall be calculated.
 *  \param      intern          TRUE: Calculate number of J1939Nm_MainFunction cycles,
 *                              FALSE: Calculate number of CanSM_MainFunction cycles.
 *  \return                     Number of J1939Nm or CanSMN MainFunction cycles, depending on parameter 'intern'.
 *  \context    May be called from interrupt or task level.
 *
 *  \trace SPEC-2168261
 *********************************************************************************************************************/

static FUNC(uint8, J1939NM_CODE) J1939Nm_InternalCalcDelay(const J1939Nm_SizeOfNodeType nodeIdx, const boolean intern)
{
  J1939Nm_RandomSeedIdxOfNodeType randomSeedIdx = J1939Nm_GetRandomSeedIdxOfNode(nodeIdx);
  uint8 delayCycles;

  /* Mixed congruential generator: x(n+1) = a * x(n) + b */
  uint8 random = (uint8) ((J1939NM_RANDOM_FACTOR * J1939Nm_GetRandomSeed(randomSeedIdx)) + J1939NM_RANDOM_OFFSET);
  J1939Nm_SetRandomSeed(randomSeedIdx, random);

  if (intern) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
  {
    delayCycles = (uint8) (((uint32) J1939Nm_GetJ1939NmDelayFactor() * (uint32) random) / J1939NM_RANDOM_SCALING);
  }
  else
  {
    delayCycles = (uint8) (((uint32) J1939Nm_GetCanSMDelayFactor() * (uint32) random) / J1939NM_RANDOM_SCALING);
  }

  return delayCycles;
}

#define J1939NM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*================================================ J1939NM_STOP_SEC_CODE ============================================*/


/**********************************************************************************************************************
 *  INTERNAL FUNCTIONS
 *********************************************************************************************************************/

/*================================================ J1939NM_START_SEC_CODE ===========================================*/
#define J1939NM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  J1939Nm_InternalGetName
 *********************************************************************************************************************/
/*! \brief      Get a NAME from the configuration.
 *
 *  \param      nodeNameIdx     Index of NAME of internal or external node.
 *  \param      nodeName        Byte array where to store the NAME.
 *  \return     none
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/

FUNC(void, J1939NM_CODE) J1939Nm_InternalGetName(J1939Nm_NameType nodeName,
                                                 const J1939Nm_SizeOfNodeNameType nodeNameIdx)
{
  nodeName[J1939NM_NAME_BYTE8] = J1939Nm_GetNodeNameByte8OfNodeName(nodeNameIdx);
  nodeName[J1939NM_NAME_BYTE7] = J1939Nm_GetNodeNameByte7OfNodeName(nodeNameIdx);
  nodeName[J1939NM_NAME_BYTE6] = J1939Nm_GetNodeNameByte6OfNodeName(nodeNameIdx);
  nodeName[J1939NM_NAME_BYTE5] = J1939Nm_GetNodeNameByte5OfNodeName(nodeNameIdx);
  nodeName[J1939NM_NAME_BYTE4] = J1939Nm_GetNodeNameByte4OfNodeName(nodeNameIdx);
  nodeName[J1939NM_NAME_BYTE3] = J1939Nm_GetNodeNameByte3OfNodeName(nodeNameIdx);
  nodeName[J1939NM_NAME_BYTE2] = J1939Nm_GetNodeNameByte2OfNodeName(nodeNameIdx);
  nodeName[J1939NM_NAME_BYTE1] = J1939Nm_GetNodeNameByte1OfNodeName(nodeNameIdx);
}


#if (((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) && ((J1939NM_EXISTS_REMOTE_NODETYPEOFEXTERNALNODE == STD_ON) || (J1939NM_EXISTS_ANONYMOUS_NODETYPEOFEXTERNALNODE == STD_ON))) || (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON))
/**********************************************************************************************************************
 *  J1939Nm_InternalCopyName
 *********************************************************************************************************************/
/*! \brief      Copy a NAME.
 *
 *  \param      newNodeName     New NAME.
 *  \param      oldNodeName     Existing NAME.
 *  \return     none
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/

FUNC(void, J1939NM_CODE) J1939Nm_InternalCopyName(J1939Nm_NameType newNodeName, J1939Nm_ConstNameType oldNodeName)
{
  newNodeName[J1939NM_NAME_BYTE8] = oldNodeName[J1939NM_NAME_BYTE8];
  newNodeName[J1939NM_NAME_BYTE7] = oldNodeName[J1939NM_NAME_BYTE7];
  newNodeName[J1939NM_NAME_BYTE6] = oldNodeName[J1939NM_NAME_BYTE6];
  newNodeName[J1939NM_NAME_BYTE5] = oldNodeName[J1939NM_NAME_BYTE5];
  newNodeName[J1939NM_NAME_BYTE4] = oldNodeName[J1939NM_NAME_BYTE4];
  newNodeName[J1939NM_NAME_BYTE3] = oldNodeName[J1939NM_NAME_BYTE3];
  newNodeName[J1939NM_NAME_BYTE2] = oldNodeName[J1939NM_NAME_BYTE2];
  newNodeName[J1939NM_NAME_BYTE1] = oldNodeName[J1939NM_NAME_BYTE1];
}
#endif


#if ((J1939NM_USESADDRESSARBITRATIONOFCHANNEL == STD_ON) || (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON))
/**********************************************************************************************************************
 *  J1939Nm_InternalNameCompare
 *********************************************************************************************************************/
/*! \brief      Compare two NAMEs.
 *
 *  \param      ownNodeName     Current NAME of internal or external node.
 *  \param      otherNodeName   NAME received or sent with AC.
 *  \return     < 0             Other NAME is greater,
 *              = 0             Both NAMEs are equal,
 *              > 0             Own NAME is greater.
 *  \context    May be called from interrupt or task level.
 *********************************************************************************************************************/

FUNC(sint16_least, J1939NM_CODE) J1939Nm_InternalNameCompare(J1939Nm_ConstNameType ownNodeName,
                                                             J1939Nm_ConstNameType otherNodeName)
{
  sint16_least cmp;

  cmp = (sint16_least) (ownNodeName[J1939NM_NAME_BYTE8] - otherNodeName[J1939NM_NAME_BYTE8]);
  if (cmp == 0)
  {
    cmp = (sint16_least) (ownNodeName[J1939NM_NAME_BYTE7] - otherNodeName[J1939NM_NAME_BYTE7]);
    if (cmp == 0)
    {
      cmp = (sint16_least) (ownNodeName[J1939NM_NAME_BYTE6] - otherNodeName[J1939NM_NAME_BYTE6]);
      if (cmp == 0)
      {
        cmp = (sint16_least) (ownNodeName[J1939NM_NAME_BYTE5] - otherNodeName[J1939NM_NAME_BYTE5]);
        if (cmp == 0)
        {
          cmp = (sint16_least) (ownNodeName[J1939NM_NAME_BYTE4] - otherNodeName[J1939NM_NAME_BYTE4]);
          if (cmp == 0)
          {
            cmp = (sint16_least) (ownNodeName[J1939NM_NAME_BYTE3] - otherNodeName[J1939NM_NAME_BYTE3]);
            if (cmp == 0)
            {
              cmp = (sint16_least) (ownNodeName[J1939NM_NAME_BYTE2] - otherNodeName[J1939NM_NAME_BYTE2]);
              if (cmp == 0)
              {
                cmp = (sint16_least) (ownNodeName[J1939NM_NAME_BYTE1] - otherNodeName[J1939NM_NAME_BYTE1]);
              }
            }
          }
        }
      }
    }
  }

  return cmp;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#define J1939NM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*============================================== J1939NM_STOP_SEC_CODE ==============================================*/


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/*============================================== J1939NM_START_SEC_CODE =============================================*/
#define J1939NM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 *  J1939Nm_InitMemory
 *********************************************************************************************************************/

FUNC(void, J1939NM_CODE) J1939Nm_InitMemory(void)
{
  J1939Nm_ModuleInitialized = J1939NM_UNINIT;
}


/**********************************************************************************************************************
 *  J1939Nm_Init
 *********************************************************************************************************************/
/* \trace SPEC-2168256
 *********************************************************************************************************************/

FUNC(void, J1939NM_CODE) J1939Nm_Init(P2CONST(J1939Nm_ConfigType, AUTOMATIC, J1939NM_PBCFG) config)
{
#if (J1939NM_USE_DUMMY_STATEMENT == STD_ON)
  /* PRQA S 3112 1 */ /* MD_J1939Nm_14.2 */
  (void) config;
#endif

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168259, SPEC-2168285 */
  J1939Nm_AssertRet(J1939Nm_ModuleInitialized == J1939NM_UNINIT, J1939NM_SID_INIT, J1939NM_E_REINIT);

#if (J1939NM_USE_INIT_POINTER == STD_ON)
  J1939Nm_ConfigDataPtr = config;

  if (J1939Nm_ConfigDataPtr == NULL_PTR)
  {
# if (J1939NM_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
    EcuM_BswErrorHook(J1939NM_MODULE_ID, ECUM_BSWERROR_NULLPTR);
# endif
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_INIT, J1939NM_E_PARAM_POINTER);
    return;
  }

# if (J1939NM_FINALMAGICNUMBER == STD_ON)
  if (J1939Nm_GetFinalMagicNumber() != J1939NM_FINAL_MAGIC_NUMBER)
  {
#  if (J1939NM_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
    EcuM_BswErrorHook(J1939NM_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
#  endif
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_INIT, J1939NM_E_INIT_FAILED);
    return;
  }
# endif
#else
  /* \trace SPEC-2168285 */
  J1939Nm_Assert(config == NULL_PTR, J1939NM_SID_INIT, J1939NM_E_PARAM_POINTER);
#endif
  /* PRQA L:PC */

#if ((J1939NM_ADDRESSSTORAGEBLOCK == STD_ON) || (J1939NM_NAMESTORAGEBLOCK == STD_ON))
  {
    Std_ReturnType result;
    uint8 errorStatus;

    result = NvM_GetErrorStatus(NvMConf___MultiBlockRequest, &errorStatus);

    if ((result != E_OK) || (errorStatus == NVM_REQ_PENDING))
    {
      return;
    }

# if (J1939NM_ADDRESSSTORAGEBLOCK == STD_ON)
    result = NvM_GetErrorStatus(NvMConf_NvMBlockDescriptor_J1939Nm_CurrentNodeAddresses, &errorStatus);

    if ((result != E_OK) ||
        ((errorStatus != NVM_REQ_OK) && (errorStatus != NVM_REQ_NV_INVALIDATED) &&
         (errorStatus != NVM_REQ_INTEGRITY_FAILED)))
    {
      return;
    }

    if (errorStatus != NVM_REQ_OK)
    {
      (void) NvM_SetRamBlockStatus(NvMConf_NvMBlockDescriptor_J1939Nm_CurrentNodeAddresses, TRUE);
    }
# endif

# if (J1939NM_NAMESTORAGEBLOCK == STD_ON)
    result = NvM_GetErrorStatus(NvMConf_NvMBlockDescriptor_J1939Nm_CurrentNodeNames, &errorStatus);

    if ((result != E_OK) ||
        ((errorStatus != NVM_REQ_OK) && (errorStatus != NVM_REQ_NV_INVALIDATED) &&
         (errorStatus != NVM_REQ_INTEGRITY_FAILED)))
    {
      return;
    }

    if (errorStatus != NVM_REQ_OK)
    {
      (void) NvM_SetRamBlockStatus(NvMConf_NvMBlockDescriptor_J1939Nm_CurrentNodeNames, TRUE);
    }
# endif
  }
#endif

  {
    J1939Nm_SizeOfNodeType nodeIdx;

    for (nodeIdx = 0; nodeIdx < J1939Nm_GetSizeOfNode(); nodeIdx++)
    {
#if (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON)
      uint8 *currentNodeName = J1939Nm_GetCurrentNodeName(J1939Nm_GetCurrentNodeNameIdxOfNode(nodeIdx));
#else
      J1939Nm_NodeNameIdxOfNodeType nodeNameIdx = J1939Nm_GetNodeNameIdxOfNode(nodeIdx);
      uint8 currentNodeName[J1939NM_NAME_LENGTH];

      J1939Nm_InternalGetName(currentNodeName, nodeNameIdx);
#endif
      J1939Nm_SetRandomSeed(J1939Nm_GetRandomSeedIdxOfNode(nodeIdx),
                            currentNodeName[J1939NM_NAME_BYTE1] + currentNodeName[J1939NM_NAME_BYTE2] +
                            currentNodeName[J1939NM_NAME_BYTE3] + currentNodeName[J1939NM_NAME_BYTE4] +
                            currentNodeName[J1939NM_NAME_BYTE5] + currentNodeName[J1939NM_NAME_BYTE6] +
                            currentNodeName[J1939NM_NAME_BYTE7] + currentNodeName[J1939NM_NAME_BYTE8]);
    }
  }

  {
    J1939Nm_SizeOfNodeChannelType nodeChannelIdx;

    for (nodeChannelIdx = 0; nodeChannelIdx < J1939Nm_GetSizeOfNodeChannel(); nodeChannelIdx++)
    {
      J1939Nm_NodeChannelStateType *nodeChannelState =
        &J1939Nm_GetNodeChannelState(J1939Nm_GetNodeChannelStateIdxOfNodeChannel(nodeChannelIdx));

      nodeChannelState->AcPending = FALSE;
      nodeChannelState->AcDelayed = FALSE;
      /* \trace SPEC-2168263 */
      nodeChannelState->NodeState = J1939NM_NODESTATE_BUS_SLEEP;
    }
  }

  {
    J1939Nm_SizeOfChannelType channelIdx;

    for (channelIdx = 0; channelIdx < J1939Nm_GetSizeOfChannel(); channelIdx++)
    {
      J1939Nm_ChannelStateType *channelState = &J1939Nm_GetChannelState(channelIdx);

      channelState->NmMode = NM_MODE_BUS_SLEEP;
      channelState->NmState = NM_STATE_BUS_SLEEP;
      channelState->BusOff = FALSE;
    }
  }

  {
    J1939Nm_SizeOfTxPduStateType txPduStateIdx;

    for (txPduStateIdx = 0; txPduStateIdx < J1939Nm_GetSizeOfTxPduState(); txPduStateIdx++)
    {
      J1939Nm_GetTxPduState(txPduStateIdx).Locked = FALSE;
    }
  }

#if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) || (J1939NM_NAME_MANAGEMENT_SUPPORT == STD_ON))
  J1939Nm_DynamicInit();
#endif

  J1939Nm_ModuleInitialized = J1939NM_INIT;
} /* PRQA S 2006, 6010, 6030 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC */


/**********************************************************************************************************************
 *  J1939Nm_DeInit
 *********************************************************************************************************************/
/* \trace SPEC-2168257
 *********************************************************************************************************************/

FUNC(void, J1939NM_CODE) J1939Nm_DeInit(void)
{
  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168258, SPEC-2168285 */
  J1939Nm_Assert(J1939Nm_ModuleInitialized == J1939NM_INIT, J1939NM_SID_DEINIT, J1939NM_E_UNINIT);
  /* PRQA L:PC */

  J1939Nm_ModuleInitialized = J1939NM_UNINIT;
}


/**********************************************************************************************************************
 *  J1939Nm_GetVersionInfo
 *********************************************************************************************************************/

#if (J1939NM_VERSION_INFO_API == STD_ON)
FUNC(void, J1939NM_CODE) J1939Nm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, J1939NM_APPL_VAR) versionInfo)
{
  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168285 */
  J1939Nm_AssertRet(versionInfo != NULL_PTR, J1939NM_SID_GETVERSIONINFO, J1939NM_E_PARAM_POINTER);
  /* PRQA L:PC */

  versionInfo->vendorID = J1939NM_VENDOR_ID;
  versionInfo->moduleID = J1939NM_MODULE_ID;
  versionInfo->sw_major_version = J1939NM_SW_MAJOR_VERSION;
  versionInfo->sw_minor_version = J1939NM_SW_MINOR_VERSION;
  versionInfo->sw_patch_version = J1939NM_SW_PATCH_VERSION;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif


/**********************************************************************************************************************
 *  J1939Nm_MainFunction
 *********************************************************************************************************************/
/*! \brief      Main function of the J1939Nm. Used for scheduling purposes and timeout supervision.
 *
 *  \context    Only to be called from task level.
 *  \note       The main function must be called cyclically with a timing corresponding to the configured Main Function
 *              Period.
 *
 *  \trace SPEC-2168260
 *********************************************************************************************************************/

FUNC(void, J1939NM_CODE) J1939Nm_MainFunction(void)
{
  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_MAINFUNCTION, J1939NM_E_UNINIT);
    return;
  }

  {
    J1939Nm_SizeOfTxPduType txPduIdx;

    for (txPduIdx = 0; txPduIdx < J1939Nm_GetSizeOfTxPdu(); txPduIdx++)
    {
      J1939Nm_TxPduStateType *pduState = &J1939Nm_GetTxPduState(J1939Nm_GetTxPduStateIdxOfTxPdu(txPduIdx));

      if ((pduState->Locked) == TRUE)
      {
        --pduState->ConfTimeout;

        if (pduState->ConfTimeout == 0)
        {
#if (J1939NM_NM_PG_SUPPORT == STD_ON)
          if (J1939Nm_GetPduTypeOfTxPdu(txPduIdx) == J1939NM_NM_TX_PDU_PDUTYPEOFTXPDU)
          {
            J1939Nm_InternalDropPendingName();
          }
#endif

          /* \trace SPEC-2168279 */
          pduState->Locked = FALSE;
          J1939Nm_ReportDetError(J1939NM_SID_MAINFUNCTION, J1939NM_E_TIMEOUT_TXCONF);
        }
      }
    }
  }

#if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) || (J1939NM_NM_PG_SUPPORT == STD_ON))
  J1939Nm_DynamicMainFunction();
#endif

  {
    J1939Nm_SizeOfNodeType nodeIdx;

    for (nodeIdx = 0; nodeIdx < J1939Nm_GetSizeOfNode(); nodeIdx++)
    {
      J1939Nm_NodeChannelNodeIndEndIdxOfNodeType nodeChannelIndIdx;

      for (nodeChannelIndIdx = J1939Nm_GetNodeChannelNodeIndStartIdxOfNode(nodeIdx);
           nodeChannelIndIdx < J1939Nm_GetNodeChannelNodeIndEndIdxOfNode(nodeIdx); nodeChannelIndIdx++)
      {
        J1939Nm_NodeChannelNodeIndType nodeChannelIdx = J1939Nm_GetNodeChannelNodeInd(nodeChannelIndIdx);
        J1939Nm_ChannelIdxOfNodeChannelType channelIdx = J1939Nm_GetChannelIdxOfNodeChannel(nodeChannelIdx);
        J1939Nm_ChannelStateType *channelState = &J1939Nm_GetChannelState(channelIdx);
        NetworkHandleType comMChannel = J1939Nm_GetComMChannelIdOfChannel(channelIdx);
        J1939Nm_NodeChannelStateType *nodeChannelState =
          &J1939Nm_GetNodeChannelState(J1939Nm_GetNodeChannelStateIdxOfNodeChannel(nodeChannelIdx));

        if ((!channelState->BusOff) && (nodeChannelState->NodeState != J1939NM_NODESTATE_BUS_SLEEP))
        {
#if (J1939NM_NM_STATECHANGENOTIF == STD_ON)
          Nm_StateType oldState = channelState->NmState;
#endif

          if (nodeChannelState->NodeState == J1939NM_NODESTATE_CLAIMING)
          {
            --nodeChannelState->Timer;
            if (nodeChannelState->Timer == 0)
            {
              /* \trace SPEC-2168270 */
              nodeChannelState->NodeState = J1939NM_NODESTATE_NORMAL;
              channelState->NmState = NM_STATE_NORMAL_OPERATION;
#if (J1939NM_J1939NM_E_ADDRESS_LOST == STD_ON)
              /* \trace SPEC-2168286, SPEC-2168305 */
              Dem_ReportErrorStatus(J1939Nm_GetJ1939NM_E_ADDRESS_LOST(), DEM_EVENT_STATUS_PREPASSED);
#endif
              nodeChannelState->StatePending = TRUE;
            }
          }

          if (nodeChannelState->AcDelayed) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
          {
            /* AddressClaimed messages must not be sent until a pending address claimed message is due */
            nodeChannelState->AcPending = FALSE;

            if (nodeChannelState->Timer == 0)
            {
              /* \trace SPEC-2168271 */
              nodeChannelState->Timer = J1939Nm_InternalCalcDelay(nodeIdx, TRUE);
            }
            else
            {
              --nodeChannelState->Timer;

              if (nodeChannelState->Timer == 0)
              {
                /* \trace SPEC-2168271 */
                nodeChannelState->AcDelayed = FALSE;
                /* \trace SPEC-2168275, SPEC-2168276, SPEC-2168277 */
                nodeChannelState->AcPending = TRUE;
              }
            }
          }

          /* \trace SPEC-2168278 */
          if (nodeChannelState->AcPending) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
          {
            /* \trace SPEC-2168271 */
            uint8 nodeAddress = J1939NM_NULL_ADDRESS;

            if ((nodeChannelState->NodeState == J1939NM_NODESTATE_NORMAL) ||
                (nodeChannelState->NodeState == J1939NM_NODESTATE_CLAIMING) ||
                (nodeChannelState->NodeState == J1939NM_NODESTATE_PRE_CLAIMING) ||
                (nodeChannelState->NodeState == J1939NM_NODESTATE_PRE_NORMAL))
            {
#if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
              if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
              {
                nodeAddress =
                  J1939Nm_GetCurrentNodeAddress(J1939Nm_GetCurrentNodeAddressIdxOfNodeChannel(nodeChannelIdx));
              }
              else
#endif
              {
                nodeAddress = J1939Nm_GetNodeAddressOfNode(nodeIdx);
              }
            }

            /* \trace SPEC-2168271 */
            J1939Nm_InternalSendAc(channelIdx, nodeIdx, nodeAddress);
          }

#if (J1939NM_NM_STATECHANGENOTIF == STD_ON)
          {
            Nm_StateType newState = channelState->NmState;

            if (oldState != newState)
            {
              /* TODO: Reported for every node. Should be accumulated. */
              /* \trace SPEC-2168305 */
              Nm_StateChangeNotification(comMChannel, oldState, newState);
            }
          }
#endif
        }

        if (nodeChannelState->StatePending) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
        {
          Nm_StateType nmState = NM_STATE_OFFLINE;

          nodeChannelState->StatePending = FALSE;

          if (nodeChannelState->NodeState == J1939NM_NODESTATE_NORMAL)
          {
            nmState = NM_STATE_NORMAL_OPERATION;
          }

          /* \trace SPEC-2168305 */
          BswM_J1939Nm_StateChangeNotification(comMChannel, (uint8) nodeIdx, nmState);
        }
      }
    }
  }
} /* PRQA S 2006, 6010, 6030, 6050, 6080 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */


/**********************************************************************************************************************
 *  J1939Nm_RxIndication
 *********************************************************************************************************************/

FUNC(void, J1939NM_CODE) J1939Nm_RxIndication(PduIdType RxPduId,
                                              P2CONST(PduInfoType, AUTOMATIC, J1939NM_APPL_DATA) info)
{
  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_RXINDICATION, J1939NM_E_UNINIT);
    return;
  }

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168285, SPEC-2168287 */
  J1939Nm_AssertRet(RxPduId < J1939Nm_GetSizeOfRxPdu(), J1939NM_SID_RXINDICATION, J1939NM_E_INVALID_PDU_SDU_ID);
#if (J1939NM_INVALIDHNDOFRXPDU == STD_ON)
  J1939Nm_AssertRet(!J1939Nm_IsInvalidHndOfRxPdu(RxPduId), J1939NM_SID_RXINDICATION, J1939NM_E_INVALID_PDU_SDU_ID);
#endif
  /* \trace SPEC-2168285 */
  J1939Nm_AssertRet(info != NULL_PTR, J1939NM_SID_RXINDICATION, J1939NM_E_PARAM_POINTER);
  J1939Nm_AssertRet(info->SduDataPtr != NULL_PTR, J1939NM_SID_RXINDICATION, J1939NM_E_PARAM_POINTER);
  /* PRQA L:PC */

#if (J1939NM_CANIF_DLCCHECK == STD_OFF)
  if ((info->SduLength > J1939NM_AC_FRAME_LENGTH) && (info->SduLength <= J1939NM_AC_PDU_LENGTH))
#endif
  {
#if ((J1939NM_NM_STATECHANGENOTIF == STD_ON) || (J1939NM_USERCALLOUT == STD_ON) || (J1939NM_USESADDRESSARBITRATIONOFCHANNEL == STD_ON))
    J1939Nm_ChannelChannelIdxOfRxPduType channelIdx = J1939Nm_GetChannelChannelIdxOfRxPdu(RxPduId);
#endif
#if ((J1939NM_NM_STATECHANGENOTIF == STD_ON) || (J1939NM_DYNAMIC_ADDRESS_SUPPORT))
    J1939Nm_ChannelStateType *channelState = &J1939Nm_GetChannelState(channelIdx);
#endif

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
    /* Distinguish between AC and NM PDUs */
    if (J1939Nm_GetPduTypeOfRxPdu(RxPduId) == J1939NM_AC_RX_PDU_PDUTYPEOFRXPDU)
#endif
    {
#if ((J1939NM_USERCALLOUT == STD_ON) || (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON))
      uint8 sourceAddress = info->SduDataPtr[J1939NM_AC_SA_POS];
#endif
#if (J1939NM_NM_STATECHANGENOTIF == STD_ON)
      Nm_StateType oldState = channelState->NmState;
#endif

#if ((J1939NM_USERCALLOUT == STD_ON) || (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON))
      /* PRQA S 3109 2 */ /* MD_MSR_14.3 */
      /* \trace SPEC-2168285 */
      J1939Nm_AssertRet(J1939Nm_SA_Valid(sourceAddress), J1939NM_SID_RXINDICATION, J1939NM_E_INVALID_ADDRESS);
#endif

#if (J1939NM_USERCALLOUT == STD_ON)
      /* \trace SPEC-2168282 */
      J1939Nm_GetUserCallout()(J1939Nm_GetComMChannelIdOfChannel(channelIdx), sourceAddress, info->SduDataPtr);
#endif

#if (J1939NM_USESADDRESSARBITRATIONOFCHANNEL == STD_ON)
      /* \trace SPEC-2168280, SPEC-2168281 */
      if (J1939Nm_IsUsesAddressArbitrationOfChannel(channelIdx))
      {
# if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
        if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx) && (sourceAddress != J1939NM_NULL_ADDRESS))
        {
          J1939Nm_SetBusAddressAllocated(channelState, sourceAddress);
        }
# endif

        if (J1939Nm_InternalCheckAcCollision(channelIdx, info, J1939NM_INVALID_NODE)) /* PRQA S 3344 */ /* MD_J1939Nm_13.2 */
        {
          J1939Nm_ReportDetError(J1939NM_SID_RXINDICATION, J1939NM_E_DUPLICATE_NAME);
        }
# if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) && ((J1939NM_EXISTS_REMOTE_NODETYPEOFEXTERNALNODE == STD_ON) || (J1939NM_EXISTS_ANONYMOUS_NODETYPEOFEXTERNALNODE == STD_ON)))
        else
        {
          J1939Nm_DynamicAddressClaimedNotification(channelIdx, info, channelState);
        }
# endif
      }
#endif

#if (J1939NM_NM_STATECHANGENOTIF == STD_ON)
      {
        Nm_StateType newState = channelState->NmState;

        if (oldState != newState)
        {
          /* \trace SPEC-2168273, SPEC-2168305 */
          Nm_StateChangeNotification(J1939Nm_GetComMChannelIdOfChannel(channelIdx), oldState, newState);
        }
      }
#endif
    }
#if (J1939NM_NM_PG_SUPPORT == STD_ON)
    else
    {
      J1939Nm_InternalHandleNm(RxPduId, info);
    }
#endif
  }
#if (J1939NM_CANIF_DLCCHECK == STD_OFF)
  else
  {
    J1939Nm_ReportDetError(J1939NM_SID_RXINDICATION, J1939NM_E_INVALID_PDU_SIZE);
  }
#endif
} /* PRQA S 2006, 6010, 6030 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC */

/**********************************************************************************************************************
 *  J1939Nm_TxConfirmation
 *********************************************************************************************************************/

FUNC(void, J1939NM_CODE) J1939Nm_TxConfirmation(PduIdType TxPduId)
{
  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_TXCONFIRMATION, J1939NM_E_UNINIT);
    return;
  }

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168285, SPEC-2168287 */
  J1939Nm_AssertRet(TxPduId < J1939Nm_GetSizeOfTxPdu(), J1939NM_SID_TXCONFIRMATION, J1939NM_E_INVALID_PDU_SDU_ID);
#if (J1939NM_INVALIDHNDOFTXPDU == STD_ON)
  J1939Nm_AssertRet(!J1939Nm_IsInvalidHndOfTxPdu(TxPduId), J1939NM_SID_TXCONFIRMATION, J1939NM_E_INVALID_PDU_SDU_ID);
#endif
  /* PRQA L:PC */

  {
    J1939Nm_ChannelChannelIdxOfTxPduType channelIdx = J1939Nm_GetChannelChannelIdxOfTxPdu(TxPduId);
    J1939Nm_ChannelStateType *channelState = &J1939Nm_GetChannelState(channelIdx);
    J1939Nm_NodeChannelNodeIndEndIdxOfNodeType nodeChannelIndIdx;
    J1939Nm_TxPduStateType *pduState = &J1939Nm_GetTxPduState(J1939Nm_GetTxPduStateIdxOfTxPdu(TxPduId));

    pduState->Locked = FALSE;

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
    /* Distinguish between AC and NM PDUs */
    if (J1939Nm_GetPduTypeOfTxPdu(TxPduId) == J1939NM_AC_TX_PDU_PDUTYPEOFTXPDU)
#endif
    {
      for (nodeChannelIndIdx = J1939Nm_GetNodeChannelNodeIndStartIdxOfNode(pduState->NodeId);
           nodeChannelIndIdx < J1939Nm_GetNodeChannelNodeIndEndIdxOfNode(pduState->NodeId); nodeChannelIndIdx++)
      {
        J1939Nm_NodeChannelNodeIndType nodeChannelIdx = J1939Nm_GetNodeChannelNodeInd(nodeChannelIndIdx);

        if (J1939Nm_GetChannelIdxOfNodeChannel(nodeChannelIdx) == channelIdx)
        {
          J1939Nm_NodeChannelStateType *nodeChannelState =
            &J1939Nm_GetNodeChannelState(J1939Nm_GetNodeChannelStateIdxOfNodeChannel(nodeChannelIdx));

          nodeChannelState->AcPending = FALSE;

          if (nodeChannelState->NodeState == J1939NM_NODESTATE_PRE_CLAIMING)
          {
            nodeChannelState->NodeState = J1939NM_NODESTATE_CLAIMING;
            /* \trace SPEC-2168269 */
            nodeChannelState->Timer = J1939Nm_GetNodeStartupDelay();
          }

          if (nodeChannelState->NodeState == J1939NM_NODESTATE_PRE_NORMAL)
          {
            nodeChannelState->NodeState = J1939NM_NODESTATE_NORMAL;
            channelState->NmState = NM_STATE_NORMAL_OPERATION;
            /* \trace SPEC-2168266 */
            nodeChannelState->StatePending = TRUE;
          }

#if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
          if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
          {
            J1939Nm_PreviousNodeAddressIdxOfNodeChannelType previousAddressIdx =
              J1939Nm_GetPreviousNodeAddressIdxOfNodeChannel(nodeChannelIdx);
            uint8 currentAddress =
              J1939Nm_GetCurrentNodeAddress(J1939Nm_GetCurrentNodeAddressIdxOfNodeChannel(nodeChannelIdx));
            uint8 previousAddress = J1939Nm_GetPreviousNodeAddress(previousAddressIdx);

            if (currentAddress != previousAddress)
            {
              J1939Nm_SetPreviousNodeAddress(previousAddressIdx, currentAddress);

              if (currentAddress != J1939NM_NULL_ADDRESS)
              {
                CanIf_SetAddressTableEntry(J1939Nm_GetCanIfCtrlIdOfChannel(channelIdx),
                                           J1939Nm_GetNodeAddressOfNode(pduState->NodeId), currentAddress);
              }
            }
          }
#endif
          break;
        }
      }
    }

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
    else
    {
# if (J1939NM_NMQUEUE == STD_ON)
      J1939Nm_InternalHandleNmTxConfirmation(channelIdx);
# endif
    }
#endif

  }
} /* PRQA S 2006, 6010, 6030, 6080 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */


/**********************************************************************************************************************
 *  J1939Nm_RequestIndication
 *********************************************************************************************************************/

FUNC(void, J1939NM_CODE) J1939Nm_RequestIndication(uint8 node, NetworkHandleType channel, uint32 requestedPgn,
                                                   P2CONST(J1939Rm_ExtIdInfoType, AUTOMATIC, J1939NM_APPL_DATA)
                                                   extIdInfo, uint8 sourceAddress, uint8 destAddress, uint8 priority)
{
#if (J1939NM_USE_DUMMY_STATEMENT == STD_ON)
  /* PRQA S 3112 4 */ /* MD_J1939Nm_14.2 */
  (void) requestedPgn;
  (void) extIdInfo;
  (void) sourceAddress;
  (void) priority;
#endif

  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_REQUESTINDICATION, J1939NM_E_UNINIT);
    return;
  }

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168285, SPEC-2168302 */
  J1939Nm_AssertRet(node < J1939Nm_GetSizeOfNode(), J1939NM_SID_REQUESTINDICATION, J1939NM_E_INVALID_NODE);
  /* \trace SPEC-2168285, SPEC-2168288 */
  J1939Nm_AssertRet(J1939Nm_Channel_Valid(channel), J1939NM_SID_REQUESTINDICATION, J1939NM_E_INVALID_NETWORK_ID);
  /* \trace SPEC-2168285, SPEC-2168302 */
  J1939Nm_AssertRet(((requestedPgn == J1939NM_PGN_AC) ||
                     (requestedPgn == J1939NM_PGN_NM)), J1939NM_SID_REQUESTINDICATION, J1939NM_E_INVALID_PGN);
  /* \trace SPEC-2168285, SPEC-2168302 */
  J1939Nm_AssertRet(J1939Nm_SA_Valid(sourceAddress), J1939NM_SID_REQUESTINDICATION, J1939NM_E_INVALID_ADDRESS);
  J1939Nm_AssertRet(J1939Nm_DA_Valid(destAddress), J1939NM_SID_REQUESTINDICATION, J1939NM_E_INVALID_ADDRESS);
  /* \trace SPEC-2168285, SPEC-2168302 */
  J1939Nm_AssertRet(J1939Nm_Priority_Valid(priority), J1939NM_SID_REQUESTINDICATION, J1939NM_E_INVALID_PRIO);
  /* PRQA L:PC */

#if (J1939NM_NM_PG_SUPPORT == STD_ON)
  if (requestedPgn == J1939NM_PGN_AC)
#endif
  {
    if ((destAddress == J1939Nm_GetNodeAddressOfNode(node)) || (destAddress == J1939NM_BROADCAST_ADDRESS))
    {
      /* TODO: Use for channel refresh, reset request timer (see ALM+ issue 84935) */
      J1939Nm_ChannelIdxOfComMChannelType channelIdx = J1939Nm_GetChannelIdxOfComMChannel(channel);
      J1939Nm_NodeChannelNodeIndEndIdxOfNodeType nodeChannelIndIdx;

      for (nodeChannelIndIdx = J1939Nm_GetNodeChannelNodeIndStartIdxOfNode(node);
           nodeChannelIndIdx < J1939Nm_GetNodeChannelNodeIndEndIdxOfNode(node); nodeChannelIndIdx++)
      {
        J1939Nm_NodeChannelNodeIndType nodeChannelIdx = J1939Nm_GetNodeChannelNodeInd(nodeChannelIndIdx);

        if (channelIdx == J1939Nm_GetChannelIdxOfNodeChannel(nodeChannelIdx))
        {
          J1939Nm_NodeChannelStateType *nodeChannelState =
            &J1939Nm_GetNodeChannelState(J1939Nm_GetNodeChannelStateIdxOfNodeChannel(nodeChannelIdx));

          if (((destAddress == J1939NM_BROADCAST_ADDRESS) &&
               (nodeChannelState->NodeState != J1939NM_NODESTATE_WAIT_NETWORK)) ||
              ((destAddress == J1939Nm_GetNodeAddressOfNode(node)) &&
               ((nodeChannelState->NodeState == J1939NM_NODESTATE_NORMAL) ||
                (nodeChannelState->NodeState == J1939NM_NODESTATE_CLAIMING))))
          {
            uint8 nodeAddress = J1939NM_NULL_ADDRESS;

            if ((nodeChannelState->NodeState == J1939NM_NODESTATE_NORMAL) ||
                (nodeChannelState->NodeState == J1939NM_NODESTATE_CLAIMING) ||
                (nodeChannelState->NodeState == J1939NM_NODESTATE_PRE_CLAIMING) ||
                (nodeChannelState->NodeState == J1939NM_NODESTATE_PRE_NORMAL))
            {
#if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
              if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
              {
                nodeAddress =
                  J1939Nm_GetCurrentNodeAddress(J1939Nm_GetCurrentNodeAddressIdxOfNodeChannel(nodeChannelIdx));
              }
              else
#endif
              {
                nodeAddress = J1939Nm_GetNodeAddressOfNode(node);
              }
            }

            if (nodeAddress == J1939NM_NULL_ADDRESS)
            {
              /* \trace SPEC-2168284 */
              nodeChannelState->AcDelayed = TRUE;
            }
            else
            {
              /* \trace SPEC-2168275, SPEC-2168276, SPEC-2168277, SPEC-2168283 */
              nodeChannelState->AcPending = TRUE;
            }
          }

          break;
        }
      }
    }
  }
#if (J1939NM_NM_PG_SUPPORT == STD_ON)
  else
  {
    if (destAddress == J1939Nm_GetNodeAddressOfNode(node))
    {
      J1939Nm_InternalNmRequestIndication(J1939Nm_GetChannelIdxOfComMChannel(channel), node, sourceAddress,
                                          destAddress);
    }
  }
#endif
} /* PRQA S 2006, 6010, 6030, 6060, 6080 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */


/**********************************************************************************************************************
 *  J1939Nm_NetworkRequest
 *********************************************************************************************************************/

FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_NetworkRequest(CONST(NetworkHandleType, AUTOMATIC) channel)
{
  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_NETWORKREQUEST, J1939NM_E_UNINIT);
    return E_NOT_OK;
  }

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168285, SPEC-2168288 */
  J1939Nm_AssertRetVal(J1939Nm_Channel_Valid(channel), J1939NM_SID_NETWORKREQUEST, J1939NM_E_INVALID_NETWORK_ID);
  /* PRQA L:PC */

  {
    J1939Nm_ChannelIdxOfComMChannelType channelIdx = J1939Nm_GetChannelIdxOfComMChannel(channel);
    J1939Nm_ChannelStateType *channelState = &J1939Nm_GetChannelState(channelIdx);

    if (channelState->NmMode == NM_MODE_BUS_SLEEP)
    {
      channelState->NmMode = NM_MODE_NETWORK;
      /* \trace SPEC-2168267 */
      channelState->NmState = NM_STATE_OFFLINE;

#if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
      if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
      {
        uint8_least i;

        for (i = 0; i < J1939NM_BUSADDRESSSTATE_SIZE; i++)
        {
          channelState->BusAddressState[i] = J1939NM_BUSADDRESS_UNUSED;
        }

        /* TODO: Use J1939Nm_GetNodeStartupDelay in case of isobus channel (see ALM+ issue 79740) */
        channelState->Timer = J1939Nm_GetRequestResponseDelay();
        channelState->NetworkState = J1939NM_NETWORKSTATE_SCANNING;
        (void) J1939Rm_SendRequest(J1939RM_NULLADDRESS_USER_ID, channel, J1939NM_PGN_AC, NULL_PTR,
                                   J1939NM_BROADCAST_ADDRESS, J1939NM_DEFAULT_PRIORITY, FALSE);
      }
#endif

      {
        J1939Nm_NodeChannelChannelIndEndIdxOfChannelType nodeChannelIndIdx;

        for (nodeChannelIndIdx = J1939Nm_GetNodeChannelChannelIndStartIdxOfChannel(channelIdx);
             nodeChannelIndIdx < J1939Nm_GetNodeChannelChannelIndEndIdxOfChannel(channelIdx); nodeChannelIndIdx++)
        {
          J1939Nm_NodeChannelChannelIndType nodeChannelIdx = J1939Nm_GetNodeChannelChannelInd(nodeChannelIndIdx);
          J1939Nm_NodeChannelStateType *nodeChannelState =
            &J1939Nm_GetNodeChannelState(J1939Nm_GetNodeChannelStateIdxOfNodeChannel(nodeChannelIdx));
#if ((J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON) || ((J1939NM_USESADDRESSARBITRATIONOFCHANNEL == STD_ON) && (J1939NM_NODESTARTUPDELAYOFNODE == STD_ON)))
          J1939Nm_NodeIdxOfNodeChannelType nodeIdx = J1939Nm_GetNodeIdxOfNodeChannel(nodeChannelIdx);
# if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
          J1939Nm_NodeAddressSearchAlgorithmOfNodeType searchAlgorithm =
            J1939Nm_GetNodeAddressSearchAlgorithmOfNode(nodeIdx);
# endif
          uint8 nodeAddress = J1939Nm_GetNodeAddressOfNode(nodeIdx);
#endif

#if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
          if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
          {
            nodeAddress = J1939Nm_DynamicNetworkRequest(nodeIdx, nodeChannelIdx, channelState, nodeAddress);
          }

          /* check if request is needed first */
          if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx) &&
              ((searchAlgorithm == J1939NM_REQUEST_NODEADDRESSSEARCHALGORITHMOFNODE) ||
               (searchAlgorithm == J1939NM_ISO_GLOBAL_NODEADDRESSSEARCHALGORITHMOFNODE)))
          {
            /* \trace SPEC-2168264 */
            nodeChannelState->NodeState = J1939NM_NODESTATE_WAIT_NETWORK;
            nodeChannelState->StatePending = TRUE;
          }
          else
#endif
          {
#if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
            if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
            {
              J1939Nm_SetBusAddressAllocated(channelState, nodeAddress);
            }
#endif

#if (J1939NM_USESADDRESSARBITRATIONOFCHANNEL == STD_ON)
            /* \trace SPEC-2168264 */
            if (J1939Nm_IsUsesAddressArbitrationOfChannel(channelIdx))
            {
              /* \trace SPEC-2168275, SPEC-2168276, SPEC-2168277 */
              nodeChannelState->AcPending = TRUE;

# if (J1939NM_NODESTARTUPDELAYOFNODE == STD_ON)
              if (J1939Nm_IsNodeStartUpDelayOfNode(nodeIdx) && (nodeAddress >= J1939NM_MIN_DELAY_ADDRESS) &&
                  (nodeAddress <= J1939NM_MAX_DELAY_ADDRESS))
              {
                /* \trace SPEC-2168264, SPEC-2168265 */
                nodeChannelState->NodeState = J1939NM_NODESTATE_PRE_CLAIMING;
                nodeChannelState->StatePending = TRUE;
              }
              else
# endif
              {
                /* \trace SPEC-2168264, SPEC-2168266 */
                nodeChannelState->NodeState = J1939NM_NODESTATE_PRE_NORMAL;
# if (J1939NM_J1939NM_E_ADDRESS_LOST == STD_ON)
                /* \trace SPEC-2168286, SPEC-2168305 */
                Dem_ReportErrorStatus(J1939Nm_GetJ1939NM_E_ADDRESS_LOST(), DEM_EVENT_STATUS_PREPASSED);
# endif
              }
            }
            else
#endif
            {
              /* \trace SPEC-2168268 */
              channelState->NmState = NM_STATE_NORMAL_OPERATION;
              /* \trace SPEC-2168264 */
              nodeChannelState->NodeState = J1939NM_NODESTATE_NORMAL;
              nodeChannelState->StatePending = TRUE;
            }
          }
        }
      }

      /* \trace SPEC-2168264, SPEC-2168305 */
      Nm_NetworkMode(channel);

#if (J1939NM_NM_STATECHANGENOTIF == STD_ON)
      /* \trace SPEC-2168305 */
      Nm_StateChangeNotification(channel, NM_STATE_BUS_SLEEP, channelState->NmState);
#endif
    }
  }

  return E_OK;
} /* PRQA S 2006, 6010, 6030, 6080 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */


/**********************************************************************************************************************
 *  J1939Nm_NetworkRelease
 *********************************************************************************************************************/

FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_NetworkRelease(CONST(NetworkHandleType, AUTOMATIC) channel)
{
  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_NETWORKRELEASE, J1939NM_E_UNINIT);
    return E_NOT_OK;
  }

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168285, SPEC-2168288 */
  J1939Nm_AssertRetVal(J1939Nm_Channel_Valid(channel), J1939NM_SID_NETWORKRELEASE, J1939NM_E_INVALID_NETWORK_ID);
  /* PRQA L:PC */

  {
    J1939Nm_ChannelIdxOfComMChannelType channelIdx = J1939Nm_GetChannelIdxOfComMChannel(channel);
    J1939Nm_ChannelStateType *channelState = &J1939Nm_GetChannelState(channelIdx);

    if (channelState->NmMode == NM_MODE_NETWORK)
    {
#if (J1939NM_NM_STATECHANGENOTIF == STD_ON)
      Nm_StateType oldState = channelState->NmState;
#endif

      channelState->NmMode = NM_MODE_BUS_SLEEP;
      channelState->NmState = NM_STATE_BUS_SLEEP;
      channelState->NetworkState = J1939NM_NETWORKSTATE_DOWN;

      {
        J1939Nm_NodeChannelChannelIndEndIdxOfChannelType nodeChannelIndIdx;

        for (nodeChannelIndIdx = J1939Nm_GetNodeChannelChannelIndStartIdxOfChannel(channelIdx);
             nodeChannelIndIdx < J1939Nm_GetNodeChannelChannelIndEndIdxOfChannel(channelIdx); nodeChannelIndIdx++)
        {
          J1939Nm_NodeChannelChannelIndType nodeChannelIdx = J1939Nm_GetNodeChannelChannelInd(nodeChannelIndIdx);
          J1939Nm_NodeIdxOfNodeChannelType nodeIdx = J1939Nm_GetNodeIdxOfNodeChannel(nodeChannelIdx);
          J1939Nm_NodeChannelStateType *nodeChannelState =
            &J1939Nm_GetNodeChannelState(J1939Nm_GetNodeChannelStateIdxOfNodeChannel(nodeChannelIdx));

#if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
          if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
          {
            J1939Nm_SetPreviousNodeAddress(J1939Nm_GetPreviousNodeAddressIdxOfNodeChannel(nodeChannelIdx),
                                           J1939NM_NULL_ADDRESS);

            if ((nodeChannelState->NodeState == J1939NM_NODESTATE_NORMAL) ||
                (nodeChannelState->NodeState == J1939NM_NODESTATE_CLAIMING))
            {
              CanIf_ResetAddressTableEntry(J1939Nm_GetCanIfCtrlIdOfChannel(channelIdx),
                                           J1939Nm_GetNodeAddressOfNode(nodeIdx));
            }
          }
#endif

          /* \trace SPEC-2168274 */
          nodeChannelState->NodeState = J1939NM_NODESTATE_BUS_SLEEP;

          /* \trace SPEC-2168305 */
          BswM_J1939Nm_StateChangeNotification(channel, (uint8) nodeIdx, NM_STATE_BUS_SLEEP);
        }
      }

#if (J1939NM_DYNAMIC_ADDRESS_SUPPORT == STD_ON)
      if (J1939Nm_IsUsesDynamicAddressingOfChannel(channelIdx))
      {
# if ((J1939NM_EXISTS_REMOTE_NODETYPEOFEXTERNALNODE == STD_ON) || (J1939NM_EXISTS_ANONYMOUS_NODETYPEOFEXTERNALNODE == STD_ON))
        J1939Nm_DynamicNetworkRelease(channelIdx);
# endif

        channelState->NetworkState = J1939NM_NETWORKSTATE_DOWN;
      }
#endif

#if (J1939NM_NM_STATECHANGENOTIF == STD_ON)
      /* \trace SPEC-2168274, SPEC-2168305 */
      Nm_StateChangeNotification(channel, oldState, NM_STATE_BUS_SLEEP);
#endif

      /* \trace SPEC-2168274, SPEC-2168305 */
      Nm_BusSleepMode(channel);
    }
  }

  return E_OK;
} /* PRQA S 2006, 6010, 6080 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STMIF */


/**********************************************************************************************************************
 *  J1939Nm_GetState
 *********************************************************************************************************************/

FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_GetState(CONST(NetworkHandleType, AUTOMATIC) channel,
                                                    CONSTP2VAR(Nm_StateType, AUTOMATIC, J1939NM_APPL_VAR) stateP,
                                                    CONSTP2VAR(Nm_ModeType, AUTOMATIC, J1939NM_APPL_VAR) modeP)
{
  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_GETSTATE, J1939NM_E_UNINIT);
    return E_NOT_OK;
  }

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168285, SPEC-2168288 */
  J1939Nm_AssertRetVal(J1939Nm_Channel_Valid(channel), J1939NM_SID_GETSTATE, J1939NM_E_INVALID_NETWORK_ID);
  /* \trace SPEC-2168285 */
  J1939Nm_AssertRetVal(stateP != NULL_PTR, J1939NM_SID_GETSTATE, J1939NM_E_PARAM_POINTER);
  J1939Nm_AssertRetVal(modeP != NULL_PTR, J1939NM_SID_GETSTATE, J1939NM_E_PARAM_POINTER);
  /* PRQA L:PC */

  {
    J1939Nm_ChannelIdxOfComMChannelType channelIdx = J1939Nm_GetChannelIdxOfComMChannel(channel);
    J1939Nm_ChannelStateType *channelState = &J1939Nm_GetChannelState(channelIdx);

    *modeP = channelState->NmMode;
    *stateP = channelState->NmState;
  }

  return E_OK;
} /* PRQA S 2006, 6010 */ /* MD_MSR_14.7, MD_MSR_STPTH */


/**********************************************************************************************************************
 *  J1939Nm_GetBusOffDelay
 *********************************************************************************************************************/

FUNC(void, J1939NM_CODE) J1939Nm_GetBusOffDelay(NetworkHandleType channel,
                                                P2VAR(uint8, AUTOMATIC, J1939NM_APPL_VAR) delayCyclesPtr)
{
  uint8 maxDelayCycles = 0;

  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_GETBUSOFFDELAY, J1939NM_E_UNINIT);
    return;
  }

  /* PRQA S 3109 PC */ /* MD_MSR_14.3 */
  /* \trace SPEC-2168285 */
  J1939Nm_AssertRet(delayCyclesPtr != NULL_PTR, J1939NM_SID_GETBUSOFFDELAY, J1939NM_E_PARAM_POINTER);
  /* PRQA L:PC */

  if (J1939Nm_Channel_Valid(channel))
  {
    J1939Nm_ChannelIdxOfComMChannelType channelIdx = J1939Nm_GetChannelIdxOfComMChannel(channel);
    J1939Nm_NodeChannelChannelIndEndIdxOfChannelType nodeChannelIndIdx;

    J1939Nm_GetChannelState(channelIdx).BusOff = TRUE;

    for (nodeChannelIndIdx = J1939Nm_GetNodeChannelChannelIndStartIdxOfChannel(channelIdx);
         nodeChannelIndIdx < J1939Nm_GetNodeChannelChannelIndEndIdxOfChannel(channelIdx); nodeChannelIndIdx++)
    {
      J1939Nm_NodeChannelChannelIndType nodeChannelIdx = J1939Nm_GetNodeChannelChannelInd(nodeChannelIndIdx);
      uint8 delayCycles;

      /* \trace SPEC-2168262 */
      delayCycles = J1939Nm_InternalCalcDelay((uint8) J1939Nm_GetNodeIdxOfNodeChannel(nodeChannelIdx), FALSE);
      if (delayCycles > maxDelayCycles)
      {
        maxDelayCycles = delayCycles;
      }
    }
  }

  *delayCyclesPtr = maxDelayCycles;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/**********************************************************************************************************************
 *  J1939Nm_BusOffEnd
 *********************************************************************************************************************/

FUNC(void, J1939NM_CODE) J1939Nm_BusOffEnd(NetworkHandleType channel)
{
  /* \trace SPEC-2168258 */
  if (J1939Nm_ModuleInitialized == J1939NM_UNINIT)
  {
    /* \trace SPEC-2168285 */
    J1939Nm_ReportDetError(J1939NM_SID_BUSOFFEND, J1939NM_E_UNINIT);
    return;
  }

  if (J1939Nm_Channel_Valid(channel))
  {
    J1939Nm_GetChannelState(J1939Nm_GetChannelIdxOfComMChannel(channel)).BusOff = FALSE;
  }
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/**********************************************************************************************************************
 *  Dummy Functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  J1939Nm_CheckRemoteSleepIndication
 *********************************************************************************************************************/

/* *INDENT-OFF* */
/* PRQA S 3673 4 */ /* MD_J1939Nm_16.7 */
/* *INDENT-ON* */
FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_CheckRemoteSleepIndication(CONST(NetworkHandleType, AUTOMATIC) channel,
                                                                      CONSTP2VAR(boolean, AUTOMATIC,
                                                                                 J1939NM_APPL_VAR) nmRemoteSleepIndPtr)
{
#if (J1939NM_USE_DUMMY_STATEMENT == STD_ON)
  /* PRQA S 3112 2 */ /* MD_J1939Nm_14.2 */
  (void) channel;
  (void) nmRemoteSleepIndPtr;
#endif

  J1939Nm_ReportDetError(J1939NM_SID_CHECKREMOTESLEEPINDICATION, J1939NM_E_DUMMY_API);

  return E_NOT_OK;
}


/**********************************************************************************************************************
 *  J1939Nm_DisableCommunication
 *********************************************************************************************************************/

FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_DisableCommunication(CONST(NetworkHandleType, AUTOMATIC) channel)
{
#if (J1939NM_USE_DUMMY_STATEMENT == STD_ON)
  /* PRQA S 3112 1 */ /* MD_J1939Nm_14.2 */
  (void) channel;
#endif

  J1939Nm_ReportDetError(J1939NM_SID_DISABLECOMMUNICATION, J1939NM_E_DUMMY_API);

  return E_NOT_OK;
}


/**********************************************************************************************************************
 *  J1939Nm_EnableCommunication
 *********************************************************************************************************************/

FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_EnableCommunication(CONST(NetworkHandleType, AUTOMATIC) channel)
{
#if (J1939NM_USE_DUMMY_STATEMENT == STD_ON)
  /* PRQA S 3112 1 */ /* MD_J1939Nm_14.2 */
  (void) channel;
#endif

  J1939Nm_ReportDetError(J1939NM_SID_ENABLECOMMUNICATION, J1939NM_E_DUMMY_API);

  return E_NOT_OK;
}


/**********************************************************************************************************************
 *  J1939Nm_GetLocalNodeIdentifier
 *********************************************************************************************************************/

/* *INDENT-OFF* */
/* PRQA S 3673 4 */ /* MD_J1939Nm_16.7 */
/* *INDENT-ON* */
FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_GetLocalNodeIdentifier(CONST(NetworkHandleType, AUTOMATIC) channel,
                                                                  CONSTP2VAR(uint8, AUTOMATIC,
                                                                             J1939NM_APPL_VAR) nmNodeIdPtr)
{
#if (J1939NM_USE_DUMMY_STATEMENT == STD_ON)
  /* PRQA S 3112 2 */ /* MD_J1939Nm_14.2 */
  (void) channel;
  (void) nmNodeIdPtr;
#endif

  J1939Nm_ReportDetError(J1939NM_SID_GETLOCALNODEIDENTIFIER, J1939NM_E_DUMMY_API);

  return E_NOT_OK;
}


/**********************************************************************************************************************
 *  J1939Nm_GetNodeIdentifier
 *********************************************************************************************************************/

/* *INDENT-OFF* */
/* PRQA S 3673 3 */ /* MD_J1939Nm_16.7 */
/* *INDENT-ON* */
FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_GetNodeIdentifier(CONST(NetworkHandleType, AUTOMATIC) channel,
                                                             CONSTP2VAR(uint8, AUTOMATIC, J1939NM_APPL_VAR) nmNodeIdPtr)
{
#if (J1939NM_USE_DUMMY_STATEMENT == STD_ON)
  /* PRQA S 3112 2 */ /* MD_J1939Nm_14.2 */
  (void) channel;
  (void) nmNodeIdPtr;
#endif

  J1939Nm_ReportDetError(J1939NM_SID_GETNODEIDENTIFIER, J1939NM_E_DUMMY_API);

  return E_NOT_OK;
}


/**********************************************************************************************************************
 *  J1939Nm_GetPduData
 *********************************************************************************************************************/

/* *INDENT-OFF* */
/* PRQA S 3673 3 */ /* MD_J1939Nm_16.7 */
/* *INDENT-ON* */
FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_GetPduData(CONST(NetworkHandleType, AUTOMATIC) channel,
                                                      CONSTP2VAR(uint8, AUTOMATIC, J1939NM_APPL_VAR) nmPduDataPtr)
{
#if (J1939NM_USE_DUMMY_STATEMENT == STD_ON)
  /* PRQA S 3112 2 */ /* MD_J1939Nm_14.2 */
  (void) channel;
  (void) nmPduDataPtr;
#endif

  J1939Nm_ReportDetError(J1939NM_SID_GETPDUDATA, J1939NM_E_DUMMY_API);

  return E_NOT_OK;
}


/**********************************************************************************************************************
 *  J1939Nm_GetUserData
 *********************************************************************************************************************/

/* *INDENT-OFF* */
/* PRQA S 3673 3 */ /* MD_J1939Nm_16.7 */
/* *INDENT-ON* */
FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_GetUserData(CONST(NetworkHandleType, AUTOMATIC) channel,
                                                       CONSTP2VAR(uint8, AUTOMATIC, J1939NM_APPL_VAR) nmUserDataPtr)
{
#if (J1939NM_USE_DUMMY_STATEMENT == STD_ON)
  /* PRQA S 3112 2 */ /* MD_J1939Nm_14.2 */
  (void) channel;
  (void) nmUserDataPtr;
#endif

  J1939Nm_ReportDetError(J1939NM_SID_GETUSERDATA, J1939NM_E_DUMMY_API);

  return E_NOT_OK;
}


/**********************************************************************************************************************
 *  J1939Nm_PassiveStartUp
 *********************************************************************************************************************/

FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_PassiveStartUp(CONST(NetworkHandleType, AUTOMATIC) channel)
{
#if (J1939NM_USE_DUMMY_STATEMENT == STD_ON)
  /* PRQA S 3112 1 */ /* MD_J1939Nm_14.2 */
  (void) channel;
#endif

  J1939Nm_ReportDetError(J1939NM_SID_PASSIVESTARTUP, J1939NM_E_DUMMY_API);

  return E_NOT_OK;
}


/**********************************************************************************************************************
 *  J1939Nm_RepeatMessageRequest
 *********************************************************************************************************************/

FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_RepeatMessageRequest(CONST(NetworkHandleType, AUTOMATIC) channel)
{
#if (J1939NM_USE_DUMMY_STATEMENT == STD_ON)
  /* PRQA S 3112 1 */ /* MD_J1939Nm_14.2 */
  (void) channel;
#endif

  J1939Nm_ReportDetError(J1939NM_SID_REPEATMESSAGEREQUEST, J1939NM_E_DUMMY_API);

  return E_NOT_OK;
}


/**********************************************************************************************************************
 *  J1939Nm_RequestBusSynchronization
 *********************************************************************************************************************/

FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_RequestBusSynchronization(CONST(NetworkHandleType, AUTOMATIC) channel)
{
#if (J1939NM_USE_DUMMY_STATEMENT == STD_ON)
  /* PRQA S 3112 1 */ /* MD_J1939Nm_14.2 */
  (void) channel;
#endif

  J1939Nm_ReportDetError(J1939NM_SID_REQUESTBUSSYNCHRONIZATION, J1939NM_E_DUMMY_API);

  return E_NOT_OK;
}


/**********************************************************************************************************************
 *  J1939Nm_SetSleepReadyBit
 *********************************************************************************************************************/

FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_SetSleepReadyBit(CONST(NetworkHandleType, AUTOMATIC) channel,
                                                            CONST(boolean, AUTOMATIC) nmSleepReadyBit)
{
#if (J1939NM_USE_DUMMY_STATEMENT == STD_ON)
  /* PRQA S 3112 2 */ /* MD_J1939Nm_14.2 */
  (void) channel;
  (void) nmSleepReadyBit;
#endif

  J1939Nm_ReportDetError(J1939NM_SID_SETSLEEPREADYBIT, J1939NM_E_DUMMY_API);

  return E_NOT_OK;
}


/**********************************************************************************************************************
 *  J1939Nm_SetUserData
 *********************************************************************************************************************/

FUNC(Std_ReturnType, J1939NM_CODE) J1939Nm_SetUserData(CONST(NetworkHandleType, AUTOMATIC) channel,
                                                       CONSTP2CONST(uint8, AUTOMATIC, J1939NM_APPL_DATA) nmUserDataPtr)
{
#if (J1939NM_USE_DUMMY_STATEMENT == STD_ON)
  /* PRQA S 3112 2 */ /* MD_J1939Nm_14.2 */
  (void) channel;
  (void) nmUserDataPtr;
#endif

  J1939Nm_ReportDetError(J1939NM_SID_SETUSERDATA, J1939NM_E_DUMMY_API);

  return E_NOT_OK;
}

#define J1939NM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*================================================ J1939NM_STOP_SEC_CODE ============================================*/


/* Module specific MISRA deviations:

 MD_J1939Nm_13.2
      Reason:     No deviation: MISRA rule 13.2 explicitly allows to use Boolean checks when it is ensured that the tested variable is only used as a Boolean value.
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
