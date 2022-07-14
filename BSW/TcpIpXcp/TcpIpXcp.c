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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  TcpIpXcp.c
 *    Component:  TcpIpXcp Implementation
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  Implementation of XCP over TcpIp based on Vector AUTOSAR Stack
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Andreas Herkommer             Hr            Vector Informatik GmbH
 *  Oliver Reineke                Ore           Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2008-10-09  Hr                    First Version
 *  02.00.00  2013-06-04  Hr      ESCAN00059277 Adapt component to SoAd
 *                        Hr      ESCAN00069959 Support ASR4 environment
 *  02.00.01  2013-11-15  Hr      ESCAN00071975 Missing XcpOnEth header in response packet
 *  02.00.02  2014-02-25  Ore     ESCAN00073922 Compiler error: Symbol "TCPIPXCP_EXCLUSIVE_AREA_0" unknown
 *                        Hr      ESCAN00074272 SetPduMode feature leads to continuous transmission
 *  02.00.03  2014-05-09  Hr      ESCAN00075589 Improve robustness of use case "if transmission of XCP on Ethernet frame fails"
 *  02.00.04  2014-07-07  Hr      ESCAN00076800 Rx buffer not aligned to 32Bit
 *                        Hr      ESCAN00076953 Xcp connection terminated in case of Alive Supervision Timeout
 *  02.01.00  2015-02-16  Hr      ESCAN00079998 Missing memory abstraction in header
 *            2015-04-23  Hr      ESCAN00080791 Support Resume Mode
 *  02.02.00  2015-05-05  Hr      ESCAN00082845 Support TxConfirmation timeout timer
 *  02.02.01  2015-11-12  Hr      ESCAN00086397 FEAT-1357: SafeXCP
 *                        Hr      ESCAN00085638 Compiler warning: Modifiable lvalue
 *  03.00.00  2016-08-31  Hr      ESCAN00091918 FEAT-1980: Add Multi Client / Multi Connection support
 *                        Hr      ESCAN00091954 Safe deactivation does not consider TcpIpXcp_Init
 *                        Hr      ESCAN00091955 Safe deactivation shall be performed before DET checks or anything
 *  03.00.01  2017-04-19  Hr      ESCAN00094223 DET error due to incorrect channel id conversion
 *********************************************************************************************************************/

#define TCPIPXCP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "TcpIpXcp.h"
#include "SoAd.h"

#if ( TCPIPXCP_DEV_ERROR_DETECT == STD_ON )
  #include "Det.h"              /* Include Diagnostic Event Manager */
#endif

#if defined (TCPIPXCP_CONFIGURATOR) && (TCPIPXCP_CONFIGURATOR == 5)
  #include "SchM_Xcp.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* vendor specific version information is BCD coded */
#if (  (TCPIPXCP_SW_MAJOR_VERSION != (0x03)) \
    || (TCPIPXCP_SW_MINOR_VERSION != (0x00)) \
    || (TCPIPXCP_SW_PATCH_VERSION != (0x01)) )
  #error "Vendor specific version numbers of TcpIpXcp.c and TcpIpXcp.h are inconsistent"
#endif

/* AUTOSAR version information check has to match definition in header file */
#if (  (TCPIPXCP_AR_MAJOR_VERSION != (0x04)) \
    || (TCPIPXCP_AR_MINOR_VERSION != (0x00)) \
    || (TCPIPXCP_AR_PATCH_VERSION != (0x03)) )
  #error "AUTOSAR Specification Version numbers of TcpIpXcp.c and TcpIpXcp.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
#define TCPIPXCP_MAX(a, b)   (((a) > (b)) ? (a) : (b)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#if (TCPIPXCP_DEV_ERROR_DETECT == STD_ON)
/* If DET is enabled, a MISRA-C:2004 analysis will yield deviations to the rules:
*  - 14.3 Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment provided that the first character following the null statement is a white-space character.
*  - 14.7 A function shall have a single point of exit at the end of the function
*  - 19.4 C macros shall only expand to a braced initializer, a constant, a parenthesized expression, a type qualifier, a storage class specifier, or a do-while-zero construct
*  - 19.7 A function should be used in preference to a function-like macro.
*  These deviations are caused by design of the runtime/ressource optimized DET handling and are globally justified.
*/
#define TcpIpXcp_CheckDetErrorReturnVoid( CONDITION, API_ID, ERROR_CODE ) { if(!(CONDITION)) { \
  (void)Det_ReportError( TCPIPXCP_MODULE_ID, 0, (API_ID), (ERROR_CODE)); return; } } /* PRQA S 3458 */ /* MD_MSR_19.4 */
#define TcpIpXcp_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL ) { if(!(CONDITION)) { \
  (void)Det_ReportError( TCPIPXCP_MODULE_ID, 0, (API_ID), (ERROR_CODE)); return (RET_VAL); } } /* PRQA S 3458 */ /* MD_MSR_19.4 */
#define TcpIpXcp_CheckDetErrorContinue( CONDITION, API_ID, ERROR_CODE ) { if(!(CONDITION)) { \
  (void)Det_ReportError( TCPIPXCP_MODULE_ID, 0, (API_ID), (ERROR_CODE)); } } /* PRQA S 3458 */ /* MD_MSR_19.4 */
#define TcpIpXcp_CallDetReportError( API_ID, ERROR_CODE ) (void)Det_ReportError( TCPIPXCP_MODULE_ID, 0, (API_ID), (ERROR_CODE)) /* PRQA S 3453 1 */ /* MD_MSR_19.7 */
#else
#define TcpIpXcp_CheckDetErrorReturnVoid( CONDITION, API_ID, ERROR_CODE )
#define TcpIpXcp_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL ) 
#define TcpIpXcp_CheckDetErrorContinue( CONDITION, API_ID, ERROR_CODE ) 
#define TcpIpXcp_CallDetReportError( API_ID, ERROR_CODE ) 
#endif

/* Define AUTOSAR types if not defined yet */
#if defined ( STATIC )
#else
  #define STATIC static
#endif

#if !defined ( TCPIPXCP_COPY )
  #if defined ( XCP_ENABLE_ALIGNED_DTO )
    #define TCPIPXCP_COPY(dest, src, length) TcpIpXcp_FastCopy( (DAQBYTEPTR)(dest), (ROMDAQBYTEPTR)(src), (uint16)(length) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */
  #else
  #define TCPIPXCP_COPY(dest, src, length) Xcp_MemCpy( (DAQBYTEPTR)(dest), (ROMDAQBYTEPTR)(src), (uint8)(length) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */
  #endif
#endif

/* Configuration access (depending on configuration variant)*/
#if ( TCPIPXCP_CONFIG_VARIANT == 3 )
#define TcpIpXcp_GetSoAdTxPduId(idx) TcpIpXcp_Configuration->TcpIpXcp_XcpPduPtr[idx].SoAdTxPduId /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TcpIpXcp_GetTxConfPduId(idx) TcpIpXcp_Configuration->TcpIpXcp_XcpPduPtr[idx].TxConfPduId /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TcpIpXcp_GetRxPduId(idx)     TcpIpXcp_Configuration->TcpIpXcp_XcpPduPtr[idx].RxPduId /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TcpIpXcp_GetSoConId(idx)     TcpIpXcp_Configuration->TcpIpXcp_XcpPduPtr[idx].SoConId /* PRQA S 3453 */ /* MD_MSR_19.7 */

#elif ( TCPIPXCP_CONFIG_VARIANT == 2 )
#define TcpIpXcp_GetSoAdTxPduId(idx) TcpIpXcp_PduIdField[idx].SoAdTxPduId /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TcpIpXcp_GetTxConfPduId(idx) TcpIpXcp_PduIdField[idx].TxConfPduId /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TcpIpXcp_GetRxPduId(idx)     TcpIpXcp_PduIdField[idx].RxPduId /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TcpIpXcp_GetSoConId(idx)     TcpIpXcp_PduIdField[idx].SoConId /* PRQA S 3453 */ /* MD_MSR_19.7 */

#elif ( TCPIPXCP_CONFIG_VARIANT == 1 )
#define TcpIpXcp_GetSoAdTxPduId(idx) TcpIpXcp_PduIdField[idx].SoAdTxPduId /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TcpIpXcp_GetTxConfPduId(idx) TcpIpXcp_PduIdField[idx].TxConfPduId /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TcpIpXcp_GetRxPduId(idx)     TcpIpXcp_PduIdField[idx].RxPduId /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define TcpIpXcp_GetSoConId(idx)     TcpIpXcp_PduIdField[idx].SoConId /* PRQA S 3453 */ /* MD_MSR_19.7 */

#else
  #error "TcpIpXcp.c: invalid or missing TCPIPXCP_CONFIG_VARIANT, (value 1, 2 or 3 expected in config file)"
#endif
  /* TCPIPXCP_CONFIG_VARIANT */

/* States of TcpIpXcp_SendWithoutQueueState */
#define TCPIPXCP_MSG_IDLE           0
#define TCPIPXCP_MSG_PENDING        1

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
typedef union { /* PRQA S 0750 */ /* MD_MSR_18.4 */
  uint8  fc[TCPIPXCP_MAX(kTcpIpXcpMaxCTO, kTcpIpXcpMaxDTO) + TCPIPXCP_HEADER_SIZE];
  uint32 dw[(TCPIPXCP_MAX(kTcpIpXcpMaxCTO, kTcpIpXcpMaxDTO) + TCPIPXCP_HEADER_SIZE)/4u]; /* This is to force proper alignment */
} TcpIpXcpAlignedFrameCacheType; /* PRQA S 3448 */ /* MD_Xcp_3448 */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/
#define TCPIPXCP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(TcpIpXcpAlignedFrameCacheType, TCPIPXCP_VAR_NOINIT) TcpIpXcp_TxFrameCache[TCPIPXCP_NUMBER_OF_CHANNELS]; /* PRQA S 3408, 0759 */ /* MD_Xcp_3408 */ /* MD_MSR_18.4 */
#if (TCPIPXCP_ALIGN_RXBUFFER == STD_ON)
  VAR(TcpIpXcpAlignedFrameCacheType, TCPIPXCP_VAR_NOINIT) TcpIpXcp_RxFrameCache; /* PRQA S 3408, 0759 */ /* MD_Xcp_3408 */ /* MD_MSR_18.4 */
#endif

#if ( TCPIPXCP_CONFIG_VARIANT == 3 )
P2CONST(TcpIpXcp_ConfigType, TCPIPXCP_VAR_NOINIT, TCPIPXCP_PBCFG) TcpIpXcp_Configuration; /* PRQA S 3408 */ /* MD_Xcp_3408 */
#endif

VAR(PduInfoType, TCPIPXCP_VAR_NOINIT) TcpIpXcp_QueuedPduInfo[TCPIPXCP_NUMBER_OF_CHANNELS]; /* PRQA S 3408 */ /* MD_Xcp_3408 */
VAR(TcpIpXcp_TimeoutTimerType, TCPIPXCP_VAR_NOINIT) TcpIpXcp_ConfTimeoutTimer[TCPIPXCP_NUMBER_OF_CHANNELS]; /* PRQA S 3408 */ /* MD_Xcp_3408 */

#if ( XCP_NUMBER_OF_CHANNELS <= 1 )
VAR(PduIdType, TCPIPXCP_VAR_NOINIT) TcpIpXcp_RxPduId; /* PRQA S 3408 */ /* MD_Xcp_3408 */
#endif

#if ( TCPIPXCP_ENABLE_PDUMODE == STD_ON )
VAR(TcpIpXcp_PduSetModeType, TCPIPXCP_VAR_NOINIT) TcpIpXcp_PduState[TCPIPXCP_NUMBER_OF_CHANNELS]; /* PRQA S 3408 */ /* MD_Xcp_3408 */
#endif

#define TCPIPXCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIPXCP_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(uint16, TCPIPXCP_VAR_NOINIT) TcpIpXcp_SequenceCounter[TCPIPXCP_NUMBER_OF_CHANNELS]; /* PRQA S 3408 */ /* MD_Xcp_3408 */

#define TCPIPXCP_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIPXCP_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(uint8, TCPIPXCP_VAR_NOINIT) TcpIpXcp_SendWithoutQueueState[TCPIPXCP_NUMBER_OF_CHANNELS]; /* PRQA S 3408 */ /* MD_Xcp_3408 */

#define TCPIPXCP_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TCPIPXCP_START_SEC_VAR_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( TCPIPXCP_DEV_ERROR_DETECT == STD_ON )
VAR(uint8, TCPIPXCP_VAR_INIT) TcpIpXcp_InitializationState = TCPIPXCP_UNINIT; /* PRQA S 3408 */ /* MD_Xcp_3408 */
#endif
#if ( TCPIPXCP_ENABLE_CONTROL == STD_ON )
VAR(uint8, TCPIPXCP_VAR_INIT) TcpIpXcp_ControlDeniedCount = 0u; /* PRQA S 3408 */ /* MD_Xcp_3408 */
#endif

#define TCPIPXCP_STOP_SEC_VAR_INIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( TCPIPXCP_ENABLE_CONTROL == STD_ON )
# define TCPIPXCP_START_SEC_VAR_INIT_UNSPECIFIED_SAFE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* TL Enable/Disable state */
VAR(uint8, TCPIPXCP_VAR_INIT) TcpIpXcp_ControlState = kXcponTcpIp_Control_Enable;

# define TCPIPXCP_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/
#define TCPIPXCP_START_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Requirement Specification Template version */
CONST(uint8, TCPIPXCP_CONST) TcpIpXcp_MainVersion    = TCPIPXCP_SW_MAJOR_VERSION;
CONST(uint8, TCPIPXCP_CONST) TcpIpXcp_SubVersion     = TCPIPXCP_SW_MINOR_VERSION;
CONST(uint8, TCPIPXCP_CONST) TcpIpXcp_ReleaseVersion = TCPIPXCP_SW_PATCH_VERSION;

#define TCPIPXCP_STOP_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define TCPIPXCP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if defined ( XCP_ENABLE_ALIGNED_DTO )
STATIC FUNC(void, FRXCP_CODE) TcpIpXcp_FastCopy( P2VAR(uint8, AUTOMATIC, FRXCP_VAR_NOINIT) dest, P2CONST(uint8, AUTOMATIC, FRXCP_VAR_NOINIT) src, uint16 len );
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

#if defined ( XCP_ENABLE_ALIGNED_DTO )
/**************************************************************************************************
   Function name    : TcpIpXcp_FastCopy
   ------------------------------------------------------------------------------------------------
   Description      : Fast 32 bit copy routine
   ------------------------------------------------------------------------------------------------
   Called by        : TcpIpXcp
   ------------------------------------------------------------------------------------------------
   Parameter        : dest, src, len
   ------------------------------------------------------------------------------------------------
   Returncode       : none
   ------------------------------------------------------------------------------------------------
   Misc             : -
**************************************************************************************************/
STATIC FUNC(void, FRXCP_CODE) TcpIpXcp_FastCopy( P2VAR(uint8, AUTOMATIC, FRXCP_VAR_NOINIT) dest, P2CONST(uint8, AUTOMATIC, FRXCP_VAR_NOINIT) src, uint16 len )
{
  while( len >= 4 )
  {
    *(P2VAR(uint32, AUTOMATIC, TCPIPXCP_VAR_NOINIT))dest = *(P2CONST(uint32, AUTOMATIC, TCPIPXCP_VAR_NOINIT))src; /* PRQA S 0310 */ /* MD_Xcp_0310 */
    dest = &dest[4];
    src = &src[4];
    len -= 4;
  }
  while( len > 0 )
  {
    *(P2VAR(uint8, AUTOMATIC, TCPIPXCP_VAR_NOINIT))dest = *(P2CONST(uint8, AUTOMATIC, TCPIPXCP_VAR_NOINIT))src;
    dest = &dest[1];
    src = &src[1];
    len -= 1;
  }
}
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Xcp_SoAdRxIndication
 **********************************************************************************************************************/
/*! \brief      This method is called by the SoAd interface to receive frames
 *  \param[in]  SockHnd
 *  \param[in]  DataPtr
 *  \param[in]  LenByte
 *  \param[in]  SrcNetAddr
 *  \param[in]  SrcPort
 *  \pre        Input parameter must not be NULL.
 *  \context    Function could be called from interrupt level or from task level
 *  \note       Requirement Specification example function.
 **********************************************************************************************************************/
FUNC(void, TCPIPXCP_CODE) Xcp_SoAdRxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, TCPIPXCP_APPL_DATA) PduInfoPtr)
{
  uint16 xcpFrameLen;
#if ( XCP_NUMBER_OF_CHANNELS > 1 )
  PduIdType TcpIpXcp_RxPduId;
#endif

  /* Activation control */
#if ( TCPIPXCP_ENABLE_CONTROL == STD_ON )
  if ( TcpIpXcp_ControlState == kXcponTcpIp_Control_Disable )
  {
    TcpIpXcp_ControlDeniedCount++;
    return;
  }
#endif

  TcpIpXcp_CheckDetErrorReturnVoid( TCPIPXCP_UNINIT != TcpIpXcp_InitializationState,
                                    TCPIPXCP_RXINDICATION_SERVICE_ID, TCPIPXCP_E_NOT_INITIALIZED )
  TcpIpXcp_CheckDetErrorReturnVoid( PduInfoPtr != (P2VAR(PduInfoType, AUTOMATIC, TCPIPXCP_APPL_VAR))NULL_PTR,
                                    TCPIPXCP_RXINDICATION_SERVICE_ID, TCPIPXCP_E_NULL_POINTER )
  TcpIpXcp_CheckDetErrorReturnVoid( PduInfoPtr->SduDataPtr != (SduDataPtrType)NULL_PTR,
                                    TCPIPXCP_RXINDICATION_SERVICE_ID, TCPIPXCP_E_NULL_POINTER )

#if ( XCP_NUMBER_OF_CHANNELS > 1 )
  TcpIpXcp_CheckDetErrorReturnVoid( (XCP_TRANSPORT_LAYER_TCPIP + RxPduId) < XCP_NUMBER_OF_CHANNELS,
                                    TCPIPXCP_RXINDICATION_SERVICE_ID, TCPIPXCP_E_INV_SOCK_IDX )
  /* Store the Rx PDU Id to determine the reception channel. */
  TcpIpXcp_RxPduId = RxPduId;
#else
# if ( TCPIPXCP_NUMBER_OF_CHANNELS > 1 )
  if ( TcpIpXcp_RxPduId != RxPduId )
  {
#  if ( TCPIPXCP_MULTI_CONN_PROT == STD_ON )
    /* Ensure that no XCP connection is interrupt via another channel. */
    if ( 0u != (Xcp_GetSessionStatus((uint8)(XCP_TRANSPORT_LAYER_TCPIP + TcpIpXcp_RxPduId)) & (SessionStatusType)SS_CONNECTED) )
    {
      /* Do not transmit an error packet due to XcpConfirmation() will be called on the wrong channel. */
      return;
    }
    else
#  endif
    {
      /* Store the Rx PDU Id to determine the reception channel. */
      TcpIpXcp_RxPduId = RxPduId;
    }
  }
# else
  /* Verifiy RX Pdu Id */
  if(RxPduId == TcpIpXcp_GetRxPduId(0))
  {
    /* Store the Rx PDU Id for the reception. */
    TcpIpXcp_RxPduId = RxPduId;
# endif
#endif

  xcpFrameLen = (PduInfoPtr->SduDataPtr[0] << 0) | ((uint16)PduInfoPtr->SduDataPtr[1] << 8);
  /* Check for valid length */
  if( xcpFrameLen <= (PduInfoPtr->SduLength - (uint16)TCPIPXCP_HEADER_SIZE) )
  {
    /* Is this a connect? */
    if(0xff == PduInfoPtr->SduDataPtr[TCPIPXCP_HEADER_SIZE])
    {
      /* In case of connect we set this as active TL */
      Xcp_SetActiveTl((uint8)(XCP_TRANSPORT_LAYER_TCPIP + TcpIpXcp_RxPduId), kTcpIpXcpMaxCTO, kTcpIpXcpMaxDTO, XCP_TRANSPORT_LAYER_TCPIP);
    }

    if(XCP_TRANSPORT_LAYER_TCPIP == Xcp_GetActiveTl((uint8)(XCP_TRANSPORT_LAYER_TCPIP + TcpIpXcp_RxPduId)))
    {
      #if (TCPIPXCP_ALIGN_RXBUFFER == STD_ON)
        /* Copy to temporary buffer to guarantee 32Bit alignment */
        Xcp_MemCpy( (DAQBYTEPTR)&TcpIpXcp_RxFrameCache.fc[0], (ROMDAQBYTEPTR)&PduInfoPtr->SduDataPtr[TCPIPXCP_HEADER_SIZE], (uint8)xcpFrameLen);
        Xcp_Command((uint8)(XCP_TRANSPORT_LAYER_TCPIP + TcpIpXcp_RxPduId), &TcpIpXcp_RxFrameCache.dw[0]);
      #else
        /* Copying of the received frame to an 32-Bit aligned buffer is not necessary as ETH pdu are already aligned. */
        Xcp_Command((uint8)(XCP_TRANSPORT_LAYER_TCPIP + TcpIpXcp_RxPduId), (uint32*)&(PduInfoPtr->SduDataPtr[TCPIPXCP_HEADER_SIZE])); /* PRQA S 0310 */ /* MD_Xcp_0310 */
      #endif
    }
  }
#if ( XCP_NUMBER_OF_CHANNELS > 1 )
#else
# if ( TCPIPXCP_NUMBER_OF_CHANNELS > 1 )
# else
  }
# endif
#endif
  /* PRQA S 2006 2 */ /* MD_MSR_14.7 */
  /* PRQA S 6010 1 */ /* MD_MSR_STPTH */
}

/***********************************************************************************************************************
 *  Xcp_SoAdTxConfirmation
 **********************************************************************************************************************/
/*! \brief      This method is called by the SoAd interface to confirm frame transmission
 *  \param[in]  SockIdx
 *  \param[in]  DataPtr
 *  \pre        Input parameter must not be NULL.
 *  \context    Function could be called from interrupt level or from task level
 *  \note       Requirement Specification example function.
 **********************************************************************************************************************/
FUNC(void, TCPIPXCP_CODE) Xcp_SoAdTxConfirmation(PduIdType TxPduId)
{
  uint8_least i;

  /* Activation control */
#if ( TCPIPXCP_ENABLE_CONTROL == STD_ON )
  if ( TcpIpXcp_ControlState == kXcponTcpIp_Control_Disable )
  {
    TcpIpXcp_ControlDeniedCount++;
    return;
  }
#endif

  TcpIpXcp_CheckDetErrorReturnVoid( TCPIPXCP_UNINIT != TcpIpXcp_InitializationState,
                                    TCPIPXCP_TXCONFIRMATION_SERVICE_ID, TCPIPXCP_E_NOT_INITIALIZED )

  /* Search respective channel handle */
  for(i=0; i<TCPIPXCP_NUMBER_OF_CHANNELS; i++)
  {
    if( TcpIpXcp_GetTxConfPduId(i) == TxPduId )
    {
      TcpIpXcp_ConfTimeoutTimer[i] = (TcpIpXcp_TimeoutTimerType)0u;
      (void)Xcp_SendCallBack((uint8)(XCP_TRANSPORT_LAYER_TCPIP + i));
      break;
    }
  }
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
}

#if ( TCPIPXCP_MODE_CHG_API == STD_ON )
/***********************************************************************************************************************
 *  Xcp_SoConModeChg
 **********************************************************************************************************************/
/*! \brief      This method is called by the SoAd interface to notify connection state changes
 *  \param[in]  SoConId
 *  \param[in]  Mode
 *  \pre        Input parameter must not be NULL.
 *  \context    Function could be called from interrupt level or from task level
 *  \note       Requirement Specification example function.
 **********************************************************************************************************************/
FUNC(void, TCPIPXCP_CODE) Xcp_SoConModeChg(SoAd_SoConIdType SoConId, SoAd_SoConModeType Mode)
{
  /* Activation control */
#if ( TCPIPXCP_ENABLE_CONTROL == STD_ON )
  if ( TcpIpXcp_ControlState == kXcponTcpIp_Control_Disable )
  {
    TcpIpXcp_ControlDeniedCount++;
    return;
  }
#endif

  TcpIpXcp_CheckDetErrorReturnVoid( TCPIPXCP_UNINIT != TcpIpXcp_InitializationState,
                                    TCPIPXCP_SOCONMODECHG_SERVICE_ID, TCPIPXCP_E_NOT_INITIALIZED )

#if( TCPIPXCP_PROTOCOL_FORMAT == TCPIPXCP_PROTOCOL_TCP )
  if( SOAD_SOCON_OFFLINE == Mode )
  {
    uint8_least i;

    /* Search respective channel handle */
    for(i=0; i<TCPIPXCP_NUMBER_OF_CHANNELS; i++)
    {
      if( TcpIpXcp_GetSoConId(i) == SoConId )
      {
        break;
      }
    }

    if( i < TCPIPXCP_NUMBER_OF_CHANNELS )
    {
      TcpIpXcp_ConfTimeoutTimer[i] = (TcpIpXcp_TimeoutTimerType)0u;
      Xcp_Disconnect((uint8)(XCP_TRANSPORT_LAYER_TCPIP + i));
    }
  }
#else
# if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
   SoConId = SoConId;
   Mode = Mode;
# endif
#endif
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
}
#endif

/***********************************************************************************************************************
 *  TcpIpXcp_Send
 **********************************************************************************************************************/
/*! \brief      This method is responsible to add Xcp On Ethernet specific information and send it via socks interface
 *  \param[in]  len: Length of xcp frame to send
 *  \param[in]  msg: Pointer to xcp frame
 *  \pre        
 *  \context    Function could be called from interrupt level or from task level
 *  \note       
 **********************************************************************************************************************/
FUNC(void, TCPIPXCP_CODE) TcpIpXcp_Send( uint8 Xcp_Channel, uint8 len, P2CONST(uint8, AUTOMATIC, TCPIPXCP_APPL_DATA) msg )
{
  PduInfoType PduInfo;

  /* Activation control */
#if ( TCPIPXCP_ENABLE_CONTROL == STD_ON )
  if ( TcpIpXcp_ControlState == kXcponTcpIp_Control_Disable )
  {
    TcpIpXcp_ControlDeniedCount++;
    return;
  }
#endif

  TcpIpXcp_CheckDetErrorReturnVoid( TCPIPXCP_UNINIT != TcpIpXcp_InitializationState,
                                    TCPIPXCP_SEND_SERVICE_ID, TCPIPXCP_E_NOT_INITIALIZED )
  TcpIpXcp_CheckDetErrorReturnVoid( msg != (P2VAR(uint8, AUTOMATIC, TCPIPXCP_APPL_VAR))NULL_PTR,
                                    TCPIPXCP_SEND_SERVICE_ID, TCPIPXCP_E_NULL_POINTER )

  /* Set correct channel index */
#if ( XCP_NUMBER_OF_CHANNELS <= 1 )
  Xcp_Channel = (uint8)TcpIpXcp_RxPduId;
#else
  Xcp_Channel -= XCP_TRANSPORT_LAYER_TCPIP;
#endif
  TcpIpXcp_CheckDetErrorReturnVoid( Xcp_Channel < TCPIPXCP_NUMBER_OF_CHANNELS,
                                    TCPIPXCP_SEND_SERVICE_ID, TCPIPXCP_E_INV_SOCK_IDX )

  TcpIpXcp_TxFrameCache[Xcp_Channel].fc[0] = len;
  TcpIpXcp_TxFrameCache[Xcp_Channel].fc[2] = (uint8)(TcpIpXcp_SequenceCounter[Xcp_Channel] & 0xff);
  TcpIpXcp_TxFrameCache[Xcp_Channel].fc[3] = (uint8)(TcpIpXcp_SequenceCounter[Xcp_Channel] >> 8);
  TcpIpXcp_SequenceCounter[Xcp_Channel]++;

  TCPIPXCP_COPY( &(TcpIpXcp_TxFrameCache[Xcp_Channel].fc[TCPIPXCP_HEADER_SIZE]), msg, len );

  PduInfo.SduDataPtr = (P2VAR(uint8, AUTOMATIC, TCPIPXCP_APPL_VAR))&TcpIpXcp_TxFrameCache[Xcp_Channel].fc[0];
  PduInfo.SduLength  = (PduLengthType)len + 4;

#if ( TCPIPXCP_ENABLE_PDUMODE == STD_ON )
  if ( TCPIPXCP_SET_ONLINE == TcpIpXcp_PduState[Xcp_Channel] )
  {
#endif
    if( (Std_ReturnType)E_OK != SoAd_IfTransmit(TcpIpXcp_GetSoAdTxPduId(Xcp_Channel), &PduInfo) )
    {
      /* Error. XCP Packet cannot be transmitted. */
      TcpIpXcp_SendWithoutQueueState[Xcp_Channel] = (uint8)TCPIPXCP_MSG_PENDING;
      TcpIpXcp_QueuedPduInfo[Xcp_Channel]         = PduInfo;
    }
    else
    {
      TcpIpXcp_ConfTimeoutTimer[Xcp_Channel] = (TcpIpXcp_TimeoutTimerType)TCPIPXCP_CONF_TIMEOUT;
    }
#if ( TCPIPXCP_ENABLE_PDUMODE == STD_ON )
  }
  else
  {
    TcpIpXcp_SendWithoutQueueState[Xcp_Channel] = (uint8)TCPIPXCP_MSG_PENDING;
    TcpIpXcp_QueuedPduInfo[Xcp_Channel]         = PduInfo;
  }
#endif
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
}

/***********************************************************************************************************************
 *  TcpIpXcp_SendFlush
 **********************************************************************************************************************/
/*! \brief      This method is responsible to flush existing transmit buffers if required
 *  \param[in]  XcpFlushTypeSel: XCP_FLUSH_CTO, XCP_FLUSH_DTO, XCP_FLUSH_ALL
 *  \pre        
 *  \context    Function could be called from interrupt level or from task level
 *  \note       
 **********************************************************************************************************************/
FUNC(void, TCPIPXCP_CODE) TcpIpXcp_SendFlush( uint8 Xcp_Channel, uint8 XcpFlushTypeSel )
{
  /* Activation control */
#if ( TCPIPXCP_ENABLE_CONTROL == STD_ON )
  if ( TcpIpXcp_ControlState == kXcponTcpIp_Control_Disable )
  {
    TcpIpXcp_ControlDeniedCount++;
    return;
  }
#endif

  TcpIpXcp_CheckDetErrorReturnVoid( TCPIPXCP_UNINIT != TcpIpXcp_InitializationState,
                                    TCPIPXCP_SENDFLUSH_SERVICE_ID, TCPIPXCP_E_NOT_INITIALIZED )

  #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  Xcp_Channel = Xcp_Channel;
  XcpFlushTypeSel = XcpFlushTypeSel;
  #endif
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
}

/***********************************************************************************************************************
 *  TcpIpXcp_TLService
 **********************************************************************************************************************/
/*! \brief      This function handles custom, Transport Layer specific commands which are:
 *  \param[in]  pCmd: Pointer to cmd string
 *  \pre        
 *  \context    Function could be called from interrupt level or from task level
 *  \note       
 **********************************************************************************************************************/
FUNC(uint8, TCPIPXCP_CODE) TcpIpXcp_TLService( uint8 Xcp_Channel, P2CONST(uint8, AUTOMATIC, TCPIPXCP_APPL_DATA) pCmd )
{
  /* Activation control */
#if ( TCPIPXCP_ENABLE_CONTROL == STD_ON )
  if ( TcpIpXcp_ControlState == kXcponTcpIp_Control_Disable )
  {
    TcpIpXcp_ControlDeniedCount++;
    return(XCP_CMD_UNKNOWN);
  }
#endif

  TcpIpXcp_CheckDetErrorReturnValue( TCPIPXCP_UNINIT != TcpIpXcp_InitializationState,
                                     TCPIPXCP_TLSERVICE_SERVICE_ID, TCPIPXCP_E_NOT_INITIALIZED, (uint8)XCP_CMD_UNKNOWN )

  #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  Xcp_Channel = Xcp_Channel;
  pCmd = pCmd;
  #endif
  return(XCP_CMD_UNKNOWN);
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
}

/***********************************************************************************************************************
 *  TcpIpXcp_MainFunction
 **********************************************************************************************************************/
/*! \brief      This method is the mainfunction and must be called cyclically
 *  \pre        
 *  \context    Function could be called from task level
 *  \note       
 **********************************************************************************************************************/
FUNC(void, TCPIPXCP_CODE) TcpIpXcp_MainFunction( void ) /* PRQA S 3408 */ /* MD_Xcp_3408_MFCT */
{
  uint8_least i;

  /* Activation control */
#if ( TCPIPXCP_ENABLE_CONTROL == STD_ON )
  if ( TcpIpXcp_ControlState == kXcponTcpIp_Control_Disable )
  {
    TcpIpXcp_ControlDeniedCount++;
    return;
  }
#endif

  for( i = 0; i < TCPIPXCP_NUMBER_OF_CHANNELS; i++ )
  {
#if ( TCPIPXCP_ENABLE_PDUMODE == STD_ON )
    if ( TCPIPXCP_SET_ONLINE == TcpIpXcp_PduState[i] )
#endif
    {
      /* If a message is pending, try to send it */
      if ( (uint8)TCPIPXCP_MSG_PENDING == TcpIpXcp_SendWithoutQueueState[i] )
      {
        /* Try to transmit the XCP Packet */
        if( (Std_ReturnType)E_OK == SoAd_IfTransmit(TcpIpXcp_GetSoAdTxPduId(i), &TcpIpXcp_QueuedPduInfo[i]) )
        {
          /* The message was sent successfully. Clear the 'message pending' bit. */
          TcpIpXcp_SendWithoutQueueState[i] = (uint8)TCPIPXCP_MSG_IDLE;
          TcpIpXcp_ConfTimeoutTimer[i] = (TcpIpXcp_TimeoutTimerType)TCPIPXCP_CONF_TIMEOUT;
        }
        else
        {
          /* The message is still pending. */
        }
      }
    }

    TcpIpXcp_EnterCritical(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if( TcpIpXcp_ConfTimeoutTimer[i] > 0u)
    {
      TcpIpXcp_ConfTimeoutTimer[i]--;
      TcpIpXcp_LeaveCritical(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      if( 0u == TcpIpXcp_ConfTimeoutTimer[i] )
      {
        /* Timeout happened, release PDU */
        (void)Xcp_SendCallBack((uint8)(XCP_TRANSPORT_LAYER_TCPIP + i));
      }
    }
    else
    {
      TcpIpXcp_LeaveCritical(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }
  }
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
}

/**********************************************************************************************************************
 * TcpIpXcp_InitMemory
 **********************************************************************************************************************/
/*! \brief      This method initializes the component and must be called before any other service
 *  \pre        
 *  \context    System Startup
 *  \note       Specification of module initialisation
 **********************************************************************************************************************/
FUNC(void, TCPIPXCP_CODE) TcpIpXcp_InitMemory( void )
{
#if ( TCPIPXCP_ENABLE_CONTROL == STD_ON )
  TcpIpXcp_ControlState = (uint8)kXcponTcpIp_Control_Enable;
  TcpIpXcp_ControlDeniedCount = 0u;
#endif

#if ( TCPIPXCP_DEV_ERROR_DETECT == STD_ON )
  TcpIpXcp_InitializationState = TCPIPXCP_UNINIT;
#endif
}

/**********************************************************************************************************************
 * TcpIpXcp_Init
 **********************************************************************************************************************/
/*! \brief      This method initializes the component and must be called before any other service
 *  \pre        
 *  \context    System Startup
 *  \note       Specification of module initialisation
 **********************************************************************************************************************/
FUNC(void, TCPIPXCP_CODE) TcpIpXcp_Init( P2CONST(TcpIpXcp_ConfigType, AUTOMATIC, TCPIPXCP_PBCFG) ConfigPtr )
{
  uint8_least i;

  /* Activation control */
#if ( TCPIPXCP_ENABLE_CONTROL == STD_ON )
  if ( TcpIpXcp_ControlState == kXcponTcpIp_Control_Disable )
  {
    TcpIpXcp_ControlDeniedCount++;
    return;
  }
#endif

#if ( TCPIPXCP_DEV_ERROR_DETECT == STD_ON )
  TcpIpXcp_InitializationState = TCPIPXCP_UNINIT;
#endif

#if ( TCPIPXCP_CONFIG_VARIANT == 3 )
  TcpIpXcp_Configuration = ConfigPtr;
#else
  #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  /* avoid compiler warning due to unused parameters */
  ConfigPtr = ConfigPtr;
  #endif
#endif

  /* Initialize static variables. */
#if ( XCP_NUMBER_OF_CHANNELS <= 1 )
  TcpIpXcp_RxPduId = TcpIpXcp_GetRxPduId(0);
#endif
  for( i = 0; i < TCPIPXCP_NUMBER_OF_CHANNELS; i++ )
  {
#if ( TCPIPXCP_ENABLE_PDUMODE == STD_ON )
    TcpIpXcp_PduState[i] = (uint8)TCPIPXCP_SET_OFFLINE;
#endif
    /* Set the length to 0 */
    TcpIpXcp_TxFrameCache[i].fc[0] = (uint8)0u;
    TcpIpXcp_TxFrameCache[i].fc[1] = (uint8)0u;
    TcpIpXcp_ConfTimeoutTimer[i] = (TcpIpXcp_TimeoutTimerType)0u;
    TcpIpXcp_SequenceCounter[i] = (uint16)0u;
    TcpIpXcp_SendWithoutQueueState[i] = (uint8)TCPIPXCP_MSG_IDLE;
  }

#if ( TCPIPXCP_DEV_ERROR_DETECT == STD_ON )
  TcpIpXcp_InitializationState = TCPIPXCP_INIT;
#endif
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
}

#if ( TCPIPXCP_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 * TcpIpXcp_GetVersionInfo
 **********************************************************************************************************************/
/*! \brief        Query module version information
 *  \param[inOut] Version information pointer
 *  \pre        
 *  \context      Application context
 **********************************************************************************************************************/
FUNC(void, TCPIPXCP_CODE) TcpIpXcp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, TCPIPXCP_APPL_DATA) versioninfo)
{
  /* Activation control */
#if ( TCPIPXCP_ENABLE_CONTROL == STD_ON )
  if ( TcpIpXcp_ControlState == kXcponTcpIp_Control_Disable )
  {
    TcpIpXcp_ControlDeniedCount++;
    return;
  }
#endif

  TcpIpXcp_CheckDetErrorReturnVoid( versioninfo != (P2VAR(Std_VersionInfoType, AUTOMATIC, TCPIPXCP_APPL_VAR))NULL_PTR,
                                  TCPIPXCP_GETVERSIONINFO_SERVICE_ID, TCPIPXCP_E_NULL_POINTER )

  versioninfo->vendorID         = TCPIPXCP_VENDOR_ID;
  versioninfo->moduleID         = TCPIPXCP_MODULE_ID;
  versioninfo->sw_major_version = TCPIPXCP_SW_MAJOR_VERSION;
  versioninfo->sw_minor_version = TCPIPXCP_SW_MINOR_VERSION;
  versioninfo->sw_patch_version = TCPIPXCP_SW_PATCH_VERSION;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
}
#endif

#if ( TCPIPXCP_ENABLE_PDUMODE == STD_ON )
/*******************************************************************************
** Function:    TcpIpXcp_SetPduMode
**
** Purpose:     Enable or disable the component
**
**  Called by:  State Manager
**
**  Parameter:  command : enable/disable
**
**  Returncode: -
*******************************************************************************/
FUNC(void, TCPIPXCP_CODE) TcpIpXcp_SetPduMode( NetworkHandleType XcpNwH, TcpIpXcp_PduSetModeType PduMode )
{
  TcpIpXcp_CheckDetErrorReturnVoid( TCPIPXCP_UNINIT != TcpIpXcp_InitializationState,
                                    TCPIPXCP_SETPDUMODE_SERVICE_ID, TCPIPXCP_E_NOT_INITIALIZED )

  TcpIpXcp_PduState[XcpNwH] = PduMode;
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
}
#endif

#if defined ( XCP_ENABLE_DAQ ) && defined ( XCP_ENABLE_DAQ_RESUME )
/*******************************************************************************
**  Function:   TcpIpXcp_DaqResumeGet
**
**  Purpose:    Retrieve socket connection information needed to configure
**              a socket once resume mode is initialized
**
**  Called by:  application call-back
**
**  Parameter:  resumeData: Pointer to resume data information
**
**  Returncode: -
*******************************************************************************/
FUNC(void, TCPIPXCP_CODE) TcpIpXcp_DaqResumeGet( CONSTP2VAR(SoAd_SockAddrIn6Type, AUTOMATIC, XCP_APPL_DATA) resumeData )
{
  uint8_least i;

  /* Activation control */
#if ( TCPIPXCP_ENABLE_CONTROL == STD_ON )
  if ( TcpIpXcp_ControlState == kXcponTcpIp_Control_Disable )
  {
    TcpIpXcp_ControlDeniedCount++;
    return;
  }
#endif

  TcpIpXcp_CheckDetErrorReturnVoid( TCPIPXCP_UNINIT != TcpIpXcp_InitializationState,
                                    TCPIPXCP_DAQRESUMEGET_SERVICE_ID, TCPIPXCP_E_NOT_INITIALIZED )
  TcpIpXcp_CheckDetErrorReturnVoid( resumeData != (CONSTP2VAR(SoAd_SockAddrIn6Type, AUTOMATIC, XCP_APPL_DATA))NULL_PTR,
                                    TCPIPXCP_DAQRESUMEGET_SERVICE_ID, TCPIPXCP_E_NULL_POINTER )

  for(i=0; i<TCPIPXCP_NUMBER_OF_CHANNELS; i++)
  {
    (void)SoAd_GetRemoteAddr(TcpIpXcp_GetSoConId(i), (CONSTP2VAR(SoAd_SockAddrType, AUTOMATIC, XCP_APPL_DATA))&(resumeData[i]));
  }
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
}

/*******************************************************************************
**  Function:   TcpIpXcp_DaqResumeStore
**
**  Purpose:    Restore socket connection information needed for resume mode
**
**  Called by:  application call-back
**
**  Parameter:  resumeData: Pointer to resume data information
**
**  Returncode: -
*******************************************************************************/
FUNC(void, TCPIPXCP_CODE) TcpIpXcp_DaqResumeStore( CONSTP2CONST(SoAd_SockAddrIn6Type, AUTOMATIC, XCP_APPL_DATA) resumeData )
{
  uint8_least i;

  /* Activation control */
#if ( TCPIPXCP_ENABLE_CONTROL == STD_ON )
  if ( TcpIpXcp_ControlState == kXcponTcpIp_Control_Disable )
  {
    TcpIpXcp_ControlDeniedCount++;
    return;
  }
#endif

  TcpIpXcp_CheckDetErrorReturnVoid( TCPIPXCP_UNINIT != TcpIpXcp_InitializationState,
                                    TCPIPXCP_DAQRESUMESTORE_SERVICE_ID, TCPIPXCP_E_NOT_INITIALIZED )
  TcpIpXcp_CheckDetErrorReturnVoid( resumeData != (CONSTP2CONST(SoAd_SockAddrIn6Type, AUTOMATIC, XCP_APPL_DATA))NULL_PTR,
                                    TCPIPXCP_DAQRESUMESTORE_SERVICE_ID, TCPIPXCP_E_NULL_POINTER )

  for(i=0; i<TCPIPXCP_NUMBER_OF_CHANNELS; i++)
  {
    (void)SoAd_SetRemoteAddr(TcpIpXcp_GetSoConId(i), (CONSTP2CONST(SoAd_SockAddrType, AUTOMATIC, XCP_APPL_DATA))&(resumeData[i]));
  }
  /* PRQA S 2006 1 */ /* MD_MSR_14.7 */
}
#endif /* defined ( XCP_ENABLE_DAQ ) && defined ( XCP_ENABLE_DAQ_RESUME ) */

#define TCPIPXCP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* module specific MISRA deviations:
  MD_Xcp_0310:    Rule 11.4
      Reason:     pointer cast has no side effects if underlying layer aligns the buffer accordingly.
      Risk:       If this option is enabled and the buffer is not aligned a trap will occur.
      Prevention: Optional feature
  MD_Xcp_3408:    Rule 8.8
      Reason:     The variable is not defined as static to be accessible for measurement. No prototype is required for this.
      Risk:       There is no risk as this variable is not accessed externally.
      Prevention: Covered by code review.
  MD_Xcp_3408_MFCT: Rule 8.8
      Reason:     The MainFunction has no prototype because it is, according to ASR, declared by the SchM.
      Risk:       There is no risk as this function is only called by the SchM.
      Prevention: Covered by code review.
  MD_Xcp_3448:    Rule 5.3
      Reason:     The type is only used locally. The globally visible definition is due to debugging and measurement.
      Risk:       There is no risk as this variable is not accessed externally.
      Prevention: Covered by code review.
*/

/**********************************************************************************************************************
 *  END OF FILE: TcpIpXcp.C
 *********************************************************************************************************************/
