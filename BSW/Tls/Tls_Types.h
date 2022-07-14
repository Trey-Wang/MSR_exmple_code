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
 *         File:  Tls_Types.h
 *      Project:  OnBoardCharger
 *       Module:  Transport Layer Security
 *    Generator:  SysSercice_Tls.dll
 *
 *  Description:  types header
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the Tls module. >> Tls.h
 *********************************************************************************************************************/

#if !defined (TLS_TYPES_H)  /* PRQA S 0883 */ /* MD_TLS_19.15_0883 */
# define TLS_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Tls_Cfg.h"
#include "TcpIp_Types.h"

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8     Tls_StateType;   /* Tls needs state also without DET */
typedef uint8     Tls_EncryptionStateType;
typedef uint8     Tls_TConnIdType;

typedef struct
{
  uint16 ContOffs;
  uint16 ContLen;
} Tls_Cert_EleInfo;

#define TLS_CERT_INT_HASH_LEN                    8u

#define TLS_HASH_SHA1_LEN                       20u
#define TLS_HASH_SHA256_LEN                     32u
#define TLS_HASH_MD5_LEN                        16u
#define TLS_HASH_NULL_LEN                        0u

typedef struct
{
  uint32           CertFoundElements; /* chapters successfully found in the certificate */
  uint8            Hash8Sha1Issuer[TLS_CERT_INT_HASH_LEN];   /* sha1 hash of issuer (first 8 byte) */
  uint8            Hash8Sha1Subject[TLS_CERT_INT_HASH_LEN];  /* sha1 hash of subject (first 8 byte) */
  uint8            HashSha1Cert[TLS_HASH_SHA1_LEN];          /* sha1 hash of the whole certificate */
  Tls_Cert_EleInfo CertCh1;           /* chapter 1 */
  Tls_Cert_EleInfo SignedCert;        /* chapter 1.1 */
#if (TLS_SUPPORT_CRL == STD_ON)
  Tls_Cert_EleInfo SerialNumber;      /* chapter 1.1.2 */
#endif
  Tls_Cert_EleInfo MySignAlgId;       /* chapter 1.1.3 */
  Tls_Cert_EleInfo Issuer;            /* chapter 1.1.4 */
  Tls_Cert_EleInfo ValidNotBefore;    /* chapter 1.1.5.1 */
  Tls_Cert_EleInfo ValidNotAfter;     /* chapter 1.1.5.2 */
  Tls_Cert_EleInfo Subject;           /* chapter 1.1.6 */
  Tls_Cert_EleInfo KeyAlgId;          /* chapter 1.1.7.1.1 */
  Tls_Cert_EleInfo KeyMain;           /* chapter 1.1.7.2_1.1  -- RSA modulus, ECC key */
  Tls_Cert_EleInfo KeyExp;            /* chapter 1.1.7.2_1.2  -- RSA exponent */
  Tls_Cert_EleInfo EccCurveId;        /* chapter 1.1.7.1.2    -- RSA exponent */
  Tls_Cert_EleInfo CertSignAlgId;     /* chapter 1.2 */
  Tls_Cert_EleInfo CertSignature;     /* chapter 1.3 */
  boolean          CertWasParsed;     /* certificate has been read and parsed */
} Tls_Cert_Descr;

#if (TLS_SUPPORT_CRL == STD_ON)
typedef struct
{
  uint32           CrlFoundElements; /* chapters successfully found in the CRL */
  uint8            Hash8Sha1Issuer[TLS_CERT_INT_HASH_LEN];   /* sha1 hash of issuer (first 8 byte) */
  boolean          CrlWasParsed;      /* CRL has been read and parsed */
} Tls_Crl_Descr;

/* number of elements in this list is 'TLS_CRL_NUM_OF_CRL_CHAPTERS' */
#define TLS_CRL_ELE_CRL_CHAPTER_1        0
#define TLS_CRL_ELE_TBS_CERT_LIST        1
#define TLS_CRL_ELE_MY_SIGN_ALG_ID       2
#define TLS_CRL_ELE_ISSUER               3
#define TLS_CRL_ELE_THIS_UPDATE          4
#define TLS_CRL_ELE_NEXT_UPDATE          5
#define TLS_CRL_ELE_REVOKED_CERTS        6
#define TLS_CRL_ELE_EXTENSIONS           7
#define TLS_CRL_ELE_CRL_SIGN_ALG_ID      8
#define TLS_CRL_ELE_CRL_SIGN             9
#endif

#define TLS_KEY_ALG_TYPE_INVALID    0U
#define TLS_KEY_ALG_TYPE_RSA        1U
#define TLS_KEY_ALG_TYPE_ECC        2U

typedef struct
{
  uint16 ContOffs;
  uint16 ContLen;
} Tls_Ocsp_RespEleInfoType;

typedef struct
{
  uint32           RespFoundElements; /* chapters successfully found in the certificate */
  Tls_Ocsp_RespEleInfoType Response;              /* chapter 1 */
  Tls_Ocsp_RespEleInfoType ResponseStatus;        /* chapter 1.1 */
  Tls_Ocsp_RespEleInfoType RespTypeOid;           /* chapter 1.2.1.1 */
  Tls_Ocsp_RespEleInfoType BasicOcspResp;         /* chapter 1.2.1.2 */

  Tls_Ocsp_RespEleInfoType TbsRespDataFull;       /* chapter 1.2.1.2___1.1   tbsResponseDataFull, element including length field */
  Tls_Ocsp_RespEleInfoType TbsRespData;           /* chapter 1.2.1.2___1.1   tbsResponseData */
  Tls_Ocsp_RespEleInfoType ResponderId;           /* chapter 1.2.1.2___1.1.1   responderId */
  Tls_Ocsp_RespEleInfoType ProducedAt;            /* chapter 1.2.1.2___1.1.2   producedAt */
  Tls_Ocsp_RespEleInfoType Responses;             /* chapter 1.2.1.2___1.1.3   responses */
  Tls_Ocsp_RespEleInfoType Response1;             /* chapter 1.2.1.2___1.1.3.1   response1 */
  Tls_Ocsp_RespEleInfoType SignAlgId;             /* chapter 1.2.1.2___1.2   signatureAlgorithm */
  Tls_Ocsp_RespEleInfoType Signature;             /* chapter 1.2.1.2___1.3   signature */
  Tls_Ocsp_RespEleInfoType Certs;                 /* chapter 1.2.1.2___1.4   certs */  /* OPTIONAL */
  Tls_Ocsp_RespEleInfoType Cert1;                 /* chapter 1.2.1.2___1.4.1   cert1 */  /* OPTIONAL */

  Tls_Ocsp_RespEleInfoType R1CertId;              /* chapter 1.2.1.2___1.1.3.1...1   r1_certID */
  Tls_Ocsp_RespEleInfoType R1Cid_HashAlg;         /* chapter 1.2.1.2___1.1.3.1...1.1   r1_hashAlgorithm */
  Tls_Ocsp_RespEleInfoType R1Cid_IssuerNameHash;  /* chapter 1.2.1.2___1.1.3.1...1.2   r1_issuerNameHash */
  Tls_Ocsp_RespEleInfoType R1Cid_IssuerKeyHash;   /* chapter 1.2.1.2___1.1.3.1...1.3   r1_issuerKeyHash */
  Tls_Ocsp_RespEleInfoType R1Cid_serialNumber;    /* chapter 1.2.1.2___1.1.3.1...1.4   r1_serialNumber */
  Tls_Ocsp_RespEleInfoType R1CertStatus;          /* chapter 1.2.1.2___1.1.3.1...2   r1_certStatus */
  Tls_Ocsp_RespEleInfoType R1RevocationTime;      /* chapter 1.2.1.2___1.1.3.1...2.1   r1_revocationTime */  /* only for status 'revoked' */
  Tls_Ocsp_RespEleInfoType R1ThisUpdate;          /* chapter 1.2.1.2___1.1.3.1...3   r1_thisUpdate */
  Tls_Ocsp_RespEleInfoType R1NextUpdate;          /* chapter 1.2.1.2___1.1.3.1...4   r1_nextUpdate */  /* OPTIONAL */

  uint8            ResponderIdType;      /* 0xA1 = 'by name' */
  uint8            R1CertStatusValue;    /* good, revoked, unknown */
} Tls_Ocsp_RespDescrType;
/* status parsing elements (used in StatusDesc): */
#define TLS_STATUS_ELE_RESPONSE               0x00000001u
#define TLS_STATUS_ELE_RESPONSE_STATUS        0x00000002u
#define TLS_STATUS_ELE_RESP_TYPE_OID          0x00000004u
#define TLS_STATUS_ELE_BASIC_OCSP_RESP        0x00000008u

#define TLS_STATUS_ELE_TBS_RESP_DATA          0x00000010u
#define TLS_STATUS_ELE_RESPONDER_ID           0x00000020u
#define TLS_STATUS_ELE_PRODUCED_AT            0x00000040u
#define TLS_STATUS_ELE_RESPONSES              0x00000080u
#define TLS_STATUS_ELE_RESPONSE_1             0x00000100u
#define TLS_STATUS_ELE_SIGN_ALG_ID            0x00000200u
#define TLS_STATUS_ELE_SIGNATURE              0x00000400u
#define TLS_STATUS_ELE_CERTS                  0x00000800u

#define TLS_STATUS_ELE_CERT1                  0x00200000u

#define TLS_STATUS_ELE_R1_CERT_ID             0x00001000u
#define TLS_STATUS_ELE_R1_CID_HASH_ALG        0x00002000u
#define TLS_STATUS_ELE_R1_CID_ISSU_NAME_HASH  0x00004000u
#define TLS_STATUS_ELE_R1_CID_ISSU_KEY_HASH   0x00008000u
#define TLS_STATUS_ELE_R1_CID_SERIAL_NUMBER   0x00010000u
#define TLS_STATUS_ELE_R1_CERT_STATUS         0x00020000u
#define TLS_STATUS_ELE_R1_REVOC_TIME          0x00040000u
#define TLS_STATUS_ELE_R1_THIS_UPDATE         0x00080000u
#define TLS_STATUS_ELE_R1_NEXT_UPDATE         0x00100000u

/* status values used for OCSP status messages */
#define TLS_STATUS_VALUE_GOOD                 0x00u
#define TLS_STATUS_VALUE_REVOKED              0x01u
#define TLS_STATUS_VALUE_UNKNOWN              0x02u

#define Tls_SockAddrType    TcpIp_SockAddrType
#define Tls_SockAddrIn6Type TcpIp_SockAddrInet6Type
#define Tls_PbufType        TcpIp_PbufType
#define Tls_SocketIdType      TcpIp_SocketIdType

typedef struct
{
  uint32 Date;  /* YYYYMMDD */
  uint32 Time;  /* DayligthSavingTime & --HHMMSS (UTC time) */
} Tls_TimeStampType;

typedef uint8      Tls_ConfigType;

typedef uint8      Tls_HeartbeatModeType;
#define TLS_HB_MODE_NONE                            0u  /* heartbeat disabled */
#define TLS_HB_PEER_ALLOWED_TO_SEND                 1u  /* peer_allowed_to_send */
#define TLS_HB_PEER_NOT_ALLOWED_TO_SEND             2u  /* peer_not_allowed_to_send */

typedef uint8      Tls_HeartbeatStatusType;
#define TLS_HEARTBEAT_STATUS_DISABLED               0u  /* usage of heartbeat is disabled */
#define TLS_HEARTBEAT_STATUS_COM_ACTIV              1u  /* normal communication is active, idle timeout has not been reached */
#define TLS_HEARTBEAT_STATUS_WAIT_FOR_RESPONSE      2u  /* heartbeat request has been sent, wait for the heartbeat response */
#define TLS_HEARTBEAT_STATUS_RESPONSE_TIMED_OUT     3u  /* no heartbeat response has been received in time */

#endif 
  /* TLS_TYPES_H */
/**********************************************************************************************************************
 *  END OF FILE: Tls_Types.h
 *********************************************************************************************************************/
