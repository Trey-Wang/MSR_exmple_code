/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                             All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Dcm_Ext.c
 *        \brief  Implementation of DCM extensions
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
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
#define DCM_EXT_SOURCE
/* ----------------------------------------------
 ~&&&   Includes
---------------------------------------------- */
                                                                                                                                                     /* PRQA S 0857, 0828 EOF */ /* MD_MSR_1.1_857, MD_MSR_1.1_828 */
#include "Dcm.h"
#include "Rte_Dcm.h"
#include "SchM_Dcm.h"
#if (DCM_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif /* (DCM_DEV_ERROR_REPORT == STD_ON) */
#if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)                                                                                                       /* COV_DCM_UNSUPPORTED XF */
# include "NvM.h"
#endif /* (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON) */
#include "Dcm_Int.h"
/* ----------------------------------------------
 ~&&&   Versions
---------------------------------------------- */
#if ((DCM_SW_MAJOR_VERSION !=  8u) || \
    (DCM_SW_MINOR_VERSION !=  3u) || \
    (DCM_SW_PATCH_VERSION !=  1u))
# error "Vendor specific version numbers of Dcm_Ext.c and Dcm.h are inconsistent"
#endif
#if ((DCM_CBK_MAJOR_VERSION !=  8u) || \
    (DCM_CBK_MINOR_VERSION !=  3u) || \
    (DCM_CBK_PATCH_VERSION !=  1u))
# error "Vendor specific version numbers of Dcm_Ext.c and Dcm_Cbk.h are inconsistent"
#endif
#if ((DCM_TYPES_MAJOR_VERSION !=  8u) || \
    (DCM_TYPES_MINOR_VERSION !=  3u) || \
    (DCM_TYPES_PATCH_VERSION !=  1u))
# error "Vendor specific version numbers of Dcm_Ext.c and Dcm_Types.h are inconsistent"
#endif
#if ((DCM_INT_MAJOR_VERSION !=  8u) || \
    (DCM_INT_MINOR_VERSION !=  3u) || \
    (DCM_INT_PATCH_VERSION !=  1u))
# error "Vendor specific version numbers of Dcm_Ext.c and Dcm_Int.h are inconsistent"
#endif
#if ((DCM_CORE_MAJOR_VERSION !=  8u) || \
    (DCM_CORE_MINOR_VERSION !=  3u) || \
    (DCM_CORE_PATCH_VERSION !=  1u))
# error "Vendor specific version numbers of Dcm_Ext.c and Dcm_Core.h are inconsistent"
#endif
#if ((DCM_CORECBK_MAJOR_VERSION !=  8u) || \
    (DCM_CORECBK_MINOR_VERSION !=  3u) || \
    (DCM_CORECBK_PATCH_VERSION !=  1u))
# error "Vendor specific version numbers of Dcm_Ext.c and Dcm_CoreCbk.h are inconsistent"
#endif
#if ((DCM_CORETYPES_MAJOR_VERSION !=  8u) || \
    (DCM_CORETYPES_MINOR_VERSION !=  3u) || \
    (DCM_CORETYPES_PATCH_VERSION !=  1u))
# error "Vendor specific version numbers of Dcm_Ext.c and Dcm_CoreTypes.h are inconsistent"
#endif
#if ((DCM_COREINT_MAJOR_VERSION !=  8u) || \
    (DCM_COREINT_MINOR_VERSION !=  3u) || \
    (DCM_COREINT_PATCH_VERSION !=  1u))
# error "Vendor specific version numbers of Dcm_Ext.c and Dcm_CoreInt.h are inconsistent"
#endif
#if ((DCM_EXT_MAJOR_VERSION !=  8u) || \
    (DCM_EXT_MINOR_VERSION !=  3u) || \
    (DCM_EXT_PATCH_VERSION !=  1u))
# error "Vendor specific version numbers of Dcm_Ext.c and Dcm_Ext.h are inconsistent"
#endif
#if ((DCM_EXTCBK_MAJOR_VERSION !=  8u) || \
    (DCM_EXTCBK_MINOR_VERSION !=  3u) || \
    (DCM_EXTCBK_PATCH_VERSION !=  1u))
# error "Vendor specific version numbers of Dcm_Ext.c and Dcm_ExtCbk.h are inconsistent"
#endif
#if ((DCM_EXTTYPES_MAJOR_VERSION !=  8u) || \
    (DCM_EXTTYPES_MINOR_VERSION !=  3u) || \
    (DCM_EXTTYPES_PATCH_VERSION !=  1u))
# error "Vendor specific version numbers of Dcm_Ext.c and Dcm_ExtTypes.h are inconsistent"
#endif
#if ((DCM_EXTINT_MAJOR_VERSION !=  8u) || \
    (DCM_EXTINT_MINOR_VERSION !=  3u) || \
    (DCM_EXTINT_PATCH_VERSION !=  1u))
# error "Vendor specific version numbers of Dcm_Ext.c and Dcm_ExtInt.h are inconsistent"
#endif
#if ((DCM_CFG_MAJOR_VERSION !=  8u) || \
    (DCM_CFG_MINOR_VERSION !=  3u) || \
    (DCM_CFG_PATCH_VERSION !=  1u))
# error "Vendor specific version numbers of Dcm_Ext.c and Dcm_Cfg.h are inconsistent"
#endif
#if ((DCM_LCFG_MAJOR_VERSION !=  8u) || \
    (DCM_LCFG_MINOR_VERSION !=  3u) || \
    (DCM_LCFG_PATCH_VERSION !=  1u))
# error "Vendor specific version numbers of Dcm_Ext.c and Dcm_Lcfg.h are inconsistent"
#endif
#if ((DCM_PBCFG_MAJOR_VERSION !=  8u) || \
    (DCM_PBCFG_MINOR_VERSION !=  3u) || \
    (DCM_PBCFG_PATCH_VERSION !=  1u))
# error "Vendor specific version numbers of Dcm_Ext.c and Dcm_PBcfg.h are inconsistent"
#endif
#if (DCM_EXTENSION_ID != 0x96ACD543UL)
# error "Mismatching OEMs between static and generated code"
#endif
/* ----------------------------------------------
 ~&&&   Switches
---------------------------------------------- */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_DCM_UNSUPPORTED XF */
# define DCM_EXT_INSTANCE_CONTEXT_ENABLED                            STD_ON
#else
# define DCM_EXT_INSTANCE_CONTEXT_ENABLED                            STD_OFF
#endif
/* ----------------------------------------------
 ~&&&   Defines
---------------------------------------------- */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_DCM_UNSUPPORTED XF */
# define DCM_EXT_SVC_86_READ_TIMEOUT                                 ((Dcm_ExtSvc86RoeTimeoutType)10)

/* states of the Roe service handler */
# define DCM_EXT_SVC_86_STATE_ACTIVE                                 ((Dcm_ExtSvc86RoeStateType)DCM_ROE_AR_STATE_ACTIVE)
# define DCM_EXT_SVC_86_STATE_INACTIVE                               ((Dcm_ExtSvc86RoeStateType)DCM_ROE_AR_STATE_INACTIVE)
# define DCM_EXT_SVC_86_STATE_CLEARED                                ((Dcm_ExtSvc86RoeStateType)DCM_ROE_AR_STATE_CLEARED)

# define DCM_EXT_SVC_86_STATE_ACTIVE_SETUP_STORED                    ((Dcm_ExtSvc86RoeStateType)0x11u)
# define DCM_EXT_SVC_86_STATE_ACTIVE_SETUP_NOT_STORED                ((Dcm_ExtSvc86RoeStateType)0x12u)
# define DCM_EXT_SVC_86_STATE_INACTIVE_SETUP_NOT_STORED              ((Dcm_ExtSvc86RoeStateType)0x13u)
# define DCM_EXT_SVC_86_STATE_INACTIVE_STOP_NOT_STORED               ((Dcm_ExtSvc86RoeStateType)0x14u)
# define DCM_EXT_SVC_86_STATE_WRITE_NVM_DATA                         ((Dcm_ExtSvc86RoeStateType)0x15u)

/* STRT sending states */
# define DCM_EXT_SVC_86_STRT_STATE_IDLE                              ((Dcm_ExtSvc86STRTStateType)0x00u)
# define DCM_EXT_SVC_86_STRT_STATE_SENDING                           ((Dcm_ExtSvc86STRTStateType)0x01u)
# define DCM_EXT_SVC_86_STRT_STATE_WAIT_FOR_TIMEOUT                  ((Dcm_ExtSvc86STRTStateType)0x02u)

/* access to Roe context data */
# define Dcm_ExtSvc86HandlerState                                    (Dcm_ExtInstanceContext.Svc86.CurrentSession.HandlerState)
# define Dcm_ExtSvc86OldHandlerState                                 (Dcm_ExtInstanceContext.Svc86.CurrentSession.OldHandlerState)
# define Dcm_ExtSvc86RoEState                                        (Dcm_ExtInstanceContext.Svc86.CurrentSession.NvmData.RoEState)
# define Dcm_ExtSvc86ReadTimeout                                     (Dcm_ExtInstanceContext.Svc86.CurrentSession.NvmReadTimeout)
# define Dcm_ExtSvc86NvMData                                         (Dcm_ExtInstanceContext.Svc86.CurrentSession.NvmData)
# define Dcm_ExtSvc86NvMReadOpStatus                                 (Dcm_ExtInstanceContext.Svc86.CurrentSession.NvmReadOpStatus)
# define Dcm_ExtSvc86DTCStatusMask                                   (Dcm_ExtInstanceContext.Svc86.CurrentSession.NvmData.DTCStatusMask)

# define Dcm_ExtSvc86CurrentSessionContext                           (Dcm_ExtInstanceContext.Svc86.CurrentSession)
# define Dcm_ExtSvc86DefaultSessionContext                           (Dcm_ExtInstanceContext.Svc86.DefaultSession)
# define Dcm_ExtSvc86STRTSendingState                                (Dcm_ExtInstanceContext.Svc86.STRTSendingState)

# define DCM_EXTSVC86_STOPROE_SID                                    0x00u
# define DCM_EXTSVC86_ONDTCSTATUSCHANGE_SID                          0x01u
# define DCM_EXTSVC86_STARTROE_SID                                   0x05u
# define DCM_EXTSVC86_CLEARROE_SID                                   0x06u

# define DCM_CFG_EWT_INFINITE_OCY                                    0x02u
# define DCM_CFG_EWT_CURRENT_OCY                                     0x03u
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
/* ----------------------------------------------
 ~&&&   Function-like macros
---------------------------------------------- */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_DCM_UNSUPPORTED XF */
# define Dcm_ExtGetRepeaterContextSvc86()                            ((Dcm_ExtSvc86RepeaterContextPtrType)Dcm_RepeaterGetExtSvcContext(DCM_REPEATER_EXT_CNTXT_SVC86))
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
/* ----------------------------------------------
 ~&&&   Typedefs
---------------------------------------------- */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_DCM_UNSUPPORTED XF */
typedef uint8 Dcm_ExtSvc86RoeTimeoutType;

struct DCM_EXTSVC86SESSIONCONTEXTTYPE_TAG
{
  Dcm_ExtSvc86NvMDataType    NvmData;
  Dcm_ExtSvc86RoeStateType   HandlerState;             /* state of the internal RoE state machine */
  Dcm_ExtSvc86RoeStateType   OldHandlerState;          /* previous state of the internal RoE state machine */
  Dcm_ExtSvc86RoeTimeoutType NvmReadTimeout;           /* timeout counter to handle failed calls to NvM_ReadBlock block */
  Dcm_OpStatusType           NvmReadOpStatus;          /* current operation status during initialization for NvM reading */
};
typedef struct DCM_EXTSVC86SESSIONCONTEXTTYPE_TAG Dcm_ExtSvc86SessionContextType;

struct DCM_EXTSVC86DEFAULTSESSIONCONTEXTTYPE_TAG
{
  Dcm_ExtSvc86RoeStateType   RoEState;       /* RoE state */
  Dcm_ExtSvc86RoeStateType   HandlerState;   /* state of the internal RoE state machine */
  PduIdType                  RxPduId;        /* Id of connection requesting RoE */
};
typedef struct DCM_EXTSVC86DEFAULTSESSIONCONTEXTTYPE_TAG Dcm_ExtSvc86DefaultSessionContextType;

# if (DCM_EXT_INSTANCE_CONTEXT_ENABLED == STD_ON)
struct DCM_EXTSVC86CONTEXTTYPE_TAG
{
  Dcm_ExtSvc86SessionContextType CurrentSession;
#  if (DCM_SVC_86_RST_ON_DSC_ENABLED == STD_ON)
  Dcm_ExtSvc86DefaultSessionContextType DefaultSession;
#  endif
  Dcm_ExtSvc86STRTStateType STRTSendingState;
};
typedef struct DCM_EXTSVC86CONTEXTTYPE_TAG Dcm_ExtSvc86ContextType;
# endif

# if (DCM_EXT_INSTANCE_CONTEXT_ENABLED == STD_ON)
struct DCM_EXTINSTANCECONTEXTTYPE_TAG
{
#  if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
  Dcm_ExtSvc86ContextType   Svc86;         /* state of the internal RoE state machine */
#  endif
};
typedef struct DCM_EXTINSTANCECONTEXTTYPE_TAG Dcm_ExtInstanceContextType;
# endif
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
/* ----------------------------------------------
 ~&&&   Module internal inlined function declarations
---------------------------------------------- */
#define DCM_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_DCM_UNSUPPORTED XF */
/**********************************************************************************************************************
 *  Dcm_ExtService86_CheckEWT_CtrlRequest_DefaultSession()
 *********************************************************************************************************************/
/*! \brief          Checks EventWindowTime and storeEvent combination for control request.
 *  \details        -
 *  \param[in]      eventWindowTime         EventWindowTime from request
 *  \param[in]      storageStateSupport     TRUE if eventWindowTime supports StorageState
 *                                          FALSE if eventWindowTime supports StorageState
 *  \param[in,out]  storeEvent              TRUE RoE state shall be stored.
 *                                          FALSE RoE state shall not be stored.
 *  \param[out]     errorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                Combination is valid.
 *  \return         DCM_E_NOT_OK            Combination is not valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            ECU is in default session and storage Stage is supported for passed eventWindowTime
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_CheckEWT_CtrlRequest_DefaultSession(
                                                                            uint8   eventWindowTime
                                                                           ,boolean storageStateSupport
                                                                           ,P2VAR(boolean, AUTOMATIC, DCM_VAR_NOINIT) storeEvent
                                                                           ,Dcm_NegativeResponseCodePtrType errorCode);

/**********************************************************************************************************************
 *  Dcm_ExtService86_CheckEWT_SetupRequest_DefaultSession()
 *********************************************************************************************************************/
/*! \brief          Checks EventWindowTime and storeEvent combination for setup request.
 *  \details        -
 *  \param[in]      eventWindowTime         EventWindowTime from request
 *  \param[in]      storageStateSupport     TRUE if eventWindowTime supports StorageState
 *                                          FALSE if eventWindowTime supports StorageState
 *  \param[in]      storeEvent              TRUE RoE state shall be stored.
 *                                          FALSE RoE state shall not be stored.
 *  \param[out]     errorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                Combination is valid.
 *  \return         DCM_E_NOT_OK            Combination is not valid.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            ECU is in default session and storage Stage is supported for passed eventWindowTime
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_CheckEWT_SetupRequest_DefaultSession(
                                                                            uint8   eventWindowTime
                                                                           ,boolean storageStateSupport
                                                                           ,P2VAR(boolean, AUTOMATIC, DCM_VAR_NOINIT) storeEvent
                                                                           ,Dcm_NegativeResponseCodePtrType errorCode);

/**********************************************************************************************************************
 *  Dcm_ExtService86_CheckEWT_NonDefaultSession()
 *********************************************************************************************************************/
/*! \brief          Checks EventWindowTime and storeEvent combination in non-default session.
 *  \details        -
 *  \param[in]      eventWindowTime         EventWindowTime from request
 *  \param[in]      storageStateSupport     TRUE if eventWindowTime supports StorageState
 *                                          FALSE if eventWindowTime supports StorageState
 *  \param[in,out]  storeEvent              TRUE RoE state shall be stored.
 *                                          FALSE RoE state shall not be stored.
 *  \param[out]     errorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                Combination is valid.
 *  \return         DCM_E_NOT_OK            Combination is not valid.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Ecu is in non-default session
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_CheckEWT_NonDefaultSession(
                                                                uint8   eventWindowTime
                                                               ,boolean storageStateSupport
                                                               ,P2VAR(boolean, AUTOMATIC, DCM_VAR_NOINIT) storeEvent
                                                               ,Dcm_NegativeResponseCodePtrType errorCode);

/**********************************************************************************************************************
 *  Dcm_ExtService86_Send_STRT()
 *********************************************************************************************************************/
/*! \brief          Trying to send ServiceToRespondTo.
 *  \details        -
 *  \param[in,out]  pPostEv               The event(s) to be processed next task activation
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            ComM state shall still be in FullCommunicationMode.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_ExtService86_Send_STRT(Dcm_TskTaskEvPtrType pPostEv);

/**********************************************************************************************************************
 *  Dcm_ExtService86_ClearTaskEvents()
 *********************************************************************************************************************/
/*! \brief          Clearing task events.
 *  \details        -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_ExtService86_ClearTaskEvents(void);

/**********************************************************************************************************************
 *  Dcm_ExtService86_CheckEWT_Internal()
 *********************************************************************************************************************/
/*! \brief          Checks EventWindowTime and storeEvent combination.
 *  \details        Checks combination for the sub-Functions stopResponseToEvent, startResponseToEvent and
 *                  onDtcStatusChange.
 *  \param[in]      session                 Current diagnostic session
 *  \param[in]      eventType               Type of RoE sub-function
 *  \param[in]      eventWindowTime         EventWindowTime from request
 *  \param[in]      storageStateSupport     TRUE if eventWindowTime supports StorageState
 *                                          FALSE if eventWindowTime supports StorageState
 *  \param[in,out]  storeEvent              If RoE information shall be stored.
 *  \param[out]     errorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                Combination is valid.
 *  \return         DCM_E_NOT_OK            Combination is not valid.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Ecu is in default session and storage Stage is supported for passed eventWindowTime
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_CheckEWT_Internal(
                                                                 Dcm_SesCtrlType session
                                                                ,uint8           eventType
                                                                ,uint8           eventWindowTime
                                                                ,boolean         storageStateSupport
                                                                ,P2VAR(boolean, AUTOMATIC, DCM_VAR_NOINIT) storeEvent
                                                                ,Dcm_NegativeResponseCodePtrType errorCode);

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtSvc86ReadNvM()
 *********************************************************************************************************************/
/*! \brief          Reads RoE information from NvM.
 *  \details        -
 *  \param[in,out]  pPostEv               The event(s) to be processed next task activation
 *  \return         DCM_E_OK              Read operation finished successfully.
 *  \return         DCM_E_NOT_OK          Read operation finished with error or got canceled.
 *  \return         DCM_E_PENDING         Read operation needs more time to be accomplished.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_ExtSvc86ReadNvM(Dcm_TskTaskEvPtrType pPostEv);
# endif

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtService86_DTCStatusChange_StoreEvent()
 *********************************************************************************************************************/
/*! \brief          Initializes RoE setup.
 *  \details        -
 *  \param[in]      opStatus                The operation status
 *  \param[in]      dtcStatusMask           DTCStatusMask in request
 *  \param[out]     errorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                Service processing done.
 *  \return         DCM_E_NOT_OK            Service processing failed.
 *  \return         DCM_E_PENDING           Service operation needs more time to be accomplished.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_DTCStatusChange_StoreEvent(Dcm_OpStatusType      opStatus
                                                                                           ,uint8               dtcStatusMask
                                                                                           ,Dcm_NegativeResponseCodePtrType errorCode);
# endif
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/* ----------------------------------------------
 ~&&&   Module internal function declarations
---------------------------------------------- */
#define DCM_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_DCM_UNSUPPORTED XF */
/**********************************************************************************************************************
 *  Dcm_ExtService86_StartRoE()
 *********************************************************************************************************************/
/*! \brief          Changes RoE state to active.
 *  \details        -
 *  \param[in]      opStatus                The operation status
 *  \param[in,out]  pMsgContext             The current request context
 *  \param[in]      storeEvent              TRUE if RoE state shall be stored.
 *                                          FALSE if RoE state shall not be stored.
 *  \param[out]     errorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                Service processing done.
 *  \return         DCM_E_NOT_OK            Service processing was not successful.
 *  \return         DCM_E_PENDING           Service operation needs more time to be accomplished.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_StartRoE(Dcm_OpStatusType      opStatus
                                                                  ,Dcm_MsgContextPtrType pMsgContext
                                                                  ,boolean               storeEvent
                                                                  ,Dcm_NegativeResponseCodePtrType errorCode);

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtService86_StartRoE_StoreEvent()
 *********************************************************************************************************************/
/*! \brief          Changes RoE state to inactive and stores event state.
 *  \details        -
 *  \param[in]      opStatus                The operation status
 *  \param[in]      pMsgContext             The current request context
 *  \param[out]     errorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                Service processing was successful.
 *  \return         DCM_E_PENDING           Service operation needs more time to be accomplished.
 *  \return         DCM_E_NOT_OK            Service processing was not successful.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_StartRoE_StoreEvent(
                                                                           Dcm_OpStatusType                opStatus
                                                                          ,Dcm_ReadOnlyMsgContextPtrType   pMsgContext
                                                                          ,Dcm_NegativeResponseCodePtrType errorCode);
# endif
/**********************************************************************************************************************
 *  Dcm_ExtService86_StartRoE_Internal()
 *********************************************************************************************************************/
/*! \brief          Changes RoE state to inactive.
 *  \details        -
 *  \param[in]      pMsgContext             The current request context
 *  \param[out]     errorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                Service processing was successful.
 *  \return         DCM_E_NOT_OK            Service processing was not successful.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_StartRoE_Internal(Dcm_ReadOnlyMsgContextPtrType pMsgContext
                                                                           ,Dcm_NegativeResponseCodePtrType errorCode);

/**********************************************************************************************************************
 *  Dcm_ExtService86_StopRoE()
 *********************************************************************************************************************/
/*! \brief          Changes RoE state to inactive.
 *  \details        -
 *  \param[in]      opStatus                The operation status
 *  \param[in,out]  pMsgContext             The current request context
 *  \param[in]      storeEvent              TRUE if RoE state shall be stored.
 *                                          FALSE if RoE state shall not be stored.
 *  \param[out]     errorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                Service processing done.
 *  \return         DCM_E_NOT_OK            Service processing was not successful.
 *  \return         DCM_E_PENDING           Service operation needs more time to be accomplished.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_StopRoE(Dcm_OpStatusType      opStatus
                                                                 ,Dcm_MsgContextPtrType pMsgContext
                                                                 ,boolean               storeEvent
                                                                 ,Dcm_NegativeResponseCodePtrType errorCode);

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtService86_StopRoE_StoreEvent()
 *********************************************************************************************************************/
/*! \brief          Changes RoE state to inactive and stores event.
 *  \details        -
 *  \param[in]      opStatus                The operation status
 *  \param[out]     errorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                Service processing was successful.
 *  \return         DCM_E_PENDING           Service operation needs more time to be accomplished.
 *  \return         DCM_E_NOT_OK            Service processing was not successful.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_StopRoE_StoreEvent(
                                                                           Dcm_OpStatusType opStatus
                                                                          ,Dcm_NegativeResponseCodePtrType errorCode);
# endif

/**********************************************************************************************************************
 *  Dcm_ExtService86_StopRoE_Internal()
 *********************************************************************************************************************/
/*! \brief          Changes RoE state to inactive.
 *  \details        -
 *  \param[in,out]  errorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                Service processing was successful.
 *  \return         DCM_E_NOT_OK            Service processing was not successful.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_StopRoE_Internal(Dcm_NegativeResponseCodePtrType errorCode);

/**********************************************************************************************************************
 *  Dcm_ExtService86_OnDTCStatusChange()
 *********************************************************************************************************************/
/*! \brief          Initializes RoE setup.
 *  \details        -
 *  \param[in]      opStatus                The operation status
 *  \param[in,out]  pMsgContext             The current request context
 *  \param[in]      storeEvent              TRUE if setup shall be stored.
 *                                          FALSE if setup shall not be stored.
 *  \param[out]     errorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                Service processing done.
 *  \return         DCM_E_NOT_OK            Service processing was not successful.
 *  \return         DCM_E_PENDING           Service operation needs more time to be accomplished.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_OnDTCStatusChange(Dcm_OpStatusType      opStatus
                                                                           ,Dcm_MsgContextPtrType pMsgContext
                                                                           ,boolean               storeEvent
                                                                           ,Dcm_NegativeResponseCodePtrType errorCode);

/**********************************************************************************************************************
 *  Dcm_ExtService86_CheckEWT()
 *********************************************************************************************************************/
/*! \brief          Checks EventWindowTime and storeEvent combination.
 *  \details        -
 *  \param[in]      eventType               Type of RoE sub-function
 *  \param[in]      eventWindowTime         EventWindowTime from request
 *  \param[in,out]  storeEvent              If RoE information shall be stored.
 *  \param[out]     errorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                Combination is valid.
 *  \return         DCM_E_NOT_OK            Combination is not valid.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Ecu is in default session and storage Stage is supported for passed eventWindowTime
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_CheckEWT(
                                                                  uint8 eventType
                                                                 ,uint8 eventWindowTime
                                                                 ,P2VAR(boolean, AUTOMATIC, DCM_VAR_NOINIT) storeEvent
                                                                 ,Dcm_NegativeResponseCodePtrType           errorCode);

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtSvc86UpdateNvM()
 *********************************************************************************************************************/
/*! \brief          Updates the NvM with the RoE information.
 *  \details        -
 *  \param[in]      opStatus              Operation status
 *  \param[in]      extSvc86ContextPtr    The RoE context
 *  \param[out]     errorCode             Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK              Update operation finished successfully.
 *  \return         DCM_E_NOT_OK          Update operation finished with error or got canceled.
 *  \return         DCM_E_PENDING         Update operation needs more time to be accomplished.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtSvc86UpdateNvM(
                                                         Dcm_OpStatusType                           opStatus
                                                        ,Dcm_ExtSvc86ReadOnlyRepeaterContextPtrType extSvc86ContextPtr
                                                        ,Dcm_NegativeResponseCodePtrType            errorCode);
# endif

/**********************************************************************************************************************
 *  Dcm_ExtSvc86_ClearRoE()
 *********************************************************************************************************************/
/*! \brief          Clear RoE logic.
 *  \details        RoE state is set to cleared and new state is stored.
 *  \param[in]      opStatus                Operation status
 *  \param[in,out]  pMsgContext             The current request context
 *  \return         DCM_E_OK                Clear processing done.
 *  \return         DCM_E_NOT_OK            Clearing was not successful. 
 *  \return         DCM_E_PENDING           Clear operation needs more time to be accomplished.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtSvc86_ClearRoE(Dcm_OpStatusType opStatus
                                                              ,Dcm_MsgContextPtrType pMsgContext
                                                              ,Dcm_NegativeResponseCodePtrType errorCode);

/**********************************************************************************************************************
 *  Dcm_ExtService86_CheckRoEClient()
 *********************************************************************************************************************/
/*! \brief          Checks if client is the same client that that started RoE.
 *  \details        -
 *  \param[in]      pMsgContext             The current request context
 *  \param[out]     errorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                Client is same.
 *  \return         DCM_E_NOT_OK            Client is not same.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_CheckRoEClient(Dcm_ReadOnlyMsgContextPtrType   pMsgContext
                                                                        ,Dcm_NegativeResponseCodePtrType errorCode);
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/* ----------------------------------------------
 ~&&&   Uninitialized RAM of unspecified size
---------------------------------------------- */
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_DCM_UNSUPPORTED XF */
# if (DCM_EXT_INSTANCE_CONTEXT_ENABLED == STD_ON)
DCM_LOCAL VAR(Dcm_ExtInstanceContextType, DCM_VAR_NOINIT)  Dcm_ExtInstanceContext;
# endif
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/* ----------------------------------------------
 ~&&&   ROM of unspecified size
---------------------------------------------- */
#define DCM_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_DCM_UNSUPPORTED XF */
CONST(Dcm_ExtSvc86NvMDataType, DCM_CONST) DcmExtSvc86DefaultNvMData =
{
  DCM_EXT_SVC_86_STATE_CLEARED /* per default - clear RoE */
 ,0x00
 ,0
 ,0
};
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/* ----------------------------------------------
 ~&&&   Inlined function implementations
---------------------------------------------- */
#define DCM_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_DCM_UNSUPPORTED XF */
/**********************************************************************************************************************
 *  Dcm_ExtService86_CheckEWT_NonDefaultSession()
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
 *
 *
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_CheckEWT_NonDefaultSession(
                                                                uint8   eventWindowTime
                                                               ,boolean storageStateSupport
                                                               ,P2VAR(boolean, AUTOMATIC, DCM_VAR_NOINIT) storeEvent
                                                               ,Dcm_NegativeResponseCodePtrType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if(storageStateSupport == TRUE)
  {
    /* storeEvent is considered */
    if(*storeEvent == TRUE)
    {
      /* Storing state 'Active' in Non-Default Session is not allowed */
      *errorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  }
  else
  {
    /* storeEvent is not considered */
    if(eventWindowTime == DCM_CFG_EWT_CURRENT_OCY)
    {
      /* RoE state is not stored for eventWindowTime current cycle */
      *storeEvent = FALSE;
    }
    else
    {
      /* Storing state 'Active' in Non-Default Session is not allowed */
      *errorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_ExtService86_CheckEWT_CtrlRequest_DefaultSession()
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
 *
 *
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_CheckEWT_CtrlRequest_DefaultSession(
                                                                             uint8   eventWindowTime
                                                                            ,boolean storageStateSupport
                                                                            ,P2VAR(boolean, AUTOMATIC, DCM_VAR_NOINIT) storeEvent
                                                                            ,Dcm_NegativeResponseCodePtrType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if(storageStateSupport == TRUE)
  {
    /* storage state supported for eventWindowTime */
    if((*storeEvent == TRUE) && (eventWindowTime == DCM_CFG_EWT_CURRENT_OCY))
    {
      /* storeEvent not allowed with DCM_CFG_EWT_CURRENT_OCY */
      *errorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  }
  else
  {
    if(eventWindowTime == DCM_CFG_EWT_CURRENT_OCY)
    {
      *storeEvent = FALSE;
    }
    else
    {
      *storeEvent = TRUE;
    }
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_ExtService86_CheckEWT_SetupRequest_DefaultSession()
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
 *
 *
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_CheckEWT_SetupRequest_DefaultSession(
                                                                            uint8   eventWindowTime
                                                                           ,boolean storageStateSupport
                                                                           ,P2VAR(boolean, AUTOMATIC, DCM_VAR_NOINIT) storeEvent
                                                                           ,Dcm_NegativeResponseCodePtrType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if(storageStateSupport == TRUE)
  {
    if( ((*storeEvent == TRUE)  && (eventWindowTime == DCM_CFG_EWT_CURRENT_OCY))
      ||((*storeEvent == FALSE) && (eventWindowTime == DCM_CFG_EWT_INFINITE_OCY))
      )
    {
      /* storeEvent and eventWindowTime combination not allowed */
      *errorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  }
  else
  {
    if(eventWindowTime == DCM_CFG_EWT_CURRENT_OCY)
    {
      *storeEvent = FALSE;
    }
    else
    {
      *storeEvent = TRUE;
    }
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_ExtService86_Send_STRT()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_ExtService86_Send_STRT(Dcm_TskTaskEvPtrType pPostEv)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  rxIndicationResult;
  Dcm_MsgItemType vReq[2] =
  {
     0x19u
    ,0x0Eu
  };

  /* ----- Implementation ----------------------------------------------- */
  /* trigger RoE */
  rxIndicationResult = Dcm_NetRxIndInternal(Dcm_ExtSvc86NvMData.RxPduId , vReq, 2);

  if (rxIndicationResult != DCM_E_OK)
  {
    /* Set STRT state to IDLE and retry sending next task cycle */
    Dcm_TskSetLocalEvent(*pPostEv, DCM_EXT_TSK_EV_ROE_SEND);
    Dcm_ExtSvc86STRTSendingState = DCM_EXT_SVC_86_STRT_STATE_IDLE;
  }
}

/**********************************************************************************************************************
 *  Dcm_ExtService86_ClearTaskEvents()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_ExtService86_ClearTaskEvents(void)
{
  Dcm_TskClrEvent(DCM_TSK_ID_ROE, (DCM_EXT_TSK_EV_ROE_SEND | DCM_EXT_TSK_EV_ROE_TX_END));

# if (DCM_SVC_86_TIMER_ENABLED != STD_ON)
  /* intermessage time is 0; next STRT can be sent with next task event */
  Dcm_ExtSvc86STRTSendingState = DCM_EXT_SVC_86_STRT_STATE_IDLE;
# endif
}

/**********************************************************************************************************************
 *  Dcm_ExtService86_CheckEWT_Internal()
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
 *
 *
 *
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_CheckEWT_Internal(
                                                                 Dcm_SesCtrlType session
                                                                ,uint8           eventType
                                                                ,uint8           eventWindowTime
                                                                ,boolean         storageStateSupport
                                                                ,P2VAR(boolean, AUTOMATIC, DCM_VAR_NOINIT) storeEvent
                                                                ,Dcm_NegativeResponseCodePtrType errorCode)
{
  Std_ReturnType lStdResult;

  if(session == DCM_DEFAULT_SESSION)
  {
    /* default session */
    if((eventType == DCM_EXTSVC86_STOPROE_SID) || (eventType == DCM_EXTSVC86_STARTROE_SID) )
    {
      /* conditions for startResponseOnEvent and stopResponseOnEvent are equal in default session */
      lStdResult = Dcm_ExtService86_CheckEWT_CtrlRequest_DefaultSession(eventWindowTime, storageStateSupport, storeEvent, errorCode);
    }
    else
    {
      /* onDTCDtatusChange */
      lStdResult = Dcm_ExtService86_CheckEWT_SetupRequest_DefaultSession(eventWindowTime, storageStateSupport, storeEvent, errorCode);
    }
  }
  else
  {
    /* non-default session */
    if(eventType == DCM_EXTSVC86_STOPROE_SID)
    {
      /* conditions for stopResponseOnEvent is in non-default session same as in default session */
      lStdResult = Dcm_ExtService86_CheckEWT_CtrlRequest_DefaultSession(eventWindowTime, storageStateSupport, storeEvent, errorCode);
    }
    else
    {
      if((eventType == DCM_EXTSVC86_ONDTCSTATUSCHANGE_SID) && (eventWindowTime == DCM_CFG_EWT_INFINITE_OCY))
      {
        /* EventWindowTime Infinite not allowed for onDTCDtatusChange */
        *errorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
      else
      {
        /* conditions for startResponseOnEvent and onDTCDtatusChange are mostly equal in non-default session */
        lStdResult = Dcm_ExtService86_CheckEWT_NonDefaultSession(eventWindowTime, storageStateSupport, storeEvent, errorCode);
      }
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6060 */ /* MD_Dcm_Design_6060 */

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtSvc86ReadNvM()
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_ExtSvc86ReadNvM(Dcm_TskTaskEvPtrType pPostEv)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult;

  /* ----- Implementation ----------------------------------------------- */
  /* get persistent data from NvM */
  lStdResult = Dcm_UtiNvMRead(Dcm_ExtSvc86NvMReadOpStatus, &Dcm_ExtSvc86NvMData, DCM_EXT_SVC_86_NVRAM_BLOCKID);

  if(lStdResult == DCM_E_OK)
  {
#  if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
    if(Dcm_ExtSvc86NvMData.RoEState == DCM_EXT_SVC_86_STATE_ACTIVE)
    {
      Dcm_CfgVarMgrComVariantType pComVariant = Dcm_VarMgrGetActiveComVariant();

      if( (Dcm_ExtSvc86NvMData.RxPduId >= Dcm_NetCfgGetNumRxPduIds(pComVariant))
        ||(Dcm_NetCfgGetRxPduInfo(pComVariant)[Dcm_ExtSvc86NvMData.RxPduId].ConnRef >= Dcm_NetCfgGetNumConnections(pComVariant)) /* keep evaluation order to guard the connRef indirection */
        ||(Dcm_ExtSvc86NvMData.ClientSrcAddr != Dcm_NetCfgGetTesterAddressOfRxPduId(pComVariant, Dcm_ExtSvc86NvMData.RxPduId)) )
      {
        /* Restore default values */
        Dcm_UtiAssignStruct(Dcm_ExtSvc86NvMData, DcmExtSvc86DefaultNvMData);
      }
    }
#  endif
    /* reading data from NvM finished; change state according to stored RoE state */
    Dcm_ExtSvc86HandlerState = Dcm_ExtSvc86NvMData.RoEState;
    Dcm_RoEOnEventStateChange(DCM_EXT_SVC_86_ROE_EVENTID, Dcm_ExtSvc86NvMData.RoEState);

    /* If RoE state is active */
    if (Dcm_ExtSvc86NvMData.RoEState == DCM_EXT_SVC_86_STATE_ACTIVE)
    {
      /* notify Dem to start triggering DTC status changes */
      Dcm_DemAPI_ControlDTCStatusChangedNotification(TRUE);
    }
  }
  else if(lStdResult == DCM_E_PENDING)
  {
    /* retry later */
    Dcm_TskSetLocalEvent(*pPostEv, DCM_EXT_TSK_EV_ROE_ACTIVATE);                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_ExtSvc86NvMReadOpStatus = DCM_PENDING;
  }
  else  /* DCM_E_NOT_OK */
  {
    /* If NvM request was not accepted -> retry later until timeout. Once accepted (OpStatus == DCM_PENDING) would mean that the read access has failed -> close the job */
    if( (Dcm_ExtSvc86NvMReadOpStatus == DCM_INITIAL)
      &&(Dcm_ExtSvc86ReadTimeout > 0) )
    {
      /* timeout not yet reached; try again in next cycle */
      Dcm_ExtSvc86ReadTimeout--;
      Dcm_TskSetLocalEvent(*pPostEv, DCM_EXT_TSK_EV_ROE_ACTIVATE);                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
     /* Start new cycle - leave opStatus = DCM_INITIAL
      * Dcm_ExtSvc86NvMReadOpStatus = DCM_INITIAL;
      */
    }
    else
    {
      /* read data from NvM failed; set to default */
      Dcm_UtiAssignStruct(Dcm_ExtSvc86NvMData, DcmExtSvc86DefaultNvMData);

      Dcm_ExtSvc86HandlerState = Dcm_ExtSvc86NvMData.RoEState;
      Dcm_RoEOnEventStateChange(DCM_EXT_SVC_86_ROE_EVENTID, Dcm_ExtSvc86NvMData.RoEState);
    }
  }
}
# endif

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtService86_DTCStatusChange_StoreEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_DTCStatusChange_StoreEvent(Dcm_OpStatusType      opStatus
                                                                                            ,uint8                dtcStatusMask
                                                                                            ,Dcm_NegativeResponseCodePtrType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult;

  /* ----- Implementation ----------------------------------------------- */
  Dcm_ExtSvc86RepeaterContextPtrType svc86Context;
  svc86Context = Dcm_ExtGetRepeaterContextSvc86();

  if (Dcm_ExtSvc86HandlerState == DCM_EXT_SVC_86_STATE_WRITE_NVM_DATA)
  {
    /* updating NvM is pending */
    lStdResult = Dcm_ExtSvc86UpdateNvM(opStatus, svc86Context, errorCode);
  }
  else
  {
    /* update NvM */
    Dcm_ExtSvc86OldHandlerState = Dcm_ExtSvc86HandlerState;
    Dcm_ExtSvc86HandlerState = DCM_EXT_SVC_86_STATE_WRITE_NVM_DATA;
    svc86Context->NewNvmData.RoEState = DCM_EXT_SVC_86_STATE_INACTIVE;
    svc86Context->NewNvmData.DTCStatusMask = dtcStatusMask;
    lStdResult = Dcm_ExtSvc86UpdateNvM(opStatus, svc86Context, errorCode);
  }

  return lStdResult;
}
# endif
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/* ----------------------------------------------
 ~&&&   Module internal function implementations
---------------------------------------------- */
#define DCM_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_DCM_UNSUPPORTED XF */
/**********************************************************************************************************************
 *  Dcm_ExtService86_CheckRoEClient()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_CheckRoEClient(Dcm_ReadOnlyMsgContextPtrType   pMsgContext
                                                                        ,Dcm_NegativeResponseCodePtrType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_OK;

  /* ----- Implementation ----------------------------------------------- */
# if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
  Dcm_CfgVarMgrComVariantType pComVariant = Dcm_VarMgrGetActiveComVariant();
# endif
  if(Dcm_NetCfgGetRxPduInfo(pComVariant)[Dcm_ExtSvc86NvMData.RxPduId].ConnRef != Dcm_NetCfgGetRxPduInfo(pComVariant)[pMsgContext->rxPduId].ConnRef)
  {
    /* Request comes from a different tester */
    *errorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_ExtService86_StartRoE()
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
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_StartRoE(Dcm_OpStatusType      opStatus
                                                                  ,Dcm_MsgContextPtrType pMsgContext
                                                                  ,boolean               storeEvent
                                                                  ,Dcm_NegativeResponseCodePtrType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_OK;
  uint8          lEventWindowTime;
  *errorCode = DCM_E_POSITIVERESPONSE;                                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */

  /* ----- Implementation ----------------------------------------------- */
  lEventWindowTime = Dcm_DiagGetReqDataAsU8(pMsgContext);
  if(opStatus == DCM_INITIAL)
  {
    lStdResult = Dcm_ExtService86_CheckEWT(DCM_EXTSVC86_STARTROE_SID, lEventWindowTime, &storeEvent, errorCode);

    /* check if request comes from tester that started RoE */
    if((lStdResult == DCM_E_OK) && (Dcm_ExtSvc86RoEState == DCM_EXT_SVC_86_STATE_ACTIVE))
    {
      lStdResult = Dcm_ExtService86_CheckRoEClient(pMsgContext, errorCode);
    }
  }

  if(lStdResult == DCM_E_OK)
  {
# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
    if(storeEvent == TRUE)
    {
      lStdResult = Dcm_ExtService86_StartRoE_StoreEvent(opStatus, pMsgContext, errorCode);
    }
    else
# endif
    {
      lStdResult = Dcm_ExtService86_StartRoE_Internal(pMsgContext, errorCode);
    }

    if (lStdResult == DCM_E_OK)
    {
      /* notify Dem to start triggering DTC status changes */
      Dcm_DemAPI_ControlDTCStatusChangedNotification(TRUE);

      /* report response data */
      Dcm_DiagProvideResDataAsU8(pMsgContext, 0x00u);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_DiagProvideResDataAsU8(pMsgContext, lEventWindowTime);                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */

    } /* else storing RoE state is pending or DCM_E_NOT_OK (errorCode already set) */
  } /* else DCM_E_NOT_OK (errrorCode already set) */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_ExtService86_StopRoE()
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_StopRoE(Dcm_OpStatusType      opStatus
                                                                 ,Dcm_MsgContextPtrType pMsgContext
                                                                 ,boolean               storeEvent
                                                                 ,Dcm_NegativeResponseCodePtrType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType        lStdResult = DCM_E_OK;
  uint8 lEventWindowTime;
  *errorCode = DCM_E_POSITIVERESPONSE;                                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */

  /* ----- Implementation ----------------------------------------------- */
  lEventWindowTime = Dcm_DiagGetReqDataAsU8(pMsgContext);
  if(opStatus == DCM_INITIAL)
  {
    lStdResult = Dcm_ExtService86_CheckEWT(DCM_EXTSVC86_STOPROE_SID, lEventWindowTime, &storeEvent, errorCode);
  }

  if(lStdResult == DCM_E_OK)
  {
    Dcm_ExtSvc86RoeStateType lOldRoEState = Dcm_ExtSvc86RoEState;
# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
    if(storeEvent == TRUE)
    {
      lStdResult = Dcm_ExtService86_StopRoE_StoreEvent(opStatus, errorCode);
    }
    else
# endif
    {
      lStdResult = Dcm_ExtService86_StopRoE_Internal(errorCode);
    }

    if (lStdResult == DCM_E_OK)
    {
      if(lOldRoEState == DCM_EXT_SVC_86_STATE_ACTIVE)
      {
        /* notify Dem to stop triggering DTC status change */
        Dcm_DemAPI_ControlDTCStatusChangedNotification(FALSE);
      }

      /* report response data */
      Dcm_DiagProvideResDataAsU8(pMsgContext, 0x00u);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_DiagProvideResDataAsU8(pMsgContext, lEventWindowTime);                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */

      /* clear task event */
      Dcm_ExtService86_ClearTaskEvents();
    } /* else storing RoE state is pending or DCM_E_NOT_OK (errorCode already set) */
  } /* else DCM_E_NOT_OK (errorCode already set) */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_ExtService86_StopRoE_Internal()
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
 *
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_StopRoE_Internal(Dcm_NegativeResponseCodePtrType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_OK;

  /* ----- Implementation ----------------------------------------------- */
  switch (Dcm_ExtSvc86HandlerState)
  {
    case DCM_EXT_SVC_86_STATE_ACTIVE:
      /* RoE state is active and stored */
      Dcm_ExtSvc86HandlerState = DCM_EXT_SVC_86_STATE_INACTIVE_STOP_NOT_STORED;
      Dcm_ExtSvc86RoEState = DCM_EXT_SVC_86_STATE_INACTIVE;
      Dcm_RoEOnEventStateChange( DCM_EXT_SVC_86_ROE_EVENTID, Dcm_ExtSvc86RoEState);
      break;
    case DCM_EXT_SVC_86_STATE_ACTIVE_SETUP_NOT_STORED:
      /* RoE state is active and setup not stored */
      Dcm_ExtSvc86HandlerState = DCM_EXT_SVC_86_STATE_INACTIVE_SETUP_NOT_STORED;
      Dcm_ExtSvc86RoEState = DCM_EXT_SVC_86_STATE_INACTIVE;
      Dcm_RoEOnEventStateChange( DCM_EXT_SVC_86_ROE_EVENTID, Dcm_ExtSvc86RoEState);
      break;
    case DCM_EXT_SVC_86_STATE_ACTIVE_SETUP_STORED:
      /* RoE state is active and setup stored */
      Dcm_ExtSvc86HandlerState = DCM_EXT_SVC_86_STATE_INACTIVE;
      Dcm_ExtSvc86RoEState = DCM_EXT_SVC_86_STATE_INACTIVE;
      Dcm_RoEOnEventStateChange( DCM_EXT_SVC_86_ROE_EVENTID, Dcm_ExtSvc86RoEState);
      break;

    case DCM_EXT_SVC_86_STATE_INACTIVE_SETUP_NOT_STORED:
    case DCM_EXT_SVC_86_STATE_INACTIVE_STOP_NOT_STORED:
      /* RoE state is already stopped and not stored */
      break;
    case DCM_EXT_SVC_86_STATE_CLEARED:
      /* Roe is cleared and can not processed in this state */
      lStdResult = DCM_E_NOT_OK;
      *errorCode = DCM_E_REQUESTSEQUENCEERROR;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    default:
      /* Sub-service can not be processed in the current state;current state is X_SETUP_NOT_STORED */
      lStdResult = DCM_E_NOT_OK;
      *errorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
  }

  return lStdResult;
}

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtService86_StopRoE_StoreEvent()
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
 *
 *
 *
 *
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_StopRoE_StoreEvent(Dcm_OpStatusType opStatus, Dcm_NegativeResponseCodePtrType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType        lStdResult = DCM_E_OK;

  /* ----- Implementation ----------------------------------------------- */
  Dcm_ExtSvc86RepeaterContextPtrType svc86Context = Dcm_ExtGetRepeaterContextSvc86();
  switch (Dcm_ExtSvc86HandlerState)
  {
    case DCM_EXT_SVC_86_STATE_ACTIVE:
    case DCM_EXT_SVC_86_STATE_INACTIVE_STOP_NOT_STORED:
      Dcm_ExtSvc86OldHandlerState = Dcm_ExtSvc86HandlerState;
      Dcm_ExtSvc86HandlerState = DCM_EXT_SVC_86_STATE_WRITE_NVM_DATA;
      svc86Context->NewNvmData.RoEState = DCM_EXT_SVC_86_STATE_INACTIVE;
      svc86Context->NewNvmData.DTCStatusMask = Dcm_ExtSvc86DTCStatusMask;
      /* Not important: Dcm_ExtSvc86NewNvMData.RxPduId = pMsgContext->rxPduid; */
      /* Not important: Dcm_ExtSvc86NewNvMData.ClientSrcAddr = Dcm_NetCfgGetTesterAddressOfRxPduId(pComVariant, pMsgContext->rxPduId); */
      /* fall through */
    case DCM_EXT_SVC_86_STATE_WRITE_NVM_DATA:                                                                                                        /* PRQA S 2003 */ /* MD_Dcm_Optimize_2003 */
      lStdResult = Dcm_ExtSvc86UpdateNvM(opStatus, svc86Context, errorCode);
      break;

    case DCM_EXT_SVC_86_STATE_ACTIVE_SETUP_STORED:
      /* Roe state was previously DCM_EXT_SVC_86_STATE_INACTIVE; NvM state must be still DCM_EXT_SVC_86_STATE_INACTIVE, so not necessary to update NvM */
      Dcm_ExtSvc86HandlerState = DCM_EXT_SVC_86_STATE_INACTIVE;
      Dcm_ExtSvc86RoEState = DCM_EXT_SVC_86_STATE_INACTIVE;
      Dcm_RoEOnEventStateChange( DCM_EXT_SVC_86_ROE_EVENTID, Dcm_ExtSvc86RoEState);
      break;
    case DCM_EXT_SVC_86_STATE_INACTIVE:
      /* Roe is already stopped and stored */
      break;
    case DCM_EXT_SVC_86_STATE_CLEARED:
      /* Roe is cleared and can not processed in this state */
      *errorCode = DCM_E_REQUESTSEQUENCEERROR;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      break;
    default:
      /* Sub-service can not be processed in the current state;current state is X_SETUP_NOT_STORED */
      *errorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      break;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtService86_StartRoE_StoreEvent()
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
 *
 *
 *
 *
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_StartRoE_StoreEvent(
                                                                           Dcm_OpStatusType                opStatus
                                                                          ,Dcm_ReadOnlyMsgContextPtrType   pMsgContext
                                                                          ,Dcm_NegativeResponseCodePtrType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_OK;

  /* ----- Implementation ----------------------------------------------- */
  Dcm_ExtSvc86RepeaterContextPtrType svc86Context = Dcm_ExtGetRepeaterContextSvc86();
  switch (Dcm_ExtSvc86HandlerState)
  {
    case DCM_EXT_SVC_86_STATE_INACTIVE:
    case DCM_EXT_SVC_86_STATE_INACTIVE_STOP_NOT_STORED:
      {
#  if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
        Dcm_CfgVarMgrComVariantType pComVariant = Dcm_VarMgrGetActiveComVariant();
#  endif
        Dcm_ExtSvc86OldHandlerState = Dcm_ExtSvc86HandlerState;
        Dcm_ExtSvc86HandlerState = DCM_EXT_SVC_86_STATE_WRITE_NVM_DATA;
        svc86Context->NewNvmData.RoEState      = DCM_EXT_SVC_86_STATE_ACTIVE;
        svc86Context->NewNvmData.DTCStatusMask = Dcm_ExtSvc86DTCStatusMask;
        svc86Context->NewNvmData.RxPduId       = pMsgContext->rxPduId;
        svc86Context->NewNvmData.ClientSrcAddr = Dcm_NetCfgGetTesterAddressOfRxPduId(pComVariant, pMsgContext->rxPduId);
      }
      /* fall through */
    case DCM_EXT_SVC_86_STATE_WRITE_NVM_DATA:                                                                                                        /* PRQA S 2003 */ /* MD_Dcm_Optimize_2003 */
      lStdResult = Dcm_ExtSvc86UpdateNvM(opStatus, svc86Context, errorCode);
      break;

    case DCM_EXT_SVC_86_STATE_ACTIVE:
      /* Roe is already started and stored */
      break;
    case DCM_EXT_SVC_86_STATE_CLEARED:
      /* RoE is cleared and can not processed in this state */
      lStdResult = DCM_E_NOT_OK;
      *errorCode = DCM_E_REQUESTSEQUENCEERROR;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    default:
      /* Sub-service can not be processed in the current state;current state is X_SETUP_NOT_STORED or DCM_EXT_SVC_86_STATE_ACTIVE_SETUP_STORED */
      *errorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      break;
  }

  return lStdResult;
}
# endif
/**********************************************************************************************************************
 *  Dcm_ExtService86_StartRoE_Internal()
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
 *
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_StartRoE_Internal(
                                                                            Dcm_ReadOnlyMsgContextPtrType pMsgContext
                                                                           ,Dcm_NegativeResponseCodePtrType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_OK;

  /* ----- Implementation ----------------------------------------------- */
  switch (Dcm_ExtSvc86HandlerState)
  {
    case DCM_EXT_SVC_86_STATE_INACTIVE:
      /* RoE state is stopped and stored */
      Dcm_ExtSvc86HandlerState = DCM_EXT_SVC_86_STATE_ACTIVE_SETUP_STORED;
      Dcm_ExtSvc86RoEState = DCM_EXT_SVC_86_STATE_ACTIVE;
      Dcm_ExtSvc86NvMData.RxPduId = pMsgContext->rxPduId;
      Dcm_RoEOnEventStateChange( DCM_EXT_SVC_86_ROE_EVENTID, Dcm_ExtSvc86RoEState);
      break;
    case DCM_EXT_SVC_86_STATE_INACTIVE_SETUP_NOT_STORED:
      /* RoE state is stopped and not stored */
      Dcm_ExtSvc86HandlerState = DCM_EXT_SVC_86_STATE_ACTIVE_SETUP_NOT_STORED;
      Dcm_ExtSvc86RoEState = DCM_EXT_SVC_86_STATE_ACTIVE;
      Dcm_ExtSvc86NvMData.RxPduId = pMsgContext->rxPduId;
      Dcm_RoEOnEventStateChange( DCM_EXT_SVC_86_ROE_EVENTID, Dcm_ExtSvc86RoEState);
      break;
    case DCM_EXT_SVC_86_STATE_CLEARED:
      /* RoE is cleared and can not processed in this state */
      lStdResult = DCM_E_NOT_OK;
      *errorCode = DCM_E_REQUESTSEQUENCEERROR;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    case DCM_EXT_SVC_86_STATE_ACTIVE_SETUP_STORED:
    case DCM_EXT_SVC_86_STATE_ACTIVE_SETUP_NOT_STORED:
      /* RoE already started and not stored */
      break;
    default:
      /* Sub-service can not be processed in the current state */
      lStdResult = DCM_E_NOT_OK;
      *errorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_ExtService86_CheckEWT()
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
 *
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_CheckEWT(
                                                                  uint8 eventType
                                                                 ,uint8 eventWindowTime
                                                                 ,P2VAR(boolean, AUTOMATIC, DCM_VAR_NOINIT) storeEvent
                                                                 ,Dcm_NegativeResponseCodePtrType           errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  sint16_least          lLookUpResult;
  Std_ReturnType        lStdResult = DCM_E_OK;

  /* ----- Implementation ----------------------------------------------- */
  lLookUpResult = Dcm_UtiLookUpUint8(Dcm_CfgEventWindowTimeLookUpTable, eventWindowTime);

  if(lLookUpResult < 0)
  {
    /* EWT not supported */
    *errorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else if(eventType != DCM_EXTSVC86_CLEARROE_SID)
  {
    Dcm_SesCtrlType lSession;
    (void)Dcm_GetSesCtrlType(&lSession); /* This API always returns E_OK */

    lStdResult = Dcm_ExtService86_CheckEWT_Internal(lSession, eventType, eventWindowTime, Dcm_CfgEWTStorageStateSupport[lLookUpResult], storeEvent, errorCode);
  }
  else
  {
      /* for SF clearResponseOnEvent only EWT existence is verified; no additional checks needed */
  }

  return lStdResult;
}

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtSvc86UpdateNvM()
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
 *
 *
 *
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtSvc86UpdateNvM(Dcm_OpStatusType opStatus
                                                        ,Dcm_ExtSvc86ReadOnlyRepeaterContextPtrType extSvc86ContextPtr
                                                        ,Dcm_NegativeResponseCodePtrType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult;

  /* ----- Implementation ----------------------------------------------- */
  lStdResult = Dcm_UtiNvMWrite(opStatus, &(extSvc86ContextPtr->NewNvmData), DCM_EXT_SVC_86_NVRAM_BLOCKID);

  if (lStdResult == E_OK)
  {
    /* NvM storage complete; RoE is now inactive/active/clear - update working mirror data */
    Dcm_ExtSvc86HandlerState = extSvc86ContextPtr->NewNvmData.RoEState;

    if(Dcm_ExtSvc86RoEState != Dcm_ExtSvc86HandlerState)
    {
      /* RoE state changed -> notify event handler */
      Dcm_RoEOnEventStateChange(DCM_EXT_SVC_86_ROE_EVENTID, Dcm_ExtSvc86HandlerState);
    }

    Dcm_UtiAssignStruct(Dcm_ExtSvc86NvMData, extSvc86ContextPtr->NewNvmData);

#  if (DCM_SVC_86_RST_ON_DSC_ENABLED == STD_ON)
    Dcm_ExtSvc86DefaultSessionContext.RoEState = Dcm_ExtSvc86RoEState;
    Dcm_ExtSvc86DefaultSessionContext.HandlerState = Dcm_ExtSvc86RoEState;
#  endif

    lStdResult = DCM_E_OK;
  }
  else if (lStdResult == DCM_E_PENDING)
  {
    /* just try later */
  }
  else /* E_NOT_OK */
  {
    /* NvM storage failed; Roe remains active/inactive */
    Dcm_ExtSvc86HandlerState = Dcm_ExtSvc86OldHandlerState;
    *errorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}
# endif

/**********************************************************************************************************************
 *  Dcm_ExtService86_OnDTCStatusChange()
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
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtService86_OnDTCStatusChange(Dcm_OpStatusType      opStatus
                                                                           ,Dcm_MsgContextPtrType pMsgContext
                                                                           ,boolean storeEvent
                                                                           ,Dcm_NegativeResponseCodePtrType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_OK;
  uint8 lEventWindowTime;
  uint8 lDtcStatusMask;
  uint16 lSTRT; /* serviceToResponseTo */
  *errorCode = DCM_E_POSITIVERESPONSE;                                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */

  /* ----- Implementation ----------------------------------------------- */
  lEventWindowTime = Dcm_DiagGetReqDataAsU8Rel(pMsgContext, 0);
  lDtcStatusMask = Dcm_DiagGetReqDataAsU8Rel(pMsgContext, 1);
  lSTRT = Dcm_DiagGetReqDataAsU16Rel(pMsgContext, 2);

  if(opStatus == DCM_INITIAL)
  {
    lStdResult = Dcm_ExtService86_CheckEWT(DCM_EXTSVC86_ONDTCSTATUSCHANGE_SID, lEventWindowTime, &storeEvent, errorCode);

    /* only Service 0x19 0E is accepted as serviceToResponseTo */
    if((lStdResult == DCM_E_OK)
      && (lSTRT != 0x190Eu))
    {
      lStdResult = DCM_E_NOT_OK;
      *errorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
    }

    /* check if request comes from tester that started RoE */
    if((lStdResult == DCM_E_OK) && (Dcm_ExtSvc86RoEState == DCM_EXT_SVC_86_STATE_ACTIVE))
    {
      lStdResult = Dcm_ExtService86_CheckRoEClient(pMsgContext, errorCode);
    }
  }

  if(lStdResult == DCM_E_OK)
  {
     Dcm_ExtSvc86RoeStateType lOldRoEState = Dcm_ExtSvc86RoEState;
# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
    if(storeEvent == TRUE)
    {
      lStdResult = Dcm_ExtService86_DTCStatusChange_StoreEvent(opStatus, lDtcStatusMask, errorCode);
    }
    else
# endif
    {
      if(Dcm_ExtSvc86RoEState != DCM_EXT_SVC_86_STATE_INACTIVE)
      {
        /* If RoE was started or cleared RoE is stopped */
        Dcm_ExtSvc86RoEState = DCM_EXT_SVC_86_STATE_INACTIVE;
        Dcm_RoEOnEventStateChange(DCM_EXT_SVC_86_ROE_EVENTID, Dcm_ExtSvc86RoEState);
      }

      Dcm_ExtSvc86HandlerState = DCM_EXT_SVC_86_STATE_INACTIVE_SETUP_NOT_STORED;
      Dcm_ExtSvc86DTCStatusMask = lDtcStatusMask;
    }

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
    if (lStdResult == DCM_E_OK)
# endif
    {
      if(lOldRoEState == DCM_EXT_SVC_86_STATE_ACTIVE)
      {
        /* notify Dem to stop triggering DTC status change */
        Dcm_DemAPI_ControlDTCStatusChangedNotification(FALSE);
      }

      /* report response data */
      Dcm_DiagProvideResDataAsU8(pMsgContext, 0x00u);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_DiagProvideResDataAsU8(pMsgContext, lEventWindowTime);                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_DiagProvideResDataAsU8(pMsgContext, lDtcStatusMask);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_DiagProvideResDataAsU16(pMsgContext, lSTRT);                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */

      Dcm_ExtService86_ClearTaskEvents();
    }/* else storing RoE state is pending or DCM_E_NOT_OK (errorCode already set) */
  } /* else DCM_E_NOT_OK (errorCode already set) */

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Dcm_ExtSvc86_ClearRoE()
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
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_ExtSvc86_ClearRoE(Dcm_OpStatusType opStatus
                                                              ,Dcm_MsgContextPtrType pMsgContext
                                                              ,Dcm_NegativeResponseCodePtrType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_OK;
  uint8 lEventWindowTime;
  boolean dummyStoreEvent = TRUE;
  *errorCode = DCM_E_POSITIVERESPONSE;                                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */

  /* ----- Implementation ----------------------------------------------- */
  lEventWindowTime = Dcm_DiagGetReqDataAsU8(pMsgContext);
  if(opStatus == DCM_INITIAL)
  {
    lStdResult = Dcm_ExtService86_CheckEWT(DCM_EXTSVC86_CLEARROE_SID, lEventWindowTime, &dummyStoreEvent, errorCode);
  }

  if(lStdResult == DCM_E_OK)
  {
    Dcm_ExtSvc86RoeStateType lOldRoEState = Dcm_ExtSvc86RoEState;
    if(Dcm_ExtSvc86RoEState == DCM_EXT_SVC_86_STATE_CLEARED)
    {
      /* RoE state is already cleared */

      /* report response data */
      Dcm_DiagProvideResDataAsU8(pMsgContext, 0x00u);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_DiagProvideResDataAsU8(pMsgContext, lEventWindowTime);                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
      Dcm_ExtSvc86RepeaterContextPtrType svc86Context;

      svc86Context = Dcm_ExtGetRepeaterContextSvc86();

      if(Dcm_ExtSvc86HandlerState == DCM_EXT_SVC_86_STATE_WRITE_NVM_DATA)
      {
        /* NvMUpdate is pending */
        lStdResult = Dcm_ExtSvc86UpdateNvM(opStatus, svc86Context, errorCode);
      }
      else
      {
        /* Update in NvM */
        Dcm_ExtSvc86OldHandlerState = Dcm_ExtSvc86HandlerState;
        Dcm_ExtSvc86HandlerState = DCM_EXT_SVC_86_STATE_WRITE_NVM_DATA;
        svc86Context->NewNvmData.RoEState = DCM_EXT_SVC_86_STATE_CLEARED;
        lStdResult = Dcm_ExtSvc86UpdateNvM(opStatus, svc86Context, errorCode);
      }
# else
      Dcm_ExtSvc86OldHandlerState = DCM_EXT_SVC_86_STATE_CLEARED;
      Dcm_ExtSvc86RoEState = DCM_EXT_SVC_86_STATE_CLEARED;
      Dcm_RoEOnEventStateChange(DCM_EXT_SVC_86_ROE_EVENTID, Dcm_ExtSvc86RoEState);
# endif

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
      if (lStdResult == DCM_E_OK)
# endif
      {
        /* report response data */
        Dcm_DiagProvideResDataAsU8(pMsgContext, 0x00u);                                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
        Dcm_DiagProvideResDataAsU8(pMsgContext, lEventWindowTime);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */

        if (lOldRoEState == DCM_EXT_SVC_86_STATE_ACTIVE)
        {
          /* notify Dem to stop triggering DTC status change */
          Dcm_DemAPI_ControlDTCStatusChangedNotification(FALSE);
        }

        /* clear task event */
        Dcm_ExtService86_ClearTaskEvents();
      }/* else storing RoE state is pending or DCM_E_NOT_OK (errorCode already set) */
    }
  } /* else DCM_E_NOT_OK (errorCode already set) */

  return lStdResult;
}
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/* ----------------------------------------------
 ~&&&   Module API function implementations
---------------------------------------------- */
#define DCM_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_DCM_UNSUPPORTED XF */
/**********************************************************************************************************************
 *  Dcm_DemTriggerOnDTCStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_DemTriggerOnDTCStatus(uint32 DTC, uint8  DTCStatusOld, uint8  DTCStatusNew)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (DCM_DEV_ERROR_DETECT == STD_ON)
  if(Dcm_DebugIsModuleInitialized() == FALSE)
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation --------------------------------------- */
    if( (Dcm_ExtSvc86RoEState == DCM_EXT_SVC_86_STATE_ACTIVE)
      &&(((DTCStatusOld ^ DTCStatusNew) & Dcm_ExtSvc86DTCStatusMask) != 0) )
    {
      /* bit changes 0->1 or 1->0 */

      /* Check ComM state */
      if(Dcm_NetGetComState(Dcm_ExtSvc86NvMData.RxPduId) == DCM_NET_COMM_STATE_RX_EN_TX_EN)
      {
        /* trigger RoE */
        Dcm_TskSetEvent(DCM_TSK_ID_ROE, DCM_EXT_TSK_EV_ROE_SEND);
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DCM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != DCM_E_NO_ERROR)
  {
    Dcm_DebugReportError(DCM_SID_DEMTRIGGERONDTCSTATUS, lErrorId);
  }
# else
  DCM_IGNORE_UNREF_PARAM(lErrorId);                                                                                                                  /* PRQA S 3112 */ /* MD_Dcm_3112 */
# endif

  DCM_IGNORE_UNREF_PARAM(DTC);                                                                                                                       /* PRQA S 3112 */ /* MD_Dcm_3112 */

  return DCM_E_OK;
}
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/* ----------------------------------------------
 ~&&&   Module internal API function implementations
---------------------------------------------- */
#define DCM_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_DCM_UNSUPPORTED XF */
/**********************************************************************************************************************
 *  Dcm_ExtService86Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ExtService86Init(void)
{
  /* set state to cleared; reading NvM in first task cycle */
  Dcm_ExtSvc86HandlerState = DCM_EXT_SVC_86_STATE_CLEARED;
  Dcm_ExtSvc86ReadTimeout  = DCM_EXT_SVC_86_READ_TIMEOUT;
  Dcm_ExtSvc86NvMReadOpStatus = DCM_INITIAL;
  Dcm_ExtSvc86STRTSendingState = DCM_EXT_SVC_86_STRT_STATE_IDLE;

  /* set default values for NvM data */
  Dcm_ExtSvc86NvMData.RoEState   = DCM_EXT_SVC_86_STATE_CLEARED;
  Dcm_ExtSvc86NvMData.RxPduId = 0;

  Dcm_TskSetEvent(DCM_TSK_ID_ROE, DCM_EXT_TSK_EV_ROE_ACTIVATE);
}

/**********************************************************************************************************************
 *  Dcm_ExtSvc86Task()
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ExtSvc86Task(Dcm_TskTaskEvOptType ev
                                     ,Dcm_TskTaskEvPtrType pPostEv)
{
# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
  /* check if configured event occurred */
  if (Dcm_TskIsLocalEventSet(ev, DCM_EXT_TSK_EV_ROE_ACTIVATE))
  {
    Dcm_ExtSvc86ReadNvM(pPostEv);
  }
# endif

  if (Dcm_TskIsLocalEventSet(ev, DCM_EXT_TSK_EV_ROE_TX_END))
  {
# if (DCM_SVC_86_TIMER_ENABLED == STD_ON)
    /* response transmission triggered; restart timer */
    Dcm_TmrStartTimer(DCM_TMR_ID_SVC86_INTERMESSAGETIME, DCM_EXT_SVC_86_INTERMESSAGETIME);
    Dcm_ExtSvc86STRTSendingState = DCM_EXT_SVC_86_STRT_STATE_WAIT_FOR_TIMEOUT;
# else
    /* response transmission triggered; next ServicToResponseTo can be sent */
    Dcm_ExtSvc86STRTSendingState = DCM_EXT_SVC_86_STRT_STATE_IDLE;
# endif
  }

  if(Dcm_TskIsLocalEventSet(ev, DCM_EXT_TSK_EV_ROE_SEND))
  {
    /* RoE event was triggered by Dem */
    if(Dcm_ExtSvc86RoEState == DCM_EXT_SVC_86_STATE_ACTIVE)
    {
      /* RoE is Active */

      /* Check ComM state */
      if(Dcm_NetGetComState(Dcm_ExtSvc86NvMData.RxPduId) == DCM_NET_COMM_STATE_RX_EN_TX_EN)
      {
        /* STRT state is IDLE */
        if (Dcm_ExtSvc86STRTSendingState == DCM_EXT_SVC_86_STRT_STATE_IDLE)
        {
          /* STRT state is set to SENDING -> next event will be queued */
          Dcm_ExtSvc86STRTSendingState = DCM_EXT_SVC_86_STRT_STATE_SENDING;
          Dcm_ExtService86_Send_STRT(pPostEv);
        }
        else
        {
          /* Try next task cycle */
          Dcm_TskSetLocalEvent(*pPostEv, DCM_EXT_TSK_EV_ROE_SEND);
        }
      }
    }
  }
}

# if (DCM_SVC_86_TIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtOnTimeoutSvc86InterMessageTime()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_ExtOnTimeoutSvc86InterMessageTime(void)
{
  Dcm_ExtSvc86STRTSendingState = DCM_EXT_SVC_86_STRT_STATE_IDLE;
  return 0u;/* no reload */
}
# endif

# if (DCM_SVC_86_RST_ON_DSC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtSvc86OnChangeSession()
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
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ExtSvc86OnChangeSession(Dcm_CfgStateGroupOptType oldSession
                                                ,Dcm_CfgStateGroupOptType newSession)
{
  if ( (DCM_STATE_VALUE_SESSION_INIT & (oldSession & newSession)) == 0)
  {
    /* at least one session is non-default */
    Dcm_NegativeResponseCodeType errorCode = DCM_E_POSITIVERESPONSE;

    if ((DCM_STATE_VALUE_SESSION_INIT & oldSession) != 0)
    {
      /* default session is left; save context */
      Dcm_ExtSvc86DefaultSessionContext.RoEState = Dcm_ExtSvc86RoEState;
      Dcm_ExtSvc86DefaultSessionContext.HandlerState = Dcm_ExtSvc86HandlerState;
      Dcm_ExtSvc86DefaultSessionContext.RxPduId = Dcm_ExtSvc86NvMData.RxPduId;
    }

    if (((DCM_STATE_VALUE_SESSION_INIT & newSession) != 0) && (Dcm_ExtSvc86DefaultSessionContext.RoEState == DCM_EXT_SVC_86_STATE_ACTIVE))
    {
      /* RoE was started when leaving default session -> restart RoE */
      if(Dcm_ExtSvc86RoEState != Dcm_ExtSvc86DefaultSessionContext.RoEState)
      {
        Dcm_RoEOnEventStateChange(DCM_EXT_SVC_86_ROE_EVENTID, Dcm_ExtSvc86DefaultSessionContext.RoEState);
      }

      /* restore default session context */
      Dcm_ExtSvc86RoEState = Dcm_ExtSvc86DefaultSessionContext.RoEState;
      Dcm_ExtSvc86HandlerState = Dcm_ExtSvc86DefaultSessionContext.HandlerState;
      Dcm_ExtSvc86NvMData.RxPduId = Dcm_ExtSvc86DefaultSessionContext.RxPduId;
    }
    else if(Dcm_ExtSvc86RoEState == DCM_EXT_SVC_86_STATE_ACTIVE)
    {
      /* stop RoE on session change if it was started before; Dcm_ExtService86_StopRoE_Internal() returns DCM_E_OK if RoE state is active */
      (void) Dcm_ExtService86_StopRoE_Internal(&errorCode);
    }
    else
    {
      /* nothing to do */
    }
  }
}
# endif
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/* ----------------------------------------------
 ~&&&   Module call-out implementations
---------------------------------------------- */
#define DCM_START_SEC_CALLOUT_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_DCM_UNSUPPORTED XF */
# if (DCM_SVC_86_00_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtService86_00Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_ExtService86_00Processor(Dcm_OpStatusType opStatus
                                                                   ,Dcm_MsgContextPtrType pMsgContext
                                                                   ,Dcm_NegativeResponseCodePtrType ErrorCode)
{
  Std_ReturnType lStdResult;

  lStdResult = Dcm_ExtService86_StopRoE(opStatus, pMsgContext, FALSE, ErrorCode);

  return lStdResult;
}
# endif

# if (DCM_SVC_86_40_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtService86_40Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_ExtService86_40Processor(Dcm_OpStatusType opStatus
                                                                   ,Dcm_MsgContextPtrType pMsgContext
                                                                   ,Dcm_NegativeResponseCodePtrType ErrorCode)
{
  Std_ReturnType lStdResult;

  lStdResult = Dcm_ExtService86_StopRoE(opStatus, pMsgContext, TRUE, ErrorCode);

  return lStdResult;
}
# endif

# if (DCM_SVC_86_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtService86_01Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_ExtService86_01Processor(Dcm_OpStatusType opStatus
                                                                   ,Dcm_MsgContextPtrType pMsgContext
                                                                   ,Dcm_NegativeResponseCodePtrType ErrorCode)
{
  Std_ReturnType lStdResult;

  lStdResult = Dcm_ExtService86_OnDTCStatusChange(opStatus, pMsgContext, FALSE, ErrorCode);

  return lStdResult;
}
# endif

# if (DCM_SVC_86_41_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtService86_41Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_ExtService86_41Processor(Dcm_OpStatusType opStatus
                                                                   ,Dcm_MsgContextPtrType pMsgContext
                                                                   ,Dcm_NegativeResponseCodePtrType ErrorCode)
{
  Std_ReturnType lStdResult;

  lStdResult = Dcm_ExtService86_OnDTCStatusChange(opStatus, pMsgContext, TRUE, ErrorCode);

  return lStdResult;
}
# endif

# if (DCM_SVC_86_05_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtService86_05Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_ExtService86_05Processor(Dcm_OpStatusType opStatus
                                                                   ,Dcm_MsgContextPtrType pMsgContext
                                                                   ,Dcm_NegativeResponseCodePtrType ErrorCode)
{
  Std_ReturnType lStdResult;

  lStdResult = Dcm_ExtService86_StartRoE(opStatus, pMsgContext, FALSE, ErrorCode);

  return lStdResult;
}
# endif

# if (DCM_SVC_86_45_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtService86_45Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_ExtService86_45Processor(Dcm_OpStatusType opStatus
                                                                   ,Dcm_MsgContextPtrType pMsgContext
                                                                   ,Dcm_NegativeResponseCodePtrType ErrorCode)
{
  Std_ReturnType lStdResult;

  lStdResult = Dcm_ExtService86_StartRoE(opStatus, pMsgContext, TRUE, ErrorCode);

  return lStdResult;
}
# endif

# if (DCM_SVC_86_06_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtService86_06Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_ExtService86_06Processor(Dcm_OpStatusType opStatus
                                                                   ,Dcm_MsgContextPtrType pMsgContext
                                                                   ,Dcm_NegativeResponseCodePtrType ErrorCode)
{
  Std_ReturnType lStdResult;

  lStdResult = Dcm_ExtSvc86_ClearRoE(opStatus, pMsgContext, ErrorCode);

  return lStdResult;
}
# endif

# if (DCM_SVC_86_46_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtService86_46Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_ExtService86_46Processor(Dcm_OpStatusType opStatus
                                                                   ,Dcm_MsgContextPtrType pMsgContext
                                                                   ,Dcm_NegativeResponseCodePtrType ErrorCode)
{
  Std_ReturnType lStdResult;

  lStdResult = Dcm_ExtSvc86_ClearRoE(opStatus, pMsgContext, ErrorCode);

  return lStdResult;
}
# endif
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CALLOUT_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
/* ********************************************************************************************************************
 * END OF FILE: Dcm_Ext.c
 * ******************************************************************************************************************** */
