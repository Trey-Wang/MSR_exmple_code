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
 *         File:  Tls_Priv.h
 *      Project:  OnBoardCharger
 *       Module:  Transport Layer Security
 *    Generator:  SysSercice_Tls.dll
 *
 *  Description:  private header
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the Tls module. >> Tls.h
 *********************************************************************************************************************/

#if !defined (TLS_PRIV_H)
# define TLS_PRIV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Tls_Types.h"

#if (TLS_PROD_ERROR_DETECT == STD_ON)
# include "Dem.h"
#endif
#if (TLS_DEV_ERROR_DETECT == STD_ON)
# include "Det.h"
#endif

#include "Tls_Lcfg.h"
#include "SchM_Tls.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define TLS_TCPIP_LISTEN_BACKLOG       1u
#define TLS_CONFIG_VARIANT_PRECOMPILE  1u /* value for pre-compile time config variant */

#define TLS_CFG_MAX_NUM_NVM_RETRY_CNT               5
    /* min=2 */ /* number of retries accessign the NVM, including the first try */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
#define TLS_VENTER_CRITICAL_SECTION() SchM_Enter_Tls_TLS_EXCLUSIVE_AREA_0()
#define TLS_VLEAVE_CRITICAL_SECTION()   SchM_Exit_Tls_TLS_EXCLUSIVE_AREA_0()

/* Diagnostic Event Manager */
#if ( TLS_PROD_ERROR_DETECT == STD_ON )
# define TLS_VDEM_REPORT_ERROR_STATUS(ErrId)   (Dem_ReportErrorStatus((ErrId), DEM_EVENT_STATUS_FAILED))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
# define TLS_VDEM_REPORT_ERROR_STATUS(ErrId)
#endif
  /* TLS_PROD_ERROR_DETECT */

/* Development Error Tracer */
#if ( TLS_DEV_ERROR_DETECT == STD_ON )
# define TLS_CheckDetErrorReturnVoid(  CONDITION, API_ID, ERROR_CODE )       { if(!(CONDITION)) {\
  (void)Det_ReportError( TLS_MODULE_ID, TLS_INSTANCE_ID, (API_ID), (ERROR_CODE)); return; } }  /* PRQA S 3458 */ /* MD_MSR_19.4 */
# define TLS_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL ) \
  { if(!(CONDITION)) {\
  (void)Det_ReportError( TLS_MODULE_ID, TLS_INSTANCE_ID, (API_ID), (ERROR_CODE)); return (RET_VAL); } }  /* PRQA S 3458 */ /* MD_MSR_19.4 */
# define TLS_CheckDetErrorContinue(    CONDITION, API_ID, ERROR_CODE )       { if(!(CONDITION)) {\
  (void)Det_ReportError( TLS_MODULE_ID, TLS_INSTANCE_ID, (API_ID), (ERROR_CODE)); } }  /* PRQA S 3458 */ /* MD_MSR_19.4 */
# define TLS_CallDetReportError( API_ID, ERROR_CODE )  { \
  (void)Det_ReportError( TLS_MODULE_ID, TLS_INSTANCE_ID, (API_ID), (ERROR_CODE)); }  /* PRQA S 3458 */ /* MD_MSR_19.4 */
# define TLS_ASSERT(CONDITION) { if(!(CONDITION)) { \
    TLS_CallDetReportError(TLS_API_ID_V_INTERNAL_FUNCTION, TLS_E_INV_PARAM); } }  /* PRQA S 3458 */ /* MD_MSR_19.4 */
#else
# define TLS_CheckDetErrorReturnVoid(  CONDITION, API_ID, ERROR_CODE )
# define TLS_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL )
# define TLS_CheckDetErrorContinue(    CONDITION, API_ID, ERROR_CODE )
# define TLS_CallDetReportError(                  API_ID, ERROR_CODE )
# define TLS_ASSERT(CONDITION)
#endif  /* TLS_DEV_ERROR_DETECT */

/* CHECK CONDITIONS: */
#define TLS_COND_INIT                     ( TLS_STATE_UNINIT != Tls_State )         /* TLS_E_NOT_INITIALIZED */
#define TLS_COND_VALID_SOCKID(SocketId)   ( (TCPIP_INV_SOCK_IDX > (SocketId)) && ((SocketId) >= TCPIP_MAX_NUM_UDP_SOCKETS) )      /* TLS_E_INV_SOCK_ID */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_COND_VALID_CONNIDX(ConnIdx)   ( Tls_VCfgGetNumConnections() > (ConnIdx) )   /* TLS_E_INV_CONN_IDX */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_COND_VALID_PTR(Pointer)       ( NULL_PTR != (Pointer) )                 /* TLS_E_INV_POINTER */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_COND_MIN_SIZE(Value, Ref)     ( (Value) >= (Ref) )                      /* TLS_E_INV_PARAM */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define TLS_COND_UNINIT                            ( TLS_STATE_UNINIT == Tls_State )             /* TLS_E_NOT_INITIALIZED */
#define TLS_COND_INVALID_SOCKID(SocketId)          ( ((SocketId) >= TCPIP_MAX_NUM_SOCKETS) || ((SocketId) < TCPIP_MAX_NUM_UDP_SOCKETS) )       /* TLS_E_INV_SOCK_ID */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_COND_INVALID_CONNIDX(ConnIdx)          ( (ConnIdx) >= Tls_VCfgGetNumConnections() )  /* TLS_E_INV_CONN_IDX */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_COND_INVALID_PTR(Pointer)              ( (Pointer) == NULL_PTR )                     /* TLS_E_INV_POINTER */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TLS_COND_INVALID_MIN_SIZE(Value, MinSize)  ( (MinSize) > (Value) )                       /* TLS_E_INV_PARAM */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* implementation limit: max message length is 64kB */
#define TLS_COND_LEN_MAX_UINT16(Len)      ( (uint32)(Len) <= (uint32)0xFFFF )       /* TLS_E_INV_PARAM */  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* reporting of runtime errors */
#define TLS_REPORT_ERROR_STATUS(ApiId, EventId, EventStatus)  /* Currently no event forwarding is implemented. DEM (Dem_ReportErrorStatus) could be used */
/* Use the status define TLS_EVENT_STATUS_XX and the event ids TLS_EVENT_XX */

#if (TLS_USE_DUMMY_STATEMENT == STD_ON)
# define TLS_UNUSED_PARAMETER(Parameter) (void)(Parameter);
#else
# define TLS_UNUSED_PARAMETER(Parameter)
#endif

/* Configuration access (depending on configuration variant)*/

#define Tls_VCfgGetNumUsers()                  (TLS_CFG_NUM_USERS)  /* not yet used */
#define Tls_VCfgGetNumConnections()            (TLS_CFG_NUM_CONNECTIONS)
#define Tls_VCfgGetMainFctPeriodMsec()         (TLS_CFG_MAIN_FKT_PERIOD_MSEC)  /* not yet used */
#define Tls_VCfgGetNumSuppCipherSuites()       (TLS_CFG_NUM_SUPP_CIPHER_SUITES)
#define Tls_VCfgGetMaxServerCertRsaExpSizeByte()  (TLS_CFG_MAX_SERVER_CERT_RSA_EXP_SIZE_BYTE)
/* #define Tls_VCfgGetMaxServerCertModSizeByte()  (TLS_CFG_MAX_SERVER_CERT_MOD_SIZE_BYTE) */
#define Tls_VCfgGetMaxServerCertKeySizeByte()  (TLS_CFG_MAX_SERVER_CERT_KEY_SIZE_BYTE)

#define Tls_VCfgGetNumServerCerts()            (TLS_CFG_NUM_SERVER_CERTS)
#define Tls_VCfgGetMaxNumNvmRetryCnt()         (TLS_CFG_MAX_NUM_NVM_RETRY_CNT)
#define Tls_VCfgGetNumTcpIpSockets()           (TLS_CFG_NUM_TCPIP_SOCKETS)

#define Tls_VCfgGetRxPlainTextStarts()         (Tls_CfgRxPlainTextStarts)
#define Tls_VCfgGetRxPlainTextSizes()          (Tls_CfgRxPlainTextSizes)
#define Tls_VCfgGetTxCipherTextStarts()        (Tls_CfgTxCipherTextStarts)
#define Tls_VCfgGetTxPlainTextStarts()         (Tls_CfgTxPlainTextStarts)
#define Tls_VCfgGetTxCipherTextSizes()         (Tls_CfgTxCipherTextSizes)
#define Tls_VCfgGetTxPlainTextSizes()          (Tls_CfgTxPlainTextSizes)  /* used for Tls_ChangeParam() */
#define Tls_VCfgGetSuppCipherSuites(EleIndex)  (Tls_CfgSuppCipherSuites[EleIndex])  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define Tls_VCfgGetTlsConn2SockIdMap()         (Tls_CfgTlsConn2SockHndMap)
#define Tls_VCfgGetSockId2TlsConn(SocketId)    (Tls_SockHnd2TlsConnMap[SocketId])  /* RAM array filled in _Init() */  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define Tls_VCfgGetTlsUser()                   (Tls_CfgTlsUser)
#define Tls_VCfgGetServerCertBlockIds(CertId)  (Tls_CfgServerCertBlockIds[CertId])  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define Tls_VCfgGetDynCryptoWsStartOffs()      (Tls_CfgDynCryptoWsStartOffs)

#define Tls_TcpClose(SocketId) TcpIp_Close(SocketId, FALSE)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define Tls_TcpReceived       TcpIp_TcpReceived

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define TLS_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h"
#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h"

#endif
  /* TLS_PRIV_H */
/**********************************************************************************************************************
 *  END OF FILE: Tls_Priv.h
 *********************************************************************************************************************/
