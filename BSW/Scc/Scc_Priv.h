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
/**        \file  Scc_Priv.h
 *        \brief  Smart Charge Communication Header File
 *
 *      \details  Implements Vehicle 2 Grid communication according to the specifications ISO/IEC 15118-2,
 *                DIN SPEC 70121 and customer specific schemas.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the Scc module. >> Scc.h
 *********************************************************************************************************************/ 
#if !defined (SCC_PRIV_H)
#define SCC_PRIV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Scc_Types.h"
#include "Scc_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* V2G constants */
#define SCC_IPV6_ADDRESS_LEN   16u
#define SCC_V2GTP_HDR_LEN       8u
#define SCC_EXI_HDR_LEN         1u
#define SCC_THREE_PHASES        3u
#define SCC_ONE_PHASE           1u

/* Crypto constants */
#define SCC_CRYPTO_BUF_LEN 80u

/* Integer constants */
#define SCC_SINT8_MAX_POS_NUM  0x7Fu       /*            127 */
#define SCC_SINT8_MAX_NEG_NUM  0x80u       /* (-)        128 */
#define SCC_SINT16_MAX_POS_NUM 0x7FFFu     /*          32767 */
#define SCC_SINT16_MAX_NEG_NUM 0x8000u     /* (-)      32768 */
#define SCC_SINT32_MAX_POS_NUM 0x7FFFFFFFu /*     2147483647 */
#define SCC_SINT32_MAX_NEG_NUM 0x80000000u /* (-) 2147483648 */

#define SCC_UINT8_MAX_NUM  0xFFu
#define SCC_UINT16_MAX_NUM 0xFFFFu
#define SCC_UINT32_MAX_NUM 0xFFFFFFFFu

#define SCC_SECONDS_PER_DAY 0x15180u /* 86,400 seconds per day */

#define SCC_SIZE_OF_SERVICE_ID              0x02u
#define SCC_SIZE_OF_TIMESTAMP               0x04u
#define SCC_MAX_SIZE_OF_EMAID               0x0Fu
#define SCC_MAX_SIZE_OF_SERVICE_NAME_PREFIX 0x20u

#if ( defined SCC_SESSION_ID_NVM_BLOCK_LEN )
#else
# define SCC_SESSION_ID_NVM_BLOCK_LEN 9
extern SCC_VARNOINIT(uint8) Scc_SessionIDNvm[SCC_SESSION_ID_NVM_BLOCK_LEN];
#endif

#if ( defined SCC_OEM_VECTOR )
#define SCC_IF_CFG_H "Scc_Interface_Cfg.h"
#define SCC_CP_CFG_H "Scc_ConfigParams_Cfg.h"
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 MACROS_FUNCTION_LIKE */ /* MD_MSR_19.7 */
/* PRQA S 3458 MACROS_BRACES */ /* MD_MSR_19.4 */

#if ( defined SCC_DUMMY_STATEMENT )
#else
# define SCC_DUMMY_STATEMENT(Var)
#endif /* SCC_DUMMY_STATEMENT */

/* Diagnostic Event Manager */
# if ( defined SCC_PROD_ERROR_DETECT ) && ( SCC_PROD_ERROR_DETECT == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
#  define Scc_DemReportErrorStatusFailed(ErrId) Dem_ReportErrorStatus((ErrId), DEM_EVENT_STATUS_FAILED)
#  define Scc_DemReportErrorStatusPassed(ErrId) Dem_ReportErrorStatus((ErrId), DEM_EVENT_STATUS_PASSED)
# else /* SCC_PROD_ERROR_DETECT */
#  define Scc_DemReportErrorStatusFailed(ErrId)
#  define Scc_DemReportErrorStatusPassed(ErrId)
# endif /* SCC_PROD_ERROR_DETECT */

/* Development Error Tracer */
#if ( defined SCC_DEV_ERROR_DETECT ) && ( SCC_DEV_ERROR_DETECT == STD_ON ) /* PRQA S 3332 */ /* MD_Scc_3332 */
#  define Scc_CheckDetErrorReturnVoid( CONDITION, API_ID, ERROR_CODE ) { if( CONDITION ) { \
    (void)Det_ReportError( SCC_MODULE_ID, SCC_INSTANCE_ID, (API_ID), (ERROR_CODE)); return; } }
#  define Scc_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL ) { if( CONDITION ) { \
    (void)Det_ReportError( SCC_MODULE_ID, SCC_INSTANCE_ID, (API_ID), (ERROR_CODE)); return(RET_VAL); } }
#  define Scc_CheckDetErrorContinue( CONDITION, API_ID, ERROR_CODE ) { if( CONDITION ) { \
    (void)Det_ReportError( SCC_MODULE_ID, SCC_INSTANCE_ID, (API_ID), (ERROR_CODE)); } }
#  define Scc_CallDetReportError( API_ID, ERROR_CODE ) { \
    (void)Det_ReportError( SCC_MODULE_ID, SCC_INSTANCE_ID, (API_ID), (ERROR_CODE)); }
#  define Scc_CallDetReportErrorReturnValue( API_ID, ERROR_CODE, RET_VAL ) { \
    (void)Det_ReportError( SCC_MODULE_ID, SCC_INSTANCE_ID, (API_ID), (ERROR_CODE)); return(RET_VAL); }
#else
#  define Scc_CheckDetErrorReturnVoid( CONDITION, API_ID, ERROR_CODE )
#  define Scc_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL )
#  define Scc_CheckDetErrorContinue( CONDITION, API_ID, ERROR_CODE )
#  define Scc_CallDetReportError( API_ID, ERROR_CODE )
#  define Scc_CallDetReportErrorReturnValue( API_ID, ERROR_CODE, RET_VAL )
#endif

#if ( defined SCC_CONFIG_VARIANT ) && ( SCC_CONFIG_VARIANT == 1u )

/* --- NvM Block Identifier & Length --- */
/* Block Identifier */
/* Block Length */

#else /* SCC_CONFIG_VARIANT */
#endif /* SCC_CONFIG_VARIANT */

#define Tl_Connect          TcpIp_TcpConnect
#define Tl_TransmitTo       TcpIp_TransmitTo
#define Tl_ProvideTxBuffer  TcpIp_ProvideTxBuffer
#define Tl_Close(Socket)    TcpIp_Close((Socket), FALSE)
#define Tl_Received         TcpIp_TcpReceived
#define Tl_GetSocket        TcpIp_SccGetSocket
#define Tl_ChangeParameter  TcpIp_ChangeParameter
#define Tl_Bind             TcpIp_Bind
#define Tls_RootCertChanged TcpIp_Tls_RootCertWasModified

/* PRQA L:MACROS_BRACES */
/* PRQA L:MACROS_FUNCTION_LIKE */
/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/* other variables */
#define SCC_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern SCC_VARNOINIT(Scc_MsgTrigType)     Scc_MsgTrig;
extern SCC_VARNOINIT(Scc_MsgStatusType)   Scc_MsgStatus;
extern SCC_VARNOINIT(Scc_StackErrorType)  Scc_StackError;
extern SCC_VARNOINIT(Scc_SDPSecurityType) Scc_SelectedConnectionType;
extern SCC_VARNOINIT(Scc_SAPSchemaIDType) Scc_SAPSchemaID;

extern SCC_VARNOINIT(Scc_PbufType)        Scc_ExiStreamRxPBuf[SCC_EXI_RX_STREAM_PBUF_ELEMENTS];
extern SCC_P2VARNOINIT(Scc_PbufType)      Scc_ExiStreamTxPBufPtr;
extern SCC_VARNOINIT(Scc_SockAddrIn6Type) Scc_ServerSockAddr;

extern SCC_VARNOINIT(TcpIp_SocketIdType)  Scc_SDPSocket;
extern SCC_VARNOINIT(TcpIp_SocketIdType)  Scc_V2GSocket;

extern Scc_ReturnType (*Scc_ExiTx_Xyz_EncodeMessageFctPtr)(void);
extern Std_ReturnType (*Scc_ExiRx_Xyz_DecodeMessageFctPtr)(void);

#define SCC_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* uint8 variables - no init */
#define SCC_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern SCC_VARNOINIT(boolean) Scc_TxStreamingActive;

#if ( defined SCC_ENABLE_PRIVATE_KEY_ENCRYPTION ) && ( SCC_ENABLE_PRIVATE_KEY_ENCRYPTION == STD_ON )
extern SCC_VARNOINIT(uint8) Scc_XcryptPrivKeyBuf[SCC_PRIV_KEY_LEN];
#endif /* SCC_ENABLE_PRIVATE_KEY_ENCRYPTION */

#define SCC_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* uint16 variables */
#define SCC_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern SCC_VARNOINIT(uint32) Scc_TxDataSent;
extern SCC_VARNOINIT(uint16) Scc_TimeoutCnt;

#define SCC_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* uint8 variables - zero init */
#define SCC_START_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern SCC_VARZEROINIT(Scc_StateType) Scc_State;

#define SCC_STOP_SEC_VAR_ZERO_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define SCC_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

SCC_FUNCCODE(void) Scc_Priv_Init(void);
SCC_FUNCCODE(void) Scc_ReportError(Scc_StackErrorType StackError);
SCC_FUNCCODE(void) Scc_ReportErrorAndStatus(Scc_StackErrorType StackError, Scc_MsgStatusType MsgStatus);
SCC_FUNCCODE(void) Scc_ReportSuccessAndStatus(Scc_MsgStatusType MsgStatus);
SCC_FUNCCODE(sint32) Scc_AdjustValueToMultiplier(sint16 Value, sint8 Multiplier, boolean LimitSInt16);
SCC_FUNCCODE(uint32) Scc_AdjustAmountToMultiplier(uint32 amount, sint8 Multiplier, boolean RoundUp);
SCC_FUNCCODE(Std_ReturnType) Scc_CheckForCpoInDc(SCC_P2CONSTAPPLDATA(uint8) CertPtr, uint16 CertLen);

#if ( defined SCC_ENABLE_INTERNAL_CHARGING_PROFILE_BUFFER )
#else
# define SCC_ENABLE_INTERNAL_CHARGING_PROFILE_BUFFER STD_OFF
#endif

#define SCC_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* SCC_PRIV_H */
/**********************************************************************************************************************
 *  END OF FILE: Scc_Priv.h
 *********************************************************************************************************************/
