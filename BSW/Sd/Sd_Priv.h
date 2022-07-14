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
/**        \file  Sd_Priv.h
 *        \brief  Service Discovery source file
 *
 *      \details  Types definitions of Service Discovery (SD)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (SD_PRIV_H)
# define SD_PRIV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Sd.h"
# if (STD_ON == SD_DEM_EVENTS_CONFIGURED)
#  include "Dem.h"
# endif /* SD_DEM_EVENTS_CONFIGURED */
# include "SchM_Sd.h"

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
# define SD_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern P2CONST(Sd_ConfigType, SD_VAR_ZERO_INIT, SD_PBCFG) Sd_ConfigDataPtr;

# define SD_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef struct
{
  SD_P2CONST(PduInfoType)             PduInfoPtr;
  Sd_SizeOfAddressStorageType         AddressStorageIdx;
  boolean                             RebootDetected;
  boolean                             MulticastMessage;
} Sd_RxMessageInfoType;

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/
# define SD_BEGIN_CRITICAL_SECTION() SchM_Enter_Sd_SD_EXCLUSIVE_AREA_0()
# define SD_END_CRITICAL_SECTION() SchM_Exit_Sd_SD_EXCLUSIVE_AREA_0()

/* PRQA S 3458 BRACED_MACROS */ /* MD_MSR_19.4 */
/* PRQA S 3453 FCT_MACROS */ /* MD_MSR_19.7 */

# define Sd_Copy(TgtDataPtr, SrcDataPtr, LenByte)    IpBase_Copy( \
                                                      (SD_P2VAR(IpBase_CopyDataType))(TgtDataPtr), \
                                                      (SD_P2VAR(IpBase_CopyDataType))(SrcDataPtr), \
                                                      (LenByte))

/* GET macros */
# define SD_GET_UINT8(PduInfoPtr, Offset, Variable)      IPBASE_GET_UINT8((PduInfoPtr)->SduDataPtr, (Offset), (Variable))

# define SD_GET_UINT16(PduInfoPtr, Offset, Variable)     IPBASE_GET_UINT16((PduInfoPtr)->SduDataPtr, (Offset), (Variable))

# define SD_GET_UINT32(PduInfoPtr, Offset, Variable)     IPBASE_GET_UINT32((PduInfoPtr)->SduDataPtr, (Offset), (Variable))

# define SD_GET_UINT24(PduInfoPtr, Offset, Variable)     ((Variable) = (uint32)(((uint32)((PduInfoPtr)->SduDataPtr[(Offset) + 0]) << 16) | \
                                                                           ((uint32)((PduInfoPtr)->SduDataPtr[(Offset) + 1]) <<  8) | \
                                                                           ((uint32)((PduInfoPtr)->SduDataPtr[(Offset) + 2]))))

# define SD_GET_OPTION_TYPE(PduInfoPtr, OptionOffset, Variable)  ((Variable) = (Sd_OptionType)(PduInfoPtr)->SduDataPtr[(OptionOffset) + SD_OPTION_LENGTH_SIZE])

# define SD_GET_ENTRY_TYPE(PduInfoPtr, EntryOffset, Variable)  ((Variable) = (Sd_EntryType)((PduInfoPtr)->SduDataPtr[(EntryOffset)]))

# define SD_GET_L4PROTOCOL_IPV4_TYPE(PduInfoPtr, EntryOffset, Variable)  ((Variable) = (Sd_L4ProtocolType)(PduInfoPtr)->SduDataPtr[(EntryOffset) + SD_OPTION_IPV4_L4PROTO_OFFSET])

# define SD_GET_L4PROTOCOL_IPV6_TYPE(PduInfoPtr, EntryOffset, Variable)  ((Variable) = (Sd_L4ProtocolType)(PduInfoPtr)->SduDataPtr[(EntryOffset) + SD_OPTION_IPV6_L4PROTO_OFFSET])

# define SD_GET_NROFOPT(PduInfoPtr, EntryOffset, Nr1stOptions, Nr2ndOptions) \
          { \
            uint8 nrOfOpt; \
            SD_GET_UINT8((PduInfoPtr), (EntryOffset) + SD_ENTRY_NROFOPTS_OFFSET, nrOfOpt); \
            (Nr1stOptions) = nrOfOpt >> 4; \
            (Nr2ndOptions) = nrOfOpt & 0x0F; \
          }

# define Sd_CheckIfTimerIsInvalid(TIMER)                     ((SD_INV_TIMER_VALUE_32BIT == (TIMER).Seconds) && (SD_INV_TIMER_VALUE_16BIT == (TIMER).Milliseconds))
# define Sd_CheckIfTimerIsZero(TIMER)                        ((0 == (TIMER).Seconds) && (0 == (TIMER).Milliseconds))

# define Sd_EventHandlerUsesUdp(EVENTHANDLERIDX)             (Sd_GetEventHandlerUdpIdxOfEventHandler(EVENTHANDLERIDX) != SD_NO_EVENTHANDLERUDPIDXOFEVENTHANDLER)
# define Sd_EventHandlerUsesTcp(EVENTHANDLERIDX)             (Sd_GetEventHandlerTcpIdxOfEventHandler(EVENTHANDLERIDX) != SD_NO_EVENTHANDLERTCPIDXOFEVENTHANDLER)
# define Sd_EventHandlerUsesMulticast(EVENTHANDLERIDX)       (Sd_GetEventHandlerMulticastIdxOfEventHandler(EVENTHANDLERIDX) != SD_NO_EVENTHANDLERMULTICASTIDXOFEVENTHANDLER)

# define Sd_ConsumedEventGroupUsesUdp(EVENTGROUPIDX)         (Sd_GetUdpActRefOfConsumedEventgroup(EVENTGROUPIDX) != SOAD_INV_ROUT_GRP_ID)
# define Sd_ConsumedEventGroupUsesTcp(EVENTGROUPIDX)         (Sd_GetTcpActRefOfConsumedEventgroup(EVENTGROUPIDX) != SOAD_INV_ROUT_GRP_ID)
# define Sd_ConsumedEventGroupUsesMulticast(EVENTGROUPIDX)   (Sd_GetMulticastActRefOfConsumedEventgroup(EVENTGROUPIDX) != SOAD_INV_ROUT_GRP_ID)

/* PRQA L:FCT_MACROS */
/* PRQA L:BRACED_MACROS */

#endif /* SD_PRIV_H */

/**********************************************************************************************************************
 *  END OF FILE: Sd_Priv.h
 *********************************************************************************************************************/
