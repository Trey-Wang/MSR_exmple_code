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
 *         File:  J1939Tp.c
 *       Module:  J1939 Transport Layer
 *
 *  Description:  Implementation of the SAE J1939 Transport Layer module.
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

/* PRQA S 0779 EOF */ /* MD_MSR_5.1_777 */

/* *INDENT-ON* */


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#define J1939TP_C

#include "J1939Tp.h"
#include "J1939Tp_Cbk.h"

#include "J1939Tp_Int.h"
#include "J1939Tp_Direct.h"
#include "J1939Tp_Bam.h"
#include "J1939Tp_Cmdt.h"
#if (J1939TP_ETP_ENABLED == STD_ON)
# include "J1939Tp_Etp.h"
#endif
#if (J1939TP_FPP_ENABLED == STD_ON)
# include "J1939Tp_Fpp.h"
#endif

/*! \trace SPEC-2167286 */
#include "PduR_J1939Tp.h"
/*! \trace SPEC-2167287 */
#include "CanIf.h"

#include "SchM_J1939Tp.h"

#if (J1939TP_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
# include "EcuM_Error.h"
#endif


/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Vendor specific BSW module version information */
#define J1939TP_MAJOR_SOURCE_VERSION            (1u)
#define J1939TP_MINOR_SOURCE_VERSION            (3u)
#define J1939TP_PATCH_SOURCE_VERSION            (2u)

/* Vendor specific BSW generator version information */
#define J1939TP_MAJOR_GENERATOR_VERSION         (1u)
#define J1939TP_MINOR_GENERATOR_VERSION         (3u)

/* Check the vendor specific version of J1939Tp module header file */
#if ((J1939TP_SW_MAJOR_VERSION != J1939TP_MAJOR_SOURCE_VERSION) || (J1939TP_SW_MINOR_VERSION != J1939TP_MINOR_SOURCE_VERSION) || (J1939TP_SW_PATCH_VERSION != J1939TP_PATCH_SOURCE_VERSION))
# error "Vendor specific version numbers of J1939Tp.c and J1939Tp.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if ((J1939TP_CFG_MAJOR_VERSION != J1939TP_MAJOR_GENERATOR_VERSION) || (J1939TP_CFG_MINOR_VERSION != J1939TP_MINOR_GENERATOR_VERSION))
# error "Vendor specific version numbers of J1939Tp.c and J1939Tp_Cfg.h are inconsistent!"
#endif


/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/*!< Global module state \trace SPEC-2167294 */
#define J1939TP_UNINIT                          (0)
#define J1939TP_INIT                            (1)


/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/*======================================= J1939TP_START_SEC_VAR_ZERO_INIT_8BIT ======================================*/
#define J1939TP_START_SEC_VAR_ZERO_INIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

/*! \brief Initialization state of the J1399Tp */
static VAR(uint8, J1939TP_VAR_ZERO_INIT) J1939Tp_ModuleInitialized = J1939TP_UNINIT;

#define J1939TP_STOP_SEC_VAR_ZERO_INIT_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*======================================= J1939TP_STOP_SEC_VAR_ZERO_INIT_8BIT =======================================*/


/**********************************************************************************************************************
 *  INTERNAL DATA
 *********************************************************************************************************************/

/*=================================== J1939TP_START_SEC_VAR_ZERO_INIT_UNSPECIFIED ===================================*/
#define J1939TP_START_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

/*! \brief Post-build configuration data of the J1399Tp */
#if (J1939TP_USE_INIT_POINTER == STD_ON)
P2CONST(J1939Tp_ConfigType, J1939TP_VAR_ZERO_INIT, J1939TP_PBCFG) J1939Tp_ConfigDataPtr;
#endif

#define J1939TP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*=================================== J1939TP_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED ====================================*/


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*============================================== J1939TP_START_SEC_CODE =============================================*/
#define J1939TP_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

#if (J1939TP_FPP_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_TxPduFpp(PduIdType sduIdx);
#endif

#if ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_ENABLED == STD_ON))
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_TxPduTpCm(PduIdType sduIdx, J1939Tp_NSduType sduType);
#endif

#if ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_TX_ENABLED == STD_ON))
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_TxPduTpDt(PduIdType sduIdx);
#endif

#if (J1939TP_ETP_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_TxPduEtpCm(PduIdType sduIdx, J1939Tp_NSduType sduType);
#endif

#if (J1939TP_ETP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_TxPduEtpDt(PduIdType sduIdx);
#endif

#if (J1939TP_DIRECT_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_TxPduDirect(PduIdType sduIdx);
#endif

#if (J1939TP_TXPDU == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_TxPdu(void);
#endif

#if (J1939TP_TXSDU == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_TxSdu(void);
#endif

#if (J1939TP_RXSDU == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_RxSdu(void);
#endif

#if (J1939TP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE Std_ReturnType J1939Tp_InternalTransmit(PduIdType txSduId,
                                                             P2CONST(PduInfoType, AUTOMATIC,
                                                                     J1939TP_APPL_DATA) pduInfoPtr);
#endif

#if (J1939TP_RXPDU == STD_ON)
/*! \trace SPEC-2167307 */
J1939TP_LOCAL_INLINE void J1939Tp_InternalRxIndication(PduIdType RxPduId,
                                                       P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) PduInfoPtr);
#endif

#if (J1939TP_TXPDU == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_InternalTxConfirmation(PduIdType TxPduId, PduIdType sduIdx, J1939Tp_NSduType sduType);
#endif

#define J1939TP_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*============================================== J1939TP_STOP_SEC_CODE ==============================================*/


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/*============================================== J1939TP_START_SEC_CODE =============================================*/
#define J1939TP_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

#if (J1939TP_FPP_TX_ENABLED == STD_ON)
/*! \trace SPEC-2167301 */
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_TxPduFpp(PduIdType sduIdx)
{
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if ((txSduState->State == J1939TP_STATE_FPP_FF_CONF) || (txSduState->State == J1939TP_STATE_FPP_AF_CONF))
  {
    /* protected against late Tx confirmation */
    txSduState->State = J1939TP_STATE_FINISH;
    SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /*! \trace SPEC-2167303 */
    PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_NOT_OK);

    /* no protection required - transition from uninterruptible state */
    txSduState->State = J1939TP_STATE_IDLE;
  }
  else
  {
    SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
}
#endif


#if ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_ENABLED == STD_ON))
/*! \trace SPEC-2167301 */
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_TxPduTpCm(PduIdType sduIdx, J1939Tp_NSduType sduType)
{
# if ((J1939TP_CMDT_RX_ENABLED == STD_ON) && ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_TX_ENABLED == STD_ON)))
  if (sduType == J1939TP_NSDU_CMDT_RX)
# endif
# if (J1939TP_CMDT_RX_ENABLED == STD_ON)
  {
    J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(sduIdx));

    SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if ((rxSduState->State == J1939TP_STATE_CMDT_CTS_CONF) || (rxSduState->State == J1939TP_STATE_CMDT_CTSWAIT_CONF) ||
        (rxSduState->State == J1939TP_STATE_CMDT_EOMACK_CONF) || (rxSduState->State == J1939TP_STATE_CMDT_ABORT_CONF))
    {
      /* protected against late Tx confirmation */
      rxSduState->State = J1939TP_STATE_FINISH;
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      if (rxSduState->InSession) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
      {
        /*! \trace SPEC-2167305 */
        PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);
      }

      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_IDLE;
    }
    else
    {
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
# endif
# if ((J1939TP_CMDT_RX_ENABLED == STD_ON) && ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_TX_ENABLED == STD_ON)))
  else /* (sduType == J1939TP_NSDU_CMDT_TX) || (sduType == J1939TP_NSDU_BAM) */
# endif
# if ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_TX_ENABLED == STD_ON))
  {
    J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

    SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if ((txSduState->State == J1939TP_STATE_BAM_BAM_CONF) || (txSduState->State == J1939TP_STATE_CMDT_RTS_CONF) ||
        (txSduState->State == J1939TP_STATE_CMDT_ABORT_CONF))
    {
      /* protected against late Tx confirmation */
      txSduState->State = J1939TP_STATE_FINISH;
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /*! \trace SPEC-2167303 */
      PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_NOT_OK);

      /* no protection required - transition from uninterruptible state */
      txSduState->State = J1939TP_STATE_IDLE;
    }
    else
    {
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
# endif

  J1939TP_DUMMY_STATEMENT(sduType); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
}
#endif


#if ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_TX_ENABLED == STD_ON))
/*! \trace SPEC-2167301 */
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_TxPduTpDt(PduIdType sduIdx)
{
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if ((txSduState->State == J1939TP_STATE_BAM_DATA_CONF) || (txSduState->State == J1939TP_STATE_CMDT_DATA_CONF))
  {
    /* protected against late Tx confirmation */
    txSduState->State = J1939TP_STATE_FINISH;
    SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /*! \trace SPEC-2167303 */
    PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_NOT_OK);

    /* no protection required - transition from uninterruptible state */
    txSduState->State = J1939TP_STATE_IDLE;
  }
  else
  {
    SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
}
#endif


#if (J1939TP_ETP_ENABLED == STD_ON)
/*! \trace SPEC-2167301 */
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_TxPduEtpCm(PduIdType sduIdx, J1939Tp_NSduType sduType)
{
# if ((J1939TP_ETP_RX_ENABLED == STD_ON) && (J1939TP_ETP_TX_ENABLED == STD_ON))
  if (sduType == J1939TP_NSDU_ETP_RX)
# endif
# if (J1939TP_ETP_RX_ENABLED == STD_ON)
  {
    J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(sduIdx));

    SchM_Enter_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if ((rxSduState->State == J1939TP_STATE_ETP_CTS_CONF) ||
        (rxSduState->State == J1939TP_STATE_ETP_CTSWAIT_CONF) ||
        (rxSduState->State == J1939TP_STATE_ETP_EOMACK_CONF) || (rxSduState->State == J1939TP_STATE_ETP_ABORT_CONF))
    {
      /* protected against late Tx confirmation */
      rxSduState->State = J1939TP_STATE_FINISH;
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      if (rxSduState->InSession) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
      {
        /*! \trace SPEC-2167305 */
        PduR_J1939TpRxIndication(J1939Tp_GetSduIdOfRxSdu(sduIdx), E_NOT_OK);
      }

      /* no protection required - transition from uninterruptible state */
      rxSduState->State = J1939TP_STATE_IDLE;
    }
    else
    {
      SchM_Exit_J1939Tp_RxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
# endif
# if ((J1939TP_ETP_RX_ENABLED == STD_ON) && (J1939TP_ETP_TX_ENABLED == STD_ON))
  else /* sduType == J1939TP_NSDU_ETP_TX */
# endif
# if (J1939TP_ETP_TX_ENABLED == STD_ON)
  {
    J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

    SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if ((txSduState->State == J1939TP_STATE_ETP_RTS_CONF) ||
        (txSduState->State == J1939TP_STATE_ETP_DPO_CONF) || (txSduState->State == J1939TP_STATE_ETP_ABORT_CONF))
    {
      /* protected against late Tx confirmation */
      txSduState->State = J1939TP_STATE_FINISH;
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      /*! \trace SPEC-2167303 */
      PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_NOT_OK);

      /* no protection required - transition from uninterruptible state */
      txSduState->State = J1939TP_STATE_IDLE;
    }
    else
    {
      SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
# endif

  J1939TP_DUMMY_STATEMENT(sduType); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
}
#endif


#if (J1939TP_ETP_TX_ENABLED == STD_ON)
/*! \trace SPEC-2167301 */
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_TxPduEtpDt(PduIdType sduIdx)
{
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (txSduState->State == J1939TP_STATE_ETP_DATA_CONF)
  {
    /* protected against late Tx confirmation */
    txSduState->State = J1939TP_STATE_FINISH;
    SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /*! \trace SPEC-2167303 */
    PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_NOT_OK);

    /* no protection required - transition from uninterruptible state */
    txSduState->State = J1939TP_STATE_IDLE;
  }
  else
  {
    SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
}
#endif


#if (J1939TP_DIRECT_TX_ENABLED == STD_ON)
/*! \trace SPEC-2167301 */
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_TxPduDirect(PduIdType sduIdx)
{
  J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

  SchM_Enter_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (txSduState->State == J1939TP_STATE_DIRECT_CONF)
  {
    /* protected against late Tx confirmation */
    txSduState->State = J1939TP_STATE_FINISH;
    SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /*! \trace SPEC-2167303 */
    PduR_J1939TpTxConfirmation(J1939Tp_GetSduIdOfTxSdu(sduIdx), E_NOT_OK);

    /* no protection required - transition from uninterruptible state */
    txSduState->State = J1939TP_STATE_IDLE;
  }
  else
  {
    SchM_Exit_J1939Tp_TxNSduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
}
#endif


#if (J1939TP_TXPDU == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_TxPdu(void)
{
  J1939Tp_SizeOfTxPduType txPduIdx;

  for (txPduIdx = 0; txPduIdx < J1939Tp_GetSizeOfTxPdu(); txPduIdx++)
  {
# if (J1939TP_INVALIDHNDOFTXPDU == STD_ON)
    if (!J1939Tp_IsInvalidHndOfTxPdu(txPduIdx))
# endif
    {
      J1939Tp_TxNPduStateType *pduState = &J1939Tp_GetTxPduState(J1939Tp_GetTxPduStateIdxOfTxPdu(txPduIdx));

      /* check for Tx confirmation timeout */
      SchM_Enter_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      if (pduState->Locked) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
      {
        --pduState->ConfTimeout;

        if (pduState->ConfTimeout == 0)
        {
          PduIdType sduIdx = pduState->CurrentNSduId;
# if ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_ENABLED == STD_ON) || (J1939TP_ETP_ENABLED == STD_ON))
          J1939Tp_NSduType sduType = pduState->CurrentNSduType;
# endif

          /* protected against concurrent Tx confirmation */
          pduState->Locked = FALSE;
          SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

          J1939Tp_ReportRuntimeError(J1939TP_SID_MAINFUNCTION, J1939TP_E_TIMEOUT_TXCONF);

          switch (J1939Tp_GetPduTypeOfTxPdu(txPduIdx))
          {
# if (J1939TP_FPP_TX_ENABLED == STD_ON)
          case J1939TP_FPP_PDUTYPEOFTXPDU:
            J1939Tp_InternalMain_TxPduFpp(sduIdx);
            break;
# endif

# if ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_ENABLED == STD_ON))
          case J1939TP_TPCM_PDUTYPEOFTXPDU:
            J1939Tp_InternalMain_TxPduTpCm(sduIdx, sduType);
            break;
# endif

# if ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_TX_ENABLED == STD_ON))
          case J1939TP_TPDT_PDUTYPEOFTXPDU:
            J1939Tp_InternalMain_TxPduTpDt(sduIdx);
            break;
# endif

# if (J1939TP_ETP_ENABLED == STD_ON)
          case J1939TP_ETPCM_PDUTYPEOFTXPDU:
            J1939Tp_InternalMain_TxPduEtpCm(sduIdx, sduType);
            break;
# endif

# if (J1939TP_ETP_TX_ENABLED == STD_ON)
          case J1939TP_ETPDT_PDUTYPEOFTXPDU:
            J1939Tp_InternalMain_TxPduEtpDt(sduIdx);
            break;
# endif

# if (J1939TP_DIRECT_TX_ENABLED == STD_ON)
          case J1939TP_DIRECT_PDUTYPEOFTXPDU:
            J1939Tp_InternalMain_TxPduDirect(sduIdx);
            break;
# endif

          default: /* COV_J1939TP_SWITCHCASE_DEFAULT */
            break; /* MISRA */
          }
        }
        else
        {
          SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
        }
      }
      else
      {
        SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
    }
  }
} /* J1939Tp_InternalMain_TxPdu */ /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif


#if (J1939TP_TXSDU == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_TxSdu(void)
{
  J1939Tp_SizeOfTxSduType txSduIdx;

  for (txSduIdx = 0; txSduIdx < J1939Tp_GetSizeOfTxSdu(); txSduIdx++)
  {
# if (J1939TP_INVALIDHNDOFTXSDU == STD_ON)
    if (!J1939Tp_IsInvalidHndOfTxSdu(txSduIdx))
# endif
    {
      J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(txSduIdx));

# if (J1939TP_FPP_TX_ENABLED == STD_ON)
      if (((txSduState->State & J1939TP_STATEMASK_FPP) != J1939TP_STATE_IDLE) &&
          ((txSduState->State & J1939TP_STATEMASK_CONF) == J1939TP_STATE_IDLE))
      {
        J1939Tp_FppTxMain(txSduIdx);
      }
# endif
# if ((J1939TP_FPP_TX_ENABLED == STD_ON) && ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_TX_ENABLED == STD_ON) || (J1939TP_ETP_TX_ENABLED == STD_ON) || (J1939TP_DIRECT_TX_ENABLED == STD_ON)))
      else
# endif
# if ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_TX_ENABLED == STD_ON) || (J1939TP_ETP_TX_ENABLED == STD_ON) || (J1939TP_DIRECT_TX_ENABLED == STD_ON))
      {
# endif
# if (J1939TP_BAM_TX_ENABLED == STD_ON)
        if (((txSduState->State & J1939TP_STATEMASK_BAM) != J1939TP_STATE_IDLE) &&
            ((txSduState->State & J1939TP_STATEMASK_CONF) == J1939TP_STATE_IDLE))
        {
          J1939Tp_BamTxMain(txSduIdx);
        }
# endif
# if ((J1939TP_BAM_TX_ENABLED == STD_ON) && ((J1939TP_CMDT_TX_ENABLED == STD_ON) || (J1939TP_ETP_TX_ENABLED == STD_ON) || (J1939TP_DIRECT_TX_ENABLED == STD_ON)))
        else
# endif
# if ((J1939TP_CMDT_TX_ENABLED == STD_ON) || (J1939TP_ETP_TX_ENABLED == STD_ON) || (J1939TP_DIRECT_TX_ENABLED == STD_ON))
        {
# endif
# if (J1939TP_CMDT_TX_ENABLED == STD_ON)
          if (((txSduState->State & J1939TP_STATEMASK_CMDT) != J1939TP_STATE_IDLE) &&
              ((txSduState->State & J1939TP_STATEMASK_CONF) == J1939TP_STATE_IDLE))
          {
            J1939Tp_CmdtTxMain(txSduIdx);
          }
# endif
# if ((J1939TP_CMDT_TX_ENABLED == STD_ON) && ((J1939TP_ETP_TX_ENABLED == STD_ON) || (J1939TP_DIRECT_TX_ENABLED == STD_ON)))
          else
# endif
# if ((J1939TP_ETP_TX_ENABLED == STD_ON) || (J1939TP_DIRECT_TX_ENABLED == STD_ON))
          {
# endif
# if (J1939TP_ETP_TX_ENABLED == STD_ON)
            if (((txSduState->State & J1939TP_STATEMASK_ETP) != J1939TP_STATE_IDLE) &&
                ((txSduState->State & J1939TP_STATEMASK_CONF) == J1939TP_STATE_IDLE))
            {
              J1939Tp_EtpTxMain(txSduIdx);
            }
# endif
# if ((J1939TP_ETP_TX_ENABLED == STD_ON) && (J1939TP_DIRECT_TX_ENABLED == STD_ON))
            else
# endif
# if (J1939TP_DIRECT_TX_ENABLED == STD_ON)
            {
              if (((txSduState->State & J1939TP_STATEMASK_DIRECT) != J1939TP_STATE_IDLE) &&
                  ((txSduState->State & J1939TP_STATEMASK_CONF) == J1939TP_STATE_IDLE))
              {
                J1939Tp_DirectTxMain(txSduIdx);
              }
            }
# endif
# if ((J1939TP_ETP_TX_ENABLED == STD_ON) || (J1939TP_DIRECT_TX_ENABLED == STD_ON))
          }
# endif
# if ((J1939TP_CMDT_TX_ENABLED == STD_ON) || (J1939TP_ETP_TX_ENABLED == STD_ON) || (J1939TP_DIRECT_TX_ENABLED == STD_ON))
        }
# endif
# if ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_TX_ENABLED == STD_ON) || (J1939TP_ETP_TX_ENABLED == STD_ON) || (J1939TP_DIRECT_TX_ENABLED == STD_ON))
      }
# endif
    }
  }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif


#if (J1939TP_RXSDU == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_InternalMain_RxSdu(void)
{
  J1939Tp_SizeOfRxSduType rxSduIdx;

  for (rxSduIdx = 0; rxSduIdx < J1939Tp_GetSizeOfRxSdu(); rxSduIdx++)
  {
# if (J1939TP_INVALIDHNDOFRXSDU == STD_ON)
    if (!J1939Tp_IsInvalidHndOfRxSdu(rxSduIdx))
# endif
    {
      J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(rxSduIdx));

# if (J1939TP_FPP_RX_ENABLED == STD_ON)
      if ((rxSduState->State & J1939TP_STATEMASK_FPP) != J1939TP_STATE_IDLE)
      {
        J1939Tp_FppRxMain(rxSduIdx);
      }
# endif
# if ((J1939TP_FPP_RX_ENABLED == STD_ON) && ((J1939TP_BAM_RX_ENABLED == STD_ON) || (J1939TP_CMDT_RX_ENABLED == STD_ON) || (J1939TP_ETP_RX_ENABLED == STD_ON)))
      else
# endif
# if ((J1939TP_BAM_RX_ENABLED == STD_ON) || (J1939TP_CMDT_RX_ENABLED == STD_ON) || (J1939TP_ETP_RX_ENABLED == STD_ON))
      {
# endif
# if (J1939TP_BAM_RX_ENABLED == STD_ON)
        if ((rxSduState->State & J1939TP_STATEMASK_BAM) != J1939TP_STATE_IDLE)
        {
          J1939Tp_BamRxMain(rxSduIdx);
        }
# endif
# if ((J1939TP_BAM_RX_ENABLED == STD_ON) && ((J1939TP_CMDT_RX_ENABLED == STD_ON) || (J1939TP_ETP_RX_ENABLED == STD_ON)))
        else
# endif
# if ((J1939TP_CMDT_RX_ENABLED == STD_ON) || (J1939TP_ETP_RX_ENABLED == STD_ON))
        {
# endif
# if (J1939TP_CMDT_RX_ENABLED == STD_ON)
          if (((rxSduState->State & J1939TP_STATEMASK_CMDT) != J1939TP_STATE_IDLE) &&
              ((rxSduState->State & J1939TP_STATEMASK_CONF) == J1939TP_STATE_IDLE))
          {
            J1939Tp_CmdtRxMain(rxSduIdx);
          }
# endif
# if ((J1939TP_CMDT_RX_ENABLED == STD_ON) && (J1939TP_ETP_RX_ENABLED == STD_ON))
          else
# endif
# if (J1939TP_ETP_RX_ENABLED == STD_ON)
          {
            if (((rxSduState->State & J1939TP_STATEMASK_ETP) != J1939TP_STATE_IDLE) &&
                ((rxSduState->State & J1939TP_STATEMASK_CONF) == J1939TP_STATE_IDLE))
            {
              J1939Tp_EtpRxMain(rxSduIdx);
            }
          }
# endif
# if ((J1939TP_CMDT_RX_ENABLED == STD_ON) || (J1939TP_ETP_RX_ENABLED == STD_ON))
        }
# endif
# if ((J1939TP_BAM_RX_ENABLED == STD_ON) || (J1939TP_CMDT_RX_ENABLED == STD_ON) || (J1939TP_ETP_RX_ENABLED == STD_ON))
      }
# endif
    }
  }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif


#if (J1939TP_TX_ENABLED == STD_ON)
J1939TP_LOCAL_INLINE Std_ReturnType J1939Tp_InternalTransmit(PduIdType txSduId,
                                                             P2CONST(PduInfoType, AUTOMATIC,
                                                                     J1939TP_APPL_DATA) pduInfoPtr)
{
  J1939Tp_TxChannelChannelIdxOfTxSduType txChannelIdx = J1939Tp_GetTxChannelChannelIdxOfTxSdu(txSduId);
  Std_ReturnType result;

# if ((J1939TP_FPP_TX_ENABLED == STD_ON) && ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_TX_ENABLED == STD_ON) || (J1939TP_ETP_TX_ENABLED == STD_ON)))
  if (J1939Tp_GetTxProtocolTypeOfTxChannel(txChannelIdx) == J1939TP_PROTOCOL_FPP_TXPROTOCOLTYPEOFTXCHANNEL)
# endif
# if (J1939TP_FPP_TX_ENABLED == STD_ON)
  {
    result = J1939Tp_FppTransmit(txSduId, pduInfoPtr);
  }
# endif
# if ((J1939TP_FPP_TX_ENABLED == STD_ON) && ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_TX_ENABLED == STD_ON) || (J1939TP_ETP_TX_ENABLED == STD_ON)))
  else
# endif
# if ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_TX_ENABLED == STD_ON) || (J1939TP_ETP_TX_ENABLED == STD_ON))
  {
#  if (J1939TP_DIRECT_TX_ENABLED == STD_ON)
    if ((!J1939Tp_IsIsVariableOfTxSdu(txSduId)) || (pduInfoPtr->SduLength > 8u))
#  endif
    {
#  if ((J1939TP_BAM_TX_ENABLED == STD_ON) && ((J1939TP_CMDT_TX_ENABLED == STD_ON) || (J1939TP_ETP_TX_ENABLED == STD_ON)))
      /* Check which J1939 transport protocol has to be used */
      if ((J1939Tp_GetTxProtocolTypeOfTxChannel(txChannelIdx) == J1939TP_PROTOCOL_BAM_TXPROTOCOLTYPEOFTXCHANNEL) ||
          ((J1939Tp_GetTxProtocolTypeOfTxChannel(txChannelIdx) == J1939TP_PROTOCOL_UNUSED_TXPROTOCOLTYPEOFTXCHANNEL) &&
           (pduInfoPtr->SduDataPtr[J1939TP_MDL_DA_POS] == J1939TP_BROADCAST_ADDRESS)))
#  endif
#  if (J1939TP_BAM_TX_ENABLED == STD_ON)
      {
        /* sending PG by BAM */
        result = J1939Tp_BamTransmit(txSduId, pduInfoPtr);
      }
#  endif
#  if ((J1939TP_BAM_TX_ENABLED == STD_ON) && ((J1939TP_CMDT_TX_ENABLED == STD_ON) || (J1939TP_ETP_TX_ENABLED == STD_ON)))
      else
#  endif
#  if ((J1939TP_CMDT_TX_ENABLED == STD_ON) || (J1939TP_ETP_TX_ENABLED == STD_ON))
      {
#  endif
#  if ((J1939TP_CMDT_TX_ENABLED == STD_ON) && (J1939TP_ETP_TX_ENABLED == STD_ON))
        if (pduInfoPtr->SduLength <= J1939TP_MAX_LENGTH)
#  endif
#  if (J1939TP_CMDT_TX_ENABLED == STD_ON)
        {
          /* sending PG by CMDT */
          result = J1939Tp_CmdtTransmit(txSduId, pduInfoPtr);
        }
#  endif
#  if ((J1939TP_CMDT_TX_ENABLED == STD_ON) && (J1939TP_ETP_TX_ENABLED == STD_ON))
        else
#  endif
#  if (J1939TP_ETP_TX_ENABLED == STD_ON)
        {
          /* sending PG by ETP */
          result = J1939Tp_EtpTransmit(txSduId, pduInfoPtr);
        }
#  endif
#  if ((J1939TP_CMDT_TX_ENABLED == STD_ON) || (J1939TP_ETP_TX_ENABLED == STD_ON))
      }
#  endif
    }
#  if (J1939TP_DIRECT_TX_ENABLED == STD_ON)
    else
    {
      result = J1939Tp_DirectTransmit(txSduId, pduInfoPtr);
    }
#  endif
  }
# endif

  J1939TP_DUMMY_STATEMENT(txChannelIdx); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */

  return result;
}
#endif


#if (J1939TP_RXPDU == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_InternalRxIndication(PduIdType RxPduId,
                                                       P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) PduInfoPtr)
{
  if (PduInfoPtr->SduLength > J1939TP_MDL_SIZE)
  {
    PduLengthType pduLen = PduInfoPtr->SduLength - J1939TP_MDL_SIZE;
    uint8 sa = PduInfoPtr->SduDataPtr[pduLen + J1939TP_MDL_SA_POS];

    if (!J1939Tp_SA_Valid(sa))
    {
      /*! \trace SPEC-2167310 */
      J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_SA);
    }
    else
    {
      J1939Tp_PduTypeOfRxPduType pduType = J1939Tp_GetPduTypeOfRxPdu(RxPduId);
      switch (pduType)
      {
# if (J1939TP_FPP_RX_ENABLED == STD_ON)
      case J1939TP_FPP_PDUTYPEOFRXPDU:
        J1939Tp_FppRxIndication(RxPduId, PduInfoPtr, sa);
        break;
# endif

# if ((J1939TP_BAM_RX_ENABLED == STD_ON) || (J1939TP_CMDT_ENABLED == STD_ON))
      case J1939TP_TPCM_PDUTYPEOFRXPDU:
      case J1939TP_TPDT_PDUTYPEOFRXPDU:
      {
        uint8 da = PduInfoPtr->SduDataPtr[pduLen + J1939TP_MDL_DA_POS];

#  if ((J1939TP_BAM_RX_ENABLED == STD_ON) && (J1939TP_CMDT_ENABLED == STD_ON))
        if (da == J1939TP_BROADCAST_ADDRESS)
#  endif
#  if (J1939TP_BAM_RX_ENABLED == STD_ON)
        {
          J1939Tp_BamRxIndication(RxPduId, PduInfoPtr, pduType, sa, da);
        }
#  endif
#  if ((J1939TP_BAM_RX_ENABLED == STD_ON) && (J1939TP_CMDT_ENABLED == STD_ON))
        else
#  endif
#  if (J1939TP_CMDT_ENABLED == STD_ON)
        {
          J1939Tp_CmdtRxIndication(RxPduId, PduInfoPtr, pduType, sa, da);
        }
#  endif
      }
        break;
# endif

# if (J1939TP_ETP_ENABLED == STD_ON)
      case J1939TP_ETPCM_PDUTYPEOFRXPDU:
#  if (J1939TP_ETP_RX_ENABLED == STD_ON)
      case J1939TP_ETPDT_PDUTYPEOFRXPDU:
#  endif
        J1939Tp_EtpRxIndication(RxPduId, PduInfoPtr, pduType, sa);
        break;
# endif

# if (J1939TP_DIRECT_RX_ENABLED == STD_ON)
      case J1939TP_DIRECT_PDUTYPEOFRXPDU:
        J1939Tp_DirectRxIndication(RxPduId, PduInfoPtr, sa);
        break;
# endif

      default: /* COV_J1939TP_SWITCHCASE_DEFAULT */
        /*! \trace SPEC-2167354 */
        break; /* MISRA */
      }
    }
  }
# if (J1939TP_RUNTIME_ERROR_REPORT == STD_ON)
  else
  {
    J1939Tp_ReportRuntimeError(J1939TP_SID_RXINDICATION, J1939TP_E_INVALID_DLC);
  }
# endif
}
#endif


#if (J1939TP_TXPDU == STD_ON)
J1939TP_LOCAL_INLINE void J1939Tp_InternalTxConfirmation(PduIdType TxPduId, PduIdType sduIdx, J1939Tp_NSduType sduType)
{
  switch (J1939Tp_GetPduTypeOfTxPdu(TxPduId))
  {
# if (J1939TP_FPP_TX_ENABLED == STD_ON)
  case J1939TP_FPP_PDUTYPEOFTXPDU:
  {
    J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

    if ((txSduState->State & J1939TP_STATEMASK_FPP) != J1939TP_STATE_IDLE)
    {
      J1939Tp_FppTxConfirmation(sduIdx);
    }
  }
    break;
# endif

# if ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_ENABLED == STD_ON))
  case J1939TP_TPCM_PDUTYPEOFTXPDU:
#  if ((J1939TP_BAM_TX_ENABLED == STD_ON) && (J1939TP_CMDT_ENABLED == STD_ON))
    if (sduType == J1939TP_NSDU_BAM)
#  endif
#  if (J1939TP_BAM_TX_ENABLED == STD_ON)
    {
      J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

      if ((txSduState->State & J1939TP_STATEMASK_BAM) != J1939TP_STATE_IDLE)
      {
        J1939Tp_BamTxConfirmation(sduIdx, J1939TP_TPCM_PDUTYPEOFTXPDU);
      }
    }
#  endif
#  if ((J1939TP_BAM_TX_ENABLED == STD_ON) && (J1939TP_CMDT_ENABLED == STD_ON))
    else
#  endif
#  if (J1939TP_CMDT_ENABLED == STD_ON)
    {
#   if ((J1939TP_CMDT_TX_ENABLED == STD_ON) && (J1939TP_CMDT_RX_ENABLED == STD_ON))
      if (sduType == J1939TP_NSDU_CMDT_TX)
#   endif
#   if (J1939TP_CMDT_TX_ENABLED == STD_ON)
      {
        J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

        if ((txSduState->State & J1939TP_STATEMASK_CMDT) != J1939TP_STATE_IDLE)
        {
          J1939Tp_CmdtTxConfirmation(sduIdx, J1939TP_TPCM_PDUTYPEOFTXPDU, sduType);
        }
      }
#   endif
#   if ((J1939TP_CMDT_TX_ENABLED == STD_ON) && (J1939TP_CMDT_RX_ENABLED == STD_ON))
      else
#   endif
#   if (J1939TP_CMDT_RX_ENABLED == STD_ON)
      {
        if (sduType == J1939TP_NSDU_CMDT_RX)
        {
          J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(sduIdx));

          if ((rxSduState->State & J1939TP_STATEMASK_CMDT) != J1939TP_STATE_IDLE)
          {
            J1939Tp_CmdtTxConfirmation(sduIdx, J1939TP_TPCM_PDUTYPEOFTXPDU, sduType);
          }
        }
      }
#   endif
    }
#  endif
    break;

#  if ((J1939TP_BAM_TX_ENABLED == STD_ON) || (J1939TP_CMDT_TX_ENABLED == STD_ON))
  case J1939TP_TPDT_PDUTYPEOFTXPDU:
#   if ((J1939TP_BAM_TX_ENABLED == STD_ON) && (J1939TP_CMDT_TX_ENABLED == STD_ON))
    if (sduType == J1939TP_NSDU_BAM)
#   endif
#   if (J1939TP_BAM_TX_ENABLED == STD_ON)
    {
      J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

      if ((txSduState->State & J1939TP_STATEMASK_BAM) != J1939TP_STATE_IDLE)
      {
        J1939Tp_BamTxConfirmation(sduIdx, J1939TP_TPDT_PDUTYPEOFTXPDU);
      }
    }
#   endif
#   if ((J1939TP_BAM_TX_ENABLED == STD_ON) && (J1939TP_CMDT_TX_ENABLED == STD_ON))
    else
#   endif
#   if (J1939TP_CMDT_TX_ENABLED == STD_ON)
    {
      J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

      if ((txSduState->State & J1939TP_STATEMASK_CMDT) != J1939TP_STATE_IDLE)
      {
        J1939Tp_CmdtTxConfirmation(sduIdx, J1939TP_TPDT_PDUTYPEOFTXPDU, sduType);
      }
    }
#   endif
    break;
#  endif
# endif

# if (J1939TP_ETP_ENABLED == STD_ON)
  case J1939TP_ETPCM_PDUTYPEOFTXPDU:
#  if ((J1939TP_ETP_TX_ENABLED == STD_ON) && (J1939TP_ETP_RX_ENABLED == STD_ON))
    if (sduType == J1939TP_NSDU_ETP_TX)
#  endif
#  if (J1939TP_ETP_TX_ENABLED == STD_ON)
    {
      J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

      if ((txSduState->State & J1939TP_STATEMASK_ETP) != J1939TP_STATE_IDLE)
      {
        J1939Tp_EtpTxConfirmation(sduIdx, J1939TP_ETPCM_PDUTYPEOFTXPDU, sduType);
      }
    }
#  endif
#  if ((J1939TP_ETP_TX_ENABLED == STD_ON) && (J1939TP_ETP_RX_ENABLED == STD_ON))
    else
#  endif
#  if (J1939TP_ETP_RX_ENABLED == STD_ON)
    {
      if (sduType == J1939TP_NSDU_ETP_RX)
      {
        J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(J1939Tp_GetRxSduStateIdxOfRxSdu(sduIdx));

        if ((rxSduState->State & J1939TP_STATEMASK_ETP) != J1939TP_STATE_IDLE)
        {
          J1939Tp_EtpTxConfirmation(sduIdx, J1939TP_ETPCM_PDUTYPEOFTXPDU, sduType);
        }
      }
    }
#  endif
    break;

#  if (J1939TP_ETP_TX_ENABLED == STD_ON)
  case J1939TP_ETPDT_PDUTYPEOFTXPDU:
  {
    J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

    if ((txSduState->State & J1939TP_STATEMASK_ETP) != J1939TP_STATE_IDLE)
    {
      J1939Tp_EtpTxConfirmation(sduIdx, J1939TP_ETPDT_PDUTYPEOFTXPDU, sduType);
    }
  }
    break;
#  endif
# endif

# if (J1939TP_DIRECT_TX_ENABLED == STD_ON)
  case J1939TP_DIRECT_PDUTYPEOFTXPDU:
  {
    J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(J1939Tp_GetTxSduStateIdxOfTxSdu(sduIdx));

    if ((txSduState->State & J1939TP_STATEMASK_DIRECT) != J1939TP_STATE_IDLE)
    {
      J1939Tp_DirectTxConfirmation(sduIdx);
    }
  }
    break;
# endif

  default: /* COV_J1939TP_SWITCHCASE_DEFAULT */
    break; /* MISRA */
  }

  J1939TP_DUMMY_STATEMENT(sduType); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
} /* J1939Tp_InternalTxConfirmation */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif

#define J1939TP_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*============================================== J1939TP_STOP_SEC_CODE ==============================================*/


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/*============================================== J1939TP_START_SEC_CODE =============================================*/
#define J1939TP_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */


/**********************************************************************************************************************
 *  J1939Tp_InitMemory
 *********************************************************************************************************************/
FUNC(void, J1939TP_CODE) J1939Tp_InitMemory(void)
{
  /*! \trace SPEC-2167295 */
  J1939Tp_ModuleInitialized = J1939TP_UNINIT;
}


/**********************************************************************************************************************
 *  J1939Tp_Init
 *********************************************************************************************************************/

FUNC(void, J1939TP_CODE) J1939Tp_Init(P2CONST(J1939Tp_ConfigType, AUTOMATIC, J1939TP_INIT_DATA) config)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = J1939TP_E_NO_ERROR;

#if (J1939TP_USE_INIT_POINTER == STD_ON)
  J1939Tp_ConfigDataPtr = config;
#else
  J1939TP_DUMMY_STATEMENT(config); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
#endif

  /* ----- Development Error Checks ------------------------------------- */
  /*! \trace SPEC-2167297, SPEC-2167298 */
  if (J1939Tp_ModuleInitialized == J1939TP_INIT)
  {
    errorId = J1939TP_E_REINIT;
  }
#if (J1939TP_USE_INIT_POINTER == STD_ON)
  else if (J1939Tp_ConfigDataPtr == NULL_PTR)
  {
# if (J1939TP_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
    EcuM_BswErrorHook(J1939TP_MODULE_ID, ECUM_BSWERROR_NULLPTR);
# endif
    errorId = J1939TP_E_PARAM_POINTER;
  }
# if (J1939TP_FINALMAGICNUMBER == STD_ON)
  else if (J1939Tp_GetFinalMagicNumber() != J1939TP_FINAL_MAGIC_NUMBER)
  {
#  if (J1939TP_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
    EcuM_BswErrorHook(J1939TP_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
#  endif
    errorId = J1939TP_E_INIT_FAILED;
  }
# endif
#endif
  else
  {
#if ((J1939TP_USE_INIT_POINTER == STD_OFF) && (J1939TP_DEV_ERROR_REPORT == STD_ON))
    if (config != NULL_PTR)
    {
      errorId = J1939TP_E_PARAM_POINTER;
    }
#endif

    /* ----- Implementation ----------------------------------------------- */
#if (J1939TP_TXPDU == STD_ON)
    {
      J1939Tp_SizeOfTxPduStateType txPduStateIdx;
      for (txPduStateIdx = 0; txPduStateIdx < J1939Tp_GetSizeOfTxPduState(); txPduStateIdx++)
      {
        J1939Tp_TxNPduStateType *pduState = &J1939Tp_GetTxPduState(txPduStateIdx);
        /* no protection required - initialization may not happen while other APIs are still active and may not be
           interrupted */
        pduState->Locked = FALSE;
      }
    }
#endif

#if (J1939TP_TXSDU == STD_ON)
    {
      J1939Tp_SizeOfTxSduStateType txSduStateIdx;
      for (txSduStateIdx = 0; txSduStateIdx < J1939Tp_GetSizeOfTxSduState(); txSduStateIdx++)
      {
        J1939Tp_TxNSduStateType *txSduState = &J1939Tp_GetTxSduState(txSduStateIdx);

# if (J1939TP_FPP_ENABLED == STD_ON)
        /* NextBlock is used as sequence counter in FPP, and needs to be initialized to the initial sequence counter */
        txSduState->NextBlock = J1939TP_FPP_SC_INITIAL;
# endif

        /* no protection required - initialization may not happen while other APIs are still active and may not be
           interrupted */
        txSduState->State = J1939TP_STATE_IDLE;
      }
    }
#endif

#if (J1939TP_RXSDU == STD_ON)
    {
      J1939Tp_SizeOfRxSduStateType rxSduStateIdx;
      for (rxSduStateIdx = 0; rxSduStateIdx < J1939Tp_GetSizeOfRxSduState(); rxSduStateIdx++)
      {
        J1939Tp_RxNSduStateType *rxSduState = &J1939Tp_GetRxSduState(rxSduStateIdx);

# if (J1939TP_FPP_ENABLED == STD_ON)
        /* NextBlock is used as sequence counter in FPP, and needs to be initialized to a value that can never occurr */
        rxSduState->NextBlock = J1939TP_FPP_SC_INVALID;
# endif

        /* no protection required - initialization may not happen while other APIs are still active and may not be
           interrupted */
        rxSduState->State = J1939TP_STATE_IDLE;
      }
    }
#endif

    J1939Tp_ModuleInitialized = J1939TP_INIT;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (J1939TP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != J1939TP_E_NO_ERROR)
  {
    /*! \trace SPEC-2167289 */
    (void) J1939Tp_ReportDevError(J1939TP_SID_INIT, errorId);
  }
#else
  J1939TP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
#endif
} /* J1939Tp_Init */


/**********************************************************************************************************************
 *  J1939Tp_Shutdown
 *********************************************************************************************************************/

FUNC(void, J1939TP_CODE) J1939Tp_Shutdown(void)
{
  /* Deviation from API pattern: The following check has no effect if reporting is disabled. */
#if (J1939TP_DEV_ERROR_REPORT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = J1939TP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /*! \trace SPEC-2167297, SPEC-2167298 */
  if (J1939Tp_ModuleInitialized == J1939TP_UNINIT)
  {
    errorId = J1939TP_E_UNINIT;
  }
#endif

  /* ----- Implementation ----------------------------------------------- */
  /* #-- Set internal state to J1939TP_UNINIT. */
  J1939Tp_ModuleInitialized = J1939TP_UNINIT;

  /* ----- Development Error Report --------------------------------------- */
#if (J1939TP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != J1939TP_E_NO_ERROR)
  {
    /*! \trace SPEC-2167289 */
    (void) J1939Tp_ReportDevError(J1939TP_SID_SHUTDOWN, errorId);
  }
#endif
}


/**********************************************************************************************************************
 *  J1939Tp_GetVersionInfo
 *********************************************************************************************************************/

#if (J1939TP_VERSION_INFO_API == STD_ON)
FUNC(void, J1939TP_CODE) J1939Tp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, J1939TP_APPL_DATA) VersionInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = J1939TP_E_NO_ERROR;

# if (J1939TP_DEV_ERROR_DETECT == STD_ON)
  if (VersionInfo == NULL_PTR)
  {
    errorId = J1939TP_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    VersionInfo->vendorID = J1939TP_VENDOR_ID;
    VersionInfo->moduleID = J1939TP_MODULE_ID;
    VersionInfo->sw_major_version = J1939TP_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = J1939TP_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = J1939TP_SW_PATCH_VERSION;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (J1939TP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != J1939TP_E_NO_ERROR)
  {
    /*! \trace SPEC-2167289 */
    (void) J1939Tp_ReportDevError(J1939TP_SID_GETVERSIONINFO, errorId);
  }
# else
  J1939TP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
# endif
}
#endif


/**********************************************************************************************************************
 *  J1939Tp_MainFunction
 *********************************************************************************************************************/
/*! \brief      Main function of the J1939Tp. Used for scheduling purposes and timeout supervision.
 *
 *  \context    Only to be called from task level.
 *  \note       The main function must be called cyclically with a timing corresponding to the configured Main Function
 *              Period.
 *********************************************************************************************************************/

FUNC(void, J1939TP_CODE) J1939Tp_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = J1939TP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /*! \trace SPEC-2167297, SPEC-2167298 */
  if (J1939Tp_ModuleInitialized == J1939TP_UNINIT)
  {
    errorId = J1939TP_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#if (J1939TP_TXPDU == STD_ON)
    J1939Tp_InternalMain_TxPdu();
#endif

#if (J1939TP_TXSDU == STD_ON)
    J1939Tp_InternalMain_TxSdu();
#endif

#if (J1939TP_RXSDU == STD_ON)
    J1939Tp_InternalMain_RxSdu();
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (J1939TP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != J1939TP_E_NO_ERROR)
  {
    /*! \trace SPEC-2167289 */
    (void) J1939Tp_ReportDevError(J1939TP_SID_MAINFUNCTION, errorId);
  }
#else
  J1939TP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
#endif
}



/**********************************************************************************************************************
 *  J1939Tp_Transmit
 *********************************************************************************************************************/

FUNC(Std_ReturnType, J1939TP_CODE) J1939Tp_Transmit(PduIdType txSduId,
                                                    P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) pduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = J1939TP_E_NO_ERROR;
  Std_ReturnType result = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  /*! \trace SPEC-2167297, SPEC-2167298 */
  if (J1939Tp_ModuleInitialized == J1939TP_UNINIT)
  {
    errorId = J1939TP_E_UNINIT;
  }
#if (J1939TP_DEV_ERROR_DETECT == STD_ON)
  else if (pduInfoPtr == NULL_PTR)
  {
    errorId = J1939TP_E_PARAM_POINTER;
  }
# if (J1939TP_TXSDU == STD_ON)
  else if (txSduId >= J1939Tp_GetSizeOfTxSdu())
  {
    errorId = J1939TP_E_INVALID_PDU_SDU_ID;
  }
#  if (J1939TP_INVALIDHNDOFTXSDU == STD_ON)
  else if (J1939Tp_IsInvalidHndOfTxSdu(txSduId))
  {
    errorId = J1939TP_E_INVALID_PDU_SDU_ID;
  }
#  endif
#  if (J1939TP_ISVARIABLEOFTXSDU == STD_ON)
  else if ((J1939Tp_IsIsVariableOfTxSdu(txSduId)) && (pduInfoPtr->SduLength > J1939Tp_GetPduLengthOfTxSdu(txSduId)))
  {
    errorId = J1939TP_E_INVALID_LENGTH;
  }
  else if ((!J1939Tp_IsIsVariableOfTxSdu(txSduId)) && (pduInfoPtr->SduLength != J1939Tp_GetPduLengthOfTxSdu(txSduId)))
  {
    errorId = J1939TP_E_INVALID_LENGTH;
  }
#  else
  else if (pduInfoPtr->SduLength != J1939Tp_GetPduLengthOfTxSdu(txSduId))
  {
    errorId = J1939TP_E_INVALID_LENGTH;
  }
#  endif
# endif
  else if (
# if (J1939TP_METADATALENGTHOFTXSDU == STD_ON)
      (J1939Tp_GetMetaDataLengthOfTxSdu(txSduId) > 0) && 
# endif
      (pduInfoPtr->SduDataPtr == NULL_PTR))
  {
    errorId = J1939TP_E_PARAM_POINTER;
  }
#endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#if (J1939TP_TX_ENABLED == STD_ON)
    result = J1939Tp_InternalTransmit(txSduId, pduInfoPtr);
#else
    J1939TP_DUMMY_STATEMENT(txSduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
    J1939TP_DUMMY_STATEMENT(pduInfoPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
#endif
  }


  /* ----- Development Error Report --------------------------------------- */
#if (J1939TP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != J1939TP_E_NO_ERROR)
  {
    /*! \trace SPEC-2167289 */
    (void) J1939Tp_ReportDevError(J1939TP_SID_TRANSMIT, errorId);
  }
#else
  J1939TP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
#endif

  return result;
} /* J1939Tp_Transmit */ /* PRQA S 6080 */ /* MD_MSR_STMIF */


/**********************************************************************************************************************
 *  J1939Tp_RxIndication
 *********************************************************************************************************************/

/*! \trace SPEC-2167299 */
FUNC(void, J1939TP_CODE) J1939Tp_RxIndication(PduIdType RxPduId,
                                              P2CONST(PduInfoType, AUTOMATIC, J1939TP_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = J1939TP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /*! \trace SPEC-2167297, SPEC-2167298 */
  if (J1939Tp_ModuleInitialized == J1939TP_UNINIT)
  {
    errorId = J1939TP_E_UNINIT;
  }
#if (J1939TP_DEV_ERROR_DETECT == STD_ON)
  else if (PduInfoPtr == NULL_PTR)
  {
    errorId = J1939TP_E_PARAM_POINTER;
  }
  else if (PduInfoPtr->SduDataPtr == NULL_PTR)
  {
    errorId = J1939TP_E_PARAM_POINTER;
  }
# if (J1939TP_RXPDU == STD_ON)
  else if (RxPduId >= J1939Tp_GetSizeOfRxPdu())
  {
    errorId = J1939TP_E_INVALID_PDU_SDU_ID;
  }
#  if (J1939TP_INVALIDHNDOFRXPDU == STD_ON)
  else if (J1939Tp_IsInvalidHndOfRxPdu(RxPduId))
  {
    errorId = J1939TP_E_INVALID_PDU_SDU_ID;
  }
#  endif
# endif
#endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#if (J1939TP_RXPDU == STD_ON)
    J1939Tp_InternalRxIndication(RxPduId, PduInfoPtr);
#else
    J1939TP_DUMMY_STATEMENT(RxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
    J1939TP_DUMMY_STATEMENT(PduInfoPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (J1939TP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != J1939TP_E_NO_ERROR)
  {
    /*! \trace SPEC-2167289 */
    (void) J1939Tp_ReportDevError(J1939TP_SID_RXINDICATION, errorId);
  }
#else
  J1939TP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
#endif

} /* J1939Tp_RxIndication */ /* PRQA S 6080 */ /* MD_MSR_STMIF */


/**********************************************************************************************************************
 *  J1939Tp_TxConfirmation
 *********************************************************************************************************************/

/*! \trace SPEC-2167299 */
FUNC(void, J1939TP_CODE) J1939Tp_TxConfirmation(PduIdType TxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = J1939TP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /*! \trace SPEC-2167297, SPEC-2167298 */
  if (J1939Tp_ModuleInitialized == J1939TP_UNINIT)
  {
    errorId = J1939TP_E_UNINIT;
  }
#if ((J1939TP_TXPDU == STD_ON) && (J1939TP_DEV_ERROR_DETECT == STD_ON))
  else if (TxPduId >= J1939Tp_GetSizeOfTxPdu())
  {
    errorId = J1939TP_E_INVALID_PDU_SDU_ID;
  }
# if (J1939TP_INVALIDHNDOFTXPDU == STD_ON)
  else if (J1939Tp_IsInvalidHndOfTxPdu(TxPduId))
  {
    errorId = J1939TP_E_INVALID_PDU_SDU_ID;
  }
# endif
#endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#if (J1939TP_TXPDU == STD_ON)
    J1939Tp_TxNPduStateType *pduState = &J1939Tp_GetTxPduState(J1939Tp_GetTxPduStateIdxOfTxPdu(TxPduId));

    SchM_Enter_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if (pduState->Locked) /* PRQA S 3344 */ /* MD_J1939Tp_13.2 */
    {
      PduIdType sduIdx = pduState->CurrentNSduId;
      J1939Tp_NSduType sduType = pduState->CurrentNSduType;

      /* protected against concurrent Tx confirmation timeout */
      pduState->Locked = FALSE;
      SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      J1939Tp_InternalTxConfirmation(TxPduId, sduIdx, sduType);
    } /* if (pduState->Locked) */
    else
    {
      SchM_Exit_J1939Tp_TxNPduLock(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
#else
    J1939TP_DUMMY_STATEMENT(TxPduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (J1939TP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != J1939TP_E_NO_ERROR)
  {
    /*! \trace SPEC-2167289 */
    (void) J1939Tp_ReportDevError(J1939TP_SID_TXCONFIRMATION, errorId);
  }
#else
  J1939TP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
#endif
} /* J1939Tp_TxConfirmation */


/**********************************************************************************************************************
 *  J1939Tp_ChangeParameter
 *********************************************************************************************************************/
/* *INDENT-OFF* */
FUNC(Std_ReturnType, J1939TP_CODE) J1939Tp_ChangeParameter(PduIdType id, TPParameterType parameter, uint16 value) /* COV_J1939TP_UNUSED_FUNC_CHANGEPARAMETER */
/* *INDENT-ON* */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType result = E_NOT_OK;
  uint8 errorId = J1939TP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /*! \trace SPEC-2167297, SPEC-2167298 */
  if (J1939Tp_ModuleInitialized == J1939TP_UNINIT) /* COV_J1939TP_UNUSED_FUNC_CHANGEPARAMETER */
  {
    errorId = J1939TP_E_UNINIT;
  }
#if (J1939TP_DEV_ERROR_DETECT == STD_ON)
  else if (parameter != TP_BS) /* COV_J1939TP_UNUSED_FUNC_CHANGEPARAMETER */
  {
    errorId = J1939TP_E_INVALID_CHANGE_PARAM;
  }
  else if ((value == J1939TP_BS_MIN) || (value > J1939TP_BS_MAX)) /* COV_J1939TP_UNUSED_FUNC_CHANGEPARAMETER */
  {
    errorId = J1939TP_E_INVALID_CHANGE_VALUE;
  }
# if (J1939TP_RXSDU == STD_ON)
  else if (id >= J1939Tp_GetSizeOfRxSdu()) /* COV_J1939TP_UNUSED_FUNC_CHANGEPARAMETER */
  {
    errorId = J1939TP_E_INVALID_PDU_SDU_ID;
  }
#  if (J1939TP_INVALIDHNDOFRXSDU == STD_ON)
  else if (J1939Tp_IsInvalidHndOfRxSdu(id)) /* COV_J1939TP_UNUSED_FUNC_CHANGEPARAMETER */
  {
    errorId = J1939TP_E_INVALID_PDU_SDU_ID;
  }
#  endif
# endif
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (J1939TP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != J1939TP_E_NO_ERROR) /* COV_J1939TP_UNUSED_FUNC_CHANGEPARAMETER */
  {
    /*! \trace SPEC-2167289 */
    (void) J1939Tp_ReportDevError(J1939TP_SID_CHANGEPARAMETER, errorId);
  }
#else
  J1939TP_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
#endif
  J1939TP_DUMMY_STATEMENT(id); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
  J1939TP_DUMMY_STATEMENT(parameter); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */
  J1939TP_DUMMY_STATEMENT(value); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */

  return result;
} /* J1939Tp_ChangeParameter */ /* PRQA S 6080 */ /* MD_MSR_STMIF */


/**********************************************************************************************************************
 *  J1939Tp_CancelReceive
 *********************************************************************************************************************/
FUNC(Std_ReturnType, J1939TP_CODE) J1939Tp_CancelReceive(PduIdType id) /* COV_J1939TP_UNUSED_FUNC_CANCELRECEIVE */
{
  J1939TP_DUMMY_STATEMENT(id); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */

  /*! \trace SPEC-2167289 */
  J1939Tp_ReportDevError(J1939TP_SID_CANCELRECEIVE, J1939TP_E_DUMMY_API);

  return E_NOT_OK;
}


/**********************************************************************************************************************
 *  J1939Tp_CancelTransmit
 *********************************************************************************************************************/
FUNC(Std_ReturnType, J1939TP_CODE) J1939Tp_CancelTransmit(PduIdType id) /* COV_J1939TP_UNUSED_FUNC_CANCELTRANSMIT */
{
  J1939TP_DUMMY_STATEMENT(id); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2, MD_J1939Tp_3199 */ /* lint -e{438} */

  /*! \trace SPEC-2167289 */
  J1939Tp_ReportDevError(J1939TP_SID_CANCELTRANSMIT, J1939TP_E_DUMMY_API);

  return E_NOT_OK;
}

#define J1939TP_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/*============================================== J1939TP_STOP_SEC_CODE ==============================================*/


/* Module specific MISRA deviations:

 MD_J1939Tp_13.2
      Reason:     No deviation: MISRA rule 13.2 explicitly allows to use Boolean checks when it is ensured that the tested variable is only used as a Boolean value.
      Risk:       None
      Prevention: N/A

 MD_J1939Tp_3199
      Reason:     Dummy statement, used to avoid compiler warnings.
      Risk:       None
      Prevention: N/A
*/

/* COV_JUSTIFICATION_BEGIN

\ID COV_J1939TP_UNUSED_FUNC_CANCELRECEIVE
  \ACCEPT X
  \ACCEPT XX
  \REASON Function J1939Tp_CancelReceive is not used yet.

\ID COV_J1939TP_UNUSED_FUNC_CANCELTRANSMIT
  \ACCEPT X
  \ACCEPT XX
  \REASON Function J1939Tp_CancelTransmit is not used yet.

\ID COV_J1939TP_UNUSED_FUNC_CHANGEPARAMETER
  \ACCEPT X
  \ACCEPT XX
  \ACCEPT XX xx xx
  \REASON Function J1939Tp_ChangeParameter is not used yet.

\ID COV_J1939TP_SWITCHCASE_DEFAULT
  \ACCEPT X
  \REASON Switch case has a default part for MISRA checks which is not reachable.

COV_JUSTIFICATION_END */
