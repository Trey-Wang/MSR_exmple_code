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
 *         File:  Eth_30_Vtt.c
 *      Project:  TcpIp-Stack
 *       Module:  Core part of Ethernet Driver for MAC
 *
 *  Description:  implementation
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#define ETH_30_VTT_SOURCE

/*lint -e438 */ /* Suppress ID438 because Config pointer only used in Post-Build Variant */
/*lint -e506 */ /* Suppress ID506 due to MD_MSR_14.1 */
/*lint -e537 */ /* Suppress ID537 due to MD_MSR_19.1 */
/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */
/* PRQA S 0777 EOF */ /* MD_MSR_5.1_777 */
/* PRQA S 0779 EOF */ /* MD_MSR_5.1_779 */
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Eth_30_Vtt.h"
#include "Eth_30_Vtt_Int.h"
#include "Eth_30_Vtt_LL_Cfg.h"
#include "Eth_30_Vtt_Cfg.h"
#include "Eth_30_Vtt_Lcfg.h"
#include "Eth_30_Vtt_LL.h"
#include "EthIf_Cbk.h"
#if (ETH_30_VTT_ENABLE_MAC_WRITE_ACCESS == STD_ON)
# include "NvM.h"
#endif
/* ETH_30_VTT_ENABLE_MAC_WRITE_ACCESS */
#if  (ETH_30_VTT_USE_PERIPHERAL_ACCESS_API == STD_ON)
# include "Os.h"
#endif
  /* ETH_30_VTT_PROTECTED_MODE */
#include "IpBase.h"
#if (ETH_30_VTT_DEV_ERROR_DETECT == STD_ON)
# include "Det.h"
#endif /* ETH_30_VTT_DEV_ERROR_DETECT */
#if (ETH_30_VTT_DEM_ERROR_DETECT == STD_ON)
# include "Dem.h"
#endif /* ETH_30_VTT_DEM_ERROR_DETECT */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#ifndef ETH_30_VTT_INLINE
# define ETH_30_VTT_INLINE static INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define ETH_30_VTT_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
VAR(Eth_StateType, ETH_30_VTT_VAR_NOINIT) Eth_30_Vtt_State;
VAR(Eth_ModeType, ETH_30_VTT_VAR_ZERO_INIT) Eth_30_Vtt_Mode[ETH_30_VTT_MAX_CTRLS_TOTAL];
#endif /* ETH_30_VTT_DEV_ERROR_DETECT */

#define ETH_30_VTT_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
#define ETH_30_VTT_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define ETH_30_VTT_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define ETH_30_VTT_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define ETH_30_VTT_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define ETH_30_VTT_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define ETH_30_VTT_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define ETH_30_VTT_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

ETH_30_VTT_INLINE FUNC(boolean, ETH_30_VTT_CODE) Eth_30_Vtt_IsBroadcast(
  P2CONST(uint8, AUTOMATIC, ETH_30_VTT_APPL_DATA) PhysAddrPtr);
#if (ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_IsZeroAddress
 **********************************************************************************************************************/
/*! \brief      Checks wheather the address is all-zero.
 *  \param[in]  PhysAddrPtr        Physical address
 *  \return     TRUE               : MAC is all-zero\n
 *              FALSE              : MAC is not all-zero
 *  \note       - Reentrant, synchronous\n
 **********************************************************************************************************************/
ETH_30_VTT_INLINE FUNC(boolean, ETH_30_VTT_CODE) Eth_30_Vtt_IsZeroAddress(
  P2CONST(uint8, AUTOMATIC, ETH_30_VTT_APPL_DATA) PhysAddrPtr);
#endif
#if (ETH_30_VTT_IMPLEMENTATION_SET_CONTROLLER_MODE == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_InitBuffersAndDescriptors
 **********************************************************************************************************************/
/*! \brief      Initializes the transmission and reception buffers, descriptors and structures
 *  \param[in]  CtrlIdx            Controller Index
 *  \return     void
 *  \note       - NOT Reentrant, synchronous\n
 **********************************************************************************************************************/
ETH_30_VTT_INLINE FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_InitBuffersAndDescriptors(uint8 CtrlIdx);
#endif /* ETH_30_VTT_IMPLEMENTATION_SET_CONTROLLER_MODE */
#if (ETH_30_VTT_IMPLEMENTATION_RECEIVE == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_IncRxReadPosition
 **********************************************************************************************************************/
/*! \brief      Increments the read position in a Rx ring
 *  \param[in]  CtrlIdx             CtrlIdx
 *  \param[in]  DescrRingIdx        Index of the Rx descriptor ring
 *  \note       Not Reentrant\n
 **********************************************************************************************************************/
ETH_30_VTT_INLINE FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_IncRxReadPosition(uint8 CtrlIdx, uint8 DescrRingIdx);
#endif /* ETH_30_VTT_IMPLEMENTATION_RECEIVE == STD_ON */

/***********************************************************************************************************************
 *  Eth_30_Vtt_IncTxDescRingCurProcPtr
 **********************************************************************************************************************/
/*! \brief      Increments the position of the CurPtr for a Tx ring
 *  \param[in]  CtrlIdx             CtrlIdx
 *  \param[in]  DescrRingIdx        Index of the Tx descriptor ring
 *  \note       Not Reentrant\n
 **********************************************************************************************************************/
ETH_30_VTT_INLINE FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_IncTxDescRingCurProcPtr(uint8  CtrlIdx, uint8 DescrRingIdx);

/***********************************************************************************************************************
 *  Eth_30_Vtt_IncTxDescRingCurFreePtr
 **********************************************************************************************************************/
/*! \brief      Increments the position of the FreePtr for a Tx ring
 *  \param[in]  CtrlIdx             CtrlIdx
 *  \param[in]  DescrRingIdx        Index of the Tx descriptor ring
 *  \note       Not Reentrant\n
 **********************************************************************************************************************/
ETH_30_VTT_INLINE FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_IncTxDescRingCurFreePtr(uint8 CtrlIdx, uint8 DescrRingIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/***********************************************************************************************************************
 *  Eth_30_Vtt_IncRxReadPosition
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
#if (ETH_30_VTT_IMPLEMENTATION_RECEIVE == STD_ON)
ETH_30_VTT_INLINE FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_IncRxReadPosition(uint8 CtrlIdx, uint8 DescrRingIdx)
{
  /* #10 Increment current read position in Rx descriptor ring */
  Eth_30_Vtt_RxReadPosition[CtrlIdx][DescrRingIdx]++;

  /* #20 Check for wraparound */
  if (Eth_30_Vtt_RxReadPosition[CtrlIdx][DescrRingIdx] >= Eth_30_Vtt_VCfgGetRxBufferNum(CtrlIdx, DescrRingIdx))
  {
    Eth_30_Vtt_RxReadPosition[CtrlIdx][DescrRingIdx] = 0;
  }
}
#endif /* ETH_30_VTT_IMPLEMENTATION_RECEIVE == STD_ON */

/***********************************************************************************************************************
 *  Eth_30_Vtt_IncTxDescRingCurProcPtr
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETH_30_VTT_INLINE FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_IncTxDescRingCurProcPtr(uint8  CtrlIdx, uint8 DescrRingIdx)
{
  /* #10 Increment current transmission position in Tx descriptor ring */
  Eth_30_Vtt_TxDescRingCurProcPos[CtrlIdx][DescrRingIdx]++;

  /* #20 Check for wraparound */
  if (Eth_30_Vtt_TxDescRingCurProcPos[CtrlIdx][DescrRingIdx] >= (Eth_30_Vtt_VCfgGetTxBufferNum(CtrlIdx)))
  {
    Eth_30_Vtt_TxDescRingCurProcPos[CtrlIdx][DescrRingIdx] = 0U;
  }
} /* Eth_30_Vtt_IncTxDescRingCurProcPtr() */

/***********************************************************************************************************************
 *  Eth_30_Vtt_IncTxDescRingCurFreePtr
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETH_30_VTT_INLINE FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_IncTxDescRingCurFreePtr(uint8 CtrlIdx, uint8 DescrRingIdx)
{
  /* #10 Increment free position in Tx descriptor ring */
  Eth_30_Vtt_TxDescRingFreePos[CtrlIdx][DescrRingIdx]++;

  /* #20 Check for wraparound */
  if (Eth_30_Vtt_TxDescRingFreePos[CtrlIdx][DescrRingIdx] >= (Eth_30_Vtt_VCfgGetTxBufferNum(CtrlIdx)))
  {
    Eth_30_Vtt_TxDescRingFreePos[CtrlIdx][DescrRingIdx] = 0U;
  }
} /* Eth_VIncTxDescRingCurFreePtr() */

/***********************************************************************************************************************
 *  Eth_30_Vtt_IsBroadcast
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETH_30_VTT_INLINE FUNC(boolean, ETH_30_VTT_CODE) Eth_30_Vtt_IsBroadcast(P2CONST(uint8, AUTOMATIC, ETH_30_VTT_APPL_DATA) PhysAddrPtr)
{
  boolean IsBroadcast = TRUE;
  uint8_least AddrIdx;

  /* #10 Iterate over all six bytes of a MAC address */
  for(AddrIdx = 0; AddrIdx < ETH_PHYS_ADDR_LEN_BYTE; AddrIdx++)
  {
    /* #100 Check whether byte is 0xFF (Broadcast) */
    if (PhysAddrPtr[(uint8)AddrIdx] != 0xFF)
    {
      IsBroadcast = FALSE;
      break;
    }
  }
  return IsBroadcast;
}

#if (ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_IsZeroAddress
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETH_30_VTT_INLINE FUNC(boolean, ETH_30_VTT_CODE) Eth_30_Vtt_IsZeroAddress(P2CONST(uint8, AUTOMATIC, ETH_30_VTT_APPL_DATA) PhysAddrPtr)
{
  boolean IsZeroAddress = TRUE;
  uint8_least AddrIdx;

  /* #10 Iterate over all six bytes of a MAC address */
  for(AddrIdx = 0; AddrIdx < ETH_PHYS_ADDR_LEN_BYTE; AddrIdx++)
  {
    /* #100 Check whether byte is zero */
    if (PhysAddrPtr[(uint8)AddrIdx] != 0x00)
    {
      IsZeroAddress = FALSE;
      break;
    }
  }
  return IsZeroAddress;
}
#endif /* ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_SET_CONTROLLER_MODE == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_InitBuffersAndDescriptors
 **********************************************************************************************************************/
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
/* PRQA S 3206 2 */ /* MD_Eth_30_Vtt_3206 */
ETH_30_VTT_INLINE FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_InitBuffersAndDescriptors(uint8 CtrlIdx)
{
  uint8_least DescIdx;
  uint8_least DescrRingIdx;

  /* #10 Loop over all Tx descriptor rings */
  for(DescrRingIdx = 0U; DescrRingIdx < Eth_30_Vtt_VCfgGetTxDescrRingCnt(CtrlIdx); DescrRingIdx++)
  {
    /* #100 Loop over all Tx descriptors in that ring */
    for(DescIdx = 0U; DescIdx < Eth_30_Vtt_VCfgGetTxDescrCnt(CtrlIdx, DescrRingIdx); DescIdx++)
    {
      /* #110 Initialize Tx descriptor */
      Eth_30_Vtt_LL_InitializeTxDescriptor(CtrlIdx, (uint8)DescrRingIdx, (uint8)DescIdx);
    }

    /* #20 Set state variables for ring positions to start values */
    Eth_30_Vtt_TxDescRingFreePos[CtrlIdx][(uint8)DescrRingIdx] = 0;
    Eth_30_Vtt_TxDescRingCurProcPos[CtrlIdx][(uint8)DescrRingIdx] = 0;
  }

  /* #30 Loop over all Rx descriptor rings */
  for(DescrRingIdx = 0U; DescrRingIdx < Eth_30_Vtt_VCfgGetRxDescrRingCnt(CtrlIdx); DescrRingIdx++)
  {
    /* #300 Loop over all Rx descriptors in that ring */
    for(DescIdx = 0; DescIdx < Eth_30_Vtt_VCfgGetRxDescrCnt(CtrlIdx, DescrRingIdx); DescIdx++)
    {
      /* #310 Initialize Rx descriptor */
       Eth_30_Vtt_LL_InitializeRxDescriptor(CtrlIdx, (uint8)DescrRingIdx, (uint8)DescIdx);
    }
    Eth_30_Vtt_RxReadPosition[CtrlIdx][(uint8)DescrRingIdx] = 0;
  }

  /* #40 Initialize HW specific global Descriptor state variables*/ /*lint -e{522} */
  Eth_30_Vtt_LL_InitDescriptorStateVariables(CtrlIdx);

  /* #50 Announce descriptor lists to hardware */
  Eth_30_Vtt_LL_SetDescriptorBaseAddresses(CtrlIdx);
}
#endif /* ETH_30_VTT_IMPLEMENTATION_SET_CONTROLLER_MODE */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
#if (ETH_30_VTT_IMPLEMENTATION_INIT_MEMORY == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_InitMemory
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_InitMemory( void )
{
#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  {
    /* #10 Initialize state variables for DET checks */
      Eth_30_Vtt_State = ETH_STATE_UNINIT;
  }
#endif
  /* ETH_30_VTT_DEV_ERROR_DETECT */
} /* Eth_30_Vtt_InitMemory() */
#endif /* ETH_30_VTT_IMPLEMENTATION_INIT_MEMORY == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_INIT == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_Init
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 3206 3 */ /* MD_Eth_30_Vtt_3206 */
FUNC(void, ETH_30_VTT_CODE)Eth_30_Vtt_Init(
  P2CONST(Eth_ConfigType, ETH_30_VTT_CONST, ETH_30_VTT_CONST) CfgPtr)
{
  uint8_least CtrlIdx;

  ETH_30_VTT_DUMMY_STATEMENT_CONST(CfgPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Loop over all available controllers */
  for(CtrlIdx = 0; CtrlIdx < ETH_30_VTT_MAX_CTRLS_TOTAL; CtrlIdx++)
  {
#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
    /* #100 Initialize state variables for DET checks */
    Eth_30_Vtt_State = ETH_STATE_INIT;
    Eth_30_Vtt_Mode[CtrlIdx] = ETH_MODE_DOWN;
#endif /* ETH_30_VTT_DEV_ERROR_DETECT */

    /* #110 Copy physical address from ROM to RAM variable */
    Eth_30_Vtt_VCfgGetPhysSrcAddr((uint8)CtrlIdx)[0] = Eth_30_Vtt_VCfgGetPhysSrcAddrRomDefault((uint8)CtrlIdx)[0];
    Eth_30_Vtt_VCfgGetPhysSrcAddr((uint8)CtrlIdx)[1] = Eth_30_Vtt_VCfgGetPhysSrcAddrRomDefault((uint8)CtrlIdx)[1];
    Eth_30_Vtt_VCfgGetPhysSrcAddr((uint8)CtrlIdx)[2] = Eth_30_Vtt_VCfgGetPhysSrcAddrRomDefault((uint8)CtrlIdx)[2];
    Eth_30_Vtt_VCfgGetPhysSrcAddr((uint8)CtrlIdx)[3] = Eth_30_Vtt_VCfgGetPhysSrcAddrRomDefault((uint8)CtrlIdx)[3];
    Eth_30_Vtt_VCfgGetPhysSrcAddr((uint8)CtrlIdx)[4] = Eth_30_Vtt_VCfgGetPhysSrcAddrRomDefault((uint8)CtrlIdx)[4];
    Eth_30_Vtt_VCfgGetPhysSrcAddr((uint8)CtrlIdx)[5] = Eth_30_Vtt_VCfgGetPhysSrcAddrRomDefault((uint8)CtrlIdx)[5];

    /* #20 Enable Mii Access so that Transceiver can be initialized during VSwitchInit */ /*lint -e{522} */
    Eth_30_Vtt_LL_EnableMiiAccess((uint8)CtrlIdx);
  }
} /* Eth_30_Vtt_Init() */
#endif /* ETH_30_VTT_CORE_IMPLEMENTATION_INIT == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_CONTROLLER_INIT == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_ControllerInit
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_ControllerInit(uint8 CtrlIdx, uint8 CfgIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 U8Idx;
  /* PRQA S 3197 1 */ /* MD_Eth_30_Vtt_3197 */
  Std_ReturnType RetVal = E_OK;
  uint8 errorId = ETH_30_VTT_E_NO_ERROR;
  uint16 HwLoopTimeout = 0;

  /* ----- Development Error Checks ------------------------------------- */
  CtrlIdx = Eth_30_Vtt_TransformToLocalCtrlIdx(CtrlIdx);

#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of input parameters */
  if(CtrlIdx >= ETH_30_VTT_MAX_CTRLS_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_CTRL_IDX;
  }
  else if(Eth_30_Vtt_State == ETH_STATE_UNINIT)
  {
    errorId = ETH_30_VTT_E_NOT_INITIALIZED;
  }
  else if(CfgIdx > ETH_30_VTT_MAX_CFGS_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_CONFIG;
  }
  else
#endif /* ETH_30_VTT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* trigger start of runtime measurement */
    Eth_30_Vtt_Rtm_Start(ControllerInit);

#if (ETH_30_VTT_ENABLE_PRE_CTRLINIT_CALL == STD_ON)
  Eth_30_Vtt_PreControllerInitCallout(CtrlIdx);
#endif /* ETH_30_VTT_ENABLE_PRE_CTRLINIT_CALL */

    /* #20 Hardware configuration which needs to be done before MAC reset */  /*lint -e{522} */
    Eth_30_Vtt_LL_PreResetMac(CtrlIdx);

    /* #30 do a soft reset if necessary */
    Eth_30_Vtt_LL_ResetMac(CtrlIdx);

    /* #40 wait for the reset to be finished */
    while (Eth_30_Vtt_LL_IsResetMacFinished(CtrlIdx) == FALSE)
    {
      /* still busy */
      if (HwLoopTimeout >  Eth_30_Vtt_ResetLoopCycles[CtrlIdx])
      {
        ETH_30_VTT_DEM_REPORT_ERROR_STATUS_ETH_E_ACCESS(CtrlIdx);
        RetVal = E_NOT_OK;
        break;
      }
      HwLoopTimeout++;
    }
    if (RetVal == E_OK)
    {
#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
      Eth_30_Vtt_Mode[CtrlIdx] = ETH_MODE_DOWN;
#endif

      /* #400 configure registers */
      for(U8Idx = 0; U8Idx < Eth_30_Vtt_VCfgGetRegTotal(CtrlIdx); U8Idx++)
      {
        /* PRQA S 0306 1 */ /* MD_Eth_30_Vtt_0306 */
        Eth_30_Vtt_VCtrlRegWriteValue(CtrlIdx, Eth_30_Vtt_VCfgGetRegAddr(CtrlIdx, U8Idx), Eth_30_Vtt_VCfgGetRegVal(CtrlIdx, U8Idx));
      }

      /* #410 Initialize all buffers and descriptors */
      Eth_30_Vtt_InitBuffersAndDescriptors(CtrlIdx);

      /* #420 Write mac address to MAC registers */
      Eth_30_Vtt_LL_WriteMacAddressToRegister(CtrlIdx, Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx));

      /* #430 Reset frame counters */ /*lint -e{522} */
      Eth_30_Vtt_LL_ResetCounters(CtrlIdx);

#if (ETH_30_VTT_ENABLE_PTP == STD_ON)
      /* Initialize global time of PTP submodule */
      Eth_30_Vtt_LL_StartTimesyncClock(CtrlIdx);
#endif /* ETH_30_VTT_ENABLE_PTP */

#if (ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER == STD_ON)
      /* #440 reset data all structures for mac address filtering */
      for(U8Idx = 0; U8Idx < ETH_30_VTT_MULTICAST_BUCKET_COUNTERS; U8Idx++)
      {
        Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][U8Idx] = 0;
      }
      Eth_30_Vtt_PromiscuousModeCounter[CtrlIdx] = 0;
#endif /* ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER */
    }

#if (ETH_30_VTT_ENABLE_POST_CTRLINIT_CALL == STD_ON)
  Eth_30_Vtt_PostControllerInitCallout(CtrlIdx);
#endif /* ETH_30_VTT_ENABLE_PRE_CTRLINIT_CALL */

    /* trigger stop of runtime measurement */
    Eth_30_Vtt_Rtm_Stop(ControllerInit);
  }
  /* ----- Development Error Report --------------------------------------- */
#if (ETH_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ETH_30_VTT_E_NO_ERROR)
  {
    (void)Det_ReportError(ETH_MODULE_ID, CtrlIdx, ETH_30_VTT_SID_CONTROLLER_INIT, errorId);
  }
#else
  ETH_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* ETH_30_VTT_DEV_ERROR_REPORT == STD_ON */
  return RetVal;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* ETH_30_VTT_IMPLEMENTATION_CONTROLLER_INIT == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_SET_CONTROLLER_MODE == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_SetControllerMode
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_SetControllerMode( uint8 CtrlIdx,
  Eth_ModeType CtrlMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETH_30_VTT_E_NO_ERROR;
  Std_ReturnType RetVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  CtrlIdx = Eth_30_Vtt_TransformToLocalCtrlIdx(CtrlIdx);
  
#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of input parameters */
  if(CtrlIdx >= ETH_30_VTT_MAX_CTRLS_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_CTRL_IDX;
  }
  else if(Eth_30_Vtt_State == ETH_STATE_UNINIT)
  {
    errorId = ETH_30_VTT_E_NOT_INITIALIZED;
  }
  else
#endif /* ETH_30_VTT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Decide whether the controller has to be switched to Active or Down */

    /* #200 Controller should be activated */
    if (CtrlMode == ETH_MODE_ACTIVE)
    {
      /* #2000 enable MAC */
      Eth_30_Vtt_LL_EnableMac(CtrlIdx);

      if ( Eth_30_Vtt_LL_IsMacEnabled(CtrlIdx) == FALSE)
      {
        /* #2010 error handling in case enabling the MAC failed */
        ETH_30_VTT_DEM_REPORT_ERROR_STATUS_ETH_E_ACCESS(CtrlIdx);
        RetVal = E_NOT_OK;
      }
      else
      {
        RetVal = E_OK;
      }
    }
    /* #300 Controller should be shut down */
    else
    {
      /* #3000 disable MAC */
      Eth_30_Vtt_LL_DisableMac(CtrlIdx);
      
      if ( Eth_30_Vtt_LL_IsMacEnabled(CtrlIdx) == TRUE)
      {
        /* #3010 error handling in case disabling the MAC failed */
        ETH_30_VTT_DEM_REPORT_ERROR_STATUS_ETH_E_ACCESS(CtrlIdx);
        RetVal = E_NOT_OK;
      }
      else
      {
        RetVal = E_OK;
      }
    }
#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
    if( RetVal == E_OK )
    {
      Eth_30_Vtt_Mode[CtrlIdx] = CtrlMode;
    }
#endif /* ETH_30_VTT_DEV_ERROR_DETECT */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (ETH_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ETH_30_VTT_E_NO_ERROR)
  {
    (void)Det_ReportError(ETH_MODULE_ID, CtrlIdx, ETH_30_VTT_SID_SET_CONTROLLER_MODE, errorId);
  }
#else
  ETH_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* ETH_30_VTT_DEV_ERROR_REPORT == STD_ON */
  return RetVal;
}
#endif /* ETH_30_VTT_IMPLEMENTATION_SET_CONTROLLER_MODE == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_GET_CONTROLLER_MODE == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_GetControllerMode
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_GetControllerMode( uint8 CtrlIdx,
  P2VAR(Eth_ModeType, AUTOMATIC, ETH_30_VTT_APPL_DATA) CtrlModePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETH_30_VTT_E_NO_ERROR;
  Std_ReturnType RetVal = E_NOT_OK;
  /* ----- Development Error Checks ------------------------------------- */
  CtrlIdx = Eth_30_Vtt_TransformToLocalCtrlIdx(CtrlIdx);
  
 #if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of input parameters */
  if(CtrlIdx >= ETH_30_VTT_MAX_CTRLS_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_CTRL_IDX;
  }
  else if(Eth_30_Vtt_State == ETH_STATE_UNINIT)
  {
    errorId = ETH_30_VTT_E_NOT_INITIALIZED;
  }
  else if(CtrlModePtr == NULL_PTR)
  {
    errorId = ETH_30_VTT_E_INV_POINTER;
  }
  else
 #endif /* ETH_30_VTT_DEV_ERROR_DETECT == STD_ON */
  /* ----- Implementation ----------------------------------------------- */
  {
    /* #100 read controller mode from HW register */
    /* PRQA S 0306 1 */ /* MD_Eth_30_Vtt_0306 */
    if (Eth_30_Vtt_LL_IsMacEnabled(CtrlIdx) == TRUE)
    {
      /* #1000 Report mode as active */
      *CtrlModePtr = ETH_MODE_ACTIVE;
    }
    else
    {
      /* #1010 Report mode as down */
      *CtrlModePtr = ETH_MODE_DOWN;
    }
    RetVal = E_OK;
  }
  /* ----- Development Error Report --------------------------------------- */
#if (ETH_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ETH_30_VTT_E_NO_ERROR)
  {
    (void)Det_ReportError(ETH_MODULE_ID, CtrlIdx, ETH_30_VTT_SID_GET_CONTROLLER_MODE, errorId);
  }
#else
  ETH_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* ETH_30_VTT_DEV_ERROR_REPORT == STD_ON */
  return RetVal;
}
#endif /* ETH_30_VTT_IMPLEMENTATION_GET_CONTROLLER_MODE == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_GET_PHYS_ADDR == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_GetPhysAddr
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_GetPhysAddr( uint8 CtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETH_30_VTT_APPL_DATA) PhysAddrPtr)

{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETH_30_VTT_E_NO_ERROR;

   /* ----- Development Error Checks ------------------------------------- */
  CtrlIdx = Eth_30_Vtt_TransformToLocalCtrlIdx(CtrlIdx);

 #if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of input parameters */
  if(CtrlIdx >= ETH_30_VTT_MAX_CTRLS_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_CTRL_IDX;
  }
  else if(Eth_30_Vtt_State == ETH_STATE_UNINIT)
  {
    errorId = ETH_30_VTT_E_NOT_INITIALIZED;
  }
  else if(PhysAddrPtr == NULL_PTR)
  {
    errorId = ETH_30_VTT_E_INV_POINTER;
  }
  else
#endif /* ETH_30_VTT_DEV_ERROR_DETECT == STD_ON */
  /* ----- Implementation ----------------------------------------------- */
  {
    /* #100 MAC address read from configuration */
    /* PRQA S 0310 2 */ /* MD_Eth_30_Vtt_0310 */
    PhysAddrPtr[0u] = Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx)[0];
    PhysAddrPtr[1u] = Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx)[1];
    PhysAddrPtr[2u] = Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx)[2];
    PhysAddrPtr[3u] = Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx)[3];
    PhysAddrPtr[4u] = Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx)[4];
    PhysAddrPtr[5u] = Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx)[5];
  }
/* ----- Development Error Report --------------------------------------- */
#if (ETH_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ETH_30_VTT_E_NO_ERROR)
  {
    (void)Det_ReportError(ETH_MODULE_ID, CtrlIdx, ETH_30_VTT_SID_GET_PHYS_ADDR, errorId);
  }
#else
  ETH_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* ETH_30_VTT_DEV_ERROR_REPORT == STD_ON */
} /* Eth_30_Vtt_GetPhysAddr() */
#endif /* ETH_30_VTT_IMPLEMENTATION_GET_PHYS_ADDR == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_SET_PHYS_ADDR == STD_ON)
/**********************************************************************************************************************
 *  Eth_30_Vtt_SetPhysAddr
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC (void, ETH_30_VTT_CODE) Eth_30_Vtt_SetPhysAddr( uint8 CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETH_30_VTT_APPL_DATA) PhysAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETH_30_VTT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  CtrlIdx = Eth_30_Vtt_TransformToLocalCtrlIdx(CtrlIdx);
  
#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of input parameters */
  if(CtrlIdx >= ETH_30_VTT_MAX_CTRLS_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_CTRL_IDX;
  }
  else if(Eth_30_Vtt_State == ETH_STATE_UNINIT)
  {
    errorId = ETH_30_VTT_E_NOT_INITIALIZED;
  }
  else if(PhysAddrPtr == NULL_PTR)
  {
    errorId = ETH_30_VTT_E_INV_POINTER;
  }
  else
#endif /* ETH_30_VTT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Set Physical Address RAM variables */
    Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx)[0] = PhysAddrPtr[0];
    Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx)[1] = PhysAddrPtr[1];
    Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx)[2] = PhysAddrPtr[2];
    Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx)[3] = PhysAddrPtr[3];
    Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx)[4] = PhysAddrPtr[4];
    Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx)[5] = PhysAddrPtr[5];

    /* #110 Write mac address to registers */
    Eth_30_Vtt_LL_WriteMacAddressToRegister(CtrlIdx, PhysAddrPtr);

#if (ETH_30_VTT_ENABLE_MAC_WRITE_ACCESS == STD_ON)
    /* #1110 Write to NVM if configured  */
    uint8 Status = NVM_REQ_PENDING;
    NvM_GetErrorStatus(Eth_30_Vtt_VCfgGetMacAddressBlockId(CtrlIdx), &Status);

    if(Status != NVM_REQ_PENDING)
    {
      /* set block Status to true */
      NvM_SetRamBlockStatus(Eth_30_Vtt_VCfgGetMacAddressBlockId(CtrlIdx), TRUE);
    }
#endif /* ETH_30_VTT_ENABLE_MAC_WRITE_ACCESS */
  }
  /* ----- Development Error Report --------------------------------------- */
  #if (ETH_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ETH_30_VTT_E_NO_ERROR)
  {
    (void)Det_ReportError(ETH_MODULE_ID, CtrlIdx, ETH_30_VTT_SID_SET_PHYS_ADDR, errorId);
  }
  #else
  ETH_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  #endif /* ETH_30_VTT_DEV_ERROR_REPORT == STD_ON */
} /* Eth_30_Vtt_SetPhysAddr() */
#endif /* ETH_30_VTT_IMPLEMENTATION_SET_PHYS_ADDR == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_UPDATE_PHYS_ADDR_FILTER == STD_ON)
/**********************************************************************************************************************
 *  Eth_30_Vtt_UpdatePhysAddrFilter
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
#if (ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER == STD_ON)
FUNC (Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_UpdatePhysAddrFilter( uint8 CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETH_30_VTT_APPL_DATA) PhysAddrPtr,
  Eth_FilterActionType Action)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 errorId = ETH_30_VTT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  CtrlIdx = Eth_30_Vtt_TransformToLocalCtrlIdx(CtrlIdx);
  
#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of input parameters */
  if(CtrlIdx >= ETH_30_VTT_MAX_CTRLS_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_CTRL_IDX;
  }
  else if(Eth_30_Vtt_State == ETH_STATE_UNINIT)
  {
    errorId = ETH_30_VTT_E_NOT_INITIALIZED;
  }
  else if(PhysAddrPtr == NULL_PTR)
  {
    errorId = ETH_30_VTT_E_INV_POINTER;
  }
  else
#endif /* ETH_30_VTT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Do HW specific filter mode settings for hash filtering */  /*lint -e{522} */
    Eth_30_Vtt_LL_SetMulticastFilteringMode(CtrlIdx);

    ETH_30_VTT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    /* #100 Check whether a special address defined by AUTOSAR is passed as parameter */

    if (TRUE == Eth_30_Vtt_IsBroadcast(PhysAddrPtr))
    {
      /* #1000 Broadcast address is passed: Promiscuous mode should be influenced */
      if(Action == ETH_ADD_TO_FILTER)
      {
        if(Eth_30_Vtt_PromiscuousModeCounter[CtrlIdx] < ETH_30_VTT_UINT8_MAX)
        {
          Eth_30_Vtt_PromiscuousModeCounter[CtrlIdx]++;
          RetVal = E_OK;

          if(Eth_30_Vtt_PromiscuousModeCounter[CtrlIdx] == 1)
          {
            /* Set promiscuous mode */
            Eth_30_Vtt_LL_EnablePromiscuousMode(CtrlIdx);
          }
        }
        else
        {
          /* Too much activations of promiscuous mode */
        }
      }
      else /* ETH_30_VTT_REMOVE_FROM_FILTER */
      {
        if(Eth_30_Vtt_PromiscuousModeCounter[CtrlIdx] > 0)
        {
          Eth_30_Vtt_PromiscuousModeCounter[CtrlIdx]--;
          RetVal = E_OK;

          if(Eth_30_Vtt_PromiscuousModeCounter[CtrlIdx] == 0)
          {
            /* Unset promiscuous mode */
            Eth_30_Vtt_LL_DisablePromiscuousMode(CtrlIdx);
          }
        }
        else
        {
          /* Tried to stop promiscuous mode, although it was not activated */
        }
      }
    }
    else if (Eth_30_Vtt_IsZeroAddress(PhysAddrPtr) == TRUE)
    {
      /* #1100 All zero address is passed: All adapted filters will be closed */
      uint8 BucketPos;
      /* Unset promiscuous mode */
      Eth_30_Vtt_LL_DisablePromiscuousMode(CtrlIdx);

      /* Close all multicast filters */
      Eth_30_Vtt_LL_CloseAllMulticastFilters(CtrlIdx);

      /* set all filter buckets to zero */
      for(BucketPos = 0; BucketPos < ETH_30_VTT_MULTICAST_BUCKET_COUNTERS; BucketPos++)
      {
        Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][BucketPos] = 0;
      }

      /* set no promiscuous mode requested */
      Eth_30_Vtt_PromiscuousModeCounter[CtrlIdx] = 0;

      RetVal = E_OK;
    }
    else
    {
      /* #1200 a mac address without special function defined by AUTOSAR should be manipulated */
      uint32 BucketMask;
      boolean IsUpperReg;
      uint8 hash;

      Eth_30_Vtt_LL_CalculateMulticastFilterRegisterHash(CtrlIdx, PhysAddrPtr, &hash, &IsUpperReg, &BucketMask);

      if(ETH_ADD_TO_FILTER == Action)
      {
        if (Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][hash] < ETH_30_VTT_BUCKET_MAX_CNT)
        {
          Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][hash]++;
          RetVal = E_OK;
        }

        if(Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][hash] == 1)
        {
          if (IsUpperReg == TRUE)
          {
            Eth_30_Vtt_LL_SetUpperMulticastFilterBucketRegister(CtrlIdx, BucketMask);
          }
          else
          {
            Eth_30_Vtt_LL_SetLowerMulticastFilterBucketRegister(CtrlIdx, BucketMask);
          }
          RetVal = E_OK;
        }
      }
      else /* ETH_30_VTT_REMOVE_FROM_FILTER */
      {
        if (Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][hash] > 0)
        {
          Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][hash]--;

          /* #1300 Set calculated filter bit in ctrl register */
          if (Eth_30_Vtt_MulticastBucketCounters[CtrlIdx][hash] == 0)
          {
            if (TRUE == IsUpperReg)
            {
              Eth_30_Vtt_LL_ResetUpperMulticastFilterBucketRegister(CtrlIdx, BucketMask);
            }
            else
            {
              Eth_30_Vtt_LL_ResetLowerMulticastFilterBucketRegister(CtrlIdx, BucketMask);
            }
          }
          RetVal = E_OK;
        }
      }
    }
    ETH_30_VTT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
  }
  /* ----- Development Error Report --------------------------------------- */
  #if (ETH_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ETH_30_VTT_E_NO_ERROR)
  {
    (void)Det_ReportError(ETH_MODULE_ID, CtrlIdx, ETH_30_VTT_SID_UPDATE_PHYS_ADDR_FILTER, errorId);
  }
  #else
  ETH_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  #endif /* ETH_30_VTT_DEV_ERROR_REPORT == STD_ON */
  return RetVal;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif
/* ETH_30_VTT_ENABLE_UPDATE_PHYS_ADDR_FILTER */
#endif /* ETH_30_VTT_IMPLEMENTATION_UPDATE_PHYS_ADDR_FILTER == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_WRITE_MII == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_WriteMii
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
#if ( ETH_30_VTT_MII_INTERFACE == STD_ON )
FUNC(Eth_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_WriteMii(
  uint8 CtrlIdx,
  uint8  TrcvIdx,
  uint8  RegIdx,
  uint16 RegVal)
{
  /* ----- Local Variables ---------------------------------------------- */

  uint8 errorId = ETH_30_VTT_E_NO_ERROR;
  Eth_ReturnType RetVal = E_NOT_OK;
  uint16 HwLoopTimeout = 0;

  /* ----- Development Error Checks ------------------------------------- */
  CtrlIdx = Eth_30_Vtt_TransformToLocalCtrlIdx(CtrlIdx);
  
#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of input parameters */
  if(CtrlIdx >= ETH_30_VTT_MAX_CTRLS_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_CTRL_IDX;
  }
  else if(Eth_30_Vtt_State == ETH_STATE_UNINIT)
  {
    errorId = ETH_30_VTT_E_NOT_INITIALIZED;
  }
  else if(RegIdx > ETH_30_VTT_MAX_MII_REG_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_PARAM;
  }
  else
#endif /* ETH_30_VTT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    ETH_30_VTT_BEGIN_CRITICAL_SECTION_1();

    /* #110 Write register value to MII */
    Eth_30_Vtt_LL_WriteMiiTrigger(CtrlIdx, TrcvIdx, RegIdx, RegVal);

    /* #120 wait for MII operation to finish  */
    /* PRQA S 0306 1 */ /* MD_Eth_30_Vtt_0306 */
    while ( Eth_30_Vtt_LL_IsMiiOperationPending(CtrlIdx) == TRUE )
    {
      /* still busy */
      if (HwLoopTimeout > Eth_30_Vtt_MiiLoopCycles[CtrlIdx])
      {
        ETH_30_VTT_DEM_REPORT_ERROR_STATUS_ETH_E_ACCESS(CtrlIdx);
        break;
      }
      HwLoopTimeout++;
    }

    if(HwLoopTimeout <= Eth_30_Vtt_MiiLoopCycles[CtrlIdx])
    {
      RetVal = E_OK;
    }
    ETH_30_VTT_END_CRITICAL_SECTION_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  /* ----- Development Error Report --------------------------------------- */
  #if (ETH_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ETH_30_VTT_E_NO_ERROR)
  {
   (void)Det_ReportError(ETH_MODULE_ID, CtrlIdx, ETH_30_VTT_SID_WRITE_MII, errorId);
  }
  #else
  ETH_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  #endif /* ETH_30_VTT_DEV_ERROR_REPORT == STD_ON */
  return RetVal;
}/* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif
/* ( ETH_30_VTT_MII_INTERFACE == STD_ON ) */
#endif /* ETH_30_VTT_IMPLEMENTATION_WRITE_MII == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_READ_MII == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_ReadMii
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
# if ( ETH_30_VTT_MII_INTERFACE == STD_ON )
FUNC(Eth_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_ReadMii( uint8 CtrlIdx,
  uint8                                   TrcvIdx,
  uint8                                   RegIdx,
  P2VAR(uint16, AUTOMATIC, ETH_30_VTT_APPL_DATA) RegValPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETH_30_VTT_E_NO_ERROR;
  Eth_ReturnType RetVal = E_NOT_OK;
  uint16 HwLoopTimeout = 0;

  /* ----- Development Error Checks ------------------------------------- */
  CtrlIdx = Eth_30_Vtt_TransformToLocalCtrlIdx(CtrlIdx);
  
#  if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of input parameters */
  if(CtrlIdx >= ETH_30_VTT_MAX_CTRLS_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_CTRL_IDX;
  }
  else if(Eth_30_Vtt_State == ETH_STATE_UNINIT)
  {
    errorId = ETH_30_VTT_E_NOT_INITIALIZED;
  }
  else if(RegIdx > ETH_30_VTT_MAX_MII_REG_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_PARAM;
  }
  else if(RegValPtr == NULL_PTR)
  {
    errorId = ETH_30_VTT_E_INV_POINTER;
  }
  else
#  endif /* ETH_30_VTT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    ETH_30_VTT_BEGIN_CRITICAL_SECTION_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #100 read MII */
    Eth_30_Vtt_LL_ReadMiiTrigger(CtrlIdx, TrcvIdx, RegIdx);

    /* #110 wait for MII operation to finish */
    /* PRQA S 0306 1 */ /* MD_Eth_30_Vtt_0306 */
    while ( Eth_30_Vtt_LL_IsMiiOperationPending(CtrlIdx) == TRUE )
    {
      if (HwLoopTimeout > Eth_30_Vtt_MiiLoopCycles[CtrlIdx])
      {
        ETH_30_VTT_DEM_REPORT_ERROR_STATUS_ETH_E_ACCESS(CtrlIdx);
        RetVal = ETH_E_NOT_OK;
        break;
      }
      HwLoopTimeout++;
    }

    if(HwLoopTimeout <= Eth_30_Vtt_MiiLoopCycles[CtrlIdx])
    {
      RetVal = E_OK;
    }
    /* read result */
    if (RetVal == E_OK)
    {
      /* PRQA S 0306 1 */ /* MD_Eth_30_Vtt_0306 */
      Eth_30_Vtt_LL_GetReadMiiResult(CtrlIdx, RegValPtr);
    }

    ETH_30_VTT_END_CRITICAL_SECTION_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  /* ----- Development Error Report --------------------------------------- */
#  if (ETH_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ETH_30_VTT_E_NO_ERROR)
  {
   (void)Det_ReportError(ETH_MODULE_ID, CtrlIdx, ETH_30_VTT_SID_READ_MII, errorId);
  }
#  else
  ETH_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  #endif /* ETH_30_VTT_DEV_ERROR_REPORT == STD_ON */
  return RetVal;
}/* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif /* ( ETH_30_VTT_MII_INTERFACE == STD_ON ) */
#endif /* ETH_30_VTT_IMPLEMENTATION_READ_MII == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_GET_COUNTER_STATE == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_GetCounterState
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
#if ( ETH_30_VTT_ENABLE_MIB_COUNTER == STD_ON )
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_GetCounterState( uint8 CtrlIdx,
  uint16                                  CtrOffs,
  P2VAR(uint32, AUTOMATIC, ETH_30_VTT_APPL_DATA) CtrValPtr)
{

  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETH_30_VTT_E_NO_ERROR;
  Std_ReturnType RetVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  CtrlIdx = Eth_30_Vtt_TransformToLocalCtrlIdx(CtrlIdx);
  
#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of input parameters */
  if(CtrlIdx >= ETH_30_VTT_MAX_CTRLS_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_CTRL_IDX;
  }
  else if(Eth_30_Vtt_State == ETH_STATE_UNINIT)
  {
    errorId = ETH_30_VTT_E_NOT_INITIALIZED;
  }
  else if(CtrValPtr == NULL_PTR)
  {
    errorId = ETH_30_VTT_E_INV_POINTER;
  }
  else
#endif /* ETH_30_VTT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Check number of counter to read for validity */
    if (Eth_30_Vtt_VCfgGetCountersTotal(CtrlIdx) > CtrOffs)
    {
      /* #1000 Read counter */
      /* PRQA S 0306 1 */ /* MD_Eth_30_Vtt_0306 */
      *CtrValPtr = Eth_30_Vtt_VCtrlMibReadValue(CtrlIdx, CtrOffs);
      RetVal = E_OK;
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (ETH_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ETH_30_VTT_E_NO_ERROR)
  {
   (void)Det_ReportError(ETH_MODULE_ID, CtrlIdx, ETH_30_VTT_SID_GET_COUNTER_STATE, errorId);
  }
#else
  ETH_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* ETH_30_VTT_DEV_ERROR_REPORT == STD_ON */
  return RetVal;
}
#endif
/* ETH_30_VTT_ENABLE_MIB_COUNTER */
#endif /* ETH_30_VTT_IMPLEMENTATION_GET_COUNTER_STATE == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_PROVIDE_TX_BUFFER == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_ProvideTxBuffer
 **********************************************************************************************************************/
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
FUNC(BufReq_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_ProvideTxBuffer( uint8 CtrlIdx,
  P2VAR(uint8,        AUTOMATIC, ETH_30_VTT_APPL_DATA) BufIdxPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETH_30_VTT_APPL_DATA) *BufPtr,
  P2VAR(uint16,       AUTOMATIC, ETH_30_VTT_APPL_DATA) LenBytePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETH_30_VTT_E_NO_ERROR;
  uint8 TxBufferBusyIdx;
  BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  CtrlIdx = Eth_30_Vtt_TransformToLocalCtrlIdx(CtrlIdx);
  
#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of input parameters */
  if(CtrlIdx >= ETH_30_VTT_MAX_CTRLS_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_CTRL_IDX;
  }
  else if(Eth_30_Vtt_State == ETH_STATE_UNINIT)
  {
    errorId = ETH_30_VTT_E_NOT_INITIALIZED;
  }
  else if(Eth_30_Vtt_Mode[CtrlIdx] == ETH_MODE_DOWN)
  {
    errorId = ETH_30_VTT_E_INV_MODE;
  }
  else if(BufIdxPtr == NULL_PTR)
  {
    errorId = ETH_30_VTT_E_INV_POINTER;
  }
  else if(BufPtr == NULL_PTR)
  {
    errorId = ETH_30_VTT_E_INV_POINTER;
  }
  else if(LenBytePtr == NULL_PTR)
  {
    errorId = ETH_30_VTT_E_INV_POINTER;
  }
  else
#endif /* ETH_30_VTT_DEV_ERROR_DETECT == STD_ON */
  {
    ETH_30_VTT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #100 Check requested buffer for valid size */
    if (Eth_30_Vtt_VCfgGetTxMaxBufSize(CtrlIdx) < (*LenBytePtr + ETH_30_VTT_HDR_LEN_BYTE))
    {
      RetVal = BUFREQ_E_OVFL;
    }
    else
    {
      /* ----- Implementation ----------------------------------------------- */

      /* #1000 Find smallest matching buffer, if available, by iterating over all transmission buffers*/
      for(TxBufferBusyIdx = 0; TxBufferBusyIdx < Eth_30_Vtt_VCfgGetTxBufferNum(CtrlIdx); TxBufferBusyIdx++)
      {
        if( ((uint8)ETH_30_VTT_BUFFER_NOT_BUSY == Eth_30_Vtt_VCfgGetTxBufferBusyTable(CtrlIdx, TxBufferBusyIdx))
            && ((*LenBytePtr + ETH_30_VTT_HDR_LEN_BYTE) <= Eth_30_Vtt_VCfgGetTxBufferLen(CtrlIdx, TxBufferBusyIdx)))
        {
          break;
        }
      }

      /* #2000 Check whether a matching buffer could be found */
      if(TxBufferBusyIdx < Eth_30_Vtt_VCfgGetTxBufferNum(CtrlIdx))
      {
        uint32 BufIdxU32 = Eth_30_Vtt_VCfgGetTxBufferStart(CtrlIdx, TxBufferBusyIdx) + ETH_30_VTT_HDR_LEN_BYTE;
        /* Divide by 4, Configuration tool ensures that it is 4 byte divisable */
        BufIdxU32 = BufIdxU32 / sizeof(Eth_DataType);

        /* #2010 Lock buffer and return BufferIdx, payload and length pointer in case of success */
        Eth_30_Vtt_VCfgGetTxBufferBusyTable(CtrlIdx, TxBufferBusyIdx) = (uint8)ETH_30_VTT_BUFFER_BUSY;
        *BufIdxPtr = TxBufferBusyIdx;
        *LenBytePtr = Eth_30_Vtt_VCfgGetTxBufferLen(CtrlIdx, TxBufferBusyIdx) - ETH_30_VTT_HDR_LEN_BYTE;
        *BufPtr = &(Eth_30_Vtt_VCfgGetCtrl2TxBufMap(CtrlIdx).U32[BufIdxU32]);
        RetVal = BUFREQ_OK;
      }
      else
      {
        /* #2020 Return BUFREQ_E_BUSY for the case, that no matching buffer could be found */
        RetVal = BUFREQ_E_BUSY;
      }
    }
    ETH_30_VTT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  /* ----- Development Error Report --------------------------------------- */
#if (ETH_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ETH_30_VTT_E_NO_ERROR)
  {
    (void)Det_ReportError(ETH_MODULE_ID, CtrlIdx, ETH_30_VTT_SID_PROVIDE_TX_BUFFER, errorId);
  }
#else
  ETH_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* ETH_30_VTT_DEV_ERROR_REPORT == STD_ON */
  return RetVal;
}/* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
#endif /* ETH_30_VTT_IMPLEMENTATION_PROVIDE_TX_BUFFER == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_TRANSMIT == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_Transmit
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_Transmit(uint8 CtrlIdx, uint8 BufIdx, Eth_FrameType FrameType, \
                                            boolean TxConfirmation, uint16 LenByte, \
                                            P2CONST(uint8, ETH_30_VTT_APPL_DATA, ETH_30_VTT_CONST) PhysAddrPtr)
{
  /* #10 Call Eth_30_Vtt_VTransmit() function with the ECUs own source MAC address as PhysAddrPtrSrc parameter */
  /* no transformation between local and global controller index necessary */
  return Eth_30_Vtt_VTransmit(CtrlIdx,
                       BufIdx,
                       FrameType,
                       TxConfirmation,
                       LenByte,
                       PhysAddrPtr,
                       Eth_30_Vtt_VCfgGetPhysSrcAddr(CtrlIdx));
} /* PRQA S 6060 */ /* MD_MSR_STPAR */
#endif /* ETH_30_VTT_IMPLEMENTATION_TRANSMIT == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_VTRANSMIT == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_VTransmit
 **********************************************************************************************************************/
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
FUNC(Std_ReturnType, ETH_30_VTT_CODE) Eth_30_Vtt_VTransmit( uint8 CtrlIdx,
  uint8                                    BufIdx,
  Eth_FrameType                            FrameType,
  boolean                                  TxConfirmation,
  uint16                                   LenByte,
  P2CONST(uint8, ETH_30_VTT_APPL_DATA, ETH_30_VTT_CONST) PhysAddrPtrDst,
  P2CONST(uint8, ETH_30_VTT_APPL_DATA, ETH_30_VTT_CONST) PhysAddrPtrSrc)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETH_30_VTT_E_NO_ERROR;
  uint32 BufferIdx;
  /* PRQA S 0759 1 */ /* MD_Eth_30_Vtt_0759 */
  Eth_30_Vtt_AlignedFrameCacheType BufferPtr;
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 DescrRingIdx = 0;
  uint8 CurPos;

  /* ----- Development Error Checks ------------------------------------- */
  CtrlIdx = Eth_30_Vtt_TransformToLocalCtrlIdx(CtrlIdx);
  
#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check parameters for plausibility */
  if(CtrlIdx >= ETH_30_VTT_MAX_CTRLS_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_CTRL_IDX;
  }
  else if(Eth_30_Vtt_State == ETH_STATE_UNINIT)
  {
    errorId = ETH_30_VTT_E_NOT_INITIALIZED;
  }
  else if(Eth_30_Vtt_Mode[CtrlIdx] == ETH_MODE_DOWN)
  {
    errorId = ETH_30_VTT_E_INV_MODE;
  }
  else if(BufIdx >= ETH_30_VTT_TX_BUF_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_PARAM;
  }
  else if(LenByte > (Eth_30_Vtt_VCfgGetTxBufferLen(CtrlIdx, BufIdx) - ETH_30_VTT_HDR_LEN_BYTE))
  {
    errorId = ETH_30_VTT_E_INV_PARAM;
  }
  else if(PhysAddrPtrDst == NULL_PTR)
  {
    errorId = ETH_30_VTT_E_INV_POINTER;
  }
  else
#endif /* ETH_30_VTT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    ETH_30_VTT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #100 Check whether buffer is locked for transmission */
    if(Eth_30_Vtt_VCfgGetTxBufferBusyTable(CtrlIdx, BufIdx) == (uint8)ETH_30_VTT_BUFFER_BUSY)
    {
      /* #1000 Check whether transmission length is zero (buffer release)*/
      if ( LenByte == 0U )
      { /* #1010 length zero: release transmit buffer only */
        Eth_30_Vtt_VCfgGetTxBufferBusyTable(CtrlIdx, BufIdx) = (uint8)ETH_30_VTT_BUFFER_NOT_BUSY;

        RetVal = E_OK;
      }
      else
      {
        /* #1020 length greater than zero: perpare buffer for transmission */
        /* set buffer index and pointer */
        BufferIdx = (uint32)Eth_30_Vtt_VCfgGetTxBufferStart(CtrlIdx, BufIdx);
        BufferPtr.U8 = &Eth_30_Vtt_VCfgGetCtrl2TxBufMap(CtrlIdx).U8[BufferIdx];

        /* Assemble Ethernet Header */
        /* Physical destination address */
        BufferPtr.U8[ETH_30_VTT_TX_DST_OFFSET_U8 + 0U] = PhysAddrPtrDst[0];
        BufferPtr.U8[ETH_30_VTT_TX_DST_OFFSET_U8 + 1U] = PhysAddrPtrDst[1];
        BufferPtr.U8[ETH_30_VTT_TX_DST_OFFSET_U8 + 2U] = PhysAddrPtrDst[2];
        BufferPtr.U8[ETH_30_VTT_TX_DST_OFFSET_U8 + 3U] = PhysAddrPtrDst[3];
        BufferPtr.U8[ETH_30_VTT_TX_DST_OFFSET_U8 + 4U] = PhysAddrPtrDst[4];
        BufferPtr.U8[ETH_30_VTT_TX_DST_OFFSET_U8 + 5U] = PhysAddrPtrDst[5];

        /* Physical source address */
        BufferPtr.U8[ETH_30_VTT_TX_SRC_OFFSET_U8 + 0U] = PhysAddrPtrSrc[0];
        BufferPtr.U8[ETH_30_VTT_TX_SRC_OFFSET_U8 + 1U] = PhysAddrPtrSrc[1];
        BufferPtr.U8[ETH_30_VTT_TX_SRC_OFFSET_U8 + 2U] = PhysAddrPtrSrc[2];
        BufferPtr.U8[ETH_30_VTT_TX_SRC_OFFSET_U8 + 3U] = PhysAddrPtrSrc[3];
        BufferPtr.U8[ETH_30_VTT_TX_SRC_OFFSET_U8 + 4U] = PhysAddrPtrSrc[4];
        BufferPtr.U8[ETH_30_VTT_TX_SRC_OFFSET_U8 + 5U] = PhysAddrPtrSrc[5];

        /* Frame type */
        BufferPtr.U16[ETH_30_VTT_TX_TYPE_OFFSET_U16] = IPBASE_HTON16(FrameType);

        /* #1030 Find the configured ring for transmission */
#if(ETH_30_VTT_ENABLE_QOS == STD_ON)
        if (Eth_30_Vtt_VCfgIsQosEnabled(CtrlIdx) == TRUE)
        {
          uint8 MappingFound = FALSE;

          /* if frame is VLAN tagged schedule transmission with descriptor ring selected for the VLAN priority */
          if((uint16)ETH_30_VTT_FRAME_TYPE_VLAN == (uint16)FrameType)
          {
            uint8 VlanPrio, Mask;
            /* retrieve VLAN priority out of frame buffer */
            VlanPrio = (uint8)((Eth_30_Vtt_Ctrl2TxBufferMap[CtrlIdx].U8[BufferIdx + ETH_30_VTT_VLAN_PRIO_OFFSET]) & 0xE0) >> 5;
            /* create mask for bitfield to check priority assignment */
            Mask = 1U << VlanPrio;
            /* Query for descriptor ring which handles the transmission.
            * If no descriptor is found  frame is assigned to default ring (best effort ring) as fall back. */
            DescrRingIdx = Eth_30_Vtt_VCfgGetTxDescrRingCnt(CtrlIdx);
            while(0U < DescrRingIdx)
            {
              DescrRingIdx--;
              if( 0U != (Mask & Eth_30_Vtt_QoSRingTx2PrioMap[DescrRingIdx]) )
              {
                /* VLAN priority mapped to corresponding descriptor */
                MappingFound = TRUE;
                break;
              }
            }
          }
          /* Set queue for untagged frames, in case no match was found */
          if(FALSE == MappingFound)
          {
            DescrRingIdx = ETH_30_VTT_UNTAGGED_FRAMES_QUEUE_TX;
          }
         }
#endif /* ETH_ENABLE_QOS */

        CurPos = Eth_30_Vtt_TxDescRingFreePos[CtrlIdx][DescrRingIdx];

        if ( TRUE == TxConfirmation )
        {
          Eth_30_Vtt_VCfgGetTxState(CtrlIdx, BufIdx) = (uint8)ETH_30_VTT_TX_STATE_CONFIRMATION_PENDING;
        }

        /* #1040 Prepare descriptor for Transmission */
       Eth_30_Vtt_LL_PrepareTxDescriptorForTransmission(CtrlIdx,
                                                            0,
                                                            CurPos,
                                                            BufIdx,
                                                            LenByte,
                                                            BufferPtr.U8);

       /* 1050: Remember the buffer connected to this descriptor and mark descriptor state as ready for transmission */
       Eth_30_Vtt_TxDescriptorBufferMap[CtrlIdx][DescrRingIdx][CurPos] = BufIdx;
       Eth_30_Vtt_TxDescriptorTxReadyTable[CtrlIdx][DescrRingIdx][CurPos] = (uint8)ETH_30_VTT_DESCRIPTOR_READY;

  #if (ETH_30_VTT_ENABLE_PTP == STD_ON)
        /* Reset PTP status */
        if (ETH_30_VTT_PTP_EGRESS_DESIRED == Eth_30_Vtt_PtpEgressTable[CtrlIdx][BufIdx])
        {
          Eth_30_Vtt_LL_PrepareTxDescriptorForTimestamping(CtrlIdx, DescrRingIdx, CurPos, BufIdx);
          Eth_30_Vtt_PtpBuf2DescMap[CtrlIdx][BufIdx] = CurPos;
        }
  #endif /* ETH_30_VTT_ENABLE_PTP */

        /* #1060 Prepare Tx descriptor for checksum offloading */  /*lint -e{522} */
        Eth_30_Vtt_LL_PrepareTxDescriptorForChecksumOffloading(CtrlIdx, DescrRingIdx, CurPos);
        
        /* #1070 Mark as owned by DMA */
        Eth_30_Vtt_LL_TriggerTransmission(CtrlIdx, DescrRingIdx, CurPos);

        /* #1080 Move next descriptor in the transmission ring */
        Eth_30_Vtt_IncTxDescRingCurFreePtr(CtrlIdx, DescrRingIdx);
        RetVal = E_OK;
      }
    }
    else
    {
      /* #110 Buffer was not provided before - transmission not possible */
      RetVal = E_NOT_OK;
    }
    ETH_30_VTT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  /* ----- Development Error Report --------------------------------------- */
#if (ETH_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ETH_30_VTT_E_NO_ERROR)
  {
    (void)Det_ReportError(ETH_MODULE_ID, CtrlIdx, ETH_30_VTT_SID_TRANSMIT, errorId);
  }
#else
  ETH_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* ETH_30_VTT_DEV_ERROR_REPORT == STD_ON */
  return RetVal;
} /* PRQA S 6010, 6030, 6050, 6060, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STPAR, MD_MSR_STMIF */
#endif /* ETH_30_VTT_IMPLEMENTATION_VTRANSMIT == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_RECEIVE == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_Receive
 **********************************************************************************************************************/
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
FUNC(void, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_Receive(uint8 CtrlIdx, P2VAR(Eth_RxStatusType, AUTOMATIC, AUTOMATIC) RxStatusPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETH_30_VTT_E_NO_ERROR;
  uint16 LenByte;
  boolean IsBroadcast;
  uint16 FrameType;
  uint8 DescrRingIdx;
  /* PRQA S 0759 1 */ /* MD_Eth_30_Vtt_0759 */
  Eth_30_Vtt_AlignedFrameCacheType RxBufDataPtr;

  /* ----- Development Error Checks ------------------------------------- */
  CtrlIdx = Eth_30_Vtt_TransformToLocalCtrlIdx(CtrlIdx);
  
#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check parameters for plausibility */
  if(CtrlIdx >= ETH_30_VTT_MAX_CTRLS_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_CTRL_IDX;
  }
  else if(Eth_30_Vtt_State == ETH_STATE_UNINIT)
  {
    errorId = ETH_30_VTT_E_NOT_INITIALIZED;
  }
  else if(Eth_30_Vtt_Mode[CtrlIdx] == ETH_MODE_DOWN)
  {
    errorId = ETH_30_VTT_E_INV_MODE;
  }
  else
#endif /* ETH_30_VTT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    ETH_30_VTT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #100 Retrieve the DescrRing where a frame has been received */
    DescrRingIdx = Eth_30_Vtt_LL_GetRxDescrRingIdxFromPendingInterrupt(CtrlIdx);

#if(ETH_30_VTT_ENABLE_QOS == STD_ON)
    if(DescrRingIdx == ETH_30_VTT_UNDEFINED_DESCRIPTOR_RING)
    {
      /* No Rx DescrRing pending operations - leave function */
      *RxStatusPtr = ETH_NOT_RECEIVED;
    }
    else
#endif
    {
      /* #110 Check whether descriptor is owned by Host or DMA */
      if(Eth_30_Vtt_LL_IsRxDescriptorOwnedByDriver(CtrlIdx, DescrRingIdx, Eth_30_Vtt_RxReadPosition[CtrlIdx][DescrRingIdx]) == TRUE)
      {
        /* #1000 Is an error flag set in the descriptor? */
        if (Eth_30_Vtt_LL_IsErrorinRxDescriptor(CtrlIdx, DescrRingIdx, Eth_30_Vtt_RxReadPosition[CtrlIdx][DescrRingIdx]) == FALSE)
        {
          uint32 BufOffs = Eth_30_Vtt_RxReadPosition[CtrlIdx][DescrRingIdx] * Eth_30_Vtt_VCfgGetRxRing(CtrlIdx, DescrRingIdx).AlignedSegmentSize;

          /* #1010 Extract frame length from RX descriptor */
          LenByte = Eth_30_Vtt_LL_GetFrameLengthFromRxDescriptor(CtrlIdx, DescrRingIdx, Eth_30_Vtt_RxReadPosition[CtrlIdx][DescrRingIdx]);

          /* #1020 Access associated buffer */
          RxBufDataPtr.U8 = &Eth_30_Vtt_VCfgGetRxRing(CtrlIdx, DescrRingIdx).BufPtr.U8[BufOffs];

          /* #1030 Check whether received frame is a broadcast */
          IsBroadcast = Eth_30_Vtt_IsBroadcast(&RxBufDataPtr.U8[ETH_30_VTT_RX_DST_OFFSET_U8]);

          /* #1040 Extract frame type */
          FrameType = IPBASE_HTON16(RxBufDataPtr.U16[(ETH_30_VTT_RX_TYPE_OFFSET_U16)]);

          ETH_30_VTT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

          /* #1050 Call RxIndication of Ethernet Interface */
          /* PRQA S 3305,0310 5 */ /* MD_Eth_30_Vtt_3305 */
          EthIf_RxIndication(
              Eth_30_Vtt_TransformToGlobalCtrlIdx(CtrlIdx),
              FrameType,
              IsBroadcast,
              &RxBufDataPtr.U8[ETH_30_VTT_RX_SRC_OFFSET_U8],
              &RxBufDataPtr.U32[ETH_30_VTT_RX_DATA_OFFSET_U32],
              LenByte - ETH_30_VTT_HDR_LEN_BYTE);

          ETH_30_VTT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        }
        else
        {
          /* payload not valid - discard package */
        }

        /* #120 Recharge descriptor so that the next frame can be received in this descriptor */
        Eth_30_Vtt_LL_RechargeRxDescriptor(CtrlIdx, DescrRingIdx, Eth_30_Vtt_RxReadPosition[CtrlIdx][DescrRingIdx]);

        /* #130 Do adaptions in the descriptor ring after decriptor processing has been finished,
         * e. g. setting of tail pointer registers, reordering of descriptors or buffers, ... */  /*lint -e{522} */
        Eth_30_Vtt_LL_AdaptRxDescriptorRingAfterReception(CtrlIdx, DescrRingIdx, Eth_30_Vtt_RxReadPosition[CtrlIdx][DescrRingIdx]);

        /* #140 Reset interrupt flag for the current Rx descriptor ring */  /*lint -e{522} */
        Eth_30_Vtt_LL_ResetRxIntFlag(CtrlIdx, DescrRingIdx);

        /* #150 Next Descriptor will be processed now */
        Eth_30_Vtt_IncRxReadPosition(CtrlIdx, DescrRingIdx);

        /* Frames may have been lost because of a buffer overflow or unavailable buffers. Nevertheless no error is returned to the EthIf.
        Otherwise congestion could happen in polling mode because ETH_RECEIVED_MORE_DATA_AVAILABLE can not be returned to the EthIf anymore
        and a maximum of one frame per mainfunction can be processed. */
        if(Eth_30_Vtt_LL_IsRxDescriptorOwnedByDriver(CtrlIdx,  DescrRingIdx, Eth_30_Vtt_RxReadPosition[CtrlIdx][DescrRingIdx]) == TRUE)
        {
          /* more data available ... */
          *RxStatusPtr = ETH_RECEIVED_MORE_DATA_AVAILABLE;
        }
        else
        {
          /* Set status to received */
          *RxStatusPtr = ETH_RECEIVED;
        }
      }
      else
      {
        *RxStatusPtr = ETH_NOT_RECEIVED;
      }
    }
    ETH_30_VTT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
  /* ----- Development Error Report --------------------------------------- */
#if (ETH_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ETH_30_VTT_E_NO_ERROR)
  {
    (void)Det_ReportError(ETH_MODULE_ID, CtrlIdx, ETH_30_VTT_SID_RECEIVE, errorId);
  }
#else
  ETH_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* ETH_30_VTT_DEV_ERROR_REPORT == STD_ON */
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL , MD_MSR_STMIF */
#endif /* ETH_30_VTT_IMPLEMENTATION_RECEIVE == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_TX_CONFIRMATION == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_TxConfirmation
 **********************************************************************************************************************/
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
FUNC(void, ETH_30_VTT_CODE_ISR) Eth_30_Vtt_TxConfirmation(uint8 CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETH_30_VTT_E_NO_ERROR;
  uint8 CurPos;
  uint8 DescrRingIdx;
  
  /* ----- Development Error Checks ------------------------------------- */
  CtrlIdx = Eth_30_Vtt_TransformToLocalCtrlIdx(CtrlIdx);
  
#if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check parameters for plausibility */
  if(CtrlIdx >= ETH_30_VTT_MAX_CTRLS_TOTAL)
  {
    errorId = ETH_30_VTT_E_INV_CTRL_IDX;
  }
  else if(Eth_30_Vtt_State == ETH_STATE_UNINIT)
  {
    errorId = ETH_30_VTT_E_NOT_INITIALIZED;
  }
  else if(Eth_30_Vtt_Mode[CtrlIdx] == ETH_MODE_DOWN)
  {
    errorId = ETH_30_VTT_E_INV_MODE;
  }
  else
#endif /* ETH_30_VTT_DEV_ERROR_DETECT == STD_ON */
  {
    /* #100 Retrieve the DescrRing where a frame has been transmitted */
    DescrRingIdx = Eth_30_Vtt_LL_GetTxDescrRingIdxFromPendingInterrupt(CtrlIdx);
#if(ETH_30_VTT_ENABLE_QOS == STD_ON)
    if(DescrRingIdx == ETH_30_VTT_UNDEFINED_DESCRIPTOR_RING)
    {
      /* No Tx DescrRing pending operations - leave function */
    }
    else
#endif
    {
      /* #200 Find last processed descriptor */
      CurPos = Eth_30_Vtt_TxDescRingCurProcPos[CtrlIdx][DescrRingIdx];

      ETH_30_VTT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 1 */ /* MD_MSR_14.3 */

      /* #300 Iterate over all Tx descriptors, which have been transmitted */
      while ((Eth_30_Vtt_LL_IsTxDescriptorOwnedByDriver(CtrlIdx, DescrRingIdx, CurPos)) &&
             (Eth_30_Vtt_TxDescriptorTxReadyTable[CtrlIdx][DescrRingIdx][CurPos] == ETH_30_VTT_DESCRIPTOR_READY))
      {
        Eth_30_Vtt_TxDescriptorTxReadyTable[CtrlIdx][DescrRingIdx][CurPos] = (uint8)ETH_30_VTT_DESCRIPTOR_TRANSMITTED;

        /* #3000 Check if upper layer requested to be notified about the finished transmission*/
        if(Eth_30_Vtt_VCfgGetTxState(CtrlIdx, Eth_30_Vtt_TxDescriptorBufferMap[CtrlIdx][DescrRingIdx][CurPos]) == ETH_30_VTT_TX_STATE_CONFIRMATION_PENDING)
        {
          ETH_30_VTT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 1 */ /* MD_MSR_14.3 */

          /* #30000 Call Ethernet Interface TxConfirmation*/
          EthIf_TxConfirmation(Eth_30_Vtt_TransformToGlobalCtrlIdx(CtrlIdx), Eth_30_Vtt_TxDescriptorBufferMap[CtrlIdx][DescrRingIdx][CurPos]);

          ETH_30_VTT_BEGIN_CRITICAL_SECTION_0(); /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
          Eth_30_Vtt_VCfgGetTxState(CtrlIdx, Eth_30_Vtt_TxDescriptorBufferMap[CtrlIdx][DescrRingIdx][CurPos]) = ETH_30_VTT_TX_STATE_CONFIRMATION_NOT_PENDING;
        }

    #if (ETH_30_VTT_ENABLE_PTP == STD_ON)
        Eth_30_Vtt_LL_ResetPtpStatusAfterTransmission(CtrlIdx, DescrRingIdx, CurPos);
    #endif  /* ETH_30_VTT_ENABLE_PTP */

        /* #3100 release descriptor and buffer */
        Eth_30_Vtt_TxDescriptorTxReadyTable[CtrlIdx][DescrRingIdx][CurPos] = (uint8)ETH_30_VTT_DESCRIPTOR_NOT_READY;
        Eth_30_Vtt_TxBufferBusyTable[CtrlIdx][Eth_30_Vtt_TxDescriptorBufferMap[CtrlIdx][DescrRingIdx][CurPos]] = (uint8)ETH_30_VTT_BUFFER_NOT_BUSY;

        /* #3200 switch to next descriptor */
        Eth_30_Vtt_IncTxDescRingCurProcPtr(CtrlIdx, DescrRingIdx);
        CurPos = Eth_30_Vtt_TxDescRingCurProcPos[CtrlIdx][DescrRingIdx];
      }
    ETH_30_VTT_END_CRITICAL_SECTION_0(); /* PRQA S 3109 1 */ /* MD_MSR_14.3 */
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (ETH_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ETH_30_VTT_E_NO_ERROR)
  {
    (void)Det_ReportError(ETH_MODULE_ID, CtrlIdx, ETH_30_VTT_SID_TX_CONFIRMATION, errorId);
  }
#else
  ETH_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* ETH_30_VTT_DEV_ERROR_REPORT == STD_ON */
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* ETH_30_VTT_IMPLEMENTATION_TX_CONFIRMATION == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_MAINFUNCTION == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_MainFunction
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
void Eth_30_Vtt_MainFunction(void)
{
  /* #10 Call MainFunction of lower layer */
  Eth_30_Vtt_LL_MainFunction();
}
#endif /* ETH_30_VTT_IMPLEMENTATION_MAINFUNCTION == STD_ON */

#if (ETH_30_VTT_IMPLEMENTATION_GET_VERSION_INFO == STD_ON)
#if (ETH_30_VTT_VERSION_INFO_API == STD_ON)
/***********************************************************************************************************************
 *  Eth_30_Vtt_GetVersionInfo
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETH_30_VTT_CODE) Eth_30_Vtt_GetVersionInfo(
  P2VAR(Std_VersionInfoType, ETH_30_VTT_APPL_DATA, ETH_30_VTT_APPL_DATA) VersionInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
   uint8 errorId = ETH_30_VTT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  #if ( ETH_30_VTT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check parameters for plausibility */
  if(VersionInfoPtr == NULL_PTR)
  {
    errorId = ETH_30_VTT_E_INV_POINTER;
  }
  else
  #endif /* ETH_30_VTT_DEV_ERROR_DETECT == STD_ON */
  {
    /*#100 Return Vendor Id, Module Id and SW version number */
    VersionInfoPtr->vendorID = ETH_VENDOR_ID;
    VersionInfoPtr->moduleID = ETH_MODULE_ID;
    VersionInfoPtr->sw_major_version = ETH_30_VTT_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = ETH_30_VTT_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = ETH_30_VTT_SW_PATCH_VERSION;
  }
  /* ----- Development Error Report --------------------------------------- */
  #if (ETH_30_VTT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != ETH_30_VTT_E_NO_ERROR)
  {
    (void)Det_ReportError(ETH_MODULE_ID, 0, ETH_30_VTT_SID_GET_VERSION_INFO, errorId);
  }
  #else
  ETH_30_VTT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* ETH_30_VTT_DEV_ERROR_REPORT == STD_ON */
} /* Eth_30_Vtt_GetVersionInfo() */
#endif /* ETH_VERSION_INFO_API */
#endif /* ETH_30_VTT_IMPLEMENTATION_GET_VERSION_INFO == STD_ON */

#define ETH_30_VTT_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* module specific MISRA deviations:
 MD_Eth_30_Vtt_0306:
 Reason:     Hardware accesses need a cast from integral type to a pointer to object.
 Risk:       There is no risk as the register is mapped to the respective memory address.
 Prevention: Covered by code review.

 MD_Eth_30_Vtt_0310:
 Reason:     The pointer where the cast led to has a less stricter alignment. The cast
             is necessary since the array represents a continuous data buffer.
 Risk:       There is no risk since the array is a continuous data buffer.
 Prevention: Covered by code review.

 MD_Eth_30_Vtt_3305:
 Reason:     The cast is necessary since the array represents a continuous data buffer and the Ethernet header
             has a length of 14 bytes
 Risk:       There is no risk since the array is a continuous data buffer. which is not divisable by 4.
 Prevention: Covered by code review.

 MD_Eth_30_Vtt_3305_1:
 Reason:     Fast access to Ethernet MAC address.
 Risk:       When the physical address buffer starts on an uneven memory address this may lead to
             a unaligned access exception on some hardware platforms. The buffer must
             start on an even memory address.
 Prevention: Covered by code review.

 MD_Eth_30_Vtt_3206:
 Reason:     The configuration pointer is only used when configuration variant 3 is chosen.
 Risk:       There is no risk.
 Prevention: Covered by code review.

 MD_Eth_30_Vtt_3199:
 Reason:     Dummy statement to avoid compiler warnings.
 Risk:       There is no risk as such statements have no effect on the code.
 Prevention: Covered by code review.

 MD_Eth_30_Vtt_0488:
 Reason:     Pointer arithmetic necessary to quickly access the reception buffer start address.
 Risk:       Wrong arithmetic could lead to a memory corruption.
 Prevention: Covered by code review. Wrong arithmetic is avoided.

 MD_Eth_30_Vtt_3371:
 Reason:     Intended behaviour of crc checksum.
 Risk:       There is no risk.
 Prevention: Covered by code review.

 MD_Eth_30_Vtt_3197:
 Reason:     First read/write access is variant depending. Possible initialization before possible write access does not harm but
             improves code readability.
 Risk:       There is no risk.
 Prevention: Covered by code review.

 MD_Eth_30_Vtt_3218:
 Reason:     No function local static variables should be used. File local variable must be used instead.
 Risk:       There is no risk.
 Prevention: Covered by code review.
*/

/**********************************************************************************************************************
 *  END OF FILE: Eth_30_Vtt.c
 *********************************************************************************************************************/

