/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Eth_30_Vtt_LL.h
 *        \brief  Header file of lower layer part of the VTT Ethernet controller driver
 *
 *      \details  Provides declaration and definition of the interface accessed by core part of the component.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  David Fessler                 visfer        Vector Informatik GmbH
 *  Mark Harsch                   vismha        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  04.00.00  2015-01-16  visfer  ESCAN00080252 FEAT-254: Globale Umbennenung von VIP nach VTT
 *  04.01.00  2015-06-07  visfer  ESCAN00083015 Incorrect handling of multiple pending received frames
 *                                ESCAN00083047 VTT: DET Error on Ethernet Packet Reception during Startup
 *                                ESCAN00083173 Compiler error: incompatible redefinition of function Eth_Receive
 *  04.01.01  2015-10-19  visfer  ESCAN00084541 Module ID not according to AUTOSAR
 *                                ESCAN00086946 uint16 BufferIdx variable overflow on Transmit function
 *  04.02.00  2016-03-30  visfer  ESCAN00089105 osek.h should not be included directly
 *  04.03.00  2016-06-23  visfer  ESCAN00090267 When more than on RxBuffer container is configured the descriptor
 *                                              memory area can get corrupted (QoS usecase)
 *                                ESCAN00090626 Erroneous unlocking of multiple Tx buffers
 *  04.03.01  2016-07-26  visfer  -             -
 *  04.04.00  2016-11-15  visfer  ESCAN00092816 Compiler warning / error "Macro redefinition of ETH_PHYS_ADDR_LEN_BYTE"
 *  05.00.00  2016-11-15  mha/fer ESCAN00094441 Adapt for usage with DrvEth__core
 *  05.00.01  2017-04-01  visfer  ESCAN00094709 Add FrameId to Ethernet Frames
 *  06.00.00  2017-06-01  visfer  ESCAN00094960 Wrong data is transmitted when transmissions is triggered for several
 *                                              frames in a short time interval
 *                                ESCAN00095402 Compiler error: missing infixing at ETH_VAR_NOINIT
 *  06.00.01  2017-07-21  visfer  ESCAN00095997 Compiler warning: Several unreferenced formal parameter warnings
 *********************************************************************************************************************/

#if !defined(ETH_30_VTT_LL_H)
# define     ETH_30_VTT_LL_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Eth_30_Vtt_Types.h"
# include "Eth_30_Vtt_Lcfg.h"
# include "Eth_30_Vtt_LL_Cfg.h"
# include "Eth_30_Vtt_LL_Int.h"

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#ifndef ETH_30_VTT_INLINE
# define ETH_30_VTT_INLINE static INLINE
#endif

/* ETH software version */
# define ETH_30_VTT_SW_MAJOR_VERSION                                           (6U)
# define ETH_30_VTT_SW_MINOR_VERSION                                           (0U)
# define ETH_30_VTT_SW_PATCH_VERSION                                           (1U)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETH_30_VTT_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

ETH_30_VTT_INLINE void Eth_30_Vtt_LL_EnableMiiAccess(uint8 CtrlIdx);
ETH_30_VTT_INLINE void Eth_30_Vtt_LL_WriteMacAddressToRegister(uint8 CtrlIdx, P2CONST(uint8, AUTOMATIC, ETH_30_VTT_APPL_DATA)  PhysAddrPtr);

/***********************************************************************************************************************
 *  Eth_30_Vtt_RxIrqHdlr
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 **********************************************************************************************************************/
FUNC(void, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_RxIrqHdlr(
  uint8 CtrlIdx);

/***********************************************************************************************************************
 *  Eth_30_Vtt_LL_MainFunction
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 **********************************************************************************************************************/
FUNC(void, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_LL_MainFunction(void);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

ETH_30_VTT_INLINE void Eth_30_Vtt_LL_EnableMiiAccess(uint8 CtrlIdx)
{
 ETH_30_VTT_DUMMY_STATEMENT(CtrlIdx);
}

ETH_30_VTT_INLINE void Eth_30_Vtt_LL_WriteMacAddressToRegister(uint8 CtrlIdx, P2CONST(uint8, AUTOMATIC, ETH_30_VTT_APPL_DATA)  PhysAddrPtr)
{
 ETH_30_VTT_DUMMY_STATEMENT(CtrlIdx);
 ETH_30_VTT_DUMMY_STATEMENT(PhysAddrPtr);
}

ETH_30_VTT_INLINE uint8 Eth_30_Vtt_LL_GetTxDescrRingIdxFromPendingInterrupt(uint8 CtrlIdx)
{
  ETH_30_VTT_DUMMY_STATEMENT(CtrlIdx);
  return 0;
}

ETH_30_VTT_INLINE boolean Eth_30_Vtt_LL_IsTxDescriptorOwnedByDriver(uint8 CtrlIdx, uint8 DescrRingIdx, uint8 DescrIdx)
{
  ETH_30_VTT_DUMMY_STATEMENT(CtrlIdx);
  ETH_30_VTT_DUMMY_STATEMENT(DescrRingIdx);
  ETH_30_VTT_DUMMY_STATEMENT(DescrIdx);
  return Eth_30_Vtt_TxDescriptors[DescrIdx].SoftwareOwned;
}

ETH_30_VTT_INLINE void Eth_30_Vtt_LL_PrepareTxDescriptorForChecksumOffloading(uint8 CtrlIdx, uint8 DescrRingIdx, uint8 DescrIdx)
{
  ETH_30_VTT_DUMMY_STATEMENT(CtrlIdx);
  ETH_30_VTT_DUMMY_STATEMENT(DescrRingIdx);
  ETH_30_VTT_DUMMY_STATEMENT(DescrIdx);
}

ETH_30_VTT_INLINE void Eth_30_Vtt_LL_PrepareTxDescriptorForTransmission(uint8 CtrlIdx,
                                                  uint8 DescrRingIdx,
                                                  uint8 DescrIdx,
                                                  uint8 BufIdx,
                                                  uint16 LenByte,
                                                  uint8* BufferPtr)
                                                  {
                                                    ETH_30_VTT_DUMMY_STATEMENT(DescrRingIdx);
                                                    ETH_30_VTT_DUMMY_STATEMENT(DescrIdx);
                                                    ETH_30_VTT_DUMMY_STATEMENT(DescrRingIdx);
                                                    ETH_30_VTT_DUMMY_STATEMENT(BufIdx);
                                                    VttEth_OutputEthernetPacket(CtrlIdx, LenByte + ETH_30_VTT_HDR_LEN_BYTE, BufferPtr);
                                                  }

ETH_30_VTT_INLINE void Eth_30_Vtt_LL_TriggerTransmission(uint8 CtrlIdx, uint8 DescrRingIdx, uint8 DescriptorIdx)
{
  ETH_30_VTT_DUMMY_STATEMENT(CtrlIdx);
  ETH_30_VTT_DUMMY_STATEMENT(DescrRingIdx);
  Eth_30_Vtt_TxDescriptors[DescriptorIdx].SoftwareOwned = FALSE;
}

# define ETH_30_VTT_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* ETH_30_VTT_LL_H */

/**********************************************************************************************************************
 *  END OF FILE: Eth_30_Vtt_LL.h
 *********************************************************************************************************************/

