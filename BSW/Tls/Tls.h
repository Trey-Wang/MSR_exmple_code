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
 *         File:  Tls.h
 *      Project:  OnBoardCharger
 *       Module:  Transport Layer Security
 *    Generator:  SysSercice_Tls.dll
 *
 *  Description:  module header
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Thorsten Albers               visal         Vector Informatik GmbH
 *  Daniel Dausend                visdd         Vector Informatik GmbH
 *  Fabian Eisele                 visefa        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2010-04-12  visal   -             created
 *            2010-06-10  visal   -             basic functionality implemented
 *                                              - handshake with predefined cipher suite
 *                                              - handshake and data transfer using TLS_RSA_WITH_AES_128_CBC_SHA
 *                                              - data transfer only for non-fragmented messages
 *  01.01.00  2010-06-30  visal   -             Support of cipher suites TLS_RSA_WITH_AES_128_CBC_SHA and
 *                                                TLS_RSA_WITH_NULL_SHA, the server can choose
 *            2010-06-30  visal   -             Certificate is scanned for the key
 *            2010-06-30  visal   -             CERT: created, derived from RCS
 *            2010-07-01  visal   -             CERT: fix parser (inner/outer part problem)
 *            2010-07-02  visal   -             Internal global buffers saved
 *            2010-07-05  visal   -             Support of large encrypted rx messages
 *            2010-07-05  visal   -             Check hash in 'server finished' message
 *            2010-07-06  visal   -             MAC of received messages is checked
 *            2010-07-06  visal   -             Content of 'server_finished' is checked
 *            2010-07-06  visal   -             Multiple opening and closing of TLS connections is implemented
 *            2010-07-14  visal   -             Rename defines and variables in certificate parser
 *            2010-07-21  visal   -             GENy dll implemented, config is OK
 *            2010-07-21  visal   -             Support of config variants (precompile, link time)
 *            2010-07-27  visal   -             Support of AES encrypted messages with TLS padding-length '0'
 *            2010-07-30  visal   -             CERT: Add support of access to elements that are no leaf
 *            2010-08-18  visal   -             connection shutdown handling improved
 *            2010-08-23  visal   -             support for client certificates prepared
 *  01.01.01  2010-08-27  visal   -             some reworks
 *            2010-10-20  visal   -             replace Http_Helper functions by IpBase functions
 *            2010-10-20  visal   -             replace SoAd types by IpBase types
 *            2010-11-08  visal   ESCAN00046713 missing support for RSA key with 2048 bit fixed
 *            2010-11-09  visal   ESCAN00046718 Compiler error with DiabData5.7 fixed
 *  01.01.02  2010-11-17  visal   -             typecasts for call of IpBase_Copy reworked
 *            2010-11-17  visal   -             adapt Tls_Time_GetTime() for platforms not equal to CANoeEmu
 *  01.01.03  2010-11-19  visal   ESCAN00047078 problem with AES decryption fixed
 *            2010-11-22  visal   ESCAN00045092 Compile error for configurations without Rcs and Http module fixed
 *  01.01.04  2010-12-03  visal   ESCAN00047442 Problem with very long RecordLayer messages fixed
 *            2010-12-09  visal   ESCAN00047567 Undefined behavior due to overwritten memory fixed
 *  01.01.05  2010-12-17  visal   -             fix some compiler warnings for DiabData
 *  01.01.06  2011-01-03  visal   ESCAN00047635 add callbacks to upper layer when receiving TCP events
 *            2011-01-05  visal   ESCAN00047897 local instantiation of crypto workspaces replaced
 *            2011-01-11  visal   ESCAN00047955 Tls_Shutdown() function added
 *            2011-01-27  visal   ESCAN00048019 MemMap.h: Missing Start/Stop Sections, fixed
 *  01.02.00  2011-03-01  visal   ESCAN00047112 handling of client certificates does not work, implemented
 *            2011-03-01  visal   ESCAN00048273 TLS sends alert when receiving a hello_request, fixed
 *            2011-03-01  visal   -             server cert verification implemented
 *            2011-03-01  visal   ESCAN00047168 Move random function to external file
 *            2011-03-07  visal   -             usage of NVM for cert storage (read access)
 *  01.02.01  2011-03-10  visal   -             define missing type, and change name of external time function
 *  01.02.02  2011-03-11  visal   ESCAN00049220 module sends wrong certificate as client certificate
 *  01.02.03  2011-03-15  visal   ESCAN00049190 Tls internal parsing of server certs is not robust enough, fixed
 *            2011-03-15  visal   ESCAN00049292 Handle cert request even if client cert is not supported
 *            2011-03-17  visal   -             Include only NvM.h, no other headers of this component
 *  01.02.04  2011-03-18  visal   -             minor code reworks
 *            2011-03-25  visal   ESCAN00049445 Tls handshake fails if tx buffer is not available
 *  01.02.05  2011-04-01  visal   ESCAN00049736 TLS verification of ServerFinished handshake message fails
 *            2011-04-01  visal   -             Handle pending transmit requests inside the MainFunction
 *  02.00.00  2011-04-01  visal   ESCAN00049544 Protocols using TLS protected connection may get stuck after sending
 *                                                some data blocks
 *            2011-04-05  visal   -             verification of single server certificate fixed
 *            2011-04-13  visal   -             verification of server certificate chain implemented
 *            2011-04-28  visal   -             move Tls_GetTime32() to Tls_Lcfg.c and rename it
 *  02.00.01  2011-06-09  visal   ESCAN00049999 GHS compiler warnings resolved
 *            2011-06-27  visal   ESCAN00051864 TLS length of client_hello_extension_list is wrong
 *            2011-07-12  visal   ESCAN00052129 Support Client Authentication for ECDH CipherSuite
 *  02.00.02  2011-08-15  visal   ESCAN00052849 MISRA deviation: MISRA-C:2004 multiple Rules
 *            2011-08-17  visal   -             rework of main function due to erroneous state machine
 *  02.00.03  2011-08-26  visal   -             remove old Tls_Cbk_CertParser.. functions from callback header
 *  02.00.04  2011-08-29  visdd   ESCAN00053115 Established TLS connections may be closed by the server due to an
 *                                                encoding error
 *  02.00.05  2011-09-23  visal   -             add state NVM_REQ_NV_INVALIDATED to NVM handling
 *            2011-09-27  visal   -             calc RSA signature using P-Q parameters of the key
 *  02.00.06  2011-10-04  visal   ESCAN00054021 TLS connection closed notification too early
 *            2011-10-06  visal   ESCAN00054073 initialized RAM variables in Tls_Lcfg.c, fixed
 *            2011-10-06  visal   ESCAN00053414 Addtional preprocessor argument for macro without DEM has to be removed
 *  02.00.07  2011-10-26  visal   ESCAN00054532 Change ModuleId to 255 (CDD) and set InstanceId instead
 *            2011-10-27  visal   -             Add check for Crypto initialization (only with DET enabled)
 *  03.00.00  2011-12-16  visal   ESCAN00055645 Support for new sock addr struct
 *            2011-12-16  visal   -             Remove empty files Tls_Time.c and .h
 *  03.00.01  2011-12-19  visal   ESCAN00054289 Unpredictable behavior due to memory overwriting around Tls_NvmExchData
 *  03.00.02  2012-01-18  visal   -             CANoeWriteString support changed
 *            2012-01-18  visal   -             time measurement functions added (internal)
 *            2012-01-20  visal   ESCAN00056191 Check order of received handshake messages
 *            2012-02-21  visal   ESCAN00054343 Don't send close_notify when peer starts closing with TCP FIN
 *            2012-03-20  visal   ESCAN00057730 Handling of client certificate fails
 *            2012-04-02  visal   ESCAN00058057 Remove DET checks that should also be active at normal operation
 *            2012-04-02  visal   -             Remove Tls_VCfgGetDummy
 *            2012-04-02  visal   ESCAN00056376 VAR_INIT / VAR_ZERO_INIT Memory Mapping sections
 *  03.01.00  2012-05-09  visal   ESCAN00058809 Add assigned IP address to IP Assigned callback
 *            2012-05-10  visal   ESCAN00058841 Remove GENy DLL version check
 *            2012-05-14  visal   -             Use Tls_MainFctCounter in normal operation mode
 *            2012-05-14  visal   -             Improve random number generation by using esl_getBytesRNG
 *            2012-05-22  visal   -             Insert time measurement points (filtered)
 *            2012-06-05  visal   ESCAN00059285 Add support for certificates with ECDSA-with-SHA256 signature
 *            2012-06-06  visal   -             Add switch to disable support for RSA handshake
 *  03.02.00  2012-08-20  visal   ESCAN00060860 Implementation of TLS protocol version 1.2
 *            2012-08-20  visal   -             Reduce stack consumtion and copy runtime in
 *                                                Tls_VHs_SendClientKeyExchangeRsa
 *            2012-08-20  visal   ESCAN00060862 Implement CipherSuites with SHA256 as MAC algorithm
 *            2012-08-20  visal   ESCAN00054370 Consolidate memory elements for RSA and ECDH keys and others
 *            2012-08-20  visal   ESCAN00047341 Adapt MainFunction for usage with IdentityManagerConfig
 *            2012-09-04  visal   ESCAN00061183 Implement cipher suite TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA and _SHA256
 *            2012-09-21  visal   -             Remove usage of the array Tls_CfgSockHnd2TlsConnMap for Cfg5
 *            2012-09-24  visal   -             use global header history for _MemMap.inc and _Compiler_Cfg.inc
 *            2012-10-01  visal   ESCAN00061889 Prepare code for usage with ASR4
 *            2012-10-01  visal   -             Rename signature and hash algorithm IDs
 *            2012-10-11  visal   ESCAN00062081 Tls_Rl_DecrytedDataLen renamed to Tls_Rl_DecryptedDataLen
 *            2012-11-27  visal   ESCAN00063284 connection setup problem with ecdsa private key structs shorter than
 *                                                usual
 *            2012-11-27  visal   ESCAN00063352 problem with ecdsa signatures sent by the ECU
 *            2012-12-13  visal   ESCAN00063703 Feature: support TLS_ECDH_ECDSA_WITH_NULL_SHA
 *  03.02.01  2012-12-21  visal   ESCAN00063867 Erroneous connection cancellation with alert 'bad record mac'
 *            2012-12-21  visal   -             Compiler Error: linking with only TLS1.2 support not possible
 *            2013-02-04  visal   ESCAN00062241 AR4-220: Remove STATIC
 *            2013-02-04  visal   ESCAN00056377 Remove Macro support for Version Info API
 *            2013-02-04  visal   ESCAN00058057 Remove DET checks that should also be active at normal operation
 *            2013-02-05  visal   ESCAN00064798 Establishing connection with TLS1.2-only does not work
 *  03.03.00  2013-03-07  visal   ESCAN00064772 Feature: Support Pass-Through Mode
 *  03.03.01  2013-07-19  visal   ESCAN00068917 Compiler error: #error "MemMap.h, wrong pragma command
 *  03.03.02  2013-10-09  visal   ESCAN00070984 Misaligned access exception during connection setup
 *            2013-10-09  visal   ESCAN00070860 Compiler warning: Signed/Unsigned Mismatch
 *                                                TCPIP_INV_SOCK_HND == TConnId
 *            2013-10-09  visal   ESCAN00070194 Subsequent connections not possible after connection try to unreachable
 *                                                host
 *  03.03.03  2014-01-09  visal   ESCAN00072818 Compiler error: error C2143 (syntax error) because of missing compiler
 *                                                abstraction
 *  03.03.04  2014-01-22  visal   ESCAN00053453 AR3-1698: Add MISRA justifications in source code for automatic
 *                                                generation of MISRA reports
 *            2014-01-28  visal   ESCAN00073262 Use Crypto_GenerateEcdsaSignature() and delete
 *                                                Tls_Pub_ValidateEcdsaSignature()
 *            2014-02-27  visal   ESCAN00073854 TLS verify of valid root certificate fails during handshake
 *  03.04.00  2014-03-07  visal   ESCAN00073687 Get NvM Block ID for connection root certificate
 *            2014-03-11  visal   ESCAN00073688 Inform TLS about updated root certificate
 *            2014-03-24  visal   ESCAN00074512 Compiler error: void function returns a value
 *            2014-04-30  visal   ESCAN00075202 TLS Handshake fails / Server reports 'Decode Error' after receiving
 *                                                Certificate Verify
 *  04.00.00  2014-07-04  visal   -             Configurator5 incompatible change
 *            2014-07-04  visal   ESCAN00076744 Implement API change for multi controller support
 *            2014-07-29  visal   ESCAN00077479 Replace compiler switch C_COMP_ANSI_CANOE by TLS_PROCESSOR_CANOEEMU
 *            2014-07-29  visal   ESCAN00077490 Allow sending less data than requested in ProvideTxBuffer
 *            2014-08-01  visal   ESCAN00077573 Keep received data until call of Tls_Received
 *            2014-08-11  visal   ESCAN00077749 Support reception of multiple appl data messages before releasing the
 *                                                first one
 *  04.01.00  2014-11-19  visal   ESCAN00078051 Division by zero while receiving message
 *            2014-12-11  visal   ESCAN00078916 Implement certificate validation using OCSP stapling V1
 *            2015-02-02  visal   ESCAN00080990 Handshake fails with receiving a 'decode error'
 *            2015-02-02  visal   ESCAN00080993 Wrong DET check reports error in parsing stored root certificates at
 *                                                ECU startup
 *  05.00.00  2015-04-01  visefa  ESCAN00082170 Added support for new ASR4.2.1 TcpIp API
 *            2015-06-23  visefa  ESCAN00083102 Incorrect calculation of Plain Text Buffer
 *            2015-07-03  visefa  ESCAN00083696 Check status of NvM block via NvM_GetErrorStatus()
                                                  before calling NvM_ReadBlock()
 *            2015-07-08  visefa  ESCAN00083896 Overwriting of memory during validation of server certificate chain
 *  05.00.01  2015-10-20  visal   ESCAN00085008 Compiler error: Wrong return type of function Tls_TcpAccepted
 *            2015-10-20  visal   ESCAN00085983 Reduce number of MISRA / PClint violations or add justifications
 *            2015-10-23  visal   ESCAN00078999 TLS does not check the validity time of the stored root certificates
 *            2015-10-26  visal   ESCAN00070040 Illegal padding check missing when doing AES decryption
 *            2015-10-26  visal   -             Remove 'extern' qualifier from function declarations
 *            2015-10-27  visal   ESCAN00057863 DET error when accessing NVM blocks
 *  05.01.00  2016-02-25  visal   ESCAN00088558 Move handshake crypto calculations to separate MainFunction
 *            2016-02-25  visal   ESCAN00088562 Use uint32 type for dynamic workspace (code + cfg)
 *            2016-02-25  visal   ------------- move module history information to main header file
 *            2016-02-26  visal   ESCAN00088570 Improve error handling when receiving unexpected server hello elements
 *  06.00.00  2016-03-07  visal   ESCAN00088757 FEAT-1347: TLS as TcpIp plug-in
 *            2016-03-07  visal   ESCAN00088760 Code cleanup
 *            2016-03-07  visal   ESCAN00088740 Handshake fails and 'Decode Error' alert is sent
 *  06.01.00  2016-04-20  visal   ESCAN00087707 FEAT-1741: Support TLS cipher suite TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
 *            2016-04-20  visal   ESCAN00089086 Using Crypto_StirRNG() with len smaller than 20 has no effect
 *            2016-04-20  visal   ------------- Remove unnecessary TLS internal variable 'Tls_Dest'
 *  06.01.01  2016-05-24  visal   ESCAN00090115 FEAT-1741: Support server certificate validation using Certificate Revocation Lists (CRL)
 *            2016-05-24  visal   ESCAN00089788 Certificate Revocation Lists are not supported
 *            2016-05-24  visal   -             Remove internal switch 'TLS_SERVER_CERT_VER_SUPPORT'
 *  06.01.02  2016-10-05  visal   ESCAN00092050 Local static variable used that cannot be mapped using MemMap.h
 *            2016-10-05  visal   -             improve process compliance
 *            2016-10-14  visal   ESCAN00092323 RSA cipher suites are not working with TLS 1.2
 *  07.00.00  2016-10-21  visal   ESCAN00092438 FEAT-2005: Release of TLS_RSA_WITH_AES_128_CBC_SHA256
 *            2016-10-21  visal   ESCAN00092440 Support RSA certificates with SHA256 signature
 *            2016-10-21  visal   ESCAN00092441 OCSP support without RealTimeClock support
 *            2016-10-21  visal   ESCAN00092442 CRL support without RealTimeClock support
 *            2016-10-21  visal   ESCAN00092445 Certificate chain with even number of certs can not be validated
 *            2016-10-26  visal   FEATC-251     FEAT-1996: TLS Heartbeat Extension
 *            2016-11-11  visal   ESCAN00092719 Compiler error: identifier 'KeyElementsPtr' is undefined
 *            2016-11-11  visal   ESCAN00092453 Client certificate support for TLS 1.2 with RSA ciphers implemented
 *  07.00.01  2016-12-06  visal   ESCAN00093081 TLS connection closes unintentionally when receiving an AES_128_CBC encrypted message
 *            2016-12-22  visal   ESCAN00093419 Code cleanup to improve condition/decision coverage
 *            2016-12-22  visal   ESCAN00093420 Disable support of Md5RSA-signed certificates
 *  08.00.00  2017-02-14  visal   ESCAN00094001 Review integration
 *            2017-02-14  visal   ESCAN00094002 Make TLS_MAC_SHA256_SUPPORT always enabled
 *            2017-02-14  visal   ESCAN00094003 Remove MainFunction declaration
 *            2017-02-14  visal   ESCAN00094004 Divide Tls_VRl_RxPayload in smaller parts
 *            2017-02-14  visal   ESCAN00094005 Remove all AUTOSAR major version dependencies to support only ASR4 now
 *            2017-02-14  visal   ESCAN00094006 Connection is not refused in case of an inconsistent OCSP status response
 *            2017-02-14  visal   ESCAN00094007 Connection is not refused in case of a too old OCSP status response
 *            2017-02-14  visal   ESCAN00094008 Introduce runtime error reporting mechanism
 *            2017-02-24  visal   FEAT-2275     Release of TLS
 *            2017-03-20  visal   ESCAN00094430 Erroneous reading of private key elements for RSA
 *            2017-03-20  visal   ESCAN00094445 Do not copy client_hello message into hash workspace
 *  08.00.01  2017-04-24  visal   ESCAN00094627 The TLS handshake stops after receiving the first set of server handshake messages
 *            2017-04-24  visal   ESCAN00094201 Bad record MAC or interchanged application data
 *            2017-04-24  visal   ESCAN00094649 Compiler error: ExtraLenHashAndSign and HashOffset ar not defined
 *            2017-04-24  visal   ESCAN00094712 Compiler error: API Tls_VHs_CalcEcdhMasterSecret is not defined
 *            2017-04-24  visal   ESCAN00094789 Implement a TLS_GET_UINT24 macro
 *            2017-04-24  visal   ESCAN00094792 Replace snprintf by snprintf_s for CANoe-Emu
 *  08.01.00  2017-04-26  visal   FEAT-2889     Support TLS 'trusted_ca_keys' extension as specified in RFC 6066
 *            2017-04-26  visal   ESCAN00089790 Implement ClientHello Extension 'Trusted CA Indication'
 *  08.02.00  2017-05-16  visal   STORYC-592    TLS Server CertChain Callout in TLS Client
 *            2017-05-16  visal   STORYC-638    Report Invalid Server CertChain (implemented with STORYC-592)
 *            2017-05-16  visal   ESCAN00095197 The handshake is cancelled if no matching CRL was found
 *            2017-05-23  visal   ESCAN00094399 Tls establishes a connection although the OCSP response contains a wrong serial number
 *            2017-05-31  visal   ESCAN00095361 TLS handshake is not completed because the client does not sent its 'finished' message
 *            2017-05-31  visal   ESCAN00095362 Review integration, change pointer memclass to AUTOMATIC for all pointers
 *            2017-06-13  visal   STORYC-1506   Grant access to the TLS master secret
 *********************************************************************************************************************/
#if !defined (TLS_H)  /* PRQA S 0883 */ /* MD_TLS_19.15_0883 */
# define TLS_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "Tls_Types.h"
#include "TcpIp.h"
#include "IpBase_Types.h"
#include "NvM.h"
#include "Tls_Lcfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ##V_CFG_MANAGEMENT ##CQProject :   SYSSERCICE_TLS CQComponent : Implementation */
#define SYSSERVICE_TLS_VERSION                   (0x0802u)  /* BCD coded version number */
#define SYSSERVICE_TLS_RELEASE_VERSION             (0x00u)  /* BCD coded release version number  */

/* software module version */
#define TLS_SW_MAJOR_VERSION                           8u
#define TLS_SW_MINOR_VERSION                           2u
#define TLS_SW_PATCH_VERSION                           0u

/* supported Autosar version */
#define TLS_AR_RELEASE_MAJOR_VERSION                   4u
#define TLS_AR_RELEASE_MINOR_VERSION                   2u
#define TLS_AR_RELEASE_REVISION_VERSION                1u

/* Tls ModuleId */
#define TLS_VENDOR_ID                                 30u  /* Vector ID */
#define TLS_MODULE_ID                                255u  /* CDD - TLS identified by InstanceId */
#define TLS_INSTANCE_ID                              103u  /* InstanceId chosen by Vector */

/* TLS ApiIds */
#define TLS_API_ID_INIT                             0x01u
#define TLS_API_ID_RX_INDICATION                    0x02u
#define TLS_API_ID_GET_VERSION_INFO                 0x03u
#define TLS_API_ID_CONNECT                          0x04u
#define TLS_API_ID_CLOSE                            0x05u
#define TLS_API_ID_RECEIVED                         0x06u
#define TLS_API_ID_PROV_TX_BUFFER                   0x07u
#define TLS_API_ID_TRANSMIT_TO                      0x08u
#define TLS_API_ID_TCP_ACCEPTED                     0x09u
#define TLS_API_ID_TCP_CONNECTED                    0x0Au
#define TLS_API_ID_TCP_IP_EVENT                     0x0Bu
#define TLS_API_ID_IP_LINK_STATE_CHG                0x0Cu
#define TLS_API_ID_MAIN_FUNCTION                    0x10u
#define TLS_API_ID_UL_RX_INDICATION                 0x20u
#define TLS_API_ID_UL_TX_CONFIRMATION               0x21u
#define TLS_API_ID_UL_TCP_ACCEPTED                  0x22u
#define TLS_API_ID_UL_TCP_CONNECTED                 0x23u
#define TLS_API_ID_UL_TCP_IP_EVENT                  0x24u
#define TLS_API_ID_SHUTDOWN                         0x25u
#define TLS_API_ID_GET_NVM_BLOCK_ID_FOR_USED_ROOT_CERT   0x27u
#define TLS_API_ID_ROOT_CERT_WAS_MODIFIED           0x28u
#define TLS_API_ID_PROV_UL_TX_BUFFER                0x29u
#define TLS_API_ID_PROV_UL_RX_BUFFER                0x2Au
#define TLS_API_ID_CHANGE_PARAMETER                 0x2Bu
#define TLS_API_ID_CRL_WAS_MODIFIED                 0x2Cu
#define TLS_API_ID_GET_SOCKET                       0x2Du
#define TLS_API_ID_SET_CLIENT_CERT_INFO             0x2Eu
#define TLS_API_ID_GET_MASTER_SECRET                0x2Fu

/* TLS internal ApiIds */
#define TLS_API_ID_V_INTERNAL_FUNCTION              0x40u
#define TLS_API_ID_V_RL_TRANSMIT                    0x41u
#define TLS_API_ID_V_HS_SEND_CLIENT_HELLO           0x42u
#define TLS_API_ID_V_HS_SEND_CLIENT_CERT            0x43u
#define TLS_API_ID_V_HS_SEND_CLIENT_KEY_EXCH_RSA    0x44u
#define TLS_API_ID_V_HS_SEND_CLIENT_KEY_EXCH_ECDH   0x45u
#define TLS_API_ID_V_HS_SEND_CLIENT_CERT_VERI       0x46u
#define TLS_API_ID_V_CCS_SEND_CHANGE_CIPHER_SPEC    0x47u
#define TLS_API_ID_V_HS_SEND_CLIENT_FINISHED        0x48u
#define TLS_API_ID_V_AL_SEND_ALERT                  0x49u
#define TLS_API_ID_V_PRF                            0x50u
#define TLS_API_ID_V_P_SHA1                         0x51u
#define TLS_API_ID_V_P_SHA256                       0x52u
#define TLS_API_ID_V_P_MD5                          0x53u
#define TLS_API_ID_AES_ENCRYPT_DATA                 0x54u
#define TLS_API_ID_AES_DECRYPT_DATA                 0x55u
#define TLS_API_ID_AES_GCM_DECRYPT_DATA             0x56u
#define TLS_API_ID_V_RL_RX_HDR                      0x57u
#define TLS_API_ID_V_RL_RX_PAYLOAD                  0x58u
/* TLS API IDs 0x80..0x8F reserved for Tls_Cert */
#define TLS_API_ID_V_PARSE_ALL_STORED_SERVER_CERTS  0x59u
#define TLS_API_ID_V_PARSE_ALL_STORED_CRLS          0x5Au

/* TLS DET errors */
#define TLS_E_NO_ERROR                              0x00u
#define TLS_E_NOT_INITIALIZED                       0x01u
#define TLS_E_INV_POINTER                           0x02u
#define TLS_E_INV_PARAM                             0x03u
#define TLS_E_INV_SOCK_ID                           0x04u
#define TLS_E_BUF_SIZE                              0x05u
#define TLS_E_CRYPTO_NOT_INIT                       0x06u
#define TLS_E_NO_VALID_ROOT_CERT                    0x07u
#define TLS_E_NO_VALID_CRL                          0x08u
#define TLS_E_UNEXPECTED_CRYPTO_ERROR               0x09u
#define TLS_E_UNEXPECTED_CFG_ERROR                  0x0Au

/* Status defines and error ids used for reporting of runtime errors (TLS_REPORT_ERROR_STATUS()) */
#define TLS_EVENT_STATUS_PASSED                               0u
#define TLS_EVENT_STATUS_FAILED                               1u
#define TLS_EVENT_READ_ROOT_CERT_FROM_NVM                     0u  /* reading a certain cert from NVM failed finally */
#define TLS_EVENT_READ_CRL_FROM_NVM                           1u  /* reading a certain CRL from NVM failed finally */
#define TLS_EVENT_READING_ANY_ROOT_CERT_FROM_NVM              2u  /* reading root cert blocks from NVM finished, but no single root cert was found */
#define TLS_EVENT_READING_ANY_CRL_FROM_NVM                    3u  /* reading CRL blocks from NVM finished, but no single CRL was found */
#define TLS_EVENT_AES_MSG_DECRYPTION                          4u  /* decrypting message usig AES failed */
#define TLS_EVENT_AES_MSG_ENCRYPTION                          5u  /* encrypting message usig AES failed */
#define TLS_EVENT_PEER_ALLOWS_SENDING_HEARTBEAT_MSG           6u  /* heartbeat agreement failed, so no heartbeat messages will be sent */
#define TLS_EVENT_LOOKING_FOR_CERT_IN_CRL                     7u  /* looking for a cert in the CRL failed */
#define TLS_EVENT_ROOT_CERT_VALIDITY_TIME_CHECK               8u  /* the root certificate is currently not inside its validity time interval */

/* TLS state */
#define TLS_STATE_UNINIT                            0x00u
#define TLS_STATE_INIT                              0x01u

/* (supported) TLS cipher suites */
#define TLS_NULL_WITH_NULL_NULL                   0x0000u
#define TLS_RSA_WITH_NULL_SHA                     0x0002u
#define TLS_RSA_WITH_AES_128_CBC_SHA              0x002Fu
#define TLS_RSA_WITH_NULL_SHA256                  0x003Bu
#define TLS_RSA_WITH_AES_128_CBC_SHA256           0x003Cu
#define TLS_ECDH_ECDSA_WITH_NULL_SHA              0xC001u
#define TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA       0xC004u
#define TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA      0xC009u
#define TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256    0xC025u
#define TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256   0xC023u
/* there is no cipher suite TLS_ECDH_ECDSA_WITH_NULL_SHA256 defined in any RFC (status March 2016) */
#define TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256    0xC02Du
#define TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256   0xC02Bu

#define TLS_MAX_RSA_SIGN_SIZE_BYTE                   256u

#if ((defined TLS_SUPPORT_SCC_ERROR_EVENT_CBK) && (TLS_SUPPORT_SCC_ERROR_EVENT_CBK == STD_ON))
/* error indication IDs */
#define TLS_E_SERVER_CERT_EXPIRED             0x01U
#define TLS_E_OCSP_RESP_NOT_GOOD              0x02U
#define TLS_E_UNKNOWN_CA                      0x03U
#define TLS_E_SERVER_TERMINATED               0x04U
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define TLS_START_SEC_CODE
#include "MemMap.h"
/***********************************************************************************************************************
 *  Tls_InitMemory
 **********************************************************************************************************************/
/*! \brief         Initializes global variable of the TLS module.
 *  \details       This function initialized the global state variable to 'uninitialized' so that other functions can
 *                 execute checks against the init state of the module.
 *  \param[in]     void             none
 *  \return        void
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \note          This function is an extension to AUTOSAR. It has to be called before any other calls to the module.
 **********************************************************************************************************************/
FUNC(void, TLS_CODE) Tls_InitMemory(void);

/***********************************************************************************************************************
 *  Tls_Init
 **********************************************************************************************************************/
/*! \brief         Initializes the TLS module.
 *  \details       this function is used to initialize the data structures.
 *  \param[in]     CfgPtr             pointer to module configuration
 *  \return        void
 *  \pre           The module 'Crypto' has to be initialized.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \note          Has to be called before usage of the module.
 *  \trace         CREQ-107316
 **********************************************************************************************************************/
FUNC(void, TLS_CODE) Tls_Init(P2CONST(Tls_ConfigType, TLS_PBCFG, TLS_CONST) CfgPtr);

/***********************************************************************************************************************
 *  Tls_GetVersionInfo
 **********************************************************************************************************************/
/*! \brief         Returns the version information
 *  \details       Tls_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *                 The Module ID is set to 255 (CDD).
 *  \param[in]     VersionInfoPtr        Pointer to where to store the version information. Parameter must not be NULL.
 *  \return        void
 *  \pre           -
 *  \context       TASK|ISR
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-107318
 **********************************************************************************************************************/
#if (TLS_VERSION_INFO_API == STD_ON)
FUNC(void, TLS_CODE) Tls_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, TLS_APPL_DATA) VersionInfoPtr );
#endif
  /* (TLS_VERSION_INFO_API == STD_ON) */

/***********************************************************************************************************************
 *  Tls_GetNvmBlockIdForUsedRootCert
 **********************************************************************************************************************/
/*! \brief         Get the NVM block ID of the root certificate used for a TLS connection.
 *  \details       During the TLS connection setup the server has sent its certificate. This certificate is
 *                 authenticated against one of the stored root certificates. This function returns the NVM block ID of
*                  the used root certificate.
 *  \param[in]     SocketId              Socket id
 *  \param[out]    RootCertBlockIdPtr    Pointer to the block ID
 *  \return        E_OK                  Block ID can be provided <br>
 *  \return        E_NOT_OK              request failed
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          -
 *  \trace         CREQ-121570
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TLS_CODE) Tls_GetNvmBlockIdForUsedRootCert(
    Tls_SocketIdType SocketId,
    P2VAR(NvM_BlockIdType, AUTOMATIC, TLS_APPL_DATA) RootCertBlockIdPtr );

/***********************************************************************************************************************
 *  Tls_RootCertWasModified
 **********************************************************************************************************************/
/*! \brief         Inform the TLS that a root cert has been updated in NVM.
 *  \details       -
 *  \param[in]     NvmBlockId            BlockId of the NVM block that has changed
 *  \return        E_OK                  Block is used as a root certificate
 *  \return        E_NOT_OK              request failed, block is not used as a root certificate
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          -
 *  \trace         CREQ-121571
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TLS_CODE) Tls_RootCertWasModified(
    NvM_BlockIdType NvmBlockId );

#if (TLS_SUPPORT_CRL == STD_ON)
/***********************************************************************************************************************
 *  Tls_CrlWasModified
 **********************************************************************************************************************/
/*! \brief         Inform the TLS that a CRL has been updated in NVM.
 *  \details       -
 *  \param[in]     NvmBlockId            BlockId of the NVM block that has changed
 *  \return        E_OK                  Block is used as a CRL
 *  \return        E_NOT_OK              request failed, block is not used as a CRL
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          -
 *  \trace         CREQ-121572
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TLS_CODE) Tls_CrlWasModified(
    NvM_BlockIdType NvmBlockId );
#endif

/***********************************************************************************************************************
 *  Tls_MainFunction
 **********************************************************************************************************************/
/*! \brief         Handle the handshake and other state dependent actions.
 *  \details       This function handles the different states to establish TLS connections, parses stored root
 *                 certificates and CRLs, transmitts data and forwards data to the upper layer.
 *  \param[in]     void             none
 *  \return        void
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          -
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Tls_MainFunctionLowPrio
 **********************************************************************************************************************/
/*! \brief         Main function to do all crypto computations.<br>
 *  \details       This function shall be called from a lower priority task. The execution time can be very long in case
 *                 of complex crypto calculations.
 *  \param[in]     void             none
 *  \return        void
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          -
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Tls_GetSocket
 **********************************************************************************************************************/
/*! \brief         Request the preparation of one connection for a socket.
 *  \details       Find a free connection mapping and bind it to the requested socket.
 *  \param[in]     SocketId           The used socket index
 *  \return        E_OK               The request has been accepted.
 *  \return        E_NOT_OK           The request has not been accepted: no free connection, or there is already a
 *                                    connection for this socket.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          -
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TLS_CODE) Tls_GetSocket(
    TcpIp_SocketIdType SocketId );

/***********************************************************************************************************************
 *  Tls_ChangeParameter
 **********************************************************************************************************************/
/*! \brief         Set a parameter to change the configuration of a socket / connection.
 *  \details       -
 *  \param[in]     SocketId              socket id
 *  \param[in]     ParameterId           parameter identification
 *  \param[in]     ParameterValuePtr     pointer to parameter value
 *  \return        E_OK                  parameter changed <br>
 *                 E_NOT_OK              parameter change failed
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          -
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TLS_CODE) Tls_ChangeParameter(
    Tls_SocketIdType SocketId,
    TcpIp_ParamIdType ParameterId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) ParameterValuePtr );

/***********************************************************************************************************************
 *  Tls_SetClientCertInfo
 **********************************************************************************************************************/
/*! \brief         Set the client certificate and key block ids (necessary for client authentication).
 *  \details       -
 *  \param[in]     SocketId       socket id
 *  \param[in]     CertPtr        pointer to client certificate (RAM block)
 *  \param[in]     KeyPtr         pointer to client private key (RAM block)
 *  \param[in]     CertLen        length of client certificate
 *  \param[in]     KeyLen         length of client private key
 *  \return        E_OK           cert info could be set<br>
 *                 E_NOT_OK       cert info could NOT be set
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          -
 *  \trace         CREQ-121569
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TLS_CODE) Tls_SetClientCertInfo(
    Tls_SocketIdType SocketId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) CertPtr,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) KeyPtr,
    uint16 CertLen,
    uint16 KeyLen );

/***********************************************************************************************************************
 *  Tls_Close
 **********************************************************************************************************************/
/*! \brief         Close TLS connection / socket
 *  \details       -
 *  \param[in]     SocketId        socket handle
 *  \param[in]     Abort          TRUE: connection will immediately be terminated by TCP, and TLS can reset all its connection variables immediatelly <br>
 *                                FALSE: connection will be terminated after performing a regular connection termination
 *                                  handshake and releasing all related resources.
 *  \return        E_OK           Close trigger could be accepted<br>
 *                 E_NOT_OK       Close trigger could NOT be accepted
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TLS_CODE) Tls_Close(
    Tls_SocketIdType SocketId,
    boolean Abort );

/***********************************************************************************************************************
 *  Tls_ProvideTxBuffer
 **********************************************************************************************************************/
/*! \brief         Request tx buffer for sending data.
 *  \details       Provide a tx buffer if available.
 *  \param[in]     SocketId            Socket id
 *  \param[out]    PbufPtr             Pointer for the data
 *  \param[in]     Length              Data length in bytes
 *  \return        BUFREQ_OK           Buffer could be provided
 *  \return        BUFREQ_E_NOT_OK     Provide buffer failed
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(BufReq_ReturnType, TLS_CODE) Tls_ProvideTxBuffer(
    Tls_SocketIdType SocketId,
    P2VAR(Tls_PbufType, AUTOMATIC, TLS_APPL_DATA) *PbufPtr,
    uint32 Length );

/***********************************************************************************************************************
 *  Tls_TransmitTo
 **********************************************************************************************************************/
/*! \brief         Trigger sending of the data or free the requested buffer.
 *  \details       -
 *  \param[in]     SocketId            Socket id
 *  \param[out]    PbufPtr             Pointer of the data
 *  \return        E_OK                Data transmission request could be accepted
 *  \return        E_NOT_OK            Data transmission request could NOT be accepted
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TLS_CODE) Tls_TransmitTo(
    Tls_SocketIdType SocketId,
    P2CONST(Tls_PbufType, AUTOMATIC, TLS_CONST) PbufPtr );

/***********************************************************************************************************************
 *  Tls_Received
 **********************************************************************************************************************/
/*! \brief         Acknowlage received data (to the stack).
 *  \details       The memory containing the received (decrypted) data is released by this function. Then the memory can
 *                 be used to receive the next frame.
 *  \param[in]     SocketId            Socket id
 *  \param[in]     Length              Length in bytes
 *  \return        E_OK                Data reception acknowledged
 *  \return        E_NOT_OK            Data reception acknowledge failed
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TLS_CODE) Tls_Received(
    Tls_SocketIdType SocketId,
    uint32 Length );

#if ((defined TLS_SUPPORT_MASTER_SECRET_ACCESS) && (TLS_SUPPORT_MASTER_SECRET_ACCESS == STD_ON))
/***********************************************************************************************************************
 *  Tls_GetMasterSecret
 **********************************************************************************************************************/
/*! \brief         Get the master secret for a TLS connection.
 *  \details       -
 *  \param[in]     SocketId            Socket id
 *  \param[out]    MsDataPtr           Pointer to the master secret
 *  \param[out]    MsLenPtr            Pointer where the length of the master secret shall be written to
 *  \return        E_OK                Access to the master secret was successful
 *  \return        E_NOT_OK            Access to the master secret failed
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
FUNC(Std_ReturnType, TLS_CODE) Tls_GetMasterSecret(
    Tls_SocketIdType SocketId,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) *MsDataPtr,
    P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) MsLenPtr );
#endif /* TLS_SUPPORT_MASTER_SECRET_ACCESS */

#define TLS_STOP_SEC_CODE
#include "MemMap.h"
#endif
  /* TLS_H */
/**********************************************************************************************************************
 *  END OF FILE: Tls.h
 *********************************************************************************************************************/
