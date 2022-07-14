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
 *         File:  J1939Nm_Dynamic.h
 *       Module:  SAE J1939 Network Management (J1939Nm)
 *
 *  Description:  Header of the dynamic sub-module of the SAE J1939 Network Management module.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (J1939NM_DYNAMIC_H)
# define J1939NM_DYNAMIC_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "J1939Nm_Types.h"
# include "ComStack_Types.h"


/**********************************************************************************************************************
 *  INTERNAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*============================================== J1939NM_START_SEC_CODE =============================================*/
# define J1939NM_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  J1939Nm_DynamicInit
 **********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_DynamicInit(void);

/***********************************************************************************************************************
 *  J1939Nm_DynamicMainFunction
 **********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_DynamicMainFunction(void);

/***********************************************************************************************************************
 *  J1939Nm_DynamicFindFreeAddress
 **********************************************************************************************************************/
FUNC(uint8, J1939NM_CODE) J1939Nm_DynamicFindFreeAddress(uint8 startAddress, J1939Nm_SizeOfNodeType nodeIdx,
                                                         P2CONST(J1939Nm_ChannelStateType, AUTOMATIC,
                                                                 J1939NM_APPL_DATA) channelState);

/***********************************************************************************************************************
 *  J1939Nm_DynamicFindFreeAddress
 **********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_DynamicAddressClaimedNotification(J1939Nm_SizeOfChannelType channelIdx,
                                                                   P2CONST(PduInfoType, AUTOMATIC,
                                                                           J1939NM_APPL_DATA) info,
                                                                   J1939Nm_ChannelStateType * channelState);

/***********************************************************************************************************************
 *  J1939Nm_DynamicNetworkRequest
 **********************************************************************************************************************/
FUNC(uint8, J1939NM_CODE) J1939Nm_DynamicNetworkRequest(J1939Nm_SizeOfNodeType nodeIdx,
                                                        J1939Nm_SizeOfNodeChannelType nodeChannelIdx,
                                                        const J1939Nm_ChannelStateType * channelState,
                                                        uint8 nodeAddress);

/***********************************************************************************************************************
 *  J1939Nm_DynamicNetworkRelease
 **********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_DynamicNetworkRelease(J1939Nm_SizeOfChannelType channelIdx);

# if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalDropPendingName()
 *********************************************************************************************************************/
/*! \brief       Stops the assembly of a new NAME via the NM message.
 *  \details
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config
  *********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_InternalDropPendingName(void);
# endif

# if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalNmRequestIndication()
 *********************************************************************************************************************/
/*! \brief       Handle a incoming Request for the PGN 0x9300 from the J1939Rm.
 *  \details
 *  \param[in]   channel        ComMChannel index.
 *  \param[in]   node           Internal node index.
 *  \param[in]   sourceAddress  Source address.
 *  \param[in]   destAddress    Destination address.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config
  *********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_InternalNmRequestIndication(J1939Nm_ChannelIdxOfComMChannelType channel, uint8 node,
                                                             uint8 sourceAddress, uint8 destAddress);
# endif

# if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalHandleNm()
 *********************************************************************************************************************/
/*! \brief       Handle a incoming name management request.
 *  \details     Supported are the modes 0, 5, 6 and 7.
 *  \param[in]   RxPduId   Id of the received Rx Pdu.
 *  \param[in]   info      Pointer to the data.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config
  *********************************************************************************************************************/
FUNC(void, J1939NM_CODE) J1939Nm_InternalHandleNm(PduIdType RxPduId,
                                                  P2CONST(PduInfoType, AUTOMATIC, J1939NM_APPL_DATA) info);
# endif

# if (J1939NM_NM_PG_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  J1939Nm_InternalHandleNmTxConfirmation()
 *********************************************************************************************************************/
/*! \brief       This function handles the TxConfirmation for a transferred Name Management PG.
 *  \details     Supported are the modes 1, 2, 3 and 4.
 *  \param[in]   channelId   Internal channel id.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config
  *********************************************************************************************************************/
FUNC(void, J1939RM_CODE) J1939Nm_InternalHandleNmTxConfirmation(J1939Nm_ChannelChannelIdxOfTxPduType channelId);
# endif

# define J1939NM_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*============================================== J1939NM_STOP_SEC_CODE ==============================================*/

#endif /* J1939NM_DYNAMIC_H */

/**********************************************************************************************************************
 *  END OF FILE: J1939Nm_Dynamic.h
 *********************************************************************************************************************/
