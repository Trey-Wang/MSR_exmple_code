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
/**        \file  Fr.c
 *        \brief  Fr source file
 *
 *      \details  FlexRay Driver for CANoeEmu platform
 *
 *********************************************************************************************************************/

 /*********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define FR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Fr_Priv.h"
#include "Std_Types.h"
#include "VStdLib.h"

#if (FR_VTT == STD_ON) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
# include "VttCntrl_Fr.h"
#else /* CanoeEmu */
# include "Os.h"
# include "CanoeApi.h"
#endif /* (FR_VTT == STD_ON) */

#if (FR_CONFIGURATION_VARIANT == FR_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
# include "EcuM_Error.h"
#endif

#include "Fr_ERay.h"

/* PRQA S 0612 EOF */ /* MD_Fr_0612 */
/* PRQA S 5013 EOF */ /* MD_Fr_5013 */
/* PRQA S 4130 EOF */ /* MD_Fr_4130 */
/* PRQA S 3747, 3757,3762, 3784, 3794, 3792 EOF */ /* MD_Fr_3747 */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

#if ((FR_SW_MAJOR_VERSION != 0x5u) || (FR_SW_MINOR_VERSION != 0x00u) || (FR_SW_PATCH_VERSION != 0x01u))
# error "Fr.c: Source and Header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#define MAXARRAYFORNMVECTOR                                           12u
#define MAX_SLOT_NUMBER                                               2048
#define DEFAULT_WAKEUP_CHANNEL                                        1
#define PAYLOAD_PREAMBLE_BIT_MASK                                     0x10

#define FR_CANOE_BUSMODE_UNKNOWN                                      0u
#define FR_CANOE_BUSMODE_SIMULATE                                     1u
#define FR_CANOE_BUSMODE_REAL                                         2u

#define FR_FIFO_MARK                                                  0x80u
#define FR_LAST_FIFO_FRAME                                            1u
#define FR_STARTSTOP_MESSAGES_ON                                      1u
#define FR_STARTSTOP_MESSAGES_OFF                                     0u
#define FR_SYNC_FRAME_LIST_MAX                                        ((uint8)15)
#define FR_HEADER_CRC_MAX                                             ((uint16)0x7FF) /* Must be 2^x-1 bitvalue to allow bitfiltering */
#define FR_FLAGS_SYNCFRAME                                            0x08u

#define FR_TXVIRTBUF_INFO_RECONFIG_LPDU                               ((uint8)0x4)
#define FR_RXVIRTBUF_CYCLEBASE_RECONFIG_LPDU                          ((uint8)0x80)

#if ( FR_VENABLE_MID_FILTERING == STD_ON )
# if (Fr_IsPostbuild())
#  if (FR_NUM_CTRL_USED > 1) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
#   define FR_GETLAST_STATIC_FRAMEID(Ctrl)                            ((uint16)Fr_VPostBuildConfigPtr->LastStaticFrameID[(Ctrl)])
#  else 
#   define FR_GETLAST_STATIC_FRAMEID(Ctrl)                            ((uint16)Fr_VPostBuildConfigPtr->LastStaticFrameID)
#  endif
#  define FR_GETSIZE_MID_ARRAY(Ctrl)                                  ((uint16)Fr_VPostBuildConfigPtr->NumMIDs)
# else
#  if (FR_NUM_CTRL_USED > 1) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
#   define FR_GETLAST_STATIC_FRAMEID(Ctrl)                            ((uint16)Fr_VLastStaticFrameID[(Ctrl)])
#  else
#   if (FR_CONFIGURATION_VARIANT == FR_CONFIGURATION_VARIANT_LINKTIME)
#    define FR_GETLAST_STATIC_FRAMEID(Ctrl)                           ((uint16)Fr_VLastStaticFrameID)
#   else
#    define FR_GETLAST_STATIC_FRAMEID(Ctrl)                           ((uint16)FR_LAST_STATIC_FRAMEID)
#   endif
#  endif
#  if (FR_CONFIGURATION_VARIANT == FR_CONFIGURATION_VARIANT_LINKTIME)
#   define FR_GETSIZE_MID_ARRAY(Ctrl)                                 ((uint16)sizeof(Fr_VMsgIdFilterMap)/sizeof(uint16))
#  else /* (FR_CONFIGURATION_VARIANT == FR_CONFIGURATION_VARIANT_PRECOMPILE ) */
#   define FR_GETSIZE_MID_ARRAY(Ctrl)                                 (FR_SIZE_MID_ARRAY)
#  endif
# endif
#endif

#if (Fr_IsPostbuild())
# define CCADDRS                                                      Fr_VPostBuildConfigPtr->CcRegAddrs
# define CCBUFS(Ctrl)                                                 Fr_VLocCcBufs[Ctrl]
# if (FR_NUM_CTRL_USED == 1) 
#  define CCBUFS_CFG(Ctrl)                                            Fr_VPostBuildConfigPtr->CcBufs
# else /* (FR_NUM_CTRL_USED > 1) */
#  define CCBUFS_CFG(Ctrl)                                            Fr_VPostBuildConfigPtr->CcBufs[Ctrl]
# endif /* (FR_NUM_CTRL_USED > 1) */
#else
# define CCADDRS                                                      Fr_VCcRegAddrs
# define CCBUFS(Ctrl)                                                 Fr_VLocCcBufs[Ctrl]
# if (FR_NUM_CTRL_USED == 1) 
#  define CCBUFS_CFG(Ctrl)                                            Fr_VCcBufs
# else /* (FR_NUM_CTRL_USED > 1) */
#  define CCBUFS_CFG(Ctrl)                                            Fr_CcBufs[Ctrl]
# endif /* (FR_NUM_CTRL_USED > 1) */
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/* PRQA S 3453 FUNCTIONLIKEMACROS */ /* MD_MSR_19.7 */

#define FR_CANOE_INVALID_VALUE(value)                                 ((value) < 0)

#define Fr_Min(val1, val2)                                            (((val1) < (val2)) ? (val1) : (val2))

#if (Fr_IsPostbuild())
# if (FR_NUM_CTRL_USED == 1) 
#  define Fr_VGetCcCfgValAt(Ctrl, Idx)                                 (Fr_VPostBuildConfigPtr->CcRegVals[(Idx)])
#  define Fr_VGetNumRxVirtBuf2PhysBufs(Ctrl)                           (Fr_VPostBuildConfigPtr->NumRxVirtBuf2PhysBufs)
#  define Fr_VGetNumTxVirtBuf2PhysBufs(Ctrl)                           (Fr_VPostBuildConfigPtr->NumTxVirtBuf2PhysBufs)
#  define Fr_VGetNumCcRegs(Ctrl)                                       (Fr_VPostBuildConfigPtr->NumCcRegs)
#  define Fr_GetRxVirtBuf2Phys(Ctrl, Idx)                              (Fr_VRxVirtBuf2PhysBufMapPtr[0][(Idx)])
#  define Fr_GetTxVirtBuf2Phys(Ctrl, Idx)                              (Fr_VTxVirtBuf2PhysBufMapPtr[0][(Idx)])

#  if (FR_NMVECTOR_ENABLE == STD_ON)
#   define Fr_GetNMVectorLength(Ctrl)                                  (Fr_VPostBuildConfigPtr->NmVectorLength)
#  endif
   
#  if ( FR_VENABLE_MID_FILTERING == STD_ON )
#   define FR_GETMIDFILTER(Ctrl, Idx)                                  (Fr_VPostBuildConfigPtr->MsgIdFilterMap[(Idx)])
#  endif
   
#  if (FR_READ_CC_CONFIGURATION == STD_ON)
#   define Fr_GetVCCConfigParam8Bit(Ctrl, Idx)                         (Fr_VPostBuildConfigPtr->CCConfigParam8Bit[(Idx)])
#   define Fr_GetVCCConfigParam16Bit(Ctrl, Idx)                        (Fr_VPostBuildConfigPtr->CCConfigParam16Bit[(Idx)])
#   define Fr_GetVCCConfigParam32Bit(Ctrl, Idx)                        (Fr_VPostBuildConfigPtr->CCConfigParam32Bit[(Idx)])
#  endif
# else /* (FR_NUM_CTRL_USED > 1)  */
#  define Fr_VGetCcCfgValAt(Ctrl, Idx)                                 (Fr_VPostBuildConfigPtr->CcRegVals[Ctrl][(Idx)])
#  define Fr_VGetNumRxVirtBuf2PhysBufs(Ctrl)                           (Fr_VPostBuildConfigPtr->NumRxVirtBuf2PhysBufs[Ctrl])
#  define Fr_VGetNumTxVirtBuf2PhysBufs(Ctrl)                           (Fr_VPostBuildConfigPtr->NumTxVirtBuf2PhysBufs[Ctrl])
#  define Fr_VGetNumCcRegs(Ctrl)                                       (Fr_VPostBuildConfigPtr->NumCcRegs[Ctrl])
#  define Fr_GetRxVirtBuf2Phys(Ctrl, Idx)                              (Fr_VRxVirtBuf2PhysBufMapPtr[Ctrl][(Idx)])
#  define Fr_GetTxVirtBuf2Phys(Ctrl, Idx)                              (Fr_VTxVirtBuf2PhysBufMapPtr[Ctrl][(Idx)])

#  if (FR_NMVECTOR_ENABLE == STD_ON)
#   define Fr_GetNMVectorLength(Ctrl)                                  (Fr_VPostBuildConfigPtr->NmVectorLength)
#  endif

#  if ( FR_VENABLE_MID_FILTERING == STD_ON )
#   define FR_GETMIDFILTER(Ctrl, Idx)                                  (Fr_VPostBuildConfigPtr->MsgIdFilterMap[(Idx)])
#  endif

#  if (FR_READ_CC_CONFIGURATION == STD_ON)
#   define Fr_GetVCCConfigParam8Bit(Ctrl, Idx)                         (Fr_VPostBuildConfigPtr->CCConfigParam8Bit[Ctrl][(Idx)])
#   define Fr_GetVCCConfigParam16Bit(Ctrl, Idx)                        (Fr_VPostBuildConfigPtr->CCConfigParam16Bit[Ctrl][(Idx)])
#   define Fr_GetVCCConfigParam32Bit(Ctrl, Idx)                        (Fr_VPostBuildConfigPtr->CCConfigParam32Bit[Ctrl][(Idx)])
#  endif
# endif /* (FR_NUM_CTRL_USED > 1) */
#else /* ((FR_CONFIGURATION_VARIANT == FR_CONFIGURATION_VARIANT_LINKTIME) || (FR_CONFIGURATION_VARIANT == FR_CONFIGURATION_VARIANT_PRECOMPILE)) */
# if (FR_NUM_CTRL_USED == 1) 
#  define Fr_VGetCcCfgValAt(Ctrl, Idx)                                 (Fr_VCcRegVals[(Idx)])
#  define Fr_GetRxVirtBuf2Phys(Ctrl, Idx)                              (Fr_VRxVirtBuf2PhysBufMap[(Idx)])
#  define Fr_GetTxVirtBuf2Phys(Ctrl, Idx)                              (Fr_VTxVirtBuf2PhysBufMap[(Idx)])
   
#  if (FR_READ_CC_CONFIGURATION == STD_ON)
#   define Fr_GetVCCConfigParam8Bit(Ctrl, Idx)                         (Fr_VCCConfigParam8Bit[(Idx)])
#   define Fr_GetVCCConfigParam16Bit(Ctrl, Idx)                        (Fr_VCCConfigParam16Bit[(Idx)])
#   define Fr_GetVCCConfigParam32Bit(Ctrl, Idx)                        (Fr_VCCConfigParam32Bit[(Idx)])
#  endif

#  if ( FR_VENABLE_MID_FILTERING == STD_ON )
#   define FR_GETMIDFILTER(Ctrl, Idx)                                  (Fr_VMsgIdFilterMap[(Idx)])
#  endif

#  if (FR_CONFIGURATION_VARIANT == FR_CONFIGURATION_VARIANT_LINKTIME)
#   define Fr_VGetNumRxVirtBuf2PhysBufs(Ctrl)                          (Fr_VNumRxVirtBuf2PhysBufs)
#   define Fr_VGetNumTxVirtBuf2PhysBufs(Ctrl)                          (Fr_VNumTxVirtBuf2PhysBufs)
#   define Fr_VGetNumCcRegs(Ctrl)                                      (Fr_VNumCcRegs)

#   if (FR_NMVECTOR_ENABLE == STD_ON)
#    define Fr_GetNMVectorLength(Ctrl)                                 (Fr_NmVectorLength)
#   endif
#  else /* (FR_CONFIGURATION_VARIANT == FR_CONFIGURATION_VARIANT_PRECOMPILE) */
#   define Fr_VGetNumRxVirtBuf2PhysBufs(Ctrl)                          (FR_SIZE_RXVIRTBUF2PHAYBUFMAP)
#   define Fr_VGetNumTxVirtBuf2PhysBufs(Ctrl)                          (FR_SIZE_TXVIRTBUF2PHAYBUFMAP)
#   define Fr_VGetNumCcRegs(Ctrl)                                      (FR_SIZE_CONTROLREGCONFARRAY)

#   if (FR_NMVECTOR_ENABLE == STD_ON)
#    define Fr_GetNMVectorLength(Ctrl)                                 (FR_NM_VECTOR_LENGTH)
#   endif
#  endif /* (FR_CONFIGURATION_VARIANT == FR_CONFIGURATION_VARIANT_PRECOMPILE) */
# else /* (FR_NUM_CTRL_USED > 1) */
#  define Fr_VGetCcCfgValAt(Ctrl, Idx)                                 (Fr_CtrlRegs[Ctrl][(Idx)])
#  define Fr_GetRxVirtBuf2Phys(Ctrl, Idx)                              (Fr_RxVirt2PhysBuf[Ctrl][(Idx)])
#  define Fr_GetTxVirtBuf2Phys(Ctrl, Idx)                              (Fr_TxVirt2PhysBuf[Ctrl][(Idx)])
#  define Fr_VGetNumRxVirtBuf2PhysBufs(Ctrl)                           (Fr_VNumRxVirtBuf2PhysBufs[Ctrl])
#  define Fr_VGetNumTxVirtBuf2PhysBufs(Ctrl)                           (Fr_VNumTxVirtBuf2PhysBufs[Ctrl])
#  define Fr_VGetNumCcRegs(Ctrl)                                       (Fr_VNumCcRegs[Ctrl])

#  if (FR_READ_CC_CONFIGURATION == STD_ON)
#   define Fr_GetVCCConfigParam8Bit(Ctrl, Idx)                         (Fr_Params8Bit[Ctrl][(Idx)])
#   define Fr_GetVCCConfigParam16Bit(Ctrl, Idx)                        (Fr_Params16Bit[Ctrl][(Idx)])
#   define Fr_GetVCCConfigParam32Bit(Ctrl, Idx)                        (Fr_Params32Bit[Ctrl][(Idx)])
#  endif

#  if ( FR_VENABLE_MID_FILTERING == STD_ON )
#   define FR_GETMIDFILTER(Ctrl, Idx)                                  (Fr_VMsgIdFilterMap[(Idx)])
#  endif

#  if (FR_NMVECTOR_ENABLE == STD_ON)
#   define Fr_GetNMVectorLength(Ctrl)                                  (Fr_NmVectorLength[Ctrl])
#  endif
# endif /* (FR_NUM_CTRL_USED > 1)  */
#endif /* (FR_CONFIGURATION_VARIANT == FR_CONFIGURATION_VARIANT_LINKTIME) */

#if (FR_VTT == STD_OFF) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
# define VttFr_RegisterFlexRayMessage(a,b,c,d,e,f,g)                  (void)CANoeAPI_RegisterFlexRayMessage((a),(b),(c),(d),(e),(f),(g))
# define VttFr_SendFlexRayMessage(a,b,c,d,e,f,g,h)                    (void)CANoeAPI_SendFlexRayMessage((a), (b), (c), (d), (e), (f), (g), (h))
# define VttFr_ResetFlexRayCC(a)                                      (void)CANoeAPI_ResetFlexRayCC(a)
# define VttFr_ResetFlexRayCCAndSendWUP(a,b, c, d, e,f)               (void)CANoeAPI_ResetFlexRayCCAndSendWUP((a),(b), (c), (d), (e),(f))
# define VttFr_GetCurrentSimTime()                                    CANoeAPI_GetCurrentSimTime()
# define VttFr_GetSimulationMode()                                    CANoeAPI_GetSimulationMode()
# define VttFr_AtomicBegin()
# define VttFr_AtomicEnd()
#endif /* #if (FR_VTT == STD_OFF) */

/* PRQA L:FUNCTIONLIKEMACROS */ /*  MD_MSR_19.7 */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef struct
{
  uint32 flags;
  sint16 Length;
  uint16 cycle;
  uint8 Data[256];
} vFrReceiveData;

typedef struct
{
  uint32 SUCC1;
  uint32 SUCC2;
  uint32 PRTC1;
  uint32 PRTC2;
  uint32 MHDC;
  uint32 GTUC1;
  uint32 GTUC2;
  uint32 GTUC7;
  uint32 GTUC11;
} vFrCcRegValsType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define FR_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */ /*lint -e{451} */

#if (FR_DEV_ERROR_DETECT == STD_ON )
VAR(Fr_VFrDrvStateType, FR_VAR_ZERO_INIT) Fr_VFrDrvState = (Fr_VFrDrvStateType)0;
#endif

#if (Fr_IsPostbuild())
P2CONST( Fr_VConfigType, FR_VAR_ZERO_INIT, FR_PBCFG) Fr_VPostBuildConfigPtr = 0;
#endif

#define FR_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */ /*lint -e{451} */

#define FR_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */ /*lint -e{451} */

# if ( FR_DIRECT_BUFFER_ACCESS_ENABLE == STD_ON )
VAR(uint8, FR_VAR_NOINIT) Fr_FrameBuf[FR_NUM_CTRL_USED][1024][256];
# endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

FR_LOCAL uint8                    gCurrentCycle[FR_NUM_CTRL_USED];
#if ( FR_NMVECTOR_ENABLE == STD_ON )
FR_LOCAL uint8                    gNmVectorData[FR_NUM_CTRL_USED][MAXARRAYFORNMVECTOR];
FR_LOCAL uint16                   gNmVectorLen[FR_NUM_CTRL_USED];
#endif
FR_LOCAL uint16                   gNumberOfStaticSlots[FR_NUM_CTRL_USED];
FR_LOCAL uint16                   gSyncSlotId[FR_NUM_CTRL_USED];
FR_LOCAL uint16                   gStaticPayloadLengthInBytes[FR_NUM_CTRL_USED];
FR_LOCAL uint16                   gStaticWUPTxIdle[FR_NUM_CTRL_USED];
FR_LOCAL uint16                   gStaticWUPTxLow[FR_NUM_CTRL_USED];
FR_LOCAL uint16                   gStaticWUPRepetitions[FR_NUM_CTRL_USED];
FR_LOCAL uint32                   gPOCStateRealBus[FR_NUM_CTRL_USED];

FR_LOCAL boolean                  gTxFramePending[FR_NUM_CTRL_USED][3][MAX_SLOT_NUMBER + 1][128];
FR_LOCAL boolean                  gTxFrameTransmitted[FR_NUM_CTRL_USED][3][MAX_SLOT_NUMBER + 1][128];
FR_LOCAL boolean                  gPreventSync[FR_NUM_CTRL_USED];
FR_LOCAL boolean                  gNormalPassive[FR_NUM_CTRL_USED];
FR_LOCAL boolean                  gUseSimulatedPocState[FR_NUM_CTRL_USED];
FR_LOCAL boolean                  gColdstartInhibit[FR_NUM_CTRL_USED];
FR_LOCAL boolean                  gIsSynchronized[FR_NUM_CTRL_USED];
FR_LOCAL boolean                  gHaltRequest[FR_NUM_CTRL_USED];
FR_LOCAL boolean                  gStartupFlag[FR_NUM_CTRL_USED];
FR_LOCAL boolean                  gTimerEnabled[FR_NUM_CTRL_USED][2];

FR_LOCAL int                      gDefaultWakeupChannel[FR_NUM_CTRL_USED];
FR_LOCAL unsigned long            gNanoSecondsPerMacrotick[FR_NUM_CTRL_USED];
FR_LOCAL unsigned long            gMacroticksPerCycle[FR_NUM_CTRL_USED];
FR_LOCAL __int64                  gStartCycleTime[FR_NUM_CTRL_USED];
FR_LOCAL __int64                  gTimeoutTimes[FR_NUM_CTRL_USED][2];

FR_LOCAL Fr_POCStateType          gSimulatedPOCState[FR_NUM_CTRL_USED];
FR_LOCAL vFrReceiveData           gReceiveData[FR_NUM_CTRL_USED][3][MAX_SLOT_NUMBER + 1];

FR_LOCAL vFrCcRegValsType         vFrCcRegVals[FR_NUM_CTRL_USED];
FR_LOCAL Fr_VCcBufType            Fr_VLocCcBufs[FR_NUM_CTRL_USED][1024]; /* 1024 for max number of physical buffers of Eray */

FR_LOCAL boolean                  wakeupChannelValid[FR_NUM_CTRL_USED];
FR_LOCAL Fr_ChannelType           wakeupChannel[FR_NUM_CTRL_USED];

#define FR_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */ /*lint -e{451} */

#define FR_START_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */ /*lint -e{451} */

#if (Fr_IsPostbuild())
FR_LOCAL P2CONST(Fr_VRxVirtBuf2PhysBufMapType, FR_VAR_NOINIT_FAST, FR_PBCFG) Fr_VRxVirtBuf2PhysBufMapPtr[FR_NUM_CTRL_USED];
FR_LOCAL P2CONST(Fr_VTxVirtBuf2PhysBufMapType, FR_VAR_NOINIT_FAST, FR_PBCFG) Fr_VTxVirtBuf2PhysBufMapPtr[FR_NUM_CTRL_USED];
#endif

#define FR_STOP_SEC_VAR_FAST_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /*  MD_MSR_19.1 */ /*lint -e{451} */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  Fr_InitFrCcRegVals
 **********************************************************************************************************************/
/*! \brief        Initialize controller registers.
 *  \details      Initialize controller registers according config structs of the FlexRay driver.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE) Fr_InitFrCcRegVals(uint8 Fr_CtrlIdx);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_IsTxBuffer
 **********************************************************************************************************************/
/*! \brief        Returns the message buffer direction.
 *  \details      Returns the Message Buffer Direction Configuration Bit (CFG) of the Write Header Section register 1 (WRHS1).
 *  \param[in]    MessageBufferIndex      Index of the requested message buffer.
 *  \return       TRUE                    The corresponding buffer is configured as Transmit Buffer.
 *  \return       FALSE                   The corresponding buffer is configured as Receive Buffer.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(boolean, FR_CODE) Fr_VCcCfgR_IsTxBuffer(uint8 Fr_CtrlIdx, uint16 MessageBufferIndex);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_HasPayloadPreambleBit
 **********************************************************************************************************************/
/*! \brief        Returns the payload preambe indicator.
 *  \details      Returns the Payload Preamble Indicator Transmit flag (PPIT) of the Write Header Section register 1 (WRHS1).
 *  \param[in]    MessageBufferIndex      Index of the requested message buffer.
 *  \return       TRUE                    Payload Preamble Indicator set.
 *  \return       FALSE                   Payload Preamble Indicator not set.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(boolean, FR_CODE) Fr_VCcCfgR_HasPayloadPreambleBit(uint8 Fr_CtrlIdx, uint16 MessageBufferIndex);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetBaseCycle
 **********************************************************************************************************************/
/*! \brief        Returns the base cycle.
 *  \details      Extracts and returns base cycle for Cycle Code (CYC) of the Write Header Section register 1 (WRHS1).
 *  \param[in]    MessageBufferIndex      Index of the requested message buffer.
 *  \return       BaseCycle               Cycle Offset part of the cycle filter mechanism.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint8, FR_CODE)   Fr_VCcCfgR_GetBaseCycle(uint8 Fr_CtrlIdx, uint16 MessageBufferIndex);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetCycleRepetition
 **********************************************************************************************************************/
/*! \brief        Returns the cycle repetition.
 *  \details      Extracts and returns cycle repetition for Cycle Code (CYC) of the Write Header Section register 1 (WRHS1).
 *  \param[in]    MessageBufferIndex      Index of the requested message buffer.
 *  \return       CycleRepetition         Repetition part of the cycle filter mechanism.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint8, FR_CODE)   Fr_VCcCfgR_GetCycleRepetition(uint8 Fr_CtrlIdx, uint16 MessageBufferIndex);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetCycleCode
 **********************************************************************************************************************/
/*! \brief        Returns the cycle code.
 *  \details      Returns the Cycle Code (CYC) of the Write Header Section register 1 (WRHS1).
 *  \param[in]    MessageBufferIndex      Index of the requested message buffer.
 *  \return       CycleCode               The 7-bit cycle code determines the cycle set used for cycle counter filtering.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint8, FR_CODE)   Fr_VCcCfgR_GetCycleCode(uint8 Fr_CtrlIdx, uint16 MessageBufferIndex);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetChannel
 **********************************************************************************************************************/
/*! \brief        Returns the channel configuration.
 *  \details      Returns the Channel Filter Control flags (CHA, CHB) of the Write Header Section register 1 (WRHS1).
 *  \param[in]    MessageBufferIndex      Index of the requested message buffer.
 *  \return       Fr_ChnlIdx              FlexRay Channel of the FrIf_LPdu.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint8, FR_CODE)   Fr_VCcCfgR_GetChannel(uint8 Fr_CtrlIdx, uint16 MessageBufferIndex);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetPayloadLengthInBytes
 **********************************************************************************************************************/
/*! \brief        Returns the payload length in bytes.
 *  \details      Returns the Payload Length Configured flags (PLC) of the Write Header Section register 2 (WRHS2).
 *  \param[in]    MessageBufferIndex      Index of the requested message buffer.
 *  \return       Fr_PayloadLength        Payloadlength in units of bytes the FrIf_LPduIdx.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint8, FR_CODE)   Fr_VCcCfgR_GetPayloadLengthInBytes(uint8 Fr_CtrlIdx, uint16 MessageBufferIndex);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetNumberOfStaticSlots
 **********************************************************************************************************************/
/*! \brief        Returns the number of static slots.
 *  \details      Returns the Number of Static Slots (NSS) of the GTU Configuration Register 7 (GTUC7).
 *  \return       uint16                  Number of configured static slots.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint16, FR_CODE)  Fr_VCcCfgR_GetNumberOfStaticSlots(uint8 Fr_CtrlIdx);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetStaticPayloadLengthInBytes
 **********************************************************************************************************************/
/*! \brief        Returns the static payload length in bytes.
 *  \details      Returns the Static Frame Data Length (SFDL) of the  MHD Configuration Register (MHDC).
 *  \return       uint16                  Payload length in bytes of frames in the static segment.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint16, FR_CODE)  Fr_VCcCfgR_GetStaticPayloadLengthInBytes(uint8 Fr_CtrlIdx);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetWUPTxIdle
 **********************************************************************************************************************/
/*! \brief        Returns the Wakeup Symbol Transmit Idle bit time.
 *  \details      Returns the Wakeup Symbol Transmit Idle (TXI) of the PRT Configuration Register 2 (PRTC2).
 *  \return       uint16                  Number of bit times used by the node to transmit the idle phase of the wakeup
 *                                        symbol.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint16, FR_CODE)  Fr_VCcCfgR_GetWUPTxIdle(uint8 Fr_CtrlIdx);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetWUPTxLow
 **********************************************************************************************************************/
/*! \brief        Returns the Wakeup Symbol Transmit Low bit time.
 *  \details      Returns the Wakeup Symbol Transmit Low (TXL) of the PRT Configuration Register 2 (PRTC2).
 *  \return       uint16                  Number of bit times used by the node to transmit the low phase of the wakeup
 *                                        symbol.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint16, FR_CODE)  Fr_VCcCfgR_GetWUPTxLow(uint8 Fr_CtrlIdx);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetWUPRepetitions
 **********************************************************************************************************************/
/*! \brief        Returns the Repetitions of Tx Wakeup Pattern.
 *  \details      Returns the Repetitions of Tx Wakeup Pattern (RWP) of the PRT Configuration Register 1 (PRTC1).
 *  \return       uint16                  Number of repetitions (sequences) of the Tx wakeup symbol.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint16, FR_CODE)  Fr_VCcCfgR_GetWUPRepetitions(uint8 Fr_CtrlIdx);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetSyncSlotId
 **********************************************************************************************************************/
/*! \brief        Returns the Frame ID of the key-slot.
 *  \details      Get the Frame ID (FID) of the Write Header Section register 1 (WRHS1) if the ECU is configured as cold-
 *                start node.
 *  \return       uint16                  Frame ID of the selected message buffer. The frame ID defines the slot number
 *                                        for transmission / reception of the respective message.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint16, FR_CODE)  Fr_VCcCfgR_GetSyncSlotId(uint8 Fr_CtrlIdx);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetMacroTicksPerCycle
 **********************************************************************************************************************/
/*! \brief        Returns the number of Macroticks per Cycle.
 *  \details      Returns the Macrotick Per Cycle (MPC) of the Confiuration Register 2 (GTUC2).
 *  \return       uint16                  Duration of one communication cycle in macroticks.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint16, FR_CODE)  Fr_VCcCfgR_GetMacroTicksPerCycle(uint8 Fr_CtrlIdx);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetSlotId
 **********************************************************************************************************************/
/*! \brief        Returns the Frame ID.
 *  \details      Get the Frame ID (FID) of the Write Header Section register 1 (WRHS1).
 *  \return       uint16                  Frame ID of the selected message buffer. The frame ID defines the slot number
 *                                        for transmission / reception of the respective message.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint16, FR_CODE)  Fr_VCcCfgR_GetSlotId(uint8 Fr_CtrlIdx, uint16 MessageBufferIndex);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetPhysicalBufferNumber
 **********************************************************************************************************************/
/*! \brief        Returns the index of the physical message buffer.
 *  \details      Returns the physical message buffer index depending whether on the direction of the given Fr_LPduIdx.
 *  \param[in]    Fr_LPduIdx              This index is used to uniquely identify a FlexRay frame.
 *  \return       uint16                  Index of the corresponding physical message buffer.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint16, FR_CODE)  Fr_VCcCfgR_GetPhysicalBufferNumber(uint8 Fr_CtrlIdx, uint16 Fr_LPduIdx);

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetNanosecondsPerMacroTick
 **********************************************************************************************************************/
/*! \brief        Returns the Frame ID.
 *  \details      Calculate the nanoseconds per macrotick based on the Microtick per Cycle (UT) of the Confiuration
 *                Register 1 (GTUC1).
 *  \return       uint32                  Duration of a macrotick in nanoseconds.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint32, FR_CODE)  Fr_VCcCfgR_GetNanosecondsPerMacroTick(uint8 Fr_CtrlIdx);

/***********************************************************************************************************************
 *  Fr_GetRepetition
 **********************************************************************************************************************/
/*! \brief        Get the cycle repetition from the given cycle code.
 *  \details      Extracts the cycle repetition from the given Cycle Code (CYC) of the Write Header Section register 1 (WRHS1).
 *  \param[in]    CycleCode               The 7-bit cycle code determines the cycle set used for cycle counter filtering.
 *  \return       uint8                   Cycle Repetition part of the cycle filter mechanism.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint8, FR_CODE)   Fr_GetRepetition(uint8 cycleCode);

/***********************************************************************************************************************
 *  Fr_GetBaseCycle
 **********************************************************************************************************************/
/*! \brief        Get the base cycle from the given cycle code.
 *  \details      Extracts the base cycle from the given Cycle Code (CYC) of the Write Header Section register 1 (WRHS1).
 *  \param[in]    CycleCode               The 7-bit cycle code determines the cycle set used for cycle counter filtering.
 *  \return       uint8                   Base Cycle part of the cycle filter mechanism.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint8, FR_CODE)   Fr_GetBaseCycle(uint8 cycleCode);

/***********************************************************************************************************************
 *  Fr_GetCycleMask
 **********************************************************************************************************************/
/*! \brief        Get the base cycle from the given cycle code.
 *  \details      Extracts the base cycle from the given Cycle Code (CYC) of the Write Header Section register 1 (WRHS1).
 *  \param[in]    CycleCode               The 7-bit cycle code determines the cycle set used for cycle counter filtering.
 *  \return       uint8                   Base Cycle part of the cycle filter mechanism.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint8, FR_CODE)   Fr_GetCycleMask(uint8 cycleCode);

/***********************************************************************************************************************
 *  Fr_SetRxMsgBuf
 **********************************************************************************************************************/
/*! \brief        Store the RX frame.
 *  \details      Store the given RX frame payload, FlexRay flags and the update informations for the frame with the
 *                given channel and slotId.
 *  \param[in]    channel                 Index of FlexRay channel within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \param[in]    slotId                  FlexRay Frame ID the RX frame.
 *  \param[in]    dataptr                 This reference points to the buffer of the received payload.
 *  \param[in]    length                  Length in bytes of the received frame.
 *  \param[in]    flags                   FlexRay flags of the CANoe frame.
 *  \param[in]    cycle                   Cycle in which the FlexRay frame was received by CANoe.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE)    Fr_SetRxMsgBuf(uint8 Fr_CtrlIdx, uint32 channel, uint16 slotId,
                                                       const uint8* dataptr, sint16 length, uint32 flags, uint16 cycle);

/***********************************************************************************************************************
 *  Fr_GetRxMsgBufData
 **********************************************************************************************************************/
/*! \brief        Get RX frame payload.
 *  \details      Copy requested length bytes of the RX frame payload with the given channel and slotId to the data pointer.
 *  \param[in]    channel                 Index of FlexRay channel within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \param[in]    slotId                  FlexRay Frame ID the RX frame.
 *  \param[in]    length                  Length in bytes to be copies from the received frame payload.
 *  \param[in]    flags                   FlexRay flags of the CANoe frame.
 *  \param[in]    cycle                   Cycle in which the FlexRay frame was received by CANoe.
 *  \param[out]   dataptr                 This reference points to the buffer where the RX frame payload shall be stored.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE)    Fr_GetRxMsgBufData(uint8 Fr_CtrlIdx, uint32 channel, uint16 slotId,
                                                                                         uint8* dataptr, sint16 length);

/***********************************************************************************************************************
 *  Fr_GetRxMsgBufLength
 **********************************************************************************************************************/
/*! \brief        Get RX frame payload length.
 *  \details      Get the received frame length of the frame with the given channel and slotId.
 *  \param[in]    channel                 Index of FlexRay channel within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \param[in]    slotId                  FlexRay Frame ID the RX frame.
 *  \return       sint16                  Length in bytes of the requested RX frame.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(sint16, FR_CODE)  Fr_GetRxMsgBufLength(uint8 Fr_CtrlIdx, uint32 channel, uint16 slotId);

/***********************************************************************************************************************
 *  Fr_GetRxMsgBufCycle
 **********************************************************************************************************************/
/*! \brief        Get RX frame update cycle.
 *  \details      Get the last update cycle of the RX frame with the given channel and slotId.
 *  \param[in]    channel                 Index of FlexRay channel within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \param[in]    slotId                  FlexRay Frame ID the RX frame.
 *  \return       uint32                  Last update cycle of the requested RX frame.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint32, FR_CODE)  Fr_GetRxMsgBufCycle(uint8 Fr_CtrlIdx, uint32 channel, uint16 slotId);

/***********************************************************************************************************************
 *  Fr_SetRxMsgBufRead
 **********************************************************************************************************************/
/*! \brief        Mark the requested RX frames as read.
 *  \details      Set the received frame length of the given channel and slotId to -1 to indicate that the frame was read.
 *  \param[in]    channel                 Index of FlexRay channel within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \param[in]    slotId                  FlexRay Frame ID the RX frame.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE)    Fr_SetRxMsgBufRead(uint8 Fr_CtrlIdx, uint32 channel, uint16 slotId);

/***********************************************************************************************************************
 *  Fr_SetTxMsgBufPending
 **********************************************************************************************************************/
/*! \brief        Set the frame TX pending state.
 *  \details      Store the frame pending state of the CANoeEmu driver for the given channel, slotId and cycle.
 *  \param[in]    channel                 Index of FlexRay channel within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \param[in]    slotId                  FlexRay Frame ID the TX frame.
 *  \param[in]    cycle                   Cycle of the requested TX frame.
 *  \param[in]    pending                 Defines whether the new frame TX pending state shall be pending or not.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE)    Fr_SetTxMsgBufPending(uint8 Fr_CtrlIdx, uint32 channel, uint16 slotId,
                                                                                         uint16 cycle, boolean pending);

/***********************************************************************************************************************
 *  Fr_GetTxMsgBufPending
 **********************************************************************************************************************/
/*! \brief        Get the frame TX pending state.
 *  \details      Get the current frame pending state of the CANoeEmu driver for the given channel, slotId and cycle.
 *  \param[in]    channel                 Index of FlexRay channel within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \param[in]    slotId                  FlexRay Frame ID the TX frame.
 *  \param[in]    cycle                   Cycle of the requested TX frame.
 *  \return       boolean                 Defines whether the requested frame TX pending state is pending or not.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(boolean, FR_CODE) Fr_GetTxMsgBufPending(uint8 Fr_CtrlIdx, uint32 channel, uint16 slotId,
                                                                                                          uint16 cycle);

/***********************************************************************************************************************
 *  Fr_SetTxMsgBufTransmitted
 **********************************************************************************************************************/
/*! \brief        Set the frame TX state.
 *  \details      Store the transmission state of the CANoeEmu driver for the given channel, slot and cycle.
 *  \param[in]    channel                 Index of FlexRay channel within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \param[in]    slotId                  FlexRay Frame ID the TX frame.
 *  \param[in]    cycle                   Cycle of the requested TX frame.
 *  \param[in]    transmit                Defines whether the new frame TX state shall be transmission requested or not.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE)    Fr_SetTxMsgBufTransmitted(uint8 Fr_CtrlIdx, uint32 channel, uint16 slotId,
                                                                                        uint16 cycle, boolean transmit);

/***********************************************************************************************************************
 *  Fr_GetTxMsgBufTransmitted
 **********************************************************************************************************************/
/*! \brief        Get the frame TX state.
 *  \details      Get current transmission state of the CANoeEmu driver for the given channel, slot and cycle.
 *  \param[in]    channel                 Index of FlexRay channel within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \param[in]    slotId                  FlexRay Frame ID the TX frame.
 *  \param[in]    cycle                   Cycle of the requested TX frame.
 *  \return       boolean                 Defines whether the requested frame TX state is transmission requested or not.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(boolean, FR_CODE) Fr_GetTxMsgBufTransmitted(uint8 Fr_CtrlIdx, uint32 channel, uint16 slotId,
                                                                                                          uint16 cycle);

/***********************************************************************************************************************
 *  Fr_SetPOCStateRealBus
 **********************************************************************************************************************/
/*! \brief        Set real bus POCstate.
 *  \details      Store the given POCstate as real bus state of the CANoeEmu driver.
 *  \param[in]    state                   Requested real bus POCstate
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE)    Fr_SetPOCStateRealBus(uint8 Fr_CtrlIdx, uint32 state);

/***********************************************************************************************************************
 *  Fr_GetPOCStateRealBus
 **********************************************************************************************************************/
/*! \brief        Set real bus POCstate.
 *  \details      Return the current real bus state of the CANoeEmu driver.
 *  \return       uint32                  Current real bus POCstate
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(uint32, FR_CODE)  Fr_GetPOCStateRealBus(uint8 Fr_CtrlIdx);

/***********************************************************************************************************************
 *  Fr_SetUseSimulatedPocState
 **********************************************************************************************************************/
/*! \brief        Enable or disable the POCstate simulation.
 *  \details      Store the simulated POCstate activation state of the CANoeEmu driver.
 *  \param[in]    UseSimulatedPocState    Defines whether the POCstate shall be simulated by FR driver or not.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE)    Fr_SetUseSimulatedPocState(uint8 Fr_CtrlIdx, boolean UseSimulatedPocState);

/***********************************************************************************************************************
 *  Fr_UseSimulatedPocState
 **********************************************************************************************************************/
/*! \brief        Check whether POCstate simulation is enabled or not.
 *  \details      Get the current simulated POCstate activation state of the CANoeEmu driver.
 *  \return       boolean                  Defines whether the POCstate is simulated by FR driver or not.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(boolean, FR_CODE) Fr_UseSimulatedPocState(uint8 Fr_CtrlIdx);

/***********************************************************************************************************************
 *  Fr_SetSynchronized
 **********************************************************************************************************************/
/*! \brief        Set the synchronization state.
 *  \details      Store the synchronization state of the CANoeEmu driver.
 *  \param[in]    UseSimulatedPocState    Defines whether the FR driver is synchronous or not.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE)    Fr_SetSynchronized(uint8 Fr_CtrlIdx, boolean UseSimulatedPocState);

/***********************************************************************************************************************
 *  Fr_IsSynchronized
 **********************************************************************************************************************/
/*! \brief        Get the synchronization state.
 *  \details      Get the current synchronization state of the CANoeEmu driver.
 *  \return       boolean                  Defines whether the FR driver is synchronous or not.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(boolean, FR_CODE) Fr_IsSynchronized(uint8 Fr_CtrlIdx);

/***********************************************************************************************************************
 *  Fr_RegisterMessages
 **********************************************************************************************************************/
/*! \brief        Register the FlexRay frames in CANoe.
 *  \details      Register the FlexRay frames in CANoe according the read message buffer configuration.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE)    Fr_RegisterMessages(void);

/**********************************************************************************************************************
 *  Fr_VExecutePOCCommand
 *********************************************************************************************************************/
/*! \brief        Requests the CC to go in appropriate POCstate
 *  \details      Internal helper function that requests the CC to go into POCstate Fr_PocCommand.
 *  \param[in]    pocCommand              Requested POCstate
 *  \return       E_OK                    Transition to requested POCstate passed
 *  \return       E_NOT_OK                PocCommand was not accepted
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FR_LOCAL FUNC(Std_ReturnType, FR_CODE) Fr_ExecutePOCCommand(uint8 Fr_CtrlIdx, uint8 pocCommand);

/**********************************************************************************************************************
 *  Fr_VStartStopMessages
 *********************************************************************************************************************/
/*! \brief        Start or stop sending messages in the static segment via VN-interface.
 *  \details      Set the requested FlexRay message status of the VN-interface for the current L-PDU to either ON or OFF.
 *  \param[in]    value                   Defines whether the VN-interface messages in the static segment shall be send (1)
 *                                        or not (0).
 *  \return       E_OK                    All VN-interface messages were reconfigure properly.
 *  \return       E_NOT_OK                PocCommand was not accepted
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FR_LOCAL FUNC(Std_ReturnType, FR_CODE) Fr_VStartStopMessages(uint8 Fr_CtrlIdx, int value); /* 1=on 0=off */

#if ( FR_GETSYNCFRAMELIST_API == STD_ON ) || ( FR_VENABLE_MID_FILTERING == STD_ON )
/**********************************************************************************************************************
 *  Fr_GetRxMsgBufFlags
 *********************************************************************************************************************/
/*! \brief        Get the FlexRay flags a RX frame with the given channel and slotId..
 *  \details      Get the FlexRay flags of the RX frame with the given channel and slotId.
 *  \param[in]    channel                 Index of FlexRay channel within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \param[in]    slotId                  FlexRay Frame ID the TX frame.
 *  \return       uint32                  FlexRay flags of the requested CANoe frame.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       FR_GETSYNCFRAMELIST_API, FR_VENABLE_MID_FILTERING
 *********************************************************************************************************************/
FR_LOCAL FUNC(uint32, FR_CODE)  Fr_GetRxMsgBufFlags(uint8 Fr_CtrlIdx, uint32 channel, uint16 slotId);
#endif

#if ( FR_NMVECTOR_ENABLE == STD_ON )
/**********************************************************************************************************************
 *  Fr_GetCanoeNMVector
 *********************************************************************************************************************/
/*! \brief        Get NM vector payload.
 *  \details      Copy stored NM vector payload to given data pointer.
 *  \param[out]   dataptr                 This reference points to the buffer where the NM vector payload shall be stored.
 *  \return       uint16                  Length in bytes of the copied NM vector payload.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       FR_NMVECTOR_ENABLE
 *********************************************************************************************************************/
FR_LOCAL FUNC(uint16, FR_CODE)  Fr_GetCanoeNMVector(uint8 Fr_CtrlIdx, uint8* dataptr);
#endif
#if (Fr_IsPostbuild())
/**********************************************************************************************************************
 *  Fr_VSetGlobalFrConfigPtr
 *********************************************************************************************************************/
/*! \brief        Set the pointer to the FR driver config struct for Fr_Init().
 *  \details      Store Fr_ConfigPtr to be used by Fr_Init() further on.
 *  \param[in]    Fr_ConfigPtr            Component configuration structure.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       FR_POSTBUILD_VARIANT_SUPPORT, FR_CONFIGURATION_VARIANT
 *********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE)    Fr_VSetGlobalFrConfigPtr(P2CONST(Fr_VConfigType, AUTOMATIC, FR_PBCFG) Fr_ConfigPtr);

# if (FR_VTT == STD_OFF) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
/*********************************************************************************************************************
 *  ApplFr_GetFrConfigPtr
 *********************************************************************************************************************/
/*! \brief        Get the pointer to the FR config struct from the application.
 *  \details      Application callback to get the FR config struct for the FR driver initialization.
 *  \param[out]   Fr_ConfigPtr            Component configuration structure.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       FR_POSTBUILD_VARIANT_SUPPORT, FR_CONFIGURATION_VARIANT, FR_VTT
 *********************************************************************************************************************/
FUNC(void, FR_APPL_CODE) ApplFr_GetFrConfigPtr(P2CONST(Fr_ConfigType, AUTOMATIC, FR_PBCFG) *Fr_ConfigPtr);
# endif 
#endif

#if (FR_VTT == STD_OFF) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */

/***********************************************************************************************************************
 *  Fr_RxHandler
 **********************************************************************************************************************/
/*! \brief        Event-handler for receive frames.
 *  \details      Store the given RX frame payload, FlexRay flags and the update informations for the frame with the
 *                given ctrlId and slotId.
 *  \param[in]    ctrlId                  Index of FlexRay ctrlId within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \param[in]    channelABMask           Channel mask of the received frame.
 *  \param[in]    slotId                  FlexRay Frame ID the RX frame.
 *  \param[in]    cycle                   Cycle in which the FlexRay frame was received by CANoe.
 *  \param[in]    segment                 Indicate whether the RX frame was in the static or dynamic segment.
 *  \param[in]    flags                   FlexRay flags of the CANoe frame.
 *  \param[in]    payloadLen              Length in bytes of the received frame.
 *  \param[in]    data                    This reference points to the buffer of the received payload.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       FR_VTT
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE) Fr_RxHandler(uint8 ctrlId, uint32 channelABMask, uint32 slotId, uint16 cycle,
                                                  sint16 segment, sint32  flags, uint16 payloadLen,  const uint8 data[]);

/***********************************************************************************************************************
 *  Fr_TxHandler
 **********************************************************************************************************************/
/*! \brief        Event-handler for transmit frames.
 *  \details      Store the frame pending state of the CANoeEmu driver for the given ctrlId, slotId and cycle.
 *  \param[in]    ctrlId                  Index of FlexRay ctrlId within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \param[in]    channelABMask           Channel mask of the transmit frame.
 *  \param[in]    slotId                  FlexRay Frame ID the TX frame.
 *  \param[in]    cycle                   Cycle in which the FlexRay frame was transmitted by CANoe.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       FR_VTT
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE) Fr_TxHandler(uint8 ctrlId, uint32  channelABMask, uint32 slotId, uint16 cycle);

/***********************************************************************************************************************
 *  Fr_StartCycleHandler
 **********************************************************************************************************************/
/*! \brief        Cycle-Start interrupt handler.
 *  \details      Handle POCstate state-machine depending on pending POCstate request, if bus simulation is enabled.
 *  \param[in]    ctrlId                  Index of FlexRay ctrlId within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \param[in]    cycle                   Cycle in which the cycle-start interrupt occurred in CANoe.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       FR_VTT
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE) Fr_StartCycleHandler(uint8 ctrlId, uint16 cycle);

/***********************************************************************************************************************
 *  Fr_NmVectorHandler
 **********************************************************************************************************************/
/*! \brief        NM-vector event-handler.
 *  \details      Store the received NM vector payload and length.
 *  \param[in]    ctrlId                  Index of FlexRay ctrlId within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \return       len                     Payloadlength in units of bytes of the NM-vector.
 *  \param[in]    cycle                   Cycle in which NM-Vector was received by CANoe.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       FR_VTT
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE) Fr_NmVectorHandler(uint8 ctrlId, uint16 len, const  uint8 nmVec[]);

/***********************************************************************************************************************
 *  Fr_StatusHandler
 **********************************************************************************************************************/
/*! \brief        FlexRay bus state event-handler.
 *  \details      Event handler of CANoe which is called if the FlexRay bus synchronization state changes.
 *  \param[in]    ctrlId                  Index of FlexRay ctrlId within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \return       state                   Indicating new FlexRay bus state (1 = sync, 0 = no sync)
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       FR_VTT
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE) Fr_StatusHandler(uint8 ctrlId, uint32 state);

/***********************************************************************************************************************
 *  Fr_TimerHandler
 **********************************************************************************************************************/
/*! \brief        FlexRay timer event-handler.
 *  \details      Calls the Fr_IrqTimer0 interrupt handler, if the absolute timer has expired (based on the current CANoe
 *                simulation time).
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       FR_VTT
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE) Fr_TimerHandler(void);

/***********************************************************************************************************************
 *  Fr_SlotHandler
 **********************************************************************************************************************/
/*! \brief        FlexRay slot event-handler.
 *  \details      Call the Fr_TimerHandler to poll the absolute timer interrupt with each received or transmitted FlexRay
 *                frame.
 *  \param[in]    channel                 Index of FlexRay channel within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \param[in]    slotId                  FlexRay Frame ID the frame.
 *  \param[in]    cycle                   Cycle in which the FlexRay frame was received or transmitted by CANoe.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       FR_VTT
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE) Fr_SlotHandler(uint8 ctrlId, uint32 slotId, uint16 cycle);

/***********************************************************************************************************************
 *  Fr_POCStateHandler
 **********************************************************************************************************************/
/*! \brief        FlexRay POCstate event-handler.
 *  \details      This function is called on POC state change of the VN-interface.
 *  \param[in]    ctrlId                  Index of FlexRay ctrlId within the context of the FlexRay CC Fr_CtrlIdx.
 *                                        Valid values are FR_CHANNEL_A and FR_CHANNEL_B.
 *  \param[in]    pocState                FlexRay Frame ID the frame.
 *  \param[in]    Fr_Info1                FlexRay Bus state:
 *                                          0= synchronized, passive,
 *                                          1= synchronized, active,
 *                                          2= not synchronized,
 *                                          3= Unknown
 *  \param[in]    Fr_Info2                Wakeup information:
 *                                          0= WAKEUP UNDEFINED,
 *                                          1= WAKEUP RECEIVED HEADER,
 *                                          2= WAKEUP RECEIVED WUP,
 *                                          3= WAKEUP COLLISION HEADER,
 *                                          4= WAKEUP COLLISION WUP,
 *                                          5= WAKEUP COLLISION UNKNOWN,
 *                                          6= WAKEUP TRANSMITTED,
 *                                          7=  WAKEUP RECEIVED
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       FR_VTT
 **********************************************************************************************************************/
FR_LOCAL FUNC(void, FR_CODE) Fr_POCStateHandler(uint8 ctrlId, uint32  pocState, uint32  Fr_Info1, uint32  Fr_Info2);

/***********************************************************************************************************************
 *  Fr_VGetCurrentMacroTick
 **********************************************************************************************************************/
/*! \brief        Get the current macrotick value.
 *  \details      Calculate and return the current macrotick value based on the current simulation time.
 *  \return       long                    Current macrotick
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       FR_VTT
 **********************************************************************************************************************/
FR_LOCAL FUNC(unsigned long, FR_CODE) Fr_VGetCurrentMacroTick(uint8 Fr_CtrlIdx);
#endif /* #if (FR_VTT == STD_OFF) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  FlexRay_Init
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
#if (FR_VTT == STD_ON) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
void FlexRay_Init(void)
#else
__declspec( dllexport ) void FlexRay_Init(void)
#endif /* #if (FR_VTT == STD_ON) */
{
  /* #10 Register the FlexRay frame in CANoe according the message buffer configuration of the FR driver. */
  Fr_RegisterMessages();
} /* FlexRay_Init() */

/***********************************************************************************************************************
 *  Fr_RxHandler
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
 */
#if (FR_VTT == STD_ON) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
FUNC(void, FR_CODE) Fr_OnFlexRayFrameHandler(
#else
FR_LOCAL FUNC(void, FR_CODE) Fr_RxHandler(
#endif /* #if (FR_VTT == STD_ON) */
  uint8  ctrlId,         /* FlexRay cluster */
  uint32 channelABMask,  /* 1: CH A, 2: CH B, 3: CH A&B */
  uint32 slotId,
  uint16 cycle,
  sint16 segment,
  sint32 flags,
  uint16 payloadLen,
  const  uint8 data[])
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 j;
  uint8 i;
  boolean RxFlag = FALSE;
  boolean TxFlag = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(segment); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 If the requested slotId is valid: */
  if(    (slotId > 0)
      && (slotId < 2048))
  {
    /* #20 If the wakeup channel was not yet set:
     *       Set the wakeup channel according the given channel mask of the received frame. */
    if (wakeupChannelValid[ctrlId] == FALSE)
    {
      wakeupChannelValid[ctrlId] = TRUE;
      if (channelABMask == 1)
      {
        wakeupChannel[ctrlId] = FR_CHANNEL_A;
      }
      else if (channelABMask == 2)
      {
        wakeupChannel[ctrlId] = FR_CHANNEL_B;
      }
      else
      { /* Choose channel A as wakeup receiver in case the first frame is received on both channels. */
        wakeupChannel[ctrlId] = FR_CHANNEL_A;
      }
    }

    for(i = 1; i <= 2; i++)
    {
      if((channelABMask & (uint32)i) != 0)
      {
        for(j = 0; j < 128; j++)    /* 128 = cycle code von ERay*/
        {
          /* #30 Determine the cycle offset, cycle repetition and cycle mask of the received frame. */
          uint8 base = Fr_GetBaseCycle((uint8)j);
          uint8 repetition = Fr_GetRepetition((uint8)j);
          uint8 cycMask = Fr_GetCycleMask(repetition);

          if((cycle & cycMask) == base)
          {
            if(FALSE != (boolean)Fr_GetTxMsgBufTransmitted(ctrlId, i,(uint16)slotId,(uint16)j))
            {
              TxFlag = TRUE;
              break;
            }
            else
            {
              RxFlag = TRUE;
            }
          }
        }

        /* #40 If the cycle of the RX indication matches with the read cycle offset and cycle mask:
         *       If the RX indication is for a frame that was previously transmitted by the CANoeEmu FR driver:
         *         Set the transmission state of the message buffer to TX pending.
         *       Otherwise:
         *         Store the received payload, FlexRay flags and update informations of the indicated RX frame.
         */

        if(TxFlag == TRUE)
        {
          /* This is a frame that we transmitted ourself  */
          Fr_SetTxMsgBufTransmitted(ctrlId, (uint32)i, (uint16)slotId, (uint16)j, FALSE);
          Fr_SetTxMsgBufPending(ctrlId, (uint32)i, (uint16)slotId, (uint16)j, FALSE);
          TxFlag = FALSE; /* default for next ctrlId */
          RxFlag = FALSE; /* no Rx if tx */
        }
        if(RxFlag == TRUE)
        {
          Fr_SetRxMsgBuf(ctrlId, (uint32)i, (uint16)slotId, data, payloadLen, flags, cycle);
          RxFlag = FALSE;   /* default for next ctrlId */
        }
      }
    }
  }

  /* PRQA S 6080 3 */ /* MD_MSR_STMIF */
  /* PRQA S 6030 2 */ /* MD_MSR_STCYC */
  /* PRQA S 6060 1 */ /* MD_MSR_STPAR */
} /* Fr_OnFlexRayFrameHandler() / Fr_RxHandler() */

/***********************************************************************************************************************
 *  Fr_SetMsgBuf
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(void, FR_CODE) Fr_SetRxMsgBuf(
  uint8 Fr_CtrlIdx,
  uint32 channel,
  uint16 slotId,
  const uint8* dataptr,
  sint16 length,
  uint32 flags,
  uint16 cycle)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Store the given RX frame payload, FlexRay flags and the update informations for the frame with the given channel and slotId. */
  VStdMemCpy(gReceiveData[FR_VCTRL_IDX][channel][slotId].Data, dataptr, length); /* PRQA S 0311 */ /* MD_Fr_0311 */
  gReceiveData[FR_VCTRL_IDX][channel][slotId].Length = length;
  gReceiveData[FR_VCTRL_IDX][channel][slotId].flags = (uint32)flags;
  gReceiveData[FR_VCTRL_IDX][channel][slotId].cycle = cycle;

  /* #30 Leave VttFr_AtomicBegin */
  VttFr_AtomicEnd();

  /* PRQA S 6060 1 */ /* MD_MSR_STPAR */
} /* Fr_SetRxMsgBuf() */

/***********************************************************************************************************************
 *  Fr_GetRxMsgBufData
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
FR_LOCAL FUNC(void, FR_CODE) Fr_GetRxMsgBufData(
  uint8 Fr_CtrlIdx,
  uint32 channel,
  uint16 slotId,
  uint8* dataptr,
  sint16 length)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check the requested length of the RX frame to be received. */
  if (length == -1)
  {
    errorId = FR_E_INV_LENGTH;
  }
  /* #20 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Enter VttFr_AtomicBegin */
    VttFr_AtomicBegin();

    /* #40 Copy requested length bytes of the RX frame payload with the given channel and slotId to the data pointer. */
    VStdMemCpy(dataptr, gReceiveData[FR_VCTRL_IDX][channel][slotId].Data, length);

    /* #50 Leave VttFr_AtomicEnd */
    VttFr_AtomicEnd();
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_RECEIVE_RX_LPDU, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
} /* Fr_GetRxMsgBufData() */

#if ( FR_GETSYNCFRAMELIST_API == STD_ON ) || ( FR_VENABLE_MID_FILTERING == STD_ON )
/***********************************************************************************************************************
 *  Fr_GetRxMsgBufFlags
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(uint32, FR_CODE) Fr_GetRxMsgBufFlags(
  uint8 Fr_CtrlIdx,
  uint32 channel,
  uint16 slotId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Get the FlexRay flags of the RX frame with the given channel and slotId. */
  retVal = (uint32)gReceiveData[FR_VCTRL_IDX][channel][slotId].flags;

  /* #30 Leave VttFr_AtomicEnd */
  VttFr_AtomicEnd();

  return retVal;
} /* Fr_GetRxMsgBufFlags() */
#endif /* ( FR_GETSYNCFRAMELIST_API == STD_ON ) || ( FR_VENABLE_MID_FILTERING == STD_ON ) */

/***********************************************************************************************************************
 *  Fr_GetRxMsgBufLength
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(sint16, FR_CODE) Fr_GetRxMsgBufLength(
  uint8 Fr_CtrlIdx,
  uint32 channel,
  uint16 slotId)
{
  /* ----- Local Variables ---------------------------------------------- */
  sint16 retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Get the received frame length of the frame with the given channel and slotId. */
  retVal = gReceiveData[FR_VCTRL_IDX][channel][slotId].Length;

  /* #30 Leave VttFr_AtomicEnd */
  VttFr_AtomicEnd();

  return retVal;
} /* Fr_GetRxMsgBufLength() */

/***********************************************************************************************************************
 *  Fr_GetRxMsgBufCycle
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(uint32, FR_CODE) Fr_GetRxMsgBufCycle(
  uint8 Fr_CtrlIdx,
  uint32 channel,
  uint16 slotId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 retVal;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Get the last update cycle of the RX frame with the given channel and slotId. */
  retVal = gReceiveData[FR_VCTRL_IDX][channel][slotId].cycle;

  /* #30 Leave VttFr_AtomicEnd */
  VttFr_AtomicEnd();

  return retVal;
} /* Fr_GetRxMsgBufCycle() */

/***********************************************************************************************************************
 *  Fr_SetRxMsgBufRead
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(void, FR_CODE) Fr_SetRxMsgBufRead(
  uint8 Fr_CtrlIdx,
  uint32 channel,
  uint16 slotId)
{
  /* ----- Implementation ----------------------------------------------- */

  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Set the received frame length of the given channel and slotId to -1 to indicate that the frame was read. */
  gReceiveData[FR_VCTRL_IDX][channel][slotId].Length = (sint16)(-1);

  /* #30 Leave VttFr_AtomicEnd */
  VttFr_AtomicEnd();
} /* Fr_SetRxMsgBufRead() */

/***********************************************************************************************************************
 *  Fr_SetTxMsgBufPending
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(void, FR_CODE) Fr_SetTxMsgBufPending(
  uint8 Fr_CtrlIdx,
  uint32 channel,
  uint16 slotId,
  uint16 cycle,
  boolean pending)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Store the frame pending state of the CANoeEmu driver for the given channel, slotId and cycle. */
  gTxFramePending[FR_VCTRL_IDX][channel][slotId][cycle] = pending;

  /* #30 Leave VttFr_AtomicEnd */
  VttFr_AtomicEnd();
} /* Fr_SetTxMsgBufPending() */

/***********************************************************************************************************************
 *  Fr_GetTxMsgBufPending
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(boolean, FR_CODE) Fr_GetTxMsgBufPending(
  uint8 Fr_CtrlIdx,
  uint32 channel,
  uint16 slotId,
  uint16 cycle)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Get the current frame pending state of the CANoeEmu driver for the given channel, slotId and cycle. */
  retVal = gTxFramePending[FR_VCTRL_IDX][channel][slotId][cycle];
  /* #30 Leave VttFr_AtomicEnd */
  VttFr_AtomicEnd();

  return retVal;
} /* Fr_GetTxMsgBufPending() */

/***********************************************************************************************************************
 *  Fr_SetTxMsgBufTransmitted
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(void, FR_CODE) Fr_SetTxMsgBufTransmitted(
  uint8 Fr_CtrlIdx,
  uint32 channel,
  uint16 slotId,
  uint16 cycle,
  boolean transmit)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Store the transmission state of the CANoeEmu driver for the given channel, slot and cycle. */
  gTxFrameTransmitted[FR_VCTRL_IDX][channel][slotId][cycle] = transmit;

  /* #30 Leave VttFr_AtomicEnd */
  VttFr_AtomicEnd();
} /* Fr_SetTxMsgBufTransmitted() */

/***********************************************************************************************************************
 *  Fr_GetTxMsgBufTransmitted
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(boolean, FR_CODE) Fr_GetTxMsgBufTransmitted(
  uint8 Fr_CtrlIdx,
  uint32 channel,
  uint16 slotId,
  uint16 cycle
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Get current transmission state of the CANoeEmu driver for the given channel, slot and cycle . */
  retVal = gTxFrameTransmitted[FR_VCTRL_IDX][channel][slotId][cycle];

  /* #30 Leave VttFr_AtomicEnd */
  VttFr_AtomicEnd();

  return retVal;
} /* Fr_GetTxMsgBufTransmitted() */

#if ( FR_NMVECTOR_ENABLE == STD_ON )
/***********************************************************************************************************************
 *  Fr_GetCanoeNMVector
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(uint16, FR_CODE) Fr_GetCanoeNMVector(uint8 Fr_CtrlIdx, uint8* dataptr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 n;
  uint16 end = gNmVectorLen[FR_VCTRL_IDX];

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Copy stored NM vector payload to given data pointer. */
  end = (uint16)Fr_Min(Fr_GetNMVectorLength(FR_VCTRL_IDX), end);
  for(n = 0; n < end; n++)
  {
    dataptr[n] = gNmVectorData[FR_VCTRL_IDX][n];
  }

  /* #30 Leave VttFr_AtomicEnd */
  VttFr_AtomicEnd();
  return end;
} /* Fr_GetCanoeNMVector() */
#endif /* ( FR_NMVECTOR_ENABLE == STD_ON ) */

/***********************************************************************************************************************
*  Fr_SetPOCStateRealBus
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(void, FR_CODE) Fr_SetPOCStateRealBus(uint8 Fr_CtrlIdx, uint32 state)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Store the given POCstate as real bus state of the CANoeEmu driver. */
  gPOCStateRealBus[FR_VCTRL_IDX] = state;

  /* #30 Leave VttFr_AtomicEnd */
  VttFr_AtomicEnd();
} /* Fr_SetPOCStateRealBus() */

/***********************************************************************************************************************
*  Fr_GetPOCStateRealBus
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(uint32, FR_CODE) Fr_GetPOCStateRealBus(uint8 Fr_CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 retval;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Return the current real bus state of the CANoeEmu driver. */
  retval = gPOCStateRealBus[FR_VCTRL_IDX];

  /* #30 Leave VttFr_AtomicEnd */
  VttFr_AtomicEnd();

  return retval;
} /* Fr_GetPOCStateRealBus() */

/***********************************************************************************************************************
*  Fr_SetUseSimulatedPocState
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(void, FR_CODE) Fr_SetUseSimulatedPocState(uint8 Fr_CtrlIdx, boolean UseSimulatedPocState)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Store the simulated POCstate activation state of the CANoeEmu driver. */
  gUseSimulatedPocState[FR_VCTRL_IDX] = UseSimulatedPocState;

  /* #30 Leave VttFr_AtomicEnd */
  VttFr_AtomicEnd();
} /* Fr_SetUseSimulatedPocState() */

/***********************************************************************************************************************
*  Fr_UseSimulatedPocState
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(boolean, FR_CODE) Fr_UseSimulatedPocState(uint8 Fr_CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retval;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Get the current simulated POCstate activation state of the CANoeEmu driver. */
  retval = gUseSimulatedPocState[FR_VCTRL_IDX];

  /* #30 Leave VttFr_AtomicEnd */
  VttFr_AtomicEnd();

  return retval;
} /* Fr_UseSimulatedPocState() */

/***********************************************************************************************************************
*  Fr_SetSynchronized
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(void, FR_CODE) Fr_SetSynchronized(uint8 Fr_CtrlIdx, boolean UseSimulatedPocState)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Store the synchronization state of the CANoeEmu driver. */
  gIsSynchronized[FR_VCTRL_IDX] = UseSimulatedPocState;

  /* #30 Leave VttFr_AtomicEnd */
  VttFr_AtomicEnd();
} /* Fr_SetSynchronized() */

/***********************************************************************************************************************
*  Fr_IsSynchronized
**********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(boolean, FR_CODE) Fr_IsSynchronized(uint8 Fr_CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retval;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Get the current synchronization state of the CANoeEmu driver. */
  retval = gIsSynchronized[FR_VCTRL_IDX];

  /* #30 Leave VttFr_AtomicEnd */
  VttFr_AtomicEnd();

  return retval;
} /* Fr_IsSynchronized() */

#if (FR_VTT == STD_OFF) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
/***********************************************************************************************************************
 *  Fr_TxHandler
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(void, FR_CODE) Fr_TxHandler(
  uint8  ctrlId,         /* FlexRay cluster */
  uint32 channelABMask,  /* 1: CH A, 2: CH B, 3: CH A&B */
  uint32 slotId,
  uint16 cycle )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 j;
  uint8 i;

  /* ----- Implementation ----------------------------------------------- */
  for(i = 1; i <= 2; i++)
  {
    if((channelABMask & (uint32)i) != 0)
    {
      for(j = 0; j < 128; j++)
      {
        /* #10 Determine the cycle offset, cycle repetition and cycle mask of the transmitted frame. */
        uint8 base = Fr_GetBaseCycle(j);
        uint8 repetition = Fr_GetRepetition(j);
        uint8 cycMask = Fr_GetCycleMask(repetition);

        /* #20 If the cycle of the TX indication matches with the read cycle offset and cycle mask:
         *       Store the frame pending state of the CANoeEmu driver for the given channel, slotId and cycle.
         */
        if((cycle & cycMask) == base)
        {
          Fr_SetTxMsgBufPending(ctrlId, i,(uint16)slotId, j,  FALSE);
        }
      }
    }
  }
} /* Fr_TxHandler */
#endif /* #if (FR_VTT == STD_OFF) */

/***********************************************************************************************************************
 *  Fr_StartCycleHandler
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
#if (FR_VTT == STD_ON) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
FUNC(void, FR_CODE) Fr_OnFlexRayStartCycleHandler(
#else
FR_LOCAL FUNC(void, FR_CODE) Fr_StartCycleHandler(
#endif /* #if (FR_VTT == STD_ON) */
  uint8  ctrlId, /* FlexRay cluster */
  uint16 cycle)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  gCurrentCycle[ctrlId] = (uint8)cycle;
  gStartCycleTime[ctrlId] = VttFr_GetCurrentSimTime();

  /* #10 If bus simulation is enabled:
   *       Handle POCstate state-machine depending on pending POCstate request. */
  if (gHaltRequest[ctrlId] != FALSE)
  {
    (void)Fr_ExecutePOCCommand(ctrlId, FR_POCSTATE_HALT);
    gHaltRequest[ctrlId] = FALSE;
  }
  /* PRQA S 4101, 4102, 4105 BITWISEOPERATION */ /* MD_Fr_4102 */
  else if ((((Fr_IsSynchronized(ctrlId) == FALSE) | (gPreventSync[ctrlId] == TRUE) | (gNormalPassive[ctrlId] == TRUE))
    & (gSimulatedPOCState[ctrlId] == FR_POCSTATE_NORMAL_ACTIVE)) != FALSE)
  {
    gSimulatedPOCState[ctrlId] = FR_POCSTATE_NORMAL_PASSIVE;
  }
  else if ((((Fr_IsSynchronized(ctrlId) == FALSE) | (gPreventSync[ctrlId] == TRUE))
    & (gSimulatedPOCState[ctrlId] == FR_POCSTATE_NORMAL_PASSIVE)) != FALSE)
  {
    if ((vFrCcRegVals[ctrlId].SUCC1 & FR_SUCC1_HCSE) != 0)
    {
      gSimulatedPOCState[ctrlId] = FR_POCSTATE_HALT;
    }
  }
  else if (((Fr_IsSynchronized(ctrlId) != FALSE)
    & (gSimulatedPOCState[ctrlId] == FR_POCSTATE_NORMAL_PASSIVE)
    & (gNormalPassive[ctrlId] == FALSE)) != FALSE)
  {
    if ((vFrCcRegVals[ctrlId].SUCC1 & FR_SUCC1_PTA_MASK) != 0)
    {
      gSimulatedPOCState[ctrlId] = FR_POCSTATE_NORMAL_ACTIVE;
    }
  }
  else if (((gSimulatedPOCState[ctrlId] == FR_POCSTATE_STARTUP)
    & (gPreventSync[ctrlId] == FALSE)) != FALSE)
  {
    Fr_SetSynchronized(ctrlId, TRUE);
    gSimulatedPOCState[ctrlId] = FR_POCSTATE_NORMAL_ACTIVE;
  }
  else if (gSimulatedPOCState[ctrlId] == FR_POCSTATE_WAKEUP)
  {
    gSimulatedPOCState[ctrlId] = FR_POCSTATE_READY;
  }
  /* PRQA L:BITWISEOPERATION */ /* MD_Fr_4102 */
  else
  {
    /* unknown gSimulatedPOCState -> do nothing */
  }
#if (FR_VTT == STD_OFF) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
  /* React as cycle interrupt */
# if (FR_CFG_APPL_CALLBACK_CYCLE_START == STD_ON) /* COV_FR_TESTSUITE_INSTRUMENTATION */
  /* #20 If FR_CFG_APPL_CALLBACK_CYCLE_START is enabled:
   *       Call Fr_IrqLine0 callback.
   */
  if (ctrlId == FR_COM_CONTROLLER_0)
  {
#  if(FR_CFG_STATUS_ISR_LINE == 0)
    Fr_IrqLine0();
#  else
    Fr_IrqLine1();
#  endif
  }
#  if (FR_NUM_CTRL_USED > 1)
  else /* (ctrlId == FR_COM_CONTROLLER_1) */
  {
#   if(FR_CFG_STATUS_ISR_LINE_1 == 0)
    Fr_IrqLine0_1();
#   else
    Fr_IrqLine1_1();
#   endif
  }
#  endif
# endif
#endif /* #if (FR_VTT == STD_OFF) */
  /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
} /* Fr_OnFlexRayStartCycleHandler() / Fr_StartCycleHandler() */

/***********************************************************************************************************************
 *  Fr_NmVectorHandler
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
#if (FR_VTT == STD_ON) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
FUNC(void, FR_CODE) Fr_OnFlexRayNmVectorHandler(
#else
FR_LOCAL FUNC(void, FR_CODE) Fr_NmVectorHandler(
#endif /* #if (FR_VTT == STD_ON) */
  uint8  ctrlId, /* FlexRay cluster */
  uint16 len,
  const  uint8 nmVec[])
{
#if ( FR_NMVECTOR_ENABLE == STD_ON )
  /* ----- Local Variables ---------------------------------------------- */
  uint16 n;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter VttFr_AtomicBegin */
  VttFr_AtomicBegin();

  /* #20 Store the received NM vector payload and length. */
  if (nmVec[0] != 0) /* first byte of NMVector holds the FRNM voting information */
  {
    for(n = 0; n < len; n++)
    {
      gNmVectorData[ctrlId][n] = nmVec[n];
    }
    gNmVectorLen[ctrlId] = len;
  }

  /* #30 Enter VttFr_AtomicBegin */
  VttFr_AtomicEnd();
#else
  FR_DUMMY_STATEMENT(len); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  FR_DUMMY_STATEMENT(ctrlId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  FR_DUMMY_STATEMENT(nmVec); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif /* ( FR_NMVECTOR_ENABLE == STD_ON ) */
} /*Fr_OnFlexRayNmVectorHandler() / Fr_NmVectorHandler() */

/***********************************************************************************************************************
 *  Fr_StatusHandler
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
#if (FR_VTT == STD_ON) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
FUNC(void, FR_CODE) Fr_OnFlexRayStatusHandler(
#else
FR_LOCAL FUNC(void, FR_CODE) Fr_StatusHandler(
#endif /* #if (FR_VTT == STD_ON) */
  uint8  ctrlId, /* FlexRay cluster */
  uint32 state) /* 1 = sync, 0 = no sync ? */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the FR driver synchronization state depending on the given FlexRay bus state. */
  Fr_SetSynchronized(ctrlId, (boolean)(((uint32)1 == state) ? TRUE : FALSE));
} /* Fr_OnFlexRayStatusHandler() / Fr_StatusHandler() */

#if (FR_VTT == STD_OFF) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
/***********************************************************************************************************************
 *  Fr_TimerHandler
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FR_LOCAL FUNC(void, FR_CODE) Fr_TimerHandler(void)
{
  /* ----- Implementation ----------------------------------------------- */
  uint8_least Fr_CtrlIdx;

  /* #10 If the absolute timer has expired (based on the current CANoe simulation time):
   *       Call the Fr_IrqTimer0 interrupt handler.
   */
  for (Fr_CtrlIdx = 0; Fr_CtrlIdx < FR_NUM_CTRL_USED; Fr_CtrlIdx++)
  {
    if(    (gTimeoutTimes[FR_VCTRL_IDX][0] <= VttFr_GetCurrentSimTime())
        && (gTimeoutTimes[FR_VCTRL_IDX][0] >= 0)
        && (gTimerEnabled[FR_VCTRL_IDX][0] == TRUE) )
    {
      gTimeoutTimes[FR_VCTRL_IDX][0] = -1;
      FR_IRQ_TIMER(FR_VCTRL_IDX)
    }

    if(    (gTimeoutTimes[FR_VCTRL_IDX][1] <= VttFr_GetCurrentSimTime())
        && (gTimeoutTimes[FR_VCTRL_IDX][1] >= 0)
        && (gTimerEnabled[FR_VCTRL_IDX][1] == TRUE))
    {
      gTimeoutTimes[FR_VCTRL_IDX][1] = -1;
    }
  }
} /* Fr_TimerHandler() */
#endif /* #if (FR_VTT == STD_OFF) */

#if (FR_VTT == STD_OFF) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
/***********************************************************************************************************************
 *  Fr_SlotHandler
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(void, FR_CODE) Fr_SlotHandler(uint8 ctrlId, uint32 slotId, uint16 cycle)
{
  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(ctrlId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  FR_DUMMY_STATEMENT(slotId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  FR_DUMMY_STATEMENT(cycle); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Call the Fr_TimerHandler to poll the absolute timer interrupt with each received or transmitted FlexRay frame. */
  Fr_TimerHandler();
} /* Fr_SlotHandler() */
#endif /* #if (FR_VTT == STD_OFF) */

/***********************************************************************************************************************
 *  Fr_POCStateHandler
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
#if (FR_VTT == STD_ON) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
FUNC(void, FR_CODE) Fr_OnFlexRayPOCStateHandler(
#else
FR_LOCAL FUNC(void, FR_CODE) Fr_POCStateHandler(
#endif /* #if (FR_VTT == STD_ON) */
  uint8  ctrlId,    /* FlexRay cluster */
  uint32 pocState,   /* POC state */
  uint32 Fr_Info1,   /* 0= synchronized, passive, 1= synchronized, active, 2= not synchronized, 3= Unknown */
  uint32 Fr_Info2    /* 0= WAKEUP UNDEFINED, 1= WAKEUP RECEIVED HEADER, 2= WAKEUP RECEIVED WUP,  3= WAKEUP COLLISION HEADER, 
                         4= WAKEUP COLLISION WUP, 5= WAKEUP COLLISION UNKNOWN, 6= WAKEUP TRANSMITTED, 7=  WAKEUP RECEIVED*/
    )
{
  /* ----- Local Variables ---------------------------------------------- */
  int busMode = 0;

  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_Info1); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  FR_DUMMY_STATEMENT(Fr_Info2); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Store the given POCstate as real bus state of the CANoeEmu driver. */
  Fr_SetPOCStateRealBus(ctrlId, pocState);

  /* #20 If the simulation mode is enabled in CANoe:
   *         Configure the FR driver to use the simulated POCstate.
   *     Otherwise:
   *         Configure Configure the FR driver to use the POCstate of the VN-interface. */
  busMode = VttFr_GetSimulationMode();
  switch (busMode)
  {
    case FR_CANOE_BUSMODE_UNKNOWN:   /* CANoes older than 7.1 SP3 will not return this value */
      Fr_SetUseSimulatedPocState(ctrlId, FALSE);
      break;
    case FR_CANOE_BUSMODE_SIMULATE:  /* CANoe is in simulation mode */
      Fr_SetUseSimulatedPocState(ctrlId, TRUE);
      break;
    case FR_CANOE_BUSMODE_REAL:      /* CANoe is in real mode */
      Fr_SetUseSimulatedPocState(ctrlId, FALSE);
      break;
    default:
      break;
  }
} /* Fr_OnFlexRayPOCStateHandler() / Fr_POCStateHandler() */

/***********************************************************************************************************************
 *  Fr_VGetCurrentMacroTick
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FR_LOCAL FUNC(unsigned long, FR_CODE) Fr_VGetCurrentMacroTick(uint8 Fr_CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* #10 Get the current simulation time of CANoe. */
  __int64 SimTime = VttFr_GetCurrentSimTime();

  /* ----- Implementation ----------------------------------------------- */
  /* #20 Calculate and return the current macrotick value based on the current simulation time. */
  unsigned long Result = (unsigned long)((SimTime - gStartCycleTime[FR_VCTRL_IDX]) / gNanoSecondsPerMacrotick[FR_VCTRL_IDX]);

  return Result;
} /* Fr_VGetCurrentMacroTick() */

/***********************************************************************************************************************
 *  Fr_RegisterMessages
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
 */
FR_LOCAL FUNC(void, FR_CODE) Fr_RegisterMessages(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ctrlId;
  uint16 i;
  FR_LOCAL uint16 slotHandlerCallbackSlots[2];

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Initialize controller low-level registers according config structs. */
  Fr_InitMemory();

  /* #20 If platform is CANoeEmu:
   *          Register the handler and callback functions for FlexRay.
   *      Otherwise:
   *          Handler and callback registration is done by VTTCtrl. On VTT platform only the CANoe cluster name must be registered to VTTCtrl. */
#if (FR_VTT == STD_OFF) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
  /* Register callback functions for FlexRay */
  (void)CANoeAPI_SetFlexRayMessageHandler(Fr_RxHandler, Fr_TxHandler);
  (void)CANoeAPI_SetFlexRayCycleHandler(Fr_StartCycleHandler, Fr_NmVectorHandler);

  /* Register status handler, will be called in case of simulated and real bus */
  (void)CANoeAPI_SetFlexRayStatusHandler(Fr_StatusHandler);
  (void)CANoeAPI_SetFlexRayTimerHandler(Fr_TimerHandler); // gibt es diese API??

  (void)CANoeAPI_SetFlexRaySlotHandler(Fr_SlotHandler);
  
  /* Register POC state handler, will only be called in case of real bus */
  (void)CANoeAPI_SetFlexRayPOCStateHandler(Fr_POCStateHandler);
  
  for (ctrlId = 0; ctrlId < FR_NUM_CTRL_USED; ctrlId++)
  {
    slotHandlerCallbackSlots[0] = gNumberOfStaticSlots[ctrlId] / 2;
    slotHandlerCallbackSlots[1] = gNumberOfStaticSlots[ctrlId];

    (void)CANoeAPI_SetRequestedFlexRaySlotIDs(ctrlId,(uint32)(sizeof(slotHandlerCallbackSlots) / sizeof(slotHandlerCallbackSlots[0])),
      slotHandlerCallbackSlots) ;
  }
#endif /* #if (FR_VTT == STD_OFF) */

  for (ctrlId = 0; ctrlId < FR_NUM_CTRL_USED; ctrlId++)
  {
    /* #40 Register the FlexRay frames in CANoe according the read message buffer configuration. */
    for (i = 0; i < (Fr_VGetNumRxVirtBuf2PhysBufs(ctrlId) + Fr_VGetNumTxVirtBuf2PhysBufs(ctrlId)); i++)
    {
      uint16 ConfigIndex = Fr_VCcCfgR_GetPhysicalBufferNumber(ctrlId, i);

      uint32 baseCycle = (uint32)Fr_VCcCfgR_GetBaseCycle(ctrlId, ConfigIndex);
      uint32 cycleRep = Fr_VCcCfgR_GetCycleRepetition(ctrlId, ConfigIndex);
      uint16 slotId = Fr_VCcCfgR_GetSlotId(ctrlId, ConfigIndex);
      uint32 channel = (uint32)Fr_VCcCfgR_GetChannel(ctrlId, ConfigIndex);
      uint16 payloadLengthInBytes = Fr_VCcCfgR_GetPayloadLengthInBytes(ctrlId, ConfigIndex);

      /* 0x01 Sets the sync bit in the header */
      /* 0x02 Sets the startup bit in the header */
      /* 0x10 Sets the send mode to event-triggered */
      /* 0x20 Sets the payload preamble bit */
      /* 0x80 TX OFF only VN3300 , 3600 */
      /* 0x400 Sets the null frame indicator in the header only VN3300 , 3600 */
      uint32 frameStatus = (slotId > gNumberOfStaticSlots[ctrlId]) ? 0x10 : ((slotId == gSyncSlotId[ctrlId]) ? 0x13 : 0x10);
      if (Fr_VCcCfgR_HasPayloadPreambleBit(ctrlId, ConfigIndex) == TRUE)
      {
        /* flag is correctly given to CANoeApi / VTT but these settings are not considered by CANoe (PP-Flag won't be set for these frames) */
        frameStatus |= 0x20;
      }
      if (Fr_VCcCfgR_IsTxBuffer(ctrlId, ConfigIndex) == TRUE)
      {
        if (slotId <= gNumberOfStaticSlots[ctrlId])
        {
          /* use FR_LOCAL payload length for all FR_LOCAL frames for real bus (i.e. VN3600) */
          payloadLengthInBytes = gStaticPayloadLengthInBytes[ctrlId];
          frameStatus |= 0x80;      /* do not send msg after init but send msg after fr_startcommunication */
        }

        if (channel != 3)
        {
          VttFr_RegisterFlexRayMessage(ctrlId, channel, slotId, baseCycle, cycleRep, frameStatus, payloadLengthInBytes);
        }
        else
        {
          VttFr_RegisterFlexRayMessage(ctrlId, (uint32)1, slotId, baseCycle, cycleRep, frameStatus, payloadLengthInBytes);
          VttFr_RegisterFlexRayMessage(ctrlId, (uint32)2, slotId, baseCycle, cycleRep, frameStatus, payloadLengthInBytes);
        }
      }
    }
  }
  /* PRQA S 6050 1 */ /* MD_MSR_STCAL */
} /* Fr_RegisterMessages() */

/***********************************************************************************************************************
 *  Fr_VStartStopMessages
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
FR_LOCAL FUNC(Std_ReturnType, FR_CODE) Fr_VStartStopMessages(uint8 Fr_CtrlIdx, int value) /* 1=on 0=off */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 Fr_LPduIdx;
  uint16 Fr_LPduLength;
  uint8 Fr_LPduPtr[255];
  uint32 baseCycle;
  uint32 cycleRep;
  uint16 slotId;
  uint16 channel;
  uint16 frameStatus;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Loop over all configured L-PDUs: */
  for (Fr_LPduIdx = 0; Fr_LPduIdx < Fr_VGetNumTxVirtBuf2PhysBufs(FR_VCTRL_IDX); Fr_LPduIdx++)
  {
    /* #20 Get the base cycle, cycle repetition, slotId and frame status for each L-PDU. */
    const uint16 ConfigIndex = Fr_GetTxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx).mbIdx;
    baseCycle = Fr_VCcCfgR_GetBaseCycle(FR_VCTRL_IDX, ConfigIndex);
    cycleRep  = Fr_VCcCfgR_GetCycleRepetition(FR_VCTRL_IDX, ConfigIndex);
    slotId    = Fr_VCcCfgR_GetSlotId(FR_VCTRL_IDX, ConfigIndex);
    channel   = Fr_VCcCfgR_GetChannel(FR_VCTRL_IDX, ConfigIndex);
    frameStatus = (slotId > gNumberOfStaticSlots[FR_VCTRL_IDX]) ? 0x10 : ((slotId == gSyncSlotId[FR_VCTRL_IDX]) ? 0x13 : 0x10);

    /* #30 If the slotId of the current L-PDU lies in the static segment: */
    if (slotId <= gNumberOfStaticSlots[FR_VCTRL_IDX])
    {
      Fr_LPduLength = (uint8)gStaticPayloadLengthInBytes[FR_VCTRL_IDX];

      /* #40 Set the requested FlexRay message status of the VN-interface for the current L-PDU to either ON or OFF. */
      if((int)FR_STARTSTOP_MESSAGES_OFF == value)  /* 0=off */
      {
        frameStatus |= 0x80;
      }
      if(Fr_LPduPtr == 0)
      {
        retVal = E_NOT_OK;
        break;
      }
      if ((Fr_LPduLength & 0x01u) != 0)
      {
        Fr_LPduLength++;  /* round up to even length */
      }

      if(channel != 3)
      {
        VttFr_SendFlexRayMessage(
          FR_VCTRL_IDX,
          channel,
          slotId,
          baseCycle,
          cycleRep,
          frameStatus,
          Fr_LPduLength,
          Fr_LPduPtr);
      }
      else
      {
        VttFr_SendFlexRayMessage(
          FR_VCTRL_IDX,
          1,
          slotId,
          baseCycle,
          cycleRep,
          frameStatus,
          Fr_LPduLength,
          Fr_LPduPtr);
        VttFr_SendFlexRayMessage(
          FR_VCTRL_IDX,
          2,
          slotId,
          baseCycle,
          cycleRep,
          frameStatus,
          Fr_LPduLength,
          Fr_LPduPtr);
      }
    }
    /* #50 Otherwise do nothing and leave the frame status of the VN-interface untouched. */

    retVal = E_OK;
  }

  return retVal;
} /* Fr_VStartStopMessages() */

/***********************************************************************************************************************
 *  Fr_InitFrCcRegVals
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(void, FR_CODE) Fr_InitFrCcRegVals(uint8 Fr_CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 i;

  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  
  /* #10 Initialize controller registers according config structs. */
  for (i = 0; i < (uint8)Fr_VGetNumCcRegs(FR_VCTRL_IDX); i++)
  {
    switch (CCADDRS[i])
    {
      case FR_SUCC1:
        vFrCcRegVals[FR_VCTRL_IDX].SUCC1 = Fr_VGetCcCfgValAt(FR_VCTRL_IDX, i);
        break;
      case FR_SUCC2:
        vFrCcRegVals[FR_VCTRL_IDX].SUCC2 = Fr_VGetCcCfgValAt(FR_VCTRL_IDX, i);
        break;
      case FR_PRTC1:
        vFrCcRegVals[FR_VCTRL_IDX].PRTC1 = Fr_VGetCcCfgValAt(FR_VCTRL_IDX, i);
        break;
      case FR_PRTC2:
        vFrCcRegVals[FR_VCTRL_IDX].PRTC2 = Fr_VGetCcCfgValAt(FR_VCTRL_IDX, i);
        break;
      case FR_MHDC:
        vFrCcRegVals[FR_VCTRL_IDX].MHDC = Fr_VGetCcCfgValAt(FR_VCTRL_IDX, i);
        break;
      case FR_GTUC1:
        vFrCcRegVals[FR_VCTRL_IDX].GTUC1 = Fr_VGetCcCfgValAt(FR_VCTRL_IDX, i);
        break;
      case FR_GTUC2:
        vFrCcRegVals[FR_VCTRL_IDX].GTUC2 = Fr_VGetCcCfgValAt(FR_VCTRL_IDX, i);
        break;
      case FR_GTUC7:
        vFrCcRegVals[FR_VCTRL_IDX].GTUC7 = Fr_VGetCcCfgValAt(FR_VCTRL_IDX, i);
        break;
      case FR_GTUC11:
        vFrCcRegVals[FR_VCTRL_IDX].GTUC11 = Fr_VGetCcCfgValAt(FR_VCTRL_IDX, i);
        break;
      default:
        /* invalid CCADDRS[i] value */
        break;
    }
  }
  /* PRQA S 6030 1 */ /* MD_MSR_STCYC */ 
} /* Fr_InitFrCcRegVals() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetNumberOfStaticSlots
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(uint16, FR_CODE) Fr_VCcCfgR_GetNumberOfStaticSlots(uint8 Fr_CtrlIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Returns the Number of Static Slots (NSS) of the GTU Configuration Register 7 (GTUC7). */
  return ((uint16)((vFrCcRegVals[FR_VCTRL_IDX].GTUC7 & 0x3FF0000) >> 16));
} /* Fr_VCcCfgR_GetNumberOfStaticSlots() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetSyncSlotId
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FR_LOCAL FUNC(uint16, FR_CODE) Fr_VCcCfgR_GetSyncSlotId(uint8 Fr_CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 retVal = 0; /* no Sync Frame */

  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 If the Transmit Startup Frame in Key Slot (TXSY) of the SUC Configuration Register 1 (SUCC1) is set: */
  if ((vFrCcRegVals[FR_VCTRL_IDX].SUCC1 & FR_SUCC1_TXSY) == FR_SUCC1_TXSY)
  {
    /* #20 Returns the Frame ID (FID) of the Write Header Section register 1 (WRHS1). */
    retVal = ((uint16)((CCBUFS_CFG(FR_VCTRL_IDX)[0].WRHS1) & 0x7FF));
  }
  /* #30 Otherwise: return 0 */

  return retVal;
} /* Fr_VCcCfgR_GetSyncSlotId() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetStaticPayloadLengthInBytes
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(uint16, FR_CODE) Fr_VCcCfgR_GetStaticPayloadLengthInBytes(uint8 Fr_CtrlIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
  
  /* #10 Returns the Static Frame Data Length (SFDL) of the  MHD Configuration Register (MHDC). */
  return (uint16)(((uint32)((vFrCcRegVals[FR_VCTRL_IDX].MHDC & 0x007F))) << 1);
} /* Fr_VCcCfgR_GetStaticPayloadLengthInBytes() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetWUPTxIdle
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(uint16, FR_CODE) Fr_VCcCfgR_GetWUPTxIdle(uint8 Fr_CtrlIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Returns the Wakeup Symbol Transmit Idle (TXI) of the PRT Configuration Register 2 (PRTC2). */
  return (uint16)((((uint32)(vFrCcRegVals[FR_VCTRL_IDX].PRTC2)) >> 16) & 0x000000FF);
} /* Fr_VCcCfgR_GetWUPTxIdle() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetWUPTxLow
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(uint16, FR_CODE) Fr_VCcCfgR_GetWUPTxLow(uint8 Fr_CtrlIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Returns the Wakeup Symbol Transmit Low (TXL) of the PRT Configuration Register 2 (PRTC2). */
  return (uint16)((((uint32)(vFrCcRegVals[FR_VCTRL_IDX].PRTC2)) >> 24) & 0x0000003F);
} /* Fr_VCcCfgR_GetWUPTxLow() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetWUPRepetitions
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(uint16, FR_CODE) Fr_VCcCfgR_GetWUPRepetitions(uint8 Fr_CtrlIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Returns the Repetitions of Tx Wakeup Pattern (RWP) of the PRT Configuration Register 1 (PRTC1). */
  return (uint16)((((uint32)(vFrCcRegVals[FR_VCTRL_IDX].PRTC1)) >> 26) & 0x0000003F);
} /* Fr_VCcCfgR_GetWUPRepetitions() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetMacroTicksPerCycle
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(uint16, FR_CODE) Fr_VCcCfgR_GetMacroTicksPerCycle(uint8 Fr_CtrlIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Returns the Macrotick Per Cycle (MPC) of the Confiuration Register 2 (GTUC2). */
  return ((uint16)((vFrCcRegVals[FR_VCTRL_IDX].GTUC2 & 0x00003FFF)));
} /* Fr_VCcCfgR_GetMacroTicksPerCycle() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetNanosecondsPerMacroTick
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(uint32, FR_CODE) Fr_VCcCfgR_GetNanosecondsPerMacroTick(uint8 Fr_CtrlIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Calculate the nanoseconds per macrotick based on the Microtick per Cycle (UT) of the Confiuration Register 1 (GTUC1). */
  return (uint32)(((vFrCcRegVals[FR_VCTRL_IDX].GTUC1 * \
                 (((vFrCcRegVals[FR_VCTRL_IDX].PRTC1 & FR_PRTC1_BRP_MASK) >> FR_PRTC1_BRP_SHIFT) + 1) * 25)) \
                 / (vFrCcRegVals[FR_VCTRL_IDX].GTUC2 & FR_GTUC2_MPC_MASK));
} /* Fr_VCcCfgR_GetNanosecondsPerMacroTick() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetSlotId
 **********************************************************************************************************************/
/*! \brief
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(uint16, FR_CODE) Fr_VCcCfgR_GetSlotId(uint8 Fr_CtrlIdx, uint16 MessageBufferIndex)
{
  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Returns the Frame ID (FID) of the Write Header Section register 1 (WRHS1). */
  return ((uint16)((CCBUFS_CFG(FR_VCTRL_IDX)[MessageBufferIndex].WRHS1) & 0x7FF));
} /* Fr_VCcCfgR_GetSlotId() */

/***********************************************************************************************************************
 *  Fr_GetRepetition
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(uint8, FR_CODE) Fr_GetRepetition(uint8 cycleCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 i;
  uint8 cycleRep = 1;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Extracts the cycle repetition from the given Cycle Code (CYC) of the Write Header Section register 1 (WRHS1). */
  for(i = 0; i < 7; i++)
  {
    if(((cycleCode >> i) & 0x1) == 0x01)
    {
      cycleRep = (uint8)(0x01 << i);
    }
  }
  return cycleRep;
} /* Fr_GetRepetition() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetCycleRepetition
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FR_LOCAL FUNC(uint8, FR_CODE) Fr_VCcCfgR_GetCycleRepetition(uint8 Fr_CtrlIdx, uint16 MessageBufferIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cycleCode;

  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Get the Cycle Code (CYC) of the Write Header Section register 1 (WRHS1). */
  cycleCode = Fr_VCcCfgR_GetCycleCode(FR_VCTRL_IDX, MessageBufferIndex);

  /* #20 Return the cycle repetition of the read cycle code. */
  return Fr_GetRepetition(cycleCode);
} /* Fr_VCcCfgR_GetCycleRepetition() */

/***********************************************************************************************************************
 *  Fr_GetCycleMask
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(uint8, FR_CODE) Fr_GetCycleMask(uint8 cycleCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cycMask;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Extracts the cycle mask from the given Cycle Code (CYC) of the Write Header Section register 1 (WRHS1). */
  switch (cycleCode)
  {
    case 64:
      cycMask = 0x3F;
      break;
    case 32:
      cycMask = 0x1F;
      break;
    case 16:
      cycMask = 0x0F;
      break;
    case 8:
      cycMask = 0x07;
      break;
    case 4:
      cycMask = 0x03;
      break;
    case 2:
      cycMask = 0x01;
      break;
    case 1:
      cycMask = 0x00;
      break;
    default:
      cycMask = 0xFF;    /* error */
      break;
  }
  return cycMask;
} /* Fr_GetCycleMask() */

/***********************************************************************************************************************
 *  Fr_GetBaseCycle
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(uint8, FR_CODE) Fr_GetBaseCycle(uint8 cycleCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 i;
  uint8 posCycleRep = 0;
  uint8 baseCycle = 0;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Extracts the base cycle from the given Cycle Code (CYC) of the Write Header Section register 1 (WRHS1). */
  for(i = 0; i < 7; i++)
  {
    if(((cycleCode >> i) & 0x1) == 0x01)
    {
      posCycleRep = i;
    }
  }
  baseCycle = (uint8)(cycleCode & (~(0x01 << posCycleRep)));
  return baseCycle;
} /* Fr_GetBaseCycle() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetBaseCycle
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FR_LOCAL FUNC(uint8, FR_CODE) Fr_VCcCfgR_GetBaseCycle(uint8 Fr_CtrlIdx, uint16 MessageBufferIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cycleCode;

  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Get the Cycle Code (CYC) of the Write Header Section register 1 (WRHS1). */
  cycleCode = Fr_VCcCfgR_GetCycleCode(FR_VCTRL_IDX, MessageBufferIndex);

  /* #20 Return the base cycle of the read cycle code. */
  return Fr_GetBaseCycle(cycleCode);
} /* Fr_VCcCfgR_GetBaseCycle() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetCycleCode
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(uint8, FR_CODE) Fr_VCcCfgR_GetCycleCode(uint8 Fr_CtrlIdx, uint16 MessageBufferIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cycleCode;
  
  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Returns the Cycle Code (CYC) of the Write Header Section register 1 (WRHS1). */
  cycleCode = (uint8)(((CCBUFS_CFG(FR_VCTRL_IDX)[MessageBufferIndex].WRHS1) & 0x007F0000) >> 16);
  return cycleCode;
} /* Fr_VCcCfgR_GetCycleCode() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetChannel
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(uint8, FR_CODE) Fr_VCcCfgR_GetChannel(uint8 Fr_CtrlIdx, uint16 MessageBufferIndex)
{
  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Returns the Channel Filter Control flags (CHA, CHB) of the Write Header Section register 1 (WRHS1). */
  return (uint8)(((CCBUFS_CFG(FR_VCTRL_IDX)[MessageBufferIndex].WRHS1) & 0x03000000) >> 24);
} /* Fr_VCcCfgR_GetChannel() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetPayloadLengthInBytes
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(uint8, FR_CODE) Fr_VCcCfgR_GetPayloadLengthInBytes(uint8 Fr_CtrlIdx, uint16 MessageBufferIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 retval;

  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Returns the Payload Length Configured flags (PLC) of the Write Header Section register 2 (WRHS2). */
  retval = (uint8)(((CCBUFS_CFG(FR_VCTRL_IDX)[MessageBufferIndex].WRHS2) & 0x007F0000) >> 16);
  return (2 * retval);
} /* Fr_VCcCfgR_GetPayloadLengthInBytes() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_IsTxBuffer
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(boolean, FR_CODE) Fr_VCcCfgR_IsTxBuffer(uint8 Fr_CtrlIdx, uint16 MessageBufferIndex)
{
  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Returns the Message Buffer Direction Configuration Bit (CFG) of the Write Header Section register 1 (WRHS1). */
  return (uint8)(((CCBUFS_CFG(FR_VCTRL_IDX)[MessageBufferIndex].WRHS1) & 0x04000000) >> 26);
} /* Fr_VCcCfgR_IsTxBuffer() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_HasPayloadPreambleBit
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(boolean, FR_CODE) Fr_VCcCfgR_HasPayloadPreambleBit(uint8 Fr_CtrlIdx, uint16 MessageBufferIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 retval;
  uint32 tmp;
  uint32 tmp1;

  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Returns the Payload Preamble Indicator Transmit flag (PPIT) of the Write Header Section register 1 (WRHS1). */
  tmp1 = (CCBUFS(FR_VCTRL_IDX)[MessageBufferIndex].WRHS1);
  tmp = (tmp1 & FR_WRHS1_PPIT);
  retval = (uint8) (tmp >> 27);

  return retval;
} /* Fr_VCcCfgR_HasPayloadPreambleBit() */

/***********************************************************************************************************************
 *  Fr_VCcCfgR_GetPhysicalBufferNumber
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(uint16, FR_CODE) Fr_VCcCfgR_GetPhysicalBufferNumber(uint8 Fr_CtrlIdx, uint16 Fr_LPduIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 retVal;

  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Return the physical message buffer index depending whether on the direction of the given Fr_LPduIdx. */
  if (Fr_LPduIdx < Fr_VGetNumTxVirtBuf2PhysBufs(FR_VCTRL_IDX))
  {   /* TX */
    retVal = (Fr_GetTxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx)).mbIdx;
  }
  else
  { /* RX */
    Fr_LPduIdx = (uint16)(Fr_LPduIdx - Fr_VGetNumTxVirtBuf2PhysBufs(FR_VCTRL_IDX));
    retVal = (Fr_GetRxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx)).mbIdx;
  }

  return retVal;
} /* Fr_VCcCfgR_GetPhysicalBufferNumber() */

/***********************************************************************************************************************
 *  Fr_ExecutePOCCommand
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FR_LOCAL FUNC(Std_ReturnType, FR_CODE) Fr_ExecutePOCCommand(uint8 Fr_CtrlIdx, uint8 pocCommand)
{
  /* ----- Implementation ----------------------------------------------- */
  FR_DUMMY_STATEMENT(Fr_CtrlIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */

  /* #10 Set the simulated POCstate according to given POC command. */
  if (Fr_UseSimulatedPocState(FR_VCTRL_IDX) == FALSE)
  {
    gSimulatedPOCState[FR_VCTRL_IDX] = pocCommand;
  }
  else
  {
    gSimulatedPOCState[FR_VCTRL_IDX] = pocCommand;
  }

  return ((Std_ReturnType) E_OK);
} /* Fr_ExecutePOCCommand() */

#if (Fr_IsPostbuild())
/***********************************************************************************************************************
 *  Fr_VSetGlobalFrConfigPtr
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FR_LOCAL FUNC(void, FR_CODE) Fr_VSetGlobalFrConfigPtr(P2CONST(Fr_VConfigType, AUTOMATIC, FR_PBCFG) Fr_ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ctrlId;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 If given Fr_ConfigPtr is valid: */
  if (Fr_ConfigPtr != NULL_PTR)
  {
    /* #20 Store Fr_ConfigPtr to be used by Fr_Init() further on. */
    Fr_VPostBuildConfigPtr      = (P2CONST(Fr_VConfigType, AUTOMATIC, FR_PBCFG)) Fr_ConfigPtr;
    for (ctrlId = 0; ctrlId < FR_NUM_CTRL_USED; ctrlId++)
    {
#  if (FR_NUM_CTRL_USED > 1) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */      
      Fr_VRxVirtBuf2PhysBufMapPtr[ctrlId] = Fr_VPostBuildConfigPtr->RxVirtBuf2PhysBufMap[ctrlId];
      Fr_VTxVirtBuf2PhysBufMapPtr[ctrlId] = Fr_VPostBuildConfigPtr->TxVirtBuf2PhysBufMap[ctrlId];
#  else
      Fr_VRxVirtBuf2PhysBufMapPtr[ctrlId] = Fr_VPostBuildConfigPtr->RxVirtBuf2PhysBufMap;
      Fr_VTxVirtBuf2PhysBufMapPtr[ctrlId] = Fr_VPostBuildConfigPtr->TxVirtBuf2PhysBufMap;
#  endif
    }
  }
} /* Fr_VSetGlobalFrConfigPtr() */
#endif /* (FR_POSTBUILD_VARIANT_SUPPORT == STD_ON) || (FR_CONFIGURATION_VARIANT == FR_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  Fr_Init
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
FUNC(void, FR_CODE) Fr_Init(P2CONST(Fr_ConfigType, AUTOMATIC, FR_PBCFG) Fr_ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ctrlId;
  uint8 errorId = FR_E_NO_ERROR;
  uint16 i;
  uint16 j;

#if (FR_DEV_ERROR_DETECT == STD_ON)
  Fr_VFrDrvState = FR_VUNINITPTR;
#endif

  /* ----- Development Error Checks ------------------------------------- */

#if (Fr_IsPostbuild())
  Fr_VPostBuildConfigPtr      = (P2CONST(Fr_VConfigType, AUTOMATIC, FR_PBCFG)) Fr_ConfigPtr;
#endif

  /* #20 Check the validity of the configuration pointer. */
  if (Fr_ConfigPtr == NULL_PTR)
  {
#if (FR_DEV_ERROR_DETECT == STD_ON)
    errorId = FR_E_INV_POINTER;
#endif
/* #30 If the configuration variant is post-build loadable: */
#if (FR_CONFIGURATION_VARIANT == FR_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
    EcuM_BswErrorHook((uint16) FR_MODULE_ID, ECUM_BSWERROR_NULLPTR);
  }
  /* #40 Check compatibility with the FR generator */
  else if (Fr_VPostBuildConfigPtr->GeneratorCompatibilityVersion != (uint16)FR_GENERATOR_COMPATIBILITY_VERSION)
  {
    EcuM_BswErrorHook((uint16) FR_MODULE_ID, ECUM_BSWERROR_COMPATIBILITYVERSION);
  }
  /* #50 Use the magic number to proof the validity of the configuration given as input. */
  else if (Fr_VPostBuildConfigPtr->FinalMagicNumber != FR_FINAL_MAGICNUMBER)
  {
    EcuM_BswErrorHook((uint16) FR_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER); 
#endif
  }
  /* #60 If the initial checks are passed or not required: */
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    Fr_Rtm_Start(Fr_Init);

# if (Fr_IsPostbuild())
    Fr_VSetGlobalFrConfigPtr(Fr_ConfigPtr);
# else
    FR_DUMMY_STATEMENT(Fr_ConfigPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif /* (FR_POSTBUILD_VARIANT_SUPPORT == STD_ON) || (FR_CONFIGURATION_VARIANT == FR_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE) */

    for (ctrlId = 0; ctrlId < FR_NUM_CTRL_USED; ctrlId++)
    {
      (void)Fr_VStartStopMessages(ctrlId, FR_STARTSTOP_MESSAGES_OFF);

      /* #65 Reset the CANoeEmu state-machine variables. */
      gCurrentCycle[ctrlId]            = 0;
      gStartCycleTime[ctrlId]          = 0;
      gPreventSync[ctrlId]             = FALSE;
      gNormalPassive[ctrlId]           = FALSE;
      gTimeoutTimes[ctrlId][0]         = 0;
      gTimeoutTimes[ctrlId][1]         = 0;
      gTimerEnabled[ctrlId][0]         = FALSE;
      gTimerEnabled[ctrlId][1]         = FALSE;
      gIsSynchronized[ctrlId]          = FALSE;
      gHaltRequest[ctrlId]             = FALSE;
      gColdstartInhibit[ctrlId]        = TRUE;
      gUseSimulatedPocState[ctrlId]    = TRUE;
      gNanoSecondsPerMacrotick[ctrlId] = Fr_VCcCfgR_GetNanosecondsPerMacroTick(ctrlId);
      gMacroticksPerCycle[ctrlId]      = Fr_VCcCfgR_GetMacroTicksPerCycle(ctrlId);
      gSyncSlotId[ctrlId]              = Fr_VCcCfgR_GetSyncSlotId(ctrlId);
      gStaticWUPTxIdle[ctrlId]         = Fr_VCcCfgR_GetWUPTxIdle(ctrlId);
      gStaticWUPTxLow[ctrlId]          = Fr_VCcCfgR_GetWUPTxLow(ctrlId);
      gStaticWUPRepetitions[ctrlId]    = Fr_VCcCfgR_GetWUPRepetitions(ctrlId);
      gStartupFlag[ctrlId]             = FALSE;
      gDefaultWakeupChannel[ctrlId]    = DEFAULT_WAKEUP_CHANNEL;

      /* #70 Transition to POCstate DEFAULT_CONFIG. */
      (void)Fr_ExecutePOCCommand(ctrlId, FR_POCSTATE_DEFAULT_CONFIG);

#if ( FR_DIRECT_BUFFER_ACCESS_ENABLE == STD_ON )   
      /* #80 If FR_DIRECT_BUFFER_ACCESS_ENABLE is enabled initialize the frame buffers. */
      for (i = 0; i < (Fr_VGetNumTxVirtBuf2PhysBufs(ctrlId) + Fr_VGetNumRxVirtBuf2PhysBufs(ctrlId)); i++) 
      {
        for(j = 0; j < 256; j++)
        {
          Fr_FrameBuf[ctrlId][i][j] = 0;
        }
      }
# endif

      /* #90 Clear TX pending flags and RX read flags of all configured message buffers. */
      for (i = 1; i <= MAX_SLOT_NUMBER; i++)
      {
        Fr_SetRxMsgBufRead(ctrlId, 0, i);
        Fr_SetRxMsgBufRead(ctrlId, 1, i);
        Fr_SetRxMsgBufRead(ctrlId, 2, i);

        for (j = 0; j < 128; j++)
        {
          Fr_SetTxMsgBufPending(ctrlId, 1, i, j, FALSE);
          Fr_SetTxMsgBufPending(ctrlId, 2, i, j, FALSE);
        }
      }
    }

  #if ( FR_DEV_ERROR_DETECT == STD_ON )
      /* #100 Initialize Fr_VFrDrvState with FR_VUNINITCFG (if FR_DEV_ERROR_DETECT is enabled). */
      Fr_VFrDrvState = FR_VUNINITCFG;
  #endif

      Fr_Rtm_Stop(Fr_Init);
    }

    /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_INIT, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif
  /* PRQA S 6080 2 */ /* MD_MSR_STMIF */
  /* PRQA S 6050 1 */ /* MD_MSR_STCAL */
} /* Fr_Init() */ 

/***********************************************************************************************************************
 *  Fr_ControllerInit
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
FUNC(Std_ReturnType, FR_CODE) Fr_ControllerInit
(
  FR_VCTRL_SYSTEMTYPE_ONLY
)
{ /* PRQA S 3007 */ /* MD_Fr_3007 */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState == FR_VUNINITPTR)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    Fr_Rtm_Start(Fr_ControllerInit);

    /* #40 Go through POC-states DEFAULT_CONFIG, CONFIG, READY to visualize the controller initialization in the CANoe trace window. */
    gColdstartInhibit[FR_VCTRL_IDX] = TRUE;
    gStartupFlag[FR_VCTRL_IDX]      = FALSE;
    retVal  = Fr_ExecutePOCCommand(FR_VCTRL_IDX, FR_POCSTATE_DEFAULT_CONFIG);
    retVal |= Fr_ExecutePOCCommand(FR_VCTRL_IDX, FR_POCSTATE_CONFIG);
    retVal |= Fr_ExecutePOCCommand(FR_VCTRL_IDX, FR_POCSTATE_READY);
    VttFr_ResetFlexRayCC(FR_VCTRL_IDX);

    wakeupChannelValid[FR_VCTRL_IDX]= FALSE;

#if ( FR_DEV_ERROR_DETECT == STD_ON )
    Fr_VFrDrvState = FR_VINIT;
#endif

    Fr_Rtm_Stop(Fr_ControllerInit);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_CONTROLLER_INIT, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* Fr_ControllerInit() */

/***********************************************************************************************************************
 *  Fr_StartCommunication
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
FUNC(Std_ReturnType, FR_CODE) Fr_StartCommunication
(
  FR_VCTRL_SYSTEMTYPE_ONLY
)
{ /* PRQA S 3007 */ /* MD_Fr_3007 */
  /* ----- Local Variables ---------------------------------------------- */
  Fr_POCStateType  POCState;
  Fr_POCStatusType PocStatus;
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
  /* #20 If required, check number of controllers. */
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check that POCstate is READY. */
  else if (Fr_VCCGetPocState(FR_VCTRL_IDX) != FR_POCSTATE_READY)
  {
    errorId = FR_E_INV_POCSTATE;
  }
  /* #40 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    gStartupFlag[FR_VCTRL_IDX] = TRUE;
    /* #50 Transition from POCstate READY to STARTUP. */
    if (Fr_UseSimulatedPocState(FR_VCTRL_IDX) == FALSE)
    {
      retVal = Fr_GetPOCStatus(FR_VCTRL_IDX, &PocStatus);
      POCState = PocStatus.State;
    }
    else
    {
      POCState = gSimulatedPOCState[FR_VCTRL_IDX];
    }

    retVal = Fr_ExecutePOCCommand(FR_VCTRL_IDX, FR_POCSTATE_STARTUP);
    retVal |= Fr_VStartStopMessages(FR_VCTRL_IDX, FR_STARTSTOP_MESSAGES_ON);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_START_COMMUNICATION, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* Fr_StartCommunication() */

/***********************************************************************************************************************
 *  Fr_AllowColdstart
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
FUNC(Std_ReturnType, FR_CODE) Fr_AllowColdstart
(
  FR_VCTRL_SYSTEMTYPE_ONLY
)
{ /* PRQA S 3007 */ /* MD_Fr_3007 */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check plausibility of input parameter 'Fr_CtrlIdx'. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check that communication controller is not in POCstate DEFAULT_CONFIG or CONFIG or HALT. */
  else if ((Fr_VCCGetPocState(FR_VCTRL_IDX) == FR_POCSTATE_DEFAULT_CONFIG) | (Fr_VCCGetPocState(FR_VCTRL_IDX) == FR_POCSTATE_CONFIG) | (Fr_VCCGetPocState(FR_VCTRL_IDX) == FR_POCSTATE_HALT))
  {
    errorId = FR_E_INV_POCSTATE;
  }
  /* #40 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Transition from POCstate READY to STARTUP using coldstart node path by clearing gColdstartInhibit flag. */
    gColdstartInhibit[FR_VCTRL_IDX] = FALSE;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_ALLOW_COLDSTART, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* Fr_AllowColdstart() */

/***********************************************************************************************************************
 *  Fr_HaltCommunication
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
FUNC(Std_ReturnType, FR_CODE) Fr_HaltCommunication
(
  FR_VCTRL_SYSTEMTYPE_ONLY
)
{ /* PRQA S 3007 */ /* MD_Fr_3007 */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  uint32 pocState = (Fr_VCCGetPocState(FR_VCTRL_IDX) == FR_POCSTATE_NORMAL_ACTIVE) | (Fr_VCCGetPocState(FR_VCTRL_IDX) == FR_POCSTATE_NORMAL_PASSIVE);

  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check that communication controller is either in POCstate NORMAL_ACTIVE or in NORMAL_PASSIVE. */
  else if (!pocState)
  {
    errorId = FR_E_INV_POCSTATE;
  }
  /* #40 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Transition from either from POCstate NORMAL_ACTIVE or NORMAL_PASSIVE to HALT by setting the gHaltRequest flag. */
    gHaltRequest[FR_VCTRL_IDX] = TRUE;
    retVal = Fr_VStartStopMessages(FR_VCTRL_IDX, FR_STARTSTOP_MESSAGES_OFF);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_HALT_COMMUNICATION, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* Fr_HaltCommunication() */

/***********************************************************************************************************************
 *  Fr_AbortCommunication
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
FUNC(Std_ReturnType, FR_CODE) Fr_AbortCommunication
(
  FR_VCTRL_SYSTEMTYPE_ONLY
)
{ /* PRQA S 3007 */ /* MD_Fr_3007 */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
#  if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
#  endif
  /* #30 If the initial checks are passed or not required: */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Transition from ANY POCstate to HALT. */
    retVal = Fr_ExecutePOCCommand(FR_VCTRL_IDX, FR_POCSTATE_HALT);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_ABORT_COMMUNICATION, errorId);
  }
# else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* Fr_AbortCommunication() */

/***********************************************************************************************************************
 *  Fr_SendWUP
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
FUNC(Std_ReturnType, FR_CODE) Fr_SendWUP
(
  FR_VCTRL_SYSTEMTYPE_ONLY
)
{ /* PRQA S 3007 */ /* MD_Fr_3007 */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check that POCstate is READY. */
  else if (Fr_VCCGetPocState(FR_VCTRL_IDX) != FR_POCSTATE_READY)
  {
    errorId = FR_E_INV_POCSTATE;
  }
  /* #40 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Transition from POCstate READY to WAKEUP to trigger the wakeup pattern transmission. */
    VttFr_ResetFlexRayCCAndSendWUP(FR_VCTRL_IDX, gDefaultWakeupChannel[FR_VCTRL_IDX],
         gStaticWUPRepetitions[FR_VCTRL_IDX], gStaticWUPTxIdle[FR_VCTRL_IDX], gStaticWUPTxLow[FR_VCTRL_IDX], NULL_PTR);
    gSimulatedPOCState[FR_VCTRL_IDX] = FR_POCSTATE_WAKEUP;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_SEND_WUP, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* Fr_SendWUP() */

/***********************************************************************************************************************
 *  Fr_SetWakeupChannel
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
FUNC(Std_ReturnType, FR_CODE) Fr_SetWakeupChannel
(
  FR_VCTRL_SYSTEMTYPE_ONLY, 
  Fr_ChannelType Fr_ChnlIdx
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
#  if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
#  endif
  /* #30 If required, check that parameter Fr_ChnlIdx is either FR_CHANNEL_A or FR_CHANNEL_B. */
  else if ((Fr_ChnlIdx != FR_CHANNEL_A) & (Fr_ChnlIdx != FR_CHANNEL_B))
  {
    errorId = FR_E_INV_CHNL_IDX;
  }
  /* #40 If required, check that POCstate is READY. */
  else if (Fr_VCCGetPocState(FR_VCTRL_IDX) != FR_POCSTATE_READY)
  {
    errorId = FR_E_INV_POCSTATE;
  }
  /* #50 If the initial checks are passed or not required: */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #60 Do nothing because API service is currently not supported by CANoeEmu driver */
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_SET_WAKEUP_CHANNEL, errorId);
  }
# else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /*Fr_SetWakeupChannel() */

/***********************************************************************************************************************
 *  Fr_GetPOCStatus
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
 */
FUNC(Std_ReturnType, FR_CODE) Fr_GetPOCStatus
(
  FR_VCTRL_SYSTEMTYPE_ONLY,
  P2VAR(Fr_POCStatusType, AUTOMATIC, FR_APPL_DATA) Fr_POCStatusPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check that Fr_POCStatusPtr points to a valid address. */
  else if (Fr_POCStatusPtr == NULL_PTR)
  {
    errorId = FR_E_INV_POINTER;
  }
  /* #40 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    Fr_POCStatusPtr->StartupState = FR_STARTUP_UNDEFINED;
    Fr_POCStatusPtr->ColdstartNoise = FALSE;
    Fr_POCStatusPtr->CHIHaltRequest = FALSE;
    Fr_POCStatusPtr->ErrorMode = FR_ERRORMODE_ACTIVE;
    Fr_POCStatusPtr->SlotMode = FR_SLOTMODE_ALL;
    Fr_POCStatusPtr->WakeupStatus = FR_WAKEUP_UNDEFINED;
    retVal = E_OK;

    /* #50 Determine either the simulated POC-State or the POC-State of the VN-Interface depending on the simulation mode of CANoe. */
    if (FALSE == Fr_UseSimulatedPocState(FR_VCTRL_IDX))
    {
      /* the environment variable can be accessed and has a value that differs from 0 */
      switch (Fr_GetPOCStateRealBus(FR_VCTRL_IDX))
      {
        case 0:
          Fr_POCStatusPtr->State = FR_POCSTATE_CONFIG;  /*DEFAULT CONFIG */
          break;
        case 1:
          Fr_POCStatusPtr->State = FR_POCSTATE_READY;
          break;
        case 2:
          Fr_POCStatusPtr->State = FR_POCSTATE_NORMAL_ACTIVE;
          break;
        case 3:
          Fr_POCStatusPtr->State = FR_POCSTATE_NORMAL_PASSIVE;
          break;
        case 4:
          Fr_POCStatusPtr->State = FR_POCSTATE_HALT;
          break;
        case 5:                  /* Monitor Mode */
          break;
        case 15:
          Fr_POCStatusPtr->State = FR_POCSTATE_READY; /* FR_POCSTATE_CONFIG; */
          break;
        case 16:
          Fr_POCStatusPtr->State = FR_POCSTATE_WAKEUP;
          Fr_POCStatusPtr->WakeupStatus = FR_WAKEUP_UNDEFINED;  /* WAKEUP STANDBY */
          break;
        case 17:
          Fr_POCStatusPtr->State = FR_POCSTATE_WAKEUP;
          Fr_POCStatusPtr->WakeupStatus = FR_WAKEUP_UNDEFINED;  /* WAKEUP LISTEN */
          break;
        case 18:
          Fr_POCStatusPtr->State = FR_POCSTATE_WAKEUP;
          Fr_POCStatusPtr->WakeupStatus = FR_WAKEUP_UNDEFINED;  /* WAKEUP SEND */
          break;
        case 19:
          Fr_POCStatusPtr->State = FR_POCSTATE_WAKEUP;
          Fr_POCStatusPtr->WakeupStatus = FR_WAKEUP_UNDEFINED;  /* WAKEUP DETECT */
          break;
        case 32:
          Fr_POCStatusPtr->State = FR_POCSTATE_STARTUP;
          Fr_POCStatusPtr->StartupState = FR_STARTUP_UNDEFINED; /* STARTUP PREPARE */
          break;
        case 33:
          Fr_POCStatusPtr->State = FR_POCSTATE_STARTUP; /* Coldstart Listen */
          Fr_POCStatusPtr->StartupState = FR_STARTUP_COLDSTART_LISTEN;
          break;
        case 34:
          Fr_POCStatusPtr->State = FR_POCSTATE_STARTUP; /*Coldstart Coll. Resolution. */
          Fr_POCStatusPtr->StartupState = FR_STARTUP_COLDSTART_COLLISION_RESOLUTION;
          break;
        case 35:
          Fr_POCStatusPtr->State = FR_POCSTATE_STARTUP; /*Coldstart Cons. Chk. */
          Fr_POCStatusPtr->StartupState = FR_STARTUP_COLDSTART_CONSISTENCY_CHECK;
          break;
        case 36:
          Fr_POCStatusPtr->State = FR_POCSTATE_STARTUP; /* Coldstart Gap */
          Fr_POCStatusPtr->StartupState = FR_STARTUP_COLDSTART_GAP;
          break;
        case 37:
          Fr_POCStatusPtr->State = FR_POCSTATE_STARTUP; /* Coldstart Join */
          Fr_POCStatusPtr->StartupState = FR_STARTUP_COLDSTART_JOIN;
          break;
        case 38:
          Fr_POCStatusPtr->State = FR_POCSTATE_STARTUP; /* int. Coldstart Chk. */
          Fr_POCStatusPtr->StartupState = FR_STARTUP_INTEGRATION_COLDSTART_CHECK;
          break;
        case 39:
          Fr_POCStatusPtr->State = FR_POCSTATE_STARTUP; /* int. listen */
          Fr_POCStatusPtr->StartupState = FR_STARTUP_INTEGRATION_LISTEN;
          break;
        case 40:
          Fr_POCStatusPtr->State = FR_POCSTATE_STARTUP; /* int. constsitency check */
          Fr_POCStatusPtr->StartupState = FR_STARTUP_INTEGRATION_CONSISTENCY_CHECK;
          break;
        case 41:
          Fr_POCStatusPtr->State = FR_POCSTATE_STARTUP; /* INITIALIZE SCHEDULE */
          Fr_POCStatusPtr->StartupState = FR_STARTUP_INITIALIZE_SCHEDULE;
          break;
        case 42:
          Fr_POCStatusPtr->State = FR_POCSTATE_STARTUP; /* ABORT STARTUP */
          Fr_POCStatusPtr->StartupState = FR_STARTUP_UNDEFINED;
          break;
        case 100:
          Fr_POCStatusPtr->State = FR_POCSTATE_STARTUP; /* SYNC PULSE RECEIVED */
          break;
        default:
          /* unknown Fr_GetPOCStateRealBus state */
          retVal = E_NOT_OK;
          break;
      }
      /* keep gSimulatedPOCState in "Real Bus state" identical to Fr_POCStatusPtr->State */
      gSimulatedPOCState[FR_VCTRL_IDX] = Fr_POCStatusPtr->State;
    }
    else
    {
      /* simulated bus, use simulated state */
      Fr_POCStatusPtr->State = gSimulatedPOCState[FR_VCTRL_IDX];
    }

    /* #60 Since resetting the CC in real bus mode means that the VN-Interface automatically begins the
     *     startup, the CANoeEmu driver must return READY as long StartCommunication has NOT been called. */
    /* PRQA S 4101, 4102, 4105 3 */ /* MD_Fr_4102 */
    if ( ((gStartupFlag[FR_VCTRL_IDX] == FALSE)
       & ((Fr_POCStatusPtr->State == FR_POCSTATE_STARTUP)
       |  (Fr_POCStatusPtr->State == FR_POCSTATE_NORMAL_ACTIVE))) != FALSE)
    {
      Fr_POCStatusPtr->State = FR_POCSTATE_READY;
      gSimulatedPOCState[FR_VCTRL_IDX] = Fr_POCStatusPtr->State;
    }

    /* PRQA S 4101, 4102, 4105 2 */ /* MD_Fr_4102 */
    if (((gStartupFlag[FR_VCTRL_IDX] == TRUE)
          & (Fr_POCStatusPtr->State == FR_POCSTATE_READY)) != FALSE)
    {
      /* After resetting the CC for real bus the CC might report POC state ready. However, the CC should stay in startup */
      Fr_POCStatusPtr->State = FR_POCSTATE_STARTUP;
      gSimulatedPOCState[FR_VCTRL_IDX] = Fr_POCStatusPtr->State;
    }

    Fr_POCStatusPtr->Freeze = FALSE;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_GET_POC_STATUS, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
  /* PRQA S 6080 3 */ /* MD_MSR_STMIF */
  /* PRQA S 6030 2 */ /* MD_MSR_STCYC */ 
  /* PRQA S 6010 1 */ /* MD_MSR_STPTH */ 
} /* Fr_GetPOCStatus() */ 

/***********************************************************************************************************************
 *  Fr_TransmitTxLPdu
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
#if ( FR_DIRECT_BUFFER_ACCESS_ENABLE == STD_ON )   
FUNC(Std_ReturnType, FR_CODE) Fr_TransmitTxLPdu_ImmediateDBA /* PRQA S 1503 */ /* MD_MSR_14.1 */
(
  FR_VCTRL_SYSTEMTYPE_ONLY, 
  uint16 Fr_LPduIdx,
  P2CONST(uint8, AUTOMATIC, FR_APPL_DATA) Fr_LPduPtr,
  uint8 Fr_LPduLength
) 
# else
FUNC(Std_ReturnType, FR_CODE) Fr_TransmitTxLPdu
(
  FR_VCTRL_SYSTEMTYPE_ONLY,
  uint16 Fr_LPduIdx,
  P2CONST(uint8, AUTOMATIC, FR_APPL_DATA) Fr_LPduPtr,
  uint8 Fr_LPduLength
)
#endif
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;
  uint8_least i;
  /* #10 Get the index to the associated virtual message buffer. */
  uint16 ConfigIndex = Fr_GetTxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx).mbIdx;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #20 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #30 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #40 If required, check that parameter Fr_LPduIdx is less than or equal to the total number of Tx frame triggerings. */
  else if (Fr_LPduIdx > Fr_VGetNumTxVirtBuf2PhysBufs(FR_VCTRL_IDX))
  {
    errorId = FR_E_INV_LPDU_IDX;
  }
  /* #50 If required, check that parameter Fr_LPduLength is less than or equal to the length configured for this frame triggering. */
  else if (Fr_LPduLength > (CCBUFS_CFG(FR_VCTRL_IDX)[ConfigIndex].WRHS2 >> FR_WRHS2_PLC_BYTE_SHIFT))
  {
    errorId = FR_E_INV_LENGTH;
  }
  /* #60 If required, check that Fr_LPduPtr points to a valid address. */
  else if (Fr_LPduPtr == NULL_PTR)
  {
    errorId = FR_E_INV_POINTER;
  }
  /* #70 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #80 Get corresponding message buffer configuration entry from Fr_VCcBufs of given Fr_LPduIdx. */
    uint32 baseCycle   = Fr_VCcCfgR_GetBaseCycle(FR_VCTRL_IDX, ConfigIndex);
    uint32 cycleRep    = Fr_VCcCfgR_GetCycleRepetition(FR_VCTRL_IDX, ConfigIndex);
    uint16 slotId      = Fr_VCcCfgR_GetSlotId(FR_VCTRL_IDX, ConfigIndex);
    uint32 channel     = Fr_VCcCfgR_GetChannel(FR_VCTRL_IDX, ConfigIndex);
    uint32 frameStatus = (slotId > gNumberOfStaticSlots[FR_VCTRL_IDX]) ? \
                                                  0x10 : (((uint16)slotId == gSyncSlotId[FR_VCTRL_IDX]) ? 0x13 : 0x10);

    if (slotId <= gNumberOfStaticSlots[FR_VCTRL_IDX])
    {
      Fr_LPduLength = (uint8)gStaticPayloadLengthInBytes[FR_VCTRL_IDX];
    }

    /* #90 Then transform Fr_LPduLength from unit byte to word. */
    if ((Fr_LPduLength & 0x01u) != 0)
    {
      /* round up to even length */
      Fr_LPduLength++;
    }

    /* #100 Trigger transmission of the L-PDU. */
    if(channel != 3)
    {
      VttFr_SendFlexRayMessage(
        FR_VCTRL_IDX,
        channel,
        slotId,
        baseCycle,
        cycleRep,
        frameStatus,
        Fr_LPduLength,
        Fr_LPduPtr);
    }
    else
    {
      VttFr_SendFlexRayMessage(
        FR_VCTRL_IDX,
        1,
        slotId,
        baseCycle,
        cycleRep,
        frameStatus,
        Fr_LPduLength,
        Fr_LPduPtr);

      VttFr_SendFlexRayMessage(
        FR_VCTRL_IDX,
        2,
        slotId,
        baseCycle,
        cycleRep,
        frameStatus,
        Fr_LPduLength,
        Fr_LPduPtr);
    }

    /* #110 Remember that frame was transmitted so that we do not receive our own transmitted data. */
    for(i = 1; i <= 2; i++)
    {
      if((channel & i) != 0)
      {
        uint8 cycleCode = Fr_VCcCfgR_GetCycleCode(FR_VCTRL_IDX, ConfigIndex);
        Fr_SetTxMsgBufTransmitted(FR_VCTRL_IDX, i, slotId, cycleCode, TRUE);
        Fr_SetTxMsgBufPending(FR_VCTRL_IDX, i, slotId, cycleCode, TRUE);
      }
    }

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
#if ( FR_DIRECT_BUFFER_ACCESS_ENABLE == STD_ON )   
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_TRANSMIT_TX_LPDU_IMMEDIATE_DBA, errorId);
#else
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_TRANSMIT_TX_LPDU, errorId);
#endif
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
  /* PRQA S 6080 3 */ /* MD_MSR_STMIF */
  /* PRQA S 6050 2 */ /* MD_MSR_STCAL */ 
  /* PRQA S 6030 1 */ /* MD_MSR_STCYC */ 
} /* Fr_TransmitTxLPdu() */ 

/***********************************************************************************************************************
 *  Fr_ReceiveRxLPdu
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
#if ( FR_DIRECT_BUFFER_ACCESS_ENABLE == STD_ON )   
FUNC(Std_ReturnType, FR_CODE) Fr_ReceiveRxLPdu_DBA   
(
  FR_VCTRL_SYSTEMTYPE_ONLY, 
  uint16 Fr_LPduIdx,  
  P2VAR(Fr_LPduPtrType, AUTOMATIC, FR_APPL_DATA) Fr_LPduPtr,
  P2VAR(Fr_RxLPduStatusType, AUTOMATIC, FR_APPL_DATA) Fr_LPduStatusPtr,
  P2VAR(uint8, AUTOMATIC, FR_APPL_DATA) Fr_LPduLengthPtr
)
#else
FUNC(Std_ReturnType, FR_CODE) Fr_ReceiveRxLPdu
(
  FR_VCTRL_SYSTEMTYPE_ONLY,
  uint16 Fr_LPduIdx,
  P2VAR(uint8, AUTOMATIC, FR_APPL_DATA) Fr_LPduPtr,
  P2VAR(Fr_RxLPduStatusType, AUTOMATIC, FR_APPL_DATA) Fr_LPduStatusPtr,
  P2VAR(uint8, AUTOMATIC, FR_APPL_DATA) Fr_LPduLengthPtr
)
#endif
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check that parameter Fr_LPduIdx is less than or equal to the total number of RX frame triggerings. */
  else if (Fr_LPduIdx > Fr_VGetNumRxVirtBuf2PhysBufs(FR_VCTRL_IDX))
  {
    errorId = FR_E_INV_LPDU_IDX;
  }
  /* #40 If required, check that Fr_LPduPtr, Fr_LPduStatusPtr and Fr_LPduLengthPtr point to a valid address. */
  else if ((Fr_LPduPtr == NULL_PTR) | (Fr_LPduStatusPtr == NULL_PTR) | (Fr_LPduLengthPtr == NULL_PTR))
  {
    errorId = FR_E_INV_POINTER;
  }
  /* #50 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 i;
    Fr_RxLPduStatusType rxStatus = FR_NOT_RECEIVED;
    uint16 ConfigIndex           = Fr_GetRxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx).mbIdx;
    uint16 slotId                = Fr_VCcCfgR_GetSlotId(FR_VCTRL_IDX, ConfigIndex);
    uint32 channel               = Fr_VCcCfgR_GetChannel(FR_VCTRL_IDX, ConfigIndex);

    for(i = 1; i <= 2; i++)
    {
      if((channel & i) != 0)
      {
        /* #60 If FR_FIFO_SUPPORT is enabled and the requested L-PDU lies within a FIFO range the following steps must be repeated until either:
        *      * a valid frame within the FIFO range was actually received (refer to 110).
        *      * no frame in the FIFO range was received (refer to 140).
        *     Otherwise the following steps are only processed for the requested L-PDU:*/
#if ( FR_FIFO_SUPPORT  ==  STD_ON)
        uint8 fifoDepth = 1;
        if ( (Fr_GetRxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx)).cycleMask == FR_FIFO_MARK) /* fifo frame */
        {
          fifoDepth = (Fr_GetRxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx)).cycleBase;
        }

        for (;fifoDepth > 0; fifoDepth--)
        {
          slotId = Fr_VCcCfgR_GetSlotId(FR_VCTRL_IDX, ConfigIndex);
#endif /* (FR_FIFO_SUPPORT == STD_ON) */

          /* #70 Check whether the following constraints hold true to determine whether the requested L-PDU was received or not: */

#if (FR_RX_STRINGENT_CHECK == STD_OFF)
          /* #80 * If FR_RX_STRINGENT_CHECK is enabled:
           *        Nothing must be considered because this feature is not supported by CANoeEmu driver. */
          /* [FR603][FR604] */
#endif /* FR_RX_STRINGENT_CHECK */

#if (FR_RX_STRINGENT_LENGTH_CHECK == STD_ON)
          /* #90 * If FR_RX_STRINGENT_LENGTH_CHECK is enabled:
           *        The configured payload length must be equal to the received one. */
          /* [FR645]  */
          if ((Fr_VCcCfgR_GetPayloadLengthInBytes(FR_VCTRL_IDX, ConfigIndex)) != /* Payload Length Configured */
              (Fr_GetRxMsgBufLength(FR_VCTRL_IDX, i, slotId))) /* Payload Length Received */
          {
            if ((0 < Fr_GetRxMsgBufLength(FR_VCTRL_IDX, i, slotId)))
            {
              rxStatus = FR_NOT_RECEIVED;
              retVal = E_OK;
# if ( FR_FIFO_SUPPORT  ==  STD_ON)
              break;
# endif
            }
          }
          else
#endif /* FR_RX_STRINGENT_LENGTH_CHECK */

          /* #100 * The message buffer of the requested L-PDU contains new valid data.
           *      * The L-PDU lies either in a FIFO range or its cycle filtering matches. */
           if ( (0 < Fr_GetRxMsgBufLength(FR_VCTRL_IDX, i,slotId)) && ((
#if ( FR_FIFO_SUPPORT  ==  STD_ON )
              ((Fr_GetRxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx)).cycleMask == FR_FIFO_MARK) ||
#endif
              (Fr_GetRxMsgBufCycle(FR_VCTRL_IDX, i, slotId) & (Fr_GetRxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx)).cycleMask) ==
                   ((Fr_GetRxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx)).cycleBase
#if (FR_RECONFIGLPDUSUPPORT == STD_ON)
                     & ((uint8)(~FR_RXVIRTBUF_CYCLEBASE_RECONFIG_LPDU))   /* PRQA S 0277 */ /* MD_Fr_0277 */
#endif
                  ))))
          /* #110 * If FR_VENABLE_MID_FILTERING is enabled:
           *        The message ID (first 2 bytes) of a received L-PDU in the dynamic segment with set payload preamble
           *        bit must be found in the message ID list of the driver. */
          {
#if ( FR_VENABLE_MID_FILTERING == STD_ON )

            if (FR_GETSIZE_MID_ARRAY(FR_VCTRL_IDX) > 0)
            {
              /* PRQA S 0715 1 */ /* MD_MSR_1.1_715 */
              if ((Fr_GetRxMsgBufFlags(FR_VCTRL_IDX, i, slotId) & PAYLOAD_PREAMBLE_BIT_MASK) == PAYLOAD_PREAMBLE_BIT_MASK)
              {
                if (slotId > FR_GETLAST_STATIC_FRAMEID(FR_VCTRL_IDX))  /* only dynamic frames */
                {
                  boolean msgIDfound = FALSE;
                  uint16 msgIDreceived;
                  uint8 tmp;
                  Fr_GetRxMsgBufData(FR_VCTRL_IDX, i, slotId, (uint8*)(&msgIDreceived), 2);

                  for (tmp = 0; tmp < FR_GETSIZE_MID_ARRAY(FR_VCTRL_IDX); tmp++) /* compare with MIDs from array */
                  {
                    if(msgIDreceived == FR_GETMIDFILTER(FR_VCTRL_IDX, tmp))
                    {
                      msgIDfound = TRUE;
                      break;        /* exit for */
                    }
                  }

                  if(msgIDfound != TRUE)  /* MID not found, quit receive function */
                  {
                    *Fr_LPduLengthPtr = 0;
                    rxStatus = FR_NOT_RECEIVED;
                    Fr_SetRxMsgBufRead(FR_VCTRL_IDX, i, slotId);
                    retVal = E_OK;
                    break;
                  }   /* if(msgIDfound != TRUE) */
                }     /* only for dynamic frames   if(slotId > FR_GETLAST_STATIC_FRAMEID)*/
              }       /* look if Preamble Bit is set */
            }   /* if(FR_GETSIZE_MID_ARRAY > 0) */
#endif /* ( FR_VENABLE_MID_FILTERING == STD_ON ) */

            /* #120 If all these constraints hold true:
             *      Copy the received payload to the buffer Fr_LPduPtr points to and set Fr_LPduLengthPtr to the frame length of the
             *      received L-PDU (number of copied bytes).
             *      Note: reading the message buffer data clears the "new data flag". */
#if ( STD_ON== FR_DIRECT_BUFFER_ACCESS_ENABLE  )   
            Fr_GetRxMsgBufData(FR_VCTRL_IDX, i, slotId,Fr_FrameBuf[FR_VCTRL_IDX][Fr_LPduIdx], Fr_GetRxMsgBufLength(FR_VCTRL_IDX, i, slotId));
            *Fr_LPduPtr = Fr_FrameBuf[FR_VCTRL_IDX][Fr_LPduIdx];
#else       
            Fr_GetRxMsgBufData(FR_VCTRL_IDX, i, slotId, Fr_LPduPtr, Fr_GetRxMsgBufLength(FR_VCTRL_IDX, i, slotId));
#endif
            *Fr_LPduLengthPtr = (uint8)Fr_GetRxMsgBufLength(FR_VCTRL_IDX, i, slotId);
            Fr_SetRxMsgBufRead(FR_VCTRL_IDX, i, slotId);
            retVal = E_OK;

#if ( FR_FIFO_SUPPORT  ==  STD_ON )
            if (fifoDepth > FR_LAST_FIFO_FRAME)
            {
              /* #130 If FR_FIFO_SUPPORT is enabled and this was not the last FIFO entry set Fr_LPduStatusPtr to FR_RECEIVED_MORE_DATA_AVAILABLE
               *     (to tell the caller that more data is available and Fr_ReceiveRxLPdu should be called for this L-PDU again). */
              rxStatus = FR_RECEIVED_MORE_DATA_AVAILABLE;
            }
            else            
#endif
            {
              /* #140 Otherwise: set Fr_LPduStatusPtr to FR_RECEIVED */
              rxStatus = FR_RECEIVED;
            }

            /* ESCAN00084558 */
            break;
          }
          else
          {
            /* #150 Otherwise: Set Fr_LPduLengthPtr to 0 and set Fr_LPduStatusPtr to FR_NOT_RECEIVED. */
            rxStatus = FR_NOT_RECEIVED;
            *Fr_LPduLengthPtr = 0;
            retVal = E_OK;
          }
#if ( FR_FIFO_SUPPORT  ==  STD_ON )
          /* Point to the next physical message buffer. */
          ConfigIndex++;
        } /* for (;fifoDepth > 0; fifoDepth--) */
#endif /* FR_VENABLE_MID_FILTERING  ==  STD_ON */
      }   /* if(channel & i) */
    }     /* for(i = 1; i <= 2; i++) */
    *Fr_LPduStatusPtr = rxStatus;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_RECEIVE_RX_LPDU, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
  /* PRQA S 6010 4 */ /* MD_MSR_STPTH */ 
  /* PRQA S 6030 3 */ /* MD_MSR_STCYC */ 
  /* PRQA S 6050 2 */ /* MD_MSR_STCAL */
  /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
} /* Fr_ReceiveRxLPdu() */ 

# if ( FR_DIRECT_BUFFER_ACCESS_ENABLE == STD_ON )
/***********************************************************************************************************************
 *  Fr_RequestBuffer_DBA
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
 */
FUNC(Std_ReturnType, FR_CODE) Fr_RequestBuffer_DBA /* PRQA S 1503 */ /* MD_MSR_14.1 */
(
  FR_VCTRL_SYSTEMTYPE_ONLY, 
  uint16 Fr_LPduIdx,
  P2VAR(Fr_LPduPtrType, AUTOMATIC, FR_APPL_DATA) Fr_LPduPtr,
  uint8 Fr_LPduLength
) 
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;
  const uint16 ConfigIndex = Fr_GetTxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx).mbIdx;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check that parameter Fr_LPduIdx is less than or equal to the total number of Rx frame triggerings. */
  else if (Fr_LPduIdx > Fr_VGetNumTxVirtBuf2PhysBufs(FR_VCTRL_IDX))
  {
    errorId = FR_E_INV_LPDU_IDX;
  }
  /* #40 If required, check that parameter Fr_LPduLength is less than or equal to the length configured for this frame triggering. */
  else if (Fr_LPduLength > (CCBUFS_CFG(FR_VCTRL_IDX)[ConfigIndex].WRHS2 >> FR_WRHS2_PLC_BYTE_SHIFT))
  {
    errorId = FR_E_INV_LENGTH;
  }
  /* #50 If required, check that Fr_LPduPtr, Fr_LPduStatusPtr and Fr_LPduLengthPtr point to a valid address. */
  else if (Fr_LPduPtr == NULL_PTR)
  {
    errorId = FR_E_INV_POINTER;
  }
  /* #60 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #70 Return the pointer to the frame buffer of the requested L-PDU */
    *Fr_LPduPtr = Fr_FrameBuf[FR_VCTRL_IDX][Fr_LPduIdx];
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_REQUEST_BUFFER_DBA, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
  /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
} /* Fr_RequestBuffer_DBA() */ 

/***********************************************************************************************************************
 *  Fr_UnlockRxLPdu_DBA
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
FUNC(Std_ReturnType, FR_CODE) Fr_UnlockRxLPdu_DBA /* PRQA S 1503 */ /* MD_MSR_14.1 */
(
  FR_VCTRL_SYSTEMTYPE_ONLY, 
  uint16 Fr_LPduIdx
) 
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check that parameter Fr_LPduIdx is less than or equal to the total number of Rx frame triggerings. */
  else if (Fr_LPduIdx > Fr_VGetNumRxVirtBuf2PhysBufs(FR_VCTRL_IDX))
  {
    errorId = FR_E_INV_LPDU_IDX;
  }
  /* #40 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Do nothing because CANoeEmu does not support a locking mechanism for the frame buffers */
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_UNLOCK_RX_LPDU_DBA, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
  /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
} /* Fr_UnlockRxLPdu_DBA() */ 

/***********************************************************************************************************************
 *  Fr_TransmitTxLPdu_DBA
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
FUNC(Std_ReturnType, FR_CODE) Fr_TransmitTxLPdu_DBA /* PRQA S 1503 */ /* MD_MSR_14.1 */
(
  FR_VCTRL_SYSTEMTYPE_ONLY, 
  uint16 Fr_LPduIdx,
  uint8 Fr_LPduLength
) 
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;  
  const uint16 ConfigIndex = Fr_GetTxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx).mbIdx;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check that parameter Fr_LPduIdx is less than or equal to the total number of Rx frame triggerings. */
  else if (Fr_LPduIdx > Fr_VGetNumTxVirtBuf2PhysBufs(FR_VCTRL_IDX))
  {
    errorId = FR_E_INV_LPDU_IDX;
  }
  /* #40 If required, check that parameter Fr_LPduLength is less than or equal to the length configured for this frame triggering. */
  else if (Fr_LPduLength > (CCBUFS_CFG(FR_VCTRL_IDX)[ConfigIndex].WRHS2 >> FR_WRHS2_PLC_BYTE_SHIFT))
  {
    errorId = FR_E_INV_LENGTH;
  }
  /* #50 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #60 Trigger the transmission of the prepared frame for the requested L-PDU */
    retVal = Fr_TransmitTxLPdu_ImmediateDBA(FR_VCTRL_IDX, Fr_LPduIdx, Fr_FrameBuf[FR_VCTRL_IDX][Fr_LPduIdx], Fr_LPduLength);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_TRANSMIT_TX_LPDU_DBA, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
  /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
} /* Fr_TransmitTxLPdu_DBA() */ 
#endif

/***********************************************************************************************************************
 *  Fr_CheckTxLPduStatus
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
 */
FUNC(Std_ReturnType, FR_CODE) Fr_CheckTxLPduStatus
(
  FR_VCTRL_SYSTEMTYPE_ONLY,
  uint16 Fr_LPduIdx,
  P2VAR(Fr_TxLPduStatusType, AUTOMATIC, FR_APPL_DATA) Fr_TxLPduStatusPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check plausibility of input parameter 'Fr_CtrlIdx'. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check that input parameter 'Fr_LPduIdx' is less than or equal to the total number of TX frame triggerings. */
  else if (Fr_LPduIdx >= Fr_VGetNumTxVirtBuf2PhysBufs(FR_VCTRL_IDX))
  {
    errorId = FR_E_INV_LPDU_IDX;
  }
  /* #40 If required, check plausibility of output parameter 'Fr_TxLPduStatusPtr'. */
  else if (Fr_TxLPduStatusPtr == NULL_PTR)
  {
    errorId = FR_E_INV_POINTER;
  }
  /* #50 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 i;
    Fr_TxLPduStatusType txStatus = FR_NOT_TRANSMITTED;
    const uint16 ConfigIndex     = Fr_GetTxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx).mbIdx;
    uint16 slotId                = Fr_VCcCfgR_GetSlotId(FR_VCTRL_IDX, ConfigIndex);
    uint32 channel               = Fr_VCcCfgR_GetChannel(FR_VCTRL_IDX, ConfigIndex);

    for(i = 1; i <= 2; i++)
    {
      if((channel & i) != FALSE)
      {
        uint8 cycleCode = Fr_VCcCfgR_GetCycleCode(FR_VCTRL_IDX, ConfigIndex);

        /* #60 Check whether really the frame requested by Fr_LPduIdx is in the associated message buffer. */
        if (FALSE == Fr_GetTxMsgBufPending(FR_VCTRL_IDX, i, slotId, cycleCode))
        {
          /* #70 If so then retrieve Transmission Request flag (TXR) of corresponding message buffer and tell the caller if the transmission is pending or not. */
          txStatus = FR_TRANSMITTED;
          break;
        }
      }
    }

    *Fr_TxLPduStatusPtr = txStatus;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_CHECK_TX_LPDU_STATUS, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
  /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
} /* Fr_CheckTxLPduStatus() */ 

/***********************************************************************************************************************
 *  Fr_GetGlobalTime
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
FUNC(Std_ReturnType, FR_CODE) Fr_GetGlobalTime
(
  FR_VCTRL_SYSTEMTYPE_ONLY,
  P2VAR(uint8,  AUTOMATIC, FR_APPL_DATA) Fr_CyclePtr,
  P2VAR(uint16, AUTOMATIC, FR_APPL_DATA) Fr_MacroTickPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check plausibility of input parameter 'Fr_CtrlIdx'. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check plausibility of output parameters 'Fr_CyclePtr' and 'Fr_MacroTickPtr'. */
  else if ((Fr_CyclePtr == NULL_PTR) | (Fr_MacroTickPtr == NULL_PTR))
  {
    errorId = FR_E_INV_POINTER;
  }
  /* #40 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Return the macrotick and cylce of the global FlexRay time */
    /* PRQA S 4101, 4102, 4105 1 */ /* MD_Fr_4102 */
    uint32 pocState = (Fr_VCCGetPocState(FR_VCTRL_IDX) == FR_POCSTATE_NORMAL_ACTIVE) | (Fr_VCCGetPocState(FR_VCTRL_IDX) == FR_POCSTATE_NORMAL_PASSIVE); 
    if (pocState != FALSE)
    {
      uint16 tmpMacro = (uint16)Fr_VGetCurrentMacroTick(FR_VCTRL_IDX);

      *Fr_CyclePtr     = (gCurrentCycle[FR_VCTRL_IDX] + (uint8)(tmpMacro / gMacroticksPerCycle[FR_VCTRL_IDX])) % 64;
      *Fr_MacroTickPtr = (uint16)(tmpMacro % gMacroticksPerCycle[FR_VCTRL_IDX]);

      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_GET_GLOBAL_TIME, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* Fr_GetGlobalTime() */

/***********************************************************************************************************************
 *  Fr_GetNmVector
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
#if (FR_NMVECTOR_ENABLE == STD_ON)  /* FR266 */
FUNC(Std_ReturnType, FR_CODE) Fr_GetNmVector
(
  FR_VCTRL_SYSTEMTYPE_ONLY,
  P2VAR(uint8, AUTOMATIC, FR_APPL_DATA) Fr_NmVectorPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

   /* ----- Development Error Checks ------------------------------------- */
# if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
#  if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check plausibility of input parameter 'Fr_CtrlIdx'. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
#  endif
  /* #30 If required, check plausibility of output parameter 'Fr_NmVectorPtr'. */
  else if (Fr_NmVectorPtr == NULL_PTR)
  {
    errorId = FR_E_INV_POINTER;
  }
  /* #40 If the initial checks are passed or not required: */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Return the received NM vector configuration */
    (void)Fr_GetCanoeNMVector(FR_VCTRL_IDX, Fr_NmVectorPtr);
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_GET_NM_VECTOR, errorId);
  }
# else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* Fr_GetNmVector() */
#endif
 /* FR_NMVECTOR_ENABLE */

#if (FR_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Fr_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, FR_CODE) Fr_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, FR_APPL_DATA) Fr_VersionInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check plausibility of all input parameters. */
  if (Fr_VersionInfoPtr == NULL_PTR)
  {
    errorId = FR_E_INV_POINTER;
  }
  /* #20 If the initial checks are passed or not required: */
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */

    /* #30 Fill the version information struct using the macros from the component header. */
    Fr_VersionInfoPtr->vendorID = FR_VENDOR_ID;
    Fr_VersionInfoPtr->moduleID = FR_MODULE_ID;
    
    Fr_VersionInfoPtr->sw_major_version = (uint8)FR_SW_MAJOR_VERSION;
    Fr_VersionInfoPtr->sw_minor_version = (uint8)FR_SW_MINOR_VERSION;
    Fr_VersionInfoPtr->sw_patch_version = (uint8)FR_SW_PATCH_VERSION;
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_GET_VERSION_INFO, errorId);
  }
#  else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#  endif
} /* Fr_GetVersionInfo() */
#endif /* FR_VERSION_INFO_API */

/***********************************************************************************************************************
 *  Fr_InitMemory
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
FUNC(void, FR_CODE) Fr_InitMemory(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ctrlId;
  uint16 i;
#if (Fr_IsPostbuild())
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(Fr_VConfigType, AUTOMATIC, FR_PBCFG) Fr_ConfigPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Call the callback to get the configuration struct for the FR driver and store the pointer to the struct for the
   *     initialization in Fr_Init(). */
#if (FR_VTT == STD_OFF) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
  ApplFr_GetFrConfigPtr((void*)&Fr_ConfigPtr);
#else
  Fr_ConfigPtr = VttFr_GetConfigPtr();
#endif
  Fr_VSetGlobalFrConfigPtr(Fr_ConfigPtr);
#endif

  /* ----- Implementation ----------------------------------------------- */
#if ( FR_DEV_ERROR_DETECT == STD_ON)
  /* #20 If FR_DEV_ERROR_DETECT is enabled, flag the FR as uninitialized.*/
  Fr_VFrDrvState = FR_VUNINITPTR;
#endif
  
  /* #30 Initialize controller low-level registers according config structs. */
  for (ctrlId = 0; ctrlId < FR_NUM_CTRL_USED; ctrlId++)
  {
#if (FR_VTT == STD_OFF) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
    CANoeAPI_MapChannel((const char*)Fr_CANoeClusterName[ctrlId],(uint32)CANOEAPI_BUSTYPE_FLEXRAY, ctrlId);
#endif
    Fr_InitFrCcRegVals(ctrlId);

    gSyncSlotId[ctrlId] = Fr_VCcCfgR_GetSyncSlotId(ctrlId);
    gNumberOfStaticSlots[ctrlId] = Fr_VCcCfgR_GetNumberOfStaticSlots(ctrlId);
    gStaticPayloadLengthInBytes[ctrlId] = Fr_VCcCfgR_GetStaticPayloadLengthInBytes(ctrlId);

    /* #30 Read the message buffer configuration from the config structs. */
    for (i = 0; i < (Fr_VGetNumRxVirtBuf2PhysBufs(ctrlId) + Fr_VGetNumTxVirtBuf2PhysBufs(ctrlId)); i++)
    {
      uint16 ConfigIndex = Fr_VCcCfgR_GetPhysicalBufferNumber(ctrlId, i);

      Fr_VLocCcBufs[ctrlId][ConfigIndex].WRHS1 = CCBUFS_CFG(ctrlId)[ConfigIndex].WRHS1;
      Fr_VLocCcBufs[ctrlId][ConfigIndex].WRHS2 = CCBUFS_CFG(ctrlId)[ConfigIndex].WRHS2;
      Fr_VLocCcBufs[ctrlId][ConfigIndex].WRHS3 = CCBUFS_CFG(ctrlId)[ConfigIndex].WRHS3;
      Fr_VLocCcBufs[ctrlId][ConfigIndex].IBCR  = CCBUFS_CFG(ctrlId)[ConfigIndex].IBCR;
      Fr_VLocCcBufs[ctrlId][ConfigIndex].Info  = CCBUFS_CFG(ctrlId)[ConfigIndex].Info;
    }
  }
} /* Fr_InitMemory() */

#if (FR_GET_CLOCK_CORRECTION_EXT == STD_ON)
/**********************************************************************************************************************
 *  Fr_GetClockCorrection()
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
FUNC(Std_ReturnType, FR_CODE) Fr_GetClockCorrection
(
 FR_VCTRL_SYSTEMTYPE_ONLY,
 P2VAR(sint16, AUTOMATIC, FR_APPL_DATA) Fr_RateCorrectionPtr,
 P2VAR(sint32, AUTOMATIC, FR_APPL_DATA) Fr_OffsetCorrectionPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
#  if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check plausibility of input parameter 'Fr_CtrlIdx'. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
#  endif
  /* #30 If required, check plausibility of output parameter 'Fr_RateCorrectionPtr' and 'Fr_OffsetCorrectionPtr'. */
  else if ((Fr_RateCorrectionPtr == NULL_PTR) | (Fr_OffsetCorrectionPtr == NULL_PTR))
  {
    errorId = FR_E_INV_POINTER;
  }
  /* #40 If the initial checks are passed or not required: */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Do nothing because API service is currently not supported by CANoeEmu driver */
    *Fr_RateCorrectionPtr = 0x0000;
    *Fr_OffsetCorrectionPtr = 0x00000000;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_GET_CLOCK_CORRECTION, errorId);
  }
# else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* Fr_GetClockCorrection() */
#endif /* FR_GET_CLOCK_CORRECTION_EXT */

#if (FR_VCHANNEL_STATUS_EXT == STD_ON)
/***********************************************************************************************************************
 *  Fr_GetChannelStatus
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
FUNC(Std_ReturnType, FR_CODE) Fr_GetChannelStatus
(
  FR_VCTRL_SYSTEMTYPE_ONLY,
  P2VAR(uint16, AUTOMATIC, FR_APPL_DATA) Fr_ChannelAStatusPtr,
  P2VAR(uint16, AUTOMATIC, FR_APPL_DATA) Fr_ChannelBStatusPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
#  if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check plausibility of input parameter 'Fr_CtrlIdx'. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
#  endif
  /* #30 If required, check plausibility of output parameters 'Fr_ChannelAStatusPtr' and 'Fr_ChannelBStatusPtr'. */
  else if ((Fr_ChannelAStatusPtr == NULL_PTR) | (Fr_ChannelBStatusPtr == NULL_PTR))
  {
    errorId = FR_E_INV_POINTER;
  }
  /* #40 If the initial checks are passed or not required: */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 If the channel status information is valid and up to date (synchronized  CC):
            Indicate vSS!ValidFrame because API service is currently not supported by CANoeEmu driver */
    /* PRQA S 4101, 4102, 4105 1 */ /* MD_Fr_4102 */
    uint32 pocState = (Fr_VCCGetPocState(FR_VCTRL_IDX) == FR_POCSTATE_NORMAL_ACTIVE) | (Fr_VCCGetPocState(FR_VCTRL_IDX) == FR_POCSTATE_NORMAL_PASSIVE);
    if (pocState != FALSE)
    {  
      *Fr_ChannelAStatusPtr = 0x0001;
      *Fr_ChannelBStatusPtr = 0x0001;
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_GET_CHANNEL_STATUS, errorId);
  }
# else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* Fr_GetChannelStatus() */
#endif /* (FR_VCHANNEL_STATUS_EXT == STD_ON) */

#if ( FR_GETSYNCFRAMELIST_API == STD_ON )
/***********************************************************************************************************************
 *  Fr_GetSyncFrameList
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
 *
 *
 *
 */
FUNC(Std_ReturnType, FR_CODE) Fr_GetSyncFrameList
(
  FR_VCTRL_SYSTEMTYPE_ONLY,
  uint8 Fr_ListSize,
  P2VAR(uint16, AUTOMATIC, FR_APPL_DATA) Fr_ChannelAEvenListPtr,
  P2VAR(uint16, AUTOMATIC, FR_APPL_DATA) Fr_ChannelBEvenListPtr,
  P2VAR(uint16, AUTOMATIC, FR_APPL_DATA) Fr_ChannelAOddListPtr,
  P2VAR(uint16, AUTOMATIC, FR_APPL_DATA) Fr_ChannelBOddListPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;
  uint8_least  n;
  uint8 ChannelAEvenListCount = 0;
  uint8 ChannelBEvenListCount = 0;
  uint8 ChannelAOddListCount  = 0;
  uint8 ChannelBOddListCount  = 0;

  /* ----- Development Error Checks ------------------------------------- */
# if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState == FR_VUNINITPTR)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
#  if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check plausibility of input parameter 'Fr_CtrlIdx'. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
#  endif
  /* #30 If required, check plausibility of output parameters 'Fr_ChannelAEvenListPtr' and 'Fr_ChannelBEvenListPtr'. */
  else if ((Fr_ChannelAEvenListPtr == NULL_PTR) | (Fr_ChannelBEvenListPtr == NULL_PTR))
  {
    errorId = FR_E_INV_POINTER;
  }
  /* #40 If required, check plausibility of output parameters 'Fr_ChannelAOddListPtr' and 'Fr_ChannelBOddListPtr'. */
  else if ((Fr_ChannelAOddListPtr == NULL_PTR) | (Fr_ChannelBOddListPtr == NULL_PTR))
  {
    errorId = FR_E_INV_POINTER;
  }
  /* #50 If the initial checks are passed or not required: */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    uint16 slotId;
    
    /* #60 Limit Fr_ListSize to 15 (FR_SYNC_FRAME_LIST_MAX). */
    if (Fr_ListSize > FR_SYNC_FRAME_LIST_MAX)
    {
      errorId = FR_E_INV_LISTSIZE;
      Fr_ListSize = FR_SYNC_FRAME_LIST_MAX;
    }

    /* #70 Initialize all entries of the even and odd arrays with 0. */
    for (n = 0; n < Fr_ListSize; n++)
    {
      Fr_ChannelAEvenListPtr[n] = 0;
      Fr_ChannelBEvenListPtr[n] = 0;
      Fr_ChannelAOddListPtr[n] = 0;
      Fr_ChannelBOddListPtr[n] = 0;
    }

    /* #80 Iterate over the 15 Even and Odd Sync ID registers that hold the sync frame IDs of the even and odd cycles. */
    for (n = 0; n < 1; n++)
    {
      slotId = Fr_VCcCfgR_GetSyncSlotId(FR_VCTRL_IDX);
      if (Fr_VCcCfgR_GetSyncSlotId(FR_VCTRL_IDX) != 0)
      {
        /* #90 Only save the current frame ID to the Fr_ChannelAEvenListPtr list if indeed a sync frame of an even cycle
         *      on channel A has been received and increment ChannelAEvenListCount. */
        uint32 channel = Fr_VCcCfgR_GetChannel(FR_VCTRL_IDX, 0);
        if ((channel & 1) != 0 /* A */)
        {
          if(ChannelAEvenListCount < Fr_ListSize)
          {
            Fr_ChannelAEvenListPtr[ChannelAEvenListCount] = slotId;
            ChannelAEvenListCount++;
          }
          
          if(ChannelAOddListCount < Fr_ListSize)
          {
            Fr_ChannelAOddListPtr[ChannelAOddListCount] = slotId;
            ChannelAOddListCount++;
          }
        }

        /* #100 Only save the current frame ID to the Fr_ChannelBEvenListPtr list if indeed a sync frame of an even cycle
         *      on channel B has been received and increment ChannelBEvenListCount. */
        if ((channel & 2) != 0 /* B */)
        {
          if(ChannelBEvenListCount < Fr_ListSize)
          {
            Fr_ChannelBEvenListPtr[ChannelBEvenListCount] = slotId;
            ChannelBEvenListCount++;
          }
          
          if(ChannelBOddListCount < Fr_ListSize)
          {
            Fr_ChannelBOddListPtr[ChannelBOddListCount] = slotId;
            ChannelBOddListCount++;
          }
        }
      }

      for(slotId = 0; slotId < 128; slotId++)
      {
        if (slotId != Fr_VCcCfgR_GetSyncSlotId(FR_VCTRL_IDX))
        {
          if((Fr_GetRxMsgBufFlags(FR_VCTRL_IDX,1, slotId) & FR_FLAGS_SYNCFRAME) != 0 )
          {
            /* #110 Only save the current frame ID to the Fr_ChannelAOddListPtr list if indeed a sync frame of an odd cycle
             *      on channel A has been received and increment ChannelAOddListCount. */
            if(ChannelAEvenListCount < Fr_ListSize)
            {
              Fr_ChannelAEvenListPtr[ChannelAEvenListCount] = slotId;
              ChannelAEvenListCount++;
            }
            
            if(ChannelAOddListCount < Fr_ListSize)
            {
              Fr_ChannelAOddListPtr[ChannelAOddListCount] = slotId;
              ChannelAOddListCount++;
            }
          }
          
          if((Fr_GetRxMsgBufFlags(FR_VCTRL_IDX, 2,slotId) & FR_FLAGS_SYNCFRAME) != 0 )
          {
            /* #120 Only save the current frame ID to the Fr_ChannelBOddListPtr list if indeed a sync frame of an odd cycle
             *      on channel B has been received and increment ChannelBOddListCount. */
            if(ChannelBEvenListCount < Fr_ListSize)
            {
              Fr_ChannelBEvenListPtr[ChannelBEvenListCount] = slotId;
              ChannelBEvenListCount++;
            }

            if(ChannelBOddListCount < Fr_ListSize)
            {
              Fr_ChannelBOddListPtr[ChannelBOddListCount] = slotId;
              ChannelBOddListCount++;
            }
          }
        }
      }
    }

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_GET_SYNC_FRAME_LIST, errorId);
  }
# else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
  /* PRQA S 6080 4 */ /* MD_MSR_STMIF */
  /* PRQA S 6060 3 */ /* MD_MSR_STPAR */ 
  /* PRQA S 6010 2 */ /* MD_MSR_STPTH */ 
  /* PRQA S 6030 1 */ /* MD_MSR_STCYC */ 
} /* Fr_GetSyncFrameList() */ 
#endif /* ( FR_GETSYNCFRAMELIST_API == STD_ON ) */

#if (FR_DISABLELPDUSUPPORT == STD_ON)
/***********************************************************************************************************************
 *  Fr_DisableLPdu
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
FUNC(Std_ReturnType, FR_CODE) Fr_DisableLPdu
(
  FR_VCTRL_SYSTEMTYPE_ONLY, 
  uint16 Fr_LPduIdx
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;
  uint8 cfgBufIdx;
  boolean reconfigurable = TRUE;

  /* ----- Development Error Checks ------------------------------------- */
# if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
#  if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check plausibility of input parameter 'Fr_CtrlIdx'. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
#  endif
  /* #30 If required, check that input parameter 'Fr_LPduIdx' is less than or equal to the total number of TX and RX
   *     frame triggerings. */
  else if (Fr_LPduIdx >= (Fr_VGetNumTxVirtBuf2PhysBufs(FR_VCTRL_IDX) + Fr_VGetNumRxVirtBuf2PhysBufs(FR_VCTRL_IDX)))
  {
    errorId = FR_E_INV_LPDU_IDX;
  }
  /* #40 If the initial checks are passed or not required: */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Determine the physical buffer index depending on whether the Fr_LPduIdx is a virtual buffer index for a TX
     *     or a RX frame triggering.
     *     In case the Fr_LPduIdx was less than the number of TX frame triggerings it is a TX frame triggering.
     *     Otherwise it is a RX frame triggering. */
    if (Fr_LPduIdx < Fr_VGetNumTxVirtBuf2PhysBufs(FR_VCTRL_IDX))
    {   /* TX */
      cfgBufIdx = (Fr_GetTxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx)).mbIdx;

      if ((Fr_GetTxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx).Info & FR_TXVIRTBUF_INFO_RECONFIG_LPDU) == 0)
      {
        reconfigurable = FALSE;
      }
    }
    else
    { /* RX */
      /* Subtract the number of TX frame triggerings because the indexes of Rx frame triggerings are zero based. */
      Fr_LPduIdx = (uint16)(Fr_LPduIdx - Fr_VGetNumTxVirtBuf2PhysBufs(FR_VCTRL_IDX));
      cfgBufIdx = (Fr_GetRxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx)).mbIdx;

      if ((Fr_GetRxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx).cycleBase & FR_RXVIRTBUF_CYCLEBASE_RECONFIG_LPDU) == 0)
      {
        reconfigurable = FALSE;
      }
    }

    /* #60 If the requested physical message buffer is marked as reconfigurable: */
    if (reconfigurable == TRUE)
    {
      /* #70 Do nothing because API service is currently not supported by CANoeEmu driver. */
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_DISABLE_LPDU, errorId);
  }
# else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* Fr_DisableLPdu() */
#endif

#if (FR_RECONFIGLPDUSUPPORT == STD_ON)
/***********************************************************************************************************************
 *  Fr_ReconfigLPdu
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
 *
 */
FUNC(Std_ReturnType, FR_CODE) Fr_ReconfigLPdu
(
   FR_VCTRL_SYSTEMTYPE_ONLY,
   uint16 Fr_LPduIdx,
   uint16 Fr_FrameId,
   Fr_ChannelType Fr_ChnlIdx,
   uint8 Fr_CycleRepetition,
   uint8 Fr_CycleOffset,
   uint8 Fr_PayloadLength,
   uint16 Fr_HeaderCRC
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;
  uint8 cfgBufIdx;
  boolean reconfigurable = TRUE;

  /* #10 Determine the physical buffer index depending on whether the Fr_LPduIdx is a virtual buffer index for a TX or
   *     a RX frame triggering. In case the Fr_LPduIdx was less than the number of TX frame triggerings it is a TX
   *     frame triggering. Otherwise it is a RX frame triggering. */
  if (Fr_LPduIdx < Fr_VGetNumTxVirtBuf2PhysBufs(FR_VCTRL_IDX))
  {   /* TX */
    cfgBufIdx = (Fr_GetTxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx)).mbIdx;

    if ((Fr_GetTxVirtBuf2Phys(FR_VCTRL_IDX, Fr_LPduIdx).Info & FR_TXVIRTBUF_INFO_RECONFIG_LPDU) == 0)
    {
      reconfigurable = FALSE;
    }
  }
  else
  { /* RX */
    uint16 tmpLPduIdx;

    tmpLPduIdx = Fr_LPduIdx - Fr_VGetNumTxVirtBuf2PhysBufs(FR_VCTRL_IDX);
    cfgBufIdx = (Fr_GetRxVirtBuf2Phys(FR_VCTRL_IDX, tmpLPduIdx)).mbIdx;

    if ((Fr_GetRxVirtBuf2Phys(FR_VCTRL_IDX, tmpLPduIdx).cycleBase & FR_RXVIRTBUF_CYCLEBASE_RECONFIG_LPDU) == 0)
    {
      reconfigurable = FALSE;
    }
  }

  /* ----- Development Error Checks ------------------------------------- */
# if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #20 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
#  if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #30 If required, check plausibility of input parameter 'Fr_CtrlIdx'. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
#  endif
  /* #40 If required, check that parameter Fr_ChnlIdx is either FR_CHANNEL_A or FR_CHANNEL_B. */
  else if ((Fr_ChnlIdx != FR_CHANNEL_A) && (Fr_ChnlIdx != FR_CHANNEL_B))
  {
    errorId = FR_E_INV_CHNL_IDX;
  }
  /* #50 If required, check that parameter Fr_LPduIdx is less than or equal to the total number of Tx and Rx frame triggerings. */
  else if (Fr_LPduIdx > (Fr_VGetNumTxVirtBuf2PhysBufs(FR_VCTRL_IDX) + Fr_VGetNumRxVirtBuf2PhysBufs(FR_VCTRL_IDX)))
  {
    errorId = FR_E_INV_LPDU_IDX;
  }

  /* #60 If required, check that input parameter 'Fr_PayloadLength' is less than or equal to the length configured
   *     for the physical message buffer. */
  else if (Fr_PayloadLength > (CCBUFS_CFG(FR_VCTRL_IDX)[cfgBufIdx].WRHS2 >> FR_WRHS2_PLC_BYTE_SHIFT))
  {
    errorId = FR_E_INV_LENGTH;
  }
  /* #70 If required, check that input parameter 'Fr_CycleRepetition' has a valid value. */
  /* According to AR 4.0.3 there are more valid values for Fr_CycleRepetition, but they are only relevant for FlexRay
   * 3.0 Hardware.
   */
  else if (((Fr_CycleRepetition !=  1) && \
            (Fr_CycleRepetition !=  2) && \
            (Fr_CycleRepetition !=  4) && \
            (Fr_CycleRepetition !=  8) && \
            (Fr_CycleRepetition != 16) && \
            (Fr_CycleRepetition != 32) && \
            (Fr_CycleRepetition != 64)))
  {
    errorId = FR_E_INV_CYCLE;
  }
  /* #80 If required, check that input parameter 'Fr_CycleOffset' (BaseCycle) is always less than Fr_CycleRepetition. */
  else if (Fr_CycleOffset >= Fr_CycleRepetition)
  {
    errorId = FR_E_INV_CYCLE;
  }
  /* #90 If required, check that input parameter 'Fr_HeaderCRC' value is between 0 and 2047 (0x7FF). */
  else if (Fr_HeaderCRC > FR_HEADER_CRC_MAX)
  {
    errorId = FR_E_INV_HEADERCRC;
  }
  /* #100 If the initial checks are passed or not required: */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    FR_DUMMY_STATEMENT(Fr_FrameId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
    
    if (reconfigurable == TRUE)
    {
      /* #110 Do nothing because API service is currently not supported by CANoeEmu driver */
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_RECONFIG_LPDU, errorId);
  }
# else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
  /* PRQA S 6080 3 */ /* MD_MSR_STMIF */
  /* PRQA S 6030 2 */ /* MD_MSR_STCYC */
  /* PRQA S 6060 1 */ /* MD_MSR_STPAR */
} /* Fr_ReconfigLPdu() */ 
#endif /* (FR_RECONFIGLPDUSUPPORT == STD_ON) */

#if (FR_READ_CC_CONFIGURATION == STD_ON)
/***********************************************************************************************************************
 *  Fr_ReadCCConfig
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
 */
FUNC(Std_ReturnType, FR_CODE) Fr_ReadCCConfig
(
  FR_VCTRL_SYSTEMTYPE_ONLY,
  uint8 Fr_ConfigParamIdx,
  P2VAR(uint32, AUTOMATIC, FR_APPL_DATA) Fr_ConfigParamValuePtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
#  if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check plausibility of input parameter 'Fr_CtrlIdx'. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
#  endif
  /* #30 If required, check plausibility of output parameter 'Fr_ConfigParamValuePtr'. */
  else if (Fr_ConfigParamValuePtr == NULL_PTR)
  {
    errorId = FR_E_INV_POINTER;
  }
  /* #40 If required, check that input parameter 'Fr_ConfigParamIdx' is less than or equal to the total number of all
   *     config parameters. */
  else if (Fr_ConfigParamIdx >= (uint8)(FR_CC_CONFIG_PARAM_MAX8BIT + FR_CC_CONFIG_PARAM_MAX16BIT + FR_CC_CONFIG_PARAM_MAX32BIT))
  {
    errorId = FR_E_INV_CONFIG_IDX;
  }
  /* #50 If the initial checks are passed or not required: */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */

    /* #60 Return the corresponding parameter via Fr_ConfigParamValuePtr for the given Fr_ConfigParamIdx. */
    if (Fr_ConfigParamIdx < FR_CC_CONFIG_PARAM_MAX32BIT)
    {
      *Fr_ConfigParamValuePtr = Fr_GetVCCConfigParam32Bit(FR_VCTRL_IDX, Fr_ConfigParamIdx);
    }
    else
    {
      if (Fr_ConfigParamIdx < (FR_CC_CONFIG_PARAM_MAX32BIT + FR_CC_CONFIG_PARAM_MAX16BIT))
      {
        *Fr_ConfigParamValuePtr = Fr_GetVCCConfigParam16Bit(FR_VCTRL_IDX, Fr_ConfigParamIdx - FR_CC_CONFIG_PARAM_MAX32BIT);
      }
      else
      {
        *Fr_ConfigParamValuePtr = Fr_GetVCCConfigParam8Bit(FR_VCTRL_IDX, Fr_ConfigParamIdx - (FR_CC_CONFIG_PARAM_MAX16BIT + FR_CC_CONFIG_PARAM_MAX32BIT));
      }
    }

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_READCCCONFIG, errorId);
  }
# else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
  /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
} /* Fr_ReadCCConfig() */
#endif /* FR_READ_CC_CONFIGURATION == STD_ON */

/***********************************************************************************************************************
 *  Fr_GetNumOfStartupFrames 
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
FUNC(Std_ReturnType, FR_CODE) Fr_GetNumOfStartupFrames
(
 FR_VCTRL_SYSTEMTYPE_ONLY,
 P2VAR(uint8, AUTOMATIC, FR_APPL_DATA) Fr_NumOfStartupFramesPtr
)
{ 
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  uint32 pocState = (Fr_VCCGetPocState(FR_VCTRL_IDX) == FR_POCSTATE_NORMAL_ACTIVE) | (Fr_VCCGetPocState(FR_VCTRL_IDX) == FR_POCSTATE_NORMAL_PASSIVE);

  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check plausibility of input parameter 'Fr_CtrlIdx'. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check plausibility of output parameter 'Fr_NumOfStartupFramesPtr'. */
  else if (Fr_NumOfStartupFramesPtr == NULL_PTR)
  {
    errorId = FR_E_INV_POINTER;
  }
  else if (!pocState)
  {
    errorId = FR_E_INV_POCSTATE;
  }
  /* #40 If the initial checks are passed or not required: */
  else
#endif
  {
    /* #50 Set Fr_NumOfStartupFramesPtr to 2 startup frames (always the same for FlexRay 2.1 Rev A hardware). */
    *Fr_NumOfStartupFramesPtr = 0x02;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_GETNUMOFSTARTUPFRAMES, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* Fr_GetNumOfStartupFrames() */

/***********************************************************************************************************************
 *  Fr_GetWakeupRxStatus
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
FUNC(Std_ReturnType, FR_CODE) Fr_GetWakeupRxStatus
(
  FR_VCTRL_SYSTEMTYPE_ONLY,
  P2VAR(uint8, AUTOMATIC, FR_APPL_DATA) Fr_WakeupRxStatusPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check plausibility of input parameter 'Fr_CtrlIdx'. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check plausibility of output parameter 'Fr_WakeupRxStatusPtr'. */
  else if (Fr_WakeupRxStatusPtr == NULL_PTR)
  {
    errorId = FR_E_INV_POINTER;
  }
  /* #40 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Determine whether a wakeup pattern was received on channel A or channel B */
    if (wakeupChannelValid[FR_VCTRL_IDX] == TRUE)
    {
      if (wakeupChannel[FR_VCTRL_IDX] == FR_CHANNEL_A)
      {
        *Fr_WakeupRxStatusPtr |= FR_WAKEUPRX_ON_CHANNEL_A;
      }
      else
      {
        *Fr_WakeupRxStatusPtr |= FR_WAKEUPRX_ON_CHANNEL_B;
      }
    }
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_GETWAKEUPRXSTATUS, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
  /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
} /* Fr_GetWakeupRxStatus() */ 

/**********************************************************************************************************************
 *  Fr_CancelTxLPdu()
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
FUNC(Std_ReturnType, FR_CODE) Fr_CancelTxLPdu
(
  FR_VCTRL_SYSTEMTYPE_ONLY,
  uint16 Fr_LPduIdx
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check plausibility of input parameter 'Fr_CtrlIdx'. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
/* #30 If required, check that input parameter Fr_LPduIdx is less than or equal to the total number of TX and RX frame
 *     triggerings. */
  else if (Fr_LPduIdx >= Fr_VGetNumTxVirtBuf2PhysBufs(FR_VCTRL_IDX))
  {
    errorId = FR_E_INV_LPDU_IDX;
  }
  /* #40 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Do nothing because API service is currently not supported by CANoeEmu driver */
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_CANCELTXLPDU, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* Fr_CancelTxLPdu() */

/**********************************************************************************************************************
 *  Fr_AllSlots()
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
FUNC(Std_ReturnType, FR_CODE) Fr_AllSlots
(
  FR_VCTRL_SYSTEMTYPE_ONLY
)
{ /* PRQA S 3007 */ /* MD_Fr_3007 */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  uint32 pocState = (Fr_VCCGetPocState(FR_VCTRL_IDX) == FR_POCSTATE_NORMAL_ACTIVE) | (Fr_VCCGetPocState(FR_VCTRL_IDX) == FR_POCSTATE_NORMAL_PASSIVE);

  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check plausibility of input parameter 'Fr_CtrlIdx'. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check if controller is not in POCstates NORMAL_ACTIVE or NORMAL_PASSIVE. */
  else if (!pocState)
  {
    errorId = FR_E_INV_POCSTATE;
  }
  /* #40 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Trigger transition to POCstate ALL_SLOTS by executing POC command CMD_ALL_SLOTS. */
    retVal = Fr_ExecutePOCCommand(FR_VCTRL_IDX, CCCMD_ALL_SLOTS); /* SWS_Fr_00518 */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_ALLSLOTS, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* Fr_AllSlots() */

#if (FR_CFG_APPL_CALLBACK_CYCLE_START == STD_ON) /* COV_FR_TESTSUITE_INSTRUMENTATION */
/***********************************************************************************************************************
 *  Fr_ISR_Status
 **********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, FR_CODE) Fr_ISR_Status
(
  FR_VCTRL_SYSTEMTYPE_ONLY
)
{ /* PRQA S 3007 */ /* MD_Fr_3007 */
  /* ----- Implementation ----------------------------------------------- */

  Fr_Rtm_Start(Fr_ISR_Status);

  /* #10 Call the ApplFr_ISR_CycleStart callback */
  if (FR_VCTRL_IDX == 0)
  {
	  ApplFr_ISR_CycleStart();
  }
# if (FR_NUM_CTRL_USED > 1)
  else
  {
	  ApplFr_ISR_CycleStart_1();
  }
# endif
  Fr_Rtm_Stop(Fr_ISR_Status);

} /* Fr_ISR_Status() */
#endif /*(FR_CFG_APPL_CALLBACK_CYCLE_START == STD_ON) */

/***********************************************************************************************************************
 *  Fr_SetAbsoluteTimer
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
 */
FUNC(Std_ReturnType, FR_CODE) Fr_SetAbsoluteTimer
(
  FR_VCTRL_SYSTEMTYPE_ONLY, 
  uint8 Fr_AbsTimerIdx, 
  uint8 Fr_Cycle, 
  uint16 Fr_Offset
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  uint32 pocState = (Fr_VCCGetPocState(FR_VCTRL_IDX) == FR_POCSTATE_NORMAL_ACTIVE) | (Fr_VCCGetPocState(FR_VCTRL_IDX) == FR_POCSTATE_NORMAL_PASSIVE);

  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check that Fr_AbsTimerIdx is less than or equal to FR_MAX_NUM_ABS_TIMER. */
  else if (Fr_AbsTimerIdx > FR_MAX_NUM_ABS_TIMER)
  {
    errorId = FR_E_INV_TIMER_IDX;
  }
  /* #40 If required, check that Fr_Cycle is less than or equal to FR_VMAX_NUM_CYCLE. */
  else if (Fr_Cycle > FR_VMAX_NUM_CYCLE)
  {
    errorId = FR_E_INV_CYCLE;
  }
  /* #50 If required, check that Fr_Offset is less than or equal to the value gMacroPerCycle. */
  else if (Fr_Offset > 16000 /* maximum macroticks per cycle */)
  {
    errorId = FR_E_INV_OFFSET;
  }
  /* #60 If required, check that communication controller is either in POCstate NORMAL_ACTIVE or in NORMAL_PASSIVE. */
  else if (!pocState)
  {
    errorId = FR_E_INV_POCSTATE;
  }
  /* #70 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (FR_VTT == STD_OFF) /* COV_FR_UNSUPPORTED_SAFEBSW_FEATURE */
    /* #80 Configure the absolute timer according the given Fr_Cycle and Fr_Offset */
    __int64 currentTime;
    __int64 timeoutTime;

    if (((Fr_Offset <= Fr_VGetCurrentMacroTick(FR_VCTRL_IDX)) & (Fr_Cycle == gCurrentCycle[FR_VCTRL_IDX])) \
       | (Fr_Cycle < gCurrentCycle[FR_VCTRL_IDX]))
    {
      Fr_Cycle += 64;
    }
    currentTime = (__int64)((gCurrentCycle[FR_VCTRL_IDX] * gMacroticksPerCycle[FR_VCTRL_IDX]) + \
                                                                                Fr_VGetCurrentMacroTick(FR_VCTRL_IDX));
    timeoutTime = (__int64)((Fr_Cycle * gMacroticksPerCycle[FR_VCTRL_IDX]) + Fr_Offset);
    gTimeoutTimes[FR_VCTRL_IDX][Fr_AbsTimerIdx] = \
      (__int64)(((timeoutTime - currentTime) * gNanoSecondsPerMacrotick[FR_VCTRL_IDX]) + CANoeAPI_GetCurrentSimTime());
#else
    VttFr_AbsoluteTimer( FR_VCTRL_IDX,
                         (Fr_Offset * gNanoSecondsPerMacrotick[FR_VCTRL_IDX]),
                         Fr_Cycle,
                         gMacroticksPerCycle[FR_VCTRL_IDX],
                         gNanoSecondsPerMacrotick[FR_VCTRL_IDX]
                        );
#endif /* #if (FR_VTT == STD_OFF) */
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_SET_ABSOLUTE_TIMER, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
  /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
} /* Fr_SetAbsoluteTimer() */ 

/***********************************************************************************************************************
 *  Fr_CancelAbsoluteTimer
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
FUNC(Std_ReturnType, FR_CODE) Fr_CancelAbsoluteTimer
(
  FR_VCTRL_SYSTEMTYPE_ONLY, 
  uint8 Fr_AbsTimerIdx
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check that Fr_AbsTimerIdx is less than or equal to FR_MAX_NUM_ABS_TIMER. */
  else if (Fr_AbsTimerIdx > FR_MAX_NUM_ABS_TIMER)
  {
    errorId = FR_E_INV_TIMER_IDX;
  }
  /* #40 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Cancel the absolute timer by setting the CANoe timer to -1 */
    gTimeoutTimes[FR_VCTRL_IDX][Fr_AbsTimerIdx] = -1;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_CANCEL_ABSOLUTE_TIMER, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* Fr_CancelAbsoluteTimer() */

/***********************************************************************************************************************
 *  Fr_EnableAbsoluteTimerIRQ
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

FUNC(Std_ReturnType, FR_CODE) Fr_EnableAbsoluteTimerIRQ
(
  FR_VCTRL_SYSTEMTYPE_ONLY, 
  uint8 Fr_AbsTimerIdx
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
# if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
# endif
  /* #30 If required, check that Fr_AbsTimerIdx is less than or equal to FR_MAX_NUM_ABS_TIMER. */
  else if (Fr_AbsTimerIdx > FR_MAX_NUM_ABS_TIMER)
  {
    errorId = FR_E_INV_TIMER_IDX;
  }
  /* #40 If the initial checks are passed or not required: */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Enable the absolute timer interrupt */
    gTimerEnabled[FR_VCTRL_IDX][Fr_AbsTimerIdx] = TRUE;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_ENABLE_ABSOLUTE_TIMER_IRQ, errorId);
  }
#else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
#endif

  return retVal;
} /* Fr_EnableAbsoluteTimerIRQ() */

#if (FR_ABSOLUTE_TIMER_DISABLE == STD_OFF)
/***********************************************************************************************************************
 *  Fr_AckAbsoluteTimerIRQ
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
FUNC(Std_ReturnType, FR_CODE) Fr_AckAbsoluteTimerIRQ
(
  FR_VCTRL_SYSTEMTYPE_ONLY, 
  uint8 Fr_AbsTimerIdx
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
#  if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
#  endif
  /* #30 If required, check that Fr_AbsTimerIdx is less than or equal to FR_MAX_NUM_ABS_TIMER. */
  else if (Fr_AbsTimerIdx > FR_MAX_NUM_ABS_TIMER)
  {
    errorId = FR_E_INV_TIMER_IDX;
  }
  /* #40 If the initial checks are passed or not required: */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Clear the absolute timer ISR flag by setting the CANoe timer to -1 */
    gTimeoutTimes[FR_VCTRL_IDX][Fr_AbsTimerIdx] = -1;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_ACK_ABSOLUTE_TIMER_IRQ, errorId);
  }
# else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* Fr_AckAbsoluteTimerIRQ() */

/***********************************************************************************************************************
 *  Fr_DisableAbsoluteTimerIRQ
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
FUNC(Std_ReturnType, FR_CODE) Fr_DisableAbsoluteTimerIRQ
(
  FR_VCTRL_SYSTEMTYPE_ONLY, 
  uint8 Fr_AbsTimerIdx
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
#  if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
#  endif
  /* #30 If required, check that Fr_AbsTimerIdx is less than or equal to FR_MAX_NUM_ABS_TIMER. */
  else if (Fr_AbsTimerIdx > FR_MAX_NUM_ABS_TIMER)
  {
    errorId = FR_E_INV_TIMER_IDX;
  }
  /* #40 If the initial checks are passed or not required: */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Disable the absolute timer interrupt */
    gTimerEnabled[FR_VCTRL_IDX][Fr_AbsTimerIdx] = FALSE;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_DISABLE_ABSOLUTE_TIMER_IRQ, errorId);
  }
# else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* Fr_DisableAbsoluteTimerIRQ() */

/***********************************************************************************************************************
 *  Fr_GetAbsoluteTimerIRQStatus
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
FUNC(Std_ReturnType, FR_CODE) Fr_GetAbsoluteTimerIRQStatus
(
  FR_VCTRL_SYSTEMTYPE_ONLY, 
  uint8 Fr_AbsTimerIdx,
  P2VAR(boolean, AUTOMATIC, FR_APPL_DATA) Fr_IRQStatusPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FR_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (FR_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check initialization state of the component. */
  if (Fr_VFrDrvState != FR_VINIT)
  {
    errorId = FR_E_NOT_INITIALIZED;
  }
#  if (FR_CTRL_ENABLE_API_OPTIMIZATION == STD_OFF)
  /* #20 If required, check number of controllers. */
  else if (FR_VCTRL_IDX >= FR_NUM_CTRL_SUPPORTED)
  {
    errorId = FR_E_INV_CTRL_IDX;
  }
#  endif
  /* #30 If required, check that Fr_AbsTimerIdx is less than or equal to FR_MAX_NUM_ABS_TIMER. */
  else if (Fr_AbsTimerIdx > FR_MAX_NUM_ABS_TIMER)
  {
    errorId = FR_E_INV_TIMER_IDX;
  }
  else if (Fr_IRQStatusPtr == NULL_PTR)
  {
    errorId = FR_E_INV_POINTER;
  }
  /* #40 If the initial checks are passed or not required: */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Do nothing because API service is currently not supported by CANoeEmu driver */
    *Fr_IRQStatusPtr = FALSE;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (FR_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FR_E_NO_ERROR)
  {
    (void)Det_ReportError(FR_MODULE_ID, FR_INSTANCE_ID_DET, FR_SID_GET_ABSOLUTE_TIMER_IRQ_STATUS, errorId);
  }
# else
  FR_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /*lint -e{438} */
# endif

  return retVal;
} /* Fr_GetAbsoluteTimerIRQStatus() */
#endif /* (FR_ABSOLUTE_TIMER_DISABLE == STD_OFF) */

/* module specific MISRA deviations: 
 MD_Fr_0612:      Rule 1.1
      Reason:     Array size is sufficient on CANoeEmu platforms (no C99 compliancy necessary)
      Risk:       None
      Prevention: None
 MD_Fr_5013:      Rule 6.3
      Reason:     API use explicit generic type and cast is needed here.
      Risk:       None
      Prevention: None
 MD_Fr_3408:      Rule 8.8
      Reason:     The function or symbol must be declared as extern by the application.
      Risk:       No risk
      Prevention: Covered by code review.
 MD_Fr_8.12:    Rule 8.12
      Reason:     The arrays in file Fr.h, Fr_PBcfg.h and Fr_Lcfg.h are declared as extern without size specification since their size depend on the configuration in GENy.
      Risk:       There is no risk because these arrays are only used by Fr.
      Prevention: Covered by code review.
 MD_Fr_3747:      Rule 10.1
      Reason:     Conversion to a wider integer type of the same signedness is uncritical.
      Risk:       None
      Prevention: Review
 MD_Fr_0311:      MISRA Rule 11.5
      Reason:     The implementation has considered that the cast is uncritical.
      Risk:       None
      Prevention: Review
 MD_Fr_4130:      Rule 12.7
      Reason:     The implementation has considered that Bitwise operations on signed data will give implementation defined results. 
      Risk:       None
      Prevention: Review
 MD_Fr_13.7:      Rule 13.7
      Reason:     If NM is enabled but NMVectorlength is set to 0 the condition is always false.
      Risk:       There is no risk because the for-loop will be skipped completely.
      Prevention: Covered by code review.
 MD_Fr_3007:      Rule 16.5
      Reason:     Due to the Single Channel API optimization void has been omitted when defining a function with no parameters.
      Risk:       There is no risk as such statements have no effect on the code.
      Prevention: Covered by code review. 
 MD_Fr_4102:      Rule 12.6
      Reason:     Deliberate violation of rule: confusing logical operators with bitwise operators is prevented by code review.
      Risk:       There is no risk.
      Prevention: Covered by code review.
 MD_Fr_0342:      Rule: 19.13: K&R compilers do not support the ISO glue operator ##.
     Reason:      Identifier has to be a combination of both IDs
     Risk:        Compile error.
     Prevention:  Not required.
*/

/* COV_JUSTIFICATION_BEGIN

\ID COV_FR_MSR_COMPATIBILITY
\ACCEPT TX 
\ACCEPT XF 
\REASON [COV_MSR_COMPATIBILITY]

\ID COV_FR_UNSUPPORTED_SAFEBSW_FEATURE
\ACCEPT TX 
\ACCEPT TF tx tf 
\ACCEPT XF xf xx
\ACCEPT XF 
\REASON [COV_MSR_UNSUPPORTED] The following features are not supported by FR in SafeBSW usecase and the correpsonding preprocessor switches must be configured to STD_OFF (which is ensured by MSSV plugin): FR_VTT

\ID COV_FR_TESTSUITE_INSTRUMENTATION
\ACCEPT XF 
\ACCEPT TX 
\ACCEPT XX 
\REASON Not all branches covered because the following preprocessor macros and defines are necessary for proper test execution with FrTestSuite: FR_CFG_APPL_CALLBACK_CYCLE_START

\ID COV_FR_SOURCE
\ACCEPT XF 
\REASON Only False branch is covered because FR_SOURCE is only defined in Fr.c and the macros shall not be defined in the C-file. Otherwise the single channel API optimization does not work properly and a compiler warning is thrown.

COV_JUSTIFICATION_END */ 
