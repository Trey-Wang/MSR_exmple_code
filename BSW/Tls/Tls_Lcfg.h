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
 *         File:  Tls_Lcfg.h
 *      Project:  OnBoardCharger
 *       Module:  Transport Layer Security
 *    Generator:  SysSercice_Tls.dll
 *
 *  Description:  link-time configuration header
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the Tls module. >> Tls.h
 *********************************************************************************************************************/

#if !defined (TLS_LCFG_H)  /* PRQA S 0883 */ /* MD_TLS_19.15_0883 */
# define TLS_LCFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Tls_Types.h"
#include "TcpIp_Types.h"
#include "IpBase_Types.h"
#include "Crypto_Types.h"
/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */
#include "NvM.h"

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef struct {
uint16 cipher_suite;  /* parameter added by Vector Informatik */
/* ConnectionEnd entity; */ /* is always 'client' */
/* PRFAlgorithm prf_algorithm; */ /* is always 'tls_prf_sha256' -- TLS1.2 only */
uint8 bulk_cipher_algorithm;  /* { null, rc4, 3des, aes } */
uint8 cipher_type; /* { stream, block, aead } */   /* -- not used, only block type supported (except NULL) */
uint8 enc_key_length; /* TLS1.0: key_size */
uint8 enc_block_length;
/* uint8 fixed_iv_length; */ /* TLS1.2 only */
/* uint8 record_iv_length; */ /* TLS1.2 only */
uint8 mac_algorithm; /* { null, hmac_md5, hmac_sha1, hmac_sha256, hmac_sha384, hmac_sha512 } */
uint8 mac_length;
/* uint8 mac_key_length; */ /* TLS1.2 only, seems to be always the same as mac_length */
/* uint8 compression_algorithm; */ /* { null(0), (255) } */
uint8 master_secret[48];
uint8 client_random[32];
uint8 server_random[32];

/* Vector Addon: */
P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) MacPtr;  /* client/server write MAC key */
P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) KeyPtr;  /* client/server write encryption key */
P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) IvPtr;   /* client/server write IV */
} Tls_SecurityParametersType; /* RFC 5246, page 18 */

typedef uint8 Tls_KeyMaterialType[64];  /* max size for all supported / implemented cipher suites, including SHA256 MAC */
typedef uint8 Tls_SessionIdType[32];
typedef uint8 Tls_RsaKey1024Type[128];
typedef uint8 Tls_RsaKey2048Type[256];
typedef uint8 Tls_RsaExponentType[3];

typedef struct {
  uint32 LowerVal;
  uint32 UpperVal;
} Tls_SeqNumType;

/* segmented buffer, used for combining received data segments forwarded from TCP */
typedef struct
{
  P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA)  DataPtr;
  uint16                                      Len;
} Tls_BufSegType;

typedef struct
{
  Tls_BufSegType BufSegs[TLS_VBUF_SEG_NUM_TOTAL];
  uint16          Idx;  /* start index */
  uint16          Lvl;  /* fill level */
} Tls_BufSegMgtType;

#if (TLS_SUPPORT_OCSP == STD_ON)
/* Specify the operation mode for OCSP. (uint8 Tls_OcspWorkingMode[])
   client_hello: When sending the client_hello both V1 and V2 can be supported in parrallel, therefore three active
                 modes are necessary (V1, V2, V1+V2).
   server_hello: Dependent on the OCSP version the server supports, the server_hello will contain either the
                 status_request extension or the status_request_v2 extension, or it won't contain any extension if it
                 does not support OCSP at all. If the server answers with the V2 extension, there still can be both an
                 ocsp response or an ocsp_multi response. Set mode to either v1 or v2.
    server_cert_status: The server sends one OCSP response, dependent on its value of StatusType the sockets operation
                 mode will be set to USE_OCSP_STD or to USE_OCSP_MULTI.
*/
#define TLS_OCSP_MODE_OFF              0U
#define TLS_OCSP_MODE_SUPPORT_V1       1U
#define TLS_OCSP_MODE_SUPPORT_V2       2U
#define TLS_OCSP_MODE_SUPPORT_V1_V2    3U
#define TLS_OCSP_MODE_USE_OCSP_STD     1U
#define TLS_OCSP_MODE_USE_OCSP_MULTI   2U

/* user-selected OCSP support (uint8 Tls_OcspUserRequestedSupport[]) */
typedef uint8 Tls_OcspOperationModeType;
#define TLS_OCSP_SUPPORT_OFF           0U
#define TLS_OCSP_SUPPORT_V1            1U
#define TLS_OCSP_SUPPORT_V2            2U
#define TLS_OCSP_SUPPORT_V1_V2         3U

/* used for Tls_OcspRequestState */
#define TLS_OCSP_REQ_STATE_GOOD          0U  /* ocsp checks finished, all checks were passed successfully */
#define TLS_OCSP_REQ_STATE_REVOKED       1U  /* ocsp checks finished, one (or more) cert was revoked */
#define TLS_OCSP_REQ_STATE_UNKNOWN       2U  /* ocsp checks finished, one (or more) cert was unknown to the OCSP responder */
#define TLS_OCSP_REQ_STATE_NO_ANSWER     3U  /* ocsp checks finished, one (or more) cert did not receive an OCSP answer */
#endif

#define TLS_CRL_NUM_OF_CRL_CHAPTERS     10U
#define TLS_CRL_MAX_CHAPTER_DEPTH        3U

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
/* PRQA S 3684 DECLWITHOUSIZE */ /* MD_TLS_8.12_3684 */

/* RAM ------------------- */

#define TLS_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern VAR(Tls_SecurityParametersType, TLS_VAR_NOINIT)  Tls_SecParam[];  /* 4 per connection */
extern P2VAR(Tls_SecurityParametersType, AUTOMATIC, TLS_VAR_NOINIT) Tls_SecParamTxActivePtr[];
extern P2VAR(Tls_SecurityParametersType, AUTOMATIC, TLS_VAR_NOINIT) Tls_SecParamTxPassivePtr[];
extern P2VAR(Tls_SecurityParametersType, AUTOMATIC, TLS_VAR_NOINIT) Tls_SecParamRxActivePtr[];
extern P2VAR(Tls_SecurityParametersType, AUTOMATIC, TLS_VAR_NOINIT) Tls_SecParamRxPassivePtr[];
extern VAR(Tls_SessionIdType, TLS_VAR_NOINIT)        Tls_SessionId[];
extern VAR(Tls_RsaExponentType, TLS_VAR_NOINIT)      Tls_RsaExp[];
extern VAR(Tls_KeyMaterialType, TLS_VAR_NOINIT)      Tls_KeyMatBlocks[];  /* 3 per connection */
extern P2VAR(Tls_KeyMaterialType, AUTOMATIC, TLS_VAR_NOINIT)        Tls_KeyMatBlockTxPtr[];
extern P2VAR(Tls_KeyMaterialType, AUTOMATIC, TLS_VAR_NOINIT)        Tls_KeyMatBlockRxActivePtr[];
extern P2VAR(Tls_KeyMaterialType, AUTOMATIC, TLS_VAR_NOINIT)        Tls_KeyMatBlockRxPassivePtr[];
extern VAR(Tls_SeqNumType, TLS_VAR_NOINIT)           Tls_TxSeqNum[];
extern VAR(Tls_SeqNumType, TLS_VAR_NOINIT)           Tls_RxSeqNum[];
extern VAR(Tls_EncryptionStateType, TLS_VAR_NOINIT)  Tls_AESEncryptionState[];
extern VAR(Tls_EncryptionStateType, TLS_VAR_NOINIT)  Tls_AESDecryptionState[];
#if (TLS_SUPPORT_AES_GCM == STD_ON)
extern VAR(eslt_WorkSpaceGCM, TLS_VAR_NOINIT) Tls_WorkSpaceAes[];
#else
extern VAR(eslt_WorkSpaceAES128, TLS_VAR_NOINIT)     Tls_WorkSpaceAes[];
#endif

extern VAR(Crypto_HmacSha256StoreType, TLS_VAR_NOINIT)   Tls_WorkSpaceHmac[]; /* used for HMAC in RL, max size for all supported HMAC algorithms (cipher suites) */

extern VAR(eslt_WorkSpaceSHA256, TLS_VAR_NOINIT)       Tls_WorkSpaceShaX_Hs1[]; /* used for hash (SHA1 or SHA256) in handshake messages */ /* TLS version dependent */
extern VAR(eslt_WorkSpaceSHA256, TLS_VAR_NOINIT)       Tls_WorkSpaceShaX_Hs2[]; /* copy of hs1 */ /* TLS version dependent */

#if (TLS_SUPPORT_PROT_VER_TLS10 == STD_ON)
extern VAR(eslt_WorkSpaceMD5, TLS_VAR_NOINIT)        Tls_WorkSpaceMd5_Hs1[];  /* used for hash (MD5) in handshake messages */ /* TLS version dependent, 1.0 only */
extern VAR(eslt_WorkSpaceMD5, TLS_VAR_NOINIT)        Tls_WorkSpaceMd5_Hs2[];  /* copy of hs1 */ /* TLS version dependent, 1.0 only */
#endif
extern VAR(IpBase_PbufType, TLS_VAR_NOINIT)          Tls_TxPbufList[];

extern VAR(boolean, TLS_VAR_NOINIT)                  Tls_ClientCertRequested[];

extern VAR(Tls_Cert_Descr, TLS_VAR_NOINIT) Tls_ServerCertDescs[];  /* certificate descriptors */
extern VAR(Tls_Cert_Descr, TLS_VAR_NOINIT) Tls_RxCertDescs[];      /* certificate descriptors, two per connection */
extern P2VAR(Tls_Cert_Descr, AUTOMATIC, TLS_VAR_NOINIT) Tls_RxCertDescCurrentPtr[];  /* pointer to the descriptor of the current certificate */
extern P2VAR(Tls_Cert_Descr, AUTOMATIC, TLS_VAR_NOINIT) Tls_RxCertDescChildPtr[];    /* pointer to the descriptor of the child certificate */

#if (TLS_SUPPORT_CRL == STD_ON)
extern VAR(Tls_Crl_Descr, TLS_VAR_NOINIT)        Tls_CrlDescs[];   /* CRL desciptors */
extern VAR(uint8, TLS_VAR_NOINIT)                Tls_CrlValidationState[];  /* one per connection */
#define TLS_CRL_VALIDATION_IDLE                      0
#define TLS_CRL_VALIDATION_START                     1
#define TLS_CRL_VALIDATION_CHOOSE_CRL                2
#define TLS_CRL_VALIDATION_GET_CRL_FROM_NVM          3
#define TLS_CRL_VALIDATION_CHECK_UPDATE_TIME_AND_SN  4
#define TLS_CRL_VALIDATION_DONE                      5
#define TLS_CRL_VALIDATION_FAILED                    6

extern VAR(NvM_BlockIdType, TLS_VAR_NOINIT)      Tls_CrlConnNvmRequBlockId[];  /* one per connection */
extern VAR(uint8_least, TLS_VAR_NOINIT)          Tls_CrlNextCrlIdToCheck[];  /* one per connection */
#define TLS_INVALID_CRL_IDX      0xFF

extern P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA)  Tls_CrlServerCertPtr[];  /* one per connection */
extern VAR(uint32, TLS_VAR_NOINIT)               Tls_CrlServerCertLen[];   /* one per connection */
extern VAR(boolean, TLS_VAR_NOINIT)              Tls_CrlMoreThanOneCert[];  /* one per connection */
extern VAR(boolean, TLS_VAR_NOINIT)              Tls_CrlIntermediateCrlChecked[];  /* one per connection */
#endif

extern VAR(IpBase_PbufType, TLS_VAR_NOINIT) Tls_RxPbufList[];
extern VAR(NvM_BlockIdType, TLS_VAR_NOINIT) Tls_ConnNvmRequBlockId[];  /* one per connection; -- 0xFFFF is invalid */
#if (TLS_SUPPORT_GET_NVM_BLOCK_ID_FOR_USED_ROOT_CERT == STD_ON)
extern VAR(NvM_BlockIdType, TLS_VAR_NOINIT) Tls_UsedRootCertNvmBlockId[];  /* root cert used to validate the servers cert */
#endif
extern VAR(Tls_SocketIdType, TLS_VAR_NOINIT) Tls_CfgTlsConn2SockHndMap[];

extern VAR(Tls_BufSegMgtType, TLS_VAR_NOINIT) Tls_RxBufSegMgt[];

typedef struct {
uint32 TxBufferSize;
uint32 RxBufferSize;
TcpIp_SocketIdType SocketId;
uint8 TlsConnectionId;
uint8 State; /* Free / InNegotiation / Active */
} Tls_Sock2ConnMapperType;
extern VAR(Tls_Sock2ConnMapperType, TLS_VAR_NOINIT) Tls_Sock2ConnMapper[];

#define TLS_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TLS_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern VAR(uint32, TLS_VAR_NOINIT) Tls_Hs_Len[];
extern VAR(uint32, TLS_VAR_NOINIT) Tls_Hs_AllCertsRemLen[];   /* remaining length for all certificates */

extern VAR(uint32, TLS_VAR_NOINIT) Tls_AppDataLenWaitingForRec[];  /* Application data forwarded to appl but not yet
                    consumed. Appl has to call Tls_Recieved for this data. The whole blocking mechanism shall be done
                    using this variable. */
extern VAR(uint32, TLS_VAR_NOINIT) Tls_CryptoCalcDataLen[];  /* data length used for the pending crypto calculation */
extern VAR(uint32, TLS_VAR_NOINIT) Tls_DynCryptoWs[];        /* configrable size * num of connections */

#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
extern VAR(uint32, TLS_VAR_NOINIT) Tls_HeartbeatIdleCountdown[];  /* this is the countdown for the next heartbeat message */
extern VAR(uint32, TLS_VAR_NOINIT) Tls_HeartbeatCounter[];        /* this is a counter for heartbeat request messages */
#endif

extern VAR(uint32, TLS_VAR_NOINIT) Tls_HsServerCertChainLen[];  /* Length of the cert chain received in the server_cert message */

#define TLS_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TLS_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
extern VAR(uint16, TLS_VAR_NOINIT) Tls_Rl_TotalPayloadLen[];  /* x = (data + MAC + padding) */
extern VAR(uint16, TLS_VAR_NOINIT) Tls_Rl_RemPayloadLen[];    /* remaining len, (= data + MAC + padding) */
extern VAR(uint16, TLS_VAR_NOINIT) Tls_Rl_DecryptedDataLen[];  /* length of data after decryption (after removing of MAC and padding) */
extern VAR(uint16, TLS_VAR_NOINIT) Tls_RsaKeyLen[];
extern VAR(uint16, TLS_VAR_NOINIT) Tls_RxPlainTextFillLevel[];
extern VAR(uint16, TLS_VAR_NOINIT) Tls_RxRemainingAckLen[];  /* Length of received TCP data that has to be released later (for the current RL element?) */
extern VAR(uint16, TLS_VAR_NOINIT) Tls_TxBufRequested[];
extern VAR(uint16, TLS_VAR_NOINIT) Tls_TxConfRemaining[];
extern VAR(uint16, TLS_VAR_NOINIT) Tls_TxCipherTextSizes[];  /* filled during handshake */
extern VAR(uint16, TLS_VAR_NOINIT) Tls_TxPlainTextSizes[];   /* filled during handshake */
extern VAR(uint16, TLS_VAR_NOINIT) Tls_HsContinueReadPos[];  /* one per connection */
extern VAR(uint16, TLS_VAR_NOINIT) Tls_ClientCertLen[];      /* one per connection */
extern VAR(uint16, TLS_VAR_NOINIT) Tls_ClientKeyLen[];       /* one per connection */

extern VAR(uint16, TLS_VAR_NOINIT) Tls_ServEncKeyLen[];  /* length per ECC key component */
extern VAR(uint16, TLS_VAR_NOINIT) Tls_EccCurveId[];

#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
extern VAR(uint16, TLS_VAR_NOINIT)   Tls_HeartbeatIdlePeriodS[];  /* this is the heartbeat period value set by the user (or the default) */
#endif

#define TLS_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TLS_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if (TLS_SUPPORT_CRL == STD_ON)
extern VAR(uint8, TLS_VAR_NOINIT) Tls_CrlNvmExchData[];        /* one global buffer for CRL handling */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_CrlConnNvmRequStatus[];  /* one per connection */
#endif

extern VAR(uint8, TLS_VAR_NOINIT) Tls_NvmExchData[];          /* Buffer to read data from NVM */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_ConnState[];            /* State of the TLS handshake */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_SessionIdLen[];         /* Length of the received session id */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_RxProtocol[];           /* Received record layer protocol (handshake, alert, application data, ...) */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_Rl_CurrentPart[];       /* Currently received/analyzed part of the record layer message (Header, Payload) */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_Rl_HdrReadPos[];
extern VAR(uint8, TLS_VAR_NOINIT) Tls_HandShakeState[];
extern VAR(uint8, TLS_VAR_NOINIT) Tls_PrevHandShakeState[];
extern VAR(uint8, TLS_VAR_NOINIT) Tls_Ul_CurrentElement[];  /* current element in upper layer of TLS */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_Hs_HsType[];
extern VAR(uint8, TLS_VAR_NOINIT) Tls_Al_Level[];  /* alert level */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_Al_Type[];   /* alert type */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_RsaExpLen[];
extern VAR(uint8, TLS_VAR_NOINIT) Tls_PendingAlertId[];
extern VAR(uint8, TLS_VAR_NOINIT) Tls_PendingAlertLevel[];

extern VAR(uint8, TLS_VAR_NOINIT) Tls_ConnNvmRequStatus[];  /* one per connection; -- TLS_NVM_REQU_IDLE, ... */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_NumRemServCerts[];    /* remaining server certs to check */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_CurrentServCertId[];  /* NEW */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_Hs_KeyExchType[];   /* RSA, ECDH, ... -- NEW, one per connection, prepared */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_ServEncKey[];       /* RSA modulus or ECC key, size of the bigger supported one */ /* RSA2048 256byte, ECC256 65byte */

#if (TLS_SUPPORT_OCSP == STD_ON)
#if (TLS_CFG_OCSP_MAX_CERT_CHAIN_LEN > 1)
extern VAR(uint8, TLS_VAR_NOINIT) Tls_Ocsp_CertChainKeys[];  /* Stored public keys from received cert chain.
                   - Size: TLS_CFG_NUM_CONNECTIONS * (TLS_CFG_OCSP_MAX_CERT_CHAIN_LEN -1) * TLS_CFG_MAX_SERVER_CERT_KEY_SIZE_BYTE
                   - The size per connection is one less than the number of certs received in the message server_cert, since the key
                     information of the main server cert is already stored for other purposes. */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_Ocsp_CertKeyType[];  /* key type of the certs in the recieved cert chain (in message 'server_certificate') (without the server cert)
                   - Size: TLS_CFG_NUM_CONNECTIONS * (TLS_CFG_OCSP_MAX_CERT_CHAIN_LEN -1) */
#endif
extern VAR(uint8, TLS_VAR_NOINIT) Tls_Ocsp_CertChainLen[];  /* length of the recieved cert chain (in message 'server_certificate') */
extern VAR(Tls_OcspOperationModeType, TLS_VAR_NOINIT) Tls_OcspUserRequestedSupport[];  /* The user selects the ocsp version to support. Default is TLS_OCSP_ENABLE_OFF */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_OcspWorkingMode[];  /* current operation mode */
#endif

extern VAR(uint8, TLS_VAR_NOINIT) Tls_PrivEccKeyArray[];  /* ECC ephemeral priv key, only one component */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_RxBlockingState[];  /* Blocking of further interpretation of received data until the current handshake element is interpreted and necessary crypto calculations are done */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_SockHnd2TlsConnMap[];  /* socket to connection index map */  /* total number of sockets, only for cfg5 */
extern P2VAR(uint8, AUTOMATIC, TLS_VAR_NOINIT) Tls_ClientCertPtr[];  /* one per connection */
extern P2VAR(uint8, AUTOMATIC, TLS_VAR_NOINIT) Tls_ClientKeyPtr[];   /* one per connection */
extern P2VAR(uint8, AUTOMATIC, TLS_VAR_NOINIT) Tls_HsCurrDataPtr[];  /* ptr to decoded received data -- one per connection; */

extern VAR(uint8, TLS_VAR_NOINIT) Tls_TxNextIv[];           /* 16 byte per (AES) connection */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_ProtVer[];    /* TLS protocol version chosen in the handshake */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_HsHashAlg[];  /* hash type used for handshake messages (SHA256 or MD5SHA1) */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_HsHashLen[];  /* hash digest length corresponding to Tls_HsHashAlg[] */
extern VAR(uint8, TLS_VAR_NOINIT) Tls_RlRxIvFillLevel[];  /* length of already received IV data */ /* TLS version dependent, 1.2 only */

extern P2VAR(uint8, AUTOMATIC, TLS_VAR_NOINIT) Tls_RxCertCurrentPtr[];
extern P2VAR(uint8, AUTOMATIC, TLS_VAR_NOINIT) Tls_RxCertChildPtr[];
extern VAR(uint8, TLS_VAR_NOINIT)              Tls_CryptoCalcPending[];  /* indicate which crypto calculation currently is pending */
extern P2VAR(uint8, AUTOMATIC, TLS_VAR_NOINIT) Tls_CryptoCalcDataPtr[];  /* data pointer used for the pending crypto calculation */

#if (TLS_SUPPORT_AES_GCM == STD_ON)
extern VAR(uint8, TLS_VAR_NOINIT)   Tls_RxNonceExplicit[];  /* array to store the received explicit nonce (used for AES_GCM) (len = TLS_AES_GCM_RECORD_IV_LENGTH = 8), one per connection */
extern VAR(uint8, TLS_VAR_NOINIT)   Tls_GcmRxTagField[];    /* array to store the received GCM TagField (len = TLS_AES_BLOCK_LEN = 16), one per connection */
extern VAR(uint8, TLS_VAR_NOINIT)   Tls_GcmRxTagFieldFillLevel[];  /* fill level of the array Tls_GcmRxTagField[] */
#endif

#if (TLS_SUPPORT_HEARTBEAT == STD_ON)
extern VAR(Tls_HeartbeatModeType, TLS_VAR_NOINIT)     Tls_HeartbeatModeOwn[];   /* this is the heartbeat mode requested by the user, default is 'off'  */
extern VAR(Tls_HeartbeatModeType, TLS_VAR_NOINIT)     Tls_HeartbeatModePeer[];  /* this is the heartbeat mode requested by the peer */
extern VAR(Tls_HeartbeatStatusType, TLS_VAR_NOINIT)   Tls_HeartbeatStatus[];    /* this is the status of the message (idle, response pending, response missing */
#endif

extern P2VAR(uint8, AUTOMATIC, TLS_VAR_NOINIT) Tls_HsServerCertChainPtr[];  /* Pointer to the cert chain received in the server_cert message */

#define TLS_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ROM ------------------- */

#define TLS_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern CONST(Tls_ConfigType, TLS_CONST) Tls_Config;
extern CONST(NvM_BlockIdType, TLS_CONST) Tls_CfgServerCertBlockIds[];
#if (TLS_SUPPORT_CRL == STD_ON)
extern CONST(NvM_BlockIdType, TLS_CONST) Tls_CfgCrlBlockIds[];
extern CONSTP2VAR(Tls_Cert_EleInfo, AUTOMATIC, TLS_VAR_NOINIT) Tls_CfgCrlElePosArrays[];
#endif

#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TLS_START_SEC_CONST_32BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern CONST(uint32, TLS_PBCFG) Tls_CfgOcspMaxResponseAgeS[];   /* maximum age a received OCSP response may have (in seconds) */

#define TLS_STOP_SEC_CONST_32BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TLS_START_SEC_CONST_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern CONST(uint16, TLS_CONST) Tls_CfgRxPlainTextSizes[];
extern CONST(uint16, TLS_CONST) Tls_CfgTxPlainTextSizes[];   /* min sizes are 262byte for 2048bit RSA key usage */
extern CONST(uint16, TLS_CONST) Tls_CfgTxCipherTextSizes[];  /* max size is TcpIp_TxBufSizeByte, and should be a multiple of cipher block size */
extern CONST(uint16, TLS_CONST) Tls_CfgDynCryptoWsStartOffs[];
extern CONST(uint16, TLS_CONST) Tls_CfgSuppCipherSuites[];
extern CONST(uint16, TLS_PBCFG) Tls_CfgRxTcpBufSizes[];

#define TLS_STOP_SEC_CONST_16BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define TLS_START_SEC_CONST_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern CONSTP2VAR(uint8, AUTOMATIC, TLS_CONST) Tls_CfgRxPlainTextStarts[];
extern CONSTP2VAR(uint8, AUTOMATIC, TLS_CONST) Tls_CfgTxCipherTextStarts[];
extern CONSTP2VAR(uint8, AUTOMATIC, TLS_CONST) Tls_CfgTxPlainTextStarts[];

#if (TLS_SUPPORT_OCSP == STD_ON)
extern CONST(boolean, TLS_PBCFG) Tls_CfgUseOcsp[];              /* use OCSP for this connection */
#endif

#define TLS_STOP_SEC_CONST_8BIT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* -----------------------------------------------------------------------------
    &&&~ Call Back Functions
 ----------------------------------------------------------------------------- */
#define TLS_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  Tls_GetRandArrayFct
 **********************************************************************************************************************/
/*! \brief      request a random number
 *  \param[in]  TgtDataPtr            pointer to target were random number shall be written to
 *  \param[in]  TgtLen                length (byte) of the random number
 *  \context    task level
 **********************************************************************************************************************/
extern FUNC(void, TLS_CODE) Tls_GetRandArrayFct(
    P2VAR(uint8, AUTOMATIC, TLS_APPL_VAR) TgtDataPtr,
    uint16 TgtLen );

/***********************************************************************************************************************
 *  Tls_GetTime32Fct
 **********************************************************************************************************************/
/*! \brief      request the current global time (UTC)
 *  \param[in]  Time32Ptr             pointer where the time shall be written to (unix format)
 *  \context    task level
 **********************************************************************************************************************/
extern FUNC(void, TLS_CODE)Tls_GetTime32Fct(
    P2VAR(uint32, AUTOMATIC, TLS_APPL_VAR) Time32Ptr );

#define TLS_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* -----------------------------------------------------------------------------
    &&&~ Configuration Variant Dependent Variables
 ----------------------------------------------------------------------------- */

/* PRQA L:DECLWITHOUSIZE */ /* MD_TCPIP_8.12_3684 */

#endif
  /* TLS_LCFG_H */

/**********************************************************************************************************************
 *  END OF FILE: Tls_Lcfg.h
 *********************************************************************************************************************/
