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
 *         File:  TcpIpXcp.h
 *    Component:  TcpIpXcp Header
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

#if !defined (TCPIPXCP_H)
#define TCPIPXCP_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "ComStack_Types.h"
#include "Xcp.h"                /* Include header of XCP Protocol Layer */
#include "TcpIpXcp_Cfg.h"
#include "TcpIpXcp_Cbk.h"

#if defined (TCPIPXCP_CONFIGURATOR) && (TCPIPXCP_CONFIGURATOR == 5)
#else
# include "SchM_TcpIpXcp.h"
#endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : <REQ_SPEC_COMPONENT> CQComponent : <Implementation> */
/* Version - BCD coded version number - Main- and Sub-Version - Release-Version */
#define V_TCPIPXCP_COMPONENT_VERSION          (0x0300u)
#define V_TCPIPXCP_COMPONENT_RELEASE_VERSION  (0x01u)

/* vendor and module identification */
#define TCPIPXCP_VENDOR_ID                    (30u)
#define TCPIPXCP_MODULE_ID                    (255u)

/* AUTOSAR Software Specification Version Information */
#define TCPIPXCP_AR_MAJOR_VERSION             (0x04)
#define TCPIPXCP_AR_MINOR_VERSION             (0x00)
#define TCPIPXCP_AR_PATCH_VERSION             (0x03)

/* Component Version Information */
#define TCPIPXCP_SW_MAJOR_VERSION             (V_TCPIPXCP_COMPONENT_VERSION >> 8u)
#define TCPIPXCP_SW_MINOR_VERSION             (V_TCPIPXCP_COMPONENT_VERSION & 0x00FF)
#define TCPIPXCP_SW_PATCH_VERSION             V_TCPIPXCP_COMPONENT_RELEASE_VERSION

/* Interrupt enable and disable functions */
# define TcpIpXcp_EnterCritical()            SchM_Enter_Xcp_TCPIPXCP_EXCLUSIVE_AREA_0()
# define TcpIpXcp_LeaveCritical()            SchM_Exit_Xcp_TCPIPXCP_EXCLUSIVE_AREA_0()

#if defined ( ApplXcpInterruptDisable )
#else
#  define ApplXcpInterruptDisable()           SchM_Enter_Xcp_TCPIPXCP_EXCLUSIVE_AREA_0()
#endif
#if defined ( ApplXcpInterruptEnable )
#else
#  define ApplXcpInterruptEnable()            SchM_Exit_Xcp_TCPIPXCP_EXCLUSIVE_AREA_0()
#endif

#define TCPIPXCP_HEADER_SIZE                  4u

#define TCPIPXCP_RXINDICATION_SERVICE_ID      0u
#define TCPIPXCP_TXCONFIRMATION_SERVICE_ID    1u
#define TCPIPXCP_SOCONMODECHG_SERVICE_ID      2u
#define TCPIPXCP_SEND_SERVICE_ID              3u
#define TCPIPXCP_SENDFLUSH_SERVICE_ID         4u
#define TCPIPXCP_TLSERVICE_SERVICE_ID         5u
#define TCPIPXCP_MAINFUNCTION_SERVICE_ID      6u
#define TCPIPXCP_INIT_SERVICE_ID              7u
#define TCPIPXCP_GETVERSIONINFO_SERVICE_ID    8u
#define TCPIPXCP_CONTROL_SERVICE_ID           9u
#define TCPIPXCP_SETPDUMODE_SERVICE_ID       10u
#define TCPIPXCP_DAQRESUMEGET_SERVICE_ID     11u
#define TCPIPXCP_DAQRESUMESTORE_SERVICE_ID   12u

#define TCPIPXCP_E_NULL_POINTER               0u
#define TCPIPXCP_E_INV_SOCK_IDX               1u
#define TCPIPXCP_E_WRONG_CONTROL_COMMAND      2u
#define TCPIPXCP_E_NOT_INITIALIZED            3u

/* Initialization status of XCP. */
#define TCPIPXCP_UNINIT   0u
#define TCPIPXCP_INIT     1u

#if !defined ( TCPIPXCP_ENABLE_PDUMODE )
# if ( defined TCPIPXCP_AUTOSARVERSION ) && ( TCPIPXCP_AUTOSARVERSION == 411 )
#  define TCPIPXCP_ENABLE_PDUMODE STD_OFF
# else
#  define TCPIPXCP_ENABLE_PDUMODE STD_ON
# endif
#endif

#if !defined ( TCPIPXCP_PROTOCOL_FORMAT )
# define TCPIPXCP_PROTOCOL_FORMAT TCPIPXCP_PROTOCOL_UDP
#endif

#if !defined ( TCPIPXCP_ALIGN_RXBUFFER )
# define TCPIPXCP_ALIGN_RXBUFFER STD_ON
#endif

#if !defined ( TCPIPXCP_CONF_TIMEOUT )
# define TCPIPXCP_CONF_TIMEOUT 25u
#endif

/* The function XcpGetSessionStatus is required for the protection against multiple connections. */
#if defined ( TCPIPXCP_MULTI_CONN_PROT_ENABLED )
  #if !defined ( XCP_ENABLE_GET_SESSION_STATUS_API )
    #define XCP_ENABLE_GET_SESSION_STATUS_API
  #endif
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define TCPIPXCP_START_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern CONST(uint8, TCPIPXCP_CONST) TcpIpXcp_MainVersion;
extern CONST(uint8, TCPIPXCP_CONST) TcpIpXcp_SubVersion;
extern CONST(uint8, TCPIPXCP_CONST) TcpIpXcp_ReleaseVersion;

#define TCPIPXCP_STOP_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( TCPIPXCP_ENABLE_CONTROL == STD_ON )
# define TCPIPXCP_START_SEC_VAR_INIT_UNSPECIFIED_SAFE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* TL Enable/Disable state */
extern VAR(uint8, TCPIPXCP_VAR_INIT) TcpIpXcp_ControlState;

# define TCPIPXCP_STOP_SEC_VAR_INIT_UNSPECIFIED_SAFE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define TCPIPXCP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Service Functions */
extern FUNC(void, TCPIPXCP_CODE) TcpIpXcp_Send( uint8 Xcp_Channel, uint8 len, P2CONST(uint8, AUTOMATIC, TCPIPXCP_APPL_DATA) msg );
extern FUNC(void, TCPIPXCP_CODE) TcpIpXcp_SendFlush( uint8 Xcp_Channel, uint8 XcpFlushTypeSel );
extern FUNC(uint8, TCPIPXCP_CODE) TcpIpXcp_TLService( uint8 Xcp_Channel, P2CONST(uint8, AUTOMATIC, TCPIPXCP_APPL_DATA) pCmd );
extern FUNC(void, TCPIPXCP_CODE) TcpIpXcp_InitMemory( void );
extern FUNC(void, TCPIPXCP_CODE) TcpIpXcp_Init( P2CONST(TcpIpXcp_ConfigType, AUTOMATIC, TCPIPXCP_PBCFG) ConfigPtr );

#if ( TCPIPXCP_VERSION_INFO_API == STD_ON )
extern FUNC(void, TCPIPXCP_CODE) TcpIpXcp_GetVersionInfo( P2VAR(Std_VersionInfoType, AUTOMATIC, TCPIPXCP_APPL_DATA) versioninfo );
#endif

#if ( TCPIPXCP_ENABLE_CONTROL == STD_ON )
# define kXcponTcpIp_Control_Disable   0x00u
# define kXcponTcpIp_Control_Enable    0x01u
#endif

#if ( TCPIPXCP_ENABLE_PDUMODE == STD_ON )
extern FUNC(void, TCPIPXCP_CODE) TcpIpXcp_SetPduMode( NetworkHandleType XcpNwH, TcpIpXcp_PduSetModeType PduMode );
#else
# define TcpIpXcp_SetPduMode( XcpNwH, PduMode )
#endif

#if defined ( XCP_ENABLE_DAQ ) && defined ( XCP_ENABLE_DAQ_RESUME )
extern FUNC(void, TCPIPXCP_CODE) TcpIpXcp_DaqResumeGet( CONSTP2VAR(SoAd_SockAddrIn6Type, AUTOMATIC, XCP_APPL_DATA) resumeData );
extern FUNC(void, TCPIPXCP_CODE) TcpIpXcp_DaqResumeStore( CONSTP2CONST(SoAd_SockAddrIn6Type, AUTOMATIC, XCP_APPL_DATA) resumeData );
#endif

#define TCPIPXCP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif
  /* TCPIPXCP_H */

/**********************************************************************************************************************
 *  END OF FILE: TcpIpXcp.h
 *********************************************************************************************************************/
