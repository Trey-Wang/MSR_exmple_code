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
 *         File:  Eth_30_Vtt_Int.h
 *      Project:  TcpIp-Stack
 *       Module:  Ethernet Driver for SPC58xx MAC
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
#ifndef ETH_30_VTT_INT_H
# define ETH_30_VTT_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Eth_30_Vtt_Cfg.h"
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define ETH_30_VTT_UNDEFINED_DESCRIPTOR_RING                                  (0xFFU)

# define ETH_30_VTT_UINT8_MAX                                                  (0xFFU)
# define ETH_30_VTT_UINT16_MAX                                                 (0xFFFFU)
# define ETH_30_VTT_UINT32_MAX                                                 (0xFFFFFFFFU)

#define ETH_30_VTT_ISR_NO_OS_SUPPORT                                           (0U)
#define ETH_30_VTT_ISR_CATEGORY_1                                              (1U)
#define ETH_30_VTT_ISR_CATEGORY_2                                              (2U)

# ifndef ETH_30_VTT_INLINE
#  define ETH_30_VTT_INLINE static INLINE
# endif
/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FUNCTION_MACROS */ /* MD_MSR_19.7 */
# define ETH_30_VTT_BEGIN_CRITICAL_SECTION_0() SchM_Enter_Eth_30_Vtt_ETH_30_VTT_EXCLUSIVE_AREA_0()
# define ETH_30_VTT_END_CRITICAL_SECTION_0()   SchM_Exit_Eth_30_Vtt_ETH_30_VTT_EXCLUSIVE_AREA_0()
# define ETH_30_VTT_BEGIN_CRITICAL_SECTION_1() SchM_Enter_Eth_30_Vtt_ETH_30_VTT_EXCLUSIVE_AREA_1()
# define ETH_30_VTT_END_CRITICAL_SECTION_1()   SchM_Exit_Eth_30_Vtt_ETH_30_VTT_EXCLUSIVE_AREA_1()

/* Configuration access macros utilized for both single and multiple controller configurations */
# define Eth_30_Vtt_VCfgGetTxBufferNum(CtrlIdx)                                (Eth_30_Vtt_TxBufferNum[CtrlIdx])
# define Eth_30_Vtt_VCfgGetTxMaxBufSize(CtrlIdx)                               (Eth_30_Vtt_TxMaxBufferSize[CtrlIdx])
# define Eth_30_Vtt_VCfgGetRxRing(CtrlIdx, RingIdx)                            (Eth_30_Vtt_Ctrl2RxRingMap[CtrlIdx][RingIdx])
# define Eth_30_Vtt_VCfgGetRxBufferNum(CtrlIdx, RingIdx)                       (Eth_30_Vtt_Ctrl2RxRingMap[CtrlIdx][RingIdx].SegmentNumber)
# define Eth_30_Vtt_VCfgGetTxState(CtrlIdx, BufIdx)                            (Eth_30_Vtt_UlTxConfState[CtrlIdx][BufIdx])
# define Eth_30_Vtt_VCfgGetCtrlNum()                                           (ETH_30_VTT_MAX_CTRLS_TOTAL)
# define Eth_30_Vtt_VCfgGetRxDescrCnt(CtrlIdx, RingIdx)                        (Eth_30_Vtt_Ctrl2RxRingMap[CtrlIdx][RingIdx].SegmentNumber)
# define Eth_30_Vtt_VCfgGetRxDescrRingCnt(CtrlIdx)                             (Eth_30_Vtt_CtrlCfg[CtrlIdx].RxDescriptorRingNum)
# define Eth_30_Vtt_VCfgGetRegVal(CtrlIdx, RegIdx)                             (Eth_30_Vtt_CtrlCfg[CtrlIdx].RegVals[RegIdx].RegValue)
# define Eth_30_Vtt_VCfgGetRegAddr(CtrlIdx, RegIdx)                            (Eth_30_Vtt_CtrlCfg[CtrlIdx].RegVals[RegIdx].RegOffset)
# define Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx)                                (Eth_30_Vtt_PhysSrcAddrs[CtrlIdx])
# define Eth_30_Vtt_VCfgGetPhysSrcAddrRomDefault(CtrlIdx)                      (Eth_30_Vtt_PhysSrcAddrsRom[CtrlIdx])
# define Eth_30_Vtt_VCfgGetRegBaseAddr(CtrlIdx)                                (Eth_30_Vtt_RegBaseAddr[CtrlIdx])
# define Eth_30_Vtt_VCfgGetMiiHwLoopMaxCycles(CtrlIdx)                         (Eth_30_Vtt_MiiHwLoopCycles[CtrlIdx])
# define Eth_30_Vtt_VCfgGetInitHwLoopMaxCycles(CtrlIdx)                        (Eth_30_Vtt_InitHwLoopCycles[CtrlIdx])
# define Eth_30_Vtt_VCfgGetRegData(CtrlIdx, RegIdx)                            (Eth_30_Vtt_CtrlCfg[CtrlIdx].RegVals[RegIdx])
# define Eth_30_Vtt_VCfgGetRegTotal(CtrlIdx)                                   (Eth_30_Vtt_CtrlCfg[CtrlIdx].RegNum)
# define Eth_30_Vtt_VCfgGetMiiModeMask(CtrlIdx)                                (Eth_30_Vtt_CtrlCfg[CtrlIdx].MiiModeMask)
# define Eth_30_Vtt_VCfgGetTxDescrRingCnt(CtrlIdx)                             (Eth_30_Vtt_CtrlCfg[CtrlIdx].TxDescriptorRingNum)
# define Eth_30_Vtt_VCfgGetTxDescrCnt(CtrlIdx, DescrRingIdx)                   (Eth_30_Vtt_TxBufferNum[CtrlIdx])
# define Eth_30_Vtt_VCfgGetCtrl2TxBufMap(CtrlIdx)                              (Eth_30_Vtt_Ctrl2TxBufferMap[0])
# define Eth_30_Vtt_VCfgGetUlTxConfState(CtrlIdx, BufIdx)                      (Eth_30_Vtt_UlTxConfState[CtrlIdx][BufIdx])
# define Eth_30_Vtt_VCfgGetTxBufferBusyTable(CtrlIdx, BufIdx)                  (Eth_30_Vtt_TxBufferBusyTable[CtrlIdx][BufIdx])
# define Eth_30_Vtt_VCfgTxDescriptorBufferMap(CtrlIdx, DescrRingIdx, BufIdx)   (Eth_30_Vtt_TxDescriptorBufferMap[CtrlIdx][DescrRingIdx][BufIdx])

# if (ETH_30_VTT_ENABLE_MAC_WRITE_ACCESS == STD_ON)
#  define Eth_VCfgGetMacAddressBlockId(CtrlIdx)                                    (Eth_CtrlMacAddrBlockIds[CtrlIdx].EthNvMBlockDesc)
#  define Eth_VCfgIsMacWriteAccess(CtrlIdx)                                        (Eth_CtrlMacAddrBlockIds[CtrlIdx].EthIsMacAddrWriteAccess)
# endif /* ETH_30_VTT_ENABLE_MAC_WRITE_ACCESS */
# define Eth_30_Vtt_VCfgGetDemErrorId(CtrlIdx)                                 (Eth_30_Vtt_DemEvents[CtrlIdx])

# define Eth_30_Vtt_VCfgGetTxBufferLen(CtrlIdx, BufIdx)                        (Eth_30_Vtt_CtrlCfg[CtrlIdx].TxBufferLen[BufIdx])
# define Eth_30_Vtt_VCfgGetTxBufferStart(CtrlIdx, BufIdx)                      (Eth_30_Vtt_CtrlCfg[CtrlIdx].TxBufferStart[BufIdx])
# define Eth_30_Vtt_VCfgGetPtpEgressTable(CtrlIdx, BufIdx)                     (Eth_30_Vtt_PtpEgressTable[CtrlIdx][BufIdx])

# define Eth_30_Vtt_VCfgGetPtpBuf2DescMap(CtrlIdx, BufIdx)                     (Eth_30_Vtt_PtpBuf2DescMap[CtrlIdx][BufIdx])
# define Eth_30_Vtt_VCfgGetPtpBuf2DescrRingMap(CtrlIdx, BufIdx)                (Eth_30_Vtt_PtpBuf2DescrRingMap[CtrlIdx][BufIdx])
# define Eth_30_Vtt_VCfgGetTxDescriptorTxReadyTable(CtrlIdx, RingIdx, BufIdx)  (Eth_30_Vtt_TxDescriptorTxReadyTable[CtrlIdx][RingIdx][BufIdx])
# define Eth_30_Vtt_VCfgIsQosEnabled(CtrlIdx)                              \
           (0U != (Eth_30_Vtt_CtrlCfg[CtrlIdx].Features & ETH_30_VTT_QOS_SUPPORT_MASK))
# define Eth_30_Vtt_VCfgIsPtpEnabled(CtrlIdx)                              \
           (0U != (Eth_30_Vtt_CtrlCfg[CtrlIdx].Features & ETH_30_VTT_PTP_SUPPORT_MASK))
# define Eth_30_Vtt_VCfgIsIpChecksumOffloadingEnabled(CtrlIdx)             \
           (0U != (Eth_30_Vtt_CtrlCfg[CtrlIdx].Features & ETH_30_VTT_IP_CHECK_OFF_MASK))
# define Eth_30_Vtt_VCfgIsProtocolChecksumOffloadingEnabled(CtrlIdx)       \
           (0U != (Eth_30_Vtt_CtrlCfg[CtrlIdx].Features & ETH_30_VTT_PROT_CHECK_OFF_MASK))
# define Eth_30_Vtt_VCfgIsMiiHwLoopCancelationEnabled(CtrlIdx)             \
           (0U != (Eth_30_Vtt_CtrlCfg[CtrlIdx].Features & ETH_30_VTT_MII_HW_LOOP_CANCELATION_MASK))
# define Eth_30_Vtt_VCfgIsInitHwLoopCancelationEnabled(CtrlIdx)            \
           (0U != (Eth_30_Vtt_CtrlCfg[CtrlIdx].Features & ETH_30_VTT_INIT_HW_LOOP_CANCELATION_MASK))

/* Ethernet controller register access */
# define Eth_30_Vtt_VCtrlRegReadValue(CtrlIdx, Reg)                        \
    (*((volatile P2VAR(uint32, AUTOMATIC, MSR_REGSPACE))((P2VAR(void, AUTOMATIC, MSR_REGSPACE)) \
        ((uint32)Eth_30_Vtt_VCfgGetRegBaseAddr(CtrlIdx)+(Reg)))))
# define Eth_30_Vtt_VCtrlMibReadValue(CtrlIdx, Reg)                        \
    (*((volatile P2VAR(uint32, AUTOMATIC, MSR_REGSPACE))((P2VAR(void, AUTOMATIC, MSR_REGSPACE)) \
        ((uint32)Eth_30_Vtt_VCfgGetCountersBaseAddr(CtrlIdx)+(Reg)))))
# define Eth_30_Vtt_VCtrlRegAddr(CtrlIdx, Reg)                             \
    (  (volatile P2VAR(uint32, AUTOMATIC, MSR_REGSPACE))((P2VAR(void, AUTOMATIC, MSR_REGSPACE)) \
        ((uint32)Eth_30_Vtt_VCfgGetRegBaseAddr(CtrlIdx)+(Reg))))
# define Eth_30_Vtt_VCtrlRegWriteValue(CtrlIdx, Reg, Value)                \
    (Eth_30_Vtt_VCtrlRegReadValue((CtrlIdx), (Reg))  = (Value))
# define Eth_30_Vtt_VCtrlRegSetBits(CtrlIdx, Reg, Bits)                    \
    (Eth_30_Vtt_VCtrlRegReadValue((CtrlIdx), (Reg)) |= (Bits))
# define Eth_30_Vtt_VCtrlRegResetBits(CtrlIdx, Reg, Bits)                  \
    (Eth_30_Vtt_VCtrlRegReadValue((CtrlIdx), (Reg)) &= ((uint32)(((uint32)0xFFFFFFFFu) ^(Bits))))
# define Eth_30_Vtt_VCtrlRegReadBits(CtrlIdx, Reg, Mask)                   \
    (Eth_30_Vtt_VCtrlRegReadValue((CtrlIdx), (Reg)) &  (Mask))
# define Eth_30_Vtt_VCtrlRegReadBitsShift(CtrlIdx, Reg, Mask, Shift)       \
    (((Eth_30_Vtt_VCtrlRegReadValue((CtrlIdx), (Reg))) & (Mask)) >> (Shift))
# define Eth_30_Vtt_VCtrlRegReadValueShiftdMask(CtrlIdx, Reg, Shift, Mask) \
    ((Eth_30_Vtt_VCtrlRegReadValue((CtrlIdx), (Reg)) >> (Shift)) & (Mask))
# define Eth_30_Vtt_VCtrlRegIsBitSet(CtrlIdx, Reg, Bit)                    \
    (Eth_VCtrlRegReadBits((CtrlIdx), (Reg), (Bit)) == (Bit))
# define Eth_30_Vtt_VRegReadValue(CtrlIdx, Reg)                            \
    (*((volatile P2VAR(uint32, AUTOMATIC, MSR_REGSPACE))((P2VAR(void, AUTOMATIC, MSR_REGSPACE))((uint32)(Reg)))))
# define Eth_30_Vtt_VRegSetBits(CtrlIdx, Reg, Bits)                        \
    (Eth_30_Vtt_VRegReadValue((CtrlIdx), (Reg)) |= (Bits))
# define Eth_30_Vtt_VRegResetBits(CtrlIdx, Reg, Bits)                      \
    (Eth_30_Vtt_VRegReadValue((CtrlIdx), (Reg)) &= ((uint32)(((uint32)0xFFFFFFFFu) ^(Bits))))
/* PRQA L:FUNCTION_MACROS */ /* MD_MSR_19.7 */

/* Diagnostic Event Manager */
# if ( ETH_30_VTT_DEM_ERROR_DETECT == STD_ON )
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
#  define ETH_30_VTT_DEM_REPORT_ERROR_STATUS_ETH_E_ACCESS(CtrlIdx) \
  (Dem_ReportErrorStatus(Eth_30_Vtt_VCfgGetDemErrorId(CtrlIdx), DEM_EVENT_STATUS_FAILED))
# else
/* PRQA S 3453 1 */ /* MD_MSR_19.7 */
#  define ETH_30_VTT_DEM_REPORT_ERROR_STATUS_ETH_E_ACCESS(CtrlIdx)
# endif
  /* ETH_30_VTT_DEM_ERROR_DETECT */

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
# define ETH_30_VTT_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
extern VAR(Eth_StateType, ETH_30_VTT_VAR_ZERO_INIT) Eth_30_Vtt_State;
extern VAR(Eth_ModeType, ETH_30_VTT_VAR_ZERO_INIT) Eth_30_Vtt_Mode[ETH_30_VTT_MAX_CTRLS_TOTAL];
# endif /* ETH_30_VTT_DEV_ERROR_DETECT */

# define ETH_30_VTT_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# endif /* ETH_30_VTT_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: Eth_30_Vtt_Int.h
 *********************************************************************************************************************/

