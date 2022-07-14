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
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file  vCanCcGbt.c
 *  \brief  vCanCcGbt
 *
 *  \details  Implementation of vCanCcGbt.
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

/* PRQA S 715 EOF */

/* *INDENT-ON* */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#define VCANCCGBT_C

#include "vCanCcGbt.h"
#include "vCanCcGbt_Cbk.h"
#include "vCanCcGbt_Int.h"
#include "vCanCcGbt_Types.h"
#include "vCanCcGbt_Callout.h"

#include "vCanCcGbt_Cfg.h"
#include "vCanCcGbt_Lcfg.h"
#include "vCanCcGbt_PBcfg.h"

#include "PduR_vCanCcGbt.h"
#include "SchM_vCanCcGbt.h"

#include "vstdlib.h"

#if (VCANCCGBT_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
# include "EcuM_Error.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Vendor specific BSW module version information */
#define VCANCCGBT_MAJOR_SOURCE_VERSION          (1u)
#define VCANCCGBT_MINOR_SOURCE_VERSION          (0u)
#define VCANCCGBT_PATCH_SOURCE_VERSION          (2u)

/* Vendor specific BSW generator version information */
#define VCANCCGBT_MAJOR_GENERATOR_VERSION       (1u)
#define VCANCCGBT_MINOR_GENERATOR_VERSION       (0u)

/* Check the vendor specific version of vCanCcGbt module header file */
#if ((VCANCCGBT_SW_MAJOR_VERSION != VCANCCGBT_MAJOR_SOURCE_VERSION) || (VCANCCGBT_SW_MINOR_VERSION != VCANCCGBT_MINOR_SOURCE_VERSION) || (VCANCCGBT_SW_PATCH_VERSION != VCANCCGBT_PATCH_SOURCE_VERSION))
# error "Vendor specific version numbers of vCanCcGbt.c and vCanCcGbt.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if ((VCANCCGBT_CFG_MAJOR_VERSION != VCANCCGBT_MAJOR_GENERATOR_VERSION) || (VCANCCGBT_CFG_MINOR_VERSION != VCANCCGBT_MINOR_GENERATOR_VERSION))
# error "Vendor specific version numbers of vCanCcGbt.c and vCanCcGbt_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Global module state */
#define VCANCCGBT_UNINIT (0)
#define VCANCCGBT_INIT (1)

#if !defined (VCANCCGBT_LOCAL)
# define VCANCCGBT_LOCAL static
#endif

#if !defined (VCANCCGBT_LOCAL_INLINE)
# define VCANCCGBT_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
/*===================================== VCANCCGBT_START_SEC_VAR_ZERO_INIT_8BIT ======================================*/
#define VCANCCGBT_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Initialization state of the vCanCcGbt */
VCANCCGBT_LOCAL VAR(uint8, VCANCCGBT_VAR_ZERO_INIT) vCanCcGbt_ModuleInitialized = VCANCCGBT_UNINIT;

#define VCANCCGBT_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*======================================= VCANCCGBT_STOP_SEC_VAR_ZERO_INIT_8BIT =====================================*/

/**********************************************************************************************************************
 *  INTERNAL DATA
 *********************************************************************************************************************/
/*=================================== VCANCCGBT_START_SEC_VAR_NOINIT_UNSPECIFIED ====================================*/
#define VCANCCGBT_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Charge state of the vCanCcGbt */
VCANCCGBT_LOCAL VAR(vCanCcGbt_ChargeStateType, VCANCCGBT_VAR_NOINIT) vCanCcGbt_ChargeState;

#define VCANCCGBT_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*==================================== VCANCCGBT_STOP_SEC_VAR_NOINIT_UNSPECIFIED =====================================*/

/*=========================================== VCANCCGBT_START_SEC_VAR_PBCFG ==========================================*/
#define VCANCCGBT_START_SEC_VAR_PBCFG
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Post-build configuration data of the vCanCcGbt */
#if (VCANCCGBT_USE_INIT_POINTER == STD_ON)
VCANCCGBT_LOCAL P2CONST(vCanCcGbt_ConfigType, VCANCCGBT_VAR_PBCFG, VCANCCGBT_PBCFG) vCanCcGbt_ConfigDataPtr;
#endif

#define VCANCCGBT_STOP_SEC_VAR_PBCFG
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*=========================================== VCANCCGBT_STOP_SEC_VAR_PBCFG ===========================================*/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/*============================================== VCANCCGBT_START_SEC_CODE ============================================*/
#define VCANCCGBT_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalCompareData
 *********************************************************************************************************************/
/*! \brief       Compares two given data areas.
 *  \details     Both data areas must have the same length.
 *  \param[in]   currentdata  First data to compare
 *  \param[in]   newData      Second data to compare
 *  \param[in]   dataLength   Length of the data
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(boolean, VCANCCGBT_CODE) vCanCcGbt_InternalCompareData(const uint8 * currentdata,
                                                                            const uint8 * newData, uint8 dataLength);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSet2ByteDataLittleEndian()
 *********************************************************************************************************************/
/*! \brief       Function converts the given value from host byte order to network byte order.
 *  \details     Can be used for two byte values.
 *  \param[in]   value       The value to copy to the destination buffer in little endian format.
 *  \param[in]   destBuffer  Destination buffer for the data.
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \return      E_OK - success
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSet2ByteDataLittleEndian(uint16 value,
                                                                                      P2VAR(uint8, AUTOMATIC,
                                                                                            VCANCCGBT_APPL_DATA)
                                                                                      destBuffer);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSet3ByteDataLittleEndian()
 *********************************************************************************************************************/
/*! \brief       Function converts the given value from host byte order to network byte order.
 *  \details     Can be used for two byte values.
 *  \param[in]   value       The value to copy to the destination buffer in little endian format.
 *  \param[in]   destBuffer  Destination buffer for the data.
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \return      E_OK - success
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSet3ByteDataLittleEndian(uint32 value,
                                                                                      P2VAR(uint8, AUTOMATIC,
                                                                                            VCANCCGBT_APPL_DATA)
                                                                                      destBuffer);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSet4ByteDataLittleEndian()
 *********************************************************************************************************************/
/*! \brief       Function converts the given value from host byte order to network byte order.
 *  \details     Can be used for two byte values.
 *  \param[in]   value       The value to copy to the destination buffer in little endian format.
 *  \param[in]   destBuffer  Destination buffer for the data.
 *  \return      E_NOT_OK - function has been called with invalid parameters.
 *  \return      E_OK - success
 *  \pre         Module is initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSet4ByteDataLittleEndian(uint32 value,
                                                                                      P2VAR(uint8, AUTOMATIC,
                                                                                            VCANCCGBT_APPL_DATA)
                                                                                      destBuffer);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalResetTpPDU
 *********************************************************************************************************************/
/*! \brief       Reset the state of a Tx PDU.
 *  \details     -
 *  \param[in]   pduId   Id of the PDU to reset
 *  \pre         This function must be called with interrupt disabled.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalResetTpPDU(uint8 pduId);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandleTpPduSendTimeout
 *********************************************************************************************************************/
/*! \brief       This function handles the send timeout of the Tp Tx PDUs.
 *  \details     -
 *  \param[in]   pduState    Current state of a Tx PDU.
 *  \return      TRUE:       Timeout has expired.
 *               FALSE:      Timeout has not expired.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(boolean,
                     VCANCCGBT_CODE) vCanCcGbt_InternalHandleTpPduSendTimeout(vCanCcGbt_TpTxPduStateType * pduState);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandleIfPduSendTimeout
 *********************************************************************************************************************/
/*! \brief       This function handles the send timeout of the If Tx PDUs.
 *  \details     -
 *  \param[in]   pduState    Current state of a Tx PDU.
 *  \return      TRUE:       Timeout has expired.
 *               FALSE:      Timeout has not expired.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(boolean,
                     VCANCCGBT_CODE) vCanCcGbt_InternalHandleIfPduSendTimeout(vCanCcGbt_IfTxPduStateType * pduState);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandleSendTimeout
 *********************************************************************************************************************/
/*! \brief       This function handles the send timeout of the Tx PDUs.
 *  \details     -
 *  \param[in]   pduState    Current state of a Tx PDU.
 *  \return      TRUE:       Timeout has expired.
 *               FALSE:      Timeout has not expired.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(boolean, VCANCCGBT_CODE) vCanCcGbt_InternalHandleSendTimeout(uint16 * sendTimeout);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSendTp
 *********************************************************************************************************************/
/*! \brief       Transmission of a given Tp PDU.
 *  \details     -
 *  \param[in]   data        Payload of the PDU
 *  \param[in]   dataLength  Size of the payload
 *  \param[in]   pduId       Id of the PDU
 *  \return      E_OK:       Transmission was successfull.
 *               E_NOT_OK:   Transmission was not successfull.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(Std_ReturnType, VCANCCGBT_CODE) vCanCcGbt_InternalSendTp(uint8 * data, uint16 dataLength,
                                                                              uint8 txPduIdx);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSendIf
 *********************************************************************************************************************/
/*! \brief       Transmission of a given If PDU.
 *  \details     -
 *  \param[in]   data        Payload of the PDU
 *  \param[in]   dataLength  Size of the payload
 *  \param[in]   pduId       Id of the PDU
 *  \return      E_OK:       Transmission was successfull.
 *               E_NOT_OK:   Transmission was not successfull.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(Std_ReturnType, VCANCCGBT_CODE) vCanCcGbt_InternalSendIf(uint8 * data, uint16 dataLength,
                                                                              uint8 txPduIdx);

/***********************************************************************************************************************
 *  vCanCcGbt_InternalSetMandatoryInitialChargingParameter()
 *********************************************************************************************************************/
/*! \brief        Function to set the mandatory initial application specific parameter needed for charging.
 *  \details      -
 *  \param[in]    chargingParameter  Struct with input parameter from Application.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE)
vCanCcGbt_InternalSetMandatoryInitialChargingParameter(vCanCcGbt_MandatoryInitialChargingParameterType
                                                       chargingParameter);

/***********************************************************************************************************************
 *  vCanCcGbt_InternalSetOptionalInitialChargingParameter()
 *********************************************************************************************************************/
/*! \brief        Function to set the optional initial application specific parameter needed for charging.
 *  \details      -
 *  \param[in]    chargingParameter  Struct with input parameter from Application.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE)
vCanCcGbt_InternalSetOptionalInitialChargingParameter(vCanCcGbt_OptionalInitialChargingParameterType chargingParameter);

/***********************************************************************************************************************
 *  vCanCcGbt_InternalSetParameterExchangeData()
 *********************************************************************************************************************/
/*! \brief        Function to set the parameter exchange data.
 *  \details      -
 *  \param[in]    chargingParameter  Struct with input parameter from Application.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE)
vCanCcGbt_InternalSetParameterExchangeData(vCanCcGbt_MandatoryParameterExchangeDataType chargingParameter);

/***********************************************************************************************************************
 *  vCanCcGbt_InternalSetBCLData()
 *********************************************************************************************************************/
/*! \brief        Function to set the BCL message data.
 *  \details      -
 *  \param[in]    bclData  Struct with input parameter from Application.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSetBCLData(vCanCcGbt_BCLDataType bclData);

/***********************************************************************************************************************
 *  vCanCcGbt_InternalSetBCSData()
 *********************************************************************************************************************/
/*! \brief        Function to set the BCS message data.
 *  \details      -
 *  \param[in]    bcsData  Struct with input parameter from Application.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSetBCSData(vCanCcGbt_BCSDataType bcsData);

/***********************************************************************************************************************
 *  vCanCcGbt_InternalSetBSMData()
 *********************************************************************************************************************/
/*! \brief        Function to set the BSM message data.
 *  \details      -
 *  \param[in]    bsmData  Struct with input parameter from Application.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSetBSMData(vCanCcGbt_BSMDataType bsmData);

/***********************************************************************************************************************
 *  vCanCcGbt_InternalSetBMVData()
 *********************************************************************************************************************/
/*! \brief        Function to set the BMV message data.
 *  \details      -
 *  \param[in]    bmvData  Struct with input parameter from Application.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSetBMVData(vCanCcGbt_BMVDataType bmvData);

/***********************************************************************************************************************
 *  vCanCcGbt_InternalSetBMTData()
 *********************************************************************************************************************/
/*! \brief        Function to set the BMT message data.
 *  \details      -
 *  \param[in]    bmtData  Struct with input parameter from Application.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSetBMTData(vCanCcGbt_BMTDataType bmtData);

/***********************************************************************************************************************
 *  vCanCcGbt_InternalSetBSPData()
 *********************************************************************************************************************/
/*! \brief        Function to set the BSP message data.
 *  \details      -
 *  \param[in]    bmtData  Struct with input parameter from Application.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSetBSPData(vCanCcGbt_BSPDataType bspData);

/***********************************************************************************************************************
 *  vCanCcGbt_InternalSetBSTData()
 *********************************************************************************************************************/
/*! \brief        Function to set the BMV message data.
 *  \details      -
 *  \param[in]    bstData  Struct with input parameter from Application.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSetBSTData(vCanCcGbt_BSTDataType bstData);

/***********************************************************************************************************************
 *  vCanCcGbt_InternalSetBSDData()
 *********************************************************************************************************************/
/*! \brief        Function to set the BSD message data.
 *  \details      -
 *  \param[in]    bstData  Struct with input parameter from Application.
 *  \pre          Module is initialized.
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSetBSDData(vCanCcGbt_BSDDataType bsdData);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandleMsgAction
 *********************************************************************************************************************/
/*! \brief       Handles the message action
 *  \details     -
 *  \param[in]   currentState Current state
 *  \param[in]   msgAction  action type
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalHandleMsgAction(vCanCcGbt_StateType currentState,
                                                                                    vCanCcGbt_MsgActionType msgAction);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalStateReset
 *********************************************************************************************************************/
/*! \brief       Reset the variable of the given state
 *  \details     -
 *  \param[in]   state    The state to reset
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalStateReset(vCanCcGbt_StateType state);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalStateInit
 *********************************************************************************************************************/
/*! \brief       Functinos initialize a state after a state transition.
 *  \details     -
 *  \param[in]   state     The state to initialize
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalStateInit(vCanCcGbt_StateType state);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalStateChange
 *********************************************************************************************************************/
/*! \brief       Functinos changes the state of the charging process.
 *  \details     -
 *  \param[in]   oldState     The old state
 *  \param[in]   newState     The new state
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalStateChange(vCanCcGbt_StateType oldState,
                                                                         vCanCcGbt_StateType newState);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandleTimeoutSupervision
 *********************************************************************************************************************/
/*! \brief       Handles the timeout supervision of Rx PDUs.
 *  \details     -
 *  \param[in]   timeout
 *  \param[in]   type
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalHandleTimeoutSupervision(uint16 * timeout,
                                                                                       vCanCcGbt_TimeoutSupervisionType
                                                                                       type);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalProtInit
 *********************************************************************************************************************/
/*! \brief       Handles the internal protocol initialization phase.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalProtInit(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandshakeRxChm
 *********************************************************************************************************************/
/*! \brief       Handles the Handshake Rx message CHM.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalHandshakeRxChm(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandshakeRxCrm
 *********************************************************************************************************************/
/*! \brief       Handles the Handshake Rx message CRM.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalHandshakeRxCrm(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandshakeRx
 *********************************************************************************************************************/
/*! \brief       Handles the Handshake Rx state.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalHandshakeRx(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandshakeTxBhm
 *********************************************************************************************************************/
/*! \brief       Handles the Handshake Tx message BHM.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalHandshakeTxBhm(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandshakeTxBrm
 *********************************************************************************************************************/
/*! \brief       Handles the Handshake Tx message BRM.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalHandshakeTxBrm(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandshakeTx
 *********************************************************************************************************************/
/*! \brief       Handles the Handshake Tx state.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalHandshakeTx(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalParamExRxCts
 *********************************************************************************************************************/
/*! \brief       Handles the parameter exchange rx message CTS.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalParamExRxCts(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalParamExRxCml
 *********************************************************************************************************************/
/*! \brief       Handles the parameter exchange rx message CML.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalParamExRxCml(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalParamExRxCro
 *********************************************************************************************************************/
/*! \brief       Handles the parameter exchange rx message CRO.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalParamExRxCro(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalParamExRx
 *********************************************************************************************************************/
/*! \brief       Handles the parameter exchange rx state.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalParamExRx(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalParamExTxBcp
 *********************************************************************************************************************/
/*! \brief       Handles the parameter exchange tx message BCP.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalParamExTxBcp(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalParamExTxBro
 *********************************************************************************************************************/
/*! \brief       Handles the parameter exchange tx message BRO.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalParamExTxBro(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalParamExTx
 *********************************************************************************************************************/
/*! \brief       Handles the parameter exchange tx state.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalParamExTx(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingRxCcs
 *********************************************************************************************************************/
/*! \brief       Handles the charging rx message CCS.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingRxCcs(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingRxCst
 *********************************************************************************************************************/
/*! \brief       Handles the charging rx message CST.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingRxCst(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingRx
 *********************************************************************************************************************/
/*! \brief       Handles the charging rx state.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingRx(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingTxBcl
 *********************************************************************************************************************/
/*! \brief       Handles the charging tx message BCL.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingTxBcl(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingTxBcs
 *********************************************************************************************************************/
/*! \brief       Handles the charging tx message BCS.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingTxBcs(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingTxBsm
 *********************************************************************************************************************/
/*! \brief       Handles the charging tx message BSM.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingTxBsm(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingTxBmv
 *********************************************************************************************************************/
/*! \brief       Handles the charging tx message BMV.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingTxBmv(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingTxBmt
 *********************************************************************************************************************/
/*! \brief       Handles the charging tx message BMT.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingTxBmt(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingTxBsp
 *********************************************************************************************************************/
/*! \brief       Handles the charging tx message BSP.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingTxBsp(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingTxBst
 *********************************************************************************************************************/
/*! \brief       Handles the charging tx message BST.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingTxBst(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingTx
 *********************************************************************************************************************/
/*! \brief       Handles the charging tx state.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingTx(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalStatisticRxCrm
 *********************************************************************************************************************/
/*! \brief       Handles the charge statistic rx message CRM.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalStatisticRxCrm(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalStatisticRx
 *********************************************************************************************************************/
/*! \brief       Handles the charge statistic rx state.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalStatisticRx(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalStatisticTxBsd
 *********************************************************************************************************************/
/*! \brief       Handles the charge statistic tx message BSD.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalStatisticTxBsd(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalStatisticTx
 *********************************************************************************************************************/
/*! \brief       Handles the charge statistic tx state.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalStatisticTx(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalErrorRxCem
 *********************************************************************************************************************/
/*! \brief       Handles the error rx message CRM.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalErrorRxCem(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalErrorRx
 *********************************************************************************************************************/
/*! \brief       Handles the error rx state.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalErrorRx(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalErrorTxBem
 *********************************************************************************************************************/
/*! \brief       Handles the error tx message BEM.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalErrorTxBem(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalErrorTx
 *********************************************************************************************************************/
/*! \brief       Handles the error tx state.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalErrorTx(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandleCharging
 *********************************************************************************************************************/
/*! \brief       Handles the charging state machine.
 *  \details     -
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalHandleCharging(void);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCHM
 *********************************************************************************************************************/
/*! \brief       Handles the reception of a CHM message.
 *  \details     -
 *  \param[in]   PduInfoPtr  Pointer to the pdu info type.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCHM(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCRM
 *********************************************************************************************************************/
/*! \brief       Handles the reception of a CRM message.
 *  \details     -
 *  \param[in]   PduInfoPtr  Pointer to the pdu info type.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCRM(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCEM
 *********************************************************************************************************************/
/*! \brief       Handles the reception of a CEM message.
 *  \details     -
 *  \param[in]   PduInfoPtr  Pointer to the pdu info type.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCEM(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCTS
 *********************************************************************************************************************/
/*! \brief       Handles the reception of a CTS message.
 *  \details     -
 *  \param[in]   PduInfoPtr  Pointer to the pdu info type.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCTS(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCML
 *********************************************************************************************************************/
/*! \brief       Handles the reception of a CML message.
 *  \details     -
 *  \param[in]   PduInfoPtr  Pointer to the pdu info type.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCML(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCRO
 *********************************************************************************************************************/
/*! \brief       Handles the reception of a CRO message.
 *  \details     -
 *  \param[in]   PduInfoPtr  Pointer to the pdu info type.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCRO(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCCS
 *********************************************************************************************************************/
/*! \brief       Handles the reception of a CCS message.
 *  \details     -
 *  \param[in]   PduInfoPtr  Pointer to the pdu info type.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCCS(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCST
 *********************************************************************************************************************/
/*! \brief       Handles the reception of a CST message.
 *  \details     -
 *  \param[in]   PduInfoPtr  Pointer to the pdu info type.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCST(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCSD
 *********************************************************************************************************************/
/*! \brief       Handles the reception of a CSD message.
 *  \details     -
 *  \param[in]   PduInfoPtr  Pointer to the pdu info type.
 *  \pre         Module is initialized.
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCSD(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr);

/*============================================== VCANCCGBT_STOP_SEC_CODE ============================================*/
#define VCANCCGBT_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/*============================================== VCANCCGBT_START_SEC_CODE ===========================================*/
#define VCANCCGBT_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalCompareData
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(boolean, VCANCCGBT_CODE) vCanCcGbt_InternalCompareData(const uint8 * currentdata,
                                                                            const uint8 * newData, uint8 dataLength)
{
  boolean retVal = TRUE;
  uint8 i;

  for (i = 0; i < dataLength; i++)
  {
    if (currentdata[i] != newData[i])
    {
      retVal = FALSE;

      break;
    }
  }

  return retVal;
} /* vCanCcGbt_InternalCompareData() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSet2ByteDataLittleEndian()
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSet2ByteDataLittleEndian(uint16 value,
                                                                                             P2VAR(uint8, AUTOMATIC,
                                                                                                   VCANCCGBT_APPL_DATA)
                                                                                             destBuffer)
{
#if(CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
  vCanCcGbt_Write_Buffer(&destBuffer[0], (uint8) (value >> 8));
  vCanCcGbt_Write_Buffer(&destBuffer[1], (uint8) value);
#else
  vCanCcGbt_Write_Buffer(&destBuffer[0], (uint8) value);
  vCanCcGbt_Write_Buffer(&destBuffer[1], (uint8) (value >> 8));
#endif
} /* vCanCcGbt_InternalSet2ByteDataLittleEndian() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSet3ByteDataLittleEndian()
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSet3ByteDataLittleEndian(uint32 value,
                                                                                             P2VAR(uint8, AUTOMATIC,
                                                                                                   VCANCCGBT_APPL_DATA)
                                                                                             destBuffer)
{
#if(CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
  vCanCcGbt_Write_Buffer(&destBuffer[0], (uint8) (value >> 16));
  vCanCcGbt_Write_Buffer(&destBuffer[1], (uint8) (value >> 8));
  vCanCcGbt_Write_Buffer(&destBuffer[2], (uint8) value);
#else
  vCanCcGbt_Write_Buffer(&destBuffer[0], (uint8) value);
  vCanCcGbt_Write_Buffer(&destBuffer[1], (uint8) (value >> 8));
  vCanCcGbt_Write_Buffer(&destBuffer[2], (uint8) (value >> 16));
#endif
} /* vCanCcGbt_InternalSet3ByteDataLittleEndian() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSet4ByteDataLittleEndian()
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSet4ByteDataLittleEndian(uint32 value,
                                                                                             P2VAR(uint8, AUTOMATIC,
                                                                                                   VCANCCGBT_APPL_DATA)
                                                                                             destBuffer)
{
#if(CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
  vCanCcGbt_Write_Buffer(&destBuffer[0], (uint8) (value >> 24));
  vCanCcGbt_Write_Buffer(&destBuffer[1], (uint8) (value >> 16));
  vCanCcGbt_Write_Buffer(&destBuffer[2], (uint8) (value >> 8));
  vCanCcGbt_Write_Buffer(&destBuffer[3], (uint8) value);
#else
  vCanCcGbt_Write_Buffer(&destBuffer[0], (uint8) value);
  vCanCcGbt_Write_Buffer(&destBuffer[1], (uint8) (value >> 8));
  vCanCcGbt_Write_Buffer(&destBuffer[2], (uint8) (value >> 16));
  vCanCcGbt_Write_Buffer(&destBuffer[3], (uint8) (value >> 24));
#endif
} /* vCanCcGbt_InternalSet4ByteDataLittleEndian() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalResetTpPDU
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalResetTpPDU(uint8 pduId)
{
  vCanCcGbt_TpTxPduStateType *pduState = &vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(pduId));

  /* #-- Release the state of the confirmed Tx Pdu. */
  pduState->IsLocked = FALSE;

  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  switch (vCanCcGbt_GetPduTypeOfTpTxPdu(pduId))
  {
  case VCANCCGBT_PDU_BRM_PDUTYPEOFTPTXPDU:
    vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.ReadPos = 0;
    break;

  case VCANCCGBT_PDU_BCP_PDUTYPEOFTPTXPDU:
    vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.ReadPos = 0;
    break;

  case VCANCCGBT_PDU_BCS_PDUTYPEOFTPTXPDU:
    vCanCcGbt_ChargeState.ChargePhase.BcsMsg.ReadPos = 0;
    break;

  case VCANCCGBT_PDU_BMT_PDUTYPEOFTPTXPDU:
    vCanCcGbt_ChargeState.ChargePhase.BmtMsg.ReadPos = 0;
    break;

  case VCANCCGBT_PDU_BMV_PDUTYPEOFTPTXPDU:
    vCanCcGbt_ChargeState.ChargePhase.BmvMsg.ReadPos = 0;
    break;

  case VCANCCGBT_PDU_BSP_PDUTYPEOFTPTXPDU:
    vCanCcGbt_ChargeState.ChargePhase.BspMsg.ReadPos = 0;
    break;

  default:
    break;
  }

  SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
} /* vCanCcGbt_InternalResetTpPDU() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandleIfPduSendTimeout
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(boolean,
                     VCANCCGBT_CODE) vCanCcGbt_InternalHandleIfPduSendTimeout(vCanCcGbt_IfTxPduStateType * pduState)
{
  boolean retVal;

  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

  retVal = vCanCcGbt_InternalHandleSendTimeout(&(pduState->SendTimeout));

  SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

  return retVal;
} /* vCanCcGbt_InternalHandleIfPduSendTimeout() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandleTpPduSendTimeout
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(boolean,
                     VCANCCGBT_CODE) vCanCcGbt_InternalHandleTpPduSendTimeout(vCanCcGbt_TpTxPduStateType * pduState)
{
  boolean retVal;

  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

  retVal = vCanCcGbt_InternalHandleSendTimeout(&(pduState->SendTimeout));

  SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

  return retVal;
} /* vCanCcGbt_InternalHandleTpPduSendTimeout() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandleSendTimeout
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(boolean, VCANCCGBT_CODE) vCanCcGbt_InternalHandleSendTimeout(uint16 * sendTimeout)
{
  boolean retVal = FALSE;

  (*sendTimeout)--;

  if ((*sendTimeout) == 0)
  {
    *sendTimeout = VCANCCGBT_SENDTIMEOUT_INIT;

    retVal = TRUE;
  }

  return retVal;
} /* vCanCcGbt_InternalHandleSendTimeout() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSendTp
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(Std_ReturnType, VCANCCGBT_CODE) vCanCcGbt_InternalSendTp(uint8 * data, uint16 dataLength,
                                                                              uint8 txPduIdx)
{
  Std_ReturnType retVal = E_NOT_OK;
  vCanCcGbt_TpTxPduStateType *pduState = &vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(txPduIdx));

  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if (!pduState->IsLocked)
  {
    /* #-- Prepare the Pdu with the data to transmit. */
    PduInfoType pdu;

    pduState->IsLocked = TRUE;

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

    pdu.SduDataPtr = data;
    pdu.SduLength = dataLength;

    /* #-- Call the PduR transmit function. */
    retVal = PduR_vCanCcGbtTransmit(vCanCcGbt_GetPduRTxPduIdOfTpTxPdu(txPduIdx), &pdu);

    if (retVal != E_OK)
    {
      pduState->IsLocked = FALSE;
    }
  }
  else
  {
    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();
  }

  return retVal;
} /* vCanCcGbt_InternalSendTp() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSendIf
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(Std_ReturnType, VCANCCGBT_CODE) vCanCcGbt_InternalSendIf(uint8 * data, uint16 dataLength,
                                                                              uint8 txPduIdx)
{
  Std_ReturnType retVal;
  PduInfoType pdu;

  /* #-- Prepare the Pdu with the data to transmit. */
  pdu.SduDataPtr = data;
  pdu.SduLength = dataLength;

  /* #-- Call the PduR transmit function. */
  retVal = PduR_vCanCcGbtTransmit(vCanCcGbt_GetPduRTxPduIdOfIfTxPdu(txPduIdx), &pdu);

  return retVal;
} /* vCanCcGbt_InternalSendIf() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSetMandatoryInitialChargingParameter
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE)
vCanCcGbt_InternalSetMandatoryInitialChargingParameter(vCanCcGbt_MandatoryInitialChargingParameterType
                                                       chargingParameter)
{
  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  /* BHM */
  vCanCcGbt_Set_BHM_TMPCV(vCanCcGbt_ChargeState.HandshakePhase.BhmMsg.Data,
                          chargingParameter.TotalMaximumPermissibleChargingVoltage);

  /* BRM */
  vCanCcGbt_Set_BRM_Version(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data, chargingParameter.Version);
  vCanCcGbt_Set_BRM_BT(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data, chargingParameter.BatteryType);
  vCanCcGbt_Set_BRM_PSBSC(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data,
                          chargingParameter.PowerStorageBatterySytemCapacity);
  vCanCcGbt_Set_BRM_PSBSV(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data,
                          chargingParameter.PowerStorageBatterySystemVoltage);

  SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
} /* vCanCcGbt_InternalSetMandatoryInitialChargingParameter() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSetOptionalInitialChargingParameter
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE)
vCanCcGbt_InternalSetOptionalInitialChargingParameter(vCanCcGbt_OptionalInitialChargingParameterType chargingParameter)
{
  uint8 notValidLength = 0;

  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  if (chargingParameter.IsBatteryManufacturerNameValid == TRUE)
  {
    vCanCcGbt_Set_BRM_BMN(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data, chargingParameter.BatteryManufacturerName);
    notValidLength = 0;
  }
  else
  {
    VStdLib_MemSet(&(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data[VCANCCGBT_BRM_BMN_POS]), VCANCCGBT_SNA_VALUE,
                   VCANCCGBT_BRM_BMN_LENGTH);
    notValidLength += VCANCCGBT_BRM_BMN_LENGTH;
  }

  if (chargingParameter.IsBatteryPackNoValid == TRUE)
  {
    vCanCcGbt_Set_BRM_BPN(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data, chargingParameter.BatteryPackNo);
    notValidLength = 0;
  }
  else
  {
    VStdLib_MemSet(&(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data[VCANCCGBT_BRM_BPN_POS]), VCANCCGBT_SNA_VALUE,
                   VCANCCGBT_BRM_BPN_LENGTH);
    notValidLength += VCANCCGBT_BRM_BPN_LENGTH;
  }

  if (chargingParameter.IsBatterySetProductionDataYearValid == TRUE)
  {
    vCanCcGbt_Set_BRM_BSPDY(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data,
                            chargingParameter.BatterySetProductionDataYear);
    notValidLength = 0;
  }
  else
  {
    VStdLib_MemSet(&(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data[VCANCCGBT_BRM_BSPDY_POS]), VCANCCGBT_SNA_VALUE,
                   VCANCCGBT_BRM_BSPDY_LENGTH);
    notValidLength += VCANCCGBT_BRM_BSPDY_LENGTH;
  }

  if (chargingParameter.IsBatterySetProductionDataMonthValid == TRUE)
  {
    vCanCcGbt_Set_BRM_BSPDM(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data,
                            chargingParameter.BatterySetProductionDataMonth);
    notValidLength = 0;
  }
  else
  {
    VStdLib_MemSet(&(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data[VCANCCGBT_BRM_BSPDM_POS]), VCANCCGBT_SNA_VALUE,
                   VCANCCGBT_BRM_BSPDM_LENGTH);
    notValidLength += VCANCCGBT_BRM_BSPDM_LENGTH;
  }

  if (chargingParameter.IsBatterySetProductionDataDayValid == TRUE)
  {
    vCanCcGbt_Set_BRM_BSPDD(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data,
                            chargingParameter.BatterySetProductionDataDay);
    notValidLength = 0;
  }
  else
  {
    VStdLib_MemSet(&(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data[VCANCCGBT_BRM_BSPDD_POS]), VCANCCGBT_SNA_VALUE,
                   VCANCCGBT_BRM_BSPDD_LENGTH);
    notValidLength += VCANCCGBT_BRM_BSPDD_LENGTH;
  }

  if (chargingParameter.IsBatterySetChargingFrequencyValid == TRUE)
  {
    vCanCcGbt_Set_BRM_BSCF(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data,
                           chargingParameter.BatterySetChargingFrequency);
    notValidLength = 0;
  }
  else
  {
    VStdLib_MemSet(&(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data[VCANCCGBT_BRM_BSCF_POS]), VCANCCGBT_SNA_VALUE,
                   VCANCCGBT_BRM_BSCF_LENGTH);
    notValidLength += VCANCCGBT_BRM_BSCF_LENGTH;
  }

  if (chargingParameter.IsBatterySetPropertyRightMarkValid == TRUE)
  {
    vCanCcGbt_Set_BRM_BSPRM(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data,
                            chargingParameter.BatterySetPropertyRightMark);
    notValidLength = 0;
  }
  else
  {
    VStdLib_MemSet(&(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data[VCANCCGBT_BRM_BSPRM_POS]), VCANCCGBT_SNA_VALUE,
                   VCANCCGBT_BRM_BSPRM_LENGTH);
    notValidLength += VCANCCGBT_BRM_BSPRM_LENGTH;
  }

  if (chargingParameter.IsVehicleIdentificationNumberValid == TRUE)
  {
    vCanCcGbt_Set_BRM_VIN(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data,
                          chargingParameter.VehicleIdentificationNumber);
    notValidLength = 0;
  }
  else
  {
    VStdLib_MemSet(&(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data[VCANCCGBT_BRM_VIN_POS]), VCANCCGBT_SNA_VALUE,
                   VCANCCGBT_BRM_VIN_LENGTH);
    notValidLength += VCANCCGBT_BRM_VIN_LENGTH + 1;
  }

  if (chargingParameter.IsBMSSoftwareVersionNumberValid == TRUE)
  {
    vCanCcGbt_Set_BRM_BSVN(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data,
                           chargingParameter.BMSSoftwareVersionNumber);
    notValidLength = 0;
  }
  else
  {
    VStdLib_MemSet(&(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data[VCANCCGBT_BRM_BSVN_POS]), VCANCCGBT_SNA_VALUE,
                   VCANCCGBT_BRM_BSVN_LENGTH);
    notValidLength += VCANCCGBT_BRM_BSVN_LENGTH;
  }

  vCanCcGbt_Set_BRM_Reserved(vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data, VCANCCGBT_SNA_VALUE);

  if (notValidLength > 0)
  {
    vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.DataLength -= notValidLength;
  }

  SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
} /* vCanCcGbt_InternalSetOptionalInitialChargingParameter() */ /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH , MD_MSR_STCYC */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSetParameterExchangeData
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE)
vCanCcGbt_InternalSetParameterExchangeData(vCanCcGbt_MandatoryParameterExchangeDataType chargingParameter)
{
  boolean isBmsReadyForCharging = AppVCanCcGbt_IsBmsReadyForCharging();

  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  /* BCP */
  vCanCcGbt_Set_BCP_MPCVSB(vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.Data,
                           chargingParameter.MaximumPermissibleChargingVoltageSingleBattery);
  vCanCcGbt_Set_BCP_MPCC(vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.Data,
                         chargingParameter.MaximumPermissibleChargingCurrent);
  vCanccGbt_Set_BCP_TNEPSB(vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.Data,
                           chargingParameter.TotalNominalEnergyOfPowerStorageBattery);
  vCanCcGbt_Set_BCP_TMPCV(vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.Data,
                          chargingParameter.TotalMaximumPermissibleChargingVoltage);
  vCanCcGbt_Set_BCP_MPT(vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.Data,
                        chargingParameter.MaximumPermissibleTemperature);
  vCanCcGbt_Set_BCP_CSPSBV(vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.Data,
                           chargingParameter.ChargeStatusPowerStorageBatteryVehicle);
  vCanCcGbt_Set_BCP_CBVPSBV(vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.Data,
                            chargingParameter.CurrentBatteryVoltagePowerStorageBatteryVehicle);

  /* BRO */
  if (isBmsReadyForCharging == TRUE)
  {
    vCanCcGbt_Set_BRO_IBRFC(vCanCcGbt_ChargeState.ParameterExchangePhase.BroMsg.Data, VCANCCGBT_BMS_IS_READY);
  }

  SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
} /* vCanCcGbt_InternalSetParameterExchangeData() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSetBCLData
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSetBCLData(vCanCcGbt_BCLDataType bclData)
{
  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  vCanCcGbt_Set_BCL_VD(vCanCcGbt_ChargeState.ChargePhase.BclMsg.Data, bclData.VoltageDemand);
  vCanCcGbt_Set_BCL_CD(vCanCcGbt_ChargeState.ChargePhase.BclMsg.Data, bclData.CurrentDemand);
  vCanCcGbt_Set_BCL_CM(vCanCcGbt_ChargeState.ChargePhase.BclMsg.Data, bclData.ChargingMode);

  SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
} /* vCanCcGbt_InternalSetBCLData() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSetBCSData
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSetBCSData(vCanCcGbt_BCSDataType bcsData)
{
  uint16 highestVoltageOfBatteryAndGroupNumber =
    (bcsData.HighestVoltageOfBattery & 0x3FF) | (((uint16) bcsData.HighestVoltageOfBatteryGroupNumber) << 12);

  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  vCanCcGbt_Set_BCS_MCV(vCanCcGbt_ChargeState.ChargePhase.BcsMsg.Data, bcsData.MeasuredValueOfChargingVoltage);
  vCanCcGbt_Set_BCS_MCC(vCanCcGbt_ChargeState.ChargePhase.BcsMsg.Data, bcsData.MeasuredValueOfChargingCurrent);
  vCanCcGbt_Set_BCS_HVB(vCanCcGbt_ChargeState.ChargePhase.BcsMsg.Data, highestVoltageOfBatteryAndGroupNumber);
  vCanCcGbt_Set_BCS_CCS(vCanCcGbt_ChargeState.ChargePhase.BcsMsg.Data, bcsData.CurrentChargeState);
  vCanCcGbt_Set_BCS_ERT(vCanCcGbt_ChargeState.ChargePhase.BcsMsg.Data, bcsData.EstimatedRemainingChargingTime);

  SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
} /* vCanCcGbt_InternalSetBCSData() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSetBSMData
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSetBSMData(vCanCcGbt_BSMDataType bsmData)
{
  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  vCanCcGbt_Set_BSM_SNHVB(vCanCcGbt_ChargeState.ChargePhase.BsmMsg.Data, bsmData.SerialNumberHighestVoltgeBattery);
  vCanCcGbt_Set_BSM_HTB(vCanCcGbt_ChargeState.ChargePhase.BsmMsg.Data, bsmData.HighestTemperatureBattery);
  vCanCcGbt_Set_BSM_SNHTDP(vCanCcGbt_ChargeState.ChargePhase.BsmMsg.Data,
                           bsmData.SerialNumberHighestTemperatureDetectionPoint);
  vCanCcGbt_Set_BSM_LTB(vCanCcGbt_ChargeState.ChargePhase.BsmMsg.Data, bsmData.LowestTemperatureBattery);
  vCanCcGbt_Set_BSM_SNLTDP(vCanCcGbt_ChargeState.ChargePhase.BsmMsg.Data,
                           bsmData.SerialNumberLowestTemperatureDetectionPoint);
  vCanCcGbt_Set_BSM_Byte6(vCanCcGbt_ChargeState.ChargePhase.BsmMsg.Data,
                          bsmData.VoltageOfSingleBateryIsOverHighOrOverLow,
                          bsmData.StateOfChargeOfBatteryCompleteVehicle, bsmData.ChargingOvercurrentBattery,
                          bsmData.ExcessTemperatureOfBattery);

  vCanCcGbt_Set_BSM_Byte7(vCanCcGbt_ChargeState.ChargePhase.BsmMsg.Data, bsmData.InsulationStateOfBattery,
                          bsmData.ConnectionStateOfOutputConnectorOfBattery, bsmData.ChargingPermissible);

  SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
} /* vCanCcGbt_InternalSetBSMData() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSetBMVData
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSetBMVData(vCanCcGbt_BMVDataType bmvData)
{
  uint16 notValidLength = 0;
  uint16 i;
  uint16 batteryVoltage;

  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  for (i = 0; i < VCANCCGBT_BMV_ELEMENT_CNT; i++)
  {
    if (bmvData.BMVSignleBattery[i].IsActive == TRUE)
    {
      batteryVoltage =
        ((bmvData.BMVSignleBattery[i].BatteryVoltage & 0x3FF) |
         (((uint16)(bmvData.BMVSignleBattery[i].BatteryGroupNumber & 0x0F)) << 12));

      vCanCcGbt_Set_BMV_SBV(vCanCcGbt_ChargeState.ChargePhase.BmvMsg.Data, batteryVoltage,
                            i * VCANCCGBT_BMV_SPSBV_LENGTH);
      notValidLength = 0;
    }
    else
    {
      VStdLib_MemSet(&(vCanCcGbt_ChargeState.ChargePhase.BmvMsg.Data[i * VCANCCGBT_BMV_SPSBV_LENGTH]),
                     VCANCCGBT_SNA_VALUE, VCANCCGBT_BMV_SPSBV_LENGTH);
      notValidLength += VCANCCGBT_BMV_SPSBV_LENGTH;
    }
  }

  vCanCcGbt_ChargeState.ChargePhase.BmvMsg.DataLength = VCANCCGBT_BMV_LENGTH - notValidLength;

  SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
} /* vCanCcGbt_InternalSetBMVData() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSetBMTData
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSetBMTData(vCanCcGbt_BMTDataType bmtData)
{
  uint8 notValidLength = 0;
  uint8 i;

  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  for (i = 0; i < VCANCCGBT_BMT_LENGTH; i++)
  {
    if (bmtData.BMTSingleTemperature[i].IsActive == TRUE)
    {
      vCanCcGbt_Set_BMT_SBT(vCanCcGbt_ChargeState.ChargePhase.BmtMsg.Data, i,
                            bmtData.BMTSingleTemperature[i].BatteryTemperature);
      notValidLength = 0;
    }
    else
    {
      VStdLib_MemSet(&(vCanCcGbt_ChargeState.ChargePhase.BmtMsg.Data[i]), VCANCCGBT_SNA_VALUE,
                     VCANCCGBT_BMT_FIELD_LENGTH);
      notValidLength += VCANCCGBT_BMT_FIELD_LENGTH;
    }
  }

  vCanCcGbt_ChargeState.ChargePhase.BmtMsg.DataLength = VCANCCGBT_BMT_LENGTH - notValidLength;

  SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
} /* vCanCcGbt_InternalSetBMTData() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSetBSPData
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSetBSPData(vCanCcGbt_BSPDataType bspData)
{
  uint8 notValidLength = 0;
  uint8 i;

  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  for (i = 0; i < VCANCCGBT_BSP_LENGTH; i++)
  {
    if (bspData.BSPSignleBatteryReserved[i].IsActive == TRUE)
    {
      vCanCcGbt_Set_BSP_BR(vCanCcGbt_ChargeState.ChargePhase.BspMsg.Data, i,
                           bspData.BSPSignleBatteryReserved[i].BatteryReserved);
      notValidLength = 0;
    }
    else
    {
      VStdLib_MemSet(&(vCanCcGbt_ChargeState.ChargePhase.BspMsg.Data[i]), VCANCCGBT_SNA_VALUE,
                     VCANCCGBT_BSP_FIELD_LENGTH);
      notValidLength += VCANCCGBT_BSP_FIELD_LENGTH;
    }
  }

  vCanCcGbt_ChargeState.ChargePhase.BspMsg.DataLength = VCANCCGBT_BSP_LENGTH - notValidLength;

  SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
} /* vCanCcGbt_InternalSetBSPData() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSetBSTData
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSetBSTData(vCanCcGbt_BSTDataType bstData)
{
  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  vCanCcGbt_Set_BST_CBSC(vCanCcGbt_ChargeState.ChargePhase.BstMsg.Data, bstData.CauseForBMSSuspendingCharging);
  vCanCcGbt_Set_BST_FCBSC(vCanCcGbt_ChargeState.ChargePhase.BstMsg.Data, bstData.FaultCauseForBMSSuspendingCharging);
  vCanCcGbt_Set_BST_ECBSC(vCanCcGbt_ChargeState.ChargePhase.BstMsg.Data, bstData.ErrorCauseForBMSSuspendingCharging);

  SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
} /* vCanCcGbt_InternalSetBSTData() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalSetBSDData
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalSetBSDData(vCanCcGbt_BSDDataType bsdData)
{
  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  vCanCcGbt_Set_BSD_SSC(vCanCcGbt_ChargeState.ChargeStatisticPhase.BsdMsg.Data, bsdData.SuspendStateOfCharge);
  vCanCcGbt_Set_BSD_MIVSB(vCanCcGbt_ChargeState.ChargeStatisticPhase.BsdMsg.Data, bsdData.MinimumVoltageSingleBattery);
  vCanCcGbt_Set_BSD_MAVSB(vCanCcGbt_ChargeState.ChargeStatisticPhase.BsdMsg.Data, bsdData.MaximumVoltageSingleBattery);
  vCanCcGbt_Set_BSD_MIT(vCanCcGbt_ChargeState.ChargeStatisticPhase.BsdMsg.Data, bsdData.MinimumTemperatureBattery);
  vCanCcGbt_Set_BSD_MAT(vCanCcGbt_ChargeState.ChargeStatisticPhase.BsdMsg.Data, bsdData.MaximumTemperatureBattery);

  SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
} /* vCanCcGbt_InternalSetBSDData() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandleMsgAction
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalHandleMsgAction(vCanCcGbt_StateType currentState,
                                                                             vCanCcGbt_MsgActionType msgAction)
{
  switch (msgAction)
  {
  case E_VCANCCGBT_MSG_ACTION_CHM_RECEIVED:
    vCanCcGbt_ChargeState.HandshakePhase.ChmMsg.IsActive = TRUE;
    break;

  case E_VCANCCGBT_MSG_ACTION_CRM_RECEIVED:
    if ((currentState == E_VCANCCGBT_STATE_CHARGE_STATISTIC) || (currentState == E_VCANCCGBT_STATE_ERROR))
    {
      vCanCcGbt_InternalStateChange(currentState, E_VCANCCGBT_STATE_HANDSHAKE);
    }

    vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.IsActive = TRUE;
    break;

  case E_VCANCCGBT_MSG_ACTION_CTS_RECEIVED:
    vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.IsActive = TRUE;
    break;

  case E_VCANCCGBT_MSG_ACTION_CML_RECEIVED:
    vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.IsActive = TRUE;
    break;

  case E_VCANCCGBT_MSG_ACTION_CRO_RECEIVED:
    vCanCcGbt_ChargeState.ParameterExchangePhase.CroMsg.IsActive = TRUE;
    break;

  case E_VCANCCGBT_MSG_ACTION_CCS_RECEIVED:
    vCanCcGbt_ChargeState.ChargePhase.CcsMsg.IsActive = TRUE;
    break;

  case E_VCANCCGBT_MSG_ACTION_CST_RECEIVED:
    vCanCcGbt_ChargeState.ChargePhase.CstMsg.IsActive = TRUE;
    break;

  case E_VCANCCGBT_MSG_ACTION_CSD_RECEIVED:
    vCanCcGbt_ChargeState.ChargeStatisticPhase.CsdMsg.IsActive = TRUE;
    break;

  case E_VCANCCGBT_MSG_ACTION_CEM_RECEIVED:
    vCanCcGbt_InternalStateChange(currentState, E_VCANCCGBT_STATE_ERROR);
    vCanCcGbt_ChargeState.ErrorPhase.CemMsg.IsActive = TRUE;
    break;

  case E_VCANCCGBT_MSG_ACTION_CHM_CYCLIC:
    vCanCcGbt_ChargeState.HandshakePhase.BhmMsg.IsActive = TRUE;
    break;

  case E_VCANCCGBT_MSG_ACTION_CRM_CYCLIC:
    vCanCcGbt_ChargeState.HandshakePhase.BhmMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.IsActive = TRUE;
    break;

  case E_VCANCCGBT_MSG_ACTION_CRM_POSITIVE_RECOGNITION_RESULT_CYCLIC:
    vCanCcGbt_InternalStateChange(currentState, E_VCANCCGBT_STATE_PARAM_EXCHANGE);
    break;

  case E_VCAMCCGNT_MSG_ACTION_CML_CYCLIC:
    vCanCcGbt_ChargeState.ParameterExchangePhase.BroMsg.IsActive = TRUE;
    vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.IsActive = FALSE;
    break;

  case E_VCANCCGBT_MSG_ACTION_CRO_CYCLIC:
    vCanCcGbt_InternalStateChange(currentState, E_VCANCCGBT_STATE_CHARGE);
    break;

  case E_VCANCCGBT_MSG_ACTION_CCS_CYCLIC:
    vCanCcGbt_ChargeState.ChargePhase.BsmMsg.IsActive = TRUE;
    vCanCcGbt_ChargeState.ChargePhase.BmvMsg.IsActive = TRUE;
    vCanCcGbt_ChargeState.ChargePhase.BmtMsg.IsActive = TRUE;
    vCanCcGbt_ChargeState.ChargePhase.BspMsg.IsActive = TRUE;
    break;

  case E_VCANCCGBT_MSG_ACTION_CST_BMS_CYCLIC:
    vCanCcGbt_InternalStateChange(currentState, E_VCANCCGBT_STATE_CHARGE_STATISTIC);
    break;

  case E_VCANCCGBT_MSG_ACTION_CST_CHARGER_CYCLIC:
    vCanCcGbt_ChargeState.ChargePhase.BclMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.BcsMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.BsmMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.BmvMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.BmtMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.BspMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.CcsMsg.IsActive = FALSE;

    vCanCcGbt_ChargeState.ChargePhase.BstMsg.IsActive = TRUE;
    break;

  case E_VCANCCGBT_MSG_ACTION_BRO_TRANSMIT_CHARGING_READY:
    vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.IsActive = FALSE;
    break;

  case E_VCANCCGBT_MSG_ACTION_BST_TRANSMIT:
    vCanCcGbt_InternalStateChange(currentState, E_VCANCCGBT_STATE_CHARGE_STATISTIC);
    break;

  case E_VCANCCGBT_MSG_ACTION_BEM_TRANSMIT:
    vCanCcGbt_InternalStateChange(currentState, E_VCANCCGBT_STATE_ERROR);
    vCanCcGbt_ChargeState.ErrorPhase.BemMsg.IsActive = TRUE;
    break;

  case E_VCANCCGBT_MSG_ACTION_SUSPENDCHARGING:
    vCanCcGbt_ChargeState.ChargePhase.BclMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.BcsMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.BsmMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.BmvMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.BmtMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.BspMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.CcsMsg.IsActive = FALSE;

    vCanCcGbt_ChargeState.ChargePhase.BstMsg.IsActive = TRUE;
    break;

  default: /* PRQA S 2018 */ /* MD_MSR_14.1 */
    break;
  }
} /* vCanCcGbt_InternalHandleMsgAction() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalStateReset
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalStateReset(vCanCcGbt_StateType state)
{
  vCanCcGbt_IfTxPduStateType *pduStateIf;
  vCanCcGbt_TpTxPduStateType *pduStateTp;

  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

  switch (state)
  {
  case E_VCANCCGBT_STATE_INIT:
    /* Intentionally left empty */
    break;

  case E_VCANCCGBT_STATE_HANDSHAKE:
    /* BHM */
    pduStateIf = &(vCanCcGbt_GetIfTxPduState(vCanCcGbt_GetIfTxPduStateIdxOfIfTxPdu(vCanCcGbt_GetBhmTxPdu())));
    pduStateIf->SendTimeout = VCANCCGBT_SENDTIMEOUT_INIT;
    vCanCcGbt_ChargeState.HandshakePhase.BhmMsg.IsActive = FALSE;

    /* BRM */
    pduStateTp = &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(vCanCcGbt_GetBrmTxPdu())));
    pduStateTp->SendTimeout = VCANCCGBT_SENDTIMEOUT_INIT;
    vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.ReadPos = 0;
    vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.DataLength = VCANCCGBT_BRM_LENGTH;
    vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.IsActive = FALSE;

    /* CHM */
    vCanCcGbt_ChargeState.HandshakePhase.ChmMsg.IsChanged = FALSE;
    vCanCcGbt_ChargeState.HandshakePhase.ChmMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.HandshakePhase.ChmMsg.Timeout = 0;
    VStdLib_MemSet(vCanCcGbt_ChargeState.HandshakePhase.ChmMsg.Data, VCANCCGBT_SNA_VALUE, VCANCCGBT_CHM_LENGTH);

    /* CRM */
    vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.IsChanged = FALSE;
    vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.Timeout = 0;
    VStdLib_MemSet(vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.Data, VCANCCGBT_SNA_VALUE, VCANCCGBT_CRM_LENGTH);

    break;

  case E_VCANCCGBT_STATE_PARAM_EXCHANGE:
    /* BCP */
    pduStateTp = &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(vCanCcGbt_GetBcpTxPdu())));
    pduStateTp->SendTimeout = VCANCCGBT_SENDTIMEOUT_INIT;
    vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.DataLength = 0;
    vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.ReadPos = 0;

    /* BRO */
    pduStateIf = &(vCanCcGbt_GetIfTxPduState(vCanCcGbt_GetIfTxPduStateIdxOfIfTxPdu(vCanCcGbt_GetBroTxPdu())));
    pduStateIf->SendTimeout = VCANCCGBT_SENDTIMEOUT_INIT;
    vCanCcGbt_ChargeState.ParameterExchangePhase.BroMsg.IsActive = FALSE;

    /* CML */
    vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.IsChanged = FALSE;
    vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.Timeout = 0;
    VStdLib_MemSet(vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.Data, VCANCCGBT_SNA_VALUE, VCANCCGBT_CML_LENGTH);


    /* CRO */
    vCanCcGbt_ChargeState.ParameterExchangePhase.CroMsg.IsChanged = FALSE;
    vCanCcGbt_ChargeState.ParameterExchangePhase.CroMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ParameterExchangePhase.CroMsg.Timeout = 0;
    VStdLib_MemSet(vCanCcGbt_ChargeState.ParameterExchangePhase.CroMsg.Data, VCANCCGBT_SNA_VALUE, VCANCCGBT_CRO_LENGTH);

    /* CTS */
    vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.IsChanged = FALSE;
    vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.Timeout = 0;
    VStdLib_MemSet(vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.Data, VCANCCGBT_SNA_VALUE, VCANCCGBT_CTS_LENGTH);

    break;

  case E_VCANCCGBT_STATE_CHARGE:
    vCanCcGbt_ChargeState.ChargePhase.IsChargingSuspended = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.IsChargerSuspendReason = FALSE;

    /* BCL */
    pduStateIf = &(vCanCcGbt_GetIfTxPduState(vCanCcGbt_GetIfTxPduStateIdxOfIfTxPdu(vCanCcGbt_GetBclTxPdu())));
    pduStateIf->SendTimeout = VCANCCGBT_SENDTIMEOUT_INIT;
    vCanCcGbt_ChargeState.ChargePhase.BclMsg.IsActive = FALSE;

    /* BCS */
    pduStateTp = &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(vCanCcGbt_GetBcsTxPdu())));
    pduStateTp->SendTimeout = VCANCCGBT_SENDTIMEOUT_INIT;
    vCanCcGbt_ChargeState.ChargePhase.BcsMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.BcsMsg.DataLength = 0;
    vCanCcGbt_ChargeState.ChargePhase.BcsMsg.ReadPos = 0;

    /* BSM */
    pduStateIf = &(vCanCcGbt_GetIfTxPduState(vCanCcGbt_GetIfTxPduStateIdxOfIfTxPdu(vCanCcGbt_GetBsmTxPdu())));
    pduStateIf->SendTimeout = VCANCCGBT_SENDTIMEOUT_INIT;
    vCanCcGbt_ChargeState.ChargePhase.BsmMsg.IsActive = FALSE;

    /* BMV */
    pduStateTp = &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(vCanCcGbt_GetBmvTxPdu())));
    pduStateTp->SendTimeout = VCANCCGBT_SENDTIMEOUT_INIT;
    vCanCcGbt_ChargeState.ChargePhase.BmvMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.BmvMsg.DataLength = 0;
    vCanCcGbt_ChargeState.ChargePhase.BmvMsg.ReadPos = 0;

    /* BMT */
    pduStateTp = &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(vCanCcGbt_GetBmtTxPdu())));
    pduStateTp->SendTimeout = VCANCCGBT_SENDTIMEOUT_INIT;
    vCanCcGbt_ChargeState.ChargePhase.BmtMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.BmtMsg.DataLength = 0;
    vCanCcGbt_ChargeState.ChargePhase.BmtMsg.ReadPos = 0;

    /* BSP */
    pduStateTp = &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(vCanCcGbt_GetBspTxPdu())));
    pduStateTp->SendTimeout = VCANCCGBT_SENDTIMEOUT_INIT;
    vCanCcGbt_ChargeState.ChargePhase.BspMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.BspMsg.DataLength = 0;
    vCanCcGbt_ChargeState.ChargePhase.BspMsg.ReadPos = 0;

    /* BST */
    pduStateIf = &(vCanCcGbt_GetIfTxPduState(vCanCcGbt_GetIfTxPduStateIdxOfIfTxPdu(vCanCcGbt_GetBstTxPdu())));
    pduStateIf->SendTimeout = VCANCCGBT_SENDTIMEOUT_INIT;
    vCanCcGbt_ChargeState.ChargePhase.BstMsg.IsActive = FALSE;

    /* CCS */
    vCanCcGbt_ChargeState.ChargePhase.CcsMsg.IsChanged = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.CcsMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.CcsMsg.Timeout = 0;
    VStdLib_MemSet(vCanCcGbt_ChargeState.ChargePhase.CcsMsg.Data, VCANCCGBT_SNA_VALUE, VCANCCGBT_CCS_LENGTH);

    /* CST */
    vCanCcGbt_ChargeState.ChargePhase.CstMsg.IsChanged = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.CstMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargePhase.CstMsg.Timeout = 0;
    VStdLib_MemSet(vCanCcGbt_ChargeState.ChargePhase.CstMsg.Data, VCANCCGBT_SNA_VALUE, VCANCCGBT_CST_LENGTH);

    break;

  case E_VCANCCGBT_STATE_CHARGE_STATISTIC:
    /* BSD */
    pduStateIf = &(vCanCcGbt_GetIfTxPduState(vCanCcGbt_GetIfTxPduStateIdxOfIfTxPdu(vCanCcGbt_GetBsdTxPdu())));
    pduStateIf->SendTimeout = VCANCCGBT_SENDTIMEOUT_INIT;
    vCanCcGbt_ChargeState.ChargeStatisticPhase.BsdMsg.IsActive = FALSE;

    /* CSD */
    vCanCcGbt_ChargeState.ChargeStatisticPhase.CsdMsg.IsChanged = FALSE;
    vCanCcGbt_ChargeState.ChargeStatisticPhase.CsdMsg.IsActive = FALSE;
    vCanCcGbt_ChargeState.ChargeStatisticPhase.CsdMsg.Timeout = 0;
    VStdLib_MemSet(vCanCcGbt_ChargeState.ChargeStatisticPhase.CsdMsg.Data, VCANCCGBT_SNA_VALUE, VCANCCGBT_CSD_LENGTH);

    break;

  case E_VCANCCGBT_STATE_ERROR:
    /* BEM */
    pduStateIf = &vCanCcGbt_GetIfTxPduState(vCanCcGbt_GetIfTxPduStateIdxOfIfTxPdu(vCanCcGbt_GetBemTxPdu()));
    pduStateIf->SendTimeout = VCANCCGBT_SENDTIMEOUT_INIT;
    vCanCcGbt_ChargeState.ErrorPhase.BemMsg.IsActive = FALSE;

    /* CEM */
    vCanCcGbt_ChargeState.ErrorPhase.CemMsg.IsActive = FALSE;
    VStdLib_MemSet(vCanCcGbt_ChargeState.ErrorPhase.CemMsg.Data, VCANCCGBT_SNA_VALUE, VCANCCGBT_CEM_LENGTH);

    break;

  default: /* PRQA S 2018 */ /* MD_MSR_14.1 */
    break;
  }

  SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();
} /* vCanCcGbt_InternalStateReset() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalStateInit
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalStateInit(vCanCcGbt_StateType state)
{
  switch (state)
  {
  case E_VCANCCGBT_STATE_INIT:
    /* Intentionally left empty */
    break;

  case E_VCANCCGBT_STATE_HANDSHAKE:
  {
    vCanCcGbt_MandatoryInitialChargingParameterType mandatoryInitialChargingParameter;
    vCanCcGbt_OptionalInitialChargingParameterType optionalInitialChargingParameter;

    vCanCcGbt_InternalStateReset(state);

    AppVCanCcGbt_GetMandatoryInitialChargingParameter(&mandatoryInitialChargingParameter);
    vCanCcGbt_InternalSetMandatoryInitialChargingParameter(mandatoryInitialChargingParameter);

    AppVCanCcGbt_GetOptionalInitialChargingParameter(&optionalInitialChargingParameter);
    vCanCcGbt_InternalSetOptionalInitialChargingParameter(optionalInitialChargingParameter);
    break;
  }

  case E_VCANCCGBT_STATE_PARAM_EXCHANGE:
  {
    vCanCcGbt_MandatoryParameterExchangeDataType parameterExchangeData;

    vCanCcGbt_InternalStateReset(state);

    AppVCanCcGbt_GetParameterExchangeData(&parameterExchangeData);
    vCanCcGbt_InternalSetParameterExchangeData(parameterExchangeData);

    vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.IsActive = TRUE;
    break;
  }

  case E_VCANCCGBT_STATE_CHARGE:
    vCanCcGbt_InternalStateReset(state);

    vCanCcGbt_ChargeState.ChargePhase.BclMsg.IsActive = TRUE;
    vCanCcGbt_ChargeState.ChargePhase.BcsMsg.IsActive = TRUE;
    break;

  case E_VCANCCGBT_STATE_CHARGE_STATISTIC:
    vCanCcGbt_InternalStateReset(state);

    vCanCcGbt_ChargeState.ChargeStatisticPhase.BsdMsg.IsActive = TRUE;
    break;

  case E_VCANCCGBT_STATE_ERROR:
    vCanCcGbt_InternalStateReset(state);
    break;

  default: /* PRQA S 2018 */ /* MD_MSR_14.1 */
    break;
  }
} /* vCanCcGbt_InternalStateInit() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalStateChange
 *********************************************************************************************************************/
VCANCCGBT_LOCAL FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalStateChange(vCanCcGbt_StateType oldState,
                                                                         vCanCcGbt_StateType newState)
{
  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  if (vCanCcGbt_ChargeState.ChargeState == oldState)
  {
    if (vCanCcGbt_ChargeState.ChargeState != newState)
    {
      vCanCcGbt_ChargeState.ChargeState = newState;

      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

      vCanCcGbt_InternalStateInit(newState);
    }
    else
    {
      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
    }
  }
  else
  {
    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }
} /* vCanCcGbt_InternalStateChange() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandleTimeoutSupervision
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalHandleTimeoutSupervision(uint16 * timeout,
                                                                                       vCanCcGbt_TimeoutSupervisionType
                                                                                       type)
{
  boolean crm00Timeout = FALSE;
  boolean crmAATimeout = FALSE;
  boolean ctsTimeout = FALSE;
  boolean croTimeout = FALSE;
  boolean ccsTimeout = FALSE;
  boolean cstTimeout = FALSE;
  boolean csdTimeout = FALSE;

  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  (*timeout)--;

  if ((*timeout) == 0)
  {
    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    switch (type)
    {
    case E_VCANCCGBT_ERROR_CRM_00:
      vCanCcGbt_Set_BEM_CRM00Timeout(vCanCcGbt_ChargeState.ErrorPhase.BemMsg.Data);
      crm00Timeout = TRUE;

      break;

    case E_VCANCCGBT_ERROR_CRM_AA:
      vCanCcGbt_Set_BEM_CRMAATimeout(vCanCcGbt_ChargeState.ErrorPhase.BemMsg.Data);
      crmAATimeout = TRUE;

      break;

    case E_VCANCCGBT_ERROR_CTS:
      vCanCcGbt_Set_BEM_CTSTimeout(vCanCcGbt_ChargeState.ErrorPhase.BemMsg.Data);
      ctsTimeout = TRUE;

      break;

    case E_VCANCCGBT_ERROR_CML:
      vCanCcGbt_Set_BEM_CMLTimeout(vCanCcGbt_ChargeState.ErrorPhase.BemMsg.Data);
      ctsTimeout = TRUE;

      break;

    case E_VCANCCGBT_ERROR_CRO:
      vCanCcGbt_Set_BEM_CROTimeout(vCanCcGbt_ChargeState.ErrorPhase.BemMsg.Data);
      croTimeout = TRUE;

      break;

    case E_VCANCCGBT_ERROR_CCS:
      vCanCcGbt_Set_BEM_CCSTimeout(vCanCcGbt_ChargeState.ErrorPhase.BemMsg.Data);
      ccsTimeout = TRUE;

      break;

    case E_VCANCCGBT_ERROR_CST:
      vCanCcGbt_Set_BEM_CSTTimeout(vCanCcGbt_ChargeState.ErrorPhase.BemMsg.Data);
      cstTimeout = TRUE;

      break;

    case E_VCANCCGBT_ERROR_CSD:
      vCanCcGbt_Set_BEM_CSDTimeout(vCanCcGbt_ChargeState.ErrorPhase.BemMsg.Data);
      csdTimeout = TRUE;

      break;

    default: /* PRQA S 2018 */ /* MD_MSR_14.1 */
      break;
    }

    AppVCanCcGbt_ProvideBmsErrors(crm00Timeout, crmAATimeout, ctsTimeout, croTimeout, ccsTimeout, cstTimeout,
                                  csdTimeout);

    vCanCcGbt_InternalHandleMsgAction(vCanCcGbt_ChargeState.ChargeState, E_VCANCCGBT_MSG_ACTION_BEM_TRANSMIT);
  }
  else
  {
    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }
} /* vCanCcGbt_InternalHandleTimeoutSupervision() */ /* PRQA S 6030 */ /* MD_MSR_STCYC */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalProtInit
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalProtInit(void)
{
  if (vCanCcGbt_ChargeState.InitPhase.IsChargerPlugged == TRUE)
  {
    vCanCcGbt_InternalStateChange(vCanCcGbt_ChargeState.ChargeState, E_VCANCCGBT_STATE_HANDSHAKE);
  }
} /* vCanCcGbt_InternalProtInit() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandshakeRxChm
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalHandshakeRxChm(void)
{
  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  if (vCanCcGbt_ChargeState.HandshakePhase.ChmMsg.IsChanged == TRUE)
  {
    uint16 chargerVersion = vCanCcGbt_Get_CHM_CV(vCanCcGbt_ChargeState.HandshakePhase.ChmMsg.Data);
    uint8 chargerRevision = vCanCcGbt_Get_CHM_CR(vCanCcGbt_ChargeState.HandshakePhase.ChmMsg.Data);

    vCanCcGbt_ChargeState.HandshakePhase.ChmMsg.IsChanged = FALSE;

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    AppVCanCcGbt_ProvideChargerVersion(chargerRevision, chargerVersion);
  }
  else
  {
    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }

  vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_HANDSHAKE, E_VCANCCGBT_MSG_ACTION_CHM_CYCLIC);
} /* vCanCcGbt_InternalHandshakeRxChm() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandshakeRxCrm
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalHandshakeRxCrm(void)
{
  uint8 recognitionResult;

  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  recognitionResult = vCanCcGbt_Get_CRM_RR(vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.Data);

  if (vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.IsChanged == TRUE)
  {
    uint8 chargerStationLocationCode[4];
    uint32 chargerNo = vCanCcGbt_Get_CRM_CN(vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.Data);

    vCanCcGbt_Get_CRM_CSL(chargerStationLocationCode, vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.Data);
    chargerStationLocationCode[VCANCCGBT_CRM_CSLC_TERMINATION_POS] = '\0';

    vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.IsChanged = FALSE;

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    AppVCanCcGbt_ProvideChargerInformation(chargerNo, chargerStationLocationCode);

    if (recognitionResult == VCANCCGBT_CRM_RECOGNITION_RESULT_AA)
    {
      AppVCanCcGbt_ProvideHandshakeSuccess();

      vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_HANDSHAKE,
                                        E_VCANCCGBT_MSG_ACTION_CRM_POSITIVE_RECOGNITION_RESULT_CYCLIC);
    }
    else
    {
      vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_HANDSHAKE, E_VCANCCGBT_MSG_ACTION_CRM_CYCLIC);
    }
  }
  else
  {
    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }

  if (recognitionResult == VCANCCGBT_CRM_RECOGNITION_RESULT_00)
  {
    vCanCcGbt_InternalHandleTimeoutSupervision(&(vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.Timeout),
                                               E_VCANCCGBT_ERROR_CRM_00);
  }
  else
  {
    vCanCcGbt_InternalHandleTimeoutSupervision(&(vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.Timeout),
                                               E_VCANCCGBT_ERROR_CRM_AA);
  }
} /* vCanCcGbt_InternalHandshakeRxCrm() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandshakeRx
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalHandshakeRx(void)
{
  if (vCanCcGbt_ChargeState.HandshakePhase.ChmMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalHandshakeRxChm();
  }

  if (vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalHandshakeRxCrm();
  }
} /* vCanCcGbt_InternalHandshakeRx() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandshakeTxBhm
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalHandshakeTxBhm(void)
{
  vCanCcGbt_IfTxPduStateType *pduState =
    &(vCanCcGbt_GetIfTxPduState(vCanCcGbt_GetIfTxPduStateIdxOfIfTxPdu(vCanCcGbt_GetBhmTxPdu())));

  if (vCanCcGbt_InternalHandleIfPduSendTimeout(pduState) == TRUE)
  {
    if (vCanCcGbt_InternalSendIf
        (vCanCcGbt_ChargeState.HandshakePhase.BhmMsg.Data, VCANCCGBT_BHM_LENGTH, vCanCcGbt_GetBhmTxPdu()) == E_OK)
    {
      SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

      pduState->SendTimeout = VCANCCGBT_BHM_SENDTIMEOUT;

      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();
    }
  }
} /* vCanCcGbt_InternalHandshakeTxBhm() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandshakeTxBrm
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalHandshakeTxBrm(void)
{
  vCanCcGbt_TpTxPduStateType *pduState =
    &vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(vCanCcGbt_GetBrmTxPdu()));

  if (vCanCcGbt_InternalHandleTpPduSendTimeout(pduState) == TRUE)
  {
    /* TODO gsi: Shall be called with NULL_PTR for BrmMsg.Data but J1939TP has a bug (ESCAN00095631). */
    if (vCanCcGbt_InternalSendTp
        (vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data, vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.DataLength,
         vCanCcGbt_GetBrmTxPdu()) == E_OK)
    {
      SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

      pduState->SendTimeout = VCANCCGBT_BRM_SENDTIMEOUT;

      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();
    }
  }
} /* vCanCcGbt_InternalHandshakeTxBrm() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandshakeTx
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalHandshakeTx(void)
{
  if (vCanCcGbt_ChargeState.HandshakePhase.BhmMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalHandshakeTxBhm();
  }

  if (vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalHandshakeTxBrm();
  }
} /* vCanCcGbt_InternalHandshakeTx() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalParamExRxCts
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalParamExRxCts(void)
{
  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  if (vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.IsChanged == TRUE)
  {
    uint8 second = vCanCcGbt_Get_CTS_Second(vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.Data);
    uint8 minute = vCanCcGbt_Get_CTS_Minute(vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.Data);
    uint8 hour = vCanCcGbt_Get_CTS_Hour(vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.Data);
    uint8 day = vCanCcGbt_Get_CTS_Day(vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.Data);
    uint8 month = vCanCcGbt_Get_CTS_Month(vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.Data);
    uint16 year = vCanCcGbt_Get_CTS_Year(vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.Data);

    vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.IsChanged = FALSE;

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    AppVCanCcGbt_ProvideChargerTime(second, minute, hour, day, month, year);
  }
  else
  {
    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }

  vCanCcGbt_InternalHandleTimeoutSupervision(&(vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.Timeout),
                                             E_VCANCCGBT_ERROR_CTS);
} /* vCanCcGbt_InternalParamExRxCts() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalParamExRxCml
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalParamExRxCml(void)
{
  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  if (vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.IsChanged == TRUE)
  {
    uint16 maxOutputVoltage = vCanCcGbt_Get_CML_MAOV(vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.Data);
    uint16 minOutputVoltage = vCanCcGbt_Get_CML_MIOV(vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.Data);
    uint16 maxOutputCurrent = vCanCcGbt_Get_CML_MAOC(vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.Data);
    uint16 minOutputCurrent = vCanCcGbt_Get_CML_MIOC(vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.Data);

    vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.IsChanged = FALSE;

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    AppVCanCcGbt_ProvideChargerMaximumOutput(maxOutputVoltage, minOutputVoltage, maxOutputCurrent, minOutputCurrent);
  }
  else
  {
    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }

  vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_PARAM_EXCHANGE, E_VCAMCCGNT_MSG_ACTION_CML_CYCLIC);

  vCanCcGbt_InternalHandleTimeoutSupervision(&(vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.Timeout),
                                             E_VCANCCGBT_ERROR_CML);
} /* vCanCcGbt_InternalParamExRxCml() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalParamExRxCro
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalParamExRxCro(void)
{
  uint8 chargerChargingState = 0x00;

  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  if (vCanCcGbt_ChargeState.ParameterExchangePhase.CroMsg.IsChanged == TRUE)
  {
    chargerChargingState = vCanCcGbt_Get_CRO_CCS(vCanCcGbt_ChargeState.ParameterExchangePhase.CroMsg.Data);

    vCanCcGbt_ChargeState.ParameterExchangePhase.CroMsg.IsChanged = FALSE;

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    AppVCanCcGbt_ProvideParameterConfigurationSuccess(chargerChargingState);
  }
  else
  {
    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }

  if (chargerChargingState == VCANCCGBT_BMS_IS_READY)
  {
    vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_PARAM_EXCHANGE, E_VCANCCGBT_MSG_ACTION_CRO_CYCLIC);
  }

  vCanCcGbt_InternalHandleTimeoutSupervision(&(vCanCcGbt_ChargeState.ParameterExchangePhase.CroMsg.Timeout),
                                             E_VCANCCGBT_ERROR_CRO);
} /* vCanCcGbt_InternalParamExRxCro() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalParamExRx
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalParamExRx(void)
{
  if (vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalParamExRxCts();
  }

  if (vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalParamExRxCml();
  }

  if (vCanCcGbt_ChargeState.ParameterExchangePhase.CroMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalParamExRxCro();
  }
} /* vCanCcGbt_InternalParamExRx() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalParamExTxBcp
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalParamExTxBcp(void)
{
  vCanCcGbt_TpTxPduStateType *pduState =
    &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(vCanCcGbt_GetBcpTxPdu())));

  if (vCanCcGbt_InternalHandleTpPduSendTimeout(pduState) == TRUE)
  {
    /* TODO gsi: Shall be called with NULL_PTR for BrmMsg.Data but J1939TP has a bug (ESCAN00095631). */
    if (vCanCcGbt_InternalSendTp
        (vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.Data, VCANCCGBT_BCP_LENGTH,
         vCanCcGbt_GetBcpTxPdu()) == E_OK)
    {
      SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

      pduState->SendTimeout = VCANCCGBT_BCP_SENDTIMEOUT;

      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();
    }
  }
} /* vCanCcGbt_InternalParamExTxBcp() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalParamExTxBro
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalParamExTxBro(void)
{
  vCanCcGbt_IfTxPduStateType *pduState =
    &(vCanCcGbt_GetIfTxPduState(vCanCcGbt_GetIfTxPduStateIdxOfIfTxPdu(vCanCcGbt_GetBroTxPdu())));

  if (vCanCcGbt_InternalHandleIfPduSendTimeout(pduState) == TRUE)
  {
    if (vCanCcGbt_Get_BRO_IBMSRFC(vCanCcGbt_ChargeState.ParameterExchangePhase.BroMsg.Data) != VCANCCGBT_BMS_IS_READY)
    {
      if (AppVCanCcGbt_IsBmsReadyForCharging() == TRUE)
      {
        vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_PARAM_EXCHANGE,
                                          E_VCANCCGBT_MSG_ACTION_BRO_TRANSMIT_CHARGING_READY);

        vCanCcGbt_Set_BRO_IBRFC(vCanCcGbt_ChargeState.ParameterExchangePhase.BroMsg.Data, VCANCCGBT_BMS_IS_READY);
      }
    }

    if (vCanCcGbt_InternalSendIf
        (vCanCcGbt_ChargeState.ParameterExchangePhase.BroMsg.Data, VCANCCGBT_BRO_LENGTH,
         vCanCcGbt_GetBroTxPdu()) == E_OK)
    {
      SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

      pduState->SendTimeout = VCANCCGBT_BRO_SENDTIMEOUT;

      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();
    }
  }
} /* vCanCcGbt_InternalParamExTxBro() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalParamExTx
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalParamExTx(void)
{
  if (vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalParamExTxBcp();
  }

  if (vCanCcGbt_ChargeState.ParameterExchangePhase.BroMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalParamExTxBro();
  }
} /* vCanCcGbt_InternalParamExTx() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingRxCcs
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingRxCcs(void)
{
  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  if (vCanCcGbt_ChargeState.ChargePhase.CcsMsg.IsChanged == TRUE)
  {
    uint16 voltgeOutput = vCanCcGbt_Get_CCS_VO(vCanCcGbt_ChargeState.ChargePhase.CcsMsg.Data);
    uint16 currentOutput = vCanCcGbt_Get_CCS_CO(vCanCcGbt_ChargeState.ChargePhase.CcsMsg.Data);
    uint16 chargingTime = vCanCcGbt_Get_CCS_CT(vCanCcGbt_ChargeState.ChargePhase.CcsMsg.Data);
    uint8 chargingPermissible = vCanCcGbt_Get_CCS_CP(vCanCcGbt_ChargeState.ChargePhase.CcsMsg.Data);

    vCanCcGbt_ChargeState.ChargePhase.CcsMsg.IsChanged = FALSE;

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    AppVCanCcGbt_ProvideChargerChargingState(voltgeOutput, currentOutput, chargingTime, chargingPermissible);
  }
  else
  {
    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }

  vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_CHARGE, E_VCANCCGBT_MSG_ACTION_CCS_CYCLIC);

  vCanCcGbt_InternalHandleTimeoutSupervision(&(vCanCcGbt_ChargeState.ChargePhase.CcsMsg.Timeout),
                                             E_VCANCCGBT_ERROR_CCS);
} /* vCanCcGbt_InternalChargingRxCcs() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingRxCst
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingRxCst(void)
{
  uint8 causeChargerSuspendingCharging = 0;

  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  if (vCanCcGbt_ChargeState.ChargePhase.CstMsg.IsChanged == TRUE)
  {
    uint16 faultCauseChargerSuspendingCharging = vCanCcGbt_Get_CST_FCCSC(vCanCcGbt_ChargeState.ChargePhase.CstMsg.Data);
    uint8 errorCauseChargerSuspendingCharging = vCanCcGbt_Get_CST_ECCSC(vCanCcGbt_ChargeState.ChargePhase.CstMsg.Data);

    causeChargerSuspendingCharging = vCanCcGbt_Get_CST_CCSC(vCanCcGbt_ChargeState.ChargePhase.CstMsg.Data);

    vCanCcGbt_ChargeState.ChargePhase.CstMsg.IsChanged = FALSE;

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    AppVCanCcGbt_ProvideChargerSuspendInformation(causeChargerSuspendingCharging, faultCauseChargerSuspendingCharging,
                                                  errorCauseChargerSuspendingCharging);
  }
  else
  {
    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }

  if (vCanCcGbt_Is_CST_CSR(causeChargerSuspendingCharging) == TRUE)
  {
    vCanCcGbt_ChargeState.ChargePhase.IsChargerSuspendReason = TRUE;

    vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_CHARGE, E_VCANCCGBT_MSG_ACTION_CST_CHARGER_CYCLIC);
  }

  if (vCanCcGbt_Is_CST_BMSSR(causeChargerSuspendingCharging) == TRUE)
  {
    vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_CHARGE, E_VCANCCGBT_MSG_ACTION_CST_BMS_CYCLIC);
  }

  vCanCcGbt_InternalHandleTimeoutSupervision(&(vCanCcGbt_ChargeState.ChargePhase.CstMsg.Timeout),
                                             E_VCANCCGBT_ERROR_CST);
} /* vCanCcGbt_InternalChargingRxCst() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingRx
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingRx(void)
{
  if (vCanCcGbt_ChargeState.ChargePhase.CcsMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalChargingRxCcs();
  }

  if (vCanCcGbt_ChargeState.ChargePhase.CstMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalChargingRxCst();
  }
} /* vCanCcGbt_InternalChargingRx() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingTxBcl
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingTxBcl(void)
{
  vCanCcGbt_IfTxPduStateType *pduState =
    &(vCanCcGbt_GetIfTxPduState(vCanCcGbt_GetIfTxPduStateIdxOfIfTxPdu(vCanCcGbt_GetBclTxPdu())));

  if (vCanCcGbt_InternalHandleIfPduSendTimeout(pduState) == TRUE)
  {
    vCanCcGbt_BCLDataType bclData;

    if (AppVCanCcGbt_GetChargingMode(&bclData) == E_OK)
    {
      vCanCcGbt_InternalSetBCLData(bclData);
    }

    if (vCanCcGbt_InternalSendIf
        (vCanCcGbt_ChargeState.ChargePhase.BclMsg.Data, VCANCCGBT_BCL_LENGTH, vCanCcGbt_GetBclTxPdu()) == E_OK)
    {
      SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

      pduState->SendTimeout = VCANCCGBT_BCL_SENDTIMEOUT;

      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();
    }
  }
} /* vCanCcGbt_InternalChargingTxBcl() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingTxBcs
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingTxBcs(void)
{
  vCanCcGbt_TpTxPduStateType *pduState =
    &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(vCanCcGbt_GetBcsTxPdu())));

  if (vCanCcGbt_InternalHandleTpPduSendTimeout(pduState) == TRUE)
  {
    vCanCcGbt_BCSDataType bcsData;

    if (AppVCanCcGbt_GetChargingStatus(&bcsData) == E_OK)
    {
      vCanCcGbt_InternalSetBCSData(bcsData);
    }

    /* TODO gsi: Shall be called with NULL_PTR for BcsMsg.Data but J1939TP has a bug (ESCAN00095631). */
    if (vCanCcGbt_InternalSendTp
        (vCanCcGbt_ChargeState.ChargePhase.BcsMsg.Data, VCANCCGBT_BCS_LENGTH, vCanCcGbt_GetBcsTxPdu()) == E_OK)
    {
      SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

      pduState->SendTimeout = VCANCCGBT_BCS_SENDTIMEOUT;

      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();
    }
  }
} /* vCanCcGbt_InternalChargingTxBcs() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingTxBsm
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingTxBsm(void)
{
  vCanCcGbt_IfTxPduStateType *pduState =
    &(vCanCcGbt_GetIfTxPduState(vCanCcGbt_GetIfTxPduStateIdxOfIfTxPdu(vCanCcGbt_GetBsmTxPdu())));

  if (vCanCcGbt_InternalHandleIfPduSendTimeout(pduState) == TRUE)
  {
    vCanCcGbt_BSMDataType bsmData;

    if (AppVCanCcGbt_GetBatteryStatus(&bsmData) == E_OK)
    {
      vCanCcGbt_InternalSetBSMData(bsmData);
    }

    if (vCanCcGbt_InternalSendIf
        (vCanCcGbt_ChargeState.ChargePhase.BsmMsg.Data, VCANCCGBT_BSM_LENGTH, vCanCcGbt_GetBsmTxPdu()) == E_OK)
    {
      SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

      pduState->SendTimeout = VCANCCGBT_BSM_SENDTIMEOUT;

      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();
    }
  }
} /* vCanCcGbt_InternalChargingTxBsm() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingTxBmv
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingTxBmv(void)
{
  vCanCcGbt_TpTxPduStateType *pduState =
    &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(vCanCcGbt_GetBmvTxPdu())));

  if (vCanCcGbt_InternalHandleTpPduSendTimeout(pduState) == TRUE)
  {
    vCanCcGbt_BMVDataType bmvData;

    if (AppVCanCcGbt_GetBatteryVoltages(&bmvData) == E_OK)
    {
      vCanCcGbt_InternalSetBMVData(bmvData);
    }

    /* TODO gsi: Shall be called with NULL_PTR for BmvMsg.Data but J1939TP has a bug (ESCAN00095631). */
    if (vCanCcGbt_InternalSendTp
        (vCanCcGbt_ChargeState.ChargePhase.BmvMsg.Data, vCanCcGbt_ChargeState.ChargePhase.BmvMsg.DataLength,
         vCanCcGbt_GetBmvTxPdu()) == E_OK)
    {
      SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

      pduState->SendTimeout = VCANCCGBT_BMV_SENDTIMEOUT;

      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();
    }
  }
} /* vCanCcGbt_InternalChargingTxBmv() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingTxBmt
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingTxBmt(void)
{
  vCanCcGbt_TpTxPduStateType *pduState =
    &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(vCanCcGbt_GetBmtTxPdu())));

  if (vCanCcGbt_InternalHandleTpPduSendTimeout(pduState) == TRUE)
  {
    vCanCcGbt_BMTDataType bmtData;

    if (AppVCanCcGbt_GetBatteryTemperatures(&bmtData) == E_OK)
    {
      vCanCcGbt_InternalSetBMTData(bmtData);
    }

    /* TODO gsi: Shall be called with NULL_PTR for BmtMsg.Data but J1939TP has a bug (ESCAN00095631). */
    if (vCanCcGbt_InternalSendTp
        (vCanCcGbt_ChargeState.ChargePhase.BmtMsg.Data, vCanCcGbt_ChargeState.ChargePhase.BmtMsg.DataLength,
         vCanCcGbt_GetBmtTxPdu()) == E_OK)
    {
      SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

      pduState->SendTimeout = VCANCCGBT_BMT_SENDTIMEOUT;

      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();
    }
  }
} /* vCanCcGbt_InternalChargingTxBmt() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingTxBsp
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingTxBsp(void)
{
  vCanCcGbt_TpTxPduStateType *pduState =
    &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(vCanCcGbt_GetBspTxPdu())));

  if (vCanCcGbt_InternalHandleTpPduSendTimeout(pduState) == TRUE)
  {
    vCanCcGbt_BSPDataType bspData;

    if (AppVCanCcGbt_GetReservedBatteries(&bspData) == E_OK)
    {
      vCanCcGbt_InternalSetBSPData(bspData);
    }

    /* TODO gsi: Shall be called with NULL_PTR for BspMsg.Data but J1939TP has a bug (ESCAN00095631). */
    if (vCanCcGbt_InternalSendTp
        (vCanCcGbt_ChargeState.ChargePhase.BspMsg.Data, vCanCcGbt_ChargeState.ChargePhase.BspMsg.DataLength,
         vCanCcGbt_GetBspTxPdu()) == E_OK)
    {
      SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

      pduState->SendTimeout = VCANCCGBT_BSP_SENDTIMEOUT;

      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();
    }
  }
} /* vCanCcGbt_InternalChargingTxBsp() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingTxBst
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingTxBst(void)
{
  vCanCcGbt_IfTxPduStateType *pduState =
    &(vCanCcGbt_GetIfTxPduState(vCanCcGbt_GetIfTxPduStateIdxOfIfTxPdu(vCanCcGbt_GetBstTxPdu())));

  if (vCanCcGbt_InternalHandleIfPduSendTimeout(pduState) == TRUE)
  {
    vCanCcGbt_BSTDataType bstData;

    if (AppVCanCcGbt_GetSuspendReason(&bstData) == E_OK)
    {
      vCanCcGbt_InternalSetBSTData(bstData);
    }

    if (vCanCcGbt_InternalSendIf
        (vCanCcGbt_ChargeState.ChargePhase.BstMsg.Data, VCANCCGBT_BST_LENGTH, vCanCcGbt_GetBstTxPdu()) == E_OK)
    {
      SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

      pduState->SendTimeout = VCANCCGBT_BST_SENDTIMEOUT;

      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();
    }

    if (vCanCcGbt_ChargeState.ChargePhase.IsChargerSuspendReason == TRUE)
    {
      vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_CHARGE, E_VCANCCGBT_MSG_ACTION_BST_TRANSMIT);
    }
  }
} /* vCanCcGbt_InternalChargingTxBst() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalChargingTx
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalChargingTx(void)
{
  if (vCanCcGbt_ChargeState.ChargePhase.BclMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalChargingTxBcl();
  }

  if (vCanCcGbt_ChargeState.ChargePhase.BcsMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalChargingTxBcs();
  }

  if (vCanCcGbt_ChargeState.ChargePhase.BsmMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalChargingTxBsm();
  }

  if (vCanCcGbt_ChargeState.ChargePhase.BmvMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalChargingTxBmv();
  }

  if (vCanCcGbt_ChargeState.ChargePhase.BmtMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalChargingTxBmt();
  }

  if (vCanCcGbt_ChargeState.ChargePhase.BspMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalChargingTxBsp();
  }

  if (vCanCcGbt_ChargeState.ChargePhase.BstMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalChargingTxBst();
  }
} /* vCanCcGbt_InternalChargingTx() */ /* PRQA S 6010 */ /* MD_MSR_STPTH */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalStatisticRxCrm
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalStatisticRxCrm(void)
{
  if (vCanCcGbt_ChargeState.ChargeStatisticPhase.CsdMsg.IsActive == TRUE)
  {
    SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    if (vCanCcGbt_ChargeState.ChargeStatisticPhase.CsdMsg.IsChanged == TRUE)
    {
      uint16 cumulativeChargingTime = vCanCcGbt_Get_CSD_CT(vCanCcGbt_ChargeState.ChargeStatisticPhase.CsdMsg.Data);
      uint16 outputEnergy = vCanCcGbt_Get_CSD_OE(vCanCcGbt_ChargeState.ChargeStatisticPhase.CsdMsg.Data);
      uint32 chargerNumber = vCanCcGbt_Get_CSD_CN(vCanCcGbt_ChargeState.ChargeStatisticPhase.CsdMsg.Data);

      vCanCcGbt_ChargeState.ChargeStatisticPhase.CsdMsg.IsChanged = FALSE;

      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

      AppVCanCcGbt_ProvideChargingStatistics(cumulativeChargingTime, outputEnergy, chargerNumber);
    }
    else
    {
      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
    }

    vCanCcGbt_InternalHandleTimeoutSupervision(&(vCanCcGbt_ChargeState.ChargeStatisticPhase.CsdMsg.Timeout),
                                               E_VCANCCGBT_ERROR_CSD);
  }
} /* vCanCcGbt_InternalStatisticRxCrm() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalStatisticRx
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalStatisticRx(void)
{
  vCanCcGbt_InternalStatisticRxCrm();
} /* vCanCcGbt_InternalStatisticRx() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalStatisticTxBsd
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalStatisticTxBsd(void)
{
  vCanCcGbt_IfTxPduStateType *pduState =
    &(vCanCcGbt_GetIfTxPduState(vCanCcGbt_GetIfTxPduStateIdxOfIfTxPdu(vCanCcGbt_GetBsdTxPdu())));

  if (vCanCcGbt_InternalHandleIfPduSendTimeout(pduState) == TRUE)
  {
    vCanCcGbt_BSDDataType bsdData;

    if (AppVCanCcGbt_GetChargingStatistics(&bsdData) == E_OK)
    {
      vCanCcGbt_InternalSetBSDData(bsdData);
    }

    if (vCanCcGbt_InternalSendIf
        (vCanCcGbt_ChargeState.ChargeStatisticPhase.BsdMsg.Data, VCANCCGBT_BSD_LENGTH, vCanCcGbt_GetBsdTxPdu()) == E_OK)
    {
      SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

      pduState->SendTimeout = VCANCCGBT_BSD_SENDTIMEOUT;

      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();
    }
  }
} /* vCanCcGbt_InternalStatisticTxBsd() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalStatisticTx
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalStatisticTx(void)
{
  if (vCanCcGbt_ChargeState.ChargeStatisticPhase.BsdMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalStatisticTxBsd();
  }
} /* vCanCcGbt_InternalStatisticTx() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalErrorRxCem
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalErrorRxCem(void)
{
  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  if (vCanCcGbt_ChargeState.ErrorPhase.CemMsg.IsChanged == TRUE)
  {
    uint8 timeoutValueBRM = vCanCcGbt_Get_CEM_BRMTimeout(vCanCcGbt_ChargeState.ErrorPhase.CemMsg.Data);
    uint8 timeoutValueBCP = vCanCcGbt_Get_CEM_BCPTimeout(vCanCcGbt_ChargeState.ErrorPhase.CemMsg.Data);
    uint8 timeoutValueBRO = vCanCcGbt_Get_CEM_BROTimeout(vCanCcGbt_ChargeState.ErrorPhase.CemMsg.Data);
    uint8 timeoutValueBCS = vCanCcGbt_Get_CEM_BCSTimeout(vCanCcGbt_ChargeState.ErrorPhase.CemMsg.Data);
    uint8 timeoutValueBSM = vCanCcGbt_Get_CEM_BSMTimeout(vCanCcGbt_ChargeState.ErrorPhase.CemMsg.Data);
    uint8 timeoutValueBST = vCanCcGbt_Get_CEM_BSTTimeout(vCanCcGbt_ChargeState.ErrorPhase.CemMsg.Data);
    uint8 timeoutValueBSD = vCanCcGbt_Get_CEM_BSDTimeout(vCanCcGbt_ChargeState.ErrorPhase.CemMsg.Data);

    vCanCcGbt_ChargeState.ErrorPhase.CemMsg.IsChanged = FALSE;

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    AppVCanCcGbt_ProvideChargerErrors(timeoutValueBRM, timeoutValueBCP, timeoutValueBRO, timeoutValueBCS,
                                      timeoutValueBSM, timeoutValueBST, timeoutValueBSD);
  }
  else
  {
    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }
} /* vCanCcGbt_InternalErrorRxCem() */ /* PRQA S 6010, 6080 */ /* MD_MSR_STPTH, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalErrorRx
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalErrorRx(void)
{
  if (vCanCcGbt_ChargeState.ErrorPhase.CemMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalErrorRxCem();
  }
} /* vCanCcGbt_InternalErrorRx() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalErrorTxBem
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalErrorTxBem(void)
{
  vCanCcGbt_IfTxPduStateType *pduState =
    &vCanCcGbt_GetIfTxPduState(vCanCcGbt_GetIfTxPduStateIdxOfIfTxPdu(vCanCcGbt_GetBemTxPdu()));

  if (vCanCcGbt_InternalHandleIfPduSendTimeout(pduState) == TRUE)
  {
    if (vCanCcGbt_InternalSendIf
        (vCanCcGbt_ChargeState.ErrorPhase.BemMsg.Data, VCANCCGBT_BEM_LENGTH, vCanCcGbt_GetBemTxPdu()) == E_OK)
    {
      SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();

      pduState->SendTimeout = VCANCCGBT_BRM_SENDTIMEOUT;

      SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_TXPDU();
    }
  }
} /* vCanCcGbt_InternalErrorTxBem() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalErrorTx
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalErrorTx(void)
{
  if (vCanCcGbt_ChargeState.ErrorPhase.BemMsg.IsActive == TRUE)
  {
    vCanCcGbt_InternalErrorTxBem();
  }
} /* vCanCcGbt_InternalErrorTx() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalHandleCharging
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InternalHandleCharging(void)
{
  if ((vCanCcGbt_ChargeState.InitPhase.IsChargerPlugged == TRUE) ||
      (vCanCcGbt_ChargeState.ChargeState == E_VCANCCGBT_STATE_INIT))
  {
    switch (vCanCcGbt_ChargeState.ChargeState)
    {
    case E_VCANCCGBT_STATE_INIT:
      vCanCcGbt_InternalProtInit();
      break;

    case E_VCANCCGBT_STATE_HANDSHAKE:
      vCanCcGbt_InternalHandshakeRx();
      vCanCcGbt_InternalHandshakeTx();
      break;

    case E_VCANCCGBT_STATE_PARAM_EXCHANGE:
      vCanCcGbt_InternalParamExRx();
      vCanCcGbt_InternalParamExTx();
      break;

    case E_VCANCCGBT_STATE_CHARGE:
      vCanCcGbt_InternalChargingRx();
      vCanCcGbt_InternalChargingTx();
      break;

    case E_VCANCCGBT_STATE_CHARGE_STATISTIC:
      vCanCcGbt_InternalStatisticRx();
      vCanCcGbt_InternalStatisticTx();
      break;

    case E_VCANCCGBT_STATE_ERROR:
      vCanCcGbt_InternalErrorRx();
      vCanCcGbt_InternalErrorTx();
      break;

    default: /* PRQA S 2018 */ /* MD_MSR_14.1 */
      break;
    }
  }
} /* vCanCcGbt_InternalHandleCharging() */ /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCHM
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCHM(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr)
{
  if ((vCanCcGbt_ChargeState.ChargeState == E_VCANCCGBT_STATE_HANDSHAKE) &&
      (vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.IsActive == FALSE))
  {
    SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_HANDSHAKE, E_VCANCCGBT_MSG_ACTION_CHM_RECEIVED);

    vCanCcGbt_ChargeState.HandshakePhase.ChmMsg.Timeout = VCANCCGBT_RX_TIMEOUT_SUPERVISION_5S;

    if (vCanCcGbt_InternalCompareData
        (vCanCcGbt_ChargeState.HandshakePhase.ChmMsg.Data, PduInfoPtr->SduDataPtr, VCANCCGBT_CHM_LENGTH) == FALSE)
    {
      VStdLib_MemCpy(vCanCcGbt_ChargeState.HandshakePhase.ChmMsg.Data, PduInfoPtr->SduDataPtr, VCANCCGBT_CHM_LENGTH);

      vCanCcGbt_ChargeState.HandshakePhase.ChmMsg.IsChanged = TRUE;
    }

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }
#if (VCANCCGBT_RUNTIME_ERROR_REPORT == STD_ON)
  else
  {
    vCanCcGbt_ReportDetError(VCANCCGBT_SID_RXINDICATION, VCANCCGBT_E_CHM_UNTIMELY);
  }
#endif
} /* vCanCcGbt_InternalRxCHM() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCRM
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCRM(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr)
{
  if ((vCanCcGbt_ChargeState.ChargeState == E_VCANCCGBT_STATE_HANDSHAKE) ||
      (vCanCcGbt_ChargeState.ChargeState == E_VCANCCGBT_STATE_ERROR) ||
      (vCanCcGbt_ChargeState.ChargeState == E_VCANCCGBT_STATE_CHARGE_STATISTIC))
  {
    vCanCcGbt_InternalHandleMsgAction(vCanCcGbt_ChargeState.ChargeState, E_VCANCCGBT_MSG_ACTION_CRM_RECEIVED);

    SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.Timeout = VCANCCGBT_RX_TIMEOUT_SUPERVISION_5S;

    if (vCanCcGbt_InternalCompareData
        (vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.Data, PduInfoPtr->SduDataPtr, VCANCCGBT_CRM_LENGTH) == FALSE)
    {
      VStdLib_MemCpy(vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.Data, PduInfoPtr->SduDataPtr, VCANCCGBT_CRM_LENGTH);

      vCanCcGbt_ChargeState.HandshakePhase.CrmMsg.IsChanged = TRUE;
    }

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }
#if (VCANCCGBT_RUNTIME_ERROR_REPORT == STD_ON)
  else
  {
    vCanCcGbt_ReportDetError(VCANCCGBT_SID_RXINDICATION, VCANCCGBT_E_CRM_UNTIMELY);
  }
#endif
} /* vCanCcGbt_InternalRxCRM() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCEM
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCEM(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr)
{
  SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

  vCanCcGbt_InternalHandleMsgAction(vCanCcGbt_ChargeState.ChargeState, E_VCANCCGBT_MSG_ACTION_CEM_RECEIVED);

  if (vCanCcGbt_InternalCompareData
      (vCanCcGbt_ChargeState.ErrorPhase.CemMsg.Data, PduInfoPtr->SduDataPtr, VCANCCGBT_CEM_LENGTH) == FALSE)
  {
    VStdLib_MemCpy(vCanCcGbt_ChargeState.ErrorPhase.CemMsg.Data, PduInfoPtr->SduDataPtr, VCANCCGBT_CEM_LENGTH);

    vCanCcGbt_ChargeState.ErrorPhase.CemMsg.IsChanged = TRUE;
  }

  SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
} /* vCanCcGbt_InternalRxCEM() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCTS
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCTS(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr)
{
  if (vCanCcGbt_ChargeState.ChargeState == E_VCANCCGBT_STATE_PARAM_EXCHANGE)
  {
    SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_PARAM_EXCHANGE, E_VCANCCGBT_MSG_ACTION_CTS_RECEIVED);

    vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.Timeout = VCANCCGBT_RX_TIMEOUT_SUPERVISION_5S;

    if (vCanCcGbt_InternalCompareData
        (vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.Data, PduInfoPtr->SduDataPtr,
         VCANCCGBT_CTS_LENGTH) == FALSE)
    {
      VStdLib_MemCpy(vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.Data, PduInfoPtr->SduDataPtr,
                     VCANCCGBT_CTS_LENGTH);

      vCanCcGbt_ChargeState.ParameterExchangePhase.CtsMsg.IsChanged = TRUE;
    }

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }
#if (VCANCCGBT_RUNTIME_ERROR_REPORT == STD_ON)
  else
  {
    vCanCcGbt_ReportDetError(VCANCCGBT_SID_RXINDICATION, VCANCCGBT_E_CTS_UNTIMELY);
  }
#endif
} /* vCanCcGbt_InternalRxCTS() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCML
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCML(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr)
{
  if (vCanCcGbt_ChargeState.ChargeState == E_VCANCCGBT_STATE_PARAM_EXCHANGE)
  {
    SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_PARAM_EXCHANGE, E_VCANCCGBT_MSG_ACTION_CML_RECEIVED);

    vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.Timeout = VCANCCGBT_RX_TIMEOUT_SUPERVISION_5S;

    if (vCanCcGbt_InternalCompareData
        (vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.Data, PduInfoPtr->SduDataPtr,
         VCANCCGBT_CML_LENGTH) == FALSE)
    {
      VStdLib_MemCpy(vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.Data, PduInfoPtr->SduDataPtr,
                     VCANCCGBT_CML_LENGTH);

      vCanCcGbt_ChargeState.ParameterExchangePhase.CmlMsg.IsChanged = TRUE;
    }

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }
#if (VCANCCGBT_RUNTIME_ERROR_REPORT == STD_ON)
  else
  {
    vCanCcGbt_ReportDetError(VCANCCGBT_SID_RXINDICATION, VCANCCGBT_E_CML_UNTIMELY);
  }
#endif
} /* vCanCcGbt_InternalRxCML() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCRO
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCRO(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr)
{
  if (vCanCcGbt_ChargeState.ChargeState == E_VCANCCGBT_STATE_PARAM_EXCHANGE)
  {
    SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_PARAM_EXCHANGE, E_VCANCCGBT_MSG_ACTION_CRO_RECEIVED);

    vCanCcGbt_ChargeState.ParameterExchangePhase.CroMsg.Timeout = VCANCCGBT_RX_TIMEOUT_SUPERVISION_5S;

    if (vCanCcGbt_InternalCompareData
        (vCanCcGbt_ChargeState.ParameterExchangePhase.CroMsg.Data, PduInfoPtr->SduDataPtr,
         VCANCCGBT_CRO_LENGTH) == FALSE)
    {
      VStdLib_MemCpy(vCanCcGbt_ChargeState.ParameterExchangePhase.CroMsg.Data, PduInfoPtr->SduDataPtr,
                     VCANCCGBT_CRO_LENGTH);

      vCanCcGbt_ChargeState.ParameterExchangePhase.CroMsg.IsChanged = TRUE;
    }

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }
#if (VCANCCGBT_RUNTIME_ERROR_REPORT == STD_ON)
  else
  {
    vCanCcGbt_ReportDetError(VCANCCGBT_SID_RXINDICATION, VCANCCGBT_E_CRO_UNTIMELY);
  }
#endif
} /* vCanCcGbt_InternalRxCRO() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCCS
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCCS(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr)
{
  if ((vCanCcGbt_ChargeState.ChargeState == E_VCANCCGBT_STATE_CHARGE) &&
      (vCanCcGbt_ChargeState.ChargePhase.CstMsg.IsActive == FALSE))
  {
    SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_CHARGE, E_VCANCCGBT_MSG_ACTION_CCS_RECEIVED);

    vCanCcGbt_ChargeState.ChargePhase.CcsMsg.Timeout = VCANCCGBT_RX_TIMEOUT_SUPERVISION_1S;

    if (vCanCcGbt_InternalCompareData
        (vCanCcGbt_ChargeState.ChargePhase.CcsMsg.Data, PduInfoPtr->SduDataPtr, VCANCCGBT_CCS_LENGTH) == FALSE)
    {
      VStdLib_MemCpy(vCanCcGbt_ChargeState.ChargePhase.CcsMsg.Data, PduInfoPtr->SduDataPtr, VCANCCGBT_CCS_LENGTH);

      vCanCcGbt_ChargeState.ChargePhase.CcsMsg.IsChanged = TRUE;
    }

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }
#if (VCANCCGBT_RUNTIME_ERROR_REPORT == STD_ON)
  else
  {
    vCanCcGbt_ReportDetError(VCANCCGBT_SID_RXINDICATION, VCANCCGBT_E_CCS_UNTIMELY);
  }
#endif
} /* vCanCcGbt_InternalRxCCS() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCST
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCST(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr)
{
  if (vCanCcGbt_ChargeState.ChargeState == E_VCANCCGBT_STATE_CHARGE)
  {
    SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_CHARGE, E_VCANCCGBT_MSG_ACTION_CST_RECEIVED);

    vCanCcGbt_ChargeState.ChargePhase.CstMsg.Timeout = VCANCCGBT_RX_TIMEOUT_SUPERVISION_5S;

    if (vCanCcGbt_InternalCompareData
        (vCanCcGbt_ChargeState.ChargePhase.CstMsg.Data, PduInfoPtr->SduDataPtr, VCANCCGBT_CST_LENGTH) == FALSE)
    {
      VStdLib_MemCpy(vCanCcGbt_ChargeState.ChargePhase.CstMsg.Data, PduInfoPtr->SduDataPtr, VCANCCGBT_CST_LENGTH);

      vCanCcGbt_ChargeState.ChargePhase.CstMsg.IsChanged = TRUE;
    }

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }
#if (VCANCCGBT_RUNTIME_ERROR_REPORT == STD_ON)
  else
  {
    vCanCcGbt_ReportDetError(VCANCCGBT_SID_RXINDICATION, VCANCCGBT_E_CST_UNTIMELY);
  }
#endif
} /* vCanCcGbt_InternalRxCST() */

/**********************************************************************************************************************
 *  vCanCcGbt_InternalRxCSD
 *********************************************************************************************************************/
VCANCCGBT_LOCAL_INLINE FUNC(void,
                            VCANCCGBT_CODE) vCanCcGbt_InternalRxCSD(P2CONST(PduInfoType, AUTOMATIC,
                                                                            VCANCCGBT_APPL_DATA) PduInfoPtr)
{
  if (vCanCcGbt_ChargeState.ChargeState == E_VCANCCGBT_STATE_CHARGE_STATISTIC)
  {
    SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    vCanCcGbt_InternalHandleMsgAction(E_VCANCCGBT_STATE_CHARGE_STATISTIC, E_VCANCCGBT_MSG_ACTION_CSD_RECEIVED);

    vCanCcGbt_ChargeState.ChargeStatisticPhase.CsdMsg.Timeout = VCANCCGBT_RX_TIMEOUT_SUPERVISION_5S;

    if (vCanCcGbt_InternalCompareData
        (vCanCcGbt_ChargeState.ChargeStatisticPhase.CsdMsg.Data, PduInfoPtr->SduDataPtr, VCANCCGBT_CSD_LENGTH) == FALSE)
    {
      VStdLib_MemCpy(vCanCcGbt_ChargeState.ChargeStatisticPhase.CsdMsg.Data, PduInfoPtr->SduDataPtr,
                     VCANCCGBT_CSD_LENGTH);

      vCanCcGbt_ChargeState.ChargeStatisticPhase.CsdMsg.IsChanged = TRUE;
    }

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }
#if (VCANCCGBT_RUNTIME_ERROR_REPORT == STD_ON)
  else
  {
    vCanCcGbt_ReportDetError(VCANCCGBT_SID_RXINDICATION, VCANCCGBT_E_CSD_UNTIMELY);
  }
#endif
} /* vCanCcGbt_InternalRxCSD() */

/*============================================== VCANCCGBT_STOP_SEC_CODE ============================================*/
#define VCANCCGBT_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/*============================================== VCANCCGBT_START_SEC_CODE ===========================================*/
#define VCANCCGBT_START_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
 *  vCanCcGbt_InitMemory
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_InitMemory(void)
{
  /* #-- Set the vCanCcGbt to not initialized. */
  vCanCcGbt_ModuleInitialized = VCANCCGBT_UNINIT;
} /* vCanCcGbt_InitMemory() */

/**********************************************************************************************************************
 *  vCanCcGbt_Init
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_Init(P2CONST(vCanCcGbt_ConfigType, AUTOMATIC, VCANCCGBT_APPL_DATA) config)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCGBT_E_NO_ERROR;

#if (VCANCCGBT_USE_INIT_POINTER == STD_ON)
  vCanCcGbt_ConfigDataPtr = config;
#else
  VCANCCGBT_DUMMY_STATEMENT(config); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
#endif

  /* ----- Development Error Checks ------------------------------------- */

  /* #-- Check that the vCanCcGbt is not initialized yet. */
  if (vCanCcGbt_ModuleInitialized != VCANCCGBT_UNINIT)
  {
    errorId = VCANCCGBT_E_REINIT;
  }
#if (VCANCCGBT_USE_INIT_POINTER == STD_ON)
  /* #-- If "VCANCCGBT_USE_INIT_POINTER" is enabled, Check that the configuration data pointer is set. */
  else if (vCanCcGbt_ConfigDataPtr == NULL_PTR)
  {
# if (VCANCCGBT_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
    EcuM_BswErrorHook(VCANCCGBT_MODULE_ID, ECUM_BSWERROR_NULLPTR);
# endif
    errorId = VCANCCGBT_E_PARAM_POINTER;
  }
# if (VCANCCGBT_FINALMAGICNUMBER == STD_ON)
  else if (vCanCcGbt_GetFinalMagicNumber() != VCANCCGBT_FINAL_MAGIC_NUMBER)
  {
#  if (VCANCCGBT_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
    EcuM_BswErrorHook(VCANCCGBT_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
#  endif
    errorId = VCANCCGBT_E_INIT_FAILED;
  }
# endif
#endif
  else
  {
#if ((VCANCCGBT_USE_INIT_POINTER == STD_OFF) && (VCANCCGBT_DEV_ERROR_DETECT == STD_ON))
    /* #-- Otherwise, check that the configuration data pointer is not set. */
    if (config != NULL_PTR)
    {
      errorId = VCANCCGBT_E_PARAM_POINTER;
    }
#endif
    /* #-- If the initial checks are passed or not required: */

    /* ----- Implementation ----------------------------------------------- */

    /* Initialization of the init phase */
    vCanCcGbt_ChargeState.InitPhase.IsChargerPlugged = FALSE;

    /* Initialization of the handshake phase */
    vCanCcGbt_InternalStateReset(E_VCANCCGBT_STATE_HANDSHAKE);

    /* Initialization of the parameter exchange phase */
    vCanCcGbt_InternalStateReset(E_VCANCCGBT_STATE_PARAM_EXCHANGE);

    /* Initialization of the charging phase */
    vCanCcGbt_InternalStateReset(E_VCANCCGBT_STATE_CHARGE);

    /* Initialization of the charge statistic phase */
    vCanCcGbt_InternalStateReset(E_VCANCCGBT_STATE_CHARGE_STATISTIC);

    /* Initialization of the error handling */
    vCanCcGbt_InternalStateReset(E_VCANCCGBT_STATE_ERROR);

    /* General initialization */
    vCanCcGbt_ChargeState.ChargeState = E_VCANCCGBT_STATE_INIT;
    vCanCcGbt_ModuleInitialized = VCANCCGBT_INIT;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (VCANCCGBT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCGBT_E_NO_ERROR)
  {
    vCanCcGbt_ReportDetError(VCANCCGBT_SID_INIT, errorId);
  }
#else
  VCANCCGBT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint !e438 */
#endif
} /* vCanCcGbt_Init() */

/**********************************************************************************************************************
 * vCanCcGbt_DeInit()
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_DeInit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCGBT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
/* #-- If VCANCCGBT_DEV_ERROR_DETECT is enabled: */
#if (VCANCCGBT_DEV_ERROR_DETECT == STD_ON)
  /* #-- Check that the vCanCcGbt is initialized. */
  if (vCanCcGbt_ModuleInitialized != VCANCCGBT_INIT)
  {
    errorId = VCANCCGBT_E_UNINIT;
  }
#endif
  /* ----- Implementation ----------------------------------------------- */

  /* #-- Set state of vCanCcGbt to not initialized. */
  vCanCcGbt_ModuleInitialized = VCANCCGBT_UNINIT;

  /* ----- Development Error Report ------------------------------------- */
#if (VCANCCGBT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCGBT_E_NO_ERROR) /* PRQA S 3356, 3359 */ /* MD_MSR_14.1 */
  {
    vCanCcGbt_ReportDetError(VCANCCGBT_SID_DEINIT, errorId);
  }
#else
  VCANCCGBT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint !e438 */
#endif
} /* vCanCcGbt_DeInit() */

#if (VCANCCGBT_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  vCanCcGbt_GetVersionInfo
 *********************************************************************************************************************/
FUNC(void,
     VCANCCGBT_CODE) vCanCcGbt_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, VCANCCGBT_APPL_DATA) versionInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCGBT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (VCANCCGBT_DEV_ERROR_DETECT == STD_ON)
  /* #-- Check if version information pointer is set. */
  if (versionInfo == NULL_PTR)
  {
    errorId = VCANCCGBT_E_PARAM_POINTER;
  }
  /* #-- If the initial checks are passed or not required: */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    versionInfo->vendorID = VCANCCGBT_VENDOR_ID;
    versionInfo->moduleID = VCANCCGBT_MODULE_ID;
    versionInfo->sw_major_version = VCANCCGBT_SW_MAJOR_VERSION;
    versionInfo->sw_minor_version = VCANCCGBT_SW_MINOR_VERSION;
    versionInfo->sw_patch_version = VCANCCGBT_SW_PATCH_VERSION;
  }

  /* ----- Development Error Report ------------------------------------- */
# if (VCANCCGBT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCGBT_E_NO_ERROR)
  {
    vCanCcGbt_ReportDetError(VCANCCGBT_SID_GETVERSIONINFO, errorId);
  }
# else
  VCANCCGBT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint !e438 */
# endif
} /* vCanCcGbt_GetVersionInfo() */
#endif /* #if (VCANCCGBT_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 *  vCanCcGbt_MainFunction
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_MainFunction(void)
{
/* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCGBT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #-- Check that the vCanCcGbt is initialized. */
  if (vCanCcGbt_ModuleInitialized != VCANCCGBT_INIT)
  {
#if (VCANCCGBT_DEV_ERROR_DETECT == STD_ON)
    errorId = VCANCCGBT_E_UNINIT;
#endif
  }
  /* #-- If the initial checks are passed or not required: */
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    vCanCcGbt_InternalHandleCharging();
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VCANCCGBT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCGBT_E_NO_ERROR)
  {
    vCanCcGbt_ReportDetError(VCANCCGBT_SID_MAINFUNCTION, errorId);
  }
#else
  VCANCCGBT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint !e438 */
#endif
} /* vCanCcGbt_MainFunction() */

/**********************************************************************************************************************
 *  vCanCcGbt_ChargerPlugged
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_ChargerPlugged(boolean isPlugged)
{
  vCanCcGbt_ChargeState.InitPhase.IsChargerPlugged = isPlugged;
} /* vCanCcGbt_ChargerPlugged() */

/**********************************************************************************************************************
 *  vCanCcGbt_SuspendCharging
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_SuspendCharging(boolean isSuspended)
{
  vCanCcGbt_ChargeState.ChargePhase.IsChargingSuspended = isSuspended;

  if (isSuspended == TRUE)
  {
    vCanCcGbt_InternalHandleMsgAction(vCanCcGbt_ChargeState.ChargeState, E_VCANCCGBT_MSG_ACTION_SUSPENDCHARGING);
  }
} /* vCanCcGbt_SuspendCharging() */

/**********************************************************************************************************************
 *  vCanCcGbt_RxIndication
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_RxIndication(PduIdType rxPduId,
                                                  P2CONST(PduInfoType, AUTOMATIC, VCANCCGBT_APPL_DATA) pduInfoPtr)
{
  uint8 errorId = VCANCCGBT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #-- Check that the vCanCcGbt is initialized. */
  if (vCanCcGbt_ModuleInitialized != VCANCCGBT_INIT)
  {
    errorId = VCANCCGBT_E_UNINIT;
  }
#if (VCANCCGBT_DEV_ERROR_DETECT == STD_ON)
  /* #-- Check if the ID of the received PDU is valid. */
  else if ((rxPduId >= vCanCcGbt_GetSizeOfRxPdu())
# if (VCANCCGBT_INVALIDHNDOFRXPDU == STD_ON)
           || (vCanCcGbt_IsInvalidHndOfRxPdu(rxPduId))
# endif
    )
  {
    errorId = VCANCCGBT_E_INVALID_PDU_SDU_ID;
  }
  /* #-- Check if the Pdu info pointer is set. */
  else if (pduInfoPtr == NULL_PTR)
  {
    errorId = VCANCCGBT_E_PARAM_POINTER;
  }
  /* #-- Check if the Sdu data pointer is set. */
  else if (pduInfoPtr->SduDataPtr == NULL_PTR)
  {
    errorId = VCANCCGBT_E_PARAM_POINTER;
  }
#endif
  /* #-- If the initial checks are passed or not required: */
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    switch (vCanCcGbt_GetPduTypeOfRxPdu(rxPduId))
    {
    case VCANCCGBT_PDU_CHM_PDUTYPEOFRXPDU:
      vCanCcGbt_InternalRxCHM(pduInfoPtr);
      break;

    case VCANCCGBT_PDU_CRM_PDUTYPEOFRXPDU:
      vCanCcGbt_InternalRxCRM(pduInfoPtr);
      break;

    case VCANCCGBT_PDU_CTS_PDUTYPEOFRXPDU:
      vCanCcGbt_InternalRxCTS(pduInfoPtr);
      break;

    case VCANCCGBT_PDU_CML_PDUTYPEOFRXPDU:
      vCanCcGbt_InternalRxCML(pduInfoPtr);
      break;

    case VCANCCGBT_PDU_CRO_PDUTYPEOFRXPDU:
      vCanCcGbt_InternalRxCRO(pduInfoPtr);
      break;

    case VCANCCGBT_PDU_CCS_PDUTYPEOFRXPDU:
      vCanCcGbt_InternalRxCCS(pduInfoPtr);
      break;

    case VCANCCGBT_PDU_CST_PDUTYPEOFRXPDU:
      vCanCcGbt_InternalRxCST(pduInfoPtr);
      break;

    case VCANCCGBT_PDU_CSD_PDUTYPEOFRXPDU:
      vCanCcGbt_InternalRxCSD(pduInfoPtr);
      break;

    case VCANCCGBT_PDU_CEM_PDUTYPEOFRXPDU:
      vCanCcGbt_InternalRxCEM(pduInfoPtr);
      break;

    default:
      break;
    }
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VCANCCGBT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCGBT_E_NO_ERROR)
  {
    vCanCcGbt_ReportDetError(VCANCCGBT_SID_RXINDICATION, errorId);
  }
#else
  VCANCCGBT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint !e438 */
#endif
} /* vCanCcGbt_RxIndication() */ /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  vCanCcGbt_CopyTxData
 *********************************************************************************************************************/
/* PRQA S 3673 8 */ /* MD_MSR_16.7 */
FUNC(BufReq_ReturnType, VCANCCGBT_CODE) vCanCcGbt_CopyTxData(PduIdType id,
                                                             P2VAR(PduInfoType, AUTOMATIC, VCANCCGBT_APPL_DATA) info,
                                                             P2VAR(RetryInfoType, AUTOMATIC, VCANCCGBT_APPL_DATA) retry,
                                                             P2VAR(PduLengthType, AUTOMATIC,
                                                                   VCANCCGBT_APPL_DATA) availableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint8 i;
  uint8 errorId = VCANCCGBT_E_NO_ERROR;

  VCANCCGBT_DUMMY_STATEMENT(retry); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  /* ----- Development Error Checks ------------------------------------- */
  /* #-- Check that the vCanCcGbt is initialized. */
  if (vCanCcGbt_ModuleInitialized != VCANCCGBT_INIT)
  {
    errorId = VCANCCGBT_E_UNINIT;
  }
#if (VCANCCGBT_DEV_ERROR_DETECT == STD_ON)
  /* #-- Check if the ID of the PDU is valid. */
  else if ((id >= vCanCcGbt_GetSizeOfTpTxPdu())
# if (VCANCCGBT_INVALIDHNDOFTPTXPDU == STD_ON)
           || (vCanCcGbt_IsInvalidHndOfTpTxPdu(id))
# endif
    )
  {
    errorId = VCANCCGBT_E_INVALID_PDU_SDU_ID;
  }
  /* #-- Check if the Pdu info pointer is set. */
  else if (info == NULL_PTR)
  {
    errorId = VCANCCGBT_E_PARAM_POINTER;
  }
  /* #-- Check if the Pdu info Sdu pointer is set. */
  else if (info->SduDataPtr == NULL_PTR)
  {
    errorId = VCANCCGBT_E_PARAM_POINTER;
  }
  /* #-- Check if the retry pointer is not set */
  else if (retry != NULL_PTR)
  {
    errorId = VCANCCGBT_E_PARAM_POINTER;
  }
#endif
  /* #-- If the initial checks are passed or not required: */
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    SchM_Enter_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();

    switch (vCanCcGbt_ChargeState.ChargeState)
    {
    case E_VCANCCGBT_STATE_HANDSHAKE:
      if (vCanCcGbt_GetPduTypeOfTpTxPdu(id) == VCANCCGBT_PDU_BRM_PDUTYPEOFTPTXPDU)
      {
        vCanCcGbt_TpTxPduStateType *pduState = &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(id)));

        if (pduState->IsLocked == TRUE)
        {
          if ((vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.ReadPos + info->SduLength) <=
              vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.DataLength)
          {
            for (i = 0; i < info->SduLength; i++)
            {
              info->SduDataPtr[i] =
                vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.Data[vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.ReadPos];
              vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.ReadPos++;
            }

            *availableDataPtr =
              vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.DataLength -
              vCanCcGbt_ChargeState.HandshakePhase.BrmMsg.ReadPos;

            retVal = BUFREQ_OK;
          }
        }
      }

      break;

    case E_VCANCCGBT_STATE_PARAM_EXCHANGE:
      if (vCanCcGbt_GetPduTypeOfTpTxPdu(id) == VCANCCGBT_PDU_BCP_PDUTYPEOFTPTXPDU)
      {
        vCanCcGbt_TpTxPduStateType *pduState = &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(id)));

        if (pduState->IsLocked == TRUE)
        {
          if ((vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.ReadPos + info->SduLength) <= VCANCCGBT_BCP_LENGTH)
          {
            for (i = 0; i < info->SduLength; i++)
            {
              info->SduDataPtr[i] =
                vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.Data[vCanCcGbt_ChargeState.ParameterExchangePhase.
                                                                         BcpMsg.ReadPos];
              vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.ReadPos++;
            }

            *availableDataPtr = VCANCCGBT_BCP_LENGTH - vCanCcGbt_ChargeState.ParameterExchangePhase.BcpMsg.ReadPos;

            retVal = BUFREQ_OK;
          }
        }
      }

      break;

    case E_VCANCCGBT_STATE_CHARGE:
      if (vCanCcGbt_GetPduTypeOfTpTxPdu(id) == VCANCCGBT_PDU_BCS_PDUTYPEOFTPTXPDU)
      {
        vCanCcGbt_TpTxPduStateType *pduState = &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(id)));

        if (pduState->IsLocked == TRUE)
        {
          if ((vCanCcGbt_ChargeState.ChargePhase.BcsMsg.ReadPos + info->SduLength) <= VCANCCGBT_BCS_LENGTH)
          {
            for (i = 0; i < info->SduLength; i++)
            {
              info->SduDataPtr[i] =
                vCanCcGbt_ChargeState.ChargePhase.BcsMsg.Data[vCanCcGbt_ChargeState.ChargePhase.BcsMsg.ReadPos];
              vCanCcGbt_ChargeState.ChargePhase.BcsMsg.ReadPos++;
            }

            *availableDataPtr = VCANCCGBT_BCS_LENGTH - vCanCcGbt_ChargeState.ChargePhase.BcsMsg.ReadPos;

            retVal = BUFREQ_OK;
          }
        }
      }
      else if (vCanCcGbt_GetPduTypeOfTpTxPdu(id) == VCANCCGBT_PDU_BMT_PDUTYPEOFTPTXPDU)
      {
        vCanCcGbt_TpTxPduStateType *pduState = &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(id)));

        if (pduState->IsLocked == TRUE)
        {
          if ((vCanCcGbt_ChargeState.ChargePhase.BmtMsg.ReadPos + info->SduLength) <=
              vCanCcGbt_ChargeState.ChargePhase.BmtMsg.DataLength)
          {
            for (i = 0; i < info->SduLength; i++)
            {
              info->SduDataPtr[i] =
                vCanCcGbt_ChargeState.ChargePhase.BmtMsg.Data[vCanCcGbt_ChargeState.ChargePhase.BmtMsg.ReadPos];
              vCanCcGbt_ChargeState.ChargePhase.BmtMsg.ReadPos++;
            }

            *availableDataPtr =
              vCanCcGbt_ChargeState.ChargePhase.BmtMsg.DataLength - vCanCcGbt_ChargeState.ChargePhase.BmtMsg.ReadPos;

            retVal = BUFREQ_OK;
          }
        }
      }
      else if (vCanCcGbt_GetPduTypeOfTpTxPdu(id) == VCANCCGBT_PDU_BMV_PDUTYPEOFTPTXPDU)
      {
        vCanCcGbt_TpTxPduStateType *pduState = &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(id)));

        if (pduState->IsLocked == TRUE)
        {
          if ((vCanCcGbt_ChargeState.ChargePhase.BmvMsg.ReadPos + info->SduLength) <=
              vCanCcGbt_ChargeState.ChargePhase.BmvMsg.DataLength)
          {
            for (i = 0; i < info->SduLength; i++)
            {
              info->SduDataPtr[i] =
                vCanCcGbt_ChargeState.ChargePhase.BmvMsg.Data[vCanCcGbt_ChargeState.ChargePhase.BmvMsg.ReadPos];
              vCanCcGbt_ChargeState.ChargePhase.BmvMsg.ReadPos++;
            }

            *availableDataPtr =
              vCanCcGbt_ChargeState.ChargePhase.BmvMsg.DataLength - vCanCcGbt_ChargeState.ChargePhase.BmvMsg.ReadPos;

            retVal = BUFREQ_OK;
          }
        }
      }
      else if (vCanCcGbt_GetPduTypeOfTpTxPdu(id) == VCANCCGBT_PDU_BSP_PDUTYPEOFTPTXPDU)
      {
        vCanCcGbt_TpTxPduStateType *pduState = &(vCanCcGbt_GetTpTxPduState(vCanCcGbt_GetTpTxPduStateIdxOfTpTxPdu(id)));

        if (pduState->IsLocked == TRUE)
        {
          if ((vCanCcGbt_ChargeState.ChargePhase.BspMsg.ReadPos + info->SduLength) <=
              vCanCcGbt_ChargeState.ChargePhase.BspMsg.DataLength)
          {
            for (i = 0; i < info->SduLength; i++)
            {
              info->SduDataPtr[i] =
                vCanCcGbt_ChargeState.ChargePhase.BspMsg.Data[vCanCcGbt_ChargeState.ChargePhase.BspMsg.ReadPos];
              vCanCcGbt_ChargeState.ChargePhase.BspMsg.ReadPos++;
            }

            *availableDataPtr =
              vCanCcGbt_ChargeState.ChargePhase.BspMsg.DataLength - vCanCcGbt_ChargeState.ChargePhase.BspMsg.ReadPos;

            retVal = BUFREQ_OK;
          }
        }
      }
      else
      {
        /* Intentionally left empty, only needed for Misra */
      }

      break;

    case E_VCANCCGBT_STATE_INIT:
    case E_VCANCCGBT_STATE_CHARGE_STATISTIC:
    case E_VCANCCGBT_STATE_ERROR:
      break;

    default: /* PRQA S 2018 */ /* MD_MSR_14.1 */
      break;
    }

    SchM_Exit_vCanCcGbt_VCANCCGBT_EXCLUSIVE_AREA_STATE();
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VCANCCGBT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCGBT_E_NO_ERROR)
  {
    vCanCcGbt_ReportDetError(VCANCCGBT_SID_COPYTXDATA, errorId);
  }
#else
  VCANCCGBT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint !e438 */
#endif

  return retVal;
} /* vCanCcGbt_CopyTxData() */ /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  vCanCcGbt_TxConfirmation
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_TxConfirmation(PduIdType txPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCGBT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #-- Check that the vCanCcGbt is initialized. */
  if (vCanCcGbt_ModuleInitialized != VCANCCGBT_INIT)
  {
    errorId = VCANCCGBT_E_UNINIT;
  }
#if (VCANCCGBT_DEV_ERROR_DETECT == STD_ON)
  /* #-- Check if the ID of the transmitted PDU is valid. */
  else if ((txPduId >= vCanCcGbt_GetSizeOfIfTxPdu())
# if (VCANCCGBT_INVALIDHNDOFIFTXPDU == STD_ON)
           || (vCanCcGbt_IsInvalidHndOfIfTxPdu(txPduId))
# endif
    )
  {
    errorId = VCANCCGBT_E_INVALID_PDU_SDU_ID;
  }
#endif
  /* #-- If the initial checks are passed or not required: */
  else
  {
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VCANCCGBT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCGBT_E_NO_ERROR)
  {
    vCanCcGbt_ReportDetError(VCANCCGBT_SID_TXCONFIRMATION, errorId);
  }
#else
  VCANCCGBT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint !e438 */
#endif
} /* vCanCcGbt_TxConfirmation() */

/**********************************************************************************************************************
 *  vCanCcGbt_TpTxConfirmation
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_TpTxConfirmation(PduIdType txPduId, Std_ReturnType result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VCANCCGBT_E_NO_ERROR;

  VCANCCGBT_DUMMY_STATEMENT(result); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  /* ----- Development Error Checks ------------------------------------- */
  /* #-- Check that the vCanCcGbt is initialized. */
  if (vCanCcGbt_ModuleInitialized != VCANCCGBT_INIT)
  {
    errorId = VCANCCGBT_E_UNINIT;
  }
#if (VCANCCGBT_DEV_ERROR_DETECT == STD_ON)
  /* #-- Check if the ID of the transmitted PDU is valid. */
  else if ((txPduId >= vCanCcGbt_GetSizeOfTpTxPdu())
# if (VCANCCGBT_INVALIDHNDOFTPTXPDU == STD_ON)
           || (vCanCcGbt_IsInvalidHndOfTpTxPdu(txPduId))
# endif
    )
  {
    errorId = VCANCCGBT_E_INVALID_PDU_SDU_ID;
  }
#endif
  /* #-- If the initial checks are passed or not required: */
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    vCanCcGbt_InternalResetTpPDU(txPduId);
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VCANCCGBT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VCANCCGBT_E_NO_ERROR)
  {
    vCanCcGbt_ReportDetError(VCANCCGBT_SID_TPTXCONFIRMATION, errorId);
  }
#else
  VCANCCGBT_DUMMY_STATEMENT(errorId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint !e438 */
#endif
} /* vCanCcGbt_TpTxConfirmation() */

/**********************************************************************************************************************
 *  vCanCcGbt_StartOfReception
 *********************************************************************************************************************/
/* PRQA S 3673 7 */ /* MD_MSR_16.7 */
FUNC(BufReq_ReturnType, VCANCCGBT_CODE) vCanCcGbt_StartOfReception(PduIdType pduId,
                                                                   P2VAR(PduInfoType, AUTOMATIC,
                                                                         VCANCCGBT_APPL_DATA) info,
                                                                   PduLengthType tpSduLength, P2VAR(PduLengthType,
                                                                                                    AUTOMATIC,
                                                                                                    VCANCCGBT_APPL_DATA)
                                                                   bufferSizePtr)
{
  VCANCCGBT_DUMMY_STATEMENT(pduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  VCANCCGBT_DUMMY_STATEMENT(info); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  VCANCCGBT_DUMMY_STATEMENT(tpSduLength); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  VCANCCGBT_DUMMY_STATEMENT(bufferSizePtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  return BUFREQ_E_NOT_OK;
} /* vCanCcGbt_StartOfReception() */

/**********************************************************************************************************************
 *  vCanCcGbt_CopyRxData
 *********************************************************************************************************************/
/* PRQA S 3673 5 */ /* MD_MSR_16.7 */
FUNC(BufReq_ReturnType, VCANCCGBT_CODE) vCanCcGbt_CopyRxData(PduIdType pduId,
                                                             P2VAR(PduInfoType, AUTOMATIC, VCANCCGBT_APPL_DATA) info,
                                                             P2VAR(PduLengthType, AUTOMATIC,
                                                                   VCANCCGBT_APPL_DATA) availableDataPtr)
{
  VCANCCGBT_DUMMY_STATEMENT(pduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  VCANCCGBT_DUMMY_STATEMENT(info); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  VCANCCGBT_DUMMY_STATEMENT(availableDataPtr); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */

  return BUFREQ_E_NOT_OK;
} /* vCanCcGbt_CopyRxData() */

/**********************************************************************************************************************
 *  vCanCcGbt_TpRxIndication
 *********************************************************************************************************************/
FUNC(void, VCANCCGBT_CODE) vCanCcGbt_TpRxIndication(PduIdType pduId, Std_ReturnType result)
{
  VCANCCGBT_DUMMY_STATEMENT(pduId); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
  VCANCCGBT_DUMMY_STATEMENT(result); /* PRQA S 3112, 3199 */ /* MD_MSR_14.2 */ /* lint -e{438} */
} /* vCanCcGbt_TpRxIndication() */

/*============================================== VCANCCGBT_STOP_SEC_CODE ============================================*/
#define VCANCCGBT_STOP_SEC_CODE
/*lint -save -esym(961, 19.1) */
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*lint -restore */
